// MainFrmWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrmWnd.h"
#include "Winsock2.h"
#include <math.h>

#include "ScreenWnd.h"

#include "FilterSetDlg.h"
#include "FilterSetWnd.h"

#include "../Dr_Include/CompanySS.h"
#include "../Dr_Include/ConditionSearchOne_Convert.h"
#include "../Dr_Include/RealdataForITGen.h"

#include "./AddControl/DlgNoItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE g_hInstance;

/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd

extern BOOL Dll_AddRegKwansimMasterGroupItemForNewsSearch(CString strName, CStringArray &arrItem);
extern int Dll_ShowPortfolioDlg(CString strCode, CString strCurrName, CString &strLastName, int nShowOpt, int nType);

//const  COLORREF	clrText		 = COLOR_WHITE;
const  COLORREF	clrText		 = COLOR_BLACK;

const		LPCTSTR		ROLLSEARCH_TEXT[] = { "30초", "1분" };
const		UINT			ROLLSEARCH_TIME[] = { 30, 60 };

typedef struct {
	EU_p0623_InRec1 inRec1;
	EU_p0623_InRec2 inRec2[MAX_IDXCNT];
} EU_p0621_InAll;

#define WMU_SIGNAL_REGRCVWND	(WM_USER + 0x732)		// 신호 수신할 윈도우 등록
#define WMU_NOTIFY_SIGNAL		(WM_USER + 0x733)		// 신호발생 통지메시지(WPARAM:신호종류 , LPARAM:종목코드)

CMainFrmWnd::CMainFrmWnd()
{	
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);

	m_clrTab	  = COLOR_TAB_CANVAS;
	
	m_brushMax.CreateSolidBrush(COLOR_STATE_MAX);
	m_brushUp.CreateSolidBrush(COLOR_STATE_UP);
	m_brushSte.CreateSolidBrush(COLOR_STATE);
	m_brushDown.CreateSolidBrush(COLOR_STATE_DOWN);
	m_brushMin.CreateSolidBrush(COLOR_STATE_MIN);
	m_brushCnt.CreateSolidBrush(COLOR_STATE_CNT);
	m_brushTab.CreateSolidBrush(COLOR_TAB_CANVAS);
	
	m_bInit = FALSE;
	
	m_nIndustry = 0;
	m_bIsFnUser = FALSE;
	m_strUserID = "";
	
	m_ptabGroup = NULL;
	m_hConditionMng = NULL;
	m_hSignalMng = NULL;
	
	m_bRollSearch = FALSE;
	
	m_bHelpShowFlag = FALSE;
	m_bLeftShowFlag = TRUE;
	m_bRealRegFlag = FALSE;
	
	m_nTrIndex_Result = 0;
	m_nTrIndex_List = 0;
	
	m_strAlertKey = "";
	
	m_stFilterInfo.plistDealTimeVol = new CList <SubFilterInfo, SubFilterInfo&>;
	m_stFilterInfo.plistDealTimePrice = new CList <SubFilterInfo, SubFilterInfo&>;

	m_bRegReal = FALSE;
	
	m_b0622Ing = FALSE;
	m_dwAdviseKey = NULL;

	m_hChildFrame = NULL;
	m_bOpenFromConditionSearch = FALSE;		// 2009.11.02 -hidden-	조건검색화면에서 오픈 할때만 감시.
	
	m_pdlgFieldEdit = NULL;

	m_bFromCall = FALSE;

	m_pModuleMainWnd = NULL;

	m_nPreReceiveLen = -1;
}

CMainFrmWnd::~CMainFrmWnd()
{
	m_fontDefault.DeleteObject();
	m_fontBold.DeleteObject();
	
	m_brushStatic.DeleteObject();
	m_brushBK.DeleteObject();
	
	m_brushMax.DeleteObject();
	m_brushUp.DeleteObject();
	m_brushSte.DeleteObject();
	m_brushDown.DeleteObject();
	m_brushMin.DeleteObject();
	m_brushCnt.DeleteObject();
	m_brushTab.DeleteObject();

	m_stFilterInfo.plistDealTimeVol->RemoveAll();
	delete m_stFilterInfo.plistDealTimeVol;
	m_stFilterInfo.plistDealTimeVol = NULL;
	
	m_stFilterInfo.plistDealTimePrice->RemoveAll();
	delete m_stFilterInfo.plistDealTimePrice;
	m_stFilterInfo.plistDealTimePrice = NULL;

	if (m_pdlgFieldEdit != NULL)
	{
		delete m_pdlgFieldEdit;
		m_pdlgFieldEdit = NULL;
	}
}

BEGIN_MESSAGE_MAP(CMainFrmWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrmWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_COPYDATA()
	//}}AFX_MSG_MAP

	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_GROUP, OnChangeGroupTab)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREEVIEW, OnChangeTreeItem)

	ON_BN_CLICKED(IDC_BTN_CODECHK, OnBtnCodeChk)
	ON_BN_CLICKED(IDC_BTN_LISTSEARCH, OnBtnListSearch)
	ON_BN_CLICKED(IDC_BTN_HELPSHOW, OnBtnShowHelp)
	ON_BN_CLICKED(IDC_BTN_EXP, OnBtnExp)
	ON_BN_CLICKED(IDC_BTN_SEND, OnBtnSend)
	ON_BN_CLICKED(IDC_BTN_FILTERCHK, OnBtnFilterChk)
	ON_BN_CLICKED(IDC_BTN_FILTERSET, OnBtnFilterSet)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
	ON_BN_CLICKED(IDC_BTN_ROLLSEARCH, OnBtnRollSearch)
	ON_BN_CLICKED(IDC_BTN_ROLLSEARCH + 1, OnBtnRollSearch)
	ON_BN_CLICKED(IDC_BTN_REGREAL, OnBtnRegReal)
	ON_BN_CLICKED(IDC_CHK_DELOUTITEM, OnBtnDelOutItem)
	ON_BN_CLICKED(IDC_BTN_ITEMEDIT, OnBtnFieldEdit)

	ON_MESSAGE(WM_NOTIFY_GRID, OnNotify_Grid)
	ON_MESSAGE(WMU_NOTIFY_SIGNAL, OnNotifySignal)

	ON_MESSAGE(WMU_GET_MODULE_MANINWND, OnGetModuleMainWnd)

	ON_COMMAND_RANGE(ID_MENU_RESULTCTRL_START, ID_MENU_RESULTCTRL_END, OnMenuResultCtrl) // 20100204 박찬모 >>

	ON_CBN_SELCHANGE(IDC_COMBO_USERLIST, OnCbnSelChangeUserList)

	ON_MESSAGE(WM_GET_BROD, OnGetBroad)

	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd message handlers

int CMainFrmWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	m_clrMainSkin = GetColor(66);
	m_brushStatic.CreateSolidBrush(m_clrMainSkin);
	m_brushBK.CreateSolidBrush(m_clrMainSkin);

	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	m_szScreenKey = pScreenWnd->GetFormCode();
	
	//m_ctrlCodeInput.InitCodeInput((CDialog*)this, CRect(0, 0, 100, 0), IDC_CODEINPUT, CODEINPUT_PROP_STOCK);
	m_ctrlCodeInput.Create(_T("Code Input"), WS_CHILD | WS_VISIBLE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlCodeInput.SetFont(&m_fontDefault);
	
	m_ptabGroup = new CDrTabCtrl();
	m_ptabGroup->Create(0, this, IDC_TAB_GROUP);
	m_ptabGroup->SetBKColor(COLOR_MAIN_CANVAS);
	m_ptabGroup->SetCurSel(0);

	m_btnCodeChk.Create(_T(""), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | SS_LEFT, 
		CRect(0, 0, 0, 0), this, IDC_BTN_CODECHK);
	m_btnCodeChk.SetFont(&m_fontDefault);
	
	m_ctrlCodeName.Create(_T(""), WS_CHILD | WS_VISIBLE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlCodeName.SetFont(&m_fontDefault);
	
	m_ctrlTitle.Create(_T(""), WS_CHILD | WS_VISIBLE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlTitle.SetFont(&m_fontDefault);
	
	m_gridList.Create(NULL, _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER,
		CRect(0, 0, 0, 0), this, NULL);
	m_gridList.SetFont(&m_fontDefault);
// 	m_gridList.OnSetup("LIST", IDB_SIGNAL_OK);
	m_gridList.SetInit("LIST", IDB_SIGNAL_OK);
//	m_gridList.ShowWindow(SW_HIDE);	//@유진에서는 사용안하므로 Hide시켜놓음.

	m_ctrlUserList.Create(WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST, CRect(0, 0, 0, 0), this, IDC_COMBO_USERLIST);
	m_ctrlUserList.SetFont(&m_fontDefault);
	
//--> Tree추가
	DWORD dwStyle;
	dwStyle = WS_CHILD | WS_VISIBLE| WS_CLIPCHILDREN | TVS_HASBUTTONS | 
						TVS_LINESATROOT | TVS_HASLINES | TVS_SHOWSELALWAYS ;
	dwStyle += TVS_EDITLABELS;
	m_treeList.Create(dwStyle, CRect(0, 0, 0, 0), this, IDC_TREEVIEW);
	m_treeList.ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	
	CBitmap bMap_on, bMap_off;
	CBitmap bMap_SigOk, bMap_SigNone;
	
	bMap_on.LoadBitmap(IDB_SMENU_ON);
	bMap_off.LoadBitmap(IDB_SMENU_OFF);	
	bMap_SigOk.LoadBitmap(IDB_SIGNAL_OK);
	bMap_SigNone.LoadBitmap(IDB_SIGNAL_NONE);
	
	m_treeImageList.Create(IDB_SMENU_OFF, 18, 2, RGB(255, 0, 255));
	m_treeImageList.Add(&bMap_on, RGB(255,0,255));
	m_treeImageList.Add(&bMap_off, RGB(255,0,255));
	m_treeImageList.Add(&bMap_SigOk, RGB(255,0,255));
	m_treeImageList.Add(&bMap_SigNone, RGB(255,0,255));

/*	
	m_treeImageList.Create(IDB_SIGNAL_OK, 0, 2, RGB(255, 0, 255));
	m_treeImageList.Add(AfxGetApp()->LoadIcon(IDB_SIGNAL_OK));
	m_treeImageList.Add(AfxGetApp()->LoadIcon(IDB_SIGNAL_NONE));
*/

	m_treeList.SetImageList(&m_treeImageList, TVSIL_NORMAL);

	m_treeList.ShowWindow(SW_HIDE);
//<--Tree추가

	m_btnHelpShow.Create(_T("선택조건설명 ▲"), CRect(0, 0, 0, 0), this, IDC_BTN_HELPSHOW, FALSE, TRUE);
	SetBtnImg(&m_btnHelpShow, 8);

	m_editHelp.Create(WS_CHILD | ES_MULTILINE | ES_READONLY | WS_VSCROLL, 
		CRect(0, 0, 0, 0), this, IDC_EDIT_ITEMHELP);
	m_editHelp.SetFont(&m_fontDefault);
	m_editHelp.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	
	// 2009.10.27 -hidden- ---------------------------------------------------------
	m_ctrlConditionText.Create(WS_CHILD | ES_MULTILINE | ES_READONLY | WS_VSCROLL, 
		CRect(0, 0, 0, 0), this, IDC_EDIT_CONDITIONTEXT);
	m_ctrlConditionText.SetFont(&m_fontDefault);
	m_ctrlConditionText.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	// -----------------------------------------------------------------------------

	m_btnExp.Create(_T(""), CRect(0, 0, 0, 0), this, IDC_BTN_EXP, FALSE, TRUE);
	SetBtnImg(&m_btnExp, "LEFT_EX");
		
	m_btnListSearch.Create(_T("검색"), CRect(0, 0, 0, 0), this, IDC_BTN_LISTSEARCH);
	SetBtnImg(&m_btnListSearch, 2, FALSE);
	
	m_btnSend.Create(_T("종목전송"), CRect(0, 0, 0, 0), this, IDC_BTN_SEND);
	SetBtnImg(&m_btnSend, 4, FALSE);
	
	m_btnItemEdit.Create(_T("항목편집"), CRect(0, 0, 0, 0), this, IDC_BTN_ITEMEDIT);
	SetBtnImg(&m_btnItemEdit, 4, FALSE);
		
	m_btnFilterChk.Create(_T(""), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | SS_LEFT, 
		CRect(0, 0, 0, 0), this, IDC_BTN_FILTERCHK);
	m_btnFilterChk.SetFont(&m_fontDefault);
	
	m_btnFilterSet.Create(_T("필터"), CRect(0, 0, 0, 0), this, IDC_BTN_FILTERSET);
	SetBtnImg(&m_btnFilterSet, 2, FALSE);

	m_btnKospiChk.Create(_T("거래소"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | SS_LEFT, 
		CRect(0, 0, 0, 0), this, IDC_BTN_KOSPICHK);
	m_btnKospiChk.SetFont(&m_fontDefault);
	
	m_btnKosdakChk.Create(_T("코스닥"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | SS_LEFT, 
		CRect(0, 0, 0, 0), this, IDC_BTN_KOSDAKCHK);
	m_btnKosdakChk.SetFont(&m_fontDefault);
	
	m_btnTemaChk.Create(_T("테마"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | SS_LEFT, 
		CRect(0, 0, 0, 0), this, IDC_BTN_TEMACHK);
	m_btnTemaChk.SetFont(&m_fontDefault);
	
	m_btnSearch.Create(_T("검  색"), CRect(0, 0, 0, 0), this, IDC_BTN_SEARCH);
	SetBtnImg(&m_btnSearch, 4, TRUE);

	m_ctrlAutoStr.Create(_T("자동검색"),  WS_CHILD , CRect(0, 0, 0, 0), this);
	//m_ctrlAutoStr.SetFont(&m_fontBold);
	m_ctrlAutoStr.SetFontBold(TRUE);
	m_ctrlAutoStr.SetTextColor(RGB(255,0,0));
	m_ctrlAutoStr.SetBkColor(RGB(247, 248, 252));

	m_btnRegReal.Create(_T("감시"), CRect(0, 0, 0, 0), this, IDC_BTN_REGREAL);
	SetBtnImg(&m_btnRegReal, 2, TRUE);
	
	m_comboSearchTime.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(0, 0, 0, 0), this, IDC_COMBO_SEARCH_TIME);
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

	m_ctrlJongCnt1.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 
		CRect(0, 0, 0, 0), this);
	m_ctrlJongCnt1.SetFont(&m_fontDefault);
	
	m_ctrlMax1.Create(_T(""), 
		WS_CHILD | WS_VISIBLE | WS_BORDER, 
		CRect(0, 0, 0, 0), this);
	m_ctrlMax1.SetFont(&m_fontDefault);
	
	m_ctrlUp1.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 
		CRect(0, 0, 0, 0), this);
	m_ctrlUp1.SetFont(&m_fontDefault);
	
	m_ctrlSteady1.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 
		CRect(0, 0, 0, 0), this);
	m_ctrlSteady1.SetFont(&m_fontDefault);
	
	m_ctrlDown1.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 
		CRect(0, 0, 0, 0), this);
	m_ctrlDown1.SetFont(&m_fontDefault);
	
	m_ctrlMin1.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 
		CRect(0, 0, 0, 0), this);
	m_ctrlMin1.SetFont(&m_fontDefault);
	
	m_ctrlUpRate1.Create(_T(""), 
		WS_CHILD | WS_VISIBLE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlUpRate1.SetFont(&m_fontDefault);
	
	m_ctrlAvg1.Create(_T(""), WS_CHILD | WS_VISIBLE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlAvg1.SetFont(&m_fontDefault);

	m_btnDelOutItem.Create(_T("탈퇴종목삭제"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | SS_LEFT,
		CRect(0, 0, 0, 0), this, IDC_CHK_DELOUTITEM);
	m_btnDelOutItem.SetFont(&m_fontDefault);
	m_btnDelOutItem.SetCheck(FALSE);
	
	m_ctrlJongCnt.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_CENTER, 
		CRect(0, 0, 0, 0), this);
	m_ctrlJongCnt.SetFont(&m_fontDefault);
	
	m_ctrlMax.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_CENTER, 
		CRect(0, 0, 0, 0), this);
	m_ctrlMax.SetFont(&m_fontDefault);
	
	m_ctrlUp.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_CENTER, 
		CRect(0, 0, 0, 0), this);
	m_ctrlUp.SetFont(&m_fontDefault);
	
	m_ctrlSteady.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_CENTER, 
		CRect(0, 0, 0, 0), this);
	m_ctrlSteady.SetFont(&m_fontDefault);
	
	m_ctrlDown.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_CENTER, 
		CRect(0, 0, 0, 0), this);
	m_ctrlDown.SetFont(&m_fontDefault);
	
	m_ctrlMin.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_CENTER, 
		CRect(0, 0, 0, 0), this);
	m_ctrlMin.SetFont(&m_fontDefault);
	
	m_ctrlUpRate.Create(_T(""), WS_CHILD | WS_VISIBLE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlUpRate.SetFont(&m_fontDefault);
	
	m_ctrlAvg.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_CENTER, 
		CRect(0, 0, 0, 0), this);
	m_ctrlAvg.SetFont(&m_fontDefault);
	
	m_gridResult.Create(NULL, _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER,
		CRect(0, 0, 0, 0), this, NULL);
	m_gridResult.SetFont(&m_fontDefault);
//	m_gridResult.OnSetup("RESULT");
	
	m_ctrlMsg.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlMsg.SetFont(&m_fontDefault);
		
	m_hChildFrame = GetFrameWnd(this);

	// 20100203 박찬모 >>
	m_bBeforeUserTap = FALSE;
	m_bSaveShowFlag = FALSE;
	// 20100203 박찬모 <<

	AfxSetResourceHandle(hInstSave);

	//tour2k
	Request_PortGroup();
	
	return 0;
}

void CMainFrmWnd::SetInit_Industry(int nIndustry)
{
	if (1 == nIndustry)
	{
		m_btnCodeChk.ShowWindow(SW_HIDE);
		m_ctrlCodeInput.ShowWindow(SW_HIDE);
		m_ctrlCodeName.ShowWindow(SW_HIDE);
		m_btnListSearch.ShowWindow(SW_HIDE);
		
		m_btnFilterChk.ShowWindow(SW_HIDE);
		m_btnFilterSet.ShowWindow(SW_HIDE);
		
		m_btnRegReal.ShowWindow(SW_HIDE);
		m_btnSend.ShowWindow(SW_HIDE);
		/*m_btnItemEdit.ShowWindow(SW_HIDE); // 20100210 박찬모*/
		
		m_ctrlJongCnt.ShowWindow(SW_HIDE);
		m_ctrlMax.ShowWindow(SW_HIDE);
		m_ctrlUp.ShowWindow(SW_HIDE);
		m_ctrlSteady.ShowWindow(SW_HIDE);
		m_ctrlDown.ShowWindow(SW_HIDE);
		m_ctrlMin.ShowWindow(SW_HIDE);
		m_ctrlUpRate.ShowWindow(SW_HIDE);
		m_ctrlAvg.ShowWindow(SW_HIDE);
//		m_btnDelOutItem.ShowWindow(SW_HIDE);
		
		m_ctrlJongCnt1.ShowWindow(SW_HIDE);
		m_ctrlMax1.ShowWindow(SW_HIDE);
		m_ctrlUp1.ShowWindow(SW_HIDE);
		m_ctrlSteady1.ShowWindow(SW_HIDE);
		m_ctrlDown1.ShowWindow(SW_HIDE);
		m_ctrlMin1.ShowWindow(SW_HIDE);
		m_ctrlUpRate1.ShowWindow(SW_HIDE);
		m_ctrlAvg1.ShowWindow(SW_HIDE);

		m_btnItemEdit.ShowWindow(SW_HIDE);
		
		m_gridResult.HeaderSetup("INDUSTRY");
		
		m_btnTemaChk.ShowWindow(SW_HIDE);
		m_btnTemaChk.SetCheck(TRUE);
	}

	else if(m_nIndustry == 2)
	{
		CRect reList;
		
		m_btnExp.ShowWindow(SW_HIDE);
		OnBtnExp();

		m_btnExp.GetClientRect(&reList);
		int nleft = reList.left;
		int nTop = reList.top;

		m_ctrlTitle.ShowWindow(SW_HIDE);
		
		m_ctrlTitle.GetClientRect(&reList);
		reList.left = nleft;
		reList.top = nTop;
		
		m_ctrlUserList.MoveWindow(&reList);
		m_ctrlUserList.ShowWindow(SW_SHOW);

		m_btnItemEdit.ShowWindow(SW_SHOW);
	}

	else
	{
		m_btnTemaChk.ShowWindow(SW_HIDE);
		m_btnItemEdit.ShowWindow(SW_SHOW);
	}
	
	m_gridResult.m_nIndustry = nIndustry;
	
	m_btnKospiChk.SetCheck(TRUE);
	m_btnKosdakChk.SetCheck(TRUE);

	Invalidate();

}

void CMainFrmWnd::SetInit(CWnd* pWnd, int nIndustry)
{
	m_pModuleMainWnd = pWnd;
	m_nIndustry = nIndustry;
	
	OnBtnCodeChk();	
	int nIndex, nIndex_Group, nIndex_Item, nFindPos;
	CString strGroupName, strCindiData, strInfo[5];
	CList <ConditionInfo, ConditionInfo&> *plistConditionInfo;
	
	int nIndex_GroupSelect = 0;
	SetInit_Industry(m_nIndustry);

	if(m_nIndustry == 0)
		m_gridResult.SetInit("RESULT");
	else if(m_nIndustry == 1)
		m_gridResult.SetInit("INDUSTRY");
	else if(m_nIndustry == 2)
		m_gridResult.SetInit("REAL");
	
	if (SetLoad_ConditionMngLib() == TRUE)
	{	
		GetFilterData(m_stFilterInfo);
		
		CStringArray saCondiData;
		m_saGroupName.RemoveAll();

		BOOL bIndustry = (m_nIndustry == 1) ? TRUE : FALSE;
		int nType = (m_nIndustry == 2) ? 5 : 2;	
		
		if (Lib_GetConditionData(nType, bIndustry, -1, m_saGroupName, saCondiData) == TRUE)
		{
			TCITEM item;
			item.mask = TCIF_TEXT;

			if( 0 == m_nIndustry)
			{
				for(int i = 0; i < m_saGroupName.GetSize(); ++i)
				{
					if(m_saGroupName.GetAt(i) == (CString)TAB_TITLE_REALSEARCH)
					{
						m_saGroupName.RemoveAt(i);
						break;
					}
				}
			}
			else if(2 == m_nIndustry)
			{
				m_saGroupName.InsertAt(0, (CString)TAB_TITLE_USERCONDI);

			}

			for (nIndex_Group = 0; nIndex_Group < m_saGroupName.GetSize(); nIndex_Group++)
			{
				item.pszText = (LPTSTR)(LPCTSTR)m_saGroupName.GetAt(nIndex_Group);
				m_ptabGroup->InsertItem(nIndex_Group, &item);
				
				if(m_nIndustry == 2)
				{
					break;
				}
			}
		}
		else
		{
			AfxMessageBox("SetLoad_ConditionMngLib fail");
			
			m_saGroupName.RemoveAll();
			SetSendMainMsg("조건 파일 Load Error!");
		}
		
//-->Tree추가 080401
		HTREEITEM hTreeParent=TVI_ROOT;
		HTREEITEM hTree = NULL;
		HTREEITEM hSubTree = NULL;
		POSITION hInfoPos = NULL;
//<--Tree추가 080401
		for (nIndex_Group = 0; nIndex_Group < m_saGroupName.GetSize(); nIndex_Group++)
		{
			strGroupName = m_saGroupName.GetAt(nIndex_Group);

//-->Tree추가 080401
			hTree = m_treeList.InsertItem(strGroupName, hTreeParent);
			m_treeList.SetItemImage(hTree, 2, 1);		
			
//<--Tree추가 080401

			plistConditionInfo = new CList <ConditionInfo, ConditionInfo&>;
			for (nIndex_Item = 0; nIndex_Item < saCondiData.GetSize(); nIndex_Item++)
			{
				strCindiData = saCondiData.GetAt(nIndex_Item);
				nIndex = 0;
				//#	[형식] : Group View Name | File View Name | Condition File Path | Alert State | 설명 |
				while (strCindiData.IsEmpty() == FALSE)
				{
					nFindPos = strCindiData.Find("|", 0);
					if (nFindPos > -1)
					{
						strInfo[nIndex] = strCindiData.Left(nFindPos);
						strCindiData = strCindiData.Mid(nFindPos + 1, strCindiData.GetLength() - nFindPos);
						
						nIndex++;
					}
					else
						strCindiData = "";
				}
				
				if (strGroupName == strInfo[0])
				{
					ConditionInfo	stConditionInfo;
					
					stConditionInfo.nGroupID = IDC_BTN_GROUP + (nIndex_Group + 1);
					stConditionInfo.strGroupName = strGroupName;
					
					strInfo[1].Replace("_", " ");
					stConditionInfo.strName = strInfo[1];
					stConditionInfo.strPath = strInfo[2];
					stConditionInfo.strAlertKey = strInfo[3];
					stConditionInfo.strExplain = strInfo[4];
					
					stConditionInfo.bLoad = FALSE;
					stConditionInfo.nOrderCnt = 0;
					memset(&stConditionInfo.stUser_Finder, 0x00, SZ_User_Finder);
					hInfoPos = plistConditionInfo->AddTail(stConditionInfo);

//-->Tree추가 080401
					hSubTree = m_treeList.InsertItem(stConditionInfo.strName, hTree);
					
					m_treeList.SetItemData(hSubTree, (DWORD)hInfoPos);

					if(stConditionInfo.strAlertKey == "FALSE")
					{			
						m_treeList.SetItemImage(hSubTree, 4, 4);
					}
					else if(stConditionInfo.strAlertKey == "TRUE" || stConditionInfo.strAlertKey.IsEmpty() == FALSE)
					{	
						m_treeList.SetItemImage(hSubTree, 3, 3);						
					}
//<--Tree추가 080401
				}
			}
			
			m_mapCondition.SetAt(strGroupName, plistConditionInfo);
		}
		saCondiData.RemoveAll();
		
		GetData_RealSearchCondition();
		GetData_UserCondition();
			
//20080602 김성조 <사용자 조건 트리	
		POSITION pos;
		HTREEITEM hItem;
		
		if(m_mapCondition.Lookup(strGroupName, (void*&)plistConditionInfo) == TRUE)
		{
			ConditionInfo stConditionInfo;
			for (int nIndex = 0; nIndex < plistConditionInfo->GetCount(); nIndex++)
			{
				pos = plistConditionInfo->FindIndex(nIndex);
				stConditionInfo = plistConditionInfo->GetAt(pos);
				
				hItem = m_treeList.GetFirstVisibleItem();
				while(hItem != NULL)
				{
					hItem = m_treeList.GetNextSiblingItem(hItem);
					if(m_treeList.GetItemText(hItem) == (CString)TAB_TITLE_USERCONDI)
					{
						hSubTree = m_treeList.InsertItem(stConditionInfo.strName, hItem);
						m_treeList.SetItemData(hSubTree, (DWORD)pos);
						
						if(stConditionInfo.strAlertKey == "FALSE")
							m_treeList.SetItemImage(hSubTree, 4, 4);
						else if(stConditionInfo.strAlertKey == "TRUE" || stConditionInfo.strAlertKey.IsEmpty() == FALSE)
						{	
							m_treeList.SetItemImage(hSubTree, 3, 3);						
						}
					}					
				}			
			}			
		}
//20080602 김성조 사용자 조건 트리>
		
		SetSearch_MSTCode();
	}
	else
	{
		SetSendMainMsg("Conddition Lib Load Error! 고객센터로 문의 하세요!");
		
		m_btnFilterChk.ShowWindow(SW_HIDE);
		m_btnFilterSet.ShowWindow(SW_HIDE);
	}

	//20100330 SJ_KIM 항목편집
	SetLoad_FieldInfo();
	SetGrid_Head(FALSE);
	SetGrid_Head(TRUE);

	_DoGridWidthSet(0);
	/////////////////////////

	m_gridResult.SetMultiSelectMode(TRUE);
	m_ptabGroup->SetCurSel(nIndex_GroupSelect);
	if (m_saGroupName.GetSize() > 0)
	{
		m_strGroupName = m_saGroupName.GetAt(nIndex_GroupSelect);
		if (m_strGroupName == (CString)TAB_TITLE_USERCONDI)
			GetData_UserCondition();
		
		OnBtnListSearch();
	}	
	m_bInit = TRUE;

/*
	if(2 == m_nIndustry)
	{
		m_bLeftShowFlag = FALSE;
		m_btnExp.ShowWindow(SW_HIDE);
	}
*/
	//ChangeSkinColor();
	OnSize();
	SetToolTip();

	m_ctrlConditionText.SetBackgroundColor(FALSE, m_clrMainSkin);

	SetLoad_SignalMngLib();
}

