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

#ifndef HELPERS_H
#define HELPERS_H

#include "Enums.h"

class VersionCode;

namespace Conflux
{
    /**
     * @brief Detects the internal HDMI kit installed in the
     * Xbox console.
     * 
     * @return HdmiHardwareId ID of the installed HDMI  
     * kit. Indexed by Conflux::HdmiHardwareId.
     */
    HdmiHardwareId DetectInstalledHardware();

    /**
     * @brief Clamps a given value from a given minimum 
     * value and maximum value. Minimum and maximum value
     * are included in the valid value range.
     * 
     * @param value the value to clamp.
     * @param min minimum valid value.
     * @param max maximum valid value.
     * @return int The clamped value.
     */
    int Clamp(int value, int min, int max);

    /**
     * @brief Get the patch version of a kernel that has
     * been patched using a IPS file provided by Dustin 
     * Holden.
     * 
     * @param kernelPatchVersion Conflux::VersionCode object
     * to be filled out with the kernel patch version information.
     * @return true if the kernel patch version was found.
     * @return false otherwise.
     */
    bool GetKernelPatchVersionCode(VersionCode* kernelpatchVersion);
} // Conflux

#endif // HELPERS_H