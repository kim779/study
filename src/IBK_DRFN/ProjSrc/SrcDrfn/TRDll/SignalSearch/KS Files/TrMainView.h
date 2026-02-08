#if !defined(AFX_TRMAINVIEW_H__ABD2E8FC_53A2_4A21_8BFE_49023C0D6025__INCLUDED_)
#define AFX_TRMAINVIEW_H__ABD2E8FC_53A2_4A21_8BFE_49023C0D6025__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrMainView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrMainView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "resource.h"
#include "..\..\..\Builder\FormMng\NodeView.h"

#include "../../TrDll/CtrlWrap/raprunjmcombo.h"
#include "..\..\TrDll\CtrlWrap\raprunchart.h"
#include "..\..\TrDll\CtrlWrap\raprunform.h"
#include "..\..\TrDll\CtrlWrap\raprunlabel.h"

#include "../../TRDLL/CtrlWrap/raprungrid.h" //20080522 김성조 [101] 
//
#include "../../../Controls/CommApi/Commexport.h"
#include "../../../Common/CommHeaderDef.h"
#include "../../../Controls/GDIMgr\ColorMap.h"

#include "Dr_Control/DrBtnCtrl.h"
#include "./AddControls/Grid_Comm.h"

#include "../../Contents/inc/PcTrMngHelper.h"	//Pctr Manager

class CTrMainView : public CFormView
{
protected:
	CTrMainView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTrMainView)

public:
	HWND			m_hFormMng;
	long			m_lBkColor;
	COLORREF		m_crBackColor;			      // 화면 백그라운드
	HWND			m_hChildFrame;
  
	CString   m_strCode;

	BOOL m_bIsFnUser;

	int         m_nInitFlag;
	CMapStringToOb	*m_MapControlName;	// 화면에 생성된 Control들을 관리하는 Map

	IDispatch *m_pFormDisp;
	IRunFormCtl m_lRunForm;

	IJMComboCtl		m_ctrlJMCombo;
	IJMComboCtl		m_ctrlJMCombo_Chart;
	IChartCtl		m_ctrlChart;
	ILabelCtl		m_lbScreenType;
	IGridCtl		m_gridSignal; //20080522 김성조 [101]
	int				m_nGridRow;	  //20080522 김성조 [101]

private:
	// 0:일반기존타입, 1:유진 현재가 지원
	int			m_nScreenType;
	CFont m_fontDefault, m_fontBold;
	CBrush m_brushStatic;

	CMapStringToPtr m_mapSignalData_Code;
	CMapStringToPtr m_mapSignalData_Type;

	BOOL m_bExp_Left;

	CWnd *m_pctrlCodeInput;
	CStatic       m_ctrlCodeName;
	CDrBtnCtrl m_btnInit_Code;

	CStatic       m_ctrlCodeName_Chart;
	CWnd *m_pctrlChart;

	CComboBox   m_comboSignalType;
	CDrBtnCtrl  m_btnExp_Left;

	/*
	CDrBtnCtrl  m_btnSell;
	CDrBtnCtrl  m_btnBuy;
	*/
//@유진삭제		CBitmapButton   m_btnSell;
//@유진삭제		CBitmapButton   m_btnBuy;

	CDrBtnCtrl  m_btnSignalSet;
	CDrBtnCtrl  m_btnSearch;

	CGrid_Comm  m_gridResult;

	// 신호매니저와 AdviseKey
	LONG	m_dwAdviseKey;
	
	CString	m_strTime;
	
	CButton		m_radioTotal;
	CButton		m_radioCode;
	CButton		m_chkMarketCondition;
	CButton		m_chkTechIndex;
	CButton		m_chkDealAgency;
	CButton		m_chkSys;
	CButton		m_chkNews;
	
	
 
private:
	void SetInit();

	void OnSize();
	void OnSize_Left(CRect reClient);
	void OnSize_Right(CRect reClient);
	int GetBtnWidth(int nTextLen) { return (int)BTN_WIDTH_BASE + (int)BTN_WIDTH_STEP * nTextLen; }

	void SetInit_MainWnd();
	BOOL SetAdvData_Alert_CurrentPrice(BOOL bReg);
	void SetAdvData_Alert(BOOL bReg);

	COLORREF GetPointToColor(int nPoint);
	BOOL GetConvert_RealToSignalData(int nRealType, LPSTR sRealData, SignalData &stSignalData);

	void SetAdd_SignalData(int nType, LPSTR sRealData, int nLen);
	void SetAdd_SignalData(SignalData stSignalData);

	void SetSearch();
	int SetFind_SignalData(BOOL bCode, CString strKey);

	void SetDelAll_SignalData();

