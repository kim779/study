#if !defined(AFX_LOCKPASS_H__9CE4F747_0A07_4C0C_BC44_7C498E78D792__INCLUDED_)
#define AFX_LOCKPASS_H__9CE4F747_0A07_4C0C_BC44_7C498E78D792__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LockPass.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLockPass dialog
#define WD_LOCKPASS			(WM_USER+0x8002)

#include "useMaindefine.h"
#include "ImgBtn.h"
class CLockPass : public CDialog
{
// Construction
public:
	CLockPass(CWnd* pParent = NULL);   // standard constructor

private:
	CWnd* m_pParent{};

#ifdef DF_USE_CPLUS17
	std::unique_ptr<CImgBtn> m_btn_cert{};
	std::unique_ptr<CImgBtn> m_btn_HTS_Close{};
#else
	CImgBtn* m_btn_cert{};
	CImgBtn* m_btn_HTS_Close{};
#endif

// Dialog Data
	//{{AFX_DATA(CLockPass)
	enum { IDD = IDD_LOCK_PASS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLockPass)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLockPass)
	virtual void OnOK();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnCert();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAxisClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOCKPASS_H__9CE4F747_0A07_4C0C_BC44_7C498E78D792__INCLUDED_)
