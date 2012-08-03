EXECUTABLE=TFTLCD_test_FSMC.elf
BIN_IMAGE=TFTLCD_test_FSMC.bin
ARMGNU=arm-none-eabi


CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CFLAGS=-g -O2 -mlittle-endian -mthumb
CFLAGS+=-mcpu=cortex-m4
CFLAGS+=-ffreestanding -nostdlib

AOPS = --warn --fatal-warnings 
LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
LLCOPS2= -march=thumb -mcpu=cortex-m3
OOPS = -std-compile-opts

LLCFLAGS=-I./ -I../../STM32F4xx_StdPeriph_Driver/inc -I../../STM32F4xx_StdPeriph_Driver/inc/device_support -I../../STM32F4xx_StdPeriph_Driver/inc/core_support -I../../STM32F4xx_StdPeriph_Driver/inc -I../../Utilities/STM32F4-Discovery -I../../CMSIS/ST/STM32F4xx/Include -I../../CMSIS/Include
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

startup_stm32f4xx.o : startup_stm32f4xx.s
	$(ARMGNU)-as $(AOPS) startup_stm32f4xx.s -o startup_stm32f4xx.o

$(BIN_IMAGE): $(EXECUTABLE)
	$(OBJCOPY) -O binary $^ $@

#stm32f4xx_it.c
$(EXECUTABLE): main.c system_stm32f4xx.c startup_stm32f4xx.s stm32fxxx_it.c printf.c TFTLCD.c \
	../../Utilities/STM32F4-Discovery/stm32f4_discovery.c \

			
	$(CC) $(CFLAGS) $^ -o $@  -L../../STM32F4xx_StdPeriph_Driver/build -lSTM32F4xx_StdPeriph_Driver

clang_bc: main.c system_stm32f4xx.c startup_stm32f4xx.s stm32fxxx_it.c printf.c TFTLCD.c ../../Utilities/STM32F4-Discovery/stm32f4_discovery.c
	clang $(LOPS) -c main.c -o main.bc $(LLCFLAGS)
	clang $(LOPS) -c system_stm32f4xx.c -o system_stm32f4xx.bc $(LLCFLAGS)
	clang $(LOPS) -c stm32fxxx_it.c -o stm32fxxx_it.bc $(LLCFLAGS)
	clang $(LOPS) -c printf.c -o printf.bc $(LLCFLAGS)
	clang $(LOPS) -c TFTLCD.c -o TFTLCD.bc $(LLCFLAGS)
	clang $(LOPS) -c ../../Utilities/STM32F4-Discovery/stm32f4_discovery.c -o stm32f4_discovery.bc $(LLCFLAGS)
	llvm-link main.bc system_stm32f4xx.bc stm32fxxx_it.bc printf.bc TFTLCD.bc stm32f4_discovery.bc -o TFTLCD_test_FSMC.bc
	opt $(OOPS) TFTLCD_test_FSMC.bc -o TFTLCD_test_FSMC.opt.bc
	llc $(LLCOPS2) TFTLCD_test_FSMC.opt.bc -o TFTLCD_test_FSMC.opt.s
	
clang_bin: startup_stm32f4xx.o stm32_flash.ld TFTLCD_test_FSMC.opt.bc
	
	$(ARMGNU)-as $(AOPS) TFTLCD_test_FSMC.opt.s -o TFTLCD_test_FSMC.opt.o
	$(ARMGNU)-ld -o TFTLCD_test_FSMC.opt.elf -T rommap startup_stm32f4xx.o TFTLCD_test_FSMC.opt.o
	# $(ARMGNU)-ld -o TFTLCD_test_FSMC.opt.elf -T stm32_flash.ld startup_stm32f4xx.o TFTLCD_test_FSMC.opt.o
	$(ARMGNU)-objdump -D TFTLCD_test_FSMC.opt.elf > TFTLCD_test_FSMC.opt.list
	$(ARMGNU)-objcopy TFTLCD_test_FSMC.opt.elf TFTLCD_test_FSMC.opt.bin -O binary
	
clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(BIN_IMAGE)
	rm -f *.bc
	rm -f *.o
	rm -f TFTLCD_test_FSMC.opt.s

.PHONY: all clean

flash: STM32F4-Discovery_Demo.elf
	arm-none-eabi-gdb --command=gdbscript $(EXECUTABLE)

