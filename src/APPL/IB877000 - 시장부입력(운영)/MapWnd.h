#include "AxisExt.h"
#include "../../control/fx_misc/fxTab.h"

#include "SiHwang.h"
#include "Tujaja.h"

#pragma once

#define IDC_TAB			(1001)
#define IDC_SIHWANG		(1002)
#define IDC_TUJAJA		(1003)

class CMapWnd : public CWnd, public CAxisExt
{
public:
	CMapWnd(CWnd *pParent);

public:
	void LoadImage();
	void SetSize(int width, int height);
	void MakeBackground(int width=-1, int height=-1);
	void DrawRoundRectangle(CDC *pDC, CRect rc);
	void DrawBitmap(CDC *pDC, int x, int y, int w, int h, HBITMAP hBitmap, COLORREF tCol=0xFF00FF);
	void DrawBitmap(CDC *pDC, CRect *pRC, HBITMAP hBitmap, COLORREF tCol=0xFF00FF);
	void Draw(CDC *pdc);
	void SafeDelete(CWnd *pWnd);

protected:
	CString m_home;
	CBitmap *m_hRndBmp[4], *m_hGuide, *m_hCombo[2], *m_hBtn2[3], *m_hBtn4[3], *m_hSearch[3], m_bgBmp;
	CRect m_rcBody, m_rcGuide;

public:
	std::unique_ptr<CfxTab> m_pTabWnd;
	std::unique_ptr<CSiHwang> m_pSiHwangWnd;
	std::unique_ptr<CTujaja> m_pTujajaWnd;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	//}}AFX_VIRTUAL

public:
	virtual ~CMapWnd();

protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTabChange(NMHDR *pHeader, LRESULT *pResult);
};

