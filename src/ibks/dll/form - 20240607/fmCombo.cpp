// fmCombo.cpp : implementation file
//

#include "stdafx.h"
#include "fmCombo.h"
#include "axform.h"
#include "cbList.h"
#include "image.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CfmCombo

IMPLEMENT_DYNCREATE(CfmCombo, CfmBase)

CfmCombo::CfmCombo()
{
	EnableAutomation();
}

CfmCombo::CfmCombo(CAxisForm* axform, struct _formR* formR, int iform, bool drawOnly)
	: CfmBase(axform, formR, iform)
{
	CRect	rect = m_rect;

	rect.right = rect.left;
	rect.top   = rect.bottom-6;

	m_spreaded = false;
	m_changed  = false;
	m_focus    = false;
	m_count    = LOWORD(m_form->keys);
	m_vcount   = HIWORD(m_form->keys);
	m_vcount   = min(m_count, m_vcount ? m_vcount : MAX_COMBO_LINE);
	m_index    = max(0, min(m_count-1, m_form->checked));
	m_rBtn.SetRectEmpty();
	m_click = ckNone;

	m_displays.RemoveAll();
	m_datas.RemoveAll();
	if (m_form->vals[0] == NOVALUE || m_form->vals[1] == NOVALUE)
		m_count = 0;

	m_editMode = false;
	m_caret    = false;
	m_state    = esNormal;
	m_focusing = true;
	m_caretpos = 0;
	m_data     = _T("");
	m_edit     = _T("");

	if (m_form->properties & PR_ACCN)
	{
		if (m_form->isEdit == IE_EDIT && m_form->onEdit != NOVALUE)
			m_edit = (char *)&m_axform->m_strR[m_form->onEdit];
	}

	/*
	if (m_form->isEdit == IE_CORGB)
	{
		m_form->isEdit = IE_NOP;
		if (m_form->attr & FA_CORGB && m_form->onEdit != NOVALUE)
			m_form->isEdit = IE_CORGB;
		m_form->attr2 &= ~SA_CORGB;
		m_form->attr2 |= SA_CORAW;
	}
	*/

	CString	tmps;
	char	wb[32];

	m_pic = NULL;
	tmps.Format(_T("%s\\%s\\axcombo.bmp"), m_axform->m_root.GetString(), IMAGEDIR);
	m_pic = new Cimage(m_axform->m_draw, tmps);
	m_move = mvNone;

	tmps.Format(_T("%s/%s/axis.ini"), m_axform->m_root.GetString(), TABDIR);
	DWORD	rc = GetPrivateProfileString(_T("Form"), _T("CBChange"), "", wb, sizeof(wb), tmps);
	m_cc = (atoi(CString(wb, rc)) > 0) ? true : false;

	m_list = NULL;
	if (m_count <= 0)
	{
		if (!drawOnly)
			loadCombo();
		return;
	}

	char*	data = &m_axform->m_strR[m_form->vals[0]];
	char*	disp = &m_axform->m_strR[m_form->vals[1]];

	for (int ii = 0; ii < m_count; ii++)
	{
		m_displays.Add(disp);
		disp += strlen(disp);
		disp++;
		m_datas.Add(data);
		data += strlen(data);
		data++;
	}
	m_count = min(m_count, min(m_displays.GetSize(), m_datas.GetSize()));
	clearInputData();
}

CfmCombo::~CfmCombo()
{
	if (m_list)
		delete m_list;
	m_spreaded = false;
	m_datas.RemoveAll();
	m_displays.RemoveAll();

	if (m_pic)
		delete m_pic;
}


