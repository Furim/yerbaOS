#include <stdint.h>
#include <stddef.h>
#include "stivale2.h"
#include "defining.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;





// PMM is =  specifically only allows to allocate and free physical memory



// We need to tell the stivale bootloader where we want our stack to be.
// We are going to allocate our stack as an uninitialised array in .bss.
static uint8_t stack[4096];
 
// stivale2 uses a linked list of tags for both communicating TO the
// bootloader, or receiving info FROM it. More information about these tags
// is found in the stivale2 specification.
 
// stivale2 offers a runtime terminal service which can be ditched at any
// time, but it provides an easy way to print out to graphical terminal,
// especially during early boot.
static struct stivale2_header_tag_terminal terminal_hdr_tag = {
    // All tags need to begin with an identifier and a pointer to the next tag.
    .tag = {
        // Identification constant defined in stivale2.h and the specification.
        .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
        // If next is 0, it marks the end of the linked list of header tags.
        .next = 0
    },
    // The terminal header tag possesses a flags field, leave it as 0 for now
    // as it is unused.
    .flags = 0
};
 
// We are now going to define a framebuffer header tag, which is mandatory when
// using the stivale2 terminal.
// This tag tells the bootloader that we want a graphical framebuffer instead
// of a CGA-compatible text mode. Omitting this tag will make the bootloader
// default to text mode, if available.
static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    // Same as above.
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        // Instead of 0, we now point to the previous header tag. The order in
        // which header tags are linked does not matter.
        .next = (uint64_t)&terminal_hdr_tag
    },
    // We set all the framebuffer specifics to 0 as we want the bootloader
    // to pick the best it can.
    .framebuffer_width  = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp    = 0
};
 
// The stivale2 specification says we need to define a "header structure".
// This structure needs to reside in the .stivale2hdr ELF section in order
// for the bootloader to find it. We use this __attribute__ directive to
// tell the compiler to put the following structure in said section.
__attribute__((section(".stivale2hdr"), used))
static struct stivale2_header stivale_hdr = {
    // The entry_point member is used to specify an alternative entry
    // point that the bootloader should jump to instead of the executable's
    // ELF entry point. We do not care about that so we leave it zeroed.
    .entry_point = 0,
    // Let's tell the bootloader where our stack is.
    // We need to add the sizeof(stack) since in x86(_64) the stack grows
    // downwards.
    .stack = (uintptr_t)stack + sizeof(stack),
    // No flags are currently defined as per spec and should be left to 0.
    .flags = 0,
    // This header structure is the root of the linked list of header tags and
    // points to the first one in the linked list.
    .tags = (uintptr_t)&framebuffer_hdr_tag
};
 
// We will now write a helper function which will allow us to scan for tags
// that we want FROM the bootloader (structure tags).
void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id) {
    struct stivale2_tag *current_tag = (void *)stivale2_struct->tags;
    for (;;) {
        // If the tag pointer is NULL (end of linked list), we did not find
        // the tag. Return NULL to signal this.
        if (current_tag == NULL) {
            return NULL;
        }
 
        // Check whether the identifier matches. If it does, return a pointer
        // to the matching tag.
        if (current_tag->identifier == id) {
            return current_tag;
        }
 
        // Get a pointer to the next tag in the linked list and repeat.
        current_tag = (void *)current_tag->next;
    }
}
 
// The following will be our kernel's entry point.
  // Let's get the terminal structure tag from the bootloader.
 void (*write)(const char *string, size_t length);
// The following will be our kernel's entry point.
void _start(struct stivale2_struct *stivale2_struct) {
    // Let's get the terminal structure tag from the bootloader.
   

    
    struct stivale2_struct_tag_memmap *mmap = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_MEMMAP_ID);
    uint64_t length = 0;
    for (int i = 0; i < mmap->entries; i++) {
    struct stivale2_mmap_entry *m = &mmap->memmap[i];
    if (m->type == STIVALE2_MMAP_USABLE) length += m->length;
                                            }
    length /= (1024 * 1024);

    struct stivale2_struct_tag_terminal *term_str_tag;
    term_str_tag = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_TERMINAL_ID);

    // Check if the tag was actually found.
    
    if (term_str_tag == NULL) {
        // It wasn't found, just hang...
        for (;;) {
            asm ("hlt");
        }
    }

    if (mmap == NULL){
        for (;;) {
            asm ("hlt");
        }
    }


    void *term_write_ptr = (void *)term_str_tag->term_write;


   


    write = term_write_ptr;
  


    
    write("Welcome to YerbaOS\n", 19);
    

    write("Memory map:\n ", 14);
    

    print(mmap); // Memory map
    
    write("\nHuman readable memory map:\n", 27);
    

    print(length); // Formated memory map into GB aka human readable memory output 
    

    gdt_load(); 
    

    idt_init(); //defined in defining.h





    for (;;) {
        asm ("hlt");
    
             }
}


