#ifndef STATEPLAYING_H
#define STATEPLAYING_H

#include "state.h"

class JoyState;
class TiltServoState;

class PlayingState : public State {
  public:
	void enter() override;
	void update() override;
	void exit() override;

  private:
	unsigned long last_read_;
	unsigned long last_write_;

	JoyState*		joy_;
	TiltServoState* center_;
	TiltServoState* tilt_;
	TiltServoState* tiltMin_;
	TiltServoState* tiltMax_;
};


#endif