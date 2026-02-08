#if !defined(AFX_TINFO1_H__6253E2D1_27BB_4928_A560_61A556CFFC55__INCLUDED_)
#define AFX_TINFO1_H__6253E2D1_27BB_4928_A560_61A556CFFC55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TInfo1.h : header file
//
#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CTInfo1 window
#include "usedefine.h"

class AFX_EXT_CLASS CTInfo1 : public CDialogBar
{
// Construction
public:
	CTInfo1(bool (*callback)(int, WPARAM, LPARAM));

// Attributes
protected:
	int			m_height;
	BOOL		m_bBase;

	CBitmap*	m_bmpBackground;
	
#ifdef DF_USESTL_TINFO1
	std::unique_ptr<class CSimpleSplitter> m_base;
	std::unique_ptr<class CNButton> m_deal;
	std::unique_ptr<class CNButton>m_config;
	std::vector < std::shared_ptr<class CPane>> m_arPane;
#else
	class CSimpleSplitter*	m_base;
	class CNButton* m_deal;
	class CNButton* m_config;
	CObList		m_arPane;
#endif
	
	//** class CNButton*	m_updn;
	bool	(*m_axiscall)(int, WPARAM, LPARAM);

// Operations
public:
	void	drawBackground(CDC* pDC);
	void	DrawGripper(CDC* pDC);
	
	void	Config(int configN, COLORREF bkColor = RGB(255, 255, 255));
	void	ReConfig(int configN);
	void	ResizeBase();
	void	GetPaneSize(int configN, CArray <int, int> &ary);
	void	SetFmx(bool setting, int kind = 0, int key = 0, CString symbol = _T(""), CString dat = _T(""));
	void	SetEFmx(bool setting, int kind = 0, int key = 0, CString symbol = _T(""), CString dat = _T(""));
	void	SetInter(char* dat, bool bRotate = true);
	void	UpdatePaneData(int kind, CString dat);
	void	ProcessRTS(CString symbol, DWORD* dat);
	CRect	GetGRect();
	void	SettingEticker(int paneID);
	void	createButton();

	void InvalidatePane();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTInfo1)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetBase(BOOL bBase);
	CString GetCurrentSkinName();
	void change_Palette(CString skinN);
	virtual ~CTInfo1();
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	// Generated message map functions
protected:
	//{{AFX_MSG(CTInfo1)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg	LONG OnXMSG(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	int GetHeight();
	CBitmap* GetBackImage();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TINFO1_H__6253E2D1_27BB_4928_A560_61A556CFFC55__INCLUDED_)
