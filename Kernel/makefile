CC = i686-elf-gcc
AS = i686-elf-as

CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra

all: mykernel.bin

boot.o: boot.s
	$(AS) boot.s -o boot.o

kernel.o: kernel.c
	$(CC) -c kernel.c -o kernel.o $(CFLAGS)

mykernel.bin: boot.o kernel.o linker.ld
	$(CC) -T linker.ld -o mykernel.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

clean:
	rm -f *.o mykernel.bin