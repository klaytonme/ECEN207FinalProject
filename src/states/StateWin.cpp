#include <Arduino.h>

#include "StateWin.h"

void WinState::enter() { Serial.println("You Won!!"); }
void WinState::update() {}
void WinState::exit() {}