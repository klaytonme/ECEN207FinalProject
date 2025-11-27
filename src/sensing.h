#ifndef SENSING_H
#define SENSING_H


#define JOYMAX 512


typedef struct LimitState {
	bool nw;
	bool ne;
	bool se;
};

typedef struct JoyState {
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
	LimitController(int, int, int);
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
	UIController(int, int, int, int, int, int);

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

	static UIController* instance;

	const int kJoyMax_ = 1024;


  private:
	const int kPinButton_;
	const int kPinJoyX_;
	const int kPinJoyY_;
	const int kPinJoyButton_;
	const int kPinCoin_;
	const int kPinWin_;


	volatile bool buttonFlag_;
	volatile bool joyButtonFlag_;

	JoyState state_;
	JoyState calib_;
};


#endif