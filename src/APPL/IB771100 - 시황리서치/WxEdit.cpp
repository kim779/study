// WxEdit.cpp : implementation file
//

#include "stdafx.h"
#include "IB771100.h"
#include "WxEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWxEdit

CWxEdit::CWxEdit()
{
	m_pParent = NULL;
}

CWxEdit::~CWxEdit()
{
}


BEGIN_MESSAGE_MAP(CWxEdit, CEdit)
	//{{AFX_MSG_MAP(CWxEdit)
	ON_WM_CHAR()
	//ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWxEdit message handlers

void CWxEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_RETURN ) 
	{
		if(m_pParent)
		{
			m_pParent->SendMessage(WM_WXMSG, WXEDIT_ENTER, 0); 
		}
	}
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CWxEdit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	/*CRect rect;
	GetClientRect(&rect);
	dc.FillSolidRect(rect, 0xFFFFFF); */
	// Do not call CEdit::OnPaint() for painting messages
	//CEdit::OnPaint();
}

