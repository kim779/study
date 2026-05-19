// fmEdit.cpp : implementation file
//

#include "stdafx.h"
#include "fmEdit.h"
#include "axform.h"
#include "month.h"
#include "image.h"

#include "../../h/axiserr.h"
#include "../lib/axislib.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CfmEdit

IMPLEMENT_DYNCREATE(CfmEdit, CfmBase)

CfmEdit::CfmEdit()
{
	EnableAutomation();
}

CfmEdit::CfmEdit(CAxisForm* axform, struct _formR* formR, bool drawOnly)
	: CfmBase(axform, formR)
{
	if (m_form->isEdit == IE_EDIT && m_form->onEdit != NOVALUE)
		m_edit = (char *)&m_axform->m_strR[m_form->onEdit];

	m_drawOnly = drawOnly;
	m_caret    = false;
	m_changed  = false;
	m_state    = esNormal;
	m_focus    = false;
	m_focusing = true;
	m_skip     = 0;
	m_stPos    = 0;
	m_float    = -1;
	m_lower    = 0;
	m_upper    = 0;

	m_strR = m_strR.Left(m_form->size);
	if (!m_drawOnly && (m_form->attr & FA_DATE))
	{
		SYSTEMTIME stime;
		GetLocalTime(&stime);
		switch (m_form->attr2 & SA_DATE)
		{
		case SA_AUTO:
			switch (m_form->size)
			{
			case szYYYYMMDD:
				m_strR.Format("%04d%02d%02d", stime.wYear, stime.wMonth, stime.wDay);
				break;
			case szYYMMDD:
				m_strR.Format("%02d%02d%02d", stime.wYear%100, stime.wMonth, stime.wDay);
				break;
			default:
				break;
			}
			break;

		case SA_YYYYMM:
			m_strR.Format("%04d%02d", stime.wYear, stime.wMonth);
			break;

		case SA_YYMM:
			m_strR.Format("%02d%02d", stime.wYear%100, stime.wMonth);
			break;

		case SA_MMDD:
			m_strR.Format("%02d%02d", stime.wMonth, stime.wDay);
			break;
		}

		CRect	rect = m_pRc;
		rect.left  = rect.right - (GetSystemMetrics(SM_CXVSCROLL)+4);
	}
	m_cal = (Cmonth *)0;

	m_data.Empty();
	m_caretPos = m_strR.GetLength();
	if (m_form->attr & FA_FLOAT)
		m_float = m_strR.Find('.');

	editData();
	m_iRc.SetRectEmpty();

	m_spinRc.SetRectEmpty();
	m_codeRc.SetRectEmpty();
	m_delta = (m_form->keys != NOVALUE) ? (double)m_form->keys : 0;
	m_click = ckNone;

	m_search = false;
	if (m_form->properties & PR_CODE)
	{
		m_category = _T("H");
		m_form->properties &= ~PR_LIST;
	}
	else
		m_category = _T("");
	m_code = _T("");
	m_codes.RemoveAll();

	CString	tmps;

	tmps.Format("%s/%s/%s", m_axform->m_root, TABDIR, PALETTE);
	m_cbRGB = GetPrivateProfileInt(GENERALSN, "ComboBg", -1, tmps);
	m_csRGB = GetPrivateProfileInt(GENERALSN, "ComboFg", -1, tmps);

	CBitmap* bitmap;
	BITMAP	bm;

	m_cpic = m_spic1 = m_spic2 = NULL;
	m_cbW = m_spinW = 0;
	if ((m_form->properties & PR_CODE) || (m_form->attr & FA_DATE))
	{
		tmps.Format("%s\\%s\\axedit.bmp", m_axform->m_root, IMAGEDIR);
		m_cpic = new Cimage(m_axform->m_draw, tmps);
		if (m_cpic->GetHandle(bitmap))
		{
			bitmap->GetBitmap(&bm);
			m_cbW = bm.bmWidth;
		}
		else	m_cbW = 0;

		// for category
		tmps.Format("%s\\%s\\axspin1.bmp", m_axform->m_root, IMAGEDIR);
		m_spic1 = new Cimage(m_axform->m_draw, tmps);
		tmps.Format("%s\\%s\\axspin2.bmp", m_axform->m_root, IMAGEDIR);
		m_spic2 = new Cimage(m_axform->m_draw, tmps);
		if (m_spic1->GetHandle(bitmap))
		{
			bitmap->GetBitmap(&bm);
			m_spinW = bm.bmWidth;
		}
		else	m_spinW = 0;
	}
	else if (m_form->properties & PR_SPIN)
	{
		tmps.Format("%s\\%s\\axspin1.bmp", m_axform->m_root, IMAGEDIR);
		m_spic1 = new Cimage(m_axform->m_draw, tmps);
		tmps.Format("%s\\%s\\axspin2.bmp", m_axform->m_root, IMAGEDIR);
		m_spic2 = new Cimage(m_axform->m_draw, tmps);
		if (m_spic1->GetHandle(bitmap))
		{
			bitmap->GetBitmap(&bm);
			m_spinW = bm.bmWidth;
		}
		else	m_spinW = 0;
	}
	m_move = mvNone;
}

CfmEdit::~CfmEdit()
{
	if (m_cal)
	{
		if (IsWindow(m_cal->GetSafeHwnd()))
			m_cal->SendMessage(WM_CLOSE);
		m_cal->Detach();
		delete m_cal;
	}

	if (m_cpic)
		delete m_cpic;
	if (m_spic1)
		delete m_spic1;
	if (m_spic2)
		delete m_spic2;
}


