\// PaneList.cpp : implementation file
//

#include "stdafx.h"
#include "PaneList.h"
#include "smdefine.h"
#include "resource.h"
#include "pane.h"

#include "../../axis/axMsg.hxx"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PGAP		2
#define	SCROLL_WIDTH	13

/////////////////////////////////////////////////////////////////////////////
// CPaneList

CPaneList::CPaneList(class CPane *parent, int id, int kind, int count, COLORREF bkColor)
{
	m_parent = parent;

	m_id = id;
	m_kind = kind;
	m_count	= count;
	m_bkColor = bkColor;
}

CPaneList::~CPaneList()
{
}

BEGIN_MESSAGE_MAP(CPaneList, CWnd)
	//{{AFX_MSG_MAP(CPaneList)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPaneList message handlers

void CPaneList::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	int bkMode = dc.SetBkMode(TRANSPARENT);

	CRect cRc;
	GetClientRect(cRc);

	CDC	memDC;
	if (memDC.CreateCompatibleDC(&dc))
	{
		CBitmap	bmp, *pbmp;
		bmp.CreateCompatibleBitmap(&dc, cRc.Width(), cRc.Height());
		pbmp = (CBitmap *) memDC.SelectObject(&bmp);
		
		DrawPaneList(&memDC);
		
		dc.BitBlt(0, 0, m_vbar.IsWindowVisible() ? cRc.Width() - SCROLL_WIDTH : cRc.Width(), cRc.Height(), 
			&memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pbmp);
		memDC.DeleteDC();
		
	}
	else
		DrawPaneList(&dc);
	
	dc.SetBkMode(bkMode);
	
}

