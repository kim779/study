// fmOut.cpp : implementation file
//

#include "stdafx.h"
#include "fmOut.h"
#include "axform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CfmOut

IMPLEMENT_DYNCREATE(CfmOut, CfmBase)

CfmOut::CfmOut()
{
	EnableAutomation();
}

CfmOut::CfmOut(CAxisForm* axform, struct _formR* formR, int iform)
	: CfmBase(axform, formR, iform)
{
	if (m_form->isEdit == IE_EDIT && m_form->onEdit != NOVALUE)
		m_edit = (char *)&m_axform->m_strR[m_form->onEdit];

	m_strR.Empty();
	m_data.Empty();
	m_tRc = m_pRc;

	if (m_form->attr & FA_RIGHT)
		m_form->align = AL_RIGHT;
	m_form->checked = m_form->iok;
	m_pop = false;
}

CfmOut::~CfmOut()
{
}


void CfmOut::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CfmOut, CfmBase)
	//{{AFX_MSG_MAP(CfmOut)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CfmOut, CfmBase)
	//{{AFX_DISPATCH_MAP(CfmOut)
	DISP_PROPERTY_EX(CfmOut, "Text", _getText, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CfmOut, "Data", _getData, _setData, VT_BSTR)
	DISP_PROPERTY_EX(CfmOut, "PRgb", _getPRgb, _setPRgb, VT_I4)
	DISP_PROPERTY_EX(CfmOut, "TRgb", _getTRgb, _setTRgb, VT_I4)
	DISP_PROPERTY_EX(CfmOut, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_PROPERTY_EX(CfmOut, "Skip", _getSkip, _setSkip, VT_BOOL)
	DISP_PROPERTY_EX(CfmOut, "FloatDigit", _getFloatDigit, _setFloatDigit, VT_I4)
	DISP_FUNCTION(CfmOut, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmOut, "Blink", _Blink, VT_EMPTY, VTS_BOOL VTS_I4)
	DISP_FUNCTION(CfmOut, "Push", _Push, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CfmOut, "Clear", _Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmOut, "GetDataKind", _GetDataKind, VT_I4, VTS_NONE)
	DISP_FUNCTION(CfmOut, "GetMode", _GetMode, VT_I4, VTS_NONE)
	DISP_FUNCTION(CfmOut, "GetLength", _GetLength, VT_I4, VTS_NONE)
	DISP_FUNCTION(CfmOut, "GetFormat", _GetFormat, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CfmOut, "SetText", _SetText, VT_EMPTY, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmOut to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {CCBF17FF-B2D8-4122-A2D1-80FDD7B5BA8C}
static const IID IID_IfmOut =
{ 0xccbf17ff, 0xb2d8, 0x4122, { 0xa2, 0xd1, 0x80, 0xfd, 0xd7, 0xb5, 0xba, 0x8c } };

BEGIN_INTERFACE_MAP(CfmOut, CfmBase)
	INTERFACE_PART(CfmOut, IID_IfmOut, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfmOut message handlers

void CfmOut::_Refresh() 
{
	Refresh();
}

void CfmOut::_Blink(BOOL set, long color) 
{
	m_axform->DoSomething(doBLINK, this, set ? 1 : 0, (LPARAM)color);
}

void CfmOut::_Push(LPCTSTR name) 
{
	m_axform->DoSomething(doPUSH, this, (WPARAM)0, (LPARAM)name);
}

void CfmOut::_Clear() 
{
	ClearData();
}

long CfmOut::_GetDataKind() 
{
	return m_form->type;
}

long CfmOut::_GetMode() 
{
	return m_form->iok;
}

long CfmOut::_GetLength() 
{
	return m_form->size;
}

BSTR CfmOut::_GetFormat() 
{
	return m_edit.AllocSysString();
}

void CfmOut::_SetText(LPCTSTR text) 
{
	if (!m_data.Compare(text))
		return;

	m_data = text;
	m_strR = text;
	if (m_form->properties & PR_VISIBLE)
	{
		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&m_tRc, true);
		else
			invalidateRect(&m_tRc, false);
	}
}

BSTR CfmOut::_getText() 
{
	CString strResult = _T("");

	ReadData(strResult, true);
	return strResult.AllocSysString();
}

BSTR CfmOut::_getData() 
{
	CString strResult = _T("");

	ReadData(strResult);
	return strResult.AllocSysString();
}

void CfmOut::_setData(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue);
}

long CfmOut::_getPRgb() 
{
	return m_pRGB;
}

void CfmOut::_setPRgb(long nNewValue) 
{
	CfmBase::SetBkColor(nNewValue);
}

long CfmOut::_getTRgb() 
{
	return m_tRGB;
}

void CfmOut::_setTRgb(long nNewValue) 
{
	CfmBase::SetFgColor(nNewValue);
}

BOOL CfmOut::_getVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CfmOut::_setVisible(BOOL bNewValue) 
{
	CfmBase::SetVisible(bNewValue ? true : false);
}

BOOL CfmOut::_getSkip() 
{
	return (m_form->attr & FA_SKIP) ? TRUE : FALSE;
}

void CfmOut::_setSkip(BOOL bNewValue) 
{
	if (bNewValue)
		m_form->attr |= FA_SKIP;
	else
		m_form->attr &= ~FA_SKIP;
}

long CfmOut::_getFloatDigit() 
{
	if (m_form->attr & FA_FLOAT)
		return (int)m_form->vals[1];
	return 0;
}

void CfmOut::_setFloatDigit(long nNewValue) 
{
	if (m_form->attr & FA_FLOAT)
		m_form->vals[1] = nNewValue;
}

void CfmOut::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE))
		return;

