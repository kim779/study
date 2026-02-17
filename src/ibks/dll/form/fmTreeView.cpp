// fmTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "fmTreeView.h"
#include "axform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CfmTreeView

IMPLEMENT_DYNCREATE(CfmTreeView, CfmBase)

CfmTreeView::CfmTreeView()
{
	EnableAutomation();
}

CfmTreeView::CfmTreeView(CAxisForm* axform, struct _formR* formR, int iform)
	: CfmBase(axform, formR, iform)
{
	m_totalItem = 0;
	m_itemH     = m_fHeight + 4;
	m_indentX   = (int)(m_fWidth * 1.0);	// 1.3 --> 1.0
	m_showItem  = (int)((float)(m_pRc.Height()-4) / m_itemH);
	m_drawing   = false;

	m_select.RemoveAll();
	m_tree.RemoveAll();

	m_name = CString((char *)m_form->name);
	m_path.Format(_T("%s\\%s\\%s"), LPCTSTR(m_axform->m_root), TABDIR, LPCTSTR(m_name));
	if (loadTree())
		findStartItem();
	else
		m_start = m_item;
	m_type   = ET_TAB;
	m_scroll = isScroll();
	m_scr.SetRectEmpty();
	m_thumbRect.SetRectEmpty();
}

CfmTreeView::~CfmTreeView()
{
	saveItem();

	Ctreeitem*	item;
	for (int ii = 0; ii < m_tree.GetSize(); ii++)
	{
		item = m_tree.GetAt(ii);
		removeItem(item);
		delete item;
	}

	m_tree.RemoveAll();
	m_select.RemoveAll();
	m_totalItem = 0;
}


void CfmTreeView::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CfmTreeView, CfmBase)
	//{{AFX_MSG_MAP(CfmTreeView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CfmTreeView, CfmBase)
	//{{AFX_DISPATCH_MAP(CfmTreeView)
	DISP_PROPERTY_EX(CfmTreeView, "Text", _getText, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CfmTreeView, "Data", _getData, _setData, VT_BSTR)
	DISP_PROPERTY_EX(CfmTreeView, "PRgb", _getPRgb, _setPRgb, VT_I4)
	DISP_PROPERTY_EX(CfmTreeView, "TRgb", _getTRgb, _setTRgb, VT_I4)
	DISP_PROPERTY_EX(CfmTreeView, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_PROPERTY_EX(CfmTreeView, "HitPos", _getHitPos, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CfmTreeView, "Enable", _getEnable, _setEnable, VT_BOOL)
	DISP_FUNCTION(CfmTreeView, "Reload", _Reload, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmTreeView, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmTreeView to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {890032BC-3F92-434A-B553-8353CCD96FBD}
static const IID IID_IfmTreeView =
{ 0x890032bc, 0x3f92, 0x434a, { 0xb5, 0x53, 0x83, 0x53, 0xcc, 0xd9, 0x6f, 0xbd } };

BEGIN_INTERFACE_MAP(CfmTreeView, CfmBase)
	INTERFACE_PART(CfmTreeView, IID_IfmTreeView, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfmTreeView message handlers

void CfmTreeView::_Reload() 
{
	SetEnum(_T(""), -1);
}

void CfmTreeView::_Refresh() 
{
	Refresh();
}

BSTR CfmTreeView::_getText() 
{
	CString strResult = _T("");

	ReadData(strResult, true, -1, -2);
	return strResult.AllocSysString();
}

BSTR CfmTreeView::_getData() 
{
	CString strResult = _T("");

	ReadData(strResult);
	return strResult.AllocSysString();
}

void CfmTreeView::_setData(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue);
}

long CfmTreeView::_getPRgb() 
{
	return m_pRGB;
}

void CfmTreeView::_setPRgb(long nNewValue) 
{
	CfmBase::SetBkColor(nNewValue);
}

long CfmTreeView::_getTRgb() 
{
	return m_tRGB;
}

void CfmTreeView::_setTRgb(long nNewValue) 
{
	CfmBase::SetFgColor(nNewValue);
}

BOOL CfmTreeView::_getVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CfmTreeView::_setVisible(BOOL bNewValue) 
{
	CfmBase::SetVisible(bNewValue ? true : false);
}

long CfmTreeView::_getHitPos() 
{
	return m_axform->DoSomething(doHITPOS, this);
}

BOOL CfmTreeView::_getEnable() 
{
	return (m_form->properties & PR_ENABLE) ? TRUE : FALSE;
}

void CfmTreeView::_setEnable(BOOL bNewValue) 
{
	CfmBase::SetEnable(bNewValue ? true : false);
}

void CfmTreeView::Resize(float hR, float vR, bool font, bool redraw)
{
	m_hR = hR;
	m_vR = vR;

	CRect	tRc, iRc;// , oldRc;

//	oldRc = m_pRc;
	tRc = calcRect(m_rect, hR, vR);
	if (m_pRc.EqualRect(tRc))
		return;

	iRc.left   = min(tRc.left, m_pRc.left);
	iRc.top    = min(tRc.top, m_pRc.top);
	iRc.right  = max(tRc.right, m_pRc.right);
	iRc.bottom = max(tRc.bottom, m_pRc.bottom);

	m_pRc = tRc;
	m_showItem  = (int)((float)(m_pRc.Height()-4) / m_itemH);
	m_thumbRect.SetRectEmpty();
	if (redraw)
	{
		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&iRc, true);
		else
			invalidateRect(&iRc, false);
	}
}

void CfmTreeView::Reload()
{
	CfmBase::Reload();

	m_itemH     = m_fHeight + 4;
	m_indentX   = (int)(m_fWidth * 1.0);	// 1.3 --> 1.0
	m_showItem  = (int)((float)(m_pRc.Height()-4) / m_itemH);

	m_scroll = isScroll();
	m_scr.SetRectEmpty();
	m_thumbRect.SetRectEmpty();
}

void CfmTreeView::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE))
		return;

	CRect	sRc,  tRc;
	CBrush*	brush;
	int	ndc   = dc->SaveDC();
	int	value;
	bool	haschildren = false;

	tRc = m_pRc;
	setFont(dc);
	if (m_form->properties & PR_ENABLE)
		brush = setBrush(dc);
	else
		brush = setBrush(dc, getRGB(cXBG));

	dc->FillRect(m_pRc, brush);
	tRc.DeflateRect(2, 2);
	sRc = tRc;
	m_drawing = false;
	for (int ii = 0; ii < m_tree.GetSize(); ii++)
	{
		clearRect(m_tree.GetAt(ii));
		if (m_tree.GetAt(ii)->m_children.GetSize() > 0)
			haschildren = true;
	}
	int	indention = 1;
	if (!(m_form->properties & PR_LINES) && !haschildren)
		indention = 0;
	for (int ii = 0; ii < m_tree.GetSize(); ii++)
	{
		drawTreeItem(dc, tRc, indention, m_tree.GetAt(ii), true);
		if (ii != m_tree.GetUpperBound())
		{
			value  = sRc.top + ((int)m_itemH - m_indentX)/2;
			value += m_indentX;
			drawLine(dc, CPoint(sRc.left+m_indentX, value), CPoint(sRc.left+m_indentX, tRc.top+m_indentX));
			sRc = tRc;
		}
	}

	if (isScroll())
	{
		sRc = m_pRc;
		sRc.DeflateRect(2, 2);
		if (m_items > 0)
		{
			m_gap  = sRc.Height() - (m_itemH+1)*2;
			m_gap /= m_items;
		}
		else
			m_gap = 0;

		CRect	uRc, dRc;
		uRc = dRc = sRc;
		if (sRc.Width() > (int)(m_indentX*2))
		{
			uRc.left = uRc.right - (int)(m_indentX*2);
			dRc.left = dRc.right - (int)(m_indentX*2);
			sRc.left = sRc.right - (int)(m_indentX*2);
		}

		if (sRc.Height() < (int)((m_itemH+1)*2))
		{
			uRc.bottom = sRc.top + (sRc.Height()) / 2;
			dRc.top    = sRc.top + (sRc.Height()) / 2 + 1;
		}
		else 
		{
			uRc.bottom = sRc.top    + (int)(m_itemH+1);
			dRc.top    = sRc.bottom - (int)(m_itemH+1);
		}
		m_scr = sRc;
		brush = setBrush(dc, getRGB(cLGRAY));
		dc->FillRect(sRc, brush);

		if (m_click == ckScrollUp)
			dc->DrawFrameControl(uRc, DFC_SCROLL, DFCS_SCROLLUP | DFCS_PUSHED);
		else
			dc->DrawFrameControl(uRc, DFC_SCROLL, DFCS_SCROLLUP);
		
		if (m_thumbRect.IsRectEmpty())
		{
			uRc.top = uRc.bottom + (int)(m_startpos * m_gap);
			if (m_startpos + m_showItem == m_items)
				uRc.bottom = dRc.top;
			else
				uRc.bottom = uRc.top + (int)(m_gap * m_showItem);

			uRc.bottom = min(uRc.bottom, dRc.top);
			m_thumbRect = uRc;
		}
		dc->DrawFrameControl(m_thumbRect, DFC_BUTTON, DFCS_BUTTONPUSH);

		if (m_click == ckScrollDown)
			dc->DrawFrameControl(dRc, DFC_SCROLL, DFCS_SCROLLDOWN | DFCS_PUSHED);
		else
			dc->DrawFrameControl(dRc, DFC_SCROLL, DFCS_SCROLLDOWN);
	}

	drawEdge(dc, m_pRc, EDGE_SUNKEN, BF_RECT); 
	dc->RestoreDC(ndc);
}

