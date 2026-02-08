// ScrTest002View.cpp : implementation file
//

#include "stdafx.h"
#include "PortAnalystView.h"
#include "AnalystMainFrmWnd.h"
#include "../../ITGEN/CommonSrc/CtlColor.h"
#include "../../ChartDll/Include_Chart/Dll_Load/IMetaTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE g_hInstance;

/////////////////////////////////////////////////////////////////////////////
// CPortAnalystView

IMPLEMENT_DYNCREATE(CPortAnalystView, CScrFormView)

CPortAnalystView::CPortAnalystView()
	: CScrFormView(CPortAnalystView::IDD)
{
	//{{AFX_DATA_INIT(CPortAnalystView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pwndMainFrm = NULL;
}

CPortAnalystView::~CPortAnalystView()
{
}	

void CPortAnalystView::DoDataExchange(CDataExchange* pDX)
{
	CScrFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortAnalystView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPortAnalystView, CScrFormView)
	//{{AFX_MSG_MAP(CPortAnalystView)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(RMSG_MOVECTRL, OnRmsgMoveCtrl)
	ON_REGISTERED_MESSAGE(RMSG_PF_CONTROL_EVENT, OnPlatformControlEvent)
END_MESSAGE_MAP()

// 폼화면 컨트롤로부터 받고 싶은 이벤트를 등록하세요.
// 예: ON_EVENT(CPortAnalystView, 3780,	CWrpButton::eidClick,	OnClickBtn, VTS_NONE)
BEGIN_EVENTSINK_MAP(CPortAnalystView, CScrFormView)
	ON_EVENT(CPortAnalystView, CTLID_FORM,	CWrpForm::eventidOnFormAfterInit,		OnFormAfterInit,	VTS_NONE)
	ON_EVENT(CPortAnalystView, CTLID_FORM,	CWrpForm::eventidOnFormBeforeInit,		OnFormBeforeInit,	VTS_NONE)
	ON_EVENT(CPortAnalystView, CTLID_TRANDM,	CWrpTranDM::eventidReceiveData,		OnReceiveTranData,	VTS_BSTR)
	ON_EVENT(CPortAnalystView, CTLID_TRANDM,	CWrpTranDM::eventidReceiveComplete,	OnReceiveTranCompleteData, VTS_BSTR)
	ON_EVENT(CPortAnalystView, CTLID_REALDM,	CWrpRealDM::eventidReceive,			OnReceiveRealData,	VTS_BSTR)
	ON_EVENT(CPortAnalystView, CTLID_REALDM,	CWrpRealDM::eventidReceiveComplete,	OnReceiveRealCompleteData, VTS_BSTR)
	ON_EVENT(CPortAnalystView, 3775, CWrpButton::eventidOnClick, OnClickbtnTotal, VTS_NONE)
	ON_EVENT(CPortAnalystView, 3780, CWrpButton::eventidOnClick, OnClickbtnKospi, VTS_NONE)
	ON_EVENT(CPortAnalystView, 3785, CWrpButton::eventidOnClick, OnClickbtnKosdaq, VTS_NONE)
	ON_EVENT(CPortAnalystView, 3790, CWrpButton::eventidOnClick, OnClickbtnSearch, VTS_NONE)
	ON_EVENT(CPortAnalystView, 3795, CWrpButton::eventidOnClick, OnClickbtnSend, VTS_NONE)
	ON_EVENT(CPortAnalystView, 3800, CWrpCombo::eventidOnListSelChanged, OnSelChangebtnComboTime, VTS_I2)
	ON_EVENT(CPortAnalystView, 3805, CWrpCalendar::eventidOnSelchanged , OnSelChangebtnCalendar, VTS_BSTR)	
	ON_EVENT(CPortAnalystView, CTLID_FORM,	CWrpForm::eventidOnChangeColorTbl,		OnChangeColorTbl,		VTS_I2)	
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortAnalystView diagnostics

