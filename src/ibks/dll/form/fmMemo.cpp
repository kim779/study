// fmMemo.cpp : implementation file
//

#include "stdafx.h"
#include "fmMemo.h"
#include "axform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	FORM_MEMO	20000
#define	MAX_MEMO_TEXT	15*1024			// 15K

/////////////////////////////////////////////////////////////////////////////
// CfmMemo

IMPLEMENT_DYNCREATE(CfmMemo, CfmEdit)

CfmMemo::CfmMemo()
{
	EnableAutomation();
}

CfmMemo::CfmMemo(CAxisForm* axform, struct _formR* formR, int iform, bool drawOnly)
	: CfmEdit(axform, formR, iform, false)
{
	m_memo = (Cmemo *) NULL;
	m_pWnd = (CWnd *) NULL;
	m_drawOnly = drawOnly;
	m_brush = NULL;

	if (!m_drawOnly)
	{
		m_pWnd = new CWnd;
		m_pWnd->Create(AfxRegisterWndClass(0), "", WS_CHILD|WS_CLIPSIBLINGS, m_pRc, m_axform->m_view, FORM_MEMO);

		DWORD	style = WS_CHILD|WS_VSCROLL|ES_MULTILINE|ES_AUTOVSCROLL|ES_WANTRETURN;
		if (m_form->borders != BD_NONE)
			style |= WS_BORDER;
		m_memo = new Cmemo(m_axform->m_view, this);
		m_memo->Create(style, CRect(0, 0, m_pRc.Width(), m_pRc.Height()), m_pWnd, FORM_MEMO+1);
		m_memo->SetColor(m_pRGB, m_tRGB);
		switch (m_form->iok)
		{
		case EIO_INPUT:
		case EIO_INOUT:
			m_memo->LimitText(MAX_MEMO_TEXT);
			break;
		case EIO_OUTPUT:
			m_memo->SetReadOnly();
			break;
		default:
			break;
		}
		m_pWnd->ShowWindow((m_form->properties & PR_VISIBLE) ? SW_SHOW : SW_HIDE);
		m_memo->ShowWindow((m_form->properties & PR_VISIBLE) ? SW_SHOW : SW_HIDE);
		setFont();
	}
}

CfmMemo::~CfmMemo()
{
	if (m_memo)
	{
		if (IsWindow(m_memo->GetSafeHwnd()))
			m_memo->SendMessage(WM_CLOSE);
		m_memo->Detach();
		delete m_memo;
		m_memo = (Cmemo *) NULL;
	}
	if (m_pWnd)	delete m_pWnd;
}


