//Dustin Gehm

#include "pch.h"
#include "TestAi.h" // TODO 1 failed test, make AStar tests
#include "TestAnimSurface.h"
#include "TestColor.h"
#include "TestCompassDirection.h"
#include "TestFileManager.h" // interactive
#include "TestMaths.h"
#include "TestMatrix.h"
#include "TestMeshManager.h" // TODO working on this
#include "TestMinHeap.h"
#include "TestNBT.h"
#include "TestPhysicalObject.h"
#include "TestPositional.h"
#include "TestQuaternion.h"
#include "TestRandomManager.h"
#include "TestScriptManager.h"
#include "TestSingleton.h"
#include "TestSurface.h" // TODO 2 test fails, more tests to write
#include "TestTextSurface.h"
#include "TestTimer.h"
#include "TestUtils.h"
#include "TestVecN.h"
#include "TestWindowManager.h" // TODO more tests to write

#include <iostream>

#include "Color.h"
#include "EventManager.h"
#include "FileManager.h"
#include "FontManager.h"
#include "MeshManager.h"
#include "RandomManager.h"
#include "ScriptManager.h"
#include "Surface.h"
#include "TextSurface.h"
#include "WindowManager.h"
#include "Utils.h"

using namespace std;

constexpr auto PROJ_NAME = "Unit Tests";
constexpr auto SCREEN_WIDTH = 600;
constexpr auto SCREEN_HEIGHT = 400;

int main(int argc, char* argv[]) {
	int result = 0;
	Bingo::FileManager fileManager("files");
	Bingo::Surfaces::WindowManager windowManager(PROJ_NAME, 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT);
	Bingo::EventManager eventManager;
	Bingo::Events::QuitListener quitListener;
	Bingo::FontManager fontMan("fonts");
	Bingo::RandomManager randMan;
	Bingo::MeshManager meshMan("models");
	Bingo::Script::Lua::ScriptManager_Lua scriptManLua("scripts/lua");
#if BINGO_SCRIPT_LANG_PY
	Bingo::Script::Python::ScriptManager_Py scriptManPy("scripts/python");
#endif

	fontMan.nickname("expressway rg.ttf", "main");
	fontMan.nickname("Rudiment.ttf", "other");

	Bingo::Surfaces::Surface screen(SCREEN_WIDTH, SCREEN_HEIGHT);
	Bingo::Surfaces::TextSurface testMessage("main", 20, "TESTS COMPLETED", Bingo::Colors::WHITE);

	screen.setRenderTarget();

	// Gtest
	{
		testing::InitGoogleTest(&argc, argv);

		// this causes a linking error
		//testing::FLAGS_gtest_filter = "*MeshManager.*";

		result = RUN_ALL_TESTS();

		if (result) {
			testMessage.setText("TESTS NOT COMPLETED");
		}
	}

	screen.draw(testMessage,
		(SCREEN_WIDTH / 2) - (testMessage.getWidth() / 2),
		SCREEN_HEIGHT - (int)(testMessage.getHeight() * 1.5f));

	screen.clearRenderTarget();

	windowManager.setDrawColor(Bingo::Colors::BLACK);

	while (!quitListener.getDone()) {
		eventManager.update();

		windowManager.clear();
		windowManager.draw(screen, 0, 0);

		windowManager.update();
	}

	return result;
}