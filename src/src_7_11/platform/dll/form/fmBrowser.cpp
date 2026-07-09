// fmBrowser.cpp : implementation file
//

#include "stdafx.h"
#include "fmBrowser.h"
#include "axform.h"
#include "browser.h"
//#include "browserX.h"

#include <memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	FORM_BROWSER		20001
#define	BMP_BROWSER_ACT		"axbrowser_act.bmp"
#define	BMP_BROWSER_DEACT	"axbrowser_deact.bmp"
#define	BMP_SIZE		20
#define	TOOL_SIZE		22
#define	TOOL_COUNT		5	// toolbar Button Count

/////////////////////////////////////////////////////////////////////////////
// CfmBrowser

IMPLEMENT_DYNCREATE(CfmBrowser, CfmBase)

CfmBrowser::CfmBrowser()
{
	EnableAutomation();
	m_runningX = false;
}

CfmBrowser::CfmBrowser(CAxisForm* axform, struct _formR* formR, int iform, bool drawOnly)
	: CfmBase(axform, formR, iform)
{
	m_drawOnly = drawOnly;

	m_hot     = -1;
	m_file    = false;
	m_hastool = (m_form->properties & PR_CHECKED) ? true : false;
	m_browser = (Cbrowser *) nullptr;
//	m_browserX = (CbrowserX *) nullptr;

	CRect	tRc = m_pRc;
	m_toolRc.SetRectEmpty();
	if (m_hastool)
	{
		m_toolRc = m_pRc;
		m_toolRc.bottom = m_toolRc.top + TOOL_SIZE;
		tRc.top = m_toolRc.bottom;
		loadBitmap();
	}

	m_runningX = false;

	if (!m_drawOnly)
	{
		if (m_strR.Find("axis://") == 0)
		{
			CString	string;
			string.Format(_T("%s\\%s"), LPCTSTR(m_axform->m_root), LPCTSTR(m_strR.Mid(7)));
			m_strR = string;
		}

//		m_browserX = new CbrowserX;
//		m_runningX = m_browserX->CreateAsync(WS_CHILD | WS_BORDER, tRc, m_axform->m_view, AFX_IDW_PANE_FIRST,
//				[this]()
//				{
////					m_browserX->SetParentView(m_axform->m_view);
//					m_browserX->DisablePopups();
//
//					m_browserX->ShowWindow((m_form->properties & PR_VISIBLE) ? SW_SHOW : SW_HIDE);
//					m_browserX->xNavigate(m_strR, nullptr);
//				});

		if (!m_runningX)
		{
			m_browser = new Cbrowser;
			m_browser->Create(NULL, NULL, WS_CHILD|WS_BORDER, tRc, m_axform->m_view, AFX_IDW_PANE_FIRST);
			m_browser->ShowWindow((m_form->properties & PR_VISIBLE) ? SW_SHOW : SW_HIDE);

			m_browser->xNavigate(m_strR);
		}
	}
}

CfmBrowser::~CfmBrowser()
{
	if (!m_drawOnly && isBrowser())
	{
		CWnd* pWnd = getBrowser();
		if (IsWindow(pWnd->GetSafeHwnd()))
		{
			pWnd->KillTimer(TM_WAIT);
			pWnd->ShowWindow(SW_HIDE);
		}
	}

	m_hImage.DeleteImageList();
	m_cImage.DeleteImageList();

	if (m_file)
		DeleteFile(m_strR);
}

