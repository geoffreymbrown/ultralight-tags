#include "hal.h"
#include "app.h"
//#include "tagdata.pb.h"
//#include "config.h"
//#include "persistent.h"

#define RTC_TIMEOUT 100

static inline uint8_t bcd2bin(uint8_t val)
{
    return ((val >> 4) & 0xf) * 10 + (val & 0xf);
}

static inline uint8_t bin2bcd(uint8_t val)
{
    return ((val / 10) << 4) | (val % 10);
}

static int I2C1_MemWrite(uint8_t device, uint8_t reg, unsigned char *buffer,
                         uint16_t size, uint32_t timeout)
{
    uint8_t txbuf[10];
    txbuf[0] = reg;
    for (int i = 0; i < 8 && i < size; i++)
    {
        txbuf[i + 1] = buffer[i];
    }
    return i2cMasterTransmitTimeout(&I2CD1, device, txbuf, size + 1, 0, 0,
                                    timeout);
}

#ifdef RV3028_RTC
int rv3028_GetReg(enum RV3028Reg reg, uint8_t *val, int num)
{
    return i2cMasterTransmitTimeout(&I2CD1, RV3028_ADR, &reg, 1, val, num,
                                    RTC_TIMEOUT);
}

static int rv3028_SetReg(enum RV3028Reg reg, unsigned char *val, int num)
{
    return I2C1_MemWrite(RV3028_ADR, reg, val, num, RTC_TIMEOUT);
}

int rv3028_EEPROM_Exec(unsigned char addr, unsigned char *val, unsigned char cmd)
{
    int i;
    unsigned char tmp;

    for (i = 0; i < 10; i++)
    { 
        chThdSleepMilliseconds(10);
        rv3028_GetReg(RV3028_STATUS, &tmp, 1);
        if (!(tmp & RV3028_STATUS_EEBUSY))
            break;
        
    }
    if (i == 10)
    {
        return MSG_TIMEOUT;
    }

    if ((cmd == RV3028_EEPROM_CMD_WRITE) ||
        (cmd == RV3028_EEPROM_CMD_READ))
        rv3028_SetReg(RV3028_EEPROM_ADDR, &addr, 1);
    if (cmd == RV3028_EEPROM_CMD_WRITE)
    {
        rv3028_SetReg(RV3028_EEPROM_DATA, val, 1);
    }
    tmp = 0;
    rv3028_SetReg(RV3028_EEPROM_CMD, &tmp, 1);
    rv3028_SetReg(RV3028_EEPROM_CMD, &cmd, 1);

    for (i = 0; i < 10; i++)
    {
        chThdSleepMilliseconds(10);
        rv3028_GetReg(RV3028_STATUS, &tmp, 1);
        if (!(tmp & RV3028_STATUS_EEBUSY))
            break;
    }

    if (i == 10)
    {
        return MSG_TIMEOUT;
    }
    else
    {
        if (cmd == RV3028_EEPROM_CMD_READ)
        {
            rv3028_GetReg(RV3028_EEPROM_DATA, val, 1);
        }
        return MSG_OK;
    }
}
/*
5.WRITE TO ONE EEPROM BYTE (EEDATA (RAM) EEPROM)
Write to one EEPROM byte of the Configuration EEPROM or User EEPROM registers:
Before starting to change data stored in the EEPROM, the auto refresh of the registers from the EEPROM
has to be disabled by writing 1 into the EERD control bit.
In order to write a single byte to the EEPROM, the address to which the data must be written is entered in
the EEADDR register and the data to be written is entered in the EEDATA register, then the command 00h
is written in the EECMD register, then a second command 21h is written in the EECMD register to start the
EEPROM write.
The time to write to one EEPROM byte is tWRITE = ~16 ms.
When the transfer is finished (EEbusy = 0), the user can enable again the auto refresh of the registers by
writing 0 into the EERD bit in the Control 1 register.
*/

