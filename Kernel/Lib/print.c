#include "../OS/video.h"
#include "../OS/memory.h"
#include "varargs.h"
#include "string.h"
#include "print.h"

void vprintf(const char* format, va_list argv) {
    char str[32] = {0};
    int i, len = strlen(format);
    for(i=0;i<len;i++) {
        if(format[i] == '%') {
            if(i == len-1) continue;
            i++;
            switch(format[i]) {
                case 'd': {
                    int value = va_arg(argv, int);
                    itoa(value, str, 10, 0);
                    video_Print(str);
                    break;
                }
                case 'u': {
                    unsigned value = va_arg(argv, unsigned);
                    utoa(value, str, 10, 0);
                    video_Print(str);
                    break;
                }
                case 's': {
                    char* string = va_arg(argv, char*);
                    if(string) video_Print(string);
                    else       video_Print("(NULL)");
                    break;
                }
                case 'o': {
                    unsigned value = va_arg(argv, unsigned);
                    utoa(value, str, 8, 0);
                    video_Print(str);
                    break;
                }
                case 'x': {
                    unsigned long value = va_arg(argv, unsigned long);
                    utoa(value, str, 16, 0);
                    video_Print(str);
                    break;
                }
                case 'X': {
                    unsigned long value = va_arg(argv, unsigned long);
                    utoa(value, str, 16, 1);
                    video_Print(str);
                    break;
                }
                case 'c': {
                    char value = va_arg(argv, char);
                    str[0] = value;
                    str[1] = 0;
                    video_Print(str);
                    break;
                }
                case 'f': {
                    float value = va_arg(argv, float);

                    // Integer part
                    int intPart = (int)value;
                    itoa(intPart, str, 10, 0);
                    video_Print(str);
                    video_Print(".");

                    // Fractional part
                    value -= intPart;
                    if(value < 0) value = -value;
                    str[1] = 0;
                    int j;
                    for(j=0;j<6;j++) {
                        value *= 10;
                        str[0] = value + '0';
                        video_Print(str);
                        value -= str[0];
                    }

                    break;
                }
                case '%':
                    str[0] = '%';
                    str[1] = 0;
                    video_Print(str);
                    break;
            }
        } else {
            str[0] = format[i];
            str[1] = 0;
            video_Print(str);
        }
    }
}

void printf(const char* format, ...) {
    va_list argv;
    va_start(argv, format);
    vprintf(format, argv);
    va_end(argv);
}