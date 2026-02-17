// fmSheet.cpp : implementation file
//

#include "stdafx.h"
#include "fmSheet.h"
#include "axform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CfmSheet

IMPLEMENT_DYNCREATE(CfmSheet, CfmBase)

CfmSheet::CfmSheet()
{
	EnableAutomation();
}

CfmSheet::CfmSheet(CAxisForm* axform, struct _formR* formR, int iform)
	: CfmBase(axform, formR, iform)
{
	m_start = m_item = 0;
	m_drop  = -1;
	m_items = m_form->keys;
	m_total = 0;

	enable();
	m_scr.SetRectEmpty();
	m_cell.SetRectEmpty();
	float	cx = m_pRc.Height()/(float)2.0;
	m_cx = (int)cx;
	if ( (cx - (double)m_cx) >- 0.5 )
		m_cx++;

	m_form->iok = EIO_INPUT;
}

CfmSheet::~CfmSheet()
{
}


void CfmSheet::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CfmSheet, CfmBase)
	//{{AFX_MSG_MAP(CfmSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CfmSheet, CfmBase)
	//{{AFX_DISPATCH_MAP(CfmSheet)
	DISP_PROPERTY_EX(CfmSheet, "Data", _getData, _setData, VT_BSTR)
	DISP_PROPERTY_EX(CfmSheet, "PRgb", _getPRgb, _setPRgb, VT_I4)
	DISP_PROPERTY_EX(CfmSheet, "TRgb", _getTRgb, _setTRgb, VT_I4)
	DISP_PROPERTY_EX(CfmSheet, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_PROPERTY_EX(CfmSheet, "Index", _getIndex, _setIndex, VT_I4)
	DISP_PROPERTY_EX(CfmSheet, "Text", _getText, SetNotSupported, VT_BSTR)
	DISP_FUNCTION(CfmSheet, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmSheet, "GetText", _GetText, VT_BSTR, VTS_I4)
	DISP_FUNCTION(CfmSheet, "GetData", _GetData, VT_BSTR, VTS_I4)
	DISP_FUNCTION(CfmSheet, "SetItems", _SetItems, VT_EMPTY, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmSheet to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {59E54F8E-A9E5-4799-8F09-79FE7B3F999A}
static const IID IID_IfmSheet =
{ 0x59e54f8e, 0xa9e5, 0x4799, { 0x8f, 0x9, 0x79, 0xfe, 0x7b, 0x3f, 0x99, 0x9a } };

BEGIN_INTERFACE_MAP(CfmSheet, CfmBase)
	INTERFACE_PART(CfmSheet, IID_IfmSheet, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfmSheet message handlers

void CfmSheet::_Refresh() 
{
	Refresh();
}

BSTR CfmSheet::_GetText(long index) 
{
	CString strResult;

	strResult = (index >= 0 && index < m_desc.GetSize()) ? m_desc.GetAt(index) : _T("");
	return strResult.AllocSysString();
}

BSTR CfmSheet::_GetData(long index) 
{
	CString strResult;

	strResult = (index >= 0 && index < m_data.GetSize()) ? m_data.GetAt(index) : _T("");
	return strResult.AllocSysString();
}

void CfmSheet::_SetItems(LPCTSTR items) 
{
	WriteAll(items);
}

BSTR CfmSheet::_getText() 
{
	CString strResult = _T("");

	ReadData(strResult, true);
	return strResult.AllocSysString();
}

BSTR CfmSheet::_getData() 
{
	CString strResult = _T("");

	ReadData(strResult);
	return strResult.AllocSysString();
}

void CfmSheet::_setData(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue);
}

long CfmSheet::_getPRgb() 
{
	return m_pRGB;
}

void CfmSheet::_setPRgb(long nNewValue) 
{
	CfmBase::SetBkColor(nNewValue);
}

long CfmSheet::_getTRgb() 
{
	return m_tRGB;
}

void CfmSheet::_setTRgb(long nNewValue) 
{
	CfmBase::SetFgColor(nNewValue);
}

BOOL CfmSheet::_getVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CfmSheet::_setVisible(BOOL bNewValue) 
{
	CfmBase::SetVisible(bNewValue ? true : false);
}

long CfmSheet::_getIndex() 
{
	return GetCurSel();
}

void CfmSheet::_setIndex(long nNewValue) 
{
	SetCurSel(nNewValue);
}

void CfmSheet::Adjust(CPoint pt, float hR, float vR)
{
	CfmBase::Adjust(pt, hR, vR);
	
	float	cx = m_pRc.Height()/(float)2.0;
	m_cx = (int)cx;
	if ( (cx - (double)m_cx) >- 0.5 )
		m_cx++;
}

void CfmSheet::Resize(float hR, float vR, bool font, bool redraw)
{
	CRect	iRc = m_pRc;

	CfmBase::Resize(hR, vR, font);
	float	cx = m_pRc.Height()/(float)2.0;
	m_cx = (int)cx;
	if ( (cx - (double)m_cx) >- 0.5 )
		m_cx++;

	iRc |= m_pRc;
	if (redraw)
	{
		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&iRc, true);
		else
			invalidateRect(&iRc, false);
	}
}

void CfmSheet::Reload()
{
	CfmBase::Reload();

	float	cx = m_pRc.Height()/(float)2.0;
	m_cx = (int)cx;
	if ( (cx - (double)m_cx) > -0.5 )
		m_cx++;
}

void CfmSheet::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE) || !m_total)
		return;

	CRect	tRc, tRc1;
	float	scrW;
	CBrush*	brush;
	CPoint	pts[4];
	CString	str, cells;
	bool	valid;
	int	index;
	UINT	style = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;
	int	ndc = dc->SaveDC();

	setPen(dc);
	setTextColor(dc);
	tRc = m_pRc;
	scrW = (float)(m_pRc.Height());
	tRc.left = tRc.right  - (int)(scrW * 2);
	tRc.top  = tRc.bottom - (int)scrW;
	m_scr = tRc;

	switch (m_form->align)
	{
	case AL_LEFT:
		style |= DT_LEFT;	break;
	case AL_RIGHT:
		style |= DT_RIGHT;	break;
	case AL_CENTER:
	default:
		style |= DT_CENTER;	break;
	}

	m_cell.SetRectEmpty();
	dc->SetBkMode(TRANSPARENT);
	for (int ii = 0; ii < m_items; ii++)
	{
		index = m_start+ii;
		if (index >= m_total)
			break;

		valid = getRect(index, tRc);
		str   = valid ? m_desc.GetAt(index) : _T("");
		switch (m_form->type)
		{
		case SHT_BORDER:
			if (m_item == index)
			{
				setFont(dc, 1);
				brush = setBrush(dc, getRGB(cLGRAY));
				dc->FillRect(tRc, brush);
				drawEdge(dc, tRc, EDGE_SUNKEN, BF_RECT);
			}
			else
			{
				setFont(dc, -1);
				brush = setBrush(dc);
				dc->FillRect(tRc, brush);
				drawEdge(dc, tRc, EDGE_RAISED, BF_RECT);
			}
			dc->SetBkMode(TRANSPARENT);
			tRc.DeflateRect(3, 0);
			break;

		case SHT_SHEET:
			if (m_start+ii == m_item)
			{
				m_cell = tRc;
				cells  = str;
				continue;
			}

			pts[0].x = tRc.left+m_cx;	pts[0].y = tRc.bottom;
			pts[1].x = tRc.right-m_cx;	pts[1].y = tRc.bottom;
			pts[2].x = tRc.right;		pts[2].y = tRc.top;
			pts[3].x = tRc.left;		pts[3].y = tRc.top;

			setFont(dc, -1);
			setPen(dc, getRGB(cBLACK));
			brush = setBrush(dc);
			dc->SetPolyFillMode(ALTERNATE);
			dc->Polygon(pts, 4);
			tRc.DeflateRect(m_cx+1, 0);
			break;
		}
		dc->DrawText(str, -1, tRc, style);
	}

	if (!m_cell.IsRectEmpty())
	{
		pts[0].x = m_cell.left+m_cx;	pts[0].y = m_cell.bottom;
		pts[1].x = m_cell.right-m_cx;	pts[1].y = m_cell.bottom;
		pts[2].x = m_cell.right;	pts[2].y = m_cell.top;
		pts[3].x = m_cell.left;		pts[3].y = m_cell.top;

		setFont(dc, 1);
		setPen(dc, getRGB(cBLACK));
		brush = setBrush(dc, getRGB(cWHITE));
		dc->SetPolyFillMode(ALTERNATE);
		dc->Polygon(pts, 4);

		tRc = m_cell;
		tRc.DeflateRect(m_cx+1, 0);
		dc->DrawText(cells, -1, tRc, style);
	}

	tRc = tRc1 = m_scr;
	tRc.right = tRc.left + m_scr.Width()/2;
	tRc1.left = tRc.right;
	if (m_prev)
	{
		if (m_click == ckPrev)
             		dc->DrawFrameControl(tRc, DFC_SCROLL, DFCS_SCROLLLEFT|DFCS_PUSHED);
		else
             		dc->DrawFrameControl(tRc, DFC_SCROLL, DFCS_SCROLLLEFT);
	}
	else
		dc->DrawFrameControl(tRc, DFC_SCROLL, DFCS_SCROLLLEFT|DFCS_INACTIVE);

	if (m_next)
	{
		if (m_click == ckNext)
			dc->DrawFrameControl(tRc1, DFC_SCROLL, DFCS_SCROLLRIGHT|DFCS_PUSHED);
		else
			dc->DrawFrameControl(tRc1, DFC_SCROLL, DFCS_SCROLLRIGHT);
	}
	else
		dc->DrawFrameControl(tRc1, DFC_SCROLL, DFCS_SCROLLRIGHT|DFCS_INACTIVE);

	dc->RestoreDC(ndc);
}

