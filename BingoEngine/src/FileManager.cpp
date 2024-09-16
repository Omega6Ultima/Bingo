//Dustin Gehm

//supressing warnings about tr2::filesystem
//#define _SILENCE_TR2_SYS_NAMESPACE_DEPRECATION_WARNING

#include "FileManager.h"

#include <filesystem>
#include <sstream>

#include <SDL_timer.h>
#include <tinyfiledialogs.h>

#include "Exception.h"
#include "ThreadManager.h"

//TODO add error handling (Error/Warn)
//TODO consider using ifstream/ofstream instead of SDL_RWops
//consider keeping track of created NBT when reading file and deleting them in destructor/flushCache func

Bingo::FileManager::FileManager(std::string configPath) {
	configFile = configPath + "/FileMan.config";

	if (!checkFile(configFile)) {
		createFile(configFile);
	}
	else if (getFileSize(configFile) < 1) {
		//file is blank, dont read
	}
	else {
		openFile(configFile, READ);

		NBT_Compound* nbt = readNBT(configFile);

		closeFile(configFile);

		NBT_Compound* dirs = static_cast<NBT_Compound*>(nbt->getTag("CreatedDirs"));

		for (uint c = 0; c < dirs->getTagCount(); c++) {
			directories[static_cast<NBT_Tag<std::string>*>(dirs->getTag(c))->getData()] = false;
		}

		delete nbt;
	}
}

Bingo::FileManager::~FileManager() {
	ATOMIC_LOCK(ThreadManager::fileLock);

	for (auto iter = files.begin(); iter != files.end(); iter++) {
		SDL_RWclose((iter->second).second);
	}

	ATOMIC_UNLOCK(ThreadManager::fileLock);

	NBT_Compound* nbt = new NBT_Compound("FileMan.config");
	NBT_Compound* dirs = new NBT_Compound("CreatedDirs");

	nbt->setTag(dirs);

	uint dirCount = 0;

	for (std::pair<std::string, bool> keyValue : directories) {
		dirs->setTag(new NBT_Tag<std::string>("directory" + dirCount, keyValue.first));
	}

	openFile(configFile, WRITE);

	writeNBT(configFile, nbt);

	closeFile(configFile);

	delete nbt;
}

void Bingo::FileManager::createFile(std::string fileName) {
	ATOMIC_LOCK(ThreadManager::fileLock);

	SDL_RWops* file = SDL_RWFromFile(fileName.c_str(), "w");

	if (file == NULL) {
		Bingo::Utils::Error("Could not create file: " + fileName, SDL_GetError());

		return;
	}

	SDL_RWclose(file);

	ATOMIC_UNLOCK(ThreadManager::fileLock);
}

bool Bingo::FileManager::checkFile(std::string fileName) {
	bool success = false;

	ATOMIC_LOCK(ThreadManager::fileLock);

	SDL_RWops* file = SDL_RWFromFile(fileName.c_str(), "r");

	if (file != NULL) {
		success = true;
		SDL_RWclose(file);
	}

	ATOMIC_UNLOCK(ThreadManager::fileLock);

	return success;
}

llong Bingo::FileManager::getFileSize(std::string fileName) {
	llong result = -1;

	if (checkFile(fileName)) {
		ATOMIC_LOCK(ThreadManager::fileLock);

		SDL_RWops* file = SDL_RWFromFile(fileName.c_str(), "r");

		if (file != NULL) {
			result = SDL_RWsize(file);
			SDL_RWclose(file);
		}

		ATOMIC_UNLOCK(ThreadManager::fileLock);
	}

	return result;
}

void Bingo::FileManager::openFile(std::string fileName, FileMode mode) {
	std::string openMode;

	switch (mode) {
	case READ:
		openMode = "rb";
		break;
	case WRITE:
		openMode = "wb";
		break;
	case APPEND:
		openMode = "ab";
		break;
	case READ_WRITE:
		openMode = "r+b";
		break;
	case READ_APPEND:
		openMode = "a+b";
		break;
	default:
		Bingo::Utils::Error("Invalid FileMode for opening file: " + mode, fileName);
	}

	ATOMIC_LOCK(ThreadManager::fileLock);

	SDL_RWops* file = SDL_RWFromFile(fileName.c_str(), openMode.c_str());

	ATOMIC_UNLOCK(ThreadManager::fileLock);

	if (file == NULL) {
		throw Exception("File could not be opened: " + fileName + "\n" + SDL_GetError());
	}

	files[fileName] = std::pair<FileMode, SDL_RWops*>(mode, file);
}

