#ifndef LIB_TIME_H
#define LIB_TIME_H

#include "../OS/types.h"

// Stored in half-hours
typedef enum {
    TIMEZONE_SYSTEM     = 127, // System time
    TIMEZONE_KIRITIMATI = 28,  // Christmas Island
    TIMEZONE_SAMOA      = 26,  // Tonga and Samoa
    TIMEZONE_NZST       = 24,  // New Zealand
    TIMEZONE_SOLOMON    = 22,  // Solomon Islands and Vanuatu
    TIMEZONE_AEST       = 20,  // Australian Eastern Time (Sydney/Melbourne)
    TIMEZONE_ACST       = 19,  // Australian Central Time (Adelaide)
    TIMEZONE_JST        = 18,  // Japanese and Korean Standard Time
    TIMEZONE_AWST       = 16,  // Australian Western Time (Perth, China, Malaysia, Singapore, and Hong Kong)
    TIMEZONE_SEA        = 14,  // South-East Asia (Thailand, Vietnam, and Cambodia)
    TIMEZONE_BANGLADESH = 12,  // Bangladesh and Bhutan
    TIMEZONE_IST        = 11,  // Indian Standard Time
    TIMEZONE_PKST       = 10,  // Pakistani Standard Time
    TIMEZONE_AFT        = 9,   // Afghani Standard Time
    TIMEZONE_GST        = 8,   // Gulf Standard Time (UAE and Oman)
    TIMEZONE_MSK        = 6,   // Moscow Standard Time (Western Russia, Turkey, Saudi Arabia, Kenya, and Ethiopia)
    TIMEZONE_EET        = 4,   // Eastern European Time (Finland, Greece, Romania, Ukraine)
    TIMEZONE_CET        = 2,   // Central European Time (Western Europe)
    TIMEZONE_UTC        = 0,   // UTC (UK and Portugal)
    TIMEZONE_AZR        = -2,  // Azores
    TIMEZONE_SGEORGIA   = -4,  // South Georgia (South American island) and the South Sandwich Islands
    TIMEZONE_BRAZIL     = -6,  // Eastern Brazil, Argentina, and Chile
    TIMEZONE_NEWFIE     = -7,  // Newfoundland
    TIMEZONE_AST        = -8,  // Atlantic Standard Time
    TIMEZONE_EST        = -10, // Eastern Standard Time
    TIMEZONE_CST        = -12, // Central Standard Time
    TIMEZONE_MST        = -14, // Mountain Standard Time
    TIMEZONE_PST        = -16, // Pacific Standard Time
    TIMEZONE_ASKT       = -18, // Alaskan Standard Time
    TIMEZONE_HST        = -20  // Hawaiian Standard Time
} TimeZone;

typedef struct {
    byte second;
    byte minute;
    byte hour;
    byte day;
    byte month;
    word year;
    TimeZone timeZone;
} Time;

void time_SetSystemTime(TimeZone systemTime);

void time_Use24Hour(void);

void time_Use12Hour(void);

Time time_GetFATTime(word date, word time);

void time_AdjustTimeZone(Time* time, TimeZone newTimeZone);

char* time_GetTimeAsString(Time time);

#endif