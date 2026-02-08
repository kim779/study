#pragma once
// MarketDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMarketDlg dialog
#include "excel.h"
#include "BkUtil.h"
#include "fxTab.h"
#include "SListCtrl.h"
#include "ColorButton.h"
#include "ColoredListCtrl.h"
#include "ImageTreeCtrl.h"
#include "AxBrower.h"
#include "../../control/fx_misc/fxImgButton.h"


//#include "BkUtil.h"

#define	AXI_BASE			21000
#define AXI_CHANGEPOPUP		(AXI_BASE + 54)		// create(change) popup window
#define ROUND_PANEL		1
#define ROUND_CONTENTS	2

#define CTRLMOVEX      0
#define CTRLMOVEY      0
#define CTRLMOVE_X  -342
#define CTRLMOVE_Y  -325

////////////////////////// 
// 클래스명	    : CMarketDlg
// 설 명	    : 종합시황뉴스(키워드 설정/필터링결과/필터링설정/제공처설정) 적용
//                컨트롤 위치는 다이얼로그 템플릿에서 위치시키고 각 탭마다 규일한 값만큼 이동시킴
// 작성일시	    : 2009/06/04
// 작성자       : kbkim(수정자)
// 관련요건	    : 000006(조원형:2009/05/12)
////////////////////////// 
class CMarketDlg : public CDialog/*, public CBkUtil*/
{
// Construction
public:
	CMarketDlg(CWnd* pParent = NULL);   // standard constructor
	CMarketDlg(CString strRoot, CString strUser, CWnd* pWizard, CFont* pFont, CWnd* pParent = NULL);   // standard constructor

	enum Tab_Index { cKEYWORD_SET=0, cFILLER_RESULT, cFILTER_SET, cMARKET_SET} m_MarketType;

// Dialog Data
	//{{AFX_DATA(CMarketDlg)
	enum { IDD = IDD_MARKET_DLG };
	CEdit	m_FilterEdit20;
	CEdit	m_FilterEdit19;
	CEdit	m_FilterEdit18;
	CEdit	m_FilterEdit17;
	CEdit	m_FilterEdit16;
	CEdit	m_FilterEdit15;
	CEdit	m_FilterEdit14;
	CEdit	m_FilterEdit13;
	CEdit	m_FilterEdit12;
	CEdit	m_FilterEdit11;
	CEdit	m_FilterEdit10;
	CButton	m_FilterCheck20;
	CButton	m_FilterCheck19;
	CButton	m_FilterCheck18;
	CButton	m_FilterCheck17;
	CButton	m_FilterCheck16;
	CButton	m_FilterCheck15;
	CButton	m_FilterCheck14;
	CButton	m_FilterCheck13;
	CButton	m_FilterCheck12;
	CButton	m_FilterCheck11;
	CButton	m_FilterCheck10;
	CfxImgButton	m_Filterlist_Excel;
	CfxImgButton	m_btnPlay;
	CfxImgButton	m_btnPath;
	CStatic	m_editSndFile;
	CfxImgButton	m_MarketCancelBtn;
	CImageTreeCtrl	m_MarketTreeCtrl;
	CfxImgButton	m_MarketBtn;
	CColoredListCtrl m_FilterResultListCtrl;
	CButton		m_FilterUseCheckBtn;
	CButton		m_FilterSoundCkeckBbtn;
	CfxImgButton m_FilterSaveEdit;
	CEdit		m_FilterEdit9;
	CEdit		m_FilterEdit8;
	CEdit		m_FilterEdit7;
	CEdit		m_FilterEdit6;
	CEdit		m_FilterEdit5;
	CEdit		m_FilterEdit4;
	CEdit		m_FilterEdit3;
	CEdit		m_FilterEdit2;
	CEdit		m_FilterEdit1;
	CfxImgButton m_FilterDelBtn;
	CButton		m_FilterCheck9;
	CButton		m_FilterCheck8;
	CButton		m_FilterCheck7;
	CButton		m_FilterCheck6;
	CButton		m_FilterCheck5;
	CButton		m_FilterCheck4;
	CButton		m_FilterCheck3;
	CButton		m_FilterCheck2;
	CButton		m_FilterCheck1;
	CButton		m_FilterStatic;
	CStatic		m_KeyWordStatic;
	CListBox	m_KeyStepListBox;
	CComboBox	m_KeyStepComboBox;
	CfxImgButton m_KeyOKBtn;
	CListBox	m_KeyListBox;
	CStatic		m_KeyLayerStatic;
	CEdit		m_KeyEdit;
	CStatic		m_KeyColorStatic;
	CColorButton m_KeyBtnColor;
	CfxImgButton m_KeyBtnCancel;
	CfxImgButton m_KeyBtnEdit;
	CfxImgButton m_KeyBtnDel;
	CfxImgButton m_KeyBtnAdd;
	CStatic		m_KeyBayStatic;
	CComboBox	m_KeyAddComboBox;
	CButton		m_KeyGroup2;
	CButton		m_KeyGroup1;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMarketDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void RunExcel(bool visible = true);
	bool RunExcel_();
	CAListArray m_arKeyList;
	CUIntArray  m_arStepColor;

	CNewsListArray m_arNewsList;
	
	bool SetMarketNewsData(news_info* pNnewsData);
	void ChangeColors(COLORREF clrBKColor, COLORREF clrRoundColor);

	CString		GetAxVariant(int cmd, CString data=_T(""));
	CString		GetAxCode(int cmd,LPARAM lParam);

protected:
	BOOL InitWindows();
	HBITMAP getBitmap(CString bmps);
	COLORREF GetAxColor(int iColor);
	CFont*	 GetAxFont(int size ,bool bBold,CString str);