void CfmMemo::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CfmMemo, CfmBase)
	//{{AFX_MSG_MAP(CfmMemo)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CfmMemo, CfmBase)
	//{{AFX_DISPATCH_MAP(CfmMemo)
	DISP_PROPERTY_EX(CfmMemo, "Text", _getText, _setText, VT_BSTR)
	DISP_PROPERTY_EX(CfmMemo, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_PROPERTY_EX(CfmMemo, "Enable", _getEnable, _setEnable, VT_BOOL)
	DISP_FUNCTION(CfmMemo, "SetFocus", _SetFocus, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmMemo, "Clear", _Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmMemo, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmMemo, "Save", _Save, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmMemo, "Print", _Print, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmMemo to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {8B328127-73D1-4510-A676-B2F56B24C351}
static const IID IID_IfmMemo =
{ 0x8b328127, 0x73d1, 0x4510, { 0xa6, 0x76, 0xb2, 0xf5, 0x6b, 0x24, 0xc3, 0x51 } };

BEGIN_INTERFACE_MAP(CfmMemo, CfmBase)
	INTERFACE_PART(CfmMemo, IID_IfmMemo, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfmMemo message handlers

void CfmMemo::_SetFocus() 
{
	m_axform->DoSomething(doFOCUS, this);
}

void CfmMemo::_Clear() 
{
	ClearData();
}

void CfmMemo::_Refresh() 
{
	Refresh();
}

void CfmMemo::_Save() 
{
	m_axform->DoSomething(doTEXT, this);
}

void CfmMemo::_Print() 
{
	m_axform->DoSomething(doPRINT, this);
}

BSTR CfmMemo::_getText() 
{
	CString strResult = _T("");

	ReadAll(strResult);
	return strResult.AllocSysString();
}

void CfmMemo::_setText(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue);
}

BOOL CfmMemo::_getVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CfmMemo::_setVisible(BOOL bNewValue) 
{
	SetVisible(bNewValue ? true : false);
}

BOOL CfmMemo::_getEnable() 
{
	return (m_form->properties & PR_ENABLE) ? TRUE : FALSE;
}

void CfmMemo::_setEnable(BOOL bNewValue) 
{
	BOOL	isenable = (m_form->properties & PR_ENABLE) ? TRUE : FALSE;

	if (bNewValue)
		m_form->properties |= PR_ENABLE;
	else
		m_form->properties &= ~PR_ENABLE;

	if (isenable != bNewValue)
		m_memo->EnableWindow(bNewValue);
}

void CfmMemo::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE))
		return;

	if (m_drawOnly)
	{
		if (!(m_form->properties & PR_TRANSPARENT))
		{
			CBrush* brush = setBrush(dc, getRGB(cWHITE));
			dc->FillRect(m_pRc, brush);
		}
		setTextColor(dc, getRGB(cBLACK));
		dc->DrawText(m_strR, &m_pRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_NOPREFIX);
		drawEdge(dc, m_pRc, EDGE_SUNKEN, BF_RECT);
	}
	else
	{
		if (!m_pWnd->IsWindowVisible() || !m_memo->IsWindowVisible())
		{
			m_pWnd->ShowWindow(SW_SHOW);
			m_memo->ShowWindow(SW_SHOW);
		}

		if (dc->IsPrinting())
		{
			CString	text;
			char	wb[1024];
			int	rc, first, lines;

			CfmBase::setFont(dc);
			setTextColor(dc, getRGB(cBLACK));
			setPaintColor(dc, getRGB(cWHITE));

			text.Empty();
			lines = m_memo->GetLineCount();
			first = m_memo->GetFirstVisibleLine();
			for (int ii = 0; ii < lines; ii++)
			{
				ZeroMemory(wb, sizeof(wb));
				rc = m_memo->GetLine(first+ii, wb, sizeof(wb));
				if (rc)
					text += CString(wb, rc);
				text += (TCHAR)0x0d;
				text += (TCHAR)0x0a;
			}
			dc->DrawText(text, &m_pRc, DT_LEFT|DT_NOPREFIX);
		}
	}
}

void CfmMemo::SetFocus(bool focus)
{
	CfmBase::SetFocus(focus);
	if (m_pWnd == NULL || m_memo == NULL)
		return;

	if (focus)
		m_memo->SetFocus();
	else if (GetFocus() == m_pWnd->GetSafeHwnd())
		m_axform->m_view->SetFocus();
	else if (GetFocus() == m_memo->GetSafeHwnd())
	{
		m_pWnd->SetFocus();
		m_axform->m_view->SetFocus();
	}
}

void CfmMemo::Adjust(CSize size)
{
	CfmBase::Adjust(size);

	if (!m_drawOnly)
	{
		UINT	flag = SWP_NOZORDER;
		if (m_form->properties & PR_VISIBLE)
			flag |= SWP_SHOWWINDOW;
		m_pWnd->SetWindowPos(m_axform->m_view, m_pRc.left + m_size.cx, m_pRc.top + m_size.cy,
					m_pRc.Width(), m_pRc.Height(), flag);
		m_memo->SetWindowPos(m_pWnd, 0, 0, m_pRc.Width(), m_pRc.Height(), flag);
	}
}

void CfmMemo::Adjust(CPoint pt, float hR, float vR)
{
	CfmBase::Adjust(pt, hR, vR);

	if (!m_drawOnly)
	{
		UINT	flag = SWP_NOZORDER;
		if (m_form->properties & PR_VISIBLE)
			flag |= SWP_SHOWWINDOW;
		m_pWnd->SetWindowPos(m_axform->m_view, m_pRc.left + m_size.cx, m_pRc.top + m_size.cy,
					m_pRc.Width(), m_pRc.Height(), flag);
		m_memo->SetWindowPos(m_pWnd, 0, 0, m_pRc.Width(), m_pRc.Height(), flag);
		setFont();
	}
}

