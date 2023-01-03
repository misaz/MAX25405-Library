#include "MAX25405.h"
#include "MAX25405_PlatformSpecific.h"

#include <stdint.h>
#include <stddef.h>

static MAX25405_Status MAX25405_ReadRegisters(MAX25405_Device* dev, uint8_t address, uint8_t* buffer, size_t bufferSize) {
	if (dev->useSpi) {
		return MAX25405_PlatformSpecific_SPI_Read(dev, address, buffer, bufferSize);
	} else {
		return MAX25405_PlatformSpecific_I2C_Read(dev, address, buffer, bufferSize);
	}
}

static MAX25405_Status MAX25405_WriteRegisters(MAX25405_Device* dev, uint8_t address, uint8_t* buffer, size_t bufferSize) {
	if (dev->useSpi) {
		return MAX25405_PlatformSpecific_SPI_Write(dev, address, buffer, bufferSize);
	} else {
		return MAX25405_PlatformSpecific_I2C_Write(dev, address, buffer, bufferSize);
	}
}

MAX25405_Status MAX25405_InitI2C(MAX25405_Device* dev, MAX25405_I2CAddress address) {
	MAX25405_Status status;

	dev->useSpi = 0;
	dev->address = address;

	status = MAX25405_PlatformSpecific_I2C_Init(dev);
	if (status) {
		return status;
	}

	uint8_t mainCfg2Reg;

	status = MAX25405_ReadRegisters(dev, MAX25405_REG_MAIN_CFG_2, &mainCfg2Reg, sizeof(mainCfg2Reg));
	if (status) {
		return status;
	}

	dev->enableOneShotMode = MAX25405_GET_FIELD(MAX25405_MAIN_CFG2_OSEN_FIELD, mainCfg2Reg);
	dev->enableShutdownMode = MAX25405_GET_FIELD(MAX25405_MAIN_CFG2_SHDN_FIELD, mainCfg2Reg);

	return MAX25405_Status_Ok;
}

MAX25405_Status MAX25405_InitSPI(MAX25405_Device* dev, uint8_t chipSelectPortNumber) {
	MAX25405_Status status;

	if (chipSelectPortNumber > 127) {
		return MAX25405_Status_BadArg;
	}

	dev->useSpi = 1;
	dev->address = chipSelectPortNumber;

	status = MAX25405_PlatformSpecific_SPI_Init(dev);
	if (status) {
		return status;
	}

	uint8_t mainCfg2Reg;

	status = MAX25405_ReadRegisters(dev, MAX25405_REG_MAIN_CFG_2, &mainCfg2Reg, sizeof(mainCfg2Reg));
	if (status) {
		return status;
	}

	dev->enableOneShotMode = MAX25405_GET_FIELD(MAX25405_MAIN_CFG2_OSEN_FIELD, mainCfg2Reg);
	dev->enableShutdownMode = MAX25405_GET_FIELD(MAX25405_MAIN_CFG2_SHDN_FIELD, mainCfg2Reg);

	return MAX25405_Status_Ok;
}

MAX25405_Status MAX25405_Deinit(MAX25405_Device* dev) {
	if (dev->useSpi) {
		return MAX25405_PlatformSpecific_SPI_Deinit(dev);
	} else {
		return MAX25405_PlatformSpecific_I2C_Deinit(dev);
	}
}

MAX25405_Status MAX25405_Reset(MAX25405_Device* dev) {
	MAX25405_Status status;

	uint8_t mainCfg2Value =
		MAX25405_SET_FIELD(MAX25405_MAIN_CFG2_OSEN_FIELD, dev->enableOneShotMode) |
		MAX25405_SET_FIELD(MAX25405_MAIN_CFG2_SHDN_FIELD, dev->enableShutdownMode) |
		MAX25405_SET_FIELD(MAX25405_MAIN_CFG2_RESET_FIELD, 1);

	status = MAX25405_WriteRegisters(dev, MAX25405_REG_MAIN_CFG_2, &mainCfg2Value, sizeof(mainCfg2Value));
	if (status) {
		return status;
	}

	// set default values to "cache":
	dev->enableOneShotMode = 0;
	dev->enableShutdownMode = 0;

	return MAX25405_Status_Ok;
}

