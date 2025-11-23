#include <Arduino.h>

#include "actuation.h"


#define LIFTSERVONWPIN 7
#define LIFTSERVONEPIN 8
#define LIFTSERVOSEPIN 9
#define TILTSERVOXPIN  10
#define TILTSERVOYPIN  11


int initActuationPins() {
	pinMode(LIFTSERVONWPIN, OUTPUT);
	pinMode(LIFTSERVONEPIN, OUTPUT);
	pinMode(LIFTSERVOSEPIN, OUTPUT);
	pinMode(TILTSERVOXPIN, OUTPUT);
	pinMode(TILTSERVOYPIN, OUTPUT);
}

int liftSet(LiftServoState state) {
	analogWrite(LIFTSERVONWPIN, state.nw);
	analogWrite(LIFTSERVONEPIN, state.ne);
	analogWrite(LIFTSERVOSEPIN, state.se);
}

int tiltSet(TiltServoState state) {
	analogWrite(TILTSERVOXPIN, state.x);
	analogWrite(TILTSERVOYPIN, state.y);
}