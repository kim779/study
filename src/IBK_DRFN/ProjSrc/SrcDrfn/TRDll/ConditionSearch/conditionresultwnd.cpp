// ConditionResultWnd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ConditionResultWnd.h"

#include "Winsock2.h"
#include "MainFrmWnd.h"
#include "FieldEditDlg.h"
#include "DlgNoItem.h"

#include "AddControl/Grid_Result.h"
#include "../../DrCommon/Dr_Control/DrXLSFile.h"

#include "../Dr_Include/CompanySS.h"

#include "../Dr_Include/EU_p0621.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//# 2005.03.30
const		LPCTSTR		ROLLSEARCH_TEXT[] = { /*"10초",*/ "30초", "1분" };
const		UINT			ROLLSEARCH_TIME[] = { /*10,*/ 30, 60 };

#define GRID_COLUMN_UNIT				8

//ConditionResultWnd Grid Column Index Define
#define COL_CODE				0
#define COL_NAME				1

#define CODE_LENGTH				6
#define REQ_LENGTH				55
#define BETWEEN_CODE_NAME		13
#define	WMU_JANGO_CTRLDATADISP	(WM_USER + 1010)

extern HINSTANCE g_hInstance;

/////////////////////////////////////////////////////////////////////////////
// CConditionResultWnd

CConditionResultWnd::CConditionResultWnd()
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, FW_BOLD, m_fontBold);
	m_brushBk.CreateSolidBrush(COLOR_MAIN_CANVAS);
	
	m_pwndMain = NULL;
	m_bCreateEnd = FALSE;
	
	m_nTotCodeCnt = 0;
	
	m_strSection = "";
	
	m_bCodeViewFlag = FALSE;
	m_pGridCtrl = NULL;
	m_pdlgFieldEdit = NULL;

	m_nLastTrIndex = 0;
	
	m_nSelRadioMonth = 0;
	m_nIndexMonthCombo = 0;

	m_pDlgSearchRng = NULL;

//	m_saMarket.Add(_T("0000"));
	
	m_bJongFillterChk = FALSE;

	m_pCommMsg = NULL;

	m_nPreReceiveLen = -1;
	m_pDlgEditSubject = NULL; // 0024816: [7127] 조건검색 화면 - 특정 고객 프로그램 다운되는 현상 (비정상 종료)
}

CConditionResultWnd::~CConditionResultWnd()
{
	m_fontDefault.DeleteObject();
	m_fontBold.DeleteObject();
	
	m_brushBk.DeleteObject();
} 

BEGIN_MESSAGE_MAP(CConditionResultWnd, CWnd)
	//{{AFX_MSG_MAP(CConditionResultWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_COPYDATA()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
	ON_BN_CLICKED(IDC_BTN_ROLLSEARCH, OnBtnRollSearch)
	ON_BN_CLICKED(IDC_BTN_ROLLSEARCH + 1, OnBtnRollSearch)
	ON_BN_CLICKED(IDC_BTN_FIELDEDIT, OnBtnFieldEdit)
	ON_BN_CLICKED(IDC_BTN_RESULTCTRL, OnBtnResultCtrl)
	ON_COMMAND_RANGE(ID_MENU_RESULTCTRL_START, ID_MENU_RESULTCTRL_END, OnMenuResultCtrl)
	ON_BN_CLICKED(IDC_BTN_RESULTEXPAND, OnBtnExpand)
	ON_MESSAGE(WM_NOTIFY_GRIDEVENT, OnNotify_GridEvent)
	ON_BN_CLICKED(IDC_BTN_REALSEARCH, OnBtnRealSearch)
	ON_MESSAGE(WMU_JANGO_CTRLDATADISP, OnReqJangoData)
	//ON_BN_CLICKED(IDC_BTN_LANGUAGE, OnBtnLanguage) >> 0029461: [7127] 조건검색 화면 - 언어편집기 버튼삭제요청

  //# 삼성증권
//	ON_MESSAGE(WM_RP_RECEIVED, OnGetData)	-hidden-
	ON_BN_CLICKED(IDC_BTN_PRINT, OnBtnPrint)
	ON_WM_SETTINGCHANGE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

int CConditionResultWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CWnd* pwndParent = GetParent();
	pwndParent = pwndParent->GetParent();   //# CConditionView
	m_pwndMain = pwndParent->GetParent();		//# CMainFrmWnd
	CMainFrmWnd *wndMainFrm = (CMainFrmWnd*)m_pwndMain;
		
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	m_btnResultGroup.Create(_T("종목검색결과  건"), 
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_GROUPBOX, 
		CRect(0,0,0,0), this, 
		IDC_RESULT_GROUPBOX);
	m_btnResultGroup.SetFont(&m_fontDefault);
	m_btnResultGroup.ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	m_btnResultGroup.ShowWindow(SW_HIDE);

	m_btnSearch.Create(_T("검  색"), CRect(0, 0, 0, 0), this, IDC_BTN_SEARCH);
	SetBtnImg(&m_btnSearch, 4, TRUE);
	
	m_ctrlSearchTime.Create(_T("자동검색"),
		WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE,// | WS_BORDER,
		CRect(0, 0, 0, 0), this);
	m_ctrlSearchTime.SetFont(&m_fontDefault);
	
	m_comboSearchTime.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(0, 0, 0, 0), this,
		IDC_COMBO_SEARCH_TIME);
	m_comboSearchTime.SetFont(&m_fontDefault);
	m_comboSearchTime.ResetContent();
	
	for (int nIndex = 0; nIndex < _countof(ROLLSEARCH_TEXT); nIndex++)
		m_comboSearchTime.AddString(ROLLSEARCH_TEXT[nIndex]);
	m_comboSearchTime.SetCurSel(0);
	
	m_btnRollSearch.Create("\n자동검색", CRect(0, 0, 20, 20), this, IDC_BTN_ROLLSEARCH);
	SetBtnImg(&m_btnRollSearch, "CS_RollSearchPlay");

	m_btnRollSearchStop.Create("\n멈춤", CRect(0, 0, 20, 20), this, IDC_BTN_ROLLSEARCH + 1);
	SetBtnImg(&m_btnRollSearchStop, "CS_RollSearchStop");
	m_btnRollSearchStop.ShowWindow(SW_HIDE);
	
	m_ctrlRealSearchBtn.Create(_T("실시간검색"), CRect(0, 0, 0, 0), this, IDC_BTN_REALSEARCH);
	SetBtnImg(&m_ctrlRealSearchBtn, 6);

	m_btnPrint.Create(_T("인쇄"), CRect(0, 0, 0, 0), this, IDC_BTN_PRINT);
	SetBtnImg(&m_btnPrint, 2);
	
	m_btnFieldEdit.Create(_T("항목편집"), CRect(0, 0, 0, 0), this, IDC_BTN_FIELDEDIT);
	SetBtnImg(&m_btnFieldEdit, 4);

	m_btnResultCtrl.Create(_T("종목전송"), CRect(0, 0, 0, 0), this, IDC_BTN_RESULTCTRL);
	SetBtnImg(&m_btnResultCtrl, 4);

	//m_btnLanguage.Create(_T("언어편집기"), CRect(0, 0, 0, 0), this, IDC_BTN_LANGUAGE); >> 0029461: [7127] 조건검색 화면 - 언어편집기 버튼삭제요청
	//SetBtnImg(&m_btnLanguage, 6);
	
	m_btnExpand.Create(_T(""), CRect(0, 0, 0, 0), this, IDC_BTN_RESULTEXPAND);
// 	if(wndMainFrm->m_nTopExp == 2)
// 	{
// 		wndMainFrm->SetBtnImg(&m_btnExpand, "UP_EX");
// 	}
// 	else
// 	{
// 		wndMainFrm->SetBtnImg(&m_btnExpand, "DOWN_EX");
// 	}
	
	m_pGridCtrl = new CGrid_Result;
	m_pGridCtrl->Create(NULL, 
								_T(""), 
								WS_CHILD | WS_BORDER | WS_VISIBLE,
								CRect(0, 0, 0, 0), 
								this, 
								NULL);
	m_pGridCtrl->SetFont(&m_fontDefault);
	m_pGridCtrl->OnSetup(wndMainFrm->m_bIndustry);
	
	
	m_ctrlConditionTitle.Create(_T(""),
		WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE,// | WS_BORDER,
		CRect(0, 0, 0, 0), this, 
		IDC_STATIC_CONDITIONTITLE);
	m_ctrlConditionTitle.SetFont(&m_fontDefault);

	m_stcStatusBar.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
	m_stcStatusBar.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	
	m_ctrlTime.Create(_T(" 검색시간 : XX:XX"), WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE,// | WS_BORDER,
		CRect(0, 0, 0, 0), this);
	m_ctrlTime.SetFont(&m_fontDefault);
	
	m_ctrlPass.Create(_T("진행상태"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,// | WS_BORDER,
		CRect(0, 0, 0, 0), this);
	m_ctrlPass.SetFont(&m_fontDefault);
	
	m_progressPass.Create(WS_CHILD | WS_VISIBLE | PBS_SMOOTH, CRect(0, 0, 0, 0), 
		this, IDC_PROGRESS_PASS);
	m_progressPass.SetFont(&m_fontDefault);
	m_progressPass.SetRange(0, 100);
	
	m_ctrlPassResult.Create(_T("XXX%"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,// | WS_BORDER,
		CRect(0, 0, 0, 0), this);
	m_ctrlPassResult.SetFont(&m_fontBold);
	
	m_ctrlCodeOKCnt_Text.Create(_T("조건만족 종목"), WS_CHILD | WS_VISIBLE | SS_CENTER |  SS_CENTERIMAGE,// | WS_BORDER,
		CRect(0, 0, 0, 0), this);
	m_ctrlCodeOKCnt_Text.SetFont(&m_fontDefault);
	
	m_ctrlCodeOKCnt.Create(_T("XXXX"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,// | WS_BORDER,
		CRect(0, 0, 0, 0), this);
	m_ctrlCodeOKCnt.SetFont(&m_fontBold);
	
	m_ctrlCodeTargetCnt_Text.Create(_T("검색대상 종목"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,// | WS_BORDER,
		CRect(0, 0, 0, 0), this);
	m_ctrlCodeTargetCnt_Text.SetFont(&m_fontDefault);
	
	m_ctrlCodeTargetCnt.Create(_T("XXXX"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,// | WS_BORDER,
		CRect(0, 0, 0, 0), this);
	m_ctrlCodeTargetCnt.SetFont(&m_fontBold);
	
	m_ctrlMsg.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlMsg.SetFont(&m_fontDefault);

	AfxSetResourceHandle(hInstSave);

	return 0;
}

void CConditionResultWnd::SetLoadCandle()
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	CString strFilePath = pwndMainFrm->GetFilePath("CANDLE");
	
	MSXML::IXMLDOMDocumentPtr pXmlDom ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	
	if (GetLoadXml(strFilePath, pXmlDom) == TRUE)
		GetLoadCandleData(pXmlDom->selectSingleNode("INDEX"), m_mapCandle);
}

void CConditionResultWnd::SetLoadMemberComp()
{
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	CString strAppPath;
	strAppPath.Format("%s", Variant(homeCC));
	
	CString strFileRoot, strLineData;
	strFileRoot = strAppPath + FOLDER_TABLE + "\\" + MEMBERCOMP_FILE;
	
	int nIndex, nFindPos, nStartPos, nCode;
	CString strResult, strName;
	
	CStdioFile fMemberComp(strFileRoot, CFile::modeRead);
	try {
		while (fMemberComp.ReadString(strLineData))
		{
			nIndex = 0;
			nStartPos = 0;
			nFindPos = 0;
			while (nFindPos > -1)
			{
				nFindPos = strLineData.Find("=", nStartPos);
				strResult = strLineData.Mid(nStartPos, nFindPos - nStartPos);
				nStartPos = nFindPos + 1;
				
				if (nIndex == 0)	//# code
					nCode = atoi(strResult);
				else if (nIndex == 1)		//# Member Comp
					strName = strResult;
				else if (nIndex == 2)
					m_mapMemberComp.SetAt(nCode, strName);
				nIndex++;
			}
		}
	}
	catch(...) 
	{ 
		CString strMsg = "시스템 파일을 읽어오는 중 에러가 발생하였습니다. 고객지원센터로 연락 바랍니다.";
		SetSendMainMsg(strMsg);
	}
	
	fMemberComp.Close();
}


BOOL CConditionResultWnd::GetLoadCandleData(MSXML::IXMLDOMNodePtr pNIndex,
											CMap <int, int, CString, LPCTSTR> &mapCandle)
{
	BOOL bResult = TRUE;
	
	MSXML::IXMLDOMNodeListPtr			pNS_FItem;
	MSXML::IXMLDOMNodePtr					pN_FItem;
	MSXML::IXMLDOMNamedNodeMapPtr	pNA_FItem;
	
	try
	{
		pNS_FItem  = pNIndex->selectNodes("FITEM");
		
		int nIndex, nLength;
		int nId;
		CString strCandleName;
		nLength = pNS_FItem->length;
		for (nIndex = 0; nIndex < nLength; nIndex++)
		{
			pN_FItem = pNS_FItem->item[nIndex];	
			
			MSXML::IXMLDOMNodePtr	pNAttributeTemp;
			pNA_FItem= pN_FItem->attributes;
			
			pNAttributeTemp = pNA_FItem->getNamedItem("ID");
			nId = atol(CString((PCHAR)pNAttributeTemp->text));
			
			pNAttributeTemp = pNA_FItem->getNamedItem("FIELD_NAME");
			strCandleName = CString((PCHAR)pNAttributeTemp->text);
			
			mapCandle.SetAt(nId, strCandleName);
		}
	}
	catch(_com_error e)
	{
		CString strMsg = "시스템 파일을 읽어오는 중 에러가 발생하였습니다. 고객지원센터로 연락 바랍니다.";
		SetSendMainMsg(strMsg);
		bResult = FALSE;
	}
	
	return bResult;
}

//솔로몬
//"piso0350"		//조건검색
//"so035010"		//영업일조회
//"so035020"		//종목리스트 조회

void CConditionResultWnd::SetInit()
{
//TR Monitor
	CMainFrmWnd *wndMainFrm = (CMainFrmWnd*)m_pwndMain;
	m_bIndustry = wndMainFrm->m_bIndustry;

	m_nSeparateForm = wndMainFrm->m_nSeparateForm;

	if (m_bIndustry == FALSE)
	{
		//위닉스의 플랫폼 특성상 TR을 동시에 요청하면 문제가 되므로 작은 데이터를 먼저요청한다.
		SetSearch_WorkDate();
		SetSearch_MSTCode();
	}
	else
	{
		CWnd* pWndParent = GetParent();
		pWndParent->SendMessage(WM_CONDITIONRESULT_WND, (WPARAM)NM_SET_WORKDATE, (LPARAM)NULL);
		pWndParent->SendMessage(WM_CONDITIONRESULT_WND, (WPARAM)NM_SET_MSTCODE, (LPARAM)NULL);
	}
	
	
	// 대상변경의 포트폴리오, 보유종목 >>
// 	linkDataManager.Initialize((LPTSTR)(LPCTSTR)CDirMgr::GetCommonDir());
// 
// 	iJangoMng.Load(CDirMgr::GetCommonDir());
// 	iJangoMng.InitJangoMngr();
	

	m_nRegJangoCount = 0;

	GetHaveItem();
	// <<

	m_bCreateEnd = TRUE;
	
	m_bSortUp = FALSE;
	m_nSortBaseCol = 100;
	
	m_nTrIndex = 0;
	m_nLastTrIndex = 0;
	
	m_nSearchType = -1;
	m_strSearchResult = "";
	
	m_pGridCtrl->SetInit();
	
	SetSearchResultState();
	SetGrid_ClearAll();

	SetLoadFixField();
	SetGrid_Head();
	SetGrid_Head(TRUE);
	SetLoadCandle();

	_DoGridWidthSet(0);	//0000207: 조건검색 실시간, 사용자의 Column 구성과 배치(크기)등의 최종상태 저장 및 복원 기능 추가

	m_pGridCtrl->SetMultiSelectMode(TRUE);
	//SetLoadMemberComp();
	
	CString strUserPath;
	CString strSavePath;
	strUserPath.Format("%s%s%s", Variant(homeCC), USRDIR, Variant(nameCC));
	
	CString stFileName;
	
	if (m_nSeparateForm == 0)
		stFileName = "7127_ConditionInfo.dat"; // 조건검색
	else
		stFileName = "8416_ConditionInfo.dat"; // 0201 ~ 0209 화면이라면
	strSavePath =  strUserPath + (CString)FOLDER_FINDDATA + "\\" + stFileName;
	
	if (!ReadTargetInfo(strSavePath))
	{
		if (m_nSeparateForm == 0)
			OnChangeSubjectInit();
		else
			OnSearchRngInit();
	}
	
	CString strType, strUserFolder, strUserFileName;
	if (LookUpSearchCenterModify(strType, strUserFolder, strUserFileName))
	{
		if (strType == "2")
			SearchUserItem(strUserFolder, strUserFileName);
	}
}

int CConditionResultWnd::GetProfileInt(CString strGKey, CString strKey, CString strSavePath)
{
	char szInt[64];
	::GetPrivateProfileString(strGKey, strKey, "0", szInt, sizeof(szInt), strSavePath);
	return atoi(szInt);
}

BOOL CConditionResultWnd::ReadTargetInfo(CString strSavePath)
{
	CFileFind ff;
	if (!ff.FindFile(strSavePath))
		return FALSE;

	CString strGKey = "Except_JM";
	char szTemp[1024];

	m_bIssuesForAdministration		= GetProfileInt(strGKey, "Issue", strSavePath);
	m_bDealStop						= GetProfileInt(strGKey, "CheckDealStop", strSavePath);
	m_bTidyDeal						= GetProfileInt(strGKey, "TidyDeal", strSavePath);
	m_bInsincerityNotice			= GetProfileInt(strGKey, "InsincerityNotice", strSavePath);
	m_bAttentionInvestment			= GetProfileInt(strGKey, "AttentionInvestment", strSavePath);
	m_bNoticeAttentionInvestment	= GetProfileInt(strGKey, "NoticeAttentionInvestment", strSavePath);
	m_bPreferenceStock				= GetProfileInt(strGKey, "CheckPreferenceStock", strSavePath);
	m_bDeposit						= GetProfileInt(strGKey, "CheckDeposit", strSavePath);
	m_bDangerInvestment				= GetProfileInt(strGKey, "DangerInvestment", strSavePath);
	m_bWarningInvestment			= GetProfileInt(strGKey, "WarningInvestment", strSavePath);
	m_bInvestAttention				= GetProfileInt(strGKey, "InvestAttention", strSavePath);
	m_bETF							= GetProfileInt(strGKey, "ETF", strSavePath);
//	m_bETN							= GetProfileInt(strGKey, "ETN", strSavePath);

	if (m_nSeparateForm == 0) // 0110 화면이라면
	{
		strGKey = "Month";
		m_nSelRadioMonth = GetProfileInt(strGKey, "SelRadioMonth", strSavePath);
		m_nIndexMonthCombo = GetProfileInt(strGKey, "IndexMonthCombo", strSavePath);
		
		strGKey = "Tree_Upjong";
		int Index = GetProfileInt(strGKey, "TargetMText_Count", strSavePath);
		for (int i = 0; i< Index; i++)
		{
			CString stTmp;
			stTmp.Format("TargetMText_%d", i);
			::GetPrivateProfileString(strGKey, stTmp, "", szTemp, sizeof(szTemp), strSavePath);	
			m_arTargetMText.Add(szTemp);
		}
		
		::GetPrivateProfileString(strGKey, "TargetItem", "0", szTemp, sizeof(szTemp), strSavePath);
		m_strEditSubjectSelectItemText.Format("%s", szTemp);
		
		::GetPrivateProfileString(strGKey, "TargetParnetItem", "0", szTemp, sizeof(szTemp), strSavePath);
		m_strEditSubjectSelectParentItemText.Format("%s", szTemp);
		
		::GetPrivateProfileString("Tree_JangGubun", "Index", "1000", szTemp, sizeof(szTemp), strSavePath);
		m_strJangGubun.Format("%s", szTemp);

		CString strText;
		if (m_strEditSubjectSelectParentItemText != "")
			strText.Format("%s - %s", m_strEditSubjectSelectParentItemText, m_strEditSubjectSelectItemText);
		else
			strText.Format("%s", m_strEditSubjectSelectItemText);
		
		m_arTargetMText.RemoveAll();
		m_arTargetMText.Add(strText);
		SetTargetEditIndicate();

		SetEditSubjectTarGetBit();
	}
	else
	{
		//적용범위 로딩
		m_bEquityCapital					= GetProfileInt("Apply_EquityCapital", "EquityCapital", strSavePath);
		m_bPriceRange						= GetProfileInt("Apply_PriceRange", "PriceRange", strSavePath);
		m_bAggregateValueOfListedStocks		= GetProfileInt("Apply_AggregateValueOfListedStocks", "AggregateValueOfListedStocks", strSavePath);
		m_bTradingVolume					= GetProfileInt("Apply_TradingVolume", "TradingVolume", strSavePath);
		m_bParValue							= GetProfileInt("Apply_ParValue", "ParValue", strSavePath);
		m_nEquityCapitalAbove				= GetProfileInt("Apply_EquityCapital", "Above", strSavePath);
		m_nEquityCapitalBelow				= GetProfileInt("Apply_EquityCapital", "Below", strSavePath);
		m_nPriceRangeAbove					= GetProfileInt("Apply_PriceRange", "Above", strSavePath);
		m_nPriceRangeBelow					= GetProfileInt("Apply_PriceRange", "Below", strSavePath);
		m_nAggregateValueOfListedStocksAbove= GetProfileInt("Apply_AggregateValueOfListedStocks", "Above", strSavePath);
		m_nAggregateValueOfListedStocksBelow= GetProfileInt("Apply_AggregateValueOfListedStocks", "Below", strSavePath);
		m_nTradingVolumeAbove				= GetProfileInt("Apply_TradingVolume", "Above", strSavePath);
		m_nTradingVolumeBelow				= GetProfileInt("Apply_TradingVolume", "Below", strSavePath);
		m_nParValueAbove					= GetProfileInt("Apply_ParValue", "Above", strSavePath);
		m_nParValueBelow					= GetProfileInt("Apply_ParValue", "Below", strSavePath);
	}

	return TRUE;
}

BOOL CConditionResultWnd::WriteTargetInfo(CString strSavePath)
{
	if (strSavePath.GetLength() <= 4)
		return FALSE;

	CString strTemp;
	strTemp.Format("%d", m_bIssuesForAdministration);
	::WritePrivateProfileString("Except_JM", "Issue",  strTemp, strSavePath);

	strTemp.Format("%d", m_bDealStop);
	::WritePrivateProfileString("Except_JM", "CheckDealStop",  strTemp, strSavePath);

	strTemp.Format("%d", m_bTidyDeal);
	::WritePrivateProfileString("Except_JM", "TidyDeal",  strTemp, strSavePath);

	strTemp.Format("%d", m_bInsincerityNotice);
	::WritePrivateProfileString("Except_JM", "InsincerityNotice",  strTemp, strSavePath);

	strTemp.Format("%d", m_bAttentionInvestment);
	::WritePrivateProfileString("Except_JM", "AttentionInvestment",  strTemp, strSavePath);

	strTemp.Format("%d", m_bNoticeAttentionInvestment);
	::WritePrivateProfileString("Except_JM", "NoticeAttentionInvestment",  strTemp, strSavePath);

	strTemp.Format("%d", m_bPreferenceStock);
	::WritePrivateProfileString("Except_JM", "CheckPreferenceStock",  strTemp, strSavePath);

	strTemp.Format("%d", m_bDeposit);
	::WritePrivateProfileString("Except_JM", "CheckDeposit",  strTemp, strSavePath);

	strTemp.Format("%d", m_bDangerInvestment);
	::WritePrivateProfileString("Except_JM", "DangerInvestment",  strTemp, strSavePath);

	strTemp.Format("%d", m_bWarningInvestment);
	::WritePrivateProfileString("Except_JM", "WarningInvestment",  strTemp, strSavePath);

	strTemp.Format("%d", m_bETF);
	::WritePrivateProfileString("Except_JM", "ETF",  strTemp, strSavePath);

//	strTemp.Format("%d", m_bETN);
//	::WritePrivateProfileString("Except_JM", "ETN",  strTemp, strSavePath);


	if (m_nSeparateForm == 0) // 0110 화면이라면
	{
		strTemp.Format("%d", m_nSelRadioMonth);
		::WritePrivateProfileString("Month", "SelRadioMonth",  strTemp, strSavePath);

		strTemp.Format("%d", m_nIndexMonthCombo);
		::WritePrivateProfileString("Month", "IndexMonthCombo",  strTemp, strSavePath);

		int nIndex =0;
		for (int i = 0; i < m_arTargetMText.GetSize(); i++)
		{
			strTemp.Format("TargetMText_%d", i);
			::WritePrivateProfileString("Tree_Upjong", strTemp, m_arTargetMText.GetAt(i), strSavePath);//최종으로 선택된 업종저장 
			nIndex++;
		}
		strTemp.Format("%d", nIndex);
		::WritePrivateProfileString("Tree_Upjong", "TargetMText_Count", strTemp, strSavePath);//최종으로 선택된 업종저장 

		::WritePrivateProfileString("Tree_Upjong", "TargetItem", m_strEditSubjectSelectItemText, strSavePath);
		::WritePrivateProfileString("Tree_Upjong", "TargetParnetItem", m_strEditSubjectSelectParentItemText, strSavePath);

		::WritePrivateProfileString("Tree_JangGubun", "Index", m_strJangGubun, strSavePath);
	}
	else
	{
		//적용범위 로딩
		strTemp.Format("%d", m_bEquityCapital);
		::WritePrivateProfileString("Apply_EquityCapital", "EquityCapital",  strTemp, strSavePath);

		strTemp.Format("%d", m_bPriceRange);
		::WritePrivateProfileString("Apply_PriceRange", "PriceRange",  strTemp, strSavePath);

		strTemp.Format("%d", m_bAggregateValueOfListedStocks);
		::WritePrivateProfileString("Apply_AggregateValueOfListedStocks", "AggregateValueOfListedStocks",  strTemp, strSavePath);

		strTemp.Format("%d", m_bTradingVolume);
		::WritePrivateProfileString("Apply_TradingVolume", "TradingVolume",  strTemp, strSavePath);

		strTemp.Format("%d", m_bParValue);
		::WritePrivateProfileString("Apply_ParValue", "ParValue",  strTemp, strSavePath);

		strTemp.Format("%d", m_nEquityCapitalAbove);
		::WritePrivateProfileString("Apply_EquityCapital", "Above",  strTemp, strSavePath);

		strTemp.Format("%d", m_nEquityCapitalBelow);
		::WritePrivateProfileString("Apply_EquityCapital", "Below",  strTemp, strSavePath);

		strTemp.Format("%d", m_nPriceRangeAbove);
		::WritePrivateProfileString("Apply_PriceRange", "Above",  strTemp, strSavePath);

		strTemp.Format("%d", m_nPriceRangeBelow);
		::WritePrivateProfileString("Apply_PriceRange", "Below",  strTemp, strSavePath);

		strTemp.Format("%d", m_nAggregateValueOfListedStocksAbove);
		::WritePrivateProfileString("Apply_AggregateValueOfListedStocks", "Above",  strTemp, strSavePath);

		strTemp.Format("%d", m_nAggregateValueOfListedStocksBelow);
		::WritePrivateProfileString("Apply_AggregateValueOfListedStocks", "Below",  strTemp, strSavePath);

		strTemp.Format("%d", m_nTradingVolumeAbove);
		::WritePrivateProfileString("Apply_TradingVolume", "Above",  strTemp, strSavePath);

		strTemp.Format("%d", m_nTradingVolumeBelow);
		::WritePrivateProfileString("Apply_TradingVolume", "Below",  strTemp, strSavePath);

		strTemp.Format("%d", m_nParValueAbove);
		::WritePrivateProfileString("Apply_ParValue", "Above",  strTemp, strSavePath);

		strTemp.Format("%d", m_nParValueBelow);
		::WritePrivateProfileString("Apply_ParValue", "Below",  strTemp, strSavePath);
	}

	return TRUE;
}

//KHD: 로딩한 업종데이터를 파싱하는 루틴
/*
void CConditionResultWnd::ParsingCommaUpJongData(int MarketType, CString stUpJong)
{
	m_saMarket.RemoveAll();

	int nLength = stUpJong.GetLength();
	int nIndex = 0;
	for (int i =0; i< nLength; i++)
	{
		nIndex = stUpJong.Find(',',nIndex);

		//20120216 이문수 >>
		if(nIndex < 0)
			break;
		//20120216 이문수 <<

		CString stTemp = stUpJong.Left(nIndex);
		stUpJong.Delete(0,nIndex);
		if(nIndex <=0 )// 처음이 ,이면 빠져나감
			break;
		stTemp.Remove(',');

		m_saMarket.Add(stTemp);
	}
}
*/

void CConditionResultWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CWnd::OnShowWindow(bShow, nStatus);
	
}

void CConditionResultWnd::OnDestroy() 
{
	if(m_pDlgSearchRng)
	{
		delete m_pDlgSearchRng;
		m_pDlgSearchRng = NULL;
	}

	m_btnSearch.DestroyWindow();
	m_btnRollSearch.DestroyWindow();
	m_btnRollSearchStop.DestroyWindow();
	m_btnResultCtrl.DestroyWindow();
	m_btnExpand.DestroyWindow();
	m_btnLanguage.DestroyWindow();
	
	if (m_pGridCtrl->GetSafeHwnd() != NULL)
	{
		_DoGridWidthSet(1);	//0000207: 조건검색 실시간, 사용자의 Column 구성과 배치(크기)등의 최종상태 저장 및 복원 기능 추가
		m_pGridCtrl->DestroyWindow();
		delete m_pGridCtrl;
	}
	
	if (m_pdlgFieldEdit != NULL)
	{
		delete m_pdlgFieldEdit;
		m_pdlgFieldEdit = NULL;
	}
	
	if (m_pCommMsg)
	{
		m_pCommMsg->DestroyWindow();
		delete m_pCommMsg;
		m_pCommMsg = NULL;
	}

	m_listFixHeadSys.RemoveAll();
	m_listFixHeadUser.RemoveAll();
	m_listField.RemoveAll();

	CString strAccount;
	if(m_saAccount.GetSize() > 0)
		strAccount = m_saAccount.GetAt(0);

	
	/*iJangoMng.UninitJangoMngr(strAccount, GetSafeHwnd());*/
	// felicusima <<

	POSITION pos;
	CString strItmeCode;
	ST_UPCODE *pstUpCode;
	for (pos = m_MapUpCode.GetStartPosition(); pos != NULL;)
	{
		m_MapUpCode.GetNextAssoc(pos, strItmeCode, (void*&)pstUpCode);
		
		delete pstUpCode;
		m_MapUpCode.RemoveKey(strItmeCode);
	}
	m_MapUpCode.RemoveAll();

	m_mapExeptItem.RemoveAll();

	CWnd::OnDestroy();
}

HBRUSH CConditionResultWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if ((pWnd->GetDlgCtrlID() == IDC_RESULT_GROUPBOX) || 
		(nCtlColor == CTLCOLOR_STATIC))
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_brushBk;
	}
	return hbr;
}

void CConditionResultWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (m_bCreateEnd == FALSE)
		return;
	
	CRect reClient, reTop, reBottom;
	GetClientRect(reClient);

	reClient.right -= 3;
	reClient.bottom -= 3;
	
	reTop = reClient;
	reBottom = reClient;
	
	reTop.top += 1;
	reTop.bottom = reTop.top + 20;
	OnSize_FunBar(&reTop);
	
	int nHeight_MsgBar = 16;
	
	BOOL bShow_Msg = FALSE;
	if (bShow_Msg == TRUE)
	{
		reBottom.top = reTop.bottom - 16;
		m_ctrlMsg.MoveWindow(reBottom);
		
		reBottom.bottom = reBottom.top - 3;
	}
	
	reBottom.top = reBottom.bottom - 24;
	OnSize_ResultBar(reBottom);
	
	/*reTop.left += 2;*/
//	reTop.right -= 4;
	reTop.top = reTop.bottom + 3;
	reTop.bottom = reBottom.top - 3;
	if (m_pGridCtrl->GetSafeHwnd() != NULL)
		m_pGridCtrl->MoveWindow(&reTop);
	
/*
	reTop.top = reTop.bottom + 4;
	reTop.bottom = reClient.bottom - (bShow_Msg == TRUE) ? 26 : 0 - nHeight_MsgBar;
	reTop.left += 4;
	reTop.right -= 4;
	if (m_pGridCtrl->GetSafeHwnd() != NULL)
		m_pGridCtrl->MoveWindow(&reTop);

	reTop.top = reTop.bottom + 3 + 3;
	reTop.bottom = reClient.bottom - (bShow_Msg == TRUE) ? nHeight_MsgBar : 0;
	OnSize_ResultBar(reTop);

	if (bShow_Msg == TRUE)
	{
		reTop.top = reTop.bottom;
		reTop.bottom = reClient.bottom;
		m_ctrlMsg.MoveWindow(reTop);
	}
*/
}

void CConditionResultWnd::OnSize_FunBar(CRect reClient)
{
	CRect reLeft, reRight, reTemp;
	
	/*reClient.left += 2;*/
//	reClient.right -= 4;
	
	reLeft = reClient;
	reRight = reClient;
	
	reLeft.right = reLeft.left + 60;
	m_btnSearch.MoveWindow(reLeft);

	reLeft.left = reLeft.right + 5;
	reLeft.right = reLeft.left + 50;
	m_ctrlSearchTime.MoveWindow(reLeft);
	
	reLeft.left = reLeft.right + 5;
	reLeft.right = reLeft.left + 50;
	reTemp = reLeft;
	reTemp.top = reLeft.top;
	reTemp.bottom = reTemp.top + 100;
	m_comboSearchTime.MoveWindow(reTemp);
	
	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + 20;
	m_btnRollSearch.MoveWindow(reLeft);
	m_btnRollSearchStop.MoveWindow(reLeft);

	reRight.left = reRight.right - 20;
	m_btnExpand.MoveWindow(reRight);
	
	reRight.right = reRight.left - 2;
	reRight.left = reRight.right - 60;
	m_btnResultCtrl.MoveWindow(reRight);

	reRight.right = reRight.left - 2;
	reRight.left = reRight.right - 60;
	m_btnFieldEdit.MoveWindow(reRight);

	reRight.right = reRight.left - 2;
	reRight.left = reRight.right - 36;
	m_btnPrint.MoveWindow(reRight);	

	reRight.right = reRight.left - 2;
	reRight.left = reRight.right - 84;
	m_ctrlRealSearchBtn.MoveWindow(reRight);

	reRight.right = reRight.left - 2;
	reRight.left = reRight.right - 84;
	m_btnLanguage.MoveWindow(reRight);
	
	if(m_nSeparateForm)
	{
		reRight.right = reRight.left - 20;
		reRight.left = reLeft.right - 100;
		m_ctrlConditionTitle.MoveWindow(reRight);
	}
	else
	{
		reRight.right = reRight.left - 5;
		reRight.left = reLeft.right + 5;
		m_ctrlConditionTitle.MoveWindow(reRight);
	}
}

void CConditionResultWnd::OnSize_ResultBar(CRect reClient)
{
	CRect reLeft, reRight;	

	m_stcStatusBar.MoveWindow(reClient);

	reClient.top += 4;
	reClient.bottom -= 4;
	
	reLeft = reClient;
	reLeft.left+= 2;
	reRight = reClient;
	
	reLeft.right = reLeft.left + 120;
	m_ctrlTime.MoveWindow(reLeft);
	
	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + 60;
	m_ctrlPass.MoveWindow(reLeft);
	
	/*
	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + 150;
	m_progressPass.MoveWindow(reLeft);
	
	  reLeft.left = reLeft.right + 2;
	  reLeft.right = reLeft.left + 50;
	  m_ctrlPassResult.MoveWindow(reLeft);
	*/
	
	reRight.right -= 2;
	reRight.left = reRight.right - 40;
	m_ctrlCodeTargetCnt.MoveWindow(reRight);
	
	reRight.right = reRight.left - 2;
	reRight.left = reRight.right - 85;
	m_ctrlCodeTargetCnt_Text.MoveWindow(reRight);
	
	reRight.right = reRight.left - 2;
	reRight.left = reRight.right - 40;
	m_ctrlCodeOKCnt.MoveWindow(reRight);
	
	reRight.right = reRight.left - 2;
	reRight.left = reRight.right - 85;
	m_ctrlCodeOKCnt_Text.MoveWindow(reRight);
	
	reRight.right = reRight.left - 2;
	reRight.left = reRight.right - 50;
	m_ctrlPassResult.MoveWindow(reRight);
	
	reLeft.left = reLeft.right + 2;
	reLeft.right = reRight.left - 2;
	reLeft.top += 2;
	reLeft.bottom -= 2;
	m_progressPass.MoveWindow(reLeft);
	
	
	
	/*
	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + 85;
	m_ctrlCodeOKCnt_Text.MoveWindow(reLeft);
	
	  reLeft.left = reLeft.right + 2;
	  reLeft.right = reLeft.left + 40;
	  m_ctrlCodeOKCnt.MoveWindow(reLeft);
	  
		reLeft.left = reLeft.right + 2;
		reLeft.right = reLeft.left + 85;
		m_ctrlCodeTargetCnt_Text.MoveWindow(reLeft);
		
		  reLeft.left = reLeft.right + 2;
		  reLeft.right = reLeft.left + 40;
		  m_ctrlCodeTargetCnt.MoveWindow(reLeft);
	*/
}

void CConditionResultWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	//20100827 이문수 주석처리>>
	//CRect reClient;
	//GetClientRect(reClient);
	//dc.FillSolidRect(reClient, m_clrMainSkin);
	//20100827 이문수 <<

	if(m_nSeparateForm)
	{
		m_ctrlSearchTime.ShowWindow(SW_HIDE);
		m_comboSearchTime.ShowWindow(SW_HIDE);
		m_btnRollSearch.ShowWindow(SW_HIDE);
		m_btnRollSearchStop.ShowWindow(SW_HIDE);
		m_btnLanguage.ShowWindow(SW_HIDE);
		m_ctrlRealSearchBtn.ShowWindow(SW_HIDE);
	}
}

void CConditionResultWnd::OnBtnSearch()
{
// 	if(m_nSeparateForm != 0)
// 	{
		CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
		if(((CConditionMakeWnd*)pwndMainFrm->GetWnd("MAKE"))->m_DataAdmin.GetConditionCnt() > 0)
			((CConditionMakeWnd*)pwndMainFrm->GetWnd("MAKE"))->m_ctrlMapLoad.EuBtnUpdate();
//	}

	m_strSection = "";
	
	//# Sort Info Insert!
	m_bSortUp = FALSE;
	m_nSortBaseCol = 100;
	
	m_progressPass.SetPos(0);
	m_ctrlPassResult.SetWindowText("0%");

	SetSearch();
}

void CConditionResultWnd::OnBtnRollSearch()
{
	BOOL bSend_EnableState = TRUE;
	
	int nIndex;
	nIndex = m_comboSearchTime.GetCurSel();
	if(nIndex<0) return;	// 선택된것이 없을 때는 return;
	
	m_nRollTime = ROLLSEARCH_TIME[nIndex] * 1000;
	
	BOOL bEnableState = FALSE;
	//# -1 : Init, 0 : Search, 1 : Run, 2 : Stop
	if (m_nSearchType == -1)
	{
		if (SetSearch() == TRUE)
		{
			m_nSearchType = 1;
			bEnableState = TRUE;
		}
		else
			bSend_EnableState = FALSE;
	}
	else if ((m_nSearchType == 0) || (m_nSearchType == 2))
	{
		// 20110412 박찬모 자동검색 시작 시 "검색" 버튼 비활성화
		m_pwndMain->GetParent()->SendMessage(RMSG_SET_ENABLE_CTRL, (WPARAM)(LPCTSTR)"btnSearch", (LPARAM)FALSE);
		
		if (SetSearch() == TRUE)		//# After Search => Condition Modify!
		{
			m_nSearchType = 1;
			bEnableState = TRUE;
		}
		else
			bSend_EnableState = FALSE;
	}
	else if (m_nSearchType == 1)
	{
		// 20110412 박찬모 자동검색 시작 시 "검색" 버튼 비활성화
		m_pwndMain->GetParent()->SendMessage(RMSG_SET_ENABLE_CTRL, (WPARAM)(LPCTSTR)"btnSearch", (LPARAM)TRUE);
		
		m_nSearchType = 2;
		bEnableState = FALSE;
	}
	
	if (bSend_EnableState == TRUE)
	{
		CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
		(pwndMainFrm->GetParent())->SendMessage(RMSG_SET_ENABLE_BTNINIT, (WPARAM)!bEnableState, NULL);		
		
		if (bEnableState == TRUE)
		{
			//SetBtnImg(&m_btnRollSearch, "CS_RollSearchStop");
			m_btnRollSearch.ShowWindow(SW_HIDE);
			m_btnRollSearchStop.ShowWindow(SW_SHOW);
		}
		else
		{
			//SetBtnImg(&m_btnRollSearch, "CS_RollSearchPlay");
			m_btnRollSearch.ShowWindow(SW_SHOW);
			m_btnRollSearchStop.ShowWindow(SW_HIDE);
		}
		
		SetControlEnable(bEnableState, TRUE);
		m_pwndMain->PostMessage(WM_CONDITIONRESULT_WND, (WPARAM)NM_ROLL_SEARCH, (LPARAM)bEnableState);
	}
	
}

void CConditionResultWnd::OnBtnRealSearch()
{
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	if(((CConditionMakeWnd*)pwndMainFrm->GetWnd("MAKE"))->m_DataAdmin.GetConditionCnt() > 0)
			((CConditionMakeWnd*)pwndMainFrm->GetWnd("MAKE"))->m_ctrlMapLoad.EuBtnUpdate();

	if(!((CConditionMakeWnd*)pwndMainFrm->GetWnd("MAKE"))->PrepareRunRealSearch())
		return;
	
	pwndMainFrm->OpenScreen("7128");
}

void CConditionResultWnd::SetLoadedFile(XMLTreeInfoFile *pstLoadedFile)
{
	if (pstLoadedFile != NULL)
	{
		m_pstLoadedFile = pstLoadedFile;

		CString strText = pstLoadedFile->strFileName;

		if(m_nSeparateForm == 1)
			strText.Replace('_', ' ');
			
		m_ctrlConditionTitle.SetWindowText(strText);
	}
	else
	{
		m_pstLoadedFile = NULL;
		m_ctrlConditionTitle.SetWindowText("");
	}
}

void CConditionResultWnd::SetLoadedFile(CString strFileName)
{
	m_pstLoadedFile = NULL;
	m_ctrlConditionTitle.SetWindowText(strFileName);
}

void CConditionResultWnd::OnBtnFieldEdit()
{
	SetFieldEdit();
}

void CConditionResultWnd::OnBtnPrint()
{
	SetResultPrint();
}

void CConditionResultWnd::OnBtnResultCtrl()
{
	CRect reClient;
	m_btnResultCtrl.GetWindowRect(reClient);
	
	CPoint ptMenu;
	ptMenu.x = reClient.left;
	ptMenu.y = reClient.bottom;
	SetPopupMenuResultCtrl(ptMenu);
}

void CConditionResultWnd::SetPopupMenuResultCtrl(CPoint ptMenu)
{
//	USE_SS_HTS();
	
	CMenu menu;
	menu.CreatePopupMenu();

	CMenu *pMenuChild = NULL;
	pMenuChild = new CMenu;
	pMenuChild->CreatePopupMenu();
	
	CString strScreenInfo;
	int nIndex, nMenuType, nMenuCnt;
	
	if (m_bIndustry == TRUE)
	{
		nMenuCnt = _countof(MENU_INDUSTRY_RESULTCTRL_NAME);
		for (nIndex = 0; nIndex < nMenuCnt; nIndex++)
		{
			if (MENU_INDUSTRY_RESULTCTRL_NAME[nIndex] == "")
				nMenuType = MF_SEPARATOR;
			else
				nMenuType = MF_STRING;
			
//--> HTS구분없앰. 사용하면 추후 다시
//			if(bIsHTS)
			{
				strScreenInfo = (CString)MENU_INDUSTRY_RESULTCTRL_SCREEN_NOMAX[nIndex];
				
				if (atoi(strScreenInfo) <= 0)
					strScreenInfo = "";
				
				if (strScreenInfo.IsEmpty() == FALSE)
				{
					strScreenInfo = strScreenInfo.Left(4);
					strScreenInfo = "[" + strScreenInfo + "] " + (CString)MENU_INDUSTRY_RESULTCTRL_NAME[nIndex];
				}
				else
					strScreenInfo = "          " + (CString)MENU_INDUSTRY_RESULTCTRL_NAME[nIndex];
			}
			menu.AppendMenu(nMenuType, MENU_INDUSTRY_RESULTCTRL_INDEX[nIndex], (LPCTSTR)strScreenInfo); 
		}
	}
	else if (m_bIndustry == FALSE)
	{
		nMenuCnt = _countof(MENU_RESULTCTRL_NAME);
		for (nIndex = 0; nIndex < nMenuCnt; nIndex++)
		{
			if (MENU_RESULTCTRL_NAME[nIndex] == "")
				nMenuType = MF_SEPARATOR;
			else
				nMenuType = MF_STRING;
			
//--> HTS구분없앰. 사용하면 추후 다시
//			if(bIsHTS)
			{
				strScreenInfo = (CString)MENU_RESULTCTRL_SCREEN_NOMAX[nIndex];
				
				if (atoi(strScreenInfo) <= 0)
					strScreenInfo = "";
				
				if (strScreenInfo.IsEmpty() == FALSE)
				{
					strScreenInfo = strScreenInfo.Left(4);
					strScreenInfo = "[" + strScreenInfo + "] " + (CString)MENU_RESULTCTRL_NAME[nIndex];
				}
				else
					strScreenInfo = "          " + (CString)MENU_RESULTCTRL_NAME[nIndex];
			}
			if(nIndex == 1)
			{
				menu.AppendMenu(MF_POPUP, (UINT)pMenuChild->GetSafeHmenu(), (LPCTSTR)strScreenInfo); 

				CStringArray arrKey, arrGroupName;
				GetData_PortGroup(arrKey, arrGroupName);

				int nMenuIndex = 11200;
				for(int i = 0; i < arrKey.GetSize(); i++)
					pMenuChild->AppendMenu(MF_STRING, nMenuIndex + atoi(arrKey.GetAt(i)), arrGroupName.GetAt(i));

				pMenuChild->AppendMenu(MF_SEPARATOR);

				pMenuChild->AppendMenu(MF_STRING, nMenuIndex, "새 관심그룹 추가");

			}
			else
				menu.AppendMenu(nMenuType, MENU_RESULTCTRL_INDEX[nIndex], (LPCTSTR)strScreenInfo); 
		}
	}
	
	GetCursorPos(&ptMenu);
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptMenu.x, ptMenu.y, this);
	
	pMenuChild = menu.GetSubMenu(0);
	if (pMenuChild)
	{
		pMenuChild->DestroyMenu();
		delete pMenuChild;
	}
	
	menu.DestroyMenu();
}

void CConditionResultWnd::OnMenuResultCtrl(UINT nIndex)
{
//	USE_SS_HTS();
	
	CString strCode;
	CString strName;
	int nMarkType;
	ShardData		stShardData;
	CString strPortGroup;
	strPortGroup.Format("%d", nIndex);

	if(nIndex >= 11200)
		nIndex = 11020;
	
	int nDataCnt;
	nDataCnt = m_pGridCtrl->GetNumberRows();
	
	if (nDataCnt <= 0)
	{
		SetSendMainMsg("전송할 데이터가 없음.");
		return;
	}
	
	int nItem;
	for (nItem = 0; nItem < _countof(MENU_RESULTCTRL_INDEX); nItem++)
	{
		if (m_bIndustry == TRUE)
		{
			if (nIndex == MENU_INDUSTRY_RESULTCTRL_INDEX[nItem])
			{
				break;
			}
		}
		else
		{
			if (nIndex == MENU_RESULTCTRL_INDEX[nItem])
				break;
		}
	}
	
	if (nIndex == (int)SCREEN_INDEX_EXCEL)
		SetResultSave();
	else if(nIndex == (int)SCREEN_INDEX_RUSULTPRINT)
		SetResultPrint();
	else
	{
		//[20110825alzioyes]0000135: 조건검색 결과 리스트에 체크박스 및 기능 추가
		BOOL bCodeCheck=TRUE;
		if(atoi(strPortGroup) >= 11200)
		{
			stShardData.saCode.Add(strPortGroup.Right(2));
			stShardData.saName.Add("");
			stShardData.iaMarkType.Add(0);
		}

		for (int nRow = 0; nRow < nDataCnt; nRow++)
		{
			/*m_pGridCtrl->GetRowCode(nRow, strCode, nMarkType);*/
			m_pGridCtrl->GetRowToCodeName(nRow, strCode, strName, nMarkType);
			bCodeCheck = m_pGridCtrl->IsRowCodeChecked(nRow);
			if (bCodeCheck && !strCode.IsEmpty())
			{
				stShardData.saCode.Add(strCode);
				stShardData.saName.Add(strName);
				stShardData.iaMarkType.Add(nMarkType);
			}
		}
	
		if (stShardData.saCode.GetSize() <= 0)
		{
			AfxMessageBox("전송할 데이터가 없습니다.");
			return;
		}
		
		if (m_bIndustry == TRUE)
		{
			stShardData.bOpen = MENU_INDUSTRY_RESULTCTRL_SCREEN_OPEN[nItem];
//--> HTS구분없앰. 사용하면 추후 다시
//			if(bIsHTS)
				stShardData.strScreenNo = MENU_INDUSTRY_RESULTCTRL_SCREEN_NOMAX[nItem];
//			else
//				stShardData.strScreenNo = MENU_INDUSTRY_RESULTCTRL_SCREEN_NOEMP[nItem];
//<--
			stShardData.strMapNo = MENU_INDUSTRY_RESULTCTRL_MAP_NO[nItem];
		}
		else
		{
			stShardData.bOpen = MENU_RESULTCTRL_SCREEN_OPEN[nItem];
//--> HTS구분없앰. 사용하면 추후 다시
//			if(bIsHTS)
				stShardData.strScreenNo = MENU_RESULTCTRL_SCREEN_NOMAX[nItem];
//			else
//				stShardData.strScreenNo = MENU_RESULTCTRL_SCREEN_NOEMP[nItem];
//<--
			stShardData.strMapNo = MENU_RESULTCTRL_MAP_NO[nItem];
		}
		
		m_pwndMain->SendMessage(WM_CONDITIONRESULT_WND, (WPARAM)NM_SENDDATA_SHARD, (LPARAM)&stShardData);
	}
}

