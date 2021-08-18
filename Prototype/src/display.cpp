#include "include/display.h"



TitleScreen::TitleScreen(Display *d, void (Display::*c)(), Adafruit_SSD1331& o, Localization& l, EventQueue& e) : GameScene(o, l, e) {
	titleFirstString = loc.getLocalizedString(LOC_TITLE_FIRST);
	titleLastString = loc.getLocalizedString(LOC_TITLE_LAST);
	startPromptString = loc.getLocalizedString(LOC_START_PROMPT);
	
	oled.setTextSize(2);
	oled.setTextColor(RED);
	oled.setCursor(0, 0);
	oled.print(titleFirstString);

	oled.getTextBounds(titleFirstString, 0, 0, &calc.x, &calc.y, &calc.w, &calc.h);
	unsigned short titleFirstHeight = calc.h;

	oled.getTextBounds(titleLastString, 0, 0, &calc.x, &calc.y, &calc.w, &calc.h);
	unsigned short titleLastWidth = calc.w;
	bottomOfTitleY = titleFirstHeight + TEXT_VERTICAL_MARGIN + calc.h;
	oled.setCursor(DISPLAY_WIDTH - titleLastWidth, titleFirstHeight + TEXT_VERTICAL_MARGIN);
	oled.print(titleLastString);

	oled.setTextSize(1);
	oled.getTextBounds(startPromptString, (DISPLAY_WIDTH - calc.w)/2, bottomOfTitleY + TEXT_VERTICAL_MARGIN, &calc.x, &calc.y, &calc.w, &calc.h);

	startGameListener = DisplayListener(d, c, EVENT_TYPE::EVENT_RAW_INPUT_DOWN);
	e.registerListener(startGameListener);
}


void TitleScreen::draw(unsigned long long nowMS) {
	if(lastStartTextStateChangeMS + START_TEXT_STATE_CHANGE_THRESHOLD_MS < nowMS) {
		startTextVisible = !startTextVisible;
		lastStartTextStateChangeMS = nowMS;

		if(startTextVisible) {
			oled.setTextColor(WHITE);
			oled.setCursor(calc.x, calc.y);
			oled.print(startPromptString);
		} else {
			oled.fillRect(calc.x, calc.y, calc.w, calc.h, BLACK);
		}
	}
}


TitleScreen::~TitleScreen() {
	events.unregisterListener(startGameListener);
}


MainMenu::MainMenu(Adafruit_SSD1331& o, Localization& l, EventQueue& e) : GameScene(o, l, e) {
	titleString = loc.getLocalizedString(LOC_TITLE);
	playString = loc.getLocalizedString(LOC_PLAY);
	optionsString = loc.getLocalizedString(LOC_OPTIONS);
	quitString = loc.getLocalizedString(LOC_QUIT);

	oled.getTextBounds(titleString, 0, 0, &calc.x, &calc.y, &calc.w, &calc.h);
	oled.drawLine((DISPLAY_WIDTH - calc.w)/2, calc.y + calc.h, DISPLAY_WIDTH - (DISPLAY_WIDTH - calc.w)/2, calc.y + calc.h, RED);
	oled.setCursor((DISPLAY_WIDTH - calc.w)/2, TEXT_VERTICAL_MARGIN);
	oled.print(titleString);

	

	unsigned short lineBottom = calc.y + calc.h;

	oled.getTextBounds(playString, 0, 0, &calc.x, &calc.y, &calc.w, &calc.h);
	playButtonRect = Gib::Rect((DISPLAY_WIDTH - calc.w)/2, lineBottom + TEXT_VERTICAL_MARGIN + calc.h, calc.w, calc.h);
	lineBottom = lineBottom + TEXT_VERTICAL_MARGIN + calc.h;
	oled.setCursor(playButtonRect.x, playButtonRect.y);
	oled.print(playString);


	oled.getTextBounds(optionsString, 0, 0, &calc.x, &calc.y, &calc.w, &calc.h);
	optionsButtonRect = Gib::Rect((DISPLAY_WIDTH - calc.w)/2, lineBottom + TEXT_VERTICAL_MARGIN + calc.h, calc.w, calc.h);
	lineBottom = lineBottom + TEXT_VERTICAL_MARGIN + calc.h;
	oled.setCursor(optionsButtonRect.x, optionsButtonRect.y);
	oled.print(optionsString);


	oled.getTextBounds(quitString, 0, 0, &calc.x, &calc.y, &calc.w, &calc.h);
	quitButtonRect = Gib::Rect((DISPLAY_WIDTH - calc.w)/2, lineBottom + TEXT_VERTICAL_MARGIN + calc.h, calc.w, calc.h);
	oled.setCursor(quitButtonRect.x, quitButtonRect.y);
	oled.print(quitString);
}


void MainMenu::draw(unsigned long long nowMS) {
	if(selectedMenu != lastSelectedMenu) {
		events.enqueue(EVENT_TYPE::EVENT_MENU_SELECTION_CHANGED, 0);

		switch(selectedMenu) {
		case 'B':
			oled.fillRect(playButtonRect.x, playButtonRect.y, playButtonRect.w, playButtonRect.h, WHITE);
			oled.setTextColor(BLACK);
			oled.setCursor(playButtonRect.x, playButtonRect.y);
			oled.print(playString);
		break;

		case 'O':
			oled.fillRect(optionsButtonRect.x, optionsButtonRect.y, optionsButtonRect.w, optionsButtonRect.h, WHITE);
			oled.setTextColor(BLACK);
			oled.setCursor(optionsButtonRect.x, optionsButtonRect.y);
			oled.print(optionsString);
		break;

		case 'Q':
			oled.fillRect(quitButtonRect.x, quitButtonRect.y, quitButtonRect.w, quitButtonRect.h, WHITE);
			oled.setTextColor(BLACK);
			oled.setCursor(quitButtonRect.x, quitButtonRect.y);
			oled.print(quitString);
		break;

		default:
			//log error: unknown menu!
		break;
		}

		lastSelectedMenu = selectedMenu;
	}
}


MainMenu::~MainMenu() {
	
}


void Display::leaveTitleScreen() {
	clear();

	delete currentScene;
	currentScene = new MainMenu(oled, loc, events);
}


void Display::clear() {
	oled.fillScreen(BLACK);
	oled.setTextSize(1);
	oled.setTextColor(WHITE);
}


void Display::setup() {
	oled.begin();
	clear();
	currentScene = new TitleScreen(this, &Display::leaveTitleScreen, oled, loc, events);
}


void Display::updateDisplay(unsigned long long nowMS) {
	currentScene->draw(nowMS);
}


Display::Display(EventQueue& e) : events{e} {

}