void CfmEdit::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CfmEdit, CfmBase)
	//{{AFX_MSG_MAP(CfmEdit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CfmEdit, CfmBase)
	//{{AFX_DISPATCH_MAP(CfmEdit)
	DISP_PROPERTY_EX(CfmEdit, "Text", _getText, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CfmEdit, "Data", _getData, _setData, VT_BSTR)
	DISP_PROPERTY_EX(CfmEdit, "PRgb", _getPRgb, _setPRgb, VT_I4)
	DISP_PROPERTY_EX(CfmEdit, "TRgb", _getTRgb, _setTRgb, VT_I4)
	DISP_PROPERTY_EX(CfmEdit, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_PROPERTY_EX(CfmEdit, "Enable", _getEnable, _setEnable, VT_BOOL)
	DISP_PROPERTY_EX(CfmEdit, "Skip", _getSkip, _setSkip, VT_BOOL)
	DISP_PROPERTY_EX(CfmEdit, "Focus", _getFocus, SetNotSupported, VT_BOOL)
	DISP_FUNCTION(CfmEdit, "SetFocus", _SetFocus, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmEdit, "Clear", _Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmEdit, "SetDelta", _SetDelta, VT_EMPTY, VTS_R8)
	DISP_FUNCTION(CfmEdit, "Remove", _Remove, VT_BSTR, VTS_I2)
	DISP_FUNCTION(CfmEdit, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmEdit, "Trigger", _Trigger, VT_EMPTY, VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CfmEdit, "Push", _Push, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CfmEdit, "Pop", _Pop, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CfmEdit, "SetType", _SetType, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CfmEdit, "SetRange", _SetRange, VT_EMPTY, VTS_R8 VTS_R8)
	DISP_FUNCTION(CfmEdit, "SetSend", _SetSend, VT_EMPTY, VTS_BOOL VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmEdit to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {CD6C18AB-F621-4BFB-A37D-6D2D80D31BAA}
static const IID IID_IfmEdit =
{ 0xcd6c18ab, 0xf621, 0x4bfb, { 0xa3, 0x7d, 0x6d, 0x2d, 0x80, 0xd3, 0x1b, 0xaa } };

BEGIN_INTERFACE_MAP(CfmEdit, CfmBase)
	INTERFACE_PART(CfmEdit, IID_IfmEdit, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfmEdit message handlers

void CfmEdit::_SetFocus() 
{
	m_axform->DoSomething(doFOCUS, this);
}

void CfmEdit::_Clear() 
{
	ClearData();
	if (m_axform->DoSomething(doIGNORE, this))
		IsChanged();
}

void CfmEdit::_SetDelta(double delta) 
{
	m_delta = delta;
}

BSTR CfmEdit::_Remove(short ch) 
{
	CString strResult;

	strResult = m_strR;
	strResult.Remove((char)ch);
	return strResult.AllocSysString();
}

void CfmEdit::_Refresh() 
{
	Refresh();
}

void CfmEdit::_Trigger(LPCTSTR name, BOOL all) 
{
	m_axform->DoSomething(doTRIGGER, this, (WPARAM)all,  (LPARAM)name);
}

void CfmEdit::_Push(LPCTSTR name) 
{
	m_axform->DoSomething(doPUSH, this, (WPARAM)0, (LPARAM)name);
}

void CfmEdit::_Pop(LPCTSTR name) 
{
	m_axform->DoSomething(doPOP, this, (WPARAM)0, (LPARAM)name);
	if (m_axform->DoSomething(doIGNORE, this))
		IsChanged();
}

void CfmEdit::_SetType(long type) 
{
	m_form->vals[2] = type;
}

void CfmEdit::_SetRange(double lower, double upper) 
{
	m_lower = lower;
	m_upper = upper;
}

void CfmEdit::_SetSend(BOOL set, long target) 
{
	if (set)
		m_form->attr |= FA_SEND;
	else
		m_form->attr &= ~FA_SEND;

	m_form->attr2 &= ~SA_SEND;

	if (target)
		m_form->attr2 |= SA_ALL;
	else
		m_form->attr2 |= SA_THIS;
}

BSTR CfmEdit::_getText() 
{
	CString strResult;

	ReadData(strResult, true);
	return strResult.AllocSysString();
}

BSTR CfmEdit::_getData() 
{
	CString strResult;

	ReadData(strResult);
	return strResult.AllocSysString();
}

void CfmEdit::_setData(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue);
	if (m_axform->DoSomething(doIGNORE, this))
		IsChanged();
}

long CfmEdit::_getPRgb() 
{
	return m_pRGB;
}

void CfmEdit::_setPRgb(long nNewValue) 
{
	CfmBase::SetBkColor(nNewValue);
}

long CfmEdit::_getTRgb() 
{
	return m_tRGB;
}

void CfmEdit::_setTRgb(long nNewValue) 
{
	CfmBase::SetFgColor(nNewValue);
}

BOOL CfmEdit::_getVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CfmEdit::_setVisible(BOOL bNewValue) 
{
	CfmBase::SetVisible(bNewValue ? true : false);
}

BOOL CfmEdit::_getEnable() 
{
	return (m_form->properties & PR_ENABLE) ? TRUE : FALSE;
}

void CfmEdit::_setEnable(BOOL bNewValue) 
{
	CfmBase::SetEnable(bNewValue ? true : false);
}

BOOL CfmEdit::_getSkip() 
{
	return (m_form->attr & FA_SKIP) ? TRUE : FALSE;
}

void CfmEdit::_setSkip(BOOL bNewValue) 
{
	if (bNewValue)
		m_form->attr |= FA_SKIP;
	else
		m_form->attr &= ~FA_SKIP;
}

BOOL CfmEdit::_getFocus() 
{
	return (m_focus ? TRUE : FALSE);
}

bool CfmEdit::IsValid(int& guide)
{
	if (!CfmBase::IsValid(guide))
		return false;

	if (!m_drawOnly && (m_form->attr & FA_DATE))
	{
		int	result;
		int	year, month, day;
		char	mday[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

		guide = AE_EDATE;
		if (m_strR.GetLength() == 0)
			return true;

		if (m_form->size != m_strR.GetLength())
			return false;

		switch (m_form->attr2 & SA_DATE)
		{
		case SA_AUTO:
			switch (m_form->size)
			{
			case szYYYYMMDD:
				year = atoi(m_strR.Left(4));
				if (year < 1900)
					return false;
				month = atoi(m_strR.Mid(4, 2));
				if (month <= 0 || month > 12)
					return false;
				day = atoi(m_strR.Mid(6, 2));
				if (day <= 0 || day > 31)
					return false;

				result = mday[month-1];
				if (month == 2 && (year % 4) == 0)
				{
					if ((year % 100) || !(year % 400))
						result++;
				}
				return (day <= result);

			case szYYMMDD:
				year = atoi(m_strR.Left(2));
				if (year < 0)
					return false;
				month = atoi(m_strR.Mid(2, 2));
				if (month <= 0 || month > 12)
					return false;
				day = atoi(m_strR.Mid(4, 2));
				if (day <= 0 || day > 31)
					return false;

				year += 1900;
				result = mday[month-1];
				if (month == 2 && (year % 4) == 0)
				{
					if ((year % 100) || !(year % 400))
						result++;
				}
				return (day <= result);

			default:
				break;
			}
			break;

		case SA_YYYYMM:
			year = atoi(m_strR.Left(4));
			if (year < 1900)
				return false;
			month = atoi(m_strR.Mid(4, 2));
			if (month <= 0 || month > 12)
				return false;
			return true;

		case SA_YYMM:
			year = atoi(m_strR.Left(2));
			if (year < 0)
				return false;
			month = atoi(m_strR.Mid(2, 2));
			if (month <= 0 || month > 12)
				return false;
			return true;

		case SA_MMDD:
			CTime	time;
			time = CTime::GetCurrentTime();
			year = time.GetYear();
			month = atoi(m_strR.Left(2));
			if (month <= 0 || month > 12)
				return false;
			day = atoi(m_strR.Mid(2, 2));
			if (day <= 0 || day > 31)
				return false;
			result = mday[month-1];
			if (month == 2 && (year % 4) == 0)
			{
				if ((year % 100) || !(year % 400))
					result++;
			}
			return (day <= result);
		}

		return false;
	}

	return true;
}

void CfmEdit::Adjust(CPoint pt, float hR, float vR)
{
	CfmBase::Adjust(pt, hR, vR);

	if (m_focus)
	{
		m_axform->m_view->HideCaret();
		m_axform->m_view->CreateSolidCaret(2, (int)m_fHeight);
		m_focusing = false;
	}
}

void CfmEdit::Resize(float hR, float vR, bool font, bool redraw)
{
	CfmBase::Resize(hR, vR, font, redraw);
	if (m_focus)
	{
		m_axform->m_view->HideCaret();
		m_axform->m_view->CreateSolidCaret(2, (int)m_fHeight);
		m_focusing = false;
	}
}

void CfmEdit::Reload()
{
	CfmBase::Reload();
	m_spinRc.SetRectEmpty();
	m_codeRc.SetRectEmpty();
}

void CfmEdit::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE))
		return;

	CRect	pRc;
	CSize	tSize;
	CPoint	tPoint;
	CBrush*	brush;
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
	pRc = m_pRc;
	if (m_form->properties & PR_SPIN)
	{
		m_spinRc = pRc;
		pRc.right -= m_spinW ? (int)(m_spinW*m_hR) : (int)(szSCROLL*m_hR);
		m_spinRc.left = pRc.right;
	}

	if ((m_form->properties & PR_CODE) || (m_form->attr & FA_DATE))
	{
		m_codeRc = pRc;
		if ((m_form->attr & FA_DATE) || m_category.GetAt(0) == 'H')
		{
			m_codeRc.DeflateRect(0, 1, 1, 1);
			m_codeRc.left = m_codeRc.right - (m_cbW ? (int)(m_cbW * m_hR) : m_codeRc.Height());
		}
		else
			m_codeRc.left = m_codeRc.right - (m_spinW ? (int)(m_spinW*m_hR) : (int)(szSCROLL*m_hR));
	}

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

	m_tRc = pRc;
	if ((m_form->properties & PR_CODE) || (m_form->attr & FA_DATE))
		m_tRc.right = m_codeRc.left;
	m_tRc.DeflateRect(3, 2);

	CString	tmpS = m_data;
	if (m_data.GetLength() != 0)
	{
		if (m_form->attr & FA_HORS)
		{
			if (!(m_form->attr & FA_RIGHT))
			{
				tmpS = (tmpS.GetLength() > m_stPos) ? tmpS.Mid(m_stPos) : _T("");
				tSize = dc->GetOutputTextExtent(m_data);
				if (tSize.cx > m_tRc.Width())
				{
					for (int ii = tmpS.GetLength(); ii >= 0; ii--)
					{
						tSize = dc->GetOutputTextExtent(tmpS.Left(ii));
						if (tSize.cx <= m_tRc.Width())
						{
							tmpS = tmpS.Left(ii);
							break;
						}
					}
				}
			}
		}
		else
		{
			tSize = dc->GetOutputTextExtent(m_data);
			if (tSize.cx > m_tRc.Width())
			{
				for (int ii = m_data.GetLength(); ii >= 0; ii--)
				{
					tSize = dc->GetOutputTextExtent(m_data.Left(ii));
					if (tSize.cx < m_tRc.Width())
					{
						tmpS = m_data.Left(ii);
						m_caretPos = min(m_caretPos, ii);
						break;
					}
				}
			}
		}
	}
	else
	{
		tSize.cx = 0;
		tSize.cy = (int)m_fHeight;
	}
	
	if (m_form->attr & FA_RIGHT)
		dc->DrawText(tmpS, m_tRc, DT_SINGLELINE|DT_VCENTER|DT_RIGHT|DT_NOPREFIX);
	else
		dc->DrawText(tmpS, m_tRc, DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_NOPREFIX);

	if (m_form->borders == BD_NONE)
	{
	//	drawEdge(dc, pRc, EDGE_SUNKEN, BF_RECT);
		setPen(dc, getRGB(cCB_LINE));
		dc->SelectStockObject(NULL_BRUSH);
		dc->Rectangle(pRc);
	}
	m_iRc = m_tRc;

	if (!(m_form->attr & FA_RIGHT))
	{
		m_iRc.right = m_iRc.left + tSize.cx;
		switch (m_state)
		{
		case esReverse:
			m_caretPos = m_strR.GetLength();
			tSize = dc->GetOutputTextExtent(tmpS);
			tSize.cx++;
			break;
		case esClick:
			for (m_caretPos = m_stPos; m_caretPos < m_strR.GetLength(); m_caretPos++)
			{
				if (whichHangulEdit(m_caretPos, m_strR) == WC_H1)
				{
					m_caretPos++;
					tSize = dc->GetOutputTextExtent(tmpS, calcCaretPos()-m_stPos);
				}
				else
					tSize = dc->GetOutputTextExtent(tmpS, calcCaretPos()-m_stPos);
			
				if (m_tRc.left + tSize.cx >= m_point.x)
					break;
			}

			if (whichHangulEdit(m_caretPos, m_strR) == WC_H2)
				m_caretPos--;
			m_state = esNormal;

		case esNormal:
			tSize = dc->GetOutputTextExtent(tmpS, calcCaretPos()-m_stPos);
			break;

		case esNone:
		default :
			break;
		}
	}

	if (m_form->properties & PR_SPIN)
	{
		CPoint	pts[3];
		COLORREF bg, fg;
		CRect	sRc1, sRc2, tRc;
		int	image;

		bg = (m_cbRGB < 0) ? PALETTERGB(27, 79, 129) : getRGB(m_cbRGB);
		fg = (m_csRGB < 0) ? PALETTERGB(255, 145, 2) : getRGB(m_csRGB);

		if (m_spinRc.Height() % 2)
			m_spinRc.bottom++;
		sRc1 = sRc2 = m_spinRc;
		sRc1.bottom = sRc1.top + m_spinRc.Height()/2;
		sRc2.top = sRc1.bottom;

		// up
		if (m_click == ckSpinUp)
			image = PI_DOWN;
		else if (m_move == mvSpinUp)
			image = PI_ENTER;
		else	image = PI_DEFAULT;

		if (m_spic1 && m_spic1->GetValid(image))
			m_spic1->DrawPicture(dc, sRc1, AL_STRETCH, image);
		else
		{
			tRc = sRc1;
			tRc.DeflateRect(1, 0);

			setPen(dc, bg);
			brush = setBrush(dc, bg);
			dc->FillRect(tRc, brush);
			dc->MoveTo(sRc1.left, sRc1.top+1);
			dc->LineTo(sRc1.left, sRc1.bottom-1);
			dc->MoveTo(sRc1.right-1, sRc1.top+1);
			dc->LineTo(sRc1.right-1, sRc1.bottom-1);

			setPen(dc, fg);
			setBrush(dc, fg);
			tRc.DeflateRect(2, tRc.Height()*2/5);
			tRc.OffsetRect(1, -1);
			tRc.right = tRc.left + (tRc.Width()/2)*2-1;
			tRc.bottom = tRc.top + tRc.Width()/2;
			pts[0].y = tRc.top;
			pts[1].y = tRc.bottom;
			pts[2].y = tRc.bottom;
			pts[0].x = tRc.left + (tRc.Width()/2);
			pts[1].x = tRc.left;
			pts[2].x = tRc.right-1;
			if (m_click == ckSpinUp)
			{
				for (int ii = 0; ii < 3; ii++)
					pts[ii].Offset(1, 1);
			}
			dc->Polygon(pts, 3);
		}

		// down
		if (m_click == ckSpinDown)
			image = PI_DOWN;
		else if (m_move == mvSpinDown)
			image = PI_ENTER;
		else	image = PI_DEFAULT;

		if (m_spic2 && m_spic2->GetValid(PI_DOWN))
			m_spic2->DrawPicture(dc, sRc2, AL_STRETCH, image);
		else
		{
			tRc = sRc2;
			tRc.DeflateRect(1, 0);

			setPen(dc, bg);
			brush = setBrush(dc, bg);
			dc->FillRect(tRc, brush);
			dc->MoveTo(sRc2.left, sRc2.top+1);
			dc->LineTo(sRc2.left, sRc2.bottom-1);
			dc->MoveTo(sRc2.right-1, sRc2.top+1);
			dc->LineTo(sRc2.right-1, sRc2.bottom-1);

			setPen(dc, fg);
			setBrush(dc, fg);
			tRc.DeflateRect(2, tRc.Height()*2/5);
			tRc.OffsetRect(1, 0);
			tRc.right = tRc.left + (tRc.Width()/2)*2-1;
			tRc.bottom = tRc.top + tRc.Width()/2;
			pts[0].y = tRc.bottom;
			pts[1].y = tRc.top;
			pts[2].y = tRc.top;
			pts[0].x = tRc.left + (tRc.Width()/2);
			pts[1].x = tRc.right-1;
			pts[2].x = tRc.left;
			if (m_click == ckSpinDown)
			{
				for (int ii = 0; ii < 3; ii++)
					pts[ii].Offset(1, 1);
			}
			dc->Polygon(pts, 3);
		}
	}

	if ((m_form->properties & PR_CODE) || (m_form->attr & FA_DATE))
	{
		CRect	tRc;
		CPoint	pts[3];
		COLORREF bg, fg;

		bg = (m_cbRGB < 0) ? PALETTERGB(27, 79, 129) : getRGB(m_cbRGB);
		fg = (m_csRGB < 0) ? PALETTERGB(255, 145, 2) : getRGB(m_csRGB);
		if ((m_form->attr & FA_DATE) || m_category.GetAt(0) == 'H')
		{
			int	image = -1;
		
			if (m_click == ckCode)
				image = PI_DOWN;
			else if (m_move == mvCode)
				image = PI_ENTER;
			else	image = PI_DEFAULT;

			if (m_cpic && m_cpic->GetValid(image))
				m_cpic->DrawPicture(dc, m_codeRc, AL_STRETCH, image);
			else
			{
				tRc = m_codeRc;
				tRc.DeflateRect(1, 0);

				setPen(dc, bg);
				brush = setBrush(dc, bg);
				dc->FillRect(tRc, brush);
				dc->MoveTo(m_codeRc.left, m_codeRc.top+1);
				dc->LineTo(m_codeRc.left, m_codeRc.bottom-1);
				dc->MoveTo(m_codeRc.right-1, m_codeRc.top+1);
				dc->LineTo(m_codeRc.right-1, m_codeRc.bottom-1);

				setPen(dc, fg);
				setBrush(dc, fg);
				tRc = m_codeRc;
				tRc.DeflateRect(3, tRc.Height()*2/5);
				tRc.OffsetRect(1, 0);
				tRc.right = tRc.left + (tRc.Width()/2)*2-1;
				tRc.bottom = tRc.top + tRc.Width()/2;
				pts[0].y = tRc.bottom;
				pts[1].y = tRc.top;
				pts[2].y = tRc.top;
				pts[0].x = tRc.left + (tRc.Width()/2);
				pts[1].x = tRc.right-1;
				pts[2].x = tRc.left;
				dc->Polygon(pts, 3);
			}
		}
		else
		{
			CRect	sRc1, sRc2;
			int	image = -1;

			if ((m_codeRc.Height() % 2) == 0)
				m_codeRc.bottom++;
			sRc1 = sRc2 = m_codeRc;
			sRc1.bottom = sRc1.top + m_codeRc.Height()/2;
			sRc2.top = sRc1.bottom+1;

			// up
			if (m_click == ckCodeUp)
				image = PI_DOWN;
			else if (m_move == mvCodeUp)
				image = PI_ENTER;
			else	image = PI_DEFAULT;

			if (m_spic1 && m_spic1->GetValid(image))
				m_spic1->DrawPicture(dc, sRc1, AL_STRETCH, image);
			else
			{
				tRc = sRc1;
				tRc.DeflateRect(1, 0);

				setPen(dc, bg);
				brush = setBrush(dc, bg);
				dc->FillRect(tRc, brush);
				dc->MoveTo(sRc1.left, sRc1.top+1);
				dc->LineTo(sRc1.left, sRc1.bottom-1);
				dc->MoveTo(sRc1.right-1, sRc1.top+1);
				dc->LineTo(sRc1.right-1, sRc1.bottom-1);

				setPen(dc, fg);
				setBrush(dc, fg);
				tRc.DeflateRect(2, tRc.Height()*2/5);
				tRc.OffsetRect(1, -1);
				tRc.right = tRc.left + (tRc.Width()/2)*2-1;
				tRc.bottom = tRc.top + tRc.Width()/2;
				pts[0].y = tRc.top;
				pts[1].y = tRc.bottom;
				pts[2].y = tRc.bottom;
				pts[0].x = tRc.left + (tRc.Width()/2);
				pts[1].x = tRc.left;
				pts[2].x = tRc.right-1;
				if (m_click == ckCodeUp)
				{
					for (int ii = 0; ii < 3; ii++)
						pts[ii].Offset(1, 1);
				}
				dc->Polygon(pts, 3);
			}

			// down
			if (m_click == ckCodeDown)
				image = PI_DOWN;
			else if (m_move == mvCodeDown)
				image = PI_ENTER;
			else	image = PI_DEFAULT;

			if (m_spic2 && m_spic2->GetValid(PI_DOWN))
				m_spic2->DrawPicture(dc, sRc2, AL_STRETCH, image);
			else
			{
				tRc = sRc2;
				tRc.DeflateRect(1, 0);

				setPen(dc, bg);
				brush = setBrush(dc, bg);
				dc->FillRect(tRc, brush);
				dc->MoveTo(sRc2.left, sRc2.top+1);
				dc->LineTo(sRc2.left, sRc2.bottom-1);
				dc->MoveTo(sRc2.right-1, sRc2.top+1);
				dc->LineTo(sRc2.right-1, sRc2.bottom-1);

				setPen(dc, fg);
				setBrush(dc, fg);
				tRc.DeflateRect(2, tRc.Height()*2/5);
				tRc.OffsetRect(1, 0);
				tRc.right = tRc.left + (tRc.Width()/2)*2-1;
				tRc.bottom = tRc.top + tRc.Width()/2;
				pts[0].y = tRc.bottom;
				pts[1].y = tRc.top;
				pts[2].y = tRc.top;
				pts[0].x = tRc.left + (tRc.Width()/2);
				pts[1].x = tRc.right-1;
				pts[2].x = tRc.left;
				if (m_click == ckCodeDown)
				{
					for (int ii = 0; ii < 3; ii++)
						pts[ii].Offset(1, 1);
				}
				dc->Polygon(pts, 3);
			}
		}
	}

	if (m_focus && !m_focusing)
	{
		if (m_form->attr & FA_RIGHT)
			tPoint.x = m_tRc.left + m_tRc.Width() - 2;
		else
			tPoint.x = m_tRc.left + tSize.cx;

		tPoint.y = m_tRc.top + (m_tRc.Height() - (int)m_fHeight) / 2;
		setCaretPos(tPoint);
		m_axform->m_view->ShowCaret();

		m_focusing = true;
	}

	dc->RestoreDC(ndc);
}

