// ������ ��������� ��� ������� ����� � ���������� � ������ �� �����. ������� ������� ��� ������ �.

#include "stdafx.h"
#include <dshow.h> // ������������ ���� DirectShow

// ����������� ���������!
#pragma comment(lib,"strmiids.lib")

// ����� ����� ������ ������� ���� � ������������
#define DEFAULT_VIDEO_WIDTH     320
#define DEFAULT_VIDEO_HEIGHT    320

enum PLAYSTATE {Stopped, Paused, Running, Init};
PLAYSTATE psCurrent = Stopped;							// ���������� ��������� ����� ������

// ���������� DirectShow
IMediaControl *pMediaControl = NULL;					// ���������� ������
IGraphBuilder *pGraphBuilder = NULL;					// ��� ���� ������
ICaptureGraphBuilder2 *pCaptureGraphBuilder2 = NULL;	// ���� ������� �����
IVideoWindow *pVideoWindow = NULL;						// ����, � ������� ��������� ����� �����
IMoniker *pMonikerVideo = NULL;							// ���������� ������������
IBaseFilter *pVideoCaptureFilter = NULL;				// ������ ������������


void SetupVideoWindow(void)	// ��������� ���� ��� ������ ����� �� �����
{
	pVideoWindow->put_Left(0); 
	pVideoWindow->put_Width(DEFAULT_VIDEO_WIDTH); 
	pVideoWindow->put_Top(0); 
	pVideoWindow->put_Height(DEFAULT_VIDEO_HEIGHT); 
	pVideoWindow->put_Caption(L"Video Window");
}

HRESULT GetInterfaces(void)		// ������������� ���������� DirectShow
{
    HRESULT hr;

    // �������� ����� ��������
    hr = CoCreateInstance (CLSID_FilterGraph, NULL, CLSCTX_INPROC,
        IID_IGraphBuilder, (void **) &pGraphBuilder);
    if (FAILED(hr))
        return hr;

    // �������� ����� ������� �����
    hr = CoCreateInstance (CLSID_CaptureGraphBuilder2 , NULL, CLSCTX_INPROC,
        IID_ICaptureGraphBuilder2, (void **) &pCaptureGraphBuilder2);
    if (FAILED(hr))
        return hr;

	// ������� ���������� ��� ���������� ������ � �����
    hr = pGraphBuilder->QueryInterface(IID_IMediaControl,(LPVOID *) &pMediaControl);
    if (FAILED(hr))
        return hr;

    hr = pGraphBuilder->QueryInterface(IID_IVideoWindow, (LPVOID *) &pVideoWindow);
    if (FAILED(hr))
        return hr;
  
	return hr;
}

void CloseInterfaces(void)		// �������� ����������� DirectShow
{
    // ���������� �����
	if (pMediaControl)
		pMediaControl->StopWhenReady();
    psCurrent = Stopped;

    // �������� ����
    if(pVideoWindow) pVideoWindow->put_Visible(OAFALSE);
   
	// �������� ��������� �����������
    pMediaControl->Release();
	pGraphBuilder->Release();
	pVideoWindow->Release();
	pCaptureGraphBuilder2->Release();
}

HRESULT InitMonikers()		// ������������� ���������� ������������
{
	HRESULT hr;
	ULONG cFetched;

	ICreateDevEnum *pCreateDevEnum;		// ��������� ��� ����������� � ������������ ���� ��������� ���������
    hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pCreateDevEnum);
	if (FAILED(hr))
    {
		printf("Failed to enumerate all video and audio capture devices!  hr=0x%x\n", hr);
		return hr;
    }

	IEnumMoniker *pEnumMoniker;			// ���������, ������� ����� ��������� ������ ���� ��������� ���������

	//	����������� ������������ ��������� ������������
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,&pEnumMoniker, 0);
	if (FAILED(hr))
    {
		printf("Failed to create ClassEnumerator!  hr=0x%x\n", hr);
        return hr;
    }

	// ����� ��������� ���������� � ������ pMonikerVideo. �� ����� ������ �� ������ ��� ��������
	hr = pEnumMoniker->Next(1, &pMonikerVideo, &cFetched);
	if (S_OK == hr)
    {
		// ��������� ���������� � ����� �������� ������� �����
        hr = pMonikerVideo->BindToObject(0,0,IID_IBaseFilter, (void**)&pVideoCaptureFilter);
		if (FAILED(hr))
		{
			printf("Couldn't bind moniker to filter object!  hr=0x%x\n", hr);
			return hr;
		}
    }
	pEnumMoniker->Release();
}

HRESULT CaptureVideo()		// ������ �����
{
	HRESULT hr = CoInitialize(NULL);	// ������������� DirectShow
	
	// ������������� ����������� �����������
	hr = GetInterfaces();
	if (FAILED(hr))
    {
		printf("Failed to get video interfaces!  hr=0x%x\n", hr);
        return hr;
    }
	
	// ����������� ���� ������� ����� � ��������� ����� ��������
	hr = pCaptureGraphBuilder2->SetFiltergraph(pGraphBuilder);
	if (FAILED(hr))
    {
		printf("Failed to attach the filter graph to the capture graph!  hr=0x%x\n", hr);
        return hr;
    }
	
	// ����������� � ���������� �������
	InitMonikers();
	
	// ��������� ������ ������� ����� �� ���� ��������
	hr = pGraphBuilder->AddFilter(pVideoCaptureFilter, L"Video Capture");
    if (FAILED(hr))
    {
        printf("Couldn't add video capture filter to graph!  hr=0x%x\n", hr);
        pVideoCaptureFilter->Release();
        return hr;
    }
	
	// ���������� ���� ������ � ����������
	hr = pCaptureGraphBuilder2->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, pVideoCaptureFilter, 0, 0);
	if (FAILED(hr))
    {
        printf("Couldn't render video capture stream. The device may already be in use.  hr=0x%x\n", hr);
        pVideoCaptureFilter->Release();
        return hr;
    }

	// ���� ������ ������� ����� ������ �� �����������
	pVideoCaptureFilter->Release();

	// ������������� ���� ��� ������ �����������
    SetupVideoWindow();
    	
	// �������� ������ �����
	hr = pMediaControl->Run();
	if (FAILED(hr))
    {
        printf("Couldn't run the graph!  hr=0x%x\n", hr);
        return hr;
    }
	else psCurrent = Running;

	return hr;
}

void StopPreview()	// ���������� �����
{
	// ��������� ��� ���������� DirectShow
    CloseInterfaces();
	CoUninitialize();
	psCurrent = Stopped;
}

int main()
{
	HRESULT hr;														// ��������� ��������. ����� ��������� ��� ������
	char cmd;														// ��� �������, �������� �������������

	printf("p - Play Video\ns - Stop Video\nq - Quit\n\n");

	while (true)
	{
		std::cin >> cmd;
		switch(cmd)
		{
		case 'p': {													// ������ �����
					printf("	Play Video!\n");
					hr = CaptureVideo();
					if (FAILED(hr))	printf("Error!");
				}break;
		case 's': {													// ���������� �����
					printf("	Stop Video!\n");
                	if (psCurrent == Running) StopPreview();
					else printf ("Video already stopped.\n");
				}break;
		case 'q': return 0;											// ����� �� ���������
			break;
		default: printf("Unknown command!\n");
			break;
		}
	}
}