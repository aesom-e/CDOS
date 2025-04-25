#include "port.h"

byte port_ByteIn(word port) {
    byte result;
    asm volatile("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

word port_WordIn(word port) {
    word result;
    asm volatile("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void port_ByteOut(word port, byte data) {
    asm volatile("out %%al, %%dx" : : "a" (data), "d" (port));
}

void port_WordOut(word port, word data) {
    asm volatile("out %%ax, %%dx" : : "a" (data), "d" (port));
}