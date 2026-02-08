 // TrMChartView.cpp : implementation file
//

#include "stdafx.h"
#include "TrMChartView.h"
#include "SI050000.h"
//#include "../../../SrcSite/include/InterestInfo.h"

// #include "../../../Controls/GDIMgr/Gdidefine.h"
// #include "../../Contents/inc/IMasterDataMng.h"
// #include "../../../Common/KHResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_FORM			999

const UINT RMSG_GETINTERFACE = ::RegisterWindowMessage(_T("RMSG_GETINTERFACE"));

// #define WM_RP_RECEIVED WM_USER + 1000
// #define UMSG_SENDSHAREDCODE				WM_USER+30	// Main공유메모리영역에 코드를 보낸다.
// #define UMSG_RECEIVE_FIND0301			WM_USER+166 //05.07. 30 조건검색 종목등을 받는다.
// #define UMSG_FORMINITAFTER_FROMFORM		WM_USER+215 //08.04.29	FormInitAfter event 처리
// #define UMSG_FORMGETPAGESAVE			WM_USER+216 //08.04.29	OnFormGetPageSave event 처리
// #define UMSG_FORMSETPAGESAVE			WM_USER+217 //08.04.29	OnFormSetPageSave event 처리

// const UINT RMSG_GET_SCREENTYPE = ::RegisterWindowMessage(_T("RMSG_GET_SCREENTYPE"));
// const UINT RMSG_GET_MARKETTYPE = ::RegisterWindowMessage(_T("RMSG_GET_MARKETTYPE"));
// const UINT RMSG_CHANGE_SKINCOLOR = ::RegisterWindowMessage(_T("RMSG_CHANGE_SKINCOLOR"));
// const UINT RMSG_WRAP_GETINFO = ::RegisterWindowMessage(_T("RMSG_WRAP_GETINFO"));
// const UINT RMSG_GET_LINKINFODATA = ::RegisterWindowMessage(_T("RMSG_GET_LINKINFODATA"));
// const UINT RMSG_OPENSCREEN = ::RegisterWindowMessage(_T("RMSG_OPENSCREEN"));
// const UINT RMSG_ORDERHOGA = ::RegisterWindowMessage(_T("RMSG_ORDERHOGA"));

// 	extern void*   dll_GetCodeMaster( );
// 	extern BOOL    dll_GetMasterData(CString szType, CStringArray &arrCode, CStringArray &arrName);
// 	extern BOOL    dll_GetMasterDataInUp(CString szType, CStringArray &arrCode, CStringArray &arrName, CString strUp);
// 	extern LPCTSTR dll_GetJongmokName( CString szType, LPCTSTR szCode, BOOL bFullName=TRUE);
// 	//extern CWnd* CREATE_TRADING(int iTradingID, CWnd* pParent);
// 
//typedef	CWnd * (CreateTradingAup)(UINT nDldresId, CWnd* pParent, LPCTSTR strMapFileName);

// extern WORD GetCurLangeCode ( );


////////////////////////////////////////////////////
// 윈도우 ID 정의
////////////////////////////////////////////////////
// enum 
// {
// 	ID_cmbJCode = 1000,	
// 	ID_lbScreenType,
// 	ID_lbMarketType
// };	
// 
// 
// int nCtrlID[] = {
// 	ID_cmbJCode,
// 	ID_lbScreenType,
// 	ID_lbMarketType
// };

//////////////////////
// 맵 컨트롤명 정의
////////////////////////////////////////////////////
// CString sCtrlName[] = {
// 	"cmbJCode",
// 	"screenType",
// 	"marketType"
// };
// 
// extern CColorMap* GetColorMap();

/////////////////////////////////////////////////////////////////////////////
// CWinixGwanManager
IMPLEMENT_IWinixGwanManager_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTrMChartView
IMPLEMENT_DYNCREATE(CTrMChartView, CFormView)

CTrMChartView::CTrMChartView()
	: CFormView(CTrMChartView::IDD)
{
	//{{AFX_DATA_INIT(CTrMChartView)
	m_nInitFlag     = 0;

	m_hInstance		= NULL;
	//m_pChartScreen	= NULL;
	m_pChartScreen = NULL;
	m_strFormCode	=  theApp.m_szDllName; //"0500";
	m_strMapFileName= "";
	m_hChildFrame   = NULL;
	m_IsLoading = TRUE;
	//}}AFX_DATA_INIT
	//m_nCurMarketType = -1;

	m_nEnumScreenType = ST_MULTICHART;
}

CTrMChartView::~CTrMChartView()
{
	if(m_pChartScreen != NULL)
	{
		m_pChartScreen->DestroyWindow();
		delete m_pChartScreen;
		m_pChartScreen = NULL;
	}
	
	if(m_hInstance)
		FreeLibrary(m_hInstance);
}

