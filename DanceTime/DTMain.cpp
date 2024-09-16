#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "AnimSurface.h"
#include "Color.h"
#include "EventManager.h"
#include "FileManager.h"
#include "FontManager.h"
#include "GUI.h"
#include "SoundManager.h"
#include "Surface.h"
#include "TextSurface.h"
#include "WindowManager.h"
#include "Utils.h"

using namespace std;

using Bingo::Colors::BLACK;
using Bingo::Colors::BLUE;
using Bingo::Colors::LIGHTGRAY;
using Bingo::Colors::RED;
using Bingo::Colors::WHITE;
using Bingo::EventManager;
using Bingo::FileManager;
using Bingo::Events::QuitListener;
using Bingo::FontManager;
using Bingo::Guis::ButtonText;
using Bingo::Guis::DropDown;
using Bingo::Guis::Input;
using Bingo::Guis::Slider;
using Bingo::NBT_Compound;
using Bingo::NBT_Tag;
using Bingo::SoundManager;
using Bingo::Surfaces::AnimSurface;
using Bingo::Surfaces::Surface;
using Bingo::Surfaces::TextSurface;
using Bingo::Surfaces::WindowManager;
using Bingo::Time::Counter;
using Bingo::Time::Timer;

#define PROJ_NAME "DanceTime"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_TIMERS 18

vector<ButtonText*> testSoundButtons;
vector<DropDown*> soundDropDowns;
vector<Input*> timeInputs;
vector<Input*> durationInputs;
vector<bool> alarmsValid;
vector<AnimSurface*> statuses;
vector<Counter*> tempTimers;
vector<string> activeSounds;

void CheckInputs() {
	for (auto iter = soundDropDowns.begin(); iter != soundDropDowns.end(); iter++) {
		uint index = distance(soundDropDowns.begin(), iter);
		string filename = (*iter)->getText();

		if (FileManager::getSingleton().checkFile(filename)) {
			if (!alarmsValid[index]) {
				string soundName = "sound" + to_string(index);

				SoundManager::getSingleton().removeSound(soundName);
				SoundManager::getSingleton().addSound(soundName, filename);

				if (Counter::isTimeStr(timeInputs[index]->getText(), "%h:%m%p")) {
					tempTimers[index]->setTime(Counter::makeTime(timeInputs[index]->getText(), "%h:%m%p"));

					statuses[index]->setClip(0);
					alarmsValid[index] = true;

					//cout << "All valid\n";
				}
			}
		}
		else {
			statuses[index]->setClip(1);
			alarmsValid[index] = false;
		}
	}
}

void TestSound(ButtonText& button, EventManager::MouseButton mouseButton) {
	for (auto iter = testSoundButtons.begin(); iter != testSoundButtons.end(); iter++) {
		if (*iter == &button) {
			auto index = distance(testSoundButtons.begin(), iter);

			SoundManager::getSingleton().removeSound("testSound");
			SoundManager::getSingleton().addSound("testSound", soundDropDowns[index]->getText());
			SoundManager::getSingleton().playSound("testSound");

			activeSounds.push_back("testSound");
		}
	}
}

void InvalidateSound0(DropDown& dropDown) {
	for (auto iter = soundDropDowns.begin(); iter != soundDropDowns.end(); iter++) {
		if (*iter == &dropDown) {
			auto index = distance(soundDropDowns.begin(), iter);

			alarmsValid[index] = false;
		}
	}
}

void InvalidateSound1(Input& input, EventManager::MouseButton mouseButton) {
	input.inputMode();

	for (auto iter = timeInputs.begin(); iter != timeInputs.end(); iter++) {
		if (*iter == &input) {
			auto index = distance(timeInputs.begin(), iter);

			alarmsValid[index] = false;
		}
	}
}

void InvalidateSound2(Input& input, EventManager::MouseButton mouseButton) {
	input.inputMode();

	for (auto iter = durationInputs.begin(); iter != durationInputs.end(); iter++) {
		if (*iter == &input) {
			auto index = distance(durationInputs.begin(), iter);

			alarmsValid[index] = false;
		}
	}
}

void StopSounds(ButtonText& button, EventManager::MouseButton but) {
	auto iter = activeSounds.begin();

	while (iter != activeSounds.end()) {
		SoundManager::getSingleton().stopSound(*iter);
		iter = activeSounds.erase(iter);
	}
}

void ChangeVolume(Slider<int>& slider) {
	SoundManager::getSingleton().setSoundVolume(slider.getValue() / 10.0f);
}

