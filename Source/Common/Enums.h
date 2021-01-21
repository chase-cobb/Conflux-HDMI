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

#ifndef ENUMS_H
#define ENUMS_H

namespace Conflux
{
    /**
     * @brief List of potential internal HDMI hardware that
     * a developer can expect to interface.
     * 
     */
    enum HdmiHardwareId
    {
        NO_HW_DETECTED,
        XBOXHDMI,
    };

    /**
     * @brief List of valid update sources.
     * 
     */
    enum UpdateSource
    {
        HDD = 1,
        DVD,
        INTERNET,
    };

    /**
     * @brief List of features that any HDMI hardware might expose
     * through the abstraction layer.
     * 
     */
    enum SupportedFeatures
    {
        NONE = 0X0000,
        CB_ADJUST = 0x0001,
        CR_ADJUST = 0x0002,
        LUMA_ADJUST = 0x0004,
        WIDESCREEN_ADJUST = 0x0008,
        VIDEO_MODE_ADJUST = 0x0010,
        LAST_ENTRY = 0x0020,
    };
}

#endif // ENUMS_H