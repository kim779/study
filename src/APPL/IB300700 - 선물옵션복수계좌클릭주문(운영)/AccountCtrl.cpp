#include "stdafx.h"
#include "IB300700.h"
#include "AccountCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAccountCtrl::CAccountCtrl(CWnd *m_pParent, LPCSTR type, int key)
: CAxisExt(m_pParent)
{
	m_hInst = NULL;
	m_pCtrl = NULL;
	m_type = type;
	m_key = key;
}

CAccountCtrl::~CAccountCtrl()
{
	if (m_pCtrl) 
	{
		m_pCtrl->DestroyWindow();
		delete m_pCtrl;
		m_pCtrl = NULL;
	}
	if (m_hInst)
		FreeLibrary(m_hInst);
}


BEGIN_MESSAGE_MAP(CAccountCtrl, CWnd)
	//{{AFX_MSG_MAP(CAccountCtrl)
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()
	
void CAccountCtrl::CreateControl()
{
	if (m_hInst) return;

	CString dllPath = CString(Variant(homeCC, "")) + CString("\\dev\\CX_ACCOUNT.DLL");
	
	m_hInst = LoadLibrary((LPCSTR)dllPath);
	if (!m_hInst)
	{
		ErrorBox("계좌컨트롤 생성 실패");
		return;
	}

	CWnd* (APIENTRY *axCreate)(CWnd *, void *);
	axCreate = (CWnd*(APIENTRY*)(CWnd*,void*))GetProcAddress(m_hInst, "axCreateDLL");
	if (!axCreate)
	{
		ErrorBox("계좌컨트롤 생성 실패");
		return;
	}

	struct _param param;
	GetClientRect(&param.rect);
	
	param.name  = m_type;
	param.fonts = "굴림체";
	param.point = 9;
	param.style = 0;
	param.tRGB  = 63;
	param.pRGB  = GetIndexColor(7);
	param.options = "";
	param.key   = m_key;

	m_pCtrl = axCreate(this, (LPVOID)&param);
}

void CAccountCtrl::ErrorBox( LPCSTR msg )
{
	MessageBox(msg, "IBK투자증권", MB_OK|MB_ICONERROR);
}

LRESULT CAccountCtrl::OnUser( WPARAM wParam, LPARAM lParam )
{
	switch(LOWORD(wParam))
	{
	case variantDLL:
		if (HIWORD(wParam) == guideCC)
		{
			MessageBox((LPCSTR)lParam);
			return GetParent()->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(DLL_GUIDE,0), strlen((LPCSTR)lParam)), lParam );
		}
		break;
	case eventDLL:
		if (HIBYTE(HIWORD(wParam)) == evOnChange)
		{
			CString data = GetNumber() + "\t" + GetName() + "\t" + GetPassword();
			return GetParent()->SendMessage(WM_APP_SIG, WP_ACCN_CHANGE, (LPARAM)(LPCSTR)data);
		}
		break;
	}
	return m_pParent->SendMessage(WM_USER, wParam, lParam);
}

CString CAccountCtrl::GetNumber()
{
	return GetCtrlProperty("Data");
}

CString CAccountCtrl::GetName()
{
	return GetCtrlProperty("Name");
}

CString CAccountCtrl::GetPassword()
{
	return GetCtrlProperty("Password");
}

CString CAccountCtrl::GetCtrlProperty( LPCSTR prop_name )
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

void CAccountCtrl::ChangeFont( LPCSTR font, int point )
{
	m_pCtrl->SendMessage(WM_USER, MAKEWPARAM(DLL_SETFONT, point), (LPARAM)font);

	if (m_pCtrl)
	{
		CRect rc;
		GetClientRect(rc);
		m_pCtrl->MoveWindow(rc, TRUE);
	}
}

void CAccountCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	if (m_pCtrl)
	{
		CRect rc;
		GetClientRect(&rc);
		m_pCtrl->MoveWindow(&rc, TRUE);
	}
}

int CAccountCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CreateControl();

	return 0;
}

void CAccountCtrl::Trigger( LPCSTR accno )
{
	if (m_pCtrl)
		m_pCtrl->SendMessage( WM_USER, MAKEWPARAM(MAKEWORD(DLL_OUB, 0), strlen(accno)), (LPARAM)accno);
}
