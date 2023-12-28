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

#include "XboxHdmi.h"
#include <chrono>
#include <stdint.h>
#include <xboxkrnl/xboxkrnl.h>
#include "XboxHDMI_Config.h"
#include "VersionCode.h"
#include "Strings.h"

namespace Conflux
{
    namespace XboxHDMI
    {
        XboxHdmi::XboxHdmi()
        {
            m_supportedFeatures = SupportedFeatures::CB_ADJUST | 
                                SupportedFeatures::CR_ADJUST |
                                SupportedFeatures::LUMA_ADJUST |
                                SupportedFeatures::WIDESCREEN_ADJUST |
                                SupportedFeatures::VIDEO_MODE_ADJUST;
            SetHardwareId(HdmiHardwareId::XBOXHDMI);

            m_loadedFirmware = nullptr;

            m_currentUpdateProcess = nullptr;
            m_currentPercentComplete = nullptr;
            m_currentErrorMessage = nullptr;
            m_updateComplete = nullptr;
        }

        XboxHdmi::~XboxHdmi()
        {
            ClearFeatureMap();
        }

        bool XboxHdmi::IsFirmwareUpdateAvailable(UpdateSource updateSource, const char* firmwareFilePath)
        {            
            bool firmwareWasFound = false;

            switch (updateSource)
            {
                case UpdateSource::HDD:
                {
                    // TODO
                    break;
                }
                case UpdateSource::DVD:
                {
                    // TODO
                    break;
                }
                case UpdateSource::INTERNET:
                {
                    // TODO
                    break;
                }
                case UpdateSource::WORKING_DIRECTORY:
                {
                    FILE* firmwareFile= fopen(DEFAULT_FIRMWARE_WORKING_DIRECTORY,
                                              DEFAULT_FIRMWARE_WORKING_DIRECTORY_OPEN_MODE);

                    if(firmwareFile)
                    {
                        fclose(firmwareFile);
                        firmwareWasFound = true;
                    }
                    break;
                }
                default:
                    break;
            }

            return firmwareWasFound;
        }

        bool XboxHdmi::UpdateFirmware(UpdateSource updateSource, void (*currentProcess)(const char* currentProcess)
                                                               , void (*percentComplete)(int percentageComplete)
                                                               , void (*errorMessage)(const char* errorMessage)
                                                               , void (*updateComplete)(bool flashSuccessful)
                                                               , const char* pathToFirmware)
        {
            m_currentUpdateProcess = currentProcess;
            m_currentPercentComplete = percentComplete;
            m_currentErrorMessage = errorMessage;
            m_updateComplete = updateComplete;

            if(m_firmwareUpdateThread.joinable())
            {
                m_firmwareUpdateThread.join();
            }

            m_firmwareUpdateThread = std::thread(&XboxHdmi::StartFirmwareUpdateProcess, this, updateSource);

            return m_firmwareUpdateThread.joinable();
        }

        bool XboxHdmi::IsFeatureSupported(SupportedFeatures feature)
        {
            return ((m_supportedFeatures & feature) != 0);
        }

        bool XboxHdmi::GetFirmwareVersion(VersionCode* versionCode)
        {
            ULONG smbusRead;
            uint8_t major, minor, patch;

            if(HalReadSMBusValue(I2C_HDMI_ADRESS, I2C_FIRMWARE_VERSION + 0, false, &smbusRead) == 0) {
                major = (uint8_t)smbusRead;

                HalReadSMBusValue(I2C_HDMI_ADRESS, I2C_FIRMWARE_VERSION + 1, false, &smbusRead);
                minor = (uint8_t)smbusRead;

                HalReadSMBusValue(I2C_HDMI_ADRESS, I2C_FIRMWARE_VERSION + 2, false, &smbusRead);
                patch = (uint8_t)smbusRead;

                // Firmware 1.0.0 will incorrectly report 0.0.0, so let's fix that.
                if(major == 0 && minor == 0 && patch == 0) {
                major = 1;
                }

                versionCode->SetVersion(major, minor, patch);
                return true;
            }
            return false;
        }

