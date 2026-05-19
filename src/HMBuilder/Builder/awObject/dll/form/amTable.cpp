// fmTable.cpp : implementation file
//

#include "stdafx.h"
#include "amTable.h"
#include "amform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CamTable

IMPLEMENT_DYNCREATE(CamTable, CamBase)

CamTable::CamTable()
{
	EnableAutomation();
}

CamTable::CamTable(CAmForm* axform, struct _formR* formR, /*struct _cellR**/vector <struct _cellR*>* cellR)
	: CamBase(axform, formR)
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
	m_totRows = m_form->row;	//LOWORD(m_form->vals[1]);
//	m_cellH	  = m_form->keys;


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
	CString	tmps;

//	m_form->isHint = IH_NOP;
	m_names.RemoveAll();
	for (int ii = 0; ii < m_totRows; ii++)
	{
		pair = new Cpair;

		pair->m_rect = m_pRc;
		if (m_show == shHead)
			pair->m_widthx = m_pRc.Width();
		else
			pair->m_widthx = (*m_cellR)[ii]->width;

		pair->m_width = (float)pair->m_widthx;
		CString sHead = (*m_cellR)[ii]->head;

//		if ((*m_cellR)[ii]->head != NOVALUE)
		if (!sHead.IsEmpty())
			pair->m_head = sHead; //m_axform->GetString((*m_cellR)[ii]->head);
		else
			pair->m_head = _T("");

//		if ((*m_cellR)[ii]->attr & FA_FLASH)
//			flash = ii;
		CString ss = CString((*m_cellR)[ii]->name);
		//if ((*m_cellR)[ii]->name != NOVALUE)
		
		if (!ss.IsEmpty())
		{
			char*	rts;
			tmps = ss;	//m_axform->GetString((*m_cellR)[ii]->name);
//			m_names.SetAt((*m_cellR)[ii]->name, tmps);
			m_names.SetAt(ii, tmps);
			rts = (char *)tmps.operator LPCSTR();
			length = (int)strlen(rts);
			if (length >= offsRTS)
				rts += (length - offsRTS);
			pair->m_rts = atoi(rts);
			if (pair->m_rts == 0)
			{
				length = (int)strlen(rts);
				for (int ii = 0; ii < length; ii++)
				{
					if (!isdigit(*rts))
					{
						pair->m_rts = -1;
						break;
					}
				}
			}
		}
		else
			pair->m_rts = -1;

		pair->m_hpRGB = ((*m_cellR)[ii]->hpRGB & userCOLOR) ? (*m_cellR)[ii]->hpRGB : getRGB((*m_cellR)[ii]->hpRGB);	
		pair->m_htRGB = ((*m_cellR)[ii]->htRGB & userCOLOR) ? (*m_cellR)[ii]->htRGB : getRGB((*m_cellR)[ii]->htRGB);
		pair->m_pRGB  = ((*m_cellR)[ii]->pRGB  & userCOLOR) ? (*m_cellR)[ii]->pRGB  : getRGB((*m_cellR)[ii]->pRGB);
		pair->m_tRGB  = ((*m_cellR)[ii]->tRGB  & userCOLOR) ? (*m_cellR)[ii]->tRGB  : getRGB((*m_cellR)[ii]->tRGB);
		pair->m_itRGB = ((*m_cellR)[ii]->tRGB  & userCOLOR) ? -1 : (*m_cellR)[ii]->tRGB;
	
	//	if ((*m_cellR)[ii]->attr & FA_RIGHT)
	//		(*m_cellR)[ii]->alignC = AL_RIGHT;
/*
		if ((*m_cellR)[ii]->isHint != IH_NOP)
		{
			m_form->isHint = IH_STRING;
			m_form->onHint = IH_STRING;
		}
		*/
//		switch ((*m_cellR)[ii]->isEdit)
//		{
//		case IE_EDIT:
//			if ((*m_cellR)[ii]->onEdit != NOVALUE)

		string s = string((*m_cellR)[ii]->editS);
		if (!s.empty())
			pair->m_edit = (char *)(*m_cellR)[ii]->editS;	//(char *)&m_axform->m_strR[(*m_cellR)[ii]->onEdit];
//			break;
//		case IE_CORGB:
//			(*m_cellR)[ii]->isEdit = IE_NOP;
//		if ((*m_cellR)[ii]->onEdit != 0)
//			(*m_cellR)[ii]->isEdit = IE_CORGB;
//		if ((*m_cellR)[ii]->attr & FA_CORGB && (*m_cellR)[ii]->onEdit != NOVALUE)
				
//			break;
//		default:
//			break;
//		}

	
		m_pairs.Add(pair);
	}

	if (flash != -1)
	{
//		m_form->attr   |= FA_FLASH;
//		m_form->checked = flash;
	}

	m_move  = mvNone;
	m_delta = 0;
	m_moveRc.SetRectEmpty();

	m_data.RemoveAll();
	m_edit.RemoveAll();



	for (int ii = 0; ii < m_totRows; ii++)
	{
		tmps = CString(' ', (*m_cellR)[ii]->size);
		m_data.Add(tmps);
		m_edit.Add(tmps);

		if ((*m_cellR)[ii]->properties & PR_VISIBLE)
			m_showRows++;
	}

	m_showRows = m_form->row;
