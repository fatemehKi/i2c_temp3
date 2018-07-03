#include "adafruitMCP9808.h"
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
        printf("Error: %d",  error);
        printf("Can not open the file");

        return false ;
    }
    if (ioctl(kI2CFileDescriptor, I2C_SLAVE, kAdafruitMCP9808I2CAddress) < 0) {
        // Could not open the device on the bus
        error = errno ;
        
        printf("Error: %d",  error);
        printf("Can not open the device on the bus");
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

}

// Return the current calculated distance in centimeters
float AdafruitMCP9808::getTemp()
{
 /* float t = read(MCP9808_REG_AMBIENT_TEMP);

  float temp = t & 0x0FFF;
  temp /=  16.0;
  if (t & 0x1000) temp -= 256;

    return temp;*/
    
    //float temp = readAdafruitMCP9808
    
        
    unsigned short int ioResult ;
    unsigned short int msb, lsb ;
    ioResult = writeAdafruitMCP9808(kAdafruitMCP9808CommandControlRegister,kAdafruitMCP9808Measure);
    if (ioResult < 0) {
        return ioResult ;
    }
    ioResult = readAdafruitMCP9808(MCP9808_REG_AMBIENT_TEMP);
    if (ioResult < 0) {
        return ioResult ;
    } else {
        msb = ioResult & 0x0FFF;
        msb /=  16.0;
        printf("MSB: %u\n",msb) ;
    }
   // ioResult = readAdafruitMCP9808(kAdafruitMCP9808TempLSB);
   // if (ioResult < 0) {
      //  return ioResult ;
   // } else {
     //   lsb = ioResult ;
     //   printf("LSB: %d\n",lsb) ;
    //}

   // float temp = (msb << 8) + lsb ;
    float temp = msb ;

return temp ;
    
}

// Return the last i/o error
int AdafruitMCP9808::getError()
{
    return error ;
}

/*uint16_t AdafruitMCP9808::read16(uint8_t reg) {
  uint16_t val;

  Wire.beginTransmission(_i2caddr);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();
  
  Wire.requestFrom((uint8_t)_i2caddr, (uint8_t)2);
  val = Wire.read();
  val <<= 8;
  val |= Wire.read();  
  return val;  
}
*/
