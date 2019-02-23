// Image ProcessingDlg.cpp : ���� ����������
//

#include "stdafx.h"
#include "Image Processing.h"
#include "Image ProcessingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���������� ���� CImageProcessingDlg




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


// ����������� ��������� CImageProcessingDlg

BOOL CImageProcessingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ������ ������ ��� ����� ����������� ����. ����� ������ ��� �������������,
	//  ���� ������� ���� ���������� �� �������� ����������
	SetIcon(m_hIcon, TRUE);			// ������� ������
	SetIcon(m_hIcon, FALSE);		// ������ ������

	// TODO: �������� �������������� �������������
	b_DrawImage = false;
	m_TabMenu.m_Main = this;
	m_TabMenu.InitDialogs();
	m_TabMenu.InsertItem(0, L"File");
	m_TabMenu.InsertItem(1, L"Pixel Filters");
	m_TabMenu.InsertItem(2, L"Convolution Filters");
	m_TabMenu.InsertItem(3, L"Displacement &&&& Noise Filters");
	m_TabMenu.ActivateTabDialogs();

	return TRUE;  // ������� �������� TRUE, ���� ����� �� ������� �������� ����������
}

// ��� ���������� ������ ����������� � ���������� ���� ����� ��������������� ����������� ���� �����,
//  ����� ���������� ������. ��� ���������� MFC, ������������ ������ ���������� ��� �������������,
//  ��� ������������� ����������� ������� ������.

void CImageProcessingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �������� ���������� ��� ���������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ������������ ������ �� ������ ����������� ��������������
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ��������� ������
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	if (b_DrawImage) DrawImage();
}

// ������� �������� ��� ������� ��� ��������� ����������� ������� ��� �����������
//  ���������� ����.
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
