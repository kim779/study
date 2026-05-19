// fmCombo.cpp : implementation file
//

#include "stdafx.h"
#include "amCombo.h"
#include "amform.h"
#include "cbList.h"
#include "image.h"

//#include "../lib/axislib.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	SZ_SCROLL	16

/////////////////////////////////////////////////////////////////////////////
// CamCombo

IMPLEMENT_DYNCREATE(CamCombo, CamBase)

CamCombo::CamCombo()
{
	EnableAutomation();
}

CamCombo::CamCombo(CAmForm* axform, struct _formR* formR)
	: CamBase(axform, formR)
{
	CRect	rect = m_rect;

	rect.right = rect.left;
	rect.top   = rect.bottom-6;

	m_spreaded = false;
	m_changed  = false;
	m_focus    = false;
//	m_count    = LOWORD(m_form->keys);
//	m_vcount   = HIWORD(m_form->keys);

	m_count    = HIWORD(m_form->keys);
	m_vcount   = LOWORD(m_form->keys);

	m_vcount   = min(m_count, m_vcount ? m_vcount : MAX_COMBO_LINE);
	m_index    = max(0, min(m_count, m_form->checked));
	
	m_rBtn.SetRectEmpty();
	m_click = ckNone;

	m_displays.RemoveAll();
	m_datas.RemoveAll();


	//if (m_form->vals[0] == NOVALUE || m_form->vals[1] == NOVALUE)
	CString strDisp = CString(m_form->str);
	CString strData = CString(m_form->dat);
	
	if (strDisp.IsEmpty() || strData.IsEmpty())
		m_count = 0;

	m_editMode = false;
	m_caret    = false;
	m_state    = esNormal;
	m_focusing = true;
	m_caretpos = 0;
	m_data     = _T("");
	m_edit     = _T("");
	/*
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

	tmps.Format("%s/%s/%s", m_axform->m_root, TABDIR, PALETTE);
	m_cbRGB = GetPrivateProfileInt(GENERALSN, "ComboBg", -1, tmps);
	m_csRGB = GetPrivateProfileInt(GENERALSN, "ComboFg", -1, tmps);

//	CBitmap* bitmap;

	m_pic = NULL;
	CString sImage = CString(m_form->str2);
	
//	sImage = "4BTN.BMP";
//	m_form->properties |= PR_IMAGE;

//tmps.Format("%s\\%s\\axedit.bmp", m_axform->m_root, IMAGEDIR);
	CString path = FORMAT("%s\\%s\\%s", m_axform->m_root, IMAGEDIR, sImage);

	if (!sImage.IsEmpty())
		m_form->properties |= (PR_IMAGE|PR_IMAGETEXT) ;

	if (m_form->properties & PR_IMAGE  && !sImage.IsEmpty())	
		m_pic = new Cimage(m_axform->m_pAmDraw, path);
	else 
		m_pic = NULL;
//	m_pic = new Cimage(m_axform->m_pAmDraw, tmps);
	
	int width, height;


	if (m_pic && m_pic->GetSize(width, height))
		m_cbW = height;
	else
		m_cbW = 0;
	
	/*
	if (m_pic->GetHandle(bitmap))
	{
		BITMAP	bm;

		bitmap->GetBitmap(&bm);
		m_cbW = bm.bmWidth;
	}
	else	m_cbW = 0;
	*/

	m_move = mvNone;

	m_list = NULL;
	if (m_count <= 0)
		return;

	//char*	data = &m_axform->m_strR[m_form->vals[0]];
	//char*	disp = &m_axform->m_strR[m_form->vals[1]];

	StrParser(HIWORD(m_form->keys), strDisp, strData);
	/*
	for (int ii = 0; ii < m_count; ii++)
	{
		m_displays.Add(disp);
		disp += strlen(disp);
		disp++;
		m_datas.Add(data);
		data += strlen(data);
		data++;
	}
	*/
	m_count = min(m_count, min((int)m_displays.GetSize(), (int)m_datas.GetSize()));
	clearInputData();
}

