#if !defined(AFX_GRPGRID_H__A535CF8F_08A6_4D55_A254_B5BD66DA7D9F__INCLUDED_)
#define AFX_GRPGRID_H__A535CF8F_08A6_4D55_A254_B5BD66DA7D9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GrpGrid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGrpGrid window

class CGrpGrid : public CWnd
{
public:
	CGrpGrid();
	virtual ~CGrpGrid();

public:

private:
	bool	m_bFirst;
	CAxisGMainApp*	m_pApp;
	CWnd*		m_pwndView;
	CWnd*		m_pwndPnChart;
	class CObjMgr*	m_pObjMgr;

	int	m_iLineCnt;
//	int	m_iMaxHeadSize;
//	int	m_iMaxDataSize;

	CFont*		m_pfont;	// normal
	CFont*		m_pfontB;	// Bold

	CPen*	m_ppenLine;
	CBrush*	m_pbrushBG;
	CBrush*	m_pbrushFG;
	CBrush*	m_pbrushScr;
	CBrush*	m_pbrushTime;
	CBrush*	m_pbrushSpec;

	
	COLORREF	m_textRGB;		// text color
	COLORREF	m_backRGB;		// background color
	COLORREF	m_frameRGB;		// frameborder border
	COLORREF	m_lineRGB;		// line color
	COLORREF	m_scrRGB;		// horizental scroll value color
	COLORREF	m_timeRGB;		// time value bgcolor;
	COLORREF	m_specRGB;		// special value bgcolor;

	
	CString		m_strData;
	CRect		m_rectDraw;

	CArray<char, char>		m_arKind;
	CArray<CString, CString>	m_arHead;
	CArray<CString, CString>	m_arData;

	enum	enumPos { eLeft, eRight }	m_ePos;


public:
	BOOL	Create(CWnd* pwndView, CWnd* pwndPnChart, class CObjMgr* pObjMgr);
	void	ShowData(CString strData, CPoint* pPoint);
	void	MoveGrid(CPoint pt, CString data = _T(""));
//	void	SetData(CString strData);

private:
	void	Init();
	int	ParseData(CString strData);
	void	DrawGrid(CDC *pDC);
//	CSize	CalStrSize(CString strExam);

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrpGrid)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CGrpGrid)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRPGRID_H__A535CF8F_08A6_4D55_A254_B5BD66DA7D9F__INCLUDED_)