//void CConditionResultWnd::OnNotify_GridEvent(WPARAM wParam, LPARAM lParam)
LRESULT CConditionResultWnd::OnNotify_GridEvent(WPARAM wParam, LPARAM lParam)
{
	if (NM_SENDDATA_SHARD == wParam)
	{
		ShardData* pstShardData = (ShardData*)lParam;
//		m_pwndMain->SendMessage(WM_CONDITIONRESULT_WND, (WPARAM)NM_SENDDATA_SHARD, (LPARAM)pstShardData);
		
		CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pwndMain;
		pMainWnd->PutLinkData("&STOCK_CODE", pstShardData->saCode.GetAt(0));
		
		m_pGridCtrl->SetFocus();		//바로 스크롤 안되던 현상 수정			
	}

	return TRUE;
}

void CConditionResultWnd::SetFieldEdit()
{	
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	CMainFrmWnd *wndMainFrm = (CMainFrmWnd*)m_pwndMain;
	int nMoveFieldCnt = wndMainFrm->GetMoveFieldCnt();

	if (m_pdlgFieldEdit == NULL)
		m_pdlgFieldEdit = new CFieldEditDlg(m_pwndMain, nMoveFieldCnt);
	else
		m_pdlgFieldEdit->m_nMoveFieldCnt = nMoveFieldCnt;
	
	if (m_pdlgFieldEdit->DoModal() == IDOK)
	{

	}

	AfxSetResourceHandle(hInstSave);
}

void CConditionResultWnd::SetFieldEditApply()
{
	SetSaveFixUserFieldData(&m_listFixHeadUser, m_strFixFieldVersion);

	//# Match Val Info UpDate!
	m_pwndMain->SendMessage(WM_CONDITIONRESULT_WND, (WPARAM)NM_UPDATE_FIXFIELD, (LPARAM)NULL);

	if (SetSearch() == FALSE)
	{
		SetGrid_Head();
		SetGrid_Head(TRUE);
	}
}

void CConditionResultWnd::SetResultSave()
{
	CFileFind FileFinder;
	
	BOOL bExist = FALSE;
	BOOL bSave = TRUE;
	
	int nReturn = 0;
	CString strMsg = "";
	CString strFileName, strFileType;
	CString strDefaultName = "임시저장";
	
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	static char BASED_CODE szFilter[] = "WorkSheet Files (*.xls)|*.xls|EZ-Finder Data Files (*.txt)|*.txt||";
	CFileDialog saveDlg(FALSE, _T("xls"), strDefaultName + _T(".xls"), NULL, szFilter, NULL);
	
	while (bSave)
	{
		if (saveDlg.DoModal() == IDOK)
		{
			strFileType = saveDlg.GetFileExt();
			strFileName = saveDlg.GetPathName();
			
			bExist = FileFinder.FindFile(strFileName);
			if (bExist == TRUE)
			{
				strMsg.Format("%s 은(는) 이미 있습니다.\n바꾸시겠습니까?", strFileName);
				if(m_nSeparateForm == 0)
					nReturn = SetMessageBox(strMsg, PROGRAM_TITLE, MB_YESNO | MB_ICONQUESTION);
				else
					nReturn = SetMessageBox(strMsg, PROGRAM_TITLE_BRIGHT, MB_YESNO | MB_ICONQUESTION);

				if (nReturn == IDYES)
				{
					if (!strFileName.IsEmpty())
					{
						if (!strFileType.CompareNoCase("xls"))				
							SetSaveGridToExcel(strFileName);
						else if (!strFileType.CompareNoCase("txt"))				
							SetSaveGridToText(strFileName);
					}
					bSave = FALSE;
					break;
				}
				else
					bSave = TRUE;
			}
			else
			{
				if(!strFileName.IsEmpty())
				{
					if (!strFileType.CompareNoCase("xls"))				
						SetSaveGridToExcel(strFileName);
					else if (!strFileType.CompareNoCase("txt"))				
						SetSaveGridToText(strFileName); //txt file save 
				}
				bSave = FALSE;
				break;
			}
		}
		else
		{
			bSave = FALSE;
			break;
		}
		
		if (bSave == FALSE)
			break;
	}

	AfxSetResourceHandle(hInstSave);
}