void CTrMChartView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrMChartView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrMChartView, CFormView)
	//{{AFX_MSG_MAP(CTrMChartView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(RMSG_GETINTERFACE, OnRmsgGetInterface)
	ON_REGISTERED_MESSAGE(RMSG_WINIXPLATFORM, OnRmsgWinixPlatform)
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CTrMChartView, CFormView)
    //{{AFX_EVENTSINK_MAP(CTrMChartView)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

// ON_EVENT(CTrMChartView, ID_cmbJCode, 1 /* SelChanged */, OncmbJCodeSelChanged_EditFull, VTS_NONE)
// ON_EVENT(CTrMChartView, ID_cmbJCode, 2 /* SelChanged */, OncmbJCodeSelChanged_EditEnter, VTS_NONE)
// ON_EVENT(CTrMChartView, ID_cmbJCode, 4 /* SelChanged */, OncmbJCodeSelChanged_SelChanged, VTS_NONE)
// ON_EVENT(CTrMChartView, ID_FORM,	2 /* OnInitAfter */, OnFormInitAfter, VTS_NONE)
// ON_EVENT(CTrMChartView,	ID_FORM,	0xb /* OnGetPageSave */,	OnFormGetPageSave,	VTS_BSTR)
// ON_EVENT(CTrMChartView,	ID_FORM,	0xc /* OnSetPageSave */,	OnFormSetPageSave,	VTS_BSTR)
// ON_EVENT(CTrMChartView,	ID_FORM,	0x1f, OnColorChange , VTS_NONE)

/////////////////////////////////////////////////////////////////////////////
// CTrMChartView diagnostics

#ifdef _DEBUG
void CTrMChartView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTrMChartView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTrMChartView message handlers
int CTrMChartView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_crBackColor = RGB(255, 255, 255);

	OnInitialUpdate();

	return 0;
}
	
void CTrMChartView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);

	SetScrollSizes(MM_TEXT, CSize(10,10));
	if(::IsWindow(m_pChartScreen->GetSafeHwnd()))
		m_pChartScreen->SetWindowPos(NULL, 1, 0, cx-2, cy-1, SWP_NOZORDER);

}

void CTrMChartView::OnInitialUpdate() 
{	
	CFormView::OnInitialUpdate();

	if( m_nInitFlag == 0 )
	{
// 		SetChangeCtrlID();
// 	
// 		if(m_ScrFormDisp)
// 			m_Form.AttachDispatch(m_ScrFormDisp, FALSE);
// 
// 		for ( int i=0; i< sizeof(nCtrlID)/sizeof(int); i++)
// 		{
// 			CSCNode* pSCNode = NULL;
// 			IDispatch* pDispatch = NULL;
// 
// 			if( m_MapControlName->Lookup(sCtrlName[i], (CObject*&)pSCNode) )
// 			{
// 				pDispatch = pSCNode->GetControlWnd()->GetDispatchPtr();
// 				if (pDispatch)
// 				{
// 					if(i == 0)		// 종목콤보
// 						m_cmbJCodeDisp.AttachDispatch(pDispatch, FALSE);
// 					else if(i == 1)	// Screen Type
// 						m_lbScreenType.AttachDispatch(pDispatch, FALSE);
// 					else if(i == 2)	// Market Type
// 						m_lbMarketType.AttachDispatch(pDispatch, FALSE);
// 				}
// 			}
// 		}
// 			
// 		CColorMap* m_pColorMap = GetColorMap();
// 		if(m_pColorMap)
// 			m_crBackColor = m_pColorMap->GetColor(m_lBkColor);

		//SetScrollSizes(MM_TEXT, CSize(10,10));

		///////////////////////////////////////
        // 이 부분을 추가해주세요...
	/*	CWnd* pMainWnd = (CWnd*)AfxGetMainWnd();
		if(pMainWnd)
		{
			CDocument* pDoc = NULL;
			pDoc = (CDocument*)pMainWnd->SendMessage(UM_OPEN_MAP, 2, (long)(LPCTSTR)"0000");
			if(pDoc != NULL)
			{
				pDoc->AddView((CView*)this);
			}
		}
	*/
       	///////////////////////////////////////
		CWnd* pChildFrame = GetFrameWnd(this);
		m_hChildFrame = pChildFrame->GetSafeHwnd();

		CreateDll();

		// ChildFrame의 설정된 Max사이즈 셋팅을 무시 (--> Max사이즈보다 축소될 때 Size 이벤트 발생)
// 		if(m_hChildFrame)
// 			::SendMessage(m_hChildFrame, UM_SET_FORMMAXSIZE, 34, MAKELPARAM(0, 0));
	}

	m_nInitFlag++;
}


void CTrMChartView::OnDestroy() 
{
	CFormView::OnDestroy();

}