void CfmEdit::OnLButton(bool down, CPoint pt, int& result)
{
	result = RC_NOP;

	if (down)
		m_axform->m_cursor = pt;
	else if (!m_focus)
	{
		m_click  = ckNone;
		m_caret  = false;
		m_search = false;
		invalidateRect(&m_iRc, false);
		return;
	}

	switch (m_click)
	{
	case ckSpinUp:
	case ckSpinDown:
		m_click = ckNone;
		invalidateRect(&m_spinRc, false);
		return;

	case ckCode:
		m_search = false;
		if (m_codeRc.PtInRect(pt))
		{
			if (m_form->properties & PR_CODE)
				result = RC_CODE;
			else if (m_form->attr & FA_DATE)
				makeCalendar();	
		}
	case ckCodeUp:
	case ckCodeDown:
		m_click = ckNone;
		invalidateRect(&m_codeRc, false);
		return;

	default:
		break;
	}

	if (((m_form->properties & PR_CODE) || (m_form->attr & FA_DATE)) && !m_codeRc.IsRectEmpty() && m_codeRc.PtInRect(pt))
	{
		m_search = false;
		if ((m_form->attr & FA_DATE) || m_category.GetAt(0) == 'H')
		{
			if (down)
			{
				m_click = ckCode;
				invalidateRect(&m_codeRc, false);
			}
			else
			{
				m_click = ckNone;
				invalidateRect(&m_pRc, false);
			}
		}
		else
		{
			m_click = ckNone;
			if (down)
			{
				CRect	sRc1, sRc2;

				m_click = ckNone;
				sRc1 = sRc2 = m_codeRc;
				sRc1.bottom = sRc1.top + sRc1.Height()/2;
				sRc2.top = sRc1.bottom;

				if (sRc1.PtInRect(pt))
				{
					getCode();
					m_click = ckCodeUp;
					invalidateRect(&sRc1, false);
				}
				else if (sRc2.PtInRect(pt))
				{
					getCode(false);
					m_click = ckCodeDown;
					invalidateRect(&sRc2, false);
				}
			}
		}
		return;
	}

	if ((m_form->properties & PR_SPIN) && !m_spinRc.IsRectEmpty() && m_spinRc.PtInRect(pt))
	{
		if (down)
		{
			CString	text = m_strR;
			CRect	sRc1, sRc2;

			m_click = ckNone;
			sRc1 = sRc2 = m_spinRc;
			sRc1.bottom = sRc1.top + sRc1.Height()/2;
			sRc2.top = sRc1.bottom;

			if (sRc1.PtInRect(pt))
			{
				m_click = ckSpinUp;
				getSpinData();
				if (m_strR.CompareNoCase(text))
				{
					result = RC_SPIN;
					m_changed = true;
				}
			}
			else if (sRc2.PtInRect(pt))
			{
				m_click = ckSpinDown;
				getSpinData(false);
				if (m_strR.CompareNoCase(text))
				{
					result = RC_SPIN;
					m_changed = true;
				}
			}

			if (m_click != ckNone)
			{
				editData();
				m_search   = false;
				m_focusing = false;
				m_state    = esReverse;
				sRc1 = m_pRc;
				sRc1.right = m_spinRc.left;
				invalidateRect(&m_pRc, false);
				invalidateRect(&m_spinRc, false);
			}
		}
		else
		{
			m_click = ckNone;
			invalidateRect(&m_pRc, false);
		}
		return;
	}

	if ((m_form->attr & FA_COMMA) && (m_state == esReverse))
	{
//		m_state = esNormal;
		return;
	}

	if (down)
	{
		m_point = pt;
		m_state = esClick;
	}
	else
	{
		m_focusing = false;
		if (m_caretPos == 0 && !(m_form->attr & FA_RIGHT))
			m_iRc.right += 2;

		invalidateRect(&m_iRc, false);
	}
}

