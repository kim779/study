// ConditionMakeWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "ConditionMakeWnd.h"

#include "MainFrmWnd.h"
#include "PortPolioDelDlg.h"
#include "SaveDlg.h"
#include "MatchValEditDlg.h"

#include "../../DrCommon/CMGridCtrl/CMGrid.h"
#include "AddControl/CheckTreeCtrl.h"

#define TREE_MARKET				0
#define TREE_PORTFOLIO			1
#define TREE_RECOMMENDATION		2

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE g_hInstance;

/////////////////////////////////////////////////////////////////////////////
// CConditionMakeWnd

CString	m_strConstVersion = "V 1.02";

short m_nConstGridColWidth[] = { 35, 60, 387, 30, 30, 30 };  //<= 가로 840 기준
int		m_nConstGridCol				 = 6;

const		LPCTSTR		BACKTEST_TIME[] = { "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "15:30" };

#define GRID_FOOTDEFULT_DATA1       1
#define GRID_FOOTDEFULT_DATA2       2

//## EzFinder Use ##
#define MAP_SIZE_WIDTH				500
#define MAP_SIZE_HEIGHT				105

#define NEWS_TYPE					8000

//# Geid Col Define 
#define COL_ELEMENT						0
#define COL_TARGET_COMBO			1
#define COL_TEXT							2
#define COL_VALVIEW_CHK				3
#define COL_MODIFY_BTN				4
#define COL_DELETE_BTN				5

#define LOGIC_EDIT_HEIGHT           24

CConditionMakeWnd::CConditionMakeWnd()
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);
	
//	m_brushStatic.CreateSolidBrush(COLOR_MAIN_CANVAS);
//	m_brushMapLoad.CreateSolidBrush(COLOR_MAPLOAD);

// KB투자증권
	m_brushStatic.CreateSolidBrush(RGB(232, 226, 210));
	m_brushMapLoad.CreateSolidBrush(RGB(232, 226, 210));
	m_brushTagetEdit.CreateSolidBrush(RGB(255, 255, 255));

	
	m_saWorkDate.RemoveAll();
	m_saMSTCode.RemoveAll();
	
	m_pwndMain = NULL;
	m_bCreateEnd = FALSE;
	
	m_hConditionMng = NULL;
	
	m_pGridCtrl = NULL;
	m_nSelectTab = 0;
	m_nGridTipMaxLength = 0;
	m_nLogicTipMaxLength = 0;
	
	
	m_bSaveFlag = FALSE;
	m_ptabSet = NULL;
	
	m_nBackDateMaxCnt = -1;
	m_bSelectCond = TRUE;

	m_hChildFrame = NULL;
	m_clrMapLoad = COLOR_MAPLOAD;
	m_nTabIndex = 0;

	m_bCommonExStateResizing = FALSE;

	m_nSelectedTargetBase = 0; // 20110222 박찬모 사용자 조건 오름차순 정렬	>>

	// 20110623 박찬모 일반검색시에는 CommonLigicEdit, 순차검색시에는 OrderLoginEdit를 보여줌 >>	
	m_pShowLogicEdit = NULL;
	m_pShowLogicStatic = NULL;
	// 20110623 박찬모 <<

	m_pShowPortfolioEdit = NULL;

	m_nTotCntCondition = 0;
	m_nSelectCondition = 0;

	m_pTabConditionForItemExcavation = NULL;
	m_bLoadConditionForItemExcavation = FALSE;

	m_bInitIng = FALSE;
}

CConditionMakeWnd::~CConditionMakeWnd()
{
	m_fontDefault.DeleteObject();
	m_fontBold.DeleteObject();
	
	m_brushStatic.DeleteObject();
	m_brushMapLoad.DeleteObject();
	m_brushTagetEdit.DeleteObject();
	m_brushTab.DeleteObject();
	
	m_saMSTCode.RemoveAll();
}

BEGIN_MESSAGE_MAP(CConditionMakeWnd, CWnd)
	//{{AFX_MSG_MAP(CConditionMakeWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP

	ON_BN_CLICKED(IDC_LEFTEXP_BTN, OnBtnExpand)
	ON_CBN_SELCHANGE(IDC_TARGETBASE_COMBO, OnChangeTargetBase)
	ON_BN_CLICKED(IDC_GRIDTIP_BTN, OnBtnGridTip)
	ON_BN_CLICKED(IDC_BTNCHK_BACKTEST, OnBtnBackTestChk)
	ON_BN_CLICKED(IDC_DETAIL_BTN, OnBtnDetail)
	ON_BN_CLICKED(IDC_INITIAL_BTN, OnBtnInit)
	ON_BN_CLICKED(IDC_BTN_ALERTCHK, OnBtnAlertChk)

	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SET, OnChangeTab)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CONDITION_ITEMEXCAVATION, OnChangeConditionTab)

	ON_BN_CLICKED(IDC_COMMONEXSIZE_BTN, OnBtnCommonExSize)
	ON_BN_CLICKED(IDC_ORDEREXSIZE_BTN, OnBtnOrderExSize)
	ON_BN_CLICKED(IDC_BTN_MATCHVALEDIT, OnBtnMatchVal)
	ON_BN_CLICKED(IDC_BRACKET_BTN, OnBtnBracket)
	ON_BN_CLICKED(IDC_BRACKETDEL_BTN, OnBtnBracketDel)
	ON_BN_CLICKED(IDC_PORTDEL_BTN, OnBtnPortDel)
	ON_BN_CLICKED(IDC_LOGICTIP_BTN, OnBtnLogicTip)
	ON_BN_CLICKED(IDC_USERSAVE_BTN, OnBtnFileSave)
	ON_BN_CLICKED(IDC_PORTCHECK_BTN, OnBtnPortCheck)
	ON_BN_CLICKED(IDC_BTN_MATCHVALCHECK, OnBtnMatchValCheck)
	ON_BN_CLICKED(IDC_BTN_DEFAULT_VALUE, OnBtnDefaultValue)
	ON_BN_CLICKED(IDC_BTN_NEXT_CONDITION, OnBtnNextCondition)
	ON_BN_CLICKED(IDC_BTN_PRE_CONDITION, OnBtnPreCondition)
	ON_BN_CLICKED(IDC_SEARCHRNG, OnBtnSearchRng)

	ON_MESSAGE(WM_NOTIFY_MAPLOAD, OnNotifyMapLoad)
	ON_MESSAGE(WM_LOGIC_EDIT, OnNotifyLogicEdit)
	ON_MESSAGE(WM_NOTIFY_CMGRIDEVENT, OnNotifyGridEvent)

//	ON_MESSAGE(UM_CHANGE_PARENT, OnChangeChildFrameWindowHandle)	-hidden-
	ON_WM_SETTINGCHANGE()
	ON_WM_ERASEBKGND() //20100827 이문수 >>

	ON_MESSAGE(NM_SETBTNIMG, OnMGridSetBtnImg)
	ON_MESSAGE(UMSG_LANGUAGE_RELOAD, OnLanguageReload)
END_MESSAGE_MAP()

#include "../../DrCommon/CMGridCtrl/GridCommonView.h"// : header file
void CConditionMakeWnd::OnChangeTargetBase()
{	
	int nSelect = m_comboTargetBase.GetCurSel();
	
	m_nTargetBaseIndex = nSelect;
	
	if (nSelect > 0)  //# Not 개별 적용
	{
		m_DataAdmin.ModifyConditionTargetBase(0, nSelect - 1);
		m_pGridCtrl->SetSameTargetBase(nSelect - 1);
	}
	else if (nSelect == 0)
		m_pGridCtrl->SetSameTargetBase(-1);

	m_pGridCtrl->m_pViewCommonGrid->Invalidate();
}

void CConditionMakeWnd::OnBtnGridTip()
{
//@유진_소리마치 테스트
//@유진_소리마치 테스트	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
//@유진_소리마치 테스트	((CConditionResultWnd*)pwndMainFrm->GetWnd("RESULT"))->SetSearch_MSTCode();
//@유진_소리마치 테스트	return;
	DlgHelp dlg(this, m_strGridInfoTip);
	if(dlg.DoModal() == IDOK)
	{
		
	}
// 	m_tipGridInfo.UpdateToolTip(this, (LPCTSTR)m_strGridInfoTip, m_nGridTipMaxLength);
// 	m_tipGridInfo.Show(m_strGridInfoTip);
}

void CConditionMakeWnd::OnBtnExpand()
{
	CMainFrmWnd *wndMainFrm = (CMainFrmWnd*)m_pwndMain;
	
	BOOL bState = wndMainFrm->m_bLeftExp;
	if (bState == TRUE)
		SetBtnImg(&m_btnExpand, "RIGHT_EX");
	else
		SetBtnImg(&m_btnExpand, "LEFT_EX");
	
	m_pwndMain->SendMessage(WM_CONDITIONMAKE_WND, (WPARAM)NM_SCREEN_EXP, (LPARAM)!bState);

// 	int nX, nY;
// 	CRect rt;
// 	m_btnExpand.GetWindowRect(rt);
// 	nX = rt.left + rt.Width()/2;
// 	nY = rt.top + rt.Height()/2;
// 
// 	SetCursorPos(nX, nY);

	Invalidate();
}

void CConditionMakeWnd::OnBtnBackTestChk()
{
	if (m_DataAdmin.GetConditionCnt() <= 0)
	{
		m_btnBackTestChk.SetCheck(FALSE);
		return;
	}
	
	if (m_btnBackTestChk.GetCheck() == TRUE)
	{
		// 20110315 박찬모 과거시점 검색시 대상시간 "개별"로만 설정 >>
		if(m_nSelectTab != 1)
		{
			m_nSelectedTargetBase = m_comboTargetBase.GetCurSel();
			m_comboTargetBase.SetCurSel(0);
			OnChangeTargetBase();
			m_comboTargetBase.EnableWindow(FALSE);
			m_comboTargetBase.Invalidate();
		}
		
		//# Back Test Date Set!
		/*int nMinTargetBase = m_DataAdmin.GetMinTargetBase();*/
		int nMinTargetBase = 0;
		if (nMinTargetBase != 100)		
			SetBackDateSearch(m_saBackDateCnt.GetAt(nMinTargetBase));
		// 20110315 박찬모 과거시점 검색시 대상시간 "개별"로만 설정 <<
		
		//# Back Test!
		for (int nIndex = 0; nIndex < _countof(BACKTEST_TIME); nIndex++)
			m_comboBackTestTime.AddString(BACKTEST_TIME[nIndex]);
		m_comboBackTestTime.SetCurSel(0);
	}
	else
	{
		m_comboBackTestDay.ResetContent();
		m_comboBackTestTime.ResetContent();

		// 20110315 박찬모 과거시점 검색시 대상시간 "개별"로만 설정 >>
		if(m_nSelectTab != 1)
		{
			m_comboTargetBase.SetCurSel(m_nSelectedTargetBase);
			OnChangeTargetBase();
			m_comboTargetBase.EnableWindow(TRUE);
			m_comboTargetBase.Invalidate();
		}
		// 20110315 박찬모 과거시점 검색시 대상시간 "개별"로만 설정 <<
	}
	
	m_comboBackTestDay.EnableWindow(m_btnBackTestChk.GetCheck());
	m_comboBackTestTime.EnableWindow(m_btnBackTestChk.GetCheck());
}

void CConditionMakeWnd::OnBtnInit()
{
	m_bInitIng = TRUE;
	
	m_bMapShowState = FALSE;
	
	m_nElementIndex = 0;
	
	SetFileSaveState(FALSE);
	SetSendFileName(FALSE);
	
	m_comboTargetBase.SetCurSel(0);
	m_comboTargetBase.EnableWindow(FALSE);
	m_nTargetBaseIndex = 0;
	
	if (m_saWorkDate.GetSize() > 0)
	{
		//# Back Test!
		m_btnBackTestChk.SetCheck(FALSE);
		
		m_comboBackTestDay.EnableWindow(FALSE);
		m_comboBackTestTime.EnableWindow(FALSE);
		
		m_comboBackTestDay.ResetContent();
		m_comboBackTestTime.ResetContent();
	}
	
	m_btnAlertChk.SetCheck(FALSE);
	
	//# Logic Edit All Clear
	m_ctrlCommonLogicEdit.DeleteAll();
	m_ctrlOrderLogicEdit.DeleteAll();
	
	//# Port Edit All Clear
	m_ctrlPortEdit.DeleteAll();
	//m_ctrlGwansimEdit.DeleteAll();
	m_btnPortCheck.SetCheck(TRUE);
	
	int nSelectGroup;
	if ((m_nSelectTab == 0) || (m_nSelectTab == 2))
	{
		nSelectGroup = 0;
		m_pGridCtrl->SetInit((int)TYPE_GRID_COMMON, FALSE);
	}
	else if (m_nSelectTab == 1)
	{
		nSelectGroup = 1;
		m_pGridCtrl->SetInit((int)TYPE_GRID_ORDER, FALSE);
		m_DataAdmin.ModifyConditionInsertDownGroup(1, 1); // 일반 -> 일반+순차 탭 이동 시 순차 그룹 1 삽입해야함
		m_ctrlOrderLogicEdit.ModifyElementInsertDownGroup(1, 1);
	}
	
	if (m_pGridCtrl->GetDetailViewState() == TRUE)
		m_btnDetail.SetWindowText("일반설정");
	else
		m_btnDetail.SetWindowText("상세설정");
	m_btnDetail.Invalidate();
	m_btnDetail.ShowWindow(SW_HIDE);	//@유진은 버튼을 보일필요가 없다.
										// 리사이징 처리시에도 처리필요함.
	
	m_pGridCtrl->SetSelectGroup(m_nSelectTab, FALSE);
	SetSelectGroup(nSelectGroup);
	m_pwndMain->SendMessage(WM_CONDITIONMAKE_WND, (WPARAM)NM_CHANGE_CONDITION, (WPARAM)nSelectGroup);
	
	//## Data All Clear ##
	m_DataAdmin.DeleteAll();	
	
	//## Common/Order Logic Edit Size ##
	m_ctrlMapLoad.DestoryAllCtrl();
	
	//# Match Val!
	POSITION pos;
	MatchInfo	stMatchInfo;
	int nIndexPos = 0;
	int nDataCnt = m_listMatchInfo.GetCount();
	for (int nIndex = 0; nIndex < nDataCnt; nIndex++)
	{
		pos = m_listMatchInfo.FindIndex(nIndexPos);
		stMatchInfo = m_listMatchInfo.GetAt(pos);
		
		if (stMatchInfo.strElement.IsEmpty() == FALSE)
			SetMatchLogic_Delete(stMatchInfo.strElement, FALSE);
		else
			nIndexPos++;
	}
	m_ctrlMatchValEdit.SetWindowText("");
	m_pwndMain->SendMessage(WM_CONDITIONMAKE_WND, (WPARAM)NM_INIT, (LPARAM)nSelectGroup);
	Invalidate();

	//CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	//if(((CConditionResultWnd*)pwndMainFrm->GetWnd("RESULT"))->m_nSeparateForm != 0)
	if(m_nSeparateForm != 0)
		m_bSelectCond = FALSE;

	if(!m_bOneClickInitForSearch) //0200 ~ 0209 조건 원클릭 선택시 EnableWindow 반복 안되게 처리
		m_btnBackTestChk.EnableWindow(FALSE);

	m_bInitIng = FALSE;
}

void CConditionMakeWnd::OnBtnDetail()
{
	m_pGridCtrl->SetDetailViewState(!m_pGridCtrl->GetDetailViewState());
	
	if (m_pGridCtrl->GetDetailViewState() == TRUE)
		m_btnDetail.SetWindowText("일반설정");
	else
		m_btnDetail.SetWindowText("상세설정");
}

void CConditionMakeWnd::OnBtnAlertChk()
{
	BOOL bState = m_btnAlertChk.GetCheck();
	
	if (bState == TRUE)
		SetStandbyAlert();
}

BOOL CConditionMakeWnd::SetStandbyAlert()
{
	BOOL bResult = FALSE;
	
	BOOL bAlertFlag = TRUE;
	CString strMessage = "";
	
	int nIndex;
	CStringArray saElement;
	if (m_DataAdmin.GetNonAlertCondiHave(&saElement) == TRUE)
	{
		for (nIndex = 0; nIndex < saElement.GetSize(); nIndex++)
		{
			strMessage += saElement.GetAt(nIndex);
			
			if (nIndex < saElement.GetSize() - 1)
				strMessage += ", ";
		}
		
		strMessage += "는 실시간 감시를 지원하지 않는 조건입니다.\n해당 조건을 삭제하시겠습니까?";
		CString strTemp;
		if(m_nSeparateForm == 0)
			strTemp = (CString)PROGRAM_TITLE;
		else
			strTemp = (CString)PROGRAM_TITLE_BRIGHT;
		if (SetMessageBox(strMessage, strTemp, MB_OKCANCEL) == IDOK)
		{
			m_DataAdmin.SetNonAlertCondiDelete();
			
			//# UpDate MakeGrid!
			int nGroup, nRow;
			BOOL bState;
			for (nIndex = 0; nIndex < saElement.GetSize(); nIndex++)
			{
				if (GetGridPos_Element(saElement.GetAt(nIndex), nGroup, nRow) == TRUE)
				{
					if (nGroup == 0)
						bState = m_ctrlCommonLogicEdit.DeleteElement(saElement.GetAt(nIndex), TRUE);
					else if (nGroup > 0)
						bState = m_ctrlOrderLogicEdit.DeleteElement(saElement.GetAt(nIndex), TRUE);
					
					if (bState = FALSE)
					{
						if(m_nSeparateForm == 0)
							SetMessageBox("지표 삭제시 에러", PROGRAM_TITLE, MB_OK);
						else
							SetMessageBox("지표 삭제시 에러", PROGRAM_TITLE_BRIGHT, MB_OK);

						return bResult;
					}
					
					m_pGridCtrl->SetDeleteCondition(nRow, nGroup);
				}
			}
			
			bAlertFlag = TRUE;
		}
		else
			bAlertFlag = FALSE;
	}
	
	if (bAlertFlag == TRUE)
	{
		if (m_DataAdmin.GetConditionViewCnt() == 0)
			bResult = FALSE;
		else if (m_DataAdmin.GetOnlyTargetCondition() == TRUE)
		{
			strMessage = "대상지정 조건만으로는 실시간 감시를 할 수 없습니다.";
			if(m_nSeparateForm == 0)
				SetMessageBox(strMessage, PROGRAM_TITLE, MB_OK);
			else
				SetMessageBox(strMessage, PROGRAM_TITLE_BRIGHT, MB_OK);
			
			bAlertFlag = FALSE;
		}
		else
			bResult = SetSaveFile("", "temp");
	}
	
	if (((bAlertFlag == FALSE) || (bResult == FALSE)) || ((bAlertFlag == TRUE) && (bResult == TRUE)))
		m_btnAlertChk.SetCheck(bAlertFlag);
	
	return bResult;
}

//void CConditionMakeWnd::OnNotifyMapLoad(WPARAM wParam, LPARAM lParam)
LRESULT CConditionMakeWnd::OnNotifyMapLoad(WPARAM wParam, LPARAM lParam)
{
	if ((NM_MAPLOAD_OK == wParam) || (NM_MAPLOAD_CLOSE == wParam))
		SetShowMapLoader(FALSE);
	else if (NM_MAPLOAD_UPDATE == wParam)
	{
		if(!m_bSelectCond) return FALSE;

		int nGroup = m_stSelectCondition.nGroup;
		
		User_Define stResultUserDefine;
		CString			strResultComposition;
		BOOL bHaveTerm = FALSE;
		if (m_ctrlMapLoad.GetMapValuesToStruct(m_stSelectCondition.strTag, 
			stResultUserDefine, 
			strResultComposition,
			bHaveTerm, nGroup, m_stSelectCondition.strElement) == TRUE)
		{
			OneConditionInfo  stOneConditionInfo;
			m_DataAdmin.GetElementToOneConditionInfo(m_stSelectCondition.strElement, stOneConditionInfo);
			
 			if (m_stSelectCondition.strTag == "LANGUAGE")
			{
				//SetMessageBox("Language Edit Run!", PROGRAM_TITLE, MB_OK);
				//SetShow_LanguageEdit(stOneConditionInfo.strName);
				return FALSE;
			}
			
			if ((stResultUserDefine.ETerm < 1) || (stResultUserDefine.ETerm > 99))	//161013 강수현: [7127] "봉 이내" 의 제한값을 20에서 99까지 입력 가능하도록 범위 확대요청
			{
				if(m_nSeparateForm == 0)
					SetMessageBox("봉이내는 1 ~ 99까지만 입력가능합니다.", (CString)PROGRAM_TITLE, MB_OK);
				else
					SetMessageBox("봉이내는 1 ~ 99까지만 입력가능합니다.", (CString)PROGRAM_TITLE_BRIGHT, MB_OK);
				return FALSE;
			}
			else if ((stResultUserDefine.STerm < 0) || (stResultUserDefine.STerm > 399))
			{
				if(m_nSeparateForm == 0)
					SetMessageBox("봉전기준은 0 ~ 399까지만 입력가능합니다.", (CString)PROGRAM_TITLE, MB_OK);
				else
					SetMessageBox("봉전기준은 0 ~ 399까지만 입력가능합니다.", (CString)PROGRAM_TITLE_BRIGHT, MB_OK);
				return FALSE;
			}
			
			if (m_stSelectCondition.strTag == stOneConditionInfo.strTag)
			{
				m_DataAdmin.ModifyConditionUserDefine(m_stSelectCondition.strElement, 
					stResultUserDefine,
					strResultComposition);
				
				// 20110216 박찬모 관심종목 mgridctrl에 표시 >>
				if(stOneConditionInfo.strTag == "PORT")
				{
					strResultComposition = m_stSelectCondition.strText;
				}
				// 20110216 박찬모 관심종목 mgridctrl에 표시 <<
				
				
				m_pGridCtrl->SetModifyCondition(m_stSelectCondition.nRow, m_stSelectCondition.nGroup,
					(stResultUserDefine.Reverse == '1') ? TRUE : FALSE,		//# Not State 
					strResultComposition,				//# 완성 구문
					stOneConditionInfo.bFieldVal);
				
				SetFileSaveState(FALSE);
			}
		}
		
		SetShowMapLoader(FALSE);
	}

	return TRUE;
}

void CConditionMakeWnd::SetShowMapLoader(BOOL bView, int nGroup, int nRow, CPoint pPosition)
{
	if (bView == TRUE)
	{
		OneConditionInfo  stOneConditionInfo;
		CString strElement = m_pGridCtrl->GetElementInfo(nGroup, nRow);
		
		if (m_DataAdmin.GetElementToOneConditionInfo(strElement, stOneConditionInfo) == FALSE)
		{
			m_ctrlMapLoad.DestoryAllCtrl();
			
			if(m_nSeparateForm == 0)
				SetMessageBox("저장 데이터 오류.\n 초기화 후 사용하십시오.", (CString)PROGRAM_TITLE, MB_OK);
			else
				SetMessageBox("저장 데이터 오류.\n 초기화 후 사용하십시오.", (CString)PROGRAM_TITLE_BRIGHT, MB_OK);

			return;
		}
		
		//# Select Condition Info!
		m_stSelectCondition.nGroup = nGroup;
		m_stSelectCondition.nRow = nRow;
		m_stSelectCondition.strElement = strElement;
		m_stSelectCondition.strTag = stOneConditionInfo.strTag;
		
// 		char ch;
// 		ch = strElement.GetAt(0);
// 		m_stSelectCondition.nIndex = (int)ch;
// 
// 		if(nGroup > 0)
// 			m_ctrlOrderLogicEdit.MakeGroupStruct(m_stSelectCondition.nIndex, nGroup);
		
		XMLTreeInfo stXMLTreeInfo;
		
		int nItemType;
		CONVERT_TAGTOTYPE(stOneConditionInfo.strTag, nItemType);
		if (nItemType == -1)
		{
			if (nGroup == 0)
				stXMLTreeInfo.nItemType = (int)TREE_SELECT_TYPE_ITEM_COMM;
			else
				stXMLTreeInfo.nItemType = (int)TREE_SELECT_TYPE_ITEM_ORDER;
		}
		else
			stXMLTreeInfo.nItemType = nItemType;
		
		stXMLTreeInfo.strName = stOneConditionInfo.strName;
		stXMLTreeInfo.strTag = stOneConditionInfo.strTag;
		
		char *pBuff;
		pBuff = new char[sizeof(stOneConditionInfo.stCondition.IndexType) + 1];
		memset(pBuff, 0x00, sizeof(stOneConditionInfo.stCondition.IndexType) + 1);
		memcpy(pBuff, &stOneConditionInfo.stCondition.IndexType, sizeof(stOneConditionInfo.stCondition.IndexType));
		stXMLTreeInfo.nCondiType = atoi(pBuff);
		delete[] pBuff;
		
		stXMLTreeInfo.nTargetBase_Type = stOneConditionInfo.nTargetBaseType;
		stXMLTreeInfo.nGubun = stOneConditionInfo.nGubunUserFlag;
		
		pBuff = new char[sizeof(stOneConditionInfo.stCondition.IdxContiGubun) + 1];
		memset(pBuff, 0x00, sizeof(stOneConditionInfo.stCondition.IdxContiGubun) + 1);
		memcpy(pBuff, &stOneConditionInfo.stCondition.IdxContiGubun, sizeof(stOneConditionInfo.stCondition.IdxContiGubun));
		stXMLTreeInfo.nGubunDefVal = atoi(pBuff);
		delete[] pBuff;
		
		stXMLTreeInfo.nNotUser = stOneConditionInfo.nNotUserFlag;
		
		pBuff = new char[sizeof(stOneConditionInfo.stCondition.Reverse) + 1];
		memset(pBuff, 0x00, sizeof(stOneConditionInfo.stCondition.Reverse) + 1);
		memcpy(pBuff, &stOneConditionInfo.stCondition.Reverse, sizeof(stOneConditionInfo.stCondition.Reverse));
		stXMLTreeInfo.nNotDefVal = atoi(pBuff);
		delete[] pBuff;
		
		CString strID;
		HeadInfoType		stHeadInfoType;
		for (int nIndex = 0; nIndex < stOneConditionInfo.stHeadMoveInfo.nColCnt; nIndex++)
		{
			stHeadInfoType = stOneConditionInfo.stHeadMoveInfo.stHeadType[nIndex];
			
			strID.Format("%d", stHeadInfoType.lHeadID);
			stXMLTreeInfo.strResultViewID += strID + "/";
		}
		stXMLTreeInfo.nResultViewDefVal = stOneConditionInfo.stHeadMoveInfo.bView;
		stXMLTreeInfo.strMapFile = stOneConditionInfo.strMapFile;
		
		if (m_ctrlMapLoad.SetDrewConditionMap(&stXMLTreeInfo, nGroup, TRUE, TRUE, strElement) == TRUE)
		{
			m_ctrlMapLoad.SetStructToMapValues(stOneConditionInfo.strTag, &stOneConditionInfo.stCondition);
			
			SetNowXMLTreeInfo(&stXMLTreeInfo);
		}
		else
		{
			if(m_nSeparateForm == 0)
				SetMessageBox("선택한 지표의 Map 파일이 존재 하지 않습니다.\n"
				"고객 센터로 문의 하시기 바랍니다.", (CString)PROGRAM_TITLE, MB_OK);
			else
				SetMessageBox("선택한 지표의 Map 파일이 존재 하지 않습니다.\n"
				"고객 센터로 문의 하시기 바랍니다.", (CString)PROGRAM_TITLE_BRIGHT, MB_OK);

			SetNowXMLTreeInfo(NULL);
			return;
		}
		
		if (m_ptabSet->GetCurSel() == 1)
		{
			m_ptabSet->SetCurSel(0);
			SetTabView(0);
		}
		
		m_bMapShowState = TRUE;
	}
	else
		m_bMapShowState = FALSE;
}

BOOL CConditionMakeWnd::SetShow_LanguageEdit(CString strLanguageName)
{
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	CString strAppPath;
	strAppPath.Format("%s", pwndMainFrm->Variant(homeCC));
	CString strFilePath = strAppPath + "\\dev\\SystemTrading.exe";
	
	CWnd* pWndCompiler = FindWindow(NULL, _T("SystemTrading"));
	COPYDATASTRUCT copyData;
	if(pWndCompiler)
	{
		strLanguageName += ".sts";

		copyData.dwData = 1000;
		copyData.cbData = ::lstrlen(strLanguageName) + sizeof(DWORD);
		copyData.lpData = (void*)(LPCSTR)strLanguageName;

		pWndCompiler->SendMessage(WM_COPYDATA, 0,(LPARAM)&copyData);

		pWndCompiler->SetForegroundWindow();
	}
	else
	{
		HINSTANCE hInst;
		//char szParam[BUFSIZ];
	//	char szUserPath[BUFSIZ], szParam[BUFSIZ];
	//	::GetEnvironmentVariable(KEY_USERDIRECTORY, szUserPath, BUFSIZ);

		CString szUserPath;
		szUserPath.Format("%s%s%s", pwndMainFrm->Variant(homeCC), USRDIR, pwndMainFrm->Variant(nameCC));
		
		if (strLanguageName.IsEmpty())
		{
			HWND hWnd = GetSafeHwnd();
			
			CString strdata;
			strdata.Format("/U\"%s\"", szUserPath);
			
			CString szOption;
			szOption.Format("%s|%08X", strdata, hWnd);
			
			hInst = ::ShellExecute(NULL, "open", (LPCTSTR)strFilePath, szOption, NULL, SW_SHOW);
		}
		// 2011.11.24 박성경: 랭귀지 검색 더블클릭시 언어편집기 실행안되게함
		else
		{			
			/*::sprintf(szParam, "/U\"%s\" /S\"%s.sts\"", (CString)szUserPath, strLanguageName);*/

			HWND hWnd = GetSafeHwnd();
			
			CString strdata;
			strdata.Format("/U\"%s\" /S\"%s.sts\"", szUserPath, strLanguageName);

			CString szOption;
			szOption.Format("%s|%08X", strdata, hWnd);

			hInst = ::ShellExecute(NULL, "open", (LPCTSTR)strFilePath, szOption, NULL, SW_SHOW);
		}
		
		if (32 >= (int)hInst)
		{
			TRACE0 ("Error Open ExpManager!!! \n");
			return FALSE;
		}
	}
		
	return TRUE;
}


