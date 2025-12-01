#include <Arduino.h>

#include "sensing.h"

void buttonISR();
void joyButtonISR();

UIController* UIController::instance = nullptr;


LimitController::LimitController(const uint8_t* pins) : kPinNW_(pins[0]), kPinNE_(pins[1]), kPinSE_(pins[2]) {}

void LimitController::initPins() {
	pinMode(kPinNW_, INPUT_PULLUP);
	pinMode(kPinNE_, INPUT_PULLUP);
	pinMode(kPinSE_, INPUT_PULLUP);
}

LimitState LimitController::readLimits() {
	state_.nw = !digitalRead(kPinNW_);
	state_.ne = !digitalRead(kPinNE_);
	state_.se = !digitalRead(kPinSE_);

	return state_;
}

bool LimitController::isDown() {
	readLimits();

	return (state_.nw && state_.ne && state_.se);
}



UIController::UIController(const uint8_t* pins)
	: kPinButton_(pins[0]), kPinJoyX_(pins[1]), kPinJoyY_(pins[2]), kPinJoyButton_(pins[3]), kPinCoin_(pins[4]),
	  kPinWin_(pins[5]) {
	state_ = JoyState(0, 0);
	calib_ = JoyState(0, 0);

	buttonFlagTime_	   = 0;
	joyButtonFlagTime_ = 0;
}

void UIController::initPins() {
	instance = this;

	pinMode(kPinButton_, INPUT_PULLUP);
	pinMode(kPinJoyButton_, INPUT_PULLUP);

	attachInterrupt(digitalPinToInterrupt(kPinButton_), buttonISR, FALLING);
	attachInterrupt(digitalPinToInterrupt(kPinJoyButton_), joyButtonISR, FALLING);

	pinMode(kPinCoin_, INPUT_PULLUP);
	pinMode(kPinWin_, INPUT_PULLUP);
}

void UIController::joyCalib() {
	state_.x = analogRead(kPinJoyX_);
	state_.y = analogRead(kPinJoyY_);
	calib_	 = state_;
}



JoyState UIController::readJoy() {
	state_.x = -(analogRead(kPinJoyX_) - calib_.x);
	state_.y = -(analogRead(kPinJoyY_) - calib_.y);

	return state_;
}

bool UIController::buttonIsPressed() { return !digitalRead(kPinButton_); }
bool UIController::joyButtonIsPressed() { return !digitalRead(kPinJoyButton_); }

bool UIController::buttonWasPressed() {
	noInterrupts();
	bool tempButtonState = buttonFlag_;
	buttonFlag_			 = false;
	interrupts();

	return tempButtonState;
}

bool UIController::joyButtonWasPressed() {
	noInterrupts();
	bool tempButtonState = joyButtonFlag_;
	joyButtonFlag_		 = false;
	interrupts();

	return tempButtonState;
}

void UIController::buttonInterrupt() {
	unsigned long time = micros();
	if (time < buttonFlagTime_ + kButtonInterruptTO_) return;

	buttonFlag_		= true;
	buttonFlagTime_ = time;
}
void UIController::joyButtonInterrupt() {
	unsigned long time = micros();
	if (time < joyButtonFlagTime_ + kButtonInterruptTO_) return;

	joyButtonFlag_	   = true;
	joyButtonFlagTime_ = time;
}

void UIController::resetButtonInterrupt() { buttonFlag_ = false; }
void UIController::resetJoyButtonInterrupt() { joyButtonFlag_ = false; }


void buttonISR() {
	if (UIController::instance) {
		UIController::instance->buttonInterrupt();
	}
}

void joyButtonISR() {
	if (UIController::instance) {
		UIController::instance->joyButtonInterrupt();
	}
}

bool UIController::coinIsThere() { return !digitalRead(kPinCoin_); }

bool UIController::winIsThere() { return !digitalRead(kPinWin_); }