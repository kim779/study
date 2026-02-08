// FOHistoryBtnCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "FOHistoryBtnCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FOHistoryBtnCtrl

CFOHistoryBtnCtrl::CFOHistoryBtnCtrl()
{
	m_pwndParent = NULL;
	m_pwndWizard = NULL;
	
	m_hiFOHistoryBtnDll = NULL;
	m_pWndFOHistoryBtn = NULL;

	m_nID = 0;
	m_dwType = NULL;
}

CFOHistoryBtnCtrl::~CFOHistoryBtnCtrl()
{
	if (m_pWndFOHistoryBtn)
	{
		m_pWndFOHistoryBtn->DestroyWindow();
		delete m_pWndFOHistoryBtn;
		m_pWndFOHistoryBtn = NULL;
	}

	if (m_hiFOHistoryBtnDll)
	{
		FreeLibrary(m_hiFOHistoryBtnDll);
		m_hiFOHistoryBtnDll = NULL;
	}
}


BEGIN_MESSAGE_MAP(CFOHistoryBtnCtrl, CWnd)
	//{{AFX_MSG_MAP(CFOHistoryBtnCtrl)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFOHistoryBtnCtrl message handlers

bool CFOHistoryBtnCtrl::CreateFOHistoryBtn(CWnd* pwndParent, CWnd* pwndWizard, CRect rect, UINT nID, int nKind, int nFutureKind, int nOptionKind)
{
	m_pwndParent = pwndParent;
	m_pwndWizard = pwndWizard;
	m_nID = nID;

	m_strHome = (char*)m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), NULL);


	if (!Create(NULL, NULL, WS_CHILD| WS_VISIBLE, rect, m_pwndParent, m_nID))
	{
		TRACE("CFOHistoryBtnCtrl::CreateFOHistoryBtn()\n");
		return false;
	}

	CString dir;
	dir.Format("%s\\dev\\CX_FOHISTORYBTN.dll", m_strHome);

	struct	_param	m_param;

	GetClientRect(&m_param.rect);
	m_param.name = "FOHISTORYBTN";	//ctrlName;
	m_param.fonts = "±¼¸²";
	m_param.point = 9;
	m_param.style = 0;
	m_param.tRGB = 63;
	m_param.pRGB = RGB(255, 255, 255);
	m_param.options.Format("/k%d/f%d/o%d", nKind, nFutureKind, nOptionKind);
	m_param.key = nID;	//nkey;

	m_hiFOHistoryBtnDll = LoadLibrary(dir);
	if (!m_hiFOHistoryBtnDll)
	{
		TRACE("CX_FOHISTORYBTN.dll load error !!\n");
		return false;
	}
	
	CWnd*	(APIENTRY *axCreateDLL)(CWnd*, CWnd*, CWnd*, void*);
	axCreateDLL = (CWnd* (APIENTRY *)(CWnd*, CWnd*, CWnd*, void*)) GetProcAddress(m_hiFOHistoryBtnDll, "axCreateDLL");
	if (axCreateDLL == NULL)
	{
		TRACE("CX_FOHISTORYBTN.dll axCreateDLL error !!\n");
		return false;
	}

	m_pWndFOHistoryBtn = axCreateDLL(this, m_pwndWizard, m_pwndParent, (void*)&m_param);
	if (!m_pWndFOHistoryBtn)
		return false;

	return true;
}

void CFOHistoryBtnCtrl::OnDestroy() 
{
	CWnd::OnDestroy();
}

CString	CFOHistoryBtnCtrl::GetCode()
{
	USES_CONVERSION;

	if (m_pWndFOHistoryBtn == NULL)
		return _T("");

	IDispatch* pDisp = m_pWndFOHistoryBtn->GetIDispatch(FALSE);

	if (pDisp == NULL)
		return _T("");

	VARIANT var;
	VariantInit(&var);

	CComDispatchDriver::GetProperty(pDisp, DISPID_CODE, &var);	
	return OLE2T(var.bstrVal);
}

CString	CFOHistoryBtnCtrl::GetName()
{
	USES_CONVERSION;

	if (m_pWndFOHistoryBtn == NULL)
		return _T("");

	IDispatch* pDisp = m_pWndFOHistoryBtn->GetIDispatch(FALSE);

	if (pDisp == NULL)
		return _T("");

	VARIANT var;
	VariantInit(&var);

	CComDispatchDriver::GetProperty(pDisp, DISPID_NAME, &var);	
	return OLE2T(var.bstrVal);
}

void CFOHistoryBtnCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (m_pWndFOHistoryBtn && IsWindow(m_pWndFOHistoryBtn->m_hWnd))
		m_pWndFOHistoryBtn->MoveWindow(0, 0, cx, cy);
	
}


CString	CFOHistoryBtnCtrl::GetCodeName(CString strCode)
{
	if (!m_pWndFOHistoryBtn)
		return "";

	CString (APIENTRY *axGetName)(CWnd*, CString);
	axGetName = (CString (APIENTRY*)(CWnd*, CString))GetProcAddress(m_hiFOHistoryBtnDll, "axGetName");
	if (!axGetName)
	{
		return "";
	}

	return axGetName(m_pWndFOHistoryBtn, strCode);
}

int	CFOHistoryBtnCtrl::GetCodeType(CString strCode)
{
	if (!m_pWndFOHistoryBtn)
		return 0;

	int (APIENTRY *axGetType)(CWnd*, CString);
	axGetType = (int (APIENTRY*)(CWnd*, CString))GetProcAddress(m_hiFOHistoryBtnDll, "axGetType");
	if (!axGetType)
	{
		return 0;
	}

	return axGetType(m_pWndFOHistoryBtn, strCode);
}
