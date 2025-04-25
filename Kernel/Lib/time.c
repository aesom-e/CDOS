#include "../OS/memory.h"
#include "string.h"
#include "time.h"

TimeZone SYSTEM_TIME = TIMEZONE_UTC;
byte     USE24HOUR = 1;

void time_SetSystemTime(TimeZone systemTime) {
    SYSTEM_TIME = systemTime;
}

void time_Use24Hour(void) {
    USE24HOUR = 1;
}

void time_Use12Hour(void) {
    USE24HOUR = 0;
}

Time time_GetFATTime(word date, word time) {
    Time ret = {.timeZone = TIMEZONE_UTC};

    ret.year   = 1980 + ((date >> 9) & 0x7f);
    ret.month  = (date >> 5) & 0x0f;
    ret.day    = date & 0x1f;
    ret.hour   = (time >> 11) & 0x1f;
    ret.minute = (time >> 5) & 0x3f;
    ret.second = (time & 0x1f) * 2;

    return ret;
}

void time_AdjustTimeZone(Time* time, TimeZone newTimeZone) {
    if(!time) return;

    // Check if the user is asking for system time
    if(newTimeZone == TIMEZONE_SYSTEM)  newTimeZone = SYSTEM_TIME;
    if(newTimeZone == -TIMEZONE_SYSTEM) newTimeZone = -SYSTEM_TIME;

    int offsetMinutes = (newTimeZone - time->timeZone) * 30;
    int totalMinutes  = (time->hour * 60) + time->minute + offsetMinutes;

    // Adjust the hours and minutes
    const static byte daysPerMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    while(totalMinutes < 0) {
        // This an underflow into the previous day
        totalMinutes += 1440; // 24 * 60
        if(time->day > 1) {
            time->day--;
        } else {
            if(time->month > 1) {
                time->month--;
            } else {
                time->month = 12;
                time->year--;
            }

            // Get the last day of the previous month
            time->day = daysPerMonth[time->month-1];
            // Handle leap years
            if(time->month == 2 && time->year % 4 == 0 && (time->year % 100 || time->year % 400 == 0))
                time->day = 29;
        }
    }
    while(totalMinutes >= 1440) {
        // Handle overflow into the next day
        totalMinutes -= 1440;
        time->day++;

        int daysInTheMonth = daysPerMonth[time->month-1];
        if(time->month == 2 && time->year % 4 == 0 && (time->year % 100 || time->year % 400 == 0))
            daysInTheMonth = 29;

        if(time->day > daysInTheMonth) {
            time->day = 1;
            if(time->month<12)
                time->month++;
            else {
                time->month = 1;
                time->year++;
            }
        }
    }

    // Apply the rest of the adjustments
    time->hour     = totalMinutes / 60;
    time->minute   = totalMinutes % 60;
    time->timeZone = newTimeZone;
}

char* time_GetTimeAsString(Time time) {
    char* timeString = memory_Callocate(USE24HOUR ? 20 : 23);
    char temp[5];

    // Year
    itoa(time.year, temp, 10, 0);
    strcpy(timeString, temp);
    strcat(timeString, "-");

    // Month
    if(time.month < 10) strcat(timeString, "0");
    itoa(time.month, temp, 10, 0);
    strcat(timeString, temp);
    strcat(timeString, "-");

    // Day
    if(time.day < 10) strcat(timeString, "0");
    itoa(time.day, temp, 10, 0);
    strcat(timeString, temp);
    strcat(timeString, " ");

    // Hour
    int hour = time.hour;
    if(!USE24HOUR && hour > 12) hour -= 12;
    if(hour < 10) strcat(timeString, "0");
    itoa(hour, temp, 10, 0);
    strcat(timeString, temp);
    strcat(timeString, ":");

    // Minute
    if(time.minute < 10) strcat(timeString, "0");
    itoa(time.minute, temp, 10, 0);
    strcat(timeString, temp);
    strcat(timeString, ":");

    // Day
    if(time.second < 10) strcat(timeString, "0");
    itoa(time.second, temp, 10, 0);
    strcat(timeString, temp);

    // AM/PM for 12-hour time
    if(!USE24HOUR) {
        strcat(timeString, " ");
        if(time.hour <= 12) strcat(timeString, "AM");
        else                strcat(timeString, "PM");
    }

    return timeString;
}