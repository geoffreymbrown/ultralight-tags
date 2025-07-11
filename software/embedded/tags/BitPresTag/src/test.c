#include <stdint.h>
#include "hal.h"
#include "custom.h"
#include "app.h"
#include "ADXL362.h"
#include "tagdata.pb.h"
#include "config.h"
#include "persistent.h"
#include "external_flash.h"
#include "lps.h"
#include  "opt3002.h"
#include "ais2dw12.h"
#include "lis2dtw12.h"
#include "mmc5633.h"

#define DEBUGTEST 0

// no init


static bool failed NOINIT;

static bool test_rtc(void)
{

  if (!initRTC())
  {
    return false;
  }

  RTCDateTime tim;
  if (MSG_OK != getRTCDateTime(&tim))
  {
    return false;
  }
  rtcSetTime(&RTCD1, &tim); 
  uint32_t prer = RTCD1.rtc->PRER;
  if (prer != STM32_RTC_PRER_BITS)
  {
    return false;
  }
  return true;
}

#ifdef USE_ADXL362
static uint8_t id[3] NOINIT;
static int xyz[3] NOINIT;
static int testxyz[3] NOINIT;

// should collect error bits for individual tests

static void adxlavg(int val[3])
{
  int i;
  short x, y, z;
  val[0] = 0;
  val[1] = 0;
  val[2] = 0;
  for (i = 0; i < 16; i++)
  {
    ADXL362_GetXyz(&x, &y, &z);
    val[0] += x;
    val[1] += y;
    val[2] += z;
    chThdSleepMilliseconds(20);
  }
  val[0] /= 16;
  val[1] /= 16;
  val[2] /= 16;
}

static void test_adxl362(void)
{
  // Test adxl
  accelSpiOn();
  failed = true;

  ADXL362_SoftwareReset();
  do
  {
    chThdSleepMilliseconds(100);
    // Read DEVID
    ADXL362_GetRegisterValue(id, ADXL362_REG_DEVID_AD, 3);
    if ((id[0] != 0xAD) || (id[1] != 0x1D) || (id[2] != 0xF2))
      break;
    // set 8g range,100hz
    ADXL362_SetRegisterValue(0x83, 0x2C, 1);
    // turn on power
    ADXL362_SetRegisterValue(0x02, 0x2D, 1);
    chThdSleepMilliseconds(200);
    adxlavg(xyz);
    // turn off power
    ADXL362_SetRegisterValue(0x00, 0x2D, 1);
    // turn on self test
    ADXL362_SetRegisterValue(0x01, 0x2E, 1);
    ADXL362_SetRegisterValue(0x02, 0x2D, 1);
    chThdSleepMilliseconds(200);
    adxlavg(testxyz);
    // turn off power
    ADXL362_SetRegisterValue(0x00, 0x2D, 1);
    int tmp = testxyz[0] - xyz[0];
    if ((tmp < 50) || (tmp > 700))
      break;
    tmp = testxyz[1] - xyz[1];
    if ((tmp < -700) || (tmp > -50))
      break;
    tmp = testxyz[2] - xyz[2];
    if ((tmp < 50) || (tmp > 700))
      break;
    failed = false;
  } while (0);
  ADXL362_SoftwareReset();
  accelSpiOff();
}
#endif

#if defined(USE_AIS2)
extern bool ais2_test(void);
#endif

#ifdef EXTERNAL_FLASH
static bool test_flash(void){
  bool result;
  ExFlashPwrUp();
  result = (ExCheckID() > -1);
  ExFlashPwrDown();
  return result;
}
#endif

void test(void)
{
  pState->test_result = TEST_RUNNING;
  failed = false;
#if defined(USE_ADXL362)
  if ((test_to_run == RUN_ALL) ||
      (test_to_run == RUN_ADXL362))
  {
    test_adxl362();
    if (failed)
    {
      pState->test_result = ADXL362_FAILED;
      return;
    }
  }
#endif
#if defined(USE_AIS2) 
  if ((test_to_run == RUN_ALL) ||
      (test_to_run == RUN_AIS2))
  {
    failed = !ais2_test();
    if (failed)
    {
      pState->test_result = AIS2_FAILED;
      return;
    }
  }
#endif
#if defined(USE_LIS2) 
  if ((test_to_run == RUN_ALL) ||
      (test_to_run == RUN_AIS2))
  {
    failed = !lis2_test();
    if (failed)
    {
      pState->test_result = AIS2_FAILED;
      return;
    }
  }
#endif
#if defined(USE_OPT3002)
  if ((test_to_run == RUN_ALL)||(test_to_run == RUN_OPT))
  {
    if (!opt3002_test())
    {
      pState->test_result = OPT_FAILED;
      return;
    }
  }
#endif
#if defined(USE_MAG)
  if ((test_to_run == RUN_ALL) ||
      (test_to_run == RUN_MMC5633))
  {
    failed = !mmc5633_test();
    if (failed)
    {
      pState->test_result = MMC5633_FAILED;
      return;
    }
  }
#endif
  if ((test_to_run == RUN_ALL) ||
      (test_to_run == RUN_RTC))
  {
     if (0 && !test_rtc())
    {
      pState->test_result = RTC_FAILED;
      return;
    } 
  }
#if defined(EXTERNAL_FLASH)
  if ((test_to_run == RUN_ALL)||(test_to_run == RUN_EXT_FLASH))
  {
    if (!test_flash())
    {
      pState->test_result = EXT_FLASH_FAILED;
      return;
    }
  }
#endif

#if defined(USE_LPS33) || defined(USE_LPS27) || defined(USE_LPS22)
  if ((test_to_run == RUN_ALL)||(test_to_run == RUN_LPS))
  {
    if (!lpsTest())
    {
      pState->test_result = LPS_FAILED;
      return;
    }
  }
#endif


  pState->test_result = ALL_PASSED;
}

TestResult testreport()
{
  return (pState->test_result);
}
