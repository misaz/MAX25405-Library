#ifndef MAX25405_PLATFORM_SPECIFIC_H
#define MAX25405_PLATFORM_SPECIFIC_H

#include "MAX25405.h"

#include <stdint.h>
#include <stddef.h>

#include "mxc_device.h"

// comment out one of the following line if you want use only one bus
#define MAX25405_USE_SPI
#define MAX25405_USE_I2C

#ifdef MAX25405_USE_SPI
    #define MAX25405_SPI                    MXC_SPI0
    #define MAX25405_SPI_SPEED              1000000
    #define MAX25405_SPI_MAX_SLAVES_COUNT   1
#endif

#ifdef MAX25405_USE_I2C
    #define MAX25405_I2C                    MXC_I2C1
    #define MAX25405_I2C_FREQUENCY          400000
#endif

MAX25405_Status MAX25405_PlatformSpecific_I2C_Init(MAX25405_Device* dev);
MAX25405_Status MAX25405_PlatformSpecific_I2C_Deinit(MAX25405_Device* dev);
MAX25405_Status MAX25405_PlatformSpecific_I2C_Write(MAX25405_Device* dev, uint8_t registerAddress, uint8_t* buffer, size_t bufferSize);
MAX25405_Status MAX25405_PlatformSpecific_I2C_Read(MAX25405_Device* dev, uint8_t registerAddress, uint8_t* buffer, size_t bufferSize);

MAX25405_Status MAX25405_PlatformSpecific_SPI_Init(MAX25405_Device* dev);
MAX25405_Status MAX25405_PlatformSpecific_SPI_Deinit(MAX25405_Device* dev);
MAX25405_Status MAX25405_PlatformSpecific_SPI_Write(MAX25405_Device* dev, uint8_t registerAddress, uint8_t* buffer, size_t bufferSize);
MAX25405_Status MAX25405_PlatformSpecific_SPI_Read(MAX25405_Device* dev, uint8_t registerAddress, uint8_t* buffer, size_t bufferSize);

#endif