CamCombo::~CamCombo()
{
	if (m_list)
		delete m_list;

	m_spreaded = false;
	m_datas.RemoveAll();
	m_displays.RemoveAll();

	if (m_pic)
		delete m_pic;
}


void CamCombo::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CamCombo, CamBase)
	//{{AFX_MSG_MAP(CamCombo)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CamCombo, CamBase)
	//{{AFX_DISPATCH_MAP(CamCombo)
	/*
	DISP_PROPERTY_EX(CamCombo, "Text", _getText, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CamCombo, "Data", _getData, _setData, VT_BSTR)
	DISP_PROPERTY_EX(CamCombo, "PRgb", _getPRgb, _setPRgb, VT_I4)
	DISP_PROPERTY_EX(CamCombo, "TRgb", _getTRgb, _setTRgb, VT_I4)
	DISP_PROPERTY_EX(CamCombo, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_PROPERTY_EX(CamCombo, "Enable", _getEnable, _setEnable, VT_BOOL)
	DISP_PROPERTY_EX(CamCombo, "Index", _getIndex, _setIndex, VT_I4)
	DISP_PROPERTY_EX(CamCombo, "Skip", _getSkip, _setSkip, VT_BOOL)
	DISP_FUNCTION(CamCombo, "SetFocus", _SetFocus, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CamCombo, "Clear", _Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CamCombo, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CamCombo, "Push", _Push, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CamCombo, "Pop", _Pop, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CamCombo, "SetItems", _SetItems, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CamCombo, "ShowDropDown", _ShowDropDown, VT_EMPTY, VTS_BOOL)
	*/
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmCombo to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {27F81344-9295-457A-B5FD-10E34D2DE25C}
static const IID IID_IfmCombo =
{ 0x27f81344, 0x9295, 0x457a, { 0xb5, 0xfd, 0x10, 0xe3, 0x4d, 0x2d, 0xe2, 0x5c } };

BEGIN_INTERFACE_MAP(CamCombo, CamBase)
	INTERFACE_PART(CamCombo, IID_IfmCombo, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CamCombo message handlers

void CamCombo::_SetFocus() 
{
	m_axform->DoSomething(doFOCUS, this);
}

void CamCombo::_Clear() 
{
	ClearData();
	if (m_axform->DoSomething(doIGNORE, this))
		IsChanged();
}

void CamCombo::_Refresh() 
{
	Refresh();
}

void CamCombo::_Push(LPCTSTR name) 
{
	m_axform->DoSomething(doPUSH, this, (WPARAM)0, (LPARAM)name);
}

void CamCombo::_Pop(LPCTSTR name) 
{
	m_axform->DoSomething(doPOP, this, (WPARAM)0, (LPARAM)name);
	if (m_axform->DoSomething(doIGNORE, this))
		IsChanged();
}

void CamCombo::_SetItems(LPCTSTR items) 
{
	WriteAll(items);
}

void CamCombo::_ShowDropDown(BOOL showIt) 
{
	if (showIt)
	{
		m_axform->DoSomething(doFOCUS, this);
		if (!m_editMode && makeListBox())
			m_spreaded = true;
	}
	else if (m_spreaded)
		DoListBox(false);
}

LPCTSTR CamCombo::GetText() 
{
	CString strResult;

	ReadData(strResult, true);
	return (LPCTSTR)strResult;
}

void CamCombo::SetText(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue);
	if (m_axform->DoSomething(doIGNORE, this))
		IsChanged();
}


LPCTSTR CamCombo::GetData() 
{
	CString strResult;
	
	ReadData(strResult);
	return (LPCTSTR)strResult;
}

void CamCombo::SetData(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue);
	if (m_axform->DoSomething(doIGNORE, this))
		IsChanged();
}

