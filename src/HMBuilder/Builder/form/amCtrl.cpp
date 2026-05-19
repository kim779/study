// fmCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "amCtrl.h"
#include "amform.h"

#include "../../h/axisfire.h"

#define	TM_FOCUS		1226

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LRESULT CALLBACK ControlProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

/////////////////////////////////////////////////////////////////////////////
// CamCtrl

IMPLEMENT_DYNCREATE(CamCtrl, CamBase)

CamCtrl::CamCtrl()
{
	EnableAutomation();
}

CamCtrl::CamCtrl(CAmForm* axform, struct _formR* formR, bool drawOnly)
	: CamBase(axform, formR)
{
	CString	name;
//	CWnd*	(APIENTRY* axCreate)(CWnd*, void*);

	m_ctrl     = NULL;
	m_instance = NULL;
	m_drawOnly = drawOnly;

	/*
	if (!m_drawOnly)
	{
		name.Format("%s/%s/%s.dll", m_axform->m_root, DEVDIR, m_strR);
		m_instance = AfxLoadLibrary(name);
		if (m_instance != NULL)
		{
			axCreate = (CWnd* (APIENTRY*)(CWnd*, void*))GetProcAddress(m_instance, _T("axCreate"));
			if (axCreate != NULL)
			{
				struct	_param	param;

				if (m_form->iok == EIO_INPUT || m_form->iok == EIO_INOUT)
					param.key   = MAKELONG(m_axform->m_key, m_form->size);
				else
					param.key   = m_axform->m_key;
				param.name  = m_name;
				param.rect  = CRect(-m_pRc.Width(), -m_pRc.Height(), 0, 0);		// m_pRc
				param.fonts = m_fonts;
				param.point = m_form->fPoint;
				param.style = m_form->fStyle;
				param.tRGB  = m_form->tRGB;
				param.pRGB  = m_form->pRGB;
				//param.options = &m_axform->m_strR[m_form->vals[1]];

				m_ctrl = (*axCreate)(m_axform->m_view, &param);
				if (m_ctrl == NULL)
				{
					AfxFreeLibrary(m_instance);
					m_instance = NULL;
				}
			}
		}
	}
	*/

	m_callproc = NULL;
	m_visible  = SW_HIDE;
	if (m_ctrl)
	{
		/*
		SetWindowLong(m_ctrl->GetSafeHwnd(), GWL_USERDATA, long(this));
		HANDLE instance  = (HANDLE)GetWindowLong(m_ctrl->GetSafeHwnd(), GWL_HINSTANCE);
		FARPROC	callproc = (FARPROC)MakeProcInstance(ControlProc, instance);
		m_callproc = (FARPROC)SetWindowLong(m_ctrl->GetSafeHwnd(), GWL_WNDPROC, (LONG)callproc);
		*/

		SetWindowLongPtr(m_ctrl->GetSafeHwnd(), GWLP_USERDATA, LONG_PTR(this));
		HANDLE instance  = (HANDLE)GetWindowLong(m_ctrl->GetSafeHwnd(), GWLP_HINSTANCE);
		FARPROC	callproc = (FARPROC)MakeProcInstance(ControlProc, instance);
		m_callproc = (FARPROC)SetWindowLongPtr(m_ctrl->GetSafeHwnd(), GWLP_WNDPROC, (LONG_PTR)callproc);

		m_visible = (m_form->properties & PR_VISIBLE) ? SW_SHOW : SW_HIDE;
		m_ctrl->ShowWindow(m_visible);
	}
}

CamCtrl::~CamCtrl()
{
	/*
	if (m_ctrl)
	{
		if (m_callproc)
			SetWindowLongPtr(m_ctrl->GetSafeHwnd(), GWLP_WNDPROC, (LONG_PTR)m_callproc);
		if (IsWindow(m_ctrl->GetSafeHwnd()))
			m_ctrl->SendMessage(WM_CLOSE);
		m_ctrl->Detach();
		delete m_ctrl;
	}
	if (m_instance)
		AfxFreeLibrary(m_instance);
		*/

}


