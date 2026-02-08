/******************************************************************************
 * NAME	: AxStatic.cpp							 
 * DESC	: CStatic을 상속받은 static class로 table 기능이 추가되어있음.
 * REV. : VER	DATE	DESCRIPTION
 * 	  ----	------	-------------------------------------------------------
 *	  1.00	2004.08.20	Initial version.
 *****************************************************************************/
// AxStatic.cpp : implementation file
//

#include "stdafx.h"
#include "AxStatic.h"
#include "../../h/memDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	ROWS	m_headArr.GetSize()

CAxStatic::CAxStatic()
{
	m_bkClr  = RGB(255, 255, 255);
	m_frClr  = RGB(0, 0, 0);
	m_hbkClr = RGB(192, 192, 192);
	m_hfrClr = RGB(0, 0, 0);
	m_bxClr  = m_bkClr;

	m_upRGB  = RGB(255, 0, 0);
	m_dnRGB  = RGB(0, 0, 255);
	m_blkRGB = RGB(210, 210, 210);
	m_nHeadWidth = 0;

	m_headArr.RemoveAll();
	m_dataArr.RemoveAll();

	m_bInitialize =false;
	m_text.Empty();
}

CAxStatic::~CAxStatic()
{
}


BEGIN_MESSAGE_MAP(CAxStatic, CStatic)
	//{{AFX_MSG_MAP(CAxStatic)
	ON_MESSAGE(WM_GETTEXT, HandleGetText)
	ON_MESSAGE(WM_SETTEXT, HandleSetText)
	ON_MESSAGE(WM_GETTEXTLENGTH, OnGetTextLength)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


LRESULT CAxStatic::HandleSetText(WPARAM wParam, LPARAM lParam)
{
	if (lParam)
	{
		m_text = (char*)lParam;
		if (m_text.Find("\t", 0) >= 0)	// is table data?
		{
			CString sText = m_text;
			while(!sText.IsEmpty())
			{
				m_headArr.Add(Parser(sText, "\t"));
				m_dataArr.Add(Parser(sText, "\t"));
				m_attrArr.Add(0);
			}
		}
		Invalidate();
	}
	return 0;
}

long CAxStatic::HandleGetText(WPARAM wParam, LPARAM lParam)
{
	lstrcpyn((LPTSTR)lParam, (LPCTSTR)m_text, wParam);
	if ((int)wParam > m_text.GetLength())
		wParam = m_text.GetLength();
	return wParam;
}

long CAxStatic::OnGetTextLength(WPARAM, LPARAM)
{
	return m_text.GetLength();
}

void CAxStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC memdc;
	CRect rc;
	CFont	*pFont{}, *pOldFont{};
	DWORD	dwStyle = DT_SINGLELINE|DT_VCENTER;
	CBitmap		*pOldBitmap, bitmap;

	GetClientRect(&rc);

	memdc.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
	pOldBitmap = (CBitmap*)memdc.SelectObject(&bitmap);

	if (GetStyle() & SS_CENTER)
		dwStyle |= DT_CENTER;
	else if (GetStyle() & SS_LEFT)
		dwStyle |= DT_LEFT;
	else if (GetStyle() & SS_RIGHT)
		dwStyle |= DT_RIGHT;

	pFont = GetFont();
	pOldFont = memdc.SelectObject(pFont);

	memdc.SetBkMode(TRANSPARENT);
	
	if (ROWS)
	{
		memdc.FillSolidRect(rc, m_bxClr);
		if (m_bVertical)
		{
			for (int nRow = 0; nRow < ROWS; nRow++)
			{
				CRect headRc = rc, dataRc;
				DWORD dataStyle = DT_RIGHT;
				headRc.top    = rc.top + nRow * rc.Height() / ROWS;
				headRc.bottom = rc.top + (nRow + 1) * rc.Height() / ROWS - 1;
				
				dataRc = headRc;
				headRc.right  = rc.left + m_nHeadWidth;
				dataRc.left   = headRc.right + 1;

				memdc.FillSolidRect(headRc, m_hbkClr);
				memdc.FillSolidRect(dataRc, (GetAttr(nRow) & DA_BLINK)?m_blkRGB:m_bkClr);

				//memdc.FillSolidRect(CRect(rc.left, headRc.bottom, rc.right, headRc.bottom + 1), m_bxClr);

				memdc.SetTextColor(m_hfrClr);
				memdc.DrawText(GetHead(nRow), headRc, dwStyle);

				if (GetAttr(nRow) & DA_LEFT)
					dataStyle = DT_LEFT;
				else if (GetAttr(nRow) & DA_CENTER)
					dataStyle = DT_CENTER;

				memdc.SetTextColor(m_frClr);
				if (GetAttr(nRow) & DA_CORGB)
				{
					CString sData = GetData(nRow);
					if (!sData.IsEmpty())
					{
						if (sData.GetAt(0) == '+' || !sData.CompareNoCase("매수"))
							memdc.SetTextColor(m_upRGB);
						else if (sData.GetAt(0) == '-' || !sData.CompareNoCase("매도"))
							memdc.SetTextColor(m_dnRGB);
					}
				}
				
				dataRc.DeflateRect(0, 0, 2, 0);
				if (GetAttr(nRow) & DA_CONTRAST)
				{
					CRect rcCont = dataRc;
					rcCont.SetRect(dataRc.left, dataRc.top + 2, 10 + dataRc.left, dataRc.top + 16);
					CString sData = GetData(nRow, true);
					if (!sData.IsEmpty())
					{
						switch (sData.GetAt(0))
						{
						case '1':	// 상한
							memdc.SetTextColor(m_upRGB);
							drawContrast(&memdc, rcCont, '1', m_upRGB);
							break;
						case '2':	// 상승
							memdc.SetTextColor(m_upRGB);
							drawContrast(&memdc, rcCont, '2', m_upRGB);
							break;
						case '3':	// 보합
							//drawContrast(&memdc, rcCont, '3', m_frClr);
							break;
						case '4':	// 하한
							memdc.SetTextColor(m_dnRGB);
							drawContrast(&memdc, rcCont, '4', m_dnRGB);
							break;
						case '5':	// 하락
							memdc.SetTextColor(m_dnRGB);
							drawContrast(&memdc, rcCont, '3', m_dnRGB);
							break;
						}
					}
					/*
					if (memdc.GetTextColor() == m_upRGB)
						drawContrast(&memdc, rcCont, '2', m_upRGB);
					else if (memdc.GetTextColor() == m_dnRGB)
						drawContrast(&memdc, rcCont, '3', m_dnRGB);
						*/
				}

				CFont bFont;
				if (GetAttr(nRow) & DA_BOLD)
				{
					LOGFONT sLogFont;
					GetFont()->GetLogFont(&sLogFont);
					sLogFont.lfWeight = FW_BOLD;
					bFont.CreateFontIndirect(&sLogFont);
					pOldFont = memdc.SelectObject(&bFont);
				}

				memdc.DrawText(GetData(nRow, true), dataRc, DT_SINGLELINE|DT_VCENTER|dataStyle);
				
				if (GetAttr(nRow) & DA_BOLD)
				{
					pFont = GetFont();
					pOldFont = memdc.SelectObject(pFont);
				}
			}
		}
		else
		{
			for (int nRow = 0; nRow < ROWS; nRow++)
			{
				CRect headRc = rc, dataRc;
				DWORD dataStyle = DT_RIGHT;
				
				headRc.left  = rc.left + nRow * rc.Width() / ROWS;
				headRc.right = rc.left + (nRow + 1) * rc.Width() / ROWS - 1;
				
				dataRc = headRc;
				if (GetAttr(nRow) & DA_WRAP)
				{
					headRc.bottom = headRc.top + headRc.Height() / 2;
					dataRc.top    = headRc.bottom + 1;
					headRc.right  = dataRc.right;
					dataRc.left   = headRc.left;
				}
				else
				{
					headRc.right  = headRc.left + m_nHeadWidth;
					dataRc.left   = headRc.right + 1;
				}

				memdc.FillSolidRect(headRc, m_hbkClr);
				memdc.FillSolidRect(dataRc, (GetAttr(nRow) & DA_BLINK)?m_blkRGB:m_bkClr);
				memdc.FillSolidRect(CRect(rc.right, headRc.top, rc.right, headRc.bottom), m_bxClr);

				memdc.SetTextColor(m_hfrClr);
				memdc.DrawText(GetHead(nRow), headRc, dwStyle);

				if (GetAttr(nRow) & DA_LEFT)
					dataStyle = DT_LEFT;
				else if (GetAttr(nRow) & DA_CENTER)
					dataStyle = DT_CENTER;

				memdc.SetTextColor(m_frClr);
				if (GetAttr(nRow) & DA_CORGB)
				{
					CString sData = GetData(nRow);
					if (!sData.IsEmpty())
					{
						if (sData.GetAt(0) == '+' || !sData.CompareNoCase("매수"))
							memdc.SetTextColor(m_upRGB);
						else if (sData.GetAt(0) == '-' || !sData.CompareNoCase("매도"))
							memdc.SetTextColor(m_dnRGB);
					}
				}


				dataRc.DeflateRect(0, 0, 2, 0);
				if (GetAttr(nRow) & DA_CONTRAST)
				{
					CRect rcCont = dataRc;
					rcCont.SetRect(dataRc.left, dataRc.top + 2, 10 + dataRc.left, dataRc.top + 16);
					CString sData = GetData(nRow, false);
					if (!sData.IsEmpty())
					{
						switch (sData.GetAt(0))
						{
						case '1':	// 상한
							memdc.SetTextColor(m_upRGB);
							drawContrast(&memdc, rcCont, '1', m_upRGB);
							break;
						case '2':	// 상승
							memdc.SetTextColor(m_upRGB);
							drawContrast(&memdc, rcCont, '2', m_upRGB);
							break;
						case '3':	// 보합
							//drawContrast(&memdc, rcCont, '3', m_frClr);
							break;
						case '4':	// 하한
							memdc.SetTextColor(m_dnRGB);
							drawContrast(&memdc, rcCont, '4', m_dnRGB);
							break;
						case '5':	// 하락
							memdc.SetTextColor(m_dnRGB);
							drawContrast(&memdc, rcCont, '3', m_dnRGB);
							break;
						}
					}
					/*
					if (memdc.GetTextColor() == m_upRGB)
						drawContrast(&memdc, rcCont, '2', m_upRGB);
					else if (memdc.GetTextColor() == m_dnRGB)
						drawContrast(&memdc, rcCont, '3', m_dnRGB);
						*/
				}

				CFont bFont;
				if (GetAttr(nRow) & DA_BOLD)
				{
					LOGFONT sLogFont;
					GetFont()->GetLogFont(&sLogFont);
					sLogFont.lfWeight = FW_BOLD;
					bFont.CreateFontIndirect(&sLogFont);
					pOldFont = memdc.SelectObject(&bFont);
				}
				
				memdc.DrawText(GetData(nRow, true), dataRc, DT_SINGLELINE|DT_VCENTER|dataStyle);
				
				if (GetAttr(nRow) & DA_BOLD)
				{
					pFont = GetFont();
					pOldFont = memdc.SelectObject(pFont);
				}
			}
		}
	}
	else
	{
		memdc.FillSolidRect(rc, m_bkClr);
		
		memdc.SetTextColor(m_frClr);
		CRect dataRc = rc;
		dataRc.DeflateRect(0, 1, 3, 1);
		memdc.DrawText(m_text, dataRc, dwStyle);
	}

	memdc.Draw3dRect(rc, m_bxClr, m_bxClr);
	memdc.SelectObject(pOldFont);

	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memdc, 0, 0, SRCCOPY);
	memdc.SelectObject(pOldBitmap);
	bitmap.DeleteObject();
	memdc.DeleteDC();
}

