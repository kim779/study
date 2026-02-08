#if !defined(AFX_MAINFRMWND_H__5D7CDB34_4C3A_4B0B_8A73_98C04C8BA19F__INCLUDED_)
#define AFX_MAINFRMWND_H__5D7CDB34_4C3A_4B0B_8A73_98C04C8BA19F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainFrmWnd.h : header file
//
#include <afxmt.h>

#include "../../DrCommon/Dr_Control/DrTabCtrl.h"
#include "../../DrCommon/Dr_Control/DrLabel.h"

#include "../../../SrcSite/control/fx_misc/misctype.h"
#include "../../../SrcSite/control/fx_misc/fxImgButton.h"
#include "../../DrCommon/Dr_Control/DrTabCtrl.h"

#include "./AddControl/Grid_Comm.h"
#include "./AddControl/HelpEditCtrl.h"

#include "../../../SrcSite/include/axisfire.h"

typedef struct
{
	CStringArray strKey;
	CStringArray strGroupName;
} ST_GwanSimGroup;
/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd frame

class CMainFrmWnd : public CFrameWnd
{
public:
	COLORREF		m_clrMainSkin, m_clrTab;
	CMainFrmWnd();           // protected constructor used by dynamic creation
	virtual ~CMainFrmWnd();

	int		m_nIndustry;
	BOOL	m_bIsFnUser;
	
	CString m_bIsHTS;
	CString m_strUserID;
	
	CFont m_fontDefault, m_fontBold;
	CBrush  m_brushStatic;
	
	BOOL m_bInit;
	CString m_strAlertKey;
	CCriticalSection	m_sect;

	CString m_sPBID;		//한투 PBID
	CString m_sDelTRID;	//한투 해제 TRrid

	
	int			m_nFixCol;
	CString		m_strFixFieldVersion;	
	CList<HeadFixInfo, HeadFixInfo&>   m_listFixHeadUser;
	CList<HeadInfoType, HeadInfoType&> m_listFixHeadSys;
	CList<HeadInfoType, HeadInfoType&> m_listField;

	CList<HeadInfoType, HeadInfoType&> m_listSelectedHead;

public:
	int Modual_SendTr(CString TrNo, int nTrLen, BYTE *TrData);
	BOOL ReceiveData(LPCSTR szTR, LPVOID aTRData, long dwTRDateLen);

	void Modual_SendReal(BOOL bReg, CString strKey = "*");
	HRESULT ReceiveData_RealAlert(SIGNAL_CONDITION stSIGNAL_CONDITION);
	
	CBrush	m_brushBK;
	CBrush	m_brushMax, m_brushUp, m_brushSte, m_brushDown, m_brushMin, m_brushCnt, m_brushTab;
	
	
	HINSTANCE		m_hConditionMng;
	typedef int(*_DLL_Open)(CStringArray *psaMSTCode);
	typedef void(*_DLL_Close)(void);
	
	typedef BOOL(*_DLL_GetConditionData) (int nType, BOOL bIndustry, int nGroupLockType, CStringArray &saGroupName, CStringArray &saCondition);
	
	typedef int (*_DLL_GetFind_Packet) (CString strPath, int nTrIndex, BOOL bGetIndustry, BOOL bDefultResultField,
												BOOL bOrdering_Apply, LPSTR &lpPacket, int &nOrderCnt);
	
	typedef BOOL (*_DLL_GetFilter) (CString strFullPath, long* plFilterInfo);
	typedef void (*_DLL_GetFind_Packet_ByteOrdering) (User_Finder *pstUserFinder);
	typedef int  (*_DLL_GetFilterToPacket) (CString strFullPath, long *plPacket);
	typedef void (*_DLL_GetFormatText)(CString strFullPath, CString& strText);		// 2009.10.27 -hidden-
	typedef void (*_DLL_SetUserFolderPath)(CString strUserFolderPath);
	
	_DLL_Open   Lib_Open;
	_DLL_Close  Lib_Close;
	_DLL_GetConditionData	Lib_GetConditionData;
	_DLL_GetFind_Packet		Lib_GetFind_Packet;
	_DLL_GetFind_Packet_ByteOrdering	Lib_GetFind_Packet_ByteOrdering;
	_DLL_GetFilter			Lib_GetFilter;
	_DLL_GetFilterToPacket	Lib_GetFilterToPacket;
	_DLL_GetFormatText		Lib_GetFormatText;
	_DLL_SetUserFolderPath	Lib_SetUserFolderPath;

	typedef CWnd*(*_DLL_SetLibOpen)(CWnd* pwndMain, HINSTANCE hInstance, CWnd* pWndModuleWnd);
	typedef void (*_DLL_SetLibClose)(CWnd* pWndTarget);
	typedef int	 (*_DLL_SetSiganl)(BOOL bAdvise, CWnd* pwndMain, CString strType, CString sPBID /* = ""*/, CString strKey /*= "*"*/);

