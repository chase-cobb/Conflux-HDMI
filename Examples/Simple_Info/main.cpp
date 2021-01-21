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