# Les outils qu'on utilise
ASM = nasm
CC = x86_64-elf-gcc
LD = x86_64-elf-ld

# Les options de compilation
CFLAGS = -ffreestanding -m32 -I kernel/
LDFLAGS = -m elf_i386 -Ttext 0x7E00 --oformat binary

# Les fichiers objets à produire
OBJ = kernel/entry.o kernel/screen.o kernel/idt.o kernel/keyboard.o kernel/interrupt.o kernel/fs.o kernel/shell.o kernel/main.o

# Cible par défaut : construire tout
all: os.img

# Compiler le bootloader
boot.bin: boot.asm
	$(ASM) -f bin boot.asm -o boot.bin

# Compiler entry.asm
kernel/entry.o: kernel/entry.asm
	$(ASM) -f elf32 kernel/entry.asm -o kernel/entry.o

# Compiler les fichiers C
kernel/%.o: kernel/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compiler interrupt.asm
kernel/interrupt.o: kernel/interrupt.asm
	$(ASM) -f elf32 kernel/interrupt.asm -o kernel/interrupt.o

# Linker le kernel
kernel/kernel.bin: $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o kernel/kernel.bin

# Créer l'image disque finale
os.img: boot.bin kernel/kernel.bin
	dd if=/dev/zero of=os.img bs=1024 count=1440
	dd if=boot.bin of=os.img conv=notrunc
	dd if=kernel/kernel.bin of=os.img bs=512 seek=1 conv=notrunc

# Lancer QEMU
run: os.img
	qemu-system-x86_64 -drive format=raw,file=os.img

# Nettoyer les fichiers générés
clean:
	rm -f boot.bin kernel/*.o kernel/kernel.bin os.img
