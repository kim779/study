// fmCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "fmCtrl.h"
#include "axform.h"

#include "../../h/axisfire.h"

#define	TM_FOCUS		1226

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LRESULT CALLBACK ControlProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

/////////////////////////////////////////////////////////////////////////////
// CfmCtrl

IMPLEMENT_DYNCREATE(CfmCtrl, CfmBase)

CfmCtrl::CfmCtrl()
{
	EnableAutomation();
}

CfmCtrl::CfmCtrl(CAxisForm* axform, struct _formR* formR, int iform, bool drawOnly)
	: CfmBase(axform, formR, iform)
{
	CString	name;
	CWnd*	(APIENTRY* axCreate)(CWnd*, void*);

	m_ctrl     = NULL;
	m_instance = NULL;
	m_drawOnly = drawOnly;
	m_bottom_gap = 0;
	m_right_gap  = 0;

	if (!m_drawOnly)
	{
		if (!m_axform->DoSomething(doVERS, this, 0, (LPARAM)m_strR.operator LPCTSTR()))
		{
			m_callproc = NULL;
			m_visible  = SW_HIDE;
			return;
		}

		name.Format(_T("%s/%s/%s.dll"), LPCTSTR(m_axform->m_root), DEVDIR, LPCTSTR(m_strR));
	
		m_instance = AfxLoadLibrary(name);
		int err = GetLastError();
		if (m_instance != NULL)
		{
			axCreate = (CWnd* (APIENTRY*)(CWnd*, void*))GetProcAddress(m_instance, _T("axCreate"));
			if (axCreate != NULL)
			{
				struct	_param	param;

				if (m_form->iok == EIO_INPUT || m_form->iok == EIO_INOUT)
					param.key = MAKELONG(m_axform->m_key, m_form->size);
				else
					param.key = m_axform->m_key;
				param.name  = (char *)m_form->name;
				param.rect  = CRect(-m_pRc.Width(), -m_pRc.Height(), 0, 0);		// m_pRc
				param.fonts = (char *)m_form->fName;
				param.point = m_form->fPoint;
				param.style = m_form->fStyle;
				param.tRGB  = m_form->tRGB;
				param.pRGB  = m_form->pRGB;
				param.options = &m_axform->m_strR[m_form->vals[1]];

				m_ctrl = (*axCreate)(m_axform->m_view, &param);
				if (m_ctrl == NULL)
				{
					AfxFreeLibrary(m_instance);
					m_instance = NULL;
				}
			}
		}
	}

	m_callproc = NULL;
	m_visible  = SW_HIDE;
	if (m_ctrl)
	{
		SetWindowLong(m_ctrl->GetSafeHwnd(), GWL_USERDATA, long(this));
		HANDLE instance  = (HANDLE)GetWindowLong(m_ctrl->GetSafeHwnd(), GWL_HINSTANCE);
		FARPROC	callproc = (FARPROC)MakeProcInstance(ControlProc, instance);
		m_callproc = (FARPROC)SetWindowLong(m_ctrl->GetSafeHwnd(), GWL_WNDPROC, (LONG)callproc);
		m_visible = (m_form->properties & PR_VISIBLE) ? SW_SHOW : SW_HIDE;
		m_ctrl->ShowWindow(m_visible);
	}
}

CfmCtrl::~CfmCtrl()
{
	if (m_ctrl)
	{
		if (m_callproc)
			SetWindowLong(m_ctrl->GetSafeHwnd(), GWL_WNDPROC, (LONG)m_callproc);
		if (IsWindow(m_ctrl->GetSafeHwnd()))
			m_ctrl->SendMessage(WM_CLOSE);
		m_ctrl->Detach();
		delete m_ctrl;
	}
	if (m_instance)
		AfxFreeLibrary(m_instance);
}


void CfmCtrl::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CfmCtrl, CfmBase)
	//{{AFX_MSG_MAP(CfmCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CfmCtrl, CfmBase)
	//{{AFX_DISPATCH_MAP(CfmCtrl)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmCtrl to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {B625BCF2-8767-4EE7-A457-77B6BF363BB7}