void CTrMChartView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CBrush BackBrush(m_crBackColor);

	CRect rect;
	dc.GetClipBox(&rect);
	dc.FillRect(rect, &BackBrush);	

	BackBrush.DeleteObject();
	// Do not call CFormView::OnPaint() for painting messages
}

// 	void CTrMChartView::SetChangeCtrlID()
// 	{
// 		for( int i=0; i< sizeof(nCtrlID)/sizeof(int); i++)
// 		{
// 			CSCNode* pSCNode = NULL;
// 			if( m_MapControlName->Lookup(sCtrlName[i], (CObject*&)pSCNode) )
// 			{
// 				pSCNode->GetControlWnd()->m_pWnd->SetDlgCtrlID(nCtrlID[i]);
// 				pSCNode->GetControlWnd()->SetCtrlID(nCtrlID[i]);
// 			}
// 		}
// 	}
// 
// 	void CTrMChartView::OncmbJCodeSelChanged_EditFull()
// 	{	
// 		CString strCode = m_cmbJCodeDisp.GetCaption();
// 		ChangeCodeInputStyle(strCode);
// 		CString strMarket = m_cmbJCodeDisp.GetCurMarketCode();
// 		CString strName = dll_GetJongmokName(strMarket, strCode);	
// 		m_pChartScreen->SendMessage(UMSG_SENDSHAREDCODE, (WPARAM)(LPCSTR)strCode, (LPARAM)0);
// 	}
// 
// 	void CTrMChartView::OncmbJCodeSelChanged_EditEnter()
// 	{	
// 		CString strCode = m_cmbJCodeDisp.GetCaption();
// 		ChangeCodeInputStyle(strCode);
// 		CString strMarket = m_cmbJCodeDisp.GetCurMarketCode();
// 		CString strName = dll_GetJongmokName(strMarket, strCode);	
// 		m_pChartScreen->SendMessage(UMSG_SENDSHAREDCODE, (WPARAM)(LPCSTR)strCode, (LPARAM)0);
// 	}
// 
// 	void CTrMChartView::OncmbJCodeSelChanged_SelChanged()
// 	{	
// 		CString strCode = m_cmbJCodeDisp.GetCaption();
// 		ChangeCodeInputStyle(strCode);
// 		CString strMarket = m_cmbJCodeDisp.GetCurMarketCode();
// 		CString strName = dll_GetJongmokName(strMarket, strCode);	
// 		m_pChartScreen->SendMessage(UMSG_SENDSHAREDCODE, (WPARAM)(LPCSTR)strCode, (LPARAM)0);
// 	}

// long CTrMChartView::OnGetData(WPARAM wParam, LPARAM lParam)
// {
// 
// 	return 0L;
// }


// long CTrMChartView::OnGetBrod(WPARAM wParam, LPARAM lParam)
// {
// 	return 0;
// }


// void CTrMChartView::ControlMove()
// {
// 	CWnd *pCombo;
// 
// 	CSCNode* pSCNode = NULL;
// 	
// 	if( m_MapControlName->Lookup(sCtrlName[1], (CObject*&)pSCNode) )
// 	{
// 		pCombo = pSCNode->GetControlWnd()->m_pWnd;
// 		pCombo->SetWindowPos(NULL,200,0,56, 20, SWP_NOZORDER);
// 				
// 	}
// 			
// }	

LRESULT CTrMChartView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
// 		switch(message)
// 		{
// 		case WM_USER+101:	//종목코드콤보 스타일 변경
// 			{
// 				int nType = wParam;
// 				CString strCode = (LPCSTR)lParam;
// 
// 				ChangeCodeInputStyle(strCode);
// 			}
// 			//break;
// 			return TRUE;
// 		case WM_USER+102:	//Open Screen
// 			{
// 				CString strScreen = (LPCTSTR)(LPCTSTR)wParam;
// 				CStringList* strData = (CStringList*)lParam;
// 				OpenScreen(strScreen, strData);
// 			}
// 			return TRUE;
// 		case WM_USER+103:	//타이틀 메시지
// 			{
// 				CString strMsgCode = (LPCTSTR)(LPCTSTR)wParam;
// 				CString strMsg = (LPCTSTR)(LPCTSTR)lParam;
// 				SetFrameMsgText(strMsg);
// 			}
// 			return TRUE;
// 		case 2000:	//종목콤보 리턴
// 				return (long)&m_cmbJCodeDisp;
// 			break;
// 		case 3000:	//종목콤보 리턴 
// 			if(m_MapControlName)
// 			{
// 				CWnd *pCombo = NULL;
// 				CSCNode* pSCNode = NULL;
// 		
// 				if( m_MapControlName->Lookup(sCtrlName[0], (CObject*&)pSCNode) )
// 				{
// 					pCombo = pSCNode->GetControlWnd()->m_pWnd;
// 				}
// 				return (long)pCombo;
// 			}
// 			break;
// 		case WM_INITMENUPOPUP:
// 		case WM_COMMAND:
// 			if(m_hChildFrame)
// 			{
// 				::PostMessage( m_hChildFrame, WM_COMMAND, wParam, 0);
// 				return TRUE;
// 			}
// 			else 
// 			{
// 			}
// 			//break;
// 			return TRUE;
// 		case WM_USER+104:	//프레임 얻어감
// 			return (long)m_hChildFrame;
// 		case WM_USER+105:	//TR보냄
// 			m_pFormWnd->SendMessage(WM_BEGIN_WAIT_CURSOR, 0, 0);
// 			//break;
// 			return TRUE;
// 		case WM_USER+106:	//TR받음
// 			m_pFormWnd->SendMessage(WM_END_WAIT_CURSOR, 0, 0);
// 			//break;
// 			return TRUE;
// 		}
// 
// 		if(message==RMSG_WRAP_GETINFO)
// 			return DoRmgWrapGetInfo(wParam, lParam);

	return CFormView::WindowProc(message, wParam, lParam);
}