//void CConditionMakeWnd::OnNotifyLogicEdit(WPARAM wParam, LPARAM lParam)
LRESULT CConditionMakeWnd::OnNotifyLogicEdit(WPARAM wParam, LPARAM lParam)
{
	if (NM_EDIT_OPTCHANG == wParam)
	{
		CStringArray* psaInfoData = (CStringArray*)lParam;
		TRACE("\n# Logic Edit Opt Change Info");
		TRACE("\n   => Element : " + psaInfoData->GetAt(0));
		TRACE("\n   => Opt     : " + psaInfoData->GetAt(1));
		
		m_DataAdmin.ModifyConditionLogicOpt(psaInfoData);
	}

	return TRUE;
}


LRESULT CConditionMakeWnd::OnNotifyGridEvent(WPARAM wParam, LPARAM lParam)
{
	CString strTemp;
	CString strElement;
	CString strElementAfter;
	
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	pwndMainFrm->ListViewSearchEditReset();

	switch(wParam)
	{
		case NM_SELECT_GROUP :	//# Group
			{
				m_bSelectCond = FALSE;	// 그룹변경, 조건은 아직 선택전.
				
				int nGroup = HIWORD((int)lParam);
				int nIndex = LOWORD((int)lParam);

				strTemp.Format("\n## 그룹 선택! \n Group : %d", nGroup);
				OutputDebugString(strTemp);

//				if (((int)lParam == 0) || (m_stSelectCondition.nGroup == 0))
				if (((int)nGroup == 0) || (m_stSelectCondition.nGroup == 0))
				{
					//# 지표 전환
//					m_pwndMain->SendMessage(WM_CONDITIONMAKE_WND, (WPARAM)NM_CHANGE_CONDITION, (WPARAM)lParam);
					m_pwndMain->SendMessage(WM_CONDITIONMAKE_WND, (WPARAM)NM_CHANGE_CONDITION, (WPARAM)nGroup);
				}

				m_stSelectCondition.nGroup = nGroup;
				m_stSelectCondition.nIndex = nIndex;
				m_stSelectCondition.nRow = -1;
				m_stSelectCondition.strElement = "";
				m_stSelectCondition.strTag = "";
				
				if(m_nTabIndex == 0)
				{
					if(m_stSelectCondition.nGroup == 0)
					{
						m_ctrlOrderLogic.ShowWindow(SW_HIDE);
						m_ctrlOrderLogicEdit.ShowWindow(SW_HIDE);
						
						m_ctrlCommonLogic.ShowWindow(SW_SHOW);
						m_ctrlCommonLogicEdit.ShowWindow(SW_SHOW);
					}
					else
					{
						m_ctrlCommonLogic.ShowWindow(SW_HIDE);
						m_ctrlCommonLogicEdit.ShowWindow(SW_HIDE);
						
						m_ctrlOrderLogic.ShowWindow(SW_SHOW);
						m_ctrlOrderLogicEdit.ShowWindow(SW_SHOW);
					}
				}
				else
				{
					m_ctrlCommonLogic.ShowWindow(SW_SHOW);
					m_ctrlCommonLogicEdit.ShowWindow(SW_SHOW);
					m_ctrlOrderLogic.ShowWindow(SW_SHOW);
					m_ctrlOrderLogicEdit.ShowWindow(SW_SHOW);
				}	
			}
			break;
			
		case NM_SELECT_CONDITION : //# (0) : Group, (1) : Row, (2) : Insert Falg
			{
				CUIntArray	*piaData = (CUIntArray*)lParam;	
				
				strTemp.Format("\n# 조건 선택 => Group : %d, Row : %d",
					piaData->GetAt(0), piaData->GetAt(1));
				OutputDebugString(strTemp);

				if ((piaData->GetAt(0) == 0) || (m_stSelectCondition.nGroup == 0))
				{
					//# 지표 전환
					m_pwndMain->SendMessage(WM_CONDITIONMAKE_WND, (WPARAM)NM_CHANGE_CONDITION, (WPARAM)piaData->GetAt(0));
				}
				
				if (piaData->GetAt(1) == -1)		//# Only Select Group!
				{
					return 0;
				}

				m_bSelectCond = TRUE;	// 그룹변경, 조건은 아직 선택.		

				SetShowMapLoader(TRUE, piaData->GetAt(0), piaData->GetAt(1));
			}
			break;
			
		case NM_CONDITION_DRAGDROP :	//# (0) : Group, (1) : Old Row, (2) : New Row
			{
				CUIntArray	*piaData = (CUIntArray*)lParam;
				strTemp.Format("\n# 조건 Drag & Drop \n Group : %d, Old Row : %d, New Row : %d",
					piaData->GetAt(0), piaData->GetAt(1), piaData->GetAt(2));
				TRACE(strTemp);
			}
			break;
			
		case NM_STARTBONG_VAL :		//# (0) : Group, (1) : Val
			{
				CUIntArray	*piaData = (CUIntArray*)lParam;
				m_DataAdmin.ModifyConditionTerm(piaData->GetAt(0), piaData->GetAt(1), "START");
				//m_bSaveFlag = FALSE;
			}
			break;
			
		case NM_ENDBONG_VAL :		//# (0) : Group, (1) : Val
			{
				CUIntArray	*piaData = (CUIntArray*)lParam;
				m_DataAdmin.ModifyConditionTerm(piaData->GetAt(0), piaData->GetAt(1), "END");
				//m_bSaveFlag = FALSE;
			}
			break;
			
		case NM_ALIGN_ELEMENT :
			{
				strTemp = "\n Grid Element 정렬!";
				TRACE(strTemp);
				
				int nIndex1st, nIndex2st;
				//## Data Modify ##
				nIndex1st = SetAlignElement();
				nIndex2st = (int)lParam;
				
				if (nIndex1st == nIndex2st)
					m_nElementIndex = nIndex1st;
				else
				{
					if(m_nSeparateForm == 0)
						SetMessageBox("Element Align Error!", PROGRAM_TITLE, MB_OK);
					else
						SetMessageBox("Element Align Error!", PROGRAM_TITLE_BRIGHT, MB_OK);
				}
			}
			break;
			
		case NM_VALVIEWALL_CHECK :	//# (0) : Group, (1) : Val
			{
				CUIntArray	*piaData;
				piaData = (CUIntArray*)lParam;	
				
				strTemp.Format("\n# 값보기 전체 선택 \n Group : %d, Val : %d",
					piaData->GetAt(0), piaData->GetAt(1));
				TRACE(strTemp);
				
				m_DataAdmin.ModifyGroupMoveFieldCheck(piaData->GetAt(0), piaData->GetAt(1));
				
				//m_bSaveFlag = FALSE;
			}
			break;
			
		case NM_DELETEALL	:		//# Group
			{
				int nGroupNo = (int)lParam;
				
				strTemp.Format("\n# 전체 데이터 삭제! Group : %d",
					nGroupNo);
				TRACE(strTemp);
				
				m_DataAdmin.ModifyConditionDeleteGroup(nGroupNo);
				
				//# Alert Chk Box State Modify!
				if (m_DataAdmin.GetConditionCnt() > 0)
				{
					CStringArray saElement;
					m_btnAlertChk.EnableWindow(!m_DataAdmin.GetNonAlertCondiHave(&saElement));
				}
				else if (m_DataAdmin.GetConditionCnt() == 0)
				{
					//m_strSelectFileName = _T("");				//#<= Init Loading File Name!
					OnBtnInit();
					return 1;
				}
				
				if (nGroupNo == 0)
					m_ctrlCommonLogicEdit.DeleteAll();
				else 
				{
					
					m_DataAdmin.ModifyConditionDeleteGroup(nGroupNo);
					m_ctrlOrderLogicEdit.ModifyElementDeleteGroup(nGroupNo);
					m_ctrlOrderLogicEdit.DeleteGroupAll(nGroupNo);

					m_stSelectCondition.nGroup = nGroupNo;
					m_stSelectCondition.nRow = -1;
					m_stSelectCondition.strElement = "";
					m_stSelectCondition.strTag = "";

					m_pGridCtrl->SetDeleteGroup(nGroupNo);

				}
					
				
				//m_bSaveFlag = FALSE;
			}
			break;
			
		case NM_CONDITION_CHECK : //# (0) : Group, (1) : Row, (2) : Val
			{
				CUIntArray	*piaData;
				piaData = (CUIntArray*)lParam;
				
				strTemp.Format("\n# 조건 선택! \n Group : %d, Row : %d, Val : %d",
					piaData->GetAt(0), piaData->GetAt(1), piaData->GetAt(2));
				TRACE(strTemp);
				
				strElement = m_pGridCtrl->GetElementInfo(piaData->GetAt(0), piaData->GetAt(1));
				m_DataAdmin.ModifyConditionCheck(strElement, piaData->GetAt(2));
				
				if (piaData->GetAt(0) == 0)
					m_ctrlCommonLogicEdit.ModifyElementView(strElement, piaData->GetAt(2));
				else if (piaData->GetAt(0) > 0)
					m_ctrlOrderLogicEdit.ModifyElementView(strElement, piaData->GetAt(2));

							
				//m_bSaveFlag = FALSE;
			}
			break;
			
		case NM_TARGET_LIST :	//# (0) : Group, (1) : Row, (2) : Target Type, (3) : Target Index
			{
				CUIntArray	*piaData;
				piaData = (CUIntArray*)lParam;
				
				
				strTemp.Format("\n# 조건 구분 선택! \n Group : %d, Row : %d, Target Type : %d, Val : %d",
					piaData->GetAt(0), piaData->GetAt(1), piaData->GetAt(2), piaData->GetAt(3));
				TRACE(strTemp);
				
				if (piaData->GetAt(0) == 0)
				{
					strElement = m_pGridCtrl->GetElementInfo(piaData->GetAt(0), piaData->GetAt(1));
					m_DataAdmin.ModifyOneConditionTargetBase(strElement, piaData->GetAt(3));
				}
				else
				{
					//# 순차 그룹 멀티 타입 적용 X (01.06)
					for (int nGroup = 1; nGroup <= m_DataAdmin.GetMaxGroup(); nGroup++)
						m_DataAdmin.ModifyConditionTargetBase(nGroup, piaData->GetAt(3));
				}
				
				if (m_saWorkDate.GetSize() > 0)
				{
					//# Back Test Date Set!
					SetBackDateSearch(m_saBackDateCnt.GetAt(m_DataAdmin.GetMinTargetBase()));
				}
				
				//m_bSaveFlag = FALSE;
			}
			break;
			
		case NM_VALVIEW_CHECK : //# (0) : Group, (1) : Row, (2) : Val
			{
				CUIntArray	*piaData;
				piaData = (CUIntArray*)lParam;
				
				strTemp.Format("\n# 조건 값보기 선택! \n Group : %d, Row : %d, Val : %d",
					piaData->GetAt(0), piaData->GetAt(1), piaData->GetAt(2));
				TRACE(strTemp);
				//AfxMessageBox(strTemp);
				
				strElement = m_pGridCtrl->GetElementInfo(piaData->GetAt(0), piaData->GetAt(1));
				m_DataAdmin.ModifyConditionMoveFieldCheck(strElement, piaData->GetAt(2));
				m_pGridCtrl->Invalidate();
				
				//m_bSaveFlag = FALSE;
			}
			break;
			
		case NM_MODIFY_BTN :	//# (0) : Group, (1) : Row, (2) : Display Row, (3) : Display Row => Top Screen Pos
			{
			/*
			CUIntArray	*piaData;
			piaData = (CUIntArray*)lParam;
			
			  strTemp.Format("\n# 조건 수정버튼 선택! \n Group : %d, Row : %d, DisRow : %d, Top Screen Pos : %d",
			  piaData->GetAt(0), piaData->GetAt(1), piaData->GetAt(2), piaData->GetAt(3));
			  TRACE(strTemp);
			  //AfxMessageBox(strTemp);
			  
				CPoint pPosition;
				pPosition.x = 0;
				pPosition.y = piaData->GetAt(3);
				
				  if (piaData->GetAt(1) - piaData->GetAt(2) > 0)
				  pPosition.y += (piaData->GetAt(1) - piaData->GetAt(2)) * 18;
				  SetShowMapLoader(TRUE, piaData->GetAt(0), piaData->GetAt(1), pPosition);
				  
					m_pGridCtrl->Invalidate();
				*/
			}
			break;
			
		case NM_DELETE_BTN :	//#=> (0) : Group, (1) : Row(Focus), (2) : nIndexElement
			{	
				CUIntArray	*piaData;
				piaData = (CUIntArray*)lParam;
				
				strTemp.Format("\n# 조건 삭제버튼 선택! \n Group : %d, Row : %d, Element Index : %d",
					piaData->GetAt(0), piaData->GetAt(1), piaData->GetAt(2));
				TRACE(strTemp);
				
				strElement = CONDITION_INDEX[piaData->GetAt(2)];
				m_DataAdmin.DeleteOneCondition(strElement);
				if(m_DataAdmin.GetConditionCnt() == 0 && m_nSeparateForm == 0)
					m_btnBackTestChk.EnableWindow(FALSE);
				
				//# Alert Chk Box State Modify!
				if (m_DataAdmin.GetConditionCnt() > 0)
				{
					CStringArray saElement;
					m_btnAlertChk.EnableWindow(!m_DataAdmin.GetNonAlertCondiHave(&saElement));
				}
				else if (m_DataAdmin.GetConditionCnt() == 0)
				{
					//					m_strSelectFileName = _T("");				//#<= Init Loading File Name!
					OnBtnInit();
					return 1;
				}
				
				if (piaData->GetAt(0) == 0)
					m_ctrlCommonLogicEdit.DeleteElement(strElement);
				else if (piaData->GetAt(0) > 0)
					m_ctrlOrderLogicEdit.DeleteElement(strElement);
				
				//# Back Test Date Set!
// 				if (m_saWorkDate.GetSize() > 0)
// 					SetBackDateSearch(m_saBackDateCnt.GetAt(m_DataAdmin.GetMinTargetBase()));
				m_pGridCtrl->Invalidate();
				
				//m_bSaveFlag = FALSE;
			}
			break;
		case NM_GROUP_ADD :	//# Group	
			{
				
			}
			break;
		case NM_GROUP_INSERTUP :	//# Group
			{
//				int nGroup = (int)lParam;

				int nGroup = HIWORD((int)lParam);
				int nIndex = LOWORD((int)lParam);
				
				if (nGroup > -1)
				{
//					m_DataAdmin.ModifyConditionInsertGroup(nGroup);
//					m_ctrlOrderLogicEdit.ModifyElementInsertGroup(nGroup);

					// 2009.11.10 -hidden-
					m_DataAdmin.ModifyConditionInsertUpGroup(nIndex, nGroup);
					m_ctrlOrderLogicEdit.ModifyElementInsertUpGroup(nIndex, nGroup);

					m_stSelectCondition.nGroup = nGroup;	
					m_stSelectCondition.nIndex = nIndex;	// 2009.11.09 -hidden-
					m_stSelectCondition.nRow = -1;
					m_stSelectCondition.strElement = "";
					m_stSelectCondition.strTag = "";
					
					m_pwndMain->SendMessage(WM_CONDITIONMAKE_WND, (WPARAM)NM_CHANGE_CONDITION, (WPARAM)nGroup);
				}
			}
			break;
		case NM_GROUP_INSERTDOWN :	//# Group
			{
//				int nGroup = (int)lParam;
				
				int nGroup = HIWORD((int)lParam);
				int nIndex = LOWORD((int)lParam);
				
				if (nGroup > -1)
				{
//					m_DataAdmin.ModifyConditionInsertGroup(nGroup);
//					m_ctrlOrderLogicEdit.ModifyElementInsertGroup(nGroup);

					// 2009.11.10 -hidden-
					m_DataAdmin.ModifyConditionInsertDownGroup(nIndex, nGroup);
					m_ctrlOrderLogicEdit.ModifyElementInsertDownGroup(nIndex, nGroup);

					m_stSelectCondition.nGroup = nGroup;
					m_stSelectCondition.nIndex = nIndex;		// 2009.11.09 -hidden-
					m_stSelectCondition.nRow = -1;
					m_stSelectCondition.strElement = "";
					m_stSelectCondition.strTag = "";

					m_pwndMain->SendMessage(WM_CONDITIONMAKE_WND, (WPARAM)NM_CHANGE_CONDITION, (WPARAM)nGroup);
				}
			}
			break;
		case NM_GROUP_DELETE : //# Group
			{
				int nGroup = (int)lParam;
				
				//# Data Delete!
				m_DataAdmin.ModifyConditionDeleteGroup(nGroup);
				m_ctrlOrderLogicEdit.ModifyElementDeleteGroup(nGroup);
				
				m_stSelectCondition.nGroup = nGroup;
				m_stSelectCondition.nRow = -1;
				m_stSelectCondition.strElement = "";
				m_stSelectCondition.strTag = "";
				
				//m_bSaveFlag = FALSE;
			}
			break;
		case NM_ROW_UP_BTN :
		case NM_ROW_DOWN_BTN :		
			{
				CUIntArray	*piaData = (CUIntArray*)lParam;
// 				strTemp.Format("\n# 조건 Drag & Drop \n Group : %d, Old Row : %d, New Row : %d",
// 				piaData->GetAt(0), piaData->GetAt(1), piaData->GetAt(2));		

				int nGroup = piaData->GetAt(0);
				
				strElement		= m_pGridCtrl->GetElementInfo(nGroup, piaData->GetAt(1));//Old Row
				strElementAfter = m_pGridCtrl->GetElementInfo(nGroup, piaData->GetAt(2));//New Row

				if(wParam == NM_ROW_UP_BTN)
					m_stSelectCondition.nRow--;
				else if(wParam == NM_ROW_DOWN_BTN)
					m_stSelectCondition.nRow++;
				
				
				if(nGroup == 0)
				{				
					m_ctrlCommonLogicEdit.DeleteAllBracket();
					m_ctrlCommonLogicEdit.SwitchElement(strElement, strElementAfter);
					m_ctrlCommonLogicEdit.ReLoadAll();
				}
				else
				{
					m_ctrlOrderLogicEdit.DeleteAllBracket();
					m_ctrlOrderLogicEdit.SwitchGroupElement(strElement, strElementAfter);
					m_ctrlOrderLogicEdit.ReLoadAll();
				}
				// 20110701 박찬모 조건의 순서를 바꾸면 해당 조건의 심볼도 수정하도록 개선 
				m_DataAdmin.ModifyConditionSwitch(strElement, strElementAfter);
/*				
				GROUP_INFO* pGroupInfo1 = NULL;
				GROUP_INFO* pGroupInfo2= NULL;

				GROUP_INFO* pTempGroupInfo = NULL;
				
				pGroupInfo1 = m_DataAdmin.m_arrGroups.GetAt(piaData->GetAt(1));
				pGroupInfo2 = m_DataAdmin.m_arrGroups.GetAt(piaData->GetAt(2));
				
				pTempGroupInfo = pGroupInfo1;

				pGroupInfo1 = pGroupInfo2;
				pGroupInfo2 = pTempGroupInfo;
				
				m_DataAdmin.m_arrGroups.SetAt(piaData->GetAt(1), pGroupInfo1);
				m_DataAdmin.m_arrGroups.SetAt(piaData->GetAt(2), pGroupInfo2);

*/
///////////////////
			/*	int nGroups = m_DataAdmin.m_arrGroups.GetSize();
				ConditionGroupInfo  stConditionGroupInfo;
				GROUP_INFO* pGroupInfo = NULL;

				for(int i = 0; i < nGroups; ++i)
				{
					pGroupInfo = m_DataAdmin.m_arrGroups.GetAt(i);
					if(pGroupInfo->nGroup == nGroup)
					{
						pGroupInfo->arrConGrpInfo.Add(stConditionGroupInfo);
						return TRUE;
					}
				}

				pGroupInfo = new GROUP_INFO;
				pGroupInfo->nGroup = nGroup;
				pGroupInfo->arrConGrpInfo.Add(stConditionGroupInfo);

				if(nGroup == 0)
					m_DataAdmin.m_arrGroups.InsertAt(0, pGroupInfo);
				else
					m_DataAdmin.m_arrGroups.Add(pGroupInfo);*/
/////////////////////


//				m_DataAdmin.m_arrGroups.InsertAt(0, pGroupInfo);
				m_pGridCtrl->Invalidate();
			}
			break;
		case NM_ROW_MAXUP_BTN :
			{
				m_stSelectCondition.nRow = 0;

				CStringArray* psaData = (CStringArray *)lParam;
				strElement		= psaData->GetAt(1);
				strElementAfter = psaData->GetAt(2);

				int nGroup = atoi(psaData->GetAt(0));
				
				if(nGroup == 0)
				{				
					m_ctrlCommonLogicEdit.DeleteAllBracket();
					m_ctrlCommonLogicEdit.InsertElementOnMaxUpBtn(strElement, strElementAfter);
					m_ctrlCommonLogicEdit.ReLoadAll();
				}
				if(nGroup > 0)
				{				
					m_ctrlOrderLogicEdit.DeleteAllBracket();
					m_ctrlOrderLogicEdit.InsertGroupElementOnMaxUpBtn(strElement, strElementAfter);
					m_ctrlOrderLogicEdit.ReLoadAll();
				}

				// 20110701 박찬모 조건의 순서를 바꾸면 해당 조건의 심볼도 수정하도록 개선 
				m_DataAdmin.ModifyConditionSwitch(strElement, strElementAfter);

				m_pGridCtrl->Invalidate();

			}
			break;
		case NM_ROW_MINDOWN_BTN :
			{
				m_stSelectCondition.nRow = m_pGridCtrl->GetGroupRowCnt(0);

				CStringArray* psaData = (CStringArray *)lParam;
				strElement		= psaData->GetAt(1);
				strElementAfter = psaData->GetAt(2);

				int nGroup = atoi(psaData->GetAt(0));

				if(nGroup == 0)
				{
					m_ctrlCommonLogicEdit.DeleteAllBracket();
					m_ctrlCommonLogicEdit.AddElementOnMinDownBtn(strElement, strElementAfter);
					m_ctrlCommonLogicEdit.ReLoadAll();
				}
				if(nGroup > 0)
				{
					m_ctrlOrderLogicEdit.DeleteAllBracket();
					m_ctrlOrderLogicEdit.AddGroupElementOnMinDownBtn(strElement, strElementAfter);
					m_ctrlOrderLogicEdit.ReLoadAll();
				}				
				// 20110701 박찬모 조건의 순서를 바꾸면 해당 조건의 심볼도 수정하도록 개선 
				m_DataAdmin.ModifyConditionSwitch(strElement, strElementAfter);

				m_pGridCtrl->Invalidate();
			}
			break;
		case NM_CHANGE_SUBJECT :
			{
				CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
				((CConditionResultWnd*)pwndMainFrm->GetWnd("RESULT"))->OnChangeSubject();
				break;
			}
		case NM_CHANGE_SUBJECT_INIT :
			{
				CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
				((CConditionResultWnd*)pwndMainFrm->GetWnd("RESULT"))->OnChangeSubjectInit();
				((CConditionResultWnd*)pwndMainFrm->GetWnd("RESULT"))->SetSaveDefalutChangeSubject();
				break;
			}
			
		case NM_UPDATE :
			if(!m_bInitIng && m_DataAdmin.GetConditionCnt() > 2)
				m_ctrlMapLoad.EuBtnUpdate();			
			break;
				
		default :
			m_pGridCtrl->ShowWindow(SW_HIDE);
			break;
	}
	
	return 1;
}

void CConditionMakeWnd::SetNowXMLTreeInfo(XMLTreeInfo *pstXMLTreeInfo)
{
}

void CConditionMakeWnd::OnBtnPortDel()
{
	int nCount = m_ctrlPortEdit.m_listPort.GetCount();
	if (nCount == 0) 
		return;
	
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	CPortPolioDelDlg *pPortDelDlg = new CPortPolioDelDlg(this);
	pPortDelDlg->m_peditPortfolio = &m_ctrlPortEdit;
	
	CRect rePortDel, rePosition;
	m_btnPortDel.GetWindowRect(&rePortDel);
	pPortDelDlg->m_rePortDel = rePortDel;
	pPortDelDlg->DoModal();
	
	delete pPortDelDlg;

	AfxSetResourceHandle(hInstSave);
}

void CConditionMakeWnd::OnBtnLogicTip()
{
// 	m_tipLogicInfo.UpdateToolTip(this, (LPCTSTR)m_strLogicInfoTip, m_nLogicTipMaxLength);
// 	m_tipLogicInfo.Show(m_strLogicInfoTip);
// 	HINSTANCE hInstSave = AfxGetResourceHandle();
// 	AfxSetResourceHandle(g_hInstance);
	
	CLogicHelp dlg(this);
	if(dlg.DoModal() == IDOK)
	{

	}
	
//	AfxSetResourceHandle(hInstSave);
}

BOOL CConditionMakeWnd::GetLoadIniInfo()
{
	CFileFind cFind;
	BOOL bRead = TRUE;
	char chBuff[BUFSIZ];
	CString strBuff = "";
	int nCount = 0;
	
	m_strGridInfoTip = "";
	m_strLogicInfoTip = "";
	m_saBackDateCnt.RemoveAll();
	
	CMainFrmWnd *wndMainFrm = (CMainFrmWnd*)m_pwndMain;
	
	CString strFile;
	strFile = wndMainFrm->GetFilePath("FINDERINFO");
	bRead = cFind.FindFile(strFile);
	if (!bRead)
	{
		CString strMsg = strFile + "을 찾지 못했습니다.";
		SetMessageBox(strMsg);
	}
	else
	{
		int nIndex;
		CString strTemp;
		
		nCount = GetPrivateProfileInt("Logic Tip", "Line Count", 0, strFile);
		memset(chBuff, ' ', sizeof(chBuff));
		for(nIndex = 0; nIndex < nCount; nIndex++)
		{
			strTemp.Format("%d", nIndex + 1);
			GetPrivateProfileString("Logic Tip", strTemp, "", chBuff, sizeof(chBuff), strFile);
			strBuff = chBuff;
			if(chBuff[0] == '.')
				strBuff = strBuff.Mid(1, strBuff.GetLength());
			
			if (strBuff.GetLength() > m_nLogicTipMaxLength)
				m_nLogicTipMaxLength = strBuff.GetLength();
			
			m_strLogicInfoTip += strBuff;
			
			if (nIndex < nCount - 1)
				m_strLogicInfoTip += "\n";
		}
		
		nCount = GetPrivateProfileInt("Grid Tip", "Line Count", 0, strFile);
		memset(chBuff, ' ', sizeof(chBuff));
		for(nIndex = 0; nIndex < nCount; nIndex++)
		{
			strTemp.Format("%d", nIndex + 1);
			GetPrivateProfileString("Grid Tip", strTemp, "", chBuff, sizeof(chBuff), strFile);
			strBuff = chBuff;
			if(chBuff[0] == '.')
				strBuff = strBuff.Mid(1, strBuff.GetLength());
			
			if (strBuff.GetLength() > m_nGridTipMaxLength)
				m_nGridTipMaxLength = strBuff.GetLength();
			
			m_strGridInfoTip += strBuff;
			
			if (nIndex < nCount - 1)
				m_strGridInfoTip += "\r\n";
		}
		
		nCount = GetPrivateProfileInt("BackTest_DateCnt", "Line Count", 0, strFile);
		memset(chBuff, ' ', sizeof(chBuff));
		for (nIndex = 0; nIndex < nCount; nIndex++)
		{
			strTemp.Format("%d", nIndex + 1);
			GetPrivateProfileString("BackTest_DateCnt", strTemp, "", chBuff, sizeof(chBuff), strFile);
			strBuff = chBuff;
			strTemp = strBuff.Mid(0, strBuff.Find("/"));
			m_saBackDateCnt.Add(strTemp);
			
			if (atoi(strTemp) > m_nBackDateMaxCnt)
				m_nBackDateMaxCnt = atoi(strTemp);
		}
	}
	
	return bRead;
}

void CConditionMakeWnd::OnBtnBracket()
{
	if (m_ctrlCommonLogicEdit.GetElementCount() > 0)
		m_ctrlCommonLogicEdit.BracketSelectMake();
}

void CConditionMakeWnd::OnBtnBracketDel()
{
	m_ctrlCommonLogicEdit.BracketDelete();
}

void CConditionMakeWnd::OnChangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{	
	int nTabIndex = m_ptabSet->GetCurSel();

	CMainFrmWnd* pMainFrm = (CMainFrmWnd*)m_pwndMain;
	pMainFrm->m_pviewCondition->OnExtendLigcEdit(m_bCommonExState, m_ptabSet->GetCurSel());	
	
	SetTabView(nTabIndex);
}

