# STM32 Peripheral Drivers

A C project focused on building reusable drivers for common STM32 peripherals, with examples showing how to work with GPIO, SPI, I²Cm UART.

## Project goal

The goal was to organize peripheral access into driver modules and demonstrate their use through small application examples.

## Tools and technologies

- C
- STM32L058 microcontroller
- Logic analyzer
- Register-level peripheral access
- GPIO, SPI, and I²C
- Polling and interrupt-oriented examples

## What I implemented

- GPIO, SPI, and I²C driver code.
- Separate driver headers and source files under `Drivers/Inc` and `Drivers/Src`.
- Application examples under `Src` that demonstrate peripheral use.
- Examples covering both polling and interrupt-oriented approaches.

## How the project works

1. An application example configures and uses a peripheral through its driver.
2. The driver code groups the peripheral-specific operations behind reusable functions.
3. Examples demonstrate how application code can interact with GPIO and serial communication peripherals using polling or interrupts.