//	CSize	tSz;
	CBrush*	brush = (CBrush *) 0;
	int	ndc = dc->SaveDC();

	setPen(dc);
	setFont(dc);
	setTextColor(dc);
	if (!(m_form->properties & PR_TRANSPARENT))
	{
		if (m_form->properties & PR_GRADIENT && m_form->vals[2] != NOVALUE)
		{
			COLORREF clr = 	(m_form->vals[2] & userCOLOR) ? m_form->vals[2] : getRGB(m_form->vals[2]);
			drawGradation(dc, m_pRGB, clr);
			dc->SetBkMode(TRANSPARENT);
		}
		else
		{
			setPaintColor(dc);
			brush = setBrush(dc);
			dc->FillRect(m_pRc, brush);
		}
	}

	m_tRc = m_pRc;
	m_tRc.left   += 3;
	m_tRc.right  -= 3;
	if ((m_form->checked == PN_BORDER && m_form->borders != BD_NONE) || m_form->checked == PN_LINE)
	{
		m_tRc.top    += 2;
		m_tRc.bottom -= 2;
	}

	setBrush(dc, m_tRGB);
	drawPolygonText(dc, m_tRc, m_data, m_form->align, m_form->type);

	switch (m_form->checked)
	{
	case PN_BORDER:
		switch (m_form->borders)
		{
		case BD_RAISED:
			drawEdge(dc, m_pRc, EDGE_RAISED, BF_RECT);	break;
		case BD_SUNKEN:
			drawEdge(dc, m_pRc, EDGE_SUNKEN, BF_RECT);	break;
		case BD_BUMP:
			drawEdge(dc, m_pRc, EDGE_BUMP, BF_RECT);	break;
		case BD_ETCHED:
			drawEdge(dc, m_pRc, EDGE_ETCHED, BF_RECT);	break;
		default:
			break;
		}
		break;

	case PN_LINE:
		setPen(dc, m_bRGB);
		dc->SelectStockObject(NULL_BRUSH);
		dc->Rectangle(m_pRc);
	case PN_NONE:
		if (m_pop)
			drawEdge(dc, m_pRc, EDGE_RAISED, BF_RECT);
		break;
	}

	dc->RestoreDC(ndc);
}

void CfmOut::OnLButton(bool down, CPoint pt, int& result)
{
	result      = RC_NOP;
	m_mousedown = down;
	if (m_mousedown && m_form->cursor >= CS_CURSOR)
		m_axform->DoSomething(doCURSOR, this, m_form->cursor);
}

int CfmOut::OnDrag(CPoint pt, int& result)
{
	int	rc = CfmBase::OnDrag(pt, result);

	if (m_form->cursor == CS_POP)
	{
		bool	pop = m_pop;
		m_pop = (result == RC_IN) ? true : false;
		if (pop != m_pop)
			invalidateRect(&m_pRc, false);
	}
	else if (m_form->cursor >= CS_CURSOR)
		m_axform->DoSomething(doCURSOR, this, (result == RC_IN) ? m_form->cursor : CS_NONE);

	return rc;
}

void CfmOut::ReadData(CString& data, bool edit, int size, int col, int row)
{
	int	len,  flen;

	if (edit)
	{
		len = m_data.GetLength();
		flen = m_form->size;
		if (m_form->attr & FA_COMMA)
			flen = flen + (flen - 1)/3;

		flen = max(flen, len);
		if (m_form->attr & FA_RIGHT)
		{
			data  = CString(' ' , flen - len);
			data += m_data;
		}
		else
		{
			data  = m_strR;
			data += CString(' ', flen - len);
		}
		return;
	}

	len  = m_strR.GetLength();
	flen = max(m_form->size, len);

	if (m_form->attr & FA_RIGHT)
	{
		data  = CString(' ', flen - len);
		data += m_strR;
	}
	else
	{
		data  = m_strR;
		data += CString(' ', flen - len);
	}
}

