#pragma once

class CAxEdit : public CEdit
{
public:
	CAxEdit();

public:
	bool	m_bKeyDown;

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxEdit)
	//}}AFX_VIRTUAL

public:
	virtual ~CAxEdit();

protected:
	//{{AFX_MSG(CAxEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

