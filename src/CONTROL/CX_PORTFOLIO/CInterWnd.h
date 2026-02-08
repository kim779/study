#pragma once


// CInterWnd

class CInterWnd : public CWnd
{
	DECLARE_DYNAMIC(CInterWnd)

public:
	CInterWnd(char* pStrRoot, char* pStrUser);
	virtual ~CInterWnd();
	
	CString m_strRoot, m_strUser;
	CWnd* m_pMainFrame{};
protected:
	afx_msg LRESULT OnMsgFromMain(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


