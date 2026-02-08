#if !defined(AFX_CONDITIONMAKEWND_H__7007DA10_926A_4EC1_84BF_7A0FA919B1E4__INCLUDED_)
#define AFX_CONDITIONMAKEWND_H__7007DA10_926A_4EC1_84BF_7A0FA919B1E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConditionMakeWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConditionMakeWnd window

//# Control
#include "../../DrCommon/Dr_Control/DrInfoTip.h"
#include "../../DrCommon/Dr_Control/DrTabCtrl.h"
#include "../../../SrcSite/control/fx_misc/fxImgButton.h"

#include "./AddControl/LogicEditCtrl.h"
#include "./AddControl/PortEditCtrl.h"
#include "./AddControl/MapLoadCtrl.h"

#include "DataAdmin.h"
#include "LogicHelp.h"
#include "DlgHelp.h"

class CCMGrid;
class CConditionMakeWnd : public CWnd
{
	// Construction
public:
	CConditionMakeWnd();
	virtual ~CConditionMakeWnd();
	
private:
	COLORREF m_clrMapLoad;
	CFont m_fontDefault, m_fontBold;
	CBrush m_brushStatic, m_brushMapLoad, m_brushTagetEdit;
	CBrush m_brushTab;
	
public:
	COLORREF			m_clrMainSkin;
	CDrInfoTip			m_tipGridInfo;
	CDrInfoTip			m_tipLogicInfo;
	CDataAdmin			m_DataAdmin;    
	
	//# Control Attributes
	CStatic				m_ctrlLeftToolBar;
	CfxImgButton		m_btnCondiAdd;
	CComboBox			m_comboSelectGroup;
	
	CfxImgButton		m_btnExpand;
	CStatic				m_ctrlTargetBase;
	CComboBox			m_comboTargetBase;
	
	CButton				m_btnBackTestChk;
	CComboBox			m_comboBackTestDay;
	CComboBox			m_comboBackTestTime;
	
	CfxImgButton		m_btnGridTip;
	CButton				m_btnAlertChk;
	CfxImgButton		m_btnLanguage;
	CfxImgButton		m_btnDetail;
	
	CStatic				m_ctrlGridBar;
	CCMGrid*			m_pGridCtrl;
	
	CDrTabCtrl*			m_ptabSet;
	
	CMapLoadCtrl		m_ctrlMapLoad;
	
	CStatic				m_ctrlLogicBar;
	CStatic				m_ctrlCommonLogic;
	CLogicEditCtrl		m_ctrlCommonLogicEdit;
	CfxImgButton		m_btnCommonEx;
	CfxImgButton		m_btnBracket;
	CfxImgButton		m_btnBracketDel;
	
	CStatic				m_ctrlOrderLogic;
	CLogicEditCtrl		m_ctrlOrderLogicEdit;
	CfxImgButton		m_btnOrderEx;
	
	CStatic				m_ctrlPortfolio;
	CButton				m_btnPortCheck;
	CPortEditCtrl		m_ctrlPortEdit;
	//CLogicEditCtrl		m_ctrlGwansimEdit;
	
	CStatic				m_ctrlMatchVal;
	CPortEditCtrl		m_ctrlMatchValEdit;
	CfxImgButton		m_btnMatchValEdit;
	CButton				m_btnMatchValCheck;
	
	CStatic				m_ctrlFunBar;
	CfxImgButton		m_btnLogicTip;
	CfxImgButton		m_btnInit;
	CfxImgButton		m_btnPortDel;	
	CfxImgButton		m_btnFileSave;
	CfxImgButton		m_btnTargetEdit;
	CfxImgButton		m_btnSearchRng;
	
	CfxImgButton		m_btnChangeSubject;
	
	int					m_nSeparateForm;
	int					m_nSelectedTargetBase;	// 20110315 박찬모 과거시점 검색시 대상시간 "개별"로만 설정

	CLogicEditCtrl*		m_pShowLogicEdit;		// 20110623 박찬모 현재 보여지는 LogicEdit
	CStatic*			m_pShowLogicStatic;

	CLogicEditCtrl*		m_pShowPortfolioEdit;//관심그룹 설정용 Edit.

	CfxImgButton		m_btnNextCondition;
	CfxImgButton		m_btnPreCondition;
	CfxImgButton		m_btnDefaultValue;
	int					m_nTotCntCondition;
	int					m_nSelectCondition;

	CRect				m_rtTab;
	CRect				m_rtTabCondition;

	CDrTabCtrl*			m_pTabConditionForItemExcavation;
	BOOL				m_bLoadConditionForItemExcavation;

	BOOL				m_bInitIng;

	CStringArray		m_saMSTCode;
	CWnd*			m_pwndMain;
private:
	BOOL			m_bIndustry;
	
	
	CStringArray	m_saWorkDate;
	
	BOOL			m_bCreateEnd;
	
	
	CString			m_strGridInfoTip;
	int				m_nGridTipMaxLength;
	CString			m_strLogicInfoTip;
	int				m_nLogicTipMaxLength;
	
