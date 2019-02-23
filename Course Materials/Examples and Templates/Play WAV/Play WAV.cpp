// Play WAV.cpp : Defines the entry point for the console application.


#include "stdafx.h"
#include <windows.h>
#include <dsound.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#pragma	comment(lib,"dsound.lib")	
#pragma	comment(lib,"dxguid.lib")

#pragma warning(disable:4996)		

using namespace std;			

IDirectSound8	*g_pDirectSound;

void	PlayWav( char* szName,int delay)
{
	ifstream	fIn;	

	fIn.open(szName,ios::beg|ios::in|ios::binary);

	if(!fIn)
	{
		cout<<"File not find!";
		return;
	}

	//���� RIFF
	cout<<"Finding chunk RIFF";

	DWORD	dwRIFF;		
	fIn.read((char*)&dwRIFF,4);
	if(dwRIFF!=MAKEFOURCC('R','I','F','F'))
	{
		cout<<endl<<"Can not find chunk RIFF - unsupported file format";
		return;
	};

	cout<<" - OK!"<<endl;

	fIn.read((char*)&dwRIFF,4);	//������ ����� 

	cout<<"File size: "<<dwRIFF<<" bytes"<<endl;

	//���� WAVE
	DWORD		dwWAVE;//���� WAVE
	DWORD		dwFormat;//���� fmt
	long		lSizeFmt;//����� ��������� WAVEFORMATEX, ���������� � �����

	cout<<"Finding chunk WAVE";
	fIn.read((char*)&dwWAVE,4);
	if(dwWAVE!=MAKEFOURCC('W','A','V','E'))	
	{
		cout<<endl<<"Can not find chunk WAVE - unsupported file format!";
		return;
	};
	cout<<" - OK!"<<endl;

	cout<<"Finding chunk fmt";
	fIn.read((char*)&dwFormat,4);
	if(dwFormat!=MAKEFOURCC('f','m','t',' '))		
	{
		cout<<endl<<"Can not find chunk fmt - unsupported file format!"<<endl;
		return;
	};
	cout<<" - OK!"<<endl;

	fIn.read((char*)&lSizeFmt,4);
	if(lSizeFmt==0)
	{
		cout<<"Can not find size of audio-information - unsupported file format!"<<endl;
		return;
	};
	cout<<"Size of information:"<<lSizeFmt<<endl;

	WAVEFORMATEX	waveFormat=WAVEFORMATEX();//��� ������ �����-������
	fIn.read((char*)&waveFormat,16);//������ WAVEFORMATEX==16
	fIn.ignore(lSizeFmt-16);		//���������� �������

	// ���� ����? 
	if(waveFormat.wFormatTag!=1)
	{
		cout<<"This file is compressed - �nsupported file format!"<<endl;
		return;
	}

	//��������� ���� ����� ���� ���� 'fact', ���� 'data'
	DWORD	dwNextChunk=0;
	fIn.read((char*)&dwNextChunk,4);
	//���� 'fact', �� �� ��� ����������
	if(dwNextChunk==MAKEFOURCC('f','a','c','t'))		
	{
		cout<<"Finded chunk  fact!"<<endl;		
		DWORD	dwSizeFact=0;						
		fIn.read((char*)&dwSizeFact,4);				//��������� ������ ������������ �������
		fIn.ignore(dwSizeFact);						//����������
		fIn.read((char*)&dwNextChunk,4);			//��������� ���� 'data'
	};												

	//���� �� �� ����� ����� 'fact', �� � ���� ��� ���������, ����� �� 'data', � ������ � dwFormat
	cout<<"Finding chunk data";
	if(dwNextChunk!=MAKEFOURCC('d','a','t','a'))		//���� data, �� �� ��� ����������
	{
		cout<<"\n Can not find chunk data - unsupported file format!"<<endl;
		return;
	};			
	cout<<" - OK"<<endl;
	cout<<	"Information accepted!"<<endl;

	DWORD	dwDataSize;					
	fIn.read((char*)&dwDataSize,4);			//���������
	//����� ���� ������ ������
	char	*pData = new char[dwDataSize];	//������ �����, ��� �������� �����
	fIn.read(pData,dwDataSize);				//� ��������� � ���� ����...
	cout<<"File "<<szName<<" loaded!"<<endl;
	fIn.close();	

	char *pData2 = new char[dwDataSize];

	memcpy(pData2,pData,dwDataSize);
	if(memcmp(pData2,pData,dwDataSize)!=0)
	{
	cout<<"Failed to clone data"<<endl;
	return;
	}
	
	IDirectSoundBuffer	*pSoundBuffer=NULL;//�������� �����
	DSBUFFERDESC  dsbd = DSBUFFERDESC();
	dsbd.dwBufferBytes	=  dwDataSize;
	dsbd.dwFlags = DSBCAPS_STATIC;
	dsbd.dwSize	= sizeof(DSBUFFERDESC);
	dsbd.lpwfxFormat = &waveFormat;

	IDirectSoundBuffer	*pSoundBuffer2=NULL;//�������� ����� 2
	DSBUFFERDESC  dsbd2 = DSBUFFERDESC();
	dsbd2.dwBufferBytes	=  dwDataSize;
	dsbd2.dwFlags = DSBCAPS_STATIC;
	dsbd2.dwSize	= sizeof(DSBUFFERDESC);
	dsbd2.lpwfxFormat = &waveFormat;


	g_pDirectSound->CreateSoundBuffer(&dsbd,&pSoundBuffer,0);
	g_pDirectSound->CreateSoundBuffer(&dsbd2,&pSoundBuffer2,0);

	void			*pDst=0;
	DWORD			dwSize=0;
	pSoundBuffer->Lock(0,0,&pDst,&dwSize,0,0,DSBLOCK_ENTIREBUFFER);
	memcpy(pDst,pData,dwSize);
	pSoundBuffer->Unlock(pDst,dwSize,0,0);
	
	void			*pDst2=0;
	DWORD			dwSize2=0;
	pSoundBuffer2->Lock(0,0,&pDst2,&dwSize2,0,0,DSBLOCK_ENTIREBUFFER);
	memcpy(pDst2,pData,dwSize2);
	pSoundBuffer->Unlock(pDst2,dwSize2,0,0);

	pSoundBuffer->Play(0,0,0);
	Sleep(delay);
	pSoundBuffer2->Play(0,0,0);

	DWORD	hrStatus=0;
	DWORD	hrStatus2=0;

	do
	{
		pSoundBuffer->GetStatus(&hrStatus);
		pSoundBuffer2->GetStatus(&hrStatus2);
	}
	while((hrStatus==DSBSTATUS_PLAYING)||(hrStatus==DSBSTATUS_PLAYING));
	
	delete pData;
	delete pData2;
	return;
}

int main()
{
	DirectSoundCreate8(0,&g_pDirectSound,0);					//�������������� DirectSound
	g_pDirectSound->SetCooperativeLevel(GetForegroundWindow(),DSSCL_NORMAL);
	
	char name[256];
	int dl;
	cout<<"Please enter file name"<<endl;
	cin>>name;
	cout<<"Please enter delay(ms)"<<endl;
	cin>>dl;
	PlayWav(name,dl);										//����������� ����

	g_pDirectSound->Release();								//��������� DirectSound
	g_pDirectSound=NULL;


	return 0;
}

