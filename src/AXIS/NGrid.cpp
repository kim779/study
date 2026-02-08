// NGrid.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "NGrid.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	SELECTCOLOR	RGB(255, 255,   0)
#define	BORDER		1

#define	TIME_ID		1
#define	TERM_CHEG	200

/////////////////////////////////////////////////////////////////////////////
// class CColumn function	start
CColumn::CColumn()
{
	m_show = true;
	m_list.RemoveAll();
}

CColumn::~CColumn()
{
	m_list.RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////
// CNGrid
CNGrid::CNGrid(CWnd* parent, int viewN, int kind)
{
	m_parent = parent;
	m_viewN  = viewN;
	m_kind   = kind;
	SetColor();
	m_clist.RemoveAll();
	CString	fontName;
	fontName.LoadString(ST_FONTNAME_DEFAULT);
	m_font.CreatePointFont(90, fontName);
	m_jnolist.RemoveAll();

	m_selectN = -1;
//	m_sColor = RGB(255, 255, 21);
	m_bReal = false;
}

CNGrid::~CNGrid()
{
	int		key{};
	CColumn* col{};
	for (POSITION pos = m_clist.GetStartPosition();pos; )
	{
		m_clist.GetNextAssoc(pos, key, col);
		col->m_list.RemoveAll();
		delete col;
	}
	m_clist.RemoveAll();
	m_jnolist.RemoveAll();

	m_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CNGrid, CWnd)
	//{{AFX_MSG_MAP(CNGrid)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

	
/////////////////////////////////////////////////////////////////////////////
// CNGrid user define function

void CNGrid::DrawHead(CDC* pDC)
{
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;

	CRect	cRc, clip;
	CBrush	br, * pbr{};
	CPen	pen, * ppen{};
	CRgn	rgn;

	GetClientRect(cRc);
	pen.CreatePen(PS_SOLID, 0, m_lColor);
	br.CreateSolidBrush(m_hColor);
	
	clip.SetRect(cRc.left, cRc.top, cRc.right-VSCROLLW, cRc.top + CELLHEIGHT+1);
	clip.DeflateRect(1, 1);
	rgn.CreateRectRgn(clip.left, clip.top, clip.right, clip.bottom);
	pDC->SelectClipRgn(&rgn, RGN_COPY);

	CFont*	font = pDC->SelectObject(&m_font);
	ppen = pDC->SelectObject(&pen);
	pbr = pDC->SelectObject(&br);
	pDC->SetBkMode(TRANSPARENT);

	const COLORREF color = pDC->SetTextColor(m_tColor);
	CColumn* col{};
	for (int ii = 0; ii < m_clist.GetCount(); ii++)
	{
		m_clist.Lookup(ii, col);
		if (!col)	continue;

		pDC->Rectangle(col->m_hRc);
		if (!ii)
		{
			CRect	bRc;
			const UINT	nState = 0;

			bRc.SetRect(col->m_hRc.left, col->m_hRc.top, col->m_hRc.left + col->m_hRc.Height(), col->m_hRc.bottom);
			bRc.DeflateRect(4, 4);
		}
		pDC->DrawText(col->m_htext, col->m_hRc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	pDC->SelectClipRgn(NULL);
	pDC->SelectObject(font);
	pDC->SelectObject(ppen);
	pDC->SelectObject(pbr);

	pen.DeleteObject();
	br.DeleteObject();
	rgn.DeleteObject();

	pDC->SetTextColor(color);
}

void CNGrid::DrawCell(bool flag, CDC* pDC)
{
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;

	CRgn		rgn;
	CRect		cRc, clip;
	CPen		pen, * ppen{};
	int			ii{};

	GetClientRect(cRc);
	pen.CreatePen(PS_SOLID, 0, m_lColor);

	CFont*	font = pDC->SelectObject(&m_font);
	ppen = pDC->SelectObject(&pen);
	pDC->SetBkMode(TRANSPARENT);

	clip.SetRect(cRc.left, cRc.top + CELLHEIGHT-1, cRc.right-VSCROLLW, cRc.bottom);
	clip.DeflateRect(1, 1);
	rgn.CreateRectRgn(clip.left, clip.top, clip.right, clip.bottom);
	pDC->SelectClipRgn(&rgn, RGN_COPY);

	bool	fLine = true;
	clip.SetRect(cRc.left, cRc.top + CELLHEIGHT-1,
			cRc.right-VSCROLLW, cRc.top + CELLHEIGHT*2-1);
	for (ii = 0; ii < m_viewN && flag; ii++)
	{
		if (fLine)	pDC->FillSolidRect(clip, m_fcColor);
		else		pDC->FillSolidRect(clip, m_scColor);

		fLine = !fLine;
		clip.OffsetRect(0, CELLHEIGHT-1);
	}
	
	clip.SetRect(cRc.left, cRc.top + CELLHEIGHT-1,
			cRc.right-VSCROLLW, cRc.top + CELLHEIGHT*2-1);
	for (ii = m_vbar.GetScrollPos(); ii <= m_selectN; ii++)
	{
		if (m_selectN == ii)
		{
			pDC->FillSolidRect(clip, m_sColor);
			break;
		}
		clip.OffsetRect(0, CELLHEIGHT-1);
	}

	CColumn* col{};
	for (ii = 0; ii < m_clist.GetCount(); ii++)
	{
		m_clist.Lookup(ii, col);
		if (!col || col->m_hRc.left == col->m_hRc.right)
			continue;
		pDC->MoveTo(col->m_hRc.left, col->m_hRc.top);
		pDC->LineTo(col->m_hRc.left, cRc.bottom);
		
		if (ii == m_clist.GetCount() -1)
		{
			pDC->MoveTo(col->m_hRc.right-1, col->m_hRc.top);
			pDC->LineTo(col->m_hRc.right-1, cRc.bottom);
		}
	}

	int	count{};
	const  int pos = m_vbar.GetScrollPos();
	for (ii = 0; ii < m_clist.GetCount(); ii++)
	{
		m_clist.Lookup(ii, col);
		if (!col)	continue;

		count = 0;
		fLine = true;
		for (int jj = 0; jj < col->m_list.GetSize(); jj++)
		{
			if (jj < pos || col->m_hRc.Width() <= 0)
				continue;

			CString	str, text = col->m_list.GetAt(jj);
			CRect	brc, rc = GetCellRect(count++, ii);
			bool	bFail = false;
			UINT	nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
			const COLORREF color = pDC->SetTextColor(m_tColor);
			const char	sign = '0';

			text.TrimRight();
			if (!flag)
			{
				brc.CopyRect(&rc);
				brc.DeflateRect(1, 1, 1, 0);
				if (m_selectN == jj)
					pDC->FillSolidRect(brc, m_sColor);
				else if (fLine)
					pDC->FillSolidRect(&brc, m_fcColor);
				else	pDC->FillSolidRect(&brc, m_scColor);
			}

			fLine = !fLine;

			switch (m_kind)
			{
			case KIND_NEWS:
				switch (ii)
				{
				case 0:	// key(recn)
				case 2:	// 구분
					str = text;
					nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
					break;
				case 1:	// 체결시간
					str = text;
					if (!str.IsEmpty() && (str.Find(':') == -1))
						str.Format("%s:%s:%s", str.Left(2), str.Mid(2, 2), str.Right(2));
					nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
					break;
				case 3:	// 종목명
					str = " " + text;
					nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
					break;
				default:
					continue;
				}
				break;
			case KIND_CONCLUSION:
				switch (ii)
				{
				case 0:	// 체결시간
					str = text;
					if (str.GetLength() <= 0)
						break;

					if (str.GetAt(0) == '#')
					{
						str = str.Mid(1);
						bFail = true;
					}
					
					if (str.Find(':') == -1)
						str.Format("%s:%s:%s", str.Left(2), str.Mid(2, 2), str.Right(2));
					nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
					if (bFail)
					{
						DrawFail(pDC, str, rc, nFormat, (m_selectN == jj) ? true : false);
						pDC->SetTextColor(color);
						continue;
					}
					break;
				case 1:	// 주문번호
				case 2: // 원주문번호
					str = text + " ";
					nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
	
					if (!str.IsEmpty() && str.GetAt(0) == '#')
					{
						str = str.Mid(1);
						DrawFail(pDC, str, rc, nFormat, (m_selectN == jj) ? true : false);
						pDC->SetTextColor(color);
						continue;
					}			
					break;
				case 3:	// 계좌번호
					str = text;
					if (!str.IsEmpty() && str.GetAt(0) == '#')
					{
						text = str.Mid(1);
						bFail = true;
					}

					str.Format("%s-%s", text.Left(8), text.Mid(8));
					nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
					if (bFail)
					{
						DrawFail(pDC, str, rc, nFormat, (m_selectN == jj) ? true : false);
						pDC->SetTextColor(color);
						continue;
					}
					break;
				case 4:	// 고객명
					str = text;
					nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
					
					if (!str.IsEmpty() && str.GetAt(0) == '#')
					{
						str = str.Mid(1);
						DrawFail(pDC, str, rc, nFormat, (m_selectN == jj) ? true : false);
						pDC->SetTextColor(color);
						continue;
					}
					break;
				case 5:	// 종목명
					str = text;
					nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
					
					if (!str.IsEmpty() && str.GetAt(0) == '#')
					{
						str = str.Mid(1);
						str = " " + str;
						DrawFail(pDC, str, rc, nFormat, (m_selectN == jj) ? true : false);
						pDC->SetTextColor(color);
						continue;
					}
					str = " " + str;

					break;
				case 6: // 매매구분
					nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
					if (text.GetAt(0) == '+')
					{
						text = text.Mid(1);
						pDC->SetTextColor(GetColor(56));
					}
					else if (text.GetAt(0) == '-')
					{
						text = text.Mid(1);
						pDC->SetTextColor(GetColor(57));
					}
					str = text;
					
					if (!str.IsEmpty() && str.GetAt(0) == '#')
					{
						str = str.Mid(1);
						DrawFail(pDC, str, rc, nFormat, (m_selectN == jj) ? true : false);
						pDC->SetTextColor(color);
						continue;
					}
					break;
				case 7:	// 체결수량
				case 8:	// 체결가격
					nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
					str = text;
					if (!str.IsEmpty() && str.GetAt(0) == '#')
					{
						text = str.Mid(1);
						bFail = true;
					}

					text = commaE(text, false);
					str = text + " ";
					
					if (bFail)
					{
						DrawFail(pDC, str, rc, nFormat, (m_selectN == jj) ? true : false);
						pDC->SetTextColor(color);
						continue;
					}
					break;
				default:
					continue;
				}
				break;
			case KIND_RTS:
				switch (ii)
				{
				case 0:	// 종목코드
				case 1:	// RTS data
					str = text;
					nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
					break;
				default:
					continue;
				}
				break;
			default:	continue;
			}
			pDC->DrawText(str, &rc, nFormat);
			pDC->SetTextColor(color);
		}
	}

//	horizontal line
/*	for (ii = 0; ii < m_clist.GetCount(); ii++)
	{
		m_clist.Lookup(ii, col);
		if (!col || ii < m_clist.GetCount() -1)		
			continue;

		CPoint	sp(0, cRc.top + (CELLHEIGHT-1)*2);
		while (sp.y < cRc.bottom)
		{
			pDC->MoveTo(sp);
			pDC->LineTo(col->m_hRc.right-1, sp.y);

			sp.Offset(0, CELLHEIGHT-1);
		}
	}//*/


	pDC->SelectClipRgn(NULL);
	pDC->SelectObject(font);
	pDC->SelectObject(ppen);

	pen.DeleteObject();
	rgn.DeleteObject();	
}

void CNGrid::DrawBorder(CDC* pDC)
{
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;

	CRect	cRc;
	CPen	Pen, *pen;
	GetClientRect(cRc);

	Pen.CreatePen(PS_SOLID, 0, m_lColor);
	pen = pDC->SelectObject(&Pen);

	pDC->MoveTo(cRc.left, cRc.top);
	pDC->LineTo(cRc.right-1, cRc.top);
	pDC->LineTo(cRc.right-1, cRc.bottom-1);
	pDC->LineTo(cRc.left, cRc.bottom-1);
	pDC->LineTo(cRc.left, cRc.top);

	pDC->SelectObject(&pen);
	Pen.DeleteObject();
}

void CNGrid::CreateScroll()
{
	CRect	cRc, sRc;
	GetClientRect(cRc);
	sRc.SetRect(cRc.right - (VSCROLLW+1), 1, cRc.right-1, cRc.bottom-1);
	m_vbar.Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|SBS_VERT|SBS_LEFTALIGN, sRc, this, 2000);

	SCROLLINFO	info;
	info.cbSize	= sizeof(SCROLLINFO);
	info.fMask	= SIF_ALL | SIF_DISABLENOSCROLL; 
	info.nMin	= 0;     
	info.nMax	= 0; 
	info.nPage	= 1;     
	info.nPos	= 0;    
	info.nTrackPos	= 0; 

	m_vbar.SetScrollInfo(&info);
}

void CNGrid::SetScroll(int nPos)
{
	CRect		cRc;
	CColumn* col{};
	SCROLLINFO	info{};

	if (!m_clist.Lookup(0, col))	return;
	if (0 >= col->m_list.GetSize())
	{
		info.cbSize	= sizeof(SCROLLINFO);
		info.fMask	= SIF_ALL | SIF_DISABLENOSCROLL; 
		info.nMin	= 0;     
		info.nMax	= 0; 
		info.nPage	= 1;     
		info.nPos	= 0;    
		info.nTrackPos	= 0; 

		m_vbar.SetScrollInfo(&info);
		Invalidate();
		return;
	}

	GetClientRect(cRc);
	m_vbar.GetScrollInfo(&info, SIF_ALL|SIF_DISABLENOSCROLL);
	info.nMax	= col->m_list.GetSize() - info.nMin;
	info.nPage	= m_viewN +1;

	m_vbar.SetScrollInfo(&info);
	m_bReal = true;
	//Invalidate();
}

void CNGrid::LineUp()
{
	SCROLLINFO	info;
	m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);

	info.nPos--;
	if (info.nMin > info.nPos)	return;
	m_vbar.SetScrollInfo(&info);
	DrawCell(false);
}

void CNGrid::LineDown()
{
	SCROLLINFO	info;
	m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);

	info.nPos++;
	if (info.nMax + 1< info.nPos + (int)info.nPage)	return;
	m_vbar.SetScrollInfo(&info);
	DrawCell(false);
}

