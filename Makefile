AS = i686-linux-gnu-as
GCC = i686-linux-gnu-g++

CC_OPTS = -ffreestanding -O2 -Wall -Wextra

all: kernel.bin
clean:
	rm *.o *.bin

boot.o: boot.s
	$(AS) boot.s -o boot.o

kernel_main.o: kernel_main.cc
	$(GCC) -c kernel_main.cc -o kernel_main.o $(CC_OPTS)

vga_terminal.o: vga_terminal.cc
	$(GCC) -c vga_terminal.cc -o vga_terminal.o $(CC_OPTS)

kernel.bin: boot.o kernel_main.o vga_terminal.o linker.ld
	$(GCC) -T linker.ld -o kernel.bin -ffreestanding -nostdlib -O2 *.o -lgcc

run: all
	qemu-system-i386 -kernel kernel.bin
