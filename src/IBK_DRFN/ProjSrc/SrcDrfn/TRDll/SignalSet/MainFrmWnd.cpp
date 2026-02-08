// MainFrmWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrmWnd.h"
#include "resource.h"

#include "../Dr_Include/Signal_PacketDef.h"

#include "../../../SrcSite/include/axis_msg.h"

#include "NewsSetWnd.h"
#include "ConditionSetWnd.h"
#include "FilterSetWnd.h"
#include "SoundSetWnd.h"

#include "ScreenWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE g_hInstance;
/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd
CMainFrmWnd::CMainFrmWnd()
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);

	//  m_brushStatic.CreateSolidBrush(COLOR_MAIN_CANVAS);
	m_brushWhite.CreateSolidBrush(RGB(255, 255, 255));
	m_brushStatic.CreateSolidBrush(COLOR_MAIN_CANVAS);
	m_brushTitlePoint.CreateSolidBrush(COLOR_TITLE_POINT);


	m_hSignalMng = NULL;

	m_pwndNewsSet = NULL;
	m_pwndSysSet = NULL;
	//m_pwndUserSet = NULL;

	m_pwndFilterSet = NULL;
	m_pwndSoundSet = NULL;

	m_bInit = TRUE;
	m_bIsFnUser = FALSE;

	m_dwAdviseKey = 0;
	m_hChildFrame = NULL;

	m_pCodeInput	= NULL;

	m_ptabCondition = NULL;
}

CMainFrmWnd::~CMainFrmWnd()
{
	m_fontDefault.DeleteObject();
	m_fontBold.DeleteObject();

	m_brushWhite.DeleteObject();
	m_brushStatic.DeleteObject();
	m_brushTitlePoint.DeleteObject();
}


BEGIN_MESSAGE_MAP(CMainFrmWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrmWnd)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP

	ON_BN_CLICKED(IDC_BTN_SETNEWS, OnBtnSet_News)
	ON_BN_CLICKED(IDC_BTN_SETSYS, OnBtnSet_Sys)
	ON_BN_CLICKED(IDC_BTN_SETFILTER, OnBtnSet_Filter)

	ON_BN_CLICKED(IDC_BTN_INIT, OnBtnInit)

	ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)

	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SEARCH, OnChangeTab)

	//	ON_MESSAGE(UM_CHANGE_REGKW, OnNotify_PortPolio)		-hidden-
	//	ON_MESSAGE(UM_CHANGE_PARENT, OnChangeChildFrameWindowHandle)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd message handlers

int CMainFrmWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	m_btnSet_News.Create(_T("뉴스신호"), CRect(0, 0, 0, 0), this, IDC_BTN_SETNEWS, FALSE, TRUE);
	SetBtnImg(&m_btnSet_News, 8);
	m_btnSet_News.ShowWindow(SW_HIDE);		// hidden

	m_ptabCondition = new CDrTabCtrl();
	m_ptabCondition->Create(0, this, IDC_TAB_SEARCH);
	m_ptabCondition->SetBKColor(COLOR_MAIN_CANVAS);	
	m_ptabCondition->SetCurSel(0);

	TC_ITEM item;
	item.mask = TCIF_TEXT | TCIF_IMAGE;
	item.pszText = "시스템신호";
	m_ptabCondition->InsertItem(0, &item);
	
	item.pszText = "신호대상";
	m_ptabCondition->InsertItem(1, &item);

	m_btnSet_Sys.Create(_T("시스템신호"), CRect(0, 0, 0, 0), this, IDC_BTN_SETSYS, FALSE, TRUE);
	SetBtnImg(&m_btnSet_Sys, 8);

	m_btnSet_Filter.Create(_T("신호대상"), CRect(0, 0, 0, 0), this, IDC_BTN_SETFILTER, FALSE, TRUE);
	SetBtnImg(&m_btnSet_Filter, 8);

	m_btnInit.Create(_T("초기화"), CRect(0, 0, 0, 0), this, IDC_BTN_INIT);
	SetBtnImg(&m_btnInit, 4);

	m_btnOk.Create(_T("확인"), CRect(0, 0, 0, 0), this, IDC_BTN_OK);
	SetBtnImg(&m_btnOk, 4);

	m_btnCancel.Create(_T("취소"), CRect(0, 0, 0, 0), this, IDC_BTN_CANCEL);
	SetBtnImg(&m_btnCancel, 4);

	m_btnApply.Create(_T("적용"), CRect(0, 0, 0, 0), this, IDC_BTN_APPLY);
	SetBtnImg(&m_btnApply, 4);

	m_ctrlTitlePoint.Create(_T(""), WS_CHILD | WS_VISIBLE, 
		CRect(0, 0, 0, 0), this, IDC_STATIC_TITLEPOINT);
	m_ctrlTitlePoint.SetFont(&m_fontDefault);
	m_ctrlTitlePoint.ShowWindow(SW_HIDE);

	//	m_ctrlTitle.Create(_T("뉴스신호설정"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
	m_ctrlTitle.Create(_T("시스템신호설정"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this, IDC_STATIC_TITLENAME);
	m_ctrlTitle.SetFont(&m_fontBold);

	AfxSetResourceHandle(hInstSave);

	m_hChildFrame = GetFrameWnd(this);

	return 0;
}


