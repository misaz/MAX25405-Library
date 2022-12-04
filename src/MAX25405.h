#ifndef MAX25405_H
#define MAX25405_H

#include <stdint.h>

#define MAX25405_COLUMNS					10
#define MAX25405_ROWS						6
#define MAX25405_PIXELS						((MAX25405_COLUMNS) * (MAX25405_ROWS))

#define MAX25405_I2C_READ_ADDRESS(dev)		(((dev)->i2cAddress << 1) | 0x01)
#define MAX25405_I2C_WRITE_ADDRESS(dev)		(((dev)->i2cAddress << 1) | 0x00)

#define MAX25405_REG_INTERRUPT_STATUS		0x00
#define MAX25405_REG_MAIN_CFG_1				0x01
#define MAX25405_REG_MAIN_CFG_2				0x02
#define MAX25405_REG_SEQ_CFG_1				0x03
#define MAX25405_REG_SEQ_CFG_2				0x04
#define MAX25405_REG_AFE_CFG				0x05
#define MAX25405_REG_LED_CFG				0x06
#define MAX25405_REG_ADC_DATA				0x10
#define MAX25405_REG_COL_GAIN_1_2			0xA5
#define MAX25405_REG_COL_GAIN_3_4			0xA6
#define MAX25405_REG_COL_GAIN_5_6			0xA7
#define MAX25405_REG_COL_GAIN_7_8			0xA8
#define MAX25405_REG_COL_GAIN_9_10			0xA9
#define MAX25405_REG_LED_CTRL				0xC1

#define MAX25405_EXPAND_MACRO(content)		content
#define MAX25405_FIELD(NAME)				NAME##_MASK, NAME##_OFFSET
#define MAX25405_GET_FIELD_IMPL(FIELD_MASK,FIELD_OFFSET,value)	(((value) >> FIELD_OFFSET) & FIELD_MASK)
#define MAX25405_SET_FIELD_IMPL(FIELD_MASK,FIELD_OFFSET,value)	(((value) & FIELD_MASK) << FIELD_OFFSET)
#define MAX25405_GET_FIELD(...)				MAX25405_EXPAND_MACRO(MAX25405_GET_FIELD_IMPL(__VA_ARGS__))
#define MAX25405_SET_FIELD(...)				MAX25405_EXPAND_MACRO(MAX25405_SET_FIELD_IMPL(__VA_ARGS__))

