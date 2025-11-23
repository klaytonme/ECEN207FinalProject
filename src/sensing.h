#ifndef SENSING_H
#define SENSING_H


#define JOYMAX 512


typedef struct {
	bool nw;
	bool ne;
	bool se;
} SwitchState;

typedef struct {
	int x;
	int y;
} JoyState;



int			initSensingPins(void);
SwitchState readLimits();
JoyState	readJoy();

#endif