#include <Arduino.h>

// #include "main.h"
#include "states/StateInit.h"
#include "states/context.h"

const uint8_t DisplaySegmentPins[] = {40, 38, 36, 34, 32, 30, 28, 26}; // a=t, b=tr, c=br, d=b, e=bl, f=tl, g=c, h=dot
const uint8_t DisplayDigitPins[]   = {53, 51, 49, 47};				   // D1, D2, D3, D4
const uint8_t LiftServoPins[]	   = {1, 2, 3};						   // NW, NE, SE
const uint8_t TiltServoPins[]	   = {4, 5};						   // X, Y
const uint8_t LimitSwitchPins[]	   = {6, 7, 8};						   // NW, NE, SE
const uint8_t UIPins[]			   = {9, 10, A0, A1, 11, 12};		   // Button, Joy Button, X in, Y in, Coin, Win

Context* ctx;


// ============ MAIN ============

void setup() {
	Serial.begin(9600);
	delay(500);
	Serial.println("Serial Setup on 9600");

	ctx = new Context(new InitState(), DisplaySegmentPins, DisplayDigitPins, LiftServoPins, TiltServoPins,
					  LimitSwitchPins, UIPins);
}

void loop() { ctx->update(); }