void CConditionMakeWnd::SetTabView(int nTabIndex)
{
	int nCmdShow;
	if (nTabIndex == 0)
		nCmdShow = SW_HIDE;
	else if (nTabIndex == 1)
		nCmdShow = SW_SHOW;

	m_nTabIndex = nTabIndex;
	
	//m_ctrlLogicBar.ShowWindow(nCmdShow);
	if(!m_nSeparateForm)
	{
		if(nTabIndex == 1)
		{
			m_ctrlCommonLogic.ShowWindow(SW_SHOW);
			m_ctrlCommonLogicEdit.ShowWindow(SW_SHOW);

			m_ctrlOrderLogic.ShowWindow(SW_SHOW);
			m_ctrlOrderLogicEdit.ShowWindow(SW_SHOW);
		}
		else if(m_pGridCtrl->GetSelectGroup() == 0)
		{
			m_ctrlOrderLogic.ShowWindow(SW_HIDE);
			m_ctrlOrderLogicEdit.ShowWindow(SW_HIDE);
			
			m_ctrlCommonLogic.ShowWindow(SW_SHOW);
			m_ctrlCommonLogicEdit.ShowWindow(SW_SHOW);
		}
		else if(m_pGridCtrl->GetSelectGroup() > 0)
		{
			m_ctrlCommonLogic.ShowWindow(SW_HIDE);
			m_ctrlCommonLogicEdit.ShowWindow(SW_HIDE);
			
			m_ctrlOrderLogic.ShowWindow(SW_SHOW);
			m_ctrlOrderLogicEdit.ShowWindow(SW_SHOW);
		}
		
		m_btnCommonEx.ShowWindow(SW_SHOW);

		m_btnBracket.ShowWindow(SW_SHOW);
		m_btnBracketDel.ShowWindow(SW_SHOW);
		m_btnChangeSubject.ShowWindow(SW_SHOW);
	}

	
// 	m_ctrlOrderLogic.ShowWindow(nCmdShow);
// 	m_ctrlOrderLogicEdit.ShowWindow(nCmdShow);
	m_btnOrderEx.ShowWindow(nCmdShow);
	
	m_ctrlPortfolio.ShowWindow(nCmdShow);
	m_btnPortCheck.ShowWindow(nCmdShow);
	m_ctrlPortEdit.ShowWindow(nCmdShow);
	//m_ctrlGwansimEdit.ShowWindow(nCmdShow);
	m_btnPortDel.ShowWindow(nCmdShow);
	
	m_ctrlMatchVal.ShowWindow(nCmdShow);
	m_ctrlMatchValEdit.ShowWindow(nCmdShow);
	m_btnMatchValEdit.ShowWindow(nCmdShow);
	m_btnMatchValCheck.ShowWindow(nCmdShow);
	
	m_ctrlMapLoad.ShowWindow(!nCmdShow);
	
	if(nCmdShow == SW_HIDE)
	{
		if (m_stSelectCondition.strTag != "LANGUAGE")
		{		

		}
		
		
	}
	else
	{
		

		
	}
	
	OnSize();
	//Invalidate();
}

void CConditionMakeWnd::OnBtnCommonExSize()
{
	CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pwndMain;
	if (m_bCommonExState == TRUE)
		SetBtnImg(&m_btnCommonEx, "CS_Logic_Exp_Up");
	else
		SetBtnImg(&m_btnCommonEx, "CS_Logic_Exp_Down");
	
	m_bCommonExState = !m_bCommonExState;
	/*OnSize();*/

	pMainWnd->m_pviewCondition->OnExtendLigcEdit(m_bCommonExState, m_ptabSet->GetCurSel());	

	if(m_ptabSet->GetCurSel() == 1)
		OnSize();
	
	Invalidate();
}

void CConditionMakeWnd::OnBtnOrderExSize()
{
	if (m_bOrderExState == TRUE)
		SetBtnImg(&m_btnOrderEx, "CS_Logic_Exp_Up");
	else
		SetBtnImg(&m_btnOrderEx, "CS_Logic_Exp_Down");
	
	m_bOrderExState = !m_bOrderExState;
	OnSize();
	
	Invalidate();
}

void CConditionMakeWnd::OnBtnMatchVal()
{
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	CMatchValEditDlg	dlgMatchVal(m_pwndMain);
	if (dlgMatchVal.DoModal() == IDOK)
	{
		m_ctrlMatchValEdit.SetWindowText(GetMatchLogic());
		m_pwndMain->SendMessage(WM_CONDITIONMAKE_WND, (WPARAM)NM_SEARCH, (WPARAM)NULL);
	}

	AfxSetResourceHandle(hInstSave);
}

void CConditionMakeWnd::OnBtnMatchValOk()
{
	m_ctrlMatchValEdit.SetWindowText(GetMatchLogic());
	m_pwndMain->SendMessage(WM_CONDITIONMAKE_WND, (WPARAM)NM_SEARCH, (WPARAM)NULL);
}

void CConditionMakeWnd::SetMatchLogic_Delete(CString strElement, BOOL bView/*=TRUE*/)
{
	POSITION pos;
	MatchInfo	stMatchInfo;
	
	int nIndex;
	int nPosIndex = 0;
	int nMaxGridSourceRow = -1;
	int nDeleteCnt = 0;
	
	int nDataCnt = m_listMatchInfo.GetCount();
	for (nIndex = 0; nIndex < nDataCnt; nIndex++)
	{
		pos = m_listMatchInfo.FindIndex(nPosIndex);
		stMatchInfo = m_listMatchInfo.GetAt(pos);
		
		if (strElement == stMatchInfo.strElement)
		{
			if (stMatchInfo.nGridSourceRow > nMaxGridSourceRow)
				nMaxGridSourceRow = stMatchInfo.nGridSourceRow;
			
			nDeleteCnt++;
			m_listMatchInfo.RemoveAt(pos);
		}
		else
			nPosIndex++;
	}
	
	//# Delete => MatchVal UpDate!
	nDataCnt = m_listMatchInfo.GetCount();
	for (nIndex = 0; nIndex < nDataCnt; nIndex++)
	{
		pos = m_listMatchInfo.FindIndex(nIndex);
		stMatchInfo = m_listMatchInfo.GetAt(pos);
		
		if (nMaxGridSourceRow < stMatchInfo.nGridSourceRow)
		{
			stMatchInfo.nGridSourceRow -= nDeleteCnt;
			m_listMatchInfo.SetAt(pos, stMatchInfo);
		}
	}
	
	if (bView == TRUE)
		m_ctrlMatchValEdit.SetWindowText(GetMatchLogic());
}


void CConditionMakeWnd::SetMatchLogic_UpDate()
{
	CMainFrmWnd *wndMainFrm = (CMainFrmWnd*)GetParent();
	
	POSITION pos;
	MatchInfo	stMatchInfo;
	
	int nIndex;
	int nPosIndex = 0;
	int nMaxGridSourceRow = -1;
	int nDeleteCnt = 0;
	
	int nDataCnt = m_listMatchInfo.GetCount();
	for (nIndex = 0; nIndex < nDataCnt; nIndex++)
	{
		pos = m_listMatchInfo.FindIndex(nPosIndex);
		stMatchInfo = m_listMatchInfo.GetAt(pos);
		
		if (stMatchInfo.strElement.IsEmpty() == FALSE)
		{
			nPosIndex++;
			continue;
		}
		
		/*
		if (wndMainFrm->m_pwndConditionResult->GetHave_FixUserField(stMatchInfo.strField) == FALSE)
		{
		if (stMatchInfo.nGridSourceRow > nMaxGridSourceRow)
		nMaxGridSourceRow = stMatchInfo.nGridSourceRow;
		
		  nDeleteCnt++;
		  m_listMatchInfo.RemoveAt(pos);
		  }
		  else
		  nPosIndex++;
		*/
	}
	
	//# Delete => MatchVal UpDate!
	nDataCnt = m_listMatchInfo.GetCount();
	for (nIndex = 0; nIndex < nDataCnt; nIndex++)
	{
		pos = m_listMatchInfo.FindIndex(nIndex);
		stMatchInfo = m_listMatchInfo.GetAt(pos);
		
		if (nMaxGridSourceRow < stMatchInfo.nGridSourceRow)
		{
			stMatchInfo.nGridSourceRow -= nDeleteCnt;
			m_listMatchInfo.SetAt(pos, stMatchInfo);
		}
	}
	
	m_ctrlMatchValEdit.SetWindowText(GetMatchLogic());
}

CString CConditionMakeWnd::GetMatchLogic()
{
	CString strResult, strField;
	
	POSITION pos;
	MatchInfo	stMatchInfo;
	for (int nIndex = 0; nIndex < m_listMatchInfo.GetCount(); nIndex++)
	{
		pos = m_listMatchInfo.FindIndex(nIndex);
		stMatchInfo = m_listMatchInfo.GetAt(pos);
		
		if (stMatchInfo.strElement.IsEmpty() == TRUE)
			strField = stMatchInfo.strField;
		else
			strField = stMatchInfo.strField + "(" + stMatchInfo.strElement + ")";
		
		if (nIndex < m_listMatchInfo.GetCount() - 1)
			strField = strField + " " + (CString)MATCHVAL_OPT_LIST[stMatchInfo.nOptIndex] + " ";
		strResult = strResult + strField;
	}
	
	return strResult;
}

void CConditionMakeWnd::OnBtnPortCheck()
{
	m_ctrlPortEdit.EnableWindow(m_btnPortCheck.GetCheck());
	//m_ctrlGwansimEdit.EnableWindow(m_btnPortCheck.GetCheck());
}

void CConditionMakeWnd::OnBtnFileSave()
{
	m_btnAlertChk.SetCheck(TRUE);
		
	if (m_DataAdmin.GetConditionCnt() > 0)
	{
		HINSTANCE hInstSave = AfxGetResourceHandle();
		AfxSetResourceHandle(g_hInstance);
		
		CSaveDlg  dlgSave(m_pwndMain, m_stLoadedFile.strFileName, m_stLoadedFile.strFolderPath);
		if (dlgSave.DoModal() == IDOK)
		{
			m_pwndMain->SendMessage(WM_CONDITIONMAKE_WND, (WPARAM)NM_FILE_SAVE, (WPARAM)NULL);
			SetFileSaveState(TRUE);
		}

		AfxSetResourceHandle(hInstSave);
	}
	else
		SetMessageBox("저장할 조건이 존재하지 않습니다.", PROGRAM_TITLE, MB_OK);
}

void CConditionMakeWnd::OnBtnMatchValCheck()
{
	BOOL bState = m_btnMatchValCheck.GetCheck();
	
	m_ctrlMatchValEdit.EnableWindow(bState);
}

void CConditionMakeWnd::CheckMakeConditionFromFile()
{
	if ((m_bSaveFlag == FALSE) && (m_DataAdmin.GetConditionCnt() > 0))
	{
		if (SetMessageBox("이미 구성한 조건이 존재합니다.\n저장하지않으면 구성된 조건이 삭제됩니다.\n\n구성한 조건을 저장하시겠습니까?", 
			(CString)PROGRAM_TITLE, MB_OKCANCEL) == IDOK)
			OnBtnFileSave();
	}
}

void CConditionMakeWnd::SetControlEnable(BOOL bState)
{
	bState = !bState;
	
	m_comboTargetBase.EnableWindow(bState);
	m_btnGridTip.EnableWindow(bState);
	
	if (m_saWorkDate.GetSize() > 0)
	{
		m_btnBackTestChk.EnableWindow(bState);
		if (bState == TRUE)
		{
			m_comboBackTestDay.EnableWindow(m_btnBackTestChk.GetCheck());
			m_comboBackTestTime.EnableWindow(m_btnBackTestChk.GetCheck());
		}
		else
		{
			m_comboBackTestDay.EnableWindow(bState);
			m_comboBackTestTime.EnableWindow(bState);
		}
		
		m_btnBackTestChk.Invalidate();
		m_comboBackTestDay.Invalidate();
		m_comboBackTestTime.Invalidate();
	}
	
	m_btnAlertChk.EnableWindow(bState);
	m_btnInit.EnableWindow(bState);
	
	CString strTemp;
	if (bState == TRUE)
	{
		m_ctrlCommonLogicEdit.GetWindowText(strTemp);
		m_ctrlCommonLogicEdit.EnableWindow(!strTemp.IsEmpty());
	}
	else
		m_ctrlCommonLogicEdit.EnableWindow(FALSE);
	m_btnCommonEx.EnableWindow(bState);
	
	if (bState == TRUE)
	{
		m_ctrlOrderLogicEdit.GetWindowText(strTemp);
		m_ctrlOrderLogicEdit.EnableWindow(!strTemp.IsEmpty());
	}
	else
		m_ctrlOrderLogicEdit.EnableWindow(FALSE);
	m_btnOrderEx.EnableWindow(bState);
	
	
	if (bState == TRUE)
	{
		m_ctrlPortEdit.EnableWindow(m_btnPortCheck.GetCheck());
		//m_ctrlGwansimEdit.EnableWindow(m_btnPortCheck.GetCheck());
	}
	else
	{
		m_ctrlPortEdit.EnableWindow(bState);
		//m_ctrlGwansimEdit.EnableWindow(bState);
	}
	m_btnPortCheck.EnableWindow(bState);
	
	if (bState == TRUE)
		m_ctrlMatchValEdit.EnableWindow(m_btnMatchValCheck.GetCheck());
	else
		m_ctrlMatchValEdit.EnableWindow(bState);
	m_btnMatchValEdit.EnableWindow(bState);
	m_btnMatchValCheck.EnableWindow(bState);
	
	
	m_btnBracket.EnableWindow(bState);
	m_btnBracketDel.EnableWindow(bState);
	
	m_btnPortDel.EnableWindow(bState);
	m_btnLogicTip.EnableWindow(bState);	
	
	m_btnFileSave.EnableWindow(bState);
	
	m_pGridCtrl->EnableWindow(bState);
	m_ctrlMapLoad.EnableWindow(bState);
	
	m_comboTargetBase.Invalidate();
	m_btnGridTip.Invalidate();
	
	m_btnAlertChk.Invalidate();
	m_btnInit.Invalidate();
	
	m_ctrlCommonLogicEdit.Invalidate();
	m_btnCommonEx.Invalidate();
	
	m_ctrlOrderLogicEdit.Invalidate();
	m_btnOrderEx.Invalidate();
	
	m_ctrlPortEdit.Invalidate();
	//m_ctrlGwansimEdit.Invalidate();
	m_btnPortCheck.Invalidate();
	m_btnPortDel.Invalidate();
	
	m_ctrlMatchValEdit.Invalidate();
	m_btnMatchValEdit.Invalidate();
	m_btnMatchValCheck.Invalidate();
	
	m_btnBracket.Invalidate();
	m_btnBracketDel.Invalidate();
	
	m_btnPortDel.Invalidate();
	m_btnLogicTip.Invalidate();
	
	m_btnFileSave.Invalidate();
	m_pGridCtrl->Invalidate();
	
	m_ctrlMapLoad.Invalidate();
}

void CConditionMakeWnd::SetSelectGroup(int nGroup)
{
	m_stSelectCondition.nGroup = nGroup;
	m_stSelectCondition.nRow = -1;
	m_stSelectCondition.strElement = "";
	m_stSelectCondition.strTag = "";
}

void CConditionMakeWnd::SetInit()
{
	m_bCreateEnd = TRUE;
	
	CMainFrmWnd *wndMainFrm = (CMainFrmWnd*)m_pwndMain;

	m_bIndustry = wndMainFrm->m_bIndustry;
	
	/*m_nSeparateForm = wndMainFrm->m_nSeparateForm;*/

	if(m_nSeparateForm)
	{
		m_ptabSet->ShowWindow(SW_HIDE);
		/*m_ptabSet->ModifyStyle(WS_BORDER, 0);*/

		m_pGridCtrl->ShowWindow(SW_HIDE);

		m_ctrlCommonLogic.ShowWindow(SW_HIDE);
		m_ctrlCommonLogicEdit.ShowWindow(SW_HIDE);
		m_btnCommonEx.ShowWindow(SW_HIDE);

		m_btnBracket.ShowWindow(SW_HIDE);
		m_btnBracketDel.ShowWindow(SW_HIDE);

		m_ctrlFunBar.ShowWindow(SW_HIDE);
	}
	
	if (m_bIndustry == TRUE)
		m_btnAlertChk.ShowWindow(SW_HIDE);
	else
	{
		m_btnAlertChk.ShowWindow(SW_HIDE);
// 		if (wndMainFrm->m_bIsFnUser == TRUE)
// 			m_btnAlertChk.ShowWindow(SW_SHOW);
	}
	
	m_ctrlMapLoad.SetInit(m_bIndustry);
	SetShowMapLoader(FALSE);
	
	SetTabView(0);
	
	GetLoadIniInfo();
	
	m_pGridCtrl->SetDetailViewState(TRUE);	//@유진은 default가 상세설정dla
	OnBtnInit();

	m_btnBackTestChk.EnableWindow(FALSE);
}

void CConditionMakeWnd::SetWorkData(CStringArray *psaWorkDate)
{
	if (psaWorkDate == NULL)
		return;
	
	int nDateCnt = psaWorkDate->GetSize();
	if (nDateCnt < m_nBackDateMaxCnt)
	{
		m_btnBackTestChk.ShowWindow(SW_HIDE);
		m_comboBackTestDay.ShowWindow(SW_HIDE);
		m_comboBackTestTime.ShowWindow(SW_HIDE);
		
		m_saWorkDate.RemoveAll();
	}
	else
	{
		m_saWorkDate.Copy(*psaWorkDate);
		m_comboBackTestDay.EnableWindow(FALSE);
		m_comboBackTestTime.EnableWindow(FALSE);
		//OnBtnBackTestChk();
	}
	
	/*OnSize();*/
}

void CConditionMakeWnd::SetInitVal()
{
	CWnd* pwndParent = GetParent();
	pwndParent = pwndParent->GetParent();   //# CConditionView
	pwndParent = pwndParent->GetParent();		//# CMainFrmWnd
	m_pwndMain = pwndParent;

	m_DataAdmin.SetMainWnd(m_pwndMain);
	
	m_nTargetBaseIndex = 0;
	
	m_nElementIndex = 0;
	SetInit_LoadedFileInfo(m_stLoadedFile);
	
	m_iaBackTestDay.RemoveAll();
	
	m_bCommonExState = FALSE;
	m_bOrderExState = FALSE;
	
	SetSelectGroup(0);
}

void CConditionMakeWnd::SetInit_LoadedFileInfo(XMLTreeInfoFile	&stLoadedFile)
{
	stLoadedFile.strType = "USER";		//# USER, SYS
	
	stLoadedFile.strFolderPath = (CString)FOLDER_TEMPFILE;
	stLoadedFile.strFilePath = "임시조건.xml";
	stLoadedFile.strFileName = "임시조건";
	
	stLoadedFile.bQuickReg = FALSE;			//# Deawoo [NEW]
	stLoadedFile.bUnitCan = FALSE;				//# Deawoo [NEW]
	stLoadedFile.strAlertReg = "FALSE";
	stLoadedFile.strExplain = "";
}

int CConditionMakeWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1) 
		return -1;
		
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	SetInitVal();

	CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pwndMain;
	m_nSeparateForm = pMainWnd->m_nSeparateForm;
	
	m_brushTab.CreateSolidBrush(pMainWnd->GetAxColor(66));

	m_btnExpand.Create(_T(""), CRect(0, 0, 0, 0), this, IDC_LEFTEXP_BTN);
	SetBtnImg(&m_btnExpand, "LEFT_EX");
	
	m_ctrlTargetBase.Create(_T("대상시간(봉)"), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlTargetBase.SetFont(&m_fontDefault);
	
	//# 대상시간 ComboBox
	m_comboTargetBase.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
		CRect(0, 0, 80, 150), this, IDC_TARGETBASE_COMBO);
	m_comboTargetBase.SetFont(&m_fontDefault);
	
	m_comboTargetBase.ResetContent();
	m_comboTargetBase.AddString(" 개별");
	for (int nIndex = 0; nIndex < _countof(TARGETLIST_TYPE1); nIndex++)
		m_comboTargetBase.AddString((CString)(TARGETLIST_TYPE1[nIndex]) + " 일괄");
	m_comboTargetBase.SetCurSel(0);
	m_comboTargetBase.EnableWindow(FALSE);
	
	//# Back Test!
	m_btnBackTestChk.Create(_T("과거시점검색"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTNCHK_BACKTEST);
	m_btnBackTestChk.SetFont(&m_fontDefault);
	
	m_comboBackTestDay.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
								CRect(0, 0, 0, 0), this, IDC_COMBO_BACKTEST_DAY);
	m_comboBackTestDay.SetFont(&m_fontDefault);
	m_comboBackTestDay.ResetContent();
	
	m_comboBackTestTime.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(0, 0, 0, 0), this, IDC_COMBO_BACKTEST_TIME);
	m_comboBackTestTime.SetFont(&m_fontDefault);
	m_comboBackTestTime.ResetContent();
	
	//# Grid Tip
	m_btnGridTip.Create(_T("도움말"), CRect(0, 0, 0, 0), this, IDC_GRIDTIP_BTN);
	SetBtnImg(&m_btnGridTip, 4);

	m_btnSearchRng.Create(_T("검색범위"), CRect(0, 0, 0, 0), this, IDC_SEARCHRNG);
	SetBtnImg(&m_btnSearchRng, 4);
	
	m_tipGridInfo.Create(this, "DOWN", 4);
	m_tipGridInfo.SetFont(&m_fontDefault);
	m_tipGridInfo.AddTool(this, (LPCTSTR)" ");
	
	m_btnAlertChk.Create(_T("신호"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_ALERTCHK);
	m_btnAlertChk.SetFont(&m_fontBold);
	m_btnAlertChk.SetCheck(1);
	
	//# 직원, 고객 구분
	BOOL bAlertUser = FALSE; 
	
	// 현재 삼성에서 Alert에 관한 신호설정을 제공하지 않기때문에 일괄적으로 Hide시킨다.
	if (bAlertUser == FALSE)	m_btnAlertChk.ShowWindow(SW_HIDE);
	
	m_btnDetail.Create(_T("상세설정"), CRect(0, 0, 0, 0), this, IDC_DETAIL_BTN);
	SetBtnImg(&m_btnDetail, 4);
	
	m_ctrlGridBar.Create(_T("TGrid2 Bar"), WS_CHILD,// | WS_BORDER | WS_VISIBLE,
		CRect(0, 0, 0, 0), this);
	
	//# Condition Make Grid #
	m_pGridCtrl = new	CCMGrid;
	m_pGridCtrl->Create(NULL, _T("Condition Make Grid"), WS_CHILD | WS_VISIBLE | WS_BORDER,
		CRect(0, 0, 300, 200), this, NULL, 0, NULL);
	
	m_ptabSet = new CDrTabCtrl();
	m_ptabSet->Create(0, this, IDC_TAB_SET);
	m_ptabSet->SetCurSel(0);

	if(m_nSeparateForm)
	{
		m_pTabConditionForItemExcavation = new CDrTabCtrl();
		m_pTabConditionForItemExcavation->Create(0, this, IDC_TAB_CONDITION_ITEMEXCAVATION);
	}
	
	TC_ITEM item;
	item.mask = TCIF_TEXT | TCIF_IMAGE;
	item.pszText = "설정";
	m_ptabSet->InsertItem(0, &item);
	
	item.pszText = "로직";
	m_ptabSet->InsertItem(1, &item);
	
	m_ctrlLogicBar.Create(_T(""), WS_CHILD,/* | WS_VISIBLE | WS_BORDER,*/ 
		CRect(0, 0, 0, 0), this);
	
	m_ctrlCommonLogic.Create(_T("일반검색"), 
		WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
		CRect(0, 0, 0, 0), this, IDC_STC_COMMON_LOGIC);
	m_ctrlCommonLogic.SetFont(&m_fontDefault);
	
	m_ctrlOrderLogic.Create(_T("순차검색"), 
		WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
		CRect(0, 0, 0, 0), this, IDC_STC_ORDER_LOGIC);
	m_ctrlOrderLogic.SetFont(&m_fontDefault);
	
	m_ctrlPortfolio.Create(_T("관심그룹"), 
		WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
		CRect(0, 0, 0, 0), this, IDC_STC_PORTFOLIO);
	m_ctrlPortfolio.SetFont(&m_fontDefault);
	
	m_ctrlMatchVal.Create(_T("값 비 교"), 
		WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
		CRect(0, 0, 0, 0), this, IDC_STC_MATCH_VAL);
	m_ctrlMatchVal.SetFont(&m_fontDefault);
	
	m_ctrlCommonLogicEdit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | CS_DBLCLKS,
		CRect(0, 0, 0, 0), this, IDC_COMMON_EDIT);
	m_ctrlCommonLogicEdit.ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	m_pShowLogicEdit = &m_ctrlCommonLogicEdit;
	
	m_ctrlOrderLogicEdit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
		CRect(0, 0, 0, 0), this, IDC_ORDER_EDIT);
	m_ctrlOrderLogicEdit.m_bUseGroup = TRUE;
	m_ctrlOrderLogicEdit.ModifyStyleEx(WS_EX_CLIENTEDGE, 0);

	m_ctrlPortEdit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER, 
		CRect(0, 0, 0, 0), this, IDC_PORTFOLIO_EDIT);
	m_ctrlPortEdit.SetFont(&m_fontDefault);
	m_ctrlPortEdit.ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	
	m_ctrlMatchValEdit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER, 
		CRect(0, 0, 0, 0), this, IDC_EDIT_MATCHVAL);
	m_ctrlMatchValEdit.SetFont(&m_fontDefault);
	m_ctrlMatchValEdit.ModifyStyleEx(WS_EX_CLIENTEDGE, 0);

	m_btnCommonEx.Create(_T(""), CRect(0, 0, 0, 0), this, IDC_COMMONEXSIZE_BTN);
	SetBtnImg(&m_btnCommonEx, "CS_Logic_Exp_Up");
	
	m_btnOrderEx.Create(_T(""), CRect(0, 0, 0, 0), this, IDC_ORDEREXSIZE_BTN);
	SetBtnImg(&m_btnOrderEx, "CS_Logic_Exp_Up");
	
	m_btnPortCheck.Create(_T(""), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_PORTCHECK_BTN);
	m_btnPortCheck.SetCheck(TRUE);
	
	m_btnMatchValEdit.Create(_T(""), CRect(0, 0, 0, 0), this, IDC_BTN_MATCHVALEDIT);
	SetBtnImg(&m_btnMatchValEdit, "CS_Match_Val");
	
	m_btnMatchValCheck.Create(_T(""), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_MATCHVALCHECK);
	m_btnMatchValCheck.SetCheck(TRUE);
	
	m_btnBracket.Create(_T(""), CRect(0, 0, 0, 0), this, IDC_BRACKET_BTN);
	SetBtnImg(&m_btnBracket, "CS_Bracket_True");
	
	m_btnBracketDel.Create(_T(""), CRect(0, 0, 0, 0), this, IDC_BRACKETDEL_BTN);
	SetBtnImg(&m_btnBracketDel, "CS_Bracket_False");
	
	m_ctrlFunBar.Create(_T(""), 
		WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER | SS_CENTERIMAGE,
		CRect(0, 0, 0, 0), this, IDC_STATIC_FUNBAR);
	m_ctrlFunBar.SetFont(&m_fontDefault);
	
	m_btnPortDel.Create(_T("Del"), CRect(0, 0, 0, 0), this, IDC_PORTDEL_BTN);
	SetBtnImg(&m_btnPortDel, 2);
	
	m_btnLogicTip.Create(_T("도움말"), CRect(0, 0, 0, 0), this, IDC_LOGICTIP_BTN);
	SetBtnImg(&m_btnLogicTip, 4);
	
	m_tipLogicInfo.Create(this, "UP");
	m_tipLogicInfo.SetFont(&m_fontDefault);
	m_tipLogicInfo.AddTool(this, (LPCTSTR)" ");
	
	m_btnInit.Create(_T("초기화"), CRect(0, 0, 0, 0), this, IDC_INITIAL_BTN);
	SetBtnImg(&m_btnInit, 4);
	
	m_btnFileSave.Create(_T("조건저장"), CRect(0, 0, 0, 0), this, IDC_USERSAVE_BTN);
	SetBtnImg(&m_btnFileSave, 4);

	m_btnTargetEdit.Create(_T("항목변경"), CRect(0, 0, 0, 0), this, IDC_TARGET_EDIT);
	SetBtnImg(&m_btnTargetEdit, 4);
	m_btnTargetEdit.ShowWindow(SW_HIDE);

	m_btnChangeSubject.Create(_T("대상변경"), CRect(0, 0, 0, 0), this, IDC_CHANGE_SUBJECT);
	SetBtnImg(&m_btnChangeSubject, 4);
	m_btnChangeSubject.ShowWindow(SW_HIDE);

	m_btnNextCondition.Create(_T(""), CRect(0, 0, 0, 0), this, IDC_BTN_NEXT_CONDITION);
	SetBtnImg(&m_btnNextCondition, "CS_Search_Next");

	m_btnPreCondition.Create(_T(""), CRect(0, 0, 0, 0), this, IDC_BTN_PRE_CONDITION);
	SetBtnImg(&m_btnPreCondition, "CS_Search_Pre");

	m_btnDefaultValue.Create(_T("기본값"), CRect(0, 0, 0, 0), this, IDC_BTN_DEFAULT_VALUE);
	SetBtnImg(&m_btnDefaultValue, 4);

	m_ctrlMapLoad.Create(this, IDC_MAPLOAD);
	
	m_hChildFrame = GetFrameWnd(this);

	AfxSetResourceHandle(hInstSave);

	if(m_nSeparateForm)
	{
		m_ptabSet->ShowWindow(SW_HIDE);
		m_pGridCtrl->ShowWindow(SW_HIDE);
		
		m_ctrlCommonLogic.ShowWindow(SW_HIDE);
		m_ctrlCommonLogicEdit.ShowWindow(SW_HIDE);
		m_btnCommonEx.ShowWindow(SW_HIDE);
		
		m_btnBracket.ShowWindow(SW_HIDE);
		m_btnBracketDel.ShowWindow(SW_HIDE);
		
		m_ctrlFunBar.ShowWindow(SW_HIDE);
	}

	return 0;
}

