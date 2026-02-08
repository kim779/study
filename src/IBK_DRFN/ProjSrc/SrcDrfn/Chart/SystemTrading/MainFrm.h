// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__F38EAA1B_B3BE_4B73_8C17_79758A8693B7__INCLUDED_)
#define AFX_MAINFRM_H__F38EAA1B_B3BE_4B73_8C17_79758A8693B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../common_ST/Label.h"
#include "../common_ST/DropSource.h"

#ifndef _USESTD_OJTASO

#define _USESTD_OJTASO
#include <vector>
#include <algorithm>
using namespace std;

#endif // #ifndef _USESTD_OJTASO

typedef BOOL (WINAPI* FpShowConfigureDlg)(HWND, LPVOID, LPCTSTR, int);

class CScriptInfomation;
class CInnerFunctionInfo;
class CErrorInfomation;
class CLoadSystemTradingWorkspaceHelper;
class CLoadSystemTradingParserHelper;
class CLoadSystemTradingExtLibHelper;
class CClientSocket;
class CDynamicHelpView;
class CLoadSystemTradingEncoder;
class CLoadSystemTradingConfigDll;
class CScriptSecurity;
class CMainFrame : public CXTPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

public:
	CXTPDockingPaneManager	m_paneManager;

// Attributes
public:
	//KHD
	HWND m_hPctrWnd;
	//
	CXTPPropertyGrid m_wndPropertyGrid;
	CTreeCtrl m_wndClassView;
	CTreeCtrl m_wndSolutionView;
	CTreeCtrl m_wndResourceView;
	CEdit m_wndOutput;
	CFont m_fontOutput;

	CImageList m_ilClassView;
	CImageList m_ilSolutionView;
	CImageList m_ilResourceView;

	int		m_nState;
	int		m_nPropertyState;
	BOOL ShowWindowEx(int nCmdShow);

	CMap<int, int, CString, CString&> m_mapCompileMessage;

	CString		m_strNewFileName;

	CXTPPropertyGridItem*		m_pPrptyScriptName;
	CXTPPropertyGridItem*		m_pPrptyScriptComment;
	CXTPPropertyGridItem*		m_pPrptyScriptLanguage;
	CXTPPropertyGridItem*		m_pPrptyScriptCheck;

	CXTPPropertyGridItem*		m_pPrptyVariables;

	CXTPPropertyGridItem*		m_pPrptyBuy;
	CXTPPropertyGridItem*		m_pPrptyExitlong;
	CXTPPropertyGridItem*		m_pPrptySell;
	CXTPPropertyGridItem*		m_pPrptyExitshort;
	CXTPPropertyGridItem*		m_pPrptyShowSignalName;
	CXTPPropertyGridItem*		m_pPrptyShowSignalQty;
	CXTPPropertyGridItem*		m_pPrptyPosOfSignal;

	CXTPPropertyGridItem*		m_pPrptyIndex;

	CXTPPropertyGridItem*		m_pPrptyBull;
	CXTPPropertyGridItem*		m_pPrptyBear;

	CXTPPropertyGridItem*		m_pPrptyPassword;
	CXTPPropertyGridItem*		m_pPrptyUserID;
	CXTPPropertyGridItem*		m_pPrptyExpires;

	// 소켓 변수
	CClientSocket*				m_pMainClient;

	CLabel						m_wndStatusConnect;
	CProgressCtrl				m_ctrlCompileProgress;

	int							m_nCompileProgressCount;
	BOOL						m_bCompiling;
	BOOL						m_bMultiCompile;

	CDropSource					m_DropSource;
	
	enum {IMG_TREE = 0, IMG_STS, IMG_STS_LOCK, IMG_STS_CERT, IMG_STS_LOCK_CERT, IMG_STS_DLL, IMG_STS_LOCK_DLL,
						IMG_FUNC, IMG_FUNC_LOCK, IMG_FUNC_CERT, IMG_FUNC_LOCK_CERT, IMG_FUNC_DLL, IMG_FUNC_LOCK_DLL};

