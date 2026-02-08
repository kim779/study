// Mainwnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_OPTIONRTS.h"
#include "Mainwnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainwnd

CMainwnd::CMainwnd()
{
	EnableAutomation();

	m_pWizard = nullptr;
	m_pParentWnd = nullptr;
}

CMainwnd::~CMainwnd()
{
}

void CMainwnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CMainwnd, CWnd)
	//{{AFX_MSG_MAP(CMainwnd)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainwnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainwnd)
	DISP_FUNCTION(CMainwnd, "send4013", send4013, VT_EMPTY, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMainwnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {F22C8A42-D70A-4E59-BD1F-E6D9C06A9DED}
static const IID IID_IMainwnd =
{ 0xf22c8a42, 0xd70a, 0x4e59, { 0xbd, 0x1f, 0xe6, 0xd9, 0xc0, 0x6a, 0x9d, 0xed } };

BEGIN_INTERFACE_MAP(CMainwnd, CWnd)
	INTERFACE_PART(CMainwnd, IID_IMainwnd, Dispatch)
END_INTERFACE_MAP()
#define TK_PIBO401C		(9)
/////////////////////////////////////////////////////////////////////////////
// CMainwnd message handlers

//pibo401c
struct  pibo401c {
	char    gubn[1];    /* '0' : 일반 '1' : 미니 '2' : 위클리 */
	char    mymd[6];    /* 만기년월 202203                    */
	char    cpgb[1];    /* Call Put 구분 1:콜, 2:풋           */
} *st_pibo401c;

void CMainwnd::send4013(LPCTSTR sdata) 
{
	if (strlen(sdata) != 6) 
		return;

	struct pibo401c mid {};

	mid.gubn[0] = '0';
	if (m_param.options.Find("k1") >= 0)
		mid.cpgb[0] = '1';
	else
		mid.cpgb[0] = '2';
	memcpy(mid.mymd, (LPCSTR)sdata, sizeof(mid.mymd));

	sendTR("pibo401c", (char*)&mid, sizeof(mid), US_KEY, TK_PIBO401C); 	
}

bool CMainwnd::sendTR(CString trN, char* datB, int datL, BYTE stat, int key)
{
	if (datL <= 0)
		return false;

	char	wb[256]{};
	struct	_userTH	userth {};
	int	idx = 0;

	// key & control name
	wb[idx++] = 1;
	CopyMemory(&wb[idx], m_param.name, m_param.name.GetLength());
	idx += m_param.name.GetLength();
	wb[idx++] = '\t';

	// userTH
	CopyMemory(userth.trc, trN, trN.GetLength());
	userth.stat = stat;
	userth.key = m_param.key;
	CopyMemory(&wb[idx], &userth, L_userTH);
	idx += L_userTH;

	// data
	CopyMemory(&wb[idx], datB, datL);
	
	return (m_pWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datL), (LPARAM)wb)) ? true : false;
}

int CMainwnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	/*
	CString strpop;
	strpop = Variant(popCC, "4013hwnd");
	m_pParentWnd = (CWnd*)atoi(strpop);
	
	int ddata = atoi(strpop);
	HWND hWnd =  (HWND)ddata;
	m_pParentWnd = CWnd::FromHandle(hWnd);
	
	m_pParentWnd->GetWindowText(strpop);
	*/
	return 0;
}

LPCTSTR CMainwnd::Variant( int cmd, LPCTSTR data )
{
	return (LPCTSTR)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, cmd), (LPARAM)data);
}

void CMainwnd::setParam(const struct _param* pParam)
{
	m_param.name = CString(pParam->name, pParam->name.GetLength());
	m_param.key = pParam->key;
	m_param.options = CString(pParam->options, pParam->options.GetLength());
}

struct  gmod {
	char	rtcd[9];	// RTS CODE
};

struct  mod {
	char	grec[4];
	struct	gmod	gmod[200];
} *mod;


long CMainwnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	CString slog;
	CString sdata;

	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUBx:
		{
			struct	_extTHx* exth = (struct _extTHx*)lParam;
			struct  mod* pmod = (struct mod*)exth->data;
		}
		break;

	case DLL_OUB:	// NOT USED. Use DLL_OUBx Only !!
		{
			struct  mod* pmod = (struct mod*)lParam;

			CString stmp;
			stmp.Format("%s", pmod->grec);
		}	
		break;

	case DLL_ALERTx:
		{
			/*
			DWORD* data;
			CString strCode;
			struct _alertR* alertR;
			alertR = (struct _alertR*)lParam;
			
			strCode = alertR->code;
			OutputDebugString(strCode + "\n");
			if(strCode.Find("000060") < 0)
				break;
			
			m_pParentWnd->SendMessage(WM_USER, MAKEWPARAM(DLL_ALERTx + 2, 0), lParam);
			
			for(int i=0;i<alertR->size;i++)
			{
				data = (DWORD*)alertR->ptr[i];
				
				bool bCurrChg = false;
				
				CString strVal;
				if (data[23])			// 현재가
				{
					strVal = (char*)data[23];
				}
			}
			*/			
		}
		break;
	}
	return 0;
}

CString CMainwnd::Variant(int nComm, CString strData /* = _T("") */)
{
	CString strRet;
	const	char*	pRet = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nComm), (LPARAM)strData.GetString());
	
	if ((long)pRet > 1)
		strRet = pRet;
	
	return strRet;
}