void SaveStuff(ButtonText& button, EventManager::MouseButton but) {
	FileManager* fileMan = FileManager::getSingletonPtr();
	vector<string> fileFilter;
	fileFilter.push_back("*.nbt");

	string fileName = fileMan->saveFileDialog("resources/Nbt/", &fileFilter);

	if (fileName.empty()) {
		return;
	}

	NBT_Compound* nbt = new NBT_Compound("DT_Values");
	NBT_Compound* sounds = new NBT_Compound("sounds");

	for (uint c = 0; c < soundDropDowns.size(); c++) {
		sounds->setTag(new NBT_Tag<uint>("sound" + to_string(c), soundDropDowns[c]->getSelection()));
	}

	nbt->setTag(sounds);

	NBT_Compound* times = new NBT_Compound("times");

	for (uint c = 0; c < timeInputs.size(); c++) {
		times->setTag(new NBT_Tag<string>("time" + to_string(c), timeInputs[c]->getText()));
	}

	nbt->setTag(times);

	NBT_Compound* durations = new NBT_Compound("durations");

	for (uint c = 0; c < durationInputs.size(); c++) {
		durations->setTag(new NBT_Tag<string>("duration" + to_string(c), durationInputs[c]->getText()));
	}

	nbt->setTag(durations);

	fileMan->openFile(fileName, FileManager::WRITE);
	fileMan->writeNBT(fileName, nbt);
	fileMan->closeFile(fileName);

	delete nbt;
}

void LoadStuff(ButtonText& button, EventManager::MouseButton but) {
	FileManager* fileMan = FileManager::getSingletonPtr();
	vector<string> fileFilter;
	fileFilter.push_back("*.nbt");

	string fileName = fileMan->openFileDialog("resources/Nbt/", &fileFilter);

	if (fileName.empty()) {
		return;
	}

	fileMan->openFile(fileName, FileManager::READ);
	NBT_Compound* nbt = fileMan->readNBT(fileName);
	fileMan->closeFile(fileName);

	NBT_Compound* sounds = static_cast<NBT_Compound*>(nbt->getTag("sounds"));

	for (uint c = 0; c < sounds->getTagCount(); c++) {
		soundDropDowns[c]->setSelection(static_cast<NBT_Tag<uint>*>(sounds->getTag("sound" + to_string(c)))->getData());
	}

	NBT_Compound* times = static_cast<NBT_Compound*>(nbt->getTag("times"));

	for (uint c = 0; c < times->getTagCount(); c++) {
		timeInputs[c]->setText(static_cast<NBT_Tag<string>*>(times->getTag("time" + to_string(c)))->getData());
	}

	NBT_Compound* durations = static_cast<NBT_Compound*>(nbt->getTag("durations"));

	for (uint c = 0; c < durations->getTagCount(); c++) {
		durationInputs[c]->setText(static_cast<NBT_Tag<string>*>(durations->getTag("duration" + to_string(c)))->getData());
	}

	delete nbt;
}

