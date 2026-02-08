// Sheet.cpp : implementation file
//

#include "stdafx.h"
#include "IB202201.h"
#include "Sheet.h"
#include "MapWnd.h"

#include "page1.h"
#include "page2.h"
#include "page4.h"
#include <Windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSheet

IMPLEMENT_DYNAMIC(CSheet, CPropertySheet)

CSheet::CSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_parent = pParentWnd;
	m_only   = false;

	m_page1 = std::make_unique<CPage1>(m_parent);
	m_page2 = std::make_unique<CPage2>(m_parent);
	m_page4 = std::make_unique<CPage4>(m_parent);

	m_page1->SetOther(m_page4.get());
	m_page4->SetOther(m_page1.get());

	AddPage(m_page1.get());
	AddPage(m_page2.get()); 
	AddPage(m_page4.get());
}

CSheet::CSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_parent = pParentWnd;
	m_only   = false;

	m_page1 = std::make_unique<CPage1>(m_parent);
	m_page2 = std::make_unique<CPage2>(m_parent);
	m_page4 = std::make_unique<CPage4>(m_parent);

	m_page1->SetOther(m_page4.get());
	m_page4->SetOther(m_page1.get());

	AddPage(m_page1.get());
	AddPage(m_page2.get());
	AddPage(m_page4.get());
}

CSheet::~CSheet()
{
	//if (m_page1) delete m_page1;
	//if (m_page2) delete m_page2;
	//if (m_page4) delete m_page4;
}

BEGIN_MESSAGE_MAP(CSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CSheet)
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_TOPARENT, OnParent)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSheet message handlers

BOOL CSheet::OnInitDialog() 
{
	const BOOL bResult = CPropertySheet::OnInitDialog();
	m_page1->m_search.SetFocus();

	////2013.01.07 KSJ win8에서는 사이즈를 366으로 해준다.
	//OSVERSIONINFOEX osvi;
	//osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFOA);
	//GetVersionExA((LPOSVERSIONINFO)&osvi);
	//
	//if(osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2)
	//{
		MoveWindow(0, 0, 840, 366, TRUE);
	//}
	//KSJ
	return bResult;
}

BOOL CSheet::OnEraseBkgnd(CDC* pDC) 
{
	CRect clientRC; GetClientRect(&clientRC);
	const COLORREF clrBack = GetSysColor(COLOR_BTNFACE);
	
	pDC->FillSolidRect(clientRC, clrBack);
	return TRUE;
	
	return CPropertySheet::OnEraseBkgnd(pDC);
}

void CSheet::RemovePageX()
{
	if (m_page2) { RemovePage(m_page2.get()); m_page2.reset(); }
	if (m_page4) { RemovePage(m_page4.get()); m_page4.reset(); }
}

bool CSheet::GetIsEdit()
{
	bool result = false;

	result = m_page1->GetIsEdit();
	
	if(m_only == false)
	{
		if(result == false)
		{
			result = m_page4->GetIsEdit();
		}
	}


	return result; 
}

// int CSheet::saveGroup()
// {
// 	const int result = m_page1->SaveGroup();

// 	return result;
// }

void CSheet::OKaction()
{
	_ball = false;
	if (m_page1 && m_page1->m_activePage)
	{
		if(GetIsEdit())
		{
			m_page1->OKaction();
		}
	}
	if (m_page2 && m_page2->m_activePage)
	{
		m_page2->OKaction();
	}
	if (m_page4 && m_page4->m_activePage)
	{
		_ball = m_page4->_bAll;
		m_page4->OKaction();
	}
}

void	CSheet::OKSubtabaction(int igubn)
{
	_ball = false;
	if (m_page2 && m_page2->m_activePage)
	{
		if(igubn == 1)
			m_page2->OKaction(1);
		else
			m_page2->OKaction();
	}
	if (m_page4 && m_page4->m_activePage)
	{
		_ball = m_page4->_bAll;
		m_page4->OKaction();
	}
}

