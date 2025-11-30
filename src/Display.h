#ifndef DISPLAY_H
#define DISPLAY_H

#define BARLEFT	 0xF0
#define BARRIGHT 0x0F
#define DOT		 0x01

const unsigned long kDisplayInitTO = 50000;

class DisplayController {
  public:
	DisplayController(int, int, int);
	void initPins();
	void writeDisplay(int);

  private:
	void writeChar(unsigned char);

	const int kPinLatch_;
	const int kPinClock_;
	const int kPinData_;
};

#endif