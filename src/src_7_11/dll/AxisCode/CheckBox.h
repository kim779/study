#if !defined(AFX_CHECKBOX_H__F6C05737_70D6_4E3C_839E_E7A8BE0CD0AF__INCLUDED_)
#define AFX_CHECKBOX_H__F6C05737_70D6_4E3C_839E_E7A8BE0CD0AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CheckBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCheckBox window

#include "mybitmap.h"

#define FUTURESDLG 0
#define MMDTDLG 1

typedef struct Data
{
	BOOL		flag;
	BOOL		atm;
	CString		code;
	CString		name;
}DATA;


class CCheckBox : public CWnd
{
// Construction
public:
	CCheckBox();
	virtual ~CCheckBox();


// Attributes
public:

// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	void DeleteTool();
	void ShowRow(int row);
	void SetArray();	
	void RemoveAll();
	DATA GetData(int row, int col);
	void AddData(int row, int col, DATA& data);
	void DrawList(int row);
	int  GetInit(){ return m_nInit; }
	int  GetCount(){ return m_nCount; }
	int  GetScroll(){ return m_nScroll; }
	void SetScroll(int val){ m_nScroll = val; }
	void SetInit(int val){ m_nInit = val; }
	BOOL SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);
	int  GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);
	void SetCount(int ct){ m_nCount = ct; }
	void SetWnd(CWnd *pWnd){ m_pWnd = pWnd; }
	DWORD SetCheckCursor(int nCursorId, BOOL bRepaint = TRUE);
	void SetWnd(int Wnd);
	
	// Generated message map functions
protected:

	void ResetScrollBars();
	void Draw();
	void SetItem(CPoint point);
	
protected:
	
	CMyBitmap	m_BitmapNormal;
	CMyBitmap	m_BitmapClick;
	CTool		*m_pTool;
	
	int		m_nInit;		// 그려지는 위치 ..
	int		m_nCount;		// 컬럼 수...
	int		m_nScroll;		// 스크롤의 위치..
	int		m_iWnd;
	
	CWnd		*m_pWnd;

	int		m_Key;
	HCURSOR		m_hCursor;


	CMap<int, int, DATA, DATA>	m_Map;
	CStringArray			m_Array;

	//{{AFX_MSG(CCheckBox)
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKBOX_H__F6C05737_70D6_4E3C_839E_E7A8BE0CD0AF__INCLUDED_)
