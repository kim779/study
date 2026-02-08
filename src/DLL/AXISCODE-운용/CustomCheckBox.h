#pragma once
// CustomCheckBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustomCheckBox window

#include "tool.h"
#include "mybitmap.h"
#include <afxtempl.h>

typedef struct _Data
{
	BOOL		flag{};
	BOOL		atm{};
	CString		code;
	CString		name;
} _DATA;


class CCustomCheckBox : public CWnd
{
// Construction
public:
	CCustomCheckBox();
	virtual ~CCustomCheckBox();


// Attributes
public:

// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomCheckBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetEmpty(bool bEmpty = true) { m_bEmpty = bEmpty; }
	void DeleteTool();
	void ShowRow(int row);
	void SetArray();	
	void RemoveAll();
	_DATA GetData(int row, int col);
	void AddData(int row, int col, const _DATA& data);
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
	
	// Generated message map functions
protected:

	void ResetScrollBars();
	void Draw();
	
protected:
	
	CMyBitmap	m_BitmapNormal;
	CMyBitmap	m_BitmapClick;
	std::unique_ptr<CTool> m_pTool{};
	
	int		m_nInit{};		// 그려지는 위치 ..
	int		m_nCount{};		// 컬럼 수...
	int		m_nScroll{};		// 스크롤의 위치..
	
	CWnd* m_pWnd{};

	int		m_Key{};
	HCURSOR		m_hCursor{};

	CString		m_sCode;

	CMap<int, int, _DATA, _DATA>	m_Map;
	CStringArray			m_Array;

	//{{AFX_MSG(CCustomCheckBox)
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	bool		m_bEmpty{};
};

