// MapWnd.cpp : implementation file
//
// 路塭濘六六六六
#include "stdafx.h"
#include "IB771100.h"
#include "MapWnd.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapWnd

CMapWnd::CMapWnd(CWnd* pWnd)
{
	m_pViewWnd = pWnd;
	m_mapKey = -1;
}

CMapWnd::~CMapWnd()
{
}


BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMapWnd message handlers

CString CMapWnd::Variant(int comm, CString data)
{
	CString retvalue = _T("");
	char* dta = (char*)m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue.Format("%s", dta);
	
	return retvalue;
}

void CMapWnd::loadMap(CString mapname, CString option)
{
	mapname += option;
	m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(loadMAP, m_mapKey), (long)(char *)mapname.operator LPCTSTR());
}

void CMapWnd::closeMap()
{
	m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(detachMAP, m_mapKey), 0);	
	m_mapKey = -1;
}

void CMapWnd::ChangeMap(CString mapname, CString option)
{
	if (m_mapKey >= 0)
		closeMap();
	m_mapKey = m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(attachMAP, vtypeNRM), (long)this);
	loadMap(mapname, option);	
}

void CMapWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	closeMap();
}

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMemDC	mdc(&dc);
	CRect	rect;
	GetClientRect(rect);
	mdc.FillSolidRect(rect, RGB(200, 200, 200));
}

int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
			
	return 0;
}


