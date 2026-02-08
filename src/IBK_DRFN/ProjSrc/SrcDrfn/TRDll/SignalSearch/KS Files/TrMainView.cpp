// TrMainView.cpp : implementation file
//

#include "stdafx.h"
#include "TrMainView.h"
#include "../../Contents/inc/INewsSignalMng.h"
#include "../../contents/inc/IKSLibMng.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrMainView

#define ID_FORM			999

#define TYPE_SEARCH_ALL     0
#define TYPE_SEARCH_NEWS    1
#define TYPE_SEARCH_SYS     2

//#define	ID_CLEARCAPTION		1

// 메인에서 신호 클릭시 신호를 보내기 위해서..
const UINT RMSG_OUTSIDE_SIGNALSEARCH = ::RegisterWindowMessage("RMSG_OUTSIDE_SIGNALSEARCH");
//#define	REGIST_FOR_MAIN_NOTIFY {	char aBufWnd[8+1]={0,};	\
//		sprintf(aBufWnd, "%08X", GetSafeHwnd());				\
//		::SetEnvironmentVariable("HANARO_SIGNALSEARCH", aBufWnd);	}


enum
{
	ID_btnSignalSet = IDC_BTN_SIGNALSET,
	ID_btnSearch = IDC_BTN_SEARCH
};

int nCtrlID[] = 
{
	ID_btnSignalSet,
	ID_btnSearch
};

CString sCtrlName[] =
{
	"btnSignalSet",
	"btnSearch"
};

// 아래 루틴은 메인에서 보내는 루틴
//	const UINT RMSG_OUTSIDE_SIGNALSEARCH = ::RegisterWindowMessage("RMSG_OUTSIDE_SIGNALSEARCH");
//	HWND hMainHwnd=NULL;
//	char aBufWnd[8+1]={0,};
//	::GetEnvironmentVariable("HANARO_SIGNALSEARCH", aBufWnd, 9);
//	sscanf(aBufWnd,"%X", &hMainHwnd);
//	if(hMainHwnd && IsWindow(hMainHwnd)) ::PostMessage(hMainHwnd, RMSG_OUTSIDE_SIGNALSEARCH, 100, 0);

IMPLEMENT_DYNCREATE(CTrMainView, CFormView)

CTrMainView::CTrMainView()
: CFormView(CTrMainView::IDD)
{
	//{{AFX_DATA_INIT(CTrMainView)
	m_nInitFlag     = 0;
	//}}AFX_DATA_INIT
	
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);
	
	m_brushStatic.CreateSolidBrush(COLOR_MAIN_CANVAS);
	
	m_pctrlCodeInput = NULL;
	m_pctrlChart = NULL;
	
	m_bExp_Left = TRUE;
	
	m_strCode = "";
	m_bIsFnUser = FALSE;

	m_dwAdviseKey = NULL;
	m_nScreenType = 0;
	m_nGridRow = 2;
	
	m_hKSJongName = NULL;
	m_nState = 0; // Default=전체
}

CTrMainView::~CTrMainView()
{
	SetDelAll_SignalData();
	
	m_fontDefault.DeleteObject();
	m_fontBold.DeleteObject();
	
	m_brushStatic.DeleteObject();
}

void CTrMainView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrMainView)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrMainView, CFormView)
	//{{AFX_MSG_MAP(CTrMainView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(UM_CHANGE_PARENT, OnChangeChildFrameWindowHandle)
	ON_BN_CLICKED(IDC_BTN_INITCODE, OnBtnInit_Code)
	ON_BN_CLICKED(IDC_BTN_EXP_LEFT, OnBtnExp_Left)
//@유진삭제		ON_BN_CLICKED(IDC_BTN_SELL, OnBtnSell)

	ON_CBN_SELCHANGE(IDC_COMBO_SIGNALTYPE, OnChangeSignalType)

	ON_BN_CLICKED(IDC_BTN_SIGNALSET, OnBtnSignalSet)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
	ON_COMMAND_RANGE(IDC_RADIO_TOTAL, IDC_RADIO_CODE, OnRadioBtnClk)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHK_MARKETCONDITION, IDC_CHK_SYS, OnChkBtnClk)

	ON_MESSAGE(WM_GET_BROD, OnGetBrod)
	ON_REGISTERED_MESSAGE(RMSG_OUTSIDE_SIGNALSEARCH		,OnRmsgOutside)
END_MESSAGE_MAP()
//@유진삭제	ON_BN_CLICKED(IDC_BTN_SELL, OnBtnSell)
//	ON_WM_TIMER()

//  ON_BN_CLICKED(IDC_BTN_BUY, OnBtnBuy)

BEGIN_EVENTSINK_MAP(CTrMainView, CFormView)
	//{{AFX_EVENTSINK_MAP(CTrMainView)
	ON_EVENT(CTrMainView, IDC_CODEINPUT,		1 ,	OnCodeEnter,			VTS_NONE)
	ON_EVENT(CTrMainView, IDC_CODEINPUT,		2 ,	OnCodeEnter,			VTS_NONE)
	ON_EVENT(CTrMainView, IDC_CODEINPUT,		4 ,	OnCodeEnter,			VTS_NONE)

	ON_EVENT(CTrMainView, IDC_CODEINPUT + 1,		1 ,	OnCodeEnter_Chart,			VTS_NONE)
	ON_EVENT(CTrMainView, IDC_CODEINPUT + 1,		2 ,	OnCodeEnter_Chart,			VTS_NONE)
	ON_EVENT(CTrMainView, IDC_CODEINPUT + 1,		4 ,	OnCodeEnter_Chart,			VTS_NONE)
	
	ON_EVENT(CTrMainView, IDC_CHART,		1,		OnReceiveDataAfter,	VTS_BSTR  VTS_PVARIANT  VTS_PVARIANT  VTS_PVARIANT  VTS_PVARIANT  VTS_I4)

