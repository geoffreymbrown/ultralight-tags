#ifndef LPS27HW_H
#define LPS27HW_H

enum LPS27_OutputRate
{
    LPS27_OutputRate_OneShot = 0b000,
    LPS27_OutputRate_1Hz = 0b001,
    LPS27_OutputRate_10Hz = 0b010,
    LPS27_OutputRate_25Hz = 0b011,
    LPS27_OutputRate_50Hz = 0b100,
    LPS27_OutputRate_75Hz = 0b101,
    LPS27_OutputRate_100Hz = 0b110,
    LPS27_OutputRate_200Hz = 0b111
};

enum LPS27_LowPassFilter
{
    LPS27_LowPassFilter_Off = 0b00,
    LPS27_LowPassFilter_ODR9 = 0b10,
    LPS27_LowPassFilter_ODR20 = 0b011,
};

enum LPS27_Reg
{
    LPS27_INTERRUPT_CFG = 0x0B,
    LPS27_THS_P_L = 0x0C,
    LPS27_THS_P_H = 0x0D,
    LPS27_IF_CTRL = 0x0E,
    LPS27_WHO_AM_I = 0x0F,
    LPS27_CTRL_REG1 = 0x10,
    LPS27_CTRL_REG2 = 0x11,
    LPS27_CTRL_REG3 = 0x12,
    LPS27_FIFO_CTRL = 0x13,
    LPS27_FIFO_WTM  = 0x14,
    LPS27_REF_P_L = 0x15,
    LPS27_REF_P_H = 0x16,
    LPS27_RPDS_L = 0x18,
    LPS27_RPDS_H = 0x19,
    LPS27_INT_SOURCE = 0x24,
    LPS27_FIFO_STATUS1 = 0x25,
    LPS27_FIFO_STATUS2 = 0x26,
    LPS27_STATUS = 0x27,
    LPS27_PRESS_OUT_XL = 0x28,
    LPS27_PRESS_OUT_L = 0x29,
    LPS27_PRESS_OUT_H = 0x2A,
    LPS27_TEMP_OUT_L = 0x2B,
    LPS27_TEMP_OUT_H = 0x2C,
    LPS27_LPFP_RES = 0x33,
    LPS27_FIFO_DATA_OUT_PRES_XL = 0x78,
    LPS27_FIFO_DATA_OUT_PRES_L = 0x79,
    LPS27_FIFO_DATA_OUT_PRES_H = 0x7A,
    LPS27_FIFO_DATA_OUT_TEMP_L = 0x7B,
    LPS27_FIFO_DATA_OUT_TEMP_H = 0x7C,
};

#define LPS27_WHO_AM_I_VALUE (0xB3)

#define LPS27_CTRL_REG1_BDU (1<<1)

#define LPS27_CTRL_REG2_ONE_SHOT   (1<<0)
#define LPS27_CTRL_REG2_LOW_NOISE_EN (1<<1)
#define LPS27_CTRL_REG2_SWRESET (1<<2)
#define LPS27_CTRL_REG2_IF_ADD_INC (1<<4)
#define LPS27_CTRL_REG2__BOOT (1<<7)

#define LPS27_CTRL_REG3_DRDY       (1<<2)
#define LPS27_RES_CONF_LC_EN       (1<<0)
#define LPS27_IF_CTRL_I2C_DISABLE (1<<0)

#endif