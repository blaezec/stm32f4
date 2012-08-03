EXECUTABLE=TFTLCD_test_FSMC.elf
BIN_IMAGE=TFTLCD_test_FSMC.bin


CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CFLAGS=-g -O2 -mlittle-endian -mthumb
CFLAGS+=-mcpu=cortex-m4
CFLAGS+=-ffreestanding -nostdlib -fno-math-errno

#usb_conf.h
CFLAGS+=-DUSE_USB_OTG_FS=1

# to run from FLASH
CFLAGS+=-Wl,-T,stm32_flash.ld

CFLAGS+=-I./

# stm32f4_discovery lib
CFLAGS+=-I../../STM32F4xx_StdPeriph_Driver/inc
CFLAGS+=-I../../STM32F4xx_StdPeriph_Driver/inc/device_support
CFLAGS+=-I../../STM32F4xx_StdPeriph_Driver/inc/core_support

#STM32_USB_Device_Library
# CFLAGS+=-I../../STM32_USB_Device_Library/Class/cdc/inc
# CFLAGS+=-I../../STM32_USB_Device_Library/Core/inc

#STM32_USB_OTG_Driver
# CFLAGS+=-I../../STM32_USB_OTG_Driver/inc

#STM32F4xx_StdPeriph_Driver\inc
CFLAGS+=-I../../STM32F4xx_StdPeriph_Driver/inc

#Utilities
CFLAGS+=-I../../Utilities/STM32F4-Discovery

CFLAGS+=-I../../CMSIS/ST/STM32F4xx/Include

CFLAGS+=-I../../CMSIS/Include
all: $(BIN_IMAGE)

$(BIN_IMAGE): $(EXECUTABLE)
	$(OBJCOPY) -O binary $^ $@

#stm32f4xx_it.c
$(EXECUTABLE): main.c system_stm32f4xx.c startup_stm32f4xx.s stm32fxxx_it.c printf.c TFTLCD.c \
	../../Utilities/STM32F4-Discovery/stm32f4_discovery.c \

			
	$(CC) $(CFLAGS) $^ -o $@  -L../../STM32F4xx_StdPeriph_Driver/build -lSTM32F4xx_StdPeriph_Driver


clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(BIN_IMAGE)

.PHONY: all clean

flash: STM32F4-Discovery_Demo.elf
	arm-none-eabi-gdb --command=gdbscript $(EXECUTABLE)