void CPaneList::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void CPaneList::DrawUpDown(CDC *pDC)
{
	int ii = 0;
	int xx = 0;
	CRect	rc, cRc, tRc;
	CString	text, keys, value, sym, tmps;
	
	CPen	pen(PS_SOLID, 1, m_parent->GetColor(COLOR_TICK_LINE));
	CPen	*pOldPen = pDC->SelectObject(&pen);
	
	GetClientRect(rc);
	pDC->FillSolidRect(rc, m_bkColor);
	pDC->SelectStockObject(NULL_BRUSH);
	int bkMode = pDC->SetBkMode(TRANSPARENT);
	
	CFont*	font = pDC->SelectObject(m_parent->GetFont());
	int	cx = m_parent->GetHeadWidth();
	
	UINT nFlag = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	
	for (xx = 0; xx < m_count; xx++)
	{
		CRect		dRc[5];
		CString		text;
		COLORREF	color;
		
		CMapStringToString	mapData;
		CString key, sym;
		m_parent->GetPaneData(xx, key, sym, mapData);
		
		color = pDC->SetTextColor(COLOR_PANETEXT);
		pDC->SelectStockObject(NULL_BRUSH);
		
		cRc.SetRect(m_pRc[xx].left, m_pRc[xx].top, m_pRc[xx].left + cx, m_pRc[xx].bottom);
		text.Format("%s", m_parent->GetIndexSName(m_parent->GetArItem(xx)));
		pDC->DrawText(text, cRc, nFlag);
		pDC->MoveTo(cRc.right, cRc.top + 2);
		pDC->LineTo(cRc.right, cRc.bottom - 2);
		
		cRc.left = cRc.right + PGAP;		cRc.right = m_pRc[xx].right - PGAP;				
		
		switch (m_parent->GetAux())
		{
		case AUX_TEXT:
			{
				double width = cRc.Width() / 5.0;
				for (ii = 0; ii < 5; ii++)
				{
					if (ii == 4)
						dRc[ii].SetRect(cRc.left + (int)(width*ii), cRc.top + PGAP, cRc.right, cRc.bottom - PGAP);
					else
						dRc[ii].SetRect(cRc.left + (int)(width*ii), cRc.top + PGAP,
						cRc.left + (int)(width*(ii + 1)) - 1, cRc.bottom - PGAP);
				}
				
				for (ii = 0; ii < 5; ii++)
				{
					pDC->FillSolidRect(dRc[ii], m_parent->GetUpDownColor(ii));
					
					switch (ii)
					{
					case 0:	mapData.Lookup(S_SH, text);
						pDC->SetTextColor(RGB(255, 255, 255));	break;
					case 1:	mapData.Lookup(S_SS, text);
						pDC->SetTextColor(RGB(255, 255, 255));	break;
					case 2:	mapData.Lookup(S_BH, text);
						pDC->SetTextColor(m_parent->GetColor(COLOR_TICK_TEXT));	break;
					case 3:	mapData.Lookup(S_HL, text);
						pDC->SetTextColor(RGB(255, 255, 255));	break;
					case 4:	mapData.Lookup(S_HH, text);
						pDC->SetTextColor(RGB(255, 255, 255));	break;
					}
					pDC->DrawText(text, dRc[ii], nFlag);
				}
				pDC->SetTextColor(color);
			}
			break;
		case AUX_GRAPH:
			{
				if (mapData.GetCount() <= 0)
					break;
				
				int	total = 0, value, w;
				CArray < int, int > ary, aryW;
				color = pDC->SetTextColor(COLOR_PANETEXT);
				
				if (!mapData.Lookup(S_SH, text))
				{
					pDC->SetTextColor(color);
					break;
				}
				
				value = atoi(text);	ary.Add(value);	total += value;
				mapData.Lookup(S_SS, text);	value = atoi(text);	ary.Add(value);	total += value;
				mapData.Lookup(S_BH, text);	value = atoi(text);	ary.Add(value);	total += value;
				mapData.Lookup(S_HL, text);	value = atoi(text);	ary.Add(value);	total += value;
				mapData.Lookup(S_HH, text);	value = atoi(text);	ary.Add(value);	total += value;
				
				int width = cRc.Width() - 4;	// 4 : 1 pixel between each column
				for (ii = (int)ary.GetSize() - 1; ii >= 0; ii--)
				{
					value = ary.GetAt(ii);
					if (value)
					{
						w = (value * width) / total;
						if (!w)	w = 1;	// fit
					}
					else	w = 0;
					aryW.InsertAt(0, w);
				}
				
				total = 0;
				for (ii = 0; ii < ary.GetSize(); ii++)
				{
					value = aryW.GetAt(ii);
					
					if (ii == ary.GetSize() - 1)
					{
						if (value)
							dRc[ii].SetRect(cRc.left + total, cRc.top, cRc.right, cRc.bottom);
						else
						{
							dRc[ii].SetRectEmpty();
							
							for (int jj = 3; jj >= 0; jj--)
							{
								if (!dRc[jj].IsRectEmpty())
								{
									dRc[jj].right = cRc.right;
									break;
								}
							}
						}
					}
					else
					{
						if (value)
						{
							dRc[ii].SetRect(cRc.left + total, cRc.top, cRc.left + total + value, cRc.bottom);
							value++;
						}
						else 
							dRc[ii].SetRectEmpty();
					}
					total += value;
				}
				
				for (ii = 0; ii < 5; ii++)
				{
					dRc[ii].DeflateRect(0, 2);
					pDC->FillSolidRect(dRc[ii], m_parent->GetUpDownColor(ii));
				}
			}
			pDC->SetTextColor(color);

		}
		pDC->Rectangle(m_pRc[xx]);
	}

	pDC->SetBkMode(bkMode);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(font);
}

