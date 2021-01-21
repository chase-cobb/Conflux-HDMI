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

#include "HdmiTools.h"
#include "Helpers.h"
#include "XboxHdmi.h"
#include "Strings.h"

namespace Conflux
{
    //static member declaration
    HdmiTools*  HdmiTools::m_instance;

    HdmiTools::HdmiTools()
    {
        m_hdmiInterface = nullptr;
        m_firmwareVersion = nullptr;
    } 

    HdmiTools::HdmiTools(const HdmiTools& copy)
    {
        // UNUSED
    }

    HdmiTools& HdmiTools::operator=(const HdmiTools& copy)
    {
        return *this;
    }

    HdmiTools::~HdmiTools()
    {
        if(m_instance)
        {
            delete m_instance;
            m_instance = nullptr;
        }

        if(m_hdmiInterface)
        {
            delete m_hdmiInterface;
            m_hdmiInterface = nullptr;
        }

        if(m_firmwareVersion)
        {
            delete m_firmwareVersion;
            m_firmwareVersion = nullptr;
        }
    }

    bool HdmiTools::DownloadFirmware()
    {
        // TODO : 
        return false;
    }

    HdmiTools* HdmiTools::GetInstance()
    {
        if(!m_instance)
        {
            m_instance = new HdmiTools();
        }

        return m_instance;
    }

    bool HdmiTools::Initialize()
    {
        HdmiHardwareId hardwareId = GetHardwareId();

        switch (hardwareId)
        {
        case HdmiHardwareId::NO_HW_DETECTED:
            return false;
            break;
        case HdmiHardwareId::XBOXHDMI:
            m_hdmiInterface = new XboxHDMI::XboxHdmi;
            m_hdmiInterface->LoadConfig();
            return true;
            break;
        default:
            break;
        }

        return false;
    }

    HdmiHardwareId HdmiTools::GetHardwareId()
    {
        return DetectInstalledHardware();
    }

    bool HdmiTools::IsFeatureSupported(SupportedFeatures feature)
    {
        if(m_hdmiInterface != nullptr)
        {
            return m_hdmiInterface->IsFeatureSupported(feature);
        }
        return false;
    }

    void HdmiTools::GetAllSupportedFeatures(std::vector<SupportedFeatures>* features)
    {
        for(int i = 0; i < ((int)SupportedFeatures::LAST_ENTRY); i == 0 ? i++ : i *= 2)
        {
            if(IsFeatureSupported((SupportedFeatures)i))
            {
                features->push_back((SupportedFeatures)i);
            }
        }
    }

    const std::map<SupportedFeatures, RangedIntValue*>& HdmiTools::GetConstFeatureMap()
    {
        if(m_hdmiInterface != nullptr)
        {
            return m_hdmiInterface->GetConstFeatureMap();
        }
        return m_emptyFeatureMap;
    }

    const char* HdmiTools::GetHdmiName()
    {
        if(m_hdmiInterface != nullptr)
        {
            return m_hdmiInterface->GetName();
        }

        return "No HDMI detected!!";
    }

    VersionCode HdmiTools::GetFirmwareVersion()
    {
        if(m_hdmiInterface != nullptr)
        {
            if(m_firmwareVersion == nullptr)
            {
                m_firmwareVersion = new VersionCode;
            }

            if(m_hdmiInterface->GetFirmwareVersion(m_firmwareVersion))
            {
                // TODO : Add a "dirty" flag to avoid polling
                //        unless necessary.
            }
        }

        return *m_firmwareVersion;
    }

    VersionCode HdmiTools::GetKernelPatchVersion()
    {
        if(m_hdmiInterface != nullptr)
        {
            if(m_kernelVersion == nullptr)
            {
                m_kernelVersion = new VersionCode;
            }

            if(GetKernelPatchVersionCode(m_kernelVersion))
            {
                // TODO
            }
        }
        
        return *m_kernelVersion;
    }

    bool HdmiTools::IsUpdateAvailable(UpdateSource updateSource)
    {
        // TODO
        return false;
    }

    bool HdmiTools::UpdateFirmware(UpdateSource updateSource)
    {
        // TODO 
        return false;
    }

    bool HdmiTools::SaveSettings()
    {
        if(m_hdmiInterface != nullptr)
        {
            return m_hdmiInterface->SaveConfig();
        }
        return false;
    }

    bool HdmiTools::GetFeatureValues(SupportedFeatures feature, int* value, int* min, int* max)
    {
        // TODO : Error check this better and store temp values
        //        of the dereferenced pointers. Upon failure, restore
        //        those values before returning false
        if(m_hdmiInterface != nullptr)
        {
            if(m_hdmiInterface->IsFeatureSupported(feature))
            {
                if(m_hdmiInterface->GetFeatureCurrentValue(feature, value))
                {
                    if(m_hdmiInterface->GetFeatureValueRange(feature, min, max))
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    }
    
    bool HdmiTools::SetFeatureValue(SupportedFeatures feature, int value)
    {
        if(m_hdmiInterface != nullptr)
        {
            if(m_hdmiInterface->IsFeatureSupported(feature))
            {
                return m_hdmiInterface->SetFeatureCurrentValue(feature, value);
            }
        }
        return false;
    }

    bool HdmiTools::UpdateFeatureConfig()
    {
        if(m_hdmiInterface != nullptr)
        {
            return m_hdmiInterface->UpdateConfigValues();
        }
        return false;
    }

    const char* HdmiTools::GetFeatureName(SupportedFeatures feature)
    {
        if(m_hdmiInterface != nullptr)
        {
            return m_hdmiInterface->GetFeatureName(feature);
        }
        return "Invalid";
    }
} // Conflux