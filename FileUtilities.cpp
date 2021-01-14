#include "Infection.h"

char* FileUtilities::GetActualTime(){
    time_t actualTime = time(NULL);
    char* aTime = ctime(&actualTime);
    std::string timeReturn = aTime;
    return aTime;
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
        //currentTime = GetActualTime function
        char* currentTime = GetActualTime();
        //Taking char* currentTime and storing it in a std::string
        std::string logTime{currentTime};
        //Remove \r from the end of currentTime
        logTime.erase(logTime.size() - 1);
        //logTime was == date, now it == [date]
        logTime = "[" + logTime + "]";
        // Writing logContent to the file
        file<<logTime<<" "<<logContent<<"\n";
        file.close();
    } else {
        std::ofstream file;
        file.open(fileDirectory);
        //currentTime = GetActualTime function
        char* currentTime = GetActualTime();
        //Taking char* currentTime and storing it in a std::string
        std::string logTime{currentTime};
        //Remove \r from the end of currentTime
        logTime.erase(logTime.size() - 1);
        //Logtime was == date, now it == [date]
        logTime = "[" + logTime + "]";
        //Log it to the file
        file<<logTime<<" "<<logContent<<"\n";
        file.close();
    }
}