AS = i686-linux-gnu-as
CC = clang++ --target=i686-linux-gnu
CXXFLAGS = -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -ffreestanding -nostdlib -O2 -lgcc -lstdc++ -lsupc++
all: kernel.bin
clean:
	rm -f *.bin
	rm -f *.o

boot.o: boot.s
	$(AS) boot.s -o boot.o

experiments.o: experiments.cc
	$(CC) $(CXXFLAGS) -c experiments.cc -o experiments.o -fno-exceptions

kernel_main.o: kernel_main.cc
	$(CC) $(CXXFLAGS) -c kernel_main.cc -o kernel_main.o

vga_terminal.o: vga_terminal.cc
	$(CC) $(CXXFLAGS) -c vga_terminal.cc -o vga_terminal.o

interrupt.o: interrupt.cc
	$(CC) $(CXXFLAGS) -c interrupt.cc -o interrupt.o -mgeneral-regs-only

interrupt.s: interrupt.cc
	$(CC) $(CXXFLAGS) -S interrupt.cc -o interrupt.s -mgeneral-regs-only

pic8259.o: pic8259.cc
	$(CC) $(CXXFLAGS) -c pic8259.cc -o pic8259.o -mgeneral-regs-only

pic8259.s: pic8259.cc
	$(CC) $(CXXFLAGS) -S pic8259.cc -o pic8259.s -mgeneral-regs-only

kernel.bin: boot.o kernel_main.o vga_terminal.o experiments.o interrupt.o pic8259.o linker.ld
	$(CC) $(LDFLAGS) -T linker.ld -o kernel.bin *.o

run: all
	qemu-system-x86_64 -kernel kernel.bin