long CamCombo::GetPRgb() 
{
	return m_pRGB;
}

void CamCombo::SetPRgb(long nNewValue) 
{
	CamBase::SetBkColor(nNewValue);
}

long CamCombo::GetTRgb() 
{
	return m_tRGB;
}

void CamCombo::SetTRgb(long nNewValue) 
{
	CamBase::SetFgColor(nNewValue);
}

BOOL CamCombo::GetVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CamCombo::SetVisible(BOOL bNewValue) 
{
	CamBase::SetVisible(bNewValue ? true : false);
}

BOOL CamCombo::GetEnable() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CamCombo::SetEnable(BOOL bNewValue) 
{
	CamBase::SetEnable(bNewValue ? true : false);
}

long CamCombo::GetIndex() 
{
	return GetCurSel();
}

void CamCombo::SetIndex(long nNewValue) 
{
	SetCurSel(nNewValue);
}

BOOL CamCombo::GetSkip() 
{
	return (m_form->attr & FA_SKIP) ? TRUE : FALSE;
}

void CamCombo::SetSkip(BOOL bNewValue) 
{
	if (bNewValue)
		m_form->attr |= FA_SKIP;
	else
		m_form->attr &= ~FA_SKIP;
}

void CamCombo::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE))
		return;

	if (m_editMode)
	{
		draw(dc);
		return;
	}

	Graphics g(dc->GetSafeHdc());
	Point	pts[3];
	UINT	bStyle;
	CRect	tmpRect;
	CRect	tRc;
//	CFont*	font;
//	CBrush*	brush;

#ifdef _GDI
	CFont*	oldfont;
	CFont*	font = setFont(dc);
	oldfont = dc->SelectObject(font);
	
	int	ndc = dc->SaveDC();
	int	sMode;
