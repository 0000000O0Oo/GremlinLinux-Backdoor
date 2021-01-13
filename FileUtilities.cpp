#include "Infection.h"

char FileUtilities::GetActualTime(){
    time_t actualTime = time(NULL);
    char* aTime = ctime(&actualTime);
    return *aTime;
}

void FileUtilities::write2File(const char* fileName, std::string fileContent){
    if(std::experimental::filesystem::exists(fileName)){
        std::ofstream file;
        file.open(fileName, std::ios::app);
        file << fileContent;
    } else {
        std::ofstream file;
        file.open(fileName);
        file << fileContent;
    }
}

void FileUtilities::log2File(const char* fileDirectory, std::string logContent){
    if(std::experimental::filesystem::exists(fileDirectory)){
        std::ofstream file;
        file.open(fileDirectory, std::ios::app);
        char currentTime = GetActualTime();
    }
}