void CfmMemo::Resize(float hR, float vR, bool font, bool redraw)
{
	m_hR = hR;
	m_vR = vR;

	CRect	tRc, iRc; //, oldRc;

//	oldRc = m_pRc;
	tRc   = calcRect(m_rect, hR, vR);
	if (m_pRc.EqualRect(tRc))
		return;

	iRc.left   = min(tRc.left, m_pRc.left);
	iRc.top    = min(tRc.top, m_pRc.top);
	iRc.right  = max(tRc.right, m_pRc.right);
	iRc.bottom = max(tRc.bottom, m_pRc.bottom);

	m_pRc = tRc;
	if (font)
		setFontSize(hR, vR, true);
	if (m_drawOnly)
	{
		if (redraw)
		{
			if (m_form->properties & PR_TRANSPARENT)
				invalidateRect(&iRc, true);
			else
				invalidateRect(&iRc, false);
		}
	}
	else
	{
		UINT	flag = SWP_NOZORDER;
		if (m_form->properties & PR_VISIBLE)
			flag |= SWP_SHOWWINDOW;
		m_pWnd->SetWindowPos(m_axform->m_view, m_pRc.left + m_size.cx, m_pRc.top + m_size.cy,
					m_pRc.Width(), m_pRc.Height(), flag);
		m_memo->SetWindowPos(m_pWnd, 0, 0, m_pRc.Width(), m_pRc.Height(), flag);
		setFont();
	}
}

void CfmMemo::Reload()
{
	CfmBase::Reload();

	if (!m_drawOnly)
	{
		UINT	flag = SWP_NOZORDER;
		if (m_form->properties & PR_VISIBLE)
			flag |= SWP_SHOWWINDOW;
		m_pWnd->SetWindowPos(m_axform->m_view, m_pRc.left + m_size.cx, m_pRc.top + m_size.cy,
					m_pRc.Width(), m_pRc.Height(), flag);
		m_memo->SetWindowPos(m_pWnd, 0, 0, m_pRc.Width(), m_pRc.Height(), flag);
		setFont();
	}
}

void CfmMemo::ReadAll(CString& data, bool edit, bool head)
{
	data.Empty();
	if (!m_drawOnly)
		m_memo->GetWindowText(m_strR);
	data = m_strR.Left(min(m_strR.GetLength(), MAX_MEMO_TEXT));
}

void CfmMemo::ReadData(CString& data, bool edit, int size, int col, int row)
{
	ReadAll(data, edit);
}

void CfmMemo::WriteAll(CString str, bool redraw)
{
	if (!m_strR.Compare(str))
		return;

	CString	tmps = _T("");
	for (int ii = 0; ii < str.GetLength(); ii++)
		tmps += str[ii] ? str[ii] : ' ';

	m_strR = tmps;
	if (!m_drawOnly)
		m_memo->SetWindowText(m_strR);
}

void CfmMemo::WriteData(CString data, bool redraw, int col, int row)
{
	WriteAll(data);
}

void CfmMemo::SetVisible(bool visible, int col)
{
	bool	isvisible = (m_form->properties & PR_VISIBLE) ? true : false;

	if (visible)
		m_form->properties |= PR_VISIBLE;
	else
		m_form->properties &= ~PR_VISIBLE;

	if (isvisible != visible)
	{
		m_pWnd->ShowWindow(visible ? SW_SHOW : SW_HIDE);
		m_memo->ShowWindow(visible ? SW_SHOW : SW_HIDE);
		invalidateRect(&m_pRc, TRUE, true);
	}
}

void CfmMemo::SetEnable(bool enable)
{
	bool	isenable = (m_form->properties & PR_ENABLE) ? true : false;

	if (enable)
		m_form->properties |= PR_ENABLE;
	else
		m_form->properties &= ~PR_ENABLE;

	if (isenable != enable)
		m_memo->EnableWindow(enable ? TRUE : FALSE);
}

