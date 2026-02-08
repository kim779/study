// Lib_SignalMng.cpp : Defines the initialization routines for the DLL.
//

/****************************************************************/
/*                                                              */
/*						 신호 메니저 (SignalMng.dll)									    */
/*																															*/
/*                   2005. 12.  		Doori												*/
/*                   작성자 : 우동우(Danny)										  */
/****************************************************************/

#include "stdafx.h"
#include "Lib_SignalMng.h"

#include <afxtempl.h>
#include "DummyWnd.h"

#include "fnda.h"
#include "fnds.h"
#include "H0STFND0.h"

#include "../../../SrcSite/include/interSt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PROGRAM_TITLE						"Lib_SignalMng"
//const int VERSION_SETFILE = 1;
const int VERSION_SETFILE = 2;		// 버전올림. 2007-08-30

#define FOLDER_SIGNALSET				""
#define SIGNALDATA_NEWS_FILE		"\\SignalData_News.dat"
#define SIGNALDATA_SYS_FILE			"\\SignalData_Sys.dat"
#define SIGNALDATA_USER_FILE		"\\SignalData_User.dat"

#define SIGNALDATA_NEWS_FILE_DEF	"\\SignalData_News_Def.dat"
#define SIGNALDATA_SYS_FILE_DEF		"\\SignalData_Sys_Def.dat"

#define SIGNALDATA_FILTER_FILE		"\\SignalData_Filter.dat"
#define SIGNALDATA_SOUND_FILE		"\\SignalData_Sound.dat"

#define WM_GET_USERPATH				WM_USER + 350

#define MAX_LINE 100

/////////////////////////////////////////////////////////////////////////////
// CLib_SignalMngApp
BEGIN_MESSAGE_MAP(CLib_SignalMngApp, CWinApp)
	//{{AFX_MSG_MAP(CLib_SignalMngApp)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLib_SignalMngApp construction


CLib_SignalMngApp::CLib_SignalMngApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLib_SignalMngApp object

CLib_SignalMngApp theApp;

#include "../../ForSite/FS_Env.h"
#include "../../../SrcSite/include/axis_msg.h"
BOOL CLib_SignalMngApp::InitInstance() 
{
	InitializeCriticalSection(&m_CriticalSection); 
	
	m_bLibOpen = FALSE;
	m_pwndDummy = NULL;
	
	m_pmapSignalSet_Sys	= NULL;
	m_pmapSignalSet_News = NULL;
	m_pstFilterInfo = NULL;
	
	m_strRegSys_Code = "";
	m_nRegSys_Cnt = 0;
	
	m_strUserId = "";
	GetDir_DrfnRoot(m_strAppPath);
	
	SetLoad_DebugInfo();
	SetWriteDebug("InitInstance", "");

	m_nLoadWndCnt = 0;

	return CWinApp::InitInstance();
}

CString CLib_SignalMngApp::GetAppFileRoot()
{
// 	char buf[255];
// 	GetModuleFileName(NULL, buf, 255); //# 실행되는 위치보기 
// 	
// 	int lastlocationofbackslash = 0;
// 	
// 	int nBufCnt = strlen(buf);
// 	for (int i = 0; i < nBufCnt; i++)
// 	{
// 		if (buf[i] == '\\') 
// 			lastlocationofbackslash = i;
// 	}
// 	
// 	CString strAppPath = _T("");
// 	for (int j = 0; j < lastlocationofbackslash; j++)
// 		strAppPath += buf[j];
// 		/*
// 		for (int nIndex = strAppPath.GetLength() - 1; nIndex > 0; nIndex--)
// 		{
// 		if (strAppPath[nIndex] == '\\') 
// 		{
// 		strAppPath = strAppPath.Mid(0, nIndex);
// 		break;
// 		}
// 		}
// 	*/
// 	
// 	return strAppPath;
	return "";
}

void CLib_SignalMngApp::SetLoad_DebugInfo()
{
	CString strFile;
	strFile = m_strAppPath + "\\Debug_SiganlMng.ini";
	
	CString strTemp;
	char chBuff[BUFSIZ];
	GetPrivateProfileString("DEBUG_SIGNALMNG", "Trace", "F", chBuff, sizeof(chBuff), strFile);
	m_bTrace = ((CString)chBuff == "T")?TRUE:FALSE;
	
	GetPrivateProfileString("DEBUG_SIGNALMNG", "Trace_Filter", "F", chBuff, sizeof(chBuff), strFile);
	m_bTrace_Filter = ((CString)chBuff == "T")?TRUE:FALSE;
	
	GetPrivateProfileString("DEBUG_SIGNALMNG", "Advise_SYS", "T", chBuff, sizeof(chBuff), strFile);
	m_bAdvise_SYS = ((CString)chBuff == "T")?TRUE:FALSE;
	
	GetPrivateProfileString("DEBUG_SIGNALMNG", "Send_DRDS", "T", chBuff, sizeof(chBuff), strFile);
	m_bSend_DRDS = ((CString)chBuff == "T")?TRUE:FALSE;
}

CWnd* _DLL_SetLibOpen(CWnd* pwndMain, HINSTANCE hInstance, CWnd* pWmdModuleWnd)
{
	int nResult = -1;
	
	theApp.SetWriteDebug("_DLL_SetLibOpen", "");

	theApp.m_nLoadWndCnt++;
	
	if(theApp.m_pWndModuleMain)
		return theApp.m_pwndDummy;
	
	if(pWmdModuleWnd)
	{
		CString strRetValue;
		
		char* dta;
		dta = (char*)pWmdModuleWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC), (LPARAM)(LPCTSTR)"");
		
		if((long)dta > 1)
			strRetValue.Format("%s", dta);
				
		theApp.m_strUserPath = theApp.m_strAppPath + "\\user\\" + strRetValue;
	}
	
	nResult = theApp.GetLoad_SignalSet();
	theApp.SetWriteDebug("Open => Signal Set File Load", (nResult == 1)?"1":"0");
	
	if ((pwndMain != NULL) && (theApp.m_bAdvise_SYS == TRUE))
	{
		if (theApp.m_pwndDummy == NULL)
		{
			theApp.m_pwndDummy = new CDummyWnd;
			
			if (theApp.m_pwndDummy->Create(NULL, _T("Dummy Window"), WS_CHILD, CRect(0, 0, 0, 0), pwndMain, NULL) == FALSE)
			theApp.SetWriteDebug("Create Dummy Windows", "실패");

			theApp.m_pWndModuleMain = pWmdModuleWnd;
		}
		
		if(!theApp.m_bLibOpen)
			theApp.SetInit_AdvTable();
		theApp.m_pwndDummy->m_ptheApp = &theApp;
		
		/*theApp.SetAdvData_Sys(TRUE);*/
		/*theApp.SetAdvData_News(TRUE);*/
		
		theApp.m_bLibOpen = TRUE;
		nResult = TRUE;
	}
	
	return theApp.m_pwndDummy;
}

BOOL CLib_SignalMngApp::SetAdvData_News(BOOL bReg)
{
	BOOL bResult = FALSE;
	return bResult;
}

BOOL CLib_SignalMngApp::SetAdvData_Sys(BOOL bReg)
{
	CString strMsg, strKey;
	CStringArray arrKeys;

	m_strRegSys_Code = "";
	m_nRegSys_Cnt = 0;
	
	if (m_pmapSignalSet_Sys != NULL)
	{
		POSITION pos;
		SetSignalData	*pstSetSignalData;
		for (pos = m_pmapSignalSet_Sys->GetStartPosition(); pos != NULL;)
		{
			m_pmapSignalSet_Sys->GetNextAssoc(pos, strKey, (void*&)pstSetSignalData);
			
			if(pstSetSignalData->nState == 0)
				continue;
			
			arrKeys.Add(strKey);
			m_nRegSys_Cnt++;
		}
	}

	char* szDataName = "H0STFBC0";
	char* aFieldName = "*.*";

	if(!m_pWndModuleMain)
		return FALSE;

	char* lpData = new char[5];
	memset(lpData, 0x00, sizeof(lpData));
	lpData = "FNDS";

	char* szSendBuf = new char[L_userTH + sizeof(int) + 1];
	memset(szSendBuf, NULL, sizeof(szSendBuf));
	
	int nPos = 0;
	
	_userTH stHeader;
	memcpy(&stHeader, TRNO_SET_REAL, sizeof(stHeader.trc));
	stHeader.key = TRKEY_SET_REAL;
	stHeader.stat = US_KEY;
	
	memcpy(szSendBuf + nPos, &stHeader, sizeof(_userTH));
	nPos += sizeof(_userTH);
	
	memcpy(szSendBuf + nPos, lpData, sizeof(lpData));
	
	m_pWndModuleMain->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, sizeof(lpData) - 1), (LPARAM)szSendBuf);

	return TRUE;
}

BOOL CLib_SignalMngApp::SetAdvData_Alert(BOOL bReg, CString strKey)
{
// 	if(m_sPBID == _T(""))
// 	{
// 		AfxMessageBox("err!");
// 		return 0;
// 	}
	char* szDataName = (LPTSTR)(LPCTSTR)m_sPBID;
	char* aFieldName = "*.*";

	char* szConnectionErrSvrSubject = (LPTSTR)(LPCTSTR)"H0STFND2";
	char* aConnectionErrFieldName = "*";

	if(!m_pWndModuleMain)
		return FALSE;
	
	char* lpData = new char[5];
	memset(lpData, 0x00, sizeof(lpData));
	lpData = "FNDA";
	
	char* szSendBuf = new char[L_userTH + sizeof(int) + 1];
	memset(szSendBuf, NULL, sizeof(szSendBuf));
	
	int nPos = 0;
	
	_userTH stHeader;
	memcpy(&stHeader, TRNO_SET_REAL, sizeof(stHeader.trc));
	stHeader.key = TRKEY_SET_REAL;
	stHeader.stat = US_KEY;
	
	memcpy(szSendBuf + nPos, &stHeader, sizeof(_userTH));
	nPos += sizeof(_userTH);
	
	memcpy(szSendBuf + nPos, lpData, sizeof(lpData));
	
	m_pWndModuleMain->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, sizeof(lpData) - 1), (LPARAM)szSendBuf);

	return TRUE;
}

BOOL _DLL_GetSiganlTable(CString strType, CStringArray &saSignalTable)
{
	BOOL bResult = FALSE;
	
	if (theApp.m_bLibOpen == FALSE)
		return bResult;
	
	CStringArray *psaData = NULL;
	if (strType == (CString)TYPE_TABLE_SYSTEM)
		psaData = &theApp.m_saSysKeyTable;
	else if (strType == (CString)TYPE_TABLE_NEWS)
		psaData = &theApp.m_saNewsKeyTable;
	else if (strType == (CString)TYPE_TABLE_PORT)
		psaData = &theApp.m_saPortKeyTable;
	
	if (psaData != NULL)
	{
		if (psaData->GetSize() > 0)
		{
			saSignalTable.Copy(*psaData);
			bResult = TRUE;
		}
	}
	
	return bResult;
}

BOOL CLib_SignalMngApp::GetLoad_SignalSet()
{
	BOOL bResult = TRUE;
	BOOL bState;

	CString strFileRoot;
// 	if (m_pmapSignalSet_News == NULL)
// 	{
// 		m_pmapSignalSet_News = new CMapStringToPtr;
// 		m_pmapSignalSet_News->RemoveAll();
// 		
// 		strFileRoot = m_strUserPath + FOLDER_SIGNALSET + SIGNALDATA_NEWS_FILE;
// 		bState = GetLoad_SinalData(strFileRoot, m_pmapSignalSet_News);
// 
// 		if(!bState)
// 		{		
// 			strFileRoot = m_strAppPath + "\\chart\\data\\finddata" + FOLDER_SIGNALSET + SIGNALDATA_NEWS_FILE_DEF;
// 			GetLoad_SinalData(strFileRoot, m_pmapSignalSet_News);
// 		}
// 
// 		SetWriteDebug("GetLoad_SignalSet => News File Load", (bState == TRUE)?"1":"0");
// 		
// 		bResult &= bState;
// 	}
// 	else
// 		bResult &= TRUE;
	
	if (m_pmapSignalSet_Sys == NULL)
	{
		m_pmapSignalSet_Sys	= new CMapStringToPtr;
		m_pmapSignalSet_Sys->RemoveAll();
		
		strFileRoot = m_strUserPath + FOLDER_SIGNALSET + SIGNALDATA_SYS_FILE;
 		bState = GetLoad_SinalData(strFileRoot, m_pmapSignalSet_Sys);

		if(!bState) 
		{
			strFileRoot = m_strAppPath + "\\icss\\finddata" + FOLDER_SIGNALSET + SIGNALDATA_SYS_FILE_DEF;
			GetLoad_SinalData(strFileRoot, m_pmapSignalSet_Sys);
		}

		SetWriteDebug("GetLoad_SignalSet => Sys File Load", (bState == TRUE)?"1":"0");
		
		bResult &= bState;
	}
	else
		bResult &= TRUE;
	
	if (m_pstFilterInfo == NULL)
	{
		m_pstFilterInfo = new FilterInfo;	
		m_pstFilterInfo->psaInCode = new CStringArray;
		m_pstFilterInfo->psaInCode->RemoveAll();
		m_pstFilterInfo->plistDealTimeVol = new CList <SubFilterInfo, SubFilterInfo&>;
		m_pstFilterInfo->plistDealTimeVol->RemoveAll();
		m_pstFilterInfo->plistDealTimePrice = new CList <SubFilterInfo, SubFilterInfo&>;
		m_pstFilterInfo->plistDealTimePrice->RemoveAll();
		
		strFileRoot = m_strUserPath + FOLDER_SIGNALSET + SIGNALDATA_FILTER_FILE;
		bState = GetLoad_FilterData(strFileRoot, m_pstFilterInfo);
		if (bState == FALSE)
			SetDefult_FilterInfo(m_pstFilterInfo);    //# Set Defult!
		SetWriteDebug("GetLoad_SignalSet => Filter File Load", (bState == TRUE)?"1":"0");
		
		bResult &= bState;
	}
	else
		bResult &= TRUE;
	
	return bResult;
}

void CLib_SignalMngApp::SetDefult_FilterInfo(FilterInfo *pstFilterInfo)
{
	if (pstFilterInfo == NULL)
		return;
	
	int nIndex;
	pstFilterInfo->nInCodeType = 0;         //# 0 : Market, 1 : PortPolio 
	pstFilterInfo->strInCodeData = "0";     //# [0 : 전체, 1 : 코스피, 2 : 코스닥] or [PortPolio Name]
	if (pstFilterInfo->psaInCode != NULL)
		pstFilterInfo->psaInCode->RemoveAll();    //# User After Load!
	
	pstFilterInfo->nNowPriceChk = 0;
	pstFilterInfo->nNowPriceUp = 5000; 
	pstFilterInfo->nNowPriceDown = 10000; 
	
	pstFilterInfo->nChangRateChk = 0; 
	pstFilterInfo->nChangRateUp = 0; 
	pstFilterInfo->nChangRateDown = 500; 
	
	for (nIndex = 0; nIndex < (int)CNT_CHANGEVAL; nIndex++)
		pstFilterInfo->nChangValChk[nIndex] = 1;
	
	pstFilterInfo->nDealVolType = 0;		//# 0 : 고정, 1 : Time
	pstFilterInfo->nDealVol = 5;
	if (pstFilterInfo->plistDealTimeVol != NULL)
		pstFilterInfo->plistDealTimeVol->RemoveAll();
	
	pstFilterInfo->nDealPriceType = 0;		//# 0 : 고정, 1 : Time
	pstFilterInfo->nDealPrice = 50;
	if (pstFilterInfo->plistDealTimePrice != NULL)
		pstFilterInfo->plistDealTimePrice->RemoveAll();
	
	for (nIndex = 0; nIndex < (int)CNT_DIRECTION_1; nIndex++)
		pstFilterInfo->nDirectionChk[nIndex] = 0;
	
	pstFilterInfo->nNewsType = 0;   //# 0 : 전체, 1 : 뉴스, 2 : 공시
	
	pstFilterInfo->saOutCode.RemoveAll();
}

BOOL CLib_SignalMngApp::SetSave_SignalSet()
{
	BOOL bResult = TRUE, bState;
	
	CString strFileRoot;
// 	strFileRoot = m_strUserPath + FOLDER_SIGNALSET + SIGNALDATA_NEWS_FILE;
// 	bState = SetSave_SignalData(strFileRoot, m_pmapSignalSet_News);
// 	SetWriteDebug("Save_SignalData NEWS", (bState == TRUE)?"1":"0");
// 	bResult &= bState;
	
	strFileRoot = m_strUserPath + FOLDER_SIGNALSET + SIGNALDATA_SYS_FILE;
	bState = SetSave_SignalData(strFileRoot, m_pmapSignalSet_Sys);
	SetWriteDebug("Save_SignalData SYS", (bState == TRUE)?"1":"0");
	bResult &= bState;
	
	strFileRoot = m_strUserPath + FOLDER_SIGNALSET + SIGNALDATA_FILTER_FILE;
	bState = SetSave_FilterData(strFileRoot, m_pstFilterInfo);
	SetWriteDebug("Save_SignalData Filter", (bState == TRUE)?"1":"0");
	bResult &= bState;
	
	return bResult;
}

void _DLL_SetLibClose(CWnd* pWndTarget)
{
	theApp.SetSave_SignalSet();

	theApp.m_nLoadWndCnt--;
	if(theApp.m_nLoadWndCnt > 0)
		return;
	
	if (theApp.m_bAdvise_SYS == TRUE)
		theApp.SetAdvData_Sys(FALSE);
	theApp.SetAdvData_News(FALSE);
	
	theApp.SetClear_AdvTable(pWndTarget);
	theApp.SetClear_ConditionTable();
	
	if (theApp.m_pwndDummy != NULL)
	{
		theApp.m_pwndDummy->DestroyWindow();
		delete theApp.m_pwndDummy;
		theApp.m_pwndDummy = NULL;
	}
	
	theApp.SetWriteDebug("_DLL_SetLibClose", "");
	theApp.m_pWndModuleMain = NULL;
}

