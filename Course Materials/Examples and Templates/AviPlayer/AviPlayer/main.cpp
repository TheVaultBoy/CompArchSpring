#include <dshow.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <strmif.h>
#include <control.h>

using namespace std;

int main() {
	bool keyEnd=false;
	string str;
	IMediaSeeking *	   pMediaSeeking;
    IGraphBuilder *    pGraphBuilder;
    IMediaControl *    pMediaControl;
    IMediaEvent *      pMediaEvent;
    CoInitialize( NULL );
    char s[]="Logo.avi";
	//Create a new COM-object
	//pGraphBuilder - new object
    CoCreateInstance( CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (LPVOID *)&pGraphBuilder );
	//Получение интерфейса управления
    pGraphBuilder->QueryInterface( IID_IMediaControl, (LPVOID *)&pMediaControl );
	//Второй интерфейс управления
	pGraphBuilder->QueryInterface( IID_IMediaSeeking, (LPVOID *)&pMediaSeeking );
	//Получение интерфейса сообщений
    pGraphBuilder->QueryInterface( IID_IMediaEvent, (LPVOID *)&pMediaEvent );
    pMediaControl->RenderFile( L"Logo.avi" );
	while(keyEnd!=true) {
		//system("clear");
		cout << "Please, enter the command\n";
		str="";
		cin >> str;
		if(str == "play") {
			pMediaControl->Run();
		}
		if(str == "pause") {
			pMediaControl->Pause();
		}
		if(str == "stop") {
			pMediaControl->Stop();
		}
		if(str == "end") {
			keyEnd=true;
		}
	}
    pMediaControl->Release();
    pGraphBuilder->Release();
    CoUninitialize();
    return 0;
}