void CNGrid::PageUp()
{
	SCROLLINFO	info;
	m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);
	if (info.nPos == info.nMin)	return;

	info.nPos -= info.nPage;
	if (info.nMin > info.nPos)
		info.nPos = info.nMin;
	m_vbar.SetScrollInfo(&info);
	DrawCell(false);
}

void CNGrid::PageDown()
{
	SCROLLINFO	info;
	m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);
	if (info.nPos == info.nMax - (int)info.nPage + 1)	return;
	
	info.nPos += info.nPage;
	if (info.nMax + 1< info.nPos + (int)info.nPage)
		info.nPos = info.nMax - info.nPage + 1;
	m_vbar.SetScrollInfo(&info);
	DrawCell(false);
}

void CNGrid::ThumbTrack(UINT nPos)
{
	SCROLLINFO	info;
	m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);

	if ((int)nPos == info.nMax+1 - (int)info.nPage && info.nPos == (int)nPos)
		return;
	info.nPos = nPos;
	if (info.nPos + (int)info.nPage > info.nMax + 1)
		info.nPos = info.nMax - info.nPage + 1;
	m_vbar.SetScrollInfo(&info);
	DrawCell(false);
}

// external call
void CNGrid::MouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	SCROLLINFO	info{};
	const int		deltaPOS = zDelta/120;

	m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);
	if (deltaPOS > 0)
	{
		if (!info.nPos)	return;
		info.nPos -= deltaPOS*3;
		if (info.nMin > info.nPos)
			info.nPos = info.nMin;
		m_vbar.SetScrollInfo(&info);
	}
	else
	{
		if (info.nPos + (int)info.nPage >= info.nMax + 1)	return;
		info.nPos -= deltaPOS*3;
		if (info.nPos + (int)info.nPage > info.nMax + 1)
			info.nPos = info.nMax - info.nPage + 1;
		m_vbar.SetScrollInfo(&info);
	}

	DrawCell(false);
}
#pragma warning (disable : 26400)
#pragma warning (disable : 26409)
void CNGrid::SetTitle(const CStringArray& ary)
{
	int		idx{}, tmpI{};
	CRect		cRc(0,0,0,0);
	CClientDC	dc(this);

	CFont*	font = dc.SelectObject(&m_font);
	idx = tmpI = 0;
	for (int ii = 0; ii < ary.GetSize(); ii++)
	{
		CColumn* col = new CColumn();
		CString	title = ary.GetAt(ii);

		const CSize size = dc.GetTextExtent(title);
		cRc.SetRect(tmpI, 0, tmpI + size.cx, CELLHEIGHT);
		col->m_htext = title;
		col->m_hRc.CopyRect(&cRc);
		tmpI += size.cx - 1;
		
		m_clist.SetAt(idx++, col);
	}
	dc.SelectObject(font);
	
	CreateScroll();
	SetScroll();
}
#pragma warning (default : 26400)
#pragma warning (default : 26409)

