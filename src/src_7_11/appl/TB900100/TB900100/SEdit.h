#pragma once


// SEdit

class SEdit : public CEdit
{
	DECLARE_DYNAMIC(SEdit)

public:
	SEdit();
	virtual ~SEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};


