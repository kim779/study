#if !defined(AFX_ESC_SUPPORT_DIALOG_469D_45B4_8BEB_6DC808FE3CA7__INCLUDED_)
#define AFX_ESC_SUPPORT_DIALOG_469D_45B4_8BEB_6DC808FE3CA7__INCLUDED_

// #include "../Common_ST/EscDialog.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EscDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEscDialog dialog

class CEscDialog : public CDialog
{
// Construction
public:
	CEscDialog(int nIDD, CWnd* pParent = NULL);   // standard constructor
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEscDialog)
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:
};

inline CEscDialog::CEscDialog(int nIDD, CWnd* pParent)
	: CDialog(nIDD, pParent)
{
	//{{AFX_DATA_INIT(CEscDialog)
	//}}AFX_DATA_INIT
}

inline BOOL CEscDialog::PreTranslateMessage(MSG* pMsg) 
{
	// 엔터가 쳤을때 죽는 현상 방지		JSJ_Modify_040421
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
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ESC_SUPPORT_DIALOG_469D_45B4_8BEB_6DC808FE3CA7__INCLUDED_)