#ifdef _DEBUG
void CPortAnalystView::AssertValid() const
{
	CScrFormView::AssertValid();
}

void CPortAnalystView::Dump(CDumpContext& dc) const
{
	CScrFormView::Dump(dc);
}
#endif //_DEBUG

void CPortAnalystView::OnCreate_MainWnd()
{
	int nWidth, nHeight;
	nWidth = 800;
	nHeight = 485;
	
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	g_iMetaTable.LoadMetaTable();
	m_LinkDataManager.Initialize();
	g_iMetaTable.SetHTSID(atoi(m_LinkDataManager.GetMediaType()));

	m_pwndMainFrm = new CAnalystMainFrmWnd;
	BOOL bResult = m_pwndMainFrm->Create(NULL, _T("Main Frame Window"), WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CRect(0, 0, nWidth, nHeight), this, NULL);
	m_pwndMainFrm->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	
	AfxSetResourceHandle(hInstSave);
	
	if (bResult)
	{
		m_pwndMainFrm->ShowWindow(SW_SHOW);
		SetWindowPos(NULL, 0, 0, nWidth, nHeight, SWP_NOZORDER);
	}
}

void CPortAnalystView::SetInit_MainWnd()
{
	OnCreate_MainWnd();

	if (m_pwndMainFrm != NULL)
	{
		CString strOpenMode;
		CWrpForm* pForm = GetFormCtl();
		if(pForm)
			strOpenMode = pForm->GetOpenMapData();
		
		if(strOpenMode == "")
			strOpenMode = "0";

		m_pwndMainFrm->SetInit(strOpenMode);
		OnChangeColorTbl(0);
	}
}
/////////////////////////////////////////////////////////////////////////////
// CPortAnalystView message handlers

// 컨트롤에 대한 Wrap클래스 변수를 연결한다.
void CPortAnalystView::OnInitialControl() 
{
	AttachCtrl(&m_ctrlTotalBtn, "btnTotal");
	AttachCtrl(&m_ctrlKospiBtn, "btnKospi");
	AttachCtrl(&m_ctrlKosdaqBtn, "btnKosdaq");
	AttachCtrl(&m_ctrlSearchBtn, "btnSearch");
	AttachCtrl(&m_ctrlCalendar, "btnCalendar");
	AttachCtrl(&m_ctrlComboTime, "btnComboTime");
	AttachCtrl(&m_ctrlSendBtn,    "btnSend");
	

	m_MapControlName["btnTotal"]  = (CObject*)&m_ctrlTotalBtn;
	m_MapControlName["btnKospi"]  = (CObject*)&m_ctrlKospiBtn;
	m_MapControlName["btnKosdaq"] = (CObject*)&m_ctrlKosdaqBtn;
	m_MapControlName["btnSearch"] = (CObject*)&m_ctrlSearchBtn;
	m_MapControlName["btnCalendar"] = (CObject*)&m_ctrlCalendar;
	m_MapControlName["btnComboTime"] = (CObject*)&m_ctrlComboTime;
	m_MapControlName["btnSend"] =	(CObject*)&m_ctrlSendBtn;
}

// 화면에 대한 초기화가 일어나기 전에 들어온다.
// DLL의 전역/클래스 변수 초기값 및 폼화면의 폼탭의 
// 디자인시가 아닌 조건에 따른 초기 로드 페이지 지정을 한다.
void CPortAnalystView::OnFormBeforeInit() 
{
	CWrpForm* pForm = GetFormCtl();
	if(pForm)
		pForm->LockFontResize(TRUE);
}

// 컨트롤에 초기속성값 지정 끝난후, 연결정보 지정되기 전, 배치화면 저장정보 로드되기 전에 이곳에 들어 온다.
// 화면과 컨트롤에 대한 각종 초기화 및 초기값 지정을 한다.
void CPortAnalystView::OnInitialUpdate() 
{
	CScrFormView::OnInitialUpdate();

	m_crBackColor = RGB(255, 255, 255);

	SetInit_MainWnd();
}

