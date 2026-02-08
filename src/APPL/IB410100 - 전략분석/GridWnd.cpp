// GridWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB410100.h"
#include "GridWnd.h"
#include "memdc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridWnd

CGridWnd::CGridWnd(CWnd* pView, CWnd* parent)
{
	m_pApp = (CIB410100App *)AfxGetApp();
	m_pParent = parent;
	m_pView   = pView;
	m_nRow	  = 12;
	m_fcolWidth = 50;
}

CGridWnd::~CGridWnd()
{
}


BEGIN_MESSAGE_MAP(CGridWnd, CWnd)
	//{{AFX_MSG_MAP(CGridWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGridWnd message handlers

int CGridWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pFont = m_pApp->GetFont(m_pView, 9);
	m_pPen  = m_pApp->GetPen(m_pView, m_gBgColor);

	CRect	rc;
	GetClientRect(&rc);
	SetColor();
	InitGrid(rc);
	
	return 0;
}

void CGridWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CMemDC	memDC(&dc);

	CRect	rc;
	GetClientRect(&rc);
	memDC.FillSolidRect(rc, m_bkColor);

	DrawGrid(&memDC);
}

void CGridWnd::SetColor()
{
	m_cFgColor = m_pApp->GetIndexColor(m_pView, 69);//63);
	m_cBgColor = m_pApp->GetIndexColor(m_pView, 68);
	m_hFgColor = m_pApp->GetIndexColor(m_pView, 76);
	m_hBgColor = m_pApp->GetIndexColor(m_pView, 74);
	m_gBgColor = m_pApp->GetIndexColor(m_pView, 29);
	m_bkColor  = m_pApp->GetIndexColor(m_pView, 7);
}

void CGridWnd::InitGrid(CRect gridRC)
{
	int	top{}, bottom{}, left{}, right{};
	for (int ii = 0 ; ii < m_nRow ; ii++)
	{
		for (int jj = 0 ; jj < 2 ; jj++)
		{
			top = ii * gridRC.Height() / m_nRow;
			bottom = (ii + 1) * gridRC.Height() / m_nRow;

			if (jj == 0)
			{
				left = gridRC.left - 2;
				right = gridRC.left + m_fcolWidth;
			}
			else
			{
				left = gridRC.left + m_fcolWidth; 
				right = gridRC.right - 1;
			}

			m_infoGrid[ii][jj].rc = CRect(left, top, right, bottom);
		}
	}

	InitFormat();

	SetGridItemText(0, 0, "지수");
	SetGridItemText(0, 1, "만기손익");
}

void CGridWnd::SetGridItemText(int row, int col, CString text)
{
	m_infoGrid[row][col].text = text;
	InvalidateRect(m_infoGrid[row][col].rc);
}

void CGridWnd::InitFormat()
{
	COLORREF	fcolor{}, bcolor{};
	UINT		format=0;
	const int		nMiddle = m_nRow / 2;
	for (int ii = 0 ; ii < m_nRow ; ii++)
	{
		for (int jj = 0 ; jj < 2 ; jj++)
		{
			switch (ii)
			{
			case 0:
				fcolor = m_hFgColor; bcolor = m_hBgColor;
				break;
			default:
				if (ii == nMiddle)
				{
					fcolor = m_cFgColor; bcolor = RGB(230, 232, 238);
				}
				else
				{
					fcolor = m_cFgColor; bcolor = m_cBgColor;
				}
				break;
			}

			if (ii == 0)
				format = DT_CENTER;
			else
			{
				if (jj == 0)
					format = DT_CENTER;
				else
					format = DT_RIGHT;
			}

			m_infoGrid[ii][jj].fcolor = fcolor;
			m_infoGrid[ii][jj].bcolor = bcolor;
			m_infoGrid[ii][jj].format = format;
		}
	}
}

void CGridWnd::DrawGrid(CDC *pDC)
{
	CFont	*sFont = (CFont*)pDC->SelectObject(m_pFont);
	CPen	*sPen = (CPen*)pDC->SelectObject(m_pPen);
	const COLORREF	sColor = pDC->SetTextColor(RGB(0,0,0));
	const int		sMode = pDC->SetBkMode(TRANSPARENT);

	CRect		rc;
	COLORREF	bcolor{}, fcolor{};
	CString		text;
	UINT		format=0;
	for (int ii = 0 ; ii < m_nRow ; ii++)
	{
		for (int jj = 0 ; jj < 2 ; jj++)
		{
			rc = m_infoGrid[ii][jj].rc;
			bcolor = m_infoGrid[ii][jj].bcolor;
			fcolor = m_infoGrid[ii][jj].fcolor;
			text = m_infoGrid[ii][jj].text;
			format = m_infoGrid[ii][jj].format;

			if (ii == 0)
				DrawBox(pDC, RGB(192, 192, 192), m_hBgColor, rc, BOX_ALL, TRUE);
			else
				DrawBox(pDC, RGB(192, 192, 192), bcolor, rc, BOX_ALL, TRUE);
			SetText(pDC, rc, text, fcolor, format);
		}
	}

	pDC->MoveTo(m_infoGrid[0][0].rc.left, m_infoGrid[0][0].rc.top);
	pDC->LineTo(m_infoGrid[0][1].rc.right, m_infoGrid[0][1].rc.top);

	pDC->MoveTo(m_infoGrid[1][0].rc.left, m_infoGrid[1][0].rc.top);
	pDC->LineTo(m_infoGrid[1][1].rc.right, m_infoGrid[1][1].rc.top);

	pDC->MoveTo(m_infoGrid[11][0].rc.left, m_infoGrid[11][0].rc.bottom);
	pDC->LineTo(m_infoGrid[11][1].rc.right, m_infoGrid[11][1].rc.bottom);

	pDC->SetBkMode(sMode);
	pDC->SetTextColor(sColor);
	pDC->SelectObject(sPen);
	pDC->SelectObject(sFont);
}


void CGridWnd::SetText(CDC* pDC, CRect rect, CString strInput, COLORREF txColor, UINT fmt)
{
	pDC->SetTextColor(txColor);
	const UINT deffmt = DT_SINGLELINE | DT_VCENTER | fmt;
	pDC->DrawText(strInput, rect, deffmt);
}

void CGridWnd::DrawBox(CDC* pDC, COLORREF nOutLine, COLORREF nBkColor, CRect rect, UINT nLine, BOOL bFill)
{
	if (rect == CRect(-1, -1, -1, -1))
		GetClientRect(rect);		

	if (bFill)
	{
		pDC->FillSolidRect(rect, nBkColor);
	}

	CPen *oldpen;
	oldpen = pDC->SelectObject(m_pApp->GetPen(m_pView, nOutLine));

	if (nLine & BOX_LEFT)
	{
		pDC->MoveTo(rect.TopLeft());
		pDC->LineTo(rect.left, rect.bottom);
	}

	if (nLine & BOX_TOP)
	{
		pDC->MoveTo(rect.TopLeft());
		pDC->LineTo(rect.right, rect.top);
	}

	if (nLine & BOX_BOTTOM)
	{
		pDC->MoveTo(rect.right, rect.bottom);
		pDC->LineTo(rect.left, rect.bottom);
	}

	if (nLine & BOX_RIGHT)
	{
		pDC->MoveTo(rect.right, rect.bottom);
		pDC->LineTo(rect.right, rect.top);
	}

	pDC->SelectObject(oldpen);
}
