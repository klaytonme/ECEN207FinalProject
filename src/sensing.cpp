#include <Arduino.h>

#include "sensing.h"

#define LIMIT1PIN 1
#define LIMIT2PIN 2
#define LIMIT3PIN 3
#define JOYXPIN	  A0
#define JOYYPIN	  A1
#define JOYBUTPIN 5
#define BUTTONPIN 4

volatile bool buttonPressed	   = false;
volatile bool joyButtonPressed = false;

void buttonInterrupt();
void joyButtonInterrupt();


LimitController::LimitController(int NELimitPin, int NWLimitPin, int SELimitPin)
	: kPinNW_(NWLimitPin), kPinNE_(NELimitPin), kPinSE_(SELimitPin) {}

void LimitController::initPins() {
	pinMode(kPinNW_, INPUT_PULLUP);
	pinMode(kPinNE_, INPUT_PULLUP);
	pinMode(kPinSE_, INPUT_PULLUP);
}

LimitState LimitController::readLimits() {
	state_.nw = digitalRead(kPinNW_);
	state_.ne = digitalRead(kPinNE_);
	state_.se = digitalRead(kPinSE_);

	return state_;
}

bool LimitController::isDown() {
	readLimits();

	return (!state_.nw && !state_.ne && !state_.se);
}



UIController::UIController(int ButtonPin, int JoyXPin, int JoyYPin, int JoyButtonPin, int CoinPin, int WinPin)
	: kPinButton_(ButtonPin), kPinJoyX_(JoyXPin), kPinJoyY_(JoyYPin), kPinJoyButton_(JoyButtonPin), kPinCoin_(CoinPin),
	  kPinWin_(WinPin) {}

void UIController::initPins() {
	pinMode(JOYBUTPIN, INPUT_PULLUP);
	pinMode(BUTTONPIN, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(JOYBUTPIN), joyButtonInterrupt, FALLING);
	attachInterrupt(digitalPinToInterrupt(BUTTONPIN), buttonInterrupt, FALLING);
}


int initSensingPins() {
	pinMode(LIMIT1PIN, INPUT_PULLUP);
	pinMode(LIMIT2PIN, INPUT_PULLUP);
	pinMode(LIMIT3PIN, INPUT_PULLUP);
}



JoyState readJoy() {
	JoyState state;
	state.x = analogRead(JOYXPIN);
	state.y = analogRead(JOYYPIN);

	return state;
}

bool buttonIsPressed() {
	return digitalRead(BUTTONPIN);
}

bool buttonWasPressed() {
	__disable_irq;
	bool tempButtonState = buttonPressed;
	buttonPressed		 = false;
	__enable_irq;

	return tempButtonState;
}

void buttonInterrupt() {
	buttonPressed = true;
}

void joyButtonInterrupt() {
	joyButtonPressed = true;
}