#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

int main() {
	WSAData wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	unsigned short listentPort = 8000;

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(listentPort);

	bind(serverSocket, (SOCKADDR*)&addr, sizeof(addr));

	char buf[256];
	listen(serverSocket, sizeof(buf));

	SOCKET client = accept(serverSocket, NULL, NULL);

	while (1) {
		int ret = recv(client, buf, sizeof(buf) - 1, 0);
		if (ret <= 0) {
			break;
		}
		buf[ret] = 0;
		printf("%s", buf);
	}

	system("pause");

	closesocket(serverSocket);
	WSACleanup();
}
