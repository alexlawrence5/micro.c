CC = gcc
LD = ld

CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -O2
LDFLAGS = -m elf_i386 -T linker.ld

all: cos.iso

boot.o: boot.S
	$(CC) $(CFLAGS) -c boot.S -o boot.o

kernel.o: micro.c
	$(CC) $(CFLAGS) -c micro.c -o kernel.o

kernel.bin: boot.o kernel.o
	$(LD) $(LDFLAGS) -o kernel.bin boot.o kernel.o

cos.iso: kernel.bin grub.cfg
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/kernel.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o micro.iso iso

run: cos.iso
	qemu-system-i386 -cdrom micro.iso

clean:
	rm -rf *.o *.bin *.iso iso