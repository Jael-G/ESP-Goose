#pragma once
#include <Arduino.h>
#include <map>
#include <functional>
#include <USBHIDKeyboard.h>
#include <USBHIDMouse.h>
#include "debug.h"

#define DEBUG_MODE 1  // 1 = on, 0 = off

#if DEBUG_MODE
  #define DEBUG_PRINT(x)  Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)    // nothing
  #define DEBUG_PRINTLN(x)  // nothing
#endif


// == HID Devices ==
extern USBHIDKeyboard Keyboard;
extern USBHIDMouse Mouse;

// === Public API ===
// Separate script as string into lines and add to output array
int splitScript(const String &input, String output[], int maxLines); 

// Process script line by line
void runScript(const String script[], int length);

// Execute a line
void runLine(const String &line);

// === Command Handlers ===
void actionType(const String &text); // typeout an entire string
void actionPress(const String &key); // press a single key
void actionRelease(const String &key); // release a single key
void actionReleaseAll(const String &dummyArg); // release all pressed keys
void actionDelay(const String &ms); // delay by ms
void actionMoveMouseDX(const String &dx); // move mouse horizontally (relative)
void actionMoveMouseDY(const String &dy); // move mouse vertically (relative)
void actionLeftClick(const String &dummyArg); // left click
void actionRightClick(const String &dummyArg); // right click

// === Maps ===
extern std::map<String, std::function<void(const String&)>> commandMap; // string to action function
extern std::map<String, uint8_t> keyMap; // string to special key code