void CSheet::OKactionOne()
{
	_ball = false;
	auto page = GetActivePage();

	if (page == m_page4.get() && m_page4->m_activePage)
	{
		_ball = m_page4->_bAll;
		m_page4->endSendAction();
	}
	if (page == m_page2.get() && m_page2->m_activePage)
		m_page2->endSendAction();
	if (page == m_page1.get() && m_page1->m_activePage)
		m_page1->endSendAction();
}


void CSheet::OkKillaction()
{
	_ball = false;
	if (m_page2 && m_page2->m_activePage)
	{
		m_page2->OnKillActive();
	}
	if (m_page4 && m_page4->m_activePage)
	{
		_ball = m_page4->_bAll;
		m_page4->OnKillActive();
	}
}

void CSheet::selectCombobox()
{	
	if (m_page1->m_activePage)
		m_page1->selectComboBox();
}

void CSheet::CCaction()
{
	if (m_page1 && m_page1->m_activePage) m_page1->CCaction();
	if (m_page2 && m_page2->m_activePage) m_page2->CCaction();
	if (m_page4 && m_page4->m_activePage) m_page4->CCaction();
}

int CSheet::GetUploadData(int gno, CString& name, char* datB)
{
	auto page = GetActivePage();

	if (m_page1.get() == page)
		return m_page1->GetUploadData(gno, name, datB);
	else if (m_page4.get() == page)
		return m_page4->GetUploadData(gno, name, datB);

	return 0;
}

// CString	CSheet::GetUploadData2(int gno, CString& name, int& count)
// {
// 	if (m_page1->m_activePage)
// 		//		return m_page1->GetUploadData(gno, name, datB);
// 		return m_page1->GetTempData(gno, name, count);
// 	return "";
// }

int CSheet::SetDnloadData(int gno, CString name, int count, char* datB)
{
	if (m_page1->m_activePage)
		return m_page1->SetDnloadData(gno, name, count, datB);
	return count;
}

//void	CSheet::recoveryFile()
//{
////	if (m_page1->m_activePage)
////		m_page1->recoveryFile();
//}

//void	CSheet::deleteBackupFile()
//{
////	if (m_page1->m_activePage)
////		m_page1->deleteBackupFile();
//}

void CSheet::initCombo()
{
	if (m_page1->m_activePage)
		m_page1->resettingGroup();
}

//void CSheet::reInitInterest()
//{
//	//if (m_page1->m_activePage) m_page1->reInitInterest();
//}

void CSheet::enableButton(BOOL enable)
{
	if (m_page1->m_activePage) m_page1->enableButton(enable);
}


//CString CSheet::GetGroupORder()
//{
//	if (m_page1->m_activePage)
//		return m_page1->GetGroupORder();
//	return _T("");
//}

CString CSheet::GetGroupORder2(CString& data)
{
	if (m_page1->m_activePage)
		return m_page1->GetGroupORder2(data);
	return _T("");
}

int	CSheet::getManageCount()
{
	if (m_page1->m_activePage)
		return m_page1->getManageCount();
	return 0;
}

bool CSheet::checkChangeSequence()
{
	if (m_page1->m_activePage)
		return m_page1->checkChangeSequence();
	return 0;
}

CString	CSheet::getManageGroupdata(int row, int col)
{
	if (m_page1->m_activePage)
		return m_page1->getManageGroupdata(row, col);
	return "";
}
void CSheet::setManageGroup(char* datB)
{
	if (m_page1->m_activePage)
		m_page1->setManageGroup(datB);
}


void CSheet::updateCode(CString code)
{
	if (m_page1->m_activePage)
	{
		m_page1->m_bUpdate = TRUE;
		m_page1->m_search.SetWindowText(code);
		m_page1->ExactGroupData();
		m_page1->SendMessage(WM_EDITX, wpSEARCH1, 0);

		m_page1->m_search.SetSel(0,-1);
	}// // 
}

