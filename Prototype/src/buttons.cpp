#include "include/buttons.h"


ButtonEvent Buttons::up() {
	return none;
}


ButtonEvent Buttons::down(const unsigned short voltageLevel) {
	if((unsigned)(voltageLevel - A_BUTTON_MIN) <= A_BUTTON_RANGE) {
		Serial.println(F("A button"));
		return ButtonEvent(BUTTON::A, true);
	}

	if((unsigned)(voltageLevel - B_BUTTON_MIN) <= B_BUTTON_RANGE) {
		Serial.println(F("B button"));
		return ButtonEvent(BUTTON::B, true);
	}

	if((unsigned)(voltageLevel - C_BUTTON_MIN) <= C_BUTTON_RANGE) {
		Serial.println(F("C button"));
		return ButtonEvent(BUTTON::C, true);
	}

	if((unsigned)(voltageLevel - AB_BUTTON_MIN) <= AB_BUTTON_RANGE) {
		Serial.println(F("A+B buttons"));
		return ButtonEvent(BUTTON::AB, true);
	}

	if((unsigned)(voltageLevel - AC_BUTTON_MIN) <= AC_BUTTON_RANGE) {
		Serial.println(F("A+C buttons"));
		return ButtonEvent(BUTTON::AC, true);
	}

	if((unsigned)(voltageLevel - BC_BUTTON_MIN) <= BC_BUTTON_RANGE) {
		Serial.println(F("B+C buttons"));
		return ButtonEvent(BUTTON::BC, true);
	}

	if((unsigned)(voltageLevel - ABC_BUTTON_MIN) <= ABC_BUTTON_RANGE) {
		Serial.println(F("A+B+C buttons"));
		return ButtonEvent(BUTTON::ABC, true);
	}

	Serial.print(F("Unexpected voltage level: "));
	Serial.println(voltageLevel);
}


ButtonEvent Buttons::poll() {
	nowMS = millis();
	inputVoltageLevel = analogRead(INPUT_PIN);
	event = none;
	//Serial.println(inputVoltageLevel);

	if(inputVoltageLevel > GROUND_THRESHOLD_VALUE) { //AT LEAST ONE BUTTON IS CURRENTLY BEING PRESSED
		if(buttonPressedLastFrame == false) { //FIRST FRAME OF BUTTON PRESSED
			inputStartTime = nowMS;
			buttonPressedLastFrame = true;
			inputProcessed = false;
		}

		if(nowMS < inputStartTime + INPUT_SEARCH_MS && inputVoltageLevel > highestInputVoltageLevel) {
			highestInputVoltageLevel = inputVoltageLevel;
		}
	} else { //NO BUTTONS ARE CURRENTLY BEING PRESSED
		if(buttonPressedLastFrame) {
			event = up();
			buttonPressedLastFrame = false;
		}
	}

	if(inputProcessed == false && nowMS > inputStartTime + INPUT_SEARCH_MS) {
		event = down(highestInputVoltageLevel);

		highestInputVoltageLevel = 0;
		inputProcessed = true;
	}

	return event;
}