#endif
	
	bStyle = DFCS_SCROLLCOMBOBOX;
	setPen(dc);
	tRc = m_pRc;
	tRc.left += 4;

	SolidBrush *pFontBr;

	if (m_form->properties & PR_ENABLE)
	{
		if (m_spreaded)
		{
			pFontBr = setgFBrush();
			
//			if (!(m_form->properties & PR_TRANSPARENT))
				g.FillRectangle(setgBrush(), RECTG(m_pRc));
		}
		else
		{
			/*
			if (m_focus)
			{
				pFontBr = setgBrush(clWHITE);
				g.FillRectangle(setgBrush(getRGB(m_form->spRGB)), RECTG(m_pRc));
			}
			else
			*/
			{
				pFontBr = setgFBrush();
	//			if (!(m_form->properties & PR_TRANSPARENT))
					g.FillRectangle(setgBrush(), RECTG(m_pRc));
			}
		}
		if (m_click == ckCombo)
			bStyle |= DFCS_PUSHED;
	}
	else
	{
		pFontBr = setgBrush(clGRAY);
		g.FillRectangle(setgBrush(clXBG), RECTG(m_pRc));
		bStyle |= DFCS_INACTIVE;
	}

	CString	tmps = _T("");
	if (m_index < m_displays.GetSize())
	{
		tmps = m_displays.GetAt(m_index);
		tRc.right -= (int)(szSCROLL*m_hR);

#ifdef _GDI
		setTextColor(dc);
		sMode = dc->SetBkMode(TRANSPARENT);		
		dc->DrawText(tmps, tRc, DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_NOPREFIX);
#else
		StringFormat sf;
		sf.SetAlignment(StringAlignmentNear);
		sf.SetLineAlignment(StringAlignmentCenter);
		g.DrawString(tmps.AllocSysString(), -1, setgFont(), RECTF(tRc), &sf, pFontBr);
#endif
	}

	drawEdge(dc, m_pRc, EDGE_SUNKEN, BF_RECT);
	int	image = -1;

	if (m_click == ckCombo)
		image = PI_DOWN;
	else if (m_move == mvCombo)
		image = PI_ENTER;
	else if (!(m_form->properties & PR_ENABLE))
		image = PI_OFF;
	else
		image = PI_DEFAULT;

	m_rBtn = m_pRc;
	
	if (m_pic && m_pic->GetValid(image))
	{
		m_rBtn.DeflateRect(0, 1, 1, 1);
		m_rBtn.left = m_rBtn.right - (int)(m_cbW*m_hR);
		m_pic->DrawPicture(dc, m_rBtn, IA_STRETCH, image);
	}
	else
	{
		ARGB bg, fg;

		bg = (m_cbRGB < 0) ? _ARGB(255, 27, 79, 129) : getgRGB(m_cbRGB);
		fg = (m_csRGB < 0) ? _ARGB(255, 255, 145, 2) : getgRGB(m_csRGB);


		bg = _ARGB(255, 27, 79, 129) ;
		fg = _ARGB(255, 255, 145, 2) ;

		m_rBtn.DeflateRect(0, 3, 2, 3);
		m_rBtn.left = m_rBtn.right - m_rBtn.Height();
		tRc = m_rBtn;
		tRc.DeflateRect(1, 0);
		tRc.left = tRc.right - SZ_SCROLL;

		//g.FillRectangle(setgBrush(bg), RECTG(tRc));
		
		//Rect rc(RECTG(tRc));

		//dc->DrawFrameControl(tRc, DFC_SCROLL, DFCS_SCROLLCOMBOBOX);
		Bitmap* pBitmap;
		pBitmap = DrawFrameControl(dc, tRc, DFC_SCROLL, DFCS_SCROLLCOMBOBOX);
		g.DrawImage(pBitmap, tRc.left, tRc.top, pBitmap->GetWidth(), pBitmap->GetHeight());
		
		//g.DrawLine(setgPen(bg), rc.GetLeft(), rc.GetTop() + 1, rc.GetLeft(), rc.GetBottom() - 1);
		//g.DrawLine(setgPen(bg), rc.GetRight() - 1, rc.GetTop()+1, rc.GetRight()-1, rc.GetBottom()-1);

		//tRc = m_rBtn;
		//tRc.DeflateRect(3, tRc.Height()*2/5);
		//tRc.OffsetRect(1, 0);
		//tRc.right = tRc.left + (tRc.Width()/2)*2-1;
		//tRc.bottom = tRc.top + tRc.Width()/2;
		//pts[0].Y = tRc.bottom;
		//pts[1].Y = tRc.top;
		//pts[2].Y = tRc.top;
		//pts[0].X = tRc.left + (tRc.Width()/2);
		//pts[1].X = tRc.right-1;
		//pts[2].X = tRc.left;
		////dc->Polygon(pts, 3);
		//g.DrawPolygon(setgPen(fg), pts, 3);
		//g.FillPolygon(setgBrush(fg), pts, 3);
	}
//	dc->RestoreDC(ndc);
#ifdef _GDI
	dc->SelectObject(oldfont);
	dc->SetBkMode(sMode);
	dc->RestoreDC(ndc);
#endif
}

