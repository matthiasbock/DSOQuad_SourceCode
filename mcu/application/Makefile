
###################################################
# Toolchain
###################################################

TC          = arm-none-eabi
CC          = $(TC)-gcc
AS          = $(TC)-as
AR          = $(TC)-ar
LD          = $(TC)-gcc
OBJCOPY     = $(TC)-objcopy
OBJDUMP     = $(TC)-objdump
SIZE        = $(TC)-size
GDB         = $(TC)-gdb


###################################################
# Toolchain configuration
###################################################

CPU = -mcpu=cortex-m3 -mthumb
DEFINES += -DEMBEDDED
DEFINES += -DDEBUG

ASMFLAGS += -Os
ASMFLAGS += $(CPU)

CFLAGS += -fno-common
CFLAGS += -Os
CFLAGS += -Wall -Wno-strict-aliasing -g
CFLAGS += $(CPU)
CFLAGS += $(DEFINES)

INCLUDES += -I include
INCLUDES += -I ../lib/FWLib/inc
INCLUDES += -I ../lib/USBLib/inc
CFLAGS += $(INCLUDES)

LINKER_FILE = stm32f10x_flash_offset.ld
LDFLAGS += $(CPU)
LDFLAGS += -T $(LINKER_FILE)
LDFLAGS += -nostartfiles


###################################################
# Targets
###################################################

TARGET = app
SRCS += $(wildcard source/*.c)
SRCS += $(wildcard ../lib/FWLib/src/*.c)
SRCS += $(wildcard ../lib/USBLib/src/*.c)
#SRCS += $(wildcard source/*.s)
OBJS += $(SRCS:.c=.o)

all: $(TARGET).bin

%.o: %.s
	$(AS) $(ASMFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.elf: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

%.bin: %.elf
	$(OBJCOPY) -Obinary -j .text $< $@

clean:
	rm -f $(OBJS) $(TARGET).bin $(TARGET).elf
