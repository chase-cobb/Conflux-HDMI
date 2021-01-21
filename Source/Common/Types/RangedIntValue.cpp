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

#include "RangedIntValue.h"
#include "Helpers.h"

namespace Conflux
{
    RangedIntValue::RangedIntValue(int currentValue, int minValue, int maxValue, const char* name) : m_name(name)
    {
        SetMinMax(minValue, maxValue);
        SetValue(currentValue);
    }

    void RangedIntValue::SetValue(int currentValue)
    {
        m_value = Clamp(currentValue, m_min, m_max);
    }

    void RangedIntValue::SetMinMax(int minValue, int maxValue)
    {
        // TODO : Perform better error checking and log any issues
        //        in usage model.

        if(minValue >= maxValue)
        {
            m_min = maxValue - 1;
            m_max = maxValue;
        }
        else
        {
            m_min = minValue;
            m_max = maxValue;
        }
    }
} // Conflux