// DllCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "IB771100.h"
#include "DllCtrl.h"
#include "MapWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDllCtrl

CDllCtrl::CDllCtrl(class CMapWnd *pParent)
	:m_pMapWnd(pParent)
{
	m_pFont = m_pMapWnd->GetFont();

	m_nkey = 0;

	m_pScreen = NULL;

	m_bCreated = FALSE;

	m_nHeight= 180;
	m_strSendMsg = _T("");
}

CDllCtrl::~CDllCtrl()
{
	CloseMap();
	SAFE_DELETE(m_pScreen);
}


BEGIN_MESSAGE_MAP(CDllCtrl, CWnd)
	//{{AFX_MSG_MAP(CDllCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDllCtrl message handlers

int CDllCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}


int CDllCtrl::GetMapHeight()
{
	return m_nHeight;
}

void CDllCtrl::LoadMap(CString strMapN, CString strName)
{
	CRect cRc;

	m_strName = strName;

	int type = m_pMapWnd->GetWizardWnd()->SendMessage(WM_USER, MAKEWPARAM(typeDLL, 0), 
		(long) strMapN.operator LPCTSTR());
	int size = m_pMapWnd->GetWizardWnd()->SendMessage(WM_USER, MAKEWPARAM(sizeMAP, 0), 
		(long)(char *) strMapN.operator LPCTSTR());
	
	GetClientRect(cRc);

	m_pScreen = new CWnd;
	m_pScreen->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, cRc, this, (int)m_pScreen);
	m_nkey = m_pMapWnd->GetWizardWnd()->SendMessage(WM_USER, MAKEWPARAM(attachMAP, type), LPARAM(m_pScreen));

	if (m_nkey != 0)
	{
		int nLoad = m_pMapWnd->GetWizardWnd()->SendMessage(WM_USER, MAKEWPARAM(loadMAP, m_nkey), 
			(long) strMapN.operator LPCTSTR());
		if (nLoad)
			m_pScreen->Invalidate();
	}
}

void CDllCtrl::CloseMap()
{
	if(IsWindow(m_pMapWnd->m_hWnd) && m_nkey != 0)
		m_pMapWnd->GetWizardWnd()->SendMessage(WM_USER, MAKEWPARAM(detachMAP, m_nkey), 0);
}

void CDllCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect cRc, dllRc;
	GetClientRect(cRc);

	CString strParam;
	if(cx == 0 || cy == 0)
		return;
	strParam.Format("%d\t%d", cx, cy);
	if (m_pScreen)
	{
		dllRc = cRc;
		dllRc.right = cRc.right;
		m_pScreen->MoveWindow(dllRc);
		m_strSendMsg = strParam;
		SetTimer(100, 1, NULL);
//		SetScreenParam(PARAM_SIZE, strParam);
	}
}

void CDllCtrl::SetScreenParam(CString strParam, CString strData)
{
	CString strBuffer = "";
	
	strBuffer.Format("%s\t%s\t%s\t", MAP_PROC, strParam, strData);
	m_pMapWnd->Procedure(strBuffer, m_nkey, 0);
}

void CDllCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect cRc;
	GetClientRect(cRc);

//	dc.FillSolidRect(cRc, m_pMapWnd->GetIndexColor(clBack));
}



void CDllCtrl::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 100)
	{
		KillTimer(nIDEvent);
		if(!m_strSendMsg.IsEmpty())
			SetScreenParam(PARAM_SIZE, m_strSendMsg);
		m_strSendMsg = _T("");
	}
	
	CWnd::OnTimer(nIDEvent);
}
