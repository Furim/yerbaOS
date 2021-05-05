# This is the name that our final kernel executable will have.
# Change as needed.
KERNEL := yerbaos.elf
 
# It is highly recommended to use a custom built cross toolchain to build a kernel.
# We are only using "cc" as a placeholder here. It may work by using
# the host system's toolchain, but this is not guaranteed.
CC = cc
 
# User controllable CFLAGS.
CFLAGS = -Wall -Wextra -O2 -pipe
 
# Internal link flags that should not be changed by the user.
INTERNALLDFLAGS :=     \
	-fno-pic -fpie \
	-Wl,-static,-pie,--no-dynamic-linker,-ztext \
	-static-pie    \
	-nostdlib      \
	-Tlinker.ld    \
	-z max-page-size=0x1000
 
# Internal C flags that should not be changed by the user.
INTERNALCFLAGS  :=           \
	-I.                  \
	-std=gnu11           \
	-ffreestanding       \
	-fno-stack-protector \
	-fno-pic -fpie       \
	-mgeneral-regs-only  \
	-mno-red-zone
 
# Use find to glob all *.c files in the directory and extract the object names.
CFILES := $(shell find ./ -type f -name '*.c')
OBJ    := $(CFILES:.c=.o)
 
# Targets that do not actually build a file of the same name.
.PHONY: all clean
 
# Default target.
all: $(KERNEL)
 
# Link rules for the final kernel executable.
$(KERNEL): $(OBJ)
	$(CC) $(INTERNALLDFLAGS) $(OBJ) -o $@
 
# Compilation rules for *.c files.
%.o: %.c
	$(CC) $(CFLAGS) $(INTERNALCFLAGS) -c $< -o $@
 
# Remove object files and the final executable.
clean:
	rm -rf $(KERNEL) $(OBJ)
