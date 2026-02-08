// ChartLanguageTable.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ChartLanguageTable.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"	// for IMetaTable
#include "MetaTable.h"								// for CMetaTable
#include "ChartCtrlMap.h"							// for CChartCtrlMap
#include "ChartCtrl.h"								// for CChartCtrl
#include "ResourceDllManager.h"						// for CResourceDllManager
#include "CodePage.h"								// for CCodePage

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
// CChartLanguageTableApp

BEGIN_MESSAGE_MAP(CChartLanguageTableApp, CWinApp)
	//{{AFX_MSG_MAP(CChartLanguageTableApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartLanguageTableApp construction

CChartLanguageTableApp::CChartLanguageTableApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CChartLanguageTableApp object

CChartLanguageTableApp theApp;

CMetaTable			g_metaTable;
CChartCtrlMap		g_chartCtrlMap;
CResourceDllManager	g_resDllManager;


//#ifdef LANGUAGETABLEDLL_EXPORTS
#define LANGUAGETABLEDLL_API extern "C" __declspec(dllexport)
//#else
//#define LANGUAGETABLEDLL_API extern "C" __declspec(dllimport)
//#endif

LANGUAGETABLEDLL_API const char *GetMetaText_( HWND p_hOcxWnd, IMetaTable::E_CODEPAGE p_eCodePage, const char *p_szLanguageText, BOOL p_bReturnNULL);
CChartCtrl *GetChartCtrlOrFirst( HWND p_hOcxWnd)
{
	CChartCtrl *pChartCtrl = NULL;
	if( p_hOcxWnd)
	{
		g_chartCtrlMap.Lookup( p_hOcxWnd, pChartCtrl);
		if( pChartCtrl) return pChartCtrl;
	}

	HWND hOcxWnd = NULL;
	pChartCtrl = NULL;
	POSITION psnChart = g_chartCtrlMap.GetStartPosition();
	if( psnChart) g_chartCtrlMap.GetNextAssoc( psnChart, hOcxWnd, pChartCtrl);
	return pChartCtrl;
}

// (2008/1/3 - Seung-Won, Bae) Create OCX's Meta Table and Retrieve Meta Texts
LANGUAGETABLEDLL_API const char **GetMetaTable( const char *p_szDataFolder, const char *p_szLanguageTableName)
{
	if( p_szDataFolder)
		if( *p_szDataFolder)
			if( CMetaTable::m_szMetaTexts[ 0] == CMetaTable::m_szNullString)
				g_metaTable.LoadMetaTable( p_szLanguageTableName, p_szDataFolder);

	assert( *CMetaTable::m_szMetaTexts[ 0]);
	return CMetaTable::m_szMetaTexts;
}

// (2008/1/3 - Seung-Won, Bae) Create OCX's Meta Table and Retrieve Meta Texts
LANGUAGETABLEDLL_API const char **GetIndicatorNameTable( void)
{
	return g_metaTable.m_szIndicatorNames;
}

// (2008/1/10 - Seung-Won, Bae) Create Language Table.
LANGUAGETABLEDLL_API BOOL CreateLocalLanguageTable( HWND p_hOcxWnd, IMetaTable::E_LANGUAGEID p_eLanguageID, const char *p_szLocalLanguageTable, BOOL p_bDesignMode)
{
	CChartCtrl *pChartCtrl = NULL;
	if( !g_chartCtrlMap.Lookup( p_hOcxWnd, pChartCtrl))
	{
		pChartCtrl = new CChartCtrl( p_hOcxWnd, p_eLanguageID);
		g_chartCtrlMap.SetAt( p_hOcxWnd, pChartCtrl);
	}
	pChartCtrl->CreateLocalLanguageTable( p_szLocalLanguageTable, p_bDesignMode);

	return TRUE;
}

// (2008/1/3 - Seung-Won, Bae) Remove OCX's Meta Table Instance
LANGUAGETABLEDLL_API BOOL RemoveLocalLanguageTable( HWND p_hOcxWnd)
{
	if( !p_hOcxWnd) return FALSE;

	CChartCtrl *pChartCtrl = NULL;
	if( !g_chartCtrlMap.Lookup( p_hOcxWnd, pChartCtrl)) return FALSE;
	g_chartCtrlMap.RemoveKey( p_hOcxWnd);
	delete pChartCtrl;

	return TRUE;
}

// (2008/1/14 - Seung-Won, Bae) Retrieve the Local Language Table
LANGUAGETABLEDLL_API const char *GetLocalLanguageTable( HWND p_hOcxWnd)
{
	CChartCtrl *pChartCtrl = NULL;
	if( !g_chartCtrlMap.Lookup( p_hOcxWnd, pChartCtrl)) return NULL;

	return pChartCtrl->GetLocalLanguageTable();
}

// (2008/1/14 - Seung-Won, Bae) Retrieve OCX's Current Language ID
LANGUAGETABLEDLL_API IMetaTable::E_LANGUAGEID GetCurrentLanguageID( HWND p_hOcxWnd)
{
	CChartCtrl *pChartCtrl = GetChartCtrlOrFirst( p_hOcxWnd);
	if( !pChartCtrl) return IMetaTable::ENGLISH;

	return pChartCtrl->GetLanguageID();
}

