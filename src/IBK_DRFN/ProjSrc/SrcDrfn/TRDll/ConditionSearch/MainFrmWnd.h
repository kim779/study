#if !defined(AFX_MAINFRMWND_H__9E654379_A5D8_4224_932A_562D2E5F1ADF__INCLUDED_)
#define AFX_MAINFRMWND_H__9E654379_A5D8_4224_932A_562D2E5F1ADF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainFrmWnd.h : header file
//




/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd frame

#include "Draw3DRectangle.h"
#include "AddControl/QuickTab.h"

#include "../../DrCommon/Dr_Control/DrSplitterCtrl.h"

#include "ConditionListView.h"
#include "ConditionView.h"

typedef struct
{
	CStringArray strKey;
	CStringArray strGroupName;
} ST_GwanSimGroup;

class CMainFrmWnd : public CFrameWnd
{
public:
	CMainFrmWnd(CString strFormCode, CWnd* pScreenWnd);  
	virtual ~CMainFrmWnd();
	
	BOOL m_bIndustry;
	BOOL m_bIsFnUser;
	BOOL m_bCreateWnd;
	BOOL m_bPortAdd;
	BOOL IsExistPortItem(CString strKey);

	int ParseGroup(char* pReceiveBuff, CStringArray& arrKey, CStringArray& arrName);
	int ParseList(char* pReceiveBuff, CStringArray& arrName);
	ST_GwanSimGroup m_GwanSimGroup;
	
	void SetInitFormCode(CString szFormCode);
	void SetInit(CString szFormCode = "");
	void SetToolTip();
	
	//# Port!
	CList <PortGroupInfo, PortGroupInfo&> m_listPortInfo;
	void SetInit_LibPortPolio(BOOL bInit);
	void GetPortData(CList <PortGroupInfo, PortGroupInfo&> &listPortInfo);

	CMap <CString,LPCTSTR, PortGroupInfo, PortGroupInfo&> m_mapPortItem;
	
	//# Language!
	CList	<LanguageInfo, LanguageInfo&>   m_listLanguageInfo;
	//CLoadSystemTradingExtLibHelper  *m_plibLanguageHelp;
	
	void SetInit_LibLanguage(BOOL bInit);
	BOOL GetData_LanguageList(CList	<LanguageInfo, LanguageInfo&> &listLanguageInfo);
	CString GetData_LanguageScript(CString strLanguageName);
	
	HINSTANCE			m_hLanguageMng;
	typedef void		(* FpNotifyList)(CPtrList*, long);
	typedef BOOL		(* EX_InitExtLib)(FpNotifyList, LPSTR);
	typedef void		(* EX_ExitExtLib)(FpNotifyList);
	typedef CPtrList*	(* EX_GetScriptList)(void);
	typedef BOOL		(* EX_GetScriptInfo)(LPSTR, CString&);
	//typedef BOOL		(* EX_GetScriptInfo)(LPSTR, LPBYTE);
	typedef void		(* EX_ReloadScriptInfo)(LPSTR);
	
	EX_InitExtLib       Lib_InitExtLib;
	EX_ExitExtLib       Lib_ExitExtLib;
	EX_GetScriptList    Lib_GetScriptList;
	EX_GetScriptInfo    Lib_GetScriptInfo;
	EX_ReloadScriptInfo Lib_ReloadScriptInfo;

	int				m_nSeparateForm;
	CStringArray m_arrInitCode;
	
public:
	CBrush m_brushStatic;
	CFont m_fontDefault;
	COLORREF	m_clrMainSkin;
	
	CDraw3DRectangle		m_Draw3DRectangle;
	
	CStatic					m_ctrlMakeQuickBar;
	CBitmapButton			m_btnMakeQuick;
	/*CQuickTab				m_tabQuick;*/
	
	//CDrSplitterCtrl			*m_pctrlSplitter;
	CDrSplitterCtrl			m_ctrlSplitter;
	CConditionListView		*m_pviewConditionList;
	CConditionView			*m_pviewCondition;
	
	BOOL m_bLeftExp;
	BOOL m_bTopExp;
	int	 m_nTopExp;
	BOOL m_bFirstState;
	
