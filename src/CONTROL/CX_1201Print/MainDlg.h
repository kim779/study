#pragma once
// MainDlg.h : header file
//
#include "CX_1201Print.h"

/////////////////////////////////////////////////////////////////////////////
// MainDlg dialog

#define HEADERBKCL	RGB(227,231,234)
#define WHITETEXT	RGB(255,255,255)
#define BLACKTEXT	RGB(0,0,0)

class MainDlg : public CDialog
{
// Construction
public:
	MainDlg(CWnd* pParent = NULL);   // standard constructor
	void Run();

// Dialog Data
	//{{AFX_DATA(MainDlg)
	enum { IDD = IDD_MAINDLG };
	CEdit	m_editStatus;
	CButton	m_btnOK;
	CProgressCtrl	m_Progress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MainDlg)
public:
	virtual void OnFinalRelease();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	CString m_dataFile;
	CStringArray m_arData;
	int	m_nTotalPages;
	int	m_nNowPage;

	void	SetProgressRange(int iMax, int iMin) ;
	void	SetProgressPos(int iPos);
	void	Start();
	void	DrawBox(CDC* pDC,int x1,int y1,int x2,int y2,LPCTSTR text,BOOL MULTI,int nBold,BOOL BKMODE,int nAlign);
	void	DrawLine(CDC* pDC,POINT sPoint,POINT ePoint);
	void	DrawStuff(CDC* pDC);
	void	SetPrintAlign(CDC *pDC, HDC hdcPrn);
	void	LoadData();
	void	SplitString(CString &strData, CString strToken,CStringArray &ar);
	void	DrawTopBox(CDC* pDC);
	void	FillSolidBox(CDC* pDC,int x,int y,int w,int h,int type);
	void	DrawGrid(CDC* pDC);
	void	DrawGridData(CDC* pDC);
	void	DrawGridHeader(CDC* pDC, bool bFirst = true);
	void	DrawGridLine(CDC* pDC, const CStringArray& ar,int y1);
	void	DrawBottomData(CDC* pDC,int y);
	void	DrawPageNumber(CDC* pDC);

protected:
	// Generated message map functions
	//{{AFX_MSG(MainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeEdit1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(MainDlg)
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};