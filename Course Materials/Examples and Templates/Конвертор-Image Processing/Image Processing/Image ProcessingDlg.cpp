// Image ProcessingDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "Image Processing.h"
#include "Image ProcessingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// диалоговое окно CImageProcessingDlg




CImageProcessingDlg::CImageProcessingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImageProcessingDlg::IDD, pParent)
	, b_DrawImage(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageProcessingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MENU, m_TabMenu);
}

BEGIN_MESSAGE_MAP(CImageProcessingDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// обработчики сообщений CImageProcessingDlg

BOOL CImageProcessingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Задает значок для этого диалогового окна. Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	b_DrawImage = false;
	m_TabMenu.m_Main = this;
	m_TabMenu.InitDialogs();
	m_TabMenu.InsertItem(0, L"File");
	m_TabMenu.InsertItem(1, L"Pixel Filters");
	m_TabMenu.InsertItem(2, L"Convolution Filters");
	m_TabMenu.InsertItem(3, L"Displacement &&&& Noise Filters");
	m_TabMenu.ActivateTabDialogs();

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок. Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей средой.

void CImageProcessingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	if (b_DrawImage) DrawImage();
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CImageProcessingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CImageProcessingDlg::DrawImage(void)
{
	b_DrawImage = true;
	CRect rImage;
	CWnd* pImage = GetDlgItem(IDC_IMAGE_BOX);
	pImage->GetWindowRect(&rImage);
	ScreenToClient(&rImage);
	rImage.DeflateRect(2,2,2,2);
	/*-----*/
	CBitmap* bImage = CBitmap::FromHandle(m_Image.operator HBITMAP());
	CClientDC dc(this);
	CDC* mdc = new CDC;
	mdc->CreateCompatibleDC(&dc);
	mdc->SelectObject(bImage);
	BITMAP bmp; bImage->GetBitmap(&bmp);
	dc.StretchBlt(rImage.TopLeft().x,rImage.TopLeft().y,rImage.Width(),rImage.Height(),mdc,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	delete mdc;
}
