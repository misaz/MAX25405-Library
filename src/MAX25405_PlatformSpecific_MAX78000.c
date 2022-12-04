#include "MAX25405.h"
#include "MAX25405_PlatformSpecific.h"

#include "mxc_device.h"
#include "i2c.h"
#include "spi.h"

#include <string.h>
#include <stdint.h>

static int lastI2CTransactionStatus = 0;
static int lastSPITransactionStatus = 0;

static void MAX25405_PlatformSpecific_I2C_Callback(mxc_i2c_req_t* reqest, int status) {
    lastI2CTransactionStatus = status;
}

static void MAX25405_PlatformSpecific_SPI_Callback(void* req, int status) {
    lastSPITransactionStatus = status;
}

MAX25405_Status MAX25405_PlatformSpecific_I2C_Init(MAX25405_Device* dev) {
    int status;

    status = MXC_I2C_Init(MAX25405_I2C, 1, 0);
    if (status) {
        return MAX25405_Status_BusError;
    }

    status = MXC_I2C_SetFrequency(MAX25405_I2C, MAX25405_I2C_FREQUENCY);
    if (status) {
        return MAX25405_Status_BusError;
    }

    return MAX25405_Status_Ok;
}

MAX25405_Status MAX25405_PlatformSpecific_I2C_Deinit(MAX25405_Device* dev) {
    int status;

    status = MXC_I2C_Shutdown(MAX25405_I2C);
    if (status) {
        return MAX25405_Status_BusError;
    }

    return MAX25405_Status_Ok;
}

MAX25405_Status MAX25405_PlatformSpecific_I2C_Write(MAX25405_Device* dev, uint8_t registerAddress, uint8_t* buffer, size_t bufferSize) {
    int status;

    // maximum transaction size for write is 1 byte addr + 6 byte cfg registers
    uint8_t bufferWithAddress[7];

    if (bufferSize > 6) {
        return MAX25405_Status_NotSupported;
    }

    bufferWithAddress[0] = registerAddress;
    memcpy(bufferWithAddress + 1, buffer, bufferSize);

    mxc_i2c_req_t transaction;
    transaction.i2c = MAX25405_I2C;
    transaction.addr = dev->address;
    transaction.tx_buf = bufferWithAddress;
    transaction.tx_len = bufferSize + 1;
    transaction.rx_buf = NULL;
    transaction.rx_len = 0;
    transaction.restart = 0;
    transaction.callback = MAX25405_PlatformSpecific_I2C_Callback;

    lastI2CTransactionStatus  = -1;
    
    status = MXC_I2C_MasterTransaction(&transaction);
    if (status) {
        return MAX25405_Status_BusError;
    }

    while (lastI2CTransactionStatus == -1) {
        __WFI();
    }

    if (lastI2CTransactionStatus != 0) {
        return MAX25405_Status_BusError;
    }

    return MAX25405_Status_Ok;
}

MAX25405_Status MAX25405_PlatformSpecific_I2C_Read(MAX25405_Device* dev, uint8_t registerAddress, uint8_t* buffer, size_t bufferSize) {
    int status;

    mxc_i2c_req_t transaction;
    transaction.i2c = MAX25405_I2C;
    transaction.addr = dev->address;
    transaction.tx_buf = &registerAddress;
    transaction.tx_len = sizeof(registerAddress);
    transaction.rx_buf = buffer;
    transaction.rx_len = bufferSize;
    transaction.restart = 0;
    transaction.callback = MAX25405_PlatformSpecific_I2C_Callback;

    lastI2CTransactionStatus  = -1;
    
    status = MXC_I2C_MasterTransaction(&transaction);
    if (status) {
        return MAX25405_Status_BusError;
    }

    while (lastI2CTransactionStatus == -1) {
        __WFI();
    }

    if (lastI2CTransactionStatus != 0) {
        return MAX25405_Status_BusError;
    }

    return MAX25405_Status_Ok;
}

