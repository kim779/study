// LabelWnd.cpp : implementation file
//

#include "stdafx.h"
#include "LabelWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLabelWnd

CLabelWnd::CLabelWnd()
{
	m_width   = 100;
	m_height  = 20;
	m_strLabel     = _T("");
	m_type    = LEFT;
	m_nFormat = DT_CENTER|DT_VCENTER;
	m_OldRect = CRect(-1,-1,-1,-1);
	m_bVeticalLabel = FALSE;
	m_nVertCharWidth = 1;
	m_nVertTextSize = -1;
}

CLabelWnd::~CLabelWnd()
{
	font.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
// CLabelWnd message handlers

void CLabelWnd::Paint(CDC* pDC, CDC* pBackDC) 
{
	CRect rect = m_rect;

	CBrush brush;
	brush.CreateSolidBrush(m_colBackground);
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&brush);
	CPen pen(PS_SOLID,1, RGB(0,0,0));
	CPen* pOldPen = (CPen*)pDC->SelectObject(&pen);
	
	pDC->Rectangle(&rect);

	pDC->SetTextColor(m_colText);
	pDC->SetBkColor(m_colBackground);

	rect.InflateRect(-2,-2,-3,-3);
	
	CFont* pOldFont = (CFont*)pDC->SelectObject(&font);
	pDC->DrawText( m_strLabel, rect, m_nFormat);
	pDC->SelectObject(pOldFont);

	if( m_OldRect != CRect(-1,-1,-1,-1))
	{
		CRect rectInter;
		BOOL res = rectInter.IntersectRect(m_OldRect,m_rect);

		if( !res)
		{
			RestoreRect(m_OldRect,pDC,pBackDC);
		}
		else if(pBackDC == NULL)
		{
			RestoreRect(m_rect,pDC,pBackDC);
		}
		else
		{			
			// Total Rect
			CRect rectOuter = m_rect;
			rectOuter |= m_OldRect;

			CRect rectPart[8];
			
			rectPart[0] = CRect( rectOuter.left, rectOuter.top, rectInter.left, rectInter.top);
			rectPart[1] = CRect( rectInter.left, rectOuter.top, rectInter.right, rectInter.top);
			rectPart[2] = CRect( rectInter.right, rectOuter.top, rectOuter.right, rectInter.top);
			rectPart[3] = CRect( rectOuter.left, rectInter.top, rectInter.left, rectInter.bottom);
			rectPart[4] = CRect( rectInter.right, rectInter.top, rectOuter.right, rectInter.bottom);
			rectPart[5] = CRect( rectOuter.left, rectInter.bottom, rectInter.left, rectOuter.bottom);
			rectPart[6] = CRect( rectInter.left, rectInter.bottom, rectInter.right, rectOuter.bottom);
			rectPart[7] = CRect( rectInter.right, rectInter.bottom, rectOuter.right, rectOuter.bottom);

			// check 8 sides : use by PtInRect
			for(int i=0; i<8; i++)
			{
				if( !rectPart[i].IsRectEmpty() && m_OldRect.PtInRect(rectPart[i].CenterPoint()))
				{
					RestoreRect(rectPart[i],pDC,pBackDC);
				}
			}
		}
	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	
	m_OldRect = m_rect;
}

void CLabelWnd::SetLabelFont(CFont *pFont)
{
	LOGFONT lp;
	pFont->GetLogFont(&lp);
	if( font.GetSafeHandle() )
	{
		font.DeleteObject();
	}
	font.CreateFontIndirect(&lp);	
}

void CLabelWnd::SetLabel(TCHAR *pChLabel, UINT nFormat, CDC* pDC,BOOL bVerticalLabel)
{
	m_strLabel.Format(_T("%s"),pChLabel);
	m_nFormat = nFormat;

	CFont* pOldFont = (CFont*)pDC->SelectObject(&font);
	m_sizeLabel = pDC->GetTextExtent( m_strLabel );
	pDC->SelectObject(pOldFont);
	m_bVeticalLabel = bVerticalLabel;
	if(bVerticalLabel == TRUE)
	{ 
		CString strLabel = m_strLabel;
		strLabel.Replace(_T("\n"),_T(""));
		int nTextLength = strLabel.GetLength();
		/*if(m_nVertTextSize != -1 && strLabel.GetLength() > m_nVertTextSize)
		{
			m_width  = m_sizeLabel.cy*m_nVertCharWidth + 4;
		}
		else
		{
			m_width  = m_sizeLabel.cy + 4;
		}*/		
		m_width = m_sizeLabel.cy*(int)(nTextLength/m_nVertTextSize) + 4;
		if(nTextLength%m_nVertTextSize != 0)
		{
			m_width += m_sizeLabel.cy;
		}

		m_height = m_sizeLabel.cx + 6; 
	}
	else
	{
		m_width  = m_sizeLabel.cx + 6;
		m_height = m_sizeLabel.cy + 4;
	}		
}

void CLabelWnd::MoveLabelWindow(int x, int y, int width, int height)
{
	CRect rect;
	switch(m_type)
	{
	case LEFT:		
		rect.left	= x;
		rect.top	= y - m_height/2;
		rect.right	= x + width;
		rect.bottom = rect.top + m_height;
		break;
	
	case RIGHT:
		rect.left	= x;
		rect.top	= y - m_height / 2;
		rect.right	= x + width;
		rect.bottom = rect.top + m_height;
		break;

	case BOTTOM:
		rect.left	= x;// - m_width / 2;
		rect.top	= y;
		rect.right	= rect.left + m_width;
		rect.bottom = rect.top + m_height;		
	}

	m_rect = rect;
}

void CLabelWnd::SetType(int type)
{
	m_type = type;
}

void CLabelWnd::SetColor(COLORREF colText, COLORREF colBackground)
{
	m_colText = colText;
	m_colBackground = colBackground;
}

BOOL CLabelWnd::CreateLabel(int type, COLORREF colText, COLORREF colBk)
{
	SetType(type);	
	SetColor(colText, colBk);	
	return TRUE;
}

void CLabelWnd::ShowWnd(BOOL bShow, CDC *pDC, CDC *pBackDC)
{
	if(bShow)// && pBackDC != NULL)
	{
		Paint(pDC, pBackDC);
	}
	else
	{
		RestoreRect(m_OldRect, pDC, pBackDC);
		m_OldRect = CRect(-1,-1,-1,-1);
	}
}

void CLabelWnd::RestoreRect(CRect rect, CDC* pDC, CDC *pBackDC)
{
	pDC->BitBlt( rect.left, rect.top, rect.Width(), rect.Height(), pBackDC, rect.left, rect.top, SRCCOPY);
}
