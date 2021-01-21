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

#include "HdmiInterface.h"

namespace Conflux
{
    bool HdmiInterface::GetFeatureCurrentValue(SupportedFeatures feature, 
                                               int* currentValue)
    {
        if(currentValue != nullptr)
        {
            if(IsFeatureSupportedAndValuesPopulated(feature))
            {
                *currentValue = m_featureValues.at(feature)->GetValue();
                return true;
            }
        }
        return false;
    }

    bool HdmiInterface::GetFeatureValueRange(SupportedFeatures feature, 
                                             int* minValue, int*maxValue)
    {
        if(minValue != nullptr && maxValue != nullptr)
        {
            if(IsFeatureSupportedAndValuesPopulated(feature))
            {
                *minValue = m_featureValues.at(feature)->GetMinValue();
                *maxValue = m_featureValues.at(feature)->GetMaxValue();
                return true;
            }
        }
        return false;
    }
    
    bool HdmiInterface::SetFeatureCurrentValue(SupportedFeatures feature, int currentValue)
    {
        if(IsFeatureSupported(feature))
        {
            m_featureValues[feature]->SetValue(currentValue);
            return true;
        }
        return false;
    }

    const char* HdmiInterface::GetFeatureName(SupportedFeatures feature)
    {
        if(m_featureValues.count(feature) == 1)
        {
            return m_featureValues.at(feature)->GetName();
        }

        return "Invalid feature";
    }

    const std::map<SupportedFeatures, RangedIntValue*>& HdmiInterface::GetConstFeatureMap()
    {
        return m_featureValues;
    }

    bool HdmiInterface::IsFeatureSupportedAndValuesPopulated(SupportedFeatures feature)
    {
        return (IsFeatureSupported(feature) && 
                m_featureValues.count(feature) == 1);
    }

    void HdmiInterface::ClearFeatureMap()
    {
        std::map<SupportedFeatures, RangedIntValue*>::iterator iter = m_featureValues.begin();
        for( ; iter != m_featureValues.end(); ++iter)
        {
            if(iter->second)
            {
                delete iter->second;
            }
        }
        m_featureValues.clear();
    }
} // Conflux