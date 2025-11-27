#include <Arduino.h>

#include "Display.h"
#include "StateIdle.h"
#include "StateInit.h"
#include "actuation.h"
#include "context.h"
#include "sensing.h"



void InitState::enter() { transitionTo(new IS_Button()); }


void InitState::update() { substate_->update(); }

// void InitState::update() {

// 	if ((init_ & InitFlags::N) == InitFlags::NONE) {
// 		if (!ctx_->Tilt_.updateTraj(ctx_->current_time_ - state_start_))
// 			return;

// 		Serial.println("South");
// 		init_ |= InitFlags::N;

// 		state_start_ = ctx_->current_time_;
// 		ctx_->Tilt_.setTraj(TiltServoState(0, -tilt_max), TILTSPEED_SLOW);
// 	}
// 	if ((init_ & InitFlags::S) == InitFlags::NONE) {
// 		if (!ctx_->Tilt_.updateTraj(ctx_->current_time_ - state_start_))
// 			return;

// 		Serial.println("Center");
// 		init_ |= InitFlags::S;
// 		init_ &= ~InitFlags::CENTER;

// 		state_start_ = ctx_->current_time_;
// 		ctx_->Tilt_.center(TILTSPEED_SLOW);
// 	}
// 	if ((init_ & InitFlags::W) == InitFlags::NONE) {
// 		if (!ctx_->Tilt_.updateTraj(ctx_->current_time_ - state_start_))
// 			return;

// 		Serial.println("East");
// 		init_ |= InitFlags::W;

// 		state_start_ = ctx_->current_time_;
// 		ctx_->Tilt_.setTraj(TiltServoState(tilt_max, 0), TILTSPEED_SLOW);
// 	}
// 	if ((init_ & InitFlags::E) == InitFlags::NONE) {
// 		if (!ctx_->Tilt_.updateTraj(ctx_->current_time_ - state_start_))
// 			return;

// 		Serial.println("Center");
// 		init_ |= InitFlags::E;

// 		state_start_ = ctx_->current_time_;
// 		ctx_->Tilt_.center(TILTSPEED_SLOW);
// 	}
// 	if ((init_ & InitFlags::CENTER2) == InitFlags::NONE) {
// 		if (!ctx_->Tilt_.updateTraj(ctx_->current_time_ - state_start_))
// 			return;

// 		Serial.println("North Fast");
// 		init_ |= InitFlags::CENTER2;

// 		state_start_ = ctx_->current_time_;
// 		ctx_->Tilt_.setTraj(TiltServoState(0, tilt_max), TILTSPEED_IMMEDIATE);
// 	}
// 	if ((init_ & InitFlags::NFAST) == InitFlags::NONE) {
// 		if (ctx_->current_time_ < state_start_ + tilt_speed_immediate_TO)
// 			return;

// 		Serial.println("South Fast");
// 		init_ |= InitFlags::NFAST;

// 		state_start_ = ctx_->current_time_;
// 		ctx_->Tilt_.setTraj(TiltServoState(0, -tilt_max), TILTSPEED_IMMEDIATE);
// 	}
// 	if ((init_ & InitFlags::SFAST) == InitFlags::NONE) {
// 		if (ctx_->current_time_ < state_start_ + tilt_speed_immediate_TO)
// 			return;

// 		Serial.println("Center");
// 		init_ |= InitFlags::SFAST;

// 		state_start_ = ctx_->current_time_;
// 		ctx_->Tilt_.center(TILTSPEED_IMMEDIATE);
// 	}
// 	if ((init_ & InitFlags::CENTER3) == InitFlags::NONE) {
// 		if (ctx_->current_time_ < state_start_ + tilt_speed_immediate_TO)
// 			return;

// 		Serial.println("West Fast");
// 		init_ |= InitFlags::CENTER3;

// 		state_start_ = ctx_->current_time_;
// 		ctx_->Tilt_.setTraj(TiltServoState(-tilt_max, 0), TILTSPEED_IMMEDIATE);
// 	}
// 	if ((init_ & InitFlags::WFAST) == InitFlags::NONE) {
// 		if (ctx_->current_time_ < state_start_ + tilt_speed_immediate_TO)
// 			return;

// 		Serial.println("East Fast");
// 		init_ |= InitFlags::WFAST;

// 		state_start_ = ctx_->current_time_;
// 		ctx_->Tilt_.setTraj(TiltServoState(tilt_max, 0), TILTSPEED_IMMEDIATE);
// 	}
// 	if ((init_ & InitFlags::EFAST) == InitFlags::NONE) {
// 		if (ctx_->current_time_ < state_start_ + tilt_speed_immediate_TO)
// 			return;

