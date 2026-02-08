#if !defined(AFX_MATRIXSELECTERDLG_H__DF203060_CFBD_4938_AD4E_3DB4AB5EE664__INCLUDED_)
#define AFX_MATRIXSELECTERDLG_H__DF203060_CFBD_4938_AD4E_3DB4AB5EE664__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MatrixSelecterDlg.h : header file
//
#include "Resource.h"
#include "RscDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CMatrixSelecterDlg dialog

class CMatrixSelecterDlg : public CRscDlg
{
// Construction
public:
	CMatrixSelecterDlg(CWnd* pParent = NULL);   // standard constructor
	void SetMatrix(int nX, int nY) { m_nXOriginal = m_nX = nX; m_nYOriginal = m_nY = nY;}
	void SetBesideRect(CRect rect);
	void SetBesideWnd(CWnd *pWnd) { m_pBesideWnd = pWnd;}
	void ShowMatrix(BOOL bInit = TRUE);
	void SetpFrameDlg(CWnd *pWnd) { m_pWndFrameDlg = pWnd;}

// Dialog Data
	//{{AFX_DATA(CMatrixSelecterDlg)
	enum { IDD = IDD_MATRIXSELECTERDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
private:
	int m_nX;
	int m_nY;
	int m_nXOriginal;
	int m_nYOriginal;
	int m_nXSelected;
	int m_nYSelected;
	CRect m_rectAll;
	CRect m_rectOfBesideWnd;
	CRect m_rectSelectedOne;
	CPoint m_ptSelected;
	CWnd *m_pWndFrameDlg;
	CWnd *m_pBesideWnd;
	BOOL  m_bSetResized;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMatrixSelecterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMatrixSelecterDlg)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MATRIXSELECTERDLG_H__DF203060_CFBD_4938_AD4E_3DB4AB5EE664__INCLUDED_)