void CfmTreeView::OnLButton(bool down, CPoint pt, int& result)
{
	CRect	tRc, tRc1;
	Ctreeitem* item = nullptr;
	Ctreeitem* prev  = m_item;
	int	state = (m_form->properties & PR_CHECKED) ? result : 0;
	int	start = m_startpos;

	result  = RC_NOP;
	if (down)
	{
		m_axform->m_cursor = pt;
		m_click = ckNone;
		if (m_scr.PtInRect(pt))
		{
			if (!m_scroll)
				return;

			tRc = tRc1 = m_scr;
			if (tRc.Height() < (int)((m_itemH+1)*2))
			{
				tRc.bottom = tRc.top + (m_scr.Height()) / 2;
				tRc1.top   = tRc1.top + (m_scr.Height()) / 2 + 1;
			}
			else 
			{
				tRc.bottom = tRc.top     + (int)(m_itemH+1);
				tRc1.top   = tRc1.bottom - (int)(m_itemH+1);
			}

			if (tRc.PtInRect(pt))
			{
				if (m_startpos > 0)
				{
					result = RC_UPC;
					m_startpos--;
					if (!findItem(m_startpos, m_start))
					{
						m_startpos = start;
						return;
					}
				}

				m_click = ckScrollUp;
			}
			else if (tRc1.PtInRect(pt))
			{
				if (m_startpos + m_showItem >= m_items)
					return;

				result = RC_DNC;
				m_startpos++;
				if (!findItem(m_startpos, m_start))
				{
					m_startpos = start;
					return;
				}
				m_click = ckScrollDown;
			}
			else if (m_thumbRect.PtInRect(pt))
			{
				m_click  = ckThumb;
				result   = RC_THUMB;
				m_lastPt = pt;
			}
			else
			{
				tRc.top     = tRc.bottom;
				tRc.bottom  = m_thumbRect.top;
				tRc1.top    = m_thumbRect.bottom;
	 			tRc1.bottom = m_scr.bottom -  - (int)(m_itemH+1);

				if (tRc.PtInRect(pt))
					m_click = ckRgnUp;
				else if (tRc1.PtInRect(pt))
					m_click = ckRgnDown;
			}
		}
		else if (m_pRc.PtInRect(pt))
		{
			for (int ii = 0; ii < m_tree.GetSize(); ii++)
			{
				item = m_tree.GetAt(ii);
				if (item->m_rect.PtInRect(pt))
				{
					result  = state;
					m_click = (item->m_type == Ctreeitem::tiFolder) ? ckFolder : ckItem;
					switch (state)
					{
					case RC_CONTROL:
						item->m_selected = !item->m_selected;
						if (item->m_selected)
							addSelect(item);
						else
							removeSelect(item);
						break;

					case RC_SHIFT:
					case RC_CTRLSHIFT:
						if (!m_shift)
							m_shift = m_item;
						shiftState(item);
						break;

					default:
						m_item->m_selected = false;
						item->m_selected   = true;
						removeSelect(m_item, true);
						addSelect(item);
						m_shift = (Ctreeitem *) NULL;
						break;
					}
					m_item = item;
					break;
				}

				if (item->m_button.PtInRect(pt))
				{
					m_click = ckExpand;
					item->m_expanded = !item->m_expanded;
					break;
				}

				if (item->m_expanded)
				{
					if (leftButton(item, pt, state))
						break;
				}
			}
		}
	}

	bool	scroll = m_scroll;
	tRc = m_pRc;
	tRc.DeflateRect(2, 2);
	switch (m_click)
	{
	case ckExpand:
		if (!down || !item) break;
		tRc.top    = item->m_rect.top;
		tRc.right -= (int)(m_indentX*3);
		invalidateRect(&tRc, false);
		tRc = m_pRc;
		tRc.DeflateRect(2, 2);
		tRc.left = tRc.right - (int)(m_indentX*3);
		m_scroll = isScroll();
		if (!m_scroll)
			findStartItem();
		if (scroll == m_scroll)
		{
			tRc.top    += (int)(m_itemH+1);
			tRc.bottom -= (int)(m_itemH+1);
		}
		m_thumbRect.SetRectEmpty();
	//	invalidateRect(&tRc, false);
		invalidateRect(&m_pRc, false);
		break;

	case ckItem:
	case ckFolder:
		if (!down)	break;
		result = state;
		if ((m_form->attr & FA_SEND) && m_select.GetSize() > 0)
			result = RC_SEND;
		else
		{
			if (!state)
				result = (m_click == ckItem) ? RC_ITEM : RC_FOLDER;
		}

		switch (state)
		{
		case RC_SHIFT:
		case RC_CTRLSHIFT:
			break;
		default:
			invalidateRect(&prev->m_rect, false);
			invalidateRect(&m_item->m_rect, false);
			break;
		}
		break;

	case ckScrollUp:
		if (!down)
			m_click = ckNone;
		if (m_startpos != start)
		{
			tRc = m_pRc;
			tRc.DeflateRect(2, 2);
			m_thumbRect.SetRectEmpty();
		}
		else
		{
			tRc = m_scr;
			tRc.bottom = tRc.top + (int)(m_itemH+1);

		}
		invalidateRect(&tRc, false);
		break;

	case ckScrollDown:
		if (!down)
			m_click = ckNone;
		if (m_startpos != start)
		{
			tRc = m_pRc;
			tRc.DeflateRect(2, 2);
			m_thumbRect.SetRectEmpty();
		}
		else
		{
			tRc = m_scr;
			tRc.top = tRc.bottom - (int)(m_itemH+1);
		}
		invalidateRect(&tRc, false);
		break;

	default:
		if (!down)	break;
		tRc = removeSelect(m_item, true);
		invalidateRect(&tRc, false);
		break;
	}
	if (!down)
		m_click = ckNone;
}

