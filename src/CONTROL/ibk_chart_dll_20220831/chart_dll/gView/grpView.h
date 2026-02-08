#if !defined(AFX_GRPVIEW_H__1807177A_BC33_4B72_9F4F_B776DEBDA800__INCLUDED_)
#define AFX_GRPVIEW_H__1807177A_BC33_4B72_9F4F_B776DEBDA800__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "afxtempl.h"

class	CAxisGViewApp;
class	CFileIO;


struct _pninfo	{
	int	whsize;
	WORD	info;
};

class CGrpView : public CWnd
{
public:
	CGrpView(CWnd* pParent, char* pcDllName);
	virtual ~CGrpView();

public:

private:
	bool		m_bAutotran;
	bool		m_bTransaction;
	bool		m_bTransactionS;
	bool		m_bExtrTr;

	int		m_iOS_mKind;		// original save mkind
	int		m_iOS_dUnit;		// original save dunit
	int		m_iOS_dIndex;		// original save dindex
	int		m_aiOS_RgnCnt[MAX_INDEX];	// original save Region Count
	int		m_aiOS_GrpCnt[MAX_INDEX];	// original save Graph Count

	int		m_iDtIndex;
	int		m_iTRIndex;

	int		m_iListKind;
	int		m_iSelect;

	char		m_acDllName[9];		// fixed by schbang 2006.02.20 to skip error in debug mode
	char*		m_pacRegion[MAX_INDEX];	// Map Region Info
	char*		m_pacGraph[MAX_INDEX];	// Map Graph Info
	char*		m_pcGDat;		// graph data info
	char*		m_pcTemp;	// temporary buffer


	CAxisGViewApp*	m_pApp;
	
	HINSTANCE	m_hiGComLib;
	HINSTANCE	m_hiGMainLib;
	HINSTANCE	m_hiGDlgLib;

	CFont*		m_pFont;

	CWnd*		m_pwndView;	// Parent
	CWnd*		m_pwndInput;
	CWnd*		m_pwndChart;
	CWnd*		m_pwndSise;
	CWnd*		m_pwndCtrl;
	CWnd*		m_pwndTool;
	CWnd*		m_pwndTable;
	CWnd*		m_pwndKwansim;
	CWnd*		m_pwndAid;

	//class CConfigBar*	m_pCfg;
	class CConfigBarPru* m_pCfg;
	class CGroupList*	m_pGroupList;
			
	struct _envInfo*	m_pEnvInfo;	// Environment info

	struct _trData		m_TrData;

	CArray<char*, char*>	m_arData;
	CArray<int, int>	m_arLen;
	CArray<struct _codeinfo *, struct _codeinfo *>	m_arCode;
	CMap <int, int, struct _pninfo *, struct _pninfo *>	m_mapPanel;



public:
	void	ResizeWnd(CRect wRC, bool bPrint = false);

private:
	// Init
	int	Init();
	CWnd*	CreateWnd(int ctrlID, CWnd* pWnd);

	void	DelPanels();

	void	InitPnInfo(struct _panel *panel);
	void	CreatePn();
	struct _pninfo*	GetPnInfo(int localID);
	int	GetPnSize(int ctrlID);

	// file IO
	bool	ReadEnv(CString strEnvPath, bool bLoadJCOD = false);
	bool	ReadGrp(CString strGrpPath);
	bool	ReadDat(CString strDatPath);
	bool	WriteEnv(CString strEnvPath, bool bSaveJCOD = false);
	void	WriteGrp(CString strGrpPath);
	void	WriteDat(CString strDatPath);	

	bool	SaveGraph(CString strPath);
	bool	LoadGraph(CString strPath);

	void	SavePanel(struct _panel *panel);

	char*	GetMapInfo(int iKey = -1);
	char*	GetDat();
	bool	GetGrpFromDat(char *info);
	bool	SetGrpAtDat(char *info);
	


	void	SendRequest();
	void	SendRequest2();
	void	SendRequestS();
	void	OnDrop(WPARAM wParam, LPARAM lParam);
	char*	OpenCodeDlg(int kind, UINT lParam);
	bool	CallStandardIndc(int kind);


	void	EnvProc();
	void	ToolEnvProc();
	void	PrintProc();
	bool	ControlShow(int iID, bool bShow = true);
	void	SyncMainChart(char *pInfo);
	void	SyncMap(char *pScreen);
	void	IndexChange(LPARAM lParam);

	void	RemoveCodeList();
	void	InitCodeList();
	void	SelectGroup(CString codelist, int kind);
	char*	GetNearCode(bool bPre, CString searchCode);
	// 20090928 : ADD : RSI 지표 추가
	void	CheckNewList();

	//** 2005.11.16 macho
	BOOL	CaseCheck(int gkind);

	// Dlg
	bool	CallEnvDlg(char *pInfo, bool bTotal);
	char*	CallMapDlg(char *pInfo);
	bool	CallToolDlg(char *pInfo);
	void	CallSaveDlg();
	bool	CallPnToolDlg(char* pvInfo);
	bool	CallPnAidDlg(char* pvInfo);
	bool	CallYAxisConfing(char* pcInfo);
	int	CallYShowConfingByMove();
	
	LRESULT	Oub(WPARAM wParam, LPARAM lParam);

	struct _envInfo*	GetEnv();

#ifdef _DEBUG
	void	WriteFile(CString strData);	//임시
#endif
	



public:
	//{{AFX_VIRTUAL(CGrpView)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CGrpView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg LRESULT	OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnViewEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChartEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInputEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSiseEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCtrlEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnToolEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCfgEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTableEvent(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnInfoEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAidEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetKwansim(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFromMain(WPARAM wParam, LPARAM lParam);
	//** macho add
	afx_msg LRESULT OnViewSendTR(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

//** macho add
private:
	bool GetControlShow(int iID);
	CWnd* m_trSender;

	void ClearBuffer();
	LPSTR AllocBuffer(int size);

	BOOL MainChartIsVarType();
public:
	void SendTR(CWnd* sender, CString strCode, LPCTSTR strData, int nSize, char stat = NULL);
};



//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_GRPVIEW_H__1807177A_BC33_4B72_9F4F_B776DEBDA800__INCLUDED_)
