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

#ifndef HDMITOOLS_H
#define HDMITOOLS_H

#include "Enums.h"
#include "VersionCode.h"
#include "HdmiInterface.h"
#include <vector>

// TODO : CC - Register callbacks for async update

namespace Conflux
{
    typedef const std::map<SupportedFeatures, RangedIntValue*> ConstFeatureMap;
    typedef std::map<Conflux::SupportedFeatures, Conflux::RangedIntValue*>::const_iterator ConstFeatureMapIterator;

    /**
     * @brief This class is a singleton that contains the full
     * developer-facing interface for developing applications
     * that leverage some of the internal HDMI kits for original
     * Xbox. Currently supported devices are indexed by 
     * Conflux::HdmiHardwareId.
     * 
     */
    class HdmiTools
    {
    public:
        /**
         * @brief Get the singleton Instance object
         * 
         * @return HdmiTools* pointer to the singleton object.
         */
        static HdmiTools* GetInstance();

        /**
         * @brief Initializes the singleton object
         * 
         * @return true iF the object was initialized.
         * @return false otherwise.
         * @note The singleton must be initialized before
         * other member functions are called.
         */
        bool Initialize();

        /**
         * @brief Check feature support on the current platform.
         * 
         * @param feature enumeration of possible supported
         * features. Indexed by Conflux::SupportedFeatures.
         * @return true if the feature is supported.
         * @return false otherwise.
         */
        bool IsFeatureSupported(SupportedFeatures feature);

        /**
         * @brief Gets all configurable features supported by 
         * the installed HDMI hardware.
         * 
         * @param features enumeration of possible supported
         * features. Indexed by Conflux::SupportedFeatures.
         */
        void GetAllSupportedFeatures(std::vector<SupportedFeatures>* features);

        /**
         * @brief Gets a read only reference to the map of supported
         * features and values
         * 
         * @return const std::map<SupportedFeatures, RangedIntValue*>& 
         * feature map.
         */
        const std::map<SupportedFeatures, RangedIntValue*>& GetConstFeatureMap();

        /**
         * @brief Get the name of the installed HDMI device.
         * 
         * @return const char* name of the HDMI device.
         */
        const char* GetHdmiName();

        /**
         * @brief Gets a VersionCode object filled out with the 
         * firmware version.
         * 
         * @return VersionCode object containing version information.
         */
        VersionCode GetFirmwareVersion();

        /**
         * @brief Gets a VersionCode object filled out with the 
         * kernel version.
         * 
         * @return VersionCode object containing version information.
         */
        VersionCode GetKernelPatchVersion();

        /**
         * @brief Checks to see if a firmware update is available via 
         * the provided update source.
         * 
         * @param updateSource Enumeration of possible update sources. 
         * Indexed by Conflux::UpdateSource.
         * @return true if an update is available.
         * @return false otherwise.
         */
        bool IsUpdateAvailable(UpdateSource updateSource);

        /**
         * @brief Perform firmware update via the proviced update 
         * source.
         * 
         * @param updateSource Enumeration of possible update sources.
         * @return true if update was successful.
         * @return false otherwise.
         */
        bool UpdateFirmware(UpdateSource updateSource);

        /**
         * @brief Get the current value of a given feature, as well
         * as the valid value range.
         * 
         * @param feature enumeration of possible supported
         * features. Indexed by Conflux::SupportedFeatures.
         * @param value current value of this configurable feature.
         * @param min minimum valid range of this configurable 
         * feature.
         * @param max maximum valid range of this configurable 
         * feature.
         * @return true if the feature values were retrieved successfully.
         * @return false otherwise.
         */
        bool GetFeatureValues(SupportedFeatures feature, int* value, 
                              int* min, int* max);

        /**
         * @brief Sets the current value of a configurable feature.
         * 
         * @param feature enumeration of possible supported
         * features. Indexed by Conflux::SupportedFeatures.
         * @param value current value of this configurable feature.
         * @return true if the value was set successfully.
         * @return false otherwise.
         */
        bool SetFeatureValue(SupportedFeatures feature, int value);

        /**
         * @brief Updates all feature values in the hardware configuration
         * of the HDMI hardware.
         * 
         * @return true if the operation was successful.
         * @return false otherwise.
         */
        bool UpdateFeatureConfig();

        /**
         * @brief Gets the name of the feature requested.
         * 
         * @param feature enumeration of possible supported
         * features. Indexed by Conflux::SupportedFeatures.
         * @return const char* name of the feature.
         */
        const char* GetFeatureName(SupportedFeatures feature);

        /**
         * @brief Save all configurable features.
         * 
         * @return true if successful.
         * @return false otherwise.
         */
        bool SaveSettings();

    private:
        static HdmiTools* m_instance;
        HdmiInterface* m_hdmiInterface;
        VersionCode* m_firmwareVersion;
        VersionCode* m_kernelVersion;

        // This member exists only to avoid compiler warnings and should
        // never be populated.
        std::map<SupportedFeatures, RangedIntValue*> m_emptyFeatureMap;

        HdmiTools();
        HdmiTools(const HdmiTools& copy);
        HdmiTools& operator=(const HdmiTools& copy);
        ~HdmiTools();

        bool DownloadFirmware();
        HdmiHardwareId GetHardwareId();
    };
} // Conflux

#endif // HDMITOOLS_H