#if !defined(AFX_INQUIRYDLG_H__C76BA81F_2B4A_4812_8D27_31D38E3BD78E__INCLUDED_)
#define AFX_INQUIRYDLG_H__C76BA81F_2B4A_4812_8D27_31D38E3BD78E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InquiryDlg.h : header file
//

#include "InquiryDataListCtrl.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CInquiryDlg dialog

class CInquiryDlg : public CDialog
{
// Construction
public:
	CInquiryDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInquiryDlg();

protected:
	CWnd*			m_pParent;
	CBrush			m_dlgBrush;
	CFont			m_pFont;

	COLORREF		m_clrInquiryDlg;			// 수치조회창 색상.
	int				m_nListDataSize;

	CStringArray*	m_saListData;

	BOOL			m_bDataDispFlag;

private:
	bool			m_bCalculateSize;

public:
	void	Refresh( void);
	void	SetInquiryData(CDC* pDC, CStringArray *pInquiryData, CStringArray *pInquiryPrevData,int nMaxDecimal = 0);
	void	MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);
	int		SetColumnWidth(int nColIndex, CDC* pDC);
	LPCTSTR	GetMaxColumnString( int nColIndex);

	void	SetDlgBrush(COLORREF clrDlgBrush);

	void	StartDisplayData(CStringArray* saListData);
	void	EndDisplayData();

	void	SetShowHide(int nType);
	BOOL	GetShowHide();

	void	CreateFontIndirect();


// Dialog Data
	//{{AFX_DATA(CInquiryDlg)
	enum { IDD = IDD_INQUIRYDATAWINDOW };
	CInquiryDataListCtrl	m_listctrlInquiryData;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInquiryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInquiryDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnGetdispinfoListInquirydata(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INQUIRYDLG_H__C76BA81F_2B4A_4812_8D27_31D38E3BD78E__INCLUDED_)
