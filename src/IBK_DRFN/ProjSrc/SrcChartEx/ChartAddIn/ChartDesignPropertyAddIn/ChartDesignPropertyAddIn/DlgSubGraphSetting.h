#if !defined(AFX_DLGSUBGRAPHSETTING_H__DA53AC9C_848B_4CA3_97EE_B922167258A9__INCLUDED_)
#define AFX_DLGSUBGRAPHSETTING_H__DA53AC9C_848B_4CA3_97EE_B922167258A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSubGraphSetting.h : header file
//

#include "Include_AddIn.h"			// for Ixxxxxx.h
#include "colourpicker.h"
#include "DlgToolTip.h"
#include "NoSystemMenuCombo.h"
#include "NoSystemMenuEdit.h"		// for CNoSystemMenuEdit

/////////////////////////////////////////////////////////////////////////////
// CDlgSubGraphSetting dialog

class CDlgGraphSetting;
class CDlgSubGraphSetting : public CDialog
{
// Construction
public:
	CDlgSubGraphSetting( HWND p_hOcxWnd, CWnd* pDlgChartWnd, CWnd* pParent = NULL);	// standard constructor
	~CDlgSubGraphSetting();											// standard destructor

public:
	void SetDrawStyle(const int nDrawStyle);
	void SetGraphType(const int graphtype);
	//현Dlg 생성할때 속성 Dll에서 Style을 Set.
	void SetGraphStyle(const int nStyle);
	//지표설정 에서 선택된 지표의 타입이 선형일때 InitDialog에서 설정한 6개의 선색깔 Set.
	void SetLineColor(const CString& strLineColor);
	//ocx에 String이 넘어갈때 변하지 않는 SubGraphName을 넘기기 위해서 속성 Dll에서 얻은 값을 설정
	void SetSubGraphName(const CString& strSubGraphName);
	//dlg위의 Control값 Setting
	void SetSubGraphDatas(const CString& strSubGraphDatas);
	//각각의 값들을 하나의 String 으로 만들기.
	CString GetAllSettingData( const char *p_szIndicatorName);

	//Color를 위한 String.
	CString m_strclrUpEdge;
	CString m_strclrUp;
	CString m_strclrEqual;
	CString m_strclrEqualEdge;
	CString m_strclrDownEdge;
	CString m_strclrDown;	
// Dialog Data
	//{{AFX_DATA(CDlgSubGraphSetting)
	enum { IDD = IDD_DIALOG_SUBGRAPHSETTING };
	CColourPicker	m_clrEqualEdge;
	CNoSystemMenuEdit	m_TitleEdit;
	CNoSystemMenuEdit	m_UpThicknessEdit;
	CNoSystemMenuEdit	m_DownThicknessEdit;
	CNoSystemMenuEdit	m_EqualThicknessEdit;
	CColourPicker	m_clrUp;
	CColourPicker	m_clrUpEdge;
	CColourPicker	m_clrDown;
	CColourPicker	m_clrDownEdge;
	CColourPicker	m_clrEqual;
	CNoSystemMenuCombo	m_TypeCombo;
	CNoSystemMenuCombo	m_DrawStyleCombo;
	CComboBox	m_GraphStyleCombo;
	BOOL	m_bFillUp;
	int		m_nUpThickNess;
	BOOL	m_bFillDown;
	int		m_nDownThickNess;
	int		m_nBorderWidth;
	CString	m_strTitle;
	BOOL	m_bGraphHide;
	int		m_nGraphType;
	int		m_nGraphStyle;
	int		m_nDrawStyle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSubGraphSetting)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSubGraphSetting)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeGraphType();
	afx_msg void OnSelchangeGraphstyle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int GetDrawStyle(CString& strStringValue);
	int GetDrawStyle();
	int GetGraphStyle(CString &strControlValue);
	int GetGraphType(CString &strControlValue);
	//부모 윈도우 포인터
	CDlgGraphSetting* m_pDlgGraphSetting;
	//변할수 없는 지표이름.
	CString m_strSubGraphName;
	//Graph의 Style. 구름식, 계단식, 일반식..등..
	CString m_strGraphStyle;
	//툴팁.
	CDlgToolTip		m_toolTip;

	//Dlg의 모든 버튼에 Color초기화
	void InitButtonColor();
	//RGB값을 저장하기 위해서 Picker를 String으로 변환
	void MakeFromPickerColorToStrColor();
	//툴팁
	void CreateToolTip();

	//GraphStyle콤보에 속성 Dll에서 받은 정보를 Add.
	void Set_GraphTypeCombo();
	void Set_GraphStyleCombo();
	void Set_GraphDrawStyleCombo();

	//GrapyType/Style 검사.
	void CheckingGraphTyleAndStyleAndCombo();
	void CheckingGraphTyleAndStyle();

// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
protected:
	IGraphInfoManager *	m_pIGraphInfoManager;
public:
	void SetIGraphInfoManager( IGraphInfoManager *p_pIGraphInfoManager);

	//graphDrawStyle 관련 콤보의 enable 여부
	bool IsEnable_GraphDrawStyleCombo(const CGraphBaseData::GRAPHTYPE eGraphType) const;
	//graphDrawStyle 이 없는 타입
	bool IsNoGraphDrawStyle(const CGraphBaseData::GRAPHTYPE eGraphType) const;

// (2008/1/16 - Seung-Won, Bae) for MultiLanguage
protected:
	HWND	m_hOcxWnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUBGRAPHSETTING_H__DA53AC9C_848B_4CA3_97EE_B922167258A9__INCLUDED_)