public :
	int	GetScreenType() { return m_nScreenType;	}
		//nType 0:일반, 1:Dialog
	void SetOpenScreen(int nType, CString strMapNo, CString strSendData = "");
	void SetLinkCode(CString strCode, BOOL bTagData = FALSE);
	void SetLinkTime(CString strTime);
	void SetSendMainMsg(CString strMsg);

	void SetSearchData_Chart(CString strCode);
	void SetSearchData_Time(CString strTime);
	void SetSearchData_TimeSet();

	short GetCurSearchType();  //UI변경 되면서 기존 콤보박스 인덱스얻어오는 기능 대신함.

//20080522 김성조 <101
	void SetAdd_SignalNowPrice(STSIGNAL_NOWPRICE  stSignalNowPrice);
	void GetSignal_NowPrice(SIGNAL_CONDITION  stSIGNAL_CONDITION);	

	BOOL SendToEugeneMain(SignalData *pstSignalData);
	BOOL ConvertForEugeneSignalManager(SignalData *pstSignalData, EUGENE_SIGNALMSGINFO *pstEUGENE_SIGNALMSGINFO);


	HWND GetKSControlHwnd(LPCSTR strCtrlName) { return (HWND)m_lRunForm.GetControlHwnd(strCtrlName);}
//20080522 김성조 101>

	// 종목명 라벨처리
	HWND	m_hKSJongName;
	CRect	m_rectJongName;
	
	int		m_nState;	//0:All, 1:개별종목
	void	DoBtnInit_Code(BOOL bClear=TRUE);
	void	SetControlDispatch();
	void	DoKSControlMove(LPCSTR szCtrlName, CWnd* pWnd);
// Form Data
public:
	//{{AFX_DATA(CTrMainView)
	enum { IDD = IDD_STD_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrMainView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTrMainView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CTrMainView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
//	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg long OnChangeChildFrameWindowHandle( WPARAM wParam, LPARAM lParam);

	afx_msg void OnBtnInit_Code();
	afx_msg void OnBtnExp_Left();
//@유진삭제		afx_msg void OnBtnSell();
	afx_msg void OnChangeSignalType();

	afx_msg void OnBtnSignalSet();
	afx_msg void OnBtnSearch();

	afx_msg void OnMouseHover();

	afx_msg long OnGetData(WPARAM wParam, LPARAM lParam);
	afx_msg long OnGetBrod(WPARAM wParam, LPARAM lParam);

	afx_msg long OnNotify_PortPolio(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRadioBtnClk(UINT nID);
	afx_msg void OnChkBtnClk(UINT nID);

	DECLARE_MESSAGE_MAP()
//@유진삭제	afx_msg void OnBtnSell();

	afx_msg void OnCodeEnter();
	afx_msg void OnCodeEnter_Chart();
	afx_msg void OnFormInitAfter();
	afx_msg void OnFormGetPageSave(LPCTSTR strFileName);	//메인 저장화면
	afx_msg void OnFormSetPageSave(LPCTSTR strFileName);	//메인 저장화면
	afx_msg void OnColorChange();	//메인의 스킨변경시 불린다.
	afx_msg void OnReceiveDataAfter(LPCTSTR strTrCode, VARIANT FAR* strVarOutInfo, VARIANT FAR* nVarOutInfo, VARIANT FAR* nVarRetValue, VARIANT FAR* nVarRetOption, long nDataCount);
	afx_msg	void OnEuBtnClk(UINT nID);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	DECLARE_EVENTSINK_MAP()

	LONG OnRmsgOutside(WPARAM wp, LPARAM lp);	//외부에서 보내오는 메시지..
//  afx_msg void OnBtnBuy();

	// 사이징처리관련 추가
	CWnd* GetFrameWnd(CWnd* pWnd);
	void DoDllSizeEx(HWND _hChildFrame=NULL);
	
	CPCTrMngHelper m_pPctrHelper;

	void SetCommMenu(POINT *point);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TR1111VIEW_H__ABD2E8FC_53A2_4A21_8BFE_49023C0D6025__INCLUDED_)