// 화면에 대한 모든 초기화 및 연결정보수신, 배치하면 저장정보 로드, 초기 이벤트 TR 호출이 끝난후 들어 온다. 
void CPortAnalystView::OnFormAfterInit() 
{
}

// 조회 데이터에 대해 응답이 내려 왔을때 호출된다.
// 데이터에 연결된 컨트롤에 업데이트 하기 전에 들어온다.
void CPortAnalystView::OnReceiveTranData(LPCTSTR szTranID)
{
}

// 조회 데이터에 대해 응답이 내려 왔을때 호출된다.
// 데이터에 연결된 컨트롤에 업데이트 한 후에 들어온다.
void CPortAnalystView::OnReceiveTranCompleteData(LPCTSTR szTranID)
{
}

// 등록한 실시간 데이터가 내려왔을때 호출된다.
// 데이터에 연결된 컨트롤에 업데이트 하기 전에 들어온다.
void CPortAnalystView::OnReceiveRealData(LPCTSTR szTranID)
{
}

// 등록한 실시간 데이터가 내려왔을때 호출된다.
// 데이터에 연결된 컨트롤에 업데이트 한 후에 들어온다.
void CPortAnalystView::OnReceiveRealCompleteData(LPCTSTR szTranID)
{
}

void CPortAnalystView::OnSize(UINT nType, int cx, int cy) 
{
	CScrFormView::OnSize(nType, cx, cy);
	
	if (m_pwndMainFrm != NULL)
	{
		CRect reClient;
		GetClientRect(&reClient);
		m_pwndMainFrm->MoveWindow(&reClient);
	}
}


void CPortAnalystView::DoControlMove(LPCSTR szCtrlName, CWnd* pWnd)
{
	if(!pWnd || !pWnd->GetSafeHwnd()) return;
	
	CRect ctlrect;
	pWnd->GetWindowRect(&ctlrect);	
	ScreenToClient(ctlrect);
	pWnd->ShowWindow(SW_HIDE);
	
	CWrpButton* pBtnObject;
	if( m_MapControlName.Lookup(szCtrlName, (CObject*&)pBtnObject))
	{
		pBtnObject->SetLeft(ctlrect.left);
		pBtnObject->SetTop(ctlrect.top);
		pBtnObject->SetWidth(ctlrect.Width());
		pBtnObject->SetHeight(ctlrect.Height());
	}
}

LRESULT CPortAnalystView::OnRmsgMoveCtrl(WPARAM wParam, LPARAM lParam)
{
	
	ST_MOVEITEM* pItem= (ST_MOVEITEM*)lParam;
	
	if(pItem == NULL) return 0L;
	
	switch(pItem->nType)
	{
	case PF_ITEM_BTN:
		OnPlatFormBtn(pItem, (int)wParam);
		DoPlatFormBtnMove(pItem->szCtrlName, pItem->pWnd);
		break;
	case PF_ITEM_CHECKBTN:
		OnPlatFormCheckBtn(pItem, (int)wParam);
		DoPlatFormCheckBtnMove(pItem->szCtrlName, pItem->pWnd);
		break;
	case PF_ITEM_CALENDAR:
		OnPlatFormCalendar(pItem, (int)wParam);
		DoPlatFormCalendarMove(pItem->szCtrlName, pItem->pWnd);
		break;
	case PF_ITEM_COMBOBOX:
		OnPlatFormComboBtn(pItem, (int)wParam);
		DoPlatFormComboBtnMove(pItem->szCtrlName, pItem->pWnd);
		break;
	default:
		OnPlatFormBtn(pItem, (int)wParam);
		DoPlatFormBtnMove(pItem->szCtrlName, pItem->pWnd);
		break;
	}
	
	return 1L;
}