void CamCtrl::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CamCtrl, CamBase)
	//{{AFX_MSG_MAP(CamCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CamCtrl, CamBase)
	//{{AFX_DISPATCH_MAP(CamCtrl)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmCtrl to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {B625BCF2-8767-4EE7-A457-77B6BF363BB7}
static const IID IID_IfmCtrl =
{ 0xb625bcf2, 0x8767, 0x4ee7, { 0xa4, 0x57, 0x77, 0xb6, 0xbf, 0x36, 0x3b, 0xb7 } };

BEGIN_INTERFACE_MAP(CamCtrl, CamBase)
	INTERFACE_PART(CamCtrl, IID_IfmCtrl, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CamCtrl message handlers

void CamCtrl::SetFont(int point, int style, CString fonts, bool resize)
{
	if (m_form->fPoint == m_axform->m_mapH->fPoint)
	{
		if (m_ctrl)
		{
			m_ctrl->SendMessage(WM_USER, MAKEWPARAM(resize ? DLL_SETFONTx : DLL_SETFONT, point),
								(LPARAM)fonts.operator LPCTSTR());
		}
	}
}

void CamCtrl::Draw(CDC* dc)
{
//	if (!m_drawOnly && m_ctrl)
//		m_ctrl->Invalidate(FALSE);

//	Graphics g(dc->GetSafeHwnd());
//	m_tRc = m_pRc;
	Graphics g(dc->GetSafeHdc());
//	int	ndc = dc->SaveDC();
//	CBrush* brush;
	

//		brush = setBrush(dc);
//		dc->FillRect(m_pRc, brush);
		g.FillRectangle(setgBrush(), RECTG(m_pRc));
	
}

void CamCtrl::Adjust(CSize size)
{
	CamBase::Adjust(size);
}

void CamCtrl::Adjust(CPoint pt, float hR, float vR)
{
	CamBase::Adjust(pt, hR, vR);
	if (m_ctrl)
		m_ctrl->MoveWindow(&m_pRc);
}

void CamCtrl::Resize(float hR, float vR, bool font, bool redraw)
{
	CamBase::Resize(hR, vR, font);
	if (m_ctrl)
		m_ctrl->MoveWindow(&m_pRc);
}

void CamCtrl::Reload()
{
}

void CamCtrl::SetBkColor(int rgb, int col, int row)
{
	if (m_ctrl)
		m_ctrl->SendMessage(WM_USER, MAKEWPARAM(DLL_SETPAL, 0));
}

void CamCtrl::ReadAll(CString& data, bool edit, bool head)
{
}

void CamCtrl::ReadData(CString& data, bool edit, int col, int row)
{
	if (m_ctrl && (m_form->iok == EIO_INPUT || m_form->iok == EIO_INOUT))
	{
		if (m_form->size)
			data = CString((char *)m_ctrl->SendMessage(WM_USER, MAKEWPARAM(DLL_INB, 0)), m_form->size);
		else
			data = (char *)m_ctrl->SendMessage(WM_USER, MAKEWPARAM(DLL_INB, 0));
	}
	else
		data = _T("");
}

void CamCtrl::WriteAll(CString str, bool redraw)
{
//	if (m_ctrl && str.GetLength() > 0)
//		m_ctrl->SendMessage(WM_USER, MAKEWPARAM(DLL_ALERT, str.GetAt(0)), (LPARAM)str.Mid(1).operator LPCTSTR());
}

void CamCtrl::WriteData(CString data, bool redraw, int col, int row)
{
	if (m_ctrl)
	{
		int	size = data.GetLength();
		m_ctrl->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(DLL_OUB, size >> 16), size), (LPARAM)data.operator LPCTSTR());
	}

/*	if (m_ctrl)
	{
		if (row <= 0)
			m_ctrl->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(DLL_OUB, col >> 16), col), (LPARAM)data.operator LPCTSTR());
		else
		{
			char*	datb;
			struct	_extTH*	extTH;

			datb = new char[L_extTH+col];
			extTH = (struct _extTH *)datb;
			extTH->key  = row;
			extTH->size = col;
			CopyMemory(&datb[L_extTH], (char *)data.operator LPCTSTR(), col);
			m_ctrl->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(DLL_OUBx, 0), 0), (LPARAM)datb);
			delete[] datb;
		}
	}
*/
}

