/*
* ��ǻ�ͳ�Ʈ��ũ 1����
* ������Ű��а� 22012225 �պ���
*/
#include <WinSock2.h>
#include <stdio.h>
#include <WS2tcpip.h>


//#pragma comment(lib, "ws2_32.lib");

int main(void) {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {  //����
		printf("Error in starting up Winsocket\n");
		return -1;
	}

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);


	SOCKADDR_IN myAddress;
	ZeroMemory(&myAddress, sizeof(myAddress));  //����ü���� ��ü�� 0���� �� �س���
	myAddress.sin_family = AF_INET;
	myAddress.sin_port = 50000;
	inet_pton(AF_INET, "192.168.210.1" , &(myAddress.sin_addr.s_addr));
	bind(s, (SOCKADDR*)&myAddress, sizeof(myAddress));  



	closesocket(s);


	WSACleanup();

	return 0;

}