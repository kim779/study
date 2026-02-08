#if !defined(AFX_TRCONVOLVIEW_H__ABD2E8FC_53A2_4A21_8BFE_49023C0D6025__INCLUDED_)
#define AFX_TRCONVOLVIEW_H__ABD2E8FC_53A2_4A21_8BFE_49023C0D6025__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrConVolView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrConVolView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "resource.h"
#include "MainFrmWnd.h"

class CTrConVolView : public CScrFormView
{

protected:
	CTrConVolView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTrConVolView)

// Form Data
public:
	//{{AFX_DATA(CTrConVolView)
	enum { IDD = IDD_MCHARTVIEW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CMainFrmWnd*	m_pwndMainFrm;
	int				m_nCurPriceRowCount;
	int				m_nScreenType;			
	int				m_nOld_InsertType;
	
	HWND			m_hFormMng;
	
	int				m_nInitFlag;
	
	long			m_lBkColor;
	COLORREF		m_crBackColor;			// 화면 백그라운드
	
	CMapStringToOb	*m_MapControlName;		// 화면에 생성된 Control들을 관리하는 Map
	
	CString			m_szScreenDll;
	
	CWnd* GetFrameWnd(CWnd*	pWnd);
	
	BOOL			m_bWatching;

	CString			m_strScreenNo, m_strSaveKey;
	CString			m_strMapFileName;
	IDispatch*		m_ScrFormDisp;
	
	
	HWND			m_hChildFrame;
	CWnd*			m_pFormWnd;
	
	int				m_nRowTotalSize;
	int				m_nOldShow;
	BOOL			m_bFirst;

	CStringArray	m_arItemData;

	CCodeCtrl	m_ctrlSelectUJCombo;
	CCodeCtrl	m_ctrlExceptJMCombo;

	CLinkDataManager	m_LinkDataManager;

// Operations
public:
	void OnCreate_MainWnd(BOOL bIndustry);
	void SetInit_MainWnd();
	
	void SetSendMainMsg(CString strMsg);
	void HeaderChange(StFieldSet* pstFieldSet);
	void SetAddRow(StConVolClient	stConVolClient);
	void SetAddRow_CurrentPrice(StConVolClient	stConVolClient);
	void OnSize(int cx, int cy);
	void GetSignInfo(int& nSign, CString& strSign);

	void Modual_SendReal(BOOL bFlag, CString strKey);
	void FrameResize(int nWidth, int hHeight = 0);

	void DoDllCreate();
	void SetFrameMsgText(CString strMsg);
	void SetCodeDragAndDrop(CString strCode);

	virtual	void OnInitialControl();
	void OnChangeColorTbl(int nTableIndex);
	void DoItemCodeMove(LPCSTR szCtrlName, CWnd* pWnd); 

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrConVolView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTrConVolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 폼화면의 컨트롤과 연결된 변수
	//{{AFX_CONTROL(CConditionSearch_OneView)
	CfxImgButton	m_ctrlEraseBtn;
	CfxImgButton	m_ctrlFieldSetBtn;
	CfxImgButton	m_ctrlConditionSetBtn;
	CfxImgButton	m_ctrlTargetItemBtn;
	CfxImgButton	m_btnWatchSet;
	
	CfxImgButton		m_btnAdd;
	CfxImgButton		m_btnRemove;
	CfxImgButton		m_btnRemoveAll;

	//}}AFX_CONTROL

	void SetCtrlVisible(BOOL bVisible);
	
	// 폼화면의 이벤트 함수
	//{{AFX_EVENTSINK(CConditionSearch_OneView)
	void	OnFormBeforeInit();
	void	OnFormAfterInit();
	void	OnReceiveTranData(LPCTSTR szTranID);
	void	OnReceiveTranCompleteData(LPCTSTR szTranID);
	void	OnReceiveRealData(LPCTSTR szTranID);
	void	OnReceiveRealCompleteData(LPCTSTR szTranID);
	void	OnClickbtnErase();
	void	OnClickbtnFieldSet();
	void	OnClickbntConditionSet();
	void	OnClickbtnTargetItem();
	void	OnClickbtnWatchSet();
	
	void	EditFullSelectMarketCode();
	void	EditEnterSelectMarketCode();
	void	EditFullExceptItemCode();
	void	EditEnterExceptItemCode();

	void	OnClickBtnAdd();
	void	OnClickBtnRemove();
	void	OnClickBtnRemoveAll();

	LRESULT OnRmsgKSMoveCtrl(WPARAM wParam, LPARAM lParam);
	//}}AFX_EVENTSINK

	// Generated message map functions
	//{{AFX_MSG(CTrConVolView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnBtnPopup();
	afx_msg LRESULT OnLButtonClickedRowCol(short nRow, short nCol, short nSubPos, short nKeyIndex, LPCTSTR strItem, LPCTSTR strData);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_EVENTSINK_MAP()
	afx_msg LRESULT OnGetSkinColor(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
		
	afx_msg void OnFormGetPageSave(LPCTSTR strFileName);	//메인 저장화면
	afx_msg void OnFormSetPageSave(LPCTSTR strFileName);	//메인 저장화면
	afx_msg void OnColorChange();	//메인의 스킨변경시 불린다.
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRCONVOLVIEW_H__ABD2E8FC_53A2_4A21_8BFE_49023C0D6025__INCLUDED_)
