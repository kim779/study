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
#include "usedefine.h"
class AFX_EXT_CLASS CSmain : public CDialogBar
{
// Construction
public:
	CSmain(bool (*callback)(int, WPARAM, LPARAM));

// Attributes
protected:
	int		m_height;
	int		m_vsN;
	int		m_vMode;

	CFont		m_font;
	CBitmap*	m_bmpBand;
	CBitmap*	m_bmpHome;

#ifdef DF_USESTL_SMAIN
	std::unique_ptr<class CXcom>m_xcom;
	std::unique_ptr<class CNButton>m_searchS;
	std::unique_ptr<class CNButton>m_smenu;
	std::unique_ptr<class CXcomex>m_xcomex;
	std::unique_ptr<class CHBand>m_band;
	std::unique_ptr<class CSVband>m_vband;
#else
	class CXcom* m_xcom;
	class CNButton* m_searchS;
	class CNButton* m_smenu;
	class CXcomex* m_xcomex;
	class CHBand* m_band;
	class CSVband* m_vband;
#endif
	
	

	//class CSDIBand*	m_band;
	
	
	class CTMenu* m_menu{};
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
	void	Refresh();
	void	set_List(const CStringArray& array);
	void	change_Group(int index);
	bool	IsInputFocus();
	void	SetHome();

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
	afx_msg	LONG OnXMSG(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMAIN_H__B60FA247_03E6_4A31_97EC_88C03AAC1D76__INCLUDED_)
