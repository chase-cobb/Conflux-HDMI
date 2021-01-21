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

#include "VersionCode.h"
#include <sstream>
#include <hal/debug.h>

namespace Conflux
{
    VersionCode::VersionCode()
    {
        m_formattedVersion = nullptr;
        SetMajor(0);
        SetMinor(0);
        SetPatch(0);
    }

    VersionCode::~VersionCode()
    {
        if(m_formattedVersion != nullptr)
        {
            delete [] m_formattedVersion;
            m_formattedVersion = nullptr;
        }
    }

    void VersionCode::SetMajor(uint8_t major)
    {
        m_major = major;
    }

    void VersionCode::SetMinor(uint8_t minor)
    {
        m_minor = minor;
    }

    void VersionCode::SetPatch(uint8_t patch)
    {
        m_patch = patch;
    }

    void VersionCode::SetVersion(uint8_t major, uint8_t minor, uint8_t patch)
    {
        SetMajor(major);
        SetMinor(minor);
        SetPatch(patch);
    }

    const char* VersionCode::GetVersionCodeAsCString()
    {
        // Format as : xxx.xxx.xxx
        if(m_formattedVersion == nullptr)
        {
            m_formattedVersion = new char[12];
            m_formattedVersion[11] = '\0';
        }

        snprintf(m_formattedVersion, 12, "%d.%d.%d", m_major,
                                                     m_minor,
                                                     m_patch);  

        return m_formattedVersion;
    }
} // Conflux