#include <winsock2.h>
#include <stdio.h>
#include <WS2tcpip.h>	// for inet_pton() and inet_ntop()

#pragma comment(lib, "ws2_32.lib")

const int MAX_BUF = 1000;

int recvn(SOCKET s, char* pBuf, int iRequired) {
	int iLeft = iRequired;

	while (iLeft > 0) {
		int iReceived = recv(s, pBuf, iRequired, 0);
		if (iReceived > 0) {
			iLeft -= iReceived;
			pBuf += iReceived;
		}
		else {
			return iReceived;	// error or connection closed
		}
	}

	return (iRequired - iLeft);	// return total rx bytes
}

int main(void) {	
	WSADATA wsa;
	if( WSAStartup(MAKEWORD(2,2), &wsa) !=0) {
		printf("Too high Winsock version\n");
		return -1;
	}

	// start message
	printf("TCP Server starts now...\n");


	// socket creation
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if( s == INVALID_SOCKET) {
		printf("Error in socket(), Error code: %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	

	// bind
	SOCKADDR_IN myAddress;
	ZeroMemory(&myAddress, sizeof(myAddress));
	myAddress.sin_family = AF_INET;
	myAddress.sin_port = htons(60000);
	inet_pton(AF_INET, "165.229.86.21", &(myAddress.sin_addr.s_addr));
	
	if( bind(s, (SOCKADDR*)&myAddress, sizeof(myAddress)) == SOCKET_ERROR) {
		printf("Error in bind(), Error code: %d\n", WSAGetLastError());		
		closesocket(s);
		WSACleanup();
		return -1;
	}


	// listen
	if( listen(s,5) == SOCKET_ERROR) {
		printf("Error in listen(), Error code: %d\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return -1;
	}


	// receive and send	
	while(1) {
		SOCKADDR_IN clientAddress;
		int iAddressLength = sizeof(clientAddress);

		SOCKET t = accept(s, (SOCKADDR*)&clientAddress, &iAddressLength);
		if( t == INVALID_SOCKET) {
			printf("Error in accept(), Error code: %d\n", WSAGetLastError());
			continue;
		}

		

		char chRxBuf[MAX_BUF] = {0};

		int iLength = recv(t, chRxBuf, MAX_BUF, 0);
		//int iLength = recvn(t, chRxBuf, 5);
		if( iLength == SOCKET_ERROR) {
			printf("Error in recv(), Error code: %d\n", GetLastError());
			closesocket(t);
			continue;
		} else if( iLength == 0) {
			printf("Connection is closed normally\n");			
		} else {
			char temp[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &(clientAddress.sin_addr), temp, INET_ADDRSTRLEN);
			printf("From client (%s, %d): %s\n", temp, clientAddress.sin_port, chRxBuf);			
		}		

		// send welcome message
		const char chTxBuf[MAX_BUF] = "Hello, TCP Client!";
		if (send(t, chTxBuf, strlen(chTxBuf), 0) == SOCKET_ERROR) {
			printf("Error in send(), Error code: %d\n", GetLastError());
			closesocket(t);
			continue;
		}

		closesocket(t);
	}


	// clean up
	closesocket(s);
	WSACleanup();

	return 0;
}