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

#define FORM_WIDTH		840
#define FORM_HEIGHT		520

/////////////////////////////////////////////////////////////////////////////
// CScreenWnd

//IMPLEMENT_DYNCREATE(CScreenWnd, CWnd)

CScreenWnd::CScreenWnd(CWnd *pParent)
{
	m_pwndModuleMain	= pParent;
	m_pwndMainFrm		= NULL;
	
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

	OnCreate_MainWnd();

	if(m_pwndMainFrm != NULL)
		m_pwndMainFrm->SetInit("");

	CRect reClient;
	GetClientRect(reClient);
	m_pwndMainFrm->MoveWindow(reClient);

	return 0;
}

void CScreenWnd::OnDestroy() 
{
	if(m_pwndMainFrm && ::IsWindow(m_pwndMainFrm->GetSafeHwnd()))	
		m_pwndMainFrm->DestroyWindow();	

	CWnd::OnDestroy();	
}

void CScreenWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	if(m_pwndMainFrm && ::IsWindow(m_pwndMainFrm->GetSafeHwnd()))	
		m_pwndMainFrm->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
}


void CScreenWnd::OnCreate_MainWnd()
{
	int nWidth	= FORM_WIDTH;
	int nHeight = FORM_HEIGHT;
		
	CString strFormCode = GetFormCode();

	if(strFormCode == "IB712700")
		m_FnSet.Variant(titleCC, PROGRAM_TITLE);
	else
		m_FnSet.Variant(titleCC, PROGRAM_TITLE_BRIGHT);
	
	m_pwndMainFrm = new CMainFrmWnd(strFormCode, this);
	m_pwndMainFrm->m_bIsFnUser = TRUE;

	BOOL bResult = m_pwndMainFrm->Create(
		NULL,
		_T("Main Frame Window"),
		WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		CRect(0, 0, nWidth, nHeight),
		this,
		NULL);
	m_pwndMainFrm->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	
	if(bResult)
	{
		m_pwndMainFrm->ShowWindow(SW_SHOW);
		SetWindowPos(NULL, 0, 0, nWidth, nHeight, SWP_NOZORDER);
		m_FnSet.SetPalete();
	}
}

void CScreenWnd::ReceiveTR(LPCSTR szTR, LPVOID aTRData, long dwTRDateLen , int nKey )
{
	if(m_pwndMainFrm)
		m_pwndMainFrm->ReceiveData(szTR, aTRData, dwTRDateLen , nKey );
}

void CScreenWnd::SetPalete(COLORREF clrPalete, ST_TABCOLOR stTabColor)
{
	if(m_pwndMainFrm)
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
		
	case DLL_ALERT:
		/*ReceiveReal(wParam, lParam);*/
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
		}
		break;
/*
			wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, nKey), nLen);
			int lLen = (int)HIWORD(wParam);
			long lLen1 = (long)LOWORD(wParam);
			lParam = (LPARAM)pBytes;
		}
*/
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
