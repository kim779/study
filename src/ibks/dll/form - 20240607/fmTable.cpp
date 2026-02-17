// fmTable.cpp : implementation file
//

#include "stdafx.h"
#include "fmTable.h"
#include "axform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CfmTable

IMPLEMENT_DYNCREATE(CfmTable, CfmBase)

CfmTable::CfmTable()
{
	EnableAutomation();
}

CfmTable::CfmTable(CAxisForm* axform, struct _formR* formR, int iform, struct _cellR* cellR)
	: CfmBase(axform, formR, iform)
{
	m_tRGB     = getRGB(cBLACK);
	m_showRows = 0;
	m_totRows  = 0;
	m_slit     = 1;
	m_index    = -1;
	m_click    = ckNone;
	m_calc     = true;
	m_cellpos  = Cpos(-1, -1);
	m_curpos   = Cpos(-1, -1);
	m_focus    = false;
	m_hasfocus = (m_form->properties & PR_CHECKED) ? true : false;

	m_data.RemoveAll();
	m_edit.RemoveAll();
	m_pairs.RemoveAll();

	if (cellR == NULL)
		return;

	m_cellR   = cellR;
	m_totRows = LOWORD(m_form->vals[1]);

	m_show = shBoth;
	if (!(m_form->attr2 & GO_MULTI))
	{
		if (m_form->attr2 & GO_XHEAD)
			m_show = shItem;
		else
			m_show = shHead;
	}

	int	length, flash = -1;
	Cpair*	pair;

	m_form->isHint = IH_NOP;
	for (int ii = 0; ii < m_totRows; ii++)
	{
		pair = new Cpair;

		pair->m_rect = m_pRc;
		if (m_show == shHead)
			pair->m_widthx = m_pRc.Width();
		else
			pair->m_widthx = m_cellR[ii].width;

		pair->m_width = (float)pair->m_widthx;
		if (m_cellR[ii].head != NOVALUE)
			pair->m_head = (char *)m_cellR[ii].head;
		else
			pair->m_head = _T("");

		if (m_cellR[ii].attr & FA_FLASH)
			flash = ii;

		if (m_cellR[ii].name != NOVALUE)
		{
			char*	rts;

			rts = (char *)m_cellR[ii].name;
			length = strlen(rts);
			if (length >= offsRTS)
				rts += (length - offsRTS);
			pair->m_rts = atoi(rts);
			if (pair->m_rts == 0)
			{
				length = strlen(rts);
				for (int ii = 0; ii < length; ii++)
				{
// updateXXX_2022XX
					if (!isdigit(static_cast<unsigned char>(*rts)))
					{
						pair->m_rts = -1;
						break;
					}
				}
			}
		}
		else
			pair->m_rts = -1;

		pair->m_hpRGB = (m_cellR[ii].hpRGB & userCOLOR) ? m_cellR[ii].hpRGB : getRGB(m_cellR[ii].hpRGB);
		pair->m_htRGB = (m_cellR[ii].htRGB & userCOLOR) ? m_cellR[ii].htRGB : getRGB(m_cellR[ii].htRGB);
		pair->m_pRGB  = (m_cellR[ii].pRGB  & userCOLOR) ? m_cellR[ii].pRGB  : getRGB(m_cellR[ii].pRGB);
		pair->m_tRGB  = (m_cellR[ii].tRGB  & userCOLOR) ? m_cellR[ii].tRGB  : getRGB(m_cellR[ii].tRGB);
		pair->m_itRGB = (m_cellR[ii].tRGB  & userCOLOR) ? -1 : m_cellR[ii].tRGB;

		if (m_cellR[ii].attr & FA_RIGHT)
			m_cellR[ii].alignC = AL_RIGHT;
		if (m_cellR[ii].isHint != IH_NOP)
		{
			m_form->isHint = IH_STRING;
			m_form->onHint = IH_STRING;
		}

		switch (m_cellR[ii].isEdit)
		{
		case IE_EDIT:
			if (m_cellR[ii].onEdit != NOVALUE)
				pair->m_edit = (char *)&m_axform->m_strR[m_cellR[ii].onEdit];
			break;
		case IE_CORGB:
			m_cellR[ii].isEdit = IE_NOP;
			if (m_cellR[ii].attr & FA_CORGB && m_cellR[ii].onEdit != NOVALUE)
				m_cellR[ii].isEdit = IE_CORGB;
			break;
		default:
			break;
		}

		m_pairs.Add(pair);
	}

	if (flash != -1)
	{
		m_form->attr   |= FA_FLASH;
		m_form->checked = flash;
	}

	m_move  = mvNone;
	m_delta = 0;
	m_moveRc.SetRectEmpty();
	m_movepos = Cpos(-1, -1);

	m_data.RemoveAll();
	m_edit.RemoveAll();

	CString	tmps;
	for (int ii = 0; ii < m_totRows; ii++)
	{
		tmps = CString(' ', m_cellR[ii].size);
		m_data.Add(tmps);
		m_edit.Add(tmps);

		if (m_cellR[ii].properties & PR_VISIBLE)
			m_showRows++;
	}

	float	value;
	m_cellH = LOWORD(m_form->keys);
	if (m_cellH > 0)
	{
	/*	value      = (float)(m_pRc.Height()-m_slit)/(float)(m_cellH+m_slit);
		m_showRows = (int)value;
		if (value - (float)m_showRows > 0.5)
			m_showRows++;
	*/
	}
	else
	{
		value   = (float)(m_pRc.Height()-m_slit*(m_showRows+1))/(float)m_showRows;
		m_cellH = (int)value;
		if ( (value - (double)m_cellH) > 0.5 )
			m_cellH++;
	}
	m_cy = (float)m_cellH;
	
	m_select  = false;
	m_lastpos = Cpos(-1, -1);
}

CfmTable::~CfmTable()
{
	Cpair*	pair;
	for (int ii = 0; ii < m_totRows; ii++)
	{
		pair = m_pairs.GetAt(ii);
		delete pair;
	}
	m_pairs.RemoveAll();
	m_data.RemoveAll();
	m_edit.RemoveAll();
}