// 	BOOL CTrMChartView::CreateDll()
// 	{
// 		CString strDllName;
// 		GET_ROOT_DIR(strDllName);
// 		strDllName += "\\dev\\MultiChart.dll";
// 		if(!m_hInstance)
// 		{
// 			m_hInstance = LoadLibrary(strDllName);
// 			if(m_hInstance == NULL)
// 			{
// 				CString strError;
// 				strError.Format(_T("%s File Load Faile"),strDllName);
// 				MessageBox(strError);
// 				return NULL;
// 			}
// 
// 			CreateTradingAup *pCreateTrading = (CreateTradingAup *)GetProcAddress(m_hInstance,_T("CREATE_TRADING"));
// 			if(pCreateTrading == NULL)
// 			{
// 				MessageBox("특화화면 로드 에러");
// 				FreeLibrary(m_hInstance);
// 				return NULL;
// 			}
// 
// 			m_pChartScreen = pCreateTrading(atoi(m_strFormCode),this,m_strMapFileName);
// 			m_pChartScreen->ModifyStyle(0, WS_CLIPSIBLINGS, 0);
// 		}
// 
// 		return TRUE;
// 	}

typedef	CWnd * (CreateChartScreen)(int nScreenID, CWnd* pParent, HWND hPlatform);

//100530:동일화면 다르게 저장/읽기
typedef	CWnd * (CreateChartScreenEx)(int nScreenID, CWnd* pParent, HWND hPlatform, LPCSTR szOpenInfo);