// (2008/1/11 - Seung-Won, Bae) Retrieve Language TEXT
LANGUAGETABLEDLL_API const char *GetLanguageText_( HWND p_hOcxWnd, IMetaTable::E_CODEPAGE p_eCodePage, const char *p_szMetaText, BOOL p_bReturnNULL)
{
	const char *szDefaultReturn = ( p_bReturnNULL ? NULL : p_szMetaText);
	if( !p_szMetaText) return szDefaultReturn;
	if( !*p_szMetaText) return szDefaultReturn;

	CChartCtrl *pChartCtrl = GetChartCtrlOrFirst( p_hOcxWnd);
	if( !pChartCtrl) return szDefaultReturn;

	const char *szLanguageText = pChartCtrl->GetLanguageText( p_eCodePage, p_szMetaText);
	if( !szLanguageText) return szDefaultReturn;
	return szLanguageText;
}

// (2008/1/11 - Seung-Won, Bae) Retrieve Language TEXT
LANGUAGETABLEDLL_API const char *GetLanguageTextDirectly_( IMetaTable::E_CODEPAGE p_eCodePage, IMetaTable::E_LANGUAGEID p_eLanguageID, const char *p_szMetaText)
{
	if( !p_szMetaText) return p_szMetaText;
	if( !*p_szMetaText) return p_szMetaText;

	CCodePage *pCodePage = g_metaTable.GetCodePage( p_eCodePage);
	if( !pCodePage) return NULL;
	return pCodePage->GetLanguageText( p_eLanguageID, p_szMetaText);
}

LANGUAGETABLEDLL_API int GetIndicatorID( const char *p_szIndicatorName)
{
	if( !p_szIndicatorName) return -1;
	if( !*p_szIndicatorName) return -1;
	WORD wID;
	if( !g_metaTable.m_mapIndicatorID.Lookup( p_szIndicatorName, wID)) return -1;
	return wID;
}

LANGUAGETABLEDLL_API HMODULE LoadResourceDLL( HWND p_hOcxWnd, HMODULE p_hMainModule)
{
	if( !p_hMainModule) return p_hMainModule;

	CChartCtrl *pChartCtrl = GetChartCtrlOrFirst( p_hOcxWnd);
	if( !pChartCtrl) return p_hMainModule;

	IMetaTable::E_LANGUAGEID eLanguageID = pChartCtrl->GetLanguageID();
	if( _DEFAULT_RESOURCE_ID == eLanguageID) return p_hMainModule;

	return g_resDllManager.LoadResourceDLL( p_hMainModule, eLanguageID);
}

LANGUAGETABLEDLL_API int GetMetaTextCount( IMetaTable::E_CODEPAGE p_eCodePage)
{
	CCodePage *pCodePage = g_metaTable.GetCodePage( p_eCodePage);
	if( !pCodePage) return 0;
	return pCodePage->GetMetaTextCount();
}

LANGUAGETABLEDLL_API BOOL GetAllMetaTexts( IMetaTable::E_CODEPAGE p_eCodePage, const char **p_szMetaTexts)
{
	CCodePage *pCodePage = g_metaTable.GetCodePage( p_eCodePage);
	if( !pCodePage) return FALSE;
	return pCodePage->GetAllMetaTexts( p_szMetaTexts);
}

LANGUAGETABLEDLL_API BOOL UnloadAllResourceDLL( void)
{
	g_resDllManager.UnloadResourceDLL();
	return TRUE;
}

// (2008/1/11 - Seung-Won, Bae) Retrieve Meta TEXT with Creation.
// *. Convert LText to MText
//		Caution! p_szLanguageText can be created already.
LANGUAGETABLEDLL_API const char *CreateLocalMetaText( HWND p_hOcxWnd, IMetaTable::E_CODEPAGE p_eCodePage, const char *p_szLanguageText)
{
	const char *szMetaText = GetMetaText_( p_hOcxWnd, p_eCodePage, p_szLanguageText, TRUE);
	if( szMetaText) return szMetaText;

	if( !p_hOcxWnd) return p_szLanguageText;
	CChartCtrl *pChartCtrl = NULL;
	if( !g_chartCtrlMap.Lookup( p_hOcxWnd, pChartCtrl)) return p_szLanguageText;
	if( !pChartCtrl) return p_szLanguageText;

	szMetaText = pChartCtrl->CreateLocalMetaText( p_szLanguageText);
	if( szMetaText) return szMetaText;
	return p_szLanguageText;
}

