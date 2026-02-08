// DlgStrategyConfigure.cpp : implementation file
//

#include "stdafx.h"
#include "DlgStrategyConfigure.h"
#include "StrategyConfigureWizardDef.h"
#include "../../inc/IMainInfoMng.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgStrategyConfigure dialog
#define WIDTHSPLITTER	(3)
#define HEIGHTTITLE		(30)
#define OUTLINEGAP		(1)
#define CRBACKGROUND	RGB(177,199,234)
#define CRSELECTEDTAB	RGB(211,223,241)

// #define WMU_GET_AUP_SIZE	WM_USER + 2243			// Aup의 기본 Size를 View에 요청한다.
const UINT RMSG_GETDLLSIZE = ::RegisterWindowMessage("RMSG_GETDLLSIZE");
const UINT RMSG_SETDLLSIZE = ::RegisterWindowMessage("RMSG_SETDLLSIZE");

#define	GET_LASTFOLDER_NAME(strDir, p_LastFolder)\
{\
	CString	_strRoot_ = strDir;\
	int _loc_ = _strRoot_.ReverseFind('\\');\
	if( _loc_ > 0)\
	p_LastFolder = _strRoot_.Mid(_loc_+1);\
}

CDlgStrategyConfigure::CDlgStrategyConfigure(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStrategyConfigure::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgStrategyConfigure)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nWidthOfOutBar = 158;
	m_pSTRunMng = NULL;
	m_pIAUDataMonitorManager = NULL;
	m_dwIAUDataMonitorManager = NULL;
}

void CDlgStrategyConfigure::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStrategyConfigure)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStrategyConfigure, CDialog)
	//{{AFX_MSG_MAP(CDlgStrategyConfigure)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(RMSG_GETDLLSIZE, OnGetDllSize)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgStrategyConfigure message handlers
void CDlgStrategyConfigure::FormLoad()
{
	Resize(0, 0);
	m_dlgStrategyList.ResizeAll();
	m_dlgOutLookBar.Resize();

}

void CDlgStrategyConfigure::DataMonitorCallbackProc(LPCSTR szKey, long pThisPoint, long dwKey, long dwSubKey, long dwNotifyData)
{
	CDlgStrategyConfigure* pThis = (CDlgStrategyConfigure*)pThisPoint;
	CString strKey = szKey;

	if(strKey == "STLIST_MODIFIED") 
	{
		LPCSTR szSTName = (LPCSTR)dwNotifyData;
		pThis->UpdateSTList(dwSubKey, szSTName);
	}
}

void CDlgStrategyConfigure::InitLoad()
{
	int nID = UUID_IAUDataMonitorManager; 
	m_pIAUDataMonitorManager = (IAUDataMonitorManager*)AfxGetPctrInterface(nID);
	CString szKey = "STLIST_MODIFIED"; 
	m_dwIAUDataMonitorManager = m_pIAUDataMonitorManager->Advise(szKey, (long)this, (long)NULL, DataMonitorCallbackProc);
}