void CMainFrmWnd::SetToolTip()
{
// 	CString strToolTip;
// 	
// 	strToolTip = "검색창 확대/축소";
// 	m_btnExp.SetTooltipText(strToolTip);
// 	
// 	strToolTip = "가격, 등락, 거래량의 조건설정 기능";
// 	m_btnFilterSet.SetTooltipText(strToolTip);
// 	
// 	strToolTip = "해당 조건을 통한 실시간 종목 감시기능";
// 	m_btnRegReal.SetTooltipText(strToolTip);
// 	
// 	strToolTip = "해당종목을 관련화면으로 전송";
// 	m_btnSend.SetTooltipText(strToolTip);
// 	
// 	strToolTip = "항목편집";
// 	m_btnItemEdit.SetTooltipText(strToolTip); // 20100210 박찬모
// 	
// 	if (m_ToolTipCtrl.m_hWnd == NULL)
// 	{
// 		m_ToolTipCtrl.Create(this);
// 		m_ToolTipCtrl.Activate(FALSE);
// 	}
// 	
// 	CRect reToolTip;
// 	if (m_ToolTipCtrl.GetToolCount() == 0)
// 	{
// 		m_ctrlJongCnt.GetWindowRect(reToolTip);
// 		ScreenToClient(reToolTip);
// 		strToolTip = "전체";
// 		m_ToolTipCtrl.AddTool(this, (LPCTSTR)strToolTip, reToolTip, 1);
// 		
// 		m_ctrlMax.GetWindowRect(reToolTip);
// 		ScreenToClient(reToolTip);
// 		strToolTip = "상한";
// 		m_ToolTipCtrl.AddTool(this, (LPCTSTR)strToolTip, reToolTip, 2);
// 		
// 		m_ctrlUp.GetWindowRect(reToolTip);
// 		ScreenToClient(reToolTip);
// 		strToolTip = "상승";
// 		m_ToolTipCtrl.AddTool(this, (LPCTSTR)strToolTip, reToolTip, 3);
// 		
// 		m_ctrlSteady.GetWindowRect(reToolTip);
// 		ScreenToClient(reToolTip);
// 		strToolTip = "보합";
// 		m_ToolTipCtrl.AddTool(this, (LPCTSTR)strToolTip, reToolTip, 4);
// 		
// 		m_ctrlDown.GetWindowRect(reToolTip);
// 		ScreenToClient(reToolTip);
// 		strToolTip = "하락";
// 		m_ToolTipCtrl.AddTool(this, (LPCTSTR)strToolTip, reToolTip, 5);
// 		
// 		m_ctrlMin.GetWindowRect(reToolTip);
// 		ScreenToClient(reToolTip);
// 		strToolTip = "하한";
// 		m_ToolTipCtrl.AddTool(this, (LPCTSTR)strToolTip, reToolTip, 6);
// 	}
// 	
// 	m_ToolTipCtrl.Activate(TRUE);
}

BOOL CMainFrmWnd::PreTranslateMessage(MSG* pMsg) 
{
	m_ToolTipCtrl.RelayEvent(pMsg);

	if(WM_RBUTTONUP == pMsg->message)
	{
		CRect rcLeftGrid;
		m_gridList.GetWindowRect(rcLeftGrid);

		if(!rcLeftGrid.PtInRect(pMsg->pt))
		{
			::PostMessage(GetParent()->m_hWnd, WM_CONTEXTMENU, 0, 0);
		}
 	}

	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrmWnd::SetSearch_MSTCode()
{
	int nType = 2;
	
	char* lpData = new char[sizeof(int) + 1];
//@유진사용안함.ntohl	nType = ntohl(nType);
	nType = nType;
	::memset(lpData, 0x00, sizeof(int) + 1);
	memcpy(lpData, &nType, sizeof(int) + 1);
	
	int nState = Modual_SendTr((CString)TRNO_MSTCODE, sizeof(int), (BYTE*)lpData);
	if (nState < 0)
		SetSendMainMsg("조회실패...");
	else
		SetSendMainMsg("조회중...");
	
	if (lpData)
		delete[] lpData;
}

void CMainFrmWnd::GetData_UserCondition()
{	
	//# User Condition Clear!
	POSITION pos;
	int nIndex, nIndex_Group, nIndex_Item, nFindPos;
	CString strGroupName, strCindiData, strInfo[5];
	CList <ConditionInfo, ConditionInfo&> *plistConditionInfo;
	for (pos = m_mapCondition.GetStartPosition(); pos != NULL;)
	{
		m_mapCondition.GetNextAssoc(pos, strGroupName, (void*&)plistConditionInfo);
		
		if (strGroupName == (CString)TAB_TITLE_USERCONDI)
		{
			if (plistConditionInfo != NULL)
			{
				plistConditionInfo->RemoveAll();
				delete plistConditionInfo;
				plistConditionInfo = NULL;
			}
			
			m_mapCondition.RemoveKey(strGroupName);
			break;
		}
	}
	
	//# 사용자 
	nIndex_Group = m_saGroupName.GetSize();
	strGroupName = (CString)TAB_TITLE_USERCONDI;
	CStringArray saGroupName, saCondiData;

	BOOL bIndustry = TRUE;
	if(0 == m_nIndustry || 2 == m_nIndustry)
		bIndustry = FALSE;
	
	m_ctrlUserList.ResetContent();
	
	if (Lib_GetConditionData(1, bIndustry, FALSE, saGroupName, saCondiData) == TRUE)
	{
		plistConditionInfo = new CList <ConditionInfo, ConditionInfo&>;

		m_ctrlUserList.AddString(TAB_TITLE_REALSEARCH);
		plistConditionInfo->AddTail(m_stRealConditionInfo);

		for (nIndex_Item = 0; nIndex_Item < saCondiData.GetSize(); nIndex_Item++)
		{
			strCindiData = saCondiData.GetAt(nIndex_Item);
			
			nIndex = 0;
			//#	[형식] : Group View Name | File View Name | Condition File Path | Alert State | 설명 |
			while (strCindiData.IsEmpty() == FALSE)
			{
				nFindPos = strCindiData.Find("|", 0);
				if (nFindPos > -1)
				{
					strInfo[nIndex] = strCindiData.Left(nFindPos);
					strCindiData = strCindiData.Mid(nFindPos + 1, strCindiData.GetLength() - nFindPos);
					
					nIndex++;
				}
				else
					strCindiData = "";
			}
			
			ConditionInfo	stConditionInfo;
			
			stConditionInfo.nGroupID = IDC_BTN_GROUP + nIndex_Group;
			stConditionInfo.strGroupName = strGroupName;
			
			//strInfo[1].Replace("_", " ");
			stConditionInfo.strName = strInfo[1];
			stConditionInfo.strPath = strInfo[2];
			stConditionInfo.strAlertKey = strInfo[3];
			stConditionInfo.strExplain = strInfo[4];

			m_ctrlUserList.AddString(stConditionInfo.strName); // 20100203 박찬모 >>
			
			stConditionInfo.bLoad = FALSE;
			stConditionInfo.nOrderCnt = 0;
			memset(&stConditionInfo.stUser_Finder, 0x00, SZ_User_Finder);
			plistConditionInfo->AddTail(stConditionInfo);
		}

		m_mapCondition.SetAt(strGroupName, plistConditionInfo);
	}

	m_ctrlUserList.SetCurSel(0);
	UpdateData(FALSE);
	
	saGroupName.RemoveAll();
	saCondiData.RemoveAll();
}

BOOL CMainFrmWnd::SetLoad_ConditionMngLib()
{
	if (m_hConditionMng != NULL)
		return TRUE;

	CString strRoot;
	strRoot = Variant(homeCC);

	CString strPath = strRoot + "\\dev\\Lib_ConditionMng.dll";
	m_hConditionMng = ::LoadLibrary(strPath); 
	
	if (m_hConditionMng)
	{
		Lib_Open = (_DLL_Open)::GetProcAddress(m_hConditionMng, "_DLL_Open");
		Lib_Close = (_DLL_Close)::GetProcAddress(m_hConditionMng, "_DLL_Close");
		Lib_GetConditionData = (_DLL_GetConditionData)::GetProcAddress(m_hConditionMng, "_DLL_GetConditionData");
		Lib_GetFilter = (_DLL_GetFilter)::GetProcAddress(m_hConditionMng, "_DLL_GetFilter");
		Lib_GetFind_Packet = (_DLL_GetFind_Packet)::GetProcAddress(m_hConditionMng, "_DLL_GetFind_Packet");
		Lib_GetFind_Packet_ByteOrdering = (_DLL_GetFind_Packet_ByteOrdering)::GetProcAddress(m_hConditionMng, "_DLL_GetFind_Packet_ByteOrdering");
		Lib_GetFilterToPacket = (_DLL_GetFilterToPacket)::GetProcAddress(m_hConditionMng, "_DLL_GetFilterToPacket");
		Lib_GetFormatText = (_DLL_GetFormatText)::GetProcAddress(m_hConditionMng, "_DLL_GetFromatText");
		Lib_SetUserFolderPath = (_DLL_SetUserFolderPath)::GetProcAddress(m_hConditionMng, "_DLL_SetUserFolderPath");
	}
	else
	{
		TRACE("\n# Lib_SignalMng.dll이 없습니다.");
		return FALSE;
	}

	CString strUserFolderPath;
	strUserFolderPath.Format("%s%s%s", Variant(homeCC), USRDIR, Variant(nameCC));
	Lib_SetUserFolderPath(strUserFolderPath);
	
	return TRUE;
}

BOOL CMainFrmWnd::SetLoad_SignalMngLib()
{
	if(m_hSignalMng)
		return TRUE;

	m_hSignalMng = ::LoadLibrary("lib_signalmng.dll");

	if(m_hSignalMng)
	{
		SignalLib_Open		= (_DLL_SetLibOpen)GetProcAddress(m_hSignalMng, "_DLL_SetLibOpen");
		SignalLib_Close		= (_DLL_SetLibClose)GetProcAddress(m_hSignalMng, "_DLL_SetLibClose");
		SignalLib_SetSignal	= (_DLL_SetSiganl)GetProcAddress(m_hSignalMng,"_DLL_SetSiganl");

		m_pWndDummy = SignalLib_Open(this, NULL, m_pModuleMainWnd);
	}
	else
		return FALSE;

	return TRUE;
}

void CMainFrmWnd::SetClear_Condition(CMapStringToPtr *pmapCondition)
{
	POSITION pos;
	CString strGroupName;
	CList <ConditionInfo, ConditionInfo&> *plistConditionInfo;
	for (pos = pmapCondition->GetStartPosition(); pos != NULL;)
	{
		pmapCondition->GetNextAssoc(pos, strGroupName, (void*&)plistConditionInfo);
		plistConditionInfo->RemoveAll();
		delete plistConditionInfo;
		plistConditionInfo = NULL;
		
		pmapCondition->RemoveKey(strGroupName);
	}
	pmapCondition->RemoveAll();
}

void CMainFrmWnd::OnDestroy() 
{
	//	CFrameWnd::OnDestroy();
	_DoGridWidthSet(1);
	
	if(m_bRealRegFlag)
	{

// 		//OnBtnRegReal();
// 		INewsSignalManager* pNSMng = (INewsSignalManager*)AfxGetPctrInterface(UUID_INewsSignalManager, 1);
// 		if(m_dwAdviseKey)
// 		{
// 			pNSMng->UnAdviseNewsSignal(m_dwAdviseKey);
// 			m_dwAdviseKey = NULL;
// 		}


		SetTrSend_UserAlertReal("DEL", TRUE);

		/*SetTrSend_AlertReal("DEL");*/
	}
	
	if (m_hConditionMng != NULL)
	{
		Lib_Close();
		FreeLibrary(m_hConditionMng); 
		m_hConditionMng = NULL;
	}

	if (m_hSignalMng)
	{
		SignalLib_Close(this);
		FreeLibrary(m_hSignalMng);
		m_hSignalMng = NULL;
	}
		
	if (m_ptabGroup != NULL)
	{
		delete m_ptabGroup;
		m_ptabGroup = NULL;
	}

	if(m_nIndustry == 2)
	{
		
// 		IMainInfoManager* pMainInfo = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager, 1);
// 		if(pMainInfo)
// 		{
// 			CWnd* pWnd = (CWnd*)pMainInfo->GetMainHandle();
// 			if(pWnd)
// 				pWnd->SendMessage(WMU_SIGNAL_REGRCVWND, (WPARAM)1, (LPARAM)this->GetSafeHwnd());
// 
// 		}
		
	}
	
	//m_gridList.DestroyWindow();
	//m_gridResult.DestroyWindow();
	
	SetClear_Condition(&m_mapCondition);
	m_listConditionInfo.RemoveAll();
	m_saGroupName.RemoveAll();

	m_treeImageList.DeleteImageList();

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
	CRect reClient, reBottom;
	GetClientRect(reClient);
	
	reClient.top += 1;
	reClient.bottom -= 5;
	reClient.right -= 5;
	reClient.left += 5;
	reBottom = reClient;
	
	BOOL bShow_Msg = FALSE;
	if (bShow_Msg == TRUE)
	{
		reBottom.bottom = reClient.bottom;
		
		reBottom.top = reBottom.bottom - 13;
		m_ctrlMsg.MoveWindow(&reBottom);

		reBottom.bottom = reBottom.top - 5;
		reBottom.top = reClient.top;
	}
//->@유진변경: 상단탭 보이지 않기
	if (m_ptabGroup != NULL && m_nIndustry != 2)
	{
		m_rtTab = reBottom;
		m_rtTab.top += 20;
		m_rtTab.bottom += 1;

		CRect rtTab = reBottom;
		rtTab.bottom = rtTab.top + 24;

		rtTab.top += 2;
		rtTab.left -= 2;
		rtTab.right += 2;
		m_ptabGroup->MoveWindow(&rtTab);
	}
	reBottom.top += 26;
//<--

	reBottom.bottom -= 2;
	reBottom.left += 2;
	reBottom.right -= 2;
	OnSize_SubMain(reBottom);
	
	Invalidate();
}

void CMainFrmWnd::OnSize_SubMain(CRect reClient)
{
	CRect reLeft;
	reLeft = reClient;
	
	if (m_bLeftShowFlag == TRUE)
	{
		reLeft.right = reLeft.left + 190;
		OnSize_Left(reLeft);
		
		reLeft.left = reLeft.right + 4;
	}
	
	if(!m_bLeftShowFlag)
		m_btnHelpShow.ShowWindow(SW_HIDE);
	else
		m_btnHelpShow.ShowWindow(SW_SHOW);

	reLeft.right = reClient.right;
	OnSize_Right(reLeft);
}

void CMainFrmWnd::OnSize_Left(CRect reClient)
{
	//reClient.top += 20;
	
	CRect reBottom;
	reBottom = reClient;
	
	if (m_bHelpShowFlag == TRUE)
		reBottom.top = reBottom.bottom - 100;
	else
		reBottom.top = reBottom.bottom + 3;
	m_editHelp.MoveWindow(&reBottom);
	
	reBottom.bottom = reBottom.top - 3;
	reBottom.top = reBottom.bottom - 18;
	m_btnHelpShow.MoveWindow(&reBottom);

	ST_MOVEITEM mItem;
	SET_ST_MOVEITEM(mItem, "btnDescription", &m_btnHelpShow);
	GetParent()->SendMessage(RMSG_KSMOVECTRL, 0, (LPARAM)&mItem);

	mItem.bVisible = m_bLeftShowFlag;
	GetParent()->SendMessage(RMSG_KSMOVECTRL, 1, (LPARAM)&mItem);
	
	reBottom.bottom = reBottom.top - 4;
	reBottom.top = reClient.top;
	m_gridList.MoveWindow(&reBottom);
//	m_treeList.MoveWindow(&reBottom); -hidden-
	
	/*
	reLine.left += 8;
	if (m_bIndustry == FALSE)
	{
	reLine.right = reLine.left + 12;
	m_btnCodeChk.MoveWindow(&reLine);
	
	  reLine.left = reLine.right + 3;
	  reLine.right = reLine.left + 100;
	  m_ctrlCodeInput.MoveWindow(&reLine);
	  
		reLine.left = reLine.right + 1;
		reLine.right = reLine.left + 30;
		m_btnListSearch.MoveWindow(&reLine);
		
		  reTop.top = reTop.bottom + 2;
		  reTop.bottom = reTop.top + 20;
		  reTemp = reTop;
		  reTemp.top += 4;
		  reTemp.bottom -= 2;
		  m_ctrlCodeName.MoveWindow(&reTemp);
		  
			reTop.top = reTop.bottom + 2;
			}
			
			  reTop.bottom = reClient.bottom;
			  m_gridList.MoveWindow(&reTop);
	*/
}

void CMainFrmWnd::OnSize_Right(CRect reClient)
{
	CRect rtNULL(0, 0, 0, 0);
	m_btnExp.MoveWindow(rtNULL);
	m_ctrlTitle.MoveWindow(rtNULL);
	m_ctrlUserList.MoveWindow(rtNULL);
	m_btnKospiChk.MoveWindow(rtNULL);
	m_btnKosdakChk.MoveWindow(rtNULL);
	if (1 == m_nIndustry)
	{
		m_btnTemaChk.MoveWindow(rtNULL);
		m_btnSearch.MoveWindow(rtNULL);
		m_comboSearchTime.MoveWindow(rtNULL);
		m_btnRollSearch.MoveWindow(rtNULL);
		m_btnRollSearchStop.MoveWindow(rtNULL);
		m_btnItemEdit.MoveWindow(rtNULL);
		m_ctrlAutoStr.MoveWindow(rtNULL);
	}
	else
	{
		m_btnFilterChk.MoveWindow(rtNULL);
		m_btnFilterSet.MoveWindow(rtNULL);
		m_btnSearch.MoveWindow(rtNULL);
		m_comboSearchTime.MoveWindow(rtNULL);
		m_btnRollSearch.MoveWindow(rtNULL);
		m_btnRollSearchStop.MoveWindow(rtNULL);
		m_btnSend.MoveWindow(rtNULL);
		m_btnItemEdit.MoveWindow(rtNULL);
		m_ctrlAutoStr.MoveWindow(rtNULL);
		m_btnRegReal.MoveWindow(rtNULL);
	}

	CRect reTop, reLine, reTemp;
	reTop = reClient;

	reTop.bottom = reTop.top + 20;
	
	if(m_nIndustry == 2)
	{
		reTop.top -= 26;
		reTop.bottom = reTop.top + 20;
		reTop.left -= 3;
		reTop.right += 4;
	}

	reTop.left -= 1;
	
	reLine = reTop;
	reLine.right = reLine.left + 20;
	m_btnExp.MoveWindow(&reLine);

	reTemp.top = reLine.top;
	reTemp.left = reLine.left;
	
	reLine.left = reLine.right + 1;
	reLine.right = reLine.left + 160;
	reLine.top += 5;
	reLine.bottom -= 3;
	m_ctrlTitle.MoveWindow(&reLine);
	
	reTemp.bottom = reLine.bottom;
	reTemp.right = reLine.right ;
	CRect reCombo = reTemp;
	reCombo.bottom += 100;
	m_ctrlUserList.MoveWindow(&reCombo);
	
	reLine.top -= 5;
	reLine.bottom += 3;
	
	reLine.top += 2;
	
	reLine.left = reLine.right + 2;
	reLine.right = reLine.left + 60;
	m_btnKospiChk.MoveWindow(&reLine);
	
	reLine.left = reLine.right + 2;
	reLine.right = reLine.left + 60;
	m_btnKosdakChk.MoveWindow(&reLine);
	
	ST_MOVEITEM mItem;
	if (1 == m_nIndustry)
	{
		reLine.left = reLine.right + 2;
		reLine.right = reLine.left + 50;
		m_btnTemaChk.MoveWindow(&reLine);
		
		reLine.top -= 2;
		
		reLine.left = reLine.right + 2;
		reLine.right = reLine.left + 60;
		m_btnSearch.MoveWindow(&reLine);

		reLine.left = reLine.right + 2;
		reLine.right = reLine.left + 50;
		reTemp = reLine;
		reTemp.bottom += 60;
		m_comboSearchTime.MoveWindow(&reTemp);
		
		reLine.left = reLine.right + 2;
		reLine.right = reLine.left + 20;
		m_btnRollSearch.MoveWindow(&reLine);
		m_btnRollSearchStop.MoveWindow(&reLine);

		reLine.left = reLine.right + 2;
		reLine.right = reLine.left + 60;
		m_btnItemEdit.MoveWindow(&reLine);
	
		// "종목전송" 위치에.
		reLine.left = reLine.right + 1;
		reLine.right = reLine.left + GetBtnWidth(6);
		reLine.top -= 2;
		m_ctrlAutoStr.MoveWindow(&reLine);
		reLine.top += 2;
	}
	else
	{
		reLine.left = reLine.right + 3;
		reLine.right = reLine.left + 12;
		m_btnFilterChk.MoveWindow(&reLine);
		
		reLine.top -= 2;
		
		reLine.left = reLine.right + 2;
		reLine.right = reLine.left + 36;
		m_btnFilterSet.MoveWindow(&reLine);
		
		reLine.left = reLine.right + 2;
		reLine.right = reLine.left + 60;
		m_btnSearch.MoveWindow(&reLine);
		
		reLine.left = reLine.right + 2;
		reLine.right = reLine.left + 50;
		reTemp = reLine;
		reTemp.bottom += 60;
		m_comboSearchTime.MoveWindow(&reTemp);

		reLine.left = reLine.right + 2;
		reLine.right = reLine.left + 20;
		m_btnRollSearch.MoveWindow(&reLine);
		m_btnRollSearchStop.MoveWindow(&reLine);
		
		reLine.left = reLine.right + 2;
		reLine.right = reLine.left + 60;
		m_btnSend.MoveWindow(&reLine);

		// 20100210 박찬모 >>
		reLine.left = reLine.right + 2;
		reLine.right = reLine.left + 60;
		m_btnItemEdit.MoveWindow(&reLine);
		// 20100210 박찬모 <<


		CRect baseRc = reLine;
		// "종목전송" 위치에.
		reLine.right += GetBtnWidth(2);
		reLine.top -= 2;
		m_ctrlAutoStr.MoveWindow(&reLine);
		reLine.top += 2;

		reLine = baseRc;
		int nBase = reLine.right +2;
		if(reTop.right-reLine.right-GetBtnWidth(2)>0)
		{
			reLine = reTop;
			reLine.left = reLine.right - 36;
			m_btnRegReal.MoveWindow(&reLine);
		}
		else
		{
			reLine.left = reLine.right + 1;
			reLine.right = reLine.left + 36;
			m_btnRegReal.MoveWindow(&reLine);
		}
	}

	reTop.top = reTop.bottom + 2;
	reTop.bottom = reClient.bottom;
	OnSize_Result(reTop);
}

void CMainFrmWnd::OnSize_Result(CRect reClient)
{
	CRect rtNULL(0, 0, 0, 0);
	
	if ((m_nIndustry != 1))
	{
		m_ctrlUpRate1.MoveWindow(rtNULL);
		m_ctrlUpRate.MoveWindow(rtNULL);
		m_btnDelOutItem.MoveWindow(rtNULL);
		m_ctrlAvg1.MoveWindow(rtNULL);
		m_ctrlAvg.MoveWindow(rtNULL);
	}
	
	if(2 == m_nIndustry)
	{
		m_ctrlConditionText.MoveWindow(&rtNULL);
	}
	
	CRect reTop, reLeft, reTemp;
	reTop = reClient;
	
//	if ((m_bRealRegFlag == FALSE) && (m_nIndustry != 1))
	if ((m_nIndustry != 1))
	{
		reTop.bottom = reTop.top + 22;
		reLeft = reTop;
		reLeft.right = reLeft.left + 200;
		OnSize_ResultBar(reLeft);
		
		reTemp = reLeft;
		int nWidth, nGap;
		nGap = 1;
		nWidth = (reClient.right - (reLeft.right + nGap * 2)) / 2;
		
		reLeft.left = reLeft.right + nGap;
		reLeft.right = reLeft.left + nWidth;
		m_ctrlUpRate1.MoveWindow(reLeft);
		
		reTemp = reLeft;
		reTemp.left += 1;
	//	reTemp.right -= 1;
		reTemp.right = reTemp.left + 120;
		reTemp.top += 4;
		reTemp.bottom -= 3;
		m_ctrlUpRate.MoveWindow(reTemp);


		reTemp.top -= 2;
	//	reTemp.bottom -=2;
		reTemp.left = reTemp.left + 120;
		reTemp.right = reTemp.left + 120;
		m_btnDelOutItem.MoveWindow(reTemp);
		
		reLeft.left = reLeft.right + nGap + 70;
		reLeft.right = reClient.right;
		m_ctrlAvg1.MoveWindow(reLeft);
		
		reTemp = reLeft;
		reTemp.left += 1;
		reTemp.right -= 1;
		reTemp.top += 4;
		reTemp.bottom -= 3;
		m_ctrlAvg.MoveWindow(reTemp);
		
		reTop.top = reTop.bottom + 2;
	}

	// 2009.10.27 -hidden-
	if(2 == m_nIndustry)
	{
		reTop.bottom += 60;
		m_ctrlConditionText.MoveWindow(&reTop);
		reTop.top = reTop.bottom + 2;
	}
	
	reTop.bottom = reClient.bottom;
	if(2 == m_nIndustry)
		reTop.bottom += 4;
	m_gridResult.MoveWindow(&reTop);
	
	Invalidate();
}

void CMainFrmWnd::OnSize_ResultBar(CRect reClient)
{
	CRect rtNULL(0, 0, 0, 0);
	m_ctrlMax1.MoveWindow(rtNULL);
	m_ctrlMax.MoveWindow(rtNULL);
	m_ctrlUp1.MoveWindow(rtNULL);
	m_ctrlUp.MoveWindow(rtNULL);
	m_ctrlSteady1.MoveWindow(rtNULL);
	m_ctrlSteady.MoveWindow(rtNULL);
	m_ctrlDown1.MoveWindow(rtNULL);
	m_ctrlDown.MoveWindow(rtNULL);
	m_ctrlMin1.MoveWindow(rtNULL);
	m_ctrlMin.MoveWindow(rtNULL);
	m_ctrlJongCnt1.MoveWindow(rtNULL);
	m_ctrlJongCnt.MoveWindow(rtNULL);
	
	CRect reLeft, reTemp;
	reClient.left +=1;
	reClient.right -= 1;
	reClient.top += 1;
	reClient.bottom -= 1;
	
	reLeft = reClient;
	
	int nWidth, nGap;
	nGap = 1;
	nWidth = reClient.right - reClient.left;
	nWidth = (nWidth - (6 * nGap)) / 6;
	
	reLeft.right = reLeft.left + nWidth;
	m_ctrlMax1.MoveWindow(reLeft);
	
	reTemp = reLeft;
	reTemp.left += 1;
	reTemp.right -= 1;
	reTemp.top += 4;
	reTemp.bottom -= 3;
	m_ctrlMax.MoveWindow(reTemp);
	
	reLeft.left = reLeft.right - nGap;
	reLeft.right = reLeft.left + nWidth;
	m_ctrlUp1.MoveWindow(reLeft);
	
	reTemp = reLeft;
	reTemp.left += 1;
	reTemp.right -= 1;
	reTemp.top += 4;
	reTemp.bottom -= 3;
	m_ctrlUp.MoveWindow(reTemp);
	
	reLeft.left = reLeft.right - nGap;
	reLeft.right = reLeft.left + nWidth;
	m_ctrlSteady1.MoveWindow(reLeft);
	
	reTemp = reLeft;
	reTemp.left += 1;
	reTemp.right -= 1;
	reTemp.top += 4;
	reTemp.bottom -= 3;
	m_ctrlSteady.MoveWindow(reTemp);
	
	reLeft.left = reLeft.right - nGap;
	reLeft.right = reLeft.left + nWidth;
	m_ctrlDown1.MoveWindow(reLeft);
	
	reTemp = reLeft;
	reTemp.left += 1;
	reTemp.right -= 1;
	reTemp.top += 4;
	reTemp.bottom -= 3;
	m_ctrlDown.MoveWindow(reTemp);
	
	reLeft.left = reLeft.right - nGap;
	reLeft.right = reLeft.left + nWidth;
	m_ctrlMin1.MoveWindow(reLeft);
	
	reTemp = reLeft;
	reTemp.left += 1;
	reTemp.right -= 1;
	reTemp.top += 4;
	reTemp.bottom -= 3;
	m_ctrlMin.MoveWindow(reTemp);
	
	reLeft.left = reLeft.right - nGap;
	reLeft.right = reLeft.left + nWidth;
	m_ctrlJongCnt1.MoveWindow(reLeft);
	
	reTemp = reLeft;
	reTemp.left += 1;
	reTemp.right -= 1;
	reTemp.top += 4;
	reTemp.bottom -= 3;
	m_ctrlJongCnt.MoveWindow(reTemp);
}

HBRUSH CMainFrmWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if ((pWnd->m_hWnd == m_ctrlMax.m_hWnd) ||
		(pWnd->m_hWnd == m_ctrlMax1.m_hWnd))
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(clrText);
		hbr = m_brushMax;
	}
	else if ((pWnd->m_hWnd == m_ctrlUp.m_hWnd) ||
		(pWnd->m_hWnd == m_ctrlUp1.m_hWnd))
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(clrText);
		hbr = m_brushUp;
	}
	else if ((pWnd->m_hWnd == m_ctrlSteady.m_hWnd) ||
		(pWnd->m_hWnd == m_ctrlSteady1.m_hWnd))
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_brushSte;
	}
	else if ((pWnd->m_hWnd == m_ctrlDown.m_hWnd) ||
		(pWnd->m_hWnd == m_ctrlDown1.m_hWnd))
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(clrText);
		hbr = m_brushDown;
	}
	else if ((pWnd->m_hWnd == m_ctrlMin.m_hWnd) ||
		(pWnd->m_hWnd == m_ctrlMin1.m_hWnd))
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(clrText);
		hbr = m_brushMin;
	}
	else if ((pWnd->m_hWnd == m_ctrlJongCnt.m_hWnd) ||
		(pWnd->m_hWnd == m_ctrlJongCnt1.m_hWnd))
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_brushCnt;
	}
	else if ((pWnd->m_hWnd == m_ctrlAvg.m_hWnd) ||
		(pWnd->m_hWnd == m_ctrlAvg1.m_hWnd))
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(m_clrTab);
		//hbr = m_brushTab;
		hbr = GetAxBrush(GetAxColor(66));
		if(m_nIndustry == 2)
			hbr = GetAxBrush(m_clrMainSkin);
		
		CString strAvg;
		m_ctrlAvg.GetWindowText(strAvg);
		
		COLORREF refColor;
		if (atof(strAvg) == 0.0)
			refColor = RGB(0, 0, 0);
		if (atof(strAvg) > 0.0)
			refColor = RGB(255, 0, 0);
		if (atof(strAvg) < 0.0)
			refColor = RGB(0, 0, 255);
		
		pDC->SetTextColor(refColor);
	}
	else if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(m_clrMainSkin);
		hbr = GetAxBrush(GetAxColor(66));
		if(m_nIndustry == 2)
			hbr = GetAxBrush(m_clrMainSkin);
 	}
	else
	{
	}
	
	return hbr;
}

void CMainFrmWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect reClient;
	GetClientRect(reClient);
//	dc.FillSolidRect(reClient, COLOR_MAIN_CANVAS);

	dc.FillSolidRect(reClient, m_clrMainSkin);
	
	if(m_nIndustry != 2)
	{
		dc.FillSolidRect(m_rtTab, GetAxColor(66));
		
		HPEN hOldPen;
		hOldPen = (HPEN)dc.SelectObject(GetAxPen(GetAxColor(13), 1, PS_SOLID));
		
		MoveToEx(dc.m_hDC, m_rtTab.right + 1, m_rtTab.top + 3, NULL);		// 오른쪽 테두리
		LineTo(dc.m_hDC, m_rtTab.right + 1,  m_rtTab.bottom);
		
		MoveToEx(dc.m_hDC, m_rtTab.right + 1, m_rtTab.bottom, NULL);	// 아래 테두리
		LineTo(dc.m_hDC, m_rtTab.left - 2, m_rtTab.bottom);
		
		MoveToEx(dc.m_hDC, m_rtTab.left - 2, m_rtTab.bottom, NULL);			// 왼쪽 테두리
		LineTo(dc.m_hDC, m_rtTab.left - 2, m_rtTab.top + 2);
		dc.SelectObject(hOldPen);
	}
	
// 	CRect rcClient;
// 	GetClientRect(&rcClient);
// 	rcClient.bottom = rcClient.top + 2;
// 	dc.FillSolidRect(rcClient, m_clrMainSkin);

//	m_gridResult.Invalidate();
}

void CMainFrmWnd::SetAddGrid_Condition(CList <ConditionInfo, ConditionInfo&> *plistConditionInfo)
{
	POSITION pos;
	ConditionInfo stConditionInfo;
	for (int nIndex = 0; nIndex < plistConditionInfo->GetCount(); nIndex++)
	{
		pos = plistConditionInfo->FindIndex(nIndex);
		stConditionInfo = plistConditionInfo->GetAt(pos);
		
		m_gridList.SetAddRow_List(&stConditionInfo, FALSE);
	}
	/*m_gridList.RedrawAll();*/

	m_ctrlConditionText.ShowWindow(SW_SHOW);		// 2009.10.27 -hidden-
}

void CMainFrmWnd::OnBtnCodeChk()
{
	BOOL bCheck = m_btnCodeChk.GetCheck();
	
	/*
	m_ctrlCodeInput.SetEnabled(bCheck);
	m_ctrlCodeName.EnableWindow(bCheck);
	m_btnListSearch.EnableWindow(bCheck);
	
	  BOOL bSearchFlag = TRUE;
	  if (bCheck == TRUE)
	  {
	  m_ctrlCodeInput.SetFocus();
	  
		CString strCode;
		strCode = m_ctrlCodeInput.GetDataText();
		strCode.TrimRight(" ");
		if (strCode.IsEmpty() == TRUE)
		bSearchFlag = FALSE;
		}
		
		  if (bSearchFlag == TRUE)
		  OnBtnListSearch();
	*/
}

void CMainFrmWnd::OnBtnListSearch()
{
	m_gridList.ClearAll();
	m_gridResult.ClearAll();
	
	if (m_strGroupName.IsEmpty() == TRUE)
	{
		SetSendMainMsg("선택 그룹이 없습니다.");
		return;
	}
	
	POSITION pos;
	CString strGroupName;
	CList <ConditionInfo, ConditionInfo&> *plistConditionInfo;
	if (m_btnCodeChk.GetCheck() == FALSE)         //# All
	{
		if (m_strGroupName == "전체")
		{
			for (pos = m_mapCondition.GetStartPosition(); pos != NULL;)
			{
				m_mapCondition.GetNextAssoc(pos, strGroupName, (void*&)plistConditionInfo);
				SetAddGrid_Condition(plistConditionInfo);
			}
			m_gridList.SetSelectRow(0);
//20080429 김성조
			
		}
		else
		{
			if (m_strGroupName == "테마")
				SetTrSend_TemaList();
			else
			{
				if (m_mapCondition.Lookup(m_strGroupName, (void*&)plistConditionInfo) == TRUE)
				{
					SetAddGrid_Condition(plistConditionInfo);
					m_gridList.SetSelectRow(0);
				}
			}
		}
		
		m_btnSearch.EnableWindow(TRUE);
	}
	else    //# Code
	{
	/*
	CString strCode = m_ctrlCodeInput.GetDataText();
	if (strCode.IsEmpty() == FALSE)
	{
	if (m_strGroupName == "전체")
	{
	for (pos = m_mapCondition.GetStartPosition(); pos != NULL;)
	{
	m_mapCondition.GetNextAssoc(pos, strGroupName, (void*&)plistConditionInfo);
	SetAddGrid_Condition(plistConditionInfo);
	}
	m_gridList.SetSelectRow(0);
	}
	else if (m_strGroupName == "테마")
	SetTrSend_TemaList();
	else
	SetTrSend_List(m_strGroupName);
	
	  m_btnSearch.EnableWindow(TRUE);
	  }
	  else
	  {
	  m_btnCodeChk.SetCheck(FALSE);
	  m_ctrlCodeName.SetWindowText("");
	  OnBtnCodeChk();
	  
		if (m_mapCondition.Lookup(m_strGroupName, (void*&)plistConditionInfo) == TRUE)
		SetAddGrid_Condition(plistConditionInfo);
		m_gridList.SetSelectRow(0);
		}
		*/
	}
}

void CMainFrmWnd::SetTrSend_TemaList()
{
	SetSendMainMsg("테마 조회중...");
	
	SetResult_Clear();
	
	m_listConditionInfo.RemoveAll();
	m_nTrIndex_List++;
	
	TR_LIST_TEMA_IN		stListTemaIn;
	memset(&stListTemaIn, 0x00, SZ_TR_LIST_TEMA_IN);
	
	if (m_btnCodeChk.GetCheck() == TRUE)
	{
	/*
	CString strCode; 
	strCode = m_ctrlCodeInput.GetDataText();
	if (strCode.IsEmpty() == FALSE)
	memcpy(&stListTemaIn.sCode, strCode.GetBuffer(0), sizeof(stListTemaIn.sCode));
	else
	memcpy(&stListTemaIn.sCode, "999999", sizeof(stListTemaIn.sCode));
		*/
	}
	else
		memcpy(&stListTemaIn.sCode, "999999", sizeof(stListTemaIn.sCode));
	
		/*
		if (m_btnFilterChk.GetCheck() == TRUE)
		{
		stListIn.sFilterGubun = '1';
		
		  User_Finder	stUser_Finder;
		  if (GetFilterPacket((long*)&stUser_Finder) == TRUE)
		  memcpy(&stListTemaIn.stFilter, &stUser_Finder, SZ_User_Define);
		  }
		  else
		  stListTemaIn.sFilterGubun = '0';
	*/
	//# 2006.01.23 => 업무 변경! (Filter는 Result에만 적용!)
	stListTemaIn.sFilterGubun = '0';
	
	//# Byte Ordering!
//@유진사용안함.ntohl	stListTemaIn.nTrIndex = ntohl(m_nTrIndex_List);
	stListTemaIn.nTrIndex = m_nTrIndex_List;
	
	int nSize_Packet = SZ_TR_LIST_TEMA_IN;
	char* lpData = new char[nSize_Packet];
	memcpy(lpData, &stListTemaIn, SZ_TR_LIST_TEMA_IN);
	
	//int nRet = Modual_SendTr((CString)TRNO_TEMACLK_LIST, SZ_TR_LIST_TEMA_IN, (BYTE *)lpData);
	
	if (lpData)
		delete[] lpData;
}

BOOL CMainFrmWnd::GetFilterPacket(long *plPacket)
{
	BOOL bResult = FALSE;
	
	CString strUserPath;
	strUserPath.Format("%s%s%s", Variant(homeCC), USRDIR, Variant(nameCC));
	
	//CString strFullPath = strUserPath + FILTTER_FILE;;  //20090615 SJ_KIM Fix the path
	CString strFullPath;
	if(m_nIndustry == 2)
		strFullPath = strUserPath + FILTTER_FILE_REAL;
	else
		strFullPath = strUserPath + FILTTER_FILE;
	
	if (Lib_GetFilterToPacket(strFullPath, plPacket) > 0)
		bResult = TRUE;
	
	return bResult;
}

BOOL CMainFrmWnd::GetFilterData(FilterInfo &stFilterInfo)
{
	BOOL bResult = FALSE;
	
	CString strAppPath;
	strAppPath = Variant(homeCC);

	CString strUserPath;
	strUserPath.Format("%s%s%s", strAppPath, USRDIR, Variant(nameCC));
	
	CString strFullPath;
	if(m_nIndustry == 2)
		strFullPath = strUserPath + FILTTER_FILE_REAL;
	else
		strFullPath = strUserPath + FILTTER_FILE;
	
	bResult = Lib_GetFilter(strFullPath, (long*)&m_stFilterInfo);
	
	return bResult;
}

void CMainFrmWnd::SetTrSend_List(CString strGroupName)
{
	SetSendMainMsg(strGroupName + " 조건 조회중...");

	SetResult_Clear();
	
	SetGroupToPacket(strGroupName);
	m_nTrIndex_List++;

	CString szTmp;
	//버퍼를 만들고 처리.
	int nPos = 0;
	char* pDataBuffer = new char[20000+1];
	EU_p0623_InRec1* pInRec1 = (EU_p0623_InRec1*)pDataBuffer[nPos];
	nPos = sizeof(EU_p0623_InRec1);

	TR_LIST_IN  stListIn;
	memset(&stListIn, 0x00, SZ_TR_LIST_IN);
	
	if (m_btnCodeChk.GetCheck() == TRUE)
	{
		CString strCode = "000660";
		//strCode = m_ctrlCodeInput.GetDataText();
		//@유진체크 : 이건뭐지 확인필요
		if (strCode.IsEmpty() == FALSE) 
		{
			memcpy(&stListIn.sCode, strCode.GetBuffer(0), sizeof(stListIn.sCode));

			//@유진 데이터 만들기
			memcpy(pInRec1->gsCode, strCode.GetBuffer(0), sizeof(stListIn.sCode));
		}
	}
	
	//# Byte Ordering!
//@유진사용안함.ntohl	stListIn.nTrIndex = ntohl(m_nTrIndex_List);
	stListIn.nTrIndex = m_nTrIndex_List;

	//@유진 데이터 만들기
	szTmp.Format("#04d", m_nTrIndex_List);
	memcpy(pInRec1->giCallGubun, szTmp.GetBuffer(0), 4);
	
	int nConditionCnt = 0;
	if (CONDITION_MAXCNT < m_listConditionInfo.GetCount())
		nConditionCnt = (int)CONDITION_MAXCNT;
	else
		nConditionCnt = m_listConditionInfo.GetCount();
//@유진사용안함.ntohl	stListIn.nConditionCnt = ntohl(nConditionCnt);
	stListIn.nConditionCnt = nConditionCnt;

	//@유진 데이터 만들기
	szTmp.Format("%#04d", nConditionCnt);
	memcpy(pInRec1->giConditionCnt, szTmp.GetBuffer(0), 4);

	int nIndex;
	POSITION pos;
	ConditionInfo		stConditionInfo;
	for (nIndex = 0; nIndex < nConditionCnt; nIndex++)
	{
		pos = m_listConditionInfo.FindIndex(nIndex);
		stConditionInfo = m_listConditionInfo.GetAt(pos);
		stListIn.sRankGubun[nIndex] = (stConditionInfo.nOrderCnt > 0)?'1':'0';

		//@유진 데이터 만들기
		pInRec1->gsRankGubun[nIndex] = (stConditionInfo.nOrderCnt > 0)?'1':'0';
	}

	int nSize_Packet = SZ_TR_LIST_IN + SZ_User_Finder * m_listConditionInfo.GetCount();
	char* lpData = new char[nSize_Packet];
	memcpy(lpData, &stListIn, SZ_TR_LIST_IN);

	CUser_Finder_To_0623InRec2	TrConvert; //여기작업.
	EU_p0623_InRec2* pInRec2 = NULL;
	for (nIndex = 0; nIndex < m_listConditionInfo.GetCount(); nIndex++)
	{
		pInRec2 = (EU_p0623_InRec2*)pDataBuffer[nPos];
		nPos += sizeof(EU_p0623_InRec2);

		pos = m_listConditionInfo.FindIndex(nIndex);
		stConditionInfo = m_listConditionInfo.GetAt(pos);
		
		//# Add Packet!
		memcpy(lpData + SZ_TR_LIST_IN + SZ_User_Finder * nIndex, &stConditionInfo.stUser_Finder, SZ_User_Finder);
		TrConvert.ConvertData(&stConditionInfo.stUser_Finder, pInRec2);
	}

//@유진 int nRet = Modual_SendTr((CString)TRNO_CONDIONECLK_LIST, nSize_Packet, (BYTE *)lpData);
	int nRet = Modual_SendTr(TRNO_CONDIONECLK_LIST, nPos, (BYTE*)pDataBuffer);
	
	if (lpData)	delete[] lpData;
	if(pDataBuffer) delete[] pDataBuffer;
}


void CMainFrmWnd::SetGroupToPacket(CString strGroupName)
{
	int nChgCnt = 0;
	POSITION pos;
	CList <ConditionInfo, ConditionInfo&> *plistConditionInfo;
	if (strGroupName == "전체")
	{
		m_listConditionInfo.RemoveAll();
		
		for (pos = m_mapCondition.GetStartPosition(); pos != NULL;)
		{
			m_mapCondition.GetNextAssoc(pos, strGroupName, (void*&)plistConditionInfo);
			
			nChgCnt = GetMakePacket_Condition(plistConditionInfo);
			if (nChgCnt > 0)
				m_mapCondition.SetAt(strGroupName, plistConditionInfo);
		}
	}
	else if (strGroupName == "테마")
	{
		
	}
	else
	{
		m_listConditionInfo.RemoveAll();
		if (m_mapCondition.Lookup(strGroupName, (void*&)plistConditionInfo) == TRUE)
		{
			GetMakePacket_Condition(plistConditionInfo);
			
			if (nChgCnt > 0)
				m_mapCondition.SetAt(strGroupName, plistConditionInfo);
		}
	}
}

int CMainFrmWnd::GetMakePacket_Condition(CList <ConditionInfo, ConditionInfo&> *plistConditionInfo)
{
	int nResult = 0;
	
	POSITION pos;
	ConditionInfo		stConditionInfo;
	for (int nIndex = 0; nIndex < plistConditionInfo->GetCount(); nIndex++)
	{
		pos = plistConditionInfo->FindIndex(nIndex);
		stConditionInfo = plistConditionInfo->GetAt(pos);
		
		if (stConditionInfo.bLoad == FALSE)
		{
			char *pcPacket;
			int nOrderCnt = 0;
			
			BOOL bIndustry;
			if(1 == m_nIndustry)
				bIndustry = TRUE;
			
			if (Lib_GetFind_Packet(stConditionInfo.strPath, 0, bIndustry, TRUE, TRUE, pcPacket, nOrderCnt) > 0)
			{
				stConditionInfo.bLoad = TRUE;
				stConditionInfo.nOrderCnt = nOrderCnt;
				memcpy(&stConditionInfo.stUser_Finder, pcPacket, SZ_User_Finder);
				
				plistConditionInfo->SetAt(pos, stConditionInfo);
				nResult++;
				
				delete pcPacket;
			}
		}
		
		//# Add Send Condtion Info!
		m_listConditionInfo.AddTail(stConditionInfo);
	}
	
	return nResult;
}

void CMainFrmWnd::OnBtnShowHelp()
{
	if (m_bHelpShowFlag == TRUE)
	{
		m_btnHelpShow.SetWindowText("선택지표설명 ▲");
		m_editHelp.ShowWindow(SW_HIDE);
	}
	else
	{
		m_btnHelpShow.SetWindowText("선택지표설명 ▼");
		m_editHelp.ShowWindow(SW_SHOW);
	}
	m_bHelpShowFlag = !m_bHelpShowFlag;
	
	OnSize();
	//m_editHelp.Invalidate();
}

#define NO_EXPEND_SIZE	564
#define FORM_WIDTH		845
#define FORM_HEIGHT		485
void CMainFrmWnd::OnBtnExp()
{
	m_bLeftShowFlag = !m_bLeftShowFlag;
			
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	if (m_bLeftShowFlag == TRUE)
	{
		pScreenWnd->SetBtnImg(&m_btnExp, "LEFT_EX");

// 		CConditionSearch_OneView* pParent = (CConditionSearch_OneView*)GetParent();
// 		pParent->FrameResize(FORM_WIDTH);
	}
	else
	{
		pScreenWnd->SetBtnImg(&m_btnExp, "RIGHT_EX");
		
		// 20100204 박찬모 >>
		/*if(m_nIndustry == 1)*/
		{
// 			CConditionSearch_OneView* pParent = (CConditionSearch_OneView*)GetParent();
// 			GetClientRect(&m_rectWnd);
// 			if(m_nIndustry)
// 				pParent->FrameResize(NO_EXPEND_SIZE);
// 			else
// 				pParent->FrameResize(NO_EXPEND_SIZE + 100 - 15);
		}
		// 20100204 박찬모 <<
	}
		
	int nCmdShow = (m_bLeftShowFlag == TRUE)?SW_SHOW:SW_HIDE;
	if (m_nIndustry != 1)
	{
		m_btnCodeChk.ShowWindow(nCmdShow);
		m_ctrlCodeInput.ShowWindow(nCmdShow);
		m_ctrlCodeName.ShowWindow(nCmdShow);
		m_btnListSearch.ShowWindow(nCmdShow);
	}
	m_gridList.ShowWindow(nCmdShow);
//	m_treeList.ShowWindow(nCmdShow);

	OnSize();

// 	int nX, nY;
// 	CRect rt;
// 	m_btnExp.GetWindowRect(rt);
// 	nX = rt.left + rt.Width()/2;
// 	nY = rt.top + rt.Height()/2;
// 
// 	if(m_nIndustry != 2)
// 		SetCursorPos(nX, nY);

	Invalidate();
}

void CMainFrmWnd::OnBtnSend()
{
	CRect reClient;
	m_btnSend.GetWindowRect(reClient);
	
	CPoint ptMenu;
	ptMenu.x = reClient.left;
	ptMenu.y = reClient.bottom;
	SetPopupMenuResultCtrl(ptMenu);
}