// (2008/1/11 - Seung-Won, Bae) Retrieve Meta TEXT with Creation.
// *. Convert LText to MText
//		Caution! p_szMetaText can be created already or not.
LANGUAGETABLEDLL_API BOOL SetLocalMetaText( HWND p_hOcxWnd, const char *p_szMetaText, const char *p_szLanguageText)
{
	if( !p_hOcxWnd) return FALSE;
	CChartCtrl *pChartCtrl = NULL;
	if( !g_chartCtrlMap.Lookup( p_hOcxWnd, pChartCtrl)) return FALSE;
	if( !pChartCtrl) return FALSE;

	return pChartCtrl->SetLocalMetaText( p_szMetaText, p_szLanguageText);
}

// (2008/1/11 - Seung-Won, Bae) Retrieve Meta TEXT
// *. Convert LText to MText
//		Caution! p_szLanguageText can be MText already.
LANGUAGETABLEDLL_API const char *GetMetaText_( HWND p_hOcxWnd, IMetaTable::E_CODEPAGE p_eCodePage, const char *p_szLanguageText, BOOL p_bReturnNULL)
{
	const char *szDefaultReturn = ( p_bReturnNULL ? NULL : p_szLanguageText);
	if( !p_szLanguageText) return szDefaultReturn;
	if( !*p_szLanguageText) return szDefaultReturn;

	CChartCtrl *pChartCtrl = GetChartCtrlOrFirst( p_hOcxWnd);
	if( !pChartCtrl) return szDefaultReturn;

	const char *szMetaText = pChartCtrl->GetMetaText( p_eCodePage, p_szLanguageText);
	if( !szMetaText) return szDefaultReturn;
	return szMetaText;
}

// (2008/1/11 - Seung-Won, Bae) Retrieve Meta TEXT
// *. Convert old MText to new MText
//		Caution! p_szOtherMetaText can be new MText already.
LANGUAGETABLEDLL_API const char *GetMetaTextFromOther_( HWND p_hOcxWnd, IMetaTable::E_CODEPAGE p_eCodePage, const char *p_szOtherMetaText, BOOL p_bReturnNULL)
{
	const char *szDefaultReturn = ( p_bReturnNULL ? NULL : p_szOtherMetaText);
	if( !p_szOtherMetaText) return szDefaultReturn;
	if( !*p_szOtherMetaText) return szDefaultReturn;

	CCodePage *pCodePage = g_metaTable.GetCodePage( p_eCodePage);
	if( !pCodePage) return szDefaultReturn;
	const char *szMetaText = NULL;
	if( pCodePage->GetMetaTextFromOther( p_szOtherMetaText, szMetaText)) return szMetaText;

	if( IMetaTable::PACKET_NAME != p_eCodePage && IMetaTable::GRAPH_TITLE != p_eCodePage)
		return szDefaultReturn;
	CChartCtrl *pChartCtrl = NULL;
	if( g_chartCtrlMap.Lookup( p_hOcxWnd, pChartCtrl)) if( pChartCtrl)
		szMetaText = pChartCtrl->CreateLocalMetaText( p_szOtherMetaText);
	
	if( !szMetaText) return szDefaultReturn;
	return szMetaText;
}

// (2008/1/11 - Seung-Won, Bae) Get Meta Text from Other Meta Text or Language Text for Script
// *. Convert old MText or LText to new MText
//		Caution! p_szOtherMOrLText can be new MText already.
LANGUAGETABLEDLL_API const char *GetMetaTextForScript_( HWND p_hOcxWnd, IMetaTable::E_CODEPAGE p_eCodePage, const char *p_szOtherMOrLText, BOOL p_bReturnNULL)
{
	const char *szDefaultReturn = ( p_bReturnNULL ? NULL : p_szOtherMOrLText);
	if( !p_szOtherMOrLText) return szDefaultReturn;
	if( !*p_szOtherMOrLText) return szDefaultReturn;

	CChartCtrl *pChartCtrl = GetChartCtrlOrFirst( p_hOcxWnd);
	if( !pChartCtrl) return szDefaultReturn;
	const char *szMetaText = pChartCtrl->GetMetaTextForScript( p_eCodePage, p_szOtherMOrLText);

	if( !szMetaText && pChartCtrl->GetOcxHwnd() == p_hOcxWnd)
		if( IMetaTable::PACKET_NAME == p_eCodePage || IMetaTable::GRAPH_TITLE == p_eCodePage)
			szMetaText = pChartCtrl->CreateLocalMetaText( p_szOtherMOrLText);
	
	if( !szMetaText) return szDefaultReturn;
	return szMetaText;
}

// (2008/1/11 - Seung-Won, Bae) Retrieve Meta TEXT
// *. Convert LText to MText
//		Caution! p_szLanguageText can be MText already.
LANGUAGETABLEDLL_API const char *GetMetaTextDirectly_( IMetaTable::E_CODEPAGE p_eCodePage, IMetaTable::E_LANGUAGEID p_eLanguageID, const char *p_szLanguageText)
{
	if( !p_szLanguageText) return p_szLanguageText;
	if( !*p_szLanguageText) return p_szLanguageText;

	CCodePage *pCodePage = g_metaTable.GetCodePage( p_eCodePage);
	if( !pCodePage) return NULL;
	return pCodePage->GetMetaText( p_eLanguageID, p_szLanguageText);
}