	void WndAllCtrlShow(BOOL bShow);
	void WndAllCtrlShow_Key(BOOL bShow);
	void WndAllCtrlShow_FilterResult(BOOL bShow);
	void WndAllCtrlShow_Filter(BOOL bShow);
	void WndAllCtrlShow_Market(BOOL bShow);

	CBitmap* getAxBitmap(CString path);

	void MoveCtrl(UINT nCtrlID, int nCx, int nCy);
	CString GetPalette();
	void DrawRoundBitmap(CDC *pDC, CRect rc, int nKind);
	void DrawBitmap(CDC *pDC, CRect rc, CString strImage);
	void Excute(CString fname);
	CString Variant(int comm, CString data);
	COLORREF GetIndexColor(int index);

	COLORREF    m_clrBk;
	COLORREF    m_clrWriteBk;
	COLORREF    m_clrRoundBk;
	
	CString     m_strPal;
	HBRUSH		m_hRoundBk;
	HBRUSH		m_hBk;
	CBrush		m_br;
	
	int			m_nSelIndex;

	// 각 탭에 맞게 컨트롤 Move 및 Show(Hidden)
	void CreateKeyWordSetWnd();
	void CreateFilterResultWnd();
	void CreateFilterSetWnd();
	void CreateMarketSetWnd();
	//-
	void draw_groupBox(CPaintDC *pdc, int ids);

	std::unique_ptr<CfxTab>	m_pTab;
	CFont*	m_pFont;
	CString m_szRootDir;
	CString m_szUser;
	CWnd*	m_pAxWnd;

	CString m_sFilterStr1;
	CString m_sFilterStr2;
	CString m_sFilterStr3;
	CString m_sFilterStr4;
	CString m_sFilterStr5;
	CString m_sFilterStr6;
	CString m_sFilterStr7;
	CString m_sFilterStr8;
	CString m_sFilterStr9;
	CString m_sFilterStr10;
	CString m_sFilterStr11;
	CString m_sFilterStr12;
	CString m_sFilterStr13;
	CString m_sFilterStr14;
	CString m_sFilterStr15;
	CString m_sFilterStr16;
	CString m_sFilterStr17;
	CString m_sFilterStr18;
	CString m_sFilterStr19;
	CString m_sFilterStr20;


	BOOL m_bMoveKeyCtrl;
	BOOL m_bMoveFilterResult;
	BOOL m_bMoveFilter;
	BOOL m_bMoveMarket;

////////// 키워드 설정 관련 함수/////////////////////////////
	void SetKeyWordData();
	void OnButtonEdit();
public:
////////// 필터링 결과 관련 함수/////////////////////////////
	void SetFilterResultSetData();
	void AddFilterResultData(CString strTime, CString strTitle, CString strKey, CString strCodeName, CString hnam);
	BOOL FilterNewsData(CString strNewsTitle);
	BOOL FilterMarketNewsData(CString sRootName, CString sSubName);
	BOOL m_bFilter;  // true이면 결과 반영

////////// 필터링 설정 관련 함수/////////////////////////////
	void SetFilterSetData();
	void OnFilterCheck(CButton* pCheckButton, CEdit* pEdit);
	void FilterCtrlClear(CButton* pCheckButton, CEdit* pEdit);
	
////////// 제공처 설정 관련 함수/////////////////////////////
	void InitMarketTreeData();
	HTREEITEM FillTreeControl(HTREEITEM hParent, CString strDate);


	// Generated message map functions
	//{{AFX_MSG(CMarketDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeKeyList();
	afx_msg void OnSelchangeKeyStepList();
	afx_msg void OnKeyButtonAdd();
	afx_msg void OnKeyButtonDel();
	afx_msg void OnKeyButtonEdit();
	afx_msg void OnSelchangeKeyStepCombo();
	afx_msg void OnKeyOkbutton();
	afx_msg void OnKeyButtonCancel();
	afx_msg void OnFilterCheck1();
	afx_msg void OnFilterCheck2();
	afx_msg void OnFilterCheck3();
	afx_msg void OnFilterCheck4();
	afx_msg void OnFilterCheck5();
	afx_msg void OnFilterCheck6();
	afx_msg void OnFilterCheck7();
	afx_msg void OnFilterCheck8();
	afx_msg void OnFilterCheck9();
	afx_msg void OnFilterUsecheck();
	afx_msg void OnFilterDelbtn();
	afx_msg void OnFilterSavebtn();
	afx_msg void OnMarketBtn();
	afx_msg void OnClickFilterResultList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMarketcancelBtn();
	afx_msg void OnFilterSoundcheck();
	afx_msg void OnBtnPlay();
	afx_msg void OnBtnPath();
	afx_msg void OnEditchangeKeyAddcombo();
	afx_msg void OnExcel();
	afx_msg void OnFilterlistExcel();
	afx_msg void OnFilterCheck10();
	afx_msg void OnFilterCheck11();
	afx_msg void OnFilterCheck12();
	afx_msg void OnFilterCheck13();
	afx_msg void OnFilterCheck14();
	afx_msg void OnFilterCheck15();
	afx_msg void OnFilterCheck16();
	afx_msg void OnFilterCheck17();
	afx_msg void OnFilterCheck18();
	afx_msg void OnFilterCheck19();
	afx_msg void OnFilterCheck20();
	//}}AFX_MSG
//	afx_msg LRESULT OnChangeTab(WPARAM wParam, LPARAM lParam);
	afx_msg void OnChangeTab(NMHDR* pHeader, LRESULT* pResult);
	afx_msg LONG OnChangeColorPicker(UINT lParam, LONG wParam);
	DECLARE_MESSAGE_MAP()
};

