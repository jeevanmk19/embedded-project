# STM32 ADC DMA UART Project

This project demonstrates a bare-metal STM32F401 implementation where
a 100Hz timer triggers ADC conversion. The ADC result is transferred via DMA,
and upon DMA completion the data is transmitted using UART and displayed on PuTTY.

## Hardware
- STM32 Blackpill F401
- ADC Channel: PA0
- UART: USART2 (PA2 TX, PA3 RX)
- Baud Rate: 115200

## Flow
Timer → ADC → DMA → Interrupt → UART → PuTTY

## Tools
- STM32CubeIDE
- PuTTY

## Result
ADC values are printed at 100Hz on serial terminal.
