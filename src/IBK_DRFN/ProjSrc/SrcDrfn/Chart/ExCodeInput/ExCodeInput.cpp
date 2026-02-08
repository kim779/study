// ExCodeInput.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ExCodeInput.h"
#include "Sfncodeinput.h"

#include "../../inc/BaseDesk.h"
#include "../../inc/drstring.h"
#include "../../inc/ExOCXDLL.h"
// #include "../../../../../Interface/ICtlBase.h"
// #include "../../../../CtrlWrap/WrpItemCode.h"		// 종목컨트롤

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT RMSG_GETDISPATCHER_ITEMCODE = ::RegisterWindowMessage(_T("RMSG_GETDISPATCHER_ITEMCODE"));
const UINT RMSG_GETINTERFACE_ITEMCODE = ::RegisterWindowMessage(_T("RMSG_GETINTERFACE_ITEMCODE"));

#define		CODEINPUT_PROPERTY_FO	_T("CC=00011000000000000010000000000:CD=0:CH=1:CT=1:MHC=20:MXL=8:MIL=8:ECP=1;1;0;1;0;0;:UEP=1:CN1=코드:CN2=종목명:GS=코드-선물옵션7;:LS=코드2;")

#define		CODEINPUT_PROPERTY_FO_0	_T("CC=00011000000000000010000000000:CD=0:CH=1:CT=1:MHC=20:MXL=8:MIL=8:ECP=0;1;0;1;0;0;:UEP=0:CN1=코드:CN2=종목명:GS=코드-선물옵션7;:LS=코드2;:USM=1")

#define		CODEINPUT_PROPERTY_F	_T("CC=00010000000000000010000000000:CD=1:CH=0:CT=1:MHC=20:MXL=8:MIL=8:ECP=1;1;0;1;0;0;:UEP=1::CN1=코드:CN2=종목명:GS=코드-선물옵션7;:LS=코드2;")

#define		CODEINPUT_PROPERTY_F_0	_T("CC=00010000000000000010000000000:CD=1:CH=0:CT=1:MHC=20:MXL=8:MIL=8:ECP=1;1;0;1;0;0;:UEP=0::CN1=코드:CN2=종목명:GS=코드-선물옵션7;:LS=코드2;")		// 선물속성에 콜풋전환 옵션 없음

#define		CODEINPUT_PROPERTY_O	_T("CC=00001000000000000000000000000:CD=1:CH=0:CT=1:MHC=20:MXL=8:MIL=8:ECP=1;1;0;1;0;0;:UEP=1::CN1=코드:CN2=종목명:GS=코드-선물옵션7;:LS=코드2;")

#define		CODEINPUT_PROPERTY_ITEM	_T("CC=00000110000010000000000000000:CD=1:CH=0:CT=1:MHC=20:MXL=4:MIL=4:ECP=1;1;0;1;0;0;:CN1=코드:CN2=종목명:GS=:LS=")

LPCSTR g_strList [] = {
	_T("CM=Name,NtsCodeInput0"),
	_T("AN=NtsCodeInput0"),
	_T("ID=27,28,1"),
	_T("RC=207,2,252,22"),
	_T("WS=WS_CHILD|WS_VISIBLE|WS_TABSTOP"),
	_T("WSE="),
	_T("BC=255,255,255"),
	_T("CC=11000000000000000000000000000"),
	_T("CCN="),
	_T("CD=0"),
	_T("CH=1"),
	_T("CT=1"),
	_T("CFT=굴림체;12;0;0;0;0;"),
	_T("FCP=-1"),
	_T("FC=0,0,0"),
	_T("IDD="),
	_T("IDE="),
	_T("IHD="),
	_T("IHE="),
	_T("IHD="),
	_T("IHE="),
	_T("MHC=20"),
	_T("MXL=6"),
	_T("MIL=6"),
	_T("MT=0"),
	_T("OCN="),
	_T("TR="),
	_T("UC=0"),
	_T("WE=1"),
	_T("UBC=0"),
	_T("UFC=0"),
	_T("UCL=0"),
	_T("IR=0"),
	_T("HI=0"),
	_T("XID="),
	_T("TKS=1"),
	_T("UDF=1"),
	_T("UEC=0"),
	_T("ECP=1;1;0;1;0;0;"),
	_T("CN1=코드"),
	_T("CN2=종목명"),
	_T("GS=코드-종목7;"),
	_T("LS=코드2;")
};
//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CExCodeInputApp

BEGIN_MESSAGE_MAP(CExCodeInputApp, CWinApp)
	//{{AFX_MSG_MAP(CExCodeInputApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExCodeInputApp construction

