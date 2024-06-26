#ifndef DATALOG_H
#define DATALOG_H
#include "tagdata.pb.h"

// Stored Data Log -- in external memory

#define member_size(type, member) sizeof(((type *)0)->member)
#define DATALOG_SAMPLES member_size(AccelTagNgLog_samples_t,bytes)

typedef struct {
  uint8_t samples[DATALOG_SAMPLES];
} t_DataLog;

typedef struct {
  int32_t epoch;
  uint16_t millis;
  uint16_t shorts;
} t_DataHeader;

extern t_DataHeader vddHeader[];

extern enum LOGERR writeDataLog(uint16_t *data, int num);
extern enum LOGERR writeDataHeader(t_DataHeader *head);
extern int restoreLog(void);

#endif