void CfmSheet::OnLButton(bool down, CPoint pt, int& result)
{
	CRect	tRc, tRc1;
	int	prev  = m_item;
	int	start = m_start;

	result  = RC_NOP;
	if (down)
	{
		m_axform->m_cursor = pt;
		m_click = ckNone;
		if (m_scr.PtInRect(pt))
		{
			if (!m_prev && !m_next)
				return;

			tRc = tRc1 = m_scr;
          		tRc.right = tRc.left + (m_scr.Width() / 2);
                 	tRc1.left = tRc.right;
			if (tRc.PtInRect(pt) && m_prev)
			{
				m_click = ckPrev;
				result  = RC_LEFT;
				m_start = max(0, m_start - max(m_items-1, 1));
				enable();				
			}
			else if (tRc1.PtInRect(pt) && m_next)
			{
				m_click = ckNext;
				result  = RC_RIGHT;
				m_start = min(m_start + max(m_items-1, 1), m_total-m_items);
				enable();
			}
			if (m_form->cursor >= CS_CURSOR)
				m_axform->DoSomething(doCURSOR, this, m_form->cursor);
		}
		else
		{
			int	item = getItem(pt);
			if (item < 0)
				m_item = prev;
			else
			{
				m_item  = item;
				m_click = ckItem;
				if (m_form->cursor >= CS_CURSOR)
					m_axform->DoSomething(doCURSOR, this, m_form->cursor);
			}
		}
	}

	tRc = m_pRc;
	switch (m_click)
	{
	case ckItem:
		if (!down)
			break;
		if (m_form->attr & FA_SEND)
			result = RC_SEND;
		else
			result = RC_ITEM;

		if (m_item != prev)
		{
			getRect(prev, tRc);
			getRect(m_item, tRc1);
			if (!tRc.IsRectEmpty())
				invalidateRect(&tRc, false);
			if (!tRc1.IsRectEmpty())
				invalidateRect(&tRc1, false);
		}
		break;

	case ckPrev:
		if (!down)
			m_click = ckNone;
		if (m_start != start)
			tRc = m_pRc;
		else
		{
			tRc = m_scr;
			tRc.right = tRc.left + (m_scr.Width()/2);
		}
		invalidateRect(&tRc, false);
		break;

	case ckNext:
		if (!down)
			m_click = ckNone;
		if (m_start != start)
			tRc = m_pRc;
		else
		{
			tRc = m_scr;
			tRc.left = tRc.right - (m_scr.Width()/2+1);
		}
		invalidateRect(&tRc, false);
		break;

	default:
		break;
	}

	if (!down)
		m_click = ckNone;
}

