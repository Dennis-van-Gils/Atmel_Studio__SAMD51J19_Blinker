How to get set up in Atmel Studio for working with SAMD51J19
(Adafruit Feather M4 Express)
-------------------------------------------------------------------------------
Dennis van Gils, 12-02-2020



MAIN CLOCK
----------
	
	The main CPU clock (GCLK_MAIN) of the SAMD51J19 chip needs to be configured
	properly using Atmel Start. It might not run out-of-the-box when using the
	default	clock settings of Atmel Start.
	The following works and results in a main CPU clock running at (a low)
	48 MHz.
	- Enable XOSC32K and feed it into Generic clock generator 3 (GCLK_GEN3),
	  which now operates at 32.768 kHz.
	- Enable DFLL48M being fed by GCLK_GEN3 and feed into GCLK_GEN0, which
	  now operates at 48 MHz.
	- Feed GCLK_GEN0 into the CPU clock.
	
	UPDATE:
	There is an article at <https://microchipsupport.force.com/s/article/SAM-D5x-E5x--Atmel-START-DPLL-configuration>
	describing how to configure the main clock for 120 MHz.
	


CODE OFFSET
-----------

	The Adafruit Feather M4 Express boards have a 16kb bootloader at address 0.
	We must skip these first bytes when writing the program code. Writing starts
	at address 0x4000.

	Properties -> Toolchain -> ARM/GNU Linker -> Miscellaneous
	Append the following to the Linker Flags:
		-Wl,--section-start=.text=0x4000

	

FLASHING SAMD51J19 CHIP
-----------------------
	
	I know of three possibilities:
	
	A) SWD and debugger
	
		Either use the SWD pins of the SAMD51 chip which you wire up to a
		debugger, e.g. J-Link or Atmel ICE or similar. The debugger gets
		recognized over the USB connection by Atmel Studio and allows for direct
		flashing of the code to the chip. The debugger provides even more
		powerful features such as step-by-step debugging at run-time and live
		monitoring of the chipset registers and memory content.
		
	B) Compile to disk, use Python
	
		Use Atmel Studio to just compile the project to disk. The compiled files
		can be found in the \Debug or \Release folder and have extension '.bin',
		'.elf' and '.hex'.
		
		There is a Python3 script at:
		<https://github.com/microsoft/uf2/blob/master/utils/uf2conv.py>
		that is able to transform the '.bin' file into a '.uf2' file, which can
		be copied over to the FeatherBoot drive. Copying a .uf2 file to the
		FeatherBoot drive will automatically flash the board and restart into
		program run.
		
		Command:
		python uf2conv.py -c -b 0x4000 -o SAMD51J19_Blinker.uf2 SAMD51J19_Blinker.bin
		
	C) Compile to disk, use bossac
	
		Another option to flash the chip with the files on disk would be to use
		the Windows tool called 'bossac'. However, this tool requires the
		Feather board to have a functioning serial port set up, which is not
		always the case when you configure the Atmel Studio project yourself
		by using Atmel Start.
		
		I could not get this to work:
		bossac -i -d --port=COM15 -U -i --offset=0x4000 -w -v SAMD51J19_Blinker.bin -R
		bossac --port=COM15 -e -w -v -R --offset=0x4000 SAMD51J19_Blinker.bin
		bossac -i -d --port=COM15 -U -i -e -w -v --offset=0x4000 SAMD51J19_Blinker.bin -R
		
	More reads:
	https://learn.adafruit.com/how-to-program-samd-bootloaders/programming-the-bootloader-with-atmel-studio
	https://learn.adafruit.com/adafruit-feather-m4-express-atsamd51/uf2-bootloader-details
	https://reprapdad.wordpress.com/2016/08/19/adafruit-feather-m0-with-atmel-studio/

	
