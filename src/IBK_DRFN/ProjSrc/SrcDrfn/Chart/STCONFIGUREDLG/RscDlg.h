#if !defined(AFX_RSCDLG_H__6000F270_B94B_4D29_BF0C_B65C7348330A__INCLUDED_)
#define AFX_RSCDLG_H__6000F270_B94B_4D29_BF0C_B65C7348330A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RscDlg.h : header file
//

#include "./ConfigHelper.h"		// 컨트롤초기셋팅에 관련된 처리.

/////////////////////////////////////////////////////////////////////////////
// CRscDlg dialog

class CRscDlg : public CDialog
{
// Construction
public:
	CRscDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL ); // standard constructor

	CString m_szCFGFile;	// ST파일위치 저장.
	CString m_szDefCFGFile;		// 디폴트세팅값을 저장하는 파일
	void SetSTInfo(LPCSTR szSTFile) { m_szCFGFile = szSTFile;	}
	void SetSTDefInfo(LPCSTR szSTFile)	{ m_szDefCFGFile = szSTFile;	}

// Dialog Data
	//{{AFX_DATA(CRscDlg)
	enum { IDD = NULL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRscDlg)
	public:
	virtual BOOL Create(UINT nID,CWnd* pParentWnd);
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void LoadInfo()		{}
	virtual BOOL SaveInfo()		{return FALSE;}
	virtual void RemoveInfo()	{}
	virtual void SetDefault()	{}

protected:

	// Generated message map functions
	//{{AFX_MSG(CRscDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
inline BOOL CRscDlg::PreTranslateMessage(MSG* pMsg) 
{
	// 엔터가 쳤을때 죽는 현상 방지		by KHD
	
	if(pMsg->message == WM_KEYDOWN)   
	{
		if(pMsg->wParam == VK_F4 && (::GetKeyState(VK_CONTROL) & 0x8000))
		{
			
			pMsg->wParam = VK_ESCAPE;
			return GetParent()->PreTranslateMessage(pMsg);
		}
		if(pMsg->wParam == VK_ESCAPE)
		{
			return GetParent()->PreTranslateMessage(pMsg);
		}
		if(pMsg->wParam == VK_RETURN)
			return GetParent()->PreTranslateMessage(pMsg);

	}

	return CDialog::PreTranslateMessage(pMsg);
}
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RSCDLG_H__6000F270_B94B_4D29_BF0C_B65C7348330A__INCLUDED_)
