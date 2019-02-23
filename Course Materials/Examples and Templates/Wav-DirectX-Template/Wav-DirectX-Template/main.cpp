#include "structs.h"

void main()
{
	DWORD RW; BYTE* Data; IDirectSound8* DirectSoundInterface;
	HANDLE File = CreateFileA("VOICE1.WAV", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	//Чтение заголовка
	RIFFBLOCK RiffBlock; ReadFile(File, &RiffBlock, sizeof(RiffBlock), &RW, NULL);
	
	if ((RiffBlock.Tag == MAKEFOURCC('R','I','F','F')) && (RiffBlock.Wave.Tag == MAKEFOURCC('W','A','V','E')) && (RiffBlock.Wave.Fmt.Tag == MAKEFOURCC('f','m','t',' ')) && (RiffBlock.Wave.Fmt.Data.Tag == MAKEFOURCC('d','a','t','a')))
	{
		//Чтение данных
		Data = new BYTE[RiffBlock.Wave.Fmt.Data.Length];
		ReadFile(File, Data, RiffBlock.Wave.Fmt.Data.Length, &RW, NULL);

		//Инициализия DirectSound
		DirectSoundCreate8(NULL, &DirectSoundInterface, NULL);
		DirectSoundInterface->SetCooperativeLevel(GetForegroundWindow(), DSSCL_NORMAL);

		/*
			Вставить редактирование Data для получения нужного эффекта
		*/

		//Создание структур для проигрывания
		WAVEFORMATEX wfex;
		wfex.nAvgBytesPerSec = RiffBlock.Wave.Fmt.Format.nAvgBytesPerSec;
		wfex.nBlockAlign = RiffBlock.Wave.Fmt.Format.nBlockAlign;
		wfex.nChannels = RiffBlock.Wave.Fmt.Format.nChannels;
		wfex.nSamplesPerSec = RiffBlock.Wave.Fmt.Format.nSamplesPerSec;
		wfex.wFormatTag = RiffBlock.Wave.Fmt.Format.wFormatTag;
		wfex.wBitsPerSample = RiffBlock.Wave.Fmt.Format.wBitsPerSample;
		wfex.cbSize = 0;
		DSBUFFERDESC dsbd = DSBUFFERDESC();
		dsbd.lpwfxFormat = &wfex;
		dsbd.dwBufferBytes = RiffBlock.Wave.Fmt.Data.Length;
		dsbd.dwSize = sizeof(DSBUFFERDESC);
		dsbd.dwFlags = DSBCAPS_STATIC;

		//Проигрывание
		void* Audio = NULL;
		DWORD Size = 0, Status = 0;
		IDirectSoundBuffer* SoundBuffer = NULL;
		DirectSoundInterface->CreateSoundBuffer(&dsbd, &SoundBuffer, NULL);
		SoundBuffer->Lock(0, 0, &Audio, &Size, NULL, NULL, DSBLOCK_ENTIREBUFFER);
		memcpy(Audio, Data, Size);
		SoundBuffer->Unlock(Audio, Size, NULL, 0);
		SoundBuffer->Play(0, 0, 0);
		do SoundBuffer->GetStatus(&Status); while ((Status == DSBSTATUS_PLAYING));
		delete[] Data;
	}

	cout << "Press any key to continue..." << endl;
	cin.sync(); cin.get();
}