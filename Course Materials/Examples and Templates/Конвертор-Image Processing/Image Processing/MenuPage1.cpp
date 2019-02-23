// MenuPage1.cpp: файл реализации
//

#include "stdafx.h"
#include "Image Processing.h"
#include "Image ProcessingDlg.h"
#include "MenuPage1.h"
#include "MenuPage2.h"
#include "MenuPage3.h"
#include "MenuPage4.h"


// диалоговое окно CMenuPage1

IMPLEMENT_DYNAMIC(CMenuPage1, CDialog)

CMenuPage1::CMenuPage1(CWnd* pParent /*=NULL*/)
	: CDialog(CMenuPage1::IDD, pParent)
	, m_Path(_T(""))
{

}

CMenuPage1::~CMenuPage1()
{
}

void CMenuPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMenuPage1, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CMenuPage1::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_RELOAD, &CMenuPage1::OnBnClickedButtonReload)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CMenuPage1::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// обработчики сообщений CMenuPage1

void CMenuPage1::OnBnClickedButtonOpen()
{
	LPCTSTR Filter = L"Точечный рисунок (*.bmp)|*.bmp|";
	CFileDialog OpenImageDialog(true, L".bmp", NULL, 0, Filter, 0, 0, 0);
	if(OpenImageDialog.DoModal() == IDOK)
	{
		m_Path = OpenImageDialog.GetFileName();
		m_Image.Destroy();
		m_Image.Load(m_Path);
		CButton* btn = (CButton*)GetDlgItem(IDC_BUTTON_RELOAD);
		btn->EnableWindow(true);
		btn = (CButton*)GetDlgItem(IDC_BUTTON_SAVE);
		btn->EnableWindow(true);
		/*-----*/
		(dynamic_cast<CImageProcessingDlg*>(m_Main))->m_Image = m_Image;
		(dynamic_cast<CImageProcessingDlg*>(m_Main))->DrawImage();
		btn = (CButton*)((dynamic_cast<CMenuPage2*>((dynamic_cast<CImageProcessingDlg*>(m_Main))->m_TabMenu.m_Dialogs[1]))->GetDlgItem(IDC_BUTTON_PF_APPLY));
		btn->EnableWindow(true);
		btn = (CButton*)((dynamic_cast<CMenuPage3*>((dynamic_cast<CImageProcessingDlg*>(m_Main))->m_TabMenu.m_Dialogs[2]))->GetDlgItem(IDC_BUTTON_CF_APPLY));
		btn->EnableWindow(true);
		btn = (CButton*)((dynamic_cast<CMenuPage4*>((dynamic_cast<CImageProcessingDlg*>(m_Main))->m_TabMenu.m_Dialogs[3]))->GetDlgItem(IDC_BUTTON_DF_APPLY));
		btn->EnableWindow(true);
	}
}

void CMenuPage1::OnBnClickedButtonReload()
{
	m_Image.Destroy();
	m_Image.Load(m_Path);
	/*-----*/
	(dynamic_cast<CImageProcessingDlg*>(m_Main))->m_Image = m_Image;
	(dynamic_cast<CImageProcessingDlg*>(m_Main))->DrawImage();
}

void CMenuPage1::OnBnClickedButtonSave()
{
	LPCTSTR Filter = L"Точечный рисунок (*.bmp)|*.bmp|";
	CFileDialog SaveImageDialog(false, L".bmp", NULL, 0, Filter, 0, 0, 0);
	if(SaveImageDialog.DoModal() == IDOK)
	{
		m_Image.Save(SaveImageDialog.GetFileName());
	}
}