//	m_cellH	  = m_form->keys;
	float	value;
/*	
	if (m_cellH > 0)
	{
		value      = (float)(m_pRc.Height()-m_slit)/(float)(m_cellH+m_slit);
		m_showRows = (int)value;
		if (value - (float)m_showRows > 0.5)
			m_showRows++;
	}
	else
*/
	{
		value   = (float)(m_pRc.Height()-m_slit*(m_showRows+1))/(float)m_showRows;
		m_cellH = (int)value;
		if (value - (float)m_cellH > 0.5)
			m_cellH++;
	}
	m_cy = (float)m_cellH;
	
	m_select  = false;
	m_lastpos = Cpos(-1, -1);


}

CamTable::~CamTable()
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


void CamTable::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CamTable, CamBase)
	//{{AFX_MSG_MAP(CamTable)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CamTable, CamBase)
	//{{AFX_DISPATCH_MAP(CamTable)
	/*DISP_PROPERTY_EX(CamTable, "Text", _getText, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CamTable, "Data", _getData, _setData, VT_BSTR)
	DISP_PROPERTY_EX(CamTable, "Cell", _getCell, _setCell, VT_BSTR)
	DISP_PROPERTY_EX(CamTable, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_PROPERTY_EX(CamTable, "Row", _getRow, _setRow, VT_I4)
	DISP_PROPERTY_EX(CamTable, "Head", _getHead, _setHead, VT_BSTR)
	DISP_PROPERTY_EX(CamTable, "HitPos", _getHitPos, SetNotSupported, VT_I4)
	DISP_FUNCTION(CamTable, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CamTable, "GetTableCell", _GetCell, VT_BSTR, VTS_I4)
	DISP_FUNCTION(CamTable, "SetTableCell", _SetCell, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CamTable, "GetTableText", _GetText, VT_BSTR, VTS_I4)
	DISP_FUNCTION(CamTable, "GetPRgb", _GetPRgb, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION(CamTable, "SetPRgb", _SetPRgb, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CamTable, "GetTRgb", _GetTRgb, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION(CamTable, "SetTRgb", _SetTRgb, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CamTable, "GetHead", _GetHead, VT_BSTR, VTS_I4)
	DISP_FUNCTION(CamTable, "SetHead", _SetHead, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CamTable, "PushTableCell", _Push, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CamTable, "Clear", _Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CamTable, "Excel", _Excel, VT_EMPTY, VTS_BOOL VTS_BOOL VTS_BSTR)
	DISP_FUNCTION(CamTable, "SetSkip", _SetSkip, VT_EMPTY, VTS_BSTR VTS_BOOL)*/
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmTable to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {FC28F962-0228-436F-8659-B33D87F25690}
static const IID IID_IfmTable =
{ 0xfc28f962, 0x228, 0x436f, { 0x86, 0x59, 0xb3, 0x3d, 0x87, 0xf2, 0x56, 0x90 } };