void CConditionMakeWnd::OnDestroy() 
{
	//	CWnd::OnDestroy();
	
	if (m_hConditionMng != NULL)
	{
		Lib_Close();
		FreeLibrary(m_hConditionMng); 
		m_hConditionMng = NULL;
	}
	
	if (m_pGridCtrl != NULL)
	{
		delete m_pGridCtrl;
		m_pGridCtrl = NULL;
	}
	
	if (m_ptabSet != NULL)
	{
		delete m_ptabSet;
		m_ptabSet = NULL;
	}

	if(m_pTabConditionForItemExcavation != NULL)
	{
		delete m_pTabConditionForItemExcavation;
		m_pTabConditionForItemExcavation = NULL;
	}
	
	m_btnCondiAdd.DestroyWindow();
	m_btnInit.DestroyWindow();
	m_btnGridTip.DestroyWindow();
	m_btnSearchRng.DestroyWindow();
	
	m_btnCommonEx.DestroyWindow();
	m_btnOrderEx.DestroyWindow();
	m_btnMatchValEdit.DestroyWindow();
	
	m_btnBracket.DestroyWindow();
	m_btnBracketDel.DestroyWindow();
	m_btnLogicTip.DestroyWindow();
	m_btnPortDel.DestroyWindow();
	
	m_btnFileSave.DestroyWindow();
	
	m_tipGridInfo.RemoveTool(this);
	m_tipLogicInfo.RemoveTool(this);
	
	m_ctrlMapLoad.DestroyWindow();
	m_DataAdmin.DeleteAll();
	
	CWnd::OnDestroy();
}

void CConditionMakeWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (m_bCreateEnd == TRUE)
		OnSize();
}

void CConditionMakeWnd::OnSizeInit()
{
	m_bCommonExState = FALSE;
	m_bOrderExState = FALSE;
	
	OnSize();
}

void CConditionMakeWnd::OnSize()
{
	CRect reClient, reTop, reBottom;
	GetClientRect(reClient);

	reTop = reClient;
	reBottom = reClient;
	int nXOffSet = 0;

	if(m_nSeparateForm == 0)
	{
		reTop.top += 3;
		reTop.bottom = reTop.top + 23;
	}	
	else
	{
		reTop.bottom = reTop.top + 26;
	}		
		
	OnSizeTopToolBar(reTop);
	
	int nLogicBarHeight = 130;

	reBottom.top = reBottom.bottom - nLogicBarHeight - 18;
	reBottom.left = nXOffSet;
	reBottom.right -= 3;
	
	if (m_ptabSet->GetSafeHwnd() != NULL)
	{
		CRect reTemp;
		reTemp = reBottom;
		
		if(m_bCommonExState && m_ptabSet->GetCurSel() == 0)
		{
			m_rtTab = reTemp;
			/*m_rtTab.bottom -= 5;*/
			m_ptabSet->SetWindowPos(&wndBottom, reTemp.left, reTemp.top - 18, reTemp.Width(), reTemp.Height() + 18, SWP_SHOWWINDOW);
		}
		else
		{
			m_rtTab = reTemp;
			m_rtTab.top = reTemp.top + 20;
			/*m_rtTab.bottom -= 5;*/
			m_ptabSet->SetWindowPos(&wndBottom, reTemp.left, reTemp.top, reTemp.Width(), reTemp.Height(), SWP_SHOWWINDOW);
		}
			
		OnSize_ConditionSet(reTemp);
		
		if(m_bCommonExState && m_ptabSet->GetCurSel() == 0)
			reTemp.top -= 18;
		
		OnSize_FunBarType2(reTemp);

		CRect rt;
		rt = reTemp;
		rt.bottom -= 4;
		rt.top = rt.bottom - 20;
		rt.left = reBottom.left + 5;
		rt.right = rt.left + 60;
		if(m_nTabIndex == 0)
		{		 
			if(m_bCommonExState)
			{
				rt.top -= 18;
				rt.bottom -= 18;
			}
			
			if(m_nSeparateForm == 0)
			{
				m_ctrlCommonLogic.MoveWindow(rt);
				m_ctrlOrderLogic.MoveWindow(rt);
				
				if(m_pGridCtrl->GetSelectGroup() == 0)
				{
					m_ctrlOrderLogic.ShowWindow(SW_HIDE);	
					m_ctrlCommonLogic.ShowWindow(SW_SHOW);
				}
				else if(m_pGridCtrl->GetSelectGroup() > 0)
				{
					m_ctrlCommonLogic.ShowWindow(SW_HIDE);					
					m_ctrlOrderLogic.ShowWindow(SW_SHOW);
				}
			}
			else
			{
				m_ctrlCommonLogic.MoveWindow(CRect(0, 0, 0, 0));
				m_ctrlOrderLogic.MoveWindow(CRect(0, 0, 0, 0));
				m_ctrlCommonLogic.ShowWindow(SW_HIDE);
				m_ctrlOrderLogic.ShowWindow(SW_HIDE);
			}
				
			
			
			int nRight = rt.right;

			rt.right = reTemp.right - 3;
			rt.left = rt.right - 20;
			m_btnBracketDel.MoveWindow(rt);
			
			rt.right = rt.left - 2;
			rt.left = rt.right - 20;
			m_btnBracket.MoveWindow(rt);

			rt.right = rt.left - 2;
			rt.left = rt.right - 20;
			m_btnCommonEx.MoveWindow(rt);

			rt.right = rt.left - 3;
			rt.left = nRight + 3;
			
			if(m_nSeparateForm == 0)
			{
				if(m_bCommonExState)
				{
					m_ctrlCommonLogicEdit.MoveWindow(rt.left, rt.top, rt.Width(), rt.Height() + 18);
					m_ctrlOrderLogicEdit.MoveWindow(rt.left, rt.top, rt.Width(), rt.Height() + 18);
				}
				else
				{
					m_ctrlCommonLogicEdit.MoveWindow(rt);
					m_ctrlOrderLogicEdit.MoveWindow(rt);
				}
				m_ctrlCommonLogicEdit.ShowWindow(SW_SHOW);
				m_ctrlOrderLogicEdit.ShowWindow(SW_HIDE);
			}
			else
			{
				m_ctrlCommonLogicEdit.MoveWindow(CRect(0, 0, 0, 0));
				m_ctrlOrderLogicEdit.MoveWindow(CRect(0, 0, 0, 0));

				m_ctrlCommonLogicEdit.ShowWindow(SW_HIDE);
				m_ctrlOrderLogicEdit.ShowWindow(SW_HIDE);
			}
			
		}
		else
		{

		}
	}
	
	reTop.top = reTop.bottom;
	reTop.bottom = reBottom.top - 4;
	reTop.left = nXOffSet;
	reTop.right = reClient.right - 3;

	if(m_bCommonExState && m_ptabSet->GetCurSel() == 0)
		reTop.bottom -= 18;
	
	if (m_pGridCtrl->GetSafeHwnd() != NULL)
		m_pGridCtrl->MoveWindow(reTop);
	
	CRect reTemp = m_pGridCtrl->GetLabelRect();
	reTemp.left = reTop.left + reTemp.Width() + 5;
	reTemp.top = reTop.top + 3;
	reTemp.right = reTemp.left + 60;
	reTemp.bottom = reTemp.top + 20;

	m_btnTargetEdit.MoveWindow(reTemp);

	if(!m_nSeparateForm)
	{
		reTemp.left = reTemp.right - 65;
		reTemp.right = reTemp.left + 60;
		
// 		m_btnChangeSubject.MoveWindow(reTemp);
// 		m_btnChangeSubject.SetWindowPos(NULL, 0, 0, 0, 0, WS_EX_TOPMOST | SWP_NOMOVE | SWP_NOSIZE);
		
		reTemp.left = reTemp.right + 2;
		reTemp.right = reClient.right - 3;
	}
	
	if(m_nSeparateForm)
	{	
		reTop.bottom = reTop.top + 23;
		
		m_rtTab = reTop;
		if(m_bLoadConditionForItemExcavation)
			m_pTabConditionForItemExcavation->MoveWindow(m_rtTab);
		
		reTop.top = reTop.bottom;
		/*reTop.bottom = reTop.top + 95;*/
		reTop.bottom = reTop.top + 92;
		
		m_rtTabCondition = reTop;
		m_ctrlMapLoad.MoveWindow(reTop.left + 1, reTop.top, reTop.Width() - 2, reTop.Height());
	
// 		reTop.left = reTop.right - 60;
// 		reTop.top = reTop.bottom + 3;
// 		reTop.bottom = reTop.top + 20;
// 		m_btnDefaultValue.MoveWindow(reTop);

// 		reTop.right = reTop.left - 1;
// 		reTop.left = reTop.right - 20;
// 		m_btnNextCondition.MoveWindow(reTop);
// 
// 		reTop.right = reTop.left - 1;
// 		reTop.left = reTop.right - 20;
// 		m_btnPreCondition.MoveWindow(reTop);

 		m_btnDefaultValue.EnableWindow(FALSE);
// 		m_btnNextCondition.EnableWindow(FALSE);
// 		m_btnPreCondition.EnableWindow(FALSE);
	}
}

void CConditionMakeWnd::OnSizeTopToolBar(CRect reClient)
{
	if(!m_btnExpand.m_hWnd  || !m_ctrlTargetBase.m_hWnd || !m_comboTargetBase.m_hWnd
		|| !m_btnGridTip.m_hWnd || !m_btnDetail.m_hWnd ||!m_btnAlertChk.m_hWnd) return;
	
	CRect reLeft, reRight, reTemp;

	if(m_nSeparateForm == 1)
		reClient.top += 3;
	
	reLeft = reClient;
	reRight = reClient;
	
	reLeft.bottom = reLeft.top + 20;
	
	reLeft.right = reLeft.left + 20;
	m_btnExpand.MoveWindow(reLeft);
	
	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + 73;
	m_ctrlTargetBase.MoveWindow(reLeft);
	
	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + 90;
	
	reTemp = reLeft;
	reTemp.bottom += reTemp.top + 150;
	m_comboTargetBase.MoveWindow(reTemp);
	
// 	if (m_saWorkDate.GetSize() > 0)
// 	{
		//# Back Test
		reLeft.left = reLeft.right + 2;
		reLeft.right = reLeft.left + 90;
		m_btnBackTestChk.MoveWindow(reLeft);
		
		reLeft.left = reLeft.right + 2;
		reLeft.right = reLeft.left + 105;
		reTemp = reLeft;
		reTemp.bottom = reTemp.top + 100;
		m_comboBackTestDay.MoveWindow(reTemp);
		
		reLeft.left = reLeft.right + 2;
		reLeft.right = reLeft.left + 55;
		reTemp = reLeft;
		reTemp.top = reLeft.top;
		//reTemp.bottom = reTemp.top + 100;
		reTemp.bottom = reTemp.top + 120; //0015075: [7127] 조건검색 과거시점검색에 "15:30" 추가요청
		m_comboBackTestTime.MoveWindow(reTemp);
	/*}*/
	
	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + 60;
	
	if(!m_nSeparateForm)
	{
		m_btnGridTip.MoveWindow(reLeft);
	}
	else
	{
		//reLeft.right += 3;
		m_btnSearchRng.MoveWindow(reLeft);
	}
	
	reRight.bottom = reRight.top + 20;
	/*
	reRight.right -= 2;
	reRight.left = reRight.right - 45;
	m_btnInit.MoveWindow(reRight);
	
	  reRight.right = reRight.left - 2;
	*/

//@유진은 "신호"체크버튼이 맨 마지막에 오도록 처리한다.
//-->
//	reRight.right -= 2;
//	reRight.left = reRight.right - GetBtnWidth(4);
//	m_btnDetail.MoveWindow(reRight);
//	if (m_bIndustry == FALSE)
//	{
//		reRight.top += 2;
//		reRight.right = reRight.left - 5;
//		reRight.left = reRight.right - 50;
//		m_btnAlertChk.MoveWindow(reRight);
//	}
	if (m_bIndustry == FALSE)
	{
		reRight.right -= 2;
		reRight.left = reRight.right - 50;
		m_btnAlertChk.MoveWindow(reRight);
	}
//<--	
}

void CConditionMakeWnd::OnSize_ConditionSet(CRect reClient)
{
	reClient.left += 4;
	reClient.top += 22;
	reClient.right -= 4;
	
	CRect reTop, reTool, reTemp;
	reTop = reClient;
	//	reTool = reClient;
	
	reTemp = reClient;
	reTemp.top += 5;
	reTemp.bottom -= LOGIC_EDIT_HEIGHT;
	
	if(m_bCommonExState && m_ptabSet->GetCurSel() == 0)
	{
		reTemp.top -= 18;
		reTemp.bottom -= 18;
	}
	
	m_ctrlMapLoad.MoveWindow(reTemp);
	
	int nTempPos;

// 	reTop.top += 3;
// 	reTop.bottom = reTop.top + 24;
// 	
// 	reTemp = reTop;
// 	reTemp.right = reTemp.left + 60;
// 	
// 	reTemp.top += 1;
// 	reTemp.bottom -= 1;
// 	if(m_nTabIndex == 1)
// 		m_ctrlCommonLogic.MoveWindow(reTemp);
// 	reTemp.top += 1;
// 	reTemp.bottom -= 1;
// 	
// 	nTempPos = reTemp.right;
// 	
// 	reTemp.right = reTop.right - 2;
// 	reTemp.left = reTemp.right - 20;
// 	if(m_nTabIndex == 1)
// 		m_btnBracketDel.MoveWindow(reTemp);
// 	
// 	reTemp.right = reTemp.left - 2;
// 	reTemp.left = reTemp.right - 20;
// 	if(m_nTabIndex == 1)
// 		m_btnBracket.MoveWindow(reTemp);
// 	
// 	reTemp.right = reTemp.left - 2;
// 	reTemp.left = reTemp.right - 20;
// 	if(m_nTabIndex == 1)
// 		m_btnCommonEx.MoveWindow(reTemp);
// 	
// 	reTemp.right = reTemp.left - 2;
// 	reTemp.left = nTempPos + 2;
// 	
// 	if (m_bCommonExState == TRUE)
// 		reTop.bottom += 18;
// 	reTemp.bottom = reTop.bottom;
// 	
// 	m_ctrlCommonLogicEdit.MoveWindow(reTemp);
// 	
// 	reTop.top = reTop.bottom + 3;
// 	reTop.bottom = reTop.top + 24;
// 	
// 	reTemp = reTop;
// 	reTemp.right = reTemp.left + 60;
// 	
// 	reTemp.top += 1;
// 	reTemp.bottom -= 1;
// 	m_ctrlOrderLogic.MoveWindow(reTemp);
// 	reTemp.top += 1;
// 	reTemp.bottom -= 1;
// 	
// 	nTempPos = reTemp.right;
// 	
// 	
// 	reTemp.right = reTop.right - 2;
// 	reTemp.left = reTemp.right - 20;
// 	//# --- Space
// 	
// 	reTemp.right = reTemp.left - 2;
// 	reTemp.left = reTemp.right - 20;
// 	//# --- Space
// 	
// 	reTemp.right = reTemp.left - 2;
// 	reTemp.left = reTemp.right - 20;
// 	m_btnOrderEx.MoveWindow(reTemp);
// 	
// 	reTemp.right = reTemp.left - 2;
// 	reTemp.left = nTempPos + 2;
// 	
// 	if (m_bOrderExState == TRUE)
// 		reTop.bottom += 18;
// 	reTemp.bottom = reTop.bottom;
// 	
// 	m_ctrlOrderLogicEdit.MoveWindow(reTemp);
// 
// 	//>># PortPolio
// 	reTop.top = reTop.bottom + 3;
// 	reTop.bottom = reTop.top + 24;
// 	
// 	reTemp = reTop;
// 	reTemp.right = reTemp.left + 60;
// 
// 	reTemp.top += 1;
// 	reTemp.bottom -= 1;
// 	m_ctrlPortfolio.MoveWindow(reTemp);
// 	reTemp.top += 1;
// 	reTemp.bottom -= 1;
// 
// 	nTempPos = reTemp.right;
// 
// 	reTemp.right = reTop.right - 3;
// 	reTemp.left = reTemp.right - 36;
// 	m_btnPortDel.MoveWindow(reTemp);
// 
// 	reTemp.right = reTop.right - 38;
// 	reTemp.left = reTemp.right - 25;
// 	m_btnPortCheck.MoveWindow(reTemp);
// 	
// 	reTemp.top -= 1;
// 	reTemp.bottom += 1;
// 	reTemp.right = reTemp.left - 5;
// 	reTemp.left = nTempPos + 2;
// 	m_ctrlPortEdit.MoveWindow(reTemp);
// 	reTemp.top += 1;
// 	reTemp.bottom -= 1;
// 	//m_ctrlGwansimEdit.MoveWindow(reTemp);
// 	//<<
// 	
// 	reTop.top = reTop.bottom + 3;
// 	reTop.bottom = reTop.top + 24;
// 	
// 	reTemp = reTop;
// 	reTemp.right = reTemp.left + 60;
// 	
// 	reTemp.top += 1;
// 	reTemp.bottom -= 1;
// 	m_ctrlMatchVal.MoveWindow(reTemp);
// 	reTemp.top += 1;
// 	reTemp.bottom -= 1;
// 	
// 	nTempPos = reTemp.right;
// 	
// 	reTemp.right = reTop.right - 2;
// 	reTemp.left = reTemp.right - 20;
// 	//# --- Space
// 	
// 	reTemp.right = reTemp.left - 2;
// 	reTemp.left = reTemp.right - 20;
// 	m_btnMatchValCheck.MoveWindow(reTemp);
// 	
// 	reTemp.right = reTemp.left - 2;
// 	reTemp.left = reTemp.right - 20;
// 	m_btnMatchValEdit.MoveWindow(reTemp);
// 	
// 	reTemp.top -= 1;
// 	reTemp.bottom += 1;
// 	reTemp.right = reTemp.left - 2;
// 	reTemp.left = nTempPos + 2;
// 	m_ctrlMatchValEdit.MoveWindow(reTemp);

//	----------------------------------------

	reTop.top += 6;
	reTop.bottom = reTop.top + 20;
	
	reTemp = reTop;
	reTemp.right = reTemp.left + 60;
	
	reTemp.top += 1;
	reTemp.bottom -= 1;
	if(m_nTabIndex == 1)
		m_ctrlCommonLogic.MoveWindow(reTemp);
	reTemp.top -= 1;
	reTemp.bottom += 1;
	
	nTempPos = reTemp.right;
	
	reTemp.right = reTop.right - 2;
	reTemp.left = reTemp.right - 20;
	if(m_nTabIndex == 1)
		m_btnBracketDel.MoveWindow(reTemp);
	
	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 20;
	if(m_nTabIndex == 1)
		m_btnBracket.MoveWindow(reTemp);
	
	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 20;
	if(m_nTabIndex == 1)
		m_btnCommonEx.MoveWindow(reTemp);
	
	reTemp.right = reTemp.left - 2;
	reTemp.left = nTempPos + 2;
	
	if (m_bCommonExState == TRUE)
		reTop.bottom += 18;
	reTemp.bottom = reTop.bottom;
	
	m_ctrlCommonLogicEdit.MoveWindow(reTemp);
	
	reTop.top = reTop.bottom + 3;
	reTop.bottom = reTop.top + 20;
	
	reTemp = reTop;
	reTemp.right = reTemp.left + 60;
	
	reTemp.top += 1;
	reTemp.bottom -= 1;
	m_ctrlOrderLogic.MoveWindow(reTemp);
	reTemp.top -= 1;
	reTemp.bottom += 1;
	
	nTempPos = reTemp.right;
	
	
	reTemp.right = reTop.right - 2;
	reTemp.left = reTemp.right - 20;
	//# --- Space
	
	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 20;
	//# --- Space
	
	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 20;
	m_btnOrderEx.MoveWindow(reTemp);
	
	reTemp.right = reTemp.left - 2;
	reTemp.left = nTempPos + 2;
	
	if (m_bOrderExState == TRUE)
		reTop.bottom += 18;
	reTemp.bottom = reTop.bottom;
	
	m_ctrlOrderLogicEdit.MoveWindow(reTemp);

	//>># PortPolio
	reTop.top = reTop.bottom + 3;
	reTop.bottom = reTop.top + 20;
	
	reTemp = reTop;
	reTemp.right = reTemp.left + 60;

	reTemp.top += 1;
	reTemp.bottom -= 1;
	m_ctrlPortfolio.MoveWindow(reTemp);
	reTemp.top -= 1;
	reTemp.bottom += 1;

	nTempPos = reTemp.right;

	reTemp.right = reTop.right - 3;
	reTemp.left = reTemp.right - 36;
	m_btnPortDel.MoveWindow(reTemp);

	reTemp.right = reTop.right - 38;
	reTemp.left = reTemp.right - 25;
	m_btnPortCheck.MoveWindow(reTemp);

	reTemp.right = reTemp.left - 5;
	reTemp.left = nTempPos + 2;
	m_ctrlPortEdit.MoveWindow(reTemp);
	//m_ctrlGwansimEdit.MoveWindow(reTemp);
	//<<
	
	reTop.top = reTop.bottom + 3;
	reTop.bottom = reTop.top + 20;
	
	reTemp = reTop;
	reTemp.right = reTemp.left + 60;
	
	reTemp.top += 1;
	reTemp.bottom -= 1;
	m_ctrlMatchVal.MoveWindow(reTemp);
	reTemp.top -= 1;
	reTemp.bottom += 1;
	
	nTempPos = reTemp.right;
	
	reTemp.right = reTop.right - 2;
	reTemp.left = reTemp.right - 20;
	//# --- Space
	
	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 20;
	m_btnMatchValCheck.MoveWindow(reTemp);
	
	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 20;
	m_btnMatchValEdit.MoveWindow(reTemp);
	
	reTemp.right = reTemp.left - 2;
	reTemp.left = nTempPos + 2;
	m_ctrlMatchValEdit.MoveWindow(reTemp);

	if(m_nTabIndex == 1 && m_nSeparateForm == 0)
	{
		m_ctrlCommonLogic.ShowWindow(SW_SHOW);
		m_ctrlCommonLogicEdit.ShowWindow(SW_SHOW);

		m_ctrlOrderLogic.ShowWindow(SW_SHOW);
		m_ctrlOrderLogicEdit.ShowWindow(SW_SHOW);
	}
}

void CConditionMakeWnd::OnSize_FunBar(CRect reClient)
{
	CRect reTop, reBottom, reLeft;
	
	m_ctrlFunBar.MoveWindow(reClient);
	
	reClient.left += 4;
	reClient.right -= 4;
	reClient.top += 4;
	reClient.bottom -= 4;
	
	reTop = reClient;
	reBottom = reClient;
	
	reTop.bottom = reTop.top + 20;
	reLeft = reTop;	
	
	reLeft.right = reLeft.left + 60;
	m_btnLogicTip.MoveWindow(reLeft);
	
	reTop.top = reTop.bottom + 2;
	reTop.bottom = reTop.top + 20;
	reLeft = reTop;
	
	reLeft.right = reLeft.left + 60;
	m_btnInit.MoveWindow(reLeft);

	reBottom.top = reBottom.bottom - 20;
	reLeft = reBottom;
	
	reLeft.right = reLeft.left + 60;
	m_btnFileSave.MoveWindow(reLeft);

	if(m_nSeparateForm)
	{
		m_btnInit.ShowWindow(SW_HIDE);
		m_btnLogicTip.ShowWindow(SW_HIDE);
		m_btnFileSave.ShowWindow(SW_HIDE);
	}

}

void CConditionMakeWnd::OnSize_FunBarType2(CRect reClient)
{
	CRect reTop, reBottom, reLeft;
	
	reClient.left = reClient.right - 170;
	reClient.bottom = reClient.top + 20;
	
	reTop = reClient;
	reBottom = reClient;	
	
	reLeft = reTop;
	
	reLeft.top -= 1;
	reLeft.bottom -= 1;
	
	CRect rect;
	GetClientRect(rect);
	if(reLeft.left < rect.left + 120)
		reLeft.left = rect.left + 120;
	
	//	reLeft.top -= 1;
	reLeft.left -= 14;
	reLeft.right = reLeft.left + 60;
	m_btnLogicTip.MoveWindow(reLeft);
	
	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + 60;
	m_btnInit.MoveWindow(reLeft);
	
	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + 60;
	m_btnFileSave.MoveWindow(reLeft);

	if(m_nSeparateForm)
	{
		m_btnLogicTip.ShowWindow(SW_HIDE);
		m_btnInit.ShowWindow(SW_HIDE);
		m_btnFileSave.ShowWindow(SW_HIDE);
	}
}

void CConditionMakeWnd::OnSizeLogicToolBar(CRect reClient)
{
	CRect reTop, reTool, reTemp;
	
	reTop = reClient;
	reTool = reClient;
	
	int nTempPos;
	int nToolWidth = 32 * 2 + 11; 
	/*reTool.top -= 4;*/
	reTool.top -= 6;
	reTool.bottom += 1;
	reTool.left = reTool.right - nToolWidth;
	
	//reTop.left += 2;
	reTop.right = reTool.left;
// 	reTop.top += 5;
// 	reTop.bottom = reTop.top + 24;
	reTop.top += 3;
	reTop.bottom = reTop.top + 20;
	
	reTemp = reTop;
	reTemp.right = reTemp.left + 60;
	
	reTemp.top += 1;
	reTemp.bottom -= 1;
	m_ctrlCommonLogic.MoveWindow(reTemp);
	reTemp.top -= 1;
	reTemp.bottom += 1;
	
	nTempPos = reTemp.right;
	
	reTemp.right = reTop.right - 2;
	reTemp.left = reTemp.right - 20;
	m_btnCommonEx.MoveWindow(reTemp);
	
	reTemp.right = reTemp.left - 2;
	reTemp.left = nTempPos + 2;
	
	if (m_bCommonExState == TRUE)
		reTop.bottom += 18;
	reTemp.bottom = reTop.bottom;
	
	m_ctrlCommonLogicEdit.MoveWindow(reTemp);
	
	
	reTop.top = reTop.bottom + 2;
	reTop.bottom = reTop.top + 20;
	
	reTemp = reTop;
	reTemp.right = reTemp.left + 60;
	
	reTemp.top += 1;
	reTemp.bottom -= 1;
	m_ctrlOrderLogic.MoveWindow(reTemp);
	reTemp.top -= 1;
	reTemp.bottom += 1;
	
	nTempPos = reTemp.right;
	
	reTemp.right = reTop.right - 2;
	reTemp.left = reTemp.right - 20;
	m_btnOrderEx.MoveWindow(reTemp);
	
	reTemp.right = reTemp.left - 2;
	reTemp.left = nTempPos + 2;
	
	if (m_bOrderExState == TRUE)
		reTop.bottom += 18;
	reTemp.bottom = reTop.bottom;
	
	m_ctrlOrderLogicEdit.MoveWindow(reTemp);
	
	//>>
	//# PortPolio
	reTop.top = reTop.bottom + 2;
	reTop.bottom = reTop.top + 20;
	
	reTemp = reTop;
	reTemp.right = reTemp.left + 60;

	reTemp.top += 1;
	reTemp.bottom -= 1;
	m_ctrlPortfolio.MoveWindow(reTemp);
	reTemp.top -= 1;
	reTemp.bottom += 1;

	nTempPos = reTemp.right;

	reTemp.right = reTop.right - 3;
	reTemp.left = reTemp.right - 36;
	m_btnPortDel.MoveWindow(reTemp);

	reTemp.right = reTop.right - 38; //reTemp.right = reTop.right - 3;
	reTemp.left = reTemp.right - 16;
	m_btnPortCheck.MoveWindow(reTemp);

	reTemp.right = reTemp.left - 5;
	reTemp.left = nTempPos + 2;
	m_ctrlPortEdit.MoveWindow(reTemp);
	//m_ctrlGwansimEdit.MoveWindow(reTemp);
	//<<
	
	reTop.top = reTop.bottom + 2;
	reTop.bottom = reTop.top + 20;
	
	reTemp = reTop;
	reTemp.right = reTemp.left + 60;
	
	reTemp.top += 1;
	reTemp.bottom -= 1;
	m_ctrlMatchVal.MoveWindow(reTemp);
	reTemp.top -= 1;
	reTemp.bottom += 1;
	
	nTempPos = reTemp.right;
	
	reTemp.right = reTop.right - 3;
	reTemp.left = reTemp.right - 16;
	m_btnMatchValCheck.MoveWindow(reTemp);
	
	reTemp.right = reTemp.left - 4;
	reTemp.left = reTemp.right - 20;
	m_btnMatchValEdit.MoveWindow(reTemp);
	
	reTemp.right = reTemp.left - 2;
	reTemp.left = nTempPos + 2;
	m_ctrlMatchValEdit.MoveWindow(reTemp);
	
	
	
	m_ctrlFunBar.MoveWindow(reTool);
	
	nTempPos = reTool.bottom;
	
	reTool.top += 12;
	reTool.left += 4;
	
	reTool.bottom = reTool.top + 20;
	reTemp = reTool;
	
	reTemp.right = reTemp.left + 32;
	if (m_btnBracket.GetSafeHwnd() != NULL )
		m_btnBracket.MoveWindow(reTemp);
	
	reTemp.left = reTemp.right + 2; 
	reTemp.right = reTemp.left + 32;
	m_btnBracketDel.MoveWindow(reTemp);
	
	
	reTool.bottom = nTempPos - 4;
	reTool.top = reTool.bottom - 20;
	//reTool.right = reTool.left + 32 * 2 + 2;
	reTool.right = reTool.left + 60;
	m_btnFileSave.MoveWindow(reTool);
	
	reTool.bottom = reTool.top - 2;
	reTool.top = reTool.bottom - 20;
	reTemp = reTool;
	
	//reTemp.right = reTemp.left + 32;
	//m_btnPortDel.MoveWindow(reTemp);
	
	reTemp.left = reTemp.right + 2; 
	reTemp.right = reTemp.left + 60;
	m_btnLogicTip.MoveWindow(reTemp);
}

