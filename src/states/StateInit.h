#ifndef STATEINIT_H
#define STATEINIT_H

#include "state.h"


const int displayInitTO = 50000;

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

  public:
	InitState* parent_;
};


enum { INIT, TEST };
enum { X, Y };

class IS_Button : public IS {
	void enter();
};

class IS_Disp : public IS {
  public:
	IS_Disp(int test) : kTest_(test) {}
	void enter();
	void update();

  private:
	const int kTest_;
	const int kLoopPeriod_ = displayInitTO;
	int		  loop_start_;
	int		  stage_;
};

class IS_JoyInit : public IS {
	void enter();
};
class IS_JoyTest : public IS {
  public:
	void enter();
	void update();

  private:
	int testJoy(int);

	bool dir_;
};

class IS_LiftTest : public IS {
	// void enter();
	void update();
};
class IS_LiftInitUp : public IS {
	void enter();
	void update();
};
class IS_LiftInitDown : public IS {
	void enter();
	void update();
};

class IS_TiltInit : public IS {
	void update();
};

#endif