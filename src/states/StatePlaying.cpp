#include <Arduino.h>

#include "StateLose.h"
#include "StatePlaying.h"
#include "StateWin.h"
#include "actuation.h"
#include "context.h"
#include "sensing.h"

const unsigned long kPlayingTO = 10000000;

void PlayingState::enter() {
	Serial.println("Playing!");

	last_read_ = t();

	joy_	 = new JoyState();
	center_	 = new TiltServoState();
	tiltMin_ = new TiltServoState();
	tiltMax_ = new TiltServoState();
	tilt_	 = new TiltServoState();

	*center_  = ctx_->Tilt_->getCenter();
	*tiltMin_ = ctx_->Tilt_->getMin();
	*tiltMax_ = ctx_->Tilt_->getMax();

	ctx_->UI_->resetButtonInterrupt();
}
void PlayingState::update() {
	if (t() > state_start_ + kPlayingTO) {
		ctx_->transitionTo(new LoseState());
	}

	if (ctx_->UI_->winIsThere()) {
		ctx_->transitionTo(new WinState());
	}

	if (t() > last_read_ + kServoRefreshRate) {
		last_read_ = t();

		*joy_ = ctx_->UI_->readJoy();

		tilt_->x = map(joy_->x, -kJoyMax, kJoyMax, tiltMin_->x, tiltMax_->x);
		tilt_->y = map(joy_->y, -kJoyMax, kJoyMax, tiltMin_->y, tiltMax_->y);
		tilt_->x = constrain(tilt_->x, tiltMin_->x, tiltMax_->x);
		tilt_->y = constrain(tilt_->y, tiltMin_->y, tiltMax_->y);

		ctx_->Tilt_->setTraj(*tilt_, TRAJ_DURATION, kServoRefreshRate);
	}

	ctx_->Tilt_->updateTraj(t() - last_read_);
}
void PlayingState::exit() { ctx_->Tilt_->toCenter(); }