void Bingo::FileManager::seekFile(std::string fileName, int offset) {
	if (files.find(fileName) != files.end()) {
		llong result = 0;

		result = SDL_RWseek(files[fileName].second, offset, RW_SEEK_SET);

		if (result == -1) {
			std::stringstream message;
			message << "Invalid offset(" << offset << ") used in seeking";
			throw Exception(message);
		}
	}
}

void Bingo::FileManager::writeString(std::string fileName, std::string content) {
	if (files.find(fileName) != files.end()) {
		ATOMIC_LOCK(ThreadManager::fileLock);

		SDL_RWwrite(files[fileName].second, content.data(), SZ_CHAR, content.size());

		ATOMIC_UNLOCK(ThreadManager::fileLock);
	}
}

std::string Bingo::FileManager::readString(std::string fileName) {
	std::string result = "";

	if (files.find(fileName) != files.end()) {
		ATOMIC_LOCK(ThreadManager::fileLock);

		llong fileSize = SDL_RWsize(files[fileName].second);

		if (fileSize > 0) {
			char* stringData = new char[static_cast<uint>(fileSize + 1)];
			memset(stringData, 0, static_cast<uint>(fileSize + 1));

			SDL_RWread(files[fileName].second, stringData, SZ_CHAR, static_cast<size_t>(fileSize));

			result = stringData;

			delete[] stringData;
		}

		ATOMIC_UNLOCK(ThreadManager::fileLock);
	}

	return result;
}

void Bingo::FileManager::writeNBT(std::string fileName, NBT_Compound* nbt) {
	if (files.find(fileName) != files.end()) {
		ATOMIC_LOCK(ThreadManager::fileLock);

		nbt->writeData(files[fileName].second);

		ATOMIC_UNLOCK(ThreadManager::fileLock);
	}
}

Bingo::NBT_Compound* Bingo::FileManager::readNBT(std::string fileName) {
	NBT_Compound* nbt = NULL;

	if (files.find(fileName) != files.end()) {
		NBT_Base::NBT_Type type;

		ATOMIC_LOCK(ThreadManager::fileLock);

		int numRead = SDL_RWread(files[fileName].second, &type, sizeof(NBT_Base::NBT_Type), 1);

		ATOMIC_UNLOCK(ThreadManager::fileLock);

		if (numRead == 0) {
			return nbt;
		}

		if (type != NBT_Base::NBT_COMPOUND) {
			Bingo::Utils::Error("NBT file is not in the correct format. Maybe its corrupted?", fileName);
		}
		else {
			int cmpdNameSize = 0;
			char cmpdName[NBT_MAX_NAME] = {};

			//memset(cmpdName, 0, NBT_MAX_NAME);

			ATOMIC_LOCK(ThreadManager::fileLock);

			SDL_RWread(files[fileName].second, &cmpdNameSize, SZ_INT, 1);
			SDL_RWread(files[fileName].second, cmpdName, SZ_CHAR, cmpdNameSize);

			ATOMIC_UNLOCK(ThreadManager::fileLock);

			nbt = new NBT_Compound(cmpdName);

			readNBT_Tag(files[fileName].second, nbt);
		}
	}

	return nbt;
}