void CSheet::updateGroup(CString codes)
{
	if (m_page1->m_activePage)
	{
		m_page1->m_appendGI = codes;
		m_page1->PostMessage(WM_TOPAGE1, 0, 0);
	}
}

void CSheet::BuildPropPageArray()
{
	[[gsl::suppress(26401)]]
	[[gsl::suppress(26409)]]
	delete[] (PROPSHEETPAGE*)m_psh.ppsp;
	m_psh.ppsp = nullptr;

	[[gsl::suppress(26400)]]
	[[gsl::suppress(26409)]]
	LPCPROPSHEETPAGE ppsp = new PROPSHEETPAGE[m_pages.GetSize()]();
	m_psh.ppsp = (LPCPROPSHEETPAGE)ppsp;

	const BOOL bWizard = (m_psh.dwFlags & (PSH_WIZARD | PSH_WIZARD97));

	for(int i = 0; i < m_pages.GetSize(); i++)
	{
		CPropertyPage* pPage = GetPage(i);
		memcpy((void*)&ppsp[i],&pPage->m_psp,sizeof(pPage->m_psp));
	}

	m_psh.nPages = m_pages.GetSize();
}

void CSheet::updateGroupNCode(int GroupNumber, CString code)
{
	if (m_page1->m_activePage)
	{
		m_page1->m_bUpdate = TRUE;
		m_page1->m_search.SetWindowText(code);
		m_page1->m_selectGroup = GroupNumber;
		m_page1->m_appendGI = code;
		m_page1->PostMessage(WM_TOSELECTGROUPNCODE, 0, 0);
	}
}

//2012.02.10 KSJ
void CSheet::appendITEMS(int GroupNumber, CString codes)
{
	if (m_page1->m_activePage)
	{
		m_page1->m_bUpdate = TRUE;
		
		if (GroupNumber == 0)	// 신규그룹으로만든다.
		{
			m_page1->AppendNewgroup();
			m_page1->insertList2x(codes);
		}
		else
		{
			m_page1->m_selectGroup = GroupNumber -1;
			m_page1->ExactGroupData();
			m_page1->AppendItem(codes);
		}
	}
}

void CSheet::updateParentGroup(CString codes)
{
	if (m_page1->m_activePage)
	{	
		m_page1->m_appendGI = codes;
		m_page1->PostMessage(WM_TOPAGE2, 0, 0);
	}
}

void CSheet::selectGroup(int GroupNumber)
{
	if (m_page1->m_activePage)
	{
		if (GroupNumber > 0)
		{
			m_page1->m_selectGroup = GroupNumber;
			m_page1->PostMessage(WM_TOSELECTPAGE, 0, 0);
		}
	}
}

LONG CSheet::OnParent(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CSheet::EnablePage1Btn(BOOL bEnable)
{
	m_page1->EnableButton(bEnable);
}

void CSheet::sendELWIssueDN(char* datB, int datL)
{
	m_page1->parsingELWIssueDN(datB,datL);
}

void CSheet::sendELWAssetDN(char* datB, int datL)
{
	m_page1->parsingELWAssetDN(datB, datL);
}

void CSheet::sendRecommandDN(char* datB, int datL)
{
	m_page1->parsingRecommandDN(datB, datL);
}


void CSheet::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	m_page2->OnKillActive();
	
	CPropertySheet::OnClose();
}

BOOL CSheet::IsMenuStatusClosed()
{
	if(m_page2->IsMenuStatusClosed())
		return TRUE;
	else
		return FALSE;
}


void CSheet::reloadConfig()
{
	m_page1->loadConfig();
}

BOOL CSheet::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	return CPropertySheet::OnCommand(wParam, lParam);
}

int CSheet::getGroupCount()
{
	return m_page1->GetGroupCount();
}

#include "ioformat.h"

void CSheet::receiveOub(int key, CString rData)
{
	auto page = GetActivePage();

	if (m_page1.get() == page)
		m_page1->receiveOub(key, rData);	
	else if (m_page4.get() == page)
		m_page4->receiveOub(key, rData);
}

