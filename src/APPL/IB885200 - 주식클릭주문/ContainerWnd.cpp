// ContainerWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ContainerWnd.h"
#include "../../h/AxisFire.h"

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
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContainerWnd message handlers

LRESULT CContainerWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case formDLL:
		{
			CString str = (char*)lParam;
			const int iPos = str.Find('\t');
			str = str.Mid(iPos + 1);
			if (m_strPreText.IsEmpty() == FALSE)
			{
				str = m_strPreText + '\t' + str;
			}
			
			GetParent()->SendMessage(WM_USER + 200, NULL, (LPARAM)str.operator LPCTSTR());
			return 0;
		}
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

	return TRUE;
}

void CContainerWnd::BtnClick(CString strPreText)
{
	m_strPreText = strPreText;
	if (m_pwndControl)
	{
		if (m_pwndControl->GetSafeHwnd())
		{
			m_pwndControl->SendMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKELONG(0, 0));
			m_pwndControl->SendMessage(WM_LBUTTONUP, MK_LBUTTON, MAKELONG(0, 0));
		}
	}
}

void CContainerWnd::SetParent(CWnd* pwndParent)
{
	m_pwndParent = pwndParent;
}

void CContainerWnd::SetValues(CString strValue)
{
	m_pwndControl->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(DLL_OUB, 0), strValue.GetLength()), (long)strValue.operator LPCTSTR());
}

void CContainerWnd::SetKind(int iKind)
{
	void (APIENTRY *SetKind)(CWnd*, int);
	SetKind = (void (APIENTRY *)(CWnd*, int)) GetProcAddress(m_hinstanceDll, "SetKind");

	if (SetKind == NULL)
	{
		TRACE("SetKind is not found !!\n");
		return;
	}

	SetKind(m_pwndControl, iKind);
}

void CContainerWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{

	CWnd::OnLButtonDblClk(nFlags, point);
}
