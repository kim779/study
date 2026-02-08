#pragma once
// CheckBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCheckBox window

#include "tool.h"
#include "mybitmap.h"
#include <afxtempl.h>

// typedef struct Data
// {
// 	BOOL		flag;
// 	BOOL		atm;
// 	CString		code;
// 	CString		name;
// }DATA;


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
	void AddData(int row, int col, const DATA& data);
	void DrawList(int row);
	int  GetInit(){ return m_nInit; }
//	int  GetInit();
	int  GetCount(){ return m_nCount; }
	int  GetScroll(){ return m_nScroll; }
	void SetScroll(int val){ m_nScroll = val; }
	void SetInit(int val){ m_nInit = val; }
//	void SetInit(int val);
	BOOL SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);
	int  GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);
	void SetCount(int ct){ m_nCount = ct; }
	void SetWnd(CWnd *pWnd){ m_pWnd = pWnd;}
	DWORD SetCheckCursor(int nCursorId, BOOL bRepaint = TRUE);

	void InitArray() { m_Array.RemoveAll();}	//2015.07.14 KSJ 초기화
	
	BOOL m_bWeek{};
	void DrawWeekList(int row);
	// Generated message map functions
protected:
	void ResetScrollBars();
	void Draw();
	void DrawWeek();
	void weeklbtnClick(UINT nFlags, CPoint point);
	int m_nWCallSelect{};
	int m_nWPutSelect{};

protected:
	void SetMonth(int row);	//2014.07.29 KSJ 선택한 월물을 보여준다.
	void SetWeekMonth(int row); 
	bool m_bCall{};		//2014.07.29 KSJ 선택한 월물이 콜인지 풋인지.
	int m_nCallSelect{};		//2014.07.29 KSJ 선택한 월물이 몇번째인지.
	int m_nPutSelect{};		//2014.07.29 KSJ 선택한 월물이 몇번째인지.
	int	m_nStandardNum{};		//2014.08.28 KSJ 종목파일이 바뀌기전에 문제가 생기므로 월물이 몇개인지 11개인지 4개인지
	int m_nColCount{};		//2014.08.28 KSJ 종목파일이 바뀌기전에 문제가 생기므로 총갯수가 몇갠지 23개인지 9개인지
	int m_nOffSet{};			//2014.08.28 KSJ 종목파일이 바뀌기전에 문제가 생기므로 현재 위치를 설청할수있도록

	CMyBitmap	m_BitmapNormal;
	CMyBitmap	m_BitmapClick;
	std::unique_ptr <CTool> m_pTool;
	
	int		m_nInit{};		// 그려지는 위치 ..
	int		m_nCount{};		// 컬럼 수...
	int		m_nScroll{};		// 스크롤의 위치..
	
	CWnd*		m_pWnd{};

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
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