void CNGrid::SetColumnWidth(const CStringArray& ary, bool fit)
{
	CClientDC	dc(this);
	CColumn* col{};
	CSize		size;
	const bool		datEnd = false;
	int		cx = BORDER;

	CFont*	font = dc.SelectObject(&m_font);
	for (int ii = 0; ii < m_clist.GetCount(); ii++)
	{
		m_clist.Lookup(ii, col);
		if (!col || ii >= ary.GetSize())
			continue;

		size = dc.GetTextExtent(ary.GetAt(ii));
		col->m_hRc.left = --cx;
		cx += size.cx;
		col->m_hRc.right = cx;
	}

	dc.SelectObject(font);

	if (fit)
	{
		const int	H = (CELLHEIGHT-1)*m_viewN + CELLHEIGHT;
		SetWindowPos(NULL, 0, 0, cx + VSCROLLW + BORDER, H, SWP_SHOWWINDOW);
	}
}

void CNGrid::SetColumnWidth(int colN, int width)
{
	CColumn* col{};
	int		cx{}, tmpI{};
	for (int ii = colN; ii < m_clist.GetCount(); ii++)
	{
		m_clist.Lookup(ii, col);
		if (!col)	continue;

		if (ii == colN)
			col->m_hRc.right = col->m_hRc.left + width;
		else
		{
			tmpI = col->m_hRc.Width();
			col->m_hRc.left = --cx;
			col->m_hRc.right = col->m_hRc.left + tmpI;
		}
		cx = col->m_hRc.right;
	}
}

