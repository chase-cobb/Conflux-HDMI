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

#ifndef XBOXHDMI_H
#define XBOXHDMI_H

#include "HdmiInterface.h"
#include <time.h>

namespace Conflux
{
    namespace XboxHDMI
    {
        enum BootMode
        {
            BOOTROM,
            HDMI_PROGRAM,
        };

        class XboxHdmi : public HdmiInterface
        {
        public:
            XboxHdmi();
            ~XboxHdmi();
            
            bool IsFirmwareUpdateAvailable(UpdateSource updateSource);
            bool UpdateFirmware(UpdateSource updateSource);
            bool IsFeatureSupported(SupportedFeatures feature);
            bool GetFirmwareVersion(VersionCode* versionCode);
            const char* GetName();

            bool LoadConfig();
            bool UpdateConfigValues();
            bool SaveConfig();

        private:
            bool GetFirmwareCompileTime(time_t* compileTime);


            bool GetBootMode(BootMode* mode);
            bool SwitchBootMode(BootMode switchToMode);

            void StartFirmwareUpdateProcess();
            bool LoadFirmwareImage(UpdateSource updateSource, uint8_t* firmwareImage);

            // TODO : Check program status // 2 version of this??
            //bool CheckProgramStatus();
            
            bool WritePageCrc(int* firmwareFile, uint32_t offset);
            bool WritePageData(int* firmwareFile, uint32_t offset);
        };
    } // XboxHDMI
} // Conflux

#endif // XBOXHDMI_H