bool initRTC(void)
{
    unsigned char tmp;
    unsigned char clkout;
    unsigned char ctrl1;

    bool result = false;
    rtcOn();
    do
    {
        int i;
        clkout = 0;
        if (rv3028_GetReg(RV3028_CLKOUT, &clkout, 1) != MSG_OK)
            break;
        rv3028_EEPROM_Exec(RV3028_CLKOUT, &tmp, RV3028_EEPROM_CMD_READ);
        if (tmp == (0xC0 | (RV3028_CLKOUT_VAL)))
        {
            result = true;
            break;
        }

        // Write to EEPROM mirror

        rv3028_GetReg(RV3028_CTRL1, &ctrl1, 1);

        // disable automatic refresh

        ctrl1 |= RV3028_CTRL1_EERD;
        rv3028_SetReg(RV3028_CTRL1, &ctrl1, 1);
        for (i = 0; i < 10; i++)
        {
            chThdSleepMilliseconds(10);
            rv3028_GetReg(RV3028_STATUS, &tmp, 1);
            if (!(tmp & RV3028_STATUS_EEBUSY))
                break;
        }
        if (i == 10)
            break;

        //clkout = 0xC0 | (RV3028_CLKOUT_VAL&7);
        //rv3028_SetReg(RV3028_CLKOUT, &clkout, 1);

        clkout = 0;
        if (rv3028_EEPROM_Exec(RV3028_CLKOUT, &clkout, RV3028_EEPROM_CMD_WRITE))
            break;

        clkout = 0xC0 | (RV3028_CLKOUT_VAL & 7);
        if (rv3028_EEPROM_Exec(RV3028_CLKOUT, &clkout, RV3028_EEPROM_CMD_WRITE))
            break;

        if (rv3028_EEPROM_Exec(RV3028_CLKOUT, &clkout, RV3028_EEPROM_CMD_REFRESH))
            break;

        // reenable EEPROM->Ram refresh

        ctrl1 &= ~RV3028_CTRL1_EERD;
        rv3028_SetReg(RV3028_CTRL1, &ctrl1, 1);
        rv3028_EEPROM_Exec(RV3028_CLKOUT, &tmp, RV3028_EEPROM_CMD_READ);

        if ((tmp == clkout) && (tmp == (0xC0 & (RV3028_CLKOUT_VAL))))
        {
            result = true;
        }
    } while (0);
    rtcOff();

    // Check RTC dividers !

    uint32_t prer = RTCD1.rtc->PRER;
    if (prer != STM32_RTC_PRER_BITS)
    {
        RTCD1.rtc->WPR = 0xCA;
        RTCD1.rtc->WPR = 0x53;
        RTCD1.rtc->ISR |= RTC_ISR_INIT;
        while ((RTCD1.rtc->ISR & RTC_ISR_INITF) == 0)
            ;
        RTCD1.rtc->PRER = STM32_RTC_PRER_BITS;
        RTCD1.rtc->PRER = STM32_RTC_PRER_BITS;
        RTCD1.rtc->ISR &= ~RTC_ISR_INIT;
    }
    return result;
}
#endif

#ifdef RV3032_RTC
int rv3032_GetReg(enum RV3032Reg reg, uint8_t *val, int num)
{
    return i2cMasterTransmitTimeout(&I2CD1, RV3032_ADR, &reg, 1, val, num,
                                    RTC_TIMEOUT);
}

static int rv3032_SetReg(enum RV3032Reg reg, unsigned char *val, int num)
{
    return I2C1_MemWrite(RV3032_ADR, reg, val, num, RTC_TIMEOUT);
}

int rv3032_EEPROM_Exec(unsigned char addr, unsigned char *val, unsigned char cmd)
{
    int i;
    unsigned char tmp;

    for (i = 0; i < 10; i++)
    { 
        chThdSleepMilliseconds(10);
        rv3032_GetReg(RV3032_TEMP_LSB, &tmp, 1);
        if (!(tmp & RV3032_TEMP_LSB_EEBUSY))
            break;
        
    }
    if (i == 10)
    {
        return MSG_TIMEOUT;
    }

    if ((cmd == RV3032_EEPROM_CMD_WRITE) ||
        (cmd == RV3032_EEPROM_CMD_READ))
        rv3032_SetReg(RV3032_EEPROM_ADDR, &addr, 1);
    if (cmd == RV3032_EEPROM_CMD_WRITE)
    {
        rv3032_SetReg(RV3032_EEPROM_DATA, val, 1);
    }
    tmp = 0;
    rv3032_SetReg(RV3032_EEPROM_CMD, &tmp, 1);
    rv3032_SetReg(RV3032_EEPROM_CMD, &cmd, 1);

    for (i = 0; i < 10; i++)
    {
        chThdSleepMilliseconds(10);
        rv3032_GetReg(RV3032_TEMP_LSB, &tmp, 1);
        if (!(tmp & RV3032_TEMP_LSB_EEBUSY))
            break;
    }

    if (i == 10)
    {
        return MSG_TIMEOUT;
    }
    else
    {
        if (cmd == RV3032_EEPROM_CMD_READ)
        {
            rv3032_GetReg(RV3032_EEPROM_DATA, val, 1);
        }
        return MSG_OK;
    }
}
/*
5.WRITE TO ONE EEPROM BYTE (EEDATA (RAM) EEPROM)
Write to one EEPROM byte of the Configuration EEPROM or User EEPROM registers:
Before starting to change data stored in the EEPROM, the auto refresh of the registers from the EEPROM
has to be disabled by writing 1 into the EERD control bit.
In order to write a single byte to the EEPROM, the address to which the data must be written is entered in
the EEADDR register and the data to be written is entered in the EEDATA register, then the command 00h
is written in the EECMD register, then a second command 21h is written in the EECMD register to start the
EEPROM write.
The time to write to one EEPROM byte is tWRITE = ~16 ms.
When the transfer is finished (EEbusy = 0), the user can enable again the auto refresh of the registers by
writing 0 into the EERD bit in the Control 1 register.
*/