void CfmOut::WriteData(CString data, bool redraw, int col, int row)
{
	if (m_form->isEdit == IE_CORGB)
		doEditCORGB(data);

	makeData(data, m_form->size, m_form->attr, m_form->attr2, m_form->type);
	if (m_form->attr & FA_FLOAT)
	{
		data.Trim();

		CString	text, tmps, str;
		int	pos;
		char	sign   = '\0';

		text = data;
		pos  = text.FindOneOf("+-");
		if (pos >= 0)
		{
			sign = text.GetAt(pos);
			text = text.Mid(pos+1);
		}

		pos = text.Find('.');
		if (pos >= 0)
		{
			tmps = text.Left(pos);
			if (m_form->vals[1] == m_form->size)
				text = tmps;
			else
			{
				str  = tmps.Right(m_form->size - m_form->vals[1]);
				tmps = text.Mid(pos+1);
				tmps = tmps.Left(m_form->vals[1]);
				if (str.IsEmpty())
					str += _T("0");
				tmps += CString('0', m_form->vals[1] - tmps.GetLength());
				str  += tmps;
				text = str;
			}
		}
		else if (!(text.IsEmpty() && (m_form->attr & FA_ZEROSUP)))
		{
			if (m_form->attr & FA_RIGHT)
				text = text.Right(m_form->size);
			else
				text = text.Left(m_form->size);

			pos = text.GetLength() - m_form->vals[1];
			if (pos <= 0)
				text = CString('0', abs(pos)+1) + text;
		}
		if (sign)
			text.Insert(0, sign);
		data = text;
	}
	else
	{
		if ((m_form->attr & FA_RIGHT) || m_form->align == AL_RIGHT)
			data = data.Right(m_form->size);
		else
			data = data.Left(m_form->size);
	}

	makeSignData(data);
	if (!m_strR.Compare(data))
		return;

	m_strR = data;
	m_strR.TrimRight();
	editData();

	if (redraw && (m_form->properties & PR_VISIBLE))
	{
		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&m_tRc, true);
		else
			invalidateRect(&m_tRc, false);
	}
}

void CfmOut::ClearData(bool selectOnly)
{
	if (m_data.IsEmpty() && m_strR.IsEmpty())
		return;

	m_data.Empty();
	m_strR.Empty();

	if (m_form->properties & PR_TRANSPARENT)
		invalidateRect(&m_tRc, true);
	else
		invalidateRect(&m_tRc, false);
}

void CfmOut::SetEnum(CString text, int value, int func)
{
	if (func == 98)	// set border color
		m_bRGB = value;
}

void CfmOut::Refresh()
{
	if (m_form->properties & PR_VISIBLE)
	{
		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&m_tRc, true);
		else
			invalidateRect(&m_tRc, false);
	}
}

void CfmOut::editData()
{
	m_data.Empty();
	if (m_strR.IsEmpty())
		return;

	int	len,  ii;
	CString	tmpS = _T("");
	m_data = m_strR;
	if (!m_edit.IsEmpty() && m_form->isEdit == IE_EDIT)
	{
		int	dcnt = 0;

		m_strR.Trim();
		len = m_strR.GetLength();
		if (len == 0)
			return;

		CString	format;
		if (m_form->attr & FA_RIGHT)
		{
			format.Empty();
			for (ii = m_edit.GetLength()-1; ii >= 0; ii--)
			{
				if (m_edit[ii] == '9')
					dcnt++;

				if (dcnt >= len)
					break;
			}
			format = m_edit.Mid(ii);
		}
		else
			format = m_edit;

		dcnt = 0;
		for (ii = 0; ii < format.GetLength(); ii++)
		{
			if (format[ii] == '9')
			{
				if (dcnt >= len)
					break;
				else
				{
					tmpS += m_strR[dcnt];
					dcnt++;
				}
			}
			else
				tmpS += format[ii];
		}
		m_data = tmpS;
 		return;
	}

	if (m_form->attr & FA_PASSWD)
	{
		CString	tmps = m_strR;
		tmps.TrimRight();
		m_data  = _T("");
		for (ii = 0; ii < tmps.GetLength(); ii++)
			tmpS += '*';
		m_data = tmpS;
		return;
	}

	m_data = m_strR;
	formatData(m_form->attr, m_data, (m_form->vals[1] == m_form->size) ? -1 : (int)m_form->vals[1], m_form->size);
}
