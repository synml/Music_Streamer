﻿#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "mswsock")
#include <stdio.h>
#include <WinSock2.h>
#include "ClassLinker.h"

#define SERVER_PORT 50000

int client(SETTINGS *setUp)
{
	int retval;

	//윈속 초기화
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	//서버와 통신할 소켓 생성
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(setUp->server_ip);
	server_addr.sin_port = htons(SERVER_PORT);

	//서버에 접속하기
	retval = connect(sock, (SOCKADDR *)&server_addr, sizeof(server_addr));
	if (retval == SOCKET_ERROR)
		err_quit("connect()");
	else
		printf("서버와 연결되었습니다. \n\n");


	//파일이 저장될 디렉토리를 설정하고 파일 이름, 크기를 받아올 변수를 선언한다.
	char save_directory[100];
	strcpy_s(save_directory, sizeof(save_directory), ".\\playQue");
	char fileName[256];
	float fileSize = 0.0f;

	//파일을 수신한다.
	retval = fileReceive(sock, save_directory, fileName, &fileSize);
	if (retval == 0)
		printf("'%s' 수신 완료! (%0.2fMB) \n\n", fileName, fileSize);
	else if (retval == 1)
	{
		printf("'%s' 수신 실패! \n\n", fileName);
		return 1;
	}

	//서버와 연결을 종료한다.
	closesocket(sock);

	printf("프로그램을 종료합니다. \n");
	WSACleanup();
	return 0;
}