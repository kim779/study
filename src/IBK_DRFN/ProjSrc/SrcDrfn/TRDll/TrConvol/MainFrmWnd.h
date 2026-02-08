#if !defined(AFX_MAINFRMWND_H__EA111D61_6A7C_4336_846B_0145136A25A3__INCLUDED_)
#define AFX_MAINFRMWND_H__EA111D61_6A7C_4336_846B_0145136A25A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainFrmWnd.h : header file
//

#include "resource.h"

#include <afxmt.h> // CriticalSection

#include "AddControl/ConVolGrid.h"

#include "../../../SrcSite/control/fx_misc/fxImgButton.h"

#include "FieldEditDlg.h"
#include "TargetSet.h"

#include "TimeEstimate.h"

class CCodeMstInfo;
class CCommApiHelper;
class CFilterSet;
/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd frame

class CMainFrmWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CMainFrmWnd)
public:
	CMainFrmWnd();           // protected constructor used by dynamic creation
	virtual ~CMainFrmWnd();

private:
	BOOL				m_bRight;

	CConVolGrid			m_ctrlGrid;
	CCommApiHelper*		m_pCommApiHelper;
	int					m_nConnectID;

	int					m_nOldShow;
	BOOL				m_bSiseOnly;
	
	CfxImgButton		m_btnErase;
	CfxImgButton		m_btnFieldSet;
	CfxImgButton		m_btnConditionSet;
	CfxImgButton		m_btnWatchSet;

	BOOL				m_bTimerChk;
	CTimeEstimate		m_TimeEstimate;
	CList <StConVolClient, StConVolClient&> m_listTempStConVol;
// Attributes
public:
	CFont				m_fontDefault;
	CFont				m_fontBold;
	CFont				m_fontNotice1;
	CFont				m_fontNotice2;
	ST_REALITEM			m_realItem;
	CCriticalSection	m_sect;
	BOOL				m_bCheckSync;
	StFieldSet			m_stMainFieldSet;
	StConVolSet			m_stConVolSet;
	ST_SEARCHTARGET		m_stSEARCHTARGET;
	COLORREF			m_clrMainSkin;
	
	LONG				m_dwAdviseKey;

	CString				m_strScreenNo;	//화면번호
	CString				m_strSaveKey;	// 저장용키
	BOOL				m_bFiltering;  //필터링 사용여부(현재가 화면일때는 사용안함)

	CList <StConVolClient, StConVolClient&> m_listStConVol;
// Operations
public:
	void SetInit(BOOL bSiseOnly);
	BOOL FilterSet();
	void FieldSet();
	void TargetSet();
	void TargetItemSet();

	// 사용자의 입력/선택 정보를 저장한다.
	void DoSaveUserInfo(LPCSTR _szSaveFile=NULL);

	LPSTR StringDivideCodeAndName(CString strData, CString &strName);
	CString GetPath(CString strdiv);

	void Modual_SendReal(BOOL bReg, CString strKey);
	
	BOOL PacketConvertToClientMap( StConVolClient &stConVolClient, StTrConVol stTrConVol);
	
	BOOL GetConVol_FieldInfo();
	BOOL GetConVol_FilterInfo();
	BOOL GetConVol_TarGetInfo();

	BOOL RunConVol_Filtering(StTrConVol	stTrConVol);
	void ChangeSkinColor(COLORREF clrBkgnd, ST_TABCOLOR stTabColor);
	void ChangeGridColor(ST_GRIDCOLOR stGridColor);

// 	CCodeCtrl*		m_pctrlSelectCodeInput;
// 	HWND			m_hSelectCodeHwnd;
// 	CCodeCtrl*		m_pctrlExceptCodeInput;
// 	HWND			m_hExceptCodeHwnd;
// 	void SetCodeInfo(CCodeCtrl *pSelectJCodeDisp, HWND hSelectControl, 
// 		CCodeCtrl *pExceptJCodeDisp, HWND hExceptControl);

	CfxImgButton		m_pBtnAdd;
	CfxImgButton		m_pBtnRemove;
	CfxImgButton		m_pBtnRemoveAll;
	HWND			m_hBtnAdd;
	HWND			m_hBtnRemove;
	HWND			m_hBtnRemoveAll;
