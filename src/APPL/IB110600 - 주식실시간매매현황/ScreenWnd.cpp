// CScreenWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ib110600.h"
#include "MapWnd.h"

#include "ScreenWnd.h"
#include "DllCtrl.h"
#include "DllCtrl2.h"

#include "SplitterCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreenWnd

CScreenWnd::CScreenWnd(class CMapWnd *pParent)
	:m_pMapWnd(pParent)
{
	m_pOrderDll = NULL;
	m_pSignedDll = NULL;
	m_pBalanceDll = NULL;

	m_pSplitter = NULL;

	m_bBtnState[0] = FALSE;
	m_bBtnState[1] = FALSE;
	m_bBtnState[2] = FALSE;

	m_bClick = FALSE;
}

CScreenWnd::~CScreenWnd()
{
//	SAFE_DELETE(m_pOrderDll);
//	SAFE_DELETE(m_pSignedDll);
//	SAFE_DELETE(m_pBalanceDll);
	
//	SAFE_DELETE(m_pSplitter);
}


BEGIN_MESSAGE_MAP(CScreenWnd, CWnd)
	//{{AFX_MSG_MAP(CScreenWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CScreenWnd message handlers

int CScreenWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CreateControl();

	return 0;
}

void CScreenWnd::CreateControl()
{
	CRect mapRc;
	GetClientRect(mapRc);

	m_pSplitter = std::make_unique<SplitterCtrl>();			// 화면 분할 오브젝트
	m_pSplitter->ShowBorder(false);
	m_pSplitter->Create(this, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, mapRc, IDC_HSPLITTER);

	m_pOrderDll = std::make_unique <CDllCtrl>(this);
	m_pOrderDll->Create(NULL, "", WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, CRect(), m_pSplitter.get(), IDC_SCREEN_ORDER);
	m_pOrderDll->LoadMap("IB110601", "주문내역");

	m_pSignedDll = std::make_unique <CDllCtrl2>(this);
	m_pSignedDll->Create(NULL, "", WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, CRect(), m_pSplitter.get(), IDC_SCREEN_SIGNED);
	m_pSignedDll->LoadMap2("IB11060A", "IB11060B", "미체결내역", "체결상세내역");

	m_pBalanceDll = std::make_unique <CDllCtrl>(this);
	m_pBalanceDll->Create(NULL, "", WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, CRect(), m_pSplitter.get(), IDC_SCREEN_BALANCE);
	m_pBalanceDll->LoadMap("IB110603", "잔고평가");

	// 화면 분할 설정
	m_pSplitter->AddColumn();
	m_pSplitter->AddRow();
	m_pSplitter->AddRow();
	m_pSplitter->AddRow();
	m_pSplitter->SetMinHeight(MAP_MIN_HEIGHT);
	
	m_pSplitter->SetWindow(0, 0, m_pOrderDll->GetSafeHwnd());
	m_pSplitter->SetWindow(1, 0, m_pSignedDll->GetSafeHwnd());
	m_pSplitter->SetWindow(2, 0, m_pBalanceDll->GetSafeHwnd());
	
	m_pSplitter->SetHeightForStatic(0, 175);
	m_pSplitter->SetHeightForStatic(1, 175);
	m_pSplitter->SetHeightForStatic(2, 175);

	m_pSplitter->Update();
}

void CScreenWnd::SetClickBtn(BOOL bClick)
{
	m_bClick = bClick;
}

void CScreenWnd::SetScreenAccount(CString strParam, CString strAccount)
{
	//int nKey = 0;
	//CString strDat = "";

	m_pOrderDll->SetScreenParam(strParam, strAccount);
	m_pSignedDll->SetScreenParam(strParam, strAccount);
	m_pBalanceDll->SetScreenParam(strParam, strAccount);
}

void CScreenWnd::SetPal()
{
	m_pOrderDll->Invalidate();
	m_pSignedDll->Invalidate();
	m_pBalanceDll->Invalidate();
}

CMapWnd* CScreenWnd::GetMapWnd()
{
	return m_pMapWnd;
}