int CLib_SignalMngApp::ExitInstance() 
{
 	DeleteCriticalSection(&m_CriticalSection);
	
	if (m_pmapSignalSet_Sys != NULL)
	{
		SetClear_SignalSet(m_pmapSignalSet_Sys);
		delete m_pmapSignalSet_Sys;
		m_pmapSignalSet_Sys = NULL;
	}
// 	
// 	if (m_pmapSignalSet_News != NULL)
// 	{
// 		SetClear_SignalSet(m_pmapSignalSet_News);
// 		delete m_pmapSignalSet_News;
// 		m_pmapSignalSet_News = NULL;
// 	}
// 
	if (m_pstFilterInfo != NULL)
	{
		if (m_pstFilterInfo->psaInCode != NULL)
		{
			m_pstFilterInfo->psaInCode->RemoveAll();
			delete m_pstFilterInfo->psaInCode;
			m_pstFilterInfo->psaInCode = NULL;
		}
		
		if (m_pstFilterInfo->plistDealTimeVol != NULL)
		{
			m_pstFilterInfo->plistDealTimeVol->RemoveAll();
			delete m_pstFilterInfo->plistDealTimeVol;
			m_pstFilterInfo->plistDealTimeVol = NULL;
		}
		
		if (m_pstFilterInfo->plistDealTimePrice != NULL)
		{
			m_pstFilterInfo->plistDealTimePrice->RemoveAll();
			delete m_pstFilterInfo->plistDealTimePrice;
			m_pstFilterInfo->plistDealTimePrice = NULL;
		}
		
		if (m_pstFilterInfo != NULL)
		{
			delete m_pstFilterInfo;
			m_pstFilterInfo = NULL;
		}
	}

	m_saNewsKeyTable.RemoveAll();
	m_saSysKeyTable.RemoveAll();
	m_saPortKeyTable.RemoveAll();
	
	SetWriteDebug("ExitInstance", "");
	
	return CWinApp::ExitInstance();
}

void CLib_SignalMngApp::SetClear_SignalSet(CMapStringToPtr *pmapSignalSet)
{
	if (pmapSignalSet == NULL)
		return;
	
	POSITION pos;
	CString strKey;
	SetSignalData	*pstSetSignalData;
	for (pos = pmapSignalSet->GetStartPosition(); pos != NULL;)
	{
		pmapSignalSet->GetNextAssoc(pos, strKey, (void*&)pstSetSignalData);
		
		if (pstSetSignalData != NULL)
		{
			delete pstSetSignalData;
			pstSetSignalData = NULL;
		}
		pmapSignalSet->RemoveKey(strKey);
	}
	pmapSignalSet->RemoveAll();
}

void _DLL_GetSignalData(CString strType, CMapStringToPtr &mapSignalSet)
{
	CMapStringToPtr		*pmapSignalSet;
	if (strType == "NEWS")
		pmapSignalSet = theApp.m_pmapSignalSet_News;
	else if (strType == "SYS")
		pmapSignalSet = theApp.m_pmapSignalSet_Sys;
	else if (strType == "USER")
	{
	
	}
	
	theApp.SetClear_SignalSet(&mapSignalSet);
	
	if (pmapSignalSet == NULL)
		return;
	
	POSITION pos;
	CString strKey;
	SetSignalData	*pstSetSignalData, *pstSetSignalData_Target;
	for (pos = pmapSignalSet->GetStartPosition(); pos != NULL;)
	{
		pmapSignalSet->GetNextAssoc(pos, strKey, (void*&)pstSetSignalData);
		
		pstSetSignalData_Target = new SetSignalData;
		SETINIT_SIGNALDATA(pstSetSignalData_Target);
		theApp.SetCopy_SetSignalData(pstSetSignalData, pstSetSignalData_Target); 
		
		mapSignalSet.SetAt(strKey, (void*&)pstSetSignalData_Target);
	}
}

void _DLL_SetSignalData(CString strType, CMapStringToPtr *pmapSignalSet)
{
	CMapStringToPtr *pmapSignalSet_Target = NULL;
	if (strType == "NEWS")
	{
		if (theApp.m_pmapSignalSet_News == NULL)
		{
			theApp.m_pmapSignalSet_News = new CMapStringToPtr;
			theApp.m_pmapSignalSet_News->RemoveAll();
		}
		
		pmapSignalSet_Target = theApp.m_pmapSignalSet_News;
	}
	else if (strType == "SYS")
	{
		if (theApp.m_pmapSignalSet_Sys == NULL)
		{
			theApp.m_pmapSignalSet_Sys = new CMapStringToPtr;
			theApp.m_pmapSignalSet_Sys->RemoveAll();
		}
		
		if (theApp.m_bAdvise_SYS == TRUE)
			theApp.SetAdvData_Sys(FALSE);
		
		pmapSignalSet_Target = theApp.m_pmapSignalSet_Sys;
	}
	else if (strType == "USER")
	{
		
	}
	
	theApp.SetClear_SignalSet(pmapSignalSet_Target);
	
	POSITION pos;
	CString strKey;
	SetSignalData	*pstSetSignalData, *pstSetSignalData_Target;
	for (pos = pmapSignalSet->GetStartPosition(); pos != NULL;)
	{
		pmapSignalSet->GetNextAssoc(pos, strKey, (void*&)pstSetSignalData);
		
		pstSetSignalData_Target = new SetSignalData;
		SETINIT_SIGNALDATA(pstSetSignalData_Target);
		theApp.SetCopy_SetSignalData(pstSetSignalData, pstSetSignalData_Target); 
		
		pmapSignalSet_Target->SetAt(strKey, (void*&)pstSetSignalData_Target);
	}
	
	if ((strType == "SYS") && (theApp.m_bAdvise_SYS == TRUE))
		theApp.SetAdvData_Sys(TRUE);
}

void CLib_SignalMngApp::SetCopy_SetSignalData(SetSignalData *pstSetSignalData, SetSignalData *pstSetSignalData_Target)
{
	pstSetSignalData_Target->nIndex = pstSetSignalData->nIndex;
	pstSetSignalData_Target->strKey = pstSetSignalData->strKey;
	pstSetSignalData_Target->nType = pstSetSignalData->nType;
	
	pstSetSignalData_Target->stKeywordData.nIndex = pstSetSignalData->stKeywordData.nIndex;
	pstSetSignalData_Target->stKeywordData.nType = pstSetSignalData->stKeywordData.nType;
	pstSetSignalData_Target->stKeywordData.strTitle = pstSetSignalData->stKeywordData.strTitle;
	pstSetSignalData_Target->stKeywordData.nBasePoint = pstSetSignalData->stKeywordData.nBasePoint;
	pstSetSignalData_Target->stKeywordData.strCode = pstSetSignalData->stKeywordData.strCode;
	pstSetSignalData_Target->stKeywordData.strCode_S = pstSetSignalData->stKeywordData.strCode_S;
	pstSetSignalData_Target->stKeywordData.strName_S = pstSetSignalData->stKeywordData.strName_S;
	pstSetSignalData_Target->stKeywordData.nPoint_S = pstSetSignalData->stKeywordData.nPoint_S;
	pstSetSignalData_Target->stKeywordData.strCode_V = pstSetSignalData->stKeywordData.strCode_V;
	pstSetSignalData_Target->stKeywordData.strName_V = pstSetSignalData->stKeywordData.strName_V;
	pstSetSignalData_Target->stKeywordData.nPoint_V = pstSetSignalData->stKeywordData.nPoint_V;
	
	pstSetSignalData_Target->stConditionData.nIndex = pstSetSignalData->stConditionData.nIndex;
	pstSetSignalData_Target->stConditionData.strTitle = pstSetSignalData->stConditionData.strTitle;
	pstSetSignalData_Target->stConditionData.nBasePoint = pstSetSignalData->stConditionData.nBasePoint;
	pstSetSignalData_Target->stConditionData.strCode = pstSetSignalData->stConditionData.strCode;
	pstSetSignalData_Target->stConditionData.strPath = pstSetSignalData->stConditionData.strPath;
	
	pstSetSignalData_Target->strTitle = pstSetSignalData->strTitle;
	pstSetSignalData_Target->nPoint = pstSetSignalData->nPoint;
	pstSetSignalData_Target->nSend_Signal = pstSetSignalData->nSend_Signal;
	pstSetSignalData_Target->nSend_Ticker = pstSetSignalData->nSend_Ticker;
	pstSetSignalData_Target->nSend_Order = pstSetSignalData->nSend_Order;
	pstSetSignalData_Target->nState = pstSetSignalData->nState;
}

void CLib_SignalMngApp::SetSerialize(CArchive &ar, int nVersion, SetSignalData	*pstSetSignalData)
{
	if (ar.IsLoading()) 
	{
		ar >> pstSetSignalData->nIndex;
		ar >> pstSetSignalData->nType;
		
		ar >> pstSetSignalData->stKeywordData.nType;		//# 0 : Sys, 1 : User
		ar >> pstSetSignalData->stKeywordData.nIndex;
		ar >> pstSetSignalData->stKeywordData.strTitle;
		ar >> pstSetSignalData->stKeywordData.nBasePoint;
		ar >> pstSetSignalData->stKeywordData.strCode;
		ar >> pstSetSignalData->stKeywordData.strCode_S;
		ar >> pstSetSignalData->stKeywordData.strName_S;
		ar >> pstSetSignalData->stKeywordData.nPoint_S;
		ar >> pstSetSignalData->stKeywordData.strCode_V;
		ar >> pstSetSignalData->stKeywordData.strName_V;
		ar >> pstSetSignalData->stKeywordData.nPoint_V;
		
		ar >> pstSetSignalData->stConditionData.nIndex;
		ar >> pstSetSignalData->stConditionData.strTitle;
		ar >> pstSetSignalData->stConditionData.nBasePoint;
		ar >> pstSetSignalData->stConditionData.strCode;
		ar >> pstSetSignalData->stConditionData.strPath;
		
		ar >> pstSetSignalData->strTitle;
		ar >> pstSetSignalData->nPoint;
		ar >> pstSetSignalData->nSend_Signal;
		ar >> pstSetSignalData->nSend_Ticker;
		ar >> pstSetSignalData->nSend_Order;
		ar >> pstSetSignalData->nState;
	}
	else
	{
		ar << pstSetSignalData->nIndex;
		ar << pstSetSignalData->nType;
		
		ar << pstSetSignalData->stKeywordData.nType;		//# 0 : Sys, 1 : User
		ar << pstSetSignalData->stKeywordData.nIndex;
		ar << pstSetSignalData->stKeywordData.strTitle;
		ar << pstSetSignalData->stKeywordData.nBasePoint;
		ar << pstSetSignalData->stKeywordData.strCode;
		ar << pstSetSignalData->stKeywordData.strCode_S;
		ar << pstSetSignalData->stKeywordData.strName_S;
		ar << pstSetSignalData->stKeywordData.nPoint_S;
		ar << pstSetSignalData->stKeywordData.strCode_V;
		ar << pstSetSignalData->stKeywordData.strName_V;
		ar << pstSetSignalData->stKeywordData.nPoint_V;
		
		ar << pstSetSignalData->stConditionData.nIndex;
		ar << pstSetSignalData->stConditionData.strTitle;
		ar << pstSetSignalData->stConditionData.nBasePoint;
		ar << pstSetSignalData->stConditionData.strCode;
		ar << pstSetSignalData->stConditionData.strPath;
		
		ar << pstSetSignalData->strTitle;
		ar << pstSetSignalData->nPoint;
		ar << pstSetSignalData->nSend_Signal;
		ar << pstSetSignalData->nSend_Ticker;
		ar << pstSetSignalData->nSend_Order;
		ar << pstSetSignalData->nState;
	}
}

CString CLib_SignalMngApp::GetSignalDataToKey(SetSignalData	*pstSetSignalData)
{
	CString strKey = "";
	if (pstSetSignalData == NULL)
		return strKey;
	
	if (pstSetSignalData->nType == 0)		//# Keyword
	{
		if (pstSetSignalData->stKeywordData.nType == 0)
			strKey = pstSetSignalData->stKeywordData.strCode;
		else if (pstSetSignalData->stKeywordData.nType == 1)
			strKey = pstSetSignalData->stKeywordData.strName_S;
	}
	else if (pstSetSignalData->nType == 1)		//# Condition
		strKey = pstSetSignalData->stConditionData.strCode;
	
	return strKey;
}

BOOL CLib_SignalMngApp::GetLoad_SinalData(CString sFullPathName, CMapStringToPtr *pmapSignalSet)
{
	CFile clsFile;
	if (!clsFile.Open(sFullPathName, CFile::typeBinary | CFile::modeRead))
		return FALSE;
	
	if (pmapSignalSet == NULL)
		return FALSE;
	
	LPSTR lpBuff = NULL;
	DWORD nLen = clsFile.GetLength();
	if(nLen == 0) return FALSE;
	
	lpBuff = new char[nLen];
	memset(lpBuff, 0, nLen);
	clsFile.Read(lpBuff, nLen);
	clsFile.Close();
	
	CMemFile memFile;// = CMemFile(nLen);//tour2k
	memFile.Attach((unsigned char*)lpBuff, nLen);
	
	CArchive ar(&memFile, CArchive::load);
	
	int nCount, nVersion;
	ar >> nVersion;
	ar >> nCount;
	CString strKey;
	SetSignalData	*pstSetSignalData = NULL;
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pstSetSignalData = new SetSignalData;
		SetSerialize(ar, nVersion, pstSetSignalData);
		
		strKey = GetSignalDataToKey(pstSetSignalData);
		pstSetSignalData->strKey = strKey;
		
		pmapSignalSet->SetAt(strKey, (void*&)pstSetSignalData);
	}
	
	ar.Close();
	if (lpBuff) 
		delete lpBuff;
	
	return TRUE;
}

BOOL CLib_SignalMngApp::SetSave_SignalData(CString sFullPathName, CMapStringToPtr *pmapSignalSet)
{
	if (pmapSignalSet == NULL)
		return FALSE;
	
	CFile clsFile;
	if (!clsFile.Open(sFullPathName, CFile::typeBinary | CFile::modeCreate | CFile::modeWrite)) 
		return FALSE;
	CArchive ar(&clsFile, CArchive::store);
	
	POSITION pos;
	CString strKey;
	SetSignalData	*pstSetSignalData = NULL;
	ar << VERSION_SETFILE;
	ar << pmapSignalSet->GetCount();
	for (pos = pmapSignalSet->GetStartPosition(); pos != NULL;)
	{
		pmapSignalSet->GetNextAssoc(pos, strKey, (void*&)pstSetSignalData);
		SetSerialize(ar, VERSION_SETFILE, pstSetSignalData);
	}
	
	ar.Close();
	clsFile.Close();
	return TRUE;
}