void CMainFrmWnd::SetPopupMenuResultCtrl(CPoint ptMenu)
{
	CMenu	menu;
	menu.CreatePopupMenu();

	CMenu *pMenuChild = NULL;
	pMenuChild = new CMenu;
	pMenuChild->CreatePopupMenu();
	
	CString strScreenInfo;
	int nMenuType;
	int nMenuCnt = _countof(MENU_RESULTCTRL_NAME);
	for (int nIndex = 0; nIndex < nMenuCnt; nIndex++)
	{
		if (MENU_RESULTCTRL_NAME[nIndex] == "")
			nMenuType = MF_SEPARATOR;
		else
			nMenuType = MF_STRING;
		
		//--> HTS구분없앰. 사용하면 추후 다시
		//		if(bIsHTS)
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

void CMainFrmWnd::OnMenuResultCtrl(UINT nIndex)
{
	CString strCode;
	CString strName;
	int nMarkType;
	ShardData		stShardData;
	CString strPortGroup;
	strPortGroup.Format("%d", nIndex);
	
	if(nIndex >= 11200)
		nIndex = 11020;

	int nDataCnt;
	nDataCnt = m_gridResult.GetNumberRows();
	
	if (nDataCnt <= 0)
	{
		SetSendMainMsg("전송할 데이터가 없음.");
		return;
	}
	
	int nItem;
	for (nItem = 0; nItem < _countof(MENU_RESULTCTRL_INDEX); nItem++)
	{
		if (nIndex == MENU_RESULTCTRL_INDEX[nItem])
			break;
	}
	
	if (nIndex == (int)SCREEN_INDEX_EXCEL)
		SetResultSave();
	else
	{
		BOOL bCodeCheck=FALSE;
		if(atoi(strPortGroup) >= 11200)
		{
			stShardData.saCode.Add(strPortGroup.Right(2));
			stShardData.saName.Add("");
			stShardData.iaMarkType.Add(0);
		}

		for (int nRow = 0; nRow < nDataCnt; nRow++)
		{
			bCodeCheck = m_gridResult.IsRowCodeChecked(nRow);
			if(bCodeCheck)
			{
				/*m_gridResult.GetRowToCode(nRow, strCode, nMarkType);*/
				m_gridResult.GetRowToCodeName(nRow, strCode, strName, nMarkType);
				
				if (!strCode.IsEmpty())
				{
					stShardData.saCode.Add(strCode);
					stShardData.saName.Add(strName);
					stShardData.iaMarkType.Add(nMarkType);
				}
			}
		}
		
		if (stShardData.saCode.GetSize() <= 0)
		{
			AfxMessageBox("전송할 데이터가 없습니다.");
			return;
		}
		
		stShardData.bOpen = MENU_RESULTCTRL_SCREEN_OPEN[nItem];
		stShardData.strScreenNo = MENU_RESULTCTRL_SCREEN_NOMAX[nItem];
		stShardData.strMapNo = MENU_RESULTCTRL_MAP_NO[nItem];
		
		SetSharedDataToSend(&stShardData);
	}
}

void CMainFrmWnd::SetSharedDataToSend(ShardData *pstShardData)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	CString strScreenNo(pstShardData->strScreenNo);

	if(pstShardData->saCode.GetSize() > 0)
	{
		pScreenWnd->OpenScreen(strScreenNo, &pstShardData->saCode, &pstShardData->saName);
	}
	else
	{
		pScreenWnd->OpenScreen(strScreenNo);
	}
}

void CMainFrmWnd::SetSharedDataToCode(CString strCode)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	pScreenWnd->ShareCode(strCode);
}

BOOL CMainFrmWnd::SetTempGroupReg(CStringArray *psaCode, CUIntArray *iaMarkType)
{
	if (psaCode->GetSize() <= 0)
		return FALSE;
	
	CString strCode;
	for (int nIndex = 0; nIndex < psaCode->GetSize(); nIndex++)
	{
		strCode = psaCode->GetAt(nIndex);
		strCode += "@";
		psaCode->SetAt(nIndex, strCode);
	}
	
	CString strGroupName = "조건검색결과";
//@유진삭제	if (Dll_AddRegKwansimMasterGroupItemForNewsSearch(strGroupName, *psaCode) == TRUE)
//@유진삭제		SetSendMainMsg("관심등록의 조건검색결과 그룹으로 종목을 보냈습니다.");
//@유진삭제	else
//		SetSendMainMsg("관심등록 실패.");
	
	return TRUE;
}

void CMainFrmWnd::SetResultSave()
{
	CFileFind FileFinder;
	
	BOOL bExist = FALSE;
	BOOL bSave = TRUE;
	
	int nReturn = 0;
	CString strMsg = "";
	CString strFileName, strFileType;
	CString strDefaultName = "임시저장";
	
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
				
				if(m_nIndustry == 0)
					nReturn = SetMessageBox(strMsg, PROGRAM_TITLE, MB_YESNO | MB_ICONQUESTION);
				else if(m_nIndustry == 1)
					nReturn = SetMessageBox(strMsg, PROGRAM_TITLE_INDUSTRY, MB_YESNO | MB_ICONQUESTION);
				else
					nReturn = SetMessageBox(strMsg, PROGRAM_TITLE_REAL, MB_YESNO | MB_ICONQUESTION);

				if (nReturn == IDYES)
				{
					if (!strFileName.IsEmpty())
					{
						if (!strFileType.CompareNoCase("xls"))				
							m_gridResult.SetSaveGridToExcel(strFileName);
						else if (!strFileType.CompareNoCase("txt"))				
							m_gridResult.SetSaveGridToText(strFileName);
					}
					bSave = FALSE;
					break;
				}
				else
					bSave = TRUE;
			}
			else
			{
				if (!strFileName.IsEmpty())
				{
					if (!strFileType.CompareNoCase("xls"))				
						m_gridResult.SetSaveGridToExcel(strFileName);
					else if (!strFileType.CompareNoCase("txt"))				
						m_gridResult.SetSaveGridToText(strFileName); //txt file save 
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
}

void CMainFrmWnd::OnBtnFilterChk()
{
	
}

void CMainFrmWnd::OnBtnFilterSet()
{	
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);	
	
	CFilterSetDlg	dlgFilterSet(this);
	dlgFilterSet.SetIndustry(m_nIndustry);

	if (dlgFilterSet.DoModal() == IDOK)
	{
		GetFilterData(m_stFilterInfo);
		if ((m_btnFilterChk.GetCheck() == TRUE) && (m_gridList.GetNumberRows() > 0))
			OnBtnSearch();
	}	

	AfxSetResourceHandle(hInstSave);
}

void CMainFrmWnd::OnBtnFilterSetOk()
{
	GetFilterData(m_stFilterInfo);
	if ((m_btnFilterChk.GetCheck() == TRUE) && (m_gridList.GetNumberRows() > 0))
		OnBtnSearch();
}

void CMainFrmWnd::OnBtnSearch()
{
	if (m_stConditionInfo.strGroupName == "테마")
		SetTrSend_TemaResult(&m_stConditionInfo);
	else
		SetTrSend_Result(&m_stConditionInfo);
}

void CMainFrmWnd::OnBtnRollSearch()
{
	int nIndex;
	nIndex = m_comboSearchTime.GetCurSel();
	
	if (m_bRollSearch == FALSE)
	{
		m_bRollSearch = TRUE;
		SetTimer(1, (UINT)ROLLSEARCH_TIME[nIndex] * 1000, NULL);
		//SetBtnImg(&m_btnRollSearch, "CS_RollSearchStop");
		m_btnRollSearch.ShowWindow(SW_HIDE);
		m_btnRollSearchStop.ShowWindow(SW_SHOW);
	}
	else
	{
		KillTimer(1);
		m_bRollSearch = FALSE;

		//SetBtnImg(&m_btnRollSearch, "CS_RollSearchPlay");
		m_btnRollSearch.ShowWindow(SW_SHOW);
		m_btnRollSearchStop.ShowWindow(SW_HIDE);
	}
	
	SetEnable(!m_bRollSearch);
	Invalidate();
}

void CMainFrmWnd::SetEnable(BOOL bEnable, BOOL bAlert /*=FALSE"*/)
{
	{	// 소리마치 컨트롤 Enable/Disable
		GetParent()->SendMessage(RMSG_KSENABLECTRL, (WPARAM)"btnFilter", (LPARAM)bEnable);
		GetParent()->SendMessage(RMSG_KSENABLECTRL, (WPARAM)"btnSearch", (LPARAM)bEnable);
		GetParent()->SendMessage(RMSG_KSENABLECTRL, (WPARAM)"btnSend", (LPARAM)bEnable);
		GetParent()->SendMessage(RMSG_KSENABLECTRL, (WPARAM)"btnReal", (LPARAM)bEnable);
		GetParent()->SendMessage(RMSG_KSENABLECTRL, (WPARAM)"btnItemEdit", (LPARAM)bEnable);
	}

	if (m_ptabGroup != NULL)
		m_ptabGroup->EnableWindow(bEnable);
	
	m_btnCodeChk.EnableWindow(bEnable);
	
	/*
	if (bEnable == TRUE)
	m_ctrlCodeInput.SetEnabled(m_btnCodeChk.GetCheck());
	else
	m_ctrlCodeInput.SetEnabled(FALSE);
	*/
	
	m_ctrlCodeName.EnableWindow(bEnable);
	m_btnListSearch.EnableWindow(bEnable);
	
	m_btnSend.EnableWindow(bEnable);
	m_btnItemEdit.EnableWindow(bEnable); // 20100210 박찬모
	
	m_btnFilterChk.EnableWindow(bEnable);
	m_btnFilterSet.EnableWindow(bEnable);
	m_btnSearch.EnableWindow(bEnable);
	
	m_comboSearchTime.EnableWindow(bEnable);
	if (bAlert == TRUE)
	{
		m_btnRollSearch.EnableWindow(bEnable);
		m_btnRollSearchStop.EnableWindow(bEnable);
		
		m_btnKospiChk.EnableWindow(bEnable);
		m_btnKosdakChk.EnableWindow(bEnable);
	}
	else if (bAlert == FALSE)
	{
		m_btnRegReal.EnableWindow(bEnable);		

		// 감시기능이 원래 FALSE이면 FALSE 그대로 남아있도록 처리한다.
		if(!m_bRegReal)	m_btnRegReal.EnableWindow(m_bRegReal);
	
	}
	m_gridList.EnableWindow(bEnable);
	
//	m_treeList.EnableWindow(bEnable);

}

void CMainFrmWnd::OnTimer(UINT nIDEvent) 
{
	if (m_bRollSearch == TRUE)
		OnBtnSearch();
	
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrmWnd::OnBtnRegReal()
{
	CString strMode;
	if (m_bRealRegFlag == TRUE)
		strMode = "DEL";
	else
		strMode = "REG";
		
	m_szSendTRMode = strMode;

	m_btnRegReal.EnableWindow(m_bRealRegFlag);
	
	if (m_strGroupName == (CString)TAB_TITLE_USERCONDI || m_strGroupName == (CString)TAB_TITLE_REALSEARCH)   // 2009.10.26 -hidden-
	{
		SetTrSend_UserAlertReal(strMode);
	}
	else
		SetTrSend_AlertReal(strMode);
}

void CMainFrmWnd::OnBtnDelOutItem()
{
	BOOL bCheck;
	bCheck = m_btnDelOutItem.GetCheck();
	m_gridResult.m_bChkDelOut = bCheck;

	if(bCheck == TRUE)
	{
		m_sect.Lock();
		m_gridResult.DeleteOutItem();
		m_sect.Unlock();	
	}
}

void CMainFrmWnd::SetTrSend_AlertReal(CString strMode)
{
	if (m_stConditionInfo.strAlertKey.IsEmpty() == FALSE)
	{
		m_strAlertKey = m_stConditionInfo.strAlertKey;
		
		CString strMag;
		m_szSendTRMode = strMode;
		if (strMode == "REG")
		{
			strMag = "조건등록 중...";
			
			m_btnRegReal.SetWindowText("해제");
			m_bRealRegFlag = TRUE;
			m_btnRegReal.Invalidate();    
			
			SetResult_Clear();
			m_gridResult.HeaderSetup("REAL", IDB_SIGNAL_STATE);
			m_gridResult.SetHeader(&m_listField, "REAL");
			
			SetTrSend_Result(&m_stConditionInfo, TRUE);
			SetEnable(FALSE, TRUE);

			CString strTitle;
			m_ctrlTitle.GetWindowText(strTitle);
			
			strMag.Format("[%s]  조건 등록중...", strTitle);

			OnBtnDelOutItem();
		}
		else if (strMode == "DEL")
		{
			
			strMag = "조건삭제 성공";
			
			m_btnRegReal.SetWindowText("감시");
			m_bRealRegFlag = FALSE;
			
			m_btnRegReal.Invalidate();
			Modual_SendReal(FALSE, m_strAlertKey);
			
			m_gridResult.HeaderSetup("RESULT");//김한두 : 20101208 : 감시해제 후 그리드 소팅안되는 현상 수정.
			m_gridResult.SetHeader(&m_listField, "RESULT");
			OnBtnSearch();
			
			SetEnable(TRUE, TRUE);
			m_strAlertKey = "";
		}
		
		SetSendMainMsg(strMag);
	}
}

void CMainFrmWnd::SetTrSend_UserAlertReal(CString strMode, BOOL bDestroy)
{
	REAL_USER_IN  stRealUserIn;
	memset(&stRealUserIn, 0x00, SZ_REAL_USER_IN);

	sprintf(stRealUserIn.sUserID, "%19.19s", Variant(userCC));

	if (m_strGroupName == (CString)TAB_TITLE_USERCONDI || m_strGroupName == "실시간검색")
		stRealUserIn.sType = 'U';
	else  //# [SYS] Real Reg!
		stRealUserIn.sType = 'S';
	
	//# strMode : REG, DEL, MDF
	//# 등록:'E', 변경:'C', 멈춤:'P', 삭제:'D'
	if (strMode == "REG")
		stRealUserIn.sFlag = 'E';
	else if (strMode == "DEL")
	{
		if (m_strAlertKey.IsEmpty() == TRUE)
		{
			SetSendMainMsg("Alert No 없음!");
			return;
		}
		
		memcpy(&stRealUserIn.sAlertNum, m_strAlertKey.GetBuffer(0), sizeof(stRealUserIn.sAlertNum));
		stRealUserIn.sFlag = 'D';
	}
	else if (strMode == "MDF")
	{
		if (m_strAlertKey.IsEmpty() == TRUE)
		{
			SetSendMainMsg("Alert No 없음!");
			return;
		}
		
		memcpy(&stRealUserIn.sAlertNum, m_strAlertKey.GetBuffer(0), sizeof(stRealUserIn.sAlertNum));
		stRealUserIn.sFlag = 'C';
	}
	
	memcpy(&stRealUserIn.sUserID, m_strUserID.GetBuffer(0), m_strUserID.GetLength());
	
	User_Finder	stUserFinder;
	if (GetMakePacket_Condition(m_stConditionInfo, &stUserFinder) == TRUE)
		memcpy(&stRealUserIn.stUserFinder, &stUserFinder, SZ_User_Finder);
	else
	{
		SetSendMainMsg("조회 데이터 오류. 재시도 하시오");
		return;
	}

	if(2 == m_nIndustry && m_strGroupName == (CString)TAB_TITLE_REALSEARCH)
		m_bOpenFromConditionSearch = TRUE;

	CUSERALERT_TR0825	clUSERALERT_TR0825;
	EU_USERALERT_IN1	stUSERALERT_IN1;
	clUSERALERT_TR0825.REAL_USER_IN_To_EU_USERALERT_IN1(&stRealUserIn, &stUSERALERT_IN1);

	EU_USERALERT_IN2	stUSERALERT_IN2;
	clUSERALERT_TR0825.USER_FINDER_To_EU_USERALERT_IN2(&stRealUserIn.stUserFinder, &stUSERALERT_IN2);

//	EU_USERALERT_IN3	stUSERALERT_IN3;
//	clUSERALERT_TR0825.USER_DEFINE_To_EU_USERALERT_IN3(stRealUserIn.stUserFinder.stUser_Define, &stUSERALERT_IN3);

// 2009.03.31 -hidden-
	CString strTemp;

	strTemp.Format("%.2s", stUSERALERT_IN2.giTargetIdxCnt);
	int nTargetCnt = atoi(strTemp);

	strTemp.Format("%.2s", stUSERALERT_IN2.giComplexIdxCnt);
	nTargetCnt += atoi(strTemp);

	int nSize_In1 = sizeof(EU_USERALERT_IN1);
	int nSize_In2 = sizeof(EU_USERALERT_IN2);
	int	nSize_In3 = sizeof(EU_USERALERT_IN3);
	int nTotalSize =  nSize_In1 + nSize_In2 + nSize_In3 * nTargetCnt;

	char* lpData = new char[nTotalSize];
	memset(lpData, 0x20, nTotalSize);
	memcpy(lpData, &stUSERALERT_IN1, nSize_In1);
	memcpy(lpData + nSize_In1, &stUSERALERT_IN2, nSize_In2);

	EU_USERALERT_IN3	stUSERALERT_IN3;
	for(int i = 0; i < nTargetCnt; ++i)
	{
		User_Define stUserItem = stRealUserIn.stUserFinder.stUser_Define[i];
		clUSERALERT_TR0825.USER_DEFINE_To_EU_USERALERT_IN3(&stUserItem, &stUSERALERT_IN3);

		memcpy(lpData + nSize_In1 + nSize_In2 + nSize_In3*i, &stUSERALERT_IN3, nSize_In3);

		if (stUserItem.SearchType == '1')
		{
			m_btnRegReal.SetWindowText("감시");
			m_btnRegReal.EnableWindow(TRUE);		
			m_bRealRegFlag = FALSE;
			m_btnRegReal.Invalidate();
		
			if (lpData)
				delete[] lpData;

			return;
		}
	}

	int nRet;
	if(strMode == "DEL")
	{
		nRet = Modual_SendTr(TRNO_ALERT_REALREG, nTotalSize, (BYTE*)lpData);

		if(bDestroy)
		{
			if (lpData)
				delete[] lpData;
			return;
		}


		m_btnRegReal.SetWindowText("감시");

		m_bRealRegFlag = FALSE;
	
		m_btnRegReal.Invalidate();
		/*Modual_SendReal(FALSE, m_strAlertKey);*/

		m_gridResult.HeaderSetup("RESULT");
		m_gridResult.SetHeader(&m_listField, "RESULT");
		m_bFromCall = TRUE;
		SetTrSend_Result(&m_stConditionInfo);
		m_bFromCall = FALSE;

		SetEnable(TRUE, TRUE);

		m_strAlertKey = "";
	}
		
	else
		nRet = Modual_SendTr((CString)TRNO_ALERT_REALREG, nTotalSize, (BYTE*)lpData);

	if (lpData)
		delete[] lpData;
}


void CMainFrmWnd::SetChangeTab(int nCurSel)
{
	if (m_saGroupName.GetSize() > 0)
	{
		m_strGroupName = m_saGroupName.GetAt(nCurSel);
		
		if (m_nIndustry != 1)
		{
			if (m_strGroupName == (CString)TAB_TITLE_CONTRACT)
			{
				m_gridResult.SetVisibleForecastPriceCol(TRUE);
				m_gridResult.SetLockColumns(m_nFixCol, TRUE);	
			}
			/*m_gridResult.QuickSetText(2, -1, "예상체결가");*/
			else
			{
				m_gridResult.SetVisibleForecastPriceCol(FALSE);
				if(m_nFixCol > 2)
					m_gridResult.SetLockColumns(m_nFixCol + 1, TRUE);	
				else
					m_gridResult.SetLockColumns(m_nFixCol, TRUE);	
			}
			/*m_gridResult.QuickSetText(1, -1, "종목명");*/
			m_gridResult.SetHeader(&m_listField, "RESULT");
		}
		
		BOOL bShow_RegReal = TRUE;
		
		m_btnRegReal.ShowWindow(SW_SHOW);
		if (m_strGroupName == (CString)TAB_TITLE_USERCONDI)
		{
			// 20100203 박찬모 >>
			m_bBeforeUserTap = TRUE; 
			m_bSaveShowFlag = m_bLeftShowFlag;
			m_btnExp.ShowWindow(SW_HIDE);
			m_ctrlTitle.ShowWindow(SW_HIDE);
			m_ctrlUserList.ShowWindow(SW_SHOW);
			if(m_bLeftShowFlag)
			{
				OnBtnExp();
			}

			// 20100203 박찬모 <<
			
			GetData_UserCondition();
			
			if (m_bIsFnUser == FALSE)
				m_btnRegReal.ShowWindow(SW_HIDE);
		}
		
		// 20100203 박찬모 >>
		else if(m_bBeforeUserTap)
		{
			if(m_bSaveShowFlag)
				OnBtnExp();
			m_ctrlUserList.ShowWindow(SW_HIDE);
			m_btnExp.ShowWindow(SW_SHOW);
			m_ctrlTitle.ShowWindow(SW_SHOW);
			m_bBeforeUserTap = FALSE;
			m_ctrlUserList.ResetContent();
		}
		// 20100203 박찬모 <<
		
		if (m_bRealRegFlag == TRUE)
			SetTrSend_AlertReal("DEL");
			
		OnBtnListSearch();
	}
	
	//Invalidate();
}

void CMainFrmWnd::OnChangeGroupTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nIndex = m_ptabGroup->GetCurSel();
	m_ctrlConditionText.SetWindowText("");		// 2009.10.28 -hidden-
	SetChangeTab(nIndex);
	m_gridList.AdjustComponentSizes();
	Invalidate();
}

//20080429 김성조 >>
void CMainFrmWnd::OnChangeTreeItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	CString strBtnReg;
	m_btnRegReal.GetWindowText(strBtnReg);

	if(strcmp(strBtnReg, "해제") == 0)
	{
		if(m_nIndustry == 0)
			SetMessageBox("감시중입니다. 조회는 해제후에 가능합니다", PROGRAM_TITLE);
		else if(m_nIndustry == 1)
			SetMessageBox("감시중입니다. 조회는 해제후에 가능합니다", PROGRAM_TITLE_INDUSTRY);
		else
			SetMessageBox("감시중입니다. 조회는 해제후에 가능합니다", PROGRAM_TITLE_REAL);
	}

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

 	HTREEITEM item = m_treeList.GetSelectedItem();
	
	if(m_treeList.GetItemData(item) == 0)
		SetChangeGroupTree(item);
	else
		SetChangeSubTree(item);
	
	*pResult = 0;
}

void CMainFrmWnd::SetChangeGroupTree(HTREEITEM hItem)
{
	m_strGroupName = m_treeList.GetItemText(hItem);

	if (m_nIndustry != 1)
	{
		if (m_strGroupName == (CString)TAB_TITLE_CONTRACT)
			m_gridResult.SetVisibleForecastPriceCol(TRUE);
		/*m_gridResult.QuickSetText(2, -1, "예상체결가");*/
		else
			m_gridResult.SetVisibleForecastPriceCol(FALSE);
		/*m_gridResult.QuickSetText(1, -1, "종목명");*/
		m_gridResult.SetHeader(&m_listField, "RESULT");
	}


	BOOL bShow_RegReal = TRUE;

	m_btnRegReal.ShowWindow(SW_SHOW);
	if (m_strGroupName == (CString)TAB_TITLE_USERCONDI)
	{
		GetData_UserCondition();
		
		if (m_bIsFnUser == FALSE)
			m_btnRegReal.ShowWindow(SW_HIDE);
	}

	if (m_bRealRegFlag == TRUE)
		SetTrSend_AlertReal("DEL");
		
	if(m_bInit == FALSE)
		OnBtnListSearch();		
}

void CMainFrmWnd::SetChangeSubTree(HTREEITEM hItem)
{
	HTREEITEM hParent;
	hParent = m_treeList.GetParentItem(hItem);
	m_strGroupName = m_treeList.GetItemText(hParent);

	if (m_nIndustry != 1)
	{
		if (m_strGroupName == (CString)TAB_TITLE_CONTRACT)
			m_gridResult.SetVisibleForecastPriceCol(TRUE);
		/*m_gridResult.QuickSetText(2, -1, "예상체결가");*/
		else
			m_gridResult.SetVisibleForecastPriceCol(FALSE);
		/*m_gridResult.QuickSetText(1, -1, "종목명");*/
		m_gridResult.SetHeader(&m_listField, "RESULT");
	}
	
	if ((m_gridList.m_strType == "RESULT") || (m_gridList.m_strType == "INDUSTRY"))
	{
	// 코드 
	//	if (col == 0)
	//		SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_RESULT_DBCLK, (LPARAM)row);
	}
	else if (m_gridList.m_strType == "LIST")
	{
	//	if (SetDrew_SelectRow(row) == TRUE)
	//선택 되어 있지 않을 때만 조회하도록
		SetSendDataTree(hItem);
	}
	else if (m_gridList.m_strType == "GROUP")
	{	
		CString strGroupName = m_treeList.GetItemText(hItem);
		
		SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_GROUP_CLK, (LPARAM)&strGroupName);
	}
}

void CMainFrmWnd::SetSendDataTree(HTREEITEM hItem)
{	
//
	if ((m_gridList.m_strType == "RESULT") || (m_gridList.m_strType == "REAL") || (m_gridList.m_strType == "INDUSTRY"))
	{
		//		SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_RESULT_CLK, (LPARAM)nRow);
	}
	else if (m_gridList.m_strType == "LIST")
	{
		ConditionInfo stConditionInfo;
		POSITION hInfoPos = NULL;
		hInfoPos = (POSITION)m_treeList.GetItemData(hItem);

		stConditionInfo = m_listConditionInfo.GetAt(hInfoPos);
		
		SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_LIST_CLK, (LPARAM)&stConditionInfo);
	}
	else if (m_gridList.m_strType == "GROUP")
	{
		CString strGroupName = m_treeList.GetItemText(hItem);
		SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_GROUP_CLK, (LPARAM)&strGroupName);
	}
	
}
//20080429 김성조 <<

///	