//체결시간\t주문번호\t원주문번호\t계좌\t고객명\t종목명\t기호(1)매매구분\t체결수량\t체결가격
BOOL CNGrid::AddData(CString dat, int max, bool bConclusion)
{
	int		ii{}, pos{}, key{}, index = 0;
	CString		str, jno, tmps;
	CColumn*	col = NULL;

	if (dat.IsEmpty())
	{
		for (POSITION po = m_clist.GetStartPosition();po; )
		{
			m_clist.GetNextAssoc(po, key, col);
			col->m_list.RemoveAll();
		}
		SetScroll();
		return FALSE;
	}

	if (!bConclusion)
	{
		tmps = dat;
		pos = tmps.Find('\t');
		if (pos == -1)	return FALSE;

		str = tmps.Left(pos++);
		tmps = tmps.Mid(pos);

		pos = tmps.Find('\t');
		if (pos == -1)	return FALSE;
		str = tmps.Left(pos++);
/*	주문번호 비교
		for (ii = 0; ii < m_jnolist.GetSize(); ii++)
		{
			jno = m_jnolist.GetAt(ii);
			if (!str.Compare(jno))
				return FALSE;
		}*/
	}

	pos = dat.Find('\t');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}

	if(m_clist.Lookup(index++, col))	// 체결시간
	{
		col->m_list.InsertAt(0, str);
		for (ii = max; ii < col->m_list.GetSize() && max != -1 && max != 0; ii++)
			col->m_list.RemoveAt(ii);
	}

	pos = dat.Find('\t');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}

	if(m_clist.Lookup(index++, col))	// 주문번호
	{
		col->m_list.InsertAt(0, str);
		for (ii = max; ii < col->m_list.GetSize() && max != -1 && max != 0; ii++)
			col->m_list.RemoveAt(ii);

		m_jnolist.Add(str);
		if (m_jnolist.GetSize() >= max && max != 0)
			m_jnolist.RemoveAt(0);
	}

	pos = dat.Find('\t');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}

	if(m_clist.Lookup(index++, col))	// 원주문번호
	{
		col->m_list.InsertAt(0, str);
		for (ii = max; ii < col->m_list.GetSize() && max != -1 && max != 0; ii++)
			col->m_list.RemoveAt(ii);
	}

	pos = dat.Find('\t');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}

	if(m_clist.Lookup(index++, col))	// 계좌
	{
		col->m_list.InsertAt(0, str);
		for (ii = max; ii < col->m_list.GetSize() && max != -1 && max != 0; ii++)
			col->m_list.RemoveAt(ii);
	}
	
	pos = dat.Find('\t');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}

	if(m_clist.Lookup(index++, col))	// 고객명
	{
		col->m_list.InsertAt(0, str);
		for (ii = max; ii < col->m_list.GetSize() && max != -1 && max != 0; ii++)
			col->m_list.RemoveAt(ii);
	}

	pos = dat.Find('\t');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}
	if(m_clist.Lookup(index++, col))	// 종목명
	{
		col->m_list.InsertAt(0, str);
		for (ii = max; ii < col->m_list.GetSize() && max != -1 && max != 0; ii++)
			col->m_list.RemoveAt(ii);
	}

	pos = dat.Find('\t');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}
	if(m_clist.Lookup(index++, col))	// 매매구분
	{
		col->m_list.InsertAt(0, str);
		for (ii = max; ii < col->m_list.GetSize() && max != -1 && max != 0; ii++)
			col->m_list.RemoveAt(ii);
	}

	pos = dat.Find('\t');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}
	if(m_clist.Lookup(index++, col))	// 체결수량
	{
		col->m_list.InsertAt(0, str);
		for (ii = max; ii < col->m_list.GetSize() && max != -1 && max != 0; ii++)
			col->m_list.RemoveAt(ii);
	}

	pos = dat.Find('\t');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}
	else	str = dat;
	
	if(m_clist.Lookup(index++, col))	// 체결가격
	{
		col->m_list.InsertAt(0, str);
		for (ii = max; ii < col->m_list.GetSize() && max != -1 && max != 0; ii++)
			col->m_list.RemoveAt(ii);
	}

	SetScroll();
	return TRUE;
}

