#if !defined(AFX_MULTIBAR_H__65E88380_0E89_4804_980A_55DE5D564209__INCLUDED_)
#define AFX_MULTIBAR_H__65E88380_0E89_4804_980A_55DE5D564209__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MultiBar.h : header file
//
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CXScalInfo
class CXScalInfo
{
public:
	CRect m_rc;
	CString m_szText;
};
typedef CList<CXScalInfo*, CXScalInfo*> LIST_CXScalInfo;


/////////////////////////////////////////////////////////////////////////////
// CMBarItem
class CMBarItem
{
public:
	CString m_szItem;
	long	m_ValueA,	//매도
			m_ValueB;	//매수

	void SetItem(char* szData)
	{
		CString szTmp;
		char aTmp[30]={0,};
		int i=0;
		memcpy(aTmp, &szData[i], 20);
		szTmp = aTmp; szTmp.TrimLeft();szTmp.TrimRight();
		m_szItem = szTmp;

		i+=20;
		memcpy(aTmp, &szData[i], 12);
		szTmp = aTmp; szTmp.TrimLeft();szTmp.TrimRight();
		m_ValueA = atol(szTmp);

		i+=12;
		memcpy(aTmp, &szData[i], 12);
		szTmp = aTmp; szTmp.TrimLeft();szTmp.TrimRight();
		m_ValueB = atol(szTmp);
	}

	long GetMin() { return min(m_ValueA, m_ValueB); }
	long GetMax() { return max(m_ValueA, m_ValueB); }
};
typedef CList<CMBarItem*, CMBarItem*> List_CMBarItem;

/////////////////////////////////////////////////////////////////////////////
// CMBarChartInfo
class CMBarChartInfo
{
public:
	int		m_nXDivCnt, m_nYDivCnt;
	long	m_lMin, m_lMax;
	int		m_nItemCnt;

	int		m_nMarginTop,
			m_nMarginLR;

	// TextScale Margin
	int		m_YTextMargin, m_XTextMargin, m_YTopMargin;
public:
	CMBarChartInfo();
	void	DoClear();
};

/////////////////////////////////////////////////////////////////////////////
// CMBarItmManager
class CMBarItmManager
{
public:
	CMBarItmManager();
	~CMBarItmManager() { ClearList(); }
	CMBarChartInfo  m_ChartInfo;
	void SetOwner(CWnd* pWnd) {m_pOwner=pWnd;}

public:
	void ClearList();
	void PrepareDraw(CDC* pDC, CRect* pDrawRect, CRect* pChartRect);

	void SetTooltipInit(CToolTipCtrl* pToolTip);
	void SetPacketData(char* pPacketData, long nLen);

	int  GetDrawTopMargin() { return m_ChartInfo.m_YTopMargin;}
	int  GetDrawAllHeight() { return m_pDrawRect->Height()-m_ChartInfo.m_YTopMargin;}

	void DrawBar(CDC* pDC, LIST_CXScalInfo& listXScale);
	int GetCount() { return m_List.GetCount(); }

protected:
	CToolTipCtrl* m_pToolTip;
	CWnd*		  m_pOwner;

	CSize	m_TextExtent;
	List_CMBarItem	m_List;

	CRect *m_pDrawRect, *m_pChartRect;

	// SetPacketData호출후에.
	void _DoCalcScale();
	void _DoClearTooltip();

};

/////////////////////////////////////////////////////////////////////////////
// CMultiBar window
class CMultiBar : public CButton
{
// Construction
public:
	CMultiBar();
	CToolTipCtrl m_ToolTip;

// Attributes
public:
	BOOL m_bHover, m_bTracking;

	//Color
	CBrush m_crBuy, m_crSell;
//	CBrush m_crBack, m_crScale;
	CPen   m_crScalePen;

	//Font
	CFont m_textFont;

	CMBarItmManager m_Manager;
	
// Operations
public:
	void SetPacketData(LPBYTE pPacketData, long nLen);

	void DrawTitle(CDC* pDC, CRect* pRect);
	void DrawBar(CDC* pDC, LIST_CXScalInfo& listXScale);
	void DrawScale(CDC* pDC, LIST_CXScalInfo &listXScale);
	void DrawLine(CDC* pDC, CRect* pRect, CRect* prcClient);
	void DoDrawYScale(CDC* pDC, CRect rc, long nMin, long nMax);

	void SetToolTipText(CRect* pRect, CString sTip);
	CString GetComma(LPCSTR pData);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMultiBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMultiBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam) ;
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
// 	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTIBAR_H__65E88380_0E89_4804_980A_55DE5D564209__INCLUDED_)
