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
        const char* const FirmwareUpdateUrl = "TODO";

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