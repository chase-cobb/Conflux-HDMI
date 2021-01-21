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

            // TODO : Fill map with supported features
        }

        XboxHdmi::~XboxHdmi()
        {
            ClearFeatureMap();
        }

        bool XboxHdmi::IsFirmwareUpdateAvailable(UpdateSource updateSource)
        {
            // TODO
            return false;
        }

        bool XboxHdmi::UpdateFirmware(UpdateSource updateSource)
        {
            // TODO
            return false;
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
            // TODO : Refactor this so that it's less ugly and lowers the
            //        cost of change.
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
                else
                {
                    *mode = BootMode::BOOTROM;
                }
            }

            return readSuccessful;
        }

        void XboxHdmi::StartFirmwareUpdateProcess()
        {
            // TODO
        }

        bool XboxHdmi::LoadFirmwareImage(UpdateSource updateSource, uint8_t* firmwareImage)
        {
            // TODO 

            return false;
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

        bool XboxHdmi::WritePageCrc(int* firmwareFile, uint32_t offset)
        {
            // TODO 

            return false;
        }

        bool XboxHdmi::WritePageData(int* firmwareFile, uint32_t offset)
        {
            // TODO 
            
            return false;
        }
    } // XboxHDMI
} // Conflux