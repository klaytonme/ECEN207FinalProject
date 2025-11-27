#ifndef STATE_H
#define STATE_H

class Context;

class State {
  protected:
	Context*	  ctx_;
	unsigned long state_start_;

  public:
	virtual ~State() = default;

	void		  setContext(Context*);
	void		  setStateStart(unsigned long);
	unsigned long t(void);

	virtual void enter(void)  = 0;
	virtual void update(void) = 0;
	virtual void exit(void)	  = 0;
};

#endif