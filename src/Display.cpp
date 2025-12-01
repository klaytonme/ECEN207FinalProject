#include "Display.h"

// DisplayController constructor
DisplayController::DisplayController(const uint8_t* segmentPins, const uint8_t* digitSelectionPins) {
	memcpy(_segmentPins, segmentPins, sizeof(uint8_t) * 8);
	memcpy(_digitSelectionPins, digitSelectionPins, sizeof(uint8_t) * 4);
}

// Configure pins
void DisplayController::initPins() {
	for (size_t i = 0; i < sizeof(_segmentPins) / sizeof(_segmentPins[0]); i++)
		pinMode(_segmentPins[i], OUTPUT);
	for (size_t i = 0; i < sizeof(_digitSelectionPins) / sizeof(_segmentPins[0]); i++)
		pinMode(_digitSelectionPins[i], OUTPUT);

	for (size_t i = 0; i < sizeof(_segmentPins) / sizeof(_segmentPins[0]); i++)
		digitalWrite(_segmentPins[i], LOW);
	for (size_t i = 0; i < sizeof(_digitSelectionPins) / sizeof(_segmentPins[0]); i++)
		digitalWrite(_digitSelectionPins[i], LOW);
}

// Get code from character
uint8_t DisplayController::getCode(char c) {
	if (c >= 'a' && c <= 'z') c = toupper(c);
	for (size_t d = 0; d < sizeof(segmentCodes) / sizeof(segmentCodes[0]); d++) {
		if (segmentCodes[d].letter == c) return segmentCodes[d].code;
	}

	return segmentCodes[0].code;
}

// Get code from number
uint8_t DisplayController::getCode(int n) {
	n = abs(n);
	while (n >= 10)
		n /= 10;
	const char new_n = (char)(n + '0');

	for (size_t d = 0; d < sizeof(segmentCodes) / sizeof(segmentCodes[0]); d++) {
		if (segmentCodes[d].letter == new_n) return segmentCodes[d].code;
	}

	return segmentCodes[0].code;
}

// format number
char* DisplayController::formatNumber(int num) {
	static char out[5];
	char		temp[6];
	itoa(num, temp, 10);

	int len = strlen(temp);
	int pad = 4 - len;

	for (int i = 0; i < pad; i++)
		out[i] = '?';
	for (int i = 0; i < len; i++)
		out[pad + i] = temp[i];

	out[4] = '\0';
	return out;
}

// Get number of decimals on a number. Max: 4
int DisplayController::decimalPlaces(float number) {
	char buffer[12];
	dtostrf(number, 3, 4, buffer);

	size_t counter = strlen(buffer) - 1;
	while (buffer[counter] != '.' && buffer[counter] == '0' && counter != 0) {
		counter--;
	}

	size_t integerCounter = 0;
	while (buffer[integerCounter] != '.' && integerCounter + counter < strlen(buffer) + 1) {
		integerCounter++;
	}

	return counter - integerCounter;
}

// Get index of last leading zero
int DisplayController::lastLeadingZeroIndex(const char* str) {
	int index = -1;
	int len	  = strlen(str);

	for (int i = 0; i < len; i++) {
		if (str[i] == '0') index = i;
		else if (str[i] >= '0' && str[i] <= '9') break;
		else break;
	}

	return index;
}

// All pins LOW -> all pins selected
void DisplayController::selectAllDigits() {
	digitalWrite(_digitSelectionPins[0], LOW);
	digitalWrite(_digitSelectionPins[1], LOW);
	digitalWrite(_digitSelectionPins[2], LOW);
	digitalWrite(_digitSelectionPins[3], LOW);
}

// 1 pin LOW, rest of pins HIGH -> 1 pin selected
void DisplayController::selectDigit(int dig) {
	digitalWrite(_digitSelectionPins[0], HIGH);
	digitalWrite(_digitSelectionPins[1], HIGH);
	digitalWrite(_digitSelectionPins[2], HIGH);
	digitalWrite(_digitSelectionPins[3], HIGH);

	// Enable Selected Digit
	digitalWrite(_digitSelectionPins[dig], LOW);
}

// all segments LOW
void DisplayController::clean() {
	for (int i = 0; i < 8; i++)
		digitalWrite(_segmentPins[i], LOW);
}

// segment code -> digitalWrite(HIGH or LOW) for each pin, dot not included (only if in code)
void DisplayController::shortDisplayCode(uint8_t code) {
	for (int i = 0; i < 8; i++) {
		digitalWrite(_segmentPins[i], (code & (1 << (7 - i))) >> (7 - i) ? HIGH : LOW);
	}
}