void CamCombo::OnLButton(bool down, CPoint pt, int& result)
{
	if (m_editMode)
	{
		lbutton(down, pt, result);
		return;
	}

 	CRect	tmpRect;
 	CRect	tmpRc1,  tmpRc2;
	int	oldIdx;
	bool	rc = FALSE;

	result  = RC_NOP;
	oldIdx  = m_index;
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

void CamCombo::OnDblClick(CPoint pt, int& result)
{
	if (m_editMode)
	{
		result = RC_NOP;
//		SetFocus(true);
		return;
	}

	clearInputData();
	result = RC_CBCLOSE;
	DoListBox(false);
	invalidateRect(&m_pRc, false);
}

int CamCombo::OnDrag(CPoint pt, int& result)
{
	int	move;

	result = RC_NOP;
	if (m_axform->IsFormVisible(this))
	{
		if ((m_form->properties & PR_VISIBLE) && (m_form->properties & PR_ENABLE))
		{
			if (m_pRc.PtInRect(pt))
				result = RC_IN;
		}
	}

	move   = m_move;
	m_move = mvNone;
	if (result == RC_IN)
	{
		m_move = mvIn;
		if (!m_rBtn.IsRectEmpty() && m_rBtn.PtInRect(pt))
		{
			m_move = mvCombo;
			result = RC_HOVER;
		}
	}
	if (move != m_move)
		invalidateRect(&m_pRc, false);

	return -1;
}

int CamCombo::OnKey(int key, int& result)
{
	result = RC_NOP;
//	if (!m_editMode && m_form->vals[2] != NOVALUE && key == (int)m_form->vals[2])
	if (!m_editMode && m_form->onEdit != NOVALUE && key == (int)m_form->onEdit)
	{
		if (makeListBox())
		{
			m_spreaded = true;
			result = RC_CBSPREAD;
		}
		return 1;
	}
	
	return CamBase::OnKey(key, result);
}

void CamCombo::Adjust(CPoint pt, float hRatio, float vRatio)
{
	CamBase::Adjust(pt, hRatio, vRatio);

	if (m_focus && m_editMode)
	{
		m_axform->m_view->HideCaret();
		m_axform->m_view->CreateSolidCaret(2, (int)m_fHeight);
		m_focusing = false;
	}
}

void CamCombo::Resize(float hR, float vR, bool font, bool redraw)
{
	CamBase::Resize(hR, vR, font, redraw);
	DoListBox(false);
	if (m_focus && m_editMode)
	{
		m_axform->m_view->HideCaret();
		m_axform->m_view->CreateSolidCaret(2, (int)m_fHeight);
		m_focusing = false;
	}
}

void CamCombo::Reload()
{
	CamBase::Reload();
	m_rBtn.SetRectEmpty();
	DoListBox(false);
}

void CamCombo::ReadData(CString& data, bool edit, int col, int row)
{
	if (m_editMode)
	{
		int	len;
		CString	strR = m_strR;
		
		if (edit)
		{
			int	flen = m_form->size;
			len = m_data.GetLength();
			flen = max(flen, len);
		/*
			if (m_form->attr & FA_RIGHT)
			{
				data  = CString(' ', flen - len);
				data += m_data;
			}
			else
		*/
			{
				data  = m_data;
				data += CString(' ', flen - len);
			}
		}
		else
		{
			len  = strR.GetLength();
		/*	if (m_form->attr & FA_RIGHT)
			{
				data  = CString(' ', m_form->size - len);
				data += strR;
			}
			else
		*/
			{
				data  = strR;
				data += CString(' ', m_form->size - len);
			}
		}
		return;
	}

	if (m_count <= 0 || m_index > m_datas.GetUpperBound())
	{
		data = CString(' ', m_form->size);
		return;
	}

	if (edit)
		data = m_displays.GetAt(m_index);
	else
		data = m_datas.GetAt(m_index);

	clearInputData();
}

void CamCombo::ReadAll(CString& str, bool edit, bool head)
{
	str.Empty();
	if (m_count <= 0)
		return;

	int	index = min((int)m_datas.GetUpperBound(), max(0, m_index));
	str  = m_datas.GetAt(index);
	str += m_displays.GetAt(index);
	str += '\t';

	CString tmps;
	for (int ii = 0; ii < m_count; ii++)
	{
		tmps.Format("%s%s\t", m_datas.GetAt(ii), m_displays.GetAt(ii));
		str += tmps;
	}
}

void CamCombo::WriteAll(CString str, bool redraw)
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
//			if (m_form->properties & PR_TRANSPARENT)
				invalidateRect(&m_pRc, true);
//			else
//				invalidateRect(&m_pRc, false);
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

//		offs = str.Find('\t');
		offs = str.Find('|');
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

	m_count  = (int)m_datas.GetSize();
	m_vcount = LOWORD(m_form->keys);

	m_vcount = min(m_count, m_vcount ? m_vcount : MAX_COMBO_LINE);
	if (indexS.IsEmpty())
		m_index = m_form->checked;
	else
		WriteData(indexS, false);
	if (redraw)
	{
		DoListBox(false);
//		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&m_pRc, true);
//		else
//			invalidateRect(&m_pRc, false);
	}
}

