#include "idt.h"

// Structure d'une entrée IDT
struct idt_entry {
    unsigned short base_low;
    unsigned short selector;
    unsigned char zero;
    unsigned char flags;
    unsigned short base_high;
} __attribute__((packed));

struct idt_ptr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

static struct idt_entry idt[256];
static struct idt_ptr idtp;

// Ecrire dans un port
static void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Lire un port
static unsigned char inb(unsigned short port) {
    unsigned char result;
    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void idt_set_gate(int n, unsigned int base) {
    idt[n].base_low  = base & 0xFFFF;
    idt[n].base_high = (base >> 16) & 0xFFFF;
    idt[n].selector  = 0x08;
    idt[n].zero      = 0;
    idt[n].flags     = 0x8E;
}

// Handler clavier déclaré dans keyboard.c
extern void keyboard_handler(void);

void idt_init(void) {
    // Initialiser le PIC
    outb(0x20, 0x11);  // init PIC maitre
    outb(0xA0, 0x11);  // init PIC esclave
    outb(0x21, 0x20);  // IRQ0-7 -> interruptions 0x20-0x27
    outb(0xA1, 0x28);  // IRQ8-15 -> interruptions 0x28-0x2F
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0xFD);  // activer seulement IRQ1 (clavier)
    outb(0xA1, 0xFF);  // masquer tout le PIC esclave

    // Enregistrer le handler clavier sur IRQ1 = interruption 0x21
    idt_set_gate(0x21, (unsigned int)keyboard_handler);

    // Charger l'IDT
    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (unsigned int)&idt;
    __asm__ volatile ("lidt %0" : : "m"(idtp));

    // Activer les interruptions
    __asm__ volatile ("sti");
}
