// DllDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MultiChart.h"
#include "DllDlg.h"
#include ".\dlldlg.h"

const UINT RMSG_GETDLLSIZE = ::RegisterWindowMessage("RMSG_GETDLLSIZE");
const UINT RMSG_SETDLLSIZE = ::RegisterWindowMessage("RMSG_SETDLLSIZE");

// CDllDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDllDlg, CDialog)
CDllDlg::CDllDlg(LPCSTR szDllName, LPCSTR szTitle, void* pSTInterface, HINSTANCE hOldRsc, CWnd* pParent /*=NULL*/)
	: CDialog(CDllDlg::IDD, pParent)
{
	//m_szDllName = "HWStrategyAnalysis";
	m_szDllName = szDllName;
	m_szTitle = szTitle;
	m_pSTInterface = pSTInterface;
	m_hOldRsc = hOldRsc;
	m_hTradeInst = NULL;
	m_nOptimizerDiv = 0;

	m_pWndTrade = NULL;

}

CDllDlg::~CDllDlg()
{
}

void CDllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDllDlg, CDialog)
	//{{AFX_MSG_MAP(CDlgAnalysis)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(RMSG_SETDLLSIZE,   OnSetDllSize)
END_MESSAGE_MAP()


// CDllDlg 메시지 처리기입니다.

void CDllDlg::OnOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnOK();
}

BOOL CDllDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	AfxSetResourceHandle(m_hOldRsc);

	if(m_szTitle.GetLength()) SetWindowText(m_szTitle);

	BOOL bRet = OpenAUP();


	if(bRet==FALSE) OnCancel();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


// usr/src/AupMain/AupFrm.cpp에서 루틴가져옴
//#include "../../../src/Shared/inc/AupAgentEventDef.h"
//#define WMU_GET_AUP_SIZE	WM_USER + 2243			// Aup의 기본 Size를 View에 요청한다.
#define WMU_SET_TRADING_INSTANCE WM_USER + 2248		// Trading화면에 Dll을 연결한다.

//#include "../../../src/Shared/inc/ISTitleBar.h"
#define		SIZE_TITLE_BAR			23			    ///> 타이틀바 높이.

BOOL CDllDlg::OpenAUP()
{
	if(!m_pWndTrade)
		m_pWndTrade = LoadTradeAup(m_szDllName, this);

	//CWnd *pTrade = LoadTradeAup(m_szDllName, GetParentFrame());
	if(m_pWndTrade == NULL)
	{
		// Error 처리
		return FALSE;
	}
	else
	{   
		//((CAUPExMainView *)m_pInfoAgent->m_pActiveView)->SetTradingWnd(pTrade);
		CPoint size;
		size.x = 800;
		size.y = 600;
		//pTrade->SendMessage(WMU_GET_AUP_SIZE,(WPARAM)&size,0);
		m_pWndTrade->SendMessage(RMSG_GETDLLSIZE,(WPARAM)&size,0);

		CRect frameRect;
		CRect viewRect;
		GetWindowRect(&frameRect);
		GetClientRect(&viewRect);

		int nTheme = nTheme = GetSystemMetrics(SM_CYCAPTION);

		frameRect.right = frameRect.right - 
			GetSystemMetrics(SM_CXEDGE) - 
			GetSystemMetrics(SM_CXDLGFRAME) -
			GetSystemMetrics(SM_CYBORDER);

		frameRect.bottom = frameRect.bottom - GetSystemMetrics(SM_CYEDGE) -
			GetSystemMetrics(SM_CYDLGFRAME) - nTheme -
			GetSystemMetrics(SM_CYBORDER) + SIZE_TITLE_BAR;

		int iNewWidth = frameRect.Width() - viewRect.Width() + size.x;
		int iNewHeight = frameRect.Height() - viewRect.Height() + size.y;

		SetWindowPos(NULL,0,0,iNewWidth,iNewHeight,SWP_NOMOVE);
	}
	return TRUE;
}


// usr/src/AupMain/MainFrm.cpp에서 루틴가져옴
// #include "../../../src/TradingAupDll/TradingAupDef.h
typedef	CWnd * (CreateTradingAup)(UINT nDldresId, CWnd* pParent);
typedef	CWnd * (CreateTradingAupEx)(UINT nDldresId, CWnd* pParent, void* pInterface);
typedef	BOOL (SA_ApplyChart)(LONG dwChartOcx, CListCtrl* pDivList, LONG dwKey);