void CamCombo::WriteData(CString data, bool redraw, int col, int row)
{
//	if (m_form->iok == EIO_INOUT && m_form->isEdit == IE_CORGB)
//		doEditCORGB(data);

	makeData(data, m_form->size, m_form->attr, m_form->attr2, m_form->type);

//	if (m_form->attr & FA_RIGHT)
//		data = data.Right(m_form->size);
//	else
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
	//	if (data.GetLength() < m_form->size && m_form->attr & FA_RIGHT)
		{
			text.TrimLeft();
			text.TrimRight();
		}

		if (!text.Compare(data))
		{
			if (m_index != ii)
			{
				m_index   = ii;
				m_changed = true;
			}
			break;
		}
	}

	clearInputData();
	if (redraw && (ii < m_count) && (m_form->properties & PR_VISIBLE))
	{
		DoListBox(false);
//		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&m_pRc, true);
//		else
//			invalidateRect(&m_pRc, false);
	}
}

void CamCombo::UpdateData(int key, bool moving, int& result)
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
		m_strR += (TCHAR)HIBYTE(LOWORD(key));
		m_strR += (TCHAR)LOBYTE(LOWORD(key));
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
	else if (/*m_form->attr & FA_RIGHT && */m_strR.GetLength() < m_form->size)
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
		{
			result = RC_CBCLOSE;
			DoListBox(false);
		}
		invalidateRect(&m_pRc, false);
	}
}

void CamCombo::InsertData(int key, bool moving, int& result)
{
	UpdateData(key, moving, result);
}

int CamCombo::GetLength()
{
	return m_editMode ? m_strR.GetLength() : m_form->size;
}

bool CamCombo::IsChanged(bool reset)
{
	bool	changed = m_changed;
	if (reset)
		m_changed = false;

	return changed;
}

void CamCombo::ClearData(bool selectOnly)
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

//	if (m_form->properties & PR_TRANSPARENT)
		invalidateRect(&m_pRc, true);
//	else
//		invalidateRect(&m_pRc, false);
}

void CamCombo::SetFocus(bool focus)
{
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
				m_click = ckNone;
			invalidateRect(&m_pRc, false);
		}
	}
}

int CamCombo::IsPointInRect(CPoint pt)
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

CRect CamCombo::GetRect(bool original)
{
	if (m_editMode)
		return CamBase::GetRect(original);

	if (original)
		return m_rect;

	CRect	tRc;

	tRc  = m_pRc;
	tRc |= m_rBtn;
	return tRc;
}

void  CamCombo::_SetEnable(bool enable)
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

void CamCombo::SetCurSel(int index, bool redraw)
{
	if (m_editMode)
		return;

	clearInputData();
	if (index < 0 || index >= m_count)
		return;

	int	idx = m_index;
	m_index = index;
	if (redraw && index != m_index)
		invalidateRect(&m_pRc, false);
}

int CamCombo::GetCurSel()
{
	if (m_editMode)
		return 0;

	clearInputData();
	return m_index;
}