void CfmMemo::ClearData(bool selectOnly)
{
	m_strR.Empty();
	if (!m_drawOnly)
		m_memo->SetWindowText(m_strR);
}

int CfmMemo::GetLength()
{
	if (!m_drawOnly)
		m_memo->GetWindowText(m_strR);

	return m_strR.GetLength();
}

void CfmMemo::Refresh()
{
	if (m_drawOnly && (m_form->properties & PR_VISIBLE))
	{
		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&m_pRc, true);
		else
			invalidateRect(&m_pRc, false);
	}
}

bool CfmMemo::IsFocus(HWND hWnd)
{
	if ((m_pWnd && m_pWnd->GetSafeHwnd() == hWnd) || (m_memo && m_memo->GetSafeHwnd() == hWnd))
		return true;
	return false;
}

void CfmMemo::ShowControl(BOOL visible)
{
	if (m_pWnd)
		m_pWnd->ShowWindow(visible ? SW_SHOW : SW_HIDE);
	if (m_memo)
		m_memo->ShowWindow(visible ? SW_SHOW : SW_HIDE);
}

void CfmMemo::DoListBox(bool apply, bool focus)
{
	m_axform->DoSomething(doIME, this, apply ? 1 : 0);
}

void CfmMemo::setFont()
{
	if (m_drawOnly)
		return;

	int	weight = FW_NORMAL;
	bool	italic = false;

	if (m_form->fStyle == FS_ITALIC || m_form->fStyle == FS_IBOLD)
		italic = true;

	float	fwidth  = m_fWidth;
	float	fheight = m_fHeight;

	if (m_form->fStyle == FS_BOLD || m_form->fStyle == FS_IBOLD)
		weight = FW_BOLD;
	if (weight == FW_BOLD)
	{
		if (m_cfPoint < m_fPoint)
			weight = FW_MEDIUM;
	}

	if (GetSystemMetrics(SM_CXSCREEN) < 1024)
		weight = FW_MEDIUM;

	float	width  = m_hR * 100;
	float	height = m_vR * 100;

	if (abs((int)(width - height)) > 35)
	{
		if (m_hR > m_vR)
			fwidth--;
		else
			fheight--;
	}

	if (width < 50 && height > 70)
		fwidth += float(0.25);

	bool	quit = false;
	CFont*	font;
	LOGFONT	logfont;
	int	basis;

	basis = (int)(height - width);
	if (basis < 10)
		basis = (int)fheight - 1;
	else if (basis < 15)
		basis = (int)fheight - 2;
	else
	{
		if (height < width)
			basis = (int)fheight - (basis / 10);
		else
		{
			basis = (int)fwidth;
			basis *= 2;
		}
	}

	for (int ii = 1; ii <= 32; ii++)
	{
		font = new CFont();

		logfont.lfHeight         = ii * 10;
		logfont.lfWidth          = 0;
		logfont.lfEscapement     = 0;
		logfont.lfOrientation    = 0;
		logfont.lfWeight         = weight;
		logfont.lfItalic         = italic;
		logfont.lfUnderline      = false;
		logfont.lfStrikeOut      = false;
		logfont.lfCharSet        = DEFAULT_CHARSET;
		logfont.lfOutPrecision   = OUT_DEFAULT_PRECIS;
		logfont.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
		logfont.lfQuality        = DEFAULT_QUALITY;
		logfont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
		strcpy_s(logfont.lfFaceName, LF_FACESIZE, (char*)m_form->fName);

		font->CreatePointFontIndirect(&logfont);
		font->GetLogFont(&logfont);

		if (abs(logfont.lfHeight) > basis)
			quit = true;
		font->DeleteObject();
		delete font;

		if (quit)
		{
			if (ii <= 6)
				quit = false;

			break;
		}
	}

	if (quit)
		m_memo->SetFont(&logfont);
	else
	{
		logfont.lfHeight = (int)fheight;
		logfont.lfWidth  = (int)fwidth;

		m_memo->SetFont(&logfont);
	}
}
