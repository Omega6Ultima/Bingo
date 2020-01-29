//Dustin Gehm

#pragma once
#ifndef _FILEMANAGER_H
#define _FILEMANAGER_H

#include <string>

#include "NBT.h"
#include "Singleton.h"

using std::string;
using std::pair;

class FileManager : public Singleton<FileManager>{
public:
	enum FileMode{ READ, WRITE, APPEND, READ_WRITE, READ_APPEND };

	FileManager(string configPath);
	~FileManager();

	/*Create the file, if it does not exist*/
	void createFile(string fileName);
	/*Checks to see if the file exists*/
	bool checkFile(string fileName);
	void openFile(string fileName, FileMode mode);
	void writeNBT(string fileName, NBT_Compound* nbt);
	NBT_Compound* readNBT(string fileName);
	void flushFile(string fileName);
	void closeFile(string fileName);

	void createDirectory(string path);
	void removeDirectoy(string path);
	vector<string> listDirectory(string path);
	bool isFile(string path);
	bool isDir(string path);

private:
	void readNBT_Tag(SDL_RWops* file, NBT_Compound* nbt);

private:
	string configFile;
	map<string, pair<FileMode, SDL_RWops*>> files;
	map<string, bool> directories;
};



#endif;