BOOL CNGrid::AddFail(CString dat, int max)
{
	int		ii{}, pos{}, key{}, index = 0;
	CString		str, jno, tmps;
	CColumn*	col = NULL;

	if (dat.IsEmpty())
	{
		for (POSITION po = m_clist.GetStartPosition();po; )
		{
			m_clist.GetNextAssoc(po, key, col);
			col->m_list.RemoveAll();
		}
		SetScroll();
		return FALSE;
	}

	pos = dat.Find('\t');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}

	if(m_clist.Lookup(index++, col))	// 체결시간
	{
		col->m_list.InsertAt(0, "#" + str);
		for (ii = max; ii < col->m_list.GetSize() && max != -1 && max != 0; ii++)
			col->m_list.RemoveAt(ii);
	}

	pos = dat.Find('\t');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}

	if(m_clist.Lookup(index++, col))	// 주문번호
	{
		col->m_list.InsertAt(0,  "#" + str);
		for (ii = max; ii < col->m_list.GetSize() && max != -1 && max != 0; ii++)
			col->m_list.RemoveAt(ii);

		m_jnolist.Add(str);
		if (m_jnolist.GetSize() >= max && max != 0)
			m_jnolist.RemoveAt(0);
	}

	pos = dat.Find('\t');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}

	if(m_clist.Lookup(index++, col))	// 원주문번호
	{
		col->m_list.InsertAt(0,  "#" + str);
		for (ii = max; ii < col->m_list.GetSize() && max != -1 && max != 0; ii++)
			col->m_list.RemoveAt(ii);
	}

	pos = dat.Find('\t');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}

	if(m_clist.Lookup(index++, col))	// 계좌
	{
		col->m_list.InsertAt(0,  "#" + str);
		for (ii = max; ii < col->m_list.GetSize() && max != -1 && max != 0; ii++)
			col->m_list.RemoveAt(ii);
	}
	
	pos = dat.Find('\t');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}

	if(m_clist.Lookup(index++, col))	// 고객명
	{
		col->m_list.InsertAt(0,  "#" + str);
		for (ii = max; ii < col->m_list.GetSize() && max != -1 && max != 0; ii++)
			col->m_list.RemoveAt(ii);
	}

	pos = dat.Find('\t');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}
	if(m_clist.Lookup(index++, col))	// 종목명
	{
		col->m_list.InsertAt(0,  "#" + str);
		for (ii = max; ii < col->m_list.GetSize() && max != -1 && max != 0; ii++)
			col->m_list.RemoveAt(ii);
	}

	pos = dat.Find('\t');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}
	if(m_clist.Lookup(index++, col))	// 매매구분
	{
		col->m_list.InsertAt(0,  "#" + str);
		for (ii = max; ii < col->m_list.GetSize() && max != -1 && max != 0; ii++)
			col->m_list.RemoveAt(ii);
	}

	pos = dat.Find('\t');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}
	if(m_clist.Lookup(index++, col))	// 체결수량
	{
		col->m_list.InsertAt(0,  "#" + str);
		for (ii = max; ii < col->m_list.GetSize() && max != -1 && max != 0; ii++)
			col->m_list.RemoveAt(ii);
	}

	pos = dat.Find('\t');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}
	else	str = dat;
	
	if(m_clist.Lookup(index++, col))	// 체결가격
	{
		col->m_list.InsertAt(0,  "#" + str);
		for (ii = max; ii < col->m_list.GetSize() && max != -1 && max != 0; ii++)
			col->m_list.RemoveAt(ii);
	}

	SetScroll();
	return TRUE;
}
//recn(key)\t수신시간\t구분\t종목
void CNGrid::AddNews(CString dat)
{
	int		pos{}, index = 0;
	CString		str;
	CColumn*	col = NULL;

	pos = dat.Find('\t');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}
	else	return;

	if(m_clist.Lookup(index++, col))	// RECN(key)
		col->m_list.InsertAt(0, str);

	pos = dat.Find('\t');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}
	else	str.Empty();

	if(m_clist.Lookup(index++, col))	// 체결시간
		col->m_list.InsertAt(0, str);

	pos = dat.Find('\t');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}
	else	str.Empty();

	if(m_clist.Lookup(index++, col))	// 구분
		col->m_list.InsertAt(0, str);

	str = dat;
	if(m_clist.Lookup(index++, col))	// 종목
		col->m_list.InsertAt(0, str);

	m_selectN = 0;
	SetScroll();
}