        const char* XboxHdmi::GetName()
        {
            return "XboxHDMI";
        }

        bool XboxHdmi::LoadConfig()
        {
            ULONG widescreenMode;
            ULONG videoOutMode;
            ULONG lumaSetting = 0;
            ULONG cbSetting = 0;
            ULONG crSetting = 0;

            // If one read fails it will cause all to fail
            bool readSuccessful = true;

            readSuccessful = (HalReadSMBusValue(I2C_HDMI_ADRESS, I2C_EEPROM_WIDESCREEN, false,
                                &widescreenMode) == 0);

            if(readSuccessful)
            {
            readSuccessful = (HalReadSMBusValue(I2C_HDMI_ADRESS, I2C_EEPROM_MODE_OUT, false,
                                &videoOutMode) == 0);
            }

            if(readSuccessful)
            {
            readSuccessful = (HalReadSMBusValue(I2C_HDMI_ADRESS, I2C_EEPROM_ADJUST_LUMA, false,
                                &lumaSetting) == 0);
            }
            
            if(readSuccessful)
            {
            readSuccessful = (HalReadSMBusValue(I2C_HDMI_ADRESS, I2C_EEPROM_ADJUST_CB, false,
                                &cbSetting) == 0);
            }
            
            if(readSuccessful)
            {
            readSuccessful = (HalReadSMBusValue(I2C_HDMI_ADRESS, I2C_EEPROM_ADJUST_CR, false,
                                &crSetting) == 0);
            }

            if(readSuccessful)
            {
                //Don't  allocate new RangedIntValue if they already exist in the map!!
                ClearFeatureMap();

                RangedIntValue* widescreenValue = new RangedIntValue((int8_t)widescreenMode, 0, 2, FEATURE_WIDESCREEN_ADJUST);
                m_featureValues[SupportedFeatures::WIDESCREEN_ADJUST] = widescreenValue;
                
                RangedIntValue* videoOutValue = new RangedIntValue((int8_t)videoOutMode, 0, 1, FEATURE_VIDEO_MODE_ADJUST);
                m_featureValues[SupportedFeatures::VIDEO_MODE_ADJUST] = videoOutValue;
                
                RangedIntValue* lumaValue = new RangedIntValue((int8_t)lumaSetting, -12, 12, FEATURE_LUMA_ADJUST);
                m_featureValues[SupportedFeatures::LUMA_ADJUST] = lumaValue;
                
                RangedIntValue* cbValue = new RangedIntValue((int8_t)cbSetting, -12, 12, FEATURE_CB_ADJUST);
                m_featureValues[SupportedFeatures::CB_ADJUST] = cbValue;
                
                RangedIntValue* crValue = new RangedIntValue((int8_t)crSetting, -12, 12, FEATURE_CR_ADJUST);
                m_featureValues[SupportedFeatures::CR_ADJUST] = crValue;
            }

            return readSuccessful;
        }

        bool XboxHdmi::UpdateConfigValues()
        {
            bool writeSuccessful = true;

            writeSuccessful = (HalWriteSMBusValue(I2C_HDMI_ADRESS, I2C_EEPROM_WIDESCREEN, 0,
                        (ULONG)m_featureValues[SupportedFeatures::WIDESCREEN_ADJUST]->GetValue()) == 0);

            if(writeSuccessful)
            {
                writeSuccessful = (HalWriteSMBusValue(I2C_HDMI_ADRESS, I2C_EEPROM_MODE_OUT, 0,
                        (ULONG)m_featureValues[SupportedFeatures::VIDEO_MODE_ADJUST]->GetValue()) == 0);
            }
            if(writeSuccessful)
            {
                writeSuccessful = (HalWriteSMBusValue(I2C_HDMI_ADRESS, I2C_EEPROM_ADJUST_LUMA, 0,
                        (ULONG)m_featureValues[SupportedFeatures::LUMA_ADJUST]->GetValue()) == 0);
            }
            if(writeSuccessful)
            {
                writeSuccessful = (HalWriteSMBusValue(I2C_HDMI_ADRESS, I2C_EEPROM_ADJUST_CB, 0,
                        (ULONG)m_featureValues[SupportedFeatures::CB_ADJUST]->GetValue()) == 0);
            }
            if(writeSuccessful)
            {
                writeSuccessful = (HalWriteSMBusValue(I2C_HDMI_ADRESS, I2C_EEPROM_ADJUST_CR, 0,
                        (ULONG)m_featureValues[SupportedFeatures::CR_ADJUST]->GetValue()) == 0);
            }

            return writeSuccessful;
        }

