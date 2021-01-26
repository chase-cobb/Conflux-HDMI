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

#include <hal/debug.h>
#include <hal/video.h>
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>

#include "HdmiTools.h"
#include "RangedIntValue.h"

int main(void) {
  XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

  Conflux::HdmiTools* hdmiTools = Conflux::HdmiTools::GetInstance();
  if(hdmiTools->Initialize())
  {
    // General info
    debugPrint("Conflux HDMI Initialized....\n");
    debugPrint("Hardware found   : %s\n", hdmiTools->GetHdmiName());
    debugPrint("Firmware version : %s\n", hdmiTools->GetFirmwareVersion().
                                          GetVersionCodeAsCString());
    debugPrint("Kernel version   : %s\n", hdmiTools->GetKernelPatchVersion().
                                          GetVersionCodeAsCString());


    // Log supported features and their values/ranges
    Conflux::ConstFeatureMap featureMap = hdmiTools->GetConstFeatureMap();
    Conflux::ConstFeatureMapIterator iter = featureMap.begin();
    for( ; iter != featureMap.end(); ++iter)
    {
      const char* featureName = iter->second->GetName();
      debugPrint("\nNow getting info about the %s feature adjustment....\n", featureName);
      debugPrint("%s value:%d    min:%d    max:%d\n", featureName, iter->second->GetValue(),
                                                                   iter->second->GetMinValue(),
                                                                   iter->second->GetMaxValue());
    }
  }
  else
  {
    debugPrint("No valid HDMI device detected!\n");
  }
  
  
  debugPrint("\n\nExiting the appliction in 10 seconds.\n");
  Sleep(10000);

  return 0;
}