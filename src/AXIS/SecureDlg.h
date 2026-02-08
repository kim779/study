#if !defined(AFX_SECUREDLG_H__C004953A_63EE_4DF9_A824_8A1255128F8F__INCLUDED_)
#define AFX_SECUREDLG_H__C004953A_63EE_4DF9_A824_8A1255128F8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SecureDlg.h : header file
//

#include "ImgBtn.h"

//#define LOG_MODE
#include "useMaindefine.h"
class CSecureDlg : public CDialog
{
// Construction
public:
	CSecureDlg(CWnd* pParent, BOOL bAOS, BOOL bFirewall, BOOL bKeysecure);   // standard constructor
	virtual ~CSecureDlg() { 
#ifdef LOG_MODE
		if (m_hDebug!=INVALID_HANDLE_VALUE) CloseHandle(m_hDebug);
#endif
	}
// Dialog Data
	//{{AFX_DATA(CSecureDlg)
	enum { IDD = IDD_SECURE_DLG };
	BOOL	m_bNoRetry;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSecureDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bFirewall, m_bKeysecure, m_bAOS;
	CFont m_font, m_bfont;
	CBitmap* m_bmp_excl{}, * m_bmp_arrow{};
#ifdef DF_USE_CPLUS17
	std::unique_ptr<CImgBtn>m_btn_yes;
	std::unique_ptr<CImgBtn>m_btn_no;
#else
	CImgBtn *m_btn_yes, *m_btn_no;
#endif

#ifdef LOG_MODE
	HANDLE m_hDebug;
#endif
	void LogWrite(LPCSTR txt, int lineno)
	{
#ifdef LOG_MODE
		if (m_hDebug!=INVALID_HANDLE_VALUE)
		{
			DWORD dwWritten;
			CString logmsg;
			logmsg.Format("[%04d] %s\r\n", lineno, txt);
			WriteFile(m_hDebug, (LPCSTR)logmsg, logmsg.GetLength(), &dwWritten, NULL);
		}
#endif
	}

	void DrawBitmap(CDC *pDC, CBitmap *bmp, int x, int y, UINT rgb);
	// Generated message map functions
	//{{AFX_MSG(CSecureDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg void OnOkClick();
	afx_msg void OnCancelClick();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SECUREDLG_H__C004953A_63EE_4DF9_A824_8A1255128F8F__INCLUDED_)