void CNGrid::AddRts(CString dat)
{
	int		pos{}, ii{}, index = 0;
	const  int max = 100;
	CString		str, code, symbol, value, tmps;
	CColumn*	col = NULL;

	pos = dat.Find('\t');
	code = dat.Left(pos++);
	dat  = dat.Mid(pos);

	if(m_clist.Lookup(index++, col))	// 종목심볼
	{
		col->m_list.InsertAt(0, code);
		for (ii = max; ii < col->m_list.GetSize(); ii++)
			col->m_list.RemoveAt(ii);
	}

	CMapStringToString fms;
	parsedata(dat, fms);

	str.Empty();
	for (POSITION po = fms.GetStartPosition(); po; )
	{
		fms.GetNextAssoc(po, symbol, value);
		value.TrimRight();
		value.TrimLeft();
		tmps.Format("[%s=%s] ", symbol, value);
		str += tmps;
	}
	if(m_clist.Lookup(index++, col))	// RTS data
	{
		col->m_list.InsertAt(0, str);
		for (ii = max; ii < col->m_list.GetSize(); ii++)
			col->m_list.RemoveAt(ii);
	}

	fms.RemoveAll();
	SetScroll();
}

CRect CNGrid::GetCellRect(int row, int col)
{
	CRect		rc;
	CColumn*	column;
	if (!m_clist.Lookup(col, column))	return CRect(0,0,0,0);

	rc.CopyRect(column->m_hRc);
	rc.OffsetRect(0, (CELLHEIGHT-1)*(row+1));
	return rc;
}