	int				m_nBackDateMaxCnt;
	CStringArray	m_saBackDateCnt;
	CUIntArray		m_iaBackTestDay;

	BOOL			m_bCommonExStateResizing;
	BOOL			m_bOneClickInitForSearch;

	CRect			m_rtConditionNavigationAndInit;
	
public:
	int				m_nTabIndex;
	int				m_nSelectTab;
	
	typedef struct tagSelectCondition{
		int nGroup;
		int	nIndex;			// 2009.11.09 -hidden-
		int nRow;
		CString strElement;
		CString	strTag;
		CString strText;  // 20110216 박찬모 관심종목 mgridctrl에 표시
	} SelectCondition;
	SelectCondition	m_stSelectCondition;
	
	int				m_nElementIndex;
	int				m_nTargetBaseIndex;
	
	BOOL			m_bMapShowState;
	XMLTreeInfoFile	m_stLoadedFile;
	BOOL			m_bSaveFlag;
	
	CList <MatchInfo, MatchInfo&>	m_listMatchInfo;
	
private:
	HINSTANCE		m_hConditionMng;
	typedef int(*_DLL_Open)(CStringArray *psaMSTCode);
	typedef void(*_DLL_Close)(void);
	typedef BOOL(*_DLL_GetLogicCalculate)(char *psData, CPtrArray *paBitSet, char **psResult);
	typedef BOOL(*_DLL_SetFree_PtrArray)(CPtrArray *paData);
	typedef void (*_DLL_SetUserFolderPath)(CString strUserFolderPath);
	
	_DLL_Open   Lib_Open;
	_DLL_Close  Lib_Close;
	_DLL_SetFree_PtrArray Lib_SetFree_PtrArray;
	_DLL_GetLogicCalculate  Lib_GetLogicCalculate;
	_DLL_SetUserFolderPath	Lib_SetUserFolderPath;
	
public:
	void SetNowXMLTreeInfo(XMLTreeInfo *pstXMLTreeInfo);
	void SetControlEnable(BOOL bState);
	void CheckMakeConditionFromFile();
	
	//## Debug 
	void DebugLogicPacket();
	void DebugMapInfoView(CStringArray &saMapInfo);
	
	BOOL GetLoadIniInfo();
	BOOL SetStandbyAlert();
	
	BOOL SetCondition(BOOL bMode, XMLTreeInfo* pstXMLTreeInfo);
	BOOL SetCondition_Port(BOOL bMode, XMLTreeInfo* pstXMLTreeInfo);	//[20110811:alzioyes]관심추가시의 처리루틴 추가.
	BOOL GetUserPattenData(CString strName, CStringArray &saUserPattenInfo);
	
	int SetAlignElement();
	void SetBackDateSearch(CString strDateCnt);
	
	void SetSelectGroup(int nGroup);
	BOOL GetGridPos_Element(CString strElement, int &nGroup, int &nRow);
//	void SetInsertPort(CString strPortName, CString strPortNumber);
	
	
	BOOL SetShow_LanguageEdit(CString strLanguageName = "");
	void SetShowMapLoader(BOOL bView, int nGroup = -1, int nRow = -1, CPoint pPosition = NULL);
	
	int GetFileToGroupCnt(CString strFullFilePath);
	int SetLoadFile(XMLTreeInfoFile*	pstXMLTreeInfoFile, BOOL bSetInit = TRUE, BOOL bOverlap = FALSE);
	BOOL SetLoadFile(CString strFullFilePath, int nOffSetElement);
	BOOL	SetSaveFile(CString strFolderName, CString strFileName);
	BOOL	SetSaveFile_Target(CString strFolderName, CString strFileName, LPCSTR strTargetXMLFile);
	
	CString SetMakePacketData(User_Finder &stUser_Finder, CStringArray &saLanguageName);
	BOOL SetTargetBit(CString strLogic, char *psResultData);
	int SetTargetBit_Portfolio(CPtrArray *_paData);
	void GetBackTestData(long &nDay, long &nTime);
	//CString GetLanguageData(CString strLanguageName);
	
	
	//# Match Val!
	CString GetMatchLogic();
	//int GetMatchLogic(CList <HeadInfoType, HeadInfoType&>	*plistFieldInfo, CString &strMatchLogic);
	void SetMatchLogic_Delete(CString strElement, BOOL bView = TRUE);
	void SetMatchLogic_UpDate();
	
	void SetTabChange(int nTabIndex);
	
	void SetSendFileName(BOOL bInit, XMLTreeInfoFile*	pstXMLTreeInfoFile = NULL);
	void SetSendFileName(CString strFileName, CString strFolderPath);
	void	SetSendFileName_Target(BOOL bInit, CString strFullFilePath);
	
	void SetFileSaveState(BOOL bSaveFlag);
	
	void SetInit();
	void SetWorkData(CStringArray *psaWorkDate);
	
	BOOL SetMSTCode(CStringArray *psaMSTCode);
//	void ChangeSkinColor(COLORREF clrSkin);
	void ChangeSkinColor(COLORREF clrSkin, ST_TABCOLOR stTabColor);
	void ChangeGridColor(ST_GRIDCOLOR stGridColor);
	BOOL PrepareRunRealSearch();
	void GetRegKwansimMasterGroupItem(CString strName, CStringArray& arrItems);
	BOOL GetBackDateAndTime(CString& strDate, CString& strTime);