void CMainFrmWnd::SetInit(CMapStringToOb	*pMapControlName)
{
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	m_pwndNewsSet = new CNewsSetWnd;
	m_pwndNewsSet->Create("News Set", this, IDC_WND_NEWSSET);

	m_pwndSysSet = new CConditionSetWnd;
	m_pwndSysSet->Create("Condition Set", 0, this, IDC_WND_SYSSET);

	m_pwndFilterSet = new CFilterSetWnd;
	m_pwndFilterSet->Create(_T("STATIC"), "Filter Set", WS_CHILD | WS_CLIPSIBLINGS /*| WS_BORDER*/,
		CRect(0, 0, 0, 0), this, IDC_WND_FILTERSET);

	m_pwndSoundSet = new CSoundSetWnd;
	m_pwndSoundSet->Create(_T("STATIC"), "Sound Set", WS_CHILD,
		CRect(0, 0, 0, 0), this, IDC_WND_SOUNFSET);	

	AfxSetResourceHandle(hInstSave);
	
	SetLoad_SignalMngLib();

	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();

	if (m_pCodeInput == NULL)
	{
		CRect rtNULL(0, 0, 0, 0);
		
		m_pCodeInput = new CCodeCtrl(pScreenWnd->GetModuleMainWnd());
		m_pCodeInput->SetRootPath(Variant(homeCC));
		m_pCodeInput->UseCodeDlg(TRUE);
		m_pCodeInput->CreateCodeCtrl(this, pScreenWnd->GetModuleMainWnd(), pScreenWnd, rtNULL, IDC_CODECTRL_ITEM_SELECT);
		m_pCodeInput->SetColor(RGB(0,0,0), RGB(255,255,255));
		m_pCodeInput->SetUnit("주식");
	}
	m_hfxCodeCtrl = m_pCodeInput->GetSafeHwnd();

	if (m_hSignalMng && m_pwndSysSet != NULL)
	{
		Lib_GetSignalData("SYS", m_pwndSysSet->m_mapSignalSet);
		m_pwndSysSet->SetInit();
	}

	if (m_hSignalMng && m_pwndFilterSet != NULL)
	{
		m_pwndFilterSet->SetInit(NULL);
		m_pwndFilterSet->SetCodeComboInfo(m_pCodeInput, m_hfxCodeCtrl);
		Lib_GetFilterData((long &)m_pwndFilterSet->m_stFilterInfo);
		m_pwndFilterSet->SetInfo();
	}
	
	
// 	INewsSignalManager* pNSMng = (INewsSignalManager*)AfxGetPctrInterface(UUID_INewsSignalManager, 1);
// 
// 	if (m_pwndNewsSet != NULL)
// 	{
// 		pNSMng->GetSignalData("NEWS", m_pwndNewsSet->m_mapSignalSet);
// 		m_pwndNewsSet->SetInit();
// 	}
// 
// 	if (m_pwndSysSet != NULL)
// 	{
// 		pNSMng->GetSignalData("SYS", m_pwndSysSet->m_mapSignalSet);
// 		m_pwndSysSet->SetInit();
// 	}
// 
// 	if (m_pwndFilterSet != NULL)
// 	{
// 		/*m_pwndFilterSet->SetInit(pMapControlName);*/
// 
// 		m_pwndFilterSet->SetInit(NULL);
// 		m_pwndFilterSet->SetCodeComboInfo(m_pCodeCombo, m_hCodeCombo);
// 		pNSMng->GetFilterData((long &)m_pwndFilterSet->m_stFilterInfo);
// 		m_pwndFilterSet->SetInfo();
// 	}
	
	
	SetChangeTab(1);
	SetInit_LibPortPolio(TRUE);

	m_bInit = TRUE;

	//	ChangeSkinColor();
	OnSize();
	/*OnBtnInit();*/
}

//extern int Dll_SetRegKwCombo(CWnd* pWnd, int nType);  //# nType 1-추가, 0-삭제	-hidden-
void CMainFrmWnd::SetInit_LibPortPolio(BOOL bInit)
{
	//	Dll_SetRegKwCombo(this, (bInit == TRUE)?1:0);
}

