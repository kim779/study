// OptionEx.cpp : implementation file
//

#include "stdafx.h"
#include "OptionEx.h"
#include "../../h/AxisFire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DISPID_CURCODE	0x00000001

/////////////////////////////////////////////////////////////////////////////
// COptionEx

COptionEx::COptionEx(CWnd* pView, CWnd* parent, CString home)
{
	m_pView = pView;
	m_pParent = parent;
	m_home = home;
	
	m_hOptionExDll = NULL;
	m_pOptionExCtrl = NULL;
}

COptionEx::~COptionEx()
{
	if (m_pOptionExCtrl)
	{
		m_pOptionExCtrl->DestroyWindow();
		delete m_pOptionExCtrl;
	}
	if (m_hOptionExDll)
		FreeLibrary(m_hOptionExDll);
}


BEGIN_MESSAGE_MAP(COptionEx, CWnd)
	//{{AFX_MSG_MAP(COptionEx)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COptionEx message handlers

void COptionEx::createOptionExCtrl(CString ctrlName, int nkey, DWORD bColor)
{
	CString dir;
	dir.Format("%s\\dev\\cx_OptionEx.dll", m_home);
	
	struct	_param	m_param;
	
	GetClientRect(&m_param.rect);
	m_param.name = ctrlName;
	m_param.fonts = "±¼¸²";
	m_param.point = 9;
	m_param.style = 0;
	m_param.tRGB = 63;
	m_param.pRGB = bColor;
	m_param.options = "";
	m_param.key = nkey;
	
	m_hOptionExDll = LoadLibrary(dir);
	if (!m_hOptionExDll)
	{
		TRACE("cx_OptionEx.dll load error !!\n");
		return;
	}
	
	CWnd*	(APIENTRY *axCreate)(CWnd*, void*);
	axCreate = (CWnd* (APIENTRY *)(CWnd*, void*)) GetProcAddress(m_hOptionExDll, "axCreateDll");
	if (axCreate == NULL)
	{
		TRACE("cx_OptionEx.dll axCtrl Create error !!\n");
		return;
	}
	
	m_pOptionExCtrl = axCreate(this, (void*)&m_param);
}

void COptionEx::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
}

CString COptionEx::GetCode()
{
	USES_CONVERSION_CONST_CONVERT;
	
	if (m_pOptionExCtrl == NULL)
		return _T("");
	
	IDispatch* pDisp = m_pOptionExCtrl->GetIDispatch(FALSE);
	
	if (pDisp == NULL)
		return _T("");
	
	VARIANT var;
	VariantInit(&var);
	
	CComDispatchDriver::GetProperty(pDisp, DISPID_CURCODE, &var);

	return (LPCTSTR)(_bstr_t)var;
}

void COptionEx::SetCode(CString strCode)
{
	USES_CONVERSION_CONST_CONVERT;
	
	if (m_pOptionExCtrl == NULL)
		return;
	
	IDispatch* pDisp = m_pOptionExCtrl->GetIDispatch(FALSE);
	
	if (pDisp == NULL)
		return;
		
	VARIANT var;
	VariantInit(&var);

	var.vt = VT_BSTR;
	var.bstrVal = strCode.AllocSysString();

	CComDispatchDriver::PutProperty(pDisp, DISPID_CURCODE, &var);	
}

long COptionEx::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case variantDLL:
		if (HIWORD(wParam) == guideCC)
		{
			CString guide = (char*)lParam;
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(DLL_GUIDE, 0), strlen((char *) lParam)), lParam);
			return 0;
		}
		break;
	case eventDLL:
		if (HIBYTE(HIWORD(wParam)) == evOnChange)	// Set Code
		{
			CString strData = (char*)lParam;
			
			return m_pParent->SendMessage(WM_USER + 1, MAKEWPARAM(200, HIWORD(wParam)), (LONG)(LPCTSTR)strData);
		}
	}

	return m_pView->SendMessage(WM_USER, wParam, lParam);
}

void COptionEx::ChangeFont(int ipoint, LPCTSTR lszname)
{
	CRect rc;
	GetClientRect(&rc);
	m_pOptionExCtrl->MoveWindow(rc);
	m_pOptionExCtrl->SendMessage(WM_USER, MAKEWPARAM(DLL_SETFONT, ipoint), (LPARAM)lszname);
}