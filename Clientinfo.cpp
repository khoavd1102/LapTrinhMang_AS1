#include <stdio.h>
#include <WinSock2.h>
#include <cstring>
#include <string>
#include <ws2tcpip.h>

#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)
#pragma warning(disable:4700)

int main() {

	char targetIp[20] = "127.0.0.1";
	unsigned short targetPort = 8000u;

	//scanf("%s %u", target_ip, &target_port);

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = inet_addr(targetIp);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(targetPort);

	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	int ret = connect(client_socket, (SOCKADDR*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR) {
		printf("CONNECTION ERRORS\n");
		return 0;
	};

	char client_info[300] = "";

	//GET COMPUTER NAME
	CHAR ComputerName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD   size = MAX_COMPUTERNAME_LENGTH + 1;
	if (GetComputerNameA(ComputerName, &size)) {
		strcat(client_info, "Computer Name: ");
		strcat(client_info, ComputerName);
		printf("WE GOT COMPUTER NAME \n");
		strcat(client_info, "\n");
	};

	//GET DRIVE NAME
	CHAR DriveName[32];
	if (GetLogicalDriveStringsA(MAX_PATH, DriveName)) {
		strcat(client_info, "Drive Name: ");
		strcat(client_info, DriveName);
		printf("WE GOT DRIVE NAME \n");
		strcat(client_info, "\n");
	};

	//GET FREE SPACE
	LPCSTR root = "C:\\";
	DWORD sectorsPerCluster;
	DWORD bytesPerSector;
	DWORD numberOfFreeClusters;
	DWORD totalNumberOfClusters;
	if (GetDiskFreeSpaceA(root, &sectorsPerCluster, &bytesPerSector, &numberOfFreeClusters, &totalNumberOfClusters)) {

		long long free_space = (long long)bytesPerSector * sectorsPerCluster * numberOfFreeClusters;

		std::string s = (std::to_string(free_space));
		const char* freespace = s.c_str();

		strcat(client_info, "Free Space: ");
		strcat(client_info, freespace);
		printf("WE GOT FREE SPACE \n");
		strcat(client_info, " Byte\n");
	};

	send(client_socket, client_info, strlen(client_info), 0);

	closesocket(client_socket);
	WSACleanup();
}