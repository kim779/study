// CodeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "IB300600.h"
#include "CodeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCodeCtrl

CCodeCtrl::CCodeCtrl(CWnd *pParent, LPCSTR type, int key)
: CAxisExt(pParent)
{
	m_type = type;
	m_key = key;
	m_hInst = NULL;
	m_pCtrl = NULL;
}

CCodeCtrl::~CCodeCtrl()
{
	if (m_pCtrl)
	{
		m_pCtrl->DestroyWindow();
		delete m_pCtrl;
	}
	if (m_hInst) 
	{
		FreeLibrary(m_hInst);
	}
}


BEGIN_MESSAGE_MAP(CCodeCtrl, CWnd)
	//{{AFX_MSG_MAP(CCodeCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		ON_WM_CREATE()
		ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()

void CCodeCtrl::CreateControl()
{
	if (m_hInst) return;
	
	CString dllPath = CString(Variant(homeCC, "")) + CString("\\dev\\CX_CODECTRL.DLL");
	
	m_hInst = LoadLibrary((LPCSTR)dllPath);
	if (!m_hInst)
	{
		ErrorBox("종목컨트롤 생성 실패1");
		return;
	}
	
	CWnd* (APIENTRY *axCreate)(CWnd *, void *);
	axCreate = (CWnd*(APIENTRY*)(CWnd*,void*))GetProcAddress(m_hInst, "axCreate");
	if (!axCreate)
	{
		ErrorBox("종목컨트롤 생성 실패2");
		return;
	}
	
	struct _param param;
	GetClientRect(&param.rect);
	param.rect.right = param.rect.left + CODECTRL_WIDTH;
	param.rect.bottom = param.rect.top + 18;
	
	param.name  = m_type;
	param.fonts = "굴림체";
	param.point = 9;
	param.style = 0;
	param.tRGB  = 63;
	param.pRGB  = GetIndexColor(7);
	param.options = "/u 4 /k 검색.BMP /l AXCOMBO.BMP /p 3 /s 0 /o True /q False";
	param.key   = m_key;
	
	m_pCtrl = axCreate(this, (LPVOID)&param);
}

void CCodeCtrl::ErrorBox( LPCSTR msg )
{
	MessageBox(msg, "IBK투자증권", MB_OK|MB_ICONERROR);
}

LRESULT CCodeCtrl::OnUser( WPARAM wParam, LPARAM lParam )
{
	switch(LOWORD(wParam))
	{
	case variantDLL:
		if (HIWORD(wParam) == guideCC)
			return GetParent()->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(DLL_GUIDE,0), strlen((LPCSTR)lParam)), lParam );
		break;
	case eventDLL:
		if (HIBYTE(HIWORD(wParam)) == evOnChange)
		{
			CString val = GetCode() + "\t" + GetName();
			GetParent()->SendMessage(WM_APP_SIG, WP_CODE_CHANGE, (LPARAM)(LPCSTR)val);
		}
		break;
	}
	return m_pParent->SendMessage(WM_USER, wParam, lParam);
}

CString CCodeCtrl::GetCtrlProperty( LPCSTR prop_name )
{
	USES_CONVERSION;
	if (!m_pCtrl) return "";
	if (!m_pCtrl->GetSafeHwnd()) return "";
	
	IDispatch *pDisp = m_pCtrl->GetIDispatch(FALSE);
	if (!pDisp) return "";
	
	_variant_t var;
	CComDispatchDriver driver(pDisp);
	driver.GetPropertyByName(_bstr_t(prop_name), &var);
	
	return (LPCSTR)(_bstr_t)var;
}

void CCodeCtrl::SetCtrlProperty( _bstr_t prop_name, _variant_t &var )
{
	if (!m_pCtrl) return;
	if (!m_pCtrl->GetSafeHwnd()) return;
	
	IDispatch *pDisp = m_pCtrl->GetIDispatch(FALSE);
	if (!pDisp) return;
	
	CComDispatchDriver driver(pDisp);
	driver.PutPropertyByName(prop_name, &var);
}

void CCodeCtrl::ChangeFont( LPCSTR font, int point )
{
	m_pCtrl->SendMessage(WM_USER, MAKEWPARAM(DLL_SETFONT, point), (LPARAM)font);
	
	if (m_pCtrl)
	{
		CRect rc;
		GetClientRect(rc);
		m_pCtrl->MoveWindow(rc, TRUE);
	}
}

void CCodeCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (m_pCtrl)
	{
		CRect rc;
		GetClientRect(&rc);
		m_pCtrl->MoveWindow(&rc, TRUE);
	}
}

int CCodeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CreateControl();
	
	return 0;
}

CString CCodeCtrl::GetName()
{
	if (!m_pCtrl) return "";
	if (!m_pCtrl->GetSafeHwnd()) return "";
	
	IDispatch *pDisp = m_pCtrl->GetIDispatch(FALSE);
	if (!pDisp) return "";
	
	_variant_t rval;
	CComDispatchDriver driver(pDisp);
	
	HRESULT hr = driver.Invoke0(L"GetName", &rval);
	
	return (LPCSTR)(_bstr_t)rval;
}

/////////////////////////////////////////////////////////////////////////////
// CCodeCtrl message handlers
