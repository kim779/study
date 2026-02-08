// PcTrMng.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PcTrMng.h"

// include 헤더정의
//#include "../../inc/EnvDir.h"
#include "../../ForSite/FS_Env.h"	//Solomon-090829

#include "../../inc/PcTrComDef.h"

#include "../../inc/AZUtilFileMng.h"

// 기타 UTIL모음.
#include "EtcUtil.h"

#include "./PCTrMainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CPcTrMngApp

BEGIN_MESSAGE_MAP(CPcTrMngApp, CWinApp)
	//{{AFX_MSG_MAP(CPcTrMngApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPcTrMngApp construction

CPcTrMngApp::CPcTrMngApp()
{
	m_hMainHwnd = NULL;
	m_pPCTrMainWnd = NULL;

	for(int k=0; k<MAX_INTERFACE; k++) m_uaInterface[k].pInterface=NULL;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance


	//-->Solomon-090829
	GetDir_DrfnRoot(m_szRootDir);
	GetDir_DrfnBin(m_szDrfnBinDir);
	//m_szLibName.Format("%s\\%s", m_szDrfnBinDir, "Lib_ConditionMng.dll");
	//<--Solomon-090829

	TRACE2("RootDir[%s], MainDir[%s]\n", m_szRootDir, m_szDrfnBinDir);

	m_MainInfoMng.SetDataString(USERINFOMNG_ROOTDIR, m_szRootDir);	// 10 : MainDirectory
	m_MainInfoMng.SetDataString(USERINFOMNG_EXECDIR, m_szDrfnBinDir); // 11 : Bin

	//solomon-090825
	m_MainInfoMng.SetDataString(USERINFOMNG_DATADIR, m_szRootDir+"\\icss");		//IBK는 icss, SOLOMON은 hanaro.
	m_MainInfoMng.SetDataString(USERINFOMNG_IMAGEDIR, m_szRootDir+"\\icss\\Image");
	m_MainInfoMng.SetDataString(USERINFOMNG_USERDIR, m_szRootDir+"\\User");
	//m_MainInfoMng.SetDataString(USERINFOMNG_REALUSERDIR, m_szRootDir+"\\User");

	{//KHD
	//	CString szOcxName;
	//	szOcxName.Format("%s\\Dev\\MsChrt20.ocx", m_szRootDir);
	//	m_SysUtilMng.DLLRegister(szOcxName);
	//END
	}		
	LoadDefaultColorTable();
}

CPcTrMngApp::~CPcTrMngApp()
{
	//UnLoadTestDLL();
}


// void CPcTrMngApp::SetHanaroGateMngHandle()
// 환경변수에 PcTrMng관련 핸들을 저장한다. 
// 키값을 만들때는 모듈의 핸들을 추가해서 Process내에서는 유일한 값이 되도록 한다.
void CPcTrMngApp::SetHanaroGateMngHandle()
{
	char aBufWnd[8+1]={0,};
	sprintf(aBufWnd, "%08X", theApp.m_uaInterface[0].pInterface);

	CString szClipFormat;
	szClipFormat.Format("%s%08X", HANARO_CLIPKEY_GATEMANAGER, ::GetModuleHandle(HANARO_PCTRMNG));

	BOOL bRet = ::SetEnvironmentVariable(szClipFormat, aBufWnd);
}

// int CPcTrMngApp::SetHanaroMainHandle(HWND hMainWnd)
// 환경변수에 메인관련 핸들을 저장한다. 
// 키값을 만들때는 모듈의 핸들을 추가해서 Process내에서는 유일한 값이 되도록 한다.
int CPcTrMngApp::SetHanaroEnvSetting(HWND hWnd, LPCSTR szFormatKey)
{
	char aBufWnd[8+1]={0,};
	sprintf(aBufWnd, "%08X", hWnd);

	CString szClipFormat;
	szClipFormat.Format("%s%08X", szFormatKey, ::GetModuleHandle(HANARO_PCTRMNG));

	BOOL bRet = ::SetEnvironmentVariable(szClipFormat, aBufWnd);
	return bRet;
}

