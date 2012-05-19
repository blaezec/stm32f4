you need the STM32F4 library and source in ../../
you need the arm-none-eabi-gcc compiler

to compile type:
make

to load using the stm32 boot loader, you need dfu-util.  Once you have it working,
cd to you working directory, connect BOOT0 pin to 3V pin and press reset.

Now type:

sudo dfu-util  --device 0483:df11 --alt 0 --dfuse-address 0x08000000 --download TFTLCD_test_FSMC.bin

Disconnect the BOOT0 pin from the 3v pin. Press reset. your software is now running on st32f4 discovery (or crashing...).

---

I used FSMC D[8:15] pins on the stm32f4, with the adafruit '2.8" 18-bit color TFT LCD with touchscreen breakout board
- ILI9325' (http://www.adafruit.com/products/335). $40. Usually out of stock. She wired it to use 8 bit data bus, so it
would work with avr µControllers (read: arduino). Bummer it's hardwired that way, it could be 2X as fast with the stm32f4,
than it is w/ 8 bit data bus.

You can use D[0:7] if you want, it seems to work. But LED5 and LED6 or on pins FSMC D0 and D1 and the signal is a bit droopy because
of that. To use D[0:7], go into TFTLCD.c, line 870 or so, (/********************************** low level readwrite interface */)
and comment out the d[8:15] versions of
writeData
writeCommand
readData

and uncomment the d[0:7] versions.