void CfmBrowser::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CfmBrowser, CfmBase)
	//{{AFX_MSG_MAP(CfmBrowser)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CfmBrowser, CfmBase)
	//{{AFX_DISPATCH_MAP(CfmBrowser)
	DISP_PROPERTY_EX(CfmBrowser, "Text", _getText, _setText, VT_BSTR)
	DISP_PROPERTY_EX(CfmBrowser, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_FUNCTION(CfmBrowser, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmBrowser, "Navigate", _Navigate, VT_EMPTY, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmBrowser to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {3BEC7DED-7805-4A75-9068-1F9DC080D83E}
static const IID IID_IfmBrowser =
{ 0x3bec7ded, 0x7805, 0x4a75, { 0x90, 0x68, 0x1f, 0x9d, 0xc0, 0x80, 0xd8, 0x3e } };

BEGIN_INTERFACE_MAP(CfmBrowser, CfmBase)
	INTERFACE_PART(CfmBrowser, IID_IfmBrowser, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfmBrowser message handlers

void CfmBrowser::_Refresh() 
{
	Refresh();
}

void CfmBrowser::_Navigate(LPCTSTR url) 
{
	WriteData(url);
}

BSTR CfmBrowser::_getText() 
{
	CString strResult;
	
	ReadData(strResult);
	return strResult.AllocSysString();
}

void CfmBrowser::_setText(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue);
}

BOOL CfmBrowser::_getVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CfmBrowser::_setVisible(BOOL bNewValue) 
{
	SetVisible(bNewValue ? true : false);
}

void CfmBrowser::Adjust(CSize size)
{
	CfmBase::Adjust(size);

	CRect	tRc = m_pRc;
	if (m_hastool)
	{
		m_toolRc = m_pRc;
		m_toolRc.bottom = m_toolRc.top + TOOL_SIZE;
		tRc.top = m_toolRc.bottom;
	}

	if (!m_drawOnly)
	{
		UINT	flag = SWP_NOZORDER;
		if (m_form->properties & PR_VISIBLE)
			flag |= SWP_SHOWWINDOW;
		getBrowser()->SetWindowPos(m_axform->m_view, tRc.left + m_size.cx, tRc.top + m_size.cy,
					tRc.Width(), tRc.Height(), flag);
	}
}

void CfmBrowser::Adjust(CPoint pt, float hR, float vR)
{
	CfmBase::Adjust(pt, hR, vR);

	CRect	tRc = m_pRc;
	if (m_hastool)
	{
		m_toolRc = m_pRc;
		m_toolRc.bottom = m_toolRc.top + TOOL_SIZE;
		tRc.top = m_toolRc.bottom;
	}

	if (!m_drawOnly)
	{
		UINT	flag = SWP_NOZORDER;
		if (m_form->properties & PR_VISIBLE)
			flag |= SWP_SHOWWINDOW;
		getBrowser()->SetWindowPos(m_axform->m_view, tRc.left, tRc.top, tRc.Width(), tRc.Height(), flag);
	}
}

void CfmBrowser::Resize(float hR, float vR, bool font, bool redraw)
{
	m_hR = hR;
	m_vR = vR;

	CRect	tRc, iRc; // , oldRc;

//	oldRc = m_pRc;
	tRc   = calcRect(m_rect, hR, vR);
	if (m_pRc.EqualRect(tRc))
		return;

	iRc.left   = min(tRc.left, m_pRc.left);
	iRc.top    = min(tRc.top, m_pRc.top);
	iRc.right  = max(tRc.right, m_pRc.right);
	iRc.bottom = max(tRc.bottom, m_pRc.bottom);

	m_pRc = tRc;
	if (m_hastool)
	{
		m_toolRc = m_pRc;
		m_toolRc.bottom = m_toolRc.top + TOOL_SIZE;
		tRc.top = m_toolRc.bottom;
	}

	if (redraw)
		invalidateRect(&iRc, false);

	if (!m_drawOnly)
	{
		UINT	flag = SWP_NOZORDER;
		if (m_form->properties & PR_VISIBLE)
			flag |= SWP_SHOWWINDOW;
		getBrowser()->SetWindowPos(m_axform->m_view, tRc.left, tRc.top, tRc.Width(), tRc.Height(), flag);
	}
}

void CfmBrowser::Reload()
{
	CfmBase::Reload();

	CRect	tRc = m_pRc;
	if (m_hastool)
	{
		m_toolRc = m_pRc;
		m_toolRc.bottom = m_toolRc.top + TOOL_SIZE;
		tRc.top = m_toolRc.bottom;
	}

	if (!m_drawOnly)
	{
		UINT	flag = SWP_NOZORDER;
		if (m_form->properties & PR_VISIBLE)
			flag |= SWP_SHOWWINDOW;
		getBrowser()->SetWindowPos(m_axform->m_view, tRc.left, tRc.top, tRc.Width(), tRc.Height(), flag);
	}
}

void CfmBrowser::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE))
		return;

	CRect	tRc(0,0,0,0);
	CBrush*	brush = (CBrush*)0;;

	if (m_drawOnly)
	{
		brush = setBrush(dc, getRGB(cWHITE));
		dc->FillRect(m_pRc, brush);
		setTextColor(dc, getRGB(cBLACK));
		dc->DrawText(m_strR, m_pRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_NOPREFIX);
	}
	else if (m_hastool)
	{
		brush = setBrush(dc, getRGB(cBASIC));
		dc->FillRect(&m_toolRc, brush);
		for (int ii = 0; ii < TOOL_COUNT; ii++)
		{
			tRc = getRect(ii);
			if (ii == m_hot)
			{
				if (m_hImage.GetSafeHandle() && ii < m_hImage.GetImageCount())
				{
					m_hImage.DrawIndirect(dc, ii, CPoint(tRc.left+1, tRc.top+1),
								CSize(BMP_SIZE, BMP_SIZE), CPoint(0, 0),
								ILD_NORMAL, SRCCOPY, CLR_NONE, CLR_DEFAULT);
					drawEdge(dc, tRc, m_mousedown ? EDGE_SUNKEN : EDGE_RAISED, BF_RECT);
				}
			}
			else if (m_cImage.GetSafeHandle() && ii < m_cImage.GetImageCount())
			{
				m_cImage.DrawIndirect(dc, ii, CPoint(tRc.left+1, tRc.top+1),
							CSize(BMP_SIZE, BMP_SIZE), CPoint(0, 0),
							ILD_NORMAL, SRCCOPY, CLR_NONE, CLR_DEFAULT);
			}
		}
		drawEdge(dc, m_toolRc, EDGE_RAISED, BF_RECT);
	}
