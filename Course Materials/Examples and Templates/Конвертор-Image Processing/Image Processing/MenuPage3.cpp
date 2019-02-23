// MenuPage3.cpp: файл реализации
//

#include "stdafx.h"
#include "Image Processing.h"
#include "Image ProcessingDlg.h"
#include "MenuPage3.h"


// диалоговое окно CMenuPage3

IMPLEMENT_DYNAMIC(CMenuPage3, CDialog)

CMenuPage3::CMenuPage3(CWnd* pParent /*=NULL*/)
	: CDialog(CMenuPage3::IDD, pParent)
{

}

CMenuPage3::~CMenuPage3()
{
}

void CMenuPage3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONVOLUTION_FILTERS, m_ConvolutionFilters);
	m_ConvolutionFilters.AddString(L"Custom");
	m_ConvolutionFilters.AddString(L"Negative");
	m_ConvolutionFilters.AddString(L"Blur");
	m_ConvolutionFilters.AddString(L"Sharpen");
	m_ConvolutionFilters.AddString(L"Emboss");
	m_ConvolutionFilters.AddString(L"LightBlur");
	m_ConvolutionFilters.AddString(L"LightSharpen");
	m_ConvolutionFilters.AddString(L"LightEmboss");
	m_ConvolutionFilters.AddString(L"GaussianBlur");
	m_ConvolutionFilters.AddString(L"MeanRemoval");
	m_ConvolutionFilters.AddString(L"EmbossLaplacian");
	m_ConvolutionFilters.AddString(L"EdgeDetection");
	m_ConvolutionFilters.AddString(L"Sobel");
}


BEGIN_MESSAGE_MAP(CMenuPage3, CDialog)
	ON_LBN_SELCHANGE(IDC_CONVOLUTION_FILTERS, &CMenuPage3::OnLbnSelchangeConvolutionFilters)
	ON_EN_CHANGE(IDC_CF_M1, &CMenuPage3::OnEnChangeCfM1)
	ON_EN_CHANGE(IDC_CF_M2, &CMenuPage3::OnEnChangeCfM2)
	ON_EN_CHANGE(IDC_CF_M3, &CMenuPage3::OnEnChangeCfM3)
	ON_EN_CHANGE(IDC_CF_M4, &CMenuPage3::OnEnChangeCfM4)
	ON_EN_CHANGE(IDC_CF_M5, &CMenuPage3::OnEnChangeCfM5)
	ON_EN_CHANGE(IDC_CF_M6, &CMenuPage3::OnEnChangeCfM6)
	ON_EN_CHANGE(IDC_CF_M7, &CMenuPage3::OnEnChangeCfM7)
	ON_EN_CHANGE(IDC_CF_M8, &CMenuPage3::OnEnChangeCfM8)
	ON_EN_CHANGE(IDC_CF_M9, &CMenuPage3::OnEnChangeCfM9)
	ON_EN_CHANGE(IDC_CF_M10, &CMenuPage3::OnEnChangeCfM10)
	ON_EN_CHANGE(IDC_CF_M11, &CMenuPage3::OnEnChangeCfM11)
	ON_BN_CLICKED(IDC_BUTTON_CF_APPLY, &CMenuPage3::OnBnClickedButtonCfApply)
END_MESSAGE_MAP()


// обработчики сообщений CMenuPage3

