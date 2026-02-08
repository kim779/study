// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "Notify.h"
#include "message.h"
#include "../../h/axisvar.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define USES_CONVERSION_CONST int _convert = 0; (_convert); const UINT _acp = ATL::_AtlGetConversionACP() /*CP_THREAD_ACP*/; (_acp); LPCWSTR _lpw = NULL; (_lpw); LPCSTR _lpa = NULL; (_lpa)
#define USES_CONVERSION_CONST_ALL const int _convert = 0; (_convert); const UINT _acp = ATL::_AtlGetConversionACP() /*CP_THREAD_ACP*/; (_acp); LPCWSTR _lpw = NULL; (_lpw); LPCSTR _lpa = NULL; (_lpa)



CNotify::CNotify()
{
	m_hInst = NULL;
	m_Wcontrol = NULL;
	//m_Icontrol = NULL;
}

CNotify::~CNotify()
{
}

BEGIN_MESSAGE_MAP(CNotify, CWnd)
	//{{AFX_MSG_MAP(CNotify)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnSend)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNotify message handlers

BOOL CNotify::CreateNotify(CWnd* parent, CString root)
{
	m_parent = parent;
	m_root   = root;

	return CWnd::Create(NULL, "notify_window", WS_CHILD, CRect(0, 0, 0, 0), parent, 5002);
}

int CNotify::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	AfxEnableControlContainer();

	struct	_param	param;
	CString	path; path.Format("%s/%s/%s", m_root, DEVDIR, "cx_notify.DLL");

	param.name	= "NOTIFY";
	param.fonts	= "굴림";
	param.point	= 9;
	param.style	= 0;
	param.tRGB	= 232;
	param.pRGB	= 192;
	param.options = "/edit1|00|01|02|03|04|05|11|06|09"; //2012.05.15 KSJ  현재가, 평가손익 추가함.
		// 선물옵션(1)|종목코드(00)|종목명(01)|구분(02)|잔고수량(03)|매도가능수량(04)|평균가(05)|매입금액(11)
		/*
		m_OrgList.InsertItem(0, "00.종목코드");
		m_OrgList.InsertItem(1, "01.종목명");
		m_OrgList.InsertItem(2, "02.구분");
		m_OrgList.InsertItem(3, "03.잔고수량");
		m_OrgList.InsertItem(4, "04.매도가능수량");
		m_OrgList.InsertItem(5, "05.평균가/정산가");
		m_OrgList.InsertItem(6, "06.현재가");
		m_OrgList.InsertItem(7, "07.대비");
		m_OrgList.InsertItem(8, "08.평가금액");
		m_OrgList.InsertItem(9, "09.평가손익");
		m_OrgList.InsertItem(10, "10.수익률");
		m_OrgList.InsertItem(11, "11.매입금액");
		*/
	GetClientRect(&param.rect);

	m_hInst = LoadLibrary(path);
	if (!m_hInst)
	{
		TRACE("cx_notify control load error....\n");
		return -1;
	}
	

	axCreate = (CWnd* (APIENTRY *)(CWnd*, void*)) GetProcAddress(m_hInst, "axCreate");
	if (axCreate == NULL)
	{
		TRACE("cx_notify axCreate proc error...\n");
		return -1;
	}

	m_Wcontrol = axCreate(this, (void *) &param);
	//m_Icontrol = new ICxNotify(m_Wcontrol->GetIDispatch(TRUE));	
	return 0;
}

void CNotify::OnDestroy() 
{
	CWnd::OnDestroy();
	
	if (m_Wcontrol != NULL)
	{
		m_Wcontrol->DestroyWindow();  delete m_Wcontrol;
		//m_Icontrol->DetachDispatch(); delete m_Icontrol;
	}
	if (m_hInst != NULL) FreeLibrary(m_hInst);
}

void CNotify::OnClose() 
{
	if (m_Wcontrol != NULL)
	{
		m_Wcontrol->DestroyWindow(); delete m_Wcontrol;
	}
	if (m_hInst != NULL) FreeLibrary(m_hInst);
	
	DestroyWindow(); CWnd::OnClose();
}

CString CNotify::GetSetAccount()
{
	if (!m_Wcontrol) return "";
	if (!m_Wcontrol->GetSafeHwnd()) return "";

	USES_CONVERSION_CONST;

	IDispatch* pDisp = m_Wcontrol->GetIDispatch(FALSE);
	if (pDisp == NULL)
		return "";
	
	CComDispatchDriver driver(pDisp);
	
	//	m_Icontrol->SetSetAccount(acc);
	VARIANT var;
	VariantInit(&var);
		
	driver.GetPropertyByName(L"SetAccount", &var);
	if (var.vt!=VT_BSTR) return "";

	return OLE2T(var.bstrVal);
}

void CNotify::SetSetAccount(LPCTSTR accn)
{
	if (!m_Wcontrol) return;
	if (!m_Wcontrol->GetSafeHwnd()) return;

	USES_CONVERSION_CONST_ALL;

	IDispatch* pDisp = m_Wcontrol->GetIDispatch(FALSE);
	if (pDisp == NULL)
		return;
	
	CComDispatchDriver driver(pDisp);
	
	//	m_Icontrol->SetSetAccount(acc);
	VARIANT var;
	VariantInit(&var);
	
	var.vt = VT_BSTR;
	var.bstrVal = (BSTR)accn;
	
	driver.PutPropertyByName(L"SetAccount", &var);
}