void CamCtrl::SetVisible(bool visible, int col)
{
	CamBase::SetVisible(visible, col);
	if (m_ctrl)
	{
		m_visible = (m_form->properties & PR_VISIBLE) ? SW_SHOW : SW_HIDE;
		m_ctrl->ShowWindow(m_visible);
	}
}

bool CamCtrl::IsVisible(int col)
{
	if (m_ctrl)
		return m_ctrl->IsWindowVisible() ? true : false;
	return 	CamBase::IsVisible();
}

void CamCtrl::SetEnable(bool enable)
{
	CamBase::SetEnable(enable);
	if (m_ctrl)
		m_ctrl->EnableWindow((m_form->properties & PR_ENABLE) ? TRUE : FALSE);
}

bool CamCtrl::IsEnable()
{
	if (m_ctrl)
		return m_ctrl->IsWindowEnabled() ? true : false;
	return 	CamBase::IsEnable();
}

void CamCtrl::ClearData(bool selectOnly)
{

}

void CamCtrl::SetFCB(bool blink, int flash)
{
	if (m_ctrl)
		m_ctrl->PostMessage(WM_USER, MAKEWPARAM(DLL_SETFCB, blink ? 1 : 0), flash);
}

void CamCtrl::SetEnum(CString text, int value, int func)
{
	if (m_ctrl && IsWindow(m_ctrl->GetSafeHwnd()))
	{
		switch (func)
		{
		case 253:
			m_ctrl->SendMessage(WM_USER, MAKEWPARAM(DLL_ALERTx, 0), (LPARAM)value);
			break;
		case 0:
			m_ctrl->SendMessage(WM_USER, MAKEWPARAM(DLL_NOTICE, 0), (LPARAM)text.operator LPCTSTR());
			break;
			break;
		default:
			if (func < 0)
				m_ctrl->PostMessage(WM_USER, MAKEWPARAM(DLL_ACCOUNT, 0));
			break;
		}
	}
}

void CamCtrl::SetFocus(bool focus)
{
	CamBase::SetFocus(focus);
	if (m_ctrl == NULL)
		return;

	if (focus)
		m_ctrl->SetTimer(TM_FOCUS, 1, NULL);
		//m_ctrl->SetFocus();
	else if (GetFocus() == m_ctrl->GetSafeHwnd())
		m_axform->m_view->SetFocus();
	else if (GetParent(GetFocus()) == m_ctrl->GetSafeHwnd())
	{
		m_ctrl->SetFocus();
		m_axform->m_view->SetFocus();
	}
}

bool CamCtrl::IsFocus(HWND hWnd)
{
//	if (m_ctrl && m_ctrl->GetSafeHwnd() == hWnd)
	if (m_ctrl && m_ctrl->GetSafeHwnd() == hWnd && (m_form->iok == EIO_INPUT || m_form->iok == EIO_INOUT) && !(m_form->attr & FA_PROTECT))
		return true;
	return false;
}

void CamCtrl::ShowControl(BOOL visible)
{
	if (m_ctrl)
	{
		m_visible = visible ? SW_SHOW : SW_HIDE;
		m_ctrl->ShowWindow(m_visible);
	}
}

LRESULT CALLBACK ControlProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CamCtrl* ctrl = (CamCtrl *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (ctrl == NULL)
		return 0;

	CRect	rect;
	CPoint	pt;

	switch (msg)
	{
	case WM_TIMER:
		if (wParam == TM_FOCUS)
		{
			ctrl->m_ctrl->KillTimer(TM_FOCUS);
			ctrl->m_ctrl->SetFocus();
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
			ctrl->m_axform->m_view->SetFocus();
			ctrl->m_axform->m_view->PostMessage(msg, wParam, lParam);
			break;
		}
		break;
	case WM_RBUTTONDOWN:
		if (ctrl->m_form->attr & FA_PROTECT)
			ctrl->m_axform->m_view->SetFocus();
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

	return ::CallWindowProc((LRESULT (__stdcall *)(HWND, UINT, WPARAM, LPARAM))ctrl->m_callproc, hwnd, msg, wParam, lParam);
}