void CfmTable::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CfmTable, CfmBase)
	//{{AFX_MSG_MAP(CfmTable)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CfmTable, CfmBase)
	//{{AFX_DISPATCH_MAP(CfmTable)
	DISP_PROPERTY_EX(CfmTable, "Text", _getText, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CfmTable, "Data", _getData, _setData, VT_BSTR)
	DISP_PROPERTY_EX(CfmTable, "Cell", _getCell, _setCell, VT_BSTR)
	DISP_PROPERTY_EX(CfmTable, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_PROPERTY_EX(CfmTable, "Row", _getRow, _setRow, VT_I4)
	DISP_PROPERTY_EX(CfmTable, "Head", _getHead, _setHead, VT_BSTR)
	DISP_PROPERTY_EX(CfmTable, "HitPos", _getHitPos, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CfmTable, "Rows", _getRows, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CfmTable, "HeadWidth", _getHeadWidth, SetNotSupported, VT_I4)
	DISP_FUNCTION(CfmTable, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmTable, "GetTableCell", _GetCell, VT_BSTR, VTS_I4)
	DISP_FUNCTION(CfmTable, "SetTableCell", _SetCell, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CfmTable, "GetTableText", _GetText, VT_BSTR, VTS_I4)
	DISP_FUNCTION(CfmTable, "GetPRgb", _GetPRgb, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION(CfmTable, "SetPRgb", _SetPRgb, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CfmTable, "GetTRgb", _GetTRgb, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION(CfmTable, "SetTRgb", _SetTRgb, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CfmTable, "GetHead", _GetHead, VT_BSTR, VTS_I4)
	DISP_FUNCTION(CfmTable, "SetHead", _SetHead, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CfmTable, "PushTableCell", _Push, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CfmTable, "Clear", _Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmTable, "Excel", _Excel, VT_EMPTY, VTS_BOOL VTS_BOOL VTS_BSTR)
	DISP_FUNCTION(CfmTable, "SetSkip", _SetSkip, VT_EMPTY, VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CfmTable, "GetDataKind", _GetDataKind, VT_I4, VTS_I4)
	DISP_FUNCTION(CfmTable, "GetMode", _GetMode, VT_I4, VTS_I4)
	DISP_FUNCTION(CfmTable, "GetLength", _GetLength, VT_I4, VTS_I4)
	DISP_FUNCTION(CfmTable, "GetFormat", _GetFormat, VT_BSTR, VTS_I4)
	DISP_FUNCTION(CfmTable, "SetTableText", _SetText, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CfmTable, "GetFloatDigit", _GetFloatDigit, VT_I4, VTS_I4)
	DISP_FUNCTION(CfmTable, "GetRowName", _GetRowName, VT_BSTR, VTS_I4)
	DISP_FUNCTION(CfmTable, "SetFloatDigit", _SetFloatDigit, VT_EMPTY, VTS_I4 VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmTable to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {FC28F962-0228-436F-8659-B33D87F25690}
static const IID IID_IfmTable =
{ 0xfc28f962, 0x228, 0x436f, { 0x86, 0x59, 0xb3, 0x3d, 0x87, 0xf2, 0x56, 0x90 } };

BEGIN_INTERFACE_MAP(CfmTable, CfmBase)
	INTERFACE_PART(CfmTable, IID_IfmTable, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfmTable message handlers

void CfmTable::_Refresh() 
{
	Refresh();
}

BSTR CfmTable::_GetCell(long row) 
{
	CString strResult = _T("");

	ReadData(strResult, false, -1, -1, row-1);
	return strResult.AllocSysString();
}

void CfmTable::_SetCell(long row, LPCTSTR cells) 
{
	WriteData(cells, true, -1, row-1);
}

BSTR CfmTable::_GetText(long row) 
{
	CString strResult = _T("");

	ReadData(strResult, true, -1, -1, row-1);
	return strResult.AllocSysString();
}

long CfmTable::_GetPRgb(long row, long column) 
{
	return GetBkColor(column-1, row-1);
}

void CfmTable::_SetPRgb(long row, long column, long rgb) 
{
	SetBkColor(rgb, column-1, row-1);
}

long CfmTable::_GetTRgb(long row, long column) 
{
	return GetFgColor(column-1, row-1);
}

void CfmTable::_SetTRgb(long row, long column, long rgb) 
{
	SetFgColor(rgb, column-1, row-1);
}

BSTR CfmTable::_GetHead(long row) 
{
	CString strResult;

	strResult = m_pairs.GetAt(row-1)->m_head;
	return strResult.AllocSysString();
}

void CfmTable::_SetHead(long row, LPCTSTR head) 
{
	if (row <= 0)
		return;

	m_pairs.GetAt(row-1)->m_head = head;
	if (m_pRc.Height() ==0 || m_pRc.Width() == 0 || m_show == shItem || !(m_cellR[row-1].properties & PR_VISIBLE))
		return;

	Cpair*	pair = m_pairs.GetAt(row-1);
	CRect	tRc  = pair->m_rect;
	if (m_show == shBoth)
		tRc.right = tRc.left + (int)pair->m_width;
	tRc.bottom = min(tRc.bottom, m_pRc.bottom-m_slit);
	invalidateRect(&tRc, false);
}

void CfmTable::_Push(long row, LPCTSTR name) 
{
	m_axform->DoSomething(doPUSHX, this, MAKEWPARAM(row-1, 0), (LPARAM)name);
}

void CfmTable::_Clear() 
{
	ClearData();
}

void CfmTable::_Excel(BOOL run, BOOL head, LPCTSTR path) 
{
	m_axform->DoSomething(doEXCEL, this, MAKEWPARAM(run ? 1 : 0, head ? 1 : 0), (LPARAM)path);
}

void CfmTable::_SetSkip(LPCTSTR cname, BOOL skip) 
{
	CString	name = CString(cname);
	for (int ii = 0; ii < m_totRows; ii++)
	{
		if (!name.CompareNoCase((char *)m_cellR[ii].name))
		{
			if (skip)
				m_cellR[ii].attr |= FA_SKIP;
			else
				m_cellR[ii].attr &= ~FA_SKIP;
			break;
		}
	}
}

long CfmTable::_GetDataKind(long row) 
{
	if (row > 0 && row <= m_totRows)
		return m_cellR[row-1].type;

	return 0;
}

long CfmTable::_GetMode(long row) 
{
	if (row > 0 && row <= m_totRows)
		return m_cellR[row-1].iok;

	return 0;
}

long CfmTable::_GetLength(long row) 
{
	if (row > 0 && row <= m_totRows)
		return m_cellR[row-1].size;

	return 0;
}

BSTR CfmTable::_GetFormat(long row) 
{
	CString strResult = _T("");

	if (row > 0 && row <= m_totRows)
		strResult = m_pairs.GetAt(row-1)->m_edit;
	return strResult.AllocSysString();
}

void CfmTable::_SetText(long row, LPCTSTR text) 
{
	if (row > 0 && row <= m_totRows)
	{
		row--;
		m_data.SetAt(row, text);
		m_edit.SetAt(row, text);

		if ((m_form->properties & PR_VISIBLE) && (m_cellR[row].properties & PR_VISIBLE))
		{
			if (m_pRc.Height() == 0 || m_pRc.Width() == 0 || m_show == shHead)
				return;

			CRect	tRc = m_pairs.GetAt(row)->m_rect;
			if (m_show != shBoth)
				tRc.left++;
			tRc.bottom = min(tRc.bottom, m_pRc.bottom-m_slit);
			invalidateRect(&tRc, false);
		}
	}
}

long CfmTable::_GetFloatDigit(long row) 
{
	if (row > 0 && row <= m_totRows)
	{
		row--;
		if (m_cellR[row].attr & FA_FLOAT)
			return (int)m_cellR[row].vals[1];
	}
	return 0;
}

void CfmTable::_SetFloatDigit(long row, long val) 
{
	if (row > 0 && row <= m_totRows)
	{
	 	row--;
	 	if (m_cellR[row].attr & FA_FLOAT)
		 	m_cellR[row].vals[1] = val;
	}
}

BSTR CfmTable::_GetRowName(long row) 
{
	CString strResult = _T("");

	if (row > 0 && row <= m_totRows)
		strResult = CString((char *)m_cellR[row-1].name);

	return strResult.AllocSysString();
}

BSTR CfmTable::_getText() 
{
	CString strResult = _T("");

	ReadData(strResult, true);
	return strResult.AllocSysString();
}

BSTR CfmTable::_getData() 
{
	CString strResult = _T("");

	ReadData(strResult);
	return strResult.AllocSysString();
}

void CfmTable::_setData(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue);
}

BSTR CfmTable::_getCell() 
{
	CString strResult = _T("");

	ReadData(strResult);
	return strResult.AllocSysString();
}

void CfmTable::_setCell(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue);
}

BOOL CfmTable::_getVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CfmTable::_setVisible(BOOL bNewValue) 
{
	CfmBase::SetVisible(bNewValue ? true : false);
}

long CfmTable::_getRow() 
{
	return GetCellRow();
}

void CfmTable::_setRow(long nNewValue) 
{
	SetCellRow(nNewValue);
}

BSTR CfmTable::_getHead() 
{
	CString strResult;

	ReadData(strResult, false, -1, -2);
	return strResult.AllocSysString();
}

void CfmTable::_setHead(LPCTSTR lpszNewValue) 
{
	if (m_cellpos.m_row > 0 && m_cellpos.m_row <= m_totRows)
		WriteData(lpszNewValue, true, -2);
}

long CfmTable::_getHitPos() 
{
	return m_axform->DoSomething(doHITPOS, this);
}

long CfmTable::_getRows() 
{
	return m_totRows;
}

long CfmTable::_getHeadWidth() 
{
	if (m_totRows > 0)
	{
		int	width = 0;

		switch (m_show)
		{
		case shBoth:
			width = m_pairs.GetAt(0)->m_widthx;
			break;
		case shHead:
			width = m_rect.Width();
			break;
		default:
			width = 0;
			break;
		}

		if (width > 0)
		{
			float	ratio;

			ratio = (float)m_axform->m_rect.Width() / (float)m_axform->m_mapH->width;
			width = (int)(width / ratio);
			return width;
		}
	}
	return 0;
}

int CfmTable::GetName(int col)
{
	if (col < 0)
		return CfmBase::GetName();

	Cpair* pair = m_pairs.GetAt(col);
	return pair->m_rts;
}

void CfmTable::Adjust(CPoint pt, float hR, float vR)
{
	Cpair*	pair;
	float	value;

	m_move = mvNone;
	m_moveRc.SetRectEmpty();
	for (int ii = 0; ii < m_totRows; ii++)
	{
		pair = m_pairs.GetAt(ii);
		pair->m_widthx = (WORD)(pair->m_widthx * hR);
	}

	value   = m_cellH * vR;
	m_cellH = (int)value;
	if ((value - (double)m_cellH) > 0.5)
		m_cellH++;
	m_cy = (float)m_cellH;
	m_calc = true;
	CfmBase::Adjust(pt, hR, vR);
}

void CfmTable::Resize(float hR, float vR, bool font, bool redraw)
{
	int	value;
	Cpair*	pair;

	m_move = mvNone;
	m_moveRc.SetRectEmpty();
	for (int ii = 0; ii < m_totRows; ii++)
	{
		pair = m_pairs.GetAt(ii);
		pair->m_width = pair->m_widthx * hR;
		value = (int)pair->m_width;
		if ((pair->m_width - (double)value) > 0.5)
			pair->m_width = (float)value + 1;
		else
			pair->m_width = (float)value;
	}

	m_cy = m_cellH * vR;
	m_calc = true;
	CfmBase::Resize(hR, vR, font, redraw);
}

void CfmTable::Reload()
{
	CfmBase::Reload();

	Cpair*	pair;
	float	value;

	for (int ii = 0; ii < m_totRows; ii++)
	{
		pair = m_pairs.GetAt(ii);
		pair->m_rect = m_pRc;
		if (m_show == shHead)
			pair->m_widthx = m_pRc.Width();
		else
			pair->m_widthx = m_cellR[ii].width;

		pair->m_width = (float)pair->m_widthx;
	}
	m_move = mvNone;
	m_moveRc.SetRectEmpty();

	m_cellH = LOWORD(m_form->keys);
	if (m_cellH > 0)
	{
	/*	value      = (float)(m_pRc.Height()-m_slit)/(float)(m_cellH+m_slit);
		m_showRows = (int)value;
		if (value - (float)m_showRows > 0.5)
			m_showRows++;
	*/
	}
	else
	{
		value   = (float)(m_pRc.Height()-m_slit*(m_showRows+1))/(float)m_showRows;
		m_cellH = (int)value;
		if ((value - (double)m_cellH) > 0.5)
			m_cellH++;
	}
	m_cy = (float)m_cellH;
}

void CfmTable::Draw(CDC* dc)
{
 	if (!(m_form->properties & PR_VISIBLE))
		return;

	CRect	clRect, tRc;
	CBrush*	brush;
	float	dy;
	CString	tmpS = _T("");
	UINT	format;
	int	dx = 0,  dd;
	int	ndc = dc->SaveDC();
	Cpair*	pair;
	COLORREF pRGB, tRGB;
	
	setPen(dc);
	setFont(dc);
	if ((m_pRc.Height() == 0) || (m_pRc.Width() == 0))
		return;

	dd = 1;
	if (m_slit > 1)
	{
		float	fdd = (float)m_slit / (float)2;
		dd = (int)fdd;
		if ((fdd - (double)dd) >= 0.5)
			dd++;
	}
	
	if (m_totRows != 0 && m_showRows != 0)
	{
		dy   = (float)m_pRc.top + m_slit;

		m_cell.SetRectEmpty();
		for (int ii = 0; ii < m_totRows; ii++)
		{
			if (!(m_cellR[ii].properties & PR_VISIBLE))
				continue;

			pair = m_pairs.GetAt(ii);
			dx = 0;
			clRect.top = (int)dy;
			dy += m_cy;

			if (dy > m_pRc.bottom-m_slit || ii == m_totRows-1)
				dy = (float)m_pRc.bottom-m_slit;

			clRect.left   = m_pRc.left+m_slit;
			dy += m_slit;
			clRect.bottom = (int)dy;
			if (m_show == shBoth || m_show == shHead)
			{
				clRect.right = m_pRc.left + (int)pair->m_width;
				clRect.right = min(clRect.right, m_pRc.right-m_slit);
				if (m_curpos.m_row-1 == ii && m_curpos.m_col == 1)
					m_cell = clRect;
				dx = clRect.right;

				pRGB = pair->m_hpRGB;
				tRGB = pair->m_htRGB;

				if (isReverseCell(ii, 0))
				{
					pRGB = getRGB(cBLACK);
					tRGB = getReverseColor(tRGB);
				}
				brush = setBrush(dc, pRGB);
         			dc->FillRect(clRect, brush);
	               		setTextColor(dc, tRGB);
				dc->SetBkMode(TRANSPARENT);

				if (!pair->m_head.IsEmpty())
				{
					format = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;
					switch (m_cellR[ii].alignH)
					{
					case AL_CENTER:
						format |= DT_CENTER;	break;
					case AL_RIGHT:
						format |= DT_RIGHT;	break;
					case AL_LEFT:
					default:
						format |= DT_LEFT;	break;
					}
					tRc = clRect;
					tRc.left  += 3;
					tRc.right -= 3;
					dc->DrawText(pair->m_head, tRc, format);
				}

				if (m_move == mvHead && m_movepos.m_row == ii && m_cellR[ii].hcursor == CS_POP)
				{
					CRect	ppRc = clRect;
					if (ppRc.bottom == m_pRc.bottom)
						ppRc.bottom--;
					drawEdge(dc, ppRc, EDGE_RAISED, BF_RECT);
				}
				clRect.left  = dx;
			}
			clRect.right = m_pRc.right - m_slit;

			if (clRect.left == clRect.right || m_show == shHead)
			{
				if (ii != m_totRows - 1 && !(m_form->attr2 & GO_XHLINE))
				{
					setPen(dc, m_bRGB, m_slit);
					dc->MoveTo(m_pRc.left+dd, (int)dy-dd);
					dc->LineTo(clRect.right,  (int)dy-dd);
				}
				if (m_calc)
				{
					pair->m_rect = clRect;
					pair->m_rect.left = m_pRc.left+m_slit;
				}
				continue;
			}

			if (m_curpos.m_row-1 == ii
				&& ((m_show == shBoth && m_curpos.m_col == 2) || (m_show == shItem && m_curpos.m_col == 1)))
				m_cell = clRect;

			if (m_calc)
			{
				pair->m_rect = clRect;
				if (m_show == shBoth)
					pair->m_rect.left = m_pRc.left+m_slit;
			}

			pRGB = pair->m_pRGB;
			tRGB = pair->m_tRGB;
			if (isReverseCell(ii, (m_show == shBoth) ? 1 : 0))
			{
				pRGB = getRGB(cBLACK);
				tRGB = getReverseColor(tRGB);
			}
			brush = setBrush(dc, pRGB);
			dc->FillRect(clRect, brush);
			setTextColor(dc, tRGB);
			dc->SetBkMode(TRANSPARENT);
			if (m_move == mvItem && m_movepos.m_row == ii && m_cellR[ii].cursor == CS_POP)
			{
				CRect	ppRc = clRect;
				if (ppRc.bottom == m_pRc.bottom)
					ppRc.bottom--;
				drawEdge(dc, ppRc, EDGE_RAISED, BF_RECT);
			}

			tmpS = m_edit.GetAt(ii);
			tRc = clRect;
			tRc.left  += 3;
			tRc.right -= (int)(m_fWidth/2);
			setPen(dc, pRGB);
			setBrush(dc, tRGB);
			drawPolygonText(dc, tRc, tmpS, m_cellR[ii].alignC, m_cellR[ii].type);
			clRect.right--;
			if (ii != m_totRows - 1 && !(m_form->attr2 & GO_XHLINE))
			{
				setPen(dc, m_bRGB, m_slit);
				dc->MoveTo(m_pRc.left+dd, (int)dy-dd);
				dc->LineTo(clRect.right+1,  (int)dy-dd);
			}
		}
	
		if (dx != m_pRc.right && dx != 0 && !(m_form->attr2 & GO_XVLINE))
		{
			setPen(dc, m_bRGB, m_slit);
			dc->MoveTo(dx, m_pRc.top + m_slit);
			dc->LineTo(dx, m_pRc.bottom - m_slit);
		}
	}

	setPen(dc, m_bRGB, m_slit);
	dc->SelectStockObject(NULL_BRUSH);
	dd = 0;
	if (m_slit > 1)
		dd = m_slit / 2;
//	dc->Rectangle(m_pRc.left+dd, m_pRc.top+dd, m_pRc.right-dd, m_pRc.bottom-dd);
	dc->MoveTo(m_pRc.left+dd, m_pRc.top+dd);
	dc->LineTo(m_pRc.right-dd, m_pRc.top+dd);
	dc->MoveTo(m_pRc.left+dd, m_pRc.bottom-m_slit);
	dc->LineTo(m_pRc.right-dd, m_pRc.bottom-m_slit);

	if (m_focus)
	{
		setPen(dc, getRGB(cBLACK));
		dc->SelectStockObject(NULL_BRUSH);
		dc->Rectangle(m_cell);
	}
	m_calc = false;
	dc->RestoreDC(ndc);
}

void CfmTable::OnLButton(bool down, CPoint pt, int& result)
{
	result = RC_NOP;
	if (m_totRows <= 0)
		return;

	if (down)
	{
		m_axform->m_cursor = pt;
		if (m_select || m_move == mvSel)
		{
			m_select = false;
			m_move   = mvNone;
			invalidateRect(&m_pRc, false);
		}
	}
	else
	{
		if (m_move == mvSel)
			m_select = true;
		else
		{
			m_select = false;
			m_move   = mvNone;
		}
	}

	switch (m_move)
	{
	case mvMove:
		if (down)
		{
			m_move  = mvDrag;
			m_point = pt;
			m_delta = m_moveRc.left - pt.x;
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));

			CDC*	dc  = getDC();
			dc->SelectStockObject(NULL_PEN);
			dc->SelectStockObject(DKGRAY_BRUSH);
			dc->SetROP2(R2_NOT);
			dc->Rectangle(m_moveRc);
			releaseDC(dc);
		}
		return;

	case mvDrag:
		if (!down)
		{
			m_move = mvNone;
			CDC*	dc  = getDC();
			dc->SelectStockObject(NULL_PEN);
			dc->SelectStockObject(DKGRAY_BRUSH);
			dc->SetROP2(R2_NOT);
			dc->Rectangle(m_moveRc);
			releaseDC(dc);

			m_delta = pt.x - m_point.x;
			Cpair* pair = m_pairs.GetAt(0);
			if (m_delta >= 0 || (m_delta < 0 && abs(m_delta) < pair->m_width))
				pair->m_width += m_delta;
			else
				pair->m_width = 5;
			pair->m_widthx = (WORD)(pair->m_width / m_hR);
			for (int ii = 1; ii < m_totRows; ii++)
			{
				pair = m_pairs.GetAt(ii);
				pair->m_width  = m_pairs.GetAt(0)->m_width;
				pair->m_widthx = m_pairs.GetAt(0)->m_widthx;
			}

			invalidateRect(&m_pRc, false);
		}
		return;

	case mvNone:
	default:
		break;
	}
	
	if (down)
	{
		CRect	hRc,  tRc;
		float	dy;

		m_click = ckNone;
		if (m_curpos.m_row > 0)
		{
			tRc = m_pRc;
			tRc.top    = m_pairs.GetAt(m_curpos.m_row-1)->m_rect.top;
			tRc.bottom = m_pairs.GetAt(m_curpos.m_row-1)->m_rect.bottom+m_slit;
			invalidateRect(tRc, false);
		}

		hRc = tRc = m_pRc;
		dy = (float)(tRc.top + m_slit);
		if (!(m_form->attr2 & GO_XHEAD) && m_totRows > 0)
		{
			hRc.right = hRc.left + (int)m_pairs.GetAt(0)->m_width;
			tRc.left  = hRc.right;
		}

		for (int ii = 0, count = 0; count < m_showRows; ii++, count++)
		{
			if (!(m_cellR[ii].properties & PR_VISIBLE))
			{
				count--;
				continue;
			}

			hRc.top = tRc.top = (int)dy;
			dy += (m_cy+m_slit);
			hRc.bottom = tRc.bottom = (int)dy;
			if (!(m_form->attr2 & GO_XHEAD) && hRc.PtInRect(pt))
			{
				m_index = ii;
				m_click = ckHead;
				result  = RC_HEAD;
				m_cellpos.m_col = 1;
				m_cellpos.m_row = m_index + 1;
				m_curpos = m_lastpos = m_cellpos;
				hRc.InflateRect(2, 2);
				invalidateRect(hRc, false);
				if (m_move == mvHead && m_movepos.m_row == ii && m_cellR[ii].hcursor >= CS_CURSOR)
					m_axform->DoSomething(doCURSOR, this, m_cellR[ii].hcursor);
				break;
			}

			if (tRc.PtInRect(pt))
			{
				m_index = ii;
				m_click = ckItem;
				result  = RC_ITEM;
				m_cellpos.m_col = (m_form->attr2 & GO_XHEAD) ? 1 : 2;
				m_cellpos.m_row = m_index + 1;
				m_curpos = m_lastpos = m_cellpos;
				tRc.InflateRect(2, 2);
				invalidateRect(tRc, false);
				if (m_move == mvItem && m_movepos.m_row == ii && m_cellR[ii].cursor >= CS_CURSOR)
					m_axform->DoSomething(doCURSOR, this, m_cellR[ii].cursor);
				break;
			}
		}
	}
	else
		m_click = ckNone;
}

void CfmTable::OnRButton(bool down, CPoint pt, int& result)
{
//	OnLButton(down, pt, result);
}

void CfmTable::OnDblClick(CPoint pt, int& result)
{
	result = RC_NOP;
	m_select = false;
	m_move   = mvNone;

	if (m_pRc.PtInRect(pt) && m_totRows > 0)
	{
		CRect	tRc, hRc;

		tRc = hRc = m_pRc;
		if (!(m_form->attr2 & GO_XHEAD))
		{
			hRc.right = hRc.left + (int)m_pairs.GetAt(0)->m_width;
			tRc.left  = hRc.right;
		}
		else
			hRc.SetRectEmpty();

		if (hRc.PtInRect(pt))
			result = RC_HEAD;
		else if (tRc.PtInRect(pt))
			result = RC_ITEM;
	}
}

int CfmTable::OnDrag(CPoint pt, int& result)
{
	int	gap, width;
	CRect	tRc;

	if (m_click == ckNone)
	{
		Cpos	pos;
		int	move, rc = CfmBase::OnDrag(pt, result);

		move = m_move;
		m_move = mvNone;
		pos = m_movepos;
		m_movepos = Cpos(-1, -1);
		if (result == RC_IN)
		{
			Cpair*	pair;
			CRect	cRc;

			tRc = m_pRc;
			for (int ii = 0, count = 0; count < m_showRows; ii++)
			{
				if (!(m_cellR[ii].properties & PR_VISIBLE))
					continue;

				pair = m_pairs.GetAt(ii);
				tRc.top    = pair->m_rect.top;
				tRc.bottom = pair->m_rect.bottom;
				if (tRc.PtInRect(pt))
				{
					m_movepos.m_row = ii;
					cRc = tRc;
					if (m_show == shBoth)
						cRc.left += (int)pair->m_width;
					if (cRc.PtInRect(pt))
					{
						m_move = mvItem;
						m_movepos.m_col = (m_show == shBoth) ? 1 : 0;
						if (m_cellR[ii].cursor == CS_POP && !pos.Equal(m_movepos))
						{
							cRc.InflateRect(0, 1);
							invalidateRect(&cRc, false);
						}
						else if (m_cellR[ii].cursor >= CS_CURSOR)
							m_axform->DoSomething(doCURSOR, this, m_cellR[ii].cursor);
					}
					else
					{
						m_move = mvHead;
						if (m_cellR[ii].hcursor == CS_POP && !pos.Equal(m_movepos))
						{
							cRc.right = cRc.left;
							cRc.left  = tRc.left;
							cRc.InflateRect(0, 1);
							invalidateRect(&cRc, false);
						}
						else if (m_cellR[ii].hcursor >= CS_CURSOR)
							m_axform->DoSomething(doCURSOR, this, m_cellR[ii].hcursor);
					}
					break;
				}
				count++;
			}

			if (!pos.IsEmpty() && (move != m_move || !pos.Equal(m_movepos)))
			{
				pair = m_pairs.GetAt(pos.m_row);
				tRc = m_pRc;
				tRc.top    = pair->m_rect.top;
				tRc.bottom = pair->m_rect.bottom;
				if (move == mvHead && m_cellR[pos.m_row].hcursor == CS_POP)
				{
					if (m_show == shBoth)
						tRc.right = tRc.left + (int)pair->m_width;
					tRc.InflateRect(0, 1);
					invalidateRect(&tRc, false);
				}
				else if (m_move == mvItem && m_cellR[pos.m_row].cursor == CS_POP)
				{
					if (m_show == shBoth)
						tRc.left += (int)pair->m_width;
					tRc.InflateRect(0, 1);
					invalidateRect(&tRc, false);
				}
			}
		}
		else
		{
			if (!pos.IsEmpty() && move != m_move)
			{
				Cpair*	pair;

				pair = m_pairs.GetAt(pos.m_row);
				tRc = m_pRc;
				tRc.top    = pair->m_rect.top;
				tRc.bottom = pair->m_rect.bottom;
				if (move == mvHead && m_cellR[pos.m_row].hcursor == CS_POP)
				{
					if (m_show == shBoth)
						tRc.right = tRc.left + (int)pair->m_width;
					tRc.InflateRect(0, 1);
					invalidateRect(&tRc, false);
				}
				else if (m_move == mvItem && m_cellR[pos.m_row].cursor == CS_POP)
				{
					if (m_show == shBoth)
						tRc.left += (int)pair->m_width;
					tRc.InflateRect(0, 1);
					invalidateRect(&tRc, false);
				}
			}
			m_axform->DoSomething(doCURSOR, this, CS_NONE);
		}

		return rc;
	}

	switch (m_move)
	{
	case mvDrag:
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));

		CDC*	dc;
		dc = getDC();
		dc->SelectStockObject(NULL_PEN);
		dc->SelectStockObject(DKGRAY_BRUSH);
		dc->SetROP2(R2_NOT);
		dc->Rectangle(m_moveRc);

		gap = m_moveRc.Width();
		m_moveRc.left  = pt.x + m_delta;
		m_moveRc.right = m_moveRc.left + gap;
		dc->Rectangle(m_moveRc);
		releaseDC(dc);
		break;

	case mvMove:
		break;		// not move

		m_move = mvNone;
		if (m_show != shBoth || !m_totRows)
			break;

		tRc   = m_pRc;
		width = tRc.left;
		tRc.left += (int)m_pairs.GetAt(0)->m_width;
		tRc.right = tRc.left + m_slit;
		if (m_slit < 3)
			tRc.InflateRect(1, 0);
		if (tRc.PtInRect(pt))
		{
			m_move   = mvMove;
			m_moveRc = tRc;
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		}
		break;

	case mvNone:
	default:
		if (m_click == ckItem || m_click == ckHead)
		{
			int	row = 0, col = 0;
			CRect	hRc(0,0,0,0), vRc(0,0,0,0);

			if (findCell(pt, row, col, hRc, vRc))
			{
				if (m_lastpos.m_row != row || m_lastpos.m_col != col)
				{
					m_lastpos.m_row = row;
					m_lastpos.m_col = col;
					if (m_lastpos.m_row != m_curpos.m_row || m_lastpos.m_col != m_curpos.m_col)
						m_move = mvSel;

				//	invalidateRect(&m_pRc, false);
					if (!hRc.IsRectEmpty())
						invalidateRect(&hRc, false);
					if (!vRc.IsRectEmpty())
						invalidateRect(&vRc, false);
				}
			}
			return -1;
		}
		break;
	}

	return -1;
}

