// Шаблон программы для захвата видео с устройства и вывода на экран. Студент кафедры ИУС Чуркин В.

#include "stdafx.h"
#include <dshow.h> // Заголовочный файл DirectShow

// Необходимые билиотеки!
#pragma comment(lib,"strmiids.lib")

// Здесь можно задать размеры окна с изображением
#define DEFAULT_VIDEO_WIDTH     320
#define DEFAULT_VIDEO_HEIGHT    320

enum PLAYSTATE {Stopped, Paused, Running, Init};
PLAYSTATE psCurrent = Stopped;							// Переменная состояния видео потока

// Интерфейсы DirectShow
IMediaControl *pMediaControl = NULL;					// Управление графом
IGraphBuilder *pGraphBuilder = NULL;					// Наш граф вцелом
ICaptureGraphBuilder2 *pCaptureGraphBuilder2 = NULL;	// Граф захвата видео
IVideoWindow *pVideoWindow = NULL;						// Окно, в которое выводится видео поток
IMoniker *pMonikerVideo = NULL;							// Устройство видеозахвата
IBaseFilter *pVideoCaptureFilter = NULL;				// Фильтр видеозахвата


void SetupVideoWindow(void)	// Настройка окна для вывода видео на экран
{
	pVideoWindow->put_Left(0); 
	pVideoWindow->put_Width(DEFAULT_VIDEO_WIDTH); 
	pVideoWindow->put_Top(0); 
	pVideoWindow->put_Height(DEFAULT_VIDEO_HEIGHT); 
	pVideoWindow->put_Caption(L"Video Window");
}

HRESULT GetInterfaces(void)		// Инициализация интерфесов DirectShow
{
    HRESULT hr;

    // Создание графа фильтров
    hr = CoCreateInstance (CLSID_FilterGraph, NULL, CLSCTX_INPROC,
        IID_IGraphBuilder, (void **) &pGraphBuilder);
    if (FAILED(hr))
        return hr;

    // Создание графа захвата видео
    hr = CoCreateInstance (CLSID_CaptureGraphBuilder2 , NULL, CLSCTX_INPROC,
        IID_ICaptureGraphBuilder2, (void **) &pCaptureGraphBuilder2);
    if (FAILED(hr))
        return hr;

	// Получим интерфейсы для управления графом и окном
    hr = pGraphBuilder->QueryInterface(IID_IMediaControl,(LPVOID *) &pMediaControl);
    if (FAILED(hr))
        return hr;

    hr = pGraphBuilder->QueryInterface(IID_IVideoWindow, (LPVOID *) &pVideoWindow);
    if (FAILED(hr))
        return hr;
  
	return hr;
}

void CloseInterfaces(void)		// Закрытие интерфейсов DirectShow
{
    // Остановить видео
	if (pMediaControl)
		pMediaControl->StopWhenReady();
    psCurrent = Stopped;

    // Закрытие окна
    if(pVideoWindow) pVideoWindow->put_Visible(OAFALSE);
   
	// Закрытие остальных интерфейсов
    pMediaControl->Release();
	pGraphBuilder->Release();
	pVideoWindow->Release();
	pCaptureGraphBuilder2->Release();
}

HRESULT InitMonikers()		// Инициализация устройства видеозахвата
{
	HRESULT hr;
	ULONG cFetched;

	ICreateDevEnum *pCreateDevEnum;		// Интерфейс для обнаружения и перечисления всех доступных устройств
    hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pCreateDevEnum);
	if (FAILED(hr))
    {
		printf("Failed to enumerate all video and audio capture devices!  hr=0x%x\n", hr);
		return hr;
    }

	IEnumMoniker *pEnumMoniker;			// Интерфейс, который будет содержать список всех доступных устройств

	//	Обнаружение подключенный устройств видеозахвата
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,&pEnumMoniker, 0);
	if (FAILED(hr))
    {
		printf("Failed to create ClassEnumerator!  hr=0x%x\n", hr);
        return hr;
    }

	// Выбор активного устройства в фильтр pMonikerVideo. Мы берем первое из списка для простоты
	hr = pEnumMoniker->Next(1, &pMonikerVideo, &cFetched);
	if (S_OK == hr)
    {
		// Соединяем устройство с нашим фильтром захвата видео
        hr = pMonikerVideo->BindToObject(0,0,IID_IBaseFilter, (void**)&pVideoCaptureFilter);
		if (FAILED(hr))
		{
			printf("Couldn't bind moniker to filter object!  hr=0x%x\n", hr);
			return hr;
		}
    }
	pEnumMoniker->Release();
}

HRESULT CaptureVideo()		// Захват видео
{
	HRESULT hr = CoInitialize(NULL);	// Инициализация DirectShow
	
	// Инициализация необходимых интерфейсов
	hr = GetInterfaces();
	if (FAILED(hr))
    {
		printf("Failed to get video interfaces!  hr=0x%x\n", hr);
        return hr;
    }
	
	// Присоединим граф захвата видео к основному графу фильтров
	hr = pCaptureGraphBuilder2->SetFiltergraph(pGraphBuilder);
	if (FAILED(hr))
    {
		printf("Failed to attach the filter graph to the capture graph!  hr=0x%x\n", hr);
        return hr;
    }
	
	// Подключение к устройству захвата
	InitMonikers();
	
	// Поместить фильтр захвата видео на граф фильтров
	hr = pGraphBuilder->AddFilter(pVideoCaptureFilter, L"Video Capture");
    if (FAILED(hr))
    {
        printf("Couldn't add video capture filter to graph!  hr=0x%x\n", hr);
        pVideoCaptureFilter->Release();
        return hr;
    }
	
	// Подключаем этот фильтр к устройству
	hr = pCaptureGraphBuilder2->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, pVideoCaptureFilter, 0, 0);
	if (FAILED(hr))
    {
        printf("Couldn't render video capture stream. The device may already be in use.  hr=0x%x\n", hr);
        pVideoCaptureFilter->Release();
        return hr;
    }

	// Этот фильтр захвата видео больше не понадобится
	pVideoCaptureFilter->Release();

	// Инициализация окна для вывода изображения
    SetupVideoWindow();
    	
	// Начинаем захват видео
	hr = pMediaControl->Run();
	if (FAILED(hr))
    {
        printf("Couldn't run the graph!  hr=0x%x\n", hr);
        return hr;
    }
	else psCurrent = Running;

	return hr;
}

void StopPreview()	// Остановить видео
{
	// Закрываем все интерфейсы DirectShow
    CloseInterfaces();
	CoUninitialize();
	psCurrent = Stopped;
}

int main()
{
	HRESULT hr;														// Результат операций. Будет содержать код ошибки
	char cmd;														// Код команды, вводимой пользователем

	printf("p - Play Video\ns - Stop Video\nq - Quit\n\n");

	while (true)
	{
		std::cin >> cmd;
		switch(cmd)
		{
		case 'p': {													// Запуск видео
					printf("	Play Video!\n");
					hr = CaptureVideo();
					if (FAILED(hr))	printf("Error!");
				}break;
		case 's': {													// Выключение видео
					printf("	Stop Video!\n");
                	if (psCurrent == Running) StopPreview();
					else printf ("Video already stopped.\n");
				}break;
		case 'q': return 0;											// Выход из программы
			break;
		default: printf("Unknown command!\n");
			break;
		}
	}
}