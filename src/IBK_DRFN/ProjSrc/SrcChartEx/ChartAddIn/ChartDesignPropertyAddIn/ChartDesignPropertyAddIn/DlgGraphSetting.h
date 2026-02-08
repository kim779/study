#if !defined(AFX_DLGGRAPHSETTING_H__C784ACD7_9191_46F3_939B_1BA9EFCB9CE0__INCLUDED_)
#define AFX_DLGGRAPHSETTING_H__C784ACD7_9191_46F3_939B_1BA9EFCB9CE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGraphSetting.h : header file
//
#include <afxtempl.h>
#include "CheckComboBox.h"
#include "DlgToolTip.h"
#include "NoSystemMenuEdit.h"
#include "NoSystemMenuCombo.h"
#include "DialogExpander.h"
#include "GraphColorCollection.h"	// Added by ClassView
#include "LineColor.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSetting dialog
class CChartLink;
class CLineColor;
class CDlgGraphSetting : public CDialog
{
// Construction
public:
	CDlgGraphSetting(CChartLink *pItem, CLineColor* LineColor, IGraphInfoManager *p_pIGraphInfoManager, HWND p_hOcxWnd, CWnd* pParent = NULL);   // standard constructor
	~CDlgGraphSetting();

public:
	CString GetIndicatorName();
	//set received data from parent.
	void SetAllItemInfoOfPacketPage(const CString& strAllItemInfoOfPacketPage);
	//그래프 타입이 특수형일때 disable되어 있는 함수명에 GraphStyle에서 선택한 항목을 콤보에 display
	void SetIndicatorNameToEditFromDlgGraphColor(const CString& strGraphStyle);
	//Tab역할 하는 버튼 클릭했을때..
	void ButtonClicked(const int& nIndex);
	//Tree Class에서 현 Dlgd에서 설정한 모든값을 하나의 String으로 만들어 가지고자 할때.
	CString GetSubGraphDatas( const char *p_szIndicatorName);

// Dialog Data
	//{{AFX_DATA(CDlgGraphSetting)
	enum { IDD = IDD_DIALOG_GRAPHSETTING };
	CButton	m_Signal_Bt;
	CCheckComboBox	m_pPacketComboBox;
	CNoSystemMenuCombo	m_IndicatorFuctionComboBox;
	CString	m_strIndicatorName;
	CString	m_strCheckedPacketItems;
	BOOL	m_bSignalShow;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGraphSetting)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CExpandDialog m_ExpandDialog;
	// Generated message map functions
	//{{AFX_MSG(CDlgGraphSetting)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnEditchangeIndicatorfunctionname();
	afx_msg void OnSelchangeIndicatorfunctionname();
	afx_msg void OnSelchangeComboPacketlist();
	afx_msg void OnEtcsetBt();
	afx_msg void OnBtnNextSubgraph();
	afx_msg void OnBtnPrevSubgraph();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void CheckDataViewItems();
	bool DoThisIndicatorNeedSignal(const CString& strIndicatorName);
	CLineColor* m_pLineColor;
	void UnCheckAllCheckedPacketItems();
	//tree에 선택되어 있는 지표이름 보관한 변수.
	CString m_strIndicatorType;
	//모든 color를 담고 있는 String
	CString m_strSubGraphDatas;
	//Packet페이지에서 받은 string
	CString m_strAllItemInfoOfPacketPage;   
	//저장되어 있던 함수명과 선택한 함수명이 다를경우 DlgGraphColor에 설정된 값을 Setting 하지 않음.
	CString m_strOldIndicatorName;
	//속성Dll의 포인터.
	//CheckComboBox에 체크된 항목이 있는지 확인
	bool m_bCheckComboState;
	CChartLink* m_pItem;
	//사용할 Data에서 선택된 항목이 있는지 여부.
	bool m_bIsSelectedPacketData;
	//Graph에 색 설정하기 위한 Dialog 관리하는 클래스.
	CGraphColorCollection* m_GraphColorCollection;
	//툴팁.
	CDlgToolTip		m_toolTip;
	//툴팁생성
	void CreateToolTip();

	//PacketPage에서 선택된 Packet을 사용할 Data 콤보에 넣기.
	void AddStringToPacketCombo(); 
	
	//IndicatorFuntion 콤보의 list 채움.
	void AddStringToIndicatorFunctionCombo();
	void AddStringToIndicatorFunctionComboWhenNoSpecialGraphType();
	void AddStringToIndicatorFunctionComboWhenSpecialGraphType();

	//Enable or disable Scale Position Radio Button
	void SetDisableWindow(UINT nRadioButton, const BOOL bCheck);
	void SetRadioDisableWindow(const BOOL bCheck);

	//Color와 title 설정하기 위한 Control 지표별로 다르게 구성.
	void SetSubGraphDlgOnGraphSettingDlg();

	//사용할 Data에서 이전에 선택했던 항목에 체크 상태를 만들어 주기 위해
	void CheckPacketbyCheckedPacketItems();

	//일반지표 : 봉형, 선형, 바형
	bool IsGeneralIndicatorType() const;
	//none 지표 : 특수형, 데이터표현형, 선물옵션형
	bool IsNoneIndicatorType() const;

	void SetSubGraphDatas_Indicater();

//sy 2006.04.17
private:
	//지표명 변경에 따른 화면값 변경
	void CheckGraphSettingbyIndicatorName(const CString& strIndicatorName);
	//함수명에 따른 Packet 자동으로 체크하기
	void CheckDefaultPacketbyIndicatorInfo(	IGraphInfo *p_pIGraphInfo);
	void CheckGraphSetting2byIndicatorInfo(	IGraphInfo *p_pIGraphInfo, const char *p_szIndicatorName);
	void CheckSignalbyIndicatorInfo(		IGraphInfo *p_pIGraphInfo);
	void CheckSignal(const bool bIsShow);

	void SetSignal(CChartLink* pItem);

	BOOL m_bSignalUse;

private:	
	CGraphSetting2Data m_graphSetting2Data;

// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
protected:
	IGraphInfoManager *	m_pIGraphInfoManager;

// (2008/1/16 - Seung-Won, Bae) for MultiLanguage
protected:
	HWND	m_hOcxWnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGRAPHSETTING_H__C784ACD7_9191_46F3_939B_1BA9EFCB9CE0__INCLUDED_)
