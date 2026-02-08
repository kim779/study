#if !defined(AFX_MAINWND_H__6E8C1897_6BC3_4FED_BD48_874CEE225A78__INCLUDED_)
#define AFX_MAINWND_H__6E8C1897_6BC3_4FED_BD48_874CEE225A78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainWnd.h : header file
//

#include "afxtempl.h"

struct _pninfo	{
	int	whsize;
	WORD	info;
};

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window

class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd(CWnd* pParent, CString name, struct _param* pInfo);

// Attributes
public:
	int		m_iDtUnit;
	int		m_iDtIndex;
	int		m_iMGap;
	int		m_iTGap;

	CWnd*		m_pwndChart;


	CString		m_strDay;
	CString		m_strCode;
	CString		m_strJName;


	bool		m_bHideMenuBtn;

	CString		m_sParam;

private:
	bool		m_bForeign;
	bool		m_bTransaction;
	bool		m_bTransactionS;
	bool		m_bExtrTr;

	int		m_iKey;
	int		m_iFPoint;
	int		m_iTRIndex;

	char*		m_pacRegion[MAX_INDEX];	// Map Region Info
	char*		m_pacGraph[MAX_INDEX];	// Map Graph Info
	char*		m_pcGDat;		// graph data info
	char*		m_pcTemp;	// temporary buffer


	CWnd*		m_pwndCtrl;
	CWnd*		m_pwndInput;
	CWnd*		m_pwndView;

	CFont*		m_pFont;

	CString		m_strDllName;
	CString		m_strIName;
	CString		m_strFName;
	CString		m_strEnvPath;	
	CString		m_strGrpPath;
	CString		m_strDatPath;


	CC_TotalApp*	m_pApp;

	// file
	_envInfo*	m_pEnvInfo;	// Environment info

	class CExtraMsg*	m_pExtraMsg;


	CArray<char*, char*>	m_arData;
	CArray<int, int>	m_arLen;
	struct _trData		m_TrData;
	CMap <int, int, struct _pninfo *, struct _pninfo *> m_mapPanel;


public:
	int	GetToken(char *pData, char token[]);

private:
	// components
	CWnd*	CreateWnd(int ctrlID, CWnd* pWnd);
	void	ResizeWnd(CRect wRC);

	void	DelPanels();

	void	InitPnInfo(struct _panel *panel);
	void	CreatePn();
	struct _pninfo*	GetPnInfo(int localID);
	int	GetPnSize(int ctrlID);

	// file IO
	bool	ReadEnv();
	bool	ReadGrp();
	bool	ReadData();
	bool	WriteEnv();
	void	WriteGrp();
	void	WriteDat();
	bool	ReMakeFile();

	void	SavePanel(struct _panel *panel);

	char*	GetMapInfo(int ikey = -1);
	char*	GetDat();
	bool	GetGrpFromDat(char *info);
	bool	SetGrpAtDat(char *info);

	// i/o
	void	SendRequest();
	void	SendRequest2();
	void	SendRequestS();
	int	WhichFOCode(LPCTSTR code);
	void	EnvProc();
	bool	CallEnvDlg(char *pInfo, bool bTotal);
	char*	CallMapDlg(char *pInfo);
	bool	CallStandardIndc(int kind);
	bool	CallToolDlg(char *pInfo);
	bool	TableShow(bool bShow);
	void	InitGraph();
	void	SyncMainChart(char *pInfo);
	void	SyncMap(char *pScreen);
	void	IndexChange(int index);
	BOOL	MainChartIsVarType();
	BOOL	IsAidAddable(LPARAM lParam);
	void	DoEvent(int nEvent);

//	void	CheckNewList();

	void	SyncCount();

	LRESULT	Oub(WPARAM wParam, LPARAM lParam);

#ifdef _DEBUG
void CMainWnd::WriteFile(CString strData);
#endif

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainWnd)
	BOOL m_visible;
	afx_msg void OnVisibleChanged();
	afx_msg BSTR GetProperties();
	afx_msg void SetProperties(LPCTSTR str);
	afx_msg BOOL RequestTR(LPCTSTR str);
	afx_msg BOOL Config();
	afx_msg long GetTotalDay();
	afx_msg long GetDisplayDay();
	afx_msg BSTR GetSelectTime();
	afx_msg void SetSelectTime(LPCTSTR DateTime);
	afx_msg BSTR GetSelectPrice();
	afx_msg void _SetTimeLine(LPCTSTR time);
	afx_msg void _RemoveTimeLine();
	afx_msg void SetOrderMode();
	afx_msg BOOL RequestTR2(LPCTSTR str);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	afx_msg LRESULT	OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnViewEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChartEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInputEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSiseEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCtrlEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnToolEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTableEvent(WPARAM wParam, LPARAM lParam);
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINWND_H__6E8C1897_6BC3_4FED_BD48_874CEE225A78__INCLUDED_)
