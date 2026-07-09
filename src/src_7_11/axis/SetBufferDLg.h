#if !defined(AFX_SETBUFFERDLG_H__17CDE9C4_F50B_4477_9214_336504CBF513__INCLUDED_)
#define AFX_SETBUFFERDLG_H__17CDE9C4_F50B_4477_9214_336504CBF513__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetBufferDLg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetBufferDlg dialog

class CSetBufferDlg : public CDialog
{
// Construction
public:
	CSetBufferDlg(CString strHome, int iIdle, int iGridDrawing, CWnd* pParent = NULL);   // standard constructor

public:
	int	m_iIdle;
	int	m_iGridDrawing;

private:
	CString	m_strPath;

//	int	m_iIdleDefault;
//	int	m_iGridDrawingDefault;


public:

// Dialog Data
	//{{AFX_DATA(CSetBufferDlg)
	enum { IDD = IDD_DIALOG_BUFFERING };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetBufferDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetBufferDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETBUFFERDLG_H__17CDE9C4_F50B_4477_9214_336504CBF513__INCLUDED_)