void CfmTreeView::OnDblClick(CPoint pt, int& result)
{
	Ctreeitem* item;
	Ctreeitem* prev = m_item;
	m_click = ckNone;

	result = RC_NOP;
	for (int ii = 0; ii < m_tree.GetSize(); ii++)
	{
		item = m_tree.GetAt(ii);
		if (item->m_rect.PtInRect(pt))
		{
			m_click = (item->m_type == Ctreeitem::tiFolder) ? ckFolder : ckItem;
			item->m_expanded = !item->m_expanded;

			m_item->m_selected = false;
			item->m_selected   = true;
			removeSelect(m_item);
			addSelect(item);

			m_item = item;
			break;
		}
		if (item->m_expanded)
		{
			if (doubleClick(item, pt))
				break;
		}
	}

	bool	scroll = m_scroll;
	CRect	tRc = m_pRc;
	tRc.DeflateRect(2, 2);
	switch (m_click)
	{
	case ckItem:
	case ckFolder:
		result  = (m_click == ckFolder) ? RC_FOLDER : RC_ITEM;
		m_shift = (Ctreeitem *) NULL;
		invalidateRect(&prev->m_rect, false);
		tRc.top = m_item->m_rect.top;
		tRc.right -= (int)(m_indentX*3);
		invalidateRect(&tRc, false);
		tRc = m_pRc;
		tRc.DeflateRect(2, 2);
		tRc.left = tRc.right - (int)(m_indentX*3);
		m_scroll = isScroll();
		if (m_scroll == scroll)
		{
			tRc.top    += (int)(m_itemH+1);
			tRc.bottom -= (int)(m_itemH+1);
		}
		m_thumbRect.SetRectEmpty();
		invalidateRect(&tRc, false);
		break;
	default:
		break;
	}
}

bool CfmTreeView::OnRepeat(CPoint pt, int& result)
{
	CRect	tRc;
	float	width = 0;
	int	start = m_startpos;

	result = RC_NOP;
	switch (m_click)
	{
	case ckScrollUp:
		tRc = m_scr;
		if (tRc.Height() < (int)((m_itemH+1)*2))
			tRc.bottom = tRc.top + (tRc.Height())/2;
		else 
			tRc.bottom = tRc.top + (int)(m_itemH+1);

		if (!tRc.PtInRect(pt) || m_startpos <= 0)
			return false;

		result = RC_UPC;
		m_startpos--;
		if (!findItem(m_startpos, m_start))
		{
			m_startpos = start;
			return false;
		}

		tRc = m_pRc;
		tRc.DeflateRect(2, 2);
		m_thumbRect.SetRectEmpty();
		invalidateRect(&tRc, false);
		break;

	case ckScrollDown:
		tRc = m_scr;
		if (tRc.Height() < (int)((m_itemH+1)*2))
			tRc.top = tRc.bottom - (tRc.Height()) / 2;
		else 
			tRc.top = tRc.bottom - (int)(m_itemH+1);

		if (!tRc.PtInRect(pt) || m_startpos + m_showItem >= m_items)
			return false;

		result = RC_DNC;
		m_startpos++;
		if (!findItem(m_startpos, m_start))
		{
			m_startpos = start;
			return false;
		}

		tRc = m_pRc;
		tRc.DeflateRect(2, 2);
		m_thumbRect.SetRectEmpty();
		invalidateRect(&tRc, false);
		break;

	default:
		return false;
	}

	return true;
}

bool CfmTreeView::OnMouseWheel(int delta, int& result)
{
	result = RC_NOP;
	if (delta == 0 || !m_scroll)
		return false;

	int	start = m_startpos;
	Ctreeitem* save = m_start;
	delta  *= WHEEL_AMOUNT;
	delta  *= -1;
	start  += delta;
	if (delta > 0)
		start = min(start, m_items - m_showItem);
	else
		start = max(0, start);

	if (start == m_startpos)
		return false;

	if (!findItem(start, m_start))
		return false;

	CRect	tRc = m_scr;
	tRc.top    += (int)(m_itemH+1);
	tRc.bottom -= (int)(m_itemH+1);
	m_thumbRect.SetRectEmpty();
	invalidateRect(&tRc, false);

	tRc = m_pRc;
	tRc.DeflateRect(2, 2);
	tRc.right = m_scr.left;
	invalidateRect(&tRc, false);

	return true;
}

int CfmTreeView::OnDrag(CPoint pt, int& result)
{
	result = RC_NOP;
	CfmBase::OnDrag(pt, result);
	if (m_click != ckThumb || !m_scroll)
	{
	//	result = RC_OUT;
		return -1;
	}

	int	size, start = m_startpos;
	CRect	tRc1,  tRc2,  iRc1,  iRc2;

	size = m_thumbRect.Height();
	iRc1 = iRc2 = m_thumbRect;
	tRc1 = tRc2 = m_scr;
	tRc1.bottom = tRc1.top    + (int)(m_itemH+1);
	tRc2.top    = tRc2.bottom - (int)(m_itemH+1);
	m_thumbRect.top   += (pt.y - m_lastPt.y);
	m_thumbRect.bottom = m_thumbRect.top + size;

	if (m_thumbRect.top <= tRc1.bottom)
	{
		m_startpos = 0;
		m_thumbRect.top    = tRc1.bottom;
		m_thumbRect.bottom = m_thumbRect.top + size;
	}
	else if (m_thumbRect.bottom >= tRc2.top)
	{
		m_startpos = m_items-m_showItem;
		if (!findItem(m_startpos, m_start))
		{
			m_startpos = start;
			return -1;
		}

		m_thumbRect.bottom = tRc2.top;
		m_thumbRect.top    = m_thumbRect.bottom - size;
	}
	else
	{
		m_startpos = (int)((m_thumbRect.top - tRc1.bottom) / m_gap);
		m_startpos = max(0, min(m_startpos, m_items - m_showItem));
		if (!findItem(m_startpos, m_start))
		{
			m_startpos = start;
			return -1;
		}
	}
	m_lastPt = pt;

	iRc1.bottom = m_thumbRect.top;
	iRc1.NormalizeRect();
	iRc2.top    = iRc2.bottom;
	iRc2.bottom = m_thumbRect.bottom;
	iRc2.NormalizeRect();
	
	if (start != m_startpos)
	{
		tRc2 = m_pRc;
		tRc2.DeflateRect(2, 2);
		tRc2.right = m_scr.left;
		invalidateRect(&tRc2, false);
	}

	if (iRc1.top != iRc1.bottom)
	{
		iRc1.bottom += 2;
		invalidateRect(&iRc1, false);
	}
	if (iRc2.top != iRc2.bottom)
	{
		iRc2.top -= 2;
		invalidateRect(&iRc2, false);
	}

	return -1;
}

bool CfmTreeView::OnDrop(CPoint pt, CString data, CString& key)
{
	Ctreeitem* item;

	key = _T("");
	m_drop = (Ctreeitem *) NULL;
	for (int ii = 0; ii < m_tree.GetSize(); ii++)
	{
		item = m_tree.GetAt(ii);
		if (item->m_rect.PtInRect(pt))
		{
			m_drop = item;
			break;
		}
		if (item->m_expanded)
		{
			if (drop(item, pt))
				break;
		}
	}

	if (m_drop == (Ctreeitem *) NULL || m_drop->m_data.IsEmpty())
		return false;

	key = m_drop->m_data;
	return true;
}

void CfmTreeView::SetFocus(bool focus)
{
	if (m_focus != focus)
	{
		CRect	tRc = m_pRc;
		m_focus = focus;
		tRc.DeflateRect(2, 2);
		invalidateRect(&tRc, false);
	}
}

void CfmTreeView::ReadAll(CString& data, bool edit, bool head)
{
	Ctreeitem* item;
	CString	tmps;

	data = _T("");
	for (int ii = 0; ii < m_select.GetSize(); ii++)
	{
		item = m_select.GetAt(ii);
		if (item == (Ctreeitem *) NULL || item->m_data.IsEmpty())
			continue;

		tmps = item->m_data;
		if (tmps.GetLength() >= m_form->size)
			tmps = tmps.Left(m_form->size);
		else
			tmps += CString(' ', m_form->size - tmps.GetLength());

		data += tmps;
	}
}