	void OnTargetEdit();
	void SetChagneTargetEditIndicate(CString strIndicate);
	int	 GetConditionCount();

	// 20110315 박찬모 조건저장 후 저장된 조건 선택 >>
	CString m_strSaveFileName;
	BOOL	GetSaveFlag(){ return m_bSaveFlag; };
	CString GetSaveFileName(){ return m_strSaveFileName; };
	// 20110315 박찬모 조건저장 후 저장된 조건 선택 <<

	int	IsOrderCondition(CString strFullFilePath); // 20110623 박찬모 사용자저정조건 로드 시 각 조건(일반, 순차)에 맞게 좌측 트리 탭 변경 >>

	void OnBtnMatchValOk();
	void SetBtnExpandImage();

	void SetBtnImg(CfxImgButton *pBtn, int nTextLen, BOOL bBold = FALSE);
	void SetBtnImg(CfxImgButton *pBtn, CString strFileName);

private:
	BOOL m_bCommonExState;
	BOOL m_bOrderExState;
	BOOL m_bSelectCond;		//조건선택을 해서 해당 조건맵을 로드했는지 여부.
	
	void SetInitVal();
	void SetInit_LoadedFileInfo(XMLTreeInfoFile	&stLoadedFile);
	
	void OnSize();
	void OnSizeInit();
	void OnSizeTopToolBar(CRect reClient);
	void OnSizeLogicToolBar(CRect reClient);
	int GetBtnWidth(int nTextLen) { return (int)BTN_WIDTH_BASE + (int)BTN_WIDTH_STEP * nTextLen; }
	
	//# Add : 2006.09
	void OnSize_ConditionSet(CRect reClient);
	void OnSize_FunBar(CRect reClient);
	void OnSize_FunBarType2(CRect reClient);
	
	void SetTabView(int nTabIndex);
	
	int SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = NULL, UINT nType = MB_OK);
	void SetSendMainMsg(CString strMsg);
	int GetDataWithFilter(CMapStringToString &resultMap, CStringArray&target, LPCSTR sOption);

	void SetTabTextConditionItemexcavation();
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConditionMakeWnd)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
	
	// Implementation
public:	
	
	// Generated message map functions
	void EuBtnGridTip();
	void EuBtnLogicTip();
	void EuBtnInit(BOOL bOneClick = FALSE);
	void EuBtnFileSave();
	
protected:
	//{{AFX_MSG(CConditionMakeWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);	
	//}}AFX_MSG
	
	afx_msg void OnChangeTargetBase();
	afx_msg void OnCloseUp();
	afx_msg void OnBtnGridTip();
	
	afx_msg void OnBtnExpand();
	afx_msg void OnBtnBackTestChk();
	afx_msg void OnBtnDetail();
	afx_msg void OnBtnInit();
	afx_msg void OnBtnAlertChk();
	
	afx_msg void OnChangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeConditionTab(NMHDR* pNMHDR, LRESULT* pResult);
	
	afx_msg void OnBtnCommonExSize();
	afx_msg void OnBtnOrderExSize();
	
	afx_msg void OnBtnMatchVal();
	
	afx_msg void OnBtnBracket();		//괄호 Button
	afx_msg void OnBtnBracketDel();		//괄호삭제 Button
	afx_msg void OnBtnLogicTip();
	afx_msg void OnBtnPortDel();
	
	afx_msg void OnBtnFileSave();		//사용자조건 저장 Button
	
	afx_msg void OnBtnPortCheck();
	afx_msg void OnBtnMatchValCheck();
	
	afx_msg LRESULT OnChangeTest(WPARAM wParam, LPARAM lParam);
	
	//afx_msg void OnNotifyLogicEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotifyLogicEdit(WPARAM wParam, LPARAM lParam);
	//afx_msg void OnNotifyMapLoad(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotifyMapLoad(WPARAM wParam, LPARAM lParam);
	
	LRESULT OnNotifyGridEvent(WPARAM wParam, LPARAM lParam);

	afx_msg	LRESULT OnMGridSetBtnImg(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBtnDefaultValue();
	afx_msg void OnBtnNextCondition();
	afx_msg void OnBtnPreCondition();
	afx_msg void OnBtnSearchRng();
	afx_msg LRESULT OnLanguageReload(WPARAM wParam, LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()

//--> RMenu
	HWND m_hChildFrame;
	void SetCommMenu(POINT *point);
	HWND GetFrameWnd(CWnd* pWnd);

	void CConditionMakeWnd::FnRButtonUp(CPoint point);
	afx_msg long OnChangeChildFrameWindowHandle( WPARAM wParam, LPARAM lParam);
//<--
	virtual void DoDataExchange(CDataExchange* pDX);
public:
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);//20100827 이문수 >>
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONDITIONMAKEWND_H__7007DA10_926A_4EC1_84BF_7A0FA919B1E4__INCLUDED_)