CExCodeInputApp::CExCodeInputApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CExCodeInputApp object
LPVOID CExCodeInputApp::CreateCodeInput(LPVOID lpPointer, CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, CStringList* pListProperty)
{
// 	CWnd* pParentWnd = pWnd->GetParent()->GetParent();
// 	CCodeEdit* pCodeInput = new CCodeEdit;
// 	//if(pCodeInput->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_TABSTOP, *pRect, pWnd, pWnd->GetDlgCtrlID()))
// 
// 	if(pCodeInput->Create(WS_CHILD | WS_VISIBLE | WS_BORDER|WS_TABSTOP,
// 		*pRect, pWnd, pWnd->GetDlgCtrlID()))
// 
// 	{
// 		CBaseDesk2* pBaseDesk = (CBaseDesk2*)lBaseDesk;
// 
// 		pCodeInput->SetBaseDesk(lBaseDesk);
// 		pCodeInput->SetRscMng(pBaseDesk->GetFrameInfo(_T("RSCMNG")));
// 		pCodeInput->SetAllProperties2((long)pListProperty);
// 	}
// 	else
// 	{
// 		delete pCodeInput;
// 		pCodeInput = NULL;
// 	}
// 
// 	return pCodeInput;
	return NULL;
}

LPVOID CExCodeInputApp::CreateCodeInput(LPVOID lpPointer, CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, LPCTSTR* lpszProperties, int nSize)
{
	CWnd* pScreenView = pWnd->GetParent()->GetParent();
//	CWrpItemCode* pItemCode = (CWrpItemCode*)pScreenView->SendMessage(RMSG_GETDISPATCHER_ITEMCODE);
//	ICtlBase* pCtlItemCode = (ICtlBase*)pScreenView->SendMessage(RMSG_GETINTERFACE_ITEMCODE);
//	if(pCtlItemCode)
//	{
//		HWND hItemCode = pCtlItemCode->GetHwnd();
// 		::SetParent(hItemCode, pWnd->GetSafeHwnd());
// 
// 		CCodeEdit* pCodeInput = new CCodeEdit;
// 		pCodeInput->SetMember(pCtlItemCode, pItemCode);
// 		return pCodeInput;
// 	}
// 	else
//	{
// 		pScreenView = pWnd->GetParent()->GetParent()->GetParent()->GetParent();
// 		pItemCode = (CWrpItemCode*)pScreenView->SendMessage(RMSG_GETDISPATCHER_ITEMCODE);
// 		pCtlItemCode = (ICtlBase*)pScreenView->SendMessage(RMSG_GETINTERFACE_ITEMCODE);
// 
// 		if(pCtlItemCode)
// 		{
// 			HWND hItemCode = pCtlItemCode->GetHwnd();
// 			::SetParent(hItemCode, pWnd->GetSafeHwnd());

			CCodeEdit* pCodeInput = new CCodeEdit;
			//pCodeInput->SetMember(NULL, NULL);
			if(pCodeInput->Create(WS_CHILD | WS_VISIBLE | WS_BORDER|WS_TABSTOP,
		 		*pRect, pWnd, pWnd->GetDlgCtrlID()))
			return pCodeInput;
// 		}
// 	}
	
	return NULL;
}

LPVOID CExCodeInputApp::CreateCodeInput(LPVOID lpPointer, CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, LPCTSTR lpszProperties)
{
	return NULL;
}

LPVOID CExCodeInputApp::CreateCodeInput(LPVOID lpPointer, CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, UINT nProperties)
{
	CWnd* pParentWnd = pWnd->GetParent()->GetParent();

	if(nProperties == CODEINPUT_PROP_STOCK)
		return CreateCodeInput(lpPointer, pWnd, pRect, nID, lBaseDesk, g_strList, _countof(g_strList));

	CMapStringToString map;

	int nIndex = 0;
	LPSTR lpszProperties = NULL;
	switch(nProperties)
	{
	case CODEINPUT_PROP_FO:
		lpszProperties = CODEINPUT_PROPERTY_FO;
		break;
	case CODEINPUT_PROP_FO_02:
		lpszProperties = CODEINPUT_PROPERTY_FO_0;
		break;
	case CODEINPUT_PROP_FUT:
		lpszProperties = CODEINPUT_PROPERTY_F;
		break;
	case CODEINPUT_PROP_FUT_02:
		lpszProperties = CODEINPUT_PROPERTY_F_0;
		break;
	case CODEINPUT_PROP_OPT:
		lpszProperties = CODEINPUT_PROPERTY_O;
		break;
	case CODEINPUT_PROP_ITEM:
		lpszProperties = CODEINPUT_PROPERTY_ITEM;
		break;
	}

	if(lpszProperties)
		return CreateCodeInput(lpPointer, pWnd, pRect, nID, lBaseDesk, g_strList, _countof(g_strList));
	return NULL;
}