void CConditionResultWnd::SetResultPrint()
{
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	CPrintDialog printDlg(FALSE);
	if(printDlg.DoModal() != IDOK) return;
	
	CDC dc;
	dc.Attach(printDlg.GetPrinterDC());
	dc.m_bPrinting = TRUE;
	
	
	//인쇄될 도큐먼트에 관한 정보 설정
	CString strTitle;
	strTitle = "조건검색";
	DOCINFO di;
	::ZeroMemory(&di, sizeof(DOCINFO));
	di.cbSize = sizeof(di);
	di.lpszDocName = strTitle;
	
	
	//도큐먼트 인쇄 시작
	BOOL bPrintingOK = dc.StartDoc(&di);
	
	//CPrintInfo에 인쇄 관련 정보설정
	CPrintInfo Info;
	Info.m_rectDraw.SetRect(0, 0, dc.GetDeviceCaps(HORZRES),
		dc.GetDeviceCaps(VERTRES));
	
	
	//프린트시 셀 스타일
	m_pGridCtrl->SetBorderStyle4Print(UG_BDR_LTHIN | UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	
	//프린트시 셀 선색 선택
	m_pGridCtrl->SetCellBorderColor4Print(RGB(0, 0, 0));
	
	
	//top헤더 출력 셋팅
	m_pGridCtrl->PrintingSetOption(UG_PRINT_TOPHEADING, TRUE);
	
	
	//좌우측 여백.
	m_pGridCtrl->PrintingSetOption(UG_PRINT_LEFTMARGIN, 5);
	m_pGridCtrl->PrintingSetOption(UG_PRINT_RIGHTMARGIN, 5);
	
	
	//페이지 셋팅
	Info.SetMaxPage(
		m_pGridCtrl->PrintingInit(&dc, &printDlg, 0, 0, m_pGridCtrl->GetNumberCols()-1, m_pGridCtrl->GetNumberRows()-1)
		);
	
	
	//페이지별 프린트
	for(UINT page = Info.GetMinPage(); page <= Info.GetMaxPage() && bPrintingOK; page++)
	{
		dc.StartPage();
		Info.m_nCurPage = page;
		
		m_pGridCtrl->PrintingPage(&dc, Info.m_nCurPage);
		
		m_pGridCtrl->PrintingTail(&dc, &Info);
		
		bPrintingOK = (dc.EndPage() > 0);
	}
	
	
	if(bPrintingOK) dc.EndDoc();
	else			dc.AbortDoc();
	
	
	dc.Detach();

	AfxSetResourceHandle(hInstSave);
}

void CConditionResultWnd::SetSaveGridToExcel(CString strFileName)
{
	int nRow, nCol;
	CDrXLSFile xlsfile;
	
	CString strColName = "";
	CString strData = "";
	
	if (!xlsfile.OpenFile(strFileName))
	{
		CString strMsg = "지정한 파일이 다른 프로그램에서 사용중입니다";
		if(m_nSeparateForm == 0)
			SetMessageBox(strMsg, (CString)PROGRAM_TITLE, MB_OK);
		else
			SetMessageBox(strMsg, (CString)PROGRAM_TITLE_BRIGHT, MB_OK);
		return;
	}
	else
	{
		nRow = -1;

		BOOL bSelectBun = FALSE;
		int nExcelColNum = 0;
	
		POSITION pos = m_listFixHeadUser.FindIndex(0);
		HeadFixInfo stHeadFixInfo = m_listFixHeadUser.GetAt(pos);
		if(stHeadFixInfo.stHeadType.strHeadName == "분")
			bSelectBun = TRUE;
				
		for (nCol = 0; nCol < m_pGridCtrl->GetNumberCols(); nCol++)
		{
			if(nCol == 0 && !bSelectBun)
				continue;
			else if(nCol == 1)
				continue;
			else if(nCol == 4 && bSelectBun)
				continue;

			strData = m_pGridCtrl->GetCellData(nRow, nCol);
			xlsfile.SetTextAlign(EXCEL_ALIGN_CENTER);
			xlsfile.WriteLabel(0, nExcelColNum, strData);
			nExcelColNum++;
		}

		int nWriteRow;
		for (nRow = 0, nWriteRow=0; nRow < m_pGridCtrl->GetNumberRows(); nRow++)
		{
			//[20110825alzioyes]0000135: 조건검색 결과 리스트에 체크박스 및 기능 추가, 저장관련.
			if(!m_pGridCtrl->IsRowCodeChecked(nRow)) continue;

			nExcelColNum = 0;
			for (nCol = 0; nCol < m_pGridCtrl->GetNumberCols(); nCol++)
			{
				strData = m_pGridCtrl->GetCellData(nRow, nCol);
				strData.Replace("&&", "&");
				
				strData.TrimRight(" ");
				strData.TrimLeft(" ");

				if(nCol == 0 && !bSelectBun)
					continue;
				else if(nCol == 1)
					continue;
				else if(nCol == 4 && bSelectBun)
					continue;
				
				strColName.Format("%s", m_pGridCtrl->GetCellData(-1, nCol));
				if ((strColName.Compare("등락률") == 0) ||
					(strColName.Compare("전일대비") == 0))
				{
					COLORREF clrText, clrBack;
					m_pGridCtrl->GetCellColor(nRow, nCol, clrText, clrBack);
					
					// 20110622 박찬모 엑셀로 저장시 등락률값이 음수일때 '-'부호가 2번 겹치는 오류
					if (clrText == COLOR_BLUE && strColName.Compare("전일대비") == 0)
					/*if (clrText == COLOR_BLUE)*/
						strData = "-" + strData;
					else if (clrText == COLOR_RED)
						strData = "+" + strData;
				}
				
				if (strColName.Compare("종목코드") == 0)
					xlsfile.SetTextAlign(EXCEL_ALIGN_CENTER);
				else if (strColName.Compare("종목명") == 0)
					xlsfile.SetTextAlign(EXCEL_ALIGN_LEFT);
				else
					xlsfile.SetTextAlign(EXCEL_ALIGN_RIGHT);
				
				xlsfile.WriteLabel(nWriteRow + 1, nExcelColNum, (LPCSTR)strData);
				nExcelColNum++;
			}
			nWriteRow++;
		}
	}
	xlsfile.CloseFile();
}


void CConditionResultWnd::SetSaveGridToText(CString strFileName)
{
	CFile SaveFile(strFileName,CFile::modeCreate | CFile::modeWrite);
	
	int nRowCnt = m_pGridCtrl->GetNumberRows();
	
	CString strLineData, strCell;
	int nRow, nCol;

	int nWriteRow=0;
	for (nRow = 0, nWriteRow=0; nRow < nRowCnt; nRow++)
	{
		BOOL bSelectBun = FALSE;
		int nExcelColNum = 0;
		// 타이틀 저장용
		if (nRow == 0)
		{
			strCell = m_pGridCtrl->GetCellData(0, 3);
			strLineData = strCell;	// 종목명
			strLineData += " ";
			
			BOOL bSelectBun = FALSE;
			int nExcelColNum = 0;
			
			POSITION pos = m_listFixHeadUser.FindIndex(0);
			HeadFixInfo stHeadFixInfo = m_listFixHeadUser.GetAt(pos);
			if(stHeadFixInfo.stHeadType.strHeadName == "분")
				bSelectBun = TRUE;
			
			for (nCol = 0; nCol < m_pGridCtrl->GetNumberCols(); nCol++)
			{
				if(nCol == 0 && !bSelectBun)
					continue;
				else if(nCol == 1)
					continue;
				else if(nCol == 4 && bSelectBun)
					continue;
				
				strCell = m_pGridCtrl->GetCellData(0, nCol);
				strLineData += strCell;	// 각 Item 정보
				strLineData += " ";
			}
			
			strLineData += "\r\n";
			SaveFile.Write( strLineData, strLineData.GetLength() );			
		}

		//[20110825alzioyes]0000135: 조건검색 결과 리스트에 체크박스 및 기능 추가, 저장관련.
		if(!m_pGridCtrl->IsRowCodeChecked(nRow)) continue;
		
		strCell = m_pGridCtrl->GetCellData(nRow, 3);
		strLineData = strCell;	// 종목명
		strLineData += " ";
		
		for (nCol = 0; nCol < m_pGridCtrl->GetNumberCols();  nCol++)
		{
			if(nCol == 0 && !bSelectBun)
				continue;
			else if(nCol == 1)
				continue;
			else if(nCol == 4 && bSelectBun)
				continue;
			
			strCell = m_pGridCtrl->GetCellData(nRow, nCol);
			strLineData += strCell;	// 각 Item 정보
			strLineData += " ";
		}
		
		strLineData += "\r\n";
		SaveFile.Write(strLineData, strLineData.GetLength());
	}
	
	SaveFile.Close();
}

void CConditionResultWnd::OnBtnExpand()
{
	CMainFrmWnd *wndMainFrm = (CMainFrmWnd*)m_pwndMain;
	
	int nState = wndMainFrm->m_nTopExp;

	if(m_nSeparateForm)
	{
		switch (nState)
		{
		case 0 :
			nState++;
			SetBtnImg(&m_btnExpand, "UP_EX");
			break;
		case 1 :
			nState--;
			SetBtnImg(&m_btnExpand, "UP_EX");
			break;
		default :
			break;
		}		
	}
	else
	{
		switch (nState)
		{
		case 0 :
			nState++;
			SetBtnImg(&m_btnExpand, "DOWN_EX");
			break;
		case 1 :
			nState++;
			SetBtnImg(&m_btnExpand, "DOWN_EX");
			break;
		case 2 :
			nState = 0;
			SetBtnImg(&m_btnExpand, "UP_EX");
			break;
		default :
			break;
		}
	}

	m_pwndMain->SendMessage(WM_CONDITIONRESULT_WND, (WPARAM)NM_SCREEN_EXP, (LPARAM)nState);
	
	//# Set Cursor position
// 	int nX, nY;
// 	CRect rt;
// 	m_btnExpand.GetWindowRect(rt);
// 	nX = rt.left + rt.Width()/2;
// 	nY = rt.top + rt.Height()/2;
// 	
// 	SetCursorPos(nX, nY);
	
	Invalidate();
}

void CConditionResultWnd::SetGrid_Head(BOOL bView /*= FALSE*/)
{
	int nCol;
	POSITION pos;
	
	if (bView == TRUE)
	{
		m_pGridCtrl->SetHeader(&m_listField);
		m_pGridCtrl->m_plistField = &m_listField;
	}
	else if (bView == FALSE)
	{
		m_listField.RemoveAll();
		
		HeadFixInfo	stHeadFixInfo;
		for (nCol = 0; nCol < m_listFixHeadUser.GetCount(); nCol++)
		{
			pos = m_listFixHeadUser.FindIndex(nCol);
			stHeadFixInfo = m_listFixHeadUser.GetAt(pos);
			
			if (stHeadFixInfo.bView == TRUE)
				m_listField.AddTail(stHeadFixInfo.stHeadType);
			else
			{
				if (stHeadFixInfo.stHeadType.strHeadName == (CString)GRID_MOVEFIELD_POS_NAME)
				{
					HeadMoveInfo stHeadMoveInfo;
					for (int nMoveCol = 0; nMoveCol < m_listHeadMoveInfo.GetCount(); nMoveCol++)
					{
						pos = m_listHeadMoveInfo.FindIndex(nMoveCol);
						stHeadMoveInfo = m_listHeadMoveInfo.GetAt(pos);
						
						for (int nSubCol = 0; nSubCol < stHeadMoveInfo.nColCnt; nSubCol++)
						{
							stHeadMoveInfo.stHeadType[nSubCol].strHeadName += "(" + stHeadMoveInfo.strElement + ")";
							m_listField.AddTail(stHeadMoveInfo.stHeadType[nSubCol]);
						}
					}
				}
			}
		}
	}
}

void CConditionResultWnd::SetGrid_ClearAll()
{
	if (m_btnResultGroup.GetSafeHwnd() != NULL)
		m_btnResultGroup.SetWindowText("종목검색결과 0건");
	
	m_progressPass.SetPos(0);
	m_ctrlPassResult.SetWindowText("0%");
	
	m_ctrlCodeOKCnt.SetWindowText("0");
	
	CString strData;
	strData.Format("%d", m_nTotCodeCnt);
	m_ctrlCodeTargetCnt.SetWindowText(strData);
	
	if (m_pGridCtrl->GetNumberRows() > 0)
		m_pGridCtrl->ClearAll();
	
	if (m_nSearchType != 1)		//# -1 : Init, 0 : Search, 1 : Run, 2 : Stop
	{
		m_bSortUp = FALSE;
		m_nSortBaseCol = 100;
	}
	
	SetAllDelete_MoveHead();
}

void CConditionResultWnd::SetClearAll()
{
	SetGrid_ClearAll();
	/*m_pGridCtrl->SetHeader_Init(&m_listFixHeadUser);*/
	
	m_ctrlConditionTitle.SetWindowText("");
	
	m_bSortUp = FALSE;
	m_nSortBaseCol = 100;
	
	m_nSearchType = -1;
	m_strSearchResult = "";
}

BOOL CConditionResultWnd::SetSearch(BOOL bFirstSearch)
{
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	if (bFirstSearch == TRUE)
	{
		CString strMsg;
		strMsg = ((CConditionMakeWnd*)pwndMainFrm->GetWnd("MAKE"))->SetMakePacketData(m_stUserFinder, m_saLanguageName);
		if (strMsg.IsEmpty() == FALSE)
		{
			SetSendMainMsg(strMsg);
			
			if (m_nSearchType == 1)
				OnBtnRollSearch();
			
			return FALSE;
		}
		if(((CConditionMakeWnd*)pwndMainFrm->GetWnd("MAKE"))->m_btnPortCheck.GetCheck())
		{
			CStringArray saPortGroup;
			BOOL bAnd;	//0(or), 1(and) 
			if(((CConditionMakeWnd*)pwndMainFrm->GetWnd("MAKE"))->m_ctrlPortEdit.GetPortGroupList(saPortGroup, bAnd)>0)
			{
				if(m_saPortfolioFilterMap.GetCount()==0)
				{
					AfxMessageBox("현재의 관심그룹 조건으로는 만족하는 종목이 없습니다.");
					return FALSE;
				}
			}
		}
		SetGrid_Head();
		m_nSearchType = 0;
	}
	
	SetSearchResultState();
	m_pGridCtrl->ClearAll();

	//# Set Sort Info!
	m_stUserFinder.Sort_flag = (m_bSortUp == TRUE) ? '1' : '0';   //# 정렬 방향(0:오름, 1:내림)
	if ((int)m_stUserFinder.ResultIdxCnt > 0)
	{
		//# Match Val!
		CString strMatchLogic;
		m_stUserFinder.ResultIdxCnt = pwndMainFrm->GetMatchLogic(&m_listField, strMatchLogic);
		
		strMatchLogic = strMatchLogic + "!";
		memset(m_stUserFinder.ResultExp, ' ', EXPLEN);
		memcpy(m_stUserFinder.ResultExp, strMatchLogic, strMatchLogic.GetLength());
	}
	
	if (((int)COL_CODE * 100 == m_nSortBaseCol) || 
		((int)COL_NAME * 100 == m_nSortBaseCol))
		m_stUserFinder.Sort_Num = m_nSortBaseCol;
	else
	{
		if (m_pGridCtrl->GetNumberCols() < m_nSortBaseCol)
			m_nSortBaseCol = m_pGridCtrl->GetNumberCols() - (3 + 1);
		else
			m_stUserFinder.Sort_Num = m_nSortBaseCol;
	}
	
	m_strSearchResult.Format("%d/%d/", m_stUserFinder.BackDay, m_stUserFinder.BackTime);
	GetTrIndex(m_stUserFinder.TrIndex);
	
	CString strData("");
	CString strLanguage, strLengthLen;
	BOOL bLanguageOK = FALSE;
	strLanguage = "";
	strLengthLen = "0";
	if (m_saLanguageName.GetSize() > 0)
	{
		strData = pwndMainFrm->GetData_LanguageScript(m_saLanguageName.GetAt(0));
		if (strData.IsEmpty() == FALSE)
		{
			//# Language => Type + Script!
			strLanguage = m_saLanguageName.GetAt(1) + strData;
			//strLengthLen.Format("%d", strLanguage.GetLength());
			bLanguageOK = TRUE;

			m_pScriptBody = NULL;
			m_pScriptBody = new char[strLanguage.GetLength() + 1];
			memset(m_pScriptBody, 0x00, strlen(m_pScriptBody));
			memcpy(m_pScriptBody, (char*)(LPCTSTR)strLanguage, strLanguage.GetLength());

			m_pScriptIndex = m_pScriptBody;
		}
	}
	
	int nPacketSize;
	nPacketSize = SZ_User_Finder + 8 + strLanguage.GetLength();
//	nPacketSize = SZ_User_Finder + sizeof(int) + strLanguage.GetLength();

// 	if (strLanguage.GetLength() > (int)LANGUAGE_MAX)
// 	{
// 		SetSendMainMsg("입력 정보가 제한값을 초과 합니다.");
// 		return FALSE;
// 	
	
	int nState;
	char* lpData;
	User_Finder	stUser_Finder;
	if (bLanguageOK == TRUE)
	{
// 		memset(&stUser_Finder, 0x00, SZ_User_Finder);
// 		memcpy(&stUser_Finder, &m_stUserFinder, SZ_User_Finder);
// 		SetByteOrdering_In(&stUser_Finder);
// 		
// 		lpData = new char[nPacketSize + 1];
// 		memset(lpData, 0x20, nPacketSize + 1);
// 		memcpy(lpData, &stUser_Finder, SZ_User_Finder);
// 		
// 		int nLanguageLen;
// 		nLanguageLen = strLanguage.GetLength();
// 
// 
// //@유진삭제 사용안함.		nLanguageLen = ntohl(nLanguageLen);
// 	//	memcpy(lpData + SZ_User_Finder, &nLanguageLen, sizeof(int));  //20080531 김성조
// 		
// 		if (strLanguage.GetLength() > 0)
// 		{			//20080531 김성조
// 			CString strLen;
// 			strLen.Format("%-8d", nLanguageLen);
// 			memcpy(lpData + SZ_User_Finder, strLen, 8);
// 			
// 			memcpy(lpData + SZ_User_Finder + 8, strLanguage.GetBuffer(0), strLanguage.GetLength());
// 		}
		
		SetLanguage_SendTr();
		return TRUE;
	}
	else
	{
		//# Byte Ordering!
		memset(&stUser_Finder, 0x00, SZ_User_Finder);
		memcpy(&stUser_Finder, &m_stUserFinder, SZ_User_Finder);
		SetByteOrdering_In(&stUser_Finder);
		
		lpData = new char[SZ_User_Finder];
		memset(lpData, 0x00, SZ_User_Finder);
		memcpy(lpData, &stUser_Finder, SZ_User_Finder);
		
		nState = Modual_SendTr((CString)TRNO_CONDITION, SZ_User_Finder, (BYTE*)lpData);
	}
	
	if (nState < 0)
		SetSendMainMsg("조회실패...");
	else
	{
		//SetSendMainMsg("조회중...");
		if(m_pCommMsg == NULL)
		{
			m_pCommMsg = new CRequestMsg;								// 통신조회 메세지바 생성
			m_pCommMsg->Create(CRequestMsg::IDD,this);
		}
		CString		strTemp = "자료를 조회중입니다...";
		m_pCommMsg->ShowMessage((LPCTSTR)strTemp, (CWnd*)this);
	}
	
	if (lpData)
		delete[] lpData;
	
#ifdef _DEBUG
	m_nTickCnt = GetTickCount();
#endif

	return TRUE;
}

void CConditionResultWnd::SetLanguage_SendTr()
{
	Language stLanguage;
	memset(&stLanguage, 0x00, SZ_Language);
	
	char* pData;
	int nCount, nLanPacketSize, nState;
	CString strSize, strCount, strState;
	nLanPacketSize = SZ_Language;
	
	nCount = ((strlen(m_pScriptBody) + 1) / (int)LANGUAGE_MAX) + 1;
	m_nToTalLanSendCnt = nCount;
	m_nLanSendCnt = 1;
	
	strState = "F";
	strSize.Format("%-8d", strlen(m_pScriptBody) + 1);
	strCount.Format("%-4d", nCount);
	if(nCount > 1)
		strSize.Format("%-8d", (int)LANGUAGE_MAX);

	memcpy(&stLanguage.sState, strState, sizeof(stLanguage.sState));
	memcpy(&stLanguage.sCnt, strCount, 4);
	memcpy(&stLanguage.ScriptLength, strSize, 8);
	memcpy(&stLanguage.gsScript, m_pScriptBody, atoi(strSize));
	
	pData = new char[nLanPacketSize + 1];
	memset(pData, 0x20, nLanPacketSize + 1);
	memcpy(pData, &stLanguage, SZ_Language);

	nState = Modual_SendTr((CString)TRNO_LANGUAGE, nLanPacketSize + 1, (BYTE*)pData);

	if (nState < 0)
		SetSendMainMsg("조회실패...");
	else
	{
		//SetSendMainMsg("조회중...");
		if(m_pCommMsg == NULL)
		{
			m_pCommMsg = new CRequestMsg;								// 통신조회 메세지바 생성
			m_pCommMsg->Create(CRequestMsg::IDD,this);
		}
		CString		strTemp = "자료를 조회중입니다...";
		m_pCommMsg->ShowMessage((LPCTSTR)strTemp, (CWnd*)this);
	}

	if (pData)
		delete[] pData;

	if(nCount > 1)
		m_pScriptIndex = m_pScriptIndex + (int)LANGUAGE_MAX;
}

void CConditionResultWnd::SetLanguage_ReceiveSendTr()
{
	m_nLanSendCnt++;

	Language stLanguage;
	memset(&stLanguage, 0x00, SZ_Language);
	
	char* pData;
	int nLanPacketSize, nState;
	CString strSize, strCount, strState;
	nLanPacketSize = SZ_Language;
	
	strState = "M";
	strSize.Format("%-8d", strlen(m_pScriptIndex) + 1);
	strCount.Format("%-4d", m_nLanSendCnt);
	if(m_nToTalLanSendCnt != m_nLanSendCnt)
		strSize.Format("%-8d", (int)LANGUAGE_MAX);
	
	memcpy(&stLanguage.sState, strState, sizeof(stLanguage.sState));
	memcpy(&stLanguage.sCnt, strCount, 4);
	memcpy(&stLanguage.ScriptLength, strSize, 8);
	memcpy(&stLanguage.gsScript, m_pScriptIndex, atoi(strSize) );
	
	pData = new char[nLanPacketSize + 1];
	memset(pData, 0x20, nLanPacketSize + 1);
	memcpy(pData, &stLanguage, SZ_Language);

	nState = Modual_SendTr((CString)TRNO_LANGUAGE, nLanPacketSize + 1, (BYTE*)pData);

	if (nState < 0)
		SetSendMainMsg("조회실패...");
	else
		SetSendMainMsg("조회중...");

	if (pData)
		delete[] pData;
	
	if(m_nToTalLanSendCnt != m_nLanSendCnt)
		m_pScriptIndex = m_pScriptIndex + (int)LANGUAGE_MAX;
}

//extern CString		Glb_SubTrCode; //ViewWrap.cpp 파일에 global로 정의되어있음.		-hidden-
/*
int CConditionResultWnd::Modual_SendTr(CString szTrNo, int nTrLen, BYTE *TrData)
{
	CString strScreenCode;
	strScreenCode.Format("%-10.10s", Glb_SubTrCode);

	//@유진삭제	CCommOptionInfo	option;
	//@유진삭제	option.m_strTrCode   = szTrNo;
	//@유진삭제	option.m_Destination = 'V';
	//@유진삭제	option.m_strScreenCode = strScreenCode;	
	//@유진삭제	int nState=0;
	//@유진삭제	nState = CommRequestDataOption(GetSafeHwnd(), (BYTE *)TrData , nTrLen, (BYTE *)&option);
	//@유진삭제	return nState;

	REQUEST_COMMINFO RqCommInfo;
	memset(&RqCommInfo, 0x0, RQ_COMM
		INFO_SIZE);

	memcpy(RqCommInfo.sServiceName, szTrNo, _tcslen(szTrNo));
	if ( _tcslen(strScreenCode) > 0)
		memcpy(RqCommInfo.sSCRN, strScreenCode, _tcslen(strScreenCode));

	RqCommInfo.TrHeader.nEncrypt	= 0;	 //TranInfo->nEncrypt;	// 암호화 여부.
	RqCommInfo.TrHeader.nPublic		= 0;	 //TranInfo->nPublic;	// 공개키 인증여부.
	RqCommInfo.TrHeader.nCompress	= 0;	 //TranInfo->nCompress;	// 압축여부...
	RqCommInfo.TrHeader.nTimeOut	= 30;	 //TranInfo->nTimeOut;
	RqCommInfo.TrHeader.nRqID		= 1;	//GetPacketUniqueID(m_hWnd); 화면별로 중복되지 않게 처리함면됨.
	RqCommInfo.cFIDCommGB			= AS_FID_TR;				// FID 통신구분		('0':TR통신,1:FID통신)


	// CommApi.dll에 통신 요청 하는 함수 
	CString strPacketUniqueID;
	CString strRQTrString;
	strPacketUniqueID.Format("%d", RqCommInfo.TrHeader.nRqID );
	//strRQTrString.Format("%s,%s", strRqName, strTrCode );
	strRQTrString.Format("%s,%s", "FID_Query000", szTrNo );
//	m_PacketID2TrCode.SetAt( strPacketUniqueID, strRQTrString );

	BYTE ucRet = CommRequestData(m_hWnd, &RqCommInfo, (BYTE*)TrData, nTrLen);
	if( ucRet )
	{
		TRACE("에러 발생 서버로 전송 실패\n");
		//m_PacketID2TrCode.RemoveKey(strPacketUniqueID);
	}

//	if ( pFIDOutputInfo)
//		m_mapFidRPMng.SetAt ( (WORD)RqCommInfo.TrHeader.nRqID , (CObject*&)pFIDOutputInfo);


	return 1;
}
*/
int CConditionResultWnd::Modual_SendTr(CString szTrNo, int nTrLen, BYTE *TrData)
{
//	int BufSz = sizeof(EU_p0621_InRec1) + ( sizeof(EU_p0621_InRec2) * MAX_IDXCNT);
	//char szSaveBuf[4096] = { 0 };
	char szSaveBuf[24576] = { 0 };
	int nPos = 0;
	CString strScreenCode = "";	//Glb_SubTrCode; -hidden-
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;

	if (szTrNo.CompareNoCase(TRNO_CONDITION)==0)
	{
		if(m_MapUpCode.GetCount() == 0)
		{
			CStringArray arrCode, arrName;
			
// 			IMasterDataManager080331 *pMaster = (IMasterDataManager080331*)AfxGetPctrInterface(UUID_IMasterDataManager080331, 1);
// 			if(pMaster)
// 			{
// 				pMaster->GetMasterDataInUpDetails("StockItemType", arrCode, arrName, "");
// 				MakeUpCodeList(&arrCode, &arrName);
// 			}
			
		}
		InMakeFid_P0621(szTrNo, nTrLen, TrData, szSaveBuf, nPos);

//		2016.09.19 강수현: 0014493: [7111] 조건을 20개 이상 추가하여 검색시 "유효한 조건이 아닙니다." 메시지 나오는 현상
// 		pwndMainFrm->SendTR(TRNO_CONDITION, szSaveBuf, nTrLen, TRKEY_CONDITION, US_KEY);
		pwndMainFrm->SendTR(TRNO_CONDITION, szSaveBuf, nPos, TRKEY_CONDITION, US_KEY);
	}
	else if(szTrNo.CompareNoCase(TRNO_MSTCODE)==0)
	{
		InMakeFid_Mstcode(szTrNo, nTrLen, TrData, szSaveBuf, nPos);

		pwndMainFrm->SendTR(TRNO_MSTCODE, szSaveBuf, nTrLen, TRKEY_MSTCODE, US_KEY);
	}

	else if(szTrNo.CompareNoCase(TRNO_WORKDATE)==0)
	{
		pwndMainFrm->SendTR(TRNO_WORKDATE, (char*) TrData, nTrLen, TRKEY_WORKDATE, US_KEY);
	}

	else if(szTrNo.CompareNoCase(TRNO_LANGUAGE)==0)
	{
		pwndMainFrm->SendTR(TRNO_LANGUAGE, (char*) TrData, nTrLen, TRKEY_LANGUAGE, US_KEY);
	}

// 	int nSendLog = AfxGetApp()->GetProfileInt("STBasic", "조건검색_SendLog", 0);
// 	nSendLog = 1;
// 	if(nSendLog==1)
// 	{
// 		CString szLogFile;
// 		szLogFile.Format("c:/Logs/조건검색_%d", m_nLastTrIndex);
// 		FILE *fp = fopen(szLogFile, "aw");
// 		if(fp)
// 		{
// 			fwrite(szSaveBuf, nPos, 1, fp);
// 			fwrite("\n", 1, 1, fp);
// 			fclose(fp);
// 		}
// 
// 		/*FILE *fp = NULL;*/
// 		if((errno = fopen_s(&fp, szLogFile, "aw")) == 0)
// 		{
// 			fwrite(szSaveBuf, nPos, 1, fp);
// 			fwrite("\n", 1, 1, fp);
// 			fclose(fp);
// 		}
// 		
// 	}

//	m_pwndMain->GetParent()->SendMessage(RMSG_WAIT_CORSOR, (WPARAM)TRUE, 0);
	return 1;
}

long CConditionResultWnd::OnGetData(WPARAM wParam, LPARAM lParam)
{
	int nResult = -1;
	return nResult;
}

void CConditionResultWnd::SetByteOrdering_In(User_Finder *pstUserFinder)
{
//!유진삭제	
	return;	//유진에서는 문자열로 바꾸므로 ntohl로 변경할 필요가 없다.

	int nIndex;
	
	pstUserFinder->TrIndex = ntohl(pstUserFinder->TrIndex);
	pstUserFinder->nTargetType = ntohl(pstUserFinder->nTargetType);
	pstUserFinder->TargetIdxCnt = ntohl(pstUserFinder->TargetIdxCnt);
	pstUserFinder->TargetResultViewCnt = ntohl(pstUserFinder->TargetResultViewCnt);
	
	pstUserFinder->ComplexIdxCnt = ntohl(pstUserFinder->ComplexIdxCnt);
	pstUserFinder->ComplexResultViewCnt = ntohl(pstUserFinder->ComplexResultViewCnt);
	
	pstUserFinder->FieldEditerCnt = ntohl(pstUserFinder->FieldEditerCnt);
	for (nIndex = 0; nIndex < (int)REAL_IDXCNT; nIndex++)
		pstUserFinder->FieldList[nIndex] = ntohl(pstUserFinder->FieldList[nIndex]);
	
	pstUserFinder->ResultIdxCnt = ntohl(pstUserFinder->ResultIdxCnt);
	
	pstUserFinder->Sort_Num = ntohl(pstUserFinder->Sort_Num);
	
	pstUserFinder->BackDay = ntohl(pstUserFinder->BackDay);
	pstUserFinder->BackTime = ntohl(pstUserFinder->BackTime);
	
	for (nIndex = 0; nIndex < (int)MAX_IDXCNT; nIndex++)
	{
		pstUserFinder->stUser_Define[nIndex].IndexNo = ntohl(pstUserFinder->stUser_Define[nIndex].IndexNo);
		pstUserFinder->stUser_Define[nIndex].GroupNo = ntohl(pstUserFinder->stUser_Define[nIndex].GroupNo);
		pstUserFinder->stUser_Define[nIndex].GroupIdxCnt = ntohl(pstUserFinder->stUser_Define[nIndex].GroupIdxCnt);
		pstUserFinder->stUser_Define[nIndex].TargetBase = ntohl(pstUserFinder->stUser_Define[nIndex].TargetBase);
		pstUserFinder->stUser_Define[nIndex].Type = ntohl(pstUserFinder->stUser_Define[nIndex].Type);
		pstUserFinder->stUser_Define[nIndex].Type1 = ntohl(pstUserFinder->stUser_Define[nIndex].Type1);
		pstUserFinder->stUser_Define[nIndex].Type2 = ntohl(pstUserFinder->stUser_Define[nIndex].Type2);
		pstUserFinder->stUser_Define[nIndex].STerm = ntohl(pstUserFinder->stUser_Define[nIndex].STerm);
		pstUserFinder->stUser_Define[nIndex].ETerm = ntohl(pstUserFinder->stUser_Define[nIndex].ETerm);
		pstUserFinder->stUser_Define[nIndex].Rank = ntohl(pstUserFinder->stUser_Define[nIndex].Rank);
		pstUserFinder->stUser_Define[nIndex].Trend = ntohl(pstUserFinder->stUser_Define[nIndex].Trend);
		pstUserFinder->stUser_Define[nIndex].SignalCnt1 = ntohl(pstUserFinder->stUser_Define[nIndex].SignalCnt1);
		pstUserFinder->stUser_Define[nIndex].SignalCnt2 = ntohl(pstUserFinder->stUser_Define[nIndex].SignalCnt2);
		pstUserFinder->stUser_Define[nIndex].ResultViewNum = ntohl(pstUserFinder->stUser_Define[nIndex].ResultViewNum);
		
		for (int nIndex_Param = 0; nIndex_Param < (int)MAX_PARAM; nIndex_Param++)
			SetByteOrderRevDouble((char far *)&(pstUserFinder->stUser_Define[nIndex].Param[nIndex_Param]));
		
		for (int nIndex_Condi = 0; nIndex_Condi < (int)MAX_CONDI; nIndex_Condi++)
			SetByteOrderRevDouble((char far *)&(pstUserFinder->stUser_Define[nIndex].Condition[nIndex_Condi]));
	}
	
	//pstUserFinder->ScriptLength = ntohl(pstUserFinder->ScriptLength);
}

void CConditionResultWnd::SetByteOrderRevDouble(LPSTR pDouble)
{
//@유진삭제 사용안함.
	return;
	int nCount, nLength = 4;
	
	for (nCount = 0; nCount < nLength; nCount++)
	{
		unsigned char chTemp;
		chTemp = *(pDouble + nCount);
		*(pDouble + nCount) = *(pDouble + sizeof(double) - 1 - nCount);
		*(pDouble + sizeof(double) - 1 - nCount) = chTemp;
	}
}

void CConditionResultWnd::SetSearch_WorkDate()
{
	int nDataCnt = 42;
	
	char* lpData = new char[sizeof(int) + 1];
	
	::memset(lpData, 0x00, sizeof(int) + 1);
	sprintf(lpData, "%4.4d", nDataCnt);

	int nState = Modual_SendTr((CString)TRNO_WORKDATE, sizeof(int), (BYTE*)lpData);
	if (nState < 0)
	{
		SetSendMainMsg("조회실패...");
		
		CWnd* pWndParent = GetParent();
		pWndParent->SendMessage(WM_CONDITIONRESULT_WND, (WPARAM)NM_SET_WORKDATE, (LPARAM)NULL);
	}
	else
		SetSendMainMsg("조회중...");

	if (lpData)
		delete[] lpData;
}

//extern BOOL    dll_GetMasterData(CString szType, CStringArray &arrCode, CStringArray &arrName);	-hidden-
void CConditionResultWnd::SetSearch_MSTCode()
{
	if (m_bIndustry == TRUE)
	{
/*
		m_nTotCodeCnt = 0;
		CStringArray saCode, saName;
		saCode.RemoveAll();
		saName.RemoveAll();
		if (dll_GetMasterData("0201", saCode, saName) == TRUE)
			m_nTotCodeCnt = saCode.GetSize();
		
		saCode.RemoveAll();
		saName.RemoveAll();
		if (dll_GetMasterData("0203", saCode, saName) == TRUE)
			m_nTotCodeCnt += saCode.GetSize();
		return;
*/
	}
	
	int nType = 2;
	char* lpData = new char[sizeof(int) + 1];
//@유진삭제 사용안함.	nType = ntohl(nType);
	::memset(lpData, 0x00, sizeof(int) + 1);
	sprintf(lpData, "%4.4d", nType);
	
	int nState = Modual_SendTr((CString)TRNO_MSTCODE, sizeof(int), (BYTE*)lpData);
	if (nState < 0)
	{
		SetSendMainMsg("조회실패...");
		
		CWnd* pWndParent = GetParent();
		pWndParent->SendMessage(WM_CONDITIONRESULT_WND, (WPARAM)NM_SET_MSTCODE, (LPARAM)NULL);
	}
	else
		SetSendMainMsg("조회중...");
	
	if (lpData)
		delete[] lpData;
}

HRESULT CConditionResultWnd::ReceiveData(LPCSTR szTR, LPVOID aTRData, long dwTRDateLen , int nKey )
{
	HRESULT dwResult = 0;
	
	if(strcmp(szTR, TRNO_CONDITION) == 0)
	{		
		if (dwTRDateLen == 0)		//# 0 : err
		{
			m_pCommMsg->HideMessage();
		}
		else if (dwTRDateLen > 0)
		{
			if (SetReceiveData((LPTSTR)aTRData, dwTRDateLen) == FALSE)
			{
			}
			else
			{
				//SetSendMainMsg("조회완료");
				m_pCommMsg->HideMessage();
			}

			SetResultTemp();
		}
	}
	else if(strcmp(szTR, TRNO_WORKDATE) == 0)
	{
		if(4 != dwTRDateLen)
		{
			char	strTmp[8+1]={0,};
			CStringArray	saWorkDate;
			int nDataCnt;	//nData, 

			//nDataCnt = dwTRDateLen / 8;		// 8Byte * N개

			char* pTrData = (char*)aTRData;

			memcpy(strTmp, &pTrData[0], 4);
			nDataCnt = atoi(strTmp);
			
			for (int nIndex = 0; nIndex < nDataCnt; nIndex++)
			{
				memcpy(strTmp, &pTrData[nIndex*8+4], 8);strTmp[8]=0;
				saWorkDate.Add(strTmp);
			}

			m_pwndMain->SendMessage(WM_CONDITIONRESULT_WND, (WPARAM)NM_SET_WORKDATE, (LPARAM)&saWorkDate);
			saWorkDate.RemoveAll();
			
			SetSendMainMsg("조회완료");		
		}
	}
	else if(strcmp(szTR, TRNO_MSTCODE) == 0)
	{
		CStringArray saMSTCode;
		MSTCode stMSTCode;
		char *psData = (char*)aTRData;
		int nPos=0;

		// 받은 마스터 데이터의 크기가 작을 경우는 오류로 판단하고, 재조회.
		if (dwTRDateLen < MINLEN_MSTCODE_TR)
		{
			CString strFile, strDataDir;
			strDataDir.Format("%s%s\\%s", Variant(homeCC), FOLDER_DATA, gszCacheDir);
			strFile.Format("%s\\%s", strDataDir, "RequestToday.ini");
			::WritePrivateProfileString(gszDataKey_MstCode, "Today", "", strFile);
			strFile.Format("%s%s", strDataDir, "\\MstCode.dat");
			::DeleteFile(strFile);

			if (m_nPreReceiveLen == dwTRDateLen)
				AfxMessageBox(_T("\"MstCode.dat\" 파일을 정상적으로 생성할 수 없습니다.\n 화면을 다시 실행하여 주십시요."));
			else
			{
				m_nPreReceiveLen = dwTRDateLen;

				SetSearch_MSTCode();
			}
			return S_OK;
		}

		int nDataCnt = dwTRDateLen / sizeof(MSTCode);
		
		char szTotCodeCnt[5] = {0, };
		memcpy(&szTotCodeCnt, (char*)&psData[nPos], 4); //종목카운트 패킷 앞의 4바이트
		nDataCnt = atoi(szTotCodeCnt);

		CString strtest;
		strtest.Format("nDataCnt = %d\r\n", nDataCnt);
		/*DebugStr(strtest);*/


		CString strCode;
		char	buf[4];

		CString strData;

		for (int nIndex = 0; nIndex < nDataCnt; nIndex++)
		{
			strData = "";
			memset(&stMSTCode, 0x00, sizeof(MSTCode));
			memcpy(&stMSTCode, (char*)&psData[nPos] + 4, sizeof(MSTCode));
			nPos += sizeof(MSTCode);
			
			strCode.Format("%-7.7s", stMSTCode.sCode);
			strCode.TrimLeft();
			strCode.TrimRight();
			saMSTCode.Add(strCode);

			strtest.Format("%s\r\n", strCode);
			/*DebugStr(strtest);*/

			if(stMSTCode.sUnder[0] == '1')				strData += "1";
			else										strData += "0";		

			if(stMSTCode.sStop[0] == '1')				strData += "1";
			else										strData += "0";

			if(stMSTCode.sJrmm[0] == '1')				strData += "1";
			else										strData += "0";

			if(stMSTCode.sDishonest[0] == '1')			strData += "1";
			else										strData += "0";

			memset(buf, 0x00, sizeof(buf));
			memcpy(buf, stMSTCode.sMrktalrm, sizeof(stMSTCode.sMrktalrm));
			switch(atoi(buf))
			{
			case 1 : m_mapExeptItem.SetAt(strCode, STOCK_ATTENTIONINVESTMENT);			strData += "1";		break;
			case 2 : m_mapExeptItem.SetAt(strCode, STOCK_WARNINGINVESTMENT);			strData += "2";		break;
			case 3 : m_mapExeptItem.SetAt(strCode, STOCK_DANGERINVESTMENT);				strData += "3";		break;
			case 4 : m_mapExeptItem.SetAt(strCode, STOCK_NOTICEATTENTIONINVESTMENT);	strData += "4";		break;
			default: strData += "0";
			}
			
			if(stMSTCode.sUsjg[0] == '1')				strData += "1";
			else										strData += "0";

			memset(buf, 0x00, sizeof(buf));
			memcpy(buf, stMSTCode.sDrat, sizeof(stMSTCode.sDrat));
			if(atoi(buf) == 100)						strData += "1";
			else										strData += "0";

			if (stMSTCode.sETF[0] == '1')				strData += "1";
			else										strData += "0";
/*
			if (stMSTCode.sETN[0] == '1')				strData += "1";
			else										strData += "0";
*/
			if (atoi(strData) > 0)
				m_mapExeptItem.SetAt(strCode, strData);

			memset(buf, 0x00, sizeof(buf));
			memcpy(buf, stMSTCode.sMonth, sizeof(stMSTCode.sMonth));
			switch(atoi(buf))
			{
			case 1 : m_saMonth[0].Add(strCode); break;
			case 2 : m_saMonth[1].Add(strCode); break;
			case 3 : m_saMonth[2].Add(strCode); break;
			case 4 : m_saMonth[3].Add(strCode); break;
			case 5 : m_saMonth[4].Add(strCode); break;
			case 6 : m_saMonth[5].Add(strCode); break;
			case 7 : m_saMonth[6].Add(strCode); break;
			case 8 : m_saMonth[7].Add(strCode); break;
			case 9 : m_saMonth[8].Add(strCode); break;
			case 10 : m_saMonth[9].Add(strCode); break;
			case 11 : m_saMonth[10].Add(strCode); break;
			case 12 : m_saMonth[11].Add(strCode); break;
			}
		}

		//m_nTotCodeCnt = saMSTCode.GetSize();
		m_nTotCodeCnt = nDataCnt;
		strData.Format("%d", m_nTotCodeCnt);
		m_ctrlCodeTargetCnt.SetWindowText(strData);
		
		m_pwndMain->SendMessage(WM_CONDITIONRESULT_WND, (WPARAM)NM_SET_MSTCODE, (LPARAM)&saMSTCode);
		saMSTCode.RemoveAll();
		
		SetSendMainMsg("조회완료");

		if(!m_strEditSubjectSelectParentItemText.IsEmpty())
			SetEditSubjectTarGetBit();
	}
	else if(strcmp(szTR, TRNO_LANGUAGE) == 0)
	{
		if (dwTRDateLen > 0)
		{
			char* trdata = (char*)aTRData;
			if(atoi(trdata) == m_nToTalLanSendCnt)
			{
				int nState;
				char* lpData;
				User_Finder	stUser_Finder;

				memset(&stUser_Finder, 0x00, SZ_User_Finder);
				memcpy(&stUser_Finder, &m_stUserFinder, SZ_User_Finder);
				SetByteOrdering_In(&stUser_Finder);
				
				lpData = new char[SZ_User_Finder];
				memset(lpData, 0x00, SZ_User_Finder);
				memcpy(lpData, &stUser_Finder, SZ_User_Finder);
				
				nState = Modual_SendTr((CString)TRNO_CONDITION, SZ_User_Finder, (BYTE*)lpData);

				if(lpData)
					delete[] lpData;
				
				if(m_pScriptBody)
				{
					delete[] m_pScriptBody;
					m_pScriptBody = NULL;
				}
				
				if(m_pScriptIndex)
					m_pScriptIndex = NULL;
			}
 			else
 				SetLanguage_ReceiveSendTr();
		}
	}

	m_pwndMain->GetParent()->SendMessage(RMSG_WAIT_CORSOR, (WPARAM)FALSE, 0);

	return dwResult;
}

BOOL CConditionResultWnd::SetReceiveData(LPTSTR pData, DWORD dwDataLen)
{
	LONG lTotCnt = 0L;
	LONG lTime = 0L;
	LONG lFieldCnt = 0L;
	LONG lSize = 0L;
	CString strData;
	
	CString strTemp;
	strTemp.Format("\n# [ConditionSearch] SetReceiveData Size : %d", dwDataLen);
	TRACE(strTemp);

	if (dwDataLen == 0)
		return FALSE;
	
	D1021OUT1* pstD1021Out1 = (D1021OUT1*)pData;
	strTemp.Format("%4.4s", pstD1021Out1->giCallGubun);
	
	if (m_nLastTrIndex != atoi(strTemp))
		return FALSE;

	if ((m_nSearchType == 0) || (m_nSearchType == 1))
		SetGrid_Head(TRUE);	
	
	SetGrid_ClearAll();

	strTemp.Format("%4.4s", pstD1021Out1->giTotalJongCnt);
	lTotCnt = atol(strTemp);

	strTemp.Format("%2.2s", pstD1021Out1->giOutFieldCnt);
	lFieldCnt = atol(strTemp);
	
	strTemp.Format("%8.8s", pstD1021Out1->giFindTime);
	lTime = atol(strTemp);
	
	SetSearchResultState(lTotCnt, lTime);
	
	if (lTotCnt <= 0)
	{
		//주의 : 조회된 결과가 없을때 (-900)으로 넘어옴.
		/*SetSendMainMsg("조회 건수가 없습니다.");*/

		m_progressPass.SetPos(100);
		m_ctrlPassResult.SetWindowText("100%");
		
		m_pCommMsg->HideMessage();

		HINSTANCE hInstSave = AfxGetResourceHandle();
		AfxSetResourceHandle(g_hInstance);
		
		CDlgNoItem dlgNoItem(this);
		if(lTotCnt == -1) //조건오류
			dlgNoItem.m_nType = COND_INVALID;

		dlgNoItem.DoModal();

		AfxSetResourceHandle(hInstSave);
		
		return FALSE;
	}

	pData = &pData[SIZE_D1021OUT1];
	dwDataLen = dwDataLen - SIZE_D1021OUT1;
	
	strTemp.Format("%d/", lTotCnt);
	m_strSearchResult += strTemp;
	
	int nRunVal = 0;

	D1021OUT2* pstOutListPacket = (D1021OUT2*)pData;
	char buf[32] = {0, };
	int nRow = 0;

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	CMapStringToString* pFilterMap = &(((CConditionResultWnd*)pwndMainFrm->GetWnd("RESULT"))->m_saPortfolioFilterMap);
	if(pFilterMap->GetCount()==0) pFilterMap = NULL;
	CString rFileRet;

	int nTotalCnt = lTotCnt;
	for (int nIndex = 0; nIndex < lTotCnt; nIndex++)
	{
		BOOL bFilterResult = TRUE;
		
		if(m_nSeparateForm)	// 0200 ~ 0209 검색범위
			bFilterResult = FilterSearchRng(&pstOutListPacket[nIndex]);
		else				// 0110 대상변경
			bFilterResult = FilterEditSubject(&pstOutListPacket[nIndex]);
		

		char szCode[7]={0,};
		memcpy(szCode, pstOutListPacket[nIndex].gsCode, sizeof(szCode) - 1);
		if(bFilterResult && m_saDelItemCode.GetSize() != 0)  // 그리드에서 삭제버튼 클릭으로 제외된 종목
		{
			for(int i = 0; i < m_saDelItemCode.GetSize(); i++)
			{
				if(m_saDelItemCode.GetAt(i) == szCode)
					bFilterResult = FALSE;
			}
		}

		if(pFilterMap && !pFilterMap->Lookup(szCode, rFileRet))
			bFilterResult = FALSE;

		//>>[0000094]alzioyes:대상변경에서 포트폴리오 -> 관심그룹 선택시 검색결과가 나오지 않는 현상.
		//0000052: 조건검색 화면에서 관심그룹추가후 AND, OR 연산 적용이 안되는 현상
		//KHD : 관심루틴 : 20101110 
		//if(bFilterResult && m_saPortfolio.GetSize() != 0)
		//{
		//	bFilterResult = FALSE;
		//	char szCode[7];
		//	memset(szCode, 0x00, sizeof(szCode));
		//	memcpy(szCode, pstOutListPacket[nIndex].gsCode, sizeof(szCode) - 1);

		//	for(int i = 0; i < m_saPortfolio.GetSize(); i++)
		//	{
		//		//AfxMessageBox(m_saPortfolio.GetAt(i));
		//		
		//		CString str;
		//		str.Format("%s=%s",m_saPortfolio.GetAt(i),szCode);
		//		
		//		if(m_saPortfolio.GetAt(i) == szCode)
		//		{

		//			bFilterResult = TRUE;
		//			break;
		//		}
		//	}
		//}
		//End
		//<<[0000094]

		if(!bFilterResult)
		{
			nTotalCnt--;
			continue;
		}
		else
		{
			SetGrid_AddData(nRow, &pstOutListPacket[nIndex]);
			nRow++;
		}
		pData = &pData[lSize];
		dwDataLen = dwDataLen - lSize;

		nRunVal = (nRow * 100 / nTotalCnt);
		m_progressPass.SetPos(nRunVal);

		strData.Format("%d", nRunVal);
		m_ctrlPassResult.SetWindowText(strData + "%");

// 		strData.Format("%d", nTotalCnt);
// 		m_ctrlCodeOKCnt.SetWindowText(strData);
	}

	if(pFilterMap) pFilterMap->RemoveAll();

	// >> 대상변경 후 100% 로 계산되기 위해 모든 add 후 다시 한번 계산
	// 결과들 중 add 후에 필터링 되는 종목들 때문에 100%가 안됨
	if(nTotalCnt != 0)
		nRunVal = (nRow * 100 / nTotalCnt);
	else
		nRunVal = 0;

		
	m_progressPass.SetPos(nRunVal);

	strData.Format("%d", nRunVal);
	m_ctrlPassResult.SetWindowText(strData + "%");

	strData.Format("%d", nTotalCnt);
	m_ctrlCodeOKCnt.SetWindowText(strData);
	// <<

	m_pGridCtrl->SetNumberRows(nTotalCnt);
	m_pGridCtrl->SetLockColumns(m_nFixCol);
	CUGHScroll* pGridHScroll = m_pGridCtrl->m_CUGHScroll;
	pGridHScroll->SetScrollPos(0, FALSE);
	m_pGridCtrl->RedrawAll();
	
	if (m_nSearchType == 1)
		SetTimer(1, (UINT)m_nRollTime, NULL);
	if (m_nSearchType == 2)
		m_nSearchType = 1;
	
	return TRUE;
}

void CConditionResultWnd::SetByteOrdering_Out(int nFieldCnt, OutListPacket *pstOutListPacket)
{
//@유진삭제 사용안함.	pstOutListPacket->lOrderRank = ntohl(pstOutListPacket->lOrderRank);
//@유진삭제 사용안함.	
//@유진삭제 사용안함.	for (int nIndex = 0; nIndex < nFieldCnt; nIndex++)
//@유진삭제 사용안함.		SetByteOrderRevDouble((char far *)&(pstOutListPacket->dFieldData[nIndex]));
}

/*
void CConditionResultWnd::SetGrid_AddData(int nRow, OutListPacket *pstOutListPacket)
{
	char cDaebi;
	char	chCode[CODE_LENGTH + 1];
	char	chName[CODENAME_LENGTH + 1];
	
	cDaebi = pstOutListPacket->chSign;
	
	memset(chCode, 0x00, CODE_LENGTH + 1);
	memcpy(chCode, pstOutListPacket->chCode, CODE_LENGTH);
	
	memset(chName, 0x00, CODENAME_LENGTH + 1);
	memcpy(chName, pstOutListPacket->chName, CODENAME_LENGTH);
	
	int nCntColumn;
	nCntColumn = m_listField.GetCount();
	
	m_pGridCtrl->SetAddRow(nRow, pstOutListPacket->chSign, nCntColumn, 	
		pstOutListPacket->chMarket, (CString)chCode, (CString)chName,	pstOutListPacket->dFieldData);	
}
*/

void CConditionResultWnd::SetGrid_AddData(int nRow, D1021OUT2* pstOutListPacket)
{
	char chCode[CODE_LENGTH + 1];
	char chName[CODENAME_LENGTH + 1];

	memset(chCode, 0x00, CODE_LENGTH + 1);
	memcpy(chCode, pstOutListPacket->gsCode, CODE_LENGTH);
	
	memset(chName, 0x00, CODENAME_LENGTH + 1);
	memcpy(chName, pstOutListPacket->gsHname, CODENAME_LENGTH);

	CString strName = chName;

	int nCntColumn;
	nCntColumn = m_listField.GetCount();

	memset(chName, 0x00, CODENAME_LENGTH + 1);
	memcpy(chName, pstOutListPacket->sPrice, sizeof(pstOutListPacket->sPrice));
	int nPrice = atoi(chName);
	
	m_pGridCtrl->SetAddRow(nRow, *pstOutListPacket->gsDaebi, nCntColumn, 	
		*pstOutListPacket->gsMarket, (CString)chCode, strName,	pstOutListPacket->gdResultField, nPrice);	
}

CString CConditionResultWnd::GetFormatStr(long lData, int nUnitNum, BOOL bFormatFlag, CString strFormat)
{
	CString strResult = "";
	int nIndex, nLength, nLengthFormat, nDotPos;
	
	BOOL bMinus = FALSE;
	
	if (lData < 0)
		bMinus = TRUE;
	lData = abs(lData);
	
	CString strData, strDis, strTemp;
	if (nUnitNum > 1)
	{
		strTemp.Format("%d", nUnitNum);
		strTemp.Format("%d", strTemp.GetLength() - 1);
		strDis = "%." + strTemp + "f";
		
		strData.Format(strDis, lData / (float)nUnitNum);
	}
	else
	{
		strDis = "%ld";
		strData.Format(strDis, lData);
	}
	
	if (bFormatFlag == TRUE)
	{
		nLength = strData.GetLength() - 1;
		nLengthFormat = strFormat.GetLength() - 1;
		
		for (nIndex = 1; nIndex < (nLengthFormat - nLength) - 1; nIndex++)
			strData = "0" + strData;
		
		int nCaretIndex = 0;
		for (nIndex = 0; nIndex <= nLengthFormat; nIndex++)
		{
			if ((CString)strFormat[nIndex] == "X")
			{
				strResult += (CString)strData[nCaretIndex];
				nCaretIndex++;
			}
			else
				strResult += (CString)strFormat[nIndex];
		}
	}
	else
	{
		int nCheckIndex = 0;
		BOOL bDotFlag = FALSE;
		nDotPos = strData.Find(".");
		
		int nStartIndex;
		nLength = strData.GetLength();
		if (nDotPos > 0)
		{
			nStartIndex = nDotPos;
			bDotFlag = TRUE;
		}
		else
		{
			nStartIndex = nLength;
			nDotPos = 0;
		}
		
		int nCaretIndex = 0;
		CString strResultInt;
		
		for (nIndex = nStartIndex - 1; nIndex >= 0; nIndex--)
		{
			if (nCheckIndex == 3)
			{
				strResultInt += "," + (CString)strData[nIndex];
				nCheckIndex = 1;
			}
			else
			{
				strResultInt += (CString)strData[nIndex];
				nCheckIndex++;
			}
		}
		
		nLength = strResultInt.GetLength();
		for (nIndex = nLength - 1; nIndex >= 0; nIndex--)
			strResult += (CString)strResultInt[nIndex];
		
		if (nDotPos > 0)
			strResult = strResult + strData.Right(strData.GetLength() - nDotPos);
	}
	
	if (bMinus == TRUE)
		strResult = "-" + strResult;
	
	return strResult;
}

void CConditionResultWnd::SetControlEnable(BOOL bState, BOOL bRollSearch/* = FALSE*/)
{
	bState = !bState;
	
	m_btnSearch.EnableWindow(bState);
	m_comboSearchTime.EnableWindow(bState);
	
	m_btnPrint.EnableWindow(bState);
	
	m_btnFieldEdit.EnableWindow(bState);
	m_btnResultCtrl.EnableWindow(bState);
	m_btnLanguage.EnableWindow(bState);
	
	if (bRollSearch == FALSE)
	{
		m_btnRollSearch.EnableWindow(bState);
		m_btnRollSearchStop.EnableWindow(bState);
		m_btnResultCtrl.EnableWindow(bState);
		m_btnExpand.EnableWindow(bState);
	}
	
	m_btnSearch.Invalidate();
	m_comboSearchTime.Invalidate();
	m_btnRollSearch.Invalidate();
	m_btnRollSearchStop.Invalidate();
	
	m_btnPrint.Invalidate();
	
	m_btnFieldEdit.Invalidate();
	m_btnResultCtrl.Invalidate();
	m_btnLanguage.Invalidate();
	
	m_btnExpand.Invalidate();

	CWnd* pViewWnd = m_pwndMain->GetParent();

	CString strCtrlName = "btnStandardValue";
	m_pwndMain->GetParent()->SendMessage(RMSG_SET_ENABLE_CTRL, (WPARAM)(LPCTSTR)strCtrlName, (LPARAM)bState);
	strCtrlName = "btnChange";
	m_pwndMain->GetParent()->SendMessage(RMSG_SET_ENABLE_CTRL, (WPARAM)(LPCTSTR)strCtrlName, (LPARAM)bState);
}


void CConditionResultWnd::SetSearchResultState(LONG lCnt/* = 0*/, LONG lTime/* = -1*/)
{
	CString strResult;
	CString strTime, strTemp;
	strResult.Format("종목검색결과  %d건", lCnt);
	CString str;
	/*
	if (lCnt == 0)
	m_pGridCtrl->ClearData(TRUE);
	*/
	
	if (lTime == -1)
	{
		m_ctrlCodeOKCnt.SetWindowText("0");
		
		strTime = "";
	}
	else
	{
		int nLength;
		strTemp.Format("%d", lTime );
		nLength = strTemp.GetLength();
		
		if (nLength == 3)
		{
			strTime = "0" + strTemp.Left(1) + ":";
			strTime += strTemp.Right(2);
		}
		else if (nLength == 4)
		{
			strTime = strTemp.Left(2) + ":";
			strTime += strTemp.Right(2);
		}
		else if( nLength == 5)// KHD : 5자리가 올경우도 있어서 따로 작성 
		{
			strTime = strTemp.Left(1) + ":";
			strTime += strTemp.Mid(1, 2) + ":";
			strTime += strTemp.Right(2);

		}
		else if (nLength == 6)
		{
			strTime = strTemp.Left(2) + ":";
			strTime += strTemp.Mid(2, 2) + ":";
			strTime += strTemp.Right(2);
		}
		
		str.Format("%d", nLength);
		strResult += " [" + strTime + "]";
	}
	
	m_btnResultGroup.SetWindowText(strResult);
	m_ctrlTime.SetWindowText(" 검색시간 :" + strTime);
}

void CConditionResultWnd::SetSendMainMsg(CString strMsg)
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	pwndMainFrm->SetSendMainMsg(strMsg);
}