MAX25405_Status MAX25405_PlatformSpecific_SPI_Init(MAX25405_Device* dev) {
    int status;

    if (dev->address > 2) {
        return MAX25405_Status_BadArg;
    }

    mxc_spi_pins_t pins;
    pins.clock = true;
    pins.miso = true;
    pins.mosi = true;
    pins.ss0 = true;
    pins.vddioh = true;
    pins.ss1 = false;
    pins.ss2 = false;
    pins.sdio2 = false;
    pins.sdio3 = false;

    status = MXC_SPI_Init(MAX25405_SPI, 1, 0, MAX25405_SPI_MAX_SLAVES_COUNT, 0, MAX25405_SPI_SPEED, pins);
    if (status) {
        return MAX25405_Status_BusError;
    }

    return MAX25405_Status_Ok;
}

MAX25405_Status MAX25405_PlatformSpecific_SPI_Deinit(MAX25405_Device* dev) {
    int status;

    status = MXC_SPI_Shutdown(MAX25405_SPI);
    if (status) {
        return MAX25405_Status_BusError;
    }

    return MAX25405_Status_Ok;
}

MAX25405_Status MAX25405_PlatformSpecific_SPI_Write(MAX25405_Device* dev, uint8_t registerAddress, uint8_t* buffer, size_t bufferSize) {
    int status;
    mxc_spi_req_t transaction;

    // maixmum size is 1 byte (reg. address) + 1 byte (command r/w) + 6 cfg registers.
    uint8_t wrBuffer[8];

    if (bufferSize > 6) {
        return MAX25405_Status_NotSupported;
    }
    
    status = MXC_SPI_SetDataSize(MAX25405_SPI, 8);
    if (status) {
        return MAX25405_Status_BusError;
    }

    status = MXC_SPI_SetWidth(MAX25405_SPI, SPI_WIDTH_STANDARD);
    if (status) {
        return MAX25405_Status_BusError;
    }

    wrBuffer[0] = registerAddress;
    wrBuffer[1] = 0x00; // write
    memcpy(wrBuffer + 2, buffer, bufferSize);

    transaction.spi = MAX25405_SPI;
    transaction.txData = wrBuffer;
    transaction.txLen = bufferSize + 2;
    transaction.rxData = NULL;
    transaction.rxLen = 0;
    transaction.ssIdx = 0;
    transaction.ssDeassert = 1;
    transaction.txCnt = 0;
    transaction.rxCnt = 0;
    transaction.completeCB = MAX25405_PlatformSpecific_SPI_Callback;

    status = MXC_SPI_MasterTransaction(&transaction);
    if (status) {
        return MAX25405_Status_BusError;
    }

    return MAX25405_Status_Ok;
}

MAX25405_Status MAX25405_PlatformSpecific_SPI_Read(MAX25405_Device* dev, uint8_t registerAddress, uint8_t* buffer, size_t bufferSize) {
    int status;
    mxc_spi_req_t transaction;

    // maixmum size is 1 byte (reg. address) + 1 byte (command r/w)
    uint8_t wrBuffer[2];
    
    status = MXC_SPI_SetDataSize(MAX25405_SPI, 8);
    if (status) {
        return MAX25405_Status_BusError;
    }

    status = MXC_SPI_SetWidth(MAX25405_SPI, SPI_WIDTH_STANDARD);
    if (status) {
        return MAX25405_Status_BusError;
    }

    wrBuffer[0] = registerAddress;
    wrBuffer[1] = 0x80; // read

    transaction.spi = MAX25405_SPI;
    transaction.ssIdx = 0;
    transaction.completeCB = MAX25405_PlatformSpecific_SPI_Callback;

    transaction.txData = wrBuffer;
    transaction.txLen = sizeof(wrBuffer);
    transaction.rxData = NULL;
    transaction.rxLen = 0;
    transaction.ssDeassert = 0;
    transaction.txCnt = 0;
    transaction.rxCnt = 0;

    MXC_SPI_MasterTransaction(&transaction);
    if (status) {
        return MAX25405_Status_BusError;
    }
    
    transaction.txData = NULL;
    transaction.txLen = 0;
    transaction.rxData = buffer;
    transaction.rxLen = bufferSize;
    transaction.ssDeassert = 1;
    transaction.txCnt = 0;
    transaction.rxCnt = 0;
    
    status = MXC_SPI_MasterTransaction(&transaction);
    if (status) {
        return MAX25405_Status_BusError;
    }

    return MAX25405_Status_Ok;
}