BOOL CTrMChartView::CreateDll()
{
	//return FALSE;

	if(m_pChartScreen)
		return TRUE;
	//if(m_strFormCode.IsEmpty())	m_strFormCode = m_pLbScreenTR->GetCaption();
	CString strMainDir;
	GetDir_DrfnBin(strMainDir);

	CString strDllName;
	//7123 : 차트-복합비교차트, HMC6501, KB0920
	//7124 : 차트-지수/업종상대비교차트, HMC6601, KB0962
	//0424 : 선물옵션/선물시세/선물전일대비차트 HMC6502(당일등락률차트)
	if(m_strFormCode == "7123" || m_strFormCode == "7124" || m_strFormCode == "0424")
		strDllName = strMainDir + "\\CompareChart.dll";
//	else if (m_strFormCode == "8041")
//		strDllName = strMainDir + "\\GNF_MultiChart.dll";
//	else if (m_strFormCode == "9041" || m_strFormCode == "9043")
//		strDllName = strMainDir + "\\FX_MultiChart.dll";
	else
		strDllName = strMainDir + "\\MultiChart.dll";


	if(!m_hInstance)
	{
		//{
		//	CString szDebug;
		//	szDebug.Format("[DRCS][LOAD](%8d) CTrMChartView::CreateDll strDllName(%s)\n", __LINE__, strDllName);
		//	OutputDebugString(szDebug);
		//}

		m_hInstance = LoadLibrary(strDllName);
		if(m_hInstance == NULL)
			return FALSE;

		m_wxGwnMng.DoInit();

		HWND hWinixHwnd = GetParent()->GetParent()->GetSafeHwnd();

		//100530:동일화면 다르게 저장/읽기 -->
		if(m_szFileKey.GetLength()==0)
		{
			CreateChartScreen *pCreateChartScreen = (CreateChartScreen*)GetProcAddress(m_hInstance, _T("CREATE_CHARTSCREEN"));
			if(pCreateChartScreen == NULL)
			{
				FreeLibrary(m_hInstance);
				return FALSE;
			}
			m_pChartScreen = pCreateChartScreen(atoi(m_strFormCode), this, hWinixHwnd);
		}
		else
		{
			CreateChartScreenEx *pCreateChartScreenEx = (CreateChartScreenEx*)GetProcAddress(m_hInstance, _T("CREATE_CHARTSCREENEX"));
			if(pCreateChartScreenEx != NULL)
			{
				m_pChartScreen = pCreateChartScreenEx(atoi(m_strFormCode), this, hWinixHwnd, (LPCSTR)m_szFileKey);
			}
			else
			{
// 				FreeLibrary(m_hInstance);
// 				return FALSE;

				// 없으면 기존 Export함수 찾기.
				CreateChartScreen *pCreateChartScreen = (CreateChartScreen*)GetProcAddress(m_hInstance, _T("CREATE_CHARTSCREEN"));
				if(pCreateChartScreen == NULL)
				{
					FreeLibrary(m_hInstance);
					return FALSE;
				}
				m_pChartScreen = pCreateChartScreen(atoi(m_strFormCode), this, hWinixHwnd);
			}
		}
		//100530:동일화면 다르게 저장/읽기 <--

		if( m_pChartScreen == NULL )
		{
			FreeLibrary(m_hInstance);
			return FALSE;
		}

		//AfxMessageBox사용시 MultiChart와 CompareChart에서의 타이틀을 일괄적으로 "차트"로 변경.
		free((void*)AfxGetApp()->m_pszAppName);
		AfxGetApp()->m_pszAppName = _tcsdup(_T("차트")); 

		//JS.Kim_20101005   화면 깜박임 현상 제거
		//m_pChartScreen->ModifyStyle(0, WS_CLIPSIBLINGS, 0);
		m_pChartScreen->ModifyStyle(0, WS_CLIPSIBLINGS|WS_CLIPCHILDREN, 0);

		
		if(m_strFormCode == "0500")			//6001
			m_nEnumScreenType = ST_MULTICHART;
		else if(m_strFormCode == "0501")	//6002
			m_nEnumScreenType = ST_STOCKCHART;
		else if(m_strFormCode == "0502")	//6003
			m_nEnumScreenType = ST_UPJONGCHART;
		else if(m_strFormCode == "0503")	//6004
			m_nEnumScreenType = ST_FUTOPTCHART;
		else if(m_strFormCode == "0515")	// FO차트
			m_nEnumScreenType = ST_FUTOPTCHART;
		else if(m_strFormCode == "0516")	//선옵과거종목조회차트.
			m_nEnumScreenType = ST_FUTOPTCHART;
		else if(m_strFormCode == "0506")	//6301
			m_nEnumScreenType = ST_STOCKCHART;
		else if(m_strFormCode == "0504")	//6005
			m_nEnumScreenType = ST_FOREIGNCHART;
		else if(m_strFormCode == "0367")	//6201
			m_nEnumScreenType = ST_MULTICHART;
		else if(m_strFormCode == "0368")		//6202
			m_nEnumScreenType = ST_STOCKCHART;
		//else if(m_strFormCode == "6203")
		//	m_nEnumScreenType = ST_STOCKCHART;
		else if(m_strFormCode == "0369")	//6204
			m_nEnumScreenType = ST_STOCKCHART;
		else if(m_strFormCode == "0370")	//6205
			m_nEnumScreenType = ST_UPJONGCHART;
		else if(m_strFormCode == "0371")	//6206
			m_nEnumScreenType = ST_FUTOPTCHART;
		else if(m_strFormCode == "0507")	//6501
			m_nEnumScreenType = ST_MULTICHART;
		//		else if(m_strFormCode == "0520")
		//			m_nEnumScreenType = ST_MULTICHART;
		else if(m_strFormCode == "0508")	//6401
			m_nEnumScreenType = ST_FUTOPTCHART;
		else if(m_strFormCode == "0510")	//6701
			m_nEnumScreenType = ST_MULTICHART;
		else if(m_strFormCode == "0511")	//상품선물 종합차트
			m_nEnumScreenType = ST_MARKETFUTURE;
		else if(m_strFormCode == "9041")	//FX 종합차트
			m_nEnumScreenType = ST_FXCHART;
		else if(m_strFormCode == "9043")	//FX 미니차트
			m_nEnumScreenType = ST_FXCHART;
	}
//	m_IsLoading = FALSE;
	return TRUE;
}


// void CTrMChartView::OpenScreen(CString strScreen, CStringList *p_strData)
// {
// 	if(p_strData && p_strData->GetCount()>0)
// 	{
// 		CString strData;
// 		int nFind = -1;
// 		for(POSITION pos=p_strData->GetHeadPosition(); pos; )
// 		{
// 			strData = p_strData->GetNext(pos);
// 			nFind = strData.Find("=");
// 			if(nFind>0)
// 			{
// 				//m_Form.SetLinkInfoTagData("", strData);
// 				CString szKey, szValue;
// 				szKey = strData.Left(nFind);
// 				szValue = strData.Mid(nFind+1);
// 				m_Form.SetLinkInfoTagData(szKey, szValue);
// 			}
// 		}
// 	}
// 
// 	m_Form.CreateLinkScreen(strScreen, 0, 0);
// }

