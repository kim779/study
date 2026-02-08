// ScreenWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ScreenWnd.h"

#include "../DR_Include/hana_typedef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FORM_WIDTH				855
#define FORM_HEIGHT				520
#define FORM_REAL_WIDTH			450
#define FORM_REAL_WIDTH_WATCH	685

#define	WM_BC_DATA_RECEIVED				WM_USER + 703

/////////////////////////////////////////////////////////////////////////////
// CScreenWnd

//IMPLEMENT_DYNCREATE(CScreenWnd, CWnd)

CScreenWnd::CScreenWnd(CWnd *pParent)
{
	m_pwndModuleMain = pParent;
	m_pwndMainFrm = NULL;
	
	m_FnSet.SetInit(m_pwndModuleMain, this);
}

CScreenWnd::~CScreenWnd()
{
}


BEGIN_MESSAGE_MAP(CScreenWnd, CWnd)
	//{{AFX_MSG_MAP(CScreenWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
 	ON_MESSAGE(WM_USER, OnUser)
	ON_MESSAGE(WM_MANAGE, OnManage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreenWnd message handlers


int CScreenWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	int nIndustry = 0;

	CString strFormCode = GetFormCode();
	if(strFormCode == "IB713000")
	{
		nIndustry = 0;
		m_FnSet.Variant(titleCC, PROGRAM_TITLE);
	}
	else if(strFormCode == "IB713100")
	{
		nIndustry = 1;
		m_FnSet.Variant(titleCC, PROGRAM_TITLE_INDUSTRY);
	}
	else if(strFormCode == "IB712800")
	{
		nIndustry = 2;
		m_FnSet.Variant(titleCC, PROGRAM_TITLE_REAL);
	}

	OnCreate_MainWnd(nIndustry);

	if(!m_pwndMainFrm)
		return 0;
	
	m_pwndMainFrm->SetInit(m_pwndModuleMain, nIndustry);
	m_pwndMainFrm->m_szScreenKey = strFormCode;
	
	CRect reClient;
	GetClientRect(reClient);
	m_pwndMainFrm->MoveWindow(reClient);

	return 0;
}

void CScreenWnd::OnDestroy() 
{
	if(m_pwndMainFrm)
	{
		m_pwndMainFrm->DestroyWindow();
	}

	CWnd::OnDestroy();	
}

void CScreenWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	m_pwndMainFrm->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
}


void CScreenWnd::OnCreate_MainWnd(int nIndustry)
{
	int nWidth	= -1;
	int nHeight	= -1;
	if(nIndustry == 0 || nIndustry == 1)
	{
		nWidth	= FORM_WIDTH;
	}
	else if(nIndustry == 2)
	{
		nWidth = FORM_REAL_WIDTH_WATCH;
	}
	nHeight = FORM_HEIGHT;

	m_pwndMainFrm = new CMainFrmWnd;
	m_pwndMainFrm->m_bIsFnUser = TRUE;

	BOOL bResult = m_pwndMainFrm->Create(
		NULL,
		_T("Main Frame Window"),
		WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		CRect(0, 0, 0, 0),
		this,
		NULL);
	m_pwndMainFrm->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	
	if(bResult)
	{
		m_pwndMainFrm->ShowWindow(SW_SHOW);
		SetWindowPos(NULL, 0, 0, nWidth, nHeight, SWP_NOZORDER);
		m_pwndMainFrm->SetWindowPos(NULL, 0, 0, nWidth, nHeight, SWP_NOZORDER);
		m_FnSet.SetPalete();
	}
}

void CScreenWnd::ReceiveTR(LPCSTR szTR, LPVOID aTRData, long dwTRDateLen)
{
	m_pwndMainFrm->ReceiveData(szTR, aTRData, dwTRDateLen);
}

void CScreenWnd::SetPalete(COLORREF clrPalete, ST_TABCOLOR stTabColor)
{
	m_pwndMainFrm->ChangeSkinColor(clrPalete, stTabColor);
}

CWnd* CScreenWnd::GetModuleMainWnd()
{
	return m_pwndModuleMain;
}

LONG CScreenWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;
	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_SETPAL:
		m_FnSet.SetPalete();
		/*SetPallette();*/
		break;
		
	case DLL_ALERTx:
		m_FnSet.ReceiveReal(wParam, lParam);
		break;
		
	case DLL_OUBx:
		{
#if(0)
			char* pBytes = (char *)lParam;
			struct _extTH* extTH = (struct _extTH *)pBytes;
			pBytes += L_extTH;

			int nKey = extTH->key;
			int nLen = extTH->size;

			m_FnSet.ReceiveTRx(nKey, pBytes, nLen);
#else

		_extTHx* info = (_extTHx*)lParam;
		int nLen = info->size;
		char* pData = info->data;
		int nKey = info->key;

		m_FnSet.ReceiveTRx(nKey, pData, nLen);
#endif

//			wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, nKey), nLen);
//			lParam = (LPARAM)pBytes;
		}
		break;

	case DLL_OUB:
		m_FnSet.ReceiveTR(wParam, lParam);
		break;

	case DLL_TRIGGER:
		// 		if (m_bDestroy)
		// 			break;
		// 		ParseTrigger((char*)lParam, FALSE);
		break;
		
	case DLL_DOMINO:		//# Open Shard!
		/*ParseTrigger((char*)lParam, FALSE);*/
		break;
		
	default:
		;
	}
	
	return ret;
}

LONG CScreenWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;
	switch (LOWORD(wParam))
	{
	case MK_SETPAL:
		m_FnSet.m_clrPalete = (COLORREF)lParam;
		/*OperPallette();*/
		Invalidate(FALSE);
		break;
	case MK_GETROOT:
		/*ret = (LONG)(char*)m_home.operator LPCTSTR();*/
		break;
		
	default:
		;
	}
	
	return ret;
}

void CScreenWnd::ReceiveReal(WPARAM wParam, LPARAM lParam)
{
	m_pwndMainFrm->m_pWndDummy->SendMessage(WM_BC_DATA_RECEIVED, wParam, lParam);
}