void CfmTreeView::ReadData(CString& data, bool edit, int size, int col, int row)
{
	data = _T("");
	if (m_item == (Ctreeitem *) NULL)
		return;

	data = (col < -1) ? m_item->m_name : m_item->m_data;
}

void CfmTreeView::WriteData(CString data, bool redraw, int col, int row)
{
	if (col < -1)
		return;

	Ctreeitem* item;
	bool	find = false;

	for (int ii = 0; ii < m_tree.GetSize(); ii++)
	{
		item = m_tree.GetAt(ii);
		if (!data.Compare(item->m_data))
		{
			setItem(item);
			find = true;
		}
		else if (item->m_type == Ctreeitem::tiFolder)
			find = findItem(item, data, true);

		if (find)
			break;
	}

	if (redraw && find)
	{
		findStartItem();
		m_scroll = isScroll();
		m_scr.SetRectEmpty();
		m_thumbRect.SetRectEmpty();
		CRect	tRc = m_pRc;
		tRc.DeflateRect(2, 2);
		invalidateRect(&tRc, false);
	}
}

void CfmTreeView::ClearData(bool selectOnly)
{
	if (m_tree.GetSize() == 0)
		return;

	setItem(m_tree.GetAt(0));
}

void CfmTreeView::GetEnum(CString& text, int& value, int func)
{
	value = m_type;
	switch (m_type)
	{
	case ET_TAB:
	case ET_USER:
		text = m_name;		break;
	case ET_ITEM:
	default:
		text = _T("");		break;
	}
}

void CfmTreeView::SetEnum(CString text, int value, int func)
{
	saveItem();
	switch (value)
	{
	case -1:
		if (m_path.IsEmpty())
			return;
		break;
	default:
		m_name = text.IsEmpty() ? (char *)m_form->name : text;
		m_path.Format(_T("%s\\%s\\%s"), LPCTSTR(m_axform->m_root), USRDIR, LPCTSTR(m_name));
		m_type = ET_USER;
		break;
	}

	if (loadTree())
		findStartItem();
	else
		m_start = m_item;

	m_scroll = isScroll();
	m_scr.SetRectEmpty();
	m_thumbRect.SetRectEmpty();
	CRect	tRc = m_pRc;
	tRc.DeflateRect(2, 2);
	invalidateRect(&tRc, false);
}

bool CfmTreeView::IsChanged(bool reset)
{
	bool	changed = m_changed;
	if (reset)
		m_changed = false;

	return changed;
}

int CfmTreeView::GetLength()
{
	return m_form->size;
}

bool CfmTreeView::loadTree()
{
	CString	tmps, finds, base;
	Ctreeitem*	item;
	char	wb[128] = { 0, };
	int	offs;
	bool	find, rc;
	
	find = false;
	for (int ii = 0; ii < m_tree.GetSize(); ii++)
	{
		item = m_tree.GetAt(ii);
		removeItem(item);
		delete item;
	}

	m_tree.RemoveAll();
	m_select.RemoveAll();
	m_totalItem = 0;
	m_click     = ckNone;
	m_item      = (Ctreeitem *) NULL;
	m_shift     = (Ctreeitem *) NULL;
	m_start     = (Ctreeitem *) NULL;

	GetPrivateProfileString(LAST_ITEM, "01", "", wb, sizeof(wb), m_path);
	finds = wb;

	for (int ii = 1; ; ii++)
	{
		tmps.Format("%02d", ii);
		GetPrivateProfileString(USR_ROOT, tmps, "", wb, sizeof(wb), m_path);
		tmps = wb;

		if (tmps.IsEmpty())
			break;

		item = new Ctreeitem;
		item->m_name = tmps;
		item->m_data = _T("");
		item->m_type = Ctreeitem::tiItem;
		item->m_rect.SetRectEmpty();
		item->m_button.SetRectEmpty();
		item->m_expanded = false;
		item->m_selected = false;
		item->m_children.RemoveAll();
	
		m_totalItem++;
		if (ii == 1)
		{
			setItem(item);
			m_start = item;
		}

		rc = false;
		base = tmps;
		offs = tmps.Find(';');	// folder
		if (offs != -1)
		{
			rc = true;
			tmps = tmps.Left(offs);
		}
		offs = tmps.Find(':');
		if (offs == -1)
		{
			rc = true;
			item->m_name = tmps;
		}
		else
		{
			item->m_name = tmps.Left(offs);
			item->m_data = tmps.Mid(++offs);
			if (!finds.IsEmpty() && !finds.Compare(item->m_data))
			{
				setItem(item);
				find = true;
			}
		}
		if (rc)
		{
			rc = loadItem(base, item, finds);
			item->m_type = Ctreeitem::tiFolder;
			item->m_expanded = rc ? true : item->m_expanded;
		}
		find = rc ? rc : find;
		m_tree.Add(item);
	}

	return find;
}

bool CfmTreeView::loadItem(CString name, Ctreeitem* parent, CString finds)
{
	Ctreeitem*	item;
	CString	tmps, base;
	char	wb[128] = { 0, };
	int	offs;
	bool	find, rc;
	
	find = false;
	for (int ii = 1; ; ii++)
	{
		tmps.Format("%02d", ii);
		GetPrivateProfileString(name, tmps, "", wb, sizeof(wb), m_path);
		tmps = wb;

		if (tmps.IsEmpty())
			break;

		item = new Ctreeitem;
		item->m_data = _T("");
		item->m_type = Ctreeitem::tiItem;
		item->m_rect.SetRectEmpty();
		item->m_button.SetRectEmpty();
		item->m_expanded = false;
		item->m_selected = false;
		item->m_children.RemoveAll();

		m_totalItem++;

		rc = false;
		base = tmps;
		offs = tmps.Find(';');
		if (offs != -1)
		{
			rc = true;
			tmps = tmps.Left(offs);
		}
		offs = tmps.Find(':');
		if (offs == -1)
		{
			rc = true;
			item->m_name = tmps;
		}
		else
		{
			item->m_name = tmps.Left(offs);
			item->m_data = tmps.Mid(++offs);
			if (!finds.IsEmpty() && !finds.Compare(item->m_data))
			{
				setItem(item);
				find = true;
				parent->m_expanded = true;
			}
		}
		if (rc)
		{
			rc = loadItem(base, item, finds);
			item->m_type = Ctreeitem::tiFolder;
			item->m_expanded = rc ? true : item->m_expanded;
		}
		find = rc ? rc : find;
		parent->m_children.Add(item);
	}

	return find;
}

void CfmTreeView::saveItem()
{
	if (m_item && !m_item->m_data.IsEmpty())
		WritePrivateProfileString(LAST_ITEM, "01", m_item->m_data, m_path);
}

void CfmTreeView::saveItem(CString data)
{
	if (!data.IsEmpty())
		WritePrivateProfileString(LAST_ITEM, "01", data, m_path);
}

void CfmTreeView::removeItem(Ctreeitem* parent)
{
	Ctreeitem*	item;
	for (int ii = 0; ii < parent->m_children.GetSize(); ii++)
	{
		item = parent->m_children.GetAt(ii);
		if (item->m_children.GetSize() != 0)
			removeItem(item);
		item->m_children.RemoveAll();
		delete item;
	}
	parent->m_children.RemoveAll();
}