BOOL CTrMChartView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	//return CFormView::OnEraseBkgnd(pDC);
	return TRUE;
}

// 	void CTrMChartView::OnFormInitAfter()
// 	{
// 		if(m_pChartScreen)
// 			m_pChartScreen->SendMessage(UMSG_FORMINITAFTER_FROMFORM);
// 	}
// 
// 	void CTrMChartView::SetFrameMsgText(CString strMsg) 
// 	{
// 		// TODO: Add your message handler code here and/or call default
// 		m_Form.SetFrameMsgText(strMsg);
// 	}
// 
// 	long CTrMChartView::OnChangeChildFrameWindowHandle( WPARAM wParam, LPARAM lParam)
// 	{
// 		m_hChildFrame = (HWND)lParam;
// 		return 0L;
// 	}
// 
// 	HRESULT CTrMChartView::OnGetScreenType(WPARAM wParam, LPARAM lParam)
// 	{
// 		CString* pStr = (CString*)lParam;
// 		*pStr = m_lbScreenType.GetCaption();
// 
// 		return 0L;
// 	}
// 
// 	HRESULT CTrMChartView::OnGetMarketType(WPARAM wParam, LPARAM lParam)
// 	{
// 		CString* pStr = (CString*)lParam;
// 		*pStr = m_lbMarketType.GetCaption();
// 
// 		return 0L;
// 	}
// 
// 	HRESULT CTrMChartView::OnGetLinkInfoData(WPARAM wParam, LPARAM lParam)
// 	{
// 		CString* pstrCode = (CString*)lParam;
// 		VARIANT sOrderType, sCode, sAccNo, sOrderPrice, sOrderQty, sPriceTp, sDealTp, sOrderNo, sUser;
// 		m_Form.GetFormLinkInfoData(&sOrderType, &sCode, &sAccNo, &sOrderPrice, &sOrderQty, &sPriceTp, &sDealTp, &sOrderNo, &sUser);
// 		m_Form.ClearFormLinkInfo();
// 		*pstrCode = sCode.bstrVal;
// 		return 0L;
// 	}
// 
// 	HRESULT CTrMChartView::OnOpenScreen(WPARAM wParam, LPARAM lParam)
// 	{
// 		CString strScrTrCode = (LPCSTR)wParam;
// 		m_Form.CreateLinkScreenForNewScreen( strScrTrCode , 0 , 1); 
// 		return 0L;
// 	}
// 
// 	HRESULT CTrMChartView::OnOrderHoga(WPARAM wParam, LPARAM lParam)
// 	{
// 		CString strOrderType = (LPCSTR)wParam;
// 		CString strParam = (LPCSTR)lParam;
// 		CString strCode, strPrice, strUser;
// 		int nPos = strParam.Find(":");
// 		strCode = strParam.Left(nPos);
// 		strPrice = strParam.Mid(nPos + 1);
// 		strUser = "1003";
// 
// 		m_Form.SetFormLinkInfoData(FALSE, strOrderType, strCode, NULL, strPrice, NULL, NULL, NULL, NULL, strUser);
// 		m_Form.CreateLinkScreen("1003", FALSE, TRUE);
// 		return 0L;
// 	}
// 
// 	void CTrMChartView::SetCodeDragAndDrop(CString strCode)
// 	{
// 		if(m_pChartScreen)
// 			m_pChartScreen->SendMessage(UMSG_SENDSHAREDCODE, (WPARAM)(LPCSTR)strCode, (LPARAM)1);
// 	}

// 프레임 윈도우 핸들을 구한다.
CWnd* CTrMChartView::GetFrameWnd(CWnd*	pWnd)
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

