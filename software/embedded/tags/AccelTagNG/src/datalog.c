#include "app.h"
#include "lps.h"
#include "datalog.h"
#include <tag.pb.h>
#include "persistent.h"
#include "external_flash.h"

const int databuf_size = sizeof(t_DataLog);
static t_DataLog databuf NOINIT;

extern int encode_ack(void);

// Recover pState from log

int restoreLog()
{
  uint32_t addr = 0;
  uint32_t next_addr = 0;
  int32_t buf;
  const uint32_t blocksize = sizeof(t_DataLog)*1024;

  ExFlashPwrUp();
  uint32_t size = ExCheckID() * 1024;

  // read epoch of block 0


  ExFlashRead(0,(uint8_t *) &buf, 4);

  if (buf == -1) // erased ?
  {
    pState->pages = 0;
    pState->external_blocks = 0;
    ExFlashPwrDown();
    return 0;
  }

  // search for end of written blocks
  // step size  1024*blocksize... 1*blocksize

  for (int i = 10; i > 0; i--) {
    uint32_t step = (1<<i)*blocksize;
    for ( next_addr = addr + step; 
          next_addr + blocksize <= size; 
          next_addr = addr + step) 
    {
      ExFlashRead(next_addr,(uint8_t *) &buf,4);
      if (buf == -1) {
        break;
      }
      addr = next_addr;
    }
  }
  ExFlashPwrDown();

  // treat last block found as full -- This needs further work
  pState->pages = addr/blocksize;
  pState->external_blocks = pState->pages*DATALOG_SAMPLES;
  return 0;
}

// 
// Write data to external log
//

enum LOGERR writeDataLog(uint16_t *data, int num)
{
  if (pState->external_blocks*2 + num > EXT_FLASH_SIZE/2)
  {
    return LOGWRITE_FULL;
  }

  int cnt;
  int addr = pState->external_blocks * 2;

  ExFlashPwrUp();
  for (int i = 0; i < num; i++)
  {
    cnt = 2;
    if (!ExFlashWrite(addr, (uint8_t *) &data[i], &cnt)) {
       /* ignore error */
       /* what is right thing to do ? */
    }
    stopMilliseconds(true,2);
    addr += 2;
  }
  ExFlashPwrDown();
  pState->external_blocks += 2;
  return LOGWRITE_OK;
}

extern enum LOGERR writeDataHeader(t_DataHeader *head)
{
  uint32_t flashend = (uint32_t)(0x8000000 +
                                (*((uint16_t *)FLASHSIZE_BASE) * 1024));

  uint32_t *writeptr = (uint32_t *)&vddHeader[pState->pages++];

  chSysLock();
  FLASH_Unlock();
  uint32_t flasherr = FLASH_Program_Array(writeptr, (uint32_t *) head, sizeof(t_DataHeader)/4);
  FLASH_Lock();
  FLASH_Flush_Data_Cache();
  chSysUnlock();

 // See if the log file is full

  if ((((uint32_t)writeptr) + 8) >= flashend)
    return LOGWRITE_FULL;
  // See if there is still energy to continue

  if (flasherr) 
    return LOGWRITE_ERROR;
  if (head->vdd100 < 200)
    return LOGWRITE_BAT;
  else
    return LOGWRITE_OK;
}

//
// Generate monitor ack for data log request
//   Executed by the monitor thread
//

int data_logAck(int index, Ack *ack)
{

  AccelTagNgLog *data = &ack->payload.acceltag_ng_data_log;
  ack->err = Ack_Err_OK;

  // read data
  ExFlashPwrUp();
  ExFlashRead(databuf_size*index, (uint8_t *) &databuf, databuf_size);
  ExFlashPwrDown();

  if (vddHeader[index].epoch != -1)
  {
    ack->which_payload = Ack_acceltag_ng_data_log_tag;
    data->epoch = vddHeader[index].epoch;
    //data->voltage = vddHeader[index].vdd100 * 0.01f;
    //data->temperature = vddHeader[index].temp10 * 0.1f;
    //data->activity_count = 0;

    for (int j = 0; j < DATALOG_SAMPLES; j++) // loop over samples
    {
      if (databuf.samples[j] == 0xffffu)
        break;
      data->samples[j] = databuf.samples[j];
      data->samples_count++;
    }
  }
  else
  {
    ack->which_payload = Ack_error_message_tag;
  }

  // encode the ack and return

  return encode_ack();
}

