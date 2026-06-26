#pragma once


// CXedit

class CXedit : public CEdit
{
	DECLARE_DYNAMIC(CXedit)

public:
	CXedit();
	virtual ~CXedit();

	void	setaxiscall(bool (*callback)(int, WPARAM, LPARAM));

	COLORREF	m_textColor{};
	COLORREF	m_bkColor{};
	CBrush		m_brush{};
	bool		m_bSelect{};
	std::unique_ptr<CToolTipCtrl> m_ToolTip;
	bool(*m_sticker_call)(int, WPARAM, LPARAM);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnEnChange();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