        bool XboxHdmi::SaveConfig()
        {
            return HalWriteSMBusValue(I2C_HDMI_ADRESS, I2C_EEPROM_SAVE, 0, (ULONG)0xFF) == 0;
        }

        bool XboxHdmi::GetFirmwareCompileTime(time_t* compileTime)
        {
            bool readSuccessful = true;
            ULONG compileTimeRaw[4];

            readSuccessful = HalReadSMBusValue(I2C_HDMI_ADRESS, I2C_COMPILE_TIME0, false,
                        &compileTimeRaw[0]) == 0;

            if(readSuccessful)
            {
            readSuccessful = HalReadSMBusValue(I2C_HDMI_ADRESS, I2C_COMPILE_TIME1, false,
                                &compileTimeRaw[1]) == 0;
            }
            
            if(readSuccessful)
            {
            readSuccessful = HalReadSMBusValue(I2C_HDMI_ADRESS, I2C_COMPILE_TIME2, false,
                                &compileTimeRaw[2]) == 0;
            }

            if(readSuccessful)
            {
            readSuccessful = HalReadSMBusValue(I2C_HDMI_ADRESS, I2C_COMPILE_TIME3, false,
                                &compileTimeRaw[3]) == 0;
            }

            if(readSuccessful)
            {
                *compileTime = (compileTimeRaw[0] << 24) +
                               (compileTimeRaw[1] << 16) +
                               (compileTimeRaw[2] << 8) + 
                               (compileTimeRaw[3]);
            }

            return readSuccessful;
        }

        bool XboxHdmi::GetBootMode(BootMode* mode)
        {
            ULONG currentBootMode;
            bool readSuccessful;
            readSuccessful = HalReadSMBusValue(I2C_HDMI_ADRESS, I2C_BOOT_MODE, 
                                               false, &currentBootMode) == 0;

            if(readSuccessful)
            {
                if(currentBootMode == BOOT_HDMI_PROGRAM)
                {
                    *mode = BootMode::HDMI_PROGRAM;
                }
                else if(currentBootMode == BOOT_HDMI_BOOTROM)
                {
                    *mode = BootMode::BOOTROM;
                }
                else if(currentBootMode == BOOT_HDMI_FIRMWARE)
                {
                    *mode = BootMode::HDMI_FIRMWARE;
                }
                else if(currentBootMode == BOOT_HDMI_INVALID)
                {
                    *mode = BootMode::INVALID;
                }
            }

            return readSuccessful;
        }

