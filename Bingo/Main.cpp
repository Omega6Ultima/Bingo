//Dustin Gehm
#define SOUNDTIMER 0

#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "_DefaultDefines.h"
#include "Utils.h"

#ifndef MIXER_AUDIO_FREQ
#define MIXER_AUDIO_FREQ 44100
#endif

#ifndef MIXER_AUDIO_CHANNELS
#define MIXER_AUDIO_CHANNELS 2
#endif

using std::cerr;

void initSDLModules(){
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
		cerr << "SDL could not be loaded\n";
		cerr << SDL_GetError();
		BAIL(1);
	}

	uint SDL_IMG_Flags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(SDL_IMG_Flags) & SDL_IMG_Flags)){
		cerr << "SDL_image could not be loaded\n";
		cerr << IMG_GetError();
		BAIL(1);
	}

	if (TTF_Init() == -1){
		cerr << "SDL_TTF could not be loaded\n";
		cerr << TTF_GetError();
		BAIL(1);
	}

	if (Mix_OpenAudio(MIXER_AUDIO_FREQ, MIX_DEFAULT_FORMAT, MIXER_AUDIO_CHANNELS, 2048) < 0){
		cerr << "SDL_mixer could not be loaded\n";
		cerr << Mix_GetError();
		BAIL(1);
	}
}