static const IID IID_IfmCtrl =
{ 0xb625bcf2, 0x8767, 0x4ee7, { 0xa4, 0x57, 0x77, 0xb6, 0xbf, 0x36, 0x3b, 0xb7 } };

BEGIN_INTERFACE_MAP(CfmCtrl, CfmBase)
	INTERFACE_PART(CfmCtrl, IID_IfmCtrl, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfmCtrl message handlers

void CfmCtrl::SetRect(CRect rect, CRect pRc)
{
	setRect(rect);
}

void CfmCtrl::SetFont(int point, int style, CString fonts, bool resize)
{
	if (m_form->fPoint == m_axform->m_mapH->fPoint)
	{
		if (m_ctrl && IsWindow(m_ctrl->GetSafeHwnd()))
		{
			m_ctrl->SendMessage(WM_USER, MAKEWPARAM(resize ? DLL_SETFONTx : DLL_SETFONT, point),
								(LPARAM)fonts.operator LPCTSTR());
		}
	}
}

void CfmCtrl::Draw(CDC* dc)
{
//	if (!m_drawOnly && m_ctrl)
//		m_ctrl->Invalidate(FALSE);
}

void CfmCtrl::Adjust(CSize size)
{
	CfmBase::Adjust(size);
}

void CfmCtrl::Adjust(CPoint pt, float hR, float vR)
{
	CfmBase::Adjust(pt, hR, vR);

	if (m_axform->IsResizable() && (m_form->attr2 & GO_FIX))
	{
		CRect	cRc(0,0,0,0);

		m_axform->m_view->GetClientRect(&cRc);
		m_pRc.right  = cRc.right;
		if (m_bottom_gap <= 0)
		{
			int	height;

			height = m_axform->GetHeight();
			if (height > 0)
				m_bottom_gap = height - m_rect.bottom;
		}

		if (m_right_gap <= 0)
		{
			int	width;

			width = m_axform->GetWidth();
			if (width > 0)
				m_right_gap = width - m_rect.right;
		}
	}

	if (m_ctrl && IsWindow(m_ctrl->GetSafeHwnd()))
		m_ctrl->MoveWindow(&m_pRc);
}

void CfmCtrl::Resize(float hR, float vR, bool font, bool redraw)
{
	if (m_axform->IsResizable() && (m_form->attr2 & GO_FIX))
	{
		CRect	cRc(0,0,0,0);

		CfmBase::Resize(hR, vR, false);
		m_axform->m_view->GetClientRect(&cRc);
		m_pRc.right  = cRc.right - m_right_gap;
		m_pRc.bottom = cRc.bottom - m_bottom_gap;

		m_rect.left   = (int)(m_pRc.left / m_hR);
		m_rect.top    = (int)(m_pRc.top / m_vR);
		m_rect.right  = (int)(m_pRc.right / m_hR);
		m_rect.bottom = (int)(m_pRc.bottom / m_vR);
	}
	else
		CfmBase::Resize(hR, vR, font);

	if (m_ctrl && IsWindow(m_ctrl->GetSafeHwnd()))
	{
		m_ctrl->MoveWindow(&m_pRc);
		if (font && m_form->fPoint == m_axform->m_mapH->fPoint)
			m_ctrl->SendMessage(WM_USER, MAKEWPARAM(DLL_SETFONT, m_cfPoint));
	}
}

void CfmCtrl::Reload()
{
}

void CfmCtrl::SetBkColor(int rgb, int col, int row)
{
	if (m_ctrl && IsWindow(m_ctrl->GetSafeHwnd()))
		m_ctrl->SendMessage(WM_USER, MAKEWPARAM(DLL_SETPAL, 0));
}

void CfmCtrl::ReadAll(CString& data, bool edit, bool head)
{
}

void CfmCtrl::ReadData(CString& data, bool edit, int size, int col, int row)
{
	data = _T("");
	if (m_ctrl && IsWindow(m_ctrl->GetSafeHwnd()))// && (m_form->iok == EIO_INPUT || m_form->iok == EIO_INOUT))
	{
		if (size < 0)
			size = m_form->size;
		if (size > 0)
		{
			data = CString((char*)m_ctrl->SendMessage(WM_USER, MAKEWPARAM(DLL_INB, 0)), size);
		}
		else
		{
// updateXXX_20220803
			char* pch = (char*)m_ctrl->SendMessage(WM_USER, MAKEWPARAM(DLL_INB, 0));
			if (pch) data = CString(pch);
		}
	}
}

void CfmCtrl::WriteAll(CString str, bool redraw)
{
	if (m_ctrl && IsWindow(m_ctrl->GetSafeHwnd()) && str.GetLength() > 0)
		m_ctrl->SendMessage(WM_USER, MAKEWPARAM(DLL_ALERT, str.GetAt(0)), (LPARAM)str.Mid(1).operator LPCTSTR());
}

// updateXXX_20220728
void CfmCtrl::WriteData(CString data, bool redraw, int col, int row)
{
	if (m_ctrl && IsWindow(m_ctrl->GetSafeHwnd()))
	{
		if (col <= 0)
			col = data.GetLength();
		row = max(row, 0);

		// col : data size, row : user-defined key
		struct _extTHx extTHx{};

		extTHx.key  = (unsigned char)row;
		extTHx.size = col;
		extTHx.data = (char*)data.GetString();
		
		m_ctrl->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(DLL_OUBx, 0), 0), (LPARAM)&extTHx);
	}
}

