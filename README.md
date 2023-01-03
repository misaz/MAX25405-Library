# MAX25405 Gesture Sensor Library

This project contains library for controlling MAX25405 Gesture Sensor from Maxim Integrated (now Analog Devices). Library is written in C and is designed as multiplatform. Library contains low-level driver for the sensor but do not contain any gesture detection algorithm. It provide you RAW data from the sensor and you can process them in any way you want.

Originally I created this Library as part of [Element14 Experimenting with Gesture Sensors Design Challenge](https://community.element14.com/challenges-projects/design-challenges/experimenting-with-gesture-sensors/w/documents/27726/experimenting-with-gesture-sensors).

## Features
- Written in C. Can be used in both C and C++ projects.
- Minimal RAM usage. No dynamic memory allocations.
- Library core depends on `stdint.h` (for `uint8_t`, `uint16_t`, `int16_t` and `uint32_t` types) and `stddef.h` (for `size_t` type).
- Porting requires implementing only 4 or 8 functions (init, deinit, read and write) depending if I2C, SPI or both communication interfaces are used.
- Support for configuring all parameters including parameters missing in datasheet.
- Support for communicating using both I2C and SPI bus including support for running on different buses at the same time.
- Support for controlling multiple devices at the same time.
- Support for soft-reset.
- Support for both automatic and one-shot modes.
- Support for configuring and reading interrupts (requires platfrom specific implementation and external interrupt handler definition).
- Support for reading all (60) pixel values or only selection of pixels.
- Tested with MAX25405EVKIT and MAX78000FTHR microcontroller board.

## Getting started
1. Go to [Release page](https://github.com/misaz/MAX25405-Library/releases) and download ZIP file targetting your Platform or ZIP targeting Generic platform if your platform is not supported.
2. Unzip downloaded file and copy all files to folder of your project
3. Add `#include "MAX25405.h"` at the begining of your source code file.
4. Allocate `MAX25405_Device dev` variable.
5. Call `MAX25405_InitSPI(&dev, 0)` or `MAX25405_InitI2C(&dev, MAX25405_I2CAddress_CS_High_50)` function at the begining of your program.
6. Use functions starting with `MAX25405_` as needed.

## Functions
Library contains following functions (you can find this listing in MAX25405.h file):

```c
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

MAX25405_Status MAX25405_GetSinglePixelData(MAX25405_Device* dev, int16_t* value, int column, int row);
MAX25405_Status MAX25405_GetPixelsData(MAX25405_Device* dev, int16_t* values, int column, int row, int pixelsCount);
MAX25405_Status MAX25405_GetAllPixelData(MAX25405_Device* dev, int16_t* values);
```

## Supported platfroms and porting
Library is currently supported on following platforms:

- **Maxim Integrated MAX78000**: Implementation use MAX78000 StdPeriph driver from Maxim SDK to control SPI bus. Port assumes using MAX78000FTHR with connected MAX25405EVKIT Shield (and sensor).

Library is designed as platform independent and you can port it to whatever platform you want. If you want to port it, just implement `src/MAX25405_PlatformSpecific.c` files and remove files with suffixed names related to other platforms. In `src/MAX25405_PlatformSpecific.c` you need to implement 4 or 8 functions responsible for initialization, deinitialization, register read and register write transactions. If you do not need both buses (I2C and SPI), then you do not need to implement unused functions. Let them return `MAX25405_Status_NotImplemented` status code in this case. You can implement both communications and use them to communicate with two sensors at the same time. Implement required functions using I2C or SPI driver available on your platform. `src/MAX25405_PlatformSpecific.c` contains comments describing what you should do in each function.

## Example

```c
#include "MAX25405.h"

int main() {
	MAX25405_Status mStatus;
	MAX25405_Device max25405dev;

	// select MAX25405 SPI bus: set SEL pin to LOW
	// Place GPIO configuration routeine and output low setting here

	MAX25405_InitSPI(&max25405dev, 0);
	MAX25405_Reset(&max25405dev);

	// deley 10ms here

	MAX25405_Configuration config;
	MAX25405_GetDefaultConfiguration(&config);
	config.mainConfig.modeOfOperation = MAX25405_ModeOfOperation_TrackingMode;
	config.ledConfig.ledDrive = MAX25405_LedDrive_PWM_16_16;
	config.ledConfig.enableDrivePwmOutput = 1;
	config.ledConfig.columnGainMode = MAX25405_ColumnGainModeSelection_Internal;
	config.mainConfig.enableEndOfConversionInterrupt = 1;
	config.sequencingConfig.endOfConversionDelay = MAX25405_EndOfConversionDelay_3_12ms;
	config.sequencingConfig.integrationTime = MAX25405_IntegrationTime_25us;
	config.sequencingConfig.numberOfCoherentDoubleSamples = MAX25405_NumberOfCoherentDoubleSamples_8;

	MAX25405_SetConfiguration(&max25405dev, &config);

	while (1) {
		MAX25405_Interrupt interrupt = 0;
		MAX25405_GetPendingInterrupts(&max25405dev, &interrupt);

		if (interrupt & MAX25405_Interrupt_EndOfConversion) {
			int16_t data[60];

			MAX25405_GetAllPixelData(&max25405dev, data);

			// process pixel data here
		}

	}
}
```

## Feedback
Any feedback (including negative) is welcome. If you found any bug kindly create issue. You can also create issue for asking questions about library or just providing feedback.