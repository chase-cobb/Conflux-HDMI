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

#ifndef HDMIINTERFACE_H
#define HDMIINTERFACE_H

#include "VersionCode.h"
#include "Enums.h"
#include "RangedIntValue.h"
#include <map>

namespace Conflux
{
    class VersionCode;

    /**
     * @brief Interface for HDMI device APIs to implement.
     * 
     */
    class HdmiInterface
    {
    public:
        virtual ~HdmiInterface() {};

        /**
         * @brief Checks to see if a firmware update is available via 
         * the provided update source.
         * 
         * @param updateSource Enumeration of possible update sources. 
         * Indexed by Conflux::UpdateSource.
         * @return true if an update is available.
         * @return false otherwise.
         */
        virtual bool IsFirmwareUpdateAvailable(UpdateSource updateSource) = 0;

        /**
         * @brief Calling this function with a valid update source
         * will begin the process of updating the firmware. This is an 
         * async process and updates to the process are provided via 
         * callbacks.
         * 
         * @param updateSource Enumeration of possible update sources. 
         * Indexed by Conflux::UpdateSource.
         * @param currentProcess Callback that provides updates
         * on the current update process.
         * @param percentComplete Callback that provides the 
         * current percentage complete.
         * @param errorMessage Callback that provides any errors
         * encountered during the process.
         * @param updateComplete Callback that notifies that the
         * update has completed successfully.
         * @param pathToFirmware Allows the location of the update
         * to be provided by absolute path, if the update source is 
         * Conflux::UpdateSource::WORKING_DIRECTORY.
         * @return true if the Process was started successfully.
         * @return false otherwise.
         */
        virtual bool UpdateFirmware(UpdateSource updateSource, void (*currentProcess)(const char* currentProcess)
                                                             , void (*percentComplete)(int percentageComplete)
                                                             , void (*errorMessage)(const char* errorMessage)
                                                             , void (*updateComplete)(void)
                                                             , const char* pathToFirmware = "") = 0;

         /**
          * @brief Check to see if this HDMI device supports
          * a specific feature.
          * 
          * @param feature enumeration of possible supported
         * features. Indexed by Conflux::SupportedFeatures.
          * @return true if the feature is supported.
          * @return false otherwise.
          */
        virtual bool IsFeatureSupported(SupportedFeatures feature) = 0;

        /**
         * @brief Fills out the provided Conflux::VersionCode
         * object with the firmware of the installed HDMI
         * hardware.
         * 
         * @param versionCode class object that contains the 
         * version information for HDMI devices. Conflux::VersionCode.
         * @return true if the version number was found.
         * @return false otherwise.
         */
        virtual bool GetFirmwareVersion(VersionCode* versionCode) = 0;

        /**
         * @brief Gets the name of the installed HDMI hardware.
         * 
         * @return const char* HDMI hardware name.
         */
        virtual const char* GetName() = 0;

        /**
         * @brief Loads persistent configuration data for the 
         * currently installed HDMI hardware. 
         * 
         * @return true if the configuration was loaded and
         * applied.
         * @return false otherwise.
         */
        virtual bool LoadConfig() = 0;

        /**
         * @brief Update the current configuration of the 
         * hardware in a way that they are applied in the
         * current session.
         * 
         * @return true if the config data was applied.
         * @return false otherwise.
         * @note It is expected that developers will call
         * SaveConfig() to write out persistent configuration
         * data sometime after calling this function.
         */
        virtual bool UpdateConfigValues() = 0;

        /**
         * @brief Write configuration data to persistent
         * storage. (EEPROM or config file)
         * 
         * @return true if the configuration was saved.
         * @return false otherwise.
         */
        virtual bool SaveConfig() = 0;

        /**
         * @brief Gets the ID of the current hardware.
         * 
         * @return HdmiHardwareId hardware ID, indexed by
         * Conflux::HdmiHardwareId.
         */
        HdmiHardwareId GetHardwareId() {return m_hardwareId;}

        /**
         * @brief Fills out the int* provided with the current
         * value of the feature specified.
         * 
         * @param feature enumeration of possible supported
         * features. Indexed by Conflux::SupportedFeatures.
         * @param currentValue filled out with the current 
         * value of the specified feature.
         * @return true if the current value was found and the
         * currentValue param was filled out.
         * @return false otherwise.
         */
        bool GetFeatureCurrentValue(SupportedFeatures feature, int* currentValue);

        /**
         * @brief Fills out the minValue and maxValue params
         * with the minimum and maximum range of the feature
         * specified.
         * 
         * @param feature enumeration of possible supported
         * features. Indexed by Conflux::SupportedFeatures.
         * @param minValue filled out with the minimum value
         * range of the specified feature.
         * @param maxValue filled out with the maximum value
         * range of the specified feature.
         * @return true if the range was found and the minValue
         * and maxValue params were filled out.
         * @return false otherwise.
         */
        bool GetFeatureValueRange(SupportedFeatures feature, int* minValue, int*maxValue);

        /**
         * @brief Sets the new current value of the feature
         * specified.
         * 
         * @param feature enumeration of possible supported
         * features. Indexed by Conflux::SupportedFeatures.
         * @param currentValue the new current value of the
         * feature specified.
         * @return true if the new current value was applied.
         * @return false otherwise.
         */
        bool SetFeatureCurrentValue(SupportedFeatures feature, int currentValue);

        /**
         * @brief Gets the name of the feature specified.
         * 
         * @param feature enumeration of possible supported
         * features. Indexed by Conflux::SupportedFeatures.
         * @return const char* feature name.
         */
        const char* GetFeatureName(SupportedFeatures feature);

        /**
         * @brief Gets a const reference to the map of features
         * supported by the currently installed HDMI hardware, and
         * the feature values.
         * 
         * @return const std::map<SupportedFeatures, RangedIntValue*>& 
         * map of currently supported feature and their values.
         */
        const std::map<SupportedFeatures, RangedIntValue*>& GetConstFeatureMap();
    protected:
        short m_supportedFeatures;
        HdmiHardwareId m_hardwareId;
        std::map<SupportedFeatures, RangedIntValue*> m_featureValues; // TODO : Clean up all memory!!

        void SetHardwareId(HdmiHardwareId iD) {m_hardwareId = iD;}
        bool IsFeatureSupportedAndValuesPopulated(SupportedFeatures feature);
        void ClearFeatureMap();
    };
} // Conflux

#endif //HDMIINTERFACE_H