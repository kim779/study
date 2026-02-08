#pragma once
// CheckBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCheckBox window

#include "tool.h"
#include "mybitmap.h"
#include <afxtempl.h>

class CStockCheckBox : public CWnd
{
// Construction
public:
	CStockCheckBox();
	virtual ~CStockCheckBox();


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
	void AddData(int row, int col, const DATA& data);
	void DrawList(int row);
	int  GetInit(){ return m_nInit; }
	int  GetCount(){ return m_nCount; }
	int  GetScroll(){ return m_nScroll; }
	void SetScroll(int val){ m_nScroll = val; }
	void SetInit(int val){ m_nInit = val; }
	BOOL SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);
	int  GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);
	void SetCount(int ct){ m_nCount = ct; if(m_nCount == 25) m_nColCount = 8;}	//2014.08.27 KSJ 종목파일이 바뀌지 않았을때 처리
	void SetWnd(CWnd *pWnd){ m_pWnd = pWnd; }
	DWORD SetCheckCursor(int nCursorId, BOOL bRepaint = TRUE);
	void SearchStock(CString strName, int nNext);								//2015.09.09 KSJ 종목서치(0:현재, 1:앞으로, -1:뒤로)
	
	// Generated message map functions
protected:

	void ResetScrollBars();
	void Draw();
	
protected:
	void SetMonth(int row);		//2014.07.29 KSJ 선택한 월물을 보여준다.
	bool m_bSpread{};		//2014.07.29 KSJ 선택한게 스프레드인지
	int m_nSFSelect{};		//2014.07.29 KSJ 선택한 월물이 몇번째인지.
	int m_nSPSelect{};		//2014.07.29 KSJ 선택한 월물이 몇번째인지.
	int m_nColCount{};		//2014.08.27 KSJ 종목파일이 바뀌지 않았을때는 8로 바뀌었을때는 14로
	int m_nSearchRow{};		//2014.08.27 KSJ 검색했때의 색깔을 지정해주기 위해

	int m_nArrSearch[50]{};		//2015.09.09 KSJ 검색된 인덱스 저장
	int m_nCurrIndex{};		//2015.09.09 KSJ 현재 인덱스

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

	CMap<int, int, DATA, DATA>	m_Map;
	CStringArray			m_Array;

	//{{AFX_MSG(CCheckBox)
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