bool CfmSheet::OnRepeat(CPoint pt, int& result)
{
	CRect	tRc;
	float	width = 0;
	int	start = m_start;

	result = RC_NOP;
	switch (m_click)
	{
	case ckPrev:
		tRc = m_scr;
       		tRc.right = tRc.left + (m_scr.Width() / 2);
		if (tRc.PtInRect(pt) && m_prev)
		{
			m_click = ckPrev;
			result  = RC_LEFT;
			m_start = max(0, m_start - max(m_items-1, 1));
			enable();				
		}
		else
		{
			m_click = ckNone;
			return false;
		}
		invalidateRect(&m_pRc, false);
		break;

	case ckNext:
		tRc = m_scr;
       		tRc.left = tRc.right - (m_scr.Width() / 2);
		if (tRc.PtInRect(pt) && m_next)
		{
			m_click = ckNext;
			result  = RC_RIGHT;
			m_start = min(m_start + max(m_items-1, 1), m_total-m_items);
			enable();				
		}
		else
		{
			m_click = ckNone;
			return false;
		}
		invalidateRect(&m_pRc, false);
		break;

	default:
		return false;
	}

	return true;
}

int CfmSheet::OnDrag(CPoint pt, int& result)
{
	int rc = CfmBase::OnDrag(pt, result);
	if (result == RC_IN && m_form->cursor >= CS_CURSOR)
	{
		if (m_scr.PtInRect(pt) || getItem(pt) >= 0)
		{
			m_axform->DoSomething(doCURSOR, this, m_form->cursor);
			return rc;
		}
	}
	m_axform->DoSomething(doCURSOR, this, CS_NONE);
	return rc;
}