// Operations
public:
	CXTPDockingPaneManager* GetDockingPaneManager() {
		return &m_paneManager;
	}
	
	CXTPTabClientWnd m_MTIClientWnd;

	CXTPDockingPaneLayout* m_pPreviewLayout;

	CWnd* CreatePropertyView();

	vector<CScriptInfomation*>*	GetUserFuncInfo() {return &m_vtUserFuncInfo;}
	vector<CInnerFunctionInfo*>*	GetInnerFuncInfo() {return &m_vtInnerFuncInfo;}
	vector<CInnerFunctionInfo*>*	GetInnerFuncInfo_YES() {return &m_vtInnerFuncInfo_YES;}
	int	IsUserFunctionKeyword(LPSTR p_lpszChars, int p_nLength);
	BOOL	IsInnerFunctionKeyword(LPSTR p_lpszChars, int p_nLength, UINT nScriptType);
	void	CheckupBackupFiles();
	void	InitStrategyList();
	void	InitUserFuntionList();
	void	InitInnerFunctionList(LPSTR lpszFileName);
	void	AddInnerFunction(CString strFunctionName, CString strInputs, int nIndex, UINT nScriptType);
	void	SetOutputText(int nMessage, LPCSTR lpszText, CErrorInfomation* pErrorInfo = NULL);
	void	MakeFileUserFunction(LPCSTR lpszFunctionName);
	void	MakeFileMainStrategy(LPCSTR lpszStrategyName, UINT nStrategyType);
	void	MakeFileAllScirpt(LPCSTR lpszNames);
	void	CollectCompileMessage();
	void	LoadConfigDlg();
	void	OpenPropertyDlg(LPCSTR szFile, int nFileType);
	void	ChangeCompiledOption(LPCSTR lpszName, UINT nFileType, UINT nCompiled);
	void	LoadSystemTradingParser(UINT nScriptType);
	void	DeleteSystemTradingParser();
	void	AddIndexFunctionList(LPCSTR lpszStrategyName, BOOL bUsed);
	void	InitIndexFunctionStrategy();
	void	CloseClientSocket();

	void	OpenNewFile(WPARAM wParam, LPARAM lParam);
	void	ChangedProperty(WPARAM wParam, LPARAM lParam);

	void	DeleteStrategyScript(UINT nID);
	void	DeleteFunctionScript(UINT nID);

	void	CompileSucceed(LPVOID prData);
	void	ChangeProperty(BOOL bActive, LPCSTR lpszName = NULL, UINT nID = 0);
	BOOL	ScanScriptFile(LPSTR lpszName, UINT nFileType, UINT nScriptType);
	void	ShowFunctionInStrategy(LPCSTR lpszName, UINT nFileType, UINT nScriptType);
	void	DeleteFunctionInStrategy(LPCSTR lpszName);
	void	ScanFunctionInStrategy();

	CString		GetShapeString(int nIndex);
	CString		GetLineType(int nIndex);

	void	ConnectToCompileServer();
	void	UploadCPPFile(UINT nStrategyType);
	void	UploadCPPFile_Debug(UINT nStrategyType);
	void	CreateNewDll(LPSTR lpszFilePath, UINT nStrategyType);
	void	CreateNewDll_Debug(LPSTR lpszFilePath, UINT nStrategyType);
	void	WriteDllFile(LPSTR lpszData, UINT nFileSize);
	void	UploadUserFunctionCPPFile(LPSTR lpszScriptName);
	void	UploadUserFunctionCPPFile_Debug(LPSTR lpszScriptName);
	void	CreateNewDllFunc(LPSTR lpszFilePath);
	void	CreateNewDllFunc_Debug(LPSTR lpszFilePath);
	void	WriteDllFileFunc(LPSTR lpszData, UINT nFileSize);
	void	WriteEndDllFile();
	void	WriteEndDllFileFunc();
	void	UploadZipFile(LPSTR lpszZipName);
	void	CreateNewZip(LPSTR lpszFilePath);
	void	WriteZipFile(LPSTR lpszData, UINT nFileSize);
	BOOL	ExtractFile(LPCSTR lpszFile);
	void	DisplayZipError(LPSTR lpszNames);

	static void	NotifyExtract(BOOL bStrategy, LPCSTR lpszName);

	BOOL	CreateSubStatusWindow();
	BOOL	CreateCompileStatusWindow();
	void	SetConnectionStatusTest(LPCSTR lpszText, COLORREF clrText = RGB(0, 0, 255));
	void	FindScript(CString strText, BOOL bMatchCase, BOOL bWholeWord, BOOL bStrategy, BOOL bFunction);
	BOOL	FindTextInFile(CString strText, BOOL bMatchCase, BOOL bWholeWord, CString strFile);
	int		FindStringHelper(LPCTSTR pszFindWhere, LPCTSTR pszFindWhat, BOOL bWholeWord);

	void	LoadExtLib();
	void	ReloadScriptInfoToExtLib();

	void	LoadEncoder();
	BOOL	EncodingFile(LPCSTR lpszSource, LPCSTR lpszDest, LPSTR lpszLine1 = NULL, LPSTR lpszLine2 = NULL, LPSTR lpszLine3 = NULL);
	BOOL	DecodingFile(LPCSTR lpszSource, LPCSTR lpszDest, LPSTR lpszLine1 = NULL, LPSTR lpszLine2 = NULL, LPSTR lpszLine3 = NULL);

	void	SingleComileDllMain(BOOL bMakeDll, LPSTR szName, UINT nScriptType, CScriptSecurity* pInfo = NULL);
	void	SingleComileDllFunction(BOOL bMakeDll, LPSTR szName, UINT nScriptType, CScriptSecurity* pInfo = NULL);

	void	StartCompileProgress();
	void	StartCompileProgressZip();
	void	StopCompileProgress();
	void	StopCompileProgressZip();

	void	StopCompiling(LPCSTR lpszName);

	void	OpenScriptFileFromDll(LPCSTR lpszDllFile, LPCSTR lpszName, UINT nFileType, LPSTR lpszTemp);
	void	ApplyView();
	
	BOOL	VerifyScriptNameBySavedName(LPCSTR lpszScriptName);
	BOOL	VerifyScriptNameByFileName(LPCSTR lpszScriptName);
	BOOL	VerifyScriptNameByInnerFunction(LPCSTR lpszScriptName, WPARAM wParam);
	BOOL	VerifyScriptNameByReservedWord(LPCSTR lpszScriptName, WPARAM wParam);

	void	SetServerMessage(LPCSTR lpszMessage, UINT nLevel);
	void	ShowDynamicHelp(LPCSTR lpszText);

	//변경: KHD : 삼성 요청으로 변경 2006.12.13
	void	MakeSTFile(CString strSTName, LPCSTR szSDFileName=NULL);
	void	MakeSTFileConfig(LPCSTR szSTFileName, LPCSTR szSDFileName, LPCSTR szSTName);
	///////////////////////////////////////////////////
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	afx_msg void OnCustomize();
	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);

	afx_msg void OnEditState(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateEditState(CCmdUI* pCmdUI);

private:
	CLoadSystemTradingWorkspaceHelper*		m_pLoadWorkspace;
	CLoadSystemTradingParserHelper*			m_pLoadPaser;
	CLoadSystemTradingExtLibHelper*			m_pLoadExtLib;
	CLoadSystemTradingEncoder*				m_pLoadEncoder;
	CLoadSystemTradingConfigDll*			m_pLoadConfig;

	vector<CScriptInfomation*>				m_vtStrategyInfo;
	vector<CScriptInfomation*>				m_vtUserFuncInfo;
	vector<CInnerFunctionInfo*>				m_vtInnerFuncInfo;
	vector<CInnerFunctionInfo*>				m_vtInnerFuncInfo_YES;

	vector<LPCSTR>							m_vtIndexFunction;

	CCriticalSection	m_CriticalSection;
	CFile*				m_pFileDll;
	UINT				m_nStrategyType;

	CDynamicHelpView*	m_pViewHelp;

	CStringArray		m_starBackupFile;

	int					m_nScriptNameCount;
	CStringArray		m_starScriptName;

protected:  // control bar embedded members
	CXTPStatusBar  m_wndStatusBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnFileNewStrategy();
	afx_msg void OnFileNewFunction();
	afx_msg void OnFileOpenStrategy();
	afx_msg void OnFileOpenFunction();
	afx_msg void OnFileSaveAll();
	afx_msg void OnUpdateFileSaveAll(CCmdUI* pCmdUI);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnCenter();
	afx_msg void OnFindandreplaceFindinfiles();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateConnectoserver(CCmdUI* pCmdUI);
	afx_msg void OnConnectoserver();
	afx_msg void OnTradestationView();
	afx_msg void OnYestraderView();
	//}}AFX_MSG
	afx_msg void OnEmptyCommand();
	afx_msg void OnEmptyCommandRange(UINT);
	afx_msg void OnShowView(UINT);
	LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	LRESULT	OnDblClickWorkspace(WPARAM p_wParam, LPARAM p_lParam);
	LRESULT	OnDblClickTree(WPARAM p_wParam, LPARAM p_lParam);
	LRESULT OnCloseConfigDialog(WPARAM wParam, LPARAM lParam);
	LRESULT OnDelScript(WPARAM wParam, LPARAM lParam);
	LRESULT OnSingleCompileDLLMain(WPARAM wParam, LPARAM lParam);
	LRESULT OnSingleCompileDLLFunction(WPARAM wParam, LPARAM lParam);
	LRESULT OnSingleCompileDLLMain_YES(WPARAM wParam, LPARAM lParam);
	LRESULT OnSingleCompileDLLFunction_YES(WPARAM wParam, LPARAM lParam);
	LRESULT OnCompileDLL(WPARAM wParam, LPARAM lParam);
	LRESULT OnViewProperty(WPARAM wParam, LPARAM lParam);
	LRESULT OnDblClickOutput(WPARAM wParam, LPARAM lParam);
	LRESULT OnDblClickFind(WPARAM wParam, LPARAM lParam);
	LRESULT OnAnyTrader(WPARAM wParam, LPARAM lParam);
	LRESULT OnVerifyScriptName(WPARAM wParam, LPARAM lParam);
	LRESULT OnStartDrag(WPARAM wParam, LPARAM lParam);
	LRESULT OnSingleCompileDLLMainTree(WPARAM wParam, LPARAM lParam);
	LRESULT OnSingleCompileDLLFunctionTree(WPARAM wParam, LPARAM lParam);
	LRESULT OnMultiCompileDLLMainTree(WPARAM wParam, LPARAM lParam);
	LRESULT OnMultiCompileDLLFunctionTree(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetCompileStateTree(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__F38EAA1B_B3BE_4B73_8C17_79758A8693B7__INCLUDED_)
