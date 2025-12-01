#ifndef SENSING_H
#define SENSING_H


const int kJoyMax = 512;


struct LimitState {
	bool nw;
	bool ne;
	bool se;
	LimitState() {
		nw = 0;
		ne = 0;
		se = 0;
	};
	LimitState(bool nw_in, bool ne_in, bool se_in) {
		nw = nw_in;
		ne = ne_in;
		se = se_in;
	};
};

struct JoyState {
	int x;
	int y;
	JoyState() {
		x = 0;
		y = 0;
	};
	JoyState(int x_in, int y_in) {
		x = x_in;
		y = y_in;
	};
};



class LimitController {
  public:
	LimitController(const uint8_t*);
	void	   initPins(void);
	LimitState readLimits(void);
	bool	   isDown(void);

  private:
	const int kPinNW_;
	const int kPinNE_;
	const int kPinSE_;

	LimitState state_;
};

class UIController {
  public:
	UIController(const uint8_t*);

	void initPins(void);
	void joyCalib(void);

	JoyState readJoy(void);
	bool	 buttonIsPressed(void);
	bool	 joyButtonIsPressed(void);

	bool buttonWasPressed(void);
	bool joyButtonWasPressed(void);
	void buttonInterrupt(void);
	void joyButtonInterrupt(void);
	void resetButtonInterrupt(void);
	void resetJoyButtonInterrupt(void);

	bool coinIsThere(void);
	bool winIsThere(void);

	static UIController* instance;


  private:
	const int kPinButton_;
	const int kPinJoyX_;
	const int kPinJoyY_;
	const int kPinJoyButton_;
	const int kPinCoin_;
	const int kPinWin_;

	const unsigned long kButtonInterruptTO_ = 500000;


	volatile bool		   buttonFlag_;
	volatile unsigned long buttonFlagTime_;
	volatile bool		   joyButtonFlag_;
	volatile unsigned long joyButtonFlagTime_;

	JoyState state_;
	JoyState calib_;
};


#endif