#pragma once


// CMPropGridCombo

class CMPropGridCombo : public CWnd
{
	DECLARE_DYNAMIC(CMPropGridCombo)

public:
	CMPropGridCombo();
	virtual ~CMPropGridCombo();

	CComboBox* m_inCombo;

protected:
	CFont*		m_font;
	DWORD		m_state;
	COLORREF	m_color;
	COLORREF	m_txtcolor;


	DECLARE_MESSAGE_MAP()

public:
	void	SetComboWidth();
	void	SetItemData(int idx, DWORD data);
	
	CString GetText();
	int	AddString(CString str);

	void	SetFont(CFont* font);
	void	SetEnable(BOOL bEnable = TRUE);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSelOk();
};


