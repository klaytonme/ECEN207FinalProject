#include <Arduino.h>
#include <Servo.h>

#include "actuation.h"

Servo TiltServoX;
Servo TiltServoY;
Servo LiftServoNW;
Servo LiftServoNE;
Servo LiftServoSE;

// const int kLiftCenter = 1450;
// const int kLiftMin	  = 25;
// const int kLiftMax	  = 500;

const LiftServoState kLiftStop = LiftServoState(1470, 1460, 1470);
const LiftServoState kLiftUp   = LiftServoState(1372, 1545, 1412);
const LiftServoState kLiftDown = LiftServoState(1540, 1389, 1560);


TiltController::TiltController(const uint8_t* pins) : kPinX_(pins[0]), kPinY_(pins[1]) {
	orig_  = TiltServoState(0, 0);
	state_ = TiltServoState(0, 0);
	targ_  = TiltServoState(0, 0);

	center_ = TiltServoState(1500, 1500);
	min_	= TiltServoState(1200, 1200);
	max_	= TiltServoState(1800, 1800);
}

void TiltController::initPins() {
	TiltServoX.attach(kPinX_);
	TiltServoY.attach(kPinY_);
}

void TiltController::tiltUpdate() {
	TiltServoX.writeMicroseconds(state_.x);
	TiltServoY.writeMicroseconds(state_.y);
}

TiltServoState TiltController::getPosition() { return state_; }

void TiltController::setTraj(TiltServoState targ, int type, int value) {
	targ_ = targ;
	orig_ = targ;

	if ((type == TRAJ_SPEED && value == TILTSPEED_IMMEDIATE) || (type == TRAJ_DURATION && value == 0)) {
		state_ = targ;
		tiltUpdate();
		return;
	}

	if (type == TRAJ_SPEED) {
		int dist = max(abs(orig_.x - targ_.x), abs(orig_.y - targ_.y));
		duration = dist * value;
	} else if (type == TRAJ_DURATION) {
		duration = value;
	}
}

int TiltController::updateTraj(unsigned long elapsed) {

	state_.x = constrain(map(elapsed, 0, duration, orig_.x, targ_.x), min(orig_.x, targ_.x), max(orig_.x, targ_.x));
	state_.y = constrain(map(elapsed, 0, duration, orig_.y, targ_.y), min(orig_.y, targ_.y), max(orig_.y, targ_.y));

	tiltUpdate();

	return (state_.x == targ_.x && state_.y == targ_.y);
}

void TiltController::set(TiltServoState state) {
	state_ = state;
	tiltUpdate();
}

void TiltController::toCenter(int type, int value) { setTraj(center_, type, value); }

void TiltController::updateCenter() { center_ = state_; }

void TiltController::updateBounds() {
	Serial.print("Center: ");
	Serial.print(center_.x);
	Serial.print(", ");
	Serial.println(center_.y);
	Serial.print("State: ");
	Serial.print(state_.x);
	Serial.print(", ");
	Serial.println(state_.y);

	min_.x = center_.x - abs(state_.x - center_.x);
	min_.y = center_.y - abs(state_.y - center_.y);
	max_.x = center_.x + abs(state_.x - center_.x);
	max_.y = center_.y + abs(state_.y - center_.y);
}



LiftController::LiftController(const uint8_t* pins) : kPinNW_(pins[0]), kPinNE_(pins[1]), kPinSE_(pins[2]) {}

void LiftController::initPins() {
	LiftServoNW.attach(kPinNW_);
	LiftServoNE.attach(kPinNE_);
	LiftServoSE.attach(kPinSE_);
}

void LiftController::update() {
	LiftServoNW.writeMicroseconds(state_.nw);
	LiftServoNE.writeMicroseconds(state_.ne);
	LiftServoSE.writeMicroseconds(state_.se);
}

void LiftController::set(LiftServoState state_in) {
	state_ = state_in;
	update();
}

void LiftController::write(int dir, LimitState limits) {
	// int msNorm, msInv;
	// msNorm = v + (v >= 0 ? kLiftMin : -kLiftMin);

	// state_.nw = kLiftCenter + (msNorm * limits.nw);
	// state_.ne = kLiftCenter - (msNorm * limits.ne);
	// state_.se = kLiftCenter + (msNorm * limits.se);


	if (dir == LIFT_DIR_UP) state_ = kLiftUp;
	else if (dir == LIFT_DIR_STOP) state_ = kLiftStop;
	else {
		state_.nw = limits.nw ? kLiftStop.nw : kLiftDown.nw;
		state_.ne = limits.ne ? kLiftStop.ne : kLiftDown.ne;
		state_.se = limits.se ? kLiftStop.se : kLiftDown.se;
	}

	// Serial.print(state_.nw);
	// Serial.print(", ");
	// Serial.print(state_.ne);
	// Serial.print(", ");
	// Serial.println(state_.se);
	update();
}

void LiftController::stop() { write(LIFT_DIR_STOP); }