//플랫폼 버튼 Show/Hide
void CPortAnalystView::OnPlatFormBtn(ST_MOVEITEM* pItem, int nFlag)
{
	BOOL bShow = (nFlag == PF_CONTROL_HIDE ? FALSE : TRUE);  //PF_CONTROL_SHOW = 1 , PF_CONTROL_HIDE = 2
	CWnd* pWnd = pItem->pWnd;
	
	if(!pWnd || !pWnd->GetSafeHwnd()) return ;
	pItem->bEnable = pWnd->IsWindowEnabled();
	BOOL bEnable = pItem->bEnable;
	
	CWrpButton* pBtnObject;
	if( m_MapControlName.Lookup(pItem->szCtrlName, (CObject*&)pBtnObject) )
	{
		pBtnObject->SetVisible(bShow);
		pBtnObject->SetEnabled(bEnable);
	}	
	
}

//플랫폼 체크박스버튼 Show/Hide
void CPortAnalystView::OnPlatFormCheckBtn(ST_MOVEITEM* pItem, int nFlag)
{
	BOOL bShow = (nFlag == PF_CONTROL_HIDE ? FALSE : TRUE);  //PF_CONTROL_SHOW = 1 , PF_CONTROL_HIDE = 2
	CWnd* pWnd = pItem->pWnd;
	
	if(!pWnd || !pWnd->GetSafeHwnd()) return ;
	pItem->bEnable = pWnd->IsWindowEnabled();
	BOOL bEnable = pItem->bEnable;
	
	CWrpCheck* pBtnObject;
	if( m_MapControlName.Lookup(pItem->szCtrlName, (CObject*&)pBtnObject) )
	{
		pBtnObject->SetVisible(bShow);	
		pBtnObject->SetEnabled(bEnable);
	}	
}

//플랫폼 버튼 Show/Hide
void CPortAnalystView::OnPlatFormCalendar(ST_MOVEITEM* pItem, int nFlag)
{
	BOOL bShow = (nFlag == PF_CONTROL_HIDE ? FALSE : TRUE);  //PF_CONTROL_SHOW = 1 , PF_CONTROL_HIDE = 2
	CWnd* pWnd = pItem->pWnd;
	
	if(!pWnd || !pWnd->GetSafeHwnd()) return ;
	pItem->bEnable = pWnd->IsWindowEnabled();
	BOOL bEnable = pItem->bEnable;
	
	CWrpCalendar* pBtnObject;
	if( m_MapControlName.Lookup(pItem->szCtrlName, (CObject*&)pBtnObject) )
	{
		pBtnObject->SetVisible(bShow);
		pBtnObject->SetEnabled(bEnable);
	}	
}

//플랫폼 버튼 Show/Hide
void CPortAnalystView::OnPlatFormComboBtn(ST_MOVEITEM* pItem, int nFlag)
{
	BOOL bShow = (nFlag == PF_CONTROL_HIDE ? FALSE : TRUE);  //PF_CONTROL_SHOW = 1 , PF_CONTROL_HIDE = 2
	CWnd* pWnd = pItem->pWnd;
	
	if(!pWnd || !pWnd->GetSafeHwnd()) return ;
	pItem->bEnable = pWnd->IsWindowEnabled();
	BOOL bEnable = pItem->bEnable;
	
	CWrpCombo* pBtnObject;
	if( m_MapControlName.Lookup(pItem->szCtrlName, (CObject*&)pBtnObject) )
	{
		pBtnObject->SetVisible(bShow);
		pBtnObject->SetEnabled(bEnable);
	}	
}