void CfmTable::SetFgColor(int rgb, int col, int row)
{
	if (row >= m_totRows)
		return;

	if (row == -1)
	{
		if (m_cellpos.m_row < 1)
			return;

		row = m_cellpos.m_row - 1;
	}

	Cpair*	pair = m_pairs.GetAt(row);
	if (rgb < 0)
	{
		if (pair->m_itRGB < 0)
			return;
		rgb = pair->m_itRGB;
	}
	DWORD color = (rgb & userCOLOR) ? rgb : getRGB(rgb);
	if (color == pair->m_tRGB && ((rgb & userCOLOR) || (!(rgb & userCOLOR) && rgb == pair->m_itRGB)))
		return;

	pair->m_itRGB = (rgb& userCOLOR) ? -1 : rgb;
	pair->m_tRGB  = color;
	if ((m_form->properties & PR_VISIBLE) && (m_cellR[row].properties & PR_VISIBLE))
	{
		if (m_pRc.Height() ==0 || m_pRc.Width() == 0 || m_show == shHead)
			return;

		CRect	tRc = pair->m_rect;
		if (m_show != shBoth)
			tRc.left++;
		tRc.bottom = min(tRc.bottom, m_pRc.bottom-m_slit);
		invalidateRect(&tRc, false);
	}
}