void CAxStatic::setColor(COLORREF bkClr, COLORREF frClr, COLORREF bxClr)
{
	m_bkClr = bkClr;
	m_frClr = frClr;
	m_bxClr = bxClr;
}

void CAxStatic::setHeadColor(COLORREF bkClr, COLORREF frClr)
{
	m_hbkClr = bkClr;
	m_hfrClr = frClr;
}

CString CAxStatic::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
			return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

void CAxStatic::SetData(int nRow, CString sDat)
{
	if (ROWS <= nRow)
		return;
	m_dataArr.SetAt(nRow, sDat);
}

void CAxStatic::SetHead(int nRow, CString sDat)
{
	if (ROWS <= nRow)
		return;
	m_headArr.SetAt(nRow, sDat);
}

CString CAxStatic::GetData(int nRow, bool bModified)
{
	if (ROWS <= nRow)
		return "";

	CString sDat = m_dataArr.GetAt(nRow);

	if (bModified)
	{
		if ((GetAttr(nRow) & DA_SUPPRESSPM) && !sDat.IsEmpty())
		{
			if (sDat.GetAt(0) == '+' || sDat.GetAt(0) == '-')
				sDat = sDat.Mid(1);
		}
		if ((GetAttr(nRow) & DA_SUPPRESSP) && !sDat.IsEmpty())
		{
			if (sDat.GetAt(0) == '+')
				sDat = sDat.Mid(1);
		}
		if ((GetAttr(nRow) & DA_CONTRAST) && !sDat.IsEmpty())
		{
			switch (sDat.GetAt(0))
			{
			case '1':	case '2':	case '3':	case '4':	case '5':
				sDat = sDat.Mid(1);
				break;
			}
			
		}
		if (GetAttr(nRow) & DA_ONLYPOSITIVE)
		{
			if (atof(sDat) < 0)
				sDat = "0";
		}
		if (GetAttr(nRow) & DA_ZERO)
		{
			if (!sDat.IsEmpty())
			{
				if (atof(sDat) == 0)
					sDat.Empty();
			//	sDat.TrimLeft('0');
			}
		}
		if (GetAttr(nRow) & DA_COMMA)
		{
			sDat = SetComma(sDat);
		}

	}
	return sDat;
}

