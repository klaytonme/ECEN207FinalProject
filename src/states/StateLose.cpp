#include <Arduino.h>

#include "StateLose.h"

void LoseState::enter() { Serial.println("You Lost :/"); }
void LoseState::update() {}
void LoseState::exit() {}