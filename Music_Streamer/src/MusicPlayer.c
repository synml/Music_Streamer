﻿#pragma comment(lib, "winmm.lib")
#include <stdio.h>
#include <Windows.h>
#include "ClassLinker.h"


//전체 재생목록을 출력한다.
int printFullPlaylist(char playlist[][512])
{
	printf("\t 재생목록 \n");
	printf("--------------------------------\n");
	for (int i = 1; i < 100; i++)
	{
		if (strlen(playlist[i]) == 0)
			break;
		else
		{
			char *pos = strrchr(playlist[i], '/');
			printf("%d. %s\n", i, pos + 1);
		}
	}

	return 0;
}

//서버용 음악 플레이어


//클라이언트용 음악 플레이어
int client_MusicPlayer(char playlist[][512])
{
	int retval;

	MCI_OPEN_PARMS mciOpen;		//장치 드라이버를 열 때 필요한 객체
	MCI_PLAY_PARMS mciPlay;		//재생할 때 필요한 객체
	MCIDEVICEID deviceID;	//장치 드라이버 ID 저장
	char fileName[256];		//오디오 파일의 이름 저장
	char fileType[10];		//파일의 형식을 저장

	for (int i = 1; i < 100; i++)
	{
		//만약, i번째 재생목록이 비어있으면 재생중지
		if (strlen(playlist[i]) == 0)
			break;

		//파일의 경로와 이름, 확장자를 분리해서 저장한다.
		strcpy_s(fileName, sizeof(fileName), strrchr(playlist[i], '/') + 1);
		strcpy_s(fileType, sizeof(fileType), strrchr(playlist[i], '.'));

		//열 파일의 이름과 형태를 기술한다.
		mciOpen.lpstrElementName = playlist[i];
		if (strcmp(fileType, ".mp3") == 0)
			mciOpen.lpstrDeviceType = "mpegvideo";
		else if (strcmp(fileType, ".wav") == 0)
			mciOpen.lpstrDeviceType = "waveaudio";
		else if (strcmp(fileType, ".mid") == 0)
			mciOpen.lpstrDeviceType = "sequencer";
		else
		{
			printf("해당 파일형식은 지원하지 않습니다. \n");
			return 1;
		}

		//사운드 장치 드라이버 열기
		retval = mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD_PTR)&mciOpen);
		if (retval != 0)
		{
			printMciError(retval);
			return 1;
		}
		else
			deviceID = mciOpen.wDeviceID;  //DeviceID를 변수에 저장한다.

		//파일을 재생한다.
		textcolor(SKY_BLUE);
		printf("%d. %s를 재생합니다. \n", i, fileName);
		textcolor(RESET);
		retval = mciSendCommand(deviceID, MCI_PLAY, MCI_WAIT, (DWORD_PTR)&mciPlay);
		if (retval != 0)
		{
			printMciError(retval);
			return 1;
		}

		//오디오 파일을 닫는다.
		closeAudioFile(deviceID);
	}

	return 0;
}