BOOL CConditionResultWnd::GetLoadXml(CString strFileRoot, MSXML::IXMLDOMDocumentPtr &pXmlDom)
{
	char msg[500];
	CString strDebug;
	
	pXmlDom->async = false;

	//20120112 이문수 >>
	CFileFind ff;
	if(!ff.FindFile(strFileRoot))
		return FALSE;
	else
	{
		strFileRoot.MakeUpper();
		if(strFileRoot.Find(".XML") < 0)//경로만 있는 경우도 로드실패로 본다
			return FALSE;
	}
	//20120112 이문수 <<

	if (!pXmlDom->load (strFileRoot.AllocSysString())) 
	{	
		MSXML::IXMLDOMParseErrorPtr pParsingErr = pXmlDom->parseError;			
		sprintf(msg, "Error 0x%.8X on line %d, position %d\r\n", 
			pParsingErr->errorCode,pParsingErr->line, pParsingErr->linepos);
		
		CString strTemp(BSTR(pParsingErr->reason));	
		strDebug = strTemp + CString(msg) + "\n" + strFileRoot;
		
		//SetMessageBox(strDebug, "[XML Files Error]", MB_OK);		
		return FALSE;
	}
	
	return TRUE;
}


void CConditionResultWnd::SetLoadFixField()
{
	MSXML::IXMLDOMDocumentPtr pXmlDom ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	CString strFilePath = pwndMainFrm->GetFilePath("FIX_SYS");
	if (GetLoadXml(strFilePath, pXmlDom) == TRUE)
	{
		GetLoadFixSysFieldData(pXmlDom->selectSingleNode("INDEX"), m_listFixHeadSys);
		m_strFixFieldVersion = GetVersion_FieldFile(pXmlDom->selectSingleNode("INDEX"));
	}
	
	BOOL bSetInit = TRUE;
	strFilePath = pwndMainFrm->GetFilePath("FIX_USER");
	if (GetLoadXml(strFilePath, pXmlDom) == TRUE)
	{
		//# Fix Field User Version Check!
		if (m_strFixFieldVersion == GetVersion_FieldFile(pXmlDom->selectSingleNode("INDEX")))//KHD: 기존것도 통과할 수 있게 수정
		{
			GetLoadFixUserFieldData(pXmlDom->selectSingleNode("INDEX"), m_listFixHeadUser);
			
			if (m_listFixHeadUser.GetCount() == 0)
				bSetInit = FALSE;
			else
				SetUpdateUserFieldData(&m_listFixHeadSys, &m_listFixHeadUser);		//# Info UpDate!
			
			bSetInit = FALSE;
		}
	}
	
	if (bSetInit == TRUE)
	{
		m_nFixCol = 2;
		m_listFixHeadUser.RemoveAll();
		
		CStringArray saFieldId;
		if (GetLoadFixField_Def(saFieldId) == TRUE)
		{
			POSITION pos;
			HeadFixInfo		stHeadFixInfo;
			HeadInfoType	stHeadInfoType;
			for (int nIndex = 0; nIndex < m_listFixHeadSys.GetCount(); nIndex++)
			{
				pos = m_listFixHeadSys.FindIndex(nIndex);
				stHeadInfoType = m_listFixHeadSys.GetAt(pos);
				
				CString strFieldId;
				for (int nItem = 0; nItem < saFieldId.GetSize(); nItem++)
				{
					strFieldId = saFieldId.GetAt(nItem);
					if (stHeadInfoType.lHeadID == atol(strFieldId))
					{
						stHeadFixInfo.stHeadType = stHeadInfoType;
						stHeadFixInfo.nColNum = m_listFixHeadUser.GetCount();
						stHeadFixInfo.bView = TRUE;
						
						m_listFixHeadUser.AddTail(stHeadFixInfo);
						break;
					}
				}
			}
			
			stHeadFixInfo.stHeadType = GetMoveFieldPosInfo();
			stHeadFixInfo.nColNum = m_listFixHeadUser.GetCount();
			stHeadFixInfo.bView = FALSE;
			
			m_listFixHeadUser.AddTail(stHeadFixInfo);
		}
	}
	
	SetSaveFixUserFieldData(&m_listFixHeadUser, m_strFixFieldVersion);
	m_pGridCtrl->SetLockColumns(m_nFixCol);	
}

CString CConditionResultWnd::GetVersion_FieldFile(MSXML::IXMLDOMNodePtr pNIndex)
{
	CString strResult = XML_VERSION_USERFILE;
	
	MSXML::IXMLDOMNamedNodeMapPtr	pNAIndex;
	try 
	{
		MSXML::IXMLDOMNodePtr	pNAttributeTemp;
		pNAIndex = pNIndex->attributes;
		
		pNAttributeTemp = pNAIndex->getNamedItem("VERSION");
		strResult = CString((PCHAR)pNAttributeTemp->text);
		//KHD : 20101111 : 기존에 저장 안되던 버젼 수정 
		if(strResult == "2.0")
			return XML_VERSION_USERFILE;
	}
	catch(_com_error e) 
	{
		strResult = "";
	}
	
	return strResult;
}

HeadInfoType CConditionResultWnd::GetMoveFieldPosInfo()
{
	HeadInfoType	stHeadInfoType;
	
	//# 값보기 위치!
	stHeadInfoType.lHeadID = (long)GRID_MOVEFIELD_POS_INDEX;
	stHeadInfoType.strHeadName = (CString)GRID_MOVEFIELD_POS_NAME;
	stHeadInfoType.nUnitNum = 0;
	stHeadInfoType.strUnitText = "";
	stHeadInfoType.strDataType = "";
	stHeadInfoType.nDataLength = 0;
	stHeadInfoType.nColWidth = 0;
	stHeadInfoType.strAlignment = "";
	
	return stHeadInfoType;
}

BOOL CConditionResultWnd::GetLoadFixSysFieldData(MSXML::IXMLDOMNodePtr pNIndex,
												 CList<HeadInfoType, HeadInfoType&> &listFixList)
{
	BOOL bResult = TRUE;
	
	MSXML::IXMLDOMNodeListPtr		pNS_FItem;	// Item List
	MSXML::IXMLDOMNodePtr				pN_FItem;	// Item
	MSXML::IXMLDOMNamedNodeMapPtr	pNA_FItem;	// Item Attribute 	
	
	try
	{
		pNS_FItem  = pNIndex->selectNodes("FITEM");
		
		//System의 version은 읽어올 필요없다
		//해당 모듈이 로드되기 전에 이미 version처리 되어진다.
		int nCount, nLength;
		nLength = pNS_FItem->length;
		for (nCount = 0; nCount < nLength; nCount++)
		{
			HeadInfoType stHeadInfoType;
			
			//node를 얻어온다.
			pN_FItem = pNS_FItem->item[nCount];	
			
			//속성을 얻어온다.
			MSXML::IXMLDOMNodePtr	pNAttributeTemp;
			pNA_FItem= pN_FItem->attributes;
			
			//ID
			pNAttributeTemp = pNA_FItem->getNamedItem("ID");
			stHeadInfoType.lHeadID = atol(CString((PCHAR)pNAttributeTemp->text));

			// 분, 신 필드 삭제
			if(stHeadInfoType.lHeadID == 8000 || stHeadInfoType.lHeadID == 8001)
				continue;
			
			//FIELD_NAME
			pNAttributeTemp = pNA_FItem->getNamedItem("FIELD_NAME");
			stHeadInfoType.strHeadName = CString((PCHAR)pNAttributeTemp->text);
			
			//ALIGN
			pNAttributeTemp = pNA_FItem->getNamedItem("ALIGN");
			stHeadInfoType.strAlignment = CString((PCHAR)pNAttributeTemp->text);
			
			//COL_WIDTH
			pNAttributeTemp = pNA_FItem->getNamedItem("COL_WIDTH");
			stHeadInfoType.nColWidth = atoi(CString((PCHAR)pNAttributeTemp->text));
			
			//DATA_TYPE
			pNAttributeTemp = pNA_FItem->getNamedItem("DATA_TYPE");
			stHeadInfoType.strDataType = CString((PCHAR)pNAttributeTemp->text);
			
			//UNIT_NUM
			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_NUM");
			stHeadInfoType.nUnitNum = atoi(CString((PCHAR)pNAttributeTemp->text));
			
			//UNIT_TEXT
			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_TEXT");
			stHeadInfoType.strUnitText = CString((PCHAR)pNAttributeTemp->text);
			
			//DATA_LENGTH
			pNAttributeTemp = pNA_FItem->getNamedItem("DATA_LENGTH");
			stHeadInfoType.nDataLength = atoi(CString((PCHAR)pNAttributeTemp->text));
			
			//			//UNIT
			//			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT");
			//			stHeadInfoType.nDataLength = atoi(CString((PCHAR)pNAttributeTemp->text));
			
			// 시스템용 리스트에 추가
			listFixList.AddTail(stHeadInfoType);
		}
	}
	catch(_com_error e)
	{
		CString strMsg = "시스템 파일을 읽어오는 중 에러가 발생하였습니다. 고객지원센터로 연락 바랍니다.";
		SetSendMainMsg(strMsg);
		bResult = FALSE;
	}
	
	return bResult;
}