void CMainFrmWnd::OnNotify_PortPolio(WPARAM wParam, LPARAM lParam)
{
	if (m_pwndFilterSet != NULL)
		m_pwndFilterSet->GetPortData();
}

BOOL CMainFrmWnd::SetLoad_SignalMngLib()
{
	BOOL bResult  = FALSE;

	m_hSignalMng = LoadLibrary("lib_signalmng.dll");
	if(m_hSignalMng)
	{
		Lib_Open		= (_DLL_SetLibOpen)GetProcAddress(m_hSignalMng, "_DLL_SetLibOpen");
		Lib_Close		= (_DLL_SetLibClose)GetProcAddress(m_hSignalMng, "_DLL_SetLibClose");
		
		Lib_SetSignal	= (_DLL_SetSiganl)GetProcAddress(m_hSignalMng, "_DLL_SetSiganl");
		
		Lib_GetSignalData	= (_DLL_GetSignalData)GetProcAddress(m_hSignalMng, "_DLL_GetSignalData");
		Lib_SetSignalData	= (_DLL_SetSignalData)GetProcAddress(m_hSignalMng,"_DLL_SetSignalData");

		Lib_GetFilterData = (_DLL_GetFilterData)GetProcAddress(m_hSignalMng, "_DLL_GetFilterData");
		Lib_SetFilterData = (_DLL_SetFilterData)GetProcAddress(m_hSignalMng, "_DLL_SetFilterData");
		Lib_SetDefSysSignalData = (_DLL_SetDefSysSignalData)GetProcAddress(m_hSignalMng,"_DLL_SetDefSysSignalData"); // 0016487: [7134] 신호검색 설정화면에서 신호 항목 변경후 [초기화] 클릭시 정렬만 되는 현상 수정
		
		Lib_Open(this, NULL, GetModuleMain());
		
		bResult = TRUE;
	}

	/*
	m_hSignalMng = LoadLibrary("Lib_SignalMng.dll");
	if (m_hSignalMng)
	{
	Lib_SetLibOpen = (_DLL_SetLibOpen)::GetProcAddress(m_hSignalMng, "_DLL_SetLibOpen");
	Lib_SetLibClose = (_DLL_SetLibClose)::GetProcAddress(m_hSignalMng, "_DLL_SetLibClose");

	//Lib_SetSiganl = (_DLL_SetSiganl)::GetProcAddress(m_hSignalMng, "_DLL_SetSiganl");

	Lib_GetSignalData = (_DLL_GetSignalData)::GetProcAddress(m_hSignalMng, "_DLL_GetSignalData");
	Lib_SetSignalData = (_DLL_SetSignalData)::GetProcAddress(m_hSignalMng, "_DLL_SetSignalData");

	Lib_GetFilterData = (_DLL_GetFilterData)::GetProcAddress(m_hSignalMng, "_DLL_GetFilterData");
	Lib_SetFilterData = (_DLL_SetFilterData)::GetProcAddress(m_hSignalMng, "_DLL_SetFilterData");

	bResult = TRUE;
	}
	else
	TRACE("\n# Lib_SignalMng.dll이 없습니다.");
	*/

	return bResult;
}

HBRUSH CMainFrmWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFrameWnd::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if (pWnd->GetDlgCtrlID() == IDC_STATIC_TITLEPOINT)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(COLOR_TITLE_POINT);
			hbr = m_brushTitlePoint;

		}
		else if(pWnd->GetDlgCtrlID() == IDC_STATIC_TITLENAME)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(m_clrMainSkin);
			hbr = m_brushStatic;
		}
		else
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(GetAxColor(66));//RGB(255,255,255));//tour2k backgroud color
			hbr = GetAxBrush(GetAxColor(66)); //m_brushWhite;
		}
	}

	return hbr;
}

void CMainFrmWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CFrameWnd::OnShowWindow(bShow, nStatus);

}

void CMainFrmWnd::OnDestroy() 
{
	//	CFrameWnd::OnDestroy();

	if(m_hSignalMng)
	{
		Lib_Close(this);
		FreeLibrary(m_hSignalMng);
		m_hSignalMng = NULL;
	}

	SetInit_LibPortPolio(FALSE);

	if (m_pwndNewsSet != NULL)
	{
		m_pwndNewsSet->DestroyWindow();
		delete m_pwndNewsSet;
		m_pwndNewsSet = NULL;
	}

	if (m_pwndSysSet != NULL)
	{
		m_pwndSysSet->DestroyWindow();
		delete m_pwndSysSet;
		m_pwndSysSet = NULL;
	}

	/*
	if (m_pwndUserSet != NULL)
	{
	m_pwndUserSet->DestroyWindow();
	delete m_pwndUserSet;
	}
	*/

	if (m_pwndFilterSet != NULL)
	{
		m_pwndFilterSet->DestroyWindow();
		delete m_pwndFilterSet;
		m_pwndFilterSet = NULL;
	}

	if (m_pwndSoundSet != NULL)
	{
		m_pwndSoundSet->DestroyWindow();
		delete m_pwndSoundSet;
		m_pwndSoundSet = NULL;
	}
	
	if(m_pCodeInput)
	{
		m_pCodeInput->DestroyWindow();
		delete m_pCodeInput;
		m_pCodeInput = NULL;
	}

	if(m_ptabCondition != NULL)
	{
		delete m_ptabCondition;
		m_ptabCondition = NULL;
	}

	CFrameWnd::OnDestroy();
}