void CamCombo::SetEnum(CString text, int value, int func)
{
	if (func == 200)
	{
//		DWORD	color;
		bool	redraw = false;
		/*
		if (m_axform->m_palette->Lookup(m_cbRGB, color))
			redraw = true;
		else if (m_axform->m_palette->Lookup(m_csRGB, color))
			redraw = true;
		*/
		if (redraw)
			invalidateRect(&m_rBtn, false);
		return;
	}
	/*
	if (text.IsEmpty() && !value)
	{
		if (m_form->properties & PR_ACCN)
		{
			DoListBox(false);
			m_editMode = true;
			invalidateRect(&m_pRc, false);
		}
	}
	*/
}

void CamCombo::Refresh()
{
	if (!(m_form->properties & PR_VISIBLE))
		return;

	DoListBox(false);
//	if (m_form->properties & PR_TRANSPARENT)
		invalidateRect(&m_pRc, true);
//	else
//		invalidateRect(&m_pRc, false);
}

void CamCombo::DoListBox(bool apply, bool focus)
{
	if (m_spreaded && m_list)
	{
		if (!focus)
			m_spreaded = false;

		m_axform->m_view->SetFocus();
		m_axform->DoSomething(doFOCUS, this);
		if (apply)
		{
			SetCurSel(m_list->GetCurSel());
			m_changed = true;
			m_axform->DoComboBox(this);
		}
		delete m_list;
		m_list = NULL;
//		m_axform->m_view->SetFocus();
//		m_axform->DoSomething(doFOCUS, this);
	}
}

bool CamCombo::IsFocus(HWND hWnd)
{
	if (hWnd == NULL && m_list && m_spreaded)
		return true;
	return false;
}

void CamCombo::draw(CDC* dc)
{
	CRect	pRc, tRc;
	CSize	size;
	CPoint	point;
	CBrush*	brush;
	CString	tmps;
	int	ndc = dc->SaveDC();


	Graphics g(dc->GetSafeHdc());

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
	pRc.right -= (int)(szSCROLL*m_hR);
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
	
//	if (m_form->attr & FA_RIGHT)
//		dc->DrawText(tmps, tRc, DT_SINGLELINE|DT_VCENTER|DT_RIGHT|DT_NOPREFIX);
///	else
	dc->DrawText(tmps, tRc, DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_NOPREFIX);
	drawEdge(dc, pRc, EDGE_SUNKEN, BF_RECT);

//	if (!(m_form->attr & FA_RIGHT))
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
	//	if (m_form->attr & FA_RIGHT)
	//		point.x = tRc.left + tRc.Width() - 2;
	//	else
			point.x = tRc.left + size.cx;

		point.y = tRc.top + (tRc.Height() - (int)m_fHeight) / 2;
		setCaretPos(point);
		m_axform->m_view->ShowCaret();
		m_focusing = true;
	}

	dc->RestoreDC(ndc);
}

void CamCombo::lbutton(bool down, CPoint pt, int& result)
{
	result = RC_NOP;

	if (down)
	{
		m_axform->m_cursor = pt;
		m_point = pt;
		m_state = esClick;
	}
	else
	{
		m_focusing = false;
		invalidateRect(&m_pRc, false);
	}
}

void CamCombo::updateData(int key, bool moving, int& result)
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
/*	if (m_form->attr & FA_RIGHT)
	{
		tmps = m_strR.Left(m_caretpos);
		if (m_form->attr & FA_UPPER)
			tmps += (TCHAR)toupper(key);
		else
			tmps += (TCHAR)key;
		len = (key < 0x80)? 1 : 2;
	}
	else*/
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
			tmps += (TCHAR)HIBYTE(LOWORD(key));
			tmps += (TCHAR)LOBYTE(LOWORD(key));
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