void CfmTreeView::drawTreeItem(CDC* dc, CRect& rc, int indention, Ctreeitem* item, bool home, bool last)
{
	if (m_start == item)
		m_drawing = true;
	if (!m_drawing)
	{
		if (item->m_children.GetSize() > 0)
		{
			if (item->m_expanded)
			{
				for (int ii = 0; ii < item->m_children.GetSize(); ii++)
				{
					if (ii != item->m_children.GetUpperBound())
						drawTreeItem(dc, rc, indention+2, item->m_children.GetAt(ii));
					else
						drawTreeItem(dc, rc, indention+2, item->m_children.GetAt(ii), false, true);
				}
			}
		}
		return;
	}

	if (rc.top+(m_itemH/2) >= m_pRc.bottom-2 || rc.left >= m_pRc.right-2)
		return;

	CSize	size;
	CBrush*	brush = (CBrush *) 0;
	CRect	tRc,  sRc;
	int	index = 0;
	bool	hasbutton = false;
		
	if (m_form->properties & PR_ENABLE)
	{
		if (m_focus && isItemSelected(item, index))
		{
			brush = setBrush(dc, getRGB(cDBLUE));
			setTextColor(dc, getRGB(cWHITE));
			setPaintColor(dc, getRGB(cDBLUE));
		}
		else
		{
			setTextColor(dc);
			setPaintColor(dc);
		}
	}
	else
	{
		setTextColor(dc, getRGB(cXFG));
		setPaintColor(dc, getRGB(cXFG));
	}

	sRc = tRc = rc;
	tRc.left  += (m_indentX * indention);
	tRc.bottom = tRc.top + (int)m_itemH;
	rc.top += (int)m_itemH;
	drawLine(dc, CPoint(tRc.left, tRc.top+(int)(m_itemH/2)),
				CPoint(tRc.left+(m_indentX*2), tRc.top+(int)(m_itemH/2)));
	if (indention)
		tRc.left += (m_indentX*2);
	tRc.top    += 2;
	tRc.left   += 2;
	tRc.bottom += 2;
	tRc.bottom = min(tRc.bottom, m_pRc.bottom-2);

	size = dc->GetOutputTextExtent(item->m_name);
	tRc.right = min(tRc.right, tRc.left + size.cx + 2);
	item->m_rect = tRc;
	if (m_focus && isItemSelected(item, index) && brush != (CBrush*)0)
	{
		dc->FillRect(&tRc, brush);
		tRc.top += 2;
		dc->DrawText(item->m_name, &tRc, DT_LEFT|DT_TOP|DT_NOPREFIX);
	}
	else
	{
		tRc.top += 2;
		dc->DrawText(item->m_name, &tRc, DT_LEFT|DT_TOP|DT_NOPREFIX);
	}

	if (m_item == item)
	{
		tRc.top -= 2;
		setTextColor(dc, getRGB(cBLACK));
		dc->DrawFocusRect(&tRc);
	}

	tRc = rc;
	tRc.top   -= (int)m_itemH;
	tRc.bottom = tRc.top + (int)m_itemH;
	if (item->m_children.GetSize() > 0)
	{
		drawButton(dc, tRc, indention, item->m_expanded);
		item->m_button = tRc;
		if (!home)
		{
			tRc.left += (m_indentX / 2);
			drawLine(dc, CPoint(tRc.left, sRc.top), CPoint(tRc.left, tRc.top));
		}

		if (item->m_expanded)
		{
			for (int ii = 0; ii < item->m_children.GetSize(); ii++)
			{
				if (ii != item->m_children.GetUpperBound())
					drawTreeItem(dc, rc, indention+2, item->m_children.GetAt(ii));
				else
					drawTreeItem(dc, rc, indention+2, item->m_children.GetAt(ii), false, true);
			}
		}

		if (!home && !last)
			drawLine(dc, CPoint(tRc.left, tRc.bottom), CPoint(tRc.left, rc.top));
	}
	else
	{
		tRc.left  += (m_indentX * indention);
		if (!home)
		{
			drawLine(dc, CPoint(tRc.left, sRc.top), CPoint(tRc.left, tRc.top+(int)(m_itemH/2)));
			if (!last)
				drawLine(dc, CPoint(tRc.left, tRc.top+(int)(m_itemH/2)), CPoint(tRc.left, tRc.bottom));
		}
	}

	for (;;)
	{
		item = getParent(item);
		indention -= 2;
		if (item == (Ctreeitem *) NULL || indention <= 0)
			break;

		Ctreeitem* sibling = getSibling(item, false);
		Ctreeitem* parent  = getParent(item);
		if (sibling == (Ctreeitem *) NULL && parent == (Ctreeitem *) NULL)
			break;

		if (item->m_rect.IsRectEmpty())
		{
			tRc = sRc;
			tRc.left  += (m_indentX * indention);
			tRc.bottom = tRc.top + (int)m_itemH;
			tRc.bottom = min(tRc.bottom, m_pRc.bottom-2);
			drawLine(dc, CPoint(tRc.left, tRc.top), CPoint(tRc.left, tRc.bottom));
		}
	}
}

void CfmTreeView::drawButton(CDC* dc, CRect& rc, int indention, bool expanded)
{
	setBrush(dc);
	setPen(dc, getRGB(cGRAY));

	rc.left  += (indention * m_indentX);
	rc.left  -= (m_indentX / 2);
	rc.right  = rc.left + m_indentX + 2;
	rc.top   += ((rc.Height() - m_indentX) / 2);
	rc.bottom = rc.top  + m_indentX + 2;
	if (!(rc.Height() % 2))
		rc.bottom++;
	if (!(rc.Width() % 2))
		rc.right++;
	dc->Rectangle(&rc);
	
	int	value = rc.top + rc.Height()/2;
	dc->MoveTo(rc.left+2, value);
	dc->LineTo(rc.right-2, value);
	if (!expanded)
	{
		value = rc.left + rc.Width()/2;
		dc->MoveTo(value, rc.top+2);
		dc->LineTo(value, rc.bottom-2);
	}
}

void CfmTreeView::drawLine(CDC* dc, CPoint pt1, CPoint pt2)
{
	if (!(m_form->properties & PR_LINES))
		return;

	CPoint	pt;
	CPen	pen, *ppen;
	LOGBRUSH lb;

	lb.lbStyle = BS_SOLID;
	lb.lbColor = getRGB(cGRAY);
	lb.lbHatch = 0;
	pen.CreatePen(PS_GEOMETRIC|PS_DOT, 1, &lb);
	ppen = dc->SelectObject(&pen);
	if (pt1.x == pt2.x)		// vertical
	{
		if (pt1.y > pt2.y)
		{
			pt  = pt1;
			pt1 = pt2;
			pt2 = pt;
		}
		dc->MoveTo(pt1.x, pt1.y);
		dc->LineTo(pt2.x, pt2.y);
	}
	else if (pt1.y == pt2.y)	// horizontal
	{
		if (pt1.x > pt2.x)
		{
			pt  = pt1;
			pt1 = pt2;
			pt2 = pt;
		}
		dc->MoveTo(pt1.x, pt1.y);
		dc->LineTo(pt2.x, pt2.y);
	}

	dc->SelectObject(ppen);
	pen.DeleteObject();
}

void CfmTreeView::clearRect(Ctreeitem* item)
{
	item->m_button.SetRectEmpty();
	item->m_rect.SetRectEmpty();

	for (int ii = 0; ii < item->m_children.GetSize(); ii++)
		clearRect(item->m_children.GetAt(ii));
}