void CMainFrmWnd::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);

	if (m_bInit == TRUE)
		OnSize();
}

void CMainFrmWnd::OnSize() 
{
	CRect reClient, reBottom, reTemp, reTab;
	GetClientRect(reClient);

	if(reClient.bottom < 180)
		reClient.bottom = 180;

	reClient.top -= 2;
	reClient.bottom -= 2;
 	reClient.left += 2;
 	reClient.right -= 2;

	reBottom = reClient;
	reBottom.top = reBottom.bottom - 20;
	OnSize_Bottom(reBottom);

	reTab = reClient;
	reTab.top += 2;
	reTab.left -= 3;
	reTab.bottom = reTab.top + 20;
	reTab.right += 2;
	OnSize_Left(reTab);

	m_rtTab = reTab;
	m_rtTab.top += 25;
	m_rtTab.left += 5;
	m_rtTab.bottom = reBottom.top - 6;
	m_rtTab.right -= 5;

	reTemp = reTab;
	reTemp.top = reTemp.bottom + 3;
	reTemp.left += 11;
	reTemp.bottom = reBottom.top - 6;
	reTemp.right -= 10;
	OnSize_Right(reTemp);

	OnPF_Contorl_Move();
}

void CMainFrmWnd::OnSize_Left(CRect reClient)
{
	CRect reTop;
	m_reSetBar = reClient;

	reClient.top += 4 + 1;
	reClient.left += 4 + 1;
	reClient.right -= 4 + 1;
	reTop = reClient;

	// -hidden-	
	reTop.bottom = reTop.top + 22;
	/*m_btnSet_News.MoveWindow(&reTop);*/
	
	//m_btnSet_Sys.MoveWindow(&reTop);
	m_ptabCondition->MoveWindow(&reTop);

	//   BOOL bSetView_Sys = FALSE;
	//   if (m_bIsFnUser == TRUE)
	// 	  bSetView_Sys = TRUE;
	//   
	//   if (bSetView_Sys == TRUE)
	//   {
	// 	  reTop.top = reTop.bottom + 3;//	-hidden-
	// 	  reTop.bottom = reTop.top + 22;
	// 	  m_btnSet_Sys.MoveWindow(&reTop); 
	//   }

// 	reTop.top = reTop.bottom + 3;
// 	reTop.bottom = reTop.top + 22;
// 	m_btnSet_Filter.MoveWindow(&reTop);
}

void CMainFrmWnd::OnSize_Right(CRect reClient)
{
	CRect reTop, reLine;
	reTop = reClient;

	reTop.bottom = reTop.top + 5;
	reLine = reTop;
	reLine.right = reLine.left + 8;
	m_ctrlTitlePoint.MoveWindow(&reLine);

	reLine.left = reLine.right + 8;
	reLine.right = reLine.left + 150;

//	reLine.top += 5;
//	reLine.left -= 5;
// 	reLine.bottom = reLine.top + 15;
// 	m_ctrlTitle.MoveWindow(&reLine);

	reTop.top = reTop.bottom;
	reTop.bottom = reClient.bottom - 5;
	if (m_pwndNewsSet != NULL)
		m_pwndNewsSet->MoveWindow(&reTop);

	if (m_pwndSysSet != NULL)
		m_pwndSysSet->MoveWindow(&reTop);

	if (m_pwndFilterSet != NULL)
		m_pwndFilterSet->MoveWindow(&reTop);

	if (m_pwndSoundSet != NULL)
		m_pwndSoundSet->MoveWindow(&reTop);
}

void CMainFrmWnd::OnSize_Bottom(CRect reClient)
{
	CRect reRight, reLeft;

	reClient.top -= 2;
	reClient.bottom -= 2;

	reRight = reClient;
	reLeft = reClient;

	reRight.right -= 2;
	reRight.left = reRight.right - 60;
	m_btnCancel.MoveWindow(&reRight);

	reRight.right = reRight.left - 2;
	reRight.left = reRight.right - 60;
	m_btnOk.MoveWindow(&reRight);

	reRight.right = reRight.left - 2;
	reRight.left = reRight.right - 60;
	m_btnApply.MoveWindow(&reRight);

	reLeft.left += 2;
	reLeft.right = reLeft.left + 60;
	m_btnInit.MoveWindow(&reLeft);
}

