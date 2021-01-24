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
#include <thread>
#include <windows.h>

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
            
            bool IsFirmwareUpdateAvailable(UpdateSource updateSource, const char* firmwareFilePath = "");
            bool UpdateFirmware(UpdateSource updateSource, void (*currentProcess)(const char* currentProcess)
                                                         , void (*percentComplete)(int percentageComplete)
                                                         , void (*errorMessage)(const char* errorMessage)
                                                         , void (*updateComplete)(void)
                                                         , const char* pathToFirmware = "");
            bool IsFeatureSupported(SupportedFeatures feature);
            bool GetFirmwareVersion(VersionCode* versionCode);
            const char* GetName();

            bool LoadConfig();
            bool UpdateConfigValues();
            bool SaveConfig();

        private:
            uint8_t* m_loadedFirmware;
            std::thread m_firmwareUpdateThread;

            void (*m_currentUpdateProcess)(const char* currentProcess);
            void (*m_currentPercentComplete)(int percentComplete);
            void (*m_currentErrorMessage)(const char* errorMessage);
            void (*m_updateComplete)(void);

            bool GetFirmwareCompileTime(time_t* compileTime);
            bool GetBootMode(BootMode* mode);
            bool SwitchBootMode(BootMode switchToMode);

            void StartFirmwareUpdateProcess(UpdateSource updateSource);
            bool LoadFirmwareImage(UpdateSource updateSource, uint8_t* firmwareImage, long* fileSize, const char* firmwareFilePath = "");
            
            void GeneratePageCrc(uint32_t* CrcValue, uint8_t* firmwareFile, uint32_t offset, long fileSize);
            bool WritePageCrc(uint32_t CrcValue);
            bool WritePageData(uint8_t* firmwareFile, uint32_t offset, long fileSize);
            bool CheckForProgrammingErrors(ULONG* statusValue);

            uint32_t CrcAddByte(uint32_t crc, uint8_t addByte);
            uint32_t CrcResult(uint32_t crc);
            uint32_t ReverseU32(uint32_t dataToReverse);
        };
    } // XboxHDMI
} // Conflux

#endif // XBOXHDMI_H