void quitSDLModules(){
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

#if SOUNDTIMER
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

using namespace std;

#define PROJ_NAME "DanceTime"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_TIMERS 18

vector<DropDown*> soundDropDowns;
vector<Input*> timeInputs;
vector<Input*> durationInputs;
vector<bool> alarmsValid;
vector<AnimSurface*> statuses;
vector<Counter*> tempTimers;
vector<string> activeSounds;

void checkInputs(){
	for(auto iter = soundDropDowns.begin(); iter != soundDropDowns.end(); iter++){
		uint index = std::distance(soundDropDowns.begin(), iter);
		string filename = (*iter)->getText();

		if (FileManager::getSingleton().checkFile(filename)){
			if (!alarmsValid[index]){
#ifdef _WIN32
				char buffer[10];
				memset(buffer, 0, sizeof(buffer));

				_itoa_s(index, buffer, 10, 10);

				string soundName = string("sound").append(buffer);
#else
				string soundName = string("sound").append(_itoa(index, NULL, 10));
#endif
				SoundManager::getSingleton().removeSound(soundName);
				SoundManager::getSingleton().addSound(soundName, filename);

				if (Counter::isTimeStr(timeInputs[index]->getText(), "%h:%m%p")){
					tempTimers[index]->setTime(Counter::makeTime(timeInputs[index]->getText(), "%h:%m%p"));

					//if (Counter::isTimeStr(durationInputs[index]->getText(), "%m%s")){
					statuses[index]->setClip(0);
					alarmsValid[index] = true;

						//cout << "All valid\n";
					//}
				}
			}
		}
		else{
			statuses[index]->setClip(1);
			alarmsValid[index] = false;
		}
	}
}

void stopSounds(Button& button, EventManager::MouseButton but){
	auto iter = activeSounds.begin();

	while (iter != activeSounds.end()){
		SoundManager::getSingleton().stopSound(*iter);
		iter = activeSounds.erase(iter);
	}
}

void changeVolume(Slider<int>& slider){
	SoundManager::getSingleton().setSoundVolume(slider.getValue() / 10.0f);
}

int main(int argc, char* argv[]){
	initSDLModules();

	WindowManager myWindow(PROJ_NAME, 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT);
	EventManager eventManager;
	QuitListener quitListener;
	FileManager fileManager(PROJ_NAME"/Nbt/");
	FontManager fontManager(PROJ_NAME"/Font/");
	SoundManager soundManager;

	fontManager.nickname("expressway rg.ttf", "main");

	Surface screen(SCREEN_WIDTH, SCREEN_HEIGHT);
	TextSurface title("main", 36, PROJ_NAME, BLACK);
	Surface background(PROJ_NAME"/Images/Background.png");

	TextSurface soundLabel("main", 20, "Sound name", BLACK);
	soundLabel.setPos({ 40, 10 });
	TextSurface timeLabel("main", 20, "Times", BLACK);
	timeLabel.setPos({ 400, 10 });
	TextSurface durationLabel("main", 20, "Duration", BLACK);
	durationLabel.setPos({ 525, 10 });

	Button stopButton(SCREEN_WIDTH - 80, 20, stopSounds, "main", 20, "Stop");
	stopButton.setTextPadding(5);
	stopButton.setBackgroundColor(WHITE);

	Slider<int> volSlider(SCREEN_WIDTH - 60, 100, 40, 200, 0, 10, 1, changeVolume, "main", 20);
	volSlider.setOrientation(Slider<int>::VERTICAL);
	volSlider.setBackgroundColor(LIGHTGRAY);
	volSlider.setValue(5);

	vector<string> soundFiles = fileManager.listDirectory(PROJ_NAME"/Sounds/");

	for (uint c = 0; c < MAX_TIMERS; c++){
		int yPos = 40 + 30 * c;

		soundDropDowns.push_back(new DropDown(50, yPos, 350, 25, &soundFiles, "main", 13));
		soundDropDowns.back()->setTextPadding(5);
		soundDropDowns.back()->setBackgroundColor(WHITE);

		timeInputs.push_back(new Input(425, yPos, 100, 25, NULL, "main", 13, ""));
		timeInputs.back()->setTextPadding(5);
		timeInputs.back()->setBackgroundColor(WHITE);

		durationInputs.push_back(new Input(550, yPos, 100, 25, NULL, "main", 13, ""));
		durationInputs.back()->setTextPadding(5);
		durationInputs.back()->setBackgroundColor(WHITE);

		statuses.push_back(new AnimSurface(PROJ_NAME"/Images/Status.png"));
		statuses.back()->setPos({ 650, yPos - 5});
		statuses.back()->addClip(0, 0, 200, 200);
		statuses.back()->addClip(0, 200, 200, 200);
		statuses.back()->setAnimSpeed(0);
		statuses.back()->setClip(1);

		alarmsValid.push_back(false);

		tempTimers.push_back(new Counter());
	}

	while (!quitListener.getDone()){
		eventManager.update();

		//check inputs and times
		if (Timer::getTicks() % 1000){
			checkInputs();
		}

		//check timers, play sound if timer is up
		for(uint c = 0; c < tempTimers.size(); c++){
			if(tempTimers[c]->isTimeUp()){
#ifdef _WIN32
				char buffer[10];
				memset(buffer, 0, sizeof(buffer));

				_itoa_s(c, buffer, 10, 10);

				string soundName = string("sound").append(buffer);
#else
				string soundName = string("sound").append(_itoa(c, NULL, 10));
#endif

				if(Counter::isTimeStr(durationInputs[c]->getText(), "%m%s")){
					tm dura = Counter::makeTime(durationInputs[c]->getText(), "%m%s");

					soundManager.playSoundFor(soundName, dura.tm_sec + dura.tm_min * 60);
				}
				else{
					soundManager.playSound(soundName);
				}

				activeSounds.push_back(soundName);
			}
		}

		//remove sounds that are done playing
		for (auto iterSounds = activeSounds.begin(); iterSounds != activeSounds.end(); iterSounds++){
			if (soundManager.isSoundNotPlaying(*iterSounds)){
				iterSounds = activeSounds.erase(iterSounds);

				if (iterSounds == activeSounds.end()){
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

		for(int c = soundDropDowns.size() - 1; c >= 0; c--){
			if(soundDropDowns[c]){
				statuses[c]->update();

				screen.draw(*soundDropDowns[c]);
				screen.draw(*timeInputs[c]);
				screen.draw(*durationInputs[c]);
				screen.drawScaled(*statuses[c], .15f, .15f);
			}
		}

		screen.clearRenderTarget();
		myWindow.draw(screen, 0, 0);
		myWindow.update();
	}

	for(uint c = 0; c < soundDropDowns.size(); c++){
		delete soundDropDowns[c];
		delete timeInputs[c];
		delete durationInputs[c];
		delete statuses[c];
		delete tempTimers[c];
	}

	quitSDLModules();

	return 0;
}

#else
#include "AI.h"
#include "AnimSurface.h"
#include "BoundsManager.h"
#include "Color.h"
#include "CompassDirection.h"
#include "EventManager.h"
#include "FileManager.h"
#include "FontManager.h"
#include "GUI.h"
#include "Matrix.h"
#include "MovableSurface.h"
#include "NBT.h"
#include "ParticleEngine.h"
#include "RandomManager.h"
#include "SoundManager.h"
#include "Surface.h"
#include "TextSurface.h"
#include "ThreadManager.h"
#include "Timer.h"
#include "Utils.h"
#include "VecN.h"
#include "WindowManager.h"

using namespace std;

//catch fatal exceptions
#define MAIN_TRY 1

//Testing blocks
#define TEST_SURF_GEOM 1
#define TEST_SURF_CLIP 0
#define TEST_SURF_ANIM 0
#define TEST_SURF_MOVE 1
#define TEST_SOUND 0
#define TEST_MUSIC 0
#define TEST_FILE 1
#define TEST_NBT 0
#define TEST_PART 1
#define TEST_THREADS 0
//come back to testing bounds
#define TEST_BNDS 0
#define TEST_MATS 0
//come back to NeuralNetworks
#define TEST_AI 0
#define TEST_GUI 1
#define TEST_TIME 0

//file paths and other constants
#define IMG_PATH "resources/images/"
#define FNT_PATH "resources/fonts/"
#define SND_PATH "resources/audio/"
#define FIL_PATH "resources/nbt/"

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 640
#endif

#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 480
#endif

#define HALF_SCREEN_WIDTH SCREEN_WIDTH / 2
#define HALF_SCREEN_HEIGHT SCREEN_HEIGHT / 2

void testThreads(NBT_Compound* nbt){
	ATOMIC_LOCK(ThreadManager::coutLock);

	cout << nbt << endl;

	ATOMIC_UNLOCK(ThreadManager::coutLock);

	NBT_Compound* readNBT = NULL;

	FileManager::getSingleton().openFile(FIL_PATH"Test1.nbt", FileManager::READ);

	readNBT = FileManager::getSingleton().readNBT(FIL_PATH"Test1.nbt");

	FileManager::getSingleton().closeFile(FIL_PATH"Test1.nbt");

	if (readNBT != NULL){
		ATOMIC_LOCK(ThreadManager::coutLock);

		cout << readNBT << endl;

		ATOMIC_UNLOCK(ThreadManager::coutLock);

		delete readNBT;
	}
}

void testButton(Button& button, EventManager::MouseButton mouseButton){
	static uint counter = 0;
	button.setText("Press Me Again!");

	cout << "Button Pressed!" << ++counter << endl;
}

void testSliderChanged(Slider<int>& slider){
	//cout << "Slider val is " << slider.getValue() << endl;
}

/*
TODO PRIORITY 0 add comments and documentation to EVERYTHING!
TODO ensure sizeof uses types instead of instances whenever possible
TODO add either string arrays or something else alongside all Enums
TODO find where ever iterators are used and make sure it cant be accomplished using indices

TODO add a 3d camera and view
TODO add a way to have tiled backgrounds that scroll

TODO add a versatile way to prompt for user input

TODO add more capability to the WindowManager for different events, more windows, and multiple displays

TODO add to SoundManager to add capabilities to record audio and save as a playable sound

TODO add mutexes and conditions into ThreadManager once multi-threading stops giving your brain spasms

TODO add bounding box/circle/etc (cube/sphere/etc)

TODO add octtree draw occlusion

TODO maybe add SDL_GetPerformanceCounter to Timer

TODO maybe make it so the three event listeners can be derived from

TODO find a way to copy Surface data for STATIC and PIXEL ACCESS Surfaces
*/
int main(int argc, char* argv[]){
#if MAIN_TRY
	try{
#endif
		initSDLModules();

		WindowManager myWindow("Testing", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT);
		EventManager eventManager;
		KeyListener keyListener;
		MouseListener mouseListener;
		QuitListener quiter;
		FontManager fontManager(FNT_PATH);
		SoundManager soundManager;
		FileManager fileManager(FIL_PATH);
		ThreadManager threadManager;
		RandomManager randomManager;
		BoundsManager boundsManager;

		Surface screen(SCREEN_WIDTH, SCREEN_HEIGHT);
		//Surface image1(IMG_PATH"image1.png", Color(254, 254, 137));

		fontManager.nickname("expressway rg.ttf", "main");
		fontManager.nickname("BebasNeue-Regular.ttf", "bebas");
		fontManager.nickname("berylium rg.ttf", "berylium");
		fontManager.nickname("carbon bl.ttf", "carbonBl");
		fontManager.nickname("carbon phyber.ttf", "carbonPhy");
		fontManager.nickname("expressway rg.ttf", "express");
		fontManager.nickname("goodfish rg.ttf", "goodfish");
		fontManager.nickname("good times rg.ttf", "goodtimes");
		fontManager.nickname("Hunters.otf", "hunters");
		fontManager.nickname("Lazy.ttf", "lazy");
		fontManager.nickname("LUMOS.ttf", "lumos");
		fontManager.nickname("octin prison rg.ttf", "octin");
		fontManager.nickname("Rudiment.ttf", "rudiment");
		fontManager.nickname("teen.ttf", "teen");
		fontManager.nickname("TooneyNoodleNF.ttf", "tooney");

		TextSurface title("main", 36, "Bingo Engine", BLACK);
		TextSurface fpsSurf("main", 20, "0.0 fps", BLACK);

#if TEST_PART
		ParticleEngine partEngine(IMG_PATH "EleParticle.png", 250, 1.5f, HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT, 75);
		ErraticBehavior wavy;

		partEngine.setParticleGravity(true);
		partEngine.setParticleBounds(true, { -HALF_SCREEN_WIDTH, -HALF_SCREEN_HEIGHT, HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT });
		partEngine.remakePartices();

		wavy.setFriction(0.0f);
		wavy.setPhysicsBoost(13.0f);
		wavy.setLifeBoost(1.5f);

		//partEngine.setBehavior(&wavy);
#endif

#if TEST_SURF_MOVE
		MovableSurface star(IMG_PATH"GreenStar.png", 50, SCREEN_HEIGHT / 2, 10.0f, 10.0f);
		star.setGravityEffects(true);
		//TODO this should be changed to width/height instead of radius
		star.setScreenBounding(true, star.getWidth() / 2, {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT});
#endif

#if TEST_MUSIC
		soundManager.setMusicVolume(.1f);
		soundManager.setMusic(SND_PATH"Numa Numa.wav");
#endif

#if TEST_FILE
		fileManager.createDirectory(FIL_PATH "/TestDir");
		fileManager.createFile(FIL_PATH "/TestDir/TestFile.dat");

		cout << "Created " FIL_PATH "/TestDir ? " << fileManager.isDir(FIL_PATH "/TestDir") << endl;
		cout << "Created " FIL_PATH "/TestDir/TestFile.dat ? " << fileManager.isFile(FIL_PATH "/TestDir/TestFile.dat") << endl;

		vector<string> files = fileManager.listDirectory(SND_PATH);

		cout << "Listing " SND_PATH "\n";

		for(auto s : files){
			cout << s << '\n';
		}

		cout << "Attempting to remove " FIL_PATH "/TestDir" << endl;

		fileManager.removeDirectoy(FIL_PATH "/TestDir");

		cout << "Does " FIL_PATH "/TestDir still exist? " << fileManager.isDir(FIL_PATH "/TestDir") << endl;

		string testDir = FIL_PATH "DoNotDelete";

		cout << "Attempting to remove " + testDir << endl;

		fileManager.removeDirectoy(testDir);
#endif

#if TEST_NBT
		string Test1 = FIL_PATH"Test1.nbt";
		fileManager.createFile(Test1);
		fileManager.openFile(Test1, FileManager::WRITE);

		NBT_Compound* nbt = new NBT_Compound("Test1");
		nbt->setTag(new NBT_Tag<bool>("FileOK", true));
		nbt->setTag(new NBT_Tag<char>("Initial", 'd'));
		nbt->setTag(new NBT_Tag<uchar>("Char254", 254));
		nbt->setTag(new NBT_Tag<short>("Points", 15000));
		nbt->setTag(new NBT_Tag<ushort>("BonusPoints", 29000));
		nbt->setTag(new NBT_Tag<int>("Lives", -33938));
		nbt->setTag(new NBT_Tag<uint>("Coins", 80001));
		nbt->setTag(new NBT_Tag<long>("Time", LONG_MAX - 1));
		nbt->setTag(new NBT_Tag<ulong>("DblTime", ULONG_MAX - 1));
		nbt->setTag(new NBT_Tag<llong>("Extremes-", LLONG_MIN));
		nbt->setTag(new NBT_Tag<ullong>("Extremes+", ULLONG_MAX));
		nbt->setTag(new NBT_Tag<float>("Height", 1.75));
		nbt->setTag(new NBT_Tag<double>("Boost", -22.55));
		nbt->setTag(new NBT_Tag<string>("Player", "BobRoss"));

		NBT_Compound* nestNBT = new NBT_Compound("Nested1");
		nestNBT->setTag(new NBT_Tag<string>("LstOppenent", "MisterR"));
		nestNBT->setTag(new NBT_Tag<uint>("LstScore", 20));

		nbt->setTag(nestNBT);

		fileManager.writeNBT(Test1, nbt);

		fileManager.closeFile(Test1);
		fileManager.openFile(Test1, FileManager::READ);

		NBT_Compound* readNbt = NULL;
		readNbt = fileManager.readNBT(Test1);

		if(readNbt != NULL){
			cout << readNbt << endl;
		}

		fileManager.closeFile(Test1);
		delete nbt;
		delete readNbt;
#endif

#if TEST_SOUND
		soundManager.setSoundVolume(.5f);
		soundManager.addSound("clap", SND_PATH"applause_y.wav");
		soundManager.addSound("disk", SND_PATH"bad_disk_x.wav");
		soundManager.addSound("bloop", SND_PATH"bloop_x.wav");
		soundManager.addSound("blurp", SND_PATH"blurp_x.wav");
		soundManager.addSound("boing", SND_PATH"boing_spring.wav");
		soundManager.addSound("numa", SND_PATH"Numa Numa.wav");

		cout << "clap is " << soundManager.getSoundLength("clap") << " milliseconds long" << endl;
		cout << "numa is " << soundManager.getSoundLength("numa") << " milliseconds long" << endl;
#endif

#if TEST_SURF_CLIP
		Surface clipper(IMG_PATH"clipTest.png");
		uint corners[4] = { 0, 0, 0, 0 };
		corners[0] = clipper.addClip(0, 0, 200, 200);
		corners[1] = clipper.addClip(200, 0, 200, 200);
		corners[2] = clipper.addClip(0, 200, 200, 200);
		corners[3] = clipper.addClip(200, 200, 200, 200);
#endif

#if TEST_SURF_ANIM
		AnimSurface anim(IMG_PATH"PulseSquare.png");
		anim.addClip(0, 0, 200, 200);
		anim.addClip(0, 200, 200, 200);
		anim.addClip(0, 400, 200, 200);
		anim.addClip(0, 600, 200, 200);
		anim.setAnimSpeed(8);

		AnimSurface anim2 = anim;
		anim2.setAnimSpeed(6);

		AnimSurface anim3(anim2);
		anim3.setAnimSpeed(0);
		anim3.setPos({ 400, 200 });
#endif

#if TEST_THREADS
		NBT_Compound* nbt1 = new NBT_Compound("cmpd1");
		NBT_Compound* nbt2 = new NBT_Compound("cmpd2");

		nbt1->setTag(new NBT_Tag<int>("ID", 1));
		nbt1->setTag(new NBT_Tag<float>("Seconds", 2.5f));
		nbt1->setTag(new NBT_Tag<string>("Name", "Player1"));
		nbt2->setTag(new NBT_Tag<int>("ID", 2));
		nbt2->setTag(new NBT_Tag<float>("Seconds", 3.5f));
		nbt2->setTag(new NBT_Tag<string>("Name", "Player76"));

		//cache the string representation for quicker prints
		nbt1->toString();
		nbt2->toString();

		threadManager.createThread("thread1", nbt1, testThreads, 2.5f);
		threadManager.createThread("thread2", nbt2, testThreads, 5.0f);

		threadManager.startThread("thread1");
		threadManager.startThread("thread2");

		threadManager.waitOnThread("thread2");
#endif

#if TEST_BNDS
		BoundsCircle boundsCirc1({ 100, 260 }, 10, RED);
		BoundsCircle boundsCirc2({ 500, 250 }, 10, BLUE);
		BoundsBox boundsBox({ 320, 240 },40, 40, GREEN);
		BoundsLine boundsLine({ 220, 100 }, { 520, 100 }, PURPLE);

		boundsCirc1.setVel({ 75, 0 });
		boundsCirc2.setVel({ -37, -37 });

		boundsCirc1.setFriction(0.0f);
		boundsCirc2.setFriction(0.0f);

		boundsManager.addBounds(&boundsCirc1);
		boundsManager.addBounds(&boundsCirc2);
		boundsManager.addBounds(&boundsBox);
		boundsManager.addBounds(&boundsLine);
#endif

#if TEST_MATS
		Matrix<int, 2, 2> twoByTwo(3);
		Matrix<double, 2, 3> twoByThree({0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f});
		Matrix<int, 5, 5> iden5 = Matrix<int, 5, 5>::identity();
		Matrix<int, 5, 5> negIden5 = -Matrix<int, 5, 5>::identity();
		Matrix<int, 2, 3> a({ 1, 2, 3, 4, 5, 6 });
		Matrix<int, 3, 2> b({ 7, 8, 9, 10, 11, 12 });
		Matrix<int, 2, 2> c = a.matrixMultiply(b);

		Uint64 startTime = SDL_GetPerformanceCounter();

		Matrix<double, 9, 9> timeTestA({ 55, 43, 84, 54, 29, 77, 57, 5, 13, 50, 18, 9, 4, 99, 59,
			34, 71, 5, 85, 42, 54, 2, 44, 96, 58, 64, 6, 21, 5, 46, 29, 95, 35, 75, 1, 98, 49, 90,
			44, 39, 18, 34, 74, 73, 25, 7, 26, 89, 6, 56, 51, 57, 83, 54, 8, 52, 13, 82, 89, 32, 42,
			91, 99, 44, 95, 26, 23, 17, 78, 16, 6, 1, 98, 32, 28, 7, 62, 100, 19, 26, 33 });

		Matrix<double, 9, 9> timeTestB({ 89, 65, 8, 28, 65, 41, 13, 97, 67, 38, 16, 4, 41, 61, 34,
			19, 87, 97, 85, 99, 92, 62, 91, 4, 31, 26, 59, 58, 87, 56, 81, 6, 13, 45, 73, 90, 79,
			23, 23, 84, 61, 74, 70, 6, 96, 11, 59, 58, 66, 66, 84, 24, 52, 69, 27, 94, 33, 34, 64,
			74, 24, 81, 72, 2, 43, 7, 19, 49, 57, 56, 38, 23, 40, 94, 100, 94, 91, 85, 36, 11, 1 });

		cout << "Creation: " << SDL_GetPerformanceCounter() - startTime << endl;

		startTime = SDL_GetPerformanceCounter();

		Matrix<double, 9, 9> timeTestC = timeTestA.matrixMultiply(timeTestB);

		cout << "Multiply: " << SDL_GetPerformanceCounter() - startTime << endl;

		cout << iden5 << endl;
		cout << negIden5 << endl;

		cout << c <<endl;

		cout << timeTestC << endl;

		DynMatrix<int> dynTwoByTwo(2, 2, 3);
		DynMatrix<double> dynTwoByThree(2, 3, { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f });
		DynMatrix<int> dynIden5 = DynMatrix<int>::identity(5);
		DynMatrix<int> dynNegIden5 = -DynMatrix<int>::identity(5);
		DynMatrix<int> dynA(2, 3, { 1, 2, 3, 4, 5, 6 });
		DynMatrix<int> dynB(3, 2, { 7, 8, 9, 10, 11, 12 });
		DynMatrix<int> dynC = dynA.matrixMultiply(dynB);

		startTime = SDL_GetPerformanceCounter();

		DynMatrix<double> dynTimeTestA(9, 9, { 55, 43, 84, 54, 29, 77, 57, 5, 13, 50, 18, 9, 4, 99, 59,
			34, 71, 5, 85, 42, 54, 2, 44, 96, 58, 64, 6, 21, 5, 46, 29, 95, 35, 75, 1, 98, 49, 90,
			44, 39, 18, 34, 74, 73, 25, 7, 26, 89, 6, 56, 51, 57, 83, 54, 8, 52, 13, 82, 89, 32, 42,
			91, 99, 44, 95, 26, 23, 17, 78, 16, 6, 1, 98, 32, 28, 7, 62, 100, 19, 26, 33 });

		DynMatrix<double> dynTimeTestB(9, 9, { 89, 65, 8, 28, 65, 41, 13, 97, 67, 38, 16, 4, 41, 61, 34,
			19, 87, 97, 85, 99, 92, 62, 91, 4, 31, 26, 59, 58, 87, 56, 81, 6, 13, 45, 73, 90, 79,
			23, 23, 84, 61, 74, 70, 6, 96, 11, 59, 58, 66, 66, 84, 24, 52, 69, 27, 94, 33, 34, 64,
			74, 24, 81, 72, 2, 43, 7, 19, 49, 57, 56, 38, 23, 40, 94, 100, 94, 91, 85, 36, 11, 1 });

		cout << "Creation: " << SDL_GetPerformanceCounter() - startTime << endl;

		startTime = SDL_GetPerformanceCounter();

		DynMatrix<double> dynTimeTestC = dynTimeTestA.matrixMultiply(dynTimeTestB);

		cout << "Multiply: " << SDL_GetPerformanceCounter() - startTime << endl;

		cout << dynIden5 << endl;
		cout << dynNegIden5 << endl;

		cout << dynC << endl;

		cout << dynTimeTestC << endl;
#endif

#if TEST_AI
		//OldNeuralNetwork<9, 3> nn;

		//nn.train(VecN<double, 9>({ 1, 2, 3, 4, 5, 6, 7, 8, 9 }), VecN<double, 3>({ 2, 5, 8 }));

		//cout << nn.run(VecN<double, 9>({1, 2, 3, 4, 5, 6, 7, 8, 9})) << endl;

		NeuralNetwork nn({ 5, 4, 3, 2 });
#endif

#if TEST_GUI
		Button button(40, 40, testButton, "main", 16, "Press Me");
		button.setColor(BLUE);
		button.setFrameColor(RED);
		button.setBackgroundColor(GREEN.inverse());
		button.setFrameWidth(5);

		Input input(40, 100, 100, 0, NULL, "main", 16, "Jenny");
		input.setColor(BLUE);
		input.setFrameColor(PURPLE);
		input.setBackgroundColor(PURPLE.inverse());
		input.setFrameWidth(5);

		Input input2(40, 140, 0, 0, NULL, "main", 16, "Jenny");
		input2.setColor(BLUE);
		input2.setFrameColor(PURPLE);
		input2.setBackgroundColor(PURPLE.inverse());
		input2.setFrameWidth(5);

		Slider<int> testSlider(40, 200, 200, 40, 1, 10, 1, testSliderChanged, "main", 16);
		testSlider.setColor(ORANGE.inverse());
		testSlider.setBackgroundColor(WHITE);

		DropDown testDropDown(40, 260, 200, 40, NULL, "main", 16);
		testDropDown.setFrameColor(BLUE);
		testDropDown.setBackgroundColor(BLUE.inverseHue());
		testDropDown.setFrameWidth(5);
		testDropDown.addValue("zero");
		testDropDown.addValue("one");
		testDropDown.addValue("two");
		testDropDown.setValue(1);

		vector<string> soundFiles;
		soundFiles.push_back("NumaNuma.wav");
		soundFiles.push_back("baddisk.wav");

		DropDown testDropDown2(50, 310, 350, 25, &soundFiles, "main", 13);
		testDropDown2.setFrameColor(RED);
		testDropDown2.setBackgroundColor(RED.inverseHue());
		testDropDown2.setFrameWidth(3);
		//TODO background of DropDown overextends from frameWidth
#endif

#if TEST_TIME
		const uint numTimes = 9;
		const char* times[] = {"1:30p", "2:30pm", "3:45am", \
				"12 : 59 a", "melon", "123abc", \
				"18:36", "1:05", "pumpkin"};
		const char* formats[] = {"%h:%m%p", "%h%m%p", "%h %m %p", \
				"%h %m%p", "%y%d", "%h:%m", \
				"%h%m", "%m%s", "%y"};

		for (auto c = 0; c < numTimes; c++){
			cout << times[c] << " is time? " << Counter::isTimeStr(times[c], formats[c]) << endl;
		}

		//if (Counter::isTimeStr(times[6], formats[6])){
		//	cout << "Is time" << endl;
		//}

		//Counter testCounter(Counter::makeTime("11:16p", "%h:%m %p"));
#endif

		cout << "Success!\n";

#if TEST_MUSIC
		soundManager.playMusic();
#endif

		while (!quiter.getDone()){
			eventManager.update();

			screen.setRenderTarget();
			//screen.fill(WHITE);
			//screen.fill(BLACK);
			screen.fill(TAN);

#if TEST_SURF_GEOM
			screen.setDrawColor(Color(192, 0, 0));
			screen.drawPoint(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

			screen.setDrawColor(TURQUIOSE);
			screen.drawLine(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			screen.drawLine(0, SCREEN_HEIGHT, SCREEN_WIDTH, 0);

			screen.setDrawColor(PURPLE);
			screen.drawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, 50, true);
			screen.drawCircle(SCREEN_WIDTH / 2, 3 * SCREEN_HEIGHT / 4, 50, false);

			screen.setDrawColor(LIGHTPINK);
			screen.drawTriangle(SCREEN_WIDTH * 3 / 4, SCREEN_HEIGHT / 2, 120, 100, 225.0f, true);

			screen.setDrawColor(CHARTREUSE);
			screen.drawRect(SCREEN_WIDTH / 16, 15 * SCREEN_HEIGHT / 16, 14 * SCREEN_WIDTH / 16, SCREEN_HEIGHT / 16, false);

			screen.setDrawColor(Color(128, 128, 128, 64));
			screen.drawRect(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 8, 6 * (SCREEN_WIDTH / 8), 6 * (SCREEN_HEIGHT / 8), true);

			screen.setDrawColor(MAGENTA);
			screen.drawDiamond(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 3 / 4, 50, 20, true);
#endif

#if TEST_SURF_CLIP
			clipper.setClip(corners[0]);
			screen.draw(clipper, 0, 0);
			clipper.setClip(corners[1]);
			screen.draw(clipper, SCREEN_WIDTH - 200, 0);
			clipper.setClip(corners[2]);
			screen.draw(clipper, 0, SCREEN_HEIGHT - 200);
			clipper.setClip(corners[3]);
			screen.draw(clipper, SCREEN_WIDTH - 200, SCREEN_HEIGHT - 200);
			clipper.clearClip();
#endif

#if TEST_SURF_ANIM
			anim.update();
			anim2.update();
			anim3.update();

			if (keyListener.checkKeyDown(EventManager::K_UP, 200)){
				anim3.advanceFrame();
			}
			else if (keyListener.checkKeyDown(EventManager::K_DOWN, 200)){
				anim3.reverseFrame();
			}

			screen.draw(anim, 200, 0);
			screen.draw(anim2, 200, 200);
			screen.draw(anim3);
#endif

#if TEST_SURF_MOVE
			if (keyListener.checkKeyDown(EventManager::K_w)){
				star.move(MovableSurface::MOV_UP);
			}
			if (keyListener.checkKeyDown(EventManager::K_s)){
				star.move(MovableSurface::MOV_DOWN);
			}
			if (keyListener.checkKeyDown(EventManager::K_a)){
				star.move(MovableSurface::MOV_LEFT);
			}
			if (keyListener.checkKeyDown(EventManager::K_d)){
				star.move(MovableSurface::MOV_RIGHT);
			}

			star.update();

			screen.draw(star);
#endif

#if TEST_SOUND
			if (keyListener.checkKeyDown(EventManager::K_e)){
				soundManager.playSound("clap");
			}
#endif

#if TEST_MUSIC
			if (keyListener.checkKeyDown(EventManager::K_SPACE, 200)){
				if (soundManager.isMusicPaused()){
					soundManager.resumeMusic();
				}
				else{
					soundManager.pauseMusic();
				}
			}
#endif

#if TEST_PART
			partEngine.update();

			partEngine.render(screen);
#endif

#if TEST_BNDS
			if (keyListener.checkKeyDown(EventManager::K_SPACE, 200)){
				boundsCirc1.setVel(boundsCirc1.getVel() * 1.2f);
				boundsCirc2.setVel(boundsCirc2.getVel() * 1.2f);
			}

			boundsManager.update();

			boundsManager.draw(screen);
#endif

#if TEST_GUI
			screen.draw(button);
			screen.draw(input);
			screen.draw(input2);
			screen.draw(testSlider);
			screen.draw(testDropDown);
			screen.draw(testDropDown2);

			if (keyListener.checkKeyDown(EventManager::K_RIGHT, 200)){
				testSlider++;
			}
			else if (keyListener.checkKeyDown(EventManager::K_LEFT, 200)){
				testSlider--;
			}
#endif

#if TEST_TIME
			//if (testCounter.isTimeUp()){
			//	cout << "Time's up!" << endl;
			//}

			//if (Timer::getTicks() % 15 == 0){
			//	cout << "Time is " << time(NULL) << endl;
			//}
#endif

			fpsSurf.setText(to_string(myWindow.getFPS()) + " fps");
			screen.draw(title, HALF_SCREEN_WIDTH - title.getWidth() / 2, 0);
			screen.draw(fpsSurf, SCREEN_WIDTH - fpsSurf.getWidth(), SCREEN_HEIGHT - fpsSurf.getHeight());

			//screen.draw(image1, 0, 0);

			screen.clearRenderTarget();

			myWindow.draw(screen, 0, 0);

			myWindow.update();
		}

#if TEST_THREADS
		delete nbt1;
		delete nbt2;
#endif

		quitSDLModules();

#if MAIN_TRY
	}
	catch (const Exception& e){
		cerr << e.what() << endl;
		BAIL(1);
	}
#endif

	return 0;
}
#endif