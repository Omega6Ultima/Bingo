#pragma once
#include "pch.h"
#include "FileManager.h"
#include "Nbt.h"

TEST(TestFileManager, TestCreateFile) {
	auto& fileMan = Bingo::FileManager::getSingleton();
	string fileName = "files/create/create.txt";

	fileMan.createDirectory("files/create");

	EXPECT_NO_THROW(fileMan.createFile(fileName));
	EXPECT_NO_THROW(fileMan.createFile(fileName));

	EXPECT_TRUE(fileMan.checkFile(fileName));

	fileMan.removeDirectory("files/create");
}

TEST(TestFileManager, TestCheckFile) {
	EXPECT_TRUE(Bingo::FileManager::getSingleton().checkFile("files/fileMan.config"));
}

TEST(TestFileManager, TestGetFileSize) {
	EXPECT_EQ(Bingo::FileManager::getSingleton().getFileSize("files/filesize/filesize.txt"), 705);
}

TEST(TestFileManager, TestOpenFile) {
	auto& fileMan = Bingo::FileManager::getSingleton();
	std::string readOnlyFileName = "files/open/read_only.txt";

	EXPECT_ANY_THROW(fileMan.openFile(readOnlyFileName, fileMan.WRITE));
	EXPECT_NO_THROW(fileMan.openFile(readOnlyFileName, fileMan.READ));
	EXPECT_NO_THROW(fileMan.closeFile(readOnlyFileName));
}

TEST(TestFileManager, TestOpenFile2) {
	auto& fileMan = Bingo::FileManager::getSingleton();
	std::string appendFileName = "files/open/append.txt";

	EXPECT_NO_THROW(fileMan.openFile(appendFileName, fileMan.READ));
	EXPECT_EQ(fileMan.readString(appendFileName), "existing file contents");
	EXPECT_NO_THROW(fileMan.closeFile(appendFileName));

	EXPECT_NO_THROW(fileMan.openFile(appendFileName, fileMan.APPEND));
	EXPECT_EQ(fileMan.readString(appendFileName), "");
	EXPECT_NO_THROW(fileMan.closeFile(appendFileName));
}

TEST(TestFileManager, TestOpenFile3) {
	auto& fileMan = Bingo::FileManager::getSingleton();
	std::string nonexist = "files/I_dont_exist.txt";

	EXPECT_ANY_THROW(fileMan.openFile(nonexist, fileMan.READ));
}

TEST(TestFileManager, TestSeekFile) {
	auto& fileMan = Bingo::FileManager::getSingleton();
	std::string fileName = "files/seek/seek.txt";

	fileMan.openFile(fileName, fileMan.READ);

	std::string read1 = fileMan.readString(fileName);

	{
		std::string read2 = fileMan.readString(fileName);

		EXPECT_FALSE(read1 == read2);
	}
	{
		EXPECT_NO_THROW(fileMan.seekFile(fileName, 0));

		std::string read2 = fileMan.readString(fileName);

		EXPECT_EQ(read1, read2);
	}

	fileMan.closeFile(fileName);
}

TEST(TestFileManager, TestSeekFile2) {
	auto& fileMan = Bingo::FileManager::getSingleton();
	std::string fileName = "files/seek/seek.txt";

	fileMan.openFile(fileName, fileMan.READ);

	EXPECT_ANY_THROW(fileMan.seekFile(fileName, -3));

	fileMan.closeFile(fileName);
}

TEST(TestFileManager, TestWriteReadString) {
	auto& fileMan = Bingo::FileManager::getSingleton();
	std::string fileName = "files/string/test.txt";
	std::string contents = "anteatereatingant";

	{
		fileMan.openFile(fileName, fileMan.WRITE);

		EXPECT_NO_THROW(fileMan.writeString(fileName, contents));

		fileMan.closeFile(fileName);
	}
	{
		fileMan.openFile(fileName, fileMan.READ);

		EXPECT_EQ(fileMan.readString(fileName), contents);

		fileMan.closeFile(fileName);
	}
}

TEST(TestFileManager, TestWriteReadNBT) {
	auto& fileMan = Bingo::FileManager::getSingleton();

	Bingo::NBT_Compound nbt = Bingo::NBT_Compound("toplevel");

	nbt.setTag(new Bingo::NBT_Tag<int>("sub_int", 42));
	nbt.setTag(new Bingo::NBT_Tag<double>("sub_dbl", 2.3));

	Bingo::NBT_Compound* subCompound = new Bingo::NBT_Compound("sub_cpd");
	subCompound->setTag(new Bingo::NBT_Tag<std::string>("sub_sub_str", "fourty-two"));

	nbt.setTag(subCompound);

	std::string fileName = "files/nbt/nbt.txt";

	{
		fileMan.openFile(fileName, fileMan.WRITE);

		EXPECT_NO_THROW(fileMan.writeNBT(fileName, &nbt));

		fileMan.closeFile(fileName);
	}
	{
		std::vector<Bingo::NBT_Compound*> readNBT_vec;
		fileMan.openFile(fileName, fileMan.READ);

		// using vector::emplace_back to capture return value of readNBT
		EXPECT_NO_THROW(readNBT_vec.emplace_back(fileMan.readNBT(fileName)));

		if (readNBT_vec[0] != NULL) {
			EXPECT_EQ(*(readNBT_vec[0]), nbt);

			delete readNBT_vec[0];
		}

		fileMan.closeFile(fileName);
	}
}