CString CAxStatic::GetHead(int nRow)
{
	if (ROWS <= nRow)
		return "";
	return m_headArr.GetAt(nRow);
}

void CAxStatic::SetAttr(int nRow, DWORD Attr)
{
	if (ROWS <= nRow)
		return;
	m_attrArr.SetAt(nRow, Attr);
}

DWORD CAxStatic::GetAttr(int nRow)
{
	if (ROWS <= nRow)
		return 0;
	return m_attrArr.GetAt(nRow);
}

CString CAxStatic::SetComma(CString src)
{
	CString temp, floatStr;
	int jj = -1;

	src.TrimLeft();
	if (src.Find('.') >= 0)
	{
		floatStr = src.Mid(src.Find('.'));
		src = src.Left(src.Find('.'));
	}
	else
		floatStr.Empty();
	
	temp = src;
	src.Empty();
	
	for (int ii = temp.GetLength() - 1; ii >= 0; ii--)
	{
		jj++;
		if (jj == 3)
		{
			if (temp.GetAt(0) != '-' && temp.GetAt(0) != '+')
				src = ',' + src;
			else if (ii != 0)
				src = ',' + src;
			
			jj = 0;
		}
		src = temp.GetAt(ii) + src;
	}
	src += floatStr;
	
	return src;
}

void CAxStatic::OnTimer(UINT nIDEvent) 
{
	KillTimer(nIDEvent);
	SetAttr(nIDEvent, GetAttr(nIDEvent) & ~(DA_BLINK));
	InvalidateRect(getRect(nIDEvent));
	CStatic::OnTimer(nIDEvent);
}

CRect CAxStatic::getRect(int nRow)
{
	CRect rc;
	GetClientRect(&rc);
	if (ROWS)
	{
		if (m_bVertical)
		{
			CRect headRc = rc, dataRc;
			headRc.top    = rc.top + nRow * rc.Height() / ROWS;
			headRc.bottom = rc.top + (nRow + 1) * rc.Height() / ROWS - 1;
			
			dataRc = headRc;
			headRc.right  = rc.left + m_nHeadWidth;
			dataRc.left   = headRc.right + 1;
			return dataRc;
		}
		else
		{
			CRect headRc = rc, dataRc;
			const DWORD dataStyle = DT_RIGHT;
			headRc.left  = rc.left + nRow * rc.Width() / ROWS;
			dataRc.right = rc.left + (nRow + 1) * rc.Width() / ROWS - 1;
			
			dataRc = headRc;
			if (GetAttr(nRow) & DA_WRAP)
			{
				headRc.bottom = headRc.top + headRc.Height() / 2;
				dataRc.top    = headRc.bottom + 1;
				headRc.right  = dataRc.right;
				dataRc.left   = headRc.left;
			}
			else
			{
				headRc.right  = headRc.left + m_nHeadWidth;
				dataRc.left   = headRc.right + 1;
			}
			
			return dataRc;
		}
	}
	return CRect(0, 0, 0, 0);
}

void CAxStatic::BeginBlink(int nRow)
{
	if (ROWS <= nRow)
		return;
	SetAttr(nRow, GetAttr(nRow) | DA_BLINK);
	InvalidateRect(getRect(nRow));
	SetTimer(nRow, 500, nullptr);
}


LRESULT CAxStatic::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (!m_bInitialize)
	{
		CString sText;
		const int _cchUsual = 32;
		
		if (CStatic::DefWindowProc(WM_GETTEXT, (WPARAM)_cchUsual,
			(LPARAM)sText.GetBufferSetLength(_cchUsual)) >= _cchUsual - 1)
		{
			// Text was too long: allocate a bigger buffer.
			
			const int nLen = CStatic::DefWindowProc(WM_GETTEXTLENGTH, (WPARAM)0, (LPARAM)0) + 1;
			CStatic::DefWindowProc(WM_GETTEXT, (WPARAM)nLen,
				(LPARAM)sText.GetBufferSetLength(nLen));
		}
		sText.ReleaseBuffer();
		if (m_text.IsEmpty())
			m_text = sText;
		m_bInitialize = true;
	}
	
	return CStatic::DefWindowProc(message, wParam, lParam);
}