bool initRTC(void)
{
    unsigned char tmp;
    unsigned char clkout;
    unsigned char ctrl1;

    bool result = false;
    rtcOn();
    do
    {
        int i;
        clkout = 0;
        if (rv3032_GetReg(RV3032_CLKOUT2, &clkout, 1) != MSG_OK)
            break;
        rv3032_EEPROM_Exec(RV3032_CLKOUT2, &tmp, RV3032_EEPROM_CMD_READ);
        if (tmp == (RV3032_CLKOUT_VAL))
        {
            result = true;
            break;
        }

        // Write to EEPROM mirror

        rv3032_GetReg(RV3032_CTRL1, &ctrl1, 1);

        // disable automatic refresh

        ctrl1 |= RV3032_CTRL1_EERD;
        rv3032_SetReg(RV3032_CTRL1, &ctrl1, 1);
        for (i = 0; i < 10; i++)
        {
            chThdSleepMilliseconds(10);
            rv3032_GetReg(RV3032_TEMP_LSB, &tmp, 1);
            if (!(tmp & RV3032_TEMP_LSB_EEBUSY))
                break;
        }
        if (i == 10)
            break;

        //clkout = 0xC0 | (RV3032_CLKOUT_VAL&7);
        //rv3032_SetReg(RV3032_CLKOUT, &clkout, 1);

        clkout = 0;
        // if (rv3032_EEPROM_Exec(RV3032_CLKOUT, &clkout, RV3032_EEPROM_CMD_WRITE))
        //     break;

        clkout = RV3032_CLKOUT_VAL;
        if (rv3032_EEPROM_Exec(RV3032_CLKOUT2, &clkout, RV3032_EEPROM_CMD_WRITE))
            break;

        if (rv3032_EEPROM_Exec(RV3032_CLKOUT2, &clkout, RV3032_EEPROM_CMD_REFRESH))
            break;

        // reenable EEPROM->Ram refresh

        ctrl1 &= ~RV3032_CTRL1_EERD;
        rv3032_SetReg(RV3032_CTRL1, &ctrl1, 1);
        rv3032_EEPROM_Exec(RV3032_CLKOUT2, &tmp, RV3032_EEPROM_CMD_READ);

        if (tmp == RV3032_CLKOUT_VAL)
        {
            result = true;
        }
    } while (0);
    rtcOff();

    // Check RTC dividers !

    uint32_t prer = RTCD1.rtc->PRER;
    if (prer != STM32_RTC_PRER_BITS)
    {
        RTCD1.rtc->WPR = 0xCA;
        RTCD1.rtc->WPR = 0x53;
        RTCD1.rtc->ISR |= RTC_ISR_INIT;
        while ((RTCD1.rtc->ISR & RTC_ISR_INITF) == 0)
            ;
        RTCD1.rtc->PRER = STM32_RTC_PRER_BITS;
        RTCD1.rtc->PRER = STM32_RTC_PRER_BITS;
        RTCD1.rtc->ISR &= ~RTC_ISR_INIT;
    }
    return result;
}
#endif

#ifdef RV8803_RTC
int rv8803_GetReg(enum RV8803Reg reg, uint8_t *val, int num)
{
    return i2cMasterTransmitTimeout(&I2CD1, RV8803_ADR, &reg, 1, val, num,
                                    RTC_TIMEOUT);
}

static int rv8803_SetReg(enum RV8803Reg reg, unsigned char *val, int num)
{
    return I2C1_MemWrite(RV8803_ADR, reg, val, num, RTC_TIMEOUT);
}

