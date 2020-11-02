//Dustin Gehm

//supressing warnings about tr2::filesystem
//#define _SILENCE_TR2_SYS_NAMESPACE_DEPRECATION_WARNING

#include "FileManager.h"

#include <filesystem>

#include "Exception.h"
#include "ThreadManager.h"

#include <tinyfiledialogs.h>

using std::filesystem::absolute;
using std::filesystem::create_directories;
using std::filesystem::directory_iterator;
using std::filesystem::is_directory;
using std::filesystem::is_regular_file;
using std::filesystem::path;
using std::filesystem::remove_all;

using Bingo::FileManager;
using Bingo::Math::DynVecN;
using Bingo::NBT_Compound;
using Bingo::Utils::Error;
using Bingo::Utils::Warn;

//TODO add error handling (Error/Warn)
//TODO consider using ifstream/ofstream instead of SDL_RWops
//consider keeping track of created NBT when reading file and deleting them in destructor/flushCache func

FileManager::FileManager(string configPath) {
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

		for (uint c = 0; c < dirs->getTagCount(); ++c) {
			directories[static_cast<NBT_Tag<string>*>(dirs->getTag(c))->getData()] = false;
		}

		delete nbt;
	}
}

FileManager::~FileManager() {
	ATOMIC_LOCK(ThreadManager::fileLock);

	for (auto iter = files.begin(); iter != files.end(); iter++) {
		SDL_RWclose((iter->second).second);
	}

	ATOMIC_UNLOCK(ThreadManager::fileLock);

	NBT_Compound* nbt = new NBT_Compound("FileMan.config");
	NBT_Compound* dirs = new NBT_Compound("CreatedDirs");

	nbt->setTag(dirs);

	uint dirCount = 0;

	for (pair<string, bool> keyValue : directories) {
		dirs->setTag(new NBT_Tag<string>("directory" + dirCount, keyValue.first));
	}

	openFile(configFile, WRITE);

	writeNBT(configFile, nbt);

	closeFile(configFile);

	delete nbt;
}

void FileManager::createFile(string fileName) {
	ATOMIC_LOCK(ThreadManager::fileLock);

	SDL_RWops* file = SDL_RWFromFile(fileName.c_str(), "w");

	if (file == NULL) {
		Error("Could not create file: ", fileName);

		return;
	}

	SDL_RWclose(file);

	ATOMIC_UNLOCK(ThreadManager::fileLock);
}

bool FileManager::checkFile(string fileName) {
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

llong FileManager::getFileSize(string fileName) {
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

void FileManager::openFile(string fileName, FileMode mode) {
	string openMode;

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
		Error("Invalid FileMode for opening file: " + mode, fileName);
	}

	ATOMIC_LOCK(ThreadManager::fileLock);

	SDL_RWops* file = SDL_RWFromFile(fileName.c_str(), openMode.c_str());

	ATOMIC_UNLOCK(ThreadManager::fileLock);

	if (file == NULL) {
		Error("File could not be opened: " + fileName);
	}

	files[fileName] = pair<FileMode, SDL_RWops*>(mode, file);
}

void FileManager::writeNBT(string fileName, NBT_Compound* nbt) {
	if (files.find(fileName) != files.end()) {
		ATOMIC_LOCK(ThreadManager::fileLock);

		nbt->writeData(files[fileName].second);

		ATOMIC_UNLOCK(ThreadManager::fileLock);
	}
}

NBT_Compound* FileManager::readNBT(string fileName) {
	NBT_Compound* nbt = NULL;

	if (files.find(fileName) != files.end()) {
		NBT_Base::NBT_Type type;

		ATOMIC_LOCK(ThreadManager::fileLock);

		SDL_RWread(files[fileName].second, &type, sizeof(NBT_Base::NBT_Type), 1);

		ATOMIC_UNLOCK(ThreadManager::fileLock);

		if (type != NBT_Base::NBT_COMPOUND) {
			Error("NBT file is not in the correct format. Maybe its corrupted?", fileName);
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

void FileManager::readNBT_Tag(SDL_RWops* file, NBT_Compound* nbt) {
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

				nbt->setTag(new NBT_Tag<string>(nameArr, data.stringData));

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

				nbt->setTag(new NBT_Tag<DynVecN<int>>(nameArr, DynVecN<int>(arraySize, dataArray)));

				break;
			}
			case NBT_Base::NBT_DOUBLE_ARRAY: {
				int arraySize;

				SDL_RWread(file, &arraySize, SZ_INT, 1);

				double* dataArray = new double[arraySize];

				SDL_RWread(file, dataArray, SZ_DOUBLE, arraySize);

				nbt->setTag(new NBT_Tag<DynVecN<double>>(nameArr, DynVecN<double>(arraySize, dataArray)));

				break;
			}
			default:
				Warn("Found Invalid NBT_Type: " + NBT_Base::NBT_TypeStr[type] + '\n');
				return;
				break;
			}

			ATOMIC_UNLOCK(ThreadManager::fileLock);
		}
	} while (true);
}

