// IBK_CaptureDlg.h : header file
//

#if !defined(AFX_IBK_CAPTUREDLG_H__01FF8826_A772_4CEF_948A_86CAF51EF79A__INCLUDED_)
#define AFX_IBK_CAPTUREDLG_H__01FF8826_A772_4CEF_948A_86CAF51EF79A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CIBK_CaptureDlg dialog

class CIBK_CaptureDlg : public CDialog
{
// Construction
public:
	CIBK_CaptureDlg(CWnd* pParent = NULL);	// standard constructor
	BOOL writeDIB(LPTSTR szFile, HANDLE hDIB);
	HANDLE writeDDBToDIB(CBitmap& bitmap, DWORD dwCompression, CPalette* pPal) ;
	CString IsWow64();
	void StartCapture();
	time_t ConvertSecondTime(int parm_year, int parm_month, int parm_day,int parm_hour, int parm_minute, int parm_second);
	BOOL	IsToDayHoliDay();
	void	BmpTojpg(CString sfile);
	int		GetEncoderClsid(const char* format, CLSID* pClsid);
// Dialog Data
	//{{AFX_DATA(CIBK_CaptureDlg)
	enum { IDD = IDD_IBK_CAPTURE_DIALOG };
	CComboBox	m_cbTime;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIBK_CaptureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CIBK_CaptureDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnStart();
	afx_msg void OnBtnFind();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnStop();
	afx_msg void OnBtnJpeg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IBK_CAPTUREDLG_H__01FF8826_A772_4CEF_948A_86CAF51EF79A__INCLUDED_)