void Bingo::FileManager::readNBT_Tag(SDL_RWops* file, NBT_Compound* nbt) {
	NBT_Base::NBT_Type type;
	int nameSize = 0;
	char nameArr[NBT_MAX_NAME];
	union NBT_Data {
		bool boolData;
		char charData;
		uchar ucharData;
		short shortData;
		ushort ushortData;
		int intData;
		uint uintData;
		long longData;
		ulong ulongData;
		llong llongData;
		ullong ullongData;
		float floatData;
		double doubleData;
		ldouble ldoubleData;

		struct {
			int stringSize;
			char stringData[NBT_MAX_NAME];
		};

		void* pointerData;
	} data;

	do {
		memset(nameArr, 0, NBT_MAX_NAME);
		memset(&data, 0, sizeof(NBT_Data));

		ATOMIC_LOCK(ThreadManager::fileLock);

		SDL_RWread(file, &type, sizeof(NBT_Base::NBT_Type), 1);

		ATOMIC_UNLOCK(ThreadManager::fileLock);

		if (type == NBT_Base::NBT_COMPOUND) {
			int cmpdNameSize = 0;
			char cmpdName[NBT_MAX_NAME];
			memset(cmpdName, 0, NBT_MAX_NAME);

			ATOMIC_LOCK(ThreadManager::fileLock);

			SDL_RWread(file, &cmpdNameSize, SZ_INT, 1);
			SDL_RWread(file, cmpdName, SZ_CHAR, cmpdNameSize);

			ATOMIC_UNLOCK(ThreadManager::fileLock);

			NBT_Compound* cmpd = new NBT_Compound(cmpdName);

			readNBT_Tag(file, cmpd);

			nbt->setTag(cmpd);
		}
		else if (type == NBT_Base::NBT_COMPOUND_END) {
			return;
		}
		else {
			ATOMIC_LOCK(ThreadManager::fileLock);

			SDL_RWread(file, &nameSize, sizeof(nameSize), 1);
			SDL_RWread(file, nameArr, SZ_CHAR, nameSize);

			switch (type) {
			case NBT_Base::NBT_BOOL:
				SDL_RWread(file, &data.boolData, SZ_BOOL, 1);

				nbt->setTag(new NBT_Tag<bool>(nameArr, data.boolData));

				break;
			case NBT_Base::NBT_CHAR:
				SDL_RWread(file, &data.charData, SZ_CHAR, 1);

				nbt->setTag(new NBT_Tag<char>(nameArr, data.charData));

				break;
			case NBT_Base::NBT_UCHAR:
				SDL_RWread(file, &data.ucharData, SZ_UCHAR, 1);

				nbt->setTag(new NBT_Tag<uchar>(nameArr, data.ucharData));

				break;
			case NBT_Base::NBT_SHORT:
				SDL_RWread(file, &data.shortData, SZ_SHORT, 1);

				nbt->setTag(new NBT_Tag<short>(nameArr, data.shortData));

				break;
			case NBT_Base::NBT_USHORT:
				SDL_RWread(file, &data.ushortData, SZ_USHORT, 1);

				nbt->setTag(new NBT_Tag<ushort>(nameArr, data.ushortData));

				break;
			case NBT_Base::NBT_INT:
				SDL_RWread(file, &data.intData, SZ_INT, 1);

				nbt->setTag(new NBT_Tag<int>(nameArr, data.intData));

				break;
			case NBT_Base::NBT_UINT:
				SDL_RWread(file, &data.uintData, SZ_UINT, 1);

				nbt->setTag(new NBT_Tag<uint>(nameArr, data.uintData));

				break;
			case NBT_Base::NBT_LONG:
				SDL_RWread(file, &data.longData, SZ_LONG, 1);

				nbt->setTag(new NBT_Tag<long>(nameArr, data.longData));

				break;
			case NBT_Base::NBT_ULONG:
				SDL_RWread(file, &data.ulongData, SZ_ULONG, 1);

				nbt->setTag(new NBT_Tag<ulong>(nameArr, data.ulongData));

				break;
			case NBT_Base::NBT_LLONG:
				SDL_RWread(file, &data.llongData, SZ_LLONG, 1);

				nbt->setTag(new NBT_Tag<llong>(nameArr, data.llongData));

				break;
			case NBT_Base::NBT_ULLONG:
				SDL_RWread(file, &data.ullongData, SZ_ULLONG, 1);

				nbt->setTag(new NBT_Tag<ullong>(nameArr, data.ullongData));

				break;
			case NBT_Base::NBT_FLOAT:
				SDL_RWread(file, &data.floatData, SZ_FLOAT, 1);

				nbt->setTag(new NBT_Tag<float>(nameArr, data.floatData));

				break;
			case NBT_Base::NBT_DOUBLE:
				SDL_RWread(file, &data.doubleData, SZ_DOUBLE, 1);

				nbt->setTag(new NBT_Tag<double>(nameArr, data.doubleData));

				break;
			case NBT_Base::NBT_STRING:
				SDL_RWread(file, &data.stringSize, SZ_INT, 1);
				SDL_RWread(file, data.stringData, SZ_CHAR, data.stringSize);

				nbt->setTag(new NBT_Tag<std::string>(nameArr, data.stringData));

				break;
			case NBT_Base::NBT_POINTER:
				SDL_RWread(file, &data.pointerData, SZ_POINTER, 1);

				nbt->setTag(new NBT_Tag<void*>(nameArr, data.pointerData));

				break;
			case NBT_Base::NBT_INT_ARRAY: {
				int arraySize;

				SDL_RWread(file, &arraySize, SZ_INT, 1);

				int* dataArray = new int[arraySize];

				SDL_RWread(file, dataArray, SZ_INT, arraySize);

				NBT_Tag<std::vector<int>>* readTag = new NBT_Tag<std::vector<int>>(nameArr, std::vector<int>(arraySize));

				for (int c = 0; c < arraySize; c++) {
					readTag->getData()[c] = dataArray[c];
				}

				nbt->setTag(readTag);

				delete[] dataArray;

				break;
			}
			case NBT_Base::NBT_DOUBLE_ARRAY: {
				int arraySize;

				SDL_RWread(file, &arraySize, SZ_INT, 1);

				double* dataArray = new double[arraySize];

				SDL_RWread(file, dataArray, SZ_DOUBLE, arraySize);

				NBT_Tag<std::vector<double>>* readTag = new NBT_Tag<std::vector<double>>(nameArr, std::vector<double>(arraySize));

				for (int c = 0; c < arraySize; c++) {
					readTag->getData()[c] = dataArray[c];
				}

				nbt->setTag(readTag);

				delete[] dataArray;

				break;
			}
			default:
				Bingo::Utils::Warn("Found Invalid NBT_Type: " + NBT_Base::NBT_TypeStr[type] + '\n');
				return;
				break;
			}

			ATOMIC_UNLOCK(ThreadManager::fileLock);
		}
	} while (true);
}