void CExCodeInputApp::ExitCodeInput(LPVOID lpPointer)
{
	//m_pCurMember = (CSfnCodeInput*)lpPointer;
	m_pCurMember = (CCodeEdit*)lpPointer;

	delete m_pCurMember;
	m_pCurMember = NULL;
}

short CExCodeInputApp::GetGubunFromCode(LPCTSTR szCode)
{
	return m_pCurMember->GetGubunFromCode(szCode);
}

void CExCodeInputApp::SetDataText(LPCTSTR szText)
{
	m_pCurMember->SetDataText(szText);
}

void CExCodeInputApp::SetDataText2(LPCTSTR szCode)
{
	m_pCurMember->SetDataText2(szCode);
}

CString CExCodeInputApp::GetDataText()
{
	return m_pCurMember->GetDataText();
}

void CExCodeInputApp::SetEnabled(BOOL propVal)
{
	m_pCurMember->SetEnabled(propVal);
}

BOOL CExCodeInputApp::GetEnabled()
{
	return m_pCurMember->GetEnabled();
}

long CExCodeInputApp::GetBase(LPCTSTR lpszKey)
{
	return m_pCurMember->GetBase(lpszKey);
}

void CExCodeInputApp::SetFocus()
{
	m_pCurMember->SetFocus();
}

void CExCodeInputApp::SetCurSel(short nIndex)
{
	m_pCurMember->SetCurSel(nIndex);
}

OLE_HANDLE CExCodeInputApp::GetHWnd()
{
	return m_pCurMember->GetHWnd();
}

HWND CExCodeInputApp::GetSafeHwnd()
{
	return m_pCurMember->GetSafeHwnd();
}

void CExCodeInputApp::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint)
{
	m_pCurMember->MoveWindow(x, y, nWidth, nHeight, bRepaint);
}

void CExCodeInputApp::MoveWindow(LPCRECT lpRect, BOOL bRepaint)
{
	m_pCurMember->MoveWindow(lpRect, bRepaint);
}

BOOL CExCodeInputApp::SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags)
{
	return m_pCurMember->SetWindowPos(pWndInsertAfter, x, y, cx, cy, nFlags);
}

BOOL CExCodeInputApp::ShowWindow(int nCmdShow)
{
	return m_pCurMember->ShowWindow(nCmdShow);
}

void CExCodeInputApp::GetWindowRect(LPRECT lpRect)
{
	m_pCurMember->GetWindowRect(lpRect);
}

void CExCodeInputApp::GetClientRect(LPRECT lpRect)
{
	m_pCurMember->GetClientRect(lpRect);
}

CExCodeInputApp theApp;

extern "C" __declspec(dllexport) LPVOID Ex_CreateCodeInputList(LPVOID lpPointer, CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, CStringList* pListProperty)
{
	return theApp.CreateCodeInput(lpPointer, pWnd, pRect, nID, lBaseDesk, pListProperty);
}

extern "C" __declspec(dllexport) LPVOID Ex_CreateCodeInputArray(LPVOID lpPointer, CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, LPCTSTR* lpszProperties, int nSize)
{
	return theApp.CreateCodeInput(lpPointer, pWnd, pRect, nID, lBaseDesk, lpszProperties, nSize);
}

extern "C" __declspec(dllexport) LPVOID Ex_CreateCodeInputString(LPVOID lpPointer, CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, LPCTSTR lpszProperties)
{
	return theApp.CreateCodeInput(lpPointer, pWnd, pRect, nID, lBaseDesk, lpszProperties);
}

extern "C" __declspec(dllexport) LPVOID Ex_CreateCodeInputInt(LPVOID lpPointer, CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, UINT nProperties)
{
	return theApp.CreateCodeInput(lpPointer, pWnd, pRect, nID, lBaseDesk, nProperties);
}

extern "C" __declspec(dllexport) void Ex_ExitCodeInput(LPVOID lpPointer)
{
	theApp.ExitCodeInput(lpPointer);
}

extern "C" __declspec(dllexport) short Ex_GetGubunFromCode(LPVOID lpPointer, LPCTSTR szCode)
{
	//theApp.m_pCurMember = (CSfnCodeInput*)lpPointer;
	theApp.m_pCurMember = (CCodeEdit*)lpPointer;
	return theApp.GetGubunFromCode(szCode);
}

extern "C" __declspec(dllexport) void Ex_SetDataText(LPVOID lpPointer, LPCTSTR szText)
{
	//theApp.m_pCurMember = (CSfnCodeInput*)lpPointer;
	theApp.m_pCurMember = (CCodeEdit*)lpPointer;
	theApp.SetDataText(szText);
}

extern "C" __declspec(dllexport) void Ex_SetDataText2(LPVOID lpPointer, LPCTSTR szCode)
{
	//theApp.m_pCurMember = (CSfnCodeInput*)lpPointer;
	theApp.m_pCurMember = (CCodeEdit*)lpPointer;
	theApp.SetDataText2(szCode);
}