void CNGrid::ChangeFont(int point, CString name)
{
//	m_font = GetFont((char *) name.operator LPCTSTR(), point);

	DrawHead();
	DrawCell(false);
	DrawBorder();
}

void CNGrid::ChangePalette()
{
	SetColor();
	DrawHead();
	DrawCell(false);
	DrawBorder();
}

void CNGrid::SetColor()
{
//	m_bkColor = GetColor(146);
	m_hColor = GetColor(74);	// 헤더색
	m_fcColor = GetColor(68);	// 1st row color
	m_scColor = GetColor(77);	// 2st row color
	m_lColor = GetColor(65);
	m_tColor = GetColor(69);
	m_sColor = GetColor(78);	// select row color
}

COLORREF CNGrid::GetColor(int index)
{
	int		pos{};
	CString		tmpS, dat, section, key;
	CStringArray	ary;

	CProfile profile(GetProfileFileName(pkPalette));

	section = profile.GetString(GENERALSN, "Palette", "Palette");

	key.Format("RGB%03d", index);

	dat = profile.GetString(section, key);
	if (dat.IsEmpty())
	{
		if (!section.Compare("Palette"))
			return RGB(0, 0, 0);
		section = _T("Palette");
		dat = profile.GetString(section, key);
		if (dat.IsEmpty())	return RGB(0, 0, 0);
	}

	for (; !dat.IsEmpty();)
	{
		pos = dat.Find(',');
		if (pos == -1)
		{
			ary.Add(dat);
			break;
		}
		tmpS = dat.Left(pos++);
		dat = dat.Mid(pos);
		ary.Add(tmpS);
	}

	if (ary.GetSize() < 3)
		return RGB(215, 215, 215);
	return RGB(atoi(ary.GetAt(0)), atoi(ary.GetAt(1)), atoi(ary.GetAt(2)));
}

CString CNGrid::commaE(CString value, bool remove)
{
	CString		tmpS_, gb, tail;
	int		pos{};

	value.TrimLeft();	value.TrimRight();
	if (value.IsEmpty())	return _T("");

	if (value.Left(1) == "+" || value.Left(1) == "-")
	{
		gb = value.Left(1);
		value = value.Mid(1);
	}
	
	pos = value.Find('.');
	if (pos != -1)
	{
		tail = value.Mid(pos);
		value = value.Left(pos);
	}

	if (remove)	gb = _T("");
	if (value.GetLength() < 3)
		return gb + value + tail;

	for (int ii = value.GetLength()-1, jj = 0; ii >= 0; jj++)
	{
		if (jj != 0 && jj % 3 == 0)
			tmpS_.Insert(0, ',');
		tmpS_.Insert(0, value.GetAt(ii));
		ii--;
	}
	return gb + tmpS_ + tail;
}