void CfmEdit::OnDblClick(CPoint pt, int& result)
{
	result = RC_NOP;

	if (m_click == ckCode || ((m_form->properties & PR_CODE) && !m_codeRc.IsRectEmpty() && m_codeRc.PtInRect(pt)))
	{
		m_click  = ckNone;
		m_search = false;
		if (m_category.GetAt(0) == 'H')
		{
			result   = RC_CODE;
			invalidateRect(&m_pRc, false);
		}
		else
		{
			CRect	sRc1, sRc2;

			sRc1 = sRc2 = m_codeRc;
			sRc1.bottom = sRc1.top + sRc1.Height()/2;
			sRc2.top = sRc1.bottom;

			if (sRc1.PtInRect(pt))
				getCode();
			else if (sRc2.PtInRect(pt))
				getCode(false);
		}
		return;
	}

	if ((m_form->properties & PR_SPIN) && !m_spinRc.IsRectEmpty() && m_spinRc.PtInRect(pt))
	{
		CRect	sRc1, sRc2;
		CString	text = m_strR;

		m_click = ckNone;
		sRc1 = sRc2 = m_spinRc;
		sRc1.bottom = sRc1.top + sRc1.Height()/2;
		sRc2.top = sRc1.bottom;

		if (sRc1.PtInRect(pt))
		{
			m_click = ckSpinUp;
			getSpinData();
			if (m_strR.CompareNoCase(text))
			{
				result = RC_CHANGED;
				m_changed = true;
			}
		}
		else if (sRc2.PtInRect(pt))
		{
			m_click = ckSpinDown;
			getSpinData(false);
			if (m_strR.CompareNoCase(text))
			{
				result = RC_CHANGED;
				m_changed = true;
			}
		}

		if (m_click != ckNone)
		{
			editData();
			m_search   = false;
			m_focusing = false;
			m_state    = esReverse;
			m_click    = ckNone;

			sRc1 = m_pRc;
			sRc1.right = m_spinRc.left;
			invalidateRect(&sRc1, false);
		}
		return;
	}

//	SetFocus(true);
}

