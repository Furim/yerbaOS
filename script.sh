#!/bin/sh
#script for building kernel and iso image

rm -r iso_root/yerbaos.elf
rm -r image.iso
make clean
make
cp -v yerbaos.elf iso_root/ 

xorriso -as mkisofs -b limine-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table -eltorito-alt-boot -e limine-eltorito-efi.bin -no-emul-boot iso_root -o image.iso


qemu-system-x86_64 -cdrom image.iso