void CMenuPage3::OnLbnSelchangeConvolutionFilters()
{
	CEdit* e1 = (CEdit*)GetDlgItem(IDC_CF_M1);
	CEdit* e2 = (CEdit*)GetDlgItem(IDC_CF_M2);
	CEdit* e3 = (CEdit*)GetDlgItem(IDC_CF_M3);
	CEdit* e4 = (CEdit*)GetDlgItem(IDC_CF_M4);
	CEdit* e5 = (CEdit*)GetDlgItem(IDC_CF_M5);
	CEdit* e6 = (CEdit*)GetDlgItem(IDC_CF_M6);
	CEdit* e7 = (CEdit*)GetDlgItem(IDC_CF_M7);
	CEdit* e8 = (CEdit*)GetDlgItem(IDC_CF_M8);
	CEdit* e9 = (CEdit*)GetDlgItem(IDC_CF_M9);
	CEdit* e10 = (CEdit*)GetDlgItem(IDC_CF_M10);
	CEdit* e11 = (CEdit*)GetDlgItem(IDC_CF_M11);
	e1->SetReadOnly(true); e2->SetReadOnly(true); e3->SetReadOnly(true);
	e4->SetReadOnly(true); e5->SetReadOnly(true); e6->SetReadOnly(true);
	e7->SetReadOnly(true); e8->SetReadOnly(true); e9->SetReadOnly(true);
	e10->SetReadOnly(true); e11->SetReadOnly(true);
	switch(m_ConvolutionFilters.GetCurSel())
	{
	case 0:
		e1->SetReadOnly(false); e2->SetReadOnly(false); e3->SetReadOnly(false);
		e4->SetReadOnly(false); e5->SetReadOnly(false); e6->SetReadOnly(false);
		e7->SetReadOnly(false); e8->SetReadOnly(false); e9->SetReadOnly(false);
		e10->SetReadOnly(false); e11->SetReadOnly(false);
		break;
	case 1:
		e1->SetWindowTextW(L"0"); e2->SetWindowTextW(L"0"); e3->SetWindowTextW(L"0");
		e4->SetWindowTextW(L"0"); e5->SetWindowTextW(L"-1"); e6->SetWindowTextW(L"0");
		e7->SetWindowTextW(L"0"); e8->SetWindowTextW(L"0"); e9->SetWindowTextW(L"0");
		e10->SetWindowTextW(L"1"); e11->SetWindowTextW(L"255");
		break;
	case 2:
		e1->SetWindowTextW(L"1"); e2->SetWindowTextW(L"1"); e3->SetWindowTextW(L"1");
		e4->SetWindowTextW(L"1"); e5->SetWindowTextW(L"1"); e6->SetWindowTextW(L"1");
		e7->SetWindowTextW(L"1"); e8->SetWindowTextW(L"1"); e9->SetWindowTextW(L"1");
		e10->SetWindowTextW(L"9"); e11->SetWindowTextW(L"0");
		break;
	case 3:
		e1->SetWindowTextW(L"0"); e2->SetWindowTextW(L"-1"); e3->SetWindowTextW(L"0");
		e4->SetWindowTextW(L"-1"); e5->SetWindowTextW(L"5"); e6->SetWindowTextW(L"-1");
		e7->SetWindowTextW(L"0"); e8->SetWindowTextW(L"-1"); e9->SetWindowTextW(L"0");
		e10->SetWindowTextW(L"1"); e11->SetWindowTextW(L"0");
		break;
	case 4:
		e1->SetWindowTextW(L"-2"); e2->SetWindowTextW(L"-1"); e3->SetWindowTextW(L"0");
		e4->SetWindowTextW(L"-1"); e5->SetWindowTextW(L"1"); e6->SetWindowTextW(L"1");
		e7->SetWindowTextW(L"0"); e8->SetWindowTextW(L"1"); e9->SetWindowTextW(L"2");
		e10->SetWindowTextW(L"1"); e11->SetWindowTextW(L"0");
		break;
	case 5:
		e1->SetWindowTextW(L"1"); e2->SetWindowTextW(L"1"); e3->SetWindowTextW(L"0");
		e4->SetWindowTextW(L"1"); e5->SetWindowTextW(L"1"); e6->SetWindowTextW(L"0");
		e7->SetWindowTextW(L"0"); e8->SetWindowTextW(L"0"); e9->SetWindowTextW(L"0");
		e10->SetWindowTextW(L"4"); e11->SetWindowTextW(L"0");
		break;
	case 6:
		e1->SetWindowTextW(L"-1"); e2->SetWindowTextW(L"0"); e3->SetWindowTextW(L"0");
		e4->SetWindowTextW(L"0"); e5->SetWindowTextW(L"2"); e6->SetWindowTextW(L"0");
		e7->SetWindowTextW(L"0"); e8->SetWindowTextW(L"0"); e9->SetWindowTextW(L"0");
		e10->SetWindowTextW(L"1"); e11->SetWindowTextW(L"0");
		break;
	case 7:
		e1->SetWindowTextW(L"1"); e2->SetWindowTextW(L"0"); e3->SetWindowTextW(L"0");
		e4->SetWindowTextW(L"0"); e5->SetWindowTextW(L"1"); e6->SetWindowTextW(L"0");
		e7->SetWindowTextW(L"0"); e8->SetWindowTextW(L"0"); e9->SetWindowTextW(L"-1");
		e10->SetWindowTextW(L"1"); e11->SetWindowTextW(L"0");
		break;
	case 8:
		e1->SetWindowTextW(L"1"); e2->SetWindowTextW(L"2"); e3->SetWindowTextW(L"1");
		e4->SetWindowTextW(L"2"); e5->SetWindowTextW(L"4"); e6->SetWindowTextW(L"2");
		e7->SetWindowTextW(L"1"); e8->SetWindowTextW(L"2"); e9->SetWindowTextW(L"1");
		e10->SetWindowTextW(L"16"); e11->SetWindowTextW(L"0");
		break;
	case 9:
		e1->SetWindowTextW(L"-1"); e2->SetWindowTextW(L"-1"); e3->SetWindowTextW(L"-1");
		e4->SetWindowTextW(L"-1"); e5->SetWindowTextW(L"9"); e6->SetWindowTextW(L"-1");
		e7->SetWindowTextW(L"-1"); e8->SetWindowTextW(L"-1"); e9->SetWindowTextW(L"-1");
		e10->SetWindowTextW(L"1"); e11->SetWindowTextW(L"0");
		break;
	case 10:
		e1->SetWindowTextW(L"-1"); e2->SetWindowTextW(L"0"); e3->SetWindowTextW(L"-1");
		e4->SetWindowTextW(L"0"); e5->SetWindowTextW(L"4"); e6->SetWindowTextW(L"0");
		e7->SetWindowTextW(L"-1"); e8->SetWindowTextW(L"0"); e9->SetWindowTextW(L"-1");
		e10->SetWindowTextW(L"1"); e11->SetWindowTextW(L"127");
		break;
	case 11:
		e1->SetWindowTextW(L"1"); e2->SetWindowTextW(L"1"); e3->SetWindowTextW(L"1");
		e4->SetWindowTextW(L"0"); e5->SetWindowTextW(L"0"); e6->SetWindowTextW(L"0");
		e7->SetWindowTextW(L"-1"); e8->SetWindowTextW(L"-1"); e9->SetWindowTextW(L"-1");
		e10->SetWindowTextW(L"1"); e11->SetWindowTextW(L"127");
		break;
	case 12:
		e1->SetWindowTextW(L"1"); e2->SetWindowTextW(L"0"); e3->SetWindowTextW(L"-1");
		e4->SetWindowTextW(L"2"); e5->SetWindowTextW(L"0"); e6->SetWindowTextW(L"-2");
		e7->SetWindowTextW(L"1"); e8->SetWindowTextW(L"0"); e9->SetWindowTextW(L"-1");
		e10->SetWindowTextW(L"1"); e11->SetWindowTextW(L"0");
		break;
	default:
		break;
	}
}

