// MenuPage4.cpp: файл реализации
//

#include "stdafx.h"
#include "Image Processing.h"
#include "Image ProcessingDlg.h"
#include "MenuPage4.h"


// диалоговое окно CMenuPage4

IMPLEMENT_DYNAMIC(CMenuPage4, CDialog)

CMenuPage4::CMenuPage4(CWnd* pParent /*=NULL*/)
	: CDialog(CMenuPage4::IDD, pParent)
{

}

CMenuPage4::~CMenuPage4()
{
}

void CMenuPage4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DISPLACEMENT_FILTERS, m_DisplacementFilters);
	m_DisplacementFilters.AddString(L"Flip");
	m_DisplacementFilters.AddString(L"RandomJitter");
	m_DisplacementFilters.AddString(L"Swirl");
	m_DisplacementFilters.AddString(L"Sphere");
	m_DisplacementFilters.AddString(L"TimeWarp");
	m_DisplacementFilters.AddString(L"Moire");
	m_DisplacementFilters.AddString(L"Water");
	m_DisplacementFilters.AddString(L"Pixelate");
	m_DisplacementFilters.AddString(L"NoiseSaltAndPepper");
	m_DisplacementFilters.AddString(L"Median");
}


BEGIN_MESSAGE_MAP(CMenuPage4, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DF_APPLY, &CMenuPage4::OnBnClickedButtonDfApply)
	ON_LBN_SELCHANGE(IDC_DISPLACEMENT_FILTERS, &CMenuPage4::OnLbnSelchangeDisplacementFilters)
	ON_EN_CHANGE(IDC_DF_P1, &CMenuPage4::OnEnChangeDfP1)
END_MESSAGE_MAP()


// обработчики сообщений CMenuPage4

void CMenuPage4::OnBnClickedButtonDfApply()
{
	bool b1, b2, b3;
	b1 = (bool)((CButton*)GetDlgItem(IDC_DF_P2))->GetCheck();
	b2 = (bool)((CButton*)GetDlgItem(IDC_DF_P3))->GetCheck();
	b3 = (bool)((CButton*)GetDlgItem(IDC_DF_P4))->GetCheck();
	CBitmap* bImage = CBitmap::FromHandle(m_Image.operator HBITMAP());
	BITMAP bmp; bImage->GetBitmap(&bmp);
	switch(m_DisplacementFilters.GetCurSel())
	{
	case 0: Flip(bmp, b1, b2); break;
	case 1: RandomJitter(bmp, (int)k); break;
	case 2: Swirl(bmp, k); break;
	case 3: Sphere(bmp); break;
	case 4: TimeWarp(bmp, (BYTE)k); break;
	case 5: Moire(bmp, k); break;
	case 6: Water(bmp, (int)k); break;
	case 7: Pixelate(bmp, (int)k, b3); break;
	case 8: NoiseSaltAndPepper(bmp, (int)k); break;
	case 9: Median(bmp, (int)k); break;
	default: break;
	}
	(dynamic_cast<CImageProcessingDlg*>(m_Main))->DrawImage();
}

void CMenuPage4::OnLbnSelchangeDisplacementFilters()
{
	CEdit* e = (CEdit*)GetDlgItem(IDC_DF_P1);
	e->SetReadOnly(false);
	switch(m_DisplacementFilters.GetCurSel())
	{
	case 0: e->SetReadOnly(true); break;
	case 1: e->SetWindowTextW(L"5"); break;
	case 2: e->SetWindowTextW(L"0.01"); break;
	case 3: e->SetReadOnly(true); break;
	case 4: e->SetWindowTextW(L"10"); break;
	case 5: e->SetWindowTextW(L"5.0"); break;
	case 6: e->SetWindowTextW(L"10"); break;
	case 7: e->SetWindowTextW(L"15"); break;
	case 8: e->SetWindowTextW(L"25"); break;
	case 9: e->SetWindowTextW(L"1"); break;
	default: break;
	}
}

void CMenuPage4::OnEnChangeDfP1()
{
	CString temp;
	CEdit* edt = (CEdit*)GetDlgItem(IDC_DF_P1);
	edt->GetWindowTextW(temp);
	k = _wtof(temp.GetString());
}