void CConditionMakeWnd::DebugLogicPacket()
{
	CString strTemp;
	m_ctrlCommonLogicEdit.GetLogicPacketOneString(strTemp);
	TRACE("\n# Common Logic : " + strTemp);
	
	strTemp = "";
	m_ctrlOrderLogicEdit.GetLogicPacketGroupString(strTemp);
	TRACE("\n# Order Logic : " + strTemp);
}

void CConditionMakeWnd::DebugMapInfoView(CStringArray &saMapInfo)
{
	//# Debug View #
	TRACE("\n## Condition Add Info ##");
	TRACE("\n  => Info Cnt : %d", saMapInfo.GetSize());
	TRACE("\n  => Tag : %s", saMapInfo.GetAt(0));
	TRACE("\n  => Title : %s", saMapInfo.GetAt(1));
	TRACE("\n  => File Name : %s", saMapInfo.GetAt(2));
	TRACE("\n  => Gubun : %s", saMapInfo.GetAt(3));
	TRACE("\n  => Type : %s", saMapInfo.GetAt(4));
	TRACE("\n  => Set Values : %s", saMapInfo.GetAt(5));
	TRACE("\n  => Result Field ID : %s", saMapInfo.GetAt(6));
}

void CConditionMakeWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	//20100827 이문수 주석처리>>
	//CRect reClient;
	//GetClientRect(reClient);
	//dc.FillSolidRect(reClient, m_clrMainSkin);
	//20100827 이문수 주석처리<<

	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;

	if(!m_nSeparateForm)
	{
		HPEN hOldPen;
		hOldPen = (HPEN)dc.SelectObject(pwndMainFrm->GetAxPen(pwndMainFrm->GetAxColor(13), 1, PS_SOLID));
		
		MoveToEx(dc.m_hDC, m_rtTab.right - 1, m_rtTab.top, NULL);
		LineTo(dc.m_hDC, m_rtTab.right - 1,  m_rtTab.bottom);
		
		MoveToEx(dc.m_hDC, m_rtTab.right - 1, m_rtTab.bottom - 1, NULL);
		LineTo(dc.m_hDC, m_rtTab.left, m_rtTab.bottom - 1);
		
		MoveToEx(dc.m_hDC, m_rtTab.left, m_rtTab.bottom - 1, NULL);
		LineTo(dc.m_hDC, m_rtTab.left, m_rtTab.top);
		dc.SelectObject(hOldPen);
	}

	if(m_nSeparateForm)
	{
 		m_ptabSet->ShowWindow(SW_HIDE);
// 		m_pGridCtrl->ShowWindow(SW_HIDE);
// 		
// 		m_ctrlCommonLogic.ShowWindow(SW_HIDE);
// 		m_ctrlCommonLogicEdit.ShowWindow(SW_HIDE);
// 		m_btnCommonEx.ShowWindow(SW_HIDE);
// 		
// 		m_btnBracket.ShowWindow(SW_HIDE);
// 		m_btnBracketDel.ShowWindow(SW_HIDE);
// 		
// 		m_ctrlFunBar.ShowWindow(SW_HIDE);

		HPEN hOldPen;
		hOldPen = (HPEN)dc.SelectObject(pwndMainFrm->GetAxPen(pwndMainFrm->GetAxColor(13), 1, PS_SOLID));
		
		MoveToEx(dc.m_hDC, m_rtTabCondition.right - 1, m_rtTabCondition.top - 1, NULL);
		LineTo(dc.m_hDC, m_rtTabCondition.right - 1,  m_rtTabCondition.bottom);
		
		MoveToEx(dc.m_hDC, m_rtTabCondition.right - 1, m_rtTabCondition.bottom, NULL);
		LineTo(dc.m_hDC, m_rtTabCondition.left, m_rtTabCondition.bottom);
		
		MoveToEx(dc.m_hDC, m_rtTabCondition.left, m_rtTabCondition.bottom, NULL);
		LineTo(dc.m_hDC, m_rtTabCondition.left, m_rtTabCondition.top - 1);

		if(!m_bLoadConditionForItemExcavation)
		{	
			MoveToEx(dc.m_hDC, m_rtTabCondition.right, m_rtTabCondition.top - 1, NULL);
			LineTo(dc.m_hDC, m_rtTabCondition.left - 1, m_rtTabCondition.top - 1);
		}

		dc.SelectObject(hOldPen);
	}
}

HBRUSH CConditionMakeWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

	int test = pWnd->GetDlgCtrlID();
	
	if (pWnd->GetDlgCtrlID() == IDC_BTN_ALERTCHK)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));
		hbr = m_brushStatic;
	}
	else if (pWnd->GetDlgCtrlID() == IDC_BTN_MATCHVALCHECK ||
		pWnd->GetDlgCtrlID() == IDC_PORTCHECK_BTN)
	{
		CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pwndMain;
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(pMainWnd->GetAxColor(66));
		hbr = m_brushTab;
	}
	else if(pWnd->GetDlgCtrlID() == IDC_TARGET_STATIC_INDICATE)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)m_brushTagetEdit;
	}
	else if(pWnd->GetDlgCtrlID() == IDC_BTN_MATCHVALEDIT)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(m_clrMainSkin);
		hbr = m_brushStatic;
	}
	
	else if (nCtlColor == CTLCOLOR_STATIC)
	{
		if (pWnd->GetDlgCtrlID() == IDC_STATIC_FUNBAR)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(m_clrMainSkin);
			hbr = m_brushStatic;
		}
		else if(pWnd->GetDlgCtrlID() == IDC_MAPLOAD ||
			pWnd->GetDlgCtrlID() == IDC_STC_COMMON_LOGIC ||
			pWnd->GetDlgCtrlID() == IDC_STC_ORDER_LOGIC ||
			pWnd->GetDlgCtrlID() == IDC_STC_PORTFOLIO ||
			pWnd->GetDlgCtrlID() == IDC_STC_MATCH_VAL)
		{
			CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pwndMain;
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(pMainWnd->GetAxColor(66));
			hbr = m_brushTab;
		}
		else
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(RGB(230, 230, 230));
			hbr = m_brushStatic;
		}
	}
	
	return hbr;
}

//[20110811:alzioyes]관심추가시의 처리루틴 추가.
BOOL CConditionMakeWnd::SetCondition_Port(BOOL bMode, XMLTreeInfo* pstXMLTreeInfo)
{
	BOOL bResult = FALSE;

	char sKey[4]={0,};
	memcpy(sKey, (LPCSTR)pstXMLTreeInfo->strName, 2);

	PortPolio  stPortPolio;
	stPortPolio.nType =	0;	//# 0 : PortPolio, 1 : Account
	stPortPolio.strPortNumber = sKey;
	stPortPolio.strPortName = pstXMLTreeInfo->strName.Mid(3);

	m_ctrlPortEdit.AddPortData(stPortPolio);

#if(1)
	CStringArray temp;
	CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pwndMain;
	if (!pMainWnd->IsExistPortItem(sKey))
	{
		pMainWnd->m_bPortAdd = TRUE;
		pMainWnd->GetData_PortItem(temp, temp, atoi(sKey));
	}
#endif
	return bResult;
}

BOOL CConditionMakeWnd::SetCondition(BOOL bMode, XMLTreeInfo* pstXMLTreeInfo)
{
	
	BOOL bResult = FALSE;
	
	int nGroup = m_stSelectCondition.nGroup;
	int nIndex = m_stSelectCondition.nIndex;		// 2009.11.09 -hidden-

	if ((bMode == FALSE) && (m_stSelectCondition.nRow == -1))		//# Insert
		return bResult;
	
	//[0000052]관심그룹 추가일때는 별도로 처리한다.
	if(pstXMLTreeInfo->strTag=="PORT")
	{
		return SetCondition_Port(bMode, pstXMLTreeInfo);
	}

	if (pstXMLTreeInfo->nItemType == (int)TREE_SELECT_TYPE_ITEM_COMM)			//# Common
	{
		if (nGroup > 0)
		{
			SetMessageBox("일반 조건은 순차그룹에 추가할수 없습니다.\n\n그룹을 재선택하고 조건을 삽입하시오.", 
				(CString)PROGRAM_TITLE, MB_OK | MB_TOPMOST);
			return bResult;
		}
	}
	else if (pstXMLTreeInfo->nItemType == (int)TREE_SELECT_TYPE_ITEM_ORDER)		//# Order
	{
		if (nGroup == 0)
		{
			SetMessageBox("순차 조건은 일반그룹에 추가할수 없습니다.\n\n그룹을 재선택하고 조건을 삽입하시오.",
				(CString)PROGRAM_TITLE, MB_OK | MB_TOPMOST);
			return bResult;
		}
	}
	else if (pstXMLTreeInfo->nItemType == (int)TREE_SELECT_TYPE_LANGUAGE)		//# Language
	{
		if (nGroup > 0)
		{
			SetMessageBox("랭귀지는 순차그룹에 추가할수 없습니다.\n\n그룹을 재선택하고 조건을 삽입하시오.", 
				(CString)PROGRAM_TITLE, MB_OK | MB_TOPMOST);
			return bResult;
		}
		
		//# Check Inserted Language => Just One Language!
		if (m_DataAdmin.GetLanguageConditionCnt() > 0)
		{
			SetMessageBox("랭귀지 조건이 이미 추가되어 있습니다.\n\n랭귀지 조건은 1개만 허용됩니다.\n\n"
				"기존 랭귀지 조건을 삭제하고 신규 랭귀지를 추가하여 주십시요.", (CString)PROGRAM_TITLE, MB_OK | MB_TOPMOST);
			return bResult;
		}
	}
	
	//# Alert 조건 구분 
	//KHD : 조건 추가는 되어 검색은 되어야 하고 실시간 버튼클릭시는 되면 안된다.
	//BOOL bAlretFlag = m_btnAlertChk.GetCheck();
	

	//if (bAlretFlag ==  TRUE)
	//{
	//	if (pstXMLTreeInfo->nNonAlert == 1)
	//	{
	//		SetMessageBox("선택 조건은 Alert용으로 부적합한 조건입니다. \n 조건을 재선택하십시오.", 
	//			(CString)PROGRAM_TITLE, MB_OK | MB_TOPMOST);
	//		return bResult;
	//	}
	//}
	//END
	if (pstXMLTreeInfo == NULL)
		return bResult;
	
	
	if (m_ctrlMapLoad.m_strAfterMapTag != pstXMLTreeInfo->strTag)
	{
		if (m_ctrlMapLoad.SetDrewConditionMap(pstXMLTreeInfo, nGroup) == TRUE)
			m_ctrlMapLoad.SetDefultToMapValues(pstXMLTreeInfo);
		else
		{
			if(m_nSeparateForm == 0)
				SetMessageBox("선택한 지표의 Map 파일이 존재 하지 않습니다.\n"
				"고객 센터로 문의 하시기 바랍니다.", (CString)PROGRAM_TITLE, MB_OK);
			else
				SetMessageBox("선택한 지표의 Map 파일이 존재 하지 않습니다.\n"
				"고객 센터로 문의 하시기 바랍니다.", (CString)PROGRAM_TITLE_BRIGHT, MB_OK);
			return bResult;
		}
	}
	else
		m_ctrlMapLoad.SetDefultToMapValues(pstXMLTreeInfo);
	
	OneConditionInfo  stOneConditionInfo;	//## One Condition Info
	
	//## Condition Cnt Check! ##
	int nTotCondition = 0;
	//for (int nTempGroup = 0; nTempGroup <= m_DataAdmin.GetMaxGroup(); nTempGroup++)
	for (int nTempGroup = 0; nTempGroup <= m_DataAdmin.GetConditionCnt(); nTempGroup++)
		nTotCondition += m_DataAdmin.GetGroupToConditionCnt(nTempGroup);
	
	if (nTotCondition + 1 > MAX_IDXCNT)
	{
		if(m_nSeparateForm == 0)
			SetMessageBox("조건은 25까지 입력 가능합니다.", (CString)PROGRAM_TITLE, MB_OK);
		else
			SetMessageBox("조건은 25까지 입력 가능합니다.", (CString)PROGRAM_TITLE_BRIGHT, MB_OK);
		return bResult;
	}
	
	stOneConditionInfo.bFalg = TRUE;
	stOneConditionInfo.nNotUserFlag = pstXMLTreeInfo->nNotUser;
	stOneConditionInfo.nGubunUserFlag = pstXMLTreeInfo->nGubun;
	
	stOneConditionInfo.strTag = pstXMLTreeInfo->strTag;
	stOneConditionInfo.strMapFile = pstXMLTreeInfo->strMapFile;
	
	stOneConditionInfo.nNonAlert = pstXMLTreeInfo->nNonAlert;
	stOneConditionInfo.nChartValSet = pstXMLTreeInfo->nChartValSet;
	
	stOneConditionInfo.strName = pstXMLTreeInfo->strName;
	stOneConditionInfo.nTargetBaseType = pstXMLTreeInfo->nTargetBase_Type;
	
	//## Get Map Loader Info ##
	//#=> Set User_Defind
	BOOL bHaveTerm = FALSE;
	if (m_ctrlMapLoad.GetMapValuesToStruct(pstXMLTreeInfo->strTag,
		stOneConditionInfo.stCondition, 
		stOneConditionInfo.strText,
		bHaveTerm, nGroup) == TRUE)
	{
		CString strOtherTitle;
		CONVERT_TYPETOTITLE(pstXMLTreeInfo->nItemType, strOtherTitle);
		if (strOtherTitle.IsEmpty() == FALSE)
			stOneConditionInfo.strText = "[" + strOtherTitle + "] " + pstXMLTreeInfo->strName;
	}
	else
		return bResult;
	
	//# Set TargetBase Values! #
	int nSTerm, nETerm;
	int nTargetBase;
	if (nGroup > 0) //# 순차
	{
		m_pGridCtrl->GetFooterData(nGroup, nSTerm, nETerm);
		
		stOneConditionInfo.stCondition.STerm = nSTerm;
		stOneConditionInfo.stCondition.ETerm = nETerm;
		
		nTargetBase = m_pGridCtrl->GetTargetBase(nGroup);
	}
	else	//# 일반
	{
		if (m_nTargetBaseIndex == 0)
			nTargetBase = 0;
		else if (m_nTargetBaseIndex > 0)
			nTargetBase = m_nTargetBaseIndex - 1;
		
		if (pstXMLTreeInfo->nTargetBase_Type == 0)
		{
			//--> 0016389: [7127] 조건추가한다음 N분일괄 설정후 조건 추가했을때 동일한주기로 추가되지 않는현상
			if (m_nTargetBaseIndex > 0)
			{
				nTargetBase = TARGETINDEX_TYPE1[m_nTargetBaseIndex-1];
				stOneConditionInfo.stCondition.TargetBase = TARGETINDEX_TYPE1[m_nTargetBaseIndex-1];
			}
			else			
				stOneConditionInfo.stCondition.TargetBase = TARGETINDEX_TYPE1[m_nTargetBaseIndex];
			//<--
		}
		else if (pstXMLTreeInfo->nTargetBase_Type == 1)
			stOneConditionInfo.stCondition.TargetBase = TARGETINDEX_TYPE2[0];
		else if (pstXMLTreeInfo->nTargetBase_Type == 2)
		{
			// stOneConditionInfo.stCondition.TargetBase = TARGETINDEX_TYPE3[m_nTargetBaseIndex];
			//--> 0015707: [7127] "당일내발생" 내 조건에 2분, 20분, 120분 주기 추가요청
			int nTargetIndex = (m_nTargetBaseIndex < 4? 0 : m_nTargetBaseIndex - 4);
			stOneConditionInfo.stCondition.TargetBase = TARGETINDEX_TYPE3[nTargetIndex];
			//<--
		}
		if (bHaveTerm == FALSE)
		{
			stOneConditionInfo.stCondition.STerm = 0;
			stOneConditionInfo.stCondition.ETerm = 1;
		}
	}
	
	//# 검색 구분 
	if (pstXMLTreeInfo->nCondiType < 0)
		return bResult;
	
	CString strTemp;
	strTemp.Format("%d", pstXMLTreeInfo->nCondiType);
	memcpy(&stOneConditionInfo.stCondition.IndexType, strTemp.GetBuffer(0), sizeof(char));
	
	//# 검색형태
	if ((pstXMLTreeInfo->nItemType == (int)TREE_SELECT_TYPE_ITEM_COMM) ||
		(pstXMLTreeInfo->nItemType == (int)TREE_SELECT_TYPE_ITEM_ORDER))
		stOneConditionInfo.stCondition.SearchType = '0';	//# 일반
	else if (pstXMLTreeInfo->nItemType == (int)TREE_SELECT_TYPE_PORTPOLIO)
	{
		
	}
	else if (pstXMLTreeInfo->nItemType == (int)TREE_SELECT_TYPE_LANGUAGE)
		stOneConditionInfo.stCondition.SearchType = '1';	//# 사용자 정의
		
	
	//## Element 정렬
	if (m_nElementIndex > _countof(CONDITION_INDEX) - 1)
	{
		if (SetMessageBox("더 이상 추가할 심볼이 없습니다.\n심볼을 재정리 하시겠습니까?", 
			(CString)PROGRAM_TITLE, MB_OKCANCEL) == IDOK)
		{
			if(m_DataAdmin.GetConditionCnt() == _countof(CONDITION_INDEX))
			{
				SetMessageBox("52개의 조건을 모두 사용하였습니다. 더 이상 추가할 수 없습니다.\n조건을 삭제하십시요.");
				return FALSE;
			}
			m_nElementIndex = SetAlignElement();
	
		}
			
		else
			return bResult;

		
	}
	
	BOOL bTargetEnable = TRUE;
	if ((nGroup == 0) && (m_nTargetBaseIndex == 0))
		bTargetEnable = FALSE;

	if (bMode == TRUE)		//# Add
	{
		m_DataAdmin.MakeGroupStruct(nIndex, nGroup);				// 2009.11.13 -hidden-

		if (m_DataAdmin.AddCondition(nGroup, m_nElementIndex, stOneConditionInfo, pstXMLTreeInfo->strResultViewID,
			(pstXMLTreeInfo->nResultViewDefVal == 1) ? TRUE : FALSE) == FALSE)
		{
			SetMessageBox("데이터 입력 오류", (CString)PROGRAM_TITLE, MB_OK);
			return bResult;
		}
		
		m_pGridCtrl->SetAddCondition(nGroup, 
			TRUE,					//# Condition View
			m_nElementIndex,	//# Element Index
			(stOneConditionInfo.stCondition.Reverse == '1')?TRUE:FALSE,						//# Not State 
			pstXMLTreeInfo->nTargetBase_Type + 1,		//# Target Type
			nTargetBase,		//# Target Index
			bTargetEnable,	//# Target Enable
			stOneConditionInfo.strText,				//# 완성 구문 
			!pstXMLTreeInfo->strResultViewID.IsEmpty(),		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
			pstXMLTreeInfo->nResultViewDefVal);			//# 값보기
		
		m_pGridCtrl->SetSelectRow(nGroup, m_pGridCtrl->GetGroupRowCnt(nGroup) - 1, 0);
		//m_pGridCtrl->Invalidate();

		if(m_DataAdmin.GetConditionCnt() != 0)
			m_btnBackTestChk.EnableWindow(TRUE);
		
		//# Select Map Info
		m_stSelectCondition.nGroup = nGroup;
		m_stSelectCondition.nIndex = nIndex;
		m_stSelectCondition.nRow = m_pGridCtrl->GetGroupRowCnt(nGroup) - 1;
		m_stSelectCondition.strElement = CONDITION_INDEX[m_nElementIndex];
		m_stSelectCondition.strTag = pstXMLTreeInfo->strTag;
		m_stSelectCondition.strText = stOneConditionInfo.strText;	// 20110216 박찬모 관심종목 mgridctrl에 표시
		
		//## Logic Edit ##
		int nOperation = 0;  //#<= 'and'
		if (nGroup == 0) //# 일반
			m_ctrlCommonLogicEdit.AddOneElement(m_nElementIndex, nOperation);
		else //if (nGroup > 0)  //# 순차
		{
			CString strElement = m_pGridCtrl->GetElementInfo(nGroup, m_pGridCtrl->m_nSelectRow);
			char ch = strElement.GetAt(0);
			m_ctrlOrderLogicEdit.MakeGroupStruct(/*nIndex*/(int)ch, nGroup);		// 2009.11.13 -hidden-
			m_ctrlOrderLogicEdit.AddGroupElement(nGroup, m_nElementIndex, nOperation);
			m_ctrlOrderLogicEdit.AddOneElement(m_nElementIndex, nOperation); // 20110623 박찬모 순차검색 저장 오류 >>
		}
	}
	else if (bMode == FALSE)		//# Insert
	{
		CString strElement;
		if (m_DataAdmin.InsertCondition(nGroup,	
			strElement,
			m_nElementIndex, 
			stOneConditionInfo, 
			pstXMLTreeInfo->strResultViewID,
			(pstXMLTreeInfo->nResultViewDefVal == 1) ? TRUE : FALSE) == FALSE)
		{
			SetMessageBox("데이터 입력 오류", (CString)PROGRAM_TITLE, MB_OK);
			return bResult;
		}
		
		//## Logic Edit ##
		int nOperation = 0;  //#<= 'and'
		if (nGroup == 0) //# 일반
			m_ctrlCommonLogicEdit.InsertOneElement(strElement, m_nElementIndex, nOperation);
		else //if (nGroup > 0)  //# 순차
			m_ctrlOrderLogicEdit.InsertGroupElement(strElement, nGroup, m_nElementIndex, nOperation);
	}
	
	//# Back Test Date Set!
	if (m_btnBackTestChk.GetCheck() == TRUE)
	{
		int nMinTargetBase = m_DataAdmin.GetMinTargetBase();
		if (nMinTargetBase != 100)
			SetBackDateSearch(m_saBackDateCnt.GetAt(nMinTargetBase));
	}
	
	m_nElementIndex++;
	
	if (m_DataAdmin.GetGroupToConditionCnt(0) > 0)
		m_comboTargetBase.EnableWindow(TRUE);
	
	SetFileSaveState(FALSE);

	// 20110315 박찬모 과거시점 검색시 대상시간 "개별"로만 설정 >>
	if(m_btnBackTestChk.GetCheck())
	{
		m_comboTargetBase.EnableWindow(FALSE);
	}
	// 20110315 박찬모 과거시점 검색시 대상시간 "개별"로만 설정 <<
	
	return TRUE;
}

int CConditionMakeWnd::SetAlignElement()
{
	int nResult  = 0;
	
	//# Data Align!
	nResult = m_DataAdmin.SetElementStandLine();
	
	//# Grid Align!
	nResult = m_pGridCtrl->SetAlignElement();
	
	//# LogigEdit Align!
	nResult = 0;
	nResult = m_ctrlCommonLogicEdit.SetOneElementStandLine();
	nResult = m_ctrlOrderLogicEdit.SetGroupElementStandLine(nResult);
	
	m_ctrlCommonLogicEdit.ReLoadAll();
	m_ctrlOrderLogicEdit.ReLoadAll();
	
	if (m_DataAdmin.GetConditionCnt() != nResult)
		SetMessageBox("Element Count Not Same!", PROGRAM_TITLE, MB_OK);
	
	return nResult;
}

void CConditionMakeWnd::SetBackDateSearch(CString strDateCnt)
{
	if (atoi(strDateCnt) == -1)
		return;
	
	int nCurSel = m_comboBackTestDay.GetCurSel();
	if (atoi(strDateCnt) > m_saWorkDate.GetSize())
	{
		CString strMessage = "과거 검색 날짜 구간 오류!";
		if(m_nSeparateForm == 0)
			SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
		else
			SetMessageBox(strMessage, (CString)PROGRAM_TITLE_BRIGHT, MB_ICONWARNING | MB_OK);
		
		return;
	}
	m_comboBackTestDay.ResetContent();
	
	CString strTemp;
	for (int nIndex = 0; nIndex < atoi(strDateCnt); nIndex++)
	{
		strTemp = m_saWorkDate.GetAt(nIndex);
		strTemp.Format("%2d %s-%s-%s", nIndex, strTemp.Left(4), strTemp.Mid(4, 2), strTemp.Mid(6));
		m_comboBackTestDay.AddString(strTemp);
	}
	
	if (nCurSel < 0 || nCurSel >= atoi(strDateCnt))
		nCurSel = 0;

	m_comboBackTestDay.SetCurSel(nCurSel);
}

int CConditionMakeWnd::GetFileToGroupCnt(CString strFullFilePath)
{
	int nResult = -1;
	
	MSXML::IXMLDOMNodePtr			    pNConditions;
	MSXML::IXMLDOMNamedNodeMapPtr	pNAConditions;
	
	try {
		MSXML::IXMLDOMDocumentPtr pDOMConditionIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
		pDOMConditionIndex->async = false;
		pDOMConditionIndex->load(_bstr_t(strFullFilePath));
		
		pNConditions = pDOMConditionIndex->selectSingleNode("CONDITIONS");
		pNAConditions = pNConditions->attributes;
		MSXML::IXMLDOMNodePtr	pNAttributeTemp;
		
		pNAttributeTemp = pNAConditions->getNamedItem("GROUP_CNT");
		nResult = atoi((PCHAR)pNAttributeTemp->text);
	} 
	catch (_com_error e)
	{
		nResult = -1;
	}
	
	return nResult;
}

int CConditionMakeWnd::IsOrderCondition(CString strFullFilePath)
{
	int nResult = 0;				//# -1 : 실패, 0 : 일반, 1 : 순차
	int nGroupOffset = 0;
	int nOffset = 0;

	MSXML::IXMLDOMNodePtr			pNConditions;
	MSXML::IXMLDOMNamedNodeMapPtr	pNAConditions;
	
	MSXML::IXMLDOMNodeListPtr		pNSCondition;
	MSXML::IXMLDOMNodePtr			pNCondition;
	MSXML::IXMLDOMNamedNodeMapPtr	pNACondition;
	
	CString strTemp;
	CString strLogic, strVersion; //KHD
	HRESULT hr;
	hr = CoInitialize(NULL);
	
	try {
		MSXML::IXMLDOMDocumentPtr pDOMConditionIndex = NULL;
		pDOMConditionIndex.CreateInstance(__uuidof(MSXML::DOMDocument)); 
		pDOMConditionIndex->async = VARIANT_FALSE;
		pDOMConditionIndex->load(strFullFilePath.AllocSysString());

		pNConditions = pDOMConditionIndex->selectSingleNode("CONDITIONS");
		pNAConditions = pNConditions->attributes;
		MSXML::IXMLDOMNodePtr	pNAttributeTemp;
		
		pNSCondition = pNConditions->selectNodes("CONDITION");
		int nConditionCnt = pNSCondition->length;
		for (int nConditionIndex = 0; nConditionIndex < nConditionCnt; nConditionIndex++)
		{ 
			MSXML::IXMLDOMNodePtr	pNAttributeTemp;
			pNCondition = pNSCondition->item[nConditionIndex];				
			pNACondition = pNCondition->attributes;

			pNAttributeTemp = pNACondition->getNamedItem("GROUP_NO");
			strTemp = CString((PCHAR)pNAttributeTemp->text);

			if(nResult < atoi(strTemp))
				nResult  = atoi(strTemp);
		}
	}
	catch (_com_error e)
	{
		nResult = -1;
	}

	return nResult;
}