// 	void CTrMChartView::OnFormGetPageSave(LPCTSTR strFileName)
// 	{
// 		if(m_pChartScreen)
// 			m_pChartScreen->SendMessage(UMSG_FORMGETPAGESAVE, (WPARAM)0, (LPARAM)strFileName);
// 	}
// 
// 	void CTrMChartView::OnFormSetPageSave(LPCTSTR strFileName)
// 	{
// 		if(m_pChartScreen)
// 			m_pChartScreen->SendMessage(UMSG_FORMSETPAGESAVE, (WPARAM)0, (LPARAM)strFileName);
// 	}
// 
// 	void CTrMChartView::OnColorChange()
// 	{
// 		if(m_pChartScreen)
// 			m_pChartScreen->SendMessage(RMSG_CHANGE_SKINCOLOR);
// 	}
// 
// 	LRESULT CTrMChartView::DoRmgWrapGetInfo(WPARAM wParam, LPARAM lParam)
// 	{
// 		int nKey = (int)wParam;
// 		switch(nKey)
// 		{
// 			case 10:
// 				return (LRESULT)GetControlHwnd((LPCSTR) lParam);
// 			default:
// 				break;
// 		}
// 		return 0;
// 	}
// 
// 	HWND CTrMChartView::GetControlHwnd(LPCSTR szCtrlName)
// 	{
// 		return (HWND)m_Form.GetControlHwnd(szCtrlName);
// 	}
// 
// 	void CTrMChartView::ChangeCodeInputStyle(CString strCode)
// 	{
// 		IMasterDataManager080331* pDataManager = (IMasterDataManager080331*)AfxGetPctrInterface(UUID_IMasterDataManager);
// 		BYTE ucMarketType = pDataManager->GetMarketTypeFromCode(strCode);
// 		if( strCode == DEF_CONTINUE_KPFUT_CODE ||
// 			strCode == DEF_CONTINUE_KQFUT_CODE )
// 			ucMarketType = 24;				// 연결선물
// 
// 		CString sUS = US;
// 		CString strCatalog;
// 		int nMarketType = 0;
// 		if(		ucMarketType == 1	||		// 상장 종목
// 				ucMarketType == 2	||		// 코스닥 종목
// 				ucMarketType == 3	||		// 제3시장(OTCBB)
// 				ucMarketType == 4	||		// HIGHYIELD 펀드(수익증권)
// 				ucMarketType == 5	||		// 주식워런트 ELW
// 				ucMarketType == 6 )			// KDR
// 			nMarketType = 0;
// 		else if( ucMarketType == 26 ||		// 장내업종
// 				 ucMarketType == 27 ||		// KOSPI 200 업종
// 				 ucMarketType == 28 ||		// KOSPI 100 업종
// 				 ucMarketType == 33 ||		// 코스닥 업종
// 				 ucMarketType == 34 ||		// 코스닥50 업종
// 				 ucMarketType == 36	||		// 코스닥STAR 업종
// 				 ucMarketType == 59 ||		// 스타지수업종
// 				 ucMarketType == 60	||		// KRX
// 				 ucMarketType == 61	||		// KRX 섹터지수
// 				 ucMarketType == 62	)		// 프리보드업종
// 			nMarketType = 1;
// 		else if( ucMarketType == 21	||		// KOFEX 복합종목
// 				 ucMarketType == 24 ||		// 선물
// 				 ucMarketType == 25 ||		// 옵션
// 				 ucMarketType == 57 ||		// 스타선물지수
// 				 ucMarketType == 65 )		// 스프레드선물
// 			nMarketType = 2;
// 		else if( ucMarketType == 64 ||		// 주식선물
// 				 ucMarketType == 38 )		// 주식옵션
// 			nMarketType = 3;
// 		else if( ucMarketType == 42	||		// 미국지수
// 				 ucMarketType == 43	||		// 미국종가(미국종목)
// 				 ucMarketType == 44	||		// 미국상장 국내기업(ADR)
// 				 ucMarketType == 45	||		// 상품선물(원자재)
// 				 ucMarketType == 46	||		// CME선물
// 				 ucMarketType == 47	||		// 유럽상장 국내기업(GDR)
// 				 ucMarketType == 48	||		// 환율
// 				 ucMarketType == 49	||		// 금리
// 				 ucMarketType == 50	||		// 리보금리
// 				 ucMarketType == 51	||		// 주요국정부채
// 				 ucMarketType == 52	||		// 국내금리
// 				 ucMarketType == 53	||		// 반도체
// 				 ucMarketType == 54	||		// 세계주요종목
// 				 ucMarketType == 55	||		// ECN
// 				 ucMarketType == 56	)		// 세계주가지수(국가별)
// 			nMarketType = 4;
// 		
// 		// 빈번하게 발생하는 것을 줄이기 위함
// 		if(m_nCurMarketType != nMarketType)
// 			m_nCurMarketType = nMarketType;
// 		else
// 			return;
// 
// 		switch(nMarketType)
// 		{
// 		case 0:	// 주식
// 			strCatalog.Format("0101,0102,0103,0104%c1101", US);
// 			m_cmbJCodeDisp.SetLinkInformation("");
// 			m_cmbJCodeDisp.ChangeCatalog_NoSetCode(strCatalog);
// 			
// 			//yskwon 종목 툴바에서 elw 연동이 안되어 주식일 경우 주식과 elw 모두 연동하도록 수정
// 	//		if(ucMarketType == 5)		// 주식워런트 ELW
// 	//			m_cmbJCodeDisp.SetLinkInformation("# ELWCODE 0 1 1");
// 	//		else
// 				m_cmbJCodeDisp.SetLinkInformation(" # JCODE 0 1 1 # ELWCODE 0 1 1");
// 			
// 			// 2007.11.09 이준옥 종목코드 상태를 유지하기 위해서 아래한줄 막음.
// 			// m_cmbJCodeDisp.SetStockShape(3);
// 			break;
// 		case 1:	//업종
// 			strCatalog.Format("0201%c0100", US);
// 			m_cmbJCodeDisp.SetLinkInformation("");
// 			m_cmbJCodeDisp.ChangeCatalog_NoSetCode(strCatalog);
// 			
// 			if(ucMarketType == 62)		// 프리보드업종
// 				m_cmbJCodeDisp.SetLinkInformation(" # OTCBB 0 1 1");
// 			else
// 				m_cmbJCodeDisp.SetLinkInformation(" # UPCODE 0 1 1");
// 			
// 			break;			
// 		case 2: //선물 '옵션
// 			strCatalog.Format("0301,0304,0305,0307,0312%c0100", US);
// 			m_cmbJCodeDisp.SetLinkInformation("");
// 			m_cmbJCodeDisp.ChangeCatalog_NoSetCode(strCatalog);
// 			m_cmbJCodeDisp.SetLinkInformation(" # FOCODE 0 1 1");
// 			break;
// 		case 3:	//주식 선물/옵션
// 			strCatalog.Format("0316,0306,0312%c0100", US);
// 			m_cmbJCodeDisp.SetLinkInformation("");
// 			m_cmbJCodeDisp.ChangeCatalog_NoSetCode(strCatalog);
// 			m_cmbJCodeDisp.SetLinkInformation(" # FOCODE 0 1 1");
// 			break;
// 		case 4:	//해외지수
// 			strCatalog.Format("0501%c0100", US);
// 			m_cmbJCodeDisp.SetLinkInformation("");
// 			m_cmbJCodeDisp.ChangeCatalog_NoSetCode(strCatalog);
// 			m_cmbJCodeDisp.SetLinkInformation(" # FRSTO 0 1 1");
// 			break;
// 		}	
// 	}
// 
// 	void CTrMChartView::GetLastData(LPCTSTR strPageScrNo, LPCTSTR strPageIndex, CString& strData)
// 	{
// 		if(m_pChartScreen)
// 		{
// 			CString strFileName;
// 			strFileName.Format("%s%s", strPageScrNo, strPageIndex);
// 			m_pChartScreen->SendMessage(UMSG_FORMGETPAGESAVE, (WPARAM)0, (LPARAM)(LPCSTR)strFileName);
// 		}
// 	}
// 
// 	void CTrMChartView::SetLastData(LPCTSTR strPageScrNo, LPCTSTR strPageIndex, LPCTSTR strData)
// 	{
// 		if(m_pChartScreen)
// 		{
// 			CString strFileName;
// 			strFileName.Format("%s%s", strPageScrNo, strPageIndex);
// 			m_pChartScreen->SendMessage(UMSG_FORMSETPAGESAVE, (WPARAM)0, (LPARAM)(LPCSTR)strFileName);
// 		}
// 	}


