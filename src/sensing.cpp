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

int initSensingPins() {
	pinMode(LIMIT1PIN, INPUT_PULLUP);
	pinMode(LIMIT2PIN, INPUT_PULLUP);
	pinMode(LIMIT3PIN, INPUT_PULLUP);
	pinMode(JOYBUTPIN, INPUT_PULLUP);
	pinMode(BUTTONPIN, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(JOYBUTPIN), joyButtonInterrupt, FALLING);
	attachInterrupt(digitalPinToInterrupt(BUTTONPIN), buttonInterrupt, FALLING);
}

SwitchState readLimits() {
	SwitchState state;
	state.nw = digitalRead(LIMIT1PIN);
	state.ne = digitalRead(LIMIT2PIN);
	state.se = digitalRead(LIMIT3PIN);

	return state;
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