void CMenuPage3::OnEnChangeCfM1()
{
	CString temp;
	CEdit* edt = (CEdit*)GetDlgItem(IDC_CF_M1);
	edt->GetWindowTextW(temp);
	m1 = (int)_wtof(temp.GetString());
}

void CMenuPage3::OnEnChangeCfM2()
{
	CString temp;
	CEdit* edt = (CEdit*)GetDlgItem(IDC_CF_M2);
	edt->GetWindowTextW(temp);
	m2 = (int)_wtof(temp.GetString());
}

void CMenuPage3::OnEnChangeCfM3()
{
	CString temp;
	CEdit* edt = (CEdit*)GetDlgItem(IDC_CF_M3);
	edt->GetWindowTextW(temp);
	m3 = (int)_wtof(temp.GetString());
}

void CMenuPage3::OnEnChangeCfM4()
{
	CString temp;
	CEdit* edt = (CEdit*)GetDlgItem(IDC_CF_M4);
	edt->GetWindowTextW(temp);
	m4 = (int)_wtof(temp.GetString());
}

void CMenuPage3::OnEnChangeCfM5()
{
	CString temp;
	CEdit* edt = (CEdit*)GetDlgItem(IDC_CF_M5);
	edt->GetWindowTextW(temp);
	m5 = (int)_wtof(temp.GetString());
}

void CMenuPage3::OnEnChangeCfM6()
{
	CString temp;
	CEdit* edt = (CEdit*)GetDlgItem(IDC_CF_M6);
	edt->GetWindowTextW(temp);
	m6 = (int)_wtof(temp.GetString());
}

void CMenuPage3::OnEnChangeCfM7()
{
	CString temp;
	CEdit* edt = (CEdit*)GetDlgItem(IDC_CF_M7);
	edt->GetWindowTextW(temp);
	m7 = (int)_wtof(temp.GetString());
}

void CMenuPage3::OnEnChangeCfM8()
{
	CString temp;
	CEdit* edt = (CEdit*)GetDlgItem(IDC_CF_M8);
	edt->GetWindowTextW(temp);
	m8 = (int)_wtof(temp.GetString());
}

void CMenuPage3::OnEnChangeCfM9()
{
	CString temp;
	CEdit* edt = (CEdit*)GetDlgItem(IDC_CF_M9);
	edt->GetWindowTextW(temp);
	m9 = (int)_wtof(temp.GetString());
}

void CMenuPage3::OnEnChangeCfM10()
{
	CString temp;
	CEdit* edt = (CEdit*)GetDlgItem(IDC_CF_M10);
	edt->GetWindowTextW(temp);
	div = (int)_wtof(temp.GetString());
}

void CMenuPage3::OnEnChangeCfM11()
{
	CString temp;
	CEdit* edt = (CEdit*)GetDlgItem(IDC_CF_M11);
	edt->GetWindowTextW(temp);
	offset = (int)_wtof(temp.GetString());
}

void CMenuPage3::OnBnClickedButtonCfApply()
{
	CBitmap* bImage = CBitmap::FromHandle(m_Image.operator HBITMAP());
	BITMAP bmp; bImage->GetBitmap(&bmp);
	int matrix[] = {m1,m2,m3,m4,m5,m6,m7,m8,m9};
	Convolution3x3(bmp,matrix,div,offset);
	(dynamic_cast<CImageProcessingDlg*>(m_Main))->DrawImage();
}
