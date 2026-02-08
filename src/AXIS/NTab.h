#if !defined(AFX_NTAB_H__E85BE553_E4A8_47EF_80EF_15C6211AF59D__INCLUDED_)
#define AFX_NTAB_H__E85BE553_E4A8_47EF_80EF_15C6211AF59D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNTab window
#include "useMaindefine.h"
class CNTab : public CWnd
{
// Construction
public:
	CNTab(CString key, class CTMenu* menu, 
		CString aMapN, bool (*axiscall)(int, WPARAM, LPARAM), int winKey);
	virtual ~CNTab();
	void SetInfo(int key) { m_key.Format("%03d", key); }  //vc2019

// Attributes
protected:
	int		m_index;
	int		m_activeN;
	int		m_sType;
	int		m_winKey;

	bool		m_expand;

	CString		m_key;
	CString		m_mapN;
	class CTMenu*	m_menu;

#ifdef DF_USE_CPLUS17
	std::unique_ptr<class CNExpand> m_exArr;
#else
	class CNExpand*	m_exArr;
#endif

	CStringArray	m_list;
	bool	(*m_axiscall)(int, WPARAM, LPARAM);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNTab)
	//}}AFX_VIRTUAL

// Implementation
public:
	void	ChangeSkin();
	void	ChangeTab(CString mapN);
	bool	IsVisible(int index);
	void	DrawEdge(CDC* pDC);
	// Generated message map functions
protected:
	void	Draw(CDC* pDC = NULL);
	void	DrawTabImg(CDC* pDC);
	void	drawImg(CDC* pDC, int index, CRect iRc, bool stretch = false);
	void	loadlist(CString aMapN);
	int	GetSelect(CPoint point);
	CString	GetTabString(CString mapName);
	//{{AFX_MSG(CNTab)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnExpand(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
private:
	CResourceHelper* m_resourceHelper;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NTAB_H__E85BE553_E4A8_47EF_80EF_15C6211AF59D__INCLUDED_)
