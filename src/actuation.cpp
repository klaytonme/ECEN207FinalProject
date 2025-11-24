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

int TiltController::tiltUpdate() {
	analogWrite(TILTSERVOXPIN, state.x);
	analogWrite(TILTSERVOYPIN, state.y);
}



TiltController::TiltController() {
	orig.x	= 0;
	orig.y	= 0;
	state.x = 0;
	state.y = 0;
	targ.x	= 0;
	targ.y	= 0;
}

TiltServoState TiltController::getPosition() {
	return state;
}

void TiltController::setTraj(TiltServoState targ_in, int speed) {
	if (speed == TILTSPEED_IMMEDIATE) {
		state = targ_in;
		tiltUpdate();
		targ = targ_in;
		orig = targ_in;
		return;
	}

	orig = state;
	targ = targ_in;

	int dist = max(abs(orig.x - targ.x), abs(orig.y - targ.y));
	duration = dist * speed;

	// Serial.print(dist);
	// Serial.print(" * ");
	// Serial.print(speed);
	// Serial.print(" = ");
	// Serial.print(duration);
	// Serial.print(" | (");
	// Serial.print(orig.x);
	// Serial.print(", ");
	// Serial.print(orig.y);
	// Serial.print("), (");
	// Serial.print(targ.x);
	// Serial.print(", ");
	// Serial.print(targ.y);
	// Serial.println(")");
}

int TiltController::updateTraj(time_t elapsed) {

	// Serial.print("BEFORE: ");
	// Serial.print(elapsed);
	// Serial.print(", ");
	// Serial.print(duration);
	// Serial.print(", ");
	// Serial.print(orig.x);
	// Serial.print(", ");
	// Serial.println(targ.x);

	state.x = constrain(map(elapsed, 0, duration, orig.x, targ.x), min(orig.x, targ.x), max(orig.x, targ.x));
	state.y = constrain(map(elapsed, 0, duration, orig.y, targ.y), min(orig.y, targ.y), max(orig.y, targ.y));

	tiltUpdate();

	// Serial.print("AFTER: (");
	// Serial.print(state.x);
	// Serial.print(", ");
	// Serial.print(state.y);
	// Serial.println(")");
	return (state.x == targ.x && state.y == targ.y);
}

int TiltController::tiltSet(TiltServoState state_in) {
	analogWrite(TILTSERVOXPIN, state_in.x);
	analogWrite(TILTSERVOYPIN, state_in.y);
}

void TiltController::center(int speed) {
	setTraj(TiltServoState(0, 0), speed);
}