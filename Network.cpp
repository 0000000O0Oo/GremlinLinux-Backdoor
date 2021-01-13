#include "Infection.h"

void Networking::enumerateAdapters(){
	printf("[+] Adapters Enumeration : \n");
	struct ifaddrs* addresses;

	if(getifaddrs(&addresses) == 1){
		printf("getifaddrs call failed\n");
		return;
	}
	struct ifaddrs* address = addresses;
	while(address){
		Adapters currentAdapter;
		currentAdapter.Family = address->ifa_addr->sa_family;
		if(currentAdapter.Family == AF_INET || currentAdapter.Family == AF_INET6){
			currentAdapter.AdapterName = address->ifa_name;
			printf("\t - %s-> ", currentAdapter.AdapterName);
			printf("%s -> ", currentAdapter.Family == AF_INET ? "IPv4" : "IPv6");
			char ap[100];
			const int family_size = currentAdapter.Family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
			getnameinfo(address->ifa_addr, family_size, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
			currentAdapter.LocalIPAddress = ap;
			printf("%s\n", currentAdapter.LocalIPAddress);
			adapters.push_back(currentAdapter);
		}
	}
}

