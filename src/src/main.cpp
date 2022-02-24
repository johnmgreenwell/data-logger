//----------------------------------------------------------------------------
// Name        : main.cpp
// Purpose     : Data Logger Test
// Description : 
//               This program intended to test the circuit for custom sd card
//               and RTC data-logger board.
//
// Platform    : Atmega328
// Language    : C++
// Framework   : Arduino
// Copyright   : MIT License 2022, John Greenwell
// Requires    : External Libraries : TimeLib.h
//                                    DS1307RTC.h
//                                    SD.h
//               Custom Libraries   : N/A
//----------------------------------------------------------------------------

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <SD.h>

// Objects
tmElements_t tm;
File root;

// Constants
const uint8_t INT0_PIN = 2;
const uint8_t CS_PIN = 10;
const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

// Local Functions
bool getTime(const char *str);
bool getDate(const char *str);

// Main Program
void setup(void) {
    bool parse=false;
    bool config=false;

    pinMode(INT0_PIN, INPUT);
    SPI.begin();
    Wire.begin();

    // Initialize RTC ref. DS1307RTC lib by Paul Stoffregen
    if (getDate(__DATE__) && getTime(__TIME__)) {
        parse = true;
        if (RTC.write(tm)) {
            config = true;
        }
  }

    // Initialize SD
    if (!SD.begin(CS_PIN)) {
        while (1);
    }
    root = SD.open("/");

}

void loop() {
    delay(1000);
}

// ref. DS1307RTC library by Paul Stoffregen
bool getTime(const char *str) {
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}

// ref. DS1307RTC library by Paul Stoffregen
bool getDate(const char *str) {
  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++) {
    if (strcmp(Month, monthName[monthIndex]) == 0) break;
  }
  if (monthIndex >= 12) return false;
  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);
  return true;
}

