#include <cstdio>
#include <WinSock2.h>
#include <iostream>
#include <cstring>
#include <thread>

using namespace std;
// #pragma commnet(lib, "ws2_32")

int client_init();
void proc_recv();

#define PORT 9999
#define PACKET_SIZE 1024
#define SERVER_IP "127.0.0.1"

SOCKET client_socket;

int main() 
{
	client_socket = client_init();
	if (client_socket < 0)
	{
		cout << "client socket init error" << endl;
		return 0;
	}

	thread cproc(proc_recv);

	char cMsg[PACKET_SIZE] = {};
	
	while (!WSAGetLastError())
	{
		cout << "입력 메시지 :";
		cin >> cMsg;
		send(client_socket, cMsg, strlen(cMsg), 0);
	}

	cproc.join();
	closesocket(client_socket);

	WSACleanup();
	return 0;

}

int client_init()
{
	WSADATA wsaData;		// WSADATA :: Windows 소켓 구조체
	SOCKET cilSocket;
	SOCKADDR_IN socktAddr = {};

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)		// 소켓버전, WSADATA 구조체 주소
	{
		cout << "WSAStartup Error" << endl;
		return -1;
	}

	if ((cilSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		cout << "socket Error" << endl;
		return -1;
	}

	memset(&socktAddr, 0, sizeof(socktAddr));
	socktAddr.sin_family = AF_INET;
	socktAddr.sin_port = htons(PORT);
	socktAddr.sin_addr.s_addr = inet_addr(SERVER_IP);		// inet_addr error 발생 시 project >> C/C++ >> 일반 >> SDL검사을 No 인지 확인.


	connect(cilSocket, (SOCKADDR*)&socktAddr, sizeof(socktAddr));

	return cilSocket;
}

void proc_recv()
{
	char cBuffer[PACKET_SIZE] = {};
	string sBreak;
	while (!WSAGetLastError())
	{
		memset(&cBuffer, 0, sizeof(cBuffer));
		recv(client_socket, cBuffer, PACKET_SIZE, 0);
		sBreak = cBuffer;
		if (sBreak == "exit") {
			break;
		}
		cout << "받은 메시지 : " << cBuffer << endl;
	}

}