bool CamCombo::makeListBox()
{
	if (m_list)
		return false;

	CRect	cRc, wRc;
	CDC*	dc   = getDC();
	CFont* oldfont;
	CFont*	font = setFont(dc);

	TEXTMETRIC metric;
	oldfont = dc->SelectObject(font);
	dc->GetTextMetrics(&metric);
	m_fHeight = (BYTE)abs(metric.tmHeight);
	m_fWidth  = (BYTE)abs(metric.tmAveCharWidth);

	oldfont = dc->SelectObject(font);
	cRc = setRect(dc);
	releaseDC(dc);
	m_list = new CcbList(this, m_displays, font);
//	if (!m_list->Create(WS_CHILD|WS_VISIBLE, cRc, AfxGetMainWnd(), 0))
	if (!m_list->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|LBS_OWNERDRAWVARIABLE | LBS_HASSTRINGS, cRc, m_axform->m_view, 0))
	{
		delete m_list;
		m_list = NULL;
		return false;
	}
	m_list->SetSelectColor(ARGBtoRGB(getRGB(m_form->spRGB)));
	m_list->GetWindowRect(&wRc);
	m_axform->m_view->ScreenToClient(&wRc);
	if (cRc.bottom <= m_pRc.top && wRc.bottom < cRc.bottom)
		m_list->SetWindowPos(NULL, wRc.left, wRc.top+(cRc.bottom-wRc.bottom), 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	m_list->SetCurSel(m_index);
	m_list->SetCaretIndex(m_index);

	dc->SelectObject(oldfont);
	return true;
}

CRect CamCombo::setRect(CDC* dc)
{
	int	value;
	CSize	size;
	CRect	rect, tRc, wRc;

	value = 0;
	tRc   = m_pRc;
	tRc.OffsetRect(m_size);
	tRc.top = tRc.bottom+1;
	tRc.right--;
	for (int ii = 0; ii < m_displays.GetSize(); ii++)
	{
		size  = dc->GetOutputTextExtent(m_displays.GetAt(ii));
		value = max(value, size.cx);
	}
	tRc.right  = max(tRc.left+value+GetSystemMetrics(SM_CXVSCROLL)+5, tRc.right);
	tRc.bottom = (int)(tRc.top+(m_fHeight+1)*m_vcount);
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

void CamCombo::clearInputData()
{
	m_strR = _T("");
}

void CamCombo::editData()
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
		/*
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
		*/
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

int CamCombo::calcCaretPos()
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

void CamCombo::keyDelete(int& result)
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

void CamCombo::keyBackspace(int& result)
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
			/*
			if (m_form->attr & FA_RIGHT)
			{
				m_caretpos--;
				tmps += m_strR.Left(m_caretpos);
			}
			else
			*/
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

void CamCombo::keyReturn(int& result)
{
	if (!m_editMode)
	{
		DoListBox(false);
		result     = RC_CBCLOSE;
		clearInputData();
		invalidateRect(&m_pRc, false);
	}
}

void CamCombo::keyEscape(int& result)
{
	keyReturn(result);
}

int CamCombo::keyArrowLeft(int& result)
{
	bool	update = false;

	result = RC_NOP;
	if (m_state == esReverse)
	{
		m_state = esNormal;
		update  = true;
	}

	//if (!(m_form->attr & FA_RIGHT))
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

void CamCombo::keyArrowRight(int& result)
{
	bool	update = false;

	result = RC_NOP;
	if (m_state == esReverse)
	{
		m_state = esNormal;
		update  = true;
	}

	//if (!(m_form->attr & FA_RIGHT))
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

void CamCombo::keyArrowDown(int& result)
{
	result = RC_NOP;
	if (!m_editMode && makeListBox())
	{
		m_spreaded = true;
		result = RC_CBSPREAD;
	}
}


void CamCombo::StrParser(int nCnt, CString strDisp, CString strData)
{
	vector<string> vDis, vData;

	Split(vDis, string(strDisp), ',');
	Split(vData, string(strDisp), ',');


//	int istrLen;
	for (int ii = 0; ii < nCnt; ii++)
	{
		string ss = vDis[ii];
		if (!ss.empty())
		{
			m_displays.Add(vDis[ii].data());
			m_datas.Add(vData[ii].data());
		}
	}
}