BOOL CConditionResultWnd::GetLoadFixUserFieldData(MSXML::IXMLDOMNodePtr pNIndex,
												  CList<HeadFixInfo, HeadFixInfo&> &listFixList)
{
	BOOL bResult = TRUE;
	
	MSXML::IXMLDOMNamedNodeMapPtr	pNAIndex;
	
	MSXML::IXMLDOMNodeListPtr			pNS_FItem;		// Item List
	MSXML::IXMLDOMNodePtr					pN_FItem;	// Item
	MSXML::IXMLDOMNamedNodeMapPtr	pNA_FItem;		// Item Attribute
	
	CString							strView;
	CString							strViewName;
	try 
	{
		MSXML::IXMLDOMNodePtr	pNAttributeTemp;
		pNAIndex = pNIndex->attributes;
		
		pNAttributeTemp = pNAIndex->getNamedItem("VERSION");
		//	m_strFileVersion = CString((PCHAR)pNAttributeTemp->text);			
		
		pNAttributeTemp = pNAIndex->getNamedItem("FIX_COL");
		m_nFixCol = atoi(CString((PCHAR)pNAttributeTemp->text));
		
		pNS_FItem  = pNIndex->selectNodes("FITEM");
		
		//Attribute를 얻어온다
		int nCount, nLength;
		nLength = pNS_FItem->length;
		for (nCount = 0; nCount < nLength; nCount++)
		{
			HeadFixInfo stHeadFixInfo;
			
			//node를 얻어온다.
			pN_FItem = pNS_FItem->item[nCount];	
			
			//속성을 얻어온다.
			MSXML::IXMLDOMNodePtr	pNAttributeTemp;
			pNA_FItem = pN_FItem->attributes;
			
			//ID
			pNAttributeTemp = pNA_FItem->getNamedItem("ID");
			stHeadFixInfo.stHeadType.lHeadID = atol(CString((PCHAR)pNAttributeTemp->text));
			
			//COL_NO
			pNAttributeTemp = pNA_FItem->getNamedItem("COL_NO");
			stHeadFixInfo.nColNum = atoi(CString((PCHAR)pNAttributeTemp->text));
			
			//FIELD_NAME
			pNAttributeTemp = pNA_FItem->getNamedItem("FIELD_NAME");
			stHeadFixInfo.stHeadType.strHeadName = CString((PCHAR)pNAttributeTemp->text);
			
			//UNIT_NUM
			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_NUM");
			stHeadFixInfo.stHeadType.nUnitNum = atoi(CString((PCHAR)pNAttributeTemp->text));
			
			//UNIT_TEXT
			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_TEXT");
			stHeadFixInfo.stHeadType.strUnitText = CString((PCHAR)pNAttributeTemp->text);
			
			//ALIGN
			pNAttributeTemp = pNA_FItem->getNamedItem("ALIGN");
			stHeadFixInfo.stHeadType.strAlignment = CString((PCHAR)pNAttributeTemp->text);
			
			//COL_WIDTH
			pNAttributeTemp = pNA_FItem->getNamedItem("COL_WIDTH");
			stHeadFixInfo.stHeadType.nColWidth = atoi(CString((PCHAR)pNAttributeTemp->text));
			
			//DATA_TYPE
			pNAttributeTemp = pNA_FItem->getNamedItem("DATA_TYPE");
			stHeadFixInfo.stHeadType.strDataType = CString((PCHAR)pNAttributeTemp->text);
			
			//DATA_LENGTH
			pNAttributeTemp = pNA_FItem->getNamedItem("DATA_LENGTH");
			stHeadFixInfo.stHeadType.nDataLength = atoi(CString((PCHAR)pNAttributeTemp->text));
			
			
			//VIEW_NAME
			pNAttributeTemp = pNA_FItem->getNamedItem("VIEW_NAME");
			strViewName = CString((PCHAR)pNAttributeTemp->text);
			
			//VISIBLE
			pNAttributeTemp = pNA_FItem->getNamedItem("VISIBLE");
			stHeadFixInfo.bView = (CString((PCHAR)pNAttributeTemp->text) == "TRUE")?TRUE:FALSE;
			
			listFixList.AddTail(stHeadFixInfo);
		}
	}
	catch(_com_error e) 
	{
		CString strMsg = "사용자 파일을 읽어오는 중 에러가 발생하였습니다. 고객지원센터로 연락 바랍니다.";
		SetSendMainMsg(strMsg);
		bResult = FALSE;
	}
	
	return bResult;
}

BOOL CConditionResultWnd::GetHave_FixUserField(CString strField)
{
	BOOL bResult = FALSE;
	
	POSITION	pos;
	HeadFixInfo	 stHeadFixInfo;
	for (int nIndex = 0; nIndex < m_listFixHeadUser.GetCount(); nIndex++)
	{
		pos = m_listFixHeadUser.FindIndex(nIndex);
		stHeadFixInfo = m_listFixHeadUser.GetAt(pos);
		
		if (strField == stHeadFixInfo.stHeadType.strHeadName)
		{
			bResult = TRUE;
			break;
		}
	}
	
	return bResult;
}

void CConditionResultWnd::SetUpdateUserFieldData(CList<HeadInfoType, HeadInfoType&> *plistFixSys, 
												 CList<HeadFixInfo, HeadFixInfo&> *plistFixUser)
{
	POSITION	posSys, posUser;
	HeadInfoType stHeadInfoType;
	HeadFixInfo	 stHeadFixInfo;
	
	int nSysCnt = plistFixSys->GetCount();
	int nUserCnt = plistFixUser->GetCount();
	for (int nSysIndex = 0; nSysIndex < nSysCnt; nSysIndex++)
	{
		posSys = plistFixSys->FindIndex(nSysIndex);
		stHeadInfoType = plistFixSys->GetAt(posSys);
		
		for (int nUserIndex = 0; nUserIndex < nUserCnt; nUserIndex++)
		{
			posUser = plistFixUser->FindIndex(nUserIndex);
			stHeadFixInfo = plistFixUser->GetAt(posUser);
			
			if (stHeadInfoType.lHeadID == stHeadFixInfo.stHeadType.lHeadID)
			{
				if (stHeadFixInfo.stHeadType.strHeadName.Compare(stHeadInfoType.strHeadName) == -1)
					stHeadFixInfo.stHeadType.strHeadName = stHeadInfoType.strHeadName;
				
				stHeadFixInfo.stHeadType.strDataType = stHeadInfoType.strDataType;
				stHeadFixInfo.stHeadType.nUnitNum	= stHeadInfoType.nUnitNum;
				stHeadFixInfo.stHeadType.strUnitText = stHeadInfoType.strUnitText;
				stHeadFixInfo.stHeadType.nDataLength = stHeadInfoType.nDataLength;
				stHeadFixInfo.stHeadType.lHeadID = stHeadInfoType.lHeadID;
				stHeadFixInfo.stHeadType.strAlignment	= stHeadInfoType.strAlignment;
				stHeadFixInfo.stHeadType.nColWidth = stHeadInfoType.nColWidth;
				//				//stHeadFixInfo.strUNIT						= stHeadInfoType.strUNIT;
				//				//stHeadFixInfo.strVISIBLE					= stHeadInfoType.strVISIBLE;
				
				plistFixUser->SetAt(posUser, stHeadFixInfo);
			}
		}
	}
}


BOOL CConditionResultWnd::GetLoadFixField_Def(CStringArray &saFieldId)
{
	CFileFind cFind;
	BOOL bRead = TRUE;
	
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	CString strFilePath = pwndMainFrm->GetFilePath("FINDERINFO");
	
	bRead = cFind.FindFile(strFilePath);
	if (!bRead)
	{
		CString strMsg = (CString)FINDERINFO_INIFILE + "을 찾지 못했습니다.";
		SetSendMainMsg(strMsg);
	}
	else
	{
		char chColID[10];
		memset(chColID, ' ', sizeof(chColID));
		
		int nCount = GetPrivateProfileInt("DefaultColumn", "Line Count", 0, strFilePath);
		CString strTemp;
		for (int nIndex = 0; nIndex < nCount; nIndex++)
		{
			strTemp.Format("%d", nIndex + 1);
			GetPrivateProfileString("DefaultColumn", strTemp, "", chColID, sizeof(chColID), strFilePath);
			saFieldId.Add(chColID);
		}
	}
	
	return bRead;
}


BOOL CConditionResultWnd::SetSaveFixUserFieldData(CList<HeadFixInfo, HeadFixInfo&> *plistFixUser,
												  CString strVersion)
{
	BOOL bResult = TRUE;
	
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	CString strFilePath = pwndMainFrm->GetFilePath("FIX_USER");
	
	MSXML::IXMLDOMNodePtr						pNIndex;
	MSXML::IXMLDOMNamedNodeMapPtr		pNAIndex;
	
	MSXML::IXMLDOMNodePtr						pN_FItem;
	MSXML::IXMLDOMNamedNodeMapPtr		pNA_FItem;
	
	try 
	{
		CString strTemp;
		
		MSXML::IXMLDOMDocumentPtr pDOMIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
		
		MSXML::IXMLDOMProcessingInstructionPtr pPI;
		MSXML::IXMLDOMNamedNodeMapPtr					pNAPI;
		MSXML::IXMLDOMNodePtr									pNEncoding;
		
		pPI = pDOMIndex->createProcessingInstruction("xml", "version=\"1.0\"");
		
		pNEncoding = pDOMIndex->createAttribute("encoding");
		pNEncoding->text = "KSC_5601";
		
		pNAPI = pPI->attributes;
		pNAPI->setNamedItem(pNEncoding);
		
		pDOMIndex->appendChild(pPI);
		
		pNIndex = pDOMIndex->createElement("INDEX");
		pNAIndex = pNIndex->attributes;
		
		MSXML::IXMLDOMNodePtr pNAttributeTemp;
		pNAttributeTemp = pDOMIndex->createAttribute("VERSION");
		pNAttributeTemp->text = (LPCTSTR)strVersion;
		pNAIndex->setNamedItem(pNAttributeTemp);
		
		pNAttributeTemp = pDOMIndex->createAttribute("FIX_COL");
		strTemp.Format("%d", m_nFixCol);
		pNAttributeTemp->text = (LPCTSTR)strTemp;
		pNAIndex->setNamedItem(pNAttributeTemp);
		
		pDOMIndex->appendChild(pNIndex);
		
		POSITION pos;
		HeadFixInfo	stHeadFixInfo;
		int nFieldCount = plistFixUser->GetCount();
		for (int nIndex = 0;  nIndex < nFieldCount; nIndex++)	
		{
			pos = plistFixUser->FindIndex(nIndex);
			stHeadFixInfo = plistFixUser->GetAt(pos);
			
			MSXML::IXMLDOMNodePtr pNAttributeTemp;
			
			pN_FItem = pDOMIndex->createElement("FITEM");
			pNIndex->appendChild(pN_FItem);
			pNA_FItem = pN_FItem->attributes;
			
			//ID
			pNAttributeTemp = pDOMIndex->createAttribute("ID");
			strTemp.Format("%d", stHeadFixInfo.stHeadType.lHeadID);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//COL_NO
			pNAttributeTemp = pDOMIndex->createAttribute("COL_NO");
			strTemp.Format("%d", stHeadFixInfo.nColNum);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//FIELD_NAME
			pNAttributeTemp = pDOMIndex->createAttribute("FIELD_NAME");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strHeadName;;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//UNIT
			pNAttributeTemp = pDOMIndex->createAttribute("UNIT");
			pNAttributeTemp->text = (LPCTSTR)"1";
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//ALIGN
			pNAttributeTemp = pDOMIndex->createAttribute("ALIGN");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strAlignment;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//COL_WIDTH
			pNAttributeTemp = pDOMIndex->createAttribute("COL_WIDTH");
			strTemp.Format("%d", stHeadFixInfo.stHeadType.nColWidth);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//DATA_TYPE
			pNAttributeTemp = pDOMIndex->createAttribute("DATA_TYPE");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strDataType;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//VIEW_NAME
			pNAttributeTemp = pDOMIndex->createAttribute("VIEW_NAME");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strHeadName;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//VISIBLE
			pNAttributeTemp = pDOMIndex->createAttribute("VISIBLE");
			strTemp = (stHeadFixInfo.bView == TRUE)?"TRUE":"FALSE";
			pNAttributeTemp->text =	(LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//UNIT_NUM
			pNAttributeTemp = pDOMIndex->createAttribute("UNIT_NUM");
			strTemp.Format("%d", stHeadFixInfo.stHeadType.nUnitNum);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//UNIT_TEXT
			pNAttributeTemp = pDOMIndex->createAttribute("UNIT_TEXT");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strUnitText;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//DATA_LENGTH
			pNAttributeTemp = pDOMIndex->createAttribute("DATA_LENGTH");
			strTemp.Format("%d", stHeadFixInfo.stHeadType.nDataLength);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);
		}
		
		pDOMIndex->save(_bstr_t(strFilePath));
}	
catch(_com_error e)
{
	bResult = FALSE;
}

return bResult;
}

void CConditionResultWnd::SetAdd_MoveHead(HeadMoveInfo stHeadMoveInfo)
{
	m_listHeadMoveInfo.AddTail(stHeadMoveInfo);
}

void CConditionResultWnd::SetAllDelete_MoveHead()
{
	m_listHeadMoveInfo.RemoveAll();
}

int CConditionResultWnd::GetCount_MoveHead()
{
	int nResult = 0;
	
	POSITION pos;
	HeadMoveInfo		stHeadMoveInfo;
	for (int nIndex = 0; nIndex < m_listHeadMoveInfo.GetCount(); nIndex++)
	{
		pos = m_listHeadMoveInfo.FindIndex(nIndex);
		stHeadMoveInfo = m_listHeadMoveInfo.GetAt(pos);
		
		if (stHeadMoveInfo.bView == TRUE)
			nResult = nResult + stHeadMoveInfo.nColCnt;
	}
	
	return nResult;
}

int CConditionResultWnd::GetCount_FixHead(BOOL &bHaveCodeFiled, CUIntArray &iaFixHeadId)
{
	int nResult = 0;
	
	bHaveCodeFiled = FALSE;
	iaFixHeadId.RemoveAll();
	
	POSITION pos;
	HeadFixInfo		stHeadFixInfo;
	for (int nIndex = 0; nIndex < m_listFixHeadUser.GetCount(); nIndex++)
	{
		pos = m_listFixHeadUser.FindIndex(nIndex);
		stHeadFixInfo = m_listFixHeadUser.GetAt(pos);
		
		if (stHeadFixInfo.bView == TRUE)
		{
			if (stHeadFixInfo.stHeadType.strDataType == "code")
				bHaveCodeFiled = TRUE;
			else
				iaFixHeadId.Add(stHeadFixInfo.stHeadType.lHeadID);
			
			nResult++;
		}
		else
		{
			if (stHeadFixInfo.stHeadType.strHeadName == (CString)GRID_MOVEFIELD_POS_NAME)
				iaFixHeadId.Add(stHeadFixInfo.stHeadType.lHeadID);
		}
	}
	
	return nResult;
}

void CConditionResultWnd::GetTrIndex(long &nTrIndex)
{
	nTrIndex = m_nTrIndex;
	
	m_nLastTrIndex = m_nTrIndex;
	m_nTrIndex++;
}

void CConditionResultWnd::OnTimer(UINT nIDEvent) 
{
	KillTimer(1);
	
	if (m_nSearchType == 1)
	{
		SetSearch(FALSE);
		m_nSearchType = 1;
	}
	
	CWnd::OnTimer(nIDEvent);
}

int CConditionResultWnd::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	return pwndMainFrm->SetMessageBox(lpszText, lpszCaption, nType);
}

typedef struct {
	EU_p0621_InRec1 inRec1;
	EU_p0621_InRec2 inRec2[MAX_IDXCNT];
} EU_p0621_InAll;

#define INT2STR_BUF(xStr, nSize, nStr) sprintf(xStr, "%#0*.*d", nSize, nSize, nStr)
int CConditionResultWnd::InMakeTr_P0621(BYTE *pTrData, BYTE* pTrOutData)
{
	int nInRec2Cnt = MAX_IDXCNT;
	User_Finder* pUserFinder = (User_Finder*)pTrData;
	EU_p0621_InAll* pInAll = (EU_p0621_InAll*)pTrOutData;
	EU_p0621_InRec1 *pInRec1 = (EU_p0621_InRec1*)&pInAll->inRec1;
	EU_p0621_InRec2 *pInRec2 = (EU_p0621_InRec2*)&pInAll->inRec2;

	char tmpVal[1024]={0,};

	memcpy(pInRec1->sJongFillterChk	, (void*)&pUserFinder->stTargetChg.sJongFillterChk,  1);
	memcpy(pInRec1->sJongFillterIndex, pUserFinder->stTargetChg.sJongFillterIndex, sizeof(pInRec1->sJongFillterIndex));

	INT2STR_BUF(tmpVal, 4, pUserFinder->TrIndex);
	memcpy(pInRec1->giCallGubun				, tmpVal, 4);

	INT2STR_BUF(tmpVal, 2, pUserFinder->nTargetType);
	memcpy(pInRec1->giTargetGubun			, tmpVal, 2);

	
	INT2STR_BUF(tmpVal, 2, pUserFinder->TargetIdxCnt);
	memcpy(pInRec1->giTargetIdxCnt			, tmpVal, 2);
	tmpVal[2]=0;
	nInRec2Cnt = atoi(tmpVal);

	INT2STR_BUF(tmpVal, 2, pUserFinder->ComplexIdxCnt);
	memcpy(pInRec1->giComplexIdxCnt			, tmpVal, 2);
	tmpVal[2]=0;
	nInRec2Cnt += atoi(tmpVal);


	INT2STR_BUF(tmpVal, 2, pUserFinder->TargetResultViewCnt);
	memcpy(pInRec1->giTargetResultViewCnt	, tmpVal, 2);
		
	memcpy(pInRec1->gsTargetExp				, pUserFinder->TargetExp, 80);

	INT2STR_BUF(tmpVal, 2, pUserFinder->ComplexIdxCnt);
	memcpy(pInRec1->giComplexIdxCnt			, tmpVal, 2);

	INT2STR_BUF(tmpVal, 2, pUserFinder->ComplexResultViewCnt);
	memcpy(pInRec1->giComplexResultViewCnt	, tmpVal, 2);

	if(pUserFinder->ComplexExp)
		memcpy(pInRec1->gsComplexExp			, pUserFinder->ComplexExp, min(80, strlen(pUserFinder->ComplexExp)));

	INT2STR_BUF(tmpVal, 2, pUserFinder->FieldEditerCnt);
	memcpy(pInRec1->giFieldEditerCnt		, tmpVal, 2);

	// FieldList
	int nFi = 0;
	for(nFi=0; nFi<18; nFi++)
	{
		sprintf(&tmpVal[nFi*4], "%#04.4d", pUserFinder->FieldList[nFi]);
	}
	sprintf(&tmpVal[nFi*4], "%s", "00000000");
	memcpy(pInRec1->giFieldList				, tmpVal, RESULT_FIELD_MAX_EUGENE);

	INT2STR_BUF(tmpVal, 4, pUserFinder->ResultIdxCnt);
	memcpy(pInRec1->giResultIdxCnt			, tmpVal, 4);

	memcpy(pInRec1->gsResultExp				, pUserFinder->ResultExp, 80);
	memcpy(pInRec1->gsTargetBitChk			, (void*)&pUserFinder->TargetBitChk,  1);
	memcpy(pInRec1->gsJongmokIndex			, pUserFinder->TargetBit,	4000);	//3000 -> 4000
//	memset(pInRec1->gsJongmokIndex			, '1',	2500);
//	memcpy(pInRec1->gsTargetBitChk			, tmpVal, 4);
//	memcpy(pInRec1->gsJongmokIndex			, pUserFinder->ResultExp, 80);
	memcpy(pInRec1->gsSort_flag				, "0", 1);

	CString strDate, strTime;
	CMainFrmWnd *wndMainFrm = (CMainFrmWnd*)m_pwndMain;
	if(wndMainFrm->GetBackDateAndTime(strDate, strTime))
	{
		memcpy(pInRec1->giBackDate, strDate, 8);
		memcpy(pInRec1->giBackTime, strTime, 6);
	}

	User_Define* pUserDefine = pUserFinder->stUser_Define;
	for(int i=0; i<nInRec2Cnt && i<MAX_IDXCNT; i++)
	{
		INT2STR_BUF(tmpVal, 2, pUserDefine[i].IndexNo);
		memcpy(pInRec2[i].giIndexNo		,tmpVal, 2);

		INT2STR_BUF(tmpVal, 2, pUserDefine[i].GroupNo);
		memcpy(pInRec2[i].giGroupNo		,tmpVal, 2);

		INT2STR_BUF(tmpVal, 2, pUserDefine[i].GroupIdxCnt);
		memcpy(pInRec2[i].giGroupIdxCnt,tmpVal, 2);

		INT2STR_BUF(tmpVal, 2, pUserDefine[i].TargetBase);
		memcpy(pInRec2[i].giTargetBase	,tmpVal, 2);

		INT2STR_BUF(tmpVal, 4, pUserDefine[i].Type);
		memcpy(pInRec2[i].giType		,tmpVal, 4);

		//재무분석 
		if(atoi(tmpVal) == 4001 ||atoi(tmpVal) == 4002)
		{		
			//순위 : 4002, 범위 : 4001
			CString strData;
			if(pUserDefine[i].IndexType == '0')
				strData = _T("4001");
			if(pUserDefine[i].IndexType == '1')
				strData = _T("4002");
			memcpy(pInRec2[i].giType		,strData, 4);
		}
		

		INT2STR_BUF(tmpVal, 2, pUserDefine[i].Type1);
		memcpy(pInRec2[i].giType1		,tmpVal, 2);

		INT2STR_BUF(tmpVal, 2, pUserDefine[i].Type2);
		memcpy(pInRec2[i].giType2		,tmpVal, 2);

		INT2STR_BUF(tmpVal, 4, pUserDefine[i].STerm);
		memcpy(pInRec2[i].giSTerm		,tmpVal, 4);

		INT2STR_BUF(tmpVal, 4, pUserDefine[i].ETerm);
		memcpy(pInRec2[i].giETerm		,tmpVal, 4);

		INT2STR_BUF(tmpVal, 4, pUserDefine[i].Rank);
		memcpy(pInRec2[i].giRank		,tmpVal, 4);

		INT2STR_BUF(tmpVal, 4, pUserDefine[i].Trend);
		memcpy(pInRec2[i].giTrend		,tmpVal, 4);

		INT2STR_BUF(tmpVal, 4, pUserDefine[i].SignalCnt1);
		memcpy(pInRec2[i].giSignalCnt1		,tmpVal, 4);

		INT2STR_BUF(tmpVal, 4, pUserDefine[i].SignalCnt2);
		memcpy(pInRec2[i].giSignalCnt2		,tmpVal, 4);

		INT2STR_BUF(tmpVal, 2, pUserDefine[i].ResultViewNum);
		memcpy(pInRec2[i].giResultViewNum	,tmpVal, 2);
		
		pInRec2[i].gsIndexType[0]		= pUserDefine[i].IndexType;
		pInRec2[i].gsSearchType[0]		= pUserDefine[i].SearchType;
		pInRec2[i].gsOperator[0]		= pUserDefine[i].Operator;
		pInRec2[i].gsReverse[0]			= pUserDefine[i].Reverse;
		pInRec2[i].gsIdxContiGubun[0]	= pUserDefine[i].IdxContiGubun;
		pInRec2[i].gsElement[0]			= pUserDefine[i].Element;

		memcpy(pInRec2[i].gsScode	,pUserDefine[i].Code, 6);

		sprintf(tmpVal, "%#016f%#016f%#016f",	pUserDefine[i].Param[0],
										pUserDefine[i].Param[1],
										pUserDefine[i].Param[2]);
		memcpy(pInRec2[i].gdParam, tmpVal, 6*16);

		for(nFi=0; nFi<12; nFi++)
		{
			sprintf(&tmpVal[nFi*16],"%#016f", pUserDefine[i].Condition[nFi]);
		}
		memcpy(pInRec2[i].gdCondition	,tmpVal, 16*12);
	}

	int nRetSize = sizeof(EU_p0621_InRec1) + ( sizeof(EU_p0621_InRec2) * nInRec2Cnt);
	return nRetSize;
}