//	ON_EVENT(CTrMainView, IDD_STD_DIALOG,	2 /* OnInitAfter */, OnFormInitAfter, VTS_NONE)
	ON_EVENT(CTrMainView,	ID_FORM,	2 /* OnInitAfter */, OnFormInitAfter, VTS_NONE)
	ON_EVENT(CTrMainView,	ID_FORM,	0xb /* OnGetPageSave */,	OnFormGetPageSave,	VTS_BSTR)
	ON_EVENT(CTrMainView,	ID_FORM,	0xc /* OnSetPageSave */,	OnFormSetPageSave,	VTS_BSTR)
	ON_EVENT(CTrMainView,	ID_FORM,	0x1f, OnColorChange , VTS_NONE)
	ON_EVENT_RANGE(CTrMainView, ID_btnSignalSet, ID_btnSearch, 6, OnEuBtnClk, VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrMainView diagnostics

#ifdef _DEBUG
void CTrMainView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTrMainView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

void CTrMainView::OnCodeEnter()
{
//	if(m_nState==0)
//	{
//		m_ctrlJMCombo.SetCaption("");
//		m_ctrlCodeName.SetWindowText("");
//		//CWnd::FromHandle(m_hKSJongName)->SetWindowText("");
//		SetTimer(ID_CLEARCAPTION, 10, NULL);
//		return;
//	}

	if (m_ctrlJMCombo.CheckCaption() == TRUE)
	{
		CString strData;
		strData = m_ctrlJMCombo.GetCurName();
		
		strData.Replace("&","&&");
		m_ctrlCodeName.SetWindowText(strData);
		
		if(m_nScreenType == 1)//현재가 화면 종목코드 변경시 Clear
		{
			m_gridSignal.ClearAllData(1, NULL);
			m_nGridRow = 2;
		}

		if(m_nState==1)		SetSearch();
	}
	else
		m_ctrlCodeName.SetWindowText("");
}

void CTrMainView::OnCodeEnter_Chart()
{
	if (m_ctrlJMCombo_Chart.CheckCaption() == TRUE)
	{
		CString strData;
		strData = m_ctrlJMCombo_Chart.GetCurName();
		strData.Replace("&","&&");
		m_ctrlCodeName_Chart.SetWindowText(strData);
	}
	else
		m_ctrlCodeName_Chart.SetWindowText("");
}


void CTrMainView::OnReceiveDataAfter(LPCTSTR strTrCode, VARIANT FAR* strVarOutInfo, VARIANT FAR* nVarOutInfo, VARIANT FAR* nVarRetValue, VARIANT FAR* nVarRetOption, long nDataCount)
{
	SetSearchData_TimeSet();
}

/////////////////////////////////////////////////////////////////////////////
// CTrMainView message handlers
int CTrMainView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;


//	m_crBackColor = RGB(255, 255, 255);
	m_crBackColor = COLOR_MAIN_CANVAS;
	
	m_ctrlCodeName.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_STATIC_NAME);
	m_ctrlCodeName.SetFont(&m_fontBold);
	
	m_btnInit_Code.Create(_T("Code Init"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | SS_CENTER, 
		CRect(0, 0, 0, 0), this, IDC_BTN_INITCODE);
	m_btnInit_Code.SetLoadImage(IDB_JERASER);
	m_btnInit_Code.SetFont(&m_fontDefault);
	m_btnInit_Code.ShowWindow(SW_HIDE);	//유진 (컨트롤 HIDE)
	
	
	m_comboSignalType.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(0, 0, 0, 0), this, IDC_COMBO_SIGNALTYPE);
	m_comboSignalType.SetFont(&m_fontDefault);
	m_comboSignalType.ShowWindow(SW_HIDE);  //유진 (컨트롤 HIDE)
	
	m_btnExp_Left.Create(_T(">"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_EXP_LEFT);
	m_btnExp_Left.SetFont(&m_fontDefault);
	m_btnExp_Left.SetLoadImage(IDB_EXP_RIGHT);
	
//@유진삭제		m_btnSell.Create(_T("주문"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 
//@유진삭제			CRect(0, 0, 0, 0), this, IDC_BTN_SELL);		
//@유진삭제		m_btnSell.LoadBitmaps(IDB_SELL_UP, IDB_SELL_DOWN);
//@유진삭제		m_btnSell.SetFont(&m_fontDefault);
	
	m_btnSignalSet.Create(_T("신호설정"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_SIGNALSET);
	m_btnSignalSet.SetFont(&m_fontDefault);
	m_btnSignalSet.SetBtnColor(2);
	
	m_btnSearch.Create(_T("검색"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_SEARCH);
	m_btnSearch.SetFont(&m_fontDefault);
	m_btnSearch.SetBtnColor(2);
	
	m_gridResult.CreateGrid(WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 0, 0), this, IDC_GRID_RESULT);
	
	m_ctrlCodeName_Chart.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
	m_ctrlCodeName_Chart.SetFont(&m_fontBold);

//유진 UI변경
	m_radioTotal.Create(_T("전체"), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON |WS_GROUP , 
		CRect(0, 0, 0, 0), this, IDC_RADIO_TOTAL);
	m_radioTotal.SetFont(&m_fontDefault);
	m_radioTotal.SetCheck(TRUE);

	m_radioCode.Create(_T(""), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_RADIO_CODE);
	m_radioCode.SetFont(&m_fontDefault);

	m_chkMarketCondition.Create(_T("시세"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX , 
		CRect(0, 0, 0, 0), this, IDC_CHK_MARKETCONDITION);
	m_chkMarketCondition.SetFont(&m_fontDefault);

	m_chkTechIndex.Create(_T("기술적 지표"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_CHK_TECHINDEX);
	m_chkTechIndex.SetFont(&m_fontDefault);

	m_chkDealAgency.Create(_T("거래원"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_CHK_DEALAGENCY);
	m_chkDealAgency.SetFont(&m_fontDefault);

	m_chkNews.Create(_T("뉴스"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_CHK_NEWS);	
	m_chkNews.SetFont(&m_fontDefault);
	m_chkNews.SetCheck(TRUE);

	m_chkSys.Create(_T("시스템"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_CHK_SYS);	
	m_chkSys.SetFont(&m_fontDefault);
	m_chkSys.SetCheck(TRUE);

//	REGIST_FOR_MAIN_NOTIFY;
//	{
//		char aBufWnd[8+1]={0,};
//		sprintf(aBufWnd, "%08X", GetSafeHwnd());
//		::SetEnvironmentVariable("HANARO_SIGNALSEARCH", aBufWnd);
//	}
	return 0;
}

void CTrMainView::OnDestroy() 
{
	//	CFormView::OnDestroy();
	SetAdvData_Alert(FALSE);
	
	CFormView::OnDestroy();
}

void CTrMainView::OnSize(UINT nType, int cx, int cy) 
{	

	CFormView::OnSize(nType, cx, cy);	

//	if(m_nScreenType==1) return;
	if (m_nInitFlag > 0)		OnSize();
}

void CTrMainView::OnSize()
{	
	CRect reClient, reTemp;
	GetClientRect(reClient);
	
	reClient.top += 4;
	reClient.bottom -= 4;
	reClient.left += 4;
	reClient.right -= 4;
	
	int nWidth;
	nWidth = (reClient.right - reClient.left) / 2;
	reTemp = reClient;
	if (m_bExp_Left == TRUE)
	{
		reTemp.right = reClient.right;
		reTemp.left = reTemp.right - nWidth;
		OnSize_Right(reTemp);
		
		reTemp.right = reTemp.left - 4;
		reTemp.left = reClient.left;
	}
	
	OnSize_Left(reTemp);
}

void CTrMainView::OnSize_Left(CRect reClient)
{
	CRect reTop, reTemp;
	
	reTop = reClient;
	reTop.bottom = reTop.top + 20;
	
	reTemp = reTop;
	reTemp.right -= 4;
	
	reTemp.left = reTemp.right - 20;
	m_btnExp_Left.MoveWindow(&reTemp);
	
	reTemp.right = reTemp.left - 1; 
	
//유진	reTemp.left = reTemp.right - 62;
	reTemp.left = reTemp.right - GetBtnWidth(2) + 6;
//	reTemp.bottom = reTemp.top + 100;
//유진	m_comboSignalType.MoveWindow(&reTemp);	
	m_btnSearch.MoveWindow(&reTemp);
	
//	reTemp.left = reTemp.right + GetBtnWidth(2) + 6;
//	reTemp.left = reTemp.right - 36;


	
//	reTemp.right = reTemp.left - 2;
//	reTemp.left = reTemp.right - 20;
	reTemp.right = reTemp.left - 1;
	reTemp.left = reTemp.right - GetBtnWidth(4) + 10;
	reTemp.bottom = reTemp.top + 20;
//유진	m_btnInit_Code.MoveWindow(&reTemp);
	m_btnSignalSet.MoveWindow(&reTemp);

		
	reTemp.left = reTemp.right + GetBtnWidth(4) + 10;
	reTemp.left = reTemp.right - 20;

//유진 추가< 
	reTemp = reTop;
	reTemp.left = reTemp.left;
	reTemp.right = reTemp.left + 50;
	m_radioTotal.MoveWindow(&reTemp);

	reTemp.left = reTemp.right;
	reTemp.right = reTemp.left + 13;
	m_radioCode.MoveWindow(&reTemp);

//유진 추가>	
//	reTemp = reTop;
	reTemp.left = reTemp.right + 5;
	reTemp.right = reTemp.left + 80;
	if (m_pctrlCodeInput != NULL)
		m_pctrlCodeInput->MoveWindow(&reTemp);
	
	reTemp.left = reTemp.right + 1;
	//reTemp.right = reTemp.left + 100;
	reTemp.right = reTemp.left + 90;
	//reTemp.top += 4;
	//reTemp.bottom = reTemp.top + 14;
	reTemp.bottom = reTemp.top + 20;
	m_ctrlCodeName.MoveWindow(&reTemp);
	if(m_hKSJongName && IsWindow(m_hKSJongName))
	{
		CWnd::FromHandle(m_hKSJongName)->MoveWindow(reTemp.left, reTemp.top, reTemp.Width(), reTemp.Height());
		m_rectJongName = reTemp;
	}
/*	
	reTemp = reTop;
	reTemp.top = reTemp.top + 22;
	reTemp.bottom = reTemp.top + 20;
	reTemp.right = reTemp.left + 50;
	m_chkMarketCondition.MoveWindow(&reTemp);

	reTemp.left = reTemp.right + 5;
	reTemp.right = reTemp.left + 90;
	m_chkTechIndex.MoveWindow(&reTemp);

	reTemp.left = reTemp.right + 5;
	reTemp.right = reTemp.left + 60;
	m_chkDealAgency.MoveWindow(&reTemp);

	reTemp.left = reTemp.right + 5;
	reTemp.right = reTemp.left + 50;
	m_chkNews.MoveWindow(&reTemp);
*/
	reTemp = reTop;
	reTemp.top = reTemp.top + 22;
	reTemp.bottom = reTemp.top + 20;
	reTemp.right = reTemp.left + 60;
	m_chkSys.MoveWindow(&reTemp);

	reTemp.left = reTemp.right + 5;
	reTemp.right = reTemp.left + 60;
	m_chkNews.MoveWindow(&reTemp);


	reTop.top = reTop.bottom + 2;
	reTop.bottom = reTop.top + 20;	
	reTemp = reTop;

	reTemp.right -= 4;
	reTemp.left = reTemp.right - GetBtnWidth(2);
//유진	m_btnSearch.MoveWindow(&reTemp);   
	
	reTemp.right = reTemp.left - 4;
	reTemp.left = reTemp.right - GetBtnWidth(4);
//유진	m_btnSignalSet.MoveWindow(&reTemp);
	
//@유진삭제			reTemp.right = reTemp.left - 4;
//@유진삭제			reTemp.left = reTemp.right - GetBtnWidth(2);
//@유진삭제			m_btnSell.MoveWindow(&reTemp);
	
//@유진삭제		reTemp.right = reTemp.left - 2;
//@유진삭제		reTemp.left = reTemp.right - GetBtnWidth(2);
//@유진삭제		m_btnBuy.MoveWindow(&reTemp);	
	
	reTop.top = reTop.bottom + 2;
	reTop.bottom = reClient.bottom;
	if(m_gridResult.GetSafeHwnd())
		m_gridResult.MoveWindow(&reTop);


	DoKSControlMove(sCtrlName[0], &m_btnSignalSet );
	DoKSControlMove(sCtrlName[1], &m_btnSearch );
}

void CTrMainView::OnSize_Right(CRect reClient)
{
	if (m_pctrlChart != NULL)
	{
		CRect reTop;
		reTop = reClient;
		
		reTop.top += 4;
		reTop.bottom = reTop.top + 14;
		m_ctrlCodeName_Chart.MoveWindow(&reTop);
		
		reTop.top = reTop.bottom + 4;
		reTop.bottom = reClient.bottom;
		m_pctrlChart->MoveWindow(&reTop);
	}
}

void CTrMainView::OnInitialUpdate() 
{	
	CFormView::OnInitialUpdate();
	
	if (m_nInitFlag == 0)
	{
		SetInit_MainWnd();
		if(m_nScreenType==1)
		{
//@유진삭제	m_btnSell.ShowWindow(SW_HIDE);
			m_btnSignalSet.ShowWindow(SW_HIDE);
			m_btnSearch.ShowWindow(SW_HIDE);
			m_gridResult.ShowWindow(SW_HIDE);
			m_comboSignalType.ShowWindow(SW_HIDE);
			m_btnExp_Left.ShowWindow(SW_HIDE);

			m_ctrlCodeName.ShowWindow(SW_HIDE);
			if(m_hKSJongName) CWnd::FromHandle(m_hKSJongName)->ShowWindow(SW_HIDE);

			m_btnInit_Code.ShowWindow(SW_HIDE);
			m_ctrlCodeName_Chart.ShowWindow(SW_HIDE);

			
			m_radioTotal.ShowWindow(SW_HIDE);
			m_radioCode.ShowWindow(SW_HIDE);
			m_chkMarketCondition.ShowWindow(SW_HIDE);
			m_chkTechIndex.ShowWindow(SW_HIDE);
			m_chkDealAgency.ShowWindow(SW_HIDE);
			m_chkNews.ShowWindow(SW_HIDE);
			m_chkSys.ShowWindow(SW_HIDE);
		}
		else
		{
			//if(m_hKSJongName) CWnd::FromHandle(m_hKSJongName)->ShowWindow(SW_HIDE);
			// 종목코드 도움말을 Disable되게 한다.
			m_pctrlCodeInput->EnableWindow(FALSE);
			
			//DoDllSizeEx();
		}
		SetControlDispatch();

		m_hChildFrame = GetFrameWnd(this)->GetSafeHwnd();
	}
	
	m_nInitFlag++;
}

void CTrMainView::SetInit_MainWnd()
{
	CSCNode* pSCNode = NULL;
	IDispatch* pDispatch = NULL;
	if (m_MapControlName->Lookup("JCombo", (CObject*&)pSCNode))
	{
		pDispatch = pSCNode->GetControlWnd()->GetDispatchPtr();
		m_pctrlCodeInput = pSCNode->GetControlWnd()->m_pWnd;
		if (pDispatch)
			m_ctrlJMCombo.AttachDispatch(pDispatch, FALSE);
		
		pSCNode->GetControlWnd()->m_pWnd->SetDlgCtrlID(IDC_CODEINPUT);
		pSCNode->GetControlWnd()->SetCtrlID(IDC_CODEINPUT);
	}
	
	if (m_MapControlName->Lookup("JCombo_Chart", (CObject*&)pSCNode))
	{
		pDispatch = pSCNode->GetControlWnd()->GetDispatchPtr();
		if (pDispatch)
			m_ctrlJMCombo_Chart.AttachDispatch(pDispatch, FALSE);
		
		pSCNode->GetControlWnd()->m_pWnd->SetDlgCtrlID(IDC_CODEINPUT + 1);
		pSCNode->GetControlWnd()->SetCtrlID(IDC_CODEINPUT + 1);
	}
	
	if (m_MapControlName->Lookup("chtData", (CObject*&)pSCNode))
	{
		pDispatch = pSCNode->GetControlWnd()->GetDispatchPtr();
		m_pctrlChart = pSCNode->GetControlWnd()->m_pWnd;
		if (pDispatch)
			m_ctrlChart.AttachDispatch(pDispatch, FALSE);
		
		pSCNode->GetControlWnd()->m_pWnd->SetDlgCtrlID(IDC_CHART);
		pSCNode->GetControlWnd()->SetCtrlID(IDC_CHART);
	}

	if (m_MapControlName->Lookup("ScreenType", (CObject*&)pSCNode))
	{
		pDispatch = pSCNode->GetControlWnd()->GetDispatchPtr();
		m_pctrlChart = pSCNode->GetControlWnd()->m_pWnd;
		if (pDispatch)
		{
			m_lbScreenType.AttachDispatch(pDispatch, FALSE);
			
			CString strData;
			strData = m_lbScreenType.GetCaption();
			if (strData.IsEmpty() == FALSE)
			{
				strData.Replace("ScreenType=", "");
				
				if (strData.CompareNoCase("current")==0)
					m_nScreenType = 1;
			}
		}
	}
//20080522 김성조 <101
	if(m_nScreenType == 1)
	{
		if (m_MapControlName->Lookup("gdNews", (CObject*&)pSCNode))
		{
			pDispatch = pSCNode->GetControlWnd()->GetDispatchPtr();
			if (pDispatch)
				m_gridSignal.AttachDispatch(pDispatch, FALSE);	
			
			pSCNode->GetControlWnd()->m_pWnd->SetDlgCtrlID(IDC_GRID_RESULT + 1);
			pSCNode->GetControlWnd()->SetCtrlID(IDC_GRID_RESULT + 1);		
		}	

		if (m_MapControlName->Lookup("JMCOMBO1", (CObject*&)pSCNode))
		{
			pDispatch = pSCNode->GetControlWnd()->GetDispatchPtr();
			m_pctrlCodeInput = pSCNode->GetControlWnd()->m_pWnd;
			if (pDispatch)
				m_ctrlJMCombo.AttachDispatch(pDispatch, FALSE);
			
			pSCNode->GetControlWnd()->m_pWnd->SetDlgCtrlID(IDC_CODEINPUT);
			pSCNode->GetControlWnd()->SetCtrlID(IDC_CODEINPUT);
			}
	}
//20080522 김성조 101>	
	if (m_pFormDisp && m_nScreenType == 0)
	{
		m_lRunForm.AttachDispatch(m_pFormDisp, FALSE);

		m_hKSJongName = GetKSControlHwnd("jcodeName");
		if(m_hKSJongName)
		{
			m_ctrlCodeName.ShowWindow(SW_HIDE);

			CRect rect;
			m_ctrlCodeName.GetWindowRect(rect);
			ScreenToClient(rect);
			CWnd::FromHandle(m_hKSJongName)->SetParent(this);
			CWnd::FromHandle(m_hKSJongName)->MoveWindow(rect);
			m_rectJongName = rect;
		}
	}

	SetInit();
}

void CTrMainView::OnFormInitAfter()
{
	
}

void CTrMainView::SetInit()
{
	CString strData;
	//# 1:HTS  0:FRONT(지점)
//--> HTS구분없앰. 사용하면 추후 다시
//	strData = KSLink_GetLoginInfo(ISHTS);
//	BOOL bIsHTS = FALSE;
//	if (strData.Left(1) == "1")
//		bIsHTS = TRUE;
	
	//strData = KSLink_GetLoginInfo(SUPER_USERLEVEL);
//@유진변경	strData = AfxGetApp()->GetProfileString("STBasic", "UserL",  "");
//@유진변경	if(strData.GetLength()==0)
//@유진변경		strData = KSLink_GetLoginInfo(SUPER_USERLEVEL);
//	strData = "S";

//	if (bIsHTS == TRUE)
//	{
//		//# "S", "A", "B", "C", "D"...
//		strData = strData.Left(1);
//		if ((strData == "S") || (strData == "A") || (strData == "B"))
			m_bIsFnUser = TRUE;
//	}
//	else if (bIsHTS == FALSE)
//	{
//		strData = strData.Left(3);
//		if ((atoi(strData) == 5) || 
//			(atoi(strData) == 6) ||
//			(atoi(strData) == 7) ||
//			(atoi(strData) == 9) ||
//			(atoi(strData) == 10) ||
//			(atoi(strData) == 11) ||
//			(atoi(strData) == 12) ||
//			(atoi(strData) == 13))
//			m_bIsFnUser = TRUE;
//	}
//<--
			
	if(m_gridResult.GetSafeHwnd())
		m_gridResult.SetInit("RESULT");
	
	m_comboSignalType.ResetContent();
	m_comboSignalType.AddString("전체");
	m_comboSignalType.AddString("뉴스");
	if (m_bIsFnUser == TRUE)
		m_comboSignalType.AddString("시스템");
	m_comboSignalType.SetCurSel(0);
	
	if(m_gridResult.GetSafeHwnd())
		m_gridResult.SetHideCol(1, TRUE);
	OnCodeEnter();
	
	
	SetAdvData_Alert(TRUE);
	OnColorChange();
	
}

void CTrMainView::SetAdvData_Alert(BOOL bReg)
{
	INewsSignalManager* pNSMng = (INewsSignalManager*)AfxGetPctrInterface(UUID_INewsSignalManager);
	if(pNSMng)
	{
		if(bReg && !m_dwAdviseKey)
		{
			m_dwAdviseKey = pNSMng->AdviseNewsSignal('A', (LONG)this);
		}
		if(!m_dwAdviseKey) return; //생성실패

		CString strRealType;
		if(m_nScreenType == 0)
		{				
			strRealType.Format("%d", (int)TYPE_SIGNAL_SYS);//20080522 김성조 <101>//TYPE_SIGNAL_SYS);
			pNSMng->SetNewsSignal(bReg, this, strRealType, "*");
		}
		else if(m_nScreenType == 1)
			SetAdvData_Alert_CurrentPrice(bReg);
			
		strRealType.Format("%d", (int)TYPE_SIGNAL_NEWS);//20080522 김성조 <101>//TYPE_SIGNAL_NEWS);
		pNSMng->SetNewsSignal(bReg, this, strRealType, "*");

		if(bReg==FALSE && m_dwAdviseKey)
		{
			pNSMng->UnAdviseNewsSignal(m_dwAdviseKey);
			m_dwAdviseKey = NULL;
		}
	}
}

BOOL CTrMainView::SetAdvData_Alert_CurrentPrice(BOOL bReg)
{
	BOOL bResult = TRUE;

	INewsSignalManager* pNSMng = (INewsSignalManager*)AfxGetPctrInterface(UUID_INewsSignalManager);

	if(pNSMng)
	{
		if(bReg && !m_dwAdviseKey)
		{
			m_dwAdviseKey = pNSMng->AdviseNewsSignal('A', (LONG)this);
		}
		if(!m_dwAdviseKey) return FALSE; //생성실패
	}

	CString strData, strkey;
	for(int nIndex = 2; nIndex <= 66; nIndex++)
	{
		strData.Format("%.4d", nIndex);		
		strkey = strData;		

		CString strRealType;					
		strRealType.Format("%d", (int)TYPE_SIGNAL_ALERT);//20080522 김성조 <101>//TYPE_SIGNAL_SYS);
		pNSMng->SetNewsSignal(bReg, this, strRealType, strkey);	
	}

/*	
	BYTE pPBID[2] = { 0 };
	pPBID[0] = (BYTE)TYPE_SIGNAL_SYS;
	
	int nBrodNo = -1;
	if (bReg == TRUE)
		nBrodNo = CommSetBrod(this->GetSafeHwnd(), (BYTE*)pPBID, 1, 'U',  (BYTE *)(LPCTSTR)strkey);
	else
		nBrodNo = CommRemoveBrod(this->GetSafeHwnd(), (BYTE*)pPBID, 1, 'U',  (BYTE *)(LPCTSTR)strkey);
	bResult = (nBrodNo > 0)?TRUE:FALSE;
*/

	return bResult;
}

void CTrMainView::OnBtnInit_Code()
{
	DoBtnInit_Code(FALSE);
}

void CTrMainView::DoBtnInit_Code(BOOL bClear)
{
	CString strData = "";
	strData = m_ctrlJMCombo.GetCaption();
	if (strData.IsEmpty() == FALSE || m_nState == 0)
	{
		if(bClear)
		{
			strData = "";
			m_ctrlJMCombo.SetCaptionCtrlEx(strData, FALSE);
		}
		//m_ctrlJMCombo.SetFocus();
		
		strData.Replace("&", "&&");
		m_ctrlCodeName.SetWindowText(strData);
		
		//# All Search!
		SetSearch();
	}
	else
		m_ctrlCodeName.SetWindowText("");
	
	
		/*
		SignalData stSignalData;
		stSignalData.nRealType = (int)TYPE_REAL_NEWS;
		stSignalData.strType = "NEWS";
		
		  stSignalData.nMarket = 0;
		  stSignalData.strCode = "000660";
		  stSignalData.strName = "하이닉스";
		  
			stSignalData.strDate = "11112233";
			stSignalData.strTime = "121212";
			
			  stSignalData.strTitle = "aweqew";
			  stSignalData.nPoint = 1;
			  stSignalData.clrTitle = RGB(52, 70, 133);
			  
				m_gridResult.SetAddRow(TRUE, &stSignalData);
	*/
}

void CTrMainView::OnBtnExp_Left()
{
	m_bExp_Left = !m_bExp_Left;
	
	if (m_bExp_Left == TRUE)
		m_btnExp_Left.SetLoadImage(IDB_EXP_RIGHT);
	else
		m_btnExp_Left.SetLoadImage(IDB_EXP_LEFT);
	
	if (m_pctrlChart != NULL)
	{
		m_ctrlCodeName_Chart.ShowWindow(m_bExp_Left);
		m_pctrlChart->ShowWindow(m_bExp_Left);
	}

	CRect rect;
	m_pctrlChart->GetWindowRect(rect);
	ScreenToClient(rect);

	VARIANT nWidth, nHeight;
	m_lRunForm.GetCurrentScreenSize(&nWidth, &nHeight);

	int nOffset = ::GetSystemMetrics(SM_CXBORDER) + 3;
	if (m_bExp_Left == TRUE)
	{
		m_lRunForm.SetFormResize(rect.right+nOffset, nHeight.iVal);
	}
	else
	{
		m_lRunForm.SetFormResize(rect.left, nHeight.iVal);
	}

	OnSize();
	
	m_ctrlCodeName.Invalidate();
	m_btnInit_Code.Invalidate();
	
	m_comboSignalType.Invalidate();
	m_btnExp_Left.Invalidate();
	
//@유진삭제	m_btnSell.Invalidate();
//@유진삭제	m_btnBuy.Invalidate();
	
	m_btnSignalSet.Invalidate();
	m_btnSearch.Invalidate();
	
	if(m_gridResult.GetSafeHwnd())
		m_gridResult.Invalidate();
	
	m_ctrlCodeName_Chart.Invalidate();
}

//@유진삭제	
//-->
//void CTrMainView::OnBtnSell()
//{	
//	CString strCode;
//	strCode = m_gridResult.GetRowToCode(m_gridResult.m_nLastSelRow);
//	
//	CString strData;
//	//# 1:HTS  0:FRONT(지점)
//	strData = KSLink_GetLoginInfo(ISHTS);
//
//	if(strData[0]=='1')	
//		SetOpenScreen(0, "1003", strCode);	//# 주식주문화면(2101)
////	else	SetOpenScreen(0, "211600", strCode);	//# 주식 주문화면 14101)	
//}
//<--

void CTrMainView::OnChangeSignalType()
{
	int nCurSel = m_comboSignalType.GetCurSel();
	
	SetSearch();
}

//@유진삭제
/*
void CTrMainView::OnBtnSell()
{
	CString strCode;
	strCode = m_gridResult.GetRowToCode(m_gridResult.m_nLastSelRow);
	
	CString strData;
	//# 1:HTS  0:FRONT(지점)
	strData = KSLink_GetLoginInfo(ISHTS);

	if(MAX_VERSION==200703)
	{
		// 기존 루틴 현재 지워놓음.
	}
	else
	{
		if(strData[0]=='1')		SetOpenScreen("2101", strCode);	//# 주식주문화면(2101)
		else	SetOpenScreen("14101", strCode);	//# 주식 주문화면 14101)
	}
}
*/

/*
void CTrMainView::OnBtnBuy()
{
CString strCode;
strCode = m_gridResult.GetRowToCode(m_gridResult.m_nLastSelRow);

  //# 주식 매수(2102)
  SetOpenScreen("2102", strCode);
  }
*/

void CTrMainView::OnBtnSignalSet()
{
	//# 신호 설정(1508)
	SetOpenScreen( 0 , "2116"); //다이얼로그 6자리 
}

void CTrMainView::OnBtnSearch()
{
	CString strCode;
	strCode = m_ctrlJMCombo.GetCaption();
	if (strCode.IsEmpty() == TRUE)
		SetSearch();
	else
	{
		if (m_ctrlJMCombo.CheckCaption() == TRUE)
		{
			CString strData;
			strData = m_ctrlJMCombo.GetCurName();
			
			strData.Replace("&","&&");
			m_ctrlCodeName.SetWindowText(strData);
			
			SetSearch();
		}
		else
			SetSendMainMsg("정확하지 않은 종목코드입니다.");
	}
}

void CTrMainView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	
	CBrush BackBrush(m_crBackColor);
	
	CRect rect;
	dc.GetClipBox(&rect);
	dc.FillRect(rect, &BackBrush);	

	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.bottom = rcClient.top + 2;
	dc.FillSolidRect(rcClient, RGB(255,255,255));
	
	BackBrush.DeleteObject();
	// Do not call CFormView::OnPaint() for painting messages
}

long CTrMainView::OnGetBrod(WPARAM wParam, LPARAM lParam)
{
	REALDATA_st stRealData;
	stRealData = *(REALDATA_st*)lParam;
	SetAdd_SignalData((int)stRealData.chRealType, stRealData.pDataBuf, stRealData.nSize);
	return 0;
}

//유진 : Client To Main 신호 define
#define SIGNAL_JONGMOKSIG		30	//		종목시그널
#define SIGNAL_CONDSEARCH		31	//		조건검색
#define SIGNAL_CONDSYSTEM		32	//		시스템트레이딩

BOOL CTrMainView::SendToEugeneMain(SignalData *pstSignalData)
{
	if(m_nScreenType == 1)
		return FALSE;
	
	//유진 Make Packet To Eugene Main Signal Manager 
	EUGENE_SIGNALMSGINFO	stEUGENE_SIGNALMSGINFO;
	memset(&stEUGENE_SIGNALMSGINFO, 0x00, sizeof(EUGENE_SIGNALMSGINFO));
	ConvertForEugeneSignalManager(pstSignalData, &stEUGENE_SIGNALMSGINFO);
//

//유진 Sendto Eugene Main Signal Manager
//	INewsSignalManager* pNSMng = (INewsSignalManager*)AfxGetPctrInterface(UUID_INewsSignalManager);

	HWND hKSWnd;
	hKSWnd = (HWND)KSLink_GetEnvInfo(2);

	CWnd* pWndKSLink = NULL;
	pWndKSLink = (CWnd*)FromHandle(hKSWnd);
	
			
	if(pWndKSLink && ::IsWindow(pWndKSLink->m_hWnd))
		pWndKSLink->SendMessage(UM_SM_SIGNAL_MSG, SIGNAL_JONGMOKSIG, (LPARAM)&stEUGENE_SIGNALMSGINFO);


	return TRUE;
}



BOOL CTrMainView::ConvertForEugeneSignalManager(SignalData *pstSignalData, EUGENE_SIGNALMSGINFO *pstEUGENE_SIGNALMSGINFO)
{
	CString strData, strNews;

	memcpy(pstEUGENE_SIGNALMSGINFO->m_sgType,		_T("신호"), 2); 

	strData = pstSignalData->strTime;
	memcpy(pstEUGENE_SIGNALMSGINFO->m_sgTime,	strData, strData.GetLength());	

	if(pstSignalData->nRealType == (int)TYPE_SIGNAL_SYS || pstSignalData->nRealType == (int)TYPE_SIGNAL_ALERT)
		strData.Format("%26s", " ");
	else
	{
		strData.Format("%8s", pstSignalData->strDate);
		strNews += strData;
		strData.Format("%6s", pstSignalData->strTime); 
		strNews += strData;
		strData.Format("%8s", pstSignalData->strSqNo);
		strNews += strData;
		strData.Format("%2s", pstSignalData->strProfferID);
		strNews += strData;
		strData.Format("%2s", pstSignalData->strSubProfferID); 
		strNews += strData;
		
		strData = strNews;
	}
	memcpy(pstEUGENE_SIGNALMSGINFO->m_sgnskey, strData, strData.GetLength());
	
	if(pstSignalData->nRealType == (int)TYPE_SIGNAL_SYS || pstSignalData->nRealType == (int)TYPE_SIGNAL_ALERT)
		strData.Format("%8s", " ");
	else
		strData.Format("%d", pstSignalData->strSqNo);
	
	memcpy(pstEUGENE_SIGNALMSGINFO->m_sgSeqno,	strData, strData.GetLength());//일련번호

	if(pstSignalData->nMarket < 0 || pstSignalData->nMarket > 4)
		strData.Format("%s", " ");
	else
		strData.Format("%d", pstSignalData->nMarket);
	memcpy(pstEUGENE_SIGNALMSGINFO->m_sgCodeGb,	strData, strData.GetLength());
	strData = pstSignalData->strCode;
	memcpy(pstEUGENE_SIGNALMSGINFO->m_sgCode,	strData, strData.GetLength());
	//# 신규:'N', 재진입:'R', 탈퇴:'O'
	CString strContext;

	strContext = pstSignalData->strName + " : ";
	strContext += pstSignalData->strTitle;
/*	STR_CONVERT(pBuff, pstSignalData->strType, 1, strData);
	CString strType;
	if(strData == "N")
		strType = "신규";
	else if(strData == "R")
		strType = "재진입";
	else if(strData == "O")
		strType = "탈퇴";
		
	strContext =  strContext + "!" + strType;
*/	
	
	memcpy(pstEUGENE_SIGNALMSGINFO->m_sgContext,	strContext, strContext.GetLength());

	int nPoint = pstSignalData->nPoint;
	
	if(nPoint > 0)
		memcpy(pstEUGENE_SIGNALMSGINFO->m_sgGubun,	"2",	1);
	else if(nPoint < 0)
		memcpy(pstEUGENE_SIGNALMSGINFO->m_sgGubun,	"3",	1);
	else
		memcpy(pstEUGENE_SIGNALMSGINFO->m_sgGubun,	"1",	1);
/*	memcpy(stEUGENE_SIGNALMSGINFO->m_sgBlink,	stSIGNAL_CONDITION
	memcpy(stEUGENE_SIGNALMSGINFO->m_sgContinue, stSIGNAL_CONDITION
	memcpy(stEUGENE_SIGNALMSGINFO->m_sgPriority, stSIGNAL_CONDITION
	memcpy(stEUGENE_SIGNALMSGINFO->m_sgIcon,		stSIGNAL_CONDITION
	memcpy(stEUGENE_SIGNALMSGINFO->m_sgBugaInfo, stSIGNAL_CONDITION
*/	
	return TRUE;

}

void CTrMainView::SetAdd_SignalData(int nRealType, LPSTR sRealData, int nLen)
{
	BOOL bAddData = FALSE;
	
	CString strData;
	SignalData stSignalData;
	
	if ((int)TYPE_REAL_NEWS == nRealType)
	{
		if (nLen != sizeof(SIGANL_NEWS))
			return;
	}
	else if ((int)TYPE_SIGNAL_NEWS == nRealType)
	{
		if (nLen != sizeof(SIGANL_NEWS))
			return;
	}
	else if ((int)TYPE_SIGNAL_SYS == nRealType)
	{
		if (nLen != sizeof(SIGNAL_CONDITION))
			return;
	}
	else if ((int)TYPE_SIGNAL_ALERT == nRealType)
	{
		if (nLen != sizeof(SIGNAL_CONDITION))
			return;
	}
	
	bAddData = GetConvert_RealToSignalData(nRealType, sRealData, stSignalData);

	if (bAddData == TRUE)
	{
		//# Add Data!
		SetAdd_SignalData(stSignalData);

//유진수정		int nCurSel = m_comboSignalType.GetCurSel();
		int nCurSel = GetCurSearchType();
		
		bAddData = FALSE;
		if ((int)TYPE_SEARCH_ALL == nCurSel)   //# 전체
			bAddData = TRUE;
		else if ((int)TYPE_SEARCH_SYS == nCurSel)    //# 시스템
		{
			if ((int)TYPE_SIGNAL_SYS == nRealType)
				bAddData = TRUE;
		}
		else if ((int)TYPE_SEARCH_NEWS == nCurSel)    //# 뉴스
		{
			if (((int)TYPE_REAL_NEWS == nRealType) || ((int)TYPE_SIGNAL_NEWS == nRealType))
				bAddData = TRUE;
		}
		
		if (bAddData == TRUE)
		{
			CString strCode;
			if(m_nState==1)
				strCode = m_ctrlJMCombo.GetCaption();
			
			if (strCode.IsEmpty() == FALSE)
			{
				if (stSignalData.strCode != strCode)
					bAddData = FALSE;
			}
			
		//	if (stSignalData.strCode.IsEmpty() == TRUE)
		//		bAddData = FALSE;     //20080604 유진추
	
			if (bAddData == TRUE)
				if(m_gridResult.GetSafeHwnd())
				{
					m_gridResult.SetAddRow(TRUE, &stSignalData);
					SendToEugeneMain(&stSignalData);
				}
			
		}
	}
}

void CTrMainView::SetAdd_SignalNowPrice(STSIGNAL_NOWPRICE stSignalNowPrice)
{
	int nRow_Prev;
	int nLastRow = m_gridSignal.GetRowCount();
//	if(m_nGridRow == nLastRow)
//				m_gridSignal.InsertRows(++nLastRow, 1);

	if(m_nGridRow == 102)
	{
		m_gridSignal.RemoveRows(m_nGridRow - 1, m_nGridRow -1 );
		m_gridSignal.InsertRows(2, 1);
				
		nRow_Prev = m_nGridRow - 1;  //다음 호출시 다시 마지막 라인을 가리키기 위해
		m_nGridRow = 2;				//첫줄에 Insert
	}
	else
	{
		m_gridSignal.InsertRows(2, 1);
		nRow_Prev = m_nGridRow;
		m_nGridRow = 2;
//		m_gridSignal.RemoveRows(nLastRow , nLastRow);
	}


	CString strTime, strDisplayTime;
	strTime = stSignalNowPrice.strTime;

	strDisplayTime = strTime.Left(2) + ":";
	strDisplayTime += strTime.Mid(2, 2) + ":";
	strDisplayTime += strTime.Right(2);	
//#시간
	m_gridSignal.SetCellString(m_nGridRow, 1, strDisplayTime, 0);
//#내용
//	CString strContens;
//	strContens = stSignalNowPrice.strName + " " + stSignalNowPrice.strTitle;
	m_gridSignal.SetCellString(m_nGridRow, 2, stSignalNowPrice.strTitle, 0);
//#진입, 재진입
	CString strJobFlag;
	if(stSignalNowPrice.strJobFlag == "N")
		strJobFlag = "신규";
	else if(stSignalNowPrice.strJobFlag == "R")
		strJobFlag = "재진입";

	m_gridSignal.SetCellString(m_nGridRow, 3, strJobFlag, 0);
//#포착가격
	m_gridSignal.SetCellString(m_nGridRow, 4, stSignalNowPrice.strPrice, 0);

//	m_nGridRow++;
	m_nGridRow = nRow_Prev + 1;
}

BOOL CTrMainView::GetConvert_RealToSignalData(int nRealType, LPSTR sRealData, SignalData &stSignalData)
{
	BOOL bResult = TRUE;
	
	if (((int)TYPE_SIGNAL_NEWS == nRealType) || ((int)TYPE_REAL_NEWS == nRealType))
	{
		SIGANL_NEWS stSIGANL_NEWS;
		memset(&stSIGANL_NEWS, 0x00, sizeof(SIGANL_NEWS));
		memcpy(&stSIGANL_NEWS, sRealData, sizeof(SIGANL_NEWS));
		
		CSIGANL_NEWSDATA  csData;
		csData.Convert(&stSIGANL_NEWS);
		
		stSignalData.nRealType = nRealType;
		stSignalData.strType = csData.m_strType;
		stSignalData.strSqNo = csData.m_strSqNo;
		
		CString strData;
		strData.Format("%0.2d",csData.m_nProfferID);
		stSignalData.strProfferID = strData;
		stSignalData.strSubProfferID = csData.m_strProfferTypeID;

	
		stSignalData.nMarket = csData.m_nMarket;//유진

		stSignalData.strCode = csData.m_strCode;
		stSignalData.strName = csData.m_strName;
		
		stSignalData.strDate = csData.m_strDate;
		stSignalData.strTime = csData.m_strTime;
		
		stSignalData.strTitle = csData.m_strTitle;
		stSignalData.nPoint = csData.m_nSignalDegree;
		stSignalData.clrTitle = GetPointToColor(csData.m_nSignalDegree);
	}
	else if (((int)TYPE_SIGNAL_SYS == nRealType) || ((int)TYPE_SIGNAL_ALERT == nRealType))
	{
		SIGNAL_CONDITION stSIGNAL_CONDITION;
		memset(&stSIGNAL_CONDITION, 0x00, sizeof(SIGNAL_CONDITION));
		memcpy(&stSIGNAL_CONDITION, sRealData, sizeof(SIGNAL_CONDITION));		

		char *pBuff;
		CString strData;
		STR_CONVERT(pBuff, stSIGNAL_CONDITION.sJobFlag, sizeof(stSIGNAL_CONDITION.sJobFlag), strData);
			
		//# 신규:'N', 재진입:'R', 탈퇴:'O'
		if (strData != "O")
		{
			CSIGNAL_CONDITIONDATA  csData;
			csData.Convert(&stSIGNAL_CONDITION);
			
			stSignalData.nRealType = nRealType;
			stSignalData.strType = "";
			
			stSignalData.nMarket = csData.m_nMarket;//유진

			stSignalData.strCode = csData.m_strCode;
			stSignalData.strName = csData.m_strName;
			
			stSignalData.strDate = csData.m_strDate;
			stSignalData.strTime = csData.m_strTime;
			
			stSignalData.strTitle = csData.m_strSignalName;
			stSignalData.nPoint = csData.m_nSignalDegree;
			stSignalData.clrTitle = GetPointToColor(csData.m_nSignalDegree);

			if(m_nScreenType == 1)
				GetSignal_NowPrice(stSIGNAL_CONDITION);	//20080522 김성조 <101>	현재가 화면 Grid	
		}
		else
			bResult = FALSE;
	}
	else
		bResult = FALSE;
	
	return bResult;
}
//20080522 김성조 <101
void CTrMainView::GetSignal_NowPrice(SIGNAL_CONDITION  stSIGNAL_CONDITION)
{

	STSIGNAL_NOWPRICE	stSTSIGNAL_NOWPRICE;
//	memset(&stSTSIGNAL_NOWPRICE, 0x00, sizeof(STSIGNAL_NOWPRICE));
	
	char *pBuff;
	CString strData;
	STR_CONVERT(pBuff, stSIGNAL_CONDITION.sJobFlag, sizeof(stSIGNAL_CONDITION.sJobFlag), strData);
	
	CSIGNAL_CONDITIONDATA	csData;
	csData.Convert(&stSIGNAL_CONDITION);

	stSTSIGNAL_NOWPRICE.strJobFlag = strData;
	stSTSIGNAL_NOWPRICE.strPrice = csData.m_strPrice;
	stSTSIGNAL_NOWPRICE.strCode = csData.m_strCode;	
	stSTSIGNAL_NOWPRICE.strName = csData.m_strName;
	stSTSIGNAL_NOWPRICE.strTime = csData.m_strTime;
	stSTSIGNAL_NOWPRICE.strTitle = csData.m_strSignalName;

	//if(strcmp(csData.m_strType, "NEWS") == 0 )	return;

	CString strCode = m_ctrlJMCombo.GetCaption();
	if(strCode == stSTSIGNAL_NOWPRICE.strCode)
		SetAdd_SignalNowPrice(stSTSIGNAL_NOWPRICE);
}

//20080522 김성조 101>
COLORREF CTrMainView::GetPointToColor(int nPoint)
{
	COLORREF clrPointText = RGB(255, 255, 255);
	
	if (nPoint <= -3)
		clrPointText = RGB(52, 70, 133);
	else if (nPoint == -2)
		clrPointText = RGB(0, 114, 188);
	else if (nPoint == -1)
		clrPointText = RGB(0, 174, 239);
	else if (nPoint == 0)
		clrPointText = RGB(135, 135, 135);
	else if (nPoint == 1)
		clrPointText = RGB(250, 166, 26);
	else if (nPoint == 2)
		clrPointText = RGB(245, 130, 32);
	else if (nPoint >= 3)
		clrPointText = RGB(237, 28, 36);
	
	return clrPointText;
}

void CTrMainView::SetAdd_SignalData(SignalData stSignalData)
{
	CString strCode, strType;
	strCode = stSignalData.strCode;
	strType.Format("%d", stSignalData.nRealType);
	
	if (strCode.IsEmpty() == TRUE)
		return;
	
	CList <SignalData, SignalData&> *plistSignalData;
	//# ALL
	if (m_mapSignalData_Type.Lookup("ALL", (void*&)plistSignalData) == TRUE)
		plistSignalData->AddTail(stSignalData);
	else
	{
		plistSignalData = new CList <SignalData, SignalData&>;
		plistSignalData->AddHead(stSignalData);
	}
	m_mapSignalData_Type.SetAt("ALL", (void*&)plistSignalData);
	
	//# Type
	if (m_mapSignalData_Type.Lookup(strType, (void*&)plistSignalData) == TRUE)
		plistSignalData->AddTail(stSignalData);
	else
	{
		plistSignalData = new CList <SignalData, SignalData&>;
		plistSignalData->AddHead(stSignalData);
	}
	m_mapSignalData_Type.SetAt(strType, (void*&)plistSignalData);
	
	//# Code
	if (m_mapSignalData_Code.Lookup(strCode, (void*&)plistSignalData) == TRUE)
		plistSignalData->AddTail(stSignalData);
	else
	{
		plistSignalData = new CList <SignalData, SignalData&>;
		plistSignalData->AddHead(stSignalData);
	}
	m_mapSignalData_Code.SetAt(strCode, (void*&)plistSignalData);
}

void CTrMainView::SetSearch()
{
//유진수정	int nCurSel = m_comboSignalType.GetCurSel();
	int nCurSel = GetCurSearchType();
	
	CString strData;
	if(m_nState==1)
		strData = m_ctrlJMCombo.GetCaption();
	
	if (strData.IsEmpty() == TRUE)
	{
		if(m_gridResult.GetSafeHwnd())
			m_gridResult.SetHideCol(1, TRUE);
		
		if ((int)TYPE_SEARCH_ALL == nCurSel)
			strData = "ALL";
		else if ((int)TYPE_SEARCH_NEWS == nCurSel)
			strData.Format("%d", (int)TYPE_SIGNAL_NEWS);//20080522 김성조 <101>//TYPE_SIGNAL_NEWS);
		else if ((int)TYPE_SEARCH_SYS == nCurSel)
			strData.Format("%d", (int)TYPE_SIGNAL_SYS);//20080522 김성조 <101>//TYPE_SIGNAL_SYS);
		
		//(int)TYPE_SIGNAL_ALERT
		if (SetFind_SignalData(FALSE, strData) > 0)
		{
			if(m_gridResult.GetSafeHwnd())
			{
				m_gridResult.SetSelectRow(-1, 0);
				strData = m_gridResult.GetRowToCode(0);
			}
		}
		else
			strData = "";
		SetSearchData_Chart(strData);
	}
	else
	{
		if (m_ctrlJMCombo.CheckCaption() == TRUE)
		{
			if(m_gridResult.GetSafeHwnd())
				m_gridResult.SetHideCol(1, FALSE);
			
			SetSearchData_Chart(strData);
			SetFind_SignalData(TRUE, strData);
		}
		else
			SetSendMainMsg("잘못된 종목 코드입니다.");
	}
}

int CTrMainView::SetFind_SignalData(BOOL bCode, CString strKey)
{
	int nResult = 0;
	if(m_gridResult.GetSafeHwnd())
		m_gridResult.ClearAll();
	
	CMapStringToPtr *pmapData;
	if (bCode == TRUE)
		pmapData = &m_mapSignalData_Code;
	else
		pmapData = &m_mapSignalData_Type;
	
	CList <SignalData, SignalData&> *plistSignalData;
	if (pmapData->Lookup(strKey, (void*&)plistSignalData) == TRUE)
	{
		POSITION pos;
		SignalData stSignalData;
		if(m_gridResult.GetSafeHwnd())
			m_gridResult.SetNumberRows(plistSignalData->GetCount());
		
		BOOL bAdd;
//유진수정		int nCurSel = m_comboSignalType.GetCurSel();
		int nCurSel = GetCurSearchType();

		CString strCode = "";
		if(m_nState != 0)
			 strCode = m_ctrlJMCombo.GetCaption();
		//for (int nIndex = 0; nIndex < plistSignalData->GetCount(); nIndex++)
		for (int nIndex = plistSignalData->GetCount() - 1; nIndex >= 0; nIndex--)
		{
			pos = plistSignalData->FindIndex(nIndex);
			stSignalData = plistSignalData->GetAt(pos);
			
			bAdd = FALSE;
			if (bCode == TRUE)
			{
				if ((int)TYPE_SEARCH_ALL == nCurSel)   //# All
					bAdd = TRUE;
				else if ((int)TYPE_SEARCH_SYS == nCurSel)    //# Sys
				{
					if ((int)TYPE_SIGNAL_SYS == stSignalData.nRealType)//20080522 김성조<101>
						bAdd = TRUE;
				}
				else if ((int)TYPE_SEARCH_NEWS == nCurSel)    //# News
				{
					if ((int)TYPE_SIGNAL_NEWS == stSignalData.nRealType) 
					//20080522 김성조<101>
						bAdd = TRUE;
				}
			}
			else
			{
				if (strCode.IsEmpty() == TRUE)
					bAdd = TRUE;
				else
				{
					if (strCode == stSignalData.strCode)
						bAdd = TRUE;
				}
			}
			
			if (bAdd == TRUE)
			{
				if(m_gridResult.GetSafeHwnd())
					m_gridResult.SetAddRow(FALSE, &stSignalData, nResult);
				nResult++;
			}
		}
		SetSendMainMsg("검색완료");
	}
	else
		SetSendMainMsg("입력된 종목의 데이터가 없습니다.");
	
	if(m_gridResult.GetSafeHwnd())
		m_gridResult.RedrawAll();
	return nResult;
}

void CTrMainView::SetDelAll_SignalData()
{
	POSITION pos;
	CString strKey;
	CList <SignalData, SignalData&> *plistSignalData;
	for (pos = m_mapSignalData_Type.GetStartPosition(); pos != NULL;)
	{
		m_mapSignalData_Type.GetNextAssoc(pos, strKey, (void *&)plistSignalData);
		
		plistSignalData->RemoveAll();
		delete plistSignalData;
		m_mapSignalData_Type.RemoveKey(strKey);
	}
	m_mapSignalData_Type.RemoveAll();
	
	for (pos = m_mapSignalData_Code.GetStartPosition(); pos != NULL;)
	{
		m_mapSignalData_Code.GetNextAssoc(pos, strKey, (void *&)plistSignalData);
		
		plistSignalData->RemoveAll();
		delete plistSignalData;
		m_mapSignalData_Code.RemoveKey(strKey);
	}
	m_mapSignalData_Code.RemoveAll();
}

HBRUSH CTrMainView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(COLOR_MAIN_CANVAS);
		hbr = m_brushStatic;
	}
	
	return hbr;
}

//nType 0:일반, 1:Dialog
void CTrMainView::SetOpenScreen(int nType, CString strMapNo, CString strSendData)
{
	if (strSendData.IsEmpty() == TRUE)
	{
		if(nType==1)
		{
			m_lRunForm.DialogScreenCreate(strMapNo, "신호검색설정", 0, "");
		}
		else
		{
			if (strMapNo.GetLength() > 5)
				m_lRunForm.CreateLinkScreen(strMapNo, TRUE, TRUE);
			else
				m_lRunForm.OpenMenuScreen(strMapNo, 0);
		}
	}
	else
	{
	//	CString strAccCode = KSLink_GetAccount(0);
	//	m_lRunForm.SetFormLinkInfoData( FALSE , 0 , strSendData/*code*/ , strAccCode);
			//		, sOrderPrice ,	sOrderQty , sPriceTp , sDealTp , sOrderNo , sUser )
	
		m_lRunForm.OpenMapCreateFromCode(strSendData, strMapNo);
//		m_lRunForm.CreateLinkScreen("100300", TRUE, TRUE);
		

	}
	//return m_lRunForm.CreateLinkScreen(strMapNo, TRUE, TRUE);
}

void CTrMainView::SetLinkCode(CString strCode, BOOL bTagData)
{
	if (strCode.IsEmpty() == TRUE)
		return;
	
	if (bTagData == TRUE)
		m_lRunForm.SetLinkInfoTagData("0101_종목코드", strCode);
	else
		m_lRunForm.OpenMapCreateFromCode(strCode, "");
}

void CTrMainView::SetLinkTime(CString strTime)
{
	//m_lTimeEdit.SetCaptionCtrl(strTime);
}

void CTrMainView::SetSendMainMsg(CString strMsg)
{
	m_lRunForm.SetFrameMsgText(strMsg);
}

void CTrMainView::SetSearchData_Chart(CString strCode)
{
//	m_ctrlJMCombo_Chart.SetCaptionCtrlEx(strCode, TRUE);
//-->TestCode JunokLee 080528
	#include "../../Contents/inc/IMasterDataMng.h"
	IMasterDataManager080331* pMng = (IMasterDataManager080331*)AfxGetPctrInterface(UUID_IMasterDataManager080331);
	if(pMng)
	{
		if(strCode.GetLength()>0)
		{
			BYTE btye = pMng->GetMarketTypeFromCode(strCode);
			//int nType = (int)btye; //strType[0] - '0';
			int nMaret = 1 ; //주식(1)/업종(2)/선옵...
			m_ctrlChart.RequestDirect(nMaret, strCode, "분", 1, TRUE);
		}
	}
	
	

//<--
}

void CTrMainView::SetSearchData_Time(CString strTime)
{
	m_strTime = strTime;
}

void CTrMainView::SetSearchData_TimeSet()
{
	CString strPropName;
	strPropName = "SetTimeMark(" + m_strTime + ", 1)";
	m_ctrlChart.GetChartProperty(strPropName);
}

LONG CTrMainView::OnRmsgOutside(WPARAM wParam, LPARAM lParam)
{
	if(wParam==100)	//메인에서 신호 클릭시.
	{
		OnBtnInit_Code();
	}
	return 0L;
}

// 메인의 저장화면관련.
void CTrMainView::OnFormGetPageSave(LPCTSTR strFileName)
{
}

// 메인의 저장화면관련.
void CTrMainView::OnFormSetPageSave(LPCTSTR strFileName)
{
}

//메인의 스킨변경시 불린다.
void CTrMainView::OnColorChange()
{
	IKSLibManager*	pKSMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager);
	if(pKSMng)
		m_crBackColor = pKSMng->Lib_KSLink_GetColor(1);
	
	m_brushStatic.DeleteObject();
	m_brushStatic.CreateSolidBrush(m_crBackColor);
}

void CTrMainView::OnRadioBtnClk(UINT nID)
{	
	if(m_radioTotal.GetDlgCtrlID() == nID)
	{	
		m_nState=0;
		CString strData;
		m_radioTotal.SetCheck(TRUE);
		m_radioCode.SetCheck(FALSE);

		DoBtnInit_Code(FALSE);			
		m_pctrlCodeInput->EnableWindow(FALSE);
//		m_ctrlJMCombo.SetEnabled(FALSE);
	}
	else
	{
		m_radioTotal.SetCheck(FALSE);
		m_radioCode.SetCheck(TRUE);
//		m_ctrlJMCombo.SetEnabled(TRUE);
		m_pctrlCodeInput->EnableWindow(TRUE);

		int nOldState = m_nState;
		m_nState=1;
		if(nOldState==0)	SetSearch();
		
	}
	//CWnd::FromHandle(m_hKSJongName)->ShowWindow(m_nState);
}

void CTrMainView::OnChkBtnClk(UINT nID)
{
	switch(nID)
	{
	case IDC_CHK_MARKETCONDITION:
		break;
	case IDC_CHK_TECHINDEX:
		break;
	case IDC_CHK_DEALAGENCY:
		break;
	case IDC_CHK_NEWS:
		{
			if(!m_chkSys.GetCheck())
				m_chkNews.SetCheck(TRUE);
	//		else if(!m_chkSys.GetCheck())
	//			m_chkSys.SetCheck(FALSE);
			OnChangeSignalType();
			break;
		}		
	case IDC_CHK_SYS:
		{
			if(!m_chkNews.GetCheck())
				m_chkSys.SetCheck(TRUE);
	//		else
	//			m_chkNews.SetCheck(FALSE);
			OnChangeSignalType();
			break;
		}		
	default:
		break;
	}

}

short CTrMainView::GetCurSearchType()
{
	short nResult = 0;
/*
	if(m_chkNews.GetCheck())
		nResult = 0;
	else if(!m_chkNews.GetCheck())
		nResult = 2;
*/
	if(m_chkNews.GetCheck())
	{
		if(m_chkSys.GetCheck())
			nResult = 0;
		else
			nResult = 1;
	}
	else if(m_chkSys.GetCheck())
		nResult = 2;
	
	return nResult;
}

//void CTrMainView::OnTimer(UINT nIDEvent)
//{
//	if(nIDEvent==ID_CLEARCAPTION)
//	{
//		KillTimer(nIDEvent);
//		m_ctrlJMCombo.SetCaption("");
//	}
//	CFormView::OnTimer(nIDEvent);
//}

void CTrMainView::SetControlDispatch()
{
	CSCNode* pSCNode = NULL;
	for(int nIndex = 0; nIndex < sizeof(nCtrlID)/sizeof(int); nIndex++)
	{
		if(m_MapControlName->Lookup(sCtrlName[nIndex], (CObject*&)pSCNode) )
		{
			pSCNode->GetControlWnd()->m_pWnd->SetDlgCtrlID(nCtrlID[nIndex]);
			pSCNode->GetControlWnd()->SetCtrlID(nCtrlID[nIndex]);
		}
	}	
}

void CTrMainView::DoKSControlMove(LPCSTR szCtrlName, CWnd* pWnd)
{
	if(!pWnd)
		return;

	pWnd->ShowWindow(SW_HIDE);

	CRect ctlrect;
	pWnd->GetWindowRect(&ctlrect);
	ScreenToClient(&ctlrect);

	VARIANT nLeft, nTop, nWidth, nHeight;
	nLeft.vt = VT_I2;
	nTop.vt = VT_I2;
	nWidth.vt = VT_I2;
	nHeight.vt = VT_I2;

	nLeft.iVal = ctlrect.left;
	nTop.iVal = ctlrect.top;
	nWidth.iVal = ctlrect.Width();
	nHeight.iVal = ctlrect.Height();	


	m_lRunForm.SetControlPos(szCtrlName, nLeft.iVal, nTop.iVal, nWidth.iVal, nHeight.iVal);
}

void CTrMainView::OnEuBtnClk(UINT nID)
{
	switch(nID)
	{
	case ID_btnSignalSet:
		OnBtnSignalSet();
		break;
	case ID_btnSearch:
		OnBtnSearch();
		break;
	default:
		break;
	}
}

//--> DLL화면 사이징처리관련추가
CWnd* CTrMainView::GetFrameWnd(CWnd* pWnd)
{
	CWnd* pParentWnd = pWnd->GetParent();  // start with one parent up
	while (pParentWnd != NULL)
	{
		DWORD dwExStyle = GetWindowLong(pParentWnd->GetSafeHwnd(), GWL_STYLE);
		if ( dwExStyle & 0x8000)
		{
			return pParentWnd;
		}
		pParentWnd = pParentWnd->GetParent();
	}
	return NULL;
}

void CTrMainView::DoDllSizeEx(HWND _hChildFrame)
{
	HWND hChildFrame = _hChildFrame;

	if(hChildFrame==NULL)
	{
		CWnd* pChildFrame = GetFrameWnd(this);
		hChildFrame = pChildFrame->GetSafeHwnd();
	}

	if(hChildFrame)
		::SendMessage(hChildFrame, UM_SET_FORMMAXSIZE, 34, MAKELPARAM(0, 0));
}
//<--

long CTrMainView::OnChangeChildFrameWindowHandle( WPARAM wParam, LPARAM lParam)
{
	m_hChildFrame = (HWND)lParam;

	return 0L;
}

void CTrMainView::OnRButtonUp(UINT nFlags, CPoint point)
{
	SetCommMenu(&point);

	CFormView::OnRButtonUp(nFlags, point);
}

extern void KSLink_SetFormLinkTagData( CString sTag, CString sValue );
//extern void KSLink_SetLinkTagData( LPCTSTR sTag, LPCTSTR sValue);
void CTrMainView::SetCommMenu(POINT *point)
{
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
}
