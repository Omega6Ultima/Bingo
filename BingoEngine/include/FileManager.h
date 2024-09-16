//Dustin Gehm

#pragma once
#ifndef _FILEMANAGER_H
#define _FILEMANAGER_H

#include <string>

#include "Core.h"
#include "NBT.h"
#include "Singleton.h"

namespace Bingo {

	class FileManager : public Singleton<FileManager>, public Core::Manager {
	public:
		enum FileMode { READ, WRITE, APPEND, READ_WRITE, READ_APPEND };

		FileManager(std::string configPath);
		~FileManager();

		/*Create the file, if it does not exist*/
		void createFile(std::string fileName);
		/*Checks to see if the file exists*/
		bool checkFile(std::string fileName);
		llong getFileSize(std::string fileName);
		void openFile(std::string fileName, FileMode mode);
		void seekFile(std::string, int offset);
		void writeString(std::string fileName, std::string content);
		std::string readString(std::string fileName);
		void writeNBT(std::string fileName, NBT_Compound* nbt);
		NBT_Compound* readNBT(std::string fileName);
		void flushFile(std::string fileName);
		void closeFile(std::string fileName);

		void createDirectory(std::string path);
		void removeDirectory(std::string path);
		std::vector<std::string> listDirectory(std::string path);
		bool isFile(std::string path);
		bool isDir(std::string path);

		std::string saveFileDialog(std::string path, std::vector<std::string>* filterList = NULL, std::string title = "");
		std::string openFileDialog(std::string path, std::vector<std::string>* filterList = NULL, std::string title = "");
		std::vector<std::string> openFileDialogMultiple(std::string path, std::vector<std::string>* filterList = NULL, std::string title = "");

	private:
		void readNBT_Tag(SDL_RWops* file, NBT_Compound* nbt);

	private:
		std::string configFile;
		std::map<std::string, std::pair<FileMode, SDL_RWops*>> files;
		std::map<std::string, bool> directories;
	};

}

#endif;