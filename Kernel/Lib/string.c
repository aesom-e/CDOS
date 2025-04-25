#include "../OS/memory.h"
#include "string.h"

void itoa(int value, char* str, int base, int isUppercase) {
    // Handle 0
    if(value == 0) {
        str[0] = '0';
        str[1] = 0;
        return;
    }

    char digits[] = "0123456789abcdef";
    char digitsUpper[] = "0123456789ABCDEF";
    char* useDigits = isUppercase ? digitsUpper : digits;

    int i = 0, isNegative = 0;

    // Handle negative numbers for base 10
    if(base == 10 && value < 0) {
        isNegative = 1;
        value = -value;
    }

    while(value > 0) {
        str[i++] = useDigits[value % base];
        value /= base;
    }

    if(isNegative) str[i++] = '-';

    str[i] = '\0';

    // Reverse the string
    int j, k = i - 1;
    for(j=0;j<k;j++,k--) {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }
}

void utoa(unsigned int value, char* str, int base, int isUppercase) {
    // Handle 0
    if(value == 0) {
        str[0] = '0';
        str[1] = 0;
        return;
    }

    char digits[] = "0123456789abcdef";
    char digitsUpper[] = "0123456789ABCDEF";
    char* useDigits = isUppercase ? digitsUpper : digits;

    int i = 0;

    while(value > 0) {
        str[i++] = useDigits[value % base];
        value /= base;
    }

    str[i] = '\0';

    // Reverse the string
    int j, k = i - 1;
    for(j=0;j<k;j++,k--) {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }
}

unsigned long strlen(const char* string) {
    unsigned long len = 0;
    while(string[len]) len++;
    return len;
}

char* strdup(const char* string) {
    unsigned long i, len = strlen(string);
    char* ret = memory_Callocate(len+1);

    for(i=0;i<len;i++) ret[i] = string[i];
    ret[i] = 0;

    return ret;
}

int strequ(const char* a, const char* b) {
    if(strlen(a) != strlen(b)) return 0;
    int i, len = strlen(a);
    for(i=0;i<len;i++) {
        if(a[i] != b[i]) return 0;
    }
    return 1;
}

int strnequ(const char* a, const char* b, int len) {
    if(strlen(a) < len || strlen(b) < len) return 0;
    int i;
    for(i=0;i<len;i++) {
        if(a[i] != b[i]) return 0;
    }
    return 1;
}

char* strtokString = NULL;
char* strtok(char* string, char delimiter) {
    if(string) strtokString = string;

    if(!strtokString || !(*strtokString)) return NULL;

    char* tokenStart = strtokString;
    while(*strtokString && *strtokString != delimiter) strtokString++;
    if(*strtokString == delimiter) *(strtokString++) = 0;
    return tokenStart;
}

char* strlwr(const char* string) {
    char* ret = strdup(string);
    unsigned long i, len = strlen(string);
    for(i=0;i<len;i++) {
        if(ret[i] >= 'A' && ret[i] <= 'Z')
            ret[i] = ret[i] - 'A' + 'a';
    }
    return ret;
}
char* strupr(const char* string) {
    char* ret = strdup(string);
    unsigned long i, len = strlen(string);
    for(i=0;i<len;i++) {
        if(ret[i] >= 'a' && ret[i] <= 'z')
            ret[i] = ret[i] - 'a' + 'A';
    }
    return ret;
}


char* strcpy(char* destination, const char* source) {
    char* destPtr = destination;
    while((*destPtr++ = *source++));
    return destination;
}

char* strcat(char* destination, const char* source) {
    char* destPtr = destination;
    while(*destPtr) destPtr++;
    while((*destPtr++ = *source++));
    return destination;
}

char* strchr(const char* string, char chr) {
    unsigned long i = 0;
    while(string[i-1] || !i) {
        if(string[i] == chr)
            return (char*)((dWord)string+i); // Trick the compiler
        i++;
    }
    return NULL;
}

char* strrchr(const char* string, char chr) {
    char* str = strchr(string, chr), *str2 = NULL;
    while(str) {
        str2 = str;
        str = strchr(str+1, chr);
    }
    return str2;
}