bool CfmEdit::OnRepeat(CPoint pt, int& result)
{
	CRect	tRc;
	CString	text = m_strR;

	result = RC_NOP;
	switch (m_click)
	{
	case ckSpinUp:
		tRc = m_spinRc;
		tRc.bottom = tRc.top + m_spinRc.Height()/2;
		if (!tRc.PtInRect(pt))
			return false;
		getSpinData();
		if (m_strR.CompareNoCase(text))
		{
			result = RC_SPIN;
			m_changed = true;
		}
		break;
	case ckSpinDown:
		tRc = m_spinRc;
		tRc.top += m_spinRc.Height()/2;
		if (!tRc.PtInRect(pt))
			return false;
		getSpinData(false);
		if (m_strR.CompareNoCase(text))
		{
			result = RC_SPIN;
			m_changed = true;
		}
		break;
	}

	if (result == RC_SPIN)
	{
		editData();
		m_search   = false;
		m_focusing = false;
		m_state    = esReverse;

		tRc = m_pRc;
		tRc.right = m_spinRc.left;
		invalidateRect(&tRc, false);
	}

	return true;
}

int CfmEdit::OnDrag(CPoint pt, int& result)
{
	int	move, rc;
	
	rc = CfmBase::OnDrag(pt, result);
	move   = m_move;
	m_move = mvNone;
	if (result == RC_IN)
	{
		m_move = mvIn;
		if (!m_codeRc.IsRectEmpty() && m_codeRc.PtInRect(pt))
		{
			m_move = mvCode;
			if (!(m_form->attr & FA_DATE) && m_category.GetAt(0) != 'H')
			{
				CRect	sRc1, sRc2;

				sRc1 = sRc2 = m_codeRc;
				sRc1.bottom = sRc1.top + sRc1.Height()/2;
				sRc2.top = sRc1.bottom;
				if (sRc1.PtInRect(pt))
					m_move = mvCodeUp;
				else if (sRc2.PtInRect(pt))
					m_move = mvCodeDown;
			}
			result = RC_HOVER;
		}
		else if (!m_spinRc.IsRectEmpty() && m_spinRc.PtInRect(pt))
		{
			CRect	sRc1, sRc2;

			sRc1 = sRc2 = m_spinRc;
			sRc1.bottom = sRc1.top + sRc1.Height()/2;
			sRc2.top = sRc1.bottom;
			if (sRc1.PtInRect(pt))
				m_move = mvSpinUp;
			else if (sRc2.PtInRect(pt))
				m_move = mvSpinDown;
			result = RC_HOVER;
		}
	}
	if (move != m_move)
		invalidateRect(&m_pRc, false);

	return rc;
}

void CfmEdit::SetFocus(bool focus)
{
	CRect	iRc = m_iRc;

	m_focus = focus;
	if (!m_focus)
	{
		m_state    = esNone;
		iRc.right += 2;
		m_axform->m_view->HideCaret();
	}
	else
	{
		m_caretPos = m_strR.GetLength();
		getStartPos();
		if (m_caretPos != 0)
			m_state = esReverse;
		else
			m_state = esNormal;

		m_focusing = false;
		if (m_caretPos == 0 && !(m_form->attr & FA_RIGHT))
			iRc.right += 2;			// caret width
	}
	m_caret = true;
	invalidateRect(&iRc, false);
}

void CfmEdit::WriteData(CString data, bool redraw, int col, int row)
{
	if (m_form->properties & PR_CODE)
	{
		int	pos = data.Find('\n');
		if (pos != -1)
		{
			m_category = data.Left(pos);
			m_code = _T("");
			m_codes.RemoveAll();
			if (m_category.GetAt(0) == 'E')
			{
				data = data.Mid(pos+1);
				for ( ; !data.IsEmpty(); )
				{
					pos = data.Find('\n');
					if (pos != -1)
					{
						m_codes.Add(data.Left(pos));
						data = data.Mid(pos+1);
					}
					else
					{
						m_codes.Add(data);
						data = _T("");
					}
				}
			}
			if (getCode())
			{
				m_codeRc.InflateRect(2, 2);
				invalidateRect(&m_codeRc, false);
			}
			return;
		}
	}

	if (m_form->iok == EIO_INOUT && m_form->isEdit == IE_CORGB)
		doEditCORGB(data);

	makeData(data, m_form->size, m_form->attr, m_form->attr2, m_form->type);
	data.TrimRight();
	if ((m_form->type == IO_NUM || m_form->type == IO_DIGIT) && m_form->attr & FA_RIGHT)
		data.TrimLeft();

	m_float = -1;
	if (m_form->attr & FA_FLOAT)
	{
		char	sign = NULL;

		data.TrimLeft();
		data.TrimRight();
		if (!data.IsEmpty() && (data[0] == '+' || data[0] == '-'))
		{
			sign = data[0];
			data = data.Mid(1);
			data.TrimLeft();
		}
		m_float = data.Find('.');
		if (m_float >= 0)
		{
			CString	tmps, str;

			tmps = data.Left(m_float);
			str  = tmps.Right(m_form->size - m_form->vals[1]);
			tmps = data.Mid(m_float+1);
			tmps = tmps.Left(m_form->vals[1]);
			str += tmps;
			data = str;
		}
		else if (data.IsEmpty() && m_form->attr & FA_ZEROSUP)
			m_float = -1;
		else
		{
			if (m_form->attr & FA_RIGHT)
				data = data.Right(m_form->size);
			else
				data = data.Left(m_form->size);

			m_float = data.GetLength() - m_form->vals[1];
			if (m_float <= 0)
			{
				data = CString('0', abs(m_float)+1) + data;
				m_float = 1;
			}
		}

		if (sign != NULL)
		{
			data.Insert(0, sign);
			m_float++;
		}
	}
	else
	{
		if (m_form->attr & FA_RIGHT)
			data = data.Right(m_form->size);
		else
			data = data.Left(m_form->size);
	}
	m_changed = true;
	if (!m_strR.Compare(data))
		return;

	m_strR = data;
	if (!m_caret)
		m_focusing = true;

	m_search = false;
	editData();
	getStartPos();

	if (redraw && (m_form->properties & PR_VISIBLE))
		invalidateRect(&m_tRc, false);

	if (col >= 0)
		m_axform->DoSomething(doCHANGE, this);
}