CString CNotify::GetDataList()
{
	if (!m_Wcontrol) return "";
	if (!m_Wcontrol->GetSafeHwnd()) return "";
	
	USES_CONVERSION_CONST;

	IDispatch* pDisp = m_Wcontrol->GetIDispatch(FALSE);
	if (pDisp == NULL)
		return "";
	
	CComDispatchDriver driver(pDisp);
	
	//	m_Icontrol->SetSetAccount(acc);
	VARIANT var;
	VariantInit(&var);
	
	driver.GetPropertyByName(L"DataList", &var);
	if (var.vt!=VT_BSTR) return "";
	
	return OLE2T(var.bstrVal);
}

void CNotify::SetDataList(LPCTSTR dat)
{
	if (!m_Wcontrol) return;
	if (!m_Wcontrol->GetSafeHwnd()) return;
	
	USES_CONVERSION_CONST_ALL;

	IDispatch* pDisp = m_Wcontrol->GetIDispatch(FALSE);
	if (pDisp == NULL)
		return;
	
	CComDispatchDriver driver(pDisp);
	
	//	m_Icontrol->SetSetAccount(acc);
	VARIANT var;
	VariantInit(&var);
	
	var.vt = VT_BSTR;
	var.bstrVal = (BSTR)dat;
	
	driver.PutPropertyByName(L"DataList", &var);
}

CString CNotify::GetFlag()
{
	if (!m_Wcontrol) return "";
	if (!m_Wcontrol->GetSafeHwnd()) return "";
	
	USES_CONVERSION_CONST;

	IDispatch* pDisp = m_Wcontrol->GetIDispatch(FALSE);
	if (pDisp == NULL)
		return "";
	
	CComDispatchDriver driver(pDisp);
	
	//	m_Icontrol->SetSetAccount(acc);
	VARIANT var;
	VariantInit(&var);
	
	driver.GetPropertyByName(L"Flag", &var);
	if (var.vt!=VT_BSTR) return "";
	
	return OLE2T(var.bstrVal);
}

void CNotify::SetFlag(LPCTSTR flag)
{
	if (!m_Wcontrol) return;
	if (!m_Wcontrol->GetSafeHwnd()) return;
	
	USES_CONVERSION_CONST_ALL;

	IDispatch* pDisp = m_Wcontrol->GetIDispatch(FALSE);
	if (pDisp == NULL)
		return;
	
	CComDispatchDriver driver(pDisp);
	
	//	m_Icontrol->SetSetAccount(acc);
	VARIANT var;
	VariantInit(&var);
	
	var.vt = VT_BSTR;
	var.bstrVal = (BSTR)flag;
	
	driver.PutPropertyByName(L"Flag", &var);
}

void CNotify::Send(LPCTSTR sAccn, LPCTSTR sPswd)
{
	if (!m_Wcontrol) return;
	if (!m_Wcontrol->GetSafeHwnd()) return;

	IDispatch* pDisp = m_Wcontrol->GetIDispatch(FALSE);
	if (pDisp == NULL)
		return;
	
	CComDispatchDriver driver(pDisp);
	
	_variant_t varArgs[4] = { L"0" , L"0", sPswd, sAccn };
	
	const HRESULT hr = driver.InvokeN(L"Send", &varArgs[0], 4);
	if (FAILED(hr))
		TRACE("CNotifyCtrl.Send is failed. HR[%X]", hr);
}

void CNotify::AllAccnSend(LPCTSTR sUserID, LPCTSTR sPswd, LPCTSTR sCode)
{
	if (!m_Wcontrol) return;
	if (!m_Wcontrol->GetSafeHwnd()) return;
	
	IDispatch* pDisp = m_Wcontrol->GetIDispatch(FALSE);
	if (pDisp == NULL)
		return;
	
	CComDispatchDriver driver(pDisp);
	
	_variant_t varArgs[3] = { sCode, sPswd, sUserID };
	
	const HRESULT hr = driver.InvokeN(L"AllAccnSend", &varArgs[0], 3);
	if (FAILED(hr))
		TRACE("CNotifyCtrl.AllAccnSend is failed. HR[%X]", hr);
}

void CNotify::Clear()
{
	if (!m_Wcontrol) return;
	if (!m_Wcontrol->GetSafeHwnd()) return;

	IDispatch* pDisp = m_Wcontrol->GetIDispatch(FALSE);
	if (pDisp == NULL)
		return;
	
	CComDispatchDriver driver(pDisp);
		
	const HRESULT hr = driver.Invoke0(L"Clear");
	if (FAILED(hr))
		TRACE("CNotifyCtrl.AllAccnSend is failed. HR[%X]", hr);
}

void CNotify::AddReminData(LPCTSTR sAccn, LPCTSTR sCode, LPCTSTR sRowData)
{
	if (!m_Wcontrol) return;
	if (!m_Wcontrol->GetSafeHwnd()) return;

	IDispatch* pDisp = m_Wcontrol->GetIDispatch(FALSE);
	if (pDisp == NULL)
		return;
	
	CComDispatchDriver driver(pDisp);
		
	_variant_t varArgs[3] = { sRowData, sCode, sAccn };
	
	const HRESULT hr = driver.InvokeN(L"AddReminData", &varArgs[0], 3);
	if (FAILED(hr))
		TRACE("CNotifyCtrl.AddReminData is failed. HR[%X]", hr);
}

LONG CNotify::OnSend(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case variantDLL:
		return m_pView->SendMessage(WM_USER, wParam, lParam);
		break;
	case eventDLL:
		return m_parent->SendMessage(WM_USER + 1, MAKEWPARAM(MSG_NOTIFY, 0), lParam);
		break;
	default:
		TRACE("CNotify.OnSend = [0x%x][%d]\n", LOWORD(wParam), HIWORD(wParam));
		break;
	}

	return 0;
}