void CMainFrmWnd::OnBtnSet_News()
{
	SetChangeTab(0);
	m_ctrlTitle.SetWindowText("뉴스신호설정");
}

void CMainFrmWnd::OnBtnSet_Sys()
{
	SetChangeTab(1);

	m_ctrlTitle.SetWindowText("시스템신호설정");
}

void CMainFrmWnd::OnBtnSet_Filter()
{
	SetChangeTab(2);
	m_ctrlTitle.SetWindowText("신호대상설정");
}

void CMainFrmWnd::SetChangeTab(int nCurSel)
{
	m_pwndNewsSet->ShowWindow(SW_HIDE);
	m_pwndSysSet->ShowWindow(SW_HIDE);
	m_pwndFilterSet->ShowWindow(SW_HIDE);

	if (m_pwndFilterSet->m_pctrlCodeInput != NULL)
		m_pwndFilterSet->m_pctrlCodeInput->ShowWindow(SW_HIDE);

	m_pwndSoundSet->ShowWindow(SW_HIDE);

	if (nCurSel == 0)
	{
		m_pwndNewsSet->ShowWindow(SW_SHOW);

		m_pwndNewsSet->m_bShow_PF_CTL = PF_CONTROL_SHOW;
		m_pwndFilterSet->m_bShow_PF_CTL = PF_CONTROL_HIDE;
		m_pwndSysSet->m_bShow_PF_CTL = PF_CONTROL_HIDE;
	}
	else if (nCurSel == 1)
	{
		m_pwndSysSet->ShowWindow(SW_SHOW);

		m_pwndNewsSet->m_bShow_PF_CTL = PF_CONTROL_HIDE;
		m_pwndFilterSet->m_bShow_PF_CTL = PF_CONTROL_HIDE;
		m_pwndSysSet->m_bShow_PF_CTL = PF_CONTROL_SHOW;
	}
	else if (nCurSel == 2)
	{
		//m_pwndFilterSet->SetInfo();
		m_pwndFilterSet->ShowWindow(SW_SHOW);
		if (m_pwndFilterSet->m_pctrlCodeInput != NULL)
		{
			m_pwndFilterSet->SetPosCodeInput();
			m_pwndFilterSet->m_pctrlCodeInput->ShowWindow(SW_SHOW);
		}
		m_pwndNewsSet->m_bShow_PF_CTL = PF_CONTROL_HIDE;
		m_pwndFilterSet->m_bShow_PF_CTL = PF_CONTROL_SHOW;
		m_pwndSysSet->m_bShow_PF_CTL = PF_CONTROL_HIDE;
	}
	else if (nCurSel == 3)
	{
		m_pwndSoundSet->ShowWindow(SW_SHOW);

		m_pwndNewsSet->m_bShow_PF_CTL = PF_CONTROL_HIDE;
		m_pwndFilterSet->m_bShow_PF_CTL = PF_CONTROL_HIDE;
		m_pwndSysSet->m_bShow_PF_CTL = PF_CONTROL_HIDE;
	}
	if(m_pwndNewsSet) m_pwndNewsSet->OnPF_Contorl_Move();
	if(m_pwndSysSet) m_pwndSysSet->OnPF_Contorl_Move();
	if(m_pwndFilterSet) m_pwndFilterSet->OnPF_Contorl_Move();
}

void CMainFrmWnd::OnBtnInit()
{
	m_pwndNewsSet->SetInit_SetVal();
	m_pwndSysSet->SetInit_SetVal();
	m_pwndFilterSet->SetInit_SetVal();
}

void CMainFrmWnd::OnBtnOk()
{
	OnBtnApply();
	keybd_event(0x1b, 0, 0, 0);
}

void CMainFrmWnd::OnBtnCancel()
{
	keybd_event(0x1b, 0, 0, 0);
}

void CMainFrmWnd::OnBtnApply()
{
	if(Lib_SetSignalData)
	{
		Lib_SetSignalData("SYS", &m_pwndSysSet->m_mapSignalSet);

		if(m_pwndFilterSet->GetInfo())
			Lib_SetFilterData((long*)&m_pwndFilterSet->m_stFilterInfo);
		else
			OnBtnSet_Filter();
	}
}

