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

#ifndef VERSIONCODE_H
#define VERSIONCODE_H

#include <stdint.h>

namespace Conflux
{
    /**
     * @brief This class contains functionality for getting
     * and setting version numbers. Used mainly by the HDMI
     * implementations for storing their firmware versions.
     * 
     */
    class VersionCode
    {
    public:
        VersionCode();
        ~VersionCode();

        /**
         * @brief Gets the major version.
         * 
         * @return uint8_t major version.
         */
        uint8_t GetMajor() {return m_major;}

        /**
         * @brief Gets the minor version.
         * 
         * @return uint8_t minor version.
         */
        uint8_t GetMinor() {return m_minor;}

        /**
         * @brief Gets the patch version.
         * 
         * @return uint8_t patch version.
         */
        uint8_t GetPatch() {return m_patch;}

        /**
         * @brief Sets the Version
         * 
         * @param major major version.
         * @param minor minor version.
         * @param patch patch version.
         */
        void SetVersion(uint8_t major, uint8_t minor, uint8_t patch);

        /**
         * @brief Gets the full concatinated 
         * version as a c style string.
         * 
         * @return const char* full version number.
         */
        const char* GetVersionCodeAsCString();

    private:
        uint8_t m_major;
        uint8_t m_minor;
        uint8_t m_patch;
        bool m_cached;
        char* m_formattedVersion;

        void SetMajor(uint8_t major);
        void SetMinor(uint8_t minor);
        void SetPatch(uint8_t patch);
    };
} // Conflux

#endif