void Bingo::FileManager::flushFile(std::string fileName) {
	if (files.find(fileName) != files.end()) {
		FileMode mode = files[fileName].first;

		closeFile(fileName);
		//SDL_Delay(500);
		//openFile(fileName, READ);
		//closeFile(fileName);
		openFile(fileName, mode);
	}
}

void Bingo::FileManager::closeFile(std::string fileName) {
	if (files.find(fileName) != files.end()) {
		ATOMIC_LOCK(ThreadManager::fileLock);

		SDL_RWclose(files[fileName].second);

		ATOMIC_UNLOCK(ThreadManager::fileLock);

		files.erase(fileName);
	}
}

void Bingo::FileManager::createDirectory(std::string pathStr) {
	if (!isDir(pathStr)) {
		directories[pathStr] = false;
		std::filesystem::create_directories(std::filesystem::path(pathStr));

		NBT_Compound* nbt;

		openFile(configFile, READ);

		nbt = readNBT(configFile);

		closeFile(configFile);

		if (nbt == NULL) {
			nbt = new NBT_Compound("FileMan.config");
			NBT_Compound* dirs = new NBT_Compound("CreatedDirs");

			nbt->setTag(dirs);
		}

		NBT_Tag<string>* tag = new NBT_Tag<std::string>("directory" + nbt->getTagCount(), pathStr);

		static_cast<NBT_Compound*>(nbt->getTag("CreatedDirs"))->setTag(tag);

		openFile(configFile, WRITE);

		writeNBT(configFile, nbt);

		closeFile(configFile);

		delete nbt;
	}
	else {
		Bingo::Utils::Warn("Attempting to create a directory that already exists: " + pathStr + '\n');
	}
}

