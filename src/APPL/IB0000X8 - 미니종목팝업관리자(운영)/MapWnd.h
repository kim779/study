#pragma once

#include "data_io.h"
#include "AxisExt.h"
#include "SisePopup.h"
#include "ColorButton.h"

#define IDC_SB_TRANSPARENT	1000
#define IDC_BTN_FLASH		1001
#define IDC_EDIT_CODE		1002
#define IDC_BTN_CODEADD		1003
#define IDC_BTN_VIEWTYPE	1004
#define IDC_BTN_BKCOLOR		1005
#define IDC_BTN_FLASHCOLOR	1006
#define IDC_BTN_SIMPLECOLOR	1007
#define IDC_BTN_TOPMOST		1008
#define IDC_CB_INTEREST     1009
#define IDC_BTN_INTERADD    1010
#define IDC_BTN_FIELD_START	2000
#define IDC_BTN_LASTVIEW	2001
#define IDC_BTN_CODENEWS	2002

#define DEF_HISTORY_KEY		255

#define SURL "http://futurewiz.co.kr/ibk/winechart/bighand/index.asp"
#define FURL "http://futurewiz.co.kr/ibk/winechart/okstone/index.asp"

#define WINE_S		"IB714100"	//옥석
#define WINE_F		"IB714200"	//큰손
#define WINE_JOIN	"IB714400"	//가입/해지

typedef CMap<CString,LPCSTR, CMapStringToString*,CMapStringToString*> SISE_DATA;



class CMapWnd : public CWnd, public CAxisExt
{
public:
	CMapWnd(CWnd *pParent);

public:
	CArray<CString,LPCSTR> m_codes;
	CArray<struct FIELD_INFO*, struct FIELD_INFO*> m_fields;
	CMapStringToString m_rtsmap;
	CMap<CString,LPCSTR, CStringArray*, CStringArray*> m_search;
	CMap<CString, LPCSTR, CSisePopup*, CSisePopup*> m_wnds;
	SISE_DATA m_datas;

	std::vector<std::unique_ptr<CMapStringToString>> _vData;
	std::vector<std::unique_ptr<CSisePopup>> _vWnds;
	std::vector<std::unique_ptr<CStringArray>> _vSearch;


	int m_transparent, m_newX, m_newY;
	CSisePopup *m_pOldHoverWnd;
	BOOL m_bFlash, m_bTracking, m_bSimple, m_bTopMost, m_bExit, m_bCodeNews, m_bLastView;
	VIEW_TYPE m_vtype;
	COLORREF m_crPopupBack, m_crPopupFlash;
	CWnd*	m_mainWnd;
	bool	m_bMiniMode;
	std::unique_ptr<CSisePopup> m_pHistroy;
	CString m_codeHist;
	CString m_user;
	CString m_sMap;
	CStringArray m_arrMap;
	HWND	m_wineHwnd;
	CMapStringToString m_mapCodeToName;

public:	// interest
	CArray<CString,LPCSTR> m_gpno, m_gpnm;

public:
	CSliderCtrl m_sbTransparent;
	CButton m_btnFlash, m_btnAdd, m_btnViewType, m_btnFields[20], m_btnSimpleColor, m_btnTopMost, m_btnInterAdd, m_btnLastView,m_btnCodeNews;
	CColorButton m_btnBkColor, m_btnFlashColor;
	CComboBox m_cbInterest;
	CEdit m_edCode;
	CRect m_rectNews;

protected:
	void ReadFile(LPCSTR path, vector<char> &dst);
	void LoadCodes();
	void ParseSise(LPCSTR dat, int len);
	void ParseAlert(LPCSTR dat, int len);
	void ParseAlertx(struct _alertR* alertR);
	void parsingNews(CString datB);
	void parsingNewsx(struct _alertR* alertR);
	CSisePopup* CreatePopup(LPCSTR code, LPRECT pRC);
	CSisePopup* CreateHistPopup( LPCSTR code, LPRECT pRC );
	void DeletePopup(CSisePopup *pwnd);
	DWORD GetPopupWidth();
	DWORD GetPopupHeight();
	CSisePopup* FindPopup(LPCSTR code);
	void LoadConfig();
	void SaveConfig();
	void AddSearchData(const CString &code, const CString &name);
	void AdjustHangul(CString &text);
	//void TestSetting();
	void LoadInterest();
	void sendTR(CString trCode, char* datB, int datL, int key);
public:
	void ChangeTransparent(int val);
	void AddCode(LPCSTR code, LPRECT pRC = NULL);
	void DelCode(LPCSTR code);
	void RequestCode(CString code = "");
	void RefreshWnd(LPCSTR key, CMapStringToString *pd);
	struct FIELD_INFO * GetFieldInfo(LPCSTR rsym);
	int  GetFieldIndex(LPCSTR rsym);
	void AlignCustom(struct FIELD_INFO *fi, CSisePopup *pbase, bool bUpper, VIEW_TYPE vtype);

	void QuerySACAQ601();
	void ParseSACAQ601(char* pBuf);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	bool IsDirect();
	virtual ~CMapWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChildClose(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetTracking(WPARAM wParam, LPARAM lParam);
	afx_msg void OnFlash();
	afx_msg void OnViewType();
	afx_msg void OnSimpleColor();
	afx_msg void OnTopMost();
	afx_msg void OnCodeAdd();
	afx_msg void OnBkColorChange();
	afx_msg void OnFlashColorChange();
	afx_msg void OnFieldChange(UINT nID);
	afx_msg void OnInterAdd();
	afx_msg void OnBtnLastView();
	afx_msg void OnBtnCodeNews();
	afx_msg LRESULT OnViewProperty(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAlignHUpper(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAlignHLower(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAlignVUpper(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAlignVLower(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLastPosUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCodeChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddNewCode(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetMainWnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHTSCloseFMini(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWFFlash(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWFViewType(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWFSimpleColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWFTopMost(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWFBkColorChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWFFlashColorChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWFFieldChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLastView(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnCodeNews(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInterest(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNewsView(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHotTrace(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTransMax(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTransMin(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHistoryView(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHistoryClose(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWine(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWineWnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInGong(WPARAM wParam, LPARAM lParam);
};