void CfmTable::SetBkColor(int rgb, int col, int row)
{
	bool	data = true;

	if (row >= m_totRows || (m_cellR[row].attr & FA_CONTRAST && (m_cellR[row].attr2 & SA_CONTRAST) == SA_PAINT))
		return;

	if (row == -1)
	{
		if (m_cellpos.m_row < 1)
			return;

		row = m_cellpos.m_row - 1;
	}

	Cpair* pair = m_pairs.GetAt(row);
	DWORD	color = (rgb & userCOLOR) ? rgb : getRGB(rgb);
	if (col < 0)
	{
		data = false;
		if (color == pair->m_hpRGB)
			return;

		pair->m_hpRGB = color;
	}
	else
	{
		if (color == pair->m_pRGB)
			return;

		pair->m_pRGB = color;
	}

	if ((m_form->properties & PR_VISIBLE) && (m_cellR[row].properties & PR_VISIBLE))
	{
		if (m_pRc.Height() ==0 || m_pRc.Width() == 0 || (data && m_show == shHead) || (!data && (m_show == shItem)))
			return;

		CRect	tRc = pair->m_rect;
		if (data)
		{
			if (m_show != shBoth)
				tRc.left++;
		}
		else
			tRc.right = tRc.left + (int)pair->m_width;
		tRc.bottom = min(tRc.bottom, m_pRc.bottom-m_slit);
		invalidateRect(&tRc, false);
	}
}

