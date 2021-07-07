#ifndef BUTTONS_H
#define BUTTONS_H

#include "Arduino.h"


//TIE-DOWN_RESISTANCE = 10000 Ohms

//NOTE: +-1 Margin Added

//A_BUTTON_RESISTANCE = 1000 Ohms
const static constexpr unsigned short A_BUTTON_MIN = 617;
const static constexpr unsigned short A_BUTTON_MAX = 621;
const static constexpr unsigned short A_BUTTON_RANGE = A_BUTTON_MAX - A_BUTTON_MIN;

//B_BUTTON_RESISTANCE = 2000 Ohms
const static constexpr unsigned short B_BUTTON_MIN = 565;
const static constexpr unsigned short B_BUTTON_MAX = 569;
const static constexpr unsigned short B_BUTTON_RANGE = B_BUTTON_MAX - B_BUTTON_MIN;

//C_BUTTON_RESISTANCE = 3000 Ohms
const static constexpr unsigned short C_BUTTON_MIN = 523;
const static constexpr unsigned short C_BUTTON_MAX = 527;
const static constexpr unsigned short C_BUTTON_RANGE = C_BUTTON_MAX - C_BUTTON_MIN;

const static constexpr unsigned short AB_BUTTON_MIN = 637;
const static constexpr unsigned short AB_BUTTON_MAX = 641;
const static constexpr unsigned short AB_BUTTON_RANGE = AB_BUTTON_MAX - AB_BUTTON_MIN;

const static constexpr unsigned short AC_BUTTON_MIN = 632;
const static constexpr unsigned short AC_BUTTON_MAX = 636;
const static constexpr unsigned short AC_BUTTON_RANGE = AC_BUTTON_MAX - AC_BUTTON_MIN;

const static constexpr unsigned short BC_BUTTON_MIN = 607;
const static constexpr unsigned short BC_BUTTON_MAX = 611;
const static constexpr unsigned short BC_BUTTON_RANGE = BC_BUTTON_MAX - BC_BUTTON_MIN;

const static constexpr unsigned short ABC_BUTTON_MIN = 644;
const static constexpr unsigned short ABC_BUTTON_MAX = 648;
const static constexpr unsigned short ABC_BUTTON_RANGE = ABC_BUTTON_MAX - ABC_BUTTON_MIN;


static const unsigned short INPUT_PIN = A0; //IMPORTANT NOTE: this pin value may change based on your board


enum class BUTTON {NONE, A, B, C, AB, AC, BC, ABC};

struct ButtonEvent {
	BUTTON button;
	bool pressed;

	ButtonEvent(BUTTON b, bool p) {
		button = b;
		pressed = p;
	}
};




class Buttons {
private:
	static const unsigned short GROUND_THRESHOLD_VALUE = 3;

	static const unsigned short INPUT_SEARCH_MS = 70; //Period to wait for additional inputs before reporting button press. Button Down is triggered at the end of the input search period

	unsigned short inputVoltageLevel = 0;
	unsigned short highestInputVoltageLevel = 0;

	unsigned long long nowMS = 0;
	
	bool buttonPressedLastFrame = false;
	unsigned long long inputStartTime = 0;
	bool inputProcessed = true;

	const ButtonEvent none = ButtonEvent(BUTTON::NONE, false);
	ButtonEvent event = none;

	ButtonEvent down(const unsigned short voltageLevel);
	ButtonEvent up();
public:
	ButtonEvent poll();
};

#endif