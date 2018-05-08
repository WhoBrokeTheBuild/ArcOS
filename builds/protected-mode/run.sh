#!/bin/sh


nasm -f bin -o test.bin test.asm
qemu-system-i386 -drive format=raw,file=test.bin
