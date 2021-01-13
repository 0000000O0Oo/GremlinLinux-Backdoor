#include "Infection.h"

char FileTiming::GetActualTime(){
    time_t actualTime = time(NULL);
    char* aTime = ctime(&actualTime);
    actualTime = *aTime;
    return actualTime;
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