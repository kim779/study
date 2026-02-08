// DrSplitterCtrl.h: interface for the CDrSplitterCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRSPLITTERCTRL_H__2ECA2173_8A9C_4320_8B06_B804C89C1290__INCLUDED_)
#define AFX_DRSPLITTERCTRL_H__2ECA2173_8A9C_4320_8B06_B804C89C1290__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDrSplitterCtrl : public CSplitterWnd
{
public:
  CDrSplitterCtrl();
  
public:
  int	 m_nResizeRow;
  bool m_bFirstSizing;
  
  int m_upBorder;
  bool m_bWhiteLine;

  COLORREF m_clrSplitBar;

public:
  void SetShow_Col(BOOL bHide, int nCol);
  void SetShow_Row(BOOL bHide, int nRow);

  void SetSplitBarCx(int nCx) 
  { 
		m_cxSplitter  = nCx; 
		m_cxSplitterGap = nCx;
  }

  void SetColorSplitBar(COLORREF clr)
  {
		m_clrSplitBar = clr;
  }	

private:
  void SetHide_Row(int nRow);
  void SetShow_Row(int nRow);

  void SetShow_Col(int nCol);
  void SetHide_Col(int nCol);

  virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);
    
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDrSplitterCtrl)
  //}}AFX_VIRTUAL
  
  // Generated message map functions
protected:
  //{{AFX_MSG(CDrSplitterCtrl)
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_DRSPLITTERCTRL_H__2ECA2173_8A9C_4320_8B06_B804C89C1290__INCLUDED_)
