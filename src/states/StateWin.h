#ifndef STATEWIN_H
#define STATEWIN_H

#include "state.h"

class WinState : public State {
  public:
	void enter() override;
	void update() override;
	void exit() override;
};


#endif