#include <Arduino.h>

#include "Display.h"

DisplayController::DisplayController(int LatchPin, int ClockPin, int DataPin)
	: kPinLatch_(LatchPin), kPinClock_(ClockPin), kPinData_(DataPin) {}

void DisplayController::initPins() {
	pinMode(kPinLatch_, OUTPUT);
	pinMode(kPinClock_, OUTPUT);
	pinMode(kPinData_, OUTPUT);
}

void DisplayController::writeDisplay(int msg) {
	writeChar((msg >> 8 * 3) & 0xFF);
	writeChar((msg >> 8 * 2) & 0xFF);
	writeChar((msg >> 8 * 1) & 0xFF);
	writeChar((msg >> 8 * 0) & 0xFF);
}

void DisplayController::writeChar(unsigned char msg) {
	digitalWrite(kPinLatch_, LOW);
	shiftOut(kPinData_, kPinClock_, MSBFIRST, msg);
	digitalWrite(kPinLatch_, HIGH);
}