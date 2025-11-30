#include <Arduino.h>

#include "Display.h"
#include "StateIdle.h"
#include "StateInit.h"
#include "StateTest.h"
#include "actuation.h"
#include "context.h"
#include "sensing.h"


enum { INIT, TEST };
enum { X, Y };

const int			kTiltCalibMaxInc	  = 10;
const unsigned long kTiltCalibRefreshRate = 5000000 * kTiltCalibMaxInc / (kTiltMax - kTiltMin);


void InitState::enter() {
	ctx_->Lift_->stop();
	if (ctx_->UI_->buttonIsPressed()) ctx_->transitionTo(new TestState());
	else transitionTo(new IS_Disp());
}
void InitState::update() { substate_->update(); }
void InitState::exit() {
	if (substate_ != nullptr) substate_->exit();
}

void InitState::transitionTo(IS* substate) {
	if (substate_ != nullptr) {
		substate_->exit();
		delete substate_;
	}

	substate_ = substate;
	substate_->setContext(ctx_, this);
	substate_->setStateStart(t());
	substate_->enter();
}



void IS_Disp::enter() {
	Serial.println("Running Display Test");
	stage_		= 0;
	loop_start_ = t();
}
void IS_Disp::update() {

	if (t() > loop_start_ + kLoopPeriod_) {
		loop_start_ = t();
		ctx_->Disp_->writeDisplay(1 << stage_);
		stage_++;
	}

	if (stage_ >= 32) {
		parent_->transitionTo(new IS_Joy());
	}
}

void IS_Joy::enter() {
	Serial.println("Calibrating Joystick");
	ctx_->UI_->joyCalib();
	parent_->transitionTo(new IS_LiftUp());
}

void IS_LiftUp::enter() {
	Serial.println("Up");
	ctx_->Lift_->write(LIFT_DIR_UP);
}
void IS_LiftUp::update() {
	if (t() >= state_start_ + kLiftUpTime) {
		parent_->transitionTo(new IS_LiftDown());
	}
}
void IS_LiftDown::enter() { Serial.println("Down"); }
void IS_LiftDown::update() {
	LimitState limits = ctx_->Limit_->readLimits();
	ctx_->Lift_->write(LIFT_DIR_DOWN, limits);

	if (ctx_->Limit_->isDown()) parent_->transitionTo(new IS_TiltCenter());
}
void IS_LiftDown::exit() { ctx_->Lift_->stop(); }

void IS_TiltCenter::enter() {
	Serial.println("Running Tilt Center Init");

	ctx_->Tilt_->toCenter(TILTSPEED_IMMEDIATE);
	loop_start_ = t();
	TO_start_	= t();
	TO_flag_	= false;
	ctx_->UI_->resetButtonInterrupt();
}
void IS_TiltCenter::update() {
	if (t() > TO_start_ + kTiltInitTO) {
		TO_flag_ = true;
		ctx_->transitionTo(new IdleState());
	}

	if (ctx_->UI_->buttonWasPressed()) {
		parent_->transitionTo(new IS_TiltBounds());
	}

	if (t() > loop_start_ + kTiltCalibRefreshRate) {
		loop_start_	 = t();
		JoyState joy = ctx_->UI_->readJoy();

		TiltServoState tilt = ctx_->Tilt_->getPosition();

		int incX = map(joy.x, -kJoyMax, kJoyMax, -kTiltCalibMaxInc, kTiltCalibMaxInc);
		int incY = map(joy.y, -kJoyMax, kJoyMax, -kTiltCalibMaxInc, kTiltCalibMaxInc);
		if (abs(incX) > 1 || abs(incY) > 1) TO_start_ = t();

		tilt.x = constrain(tilt.x + incX, kTiltMin, kTiltMax);
		tilt.y = constrain(tilt.y + incY, kTiltMin, kTiltMax);
		ctx_->Tilt_->setTraj(tilt, TILTSPEED_IMMEDIATE);
	}
}
void IS_TiltCenter::exit() {
	if (!TO_flag_) ctx_->Tilt_->updateCenter();
	else ctx_->Tilt_->toCenter(TILTSPEED_IMMEDIATE);
}
void IS_TiltBounds::enter() {
	Serial.println("Running Tilt Bounds Init");

	ctx_->Tilt_->toCenter(TILTSPEED_IMMEDIATE);

	loop_start_ = t();
	TO_start_	= t();
	TO_flag_	= false;
	ctx_->UI_->resetButtonInterrupt();
}
void IS_TiltBounds::update() {
	if (t() > TO_start_ + kTiltInitTO) {
		TO_flag_ = true;
		ctx_->transitionTo(new IdleState());
	}

	if (ctx_->UI_->buttonWasPressed()) {
		ctx_->transitionTo(new IdleState());
	}

	if (t() > loop_start_ + kTiltCalibRefreshRate) {
		loop_start_ = t();

		JoyState	   joy	= ctx_->UI_->readJoy();
		TiltServoState tilt = ctx_->Tilt_->getPosition();

		int incX = map(joy.x, -kJoyMax, kJoyMax, -kTiltCalibMaxInc, kTiltCalibMaxInc);
		int incY = map(joy.y, -kJoyMax, kJoyMax, -kTiltCalibMaxInc, kTiltCalibMaxInc);
		if (abs(incX) > 0 || abs(incY) > 0) TO_start_ = t();

		tilt.x = constrain(tilt.x + incX, kTiltMin, kTiltMax);
		tilt.y = constrain(tilt.y + incY, kTiltMin, kTiltMax);

		ctx_->Tilt_->setTraj(tilt, TILTSPEED_IMMEDIATE);
	}
}
void IS_TiltBounds::exit() {
	if (!TO_flag_) ctx_->Tilt_->updateBounds();
	ctx_->Tilt_->toCenter(TILTSPEED_IMMEDIATE);
}
