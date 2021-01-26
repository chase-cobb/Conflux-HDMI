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
#include <SDL.h>
#include <windows.h>
#include <pbkit/pbkit.h>
#include <stdbool.h>
#include <map>
#include <sstream>

#include "HdmiTools.h"
#include "RangedIntValue.h"

int main(void)
{
  SDL_GameController *pad = NULL;
  Conflux::HdmiTools* hdmiTools = Conflux::HdmiTools::GetInstance();
  bool pbkInit = false, sdlInit = false, isRunning = true;

  XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

  sdlInit = SDL_Init(SDL_INIT_GAMECONTROLLER) == 0;
  if (!sdlInit)
  {
    debugPrint("SDL_Init failed: %s\n", SDL_GetError());
    isRunning = false;
  }

  while (isRunning && SDL_NumJoysticks() < 1)
  {
    debugPrint("Please connect gamepad\n");
    Sleep(1000); // Wait one second to try again.
  }

  pad = (!isRunning) ? NULL : SDL_GameControllerOpen(0);
  if (pad == NULL)
  {
    debugPrint("Failed to open gamecontroller 0\n");
    isRunning = false;
  }

  if(isRunning)
  {
    isRunning = Conflux::HdmiTools::GetInstance()->Initialize();
    if(!isRunning)
    {
      debugPrint("Conflux detected no supported HDMI hardware!!\nClosing app...");
    }
  }

  pbkInit = (!isRunning) ? false : pb_init() == 0;
  if (!pbkInit && isRunning)
  {
    debugPrint("pbkit init failed\n");
    isRunning = false;
  }

  if(isRunning)
  {
    pb_show_front_screen();
  }

  while (isRunning)
  {
    pb_wait_for_vbl();
    pb_target_back_buffer();
    pb_reset();
    pb_fill(0, 0, 640, 480, 0);
    pb_erase_text_screen();
    SDL_GameControllerUpdate();

    int sleepTime = 200; // milliseconds
    if(pad != NULL)
    {
      bool settingsSaved = false;
      std::ostringstream oss;
      oss << "Conflux HDMI feature configuration example \n";
      oss << "Firmware: " << hdmiTools->GetFirmwareVersion().GetVersionCodeAsCString();
      oss << "     Kernel : " << hdmiTools->GetKernelPatchVersion().GetVersionCodeAsCString() << "\n\n";
      Conflux::ConstFeatureMap featureMap = hdmiTools->GetConstFeatureMap();
      Conflux::ConstFeatureMapIterator iter = featureMap.begin();
      for( ; iter != featureMap.end(); ++iter)
      {
        int currentValue = iter->second->GetValue();
        int minValue = iter->second->GetMinValue();
        int maxValue = iter->second->GetMaxValue();

        // log the name of the feature
        oss << iter->second->GetName();

        // handle input based on feature
        switch (iter->first)
        {
        case Conflux::SupportedFeatures::CB_ADJUST:
        {
          oss << " : Use d-pad left/right to adjust\n";
          int leftDown = SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
          int rightDown = SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

          if(leftDown == 1) {currentValue--;}
          else if(rightDown == 1) {currentValue++;}
          break;
        }
        case Conflux::SupportedFeatures::CR_ADJUST:
        {
          oss << " : Use d-pad up/down to adjust\n";
          int upDown = SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_UP);
          int downDown = SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_DOWN);

          if(upDown == 1) {currentValue++;}
          else if(downDown == 1) {currentValue--;}
          break;
        }
        case Conflux::SupportedFeatures::LUMA_ADJUST:
        {
          oss << " : Use triggers to adjust\n";
          short leftTrigger = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
          short rightTrigger = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

          if(leftTrigger > 25000) {currentValue--;}
          if(rightTrigger > 2500) {currentValue++;}
          break;
        }
        case Conflux::SupportedFeatures::WIDESCREEN_ADJUST:
        {
          oss << " : Use Y button to toggle mode\n";
          int yDown = SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_Y);

          if(yDown)
          {
            if(++currentValue > maxValue)
            {
              currentValue = minValue;
            }
          }
          break;
        }
        case Conflux::SupportedFeatures::VIDEO_MODE_ADJUST:
        {
          oss << " : Use X button to toggle mode\n";
          int xDown = SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_X);

          if(xDown)
          {
            if(++currentValue > maxValue)
            {
              currentValue = minValue;
            }
          }
          break;
        }
        default:
          break;
        }

        // if the current value has changed, it needs to 
        // be set via the Conflux API
        if(currentValue != iter->second->GetValue())
        {
          hdmiTools->SetFeatureValue(iter->first, currentValue);
          hdmiTools->UpdateFeatureConfig();
        }

        // log feature values
        oss << " value: " << iter->second->GetValue() <<
                            "    min: " << minValue <<
                            "    max: " << maxValue << "\n";
      }

      oss << "\n----Press A:Save       Press B: Exit----\n";
      if(SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_A) == 1)
      {
        hdmiTools->SaveSettings();
        oss << "Settings Saved!";
        sleepTime = 1500;
      }
      if(SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_B) == 1)
      {
        isRunning = false;
      }

      pb_print(oss.str().c_str());
    }
    else
    {
      pb_print("Gamepad is NULL!!");
      pad = SDL_GameControllerOpen(0);
    }

    pb_draw_text_screen();
    while (pb_busy());
    while (pb_finished());
    Sleep(sleepTime);
  }

  Sleep(2000);

  if (pbkInit)
  {
    pb_kill();
  }

  if (pad != NULL)
  {
    SDL_GameControllerClose(pad);
  }

  if (sdlInit)
  {
    SDL_Quit();
  }

  return 0;
}