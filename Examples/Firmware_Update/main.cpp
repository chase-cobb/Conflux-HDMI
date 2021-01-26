#include <hal/debug.h>
#include <hal/video.h>
#include <hal/xbox.h>
#include <SDL.h>
#include <windows.h>
#include <pbkit/pbkit.h>
#include <stdbool.h>
#include <map>
#include <sstream>
#include <mutex>
#include <chrono>

#include "HdmiTools.h"
#include "RangedIntValue.h"

#define SPINNER_LENGTH 4

void ProgressUpdate(const char* progressUpdate);
void PercentComplete(int percentComplete);
void ErrorMessage(const char* currentErrorMessage);
void FirmwareUpdateComplete();

bool firmwareFlashInProgress = false;
bool firmwareWasUpdatedSuccessfully = false;
int updateCompletionPercentage = 0;

// using shared pointers for thread safety
std::string currentProgress;
std::string errorMessage;

std::mutex mutexObject;

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::duration<float, std::milli> Duration;

int main(void)
{
  SDL_GameController *pad = NULL;
  Conflux::HdmiTools* hdmiTools = Conflux::HdmiTools::GetInstance();
  bool pbkInit = false, sdlInit = false, isRunning = true;
  bool newFirmwareIsAvailable = false;

  const char* flashingSpinner[SPINNER_LENGTH] = {"/\0", "-\0", "\\\0", "|\0"};
  int spinnerIndex = 0;

  // Start time
  Clock::time_point previousTime = Clock::now();

  // Loading spinner timer
  int spinnerTimer = 0;
  int loadingSpinnerRotateTime = 200; // milliseconds

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
    isRunning = hdmiTools->Initialize();
    if(!isRunning)
    {
      debugPrint("Conflux detected no supported HDMI hardware!!\nClosing app...");
    }
    else
    {
      newFirmwareIsAvailable = hdmiTools->IsUpdateAvailable(Conflux::UpdateSource::WORKING_DIRECTORY);
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

    if(pad != NULL)
    {
      std::ostringstream oss;

      bool tempFirmwareFlashInProgress = firmwareFlashInProgress;

      oss << "Conflux HDMI async firmware update example \n";
      oss << "Firmware: " << hdmiTools->GetFirmwareVersion().GetVersionCodeAsCString() << "\n";
      oss << "Kernel : " << hdmiTools->GetKernelPatchVersion().GetVersionCodeAsCString() << "\n\n";

      if(newFirmwareIsAvailable && !firmwareWasUpdatedSuccessfully)
      {
        // If the firmware is not being flashed, allow
        // the user to start the process by pressing "A"
        if(!tempFirmwareFlashInProgress)
        {
          oss << "\n----Press A:Update Firmware       Press B: Exit----\n";
          if(SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_A) == 1)
          {
            // This will flash a firmware located in the directory
            // of the currently executing program.
            if(hdmiTools->UpdateFirmware(Conflux::UpdateSource::WORKING_DIRECTORY,
                                      (*ProgressUpdate),
                                      (*PercentComplete),
                                      (*ErrorMessage),
                                      (*FirmwareUpdateComplete)))
            {
              firmwareFlashInProgress = true;
              previousTime = Clock::now();
            }
          }
        }
        else if(tempFirmwareFlashInProgress)
        {
          // FLASHING FIRMWARE!! DO NOT POWER OFF YOUR CONSOLE!!
          const char* spin = flashingSpinner[spinnerIndex];
          oss << spin << spin << spin << "  Flashing firmware  " << spin << spin << spin << "\n";
          oss << "Do NOT power off your console!!\n\n";

          //mutexObject.lock();
          oss << "Flash status     : " << currentProgress << "\n";
          //mutexObject.unlock();

          oss << "Overall progress : " << updateCompletionPercentage << " percent complete" << "\n";
          
          //mutexObject.lock();
          oss << "Error detected   : " << errorMessage << "\n";
          //mutexObject.unlock();

          // TODO : Update spinner timer with delta time in milliseconds.
          Duration deltaTime = Clock::now() - previousTime;
          spinnerTimer += (int)deltaTime.count();

          // Update the flashing spinner
          if(spinnerTimer >= loadingSpinnerRotateTime)
          {
            if(++spinnerIndex >= SPINNER_LENGTH)
            {
              spinnerIndex = 0;
            }
            spinnerTimer = 0;
          }

          previousTime = Clock::now();
        }
      }
      else if(!newFirmwareIsAvailable)
      {
        oss << "\nNo Firmware Updates Available :(\n";
        oss << "\n----Press B: Exit----\n";
      }

      if(firmwareWasUpdatedSuccessfully)
      {
        oss << "\nFirmware was flashed successfully!\n";
        oss << "\nPress B to reboot console\n";
      }
      
      if(SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_B) == 1 && !tempFirmwareFlashInProgress)
      {
        if(firmwareWasUpdatedSuccessfully)
        {
          XReboot();
        }
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

    // Yield to firmware update
    Sleep(0);
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

/*
The mutex locks below may not be necessary in this specific
implementation, but they are included to inform application
developers that these callbacks are coming from a different 
thread. So make sure to be safe with any operations you run
inside these functions!!
*/

void ProgressUpdate(const char* progressUpdate)
{
  //mutexObject.lock();
    currentProgress = std::string(progressUpdate);
  //mutexObject.unlock();
}

void PercentComplete(int percentComplete)
{
  // no need to lock an int assignment  
  updateCompletionPercentage = percentComplete;
}

void ErrorMessage(const char* currentErrorMessage)
{
  //mutexObject.lock();
    errorMessage = std::string(currentErrorMessage);
  //mutexObject.unlock();
}

void FirmwareUpdateComplete()
{
  //mutexObject.lock();
    firmwareFlashInProgress = false;
    firmwareWasUpdatedSuccessfully = true;
    updateCompletionPercentage = 0;
    currentProgress = "Firmware successfully flashed!! Reboot your console.";
    errorMessage.clear();
  //mutexObject.unlock();
}