BOOL CDlgStrategyConfigure::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitLoad();
	BOOL bSel = IsSTSelected();
	BOOL bRetValue = m_dlgStrategyList.Create(CDlgStrategyList::IDD,this);
	m_dlgStrategyList.MoveWindow(GetRect(IDD_DLG_STLIST));
	m_dlgStrategyList.ShowWindow(SW_SHOW);
	m_dlgOutLookBar.SetSTParent(this);
	m_dlgOutLookBar.Create(COutLookBarDlg::IDD, this);

	if(bSel) {
		STMNG_ITEMINFO* pItemInfo = (STMNG_ITEMINFO*)m_pSTRunMng->GetSelectedSTInfo();
		if(pItemInfo) {
			//m_dlgOutLookBar.m_pSTLoadMnger->SetSelectedItem(pItemInfo->nFolderIndx, pItemInfo->nItemIndx);
			m_dlgOutLookBar.m_pSTLoadMnger->SetSelectedItem(pItemInfo);

			CString szFolderPath;
			szFolderPath = pItemInfo->szSTPath;
			szFolderPath += "\\";
			szFolderPath += pItemInfo->szSTName;

			STLISTITEMDATA	dataItem;
			dataItem.szSTPath = szFolderPath;
			dataItem.bUserFolder = pItemInfo->nSTType;
			m_dlgStrategyList.SendMessage(UWM_ADDCOMPOUND, (WPARAM)&dataItem);

			if(m_dlgStrategyList.GetGridSTRowCount() > 0) {
				m_dlgStrategyList.OnClickGridSTList(1, 1);
				m_dlgStrategyList.m_GridSTList.SetSelectedRange(1,0,1,4);
			}
		}
	}

	m_dlgOutLookBar.ShowWindow(SW_SHOW);
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgStrategyConfigure::SelectSTItem(CString szItemName, CString szSelSTPath)
{
	CString szSTName = szItemName;
	int nFind = szSTName.Find(gSTGubunKeys);
	if(nFind>0) szSTName = szSTName.Left(nFind);

	szSTName.TrimRight();
	szSTName.MakeUpper();

	if(szSTName.GetLength()==0) 	
	{
		m_dlgOutLookBar.m_wndOutBar.SetSelFolder(0);
		return;
	}

	CString szSTRData;
	GET_LASTFOLDER_NAME(szSelSTPath, szSTRData);

	STMNG_ITEMINFO InData;
	m_dlgOutLookBar.m_pSTLoadMnger->GetSelectedItem(InData);
	//if(hItem)
	{
		//if(!szSTRData.Compare("사용자단위전략") || !szSTRData.Compare("사용자조합전략"))
		//	m_dlgOutLookBar.m_RunSTInfo.nSTType = 1;
		//else
		//	m_dlgOutLookBar.m_RunSTInfo.nSTType = 0;
		//m_dlgOutLookBar.m_RunSTInfo.szSTName = szItemName;
		//m_dlgOutLookBar.m_RunSTInfo.szSTPath = szSelSTPath;
		//m_dlgOutLookBar.m_RunSTInfo.szSTRData = szSTRData;

		m_dlgOutLookBar.m_pSTLoadMnger->SetSelectedItem(&InData);
	}
}

BOOL CDlgStrategyConfigure::IsSTSelected()
{
	// 선택된 전략이 있는지 체크한다.
	// -1 이면 선택되지 않은 상태.
	if(m_pSTRunMng)
	{
		STMNG_ITEMINFO* pItemInfo = (STMNG_ITEMINFO*)m_pSTRunMng->GetSelectedSTInfo();
		if(pItemInfo)
		{
			int nFolderIndx	= pItemInfo->nFolderIndx;
			int nItemIndx	= pItemInfo->nItemIndx;	

			if(nFolderIndx>=0 && nItemIndx>=0)
				return TRUE;
		}
	}
	return FALSE;
}

CRect CDlgStrategyConfigure::GetRect(int nId)
{
	CRect rect;
	GetClientRect(rect);
	switch(nId)
	{
	case IDD_OUTLOOKBARDLG:
		rect.right = m_nWidthOfOutBar;
		break;
	case IDD_DLG_STLIST:
		rect.left = m_nWidthOfOutBar;
		break;
	default:
		break;
	}
	return rect;
}

void CDlgStrategyConfigure::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	Resize(cx, cy);
}

void CDlgStrategyConfigure::Resize(int cx, int cy)
{
	if(cx==0 || cy==0) return;

// 	if(m_dlgOutLookBar.GetSafeHwnd())	
// 		m_dlgOutLookBar.MoveWindow(GetRect(m_dlgOutLookBar.IDD),TRUE);
// 	if(m_dlgStrategyList.GetSafeHwnd())
// 		m_dlgStrategyList.MoveWindow(GetRect(m_dlgStrategyList.IDD),TRUE);
	if(m_dlgOutLookBar.GetSafeHwnd())	
	{
		CRect rc;
		rc = GetRect(m_dlgOutLookBar.IDD);
		rc.bottom = rc.top + cy;
		m_dlgOutLookBar.MoveWindow(rc,TRUE);
	}
	if(m_dlgStrategyList.GetSafeHwnd())
	{
		CRect rc;
		rc = GetRect(m_dlgStrategyList.IDD);
		rc.bottom = rc.top + cy;
		m_dlgStrategyList.MoveWindow(rc,TRUE);
	}
}

void CDlgStrategyConfigure::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if(m_dwIAUDataMonitorManager)
		m_pIAUDataMonitorManager->UnAdvise(m_dwIAUDataMonitorManager);

	m_dlgOutLookBar.DestroyWindow();
	m_dlgStrategyList.DestroyWindow();
}

void CDlgStrategyConfigure::UpdateSTList(int nState, CString szSTPath)
{
	m_dlgOutLookBar.UpdateSTList(nState, szSTPath);
}