	HINSTANCE			m_hSignalMng;
	CWnd*				m_pWndDummy;
	_DLL_SetLibOpen		SignalLib_Open;
	_DLL_SetLibClose	SignalLib_Close; 
	_DLL_SetSiganl		SignalLib_SetSignal;
	
	CMapStringToPtr		m_mapCondition;
	FilterInfo m_stFilterInfo;

	CString GetSelectGridItemCode(); // 20110328 박찬모 R-Mouse Menu를 통한 종목 연동 기능 제공

	void OnBtnFilterSetOk();
	int ParseGroup(char* pReceiveBuff, CStringArray& arrKey, CStringArray& arrName);
	void Request_PortGroup();
private:
	ST_GwanSimGroup m_GwanSimGroup;
	CStringArray m_saGroupName;
	CString      m_strGroupName;
	// 20100203 박찬모 >>
	BOOL		 	m_bBeforeUserTap;	// 사용자검색->다른 탭 선택을 구분
	BOOL		 	m_bSaveShowFlag;	// 확장탭 전 상태 저장
	CComboBox	 	m_ctrlUserList;		// 사용자 검색 탭 콤보박스
	CRect		 	m_rectWnd;			// 빠른업종 검색 확장탭 사용 시 필요
	ConditionInfo   m_stRealConditionInfo;	// 탭 삭제로 인한 temp.xml 저장
	int GetCount_FixHead(BOOL &bHaveCodeFiled, CUIntArray &iaFixHeadId);
	// 20100203 박찬모 <<
	
	BOOL m_bRollSearch;
	
	BOOL m_bHelpShowFlag;
	BOOL m_bLeftShowFlag;
	
	int m_nTrIndex_List;
	CList <ConditionInfo, ConditionInfo&>		m_listConditionInfo;
	
	int m_nTrIndex_Result;
	ConditionInfo	m_stConditionInfo;

	BOOL		m_bRegReal;	// 감시 세팅여부.
	BOOL		m_bOpenFromConditionSearch;		// 2009.11.02 -hidden-	조건검색화면에서 오픈 할때만 감시.

	BOOL		m_bFromCall;

public:
	CDrTabCtrl*		m_ptabGroup;
	CRect			m_rtTab;
	CFieldEditDlg*	m_pdlgFieldEdit;

	ST_TABCOLOR		m_stTabColor;
	COLORREF		m_clrBkgnd;

	CButton			m_btnCodeChk;
	CStatic			m_ctrlCodeInput;
	CStatic			m_ctrlCodeName;
	CfxImgButton	m_btnListSearch;

	CGrid_Comm		m_gridList;		//@유진에서는 사용안함.
	CTreeCtrl		m_treeList;		//@유진에서 사용
	CImageList		m_treeImageList;

	CfxImgButton	m_btnHelpShow;

	CHelpEditCtrl	m_editHelp;
	CHelpEditCtrl	m_ctrlConditionText;	// 2009.10.27 -hidden-

	CfxImgButton	m_btnExp;
	CStatic			m_ctrlTitle;
	
	CButton         m_btnFilterChk;
	CfxImgButton	m_btnFilterSet;

	CButton         m_btnKospiChk;
	CButton         m_btnKosdakChk;
	CButton         m_btnTemaChk;
	CButton			m_btnDelOutItem;
	
	CfxImgButton	m_btnSearch;
	
	CComboBox		m_comboSearchTime;

	CfxImgButton	m_btnRollSearch;
	CfxImgButton	m_btnRollSearchStop;
	CfxImgButton	m_btnRegReal;
	CfxImgButton	m_btnSend;
	CfxImgButton	m_btnItemEdit;
	
	CToolTipCtrl	m_ToolTipCtrl;
	
	CStatic			m_ctrlJongCnt;
	CStatic			m_ctrlMax;
	CStatic			m_ctrlUp;
	CStatic			m_ctrlSteady;
	CStatic			m_ctrlDown;
	CStatic			m_ctrlMin;
	CStatic			m_ctrlUpRate;
	CStatic			m_ctrlAvg;
	
	CStatic			m_ctrlJongCnt1;
	CStatic			m_ctrlMax1;
	CStatic			m_ctrlUp1;
	CStatic			m_ctrlSteady1;
	CStatic			m_ctrlDown1;
	CStatic			m_ctrlMin1;
	CStatic			m_ctrlUpRate1;
	CStatic			m_ctrlAvg1;

	CDrLabel		m_ctrlAutoStr;	// 자동검색중.
	
	CGrid_Comm		m_gridResult;
	
	CStatic			m_ctrlMsg;	
	CString			m_szSendTRMode;

