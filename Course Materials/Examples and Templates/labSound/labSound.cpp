// labSound.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define STRICT
#include <windows.h>
#include <basetsd.h>
#include <commdlg.h>
#include <mmreg.h>
#include <dxerr8.h>
#include <dsound.h>
#include "resource.h"
#include "DSUtil.h"
#include "DXUtil.h"

CSoundManager* g_pSoundManager = NULL;
CSound*        g_pSound = NULL;
BOOL           g_bBufferPaused;


int _tmain(int argc, _TCHAR* argv[])
{

	g_pSoundManager = new CSoundManager();

	HRESULT hr = g_pSoundManager->Initialize(NULL, DSSCL_PRIORITY, 2, 22050, 16 ) ;
    
	static TCHAR strFileName[MAX_PATH] = TEXT("C:\\20843\\chimes.wav");
	
	hr = g_pSoundManager->Create( &g_pSound, strFileName, 0, GUID_NULL );
   
	g_pSound->Play(0,DSBPLAY_LOOPING);

	Sleep(1000);
	g_pSound->Stop();
	delete g_pSound;
	delete g_pSoundManager;

	return 0;
}