// segment code -> digitalWrite(HIGH or LOW) for each pin, dot included
void DisplayController::shortDisplayCode(uint8_t code, boolean dot) {
	if (dot) code |= 1;
	else code &= ~1;

	for (int i = 0; i < 8; i++)
		digitalWrite(_segmentPins[i], (code & (1 << (7 - i))) >> (7 - i) ? HIGH : LOW);
}

// number -> segment code -> digitalWrite(HIGH or LOW) for each pin. dot not activated
void DisplayController::shortDisplayNumber(int n) {
	uint8_t code = getCode(n);
	shortDisplayCode(code);
}

// number -> segment code ->  digitalWrite(HIGH or LOW) for each pin, dot included
void DisplayController::shortDisplayNumber(int n, boolean dot) {
	uint8_t code = getCode(n);
	shortDisplayCode(code, dot);
}

// char -> segment code -> digitalWrite(HIGH or LOW) for each pin. dot not activated
void DisplayController::shortDisplayLetter(char c) {
	uint8_t code = getCode(c);
	shortDisplayCode(code);
}

// char -> segment code -> digitalWrite(HIGH or LOW) for each pin. dot not activated
void DisplayController::shortDisplayLetter(char c, boolean dot) {
	uint8_t code = getCode(c);
	shortDisplayCode(code, dot);
}

// segment code -> digitalWrite(HIGH or LOW) for each pin, dot not included (only if in code)
void DisplayController::shortDisplay(uint8_t code) { shortDisplayCode(code); }

// number -> segment code -> digitalWrite(HIGH or LOW) for each pin. dot not activated
void DisplayController::shortDisplay(int n) { shortDisplayNumber(n); }

// number -> segment code -> digitalWrite(HIGH or LOW) for each pin. dot not activated
void DisplayController::shortDisplay(char c) { shortDisplayLetter(c); }

// segment code -> digitalWrite(HIGH or LOW) for each pin, dot included (only if in code)
void DisplayController::shortDisplay(uint8_t code, boolean dot) { shortDisplayCode(code, dot); }

// number -> segment code -> digitalWrite(HIGH or LOW) for each pin. dot activated
void DisplayController::shortDisplay(int n, boolean dot) { shortDisplayNumber(n, dot); }

// number -> segment code -> digitalWrite(HIGH or LOW) for each pin. dot activated
void DisplayController::shortDisplay(char c, boolean dot) { shortDisplayLetter(c, dot); }

// cycle function: display one number
void DisplayController::cycle(long digitDisplayTime, int number) {
	clean();
	for (int i = 0; i < 4; i++) {
		selectDigit(i);
		shortDisplay((abs(number) / (int)pow(10, 3 - i)) % 10);
		delayMicroseconds(digitDisplayTime);
		clean();
	}
}

// cycle function: display one number with dot
void DisplayController::cycle(long digitDisplayTime, int number, int dot) {
	clean();
	for (int i = 0; i < 4; i++) {
		selectDigit(i);
		shortDisplay((abs(number) / (int)pow(10, 3 - i)) % 10, dot == i);
		delayMicroseconds(digitDisplayTime);
		clean();
	}
}

// cycle function: display char[]
void DisplayController::cycle(long digitDisplayTime, char* word) {
	clean();
	for (int i = 0; i < 4; i++) {
		selectDigit(i);
		shortDisplay(word[i]);
		delayMicroseconds(digitDisplayTime);
		clean();
	}
}

// cycle function: display char[] with dot
void DisplayController::cycle(long digitDisplayTime, char* word, int dot) {
	clean();
	for (int i = 0; i < 4; i++) {
		selectDigit(i);
		shortDisplay(word[i], dot == i);
		delayMicroseconds(digitDisplayTime);
		clean();
	}
}

// cycle function: display custom codes
void DisplayController::cycle(long digitDisplayTime, uint8_t codes[4]) {
	clean();
	for (int i = 0; i < 4; i++) {
		selectDigit(i);
		shortDisplay(codes[i]);
		delayMicroseconds(digitDisplayTime);
		clean();
	}
}

// cycle function: display custom codes with dot
void DisplayController::cycle(long digitDisplayTime, uint8_t codes[4], int dot) {
	clean();
	for (int i = 0; i < 4; i++) {
		selectDigit(i);
		shortDisplay(codes[i], dot == i);
		delayMicroseconds(digitDisplayTime);
		clean();
	}
}