int CfmTable::GetFgColor(int col, int row)
{
	if (row >= m_totRows)
		return -1;

	if (row == -1)
	{
		if (m_cellpos.m_row < 1)
			return -1;

		row = m_cellpos.m_row - 1;
	}

	return m_pairs.GetAt(row)->m_tRGB;
}

int CfmTable::GetBkColor(int col, int row)
{
	if (row >= m_totRows)
		return -1;

	if (row == -1)
	{
		if (m_cellpos.m_row < 1)
			return -1;

		row = m_cellpos.m_row - 1;
	}

	return m_pairs.GetAt(row)->m_pRGB;
}

void CfmTable::SetDomino(bool redraw)
{
	DWORD	rgb;

	for (int ii = 0; ii < m_totRows; ii++)
	{
		if (!(m_cellR[ii].attr & FA_DOMINO))
			continue;

		if (m_cellR[ii].keys == NOVALUE)
			continue;

		if (m_cellR[ii].size <= 0)
			continue;

		if (m_cellR[ii].keys < 0 || (int)(m_cellR[ii].keys) >= m_totRows)
			continue;

		rgb = GetFgColor(-1, m_cellR[ii].keys);
		if (redraw)
			SetFgColor(rgb, -1, ii);
		else
			m_pairs.GetAt(ii)->m_tRGB = rgb;
		m_pairs.GetAt(ii)->m_itRGB = m_pairs.GetAt(m_cellR[ii].keys)->m_itRGB;
	}
}

