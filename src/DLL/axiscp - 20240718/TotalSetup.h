#if !defined(AFX_TOTALSETUP_H__FF43E3B1_A022_4E38_8D77_DF304A67AA43__INCLUDED_)
#define AFX_TOTALSETUP_H__FF43E3B1_A022_4E38_8D77_DF304A67AA43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TotalSetup.h : header file
//
#include "resource.h"
#include <afxtempl.h>
#include "../ctrl/fxTreeCtrl.h"
#include "SetupDialog.h"
#define MAX_SETUP	8
#include "../../axis/BmpButton.h"
/////////////////////////////////////////////////////////////////////////////
// CTotalSetup dialog

class AFX_EXT_CLASS CTotalSetup : public CDialog
{
// Construction
public:
	CTotalSetup(class CTMenu* menu, int index = 0, int tick = 0, CWnd* pParent = NULL);   // standard constructor

	BOOL		m_bTickView[2]{};
	BOOL		m_bScreenBar{};

	CWnd* m_pMain;
protected:
	int		m_index;
	int		m_tick;

	CImageList	m_imageList;
		

	class CTMenu*		m_menu;
	
	CSetupDialog* CTotalSetup::getanyFind(std::any& item);
	//CArray<CSetupDialog*, CSetupDialog*> m_dlgs;
	std::vector<std::any> m_dlgs;
	CMap<UINT, UINT&, bool, bool> m_applys;
public:
// Dialog Data
	//{{AFX_DATA(CTotalSetup)
	enum { IDD = IDD_TOTALSETUP };
	CfxTreeCtrl	m_tree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTotalSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTotalSetup)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedTreemenu(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDefault();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnApply();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);  //vc2019 UINT->LRESULT
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void DestroyAllDialogs();
	
	//CControlChanger* m_changer{};
	std::unique_ptr<CControlChanger>m_changer;
	//CBitmap* m_bmpcap;
	//CBitmap* m_bmplogo;
	//class CBmpButton* m_bitmapBtn{};
	std::unique_ptr<CBmpButton> m_bitmapBtn;

	COLORREF m_clrFrame{};
	
public:
	CFont m_font;
	
	
	//
	void DrawFrame(CDC* pDC);
	bool IsApplied(UINT id);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOTALSETUP_H__FF43E3B1_A022_4E38_8D77_DF304A67AA43__INCLUDED_)
