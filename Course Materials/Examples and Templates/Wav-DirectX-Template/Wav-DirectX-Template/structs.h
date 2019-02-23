#include <windows.h>
#include "dsound.h"
#include <cmath>
#include <string>
#include <iostream>
using namespace std;
#pragma	comment(lib,"dsound.lib")

typedef struct waveformatm_tag
{
    WORD wFormatTag;
    WORD nChannels;
    DWORD nSamplesPerSec;
    DWORD nAvgBytesPerSec;
    WORD nBlockAlign;
	WORD wBitsPerSample;
} WAVEFORMATM;

typedef struct datablock_tag
{
	DWORD Tag;
	DWORD Length;
} DATABLOCK;

typedef struct fmtblock_tag
{
	DWORD Tag;
	DWORD Length;
	WAVEFORMATM Format;
	DATABLOCK Data;
} FMTBLOCK;

typedef struct waveblock_tag
{
	DWORD Tag;
	FMTBLOCK Fmt;
} WAVEBLOCK;

typedef struct riffblock_tag
{
	DWORD Tag;
	DWORD Length;
	WAVEBLOCK Wave;
} RIFFBLOCK;