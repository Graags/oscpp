AS = i686-linux-gnu-as
GCC = i686-linux-gnu-gcc

all: myos_bin
clean:
	rm boot.o kernel_main.o myos.bin

boot: boot.s
	$(AS) boot.s -o boot.o

kernel_main: kernel_main.c
	$(GCC) -c kernel_main.c -o kernel_main.o -ffreestanding -O2 -Wall -Wextra

myos_bin: boot kernel_main
	$(GCC) -T linker.ld -o myos.bin -ffreestanding -nostdlib -O2 boot.o kernel_main.o -lgcc

run: clean all
	qemu-system-i386 -kernel myos.bin