void CPortAnalystView::DoPlatFormBtnMove(LPCSTR szCtrlName, CWnd* pWnd)
{	
	CRect ctlrect;
	pWnd->GetWindowRect(&ctlrect);	
	ScreenToClient(ctlrect);
	pWnd->ShowWindow(SW_HIDE);
	
	CWrpButton* pBtnObject;
	if( m_MapControlName.Lookup(szCtrlName, (CObject*&)pBtnObject))
	{
		pBtnObject->SetLeft(ctlrect.left);
		pBtnObject->SetTop(ctlrect.top);
		pBtnObject->SetWidth(ctlrect.Width());
		pBtnObject->SetHeight(ctlrect.Height());
	}
}

//플랫폼 체크박스버튼컨트롤 이동 
void CPortAnalystView::DoPlatFormCheckBtnMove(LPCSTR szCtrlName, CWnd* pWnd)
{	
	CRect ctlrect;
	pWnd->GetWindowRect(&ctlrect);	
	ScreenToClient(ctlrect);
	pWnd->ShowWindow(SW_HIDE);
	
	CWrpCheck* pBtnObject;
	if( m_MapControlName.Lookup(szCtrlName, (CObject*&)pBtnObject))
	{
		pBtnObject->SetLeft(ctlrect.left);
		pBtnObject->SetTop(ctlrect.top);
		pBtnObject->SetWidth(ctlrect.Width());
		pBtnObject->SetHeight(ctlrect.Height());
	}
}

//플랫폼 체크박스버튼컨트롤 이동 
void CPortAnalystView::DoPlatFormCalendarMove(LPCSTR szCtrlName, CWnd* pWnd)
{	
	CRect ctlrect;
	pWnd->GetWindowRect(&ctlrect);	
	ScreenToClient(ctlrect);
	pWnd->ShowWindow(SW_HIDE);
	
	CWrpCalendar* pBtnObject;
	if( m_MapControlName.Lookup(szCtrlName, (CObject*&)pBtnObject))
	{
		pBtnObject->SetLeft(ctlrect.left);
		pBtnObject->SetTop(ctlrect.top);
		pBtnObject->SetWidth(ctlrect.Width());
		pBtnObject->SetHeight(ctlrect.Height());
	}
}

//플랫폼 체크박스버튼컨트롤 이동 
void CPortAnalystView::DoPlatFormComboBtnMove(LPCSTR szCtrlName, CWnd* pWnd)
{	
	CRect ctlrect;
	pWnd->GetWindowRect(&ctlrect);	
	ScreenToClient(ctlrect);
	pWnd->ShowWindow(SW_HIDE);
	
	CWrpCombo* pBtnObject;
	if( m_MapControlName.Lookup(szCtrlName, (CObject*&)pBtnObject))
	{
		pBtnObject->SetLeft(ctlrect.left);
		pBtnObject->SetTop(ctlrect.top);
		pBtnObject->SetWidth(ctlrect.Width());
		pBtnObject->SetHeight(ctlrect.Height());
	}
}

//DLL -> 플랫폼 컨트롤 이벤트
LRESULT CPortAnalystView::OnPlatformControlEvent(WPARAM wParam, LPARAM lParam)
{
	ST_PF_CONTROL_EVENT* pfEvent= (ST_PF_CONTROL_EVENT*)lParam;
	
	if(pfEvent == NULL)
		return 0L;	
	
	switch(pfEvent->nControlType)
	{
	case PF_ITEM_BTN:
		PlatForm_BtnEvent(pfEvent);
		break;
	case PF_ITEM_RADIOBTN:
		break;
	case PF_ITEM_CHECKBTN:
		PlatForm_CheckBtnEvent(pfEvent);
		break;
	case PF_ITEM_COMBOBOX:
		PlatForm_ComboBtnEvent(pfEvent);
		break;		
	case PF_ITEM_CALENDAR:
		PlatForm_CalendarEvent(pfEvent);
		break;
	default:
		break;
	}
	return 1L;	
}