// 	void SetBtnInfo(CfxImgButton* pBtnAdd, HWND hBtnAdd,
// 		CfxImgButton* pBtnRemove, HWND hbtnRemove,
// 		CfxImgButton* pBtnRemoveAll, HWND hbtnRemoveAll);

	CFilterSet*		m_pDlgFilterSet;

	CStringArray	m_arExceptItemCode;
	CStringArray	m_arExceptItemName;

	void OnBtnAdd(CString szItemCode, CString szItemName);
	void OnBtnRemove();
	void OnBtnRemoveAll();

	CTargetSet*	m_pTargetSet;
	void DoCodeInput(int nType)
	{
		if(m_pTargetSet)
			m_pTargetSet->DoCodeInput(nType);
	}

	void PlaySound();
	void SetUserRight(BOOL bRight);
	void ClearAll();
	void HeaderChange(StFieldSet *pstFieldSet, BOOL bFirst = FALSE);
	void ChangeConditionSet();
	void Change_GridInsertType(int nType);
	void SetCopy_ConVolData(StConVolClient stConVolClient);

	///////////////////////////////////////////////////////////
	// 항목편집
	int					m_nFixCol;
	CString			    m_strFixFieldVersion;	
	CList<HeadFixInfo, HeadFixInfo&>   m_listFixHeadUser;
	CList<HeadInfoType, HeadInfoType&> m_listFixHeadSys;
	CList<HeadInfoType, HeadInfoType&> m_listField;
	CList<HeadInfoType, HeadInfoType&> m_listSelectedHead;
	COLORREF	m_clrBkgnd;
	ST_TABCOLOR	m_stTabColor;

	CFieldEditDlg *m_pdlgFieldEdit;

	void	OnBtnFieldEdit();
	BOOL	SetSaveFixUserFieldData(CList<HeadFixInfo, HeadFixInfo&> *plistFixUser, CString strVersion);
	BOOL	GetLoadFixUserFieldData(MSXML::IXMLDOMNodePtr pNIndex, CList<HeadFixInfo, HeadFixInfo&> &listFixList);
	void	SetLoad_FieldInfo();
	BOOL	GetLoadXml(CString strFileRoot, MSXML::IXMLDOMDocumentPtr &pXmlDom);
	void	SetGrid_Head(BOOL bView = FALSE);
	BOOL	GetLoadFixSysFieldData(MSXML::IXMLDOMNodePtr pNIndex, CList<HeadInfoType, HeadInfoType&> &listFixList);
	CString	GetVersion_FieldFile(MSXML::IXMLDOMNodePtr pNIndex);
	void	SetUpdateUserFieldData(CList<HeadInfoType, HeadInfoType&> *plistFixSys,CList<HeadFixInfo, HeadFixInfo&> *plistFixUser);
	BOOL	GetLoadFixField_Def(CStringArray &saFieldId);
	int		GetCount_FixHead(BOOL &bHaveCodeFiled, CUIntArray &iaFixHeadId);
	int		SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType);
	void	GetTabClr(COLORREF* clrBkgnd, ST_TABCOLOR* stTabColor);
	CString GetFilePath(CString strLoadType);
	///////////////////////////////////////////////////////////
	void OnFilterSetMessage(int nFlag);
	
	HRESULT ReceiveData( LPCSTR szTR, LPVOID aTRData, long dwTRDateLen);

	void		SetBtnImg(CfxImgButton *pBtn, CString strFileName);
	void		SetBtnImg(CfxImgButton *pBtn, int nTextLen, BOOL bBold = FALSE);
	
	CFont*		GetFont(BOOL bBold = FALSE);
	COLORREF	GetColor(int nColor);
	CString		Variant(int nComm, CString strData = "");
	
	COLORREF	GetAxColor(int nColor);
	HBRUSH		GetAxBrush(COLORREF clr);
	CWnd*		GetModuleMain();
	void		SetSharedDataToCode(CString strCode);

	// 버튼관련
	BOOL	m_bWatching;
	void	OnClickbtnErase();
	void	OnClickbtnFieldSet();
	void	OnClickbntConditionSet();
	void	OnClickbtnWatchSet();
	void	OnFieldSetBtnOK(StFieldSet *pstFieldSet);	// 필드설정 OK버튼 클릭시 처리함수

	void	OnRButtonClick(CPoint *point, CString strCode = "");

	BOOL	GetStockMasterData(CStringArray &arrCode, CStringArray &arrName, UINT Jgub, UINT Upgub);

public:
	//{{AFX_DATA(CMainFrmWnd)
	enum { IDD = IDD_STD_DIALOG };	
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrmWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam) ;
	//}}AFX_VIRTUAL

// Implementation
protected:


	// Generated message map functions
	//{{AFX_MSG(CMainFrmWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg long OnGetBroad(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotify_Grid(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTest(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRMWND_H__EA111D61_6A7C_4336_846B_0145136A25A3__INCLUDED_)