MAX25405_Status MAX25405_ForceSync(MAX25405_Device* dev) {
	uint8_t mainCfg2Value =
		MAX25405_SET_FIELD(MAX25405_MAIN_CFG2_OSEN_FIELD, dev->enableOneShotMode) |
		MAX25405_SET_FIELD(MAX25405_MAIN_CFG2_SHDN_FIELD, dev->enableShutdownMode) |
		MAX25405_SET_FIELD(MAX25405_MAIN_CFG2_SYNC_FIELD, 1);

	return MAX25405_WriteRegisters(dev, MAX25405_REG_MAIN_CFG_2, &mainCfg2Value, sizeof(mainCfg2Value));
}
MAX25405_Status MAX25405_TriggerOneShoot(MAX25405_Device* dev) {
	uint8_t mainCfg2Value =
		MAX25405_SET_FIELD(MAX25405_MAIN_CFG2_OSEN_FIELD, dev->enableOneShotMode) |
		MAX25405_SET_FIELD(MAX25405_MAIN_CFG2_SHDN_FIELD, dev->enableShutdownMode) |
		MAX25405_SET_FIELD(MAX25405_MAIN_CFG2_OSTRIG_FIELD, 1);

	return MAX25405_WriteRegisters(dev, MAX25405_REG_MAIN_CFG_2, &mainCfg2Value, sizeof(mainCfg2Value));
}

MAX25405_Status MAX25405_GetPendingInterrupts(MAX25405_Device* dev, MAX25405_Interrupt* pendingInterrupts) {
	MAX25405_Status status;

	uint8_t interrupts;

	status = MAX25405_ReadRegisters(dev, MAX25405_REG_INTERRUPT_STATUS, &interrupts, sizeof(interrupts));
	if (status) {
		return status;
	}

	*pendingInterrupts = (MAX25405_Interrupt)interrupts;

	return MAX25405_Status_Ok;
}

MAX25405_Status MAX25405_GetDefaultConfiguration(MAX25405_Configuration* config) {
	config->mainConfig.modeOfOperation = MAX25405_ModeOfOperation_SuperProximityMode;
	config->mainConfig.enableEndOfConversionInterrupt = 0;
	config->mainConfig.externalSyncMode = MAX25405_ExternalSync_NoFunction;
	config->mainConfig.enableShutdownMode = 0;
	config->mainConfig.enableOneShotMode = 0;

	config->sequencingConfig.endOfConversionDelay = MAX25405_EndOfConversionDelay_99_94ms;
	config->sequencingConfig.integrationTime = MAX25405_IntegrationTime_50us;
	config->sequencingConfig.numberOfRepeats = MAX25405_NumberOfRepeats_16;
	config->sequencingConfig.coherentDoubleSamplingMode = MAX25405_CoherentDoubleSamplingMode_SeqA_Add_SeqB_Sub;
	config->sequencingConfig.numberOfCoherentDoubleSamples = MAX25405_NumberOfCoherentDoubleSamples_16;
	config->sequencingConfig.capFlipping = MAX25405_CapFlipping_Enabled;

	config->afeConfig.enableCoarseAmbientLightCompensation = 1;
	config->afeConfig.afePgaGain = MAX25405_AfePgaGain_1;
	config->afeConfig.afePolarization = MAX25405_AfePolarizationMode_Unipolar;
	config->afeConfig.afeCapacitance = MAX25405_AfeCapacitance_0_133pF;
	config->afeConfig.afeAlcIsel = MAX25405_AfeAlcIsel_10uA;
	config->afeConfig.afeReturnToZero = MAX25405_AfeReturnToZero_Disabled;

	config->ledConfig.ledDrive = MAX25405_LedDrive_PWM_1_16;
	config->ledConfig.enableDriveCurrentOutput = 0;
	config->ledConfig.enableDrivePwmOutput = 0;
	config->ledConfig.externalLedPolarity = MAX25405_ExternalLedPolarity_DriveNMOS;

	config->ledConfig.columnGainMode = MAX25405_ColumnGainModeSelection_Configuration;
	for (int i = 0; i < MAX25405_COLUMNS; i++) {
		config->columnGain[i] = MAX25405_ColumnGain_1_00;
	}

	return MAX25405_Status_Ok;
}

