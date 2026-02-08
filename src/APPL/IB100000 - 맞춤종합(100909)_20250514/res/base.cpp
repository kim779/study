// base.cpp : implementation file
//

#include "stdafx.h"
#include "IB100000.h"
#include "base.h"

#include "../../h/axisfire.h"
#include "../../h/axisvar.h"

#define	BACKGROUND	RGB(255, 255, 255)
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBase

CBase::CBase()
{
}

CBase::~CBase()
{
}


BEGIN_MESSAGE_MAP(CBase, CWnd)
	//{{AFX_MSG_MAP(CBase)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUSER)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBase message handlers

void CBase::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	 cRc;

	GetClientRect(cRc);
	dc.SetBkColor(BACKGROUND);
	dc.ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, &cRc, "", 0, NULL);
}

BOOL CBase::create_Map(CWnd* parent, CRect wRc)
{
	DWORD	dwStyle = WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN;
	 if (!CWnd::Create(NULL, NULL, dwStyle, wRc, parent, 1))
		 return FALSE;
	m_parent = parent;
	variant(titleCC, "맞춤 종합화면");
	m_home = variant(homeCC);
	return	TRUE;
}

CString CBase::variant(int comm, CString data)
{
	char* dta = (char*)m_parent->SendMessage(WM_USER, 
			MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)	return dta;
	else			return "";
}



LONG CBase::OnUSER(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:		break;
	case DLL_ALERT:		break;
	case DLL_TRIGGER:	break;
	case DLL_DOMINO:	break;
	case DLL_NOTICE:	break;
	default:		break;
	}
	return 0;
}
