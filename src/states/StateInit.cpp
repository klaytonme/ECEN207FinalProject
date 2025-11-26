#include <Arduino.h>

#include "StateIdle.h"
#include "StateInit.h"
#include "actuation.h"
#include "context.h"
#include "sensing.h"



void InitState::enter() {
	state_start_ = ctx_->current_time_;

	transitionTo(new InitSubstateButton());
}


void InitState::update() {
	substate_->update();
}

// void InitState::update() {

// 	if ((init_ & InitFlags::UP) == InitFlags::NONE) {
// 		ctx_->Lift_.raise();

// 		if (ctx_->current_time_ < state_start_ + lift_up_time)
// 			return;
// 		Serial.println("Down");
// 		init_ |= InitFlags::UP;
// 	}
// 	if ((init_ & InitFlags::DOWN) == InitFlags::NONE) {
// 		LimitState limits = ctx_->Limit_.readLimits();
// 		ctx_->Lift_.lower(limits);

// 		if (!ctx_->Limit_.isDown())
// 			return;

// 		Serial.println("North");
// 		init_ |= InitFlags::DOWN;

// 		state_start_ = ctx_->current_time_;
// 		ctx_->Tilt_.setTraj(TiltServoState(0, tilt_max), TILTSPEED_SLOW);
// 	}
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

void InitState::transitionTo(InitSubState* substate) {
	if (substate_ != nullptr) {
		substate_->exit();
		delete substate_;
	}

	substate_ = substate;
	substate_->setContext(ctx_, this);
	substate_->enter();
}


void InitSubstateButton::enter() {
	if (ctx_->UI_.buttonIsPressed())
		parent_->transitionTo(new InitSubstateTestLift());

	else
		parent_->transitionTo(new InitSubstateInitLift());
}


void InitSubstateTestLift::update() {
	Serial.println("Running Test");
}

void InitSubstateInitLift::update() {
	Serial.println("Running Init");
}