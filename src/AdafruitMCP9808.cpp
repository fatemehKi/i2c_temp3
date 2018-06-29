#include "AdafruitMCP9808.h"
// Interface for Lidar-Lite V2 (Blue Label) with NVIDIA Jetson TK1


AdafruitMCP9808::AdafruitMCP9808()
{
    kI2CBus = 1 ;           // Default I2C bus for Lidar-Lite on Jetson TK1
    error = 0 ;
}

AdafruitMCP9808::~AdafruitMCP9808()
{
    closeAdafruitMCP9808() ;
}

// Returns true if device file descriptor opens correctly, false otherwise

bool AdafruitMCP9808::openAdafruitMCP9808()
{
    char fileNameBuffer[32];
    sprintf(fileNameBuffer,"/dev/i2c-%d", kI2CBus);
    kI2CFileDescriptor = open(fileNameBuffer, O_RDWR);
    if (kI2CFileDescriptor < 0) {
        // Could not open the file
        error = errno ;
        return false ;
    }
    if (ioctl(kI2CFileDescriptor, I2C_SLAVE, kAdafruitMCP9808I2CAddress) < 0) {
        // Could not open the device on the bus
        error = errno ;
        return false ;
    }
    return true ;
}

void AdafruitMCP9808::closeAdafruitMCP9808()
{
    if (kI2CFileDescriptor > 0) {
        close(kI2CFileDescriptor);
        // WARNING - This is not quite right, need to check for error first
        kI2CFileDescriptor = -1 ;
    }
}

// Read the given register on the Lidar-Lite
int AdafruitMCP9808::readAdafruitMCP9808(int readRegister)
{
    // Do not use i2c_smbus_read_byte_data here ; AdafruitMCP9808 V2 needs STOP between write and read
    int toReturn ;
    toReturn = i2c_smbus_write_byte(kI2CFileDescriptor, readRegister) ;
    if (toReturn < 0) {
        error = errno ;
        toReturn = -1 ;
    }
    toReturn = i2c_smbus_read_byte(kI2CFileDescriptor) ;
    if (toReturn < 0) {
        error = errno ;
        toReturn = -1 ;
    }
    return toReturn ;
}

/*
// Write the the given value to the given register on the Lidar-Lite
int AdafruitMCP9808::writeAdafruitMCP9808(int writeRegister, int writeValue)
{
    int toReturn = i2c_smbus_write_byte_data(kI2CFileDescriptor, writeRegister, writeValue);
    // Wait a little bit to make sure it settles
    usleep(10000);
    if (toReturn < 0) {
        error = errno ;
        toReturn = -1 ;
    }
    return toReturn ;

}*/

// Return the current calculated distance in centimeters
int AdafruitMCP9808::getTemp()
{
    int ioResult ;
    int msb, lsb ;
    ioResult = writeAdafruitMCP9808(kAdafruitMCP9808CommandControlRegister,kAdafruitMCP9808Measure);
    if (ioResult < 0) {
        return ioResult ;
    }
    ioResult = readAdafruitMCP9808(kAdafruitMCP9808TempMSB);
    if (ioResult < 0) {
        return ioResult ;
    } else {
        msb = ioResult ;
    }
    ioResult = readAdafruitMCP9808(kAdafruitMCP9808TempLSB);
    if (ioResult < 0) {
        return ioResult ;
    } else {
        lsb = ioResult ;
    }

    int distance = (msb << 8) + lsb ;

    return distance ;
}

// Return the last i/o error
int AdafruitMCP9808::getError()
{
    return error ;
}