//IAUDrdsManager* CPcTrMngApp::GetNewIAUDrdsManager()
//{
//	CAUDrdsManager* pNew = new CAUDrdsManager;
//	return pNew;
//}


BOOL CPcTrMngApp::InitInstance() 
{
	DOChartOCXRegister();

	_DoClearDataFile();

	CPCtrMainWnd* pMainWnd = new CPCtrMainWnd;
	pMainWnd->OpenWindow();
	m_pPCTrMainWnd = (CWnd*)pMainWnd;

	m_MainInfoMng.SetDataString(26, CSystemInfo::GetPcIP());

	m_pTrCommMng	= new CTrCommManager;
	m_pTLMngLoader	= new CTreeListMngLoader;
//	m_pECWMngLoader	= new CExtCodeWndMngLoader;
	m_pDataMonMng	= new CAUDataMonitorManager;
	m_pPtMngLoader  = new CPartnerMngLoader;

	m_pClipMng = NULL;

	m_uaInterface[ 0].pInterface = (LONG)&m_GateMng;
	m_uaInterface[ 1].pInterface = (LONG)&m_PcTrMng;
	m_uaInterface[ 2].pInterface = (LONG)m_pTrCommMng;
	m_uaInterface[ 3].pInterface = NULL;
//	m_uaInterface[ 4].pInterface = (LONG)&m_GdiMainMng;

	m_uaInterface[ 5].pInterface = (LONG)&m_MasterMng;
	m_uaInterface[ 6].pInterface = (LONG)&m_SkinColorMng;
//	m_uaInterface[ 7].pInterface = (LONG)&m_BGManager;
//	m_uaInterface[ 8].pInterface = (LONG)&m_OrderMonMng;
//	m_uaInterface[ 9].pInterface = (LONG)&m_GwanMng;

//	m_uaInterface[10].pInterface = (LONG)&m_SigMng;
	m_uaInterface[11].pInterface = NULL;
	m_uaInterface[12].pInterface = NULL;
	m_uaInterface[13].pInterface = NULL;
	m_uaInterface[14].pInterface = (LONG)&m_DebugMng;

	//m_uaInterface[15].pInterface = (LONG)&m_AccountMng;
	m_uaInterface[16].pInterface = NULL;
//	m_uaInterface[17].pInterface = (LONG)&m_RegMng;
	m_uaInterface[18].pInterface = NULL;
	m_uaInterface[19].pInterface = (LONG)&m_DrdsMng;

	m_uaInterface[20].pInterface = NULL;
	m_uaInterface[21].pInterface = (LONG)&m_MainInfoMng;
	m_uaInterface[22].pInterface = (LONG)&m_MainExinfoMng;
//	m_uaInterface[23].pInterface = (LONG)&m_EncMng;
	m_uaInterface[24].pInterface = (LONG)&m_DataUtilMng;

//	m_uaInterface[25].pInterface = (LONG)&m_PCOrderMng;
	m_uaInterface[26].pInterface = NULL;						// ICodeListManager*
	m_uaInterface[27].pInterface = NULL;						// IExtCodeWndManager*
	m_uaInterface[28].pInterface = (LONG)m_pDataMonMng;			// IAUDataMonitorManager*
	m_uaInterface[29].pInterface = (LONG)m_pClipMng;			// IClipboardManager*

//	m_uaInterface[30].pInterface = (LONG)&m_FormOpenDispMng;	// IAUFormOpenManager
//	m_uaInterface[31].pInterface = (LONG)&m_FormDataDispMng;	// IAUFormDataManager
//	m_uaInterface[32].pInterface = (LONG)&m_DeskDispMng;		// IAUDispManager
//	m_uaInterface[33].pInterface = (LONG)&m_MainDispMng;		// No.33 IAUMainDispManager
//	m_uaInterface[34].pInterface = (LONG)&m_FrameDispMng;		// No.34 IAUFrameDispManager

//	m_uaInterface[35].pInterface = (LONG)&m_ViewDispMng;		// No.35 IAUViewDispManager
//	m_uaInterface[36].pInterface = (LONG)&m_PrimLibDispMng;		// No.36 IAUPrimLibDispManager
//	m_uaInterface[37].pInterface = (LONG)&m_PrimScrDispMng;		// No.37 IAUPrimScrDispManager
//	m_uaInterface[38].pInterface = (LONG)&m_StateMng;			// No.38 IRunStateManager
//	m_uaInterface[39].pInterface = (LONG)&m_CtlPosMng;			// No.39 IControlPosManager

	//m_uaInterface[40].pInterface = (LONG)&m_CumtomMng;			// No.40 ICumstomManager
	m_uaInterface[40].pInterface = NULL;
	m_uaInterface[41].pInterface = (LONG)&m_StMng;				// No.41 ISTManager
	m_uaInterface[42].pInterface = (LONG)&m_SysUtilMng;			// No.42 ISystemUtilManager
	m_uaInterface[43].pInterface = (LONG)&m_CompanyMng;			// No.43 ICompanyManager
//	m_uaInterface[44].pInterface = (LONG)&m_ChartUtilMng;		// No.44 IChartUtilManager

	m_uaInterface[45].pInterface = (LONG)&m_NewsSignalMng;		// No.45 INewsSignalManager
	m_uaInterface[46].pInterface = NULL;
	m_uaInterface[47].pInterface = NULL;
	m_uaInterface[48].pInterface = NULL;
	m_uaInterface[49].pInterface = NULL;

	//No.50 IPartnerSvrManager	-> CGateManager::GetInterface(..)에서 구현함.
	//No.51 IChartBozoMsgManager -> CGateManager::GetInterface(..)에서 구현함.

	SetHanaroGateMngHandle();			// 어디에서나 접근할 수 있게 처리한다.
	if(AfxGetMainWnd())
	{
		HWND hMainWnd=AfxGetMainWnd()->m_hWnd;
		SetHanaroEnvSetting(hMainWnd, HANARO_CLIPKEY_MAINFRAME);
	}
	SetHanaroEnvSetting(m_pPCTrMainWnd->m_hWnd, HANARO_CLIPKEY_PCTRMAIN);

	// 관심에 등록하는 것은 여기에서 하지 않는다.
	//m_GwanMng.m_GwanWnd.Create(NULL);

//@Solomon090819	CString sPath = CDirMgr::GetCommonDir();
//@Solomon090819	theApp.m_MainInfoMng.m_linkDataMng.Initialize((LPTSTR)(LPCTSTR)sPath);
//@Solomon090819	theApp.m_GwanMng.m_intrMngr.LoadIntrMngr(sPath);
//@Solomon090819	theApp.m_GwanMng.m_intrMngr.Initialize();

	m_pPctrWinixLoader = new CPctrWinixLoader;	//For 위닉스 LIB

	_DoPrepareData();

	return CWinApp::InitInstance();
}