MAX25405_Status MAX25405_GetConfiguration(MAX25405_Device* dev, MAX25405_Configuration* config) {
	MAX25405_Status status;
	uint8_t cfgRegs[6];

	// shift left efficiently divides number by 2. 
	// It is because each config byte contains values for two columns
	uint8_t colGainRegs[MAX25405_COLUMNS >> 1];
	uint8_t ledCtrlReg;

	status = MAX25405_ReadRegisters(dev, MAX25405_REG_MAIN_CFG_1, cfgRegs, sizeof(cfgRegs));
	if (status) {
		return status;
	}

	status = MAX25405_ReadRegisters(dev, MAX25405_REG_COL_GAIN_1_2, colGainRegs, sizeof(colGainRegs));
	if (status) {
		return status;
	}

	status = MAX25405_ReadRegisters(dev, MAX25405_REG_LED_CTRL, &ledCtrlReg, sizeof(ledCtrlReg));
	if (status) {
		return status;
	}

	config->mainConfig.enableEndOfConversionInterrupt = MAX25405_GET_FIELD(MAX25405_MAIN_CFG1_EOCINTE_FIELD, cfgRegs[0]);
	config->mainConfig.externalSyncMode = MAX25405_GET_FIELD(MAX25405_MAIN_CFG1_EXSYNC_FIELD, cfgRegs[0]);
	config->mainConfig.enableShutdownMode = MAX25405_GET_FIELD(MAX25405_MAIN_CFG2_SHDN_FIELD, cfgRegs[1]);
	config->mainConfig.modeOfOperation = MAX25405_GET_FIELD(MAX25405_MAIN_CFG2_MODE_FIELD, cfgRegs[1]);
	config->mainConfig.enableOneShotMode = MAX25405_GET_FIELD(MAX25405_MAIN_CFG2_OSEN_FIELD, cfgRegs[1]);

	config->sequencingConfig.endOfConversionDelay = MAX25405_GET_FIELD(MAX25405_SEQ_CFG1_SDLY_FIELD, cfgRegs[2]);
	config->sequencingConfig.integrationTime = MAX25405_GET_FIELD(MAX25405_SEQ_CFG1_TIM_FIELD, cfgRegs[2]);
	config->sequencingConfig.capFlipping = MAX25405_GET_FIELD(MAX25405_SEQ_CFG1_CRST_FIELD, cfgRegs[2]);
	config->sequencingConfig.numberOfRepeats = MAX25405_GET_FIELD(MAX25405_SEQ_CFG2_NRPT_FIELD, cfgRegs[3]);
	config->sequencingConfig.coherentDoubleSamplingMode = MAX25405_GET_FIELD(MAX25405_SEQ_CFG2_CDSMODE_FIELD, cfgRegs[3]);
	config->sequencingConfig.numberOfCoherentDoubleSamples = MAX25405_GET_FIELD(MAX25405_SEQ_CFG2_NCDS_FIELD, cfgRegs[3]);

	config->afeConfig.enableCoarseAmbientLightCompensation = MAX25405_GET_FIELD(MAX25405_AFE_CFG_ALCEN_FIELD, cfgRegs[4]);
	config->afeConfig.afePgaGain = MAX25405_GET_FIELD(MAX25405_AFE_CFG_PGA_FIELD, cfgRegs[4]);
	config->afeConfig.afePolarization = MAX25405_GET_FIELD(MAX25405_AFE_CFG_BIP_FIELD, cfgRegs[4]);
	config->afeConfig.afeCapacitance = MAX25405_GET_FIELD(MAX25405_AFE_CFG_CSEL_FIELD, cfgRegs[4]);
	config->afeConfig.afeAlcIsel = MAX25405_GET_FIELD(MAX25405_AFE_CFG_ALC_ISEL_FIELD, cfgRegs[4]);
	config->afeConfig.afeReturnToZero = MAX25405_GET_FIELD(MAX25405_AFE_CFG_RZOFF_FIELD, cfgRegs[4]);

	config->ledConfig.ledDrive = MAX25405_GET_FIELD(MAX25405_LED_CFG_DRV_FIELD, cfgRegs[5]);
	config->ledConfig.enableDriveCurrentOutput = MAX25405_GET_FIELD(MAX25405_LED_CTRL_DRV_EN_FIELD, ledCtrlReg);
	config->ledConfig.enableDrivePwmOutput = MAX25405_GET_FIELD(MAX25405_LED_CTRL_ELED_EN_FIELD, ledCtrlReg);
	config->ledConfig.externalLedPolarity = MAX25405_GET_FIELD(MAX25405_LED_CTRL_ELED_POL_FIELD, ledCtrlReg);
	config->ledConfig.columnGainMode = MAX25405_ColumnGainModeSelection_Configuration;

	for (int i = 0; i < (MAX25405_COLUMNS >> 1); i++) {
		config->columnGain[i * 2] = MAX25405_GET_FIELD(MAX25405_COL_GAIN_CFG_CGAIN1_FIELD, colGainRegs[i]);
		config->columnGain[i * 2 + 1] = MAX25405_GET_FIELD(MAX25405_COL_GAIN_CFG_CGAIN2_FIELD, colGainRegs[i]);
	}

	return MAX25405_Status_Ok;
}

