#ifndef STATETEST_H
#define STATETEST_H

#include "state.h"


extern const unsigned long kDisplayInitTO;

class TS;

class TestState : public State {
  public:
	void enter() override;
	void update() override;
	void exit() override;

	void transitionTo(TS*);

  private:
	TS* substate_;
};


class TS : public State {
  public:
	virtual ~TS() {}

	virtual void enter() override {};
	virtual void update() override {};
	virtual void exit() override {};

	void setContext(Context* ctx, TestState* parent) {
		ctx_	= ctx;
		parent_ = parent;
	};

  protected:
	TestState* parent_;
	int		   loop_start_;
};


class TS_Disp : public TS {
  public:
	void enter() override;
	void update() override;

  private:
	const int kLoopPeriod_ = kDisplayInitTO;
	int		  stage_;
};

class TS_Coin : public TS {
	void enter() override;
	void update() override;
};

class TS_Win : public TS {
	void enter() override;
	void update() override;
};

class TS_Joy : public TS {
  public:
	void enter() override;
	void update() override;

  private:
	int testJoy(int);

	bool dir_;
};

class TS_Lift : public TS {
	void enter() override;
	void update() override;
	void exit() override;
};

class JoyState;
class TiltServoState;

class TS_TiltCenterInit : public TS {
	void update() override;
	void enter() override;
	void exit() override;
};
class TS_TiltBoundsInit : public TS {
	void update() override;
	void enter() override;
	void exit() override;
};
class TS_Tilt : public TS {
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