void CfmCtrl::SetVisible(bool visible, int col)
{
	CfmBase::SetVisible(visible, col);
	if (m_ctrl && IsWindow(m_ctrl->GetSafeHwnd()))
	{
		m_visible = (m_form->properties & PR_VISIBLE) ? SW_SHOW : SW_HIDE;
		m_ctrl->ShowWindow(m_visible);
	}
}

bool CfmCtrl::IsVisible(int col)
{
	if (m_ctrl && IsWindow(m_ctrl->GetSafeHwnd()))
		return m_ctrl->IsWindowVisible() ? true : false;
	return 	CfmBase::IsVisible();
}

void CfmCtrl::SetEnable(bool enable)
{
	CfmBase::SetEnable(enable);
	if (m_ctrl && IsWindow(m_ctrl->GetSafeHwnd()))
		m_ctrl->EnableWindow((m_form->properties & PR_ENABLE) ? TRUE : FALSE);
}

bool CfmCtrl::IsEnable()
{
	if (m_ctrl && IsWindow(m_ctrl->GetSafeHwnd()))
		return m_ctrl->IsWindowEnabled() ? true : false;
	return 	CfmBase::IsEnable();
}

void CfmCtrl::ClearData(bool selectOnly)
{

}

void CfmCtrl::SetFCB(bool blink, int flash)
{
	if (m_ctrl && IsWindow(m_ctrl->GetSafeHwnd()))
		m_ctrl->PostMessage(WM_USER, MAKEWPARAM(DLL_SETFCB, blink ? 1 : 0), flash);
}

void CfmCtrl::SetEnum(CString text, int value, int func)
{
	if (m_ctrl && IsWindow(m_ctrl->GetSafeHwnd()))
	{
		switch (func)
		{
		case 255:
			m_ctrl->SendMessage(WM_USER, MAKEWPARAM(DLL_INIT, 0));
			break;
		case 253:
			m_ctrl->SendMessage(WM_USER, MAKEWPARAM(DLL_ALERTx, 0), (LPARAM)value);
			break;
		case 0:
			m_ctrl->SendMessage(WM_USER, MAKEWPARAM(DLL_NOTICE, 0), (LPARAM)text.operator LPCTSTR());
			break;
		default:
			if (func < 0)
				m_ctrl->PostMessage(WM_USER, MAKEWPARAM(DLL_ACCOUNT, 0));
			break;
		}
	}
}

