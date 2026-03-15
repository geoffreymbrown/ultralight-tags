# Important note 

PB7 is shared with boot0 on stm32u375 -- to use as an I/O requires a change to the option bytes.

BOOT0 becomes VCAP and requires a 4.7uF capacitor


# Pin Assignments

These are for both the STM32l432 and the STM32U375

FLash

- Spi 3
  - sck:
    * PB3
  - miso:
    * PB4
  - mosi:
    * PB5
  - CS:
    * PA15

ISM6DSV IMU

- SPI1
  -  sck:  
      * PA5
  -  miso: 
      * PA6
  -  mosi: 
      * PA7
  -  ODR Trigger: 
      * PA8 (stm32l432, LPIM2_OUT), (stm32u375 -- LPTIM2_CH1)
  -  CS:

AK09940A Magnetometer

- SPI1
  -  sck:
     * PA5 (shared with IMU)
  -  miso:
     * PA11 (shared with LPS22)
  -  mosi:
     * PA7 (shared with IMU)
  -  cs:
  -  drdy/trgr:



LPS22DV

- SPI1
  - sck:
    - PA5 (Shared with LSM6DSV)
  - miso:
    - PA11
  - mosi:
    - PA12
  - cs:
  - drdy:
    - PA9