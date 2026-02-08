// ScreenWnd.cpp : implementation file
//
// 路塭濘六六六六
#include "stdafx.h"
#include "../../h/axisfire.h"
#include "ScreenWnd.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapWnd

CScreenWnd::CScreenWnd(CWnd* pWnd)
{
	m_pViewWnd = pWnd;
	m_mapKey = -1;
}

CScreenWnd::~CScreenWnd()
{
}


BEGIN_MESSAGE_MAP(CScreenWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMapWnd message handlers

CString CScreenWnd::Variant(int comm, CString data)
{
	CString retvalue = _T("");
	char* dta = (char*)m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue.Format("%s", dta);
	
	return retvalue;
}

void CScreenWnd::loadMap(CString mapname, CString option)
{
	mapname += option;
	m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(loadMAP, m_mapKey), (long)(char *)mapname.operator LPCTSTR());
}

void CScreenWnd::closeMap()
{
	m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(detachMAP, m_mapKey), 0);	
	m_mapKey = -1;
}

void CScreenWnd::ChangeMap(CString mapname, CString option)
{
	if (m_mapKey >= 0)
		closeMap();

	int type = m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(typeDLL, 0), 
		(long) mapname.operator LPCTSTR());

	m_mapKey = m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(attachMAP, type), (long)this);
	loadMap(mapname, option);	
}

#define		P_TAB				0x09
void CScreenWnd::SendTrigger(CString code)
{
	CString send;
	int major = m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC));
	send.Format("%s%c%s", "SetTrigger", P_TAB, code); 
	m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(procDLL, MAKEWORD(major, m_mapKey) ), (LPARAM)(LPCTSTR)send);
}

void CScreenWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	closeMap();
}

void CScreenWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMemDC	mdc(&dc);
	CRect	rect;
	GetClientRect(rect);
	mdc.FillSolidRect(rect, RGB(200, 200, 200));
}

int CScreenWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
			
	return 0;
}