//void CMainFrmWnd::OnNotify_Grid(WPARAM wParam, LPARAM lParam)
LRESULT CMainFrmWnd::OnNotify_Grid(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NM_GRID_LIST_CLK)
	{
		ConditionInfo	stConditionInfo = *(ConditionInfo*)lParam;
		
//		if (stConditionInfo.strPath.Find(" ") > 0)
//			SetMessageBox("잘못된 파일명입니다.");
		
		m_stConditionInfo.nGroupID = stConditionInfo.nGroupID;
		m_stConditionInfo.strGroupName = stConditionInfo.strGroupName;
		
		m_btnRegReal.EnableWindow(TRUE);		
		if ((stConditionInfo.strAlertKey == "FALSE") || (stConditionInfo.strAlertKey.IsEmpty() == TRUE))
		{
			m_bRegReal = FALSE;
			m_btnRegReal.EnableWindow(FALSE);
		}
		else
		{
			m_bRegReal = TRUE;
		}
		
		m_stConditionInfo.strName = stConditionInfo.strName;
		m_stConditionInfo.strPath = stConditionInfo.strPath;
		m_stConditionInfo.strAlertKey = stConditionInfo.strAlertKey;
		m_stConditionInfo.strExplain = stConditionInfo.strExplain;
		
		m_stConditionInfo.bLoad = stConditionInfo.bLoad;
		m_stConditionInfo.nOrderCnt = stConditionInfo.nOrderCnt;

		m_ctrlTitle.SetWindowText(m_stConditionInfo.strName);
		
		//# Help
		m_editHelp.SetWindowText(m_stConditionInfo.strExplain);
		if (m_stConditionInfo.strGroupName == "테마")
			SetTrSend_TemaResult(&m_stConditionInfo);
		else
		{
			if(2 == m_nIndustry/* && m_strGroupName == (CString)TAB_TITLE_REALSEARCH && !m_bOpenFromConditionSearch*/)
				OnBtnRegReal();
			else
				SetTrSend_Result(&m_stConditionInfo);
		}
		m_gridList.Invalidate();
	}
	else if (wParam == NM_GRID_GROUP_CLK)
	{
		m_strGroupName = *(CString*)lParam;
		OnBtnListSearch();
	}
	else if (wParam == NM_GRID_RESULT_CLK)
	{
//-->@.080616-002 : 더블클릭시 코드공유에서 원클릭으로 변경.
// 		if (1 == m_nIndustry)
// 			//return;
// 			return FALSE;
		
		int nRow = (int)lParam;
		
		CString strCode; 
		int nMarketType;
		if (m_gridResult.GetRowToCode(nRow, strCode, nMarketType) == TRUE)
			SetSharedDataToCode(strCode);
		
		m_gridResult.SetFocus();
//<--
	}
	else if (wParam == NM_GRID_RESULT_DBCLK)
	{
		
		// 20110328 박찬모 검색 결과 Double Click 를 통한 종목 연동 기능 >>
// 		CString strCode;
// 		strCode.Format("%s", (CString*)lParam);
// 		CConditionSearch_OneView* pParent = (CConditionSearch_OneView*)GetParent();
// 		pParent->OnDBClick(strCode);

		// 20110328 박찬모 검색 결과 Double Click 를 통한 종목 연동 기능 <<
		
		
		
//-->@.080616-002 : 더블클릭시 코드공유에서 원클릭으로 변경.
//		if (m_bIndustry == TRUE)
//			return;
//		
//		int nRow = (int)lParam;
//		
//		CString strCode; 
//		int nMarketType;
//		if (m_gridResult.GetRowToCode(nRow, strCode, nMarketType) == TRUE)
//			SetSharedDataToCode(strCode);
//<--

			/*
			int nRow = (int)lParam;
			CString strCode; 
			int nMarketType;
			if (m_gridResult.GetRowToCode(nRow, strCode, nMarketType) == TRUE)
			{
			//# 현재가 연결!
			ShardData stShardData;
			stShardData.bOpen = TRUE;
			stShardData.strScreenNo = "0501";
			stShardData.strMapNo = "hfis077aq";
			
			  stShardData.iaMarkType.Add(nMarketType);
			  stShardData.saCode.Add(strCode);
			  
				SetSharedDataToSend(&stShardData);
				}
		*/
	}
	else if (wParam == NM_GRID_RESULT_SHARED)
	{
		ShardData *pstShardData = (ShardData*)lParam;
		SetSharedDataToSend(pstShardData);
	}

	return TRUE;
}

BOOL CMainFrmWnd::GetMakePacket_Condition(ConditionInfo	stConditionInfo,
										  User_Finder	*pstUser_Finder)
{
	BOOL bResult  = FALSE;
	
	CList <ConditionInfo, ConditionInfo&> *plistConditionInfo;
	if (m_mapCondition.Lookup(stConditionInfo.strGroupName, (void*&)plistConditionInfo) == TRUE)
	{
		POSITION pos;
		ConditionInfo		stConditionInfo_Target;
		for (int nIndex = 0; nIndex < plistConditionInfo->GetCount(); nIndex++)
		{
			pos = plistConditionInfo->FindIndex(nIndex);
			stConditionInfo_Target = plistConditionInfo->GetAt(pos);
			
			if (stConditionInfo.strPath == stConditionInfo_Target.strPath)
			{
				bResult = TRUE;
				if (stConditionInfo_Target.bLoad == FALSE)
				{
					char *pcPacket;
					int nOrderCnt = 0;

					BOOL bIndustry = FALSE;
					if(1 == m_nIndustry)
						bIndustry = TRUE;

					if (Lib_GetFind_Packet(stConditionInfo_Target.strPath, 0, bIndustry, TRUE, TRUE, pcPacket, nOrderCnt) > 0)
					{
						stConditionInfo_Target.bLoad = TRUE;
						stConditionInfo_Target.nOrderCnt = nOrderCnt;
						memcpy(&stConditionInfo_Target.stUser_Finder, pcPacket, SZ_User_Finder);
						
						plistConditionInfo->SetAt(pos, stConditionInfo_Target);
						m_mapCondition.SetAt(stConditionInfo.strGroupName, plistConditionInfo);
						
						delete pcPacket;
					}
					else
						bResult = FALSE;
				}
				
				// 2009.10.28 -hidden-
				if(m_hConditionMng)
				{
					CString strConditionText;
					m_ctrlConditionText.SetWindowText("");
					if(m_ctrlUserList.GetCurSel() == 0)
					{
// 						GETPATH_APP(strRoot);
// 						CString strFileRoot = strRoot + FOLDER_DATA + "\\";
// 						stConditionInfo_Target.strPath = strRoot + stConditionInfo_Target.strPath;
					}
						
					Lib_GetFormatText(stConditionInfo_Target.strPath, strConditionText);
 					m_ctrlConditionText.SetWindowText(strConditionText);
					
				}

				if (bResult == TRUE)
				{
					memset(pstUser_Finder, 0x00, SZ_User_Finder);
					memcpy(pstUser_Finder, &stConditionInfo_Target.stUser_Finder, SZ_User_Finder);
				}
				
				break;
			}
		}
	}
	
	return bResult;
}

void CMainFrmWnd::SetTrSend_Result(ConditionInfo *pstConditionInfo, BOOL bAlert)
{
// 	if(m_b0622Ing) 
// 	{
// 		SetMessageBox("(결과기다리는중..) 조회중입니다.", PROGRAM_TITLE);
// 	}


	SetSendMainMsg("결과 조회중...");
	SetResult_Clear();

	
	TR_RESULT_IN		stReultIn;
	memset(&stReultIn, 0x00, SZ_TR_RESULT_IN);
	
	if (m_strGroupName == (CString)TAB_TITLE_CONTRACT)
		stReultIn.sType = '1';
	
	if (bAlert == TRUE)
	{
		if (m_nTrIndex_Result <= 0)
			m_nTrIndex_Result--;
		else
			m_nTrIndex_Result = m_nTrIndex_Result * -1;
	}
	else
	{
		if (m_nTrIndex_Result < 0)
			m_nTrIndex_Result = m_nTrIndex_Result * -1;
		else
			m_nTrIndex_Result++;
	}
//@유진사용안함.아래루틴으로 변경ntohl	stReultIn.nTrIndex = ntohl(m_nTrIndex_Result);
	stReultIn.nTrIndex = m_nTrIndex_Result;

	/*
	if (m_btnFilterChk.GetCheck() == TRUE)
	{
	stReultIn.sFilterGubun = '1';
	
	  User_Finder	stUser_Finder;
	  if (GetFilterPacket((long*)&stUser_Finder) == TRUE)
	  memcpy(&stReultIn.stFilter, &stUser_Finder, SZ_User_Finder);
	  }
	  else
	  stReultIn.sFilterGubun = '0';
	*/

	int nMaxSize = sizeof(EU_p0622_InRec1) + sizeof(EU_p0622_InRec2) + (sizeof(EU_p0622_InRec3)*MAX_IDXCNT);
	char* pSendData = new char[nMaxSize+1];
	int nPos = 0;

	EU_p0622_InRec1* pInRec1 = (EU_p0622_InRec1*)&pSendData[nPos];
	nPos += sizeof(EU_p0622_InRec1);

	C0622TR TrHelper;
	TrHelper.TR_RESULT_IN_To_EU_p0622_InRec1(&stReultIn, pInRec1);

	User_Finder	stUserFinder;
	if (GetMakePacket_Condition(*pstConditionInfo, &stUserFinder) == TRUE)
	{
		memcpy(&stReultIn.stCondition, &stUserFinder, SZ_User_Finder);

		////////////////////////////////////////
		BOOL bHaveCodeFiled = FALSE;
		CUIntArray iaFixHeadId;
		stUserFinder.FieldEditerCnt = GetCount_FixHead(bHaveCodeFiled, iaFixHeadId);

		if (bHaveCodeFiled == TRUE)
			stUserFinder.FieldEditerCnt--;	

		long lHeadID;
		int nIndexOffSet = 0;
		for (int nIndex = 0; nIndex < iaFixHeadId.GetSize(); nIndex++)
		{
			lHeadID = (long)iaFixHeadId.GetAt(nIndex);		
			
			stUserFinder.FieldList[nIndexOffSet] = lHeadID;
			nIndexOffSet++;
			
		}
		iaFixHeadId.RemoveAll();
		/////////////////
		
		// 내부에서 nPos 위치부터 처리한다.
		TrHelper.User_Finder_To_EU_p0622_InRec2(&stUserFinder, &pSendData[0], nPos);
		pSendData[nPos] = 0;
	}
	else
	{
		if (pSendData)	delete[] pSendData;
		SetSendMainMsg("조회 데이터 오류. 재시도 하시오");
		return;
	}

	for (int i=0; i<MAX_IDXCNT; i++)
	{
		if (stUserFinder.stUser_Define[i].SearchType == '1')
		{
			AfxMessageBox(_T("랭귀지 조건을 포함한 사용자 조건은 검색이 불가합니다."));
			return;
		}
	}


	//@유진변경char* lpData = new char[SZ_TR_RESULT_IN];
	//@유진변경memcpy(lpData, &stReultIn, SZ_TR_RESULT_IN);
	//@유진변경int nRet = Modual_SendTr(TRNO_CONDIONECLK_RESULT, SZ_TR_RESULT_IN, (BYTE *)lpData);

	int nRet;
	
	// 한투 해제TR 응답기능을 제거 후 강제로 조회 TR을 보내야함
	// 하지만 SetTrSend_Result을 그대로 태울 시 "TRNO_CONDIONECLK_RESULT" 을 거치므로
	// 해당 TR을 거치지 않도록 m_bFromCall변수를 이용하여 구분
	if(!m_bFromCall) 
	{
		if(m_szSendTRMode == "DEL" && m_ptabGroup->GetCurSel() == 8)
			nRet = Modual_SendTr(m_sDelTRID, nPos, (BYTE*)pSendData);
		else
			nRet = Modual_SendTr(TRNO_CONDIONECLK_RESULT, nPos, (BYTE*)pSendData);
	}
	else
	{
		nRet = Modual_SendTr(TRNO_CONDIONECLK_RESULT, nPos, (BYTE*)pSendData);
	}
	

	m_szSendTRMode = _T("");
	
	//@유진변경if (lpData)	delete[] lpData;
	if (pSendData)	delete[] pSendData;
}

void CMainFrmWnd::SetSendMainMsg(CString strMsg)
{
	//m_ctrlMsg.SetWindowText(strMsg);
	UINT nID = WM_USER + 101;

	GetParent()->SendMessage(nID, NULL, (LPARAM)(LPCSTR)strMsg);
}

void CMainFrmWnd::SetTrSend_TemaResult(ConditionInfo *pstConditionInfo)
{
	CString strCode = pstConditionInfo->strPath;
	
	SetResult_Clear();
	m_nTrIndex_Result++;
	
	TR_RESULT_TEMA_IN		stResultTemaIn;
	memset(&stResultTemaIn, 0x00, SZ_TR_RESULT_TEMA_IN);
//@유진사용안함.ntohl	stResultTemaIn.nTrIndex = ntohl(m_nTrIndex_Result);
	stResultTemaIn.nTrIndex = m_nTrIndex_Result;
	
	if (m_btnFilterChk.GetCheck() == TRUE)
	{
		stResultTemaIn.sFilterGubun = '1';
		
		User_Finder	stUser_Finder;
		if (GetFilterPacket((long*)&stUser_Finder) == TRUE)
			memcpy(&stResultTemaIn.stFilter, &stUser_Finder, SZ_User_Finder);
	}
	else
		stResultTemaIn.sFilterGubun = '0';
	memcpy(stResultTemaIn.sCode, strCode.GetBuffer(0), sizeof(stResultTemaIn.sCode));
	
	char* lpData = new char[SZ_TR_RESULT_TEMA_IN];
	memcpy(lpData, &stResultTemaIn, SZ_TR_RESULT_TEMA_IN);
//	int nRet = Modual_SendTr((CString)TRNO_TEMACLK_RESULT, SZ_TR_RESULT_TEMA_IN, (BYTE *)lpData);
	
	SetMessageBox("@@@@@@@@@@제거확인");
	
	int nRet = 0;
	
	if (lpData)
		delete[] lpData;
	
	SetSendMainMsg("테마 결과 조회중...");
}

//extern CString		Glb_SubTrCode; //ViewWrap.cpp 파일에 global로 정의되어있음.	-hidden-
/*
int CMainFrmWnd::Modual_SendTr(CString TrNo, int nTrLen, BYTE *TrData)
{
	CString strMsg;
	if ((CString)TRNO_CONDIONECLK_LIST == TrNo)
		strMsg = "조건 조회 요청";
	else if ((CString)TRNO_CONDIONECLK_RESULT == TrNo)
		strMsg = "조건 결과 조회 요청";
	else if ((CString)TRNO_TEMACLK_LIST == TrNo)
		strMsg = "테마 조회 요청";
	else if ((CString)TRNO_TEMACLK_RESULT == TrNo)
		strMsg = "테마 결과 조회 요청";
	else if ((CString)TRNO_ALERT_REALREG == TrNo)
		strMsg = "실시간 등록 요청";
	SetSendMainMsg(strMsg);

	CString strScreenCode;
	strScreenCode.Format("%-10.10s", Glb_SubTrCode);

//@유진삭제		CCommOptionInfo	option;
//@유진삭제		
//@유진삭제		option.m_strTrCode   = TrNo;
//@유진삭제		option.m_Destination = 'V';
//@유진삭제		option.m_strScreenCode = strScreenCode;

	int nState=0;
//@유진삭제		nState = CommRequestDataOption(GetSafeHwnd(), (BYTE *)TrData , nTrLen, (BYTE *)&option);
	
	return nState;
}
*/

int CMainFrmWnd::Modual_SendTr(CString szTrNo, int nTrLen, BYTE *TrData)
{		
	m_CommCritical.Lock();
	
	CString strScreenCode = "";

	CString strMsg;
	char *pszSaveBuf = new char[nTrLen+200+1];
	int nPos = 0;

	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	
	if ((CString)TRNO_CONDIONECLK_RESULT == szTrNo)
	{
		m_b0622Ing = TRUE;
		strMsg = "조건 결과 조회 요청";
		InMakeFid_P0622(szTrNo, nTrLen, TrData, pszSaveBuf, nPos);

		pScreenWnd->SendTR(TRNO_CONDIONECLK_RESULT, (char*)TrData, nTrLen, TRKEY_CONDIONECLK_RESULT, US_KEY);
	}
	else if(szTrNo.CompareNoCase(TRNO_MSTCODE)==0)
	{
		if(CheckSavedMstcode())
		{
			if(pszSaveBuf) delete [] pszSaveBuf;
			pszSaveBuf = NULL;
			m_CommCritical.Unlock();
			return 1;
		}
		
		InMakeFid_Mstcode(szTrNo, nTrLen, TrData, pszSaveBuf, nPos);
		pScreenWnd->SendTR(TRNO_MSTCODE, pszSaveBuf, nTrLen, TRKEY_MSTCODE, US_KEY);
	}
	else if((CString)TRNO_ALERT_REALREG == szTrNo)
	{
//		InMakeFid_P0825(szTrNo, nTrLen, TrData, pszSaveBuf, nPos);

		
// 		m_pITrComm->SetBaseData("SCREENCODE", strScreenCode);
// 		m_pITrComm->SetTR(szTrNo);
// 		m_pITrComm->Send2Server(TrData, nTrLen);
		
		pScreenWnd->SendTR(szTrNo, (char*)TrData, nTrLen, TRKEY_ALERT_REALREG, US_KEY);
		
	}
	else if(szTrNo == TRNO_SET_REAL)
	{
		pScreenWnd->SendTR(szTrNo, (char*)TrData, nTrLen, TRKEY_SET_REAL, US_KEY);
	}
	else if(szTrNo == m_sDelTRID)
	{
		
// 		m_pITrComm->SetBaseData("SCREENCODE", strScreenCode);
// 		m_pITrComm->SetTR(szTrNo);
// 		m_pITrComm->Send2Server(TrData, nTrLen);
		
	}	
	else
	{
		
// 		m_pITrComm->SetBaseData("SCREENCODE", strScreenCode);
// 		m_pITrComm->SetTR(szTrNo);
// 		m_pITrComm->Send2Server(TrData, nTrLen);
		
				
// 		if(pszSaveBuf) delete [] pszSaveBuf;
// 		pszSaveBuf = NULL;
// 		m_CommCritical.Unlock();
// 
// 		CString szTmp;
// 		szTmp.Format("TR[%s] 진행중...", szTrNo);
// 		g_iMetaTable.ShowMessageBox(m_hWnd, szTmp);
		return 1;
	}
	SetSendMainMsg(strMsg);

	if(pszSaveBuf) delete [] pszSaveBuf;
	m_CommCritical.Unlock();

	// 실시간 해제TR은 응답이 없으므로 RMSG_WAIT_CORSOR를 발생시키지 않는다
	if(szTrNo != m_sDelTRID)
		GetParent()->SendMessage(RMSG_WAIT_CORSOR, (WPARAM)TRUE, 0);
	
	return 1;
}

long CMainFrmWnd::OnGetData(WPARAM wParam, LPARAM lParam)
{
	int nResult = -1;
//@유진삭제	
/*

	CRpDataParser rpParser;
	rpParser.OnGetData (wParam, lParam);
	
	if (!rpParser.IsError())
	{
		CString strTrCode;
		strTrCode = rpParser.GetTranCode();
		strTrCode.TrimRight(" ");
		!
		nResult = ReceiveData(strTrCode, rpParser.GetErrCode(), rpParser.GetErrMsg(), 
			rpParser.GetData(), rpParser.GetDataSize());
	}
	else
	{
		//char *pErrMsg = rpParser.GetErrMsg();
	
		//CString strMsg;
		//if (pErrMsg)
		//strMsg.Format("ErrorCode[%s]", rpParser.GetErrCode());
		//else
		//strMsg.Format("ErrorCode[%s]=[%s]", rpParser.GetErrCode(), pErrMsg);
		//SetSendMainMsg(strMsg);
	}
	
	if (nResult == 1)
	{
		CString strMsgCode = rpParser.GetReciveCode();
		CString strMsg = CommGetMsgString(strMsgCode);
		SetSendMainMsg(strMsg);
	}
*/	
	return nResult;
}

BOOL CMainFrmWnd::ReceiveData(LPCSTR szTR, LPVOID aTRData, long dwTRDateLen)
{
	m_CommCritical.Lock();
	BOOL bResult = TRUE;   
	
	char *pData;
	if (dwTRDateLen == 0)	
	{
		if ( strcmp(szTR,TRNO_CONDIONECLK_RESULT)==0)
		{
		}
		else if (strcmp(szTR ,TRNO_CONDIONECLK_LIST)==0)
			m_btnSearch.EnableWindow(FALSE);
	}
	else if (dwTRDateLen > 0)
	{
		if (strcmp(szTR ,TRNO_CONDIONECLK_RESULT)==0)
		{
			m_b0622Ing = FALSE;
			SetReceiveData_Result((char*)aTRData, dwTRDateLen);
			SetResultSearchTemp();
		}
		else if(strcmp(szTR ,TRNO_CONDIONECLK_LIST)==0)
		{
			TR_LIST_OUT		stListOut;
			::memset(&stListOut, 0x00, SZ_TR_LIST_OUT);
			::memcpy(&stListOut, aTRData, SZ_TR_LIST_OUT);
			
			stListOut.nTrIndex = stListOut.nTrIndex;
			if (m_nTrIndex_List != stListOut.nTrIndex)
			{
				m_CommCritical.Unlock();

				GetParent()->SendMessage(RMSG_WAIT_CORSOR, (WPARAM)FALSE, 0);
				return S_OK;
			}
			
			stListOut.nConditionCnt = stListOut.nConditionCnt;
			
			aTRData = (char*)aTRData + SZ_TR_LIST_OUT;
			pData = new char[stListOut.nConditionCnt];
			memset(pData, 0x00, stListOut.nConditionCnt);
			memcpy(pData, aTRData, stListOut.nConditionCnt);

			m_gridList.ClearAll();

			POSITION pos;
			ConditionInfo		stConditionInfo;
			for (int nIndex = 0; nIndex < m_listConditionInfo.GetCount(); nIndex++)
			{
				pos = m_listConditionInfo.FindIndex(nIndex);
				stConditionInfo = m_listConditionInfo.GetAt(pos);
				
				if (pData[nIndex] == '1')
					m_gridList.SetAddRow_List(&stConditionInfo, FALSE);
			}
			m_gridList.RedrawAll();
			m_gridList.SetSelectRow(0);
			
			delete [] pData;
			
			if (m_gridList.GetNumberRows() > 0)
				m_btnSearch.EnableWindow(TRUE);
			else
				m_btnSearch.EnableWindow(FALSE);
			/*m_btnSearch.Invalidate();*/
		}
		else if( strcmp(szTR ,TRNO_ALERT_REALREG)==0)
		{
			pData = new char[dwTRDateLen + 1];
			::memset(pData, 0x00, sizeof(pData));
			::memcpy(pData, aTRData, dwTRDateLen);
			
			SetReceiveData_UserAlertReal(pData, dwTRDateLen);
			delete [] pData;
			
			bResult = FALSE;
		}
		else if (strcmp(szTR, TRNO_MSTCODE)==0)
		{
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

			CStringArray saMSTCode;
			int nDataCnt = dwTRDateLen / sizeof(MSTCode);

			char *psData = (char*)aTRData;
			
			int nPos = 0;
			char aCode[6+1]={0,};
			for (int nIndex = 0; nIndex < nDataCnt; nIndex++)
			{
				nPos = nIndex*sizeof(MSTCode);				
				memcpy(aCode, &psData[nPos+5] , 6);
				saMSTCode.Add(aCode);
			}
			Lib_Open(&saMSTCode);
			saMSTCode.RemoveAll();
		}
		else if (strcmp(szTR, TRNO_SET_REAL)==0)
		{

		}
		else if ((CString)szTR == TRNO_FAVORITE) //tour2k 관심종목 그룹
		{
			char* pReceiveBuff = new char[dwTRDateLen + 1];
			memset(pReceiveBuff, 0x00, dwTRDateLen + 1);
			memcpy(pReceiveBuff, aTRData, dwTRDateLen);

			//if (nKey == TRKEY_GWANSIMGROUP)//관심종목
			{
				CStringArray arrName, arrKey;
				ParseGroup(pReceiveBuff, arrKey, arrName);
			}
			delete []pReceiveBuff; pReceiveBuff = nullptr;
		}
		else
		{
			/*SetMessageBox("@@@@@@@@@@제거확인", PROGRAM_TITLE);	*/
		}
	}
	m_CommCritical.Unlock();

	/*Invalidate();*/		// 2009.11.02 -hidden-
	/*m_gridResult.Invalidate();*/
	if(m_nIndustry == 2)
		Invalidate();
	else
		m_gridResult.Invalidate();

	GetParent()->SendMessage(RMSG_WAIT_CORSOR, (WPARAM)FALSE, 0);

	return bResult;
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

	m_GwanSimGroup.strKey.RemoveAll();
	m_GwanSimGroup.strGroupName.RemoveAll();

	for (int ii = 0; ii < cnt; ii++)
	{
		stgr = (struct _grSearch*)&pReceiveBuff[parseL];
		memset(pSeq, 0x00, sizeof(stgr->grseq) + 1);
		memcpy(pSeq, stgr->grseq, sizeof(stgr->grseq));
		arrKey.Add(pSeq);
		m_GwanSimGroup.strKey.Add(pSeq);

		memset(pgame, 0x00, sizeof(stgr->gname) + 1);
		memcpy(pgame, stgr->gname, sizeof(stgr->gname));

		arrName.Add(pgame);
		m_GwanSimGroup.strGroupName.Add(pgame);

		parseL += sizeof(struct _grSearch);

	}

	delete[] pSeq; pSeq = nullptr;
	delete[] pgame; pgame = nullptr;

	return 0;
}

