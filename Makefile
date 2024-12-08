AS = i686-linux-gnu-as
GCC = i686-linux-gnu-g++

all: kernel.bin
clean:
	rm boot.o kernel_main.o kernel.bin

boot: boot.s
	$(AS) boot.s -o boot.o

kernel_main: kernel_main.cc
	$(GCC) -c kernel_main.cc -o kernel_main.o -ffreestanding -O2 -Wall -Wextra

kernel.bin: boot kernel_main linker.ld
	$(GCC) -T linker.ld -o kernel.bin -ffreestanding -nostdlib -O2 boot.o kernel_main.o -lgcc

run: all
	qemu-system-i386 -kernel kernel.bin