int CfmTreeView::keyArrowLeft(int& result)
{
	result = RC_NOP;
	if (m_item == (Ctreeitem *) NULL)
		return 0;

	bool	scroll = m_scroll;
	if (m_item->m_expanded)
	{
		m_item->m_expanded = false;
		findStartItem();
		CRect	tRc = m_pRc;
		tRc.DeflateRect(2, 2);
		if (scroll)
			tRc.right -= (int)(m_indentX*3);
		invalidateRect(&tRc, false);
		tRc = m_pRc;
		tRc.DeflateRect(2, 2);
		tRc.left = tRc.right - (int)(m_indentX*3);
		m_scroll = isScroll();
		if (scroll == m_scroll)
		{
			tRc.top    += (int)(m_itemH+1);
			tRc.bottom -= (int)(m_itemH+1);
		}
		m_thumbRect.SetRectEmpty();
		invalidateRect(&tRc, false);
		return 0;
	}

	Ctreeitem* item = getParent(m_item);
	if (item == (Ctreeitem *) NULL)
		return 0;

	Ctreeitem* start = m_start;
	Ctreeitem* item_ = m_item;
	setItem(item);
	findStartItem();
	if (m_item != item_ || m_start != start)
	{
		CRect	tRc = m_pRc;
		tRc.DeflateRect(2, 2);
		if (scroll)
			tRc.right -= (int)(m_indentX*3);
		invalidateRect(&tRc, false);
		tRc = m_pRc;
		tRc.DeflateRect(2, 2);
		tRc.left = tRc.right - (int)(m_indentX*3);
		m_scroll = isScroll();
		if (scroll == m_scroll)
		{
			tRc.top    += (int)(m_itemH+1);
			tRc.bottom -= (int)(m_itemH+1);
		}
		m_thumbRect.SetRectEmpty();
		invalidateRect(&tRc, false);
	}

	return 0;
}

void CfmTreeView::keyArrowRight(int& result)
{
	result = RC_NOP;
	if (m_item->m_children.GetSize() == 0)
		return;

	bool	scroll = m_scroll;
	if (!m_item->m_expanded)
	{
		m_item->m_expanded = true;
		CRect	tRc = m_pRc;
		tRc.DeflateRect(2, 2);
		if (scroll)
			tRc.right -= (int)(m_indentX*3);
		invalidateRect(&tRc, false);
		tRc = m_pRc;
		tRc.DeflateRect(2, 2);
		tRc.left = tRc.right - (int)(m_indentX*3);
		m_scroll = isScroll();
		if (scroll == m_scroll)
		{
			tRc.top    += (int)(m_itemH+1);
			tRc.bottom -= (int)(m_itemH+1);
		}
		m_thumbRect.SetRectEmpty();
		invalidateRect(&tRc, false);
		return;
	}

	Ctreeitem* item = getFirstChild(m_item);
	if (item == (Ctreeitem *) NULL)
		return;

	Ctreeitem* start = m_start;
	Ctreeitem* item_ = m_item;
	setItem(item);
	findStartItem();
	if (m_item != item_ || m_start != start)
	{
		CRect	tRc = m_pRc;
		tRc.DeflateRect(2, 2);
		if (scroll)
			tRc.right -= (int)(m_indentX*3);
		invalidateRect(&tRc, false);
		tRc = m_pRc;
		tRc.DeflateRect(2, 2);
		tRc.left = tRc.right - (int)(m_indentX*3);
		m_scroll = isScroll();
		if (scroll == m_scroll)
		{
			tRc.top    += (int)(m_itemH+1);
			tRc.bottom -= (int)(m_itemH+1);
		}
		m_thumbRect.SetRectEmpty();
		invalidateRect(&tRc, false);
	}
}

void CfmTreeView::keyArrowUp(int& result)
{
	Ctreeitem* item;
	int	state = (m_form->properties & PR_CHECKED) ? result : 0;
	result = RC_NOP;

	switch (state)
	{
	case RC_CONTROL:
	case RC_CTRLSHIFT:
		return;
	default:
		item = getSibling(m_item);
		if (item == (Ctreeitem *) NULL)
			item = getParent(m_item);
		else
		{
			while (item->m_children.GetSize() > 0 && item->m_expanded)
				item = getLastChild(item);
		}
		break;
	}

	if (item == (Ctreeitem *) NULL)
		return;

	if (m_item != item)
	{
		Ctreeitem* prev  = m_item;
		Ctreeitem* start = m_start;

		int	index;
		switch (state)
		{
		case RC_SHIFT:
			if (!m_shift)
				m_shift = m_item;
			if (m_shift != m_item && isItemSelected(m_item, index) && isItemSelected(item, index))
			{
				m_item->m_selected = false;
				removeSelect(m_item);
			}
			else
				addSelect(m_item);
			item->m_selected = true;
			addSelect(item);
			m_item = item;
			break;
		default:
			setItem(item);
			m_shift = (Ctreeitem *) NULL;
			break;
		}

		findStartItem();
		if (m_start == start)
		{
			invalidateRect(&prev->m_rect, false);
			invalidateRect(&m_item->m_rect, false);
		}
		else
		{
			CRect	tRc = m_pRc;
			tRc.DeflateRect(2, 2);
			invalidateRect(&tRc, false);
		}
	}
}

void CfmTreeView::keyArrowDown(int& result)
{
	Ctreeitem* item;
	int	state = (m_form->properties & PR_CHECKED) ? result : 0;
	result = RC_NOP;

	switch (state)
	{
	case RC_CONTROL:
	case RC_CTRLSHIFT:
		return;
	default:
		if (m_item->m_children.GetSize() > 0 && m_item->m_expanded)
			item = getFirstChild(m_item);
		else
		{
			item = getSibling(m_item, false);
			if (item == (Ctreeitem *) NULL)
			{
				bool	root = false;
				Ctreeitem* itemDN = (Ctreeitem *) NULL;
				item = m_item;
				for (;;)
				{
					item = getParent(item, root);
					if (root)
						break;
					if (item != (Ctreeitem *) NULL)
						itemDN = getSibling(item, false);
					if (itemDN != (Ctreeitem *) NULL)
					{
						item = itemDN;
						break;
					}
				}
			}
		}
		break;
	}

	if (item == (Ctreeitem *) NULL)
		return;

	if (m_item != item)
	{
		Ctreeitem* prev  = m_item;
		Ctreeitem* start = m_start;

		int	index;
		switch (state)
		{
		case RC_SHIFT:
			if (!m_shift)
				m_shift = m_item;
			if (m_shift != m_item && isItemSelected(m_item, index) && isItemSelected(item, index))
			{
				m_item->m_selected = false;
				removeSelect(m_item);
			}
			else
				addSelect(m_item);
			item->m_selected = true;
			addSelect(item);
			m_item = item;
			break;
		default:
			setItem(item);
			m_shift = (Ctreeitem *) NULL;
			break;
		}

		findStartItem();
		if (m_start == start)
		{
			invalidateRect(&prev->m_rect, false);
			invalidateRect(&m_item->m_rect, false);
		}
		else
		{
			CRect	tRc = m_pRc;
			tRc.DeflateRect(2, 2);
			invalidateRect(&tRc, false);
		}
	}
}

void CfmTreeView::keyPrior(int& result)
{
	Ctreeitem* item;
	int	order = 0;
	int	state = (m_form->properties & PR_CHECKED) ? result : 0;
	
	result = RC_NOP;
	switch (state)
	{
	case RC_CONTROL:
	case RC_CTRLSHIFT:
		return;

	default:
		for (int ii = 0; ii < m_tree.GetSize(); ii++, order++)
		{
			item = m_tree.GetAt(ii);
			if (m_item == item)
				break;

			if (item->m_expanded)
			{
				if (findItem(item, m_item, ++order))
					break;
			}
		}

		order = max(0, order-m_showItem+1);
		if (!findItem(order, item))
			return;
		break;
	}

	if (m_item == item)
		return;

	CRect	tRc = m_pRc;
	tRc.DeflateRect(2, 2);
	switch (state)
	{
	case RC_SHIFT:
		if (!m_shift)
			m_shift = m_item;
		setItem(item);
		shiftState(item);
		if (m_start != item)
			m_start = item;
		invalidateRect(&tRc, false);
		break;

	default:
		m_shift = (Ctreeitem *) NULL;
		if (m_start == item)
		{
			Ctreeitem* prev = m_item;
			setItem(item);
			invalidateRect(&prev->m_rect,   false);
			invalidateRect(&m_item->m_rect, false);
		}
		else
		{
			m_start = item;
			setItem(item);
			invalidateRect(&tRc, false);
		}
		break;
	}
}

