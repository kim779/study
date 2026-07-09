#pragma once


// CGpEdit

class CGpEdit : public CEdit
{
	DECLARE_DYNAMIC(CGpEdit)

public:
	CGpEdit();
	virtual ~CGpEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};