BOOL CLib_SignalMngApp::GetLoad_FilterData(CString sFullPathName, FilterInfo *pstFilterInfo)
{
	if (pstFilterInfo == NULL)
		return FALSE;
	
	CFile clsFile;
	if (clsFile.Open(sFullPathName, CFile::typeBinary | CFile::modeRead))
	{
		LPSTR lpBuff = NULL;
		DWORD nLen = clsFile.GetLength();
		if(nLen == 0) return FALSE;
		
		lpBuff = new char[nLen];
		memset(lpBuff, 0, nLen);
		clsFile.Read(lpBuff, nLen);
		clsFile.Close();
		
		CMemFile memFile;// = CMemFile(nLen);//tour2k
		memFile.Attach((unsigned char*)lpBuff, nLen);
		
		CArchive ar(&memFile, CArchive::load);
		
		int nIndex, nCount, nVersion;
		CString strData;
		
		ar >> nVersion;
		if(nVersion==1)
		{
			ar >> pstFilterInfo->nInCodeType;        //# 0 : Market, 1 : PortPolio 
			ar.ReadString(pstFilterInfo->strInCodeData);  //# [0:전체, 1:거래소, 2:코스닥] or [PortPolio Name]
			//CStringArray saInCode;  //# User After Load!
			
			//# Port Data Load!
			if (pstFilterInfo->nInCodeType == 1)
				GetLoad_PortData(pstFilterInfo->strInCodeData, pstFilterInfo->psaInCode);
			
			ar >> pstFilterInfo->nNowPriceChk;
			ar >> pstFilterInfo->nNowPriceUp; 
			ar >> pstFilterInfo->nNowPriceDown;
			
			ar >> pstFilterInfo->nChangRateChk;
			ar >> pstFilterInfo->nChangRateUp; 
			ar >> pstFilterInfo->nChangRateDown;
			
			for (nIndex = 0; nIndex < (int)CNT_CHANGEVAL; nIndex++)
				ar >> pstFilterInfo->nChangValChk[nIndex];
			
			ar >> pstFilterInfo->nDealVolType;		//# 0 : 고정, 1 : Time
			ar >> pstFilterInfo->nDealVol;
			
			ar >> nCount;
			SubFilterInfo	stSubFilterInfo;
			for (nIndex = 0; nIndex < nCount; nIndex++)
			{
				ar >> stSubFilterInfo.nTime;
				ar >> stSubFilterInfo.nData;
				pstFilterInfo->plistDealTimeVol->AddTail(stSubFilterInfo);
			}
			
			ar >> pstFilterInfo->nDealPriceType;		//# 0 : 고정, 1 : Time
			ar >> pstFilterInfo->nDealPrice;
			
			ar >> nCount;
			for (nIndex = 0; nIndex < nCount; nIndex++)
			{
				ar >> stSubFilterInfo.nTime;
				ar >> stSubFilterInfo.nData;
				pstFilterInfo->plistDealTimePrice->AddTail(stSubFilterInfo);
			}
			
			ar >> pstFilterInfo->nNewsType;		//# 0 : 전체, 1 : 뉴스, 2 : 공시
			
			//			for (nIndex = 0; nIndex < (int)CNT_DIRECTION_0; nIndex++)
			//				ar >> pstFilterInfo->nDirectionChk[nIndex];
			ar >> pstFilterInfo->nDirectionChk[0];
			pstFilterInfo->nDirectionChk[1] = 0;
			pstFilterInfo->nDirectionChk[2] = 0;
			pstFilterInfo->nDirectionChk[3] = 0;
			pstFilterInfo->nDirectionChk[4] = 0;
			ar >> pstFilterInfo->nDirectionChk[5];	// 사라진필드므로 무시된다.
			for (nIndex = 2; nIndex < (int)CNT_DIRECTION_0; nIndex++)
				ar >> pstFilterInfo->nDirectionChk[nIndex+3];
			
			ar >> nCount;
			for (nIndex = 0; nIndex < nCount; nIndex++)
			{
				ar >> strData;
				pstFilterInfo->saOutCode.Add(strData);
			}
			
			ar.Close();
			if(lpBuff) delete lpBuff;
			return TRUE;
		}
		else
		{
			ar >> pstFilterInfo->nInCodeType;        //# 0 : Market, 1 : PortPolio 
			ar.ReadString(pstFilterInfo->strInCodeData);  //# [0:전체, 1:거래소, 2:코스닥] or [PortPolio Name]
			//CStringArray saInCode;  //# User After Load!
			
			//# Port Data Load!
			if (pstFilterInfo->nInCodeType == 1)
				GetLoad_PortData(pstFilterInfo->strInCodeData, pstFilterInfo->psaInCode);
			
			ar >> pstFilterInfo->nNowPriceChk;
			ar >> pstFilterInfo->nNowPriceUp; 
			ar >> pstFilterInfo->nNowPriceDown;
			
			ar >> pstFilterInfo->nChangRateChk;
			ar >> pstFilterInfo->nChangRateUp; 
			ar >> pstFilterInfo->nChangRateDown;
			
			for (nIndex = 0; nIndex < (int)CNT_CHANGEVAL; nIndex++)
				ar >> pstFilterInfo->nChangValChk[nIndex];
			
			ar >> pstFilterInfo->nDealVolType;		//# 0 : 고정, 1 : Time
			ar >> pstFilterInfo->nDealVol;
			
			ar >> nCount;
			SubFilterInfo	stSubFilterInfo;
			for (nIndex = 0; nIndex < nCount; nIndex++)
			{
				ar >> stSubFilterInfo.nTime;
				ar >> stSubFilterInfo.nData;
				pstFilterInfo->plistDealTimeVol->AddTail(stSubFilterInfo);
			}
			
			ar >> pstFilterInfo->nDealPriceType;		//# 0 : 고정, 1 : Time
			ar >> pstFilterInfo->nDealPrice;
			
			ar >> nCount;
			for (nIndex = 0; nIndex < nCount; nIndex++)
			{
				ar >> stSubFilterInfo.nTime;
				ar >> stSubFilterInfo.nData;
				pstFilterInfo->plistDealTimePrice->AddTail(stSubFilterInfo);
			}
			
			ar >> pstFilterInfo->nNewsType;		//# 0 : 전체, 1 : 뉴스, 2 : 공시
			
			for (nIndex = 0; nIndex < (int)CNT_DIRECTION_1; nIndex++)
				ar >> pstFilterInfo->nDirectionChk[nIndex];
			
			ar >> nCount;
			for (nIndex = 0; nIndex < nCount; nIndex++)
			{
				ar >> strData;
				pstFilterInfo->saOutCode.Add(strData);
			}
			
			ar.Close();
			if(lpBuff) delete lpBuff;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CLib_SignalMngApp::SetSave_FilterData(CString sFullPathName, FilterInfo *pstFilterInfo)
{
	if (pstFilterInfo == NULL)
		return FALSE;
	
	CFile clsFile;
	if(!clsFile.Open(sFullPathName, CFile::typeBinary | CFile::modeCreate | CFile::modeWrite)) 
		return FALSE;
	CArchive ar(&clsFile, CArchive::store);
	
	int nIndex, nCount;
	CString strData;
	
	ar << (int)VERSION_SETFILE;
	
	ar << pstFilterInfo->nInCodeType;    //# 0 : Market, 1 : PortPolio 
	ar.WriteString(pstFilterInfo->strInCodeData);   //# [0:전체, 1:거래소, 2:코스닥] or [PortPolio Name]
	ar.WriteString("\n");
	
	ar << pstFilterInfo->nNowPriceChk;
	ar << pstFilterInfo->nNowPriceUp;
	ar << pstFilterInfo->nNowPriceDown;
	
	ar << pstFilterInfo->nChangRateChk;
	ar << pstFilterInfo->nChangRateUp; 
	ar << pstFilterInfo->nChangRateDown;
	
	for (nIndex = 0; nIndex < (int)CNT_CHANGEVAL; nIndex++)
		ar << pstFilterInfo->nChangValChk[nIndex];
	
	ar << pstFilterInfo->nDealVolType;		//# 0 : 고정, 1 : Time
	ar << pstFilterInfo->nDealVol;
	
	nCount = pstFilterInfo->plistDealTimeVol->GetCount();
	
	ar << nCount;
	POSITION pos;
	SubFilterInfo	stSubFilterInfo;
	for (nIndex = 0; nIndex < nCount; nIndex++)
	{
		pos = pstFilterInfo->plistDealTimeVol->FindIndex(nIndex);
		stSubFilterInfo = pstFilterInfo->plistDealTimeVol->GetAt(pos);
		
		ar << stSubFilterInfo.nTime;
		ar << stSubFilterInfo.nData;
	}
	
	ar << pstFilterInfo->nDealPriceType;		//# 0 : 고정, 1 : Time
	ar << pstFilterInfo->nDealPrice;
	
	nCount = pstFilterInfo->plistDealTimePrice->GetCount();
	ar << nCount;
	for (nIndex = 0; nIndex < nCount; nIndex++)
	{
		pos = pstFilterInfo->plistDealTimePrice->FindIndex(nIndex);
		stSubFilterInfo = pstFilterInfo->plistDealTimePrice->GetAt(pos);
		
		ar << stSubFilterInfo.nTime;
		ar << stSubFilterInfo.nData;
	}
	
	ar << pstFilterInfo->nNewsType;		//# 0 : 전체, 1 : 뉴스, 2 : 공시
	
	for (nIndex = 0; nIndex < (int)CNT_DIRECTION_1; nIndex++)
		ar << pstFilterInfo->nDirectionChk[nIndex];
	
	nCount = pstFilterInfo->saOutCode.GetSize();
	ar << nCount;
	for (nIndex = 0; nIndex < nCount; nIndex++)
	{
		strData = pstFilterInfo->saOutCode.GetAt(nIndex);
		ar << strData;
	}
	
	ar.Close();
	clsFile.Close();
	return TRUE;
}

void _DLL_GetFilterData(long &stFilterInfo)
//void _DLL_GetFilterData(long &stFilterInfo, BOOL bDefultVal /*= FALSE*/)
{
	//  if ((bDefultVal == TRUE) || (theApp.m_pstFilterInfo == NULL))
	if (theApp.m_pstFilterInfo == NULL)
		theApp.SetDefult_FilterInfo((FilterInfo*)&stFilterInfo);    //# Set Defult!
	else
		theApp.SetAttach_FilterInfo(theApp.m_pstFilterInfo, (FilterInfo*)&stFilterInfo);
}

void _DLL_SetFilterData(long *pstFilterInfo)
{
	theApp.SetAttach_FilterInfo((FilterInfo*)pstFilterInfo, theApp.m_pstFilterInfo);
}

void CLib_SignalMngApp::SetAttach_FilterInfo(FilterInfo *pstFilterInfo_Base, 
											 FilterInfo *pstFilterInfo_Target)
{
	pstFilterInfo_Target->nInCodeType = pstFilterInfo_Base->nInCodeType;        //# 0 : Market, 1 : PortPolio 
	pstFilterInfo_Target->strInCodeData = pstFilterInfo_Base->strInCodeData;    //# [전체, 거래소, 코스닥] or [PortPolio Name]
	
	//# Port Data Load!
	if (pstFilterInfo_Target->nInCodeType == 1)
		GetLoad_PortData(pstFilterInfo_Target->strInCodeData, pstFilterInfo_Target->psaInCode);
	
	pstFilterInfo_Target->nNowPriceChk = pstFilterInfo_Base->nNowPriceChk;
	pstFilterInfo_Target->nNowPriceUp = pstFilterInfo_Base->nNowPriceUp;
	pstFilterInfo_Target->nNowPriceDown = pstFilterInfo_Base->nNowPriceDown;
	
	pstFilterInfo_Target->nChangRateChk = pstFilterInfo_Base->nChangRateChk;
	pstFilterInfo_Target->nChangRateUp = pstFilterInfo_Base->nChangRateUp;
	pstFilterInfo_Target->nChangRateDown = pstFilterInfo_Base->nChangRateDown;
	
	int nIndex;
	for (nIndex = 0; nIndex < (int)CNT_CHANGEVAL; nIndex++)
		pstFilterInfo_Target->nChangValChk[nIndex] = pstFilterInfo_Base->nChangValChk[nIndex];
	
	pstFilterInfo_Target->nDealVolType = pstFilterInfo_Base->nDealVolType;		//# 0 : 고정, 1 : Time
	pstFilterInfo_Target->nDealVol = pstFilterInfo_Base->nDealVol;
	
	
	POSITION pos;
	SubFilterInfo	stSubFilterInfo;
	int nCount;
	pstFilterInfo_Target->plistDealTimeVol->RemoveAll();
	nCount = pstFilterInfo_Base->plistDealTimeVol->GetCount();
	for (nIndex = 0; nIndex < nCount; nIndex++)
	{
		pos = pstFilterInfo_Base->plistDealTimeVol->FindIndex(nIndex);
		stSubFilterInfo = pstFilterInfo_Base->plistDealTimeVol->GetAt(pos);
		
		pstFilterInfo_Target->plistDealTimeVol->AddTail(stSubFilterInfo);
	}
	
	pstFilterInfo_Target->nDealPriceType = pstFilterInfo_Base->nDealPriceType;		//# 0 : 고정, 1 : Time
	pstFilterInfo_Target->nDealPrice = pstFilterInfo_Base->nDealPrice;
	
	pstFilterInfo_Target->plistDealTimePrice->RemoveAll();
	nCount = pstFilterInfo_Base->plistDealTimePrice->GetCount();
	for (nIndex = 0; nIndex < nCount; nIndex++)
	{
		pos = pstFilterInfo_Base->plistDealTimePrice->FindIndex(nIndex);
		stSubFilterInfo = pstFilterInfo_Base->plistDealTimePrice->GetAt(pos);
		
		pstFilterInfo_Target->plistDealTimePrice->AddTail(stSubFilterInfo);
	}
	
	pstFilterInfo_Target->nNewsType = pstFilterInfo_Base->nNewsType;
	
	for (nIndex = 0; nIndex < (int)CNT_DIRECTION_1; nIndex++)
		pstFilterInfo_Target->nDirectionChk[nIndex] = pstFilterInfo_Base->nDirectionChk[nIndex];
	
	CString strData;
	pstFilterInfo_Target->saOutCode.RemoveAll();
	nCount = pstFilterInfo_Base->saOutCode.GetSize();
	for (nIndex = 0; nIndex < nCount; nIndex++)
	{
		strData = pstFilterInfo_Base->saOutCode.GetAt(nIndex);
		
		pstFilterInfo_Target->saOutCode.Add(strData);
	}
}

void CLib_SignalMngApp::GetLoad_PortData(CString strPortGroup, CStringArray *psaInCode)
{
	if (psaInCode == NULL)
		return;
	
	psaInCode->RemoveAll();
	
	CString strFileName;
	//strFileName.Format("%s%s") = m_strUserPath + "\\" + m_strUserId + "_PORTFOLIO.ini";
	strFileName.Format("%s%s\\portfolio.i%02d", m_strUserPath, m_strUserId, atoi(strPortGroup));
	
	DWORD	fLen = 0;
	CFileFind finder;
	if (!finder.FindFile(strFileName))
		return ;
	
	UINT	readL;
	CString	code = _T(""), amount = _T(""), price = _T(""), name = _T("");
	struct	_inters interest;
	
	CFileStatus fs;
	if (CFile::GetStatus(strFileName, fs) == FALSE)
		return ;
	
	CFile	fileH(strFileName, CFile::modeRead);
	if (fileH.m_hFile == CFile::hFileNull)
		return ;
	
	for (int nIndex = 0; nIndex < MAX_LINE ; nIndex++)
	{
		readL = fileH.Read(&interest, sizeof(struct _inters));
		if (readL < sizeof(struct _inters))
			break;
		
		code.Format("%.*s", sizeof(interest.code), interest.code);
		name.Format("%.*s", sizeof(interest.name), interest.name);		
		name.TrimLeft(), name.TrimRight();
		psaInCode->Add(code);
	}
	fileH.Close();

// 	char	szBuffer[128];
// 	CString strData = "", strKey = "";
// 	memset(szBuffer, 0, 128);
// 	::GetPrivateProfileString(strPortGroup, "COUNT", "0", szBuffer, 128, strFileName);
// 	strData = szBuffer;
// 	
// 	int nItemCnt = atol(strData);
// 	if (nItemCnt <= 0)
// 		return;
// 	
// 	CString strData_Trace = strPortGroup + "/";
// 	for (int nIndex = 0; nIndex < nItemCnt; nIndex++)
// 	{
// 		strKey.Format("%d", nIndex);
// 		memset(szBuffer, 0, 128);
// 		::GetPrivateProfileString(strPortGroup, strKey, "", szBuffer, 128, strFileName);
// 		strData = szBuffer;
// 		
// 		strData = GetFormat_Port(strData);
// 		if (strData.IsEmpty() == FALSE)
// 			psaInCode->Add(strData);
// 		
// 		strData_Trace += strData + "/";
// 	}
//	
//	theApp.SetWriteDebug("Port Data", strData_Trace);
}

CString CLib_SignalMngApp::GetFormat_Port(CString& strData)
{
	CString strSeparator = "@", strResult = "";
	
	while (strData.GetLength() > 0 )
	{
		strResult	= strData.Left(strData.Find(strSeparator));
		strData	= strData.Right(strData.GetLength() - (strData.Find(strSeparator)+1));
		
		return strResult;
	}
	
	return strResult;
}

int CLib_SignalMngApp::GetRealData(int nSignalType, LPSTR sRealData, int nLen, CString sPBID)
{
	int nResult = -1;

	char szSvrErr[6] = {0,};
	CString strSvrErr;
	memcpy(szSvrErr, sRealData, sizeof(szSvrErr));
	strSvrErr.Format("%s", szSvrErr);
	
	
	if(sPBID == "H0STFND2")
	{	
		// 실시간 Set이 걸려있는 서버이상, 다시 Set
		CMapStringToOb *pmapData;
		pmapData = m_pmapAlert_Sigal;

		if (pmapData != NULL)
		{
			CString strKey;
			CPtrArray *paData;
			CList<HWND, HWND> listWndSend;
			POSITION _pos;

			POSITION pos = pmapData->GetStartPosition();
			int nSize = pmapData->GetCount();
			for(int i = 0; i < nSize; i++)
			{
				pmapData->GetNextAssoc(pos, strKey, (CObject*&)paData);

				if (pmapData->Lookup(strKey, (CObject*&)paData) == TRUE)
				{
					for (int nIndex = 0; nIndex < paData->GetSize(); nIndex++)
					{
						CWnd *pwndSend = (CWnd *)paData->GetAt(nIndex);

						BOOL bAlreadySend = FALSE;
						for(int i = 0; i < listWndSend.GetCount(); i++)
						{
							_pos = listWndSend.FindIndex(i);
							if(listWndSend.GetAt(_pos) == pwndSend->m_hWnd)
							{
								bAlreadySend = TRUE;
							}	
						}

						
// 						if(!bAlreadySend)
// 						{
// 							if (::IsWindow(pwndSend->m_hWnd) == TRUE)
// 								::SendMessage(pwndSend->m_hWnd, WM_BC_DATA_RECEIVED, (WPARAM)(LPCTSTR)sPBID, (LPARAM)(LPCTSTR)strSvrErr);
// 
// 							listWndSend.AddTail(pwndSend->m_hWnd);
// 						}
						
					}
				}
			}
		}
	
		pmapData->RemoveAll();	
		return 0L;
	}
	
	if (theApp.m_pstFilterInfo == NULL)
	{
		theApp.SetWriteDebug("FilterInfo (NULL)", "");
		return nResult;
	}

	CString strTemp;
	strTemp.Format("ID : %d, Data : %d , Length : %d", nSignalType, strlen(sRealData), nLen);
	theApp.SetWriteDebug("Input Type", strTemp);
	theApp.SetWriteDebug("Input : Data", sRealData);
	if ((int)TYPE_REAL_NEWS == nSignalType)
	{
		if (theApp.GetRealData_News(sRealData, nLen) == TRUE)
			nResult = 1;
	}
	else if ((int)TYPE_SIGNAL_SYS == nSignalType)
	{
		if (theApp.m_pmapSignalSet_Sys != NULL)
		{
			// 한투 조건검색 서버 분리로 인한 서버 분류, sPBID 작업
			if (theApp.GetCheck_Condition(sRealData, nLen, sPBID) > 0)
				nResult = 1;
		}
		else
			theApp.SetWriteDebug("Data Input => Reg SYS NULL", "");
	}
	else if ((int)TYPE_SIGNAL_ALERT == nSignalType)
	{
		if (theApp.GetCheck_Condition(sRealData, nLen, sPBID, FALSE) > 0)
			nResult = 1;
	}
	
	return nResult;
}

BOOL CLib_SignalMngApp::GetCheck_Filter(FilterInfo *pstFilterInfo, 
										CString strTime, CString strCode,
										Signal_CommonInfo stCommon)
{
	CString strData, strTemp;
	if (pstFilterInfo == NULL)
		return FALSE;
	
	strCode.TrimRight(" ");
	if (strCode.IsEmpty() == TRUE)
	{
		if (m_bTrace_Filter == TRUE)
			SetWriteDebug("Check Filter X [Code]", strCode);
		return FALSE;
	}
	
	//# nTime => HHMMSS => HH
	strData = strTime.Left(2);
	int nTime = atoi(strData);
	
	int nIndex, nData;
	char *pData;
	BOOL bState;
	
	//# 제외 종목
	bState = TRUE;
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
	{
		if (m_bTrace_Filter == TRUE)
			SetWriteDebug("Check Filter X [OutCode]", strCode);
		
		return FALSE;
	}
	
	if (pstFilterInfo->nInCodeType == 0)    //# Market
	{
		int nMarketType;
		nMarketType = atoi(pstFilterInfo->strInCodeData);
		
		//# 0 : 전체, 1 : 코스피, 2 : 코스닥
		if (nMarketType > 0)			
		{
			pData = new char[sizeof(stCommon.sMarket) + 1];
			memset(pData, 0x00, sizeof(stCommon.sMarket) + 1);
			memcpy(pData, &stCommon.sMarket, sizeof(stCommon.sMarket));
			nData = atoi(pData);
			delete[] pData;
			
			if (nMarketType != nData)
			{
				if (m_bTrace_Filter == TRUE)
				{
					strTemp.Format("%d", nData);
					SetWriteDebug("Check Filter X [MarketType]", strTemp);
				}
				return FALSE;
			}
		}
	}
	else if (pstFilterInfo->nInCodeType == 1)    //# PortPolio 
	{
		bState = TRUE;
		if (pstFilterInfo->psaInCode == NULL)
			return FALSE;
		
		for (nIndex = 0; nIndex < pstFilterInfo->psaInCode->GetSize(); nIndex++)
		{
			strData = pstFilterInfo->psaInCode->GetAt(nIndex);
			
			if (strCode == strData.Left((int)CODE_LEN))
			{
				bState = FALSE;
				break;
			}
		}
		
		if (bState == TRUE)
		{
			if (m_bTrace_Filter == TRUE)
				SetWriteDebug("Check Filter X [In Code]", strCode);  
			return FALSE;
		}
	}
	
	if (pstFilterInfo->nNowPriceChk == 1)
	{
		pData = new char[sizeof(stCommon.sPrice) + 1];
		memset(pData, 0x00, sizeof(stCommon.sPrice) + 1);
		memcpy(pData, &stCommon.sPrice, sizeof(stCommon.sPrice));
		nData = atoi(pData);
		delete[] pData;
		
		if ((pstFilterInfo->nNowPriceUp <= nData) &&
			(pstFilterInfo->nNowPriceDown >= nData))
		{
			
		}
		else
		{
			if (m_bTrace_Filter == TRUE)
			{
				strTemp.Format("%d", nData);
				SetWriteDebug("Check Filter X [NowPrice]", strTemp);
			}
			return FALSE;
		}
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
		
		if ((pstFilterInfo->nChangRateUp <= nData) &&
			(pstFilterInfo->nChangRateDown >= nData))
		{
			
		}
		else
		{
			if (m_bTrace_Filter == TRUE)
			{
				strTemp.Format("%d", nData);
				SetWriteDebug("Check Filter X [ChangRate]", strTemp);
			}
			return FALSE;
		}
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
	{
		if (m_bTrace_Filter == TRUE)
		{
			strTemp.Format("%d", nData);
			SetWriteDebug("Check Filter X [Sign]", strTemp);
		}
		return FALSE;
	}
	
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
		nDealVol = pstFilterInfo->nDealVol;
		nDealVol *= 1000;
		if (nDealVol > nData)
		{
			if (m_bTrace_Filter == TRUE)
			{
				strTemp.Format("%d", nData);
				SetWriteDebug("Check Filter X [Volume]", strTemp);
			}
			return FALSE;
		}
	}
	else if (pstFilterInfo->nDealVolType == 1)		//# 0 : 고정, 1 : Time
	{
		bState = FALSE;
		
		CTime timeDest;
		timeDest = CTime::GetCurrentTime();
		CString strTemp;
		
		strTemp = timeDest.Format("%H");
		CString strMin = timeDest.Format("%M");
		if(_ttoi(strMin) > 0)
		{
			strTemp.Format("%d", _ttoi(strTemp) + 1);
		}
		
		int nTime = atoi(strTemp);
		
		for (nIndex = 0; nIndex < pstFilterInfo->plistDealTimeVol->GetCount(); nIndex++)
		{
			pos = pstFilterInfo->plistDealTimeVol->FindIndex(nIndex);
			stSubFilterInfo = pstFilterInfo->plistDealTimeVol->GetAt(pos);
			
			nDealVol = stSubFilterInfo.nData;
			nDealVol *= 1000;
			
			if (stSubFilterInfo.nTime >= nTime)
			{
				if(nDealVol <= nData)
				{
					bState = TRUE;
				}
				else
					bState = FALSE;
			}
			
			if (nMax_Time < stSubFilterInfo.nTime)
			{
				nMax_Time = stSubFilterInfo.nTime;
				nMaxVal = nDealVol;
			}
			
			
			// 			if (stSubFilterInfo.nTime == nTime) 
			// 			{
			// 				bState = TRUE;
			// 				if (nDealVol <= nData)
			// 					break;
			// 				else
			// 				{
			// 					if (m_bTrace_Filter == TRUE)
			// 					{
			// 						strData.Format("%d", nData);
			// 						SetWriteDebug("Check Filter X [Volume]", strData);
			// 					}
			// 					return FALSE;
			// 				}
			// 			}
			// 			
			// 			if (nMax_Time < stSubFilterInfo.nTime)
			// 			{
			// 				nMax_Time = stSubFilterInfo.nTime;
			// 				nMaxVal = nDealVol;
			// 			}
		}
		
		if (bState == FALSE)
		{
			if ((nMax_Time < nTime) && (nMaxVal <= nData))
			{
				
			}
			else
			{
				if (m_bTrace_Filter == TRUE)
				{
					strData.Format("%d", nData);
					SetWriteDebug("Check Filter X [Volume]", strData);
				}
				return FALSE;
			}
		}
	}
	
	pData = new char[sizeof(stCommon.sAmount) + 1];
	memset(pData, 0x00, sizeof(stCommon.sAmount) + 1);
	memcpy(pData, &stCommon.sAmount, sizeof(stCommon.sAmount));
	nData = atoi(pData);
	delete[] pData;
	
	int nDealPrice;
	if (pstFilterInfo->nDealPriceType == 0)		//# 0 : 고정, 1 : Time
	{
		nDealPrice = pstFilterInfo->nDealPrice;
		nDealPrice *= 10000;
		if (nDealPrice > nData)
		{
			if (m_bTrace_Filter == TRUE)
			{
				strTemp.Format("%d", nData);
				SetWriteDebug("Check Filter X [Amount]", strTemp);
			}
			return FALSE;
		}
	}
	else if (pstFilterInfo->nDealPriceType == 1)		//# 0 : 고정, 1 : Time
	{
		bState = FALSE;
		for (nIndex = 0; nIndex < pstFilterInfo->plistDealTimePrice->GetCount(); nIndex++)
		{
			pos = pstFilterInfo->plistDealTimePrice->FindIndex(nIndex);
			stSubFilterInfo = pstFilterInfo->plistDealTimePrice->GetAt(pos);
			
			nDealPrice = stSubFilterInfo.nData;
			nDealPrice *= 10000;
			
			if (stSubFilterInfo.nTime >= nTime)
			{
				if(nDealPrice <= nData)
				{
					bState = TRUE;
				}
				else
					bState = FALSE;
			}
			
			if (nMax_Time < stSubFilterInfo.nTime)
			{
				nMax_Time = stSubFilterInfo.nTime;
				nMaxVal = nDealPrice;
			}
			
			
			// 			if (stSubFilterInfo.nTime == nTime)
			// 			{
			// 				bState = TRUE;
			// 				if (nDealPrice <= nData)
			// 					break;
			// 				else
			// 				{
			// 					if (m_bTrace_Filter == TRUE)
			// 					{
			// 						strData.Format("%d", nData);
			// 						SetWriteDebug("Check Filter X [Amount]", strData);
			// 					}
			// 					return FALSE;
			// 				}
			// 			}
			// 			
			// 			if (nMax_Time < stSubFilterInfo.nTime)
			// 			{
			// 				nMax_Time = stSubFilterInfo.nTime;
			// 				nMaxVal = nDealPrice;
			// 			}
		}
		
		if (bState == FALSE)
		{
			if ((nMax_Time < nTime) && (nMaxVal <= nData))
			{
				
			}
			
			else
			{
				if (m_bTrace_Filter == TRUE)
				{
					strData.Format("%d", nData);
					SetWriteDebug("Check Filter X [Amount]", strData);
				}
				return FALSE;
			}
		}
	}
	
	CUIntArray	iaDirectionChk;
	for (nIndex = 0; nIndex < (int)CNT_DIRECTION_1; nIndex++)
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
					pData = new char[sizeof(stCommon.sManagement) + 1];
					memset(pData, 0x00, sizeof(stCommon.sManagement) + 1);
					memcpy(pData, &stCommon.sManagement, sizeof(stCommon.sManagement));
					nData = atoi(pData);
					delete[] pData;
					
					if (pstFilterInfo->nDirectionChk[nIndex] == 1)
					{
						if (nData == 1)
						{
							if (m_bTrace_Filter == TRUE)
							{
								strTemp.Format("Index %d, Data : %d", nIndex_Chk, nData);
								SetWriteDebug("Check Filter X [Direction]", strTemp);
							}
							return FALSE;
						}
					}
				}
				break;
				/*
				case 1 :		//# 이상급등
				{
				nIndex = 1;
				pData = new char[sizeof(stCommon.sControl) + 1];
				memset(pData, 0x00, sizeof(stCommon.sControl) + 1);
				memcpy(pData, &stCommon.sControl, sizeof(stCommon.sControl));
				nData = atoi(pData);
				delete[] pData;
				
				  if (pstFilterInfo->nDirectionChk[nIndex] == 1)
				  {
				  if (nData == 1)
				  {
				  if (m_bTrace_Filter == TRUE)
				  {
				  strTemp.Format("Index %d, Data : %d", nIndex_Chk, nData);
				  SetWriteDebug("Check Filter X [Direction]", strTemp);
				  }
				  return FALSE;
				  }
				  }
				  }
				  break;
				*/
			case 1 :		//# 투자주의/주자경고/투자위험/투자위험예고
				{
					//pData = new char[sizeof(stCommon.sControl) + 1];
					//memset(pData, 0x00, sizeof(stCommon.sControl) + 1);
					//memcpy(pData, &stCommon.sControl, sizeof(stCommon.sControl));
					//nData = atoi(pData) - 'A';
					//delete[] pData;
					
					// 20110412 박찬모 신호검색 필터링 오류 >>
					//nData = stCommon.sControl - 'A';
					
					CString strData;
					strData.Format("%c", stCommon.sControl);
					nData = _ttoi(strData);
					// 20110412 박찬모 신호검색 필터링 오류 <<
					
					nIndex = 1;
					if (pstFilterInfo->nDirectionChk[nIndex] == 1)
					{
						/*if (nData & 1)*/
						if (nData == 1) // 20110412 박찬모 신호검색 필터링 오류 >>
						{
							if (m_bTrace_Filter == TRUE)
							{
								//strTemp.Format("Index %d, Data : %d", nIndex_Chk, nData);
								strTemp.Format("Index %d, Data : %d", nIndex_Chk, nData&1);
								SetWriteDebug("Check Filter X [Direction]", strTemp);
							}
							return FALSE;
						}
					}
					
					nIndex = 2;
					if (pstFilterInfo->nDirectionChk[nIndex] == 1)
					{
						/*if (nData & 2)*/
						if (nData == 2) // 20110412 박찬모 신호검색 필터링 오류 >>
						{
							if (m_bTrace_Filter == TRUE)
							{
								strTemp.Format("Index %d, Data : %d", nIndex_Chk, nData&2);
								SetWriteDebug("Check Filter X [Direction]", strTemp);
							}
							return FALSE;
						}
					}
					
					nIndex = 4;
					if (pstFilterInfo->nDirectionChk[nIndex] == 1)
					{
						/*if (nData & 4)*/
						if (nData == 3) // 20110412 박찬모 신호검색 필터링 오류 >>
						{
							if (m_bTrace_Filter == TRUE)
							{
								strTemp.Format("Index %d, Data : %d", nIndex_Chk, nData&4);
								SetWriteDebug("Check Filter X [Direction]", strTemp);
							}
							return FALSE;
						}
					}
					
					nIndex = 3;
					if (pstFilterInfo->nDirectionChk[nIndex] == 1)
					{
						/*if (nData & 8)*/
						if (nData == 4) // 20110412 박찬모 신호검색 필터링 오류 >>
						{
							if (m_bTrace_Filter == TRUE)
							{
								strTemp.Format("Index %d, Data : %d", nIndex_Chk, nData&8);
								SetWriteDebug("Check Filter X [Direction]", strTemp);
							}
							return FALSE;
						}
					}
					
				}
				break;
			case 2 :		//# 거래정지
				{
					nIndex = 2;
					nIndex += 3;	// index변경때문에 3씩 더해줌.
					pData = new char[sizeof(stCommon.sTradeStop) + 1];
					memset(pData, 0x00, sizeof(stCommon.sTradeStop) + 1);
					memcpy(pData, &stCommon.sTradeStop, sizeof(stCommon.sTradeStop));
					nData = atoi(pData);
					delete[] pData;
					
					if (pstFilterInfo->nDirectionChk[nIndex] == 1)
					{
						if (nData == 1)
						{
							if (m_bTrace_Filter == TRUE)
							{
								strTemp.Format("Index %d, Data : %d", nIndex_Chk, nData);
								SetWriteDebug("Check Filter X [Direction]", strTemp);
							}
							return FALSE;
						}
					}
				}
				break;
			case 3 :					//# 투자유의
				{
					nIndex = 3;
					nIndex += 3;	// index변경때문에 3씩 더해줌.
					pData = new char[sizeof(stCommon.sInvestAttention) + 1];
					memset(pData, 0x00, sizeof(stCommon.sInvestAttention) + 1);
					memcpy(pData, &stCommon.sInvestAttention, sizeof(stCommon.sInvestAttention));
					nData = atoi(pData);
					delete[] pData;
					
					if (pstFilterInfo->nDirectionChk[nIndex] == 1)
					{
						if (nData == 1)
						{
							if (m_bTrace_Filter == TRUE)
							{
								strTemp.Format("Index %d, Data : %d", nIndex_Chk, nData);
								SetWriteDebug("Check Filter X [Direction]", strTemp);
							}
							return FALSE;
						}
					}
				}
				break;
			case 4:		//# 불성실공시
				{
					nIndex = 4;
					nIndex += 3;	// index변경때문에 3씩 더해줌.
					pData = new char[sizeof(stCommon.sInsincerityNotice) + 1];
					memset(pData, 0x00, sizeof(stCommon.sInsincerityNotice) + 1);
					memcpy(pData, &stCommon.sInsincerityNotice, sizeof(stCommon.sInsincerityNotice));
					nData = atoi(pData);
					delete[] pData;
					
					if (pstFilterInfo->nDirectionChk[nIndex] == 1)
					{
						if (nData == 1)
						{
							if (m_bTrace_Filter == TRUE)
							{
								strTemp.Format("Index %d, Data : %d", nIndex_Chk, nData);
								SetWriteDebug("Check Filter X [Direction]", strTemp);
							}
							return FALSE;
						}
					}
				}
				break;
			case 5:		//# 정리매매
				{
					nIndex = 5;
					nIndex += 3;	// index변경때문에 3씩 더해줌.
					pData = new char[sizeof(stCommon.sJungriMeme) + 1];
					memset(pData, 0x00, sizeof(stCommon.sJungriMeme) + 1);
					memcpy(pData, &stCommon.sJungriMeme, sizeof(stCommon.sJungriMeme));
					nData = atoi(pData);
					delete[] pData;
					
					if (pstFilterInfo->nDirectionChk[nIndex] == 1)
					{
						if (nData == 1)
						{
							if (m_bTrace_Filter == TRUE)
							{
								strTemp.Format("Index %d, Data : %d", nIndex_Chk, nData);
								SetWriteDebug("Check Filter X [Direction]", strTemp);
							}
							return FALSE;
						}
					}
				}
				break;
			case 6:		//# 증거금 100%
				{
					nIndex = 6;
					nIndex += 3;	// index변경때문에 3씩 더해줌.
					pData = new char[sizeof(stCommon.sDepositMoney) + 1];
					memset(pData, 0x00, sizeof(stCommon.sDepositMoney) + 1);
					memcpy(pData, &stCommon.sDepositMoney, sizeof(stCommon.sDepositMoney));
					nData = atoi(pData);
					delete[] pData;
					
					if (pstFilterInfo->nDirectionChk[nIndex] == 1)
					{
						if (nData == 100) 
						{
							if (m_bTrace_Filter == TRUE)
							{
								strTemp.Format("Index %d, Data : %d", nIndex_Chk, nData);
								SetWriteDebug("Check Filter X [Direction]", strTemp);
							}
							return FALSE;
						}
					}
				}
				break;
			case 7:		//# 신용제한 종목
				{
					nIndex = 7;
					nIndex += 3;	// index변경때문에 3씩 더해줌.
					pData = new char[sizeof(stCommon.sConfidenceLimit) + 1];
					memset(pData, 0x00, sizeof(stCommon.sConfidenceLimit) + 1);
					memcpy(pData, &stCommon.sConfidenceLimit, sizeof(stCommon.sConfidenceLimit));
					nData = atoi(pData);
					delete[] pData;
					
					if (pstFilterInfo->nDirectionChk[nIndex] == 1)
					{
						if (nData == 1)
						{
							if (m_bTrace_Filter == TRUE)
							{
								strTemp.Format("Index %d, Data : %d", nIndex_Chk, nData);
								SetWriteDebug("Check Filter OK [Direction]", strTemp);
							}
							return FALSE;
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

return TRUE;
}

BOOL Convert_REALNEWS_HEAD_Signal_NewsInfo_Head(LPSTR &sRealData, int nLen, Signal_NewsInfo_Head* _pSNHead)
{
	REALNEWS_HEAD* pRealNews = (REALNEWS_HEAD*)&sRealData[7];
//확인필
//	REALNEWS_HEAD* pRealNews = (REALNEWS_HEAD*)&sRealData[10];
	char *pData;
	CString strTmp; //, strCode, strTitle, strProfferID;

	STR_CONVERT(pData, pRealNews->stHeadInfo.sSize, sizeof(pRealNews->stHeadInfo.sSize), strTmp);

	// Space
	memset(_pSNHead, 0x20, nLen);

	// dgubun+nwcode + Signal_HeadInfo + DR_NEWS_KEY 
	int nCopyLen = sizeof(Signal_HeadInfo) + sizeof(DR_NEWS_KEY);
	memcpy(_pSNHead->stHeadInfo.sKey, pRealNews->stHeadInfo.sKey, nCopyLen);
	//memcpy(_pSNHead->dgubun, pRealNews->dgubun, nCopyLen);

	memcpy(_pSNHead->stNewsInfo.stNewsCommon.sDate, pRealNews->sDate, sizeof(pRealNews->sDate));
	memcpy(_pSNHead->stNewsInfo.stNewsCommon.sTime, pRealNews->sTime, sizeof(pRealNews->sTime));
	// 뉴스정보
	memcpy(_pSNHead->stNewsInfo.stNewsCommon.sNewsPoint, pRealNews->sNsPnt, 2);			/* 뉴스 점수					*/
	memcpy(_pSNHead->stNewsInfo.stNewsCommon.sCode, pRealNews->sRepCd, CODE_LEN);		/* 종목 코드 				*/
	memcpy(_pSNHead->stNewsInfo.stNewsCommon.sName, pRealNews->sRepNm, 40);				/* 종목명    				*/
	memcpy(_pSNHead->stNewsInfo.stNewsCommon.sProfferID, pRealNews->sPrfCd, 2);			/* 뉴스 제공처 ID		*/
	memcpy(_pSNHead->stNewsInfo.stNewsCommon.sProffer, pRealNews->sPrfNm, 20);			/* 뉴스 제공처 			*/
	memcpy(_pSNHead->stNewsInfo.stNewsCommon.sProfferTypeID, pRealNews->sBrnchCd, 3);	/* 제공처 분류 ID		*/
	memcpy(_pSNHead->stNewsInfo.stNewsCommon.sProfferType, pRealNews->sBrnchNm, 50);	/* 제공처 분류 			*/
	memcpy(_pSNHead->stNewsInfo.stNewsCommon.sTitle, pRealNews->sTitle, 255);			/* 제 목 						*/

	// 부가정보
	nCopyLen = sizeof(Signal_CommonInfo);
	memcpy(&_pSNHead->stNewsInfo.stCommon, &pRealNews->stCommon, nCopyLen);

	//상황문
	memcpy(_pSNHead->sCaseLen, pRealNews->sCSCnt, 2);
	memcpy(_pSNHead->sCSCd, pRealNews->sCSCd, 180);
	
	return TRUE;
}

BOOL CLib_SignalMngApp::GetRealData_News(LPSTR &sRealData, int nLen)
{
	EnterCriticalSection(&m_CriticalSection);

	BOOL bResult = FALSE;

	char* pDataBuf = new char[nLen+1];
	pDataBuf[nLen] = 0;
	Signal_NewsInfo_Head* pSNHead = (Signal_NewsInfo_Head*)pDataBuf;
	Convert_REALNEWS_HEAD_Signal_NewsInfo_Head(sRealData, nLen, pSNHead);

	CString strMsg;
	char *pBuff;
	int nDataLen;

	INT_CONVERT(pBuff, pSNHead->stHeadInfo.sSize, 4, nDataLen);
	strMsg.Format("News Data Data : %d, Length : %d", sizeof(Signal_NewsInfo_Head), nLen);
	strMsg.Format("News Data Data : %d, Length : %d", nDataLen, nLen);
	
//확인필요
//  if (nDataLen+10 > nLen)
	if (nDataLen+6 > nLen)
	{
		theApp.SetWriteDebug("[ERROR] Data Length (X)", strMsg);
		theApp.SetWriteDebug("[ERROR] Data Length (X)", sRealData);
		
		LeaveCriticalSection(&m_CriticalSection);

		delete [] pDataBuf;
		return bResult;
	}

	BOOL bCheck_UserKeyword = TRUE;   //# USER KeyWord Check?
	BOOL bSignalOK = FALSE;        //# Signal News = Real News Point! (Danny : 2007.1.2)

	//# Convert Packet!
	SIGANL_NEWS	stNews;
	//if (GetConvertPacket_News("NEWS", stNewsInfo_Head.stNewsInfo, stNews) == TRUE)
	if (GetConvertPacket_News("NEWS", pSNHead->stNewsInfo, stNews) == TRUE)
	{
		CString strCode, strProfferID;
		STR_CONVERT(pBuff, pSNHead->stNewsInfo.stNewsCommon.sProfferID, sizeof(pSNHead->stNewsInfo.stNewsCommon.sProfferID), strProfferID);
		STR_CONVERT(pBuff, pSNHead->stNewsInfo.stNewsCommon.sCode, sizeof(pSNHead->stNewsInfo.stNewsCommon.sCode), strCode);
		strCode.TrimRight(" ");
		theApp.SetWriteDebug("Input : Code Data", strCode);

		if (bCheck_UserKeyword == TRUE)
		{
			//# Point Calculation!
			int nKeyCodeCnt;
			CString strCaseCodeList;
			INT_CONVERT(pBuff, pSNHead->sCaseLen, sizeof(pSNHead->sCaseLen), nKeyCodeCnt);
			if(nKeyCodeCnt>0)
			{
				TRACE("fdkjfkd");
			} 
		//	STR_CONVERT(pBuff, sRealData + sizeof(Signal_NewsInfo_Head), 6 * nKeyCodeCnt, strCaseCodeList);
		//	STR_CONVERT(pBuff, pSNHead->sCSCd, 6 * nKeyCodeCnt, strCaseCodeList);
			STR_CONVERT(pBuff, pSNHead->sCSCd, 6 * nKeyCodeCnt, strCaseCodeList);

			strMsg.Format("Cnt : %d, Code : %s", nKeyCodeCnt, strCaseCodeList);
			theApp.SetWriteDebug("Input : CaseCode Data", strMsg);
			BOOL bCaseCodeState = FALSE;
			bCaseCodeState = GetCheck_CaseCodeToPoint(strCaseCodeList, &stNews);
			if (bCaseCodeState == FALSE)
				theApp.SetWriteDebug("[ERROR] Get News Point (X)", strCaseCodeList);

			if (bCaseCodeState == TRUE)
			{
				BOOL bNewsProffer = FALSE;
				if (m_pstFilterInfo->nNewsType == 0)        //# 전체
					bNewsProffer = TRUE;
				else
				{
					if (m_pstFilterInfo->nNewsType == 1)    //# 뉴스
					{
						if (strProfferID != "10")   //# 공시
							bNewsProffer = TRUE;
					}
					else if (m_pstFilterInfo->nNewsType == 2)    //# 공시
					{
						if (strProfferID == "10")   //# 공시
							bNewsProffer = TRUE;
					}
				}

				if (bNewsProffer == TRUE)
				{
					CString strTime;
					STR_CONVERT(pBuff, pSNHead->stNewsInfo.stNewsCommon.stNewsKey.sTime, sizeof(pSNHead->stNewsInfo.stNewsCommon.stNewsKey.sTime), strTime);
					bSignalOK = GetCheck_Filter(m_pstFilterInfo, strTime, strCode, pSNHead->stNewsInfo.stCommon);
				}
				else
					SetWriteDebug("Check Filter X [News Proffer]", strProfferID);
			}
		}
		else
		{
			if ((m_pmapSignalSet_News->GetCount() > 0) && (strCode.IsEmpty() == FALSE))
			{
				BOOL bSearch_InBody = FALSE;
				BOOL bTrState_NewsAll;
				if (bSearch_InBody == TRUE)
				{
					bTrState_NewsAll = m_pwndDummy->SetSearch_NewsAll(pSNHead->stNewsInfo);
					SetWriteDebug("Signal NEWS => 공시 TR 요청!", (bTrState_NewsAll == TRUE) ? "성공":"실패");
				}
				else
				{
					if (strProfferID == "10")    //# 공시
					{
						bTrState_NewsAll = m_pwndDummy->SetSearch_NewsAll(pSNHead->stNewsInfo);
						SetWriteDebug("Signal NEWS => 공시 TR 요청!", (bTrState_NewsAll == TRUE) ? "성공":"실패");
					}
					else
						GetCheck_News(pSNHead->stNewsInfo);
				}
			}
		}

		char lpData[SZ_SIGANL_NEWS + 1];
		memset(&lpData, 0x00, SZ_SIGANL_NEWS + 1);
		memcpy(&lpData, &stNews, SZ_SIGANL_NEWS);
		SetWriteDebug("Real NEWS : Before Send", lpData);

		int nSendDataLen = strlen(lpData);
		if (nSendDataLen != SZ_SIGANL_NEWS)
		{
			SetWriteDebug("Real NEWS : Have Null Data!", "");
			for (int nIndex = 0; nIndex < SZ_SIGANL_NEWS; nIndex++)
			{
				if (lpData[nIndex] == 0)
					lpData[nIndex] = ' ';
			}
		}

		if (m_bSend_DRDS == TRUE)
		{
			bResult = SetRealData((int)TYPE_REAL_NEWS, strProfferID, (LPSTR)lpData);
			SetWriteDebug("Real NEWS : After Send", (bResult == TRUE)?"O":"X");
			
			if ((bCheck_UserKeyword == TRUE) && (bSignalOK == TRUE))
			{
				bResult = SetRealData((int)TYPE_SIGNAL_NEWS, strProfferID, (LPSTR)lpData);
				SetWriteDebug("Signal NEWS : After Send", (bResult == TRUE)?"O":"X");
			}
		}
	}
	LeaveCriticalSection(&m_CriticalSection);
	delete [] pDataBuf;

	return bResult;
}

BOOL CLib_SignalMngApp::GetCheck_News(Signal_NewsInfo stNewsInfo)
{
	SetWriteDebug("Signal NEWS => Data Input", "Normal News");
	
	EnterCriticalSection(&m_CriticalSection);
	
	BOOL bResult = FALSE;
	int nIndex, nSiganlChk = 0;
	
	CString strMsg;
	
	char *pData;
	CString strTime, strCode, strTitle, strProfferID;
	STR_CONVERT(pData, stNewsInfo.stNewsCommon.stNewsKey.sTime, sizeof(stNewsInfo.stNewsCommon.stNewsKey.sTime), strTime);
	STR_CONVERT(pData, stNewsInfo.stNewsCommon.sCode, sizeof(stNewsInfo.stNewsCommon.sCode), strCode);
	STR_CONVERT(pData, stNewsInfo.stNewsCommon.sTitle, sizeof(stNewsInfo.stNewsCommon.sTitle), strTitle);
	STR_CONVERT(pData, stNewsInfo.stNewsCommon.sProfferID, sizeof(stNewsInfo.stNewsCommon.sProfferID), strProfferID);   //# Key
	
	int nBrocastType = BROADCAST_ALL;
	int nCompomentType = COMPOMENT_NORMAL; //# 일반
	SetWriteDebug("Signal NEWS : Before Data Convert", "");
	
	//# Convert Packet!
	SIGANL_NEWS	stNews;
	GetConvertPacket_News("NEWS", stNewsInfo, stNews);
	
	if (strCode.IsEmpty() == FALSE)
	{
		SetWriteDebug("Signal NEWS : Before Filter Check", "");
		if (GetCheck_Filter(m_pstFilterInfo, 
			strTime, strCode, stNewsInfo.stCommon) == TRUE)
		{
			if (theApp.m_pmapSignalSet_News != NULL)
			{
				BOOL bNewsProffer = FALSE;
				if (GetCheck_Keyword(bNewsProffer, strTitle, "", &stNews, nCompomentType) == TRUE)
				{
					nBrocastType |= BROADCAST_SIGNAL | BROADCAST_COMPONENT;
					SetWriteDebug("Signal NEWS => Keyword Check", "성공");
					nSiganlChk = 1;
				}
				else
				{
					//# Signal X!
					SetWriteDebug("Signal NEWS => Keyword Check", "실패");
					
					LeaveCriticalSection(&m_CriticalSection);
					return bResult;
				}
			}
			else
				theApp.SetWriteDebug("Signal NEWS => Reg NEWS Keyword NULL", "");
		}
		else
			SetWriteDebug("Signal NEWS => After Filter Check [X]", "");
	}
	
	char lpData[SZ_SIGANL_NEWS + 1];
	memset(&lpData, 0x00, SZ_SIGANL_NEWS + 1);
	memcpy(&lpData, &stNews, SZ_SIGANL_NEWS);
	SetWriteDebug("Signal NEWS : Before Send DRDS", lpData);
	
	//int nIndex;
	int nSendDataLen = strlen(lpData);
	if (nSendDataLen != SZ_SIGANL_NEWS)
	{
		SetWriteDebug("Signal NEWS : Have Null Data!", "");
		for (nIndex = 0; nIndex < SZ_SIGANL_NEWS; nIndex++)
		{
			if (lpData[nIndex] == 0)
				lpData[nIndex] = ' ';
		}
	}
	
	if (m_bSend_DRDS == TRUE)
	{
		bResult = SetRealData((int)TYPE_SIGNAL_NEWS, strProfferID, (LPSTR)lpData);
		SetWriteDebug("Signal NEWS : After Send DRDS", (bResult == TRUE)?"O":"X");
		
		LeaveCriticalSection(&m_CriticalSection);
		return bResult;
	}
	
	LeaveCriticalSection(&m_CriticalSection);
	return bResult;
}

BOOL CLib_SignalMngApp::GetCheck_News(LPSTR &sRealData, int nLen)
{
	SetWriteDebug("Signal NEWS => Data Input", "공시 News");
	
	EnterCriticalSection(&m_CriticalSection);
	
	BOOL bResult = FALSE;
	int nIndex, nSiganlChk = 0;
	
	CString strMsg;
	Signal_AllNewsInfo	stNewsAllInfo;
	memset(&stNewsAllInfo, 0x00, sizeof(Signal_AllNewsInfo));
	memcpy(&stNewsAllInfo, sRealData, sizeof(Signal_AllNewsInfo));
	
	char *pData;
	CString strTime, strCode, strTitle, strProfferID;
	STR_CONVERT(pData, stNewsAllInfo.stNewsInfo.stNewsCommon.stNewsKey.sTime, sizeof(stNewsAllInfo.stNewsInfo.stNewsCommon.stNewsKey.sTime), strTime);
	STR_CONVERT(pData, stNewsAllInfo.stNewsInfo.stNewsCommon.sCode, sizeof(stNewsAllInfo.stNewsInfo.stNewsCommon.sCode), strCode);
	STR_CONVERT(pData, stNewsAllInfo.stNewsInfo.stNewsCommon.sTitle, sizeof(stNewsAllInfo.stNewsInfo.stNewsCommon.sTitle), strTitle);
	STR_CONVERT(pData, stNewsAllInfo.stNewsInfo.stNewsCommon.sProfferID, sizeof(stNewsAllInfo.stNewsInfo.stNewsCommon.sProfferID), strProfferID);   //# Key
	
	int nBrocastType = BROADCAST_ALL;
	int nCompomentType = COMPOMENT_NORMAL; //# 일반
	SetWriteDebug("Signal NEWS : Before Data Convert", "");
	
	//# Convert Packet!
	SIGANL_NEWS	stNews;
	GetConvertPacket_News("NEWS", stNewsAllInfo.stNewsInfo, stNews);
	
	int nDataStartPos = sizeof(Signal_AllNewsInfo);
	if (strCode.IsEmpty() == FALSE)
	{
		SetWriteDebug("Signal NEWS : Before Filter Check", "");
		if (GetCheck_Filter(m_pstFilterInfo, 
			strTime, strCode, stNewsAllInfo.stNewsInfo.stCommon) == TRUE)
		{
			if (theApp.m_pmapSignalSet_News != NULL)
			{
				BOOL bNewsProffer = FALSE;
				if (strProfferID == "10")   //# 공시
					bNewsProffer = TRUE;
				
				BOOL bKeyWordCheck = FALSE;
				if (m_pstFilterInfo->nNewsType == 0)        //# 전체
					bKeyWordCheck = TRUE;
				else if (m_pstFilterInfo->nNewsType == 1)    //# 뉴스
				{
					if (bNewsProffer == FALSE)
						bKeyWordCheck = TRUE;
				}
				else if (m_pstFilterInfo->nNewsType == 2)    //# 공시
				{
					if (bNewsProffer == TRUE)
						bKeyWordCheck = TRUE;
				}
				
				if (bKeyWordCheck == TRUE)
				{
					int nCodeLen, nCaseLen, nBodyLen;
					CString strCodeList, strCaseList, strBody;
					
					//# Code Data : Type(1) + Code(6)
					//# Type : C-대표코드, T-Title, B-Body, K-연관
					INT_CONVERT(pData, stNewsAllInfo.sCodeLen, sizeof(stNewsAllInfo.sCodeLen), nCodeLen);
					STR_CONVERT(pData, sRealData + nDataStartPos, nCodeLen, strCodeList);
					nDataStartPos += nCodeLen;
					
					INT_CONVERT(pData, stNewsAllInfo.sCaseLen, sizeof(stNewsAllInfo.sCaseLen), nCaseLen);
					STR_CONVERT(pData, sRealData + nDataStartPos, nCaseLen, strCaseList);
					nDataStartPos += nCaseLen;
					
					INT_CONVERT(pData, stNewsAllInfo.sBodyLen, sizeof(stNewsAllInfo.sBodyLen), nBodyLen);
					STR_CONVERT(pData, sRealData + nDataStartPos, nBodyLen, strBody);
					if (GetCheck_Keyword(bNewsProffer, strTitle, strBody, &stNews, nCompomentType) == TRUE)
					{
						nBrocastType |= BROADCAST_SIGNAL | BROADCAST_COMPONENT;
						SetWriteDebug("Signal NEWS => Keyword Check", "성공");
						nSiganlChk = 1;
					}
					else
					{
						//# Signal X!
						SetWriteDebug("Signal NEWS => Keyword Check", "실패");
						LeaveCriticalSection(&m_CriticalSection);
						return bResult;
					}
					
					
					/*
					{
					CString strPoint;
					STR_CONVERT(pData, stNewsAllInfo.stNews.sNewsPoint, sizeof(stNewsAllInfo.stNews.sNewsPoint), strPoint);
					
					  GetPointToImgIndex((int)TYPE_SIGNAL_NEWS, atoi(strPoint), stNews.sSignalImageIndex);
					  memcpy(stNews.sSignalDegree, strPoint.GetBuffer(0), sizeof(stNews.sSignalDegree));
					  }
					*/
				}
			}
			else
				theApp.SetWriteDebug("Data Input => Reg NEWS Keyword NULL", "");
		}
		else
			SetWriteDebug("Signal NEWS => After Filter Check [X]", "");
	}
	
	char lpData[SZ_SIGANL_NEWS + 1];
	memset(&lpData, 0x00, SZ_SIGANL_NEWS + 1);
	memcpy(&lpData, &stNews, SZ_SIGANL_NEWS);
	SetWriteDebug("Signal NEWS : Before Send DRDS", lpData);
	
	//int nIndex;
	int nSendDataLen = strlen(lpData);
	if (nSendDataLen != SZ_SIGANL_NEWS)
	{
		SetWriteDebug("Signal NEWS : Have Null Data!", "");
		for (nIndex = 0; nIndex < SZ_SIGANL_NEWS; nIndex++)
		{
			if (lpData[nIndex] == 0)
				lpData[nIndex] = ' ';
		}
	}
	
	if (m_bSend_DRDS == TRUE)
	{
		bResult = SetRealData((int)TYPE_SIGNAL_NEWS, strProfferID, (LPSTR)lpData);
		SetWriteDebug("Signal NEWS : After Send DRDS", (bResult == TRUE)?"O":"X");
		
		LeaveCriticalSection(&m_CriticalSection);
		return bResult;
		
		
		/*
		int nCodeCnt;
		CString strCodeList;
		INT_CONVERT(pData, stNewsAllInfo.sCodeCnt, sizeof(stNewsAllInfo.sCodeCnt), nCodeCnt);
		STR_CONVERT(pData, sRealData + nDataStartPos, 8 * nCodeCnt, strCodeList);
		nDataStartPos += 8 * nCodeCnt;
		//# Code Data : Code(7) + Type(1)
		//# Type : C-대표, T-Title, B-포함, K-연관
		*/
		
		/*
		strCodeList = "049370 C";
		strCodeList += "035710 B035830 B036030 B040740 B041510 B080440 B000800 B";
		strCodeList += "001130 K004790 K009410 K012330 K017050 K020710 K030520 K";
		nCodeCnt = 15;
		*/
		
		
		/*
		CString strData;
		strData.Format("Cnt : %d, %s", nCodeCnt, strCodeList);
		SetWriteDebug("Signal NEWS : 연관 + 포함 Data", strData);
		
		  CString strCodeType;
		  for (int nIndex_Code = 0; nIndex_Code < nCodeCnt; nIndex_Code++)
		  {
		  strCodeType = strCodeList.Mid(nIndex_Code * 8 + 7, 1);
		  
			if (strCodeType == "C")   //# 대표
			{
			if (nSiganlChk == 1)
			continue;
			else
			{
			memcpy(&stNews.sSignalDegree, (CString)NEWS_CODETYPE_RPN, sizeof(stNews.sSignalDegree));
			GetPointToImgIndex((int)TYPE_SIGNAL_NEWS, (int)NEWS_CODETYPE_RPN, stNews.sSignalImageIndex);
			}
			}
			else if ((strCodeType == "B") || (strCodeType == "K") || (strCodeType == "T"))  //# 포함, 연관
			{
			strCode = strCodeList.Mid(nIndex_Code * 8, 6);
			memcpy(&stNews.sCode, strCode.GetBuffer(0), sizeof(stNews.sCode));
			
			  if (nSiganlChk == 0)
			  {
			  if (strCodeType == "B") //# 포함
			  {
			  memcpy(&stNews.sSignalDegree, (CString)NEWS_CODETYPE_C_INS, sizeof(stNews.sSignalDegree));
			  GetPointToImgIndex((int)TYPE_SIGNAL_NEWS, (int)NEWS_CODETYPE_C_INS, stNews.sSignalImageIndex);
			  }
			  else if (strCodeType == "K")  //# 연관
			  {
			  memcpy(&stNews.sSignalDegree, (CString)NEWS_CODETYPE_RLT, sizeof(stNews.sSignalDegree));
			  GetPointToImgIndex((int)TYPE_SIGNAL_NEWS, (int)NEWS_CODETYPE_RLT, stNews.sSignalImageIndex);
			  }
			  else if (strCodeType == "T")  //# Title
			  {
			  memcpy(&stNews.sSignalDegree, (CString)NEWS_CODETYPE_T_INS, sizeof(stNews.sSignalDegree));
			  GetPointToImgIndex((int)TYPE_SIGNAL_NEWS, (int)NEWS_CODETYPE_T_INS, stNews.sSignalImageIndex);
			  }
			  }
			  }
			  
				memset(&lpData, 0x00, SZ_SIGANL_NEWS + 1);
				memcpy(&lpData, &stNews, SZ_SIGANL_NEWS);
				
				  for (nIndex = 0; nIndex < SZ_SIGANL_NEWS; nIndex++)
				  {
				  if (lpData[nIndex] == 0)
				  lpData[nIndex] = ' ';
				  }
				  
					nBrocastType = BROADCAST_COMPONENT;
					nCompomentType = COMPOMENT_ALRAM;
					
					  
						bResult = SetRealData((int)TYPE_SIGNAL_NEWS, strProfferID, (LPSTR)lpData);
						/*
						if (m_libDrds.SignalMng_SetNewRealData((LPSTR)lpData, SZ_SIGANL_NEWS, nCompomentType, nBrocastType) > -1)
						SetWriteDebug("Signal NEWS : Send DRDS(연관 + 포함) => O", strCode);
						else 
						SetWriteDebug("Signal NEWS : Send DRDS(연관 + 포함) => X", strCode);
		*/
		//}
	}
	
	LeaveCriticalSection(&m_CriticalSection);
	
	return bResult;
}

BOOL CLib_SignalMngApp::GetCheck_CaseCodeToPoint(CString strCaseCodeList, SIGANL_NEWS *pstNews)
{
	BOOL bResult = FALSE;
	
	if (m_pmapSignalSet_News == NULL)
		return bResult;

	{
		CString szMsg;
		if(strCaseCodeList.IsEmpty())
			szMsg.Format("상황문없음.");
		else
			szMsg.Format("상황문=%s", strCaseCodeList);

	}

	CString strMsg;
	int nSignalDegree = 0;
	if (strCaseCodeList.IsEmpty() == FALSE)
	{
		CString strCaseCode;
		SetSignalData	*pstSetSignalData;
		int nSPos = 0;
		for (int nIndex = 0; nIndex < strCaseCodeList.GetLength() / 6; nIndex++)
		{
			strCaseCode = strCaseCodeList.Mid(nSPos, 6);
			if (m_pmapSignalSet_News->Lookup(strCaseCode, (void*&)pstSetSignalData) == TRUE)
			{
				nSignalDegree += pstSetSignalData->nPoint;
				
				strMsg.Format("%s, Point : %d", pstSetSignalData->strTitle, pstSetSignalData->nPoint);
				theApp.SetWriteDebug("Result : CaseCode Data", strMsg);
				bResult = TRUE;
			}
			
			nSPos += 6;
		}
	}
	
	memset(&pstNews->sSignalDegree, 0x00, sizeof(pstNews->sSignalDegree));
	if (bResult == TRUE)
	{
		CString strSignalDegree;
		strSignalDegree.Format("%d", nSignalDegree);
		memcpy(&pstNews->sSignalDegree, strSignalDegree.GetBuffer(0), strSignalDegree.GetLength());
	}
	
	return bResult;
}

BOOL CLib_SignalMngApp::GetCheck_Keyword(BOOL bNewsProffer, CString strTitle, CString strBody, 
										 SIGANL_NEWS *pstNews, int &nCompomentType)
{
	BOOL bResult = FALSE;
	
	if (m_pmapSignalSet_News == NULL)
		return bResult;
	
	POSITION pos;
	CString strKey, strData;
	int nFindPos_TS, nFindPos_TV, nFindPos_BS, nFindPos_BV;
	SetSignalData	*pstSetSignalData;
	for (pos = m_pmapSignalSet_News->GetStartPosition(); pos != NULL;)
	{
		m_pmapSignalSet_News->GetNextAssoc(pos, strKey, (void*&)pstSetSignalData);
		
		if (bNewsProffer == TRUE)     //# 공시
		{
			nFindPos_BS = strBody.Find(pstSetSignalData->stKeywordData.strName_S);
			if (pstSetSignalData->stKeywordData.strName_V.IsEmpty() == 0)
				nFindPos_BV = strBody.Find(pstSetSignalData->stKeywordData.strName_V);
			else
				nFindPos_BV = 0;
		}
		
		nFindPos_TS = strTitle.Find(pstSetSignalData->stKeywordData.strName_S);
		if (pstSetSignalData->stKeywordData.strName_V.IsEmpty() == 0)
			nFindPos_TV = strTitle.Find(pstSetSignalData->stKeywordData.strName_V);
		else
			nFindPos_TV = 0;
		
		if (((nFindPos_TS > 0) && (nFindPos_TV >= 0)) || 
			((nFindPos_BS > 0) && (nFindPos_BV >= 0)))
		{
			if (pstSetSignalData->nState == 0)
				bResult = FALSE;
			else
			{
				bResult = TRUE;
				
				if (pstSetSignalData->nSend_Signal == 1)
					nCompomentType |= COMPOMENT_ALRAM;   //# 신호등
				
				if (pstSetSignalData->nSend_Order == 1)
					nCompomentType |= COMPOMENT_ORDER;   //# 주문
				
				if (pstSetSignalData->nSend_Ticker == 1)
					nCompomentType |= COMPOMENT_TICKER;  //# 티커
				
				GetPointToImgIndex((int)TYPE_SIGNAL_NEWS, pstSetSignalData->nPoint, pstNews->sSignalImageIndex);
				
				strData.Format("%02d", pstSetSignalData->nPoint);
				memcpy(pstNews->sSignalDegree, strData.GetBuffer(0), sizeof(pstNews->sSignalDegree));
			}
			
			break;
		}
	}
	
	return bResult;
}

BOOL CLib_SignalMngApp::GetConvertPacket_News(CString strType, Signal_NewsInfo stNewsInfo,
											  SIGANL_NEWS	&stNews)
{
	memset(&stNews, 0x20, SZ_SIGANL_NEWS);
	
	CString strKey, strData;
	strData.Format("%d", SZ_SIGANL_NEWS);
	memcpy(&stNews.stHEAD.sSize, strData.GetBuffer(0), strData.GetLength());
	memcpy(&stNews.stHEAD.sType, strType.GetBuffer(0), strType.GetLength());
	
	memcpy(&stNews.stNewsKey, &stNewsInfo.stNewsCommon.stNewsKey, sizeof(stNewsInfo.stNewsCommon.stNewsKey));
	
	GetPointToImgIndex((int)TYPE_SIGNAL_NEWS, atoi(stNewsInfo.stNewsCommon.sNewsPoint), stNews.sSignalImageIndex);
	memcpy(&stNews.sSignalDegree, &stNewsInfo.stNewsCommon.sNewsPoint, sizeof(stNewsInfo.stNewsCommon.sNewsPoint));
	
	//memcpy(&stNews.sGoodPoint, &stNewsInfo.stNewsCommon.sGoodPoint, sizeof(stNewsInfo.stNewsCommon.sGoodPoint));
	//memcpy(&stNews.sBadPoint, &stNewsInfo.stNewsCommon.sBadPoint, sizeof(stNewsInfo.stNewsCommon.sBadPoint));
	
	memcpy(&stNews.sCode, &stNewsInfo.stNewsCommon.sCode, CODE_LEN);
	memcpy(&stNews.sName, &stNewsInfo.stNewsCommon.sName, CODE_NAME_LEN);
	
	memcpy(&stNews.sProfferID, &stNewsInfo.stNewsCommon.sProfferID, sizeof(stNewsInfo.stNewsCommon.sProfferID));
	memcpy(&stNews.sProffer, &stNewsInfo.stNewsCommon.sProffer, sizeof(stNewsInfo.stNewsCommon.sProffer));
	memcpy(&stNews.sProfferTypeID, &stNewsInfo.stNewsCommon.sProfferTypeID, sizeof(stNewsInfo.stNewsCommon.sProfferTypeID));
	memcpy(&stNews.sProfferType, &stNewsInfo.stNewsCommon.sProfferType, sizeof(stNewsInfo.stNewsCommon.sProfferType));
	memcpy(&stNews.sTitle, &stNewsInfo.stNewsCommon.sTitle, sizeof(stNewsInfo.stNewsCommon.sTitle) - 1);
	
	//# Comm Add : Danny(2006.10)
	memcpy(&stNews.stCommon.sMarket, &stNewsInfo.stCommon.sMarket, sizeof(stNewsInfo.stCommon.sMarket));
	memcpy(&stNews.stCommon.sCode, &stNewsInfo.stNewsCommon.sCode, CODE_LEN);
	memcpy(&stNews.stCommon.sPrice, &stNewsInfo.stCommon.sPrice, sizeof(stNewsInfo.stCommon.sPrice));
	memcpy(&stNews.stCommon.sSign, &stNewsInfo.stCommon.sSign, sizeof(stNewsInfo.stCommon.sSign));
	memcpy(&stNews.stCommon.sChage, &stNewsInfo.stCommon.sChage, sizeof(stNewsInfo.stCommon.sChage));
	memcpy(&stNews.stCommon.sChgRate, &stNewsInfo.stCommon.sChgRate, sizeof(stNewsInfo.stCommon.sChgRate));
	memcpy(&stNews.stCommon.sVolume, &stNewsInfo.stCommon.sVolume, sizeof(stNewsInfo.stCommon.sVolume));
	memcpy(&stNews.stCommon.sVolumeRect, &stNewsInfo.stCommon.sVolumeRect, sizeof(stNewsInfo.stCommon.sVolumeRect));
	//memcpy(&stNews.stCommon.sAmount, &stNewsInfo.stCommon.sAmount, sizeof(stNewsInfo.stCommon.sAmount));
	
	return TRUE;
}

int CLib_SignalMngApp::GetCheck_Condition(LPSTR &sRealData, int nLen, CString sPBID, BOOL bUseFilter /*= TRUE*/)
{
	EnterCriticalSection(&m_CriticalSection);
	
	int nResult = 0;
	
	char *pData_Pos;
	
	char* pData = new char[nLen + 1];
	::memset(pData, 0x00, nLen + 1);
	::memcpy(pData, sRealData, nLen);
	pData_Pos = pData;
	
	Signal_ConditionInfo_Head		stConditionInfo_Head;
	::memset(&stConditionInfo_Head, 0x00, SZ_SIGNAL_CINDITIONINFO_HEAD);
	::memcpy(&stConditionInfo_Head, pData, SZ_SIGNAL_CINDITIONINFO_HEAD);
	
	CString strSignalType, strAlertNo, strState;
	int nDataLen, nDataCnt;//, nCompress;
	
	char *pBuff;
/*	pBuff = new char[sizeof(stConditionInfo_Head.stHeadInfo.sSize) + 1];
	::memset(pBuff, 0x00, sizeof(stConditionInfo_Head.stHeadInfo.sSize) + 1);
	::memcpy(pBuff, stConditionInfo_Head.stHeadInfo.sSize, sizeof(stConditionInfo_Head.stHeadInfo.sSize));
	nDataLen = atoi(pBuff);
	delete [] pBuff;
*/	//성조 유진

//확인필
//nDataLen = SZ_SIGNAL_CINDITIONINFO_HEAD + SZ_SIGNAL_CONDITIONINFO;
	nDataLen = SZ_SIGNAL_CINDITIONINFO_HEAD + SZ_SIGNAL_CONDITIONINFO2;//nLen + 1;
/*	
	CString strMsg;
	strMsg.Format("Condition Data Data : %d , Length : %d", nDataLen, nLen);
	if (nDataLen != nLen)
	{
	theApp.SetWriteDebug("[ERROR] Data Length (X)", strMsg);
	theApp.SetWriteDebug("[ERROR] Data Length (X)", sRealData);
	
	  delete [] pData_Pos;
	  LeaveCriticalSection(&m_CriticalSection);
	  return nResult;
	  }
*/	
	
/*	pBuff = new char[sizeof(stConditionInfo_Head.stHeadInfo.sCompress) + 1];
	::memset(pBuff, 0x00, sizeof(stConditionInfo_Head.stHeadInfo.sCompress) + 1);
	::memcpy(pBuff, stConditionInfo_Head.stHeadInfo.sCompress, sizeof(stConditionInfo_Head.stHeadInfo.sCompress));
	nCompress = atoi(pBuff);
	delete [] pBuff;
*///성조 유진	
	pBuff = new char[sizeof(stConditionInfo_Head.sStatus) + 1];
	::memset(pBuff, 0x00, sizeof(stConditionInfo_Head.sStatus) + 1);
	::memcpy(pBuff, stConditionInfo_Head.sStatus, sizeof(stConditionInfo_Head.sStatus));
	strState = (CString)pBuff;
	delete [] pBuff;
	
	if (strState != "S")
	{
		SetWriteDebug("Condition Signal! (비정상)", "");
		
		// 2009.02.22 -hidden- 실시간이 정상이 아닌경우
		pBuff = new char[sizeof(stConditionInfo_Head.sAlertNum) + 1];
		::memset(pBuff, 0x00, sizeof(stConditionInfo_Head.sAlertNum) + 1);
		::memcpy(pBuff, stConditionInfo_Head.sAlertNum, sizeof(stConditionInfo_Head.sAlertNum));
		strAlertNo = (CString)pBuff;
		delete [] pBuff;

		SIGNAL_CONDITION	stCondNull;
		memset(&stCondNull, 0x20, sizeof(stCondNull));
		memcpy(stCondNull.sJobFlag, strState, 1);
		// 한투 조건검색 서버 분리로 인한 서버 분류, sPBID 작업
		if(bUseFilter)
			SetRealData((int)TYPE_SIGNAL_SYS, strAlertNo, (LPSTR)&stCondNull, sPBID);    // 9999
		else
			SetRealData((int)TYPE_SIGNAL_ALERT, strAlertNo, (LPSTR)&stCondNull, sPBID);

		delete [] pData_Pos;
		LeaveCriticalSection(&m_CriticalSection);
		return nResult;
	}
	
	pBuff = new char[sizeof(stConditionInfo_Head.sDataCnt) + 1];
	//::memset(pBuff, 0x00, sizeof(pBuff) + 1);
	pBuff[sizeof(pBuff)] = 0;
	::memcpy(pBuff, stConditionInfo_Head.sDataCnt, sizeof(stConditionInfo_Head.sDataCnt));
	nDataCnt = atoi(pBuff);
	delete [] pBuff;
	
	
	pBuff = new char[sizeof(stConditionInfo_Head.sType) + 1];
	::memset(pBuff, 0x00, sizeof(stConditionInfo_Head.sType) + 1);
	::memcpy(pBuff, stConditionInfo_Head.sType, sizeof(stConditionInfo_Head.sType));
	strSignalType = (CString)pBuff;
	delete [] pBuff;
	
	pBuff = new char[sizeof(stConditionInfo_Head.sAlertNum) + 1];
	::memset(pBuff, 0x00, sizeof(stConditionInfo_Head.sAlertNum) + 1);
	::memcpy(pBuff, stConditionInfo_Head.sAlertNum, sizeof(stConditionInfo_Head.sAlertNum));
	strAlertNo = (CString)pBuff;
	delete [] pBuff;
	
	CString strTemp;
	strTemp.Format("%d", nDataCnt);
	if (strSignalType == "S")
		SetWriteDebug("Signal SYS : Data Count", strTemp);
	else if (strSignalType == "U")
		SetWriteDebug("Signal Alert : Data Count", strTemp);
	else
	{
		SetWriteDebug("Other Signal Type!", "");
		
		delete [] pData_Pos;
		LeaveCriticalSection(&m_CriticalSection);
		return nResult;
	}
	
	pData = &pData[SZ_SIGNAL_CINDITIONINFO_HEAD];
	nDataLen = nDataLen - SZ_SIGNAL_CINDITIONINFO_HEAD;

	CString strCode;
	Signal_ConditionInfo		stConditionInfo;

	Signal_ConditionInfo2		stConditionInfo2;
	for (int nIndex = 0; nIndex < nDataCnt; nIndex++)
	{
		memset(&stConditionInfo2, 0x00, SZ_SIGNAL_CONDITIONINFO2);
		memcpy(&stConditionInfo2, pData, SZ_SIGNAL_CONDITIONINFO2);
// 		memset(&stConditionInfo, 0x00, SZ_SIGNAL_CONDITIONINFO);
// 		memcpy(&stConditionInfo, pData, SZ_SIGNAL_CONDITIONINFO);
		memset(&stConditionInfo, 0x00, SZ_SIGNAL_CONDITIONINFO);	
	
		//뉴스 구조체 변경되어서 추가된 작업
		memcpy(&stConditionInfo.sJobFlag, &stConditionInfo2.sJobFlag, sizeof(stConditionInfo.sJobFlag));
		memcpy(&stConditionInfo.sCode, stConditionInfo2.sCode, sizeof(stConditionInfo.sCode));
		memcpy(&stConditionInfo.sName, stConditionInfo2.sName, sizeof(stConditionInfo.sName));
		memcpy(&stConditionInfo.sEntryCnt, stConditionInfo2.sEntryCnt, sizeof(stConditionInfo.sEntryCnt));		

		memcpy(&stConditionInfo.stCommon.sManagement, &stConditionInfo2.stCommon.sManagement, sizeof(stConditionInfo.stCommon.sManagement));
		memcpy(&stConditionInfo.stCommon.sControl,    &stConditionInfo2.stCommon.sControl, sizeof(stConditionInfo.stCommon.sControl));
		memcpy(&stConditionInfo.stCommon.sTradeStop,  &stConditionInfo2.stCommon.sTradeStop, sizeof(stConditionInfo.stCommon.sTradeStop));
		memcpy(&stConditionInfo.stCommon.sInvestAttention, &stConditionInfo2.stCommon.sInvestAttention, sizeof(stConditionInfo.stCommon.sInvestAttention));
		memcpy(&stConditionInfo.stCommon.sInsincerityNotice, &stConditionInfo2.stCommon.sInsincerityNotice, sizeof(stConditionInfo.stCommon.sInsincerityNotice));
		memcpy(&stConditionInfo.stCommon.sConfidenceLimit, &stConditionInfo2.stCommon.sManagement, sizeof(stConditionInfo.stCommon.sManagement));
		memcpy(&stConditionInfo.stCommon.sCodeNotice, &stConditionInfo2.stCommon.sCodeNotice, sizeof(stConditionInfo.stCommon.sCodeNotice));
		memcpy(&stConditionInfo.stCommon.sJungriMeme, &stConditionInfo2.stCommon.sJungriMeme, sizeof(stConditionInfo.stCommon.sJungriMeme));
		memcpy(&stConditionInfo.stCommon.sMarket, &stConditionInfo2.stCommon.sMarket, sizeof(stConditionInfo.stCommon.sMarket));
		memcpy(&stConditionInfo.stCommon.sDepositMoney, stConditionInfo2.stCommon.sDepositMoney, sizeof(stConditionInfo.stCommon.sDepositMoney));
		memcpy(&stConditionInfo.stCommon.sPrice, stConditionInfo2.stCommon.sPrice, sizeof(stConditionInfo.stCommon.sPrice));
		memcpy(&stConditionInfo.stCommon.sSign, &stConditionInfo2.stCommon.sSign, sizeof(stConditionInfo.stCommon.sSign));
		memcpy(&stConditionInfo.stCommon.sChage, stConditionInfo2.stCommon.sChage, sizeof(stConditionInfo.stCommon.sChage));
		memcpy(&stConditionInfo.stCommon.sChgRate, stConditionInfo2.stCommon.sChgRate, sizeof(stConditionInfo.stCommon.sChgRate));
		memcpy(&stConditionInfo.stCommon.sVolume, stConditionInfo2.stCommon.sVolume, sizeof(stConditionInfo.stCommon.sVolume));
		memcpy(&stConditionInfo.stCommon.sVolumeRect, stConditionInfo2.stCommon.sVolumeRect, sizeof(stConditionInfo.stCommon.sVolumeRect));
		memcpy(&stConditionInfo.stCommon.sAmount, stConditionInfo2.stCommon.sAmount, sizeof(stConditionInfo.stCommon.sAmount));
		//

		memcpy(&stConditionInfo.stCommon.sResultField, stConditionInfo2.stCommon.sResultField, sizeof(stConditionInfo.stCommon.sResultField));

		pBuff = new char[sizeof(stConditionInfo.sCode) + 1];
		memset(pBuff, 0x00, sizeof(stConditionInfo.sCode) + 1);
		memcpy(pBuff, stConditionInfo.sCode, sizeof(stConditionInfo.sCode));
		strCode = (CString)pBuff;
		delete [] pBuff;
		
		
		SIGNAL_CONDITION	stCondition;
		if (bUseFilter == TRUE)
		{
			BOOL bSignalOK = TRUE;
			SetWriteDebug("Signal Condition : Before Filter Check", pData);
			bSignalOK = GetCheck_Filter(m_pstFilterInfo, (CString)stConditionInfo_Head.sTime, strCode, 
				stConditionInfo.stCommon);
			
			if (bSignalOK == FALSE)
			{
				strTemp.Format("Signal %s => After Filter Check [X]", strSignalType);
				SetWriteDebug(strTemp, "");
				
				delete [] pData_Pos;
				LeaveCriticalSection(&m_CriticalSection);
				return nResult;
			}
			
			//# Convert Packet!
			if (GetConvertPacket_Condition(TYPE_SIGNAL_SYS, strAlertNo, stConditionInfo, stCondition) == TRUE)
			{
				SetWriteDebug("Signal Condition : After Data Convert", "");
				
				if (m_bSend_DRDS == TRUE)
				{
					memcpy(&stCondition.sDate, &stConditionInfo_Head.sDate, DATE_LEN);
					memcpy(&stCondition.sTime, &stConditionInfo_Head.sTime, TIME_LEN);
					
					SetWriteDebug("Signal Condition : Before Send DRDS", "");
					
					char lpData[SZ_SIGNAL_CONDITION + 1];
					memset(&lpData, 0x00, SZ_SIGNAL_CONDITION + 1);
					memcpy(&lpData, &stCondition, SZ_SIGNAL_CONDITION);
					
					int nSendDataLen = strlen(lpData);
					if (nSendDataLen != SZ_SIGNAL_CONDITION)
					{
						SetWriteDebug("Signal Condition : Have Null Data", lpData);
						
						for (int i = 0; i < SZ_SIGNAL_CONDITION; i++)
						{
							if (lpData[i] == 0)
								lpData[i] = ' ';
						}
					}
					
					int nBrocastType = BROADCAST_ALL | BROADCAST_COMPONENT | BROADCAST_SIGNAL;
					
					SetWriteDebug("Signal Condition : DRDS Send Data", lpData);
					BOOL bSend_Flag = FALSE;
					// 한투 조건검색 서버 분리로 인한 서버 분류, sPBID 작업
					bSend_Flag = SetRealData((int)TYPE_SIGNAL_SYS, strAlertNo, (LPSTR)lpData, sPBID);    //# SYS Signal!
					SetWriteDebug("Signal Condition : After Send DRDS", (bSend_Flag == TRUE)?"O":"X");
				}
				nResult++;
			}
			
			strTemp.Format("Signal %s => After Filter Check [O]", strSignalType);
			SetWriteDebug(strTemp, "");
		}
		else
		{
			if (GetConvertPacket_Condition(TYPE_SIGNAL_ALERT, strAlertNo, stConditionInfo, stCondition) == TRUE)
			{
				SetWriteDebug("Signal Condition : After Data Convert", "");
				
				if (m_bSend_DRDS == TRUE)
				{
					memcpy(&stCondition.sDate, &stConditionInfo_Head.sDate, DATE_LEN);
					memcpy(&stCondition.sTime, &stConditionInfo_Head.sTime, TIME_LEN);
					
					SetWriteDebug("Signal Condition : Before Send DRDS", "");
					
					char lpData[SZ_SIGNAL_CONDITION + 1];
					memset(&lpData, 0x00, SZ_SIGNAL_CONDITION + 1);
					memcpy(&lpData, &stCondition, SZ_SIGNAL_CONDITION);
					
					int nSendDataLen = strlen(lpData);
					if (nSendDataLen != SZ_SIGNAL_CONDITION)
					{
						SetWriteDebug("Signal Condition : Have Null Data", lpData);
						
						for (int i = 0; i < SZ_SIGNAL_CONDITION; i++)
						{
							if (lpData[i] == 0)
								lpData[i] = ' ';
						}
					}
					
					SetWriteDebug("Signal Condition : DRDS Send Data", lpData);
					BOOL bSend_Flag = FALSE;
					// 한투 조건검색 서버 분리로 인한 서버 분류, sPBID 작업
					bSend_Flag = SetRealData((int)TYPE_SIGNAL_ALERT, strAlertNo, (LPSTR)lpData, sPBID);
					SetWriteDebug("Signal Condition : After Send DRDS", (bSend_Flag == TRUE)?"O":"X");
				}
				nResult++;
			}
		}
	
//확인필요 
		pData = &pData[SZ_SIGNAL_CONDITIONINFO2];
		nDataLen = nDataLen - SZ_SIGNAL_CONDITIONINFO2;
	}

	delete [] pData_Pos;
	LeaveCriticalSection(&m_CriticalSection);

	return nResult;
}

BOOL CLib_SignalMngApp::GetConvertPacket_Condition(int nSignalType, CString strKey, Signal_ConditionInfo	stConditionInfo,
												   SIGNAL_CONDITION	&stCondition)
{
	strKey.TrimLeft(" ");
	strKey.TrimRight(" ");
	memset(&stCondition, 0x20, SZ_SIGNAL_CONDITION);
	
	CString strData, strTitle, strPoint, strOrderType, strSignalType;
	if (nSignalType == (int)TYPE_SIGNAL_SYS)
	{
		if (m_pmapSignalSet_Sys == NULL)
			return FALSE;
		
		SetSignalData	*pstSetSignalData;
		if (m_pmapSignalSet_Sys->Lookup(strKey, (void*&)pstSetSignalData) == FALSE)
		{
			SetWriteDebug("[ERROR]", "Not Find SYS Data : " + strKey);
			return FALSE;
		}
		else
		{
			strTitle = pstSetSignalData->strTitle;
			
			strPoint.Format("%d", pstSetSignalData->nPoint);
			strOrderType.Format("%d", pstSetSignalData->nSend_Order);
			
			if(pstSetSignalData->nState == 0)
				return FALSE;
			
			GetPointToImgIndex((int)TYPE_SIGNAL_ALERT, pstSetSignalData->nPoint, stCondition.sSignalImageIndex);
		}
		
		strSignalType = "SYS";
	}
	else if (nSignalType == (int)TYPE_SIGNAL_ALERT)
	{
		if (m_pmapCondition == NULL)
			return FALSE;
		
		strPoint = "0";
		strOrderType = "0";
		GetPointToImgIndex((int)TYPE_SIGNAL_ALERT, 0, stCondition.sSignalImageIndex);
		
		ConditionData	*pstConditionData;
		if (m_pmapCondition->Lookup(strKey, (CObject*&)pstConditionData) == FALSE)
		{
			SetWriteDebug("[ERROR] - 사용자등록조건", "Not Find Alert Data : " + strKey);
			strTitle = "사용자 등록 조건";
		}
		else
		{
			strTitle = pstConditionData->strTitle;
			strPoint.Format("%d", pstConditionData->nBasePoint);
			
			GetPointToImgIndex((int)TYPE_SIGNAL_ALERT, pstConditionData->nBasePoint, stCondition.sSignalImageIndex);
		}
		
		strSignalType = "USER";
	}
	
	strData.Format("%d", SZ_SIGNAL_CONDITION);
//	memcpy(&stCondition.stHEAD.sSize, strData.GetBuffer(0), strData.GetLength());
//	memcpy(&stCondition.stHEAD.sType, strSignalType.GetBuffer(0), strSignalType.GetLength());
	
	memcpy(&stCondition.sCode, &stConditionInfo.sCode, CODE_LEN);
	memcpy(&stCondition.sName, &stConditionInfo.sName, CODE_NAME_LEN);
	memcpy(&stCondition.sJobFlag, &stConditionInfo.sJobFlag, sizeof(stConditionInfo.sJobFlag));
	memcpy(&stCondition.sEntryCnt, &stConditionInfo.sEntryCnt, sizeof(stConditionInfo.sEntryCnt));
	
	memcpy(&stCondition.sSignalDegree, strPoint.GetBuffer(0), strPoint.GetLength());
	memcpy(&stCondition.sOrderType, strOrderType.GetBuffer(0), strOrderType.GetLength());
	memcpy(&stCondition.sSignalKey, strKey.GetBuffer(0), strKey.GetLength());
	memcpy(&stCondition.sSignalName, strTitle.GetBuffer(0), strTitle.GetLength());
	
	//# Comm Add : Danny(2006.10)
	memcpy(&stCondition.stCommon.sMarket, &stConditionInfo.stCommon.sMarket, sizeof(stConditionInfo.stCommon.sMarket));
	memcpy(&stCondition.stCommon.sCode, &stConditionInfo.sCode, CODE_LEN);
	memcpy(&stCondition.stCommon.sPrice, &stConditionInfo.stCommon.sPrice, sizeof(stConditionInfo.stCommon.sPrice));
	memcpy(&stCondition.stCommon.sSign, &stConditionInfo.stCommon.sSign, sizeof(stConditionInfo.stCommon.sSign));
	memcpy(&stCondition.stCommon.sChage, &stConditionInfo.stCommon.sChage, sizeof(stConditionInfo.stCommon.sChage));
	memcpy(&stCondition.stCommon.sChgRate, &stConditionInfo.stCommon.sChgRate, sizeof(stConditionInfo.stCommon.sChgRate));
	memcpy(&stCondition.stCommon.sVolume, &stConditionInfo.stCommon.sVolume, sizeof(stConditionInfo.stCommon.sVolume));
	memcpy(&stCondition.stCommon.sVolumeRect, &stConditionInfo.stCommon.sVolumeRect, sizeof(stConditionInfo.stCommon.sVolumeRect));
	//memcpy(&stCondition.stCommon.sAmount, &stConditionInfo.stCommon.sAmount, sizeof(stConditionInfo.stCommon.sAmount));

	memcpy(&stCondition.stCommon.sResultField, &stConditionInfo.stCommon.sResultField, sizeof(stConditionInfo.stCommon.sResultField));
	
	return TRUE;
}

void CLib_SignalMngApp::GetPointToImgIndex(int nSignalType, int nPoint, char *psSignalImageIndex)
{
	int nImgStep = 7;
	int nImgIndex;
	CString strImgIndex;
	
	if (nPoint > 3)
		nPoint = 3;
	else if (nPoint < -3)
		nPoint = -3;
	
	if ((int)TYPE_SIGNAL_ALERT == nSignalType)
		nImgIndex = -1 * nPoint + 3 + nImgStep + 1;
		/*
		else if (strSignalType == (CString)TYPE_SIGNAL_USER)
		nImgIndex = -1 * nPoint + 3 + nImgStep * 2 + 1;
	*/
	else if ((int)TYPE_SIGNAL_NEWS == nSignalType)
	{
		int nInfoIndexStart = nImgStep * 3 + 1;
		
		if (nPoint == NEWS_CODETYPE_RPN)
			nImgIndex = nInfoIndexStart + 1;
		else if (nPoint == NEWS_CODETYPE_RLT)
			nImgIndex = nInfoIndexStart + 2;
		else if (nPoint == NEWS_CODETYPE_C_INS)
			nImgIndex = nInfoIndexStart + 3;
		else if (nPoint == NEWS_CODETYPE_T_INS)
			nImgIndex = nInfoIndexStart + 3;
		else
			nImgIndex = -1 * nPoint + 3 + 1;
	}
	
	/*
	CString strTemp;
	strTemp.Format("Point : %d, Img Index : %02d", nPoint, nImgIndex);
	AfxMessageBox(strTemp);
	*/
	
	strImgIndex.Format("%02d", nImgIndex);
	memcpy(psSignalImageIndex, strImgIndex.GetBuffer(0), 2);
}

void CLib_SignalMngApp::SetWriteDebug(CString strPos, LPCTSTR strData)
{
// 	if (m_bTrace == FALSE)
// 		return;
// 	
// 	SYSTEMTIME					sTime;
// 	CString							strCurTime;
// 	CString							strTemp;
// 	HFILE								hFile;
// 	
// 	GetSystemTime(&sTime);
// 	if(strPos != "")
// 	{
// 		strCurTime.Format("[%02d:%02d:%02d:%03d  ", sTime.wHour + 9, 
// 			sTime.wMinute, sTime.wSecond, sTime.wMilliseconds);
// 		strTemp = strCurTime + strPos + "]	" + strData + "\r\n";
// 	}
// 	else
// 		strTemp = strData;
// 	
// 	CString strFile, strLogFolder, strLogFile;
// 	strLogFolder = "\\Log_SignalMng";
// 	strFile.Format("\\Sansung_SignalMng%04d%02d%02d.log",  
// 		sTime.wYear, sTime.wMonth, sTime.wDay);
// 	strLogFile = m_strAppPath + strLogFolder + strFile;
// 	
// 	hFile = _lopen((LPCTSTR)strLogFile, OF_WRITE);
// 	if (hFile > 0)
// 	{
// 		if(_llseek(hFile, 0L, FILE_END) < 0)
// 			return;
// 		
// 		_hwrite(hFile, (LPCTSTR)strTemp, strTemp.GetLength());
// 		_lclose(hFile);
// 	}
// 	else
// 	{
// 		::CreateDirectory(m_strAppPath + strLogFolder, NULL);
// 		hFile = _lcreat((LPCTSTR)strLogFile, 0);
// 		if(hFile > 0)
// 		{
// 			_hwrite(hFile, (LPCTSTR)strTemp, strTemp.GetLength());
// 			_lclose(hFile);
// 		}
// 	}
}

int CLib_SignalMngApp::SetSiganl(BOOL bAdvise, CWnd* pWnd, CString strRealType, CString sPBID, CString strKey)
{
	int nRealType;
	nRealType = atoi(strRealType);

	m_sPBID = sPBID;
	
	if(nRealType!=TYPE_REAL_NEWS && nRealType!=TYPE_SIGNAL_NEWS)
	{
#ifdef __DEBUG		
		AfxMessageBox("CLib_SignalMngApp::SetSiganl(..) 리얼타입이 아직 정의안됨");

		return 0;
#endif
	}
//@유진 080422수 아직 정의되어있지 않으므로 리턴함.
//		TYPE_REAL_NEWS만 정의됨.

	int nResult = 0;
	if (bAdvise == TRUE)
		nResult = SetAdd_AdvTable(pWnd, nRealType, strKey);
	else
		nResult = SetDelete_AdvTable(pWnd, nRealType, strKey);
	
	return nResult;
}

int _DLL_SetSiganl(BOOL bAdvise, CWnd* pwndMain, CString strType, CString sPBID, CString strKey /*= "*"*/)
{
	int nResult = -1;
	
	if (theApp.m_bLibOpen == TRUE)
		nResult = theApp.SetSiganl(bAdvise, pwndMain, strType, sPBID, strKey);
	
	return nResult;
}

/*
BOOL _DLL_SetMainExeManager(CWnd* pwndMain)
{
	theApp.m_hMainExeManager = pwndMain->m_hWnd;
	return TRUE;
}
*/
BOOL CLib_SignalMngApp::SetInit_AdvTable()
{
	int nResult = 0;
	
	theApp.SetWriteDebug("SetInit_AdvTable", "");
	
	m_saSysKeyTable.RemoveAll();
	m_saNewsKeyTable.RemoveAll();
	m_saPortKeyTable.RemoveAll();
	
	m_pmapCondition = NULL;
	m_pmapCondition = new CMapStringToOb;
	
	m_pmapAlert_Sys = NULL;
	m_pmapAlert_Sigal = NULL;
	m_pmapNews_Signal = NULL;
	m_pmapNews_Real = NULL;
	m_pmapAlert_Sigal = new CMapStringToOb;
	
	int nPos, nIndex;
	CString strKey, strData;
	CPtrArray *paTarget;
	ConditionData *pstConditionData;
	
	FILE *fr = NULL;
	char buffer[1024] = {0,};
	
	CString strAppPath;
	GetDir_DrfnRoot(strAppPath);
	CString strFile = strAppPath + "\\icss\\finddata\\signal_syslist.dat";
	if ((fr = fopen((LPTSTR)(LPCTSTR)strFile,"r")) == NULL) 
		nResult--;
	else
	{
		m_pmapAlert_Sys = new CMapStringToOb;
		while (!feof(fr)) 
		{
			memset(buffer, 0x00, 1024);
			if (fgets(buffer, sizeof(buffer), fr) == NULL) 
				break;
			buffer[strlen(buffer) - 1] = 0x00;
			
			strData = (CString)buffer;
			nIndex = 0;
			CString strTokenData[4];
			while (nIndex < 4)
			{
				if (strData.IsEmpty() == TRUE)
					break;
				
				nPos = strData.Find(":");
				strTokenData[nIndex] = strData.Left(nPos);
				strData = strData.Mid(nPos + 1, strData.GetLength() - nPos);
				nIndex++;
			}
			
			if (strTokenData[0].IsEmpty() == TRUE)
				continue;
			
			if ((CString)TYPE_TABLE_PORT == (CString)strTokenData[1])
				m_saPortKeyTable.Add(strTokenData[0] + "|" + strTokenData[3]);
			else
			{
				paTarget = new CPtrArray;
				m_pmapAlert_Sys->SetAt(strTokenData[0], (CObject*&)paTarget);
				
				m_saSysKeyTable.Add(strTokenData[0] + "|" + strTokenData[3]);
			}
			
			//# Server Condition Table!
			pstConditionData = new ConditionData;
			pstConditionData->nIndex = m_pmapCondition->GetCount();
			pstConditionData->strTitle = strTokenData[3];
			pstConditionData->nBasePoint = atoi(strTokenData[2]);
			pstConditionData->strCode = strTokenData[0];
			pstConditionData->strPath = "";
			
			TRACE("\n#" + strTokenData[0]);
			m_pmapCondition->SetAt(strTokenData[0], (CObject*&)pstConditionData);
		}
		fclose(fr);
		fr = NULL;

		nResult++;
	}
	
	theApp.SetWriteDebug("SetInit_AdvTable", "Make Sys Table");
	
	CFileFind cFind;
	strFile = strAppPath + "\\chart\\Data\\NewsData\\news_proffer.ini";
	if (!cFind.FindFile(strFile))
	{
		CString strMsg;
		strMsg.Format("\n [%s] 을 찾지 못했습니다.", strFile);
		theApp.SetWriteDebug("SetInit_AdvTable", "strMsg");
		
		nResult--;
	}
	else
	{
		m_pmapNews_Signal = new CMapStringToOb;
		m_pmapNews_Real = new CMapStringToOb;
		
		CString strSection = "제공처";
		int nCnt = GetPrivateProfileInt(strSection, "cnt", 0, strFile);
		char chBuff[BUFSIZ];
		for (int nItem = 0; nItem < nCnt; nItem++)
		{
			memset(chBuff, ' ', sizeof(chBuff));
			strKey.Format("%d", nItem + 1);
			GetPrivateProfileString(strSection, strKey, "", chBuff, sizeof(chBuff), strFile);
			strData = (CString)chBuff;
			
			nIndex = 0;
			CString strTokenData[2];
			while (nIndex < 2)
			{
				if (strData.IsEmpty() == TRUE)
					break;
				
				nPos = strData.Find(";");
				strTokenData[nIndex] = strData.Left(nPos);
				strData = strData.Mid(nPos + 1, strData.GetLength() - nPos);
				nIndex++;
			}
			
			paTarget = new CPtrArray;
			m_pmapNews_Signal->SetAt(strTokenData[0], (CObject*&)paTarget);
			
			paTarget = new CPtrArray;
			m_pmapNews_Real->SetAt(strTokenData[0], (CObject*&)paTarget);
			
			m_saNewsKeyTable.Add(strTokenData[0] + "|" + strTokenData[1]);
		}
		nResult++;
	}
	
	theApp.SetWriteDebug("SetInit_AdvTable", "Make News Table");
	
	return nResult;
}

void CLib_SignalMngApp::SetClear_ConditionTable()
{
	if (m_pmapCondition == NULL)
		return;
	
	POSITION pos;
	CString strKey;
	ConditionData *pstConditionData;
	for (pos = m_pmapCondition->GetStartPosition(); pos != NULL;)
	{
		m_pmapCondition->GetNextAssoc(pos, strKey, (CObject *&)pstConditionData);
		
		delete pstConditionData;
		pstConditionData = NULL;
		
		m_pmapCondition->RemoveKey(strKey);
	}
	delete m_pmapCondition;
	m_pmapCondition = NULL;
}

void CLib_SignalMngApp::SetClear_AdvTable(int nRealType, CWnd* pWndTarget)
{
	CMapStringToOb *pmapData;
	CPtrArray *paTarget;
	POSITION pos;
	CString strKey;
	
	if ((int)TYPE_SIGNAL_SYS == nRealType)
		pmapData = m_pmapAlert_Sys;
	else if ((int)TYPE_SIGNAL_ALERT == nRealType)
		pmapData = m_pmapAlert_Sigal;
	else if ((int)TYPE_SIGNAL_NEWS == nRealType)
		pmapData = m_pmapNews_Signal;
	else if ((int)TYPE_REAL_NEWS == nRealType)
		pmapData = m_pmapNews_Real;
	
	if (pmapData == NULL)
		return;
	
	for (pos = pmapData->GetStartPosition(); pos != NULL;)
	{
		pmapData->GetNextAssoc(pos, strKey, (CObject *&)paTarget);
		
		//# Real Stop!
		if ((int)TYPE_SIGNAL_ALERT == nRealType)
			SetAdvData_Alert(FALSE, strKey);
		
		BOOL bDeleteCheck = FALSE;
		int nArrIndex;
		for(nArrIndex = 0; nArrIndex < paTarget->GetSize(); nArrIndex++)
		{
			CWnd* pTargetWndArrItem = (CWnd*)paTarget->GetAt(nArrIndex);
			if(pWndTarget == pTargetWndArrItem)
			{
				bDeleteCheck = TRUE;
				break;
			}
		}

		if(!bDeleteCheck)
			continue;
		else
			paTarget->RemoveAt(nArrIndex);
		
		/*paTarget->RemoveAll();*/
		
		if(paTarget->GetSize() == 0)
		{
			delete paTarget;
			paTarget = NULL;
			pmapData->RemoveKey(strKey);
		}
	}
}

void CLib_SignalMngApp::SetClear_AdvTable(CWnd* pTargetWnd)
{
	SetClear_AdvTable((int)TYPE_SIGNAL_SYS, pTargetWnd);
	SetClear_AdvTable((int)TYPE_SIGNAL_ALERT, pTargetWnd);
	SetClear_AdvTable((int)TYPE_SIGNAL_NEWS, pTargetWnd);
	SetClear_AdvTable((int)TYPE_REAL_NEWS, pTargetWnd);
}

BOOL CLib_SignalMngApp::SetAdd_AdvTable(CWnd *pWnd, int nRealType, CString strKey)
{
	BOOL bResult = FALSE;
	CMapStringToOb *pmapData;
	CPtrArray *paTarget;
	POSITION pos;
	
	if ((int)TYPE_SIGNAL_SYS == nRealType)
		pmapData = m_pmapAlert_Sys;
	else if ((int)TYPE_SIGNAL_ALERT == nRealType)
		pmapData = m_pmapAlert_Sigal;
	else if ((int)TYPE_SIGNAL_NEWS == nRealType)
		pmapData = m_pmapNews_Signal;
	else if ((int)TYPE_REAL_NEWS == nRealType)
		pmapData = m_pmapNews_Real;
	
	if (pmapData == NULL)
		return bResult;
	
	if (strKey == "*")
	{
		for (pos = pmapData->GetStartPosition(); pos != NULL;)
		{
			pmapData->GetNextAssoc(pos, strKey, (CObject *&)paTarget);
			
			paTarget->Add(pWnd);
			pmapData->SetAt(strKey, (CObject *&)paTarget);
		}
		
		SetAdvData_Sys(TRUE);
		bResult = TRUE;
	}
	else
	{
		if (pmapData->Lookup(strKey, (CObject*&)paTarget) == TRUE)
		{
			paTarget->Add(pWnd);
			pmapData->SetAt(strKey, (CObject*&)paTarget);
			bResult = TRUE;
		}
		else
		{
			if ((int)TYPE_SIGNAL_ALERT == nRealType)
			{
				paTarget = new CPtrArray;
				paTarget->Add(pWnd);
				pmapData->SetAt(strKey, (CObject*&)paTarget);
				bResult = TRUE;
			}
			else
				bResult = FALSE;
		}
		
		if (((int)TYPE_SIGNAL_ALERT == nRealType) && (bResult == TRUE))
			SetAdvData_Alert(TRUE, strKey);
	}
	
	return bResult;
}

BOOL CLib_SignalMngApp::SetDelete_AdvTable(CWnd *pWnd, int nRealType, CString strKey)
{
	BOOL bResult = FALSE;
	CMapStringToOb *pmapData;
	CPtrArray *paTarget;
	POSITION pos;
	
	if ((int)TYPE_SIGNAL_SYS == nRealType)
		pmapData = m_pmapAlert_Sys;
	else if ((int)TYPE_SIGNAL_ALERT == nRealType)
		pmapData = m_pmapAlert_Sigal;
	else if ((int)TYPE_SIGNAL_NEWS == nRealType)
		pmapData = m_pmapNews_Signal;
	else if ((int)TYPE_REAL_NEWS == nRealType)
		pmapData = m_pmapNews_Real;
	
	if (pmapData == NULL)
		return bResult;
	
	int nIndex, nDelCnt = 0;
	for (pos = pmapData->GetStartPosition(); pos != NULL;)
	{
		pmapData->GetNextAssoc(pos, strKey, (CObject *&)paTarget);
		for (nIndex = 0; nIndex < paTarget->GetSize(); nIndex++)
		{
			if (paTarget->GetAt(nIndex) == pWnd)
			{
				paTarget->RemoveAt(nIndex);
				nDelCnt++;
				break;
			}
		}
		
// 각 화면별로 얼럿을 관리하기 위해 주석
// 여기서 하면 다른 화면에서도 삭제
//		if ((int)TYPE_SIGNAL_ALERT == nRealType)
//			SetAdvData_Alert(FALSE, strKey);
		
		pmapData->SetAt(strKey, (CObject *&)paTarget);
	}
	
	if (nDelCnt > 0)
		bResult = TRUE;
	
	return bResult;
}

BOOL CLib_SignalMngApp::SetRealData(int nRealType, CString strKey, LPSTR sRealData, CString sPBID)
{
	BOOL bResult = FALSE;  

	ST_REALDATA stRealData;
	memset(&stRealData, 0x20, sizeof(ST_REALDATA));
	
	stRealData.chCount = (BYTE)1;
	stRealData.nSize = strlen(sRealData); // 자동갱신 구조체 사이즈 
	stRealData.pDataBuf = sRealData;			// 실제 데이터 
	stRealData.lRealStruct = 0;						// 자동갱신 구조체 (구조체에 대한 Pointer)
	memcpy(stRealData.szKeyCode, strKey.GetBuffer(0), strKey.GetLength());
	
	stRealData.chRealType = (BYTE)(int)nRealType;

	CMapStringToOb *pmapData;
	if ((int)TYPE_SIGNAL_SYS == nRealType)
	{
		pmapData = m_pmapAlert_Sys;
	}
	else if ((int)TYPE_SIGNAL_ALERT == nRealType)
	{
		pmapData = m_pmapAlert_Sigal;
	}
	else if ((int)TYPE_SIGNAL_NEWS == nRealType)
	{
		pmapData = m_pmapNews_Signal;
	}
	else if ((int)TYPE_REAL_NEWS == nRealType)
	{
		pmapData = m_pmapNews_Real;
	}
	
	if (pmapData != NULL)
	{
		CPtrArray *paData;
		if (pmapData->Lookup(strKey, (CObject*&)paData) == TRUE)
		{
			for (int nIndex = 0; nIndex < paData->GetSize(); nIndex++)
			{
				CWnd *pwndSend = (CWnd *)paData->GetAt(nIndex); 

				if (::IsWindow(pwndSend->m_hWnd) == TRUE)
					/*::SendMessage(pwndSend->m_hWnd, WM_BC_DATA_RECEIVED, (WPARAM)(LPCTSTR)sPBID, (LPARAM)&stRealData);*/
					::SendMessage(pwndSend->m_hWnd, WM_GET_BROD, (WPARAM)(LPCTSTR)sPBID, (LPARAM)&stRealData);
			}
		}
		bResult = TRUE;
	}

	return bResult;
}

//--> 0016487: [7134] 신호검색 설정화면에서 신호 항목 변경후 [초기화] 클릭시 정렬만 되는 현상 수정
void _DLL_SetDefSysSignalData(CMapStringToPtr *pmapSignalSet)
{
	pmapSignalSet->RemoveAll();

	CString strFileRoot = theApp.m_strAppPath + "\\icss\\finddata" + FOLDER_SIGNALSET + SIGNALDATA_SYS_FILE_DEF;
	theApp.GetLoad_SinalData(strFileRoot, pmapSignalSet);
}
//<--