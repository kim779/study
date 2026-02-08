#if !defined(AFX_QUICKTAB_H__5F09F59A_B285_4D3C_9AB8_61C2531B6CB5__INCLUDED_)
#define AFX_QUICKTAB_H__5F09F59A_B285_4D3C_9AB8_61C2531B6CB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QuickTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CQuickTab window

//<Debug Beginning>/////////////////////////////////
enum
{
	BTC_NONE = 0,
	BTC_TABS = 1,
	BTC_ALL	= 2,
};

// custom look
enum
{
	ETC_FLAT		  = 1, 
	ETC_COLOR	  = 2, // draw tabs with color
	ETC_SELECTION = 4, // highlight the selected tab
	ETC_GRADIENT  = 8, // draw colors with a gradient
	ETC_BACKTABS  = 16,
};
///////////////////////////////////////<Debug END>//


#include "ExToolTip.h"
#include "../DefineFiles/client_typedef.h"
class CQuickTab : public CTabCtrl
{
// Construction
public:
	CQuickTab();

// Attributes
protected:
	int m_nDrawType;
	static DWORD s_dwCustomLook;

public:
	COLORREF			m_crBack;
	CExToolTip		m_ExToolTip;
		
	CString m_strToolMsg;
	CList<QuickList, QuickList> *m_plistQuick;

	CUIntArray		m_iaTabPosData;

protected:
	virtual void DrawMainBorder(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DrawItemBorder(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual COLORREF GetTabColor(BOOL bSelected = FALSE);
	virtual COLORREF GetTabTextColor(BOOL bSelected = FALSE);

private:
	COLORREF Darker(COLORREF crBase, float fFactor);
	COLORREF Lighter(COLORREF crBase, float fFactor); 
	CSize FormatText(CString& sText, CDC* pDC, int nWidth);


	BOOL SetAdd_Item(CString strItem, int nIndex, int nImage, int nPos);
	void SetDelete_Item();
		
	BOOL SetEnableDraw(int nType = BTC_ALL);
	static void SetEnableCustomLook(BOOL bEnable = TRUE, DWORD dwStyle = ETC_FLAT | ETC_COLOR);

public:
	void SetInitTab(CList<QuickList, QuickList> *plistQuick);
	void SetDeleteAll();

	void SetBkgndColor(COLORREF color);



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQuickTab)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CQuickTab();

	// Generated message map functions
protected:
	//{{AFX_MSG(CQuickTab)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeQuickTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//afx_msg UINT OnNcHitTest(CPoint point);
	#if _MSC_VER >= 1400
		afx_msg LRESULT OnNcHitTest(CPoint point);
	#else
		afx_msg UINT OnNcHitTest(CPoint point);
	#endif
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUICKTAB_H__5F09F59A_B285_4D3C_9AB8_61C2531B6CB5__INCLUDED_)