void CfmEdit::UpdateData(int key, bool moving, int& result)
{
	result = RC_NOP;
	if (m_form->attr & FA_PROTECT)
		return;

	if (!(m_form->properties & PR_CODE) && !checkKey(key, m_form->type, m_form->attr))
	{
		result = RC_OVERFLOW;
		return;
	}

	CString	tmpS = _T("");
	int	ii, len;

	if (m_state == esReverse)
	{
		m_strR.Empty();
		m_caretPos = 0;
		m_stPos = 0;
		m_float = -1;
		m_state = esNormal;
		invalidateRect(&m_tRc, false);
	}

	if (!m_search)
	{
		switch (key)
		{
		case '+':
		case '-':
			if (m_form->type == IO_NUM && m_strR.GetLength() > 0)
				return;
			break;
		case '.':
			if (m_form->attr & FA_XFLOAT)
				return;
			break;
		default:
			break;
		}

		if ((m_form->attr & FA_FLOAT) || (m_form->attr & FA_COMMA))
		{
			switch (key)
			{
			case '.':
				if (!(m_form->attr & FA_FLOAT))
					break;

				if (m_float != -1)
					return;

				m_float = m_strR.GetLength();
				if (m_float == 0)
				{
					m_strR = '0';
					m_float++;
					m_caretPos = 1;
				}

				editData();
				getStartPos();
				m_changed  = true;
				m_focusing = false;
				invalidateRect(&m_tRc, false);
				return;

			case '0':
				if (m_float >= 0)
					break;

				for (ii = 0; ii < m_strR.GetLength(); ii++)
				{
					if (m_strR[ii] != '0')
						break;
				}
				if (ii >= m_strR.GetLength())
				{
					m_strR = '0';
					m_caretPos = 1;

					editData();
					getStartPos();
					m_changed  = true;
					m_focusing = false;
					invalidateRect(&m_tRc, false);
					return;
				}
				break;

			case '1': case '2': case '3':
			case '4': case '5': case '6':
			case '7': case '8': case '9':
				if ((m_form->attr & FA_FLOAT) && m_float >= 0)
					break;

				for (ii = 0; ii < m_strR.GetLength(); ii++)
				{
					if (m_strR[ii] != '0' && m_strR[ii] != ' ')
						break;
				}

				if (ii >= m_strR.GetLength())
				{
					m_strR.Empty();
					m_caretPos = 0;
				}
				break;
			}
		}
	}

	checkMode();
	if (!m_search && ((m_form->attr & FA_FLOAT)
		&& ((m_float == -1 && m_caretPos >= m_form->size - (int)m_form->vals[1])
		|| (m_float >= 0 && m_strR.GetLength() - m_float >= (int)m_form->vals[1]))
		|| (!(m_form->attr & FA_FLOAT) && m_caretPos >= m_form->size)))
	{
		result = RC_OVERFLOW;
		return;
	}

	m_state = esNormal;
	if (m_form->attr & FA_RIGHT)
	{
		tmpS = m_strR.Left(m_caretPos);
		if (m_form->attr & FA_UPPER)
			tmpS += (TCHAR)toupper(key);
		else
			tmpS += (TCHAR)key;
		len = (key < 0x80)? 1 : 2;
	}
	else
	{
		tmpS += m_strR.Left(m_caretPos);
		len = (key < 0x80)? 1 : 2;
		if (len == 1)
		{
			if (m_form->attr & FA_UPPER)
				tmpS += (TCHAR)toupper(key);
			else
				tmpS += (TCHAR)key;
		}
		else
		{
			tmpS += (TCHAR)HIBYTE(LOWORD(key));
			tmpS += (TCHAR)LOBYTE(LOWORD(key));
		}

		int	index = (whichHangulEdit(m_caretPos, m_strR) == WC_H1) ? m_caretPos+2 : m_caretPos+len;
		if (index < m_strR.GetLength())
			tmpS += m_strR.Mid(index);
	}

	if (!(m_form->properties & PR_CODE) && tmpS.GetLength() > m_form->size)
	{
		result = RC_OVERFLOW;
		return;
	}

	if (moving)
		m_caretPos += len;

	m_strR = tmpS;
	checkMode();
	editData();
	getStartPos();

	m_changed = true;
	if (!m_search && !(m_form->attr & FA_FLOAT) && m_caretPos == m_form->size)
		result = RC_FULL;

	if (m_search)
		result = RC_SEARCH;

	m_focusing = false;
	invalidateRect(&m_tRc, false);
}

void CfmEdit::InsertData(int key, bool moving, int& result)
{
	result = RC_NOP;
	if (m_form->attr & FA_PROTECT)
		return;

	if (!(m_form->properties & PR_CODE) && !checkKey(key, m_form->type, m_form->attr))
	{
		result = RC_OVERFLOW;
		return;
	}

	CString	tmpS = _T("");
	int	len;

	if (m_state == esReverse)
	{
		m_strR.Empty();
		m_caretPos = 0;
		m_stPos = 0;
		m_float = -1;
		m_state = esNormal;
		invalidateRect(&m_tRc, false);
	}

	if (!m_search)
	{
		switch (key)
		{
		case '+':
		case '-':
			if (m_form->type == IO_NUM && m_strR.GetLength() > 0)
				return;
			break;
		case '.':
			if (m_form->attr & FA_XFLOAT)
				return;
			if (m_form->attr & FA_FLOAT)
			{
				if (m_float != -1)
					return;

				m_float = m_strR.GetLength();
				if (m_float == 0)
				{
					m_strR = '0';
					m_float++;
					m_caretPos++;
				}

				editData();
				getStartPos();

				m_focusing = false;
				invalidateRect(&m_tRc, false);
				return;
			}
			break;
		default:
			break;
		}
	}

	checkMode();
	if (!m_search && ((m_form->attr & FA_FLOAT)
		&& ((m_float == -1 && m_caretPos >= m_form->size - (int)m_form->vals[1])
		|| (m_float >= 0 && m_strR.GetLength() - m_float >= (int)m_form->vals[1]))
		|| (!(m_form->attr & FA_FLOAT) && m_caretPos >= m_form->size)))
	{
		result = RC_OVERFLOW;
		return;
	}

	m_state = esNormal;
	if (m_form->attr & FA_RIGHT)
	{
		tmpS = m_strR.Left(m_caretPos);
		if (m_form->attr & FA_UPPER)
			tmpS += (TCHAR)toupper(key);
		else
			tmpS += (TCHAR)key;

		len = (key < 0x80)? 1 : 2;
	}
	else
	{
		tmpS += m_strR.Left(m_caretPos);
		len = (key < 0x80)? 1 : 2;
		if (len == 1)
		{
			if (m_form->attr & FA_UPPER)
				tmpS += (TCHAR)toupper(key);
			else
				tmpS += (TCHAR)key;
		}
		else
		{
			tmpS += (TCHAR)HIBYTE(LOWORD(key));
			tmpS += (TCHAR)LOBYTE(LOWORD(key));
		}

		CString	tmpS_ = (m_strR.GetLength() > m_caretPos+m_skip) ? m_strR.Mid(m_caretPos+m_skip) : _T("");
		tmpS += tmpS_;
	}

	if (!(m_form->properties & PR_CODE) && tmpS.GetLength() > m_form->size)
	{
		result = RC_OVERFLOW;
		return;
	}

	if (moving)
	{
		m_caretPos += len;
		m_skip = 0;
	}
	else
		m_skip = 2;

	m_strR = tmpS;
	checkMode();
	editData();
	getStartPos();

	m_changed = true;
	if (!m_search && !(m_form->attr & FA_FLOAT) && m_strR.GetLength() == m_form->size)
		result = RC_FULL;

	if (m_search)
		result = RC_SEARCH;

	m_focusing = false;
	invalidateRect(&m_tRc, false);
}

void CfmEdit::ReadData(CString& data, bool edit, int col, int row)
{
	int	len;
	char	filler = ' ';
	char	sign   = ' ';
	CString	strR   = m_strR;
	
	if (edit)
	{
		int	flen = m_form->size;
		len = m_data.GetLength();
		if (m_form->attr & FA_COMMA)
			flen += (flen - 1)/3;
		if (m_form->attr & FA_FLOAT)
			flen++;

		flen = max(flen, len);
		if (m_form->attr & FA_RIGHT)
		{
			data  = CString(filler, flen - len);
			data += m_data;
		}
		else
		{
			data  = m_data;
			data += CString(filler, flen - len);
		}
		return;
	}
	
	if (m_search)
	{
		data     = m_strR;
		m_search = false;
		return;
	}

	if (m_form->attr & (FA_COMMA|FA_ZERO))
	{
		filler = '0';
		sign   = '0';
	}

	if (strR.FindOneOf("+-") == 0  && (m_form->attr & FA_RIGHT))
	{
		sign = strR[0];
		strR = strR.Mid(1);

		if (m_form->type == IO_NUM || m_form->type == IO_DIGIT)
			filler = '0';
	}

	len = strR.GetLength();
	if (m_form->attr & FA_RIGHT)
	{
		if (m_form->attr & FA_FLOAT)
		{
			if (m_float <= -1)
			{
				strR  = strR.Right(m_form->size - m_form->vals[1]);
				data  = CString(filler, m_form->size - m_form->vals[1] - strR.GetLength());
				data += strR;
				data += CString(filler, m_form->vals[1]);
			}
			else
			{
				CString	tmps;
				int	floatx = m_float;
				bool	is_sign = (sign == '+' || sign == '-') ? true : false;

				if (is_sign)
					floatx--;
				tmps  = strR.Left(floatx);
				data  = CString(filler, m_form->size - m_form->vals[1] - tmps.GetLength());
				data += tmps;

				tmps  = strR.Mid(floatx);
				tmps  = tmps.Left(m_form->vals[1]);
				data += tmps;
				data += CString(filler, m_form->vals[1] - tmps.GetLength());
			}
		}
		else
		{
			data  = CString(filler, m_form->size - len);
			data += strR;
		}

		if (sign != filler)
			data.SetAt(0, sign);
	}
	else
	{
		data  = strR;
		data += CString(filler, m_form->size - len);
	}
}

void CfmEdit::ClearData(bool selectOnly)
{
	if (m_data.IsEmpty() && m_strR.IsEmpty() && !m_caretPos)
		return;

	m_data.Empty();
	m_strR.Empty();
	m_caretPos = 0;
	m_stPos    = 0;
	m_float    = -1;
	m_search   = false;
	if (m_focus)
	{
		m_caret    = true;
		m_focusing = false;
	}

	invalidateRect(&m_iRc, false);
}