#define MAX25405_INT_STATUS_EOCINTS_MASK	0x1
#define MAX25405_INT_STATUS_EOCINTS_OFFSET	2
#define MAX25405_INT_STATUS_EOCINTS_FIELD	MAX25405_FIELD(MAX25405_INT_STATUS_EOCINTS)
#define MAX25405_INT_STATUS_PWRON_MASK		0x1
#define MAX25405_INT_STATUS_PWRON_OFFSET	4
#define MAX25405_INT_STATUS_PWRON_FIELD		MAX25405_FIELD(MAX25405_INT_STATUS_PWRON)
#define MAX25405_MAIN_CFG1_EXSYNC_MASK		0x7
#define MAX25405_MAIN_CFG1_EXSYNC_OFFSET	4
#define MAX25405_MAIN_CFG1_EXSYNC_FIELD		MAX25405_FIELD(MAX25405_MAIN_CFG1_EXSYNC)
#define MAX25405_MAIN_CFG1_EOCINTE_MASK		0x1
#define MAX25405_MAIN_CFG1_EOCINTE_OFFSET	2
#define MAX25405_MAIN_CFG1_EOCINTE_FIELD	MAX25405_FIELD(MAX25405_MAIN_CFG1_EOCINTE)
#define MAX25405_MAIN_CFG2_MODE_MASK		0x3
#define MAX25405_MAIN_CFG2_MODE_OFFSET		0
#define MAX25405_MAIN_CFG2_MODE_FIELD		MAX25405_FIELD(MAX25405_MAIN_CFG2_MODE)
#define MAX25405_MAIN_CFG2_OSTRIG_MASK		0x1
#define MAX25405_MAIN_CFG2_OSTRIG_OFFSET	2
#define MAX25405_MAIN_CFG2_OSTRIG_FIELD		MAX25405_FIELD(MAX25405_MAIN_CFG2_OSTRIG)
#define MAX25405_MAIN_CFG2_OSEN_MASK		0x1
#define MAX25405_MAIN_CFG2_OSEN_OFFSET		3
#define MAX25405_MAIN_CFG2_OSEN_FIELD		MAX25405_FIELD(MAX25405_MAIN_CFG2_OSEN)
#define MAX25405_MAIN_CFG2_SYNC_MASK		0x1
#define MAX25405_MAIN_CFG2_SYNC_OFFSET		4
#define MAX25405_MAIN_CFG2_SYNC_FIELD		MAX25405_FIELD(MAX25405_MAIN_CFG2_SYNC)
#define MAX25405_MAIN_CFG2_RESET_MASK		0x1
#define MAX25405_MAIN_CFG2_RESET_OFFSET		6
#define MAX25405_MAIN_CFG2_RESET_FIELD		MAX25405_FIELD(MAX25405_MAIN_CFG2_RESET)
#define MAX25405_MAIN_CFG2_SHDN_MASK		0x1
#define MAX25405_MAIN_CFG2_SHDN_OFFSET		7
#define MAX25405_MAIN_CFG2_SHDN_FIELD		MAX25405_FIELD(MAX25405_MAIN_CFG2_SHDN)
#define MAX25405_SEQ_CFG1_CRST_MASK			0x1
#define MAX25405_SEQ_CFG1_CRST_OFFSET		0
#define MAX25405_SEQ_CFG1_CRST_FIELD		MAX25405_FIELD(MAX25405_SEQ_CFG1_CRST)
#define MAX25405_SEQ_CFG1_TIM_MASK			0x7
#define MAX25405_SEQ_CFG1_TIM_OFFSET		1
#define MAX25405_SEQ_CFG1_TIM_FIELD			MAX25405_FIELD(MAX25405_SEQ_CFG1_TIM)
#define MAX25405_SEQ_CFG1_SDLY_MASK			0xF
#define MAX25405_SEQ_CFG1_SDLY_OFFSET		4
#define MAX25405_SEQ_CFG1_SDLY_FIELD		MAX25405_FIELD(MAX25405_SEQ_CFG1_SDLY)
#define MAX25405_SEQ_CFG2_CDSMODE_MASK		0x1
#define MAX25405_SEQ_CFG2_CDSMODE_OFFSET	1
#define MAX25405_SEQ_CFG2_CDSMODE_FIELD		MAX25405_FIELD(MAX25405_SEQ_CFG2_CDSMODE)
#define MAX25405_SEQ_CFG2_NCDS_MASK			0x7
#define MAX25405_SEQ_CFG2_NCDS_OFFSET		2
#define MAX25405_SEQ_CFG2_NCDS_FIELD		MAX25405_FIELD(MAX25405_SEQ_CFG2_NCDS)
#define MAX25405_SEQ_CFG2_NRPT_MASK			0x7
#define MAX25405_SEQ_CFG2_NRPT_OFFSET		5
#define MAX25405_SEQ_CFG2_NRPT_FIELD		MAX25405_FIELD(MAX25405_SEQ_CFG2_NRPT)
#define MAX25405_AFE_CFG_PGA_MASK			0x3
#define MAX25405_AFE_CFG_PGA_OFFSET			0
#define MAX25405_AFE_CFG_PGA_FIELD			MAX25405_FIELD(MAX25405_AFE_CFG_PGA)
#define MAX25405_AFE_CFG_BIP_MASK			0x1
#define MAX25405_AFE_CFG_BIP_OFFSET			2
#define MAX25405_AFE_CFG_BIP_FIELD			MAX25405_FIELD(MAX25405_AFE_CFG_BIP)
#define MAX25405_AFE_CFG_ALCEN_MASK			0x1
#define MAX25405_AFE_CFG_ALCEN_OFFSET		3
#define MAX25405_AFE_CFG_ALCEN_FIELD		MAX25405_FIELD(MAX25405_AFE_CFG_ALCEN)
#define MAX25405_AFE_CFG_CSEL_MASK			0x3
#define MAX25405_AFE_CFG_CSEL_OFFSET		4
#define MAX25405_AFE_CFG_CSEL_FIELD			MAX25405_FIELD(MAX25405_AFE_CFG_CSEL)
#define MAX25405_AFE_CFG_ALC_ISEL_MASK		0x1
#define MAX25405_AFE_CFG_ALC_ISEL_OFFSET	6
#define MAX25405_AFE_CFG_ALC_ISEL_FIELD		MAX25405_FIELD(MAX25405_AFE_CFG_ALC_ISEL)
#define MAX25405_AFE_CFG_RZOFF_MASK			0x1
#define MAX25405_AFE_CFG_RZOFF_OFFSET		7
#define MAX25405_AFE_CFG_RZOFF_FIELD		MAX25405_FIELD(MAX25405_AFE_CFG_RZOFF)
#define MAX25405_LED_CFG_DRV_MASK			0xF
#define MAX25405_LED_CFG_DRV_OFFSET			0
#define MAX25405_LED_CFG_DRV_FIELD			MAX25405_FIELD(MAX25405_LED_CFG_DRV)
#define MAX25405_COL_GAIN_CFG_CGAIN1_MASK	0xF
#define MAX25405_COL_GAIN_CFG_CGAIN1_OFFSET	0
#define MAX25405_COL_GAIN_CFG_CGAIN1_FIELD	MAX25405_FIELD(MAX25405_COL_GAIN_CFG_CGAIN1)
#define MAX25405_COL_GAIN_CFG_CGAIN2_MASK	0xF
#define MAX25405_COL_GAIN_CFG_CGAIN2_OFFSET	4
#define MAX25405_COL_GAIN_CFG_CGAIN2_FIELD	MAX25405_FIELD(MAX25405_COL_GAIN_CFG_CGAIN2)
#define MAX25405_LED_CTRL_ELED_POL_MASK		0x1
#define MAX25405_LED_CTRL_ELED_POL_OFFSET	0
#define MAX25405_LED_CTRL_ELED_POL_FIELD	MAX25405_FIELD(MAX25405_LED_CTRL_ELED_POL)
#define MAX25405_LED_CTRL_ELED_EN_MASK		0x1
#define MAX25405_LED_CTRL_ELED_EN_OFFSET	1
#define MAX25405_LED_CTRL_ELED_EN_FIELD		MAX25405_FIELD(MAX25405_LED_CTRL_ELED_EN)
#define MAX25405_LED_CTRL_DRV_EN_MASK		0x1
#define MAX25405_LED_CTRL_DRV_EN_OFFSET		2
#define MAX25405_LED_CTRL_DRV_EN_FIELD		MAX25405_FIELD(MAX25405_LED_CTRL_DRV_EN)
#define MAX25405_LED_CTRL_GAINSEL_MASK		0x1
#define MAX25405_LED_CTRL_GAINSEL_OFFSET	3
#define MAX25405_LED_CTRL_GAINSEL_FIELD		MAX25405_FIELD(MAX25405_LED_CTRL_GAINSEL)

