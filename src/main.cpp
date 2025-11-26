#include <Arduino.h>

// #include "main.h"
#include "states/StateInit.h"
#include "states/context.h"


#define LIFTSERVONWPIN 9
#define LIFTSERVONEPIN 10
#define LIFTSERVOSEPIN 11
#define TILTSERVOXPIN  5
#define TILTSERVOYPIN  6

#define LIMITSWITCHNWPIN  0
#define LIMITSWITCHNEPIN  1
#define LIMITSWITCHSEPIN  2
#define BUTTONPIN		  3
#define JOYSTICKBUTTONPIN A0
#define JOYSTICKXPIN	  A1
#define JOYSTICKYPIN	  A2
#define COININTERRUPTPIN  12
#define WININTERRUPTPIN	  13


Context* ctx;


// ============ MAIN ============

void setup() {
	Serial.begin(9600);
	Serial.println("Ran!");

	ctx = new Context(new InitState(), TILTSERVOXPIN, TILTSERVOYPIN, LIFTSERVONWPIN, LIFTSERVONEPIN, LIFTSERVOSEPIN,
					  LIMITSWITCHNWPIN, LIMITSWITCHNEPIN, LIMITSWITCHSEPIN, BUTTONPIN, JOYSTICKXPIN, JOYSTICKYPIN,
					  JOYSTICKBUTTONPIN, COININTERRUPTPIN, WININTERRUPTPIN);
}

void loop() {
	ctx->update();
}