void CMainFrmWnd::Debug_SignalSetInfo(CMapStringToPtr *pmapSignalSet)
{
	POSITION pos;
	CString strKey;
	SetSignalData	*pstSetSignalData;

	CString strTemp;
	for (pos = pmapSignalSet->GetStartPosition(); pos != NULL;)
	{
		pmapSignalSet->GetNextAssoc(pos, strKey, (void*&)pstSetSignalData);

		strTemp.Format("\n# %s", pstSetSignalData->strTitle);
		TRACE(strTemp);
	}
}

int CMainFrmWnd::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	return MessageBox(lpszText, lpszCaption, nType);
}

void CMainFrmWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect reClient;
	GetClientRect(reClient);
	//	dc.FillSolidRect(reClient, COLOR_MAIN_CANVAS);
	dc.FillSolidRect(reClient, m_clrMainSkin);


	dc.FillSolidRect(m_rtTab, GetAxColor(66));

	HPEN hOldPen;
	hOldPen = (HPEN)dc.SelectObject(GetAxPen(GetAxColor(13), 1, PS_SOLID));
	
	MoveToEx(dc.m_hDC, m_rtTab.right, m_rtTab.top + 1, NULL);		// 오른쪽 테두리
	LineTo(dc.m_hDC, m_rtTab.right,  m_rtTab.bottom);
	
	MoveToEx(dc.m_hDC, m_rtTab.right, m_rtTab.bottom, NULL);	// 아래 테두리
	LineTo(dc.m_hDC, m_rtTab.left, m_rtTab.bottom);
	
	MoveToEx(dc.m_hDC, m_rtTab.left, m_rtTab.bottom, NULL);			// 왼쪽 테두리
	LineTo(dc.m_hDC, m_rtTab.left, m_rtTab.top);
	dc.SelectObject(hOldPen);

// 	CRect rcClient;
// 	GetClientRect(&rcClient);
// 	rcClient.bottom = rcClient.top + 2;
// 	dc.FillSolidRect(rcClient, m_clrMainSkin);

	//SetDrewBox(&dc, m_reSetBar, COLOR_MENULINE);
}

void CMainFrmWnd::SetDrewBox(CPaintDC *pDC, CRect reData, COLORREF clLine)
{
	CPen *pOldPen;
	CPen penLine(PS_SOLID, 0, clLine);

	pOldPen = pDC->SelectObject(&penLine);
	pDC->MoveTo(reData.left, reData.top);
	pDC->LineTo(reData.right, reData.top);
	pDC->LineTo(reData.right, reData.bottom);
	pDC->LineTo(reData.left, reData.bottom);
	pDC->LineTo(reData.left, reData.top);

	pDC->SelectObject(pOldPen);
	penLine.DeleteObject();
}

// void CMainFrmWnd::ChangeSkinColor()
// {
// //	m_clrMainSkin = GetParent()->SendMessage(RMSG_GET_SKINCOLOR, 0, 0);
// 	m_clrMainSkin = RGB(255, 255, 255);
// 	m_brushStatic.DeleteObject();
// 	m_brushStatic.CreateSolidBrush(m_clrMainSkin);	
// }

BOOL CMainFrmWnd::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_RBUTTONUP)
	{
		CPoint point = pMsg->pt;
		FnRButtonUp(point);
	}

	return CFrameWnd::PreTranslateMessage(pMsg);
}
void CMainFrmWnd::FnRButtonUp(CPoint point)
{
	TRACE("\nFnRButtonUp called\n");
	SetCommMenu(&point);
}

extern void KSLink_SetFormLinkTagData( CString sTag, CString sValue );
//extern void KSLink_SetLinkTagData( LPCTSTR sTag, LPCTSTR sValue);
void CMainFrmWnd::SetCommMenu(POINT *point)
{
	/*	-hidden-
	HWND hChildFrame = m_hChildFrame;
	if(!hChildFrame) return;

	long lMenu = ::SendMessage(hChildFrame, UM_GET_SCREENLINKMENU, 0, 3 );
	if ( lMenu > 0 )
	{
	CMenu* pMenu = (CMenu*)lMenu;
	ASSERT ( pMenu );

	GetCursorPos(point);
	UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
	int nRetValue = pMenu->TrackPopupMenu(nFlagsForMenu, point->x, point->y, this);
	pMenu->DestroyMenu();
	delete pMenu;

	if( nRetValue>0 )
	{
	::PostMessage(hChildFrame, WM_COMMAND, nRetValue, 0);
	return;
	}
	}
	*/
}

HWND CMainFrmWnd::GetFrameWnd(CWnd* pWnd)
{
	CWnd* pParentWnd = pWnd->GetParent();  // start with one parent up
	while (pParentWnd != NULL)
	{
		DWORD dwExStyle = GetWindowLong(pParentWnd->GetSafeHwnd(), GWL_STYLE);
		if ( dwExStyle & 0x8000)
		{
			return pParentWnd->GetSafeHwnd();
		}
		pParentWnd = pParentWnd->GetParent();
	}
	return NULL;
}

