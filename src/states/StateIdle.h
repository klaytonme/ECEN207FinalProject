#ifndef STATEIDLE_H
#define STATEIDLE_H

#include "state.h"


class IdleState : public State {
  public:
	void enter() override;
	void update() override;
	void exit() override;
};


#endif