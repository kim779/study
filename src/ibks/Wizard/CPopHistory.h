#pragma once


// CPopHistory

class CPopHistory : public CListBox
{
	DECLARE_DYNAMIC(CPopHistory)

public:
	CPopHistory(class CClient* client, CString items);
	virtual ~CPopHistory();

protected:
	CFont		m_font;
	CString m_slog{};
private:
	class CGuard* m_guard;
	class CClient* m_client;

	CString		m_items;

	COLORREF	m_tRGB;
	COLORREF	m_pRGB;
	int		m_height;	// cell height

public:
	bool	GetItem(CString& item);
	void	FindItem(CString item, bool self);
	bool	OnKey(WPARAM wParam);
	void	OnWheel(int delta);


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


