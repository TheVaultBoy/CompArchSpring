// TabMenu.cpp: файл реализации
//

#include "stdafx.h"
#include "Image Processing.h"
#include "TabMenu.h"
#include "MenuPage1.h"
#include "MenuPage2.h"
#include "MenuPage3.h"
#include "MenuPage4.h"
#include "Image ProcessingDlg.h"


// CTabMenu

IMPLEMENT_DYNAMIC(CTabMenu, CTabCtrl)

CTabMenu::CTabMenu()
{
	m_DialogsID[0] = IDD_MENU_PAGE1;
	m_DialogsID[1] = IDD_MENU_PAGE2;
	m_DialogsID[2] = IDD_MENU_PAGE3;
	m_DialogsID[3] = IDD_MENU_PAGE4;
	m_Dialogs[0] = new CMenuPage1();
	m_Dialogs[1] = new CMenuPage2();
	m_Dialogs[2] = new CMenuPage3();
	m_Dialogs[3] = new CMenuPage4();
	m_Pages = 4;
}

CTabMenu::~CTabMenu()
{
}

void CTabMenu::InitDialogs()
{
	m_Dialogs[0]->Create(m_DialogsID[0], GetParent());
	(dynamic_cast<CMenuPage1*>(m_Dialogs[0]))->m_Main = this->m_Main;
	m_Dialogs[1]->Create(m_DialogsID[1], GetParent());
	(dynamic_cast<CMenuPage2*>(m_Dialogs[1]))->m_Main = this->m_Main;
	m_Dialogs[2]->Create(m_DialogsID[2], GetParent());
	(dynamic_cast<CMenuPage3*>(m_Dialogs[2]))->m_Main = this->m_Main;
	m_Dialogs[3]->Create(m_DialogsID[3], GetParent());
	(dynamic_cast<CMenuPage4*>(m_Dialogs[3]))->m_Main = this->m_Main;
}

void CTabMenu::ActivateTabDialogs()
{
	int nSel = GetCurSel();
	if(m_Dialogs[nSel]->m_hWnd)
		m_Dialogs[nSel]->ShowWindow(SW_HIDE);

	CRect l_rectClient;
	CRect l_rectWnd;

	GetClientRect(l_rectClient);
	AdjustRect(false,l_rectClient);
	GetWindowRect(l_rectWnd);
	GetParent()->ScreenToClient(l_rectWnd);
	l_rectClient.OffsetRect(l_rectWnd.left,l_rectWnd.top);
	for (int nCount=0; nCount<m_Pages; nCount++)
		m_Dialogs[nCount]->SetWindowPos(&wndTop, l_rectClient.left, l_rectClient.top, l_rectClient.Width(), l_rectClient.Height(), SWP_HIDEWINDOW);
	m_Dialogs[nSel]->SetWindowPos(&wndTop, l_rectClient.left, l_rectClient.top, l_rectClient.Width(), l_rectClient.Height(), SWP_SHOWWINDOW);
	m_Dialogs[nSel]->ShowWindow(SW_SHOW);
}

BEGIN_MESSAGE_MAP(CTabMenu, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CTabMenu::OnTcnSelchange)
END_MESSAGE_MAP()



// обработчики сообщений CTabMenu



void CTabMenu::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	(dynamic_cast<CImageProcessingDlg*>(m_Main))->m_Image = 
	(dynamic_cast<CMenuPage4*>(m_Dialogs[3]))->m_Image = 
	(dynamic_cast<CMenuPage3*>(m_Dialogs[2]))->m_Image = 
	(dynamic_cast<CMenuPage2*>(m_Dialogs[1]))->m_Image = 
	(dynamic_cast<CMenuPage1*>(m_Dialogs[0]))->m_Image;
	ActivateTabDialogs();
	*pResult = 0;
}
