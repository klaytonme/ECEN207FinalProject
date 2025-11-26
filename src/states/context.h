#ifndef CONTEXT_H
#define CONTEXT_H

#include "actuation.h"
#include "sensing.h"
#include "state.h"

class Context {
  private:
	State* state_;


  public:
	Context(State*, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
	~Context() = default;

	void transitionTo(State*);
	void update(void);


	TiltController	Tilt_;
	LiftController	Lift_;
	LimitController Limit_;
	UIController	UI_;

	time_t current_time_;
};

#endif