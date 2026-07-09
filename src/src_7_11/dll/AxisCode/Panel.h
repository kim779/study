#pragma once


// CPanel

class CPanel : public CWnd
{
	DECLARE_DYNAMIC(CPanel)

public:
	CPanel();
	virtual ~CPanel();

	CPen* m_pPenup, *m_pPen;
	CFont* m_pFont;

	bool m_bleft;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


