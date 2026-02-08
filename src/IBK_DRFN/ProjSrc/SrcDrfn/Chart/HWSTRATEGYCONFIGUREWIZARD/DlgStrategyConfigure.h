#if !defined(AFX_DLGSTRATEGYCONFIGURE_H__89885CAC_33BB_4978_B681_845B2E4B7AB4__INCLUDED_)
#define AFX_DLGSTRATEGYCONFIGURE_H__89885CAC_33BB_4978_B681_845B2E4B7AB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgStrategyConfigure.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgStrategyConfigure dialog
// {{ 메인쪽 라이브러리 링크 ( 확장 dll )  }}
//#include "../_#HanWha/_include/SharedBase.h"
#include "../../inc/OcxDLLDef.h"
#include "../../inc/ISTRunManager.h"			// Interface For ISTRunManager

#include "OutLookBarDlg.h"
#include "DlgStrategyList.h"
#include "../../inc/IAUDataMonitorMng.h"	// UUID_IAUDataMonitorManager : 콜백함수 받는데 사용
#include "../../inc/IMainInfoMng.h"

#define MYMETHOD_PROLOGUE(theClass, localClass) \
	theClass* pThis = \
	((theClass*)((BYTE*)this - offsetof(theClass, m_x##localClass))); \
	pThis;


class CDlgStrategyConfigure : public CDialog// public CSharedBase 
{
// Construction
public:
	CDlgStrategyConfigure(CWnd* pParent = NULL);   // standard constructor
	CWnd*	GetStrategyList() { return &m_dlgStrategyList; }
	void	UpdateSTList(int nState, CString szSTPath);
	CString	GetSTPathSelected(CString& szSTType);
	void	LoadEditor(CString szScriptName);
	short	CopyBaseScriptFile();

// Dialog Data
	//{{AFX_DATA(CDlgStrategyConfigure)
	enum { IDD = IDD_DLG_CONFIGURE };
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgStrategyConfigure)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void	FormInit() {}
	void	FormLoad();
	int		RunST(BOOL bImsiRun, CString& szRunFile);
	BOOL	IsSTSelected();		// 전략이 선택된 상태에서 실행된 것인지 확인한다.
	void	SelectSTItem(CString szItemName, CString szSelSTPath);
	LONG	GetOutBarColor(COLORREF& clrOLBkgrnd, COLORREF& clrOLOutLine);

	CString			m_strUserID;
	CString			m_strRootPath;
	CString			m_strDataDir;
	CString			m_strImageDir;
	CString			m_strUserDir;
	ISTRunManager*	m_pSTRunMng;				// 전략실행에 관련된 ISTRunManager 인터페이스

// Implementation
protected:
	void	Resize(int cx, int cy);
	CRect	GetRect(int nId);
	int					m_nWidthOfOutBar;
	COutLookBarDlg		m_dlgOutLookBar;
	CDlgStrategyList	m_dlgStrategyList;
	CString				m_szSelSTItem;

	// Generated message map functions
	//{{AFX_MSG(CDlgStrategyConfigure)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg	LRESULT OnGetDllSize(WPARAM wParam, LPARAM lParam); //화면로딩시 화면크기를 알기위해 호출됨.

	IAUDataMonitorManager*	m_pIAUDataMonitorManager; // 28 : 데이터변경처리를 위한 인터페이스
	long					m_dwIAUDataMonitorManager;
	void InitLoad();
	static void CALLBACK DataMonitorCallbackProc(LPCSTR szKey, long pThisPoint, long dwKey, long dwSubKey, long dwNotifyData);

	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSTRATEGYCONFIGURE_H__89885CAC_33BB_4978_B681_845B2E4B7AB4__INCLUDED_)