int CPcTrMngApp::ExitInstance() 
{
	m_MainExinfoMng.PreUnLoad();
//	m_BGManager.PreUnLoad();

	if(m_pTLMngLoader) m_pTLMngLoader->CleanAll();
	SafeDelete(m_pTLMngLoader);

//	SafeDelete(m_pECWMngLoader);
	SafeDelete(m_pDataMonMng);
	SafeDelete(m_pTrCommMng);
	SafeDelete(m_pClipMng);

	SafeDelete(m_pPtMngLoader);
	SafeDelete(m_pPctrWinixLoader);
	
	m_MainExinfoMng.fnClearAll();

	CPCtrMainWnd* pMainWnd = (CPCtrMainWnd*)m_pPCTrMainWnd;
	SafeDelete(pMainWnd);
	m_pPCTrMainWnd = NULL;

	return CWinApp::ExitInstance();
}

HWND CPcTrMngApp::PctrGetMainHandle()
{
	if(!m_hMainHwnd)
		return AfxGetMainWnd()->GetSafeHwnd();
	else
		return m_hMainHwnd;
}

CWnd* CPcTrMngApp::PctrGetMainWnd()
{
	if(!m_hMainHwnd)
		return AfxGetMainWnd();
	else
		return CWnd::FromHandle(m_hMainHwnd);
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPcTrMngApp object

CPcTrMngApp theApp;

LONG WINAPI GetInferface(WPARAM wp, LPARAM lp)
{
	int nID = (int)wp;
	return theApp.m_GateMng.GetInterface(nID, (long)lp);
}

void* WINAPI GetPCTrData(WPARAM wp, LPARAM lp)
{
	char cType = (char)wp;
	int  nID = (int)lp;

	if(cType=='A')
	{
//@유진삭제
//		if(nID==12)
//		{
//			GET_DATA_DIR(theApp.m_szTemp);
//			return (void*)(LPCSTR)theApp.m_szTemp;
//		}
//@유진삭제
		return (void*)(LPCSTR)theApp.m_MainInfoMng.GetDataString(nID);
	}
	else if(cType=='E')
	{
		return (void*)theApp.m_MainExinfoMng.GetData(nID);
	}

	return 0L;
}


LONG WINAPI SetPCTrData(WPARAM wp, LPARAM lp)
{
	char cType = (char)wp;
	if(cType=='A')
	{
		STPCTR_SETDATA* sdata = (STPCTR_SETDATA*)lp;
		return (LONG)theApp.m_MainInfoMng.SetDataString(sdata->nID, sdata->szData);
	}
	else if(cType=='E')
	{
		STPCTR_SETEXDATA* sdata = (STPCTR_SETEXDATA*)lp;
		return (LONG)theApp.m_MainExinfoMng.SetData(sdata->nID, &sdata->varData);
	}
	return 0L;
}


#define CASE_TRACE_INTSTRING(xxInt) case xxInt: TRACE1("%s\n", #xxInt); break;

LONG WINAPI NotifyPCTrState(WPARAM wp, LPARAM lp)
{
	if(wp==NotifyPCTrW_AppState)
	{
		switch(lp)
		{
			CASE_TRACE_INTSTRING(PCTrMng_InitInstance);
			CASE_TRACE_INTSTRING(PCTrMng_ExitInstance);
			CASE_TRACE_INTSTRING(PCTrMng_MainCreate);
			case PCTrMng_MainDestroyStart: 
				TRACE1("%s\n", "PCTrMng_MainDestroyStart");
//				theApp.m_GwanMng.m_GwanWnd.GwanUnAdvise(); // SendMessage(WM_CLOSE, 0, 0); // .GwanUnAdvise(); 
//				theApp.m_GwanMng.m_GwanWnd.SendMessage(WM_DESTROY, 0, 0);
				break;
			CASE_TRACE_INTSTRING(PCTrMng_MainDestroyMiddle);
			CASE_TRACE_INTSTRING(PCTrMng_MainDestroyEnd); 
			CASE_TRACE_INTSTRING(PCTrMng_UserState); 
			default: break;
		}
	}
	return 0L;
}

// {{
// Testing.....
typedef  LONG (WINAPI* FpTestLoad)(long);
typedef  LONG (WINAPI* FpTestUnLoad)(long);
void CPcTrMngApp::LoadTestDLL()
{
/*
	m_hTestLib  = (HINSTANCE) ::LoadLibrary((LPCTSTR)"TestDLL.dll");
	if(m_hTestLib)
	{
		FpTestLoad fpTestLoad = (FpTestLoad)::GetProcAddress(m_hTestLib,"TestLoad");

		long dwData=NULL;
		m_Key = fpTestLoad(dwData);
	}
*/
}

void CPcTrMngApp::UnLoadTestDLL()
{
/*
	if(m_hTestLib)
	{
//		FpTestUnLoad fpTestUnLoad = (FpTestLoad)::GetProcAddress(m_hTestLib,"TestUnLoad");
//		fpTestUnLoad(m_Key);
		::FreeLibrary(m_hTestLib);
	}
*/
}

// }}

void CPcTrMngApp::LoadDefaultColorTable()
{
	CString szFilePath;
	szFilePath.Format("%s\\%s", m_MainInfoMng.GetDataDir(), "DefaultStyle.cfg");
	m_SkinColorMng.LoadColorTable(szFilePath);
}

BOOL CPcTrMngApp::DOChartOCXRegister()
{
	//자동등록을 원하지 않으면 셋팅해요.
	CString szCfgFile;
	szCfgFile.Format("%s\\PcTrMng.cfg", m_MainInfoMng.GetDataDir());

	BOOL bReg = (BOOL)::GetPrivateProfileInt("ChartOCX", "AutoRegist", 1, szCfgFile);
	if(bReg==0) return TRUE;

	szCfgFile.Format("%s\\chartmetatable.dat", m_MainInfoMng.GetDataDir());
	
	char aProgID[MAX_PATH]={0,}, aFileName[MAX_PATH]={0,};
	::GetPrivateProfileString("Chart Version", "ProgID", "NotDefine", aProgID, MAX_PATH, szCfgFile);

	// 이 부분은 적용시작이므로 사이트마다 없을 수 있으므로 확인필요함.
	::GetPrivateProfileString("Chart Version", "ProgName", "NotDefine", aFileName, MAX_PATH, szCfgFile);

	CLSID clsid;
	{
		
		CString szOcxName;
		szOcxName.Format("%s\\%s", m_szDrfnBinDir, aFileName);
		
		if(m_SysUtilMng.IsRegisteredEx(aProgID, clsid, aFileName, szOcxName)==FALSE)
		{
			m_SysUtilMng.DLLRegister(szOcxName);
		}
		return m_SysUtilMng.IsRegistered(aProgID, clsid);
	}
	return TRUE;
}

//@Solomon -->
#define variantDLL 0x10 
CString CPcTrMngApp::_WinixVariant(HWND hWnd, int comm, CString data)
{ 
// 	CString retvalue = _T("");
// 	char* dta = (char*)::SendMessage(hWnd, WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);
// 	
// 	if ((long)dta > 1)
// 		retvalue.Format("%s", dta);
// 	
// 	return retvalue;

	return (char*)::SendMessage(hWnd, WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);
}
//@Solomon <--

//alzioyes:100611-->
void CPcTrMngApp::_DoPrepareData()
{
	m_pPctrWinixLoader->PTM_GetCodeMaster();	//For IBK : Master를 파일에서 Load하는 루틴이 초기에 불릴 수 있도록.

	//091208alzioyes 1주일후에 삭제할 루틴
// 	{
// 		CString szCFG;
// 		szCFG.Format("%s\\ChartDef.cfg", m_MainInfoMng.GetEncUserDir());
// 		
// 		CString szVer;
// 		szVer.GetBufferSetLength(10);
// 		BOOL bDelete = TRUE;
// 		if(::GetPrivateProfileString("Ver", "Current", "", (LPSTR)(LPCSTR)szVer, 10, szCFG))
// 		{
// 			bDelete = FALSE;
// 		}
// 		szVer.ReleaseBuffer();
// 		::WritePrivateProfileString("Ver", "Current", "091208", szCFG);
// 		
// 		if(bDelete)
// 		{
// 			szCFG.Format("%s\\Chart", m_MainInfoMng.GetEncUserDir());
// 			CAZUtil_FileManager::DoDirFileRemove(szCFG, "*.*", TRUE);
// 		}
// 	}

	char aSendFile[MAX_PATH]={0,};

	{	// WorkDate
		CString szDataDir;
		szDataDir.Format("%s\\icss\\%s", theApp.m_MainInfoMng.GetDataDir(), gszCacheDir);

		CString szFilePath;
		szFilePath.Format("%s\\%s", szDataDir, "WorkDate.dat");

		CFile file;
		if(file.Open(szFilePath, CFile::modeRead|CFile::typeBinary)==TRUE)
		{
			DWORD nSendLen = file.GetLength();
			m_WorkDate.GetBufferSetLength(nSendLen);
			file.Read((char*)(LPCSTR)m_WorkDate, nSendLen);
			file.Close();

			m_MainExinfoMng.SetUserStringData(gszDataKey_WorkDate, (LPSTR)(LPCSTR)m_WorkDate);
			m_WorkDate.ReleaseBuffer();
		}
	}

//  	{	// US달러.
// 		sprintf(aSendFile, "%s", "\\\\158.1.128.35\\hts차트\\05버전처리\\USDollar.dat");
// 		
// 		CFile file;
// 		if(file.Open(aSendFile, CFile::modeRead|CFile::typeBinary)==TRUE)
// 		{
// 			DWORD nSendLen = file.GetLength();
// 			CString szData;
// 			szData.GetBufferSetLength(nSendLen);
// 			file.Read((char*)(LPCSTR)szData, nSendLen);
// 			file.Close();
// 			
// 			m_MainExinfoMng.SetUserStringData(gszDataKey_USDollar, (LPSTR)(LPCSTR)szData);
// 			szData.ReleaseBuffer();
// 		}
// 	}

}

#include "../../inc/AZUtilFileMng.h"
#include "../../inc/Condition_MngDef.h"
//#include "../../inc/IMainExInfoMng.h"
void CPcTrMngApp::_DoClearDataFile()
{
	CString szDataDir;
	szDataDir.Format("%s\\FindData\\%s", theApp.m_MainInfoMng.GetDataDir(), gszCacheDir);
	::mkdir(szDataDir);

	CString szIni;
	szIni.Format("%s\\%s", szDataDir, "RequestToday.ini");

	CTime tm = CTime::GetCurrentTime();
	CString szKey;
	szKey.Format("%04d%02d%02d", tm.GetYear(), tm.GetMonth(), tm.GetDay());

	CString szFilePath;
	bool bSkip = false;
	char aBuff[30+1]={0,};

	{
		bSkip = false;


		szFilePath.Format("%s%s", szDataDir, "\\MstCode.dat");
		if(::GetPrivateProfileString(gszDataKey_MstCode, "Today", "", aBuff, 30, szIni)>0)
		{
			if(szKey.Compare(aBuff)==0)
			{
				bSkip = true;

				CFileFind find;
				if (find.FindFile(szFilePath))
				{
					CFile file;
					if (file.Open(szFilePath, CFile::modeRead|CFile::typeBinary)==TRUE)
					{
						long nDataLen = file.GetLength();
						if (nDataLen >= 4)
						{
							CString rValue;
							rValue.GetBufferSetLength(nDataLen);
							file.Read((char*)(LPCSTR)rValue, 4);
							file.Close();			

							if (nDataLen < atoi(rValue.Left(4)) * sizeof(MSTCode) + 4)
								bSkip = false;

							rValue.ReleaseBuffer();
						}
					}
				}
			}
		}
		if(bSkip==false)
		{
			::WritePrivateProfileString(gszDataKey_MstCode, "Today", "", szIni);
			CAZUtil_FileManager::DoFileRemove(szFilePath);
		}
	}

	{
		bSkip = false;
		if(::GetPrivateProfileString(gszDataKey_WorkDate, "Today", "", aBuff, 30, szIni)>0)
		{
			if(szKey.Compare(aBuff)==0) bSkip = true;
		}
		if(bSkip==false)
		{
			::WritePrivateProfileString(gszDataKey_WorkDate, "Today", "", szIni);
			szFilePath.Format("%s%s", szDataDir, "\\WorkDate.dat");
			CAZUtil_FileManager::DoFileRemove(szFilePath);
		}
	}
}
//alzioyes:100611<--
