#include <iomanip>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "AI.h"
#include "EventManager.h"
#include "FileManager.h"
#include "Matrix.h"
#include "Nbt.h"
#include "RandomManager.h"
#include "VecN.h"
#include "WindowManager.h"
#include "Utils.h"

#include <Windows.h>

using namespace std;

using Bingo::AI::GeneticAlgorithm;
using Bingo::Events::QuitListener;
using Bingo::FileManager;
using Bingo::Math::Matrix;
using Bingo::Math::VecN;
using Bingo::NBT_Compound;
using Bingo::NBT_Tag;
using Bingo::RandomManager;
using Bingo::Surfaces::WindowManager;

const int NUM_BLOCKS = 18;
const int NUM_ROOMS = 4;

enum ClassType : int {
	M_INTERVENTION,
	FITNESS,
	SCIENCE,
	MATH,
	READING,
	WRITING,
	T1_INTERVENTION,
	SOCIAL_STUDIES,
	LUNCH,
	TRANSITION
};

enum ClassLocation : int {
	R_7A = 0,
	R_7B = 1,
	R_8A = 2,
	R_8B = 3
};

struct Schedule_2021 {
	static const VecN<int, NUM_BLOCKS> BlockLengths;
	Matrix<ClassType, NUM_BLOCKS, NUM_ROOMS> data;
};

template<>
Bingo::NBT_Base::NBT_Type NBT_Tag<VecN<ClassType, NUM_BLOCKS>>::getType() {
	return NBT_INT_ARRAY;
}

template<>
void NBT_Tag<VecN<ClassType, NUM_BLOCKS>>::writeData(SDL_RWops* file) {
	int nameSize = name.size();
	int tagSize = 18;

	SDL_RWwrite(file, &type, sizeof(type), 1);
	SDL_RWwrite(file, &nameSize, SZ_INT, 1);
	SDL_RWwrite(file, name.data(), SZ_CHAR, nameSize);
	SDL_RWwrite(file, &tagSize, SZ_INT, 1);
	SDL_RWwrite(file, data.data(), SZ_INT, tagSize);
}

const VecN<int, NUM_BLOCKS> Schedule_2021::BlockLengths = VecN<int, NUM_BLOCKS>({ 5, 40, 5, 40, 5, 50, 5, 50, 5, 50, 15, 5, 25, 5, 40, 55, 5, 30 });

ostream& operator <<(ostream& os, const Schedule_2021& sched) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	os << "Schedule 2021\t7A\t\t7B\t\t8A\t\t8B\n";

	for (size_t i = 0; i < NUM_BLOCKS; i++) {
		os << "Block " << setw(2) << setfill('0') << i << '\t';

		if (i % 2 == 1) {
			SetConsoleTextAttribute(console, 112);
		}
		else {
			SetConsoleTextAttribute(console, 8);
		}

		for (size_t j = 0; j < NUM_ROOMS; j++) {
			switch (sched.data.get(i, j)) {
			case M_INTERVENTION:
				os << "Math Int\t";
				break;
			case FITNESS:
				os << "Fitness\t\t";
				break;
			case SCIENCE:
				os << "Science\t\t";
				break;
			case MATH:
				os << "Math\t\t";
				break;
			case READING:
				os << "Reading\t\t";
				break;
			case WRITING:
				os << "Writing\t\t";
				break;
			case T1_INTERVENTION:
				os << "Title 1\t\t";
				break;
			case SOCIAL_STUDIES:
				os << "Social Studies\t";
				break;
			case LUNCH:
				os << "Lunch\t\t";
				break;
			case TRANSITION:
				os << "Transition\t";
				break;
			default:
				os << "Error\t";
				break;
			}
		}

		SetConsoleTextAttribute(console, 7);

		os << '\n';
	}

	return os;
}

