#ifndef LIB_STRING_H
#define LIB_STRING_H

void itoa(int value, char* str, int base, int isUppercase);

void utoa(unsigned value, char* str, int base, int isUppercase);

unsigned long strlen(const char* string);

char* strdup(const char* string);

int strequ(const char* a, const char* b);

int strnequ(const char* a, const char* b, int len);

char* strtok(char* string, char delimiter);

char* strlwr(const char* string);

char* strupr(const char* string);

char* strcpy(char* destination, const char* source);

char* strcat(char* destination, const char* source);

char* strchr(const char* string, char chr);

char* strrchr(const char* string, char chr);

#endif