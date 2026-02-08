// EXTCTRLCtl.cpp : Implementation of the CEXTCTRLCtrl ActiveX Control class.

#include "stdafx.h"
#include "CX_CURSORMAP.h"
#include "EXTCTRLCtl.h"
#include "EXTCTRLPpg.h"

#include "../../h/fxCommonx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	WM_CURSORMAP	(WM_USER+503)		// mouse hand : view¿¡ ÁöÁ¤

//////////////////////////////////////////////////////////////////////////
// WM_CURSORMAP LOWORD(wParam) define
#define	SCM_SETCURSORMAP	1
#define	SCM_DELETECURSORMAP	2
#define	SCM_DELETEALLCURSORMAP	3

#define	WM_INITEXTCONTROL	WM_USER + 990

[[gsl::suppress(26409)]]
IMPLEMENT_DYNCREATE(CEXTCTRLCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CEXTCTRLCtrl, COleControl)
	//{{AFX_MSG_MAP(CEXTCTRLCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_MESSAGE(WM_INITEXTCONTROL, OnInitExtControl)
	ON_MESSAGE(WM_USER, OnWizard)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CEXTCTRLCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CEXTCTRLCtrl)
	DISP_FUNCTION(CEXTCTRLCtrl, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CEXTCTRLCtrl, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CEXTCTRLCtrl, "SetParams", SetParams, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CEXTCTRLCtrl, "IsInvokeAllowed", IsInvokeAllowed, VT_BOOL, VTS_I4)
	DISP_FUNCTION(CEXTCTRLCtrl, "SetCursorMap", SetCursorMap, VT_EMPTY, VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CEXTCTRLCtrl, "DeleteAllCursorMap", DeleteAllCursorMap, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CEXTCTRLCtrl, "DeleteCursorMap", DeleteCursorMap, VT_EMPTY, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CEXTCTRLCtrl, COleControl)
	//{{AFX_EVENT_MAP(CEXTCTRLCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CEXTCTRLCtrl, 1)
	PROPPAGEID(CEXTCTRLPropPage::guid)
END_PROPPAGEIDS(CEXTCTRLCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CEXTCTRLCtrl, "CXCURSORMAP.CXCURSORMAPCtrl.1",
 0xaa0f54a8, 0x970e, 0x451a, 0x97, 0x3e, 0xb7, 0x4d, 0x98, 0xf9, 0xcf, 0x16)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CEXTCTRLCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DEXTCTRL =
{ 0x5adc603a, 0xf412, 0x41b4, {0x99, 0x64, 0xf4, 0xbd, 0x0a, 0x15, 0x3c, 0x92}};
const IID BASED_CODE IID_DEXTCTRLEvents =
{ 0xc324a902, 0x68a9, 0x42be, {0x94, 0x2c, 0x7e, 0x66, 0x8f, 0x1d, 0x32, 0x5e}};


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwEXTCTRLOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CEXTCTRLCtrl, IDS_EXTCTRL, _dwEXTCTRLOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CEXTCTRLCtrl::CEXTCTRLCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CEXTCTRLCtrl

BOOL CEXTCTRLCtrl::CEXTCTRLCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_EXTCTRL,
			IDB_EXTCTRL,
			afxRegApartmentThreading,
			_dwEXTCTRLOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CEXTCTRLCtrl::CEXTCTRLCtrl - Constructor

CEXTCTRLCtrl::CEXTCTRLCtrl()
{
	InitializeIIDs(&IID_DEXTCTRL, &IID_DEXTCTRLEvents);

	m_bParentDetach = FALSE;
	m_bOleLock      = TRUE;
	m_pParent       = NULL;

	// TODO: Initialize your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CEXTCTRLCtrl::~CEXTCTRLCtrl - Destructor

CEXTCTRLCtrl::~CEXTCTRLCtrl()
{
	if (!m_bOleLock)
		AfxOleLockApp();
	if (m_bParentDetach && m_pParent)
	{
		m_pParent->Detach();
		delete m_pParent;
	}

	// TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CEXTCTRLCtrl::OnDraw - Drawing function

void CEXTCTRLCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}


/////////////////////////////////////////////////////////////////////////////
// CEXTCTRLCtrl::DoPropExchange - Persistence support

void CEXTCTRLCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CEXTCTRLCtrl::OnResetState - Reset control to default state

void CEXTCTRLCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CEXTCTRLCtrl message handlers

int CEXTCTRLCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_pParent)		// design mode in the axisBuilder
	{
		m_pParent = new CWnd();
		m_pParent->FromHandle(lpCreateStruct->hwndParent);
		m_pParent->m_hWnd = lpCreateStruct->hwndParent;
		m_bParentDetach = TRUE;
	}

	PostMessage(WM_INITEXTCONTROL);
	
	return 0;
}