        void XboxHdmi::StartFirmwareUpdateProcess(UpdateSource updateSource)
        {
            BootMode bootMode;
            ULONG errorStatus;
            uint8_t* loadedFirmware = nullptr;
            long firmwareFileSize;
            int sleepBetweenOpsInSeconds = 2;
            bool flashWasSuccessful = true;

            m_currentErrorMessage("None");

            // Load Firmware
            m_currentUpdateProcess(PROG_PROCESS_LOADING_FIRMWARE);
            if(!LoadFirmwareImage(updateSource, loadedFirmware, &firmwareFileSize))
            {
                m_currentErrorMessage(PROG_ERROR_FAILED_TO_LOAD_FIRMWARE);
                m_updateComplete(false); // Early out
            }
            std::this_thread::sleep_for (std::chrono::seconds(sleepBetweenOpsInSeconds));

            // Output the firmware file size
            std::string firmwareSizeToString = PROG_PROCESS_FIRMWARE_FILE_SIZE;
            firmwareSizeToString.append(std::to_string(firmwareFileSize));
            m_currentUpdateProcess(firmwareSizeToString.c_str());
            std::this_thread::sleep_for (std::chrono::seconds(sleepBetweenOpsInSeconds));

            // Output firmware loaded!
            m_currentUpdateProcess(PROG_PROCESS_LOADED_FIRMWARE);
            std::this_thread::sleep_for (std::chrono::seconds(sleepBetweenOpsInSeconds));
            
            // Switch to bootloader
            m_currentUpdateProcess(PROG_CHECKING_BOOT_MODE);
            if(!GetBootMode(&bootMode))
            {
                m_currentErrorMessage(PROG_ERROR_UNABLE_TO_GET_BOOT_MODE);
                m_updateComplete(false); // Early out
            }
            else
            {
                if(bootMode == BootMode::HDMI_PROGRAM)
                {
                    m_currentUpdateProcess(BOOT_MODE_HDMI_PROGRAM);
                }
                else if(bootMode == BootMode::BOOTROM)
                {
                    // Even though it sometimes reports as already in bootrom
                    // at startup, we still need to switch to bootrom manually
                    // because the bootrom is loaded by swapping to the
                    // HDMI_PROGRAM boot mode to make sure the full order of
                    // operations is followed
                    m_currentUpdateProcess(BOOT_MODE_HDMI_BOOTROM);
                }
                else if(bootMode == BootMode::HDMI_FIRMWARE)
                {
                    m_currentUpdateProcess(BOOT_MODE_HDMI_FIRMWARE);
                }
                else if(bootMode == BootMode::INVALID)
                {
                    m_currentUpdateProcess(BOOT_MODE_HDMI_INVALID);
                }
            }
            std::this_thread::sleep_for (std::chrono::seconds(sleepBetweenOpsInSeconds));
            
            // XboxHDMI actually expects this to switch to bootrom
            // and not to switch to the bootrom directly
            if(!SwitchBootMode(BootMode::HDMI_PROGRAM))
            {
                m_currentErrorMessage(PROG_ERROR_UNABLE_TO_SIGNAL_BOOT_MODE);
                m_updateComplete(false); // Early out
            }
            // Waiting for boot rom
            m_currentUpdateProcess(PROG_WAITING_FOR_BOOTROM);
            std::this_thread::sleep_for (std::chrono::seconds(sleepBetweenOpsInSeconds));

            // Verifying current boot mode
            m_currentUpdateProcess(PROG_CHECKING_BOOT_MODE);
            if(GetBootMode(&bootMode))
            {
                if(bootMode == BootMode::BOOTROM)
                {
                    m_currentUpdateProcess(PROG_SWAPPED_TO_BOOTROM);
                }
                else
                {
                    m_currentErrorMessage(PROG_ERROR_UNABLE_TO_SWAP_TO_BOOTROM);
                }
                
            }
            std::this_thread::sleep_for (std::chrono::seconds(sleepBetweenOpsInSeconds));

            int totalBytesToWrite = PROGRAMMABLE_PAGES * XBOX_HDMI_PAGE_SIZE;

            // Flashing firmware
            m_currentUpdateProcess(PROG_FLASHING_FIRMWARE);
            for(uint32_t pageIndex = 0; pageIndex < PROGRAMMABLE_PAGES; )
            {
                uint32_t pageOffset = pageIndex * XBOX_HDMI_PAGE_SIZE;
                uint32_t crcValue = CRC_INIT;
                uint32_t firmwareOffset = 0;

                m_currentUpdateProcess(PROG_WRITING_PAGE_CRC);
                // Generate CRC and write it for this page
                for(uint32_t index = 0; index < XBOX_HDMI_PAGE_SIZE; ++index)
                {
                    firmwareOffset = index + pageOffset;

                    // Generate page CRC
                    GeneratePageCrc(&crcValue, loadedFirmware, firmwareOffset, firmwareFileSize);
                }

                crcValue = CrcResult(crcValue);
                if(!WritePageCrc(crcValue))
                {
                    m_currentErrorMessage(PROG_ERROR_UNABLE_TO_WRITE_CRC_DATA);
                }

                // Sleeping here is required to avoid CRC verification errors.
                std::this_thread::sleep_for (std::chrono::milliseconds(750));
                
                m_currentUpdateProcess(PROG_WRITING_PAGE_DATA);
                for(uint32_t index = 0; index < XBOX_HDMI_PAGE_SIZE; ++index)
                {
                    firmwareOffset = index + pageOffset;
                    
                    // Write page data
                    m_currentUpdateProcess(PROG_WRITING_PAGE_DATA);
                    if(!WritePageData(loadedFirmware, firmwareOffset, firmwareFileSize))
                    {
                        m_currentErrorMessage(PROG_ERROR_UNABLE_TO_WRITE_PAGE_DATA);
                    }

                    // Attempt to yield to other threads
                    std::this_thread::yield();

                    // Update percentage complete
                    float currentPercentage = ((float)firmwareOffset/(float)totalBytesToWrite) * 100.0f;
                    m_currentPercentComplete((int)currentPercentage);
                }

                // Check program status
                bool errorFound = false;
                if(CheckForProgrammingErrors(&errorStatus))
                {
                    switch (errorStatus)
                    {
                        case I2C_PROG_ERROR_ERASE:
                        {
                            m_currentErrorMessage(I2C_PROG_ERROR_ERASE_MESSAGE);
                            break;
                        }
                        case I2C_PROG_ERROR_WRITE:
                        {
                            m_currentErrorMessage(I2C_PROG_ERROR_WRITE_MESSAGE);
                            break;
                        }
                        case I2C_PROG_ERROR_CRC:
                        {
                            m_currentErrorMessage(I2C_PROG_ERROR_CRC_MESSAGE);
                            break;
                        }
                        default:
                            ++pageIndex;
                            break;
                    }
                }
                else
                {
                    m_currentErrorMessage(PROG_ERROR_UNABLE_TO_CHECK_ERROR_STATUS);
                }

                // Sleeping here is required to avoid "failed to erase flash" errors
                std::this_thread::sleep_for (std::chrono::milliseconds(750));
            }

            // Clean up the loaded firmware
            if(loadedFirmware != nullptr)
            {
                delete [] loadedFirmware;
                loadedFirmware = nullptr;
            }

            // Inform the client application that the update is complete.
            m_updateComplete(true);
        }