//	drawEdge(dc, m_pRc, EDGE_SUNKEN, BF_RECT);
}

void CfmBrowser::OnLButton(bool down, CPoint pt, int& result)
{
	CRect	tRc(0,0,0,0);

	result = RC_NOP;
	m_mousedown = down;
	if (down)
		m_axform->m_cursor = pt;
	if (!m_hastool)
		return;

	int hot = getIndex(pt);
	if (m_hot != hot && m_hot >= 0)
	{
		tRc = getRect(m_hot);
		invalidateRect(&tRc, false);
	}
	if (m_mousedown)
		m_hot = hot;
	else if (!m_mousedown && (m_hot != hot))
		m_hot = -1;

	if (m_hot >= 0)
	{
		switch (m_hot)
		{
		case 0:		// back
//			if (m_runningX)
//				m_browserX->GoBack();
//			else	
				m_browser->GoBack();
			break;
		case 1:		// forward
//			if (m_runningX)
//				m_browserX->GoForward();
//			else
				m_browser->GoForward();
			break;
		case 2:		// stop
//			if (m_runningX)
//				m_browserX->Stop();
//			else
				m_browser->Stop();
			break;
		case 3:		// refresh
//			if (m_runningX)
//				m_browserX->Reload();
//			else
				m_browser->Refresh();
			break;
		case 4:		// print
//			if (m_runningX)
//				m_browserX->PrintDocument();
//			else
				m_browser->SendMessage(WM_COMMAND, ID_FILE_PRINT, 0);
			break;
		default:
			break;
		}
		tRc = getRect(m_hot);
		invalidateRect(&tRc, false);
	}
}

int CfmBrowser::OnDrag(CPoint pt, int& result)
{
	CRect	tRc(0,0,0,0);
	int	hot = m_hot;

	CfmBase::OnDrag(pt, result);
	if (!m_hastool)
		return -1;

	m_hot  = getIndex(pt);
	if (hot != m_hot)
	{
		if (hot >= 0)
		{
			tRc = getRect(hot);
			invalidateRect(&tRc, false);
		}
		if (m_mousedown)
			m_hot = -1;
		if (m_hot >= 0)
		{
			tRc = getRect(m_hot);
			invalidateRect(&tRc, false);
		}
	}
	return -1;
}

