// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_K1Driver.h"
#include "MainWnd.h"
#include "trsmain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainWnd

CMainWnd::CMainWnd(CWnd *parent)
{
	EnableAutomation();
	m_pWizard = parent;
}

CMainWnd::~CMainWnd()
{
}

void CMainWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
	DISP_FUNCTION(CMainWnd, "Start", Start, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "Run", Run, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "GetPasswd", GetPasswd, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "Close", Close, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {E9F51590-7F2D-4101-B816-CE3E4EE75EAB}
static const IID IID_IMainWnd =
{ 0xe9f51590, 0x7f2d, 0x4101, { 0xb8, 0x16, 0xce, 0x3e, 0x4e, 0xe7, 0x5e, 0xab } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers

void CMainWnd::Start() 
{
	// TODO: Add your dispatch handler code here
	//==============포트 열기============
	if(PHONEPAD_PortOpen(0))
	{
		PHONEPAD_InitDevice();
		PHONEPAD_SetConfig(0,10);	
		PHONEPAD_GetPWSignalWindow(m_hWnd);	
		
		MessageBox("폰패드 연결 성공(Port Open Success)!!","IBK투자증권");
	}
	else
	{
		//char * tmp = "0423";
		//PostMessage(WM_PHONEPAD,0,(long)tmp);
		MessageBox("폰패드 연결 실패(Port Open Failure)!!","IBK투자증권");
		return;
	}

}

BSTR CMainWnd::GetProperties() 
{
	CString strResult;
	// TODO: Add your dispatch handler code here

	return strResult.AllocSysString();
}

void CMainWnd::SetProperties(LPCTSTR str) 
{
	// TODO: Add your dispatch handler code here

}

void CMainWnd::Run(LPCTSTR str) 
{
	// TODO: Add your dispatch handler code here
	OutputDebugString("RUN K1\n");
}

void CMainWnd::GetPasswd() 
{
	// TODO: Add your dispatch handler code here
	//==============비밀번호 읽어오기 시작============
	// TODO: Add your control notification handler code here
	char * temp = NULL;
	int  loop=1;
	int  limit = 0;
	CString string;
	
	while(loop)
	{			
		//temp = PHONEPAD_GetPassWDSTR();
		//if(temp != NULL) break;		
		Sleep(300);
		
		switch(PHONEPAD_Status())
		{
		case 0: 
			break;     // normal
			
		case 1: 
			loop = 0; 
			temp = PHONEPAD_GetPassWDSTR();
			break;     //  비밀번호 입력완료
			
		case 2: 
			MessageBox("시간을 초과하였습니다.","IBK투자증권");
			//m_pWizard->CloseWindow();
			//m_phone_dlgs = NULL;
			return; 
			break;       //  비밀번호 입력취소
		}

		if(loop == 0)
			break;

		Sleep(300);
		
		limit++;
		if (limit>=20) break;
	}
	if (limit == 20)
	{
		//m_phone_dlgs = NULL;
		MessageBox("입력 시간이 초과되었습니다.","IBK투자증권");	
	}	

//	AfxMessageBox((LPCTSTR)temp);
	string.Format("Set_pwd\t%s", temp);

	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCSTR)string);

// 	string.Format("1301\t%s", "000660");
// 	
// 	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, triggerCC), (LPARAM)(LPCSTR)string);

	Variant(triggerCC, string);	
	
}


void CMainWnd::Close() 
{
	// TODO: Add your dispatch handler code here
	PHONEPAD_PortClose();
}

CString CMainWnd::Variant(int comm, CString data)
{
	CString retvalue = _T("");
	
	char* dta = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);
	
	if ((long)dta > 1)
		retvalue.Format("%s", dta);
	
	return retvalue;
}