/////////////////////////////////////////////////////////////////////////////
// CNGrid message handlers

void CNGrid::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	DrawHead(&dc);
	DrawCell(true, &dc);
	DrawBorder(&dc);
}

void CNGrid::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	SCROLLINFO	info;
	m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);

	switch (nSBCode)
	{
	case	SB_LINEUP:		// move 1
		LineUp();
		break;
	case	SB_LINEDOWN:		// move 1
		LineDown();
		break;
	case	SB_PAGEUP:
		PageUp();		// move nPage
		break;
	case	SB_PAGEDOWN:		// move nPage
		PageDown();
		break;
	case	SB_THUMBTRACK:
		ThumbTrack(nPos);
		break;
	default:
		break;
	}
	SetFocus();
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CNGrid::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	MouseWheel(nFlags, zDelta, pt);
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CNGrid::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect	cRc, sRc;
	GetClientRect(cRc);
	if (m_vbar.GetSafeHwnd())
	{
		sRc.SetRect(cRc.right - VSCROLLW - BORDER, 1, cRc.right - BORDER, cRc.bottom -1);
		m_vbar.MoveWindow(&sRc);
	}
}

void CNGrid::OnLButtonDown(UINT nFlags, CPoint point) 
{
	const int index = GetIndex(point);
	if (index != m_selectN)
	{
		m_selectN = index;
		Invalidate(FALSE);
	}
	if (m_selectN >= 0 && m_kind == KIND_NEWS)
	{
		CString key = GetNewsKey(m_selectN);
		m_parent->SendMessage(WM_NEWSMSG, MAKEWPARAM(NEWS_CLICK, 0), (long)(const char*)key);
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CNGrid::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	const int index = GetIndex(point);
	if (index != m_selectN)
	{
		m_selectN = index;
		Invalidate(FALSE);
	}
	if (m_selectN >= 0 && m_kind == KIND_NEWS)
	{
		CString key = GetNewsKey(m_selectN);
		m_parent->SendMessage(WM_NEWSMSG, MAKEWPARAM(NEWS_DBCLICK, 0), (long)(const char*)key);
	}
	CWnd::OnLButtonDblClk(nFlags, point);
}

CString CNGrid::GetNewsKey(int index)
{
	if (m_kind != KIND_NEWS)
		return _T("");

	CColumn*	col;
	if (!m_clist.Lookup(0, col))		return _T("");
	if (index >= col->m_list.GetSize())	return _T("");
	CString key = col->m_list.GetAt(index);
	return key;
}

int CNGrid::GetIndex(CPoint point)
{
	int	index = -1;
	CRect	cRc, iRc;

	GetClientRect(cRc);
	iRc.SetRect(cRc.left, cRc.top, cRc.right, cRc.top + CELLHEIGHT);
	for (int  ii = 0; ii <= m_viewN; ii++)
	{
		if (!iRc.PtInRect(point))
		{
			iRc.OffsetRect(0, CELLHEIGHT-1);
			continue;
		}
		index = ii;
		break;
	}

	if (index)	index += m_vbar.GetScrollPos();
	return index-1;
}

void CNGrid::parsedata(CString dat, CMapStringToString& ary)
{
	CString sym, value;
	int	pos{};
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

void CNGrid::DrawFail(CDC* pDC, CString str, CRect rect, int nFormat, bool bSelect)
{
	CFont		font;
	CFont*		oldfont = NULL;
	const COLORREF	textColor = pDC->SetTextColor(RGB(0, 0, 0));
	LOGFONT		lf;

	m_font.GetLogFont(&lf);
	lf.lfHeight = 90;

	font.CreatePointFontIndirect(&lf);
	oldfont = pDC->SelectObject(&font);

	if (bSelect)
	{
		pDC->SetTextColor(RGB(215, 0, 0));
	}
	else
	{
		CRect rcBack;

		rcBack.CopyRect(&rect);
		rcBack.DeflateRect(1, 1, 1, 0);
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->FillSolidRect(rcBack, RGB(215, 0, 0));
	}

	pDC->DrawText(str, rect, nFormat);
	pDC->SetTextColor(textColor);
	pDC->SelectObject(oldfont);
	font.DeleteObject();

}

void CNGrid::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case TIME_ID:
		if (m_bReal)
		{
			Invalidate();
			m_bReal = false;
		}
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

int CNGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetTimer(TIME_ID, TERM_CHEG, NULL);	
	return 0;
}

void CNGrid::OnDestroy() 
{
	KillTimer(TIME_ID);
	CWnd::OnDestroy();
}