BOOL CConditionMakeWnd::SetLoadFile(XMLTreeInfoFile*	pstXMLTreeInfoFile, BOOL bSetInit, BOOL bOverlap/* = FALSE*/)
{
	BOOL bResult = FALSE;
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	CString strAppPath;
	strAppPath.Format("%s", pwndMainFrm->Variant(homeCC));
	
	CString strFullFilePath, strMessage;
	if ((m_stLoadedFile.strFileName == pstXMLTreeInfoFile->strFileName) &&
		(m_stLoadedFile.strFolderPath == pstXMLTreeInfoFile->strFolderPath))
	{
		if (bOverlap == FALSE)
		{
			strMessage = "선택 조건이 기존 조건과 동일합니다.";
			if(m_nSeparateForm == 0)
				SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
			else
				SetMessageBox(strMessage, (CString)PROGRAM_TITLE_BRIGHT, MB_ICONWARNING | MB_OK);
		}
		
		bResult = TRUE;
	}
	else 
	{
		CMainFrmWnd *wndMainFrm = (CMainFrmWnd*)m_pwndMain;
		
		if (pstXMLTreeInfoFile->strType.IsEmpty() == TRUE)
		{
			SetMessageBox("Not Have File Type(USER/SYS)", (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
			return FALSE;
		}
		else
			strFullFilePath = wndMainFrm->GetFolderPath(pstXMLTreeInfoFile->strType);
		
		strFullFilePath += "\\"+ pstXMLTreeInfoFile->strFolderPath + "\\"+ pstXMLTreeInfoFile->strFilePath;
		// int nGroupCnt = GetFileToGroupCnt(strFullFilePath);
		int nGroupCnt = IsOrderCondition(strFullFilePath); // -1 : 읽기 실패; 0 : 일반검색, 0 이상 : 순차검색
		if (bSetInit == TRUE)
		{
			int nTabIndex;
// 			if (nGroupCnt == 1)
// 				nTabIndex = (m_nSelectTab == 1)?1:0;
// 			else if (nGroupCnt > 0)
// 				nTabIndex = 1;
// 			else
// 				nTabIndex = -1;
			if(nGroupCnt == 0)
				nTabIndex = 0;
			else if(nGroupCnt > 0)
				nTabIndex = 1;
			else
				nTabIndex = -1;
			
			if (nTabIndex > -1)
			{
				/*if (m_nSelectTab == 0)*/
				if (m_nSelectTab != nTabIndex)
				{
					CUIntArray siData;
					siData.Add(0);        //# View Type
					siData.Add(nTabIndex);//# Select Tab
					m_pwndMain->SendMessage(WM_CONDITIONMAKE_WND, (WPARAM)NM_TAB_CHANGE, (WPARAM)&siData);
				}
	
				SetTabChange(nTabIndex);
			}
			else
			{
				strMessage = "'" + pstXMLTreeInfoFile->strFileName + "' File Load 중 Error!" +
					"\n\n 본 조건 파일은 사용하실수 없습니다.";
				if(m_nSeparateForm == 0)
					SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
				else
					SetMessageBox(strMessage, (CString)PROGRAM_TITLE_BRIGHT, MB_ICONWARNING | MB_OK);

				SetSendFileName(FALSE);
				
				return FALSE;
			}
		}
		else
		{
			if ((m_DataAdmin.GetGroupCnt() == 1) && (nGroupCnt > 1))
				m_pGridCtrl->SetGridType((int)TYPE_GRID_ORDER);
		}
		
		int nLoadState;		//# -1 : 실패, 0 : 성공, 1 : 지표초과
		
		nLoadState = SetLoadFile(strFullFilePath, m_nElementIndex);
		if (nLoadState == -1)
		{
			
			strMessage = "'" + pstXMLTreeInfoFile->strFileName + "' File Load 중 Error!" +
				"\n\n 본 조건 파일은 사용하실수 없습니다.";
			if(m_nSeparateForm == 0)
				SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
			else
				SetMessageBox(strMessage, (CString)PROGRAM_TITLE_BRIGHT, MB_ICONWARNING | MB_OK);
			
			SetSendFileName(FALSE);
			if (bSetInit == TRUE)
				OnBtnInit();
		}
		else if (nLoadState == 0)
		{
			SetSendFileName_Target(bSetInit, strFullFilePath);
			SetSendFileName(bSetInit, pstXMLTreeInfoFile);

			bResult = TRUE;
		}
		else if (nLoadState == 1)
			bResult = TRUE;
	}
	
	if (bResult == TRUE)
		m_pwndMain->SendMessage(WM_CONDITIONMAKE_WND, (WPARAM)NM_SEARCH, (WPARAM)NULL);
	
	int nGroupCnt = IsOrderCondition(strFullFilePath); // -1 : 읽기 실패; 0 : 일반검색, 0 이상 : 순차검색
	if(nGroupCnt >= 0)
		m_pGridCtrl->SetSelectRow(nGroupCnt, 0, TRUE);

	if(m_nSeparateForm == 1)
		SetTabTextConditionItemexcavation();
	
	return bResult;
}

void CConditionMakeWnd::SetSendFileName(CString strFileName, CString strFolderPath)
{
	XMLTreeInfoFile	stLoadedFile;
	SetInit_LoadedFileInfo(stLoadedFile);
	
	stLoadedFile.strFileName = strFileName;
	stLoadedFile.strFilePath = strFileName + ".xml";
	stLoadedFile.strFolderPath = strFolderPath;
	
	//# Set Info!
	CMainFrmWnd *wndMainFrm = (CMainFrmWnd*)m_pwndMain;
	wndMainFrm->GetFileSetState(stLoadedFile.strType, stLoadedFile.strFolderPath, stLoadedFile.strFilePath,
		stLoadedFile.strAlertReg, stLoadedFile.bUnitCan);				
	
	m_strSaveFileName = strFileName;	// 20110315 박찬모 조건저장 후 저장된 조건 선택
	
	SetSendFileName(TRUE, &stLoadedFile);
}

void CConditionMakeWnd::SetSendFileName_Target(BOOL bInit, CString strFullFilePath)
{
	CString szExt = strFullFilePath.Right(4);
	if (szExt.CompareNoCase(".xml") !=0)
		return;

	CString strFileCondTarget;
	strFileCondTarget = strFullFilePath.Left(strFullFilePath.GetLength()-4);
	strFileCondTarget += FILE_CONDTARGET_EXT;

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	if (pwndMainFrm == NULL)
		return;

	CConditionResultWnd* pCondResultWnd = (CConditionResultWnd*)pwndMainFrm->GetWnd("RESULT");
	if (pCondResultWnd == NULL)
		return;

	if (pCondResultWnd->ReadTargetInfo(strFileCondTarget))
		pCondResultWnd->SetTargetEditIndicate();

/*
	int nPos = strFullFilePath.ReverseFind('/');
	if (nPos < 0)
		nPos = strFullFilePath.ReverseFind('\\');
	if (nPos < 0)
		return;

	CString szDir(strFullFilePath.Left(nPos));

	CString stBaseName;
	if (m_nSeparateForm == 0)
		stBaseName = "7127_ConditionInfo.dat";	// 기본조건검색 화면.
	else
		stBaseName = "8416_ConditionInfo.dat";	// 그외 화면.

	CString strFileCondTarget, strFileFullRoot;
	strFileCondTarget = strFullFilePath.Left(strFullFilePath.GetLength()-4);
	strFileCondTarget += FILE_CONDTARGET_EXT;

	strFileFullRoot.Format("%s\\%s", szDir, stBaseName);
	CFileFind filefind;
//	if (filefind.FindFile(strFileCondTarget))
	{
//		if (::CopyFile(strFileCondTarget, strFileFullRoot, FALSE))
		{
			CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
			CConditionResultWnd* pCondResultWnd = (CConditionResultWnd*)pwndMainFrm->GetWnd("RESULT");
			pCondResultWnd->ReadTargetInfo(strFileCondTarget);
		}
	}
*/
}

void CConditionMakeWnd::SetFileSaveState(BOOL bSaveFlag)
{
	return;
	
	
	if (m_bSaveFlag == bSaveFlag)
		return;
	
	m_bSaveFlag = bSaveFlag;
	if (bSaveFlag == FALSE)
	{
		//SetInit_LoadedFileInfo(m_stLoadedFile);
		
		if (m_stLoadedFile.strFileName.Find("_(임시)") == -1)
		{
			m_stLoadedFile.strFilePath = m_stLoadedFile.strFileName + "_(임시).xml";
			m_stLoadedFile.strFileName = m_stLoadedFile.strFileName + "_(임시)";
			
			m_pwndMain->SendMessage(WM_CONDITIONMAKE_WND, (WPARAM)NM_LOAD_FINENAME_TEMP, (LPARAM)&m_stLoadedFile.strFileName);
		}
	}
	else
		m_pwndMain->SendMessage(WM_CONDITIONMAKE_WND, (WPARAM)NM_LOAD_FINENAME, (LPARAM)&m_stLoadedFile);
}

void CConditionMakeWnd::SetSendFileName(BOOL bInit, XMLTreeInfoFile	*pstXMLTreeInfoFile)
{
	BOOL bSendNull = FALSE;
	if (pstXMLTreeInfoFile == NULL)
	{
		if (bInit == TRUE)
		{
			SetInit_LoadedFileInfo(m_stLoadedFile);
			m_bSaveFlag = TRUE;
		}
		else
		{
			m_stLoadedFile.strType = "";
			m_stLoadedFile.strFolderPath = "";
			m_stLoadedFile.strFilePath = "";
			m_stLoadedFile.strFileName = "";
			m_stLoadedFile.bQuickReg = FALSE;			//# Deawoo [NEW]
			m_stLoadedFile.bUnitCan = FALSE;			//# Deawoo [NEW]
			m_stLoadedFile.strAlertReg = "FALSE";
			m_stLoadedFile.strExplain = "";
			
			bSendNull = TRUE;
		}
	}
	else
	{
		if (bInit == TRUE)
		{
			m_stLoadedFile = *pstXMLTreeInfoFile;
			m_bSaveFlag = TRUE;
		}
		else
		{
			if (m_stLoadedFile.strFileName.Find("_(임시)") == -1)
			{
				m_stLoadedFile.strFilePath = m_stLoadedFile.strFileName + "_(임시).xml";
				m_stLoadedFile.strFileName = m_stLoadedFile.strFileName + "_(임시)";
				m_pwndMain->SendMessage(WM_CONDITIONMAKE_WND, (WPARAM)NM_LOAD_FINENAME_TEMP, (LPARAM)&m_stLoadedFile.strFileName);
			}
			m_bSaveFlag = FALSE;
			return;
		}
	}
	
	if (bSendNull == TRUE)
		m_pwndMain->SendMessage(WM_CONDITIONMAKE_WND, (WPARAM)NM_LOAD_FINENAME, (LPARAM)NULL);
	else
		m_pwndMain->SendMessage(WM_CONDITIONMAKE_WND, (WPARAM)NM_LOAD_FINENAME, (LPARAM)&m_stLoadedFile);
}

int CConditionMakeWnd::SetLoadFile(CString strFullFilePath, int nOffSetElement)
{
	int nResult = 0;				//# -1 : 실패, 0 : 성공, 1 : 지표초과
	int nGroupOffset = 0;
	int nOffset = 0;

	BOOL bExistPort = FALSE; // 20110413 박찬모 사용자가 저장한 조건에 관심종목이 있었을 시 오동작 하는 현상

	MSXML::IXMLDOMNodePtr			    pNConditions;
	MSXML::IXMLDOMNamedNodeMapPtr	pNAConditions;
	
	MSXML::IXMLDOMNodeListPtr		  pNSFixHead;
	MSXML::IXMLDOMNodePtr			    pNFixHead;
	MSXML::IXMLDOMNamedNodeMapPtr	pNAFixHead;
	
	MSXML::IXMLDOMNodeListPtr		  pNSCondition;
	MSXML::IXMLDOMNodePtr			    pNCondition;
	MSXML::IXMLDOMNamedNodeMapPtr	pNACondition;
	
	MSXML::IXMLDOMNodeListPtr		  pNSFormat;
	MSXML::IXMLDOMNodePtr			    pNFormat;
	MSXML::IXMLDOMNamedNodeMapPtr	pNAFormat;
	
	CString strTemp;
	CString strLogic, strVersion; //KHD
	HRESULT hr;
	hr = CoInitialize(NULL);
	
	try {
		MSXML::IXMLDOMDocumentPtr pDOMConditionIndex = NULL;
		pDOMConditionIndex.CreateInstance(__uuidof(MSXML::DOMDocument)); 
		pDOMConditionIndex->async = VARIANT_FALSE;
		pDOMConditionIndex->load(strFullFilePath.AllocSysString());

		pNConditions = pDOMConditionIndex->selectSingleNode("CONDITIONS");
		pNAConditions = pNConditions->attributes;
		MSXML::IXMLDOMNodePtr	pNAttributeTemp;
		
		BOOL bAlertChk;
		pNAttributeTemp = pNAConditions->getNamedItem("ALERT");
		bAlertChk = (atoi((PCHAR)pNAttributeTemp->text) == 1) ? TRUE : FALSE;
		
		//## PortPolio Operation Set ##
		pNAttributeTemp = pNAConditions->getNamedItem("PORTFOLIO_OPERATOR");
		m_ctrlPortEdit.m_strAndOr = CString((PCHAR)pNAttributeTemp->text);
		
		if (m_ctrlPortEdit.m_strAndOr.IsEmpty() == TRUE)
			m_ctrlPortEdit.m_strAndOr = "or";
		
		//## PortPolio Element Set ##
		pNAttributeTemp = pNAConditions->getNamedItem("PORTFOLIO_DATA");
		strTemp = CString((PCHAR)pNAttributeTemp->text);
		CString strPortfolio; 
		int nFindPos, nFindPos2, nFindPosType;
		int nPortLength = strTemp.GetLength();
		int nCount = 0, nMaxGroupNum = 1;
		do
		{
			nFindPos = strTemp.Find('/');
			if (nFindPos == -1)
				break;
			strPortfolio = strTemp.Mid(0, nFindPos);
			
			PortPolio  strucePort;
			nFindPos2 = strPortfolio.Find("_");
			strucePort.strPortNumber = strPortfolio.Mid(0, nFindPos2); 
			
			//# 2004. 10, 27	<= Account Info Add!
			nFindPosType = strucePort.strPortNumber.Find("#");
			if (nFindPosType == -1)
				strucePort.nType = 0;
			else		//# Account
			{
				strucePort.nType = 1;
				strucePort.strPortNumber = strucePort.strPortNumber.Mid(0, strucePort.strPortNumber.GetLength() - 1);
			}
			
			strucePort.strPortName = strPortfolio.Mid(nFindPos2 + 1, strPortfolio.GetLength() - nFindPos2); 
			m_ctrlPortEdit.AddPortData(strucePort);
			
			strTemp = strTemp.Mid(nFindPos + 1, nPortLength - nFindPos);
			nCount++;
		} while(1);
		
		pNAttributeTemp = pNAConditions->getNamedItem("PORTFOLIO_CHK");
		m_btnPortCheck.SetCheck((CString((PCHAR)pNAttributeTemp->text) == "1") ? TRUE : FALSE);
		
		m_ctrlPortEdit.EnableWindow(m_btnPortCheck.GetCheck());
		m_ctrlPortEdit.Invalidate();
		
		
		pNAttributeTemp = pNAConditions->getNamedItem("TARGETBASE_TYPE");
		int nTargetBaseIndex = atoi((PCHAR)pNAttributeTemp->text);
		int nOrderTargetBaseIndex = -1;
		
		//KHD : 20101109 사용자 정의 저장 로직 불러오기
		pNAttributeTemp = pNAConditions->getNamedItem("VERSION");
		strVersion  = CString((PCHAR)pNAttributeTemp->text);

		if(strVersion == XML_VERSION_USERFILE)// 20101111 : 저장한 xml 파일의 버젼을 업해서 기존것에서 오는 오류를 차단함.
		{
			pNAttributeTemp = pNAConditions->getNamedItem("LOGIC_VALUE");
			strLogic  = CString((PCHAR)pNAttributeTemp->text);
		}
		else 
			strLogic = "";
		//END
		//## User_Define Struct Data Insert ##
		pNSCondition = pNConditions->selectNodes("CONDITION");
		int nConditionCnt = pNSCondition->length;

		m_nTotCntCondition = nConditionCnt;
		m_nSelectCondition = 0;

		for (int nConditionIndex = 0; nConditionIndex < nConditionCnt; nConditionIndex++)
		{ 
			//# Check Condition Count!
			if (nOffSetElement > 0)
			{
				if (m_DataAdmin.GetConditionCnt() + 1 > (int)MAX_IDXCNT)
				{
					if(m_nSeparateForm == 0)
						SetMessageBox("조건은 20까지 입력 가능합니다.", (CString)PROGRAM_TITLE, MB_OK);
					else
						SetMessageBox("조건은 20까지 입력 가능합니다.", (CString)PROGRAM_TITLE_BRIGHT, MB_OK);
					nResult = 1;
					break;
				}
			}
			
			OneConditionInfo	stOneConditionInfo;
			MSXML::IXMLDOMNodePtr	pNAttributeTemp;
			pNCondition = pNSCondition->item[nConditionIndex];				
			pNACondition = pNCondition->attributes;
			
			pNAttributeTemp = pNACondition->getNamedItem("CONDITION_FLAG");
			stOneConditionInfo.bFalg = (CString((PCHAR)pNAttributeTemp->text) == "1")?TRUE:FALSE;
			
			//# 조건명 셋팅
			pNAttributeTemp = pNACondition->getNamedItem("NAME");
			stOneConditionInfo.strName = CString((PCHAR)pNAttributeTemp->text);
			
			//# 조건태그셋팅(A1...)
			pNAttributeTemp = pNACondition->getNamedItem("TAGNAME");
			stOneConditionInfo.strTag = CString((PCHAR)pNAttributeTemp->text);
			
			pNAttributeTemp = pNACondition->getNamedItem("FILE_NAME");
			stOneConditionInfo.strMapFile = CString((PCHAR)pNAttributeTemp->text);
			
			
			pNAttributeTemp = pNACondition->getNamedItem("NON_ALERT");
			stOneConditionInfo.nNonAlert = atoi((PCHAR)pNAttributeTemp->text);
			
			pNAttributeTemp = pNACondition->getNamedItem("CHART_VALSET");
			stOneConditionInfo.nChartValSet = atoi((PCHAR)pNAttributeTemp->text);
			
			
			//## Grid Field View CheckBox View State ##
			pNAttributeTemp = pNACondition->getNamedItem("FIELD_VAL");
			stOneConditionInfo.bFieldVal = (CString((PCHAR)pNAttributeTemp->text) == "TRUE") ? TRUE : FALSE;
			
			//## View CheckBox State Flag ##
			pNAttributeTemp = pNACondition->getNamedItem("FIELD_STATE");
			stOneConditionInfo.bFieldState = (CString((PCHAR)pNAttributeTemp->text) == "TRUE") ? TRUE : FALSE;
			
			pNAttributeTemp = pNACondition->getNamedItem("FIELD_ENABLE");
			stOneConditionInfo.bFieldEnable = !stOneConditionInfo.bFalg;
			
			//### User_define Data Set ###
			pNAttributeTemp = pNACondition->getNamedItem("INDEX_NO");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.IndexNo = atoi(strTemp);
			
			pNAttributeTemp = pNACondition->getNamedItem("GROUP_NO");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.GroupNo = atoi(strTemp);
			
			pNAttributeTemp = pNACondition->getNamedItem("TARGETBASE_TYPE");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.nTargetBaseType = atoi(strTemp);
			
			pNAttributeTemp = pNACondition->getNamedItem("TARGETBASE");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.TargetBase = atoi(strTemp);
			
			pNAttributeTemp = pNACondition->getNamedItem("TYPE");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.Type = atoi(strTemp);
			
			pNAttributeTemp = pNACondition->getNamedItem("TYPE1");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.Type1 = atoi(strTemp);
			
			pNAttributeTemp = pNACondition->getNamedItem("TYPE2");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.Type2 = atoi(strTemp);
			
			pNAttributeTemp = pNACondition->getNamedItem("STERM");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.STerm = atoi(strTemp);
			
			pNAttributeTemp = pNACondition->getNamedItem("ETERM");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.ETerm = atoi(strTemp);
			
			pNAttributeTemp = pNACondition->getNamedItem("RANK");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.Rank = atoi(strTemp);
			
			pNAttributeTemp = pNACondition->getNamedItem("TREND");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.Trend = atoi(strTemp);
			
			pNAttributeTemp = pNACondition->getNamedItem("SIGNALCNT1");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.SignalCnt1 = atoi(strTemp);
			
			pNAttributeTemp = pNACondition->getNamedItem("SIGNALCNT2");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.SignalCnt2 = atoi(strTemp);
			
			//# RESULTVIEW => 값보기(Values)
			pNAttributeTemp = pNACondition->getNamedItem("RESULTVIEWNUM");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.ResultViewNum = atoi(strTemp);
			
			//# FIELD ID
			CString strFieldID;
			pNAttributeTemp = pNACondition->getNamedItem("FIELD_ID");
			strFieldID = CString((PCHAR)pNAttributeTemp->text);
			
			//# 값보기 
			stOneConditionInfo.bFieldState = !strFieldID.IsEmpty();
			
			//# INDEX TYPE
			pNAttributeTemp = pNACondition->getNamedItem("INDEXTYPE");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			memcpy(&stOneConditionInfo.stCondition.IndexType,
				strTemp.operator const char*(),
				sizeof(stOneConditionInfo.stCondition.IndexType));
			
			//# SEARCH TYPE
			pNAttributeTemp = pNACondition->getNamedItem("SEARCHTYPE");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			memcpy(&stOneConditionInfo.stCondition.SearchType,
				strTemp.operator const char*(),
				sizeof(stOneConditionInfo.stCondition.SearchType));
			
			//# OPERATOR
			pNAttributeTemp = pNACondition->getNamedItem("OPERATOR");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			memcpy(&stOneConditionInfo.stCondition.Operator,
				strTemp.operator const char*(),
				sizeof(stOneConditionInfo.stCondition.Operator));
			
			pNAttributeTemp = pNACondition->getNamedItem("NOT_USER");
			stOneConditionInfo.nNotUserFlag = (CString((PCHAR)pNAttributeTemp->text) == "1") ? 1 : 0;
			
			//# REVERSE
			pNAttributeTemp = pNACondition->getNamedItem("REVERSE");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			memcpy(&stOneConditionInfo.stCondition.Reverse,
				strTemp.operator const char*(),
				sizeof(stOneConditionInfo.stCondition.Reverse));
			
			pNAttributeTemp = pNACondition->getNamedItem("GUBUN_USER");
			stOneConditionInfo.nGubunUserFlag = (CString((PCHAR)pNAttributeTemp->text) == "1") ? 1 : 0;
			
			pNAttributeTemp = pNACondition->getNamedItem("GUBUN");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			memcpy(&stOneConditionInfo.stCondition.IdxContiGubun,
				strTemp.operator const char*(),
				sizeof(stOneConditionInfo.stCondition.IdxContiGubun));
			
			
			//# 2005.04.07 => 지표 추가에 대한 Packet 변경
			pNAttributeTemp = pNACondition->getNamedItem("CODE");
			
			if (pNAttributeTemp != NULL)
			{
				strTemp = CString((PCHAR)pNAttributeTemp->text);
				memcpy(&stOneConditionInfo.stCondition.Code,
					strTemp.operator const char*(),
					sizeof(stOneConditionInfo.stCondition.Code));
			}

			//# PARAM
			for (int i = 0; i < MAX_PARAM; i++)
			{
				strTemp.Format("P%d", i+1);
				pNAttributeTemp = pNACondition->getNamedItem(_bstr_t(strTemp));
				if (pNAttributeTemp == NULL)
					stOneConditionInfo.stCondition.Param[i] = 0.0;
				else
					stOneConditionInfo.stCondition.Param[i] = atof(CString((PCHAR)pNAttributeTemp->text));
			}
			
			//# CONDITION
			for (int j = 0; j < MAX_CONDI; j++)
			{
				strTemp.Format("C%d", j+1);
				pNAttributeTemp = pNACondition->getNamedItem(_bstr_t(strTemp));
				if (pNAttributeTemp == NULL)
					stOneConditionInfo.stCondition.Condition[j] = 0.0;
				else
					stOneConditionInfo.stCondition.Condition[j] = atof(CString((PCHAR)pNAttributeTemp->text));
			}
			
			//## Title Name ##
			pNFormat = pNCondition->selectSingleNode("FORMAT");
			pNAFormat = pNFormat->attributes;
			
			// 20110216 박찬모 관심종목 mgridctrl에 표시 >>
			if (stOneConditionInfo.strTag == "PORT")
			{
				stOneConditionInfo.strText.Format("[관심그룹] %s", stOneConditionInfo.strName);
				bExistPort = TRUE; // 20110413 박찬모 사용자가 저장한 조건에 관심종목이 있었을 시 오동작 하는 현상
			}
			else
				stOneConditionInfo.strText = CString((PCHAR)pNFormat->text);
			// 20110216 박찬모 관심종목 mgridctrl에 표시 <<
			
			//## Element Info ##
			OneElementInfo	stOneElement;
			
			pNAttributeTemp = pNACondition->getNamedItem("LOGIC_VIEW");
			stOneElement.bView = stOneConditionInfo.bFalg;
			
			pNAttributeTemp = pNACondition->getNamedItem("LOGIC_ELEMENT");
			stOneElement.strElement = CString((PCHAR)pNAttributeTemp->text);

			//KHD stOneElement.strElement = CONDITION_INDEX[nConditionIndex + nOffSetElement];
			for(int k =0; k< 52; k++)
			{
				if(CONDITION_INDEX[k] == stOneElement.strElement) //Element 값을 차례데로만 넣는 현상 수정
				{
					nOffSetElement = k - nConditionIndex ;
					if(nOffset < k)
						nOffset = k;
					break;
				}
			}
			//END
			pNAttributeTemp = pNACondition->getNamedItem("LOGIC_NOT");
			stOneElement.strNot = CString((PCHAR)pNAttributeTemp->text);	
			
			pNAttributeTemp = pNACondition->getNamedItem("LOGIC_OPERATION");
			stOneElement.strOperation = CString((PCHAR)pNAttributeTemp->text);
			
			pNAttributeTemp = pNACondition->getNamedItem("LOGIC_BRACKET");
			stOneElement.strBracket = CString((PCHAR)pNAttributeTemp->text);
			
			pNAttributeTemp = pNACondition->getNamedItem("LOGIC_BRACKETCNT");
			stOneElement.lBracketCount = atol((PCHAR)pNAttributeTemp->text);
			
			int nGroup = stOneConditionInfo.stCondition.GroupNo;
			if (nMaxGroupNum < nGroup)
			{
				//# Not Have Condition Group Add!
				if ((nGroup - nMaxGroupNum) > 1)
				{
					nGroupOffset++;
					nGroup = nGroup - nGroupOffset;
				}
				nMaxGroupNum = nGroup;
				m_pGridCtrl->SetAddGroup();
			}
			
			if ((nGroup > 0) && (m_pGridCtrl->GetGroupRowCnt(nGroup) == 0))
			{
				m_pGridCtrl->SetFooterData(nGroup, 
					stOneConditionInfo.stCondition.STerm,
					stOneConditionInfo.stCondition.ETerm);
				
				m_pGridCtrl->SetTargetBase(nGroup, stOneConditionInfo.stCondition.TargetBase);
			}
			
			BOOL bTargetEnable = FALSE;
			if ((nGroup == 0) && (nTargetBaseIndex > 0))
				bTargetEnable = TRUE;
			
			m_pGridCtrl->SetAddCondition(nGroup, 
				stOneConditionInfo.bFalg,					//# Condition View
				nConditionIndex + nOffSetElement,	//# Element Index
				(stOneConditionInfo.stCondition.Reverse == '1')?TRUE:FALSE,						//# Not State 
				stOneConditionInfo.nTargetBaseType + 1,		//# Target Type
				stOneConditionInfo.stCondition.TargetBase,		//# Target Index
				bTargetEnable,	//# Target Enable
				stOneConditionInfo.strText,				//# 완성 구문 
				!strFieldID.IsEmpty(),		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
				stOneConditionInfo.bFieldVal);			//# 값보기
			
			m_DataAdmin.AddCondition(nGroup,	
				nConditionIndex + nOffSetElement, 
				stOneConditionInfo, 
				strFieldID, 
				stOneConditionInfo.bFieldVal,
				TRUE);
			
			if(m_DataAdmin.GetConditionCnt() != 0)
				m_btnBackTestChk.EnableWindow(TRUE);
			
			if (nGroup == 0)
				m_ctrlCommonLogicEdit.m_listOneElement.AddTail(stOneElement);
			else
			{
				GroupElementInfo  stGroupElement;
				
				stGroupElement.nGroup = nGroup;
				stGroupElement.stOneElementInfo = stOneElement;
				m_ctrlOrderLogicEdit.m_listGroupElement.AddTail(stGroupElement); // 20110623 박찬모 순차검색 저장 오류 >>
// 2009.12.04 -hidden-
				m_ctrlOrderLogicEdit.AddGroupElement(stGroupElement);
			}
		}
		
		if (m_nSeparateForm)
		{
			m_pGridCtrl->SetSelectRow(0, m_nSelectCondition, TRUE);
			m_btnDefaultValue.EnableWindow(TRUE);
			
			if(m_nTotCntCondition <= 1)
				m_btnNextCondition.EnableWindow(FALSE);
			else
				m_btnNextCondition.EnableWindow(TRUE);
		}

		m_pGridCtrl->SetRedrewAll();

		if ((m_DataAdmin.GetGroupToConditionCnt(0) > 0) && (nOffSetElement == 0))
			m_comboTargetBase.EnableWindow(TRUE);

		//# Check Not User Alert Condition!
		CWnd* pwndParent = GetParent();
		if (nOffSetElement == 0)
			m_btnAlertChk.SetCheck(bAlertChk);
		else if (!bExistPort) // 20110413 박찬모 사용자가 저장한 조건에 관심종목이 있었을 시 오동작 하는 현상
		{
			if (bAlertChk != m_btnAlertChk.GetCheck())
			{
				int nIndex;
				CStringArray saElement;
				if (m_DataAdmin.GetNonAlertCondiHave(&saElement) == TRUE)
				{
					CString strMessage = "";
					for (nIndex = 0; nIndex < saElement.GetSize(); nIndex++)
					{
						strMessage += saElement.GetAt(nIndex);
						
						if (nIndex < saElement.GetSize() - 1)
							strMessage += ", ";
					}
					strMessage += "는 얼럿을 지원하지 안는 지표입니다.\n 해당 지표를 삭제하시겠습니까?";
					CString strTemp;
					
					if(m_nSeparateForm == 0)
						strTemp = (CString)PROGRAM_TITLE;
					else
						strTemp = (CString)PROGRAM_TITLE_BRIGHT;
					if (SetMessageBox(strMessage, strTemp, MB_OKCANCEL) == IDOK)
					{
						m_DataAdmin.SetNonAlertCondiDelete();
						
						//# UpDate MakeGrid!
						int nGroup, nRow;
						BOOL bState;
						for (nIndex = 0; nIndex < saElement.GetSize(); nIndex++)
						{
							if (GetGridPos_Element(saElement.GetAt(nIndex), nGroup, nRow) == TRUE)
							{
								if (nGroup == 0)
									bState = m_ctrlCommonLogicEdit.DeleteElement(saElement.GetAt(nIndex), TRUE);
								else if (nGroup > 0)
									bState = m_ctrlOrderLogicEdit.DeleteElement(saElement.GetAt(nIndex), TRUE);
								
								if (bState = FALSE)
								{
									if(m_nSeparateForm == 0)
										SetMessageBox("지표 삭제시 에러", (CString)PROGRAM_TITLE, MB_OK);
									else
										SetMessageBox("지표 삭제시 에러", (CString)PROGRAM_TITLE_BRIGHT, MB_OK);
								}
							}
						}
						
						m_btnAlertChk.SetCheck(TRUE);
					}
					else
						m_btnAlertChk.SetCheck(FALSE);
				}
			}
		}

		//# Back Test Date Set!
		if (m_btnBackTestChk.GetCheck() == TRUE)
		{
			int nMinTargetBase = m_DataAdmin.GetMinTargetBase();
			if (nMinTargetBase != 100)		
				SetBackDateSearch(m_saBackDateCnt.GetAt(nMinTargetBase));
		}

		m_ctrlOrderLogicEdit.ReLoadAll();
		
		if	(strLogic.IsEmpty() == FALSE)
		{
			m_ctrlCommonLogicEdit.EnableWindow(TRUE);
			m_ctrlCommonLogicEdit.SetWindowText(strLogic);
			m_ctrlCommonLogicEdit.SetCStringColor();
			m_ctrlCommonLogicEdit.Invalidate();
		}
		else
			m_ctrlCommonLogicEdit.ReLoadAll();

		m_nElementIndex = nOffset+1;	//KHD : 로직변경 다시 추가할때 그리드 Element 값 에러. + nOffSetElement;

		if (nTargetBaseIndex > 0)
			m_comboTargetBase.SetCurSel(nTargetBaseIndex);	//# => OnSelChangeTargetBase()
		else
			m_nTargetBaseIndex = nTargetBaseIndex;		//# 일반 조건 => 대상 시간 
	} 
	catch (_com_error e)
	{
		nResult = -1;
	}

	#ifdef _DEBUG
		m_DataAdmin.DebugMoveFiledInfo();
	#endif

	return nResult;
}

BOOL CConditionMakeWnd::SetSaveFile_Target(CString strFolderName, CString strFileName, LPCSTR strTargetXMLFile)
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	CConditionResultWnd* pCondResultWnd = (CConditionResultWnd*)pwndMainFrm->GetWnd("RESULT");

	CString rTargetText;
	CString strTDFFile(strTargetXMLFile);
	strTDFFile.MakeLower();
	strTDFFile.Replace(".xml", FILE_CONDTARGET_EXT);

	pCondResultWnd->WriteTargetInfo(strTDFFile);

	return TRUE;
}

BOOL CConditionMakeWnd::SetSaveFile(CString strFolderName, CString strFileName)
{
	BOOL bResult = TRUE;

	MSXML::IXMLDOMNodePtr			pNConditions;
	MSXML::IXMLDOMNamedNodeMapPtr	pNAConditions;
	
	MSXML::IXMLDOMNodePtr			pNCondition;
	MSXML::IXMLDOMNamedNodeMapPtr	pNACondition;
	
	MSXML::IXMLDOMNodePtr			pNFormat;
	MSXML::IXMLDOMNamedNodeMapPtr	pNAFormat;
	
	MSXML::IXMLDOMNodePtr			pNAttributeTemp;
	
	CoInitialize(NULL);
	MSXML::IXMLDOMDocumentPtr pDOMConditionIndex("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	
	try 
	{
		CString strTemp;
		
		MSXML::IXMLDOMProcessingInstructionPtr	pPI;
		MSXML::IXMLDOMNamedNodeMapPtr			pNAPI;
		MSXML::IXMLDOMNodePtr					pNEncoding;
		
		pPI = pDOMConditionIndex->createProcessingInstruction("xml", "version=\"1.0\"");
		pDOMConditionIndex->appendChild(pPI);
		
		pNEncoding = pDOMConditionIndex->createAttribute("encoding");
		pNEncoding->text = "KSC_5601";
		
		pNAPI = pPI->attributes;
		pNAPI->setNamedItem(pNEncoding);
		
		
		pNConditions = pDOMConditionIndex->createElement("CONDITIONS");
		pDOMConditionIndex->appendChild(pNConditions);
		pNAConditions = pNConditions->attributes;
		
		pNAttributeTemp = pDOMConditionIndex->createAttribute("VERSION");
		pNAttributeTemp->text = _bstr_t((CString)XML_VERSION_USERFILE);
		pNAConditions->setNamedItem(pNAttributeTemp);
		
		
		pNAttributeTemp = pDOMConditionIndex->createAttribute("ALERT");
		pNAttributeTemp->text = _bstr_t((m_btnAlertChk.GetCheck() == TRUE) ? "1" : "0"); 
		pNAConditions->setNamedItem(pNAttributeTemp);
		
		
		//##### 전체 조건 데이타 START #####
		int nDataCnt;
		//# 포트폴리오구성데이터 #
		PortPolio   stPort;
		nDataCnt = m_ctrlPortEdit.m_listPort.GetCount();
		POSITION posPort;
		CString strPort = "";
		CString strPortOpt = "";
		CString strType = "";
		for (int nPortIndex = 0; nPortIndex < nDataCnt; nPortIndex++)
		{
			posPort = m_ctrlPortEdit.m_listPort.FindIndex(nPortIndex);
			stPort = m_ctrlPortEdit.m_listPort.GetAt(posPort);
			
			strPortOpt  = stPort.strOperation;
			strType = (stPort.nType == 1) ? "#":"";
			strPort += stPort.strPortNumber + strType + "_" + stPort.strPortName + "/";
		}
		
		pNAttributeTemp = pDOMConditionIndex->createAttribute("PORTFOLIO_DATA");
		pNAttributeTemp->text = _bstr_t(strPort); 
		pNAConditions->setNamedItem(pNAttributeTemp);
		
		//# 포트폴리오 연산자
		pNAttributeTemp = pDOMConditionIndex->createAttribute("PORTFOLIO_OPERATOR");
		pNAttributeTemp->text = _bstr_t(strPortOpt); 
		pNAConditions->setNamedItem(pNAttributeTemp);
		
		pNAttributeTemp = pDOMConditionIndex->createAttribute("PORTFOLIO_CHK");
		pNAttributeTemp->text = _bstr_t((m_btnPortCheck.GetCheck() == TRUE) ? "1" : "0"); 
		pNAConditions->setNamedItem(pNAttributeTemp);
		
		pNAttributeTemp = pDOMConditionIndex->createAttribute("TARGETBASE_TYPE");
		strTemp.Format("%d", m_nTargetBaseIndex);
		pNAttributeTemp->text = _bstr_t(strTemp); 
		pNAConditions->setNamedItem(pNAttributeTemp);
		
		pNAttributeTemp = pDOMConditionIndex->createAttribute("GROUP_CNT");
		strTemp.Format("%d", m_DataAdmin.GetGroupCnt());
		pNAttributeTemp->text = _bstr_t(strTemp); 
		pNAConditions->setNamedItem(pNAttributeTemp);


		//KHD : 수식 저장 하기.. 20101109
		CString strUserInput;
		m_ctrlCommonLogicEdit.GetWindowText(strUserInput);
		pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_VALUE");
		strTemp.Format("%s",strUserInput);
		pNAttributeTemp->text = _bstr_t(strTemp); 
		pNAConditions->setNamedItem(pNAttributeTemp);


		//END

		ConditionGroupInfo   stConditionGroup;
		OneElementInfo       stOneElement;
		GroupElementInfo     stGroupElement;
		int nCondSize = 0;
		POSITION posLogic;

		if (m_DataAdmin.SetDataGroupSort() == FALSE)
			return FALSE;
		
		int nIndex_No = -1;

//KHD: 로직 변경 : 차례 데로 저장되도록 변경.
		nDataCnt = m_DataAdmin.m_arrGroups.GetSize();
		for(int nIndex = 0; nIndex < nDataCnt; nIndex++)
		{
			GROUP_INFO* pGroup = m_DataAdmin.m_arrGroups.GetAt(nIndex);
			nCondSize = pGroup->arrConGrpInfo.GetSize();

			int nLogicCnt, nLogicIndex;
			BOOL bHaveElement = FALSE;
			// 20110623 박찬모 순차검색 저장 오류 >>
			if(nIndex == 0)
				nLogicCnt = m_ctrlCommonLogicEdit.GetElementCount();
			else
				nLogicCnt = m_pShowLogicEdit->GetElementCount();

			BOOL bOnlyOrderLogic = FALSE;
			if(m_ctrlCommonLogicEdit.GetElementCount() == 0 && m_pShowLogicEdit->GetElementCount() > 0)
			{
				nLogicCnt = m_pShowLogicEdit->GetElementCount();
				bOnlyOrderLogic = TRUE;
			}

			BOOL bShowLogicEdit;
			if(m_pShowLogicEdit == &m_ctrlCommonLogicEdit)	// 일반검색
				bShowLogicEdit = TRUE;
			else 
				bShowLogicEdit = FALSE;																// 순차검색

			for(int k =0; k < nLogicCnt; ++k)
			{
				if(nIndex == 0 && !bOnlyOrderLogic)
				{
					posLogic = m_ctrlCommonLogicEdit.m_listOneElement.FindIndex(k);
					stOneElement = m_ctrlCommonLogicEdit.m_listOneElement.GetAt(posLogic);
				}
				else
				{
					GROUP_ELEMINFO* pElemGroup = m_ctrlOrderLogicEdit.FindGroupInfoWithGroupNumber(nIndex);
					stGroupElement = pElemGroup->arrGroupElemInfo.GetAt(k);
					stOneElement = stGroupElement.stOneElementInfo;
				}
				// 20110623 박찬모 <<

				stOneElement.strElement.TrimLeft();
				stOneElement.strElement.TrimRight();

				for(int u = 0; u < nCondSize; u++)
				{
					stConditionGroup = pGroup->arrConGrpInfo.GetAt(u);
					if (stConditionGroup.strElement == stOneElement.strElement)
					{
						break;
					}

				}
				
				if (stConditionGroup.nGroup == 0)
				{
						bHaveElement = TRUE;
						nIndex_No++  ;
				}
				else if (stConditionGroup.nGroup > 0)
				{
					GROUP_ELEMINFO* pElemGroup = m_ctrlOrderLogicEdit.FindGroupInfoWithGroupNumber(stConditionGroup.nGroup);
					if(!pElemGroup)
						continue;
					
					nLogicCnt = pElemGroup->arrGroupElemInfo.GetSize();
					
					for (nLogicIndex = 0; nLogicIndex < nLogicCnt; ++nLogicIndex)
					{
						stGroupElement = pElemGroup->arrGroupElemInfo.GetAt(nLogicIndex);
						stGroupElement.stOneElementInfo.strElement.TrimLeft();
						stGroupElement.stOneElementInfo.strElement.TrimRight();
						
						if (stConditionGroup.strElement == stGroupElement.stOneElementInfo.strElement)
						{
							bHaveElement = TRUE;
							++nIndex_No;

							break;
						}
					}
				}

				if (bHaveElement == FALSE)
					return FALSE;

				//### User_Define Values Insert ###
				MSXML::IXMLDOMNodePtr pNAttributeTemp;	
				pNCondition = pDOMConditionIndex->createElement("CONDITION");
				pNConditions->appendChild( pNCondition );
				pNACondition = pNCondition->attributes;
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("CONDITION_FLAG");
				strTemp = (stConditionGroup.stOneConditionInfo.bFalg == TRUE)?"1":"0";
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("NAME");
				strTemp.Format("%s", stConditionGroup.stOneConditionInfo.strName);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("TAGNAME");
				strTemp.Format("%s", stConditionGroup.stOneConditionInfo.strTag);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("FILE_NAME");
				strTemp.Format("%s", stConditionGroup.stOneConditionInfo.strMapFile);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("FILE_NAME");
				strTemp.Format("%s", stConditionGroup.stOneConditionInfo.strMapFile);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("NON_ALERT");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.nNonAlert);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("CHART_VALSET");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.nChartValSet);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				
				
				//## View CheckBox State Valuse Flag ##
				pNAttributeTemp = pDOMConditionIndex->createAttribute("FIELD_VAL");
				strTemp = (stConditionGroup.stOneConditionInfo.bFieldVal == TRUE) ? "TRUE" : "FALSE";
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				//## View CheckBox State Flag ##
				pNAttributeTemp = pDOMConditionIndex->createAttribute("FIELD_STATE");
				strTemp = (stConditionGroup.stOneConditionInfo.bFieldState == TRUE) ? "TRUE" : "FALSE";
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("FIELD_ENABLE");
				strTemp = (stConditionGroup.stOneConditionInfo.bFieldEnable == TRUE) ? "TRUE" : "FALSE";
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				//## Group Info ##
				pNAttributeTemp = pDOMConditionIndex->createAttribute("INDEX_NO");
				strTemp.Format("%d", nIndex_No);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("GROUP_NO");
				strTemp.Format("%d", stConditionGroup.nGroup);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("ORG_GROUP_NO");
				strTemp.Format("%d", stConditionGroup.nGroup);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				//## User_Define Struct to Insert ##
				pNAttributeTemp = pDOMConditionIndex->createAttribute("TARGETBASE_TYPE");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.nTargetBaseType);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("TARGETBASE");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.stCondition.TargetBase);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("TYPE");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.stCondition.Type);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("TYPE1");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.stCondition.Type1);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("TYPE2");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.stCondition.Type2);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("STERM");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.stCondition.STerm);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("ETERM");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.stCondition.ETerm);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("RANK");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.stCondition.Rank);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("TREND");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.stCondition.Trend);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("SIGNALCNT1");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.stCondition.SignalCnt1);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("SIGNALCNT2");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.stCondition.SignalCnt2);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("RESULTVIEWNUM");
				strTemp.Format("%d", stConditionGroup.stOneConditionInfo.stCondition.ResultViewNum);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				//### Move Fielder Data ###
				strTemp = "";
				CString strHeadID;
				int nFieldCnt = stConditionGroup.stOneConditionInfo.stHeadMoveInfo.nColCnt;
				for(int nFieldIndex = 0; nFieldIndex < nFieldCnt; nFieldIndex++)
				{
					strHeadID.Format("%d", stConditionGroup.stOneConditionInfo.stHeadMoveInfo.stHeadType[nFieldIndex].lHeadID);
					strTemp += strHeadID + "/";
				}
				pNAttributeTemp = pDOMConditionIndex->createAttribute("FIELD_ID");
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("INDEXTYPE");
				strTemp.Format("%c", stConditionGroup.stOneConditionInfo.stCondition.IndexType);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("SEARCHTYPE");
				strTemp.Format("%c", stConditionGroup.stOneConditionInfo.stCondition.SearchType);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("OPERATOR");
				if (stConditionGroup.nGroup == 0)
				{
					if ((stOneElement.strOperation == "and") || 
						(stOneElement.strOperation == "next"))
						strTemp = "1";
					else if (stOneElement.strOperation == "or")
						strTemp = "0";
				}
				else
				{
					if ((stGroupElement.stOneElementInfo.strOperation == "and") || 
						(stGroupElement.stOneElementInfo.strOperation == "next"))
						strTemp = "1";
					else if (stGroupElement.stOneElementInfo.strOperation == "or")
						strTemp = "0";
				}
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("NOT_USER");
				strTemp = (stConditionGroup.stOneConditionInfo.nNotUserFlag == 1) ? "1" : "0";
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("REVERSE");
				strTemp.Format("%c", stConditionGroup.stOneConditionInfo.stCondition.Reverse);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("GUBUN_USER");
				strTemp = (stConditionGroup.stOneConditionInfo.nGubunUserFlag == 1) ? "1" : "0";
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				pNAttributeTemp = pDOMConditionIndex->createAttribute("GUBUN");
				strTemp.Format("%c", stConditionGroup.stOneConditionInfo.stCondition.IdxContiGubun);
				pNAttributeTemp->text = _bstr_t(strTemp);			
				pNACondition->setNamedItem(pNAttributeTemp);
				
				
				//# 2005.04.07 => 지표 추가에 대한 Packet 변경
				pNAttributeTemp = pDOMConditionIndex->createAttribute("CODE");
				
				char chCode[7] = {0,};
				memcpy(chCode, stConditionGroup.stOneConditionInfo.stCondition.Code, 
					sizeof(stConditionGroup.stOneConditionInfo.stCondition.Code));
				pNAttributeTemp->text = _bstr_t(chCode);
				pNACondition->setNamedItem(pNAttributeTemp);
				
				
				int nValIndex;
				for(nValIndex = 0; nValIndex < MAX_PARAM; nValIndex++)
				{
					CString arr_temp;
					arr_temp.Format("P%d", nValIndex + 1);
					
					pNAttributeTemp = pDOMConditionIndex->createAttribute(_bstr_t(arr_temp));
					strTemp.Format("%f", stConditionGroup.stOneConditionInfo.stCondition.Param[nValIndex]);
					pNAttributeTemp->text = _bstr_t(strTemp);
					pNACondition->setNamedItem(pNAttributeTemp);
				}
				
				for (nValIndex = 0; nValIndex < MAX_CONDI; nValIndex++)
				{
					CString arr_temp;
					arr_temp.Format("C%d", nValIndex + 1);
					
					pNAttributeTemp = pDOMConditionIndex->createAttribute(_bstr_t(arr_temp));
					strTemp.Format("%f", stConditionGroup.stOneConditionInfo.stCondition.Condition[nValIndex]);
					pNAttributeTemp->text = _bstr_t(strTemp);
					pNACondition->setNamedItem(pNAttributeTemp);
				}
				
				//## Condition Text ##
				pNFormat = pDOMConditionIndex->createElement("FORMAT");
				pNCondition->appendChild( pNFormat );
				pNAFormat = pNFormat->attributes;
				
				
				//### Logic Edit Info ###
				if (stConditionGroup.nGroup == 0)
				{
					pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_ELEMENT");

					pNAttributeTemp->text = _bstr_t(stOneElement.strElement);	
					
//					pNAttributeTemp->text = _bstr_t(CONDITION_INDEX[nIndex]);
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_NOT");
					pNAttributeTemp->text = _bstr_t(stOneElement.strNot);			
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_OPERATION");
					pNAttributeTemp->text = _bstr_t(stOneElement.strOperation);			
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_VIEW");
					strTemp = (stOneElement.bView == TRUE)?"TRUE":"FALSE";
					pNAttributeTemp->text = _bstr_t(strTemp);			
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_BRACKET");
					pNAttributeTemp->text = _bstr_t(stOneElement.strBracket);			
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_BRACKETCNT");
					strTemp.Format("%d", stOneElement.lBracketCount);
					pNAttributeTemp->text = _bstr_t(strTemp);			
					pNACondition->setNamedItem(pNAttributeTemp);
				}
				else
				{
					pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_ELEMENT");
					pNAttributeTemp->text = _bstr_t(stGroupElement.stOneElementInfo.strElement);
//					pNAttributeTemp->text = _bstr_t(CONDITION_INDEX[nIndex]);
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_NOT");
					pNAttributeTemp->text = _bstr_t(stGroupElement.stOneElementInfo.strNot);			
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_OPERATION");
					pNAttributeTemp->text = _bstr_t(stGroupElement.stOneElementInfo.strOperation);			
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_VIEW");
					strTemp = (stGroupElement.stOneElementInfo.bView == TRUE)?"TRUE":"FALSE";
					pNAttributeTemp->text = _bstr_t(strTemp);			
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_BRACKET");
					pNAttributeTemp->text = _bstr_t(stGroupElement.stOneElementInfo.strBracket);			
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMConditionIndex->createAttribute("LOGIC_BRACKETCNT");
					strTemp.Format("%d", stGroupElement.stOneElementInfo.lBracketCount);
					pNAttributeTemp->text = _bstr_t(strTemp);			
					pNACondition->setNamedItem(pNAttributeTemp);
				}
				
				strTemp = stConditionGroup.stOneConditionInfo.strText;
				pNFormat->text = _bstr_t(strTemp);
			}
		}

		//input User_define struct -> List Insert
		CString strFileFullRoot;
		if (strFolderName.IsEmpty() == TRUE)
			strFileFullRoot	= ((CMainFrmWnd*)m_pwndMain)->GetFolderPath("SYS") + "\\" + strFileName + ".xml";
		else
			strFileFullRoot = ((CMainFrmWnd*)m_pwndMain)->GetFolderPath("USER") + "\\" + strFolderName + "\\" + strFileName + ".xml";
		pDOMConditionIndex->save(_bstr_t(strFileFullRoot));

		SetSaveFile_Target(strFolderName, strFileName, strFileFullRoot);		//대상저장.
	}
	catch (_com_error e)
	{
		bResult = FALSE;
	}

	return bResult;
}