//DLL -> 플랫폼 버튼 이벤트 
void CPortAnalystView::PlatForm_BtnEvent(ST_PF_CONTROL_EVENT* pfEvent)
{
	CWrpButton* pBtnObject;
	BOOL bState = pfEvent->bState;
	CString strText;
	if( m_MapControlName.Lookup(pfEvent->szCtrlName, (CObject*&)pBtnObject))
	{
		switch(pfEvent->nEventID)
		{
		case PF_EVENT_BTN_SETWINDOWTEXT:
			{
				strText = pfEvent->szData;	
				pBtnObject->SetCaption((LPCTSTR)strText);
				break;
			}	
		case PF_EVENT_BTN_SETTEXTCOLOR:
			{
				COLORREF clr = (COLORREF)pfEvent->nData;
				strText = pfEvent->szData;	
				pBtnObject->SetForeColor(clr);
				break;
			}
		default:
			break;	
		}			
	}	
}

//DLL -> 플랫폼 체크박스버튼 이벤트 
void CPortAnalystView::PlatForm_CheckBtnEvent(ST_PF_CONTROL_EVENT* pfEvent)
{
	CWrpCheck* pBtnObject;
	BOOL bState = pfEvent->bState;
	if( m_MapControlName.Lookup(pfEvent->szCtrlName, (CObject*&)pBtnObject))
	{
		pBtnObject->SetCheck(bState);	
	}	
}

//DLL -> 플랫폼 콤보박스버튼 이벤트 
void CPortAnalystView::PlatForm_ComboBtnEvent(ST_PF_CONTROL_EVENT* pfEvent)
{
	CWrpCombo* pBtnObject;
	BOOL bState = pfEvent->bState;
	CString strComboText = "";
	if( m_MapControlName.Lookup(pfEvent->szCtrlName, (CObject*&)pBtnObject))
	{
		if(pfEvent->nEventID == PF_EVENT_COMBO_ADDLIST)
		{
			for(int nIndex = 0; nIndex < pfEvent->arrData.GetSize(); nIndex++)
				pBtnObject->AddRow(pfEvent->arrData.GetAt(nIndex));		
			//		pBtnObject->SetCaption("");
		}
		else if(pfEvent->nEventID == PF_EVENT_COMBO_ADDSTRING)
		{		
			pBtnObject->AddRow(pfEvent->szData);
		}
		else if(pfEvent->nEventID == PF_EVENT_COMBO_GETWINDOWTEXT)
		{		
			strComboText = pBtnObject->GetCaption();
			pfEvent->stReturn.strWindowText = strComboText;
			pBtnObject->InsertRow(strComboText, 0);
		}
		else if(pfEvent->nEventID == PF_EVENT_COMBO_SETCURSEL)
		{
			pBtnObject->SetCurSel(pfEvent->nData);
		}
		else if(pfEvent->nEventID == PF_EVENT_COMBO_GETCURSEL)
		{
			pfEvent->stReturn.nCurSel = pBtnObject->GetCurSel();			
		}
		else if(pfEvent->nEventID == PF_EVENT_COMBO_RESETCONTENTS)
		{
			pBtnObject->ResetContent();
			pBtnObject->SetCurSel(0);
		}	
	}	
}

//DLL -> 플랫폼 달력버튼 이벤트 
void CPortAnalystView::PlatForm_CalendarEvent(ST_PF_CONTROL_EVENT* pfEvent)
{
	CWrpCalendar* pBtnObject;
	BOOL bState = pfEvent->bState;
	CString strText = "";
	CString strYear = "";CString strMon = "";CString strDay = "";
	if( m_MapControlName.Lookup(pfEvent->szCtrlName, (CObject*&)pBtnObject))
	{
		strText = pfEvent->szData;
		strYear = strText.Mid(0, 4);
		strMon  = strText.Mid(4, 2);
		strDay  = strText.Mid(6, 2);
		if(pfEvent->nEventID == PF_EVENT_CALENDAR_SETTIME)
		{
			pBtnObject->SetDate(strYear, strMon, strDay);
		}		
	}	
}

