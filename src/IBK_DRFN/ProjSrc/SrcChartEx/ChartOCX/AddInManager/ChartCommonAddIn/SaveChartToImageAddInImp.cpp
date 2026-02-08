// SaveChartToImageAddInImp.cpp: implementation of the CSaveChartToImageAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "SaveChartToImageAddInImp.h"

#include "../../Include_Chart/Dll_Load/IMetaTable.h"	// for ML_LANGUAGE_RES
#include "../Include_AddIn/_resource.h"					// for ID_SCI_SAVE_SCREEN
#include "../Include_AddIn/AddInCommand.h"				// for EAI_CUSTOM_SAVE_IMAGE
#include "../Include_AddIn/I000000/_ICmdUI.h"			// for ICmdUI
#include "../Include_AddIn/I000000/_IString.h"			// for ILPCSTR
#include "DlgSaveScreen.h"								// for CDlgSaveScreen
#include "BmpFileMaker.h"								// for CBmpFileMaker

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSaveChartToImageAddInImp::CSaveChartToImageAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{

}

CSaveChartToImageAddInImp::~CSaveChartToImageAddInImp()
{

}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CSaveChartToImageAddInImp::m_strAddInItemName = "SAVE_IMAGE";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CSaveChartToImageAddInImp)
	ONADDINEVENT( OnToolCommand)
	ONADDINEVENT( OnCmdMsg)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType)
END_ADDIN_EVENT_MAP()

// (2006/11/24 - Seung-Won, Bae) On Tool Command
void CSaveChartToImageAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption)
{
	if( p_nToolType != CToolOptionInfo::T_SAVE_CHART_SCREEN) return;
	OnAddInToolCommand( EAI_CUSTOM_SAVE_IMAGE);
}

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

BOOL CSaveChartToImageAddInImp::OnCmdMsg( UINT nID, int nCode, ICmdUI *p_pICmdUI)
{
	if( ID_SCI_SAVE_SCREEN != nID) return FALSE;
	if( CN_COMMAND == nCode) return OnAddInToolCommand( EAI_CUSTOM_SAVE_IMAGE);
	else if( CN_UPDATE_COMMAND_UI == nCode)
	{
		if( p_pICmdUI) p_pICmdUI->Enable( TRUE);
		return TRUE;
	}
	return FALSE;
}

BOOL CSaveChartToImageAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID)
{
	if( EAI_CUSTOM_SAVE_IMAGE != p_eCommandID) return FALSE;

	// (2004.10.18, 배승원) User File Path를 우선 확보한다.
	ILPCSTR szUserFilePath = m_pIChartOCX->GetUserFilePathString();
	if( szUserFilePath.IsEmpty()) return TRUE;

	ML_SET_LANGUAGE_RES();
	CDlgSaveScreen dlg;
	dlg.SetFilePath( szUserFilePath);
	if( dlg.DoModal() != IDOK) return TRUE;

	int nWidth = dlg.m_nWidth;
	int nHeight = dlg.m_nHeight;

	// (2004.10.18, 배승원) FilePath가 "\"로 끝날것을 대비한다.
	if( dlg.m_strFilePath.Right( 1) != "\\") dlg.m_strFilePath += "\\";

	// (2009/5/19 - Seung-Won, Bae) for Auto Resize of Print and Image Capture
	m_pIChartOCX->LockScrollUIUpdateWithScrollInfo( TRUE);

	CString strFilePath = dlg.m_strFilePath + dlg.m_strFileName + ".bmp";
	CBmpFileMaker maker( m_pIChartOCX->GetOcxHwnd());
	if(dlg.GetFileType() == 0) //bmp저장
		maker.MakeBmpFile(nWidth, nHeight, strFilePath, m_pIChartOCX);
	else //jpg
		maker.MakeJpgFile(nWidth, nHeight, strFilePath, m_pIChartOCX);

	// (2009/5/19 - Seung-Won, Bae) for Auto Resize of Print and Image Capture
	m_pIChartOCX->LockScrollUIUpdateWithScrollInfo( FALSE);

	// (2009/9/23 - Seung-Won, Bae) Use '{}' for ML_SET_LANGUAGE_RES()'s re-using.
	{
		ML_SET_LANGUAGE_RES();
		CString strFormat;
		strFormat.LoadString( IDS_CURRENT_SCR_SAVED);
		CString msg;
		msg.Format( strFormat, strFilePath);
		MessageBox( NULL, msg, "Chart OCX - ChartCustomAddin DLL", MB_ICONINFORMATION);
	}

	return TRUE;
}