void Bingo::FileManager::removeDirectory(std::string pathStr) {
	if (directories.find(pathStr) != directories.end()) {
		directories.erase(pathStr);
		std::filesystem::remove_all(std::filesystem::path(pathStr));
	}
	else {
		Bingo::Utils::Warn("Attempting to delete a directory that was not created by FileManager: " + pathStr + '\n');
	}
}

std::vector<std::string> Bingo::FileManager::listDirectory(std::string pathStr) {
	std::vector<std::string> result;
	std::filesystem::directory_iterator end;

	for (auto iter = std::filesystem::directory_iterator(std::filesystem::path(pathStr)); iter != end; iter++) {
		std::string pathStr = iter->path().string();

		if (pathStr.find("\\") != std::string::npos) {
			pathStr.replace(pathStr.find("\\"), 1, "/");
		}

		result.push_back(pathStr);
	}

	return result;
}

bool Bingo::FileManager::isFile(std::string pathStr) {
	return std::filesystem::is_regular_file(std::filesystem::path(pathStr));
}

bool Bingo::FileManager::isDir(std::string pathStr) {
	return std::filesystem::is_directory(std::filesystem::path(pathStr));
}

std::string Bingo::FileManager::saveFileDialog(std::string pathStr, std::vector<std::string>* filterList, std::string title) {
	char* result;
	const char* filters[10];
	memset(filters, 0, sizeof(filters));

	std::filesystem::path filePath = pathStr;

	if (!filePath.is_absolute()) {
		filePath = std::filesystem::absolute(filePath);
	}

	pathStr = filePath.string();

	if (!pathStr.ends_with('/')) {
		pathStr += "/";
	}

	if (filterList) {
		for (uint c = 0; c < filterList->size() && c < 10; c++) {
			filters[c] = (*filterList)[c].c_str();
		}
	}

	if (title.empty()) {
		title = "Save File";
	}

	result = tinyfd_saveFileDialog(
		title.c_str(),
		pathStr.c_str(),
		(filterList ? filterList->size() : 0),
		filters,
		NULL);

	if (result) {
		return result;
	}

	return "";
}

std::string Bingo::FileManager::openFileDialog(std::string pathStr, std::vector<std::string>* filterList, std::string title) {
	char* result;
	const char* filters[10];
	memset(filters, 0, sizeof(filters));

	std::filesystem::path filePath = pathStr;

	if (!filePath.is_absolute()) {
		filePath = std::filesystem::absolute(filePath);
	}

	pathStr = filePath.string();

	if (!pathStr.ends_with('/')) {
		pathStr += "/";
	}

	if (filterList) {
		for (uint c = 0; c < filterList->size() && c < 10; c++) {
			filters[c] = (*filterList)[c].c_str();
		}
	}

	if (title.empty()) {
		title = "Select File";
	}

	result = tinyfd_openFileDialog(
		title.c_str(),
		pathStr.c_str(),
		(filterList ? filterList->size() : 0),
		filters,
		NULL,
		0);

	if (result) {
		return result;
	}

	return "";
}

std::vector<std::string> Bingo::FileManager::openFileDialogMultiple(std::string pathStr, std::vector<std::string>* filterList, std::string title) {
	char* result;
	std::vector<std::string> fileResult;
	const char* filters[10];
	memset(filters, 0, sizeof(filters));

	std::filesystem::path filePath = pathStr;

	if (!filePath.is_absolute()) {
		filePath = std::filesystem::absolute(filePath);
	}

	pathStr = filePath.string();

	if (!pathStr.ends_with('/')) {
		pathStr += "/";
	}

	if (filterList) {
		for (uint c = 0; c < filterList->size() && c < 10; c++) {
			filters[c] = (*filterList)[c].c_str();
		}
	}

	if (title.empty()) {
		title = "Select Multiple Files";
	}

	result = tinyfd_openFileDialog(
		title.c_str(),
		pathStr.c_str(),
		(filterList ? filterList->size() : 0),
		filters,
		NULL,
		1);

	if (result) {
		fileResult = Bingo::Utils::split(std::string(result), "|");
	}

	return fileResult;
}