void FileManager::flushFile(string fileName) {
	if (files.find(fileName) != files.end()) {
		//FileMode mode = files[fileName].first;

		//closeFile(fileName);
		//SDL_Delay(500);
		//openFile(fileName, READ);
		//closeFile(fileName);
		//openFile(fileName, mode);
	}
}

void FileManager::closeFile(string fileName) {
	if (files.find(fileName) != files.end()) {
		ATOMIC_LOCK(ThreadManager::fileLock);

		SDL_RWclose(files[fileName].second);

		ATOMIC_UNLOCK(ThreadManager::fileLock);

		files.erase(fileName);
	}
}

void FileManager::createDirectory(string pathStr) {
	if (!isDir(pathStr)) {
		directories[pathStr] = false;
		create_directories(path(pathStr));

		NBT_Compound* nbt;

		openFile(configFile, READ);

		nbt = readNBT(configFile);

		closeFile(configFile);

		NBT_Tag<string>* tag = new NBT_Tag<string>("directory" + nbt->getTagCount(), pathStr);

		static_cast<NBT_Compound*>(nbt->getTag("CreatedDirs"))->setTag(tag);

		openFile(configFile, WRITE);

		writeNBT(configFile, nbt);

		closeFile(configFile);

		delete nbt;
	}
	else {
		Warn("Attempting to create a directory that already exists: " + pathStr + '\n');
	}
}

void FileManager::removeDirectoy(string pathStr) {
	if (directories.find(pathStr) != directories.end()) {
		directories.erase(pathStr);
		remove_all(path(pathStr));
	}
	else {
		Warn("Attempting to delete a directory that was not created by FileManager: " + pathStr + '\n');
	}
}

vector<string> FileManager::listDirectory(string pathStr) {
	vector<string> result;
	directory_iterator end;

	for (auto iter = directory_iterator(path(pathStr)); iter != end; iter++) {
		result.push_back(iter->path().string());
	}

	return result;
}

bool FileManager::isFile(string pathStr) {
	return is_regular_file(path(pathStr));
}

bool FileManager::isDir(string pathStr) {
	return is_directory(path(pathStr));
}

string FileManager::saveFileDialog(string pathStr, vector<string>* filterList, string title) {
	char* result;
	const char* filters[10];
	memset(filters, 0, sizeof(filters));

	path filePath = pathStr;

	if (!filePath.is_absolute()) {
		filePath = absolute(filePath);
	}

	pathStr = filePath.string();

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

string FileManager::openFileDialog(string pathStr, vector<string>* filterList, string title) {
	char* result;
	const char* filters[10];
	memset(filters, 0, sizeof(filters));

	path filePath = pathStr;

	if (!filePath.is_absolute()) {
		filePath = absolute(filePath);
	}

	pathStr = filePath.string();

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

vector<string> FileManager::openFileDialogMultiple(string pathStr, vector<string>* filterList, string title) {
	char* result;
	vector<string> fileResult;
	const char* filters[10];
	memset(filters, 0, sizeof(filters));

	path filePath = pathStr;

	if (!filePath.is_absolute()) {
		filePath = absolute(filePath);
	}

	pathStr = filePath.string();

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
		string resultStr(result);
		uint offset = 0;
		uint index = resultStr.find('|', offset);

		while (index != string::npos) {
			fileResult.push_back(resultStr.substr(offset, index - offset));

			offset = index;
			index = resultStr.find('|', offset + 1);
		}

		fileResult.push_back(resultStr.substr(offset));
	}

	return fileResult;
}