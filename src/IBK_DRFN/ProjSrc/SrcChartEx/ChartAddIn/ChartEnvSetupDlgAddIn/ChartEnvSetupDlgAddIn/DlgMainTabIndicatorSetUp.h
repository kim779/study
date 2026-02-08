#if !defined(AFX_DLGMAINTABINDICATORSETUP_H__46B898E1_41B8_4B7C_A468_421DF2A2ACFF__INCLUDED_)
#define AFX_DLGMAINTABINDICATORSETUP_H__46B898E1_41B8_4B7C_A468_421DF2A2ACFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMainTabIndicatorSetUp.h : header file
//

#include <AFXTEMPL.H>
#include "BaseTabDlg.h"
#include "OrgViewData.h"
#include "GroupButtonCtrl.h"
#include "BaseDialogContainer.h"
#include "MakeChildDlg.h"
#include "TabSubIndicatorSetUpCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMainTabIndicatorSetUp dialog

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 9
// Comments		: 
// Using way	: 
// See			: class CDialog
//-----------------------------------------------------------------------------
class COutsideCommonInterfaceFunction;
class CDlgMainTabIndicatorSetUp : public CBaseTabDlg, public CTabParentDlg
{
	DECLARE_DYNAMIC( CDlgMainTabIndicatorSetUp)
// Construction
public:
	CDlgMainTabIndicatorSetUp( IChartOCX *p_pIChartOCX, CWnd *pParent = NULL );   // standard constructor
	~CDlgMainTabIndicatorSetUp();   // standard destructor

// Dialog Data
	//{{AFX_DATA(CDlgMainTabIndicatorSetUp)
	enum { IDD = IDD_MAINTAB_PAGE_INDICATOR };
	CTabSubIndicatorSetUpCtrl	m_tabIndicatorSetUpAll;
	CString	m_editIndicatorExp;
	BOOL	m_bSaveDefault;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMainTabIndicatorSetUp)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void SelChangeGroupButton(const CString &strIndicatorName, const CString &strGraphName);					// 그룹버튼 변경
	void SelChangeGraph( const CString &strIndicatorName, const CString &strGraphName);		// 지표설정창 변경
	void AddGraph( const CString &strIndicatorName, CString &p_strGraphName);								// 지표 추가
	BOOL RemoveGraph( const char *p_szGraphName);
	int GetInitChartIndex() const;

//	void InitializeAllIndicator( void );
	void InitializeSelectedIndicator( void );

	void Recover( void )	{ OnBtnRecover(); };
	void Save( void )		{ OnBtnSave(); };
	void PreView( void )	{ OnBtnPreview(); };

	void OK( void );
	void Cancel( void );

	// (2006/8/15 - Seung-Won, Bae) Support MChart Interface
	// (2009/9/3 - Seung-Won, Bae) for WorldOn MChartInterface
	void GetMChartInterface( COutsideCommonInterfaceFunction &p_outsideIF);

	// ( 2006 / 11 / 06 - Sang-Woo, Cho ) - private에서 public으로 옮김.
	void ShowChildDialog(BOOL bShow);					// ChildDlg 보여주기 유무.
	
	// ( 2006 / 11 / 15 - Sang-Woo, Cho ) 
	CString GetDataFolderPath( void );

public:
	virtual bool ChangeTabPage( CBaseTabDlg *pSourceDlgTabPage );
	virtual CBaseTabDlg::DLG_TYPE GetDialogType() const;
	
private:
	IGraphInfo* GetIndicatorInfoInBaseDlg(const CString& strIndicatorName);
	bool GetDrawingCurPrice(CBaseTabDlg *pSourceDlgTabPage, int& nDrawingCurPrice) const;