long CMainFrmWnd::OnChangeChildFrameWindowHandle( WPARAM wParam, LPARAM lParam)
{
	m_hChildFrame = (HWND)lParam;

	return 0L;
}

void CMainFrmWnd::ChangeSkinColor(COLORREF clrBkgnd, ST_TABCOLOR stTabColor)
{
	if(!clrBkgnd)
		return;

	m_clrMainSkin = clrBkgnd;

	if(m_brushStatic.DeleteObject())
		m_brushStatic.CreateSolidBrush(m_clrMainSkin);

	m_ptabCondition->SetTabColor(m_clrMainSkin, m_clrMainSkin, stTabColor.clrB, NULL, NULL, m_clrMainSkin);
	m_ptabCondition->SetTextColor(stTabColor.clrSF, stTabColor.clrSF);

	if(m_pwndSysSet)
		m_pwndSysSet->ChangeSkinColor(m_clrMainSkin);

	if(m_pwndFilterSet)
		m_pwndFilterSet->ChangeSkinColor(m_clrMainSkin);

	m_ctrlTitle.Invalidate();
	m_ptabCondition->Invalidate();

	Invalidate();	
}

void CMainFrmWnd::PlatFormBtnSystem()
{
	OnBtnSet_Sys();
}
void CMainFrmWnd::PlatFormBtnTarget()
{
	OnBtnSet_Filter();
}
void CMainFrmWnd::PlatFormBtnInit()
{
	OnBtnInit();
}
void CMainFrmWnd::PlatFormBtnOK()
{
	OnBtnOk();
}
void CMainFrmWnd::PlatFormBtnCancel()
{
	OnBtnCancel();
}
void CMainFrmWnd::PlatFormBtnApply()
{
	OnBtnApply();
}
void CMainFrmWnd::PlatFormBtnNewsSignal()
{
	OnBtnSet_News();
}

void CMainFrmWnd::OnPF_Contorl_Move()
{
	ST_MOVEITEM stItem;	
	stItem.nType = PF_ITEM_BTN;

	SET_ST_MOVEITEM(stItem, "btnSystem", &m_btnSet_Sys);	
	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);

	SET_ST_MOVEITEM(stItem, "btnTarget", &m_btnSet_Filter);
	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);

	SET_ST_MOVEITEM(stItem, "btnInit", &m_btnInit);
	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);

	SET_ST_MOVEITEM(stItem, "btnOK", &m_btnOk);
	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);

	SET_ST_MOVEITEM(stItem, "btnCancel", &m_btnCancel);
	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);

	SET_ST_MOVEITEM(stItem, "btnApply", &m_btnApply);
	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);	

	// 	SET_ST_MOVEITEM(stItem, "btnNewsSignal", &m_btnSet_News);
	// 	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);
}

LRESULT CMainFrmWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{	
	switch(message)
	{
	case MF_PFCONTORL_MOVE:	
		{	
			CWnd *pWnd = GetParent();
			if(!pWnd) return FALSE;
			GetParent()->SendMessage(RMSG_KSMOVECTRL, wParam, lParam);
			return TRUE;
		}
	case MF_PFCONTORL_EVENT:
		{
			CWnd *pWnd = GetParent();
			if(!pWnd) return FALSE;
			GetParent()->SendMessage(RMSG_PF_CONTROL_EVENT, wParam, lParam);
			return TRUE;
		}
	}
	return CFrameWnd::WindowProc(message, wParam, lParam);
}

void CMainFrmWnd::SetBtnImg(CfxImgButton *pBtn, CString strFileName)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	pScreenWnd->SetBtnImg(pBtn, strFileName);
}

void CMainFrmWnd::SetBtnImg(CfxImgButton *pBtn, int nTextLen, BOOL bBold)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	pScreenWnd->SetBtnImg(pBtn, nTextLen, bBold);
}

void CMainFrmWnd::GetData_PortGroup(CStringArray& saKey, CStringArray& saGroupName)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
#if (0)//tour2k
	pScreenWnd->GetData_PortGroup(saKey, saGroupName);
#else
	//	sprintf( sSend,"%s%80s%s", "MYUg", " ", "00001U");

	int sendL = 0;
	char sendB[16 * 1024] = { 0, }, tempB[32];
	int isz_updn = sz_uinfo;
	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += isz_updn;
	FillMemory((char*)updn, isz_updn, ' ');
	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'g';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'U';

	pScreenWnd->SendTR(TRNO_FAVORITE, sendB, sendL, TRKEY_GWANSIMGROUP, US_PASS);
