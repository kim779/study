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

#define FORM_WIDTH		800
#define FORM_HEIGHT		500

extern HINSTANCE g_hInstance;
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
	
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	// TODO: Add your specialized creation code here
	int nIndustry = -1;

	OnCreate_MainWnd(0);

	if(m_pwndMainFrm != NULL)
		m_pwndMainFrm->SetInit("0");

	CRect reClient;
	GetClientRect(reClient);
	m_pwndMainFrm->MoveWindow(reClient);

	AfxSetResourceHandle(hInstSave);

	m_FnSet.Variant(titleCC, PROGRAM_TITLE);

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
	if(nIndustry == 0)
	{
		nWidth	= FORM_WIDTH;
		nHeight = FORM_HEIGHT;
	}

	m_pwndMainFrm = new CAnalystMainFrmWnd;

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

void CScreenWnd::ReceiveTR(LPCSTR szTR, LPVOID aTRData, long dwTRDateLen , int nKey)
{
	m_pwndMainFrm->ReceiveData(szTR, aTRData, dwTRDateLen , nKey );
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
	int i = 0;
	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_SETPAL:
		m_FnSet.SetPalete();
		break;
		
	case DLL_ALERT:
		/*ReceiveReal(wParam, lParam);*/
		break;
		
	case DLL_OUBx:
		{
#if (0)//tour2k
			char* pBytes = (char *)lParam;
			struct _extTH* extTH = (struct _extTH *)pBytes;
			pBytes += L_extTH;

			int nKey = extTH->key;
			int nLen = extTH->size;

			m_FnSet.ReceiveTRx(nKey, pBytes, nLen);

//			wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, nKey), nLen);
//			lParam = (LPARAM)pBytes;
#else
		_extTHx* info = (_extTHx*)lParam;
		const int nLen = info->size;
		char* pData = info->data;
		int nKey = info->key;

		m_FnSet.ReceiveTRx(nKey, pData, nLen);

#endif
		}
		break;

	case DLL_OUB:
		m_FnSet.ReceiveTR(wParam, lParam);
		break;
		
	case DLL_TRIGGER:
		ParseTrigger((char*)lParam, FALSE);
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

void CScreenWnd::ParseTrigger(CString sTrigger, BOOL bTypeChk)
{
	int nFind = -1;
	nFind = sTrigger.Find("Null");
	
	while (!sTrigger.IsEmpty())
	{
		CString strData = CUtils::Parser(sTrigger, "\n");
		CString sSymbol = CUtils::Parser(strData, "\t");

		if(sSymbol == "대상변경")
		{
			BOOL bResult;
			CString strTitle;
			CStringArray arrName, arrCode;

			m_FnSet.GetUserMaster(usrMasterCondition, arrCode, arrName);
			bResult = (arrCode.GetSize() > 0) ? TRUE : FALSE;

			m_pwndMainFrm->m_ctrlTitle.GetWindowText(strTitle);

			if (bResult == TRUE && strTitle == "검색종목")
				m_pwndMainFrm->SendMessage(WM_NOTIFY_TREE, (WPARAM)NM_TREE_CLK, (LPARAM)&arrCode);
		}
		
	}
}