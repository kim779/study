#if !defined(AFX_DLGSTRATEGYLIST_H__39A7CB0D_CC84_4134_80EA_BE1E6803DB42__INCLUDED_)
#define AFX_DLGSTRATEGYLIST_H__39A7CB0D_CC84_4134_80EA_BE1E6803DB42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgStrategyList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgStrategyList dialog
#include "Resource.h"
#include "GridCtrl_src/GridCtrl.h"
#include "control/LogicEditCtrl.h"
#include "SDItemInfo.h"
#include "Control/xSkinButton.h"
#include "DlgTimeSelector.h"
#include "../STConfigureDlg/Control/XListCtrl.h"

#include "../../inc/ExCommonHeader.h"
#include "../../inc/ExDrawButton.h"
#include "../../inc/ExStatic.h"
#include "../../inc/ExEdit.h"
#include "afxcmn.h"

//#include "../../inc/IAUDataMonitorMng.h"	// UUID_IAUDataMonitorManager : 콜백함수 받는데 사용
class CDlgStrategyList : public CDialog
{
// Construction
public:
	CDlgStrategyList(CWnd* pParent = NULL);   // standard constructor
	void ResizeAll();
	//KHD.2.15
	//IAUDataMonitorManager*	m_pIAUDataMonitorManager; // 28 : 데이터변경처리를 위한 인터페이스
	//long					m_dwIAUDataMonitorManager;
	//void InitLoad();
	//static void CALLBACK DataMonitorCallbackProc(LPCSTR szKey, long pThisPoint, long dwKey, long dwSubKey, long dwNotifyData);
	//void UpdateSTFileName(CWnd* pParent, LPCSTR szFileName);
	///////////
	enum
	{
		Basic_STChange_Delete=0x10,		//초급전략의 삭제
		Basic_STChange_Add=0x11,		//초급전략의 추가
		Ext_STChange_SDDelete=0x20,		//고급전략의 사용자단위전략삭제
		Ext_STChange_SDAdd=0x21,		//고급전략의 사용자단위전략추가
		Ext_STChange_Delete=0x40,		//고급전략의 사용자조합전략삭제
		Ext_STChange_Add=0x41,			//고급전략의 사용자조합전략추가
	};

// Dialog Data
	//{{AFX_DATA(CDlgStrategyList)
	enum { IDD = IDD_DLG_STLIST };
	CExEdit			m_editSTTitle;
	CStatic			m_stName;
	CStatic			m_stFomula;
	CLogicEditCtrl	m_ctrlLogic;
	CExDrawButton	m_btnUp;
	CExDrawButton	m_btnDown;
	CExDrawButton	m_btnDelete;
	CExDrawButton	m_btnEdit;
	CExDrawButton	m_btnAnd;
	CExDrawButton	m_btnOr;
	CExDrawButton	m_btnNot;
	CExDrawButton	m_btnTrue;
	CExDrawButton	m_btnNext;
	CExDrawButton	m_btnParenthesis;
	CExDrawButton	m_btnX;
	CExDrawButton	m_btnInit;
	CExDrawButton	m_btnAdd;
	CExDrawButton	m_btnSTDel;
	CExDrawButton	m_btnSave;
	CExDrawButton	m_btnSTClose;
	CExDrawButton	m_btnSTConfigDlg;
	CExDrawButton	m_btnSTRun;
	CExDrawButton	m_btnSystemTrading;
	CExDrawButton	m_btnSaveConfig;
	CXListCtrl		m_InputList;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgStrategyList)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Variables
public:
	CString m_strRootPath;
	CString m_szDataDir;
	CString m_szUserDir;
	CString m_szSTFilePath, m_szSTName;
	CGridCtrl m_GridSTList;
	CString m_szSTFilePath4Setup;		// 전략설정을 위한 전략명.
										// 전략설정을 하고나서 저장을 다른 이름으로 할 경우 안내메시지를 보내기 위해서 저장한다.

	int		m_nFlagCnt;					// 변경여부, 변경시 카운트증가
	HWND	m_hViewHandle;
	CFont	m_Font;

