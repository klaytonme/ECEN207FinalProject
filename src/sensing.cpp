#include <Arduino.h>

#include "sensing.h"

#define LIMIT1PIN 1
#define LIMIT2PIN 2
#define LIMIT3PIN 3
#define JOYXPIN	  A0
#define JOYYPIN	  A1
#define BUTTONPIN 4


int initSensingPins() {
	pinMode(LIMIT1PIN, INPUT_PULLUP);
	pinMode(LIMIT2PIN, INPUT_PULLUP);
	pinMode(LIMIT3PIN, INPUT_PULLUP);
	pinMode(BUTTONPIN, INPUT_PULLUP);
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