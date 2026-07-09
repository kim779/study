#pragma once


// CHanEdit

class CHanEdit : public CWnd
{
	DECLARE_DYNAMIC(CHanEdit)

public:
	CHanEdit();
	virtual ~CHanEdit();
	DWORD	m_Conversion, m_Sentence;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};