void CfmCombo::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CfmCombo, CfmBase)
	//{{AFX_MSG_MAP(CfmCombo)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CfmCombo, CfmBase)
	//{{AFX_DISPATCH_MAP(CfmCombo)
	DISP_PROPERTY_EX(CfmCombo, "Text", _getText, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CfmCombo, "Data", _getData, _setData, VT_BSTR)
	DISP_PROPERTY_EX(CfmCombo, "PRgb", _getPRgb, _setPRgb, VT_I4)
	DISP_PROPERTY_EX(CfmCombo, "TRgb", _getTRgb, _setTRgb, VT_I4)
	DISP_PROPERTY_EX(CfmCombo, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_PROPERTY_EX(CfmCombo, "Enable", _getEnable, _setEnable, VT_BOOL)
	DISP_PROPERTY_EX(CfmCombo, "Index", _getIndex, _setIndex, VT_I4)
	DISP_PROPERTY_EX(CfmCombo, "Skip", _getSkip, _setSkip, VT_BOOL)
	DISP_PROPERTY_EX(CfmCombo, "Size", _getSize, _setSize, VT_I4)
	DISP_PROPERTY_EX(CfmCombo, "Focus", _getFocus, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX(CfmCombo, "TabStop", _getTabStop, _setTabStop, VT_BOOL)
	DISP_FUNCTION(CfmCombo, "SetFocus", _SetFocus, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmCombo, "Clear", _Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmCombo, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmCombo, "Push", _Push, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CfmCombo, "Pop", _Pop, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CfmCombo, "SetItems", _SetItems, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CfmCombo, "ShowDropDown", _ShowDropDown, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CfmCombo, "GetItemCount", _GetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION(CfmCombo, "GetDataKind", _GetDataKind, VT_I4, VTS_NONE)
	DISP_FUNCTION(CfmCombo, "GetLength", _GetLength, VT_I4, VTS_NONE)
	DISP_FUNCTION(CfmCombo, "GetItems", _GetItems, VT_BSTR, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmCombo to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {27F81344-9295-457A-B5FD-10E34D2DE25C}
static const IID IID_IfmCombo =
{ 0x27f81344, 0x9295, 0x457a, { 0xb5, 0xfd, 0x10, 0xe3, 0x4d, 0x2d, 0xe2, 0x5c } };

BEGIN_INTERFACE_MAP(CfmCombo, CfmBase)
	INTERFACE_PART(CfmCombo, IID_IfmCombo, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfmCombo message handlers

void CfmCombo::_SetFocus() 
{
	m_axform->DoSomething(doFOCUS, this);
}

void CfmCombo::_Clear() 
{
	ClearData();
	if (m_axform->DoSomething(doIGNORE, this))
		IsChanged();
}

void CfmCombo::_Refresh() 
{
	Refresh();
}

void CfmCombo::_Push(LPCTSTR name) 
{
	m_axform->DoSomething(doPUSH, this, (WPARAM)0, (LPARAM)name);
}

void CfmCombo::_Pop(LPCTSTR name) 
{
	m_axform->DoSomething(doPOP, this, (WPARAM)0, (LPARAM)name);
	if (m_axform->DoSomething(doIGNORE, this))
		IsChanged();
}

void CfmCombo::_SetItems(LPCTSTR items) 
{
	WriteAll(items);
}

void CfmCombo::_ShowDropDown(BOOL showIt) 
{
	if (showIt)
	{
		if (!m_focus)
			m_axform->DoSomething(doFOCUS, this);
		if (!m_editMode && makeListBox())
			m_spreaded = true;
	}
	else if (m_spreaded)
		DoListBox(false);
}

long CfmCombo::_GetItemCount() 
{
	return m_count;
}

long CfmCombo::_GetDataKind() 
{
	return m_form->type;
}

long CfmCombo::_GetLength() 
{
	return m_form->size;
}

BSTR CfmCombo::_GetItems() 
{
	CString strResult = _T("");

	if (m_editMode)
		return strResult.AllocSysString();

	CString	tmps;
	for (int ii = 0; ii < m_count; ii++)
	{
		tmps.Format(_T("%s%s\t"), LPCTSTR(m_datas.GetAt(ii)), LPCTSTR(m_displays.GetAt(ii)));
		strResult += tmps;
	}
	return strResult.AllocSysString();
}

BSTR CfmCombo::_getText() 
{
	CString strResult;

	ReadData(strResult, true);
	return strResult.AllocSysString();
}

BSTR CfmCombo::_getData() 
{
	CString strResult;
	
	ReadData(strResult);
	return strResult.AllocSysString();
}

void CfmCombo::_setData(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue);
	if (m_axform->DoSomething(doIGNORE, this))
		IsChanged();
}

long CfmCombo::_getPRgb() 
{
	return m_pRGB;
}

void CfmCombo::_setPRgb(long nNewValue) 
{
	CfmBase::SetBkColor(nNewValue);
}

long CfmCombo::_getTRgb() 
{
	return m_tRGB;
}

void CfmCombo::_setTRgb(long nNewValue) 
{
	CfmBase::SetFgColor(nNewValue);
}

BOOL CfmCombo::_getVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CfmCombo::_setVisible(BOOL bNewValue) 
{
	CfmBase::SetVisible(bNewValue ? true : false);
}

BOOL CfmCombo::_getEnable() 
{
	return (m_form->properties & PR_ENABLE) ? TRUE : FALSE;
}

void CfmCombo::_setEnable(BOOL bNewValue) 
{
	CfmBase::SetEnable(bNewValue ? true : false);
}

long CfmCombo::_getIndex() 
{
	return GetCurSel();
}

void CfmCombo::_setIndex(long nNewValue) 
{
	SetCurSel(nNewValue);
}

BOOL CfmCombo::_getSkip() 
{
	return (m_form->attr & FA_SKIP) ? TRUE : FALSE;
}

void CfmCombo::_setSkip(BOOL bNewValue) 
{
	if (bNewValue)
		m_form->attr |= FA_SKIP;
	else
		m_form->attr &= ~FA_SKIP;
}

long CfmCombo::_getSize() 
{
	return m_form->size;
}

void CfmCombo::_setSize(long nNewValue) 
{
	m_form->size = (WORD)nNewValue;
}

BOOL CfmCombo::_getFocus() 
{
	if (m_focus || (m_spreaded && m_list))
		return TRUE;

	return FALSE;
}

BOOL CfmCombo::_getTabStop() 
{
	return (m_form->attr & FA_PASS) ? FALSE : TRUE;
}

void CfmCombo::_setTabStop(BOOL bNewValue) 
{
	if (bNewValue)
		m_form->attr &= ~FA_PASS;
	else
		m_form->attr |= FA_PASS;
}

void CfmCombo::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE))
		return;

	if (m_editMode)
	{
		draw(dc);
		return;
	}

	UINT	bStyle = DFCS_SCROLLCOMBOBOX;
	CRect	tRc;
	CFont*	font;
	CBrush*	brush;
	int	ndc = dc->SaveDC();

	setPen(dc);
	font = setFont(dc);
	tRc = m_pRc;
//	tRc.right -= (int)(szCOMBO*m_hR);
	if (m_form->properties & PR_ENABLE)
	{
		if (m_spreaded)
		{
			brush = setBrush(dc);
			setTextColor(dc);
			setPaintColor(dc);
			if (!(m_form->properties & PR_TRANSPARENT))
				dc->FillRect(tRc, brush);
		}
		else
		{
			if (m_focus)
			{
				brush = setBrush(dc, getRGB(cDBLUE));
				setTextColor(dc, getRGB(cWHITE));
				setPaintColor(dc, getRGB(cDBLUE));
				dc->FillRect(tRc, brush);
			}
			else
			{
				brush = setBrush(dc);
				setTextColor(dc);
				setPaintColor(dc);
				if (!(m_form->properties & PR_TRANSPARENT))
					dc->FillRect(tRc, brush);
			}
		}
		if (m_click == ckCombo)
			bStyle |= DFCS_PUSHED;
	}
	else
	{
		brush = setBrush(dc, getRGB(cXBG));
		setTextColor(dc, getRGB(cXFG));
		dc->FillRect(tRc, brush);
		dc->SetBkMode(TRANSPARENT);
		bStyle |= DFCS_INACTIVE;
	}

	CString	tmpS = _T("");

	tRc.left += 4;
	tRc.right -= (int)(szCOMBO*m_hR);
	if (m_index < m_displays.GetSize())
	{
		tmpS = m_displays.GetAt(m_index);
		tRc.top += 2;
		dc->DrawText(tmpS, tRc, DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
	}
	if (m_form->borders == BD_NONE)
		drawEdge(dc, m_pRc, EDGE_SUNKEN, BF_RECT);
	else
	{
		tRc = m_pRc;
		tRc.right -= (int)(szCOMBO*m_hR);
	//	if (m_move == mvNone)
			setPen(dc, getRGB(cNORMAL));
	//	else
	//		setPen(dc, getRGB(cHOVER));
		dc->SelectStockObject(NULL_BRUSH);
		dc->Rectangle(tRc);
	}

	int	image = -1;

	if (m_click == ckCombo)
		image = PI_DOWN;
	else if (m_move == mvCombo)
		image = PI_ENTER;
	else	image = PI_DEFAULT;

	m_rBtn = m_pRc;
	if (m_pic && m_pic->GetValid(image))
	{
	//	m_rBtn.DeflateRect(0, 2, 2, 2);
		m_rBtn.left = m_rBtn.right - (int)(szCOMBO*m_hR);
		m_pic->DrawPicture(dc, m_rBtn, AL_STRETCH, m_axform->m_oldVers, image);
	}
	else
	{
	//	m_rBtn.right  -= 2;
		m_rBtn.left    = m_rBtn.right - (int)(szCOMBO*m_hR);
	//	m_rBtn.top    += 2;
	//	m_rBtn.bottom -= 2;
		dc->DrawFrameControl(m_rBtn, DFC_SCROLL, bStyle);
	}

	dc->RestoreDC(ndc);
}

void CfmCombo::OnLButton(bool down, CPoint pt, int& result)
{
	if (m_editMode)
	{
		lbutton(down, pt, result);
		return;
	}

	if (GetTickCount64() - m_elapse <= 10)
		return;

	int oldIdx = m_index;
	result  = RC_NOP;
	clearInputData();

	if (down)
		m_axform->m_cursor = pt;
	else if (!m_focus)
	{
		m_click = ckNone;
		DoListBox(false);
		m_spreaded = false;
		invalidateRect(&m_pRc, false);
		return;
	}

	switch (m_click)
	{
	case ckCombo:
	case ckOrigin:
		if (makeListBox())
		{
			m_spreaded = true;
			result = RC_CBSPREAD;
		}
	default:
		break;
	}

	if (down)
	{
		if (m_rBtn.PtInRect(pt))
			m_click = ckCombo;
		else if (m_pRc.PtInRect(pt))
			m_click = ckOrigin;
		else
		{
			m_click = ckNone;
			result  = RC_OUT;
			DoListBox(false);
			m_spreaded = false;
		}
	
		if (m_form->cursor >= CS_CURSOR)
			m_axform->DoSomething(doCURSOR, this, (m_click != ckNone) ? m_form->cursor : CS_NONE);
	}

	switch (m_click)
	{
	case ckOrigin:
		if (down && m_spreaded)
		{
			DoListBox(false);
			m_spreaded = false;
			m_click = ckNone;
			result  = RC_CBCLOSE;
		}
		invalidateRect(&m_pRc, false);
		break;

	case ckCombo:
		if (!down)
		{
			if (!m_spreaded)
				result = RC_CBCLOSE;
		}
		else if (m_spreaded)
		{
			DoListBox(false);
			m_spreaded = false;
			m_click = ckNone;
			result  = RC_CBCLOSE;
		}
		invalidateRect(&m_pRc, false);
		break;

	case ckNone:
	default:
		m_click = ckNone;
		if (!down)
			break;
		if (m_spreaded)
		{
			DoListBox(false);
			invalidateRect(&m_pRc, false);
		}
		return;
	}

	if (!down)
		m_click  = ckNone;
}

void CfmCombo::OnDblClick(CPoint pt, int& result)
{
	if (m_editMode)
	{
		result = RC_NOP;
	//	SetFocus(true);
		return;
	}

	clearInputData();
	result = RC_CBCLOSE;
	DoListBox(false);
	invalidateRect(&m_pRc, false);
}

int CfmCombo::OnDrag(CPoint pt, int& result)
{
	result = RC_NOP;
	if (m_axform->IsFormVisible(this))
	{
		if ((m_form->properties & PR_VISIBLE) && (m_form->properties & PR_ENABLE))
		{
			if (m_pRc.PtInRect(pt))
				result = RC_IN;
		}
	}

	int move = m_move;
	m_move = mvNone;
	if (result == RC_IN)
	{
		m_move = mvIn;
		if (!m_rBtn.IsRectEmpty() && m_rBtn.PtInRect(pt))
			m_move = mvCombo;
	}
	if (move != m_move)
		invalidateRect(&m_pRc, false);
	if (m_editMode && m_move == mvIn)
		m_axform->DoSomething(doCURSOR, this, CS_IBEAM);
	else if (m_form->cursor >= CS_CURSOR)
		m_axform->DoSomething(doCURSOR, this, (result == RC_IN) ? m_form->cursor : CS_NONE);

	return -1;
}

int CfmCombo::OnKey(int key, int& result)
{
	if (!(m_form->properties & PR_ENABLE))
		return 0;

	result = RC_NOP;
	if (!m_editMode && m_form->vals[2] != NOVALUE && key == (int)m_form->vals[2])
	{
		if (makeListBox())
		{
			m_spreaded = true;
			result = RC_CBSPREAD;
		}
		return 1;
	}
	
	return CfmBase::OnKey(key, result);
}

void CfmCombo::Adjust(CPoint pt, float hRatio, float vRatio)
{
	CfmBase::Adjust(pt, hRatio, vRatio);

	if (m_focus && m_editMode)
	{
		m_axform->m_view->HideCaret();
		m_axform->m_view->CreateSolidCaret(2, (int)m_fHeight);
		m_focusing = false;
	}
}

void CfmCombo::Resize(float hR, float vR, bool font, bool redraw)
{
	CfmBase::Resize(hR, vR, font, redraw);
	DoListBox(false);
	if (m_focus && m_editMode)
	{
		m_axform->m_view->HideCaret();
		m_axform->m_view->CreateSolidCaret(2, (int)m_fHeight);
		m_focusing = false;
	}
}

void CfmCombo::Reload()
{
	CfmBase::Reload();
	m_rBtn.SetRectEmpty();
	DoListBox(false);
}

void CfmCombo::ReadData(CString& data, bool edit, int size, int col, int row)
{
	if (size < 0)
		size = m_form->size;

	if (m_editMode)
	{
		int	len = 0;
		CString	strR = m_strR;
		
		if (edit)
		{
			int	flen = m_form->size;
			len = m_data.GetLength();
			flen = max(flen, len);
			if (m_form->attr & FA_RIGHT)
			{
				data  = CString(' ', flen - len);
				data += m_data;
			}
			else
			{
				data  = m_data;
				data += CString(' ', flen - len);
			}
		}
		else
		{
			len  = strR.GetLength();
			if (m_form->attr & FA_RIGHT)
			{
				data  = CString(' ', size - len);
				data += strR;
			}
			else
			{
				data  = strR;
				data += CString(' ', size - len);
			}
		}
		return;
	}

	if (m_count <= 0 || m_index > m_datas.GetUpperBound())
	{
		data = CString(' ', size);
		return;
	}

	if (edit)
		data = m_displays.GetAt(m_index);
	else
	{
		data = m_datas.GetAt(m_index);
		if (data.GetLength() < size)
		{
			int	len = data.GetLength();
			if (m_form->attr & FA_RIGHT)
				data.Insert(0, CString(' ', max(size, len) - len));
			else
				data += CString(' ', max(size, len) - len);
		}
		else	data = data.Left(size);
	}

	clearInputData();
}

void CfmCombo::ReadAll(CString& str, bool edit, bool head)
{
	str.Empty();
	if (m_count <= 0)
		return;

	int	index = min(m_datas.GetUpperBound(), max(0, m_index));
	str  = m_datas.GetAt(index);
	str += m_displays.GetAt(index);
	str += '\t';

	CString tmps;
	for (int ii = 0; ii < m_count; ii++)
	{
		tmps.Format(_T("%s%s\t"), LPCTSTR(m_datas.GetAt(ii)), LPCTSTR(m_displays.GetAt(ii)));
		str += tmps;
	}
}

void CfmCombo::WriteAll(CString str, bool redraw)
{
	if (m_editMode)
		return;

	if (str.IsEmpty() || str.GetLength() < m_form->size)
	{
		m_index   = 0;
		m_count   = 0;
		m_datas.RemoveAll();
		m_displays.RemoveAll();

		if (redraw)
		{
			if (m_form->properties & PR_TRANSPARENT)
				invalidateRect(&m_pRc, true);
			else
				invalidateRect(&m_pRc, false);
		}
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
	
	m_datas.RemoveAll();
	m_displays.RemoveAll();
	str = (str.GetLength() > offs+1) ? str.Mid(offs+1) : _T("");
	for ( ; true; )
	{
		if (str.IsEmpty())
			break;

		offs = str.Find('\t');
		if (offs == -1)
		{
			m_datas.Add(str.Left(m_form->size));
			tmpS = (str.GetLength() > m_form->size) ? str.Mid(m_form->size) : _T("");
			m_displays.Add(tmpS);
			break;
		}

		tmpS = str.Left(offs);
		m_datas.Add(tmpS.Left(m_form->size));
		tmpS = (tmpS.GetLength() > m_form->size) ? tmpS.Mid(m_form->size) : _T("");
		m_displays.Add(tmpS);
		str = (str.GetLength() > offs+1) ? str.Mid(offs+1) : _T("");
	}

	m_count  = m_datas.GetSize();
	m_vcount = HIWORD(m_form->keys);
	m_vcount = min(m_count, m_vcount ? m_vcount : MAX_COMBO_LINE);
	if (indexS.IsEmpty())
		m_index = m_form->checked;
	else
		WriteData(indexS, false);
	if (redraw)
	{
		DoListBox(false);
		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&m_pRc, true);
		else
			invalidateRect(&m_pRc, false);
	}
}

void CfmCombo::WriteData(CString data, bool redraw, int col, int row)
{
	if (m_form->iok == EIO_INOUT && m_form->isEdit == IE_CORGB)
		doEditCORGB(data);

	makeData(data, m_form->size, m_form->attr, m_form->attr2, m_form->type);

	if (m_form->attr & FA_RIGHT)
		data = data.Right(m_form->size);
	else
		data = data.Left(m_form->size);

	if (m_editMode)
	{
		m_changed = true;
		if (!m_strR.Compare(data))
			return;

		m_strR = data;
		if (!m_caret)
			m_focusing = true;
		editData();
		if (redraw && (m_form->properties & PR_VISIBLE))
			invalidateRect(&m_pRc, false);
		return;
	}

	int	ii;
	CString	text;

	for (ii = 0; ii < m_count; ii++)
	{
		text = m_datas.GetAt(ii);
		if (data.GetLength() < m_form->size && m_form->attr & FA_RIGHT)
		{
			text.TrimLeft();
			text.TrimRight();
		}

		if (!text.Compare(data))
		{
			if (m_cc)
			{
				if (m_index != ii)
				{
					m_index   = ii;
					m_changed = true;
				}
			}
			else
			{
				m_changed = true;
				if (m_index != ii)
					m_index = ii;
			}
			break;
		}
	}

	clearInputData();
	if (redraw && (ii < m_count) && (m_form->properties & PR_VISIBLE))
	{
		DoListBox(false);
		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&m_pRc, true);
		else
			invalidateRect(&m_pRc, false);
	}
}

void CfmCombo::UpdateData(int key, bool moving, int& result)
{
	if (m_editMode)
	{
		updateData(key, moving, result);
		return;
	}

	int	ii, index;

	result    = RC_NOP;
	m_changed = false;

	if (m_strR.GetLength() == m_form->size)
		clearInputData();

	if (key < 0x80)
		m_strR += (TCHAR)key;
	else
	{
		m_strR += HIBYTE(LOWORD(key));
		m_strR += LOBYTE(LOWORD(key));
	}

	if (m_strR.GetLength() == m_form->size)
	{
		for (ii = 0; ii < m_count; ii++)
		{
			if (!m_datas.GetAt(ii).Compare(m_strR))
			{
				index = ii;
				break;
			}
		}

		if (ii < m_count && index != m_index)
		{
			m_index   = index;
			result    = RC_FULL;
			m_changed = true;
			if (m_spreaded && m_list)
				m_list->SetCurSel(m_index);
		}
		else
			m_strR = _T("");
	}
	else if (m_form->attr & FA_RIGHT && m_strR.GetLength() < m_form->size)
	{
		CString	text;

		for (ii = 0; ii < m_count; ii++)
		{
			text = m_datas.GetAt(ii);
			text.TrimLeft();
			text.TrimRight();
			if (!text.CompareNoCase(m_strR))
			{
				index = ii;
				break;
			}
		}

		if (ii < m_count)
		{
			m_strR = _T("");
			if (index != m_index)
			{
				m_index   = index;
				result    = RC_FULL;
				m_changed = true;
				if (m_spreaded && m_list)
					m_list->SetCurSel(m_index);
			}
		}
	}

	if (m_changed)
	{
		if (m_spreaded)
			DoListBox(true);

		invalidateRect(&m_pRc, false);
	}
}

void CfmCombo::InsertData(int key, bool moving, int& result)
{
	UpdateData(key, moving, result);
}

int CfmCombo::GetLength()
{
	return m_editMode ? m_strR.GetLength() : m_form->size;
}

bool CfmCombo::IsChanged(bool reset)
{
	bool	changed = m_changed;
	if (reset)
		m_changed = false;

	return changed;
}

void CfmCombo::ClearData(bool selectOnly)
{
	if (m_editMode)
	{
		if (m_data.IsEmpty() && m_strR.IsEmpty() && !m_caretpos)
			return;

		m_data.Empty();
		m_strR.Empty();
		m_caretpos = 0;
		if (m_focus)
		{
			m_caret    = true;
			m_focusing = false;
		}
	}
	else
	{
		clearInputData();
		if (!selectOnly)
			m_index = 0;
		DoListBox(false);
	}

	if (m_form->properties & PR_TRANSPARENT)
		invalidateRect(&m_pRc, true);
	else
		invalidateRect(&m_pRc, false);
}

void CfmCombo::SetFocus(bool focus)
{
	bool	setfocus = false;

	if (m_focus != focus)	// && focus
		setfocus = true;

	if (m_editMode)
	{
		m_focus = focus;
		if (!m_focus)
		{
			m_state = esNone;
			m_axform->m_view->HideCaret();
		}
		else
		{
			m_caretpos = m_strR.GetLength();
			if (m_caretpos != 0)
				m_state = esReverse;
			else
				m_state = esNormal;

			m_focusing = false;
		}
		m_caret = true;
		invalidateRect(&m_pRc, false);
	}
	else
	{
		clearInputData();
		if (m_focus != focus)
		{
			m_focus = focus;
			if (!m_focus)
			{
				m_click = ckNone;
				if (m_spreaded)
				{
					m_spreaded = false;
					m_axform->DoSomething(doCOMBO, this, 0);
					m_list->ShowWindow(SW_HIDE);
					delete m_list;
					m_list = NULL;
				}
			}
			invalidateRect(&m_pRc, false);
		}
	}

	m_elapse = 0;
	if (setfocus && m_axform->DoSomething(doSETFOCUS, this))
		m_elapse = GetTickCount64();
}

int CfmCombo::IsPointInRect(CPoint pt)
{
	CRect	tRc = m_pRc;

	if (!m_axform->IsFormVisible(this))
		return P_OUT;

	if ((m_form->properties & PR_VISIBLE) && (m_form->properties & PR_ENABLE))
	{
		if (m_spreaded || m_pRc.PtInRect(pt))
			return P_IN;
	}

	return P_OUT;
}

CRect CfmCombo::GetRect(bool original)
{
	if (m_editMode)
		return CfmBase::GetRect(original);

	if (original)
		return m_rect;

	CRect	tRc;

	tRc  = m_pRc;
	tRc |= m_rBtn;
	return tRc;
}

void  CfmCombo::SetEnable(bool enable)
{
	bool	isenable = (m_form->properties & PR_ENABLE) ? true : false;

	if (!m_editMode)
		clearInputData();
	if (enable)
		m_form->properties |= PR_ENABLE;
	else
		m_form->properties &= ~PR_ENABLE;

	if (isenable != enable)
		invalidateRect(&m_pRc, false);
}

void CfmCombo::SetCurSel(int index, bool redraw)
{
	if (m_editMode)
		return;

	clearInputData();
	if (index < 0 || index >= m_count)
		return;

	int	idx = m_index;
	m_index = index;
// updateXXX_2022XX	
	if (redraw && idx != m_index)
		invalidateRect(&m_pRc, false);
}

int CfmCombo::GetCurSel()
{
	if (m_editMode)
		return 0;

	clearInputData();
	return m_index;
}

void CfmCombo::SetEnum(CString text, int value, int func)
{
	if (func > 0)
	{
		switch (func)
		{
		case 55:
			if (m_list)
				m_list->MeasureItemX((LPMEASUREITEMSTRUCT)value);
			break;
		case 56:
			if (m_list && m_spreaded)
				m_list->DrawItemX((LPDRAWITEMSTRUCT)value);
			break;
		}
	}

	if (text.IsEmpty() && !value)
	{
		if (m_form->properties & PR_ACCN)
		{
			DoListBox(false);
			m_editMode = true;
			invalidateRect(&m_pRc, false);
		}
	}
}

void CfmCombo::Refresh()
{
	if (!(m_form->properties & PR_VISIBLE))
		return;

	DoListBox(false);
	if (m_form->properties & PR_TRANSPARENT)
		invalidateRect(&m_pRc, true);
	else
		invalidateRect(&m_pRc, false);
}

void CfmCombo::DoListBox(bool apply, bool focus)
{
	if (m_spreaded && m_list)
	{
		int idx = m_list->GetCurSel();

		if (!focus)
			m_spreaded = false;

		m_list->ShowWindow(SW_HIDE);
		delete m_list;
		m_list = NULL;

		m_axform->DoSomething(doCOMBO, this, 0);
		m_axform->m_view->SetFocus();
		m_axform->DoSomething(doFOCUS, this);
		if (apply)
		{
			SetCurSel(idx);
			m_changed = true;
			m_axform->DoComboBox(this);
		}
//		m_axform->m_view->SetFocus();
//		m_axform->DoSomething(doFOCUS, this);
	}
}

bool CfmCombo::IsFocus(HWND hWnd)
{
	if (hWnd == NULL && m_list && m_spreaded)
		return true;
	return false;
}

void CfmCombo::draw(CDC* dc)
{
	CRect	pRc, tRc;
	CSize	size;
	CPoint	point;
	CBrush*	brush;
	CString	tmps;
	int	ndc = dc->SaveDC();

	if (m_caret)
	{
		m_caret = false;
		if (!m_focus)
			m_axform->m_view->HideCaret();
		else
			m_axform->m_view->CreateSolidCaret(2, (int)m_fHeight);
	}

	setPen(dc);
	setFont(dc);
	pRc  = m_pRc;
	pRc.right -= 2;
	if (m_state == esReverse)
	{
		brush = setBrush(dc);
		setTextColor(dc, getRGB(cWHITE));
		setPaintColor(dc, getRGB(cDBLUE));
		dc->FillRect(pRc, brush);
	}
	else
	{
		if (m_form->properties & PR_ENABLE)
		{
			brush = setBrush(dc);
			setTextColor(dc);
			setPaintColor(dc);
			dc->FillRect(pRc, brush);
		}
		else
		{
			brush = setBrush(dc, getRGB(cXBG));
			setTextColor(dc, getRGB(cXFG));
			dc->FillRect(pRc, brush);
			dc->SetBkMode(TRANSPARENT);
		}
	}

	tRc = pRc;
	tRc.DeflateRect(3, 2);

	tmps = m_data;
	if (m_data.GetLength() != 0)
	{
		size = dc->GetOutputTextExtent(m_data);
		if (size.cx > tRc.Width())
		{
			for (int ii = m_data.GetLength(); ii >= 0; ii--)
			{
				size = dc->GetOutputTextExtent(m_data.Left(ii));
				if (size.cx < tRc.Width())
				{
					tmps = m_data.Left(ii);
					m_caretpos = min(m_caretpos, ii);
					break;
				}
			}
		}
	}
	else
	{
		size.cx = 0;
		size.cy = (int)m_fHeight;
	}
	
	if (m_form->attr & FA_RIGHT)
		dc->DrawText(tmps, tRc, DT_SINGLELINE|DT_VCENTER|DT_RIGHT|DT_NOPREFIX);
	else
		dc->DrawText(tmps, tRc, DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_NOPREFIX);

	if (m_form->borders == BD_NONE)
		drawEdge(dc, pRc, EDGE_SUNKEN, BF_RECT);
	else
	{
	//	if (m_move == mvNone)
			setPen(dc, getRGB(cNORMAL));
	//	else
	//		setPen(dc, getRGB(cHOVER));
		dc->SelectStockObject(NULL_BRUSH);
		dc->Rectangle(pRc);
	}

	if (!(m_form->attr & FA_RIGHT))
	{
		switch (m_state)
		{
		case esReverse:
			m_caretpos = m_strR.GetLength();
			size = dc->GetOutputTextExtent(tmps);
			size.cx++;
			break;
		case esClick:
			for (m_caretpos = 0; m_caretpos < m_strR.GetLength(); m_caretpos++)
			{
				if (whichHangulEdit(m_caretpos, m_strR) == WC_H1)
				{
					m_caretpos++;
					size = dc->GetOutputTextExtent(tmps, calcCaretPos());
				}
				else
					size = dc->GetOutputTextExtent(tmps, calcCaretPos());
			
				if (tRc.left + size.cx >= m_point.x)
					break;
			}

			if (whichHangulEdit(m_caretpos, m_strR) == WC_H2)
				m_caretpos--;
			m_state = esNormal;

		case esNormal:
			size = dc->GetOutputTextExtent(tmps, calcCaretPos());
			break;

		case esNone:
		default :
			break;
		}
	}

	if (m_focus && !m_focusing)
	{
		if (m_form->attr & FA_RIGHT)
			point.x = tRc.left + tRc.Width() - 2;
		else
			point.x = tRc.left + size.cx;

		point.y = tRc.top + (tRc.Height() - (int)m_fHeight) / 2;
		setCaretPos(point);
		m_axform->m_view->ShowCaret();
		m_focusing = true;
	}

	dc->RestoreDC(ndc);
}

void CfmCombo::loadCombo()
{
	char	wb[128] = {0, };
	CString	text, path, string;

	string.Empty();
	path.Format(_T("%s\\%s\\%s.cbs"), m_axform->m_root.GetString(), TABDIR, (char *)m_form->name);
	for (int ii = 0; ; ii++)
	{
		text.Format("%d", ii);
		GetPrivateProfileString("Items", text, "", wb, sizeof(wb), path);
		if (!strlen(wb))
			break;

		string += CString(wb);
		string += '\t';
	}

	if (!string.IsEmpty())
	{
		GetPrivateProfileString("Items", "Index", "", wb, sizeof(wb), path);
		text  = wb;
		text += '\t';
		string.Insert(0, text);
		WriteAll(string);
	}
}

void CfmCombo::lbutton(bool down, CPoint pt, int& result)
{
	result = RC_NOP;

	if (down)
	{
		m_axform->m_cursor = pt;
		m_point = pt;
		m_state = esClick;
		m_axform->DoSomething(doCURSOR, this, CS_IBEAM);
	}
	else
	{
		m_focusing = false;
		invalidateRect(&m_pRc, false);
	}
}

void CfmCombo::updateData(int key, bool moving, int& result)
{
	result = RC_NOP;
	if (m_form->attr & FA_PROTECT)
		return;

	if (!checkKey(key, m_form->type, m_form->attr))
	{
		result = RC_OVERFLOW;
		return;
	}

	CString	tmps = _T("");
	int	len;

	if (m_state == esReverse)
	{
		m_strR.Empty();
		m_caretpos = 0;
		m_state = esNormal;
		invalidateRect(&m_pRc, false);
	}

	m_state = esNormal;
	if (m_form->attr & FA_RIGHT)
	{
		tmps = m_strR.Left(m_caretpos);
		if (m_form->attr & FA_UPPER)
			tmps += (TCHAR)toupper(key);
		else
			tmps += (TCHAR)key;
		len = (key < 0x80)? 1 : 2;
	}
	else
	{
		tmps += m_strR.Left(m_caretpos);
		len = (key < 0x80)? 1 : 2;
		if (len == 1)
		{
			if (m_form->attr & FA_UPPER)
				tmps += (TCHAR)toupper(key);
			else
				tmps += (TCHAR)key;
		}
		else
		{
			tmps += HIBYTE(LOWORD(key));
			tmps += LOBYTE(LOWORD(key));
		}

		int	index = (whichHangulEdit(m_caretpos, m_strR) == WC_H1) ? m_caretpos+2 : m_caretpos+len;
		if (index < m_strR.GetLength())
			tmps += m_strR.Mid(index);
	}

	if (tmps.GetLength() > m_form->size)
	{
		result = RC_OVERFLOW;
		return;
	}

	if (moving)
		m_caretpos += len;

	m_strR = tmps;
	editData();

	m_changed = true;
	if (m_caretpos == m_form->size)
		result = RC_FULL;

	m_focusing = false;
	invalidateRect(&m_pRc, false);
}

bool CfmCombo::makeListBox()
{
	if (m_list)
		return false;

	CRect	cRc, wRc;
	CDC*	dc   = getDC();
	CFont*	font = setFont(dc);
	int	height = (int)m_fHeight + 4;	// 4 = edge(2) * 2

	height = (++height)/2*2;
	cRc = setRect(dc, height);
	releaseDC(dc);
	m_list = new CcbList(this, m_displays, font, height, m_pRc);
	m_axform->DoSomething(doCOMBO, this, 1);
//	if (!m_list->Create(WS_CHILD|WS_VISIBLE, cRc, AfxGetMainWnd(), 0))
	if (!m_list->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, cRc, m_axform->m_view, 226))
	{
		m_axform->DoSomething(doCOMBO, this, 0);
		delete m_list;
		m_list = NULL;
		return false;
	}
	m_list->GetWindowRect(&wRc);
	m_axform->m_view->ScreenToClient(&wRc);
	if (cRc.bottom <= m_pRc.top && wRc.bottom < cRc.bottom)
		m_list->SetWindowPos(NULL, wRc.left, wRc.top+(cRc.bottom-wRc.bottom), 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	m_list->SetCurSel(m_index);
	m_list->SetCaretIndex(m_index);
	m_list->Invalidate();
	return true;
}

CRect CfmCombo::setRect(CDC* dc, int height)
{
	int	value = 0;
	CSize	size;
	CRect	rect, tRc; // wRc;

	tRc   = m_pRc;
	tRc.OffsetRect(m_size);
	tRc.top = tRc.bottom;//+1;
	tRc.right--;
	for (int ii = 0; ii < m_displays.GetSize(); ii++)
	{
		size  = dc->GetOutputTextExtent(m_displays.GetAt(ii));
		value = max(value, size.cx);
	}
	tRc.right  = max(tRc.left+value+GetSystemMetrics(SM_CXVSCROLL)+5, tRc.right);
	tRc.bottom = (int)(tRc.top+(height+1)*m_vcount);
//	tRc.bottom = (int)(tRc.top+(m_fHeight+1)*m_vcount);
	tRc.bottom++;

	rect = tRc;
	m_axform->m_view->GetClientRect(&tRc);
	if (tRc.Height() > 0)
	{
		if (rect.right > tRc.right)
			rect.OffsetRect(tRc.right-rect.right, 0);
		if (rect.bottom > tRc.bottom)
		{
			int	aboveH = min(rect.Height(), m_pRc.top);
			int	belowH = tRc.bottom - (m_pRc.bottom+1);

			if (aboveH > belowH)
			{
				rect.bottom = m_pRc.top+m_size.cy;
				rect.top    = rect.bottom - aboveH;
			}
			else
				rect.bottom = tRc.bottom;
		}
	}

	return rect;
	/*
	rect = tRc;
	m_axform->m_view->ClientToScreen(&tRc);
	AfxGetMainWnd()->GetClientRect(&wRc);
	AfxGetMainWnd()->ClientToScreen(&wRc);
	if (tRc.right > wRc.right)
		tRc.OffsetRect(wRc.right-tRc.right, 0);
	tRc.top    = max(tRc.top, 0);
	tRc.left   = max(tRc.left, 0);
	tRc.bottom = min(tRc.bottom, wRc.bottom);
	AfxGetMainWnd()->ScreenToClient(&tRc);
	return tRc;
	*/
}

void CfmCombo::clearInputData()
{
	m_strR = _T("");
}

void CfmCombo::editData()
{
	m_data.Empty();
	if (!m_editMode || (m_strR.IsEmpty() && !(m_form->properties & PR_EDIT)))
		return;

	m_data = m_strR;
	if (m_form->isEdit == IE_EDIT && !m_edit.IsEmpty())
	{
		int	len,  dcnt = 0;
		
		m_data.Empty();
		len = m_strR.GetLength();
		if (len == 0  && !(m_form->properties & PR_EDIT))
			return;

		CString	format;
		if (m_form->attr & FA_RIGHT)
		{
			format.Empty();
			for (int ii = m_edit.GetLength()-1; ii >= 0; ii--)
			{
				if (m_edit[ii] == '9')
					dcnt++;
				format += m_edit[ii];

				if (dcnt >= len)
					break;
			}
			format.MakeReverse();
		}
		else
			format = m_edit;

		dcnt = 0;
		for (int ii = 0; ii < format.GetLength(); ii++)
		{
			if (format[ii] == '9')
			{
				if (dcnt >= len)
				{
					if (m_form->properties & PR_EDIT)
						m_data += ' ';
					else	break;
				}
				else
				{
					m_data += m_strR[dcnt];
					dcnt++;
				}
			}
			else
				m_data += format[ii];
		}
	}
}

int CfmCombo::calcCaretPos()
{
	int	pos = m_strR.GetLength();

	if (m_strR.IsEmpty())
		return pos;

	if (!m_edit.IsEmpty())
	{
		int	len,  dcnt = 0;
		
		len = m_strR.GetLength();
		for (int ii = 0; ii < m_edit.GetLength(); ii++)
		{
			if (m_edit[ii] == '9')
			{
				pos = ii;
				if (dcnt == m_caretpos)
					break;
				else
				{
					if (m_caretpos == len)
						pos = m_edit.GetLength();
					dcnt++;
				}
			}
		}
	}
	else
		pos = m_caretpos;

	return pos;
}

void CfmCombo::keyDelete(int& result)
{
	CString	tmps = _T("");

	result = RC_NOP;
	if (m_state == esReverse)
	{
		m_strR.Empty();
		m_caretpos = 0;
		m_state = esNormal;
	}
	else
	{
		if (m_caretpos == m_strR.GetLength())
		{
			result = RC_NODELETE;
			return;
		}
		else
		{
			tmps += m_strR.Left(m_caretpos);
			if (whichHangulEdit(m_caretpos, m_strR) == WC_H1)
			{
				if (m_strR.GetLength() > m_caretpos+2)
					tmps += m_strR.Mid(m_caretpos+2);
			}
			else
			{
				if (m_strR.GetLength() > m_caretpos+1)
					tmps += m_strR.Mid(m_caretpos+1);
			}
			m_strR = tmps;
		}
	}

	editData();
	m_changed  = true;
	m_focusing = false;
	invalidateRect(&m_pRc, false);
}

void CfmCombo::keyBackspace(int& result)
{
	CString	tmps = _T("");
	int	pos;

	result = RC_NOP;
	if (m_state == esReverse)
	{
		m_strR.Empty();
		m_caretpos = 0;
		m_state = esNormal;
	}
	else
	{
		if (m_caretpos == 0)
		{
			result = RC_NODELETE;
			return;
		}
		else
		{
			if (m_form->attr & FA_RIGHT)
			{
				m_caretpos--;
				tmps += m_strR.Left(m_caretpos);
			}
			else
			{
				pos = m_caretpos;
				m_caretpos--;
				if (whichHangulEdit(m_caretpos, m_strR) == WC_H2)
					m_caretpos--;
				
				tmps += m_strR.Left(m_caretpos);
				if (m_strR.GetLength() > pos)
					tmps += m_strR.Mid(pos);
			}
			m_strR = tmps;
		}
	}

	editData();
	m_changed  = true;
	m_focusing = false;
	invalidateRect(&m_pRc, false);
}

void CfmCombo::keyReturn(int& result)
{
	if (!m_editMode)
	{
		DoListBox(false);
		result     = RC_CBCLOSE;
		clearInputData();
		invalidateRect(&m_pRc, false);
	}
}

void CfmCombo::keyEscape(int& result)
{
	keyReturn(result);
}

int CfmCombo::keyArrowLeft(int& result)
{
	bool	update = false;

	result = RC_NOP;
	if (m_state == esReverse)
	{
		m_state = esNormal;
		update  = true;
	}

	if (!(m_form->attr & FA_RIGHT))
	{
		m_state = esNormal;
		if (m_caretpos > 0)
		{
			m_caretpos--;
			if (whichHangulEdit(m_caretpos, m_strR) == WC_H2)
				m_caretpos--;
		
			m_focusing = false;
			update     = true;
		}
		else
			return -1;
	}

	if (update)
		invalidateRect(&m_pRc, false);

	return 0;
}

void CfmCombo::keyArrowRight(int& result)
{
	bool	update = false;

	result = RC_NOP;
	if (m_state == esReverse)
	{
		m_state = esNormal;
		update  = true;
	}

	if (!(m_form->attr & FA_RIGHT))
	{
		m_state = esNormal;
		if (m_caretpos < m_strR.GetLength())
		{
			m_caretpos++;
			if (whichHangulEdit(m_caretpos, m_strR) == WC_H2)
				m_caretpos++;

			m_focusing = false;
			update     = true;
		}
	}

	if (update)
		invalidateRect(&m_pRc, false);
}

void CfmCombo::keyArrowDown(int& result)
{
	result = RC_NOP;
	if (!m_editMode && makeListBox())
	{
		m_spreaded = true;
		result = RC_CBSPREAD;
	}
}
