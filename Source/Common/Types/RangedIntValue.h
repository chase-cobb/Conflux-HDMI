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

#ifndef RANGED_INT_VALUE_H
#define RANGED_INT_VALUE_H

namespace Conflux
{
    /**
     * @brief This class is used by HDMI implementations to
     * get/set values related to a specific feature.
     * 
     */
    class RangedIntValue
    {
    public:
    RangedIntValue(int currentValue, int minValue, int maxValue, const char* name);

    /**
     * @brief Gets the current value.
     * 
     * @return int the current value.
     */
    int GetValue() {return m_value;}

    /**
     * @brief Gets the minimum value range.
     * 
     * @return int minimum value range.
     */
    int GetMinValue() {return m_min;}

    /**
     * @brief Gets the maximum value range.
     * 
     * @return int maximum value range.
     */
    int GetMaxValue() {return m_max;}

    /**
     * @brief Gets the name of this feature.
     * 
     * @return const char* feature name.
     */
    const char* GetName() {return m_name;}

    /**
     * @brief Sets the current value.
     * 
     * @param currentValue the new current Value.
     * @note This function will clamp the new
     * value within the feature min/max range.
     */
    void SetValue(int currentValue);

    /**
     * @brief Sets the value range for this 
     * feature.
     * 
     * @param minValue minimum value range.
     * @param maxValue maximum value range.
     * @note The ranges set here are inclusive.
     */
    void SetMinMax(int minValue, int maxValue);

    private:
    int m_value, m_min, m_max;
    const char* m_name;
    };
} // Conflux

#endif // RANGED_INT_VALUE_H