int CfmEdit::GetLength()
{
	return m_strR.GetLength();
}

bool CfmEdit::IsChanged(bool reset)
{
	bool	changed = m_changed;
	if (reset)
		m_changed = false;

	return changed;
}

void CfmEdit::Refresh()
{
	if (m_form->properties & PR_VISIBLE)
		invalidateRect(&m_iRc, false);
}

CString CfmEdit::Remove(char ch)
{
	CString str;

	str = m_strR;
	str.Remove(ch);
	return str;
}

void CfmEdit::GetEnum(CString& text, int& value, int func)
{
	text.Empty();
	if (m_category.Find('H'))
		text = m_category;
	text.TrimLeft();
}

void CfmEdit::SetEnum(CString text, int value, int func)
{
	if (func == 200 && (m_form->properties & (PR_CODE|PR_SPIN))  || (m_form->attr & FA_DATE))
	{
		DWORD	color;
		bool	redraw = false;

		if (m_axform->m_palette->Lookup(m_cbRGB, color))
			redraw = true;
		else if (m_axform->m_palette->Lookup(m_csRGB, color))
			redraw = true;

		if (redraw)
			invalidateRect(&m_codeRc, false);
	}
}

void CfmEdit::DoListBox(bool apply, bool focus)
{
	if (focus)
	{
		m_axform->m_view->SetFocus();
		m_axform->DoSomething(doFOCUS, this);
	}
}

void CfmEdit::keyDelete(int& result)
{
	CString	tmpS = _T("");

	result = RC_NOP;
	if (m_state == esReverse)// || (m_form->attr & FA_FLOAT))
	{
		m_strR.Empty();
		m_caretPos = 0;
		m_stPos = 0;
		m_float = -1;
		m_state = esNormal;
	}
	else if (m_form->attr & FA_FLOAT)
	{
		if (m_caretPos == m_float)
			m_float = -1;
		else
		{
			m_caretPos--;
			m_strR = m_strR.Left(m_caretPos);
		}
		
	}
	else
	{
		if (m_caretPos == m_strR.GetLength())
		{
			result = RC_NODELETE;
			return;
		}
		else
		{
			tmpS += m_strR.Left(m_caretPos);
			if (whichHangulEdit(m_caretPos, m_strR) == WC_H1)
			{
				if (m_strR.GetLength() > m_caretPos+2)
					tmpS += m_strR.Mid(m_caretPos+2);
			}
			else
			{
				if (m_strR.GetLength() > m_caretPos+1)
					tmpS += m_strR.Mid(m_caretPos+1);
			}

			m_strR.Empty();
			m_strR = tmpS;
		}
	}

	checkMode();
	editData();
	getStartPos();

	if (m_search)
		result = RC_SEARCH;
	m_changed  = true;
	m_focusing = false;
	invalidateRect(&m_iRc, false);
}

void CfmEdit::keyBackspace(int& result)
{
	CString	tmpS = _T("");
	int	pos;

	result = RC_NOP;
	if (m_state == esReverse || (m_form->attr & FA_FLOAT))
	{
		m_strR.Empty();
		m_caretPos = 0;
		m_stPos = 0;
		m_float = -1;
		m_state = esNormal;
	}
	else
	{
		if (m_caretPos == 0)
		{
			result = RC_NODELETE;
			return;
		}
		else
		{
			if (m_form->attr & FA_RIGHT)
			{
				m_caretPos--;
				tmpS += m_strR.Left(m_caretPos);
			}
			else
			{
				pos = m_caretPos;
				m_caretPos--;
				if (whichHangulEdit(m_caretPos, m_strR) == WC_H2)
					m_caretPos--;
				
				tmpS += m_strR.Left(m_caretPos);
				if (m_strR.GetLength() > pos)
					tmpS += m_strR.Mid(pos);
			}
			m_strR = tmpS;
		}
	}

	checkMode();
	editData();
	getStartPos();

	if (m_search)
		result = RC_SEARCH;
	m_changed  = true;
	m_focusing = false;
	invalidateRect(&m_iRc, false);
}

int CfmEdit::keyArrowLeft(int& result)
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
		if (m_caretPos > 0)
		{
			m_caretPos--;
			if (whichHangulEdit(m_caretPos, m_strR) == WC_H2)
				m_caretPos--;
		
			m_focusing = false;
			update     = true;
		}
		else
			return (-1);
	}

	getStartPos();
	if (update)
		invalidateRect(&m_iRc, false);

	return 0;
}

void CfmEdit::keyArrowRight(int& result)
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
		if (m_caretPos < m_strR.GetLength())
		{
			m_caretPos++;
			if (whichHangulEdit(m_caretPos, m_strR) == WC_H2)
				m_caretPos++;

			m_focusing = false;
			update     = true;
		}
	}

	getStartPos();
	if (update)
		invalidateRect(&m_iRc, false);
}

void CfmEdit::keyArrowUp(int& result)
{
	result = RC_NOP;
	if (m_form->properties & PR_CODE)
		getCode();
}

void CfmEdit::keyArrowDown(int& result)
{
	result = RC_NOP;
	if (m_form->properties & PR_CODE)
		getCode(false);
}

void CfmEdit::keyPrior(int& result)
{
	keyArrowUp(result);
}

void CfmEdit::keyNext(int& result)
{
	keyArrowDown(result);
}

void CfmEdit::editData()
{
	if (m_form->kind == FM_MEMO)
		return;

	m_data.Empty();
	if (m_strR.IsEmpty() && !(m_form->properties & PR_EDIT))
		return;

	int	ii;

	for (ii = 0; ii < m_strR.GetLength(); ii++)
	{
		if (m_strR[ii] == SO || m_strR[ii] == SI)
		{
			m_data += ' ';
			continue;
		}

		m_data += m_strR[ii];
	}

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
			for (ii = m_edit.GetLength()-1; ii >= 0; ii--)
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
		for (ii = 0; ii < format.GetLength(); ii++)
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
 		return;
	}

	if (m_form->attr & FA_PASSWD)
	{
		CString	tmps = m_strR;
		tmps.TrimRight();
		m_data = _T("");
		for (ii = 0; ii < tmps.GetLength(); ii++)
			m_data += '*';
		return;
	}

	if (m_form->attr & (FA_COMMA|FA_FLOAT))
	{
		bool	zero  = true;
		bool	has_float = false;
		CString	valS  = _T("");
		CString	tmpS  = _T("");
		CString	tmpS_ = _T("");
		int	index = 0;

		m_strR.TrimLeft();
		m_strR.TrimRight();
		if (m_strR.GetLength() == 0)
			return;

		if (m_strR.Find('.') != -1)
			has_float = true;
		valS = CString(' ', m_strR.GetLength());
		for (ii = 0; ii < m_strR.GetLength(); ii++)
		{
			switch (m_strR[ii])
			{
			case '1': case '2': case '3':
			case '4': case '5': case '6':
			case '7': case '8': case '9':
			case '.':
				zero = false;
				valS.SetAt(ii, 'V');
				break;

			case '0':
				if (((m_form->attr & FA_FLOAT) || has_float) && (ii >= m_float-1 || (m_float < 0 && !ii)))
				{
					zero = false;
					valS.SetAt(ii, 'V');
					break;
				}
			case ' ':
				if (zero)
				{
					valS.SetAt(ii, 'G');
					m_strR.SetAt(ii, ' ');
				}
				else
					valS.SetAt(ii, 'V');
				break;
			
			default:
				zero = false;
				valS.SetAt(ii, 'S');
				break;
			}
		}

		if ((m_form->attr & FA_FLOAT) && valS.Find('V') == -1 && valS.Find('S') == -1)
			return;

		if (zero)
		{
			m_strR = m_data = '0';
			return;
		}

		tmpS = m_strR;
		valS.MakeReverse();
		tmpS.MakeReverse();

		ii = 0;
		if (m_form->attr & FA_FLOAT)
		{
			if (m_float >= 0)
			{
				for (ii = 0; ii < tmpS.GetLength() - m_float; ii++)
				{
					if (ii > valS.GetLength()-1)
					{
						tmpS_ += '0';
						continue;
					}

					switch (valS[ii])
					{
					case 'V':
					case 'S':
						tmpS_ += tmpS[ii];
						break;
					case 'G':
						tmpS_ += '0';
					default:
						break;
					}
				}
				tmpS_ += '.';
			}

			if (ii > valS.GetLength()-1)
			{	
				tmpS_ += '0';
				tmpS_.MakeReverse();
				m_data = tmpS_;
				return;
			}
			else
			{
				if (valS[ii] == 'G')
				{
					tmpS_ += '0';
					index++;
					ii++;
				}

				if (!(m_form->attr & FA_COMMA))
				{
					for ( ; ii < tmpS.GetLength(); ii++)
						tmpS_ += tmpS[ii];

					tmpS_.MakeReverse();
					m_data = tmpS_;
					return;
				}
				if (index > 0)
					ii--;
			}
			if (valS.GetLength() > ii)
				valS = valS.Mid(ii);
			else
				valS = _T("");

			if (tmpS.GetLength() > ii)
				tmpS = tmpS.Mid(ii);
			else
				tmpS = _T("");
		}
		else
		{
			ii = tmpS.Find('.');
			if (ii >= 0)
			{
				if (ii < tmpS.GetLength()-1)
				{
					ii++;
					tmpS_ += tmpS.Left(ii);
					if (valS[ii] == 'G')
					{
						tmpS_ += '0';
						index++;
					}
				}
				else if (ii == tmpS.GetLength() -1)
				{
					ii++;
					tmpS_ += tmpS.Left(ii);
					tmpS_ += '0';
					index++;
				}

				if (valS.GetLength() > ii)
					valS = valS.Mid(ii);
				else
					valS = _T("");

				if (tmpS.GetLength() > ii)
					tmpS = tmpS.Mid(ii);
				else
					tmpS = _T("");

				int	signpos = valS.Find('S');
				CString	tmpX  = valS;
				CString tmpX_ = tmpS;
				if (signpos >= 0)
				{
					tmpX  = valS.Left(signpos);
					if (valS.GetLength() > signpos)
						tmpX += valS.Mid(signpos);

					tmpX_  = tmpS.Left(signpos);
					if (tmpS.GetLength() > signpos)
						tmpX_ += tmpS.Mid(signpos);
				}
				else
				{
					tmpX  += ' ';
					tmpX_ += ' ';
				}

				valS = tmpX;
				tmpS = tmpX_;
			}
			else
			{
				if (valS.GetLength() > ii)
					valS = valS.Mid(ii);
				else
					valS = _T("");

				if (tmpS.GetLength() > ii)
					tmpS = tmpS.Mid(ii);
				else
					tmpS = _T("");
			}
		}

		for (ii = 0;  ii < valS.GetLength(); ii++)
		{
			if (ii < index)
				continue;

			if ((ii != 0) && ((ii % 3) == 0))
			{
				switch (valS[ii])
				{
				case 'G':
					tmpS_ += ' ';
					break;
				case 'V':
					tmpS_ += ',';
				default:
					break;
				}
			}
			tmpS_ += tmpS[ii];
		}

		tmpS_.MakeReverse();
		m_data = tmpS_;
	}
}

