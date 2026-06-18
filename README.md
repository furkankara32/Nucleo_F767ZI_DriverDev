# \# Nucleo F767ZI Bare-Metal Driver Development

# 

# This repository contains a bare-metal peripheral driver development project for the \*\*STM32F767ZI\*\* microcontroller on the \*\*NUCLEO-F767ZI\*\* development board.

# 

# The main purpose of this project is to develop low-level peripheral drivers without relying on high-level HAL driver APIs, in order to better understand register-level embedded software development and build a reusable driver layer for future embedded projects.

# 

# \## Target Board

# 

# \* Board: ST NUCLEO-F767ZI

# \* MCU: STM32F767ZITx

# \* Core: ARM Cortex-M7

# \* IDE: STM32CubeIDE

# \* Language: C

# \* Development approach: Bare-metal / register-level driver development

# 

# \## Implemented Drivers

# 

# The following peripheral drivers are currently implemented and tested:

# 

# \* GPIO driver

# \* EXTI / interrupt handling

# \* SysTick driver

# \* NVIC driver

# \* USART driver

# \* SPI driver

# \* I2C driver

# 

# \## USART Features

# 

# The USART driver supports:

# 

# \* Blocking transmit and receive

# \* Timeout-based transmit and receive

# \* Interrupt-based non-blocking transmit

# \* RX ring buffer

# \* IDLE line detection

# \* Error event handling

# \* Application callback mechanism

# 

# Tested USART features:

# 

# \* USART3 TX

# \* USART3 RX echo

# \* USART3 interrupt RX echo

# \* USART3 interrupt TX

# \* USART3 IDLE-based RX packet detection

# 

# \## SPI Features

# 

# The SPI driver supports:

# 

# \* Blocking transmit

# \* Blocking receive

# \* Blocking transmit-receive

# \* Timeout-based transfers

# \* Interrupt-based transmit

# \* Interrupt-based receive

# \* Interrupt-based transmit-receive

# \* Error handling for OVR, MODF, CRCERR and FRE

# \* Application callback mechanism

# 

# Tested SPI features:

# 

# \* SPI1 loopback blocking test

# \* SPI1 loopback with chip-select test

# \* SPI1 interrupt transmit-receive test

# \* SPI1 interrupt transmit-only test

# \* SPI1 interrupt receive-only test

# 

# \## I2C Features

# 

# The I2C driver supports:

# 

# \* I2C initialization with configurable timing

# \* Analog filter and digital filter configuration

# \* Blocking master transmit

# \* Blocking master receive

# \* Blocking write-read transaction

# \* Device-ready check

# \* Interrupt-based master transmit

# \* Interrupt-based master receive

# \* Interrupt-based master write-read transaction

# \* NACK, STOP, transfer complete and error event handling

# \* Application callback mechanism

# 

# Tested I2C features:

# 

# | Example | Test Description                                   | Result |

# | ------- | -------------------------------------------------- | ------ |

# | EX024   | I2C MasterTransmitIT NACK test                     | Passed |

# | EX025   | I2C MasterReceiveIT NACK test                      | Passed |

# | EX026   | I2C1 MasterTransmitIT to I2C2 slave success test   | Passed |

# | EX027   | I2C1 MasterReceiveIT from I2C2 slave success test  | Passed |

# | EX028   | I2C1 MasterWriteReadIT repeated-start success test | Passed |

# | EX029   | I2C MasterWriteReadIT NACK test                    | Passed |

# 

# \## Example List

# 

# | Example | Description                                   |

# | ------- | --------------------------------------------- |

# | EX001   | GPIO LED blink                                |

# | EX002   | GPIO set/reset test                           |

# | EX003   | GPIO button input                             |

# | EX004   | GPIO deinit test                              |

# | EX005   | GPIO external interrupt test                  |

# | EX006   | SysTick LED blink                             |

# | EX007   | USART3 transmit                               |

# | EX008   | USART3 receive echo                           |

# | EX009   | USART3 interrupt RX echo                      |

# | EX010   | USART3 10-byte packet test                    |

# | EX011   | USART3 packet parser                          |

# | EX012   | SPI1 loopback test                            |

# | EX013   | SPI1 loopback with chip select                |

# | EX014   | SPI1 send-receive test                        |

# | EX015   | I2C1 init test                                |

# | EX016   | I2C1 master send NACK test                    |

# | EX017   | I2C1 scanner test                             |

# | EX018   | I2C1 SSD1306 command test                     |

# | EX019   | USART3 interrupt TX test                      |

# | EX020   | USART3 IDLE RX test                           |

# | EX021   | SPI1 interrupt loopback TX/RX test            |

# | EX022   | SPI1 interrupt TX-only test                   |

# | EX023   | SPI1 interrupt RX-only test                   |

# | EX024   | I2C1 interrupt TX NACK test                   |

# | EX025   | I2C1 interrupt RX NACK test                   |

# | EX026   | I2C1 interrupt TX to I2C2 slave test          |

# | EX027   | I2C1 interrupt RX from I2C2 slave test        |

# | EX028   | I2C1 interrupt write-read repeated-start test |

# | EX029   | I2C1 interrupt write-read NACK test           |

# 

# \## Current Limitations

# 

# \* I2C interrupt APIs currently support 7-bit addressing only.

# \* I2C interrupt APIs currently support transfer lengths up to 255 bytes.

# \* SPI interrupt APIs are currently tested with 8-bit data size.

# \* I2C slave mode is not implemented as a complete driver yet. I2C2 slave behavior was used only as a polling-based test helper.

# \* DMA support is not implemented yet.

# \* RTOS integration is not included yet.

# 

# \## Project Goals

# 

# The goal of this project is not only to make the peripherals work, but also to build a clean and reusable embedded driver architecture.

# 

# Main learning and development goals:

# 

# \* Understanding STM32 peripheral registers

# \* Writing reusable driver APIs

# \* Implementing interrupt-based non-blocking communication

# \* Handling peripheral errors safely

# \* Building a testable driver structure

# \* Preparing a stronger foundation for safety-critical embedded projects

# 

# \## Future Work

# 

# Planned improvements:

# 

# \* Add DMA support for USART, SPI and I2C

# \* Add full I2C slave driver support

# \* Add more structured error reporting

# \* Add regression test documentation

# \* Add FreeRTOS-based examples

# \* Integrate the drivers into a larger embedded application project

# 