BEGIN_INTERFACE_MAP(CamTable, CamBase)
	INTERFACE_PART(CamTable, IID_IfmTable, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CamTable message handlers

void CamTable::_Refresh() 
{
	Refresh();
}

BSTR CamTable::_GetCell(long row) 
{
	CString strResult;

	ReadData(strResult, false, -1, row-1);
	return strResult.AllocSysString();
}

void CamTable::_SetCell(long row, LPCTSTR cells) 
{
	WriteData(cells, true, -1, row-1);
}

BSTR CamTable::_GetText(long row) 
{
	CString strResult;

	ReadData(strResult, true, -1, row-1);
	return strResult.AllocSysString();
}

long CamTable::_GetPRgb(long row, long column) 
{
	return GetBkColor(column-1, row-1);
}

void CamTable::_SetPRgb(long row, long column, long rgb) 
{
	SetBkColor(rgb, column-1, row-1);
}

long CamTable::_GetTRgb(long row, long column) 
{
	return GetFgColor(column-1, row-1);
}

void CamTable::_SetTRgb(long row, long column, long rgb) 
{
	SetFgColor(rgb, column-1, row-1);
}

BSTR CamTable::_GetHead(long row) 
{
	CString strResult;

	strResult = m_pairs.GetAt(row-1)->m_head;
	return strResult.AllocSysString();
}

void CamTable::_SetHead(long row, LPCTSTR head) 
{
	if (row <= 0)
		return;

	m_pairs.GetAt(row-1)->m_head = head;
	if (m_pRc.Height() ==0 || m_pRc.Width() == 0 || m_show == shItem || !((*m_cellR)[row-1]->properties & PR_VISIBLE))
		return;

	Cpair*	pair = m_pairs.GetAt(row-1);
	CRect	tRc  = pair->m_rect;
	if (m_show == shBoth)
		tRc.right = tRc.left + (int)pair->m_width;
	tRc.bottom = min(tRc.bottom, m_pRc.bottom-m_slit);
	invalidateRect(&tRc, false);
}

void CamTable::_Push(long row, LPCTSTR name) 
{
	m_axform->DoSomething(doPUSHX, this, MAKEWPARAM(row-1, 0), (LPARAM)name);
}

void CamTable::_Clear() 
{
	ClearData();
}

void CamTable::_Excel(BOOL run, BOOL head, LPCTSTR path) 
{
	m_axform->DoSomething(doEXCEL, this, MAKEWPARAM(run ? 1 : 0, head ? 1 : 0), (LPARAM)path);
}

void CamTable::_SetSkip(LPCTSTR cname, BOOL skip) 
{
	CString	name = CString(cname);
	for (int ii = 0; ii < m_totRows; ii++)
	{
//		if (!name.CompareNoCase(GetSymbolName((*m_cellR)[ii]->name)))
		if (!name.CompareNoCase(GetSymbolName(ii)))
		{
			if (skip)
				(*m_cellR)[ii]->attr |= FA_SKIP;
			else
				(*m_cellR)[ii]->attr &= ~FA_SKIP;
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////
BOOL CamTable::GetEnable() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CamTable::SetEnable(BOOL bNewValue) 
{
	CamBase::SetEnable(bNewValue ? true : false);
}

long CamTable::GetPRgb() 
{
	return m_pRGB;
}

void CamTable::SetPRgb(long nNewValue) 
{
	CamBase::SetBkColor(nNewValue);
}

long CamTable::GetTRgb() 
{
	return m_tRGB;
}

void CamTable::SetTRgb(long nNewValue) 
{
	CamBase::SetFgColor(nNewValue);
}
/////////////////////////////////////////////////////////////////////


LPCTSTR CamTable::GetText() 
{
	CString strResult;

	ReadData(strResult, true);
	return (LPCTSTR)strResult;
}

LPCTSTR CamTable::GetData() 
{
	CString strResult;

	ReadData(strResult);
	return (LPCTSTR)strResult;
}

void CamTable::SetData(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue);
}

LPCTSTR CamTable::GetCell() 
{
	CString strResult;

	ReadData(strResult);
	return (LPCTSTR)strResult;
}

void CamTable::SetCell(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue);
}

BOOL CamTable::GetVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CamTable::SetVisible(BOOL bNewValue) 
{
	CamBase::SetVisible(bNewValue ? true : false);
}

long CamTable::GetRow() 
{
	return GetCellRow();
}

void CamTable::SetRow(long nNewValue) 
{
	SetCellRow(nNewValue);
}

LPCTSTR CamTable::GetHead() 
{
	CString strResult;

	ReadData(strResult, false, -2);
	return (LPCTSTR)strResult;
}

void CamTable::SetHead(LPCTSTR lpszNewValue) 
{
	if (m_cellpos.m_row > 0 && m_cellpos.m_row <= m_totRows)
		WriteData(lpszNewValue, true, -2);
}

long CamTable::GetHitPos() 
{
	return m_axform->DoSomething(doHITPOS, this);
}

CString CamTable::GetSymbolName(int col)
{
	if (col < 0)
		return CamBase::GetSymbolName();

	CString	name;
	if (m_names.Lookup((DWORD)col, name))
		return name;

	return _T("");
}

int CamTable::GetName(int col)
{
	if (col < 0)
		return CamBase::GetName();

	Cpair* pair = m_pairs.GetAt(col);
	return pair->m_rts;
}

void CamTable::Adjust(CPoint pt, float hR, float vR)
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

	value   = (float)(m_cellH * vR);
	m_cellH = (int)value;
	if (value - (float)m_cellH > 0.5)
		m_cellH++;
	m_cy = (float)m_cellH;
	m_calc = true;
	CamBase::Adjust(pt, hR, vR);
}

void CamTable::Resize(float hR, float vR, bool font, bool redraw)
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
		if (pair->m_width - (float)value > 0.5)
			pair->m_width = (float)value + 1;
		else
			pair->m_width = (float)value;
	}

	m_cy = (float)(m_cellH * vR);
	m_calc = true;
	CamBase::Resize(hR, vR, font, redraw);
}

void CamTable::Reload()
{
	CamBase::Reload();

	Cpair*	pair;
	float	value;

	for (int ii = 0; ii < m_totRows; ii++)
	{
		pair = m_pairs.GetAt(ii);
		pair->m_rect = m_pRc;
		if (m_show == shHead)
			pair->m_widthx = m_pRc.Width();
		else
			pair->m_widthx = (*m_cellR)[ii]->width;

		pair->m_width = (float)pair->m_widthx;
	}
	m_move = mvNone;
	m_moveRc.SetRectEmpty();

	//m_cellH = LOWORD(m_form->keys);
	//m_cellH = (m_pRc.bottom - m_pRc.top) / m_form->row;	
	
	if (m_cellH > 0)
	{
		value      = (float)(m_pRc.Height()-m_slit)/(float)(m_cellH+m_slit);
		m_showRows = (int)value;
		if (value - (float)m_showRows > 0.5)
			m_showRows++;
	}
	else
	{
		value   = (float)(m_pRc.Height()-m_slit*(m_showRows+1))/(float)m_showRows;
		m_cellH = (int)value;
		if (value - (float)m_cellH > 0.5)
			m_cellH++;
	}
	m_cy = (float)m_cellH;
}