	CMapStringToString	m_mapResultFieldIndex;

	CString			m_szScreenKey;

	CWnd*			m_pModuleMainWnd;

	BOOL			m_bRealRegFlag;
public:
	void SetInit(CWnd* pWnd, int nIndustry);
	void SetInit_Industry(int nSetIndustry);
	
	int SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = PROGRAM_TITLE, UINT nType = MB_OK);
	
	void SetChangeTab(int nCurSel);
	//20080428 김성조 >>
	void SetChangeGroupTree(HTREEITEM hItem);
	void SetChangeSubTree(HTREEITEM hItem);
	void SetSendDataTree(HTREEITEM hItem);

	void ChangeSkinColor(COLORREF clrBkgnd, ST_TABCOLOR stTabColor);  //스킨 색상
	void ChangeGridColor(ST_GRIDCOLOR stGridColor);

	//유진 컨트롤 관련
	void EuBtnFilterSet();
	void EuBtnSearch();
	void EuBtnSend();
	void EuBtnReal();
	void EuBtnDescription();
	void SetFieldEditApply();

	//20100330 SJ_KIM 항목편집/////////////////////////
	void SetGrid_Head(BOOL bView = FALSE);
	void SetLoad_FieldInfo();
	BOOL SetSaveFixUserFieldData(CList<HeadFixInfo, HeadFixInfo&> *plistFixUser, CString strVersion);
	BOOL GetLoadFixUserFieldData(MSXML::IXMLDOMNodePtr pNIndex,	CList<HeadFixInfo, HeadFixInfo&> &listFixList);
	BOOL GetLoadXml(CString strFileRoot, MSXML::IXMLDOMDocumentPtr &pXmlDom);
	BOOL GetLoadFixSysFieldData(MSXML::IXMLDOMNodePtr pNIndex, CList<HeadInfoType, HeadInfoType&> &listFixList);
	CString GetVersion_FieldFile(MSXML::IXMLDOMNodePtr pNIndex);
	void SetUpdateUserFieldData(CList<HeadInfoType, HeadInfoType&> *plistFixSys, CList<HeadFixInfo, HeadFixInfo&> *plistFixUser);
	BOOL GetLoadFixField_Def(CStringArray &saFieldId);
	void GetTabClr(COLORREF* clrBkgnd, ST_TABCOLOR* stTabColor);
	//////////////////////////////////////////////////
	BOOL GetRealFlag();
	LPCSTR GetScreenKey() { return m_szScreenKey;}

	void		SetBtnImg(CfxImgButton *pBtn, CString strFileName);
	void		SetBtnImg(CfxImgButton *pBtn, int nTextLen, BOOL bBold = FALSE);

	CFont*		GetFont(BOOL bBold = FALSE);
	COLORREF	GetColor(int nColor);
	CString		Variant(int nComm, CString strData = "");

	COLORREF	GetAxColor(int nColor);
	HBRUSH		GetAxBrush(COLORREF clr);

	BOOL		GetStockMasterData(CStringArray &arrCode, CStringArray &arrName, UINT Jgub, UINT Upgub);
	BOOL		GetUpMasterData(CStringArray *&psaCode, CStringArray *&psaName, int jgub);
	CString		GetMarketNameToCode(CString strMaketCode);
	CPen*		GetAxPen(COLORREF clr, int nWidth, int nStyle);
	void		GetData_PortGroup(CStringArray& saKey, CStringArray& saGroupName);

	int			SetResultSearchTemp();
	COLORREF	GetMainBackColor();
