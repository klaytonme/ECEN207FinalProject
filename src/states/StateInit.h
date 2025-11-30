#ifndef STATEINIT_H
#define STATEINIT_H

#include "state.h"


extern const unsigned long kDisplayInitTO;

class IS;

class InitState : public State {
  public:
	void enter() override;
	void update() override;
	void exit() override;

	void transitionTo(IS*);

  private:
	IS* substate_;
};


class IS : public State {
  public:
	virtual ~IS() {}

	virtual void enter() override {};
	virtual void update() override {};
	virtual void exit() override {};

	void setContext(Context* ctx, InitState* parent) {
		ctx_	= ctx;
		parent_ = parent;
	};

  protected:
	InitState* parent_;
	int		   loop_start_;
};



class IS_Disp : public IS {
  public:
	void enter() override;
	void update() override;

  private:
	const int kLoopPeriod_ = kDisplayInitTO;
	int		  stage_;
};

class IS_Joy : public IS {
	void enter() override;
};

class IS_LiftUp : public IS {
	void enter() override;
	void update() override;
};
class IS_LiftDown : public IS {
	void enter() override;
	void update() override;
	void exit() override;
};

class IS_TiltCenter : public IS {
  public:
	void update() override;
	void enter() override;
	void exit() override;

  private:
	unsigned long TO_start_;
	bool		  TO_flag_;
};
class IS_TiltBounds : public IS {
  public:
	void update() override;
	void enter() override;
	void exit() override;

  private:
	unsigned long TO_start_;
	bool		  TO_flag_;
};

#endif