long CTrMChartView::OnRmsgGetInterface(WPARAM wp, LPARAM lp)
{
	int nID = (int)wp;
	if(nID==0) return (long)&m_wxGwnMng;

	return 0;
}

long CTrMChartView::OnRmsgWinixPlatform(WPARAM wp, LPARAM lp)
{
	WORD dlKey = LOWORD(wp);
	WORD dhKey = HIWORD(wp);
	if(dlKey==rwpComm)
	{
		stWINIXComm* pItem = (stWINIXComm*)lp;
		return (long)_DoToCommList((BOOL)dhKey, pItem);
	}

	return 0L;
}

BOOL CTrMChartView::_DoToCommList(BOOL bAdvise, stWINIXComm* pItem)
{
	if(bAdvise)
	{
		stWINIXComm* pNItem = new stWINIXComm;
		Copy_stWINIXComm(pItem, pNItem);
		m_CommList.AddTail(pNItem);
	}
	else
	{
		POSITION tpos;
		for(POSITION pos=m_CommList.GetHeadPosition(); pos;)
		{
			tpos = pos;
			stWINIXComm* pOneItem = m_CommList.GetNext(pos);
			if(pOneItem && pOneItem->nKey==pItem->nKey)
			{
				m_CommList.RemoveAt(tpos);
				delete pOneItem;
				return TRUE;
			}
		}
	}
	return FALSE;
}

stWINIXComm* CTrMChartView::GetSendComm(int nKey)
{
	for(POSITION pos=m_CommList.GetHeadPosition(); pos;)
	{
		stWINIXComm* pItem = m_CommList.GetNext(pos);
		if(pItem && pItem->nKey==nKey)
			return pItem;
	}
	return NULL;
}

void CTrMChartView::_DoClearCommList()
{
	for(POSITION pos=m_CommList.GetHeadPosition(); pos;)
	{
		stWINIXComm* pItem = m_CommList.GetNext(pos);
		if(pItem)delete pItem;
	}
	m_CommList.RemoveAll();
}