void CfmTable::ReadAll(CString& data, bool edit, bool head)
{
	data = _T("");
	if (m_totRows <= 0)
		return;

	for (int ii = 0; ii < m_totRows; ii++)
	{
		if (!(m_cellR[ii].properties & PR_VISIBLE))
			continue;

		if (head)
		{
			data += m_pairs.GetAt(ii)->m_head;
			data += '\t';
		}
		data += (edit ? m_edit.GetAt(ii) : m_data.GetAt(ii));
		data += '\n';
	}
}

void CfmTable::ReadData(CString& data, bool edit, int size, int col, int row)
{
	data = _T("");
	if (row >= m_totRows)
		return;

	if (col == -2)		// head
	{
		if (row == -1)
			row  = m_cellpos.m_row-1;
		if (row >= 0)
			data = m_pairs.GetAt(row)->m_head;
		return;
	}

	if (col == -3)		// tooltip
	{
	}

	if (row == -1)
	{
		if (m_cellpos.m_row < 1)
			return;

		row = m_cellpos.m_row - 1;
	}

	int	len,  flen;
	CString	tmpS = _T("");
	if (edit)
	{
		tmpS = m_edit.GetAt(row);
		len  = tmpS.GetLength();
		flen = m_cellR[row].size;
		if (m_cellR[row].attr & FA_COMMA)
			flen = flen + (flen - 1)/3;

		flen = max(flen, len);
		if (m_cellR[row].attr & FA_RIGHT)
		{
			data  = CString(' ' , flen - len);
			data += tmpS;
		}
		else
		{
			data  = tmpS;
			data += CString(' ', flen - len);
		}
		return;
	}

	tmpS = m_data.GetAt(row);
	len  = tmpS.GetLength();
	flen = max(m_cellR[row].size, len);

	if (m_cellR[row].attr & FA_RIGHT)
	{
		data  = CString(' ', flen - len);
		data += tmpS;
	}
	else
	{
		data  = tmpS;
		data += CString(' ', flen - len);
	}
}

void CfmTable::WriteAll(CString str, bool redraw)
{
	CString	tmpS = _T("");
	int	offs = 0;

	for (int ii = 0 ; ii < m_totRows; ii++)
	{
		if (m_cellR[ii].attr & FA_SKIP)
			continue;

		if (str.IsEmpty())
			break;

		offs = str.Find('\t');
		if (offs != -1)
		{
			tmpS = str.Left(offs);
			if (str.GetLength() > offs+1)
				str = str.Mid(offs+1);
			else
				str = _T("");
		}
		else
		{
			tmpS = str;
			str = _T("");
		}
		WriteData(tmpS, false, -1, ii);
	}
	SetDomino(false);

	CRect	tRc = m_pRc;
	if (!(m_form->attr2 & GO_XHEAD) && m_totRows > 0)
		tRc.left += (int)m_pairs.GetAt(0)->m_width;
	else
		tRc.left++;
	tRc.top++;
	tRc.right--;
	tRc.bottom--;
	if (redraw)
		invalidateRect(&tRc, false);
}

void CfmTable::WriteData(CString data, bool redraw, int col, int row)
{
	if (row >= m_totRows)
		return;

	if (col == -2)
	{
		if (row == -1)
			row  = m_cellpos.m_row-1;
		if (row >= 0)
		{
			m_pairs.GetAt(row)->m_head = data;
			if (redraw && (m_form->properties & PR_VISIBLE) && (m_cellR[row].properties & PR_VISIBLE))
			{
				if (m_pRc.Height() ==0 || m_pRc.Width() == 0 || m_show == shHead)
					return;

				CRect	tRc = m_pairs.GetAt(row)->m_rect;
				if (m_show != shBoth)
					tRc.left++;
				tRc.bottom = min(tRc.bottom, m_pRc.bottom-m_slit);
				invalidateRect(&tRc, false);
			}
		}
		return;
	}

	if (row == -1)
	{
		if (m_cellpos.m_row < 1)
			return;

		row = m_cellpos.m_row -1;
	}

	if (m_cellR[row].isEdit == IE_CORGB)
		doDataEditCORGB(row, data);
	makeData(data, m_cellR[row].size, m_cellR[row].attr, m_cellR[row].attr2, m_cellR[row].type);

	CString	tmps = _T("");
	for (int ii = 0; ii < data.GetLength(); ii++)
		tmps += data[ii] ? data[ii] : ' ';

	if (m_cellR[row].attr & FA_FLOAT)
	{
		tmps.TrimLeft();
		tmps.TrimRight();

		CString	text, str;
		int	pos;
		char	sign   = '\0';

		pos = tmps.FindOneOf("+-");
		if (pos >= 0)
		{
			sign = tmps.GetAt(pos);
			tmps = tmps.Mid(pos+1);
		}

		pos = tmps.Find('.');
		if (pos >= 0)
		{
			text = tmps.Left(pos);
			if (m_cellR[row].vals[1] == m_cellR[row].size)
				tmps = text;
			else
			{
				str  = text.Right(m_cellR[row].size - m_cellR[row].vals[1]);
				text = tmps.Mid(pos+1);
				text = text.Left(m_cellR[row].vals[1]);
				if (str.IsEmpty())
					str += _T("0");
				text += CString('0', m_cellR[row].vals[1] - text.GetLength());
				str  += text;
				tmps = str;
			}
		}
		else if (!(tmps.IsEmpty() && (m_cellR[row].attr & FA_ZEROSUP)))
		{
			if (m_cellR[row].attr & FA_RIGHT)
				tmps = tmps.Right(m_cellR[row].size);
			else
				tmps = tmps.Left(m_cellR[row].size);

			pos = tmps.GetLength() - m_cellR[row].vals[1];
			if (pos <= 0)
				tmps = CString('0', abs(pos)+1) + tmps;
		}

		if (sign)
			tmps.Insert(0, sign);
	}
	else
	{
		if (m_cellR[row].attr & FA_RIGHT || m_cellR[row].alignC == AL_RIGHT)
			tmps = tmps.Right(m_cellR[row].size);
		else
			tmps = tmps.Left(m_cellR[row].size);
	}

	m_data.SetAt(row, tmps);
	makeSignData(tmps, row, col);
	CString	tmpE = editData(tmps, row);
	m_edit.SetAt(row, tmpE);

	if (redraw && (m_form->properties & PR_VISIBLE) && (m_cellR[row].properties & PR_VISIBLE))
	{
		if (m_pRc.Height() == 0 || m_pRc.Width() == 0 || m_show == shHead)
			return;

		CRect	tRc = m_pairs.GetAt(row)->m_rect;
		if (m_show != shBoth)
			tRc.left++;
		tRc.bottom = min(tRc.bottom, m_pRc.bottom-m_slit);
		invalidateRect(&tRc, false);
	}
}

void CfmTable::ClearData(bool selectOnly)
{
	for (int ii = 0; ii < m_totRows; ii++)
	{
		CString tmps = CString(' ', m_cellR[ii].size);
		m_data.SetAt(ii, tmps);
		m_edit.SetAt(ii, tmps);
	}

	m_index = 0;
	CRect	tRc = m_pRc;
	if (!(m_form->attr2 & GO_XHEAD) && m_totRows > 0)
		tRc.left += (int)m_pairs.GetAt(0)->m_width;
	else
		tRc.left++;
	tRc.top++;
	tRc.right--;
	tRc.bottom--;
	invalidateRect(&tRc, false);
}