void CConditionResultWnd::InMakeFid_P0621(LPCSTR szTrNo, int nTrLen, BYTE *pTrData, char* szSaveBuf, int &nPos)
{
	//1. 연속패킷, DRFN 연속키값 쓸필요있나???, 그련 space(80)
//	memset(&szSaveBuf[nPos], 0x20, 80);
//	nPos += 80;

	//2. TR값 문자열로 만든다.
	EU_p0621_InAll inAll;
	memset(&inAll, 0x20, sizeof(EU_p0621_InAll));
	int nSendTrDataSize = InMakeTr_P0621(pTrData, (BYTE*)&inAll);
	memcpy(&szSaveBuf[nPos], &inAll, nSendTrDataSize);
	nPos += nSendTrDataSize;

	char *pBuff;
	int nData;
	INT_CONVERT(pBuff, pTrData + SZ_User_Finder, 8, nData );
	
	if(nData >0)
	{
		memcpy(&szSaveBuf[nPos], pTrData + SZ_User_Finder, 8);
		nPos += 8;
		memcpy(&szSaveBuf[nPos], pTrData + SZ_User_Finder + 8, nData);
		nPos += nData;
	}

}

void CConditionResultWnd::InMakeFid_Mstcode(LPCSTR szTrNo, int nTrLen, BYTE *pTrData, char* szSaveBuf, int &nPos)
{
	//1. 연속패킷, DRFN 연속키값 쓸필요있나???, 그련 space(80)
	memset(&szSaveBuf[nPos], 0x20, 80);
	nPos += 80;

	//2. TR값 문자열로 만든다.
//	EU_p0621_InAll inAll;
//	memset(&inAll, 0x20, sizeof(EU_p0621_InAll));
//	int nSendTrDataSize = InMakeTr_Mstcode(pTrData, (BYTE*)&inAll);
//	memcpy(&szSaveBuf[nPos], &inAll, nSendTrDataSize);
//	nPos += nSendTrDataSize;

	//3. 레코드 번호 1부터 차례로 
	CString szOutRecord;

	// 1st Record
	szOutRecord.Format("%-16d", 1);
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	GetFillString(szOutRecord, 0, 0, 3);	// 0 ~ 4
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	memcpy(&szSaveBuf[nPos], "|", 1);	//구분자.
	nPos++;

	// 2st Record
	szOutRecord.Format("%-16d", 2);
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	GetFillString(szOutRecord, 0, 3, 3);	// 0 ~ 5
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	memcpy(&szSaveBuf[nPos], ";", 1);	//구분자.
	nPos++;
}

int CConditionResultWnd::GetFillString(CString& _szGetStr, int nStart, int nEnd, int nOneWordSize)
{
	int nLen = nEnd-nStart+1;
	if(nLen<=0) return 0;
	
	_szGetStr.Empty();
	
	char *szGetStr = (char*)malloc((nLen*nOneWordSize)+1);
	char *szOneWord = (char*)malloc(nOneWordSize+1);
	memset(szOneWord, 0, nOneWordSize+1);
	
	for(int i=nStart; i<=nEnd; i++)
	{
		sprintf(szOneWord, "%*.*d", nOneWordSize, nOneWordSize, i);
		memcpy(&szGetStr[nOneWordSize*i], szOneWord, nOneWordSize);
	}
	
	szGetStr[nLen*nOneWordSize] = 0;
	_szGetStr = szGetStr;
	free(szOneWord);
	free(szGetStr);
	
	return _szGetStr.GetLength();
}

int CConditionResultWnd::InMakeTr_Mstcode(BYTE *pTrData, BYTE* pTrOutData)
{
	return 0;
}

void CConditionResultWnd::InMakeFid_Workdata(LPCSTR szTrNo, int nTrLen, BYTE *pTrData, char* szSaveBuf, int &nPos)
{
	//1. 연속패킷, DRFN 연속키값 쓸필요있나???, 그련 space(80)
	memset(&szSaveBuf[nPos], 0x20, 80);
	nPos += 80;

	//2. TR값 문자열로 만든다.
	CString szInputData;
	int nSendTrDataSize = InMakeTr_Workdate(pTrData, szInputData);
	memcpy(&szSaveBuf[nPos], (LPCSTR)szInputData, nSendTrDataSize);
	nPos += nSendTrDataSize;

	//3. 레코드 번호 1부터 차례로 
	CString szOutRecord;

	// 1st Record
	szOutRecord.Format("%-16d", 1);
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	GetFillString(szOutRecord, 0, 0, 3);	// 0 Only
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	memcpy(&szSaveBuf[nPos], "|", 1);	//구분자.
	nPos++;

	// 2st Record
	szOutRecord.Format("%-16d", 2);
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	GetFillString(szOutRecord, 0, 0, 3);	// 0 Only
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	memcpy(&szSaveBuf[nPos], ";", 1);	//구분자.
	nPos++;
}

int CConditionResultWnd::InMakeTr_Workdate(BYTE *pTrData, CString &szInputData)
{
	//CConditionResultWnd::SetSearch_WorkDate()에서 42셋팅. 고정임.
	szInputData = _T("0042");		// 카운트 42개 요청.
	return szInputData.GetLength();
}

void CConditionResultWnd::ChangeSkinColor(COLORREF clrSkin)
{
	if(!clrSkin)
		return;

	m_clrMainSkin = clrSkin;

	if(m_brushBk.DeleteObject())
		m_brushBk.CreateSolidBrush(m_clrMainSkin);

	CMainFrmWnd *wndMainFrm = (CMainFrmWnd*)m_pwndMain;

	ST_GRIDCOLOR stGridColor;
	stGridColor.clrGridHeader = wndMainFrm->GetAxColor(74);
 	stGridColor.clrGridHilight = wndMainFrm->GetAxColor(78);

	ChangeGridColor(stGridColor);

	Invalidate();
}

void CConditionResultWnd::OnClickSearchBtn()
{
	OnBtnSearch();
}

void CConditionResultWnd::OnClickPrintBtn()
{
	OnBtnPrint();
}

void CConditionResultWnd::OnClickEditBtn()
{
	OnBtnFieldEdit();
}

void CConditionResultWnd::OnClickSendBtn()
{
	OnBtnResultCtrl();
}


BOOL CConditionResultWnd::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_RBUTTONUP == pMsg->message)
	{
		CMainFrmWnd* wndMainFrm = (CMainFrmWnd*)m_pwndMain;
		::PostMessage(wndMainFrm->GetParent()->m_hWnd, WM_CONTEXTMENU, 0, 0);
	}
	
	return CWnd::PreTranslateMessage(pMsg);
}

//piso0350","so035010", "so035020"
BOOL CConditionResultWnd::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
//Tr Monitor
// 	if(!m_pDebugMng || !pCopyDataStruct)
// 		return FALSE;
// 
// 	CString strTr;
// 	STIDM2Info_TranData *pData = (STIDM2Info_TranData*)pCopyDataStruct->lpData;
// 	
// 	int nSize = (int)(pCopyDataStruct->cbData);
// 
// 	if(strstr(pData->aDataKey2, "piso0350") >= 0)
// 		strTr = TRNO_CONDITION;
// 	else if(strstr(pData->aDataKey2, "so035010") >= 0)
// 		strTr = TRNO_WORKDATE;
// 	else if(strstr(pData->aDataKey2, "so035020") >= 0)
// 		strTr = TRNO_MSTCODE;
// 
// 	if (strTr.CompareNoCase("") != 0)
// 	{
// 		ReceiveData(strTr, "", "", (LPSTR)(LPCTSTR)pData->pData, nSize-(sizeof(pData)-1));	
// 	}
// 
 	return CWnd::OnCopyData(pWnd, pCopyDataStruct);
}

void CConditionResultWnd::ChangeGridColor(ST_GRIDCOLOR stGridColor)
{
	//우측 상단 그리드
	m_pGridCtrl->ChangeGridColor(stGridColor);
	m_pGridCtrl->Invalidate();
	m_pGridCtrl->SetCellBorderColor4Print(COLOR_BORDER);
}

void CConditionResultWnd::OnChangeSubject()
{
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	CDlgEditSubject dlgEdotSubject(this);
	m_pDlgEditSubject = &dlgEdotSubject;
	
	if (dlgEdotSubject.DoModal() == IDOK)
	{
//		m_saMarket.RemoveAll();
		SetTargetEditIndicate();

		CString strFileName, strSavePath, strUserPath;
		strUserPath.Format("%s%s%s", Variant(homeCC), USRDIR, Variant(nameCC));
		if (m_nSeparateForm == 0)
			strFileName = "7127_ConditionInfo.dat"; // 조건검색
		else
			strFileName = "8416_ConditionInfo.dat"; // 0201 ~ 0209 화면이라면
		strSavePath =  strUserPath + (CString)FOLDER_FINDDATA + "\\" + strFileName;

		WriteTargetInfo(strSavePath);
	}

	AfxSetResourceHandle(hInstSave);
	m_pDlgEditSubject = NULL;
}

void CConditionResultWnd::OnChangeSubjectInit()
{
	m_strEditSubjectSelectItemText		 = "전체";
	m_strEditSubjectSelectParentItemText = "";

	m_arTargetMText.RemoveAll();
	m_arTargetMText.Add(m_strEditSubjectSelectItemText);
	
	m_bIssuesForAdministration		= TRUE;
	m_bDealStop						= TRUE;
	m_bTidyDeal						= FALSE;
	m_bInsincerityNotice			= FALSE;
	m_bAttentionInvestment			= TRUE;
	m_bNoticeAttentionInvestment	= FALSE;
	m_bPreferenceStock				= TRUE;
	m_bDeposit						= FALSE;
	m_bDangerInvestment				= FALSE;
	m_bWarningInvestment			= FALSE;
	m_bETF							= FALSE;
//	m_bETN							= FALSE;

	m_nSelRadioMonth = 0;

	POSITION pos = m_mapExeptItem.GetStartPosition();
	CString strItemCode;
	CString strExceptDetail;
	CStringArray saExceptItem;
	for(pos = m_mapExeptItem.GetStartPosition(); pos != NULL;)
	{
		m_mapExeptItem.GetNextAssoc(pos, strItemCode, strExceptDetail);
		
		if(strExceptDetail.GetLength() != 8) //7 -> 8 tour2k
			continue;
		
		if(strExceptDetail.GetAt(0) == '1')		// 관리종목
			saExceptItem.Add(strItemCode);
		if(strExceptDetail.GetAt(1) == '1')		// 거래정지
			saExceptItem.Add(strItemCode);
		if(strExceptDetail.GetAt(4) == '1')		// 투자주의
			saExceptItem.Add(strItemCode);
		if(strExceptDetail.GetAt(5) == '1')		// 우선주
			saExceptItem.Add(strItemCode);
	}

	SetTargetEditIndicate();
	SetTargetBit(saExceptItem);
}

void CConditionResultWnd::OnSearchRngInit()
{
	m_bIssuesForAdministration		= TRUE;
	m_bDealStop						= TRUE;
	m_bTidyDeal						= FALSE;
	m_bInsincerityNotice			= FALSE;
	m_bAttentionInvestment			= TRUE;
	m_bNoticeAttentionInvestment	= FALSE;
	m_bPreferenceStock				= TRUE;
	m_bDeposit						= FALSE;
	m_bDangerInvestment				= FALSE;
	m_bWarningInvestment			= FALSE;
	m_bETF							= FALSE;
//	m_bETN							= FALSE;

	m_bOver							= FALSE;					
	// 20110629 박찬모 >>
	// 이상급등 -> 투자주의, 투자경고, 투자위험예고, 투자위험 투자유의 -> 투자주의환기
	m_bInvestAttention				= FALSE;			
	
	m_bEquityCapital				= FALSE;				
	m_bPriceRange					= FALSE;					
	m_bAggregateValueOfListedStocks	= FALSE;
	m_bTradingVolume				= FALSE;				
	m_bParValue						= FALSE;

	m_nEquityCapitalAbove					= 1;				
	m_nEquityCapitalBelow					= 10000;				
	m_nPriceRangeAbove						= 100;					
	m_nPriceRangeBelow						= 10000000;					
	m_nAggregateValueOfListedStocksAbove	= 10;
	m_nAggregateValueOfListedStocksBelow	= 100000;
	m_nTradingVolumeAbove					= 0;				
	m_nTradingVolumeBelow					= 1000;				
	m_nParValueAbove						= 100;					
	m_nParValueBelow						= 100000;
}

void CConditionResultWnd::SetTargetEditIndicate()
{
	m_szTargetEditIndicate = _T("");
	CString szTemp;
	
	szTemp.Format("<");

	for(int i = 0; i < m_arTargetMText.GetSize(); i++)
	{
		szTemp += m_arTargetMText.GetAt(i);
		
		if(_T("전체") == m_arTargetMText.GetAt(i))
			break;
		
		if(i != m_arTargetMText.GetSize() - 1)
			szTemp += _T(",");
	}
	
	szTemp += _T(">");
	RemoveCharFromString(szTemp, ' ');
	m_szTargetEditIndicate += szTemp;

	szTemp = _T("<");
	BOOL bExistBefore = FALSE;
	if(m_bIssuesForAdministration)
	{
		szTemp += _T("관리종목");
		bExistBefore = TRUE;
	}
	if(m_bDealStop)
	{
		if(bExistBefore)
			szTemp += _T("+");	
		else
			bExistBefore = TRUE;
		szTemp += _T("거래정지");
	}
	if(m_bTidyDeal)
	{
		if(bExistBefore)
			szTemp += _T("+");		
		else
			bExistBefore = TRUE;
		szTemp += _T("정리매매");
	}
	if(m_bInsincerityNotice)
	{
		if(bExistBefore)
			szTemp += _T("+");	
		else
			bExistBefore = TRUE;
		szTemp += _T("불성실공시");
	}
	if(m_bAttentionInvestment)
	{
		if(bExistBefore)
			szTemp += _T("+");	
		else
			bExistBefore = TRUE;
		szTemp += _T("투자주의");
	}
	if(m_bNoticeAttentionInvestment)
	{
		if(bExistBefore)
			szTemp += _T("+");	
		else
			bExistBefore = TRUE;
		szTemp += _T("투자위험예고");
	}
	if(m_bPreferenceStock)
	{
		if(bExistBefore)
			szTemp += _T("+");	
		else
			bExistBefore = TRUE;
		szTemp += _T("우선주");
	}
	if(m_bDeposit)
	{
		if(bExistBefore)
			szTemp += _T("+");	
		else
			bExistBefore = TRUE;
		szTemp += _T("증거금100%종목");
	}
	if(m_bDangerInvestment)
	{
		if(bExistBefore)
			szTemp += _T("+");	
		else
			bExistBefore = TRUE;
		szTemp += _T("투자위험");
	}
	if(m_bWarningInvestment)
	{
		if(bExistBefore)
			szTemp += _T("+");	
		else
			bExistBefore = TRUE;
		szTemp += _T("투자경고");
	}
	if(m_bETF)
	{
		if(bExistBefore)
			szTemp += _T("+");	
		else
			bExistBefore = TRUE;
		szTemp += _T("ETF");
	}
/*	if(m_bETN)
	{
		if(bExistBefore)
			szTemp += _T("+");	
		else
			bExistBefore = TRUE;
		szTemp += _T("ETN");
	}
*/
	if(!bExistBefore)
		szTemp += _T("제외없음>");
	else
		szTemp += _T("제외>");
	m_szTargetEditIndicate += szTemp;

	szTemp = _T("<");
	switch(m_nSelRadioMonth)
	{
		case 0 : szTemp += _T("전체월결산");	break;
		case 1 : szTemp += _T("3월결산");		break;
		case 2 : szTemp += _T("6월결산");		break;
		case 3 : szTemp += _T("9월결산");		break;
		case 4 : szTemp += _T("12월결산");		break;
		case 5 : szTemp.Format("<%d월 결산", m_nIndexMonthCombo + 1); break;
	}
	szTemp += _T(">");
	m_szTargetEditIndicate += szTemp;

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	((CConditionMakeWnd*)pwndMainFrm->GetWnd("MAKE"))->SetChagneTargetEditIndicate(m_szTargetEditIndicate);
		
}

void CConditionResultWnd::RemoveCharFromString(CString &parm_string, char parm_remove_char)
{
    int count = parm_string.GetLength(), index = 0;
	
    if(count > 0)
	{
        char *p_data = new char[count + 1];	
        for(int i = 0; i < count; i++)
		{
            if(parm_string[i] & 0x80)
			{
                p_data[index++] = parm_string[i++];
                p_data[index++] = parm_string[i];
            } 
			else 
			{    
                if(parm_remove_char != parm_string[i]) p_data[index++] = parm_string[i];
            }
        }
        p_data[index] = 0;        
        parm_string = p_data;
		
        delete[] p_data;
    }
}

//KHD : File 경로 Add
void CConditionResultWnd::OnSearchRng()
{
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	if(!m_pDlgSearchRng)
	{
		m_pDlgSearchRng = new CDlgSearchRng(this);

		CString strUserPath, strSaveFileName;
		strUserPath.Format("%s%s%s", Variant(homeCC), USRDIR, Variant(nameCC));
		/*strSaveFileName.Format("%d", ((CMainFrmWnd*)m_pwndMain)->m_nSeparateForm);*/
		strSaveFileName = "8416";
		m_pDlgSearchRng->m_stSavePath =  strUserPath + (CString)FOLDER_FINDDATA + "\\"+ strSaveFileName +"_ConditionInfo.dat";
	}

	if(m_pDlgSearchRng->DoModal() == IDOK)
	{
		OnBtnSearch();
	}

	AfxSetResourceHandle(hInstSave);
}

BOOL CConditionResultWnd::FilterEditSubject(D1021OUT2* pstOutListPacket)
{
	return TRUE;
}

BOOL CConditionResultWnd::FilterSearchRng(D1021OUT2* pstOutListPacket)
{
	// 20110629 박찬모 >>
	// 이상급등 -> 투자주의, 투자경고, 투자위험예고, 투자위험 투자유의 -> 투자주의환기
	if(m_bIssuesForAdministration && pstOutListPacket->sUnder[0] == 'Y')	 	// 관리종목
		return FALSE;

	if(m_bAttentionInvestment && pstOutListPacket->sMrktalrm[0] == '1')	 		// 투자주의
		return FALSE;

	if(m_bWarningInvestment && pstOutListPacket->sMrktalrm[0] == '2')			// 투자경고
		return FALSE;

	if(m_bNoticeAttentionInvestment && pstOutListPacket->sMrktalrm[0] == '4')	// 투자위험예고
		return FALSE;

	if(m_bDangerInvestment && pstOutListPacket->sMrktalrm[0] == '3')			// 투자위험
		return FALSE;

	if(m_bDealStop && pstOutListPacket->sStop[0] == 'Y')					 	// 거래정지
		return FALSE;

	if(m_bTidyDeal && pstOutListPacket->sJrmm[0] == 'Y')					 	// 정리매매
		return FALSE;

	if(m_bInvestAttention && pstOutListPacket->sInvtalrm[0] == '1')				// 투자주의환기
		return FALSE;
	
	if(m_bInsincerityNotice && pstOutListPacket->sDishonest[0] == 'Y')	 		// 불성실공시
		return FALSE;
	
	if(m_bPreferenceStock && pstOutListPacket->sUsjg[0] == '1')			 		// 우선주
		return FALSE;

	// 20110629 박찬모 <<

	char buf[20] = {0, };
	if(m_bEquityCapital)				// 자본금
	{
		memset(buf, 0x00, sizeof(buf));
		memcpy(buf, pstOutListPacket->sCapital, sizeof(pstOutListPacket->sCapital));
		double dEquityCapital = atof(buf);
		dEquityCapital /= 100000000; // 서버 -> 원단위, 화면 -> 억단위
		if(!(m_nEquityCapitalAbove < (int)dEquityCapital &&
			m_nEquityCapitalBelow > (int)dEquityCapital))
			return FALSE;
	}

	if(m_bPriceRange)					// 가격대
	{
		memset(buf, 0x00, sizeof(buf));
		memcpy(buf, pstOutListPacket->sPrice, sizeof(pstOutListPacket->sPrice));
		double dPriceRnage = atof(buf);
		if(!(m_nPriceRangeAbove < (int)dPriceRnage &&
			m_nPriceRangeBelow > (int)dPriceRnage))
			return FALSE;
	}

	if(m_bAggregateValueOfListedStocks)	// 시가총액
	{
		memset(buf, 0x00, sizeof(buf));
		memcpy(buf, pstOutListPacket->sTotPrice, sizeof(pstOutListPacket->sTotPrice));
		double dAggregateValueOfListedStocks = atof(buf);
		dAggregateValueOfListedStocks /= 100000000;  // 서버 -> 원단위, 화면 -> 억단위
		if(!((double)m_nAggregateValueOfListedStocksAbove < dAggregateValueOfListedStocks &&
			(double)m_nAggregateValueOfListedStocksBelow > dAggregateValueOfListedStocks))
			return FALSE;
	}

	if(m_bTradingVolume)				// 거래량
	{
		memset(buf, 0x00, sizeof(buf));
		memcpy(buf, pstOutListPacket->sVolume, sizeof(pstOutListPacket->sVolume));
		double dTradingVolume = atof(buf);
		dTradingVolume /= 10000; // 서버 -> (1)주단위, 화면 -> 만주단위
		if(!((double)m_nTradingVolumeAbove < dTradingVolume &&
			(double)m_nTradingVolumeBelow > dTradingVolume))
			return FALSE;
	}

	if(m_bParValue)						// 액면가
	{
		memset(buf, 0x00, sizeof(buf));
		memcpy(buf, pstOutListPacket->sParValue, sizeof(pstOutListPacket->sParValue));
		double dParValue = atof(buf);
		if(!(m_nParValueAbove < dParValue &&
			m_nParValueBelow > dParValue))
			return FALSE;
	}

	return TRUE;
}

void CConditionResultWnd::GetHaveItem()
{
	
	int nAccCnt = 0;
	/*int nAccCnt = linkDataManager.GetAccountCount();*/
	

	CString strTest;
	
	char szBuf[256] = {0,};
	CString szAccount;
	CString szTemp;

	m_saAccount.RemoveAll();
	for(int i = 0; i < nAccCnt; i++)
	{
		
		/*linkDataManager.GetAccountByIndex(i, szBuf, sizeof(szBuf));*/
		
		szAccount = szBuf;

		// (2011/2/22 - Seung-Won, Bae) Request only stock item.
		if( 2 < szAccount.GetLength()) switch( atoi( szAccount.Right( 2)))
		{
			case 1:		// 위탁계좌
			case 30:	// 일반증권저축
			case 31:	// 근로자저축
			case 32:	// 장기증권저축
			case 33:	// 근로자주식저축1
			case 34:	// 근로자주식저축2
			case 35:	// 할부식증권저축
			case 36:	// 근로자주식증권저축
			case 37:	// 세금우대종합저축
			case 38:	// 장기증권저축
			case 39:	// 생계형증권저축
			case 52:	// 어셋피아자문형
			case 53:	// True Friend WRAP
			case 54:	// 부자아빠 리서치파워
						m_saAccount.Add(szBuf);
						
						/*iJangoMng.RequestJangoData4Dll(szAccount, GetSafeHwnd());*/
						
						break;
		}
	}
}

LRESULT CConditionResultWnd::OnReqJangoData(WPARAM wParam, LPARAM lParam)
{
	m_nRegJangoCount++;

	if(m_saAccount.GetSize() == m_nRegJangoCount)
	{
		CString strItem;
		CString strCode;
		CString strName;
		int		nSize = 0;
		BOOL	bAlreadyInset;
		for(int i = 0; i < m_saAccount.GetSize(); i++)
		{
			CString strTempAccount = m_saAccount.GetAt(i);
			if(strTempAccount.Right(2) == "01")
			{
				ST_HAVEITEM stHaveItem;
				stHaveItem.strAccount = strTempAccount;
				
				
				/*strItem = iJangoMng.GetFindKeyData4Bal(strTempAccount, "", "", "0,1", ",");*/
				

				int nCount = strItem.GetLength();
				nCount /= REQ_LENGTH; // 보유종목수

				if(nCount == 0)
					continue;

				for(int i = 0; i < nCount; i++)
				{
					bAlreadyInset = FALSE;
					strCode = strItem.Mid(i * REQ_LENGTH, CODE_LENGTH);
					strName = strItem.Mid(i * REQ_LENGTH + BETWEEN_CODE_NAME, REQ_LENGTH - CODE_LENGTH - BETWEEN_CODE_NAME);					
					strName.TrimRight(' ');

					for(int i = 0; i < (int)stHaveItem.vecItemCode.size(); i++)
					{
						if(strCode == stHaveItem.vecItemCode[i])
						{
							bAlreadyInset = TRUE;
							break;
						}
					}

					if(!bAlreadyInset)
					{
						stHaveItem.vecItemCode.push_back(strCode);
						stHaveItem.vecItemName.push_back(strName);
					}
				}
				m_listHaveItem.AddTail(stHaveItem);
			}
		}
	}
	return TRUE;
}