void CfmCtrl::SetFocus(bool focus)
{
	CfmBase::SetFocus(focus);
	if (m_ctrl == NULL || !IsWindow(m_ctrl->GetSafeHwnd()))
		return;

	if (focus)
	{
		CWnd*	pWnd = CWnd::GetFocus();
		if (m_ctrl == pWnd)
			return;
		if (m_ctrl->IsChild(pWnd))
			pWnd = pWnd;
		if (!m_ctrl->IsChild(pWnd))
			m_ctrl->SetFocus();
	//	m_ctrl->SetTimer(TM_FOCUS, 1, NULL);
	}
	else if (GetFocus() == m_ctrl->GetSafeHwnd())
	{
		m_axform->m_view->SetFocus();
	}
	else if (GetParent(GetFocus()) == m_ctrl->GetSafeHwnd())
	{
		m_ctrl->SetFocus();
		m_axform->m_view->SetFocus();
	}
	else
	{
		m_axform->m_view->SetFocus();
	}
}

bool CfmCtrl::IsFocus(HWND hWnd)
{
	if (m_ctrl && IsWindow(m_ctrl->GetSafeHwnd()) && m_ctrl->GetSafeHwnd() == hWnd)
		return true;
	return false;
}

void CfmCtrl::ShowControl(BOOL visible)
{
	if (m_ctrl && IsWindow(m_ctrl->GetSafeHwnd()))
	{
		m_visible = visible ? SW_SHOW : SW_HIDE;
		m_ctrl->ShowWindow(m_visible);
	}
}

LRESULT CALLBACK ControlProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CfmCtrl* ctrl = (CfmCtrl *)GetWindowLong(hwnd, GWL_USERDATA);
	if (ctrl == NULL)
		return 0;

	CRect	rect(0,0,0,0);
	CPoint	pt(0,0);

	switch (msg)
	{
	case WM_TIMER:
		if (wParam == TM_FOCUS)
		{
			ctrl->m_ctrl->KillTimer(TM_FOCUS);
		//	ctrl->m_ctrl->SetFocus();
			if ((GetFocus() == ctrl->m_axform->m_view->GetSafeHwnd()) ||
			    (GetParent(GetFocus()) == ctrl->m_axform->m_view->GetSafeHwnd()))
			{
				ctrl->m_ctrl->SetFocus();
			}

		}
		break;
	case WM_SHOWWINDOW:
		if (wParam && ctrl->m_visible != SW_SHOW)
		{
			ctrl->m_visible = SW_SHOW;
			ctrl->m_form->properties |= PR_VISIBLE;
		}
		else if (!wParam && ctrl->m_visible != SW_HIDE)
		{
			ctrl->m_visible = SW_HIDE;
			ctrl->m_form->properties &= ~PR_VISIBLE;
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_TAB:
		case VK_RETURN:
		//	ctrl->m_axform->m_view->SetFocus();
		//	ctrl->m_axform->m_view->PostMessage(msg, wParam, lParam);
TRACE("KEY [%s][%d]\n", CString(ctrl->m_axform->m_mapH->mapN, L_MAPN), wParam);
			ctrl->m_axform->m_view->SendMessage(msg, wParam, lParam);
			ctrl->m_axform->m_view->SetFocus();
			break;
		}
		break;
	case WM_RBUTTONDOWN:
		if (ctrl->m_form->attr & FA_PROTECT)
			ctrl->m_axform->m_view->SetFocus();
		[[fallthrough]];
	case WM_RBUTTONUP:
		rect = ctrl->GetRect();
		pt = CPoint(LOWORD(lParam), HIWORD(lParam));
		pt.Offset(rect.left, rect.top);
		ctrl->m_axform->m_view->PostMessage(msg, wParam, MAKELPARAM(pt.x, pt.y));
		break;
	case WM_LBUTTONDOWN:
		if (ctrl->m_form->attr & FA_PROTECT)
		{
			ctrl->m_axform->m_view->SetFocus();
			return 0;
		}
		break;
	}

	return ::CallWindowProc((long (__stdcall*)(HWND, UINT, WPARAM, LPARAM))ctrl->m_callproc, hwnd, msg, wParam, lParam);
}
