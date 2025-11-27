#ifndef CONTEXT_H
#define CONTEXT_H

class State;
class TiltController;
class LiftController;
class LimitController;
class UIController;
class DisplayController;

class Context {
  private:
	State* state_;


  public:
	Context(State*, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
	~Context() = default;

	void		  transitionTo(State*);
	void		  update(void);
	unsigned long t(void);


	TiltController*	   Tilt_;
	LiftController*	   Lift_;
	LimitController*   Limit_;
	UIController*	   UI_;
	DisplayController* Disp_;

	unsigned long current_time_;
};

#endif