void CamTable::Draw(CDC* dc)
{
 	if (!(m_form->properties & PR_VISIBLE))
		return;

	Graphics g(dc->GetSafeHdc());
	StringFormat sf;
	CRect	clRect, tRc;
	CBrush*	brush;
	float	dy;
	CString	tmpS = _T("");
	int	dx,  dd;
	Cpair*	pair;
	COLORREF pRGB, tRGB;
	
	setPen(dc);
	
#ifdef _GDI
	CFont*	oldfont;
	CFont*	font = setFont(dc);
	oldfont = dc->SelectObject(font);
	
	int	ndc = dc->SaveDC();
	int	sMode;	
#endif


	if ((m_pRc.Height() == 0) || (m_pRc.Width() == 0))
		return;

	dd = 1;
	if (m_slit > 1)
	{
		float	fdd = (float)m_slit / (float)2;
		dd = (int)fdd;
		if (fdd - (float)dd >= 0.5)
			dd++;
	}
	
	if (m_totRows != 0 && m_showRows != 0)
	{
		dy   = (float)m_pRc.top + m_slit;

		m_cell.SetRectEmpty();
		for (int ii = 0; ii < m_totRows; ii++)
		{
			if (!((*m_cellR)[ii]->properties & PR_VISIBLE))
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
#ifdef _last
					pRGB = clBLACK;
					tRGB = getgReverseColor(tRGB);
#else
					pRGB = getRGB(cBLACK);
					tRGB = getReverseColor(tRGB);
#endif
				}
				brush = setBrush(dc, pRGB);
         		//dc->FillRect(clRect, brush);
				g.DrawRectangle(setgPen(GRGB(pRGB)), RECTG(clRect));
				g.FillRectangle(setgBrush(GRGB(pRGB)), RECTG(clRect));
				setTextColor(dc, tRGB);
				sMode = dc->SetBkMode(TRANSPARENT);

				if (!pair->m_head.IsEmpty())
				{
					sf.SetLineAlignment(StringAlignmentCenter);
					sf.SetFormatFlags(StringFormatFlagsLineLimit|StringFormatFlagsNoFitBlackBox|StringFormatFlagsNoWrap|StringFormatFlagsNoClip);

					sf.SetHotkeyPrefix(HotkeyPrefixNone);

					switch ((*m_cellR)[ii]->alignH)
					{
					case AL_CENTER:
						sf.SetAlignment(StringAlignmentCenter);	break;
					case AL_RIGHT:
						sf.SetAlignment(StringAlignmentFar);	break;
					case AL_LEFT:
					default:
						sf.SetAlignment(StringAlignmentNear);	break;
					}
					tRc = clRect;
					tRc.OffsetRect(-1, 0);
					//tRc.top = clRect.top - 20;
					//tRc.bottom = clRect.bottom + 20;
					sf.SetTrimming(StringTrimmingNone);
					

					dc->DrawText(pair->m_head, tRc, DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX|DT_CENTER);
					//g.DrawString(pair->m_head.AllocSysString(), -1, setgFont(), RECTF(tRc), &sf, setgBrush(GRGB(tRGB)));


				}
				clRect.left  = dx;
			}
			clRect.right = m_pRc.right - m_slit;

			if (clRect.left == clRect.right || m_show == shHead)
			{
				if (ii != m_totRows - 1 && !(m_form->attr2 & GO_XHLINE))
				{
//					setPen(dc, m_bRGB, m_slit);
//					dc->MoveTo(m_pRc.left+dd, (int)dy-dd);
//					dc->LineTo(clRect.right,  (int)dy-dd);

					g.DrawLine(setgPen(GRGB(m_bRGB)), Point(m_pRc.left+dd, (int)dy-dd), Point(clRect.right,  (int)dy-dd));
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
			m_tRGB = pair->m_tRGB;
			if (isReverseCell(ii, (m_show == shBoth) ? 1 : 0))
			{
#ifdef _last
				pRGB = clBLACK;
				m_tRGB = getgReverseColor(tRGB);
#else
				pRGB = getRGB(cBLACK);
				tRGB = getReverseColor(tRGB);
#endif
			}

			//brush = setBrush(dc, pRGB);
			//dc->FillRect(clRect, brush);
			g.DrawRectangle(setgPen(GRGB(pRGB)), RECTG(clRect));
			g.FillRectangle(setgBrush(GRGB(pRGB)), RECTG(clRect));

			setTextColor(dc);
			dc->SetBkMode(TRANSPARENT);

			tmpS = m_edit.GetAt(ii);
			tRc = clRect;
			tRc.left  += 3;
			tRc.right -= (int)(m_fWidth/2);
//			setPen(dc, pRGB);
//			setBrush(dc, tRGB);
			drawPolygonText(dc, tRc, tmpS, (*m_cellR)[ii]->alignC, (*m_cellR)[ii]->type);
			clRect.right--;
			if (ii != m_totRows - 1 && !(m_form->attr2 & GO_XHLINE))
			{
			//	setPen(dc, m_bRGB, m_slit);
			//	dc->MoveTo(m_pRc.left+dd, (int)dy-dd);
			//	dc->LineTo(clRect.right+1,  (int)dy-dd);
				g.DrawLine(setgPen(GRGB(m_bRGB), m_slit), Point(m_pRc.left+dd, (int)dy-dd), Point(clRect.right+1,  (int)dy-dd));
			}
		}
	
		if (dx != m_pRc.right && dx != 0 && !(m_form->attr2 & GO_XVLINE))
		{
		//	setPen(dc, m_bRGB, m_slit);
		//	dc->MoveTo(dx, m_pRc.top + m_slit);
		//	dc->LineTo(dx, m_pRc.bottom - m_slit);
			g.DrawLine(setgPen(GRGB(m_bRGB), m_slit), Point(dx, m_pRc.top + m_slit), Point(dx, m_pRc.bottom - m_slit));
		}
	}

	setPen(dc, m_bRGB, m_slit);
	dc->SelectStockObject(NULL_BRUSH);
	dd = 0;
	if (m_slit > 1)
		dd = m_slit / 2;
//	dc->Rectangle(m_pRc.left+dd, m_pRc.top+dd, m_pRc.right-dd, m_pRc.bottom-dd);
	
	CRect rc = m_pRc;
	rc.DeflateRect(dd, dd);
	g.DrawRectangle(setgPen(GRGB(m_bRGB), m_slit), RECTG(rc));

	if (m_focus)
	{
		//setPen(dc, getRGB(cBLACK));
		//dc->SelectStockObject(NULL_BRUSH);
		//dc->Rectangle(m_cell);
		g.DrawRectangle(setgPen(clBLACK), RECTG(m_cell));
	}
	m_calc = false;
	
#ifdef _GDI
	dc->SelectObject(oldfont);
	dc->SetBkMode(sMode);
	dc->RestoreDC(ndc);	
#endif
}

