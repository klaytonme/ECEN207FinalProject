#include <Arduino.h>

#include "context.h"


Context::Context(State* state, int XServoPin, int YServoPin, int NWServoPin, int NEServoPin, int SEServoPin,
				 int NELimitPin, int NWLimitPin, int SELimitPin, int ButtonPin, int JoyXPin, int JoyYPin,
				 int JoyButtonPin, int CoinPin, int WinPin)
	: state_(nullptr), Tilt_(XServoPin, YServoPin), Lift_(NWServoPin, NEServoPin, SEServoPin),
	  Limit_(NELimitPin, NWLimitPin, SELimitPin), UI_(ButtonPin, JoyXPin, JoyYPin, JoyButtonPin, CoinPin, WinPin) {

	Tilt_.initPins();
	Lift_.initPins();
	Limit_.initPins();
	UI_.initPins();

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
	state_->enter();
}

void Context::update() {
	current_time_ = micros();
	state_->update();
}