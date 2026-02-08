// ContainerWnd.cpp : implementation file
//

#include "stdafx.h"

#include "ContainerWnd.h"
//#include "../../h/AxisFire.h"
#include "../H/axisfire.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <atlcom.h>
#define USES_CONVERSION_CONST const int _convert = 0; (_convert); const UINT _acp = ATL::_AtlGetConversionACP() /*CP_THREAD_ACP*/; (_acp); LPCWSTR _lpw = NULL; (_lpw); LPCSTR _lpa = NULL; (_lpa)

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
	ON_WM_MOVE()
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
			GetParent()->SendMessage(WM_USER + 99, wParam, lParam);
		}
		break;
	}
	return 0;
}

LRESULT CContainerWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case formDLL:
		{
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

	m_pwndControl->SendMessage(WM_USER + 2, MAKEWPARAM(100, 0), (LPARAM)this);
	m_pwndControl->SetFocus();
	return TRUE;
}

void CContainerWnd::SetParent(CWnd* pwndParent)
{
	m_pwndParent = pwndParent;
}

short CContainerWnd::IsNxtAble(LPCSTR code)
{
	CString strCode;
	strCode.Format("%s", code);
	IDispatch* pDisp = m_pwndControl->GetIDispatch(FALSE);
	if (!pDisp) return -1;

	// DISPID 구하기
	DISPID dispid;
	OLECHAR* methodName = L"IsNXTAble";
	HRESULT hr = pDisp->GetIDsOfNames(IID_NULL, &methodName, 1, LOCALE_USER_DEFAULT, &dispid);
	if (FAILED(hr)) return -1;

	// 인자 구성 (1개)
	CComVariant varParam(strCode); // BSTR 인자

	DISPPARAMS dispParams = {};
	dispParams.rgvarg = &varParam; // 역순 주의! 하나면 그대로
	dispParams.cArgs = 1;

	VARIANT varResult;
	VariantInit(&varResult);

	// 메서드 호출
	hr = pDisp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD,
		&dispParams, &varResult, nullptr, nullptr);
	if (FAILED(hr)) return -1;

	// 결과가 SHORT 타입일 경우
	if (varResult.vt == VT_I2)
		return varResult.iVal;
	else
		return -1; // 예상과 다른 타입일 경우
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

	if (m_pwndControl)
	{
		CRect rc;
		GetWindowRect(rc);
	
		m_slog.Format("[%s]<%d> containter 위치이동 cx=[%d] cy=[%d] rc.left = [%d] rc.top =[%d]  W=[%d] H=[%d] ", 
			__FUNCTION__, __LINE__, cx, cy , rc.left, rc.top, rc.Width(), rc.Height());
		OutputDebugString(m_slog);

		GetClientRect(rc);
		m_pwndControl->MoveWindow(rc);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CContainerWnd::OnMove(int x, int y)
{
	CWnd::OnMove(x, y);

	if (m_pwndControl)
	{
		CRect rec;
		GetWindowRect(&rec);

		m_slog.Format("[%s]<%d> containter 위치이동 x=[%d] y=[%d] rec.left = [%d] rec.top =[%d] W=[%d] H=[%d]  ",
			__FUNCTION__, __LINE__, x, y, rec.left, rec.top, rec.Width(), rec.Height());

		OutputDebugString(m_slog);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