void CfmTreeView::keyNext(int& result)
{
	Ctreeitem* item;
	Ctreeitem* item_;
	int	start, order;
	int	state = (m_form->properties & PR_CHECKED) ? result : 0;
	
	result = RC_NOP;
	switch (state)
	{
	case RC_CONTROL:
	case RC_CTRLSHIFT:
		return;

	default:
		start = order = 0;
		for (int ii = 0; ii < m_tree.GetSize(); ii++, start++)
		{
			item = m_tree.GetAt(ii);
			if (m_item == item)
				break;

			if (item->m_expanded)
			{
				if (findItem(item, m_item, ++start))
					break;
			}
		}

		start = max(0, min(start, m_items-m_showItem));
		if (!findItem(start, item_))
			return;

		order = min(start+m_showItem-1, m_items-1);
		if (!findItem(order, item))
			return;
		break;
	}

	if (m_item == item)
		return;

	switch (state)
	{
	case RC_SHIFT:
		if (!m_shift)
			m_shift = m_item;
		setItem(item);
		shiftState(item);
		break;

	default:
		m_shift = (Ctreeitem *) NULL;
		if (m_start == item_)
		{
			item_  = m_item;
			setItem(item);
			invalidateRect(&item_->m_rect,  false);
			invalidateRect(&m_item->m_rect, false);
		}
		else
		{
			m_start = item_;
			setItem(item);
			CRect	tRc = m_pRc;
			tRc.DeflateRect(2, 2);
			invalidateRect(&tRc, false);
		}
		break;
	}
}

void CfmTreeView::keyHome(int& result)
{
	result = RC_NOP;
	if (m_tree.GetSize() > 0 && m_item != m_tree.GetAt(0))
	{
		setItem(m_tree.GetAt(0));
		CRect	tRc = m_pRc;
		tRc.DeflateRect(2, 2);
		invalidateRect(&tRc, false);
	}
}

void CfmTreeView::keyEnd(int& result)
{
	result = RC_NOP;
	if (m_tree.GetSize() == 0)
		return;

 	Ctreeitem* item = m_tree.GetAt(m_tree.GetUpperBound());
	for (;;)
	{
		Ctreeitem* last = getLastChild(item);
		if (last == (Ctreeitem *) NULL)
			break;

		item = last;
	}

	if (m_item != item)
	{
		setItem(item);
		int	start = max(0, m_items-m_showItem);
		if (!findItem(start, m_start))
			return;

		m_startpos = start;
		CRect	tRc = m_pRc;
		tRc.DeflateRect(2, 2);
		invalidateRect(&tRc, false);
	}
}

Ctreeitem* CfmTreeView::getParent(Ctreeitem* item)
{
	Ctreeitem* parent;
	for (int ii = 0; ii < m_tree.GetSize(); ii++)
	{
		parent = m_tree.GetAt(ii);
		if (parent == item)
			return (Ctreeitem *) NULL;
		if (getParent(item, parent))
			return parent;
	}
	return (Ctreeitem *) NULL;
}

Ctreeitem* CfmTreeView::getParent(Ctreeitem* item, bool& root)
{
	Ctreeitem* parent;
	for (int ii = 0; ii < m_tree.GetSize(); ii++)
	{
		parent = m_tree.GetAt(ii);
		if (parent == item)
		{
			root = true;
			return (Ctreeitem *) NULL;
		}
		if (getParent(item, parent))
			return parent;
	}
	return (Ctreeitem *) NULL;
}

bool CfmTreeView::getParent(Ctreeitem* item, Ctreeitem*& parent)
{
	Ctreeitem*	child;
	for (int ii = 0; ii < parent->m_children.GetSize(); ii++)
	{
		child = parent->m_children.GetAt(ii);
		if (child == item)
			return true;

		if (child->m_children.GetSize() != 0)
		{
			if (getParent(item, child))
			{
				parent = child;
				return true;
			}
		}
	}

	parent = (Ctreeitem *) NULL;
	return false;
}

Ctreeitem* CfmTreeView::getSibling(Ctreeitem* item, bool elder)
{
	int	ii;
	bool	root = false;
	Ctreeitem* sibling = (Ctreeitem *) NULL;
	Ctreeitem* parent = getParent(item, root);
	if (root)
	{
		for (ii = 0; ii < m_tree.GetSize(); ii++)
		{
			if (item == m_tree.GetAt(ii))
				break;

			sibling = m_tree.GetAt(ii);
		}

		if (ii < m_tree.GetSize())
		{
			if (elder)
				return sibling;
			else
			{
				if (ii < m_tree.GetUpperBound())
					return m_tree.GetAt(ii+1);
			}
		}
	}

	if (parent == (Ctreeitem *) NULL)
		return (Ctreeitem *) NULL;

	for (ii = 0; ii < parent->m_children.GetSize(); ii++)
	{
		if (item == parent->m_children.GetAt(ii))
			break;

		sibling = parent->m_children.GetAt(ii);
	}

	if (ii < parent->m_children.GetSize())
	{
		if (elder)
			return sibling;
		else
		{
			if (ii < parent->m_children.GetUpperBound())
				return parent->m_children.GetAt(ii+1);
		}
	}

	return (Ctreeitem *) NULL;
}

Ctreeitem* CfmTreeView::getFirstChild(Ctreeitem* item)
{
	if (item->m_children.GetSize() > 0)
		return item->m_children.GetAt(0);

	return (Ctreeitem *) NULL;
}

Ctreeitem* CfmTreeView::getLastChild(Ctreeitem* item)
{
	if (item->m_children.GetSize() > 0)
		return item->m_children.GetAt(item->m_children.GetUpperBound());

	return (Ctreeitem *) NULL;
}

void CfmTreeView::setItem(Ctreeitem* item)
{
	item->m_selected   = true;
	if (m_item != (Ctreeitem *) NULL)
		m_item->m_selected = false;

	m_item = item;
	m_select.RemoveAll();
	m_select.Add(item);
}

int CfmTreeView::addSelect(Ctreeitem* item)
{
	int	index = -1;
	if (isItemSelected(item, index))
		return m_select.GetUpperBound();

	return m_select.Add(item);
}

CRect CfmTreeView::removeSelect(Ctreeitem* item, bool all)
{
	CRect	tRc; tRc.SetRectEmpty();

	if (all)
	{
		int	ii;

		for (ii = 0; ii < m_select.GetSize(); ii++)
			tRc |= m_select.GetAt(ii)->m_rect;

		m_select.RemoveAll();
		for (ii = 0; ii < m_tree.GetSize(); ii++)
		{
			item = m_tree.GetAt(ii);
			item->m_selected = false;
			resetSelect(item);
		}

		return tRc;
	}

	int	index = -1;
	if (isItemSelected(item, index))
	{
		tRc = m_select.GetAt(index)->m_rect;
		m_select.RemoveAt(index);
	}

	return tRc;
}

void CfmTreeView::resetSelect(Ctreeitem* item)
{
	Ctreeitem*	item_;
	for (int ii = 0; ii < item->m_children.GetSize(); ii++)
	{
		item_ = item->m_children.GetAt(ii);
		item_->m_selected = false;
		resetSelect(item_);
	}
}

bool CfmTreeView::isItemSelected(Ctreeitem* item, int& index)
{
	for (int ii = 0; ii < m_select.GetSize(); ii++)
	{
		if (item == m_select.GetAt(ii))
		{
			index = ii;
			return true;
		}
	}
	return false;
}

bool CfmTreeView::isItemVisible(Ctreeitem* item)
{
	if (!item->m_rect.IsRectEmpty() && !item->m_button.IsRectEmpty())
		return true;

	return false;
}