int CScreenWnd::GetScreenHeight()
{
	int nMapHeight[3]{};

	nMapHeight[0] = m_bBtnState[0] ? MAP_MIN_HEIGHT : m_pOrderDll->GetMapHeight();
	nMapHeight[1] = m_bBtnState[1] ? MAP_MIN_HEIGHT : m_pSignedDll->GetMapHeight();
	nMapHeight[2] = m_bBtnState[2] ? MAP_MIN_HEIGHT : m_pBalanceDll->GetMapHeight();

	const int m_nTotalHeight = nMapHeight[0] + nMapHeight[1] + nMapHeight[2];

	return m_nTotalHeight + 2*SPLITTER_BORDER;
}

void CScreenWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	m_pSplitter->SetDraggingMode(SPLITTERCTRL_DRAGGING_DYNAMIC);
	m_pSplitter->SetResizeMode(SPLITTERCTRL_RESIZE_DYNAMIC);

	if(m_bClick)
		BtnClickReSize(cx, cy);			//버튼 클릭시 리사이징
	else
		ScreenReSize(cx, cy);			//기본 movewindow 시
}

void CScreenWnd::BtnClickReSize(int cx, int cy)
{
	m_bClick = FALSE;
	m_pSplitter->SetDraggingMode(SPLITTERCTRL_DRAGGING_STATIC);
	m_pSplitter->SetResizeMode(SPLITTERCTRL_RESIZE_STATIC);

	int nHeight = m_bBtnState[0] ? MAP_MIN_HEIGHT : m_pOrderDll->GetMapHeight();
	m_pSplitter->SetHeightForStatic(0, nHeight);
	
	nHeight = m_bBtnState[1] ? MAP_MIN_HEIGHT : m_pSignedDll->GetMapHeight();
	m_pSplitter->SetHeightForStatic(1, nHeight);

	nHeight = m_bBtnState[2] ? MAP_MIN_HEIGHT : m_pBalanceDll->GetMapHeight();
	m_pSplitter->SetHeightForStatic(2, nHeight);
	
	m_pSplitter->Update();
	m_pSplitter->MoveWindow(0, 0, cx, cy);
}

void CScreenWnd::ScreenReSize(int cx, int cy)
{
	CRect cRc;
	int nRemainHeight = 0;

	m_pSplitter->MoveWindow(0, 0, cx, cy);

	m_pSplitter->SetDraggingMode(SPLITTERCTRL_DRAGGING_STATIC);
	m_pSplitter->SetResizeMode(SPLITTERCTRL_RESIZE_STATIC);

	if(m_bBtnState[2])
	{
		m_pSplitter->SetHeightForStatic(2, MAP_MIN_HEIGHT);
		
		if(m_bBtnState[0])
		{
			nRemainHeight = cy - 2*MAP_MIN_HEIGHT - SPLITTER_BORDER*2;

			m_pSplitter->SetHeightForStatic(0, MAP_MIN_HEIGHT);
			if(nRemainHeight > 0)
				m_pSplitter->SetHeightForStatic(1, nRemainHeight);
		}
		else
		{
			m_pSplitter->GetWindowRect(1, 0, &cRc);
			const int nSignedHeight = cRc.Height();

			if(m_bBtnState[1])
			{
				nRemainHeight = cy - 2*MAP_MIN_HEIGHT - SPLITTER_BORDER*2;
				if(nRemainHeight > 0)
					m_pSplitter->SetHeightForStatic(0, nRemainHeight);
				m_pSplitter->SetHeightForStatic(1, MAP_MIN_HEIGHT);
			}
			else
			{
				m_pSplitter->GetWindowRect(0, 0, &cRc);
				const int nOrderHeight = cRc.Height();
				nRemainHeight = cy - MAP_MIN_HEIGHT - nOrderHeight - nSignedHeight - SPLITTER_BORDER*2;
				if(nOrderHeight + nRemainHeight / 2 > 0)
					m_pSplitter->SetHeightForStatic(0, nOrderHeight + nRemainHeight/2);

				if(nSignedHeight + nRemainHeight / 2 > 0)
					m_pSplitter->SetHeightForStatic(1, nSignedHeight + nRemainHeight/2);
			}
		}
	}
	
	m_pSplitter->Update();
}

void CScreenWnd::GetBtnState(BOOL *btn)
{
	btn[0] = m_bBtnState[0];
	btn[1] = m_bBtnState[1];
	btn[2] = m_bBtnState[2];
}

void CScreenWnd::SetBtnState()
{
	m_bBtnState[0] = m_pOrderDll->GetBtnState();
	m_bBtnState[1] = m_pSignedDll->GetBtnState();
	m_bBtnState[2] = m_pBalanceDll->GetBtnState();
}