TEST(TestFileManager, TestFlushFile) {
	auto& fileMan = Bingo::FileManager::getSingleton();
	std::string fileName = "files/flush/flush.txt";
	std::string contents = "Trans rights are human rights";
	bool flushNeeded = false;

	{
		fileMan.openFile(fileName, fileMan.READ_WRITE);
		fileMan.writeString(fileName, contents);

		fileMan.seekFile(fileName, 0);

		flushNeeded = (fileMan.readString(fileName) != contents);

		fileMan.closeFile(fileName);
	}

	// If files are flushed on write, skip testing FileManager.flushFile
	if (flushNeeded) {
		fileMan.openFile(fileName, fileMan.READ_WRITE);
		fileMan.writeString(fileName, contents);

		EXPECT_NO_THROW(fileMan.flushFile(fileName));

		fileMan.seekFile(fileName, 0);

		EXPECT_EQ(fileMan.readString(fileName), contents);

		fileMan.closeFile(fileName);
	}
}

TEST(TestFileManager, TestCloseFile) {
	auto& fileMan = Bingo::FileManager::getSingleton();
	std::string fileName = "files/close/close.txt";

	fileMan.openFile(fileName, fileMan.READ);

	EXPECT_NO_THROW(fileMan.closeFile(fileName));
	EXPECT_NO_THROW(fileMan.closeFile(fileName));
}

TEST(TestFileManager, TestCreateDirectory) {
	auto& fileMan = Bingo::FileManager::getSingleton();
	std::string createDir = "files/createDir";

	fileMan.createDirectory(createDir);

	EXPECT_TRUE(fileMan.isDir(createDir));
}

TEST(TestFileManager, TestRemoveDirectory) {
	auto& fileMan = Bingo::FileManager::getSingleton();
	// Directory was created in above test
	std::string removeDir = "files/createDir";

	fileMan.removeDirectory(removeDir);

	EXPECT_FALSE(fileMan.isDir(removeDir));
}

TEST(TestFileManager, TestListDirectory) {
	auto& fileMan = Bingo::FileManager::getSingleton();
	std::string listDir = "files/listDir";
	std::vector<std::string> fileNames;
	fileNames.push_back(listDir + "/" + "exist.txt");
	fileNames.push_back(listDir + "/" + "should.txt");
	fileNames.push_back(listDir + "/" + "these.txt");

	for (auto& f : fileNames) {
		fileMan.createFile(f);
	}

	EXPECT_EQ(fileMan.listDirectory(listDir), fileNames);
}

TEST(TestFileManager, TestIsDir) {
	auto& fileMan = Bingo::FileManager::getSingleton();

	EXPECT_TRUE(fileMan.isDir("files/isDir"));
	EXPECT_FALSE(fileMan.isDir("files/isDir2"));
}

TEST(TestFileManager, TestIsFile) {
	auto& fileMan = Bingo::FileManager::getSingleton();

	EXPECT_TRUE(fileMan.isFile("files/isDir/isFile.txt"));
	EXPECT_FALSE(fileMan.isFile("files/isDir/isFile2.txt"));
}

TEST(TestFileManager, TestSaveFileDialog) {
	auto& fileMan = Bingo::FileManager::getSingleton();
	std::string dir = "files/fileDialog";
	std::string saveFile = "";
	
	saveFile = fileMan.saveFileDialog(dir, NULL, "");

	fileMan.createFile(saveFile);
}

TEST(TestFileManager, TestOpenFileDialog) {
	auto& fileMan = Bingo::FileManager::getSingleton();
	std::string dir = "files/fileDialog";
	std::string openFile = "";

	openFile = fileMan.openFileDialog(dir, NULL, "");

	EXPECT_TRUE(fileMan.checkFile(openFile));
}

TEST(TestFileManager, TestOpenFileDialogMultiple) {
	auto& fileMan = Bingo::FileManager::getSingleton();
	std::string dir = "files/fileDialog";
	std::vector<std::string> openFiles;

	openFiles = fileMan.openFileDialogMultiple(dir, NULL, "");

	for (auto& f : openFiles) {
		EXPECT_TRUE(fileMan.checkFile(f));
	}
}