BOOL CMainFrmWnd::SetReceiveData_UserAlertReal(LPTSTR pData, DWORD dwDataLen)
{
	BOOL bResult = FALSE;
	
	REAL_USER_OUT		stRealUserOut;
	::memset(&stRealUserOut, 0x30, SZ_REAL_USER_OUT);
	::memcpy(&stRealUserOut, pData, SZ_REAL_USER_OUT);
	
	char *pBuff;
	pBuff = new char[sizeof(stRealUserOut.sFlag) + 1];
	memset(pBuff, 0x00, sizeof(stRealUserOut.sFlag) + 1);
	memcpy(pBuff, &stRealUserOut.sFlag, sizeof(stRealUserOut.sFlag));
	CString strMode = (CString)pBuff;
	delete [] pBuff;
	
	pBuff = new char[sizeof(stRealUserOut.sResultFlag) + 1];
	memset(pBuff, 0x00, sizeof(stRealUserOut.sResultFlag) + 1);
	memcpy(pBuff, &stRealUserOut.sResultFlag, sizeof(stRealUserOut.sResultFlag));
	CString strResultFlag = (CString)pBuff;
	delete [] pBuff;
	
	pBuff = new char[sizeof(stRealUserOut.sTime) + 1];
	memset(pBuff, 0x00, sizeof(stRealUserOut.sTime) + 1);
	memcpy(pBuff, &stRealUserOut.sTime, sizeof(stRealUserOut.sTime));
	CString sTime = (CString)pBuff;
	delete [] pBuff;
	
	pBuff = new char[sizeof(stRealUserOut.sAlertNum) + 1];
	memset(pBuff, 0x00, sizeof(stRealUserOut.sAlertNum) + 1);
	memcpy(pBuff, &stRealUserOut.sAlertNum, sizeof(stRealUserOut.sAlertNum));
	m_strAlertKey = (CString)pBuff;
	delete [] pBuff;

	pBuff = new char[sizeof(stRealUserOut.sPBID) + 1];
	memset(pBuff, 0x00, sizeof(stRealUserOut.sPBID) + 1);
	memcpy(pBuff, &stRealUserOut.sPBID, sizeof(stRealUserOut.sPBID));
	m_sPBID = (CString)pBuff;
	delete [] pBuff;

	pBuff = new char[sizeof(stRealUserOut.sDelTRid) + 1];
	memset(pBuff, 0x00, sizeof(stRealUserOut.sDelTRid) + 1);
	memcpy(pBuff, &stRealUserOut.sDelTRid, sizeof(stRealUserOut.sDelTRid));
	m_sDelTRID = (CString)pBuff;
	delete [] pBuff;
	
	/*m_xTrCommSite.m_delTRID = m_sDelTRID;*/
	
	
	CString strMag;
	pBuff = new char[sizeof(stRealUserOut.sErrMsg) + 1];
	memset(pBuff, 0x00, sizeof(stRealUserOut.sErrMsg) + 1);
	memcpy(pBuff, &stRealUserOut.sErrMsg, sizeof(stRealUserOut.sErrMsg));
	strMag = (CString)pBuff;
	delete [] pBuff;
	strMag.TrimRight(" ");
	
	if (m_strAlertKey.IsEmpty() == TRUE)
		strResultFlag = "E";
	
	//# 등록:'E', 변경:'C', 멈춤:'P', 삭제:'D'
	if (strMode == "E")
		strMag += " 조건 등록 ";
	else if (strMode == "C")
		strMag += " 조건 변경 ";
	else if (strMode == "P")
		strMag += " 조건 멈춤 ";
	else if (strMode == "D")
		strMag += " 조건 삭제 ";
	
	if (strResultFlag == "S")
	{
		strMag += "성공";
		
		if (strMode == "D")
		{
			m_btnRegReal.SetWindowText("감시");
			m_bRealRegFlag = FALSE;		
			m_btnRegReal.Invalidate();
			/*Modual_SendReal(FALSE, m_strAlertKey);*/
			
			/*m_gridResult.HeaderSetup("RESULT");*/
			m_gridResult.SetHeader(&m_listField, "RESULT");
			OnBtnSearch();
			
			SetEnable(TRUE, TRUE);
			
			m_strAlertKey = "";
		}
		else
		{
			m_btnRegReal.SetWindowText("해제");
			m_bRealRegFlag = TRUE;
			m_btnRegReal.Invalidate();    
			
			SetResult_Clear();
			m_gridResult.HeaderSetup("REAL", IDB_SIGNAL_STATE);
			m_gridResult.SetHeader(&m_listField, "REAL");
			
			m_stConditionInfo.strAlertKey = m_strAlertKey;
			SetTrSend_Result(&m_stConditionInfo, TRUE);
			SetEnable(FALSE, TRUE);
		}
		
		SetSendMainMsg(strMag);
	}
	else if (strResultFlag == "E")
	{
		strMag += "실패";
		
		m_btnRegReal.SetWindowText("감시");
		m_bRealRegFlag = FALSE;
		m_btnRegReal.Invalidate();
		
		SetSendMainMsg(strMag);
	}
	else if (strResultFlag == "F")
	{
		strMag = " 실시간 감시 최대 인원을 초과 하였습니다.\n 잠시후 재시도 하세요.";
		SetMessageBox(strMag);
		
		m_btnRegReal.SetWindowText("감시");
		m_bRealRegFlag = FALSE;
		m_btnRegReal.Invalidate();
	}
	
	return bResult;
}

BOOL CMainFrmWnd::SetReceiveData_TemaList(LPTSTR pData, DWORD dwDataLen)
{
	TR_LIST_TEMA_OUT_HEADER		stListTemaOutHeader;
	::memset(&stListTemaOutHeader, 0x00, SZ_TR_LIST_TEMA_OUT_HEADER);
	::memcpy(&stListTemaOutHeader, pData, SZ_TR_LIST_TEMA_OUT_HEADER);
	
//@유진사용안함.ntohl	stListTemaOutHeader.nTrIndex = ntohl(stListTemaOutHeader.nTrIndex);
	stListTemaOutHeader.nTrIndex = stListTemaOutHeader.nTrIndex;
	if (m_nTrIndex_List != stListTemaOutHeader.nTrIndex)
		return FALSE;
	
//@유진사용안함.ntohl	stListTemaOutHeader.nCnt = ntohl(stListTemaOutHeader.nCnt);
	stListTemaOutHeader.nCnt = stListTemaOutHeader.nCnt;
	if (stListTemaOutHeader.nCnt <= 0)
		return FALSE;
	
	pData = (char*)pData + SZ_TR_LIST_TEMA_OUT_HEADER;
	dwDataLen = dwDataLen - SZ_TR_LIST_TEMA_OUT_HEADER;
	
	TR_LIST_TEMA_OUT	stTemaListOut;
	char *pBuff;
	for (int nIndex = 0; nIndex < stListTemaOutHeader.nCnt; nIndex++)
	{
		memset(&stTemaListOut, 0x00, SZ_TR_LIST_TEMA_OUT);
		memcpy(&stTemaListOut, pData, SZ_TR_LIST_TEMA_OUT);
		
		ConditionInfo	stConditionInfo;
		
		pBuff = new char[sizeof(stTemaListOut.sCode) + 1];
		memset(pBuff, 0x00, sizeof(stTemaListOut.sCode) + 1);
		memcpy(pBuff, &stTemaListOut.sCode, sizeof(stTemaListOut.sCode));
		stConditionInfo.strPath = (CString)pBuff;
		delete [] pBuff;
		
		pBuff = new char[sizeof(stTemaListOut.sName) + 1];
		memset(pBuff, 0x00, sizeof(stTemaListOut.sName) + 1);
		memcpy(pBuff, &stTemaListOut.sName, sizeof(stTemaListOut.sName));
		stConditionInfo.strName = (CString)pBuff;
		delete [] pBuff;
		
		stConditionInfo.bLoad = FALSE;
		stConditionInfo.strGroupName = "테마";
		
		m_gridList.SetAddRow_List(&stConditionInfo);
		
		pData = &pData[SZ_TR_LIST_TEMA_OUT];
		dwDataLen = dwDataLen - SZ_TR_LIST_TEMA_OUT;
	}
	m_gridList.RedrawAll();
	
	m_gridList.SetSelectRow(0);
	return TRUE;
}

BOOL CMainFrmWnd::SetReceiveData_Result(LPTSTR pData, DWORD dwDataLen)
{
//@유진변경. 메모리중복	TR_RESULT_HEADER		stTR_RESULT_HEADER;
//	memset(&stTR_RESULT_HEADER, 0x00, SZ_TR_RESULT_HEADER);
//	memcpy(&stTR_RESULT_HEADER, pData, SZ_TR_RESULT_HEADER);
	
	char szTrIndex[5];
	memset(szTrIndex, 0x00, 5);
	memcpy(szTrIndex, pData, 4);
	
	TR_RESULT_HEADER*	pstTR_RESULT_HEADER = (TR_RESULT_HEADER*)&pData[0];
	pstTR_RESULT_HEADER->nTrIndex = atoi(szTrIndex);

//@유진사용안함.ntohl	stTR_RESULT_HEADER.nTrIndex = ntohl(stTR_RESULT_HEADER.nTrIndex);
	//stTR_RESULT_HEADER.nTrIndex = stTR_RESULT_HEADER.nTrIndex;

	if (pstTR_RESULT_HEADER->nTrIndex != m_nTrIndex_Result)
	{
		if(m_nTrIndex_Result < 0)
		{
			OnBtnRegReal();
			CDlgNoItem dlgNoItem(this);
			dlgNoItem.DoModal();
		}
		else
			return FALSE;
	}
	
	BOOL bAlert_Result = FALSE;
	if (pstTR_RESULT_HEADER->nTrIndex < 0)
		bAlert_Result = TRUE;
	
	int nTotCnt;
	char *pBuff;
	pBuff = new char[sizeof(pstTR_RESULT_HEADER->sJongCnt) + 1];
	memset(pBuff, 0x00, sizeof(pstTR_RESULT_HEADER->sJongCnt) + 1);
	memcpy(pBuff, &pstTR_RESULT_HEADER->sJongCnt, sizeof(pstTR_RESULT_HEADER->sJongCnt));
	nTotCnt = atoi(pBuff);
	delete[] pBuff;
	
	//pData = &pData[SZ_TR_RESULT_HEADER];
	int nPos = SZ_TR_RESULT_HEADER;
	dwDataLen = dwDataLen - SZ_TR_RESULT_HEADER;
	
	
	CString strData;
	int nIndex, nAddCnt = 0, nSign;
	double dAddRate = 0;
	CUIntArray iaFluctuating;
	for (nIndex = 0; nIndex < 5; nIndex++)
		iaFluctuating.Add(0);
	
	BOOL bAdd;
	TR_RESULT_LIST		stTR_RESULT_LIST;
	
	CList <TR_RESULT_LIST, TR_RESULT_LIST&> listResult;
	for (nIndex = 0; nIndex < nTotCnt; nIndex++)
	{
		// 2012.2.14 박성경: 자동검색 모든종목 다 받게 처리 >>
// 		if (m_bRollSearch == TRUE)
// 		{
// 			if (nIndex >= 50)
// 				break;
// 		}
		// 2012.2.14 박성경: 자동검색 모든종목 다 받게 처리 <<
		
		memset(&stTR_RESULT_LIST, 0x00, SZ_TR_RESULT_LIST);
		memcpy(&stTR_RESULT_LIST, &pData[nPos], SZ_TR_RESULT_LIST);
		
		bAdd = FALSE;
		pBuff = new char[sizeof(stTR_RESULT_LIST.sMarket) + 1];
		memset(pBuff, 0x00, sizeof(stTR_RESULT_LIST.sMarket) + 1);
		memcpy(pBuff, &stTR_RESULT_LIST.sMarket, sizeof(stTR_RESULT_LIST.sMarket));
		strData = (CString)pBuff;
		delete[] pBuff;
		
		if (1 == m_nIndustry)
		{
			if (m_btnKospiChk.GetCheck() == TRUE)
			{
				//        if ((strData != "ⓚ") && (strData != "ⓣ"))
				if (strData == "1")
					bAdd = TRUE;
			}
			
			if (m_btnKosdakChk.GetCheck() == TRUE)
			{
				//if (strData == "ⓚ")
				if (strData == "2")
					bAdd = TRUE;
			}
			
			if (strData == "9")   //# 기타 (KRX, 예상지수, Kospi200, kosdaq50)
			{
				
			}
			
			if (m_btnTemaChk.GetCheck() == TRUE)
			{
				if (strData == "ⓣ")
					bAdd = TRUE;
			}
		}
		else
		{
			if (m_btnKospiChk.GetCheck() == TRUE)
			{
				if (strData == "1")
					bAdd = TRUE;
			}
			
			if (m_btnKosdakChk.GetCheck() == TRUE)
			{
				if (strData == "2")
					bAdd = TRUE;
			}
		}
		
		if ((m_btnFilterChk.GetCheck() == TRUE) && (bAdd == TRUE))
			bAdd = GetCheck_Filter(&m_stFilterInfo, stTR_RESULT_LIST);
		
		if (bAdd == TRUE)
		{
			pBuff = new char[sizeof(stTR_RESULT_LIST.sSign) + 1];
			memset(pBuff, 0x00, sizeof(stTR_RESULT_LIST.sSign) + 1);
			memcpy(pBuff, &stTR_RESULT_LIST.sSign, sizeof(stTR_RESULT_LIST.sSign));
			nSign = atoi(pBuff);
			delete[] pBuff;
			
			if ((nSign >= 1) && (nSign <= 5))
				iaFluctuating.SetAt(nSign - 1, iaFluctuating.GetAt(nSign - 1) + 1);
			
			pBuff = new char[sizeof(stTR_RESULT_LIST.sChgRate) + 1];
			memset(pBuff, 0x00, sizeof(stTR_RESULT_LIST.sChgRate) + 1);
			memcpy(pBuff, &stTR_RESULT_LIST.sChgRate, sizeof(stTR_RESULT_LIST.sChgRate));
			if(nSign >= 4)
				dAddRate -= atof(pBuff);
			else
				dAddRate += atof(pBuff);
			delete[] pBuff;
			
			listResult.AddTail(stTR_RESULT_LIST);
			
			nAddCnt++;
		}
		
		//pData = &pData[SZ_TR_RESULT_LIST];
		nPos += SZ_TR_RESULT_LIST;
		dwDataLen = dwDataLen - SZ_TR_RESULT_LIST;
	}
	
	m_gridResult.SetColResize(TRUE);
	POSITION pos;
	nTotCnt = listResult.GetCount();
	m_gridResult.SetNumberRows(nTotCnt, FALSE);
	
	int nCntColumn = m_listField.GetCount();
	
	for (nIndex = 0; nIndex < nTotCnt; nIndex++)
	{
		pos = listResult.FindIndex(nIndex);
		stTR_RESULT_LIST = listResult.GetAt(pos);

		char buf[32];
		memset(buf, 0x00, sizeof(buf));
		memcpy(buf, stTR_RESULT_LIST.sPrice, sizeof(stTR_RESULT_LIST.sPrice));
		int nPrice = atoi(buf);
		
		if (bAlert_Result == TRUE)
		{
			/*m_gridResult.SetAddRow_Alert(nIndex, stTR_RESULT_LIST);*/
			m_gridResult.SetAddRow_Alert_DynamicField(nIndex, nCntColumn, stTR_RESULT_LIST.gdResultField, 
				CString(stTR_RESULT_LIST.sName, sizeof(stTR_RESULT_LIST.sName)), (char)stTR_RESULT_LIST.sMarket, stTR_RESULT_LIST.sSign[0], stTR_RESULT_LIST.sCode, FALSE, nPrice);
		}
		else
		{
			if(m_nIndustry == 1)
				m_gridResult.SetAddRow_Result(nIndex, stTR_RESULT_LIST);
			else
				m_gridResult.SetAddRow(nIndex, stTR_RESULT_LIST.sSign[0], nCntColumn, (char)stTR_RESULT_LIST.sMarket,
					stTR_RESULT_LIST.sCode, stTR_RESULT_LIST.sName, stTR_RESULT_LIST.gdResultField, nPrice, FALSE);
		}

	}
	CUGHScroll* pGridHScroll = m_gridResult.m_CUGHScroll;
	pGridHScroll->SetScrollPos(0, FALSE);
	m_gridResult.SetColResize(TRUE);
//	m_gridResult.SetSelectRow(0);
	
	
	//# 1:상한 2:상승 3:보합 4:하한 5:하락
	strData.Format("%d", nAddCnt);
	m_ctrlJongCnt.SetWindowText(strData);
	
	strData.Format("%d", iaFluctuating.GetAt(0));
	m_ctrlMax.SetWindowText(strData);
	
	strData.Format("%d", iaFluctuating.GetAt(1));
	m_ctrlUp.SetWindowText(strData);
	
	strData.Format("%d", iaFluctuating.GetAt(2));
	m_ctrlSteady.SetWindowText(strData);
	
	strData.Format("%d", iaFluctuating.GetAt(4));
	m_ctrlDown.SetWindowText(strData);
	
	strData.Format("%d", iaFluctuating.GetAt(3));
	m_ctrlMin.SetWindowText(strData);
	
	int nUpSignCnt = iaFluctuating.GetAt(0) + iaFluctuating.GetAt(1);
	if (nUpSignCnt > 0)
		strData.Format("상승비중: %3.2f", (double)nUpSignCnt / (double)nAddCnt * 100);
	else 
		strData = "상승비중: 0.00";
	m_ctrlUpRate.SetWindowText(strData + " %");
	
	if (dAddRate != 0)
		strData.Format("평균등락률: %.2f", dAddRate / nAddCnt); 
	else
		strData = "평균등락률: 0.00";
	m_ctrlAvg.SetWindowText(strData + " %");
	
	/*m_gridResult.RedrawAll();*/
	
	if(m_bRealRegFlag)
		m_btnRegReal.EnableWindow(TRUE);

	if (nAddCnt == 0)
		SetSendMainMsg("조회 결과가 없습니다.");
	
	if (bAlert_Result == TRUE)
		Modual_SendReal(TRUE, m_strAlertKey);
	
	return TRUE;
}

BOOL CMainFrmWnd::GetCheck_Filter(FilterInfo *pstFilterInfo, TR_RESULT_LIST stTR_RESULT_LIST)
{
	CString strData, strTemp;
	if (pstFilterInfo == NULL)
		return FALSE;

	char *pData;
	int nIndex, nData;
	BOOL bState;

	//# 제외 종목
	bState = TRUE;
	CString strCode;
	STR_CONVERT(pData, stTR_RESULT_LIST.sCode, sizeof(stTR_RESULT_LIST.sCode), strCode)
		for (nIndex = 0; nIndex < pstFilterInfo->saOutCode.GetSize(); nIndex++)
		{
			strData = pstFilterInfo->saOutCode.GetAt(nIndex);
			if (strCode == strData.Left((int)CODE_LEN))
			{
				bState = FALSE;
				break;
			}
		}
		if (bState == FALSE)
			return FALSE;
		
		
		if (pstFilterInfo->nMarketType > 0)			//# 0 : 전체, 1 : 거래소, 2 : 코스닥
		{
			INT_CONVERT(pData, stTR_RESULT_LIST.sMarket, sizeof(stTR_RESULT_LIST.sMarket), nData)
				
				if (pstFilterInfo->nMarketType != nData)
					return FALSE;
		}
		
		if (pstFilterInfo->nNowPriceChk == 1)
		{
			INT_CONVERT(pData, stTR_RESULT_LIST.sPrice, sizeof(stTR_RESULT_LIST.sPrice), nData)
				if ((pstFilterInfo->nNowPriceUp > nData) ||
					(pstFilterInfo->nNowPriceDown < nData))
					return FALSE;
		}
		
		if (pstFilterInfo->nChangRateChk == 1)
		{
			pData = new char[sizeof(stTR_RESULT_LIST.sChgRate) + 1];
			memset(pData, 0x00, sizeof(stTR_RESULT_LIST.sChgRate) + 1);
			memcpy(pData, &stTR_RESULT_LIST.sChgRate, sizeof(stTR_RESULT_LIST.sChgRate));
			nData = (int)(atof(pData) * 100);
			delete[] pData;
			
			pData = new char[sizeof(stTR_RESULT_LIST.sSign) + 1];
			memset(pData, 0x00, sizeof(stTR_RESULT_LIST.sSign) + 1);
			memcpy(pData, &stTR_RESULT_LIST.sSign, sizeof(stTR_RESULT_LIST.sSign));
			int nSign = atoi(pData);
			delete[] pData;
			
			if ((nSign == 4) || (nSign == 5))
				nData = nData * -1;
			
			if ((pstFilterInfo->nChangRateUp > nData) ||
				(pstFilterInfo->nChangRateDown < nData))
				return FALSE;
		}
		
		pData = new char[sizeof(stTR_RESULT_LIST.sSign) + 1];
		memset(pData, 0x00, sizeof(stTR_RESULT_LIST.sSign) + 1);
		memcpy(pData, &stTR_RESULT_LIST.sSign, sizeof(stTR_RESULT_LIST.sSign));
		nData = atoi(pData);
		delete[] pData;
		
		if ((nData < 1) && (nData > 5))
			return FALSE;
		
		for (nIndex = 0; nIndex < (int)CNT_CHANGEVAL; nIndex++)
		{
			bState = FALSE;
			//# '1' : 상한, '2' : 상승, '3' : 보합, '4' : 하한, '5' : 하락
			if ((pstFilterInfo->nChangValChk[nIndex] == 1) && (nData == nIndex + 1))
			{
				bState = TRUE;
				break;
			}
		}
		
		if (bState == FALSE)
			return FALSE;
		
		int nMaxVal, nMax_Time = 0;
		POSITION pos;
		SubFilterInfo stSubFilterInfo;
		pData = new char[sizeof(stTR_RESULT_LIST.sVolume) + 1];
		memset(pData, 0x00, sizeof(stTR_RESULT_LIST.sVolume) + 1);
		memcpy(pData, &stTR_RESULT_LIST.sVolume, sizeof(stTR_RESULT_LIST.sVolume));
		nData = atoi(pData);
		delete[] pData;
		
		int nDealVol;
		if (pstFilterInfo->nDealVolType == 0)		//# 0 : 고정, 1 : Time
		{
			// >> 0001908: [0362] 빠른종목검색 - 필터의 시간대별거래량이 동작하지 않는 현상
			nDealVol = pstFilterInfo->nDealVol * 1000; 
			if (nDealVol > nData)
				return FALSE;
		}
		else if (pstFilterInfo->nDealVolType == 1)		//# 0 : 고정, 1 : Time
		{
			bState = FALSE;
			
			CTime timeNow;
			timeNow = CTime::GetCurrentTime();
			//strTemp = timeNow.Format("%H:%M:%S");
			strTemp = timeNow.Format("%H");

			CString strMin = timeNow.Format("%M");
			if(_ttoi(strMin) > 0)
			{
				strTemp.Format("%d", _ttoi(strTemp) + 1);
			}
			int nTime = atoi(strTemp);
			for (nIndex = 0; nIndex < pstFilterInfo->plistDealTimeVol->GetCount(); nIndex++)
			{
				pos = pstFilterInfo->plistDealTimeVol->FindIndex(nIndex);
				stSubFilterInfo = pstFilterInfo->plistDealTimeVol->GetAt(pos);
				
				//nDealVol = pstFilterInfo->nDealVol * 1000;
				nDealVol = stSubFilterInfo.nData * 1000;
				//if ((stSubFilterInfo.nTime == nTime) && (nDealVol <= nData))
				if(stSubFilterInfo.nTime >= nTime)
				{
					if(nDealVol <= nData)
						bState = TRUE;
					else
						return FALSE;
				}
				
				if (nMax_Time < stSubFilterInfo.nTime)
				{
					nMax_Time = stSubFilterInfo.nTime;
					nMaxVal = nDealVol;
				}
			}
		}
		// << 0001908
		
		/*
		pData = new char[sizeof(stCommonInfo.sAmount) + 1];
		memset(pData, 0x00, sizeof(stCommonInfo.sAmount) + 1);
		memcpy(pData, &stCommonInfo.sAmount, sizeof(stCommonInfo.sAmount));
		nData = atoi(pData);
		delete[] pData;
		
		  int nDealPrice;
		  if (pstFilterInfo->nDealPriceType == 0)		//# 0 : 고정, 1 : Time
		  {
		  nDealPrice = pstFilterInfo->nDealPrice * 100;
		  if (nDealPrice > nData)
		  return FALSE;
		  }
		  else if (pstFilterInfo->nDealPriceType == 1)		//# 0 : 고정, 1 : Time
		  {
		  bState = FALSE;
		  for (nIndex = 0; nIndex < pstFilterInfo->plistDealTimePrice->GetCount(); nIndex++)
		  {
		  pos = pstFilterInfo->plistDealTimePrice->FindIndex(nIndex);
		  stSubFilterInfo = pstFilterInfo->plistDealTimePrice->GetAt(pos);
		  
			nDealPrice = pstFilterInfo->nDealPrice * 100;
			if ((stSubFilterInfo.nTime == nTime) && (nDealPrice > nData))
			{
			bState = TRUE;
			break;
			}
			
			  if (nMax_Time < stSubFilterInfo.nTime)
			  {
			  nMax_Time = stSubFilterInfo.nTime;
			  nMaxVal = nDealPrice;
			  }
			  }
			  
				if ((bState == FALSE) && (nMaxVal > nData))
				return FALSE;
				}
				if (m_bTrace_Filter == TRUE)
				{
				strTemp.Format("%d", nData);
				SetWriteDebug("Check Filter OK [Amount]", strTemp);
				}
		*/
		
		
		
		
		/*
		CUIntArray	iaDirectionChk;
		for (nIndex = 0; nIndex < (int)CNT_DIRECTION; nIndex++)
		{
		if (pstFilterInfo->nDirectionChk[nIndex] == 1)
		iaDirectionChk.Add(nIndex);
		}
		
		  if (iaDirectionChk.GetSize() > 0)
		  {
		  int nIndex_Chk;  
		  for (nIndex = 0; nIndex < iaDirectionChk.GetSize(); nIndex++)
		  {
		  nIndex_Chk = iaDirectionChk.GetAt(nIndex);
		  
			switch (nIndex_Chk)
			{
			case 0 :		//# 관리
			{
			nIndex = 0;
			pData = new char[sizeof(stCommonInfo.sManagement) + 1];
			memset(pData, 0x00, sizeof(stCommonInfo.sManagement) + 1);
			memcpy(pData, &stCommonInfo.sManagement, sizeof(stCommonInfo.sManagement));
			nData = atoi(pData);
			delete[] pData;
			
			  if (pstFilterInfo->nDirectionChk[nIndex] == 1)
			  {
			  if (nData == 1)
			  return FALSE;
			  else
			  {
			  strTemp.Format("Index %d, Data : %d", nIndex_Chk, nData);
			  SetWriteDebug("Check Filter OK [Direction]", strTemp);
			  }
			  }
			  }
			  break;
			  case 1 :		//# 이상급등
			  {
			  nIndex = 1;
			  pData = new char[sizeof(stCommonInfo.sControl) + 1];
			  memset(pData, 0x00, sizeof(stCommonInfo.sControl) + 1);
			  memcpy(pData, &stCommonInfo.sControl, sizeof(stCommonInfo.sControl));
			  nData = atoi(pData);
			  delete[] pData;
			  
				if (pstFilterInfo->nDirectionChk[nIndex] == 1)
				{
				if (nData == 1)
				return FALSE;
				else
				{
				strTemp.Format("Index %d, Data : %d", nIndex_Chk, nData);
				SetWriteDebug("Check Filter OK [Direction]", strTemp);
				}
				}
				}
				break;
				case 2 :		//# 거래정지
				{
				nIndex = 2;
				pData = new char[sizeof(stCommonInfo.sTradeStop) + 1];
				memset(pData, 0x00, sizeof(stCommonInfo.sTradeStop) + 1);
				memcpy(pData, &stCommonInfo.sTradeStop, sizeof(stCommonInfo.sTradeStop));
				nData = atoi(pData);
				delete[] pData;
				
				  if (pstFilterInfo->nDirectionChk[nIndex] == 1)
				  {
				  if (nData == 1)
				  return FALSE;
				  else
				  {
				  strTemp.Format("Index %d, Data : %d", nIndex_Chk, nData);
				  SetWriteDebug("Check Filter OK [Direction]", strTemp);
				  }
				  }
				  }
				  break;
				  case 3 :					//# 투자유의
				  {
				  nIndex = 3;
				  pData = new char[sizeof(stCommonInfo.sInvestAttention) + 1];
				  memset(pData, 0x00, sizeof(stCommonInfo.sInvestAttention) + 1);
				  memcpy(pData, &stCommonInfo.sInvestAttention, sizeof(stCommonInfo.sInvestAttention));
				  nData = atoi(pData);
				  delete[] pData;
				  
					if (pstFilterInfo->nDirectionChk[nIndex] == 1)
					{
					if (nData == 1)
					return FALSE;
					else
					{
					strTemp.Format("Index %d, Data : %d", nIndex_Chk, nData);
					SetWriteDebug("Check Filter OK [Direction]", strTemp);
					}
					}
					}
					break;
					case 4:		//# 불성실공시
					{
					nIndex = 4;
					pData = new char[sizeof(stCommonInfo.sInsincerityNotice) + 1];
					memset(pData, 0x00, sizeof(stCommonInfo.sInsincerityNotice) + 1);
					memcpy(pData, &stCommonInfo.sInsincerityNotice, sizeof(stCommonInfo.sInsincerityNotice));
					nData = atoi(pData);
					delete[] pData;
					
					  if (pstFilterInfo->nDirectionChk[nIndex] == 1)
					  {
					  if (nData == 1)
					  return FALSE;
					  else
					  {
					  strTemp.Format("Index %d, Data : %d", nIndex_Chk, nData);
					  SetWriteDebug("Check Filter OK [Direction]", strTemp);
					  }
					  }
					  }
					  break;
					  case 5:		//# 정리매매
					  {
					  nIndex = 5;
					  pData = new char[sizeof(stCommonInfo.sJungriMeme) + 1];
					  memset(pData, 0x00, sizeof(stCommonInfo.sJungriMeme) + 1);
					  memcpy(pData, &stCommonInfo.sJungriMeme, sizeof(stCommonInfo.sJungriMeme));
					  nData = atoi(pData);
					  delete[] pData;
					  
						if (pstFilterInfo->nDirectionChk[nIndex] == 1)
						{
						if (nData == 1)
						return FALSE;
						else
						{
						strTemp.Format("Index %d, Data : %d", nIndex_Chk, nData);
						SetWriteDebug("Check Filter OK [Direction]", strTemp);
						}
						}
						}
						break;
						case 6:		//# 증거금 100%
						{
						nIndex = 6;
						pData = new char[sizeof(stCommonInfo.sDepositMoney) + 1];
						memset(pData, 0x00, sizeof(stCommonInfo.sDepositMoney) + 1);
						memcpy(pData, &stCommonInfo.sDepositMoney, sizeof(stCommonInfo.sDepositMoney));
						nData = atoi(pData);
						delete[] pData;
						
						  if (pstFilterInfo->nDirectionChk[nIndex] == 1)
						  {
						  if (nData == 100) 
						  return FALSE;
						  else
						  {
						  strTemp.Format("Index %d, Data : %d", nIndex_Chk, nData);
						  SetWriteDebug("Check Filter OK [Direction]", strTemp);
						  }
						  }
						  }
						  break;
						  case 7:		//# 신용제한 종목
						  {
						  nIndex = 7;
						  pData = new char[sizeof(stCommonInfo.sConfidenceLimit) + 1];
						  memset(pData, 0x00, sizeof(stCommonInfo.sConfidenceLimit) + 1);
						  memcpy(pData, &stCommonInfo.sConfidenceLimit, sizeof(stCommonInfo.sConfidenceLimit));
						  nData = atoi(pData);
						  delete[] pData;
						  
							if (pstFilterInfo->nDirectionChk[nIndex] == 1)
							{
							if (nData == 1)
							return FALSE;
							else
							{
							strTemp.Format("Index %d, Data : %d", nIndex_Chk, nData);
							SetWriteDebug("Check Filter OK [Direction]", strTemp);
							}
							}
							}
							break;
							default:
							{
							
							  }
							  break;
							  }
							  }
							  }
*/

return TRUE;
}


