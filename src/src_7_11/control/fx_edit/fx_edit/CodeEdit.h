#pragma once
//#include "Crypt.h"

// CCodeEdit

class CCodeEdit : public CEdit
{
	DECLARE_DYNAMIC(CCodeEdit)

public:
	CCodeEdit(class CControlwnd *pwnd);
	virtual ~CCodeEdit();

	void ShellexcuteLogin();
	void JustLogin();
	bool CheckMessangerLogin();

	CString m_sUserName, m_sName, m_pass;
	CString m_sUserID, m_sID;
//	Crypt			m_crypt;
	int	 m_imsgcnt;
	bool m_bServerError;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