        bool XboxHdmi::LoadFirmwareImage(UpdateSource updateSource, uint8_t*& firmwareImage, long* fileSize, const char* firmwareFilePath)
        {
            bool imageWasLoaded = false;

            switch (updateSource)
            {
            case UpdateSource::HDD:
                // TODO
                break;
            case UpdateSource::DVD:
                // TODO
                break;
            case UpdateSource::INTERNET:
                // TODO
                break;
            case UpdateSource::WORKING_DIRECTORY:
            {
                FILE* firmwareFile= fopen("D:\\firmware.bin", "rb");

                if(firmwareFile)
                {
                    fseek(firmwareFile, 0, SEEK_END);
                    *fileSize = ftell(firmwareFile);
                    rewind(firmwareFile);

                    if(firmwareImage != nullptr)
                    {
                        delete [] firmwareImage;
                        firmwareImage = nullptr;
                    }

                    firmwareImage = new uint8_t[*fileSize + 1];

                    fread(firmwareImage, 1, *fileSize, firmwareFile);
                    fclose(firmwareFile);
                    imageWasLoaded = true;
                }
                break;
            }
            default:
                break;
            }

            return imageWasLoaded;
        }

        bool XboxHdmi::SwitchBootMode(BootMode switchToMode)
        {
            BootMode currentMode;
            bool writeSuccessful = false;

            if(GetBootMode(&currentMode) && currentMode != (ULONG)switchToMode)
            {
                if(switchToMode == BootMode::HDMI_PROGRAM)
                {
                    writeSuccessful = HalWriteSMBusValue(I2C_HDMI_ADRESS, I2C_LOAD_APP, 
                                                        0, BOOT_HDMI_PROGRAM) == 0;
                }
                else // BootMode::BOOTROM
                {
                    writeSuccessful = HalWriteSMBusValue(I2C_HDMI_ADRESS, I2C_LOAD_APP, 
                                                        0, BOOT_HDMI_BOOTROM) == 0;
                }
            }

            return writeSuccessful;
        }

