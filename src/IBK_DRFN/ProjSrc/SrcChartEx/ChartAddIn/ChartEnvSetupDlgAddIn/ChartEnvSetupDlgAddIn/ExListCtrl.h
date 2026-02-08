#if !defined(AFX_EXLISTCTRL_H__A2FB355F_2F05_4907_99BC_31FBFCA47CAC__INCLUDED_)
#define AFX_EXLISTCTRL_H__A2FB355F_2F05_4907_99BC_31FBFCA47CAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExListCtrl.h : header file
//


#include <afxcoll.h>
#include <afxtempl.h>



/////////////////////////////////////////////////////////////////////////////
// CExListCtrl window

#define MAX_COLUMN	10
#define MESSAGE_NONE 909090


struct ControlMessageInfo
{
	int m_iMessageID;
	void (* pEventHandler)(LPVOID lpVoid);
	ControlMessageInfo()
	{
		m_iMessageID = MESSAGE_NONE;
	}
};



struct SubItemControlsInfo
{
	int m_nIdx;
	CWnd *m_pWnd;
	COLORREF m_BKColor;
	SubItemControlsInfo()
	{
		m_nIdx = -1;
		m_pWnd = NULL;
		m_BKColor = RGB(255,255,255);		// GetSysColor(COLOR_BTNFACE);
	}

	CArray<ControlMessageInfo, ControlMessageInfo> m_arrControlMessageInfo;
};




//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 9
// Comments		: 
// Using way	: 
// See			: CListCtrl
//-----------------------------------------------------------------------------
class CExListCtrl : public CListCtrl
{
// Construction
public:
	CExListCtrl();

	
// Attributes
public:



protected:
	CPtrArray *m_pControlPtrArray;


private:
	int m_nSelctedItemIdx;
	int m_nSubItem;
	int m_nControlIDCounter;

// Operations
public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExListCtrl)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL


// Implementation
public:
	virtual ~CExListCtrl();
	BOOL Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );
	
	BOOL AddToControlsData(int nIndex, CString strData);
	BOOL AddToControlsData(int nIndex, int nCurVal, int nStart, int nEnd );

	void AdjustControlsAccordingToHeaderWidth(HD_NOTIFY * pNotify);
	void AttachEvent_HandlerForItemControl(int iIndex,UINT unMessage,void (*pEventHandler)(LPVOID lpvoid));

	void SetControlBKColor(int index ,COLORREF col);
	
	BOOL SetColumnRatio( CString szEachRatio );
	
	void SetItemControl( int nColumnIdx );
	void SetItemControl( int nRowIdx, int nColumnIdx );
	
	void MapControlsForSubItems( void );
	void ShowControls(BOOL bShow);
	void EnableControls(BOOL bEnable);
	void ReleaseControls( void );
	
private:
	int ControlIndexDuplicating(int nIndex);
	BOOL DetachControl( int nIndex );
	void GetAndDisplayControlsData();




	// Generated message map functions
protected:
	//{{AFX_MSG(CExListCtrl)
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXLISTCTRL_H__A2FB355F_2F05_4907_99BC_31FBFCA47CAC__INCLUDED_)
