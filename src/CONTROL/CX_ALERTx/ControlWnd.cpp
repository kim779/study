// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "cx_alertx.h"
#include "ControlWnd.h"
#include "../../h/memdc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlWnd

CControlWnd::CControlWnd(CWnd* pParent, _param* pParam)
: m_pParent(pParent)
{
	EnableAutomation();
	
	m_key = pParam->key;
	m_name = CString(pParam->name, pParam->name.GetLength());
}

CControlWnd::~CControlWnd()
{
}

void CControlWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_PROPERTY_NOTIFY(CControlWnd, "strCode", m_strCode, OnStrCodeChanged, VT_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {4F07B760-F92F-4016-8FFA-6E73092055B1}
static const IID IID_IControlWnd =
{ 0x4f07b760, 0xf92f, 0x4016, { 0x8f, 0xfa, 0x6e, 0x73, 0x9, 0x20, 0x55, 0xb1 } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers

long CControlWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_ALERTx:
		{
			CString sMkty, strCode;
			struct _alertR* alertR = (struct _alertR*)lParam;

			strCode = alertR->code;
			sMkty = strCode.Mid(1, 2);

			m_strCode.Empty();
			if (strCode.GetLength() == 8 && ("11" >= sMkty && sMkty <= "59" || "B0" >= sMkty && sMkty <= "ZZ" || "0A" >= sMkty && sMkty <= "0Z" || 
				"1A" >= sMkty && sMkty <= "1Z" || "2A" >= sMkty && sMkty <= "2Z" || "3A" >= sMkty && sMkty <= "3Z" || "4A" >= sMkty && sMkty <= "4Z" ||
				"5A" >= sMkty && sMkty <= "5Z" || "6A" >= sMkty && sMkty <= "6Z" || "7A" >= sMkty && sMkty <= "7Z" || "8A" >= sMkty && sMkty <= "8Z" ||
				"9A" >= sMkty && sMkty <= "9Z"))	//주식선물	
			{
				m_strCode = strCode;
				CString m_slog;
				m_slog.Format("[cx_alertx][SF][%s]<%d> m_strCode=[%s]", __FUNCTION__, __LINE__, m_strCode);
				OutputDebugString(m_slog);
			}
			else if (strCode.GetLength() == 7 && strCode.GetAt(0) == 'A')
			{
				m_strCode = strCode;
			}

			if (m_strCode)
			{
				m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_key, evOnChange/*Change*/)), 
						(LPARAM)m_name.GetString());
			}
		}
		break;
	default:break;
	}
	return 0;
}

void CControlWnd::OnStrCodeChanged() 
{
	// TODO: Add notification handler code

}
