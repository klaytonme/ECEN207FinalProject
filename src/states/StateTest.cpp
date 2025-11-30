#include <Arduino.h>

#include "Display.h"
#include "StateIdle.h"
#include "StateTest.h"
#include "actuation.h"
#include "context.h"
#include "sensing.h"



enum { INIT, TEST };
enum { X, Y };


const int			kTiltCalibMaxInc	  = 10;
const unsigned long kTiltCalibRefreshRate = 5000000 * kTiltCalibMaxInc / (kTiltMax - kTiltMin);


void TestState::enter() {
	ctx_->Lift_->stop();
	transitionTo(new TS_Disp());
}
void TestState::update() { substate_->update(); }
void TestState::exit() {
	if (substate_ != nullptr) substate_->exit();
}

void TestState::transitionTo(TS* substate) {
	if (substate_ != nullptr) {
		substate_->exit();
		delete substate_;
	}

	substate_ = substate;
	substate_->setContext(ctx_, this);
	substate_->setStateStart(t());
	substate_->enter();
}



void TS_Disp::enter() {
	stage_		= 0;
	loop_start_ = t();
}
void TS_Disp::update() {
	if (t() > loop_start_ + kLoopPeriod_) {
		loop_start_ = t();
		ctx_->Disp_->writeDisplay(1 << stage_);
		Serial.println(stage_);
		stage_++;
	}

	if (stage_ >= 32) parent_->transitionTo(new TS_Joy());
}

void TS_Coin::enter() {
	Serial.println("Testing Coin");
	ctx_->UI_->resetButtonInterrupt();
}
void TS_Coin::update() {
	if (ctx_->UI_->buttonWasPressed()) parent_->transitionTo(new TS_Win());

	if (ctx_->UI_->coinIsThere()) ctx_->Disp_->writeDisplay(0xFFFFFFFF);
	else ctx_->Disp_->writeDisplay(0x00000000);
}

void TS_Win::enter() {
	Serial.println("Testing Win");
	ctx_->UI_->resetButtonInterrupt();
}
void TS_Win::update() {
	if (ctx_->UI_->buttonWasPressed()) parent_->transitionTo(new TS_Joy());

	if (ctx_->UI_->winIsThere()) ctx_->Disp_->writeDisplay(0xFFFFFFFF);
	else ctx_->Disp_->writeDisplay(0x00000000);
}

void TS_Joy::enter() {
	dir_ = X;

	ctx_->UI_->joyCalib();
	JoyState joy = ctx_->UI_->readJoy();

	Serial.print("Calibrated Joystick: ");
	Serial.print(joy.x);
	Serial.print(", ");
	Serial.println(joy.y);
	Serial.println("Running Joystick Test");

	ctx_->UI_->resetButtonInterrupt();
	ctx_->UI_->resetJoyButtonInterrupt();
}
void TS_Joy::update() {
	if (ctx_->UI_->buttonWasPressed()) parent_->transitionTo(new TS_Lift());
	if (ctx_->UI_->joyButtonWasPressed()) dir_ = dir_ == X ? Y : X;

	JoyState state	  = ctx_->UI_->readJoy();
	int		 joyValue = dir_ == X ? state.x : state.y;

	int displayVal = testJoy(joyValue);
	ctx_->Disp_->writeDisplay(displayVal);
}
int TS_Joy::testJoy(int joyValue) {
	if (joyValue < -(7 * kJoyMax) / 9) return BARLEFT << (8 * 3);
	if (joyValue < -(5 * kJoyMax) / 9) return BARRIGHT << (8 * 3);
	if (joyValue < -(3 * kJoyMax) / 9) return BARLEFT << (8 * 2);
	if (joyValue < -(1 * kJoyMax) / 9) return BARRIGHT << (8 * 2);
	if (joyValue < (1 * kJoyMax) / 9) return DOT << (8 * 2);
	if (joyValue < (3 * kJoyMax) / 9) return BARLEFT << (8 * 1);
	if (joyValue < (5 * kJoyMax) / 9) return BARRIGHT << (8 * 1);
	if (joyValue < (7 * kJoyMax) / 9) return BARLEFT << (8 * 0);
	else return BARRIGHT << (8 * 0);
}


void TS_Lift::enter() {
	Serial.println("Running Lift Test");
	ctx_->UI_->resetButtonInterrupt();
	loop_start_ = t();
}
void TS_Lift::update() {
	if (ctx_->UI_->buttonWasPressed()) parent_->transitionTo(new TS_TiltCenterInit());

	if (t() > loop_start_ + kServoRefreshRate) {
		loop_start_ = t();

		JoyState joy   = ctx_->UI_->readJoy();
		int		 liftV = LIFT_DIR_STOP;
		if (joy.y < -kJoyMax / 2) liftV = LIFT_DIR_DOWN;
		if (joy.y > kJoyMax / 2) liftV = LIFT_DIR_UP;

		if (liftV >= 0) ctx_->Lift_->write(liftV);
		else {
			LimitState limits = ctx_->Limit_->readLimits();
			ctx_->Lift_->write(liftV, limits);
		}
	}
}
void TS_Lift::exit() { ctx_->Lift_->stop(); }