bool CfmSheet::OnDrop(CPoint pt, CString data, CString& key)
{
	CRect	tRc = m_pRc;

	key = _T("");
	m_drop = getItem(pt);
	if (m_drop == -1 || m_data.GetAt(m_drop).IsEmpty())
		return false;

	key = m_data.GetAt(m_drop);
	return true;
}

void CfmSheet::SetFocus(bool focus)
{
	if (m_focus != focus)
	{
		m_focus = focus;
		invalidateRect(&m_pRc, false);
	}
}

void CfmSheet::ReadAll(CString& data, bool edit, bool head)
{
	data.Empty();
	if (m_total <= 0)
		return;

	int	index = min(m_data.GetUpperBound(), max(0, m_item));
	data  = m_data.GetAt(index);
	data += m_desc.GetAt(index);
	data += '\t';

	CString tmps;
	for (int ii = 0; ii < m_total; ii++)
	{
		tmps.Format(_T("%s%s\t"), LPCTSTR(m_data.GetAt(ii)), LPCTSTR(m_desc.GetAt(ii)));
		data += tmps;
	}
}

void CfmSheet::ReadData(CString& data, bool edit, int size, int col, int row)
{
	data.Empty();
	if (m_item < m_desc.GetSize() && m_item < m_data.GetSize())
		data = edit ? m_desc.GetAt(m_item) : m_data.GetAt(m_item);
}

