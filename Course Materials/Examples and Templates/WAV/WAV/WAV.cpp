// WAV.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	
	CSoundManager* g_pSoundManager;
	CSound* g_pSound;
	LPTSTR strFileName = TEXT("C:\\drums.wav");
	HRESULT hres;
	char chSelection;
	g_pSoundManager = new CSoundManager(); 
	g_pSoundManager->Initialize( GetForegroundWindow(), DSSCL_PRIORITY ) ; 
	g_pSoundManager->SetPrimaryBufferFormat( 2, 22050, 16 ) ;
	g_pSoundManager->Create( &g_pSound, strFileName, 0, GUID_NULL, 1 );
	g_pSound->Play( 0, DSBPLAY_LOOPING);

	cout << "Would you like to start playing bass? (y or n)"<<endl;
	cin >> chSelection;
	if (chSelection=='y')
	{
		strFileName = TEXT("c:\\bass.wav");
		g_pSoundManager->Create( &g_pSound, strFileName, 0, GUID_NULL, 1 );
		g_pSound->Play( 0, DSBPLAY_LOOPING);
	}
	
	cout << "Would you like to start playing guitar? (y or n)"<<endl;
	cin >> chSelection;
	if (chSelection=='y')
	{
		strFileName = TEXT("c:\\guitar.wav");
		g_pSoundManager->Create( &g_pSound, strFileName, 0, GUID_NULL, 1 );
		g_pSound->Play( 0, DSBPLAY_LOOPING);
	}
	cout << "press any key to exit";
	cin >> chSelection;
	delete g_pSound;
	delete g_pSoundManager;
	return 0;
}