#endif
}

CWnd* CMainFrmWnd::GetModuleMain()
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetModuleMainWnd();
}

CString CMainFrmWnd::Variant(int nComm, CString strData)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	
	return pScreenWnd->Variant(nComm, strData);
}

CFont* CMainFrmWnd::GetFont(BOOL bBold)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	
	CFont* pFont;
	
	if(!bBold)
		pFont = pScreenWnd->GetDefFont();
	else
		pFont = pScreenWnd->GetBoldFont();
	
	return pFont;
}

HBRUSH CMainFrmWnd::GetAxBrush(COLORREF clr)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return (HBRUSH)(*pScreenWnd->GetAxBrush(clr));
}

COLORREF CMainFrmWnd::GetColor(int nColor)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	
	return pScreenWnd->GetAxColor(nColor);
}

COLORREF CMainFrmWnd::GetAxColor(int nColor)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetAxColor(nColor);
}

CPen* CMainFrmWnd::GetAxPen(COLORREF clr, int nWidth, int nStyle)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetAxPen(clr, nWidth, nStyle);
}

void CMainFrmWnd::OnChangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nTabIndex = m_ptabCondition->GetCurSel();

	if(nTabIndex == 0)
		SetChangeTab(1);
	else
		SetChangeTab(2);
}

//--> 0016487: [7134] 신호검색 설정화면에서 신호 항목 변경후 [초기화] 클릭시 정렬만 되는 현상 수정
void CMainFrmWnd::GetSignalDef()
{
	if (m_hSignalMng && m_pwndSysSet != NULL)
	{
		Lib_SetDefSysSignalData(&m_pwndSysSet->m_mapSignalSet);
	}
}

void CMainFrmWnd::SetWaitCursor(BOOL bState)
{
	if (bState == TRUE)
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	else
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}

HRESULT CMainFrmWnd::ReceiveData(LPCSTR szTR, LPVOID aTRData, long dwTRDateLen, int nKey)
{
	char* pchData;
	CString strData;

	CString strMsg;
	if (dwTRDateLen == 0)		//# 0 : err
	{
		SetWaitCursor(FALSE);
		// 2011.11.10 박성경
		//SetSendMainMsg(szMessage);
	}
	else if (dwTRDateLen > 0)
	{
		if ((CString)szTR == TRNO_FAVORITE) //tour2k 관심종목 그룹
		{
			char* pReceiveBuff = new char[dwTRDateLen + 1];
			memset(pReceiveBuff, 0x00, dwTRDateLen + 1);
			memcpy(pReceiveBuff, aTRData, dwTRDateLen);

			if (nKey == TRKEY_GWANSIMGROUP)//관심종목
			{
				CStringArray arrName, arrKey;
				ParseGroup(pReceiveBuff, arrKey, arrName);

				for (int nIndex = 0; nIndex < arrKey.GetSize(); nIndex++)
					m_pwndFilterSet->m_comboPortGroup.AddString(arrName.GetAt(nIndex));

				if (arrName.GetSize() > 0)
					m_pwndFilterSet->m_comboPortGroup.SetCurSel(0);
			}
			delete[] pReceiveBuff; pReceiveBuff = nullptr;
		}
	}

	SetWaitCursor(FALSE);
	return 0L;
}

int CMainFrmWnd::ParseGroup(char* pReceiveBuff, CStringArray& arrKey, CStringArray& arrName)
{
	int cnt = atoi(CString(pReceiveBuff, 4));
	int parseL = 4;

	struct _grSearch
	{
		char ngrs[2];
		char grseq[2];
		char gname[30];
	};

	struct _grSearch* stgr;
	char* pSeq, * pgame;
	pSeq = new char[sizeof(stgr->grseq) + 1];
	pgame = new char[sizeof(stgr->gname) + 1];

	for (int ii = 0; ii < cnt; ii++)
	{
		stgr = (struct _grSearch*)&pReceiveBuff[parseL];
		memset(pSeq, 0x00, sizeof(stgr->grseq) + 1);
		memcpy(pSeq, stgr->grseq, sizeof(stgr->grseq));
		m_pwndFilterSet->m_GwanSimGroup.strKey.Add(pSeq);
		arrKey.Add(pSeq);

		memset(pgame, 0x00, sizeof(stgr->gname) + 1);
		memcpy(pgame, stgr->gname, sizeof(stgr->gname));

		m_pwndFilterSet->m_GwanSimGroup.strGroupName.Add(pgame);
		arrName.Add(pgame);

		parseL += sizeof(struct _grSearch);

	}

	delete[] pSeq; pSeq = nullptr;
	delete[] pgame; pgame = nullptr;

	return 0;
}