        void XboxHdmi::GeneratePageCrc(uint32_t* crcValue, uint8_t* firmwareFile, uint32_t offset, long fileSize)
        {
            if (offset < fileSize)
            {
                *crcValue = CrcAddByte(*crcValue, firmwareFile[offset]);
            }
            else
            {
                *crcValue = CrcAddByte(*crcValue, 0x00);
            }

            return;
        }

        bool XboxHdmi::WritePageCrc(uint32_t crcValue)
        {
            bool writeWasSuccessful = false;

            writeWasSuccessful = HalWriteSMBusValue(I2C_HDMI_ADRESS, I2C_PROG_CRC3, 0,
                                                    (crcValue >> 24) & 0xFF) == 0;
            
            if(writeWasSuccessful)
            {
            writeWasSuccessful = HalWriteSMBusValue(I2C_HDMI_ADRESS, I2C_PROG_CRC2, 0,
                                                    (crcValue >> 16) & 0xFF) == 0;
            }

            if(writeWasSuccessful)
            {
            writeWasSuccessful = HalWriteSMBusValue(I2C_HDMI_ADRESS, I2C_PROG_CRC1, 0,
                                                    (crcValue >> 8) & 0xFF) == 0;
            }

            if(writeWasSuccessful)
            {
            writeWasSuccessful = HalWriteSMBusValue(I2C_HDMI_ADRESS, I2C_PROG_CRC0, 0, 
                                                    (crcValue)&0xFF) == 0;
            }

            return writeWasSuccessful;
        }

        bool XboxHdmi::WritePageData(uint8_t* firmwareFile, uint32_t offset, long fileSize)
        {
            if (offset < fileSize)
            {
                if(HalWriteSMBusValue(I2C_HDMI_ADRESS, I2C_PROG_DATA, 0, firmwareFile[offset]) == 0)
                {
                    return true;
                }
            }
            else
            {
                if(HalWriteSMBusValue(I2C_HDMI_ADRESS, I2C_PROG_DATA, 0, 0x00) == 0)
                {
                    return true;
                }
            }
            return false;
        }

        bool XboxHdmi::CheckForProgrammingErrors(ULONG* statusValue)
        {
            return (HalReadSMBusValue(I2C_HDMI_ADRESS, I2C_PROG_ERROR, false, statusValue) == 0);
        }

        uint32_t XboxHdmi::CrcAddByte(uint32_t crc, uint8_t addByte)
        {
            for (uint8_t bitPosition = 0x01; bitPosition > 0; bitPosition <<= 1)
            {
                uint32_t mostSignificantBit = crc & 0x80000000;
                if (addByte & bitPosition)
                {
                    mostSignificantBit ^= 0x80000000;
                } 

                if (mostSignificantBit)
                {
                    crc = (crc << 1) ^ 0x4C11DB7;
                }
                else
                {
                    crc = (crc << 1);
                }
            }
            return crc;
        }

        uint32_t XboxHdmi::CrcResult(uint32_t crc)
        {
            return (ReverseU32(crc) ^ 0xFFFFFFFF);
        }
        
        uint32_t XboxHdmi::ReverseU32(uint32_t dataToReverse)
        {
            uint32_t reversed = 0;
            for (size_t iter = 0; iter < 32; ++iter)
            {
                reversed <<= 1;
                reversed |= (dataToReverse & 0x01);
                dataToReverse >>= 1;
            }
            return reversed;
        }
    } // XboxHDMI
} // Conflux