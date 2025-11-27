#include <Arduino.h>

#include "Display.h"
#include "actuation.h"
#include "context.h"
#include "sensing.h"
#include "state.h"

Context::Context(State* state, int XServoPin, int YServoPin, int NWServoPin, int NEServoPin, int SEServoPin,
				 int NWLimitPin, int NELimitPin, int SELimitPin, int ButtonPin, int JoyXPin, int JoyYPin,
				 int JoyButtonPin, int CoinPin, int WinPin, int DispLatchPin, int DispClockPin, int DispDataPin) {

	Tilt_  = new TiltController(XServoPin, YServoPin);
	Lift_  = new LiftController(NWServoPin, NEServoPin, SEServoPin);
	Limit_ = new LimitController(NWLimitPin, NELimitPin, SELimitPin);
	UI_	   = new UIController(ButtonPin, JoyXPin, JoyYPin, JoyButtonPin, CoinPin, WinPin);
	Disp_  = new DisplayController(DispLatchPin, DispClockPin, DispDataPin);

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
}

unsigned long Context::t() { return current_time_; }