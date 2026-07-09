// MyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "axiscode.h"
#include "MyEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyEdit

CMyEdit::CMyEdit()
{
	m_pTool = new CTool;
}

CMyEdit::~CMyEdit()
{
	SAFE_DELETE(m_pTool);
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	//{{AFX_MSG_MAP(CMyEdit)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEdit message handlers

#include "myfont.h"

void CMyEdit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	CString szText;
		GetClientRect(&rc);

		dc.FillSolidRect(rc, COR_WHITE);

		GetWindowText(szText);
		
		CFont font;
		CFont *poldfont;
		font.CreatePointFont(9*10, "±¼¸²");
		poldfont = dc.SelectObject(&font);
		dc.DrawText(szText, rc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		
		dc.SelectObject(poldfont);
		font.DeleteObject();
		

	// Do not call CEdit::OnPaint() for painting messages
}
