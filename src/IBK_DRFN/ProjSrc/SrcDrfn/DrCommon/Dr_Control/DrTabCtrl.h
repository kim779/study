#if !defined(AFX_DRTABCTRL_H__28407441_38AF_11D1_ABBA_00A0243D1382__INCLUDED_)
#define AFX_DRTABCTRL_H__28407441_38AF_11D1_ABBA_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TabCtrlEx.h : header file
//

/************************************************/
/*																		          */
/*							DR Tab Control							    */
/*                                              */
/*																		          */
/*							작성일 : 2006. 07 ~							*/
/*						작성자 : 우동우 (Doori co.)		    */
/*																		          */
/************************************************/

/////////////////////////////////////////////////////////////////////////////
// CDrTabCtrl window

class CDrTabCtrl : public CTabCtrl
{
public:
	CDrTabCtrl();
	virtual ~CDrTabCtrl();

public:
	int m_lfEscapement;

private:
  int m_nTabPos;
  CFont m_fontDefault, m_fontBold;
    
	CPen m_penUnSelectLine, m_penSelectLine, m_penOutLine, m_penOutLine_In, m_penSelectOutLine_In;
 
protected:
  COLORREF m_clrBK;
  COLORREF m_clrBoard, m_crlSel, m_crlUnSel, m_crlSelGraDown, m_crlUnSelGraDown;
  COLORREF m_clrOutLine;

  COLORREF m_crlSelText, m_crlUnSelText;

public:
  virtual BOOL Create(int nTabPos, CWnd* pParentWnd, UINT nID);

  void SetBKColor(COLORREF crlBK);
  void SetTabColor(COLORREF crlBoard, COLORREF crlSel, COLORREF crlUnSel, COLORREF crlSelGraDown = RGB(0,0,0), COLORREF crlUnSelGraDown = RGB(0,0,0), COLORREF crlBody = COLOR_MAIN_CANVAS);
  void SetTextColor(COLORREF crlSel, COLORREF crlUnSel);
 
  void SetFonts(int nSelWeight, int nUnselWeight);

  void SetColorType(int nType);

private:
  void OnPaint_DefType(CPaintDC *pDC, CRect rePaint);
  void OnPaint_LeftType(CPaintDC *pDC, CRect rePaint);

//20080904 김성조 //신영 추가(더블버퍼링으로 변경하면서)
  void OnPaint_DefType(CDC *pDC, CRect rePaint);
  void OnPaint_LeftType(CDC *pDC, CRect rePaint);
       

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrTabCtrl)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	
// Generated message map functions
protected:
	//{{AFX_MSG(CDrTabCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRTABCTRL_H__28407441_38AF_11D1_ABBA_00A0243D1382__INCLUDED_)
