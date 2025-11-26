#include <Arduino.h>

#include "actuation.h"
#include "sensing.h"


TiltController::TiltController(int XServoPin, int YServoPin) : PIN_X(XServoPin), PIN_Y(YServoPin) {
	orig_.x	 = 0;
	orig_.y	 = 0;
	state_.x = 0;
	state_.y = 0;
	targ_.x	 = 0;
	targ_.y	 = 0;
}

void TiltController::initPins() {
	pinMode(PIN_X, OUTPUT);
	pinMode(PIN_Y, OUTPUT);
}

int TiltController::tiltUpdate() {
	analogWrite(PIN_X, state_.x);
	analogWrite(PIN_Y, state_.y);
}

TiltServoState TiltController::getPosition() {
	return state_;
}

void TiltController::setTraj(TiltServoState targ, int speed) {
	if (speed == TILTSPEED_IMMEDIATE) {
		state_ = targ;
		tiltUpdate();
		targ_ = targ;
		orig_ = targ;
		return;
	}

	orig_ = state_;
	targ_ = targ;

	int dist = max(abs(orig_.x - targ_.x), abs(orig_.y - targ_.y));
	duration = dist * speed;
}

int TiltController::updateTraj(time_t elapsed) {

	state_.x = constrain(map(elapsed, 0, duration, orig_.x, targ_.x), min(orig_.x, targ_.x), max(orig_.x, targ_.x));
	state_.y = constrain(map(elapsed, 0, duration, orig_.y, targ_.y), min(orig_.y, targ_.y), max(orig_.y, targ_.y));

	tiltUpdate();

	return (state_.x == targ_.x && state_.y == targ_.y);
}

int TiltController::set(TiltServoState state_in) {
	analogWrite(PIN_X, state_in.x);
	analogWrite(PIN_Y, state_in.y);
}

void TiltController::center(int speed) {
	setTraj(TiltServoState(0, 0), speed);
}



LiftController::LiftController(int NWServoPin, int NEServoPin, int SEServoPin)
	: kPinNW_(NWServoPin), kPinNE_(NEServoPin), kPinSE_(SEServoPin) {}

void LiftController::initPins() {
	pinMode(kPinNW_, OUTPUT);
	pinMode(kPinNE_, OUTPUT);
	pinMode(kPinSE_, OUTPUT);
}

void LiftController::update() {
	analogWrite(kPinNW_, state_.nw);
	analogWrite(kPinNE_, state_.ne);
	analogWrite(kPinSE_, state_.se);
}

void LiftController::set(LiftServoState state_in) {
	state_ = state_in;
	update();
}

void LiftController::raise(int speed) {
	if (!speed)
		speed = lift_up_speed;

	state_ = LiftServoState(90 + speed, 90 + speed, 90 + speed);
	update();
}

void LiftController::lower(LimitState limits, int speed) {
	if (!speed)
		speed = lift_down_speed;


	state_ = LiftServoState(90 + (speed * limits.nw), 90 + (speed * limits.ne), 90 + (speed * limits.se));
	update();
}