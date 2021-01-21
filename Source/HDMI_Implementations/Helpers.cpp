/*
Copyright 2021 Chase Cobb

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "Helpers.h"
#include "XboxHDMI_Config.h"
#include "VersionCode.h"

#include <stdint.h>
#include <xboxkrnl/xboxkrnl.h>
#include <cstring>

namespace Conflux
{
    HdmiHardwareId DetectInstalledHardware()
    {
        // Look for XboxHDMI hardware
        // HACK : This method of checking should be replaced with soemthing more
        //        scalable.
        ULONG bootMode;
        if(HalReadSMBusValue(XboxHDMI::I2C_HDMI_ADRESS, XboxHDMI::I2C_BOOT_MODE, false, &bootMode) == 0)
        {
            return HdmiHardwareId::XBOXHDMI;
        }

        return HdmiHardwareId::NO_HW_DETECTED;
    }

    int Clamp(int value, int min, int max)
    {
        int returnValue = value;

        if(value < min)
        {
            returnValue = min;
        }
        else if(value > max)
        {
            returnValue = max;
        }

        return returnValue;
    }

    bool GetKernelPatchVersionCode(VersionCode* kernelpatchVersion)
    {
        if(kernelpatchVersion != nullptr)
        {
            char tag[] = "HDMIkv";

            for(uint16_t offset = 0; offset < 0x00001000; offset++)
            {
                if(memcmp(tag, ((char *)&AvSetDisplayMode) + offset, sizeof(tag) - 1) == 0)
                {
                
                    int8_t major = (((char *)&AvSetDisplayMode) + offset)[6];
                    int8_t minor = (((char *)&AvSetDisplayMode) + offset)[7];
                    int8_t patch = (((char *)&AvSetDisplayMode) + offset)[8];

                    kernelpatchVersion->SetVersion(major, minor, patch);

                    return true;
                }
            }
        }

        return false;
    }
} // Conflux