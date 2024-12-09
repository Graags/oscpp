AS = i686-linux-gnu-as
CC = clang++ --target=i686-linux-gnu
CXXFLAGS = -ffreestanding -O3 -Wall -Wextra
LDFLAGS = -ffreestanding -nostdlib -O3
all: kernel.bin
clean:
	rm *.o
	rm *.bin

boot.o: boot.s
	$(AS) boot.s -o boot.o

kernel_main.o: kernel_main.cc
	$(CC) $(CXXFLAGS) -c kernel_main.cc -o kernel_main.o -fno-exceptions

vga_terminal.o: vga_terminal.cc
	$(CC) $(CXXFLAGS) -c vga_terminal.cc -o vga_terminal.o 

kernel.bin: boot.o kernel_main.o vga_terminal.o linker.ld
	$(CC) $(LDFLAGS) -T linker.ld -o kernel.bin *.o

run: all
	qemu-system-x86_64 -kernel kernel.bin