	int m_nNowDataType;			//# 0 : Condotion, 1 : Portfolio, 2 : File, 3 : Language, 4 : User Patten
	
	XMLTreeInfo				m_stNowXMLTreeInfo;		//#<= 선택된 지표
	PortfolioGroup			m_stNowPortfolioGroup;
	XMLTreeInfoFile			m_stNowXMLTreeInfoFile;
	
	BOOL m_bLoadFile_User, m_bLoadFile_Sys;

	COLORREF	m_clrBkgnd;
	ST_TABCOLOR m_stTabColor;
	CWnd*		m_pScreenWnd;
	CString		m_strLinkCode;
	
	
public:
	CWnd* GetWnd(CString strType);
	
	BOOL GetLoadWindowState(CString strFile);
	BOOL SetSaveWindowState(CString strFile);
	
	void SetControlEnable(BOOL bState);
	
	BOOL SetStandbyAlert() { return ((CConditionMakeWnd*)GetWnd("MAKE"))->SetStandbyAlert(); }
	
	int GetFixFieldCnt();
	int GetMoveFieldCnt();
	
	int GetMatchLogic(CList <HeadInfoType, HeadInfoType&>	*plistFieldInfo, 
		CString &strMatchLogic);
	
	void SetQuickInfo_UpDate();
	void SetQuickInfo_Delete(CString strType, CString strFolderPath = "", CString strFilePath = "");
	
	void SetReloadUserPatten();
	
	void SetSharedDataToSend(ShardData *pstShardData);
	void SetSharedDataToCode(CString strCode);
	
	BOOL SetPortfolioReg(CString strOpenDlg, CStringArray *psaCode);
	
	void OnSize(CRect reClient);
	void OnSize_Splitter(CRect reClient, int nSplitterPos, BOOL bInvalidate);
	
	void SetLeftExpend(BOOL bState);
// 	void ChangeSkinColor();
	void ChangeSkinColor(COLORREF clrBkgnd, ST_TABCOLOR stTabColor);
	BOOL GetBackDateAndTime(CString& strDate, CString& strTime);
	void SetSelectCondition();
	void GetTabClr(COLORREF* clrBkgnd, ST_TABCOLOR* stTabColor);

	void OnDBClickResultGrid(CString strCode);  // 20110118 박찬모 검색 결과 Double Click 를 통한 종목 연동 기능

	CFont*		GetFont(BOOL bBold = FALSE);
	COLORREF	GetColor(int nColor);
	CString		Variant(int nComm, CString strData = "");
	void		SetBtnImg(CfxImgButton *pBtn, CString strFileName);
	void		SetBtnImg(CfxImgButton *pBtn, int nTextLen, BOOL bBold = FALSE);
	COLORREF	GetAxColor(int nColor);
	HBRUSH		GetAxBrush(COLORREF clr);
	void		SendTR(CString strTRCode, char* szSendData, int nDataLen, int nKey, char cStat);
	void		ReceiveData(LPCSTR szTR, LPVOID aTRData, long dwTRDateLen , int nKey );
	CWnd*		GetModuleMainWnd();
	CPen*		GetAxPen(COLORREF clr, int nWidth, int nStyle);
	
	BOOL		GetKSPUpMst(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName, int nOption = 0);
	BOOL		GetKSQUpMst(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName, int nOption = 0);

	void		GetData_PortGroup(CStringArray& saKey, CStringArray& saGroupName);
	BOOL		GetData_PortItem(CStringArray& saCode, CStringArray& saName, int nKey);
	void		RegPortItem(CStringArray* pSaItemCode);
	
	void		SyncExcavationTreeListFile(CString strFilePath);

	BOOL		GetStockMasterData(CStringArray &arrCode, CStringArray &arrName, UINT Jgub, UINT Upgub);
	BOOL		GetUpMasterData(CStringArray *&psaCode, CStringArray *&psaName, int jgub);
	CString		GetMarketNameToCode(CString strMaketCode);

	void		OpenScreen(CString strScreenNo);
	int			SetUserMaster(LPCSTR szKey, int nCount, CStringArray *pArrCode, CStringArray *pArrName);

	COLORREF	GetMainBackColor();