void CPaneList::DrawIndex(CDC *pDC)
{
	CRect	rc, cRc, tRc;
	CString	text, keys, value, sym, tmps;

	CPen	pen(PS_SOLID, 1, m_parent->GetColor(COLOR_TICK_LINE));
	CPen	*pOldPen = pDC->SelectObject(&pen);

	GetClientRect(rc);
	pDC->FillSolidRect(rc, m_bkColor);
	pDC->SelectStockObject(NULL_BRUSH);

	CFont*	font = pDC->SelectObject(m_parent->GetFont());
	int	cx = m_parent->GetHeadWidth();

	UINT nFlag = DT_CENTER|DT_VCENTER|DT_SINGLELINE;

	for (int ii = 0; ii < m_count; ii++)
	{
		BOOL	bExpect = FALSE;
		CMapStringToString	mapData;
		CString key, sym;
		m_parent->GetPaneData(ii, key, sym, mapData);
		cx = m_parent->GetHeadWidth();
		
		if (mapData.Lookup("600", text))
		{
			if (atoi(text) == 0)
				bExpect = TRUE;
		}
		
		if (bExpect)
		{
			text.Format("%s", m_parent->GetExpectSymbolName(sym));
			if (text.IsEmpty())
				text.Format("%s", m_parent->GetIndexSName(atoi(key)));
		}
		else
			text.Format("%s", m_parent->GetIndexSName(atoi(key)));
		
		tRc.SetRect(m_pRc[ii].left, m_pRc[ii].top, m_pRc[ii].left + cx, m_pRc[ii].bottom);
		pDC->MoveTo(tRc.right, tRc.top + 2);
		pDC->LineTo(tRc.right, tRc.bottom - 2);
		
		pDC->DrawText(text, tRc, nFlag);
		// draw index data
		COLORREF color;
		if (mapData.Lookup(S_CUR, value))	// 현재가
		{
			switch (m_parent->GetSign(value))
			{
			case '+':
				color = pDC->SetTextColor(m_parent->GetColor(COLOR_TICK_UP));
				sym.LoadString(ST_SYMBOL_UP2);
				break;
			case '-':
				color = pDC->SetTextColor(m_parent->GetColor(COLOR_TICK_DOWN));
				sym.LoadString(ST_SYMBOL_DOWN2);
				break;
			default:
				color = pDC->SetTextColor(m_parent->GetColor(COLOR_TICK_TEXT));
				break;
			}
			
			text.Format("%s", m_parent->commaE(value));
			cx = pDC->GetTextExtent("99,999.99").cx;
			tRc.left = tRc.right;
			tRc.right = tRc.left + cx + PGAP;
			
			pDC->DrawText(text, tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
		}
		
		switch (atoi(key))
		{
		case 8:
			if (mapData.Lookup(S_KOSPI, value))	// K200
			{
				tRc.left = tRc.right;
				tRc.right = tRc.left + 20;
				pDC->SetTextColor(RGB(0, 0, 0));
				pDC->DrawText("/", tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
				
				switch (m_parent->GetSign(value))
				{
				case '+':
					color = pDC->SetTextColor(m_parent->GetColor(COLOR_TICK_UP));
					sym.LoadString(ST_SYMBOL_UP2);
					break;
				case '-':
					color = pDC->SetTextColor(m_parent->GetColor(COLOR_TICK_DOWN));
					sym.LoadString(ST_SYMBOL_DOWN2);
					break;
				default:
					color = pDC->SetTextColor(m_parent->GetColor(COLOR_TICK_TEXT));
					break;
				}
				
				text.Format("%s", m_parent->commaE(value));
				cx = pDC->GetTextExtent("9,999.99").cx;
				tRc.left = tRc.right;
				tRc.right = tRc.left + cx;
				
				pDC->DrawText(text, tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
			}
			break;
		default:
			if (mapData.Lookup(S_DB, value))	// 대비
			{
				text.Format("%s", m_parent->getDBSign(value));
				cx = pDC->GetTextExtent("  ▲").cx;
				tRc.left = tRc.right;
				tRc.right = tRc.left + cx;
				
				pDC->DrawText(text, tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
				
				cx = pDC->GetTextExtent(" 999.99").cx;
				tRc.left = tRc.right;
				tRc.right = tRc.left + cx;
				
				pDC->DrawText(m_parent->commaE(value), tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
			}
			break;
		}
		
		switch (atoi(key))
		{
		case 8:case 9:case 10:
			if (mapData.Lookup(BASIS, value))	// BASIS
			{
				switch (m_parent->GetSign(value))
				{
				case '+':	pDC->SetTextColor(m_parent->GetColor(COLOR_TICK_UP));	break;
				case '-':	pDC->SetTextColor(m_parent->GetColor(COLOR_TICK_DOWN));	break;
				default:	pDC->SetTextColor(m_parent->GetColor(COLOR_TICK_TEXT));	break;
				}
				
				text.Format("%sB", m_parent->commaE(value, false));
				cx = pDC->GetTextExtent(" +99.99%").cx;
				tRc.left = tRc.right;
				tRc.right = tRc.left + cx;
				
				pDC->DrawText(text, tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
			}
			break;
		default:
			if (mapData.Lookup(S_RATE, value))	// 등락률
			{
				text.Format("%s%%", value);
				cx = pDC->GetTextExtent(" +99.99%").cx;
				tRc.left = tRc.right;
				tRc.right = tRc.left + cx;
				
				pDC->DrawText(text, tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
			}
			break;
		}
		
		switch (atoi(key))
		{
		case 8:
			if (mapData.Lookup(S_THEORY, value))		// 이론가
			{
				tRc.left = tRc.right;
				tRc.right = tRc.left + 20;
				pDC->SetTextColor(RGB(0, 0, 0));
				pDC->DrawText("/", tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
				
				switch (m_parent->GetSign(value))
				{
				case '+':
					color = pDC->SetTextColor(m_parent->GetColor(COLOR_TICK_UP));
					sym.LoadString(ST_SYMBOL_UP2);
					break;
				case '-':
					color = pDC->SetTextColor(m_parent->GetColor(COLOR_TICK_DOWN));
					sym.LoadString(ST_SYMBOL_DOWN2);
					break;
				default:
					color = pDC->SetTextColor(m_parent->GetColor(COLOR_TICK_TEXT));
					break;
				}
				
				text.Format("%s", m_parent->commaE(value));
				cx = pDC->GetTextExtent("9,999.99").cx;
				tRc.left = tRc.right;
				tRc.right = tRc.left + cx;
				
				pDC->DrawText(text, tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
			}
			break;
		case 11:	case 12:				// 해외지수 시간
		case 17:	case 18:
			if (mapData.Lookup(S_FTIME, value))
			{
				int pos = value.Find(' ');
				if (pos < 0)	break;
				value = value.Mid(++pos);
				if (value.GetLength() > 5)	value = value.Left(5);
				
				text = _T(" ") + value + _T(" (한국시간기준)");
				cx = pDC->GetTextExtent(text).cx;
				tRc.left = tRc.right;
				tRc.right = tRc.left + cx;
				pDC->SetTextColor(m_parent->GetColor(COLOR_TICK_TEXT));
				pDC->DrawText(text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			}
			break;
		default:
			tRc.right += 4;
			if (m_parent->GetAux())
			{
				if (mapData.Lookup(S_NVOL, value))	// 거래량
				{
					text.Format("%s", m_parent->commaE(value));
					cx = pDC->GetTextExtent(text).cx;
					tRc.left = tRc.right;
					tRc.right = tRc.left + cx;
					
					pDC->SetTextColor(m_parent->GetColor(COLOR_TICK_TEXT));
					pDC->DrawText(text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
				}
			}
			else
			{
				for (int ii = 0; ii < m_parent->GetArSign()[atoi(key)].GetSize(); ii++)
				{
					switch (atoi(m_parent->GetArSign()[atoi(key)].GetAt(ii)))
					{
					case 3:
						text = S_NONE;
						pDC->SetTextColor(m_parent->GetColor(COLOR_TICK_TEXT));
						break;
					case 1:
						text = S_SKofDay;
						pDC->SetTextColor(m_parent->GetColor(COLOR_TICK_UP));
						break;
					case 5:
						text = S_SJofDay;
						pDC->SetTextColor(m_parent->GetColor(COLOR_TICK_DOWN));
						break;
					case 2:
						pDC->SetTextColor(m_parent->GetColor(COLOR_TICK_UP));
						text = S_SSofPrev;
						break;
					case 4:
						text = S_HLofPrev;
						pDC->SetTextColor(m_parent->GetColor(COLOR_TICK_DOWN));
						break;
					default:
						continue;
					}
					
					cx = pDC->GetTextExtent(text).cx;
					tRc.left = tRc.right + 1;
					tRc.right = tRc.left + cx;
					
					pDC->DrawText(text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
				}
			}
			break;
		}
		
		pDC->Rectangle(m_pRc[ii]);
		pDC->SetTextColor(color);
	}

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(font);
}

void CPaneList::DrawInters(CDC *pDC)
{
	CRect	rc, cRc, tRc;
	CString	text, keys, value, sym, tmps;

	CPen	pen(PS_SOLID, 1, m_parent->GetColor(COLOR_TICK_LINE));
	CPen	*pOldPen = pDC->SelectObject(&pen);

	GetClientRect(rc);
	pDC->FillSolidRect(rc, m_bkColor);
	pDC->SelectStockObject(NULL_BRUSH);

	CFont*	font = pDC->SelectObject(m_parent->GetFont());
	int	cx = m_parent->GetHeadWidth();

	UINT nFlag = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
//
	int nPos;
	if (m_vbar.IsWindowVisible())
	{
		SCROLLINFO	info;
		m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);
		nPos = info.nPos;
	}
	else	nPos = 0;
	
	for (int ii = 0; ii < m_arInter->GetSize() && ii < MAX_PANELIST; ii++)
	{
		_inter* inter = m_arInter->GetAt(nPos + ii);
		
		cx = m_parent->GetHeadWidth();
		tRc.SetRect(m_pRc[ii].left, m_pRc[ii].top, m_pRc[ii].left + cx, m_pRc[ii].bottom);
		text.Format("%s", inter->Name);
		if (inter->Name.IsEmpty())	break;
		text.Replace("&", "&&");
		text.TrimLeft();	text.TrimRight();
		pDC->DrawText(text, tRc, nFlag);
		
		pDC->MoveTo(tRc.right, tRc.top + 2);
		pDC->LineTo(tRc.right, tRc.bottom - 2);
		
		COLORREF color;
		switch (m_parent->GetSign(inter->curr))
		{
		case '+':
			color = pDC->SetTextColor(m_parent->GetColor(COLOR_TICK_UP));
			sym.LoadString(ST_SYMBOL_UP2);
			break;
		case '-':
			color = pDC->SetTextColor(m_parent->GetColor(COLOR_TICK_DOWN));
			sym.LoadString(ST_SYMBOL_DOWN2);
			break;
		default:
			color = pDC->SetTextColor(m_parent->GetColor(COLOR_TICK_TEXT));
			break;
		}
		
		text = m_parent->commaE(inter->curr);
		cx = pDC->GetTextExtent(" " + text).cx;
		tRc.SetRect(tRc.right + PGAP, tRc.top, tRc.right + PGAP + cx, tRc.bottom);
		pDC->DrawText(" " + text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		
		text.Format("  %s", m_parent->dbE(inter->diff));
		cx = pDC->GetTextExtent(text).cx;
		tRc.left = tRc.right;
		tRc.right = tRc.left + cx;
		
		pDC->DrawText(text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		pDC->SetTextColor(color);
		
		text.Format("  %s", m_parent->commaE(inter->gvol));
		cx = pDC->GetTextExtent(text).cx;
		tRc.left = tRc.right;
		tRc.right = tRc.left + cx;
		
		pDC->DrawText(text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		
		pDC->Rectangle(m_pRc[ii]);
	}
//
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(font);
}

void CPaneList::DrawNews(CDC *pDC)
{
	CRect	rc, cRc, tRc;
	CString	text, keys, value, sym, tmps;

	CPen	pen(PS_SOLID, 1, m_parent->GetColor(COLOR_TICK_LINE));
	CPen	*pOldPen = pDC->SelectObject(&pen);

	GetClientRect(rc);
	pDC->FillSolidRect(rc, m_bkColor);
	pDC->SelectStockObject(NULL_BRUSH);

	CFont*	font = pDC->SelectObject(m_parent->GetFont());
	int	cx = m_parent->GetHeadWidth();

	UINT nFlag = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
//
	int nPos;
	if (m_vbar.IsWindowVisible())
	{
		SCROLLINFO	info;
		m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);
		nPos = info.nPos;
	}
	else	nPos = 0;
	
	CMapStringToString	fms;
	for (int ii = 0; ii < m_arNews->GetSize() && ii < MAX_PANELIST; ii++)
	{
		tRc.SetRect(m_pRc[ii].left, m_pRc[ii].top, m_pRc[ii].left + cx, m_pRc[ii].bottom);
		
		fms.RemoveAll();
		value = m_arNews->GetAt(nPos + ii);	parseDat(value, fms);
		
		fms.Lookup(SNEW_INFO, text);	text.TrimRight();
		pDC->DrawText(m_parent->GetNewsTitle(atoi(text)), tRc, nFlag);
		
		pDC->MoveTo(tRc.right, tRc.top + 2);
		pDC->LineTo(tRc.right, tRc.bottom - 2);
		
		tRc.left = tRc.right + PGAP;	tRc.right = m_pRc[ii].right - PGAP;
		fms.Lookup(SNEW_TITLE, text);	text.TrimRight();
		pDC->DrawText(" " + text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
		
		pDC->Rectangle(m_pRc[ii]);
	}
	fms.RemoveAll();
//
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(font);
}

void CPaneList::DrawConclusion(CDC *pDC)
{
	CRect	rc, cRc, tRc;
	CString	text, keys, value, sym, tmps;

	CPen	pen(PS_SOLID, 1, m_parent->GetColor(COLOR_TICK_LINE));
	CPen	*pOldPen = pDC->SelectObject(&pen);

	GetClientRect(rc);
	pDC->FillSolidRect(rc, m_bkColor);
	pDC->SelectStockObject(NULL_BRUSH);

	CFont*	font = pDC->SelectObject(m_parent->GetFont());
	int	cx = m_parent->GetHeadWidth();

	BOOL	bExpect = FALSE;
	UINT nFlag = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
//

//
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(font);
}

void CPaneList::DrawPaneList(CDC *pDC)
{
	switch (m_kind)
	{
	case TKIND_INDEX:	DrawIndex(pDC);		break;
	case TKIND_NEWS:	DrawNews(pDC);		break;
	case TKIND_UPDOWN:	DrawUpDown(pDC);	break;
	case TKIND_INTER:	DrawInters(pDC);	break;
	case TKIND_CONCLUSION:	DrawConclusion(pDC);	break;
	}
}

int CPaneList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect	rc;
	GetClientRect(rc);

	rc.SetRect(0, 0, rc.right, 18);
	for (int ii = 0; ii < MAX_PANELIST; ii++)
	{
		m_pRc[ii] = rc;	

		rc.OffsetRect(0, 20);
	}
	
	m_vbar.Create(WS_CHILD|SBS_VERT|SBS_RIGHTALIGN,
		CRect(rc.right - SCROLL_WIDTH, rc.top, rc.right - 1, rc.bottom), this, -1);

	return 0;
}

void CPaneList::SetNews(CStringArray* ary)
{
	m_arNews = ary;

	SCROLLINFO	info;
	info.cbSize	= sizeof(SCROLLINFO);
	info.fMask	= SIF_ALL | SIF_DISABLENOSCROLL; 
	info.nMin	= 0;
	info.nMax	= (int)m_arNews->GetSize() - 1;
	info.nPage	= MAX_PANELIST;
	info.nPos	= 0;
	info.nTrackPos	= 0;
	m_vbar.SetScrollInfo(&info);

	if (m_arNews->GetSize() > MAX_PANELIST)
	{
		m_vbar.ShowWindow(SW_SHOW);
		CRect	rc;
		GetClientRect(rc);
		m_vbar.MoveWindow(rc.right - SCROLL_WIDTH, rc.top, SCROLL_WIDTH, rc.Height());
	}
}

void CPaneList::SetInters(CArray <class _inter*, class _inter*> *arInter)
{
	m_arInter = arInter;

	SCROLLINFO	info;
	info.cbSize	= sizeof(SCROLLINFO);
	info.fMask	= SIF_ALL | SIF_DISABLENOSCROLL; 
	info.nMin	= 0;
	info.nMax	= (int)m_arInter->GetSize() - 1;
	info.nPage	= MAX_PANELIST;
	info.nPos	= 0;
	info.nTrackPos	= 0;

	m_vbar.SetScrollInfo(&info);

	if (m_arInter->GetSize() > MAX_PANELIST)
	{
		m_vbar.ShowWindow(SW_SHOW);
		CRect	rc;
		GetClientRect(rc);
		m_vbar.MoveWindow(rc.right - SCROLL_WIDTH, rc.top, SCROLL_WIDTH, rc.Height());
	}
}

void CPaneList::parseDat(CString dat, CMapStringToString& ary)
{
	CString sym, value;
	int	pos;
	while (!dat.IsEmpty())
	{
		pos = dat.Find('\t');
		if (pos == -1)	break;
		sym = dat.Left(pos++);
		dat = dat.Mid(pos);

		pos = dat.Find('\t');
		if (pos == -1)
		{
			value = dat;
			dat.Empty();
		}
		else
		{
			value = dat.Left(pos++);
			dat = dat.Mid(pos);
		}

		ary.SetAt(sym, value);
	}
}

void CPaneList::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	

}

void CPaneList::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);

	m_parent->PostMessage(WM_PANELIST, (WPARAM)0, (LPARAM)this);
}

void CPaneList::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int index = -1;
	for (int ii = 0; ii < MAX_PANELIST; ii++)
	{
		if (m_pRc[ii].PtInRect(point))
		{
			index = ii;
			break;
		}
	}

	int nPos;
	if (m_vbar.IsWindowVisible())
	{
		SCROLLINFO	info;
		m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);
		nPos = info.nPos;
	}
	else	nPos = 0;

	index += nPos;

	if (index >= 0)
		m_parent->PostMessage(WM_PANELIST, MAKEWPARAM(1, index), (LPARAM)this);		
	
	CWnd::OnLButtonDown(nFlags, point);
}
