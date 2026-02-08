// KTBChart.cpp : Implementation of CKTBChartApp and DLL registration.

#include "stdafx.h"
#include "KTBChart.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for IMetaTable
#include "ProgIDInfo.h"									// for CProgIDInfo::ComServerFromProgID()
#include "KTBChartCtl.h"								// for CKTBChartCtrl
#include "OcxIDs.h"										// for _OCX_tlid

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// (2004.11.26, 배승원) 실분봉과 허수분봉의 선택 설정을 Loading하는 Interface Header를 제공하여,
//		OCX APP InitInstance()에서 기본 설정을 Loading할 수 있도록 한다.
CHARTOCX_API BOOL LoadNomalizedMinuteTimeRule( const char *p_szHtsPath);

CKTBChartApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid = _OCX_TYPE_LIBRARY_ID_VALUES;
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.23
//
//	목적	:	OCX가 설치된 경로를 구하여 관리한다.
///////////////////////////////////////////////////////////////////////////////
CString	g_strOcxPath;

// (2004.11.26, 배승원) Real의 분차트시 허수봉개념의 전체 시간을 지원할 것인지를 Flag으로 관리 지원한다.
//		특히, 별도의 Global을 속성을 두어 HTS 기본 설정을 관리하고,
//		별도의 Property를 제공하여 Chart별 속성을 변경토록 한다.
//		기본으로 실봉을 기준으로 하며, HTS의 기본 설정 CFG File을 참조하여 변경토록한다.
BOOL g_bNormalizedMinuteTimeRule = FALSE;

////////////////////////////////////////////////////////////////////////////
// CKTBChartApp::InitInstance - DLL initialization

BOOL CKTBChartApp::InitInstance()
{
	AfxEnableControlContainer();
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.

		// (2003.12.23, 배승원) OCX가 설치된 경로를 구하여 관리한다.
		CString strOcxFile;
		CProgIDInfo::ComServerFromProgID( _OCX_PROG_ID, strOcxFile);
		g_strOcxPath = strOcxFile.Left( strOcxFile.ReverseFind( '\\') + 1);

		// (2004.11.26, 배승원) Real의 분차트시 허수봉개념의 전체 시간을 지원할 것인지를 Flag으로 관리 지원한다.
		//		특히, 별도의 Global을 속성을 두어 HTS 기본 설정을 관리하고,
		//		별도의 Property를 제공하여 Chart별 속성을 변경토록 한다.
		//		기본으로 실봉을 기준으로 하며, HTS의 기본 설정 CFG File을 참조하여 변경토록한다.
		//		이때 별도의 HTS Main Path를 전달받아, 설정을 Loading하는 Routine을 이용한다.
		//		여기서는 OCX의 Path를 기준으로 처리한다. (기본 Bin Folder에 있는 것을 전제로 한다.)
		LoadNomalizedMinuteTimeRule( g_strOcxPath + "..\\");

		// (2006/11/24 - Seung-Won, Bae) Check ToolName Define with Tool IDs
		if( IMetaTable::m_nToolOptionCount != CToolOptionInfo::T_TOOLOPTION_COUNT)
		{
			CString strMsg;
			strMsg.Format( "%d ToolOptinName Not Defined!\r\nPlease Check it!", CToolOptionInfo::T_TOOLOPTION_COUNT - IMetaTable::m_nToolOptionCount);
			AfxMessageBox( strMsg);
			return FALSE;
		}
	}

	return bInit;
}


////////////////////////////////////////////////////////////////////////////
// CKTBChartApp::ExitInstance - DLL termination

int CKTBChartApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	// (2006/8/23 - Seung-Won, Bae) Manage AddIn Load String
	CKTBChartCtrl::m_mapAddInLoadString.RemoveAll();

	// (2008/1/22 - Seung-Won, Bae) for Multi-Language
	g_iMetaTable.UnloadAllResourceDLL();

	return COleControlModule::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}


/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}

// (2004.11.26, 배승원) Real의 분차트시 허수봉개념의 전체 시간을 지원할 것인지를 Flag으로 관리 지원한다.
//		특히, 별도의 Global을 속성을 두어 HTS 기본 설정을 관리하고,
//		별도의 Property를 제공하여 Chart별 속성을 변경토록 한다.
//		기본으로 실봉을 기준으로 하며, HTS의 기본 설정 CFG File을 참조하여 변경토록한다.
//		이때 별도의 HTS Main Path를 전달받아, 설정을 Loading하는 Interface를 제공한다.
//		끝으로 Loading한 설정값을 Return한다.
CHARTOCX_API BOOL LoadNomalizedMinuteTimeRule( const char *p_szHtsPath)
{
	// 1. HTS Path를 이용하여 CFG Path를 구성한다.
	CString strCfgPath( p_szHtsPath);
	int nHtsPathLength = strCfgPath.GetLength();
	if( 0 < nHtsPathLength)
	{
		if( strCfgPath.GetAt( nHtsPathLength - 1) != '\\') strCfgPath += "\\";
		strCfgPath += "Data\\Config.cfg";
		
		// 2. CFG의 CHART Section중 NMTR의 값을 반영한다. (0과 1로 확인한다.)
		g_bNormalizedMinuteTimeRule = !( GetPrivateProfileInt( "CHART", "NMTR", g_bNormalizedMinuteTimeRule ? 1 : 0, strCfgPath) == 0);
	}

	return g_bNormalizedMinuteTimeRule;
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/3/5
// Modifier		: 
// Comments		: Support Version Info for Loading of System Trading Strategy DLL
//					1 : Release
//					2 : Debug
//-----------------------------------------------------------------------------
CHARTOCX_API int IsDebugVersion( void)
{
#ifndef _DEBUG
	return FALSE;
#else
	return TRUE;
#endif
}