int main(int argc, char* argv[]) {
	WindowManager myWindow(PROJ_NAME, 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT);
	EventManager eventManager;
	QuitListener quitListener;
	FileManager fileManager("resources/Nbt/");
	FontManager fontManager("resources/Font/");
	SoundManager soundManager;

	fontManager.nickname("expressway rg.ttf", "main");

	Surface screen(SCREEN_WIDTH, SCREEN_HEIGHT);
	TextSurface title("main", 36, PROJ_NAME, BLACK);
	Surface background("resources/Images/Background.png");

	TextSurface soundLabel("main", 20, "Sound name", BLACK);
	soundLabel.setPos({ 40, 10 });
	TextSurface timeLabel("main", 20, "Times", BLACK);
	timeLabel.setPos({ 400, 10 });
	TextSurface durationLabel("main", 20, "Duration", BLACK);
	durationLabel.setPos({ 525, 10 });

	ButtonText stopButton(SCREEN_WIDTH - 80, 20, StopSounds, "main", 20, "Stop");
	stopButton.setTextPadding(5);
	stopButton.setBackgroundColor(WHITE);

	Slider<int> volSlider(SCREEN_WIDTH - 60, 100, 40, 200, 0, 10, 1, ChangeVolume, "main", 20);
	volSlider.setOrientation(Slider<int>::Orientation::VERTICAL);
	volSlider.setBackgroundColor(LIGHTGRAY);
	volSlider.setFrameColor(BLUE);
	volSlider.setColor(RED);
	volSlider.setValue(5);

	ButtonText saveButton(SCREEN_WIDTH - 80, SCREEN_HEIGHT - 120, SaveStuff, "main", 20, "Save");
	saveButton.setTextPadding(5);
	saveButton.setBackgroundColor(WHITE);

	ButtonText loadButton(SCREEN_WIDTH - 80, SCREEN_HEIGHT - 60, LoadStuff, "main", 20, "Load");
	loadButton.setTextPadding(5);
	loadButton.setBackgroundColor(WHITE);

	vector<string> soundFiles = fileManager.listDirectory("resources/Sounds/");

	cout << "Royalty free music made available by www.bensound.com" << endl;
	cout << "Thank you BenSound" << endl;

	for (uint c = 0; c < MAX_TIMERS; c++) {
		int yPos = 40 + 30 * c;

		testSoundButtons.push_back(new ButtonText(10, yPos, TestSound, "main", 13, "T"));
		testSoundButtons.back()->setTextPadding(5);
		testSoundButtons.back()->setBackgroundColor(WHITE);

		soundDropDowns.push_back(new DropDown(50, yPos, 350, 25, &soundFiles, InvalidateSound0, "main", 13));
		soundDropDowns.back()->setTextPadding(5);
		soundDropDowns.back()->setBackgroundColor(WHITE);

		timeInputs.push_back(new Input(425, yPos, 100, 25, InvalidateSound1, "main", 13, ""));
		timeInputs.back()->setTextPadding(5);
		timeInputs.back()->setBackgroundColor(WHITE);

		durationInputs.push_back(new Input(550, yPos, 100, 25, InvalidateSound2, "main", 13, ""));
		durationInputs.back()->setTextPadding(5);
		durationInputs.back()->setBackgroundColor(WHITE);

		statuses.push_back(new AnimSurface("resources/Images/Status.png"));
		statuses.back()->setPos({ 650, yPos - 5 });
		statuses.back()->setScale(.15f);
		statuses.back()->addClip(0, 0, 200, 200);
		statuses.back()->addClip(0, 200, 200, 200);
		statuses.back()->setAnimSpeed(0);
		statuses.back()->setClip(1);

		alarmsValid.push_back(false);

		tempTimers.push_back(new Counter());
	}

	while (!quitListener.getDone()) {
		eventManager.update();

		//check inputs and times
		if (Timer::getTicks() % 1000) {
			CheckInputs();
		}

		//check timers, play sound if timer is up
		for (uint c = 0; c < tempTimers.size(); c++) {
			if (tempTimers[c]->isTimeUp()) {
				string soundName = "sound" + to_string(c);

				if (Counter::isTimeStr(durationInputs[c]->getText(), "%m%s")) {
					tm dura = Counter::makeTime(durationInputs[c]->getText(), "%m%s");

					soundManager.playSoundFor(soundName, dura.tm_sec + dura.tm_min * 60);
				}
				else {
					soundManager.playSound(soundName);
				}

				activeSounds.push_back(soundName);
			}
		}

		//remove sounds that are done playing
		for (auto iterSounds = activeSounds.begin(); iterSounds != activeSounds.end(); iterSounds++) {
			if (soundManager.isSoundNotPlaying(*iterSounds)) {
				iterSounds = activeSounds.erase(iterSounds);

				if (iterSounds == activeSounds.end()) {
					break;
				}
			}
		}

		screen.setRenderTarget();
		screen.draw(background);
		screen.draw(soundLabel);
		screen.draw(durationLabel);
		screen.draw(timeLabel);
		screen.draw(stopButton);
		screen.draw(volSlider);
		screen.draw(saveButton);
		screen.draw(loadButton);

		for (int c = soundDropDowns.size() - 1; c >= 0; c--) {
			if (soundDropDowns[c]) {
				statuses[c]->update();

				screen.draw(*testSoundButtons[c]);
				screen.draw(*soundDropDowns[c]);
				screen.draw(*timeInputs[c]);
				screen.draw(*durationInputs[c]);
				//screen.drawScaled(*statuses[c], .15f, .15f);
				screen.draw(*statuses[c]);
			}
		}

		screen.clearRenderTarget();
		myWindow.draw(screen, 0, 0);
		myWindow.update();
	}

	for (uint c = 0; c < soundDropDowns.size(); c++) {
		delete soundDropDowns[c];
		delete timeInputs[c];
		delete durationInputs[c];
		delete statuses[c];
		delete tempTimers[c];
	}

	return 0;
}