// zeroAnimation function, with all parameters available
void DisplayController::zeroAnimation(long cycleDelay, long cycles) {
	int	 counter	   = 8;
	long cycleIterator = 0L;
	while (counter > 0) {
		while (cycleIterator < cycles) {
			clean();
			selectAllDigits();
			cycleIterator % 4 == 0 && counter % 2 == 0 ? shortDisplayNumber(0, false) : clean();

			delayMicroseconds(cycleDelay);
			cycleIterator++;
		}
		cycleIterator = 0;
		counter--;
	}
}

// zeroAnimation function, with default parameters
void DisplayController::zeroAnimation() { zeroAnimation(155, 100 * 16); }

// ########################## CODES ##########################
// Display codes
void DisplayController::Display(long cyclesAmount, long cycleDelay, long digitDisplayTime, uint8_t codes[4], int dot) {
	long cycleIterator = 0L;
	while (cycleIterator < cyclesAmount) {
		clean();
		for (int i = 0; i < 4; i++) {
			selectDigit(i);
			shortDisplay(codes[i], dot == i);
			delayMicroseconds(digitDisplayTime);
			clean();
		}
		delayMicroseconds(cycleDelay);
		cycleIterator++;
	}
}

// Display codes
void DisplayController::Display(long cyclesAmount, long cycleDelay, long digitDisplayTime, uint8_t codes[4]) {
	Display(cyclesAmount, cycleDelay, digitDisplayTime, codes, 5);
}

// Display codes
void DisplayController::Display(int seconds, uint8_t codes[4], int dot) {
	Display(seconds * 690L, 400, 155, codes, dot);
}

// Display codes
void DisplayController::Display(int seconds, uint8_t codes[4]) { Display(seconds * 690L, 400, 155, codes, 5); }

// ########################## WORDS ##########################
// Display words
void DisplayController::Display(long cyclesAmount, long cycleDelay, long digitDisplayTime, char* word, int dot) {
	long cycleIterator = 0L;
	while (cycleIterator < cyclesAmount) {
		clean();
		for (int i = 0; i < 4; i++) {
			selectDigit(i);
			shortDisplay(word[i], dot == i);
			delayMicroseconds(digitDisplayTime);
			clean();
		}

		delayMicroseconds(cycleDelay);
		cycleIterator++;
	}
}

// Display words
void DisplayController::Display(long cyclesAmount, long cycleDelay, long digitDisplayTime, char* word) {
	Display(cyclesAmount, cycleDelay, digitDisplayTime, word, 5);
}

// Display words
void DisplayController::Display(int seconds, char* word, int dot) { Display(seconds * 690L, 400, 155, word, dot); }

// Display words
void DisplayController::Display(int seconds, char* word) { Display(seconds * 690L, 400, 155, word, 5); }

// ########################## NUMBERS ##########################
// Display numbers (negative numbers only up to 3 digits, 1 last digit is minus)
void DisplayController::Display(long cyclesAmount, long cycleDelay, long digitDisplayTime, int n, int dot) {
	char* formatted = formatNumber(n);
	Display(cyclesAmount, cycleDelay, digitDisplayTime, formatted, dot);
}

// Display numbers (negative numbers only up to 3 digits, 1 last digit is minus)
void DisplayController::Display(long cyclesAmount, long cycleDelay, long digitDisplayTime, int n) {
	Display(cyclesAmount, cycleDelay, digitDisplayTime, n, 5);
}

// Display numbers (negative numbers only up to 3 digits, 1 last digit is minus)
void DisplayController::Display(int seconds, int n, int dot) { Display(seconds * 690L, 400, 155, n, dot); }

// Display numbers (negative numbers only up to 3 digits, 1 last digit is minus)
void DisplayController::Display(int seconds, int n) { Display(seconds * 690L, 400, 155, n, 5); }



// ##################### WRITE/UPDATE ####################
void DisplayController::write(uint32_t codes, unsigned long time) {
	code_[0]   = codes >> 8 * 0;
	code_[1]   = codes >> 8 * 1;
	code_[2]   = codes >> 8 * 2;
	code_[3]   = codes >> 8 * 3;
	codeStart_ = time;
}

void DisplayController::write(const char* word, unsigned long time) {
	code_[0]   = getCode(word[0]);
	code_[1]   = getCode(word[1]);
	code_[2]   = getCode(word[2]);
	code_[3]   = getCode(word[3]);
	codeStart_ = time;
}

void DisplayController::write(int num, unsigned long time) {
	char* formatted = formatNumber(num);
	write(formatted, time);
}

void DisplayController::update(unsigned long time) {
	int cycle = ((time - codeStart_) / 155) % 4;
	clean();
	selectDigit(cycle);
	shortDisplay(code_[cycle]);
}