BOOL CConditionResultWnd::LookUpSearchCenterModify(CString& strType, CString& strUserFolder, CString& strUserFileName)
{
	CString strUserPath;
	strUserPath.Format("%s%s%s", Variant(homeCC), USRDIR, Variant(nameCC));

	CString strFileName = strUserPath + "\\SearchCenterModify.xml";

	BOOL bResult = TRUE;
	VARIANT_BOOL bLoadedXMLFile;

	DOCUMENT pDOMConditionIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	pDOMConditionIndex->async = false;

	//20120112 이문수 >>
	CFileFind ff;
	if(!ff.FindFile(strFileName))
		return FALSE;
	//20120112 이문수 <<

	bLoadedXMLFile = pDOMConditionIndex->load(_bstr_t(strFileName));

	if(bLoadedXMLFile == VARIANT_FALSE) 
	{
		bResult = FALSE;
		return bResult;
	}
	try
	{
		XMLELEMENT pParent = pDOMConditionIndex->documentElement;

		NODE pUserItem = pParent->firstChild;
		ATTRIBUTE pAttr = pUserItem->attributes;

		int i;
		for(i = 0; i < pAttr->length; ++i)
		{
			if(_stricmp(pAttr->item[i]->nodeName, "TYPE") == 0)
				strType = (char*)pAttr->item[i]->text;
			else if(_stricmp(pAttr->item[i]->nodeName, "FULLNAME") == 0)
			{
				strUserFileName = (char*)pAttr->item[i]->text;
				int nPos = strUserFileName.Find("\\");
				if(nPos >= 0)
				{
					strUserFolder = strUserFileName.Left(nPos);
					strUserFileName = strUserFileName.Mid(nPos + 1);
				}
				else
				{
					strUserFolder = "";
					strUserFileName = "";
				}
			}
		}
	}
	catch(_com_error e)
	{
		bResult = FALSE;
	}

	return bResult;
}

void CConditionResultWnd::SearchUserItem(CString& strUserFolder, CString& strFullName)
{
/*	CString strUserPath;
	IMainInfoManager* pMainInfoMng = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager, 1);
	if(pMainInfoMng)
		strUserPath = pMainInfoMng->GetEncUserDir();

	CString strFilePath = strUserPath + (CString)FOLDER_FINDDATA + "\\" + "\\strFullName;
*/
	XMLTreeInfoFile xmlTreeInfo;
	xmlTreeInfo.strType = "USER";
	xmlTreeInfo.strFolderPath = strUserFolder;
	xmlTreeInfo.strFilePath = strFullName;
	strFullName.Replace(".xml", "");
	xmlTreeInfo.strFileName = strFullName;
	xmlTreeInfo.bQuickReg = FALSE;
	xmlTreeInfo.bUnitCan = 1;
	xmlTreeInfo.strAlertReg = "TRUE";


	m_pwndMain->SendMessage(WM_XMLTREE_CTRL, (WPARAM)NM_LOAD_FILE, (LPARAM)&xmlTreeInfo);
}

CString CConditionResultWnd::GetSelectGridItemCode()
{
	CString strItemCode;
	int nMarkType;
	
	m_pGridCtrl->GetRowCode(m_pGridCtrl->m_nLastSelRow, strItemCode, nMarkType);

	return strItemCode;
}
void CConditionResultWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	Invalidate();
	CWnd::OnSettingChange(uFlags, lpszSection);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CConditionResultWnd::MakeUpCodeList(CStringArray* arrCode, CStringArray* arrName)
{
	int nIndex;
	int nFind;
	CString strData;
	CString strMarket, strType;
	CString strUpCodeL, strUpCodeM, strUpCodeS;
	for(nIndex = 0; nIndex < arrName->GetSize(); nIndex++)
	{
		strData = arrCode->GetAt(nIndex);

		nFind = strData.Find("|");
		// 20110628 박찬모 대상변경 업종 부분 필터링 오류 >>
		/*strMarket = strData.Mid(0, nFind);*/
		if(strData.Mid(0, nFind) == "1")
			strMarket = "0001";
		else if(strData.Mid(0, nFind) == "2")
			strMarket = "1001";
		strData = strData.Mid(nFind + 1);

		nFind = strData.Find("|");
		/*strType = strData.Mid(0, nFind);*/
		/* (0:제외 1:대 2:중 3:소)                      */
		switch(_ttoi(strData.Mid(0, nFind)))
		{
		case 0 : strType = "0000";	break;
		case 1 : strType = "0002";	break;
		case 2 : strType = "0003";	break;
		case 3 : strType = "0004";	break;
		}
		strData = strData.Mid(nFind + 1);
		// 20110628 박찬모 <<

		nFind = strData.Find("|");
		strUpCodeL = strData.Mid(0, nFind);
		strData = strData.Mid(nFind + 1);

		nFind = strData.Find("|");
		strUpCodeM = strData.Mid(0, nFind);
		strData = strData.Mid(nFind + 1);

		strUpCodeS = strData;
// 
		ST_UPCODE *pstUpCode = new ST_UPCODE;
		pstUpCode->strItemCode = arrName->GetAt(nIndex);
		pstUpCode->strMarket = strMarket;
		pstUpCode->strType = strType;
		pstUpCode->strUpCodeL = strUpCodeL;
		pstUpCode->strUpCodeM = strUpCodeM;
		pstUpCode->strUpCodeS = strUpCodeS;

		m_MapUpCode.SetAt(pstUpCode->strItemCode, pstUpCode);
	}
}

ST_UPCODE* CConditionResultWnd::GetUpCode(CString strItemCode)
{
	if(	m_MapUpCode.GetCount() == 0 )
		return NULL;

	ST_UPCODE* pstUpCode;
	if(m_MapUpCode.Lookup(strItemCode, (void*&)pstUpCode))
	{
		if(pstUpCode)
			return pstUpCode;
	}
	return NULL;
}

//20100827 이문수 >>
BOOL CConditionResultWnd::OnEraseBkgnd(CDC* pDC)
{
	
	CRect rcClient;
	GetClientRect(rcClient);
	pDC->FillSolidRect(rcClient, m_clrMainSkin);

	return FALSE;
}
//20100827 이문수 <<

// 20110118 박찬모 검색 결과 Double Click 를 통한 종목 연동 기능 >>
void CConditionResultWnd::OnDBClickResultGrid(CString strCode)
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	pwndMainFrm->OnDBClickResultGrid(strCode);
	m_pGridCtrl->SetFocus();
}
// 20110118 박찬모 <<

//>>0000207: 조건검색 실시간, 사용자의 Column 구성과 배치(크기)등의 최종상태 저장 및 복원 기능 추가
#define	szDoGridWidthSetSectKey "UserGridWidth"
void CConditionResultWnd::_DoGridWidthSet(int nOption)
{
	CUGCtrl*pGridCtrl = (CUGCtrl*)m_pGridCtrl;
	CMapStringToString UserColWidthsMap;
	LPCSTR szSectKey = szDoGridWidthSetSectKey;
	CString			   szUserMapFile;

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	CString szScreenCFG;
	szScreenCFG.Format("%s.cfg", pwndMainFrm->GetScreenKey());
	szUserMapFile = pwndMainFrm->GetUserFilePath(szScreenCFG);
	//szUserMapFile = pwndMainFrm->GetFilePath("FINDERINFO");

	if(nOption==0)
	{
		CFindData_GridUtil::LoadFromUserEnvFile(szUserMapFile, szSectKey, &UserColWidthsMap);
		if(UserColWidthsMap.GetCount()>0)
			CFindData_GridUtil::SetGridAllWidth(pGridCtrl, &UserColWidthsMap);
	}
	else if(nOption==1)
	{
		CFindData_GridUtil::GetGridAllWidth(pGridCtrl, &UserColWidthsMap);
		CFindData_GridUtil::SaveToUserEnvFile(szUserMapFile, szSectKey, &UserColWidthsMap);
	}
}
//<<

CString CConditionResultWnd::Variant(int nComm, CString strData)
{
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	return pwndMainFrm->Variant(nComm, strData);
}

void CConditionResultWnd::GetData_PortGroup(CStringArray& saKey, CStringArray& saGroupName)
{
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
#if(0)//tour2k
	pwndMainFrm->GetData_PortGroup(saKey, saGroupName);
#else
	saKey.Copy(pwndMainFrm->m_GwanSimGroup.strKey);
	saGroupName.Copy(pwndMainFrm->m_GwanSimGroup.strGroupName);
#endif
}

BOOL CConditionResultWnd::GetData_PortItem(CStringArray& saCode, CStringArray& saName, int nKey)
{
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	return pwndMainFrm->GetData_PortItem(saCode, saName, nKey);
}

BOOL CConditionResultWnd::GetUpMasterData(CStringArray *&psaCode, CStringArray *&psaName, int jgub)
{
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	return pwndMainFrm->GetUpMasterData(psaCode, psaName, jgub);
}

BOOL CConditionResultWnd::GetStockMasterData(CStringArray &arrCode, CStringArray &arrName, UINT Jgub, UINT Upgub)
{
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	return pwndMainFrm->GetStockMasterData(arrCode, arrName, Jgub, Upgub);
}

void CConditionResultWnd::SetBtnImg(CfxImgButton *pBtn, CString strFileName)
{
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	pwndMainFrm->SetBtnImg(pBtn, strFileName);
}

void CConditionResultWnd::SetBtnImg(CfxImgButton *pBtn, int nTextLen, BOOL bBold)
{
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	pwndMainFrm->SetBtnImg(pBtn, nTextLen, bBold);
}

CWnd* CConditionResultWnd::GetModuleMainWnd()
{
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	return pwndMainFrm->GetModuleMainWnd();
}

int CConditionResultWnd::SetResultTemp()
{
	CString strCode, strCodeName; 
	CStringArray strArrayCode, strArrayCodeName;
	
	int nDataCnt = m_pGridCtrl->GetNumberRows();
	if(nDataCnt>0)
	{
		int nRow;
		int nMarkType;
		for (nRow = 0; nRow < m_pGridCtrl->GetNumberRows(); nRow++)
		{
			m_pGridCtrl->GetRowCode(nRow, strCode, nMarkType);
			strCodeName = m_pGridCtrl->GetCellData(nRow, 0);
			
			strArrayCode.Add(strCode);
			strArrayCodeName.Add(strCodeName);
		}
		
		CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
		pwndMainFrm->SetUserMaster(usrMasterCondition, strArrayCode.GetSize(), &strArrayCode, &strArrayCodeName);
	}
	return 0;
}

void CConditionResultWnd::OnBtnLanguage()
{
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	pwndMainFrm->SendMessage(RMSG_CHECKMENU, (WPARAM)100, (LPARAM)0);
}

void CConditionResultWnd::SetBtnExpandImage()
{
	CMainFrmWnd *wndMainFrm = (CMainFrmWnd*)m_pwndMain;

	int nState = wndMainFrm->m_nTopExp;

	switch (nState)
	{
	case 0 :
		SetBtnImg(&m_btnExpand, "UP_EX");
		break;
	case 1 :
		SetBtnImg(&m_btnExpand, "DOWN_EX");
		break;
	case 2 :
		SetBtnImg(&m_btnExpand, "DOWN_EX");
		break;
	default :
		break;
	}
}
void CConditionResultWnd::GetAccNo(CStringArray* psaAccNo)
{
	CMainFrmWnd *wndMainFrm = (CMainFrmWnd*)m_pwndMain;
	wndMainFrm->GetAccNo(psaAccNo);
}

void CConditionResultWnd::RequestHaveItem(CString strAccNo)
{
	CMainFrmWnd *wndMainFrm = (CMainFrmWnd*)m_pwndMain;
	wndMainFrm->RequestHaveItem(strAccNo);
}

void CConditionResultWnd::SendHaveItem(CStringArray* psaItemCode)
{
	if (m_pDlgEditSubject)
	m_pDlgEditSubject->SendHaveItem(psaItemCode);
}

void CConditionResultWnd::SetTargetBit()
{
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	CStringArray saMSTCode;
	saMSTCode.Copy(((CConditionMakeWnd*)pwndMainFrm->GetWnd("MAKE"))->m_saMSTCode);

	if(saMSTCode.GetSize() <= 0)
		return ;

	memset(m_sTargetBit, 0x30, sizeof(m_sTargetBit));
	m_bJongFillterChk = TRUE;

	for(int i=0; i < m_arTargetResult.GetSize(); i++)
	{
		for(int j=0; j < saMSTCode.GetSize(); j++)
		{
			if(saMSTCode.GetAt(j) == m_arTargetResult.GetAt(i))
				m_sTargetBit[j] = '1';
		}
	}
}

void CConditionResultWnd::SetTargetBit(CStringArray& saExceptCode)
{
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	CStringArray saMSTCode;
	saMSTCode.Copy(((CConditionMakeWnd*)pwndMainFrm->GetWnd("MAKE"))->m_saMSTCode);
	
	memset(m_sTargetBit, 0x31, sizeof(m_sTargetBit));
	m_bJongFillterChk = TRUE;
	
	for(int i=0; i < saExceptCode.GetSize(); i++)
	{
		for(int j=0; j < saMSTCode.GetSize(); j++)
		{
			if(saMSTCode.GetAt(j) == saExceptCode.GetAt(i))
				m_sTargetBit[j] = '0';
		}
	}
}

void CConditionResultWnd::GetMonthSettlement(CStringArray* psaItemCode, int nMonth)
{
	psaItemCode->Copy(m_saMonth[nMonth - 1]);
}

COLORREF CConditionResultWnd::GetAxColor(int nColor)
{
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	return pwndMainFrm->GetAxColor(nColor);
}

void CConditionResultWnd::SetSaveDefalutChangeSubject()
{
	CString strUserPath, strSavePath, strTemp;
	strUserPath.Format("%s%s%s", Variant(homeCC), USRDIR, Variant(nameCC));
	strSavePath =  strUserPath + (CString)FOLDER_FINDDATA + "\\7127_ConditionInfo.dat";

//	::WritePrivateProfileString("Tree_Upjong", "AllKospi", "코스피", strSavePath);		//업종저장
//	::WritePrivateProfileString("Tree_Upjong", "AllKosdak", "코스닥", strSavePath);		//업종저장
	::WritePrivateProfileString("Tree_Upjong", "TargetItem", "전체", strSavePath);
	::WritePrivateProfileString("Tree_Upjong", "TargetParnetItem", "", strSavePath);
	::WritePrivateProfileString("Tree_Upjong", "TargetMText_0", "전체", strSavePath);	//최종으로 선택된 업종저장 
	::WritePrivateProfileString("Tree_Upjong", "TargetMText_Count", "1", strSavePath);	//최종으로 선택된 업종저장 
//	::WritePrivateProfileString("Tree_Upjong", "Select", "0", strSavePath);				//최종으로 선택된 업종저장 
	//::WritePrivateProfileString("Tree_Upjong", "Upjong_Index", "0000", strSavePath);	//업종저장
//	::WritePrivateProfileString("Tree_Upjong", "K200AndKRX", "", strSavePath);			//K200 KRX업종저장
//	::WritePrivateProfileString("Tree_Upjong", "Port_Index", "", strSavePath);			//관심저장 
	//::WritePrivateProfileString("Tree_Upjong", "Tema_Index", "", strSavePath);		//테마저장
	
	strTemp.Format("%d", m_bIssuesForAdministration);
	::WritePrivateProfileString("Except_JM", "Issue",  strTemp, strSavePath);
	
	strTemp.Format("%d", m_bDealStop);
	::WritePrivateProfileString("Except_JM", "CheckDealStop",  strTemp, strSavePath);
	
	strTemp.Format("%d", m_bTidyDeal);
	::WritePrivateProfileString("Except_JM", "TidyDeal",  strTemp, strSavePath);
	
	strTemp.Format("%d", m_bInsincerityNotice);
	::WritePrivateProfileString("Except_JM", "InsincerityNotice",  strTemp, strSavePath);
	
	strTemp.Format("%d", m_bAttentionInvestment);
	::WritePrivateProfileString("Except_JM", "AttentionInvestment",  strTemp, strSavePath);
	
	strTemp.Format("%d", m_bNoticeAttentionInvestment);
	::WritePrivateProfileString("Except_JM", "NoticeAttentionInvestment",  strTemp, strSavePath);
	
	strTemp.Format("%d", m_bPreferenceStock);
	::WritePrivateProfileString("Except_JM", "CheckPreferenceStock",  strTemp, strSavePath);
	
	strTemp.Format("%d", m_bDeposit);
	::WritePrivateProfileString("Except_JM", "CheckDeposit",  strTemp, strSavePath);
	
	strTemp.Format("%d", m_bDangerInvestment);
	::WritePrivateProfileString("Except_JM", "DangerInvestment",  strTemp, strSavePath);
	
	strTemp.Format("%d", m_bWarningInvestment);
	::WritePrivateProfileString("Except_JM", "WarningInvestment",  strTemp, strSavePath);
	
	strTemp.Format("%d", m_bETF);
	::WritePrivateProfileString("Except_JM", "ETF",  strTemp, strSavePath);
/*	
	strTemp.Format("%d", m_bETN);
	::WritePrivateProfileString("Except_JM", "ETN",  strTemp, strSavePath);
*/
	::WritePrivateProfileString("Month", "SelRadioMonth",  "0", strSavePath);
}

void CConditionResultWnd::SetEditSubjectTarGetBit(BOOL bRecvTR )
{
	UINT Jgub=0, Upgub=0;
	
	CStringArray arrMasterItemCode, arrTargetItemName;
	
	CString strTitle;
	if(m_strEditSubjectSelectParentItemText != "")
		strTitle.Format("%s - %s", m_strEditSubjectSelectParentItemText, m_strEditSubjectSelectItemText);
	else
		strTitle.Format("%s", m_strEditSubjectSelectItemText);

	Jgub = _ttoi(m_strJangGubun.Mid(0, 1)) * 100;

	if (strTitle.CompareNoCase("거래소") == 0)			Jgub = GETMARKET_KOSPI;
	else if (strTitle.CompareNoCase("코스닥") == 0)		Jgub = GETMARKET_KOSDAQ;
	else if (strTitle.CompareNoCase("KOSPI200") == 0)	Jgub = GETMARKET_KOSPI200;
	else if (strTitle.CompareNoCase("KRX100") == 0)		Jgub = GETMARKET_KRX;
	else
	{
		if (strTitle.CompareNoCase("전체") == 0)		Jgub = GETMARKET_ALL;

		Upgub = _ttoi(m_strJangGubun);
	}

	if (m_strEditSubjectSelectParentItemText == "관심종목")
	{
		if (bRecvTR)
		{
			CMainFrmWnd* pMainFrm = (CMainFrmWnd*)m_pwndMain;
			arrMasterItemCode.Copy(pMainFrm->m_arrInitCode);
		}
		else {
			GetData_PortItem(arrMasterItemCode, arrTargetItemName, _ttoi(m_strJangGubun));
			return;
		}
	}
	else if (m_strEditSubjectSelectParentItemText == "테마그룹")
		GetItemCodeFromThemeGroup(arrMasterItemCode, m_strJangGubun);
	else
		GetStockMasterData(arrMasterItemCode, arrTargetItemName, Jgub, Upgub);

	CStringArray arrTargetItemCode;
	
	CString strExceptDetail;
	for(int nIndex = 0; nIndex < arrMasterItemCode.GetSize(); nIndex++)
	{
		if(m_mapExeptItem.Lookup(arrMasterItemCode.GetAt(nIndex), strExceptDetail))
		{
			if ((strExceptDetail.GetLength() != 8) ||
				(m_bIssuesForAdministration		&& strExceptDetail.GetAt(0) == '1') ||	// 관리종목
				(m_bDealStop					&& strExceptDetail.GetAt(1) == '1') ||	// 거래정지
				(m_bTidyDeal					&& strExceptDetail.GetAt(2) == '1') ||	// 정리매매
				(m_bInsincerityNotice			&& strExceptDetail.GetAt(3) == '1') ||	// 불성실공시
				(m_bAttentionInvestment			&& strExceptDetail.GetAt(4) == '1') ||	// 투자주의
				(m_bWarningInvestment			&& strExceptDetail.GetAt(4) == '2') ||	// 투자경고
				(m_bDangerInvestment			&& strExceptDetail.GetAt(4) == '3') ||	// 투자위험
				(m_bNoticeAttentionInvestment	&& strExceptDetail.GetAt(4) == '4') ||	// 투자위험예고
				(m_bPreferenceStock				&& strExceptDetail.GetAt(5) == '1') ||	// 우선주
				(m_bDeposit						&& strExceptDetail.GetAt(6) == '1') ||	// 증거금
//				(m_bETN							&& strExceptDetail.GetAt(7) == '1') ||	// ETN
				(m_bETF							&& strExceptDetail.GetAt(7) == '1'))	// ETF
			{
				continue;
			}
		}
		
		arrTargetItemCode.Add(arrMasterItemCode.GetAt(nIndex));
	}
	
	CStringArray arrFilterMonthItemCode;
	if (m_nSelRadioMonth)
	{
		CStringArray saMonthSettlement;

		int nMonth;

		if (m_nSelRadioMonth == 1)			nMonth = 3;
		else if (m_nSelRadioMonth == 2)		nMonth = 6;
		else if (m_nSelRadioMonth == 3)		nMonth = 9;
		else if (m_nSelRadioMonth == 4)		nMonth = 12;
		else if (m_nSelRadioMonth == 5)		nMonth = m_nIndexMonthCombo + 1;

		GetMonthSettlement(&saMonthSettlement, nMonth);
		
		for (int nIndex = 0; nIndex < saMonthSettlement.GetSize(); nIndex++)
		{
			CString strItemCodeMonth;
			strItemCodeMonth = saMonthSettlement.GetAt(nIndex);

			for (int nTargetIndex = 0; nTargetIndex < arrTargetItemCode.GetSize(); nTargetIndex++)
			{
				if (strItemCodeMonth == arrTargetItemCode.GetAt(nTargetIndex))
				{
					arrFilterMonthItemCode.Add(arrTargetItemCode.GetAt(nTargetIndex));
					break;
				}
			}
		}
	}
	else
		arrFilterMonthItemCode.Copy(arrTargetItemCode);

	m_arTargetResult.RemoveAll();
	m_arTargetResult.Copy(arrFilterMonthItemCode);
	
	SetTargetBit();
}

void CConditionResultWnd::GetItemCodeFromThemeGroup(CStringArray &arrCode, CString strThemeIndex)
{
	arrCode.RemoveAll();

	CString sFile;
	sFile.Format("%s\\%s\\%s", Variant(homeCC), TABDIR, FILE_THEME);	
	
	for (int i = 0; ; i++)
	{
		char szBuf[BUFSIZ] = { 0x00, };
		CString sKey;
		
		if (i < 100)
			sKey.Format("%02d", i);
		else
			sKey.Format("%03d", i);
		
		GetPrivateProfileString("FOLDER", sKey, "", szBuf, sizeof(szBuf), sFile);
		
		if (strlen(szBuf) <= 0)
			break;
		
		CString sData = szBuf;
		
		if (sData.GetLength() < 33)
			continue;
		
		CString sName = sData.Left(30);
		CString sTheCode = sData.Right(3);
		
		sName.TrimLeft(); sName.TrimRight();
		sTheCode.TrimLeft(); sTheCode.TrimRight();
		
		if(strThemeIndex != sTheCode)
			continue;
		
		char szCList[BUFSIZ] = { 0x00, };
		GetPrivateProfileString("TMCODE", sTheCode, "", szCList, sizeof(szCList), sFile);
		
		CString sCList = szCList;
		while (1)
		{
			CString sCode = CUtils::Parser(sCList, ";");
			sCode.TrimLeft(); sCode.TrimRight();
			
			if (sCode.GetLength() <= 0)
				break;
			
			arrCode.Add(sCode);
		}
		
		if (arrCode.GetSize() <= 0)
		{
			return ;
		}
		
		break;
	}
}