typedef enum {
	MAX25405_Status_Ok = 0,
	MAX25405_Status_BusNack = -10,
	MAX25405_Status_BusError = -11,
	MAX25405_Status_BusTimeout = -12,
	MAX25405_Status_NotImplemented = -20,
	MAX25405_Status_BadArg = -21,
	MAX25405_Status_InvalidOperation = -22,
	MAX25405_Status_NotSupported = -24,
} MAX25405_Status;

typedef struct {
	// 1 in case of SPI
	// 0 in case of I2C
	uint8_t useSpi : 1;

	// if useSpi == 1, then address is number of chip select pin
	// if useSpi == 0, then address is 7-bit I2C address
	uint8_t address : 7;

	// following variables contains cached actual configuration informations 
	// which reduces need to read registers when triggering actions.
	int enableShutdownMode;
	int enableOneShotMode;
} MAX25405_Device;

typedef enum {
	MAX25405_I2CAddress_CS_Low_4F = 0x4F,
	MAX25405_I2CAddress_CS_High_50 = 0x50,
} MAX25405_I2CAddress;

typedef enum {
	MAX25405_Interrupt_PowerOnReset = 0x10,
	MAX25405_Interrupt_EndOfConversion = 0x04
} MAX25405_Interrupt;

typedef enum {
	MAX25405_ModeOfOperation_SuperProximityMode = 0,
	MAX25405_ModeOfOperation_GestureMode = 1,
	MAX25405_ModeOfOperation_TrackingMode = 2,
} MAX25405_ModeOfOperation;

