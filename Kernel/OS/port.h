#include "types.h"

#ifndef OS_PORT_H
#define OS_PORT_H

byte port_ByteIn(word port);

word port_WordIn(word port);

void port_ByteOut(word port, byte data);

void port_WordOut(word port, word data);

#endif