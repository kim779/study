// ChartEnvSetupDlgAddInImp.h: interface for the CChartEnvSetupDlgAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTENVSETUPDLGADDINIMP_H__17E29FF5_5A50_4E5A_9319_0DCC2E047683__INCLUDED_)
#define AFX_CHARTENVSETUPDLGADDINIMP_H__17E29FF5_5A50_4E5A_9319_0DCC2E047683__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Include_AddIn.h"							// for Ixxxxxx.h

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/8/10
// Comments		: 
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
//
//	1. Graph Info Control Logic
//		Case I. Single Setting Dlg
//			OnInitDialog
//				- from CGraphSetting Dialog with User Graph Info.
//				- Manage Copy Graph Info in BaseDialog for Initial Data Source of Org Data and TotalSettingDlg's Reset Function.
//			Apply
//				- from CGraphSetting::OnOK with User Graph Info.
//				- first Apply to Copy in BaseDialog for Manage Data Source of UI
//				- next ReCopy from Copy to Org.
//		Case II. Total Setting Dlg
//			OnInitDialog
//				- from CDlgMainTabIndicatorSetup Dialog with User Graph Info.
//				- Manage Copy Graph Info in CDlgMainTabIndicatorSetup::m_OrgViewData for Cancel of Preview.
//				- Manage Copy Graph Info in BaseDialog for Initial Data Source of Org Data and TotalSettingDlg's Reset Function.
//			Preview
//				- Apply without Closing Setting Dlg.
//			Restore (Cancel)
//				- Restore User Graph Info List with CDlgMainTabIndicatorSetup::m_OrgViewData
//			Reset
//				- Init Setting Dialog with User Default Info to BaseDialog Copy and Update Dialog UI.
//			Apply
//				- from from CDlgMainTabIndicatorSetUp::OnBtnPreview or OnBtnSave with User Graph Info.
//				- first Apply to Copy in BaseDialog for Manage Data Source of UI
//				- next ReCopy from Copy to Org.
//
//	2. Base Dialog Copy
//		- UI Dialog State Data.
//		- In Initializing, It is User Graph Info or User Default Graph Info (in Initializing of Total Setting Dlg).
//		- and In Editting the Complex SubGraph Info, It is state data of User Setting.
//
//	3. Apply or Preview
//		- Update User Graph Info with Dialog Setting.
//		- Update Graph Info Copy of BaseDialog for Preview's Netxt Operation.

class CDlgEnvSetUpAll;
class CChartEnvSetupDlgAddInImp : public CChartAddInBase
{
public:
	CChartEnvSetupDlgAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CChartEnvSetupDlgAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
protected:
	BOOL			m_bShowMenu;		// (2007/10/31 - Seung-Won, Bae) for Hide Menu
protected:
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData);

// (2007/10/5 - Seung-Won, Bae) for the Revised Price of Map
protected:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData);

// (2007/1/23 - Seung-Won, Bae) Main Indicator CFG Dialog for Deleting State Update.
protected:
	CDlgEnvSetUpAll *m_pMainIndicatorCFG;
protected:
	// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
	virtual void	OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnCmdMsg(			UINT nID, int nCode, ICmdUI *p_pICmdUI);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2);
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, IGraphInfo *p_pIGraphInfo);

private:
	BOOL m_bEnableWithDefaultInfo; // User Default Indicator Info를 변경할지 여부

public:
	void BackUpIndicatorInfo( IStringList *pStringList = NULL );

// (2007/1/19 - Seung-Won, Bae) Check OS Version. (for excepting Win98)
protected:
	BOOL	m_bWin98;

// 2008.01.25 by LYH >>
protected:
	HWND	m_hOcxWnd;
// 2008.01.25 by LYH <<
};

#endif // !defined(AFX_CHARTENVSETUPDLGADDINIMP_H__17E29FF5_5A50_4E5A_9319_0DCC2E047683__INCLUDED_)