private:
	void SetSearch_MSTCode();
	
	void SetToolTip();
	
	void OnSize();
	void OnSize_SubMain(CRect reClient);
	void OnSize_Left(CRect reClient);
	void OnSize_Right(CRect reClient);
	
	void OnSize_Result(CRect reClient);
	void OnSize_ResultBar(CRect reClient);
	
	int GetBtnWidth(int nTextLen) { return (int)BTN_WIDTH_BASE + (int)BTN_WIDTH_STEP * nTextLen; }
	
	void GetData_UserCondition();
	BOOL SetLoad_ConditionMngLib();
	BOOL SetLoad_SignalMngLib();

	void GetData_RealSearchCondition();
	
	void SetAddGrid_Condition(CList <ConditionInfo, ConditionInfo&> *plistConditionInfo);
	void SetClear_Condition(CMapStringToPtr *pmapCondition);
	
	void SetTrSend_AlertReal(CString strMode);
	
	void SetTrSend_UserAlertReal(CString strMode, BOOL bDestroy = FALSE);
	BOOL SetReceiveData_UserAlertReal(LPTSTR pData, DWORD dwDataLen);
	
	void SetTrSend_List(CString strGroupName);
	void SetTrSend_Result(ConditionInfo *pstConditionInfo, BOOL bAlert = FALSE);
	
	int GetMakePacket_Condition(CList <ConditionInfo, ConditionInfo&> *plistConditionInfo);
	BOOL GetMakePacket_Condition(ConditionInfo	stConditionInfo, User_Finder *pstUser_Finder);
	
	BOOL SetReceiveData_Result(LPTSTR pData, DWORD dwDataLen);
	BOOL GetCheck_Filter(FilterInfo *pstFilterInfo, TR_RESULT_LIST stTR_RESULT_LIST);
	BOOL GetCheck_Filter(FilterInfo *pstFilterInfo, SIGNAL_COMMON stCommon);
	
	void SetTrSend_TemaList();
	BOOL SetReceiveData_TemaList(LPTSTR pData, DWORD dwDataLen);
	
	void SetTrSend_TemaResult(ConditionInfo *pstConditionInfo);
	
	void SetResult_Info(TR_RESULT_HEADER stTR_RESULT_HEADER);
	void SetResult_Clear();
	
	BOOL GetFilterData(FilterInfo &stFilterInfo);
	BOOL GetFilterPacket(long *plPacket);
	void SetGroupToPacket(CString strGroupName);
	
	void SetPopupMenuResultCtrl(CPoint ptMenu);
	void SetResultSave();
	void SetSharedDataToSend(ShardData *pstShardData);
	void SetSharedDataToCode(CString strCode);
	
	BOOL SetTempGroupReg(CStringArray *psaCode, CUIntArray *iaMarkType);
	
	void SetEnable(BOOL bEnable, BOOL bAlert = FALSE);
	void SetSendMainMsg(CString strMsg);
	BOOL SendToEugeneMain(SIGNAL_CONDITION *pstSIGNAL_CONDITION);
	BOOL ConvertForEugeneSignalManager(SIGNAL_CONDITION *pstSIGNAL_CONDITION, EUGENE_SIGNALMSGINFO* stEUGENE_SIGNALMSGINFO);

	CString GetFilePath(CString strLoadType);
	int GetFillString(CString& _szGetStr, int nStart, int nEnd, int nOneWordSize);
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrmWnd)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMainFrmWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	//}}AFX_MSG
	
	//20080428 김성조 >>
	afx_msg void OnChangeTreeItem(NMHDR* pNMHDR, LRESULT* pResult);
	//20080428 김성조 <<
	afx_msg void OnChangeGroupTab(NMHDR* pNMHDR, LRESULT* pResult);
	
	afx_msg void OnBtnCodeChk();
	afx_msg void OnBtnListSearch();
	afx_msg void OnBtnShowHelp();
	afx_msg void OnBtnExp();
	afx_msg void OnBtnSend();
	afx_msg void OnBtnFilterChk();
	afx_msg void OnBtnFilterSet();
	afx_msg void OnBtnSearch();
	afx_msg void OnBtnRollSearch();
	afx_msg void OnBtnRegReal();
	afx_msg void OnBtnDelOutItem();

	afx_msg void OnBtnFieldEdit();
	
	//afx_msg void OnNotify_Grid(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotify_Grid(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMenuResultCtrl(UINT nIndex); // 20100204 박찬모 >>

	afx_msg void OnCbnSelChangeUserList();
	
	//# 삼성증권
	afx_msg long OnGetData(WPARAM wParam, LPARAM lParam);
	afx_msg long OnGetBroad(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnNotifySignal(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnGetModuleMainWnd(WPARAM wParam, LPARAM lParam);/*CWnd* tour2k*/
	
	DECLARE_MESSAGE_MAP()

	//--> 통신
	CCriticalSection	m_CommCritical;

	BOOL m_b0622Ing;
	LONG m_dwAdviseKey;

	void InMakeFid_P0622(LPCSTR szTrNo, int nTrLen, BYTE *pTrData, char* szSaveBuf, int &nPos);
	int InMakeTr_P0622(BYTE *pTrData, BYTE* pTrOutData);
	void InMakeFid_P0825(LPCSTR szTrNo, int nTrLen, BYTE *pTrData, char* szSaveBuf, int &nPos);
	
	// p0627
	void InMakeFid_Mstcode(LPCSTR szTrNo, int nTrLen, BYTE *pTrData, char* szSaveBuf, int &nPos);
		// 저장된 MSTCODE있으면 가져온다.
	BOOL CheckSavedMstcode();

	void SetCommMenu(POINT *point);
	long OnChangeChildFrameWindowHandle( WPARAM wParam, LPARAM lParam);
	HWND GetFrameWnd(CWnd* pWnd);
	HWND m_hChildFrame;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void _DoGridWidthSet(int nOption);


	int			m_nPreReceiveLen;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRMWND_H__5D7CDB34_4C3A_4B0B_8A73_98C04C8BA19F__INCLUDED_)
