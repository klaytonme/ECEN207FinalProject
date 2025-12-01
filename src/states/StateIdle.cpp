#include <Arduino.h>

#include "StateIdle.h"
#include "StatePlaying.h"
#include "context.h"
#include "sensing.h"

void IdleState::enter() { Serial.println("Entering Idle State"); }

void IdleState::update() {
	if (ctx_->UI_->coinIsThere()) ctx_->transitionTo(new PlayingState());
}

void IdleState::exit() {}