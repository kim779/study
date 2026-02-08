// AccountCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "AccountCtrl.h"
#include "../../h/AxisFire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define USES_CONVERSION_CONST int _convert = 0; (_convert); const UINT _acp = ATL::_AtlGetConversionACP() /*CP_THREAD_ACP*/; (_acp); LPCWSTR _lpw = NULL; (_lpw); LPCSTR _lpa = NULL; (_lpa)

/////////////////////////////////////////////////////////////////////////////
// CAccountCtrl

CAccountCtrl::CAccountCtrl(CWnd* pView, CWnd* parent, CString home)
{
	m_pView = pView;
	m_pParent = parent;
	m_home = home;

	m_hAccountDll = NULL;
	m_pAccountCtrl = NULL;
}

CAccountCtrl::~CAccountCtrl()
{
	if (m_pAccountCtrl)
	{
		m_pAccountCtrl->DestroyWindow();
		delete m_pAccountCtrl;
	}
	if (m_hAccountDll)
		FreeLibrary(m_hAccountDll);
}


BEGIN_MESSAGE_MAP(CAccountCtrl, CWnd)
	//{{AFX_MSG_MAP(CAccountCtrl)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_USER + 1, OnMessage1)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAccountCtrl message handlers

void CAccountCtrl::createAccountCtrl(CString ctrlName, int nkey, DWORD bColor)
{

	CString dir;
	dir.Format("%s\\dev\\cx_Account.dll", m_home);

	struct	_param	m_param;

	GetClientRect(&m_param.rect);
	m_param.name = ctrlName;
	m_param.fonts = "±¼¸²Ã¼";
	m_param.point = 9;
	m_param.style = 0;
	m_param.tRGB = 63; 
	m_param.pRGB = bColor;
	m_param.options = "";
	m_param.key = nkey;

	m_hAccountDll = LoadLibrary(dir);
	if (!m_hAccountDll)
	{
		TRACE("cx_Account.dll load error !!\n");
		return;
	}
	
	CWnd*	(APIENTRY *axCreate)(CWnd*, void*);
	axCreate = (CWnd* (APIENTRY *)(CWnd*, void*)) GetProcAddress(m_hAccountDll, "axCreateDLL");
	if (axCreate == NULL)
	{
		TRACE("cx_Account.dll axCtrl Create error !!\n");
		return;
	}
	m_pAccountCtrl = axCreate(this, (void*)&m_param);
}

void CAccountCtrl::OnDestroy() 
{
	CWnd::OnDestroy();
}

long CAccountCtrl::OnMessage(WPARAM wParam, LPARAM lParam)
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
		if (HIBYTE(HIWORD(wParam)) == evOnChange)	// get name
		{
			CString strData;
			strData += GetAccNo();
			strData += _T("\t");
			strData += GetAccName();
			strData += _T("\t");
			strData += GetPswd();

			return m_pParent->SendMessage(WM_USER + 1, MAKEWPARAM(10, HIWORD(wParam)), (LONG)(LPCTSTR)strData);
		}
	}

	return m_pView->SendMessage(WM_USER, wParam, lParam);
}

long CAccountCtrl::OnMessage1(WPARAM wParam, LPARAM lParam)
{
	CString tmpS = (char*)lParam;

	WPARAM wParamNew;
	wParamNew = MAKEWPARAM(100, HIWORD(wParam));

	return m_pParent->SendMessage(WM_USER + 1, wParamNew, lParam);
}

CString	CAccountCtrl::GetAccNo()
{
	USES_CONVERSION_CONST;
	
	if (m_pAccountCtrl == NULL)
		return _T("");
	
	IDispatch* pDisp = m_pAccountCtrl->GetIDispatch(FALSE);
	
	if (pDisp == NULL)
		return _T("");
	
	VARIANT var;
	VariantInit(&var);
	
	CComDispatchDriver driver(pDisp);
	
	driver.GetPropertyByName(L"Data", &var);
	
	return OLE2T(var.bstrVal);
}

CString CAccountCtrl::GetAccName()
{
	USES_CONVERSION_CONST;
	
	if (m_pAccountCtrl == NULL)
		return _T("");
	
	IDispatch* pDisp = m_pAccountCtrl->GetIDispatch(FALSE);
	
	if (pDisp == NULL)
		return _T("");
	
	VARIANT var;
	VariantInit(&var);
	
	CComDispatchDriver driver(pDisp);
	
	driver.GetPropertyByName(L"Name", &var);
	
	return OLE2T(var.bstrVal);
}

CString CAccountCtrl::GetPswd()
{
	USES_CONVERSION_CONST;
	
	if (m_pAccountCtrl == NULL)
		return _T("");
	
	IDispatch* pDisp = m_pAccountCtrl->GetIDispatch(FALSE);
	
	if (pDisp == NULL)
		return _T("");
	
	VARIANT var;
	VariantInit(&var);
	
	CComDispatchDriver driver(pDisp);
	
	driver.GetPropertyByName(L"Password", &var);
	
	return OLE2T(var.bstrVal);
}

CString CAccountCtrl::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

void CAccountCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);

	m_pAccountCtrl->SendMessage(WM_USER + 1, 4, 0);
}

void CAccountCtrl::TriggerAccount(CString acc)
{
	m_pAccountCtrl->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(DLL_OUB, 0), acc.GetLength()), (LPARAM)acc.operator LPCSTR());
}