// MapWnd.cpp : implementation file
//
// 路塭濘六六六六
#include "stdafx.h"
#include "IB410100.h"
#include "SearchPage.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchPage

CSearchPage::CSearchPage(CWnd* pView, CWnd* pParent)
{
	//{{AFX_DATA_INIT(CStgPage)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_pView = pView;

	m_mapKey = -1;
}

CSearchPage::~CSearchPage()
{
}


BEGIN_MESSAGE_MAP(CSearchPage, CWnd)
	//{{AFX_MSG_MAP(CSearchPage)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSearchPage message handlers

CString CSearchPage::Variant(int comm, CString data)
{
	CString retvalue = _T("");
//	char* dta = (char*)m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

// 	if ((long)dta > 1)
// 		retvalue.Format("%s", dta);
	
	return retvalue;
}

void CSearchPage::loadMap(CString mapname, CString option)
{
	mapname += option;
	m_pView->SendMessage(WM_USER, MAKEWPARAM(loadMAP, m_mapKey), (long)(char *)mapname.operator LPCTSTR());
}

void CSearchPage::closeMap()
{
	m_pView->SendMessage(WM_USER, MAKEWPARAM(detachMAP, m_mapKey), 0);	
	m_mapKey = -1;
}

void CSearchPage::ChangeMap(CString mapname, CString option)
{
	if (m_mapKey >= 0)
		closeMap();
	m_mapKey = m_pView->SendMessage(WM_USER, MAKEWPARAM(attachMAP, vtypeNRM), (long)this);
	loadMap(mapname, option);	
}

#define		P_TAB				0x09
void CSearchPage::SendTrigger(CString code)
{
	CString send;
	const int major = m_pView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC));
	send.Format("%s%c%s", "SetTrigger", P_TAB, code); 

// 	OutputDebugString("[KSJ]SendTrigger " + send);

	m_pView->SendMessage(WM_USER, MAKEWPARAM(procDLL, MAKEWORD(major, m_mapKey) ), (LPARAM)(LPCTSTR)send);
}

void CSearchPage::ProcDll(CString strMethod, CString strData)
{
	CString send;
	const int major = m_pView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC));
	send.Format("%s%c%s", strMethod, P_TAB, strData); 
	
	// 	OutputDebugString("[KSJ]SendTrigger " + send);
	
	m_pView->SendMessage(WM_USER, MAKEWPARAM(procDLL, MAKEWORD(major, m_mapKey) ), (LPARAM)(LPCTSTR)send);
}

void CSearchPage::OnDestroy() 
{
	CWnd::OnDestroy();
	
	closeMap();
}

void CSearchPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMemDC	mdc(&dc);
	CRect	rect;
	GetClientRect(rect);
	mdc.FillSolidRect(rect, RGB(200, 200, 200));
}

int CSearchPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
			
	return 0;
}