CString WINAPI Ex_GetDataText(LPVOID lpPointer)
{
	//theApp.m_pCurMember = (CSfnCodeInput*)lpPointer;
	theApp.m_pCurMember = (CCodeEdit*)lpPointer;
	return theApp.GetDataText();
}

extern "C" __declspec(dllexport) void Ex_SetEnabled(LPVOID lpPointer, BOOL propVal)
{
	//theApp.m_pCurMember = (CSfnCodeInput*)lpPointer;
	theApp.m_pCurMember = (CCodeEdit*)lpPointer;
	theApp.SetEnabled(propVal);
}

extern "C" __declspec(dllexport) BOOL Ex_GetEnabled(LPVOID lpPointer)
{
	//theApp.m_pCurMember = (CSfnCodeInput*)lpPointer;
	theApp.m_pCurMember = (CCodeEdit*)lpPointer;
	return theApp.GetEnabled();
}

extern "C" __declspec(dllexport) long Ex_GetBase(LPVOID lpPointer, LPCTSTR lpszKey)
{
	//theApp.m_pCurMember = (CSfnCodeInput*)lpPointer;
	theApp.m_pCurMember = (CCodeEdit*)lpPointer;
	return theApp.GetBase(lpszKey);
}

extern "C" __declspec(dllexport) void Ex_SetFocus(LPVOID lpPointer)
{
	//theApp.m_pCurMember = (CSfnCodeInput*)lpPointer;
	theApp.m_pCurMember = (CCodeEdit*)lpPointer;
	theApp.SetFocus();
}

extern "C" __declspec(dllexport) void Ex_SetCurSel(LPVOID lpPointer, short nIndex)
{
	//theApp.m_pCurMember = (CSfnCodeInput*)lpPointer;
	theApp.m_pCurMember = (CCodeEdit*)lpPointer;
	theApp.SetCurSel(nIndex);
}

extern "C" __declspec(dllexport) OLE_HANDLE Ex_GetHWnd(LPVOID lpPointer)
{
	//theApp.m_pCurMember = (CSfnCodeInput*)lpPointer;
	theApp.m_pCurMember = (CCodeEdit*)lpPointer;
	return theApp.GetHWnd();
}

extern "C" __declspec(dllexport) HWND Ex_GetSafeHwnd(LPVOID lpPointer)
{
	//theApp.m_pCurMember = (CSfnCodeInput*)lpPointer;
	theApp.m_pCurMember = (CCodeEdit*)lpPointer;
	return theApp.GetSafeHwnd();
}

extern "C" __declspec(dllexport) void Ex_MoveWindow(LPVOID lpPointer, int x, int y, int nWidth, int nHeight, BOOL bRepaint)
{
	//theApp.m_pCurMember = (CSfnCodeInput*)lpPointer;
	theApp.m_pCurMember = (CCodeEdit*)lpPointer;
	theApp.MoveWindow(x, y, nWidth, nHeight, bRepaint);
}

extern "C" __declspec(dllexport) void Ex_MoveWindow2(LPVOID lpPointer, LPCRECT lpRect, BOOL bRepaint)
{
	//theApp.m_pCurMember = (CSfnCodeInput*)lpPointer;
	theApp.m_pCurMember = (CCodeEdit*)lpPointer;
	theApp.MoveWindow(lpRect, bRepaint);
}

extern "C" __declspec(dllexport) BOOL Ex_SetWindowPos(LPVOID lpPointer, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags)
{
	//theApp.m_pCurMember = (CSfnCodeInput*)lpPointer;
	theApp.m_pCurMember = (CCodeEdit*)lpPointer;
	return theApp.SetWindowPos(pWndInsertAfter, x, y, cx, cy, nFlags);
}

extern "C" __declspec(dllexport) BOOL Ex_ShowWindow(LPVOID lpPointer, int nCmdShow)
{
	//theApp.m_pCurMember = (CSfnCodeInput*)lpPointer;
	theApp.m_pCurMember = (CCodeEdit*)lpPointer;
	return theApp.ShowWindow(nCmdShow);
}

extern "C" __declspec(dllexport) void Ex_GetWindowRect(LPVOID lpPointer, LPRECT lpRect)
{
	//theApp.m_pCurMember = (CSfnCodeInput*)lpPointer;
	theApp.m_pCurMember = (CCodeEdit*)lpPointer;
	theApp.GetWindowRect(lpRect);
}

extern "C" __declspec(dllexport) void Ex_GetClientRect(LPVOID lpPointer, LPRECT lpRect)
{
	//theApp.m_pCurMember = (CSfnCodeInput*)lpPointer;
	theApp.m_pCurMember = (CCodeEdit*)lpPointer;
	theApp.GetClientRect(lpRect);
}
