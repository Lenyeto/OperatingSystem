

#hack to detect if we're running on Windows...
ifdef SYSTEMROOT   

#use these for Windows
CC=/cygdrive/c/GNUToolsARMEmbedded/5.3_2016q1/bin/arm-none-eabi-gcc.exe
LD=/cygdrive/c/GNUToolsARMEmbedded/5.3_2016q1/bin/arm-none-eabi-ld.exe
AS=$(CC)
OBJCOPY=/cygdrive/c/gcc-arm-none-eabi-5_3-2016q1-20160330-win32/bin/arm-none-eabi-objcopy.exe
QEMU=/cygdrive/c/qemu/qemu-system-arm.exe


else

#for Linux
CC=arm-linux-gnu-gcc
LD=arm-linux-gnu-ld
AS=$(CC)
OBJCOPY=arm-linux-gnu-objcopy
QEMU=qemu-system-arm


endif


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
	$(CC) kernelc.c
	$(CC) console.c
	$(CC) kprintf.c
	$(LD) -o kernel.tmp kernelasm.o kernelc.o console.o
	$(OBJCOPY) -Obinary kernel.tmp kernel.bin
	$(QEMU) $(QEMUARGS) kernel.bin
	
	
clean:
ifdef SYSTEMROOT
	-cmd //C del *.o *.exe *.bin *.img *.tmp
else
	-/bin/rm *.o *.exe *.bin *.img *.tmp
endif

	