	void		GetAccNo(CStringArray* psaAccNo);
	void		RequestHaveItem(CString strAccNo);
	void		SendHaveItem(CStringArray* psaHaveItem);

	CString		GetSelectedCondition();
	void		SaveSelectedCondition();

	void		OnLanguageReload();

	BOOL		GetMemberCompany(CStringArray &arrCode, CStringArray &arrName);

	void		ListViewSearchEditReset();
	
public :
	void SetLoadXml();
	void SetJobFolder();
	
	//## System function START ##
	CList	<XMLTreeInfoFolder, XMLTreeInfoFolder&> m_listUserInfoFolder;
	CList	<XMLTreeInfoFolder, XMLTreeInfoFolder&> m_listSysInfoFolder;
	CList	<XMLTreeInfoFolder, XMLTreeInfoFolder&> m_listExcavationFolder;
	CList	<QuickList, QuickList> m_listQuickInfo;
	
	CString GetUserFilePath(CString strOnlyFileName);
	CString GetFilePath(CString strLoadType);
	CString GetFolderPath(CString strLoadType);
	
	void SetDeleteFolderList(CList	<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList);
	
	void SetControlState(BOOL bState);
	
	BOOL SetLoadTreeXml(CString strType);
	BOOL SetSaveTreeXml(CString strType, CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList);
	void SetSaveTreeXml_User(BOOL bTreeReload = FALSE);
	
	void SyncTreeListFile(CString strFilePath);
	void GetPathToList(CString strRootFolderPath, 
		CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList);
	BOOL GetXMLToList(CString strFilePath, CString strType,
		CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList);
	void ProcessDeleteFolder(CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pSourceList, 
		CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pDestList);
	void ProcessInsertFolder(CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pSourceList, 
		CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pDestList);
	
	void ProcessDeleteFile(CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pSourceList, 
		CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pDestList);
	void ProcessInsertFile(CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pSourceList, 
		CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pDestList);
	
	BOOL GetFindFile(CString strFilePath,
		CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pFileList);
	BOOL GetFindFile(CString strFolderPath, CString strFilePath,
		CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList);
	BOOL GetFindFolder(CString strFolderPath, 
		CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList);
	
	BOOL GetFileSetState(CString strType, CString strFolderPath, CString strFilePath,
								CString &strAlertReg, BOOL &bUnitCan);				
	
	
	POSITION GetPosFolder(CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList, 
								CString strFolderPath);
	POSITION GetPosFile(CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pFileList, 
								CString strFolderPath, CString strFilePath);
	
	
	BOOL SetLoadQuickXml();
	void SyncQuickList(CString strXmlFilePath);
	BOOL SetLoadQuickList(CString strXmlFilePath, 
								CList<QuickList, QuickList> &listQuickInfo);
	BOOL SetSaveQuickXml(CList<QuickList, QuickList> *plistQuickInfo);
	
	
	void SetSendMainMsg(CString strMsg);
	int SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = PROGRAM_TITLE, UINT nType = MB_OK);

	BOOL FnUserConDel(LPCSTR strFolderPath, LPCSTR strFilePath);	// 사용자조건삭제.

	void PutLinkData(CString strType, CString strCode);
	LPCSTR GetScreenKey() { return m_szScreenKey;}
	//void LoadlanguageEditor(CString szScriptName);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrmWnd)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	int m_nSplitterPos;	//0000145: 조건검색 좌측탭 Split 조정위치 기억기능 추가
	CString m_szScreenKey;
	//	virtual ~CMainFrmWnd();
	
	// Generated message map functions
	//{{AFX_MSG(CMainFrmWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	afx_msg void OnBtnMakeQuick();
	afx_msg void OnDisplayToolTip(NMHDR *pNMHDR, LRESULT* pResult);
	
	afx_msg LRESULT OnNotifyConditionResult(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotifyConditionMake(WPARAM wParam, LPARAM lParam);
	
	afx_msg LRESULT OnNotifyTabCtrl(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotifyQuick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotifyXMLTree(WPARAM wParam, LPARAM lParam);
	
	afx_msg void OnNotify_PortPolio(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotify_Language(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRmsgCheckMenu(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRMWND_H__9E654379_A5D8_4224_932A_562D2E5F1ADF__INCLUDED_)
