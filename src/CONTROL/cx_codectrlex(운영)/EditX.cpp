// EditX.cpp : implementation file
//

#include "stdafx.h"
#include "EditX.h"
#include "InfoPopup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditX

CEditX::CEditX()
{
}

CEditX::~CEditX()
{
}


BEGIN_MESSAGE_MAP(CEditX, CEdit)
	//{{AFX_MSG_MAP(CEditX)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditX message handlers
BOOL CEditX::Create(CWnd* parent, UINT nID)
{
	m_brush.DeleteObject();
//	m_brush.CreateSolidBrush(GetSysColor(COLOR_INFOBK));

	const	DWORD	dwStyle = WS_CHILD|ES_LEFT|ES_AUTOVSCROLL|ES_WANTRETURN|ES_MULTILINE;
	if (CEdit::Create(dwStyle, CRect(0,0,0,0), parent, nID))
	{
		m_brush.CreateSolidBrush(((CInfoPopup*)GetParent())->GetIndexColor(97));
		return TRUE;
	}
	return FALSE;
}

HBRUSH CEditX::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->SetBkColor(((CInfoPopup*)GetParent())->GetIndexColor(97));
	
	return m_brush;
	return NULL;
}