BOOL CConditionMakeWnd::GetGridPos_Element(CString strElement, int &nGroup, int &nRow)
{
	BOOL bResult = FALSE;
	
	CString strElement_Target;
	int nIndex_Group, nIndex_Row;
	for (nIndex_Group = 0; nIndex_Group < m_pGridCtrl->GetMaxGroupNo(); nIndex_Group++)
	{
		for (nIndex_Row = 0; nIndex_Row < m_pGridCtrl->GetGroupRowCnt(nIndex_Group); nIndex_Row++)
		{
			strElement_Target = m_pGridCtrl->GetElementInfo(nIndex_Group, nIndex_Row);
			if (strElement == strElement_Target)
			{
				nGroup = nIndex_Group;
				nRow = nIndex_Row;
				
				bResult = TRUE;
				break;
			}
		}
		
		if (bResult == TRUE)
			break;
	}
	
	return bResult;
}

CString CConditionMakeWnd::SetMakePacketData(User_Finder &stUser_Finder, CStringArray &saLanguageName)
{
	int nConCount = GetConditionCount();
	if(nConCount>MAX_IDXCNT)
	{
		CString szMsg;
		szMsg.Format("검색가능한 조건수(%d)를 초과하였습니다. \r\n확인해주시기 바랍니다.", MAX_IDXCNT);
		AfxMessageBox(szMsg);

		return _T("검색가능한 조건수(%d)를 초과");
	}

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	((CConditionResultWnd*)pwndMainFrm->GetWnd("RESULT"))->SetAllDelete_MoveHead();
	
	int nIndex, nIndexNo;
	int nUserDefineIndex = 0;
	POSITION pos, posFirstOrder = NULL;
	
	User_Define		stUser_Define[MAX_IDXCNT];
	CList<User_Define, User_Define> listUser_Define;


	
	saLanguageName.RemoveAll();
	memset((LPVOID)&stUser_Finder, 0x00, SZ_User_Finder);
	memset((LPVOID)stUser_Define, 0x00, SZ_User_Define * MAX_IDXCNT);
	
	nIndexNo = 1;
	
	// 대상변경 >>
	BOOL bJongFillterChk;
	bJongFillterChk = ((CConditionResultWnd*)pwndMainFrm->GetWnd("RESULT"))->m_bJongFillterChk;
	stUser_Finder.stTargetChg.sJongFillterChk = (bJongFillterChk == TRUE) ? '1' : '0';
	memset(stUser_Finder.stTargetChg.sJongFillterIndex, 0x30, sizeof(stUser_Finder.stTargetChg.sJongFillterIndex));
	if(bJongFillterChk)
		memcpy(stUser_Finder.stTargetChg.sJongFillterIndex + 1, ((CConditionResultWnd*)pwndMainFrm->GetWnd("RESULT"))->m_sTargetBit, (int)RESULT_MAX - (int)RESULT_MAX_OFFSET);
	// 대상변경 <<

	BOOL bIndustry = pwndMainFrm->m_bIndustry;
	stUser_Finder.nTargetType = (bIndustry == TRUE)?10:0;
	
	stUser_Finder.TargetIdxCnt = 0;
	stUser_Finder.TargetResultViewCnt = 0;
	stUser_Finder.ComplexIdxCnt = 0;
	stUser_Finder.ComplexResultViewCnt = 0;
	stUser_Finder.TargetIdxCnt = 0;
	stUser_Finder.ComplexIdxCnt = 0;
	
	
	int nCntCandle = 0;
	CString strElement;
	OneConditionInfo stOneConditionInfo;
	stOneConditionInfo.stCondition.IndexNo = 0;

// 2009.11.12 -hidden-
//	for (int nGroupIndex = 0; nGroupIndex <= m_DataAdmin.GetMaxGroup(); nGroupIndex++)
	for (int nGroupIndex = 0; nGroupIndex < m_DataAdmin.m_arrGroups.GetSize(); ++nGroupIndex)
	{ 
/*		if (nGroupIndex == 0)
			stUser_Finder.TargetIdxCnt = m_DataAdmin.GetGroupToConditionCnt(0);
		else
			stUser_Finder.ComplexIdxCnt += m_DataAdmin.GetGroupToConditionCnt(nGroupIndex);
*/
		GROUP_INFO* pGroup = m_DataAdmin.m_arrGroups.GetAt(nGroupIndex);

		if(0 == pGroup->nGroup)
			stUser_Finder.TargetIdxCnt = m_DataAdmin.GetGroupToConditionCnt(pGroup->nGroup);
		else
			stUser_Finder.ComplexIdxCnt += m_DataAdmin.GetGroupToConditionCnt(pGroup->nGroup);

		int nCondCount = m_DataAdmin.GetGroupToConditionCnt(pGroup->nGroup, FALSE);
		if(0 == nCondCount)
			continue;

		for (int nConditionIndex = 0; nConditionIndex < nCondCount; ++nConditionIndex)
		{
			strElement = m_pGridCtrl->GetElementInfo(pGroup->nGroup, nConditionIndex);
			
			if (strElement.IsEmpty() == FALSE)
			{
				m_DataAdmin.GetElementToOneConditionInfo(strElement, stOneConditionInfo);	
				
				if (stOneConditionInfo.bFalg == TRUE)
				{
					stOneConditionInfo.stCondition.IndexNo = nIndexNo;
					nIndexNo++;
					
//					stOneConditionInfo.stCondition.GroupNo = nGroupIndex;
//					stOneConditionInfo.stCondition.GroupIdxCnt = m_DataAdmin.GetGroupToConditionCnt(nGroupIndex);

					stOneConditionInfo.stCondition.GroupNo = pGroup->nGroup;
					stOneConditionInfo.stCondition.GroupIdxCnt = m_DataAdmin.GetGroupToConditionCnt(pGroup->nGroup);
					
					memcpy(&stOneConditionInfo.stCondition.Element, strElement, sizeof(stOneConditionInfo.stCondition.Element));
					
					stOneConditionInfo.stCondition.STerm += nCntCandle;
					stOneConditionInfo.stCondition.ETerm += nCntCandle;
					
					//# Language
					if (stOneConditionInfo.strTag == "LANGUAGE")
					{
						saLanguageName.Add(stOneConditionInfo.strName);   //# Name
						
						CString strTemp;
						strTemp.Format("%d", stOneConditionInfo.nChartValSet);
						saLanguageName.Add(strTemp);    //# Type
					}
					
					if (stOneConditionInfo.stCondition.IdxContiGubun == '0')
					{
						if ((stOneConditionInfo.bFieldState == TRUE) &&
							(stOneConditionInfo.bFieldVal == TRUE))
						{	
							//# Add Move Filed Info!
							((CConditionResultWnd*)pwndMainFrm->GetWnd("RESULT"))->SetAdd_MoveHead(stOneConditionInfo.stHeadMoveInfo);
							stOneConditionInfo.stCondition.ResultViewNum = stOneConditionInfo.stHeadMoveInfo.nColCnt;
						}
						else
							stOneConditionInfo.stCondition.ResultViewNum = 0;
						
						if (posFirstOrder == NULL)
							listUser_Define.AddTail(stOneConditionInfo.stCondition);
						else
							listUser_Define.InsertBefore(posFirstOrder, stOneConditionInfo.stCondition);
					}
					else
					{
						if ((stOneConditionInfo.bFieldState == TRUE) &&
							(stOneConditionInfo.bFieldVal == TRUE))
						{
							//# Add Move Filed Info!
							((CConditionResultWnd*)pwndMainFrm->GetWnd("RESULT"))->SetAdd_MoveHead(stOneConditionInfo.stHeadMoveInfo);
							stOneConditionInfo.stCondition.ResultViewNum = stOneConditionInfo.stHeadMoveInfo.nColCnt;
						}
						else
							stOneConditionInfo.stCondition.ResultViewNum = 0;
						
						if (posFirstOrder == NULL)
						{
							listUser_Define.AddTail(stOneConditionInfo.stCondition);
							posFirstOrder = listUser_Define.GetTailPosition();
						}
						else
							listUser_Define.AddTail(stOneConditionInfo.stCondition);
					}
				}
			}
		}
		
		//로직 에디터 컨트롤
		//20100716 사용자가 입력한 조건식(Element)만 검색하기 위한 작업	
		CStringArray arrElement, arrOpt; //Element, (or,and)
		CString strElementData;
		m_ctrlCommonLogicEdit.GetLogicElement(arrElement, arrOpt);

		//{ //alzioyes(20111112)Type 확인하고자 로그추가함.
		//	CString szDebug;
		//	szDebug.Format("[SMalzio] type=%ld\n", stOneConditionInfo.stCondition.Type);
		//	OutputDebugString(szDebug);
		//}	
		int nEleCount;
		BOOL bChk = FALSE;
		for(nEleCount = 0; nEleCount < arrElement.GetSize(); nEleCount++)
		{
			for (nIndex = 0; nIndex < listUser_Define.GetCount(); nIndex++)
			{
				User_Define stUser_LogicEdit;

				pos = listUser_Define.FindIndex(nIndex);				
				memcpy(&stUser_LogicEdit, &listUser_Define.GetAt(pos), SZ_User_Define);
				strElementData.Format("%c", stUser_LogicEdit.Element);

				if( strElementData.CompareNoCase(arrElement.GetAt(nEleCount)) == 0)
				{
					memcpy(&stUser_Finder.stUser_Define[nUserDefineIndex], &listUser_Define.GetAt(pos), SZ_User_Define);
					//# News Condition => C1 Val Change 
					
					//사용자가 입력한 수식으로 수정(and, or)
					memcpy(&stUser_Finder.stUser_Define[nUserDefineIndex].Operator, arrOpt.GetAt(nEleCount), 1);
					if (stUser_Finder.stUser_Define[nUserDefineIndex].Type == NEWS_TYPE)
					{
						//stUser_Define[nUserDefineIndex].Condition[0] = GetNewsCodeToIndex((int)stUser_Define[nUserDefineIndex].Condition[0]);

						stUser_Finder.stUser_Define[nUserDefineIndex].SignalCnt1 = 1;
						stUser_Finder.stUser_Define[nUserDefineIndex].SignalCnt2 = 1;
					}		
					nUserDefineIndex++;
					bChk = TRUE;
					break;
				}
			}				
		}
//20100716 20100716 사용자가 입력한 조건식(Element)만 검색하기 위한 작업
		if (/*nEleCount == 0 && */!bChk && listUser_Define.GetCount() > 0)
		{
			for (nIndex = 0; nIndex < listUser_Define.GetCount(); nIndex++)
			{
				pos = listUser_Define.FindIndex(nIndex);			

				memcpy(&stUser_Finder.stUser_Define[nUserDefineIndex], &listUser_Define.GetAt(pos), SZ_User_Define);
				//# News Condition => C1 Val Change 
				if (stUser_Finder.stUser_Define[nUserDefineIndex].Type == NEWS_TYPE)
				{
					//stUser_Define[nUserDefineIndex].Condition[0] = GetNewsCodeToIndex((int)stUser_Define[nUserDefineIndex].Condition[0]);
					
					stUser_Finder.stUser_Define[nUserDefineIndex].SignalCnt1 = 1;
					stUser_Finder.stUser_Define[nUserDefineIndex].SignalCnt2 = 1;
				}		
				nUserDefineIndex++;
			}
		}
		listUser_Define.RemoveAll();
		posFirstOrder = NULL;
	}
	
	if ((stUser_Finder.TargetIdxCnt + stUser_Finder.ComplexIdxCnt) == 0)
	{
		//m_btnSearch.EnableWindow(TRUE);
		return _T("검색할 지표가 없습니다.");
	}
	
	stUser_Finder.TargetResultViewCnt = m_DataAdmin.GetMoveFieldTotCnt(TRUE);
	stUser_Finder.ComplexResultViewCnt = m_DataAdmin.GetMoveFieldTotCnt(FALSE);
	
	BOOL bHaveCodeFiled = FALSE;
	CUIntArray iaFixHeadId;
	stUser_Finder.FieldEditerCnt = ((CConditionResultWnd*)pwndMainFrm->GetWnd("RESULT"))->GetCount_FixHead(bHaveCodeFiled, iaFixHeadId) + 
		stUser_Finder.TargetResultViewCnt + stUser_Finder.ComplexResultViewCnt;
	
	if (bHaveCodeFiled == TRUE)
		stUser_Finder.FieldEditerCnt--;
	
	if (m_btnMatchValCheck.GetCheck() == TRUE)
		//# Field Sync => Set Data, Out Function!
		stUser_Finder.ResultIdxCnt = m_listMatchInfo.GetCount();
	
	long lHeadID;
	int nIndexOffSet = 0;
	for (nIndex = 0; nIndex < iaFixHeadId.GetSize(); nIndex++)
	{
		lHeadID = (long)iaFixHeadId.GetAt(nIndex);
		
		if (lHeadID == (long)GRID_MOVEFIELD_POS_INDEX)
		{
			for (int nFieldIndex = 0; nFieldIndex < stUser_Finder.TargetResultViewCnt + stUser_Finder.ComplexResultViewCnt; nFieldIndex++)
			{
				stUser_Finder.FieldList[nIndexOffSet] = 0; 
				nIndexOffSet++;
			}
		}
		else
		{
			stUser_Finder.FieldList[nIndexOffSet] = lHeadID;
			nIndexOffSet++;
		}
	}
	iaFixHeadId.RemoveAll();
	
	CString strCommonExpression = "";
	m_ctrlCommonLogicEdit.GetLogicPacketOneString(strCommonExpression);
	if(strCommonExpression != "")
	{
		strCommonExpression = strCommonExpression + "!";
		memset(stUser_Finder.TargetExp, ' ', EXPLEN);
		memcpy(stUser_Finder.TargetExp, strCommonExpression, strCommonExpression.GetLength());
	}
	
	CString strOrderExpression = "";
	m_ctrlOrderLogicEdit.GetLogicPacketGroupString(strOrderExpression);
	if(strOrderExpression != "")
	{
		strOrderExpression = strOrderExpression + "!";
		memset(stUser_Finder.ComplexExp, ' ', EXPLEN);
		memcpy(stUser_Finder.ComplexExp, strOrderExpression, strOrderExpression.GetLength());
	}
	
	if (m_saMSTCode.GetSize() > 0)
	{
		strCommonExpression.Replace("!", "");
		strOrderExpression.Replace("!", "");
		CString strLogic;
		
		if (strCommonExpression.IsEmpty() == TRUE) 
			strLogic = strOrderExpression;
		else if (strOrderExpression.IsEmpty() == TRUE)
			strLogic = strCommonExpression;
		else
			strLogic = strCommonExpression + "&" + strOrderExpression;
		
		if (strLogic.IsEmpty() == FALSE)
		{
			memset(stUser_Finder.TargetBit, 0x30, (int)RESULT_MAX);
			
			char sTargetBit[RESULT_MAX - RESULT_MAX_OFFSET];
			stUser_Finder.TargetBitChk = (SetTargetBit(strLogic, sTargetBit) == TRUE) ?'1':'0';
			stUser_Finder.TargetBitChk = '0';
			if (stUser_Finder.TargetBitChk == '1')
				memcpy(stUser_Finder.TargetBit + 1, sTargetBit, (int)RESULT_MAX - (int)RESULT_MAX_OFFSET);   
		}
		else
		{
			stUser_Finder.TargetBitChk = '0';
			memset(&stUser_Finder.TargetBit, 0x30, (int)RESULT_MAX);
		}
	}
	else
	{
		stUser_Finder.TargetBitChk = '0';
		memset(&stUser_Finder.TargetBit, 0x30, (int)RESULT_MAX);
	}
	
	GetBackTestData(stUser_Finder.BackDay, stUser_Finder.BackTime);
	
	return _T("");
}

