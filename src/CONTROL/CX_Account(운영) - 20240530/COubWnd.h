#pragma once


// COubWnd

class COubWnd : public CWnd
{
	DECLARE_DYNAMIC(COubWnd)

public:
	COubWnd();
	virtual ~COubWnd();


	CRect  m_recParent{};
	CRect  m_rect{};
	bool		m_bDrag{};
	bool     m_bShow{};
	CWnd* m_pWizard{};
	CWnd* m_pParent{};
	CFont* getAxFont(CString fName, int point, int style);

	void drawData(CDC* pDC);
	void drawCaption(CDC* pDC);
	CPen* getAxPen(COLORREF clr, int width, int style);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);



	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
};


