

#use these for Windows
CC=/cygdrive/c/Program Files (x86)/GNU Tools ARM Embedded/5.3 2016q1/bin/arm-none-eabi-gcc.exe
LD=/cygdrive/c/Program Files (x86)/GNU Tools ARM Embedded/5.3 2016q1/bin/arm-none-eabi-ld.exe
AS=$(CC)
OBJCOPY=/cygdrive/c/gcc-arm-none-eabi-5_3-2016q1-20160330-win32/bin/arm-none-eabi-objcopy.exe
QEMU=/cygdrive/c/qemu/qemu-system-arm.exe



#these should be cross-platform...
CC+= -Wall -c -mcpu=arm926ej-s -marm -Werror
LD+=-Map kernelmap.txt -T linkerscript.txt
AS+= -c -x assembler-with-cpp -mcpu=arm926ej-s
QEMUARGS=-machine integratorcp -kernel kernel.bin -serial stdio
DISPLAY?=:0
export DISPLAY
SDL_STDIO_REDIRECT=no
export SDL_STDIO_REDIRECT

all: 
	$(AS) kernelasm.s
	$(LD) -o kernel.tmp kernelasm.o
	$(OBJCOPY) -Obinary kernel.tmp kernel.bin
	$(QEMU) $(QEMUARGS) kernel.bin
	
	
clean:
	-cmd //C del *.o *.exe *.bin *.img *.tmp

	