MAX25405_Status MAX25405_SetConfiguration(MAX25405_Device* dev, MAX25405_Configuration* config) {
	MAX25405_Status status;

	uint8_t cfgRegs[6];

	// shift right efficiently divides number by 2. 
	// It is because each config byte contains values for two columns
	uint8_t colGainRegs[MAX25405_COLUMNS >> 1];
	uint8_t ledCtrlReg;

	// at the begining disable LED for prevent spurious lights when reconfiguring sensor
	ledCtrlReg =
		MAX25405_SET_FIELD(MAX25405_LED_CTRL_ELED_POL_FIELD, config->ledConfig.externalLedPolarity) |
		MAX25405_SET_FIELD(MAX25405_LED_CTRL_ELED_EN_FIELD, 0) |
		MAX25405_SET_FIELD(MAX25405_LED_CTRL_DRV_EN_FIELD, 0) |
		MAX25405_SET_FIELD(MAX25405_LED_CTRL_GAINSEL_FIELD, config->ledConfig.columnGainMode);

	status = MAX25405_WriteRegisters(dev, MAX25405_REG_LED_CTRL, &ledCtrlReg, sizeof(ledCtrlReg));
	if (status) {
		return status;
	}

	cfgRegs[0] =
		MAX25405_SET_FIELD(MAX25405_MAIN_CFG1_EXSYNC_FIELD, config->mainConfig.externalSyncMode) |
		MAX25405_SET_FIELD(MAX25405_MAIN_CFG1_EOCINTE_FIELD, config->mainConfig.enableEndOfConversionInterrupt);

	cfgRegs[1] =
		MAX25405_SET_FIELD(MAX25405_MAIN_CFG2_MODE_FIELD, config->mainConfig.modeOfOperation) |
		MAX25405_SET_FIELD(MAX25405_MAIN_CFG2_OSEN_FIELD, config->mainConfig.enableOneShotMode) |
		MAX25405_SET_FIELD(MAX25405_MAIN_CFG2_SHDN_FIELD, config->mainConfig.enableShutdownMode);

	cfgRegs[2] =
		MAX25405_SET_FIELD(MAX25405_SEQ_CFG1_SDLY_FIELD, config->sequencingConfig.endOfConversionDelay) |
		MAX25405_SET_FIELD(MAX25405_SEQ_CFG1_TIM_FIELD, config->sequencingConfig.integrationTime) |
		MAX25405_SET_FIELD(MAX25405_SEQ_CFG1_CRST_FIELD, config->sequencingConfig.capFlipping);

	cfgRegs[3] =
		MAX25405_SET_FIELD(MAX25405_SEQ_CFG2_CDSMODE_FIELD, config->sequencingConfig.coherentDoubleSamplingMode) |
		MAX25405_SET_FIELD(MAX25405_SEQ_CFG2_NCDS_FIELD, config->sequencingConfig.numberOfCoherentDoubleSamples) |
		MAX25405_SET_FIELD(MAX25405_SEQ_CFG2_NRPT_FIELD, config->sequencingConfig.numberOfRepeats);

	cfgRegs[4] =
		MAX25405_SET_FIELD(MAX25405_AFE_CFG_PGA_FIELD, config->afeConfig.afePgaGain) |
		MAX25405_SET_FIELD(MAX25405_AFE_CFG_BIP_FIELD, config->afeConfig.afePolarization) |
		MAX25405_SET_FIELD(MAX25405_AFE_CFG_CSEL_FIELD, config->afeConfig.afeCapacitance) |
		MAX25405_SET_FIELD(MAX25405_AFE_CFG_ALC_ISEL_FIELD, config->afeConfig.afeAlcIsel) |
		MAX25405_SET_FIELD(MAX25405_AFE_CFG_RZOFF_FIELD, config->afeConfig.afeReturnToZero) |
		MAX25405_SET_FIELD(MAX25405_AFE_CFG_ALCEN_FIELD, config->afeConfig.enableCoarseAmbientLightCompensation);

	cfgRegs[5] =
		MAX25405_SET_FIELD(MAX25405_LED_CFG_DRV_FIELD, config->ledConfig.ledDrive);

	status = MAX25405_WriteRegisters(dev, MAX25405_REG_MAIN_CFG_1, cfgRegs, sizeof(cfgRegs));
	if (status) {
		return status;
	}

	for (int i = 0; i < (MAX25405_COLUMNS >> 1); i++) {
		colGainRegs[i] =
			MAX25405_SET_FIELD(MAX25405_COL_GAIN_CFG_CGAIN1_FIELD, config->columnGain[i * 2]) |
			MAX25405_SET_FIELD(MAX25405_COL_GAIN_CFG_CGAIN2_FIELD, config->columnGain[i * 2 + 1]);
	}

	status = MAX25405_WriteRegisters(dev, MAX25405_REG_COL_GAIN_1_2, colGainRegs, sizeof(colGainRegs));
	if (status) {
		return status;
	}

	ledCtrlReg =
		MAX25405_SET_FIELD(MAX25405_LED_CTRL_ELED_POL_FIELD, config->ledConfig.externalLedPolarity) |
		MAX25405_SET_FIELD(MAX25405_LED_CTRL_ELED_EN_FIELD, config->ledConfig.enableDrivePwmOutput) |
		MAX25405_SET_FIELD(MAX25405_LED_CTRL_DRV_EN_FIELD, config->ledConfig.enableDriveCurrentOutput) |
		MAX25405_SET_FIELD(MAX25405_LED_CTRL_GAINSEL_FIELD, config->ledConfig.columnGainMode);

	status = MAX25405_WriteRegisters(dev, MAX25405_REG_LED_CTRL, &ledCtrlReg, sizeof(ledCtrlReg));
	if (status) {
		return status;
	}

	return MAX25405_Status_Ok;
}

