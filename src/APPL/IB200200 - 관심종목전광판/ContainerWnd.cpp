// ContainerWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ContainerWnd.h"
#include "../H/axisfire.h"
//#include "sharemsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContainerWnd


CContainerWnd::CContainerWnd()
{
	m_hinstanceDll	= NULL;
	m_pwndControl	= NULL;

	m_pwndParent	= NULL;
}

CContainerWnd::~CContainerWnd()
{
	if (m_pwndControl)
	{
		m_pwndControl->DestroyWindow();
		delete m_pwndControl;
	}
	if (m_hinstanceDll)
	{
		FreeLibrary(m_hinstanceDll);
	}
}


BEGIN_MESSAGE_MAP(CContainerWnd, CWnd)
	//{{AFX_MSG_MAP(CContainerWnd)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_USER + 1, OnMaketPickerMessage)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContainerWnd message handlers

LRESULT CContainerWnd::OnMaketPickerMessage(WPARAM wParam, LPARAM lParam)
{
	CString slog;
	switch (LOWORD(wParam))
	{
	case 100: //내부 마켓컨트롤 클릭시 메시지
	{
		CString data = (char*)lParam;
		slog.Format("%s", (char*)lParam);
		//   컨트롤이름\tCXMARKETPICKER\t\t\KRX
	//	AfxMessageBox("cx_marketpicker 로부터 메시지 \n " + slog);
		std::vector<CString> vdata;
		AxStd::SplitSub(vdata, data, "\t");
		if (vdata[0] == "xxInter")
		{
			if (vdata[3] == "KRX")
			{
				GetParent()->SendMessage(WM_MANAGE, MK_MARKETEVENT, 1);
			}
			else if (vdata[3] == "NXT")
			{
				GetParent()->SendMessage(WM_MANAGE, MK_MARKETEVENT, 2);
			}
			else
			{
				GetParent()->SendMessage(WM_MANAGE, MK_MARKETEVENT, 3);
			}
		}

	}
	break;
	default:
		break;
	}
	return 0;
}

LRESULT CContainerWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case formDLL:
		break;
	default:
		break;
	}
	
	if (m_pwndParent)
	{
		if (m_pwndParent->GetSafeHwnd())
		{
			return m_pwndParent->SendMessage(WM_USER, wParam, lParam);
		}
	}

	return 0;
}

BOOL CContainerWnd::CreateControl(CString strRoot,
					CString strDllName,
					CString strSymbolName,
					CRect rectControl,
					CString strOption,
					CString strFontName,
					int iFontPoint,
					int iStyle,
					int iKey,
					COLORREF rgbFG,
					COLORREF rgbBG)
{
	CString strDllDir;
	strDllDir.Format("%s\\dev\\%s.dll", strRoot, strDllName);
	m_hinstanceDll = LoadLibrary(strDllDir);
	if (!m_hinstanceDll)
	{
		TRACE("dll load error !!\n");
		return FALSE;
	}

	struct _param param;

	param.name = strSymbolName;
	param.fonts = strFontName;
	param.point = iFontPoint;
	param.style = iStyle;
	param.tRGB = rgbFG;
	param.pRGB = rgbBG;
	param.options = strOption;	//.Format("/k2/i2btn.bmp");
	param.rect = rectControl;
	
	CWnd*	(APIENTRY *axCreate)(CWnd*, void*);
	axCreate = (CWnd* (APIENTRY *)(CWnd*, void*)) GetProcAddress(m_hinstanceDll, "axCreate");
	if (axCreate == NULL)
	{
		TRACE("axCtrl Create is not found !!\n");
		return FALSE;
	}

	m_pwndControl = axCreate(this, (void*)&param);  
	if (m_pwndControl == NULL)
	{
		TRACE("m_pwndControl is NULL !!\n");
		return FALSE;
	}

	m_pwndControl->SendMessage(WM_USER + 2, MAKEWPARAM(100, 0), (LPARAM)this);
	PostMessage(WM_SIZE);
	m_pwndControl->SetFocus();
	return TRUE;
}

void CContainerWnd::SetParent(CWnd* pwndParent)
{
	m_pwndParent = pwndParent;
}

CString CContainerWnd::GetCtrlProperty( LPCSTR prop_name )
{
	USES_CONVERSION_CONST;

	if (!m_pwndControl) return "";
	if (!m_pwndControl->GetSafeHwnd()) return "";
	
	IDispatch *pDisp = m_pwndControl->GetIDispatch(FALSE);
	if (!pDisp) return "";
	
	_variant_t var;
	CComDispatchDriver driver(pDisp);
	driver.GetPropertyByName(_bstr_t(prop_name), &var);
	return (LPCSTR)(_bstr_t)var;
}

void CContainerWnd::SetCtrlProperty( _bstr_t prop_name, _variant_t &var )
{
	if (!m_pwndControl) return;
	if (!m_pwndControl->GetSafeHwnd()) return;
	
	IDispatch *pDisp = m_pwndControl->GetIDispatch(FALSE);
	if (!pDisp) return;
	
	CComDispatchDriver driver(pDisp);
	driver.PutPropertyByName(prop_name, &var);
}

void CContainerWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.

	m_pwndControl->Invalidate();
}

void CContainerWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_pwndControl->SendMessage(WM_LBUTTONDOWN, 0, 0);
	CWnd::OnLButtonDown(nFlags, point);
}

void CContainerWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_pwndControl->GetSafeHwnd())
	{
		CRect rc;
		GetClientRect(rc);
		m_pwndControl->SetWindowPos(&CWnd::wndTop, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);
	}
}

void CContainerWnd::OnMove(int x, int y)
{
	CWnd::OnMove(x, y);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}