typedef enum {
	MAX25405_ExternalSync_NoFunction = 0,
	MAX25405_ExternalSync_Slave = 1,
	MAX25405_ExternalSync_Master = 2
} MAX25405_ExternalSync;

typedef enum {
	MAX25405_EndOfConversionDelay_0ms = 0x00,
	MAX25405_EndOfConversionDelay_1_56ms = 0x01,
	MAX25405_EndOfConversionDelay_3_12ms = 0x02,
	MAX25405_EndOfConversionDelay_6_25ms = 0x03,
	MAX25405_EndOfConversionDelay_12_49ms = 0x04,
	MAX25405_EndOfConversionDelay_24_98ms = 0x05,
	MAX25405_EndOfConversionDelay_49_97ms = 0x06,
	MAX25405_EndOfConversionDelay_99_94ms = 0x07,
	MAX25405_EndOfConversionDelay_199_98ms = 0x08,
	MAX25405_EndOfConversionDelay_399_75ms = 0x09,
	MAX25405_EndOfConversionDelay_799_5ms = 0x0A,
	MAX25405_EndOfConversionDelay_1599ms = 0xB
} MAX25405_EndOfConversionDelay;

typedef enum {
	MAX25405_IntegrationTime_6_25us = 0x00,
	MAX25405_IntegrationTime_12_5us = 0x01,
	MAX25405_IntegrationTime_25us = 0x02,
	MAX25405_IntegrationTime_50us = 0x03,
	MAX25405_IntegrationTime_100us = 0x04,
	MAX25405_IntegrationTime_200us = 0x05,
	MAX25405_IntegrationTime_400us = 0x06,
	MAX25405_IntegrationTime_800us = 0x07
} MAX25405_IntegrationTime;

typedef enum {
	MAX25405_NumberOfRepeats_1 = 0x00,
	MAX25405_NumberOfRepeats_2 = 0x01,
	MAX25405_NumberOfRepeats_4 = 0x02,
	MAX25405_NumberOfRepeats_8 = 0x03,
	MAX25405_NumberOfRepeats_16 = 0x04,
	MAX25405_NumberOfRepeats_32 = 0x05,
	MAX25405_NumberOfRepeats_64 = 0x06,
	MAX25405_NumberOfRepeats_128 = 0x07
} MAX25405_NumberOfRepeats;

typedef enum {
	MAX25405_NumberOfCoherentDoubleSamples_1 = 0x00,
	MAX25405_NumberOfCoherentDoubleSamples_2 = 0x01,
	MAX25405_NumberOfCoherentDoubleSamples_4 = 0x02,
	MAX25405_NumberOfCoherentDoubleSamples_8 = 0x03,
	MAX25405_NumberOfCoherentDoubleSamples_16 = 0x04,
	MAX25405_NumberOfCoherentDoubleSamples_32 = 0x05,
	MAX25405_NumberOfCoherentDoubleSamples_64 = 0x06,
	MAX25405_NumberOfCoherentDoubleSamples_128 = 0x07
} MAX25405_NumberOfCoherentDoubleSamples;

typedef enum {
	MAX25405_CoherentDoubleSamplingMode_SeqA_Add_SeqB_Sub = 0,
	MAX25405_CoherentDoubleSamplingMode_SeqA_Add_SeqB_Nop = 1,
} MAX25405_CoherentDoubleSamplingMode;

typedef enum {
	MAX25405_CapFlipping_Enabled = 0,
	MAX25405_CapFlipping_Disabled = 1,
} MAX25405_CapFlipping;

