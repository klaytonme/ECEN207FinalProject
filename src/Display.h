#ifndef Simple5641_h
#define Simple5641_h

#include <Arduino.h>

const unsigned long kDisplayInitTO = 50000;

typedef struct {
	char	letter;
	uint8_t code;
} symbol;

const symbol segmentCodes[] = {{'?', B00000000}, /* ░ */
							   {'.', B00000001}, /* . */
							   {'-', B00000010}, /* - */
							   {'|', B00001100}, /* | */
							   // Numbers
							   {'0', B11111100}, /* 0 */
							   {'1', B01100000}, /* 1 */
							   {'2', B11011010}, /* 2 */
							   {'3', B11110010}, /* 3 */
							   {'4', B01100110}, /* 4 */
							   {'5', B10110110}, /* 5 */
							   {'6', B10111110}, /* 6 */
							   {'7', B11100000}, /* 7 */
							   {'8', B11111110}, /* 8 */
							   {'9', B11110110}, /* 9 */
							   // Letters
							   {'A', B11101110}, /* A */
							   {'B', B00111110}, /* B */
							   {'C', B10011100}, /* C */
							   {'D', B01111010}, /* D */
							   {'E', B10011110}, /* E */
							   {'F', B10001110}, /* F */
							   {'G', B10111100} /* G */};

class DisplayController {
  public:
	DisplayController(const uint8_t* segmentPins, const uint8_t* digitSelectionPins);

	void initPins();

	void selectAllDigits();
	void selectDigit(int digit);

	void clean();

	// Base output
	void shortDisplayCode(uint8_t code);
	void shortDisplayCode(uint8_t code, boolean dot);

	void shortDisplayNumber(int n);
	void shortDisplayNumber(int n, boolean dot);

	void shortDisplayLetter(char c);
	void shortDisplayLetter(char c, boolean dot);

	// Simplified
	void shortDisplay(uint8_t code);
	void shortDisplay(uint8_t code, boolean dot);

	void shortDisplay(int n);
	void shortDisplay(int n, boolean dot);

	void shortDisplay(char c);
	void shortDisplay(char c, boolean dot);

	// Cycles
	void cycle(long digitDisplayTime, uint8_t codes[4]);
	void cycle(long digitDisplayTime, uint8_t codes[4], int dot);

	void cycle(long digitDisplayTime, int number);
	void cycle(long digitDisplayTime, int number, int dot);

	void cycle(long digitDisplayTime, char* word);
	void cycle(long digitDisplayTime, char* word, int dot);

	// Animations
	void zeroAnimation(long cycleDelay, long cycles);
	void zeroAnimation();

	// Display
	void Display(long cyclesAmount, long cycleDelay, long digitDisplayTime, uint8_t codes[4], int dot);
	void Display(long cyclesAmount, long cycleDelay, long digitDisplayTime, uint8_t codes[4]);
	void Display(int seconds, uint8_t codes[4], int dot);
	void Display(int seconds, uint8_t codes[4]);

	void Display(long cyclesAmount, long cycleDelay, long digitDisplayTime, char* word, int dot);
	void Display(long cyclesAmount, long cycleDelay, long digitDisplayTime, char* word);
	void Display(int seconds, char* word, int dot);
	void Display(int seconds, char* word);

	void Display(long cyclesAmount, long cycleDelay, long digitDisplayTime, int n, int dot);
	void Display(long cyclesAmount, long cycleDelay, long digitDisplayTime, int n);
	void Display(int seconds, int n, int dot);
	void Display(int seconds, int n);

	// Write/Update
	void write(uint32_t codes, unsigned long time);
	void write(const char* word, unsigned long time);
	void write(int num, unsigned long time);
	void update(unsigned long time);

  private:
	uint8_t getCode(char c);
	uint8_t getCode(int c);

	char* formatNumber(int num);

	int decimalPlaces(float number);
	int lastLeadingZeroIndex(const char* str);

	uint8_t _segmentPins[8];
	uint8_t _digitSelectionPins[4];

	uint8_t		  code_[4];
	unsigned long codeStart_;
};

#endif