void CfmTable::GetEnum(CString& text, int& value, int func)
{
	if (func == 109)
	{
		value = -1;
		for (int ii = 0; ii < m_totRows; ii++)
		{
			if (!text.CompareNoCase((char *)m_cellR[ii].name))
			{
				value = ii;
				break;
			}
		}
	}
}

void CfmTable::SetEnum(CString text, int value, int func)
{
	if (func == 98)	// set border color
		m_bRGB = value;
}

void CfmTable::SetFocus(bool focus)
{
	if (m_hasfocus && m_focus != focus)
	{
		m_focus = focus;
		if (!m_focus)
		{
			m_select = false;
			m_move   = mvNone;
			invalidateRect(&m_pRc, false);
		}
	}
}

void CfmTable::SetCellRow(int row)
{
	if (row <= m_totRows)
		m_cellpos.m_row = row;
}

int CfmTable::GetCellRow()
{
	return m_cellpos.m_row;
}

int CfmTable::GetValidRows()
{
	return m_totRows;
}

void CfmTable::SetCurSel(int index, bool redraw)
{
	if (index < 0 || index >= m_totRows)
		return;

	if (m_index != index)
	{
		m_index = index;
		invalidateRect(&m_pRc, false);
	}
}

int CfmTable::GetCurSel()
{
	return m_index;
}

int CfmTable::GetSize()
{
	return m_totRows;
}

CString CfmTable::editData(CString str, int index)
{
	CString tmpS, tmpS_;
	int	ii,  len,  dcnt = 0;
	Cpair*	pair;

	pair = m_pairs.GetAt(index);
	tmpS = str;
	if (m_cellR[index].attr & FA_RIGHT)
		tmpS.TrimRight();

	if (!pair->m_edit.IsEmpty())
	{
		tmpS.Empty();
		tmpS_ = str;

		str.Trim();
		len = str.GetLength();
		if (len == 0)
			return tmpS;

		CString	format;
		if (m_cellR[index].attr & FA_RIGHT)
		{
			format.Empty();
			for (ii = pair->m_edit.GetLength()-1; ii >= 0; ii--)
			{
				if (pair->m_edit[ii] == '9')
					dcnt++;

				if (dcnt >= len)
					break;
			}
			format = pair->m_edit.Mid(ii);
		}
		else
			format = pair->m_edit;

		dcnt = 0;
		for (ii = 0; ii < format.GetLength(); ii++)
		{
			if (format[ii] == '9')
			{
				if (dcnt >= len)
					break;
				else
				{
					tmpS += str[dcnt];
					dcnt++;
				}
			}
			else
				tmpS += format[ii];
		}
		return tmpS;
	}

	if (m_cellR[index].attr & FA_PASSWD)
	{
		tmpS_ = str;
		tmpS_.TrimRight();
		tmpS.Empty();
		for (ii = 0; ii < tmpS_.GetLength(); ii++)
			tmpS += '*';
		return tmpS;
	}

	formatData(m_cellR[index].attr, tmpS, (m_cellR[index].vals[1] == m_cellR[index].size) ? -1 : (int)m_cellR[index].vals[1], m_cellR[index].size);
	return tmpS;
}

void CfmTable::makeSignData(CString& data, int row, int col)
{
	if (!(m_cellR[row].attr & FA_CONTRAST) || data.IsEmpty())
		return;

	CString	tmps;
	int	state;

	state = m_axform->GetContrast(data[0], tmps);
	tmps += CString(' ', m_cellR[row].size-data.GetLength());
	tmps += data.Mid(1);
	data = tmps;

	int	rgb;
	DWORD	upRGB, dnRGB;
	if (m_cellR[row].onEdit != NOVALUE)
	{
		upRGB = m_axform->m_valueR[m_cellR[row].onEdit];
		dnRGB = m_axform->m_valueR[m_cellR[row].onEdit+1];
	}

	switch (state)
	{
	case ct_uLimit:
	case ct_Rise:
		rgb = (m_cellR[row].onEdit == NOVALUE) ? cRGB_UP : upRGB;	break;
	case ct_lLimit:
	case ct_Fall:
		rgb = (m_cellR[row].onEdit == NOVALUE) ? cRGB_DN : dnRGB;	break;
	case ct_Flat:
	default:
		rgb = m_cellR[row].tRGB;	break;
	}
	rgb = (rgb < 0) ? m_cellR[row].tRGB : rgb;

	if ((m_cellR[row].attr2 & SA_CORGB) == SA_COSUP)
		data = data.Mid(2);

	Cpair* pair = m_pairs.GetAt(row);
	if ((m_cellR[row].attr2 & SA_CONTRAST) == SA_PAINT)
	{
		if (state == ct_uLimit || state == ct_lLimit)
			SetFgColor(getRGB(cWHITE), -1, row);
		else
		{
			SetFgColor(rgb, -1, row);
			rgb = m_cellR[row].pRGB;
		}

		Cpair* pair = m_pairs.GetAt(row);
		DWORD	color = (rgb & userCOLOR) ? rgb : getRGB(rgb);
		if (color == pair->m_pRGB)
			return;

		pair->m_pRGB = color;
		return;
	}
	SetFgColor(rgb, -1, row);
}

void CfmTable::doDataEditCORGB(int index, CString& data)
{
	if (m_cellR[index].onEdit == NOVALUE)
		return;

	int	rgb = getCORGB(m_cellR[index].onEdit, data, m_cellR[index].size, m_cellR[index].attr2 & SA_CORGB);
	rgb = (rgb < 0) ? m_cellR[index].tRGB : rgb;
	if (rgb & userCOLOR)
	{
		m_pairs.GetAt(index)->m_tRGB  = rgb;
		m_pairs.GetAt(index)->m_itRGB = -1;
	}
	else
	{
		m_pairs.GetAt(index)->m_tRGB  = getRGB(rgb);
		m_pairs.GetAt(index)->m_itRGB = rgb;
	}
}

int CfmTable::keyArrowLeft(int& result)
{
	int	pos = 0;
	CRect	pRc;
	Cpos	lastpos = m_lastpos;

	if (m_show != shBoth)
		return 0;

	pRc.SetRectEmpty();
	if (result == RC_SHIFT)
		pos = m_lastpos.m_col;
	else
	{
		if (m_select)
			pRc = m_pRc;
		m_select = false;
		m_move   = mvNone;
		pos = m_curpos.m_col;
	}

	if (pos-1 > 0)
		pos--;

	if (result == RC_SHIFT)
	{
		if (m_lastpos.m_col != pos)
		{
			m_select = true;
			m_lastpos.m_col = pos;
			refreshSel(lastpos, VK_LEFT);
		}
		return 0;
	}
	else
	{
		if (m_curpos.m_col == pos)
		{
			m_lastpos = m_curpos;
			if (!pRc.IsRectEmpty())
				invalidateRect(pRc, false);
			return 0;
		}
		m_curpos.m_col  = pos;
		m_lastpos = m_curpos;
	}

	CRect	tRc = m_pRc;
	tRc.top    = m_pairs.GetAt(m_curpos.m_row-1)->m_rect.top;
	tRc.bottom = min(m_pairs.GetAt(m_curpos.m_row-1)->m_rect.bottom+m_slit, m_pRc.bottom-m_slit);
	pRc |= tRc;
	invalidateRect(&pRc, false);
	return 0;
}

void CfmTable::keyArrowRight(int& result)
{
	int	pos = 0;
	CRect	pRc;
	Cpos	lastpos = m_lastpos;

	if (m_show != shBoth)
		return;

	pRc.SetRectEmpty();
	if (result == RC_SHIFT)
		pos = m_lastpos.m_col;
	else
	{
		if (m_select)
			pRc = m_pRc;
		m_select = false;
		m_move   = mvNone;
		pos = m_curpos.m_col;
	}

	if (pos-1 <= 0)
		pos++;

	if (result == RC_SHIFT)
	{
		if (m_lastpos.m_col != pos)
		{
			m_select = true;
			m_lastpos.m_col = pos;
			refreshSel(lastpos, VK_RIGHT);
		}
		return;
	}
	else
	{
		if (m_curpos.m_col == pos)
		{
			m_lastpos = m_curpos;
			if (!pRc.IsRectEmpty())
				invalidateRect(pRc, false);
			return;
		}
		m_curpos.m_col  = pos;
		m_lastpos = m_curpos;
	}

	CRect	tRc = m_pRc;
	tRc.top    = m_pairs.GetAt(m_curpos.m_row-1)->m_rect.top;
	tRc.bottom = min(m_pairs.GetAt(m_curpos.m_row-1)->m_rect.bottom+m_slit, m_pRc.bottom-m_slit);
	pRc |= tRc;
	invalidateRect(&pRc, false);
}