private:
	void SetOrgViewData();								// OrgViewData를 세팅한다. (취소시에 필요한 클래스이다!)
	void SetIndicatorSize();							// IndicatorCtrl의 사이즈 세팅하기.
	void SelChangeChildDlg( const CString &strIndicatorName, const CString &strGraphName);	// 해당지표의 ChildDlg창을 띄운다.

	void SetChildDlgPosition();							// ChildDlg의 위치를 세팅한다.
	void SyncItemCheck();								// ListCtrl의 아이템체크상태를 Chart와 일치시킨다.
	void ApplyGraph(const CString &strIndicatorName);	// ChildDlg에서 설정된값을 (지표)Graph에 적용시킨다.
	void RestoreChartCfg(const int nInitChartIndex);

protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgMainTabIndicatorSetUp)
	virtual BOOL OnInitDialog();
	afx_msg void OnInitilizeAllindicator();
	afx_msg void OnDefaultIndicatordata();
	afx_msg void OnChkWithDefault();
	afx_msg void OnBtnSave();
	afx_msg void OnBtnRecover();
	afx_msg void OnBtnPreview();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnAddAll();
	afx_msg void OnBtnDelete();
	afx_msg void OnBtnDeleteAll();
	afx_msg void OnBtnWithDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CFont m_pftGroupNameFont;
	CFont *m_pftOldStaticFont;
	CMapStringToPtr m_mapIndicatorExp;
	BOOL m_bPreview;
	BOOL m_bSave;
	BOOL m_bDlgMode;									// TRUE : 지표 설정, FALSE : 지표 설명

private:
	bool GetGraphNameList(const bool bIsOnlyViewData, IStringList * indicatorNameList);
	bool GetGraphNameList(IStringList *sourceIndiNameList, IStringList * targetIndiNameList);
	bool IsExistenceIndicatorName(IStringList *indicatorNameList, const char *p_szGraphName) const;

	bool IsActionInitilizeAllindicator();
	bool Initilizeindicator(CBaseDialog* pBaseDlg);

private:
	const bool m_bIsOnlyViewData;
	IChartOCX *			m_pIChartOCX;
	IChartManager *		m_pIChartManager;
	IGraphInfoManager *	m_pIGraphInfoManager;
	IPacketManager *	m_pIPacketManager;
	
	IStringList *	m_pIndicatorNameList;				// 현재 사용중인 지표명들의 리스트.
	IStringList *	m_pOrgIndicatorNameList;			// 저장한 지표명의 List
	
	int	m_nInitChartIndex;								// 지표설정창 생성 초기의 지표설정 인덱스. ('취소'시에 이값을 사용)
	CString m_strIndicatorName;							// 지표설정창에서 현재 포커싱되어있는 지표명.

	CBaseDialog *m_pChildDlg;	// 지표의 설정Dialog클래스 (지표설정창이 두개로 구성될 경우도 있으므로 CList로 갖는듯하다.)

	CMakeChildDlg m_MakeChildDlg;						// ChildDlg 생성 팩토리클래스
	CBaseDialogContainer m_BaseDialogContainer;			// 지표설정dialog 관리클래스.

	COrgViewData m_OrgViewData;							// IGraphInfo 최초설정값 관리클래스.

// ( 2007 / 01 / 09 - Sang-Woo, Cho ) - 위치이동
public:
	CGroupButtonCtrl m_ctrlGroupButton;					// GroupButton과 ListCtrl 관리클래스.
	
	// ( 2007 / 01 / 08 - Sang-Woo, Cho ) - 사용지표 GroupButton
	CGroupButtonCtrl m_ctrlUseGroupButton;				// 사용지표 GroupButton

// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
public:
	void	OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName);

// (2007/1/26 - Seung-Won, Bae) Deny the Serial Button Clock
protected:
	DWORD	m_dwLastClickedTime;

// 2008.01.25 by LYH >>
protected:
	HWND	m_hOcxWnd;
// 2008.01.25 by LYH <<
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMAINTABINDICATORSETUP_H__46B898E1_41B8_4B7C_A468_421DF2A2ACFF__INCLUDED_)
