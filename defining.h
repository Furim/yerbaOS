#include <stdint.h>
#include <stddef.h>
#include "stivale2.h"


#ifndef PROCESS_H
#define PROCESS_H


typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;



//defining print function(printing integers function)
void (*write)(const char *string, size_t length);
void print(int val) {
    char buf[30];
    buf[29] = 0; // Null terminator
    int written = 0;
    do {
        char c = '0' + (val % 10);
        val /= 10;
        buf[28 - written] = c;
        written++;
        } while (val != 0);
    
    char *string = buf + 29 - written;
    write(string, written);
                    }
//defining print function(printing integers function)


//defining idt_init



struct idt_descriptor {
     u16 offset_lowest; // MAX_UNSIGNED_SHORT
     u16 selector; // must point to gdt_descriptor
     u8 ist;
     u8 type_attr;
     u16 offset_medium;
     u32 offset_highest;
     u32 zero; 
}
idtdes;


void idt_init(){
    idtdes.offset_lowest = 0xFFFF;
    idtdes.selector = 0x08;
    idtdes.ist = 0;
    idtdes.type_attr = 0x8e;
    idtdes.offset_medium = 0xFFFF;
    idtdes.offset_highest = 0xFFFFFFFF;
}

//defining idt_init

//defining gdt_load


struct gdt_descriptor { //defining gdt_descriptor
    u32 _0;
    u8 _1, access, granularity, _2;
} __attribute__((packed));

struct gdt_pointer { // pointer where all values are gonna be stored and pointed
    u16 size; u64 addr;
} __attribute__((packed));



const u8 access_flagsGDT = 0b10010010;  // Present, ring 0 only, readable cs and writable data
const u8 gdt_is_code_segment = 1 << 3, gdt_longmode_cs = 1 << 5;



static struct gdt_descriptor gdt[] = {
    {},
    {.access = access_flagsGDT | gdt_is_code_segment, .granularity = gdt_longmode_cs},  // kern cs
    {.access = access_flagsGDT, .granularity = 0} // kern ds
};


void gdt_load() {
    struct gdt_pointer gdtr = {.size = sizeof(gdt) - 1, .addr = (u64)&gdt};
    
    asm volatile(
            
            "cli"
            
                );
    
    asm volatile(


            "lgdt %0\n\t"
            
            : : "m"(gdtr)
                );
    
    asm volatile(
            "leaq 1f(%%rip), %%rax\n"
            
            "pushq $0x08\n"
            
            "pushq %%rax\n"
            
            "lretq\n"
            
            "1:\n": :
                );

    asm volatile(
            
            "mov %0,%%ds\nmov %0,%%es\nmov %0,%%gs\nmov %0,%%fs\nmov %0,%%ss\n"
            
            : : "a"((u16)0x10)
                );
}
// defining gdt_load




#endif
