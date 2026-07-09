// axStatic.cpp : implementation file
//

#include "stdafx.h"
#include "axStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const	int	repeatMAX  = 3;
const	int	idEVENT    = 1;
const	int	delayEVENT = 500;

/////////////////////////////////////////////////////////////////////////////
// CAxStatic

CAxStatic::CAxStatic()
{
	m_textColor  = PALETTERGB(  0,   0,   0);
	m_redColor   = PALETTERGB(255,   0,   0);
	m_blueColor  = PALETTERGB(  0,   0, 255);
	m_backColor  = PALETTERGB(255, 255, 255);
	m_flashColor = PALETTERGB(202, 208, 203);

	m_index = 0;
	m_count = 0;
	m_text  = m_key = _T("");
	m_color = '=';
}

CAxStatic::~CAxStatic()
{
}


BEGIN_MESSAGE_MAP(CAxStatic, CStatic)
	//{{AFX_MSG_MAP(CAxStatic)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxStatic message handlers

int CAxStatic::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_font.DeleteObject();
	m_font.CreateFont(12, 0, 0, 0, FW_MEDIUM, FALSE, false, false,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			PROOF_QUALITY, DEFAULT_PITCH|FF_DONTCARE, "±¼¸²Ã¼");
	return 0;
}

void CAxStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect	rect, cRc; 
	CBrush*	brush = new CBrush(m_count%2 ? m_flashColor : m_backColor);
	CBrush* oldBrush = dc.SelectObject(brush); 
	GetClientRect(cRc);

	cRc.DeflateRect(1, 1);
	dc.PatBlt(cRc.left, cRc.top, cRc.Width(), cRc.Height(), PATCOPY);
	CFont* pfont = dc.SelectObject(&m_font);
	dc.SetBkMode(TRANSPARENT);
	cRc.left += 2;
	switch (m_color)
	{
	case '+':
		dc.SetTextColor(m_redColor);	break;
	case '-':
		dc.SetTextColor(m_blueColor);	break;
	case '=':
	default:
		dc.SetTextColor(m_textColor);	break;
	}
	dc.DrawText(m_text, cRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cRc.left -= 2;
	dc.DrawEdge(cRc, EDGE_ETCHED, BF_RECT);
	dc.SelectObject(pfont);
	dc.SelectObject(oldBrush); 

	brush->DeleteObject();
	delete brush;
}

void CAxStatic::OnTimer(UINT_PTR nIDEvent) 
{
	if (nIDEvent == idEVENT)
	{
		m_count++;
		KillTimer(idEVENT);

		if (m_count <= 10)
			SetTimer(idEVENT, delayEVENT, NULL);
		else
			m_count = 0;

		Invalidate();
	}

	CStatic::OnTimer(nIDEvent);
}

void CAxStatic::UpdateItem(CString str, char color)
{
	if (str.IsEmpty())
		return;

	int	pos = str.Find('\n');
	if (pos >= 0)
	{
		m_text = str.Left(pos);
		m_key  = str.Mid(pos+1);
	}
	else
	{
		m_text = str;
		m_key  = _T("");
	}

	Invalidate(); m_color = color;

	SetTimer(idEVENT, delayEVENT, NULL);
}

void CAxStatic::ClearItem()
{
	m_text = _T("");
	m_key  = _T("");

	KillTimer(idEVENT);
	m_count = 0; Invalidate();
}