CWnd *CDllDlg::LoadTradeAup(CString strFile, CWnd *pParent)
{
	CString strTradingName = strFile + ".dll";

	HINSTANCE hTradeInst = LoadLibrary(strTradingName);
	m_hTradeInst = hTradeInst;

	if(hTradeInst == NULL)
	{
		CString strError;
		strError.Format(_T("%s File Load Faile"),strTradingName);
		MessageBox(strError);
		return NULL;
	}

	CreateTradingAup *pCreateTrading = NULL;
	CreateTradingAupEx *pCreateTradingEx = NULL;
	CWnd *pTrade = NULL;

	if(m_pSTInterface)
	{
		pCreateTradingEx = (CreateTradingAupEx *)GetProcAddress(hTradeInst,_T("CREATE_TRADING_FX"));
		if(pCreateTradingEx == NULL)
		{
			MessageBox("특화화면 로드 에러");
			FreeLibrary(hTradeInst);
			return NULL;
		}

		pTrade = pCreateTradingEx(m_nOptimizerDiv,pParent, m_pSTInterface);
		if(pTrade != NULL)
		{
			pParent->SendMessage(WMU_SET_TRADING_INSTANCE,(WPARAM)hTradeInst,0);
		}
		else
		{
			FreeLibrary(hTradeInst);
		}
	}	
	else
	{
		pCreateTrading = (CreateTradingAup *)GetProcAddress(hTradeInst,_T("CREATE_TRADING"));
		if(pCreateTrading == NULL)
		{
			MessageBox("특화화면 로드 에러");
			FreeLibrary(hTradeInst);
			return NULL;
		}

		pTrade = pCreateTrading(m_nOptimizerDiv,pParent);
		if(pTrade != NULL)
		{
			pParent->SendMessage(WMU_SET_TRADING_INSTANCE,(WPARAM)hTradeInst,0);
		}
		else
		{
			FreeLibrary(hTradeInst);
		}
	}
	//if(pTrade)
	//	pTrade->DestroyWindow();
	return pTrade;						
}

LONG CDllDlg::OnSetDllSize(WPARAM wParam, LPARAM lParam)
{
	CPoint *pSize = (CPoint*)wParam;

	CRect frameRect;
	CRect viewRect;
	GetWindowRect(&frameRect);
	GetClientRect(&viewRect);

	int nTheme = nTheme = GetSystemMetrics(SM_CYCAPTION);

	frameRect.right = frameRect.right - 
		GetSystemMetrics(SM_CXEDGE) - 
		GetSystemMetrics(SM_CXDLGFRAME) -
		GetSystemMetrics(SM_CYBORDER);

	frameRect.bottom = frameRect.bottom - GetSystemMetrics(SM_CYEDGE) -
		GetSystemMetrics(SM_CYDLGFRAME) - nTheme -
		GetSystemMetrics(SM_CYBORDER) + SIZE_TITLE_BAR;

	int iNewWidth = frameRect.Width() - viewRect.Width() + pSize->x;
	int iNewHeight = frameRect.Height() - viewRect.Height() + pSize->y;

	SetWindowPos(NULL,0,0,iNewWidth,iNewHeight,SWP_NOMOVE);

	return 0L;
}

typedef	void (DeleteTradingAupEx)(CWnd* pWndTrade);
void CDllDlg::OnDestroy()
{
	if(m_hTradeInst && m_pWndTrade) 
	{
		DeleteTradingAupEx *pDeleteTradingEx = NULL;
		pDeleteTradingEx = (DeleteTradingAupEx *)GetProcAddress(m_hTradeInst,_T("DESTROY_TRADING"));
		pDeleteTradingEx(m_pWndTrade);
		//if(m_szDllName=="HWStrategyAnalysis")
		//{
			FreeLibrary(m_hTradeInst);//KHD : dll이 안내려가는 오류 잡음 : 2006.11.17
			m_hTradeInst = NULL;
		//}
		m_pWndTrade = NULL;
	}

	CDialog::OnDestroy();
}