void CfmSheet::WriteAll(CString str, bool redraw)
{
	m_start = 0;
	if (str.IsEmpty() || str.GetLength() < m_form->size)
	{
		m_item  = 0;
		m_total = 0;
		m_data.RemoveAll();
		m_desc.RemoveAll();

		if (redraw)
		{
			if (m_form->properties & PR_TRANSPARENT)
				invalidateRect(&m_pRc, true);
			else
				invalidateRect(&m_pRc, false);
		}
		enable();
		return;
	}

	CString	tmpS,  indexS;
	int	offs = str.Find('\t');
	if (offs == -1)
		return;

	tmpS = str.Left(offs);
	if (tmpS.GetLength() < m_form->size)
		indexS = _T("");
	else
		indexS = str.Left(m_form->size);
	
	m_data.RemoveAll();
	m_desc.RemoveAll();
	str = (str.GetLength() > offs+1) ? str.Mid(offs+1) : _T("");
	for ( ; true; )
	{
		if (str.IsEmpty())
			break;

		offs = str.Find('\t');
		if (offs == -1)
		{
			m_data.Add(str.Left(m_form->size));
			tmpS = (str.GetLength() > m_form->size) ? str.Mid(m_form->size) : _T("");
			m_desc.Add(tmpS);
			break;
		}

		tmpS = str.Left(offs);
		m_data.Add(tmpS.Left(m_form->size));
		tmpS = (tmpS.GetLength() > m_form->size) ? tmpS.Mid(m_form->size) : _T("");
		m_desc.Add(tmpS);
		str = (str.GetLength() > offs+1) ? str.Mid(offs+1) : _T("");
	}

	m_total = m_data.GetSize();
	if (indexS.IsEmpty())
		m_item = 0;
	else
		WriteData(indexS, false);

	enable();
	if (redraw)
	{
		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&m_pRc, true);
		else
			invalidateRect(&m_pRc, false);
	}
}

void CfmSheet::WriteData(CString data, bool redraw, int col, int row)
{
	int	ii;

	for (ii = 0; ii < m_total; ii++)
	{
		if (!data.Compare(m_data.GetAt(ii)))
		{
			m_item = ii;
			break;
		}
	}

	if (redraw && ii < m_total)
	{
		findStartItem();
		enable();
		invalidateRect(&m_pRc, false);
	}
}

void CfmSheet::ClearData(bool selectOnly)
{
	m_item = 0;
}

void CfmSheet::GetEnum(CString& text, int& value, int func)
{
	if (func > 0)
	{
		switch (func)
		{
		case 100:
			text = (value >= 0 && value < m_data.GetSize()) ? m_data.GetAt(value) : _T("");
			break;
		case 101:
			text = (value >= 0 && value < m_desc.GetSize()) ? m_desc.GetAt(value) : _T("");
			break;
		}
	}
}

int CfmSheet::GetLength()
{
	return m_form->size;
}

void CfmSheet::SetCurSel(int index, bool redraw)
{
	if (index < 0 || index > m_total)
		return;

	m_item = index;
	if (redraw)
	{
		findStartItem();
		enable();
		invalidateRect(&m_pRc, false);
	}
}

void CfmSheet::findStartItem()
{
	if (m_item < m_start)
	{
		for ( ; true; )
		{
			m_start = max(0, m_start-m_items);
			if (m_item >= m_start && m_item < m_start+m_items)
				break;
		}
	}
	else if (m_item >= m_start+m_items)
	{
		for ( ; true; )
		{
			m_start = min(m_start+m_items, m_total-m_items);
			if (m_item >= m_start && m_item < m_start+m_items)
				break;
		}
	}
}

int CfmSheet::findItem(CString data)
{
	for (int ii = 0; ii < m_total; ii++)
	{
		if (!data.Compare(m_data.GetAt(ii)))
			return ii;
	}

	return -1;
}

void CfmSheet::enable()
{
	m_prev = m_next = false;
	if (m_start > 0)
		m_prev = true;
	if (m_start+m_items < m_total)
		m_next = true;
}