typedef enum {
	MAX25405_AfePgaGain_1 = 0x00,
	MAX25405_AfePgaGain_1_4 = 0x01,
	MAX25405_AfePgaGain_1_16 = 0x02,
	MAX25405_AfePgaGain_1_32 = 0x03
} MAX25405_AfePgaGain;

typedef enum {
	MAX25405_AfeReturnToZero_Disabled = 0,
	MAX25405_AfeReturnToZero_Enabled = 1
} MAX25405_AfeReturnToZero;

typedef enum {
	MAX25405_AfeAlcIsel_10uA = 0,
	MAX25405_AfeAlcIsel_5uA = 1
} MAX25405_AfeAlcIsel;

typedef enum {
	MAX25405_AfeCapacitance_0_133pF = 0,
	MAX25405_AfeCapacitance_0_26pF = 1,
	MAX25405_AfeCapacitance_1pF = 2,
	MAX25405_AfeCapacitance_2pF = 3
} MAX25405_AfeCapacitance;

typedef enum {
	MAX25405_AfePolarizationMode_Unipolar = 0,
	MAX25405_AfePolarizationMode_Bipolar = 1
} MAX25405_AfePolarizationMode;

typedef enum {
	MAX25405_LedDrive_PWM_1_16 = 0x00,
	MAX25405_LedDrive_PWM_2_16 = 0x01,
	MAX25405_LedDrive_PWM_3_16 = 0x02,
	MAX25405_LedDrive_PWM_4_16 = 0x03,
	MAX25405_LedDrive_PWM_5_16 = 0x04,
	MAX25405_LedDrive_PWM_6_16 = 0x05,
	MAX25405_LedDrive_PWM_7_16 = 0x06,
	MAX25405_LedDrive_PWM_8_16 = 0x07,
	MAX25405_LedDrive_PWM_9_16 = 0x08,
	MAX25405_LedDrive_PWM_10_16 = 0x09,
	MAX25405_LedDrive_PWM_11_16 = 0x0A,
	MAX25405_LedDrive_PWM_12_16 = 0x0B,
	MAX25405_LedDrive_PWM_13_16 = 0x0C,
	MAX25405_LedDrive_PWM_14_16 = 0x0D,
	MAX25405_LedDrive_PWM_15_16 = 0x0E,
	MAX25405_LedDrive_PWM_16_16 = 0x0F,
	
	MAX25405_LedDrive_Current_0mA = 0x00,
	MAX25405_LedDrive_Current_13_3mA = 0x01,
	MAX25405_LedDrive_Current_26_7mA = 0x02,
	MAX25405_LedDrive_Current_40mA = 0x03,
	MAX25405_LedDrive_Current_53_3mA = 0x04,
	MAX25405_LedDrive_Current_66_7mA = 0x05,
	MAX25405_LedDrive_Current_80mA = 0x06,
	MAX25405_LedDrive_Current_93_3mA = 0x07,
	MAX25405_LedDrive_Current_106_7mA = 0x08,
	MAX25405_LedDrive_Current_120mA = 0x09,
	MAX25405_LedDrive_Current_133_3mA = 0x0A,
	MAX25405_LedDrive_Current_146_7mA = 0x0B,
	MAX25405_LedDrive_Current_160mA = 0x0C,
	MAX25405_LedDrive_Current_173_3mA = 0x0D,
	MAX25405_LedDrive_Current_186_7mA = 0x0E,
	MAX25405_LedDrive_Current_200mA = 0x0F
} MAX25405_LedDrive;

typedef enum {
	MAX25405_ColumnGain_0_33 = 0x00,
	MAX25405_ColumnGain_0_37 = 0x01,
	MAX25405_ColumnGain_0_43 = 0x02,
	MAX25405_ColumnGain_0_49 = 0x03,
	MAX25405_ColumnGain_0_56 = 0x04,
	MAX25405_ColumnGain_0_65 = 0x05,
	MAX25405_ColumnGain_0_75 = 0x06,
	MAX25405_ColumnGain_0_86 = 0x07,
	MAX25405_ColumnGain_1_00 = 0x08,
	MAX25405_ColumnGain_1_14 = 0x09,
	MAX25405_ColumnGain_1_33 = 0x0A,
	MAX25405_ColumnGain_1_53 = 0x0B,
	MAX25405_ColumnGain_1_79 = 0x0C,
	MAX25405_ColumnGain_2_04 = 0x0D,
	MAX25405_ColumnGain_2_38 = 0x0E,
	MAX25405_ColumnGain_2_70 = 0x0F
} MAX25405_ColumnGain;

