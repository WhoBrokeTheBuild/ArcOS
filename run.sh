#!/bin/sh

nasm -f bin -o test.bin test.asm
qemu -fda test.bin