int CfmEdit::calcCaretPos()
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
				if (dcnt == m_caretPos)
					break;
				else
				{
					if (m_caretPos == len)
						pos = m_edit.GetLength();
					dcnt++;
				}
			}
		}
	}
	else
		pos = m_caretPos;

	return pos;
}

void CfmEdit::getStartPos()
{
	int	stpos = m_stPos;

	m_stPos = 0;
	if (!(m_form->attr & FA_HORS))
		return;

	if (m_strR.IsEmpty())
	{
		m_caretPos = 0;
		return;
	}

	if (m_form->attr & FA_RIGHT)
	{
		m_caretPos = m_strR.GetLength();
		return;
	}

	CDC*	dc  = getDC();
	CSize	tSz = CSize(0, 0);
	setFont(dc);
	m_stPos = stpos;
	tSz = dc->GetOutputTextExtent(m_data);

	if (m_stPos == 0 && tSz.cx + m_fWidth < m_tRc.Width())
	{
		releaseDC(dc);
		return;
	}

	if (m_caretPos <= m_stPos)
	{
		m_stPos -= 4;
		m_stPos = max(0, m_stPos);
	}
	else
	{
		CString	tmpS = _T("");
		if (m_data.GetLength() > m_stPos)
			tmpS = m_data.Mid(m_stPos, m_caretPos - m_stPos);
		tSz = dc->GetOutputTextExtent(tmpS);
		if (tSz.cx + m_fWidth <= m_tRc.Width())
		{
			releaseDC(dc);
			return;
		}
		else
		{
			if (m_caretPos == m_strR.GetLength())
			{
				for (int ii = 0; ii < m_data.GetLength(); ii++)
				{
					tSz = dc->GetOutputTextExtent(m_data.Right(ii));
					if (tSz.cx + m_fWidth >= m_tRc.Width())
					{
						m_stPos = m_caretPos - ii + 3;
						m_stPos = max(0, min(m_stPos, m_caretPos));
						break;
					}
				}
			}
			else
			{
				m_stPos += 4;
				m_stPos = min(m_stPos, m_caretPos);
			}
		}
	}

	if (whichHangulEdit(m_stPos, m_strR) == WC_H2)
		m_stPos++;

	releaseDC(dc);
}

double CfmEdit::getSpinPos(bool up)
{
	double	minV, maxV, value;
	double	delta;

	if (m_lower == 0 && m_upper == 0)
	{
		minV = 0;
		maxV = atof(CString('9', m_form->size));
	}
	else
	{
		minV = m_lower;
		maxV = m_upper;
	}
	if (m_form->vals[2] == NOVALUE)
		value = atof(m_strR);
	else
	{
		CString	tmps = _T("");
		ReadData(tmps);
		value = atof(tmps);
		if (m_form->attr & FA_RIGHT && m_form->attr & FA_FLOAT)
			value /= pow((double)10, (int)m_form->vals[1]);
	}

	if ((up && value == maxV) || (!up && value == minV))
		return value;

	if (m_form->vals[2] == NOVALUE)
		delta = (m_delta > 0) ? m_delta : -m_delta;
	else
		delta = m_axform->GetDelta(m_form->vals[2], value, up);

	value += (up ? delta : -delta);
	value  = max(minV, min(value, maxV));

	return value;
}

void CfmEdit::getSpinData(bool up)
{
	double	value;

	value   = getSpinPos(up);
	m_float = m_axform->GetSpinData(m_form->vals[2], (m_form->attr & FA_FLOAT) ? m_form->vals[1] : 0, m_float, m_strR, value, up);
}

void CfmEdit::checkMode()
{
	m_search = false;
	if (!(m_form->properties & PR_CODE) || (m_form->type != IO_MIXED && m_form->type != IO_ANY))
		return;

	for (int ii = 0; ii < m_strR.GetLength(); ii++)
	{
		if (m_strR.GetAt(ii) < '0' || m_strR.GetAt(ii) > '9')
		{
			if (ii == 0 || isalpha(m_strR.GetAt(ii)) || m_strR.GetAt(ii) & 0x80)
				m_search = true;
			return;
		}
	}
}

bool CfmEdit::getCode(bool up)
{
	int	kind  = -1;
	int	index = 0;
	switch (m_category.GetAt(0))
	{
	case 'H': return true;
	case 'E': return findCode(up);
	default:  break;
	}

	if (m_category.GetLength() > 1)
		index = atoi(m_category.Mid(1));
	return findCode(m_category.GetAt(0), index, up);
}

bool CfmEdit::findCode(bool up)
{
	CString	code = _T("");

	if (m_codes.GetSize() <= 0)
		return false;

	if (m_code.IsEmpty())
	{
		code   = m_codes.GetAt(0);
		m_code = code;
	}
	else
	{
		int	ii;

		for (ii = 0; ii < m_codes.GetSize(); ii++)
		{
			if (!m_code.CompareNoCase(m_codes.GetAt(ii)))
				break;
		}

		if (ii < m_codes.GetSize())
		{
			if (up)
				ii = (ii == 0) ? (int)m_codes.GetUpperBound() : ii-1;
			else
				ii = (ii == (int)m_codes.GetUpperBound()) ? 0 : ii+1;
			code   = m_codes.GetAt(ii);
			m_code = code;
		}
		else
			return false;
	}
	WriteData(code, true, 0);
	return true;
}

bool CfmEdit::findCode(int kind, int index, bool up)
{
	if (m_axform->GetCode(kind, index, m_code, up))
	{
		WriteData(m_code, true, 0);
		return true;
	}

	return false;
}

BOOL CfmEdit::makeCalendar()
{
	if (m_cal)
		delete m_cal;

	m_cal = new Cmonth;
	if (!m_cal->Create(m_axform->m_view, m_codeRc, this))
	{
		delete m_cal;
		m_cal = NULL;
		return FALSE;
	}

	return TRUE;
}
