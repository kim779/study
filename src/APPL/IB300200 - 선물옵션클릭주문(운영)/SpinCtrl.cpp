// SpinCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SpinCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpinCtrl

CSpinCtrl::CSpinCtrl()
{
	m_pfont		= NULL;
	m_bCreateFont	= FALSE;
}

CSpinCtrl::~CSpinCtrl()
{
	if (	m_pfont
		&& m_bCreateFont)
	{
		m_pfont->DeleteObject();
		delete m_pfont;
		m_pfont = NULL;
	}
}


BEGIN_MESSAGE_MAP(CSpinCtrl, CSpinButtonCtrl)
	//{{AFX_MSG_MAP(CSpinCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpinCtrl message handlers

BOOL CSpinCtrl::Create(	DWORD dwStyle,
			const RECT& rect,
			CWnd* pParentWnd,
			UINT nID,
			CFont* pfont) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (dwStyle == NULL)
	{
		dwStyle =	WS_CHILDWINDOW
				| WS_VISIBLE
				| UDS_ARROWKEYS
				| UDS_ALIGNRIGHT
				| UDS_SETBUDDYINT;
	}

	m_pfont	= pfont;

	return CWnd::Create(UPDOWN_CLASS, NULL, dwStyle, rect, pParentWnd, nID);
}

int CSpinCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSpinButtonCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	//return 0;
	return Initialize();
}

int CSpinCtrl::Initialize()
{
	if (m_pfont == NULL)
	{
		m_pfont = (CFont*) new CFont;
		m_pfont->CreatePointFont(90, "±¼¸²");
		m_bCreateFont	= TRUE;
	}

	SetFont(m_pfont);

	return 0;
}

void CSpinCtrl::SetAllAccel(int iAccel)
{
	//UDACCEL* pAccel = NULL;
	UDACCEL udaccel[10]{};
	const int iTotalAccelIndex = GetAccel(10, &udaccel[0]);
	for (int ii = 0; ii < iTotalAccelIndex; ii++)
	{
		udaccel[ii].nInc = iAccel;
	}
	SetAccel(iTotalAccelIndex, &udaccel[0]);
}

void CSpinCtrl::SetFontFromParent(CFont* pfont)
{
	if (	m_pfont
		&& m_bCreateFont)
	{
		m_pfont->DeleteObject();
		delete m_pfont;
	}

	m_pfont = pfont;
	m_bCreateFont = FALSE;
}