Schedule_2021 create() {
	Schedule_2021 sched;
	RandomManager& randMan = RandomManager::getSingleton();

	for (size_t i = 0; i < NUM_ROOMS; i++) {
		for (size_t j = 0; j < NUM_BLOCKS; j++) {
			sched.data.get(j, i) = static_cast<ClassType>(randMan.randInt(0, 7));
		}

		sched.data.get(12, i) = LUNCH;

		for (int block : { 0, 2, 4, 6, 8, 11, 13, 16 }) {
			sched.data.get(block, i) = TRANSITION;
		}
	}

	sched.data.get(3, R_7A) = FITNESS;
	sched.data.get(3, R_7B) = FITNESS;

	sched.data.get(1, R_8A) = FITNESS;
	sched.data.get(1, R_8B) = FITNESS;

	return sched;
}

Schedule_2021 crossover(const Schedule_2021& sched1, const Schedule_2021& sched2) {
	RandomManager& randMan = RandomManager::getSingleton();
	Schedule_2021 newSched;

	for (uint i = 0; i < NUM_BLOCKS; i++) {
		for (uint j = 0; j < NUM_ROOMS; j++) {
			if (randMan.randBool()) {
				newSched.data.get(i, j) = sched1.data.get(i, j);
			}
			else {
				newSched.data.get(i, j) = sched2.data.get(i, j);
			}
		}
	}

	return newSched;
}

Schedule_2021 mutate(const Schedule_2021& sched) {
	Schedule_2021 newSched(sched);
	RandomManager& randMan = RandomManager::getSingleton();

	for (int block : {1, 3, 5, 7, 9, 10, 14, 15, 17}) {
		ClassLocation room = static_cast<ClassLocation>(randMan.randInt(0, 3));

		if (randMan.randInt(0, 4) == 0) {
			newSched.data.get(block, room) = static_cast<ClassType>(randMan.randInt(0, 7));
		}
	}

	newSched.data.get(3, R_7A) = FITNESS;
	newSched.data.get(3, R_7B) = FITNESS;
	newSched.data.get(1, R_8A) = FITNESS;
	newSched.data.get(1, R_8B) = FITNESS;

	return newSched;
}

//optimal score: 9,280
int score(const Schedule_2021& sched) {
	int score = 0;

	//Set time blocks
	if (sched.data.get(3, R_7A) != FITNESS) {
		score -= 100;
	}

	if (sched.data.get(3, R_7B) != FITNESS) {
		score -= 100;
	}

	if (sched.data.get(1, R_8A) != FITNESS) {
		score -= 100;
	}

	if (sched.data.get(1, R_8B) != FITNESS) {
		score -= 100;
	}

	for (size_t i = 0; i < NUM_ROOMS; i++) {
		if (sched.data.get(12, i) != LUNCH) {
			score -= 25;
		}

		for (int block : { 0, 2, 4, 6, 8, 11, 13, 16 }) {
			if (sched.data.get(block, i) != TRANSITION) {
				score -= 25;
			}
		}
	}

	//homeroom at end of day
	if (sched.data.get(17, R_8B) == MATH) {
		score += 20;
	}

	if (sched.data.get(17, R_8A) == SCIENCE || sched.data.get(17, R_8A) == WRITING) {
		score += 20;
	}

	if (sched.data.get(17, R_7A) == READING || sched.data.get(17, R_7A) == WRITING) {
		score += 20;
	}

	if (sched.data.get(17, R_7B) == SOCIAL_STUDIES || sched.data.get(17, R_7B) == M_INTERVENTION) {
		score += 20;
	}

	//none of the same class in 1 block
	for (size_t block = 0; block < NUM_BLOCKS; block++) {
		for (size_t room = 0; room < NUM_ROOMS; room++) {
			ClassType clazz = sched.data.get(block, room);

			if (clazz != FITNESS && clazz != LUNCH && clazz != TRANSITION) {
				if (clazz != sched.data.get(block, (room + 1) % 4) &&
					clazz != sched.data.get(block, (room + 2) % 4) &&
					clazz != sched.data.get(block, (room + 3) % 4)) {
					score += 100;
				}
			}
		}
	}

	//ensure the classes have at least the required time per day
	int mathTime = 0;
	int englishTime = 0;
	int scienceTime = 0;
	int socialTime = 0;
	int interventionTime = 0;

	for (size_t i = 0; i < NUM_ROOMS; i++) {
		mathTime = 0;
		englishTime = 0;
		scienceTime = 0;
		socialTime = 0;
		interventionTime = 0;

		for (size_t j = 0; j < NUM_BLOCKS; j++) {
			ClassType type = sched.data.get(j, i);

			if (type == MATH || type == M_INTERVENTION) {
				mathTime += Schedule_2021::BlockLengths[j];
			}
			else if (type == READING || type == WRITING) {
				englishTime += Schedule_2021::BlockLengths[j];
			}
			else if (type == SCIENCE) {
				scienceTime += Schedule_2021::BlockLengths[j];
			}
			else if (type == SOCIAL_STUDIES) {
				socialTime += Schedule_2021::BlockLengths[j];
			}
			else if (type == T1_INTERVENTION) {
				interventionTime += Schedule_2021::BlockLengths[j];
			}
		}

		if (mathTime > 90) {
			score += 100;
		}

		if (englishTime > 120) {
			score += 100;
		}

		if (scienceTime > 50) {
			score += 100;
		}

		if (socialTime > 50) {
			score += 100;
		}

		if (interventionTime > 0) {
			score += 100;
		}
	}

	//adjacent classes get bonus

	return score;
}