void CDlgStrategyConfigure::LoadEditor(CString szScriptName)
{
	CopyBaseScriptFile();

	CWnd *pFrameWnd = GetParentFrame();

	CString szSystemTradingPath;
#ifdef _DEBUG
	//@Solomon szSystemTradingPath = m_strRootPath + "\\Chart\\SystemTrading.exe";
	szSystemTradingPath = m_strRootPath + "\\dev\\SystemTrading.exe";
#else
	//@Solomon szSystemTradingPath = m_strRootPath + "\\Chart\\SystemTrading.exe";
	szSystemTradingPath = m_strRootPath + "\\dev\\SystemTrading.exe";
#endif

	CWnd* pWndCompiler = FindWindow(NULL, _T("Solomon SystemTrading"));
	COPYDATASTRUCT copyData;
	if(pWndCompiler)
	{
		
		copyData.dwData = 1000;
		copyData.cbData = ::lstrlen(szScriptName) + sizeof(DWORD);
		copyData.lpData = (void*)(LPCSTR)szScriptName;
		
		pWndCompiler->SendMessage(WM_COPYDATA, 0,(LPARAM)&copyData);

		pWndCompiler->SetForegroundWindow();
	}
	else
 	{
		HWND h_pctr = NULL;
		h_pctr = AfxGetPctrMainHandle();

		//@Solomon LPCSTR szBaseDir = "Data\\STData";
		LPCSTR szBaseDir = "Icss\\STData";
		char szConfigFile[MAX_PATH] = {NULL, };
		char szIP[16] = {NULL, };
		::wsprintf(szConfigFile, _T("%s\\%s\\STConfig.cfg"), m_strRootPath, szBaseDir);

		CString szScriptPath;
		if(szScriptName.IsEmpty())
			szScriptPath.Format(_T("/U\"%s\""), m_strUserDir);
		else
			szScriptPath.Format(_T("/U\"%s\" /S\"%s\""), m_strUserDir, szScriptName);

		CString szOption;
		szOption.Format("%s|%08X",szScriptPath,h_pctr);

		::ShellExecute(NULL, _T("open"), szSystemTradingPath, szOption, NULL, SW_SHOWMAXIMIZED);

//		AfxMessageBox(szSystemTradingPath);

//		IMainInfoManager* pMainMng = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
//
//		//접속 분류 코드 ★ ( 10 : 차세대 Client, 11 : 차세대 HTS)
//		CString szConnClssCode = pMainMng->GetMainData("ISHTS", "");
//		if(szConnClssCode.Compare("10") == 0)
//		{
//			::GetPrivateProfileString(_T("CONNECT"), _T("BPIP"), _T("10.204.10.101"), szIP, sizeof(szIP), szConfigFile);
//		}
//		else
//		{
//			::GetPrivateProfileString(_T("CONNECT"), _T("HTSIP"), _T("211.255.203.238"), szIP, sizeof(szIP), szConfigFile);
//		}
//		::WritePrivateProfileString(_T("CONNECT"), _T("IP"), szIP, szConfigFile);
		
	}
}

#include <direct.h>
#include <io.h>
short CDlgStrategyConfigure::CopyBaseScriptFile()
{
	{	//Make Directory
		CString szDir(m_strUserDir);
		if (_access(szDir, 0) == -1) ::_mkdir(szDir);

		szDir += "\\STData";
		if (_access(szDir, 0) == -1) ::_mkdir(szDir);

		szDir += "\\ScriptFiles";
		if (_access(szDir, 0) == -1) ::_mkdir(szDir);
	}
	//@Solomon LPCSTR szBaseDir = "Data\\STData";
	LPCSTR szBaseDir = "Icss\\STData";
	LPCSTR szBaseDir2 = "STData\\ScriptFiles";

//	IMainInfoManager* pMng = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
//	m_strUserDir = pMng->GetEncUserDir();

	char szConfigFile[MAX_PATH] = {NULL, };
	::wsprintf(szConfigFile, _T("%s\\%s\\STConfig.cfg"), m_strRootPath, szBaseDir);

	LPCSTR szSecKey = "LIST";

	int nCopyNO = ::GetPrivateProfileInt(szSecKey, "CopyNo", 0, szConfigFile);
	if(nCopyNO==100) return 0;

	int nCnt = ::GetPrivateProfileInt(szSecKey, "Count", 0, szConfigFile);

	char aFileFrom[MAX_PATH]={0,},  aFileTo[MAX_PATH]={0,};
	char aKeyFileName[MAX_PATH]={0,};
	char aKey[5];
	for(int i=0; i<nCnt; i++)
	{
		sprintf(aKey, "%d", i+1);
		if(::GetPrivateProfileString(szSecKey, aKey, "", aKeyFileName, MAX_PATH, szConfigFile)>0)
		{
			//@Solomon sprintf(aFileFrom,	"%s\\Sys\\%s\\high\\%s", m_strRootPath, szBaseDir2, aKeyFileName);
			sprintf(aFileFrom,	"%s\\Icss\\%s\\high\\%s", m_strRootPath, szBaseDir2, aKeyFileName);
			sprintf(aFileTo,	"%s\\%s\\%s", m_strUserDir, szBaseDir2, aKeyFileName);
			CopyFile(aFileFrom, aFileTo, TRUE);
		}
	}
	return nCnt;
}

