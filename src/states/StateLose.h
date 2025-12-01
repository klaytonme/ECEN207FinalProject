#ifndef STATELOSE_H
#define STATELOSE_H

#include "state.h"

class LoseState : public State {
  public:
	void enter() override;
	void update() override;
	void exit() override;
};


#endif