#if !defined(AFX_GRAPHSETTING_H__3A3632A1_13B5_43C3_871C_FD60AB5A300B__INCLUDED_)
#define AFX_GRAPHSETTING_H__3A3632A1_13B5_43C3_871C_FD60AB5A300B__INCLUDED_

#include <afxtempl.h>

#include "Include_AddIn.h"		// for IGraphInfo
#include "TabSubIndicatorSetUpCtrl.h"
#include "MakeChildDlg.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGraphSetting dialog
class CBaseDialog;
class CIndicatorConfigurationData;

class CGraphSetting : public CDialog, public CTabParentDlg
{
	DECLARE_DYNAMIC( CGraphSetting)

// Construction
public:
	// (2004.09.06, 배승원) 선택된 Graph별 지표 설정을 받는도록 하는 기능에서,
	//		별도의 Check Button으로 Default (User Default) Indicator Info도 변경하도록 지원할 것인지를
	//		설정받아 처리토록 한다.
	CGraphSetting( IChartOCX *p_pIChartOCX, BOOL p_bEnableWithDefaultInfo, CWnd* pParent = NULL);   // standard constructor
	~CGraphSetting();

	void SetData( IGraphInfo *p_pIGraphInfo, BOOL p_bLogScale, BOOL p_bReverseScale, BOOL p_bDelAllCheck);
	void SetDeleteAllButtonState(const BOOL enableDeleteAll);
	//Ok, Cancel, Delete 중 사용자가 선택한 값을 얻는다.
	int GetReturnValue() const;

	void OK( void )			{ OnOK(); };
	void Cancel( void )		{ OnCancel(); };

	void ShowChildDialog(BOOL bShow);

// Dialog Data
	//{{AFX_DATA(CGraphSetting)
	enum { IDD = IDD_GRAPH_SETTING };
	CTabSubIndicatorSetUpCtrl	m_tabIndicatorSetUpAll;
	BOOL	m_bWithDefaultInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphSetting)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGraphSetting)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnAllDelete();
	virtual void OnCancel();
	afx_msg void OnChkWithDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	LRESULT OnInsertOscillator(WPARAM wParam, LPARAM lParam);

// (2004.10.11, 배승원) OCX로부터 주요 Manager의 Interface를 구하여 관리한다.
protected:
	IChartOCX *			m_pIChartOCX;
	IPacketManager *	m_pIPacketManager;
	IChartManager *		m_pIChartManager;
	IGraphInfoManager *	m_pIGraphInfoManager;
	IAddInToolMgr *		m_pIAddInToolMgr;

private:
	BOOL m_bEnableDeleteAll;
	BOOL m_bHaveToDeleteMAGraph;
	CMakeChildDlg m_MakeChildDlg;
	CBaseDialog *m_pChildDlg;

	int m_eReturn; //Ok(IDOK), Cancel(IDCANCEL), Delete(IDABORT) 중 한가지.	

	IGraphInfo *m_pIGraphInfo;
	CMainBlock *m_pMainBlock;
	BOOL m_bDoCalculate;	// 종찬(05/01/20) 계산여부.
	BOOL m_bVertLogScale;
	BOOL m_bVertInvertScale;

	void SetChildDlgPosition();
	void DeleteChildDlg();

// (2004.09.06, 배승원) 선택된 Graph별 지표 설정을 받는도록 하는 기능에서,
//		별도의 Check Button으로 Default (User Default) Indicator Info도 변경하도록 지원할 것인지를
//		설정받아 처리토록 한다.
protected:
	BOOL	m_bEnableWithDefaultInfo;
public:
	BOOL	IsWithDefaultInfo( void) const	{	return m_bWithDefaultInfo;	}


// 종찬(05/01/18) 
public:
	BOOL GetLogState() const;
	void SetLogState(BOOL bLogState);

	BOOL GetInvertScaleType() const;
	void SetInvertScaleType(BOOL bInvertScaleType);
	
	CString GetSubGraphData();
	void SetSubGraphData(CString strSubGraphData);

	BOOL DoCalculate() const;	// 종찬(05/01/20) 다시 계산할지를 리턴한다.

// (2008/1/16 - Seung-Won, Bae) for MultiLanguage
protected:
	HWND	m_hOcxWnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHSETTING_H__3A3632A1_13B5_43C3_871C_FD60AB5A300B__INCLUDED_)