void CamTable::OnLButton(bool down, CPoint pt, int& result)
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
			if (!((*m_cellR)[ii]->properties & PR_VISIBLE))
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
				break;
			}
		}
	}
	else
		m_click = ckNone;
}

void CamTable::OnRButton(bool down, CPoint pt, int& result)
{
//	OnLButton(down, pt, result);
}

void CamTable::OnDblClick(CPoint pt, int& result)
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

int CamTable::OnDrag(CPoint pt, int& result)
{
	int	gap, width;
	CRect	tRc;

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
			int	row, col;
			CRect	hRc, vRc;

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
			return (-1);
		}
		break;
	}

	return (-1);
}

void CamTable::SetFgColor(int rgb, int col, int row)
{
	if (row >= m_totRows)
		return;

	if (row == -1)
	{
		if (m_cellpos.m_row < 1)
			return;

		row = m_cellpos.m_row - 1;
	}

	DWORD	color;
	Cpair*	pair = m_pairs.GetAt(row);
/*
	if (rgb < 0)
	{
		if (pair->m_itRGB < 0)
			return;
		rgb = pair->m_itRGB;
	}
*/
	color = (rgb & userCOLOR) ? rgb : getRGB(rgb);
	if (color == pair->m_tRGB && ((rgb & userCOLOR) || (!(rgb & userCOLOR) && rgb == pair->m_itRGB)))
		return;

	pair->m_itRGB = (rgb& userCOLOR) ? -1 : rgb;
	pair->m_tRGB  = color;
	if ((m_form->properties & PR_VISIBLE) && ((*m_cellR)[row]->properties & PR_VISIBLE))
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

void CamTable::SetBkColor(int rgb, int col, int row)
{
	bool	data = true;

	if (row >= m_totRows || (((*m_cellR)[row]->attr & FA_CORGB) && ((*m_cellR)[row]->attr2 & SA_CORGB) == SA_PAINT))
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

	if ((m_form->properties & PR_VISIBLE) && ((*m_cellR)[row]->properties & PR_VISIBLE))
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

int CamTable::GetFgColor(int col, int row)
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

int CamTable::GetBkColor(int col, int row)
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

void CamTable::SetDomino(bool redraw)
{
	DWORD	rgb;
	CString ss, sCell;
	int key;

	for (int ii = 0; ii < m_totRows; ii++)
	{
		if (!((*m_cellR)[ii]->attr & FA_DOMINO))
			continue;

//		if ((*m_cellR)[ii]->keys == NOVALUE)
//			continue;
		ss = CString((*m_cellR)[ii]->auxS);
	
		
		key = this->findCell(ss);


		if ((*m_cellR)[ii]->size <= 0)
			continue;

		if ((*m_cellR)[ii]->keys < 0 || (int)(key >= m_totRows))
			continue;

		rgb = GetFgColor(-1, key);
		if (redraw)
			SetFgColor(rgb, -1, ii);
		else
			m_pairs.GetAt(ii)->m_tRGB  = rgb;
		m_pairs.GetAt(ii)->m_itRGB = m_pairs.GetAt(key)->m_itRGB;
	}
}

void CamTable::ReadAll(CString& data, bool edit, bool head)
{
	data = _T("");
	if (m_totRows <= 0)
		return;

	for (int ii = 0; ii < m_totRows; ii++)
	{
		if (!((*m_cellR)[ii]->properties & PR_VISIBLE))
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

void CamTable::ReadData(CString& data, bool edit, int col, int row)
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
		flen = (*m_cellR)[row]->size;
		if ((*m_cellR)[row]->attr & FA_COMMA)
			flen = flen + (flen - 1)/3;

		flen = max(flen, len);
		/*
		if ((*m_cellR)[row]->attr & FA_RIGHT)
		{
			data  = CString(' ' , flen - len);
			data += tmpS;
		}
		else
		*/
		{
			data  = tmpS;
			data += CString(' ', flen - len);
		}
		return;
	}

	tmpS = m_data.GetAt(row);
	len  = tmpS.GetLength();
	flen = max((*m_cellR)[row]->size, len);
/*
	if ((*m_cellR)[row]->attr & FA_RIGHT)
	{
		data  = CString(' ', flen - len);
		data += tmpS;
	}
	else
	*/
	{
		data  = tmpS;
		data += CString(' ', flen - len);
	}
}

void CamTable::WriteAll(CString str, bool redraw)
{
	CString	tmpS = _T("");
	int	offs = 0;

	for (int ii = 0 ; ii < m_totRows; ii++)
	{
		if ((*m_cellR)[ii]->attr & FA_SKIP)
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

void CamTable::WriteData(CString data, bool redraw, int col, int row)
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
			if (redraw && (m_form->properties & PR_VISIBLE) && ((*m_cellR)[row]->properties & PR_VISIBLE))
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

	if ((*m_cellR)[row]->attr & FA_CORGB)
		doDataEditCORGB(row, data);
	makeData(data, (*m_cellR)[row]->size, (*m_cellR)[row]->attr, (*m_cellR)[row]->attr2, (*m_cellR)[row]->type);

	CString	tmps = _T("");
	for (int ii = 0; ii < data.GetLength(); ii++)
		tmps += data[ii] ? data[ii] : ' ';

	if ((*m_cellR)[row]->attr & FA_FLOAT)
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
			if ((*m_cellR)[row]->digit/*vals[1]*/ == (*m_cellR)[row]->size)
				tmps = text;
			else
			{
				str  = text.Right((*m_cellR)[row]->size - (*m_cellR)[row]->digit/*vals[1]*/);
				text = tmps.Mid(pos+1);
				text = text.Left((*m_cellR)[row]->digit/*vals[1]*/);
				if (str.IsEmpty())
					str += _T("0");
				str += CString('0', (*m_cellR)[row]->digit/*vals[1]*/ - text.GetLength());
				str += text;
				tmps = str;
			}
		}
		else if (!(tmps.IsEmpty() && ((*m_cellR)[row]->attr & FA_ZEROSUP)))
		{
			/*
			if ((*m_cellR)[row]->attr & FA_RIGHT)
				tmps = tmps.Right((*m_cellR)[row]->size);
			else
			*/
			tmps = tmps.Left((*m_cellR)[row]->size);

			pos = tmps.GetLength() - (*m_cellR)[row]->digit/*vals[1]*/;
			if (pos <= 0)
				tmps   = CString('0', abs(pos)+1) + tmps;
		}

		if (sign)
			tmps.Insert(0, sign);
	}
	else
	{
		if (/*(*m_cellR)[row]->attr & FA_RIGHT || */(*m_cellR)[row]->alignC == AL_RIGHT)
			tmps = tmps.Right((*m_cellR)[row]->size);
		else
			tmps = tmps.Left((*m_cellR)[row]->size);
	}

	m_data.SetAt(row, tmps);
	makeSignData(tmps, row, col);
	CString	tmpE = editData(tmps, row);
	m_edit.SetAt(row, tmpE);

	if (redraw && (m_form->properties & PR_VISIBLE) && ((*m_cellR)[row]->properties & PR_VISIBLE))
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

void CamTable::ClearData(bool selectOnly)
{
	for (int ii = 0; ii < m_totRows; ii++)
	{
		CString tmps = CString(' ', (*m_cellR)[ii]->size);
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

void CamTable::SetFocus(bool focus)
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

void CamTable::SetCellRow(int row)
{
	if (row <= m_totRows)
		m_cellpos.m_row = row;
}

int CamTable::GetCellRow()
{
	return m_cellpos.m_row;
}

int CamTable::GetValidRows()
{
	return m_totRows;
}

void CamTable::SetCurSel(int index, bool redraw)
{
	if (index < 0 || index >= m_totRows)
		return;

	if (m_index != index)
	{
		m_index = index;
		invalidateRect(&m_pRc, false);
	}
}

int CamTable::GetCurSel()
{
	return m_index;
}

int CamTable::GetSize()
{
	return m_totRows;
}

CString CamTable::editData(CString str, int index)
{
	CString tmpS, tmpS_;
	int	ii,  len,  dcnt = 0;
	Cpair*	pair;

	pair = m_pairs.GetAt(index);
	tmpS = str;
	
//	if ((*m_cellR)[index]->attr & FA_RIGHT)
//		tmpS.TrimRight();

	if (!pair->m_edit.IsEmpty())
	{
		tmpS.Empty();
		tmpS_ = str;

		str.TrimLeft();
		str.TrimRight();
		len = str.GetLength();
		if (len == 0)
			return tmpS;

		CString	format;
		/*
		if ((*m_cellR)[index]->attr & FA_RIGHT)
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
		*/
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


	if ((*m_cellR)[index]->attr & FA_PASSWD)
	{
		tmpS_ = str;
		tmpS_.TrimRight();
		tmpS.Empty();
		for (ii = 0; ii < tmpS_.GetLength(); ii++)
			tmpS += '*';
		return tmpS;
	}


	formatData((*m_cellR)[index]->attr, tmpS, ((*m_cellR)[index]->digit/*vals[1]*/ == (*m_cellR)[index]->size) ? -1 : (int)(*m_cellR)[index]->digit/*vals[1]*/);
	return tmpS;

	/*
	if ((*m_cellR)[index]->attr & FA_COMMA || (*m_cellR)[index]->attr & FA_FLOAT)
	{
		bool	zero = true;
		CString	valS = _T("");
		int	idx = 0;

		tmpS_.Empty();
		tmpS.TrimLeft();
		tmpS.TrimRight();
		if (tmpS.GetLength() == 0)
			return tmpS;

		valS = CString(' ', tmpS.GetLength());
		for (ii = 0; ii < tmpS.GetLength(); ii++)
		{
			switch (tmpS[ii])
			{
			case '1': case '2': case '3':
			case '4': case '5': case '6':
			case '7': case '8': case '9':
			case '.':
				zero = false;
				valS.SetAt(ii, 'V');
				break;

			case '0':
			case ' ':
				if (zero)
				{
					valS.SetAt(ii, 'G');
					tmpS.SetAt(ii, ' ');
				}
				else
					valS.SetAt(ii, 'V');
				break;

			default:
				valS.SetAt(ii, 'S');
				break;
			}
		}

		if (zero)
		{
			tmpS_ += '0';
			return tmpS_;
		}

		tmpS_ = tmpS;
		tmpS_.MakeReverse();
		valS.MakeReverse();
		tmpS.Empty();
		ii = 0;
		if ((*m_cellR)[index]->attr & FA_FLOAT)
		{
			for (ii = 0; ii < (int)(*m_cellR)[index]->digit/*vals[1]*///; ii++)
		/*	{
				if (ii > valS.GetLength()-1)
				{
					tmpS += '0';
					continue;
				}

				switch (valS[ii])
				{
				case 'V':
				case 'S':
					tmpS += tmpS_[ii];
					break;
				case 'G':
					tmpS += '0';
				default:
					break;
				}
			}
			tmpS += '.';
			if (ii > valS.GetLength()-1)
			{
				tmpS += '0';
				tmpS.MakeReverse();
				return tmpS;
			}
			else
			{
				if (valS[ii] == 'G')
				{
					tmpS += '0';
					idx++;
					ii++;
				}

				if (!((*m_cellR)[index]->attr & FA_COMMA))
				{
					for ( ; ii < tmpS_.GetLength(); ii++)
						tmpS += tmpS_[ii];

					tmpS.MakeReverse();
					return tmpS;
				}
				if (idx > 0)
					ii--;
			}
			if (valS.GetLength() > ii)
				valS = valS.Mid(ii);
			else
				valS = _T("");

			if (tmpS_.GetLength() > ii)
				tmpS_ = tmpS_.Mid(ii);
			else
				tmpS_ = _T("");
		}
		else
		{
			ii = tmpS_.Find('.');
			if (ii >= 0)
			{
				if (ii < tmpS_.GetLength()-1)
				{
					ii++;
					tmpS += tmpS_.Left(ii);
					if (valS[ii] == 'G')
					{
						tmpS += '0';
						idx++;
					}
				}
				else if (ii == tmpS_.GetLength() -1)
				{
					ii++;
					tmpS += tmpS_.Left(ii);
					tmpS += '0';
					idx++;
				}

				if (valS.GetLength() > ii)
					valS = valS.Mid(ii);
				else
					valS = _T("");

				if (tmpS_.GetLength() > ii)
					tmpS_ = tmpS_.Mid(ii);
				else
					tmpS_ = _T("");

				int	sign = valS.Find('S');
				CString	tmpX  = valS;
				CString tmpX_ = tmpS_;
				if (sign >= 0)
				{
					tmpX  = valS.Left(sign);
					if (valS.GetLength() > sign)
						tmpX += valS.Mid(sign);

					tmpX_  = tmpS_.Left(sign);
					if (tmpS_.GetLength() > sign)
						tmpX_ += tmpS_.Mid(sign);
				}
				else
				{
					tmpX  += ' ';
					tmpX_ += ' ';
				}
				valS = tmpX;
				tmpS_ = tmpX_;
			}
		}

		for (ii = 0;  ii < valS.GetLength(); ii++)
		{
			if (ii < idx)
				continue;

			if ((ii != 0) && ((ii % 3) == 0))
			{
				switch (valS[ii])
				{
				case 'G':
					tmpS += ' ';
					break;
				case 'V':
					tmpS += ',';
				default:
					break;
				}
			}
			tmpS += tmpS_[ii];
		}

		tmpS.MakeReverse();
	}

	return tmpS;
	*/
}

void CamTable::makeSignData(CString& data, int row, int col)
{
	if (!((*m_cellR)[row]->attr & FA_CORGB) || data.IsEmpty())
		return;

	if ( !(((*m_cellR)[row]->attr2 & SA_CORGB) & SA_PAINT) ||
	     !(((*m_cellR)[row]->attr2 & SA_CORGB) & SA_TEXT) )
		 return;

//	if (!((m_form->attr2 & SA_CORGB) & SA_PAINT) || 
		
		//!((m_form->attr2 & SA_CORGB) & SA_TEXT) )
//		return;


	CString	tmps;
	int	state;

	state = m_axform->GetContrast(data[0], tmps);
	tmps += CString(' ', (*m_cellR)[row]->size-data.GetLength());
	tmps += data.Mid(1);
	data = tmps;

	int	rgb;
	DWORD	upRGB, dnRGB;
	if ((*m_cellR)[row]->onEdit != 0)
	{
		upRGB = HIWORD((*m_cellR)[row]->onEdit);
		dnRGB = LOWORD((*m_cellR)[row]->onEdit);
	}
	else 
		upRGB = dnRGB = (*m_cellR)[row]->tRGB;

	switch (state)
	{
	case ct_uLimit:
	case ct_Rise:
		rgb = /*((*m_cellR)[row]->onEdit == 0) ? cRGB_UP :*/ upRGB;	break;
	case ct_lLimit:
	case ct_Fall:
		rgb = /*((*m_cellR)[row]->onEdit == 0) ? cRGB_DN :*/ dnRGB;	break;
	case ct_Flat:
	default:
		rgb = (*m_cellR)[row]->tRGB;	break;
	}
	rgb = (rgb < 0) ? (*m_cellR)[row]->tRGB : rgb;

//	if (((*m_cellR)[row]->attr2 & SA_CORGB) == SA_COSUP)
//		data = data.Mid(2);

	Cpair* pair = m_pairs.GetAt(row);
	if (((*m_cellR)[row]->attr2 & SA_CORGB) == SA_PAINT)
	{
		if (state != ct_Flat)
			SetFgColor(getgINDEX(clWHITE), -1, row);
		else
		{
			SetFgColor(rgb, -1, row);
			rgb = (*m_cellR)[row]->pRGB;
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

void CamTable::doDataEditCORGB(int index, CString& data)
{
//	if ((*m_cellR)[index]->onEdit == 0)
//		return;

	int	rgb = getCORGB((*m_cellR)[index]->onEdit, data, (*m_cellR)[index]->size, (*m_cellR)[index]->attr2 & SA_CORGB);
	rgb = (rgb < 0) ? (*m_cellR)[index]->tRGB : rgb;
	
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

int CamTable::keyArrowLeft(int& result)
{
	int	pos;
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

void CamTable::keyArrowRight(int& result)
{
	int	pos;
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

void CamTable::keyArrowUp(int& result)
{
	int	pos;
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
		if ((*m_cellR)[pos-1]->properties & PR_VISIBLE)
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

void CamTable::keyArrowDown(int& result)
{
	int	pos, count;
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

	count = 0;
	for (int ii = 1; ii < pos; ii++)
	{
		if ((*m_cellR)[ii-1]->properties & PR_VISIBLE)
			count++;
	}

	while (pos < m_totRows && count < m_showRows)
	{
		pos++;
		if ((*m_cellR)[pos-1]->properties & PR_VISIBLE)
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

void CamTable::keyEscape(int& result)
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

bool CamTable::isReverseCell(int row, int col)
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

void CamTable::refreshSel(Cpos pos, int key)
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

bool CamTable::findCell(CPoint pt, int& row, int& col, CRect& hRc, CRect& vRc)
{
	CRect	tRc, cRc;
	Cpair*	pair;

	tRc = m_pRc;
	hRc.SetRectEmpty();
	vRc.SetRectEmpty();
	for (int ii = 0, count = 0; count < m_showRows; ii++)
	{
		if (!((*m_cellR)[ii]->properties & PR_VISIBLE))
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

int CamTable::findCell(CString sName)
{
	CString sSymbol;
	for (int ii = 0; ii < m_showRows; ii++)
	{
		sSymbol = CString((*m_cellR)[ii]->name);
		TRIM(sSymbol);

		if (sName.CompareNoCase(sSymbol) == 0)
			return ii;

	}

	return -1;
}