void CfmTreeView::shiftState(Ctreeitem* item)
{
	Ctreeitem*	item_;
	int	ii, order, order_;
	
	removeSelect(item, true);
	order = order_ = 0;
	for (ii = 0; ii < m_tree.GetSize(); ii++, order++)
	{
		item_ = m_tree.GetAt(ii);
		if (item == item_)
			break;

		if (item_->m_expanded)
		{
			if (findItem(item_, item, ++order))
				break;
		}
	}

	for (ii = 0; ii < m_tree.GetSize(); ii++, order_++)
	{
		item_ = m_tree.GetAt(ii);
		if (m_shift == item_)
			break;

		if (item_->m_expanded)
		{
			if (findItem(item_, m_shift, ++order_))
				break;
		}
	}

	int	tmpN = 0;
	if (order > order_)
	{
		tmpN   = order;
		order  = order_;
		order_ = tmpN;
	}

	for (; order <= order_; order++)
	{
		if (findItem(order, item))
		{
			item->m_selected = true;
			addSelect(item);

		}
	}

	findStartItem();
	CRect	tRc = m_pRc;
	tRc.DeflateRect(2, 2);
	invalidateRect(&tRc, false);
}

void CfmTreeView::findStartItem()
{
	Ctreeitem*	item;
	int	ii, order, order_;
	
	order = order_ = 0;
	for (ii = 0; ii < m_tree.GetSize(); ii++, order++)
	{
		item = m_tree.GetAt(ii);
		if (m_item == item)
			break;

		if (item->m_expanded)
		{
			if (findItem(item, m_item, ++order))
				break;
		}
	}

	for (ii = 0; ii < m_tree.GetSize(); ii++, order_++)
	{
		item = m_tree.GetAt(ii);
		if (m_start == item)
			break;

		if (item->m_expanded)
		{
			if (findItem(item, m_start, ++order_))
				break;
		}
	}

	if (order < order_)	// m_item < m_start
		m_start = m_item;
	else			// m_item > m_start
	{
//		if (order_ + m_showItem >= order+1)
//			return;

		Ctreeitem*	find;
		order_ += (order - (order_ + m_showItem)) + 1;
		order_ = max(0, order_);
		for (ii = 0; ii < m_tree.GetSize(); ii++, order_--)
		{
			item = m_tree.GetAt(ii);
			if (order_ <= 0)
			{
				m_start = item;
				break;
			}

			if (item->m_expanded)
			{
				if (findItem(item, --order_, find))
				{
					m_start = find;
					break;
				}
			}
		}
	}
}

bool CfmTreeView::findItem(Ctreeitem* item, Ctreeitem* find, int& order)
{
	Ctreeitem*	item_;
	for (int ii = 0; ii < item->m_children.GetSize(); ii++, order++)
	{
		item_ = item->m_children.GetAt(ii);
		if (find == item_)
			return true;

		if (item_->m_children.GetSize() > 0 && item_->m_expanded)
		{
			if (findItem(item_, find, ++order))
				return true;
		}
	}
	order--;
	return false;
}

bool CfmTreeView::findItem(Ctreeitem* item, int& order, Ctreeitem*& find)
{
	Ctreeitem*	item_;
	for (int ii = 0; ii < item->m_children.GetSize(); ii++, order--)
	{
		item_ = item->m_children.GetAt(ii);
		if (order == 0)
		{
			find = item_;
			return true;
		}

		if (item_->m_children.GetSize() > 0 && item_->m_expanded)
		{
			if (findItem(item_, --order, find))
				return true;
		}
	}
	order++;
	return false;
}

bool CfmTreeView::findItem(int order, Ctreeitem*& find)
{
	Ctreeitem*	item;
	for (int ii = 0; ii < m_tree.GetSize(); ii++, order--)
	{
		item = m_tree.GetAt(ii);
		if (order == 0)
		{
			find = item;
			return true;
		}

		if (item->m_children.GetSize() > 0 && item->m_expanded)
		{
			if (findItem(item, --order, find))
				return true;
		}
	}
	return false;
}

bool CfmTreeView::findItem(Ctreeitem* item, CString data, bool all)
{
	Ctreeitem*	item_;
	for (int ii = 0; ii < item->m_children.GetSize(); ii++)
	{
		item_ = item->m_children.GetAt(ii);
		if (item_->m_children.GetSize() > 0 && (item_->m_expanded || all))
		{
			if (findItem(item_, data, all))
				return true;
		}
		else
		{
			if (!data.Compare(item->m_data))
			{
				setItem(item_);
				return true;
			}
		}
	}
	return false;
}

bool CfmTreeView::leftButton(Ctreeitem* item, CPoint pt, int state)
{
	Ctreeitem*	item_;
	for (int ii = 0; ii < item->m_children.GetSize(); ii++)
	{
		item_ = item->m_children.GetAt(ii);
		if (item_->m_rect.PtInRect(pt))
		{
			m_click = (item_->m_type == Ctreeitem::tiFolder) ? ckFolder : ckItem;
			switch (state)
			{
			case RC_CONTROL:
				item_->m_selected = !item_->m_selected;
				if (item_->m_selected)
					addSelect(item_);
				else
					removeSelect(item_);
				break;

			case RC_SHIFT:
			case RC_CTRLSHIFT:
				if (!m_shift)
					m_shift = m_item;
				shiftState(item_);
				break;

			default:
				m_item->m_selected = false;
				item_->m_selected  = true;
				removeSelect(m_item, true);
				addSelect(item_);
				m_shift = (Ctreeitem *) NULL;
				break;
			}
			m_item = item_;
			return true;
		}

		if (item_->m_button.PtInRect(pt))
		{
			m_click = ckExpand;
			item_->m_expanded = !item_->m_expanded;
			return true;
		}

		if (item_->m_expanded)
		{
			if (leftButton(item_, pt, state))
				return true;
		}
	}

	return false;
}

bool CfmTreeView::doubleClick(Ctreeitem* item, CPoint pt)
{
	Ctreeitem*	item_;
	for (int ii = 0; ii < item->m_children.GetSize(); ii++)
	{
		item_ = item->m_children.GetAt(ii);
		if (item_->m_rect.PtInRect(pt))
		{
			m_click = (item_->m_type == Ctreeitem::tiFolder) ? ckFolder : ckItem;
			item_->m_expanded = !item_->m_expanded;

			m_item->m_selected = false;
			item_->m_selected  = true;
			removeSelect(m_item, true);
			addSelect(item_);
			
			m_item = item_;
			return true;
		}

		if (item_->m_expanded)
		{
			if (doubleClick(item_, pt))
				return true;
		}
	}

	removeSelect(m_item, true);
	CRect	tRc = m_pRc;
	tRc.DeflateRect(2, 2);
	invalidateRect(&tRc, false);
	return false;
}

bool CfmTreeView::drop(Ctreeitem* item, CPoint pt)
{
	Ctreeitem*	item_;
	for (int ii = 0; ii < item->m_children.GetSize(); ii++)
	{
		item_ = item->m_children.GetAt(ii);
		if (item_->m_rect.PtInRect(pt))
		{
			m_drop = item_;
			return true;
		}

		if (item_->m_expanded)
		{
			if (drop(item_, pt))
				return true;
		}
	}

	return false;
}

bool CfmTreeView::isScroll()
{
	Ctreeitem*	item;

	m_items    = 0;
	m_startpos = 0;
	m_scroll   = false;
	for (int ii = 0; ii < m_tree.GetSize(); ii++)
	{
		item = m_tree.GetAt(ii);
		if (item == m_start)
			m_startpos = m_items;
		m_items++;
		countVisibleItem(item, m_items);
	}

	if (m_items > m_showItem)
		m_scroll = true;

	return m_scroll;
}

void CfmTreeView::countVisibleItem(Ctreeitem* item, int& count)
{
	Ctreeitem*	child;

	if (item->m_children.GetSize() > 0)
	{
		if (item->m_expanded)
		{
			for (int ii = 0; ii < item->m_children.GetSize(); ii++)
			{
				child = item->m_children.GetAt(ii);
				if (child == m_start)
					m_startpos = count;
				count++;
				countVisibleItem(child, count);
			}
		}
	}
}