int main(int argc, char** argv) {
	FileManager fileMan("./");
	RandomManager randMan;

	const string saveFile = "bestSched.nbt";
	int scoreThreshold = 4500;
	double mutateMod = 0.01;

	GeneticAlgorithm<Schedule_2021> ga(90, create, crossover, mutate, score);
	ga.setEvolveRatio(.10);
	ga.setCrossoverRatio(.80);
	ga.setMutateRatio(.25);

	vector<Schedule_2021> best;
	int loadedScore = INT_MIN;
	bool done = false;

	{
		Schedule_2021 bestSched;

		if (!fileMan.checkFile(saveFile)) {
			fileMan.createFile(saveFile);
		}

		fileMan.openFile(saveFile, FileManager::FileMode::READ);

		NBT_Compound* loadRoot = fileMan.readNBT(saveFile);

		fileMan.closeFile(saveFile);

		if (loadRoot) {
			auto* data = static_cast<NBT_Compound*>(loadRoot->getTag("data"));

			for (uint i = 0; i < NUM_ROOMS; i++) {
				bestSched.data.setCol(i, static_cast<NBT_Tag<vector<ClassType>>*>(data->getTag("col_" + to_string(i)))->getData());
			}
		}

		loadedScore = score(bestSched);
		scoreThreshold = MAX(loadedScore, scoreThreshold);

		cout << loadedScore << '\n';
		cout << bestSched << '\n';

		for (uint c = 0; c < 10; c++) {
			ga.insertEntity(bestSched);
		}
	}

	cout << "Calculating";

	while (!done) {
		ga.advanceGeneration(1000);

		cout << '.';

		best = ga.getBest(2);

		cout << score(best[0]);

		if (score(best[0]) > scoreThreshold) {
			done = true;
		}

		double mutateRatio = ga.getMutateRatio();

		if (mutateRatio > .9) {
			mutateRatio = 0.1;
		}

		ga.setMutateRatio(mutateRatio + mutateMod);
	}

	cout << '\n';

	for (Schedule_2021 sched : best) {
		cout << score(sched) << '\n';
		cout << sched << '\n';
	}

	if (score(best[0]) > loadedScore) {
		NBT_Compound* saveRoot = new NBT_Compound("Schedule_2021");
		NBT_Compound* data = new NBT_Compound("data");

		for (size_t i = 0; i < NUM_ROOMS; i++) {
			auto* col = new NBT_Tag<VecN<ClassType, NUM_BLOCKS>>("col_" + to_string(i), best[0].data.getCol(i));

			data->setTag(col);
		}

		saveRoot->setTag(data);

		fileMan.openFile(saveFile, FileManager::FileMode::WRITE);
		fileMan.writeNBT(saveFile, saveRoot);
		fileMan.closeFile(saveFile);
	}

	//HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//// you can loop k higher to see more color choices
	//for (int k = 1; k < 255; k++)
	//{
	//	// pick the colorattribute k you want
	//	SetConsoleTextAttribute(hConsole, k);
	//	cout << k << " I want to be nice today!" << endl;
	//}

	cin.get();

	return 0;
}