bool initRTC(void)
{
    bool result = true;
    const unsigned char bufval = 4;
    unsigned char buf = bufval; // 2<<2; // 1024hz clock input
    rtcOn();
    rv8803_SetReg(RV8803_EXT, &buf, 1);
    rv8803_GetReg(RV8803_EXT, &buf, 1);
    rtcOff();
    result = (buf == bufval); 
    if (RTCD1.rtc->PRER != STM32_RTC_PRER_BITS)
    {
        RTCD1.rtc->WPR = 0xCA;
        RTCD1.rtc->WPR = 0x53;
        RTCD1.rtc->ISR |= RTC_ISR_INIT;
        while ((RTCD1.rtc->ISR & RTC_ISR_INITF) == 0)
            ;
        RTCD1.rtc->PRER = STM32_RTC_PRER_BITS;
        RTCD1.rtc->PRER = STM32_RTC_PRER_BITS;
        RTCD1.rtc->ISR &= ~RTC_ISR_INIT;
    }
    return result && (RTCD1.rtc->PRER == STM32_RTC_PRER_BITS);
}
#endif
msg_t setRTCDateTime(RTCDateTime *tm)
{
    int ret = MSG_OK;

#if defined(RV3028_RTC) || defined(RV8803_RTC) || defined(RV3032_RTC)

    uint8_t date[7];

    rtcOn();
    do
    {
        uint32_t seconds = tm->millisecond / 1000;
        date[RV3028_SEC] = bin2bcd(seconds % 60);
        seconds = seconds / 60;
        date[RV3028_MIN] = bin2bcd(seconds % 60);
        seconds = seconds / 60;
        date[RV3028_HOUR] = bin2bcd(seconds % 24);
        date[RV3028_WDAY] = 1 << (tm->dayofweek - 1);
        date[RV3028_DAY] = bin2bcd(tm->day);
        date[RV3028_MONTH] = bin2bcd(tm->month);
        date[RV3028_YEAR] = bin2bcd(tm->year);
#ifdef RV3028_RTC
        ret = rv3028_SetReg(RV3028_SEC, date, sizeof(date));
#elif defined(RV8803_RTC)
        ret = rv8803_SetReg(RV8803_SEC, date, sizeof(date));
#elif defined(RV3032_RTC)
        ret = rv3032_SetReg(RV3032_SEC, date, sizeof(date));
#endif
        if (ret != MSG_OK)
            break;

        uint8_t status = 0;
#ifdef RV3028_RTC
        ret = rv3028_SetReg(RV3028_STATUS, &status, 1);
#elif defined(RV8803_RTC)
        ret = rv8803_SetReg(RV8803_FLAG, &status, 1);
#elif defined(RV3032_RTC)
        ret = rv3032_SetReg(RV3032_STATUS, &status, 1);
#endif

    } while (0);
    rtcOff();
#endif
    return ret;
}

msg_t getRTCDateTime(RTCDateTime *tm)
{
    int ret = MSG_OK;

#if defined(RV3028_RTC) || defined(RV8803_RTC)  || defined(RV3032_RTC)
    uint8_t status = 0;
    uint8_t date[7];
    rtcOn();
    do
    {
#ifdef RV3028_RTC
        ret = rv3028_GetReg(RV3028_STATUS, &status, 1);
        if ((ret != MSG_OK) || (status & RV3028_STATUS_PORF))
        {
            ret = (ret == MSG_OK) ? -1 : ret;
            break;
        }
        ret = rv3028_GetReg(RV3028_SEC, date, sizeof(date));
#elif RV3032_RTC
        ret = rv3032_GetReg(RV3032_STATUS, &status, 1);
        if ((ret != MSG_OK) || (status & RV3032_STATUS_PORF))
        {
            ret = (ret == MSG_OK) ? -1 : ret;
            break;
        }
        ret = rv3032_GetReg(RV3032_SEC, date, sizeof(date));
#elif defined(RV8803_RTC)
        ret = rv8803_GetReg(RV8803_FLAG, &status, 1);
        if ((ret != MSG_OK) || (status & RV8803_FLAG_V2F))
        {
            ret = (ret == MSG_OK) ? -1 : ret;
            break;
        }
        ret = rv8803_GetReg(RV8803_SEC, date, sizeof(date));
#endif
        if (ret != MSG_OK)
        {
            break;
        }
        tm->millisecond = bcd2bin(date[RV3028_SEC] & 0x7f);
        tm->millisecond += 60 * bcd2bin(date[RV3028_MIN] & 0x7f);
        tm->millisecond += 3600 * bcd2bin(date[RV3028_HOUR] & 0x3f);
        tm->millisecond *= 1000;
        tm->dayofweek = bcd2bin(date[RV3028_WDAY] & 0x7) + 1;
        tm->day = bcd2bin(date[RV3028_DAY] & 0x3f);
        tm->month = bcd2bin(date[RV3028_MONTH] & 0x3f);
        tm->year = bcd2bin(date[RV3028_YEAR]);
    } while (0);
    rtcOff();
#endif
    return ret;
}
