#if !defined(AFX_SMAIN_H__B60FA247_03E6_4A31_97EC_88C03AAC1D76__INCLUDED_)
#define AFX_SMAIN_H__B60FA247_03E6_4A31_97EC_88C03AAC1D76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Smain.h : header file
//
#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CSmain window

class AFX_EXT_CLASS CSmain : public CDialogBar
{
// Construction
public:
	CSmain(bool (*callback)(int, WPARAM, LPARAM), CString home, CString userN);

// Attributes
protected:
	CString		m_home;
	CString		m_userN;
	int		m_height;
	int		m_vsN;

	CFont		m_font;
	CBitmap		m_bkIMG;

	class CXcom*	m_xcom;
	class CNButton*	m_searchS;
	class CNButton*	m_smenu;
	class CXcomex*	m_xcomex;

	class CSDIBand*	m_band;
	class CTMenu*	m_menu;
	bool	(*m_axiscall)(int, WPARAM, LPARAM);

// Operations
public:
	void	make_Ctrl();
	void	drawBackground(CDC* pDC);
	void	drawGripper(CDC* pDC);
	void	drawEtc(CDC* pDC);
	void	drawSeperate(CDC* pDC, CRect rc);

	CRect	GetGRect();
	void	SetPosition();
	void	load_BandItem();
	bool	IsList(CString key);
	void	Change_Skin(CString skinName);
	void	set_MenuInfo(class CTMenu* menu)	{ m_menu = menu; }
	void	add_History(CString dat);
	void	change_VirtualScreen(int vsN);
	void	SetHome();
	void	Refresh();
	void	set_List(CStringArray& array);
	void	change_Group(int index);
	bool	IsInputFocus();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmain)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSmain();
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	// Generated message map functions
protected:
	//{{AFX_MSG(CSmain)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg	LRESULT OnXMSG(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMAIN_H__B60FA247_03E6_4A31_97EC_88C03AAC1D76__INCLUDED_)