MAX25405_Status MAX25405_GetSinglePixelData(MAX25405_Device* dev, int16_t* value, int column, int row) {
	MAX25405_Status status;
	uint8_t pixelBuffer[2];

	uint8_t regAddress = MAX25405_REG_ADC_DATA + (row * MAX25405_COLUMNS + column) * 2;

	status = MAX25405_ReadRegisters(dev, regAddress, pixelBuffer, sizeof(pixelBuffer));
	if (status) {
		return status;
	}

	uint16_t byteH = (uint16_t)(pixelBuffer[0]);
	uint16_t byteL = (uint16_t)(pixelBuffer[1]);
	*value = (int16_t)((byteH << 8) | byteL);

	return MAX25405_Status_Ok;
}

MAX25405_Status MAX25405_GetPixelsData(MAX25405_Device* dev, int16_t* values, int column, int row, int pixelsCount) {
	MAX25405_Status status;

	uint8_t regAddress = MAX25405_REG_ADC_DATA + (row * MAX25405_COLUMNS + column) * 2;
	uint8_t* rawBuffer = (uint8_t*)values;

	status = MAX25405_ReadRegisters(dev, regAddress, rawBuffer, pixelsCount * 2);
	if (status) {
		return status;
	}

	for (size_t i = 0; i < pixelsCount; i++) {
		uint8_t swap = rawBuffer[i * 2];
		rawBuffer[i * 2] = rawBuffer[i * 2 + 1];
		rawBuffer[i * 2 + 1] = swap;
	}

	return MAX25405_Status_Ok;
}

MAX25405_Status MAX25405_GetAllPixelData(MAX25405_Device* dev, int16_t* values) {
	return MAX25405_GetPixelsData(dev, values, 0, 0, MAX25405_PIXELS);
}
