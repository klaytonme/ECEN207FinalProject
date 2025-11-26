#include <Arduino.h>

#include "sensing.h"

void buttonISR();
void joyButtonISR();

UIController* UIController::instance = nullptr;


LimitController::LimitController(int NELimitPin, int NWLimitPin, int SELimitPin)
	: kPinNW_(NWLimitPin), kPinNE_(NELimitPin), kPinSE_(SELimitPin) {}

void LimitController::initPins() {
	pinMode(kPinNW_, INPUT_PULLUP);
	pinMode(kPinNE_, INPUT_PULLUP);
	pinMode(kPinSE_, INPUT_PULLUP);
}

LimitState LimitController::readLimits() {
	state_.nw = digitalRead(kPinNW_);
	state_.ne = digitalRead(kPinNE_);
	state_.se = digitalRead(kPinSE_);

	return state_;
}

bool LimitController::isDown() {
	readLimits();

	return (!state_.nw && !state_.ne && !state_.se);
}



UIController::UIController(int ButtonPin, int JoyXPin, int JoyYPin, int JoyButtonPin, int CoinPin, int WinPin)
	: kPinButton_(ButtonPin), kPinJoyX_(JoyXPin), kPinJoyY_(JoyYPin), kPinJoyButton_(JoyButtonPin), kPinCoin_(CoinPin),
	  kPinWin_(WinPin) {}

void UIController::initPins() {
	instance = this;

	pinMode(kPinButton_, INPUT_PULLUP);
	pinMode(kPinJoyButton_, INPUT_PULLUP);

	attachInterrupt(digitalPinToInterrupt(kPinButton_), buttonISR, FALLING);
	attachInterrupt(digitalPinToInterrupt(kPinJoyButton_), joyButtonISR, FALLING);
}



JoyState UIController::readJoy() {
	JoyState state;
	state.x = analogRead(kPinJoyX_);
	state.y = analogRead(kPinJoyY_);

	return state;
}

bool UIController::buttonIsPressed() {
	return !digitalRead(kPinButton_);
}

bool UIController::joyButtonIsPressed() {
	return !digitalRead(kPinJoyButton_);
}

bool UIController::buttonWasPressed() {
	__disable_irq;
	bool tempButtonState = buttonFlag_;
	buttonFlag_			 = false;
	__enable_irq;

	return tempButtonState;
}

bool UIController::joyButtonWasPressed() {
	__disable_irq;
	bool tempButtonState = joyButtonFlag_;
	joyButtonFlag_		 = false;
	__enable_irq;

	return tempButtonState;
}

void UIController::buttonInterrupt() {
	buttonFlag_ = true;
}

void UIController::joyButtonInterrupt() {
	joyButtonFlag_ = true;
}


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