// Implementation
public:
	CRect	m_rcSTOption;
	void	InitSTOption();
	void	NotifySTChangeState(int nState, LPCSTR szSTName);
	int		GetGridSTRowCount();
	void	OnClickGridSTList(int nRow, int nCol);

protected:
	
	void	InitGrid();
	CRect	GetRect(int nId);
	BOOL	AddRowData(CString szFullSTPath, LPCTSTR lpSfgName, int nType, int nSTPos = -1);
	void	LoadST(LONG lpData);
	int		GetNewSTPosNo();
	BOOL	DeleteAllItem();
	void	SetSignalInfoGridAll(long lShape, long lColor,LPCTSTR lpAppName,LPCTSTR lpShapeKeyName,LPCTSTR lpColorKeyName);

	void	SaveST(LPCTSTR lpSTName, BOOL bImsi);
	int		CheckFileSave(BOOL bCheck, CString& szSTFilePath); // 전략저장여부를 확인한다.
	void	SaveVariable(CString& strGrid, int nIndex, LPCSTR strSTPath);
	void	InputModify(int nRow, CString& szTitle);

	BOOL	SaveRunTempFile();	// 전략실행을 위한 임시파일을 만든다.

	void	InitColumn();

	long		m_lPosOfGirdBottom;
	CDlgTimeSelector m_dlgTimeSelector;
	long m_lRowEditing;
	COLORREF	m_crBk;
	CBrush		m_brBkColor;
	char		m_chLastIndex;

	// Generated message map functions
	//{{AFX_MSG(CDlgStrategyList)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnBtnInit();
	afx_msg void OnBtnDown();
	afx_msg void OnBtnUp();
	afx_msg void OnBtnAnd();
	afx_msg void OnBtnOr();
	afx_msg void OnBtnNext();
	afx_msg void OnBtnNot();
	afx_msg void OnBtnTrue();
	afx_msg void OnBtnParenthesis();
	afx_msg void OnBtnX();
	afx_msg void OnBtSave();
	afx_msg void OnBtnDelete();
	afx_msg void OnBtnEdit();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnSTDelete();
	afx_msg void OnBnClickedSTConfigDlg();
	afx_msg void OnBnClickedSTClose();
	afx_msg void OnBnClickedSTRun();
	//}}AFX_MSG
	LRESULT OnSTController( WPARAM wParam, LPARAM lParam);
	LRESULT OnUpdateSTSfg( WPARAM wParam, LPARAM lParam);
	LRESULT OnSaveToUserST(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSystemtrading();
	afx_msg void OnBnClickedButtonSaveconfig();
};

class CHelperSTTitle
{
public:
	static int GetSTName(CString &szTitle, CString &rString)
	{
		int nFind = szTitle.Find("(");
		if(nFind<0) return nFind;

		rString = szTitle.Left(nFind);

		return nFind;
	}

	// 
	static int GetInputData(CString szInput, CStringArray& inArr)
	{
		int i=0;
		int nFind = szInput.Find(',');
		CString szTmp;
		while(nFind>0)
		{
			szTmp = szInput.Left(nFind);
			szInput = szInput.Mid(nFind+1);

			inArr.Add(szTmp);
			nFind = szInput.Find(',');
		}

		inArr.Add(szInput);
		return inArr.GetSize();
	}

	static CString MakeTitle(LPCSTR szSTName, CStringArray& inArr)
	{
		CString szTitle, szTmp;
		int nCount = inArr.GetSize();
		szTitle = szSTName;
		if(nCount==0) return szTitle;

		szTitle+= "(";
		for(int i=0; i<nCount; i++)
		{
			szTitle += inArr.GetAt(i);
			if(i+1<nCount) szTitle += ",";
		}
		szTitle += ")";
		return szTitle;
	}
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSTRATEGYLIST_H__39A7CB0D_CC84_4134_80EA_BE1E6803DB42__INCLUDED_)
