#ifndef MAX25405_PLATFORM_SPECIFIC_H
#define MAX25405_PLATFORM_SPECIFIC_H

#include "MAX25405.h"

#include <stdint.h>
#include <stddef.h>

MAX25405_Status MAX25405_PlatformSpecific_I2C_Init(MAX25405_Device* dev);
MAX25405_Status MAX25405_PlatformSpecific_I2C_Deinit(MAX25405_Device* dev);
MAX25405_Status MAX25405_PlatformSpecific_I2C_Write(MAX25405_Device* dev, uint8_t registerAddress, uint8_t* buffer, size_t bufferSize);
MAX25405_Status MAX25405_PlatformSpecific_I2C_Read(MAX25405_Device* dev, uint8_t registerAddress, uint8_t* buffer, size_t bufferSize);

MAX25405_Status MAX25405_PlatformSpecific_SPI_Init(MAX25405_Device* dev);
MAX25405_Status MAX25405_PlatformSpecific_SPI_Deinit(MAX25405_Device* dev);
MAX25405_Status MAX25405_PlatformSpecific_SPI_Write(MAX25405_Device* dev, uint8_t registerAddress, uint8_t* buffer, size_t bufferSize);
MAX25405_Status MAX25405_PlatformSpecific_SPI_Read(MAX25405_Device* dev, uint8_t registerAddress, uint8_t* buffer, size_t bufferSize);

#endif
