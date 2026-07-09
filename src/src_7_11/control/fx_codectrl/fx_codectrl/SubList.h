#pragma once


// CSubList
class CControlwnd;
class CSubList : public CListBox
{
	DECLARE_DYNAMIC(CSubList)

public:
	CSubList(class CControlwnd *pCodeCtrl, class CCodeCombo *pCodeCombo);
	CSubList();
	virtual ~CSubList();

	CControlwnd	*m_pCodeCtrl;
	class CCodeCombo	*m_pCodeCombo;

	CFont			*m_pFont;
	COLORREF		m_tRGB, m_pRGB, m_bRGB;
	int			m_height;
	CPen			m_pen;

	void SetListItems(CFont *pFont, int height);
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