void CfmTable::keyArrowUp(int& result)
{
	int	pos = 0;
	CRect	pRc;
	Cpos	lastpos = m_lastpos;

	if (m_totRows <= 1 || m_showRows <= 1)
		return;

	pRc.SetRectEmpty();
	if (result == RC_SHIFT)
		pos = m_lastpos.m_row;
	else
	{
		if (m_select)
			pRc = m_pRc;
		m_select = false;
		m_move   = mvNone;
		pos = m_curpos.m_row;
	}

	while (pos-1 > 0)
	{
		pos--;
		if (m_cellR[pos-1].properties & PR_VISIBLE)
			break;
	}

	if (result == RC_SHIFT)
	{
		if (m_lastpos.m_row != pos)
		{
			m_select = true;
			m_lastpos.m_row = pos;
			refreshSel(lastpos, VK_UP);
		}
		return;
	}
	else
	{
		if (m_curpos.m_row == pos)
		{
			m_lastpos = m_curpos;
			if (!pRc.IsRectEmpty())
				invalidateRect(pRc, false);
			return;
		}
		m_curpos.m_row = pos;
		m_lastpos = m_curpos;
	}

	CRect	tRc = m_pRc;
	if (!(m_form->attr2 & GO_XHEAD))
	{
		if (m_curpos.m_col == 1)
			tRc.right = tRc.left + (int)m_pairs.GetAt(0)->m_width + m_slit;
		else
			tRc.left += (int)m_pairs.GetAt(0)->m_width;
	}
	else
		tRc.left++;
	tRc.InflateRect(0, 1, -1, -1);
	pRc |= tRc;
	invalidateRect(&pRc, false);
}

void CfmTable::keyArrowDown(int& result)
{
	int	pos, ii, count;
	CRect	pRc;
	Cpos	lastpos = m_lastpos;

	if (m_totRows <= 1 || m_showRows <= 1)
		return;

	pRc.SetRectEmpty();
	if (result == RC_SHIFT)
		pos = m_lastpos.m_row;
	else
	{
		if (m_select)
			pRc = m_pRc;
		m_select = false;
		m_move   = mvNone;
		pos = m_curpos.m_row;
	}

	for (ii = 1, count = 0; ii < pos; ii++)
	{
		if (m_cellR[ii-1].properties & PR_VISIBLE)
			count++;
	}

	while (pos < m_totRows && count < m_showRows)
	{
		pos++;
		if (m_cellR[pos-1].properties & PR_VISIBLE)
			break;
	}

	if (result == RC_SHIFT)
	{
		if (m_lastpos.m_row != pos)
		{
			m_select = true;
			m_lastpos.m_row = pos;
			refreshSel(lastpos, VK_DOWN);
		}
		return;
	}
	else
	{
		if (m_curpos.m_row == pos)
		{
			m_lastpos = m_curpos;
			if (!pRc.IsRectEmpty())
				invalidateRect(pRc, false);
			return;
		}
		m_curpos.m_row = pos;
		m_lastpos = m_curpos;
	}

	CRect	tRc = m_pRc;
	if (!(m_form->attr2 & GO_XHEAD))
	{
		if (m_curpos.m_col == 1)
			tRc.right = tRc.left + (int)m_pairs.GetAt(0)->m_width + m_slit;
		else
			tRc.left += (int)m_pairs.GetAt(0)->m_width;
	}
	else
		tRc.left++;
	tRc.InflateRect(0, 1, 0, -1);
	pRc |= tRc;
	invalidateRect(&pRc, false);
}

void CfmTable::keyEscape(int& result)
{
	result   = RC_NOP;
	if (m_select)
	{
		m_select = false;
		m_move   = mvNone;

		CRect	pRc = m_pRc;
		invalidateRect(&pRc, false);
	}
}

bool CfmTable::isReverseCell(int row, int col)
{
	if (row == m_curpos.m_row-1 && col == m_curpos.m_col-1)
		return false;

	if (m_focus && (m_select || m_move == mvSel))
	{
		CRect	rect;

		rect.left   = max(m_curpos.m_col,  0);
		rect.top    = max(m_curpos.m_row,  0);
		rect.right  = max(m_lastpos.m_col, 0);
		rect.bottom = max(m_lastpos.m_row, 0);
		rect.NormalizeRect();
		rect.OffsetRect(-1, -1);

		if (col >= rect.left && col <= rect.right && row >= rect.top && row <= rect.bottom)
			return true;
	}

	return false;
}

void CfmTable::refreshSel(Cpos pos, int key)
{
	if (m_lastpos.m_row <= 0 || m_lastpos.m_col <= 0)
		return;

	switch (key)
	{
	case VK_LEFT:
		if (pos.m_col <= m_curpos.m_col)
			pos.m_col = m_lastpos.m_col;
		break;
	case VK_RIGHT:
		if (pos.m_col >= m_curpos.m_col)
			pos.m_col = m_lastpos.m_col;
		break;
	case VK_UP:
		if (pos.m_row <= m_curpos.m_row)
			pos.m_row = m_lastpos.m_row;
		break;
	case VK_DOWN:
		if (pos.m_row >= m_curpos.m_row)
			pos.m_row = m_lastpos.m_row;
		break;
	}

	CRect	pRc = m_pRc;
	if (!(m_form->attr2 & GO_XHEAD))
	{
		if (pos.m_col == 1)
			pRc.right = pRc.left + (int)m_pairs.GetAt(0)->m_width;
		else
			pRc.left += (int)m_pairs.GetAt(0)->m_width;
	}
	else
		pRc.left++;
	pRc.top    = m_pairs.GetAt(pos.m_row-1)->m_rect.top;
	pRc.bottom = min(m_pairs.GetAt(pos.m_row-1)->m_rect.bottom+m_slit, m_pRc.bottom-m_slit);

	pRc.left = min(pRc.left, m_cell.left);
	pRc.top  = min(pRc.top, m_cell.top);
	pRc.right  = max(pRc.right, m_cell.right);
	pRc.bottom = max(pRc.bottom, m_cell.bottom);
	invalidateRect(&pRc, false);
}

bool CfmTable::findCell(CPoint pt, int& row, int& col, CRect& hRc, CRect& vRc)
{
	CRect	tRc, cRc;
	Cpair*	pair;

	tRc = m_pRc;
	hRc.SetRectEmpty();
	vRc.SetRectEmpty();
	for (int ii = 0, count = 0; count < m_showRows; ii++)
	{
		if (!(m_cellR[ii].properties & PR_VISIBLE))
			continue;

		pair = m_pairs.GetAt(ii);
		tRc.top    = pair->m_rect.top;
		tRc.bottom = pair->m_rect.bottom;
		if (tRc.PtInRect(pt))
		{
			hRc = tRc;
			row = ii+1;
			cRc = pair->m_rect;
			if (m_show == shBoth)
				cRc.left += (int)pair->m_width;
			if (cRc.PtInRect(pt))
				col = (m_show == shBoth) ? 2 : 1;
			else
				col = 1;

			if (m_lastpos.m_col == col)
			{
				if (m_lastpos.m_row != row)
				{
					tRc.top    = m_pairs.GetAt(m_lastpos.m_row-1)->m_rect.top;
					tRc.bottom = m_pairs.GetAt(m_lastpos.m_row-1)->m_rect.bottom;
					hRc |= tRc;
				}
			}
			else
			{
				hRc.SetRectEmpty();
				vRc = m_pRc;
			}
			hRc.InflateRect(1, 2);
			vRc.InflateRect(1, 2);
			return true;
		}
		count++;
	}

	return false;
}