// 		Serial.println("Center");
// 		init_ |= InitFlags::EFAST;

// 		state_start_ = ctx_->current_time_;
// 		ctx_->Tilt_.center(TILTSPEED_IMMEDIATE);
// 	}
// 	if ((init_ & InitFlags::CENTER4) == InitFlags::NONE) {
// 		if (ctx_->current_time_ < state_start_ + tilt_speed_immediate_TO)
// 			return;

// 		Serial.println("Display");
// 		init_ |= InitFlags::CENTER4;
// 		state_start_ = ctx_->current_time_;
// 	}
// 	if ((init_ & InitFlags::DISP) == InitFlags::NONE) {
// 		Serial.println("Button");
// 		init_ |= InitFlags::DISP;
// 	}
// 	if ((init_ & InitFlags::BUTTON) == InitFlags::NONE) {
// 		init_ |= InitFlags::BUTTON;
// 	}

// 	ctx_->transitionTo(new IdleState());
// }


void InitState::exit() {}

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


void IS_Button::enter() {
	int testMode = ctx_->UI_->buttonIsPressed() ? TEST : INIT;
	parent_->transitionTo(new IS_Disp(testMode));
}

void IS_Disp::enter() {
	stage_		= 0;
	loop_start_ = t();
}
void IS_Disp::update() {

	if (t() > loop_start_ + kLoopPeriod_) {
		loop_start_ = t();
		ctx_->Disp_->writeDisplay(1 << stage_);
		Serial.println(stage_);
		stage_++;
	}

	if (stage_ >= 32) {
		if (kTest_ == TEST) parent_->transitionTo(new IS_JoyTest());
		else parent_->transitionTo(new IS_JoyInit());
	}
}

void IS_JoyInit::enter() {
	ctx_->UI_->joyCalib();
	Serial.println("Calibrated Joystick");
	parent_->transitionTo(new IS_LiftInitUp());
}
void IS_JoyTest::enter() {
	ctx_->UI_->joyCalib();
	dir_ = X;
	ctx_->UI_->resetButtonInterrupt();
	ctx_->UI_->resetJoyButtonInterrupt();

	Serial.println("Calibrated Joystick");
}
void IS_JoyTest::update() {
	if (ctx_->UI_->buttonWasPressed()) parent_->transitionTo(new IS_LiftTest());
	if (ctx_->UI_->joyButtonWasPressed()) dir_ = dir_ == X ? Y : X;

	JoyState state	  = ctx_->UI_->readJoy();
	int		 joyValue = dir_ == X ? state.x : state.y;

	int displayVal = testJoy(joyValue);
	ctx_->Disp_->writeDisplay(displayVal);
}
int IS_JoyTest::testJoy(int joyValue) {
	int max = ctx_->UI_->kJoyMax_;
	if (joyValue < -(7 * max) / (9 * 2)) return BARLEFT << (8 * 3);
	if (joyValue < -(5 * max) / (9 * 2)) return BARRIGHT << (8 * 3);
	if (joyValue < -(3 * max) / (9 * 2)) return BARLEFT << (8 * 2);
	if (joyValue < -(1 * max) / (9 * 2)) return BARRIGHT << (8 * 2);
	if (joyValue < (1 * max) / (9 * 2)) return DOT << (8 * 2);
	if (joyValue < (3 * max) / (9 * 2)) return BARLEFT << (8 * 1);
	if (joyValue < (5 * max) / (9 * 2)) return BARRIGHT << (8 * 1);
	if (joyValue < (7 * max) / (9 * 2)) return BARLEFT << (8 * 0);
	else return BARRIGHT << (8 * 0);
}

void IS_LiftInitUp::enter() {
	Serial.println("Up");
	ctx_->Lift_->raise();
}
void IS_LiftInitUp::update() {
	if (t() >= state_start_ + lift_up_time) {
		parent_->transitionTo(new IS_LiftInitDown());
	}
}
void IS_LiftInitDown::enter() { Serial.println("Down"); }
void IS_LiftInitDown::update() {
	LimitState limits = ctx_->Limit_->readLimits();
	ctx_->Lift_->lower(limits);

	if (ctx_->Limit_->isDown()) parent_->transitionTo(new IS_TiltInit());
}
void IS_LiftTest::update() { Serial.println("Running Lift Test"); }

void IS_TiltInit::update() { Serial.println("Running Tilt Init"); }