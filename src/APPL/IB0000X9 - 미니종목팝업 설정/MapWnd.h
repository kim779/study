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
#define IDC_BTN_LASTRUN		2001
#define IDC_BTN_CODENEWS	2002

using SArray = std::unique_ptr<CStringArray>;
//using SISE_DATA = CMap<CString, LPCSTR, SArray, SArray>;

//using SARRAY = std::shared_ptr<CStringArray>; 
using SISE_DATA = CMap<CString, LPCSTR, CMapStringToString*, CMapStringToString*>;

class CMapWnd : public CWnd, public CAxisExt
{
public:
	CMapWnd(CWnd *pParent);

public:
	CArray<CString,LPCSTR> m_codes;
	CArray<struct FIELD_INFO*, struct FIELD_INFO*> m_fields;
	CMapStringToString m_rtsmap;
	
//	CMap<CString, LPCSTR, SArray*, SArray*> m_search;
	CMap<CString, LPCSTR, CStringArray*, CStringArray*> m_search;
	CMap<CString, LPCSTR, CSisePopup*, CSisePopup*> m_wnds;

	std::vector<std::unique_ptr<CStringArray>> _vSearch;

	SISE_DATA _search;

	
	SISE_DATA m_datas;
	int m_transparent, m_newX, m_newY;
	CSisePopup *m_pOldHoverWnd;
	BOOL m_bFlash, m_bTracking, m_bSimple, m_bTopMost, m_bExit, m_bLastRun, m_bCodeNews;
	VIEW_TYPE m_vtype;
	COLORREF m_crPopupBack, m_crPopupFlash;
	CWnd* m_pMiniWnd;

public:	// interest
	CArray<CString,LPCSTR> m_gpno, m_gpnm;

public:
	CSliderCtrl m_sbTransparent;
	CButton m_btnFlash, m_btnAdd, m_btnViewType, m_btnFields[20], m_btnSimpleColor, m_btnTopMost, m_btnInterAdd,m_btnLastRun,m_btnNews;
	CColorButton m_btnBkColor, m_btnFlashColor;
	CComboBox m_cbInterest;
	CEdit m_edCode;

protected:
	void ReadFile(LPCSTR path, vector<char> &dst);
	void LoadCodes();
	void DeletePopup(CSisePopup *pwnd);
	DWORD GetPopupWidth();
	DWORD GetPopupHeight();
	CSisePopup* FindPopup(LPCSTR code);
	void LoadConfig();
	void SaveConfig();
	void AddSearchData(const CString &code, const CString &name);
	void AdjustHangul(CString &text);

public:
	void ChangeTransparent(int val);
	void AddCode(LPCSTR code, LPRECT pRC = NULL);
	void DelCode(LPCSTR code);
	void RefreshWnd(LPCSTR key, CMapStringToString *pd);
	struct FIELD_INFO * GetFieldInfo(LPCSTR rsym);
	int  GetFieldIndex(LPCSTR rsym);
	void AlignCustom(struct FIELD_INFO *fi, CSisePopup *pbase, bool bUpper, VIEW_TYPE vtype);
	void DrawGradient(CDC *m_dc,CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
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
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()afx_msg void OnFlash();
	afx_msg void OnViewType();
	afx_msg void OnSimpleColor();
	afx_msg void OnTopMost();
	afx_msg void OnCodeAdd();
	afx_msg void OnBkColorChange();
	afx_msg void OnFlashColorChange();
	afx_msg void OnFieldChange(UINT nID);
	afx_msg void OnInterAdd();
	afx_msg void OnLastView();
	afx_msg void OnCodeNews();
};