void CPortAnalystView::OnClickbtnTotal()
{
	if(m_pwndMainFrm) m_pwndMainFrm->PlatFormBtnTotal();
}

void CPortAnalystView::OnClickbtnKospi()
{
	if(m_pwndMainFrm) m_pwndMainFrm->PlatFormBtnKospi();
}

void CPortAnalystView::OnClickbtnKosdaq()
{	
	if(m_pwndMainFrm) m_pwndMainFrm->PlatFormBtnKosdaq();
}

void CPortAnalystView::OnClickbtnSearch()
{
	if(m_pwndMainFrm) m_pwndMainFrm->PlatFormBtnSearch();
}

void CPortAnalystView::OnClickbtnSend()
{
	if(m_pwndMainFrm) m_pwndMainFrm->PlatFormBtnSend();
}

void CPortAnalystView::OnSelChangebtnComboTime(int nIndex)
{
	if(m_pwndMainFrm) m_pwndMainFrm->PlatFormBtnComboTime(nIndex);
}

void CPortAnalystView::OnSelChangebtnCalendar(LPCTSTR szData)
{
	if(m_pwndMainFrm) m_pwndMainFrm->PlatFormBtnCalendar(szData);
}

void CPortAnalystView::OnChangeColorTbl(int nTableIndex)
{
	COLORREF clrBkgnd =			CCtlColor::GetColor(2); 
	
	ST_TABCOLOR stTabColor;
	stTabColor.clrTabSelText	= CCtlColor::GetColor(12);   //탭 활성 텍스트
	stTabColor.clrTabUnSelText	= CCtlColor::GetColor(14);   //탭 비활성 텍스트
	stTabColor.clrTabBody		= CCtlColor::GetColor(10);   //텝 바디 배경
	stTabColor.clrTabSelGraUp		= CCtlColor::GetColor(10);	//탭 활성 헤더 그라데이션 시작
	stTabColor.clrTabSelGraDown		= CCtlColor::GetColor(10);	//탭 활성 헤더 그라데이션 끝
	stTabColor.clrTabUnSelGraUp		= CCtlColor::GetColor(13);	//탭 비활성 헤더 그라데이션 시작
	stTabColor.clrTabUnSelGraDown	= CCtlColor::GetColor(13);	//탭 비활성 헤더 그라데이션 끝 
	
	m_pwndMainFrm->ChangeSkinColor(clrBkgnd, stTabColor);
	ST_GRIDCOLOR stGridColor;
	stGridColor.clrGridHeader	= CCtlColor::GetColor(21);   //그리드 헤더배경
	stGridColor.clrGridHText	= CCtlColor::GetColor(143);   //그리드 헤더 텍스트
	stGridColor.clrGridHilight	= CCtlColor::GetColor(29);   //그리드 하이라이트
	stGridColor.clrGridSBk		= CCtlColor::GetColor(22);   //그리드 셀배경
	stGridColor.clrGridSText	= CCtlColor::GetColor(142);   //그리드 셀 텍스트
	
	m_pwndMainFrm->ChangeGridColor(stGridColor);
}

void CPortAnalystView::SetOpenScreen(CString strScreen, CString strCode)
{
	CWrpForm* pForm = GetFormCtl();
	
	if(pForm && strScreen != "")
	{
	//	PutLinkData("&STOCK_CODE", strCode);
		pForm->OpenScreenWithScreenNum(strScreen);
	}
}

void CPortAnalystView::OpenScreenData(CString strScreen, CString strCode)
{
	CWrpForm* pForm = GetFormCtl();
	if(pForm)
		pForm->OpenScreenData((LPCTSTR)strScreen, (LPCTSTR)strCode, (short)strCode.GetLength());
}

void CPortAnalystView::OnDBClick(LPCSTR strCode)
{
	CWrpForm* pForm = GetFormCtl();
	if(pForm) pForm->SendMsgToMainDblLinkScr("ST_DBL_LNK", strCode);
}

