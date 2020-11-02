//Dustin Gehm

#include <iomanip>
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

void initSDLModules() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		cerr << "SDL could not be loaded\n";
		cerr << SDL_GetError();
		BAIL(1);
	}

	uint SDL_IMG_Flags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(SDL_IMG_Flags) & SDL_IMG_Flags)) {
		cerr << "SDL_image could not be loaded\n";
		cerr << IMG_GetError();
		BAIL(1);
	}

	if (TTF_Init() == -1) {
		cerr << "SDL_TTF could not be loaded\n";
		cerr << TTF_GetError();
		BAIL(1);
	}

	if (Mix_OpenAudio(MIXER_AUDIO_FREQ, MIX_DEFAULT_FORMAT, MIXER_AUDIO_CHANNELS, 2048) < 0) {
		cerr << "SDL_mixer could not be loaded\n";
		cerr << Mix_GetError();
		BAIL(1);
	}
}

void quitSDLModules() {
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

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
#include "TileSystem.h"
#include "Timer.h"
#include "Utils.h"
#include "VecN.h"
#include "WindowManager.h"

using std::cerr;
using std::cout;
using std::make_pair;
using std::setfill;
using std::setw;
using std::to_string;

using namespace Bingo;

//catch fatal exceptions
#define MAIN_TRY 1

//Testing blocks
#define TEST_SURF_GEOM 0
#define TEST_SURF_CLIP 0
#define TEST_SURF_ANIM 0
#define TEST_SURF_MOVE 0

#define TEST_SOUND 0
#define TEST_MUSIC 1
#define TEST_FILE 0
#define TEST_FILE_DIAG 0
#define TEST_NBT 0
#define TEST_PART 1
#define TEST_THREADS 0
//come back to testing bounds
#define TEST_BNDS 0
#define TEST_MATS 0
#define TEST_SORT 0
//come back to NeuralNetworks
#define TEST_AI 0
//sub-tests
#define TEST_AI_IMAGER 0
#define TEST_AI_NN 0
#define TEST_AI_GA 0
//TextSurfaces within Slider not working
#define TEST_GUI 0
#define TEST_TIME 0
#define TEST_TILE 0

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

#if TEST_THREADS
void testThreads(NBT_Compound* nbt) {
	ATOMIC_LOCK(ThreadManager::coutLock);

	cout << nbt << endl;

	ATOMIC_UNLOCK(ThreadManager::coutLock);

	NBT_Compound* readNBT = NULL;

	FileManager::getSingleton().openFile(FIL_PATH"Test1.nbt", FileManager::READ);

	readNBT = FileManager::getSingleton().readNBT(FIL_PATH"Test1.nbt");

	FileManager::getSingleton().closeFile(FIL_PATH"Test1.nbt");

	if (readNBT != NULL) {
		ATOMIC_LOCK(ThreadManager::coutLock);

		cout << readNBT << endl;

		ATOMIC_UNLOCK(ThreadManager::coutLock);

		delete readNBT;
	}
}
#endif

#if TEST_GUI
void testButton(Guis::Button& button, EventManager::MouseButton mouseButton) {
	static uint counter = 0;
	button.setText("Press Me Again!");

	cout << "Button Pressed!" << ++counter << endl;
}

void testSliderChanged(Guis::Slider<int>& slider) {
	//cout << "Slider val is " << slider.getValue() << endl;
}

void dropDownChanged(Guis::DropDown& dropDown) {
	cout << "DropDown is " << dropDown.getValue() << endl;
}
#endif

#if TEST_AI_GA
struct LottoNumbers {
	Math::VecN<uint, 6> nums;
};

ostream& operator<<(ostream& os, const LottoNumbers& val) {
	os << "LN{";

	for (uint c = 0; c < 5; c++) {
		os << setw(2) << setfill('0') << val.nums[c] << ", ";
	}

	os << setw(2) << setfill('0') << val.nums[5] << "}";

	return os;
}

LottoNumbers CreateLN() {
	LottoNumbers ln;

	for (uint c = 0; c < 5; c++) {
		ln.nums[c] = RandomManager::getSingleton().randInt(1, 69);
	}

	ln.nums[5] = RandomManager::getSingleton().randInt(1, 26);

	return ln;
}

LottoNumbers MutateLN(const LottoNumbers& val) {
	LottoNumbers ln;

	for (uint c = 0; c < 5; c++) {
		ln.nums[c] = val.nums[c] + RandomManager::getSingleton().randInt(-5, 5);

		if (ln.nums[c] < 1) {
			ln.nums[c] = 1;
		}
		else if (ln.nums[c] > 56) {
			ln.nums[c] = 56;
		}
	}

	ln.nums[5] = val.nums[5] + RandomManager::getSingleton().randInt(-3, 3);

	if (ln.nums[5] < 1) {
		ln.nums[5] = 1;
	}
	else if (ln.nums[5] > 26) {
		ln.nums[5] = 26;
	}

	return ln;
}

int ScoreLN(const LottoNumbers& val) {
	int score = 0;
	int avgNum = 0;

	for (uint c = 0; c < 6; c++) {
		for (uint d = 0; d < 6; d++) {
			if (val.nums[c] == val.nums[d]) {
				score -= 20;
			}
		}

		if ((val.nums[c] % 2) == 0) {
			score += 20;
		}

		if ((val.nums[c] % 5) == 0) {
			score += 10;
		}

		avgNum += val.nums[c];
	}

	avgNum /= 6;

	score -= (abs(avgNum - 30) * 2);

	return score;
}
#endif

/*
TODO PRIORITY 0 add comments and documentation to EVERYTHING!, get Doxygen
TODO while in InputMode from one Input, you can click another Input and "nest" InputMode
TODO when clicking a button on an external dialog "openFileDialog", that click can trigger InputMode on a input below the button
TODO add either string arrays or something else alongside all Enums
TODO find where ever iterators are used and make sure it cant be accomplished using indices
	if you really need the index (e.g. access the previous or next element, printing/logging the index inside the loop etc.) or you need a stride different than 1, then I would go for the explicitly indexed-loop, otherwise I'd go for the range-for loop.

	For generic algorithms on generic containers I'd go for the explicit iterator loop unless the code contained no flow control inside the loop and needed stride 1, in which case I'd go for the STL for_each + a lambda.

TODO switch enums over to enum classes where applicable

TODO add a 3d camera and view
TODO add a way to have tiled backgrounds that scroll

TODO add more capability to the WindowManager for different events, more windows, and multiple displays

TODO add to SoundManager to add capabilities to record audio and save as a playable sound

TODO add mutexes and conditions into ThreadManager once multi-threading stops giving your brain spasms

TODO add bounding box/circle/etc (cube/sphere/etc)

TODO add octtree draw occlusion

TODO maybe add SDL_GetPerformanceCounter to Timer

TODO add logging manager

TODO add max/min bounds to PhysicalObject/Positional
*/
int main(int argc, char* argv[]) {
#if MAIN_TRY
	try {
#endif
		initSDLModules();

		Surfaces::WindowManager myWindow("Testing", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT);
		EventManager eventManager;
		Events::KeyListener keyListener;
		Events::MouseListener mouseListener;
		Events::QuitListener quiter;
		FontManager fontManager(FNT_PATH);
		SoundManager soundManager;
		FileManager fileManager(FIL_PATH);
		ThreadManager threadManager;
		RandomManager randomManager;
		BoundsManager boundsManager;

		Surfaces::Surface screen(SCREEN_WIDTH, SCREEN_HEIGHT);
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

		Surfaces::TextSurface title("main", 36, "Bingo Engine", Colors::BLACK);
		Surfaces::TextSurface fpsSurf("main", 20, "0.0 fps", Colors::BLACK);

#if TEST_TILE
		vector<Math::VecN<int, 4>> tileClips;

		for (int r = 0; r < 3; r++) {
			for (int c = 0; c < 9; c++) {
				tileClips.push_back({ 41 * c, 41 * r, 41, 41 });
			}
		}

		Tiling::TileSystem testTiles("resources/TestLevel.map", IMG_PATH"TestLevelTiles.png", tileClips);
		testTiles.setScreenBounding(true, 41 / 2, { 0, 0, static_cast<int>(SCREEN_WIDTH - testTiles.getWidth()), static_cast<int>(SCREEN_HEIGHT - testTiles.getHeight()) });
		testTiles.setFriction(0.125f);
		Math::VecN<float, 2> testTilesVel({ 50.0f, 50.0f });
		Time::Timer tilesTimer;
#endif

#if TEST_PART
		Particles::ParticleEngine partEngine(IMG_PATH "EleParticle.png", 250, 1.5f, HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT, 75);
		Particles::ErraticBehavior wavy;

		wavy.setFriction(0.0f);
		wavy.setPhysicsBoost(13.0f);
		wavy.setLifeBoost(1.5f);

		//partEngine.setBehavior(&wavy);
		partEngine.setParticleGravity(true);
		partEngine.setParticleBounds(true, { -HALF_SCREEN_WIDTH, -HALF_SCREEN_HEIGHT, HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT });
		partEngine.remakePartices();

		vector<Math::VecN<int, 4>> rotPartClips;
		rotPartClips.push_back({ 0, 0, 4, 4 });
		rotPartClips.push_back({ 0, 4, 4, 4 });
		rotPartClips.push_back({ 0, 8, 4, 4 });
		rotPartClips.push_back({ 0, 12, 4, 4 });

		vector<Math::VecN<int, 4>> bigRotPartClips;
		bigRotPartClips.push_back({ 0, 0, 40, 40 });
		bigRotPartClips.push_back({ 0, 40, 40, 40 });
		bigRotPartClips.push_back({ 0, 80, 40, 40 });
		bigRotPartClips.push_back({ 0, 120, 40, 40 });

		Particles::AnimParticleEngine animPartEngine(IMG_PATH"RotatingParticle.png", rotPartClips, 250, 1.5f, HALF_SCREEN_WIDTH / 2, HALF_SCREEN_HEIGHT, 25);
		Particles::DefaultBehavior animBehav;

		animBehav.setFriction(.007f);
		animBehav.setPhysicsBoost(1.5f);
		animBehav.setLifeBoost(2.0f);

		animPartEngine.setBehavior(&animBehav);
		//animPartEngine.setParticleGravity(true);
		animPartEngine.setParticleBounds(true, { -HALF_SCREEN_WIDTH, -HALF_SCREEN_HEIGHT, HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT });
		animPartEngine.setAnimSpeed(10);
		animPartEngine.remakePartices();

		vector<Math::VecN<int, 4>> fireClips;
		fireClips.push_back({ 5, 2, 13, 21 });
		fireClips.push_back({ 29, 2, 13, 21 });
		fireClips.push_back({ 52, 2, 13, 21 });
		fireClips.push_back({ 75, 2, 13, 21 });
		fireClips.push_back({ 101, 2, 13, 21 });
		fireClips.push_back({ 125, 2, 13, 21 });
		fireClips.push_back({ 148, 2, 13, 21 });

		Particles::AnimParticleEngine firePartEngine(IMG_PATH"FireAnim.png", fireClips, 500, 1.5f, 3 * (HALF_SCREEN_WIDTH / 2), HALF_SCREEN_HEIGHT, -50, 50, 10, -150);
		Particles::DefaultBehavior fireBehav;

		fireBehav.setFriction(0.025f);
		fireBehav.setPhysicsBoost(1.5f);
		fireBehav.setLifeBoost(0.5f);

		firePartEngine.setBehavior(&fireBehav);
		firePartEngine.setParticleBounds(true, { -HALF_SCREEN_WIDTH, -HALF_SCREEN_HEIGHT, HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT });
		firePartEngine.setAnimSpeed(5);
		firePartEngine.remakePartices();
#endif

#if TEST_SURF_MOVE
		Surfaces::MovableSurface star(IMG_PATH"GreenStar.png", 50, SCREEN_HEIGHT / 2, 10.0f, 10.0f);
		star.setGravityEffects(true);
		//TODO this should be changed to width/height instead of radius
		star.setScreenBounding(true, star.getWidth() / 2, { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT });
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

		for (auto s : files) {
			cout << s << '\n';
		}

		cout << "Attempting to remove " FIL_PATH "/TestDir" << endl;

		fileManager.removeDirectoy(FIL_PATH "/TestDir");

		cout << "Does " FIL_PATH "/TestDir still exist? " << fileManager.isDir(FIL_PATH "/TestDir") << endl;

		string testDir = FIL_PATH "DoNotDelete";

		cout << "Attempting to remove " + testDir << endl;

		fileManager.removeDirectoy(testDir);

#if TEST_FILE_DIAG
		vector<string> fileFilter;
		//fileFilter.push_back("*.map");
		//fileFilter.push_back("*.nbt");

		string testSaveFile = fileManager.saveFileDialog("resources/");

		cout << "Selected " << testSaveFile << " to save file.\n";

		string testOpenFile = fileManager.openFileDialog("resources/", &fileFilter);

		cout << "Selected " << testOpenFile << " in diaglog box.\n";

		//vector<string> testFileOpenMult = fileManager.openFileDialogMultiple("resources/");

		//cout << "Selected " << testFileOpenMult << " in dialog box.\n";
#endif
#endif

#if TEST_NBT
		string Test1 = FIL_PATH"Test1.nbt";

		//create NBT
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

		NBT_Compound* nestNBT2 = new NBT_Compound("Nested2");
		nestNBT2->setTag(new NBT_Tag<void*>("Pointer", nestNBT));
		nestNBT2->setTag(new NBT_Tag<vector<int>>("Ids", vector<int>(2)));
		nestNBT2->setTag(new NBT_Tag<Math::DynVecN<double>>("Idk", Math::DynVecN<double>(3, { 0.5, 0.25, 0.125 })));
		nestNBT2->setTag(new NBT_Tag<Math::DynVecN<double>>("Test_DVd", Math::DynVecN<double>(6, { randomManager.randDouble(-1.0, 1.0),
																						randomManager.randDouble(-1.0, 1.0),
																						randomManager.randDouble(-1.0, 1.0),
																						randomManager.randDouble(-1.0, 1.0),
																						randomManager.randDouble(-1.0, 1.0),
																						randomManager.randDouble(-1.0, 1.0) })));

		nestNBT->setTag(nestNBT2);

		nbt->setTag(nestNBT);

		//write NBT to file
		fileManager.createFile(Test1);
		fileManager.openFile(Test1, FileManager::WRITE);
		fileManager.writeNBT(Test1, nbt);
		fileManager.closeFile(Test1);

		//read NBT from file
		NBT_Compound* readNbt = NULL;

		fileManager.openFile(Test1, FileManager::READ);
		readNbt = fileManager.readNBT(Test1);
		fileManager.closeFile(Test1);

		if (readNbt != NULL) {
			cout << readNbt << endl;
		}

		//write NBT to file again
		fileManager.openFile(Test1, FileManager::WRITE);
		fileManager.writeNBT(Test1, readNbt);
		fileManager.closeFile(Test1);

		delete readNbt;

		//read NBT from file again
		fileManager.openFile(Test1, FileManager::READ);
		readNbt = fileManager.readNBT(Test1);
		fileManager.closeFile(Test1);

		if (readNbt != NULL) {
			cout << readNbt << endl;
		}

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
		Surfaces::Surface clipper(IMG_PATH"clipTest.png");
		uint corners[4] = { 0, 0, 0, 0 };
		corners[0] = clipper.addClip(0, 0, 200, 200);
		corners[1] = clipper.addClip(200, 0, 200, 200);
		corners[2] = clipper.addClip(0, 200, 200, 200);
		corners[3] = clipper.addClip(200, 200, 200, 200);
#endif

#if TEST_SURF_ANIM
		Surfaces::AnimSurface anim(IMG_PATH"PulseSquare.png");
		anim.addClip(0, 0, 200, 200);
		anim.addClip(0, 200, 200, 200);
		anim.addClip(0, 400, 200, 200);
		anim.addClip(0, 600, 200, 200);
		anim.setAnimSpeed(8);

		Surfaces::AnimSurface anim2 = anim;
		anim2.setAnimSpeed(6);

		Surfaces::AnimSurface anim3(anim2);
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
		Collision::BoundsCircle boundsCirc1({ 100, 260 }, 10, Colors::RED);
		Collision::BoundsCircle boundsCirc2({ 500, 250 }, 10, Colors::BLUE);
		Collision::BoundsBox boundsBox({ 320, 240 }, 40, 40, Colors::GREEN);
		Collision::BoundsLine boundsLine({ 220, 100 }, { 520, 100 }, Colors::PURPLE);

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
		Math::Matrix<int, 2, 2> twoByTwo(3);
		Math::Matrix<double, 2, 3> twoByThree({ 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f });
		Math::Matrix<int, 5, 5> iden5 = Math::Matrix<int, 5, 5>::identity();
		Math::Matrix<int, 5, 5> negIden5 = -Math::Matrix<int, 5, 5>::identity();
		Math::Matrix<int, 2, 3> a({ 1, 2, 3, 4, 5, 6 });
		Math::Matrix<int, 3, 2> b({ 7, 8, 9, 10, 11, 12 });
		Math::Matrix<int, 2, 2> c = a.matrixMultiply(b);

		Uint64 startTime = SDL_GetPerformanceCounter();

		Math::Matrix<double, 9, 9> timeTestA({ 55, 43, 84, 54, 29, 77, 57, 5, 13, 50, 18, 9, 4, 99, 59,
			34, 71, 5, 85, 42, 54, 2, 44, 96, 58, 64, 6, 21, 5, 46, 29, 95, 35, 75, 1, 98, 49, 90,
			44, 39, 18, 34, 74, 73, 25, 7, 26, 89, 6, 56, 51, 57, 83, 54, 8, 52, 13, 82, 89, 32, 42,
			91, 99, 44, 95, 26, 23, 17, 78, 16, 6, 1, 98, 32, 28, 7, 62, 100, 19, 26, 33 });

		Math::Matrix<double, 9, 9> timeTestB({ 89, 65, 8, 28, 65, 41, 13, 97, 67, 38, 16, 4, 41, 61, 34,
			19, 87, 97, 85, 99, 92, 62, 91, 4, 31, 26, 59, 58, 87, 56, 81, 6, 13, 45, 73, 90, 79,
			23, 23, 84, 61, 74, 70, 6, 96, 11, 59, 58, 66, 66, 84, 24, 52, 69, 27, 94, 33, 34, 64,
			74, 24, 81, 72, 2, 43, 7, 19, 49, 57, 56, 38, 23, 40, 94, 100, 94, 91, 85, 36, 11, 1 });

		cout << "Creation: " << SDL_GetPerformanceCounter() - startTime << endl;

		startTime = SDL_GetPerformanceCounter();

		Math::Matrix<double, 9, 9> timeTestC = timeTestA.matrixMultiply(timeTestB);

		cout << "Multiply: " << SDL_GetPerformanceCounter() - startTime << endl;

		cout << iden5 << endl;
		cout << negIden5 << endl;

		cout << c << endl;

		cout << timeTestC << endl;

		Math::DynMatrix<int> dynTwoByTwo(2, 2, 3);
		Math::DynMatrix<double> dynTwoByThree(2, 3, { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f });
		Math::DynMatrix<int> dynIden5 = Math::DynMatrix<int>::identity(5);
		Math::DynMatrix<int> dynNegIden5 = -Math::DynMatrix<int>::identity(5);
		Math::DynMatrix<int> dynA(2, 3, { 1, 2, 3, 4, 5, 6 });
		Math::DynMatrix<int> dynB(3, 2, { 7, 8, 9, 10, 11, 12 });
		Math::DynMatrix<int> dynC = dynA.matrixMultiply(dynB);

		startTime = SDL_GetPerformanceCounter();

		Math::DynMatrix<double> dynTimeTestA(9, 9, { 55, 43, 84, 54, 29, 77, 57, 5, 13, 50, 18, 9, 4, 99, 59,
			34, 71, 5, 85, 42, 54, 2, 44, 96, 58, 64, 6, 21, 5, 46, 29, 95, 35, 75, 1, 98, 49, 90,
			44, 39, 18, 34, 74, 73, 25, 7, 26, 89, 6, 56, 51, 57, 83, 54, 8, 52, 13, 82, 89, 32, 42,
			91, 99, 44, 95, 26, 23, 17, 78, 16, 6, 1, 98, 32, 28, 7, 62, 100, 19, 26, 33 });

		Math::DynMatrix<double> dynTimeTestB(9, 9, { 89, 65, 8, 28, 65, 41, 13, 97, 67, 38, 16, 4, 41, 61, 34,
			19, 87, 97, 85, 99, 92, 62, 91, 4, 31, 26, 59, 58, 87, 56, 81, 6, 13, 45, 73, 90, 79,
			23, 23, 84, 61, 74, 70, 6, 96, 11, 59, 58, 66, 66, 84, 24, 52, 69, 27, 94, 33, 34, 64,
			74, 24, 81, 72, 2, 43, 7, 19, 49, 57, 56, 38, 23, 40, 94, 100, 94, 91, 85, 36, 11, 1 });

		cout << "Creation: " << SDL_GetPerformanceCounter() - startTime << endl;

		startTime = SDL_GetPerformanceCounter();

		Math::DynMatrix<double> dynTimeTestC = dynTimeTestA.matrixMultiply(dynTimeTestB);

		cout << "Multiply: " << SDL_GetPerformanceCounter() - startTime << endl;

		cout << dynIden5 << endl;
		cout << dynNegIden5 << endl;

		cout << dynC << endl;

		cout << dynTimeTestC << endl;
#endif

#if TEST_SORT
		vector<uint> testVec;
		testVec.push_back(55);
		testVec.push_back(35);
		testVec.push_back(45);
		testVec.push_back(10);
		testVec.push_back(20);
		testVec.push_back(50);
		testVec.push_back(5);
		testVec.push_back(30);
		testVec.push_back(25);
		testVec.push_back(40);
		testVec.push_back(15);

		using Utils::operator<<;

		cout << testVec << endl;

		Sort::selectionSort(testVec);
		Sort::insertionSort(testVec);
		Sort::quickSort(testVec);

		cout << testVec << endl;
#endif

#if TEST_AI
#if TEST_AI_IMAGER
		vector<string> trainFiles = fileManager.listDirectory(IMG_PATH"/NN_train/");
		vector<vector<Surfaces::Surface*>> trainSurfaces;
		vector<vector<Surfaces::TextSurface*>> trainLabels;
		vector<vector<Math::DynVecN<uint>>> trainData;
		Math::VecN<int, 2> trainSurfBasePos = { 30, 30 };
		Math::VecN<int, 2> trainLabelBasePos = { 45, 45 };
		int trainSurfInterval = 40;

		trainSurfaces.resize(10);
		trainLabels.resize(10);
		trainData.resize(10);

		for (auto iter = trainFiles.begin(); iter != trainFiles.end(); iter++) {
			Surfaces::Surface* surf = new Surfaces::Surface(*iter);
			uint index = -1;

			if (iter->find("Zero") != string::npos) {
				index = 0;
			}
			else if (iter->find("One") != string::npos) {
				index = 1;
			}
			else if (iter->find("Two") != string::npos) {
				index = 2;
			}
			else if (iter->find("Three") != string::npos) {
				index = 3;
			}
			else if (iter->find("Four") != string::npos) {
				index = 4;
			}
			else if (iter->find("Five") != string::npos) {
				index = 5;
			}
			else if (iter->find("Six") != string::npos) {
				index = 6;
			}
			else if (iter->find("Seven") != string::npos) {
				index = 7;
			}
			else if (iter->find("Eight") != string::npos) {
				index = 8;
			}
			else if (iter->find("Nine") != string::npos) {
				index = 9;
			}
			else {
				continue;
			}

			trainSurfaces[index].push_back(surf);
			trainLabels[index].push_back(new Guis::Label("main", 16, "0", Colors::BLACK));
			trainData[index].push_back(Math::DynVecN<uint>(16 * 16));

			surf->fetchPixels();

			for (int x = 0; x < surf->getWidth(); x++) {
				for (int y = 0; y < surf->getHeight(); y++) {
					trainData[index].back()[y * 16 + x] = surf->getPixelAt(x, y).getRed() / 200;
				}
			}

			//cout << trainData[index].back() << endl;
		}

		for (uint c = 0; c < trainSurfaces.size(); c++) {
			for (uint d = 0; d < trainSurfaces[c].size(); d++) {
				trainSurfaces[c][d]->setPosX(trainSurfBasePos[0] + (trainSurfInterval * d));
				trainSurfaces[c][d]->setPosY(trainSurfBasePos[1] + (trainSurfInterval * c));

				trainLabels[c][d]->setPosX(trainLabelBasePos[0] + (trainSurfInterval * d));
				trainLabels[c][d]->setPosY(trainLabelBasePos[1] + (trainSurfInterval * c));
			}
		}

		string imagerFile = FIL_PATH"Imager.nbt";
		AI::NeuralNetwork imager(16 * 16, 1);
		imager.setLearningRate(0.5);

		Math::DynVecN<uint> imagerOutput(1);
		uint imagerIndex = 6;
		auto imagerIter = trainData[imagerIndex].begin();
		auto imagerLabelIter = trainLabels[imagerIndex].begin();

		//imager.load(imagerFile);
#endif

#if TEST_AI_NN
		AI::NeuralNetwork nn(4, 3);
		vector<pair<Math::DynVecN<uint>, Math::DynVecN<uint>>> inputs;
		Math::DynVecN<uint> output;

		inputs.push_back(make_pair(Math::DynVecN<uint>(4, { 0, 0, 0, 0, }), Math::DynVecN<uint>(3, { 0, 0, 0 })));
		inputs.push_back(make_pair(Math::DynVecN<uint>(4, { 0, 0, 0, 1, }), Math::DynVecN<uint>(3, { 0, 0, 0 })));
		inputs.push_back(make_pair(Math::DynVecN<uint>(4, { 0, 0, 1, 0, }), Math::DynVecN<uint>(3, { 0, 0, 1 })));
		inputs.push_back(make_pair(Math::DynVecN<uint>(4, { 0, 0, 1, 1, }), Math::DynVecN<uint>(3, { 0, 0, 1 })));
		inputs.push_back(make_pair(Math::DynVecN<uint>(4, { 0, 1, 0, 0, }), Math::DynVecN<uint>(3, { 0, 1, 0 })));
		inputs.push_back(make_pair(Math::DynVecN<uint>(4, { 0, 1, 0, 1, }), Math::DynVecN<uint>(3, { 0, 1, 0 })));
		inputs.push_back(make_pair(Math::DynVecN<uint>(4, { 0, 1, 1, 0, }), Math::DynVecN<uint>(3, { 0, 1, 1 })));
		inputs.push_back(make_pair(Math::DynVecN<uint>(4, { 0, 1, 1, 1, }), Math::DynVecN<uint>(3, { 0, 1, 1 })));
		inputs.push_back(make_pair(Math::DynVecN<uint>(4, { 1, 0, 0, 0, }), Math::DynVecN<uint>(3, { 1, 0, 0 })));
		inputs.push_back(make_pair(Math::DynVecN<uint>(4, { 1, 0, 0, 1, }), Math::DynVecN<uint>(3, { 1, 0, 0 })));
		inputs.push_back(make_pair(Math::DynVecN<uint>(4, { 1, 0, 1, 0, }), Math::DynVecN<uint>(3, { 1, 0, 1 })));
		inputs.push_back(make_pair(Math::DynVecN<uint>(4, { 1, 0, 1, 1, }), Math::DynVecN<uint>(3, { 1, 0, 1 })));
		inputs.push_back(make_pair(Math::DynVecN<uint>(4, { 1, 1, 0, 0, }), Math::DynVecN<uint>(3, { 1, 1, 0 })));
		inputs.push_back(make_pair(Math::DynVecN<uint>(4, { 1, 1, 0, 1, }), Math::DynVecN<uint>(3, { 1, 1, 0 })));
		inputs.push_back(make_pair(Math::DynVecN<uint>(4, { 1, 1, 1, 0, }), Math::DynVecN<uint>(3, { 1, 1, 1 })));
		inputs.push_back(make_pair(Math::DynVecN<uint>(4, { 1, 1, 1, 1, }), Math::DynVecN<uint>(3, { 1, 1, 1 })));

		//nn.load(FIL_PATH"NN_Test.nbt");

		for (uint c = 0; c < 3000; c++) {
			for (auto iter = inputs.begin(); iter != inputs.end(); iter++) {
				nn.train((*iter).first, (*iter).second);
			}
		}

		for (auto iter = inputs.begin(); iter != inputs.end(); iter++) {
			output = nn.run((*iter).first);

			cout << "O(" << (*iter).first << "):\t" << output << endl;
		}

		//const uint answersSize = 16;
		//VecN<uint, 1> answer;
		//uint correctCount = 0;

		//for (uint c = 0; c < answersSize; c++) {
		//	answer = nn.run(DynVecN<uint>(4, { 1, 0, 1, 0, }));

		//	cout << c << ":\t" << answer << endl;

		//	if (answer == DynVecN<uint>(1, { 10, })) {
		//		correctCount++;
		//	}
		//}

		//cout << "Correct " << correctCount << " times.\n";
#endif

#if TEST_AI_GA
		AI::GeneticAlgorithm<LottoNumbers> testGenAlg(100, CreateLN, MutateLN, ScoreLN);

		testGenAlg.advanceGeneration(1000);

		vector<LottoNumbers> results = testGenAlg.getBest(5);

		for (auto iter = results.begin(); iter != results.end(); iter++) {
			cout << *iter << endl;
		}
#endif
#endif

#if TEST_GUI
		Guis::Button button(40, 40, testButton, "main", 16, "Press Me");
		button.setColor(Colors::BLUE);
		button.setFrameColor(Colors::RED);
		button.setBackgroundColor(Colors::GREEN.inverse());
		button.setFrameWidth(5);

		Guis::Input input(40, 100, 100, 0, NULL, "main", 16, "Jenny");
		input.setColor(Colors::BLUE);
		input.setFrameColor(Colors::PURPLE);
		input.setBackgroundColor(Colors::PURPLE.inverse());
		input.setFrameWidth(5);

		Guis::Input input2(40, 140, 0, 0, NULL, "main", 16, "Jenny");
		input2.setColor(Colors::BLUE);
		input2.setFrameColor(Colors::PURPLE);
		input2.setBackgroundColor(Colors::PURPLE.inverse());
		input2.setFrameWidth(5);

		Guis::Slider<int> testSlider(40, 200, 200, 40, 0, 10, 1, testSliderChanged, "main", 16);
		testSlider.setColor(Colors::ORANGE.inverse());
		testSlider.setBackgroundColor(Colors::WHITE);

		Guis::DropDown testDropDown(40, 260, 200, 40, NULL, dropDownChanged, "main", 16);
		testDropDown.setFrameColor(Colors::BLUE);
		testDropDown.setBackgroundColor(Colors::BLUE.inverseHue());
		testDropDown.setFrameWidth(5);
		testDropDown.addValue("zero");
		testDropDown.addValue("one");
		testDropDown.addValue("two");
		testDropDown.setSelection(1);

		vector<string> soundFiles;
		soundFiles.push_back("NumaNuma.wav");
		soundFiles.push_back("baddisk.wav");
		soundFiles.push_back("test.txt");

		Guis::DropDown testDropDown2(50, 310, 350, 25, &soundFiles, dropDownChanged, "main", 13);
		testDropDown2.setFrameColor(Colors::RED);
		testDropDown2.setBackgroundColor(Colors::RED.inverseHue());
		testDropDown2.setFrameWidth(3);
		testDropDown2.setTextPadding(5);
#endif

#if TEST_TIME
		const uint numTimes = 9;
		const char* times[] = { "1:30p", "2:30pm", "3:45am", \
				"12 : 59 a", "melon", "123abc", \
				"18:36", "1:05", "pumpkin"
		};
		const char* formats[] = { "%h:%m%p", "%h%m%p", "%h %m %p", \
				"%h %m%p", "%y%d", "%h:%m", \
				"%h%m", "%m%s", "%y"
		};

		for (auto c = 0; c < numTimes; c++) {
			cout << times[c] << " is time? " << Time::Counter::isTimeStr(times[c], formats[c]) << endl;
		}

		//if (Counter::isTimeStr(times[6], formats[6])){
		//	cout << "Is time" << endl;
		//}

		//Counter testCounter(Counter::makeTime("11:16p", "%h:%m %p"));
#endif

#if TEST_MUSIC
		soundManager.playMusic();
#endif

		cout << "Success!\n";

		while (!quiter.getDone()) {
			eventManager.update();

			screen.setRenderTarget();
			//screen.fill(WHITE);
			//screen.fill(BLACK);
			screen.fill(Colors::TAN);

#if TEST_SURF_GEOM
			screen.setDrawColor(Colors::Color(192, 0, 0));
			screen.drawPoint(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

			screen.setDrawColor(Colors::TURQUIOSE);
			screen.drawLine(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			screen.drawLine(0, SCREEN_HEIGHT, SCREEN_WIDTH, 0);

			screen.setDrawColor(Colors::PURPLE);
			screen.drawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, 50, true);
			screen.drawCircle(SCREEN_WIDTH / 2, 3 * SCREEN_HEIGHT / 4, 50, false);

			screen.setDrawColor(Colors::LIGHTPINK);
			screen.drawTriangle(SCREEN_WIDTH * 3 / 4, SCREEN_HEIGHT / 2, 120, 100, 225.0f, true);

			screen.setDrawColor(Colors::CHARTREUSE);
			screen.drawRect(SCREEN_WIDTH / 16, 15 * SCREEN_HEIGHT / 16, 14 * SCREEN_WIDTH / 16, SCREEN_HEIGHT / 16, false);

			screen.setDrawColor(Colors::Color(128, 128, 128, 64));
			screen.drawRect(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 8, 6 * (SCREEN_WIDTH / 8), 6 * (SCREEN_HEIGHT / 8), true);

			screen.setDrawColor(Colors::MAGENTA);
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

			if (keyListener.checkKeyDown(EventManager::K_UP, 200)) {
				anim3.advanceFrame();
			}
			else if (keyListener.checkKeyDown(EventManager::K_DOWN, 200)) {
				anim3.reverseFrame();
			}

			screen.draw(anim, 200, 0);
			screen.draw(anim2, 200, 200);
			screen.draw(anim3);
#endif

#if TEST_SURF_MOVE
			if (keyListener.checkKeyDown(EventManager::K_w)) {
				star.move(Surfaces::MovableSurface::MOV_UP);
			}
			if (keyListener.checkKeyDown(EventManager::K_s)) {
				star.move(Surfaces::MovableSurface::MOV_DOWN);
			}
			if (keyListener.checkKeyDown(EventManager::K_a)) {
				star.move(Surfaces::MovableSurface::MOV_LEFT);
			}
			if (keyListener.checkKeyDown(EventManager::K_d)) {
				star.move(Surfaces::MovableSurface::MOV_RIGHT);
			}

			star.update();

			screen.draw(star);
#endif

#if TEST_SOUND
			if (keyListener.checkKeyDown(EventManager::K_e)) {
				soundManager.playSound("clap");
				//soundManager.playSound("disk");
				//soundManager.playSound("bloop");
			}
#endif

#if TEST_MUSIC
			if (keyListener.checkKeyDown(EventManager::K_SPACE, 200)) {
				if (soundManager.isMusicPaused()) {
					soundManager.resumeMusic();
				}
				else {
					soundManager.pauseMusic();
				}
			}
#endif

#if TEST_PART
			if (keyListener.checkKeyDown(EventManager::K_RIGHT)) {
				fireBehav.setPhysicsBoost(fireBehav.getPhysicsBoost() + 0.1f);
			}
			else if (keyListener.checkKeyDown(EventManager::K_LEFT)) {
				fireBehav.setPhysicsBoost(fireBehav.getPhysicsBoost() - 0.1f);
			}

			if (keyListener.checkKeyDown(EventManager::K_UP)) {
				fireBehav.setLifeBoost(fireBehav.getLifeBoost() + 0.1f);
			}
			else if (keyListener.checkKeyDown(EventManager::K_DOWN)) {
				fireBehav.setLifeBoost(fireBehav.getLifeBoost() - 0.1f);
			}

			if (keyListener.checkKeyDown(EventManager::K_SPACE, 1000)) {
				if (fireBehav.getFriction() > 0.0) {
					fireBehav.setFriction(0.0f);
				}
				else {
					fireBehav.setFriction(0.25f);
				}
			}

			partEngine.update();
			animPartEngine.update();
			firePartEngine.update();

			partEngine.render(screen);
			animPartEngine.render(screen);
			firePartEngine.render(screen);
#endif

#if TEST_BNDS
			if (keyListener.checkKeyDown(EventManager::K_SPACE, 200)) {
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

			if (keyListener.checkKeyDown(EventManager::K_RIGHT, 200)) {
				testSlider++;
			}
			else if (keyListener.checkKeyDown(EventManager::K_LEFT, 200)) {
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

#if TEST_AI
#if TEST_AI_IMAGER
			//imagerOutput = imager.train(*imagerIter, DynVecN<uint>(1, { imagerIndex }));

			(*imagerLabelIter)->setText(to_string(output[0]));
			(*imagerLabelIter)->setColor(Colors::RED);

			for (uint c = 0; c < trainSurfaces.size(); c++) {
				for (uint d = 0; d < trainSurfaces[c].size(); d++) {
					screen.draw(*trainSurfaces[c][d]);

					screen.draw(*trainLabels[c][d]);
				}
			}

			(*imagerLabelIter)->setColor(Colors::BLACK);

			imagerIter++;
			imagerLabelIter++;

			if (imagerIter == trainData[imagerIndex].end()) {
				//imagerIndex = (imagerIndex + 1) % trainData.size();

				imagerIter = trainData[imagerIndex].begin();
				imagerLabelIter = trainLabels[imagerIndex].begin();
			}
#endif
#endif

#if TEST_TILE
			if (keyListener.checkKeyDown(EventManager::K_w, 200)) {
				testTiles.addForceY(-testTilesVel[1]);
			}
			if (keyListener.checkKeyDown(EventManager::K_s, 200)) {
				testTiles.addForceY(testTilesVel[1]);
			}
			if (keyListener.checkKeyDown(EventManager::K_a, 200)) {
				testTiles.addForceX(-testTilesVel[0]);
			}
			if (keyListener.checkKeyDown(EventManager::K_d, 200)) {
				testTiles.addForceX(testTilesVel[0]);
			}

			tilesTimer.end();
			tilesTimer.start();

			testTiles.update(TimeConvert::ms_to_sec(tilesTimer.diff()));
			testTiles.render(screen);
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

#if TEST_AI
#if TEST_AI_IMAGER
		for (uint index = 0; index < trainSurfaces.size(); index++) {
			for (auto iter = trainSurfaces[index].begin(); iter != trainSurfaces[index].end(); iter++) {
				delete (*iter);
			}
		}

		//imager.save(imagerFile);
#endif

#if TEST_AI_NN
		//nn.save(FIL_PATH"NN_Test.nbt");
#endif
#endif

		quitSDLModules();

#if MAIN_TRY
	}
	catch (const Exception& e) {
		cerr << e.what() << endl;
		BAIL(1);
	}
#endif

	return 0;
}