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

#ifndef XBOXHDMI_CONFIG_H
#define XBOXHDMI_CONFIG_H

namespace Conflux
{
    namespace XboxHDMI
    {
        const char* const DEFAULT_FIRMWARE_WORKING_DIRECTORY = "D:\\firmware.bin";
        const char* const DEFAULT_FIRMWARE_WORKING_DIRECTORY_OPEN_MODE = "rb";

        const char* const PROG_PROCESS_LOADING_FIRMWARE = "Loading firmware";
        const char* const PROG_PROCESS_FIRMWARE_FILE_SIZE = "Firmware file size-> ";
        const char* const PROG_PROCESS_LOADED_FIRMWARE = "Firmware loaded!!";
        const char* const PROG_ERROR_FAILED_TO_LOAD_FIRMWARE = "Failed to load firmware";
        const char* const PROG_SWITCHING_TO_BOOTROM = "Switching to bootrom";
        const char* const PROG_WAITING_FOR_BOOTROM = "Waiting to reset to bootrom";
        const char* const PROG_CHECKING_BOOT_MODE = "Checking boot mode";
        const char* const PROG_SWAPPED_TO_BOOTROM = "Successfully swapped to bootrom";
        const char* const PROG_FLASHING_FIRMWARE = "Flashing firmware";
        const char* const PROG_WRITING_PAGE_CRC = "Writing page CRC";
        const char* const PROG_WRITING_PAGE_DATA = "Writing page data";

        const char* const BOOT_MODE_HDMI_PROGRAM = "HDMI program mode";
        const char* const BOOT_MODE_HDMI_BOOTROM = "Bootrom mode";
        const char* const BOOT_MODE_HDMI_FIRMWARE = "HDMI firmware mode";
        const char* const BOOT_MODE_HDMI_INVALID = "Invalid boot mode";

        const char* const PROG_ERROR_UNABLE_TO_GET_BOOT_MODE = "Unable to get boot mode";
        const char* const PROG_ERROR_UNABLE_TO_SIGNAL_BOOT_MODE = "Unable to send signal to switch boot mode";
        const char* const PROG_ERROR_UNABLE_TO_SWAP_TO_BOOTROM = "Unable to swap to boot rom";
        const char* const PROG_ERROR_UNABLE_TO_WRITE_PAGE_DATA = "Unable to write page data";
        const char* const PROG_ERROR_UNABLE_TO_WRITE_CRC_DATA = "Unable to write CRC data";
        const char* const PROG_ERROR_UNABLE_TO_CHECK_ERROR_STATUS = "Unable to read error status";

        const char* const I2C_PROG_ERROR_CRC_MESSAGE = "Failed to verify CRC";
        const char* const I2C_PROG_ERROR_WRITE_MESSAGE = "Failed to write flash";
        const char* const I2C_PROG_ERROR_ERASE_MESSAGE = "Failed to erase flash";

        const unsigned int PROGRAMMABLE_PAGES = 38;
        const unsigned int PAGE_SIZE = 1024;

        const unsigned int CRC_INIT = 0xffffffff;

        // XboxHDMI addresses were changed to const values from
        // preprocessor defines to allow them to be properly
        // namespaced
        const unsigned char I2C_HDMI_ADRESS  = 0x88;
        const unsigned char I2C_BOOT_MODE = 0x20;
        const unsigned char I2C_BOOT_FLAG = 0x21;
        const unsigned char I2C_COMPILE_TIME = 0x25;
        const unsigned char I2C_COMPILE_TIME0 = 0x25;
        const unsigned char I2C_COMPILE_TIME1 = 0x26;
        const unsigned char I2C_COMPILE_TIME2 = 0x27;
        const unsigned char I2C_COMPILE_TIME3 = 0x28;
        const unsigned char I2C_LOAD_APP = 0x29;
        const unsigned char BOOT_HDMI_INVALID = 0x00;
        const unsigned char BOOT_HDMI_BOOTROM = 0x40;
        const unsigned char BOOT_HDMI_PROGRAM = 0x42;
        const unsigned char BOOT_HDMI_FIRMWARE = 0x60;
        const unsigned char I2C_PROG_MODE = 0x2D;
        const unsigned char I2C_PROG_WRITE = 0x2E;
        const unsigned char I2C_PROG_WRITE_PAGE00 = 0x00;
        const unsigned char I2C_PROG_WRITE_PAGE01 = 0x01;
        const unsigned char I2C_PROG_DATA = 0x2F;
        const unsigned char I2C_PROG_PAGE = 0x33;
        const unsigned char I2C_PROG_POS = 0x34;
        const unsigned char I2C_PROG_FULL = 0x36;
        const unsigned char I2C_PROG_BUSY = 0x37;
        const unsigned char I2C_PROG_CRC = 0x38;
        const unsigned char I2C_PROG_CRC0 = 0x38;
        const unsigned char I2C_PROG_CRC1 = 0x39;
        const unsigned char I2C_PROG_CRC2 = 0x3A;
        const unsigned char I2C_PROG_CRC3 = 0x3B;
        const unsigned char I2C_PROG_ERROR = 0x3C;
        const unsigned char I2C_PROG_ERROR_NONE = 0x00;
        const unsigned char I2C_PROG_ERROR_ERASE = 0x01;
        const unsigned char I2C_PROG_ERROR_WRITE = 0x02;
        const unsigned char I2C_PROG_ERROR_CRC = 0x03;
        const unsigned char I2C_VIDEO_MODE_PRE = 0x3E;
        const unsigned char I2C_VIDEO_MODE_POST = 0x42;
        const unsigned char I2C_VIDEO_MODE_REV = 0x46;
        const unsigned char I2C_EEPROM_SAVE = 0x4A;
        const unsigned char I2C_EEPROM_WIDESCREEN = 0x4B;
        const unsigned char I2C_EEPROM_MODE_OUT = 0x4C;
        const unsigned char I2C_EEPROM_ADJUST_LUMA = 0x4D;
        const unsigned char I2C_EEPROM_ADJUST_CB = 0x4E;
        const unsigned char I2C_EEPROM_ADJUST_CR = 0x4F;
        const unsigned char I2C_FIRMWARE_VERSION = 0x57;
    } // XboxHDMI
} // Conflux

#endif // XBOXHDMI_CONFIG_H