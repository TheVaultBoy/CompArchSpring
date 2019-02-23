// MenuPage2.cpp: файл реализации
//

#include "stdafx.h"
#include "Image Processing.h"
#include "Image ProcessingDlg.h"
#include "MenuPage2.h"


// диалоговое окно CMenuPage2

IMPLEMENT_DYNAMIC(CMenuPage2, CDialog)

CMenuPage2::CMenuPage2(CWnd* pParent /*=NULL*/)
	: CDialog(CMenuPage2::IDD, pParent)
{
}

CMenuPage2::~CMenuPage2()
{
}

void CMenuPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIXEL_FILTERS, m_PixelFilters);
	m_PixelFilters.AddString(L"Invert");
	m_PixelFilters.AddString(L"GrayScale");
	m_PixelFilters.AddString(L"Brightness");
	m_PixelFilters.AddString(L"Contrast");
	m_PixelFilters.AddString(L"Gamma");
	m_PixelFilters.AddString(L"Color");
}


BEGIN_MESSAGE_MAP(CMenuPage2, CDialog)
//	ON_LBN_SELCANCEL(IDC_PIXEL_FILTERS, &CMenuPage2::OnLbnSelcancelPixelFilters)
	ON_EN_CHANGE(IDC_PF_P1, &CMenuPage2::OnEnChangePfP1)
	ON_EN_CHANGE(IDC_PF_P2, &CMenuPage2::OnEnChangePfP2)
	ON_EN_CHANGE(IDC_PF_P3, &CMenuPage2::OnEnChangePfP3)
	ON_LBN_SELCHANGE(IDC_PIXEL_FILTERS, &CMenuPage2::OnLbnSelchangePixelFilters)
	ON_BN_CLICKED(IDC_BUTTON_PF_APPLY, &CMenuPage2::OnBnClickedButtonPfApply)
END_MESSAGE_MAP()


// обработчики сообщений CMenuPage2

//void CMenuPage2::OnLbnSelcancelPixelFilters()
//{
//}

void CMenuPage2::OnEnChangePfP1()
{
	CString temp;
	CEdit* edt = (CEdit*)GetDlgItem(IDC_PF_P1);
	edt->GetWindowTextW(temp);
	v1 = _wtof(temp.GetString());
}

void CMenuPage2::OnEnChangePfP2()
{
	CString temp;
	CEdit* edt = (CEdit*)GetDlgItem(IDC_PF_P2);
	edt->GetWindowTextW(temp);
	v2 = _wtof(temp.GetString());
}

void CMenuPage2::OnEnChangePfP3()
{
	CString temp;
	CEdit* edt = (CEdit*)GetDlgItem(IDC_PF_P3);
	edt->GetWindowTextW(temp);
	v3 = _wtof(temp.GetString());
}

void CMenuPage2::OnLbnSelchangePixelFilters()
{
	CEdit* e1 = (CEdit*)GetDlgItem(IDC_PF_P1);
	CEdit* e2 = (CEdit*)GetDlgItem(IDC_PF_P2);
	CEdit* e3 = (CEdit*)GetDlgItem(IDC_PF_P3);
	e1->SetReadOnly(true);
	e2->SetReadOnly(true);
	e3->SetReadOnly(true);
	switch(m_PixelFilters.GetCurSel())
	{
	case 2:
		e1->SetReadOnly(false);
		e1->SetWindowTextW(L"100");
		break;
	case 3:
		e1->SetReadOnly(false);
		e1->SetWindowTextW(L"150");
		break;
	case 4:
		e1->SetReadOnly(false);
		e2->SetReadOnly(false);
		e3->SetReadOnly(false);
		e1->SetWindowTextW(L"0.5");
		e2->SetWindowTextW(L"0.5");
		e3->SetWindowTextW(L"0.5");
		break;
	case 5:
		e1->SetReadOnly(false);
		e2->SetReadOnly(false);
		e3->SetReadOnly(false);
		e1->SetWindowTextW(L"-255");
		e2->SetWindowTextW(L"-255");
		e3->SetWindowTextW(L"0");
		break;
	default:
		break;
	}
}

void CMenuPage2::OnBnClickedButtonPfApply()
{
	CBitmap* bImage = CBitmap::FromHandle(m_Image.operator HBITMAP());
	BITMAP bmp; bImage->GetBitmap(&bmp);
	switch(m_PixelFilters.GetCurSel())
	{
	case 0: Invert(bmp); break;
	case 1: GrayScale(bmp); break;
	case 2: Brightness(bmp, (int)v1); break;
	case 3: Contrast(bmp, (int)v1); break;
	case 4: Gamma(bmp, v1, v2, v3); break;
	case 5: Color(bmp, (int)v1, (int)v2, (int)v3); break;
	default: break;
	}
	(dynamic_cast<CImageProcessingDlg*>(m_Main))->DrawImage();
}