void CfmBrowser::WriteAll(CString str, bool redraw)
{
	if (m_file)
	{
		m_file = false;
		DeleteFile(m_strR);
	}

	if (str.Find("text://") == 0)
	{
		CString	tmps;
		CFile	file;

		tmps.Format(_T("%s\\%s\\%s%d.html"), LPCTSTR(m_axform->m_root), USRDIR, LPCTSTR(m_form->name), int(GetTickCount64()));
		if (file.Open(tmps, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite))
		{
			m_file = true;
			m_strR = tmps;
			tmps = str.Mid(7);

			file.Write(tmps, tmps.GetLength());
			file.Close();

			if (!m_drawOnly && isBrowser())
			{
//				if (m_runningX)
//					m_browserX->xNavigate(m_strR, nullptr);
//				else
					m_browser->xNavigate(m_strR);
			}
			return;
		}
	}
	else if (str.Find("axis://") == 0)
	{
		CString	tmps;
		tmps.Format(_T("%s\\%s"), LPCTSTR(m_axform->m_root), LPCTSTR(str.Mid(7)));
		str = tmps;
	}

	m_strR = str;
	if (!m_drawOnly && isBrowser())
	{
//		if (m_runningX)
//			m_browserX->xNavigate(m_strR, nullptr);
//		else
			m_browser->xNavigate(m_strR);
	}
}

void CfmBrowser::WriteData(CString data, bool redraw, int col, int row)
{
	if (m_file)
	{
		m_file = false;
		DeleteFile(m_strR);
	}

	m_strR = data;
	if (m_strR.Find("axis://") == 0)
	{
		CString	string;
		string.Format(_T("%s\\%s"), LPCTSTR(m_axform->m_root), LPCTSTR(m_strR.Mid(7)));
		m_strR = string;
	}

	if (!m_drawOnly && isBrowser())
	{
//		if (m_runningX)
//			m_browserX->xNavigate(m_strR, nullptr);
//		else
			m_browser->xNavigate(m_strR);
	}
}

void CfmBrowser::SetVisible(bool visible, int col)
{
	bool	isvisible = (m_form->properties & PR_VISIBLE) ? true : false;

	if (visible)
		m_form->properties |= PR_VISIBLE;
	else
		m_form->properties &= ~PR_VISIBLE;

	if (isvisible != visible)
	{
		getBrowser()->ShowWindow(visible ? SW_SHOW : SW_HIDE);
		invalidateRect(&m_pRc, true, true);
	}
}

bool CfmBrowser::IsFocus(HWND hWnd)
{
	if (isBrowser() && getBrowser()->GetSafeHwnd() == hWnd)
		return true;
	return false;
}

void CfmBrowser::ShowControl(BOOL visible)
{
	if (isBrowser())
		getBrowser()->ShowWindow(visible ? SW_SHOW : SW_HIDE);
}

void CfmBrowser::loadBitmap()
{
	CString	path;
	CBitmap	bitmap;
	HBITMAP	hbitmap;

	path.Format(_T("%s\\%s\\%s"), m_axform->m_root.GetString(), IMAGEDIR, BMP_BROWSER_ACT);
	hbitmap = (HBITMAP) LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (hbitmap)
	{
		m_hImage.DeleteImageList();
		bitmap.Attach(hbitmap);
		m_hImage.Create(BMP_SIZE, BMP_SIZE, ILC_COLORDDB|ILC_MASK, TOOL_COUNT, 1);
		m_hImage.Add(&bitmap, RGB(255, 0, 255));
		m_hImage.SetBkColor(CLR_NONE);
		bitmap.DeleteObject();
		DeleteObject(hbitmap);
	}

	path.Format(_T("%s\\%s\\%s"), m_axform->m_root.GetString(), IMAGEDIR, BMP_BROWSER_DEACT);
	hbitmap = (HBITMAP) LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (hbitmap)
	{
		m_cImage.DeleteImageList();
		bitmap.Attach(hbitmap);
		m_cImage.Create(BMP_SIZE, BMP_SIZE, ILC_COLORDDB|ILC_MASK, TOOL_COUNT, 1);
		m_cImage.Add(&bitmap, RGB(255, 0, 255));
		m_cImage.SetBkColor(CLR_NONE);
		bitmap.DeleteObject();
		DeleteObject(hbitmap);
	}
}

int CfmBrowser::getIndex(CPoint pt)
{
	CRect	tRc(0,0,0,0);
	for (int ii = 0; ii < TOOL_COUNT; ii++)
	{
		tRc = getRect(ii);
		if (tRc.PtInRect(pt))
			return ii;
	}

	return -1;
}

CRect CfmBrowser::getRect(int index)
{
	CRect	tRc = m_toolRc;
	tRc.left += (index*TOOL_SIZE);
	tRc.right = tRc.left + TOOL_SIZE;

	return tRc;
}
