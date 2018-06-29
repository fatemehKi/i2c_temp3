// AdafruitMCP9808.h
// Interface for AdafruitMCP9808 V2 (Blue Label) with NVIDIA Jetson TK1
// The MIT License (MIT)
//
// Copyright (c) 2015 Jetsonhacks
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef ADAFRUITMCP9808_H
#define ADAFRUITMCP9808_H

#include <cstddef>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>


// Information taken from PulsedLight knowledge base 5-4-15
// Internal Control Registers
// http://kb.pulsedlight3d.com/support/solutions/articles/5000549552-detailed-register-descriptions-internal
// External Control Registers
// http://kb.pulsedlight3d.com/support/solutions/articles/5000549565-detailed-register-descriptions-external

// I2C Slave Address
#define kAdafruitMCP9808I2CAddress                    0x1b

// Internal Control Registers
#define kAdafruitMCP9808CommandControlRegister        0x01    // Command Control Register

// High byte set means read two bytes
#define kAdafruitMCP9808TempMSB          0x02    // Calculated distance in cm (difference between signal and reference delay)
                                                        // High byte of calculated delay of signal [Read Only]: reference – calculated after correlation record processing
                                                        // If the returned MSB is 1 then the reading is not considered valid.

#define kAdafruitMCP9808TempLSB          0x03    // Low byte of calculated delay of signal [Read Only]: reference – calculated after correlation record processing
#define MCP9808_REG_AMBIENT_TEMP         0x05
// External Control Registers
//#define kAdafruitMCP9808HardwareVersion               0x41    // Hardware Version: revisions begin with 0x01
//#define kAdafruitMCP9808SoftwareVersion               0x4f    // Software Version: Revisions begin with 0x01

// Register Command
#define kAdafruitMCP9808Measure                       0x04    // Take acquisition & correlation processing with DC correction

class AdafruitMCP9808
{
public:
    unsigned char kI2CBus ;         // I2C bus of the AdafruitMCP9808
    int kI2CFileDescriptor ;        // File Descriptor to the AdafruitMCP9808
    int error ;
    AdafruitMCP9808();
    ~AdafruitMCP9808() ;
    bool openAdafruitMCP9808() ;                   // Open the I2C bus to the AdafruitMCP9808
    void closeAdafruitMCP9808();                   // Close the I2C bus to the AdafruitMCP9808
   // int writeAdafruitMCP9808(int writeRegister,int writeValue) ;
    int readAdafruitMCP9808(int readRegister) ;
    int getTemp() ;

    int getError() ;

};

#endif // AdafruitMCP9808_H
