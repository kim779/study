#pragma once
#include <afxwin.h>

#define	WM_EDITX	(WM_USER + 100)

#define	wpBLANK		1
#define	wpENTER		2
#define	wpSEARCH1	3
#define	wpSEARCH2	4
#define	wpUP		5
#define	wpDOWN		6

class CEditSearch :
    public CEdit
{
public:
	CEditSearch();
	bool	m_bKeyDown;
	CRect m_rectClear{};
	CBitmap* m_pClearBmp{};
	CString m_path{};
	void DrawBitmap(CDC* pDC, CRect rect);
	void Init();
	CString m_strimagepath;
	CWnd* m_pParent{};
	virtual ~CEditSearch();
	virtual void  OnDraw(CDC* pDC);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//afx_msg void OnPaint();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};