typedef enum {
	MAX25405_ColumnGainModeSelection_Configuration = 0x00,
	MAX25405_ColumnGainModeSelection_Internal = 0x01
} MAX25405_ColumnGainModeSelection;

typedef enum {
	MAX25405_ExternalLedPolarity_DriveNMOS = 0x00,
	MAX25405_ExternalLedPolarity_DrivePMOS = 0x01
} MAX25405_ExternalLedPolarity;

typedef struct {
	MAX25405_ModeOfOperation modeOfOperation;
	MAX25405_ExternalSync externalSyncMode;
	int enableEndOfConversionInterrupt;
	int enableShutdownMode;
	int enableOneShotMode;
	int enableCoarseAmbientLightCompensation;
	int enableDriveCurrentOutput;
	int enableDrivePwmOutput;
	MAX25405_AfePgaGain afePgaGain;
	MAX25405_AfePolarizationMode afePolarization;
	MAX25405_AfeCapacitance afeCapacitance;
	MAX25405_AfeAlcIsel afeAlcIsel;
	MAX25405_AfeReturnToZero afeReturnToZero;
	MAX25405_LedDrive ledDrive;
	MAX25405_ColumnGainModeSelection columnGainMode;
	MAX25405_ColumnGain columnGain[MAX25405_COLUMNS];
	MAX25405_ExternalLedPolarity externalLedPolarity;
} MAX25405_Configuration;

typedef struct {
	MAX25405_EndOfConversionDelay endOfConversionDelay;
	MAX25405_IntegrationTime integrationTime;
	MAX25405_NumberOfRepeats numberOfRepeats;
	MAX25405_NumberOfCoherentDoubleSamples numberOfCoherentDoubleSamples;
	MAX25405_CoherentDoubleSamplingMode coherentDoubleSamplingMode;
	MAX25405_CapFlipping capFlipping;
} MAX25405_SequencingConfiguration;

MAX25405_Status MAX25405_InitI2C(MAX25405_Device* dev, MAX25405_I2CAddress address);
MAX25405_Status MAX25405_InitSPI(MAX25405_Device* dev, uint8_t chipSelectPortNumber);
MAX25405_Status MAX25405_Deinit(MAX25405_Device* dev);

MAX25405_Status MAX25405_Reset(MAX25405_Device* dev);
MAX25405_Status MAX25405_ForceSync(MAX25405_Device* dev);
MAX25405_Status MAX25405_TriggerOneShoot(MAX25405_Device* dev);

MAX25405_Status MAX25405_GetPendingInterrupts(MAX25405_Device* dev, MAX25405_Interrupt* pendingInterrupts);

MAX25405_Status MAX25405_GetDefaultConfiguration(MAX25405_Configuration* config);
MAX25405_Status MAX25405_GetConfiguration(MAX25405_Device* dev, MAX25405_Configuration* config);
MAX25405_Status MAX25405_SetConfiguration(MAX25405_Device* dev, MAX25405_Configuration* config);

MAX25405_Status MAX25405_GetDefaultSequencingConfiguration(MAX25405_SequencingConfiguration* config);
MAX25405_Status MAX25405_GetSequencingConfiguration(MAX25405_Device* dev, MAX25405_SequencingConfiguration* config);
MAX25405_Status MAX25405_SetSequencingConfiguration(MAX25405_Device* dev, MAX25405_SequencingConfiguration* config);

MAX25405_Status MAX25405_GetSinglePixelData(MAX25405_Device* dev, int16_t* value, int column, int row);
MAX25405_Status MAX25405_GetPixelsData(MAX25405_Device* dev, int16_t* values, int column, int row, int pixelsCount);
MAX25405_Status MAX25405_GetAllPixelData(MAX25405_Device* dev, int16_t* values);

#endif