// extern void Dll_GetRegKwansimMasterGroupItem(CString strName, CStringArray &arrItem); -hidden-
BOOL CConditionMakeWnd::SetTargetBit(CString strLogic, char *psResultData)
{
	BOOL bResult = TRUE;
	

	int nHaveCnt_Port = 0;
	
	char *psLogic;
	psLogic = (char *)malloc(strLogic.GetLength() + 1);
	memset(psLogic, 0, strLogic.GetLength() + 1);
	memcpy(psLogic, strLogic.GetBuffer(0), strLogic.GetLength());
	
	CPtrArray *paData = new CPtrArray;
	
	CString strCode;
	ConditionGroupInfo  stConditionGroupInfo;
	

// 2009.11.13 -hidden-
	int nCondSize = 0;
	int nSize = m_DataAdmin.m_arrGroups.GetSize();
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;

	//[20110812:alzioyes][0000052]기존에는 일반검색 조건에서 관심일때의 체크루틴이 있었지만
	//수정버전에서는 "관심그룹"으로 설정한 상태의 종목을 읽어와서 처리하도록 해야함.
	// 
/* >>[0000052] alzioyes막음
	for (nIndex = 0; nIndex < nSize; ++nIndex)
	{
		GROUP_INFO* pGroup = m_DataAdmin.m_arrGroups.GetAt(nIndex);
		nCondSize = pGroup->arrConGrpInfo.GetSize();
		if(!nCondSize)
			continue;

		for(int j = 0; j < nCondSize; ++j)
		{
			stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(j);
					
			CodeList *pstPortInfo = (CodeList *)malloc(sizeof(CodeList));
			memset(pstPortInfo, 0x00, sizeof(CodeList));
			memcpy(&pstPortInfo->sElement, stConditionGroupInfo.strElement.GetBuffer(0), sizeof(pstPortInfo->sElement));
			stConditionGroupInfo.strElement.ReleaseBuffer();
		}
	}
>>[0000052] alzioyes막음
*/

	if(m_btnPortCheck.GetCheck())
	{
		nHaveCnt_Port = SetTargetBit_Portfolio(paData);
	}
	else
	{
		CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
		CMapStringToString* pFilterMap = &(((CConditionResultWnd*)pwndMainFrm->GetWnd("RESULT"))->m_saPortfolioFilterMap);
		pFilterMap->RemoveAll();
	}

	if (nHaveCnt_Port > 0)
	{
	}
	else
		bResult = FALSE;
	
	Lib_SetFree_PtrArray(paData);
	delete paData;
	free(psLogic);
	
	return bResult;
}

int CConditionMakeWnd::SetTargetBit_Portfolio(CPtrArray *_paData)
{
	//# Logic Calculate!   Add : 2006.10
	typedef struct tagCodeList
	{
		char sElement;
		int nCnt;
		char *psData;
	} CodeList;

	CString rString;
	m_ctrlPortEdit.GetWindowText(rString);

	CStringArray saPortGroup;
	BOOL bAnd;	//0(or), 1(and) 
	if(m_ctrlPortEdit.GetPortGroupList(saPortGroup, bAnd)==0)
		return 0;

	int nPortGroup = m_ctrlPortEdit.GetPortDataCount();
	if(nPortGroup==0) return 0;

	CStringArray target;
	CString szKey;

	CMapStringToString resultMap;
	szKey = saPortGroup.GetAt(0);
	GetRegKwansimMasterGroupItem(szKey, target);
	int nItemSize = target.GetSize();
	for(int k=0; k<nItemSize; k++)
	{
		resultMap.SetAt(target.GetAt(k), "1");
	}

	char *aOption[2] = {"or", "and"};

	for(int i=1; i<nPortGroup; i++)
	{
		target.RemoveAll();
		szKey = saPortGroup.GetAt(i);
		GetRegKwansimMasterGroupItem(szKey, target);

		GetDataWithFilter(resultMap, target, aOption[bAnd]);
	}

	POSITION pos = resultMap.GetStartPosition();
	CString strItmeCode, rValue;
	CodeList *pstPortInfo = (CodeList *)malloc(sizeof(CodeList));
	memset(pstPortInfo, 0x00, sizeof(CodeList));
	pstPortInfo->sElement = 'A';

	int nResultCnt = resultMap.GetCount();
	int nSize = 7 * nResultCnt;
	pstPortInfo->psData = (char *)malloc(nSize);
	memset(pstPortInfo->psData, 0x30, nSize); 

	//[2011.08.19]서버로 올려야하나 동작이 제대로 안되고 있어서 PC에서 결과를 Filter하도록 처리함.
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	CMapStringToString* pFilterMap = &(((CConditionResultWnd*)pwndMainFrm->GetWnd("RESULT"))->m_saPortfolioFilterMap);
	pFilterMap->RemoveAll();

	int nItem=0;
	for ( ; pos != NULL;)
	{
		resultMap.GetNextAssoc(pos, strItmeCode, rValue);
		memcpy(pstPortInfo->psData + 7 * nItem, (LPCSTR)strItmeCode, CODE_LENGTH);

		pFilterMap->SetAt(strItmeCode, "1");

		nItem++;
	}
	pstPortInfo->nCnt = nItem;
	_paData->Add(pstPortInfo);

	return nItem;
}

int CConditionMakeWnd::GetDataWithFilter(CMapStringToString &resultMap, CStringArray&target, LPCSTR sOption)
{
	int nOption=-1;
	if(strcmp(sOption, "or")==0) nOption = 0;
	else if(strcmp(sOption, "and")==0) nOption = 1;
	
	int nTargetCount = target.GetSize();
	int nIdxTarget=0;
	CString strTmpTarget;
	
	int nResult = 0;
	if(nOption==1)
	{
		BOOL bFound = FALSE;
		CString rKey,rValue;
		POSITION pos = resultMap.GetStartPosition();
		while(pos)
		{
			resultMap.GetNextAssoc(pos, rKey,rValue);
			
			bFound = FALSE;
			for(nIdxTarget=0; nIdxTarget<nTargetCount; nIdxTarget++)
			{
				strTmpTarget = target.GetAt(nIdxTarget);
				if(rKey.Compare(strTmpTarget)==0)
				{
					bFound = TRUE;
					break;
				}
			}
			if(!bFound) resultMap.RemoveKey(rKey);
		}
	}
	else if(nOption==0)
	{
		for(nIdxTarget=0; nIdxTarget<nTargetCount; nIdxTarget++)
			resultMap.SetAt(target.GetAt(nIdxTarget), "1");
	}
	else
		return 0;
	
	nResult = resultMap.GetCount();
	return nResult;
}


void CConditionMakeWnd::GetBackTestData(long &nDay, long &nTime)
{
	if (m_btnBackTestChk.GetCheck() == TRUE)
	{
		CString strTemp;
		m_comboBackTestDay.GetWindowText(strTemp);
		
		strTemp = strTemp.Mid(3, strTemp.GetLength() - 3);    //# Count!
		
		strTemp.Replace("-", "");
		nDay = atoi(strTemp);
		
		m_comboBackTestTime.GetWindowText(strTemp);
		strTemp.Replace(":", "");
		strTemp = strTemp + "00";
		nTime = atol(strTemp);
	}
	else
	{
		nDay = -1;
		nTime = -1;
	}
}

/*
CString CConditionMakeWnd::GetLanguageData(CString strLanguageName)
{
CString strResult = "";

  char szFileName[MAX_PATH] = { 0 };
  CString strPath = m_strAppPath + "\\XTRSTD.dll";
  HMODULE hModule = ::LoadLibrary(strPath);
  
	if (hModule)
	{
	typedef CString (*fpGetIndexFullSourceQway)(LPCTSTR pszIndexName, int nIndexType, CStringArray& arrInputStrings);
	fpGetIndexFullSourceQway fn;
	fn = (fpGetIndexFullSourceQway)::GetProcAddress(hModule, "?GetIndexFullSourceQway@@YA?AVCString@@PBDHAAVCStringArray@@@Z");
	if (fn)
	{
	CStringArray arrInputStrings;
	strResult = fn(strLanguageName, 1, arrInputStrings);
	}
	::FreeLibrary(hModule);
	}
	
	  CEzData21::EzMessageBox("fpGetIndexFullSourceQway 결과\n =>" + strResult, MB_OK, PROGRAM_TITLE);
	  
		return strResult;
		}
*/

void CConditionMakeWnd::SetTabChange(int nTabIndex)
{
	m_nSelectTab = nTabIndex;

	if(m_nSelectTab == 0 && m_nSeparateForm == 0)
	{
		if(m_ptabSet->GetCurSel() == 0)
		{
			m_ctrlOrderLogic.ShowWindow(SW_HIDE);
			m_ctrlOrderLogicEdit.ShowWindow(SW_HIDE);
			
			m_ctrlCommonLogic.ShowWindow(SW_SHOW);
			m_ctrlCommonLogicEdit.ShowWindow(SW_SHOW);
		}
		else if(m_ptabSet->GetCurSel() == 1)
		{
			m_ctrlCommonLogic.ShowWindow(SW_SHOW);
			m_ctrlCommonLogicEdit.ShowWindow(SW_SHOW);
			
			m_ctrlOrderLogic.ShowWindow(SW_SHOW);
			m_ctrlOrderLogicEdit.ShowWindow(SW_SHOW);
		}
		
		
		m_pShowLogicStatic = &m_ctrlCommonLogic;
		m_pShowLogicEdit = &m_ctrlCommonLogicEdit;

	}
	else if(m_nSelectTab == 1)
	{
		if(m_ptabSet->GetCurSel() == 0)
		{
			m_ctrlCommonLogic.ShowWindow(SW_HIDE);
			m_ctrlCommonLogicEdit.ShowWindow(SW_HIDE);
			
			m_ctrlOrderLogic.ShowWindow(SW_SHOW);
			m_ctrlOrderLogicEdit.ShowWindow(SW_SHOW);
		}
		else if(m_ptabSet->GetCurSel() == 1)
		{
			m_ctrlCommonLogic.ShowWindow(SW_SHOW);
			m_ctrlCommonLogicEdit.ShowWindow(SW_SHOW);
			
			m_ctrlOrderLogic.ShowWindow(SW_SHOW);
			m_ctrlOrderLogicEdit.ShowWindow(SW_SHOW);
		}
		
		m_pShowLogicStatic = &m_ctrlOrderLogic;
		m_pShowLogicEdit = &m_ctrlOrderLogicEdit;
	}

	OnBtnInit();
}

int CConditionMakeWnd::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	return pwndMainFrm->SetMessageBox(lpszText, lpszCaption, nType);
}

void CConditionMakeWnd::SetSendMainMsg(CString strMsg)
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	pwndMainFrm->SetSendMainMsg(strMsg);
}

BOOL CConditionMakeWnd::SetMSTCode(CStringArray *psaMSTCode)
{
	BOOL bResult = FALSE;
	
	if (psaMSTCode == NULL)
		return bResult;
	
	if (psaMSTCode->GetSize() > 0)
	{
		m_saMSTCode.Copy(*psaMSTCode);
		
		//# Lib Load!
		if (m_hConditionMng != NULL)
			return bResult;

		CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
		CString strRoot;
		strRoot.Format("%s", pwndMainFrm->Variant(homeCC));
		CString strPath = strRoot + "\\dev\\Lib_ConditionMng.dll";
		
		m_hConditionMng = ::LoadLibrary(strPath);
		if (m_hConditionMng)
		{
			Lib_Open = (_DLL_Open)::GetProcAddress(m_hConditionMng, "_DLL_Open");
			Lib_Close = (_DLL_Close)::GetProcAddress(m_hConditionMng, "_DLL_Close");
			
			Lib_SetFree_PtrArray = (_DLL_SetFree_PtrArray)::GetProcAddress(m_hConditionMng, "_DLL_SetFree_PtrArray");
			Lib_GetLogicCalculate = (_DLL_GetLogicCalculate)::GetProcAddress(m_hConditionMng, "_DLL_GetLogicCalculate");
			
			Lib_SetUserFolderPath = (_DLL_SetUserFolderPath)::GetProcAddress(m_hConditionMng, "_DLL_SetUserFolderPath");
			CString strUserFolderPath;
			strUserFolderPath.Format("%s%s%s", pwndMainFrm->Variant(homeCC), USRDIR, pwndMainFrm->Variant(nameCC));
			Lib_SetUserFolderPath(strUserFolderPath);
			
			Lib_Open(&m_saMSTCode);
			
			TRACE("\n# Logic Calculate Create Success!");
			
			bResult = TRUE;
		}
	}
	else
		SetSendMainMsg("Set Master Code Error! 고객 센터로 문의 하십시오.");
	
	return bResult;
}

// void CConditionMakeWnd::ChangeSkinColor(COLORREF clrSkin)
// {
// //	m_clrMainSkin = COLOR_MAIN_CANVAS;
// 	m_clrMainSkin = RGB(213, 213, 213);		// KB투자증권
// 
// //	m_clrMainSkin = clrSkin;
// 
// 	m_brushStatic.DeleteObject();
// 	m_brushStatic.CreateSolidBrush(m_clrMainSkin);
// }

void CConditionMakeWnd::ChangeSkinColor(COLORREF clrSkin, ST_TABCOLOR stTabColor)
{
	if(!clrSkin)
		return;
	
	CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pwndMain;
	
	m_clrMainSkin = clrSkin;

	if(m_brushStatic.DeleteObject())
		m_brushStatic.CreateSolidBrush(m_clrMainSkin);
	if(m_brushMapLoad.DeleteObject())
		m_brushMapLoad.CreateSolidBrush(stTabColor.clrBACK);
	if(m_brushTab.DeleteObject())
		m_brushTab.CreateSolidBrush(pMainWnd->GetAxColor(66));
	
	m_ctrlMapLoad.ChangeSkinColor(pMainWnd->GetAxColor(66));
	m_clrMapLoad = stTabColor.clrBACK;

	m_pGridCtrl->Invalidate();

 	m_ptabSet->SetTabColor(m_clrMainSkin, m_clrMainSkin, stTabColor.clrB, NULL, NULL, m_clrMainSkin);
 	m_ptabSet->SetTextColor(stTabColor.clrSF, stTabColor.clrSF);
	
	m_ptabSet->Invalidate();

	m_ctrlTargetBase.Invalidate();
	m_comboTargetBase.Invalidate();
	m_btnAlertChk.Invalidate();

	Invalidate();
}


void CConditionMakeWnd::EuBtnGridTip()
{
	OnBtnGridTip();
}
void CConditionMakeWnd::EuBtnLogicTip()
{
	OnBtnLogicTip();
}
void CConditionMakeWnd::EuBtnInit(BOOL bOneClick/* = FALSE */)
{
	m_bOneClickInitForSearch = bOneClick;
	OnBtnInit();
	m_bOneClickInitForSearch = FALSE;
}
void CConditionMakeWnd::EuBtnFileSave()
{
	OnBtnFileSave();
}

//--> RMenu시작
long CConditionMakeWnd::OnChangeChildFrameWindowHandle( WPARAM wParam, LPARAM lParam)
{
	m_hChildFrame = (HWND)lParam;
	return 0L;
}

HWND CConditionMakeWnd::GetFrameWnd(CWnd* pWnd)
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

extern void KSLink_SetFormLinkTagData( CString sTag, CString sValue );
//extern void KSLink_SetLinkTagData( LPCTSTR sTag, LPCTSTR sValue);
void CConditionMakeWnd::SetCommMenu(POINT *point)
{
	HWND hChildFrame = m_hChildFrame;
	if(!hChildFrame) return;

	long lMenu = 0; //::SendMessage(hChildFrame, UM_GET_SCREENLINKMENU, 0, 3 ); -hidden-
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
//<--

BOOL CConditionMakeWnd::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_RBUTTONUP)
	{
		CPoint point = pMsg->pt;
		FnRButtonUp(point);
	}
	return CWnd::PreTranslateMessage(pMsg);
}

void CConditionMakeWnd::FnRButtonUp(CPoint point)
{
//	SetCommMenu(&point);

	CMainFrmWnd* wndMainFrm = (CMainFrmWnd*)m_pwndMain;
	::PostMessage(wndMainFrm->GetParent()->m_hWnd, WM_CONTEXTMENU, 0, 0);
}

BOOL CConditionMakeWnd::PrepareRunRealSearch()
{
	if (!m_DataAdmin.GetConditionCnt())
	{
		if(m_nSeparateForm == 0)
			SetMessageBox("조건이 존재하지 않습니다.", PROGRAM_TITLE, MB_OK);
		else
			SetMessageBox("조건이 존재하지 않습니다.", PROGRAM_TITLE_BRIGHT, MB_OK);

		return FALSE;
	}

	m_btnAlertChk.SetCheck(TRUE);

	if(!SetStandbyAlert())
		return FALSE;

	return TRUE;
}

void CConditionMakeWnd::GetRegKwansimMasterGroupItem(CString strName, CStringArray& arrItems)
{
	CStringArray arrName;
	CMainFrmWnd* pWndMainFrm = (CMainFrmWnd*)m_pwndMain;
#if(0)
	pWndMainFrm->GetData_PortItem(arrItems, arrName, _ttoi(strName));
#else
	PortGroupInfo stPort;
	if (pWndMainFrm->m_mapPortItem.Lookup(strName, stPort))
	{
		int nMax = stPort.nItemCnt;
		for (int i = 0; i < nMax; i++)
			arrItems.Add(stPort.strCode[i]);
	}
#endif
}

BOOL CConditionMakeWnd::GetBackDateAndTime(CString& strDate, CString& strTime)
{
	CString strTemp;

	if(!m_btnBackTestChk.GetCheck())
	{
		strDate = "";
		strTime = "";

		return FALSE;
	}
	else
	{
		m_comboBackTestDay.GetWindowText(strTemp);
		strTemp = strTemp.Mid(2);
		strTemp.Remove('-');
		strTemp.TrimRight(); strTemp.TrimLeft();
		strDate = strTemp;

		m_comboBackTestTime.GetWindowText(strTemp);
		strTemp.Remove(':');
		strTime = strTemp + "00";

		return TRUE;
	}
}

void CConditionMakeWnd::ChangeGridColor(ST_GRIDCOLOR stGridColor)
{
	//우측 상단 그리드
	m_pGridCtrl->ChangeSkinColor(stGridColor);
	m_pGridCtrl->Invalidate();
}

void CConditionMakeWnd::DoDataExchange(CDataExchange* pDX)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CWnd::DoDataExchange(pDX);
}

void CConditionMakeWnd::SetChagneTargetEditIndicate(CString strIndicate)
{
	m_pGridCtrl->SetChagneTargetEditIndicate(strIndicate);
}

int	 CConditionMakeWnd::GetConditionCount()
{
	return m_DataAdmin.GetConditionCnt();
}
void CConditionMakeWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	Invalidate();
	CWnd::OnSettingChange(uFlags, lpszSection);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//20100827 이문수 >>
BOOL CConditionMakeWnd::OnEraseBkgnd(CDC* pDC)
{
	CRect reClient;
	GetClientRect(reClient);
	pDC->FillSolidRect(reClient, m_clrMainSkin);

	if(!m_nSeparateForm)
	{
		CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pwndMain;
		pDC->FillSolidRect(m_rtTab, pMainWnd->GetAxColor(66));
	}
 	return FALSE;	
}
//20100827 이문수 <<

LRESULT CConditionMakeWnd::OnMGridSetBtnImg(WPARAM wParam, LPARAM lParam)
{
	CfxImgButton* pBtn = (CfxImgButton*)wParam;
	CString strImgName = (LPCTSTR)lParam;
	
	if(strImgName.GetLength() > 2)
		SetBtnImg(pBtn, strImgName);
	else
		SetBtnImg(pBtn, _ttoi(strImgName));

	return 0;
}

void CConditionMakeWnd::OnBtnDefaultValue()
{
	m_ctrlMapLoad.SetBase();
}

void CConditionMakeWnd::OnBtnNextCondition()
{
	m_nSelectCondition++;
	m_pGridCtrl->SetSelectRow(0, m_nSelectCondition, TRUE);
	m_btnPreCondition.EnableWindow(TRUE);

	if(m_nSelectCondition == m_nTotCntCondition - 1)
		m_btnNextCondition.EnableWindow(FALSE);
}

void CConditionMakeWnd::OnBtnPreCondition()
{
	m_nSelectCondition--;
	m_pGridCtrl->SetSelectRow(0, m_nSelectCondition, TRUE);
	m_btnNextCondition.EnableWindow(TRUE);

	if(m_nSelectCondition == 0)
		m_btnPreCondition.EnableWindow(FALSE);
}

void CConditionMakeWnd::OnBtnSearchRng()
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	((CConditionResultWnd*)pwndMainFrm->GetWnd("RESULT"))->OnSearchRng();
}

void CConditionMakeWnd::SetBtnExpandImage()
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	
	BOOL bState = pwndMainFrm->m_bLeftExp;

	switch (bState)
	{
	case 0 :
		SetBtnImg(&m_btnExpand, "RIGHT_EX");
		break;
	case 1 :
		SetBtnImg(&m_btnExpand, "LEFT_EX");
		break;
	default :
		break;
	}
}

void CConditionMakeWnd::SetTabTextConditionItemexcavation()
{
// 	if(m_nSelectCondition == 0)
// 		return;
	
	CStringArray saAllConditionName;
	m_DataAdmin.GetAllConditionName(&saAllConditionName);
	
	m_pTabConditionForItemExcavation->DeleteAllItems();

	for(int nIndex = 0; nIndex < saAllConditionName.GetSize(); nIndex++)
	{
		TC_ITEM item;
		item.mask = TCIF_TEXT | TCIF_IMAGE;
		item.pszText = (LPSTR)(LPCTSTR)saAllConditionName.GetAt(nIndex);
		m_pTabConditionForItemExcavation->InsertItem(nIndex, &item);
	}

	m_pTabConditionForItemExcavation->MoveWindow(m_rtTab);

	CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pwndMain;

	m_pTabConditionForItemExcavation->SetTabColor(pMainWnd->GetColor(181), pMainWnd->GetColor(66), 
 			pMainWnd->GetColor(22), pMainWnd->GetColor(66), pMainWnd->GetColor(22), pMainWnd->GetColor(181));
	m_pTabConditionForItemExcavation->SetTextColor(pMainWnd->GetColor(26), pMainWnd->GetColor(26));

	m_bLoadConditionForItemExcavation = TRUE;
}

void CConditionMakeWnd::OnChangeConditionTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nIndex = m_pTabConditionForItemExcavation->GetCurSel();
	SetFocus();
	m_ctrlMapLoad.EuBtnUpdate();
	m_pGridCtrl->SetSelectRow(0, nIndex, TRUE);
}

void CConditionMakeWnd::SetBtnImg(CfxImgButton *pBtn, CString strFileName)
{
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	pwndMainFrm->SetBtnImg(pBtn, strFileName);
}

void CConditionMakeWnd::SetBtnImg(CfxImgButton *pBtn, int nTextLen, BOOL bBold)
{
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	pwndMainFrm->SetBtnImg(pBtn, nTextLen, bBold);
}

LRESULT CConditionMakeWnd::OnLanguageReload(WPARAM wParam, LPARAM lParam)
{
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	pwndMainFrm->OnLanguageReload();

	return 0;
}