int CfmSheet::getItem(CPoint pt)
{
	CRgn	rgn;
	CPoint	pts[6];

	if (!m_cell.IsRectEmpty() && m_cell.PtInRect(pt))
	{
		pts[0].x = m_cell.left;		pts[0].y = m_cell.top;
		pts[1].x = m_cell.left+m_cx;	pts[1].y = m_cell.bottom;
		pts[2].x = m_cell.right-m_cx;	pts[2].y = m_cell.bottom;
		pts[3].x = m_cell.right;	pts[3].y = m_cell.top;

		rgn.CreatePolygonRgn(pts, 4, ALTERNATE);
		if (rgn.PtInRegion(pt))
		{
			rgn.DeleteObject();
			return m_item;
		}
		rgn.DeleteObject();
	}

	CRect	tRc(0,0,0,0);
	for (int ii = 0; ii < m_items; ii++)
	{
		if (!getRect(m_start+ii, tRc) || m_start+ii >= m_total)
			break;

		switch (m_form->type)
		{
		case SHT_BORDER:
			if (tRc.PtInRect(pt))
				return (m_start+ii);
			break;

		case SHT_SHEET:
			if (m_start+ii == m_item)
				continue;

			pts[0].x = tRc.left+m_cx;	pts[0].y = tRc.bottom;
			pts[1].x = tRc.right-m_cx;	pts[1].y = tRc.bottom;
			if (m_start+ii == m_item+1)	// select+1
			{
				pts[2].x = tRc.right;		pts[2].y = tRc.top+m_cx;
				pts[3].x = pts[1].x;		pts[3].y = tRc.top;
				pts[4].x = pts[0].x;		pts[4].y = tRc.top;
				pts[5].x = tRc.left;		pts[5].y = pts[2].y;

				rgn.CreatePolygonRgn(pts, 6, ALTERNATE);
			}
			else if (m_start+ii == m_total-1)	// last
			{
				pts[2].x = tRc.right;		pts[2].y = tRc.top;
				pts[3].x = tRc.left;		pts[3].y = tRc.top;

				rgn.CreatePolygonRgn(pts, 4, ALTERNATE);
			}
			else					// normal
			{
				pts[2].x = tRc.right;		pts[2].y = tRc.top+m_cx;
				pts[3].x = pts[1].x;		pts[3].y = tRc.top;
				pts[4].x = tRc.left;		pts[4].y = tRc.top;

				rgn.CreatePolygonRgn(pts, 5, ALTERNATE);
			}

			if (rgn.PtInRegion(pt))
			{
				rgn.DeleteObject();
				return (m_start+ii);
			}
			rgn.DeleteObject();
			break;
		}
	}

	return -1;
}

bool CfmSheet::getRect(int index, CRect& rect)
{
	rect.SetRectEmpty();
	if (index < m_start || index >= m_start+m_items)
		return false;

	int	ii;
	float	width;

	rect = m_pRc;
	rect.right = m_scr.left;
	switch (m_form->type)
	{
	case SHT_SHEET:
		width  = (float)rect.Width() - m_cx*(m_items+1);
		width /= (float)m_items;
		rect.left += (int)((m_cx + width) * (index-m_start));
		rect.right = (int)(rect.left + width + m_cx*2);
		for (ii = 0; ii < m_items; ii++)
		{
			if (index == m_start+ii)
				return (index < m_total) ? true : false;
		}
		break;

	case SHT_BORDER:
	default:
		width = (float)rect.Width() / (float)m_items;
		for (ii = 0; ii < m_items; ii++)
		{
			rect.right = rect.left + (int)width;
			if (index == m_start+ii)
				return (index < m_total) ? true : false;

			rect.left = rect.right;
		}
		break;
	}

	return false;
}

void CfmSheet::clearSheet()
{
	m_start = 0;
	m_cell.SetRectEmpty();

	m_desc.RemoveAll();
	m_data.RemoveAll();
}
