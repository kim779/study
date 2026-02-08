// MxFlatEdit.cpp : implementation file
//

#include "stdafx.h"
#include "MxFlatEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define			IDC_MXEDIT				1000
/////////////////////////////////////////////////////////////////////////////
CMxEdit::CMxEdit()
{

}

CMxEdit::~CMxEdit()
{
}

BEGIN_MESSAGE_MAP(CMxEdit, CEdit)
	//{{AFX_MSG_MAP(CMxEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
void CMxEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_RETURN ) 
	{
		CWnd* parent = GetOwner(); 
		if(parent)
			parent->SendMessage(WM_USER, VK_RETURN, 0); 
	}
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
/////////////////////////////////////////////////////////////////////////////
// CMxFlatEdit

CMxFlatEdit::CMxFlatEdit()
{
	m_linecolor = 0x8A8A8A;
	m_pEidt = NULL;
	m_pFont = NULL;
}

CMxFlatEdit::~CMxFlatEdit()
{
}


BEGIN_MESSAGE_MAP(CMxFlatEdit, CWnd)
	//{{AFX_MSG_MAP(CMxFlatEdit)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMxFlatEdit message handlers
BOOL CMxFlatEdit::Create(UINT nID, CRect rect, CWnd* parent, CFont* pFont, UINT nEditStyle, COLORREF linecolor)
{
	m_pFont = pFont;
	m_linecolor = linecolor;
	m_nEditStyle = nEditStyle;
	return CWnd::Create(NULL, NULL, WS_VISIBLE|WS_CHILD, rect, parent, nID, NULL); 
}

void CMxFlatEdit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CPen pen(PS_SOLID, 1, m_linecolor );
	CPen* pOldPen = dc.SelectObject(&pen); 
	dc.Rectangle(m_ClientRect); 
	
	// TODO: Add your message handler code here
	dc.SelectObject(pOldPen); 
	DeleteObject(pOldPen);	
	
	// Do not call CWnd::OnPaint() for painting messages
}

int CMxFlatEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	GetClientRect(&m_ClientRect);
	CRect rc = m_ClientRect;
	rc.DeflateRect(1,1,1,1); 
	m_pEidt = std::make_unique<CMxEdit>();
	DWORD style = WS_CHILD|WS_VISIBLE|EM_SETRECTNP;
	if(m_nEditStyle) style |= m_nEditStyle;
	m_pEidt->Create(style, rc, this, IDC_MXEDIT); 

	int ih = 10;
	if(m_pFont) 
	{
		m_pEidt->SetFont(m_pFont);
		LOGFONT lpf;
		m_pFont->GetLogFont(&lpf); 
		if( lpf.lfHeight < 0) ih = lpf.lfHeight * (-1);
		else ih = lpf.lfHeight / 10;
	}

	const int igap = (rc.Height() - ih) / 2; 
	rc.DeflateRect(0,igap,0,igap); 
	m_pEidt->SetRectNP (rc);

	if(style & ES_PASSWORD)
		m_pEidt->SetPasswordChar('*'); 
	m_pEidt->ShowWindow(SW_SHOW); 

	return 0;
}


void CMxFlatEdit::SetLimitText(int ilen)
{
	if(m_pEidt)
		m_pEidt->LimitText(ilen); 
}

CString CMxFlatEdit::GetText()
{
	CString sText;
	if(m_pEidt)
	{
		m_pEidt->GetWindowText(sText);
	}
	return sText;
}

void CMxFlatEdit::SetText(CString sText)
{
	if(m_pEidt)
	{
		m_pEidt->SetWindowText(sText);
	}
}

void CMxFlatEdit::SetSelectFocus()
{
	if(m_pEidt)
	{
		m_pEidt->SetSel (0, -1);
		m_pEidt->SetFocus(); 
	}
}

LRESULT CMxFlatEdit::OnUser(WPARAM wParam, LPARAM lParam)
{
	if(wParam == VK_RETURN)
	{
		CWnd* parent = GetOwner(); 
		if(parent)
			parent->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), VK_RETURN), (LPARAM)GetSafeHwnd()); 
	}

	return 0;
}

void CMxFlatEdit::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_pEidt)
	{
		m_pEidt->DestroyWindow();
		m_pEidt = NULL;
	}
}
