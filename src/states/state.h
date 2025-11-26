#ifndef STATE_H
#define STATE_H

class Context;

class State {
  protected:
	Context*  ctx_;
	long long state_start_;

  public:
	virtual ~State() = default;

	void setContext(Context* ctx) {
		ctx_ = ctx;
	}

	virtual void enter(void)  = 0;
	virtual void update(void) = 0;
	virtual void exit(void)	  = 0;
};

#endif