void CMainFrmWnd::SetResult_Info(TR_RESULT_HEADER stTR_RESULT_HEADER)
{
	char *pBuff;
	
	pBuff = new char[sizeof(stTR_RESULT_HEADER.sJongCnt) + 1];
	memset(pBuff, 0x00, sizeof(stTR_RESULT_HEADER.sJongCnt) + 1);
	memcpy(pBuff, &stTR_RESULT_HEADER.sJongCnt, sizeof(stTR_RESULT_HEADER.sJongCnt));
	m_ctrlJongCnt.SetWindowText((CString)pBuff);
	delete[] pBuff;
	
	pBuff = new char[sizeof(stTR_RESULT_HEADER.sHigh) + 1];
	memset(pBuff, 0x00, sizeof(stTR_RESULT_HEADER.sHigh) + 1);
	memcpy(pBuff, &stTR_RESULT_HEADER.sHigh, sizeof(stTR_RESULT_HEADER.sHigh));
	m_ctrlMax.SetWindowText((CString)pBuff);
	delete[] pBuff;
	
	pBuff = new char[sizeof(stTR_RESULT_HEADER.sUp) + 1];
	memset(pBuff, 0x00, sizeof(stTR_RESULT_HEADER.sUp) + 1);
	memcpy(pBuff, &stTR_RESULT_HEADER.sUp, sizeof(stTR_RESULT_HEADER.sUp));
	m_ctrlUp.SetWindowText((CString)pBuff);
	delete[] pBuff;
	
	pBuff = new char[sizeof(stTR_RESULT_HEADER.sNoch) + 1];
	memset(pBuff, 0x00, sizeof(stTR_RESULT_HEADER.sNoch) + 1);
	memcpy(pBuff, &stTR_RESULT_HEADER.sNoch, sizeof(stTR_RESULT_HEADER.sNoch));
	m_ctrlSteady.SetWindowText((CString)pBuff);
	delete[] pBuff;
	
	pBuff = new char[sizeof(stTR_RESULT_HEADER.sDown) + 1];
	memset(pBuff, 0x00, sizeof(stTR_RESULT_HEADER.sDown) + 1);
	memcpy(pBuff, &stTR_RESULT_HEADER.sDown, sizeof(stTR_RESULT_HEADER.sDown));
	m_ctrlDown.SetWindowText((CString)pBuff);
	delete[] pBuff;
	
	pBuff = new char[sizeof(stTR_RESULT_HEADER.sLow) + 1];
	memset(pBuff, 0x00, sizeof(stTR_RESULT_HEADER.sLow) + 1);
	memcpy(pBuff, &stTR_RESULT_HEADER.sLow, sizeof(stTR_RESULT_HEADER.sLow));
	m_ctrlMin.SetWindowText((CString)pBuff);
	delete[] pBuff;
	
	pBuff = new char[sizeof(stTR_RESULT_HEADER.sUpRate) + 1];
	memset(pBuff, 0x00, sizeof(stTR_RESULT_HEADER.sUpRate) + 1);
	memcpy(pBuff, &stTR_RESULT_HEADER.sUpRate, sizeof(stTR_RESULT_HEADER.sUpRate));
	m_ctrlUpRate.SetWindowText("상승비중:" + (CString)pBuff);
	delete[] pBuff;
	
	pBuff = new char[sizeof(stTR_RESULT_HEADER.sChgrateAvg) + 1];
	memset(pBuff, 0x00, sizeof(stTR_RESULT_HEADER.sChgrateAvg) + 1);
	memcpy(pBuff, &stTR_RESULT_HEADER.sChgrateAvg, sizeof(stTR_RESULT_HEADER.sChgrateAvg));
	m_ctrlAvg.SetWindowText("평균등락률:" + (CString)pBuff);
	delete[] pBuff;
}

void CMainFrmWnd::SetResult_Clear()
{
	m_ctrlJongCnt.SetWindowText("0");
	m_ctrlMax.SetWindowText("0");
	m_ctrlUp.SetWindowText("0");
	m_ctrlSteady.SetWindowText("0");
	m_ctrlDown.SetWindowText("0");
	m_ctrlMin.SetWindowText("0");
	m_ctrlUpRate.SetWindowText("상승비중: 0.00");
	m_ctrlAvg.SetWindowText("평균등락률: 0.00");
	
	m_gridResult.ClearAll();
	/*m_gridResult.RedrawAll();*/
}

void CMainFrmWnd::Modual_SendReal(BOOL bReg, CString strKey)
{
// 	if(!bReg)
// 		return;
	
	CString strRealType;
	if(m_hSignalMng)
	{
		strRealType.Format("%d", (int)TYPE_SIGNAL_ALERT);
		SignalLib_SetSignal(bReg, this, strRealType, "", m_strAlertKey);
	}
}

long CMainFrmWnd::OnGetBroad(WPARAM wParam, LPARAM lParam)
{
	ST_REALDATA* pstRealData = (ST_REALDATA*)lParam;

	SIGNAL_CONDITION *pstSIGNAL_CONDITION = (SIGNAL_CONDITION*)pstRealData->pDataBuf;

	SIGNAL_CONDITION stSIGNAL_CONDITION;
	memcpy(&stSIGNAL_CONDITION, pstSIGNAL_CONDITION, SZ_SIGNAL_CONDITION);

	// 2009/02/22 -hidden-	비정상 실시간 데이터 수신시 실시간 해제하고 리턴
	CString strCode;
	strCode.Format("%6.6s", stSIGNAL_CONDITION.sCode);
	if(0 == atoi(strCode))
	{
		OnBtnRegReal();

		CString strErrMsg = "실시간 데이터 ";

		if(stSIGNAL_CONDITION.sJobFlag[0] == 'W')
			strErrMsg += "조건 점검";
		else if(stSIGNAL_CONDITION.sJobFlag[0] == 'I')
			strErrMsg += "조건 초기화";
		else if(stSIGNAL_CONDITION.sJobFlag[0] == 'K')
			strErrMsg += "조건 강제종료";
		else
			strErrMsg += "조건 이상";

		SetSendMainMsg(strErrMsg);

		return 0;
	}

	BOOL bState = FALSE;
	if (m_btnKospiChk.GetCheck() == TRUE)
	{
		if (stSIGNAL_CONDITION.stCommon.sMarket[0] == '1')
		{
			bState = TRUE;
		}
	}
	
	if (m_btnKosdakChk.GetCheck() == TRUE)
	{
		if (stSIGNAL_CONDITION.stCommon.sMarket[0] == '2')
		{
			bState = TRUE;
		}
	}
	if (bState == FALSE)
	{
		SendToEugeneMain(&stSIGNAL_CONDITION);
		return 0;
	}

	BOOL bJobChk = TRUE;
	if (m_strGroupName == (CString)TAB_TITLE_USERCONDI)
		bJobChk = FALSE;

	//# Filter
	BOOL bAdd = TRUE;   
	if (m_btnFilterChk.GetCheck() == TRUE)
		bAdd = GetCheck_Filter(&m_stFilterInfo, stSIGNAL_CONDITION.stCommon);

	if (bAdd == TRUE)
	{
		int ntotCol = m_listField.GetCount();
		m_gridResult.SetAddRow_Real_DynamicField(stSIGNAL_CONDITION, bJobChk, FALSE, ntotCol);
	}
	
	SendToEugeneMain(&stSIGNAL_CONDITION);

	return 0;
}

#define SIGNAL_JONGMOKSIG		30	//		종목시그널
#define SIGNAL_CONDSEARCH		31	//		조건검색
#define SIGNAL_CONDSYSTEM		32	//		시스템트레이딩

BOOL CMainFrmWnd::SendToEugeneMain(SIGNAL_CONDITION *pstSIGNAL_CONDITION)
{

	ALARM_CONDSEARCH_INFO stSignal;
	memset(&stSignal, 0x00, sizeof(ALARM_CONDSEARCH_INFO));
	
	switch (m_nIndustry)
	{
		case 0 :
			memcpy(stSignal.szScrNo, "0362", 4);
			break;

		case 1 :
			memcpy(stSignal.szScrNo, "0363", 4);
			break;

		case 2 :
			memcpy(stSignal.szScrNo, "0360", 4);
			break;
	}

	char buf[8];
	memset(buf, 0x00, sizeof(buf));
	memcpy(buf, pstSIGNAL_CONDITION->sSignalDegree, sizeof(pstSIGNAL_CONDITION->sSignalDegree));
	
	int nPoint = atoi(buf);
	
	CString strType = "0";
	if(nPoint > 0)
		strType = "2";
	else if(nPoint < 0)
		strType = "1";

	memcpy(stSignal.szMemegb, strType, strType.GetLength());
	memcpy(stSignal.szCode, pstSIGNAL_CONDITION->sCode, min(sizeof(stSignal.szCode), sizeof(pstSIGNAL_CONDITION->sCode)));

	
// 	IMainInfoManager*	pMainInfo = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager, 1);
// 	CWnd* pWnd = NULL;
// 	if(pMainInfo)
// 		pWnd = (CWnd*)pMainInfo->GetMainHandle();
	

// 	if(pWnd)
// 		pWnd->SendMessage(WMU_SIGNAL_CONDSEARCH, 1, (LPARAM)&stSignal);	
	

	return TRUE;
}

BOOL CMainFrmWnd::ConvertForEugeneSignalManager(SIGNAL_CONDITION *pstSIGNAL_CONDITION, EUGENE_SIGNALMSGINFO *pstEUGENE_SIGNALMSGINFO)
{	
	char* pBuff;
	CString strData;

	memcpy(pstEUGENE_SIGNALMSGINFO->m_sgType,		_T("조"), 2); 
	memcpy(pstEUGENE_SIGNALMSGINFO->m_sgTime,		pstSIGNAL_CONDITION->sTime, sizeof(pstSIGNAL_CONDITION->sTime));

	strData.Format("%26s", " ");
	memcpy(pstEUGENE_SIGNALMSGINFO->m_sgnskey,	strData,		strData.GetLength());
	memcpy(pstEUGENE_SIGNALMSGINFO->m_sgSeqno,	pstSIGNAL_CONDITION->sSignalKey, sizeof(pstSIGNAL_CONDITION->sSignalKey));//일련번호
	memcpy(pstEUGENE_SIGNALMSGINFO->m_sgCodeGb,	pstSIGNAL_CONDITION->stCommon.sMarket, 1);
	memcpy(pstEUGENE_SIGNALMSGINFO->m_sgCode,	pstSIGNAL_CONDITION->sCode, sizeof(pstSIGNAL_CONDITION->sCode));
	//# 신규:'N', 재진입:'R', 탈퇴:'O'
	CString strContext;


	STR_CONVERT(pBuff, pstSIGNAL_CONDITION->sName, sizeof(pstSIGNAL_CONDITION->sName), strData);
	strContext =  strData + " : ";

	STR_CONVERT(pBuff, pstSIGNAL_CONDITION->sSignalName, sizeof(pstSIGNAL_CONDITION->sSignalName), strData);
	strContext = strContext + "!" + strData;strData;

	

	STR_CONVERT(pBuff, pstSIGNAL_CONDITION->sJobFlag, 1, strData);
	CString strType;
	if(strData == "N")
		strType = "신규";
	else if(strData == "R")
		strType = "재진입";
	else if(strData == "O")
		strType = "탈퇴";
		
	strContext =  strContext + "!" + strType;
	
	memcpy(pstEUGENE_SIGNALMSGINFO->m_sgContext,	strContext, strContext.GetLength());
	

	int nPoint;
	INT_CONVERT(pBuff, pstSIGNAL_CONDITION->sSignalDegree, sizeof(pstSIGNAL_CONDITION->sSignalDegree), nPoint);

	if(nPoint > 0)
		memcpy(pstEUGENE_SIGNALMSGINFO->m_sgGubun,	"2",	1);
	else if(nPoint < 0)
		memcpy(pstEUGENE_SIGNALMSGINFO->m_sgGubun,	"3",	1);
	else
		memcpy(pstEUGENE_SIGNALMSGINFO->m_sgGubun,	"1",	1);
	memcpy(pstEUGENE_SIGNALMSGINFO->m_sgGubun,	"1",	1);
/*	memcpy(stEUGENE_SIGNALMSGINFO->m_sgBlink,	stSIGNAL_CONDITION
	memcpy(stEUGENE_SIGNALMSGINFO->m_sgContinue, stSIGNAL_CONDITION
	memcpy(stEUGENE_SIGNALMSGINFO->m_sgPriority, stSIGNAL_CONDITION
	memcpy(stEUGENE_SIGNALMSGINFO->m_sgIcon,		stSIGNAL_CONDITION
	memcpy(stEUGENE_SIGNALMSGINFO->m_sgBugaInfo, stSIGNAL_CONDITION
*/	
	return TRUE;

}
HRESULT CMainFrmWnd::ReceiveData_RealAlert(SIGNAL_CONDITION stSIGNAL_CONDITION)
{
	return 0;
}

BOOL CMainFrmWnd::GetCheck_Filter(FilterInfo *pstFilterInfo, SIGNAL_COMMON stCommon)
{
	CString strData, strTemp;
	if (pstFilterInfo == NULL)
		return FALSE;
	
	char *pData;
	int nIndex, nData;
	BOOL bState;
	
	pData = new char[sizeof(stCommon.sChgRate) + 1];
	memset(pData, 0x00, sizeof(stCommon.sChgRate) + 1);
	/*memcpy(pData, &stCommon.sChgRate, sizeof(stCommon.sChgRate));*/
	memcpy(pData, &stCommon.sMarket, sizeof(stCommon.sMarket));
	nData = atoi(pData);
	delete[] pData;
	
	bState = FALSE;
	if (m_btnKospiChk.GetCheck() == TRUE)
	{
		if (nData == 1)
			bState = TRUE;
	}
	
	if (m_btnKosdakChk.GetCheck() == TRUE)
	{
		if (nData == 2)
			bState = TRUE;
	}
	if (bState == FALSE)
		return FALSE;
	
	//# 제외 종목
	bState = TRUE;
	CString strCode;
	STR_CONVERT(pData, stCommon.sCode, sizeof(stCommon.sCode), strCode)
		for (nIndex = 0; nIndex < pstFilterInfo->saOutCode.GetSize(); nIndex++)
		{
			strData = pstFilterInfo->saOutCode.GetAt(nIndex);
			if (strCode == strData.Left((int)CODE_LEN))
			{
				bState = FALSE;
				break;
			}
		}
		if (bState == FALSE)
			return FALSE;
		
		
		if (pstFilterInfo->nMarketType > 0)			//# 0 : 전체, 1 : 거래소, 2 : 코스닥
		{
			INT_CONVERT(pData, stCommon.sMarket, sizeof(stCommon.sMarket), nData)
				
				if (pstFilterInfo->nMarketType != nData)
					return FALSE;
		}
		
		if (pstFilterInfo->nNowPriceChk == 1)
		{
			INT_CONVERT(pData, stCommon.sPrice, sizeof(stCommon.sPrice), nData)
				if ((pstFilterInfo->nNowPriceUp > nData) ||
					(pstFilterInfo->nNowPriceDown < nData))
					return FALSE;
		}
		
		if (pstFilterInfo->nChangRateChk == 1)
		{
			pData = new char[sizeof(stCommon.sChgRate) + 1];
			memset(pData, 0x00, sizeof(stCommon.sChgRate) + 1);
			memcpy(pData, &stCommon.sChgRate, sizeof(stCommon.sChgRate));
			nData = (int)(atof(pData) * 100);
			delete[] pData;
			
			pData = new char[sizeof(stCommon.sSign) + 1];
			memset(pData, 0x00, sizeof(stCommon.sSign) + 1);
			memcpy(pData, &stCommon.sSign, sizeof(stCommon.sSign));
			int nSign = atoi(pData);
			delete[] pData;
			
			if ((nSign == 4) || (nSign == 5))
				nData = nData * -1;
			
			if ((pstFilterInfo->nChangRateUp > nData) ||
				(pstFilterInfo->nChangRateDown < nData))
				return FALSE;
		}
		
		pData = new char[sizeof(stCommon.sSign) + 1];
		memset(pData, 0x00, sizeof(stCommon.sSign) + 1);
		memcpy(pData, &stCommon.sSign, sizeof(stCommon.sSign));
		nData = atoi(pData);
		delete[] pData;
		
		if ((nData < 1) && (nData > 5))
			return FALSE;
		
		for (nIndex = 0; nIndex < (int)CNT_CHANGEVAL; nIndex++)
		{
			bState = FALSE;
			//# '1' : 상한, '2' : 상승, '3' : 보합, '4' : 하한, '5' : 하락
			if ((pstFilterInfo->nChangValChk[nIndex] == 1) && (nData == nIndex + 1))
			{
				bState = TRUE;
				break;
			}
		}
		
		if (bState == FALSE)
			return FALSE;
		
		int nMaxVal, nMax_Time = 0;
		POSITION pos;
		SubFilterInfo stSubFilterInfo;
		pData = new char[sizeof(stCommon.sVolume) + 1];
		memset(pData, 0x00, sizeof(stCommon.sVolume) + 1);
		memcpy(pData, &stCommon.sVolume, sizeof(stCommon.sVolume));
		nData = atoi(pData);
		delete[] pData;
		
		int nDealVol;
		if (pstFilterInfo->nDealVolType == 0)		//# 0 : 고정, 1 : Time
		{
			nDealVol = pstFilterInfo->nDealVol * 1000;
			if (nDealVol > nData)
				return FALSE;
		}
		else if (pstFilterInfo->nDealVolType == 1)		//# 0 : 고정, 1 : Time
		{
			bState = TRUE;
			
			CDateTimeCtrl ctrlDateTime;
			CTime timeDest;
			/*ctrlDateTime.GetTime(timeDest);*/
			timeDest = CTime::GetCurrentTime();
			
			strTemp = timeDest.Format("%H");
			CString strMin = timeDest.Format("%M");
			if(_ttoi(strMin) > 0)
			{
				strTemp.Format("%d", _ttoi(strTemp) + 1);
			}
			//strTemp.Format("%h%m%s", timeDest);
			int nTime = atoi(strTemp);
			
			for (nIndex = 0; nIndex < pstFilterInfo->plistDealTimeVol->GetCount(); nIndex++)
			{
				pos = pstFilterInfo->plistDealTimeVol->FindIndex(nIndex);
				stSubFilterInfo = pstFilterInfo->plistDealTimeVol->GetAt(pos);
				
				/*nDealVol = pstFilterInfo->nDealVol * 1000;*/
				nDealVol = stSubFilterInfo.nData * 1000;
				/*if ((stSubFilterInfo.nTime == nTime) && (nDealVol <= nData))*/
				if (stSubFilterInfo.nTime >= nTime)
				{
					if(nDealVol <= nData)
					{
						bState = TRUE;
					}
					else
						return FALSE;
				}
				
				if (nMax_Time < stSubFilterInfo.nTime)
				{
					nMax_Time = stSubFilterInfo.nTime;
					nMaxVal = nDealVol;
				}
			}
			
			if ((bState == FALSE)/* && (nMaxVal > nData)*/)
				return FALSE;
		}
		
		/*
		pData = new char[sizeof(stCommon.sAmount) + 1];
		memset(pData, 0x00, sizeof(stCommon.sAmount) + 1);
		memcpy(pData, &stCommon.sAmount, sizeof(stCommon.sAmount));
		nData = atoi(pData);
		delete[] pData;
		
		  int nDealPrice;
		  if (pstFilterInfo->nDealPriceType == 0)		//# 0 : 고정, 1 : Time
		  {
		  nDealPrice = pstFilterInfo->nDealPrice * 100;
		  if (nDealPrice > nData)
		  return FALSE;
		  }
		  else if (pstFilterInfo->nDealPriceType == 1)		//# 0 : 고정, 1 : Time
		  {
		  bState = FALSE;
		  for (nIndex = 0; nIndex < pstFilterInfo->plistDealTimePrice->GetCount(); nIndex++)
		  {
		  pos = pstFilterInfo->plistDealTimePrice->FindIndex(nIndex);
		  stSubFilterInfo = pstFilterInfo->plistDealTimePrice->GetAt(pos);
		  
			nDealPrice = pstFilterInfo->nDealPrice * 100;
			if ((stSubFilterInfo.nTime == nTime) && (nDealPrice > nData))
			{
			bState = TRUE;
			break;
			}
			
			  if (nMax_Time < stSubFilterInfo.nTime)
			  {
			  nMax_Time = stSubFilterInfo.nTime;
			  nMaxVal = nDealPrice;
			  }
			  }
			  
				if ((bState == FALSE) && (nMaxVal > nData))
				return FALSE;
				}
				if (m_bTrace_Filter == TRUE)
				{
				strTemp.Format("%d", nData);
				SetWriteDebug("Check Filter OK [Amount]", strTemp);
				}
		*/
		
		return TRUE;
}

int CMainFrmWnd::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	return ::MessageBox(m_hWnd, lpszText, lpszCaption, nType);
}

void CMainFrmWnd::ChangeGridColor(ST_GRIDCOLOR stGridColor)
{
	m_gridResult.ChangeGridColor(stGridColor);
	m_gridResult.Invalidate();
}

LRESULT CMainFrmWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	
	return CFrameWnd::WindowProc(message, wParam, lParam);
}

int CMainFrmWnd::InMakeTr_P0622(BYTE *pTrData, BYTE* pTrOutData)
{
	return 0;
}