void CAxStatic::drawContrast(CDC *pDC, CRect cellRc, char cStyle, COLORREF clr)
{
	CRect	pRc = cellRc;
	float	hh = (float)cellRc.Height();
	CSize	size;
	CPoint	pts[7];
	CPen* pOldPen{}, pen;
	CBrush* pOldBrush{}, brush;
	
	pen.CreatePen(PS_SOLID, 1, clr);
	brush.CreateSolidBrush(clr);

	pOldPen = (CPen *) pDC->SelectObject(&pen);
	pOldBrush = (CBrush *) pDC->SelectObject(&brush);

	pRc.top = (cellRc.top + cellRc.Height() / 2) - (int)(hh / 2);
	pRc.bottom = (int)(pRc.top + hh);
	
	size = pDC->GetOutputTextExtent("A");
	if (size.cx > 10)
		pRc.left = (int)(pRc.right - (size.cx * 0.8));
	else
		pRc.left = pRc.right - size.cx;
	
	hh = (float)(int)(pRc.Height()+1)/2;
	hh = (float)(int)((hh+1)/2) * 2 - 1;
	int	ww = pRc.Width();
	if (ww >= 4)
		ww = (ww / 2 * 2);
	pRc.right = pRc.left + ww;
	const int	rr = ww % 3;
	ww /= 3;
	if (rr >= 2 && pRc.Width() > 3)
		ww++;
	
	if (pRc.left >= cellRc.left && pRc.right <= cellRc.right)
	{
		pDC->SetPolyFillMode(ALTERNATE);
		switch(cStyle)
		{
		case '1':	// arrow up
			pts[1].y = pRc.top + (int)hh;
			pts[2].y = pRc.top + (int)hh;
			pts[5].y = pRc.top + (int)hh;
			pts[6].y = pRc.top + (int)hh;
			pts[0].y = pRc.top;
			pts[3].y = pRc.bottom;
			pts[4].y = pRc.bottom;
			
			pts[0].x = pRc.left + (pRc.Width()/2);
			pts[1].x = pRc.left;
			pts[6].x = pRc.right;
			pts[2].x = pRc.left + ww;
			pts[3].x = pRc.left + ww;
			pts[4].x = pRc.right - ww;
			pts[5].x = pRc.right - ww;
			pDC->Polygon(pts, 7);
			break;
		case '4':	// arrow down
			pts[1].y = pRc.top + (int)hh;
			pts[2].y = pRc.top + (int)hh;
			pts[5].y = pRc.top + (int)hh;
			pts[6].y = pRc.top + (int)hh;
			pts[0].y = pRc.bottom;
			pts[3].y = pRc.top;
			pts[4].y = pRc.top;
			
			pts[0].x = pRc.left + (pRc.Width()/2);
			pts[1].x = pRc.right;
			pts[6].x = pRc.left;
			pts[2].x = pRc.right - ww;
			pts[3].x = pRc.right - ww;
			pts[4].x = pRc.left + ww;
			pts[5].x = pRc.left + ww;
			pDC->Polygon(pts, 7);
			break;
		case '2':	// triangle up
			pRc.top += 2;
			pRc.bottom -= 2;
			if (!(pRc.Height() % 2))
				pRc.top++;
			pts[0].y = pRc.top;
			pts[1].y = pRc.bottom;
			pts[2].y = pRc.bottom;
			
			pts[0].x = pRc.left + (pRc.Width()/2);
			pts[1].x = pRc.left;
			pts[2].x = pRc.right;
			pDC->Polygon(pts, 3);
			break;
		case '3':	// triangle down
			pRc.top += 2;
			pRc.bottom -= 2;
			if (!(pRc.Height() % 2))
				pRc.top++;
			pts[0].y = pRc.bottom;
			pts[1].y = pRc.top;
			pts[2].y = pRc.top;
			
			pts[0].x = pRc.left + (pRc.Width()/2);
			pts[1].x = pRc.right;
			pts[2].x = pRc.left;
			pDC->Polygon(pts, 3);
			break;
		}
	}
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	pen.DeleteObject();
	brush.DeleteObject();
}
