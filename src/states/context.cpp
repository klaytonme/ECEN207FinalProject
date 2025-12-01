#include <Arduino.h>

#include "Display.h"
#include "actuation.h"
#include "context.h"
#include "sensing.h"
#include "state.h"

Context::Context(State* state, const uint8_t* DisplaySegmentPins, const uint8_t* DisplayDigitPins,
				 const uint8_t* LiftServoPins, const uint8_t* TiltServoPins, const uint8_t* LimitSwitchPins,
				 const uint8_t* UIPins)
	: state_(nullptr) {

	Tilt_  = new TiltController(TiltServoPins);
	Lift_  = new LiftController(LiftServoPins);
	Limit_ = new LimitController(LimitSwitchPins);
	UI_	   = new UIController(UIPins);
	Disp_  = new DisplayController(DisplaySegmentPins, DisplayDigitPins);

	Serial.println("Initializing objects and configuring pins");

	Tilt_->initPins();
	Lift_->initPins();
	Limit_->initPins();
	UI_->initPins();
	Disp_->initPins();


	current_time_ = micros();
	transitionTo(state);
}


void Context::transitionTo(State* state) {
	if (state_ != nullptr) {
		state_->exit();
		delete state_;
	}

	state_ = state;
	state_->setContext(this);
	state_->setStateStart(current_time_);
	state_->enter();
}

void Context::update() {
	current_time_ = micros();
	state_->update();
	Disp_->update(current_time_);
}

unsigned long Context::t() { return current_time_; }