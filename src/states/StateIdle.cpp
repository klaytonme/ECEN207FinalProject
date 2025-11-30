#include <Arduino.h>

#include "StateIdle.h"

void IdleState::enter() { Serial.println("Entering Idle State"); }

void IdleState::update() {}

void IdleState::exit() {}