void TS_TiltCenterInit::enter() {
	Serial.println("Running Tilt Center Init");

	ctx_->Tilt_->toCenter(TILTSPEED_IMMEDIATE);
	loop_start_ = t();
	ctx_->UI_->resetButtonInterrupt();
}
void TS_TiltCenterInit::update() {
	if (ctx_->UI_->buttonWasPressed()) {
		parent_->transitionTo(new TS_TiltBoundsInit());
	}
	if (t() > loop_start_ + kTiltCalibRefreshRate) {
		loop_start_	 = t();
		JoyState joy = ctx_->UI_->readJoy();

		TiltServoState tilt = ctx_->Tilt_->getPosition();

		tilt.x += map(joy.x, -kJoyMax, kJoyMax, -kTiltCalibMaxInc, kTiltCalibMaxInc);
		tilt.y += map(joy.y, -kJoyMax, kJoyMax, -kTiltCalibMaxInc, kTiltCalibMaxInc);
		tilt.x = constrain(tilt.x, kTiltMin, kTiltMax);
		tilt.y = constrain(tilt.y, kTiltMin, kTiltMax);
		ctx_->Tilt_->setTraj(tilt, TILTSPEED_IMMEDIATE);
	}
}
void TS_TiltCenterInit::exit() { ctx_->Tilt_->updateCenter(); }
void TS_TiltBoundsInit::enter() {
	Serial.println("Running Tilt Bounds Init");

	ctx_->Tilt_->toCenter(TILTSPEED_IMMEDIATE);

	loop_start_ = t();
	ctx_->UI_->resetButtonInterrupt();
}
void TS_TiltBoundsInit::update() {
	if (ctx_->UI_->buttonWasPressed()) {
		parent_->transitionTo(new TS_Tilt());
	}

	if (t() > loop_start_ + kTiltCalibRefreshRate) {
		loop_start_ = t();

		JoyState	   joy	= ctx_->UI_->readJoy();
		TiltServoState tilt = ctx_->Tilt_->getPosition();

		int incX = map(joy.x, -kJoyMax, kJoyMax, -kTiltCalibMaxInc, kTiltCalibMaxInc);
		int incY = map(joy.y, -kJoyMax, kJoyMax, -kTiltCalibMaxInc, kTiltCalibMaxInc);

		tilt.x = constrain(tilt.x + incX, kTiltMin, kTiltMax);
		tilt.y = constrain(tilt.y + incY, kTiltMin, kTiltMax);

		ctx_->Tilt_->setTraj(tilt, TILTSPEED_IMMEDIATE);
	}
}
void TS_TiltBoundsInit::exit() {
	ctx_->Tilt_->updateBounds();
	ctx_->Tilt_->toCenter(TILTSPEED_IMMEDIATE);
}

void TS_Tilt::enter() {
	Serial.println("Running Tilt Test");

	last_read_ = t();

	joy_	 = new JoyState();
	center_	 = new TiltServoState();
	tiltMin_ = new TiltServoState();
	tiltMax_ = new TiltServoState();
	tilt_	 = new TiltServoState();

	*center_  = ctx_->Tilt_->getCenter();
	*tiltMin_ = ctx_->Tilt_->getMin();
	*tiltMax_ = ctx_->Tilt_->getMax();

	ctx_->UI_->resetButtonInterrupt();
}
void TS_Tilt::update() {
	if (ctx_->UI_->buttonWasPressed()) {
		ctx_->transitionTo(new IdleState());
	}

	if (t() > last_read_ + kServoRefreshRate) {
		last_read_ = t();

		*joy_ = ctx_->UI_->readJoy();

		tilt_->x = map(joy_->x, -kJoyMax, kJoyMax, tiltMin_->x, tiltMax_->x);
		tilt_->y = map(joy_->y, -kJoyMax, kJoyMax, tiltMin_->y, tiltMax_->y);
		tilt_->x = constrain(tilt_->x, tiltMin_->x, tiltMax_->x);
		tilt_->y = constrain(tilt_->y, tiltMin_->y, tiltMax_->y);

		ctx_->Tilt_->setTraj(*tilt_, TRAJ_DURATION, kServoRefreshRate);
	}

	ctx_->Tilt_->updateTraj(t() - last_read_);
}
void TS_Tilt::exit() { ctx_->Tilt_->toCenter(); }
