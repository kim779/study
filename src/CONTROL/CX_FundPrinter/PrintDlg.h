#if !defined(AFX_PRINTDLG_H__1618E5E9_3760_4582_B594_8FB625913328__INCLUDED_)
#define AFX_PRINTDLG_H__1618E5E9_3760_4582_B594_8FB625913328__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrintDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrintDlg dialog

#define WM_MANAGE	(WM_USER + 1000)

#define IDC_PRINT_BTN		1000

#define MK_SETDATA			3001

#define		P_DELI					0x7f
#define		P_TAB					0x09
#define		P_NEW					0x0a
#define		P_PER					0x25

#include "CX_FundPrinter.h"

class CPrintDlg : public CDialog
{
// Construction
public:
	CPrintDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL m_flag;
	int m_iXP,m_iYP,m_iXD,m_iYD;
	double m_dpiRatio;
	HBITMAP			m_bitmap_nm, m_bitmap_dn, m_bitmap_hv;
	int			m_tokenSize;
	BOOL		m_printFlag;
	CString		m_printData[23];

	//void SetData(CFundData data);
	void StartPrint();
	void SetData(LPCTSTR data);

// Dialog Data
	//{{AFX_DATA(CPrintDlg)
	enum { IDD = IDD_DIALOG2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	private:
	void DrawStuff(CDC* pDC, double dpiRatio);
	void DrawTitle(CDC* pDC, double dpiRatio);
	void DrawLine(CDC* pDC,POINT s,POINT e);
	void DrawBox(CDC* pDC,int x1,int y1,int x2,int y2,LPCTSTR text,BOOL MULTI,int nBold);
	void SetPrintAlign(CDC *pDC, HDC hdcPrn);
	void GetTokenSplit(CString sText, TCHAR sSplit);
	CString			parseX(CString &srcstr, CString substr);
	CString			AddComma(CString str,TCHAR ch);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintDlg)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush m_brush;
	// Generated message map functions
	//{{AFX_MSG(CPrintDlg)
	afx_msg void OnPrintButton();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CPrintDlg)
	afx_msg void SetProperties(LPCTSTR str);
	afx_msg BSTR GetProperties();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTDLG_H__1618E5E9_3760_4582_B594_8FB625913328__INCLUDED_)
