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
SwitchState readLimits(void);
JoyState	readJoy(void);
bool		buttonIsPressed(void);
bool		buttonWasPressed(void);

#endif