# List of all the project related files.
USE_HAL_I2C_FALLBACK = yes
include $(BOARDDIR)/BitTagNG/board.mk
#include $(CHIBIOS)/os/hal/lib/streams/streams.mk

ALLCSRC += \
       ADXL367.c \
       at25xe.c \
       config.c \
       datalog.c \
       default_config.c \
       handlers.c  \
       main.c \
       monitor.c \
       pb_common.c \
       pb_decode.c \
       pb_encode.c  \
       persistent.c \
       pwr.c \
       rtc_rv3028.c \
       state_machine.c  \
       state_run.c \
       stm32adc.c \
       stm32flash.c \
       tag.pb.c \
       tagdata.pb.c \
       test.c   \
       time.c   
   