LRESULT CEXTCTRLCtrl::OnInitExtControl(WPARAM wParam, LPARAM lParam)
{
	// Add code initilaize internal controls
	return 0;
}

LRESULT CEXTCTRLCtrl::OnWizard(WPARAM wParam, LPARAM lParam)
{
	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:
		return (long)m_sRtn.operator LPCTSTR();
		break;
	case DLL_OUB:
		{
			
		}
		break;
	case DLL_ALERT:
		break;
	case DLL_TRIGGER:
		break;
	case DLL_DOMINO:
		break;
	case DLL_SETFONT:
		{
			m_Param.point = HIWORD(wParam);
			CString tmp = CString((char*)lParam);
			if (!tmp.IsEmpty())
				m_Param.fonts = tmp;
			Invalidate();
		}
		break;
	case DLL_SETPAL:
		Invalidate(FALSE);
		break;
	case DLL_NOTICE:	// this message no use in extControl
		break;
	}
	return 0;
}


BOOL CEXTCTRLCtrl::SendTR(CString sTR, BYTE type, CString data, int key /*= -1*/)
{
	if (key != -1)
	{
		type |= US_KEY;
		key = MAKELONG(key, m_Param.key);
	}
	else
	{
		type &= ~US_KEY;
		key = m_Param.key;
	}

	return SendTRA(m_pParent, sTR, type, data, key, m_Param.name);
}

CString CEXTCTRLCtrl::Variant(int comm, CString data)
{
	return VariantA(m_pParent, comm, data);
}

int CEXTCTRLCtrl::OpenView(int type, CString data)
{
	return openViewA(m_pParent, type, data);
}

COLORREF CEXTCTRLCtrl::GetIndexColor(int nIndex)
{
	if (m_pParent && IsWindow(m_pParent->m_hWnd))
		return GetColorA(m_pParent, nIndex);
	return RGB(0, 0, 0);
}

void CEXTCTRLCtrl::SetParam(_param *pParam)
{
	m_Param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	SetParams((long)pParam);
}


////////////////////////////////////////////////////////
//	Control options 

void CEXTCTRLCtrl::ParseOptions()
{
	CString tmp = m_Param.options;
	// TODO: Add code parsing your options here
}

CString CEXTCTRLCtrl::MakeOptions()
{
	// TODO: Add code making your options here
	return m_Param.options;
}

////////////////////////////////////////////////////////
//	Control Automation Method and Properties

BSTR CEXTCTRLCtrl::GetProperties() 
{
	MakeOptions();
	CString strResult = m_Param.options;

	return strResult.AllocSysString();
}

void CEXTCTRLCtrl::SetProperties(LPCTSTR sOptions) 
{
	m_Param.options = sOptions;
	ParseOptions();
}

void CEXTCTRLCtrl::SetParams(long pParams) 
{
	struct _param *pParam = (struct _param *)pParams;
	m_Param.key     = pParam->key;
	m_Param.name    = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect    = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts   = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point   = pParam->point;
	m_Param.style   = pParam->style;
	m_Param.tRGB    = pParam->tRGB;
	m_Param.pRGB    = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());
	ParseOptions();
}

BOOL CEXTCTRLCtrl::IsInvokeAllowed(long) 
{
	// You can check to see if COleControl::m_bInitialized is FALSE
	// in your automation functions to limit access.
	return TRUE;
}


void CEXTCTRLCtrl::SetCursorMap(LPCTSTR sKey, long left, long top, long right, long bottom) 
{
	struct _keyRect {
		RECT rect;
		char *pStr;
	}keyRect;

	keyRect.rect.left   = left;
	keyRect.rect.top    = top;
	keyRect.rect.right  = right;
	keyRect.rect.bottom = bottom;
	
	keyRect.pStr = (LPSTR)sKey;

	GetParent()->SendMessage(WM_CURSORMAP, SCM_SETCURSORMAP, (LPARAM)&keyRect);
}

void CEXTCTRLCtrl::DeleteAllCursorMap() 
{
	GetParent()->SendMessage(WM_CURSORMAP, SCM_DELETEALLCURSORMAP, 0);

}

void CEXTCTRLCtrl::DeleteCursorMap(LPCTSTR sKey) 
{
	GetParent()->SendMessage(WM_CURSORMAP, SCM_DELETECURSORMAP, (LPARAM)sKey);

}