void CMainFrmWnd::InMakeFid_P0622(LPCSTR szTrNo, int nTrLen, BYTE *pTrData, char* szSaveBuf, int &nPos)
{
	//1. 연속패킷, DRFN 연속키값 쓸필요있나???, 그련 space(80)
	memset(&szSaveBuf[nPos], 0x20, 80);
	nPos += 80;

	//2. TR값 문자열로 만든다.
	// 이미 TR데이터를 가공해서 가져왔다.
	//int nSendTrDataSize = InMakeTr_P0622(pTrData, (BYTE*)&inAll);
	int nSendTrDataSize = nTrLen;
	memcpy(&szSaveBuf[nPos], pTrData, nSendTrDataSize);
	nPos += nSendTrDataSize;

	//3. 레코드 번호 1부터 차례로 
	CString szOutRecord;

	// 1st Recor
	szOutRecord.Format("%-16d", 1);
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	GetFillString(szOutRecord, 0, 8, 3);	// 0 ~ 8
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	memcpy(&szSaveBuf[nPos], "|", 1);	//구분자.
	nPos++;

	// 2st Record
	szOutRecord.Format("%-16d", 2);
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	GetFillString(szOutRecord, 0, 9, 3);	// 0 ~ 9
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	memcpy(&szSaveBuf[nPos], ";", 1);	//구분자.
	nPos++;
}

void CMainFrmWnd::InMakeFid_P0825(LPCSTR szTrNo, int nTrLen, BYTE *pTrData, char* szSaveBuf, int &nPos)
{
	//1. 연속패킷, DRFN 연속키값 쓸필요있나???, 그련 space(80)
	memset(&szSaveBuf[nPos], 0x20, 80);
	nPos += 80;

	//2. TR값 문자열로 만든다.
	// 이미 TR데이터를 가공해서 가져왔다.
	//int nSendTrDataSize = InMakeTr_P0622(pTrData, (BYTE*)&inAll);
	int nSendTrDataSize = nTrLen;
	memcpy(&szSaveBuf[nPos], pTrData, nSendTrDataSize);
	nPos += nSendTrDataSize;

	//3. 레코드 번호 1부터 차례로 
	CString szOutRecord;

	// 1st Record
	szOutRecord.Format("%-16d", 1);
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	GetFillString(szOutRecord, 0, 5, 3);	// 0 ~ 8
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	memcpy(&szSaveBuf[nPos], "|", 1);	//구분자.
	nPos++;
}

int CMainFrmWnd::GetFillString(CString& _szGetStr, int nStart, int nEnd, int nOneWordSize)
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

// p0627
void CMainFrmWnd::InMakeFid_Mstcode(LPCSTR szTrNo, int nTrLen, BYTE *pTrData, char* szSaveBuf, int &nPos)
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



BOOL CMainFrmWnd::CheckSavedMstcode()
{
	// 기존에 저장된 데이터가 있는지 확인한다.
	// ::CTrCommSite::ReceiveData에서 저장을 했을 경우 다시 읽어온다.
	//#include "../../inc/IMainExInfoMng.h" 추가해야한다.
	long  rValue = NULL;
	int   rLen = 0;
	
	
// 	IMainExInfoManager3* pMainExMng = (IMainExInfoManager3*)AfxGetPctrInterface(UUID_IMainExInfoManager3, 1);
// 	if(pMainExMng)
// 	{
// 		BOOL bSaved = (BOOL)pMainExMng->GetUserData("MSTCODE", rValue, rLen);
// 		if(!bSaved) return FALSE;
// 
// 		LPCSTR szTR = TRNO_MSTCODE;
// 		LPCSTR szMsgCode="00000";
// 		LPCSTR szMessage="조회완료";
// 
// 		ReceiveData(szTR, szMsgCode, szMessage, (BYTE*)rValue, rLen);
// 		return TRUE;
// 	}
	
	
	return FALSE;
}

void CMainFrmWnd::ChangeSkinColor(COLORREF clrBkgnd, ST_TABCOLOR stTabColor)
{
	if(!clrBkgnd)
		return;

	m_stTabColor = stTabColor;
	m_clrBkgnd = clrBkgnd;
	
	m_clrMainSkin = clrBkgnd;
	m_clrTab	  = stTabColor.clrBACK;

	if(m_brushBK.DeleteObject())
		m_brushBK.CreateSolidBrush(m_clrMainSkin);
	if(m_brushTab.DeleteObject())
		m_brushTab.CreateSolidBrush(m_clrMainSkin);

	m_ptabGroup->SetTabColor(m_clrMainSkin, m_clrMainSkin, stTabColor.clrB, NULL, NULL, m_clrMainSkin);
	m_ptabGroup->SetTextColor(stTabColor.clrSF, stTabColor.clrSF);

	ST_GRIDCOLOR stGridColor;
	stGridColor.clrGridHeader = GetAxColor(74);
	stGridColor.clrGridHilight = GetAxColor(78);
	
	ChangeGridColor(stGridColor);

	if(m_nIndustry == 2)
		m_ctrlConditionText.SetBackgroundColor(FALSE, m_clrMainSkin);

	m_comboSearchTime.Invalidate();
	m_ctrlTitle.Invalidate();
	m_btnKospiChk.Invalidate();
	m_btnKosdakChk.Invalidate();
 	m_ctrlUpRate.Invalidate();
 	m_btnDelOutItem.Invalidate();
 	m_ctrlAvg.Invalidate();
	m_ctrlUpRate1.Invalidate();
	m_ctrlAvg1.Invalidate();

	Invalidate();
}

//유진 컨트롤 관련
void CMainFrmWnd::EuBtnFilterSet()
{
	OnBtnFilterSet();
}

void CMainFrmWnd::EuBtnSearch()
{
	OnBtnSearch();
}

void CMainFrmWnd::EuBtnSend()
{
	OnBtnSend();
}

void CMainFrmWnd::EuBtnReal()
{
	OnBtnRegReal();
}

// 2008.09.24 -hidden- 추가
void CMainFrmWnd::EuBtnDescription()	
{
	OnBtnShowHelp();
}

void CMainFrmWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
//	SetCommMenu(&point);

	CFrameWnd::OnRButtonUp(nFlags, point);
}

extern void KSLink_SetFormLinkTagData( CString sTag, CString sValue );
//extern void KSLink_SetLinkTagData( LPCTSTR sTag, LPCTSTR sValue);
void CMainFrmWnd::SetCommMenu(POINT *point)
{
	HWND hChildFrame = m_hChildFrame;
	if(!hChildFrame) return;

/* hidden
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

long CMainFrmWnd::OnChangeChildFrameWindowHandle( WPARAM wParam, LPARAM lParam)
{
	m_hChildFrame = (HWND)lParam;
	return 0L;
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

void CMainFrmWnd::GetData_RealSearchCondition()
{
	//# User Condition Clear!
//	POSITION pos;
	int nIndex, nIndex_Group, nIndex_Item, nFindPos;
	CString strGroupName, strCindiData, strInfo[5];
	CList <ConditionInfo, ConditionInfo&> *plistConditionInfo;

/* 2009.10.26 -hidden-
	for (pos = m_mapCondition.GetStartPosition(); pos != NULL;)
	{
		m_mapCondition.GetNextAssoc(pos, strGroupName, (void*&)plistConditionInfo);
		
		if (strGroupName != (CString)TAB_TITLE_REALSEARCH)
		{
			if (plistConditionInfo != NULL)
			{
				plistConditionInfo->RemoveAll();
				delete plistConditionInfo;
				plistConditionInfo = NULL;
			}
			
			m_mapCondition.RemoveKey(strGroupName);
		}
	}
*/	
	//# 사용자 
	nIndex_Group = m_saGroupName.GetSize();
	strGroupName = (CString)TAB_TITLE_REALSEARCH;
	CStringArray saGroupName, saCondiData;
	
// 	BOOL bIndustry;
// 	if(1 == m_nIndustry)
// 		bIndustry = TRUE;

	BOOL bIndustry = (m_nIndustry == 1) ? TRUE : FALSE;
	
	if (Lib_GetConditionData(3, bIndustry, FALSE, saGroupName, saCondiData) == TRUE)
	{
		plistConditionInfo = new CList <ConditionInfo, ConditionInfo&>;
		for (nIndex_Item = 0; nIndex_Item < saCondiData.GetSize(); nIndex_Item++)
		{
			strCindiData = saCondiData.GetAt(nIndex_Item);
			
			nIndex = 0;
			//#	[형식] : Group View Name | File View Name | Condition File Path | Alert State | 설명 |
			while (strCindiData.IsEmpty() == FALSE)
			{
				nFindPos = strCindiData.Find("|", 0);
				if (nFindPos > -1)
				{
					strInfo[nIndex] = strCindiData.Left(nFindPos);
					strCindiData = strCindiData.Mid(nFindPos + 1, strCindiData.GetLength() - nFindPos);
					
					nIndex++;
				}
				else
					strCindiData = "";
			}
			
			ConditionInfo	stConditionInfo;
			
			stConditionInfo.nGroupID = IDC_BTN_GROUP + nIndex_Group;
			stConditionInfo.strGroupName = strGroupName;
			
			//strInfo[1].Replace("_", " ");
			stConditionInfo.strName = strInfo[1];
			stConditionInfo.strPath = strInfo[2];
			stConditionInfo.strAlertKey = strInfo[3];
			stConditionInfo.strExplain = strInfo[4];
			
			stConditionInfo.bLoad = FALSE;
			stConditionInfo.nOrderCnt = 0;
			memset(&stConditionInfo.stUser_Finder, 0x00, SZ_User_Finder);
			plistConditionInfo->AddTail(stConditionInfo);

			CString strRoot;
			strRoot = Variant(homeCC);
			stConditionInfo.strPath = strRoot + stConditionInfo.strPath;
			
			m_stRealConditionInfo = stConditionInfo;
			m_stRealConditionInfo.strGroupName = _T("사용자검색");
		}
		
		m_mapCondition.SetAt(strGroupName, plistConditionInfo);
	}
	
	saGroupName.RemoveAll();
	saCondiData.RemoveAll();
}

void CMainFrmWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CString strBtnReg;
	m_btnRegReal.GetWindowText(strBtnReg);
	
	if(strcmp(strBtnReg, "해제") == 0)
	{
		CWnd* pCurWnd = ChildWindowFromPoint(point);
		if(m_ptabGroup->m_hWnd == pCurWnd->m_hWnd)
		{
			int nItemWidth = 0;
			CRect rcTab, rcItem, rcGrid;
			m_ptabGroup->GetWindowRect(rcTab);

			for(int i = 0; i < m_ptabGroup->GetItemCount(); ++i)
			{
				m_ptabGroup->GetItemRect(i, rcItem);
				nItemWidth += rcItem.Width();
			}

			rcTab.right = rcTab.left + nItemWidth;
			rcTab.bottom = rcTab.top + rcItem.Height();
			ScreenToClient(rcTab);

			if(rcTab.PtInRect(point))
				SetMessageBox("감시중입니다. 조회는 해제후에 가능합니다");

			m_gridList.GetWindowRect(rcGrid);
			ScreenToClient(rcGrid);
			
			if(rcGrid.PtInRect(point))
				SetMessageBox("감시중입니다. 조회는 해제후에 가능합니다");
			
		}
	}

	CFrameWnd::OnLButtonDown(nFlags, point);
}

void CMainFrmWnd::OnCbnSelChangeUserList()
{
	m_gridList.SetSelectRow(m_ctrlUserList.GetCurSel());
}

BOOL CMainFrmWnd::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
// 	if(!m_pDebugMng || !pCopyDataStruct)
// 		return FALSE;
// 
// 	CString strTr;
// 	STIDM2Info_TranData *pData = (STIDM2Info_TranData*)pCopyDataStruct->lpData;
// 
// 	int nSize = (int)(pCopyDataStruct->cbData);
// 
// 	if(strstr(pData->aDataKey2, "piso0351") >= 0)
// 		strTr = TRNO_CONDIONECLK_RESULT;
// 	else if(strstr(pData->aDataKey2, "so035020") >= 0)
// 		strTr = TRNO_MSTCODE;
// 	else if(strstr(pData->aDataKey2, "so035110") >= 0)
// 		strTr = TRNO_ALERT_REALREG;
// 
// 	if (strstr(pData->aDataKey2, TRNO_CONDIONECLK_RESULT) >= 0)
// 	{
// 		ReceiveData(strTr, "", "", (LPSTR)(LPCTSTR)pData->pData, nSize-(sizeof(pData)-1));
// 
// 	}

	return CWnd::OnCopyData(pWnd, pCopyDataStruct);
}

void CMainFrmWnd::SetFieldEditApply()
{
	SetSaveFixUserFieldData(&m_listFixHeadUser, m_strFixFieldVersion);

	SetGrid_Head();
	SetGrid_Head(TRUE);
	OnBtnSearch();
}


void CMainFrmWnd::OnBtnFieldEdit()
{
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	if (m_pdlgFieldEdit == NULL)
		m_pdlgFieldEdit = new CFieldEditDlg(this, 0);
	else
		m_pdlgFieldEdit->m_nMoveFieldCnt = 0;
	
	
	if (m_pdlgFieldEdit->DoModal() == IDOK)
	{
		
	}
	
	AfxSetResourceHandle(hInstSave);
}


BOOL CMainFrmWnd::SetSaveFixUserFieldData(CList<HeadFixInfo, HeadFixInfo&> *plistFixUser,
										  CString strVersion)
{
	BOOL bResult = TRUE;

	CString strRoot;
	strRoot = Variant(homeCC);
	CString strFileRoot = strRoot + USRDIR + Variant(nameCC) + FOLDER_FINDDATA + "\\"; 

	switch (m_nIndustry)
	{
		case 0 : strFileRoot += FIXFILED_USER_ONE_ITEM;		break;
		case 1 : strFileRoot += FIXFILED_USER_ONE_MAKRET;	break;
		case 2 : strFileRoot += FIXFILED_USER_ONE_REAL;		break;

	}

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

		pDOMIndex->save(_bstr_t(strFileRoot));
	}	
	catch(_com_error e)
	{
		bResult = FALSE;
	}

	return bResult;
}

BOOL CMainFrmWnd::GetLoadFixUserFieldData(MSXML::IXMLDOMNodePtr pNIndex,
										  CList<HeadFixInfo, HeadFixInfo&> &listFixList)
{
	BOOL bResult = TRUE;

	MSXML::IXMLDOMNamedNodeMapPtr	pNAIndex;

	MSXML::IXMLDOMNodeListPtr		pNS_FItem;		// Item List
	MSXML::IXMLDOMNodePtr			pN_FItem;		// Item
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
		//SetSendMainMsg(strMsg);
		bResult = FALSE;
	}

	return bResult;
}


void CMainFrmWnd::SetLoad_FieldInfo()
{
	MSXML::IXMLDOMDocumentPtr pXmlDom ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);

	CString strRoot;
	strRoot =  Variant(homeCC);
	
	CString strFileRoot = strRoot + FOLDER_DATA + "\\" + FIXFIELD_SYS_FILE;

	
	if (GetLoadXml(strFileRoot, pXmlDom) == TRUE)
	{
		GetLoadFixSysFieldData(pXmlDom->selectSingleNode("INDEX"), m_listFixHeadSys);
		m_strFixFieldVersion = GetVersion_FieldFile(pXmlDom->selectSingleNode("INDEX"));
	}

	BOOL bSetInit = TRUE;
	strFileRoot = strRoot + USRDIR + Variant(nameCC) + FOLDER_FINDDATA + "\\";

	switch (m_nIndustry)
	{
		case 0 : strFileRoot += FIXFILED_USER_ONE_ITEM;		break;
		case 1 : strFileRoot += FIXFILED_USER_ONE_MAKRET;	break;
		case 2 : strFileRoot += FIXFILED_USER_ONE_REAL;		break;
	}

	if (GetLoadXml(strFileRoot, pXmlDom) == TRUE)
	{
		//# Fix Field User Version Check!
		if (m_strFixFieldVersion == GetVersion_FieldFile(pXmlDom->selectSingleNode("INDEX")))
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
		}
	}

	SetSaveFixUserFieldData(&m_listFixHeadUser, m_strFixFieldVersion);
}

BOOL CMainFrmWnd::GetLoadXml(CString strFileRoot, MSXML::IXMLDOMDocumentPtr &pXmlDom)
{
	char msg[500];
	CString strDebug;

	pXmlDom->async = false;
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

void CMainFrmWnd::SetGrid_Head(BOOL bView /*= FALSE*/)
{
	int nCol;
	POSITION pos;

	if (bView == TRUE)
	{
		if(m_nIndustry != 1)
		{
			m_gridResult.SetHeader(&m_listField, "RESULT");
			m_gridResult.m_plistField = &m_listField;
		}
		else
		{
			m_gridResult.HeaderSetup("INDUSTRY");
		}
		
		if(m_strGroupName != (CString)TAB_TITLE_CONTRACT && m_nFixCol > 2)
			m_gridResult.SetLockColumns(m_nFixCol + 1, TRUE);
		else
			m_gridResult.SetLockColumns(m_nFixCol, TRUE);
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
		}
	}
}

BOOL CMainFrmWnd::GetLoadFixSysFieldData(MSXML::IXMLDOMNodePtr pNIndex,
										 CList<HeadInfoType, HeadInfoType&> &listFixList)
{
	BOOL bResult = TRUE;

	MSXML::IXMLDOMNodeListPtr		pNS_FItem;	// Item List
	MSXML::IXMLDOMNodePtr			pN_FItem;	// Item
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
			CString szIndex;
			szIndex.Format("%d", nCount);
			m_mapResultFieldIndex.SetAt(stHeadInfoType.strHeadName, szIndex);
		}
	}
	catch(_com_error e)
	{
		CString strMsg = "시스템 파일을 읽어오는 중 에러가 발생하였습니다. 고객지원센터로 연락 바랍니다.";
		//SetSendMainMsg(strMsg);
		bResult = FALSE;
	}

	return bResult;
}

CString CMainFrmWnd::GetVersion_FieldFile(MSXML::IXMLDOMNodePtr pNIndex)
{
	CString strResult = XML_VERSION_FIXFIELDFILE;

	MSXML::IXMLDOMNamedNodeMapPtr	pNAIndex;
	try 
	{
		MSXML::IXMLDOMNodePtr	pNAttributeTemp;
		pNAIndex = pNIndex->attributes;

		pNAttributeTemp = pNAIndex->getNamedItem("VERSION");
		strResult = CString((PCHAR)pNAttributeTemp->text);
	}
	catch(_com_error e) 
	{
		strResult = "";
	}

	return strResult;
}

void CMainFrmWnd::SetUpdateUserFieldData(CList<HeadInfoType, HeadInfoType&> *plistFixSys, 
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

BOOL CMainFrmWnd::GetLoadFixField_Def(CStringArray &saFieldId)
{
	CFileFind cFind;
	BOOL bRead = TRUE;

	CString strFilePath = GetFilePath("FINDERINFO");

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

int CMainFrmWnd::GetCount_FixHead(BOOL &bHaveCodeFiled, CUIntArray &iaFixHeadId)
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

void CMainFrmWnd::GetTabClr(COLORREF* clrBkgnd, ST_TABCOLOR* stTabColor)
{
	*clrBkgnd = m_clrBkgnd;
	*stTabColor = m_stTabColor;
}

CString CMainFrmWnd::GetFilePath(CString strLoadType)
{
	CString strFile, strFolder;
	CString strFilePath = "";

	BOOL bUserFolder = FALSE;
	BOOL bSetIndustry = TRUE;

	strFile = (CString)FINDERINFO_INIFILE;

	if (bUserFolder == TRUE)
	{
		CString strUserPath;
		strUserPath.Format("%s%s%s", Variant(homeCC), USRDIR, Variant(nameCC));
		strFilePath = (CString)strUserPath + (CString)FOLDER_FINDDATA + "\\" + strFile;
	}
	else
	{
		CString strAppPath;
		strAppPath = Variant(homeCC);
		
		strFilePath = strAppPath + (CString)FOLDER_DATA + "\\" + strFile;
	}

	return strFilePath;
}

#include <WinDef.h>

#define		SGNL_IDX_NEWS					0				// 뉴스
#define		SGNL_IDX_JM						1				// 종목시그널
#define		SGNL_IDX_CONDITION				2				// 조건검색
#define		SGNL_IDX_SISE					3				// 시세감시
#define		SGNL_IDX_CHART					4				// 차트

#define		SGNL_NO_SEP						0				// 구분없음
#define		SGNL_SELL						1				// 매도
#define     SGNL_BUY						2				// 매수
LRESULT CMainFrmWnd::OnNotifySignal(WPARAM wParam, LPARAM lParam)
{	
	// 신호종류
	int nSgnlKind = LOWORD(wParam);
	int nDealType = HIWORD(wParam);
	//데이타
	
	CString	sItemCode((LPCSTR)lParam);
	CString strSignal;

	// 신호 Data 수신 처리
	BOOL bSgnlDsp = FALSE;

	char szTempData[41] = {0, };
	CString strJMCode = "";

	COLORREF clrBK;
	COLORREF clrText = RGB(255, 255, 255);

	switch (nSgnlKind)
	{
		case SGNL_IDX_NEWS		:		// 뉴스 신호
			if(nDealType == SGNL_NO_SEP)
			{
				clrBK = RGB(62, 169, 33);			
			}
			else if(nDealType == SGNL_SELL)
			{
				clrBK = RGB(244, 49, 39);
			}
			else if(nDealType == SGNL_BUY)
			{
				clrBK = RGB(20, 124, 221);
			}
			strSignal = "뉴";
			
			break;	

		case SGNL_IDX_JM		:		// 종목 신호
			if(nDealType == SGNL_NO_SEP)
			{
				clrBK = RGB(62,169,33);			
			}
			else if(nDealType == SGNL_SELL)
			{
				clrBK = RGB(244, 49 ,39);
			}
			else if(nDealType == SGNL_BUY)
			{
				clrBK = RGB(20, 124, 221);
			}
			strSignal = "공";
			break;	

		case SGNL_IDX_CONDITION	:		// 조건검색 신호	
			clrBK = RGB(184, 97, 229);
			strSignal = "검";
			break;	

		case SGNL_IDX_SISE		:		// 시세감시
			if(nDealType == SGNL_SELL)
			{
				clrBK = RGB(244, 49, 39);
			}
			else if(nDealType == SGNL_BUY)
			{
				clrBK = RGB(20, 124, 221);
			}
			strSignal = "시";
			break;	

		case SGNL_IDX_CHART		:		// 차트 신호
			clrBK = RGB(184, 97, 229);
			strSignal = "트";
			break;	

		default					:
			return FALSE;
	}
	
	m_gridResult.SetSignalAtGrid(sItemCode, strSignal, clrBK, clrText);

	return 0L;
}

BOOL CMainFrmWnd::GetRealFlag()
{
	return m_bRealRegFlag;
}
BOOL CMainFrmWnd::OnEraseBkgnd(CDC* pDC)
{	
	CRect reClient;
	GetClientRect(reClient);

	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	pDC->FillSolidRect(reClient, pScreenWnd->GetAxColor(66));

	m_gridResult.Invalidate();

	return FALSE;
}

// 20110328 박찬모 R-Mouse Menu를 통한 종목 연동 기능 제공 >>
CString CMainFrmWnd::GetSelectGridItemCode()
{
	CString strItemCode;
	int nMarkType;

	m_gridResult.GetRowToCode(m_gridResult.m_nLastSelRow, strItemCode, nMarkType);
	
	return strItemCode;
}
// 20110328 박찬모 R-Mouse Menu를 통한 종목 연동 기능 제공 << CWnd*
//tour2k cwnd* -> HWND로 변경
LRESULT CMainFrmWnd::OnGetModuleMainWnd(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)m_pModuleMainWnd->GetSafeHwnd();
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

COLORREF CMainFrmWnd::GetColor(int nColor)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();

	return pScreenWnd->GetAxColor(nColor);
}

CString CMainFrmWnd::Variant(int nComm, CString strData)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();

	return pScreenWnd->Variant(nComm, strData);
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

COLORREF CMainFrmWnd::GetAxColor(int nColor)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetAxColor(nColor);
}

HBRUSH CMainFrmWnd::GetAxBrush(COLORREF clr)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return (HBRUSH)(*pScreenWnd->GetAxBrush(clr));
}

BOOL CMainFrmWnd::GetStockMasterData(CStringArray &arrCode, CStringArray &arrName, UINT Jgub, UINT Upgub)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetStockMasterData(arrCode, arrName, Jgub, Upgub);
}

BOOL CMainFrmWnd::GetUpMasterData(CStringArray *&psaCode, CStringArray *&psaName, int jgub)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetUpMasterData(psaCode, psaName, jgub);
}

CString CMainFrmWnd::GetMarketNameToCode(CString strMaketCode)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetMarketNameToCode(strMaketCode);
}

CPen* CMainFrmWnd::GetAxPen(COLORREF clr, int nWidth, int nStyle)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetAxPen(clr, nWidth, nStyle);
}

void CMainFrmWnd::Request_PortGroup()
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();

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

}

void CMainFrmWnd::GetData_PortGroup(CStringArray& saKey, CStringArray& saGroupName)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
#if(0)//tour2k
	pScreenWnd->GetData_PortGroup(saKey, saGroupName);
#else
	saKey.Copy(m_GwanSimGroup.strKey);
	saGroupName.Copy(m_GwanSimGroup.strGroupName);
#endif
}

#define	szDoGridWidthSetSectKey "UserGridWidth"
void CMainFrmWnd::_DoGridWidthSet(int nOption)
{
	CUGCtrl*pGridCtrl = (CUGCtrl*)&m_gridResult;
	CMapStringToString UserColWidthsMap;
	LPCSTR szSectKey = szDoGridWidthSetSectKey;
	CString			   szUserMapFile;
	
	CString szScreenCFG;
	szScreenCFG.Format("%s.cfg", GetScreenKey());
	szUserMapFile.Format("%s%s%s", Variant(homeCC), USRDIR, Variant(nameCC));
	
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

int CMainFrmWnd::SetResultSearchTemp()
{
	CString strCode, strCodeName; 
	CStringArray strArrayCode, strArrayCodeName;
	
	int nDataCnt = m_gridResult.GetNumberRows();
	if(nDataCnt>0)
	{
		int nRow;
		int nMarkType;
		for (nRow = 0; nRow < m_gridResult.GetNumberRows(); nRow++)
		{
			m_gridResult.GetRowCode(nRow, strCode, nMarkType);
			strCodeName = m_gridResult.GetCellData(nRow, 0);
			
			strArrayCode.Add(strCode);
			strArrayCodeName.Add(strCodeName);
		}
		
		CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
		pScreenWnd->SetUserMaster(usrMasterCondition, strArrayCode.GetSize(), &strArrayCode, &strArrayCodeName);
	}
	return 0;
}

COLORREF CMainFrmWnd::GetMainBackColor()
{
	return m_clrMainSkin;
}