BOOL CDlgStrategyConfigure::PreTranslateMessage(MSG* pMsg) 
{
	// 엔터가 쳤을때 죽는 현상 방지		JSJ_Modify_040421
	if(pMsg->message == WM_KEYDOWN)   
	{
		if(pMsg->wParam == VK_F4 && (::GetKeyState(VK_CONTROL) & 0x8000))
		{
			pMsg->wParam = VK_ESCAPE;
			return GetParent()->PreTranslateMessage(pMsg);
		}
		else
			return FALSE;
	}
	else if(pMsg->message == WM_SYSKEYDOWN)
	{
		if( ::GetKeyState(VK_MENU) & 0x8000)
			return FALSE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

//화면로딩시 화면크기를 알기위해 호출되므로 화면크기를 넘겨줌.
//LONG CDlgAnalysis::OnGetAupSize(WPARAM wParam, LPARAM lParam)
// 런타임시에 DLL의 사이즈를 변경하고 싶을 경우에는 RMSG_SETDLLSIZE 메시지를 보낸다. 부모(다이얼로그프레임)에게.
LONG CDlgStrategyConfigure::OnGetDllSize(WPARAM wParam, LPARAM lParam)
{
	CPoint* pSize = (CPoint*)wParam;
	pSize->x = 890;
	pSize->y = 384;

	return 0L;
}

#include "../Common_ST/LoadSTDir.h"
int	 CDlgStrategyConfigure::RunST(BOOL bImsiRun, CString &szRunFile)
{
	//if(m_pSTRunMng)
	//{
	//	STMNG_ITEMINFO InData;
	//	if(m_dlgOutLookBar.m_pSTLoadMnger->GetSelectedItem(InData))
	//		return m_pSTRunMng->RunST(bImsiRun, szRunFile, (LONG)&InData);
	//}
	if(m_pSTRunMng)
	{
		STMNG_ITEMINFO_EX1 InData;
		InData.nSTLevel = STLevel_Ext;		// src\Common_ST\LoadSTDir.h
		InData.hTreeItem = NULL;			// 셋팅할 필요없음
		InData.hParentItem = NULL;			// 셋팅할 필요없음.
		InData.bIsItem = TRUE;				// 셋팅할 필요없음.

		CString szSTName(szRunFile);
		szSTName = szSTName.Mid(szSTName.ReverseFind('\\') + 1);
		szSTName = szSTName.Mid(0, szSTName.ReverseFind('.'));
		InData.szSTName = szSTName;

		CString szFolderName(szRunFile);
		szFolderName = szFolderName.Mid(0, szFolderName.ReverseFind('\\'));
		InData.szSTPath = szFolderName;
		GET_LASTFOLDER_NAME(szFolderName, InData.szSTRData);

		int nSTType = -1;
		if((nSTType = szSTName.Find(gSTGubunKey)) != -1)
			szSTName = szSTName.Left(nSTType);

		// 최종 선택된 전략 정보 셋팅
		if(m_dlgOutLookBar.m_pSTLoadMnger->GetRunSTItemInfo(szSTName, InData))
			return m_pSTRunMng->RunST(bImsiRun, szRunFile, (LONG)&InData);
	}	
	return -1;	// Interface가 셋팅되지 않음. CDlgStrategyConfigure 생성시 세팅할 것.
}

LONG CDlgStrategyConfigure::GetOutBarColor(COLORREF& clrOLBkgrnd, COLORREF& clrOLOutLine)
{
	if(m_pSTRunMng)
	{
		m_pSTRunMng->GetOutBarColor(clrOLBkgrnd, clrOLOutLine);
		return 1L;
	}

	return 0L;
}