#if !defined(AFX_TOOLANDRBUTTONPAGE_H__3E6BCC96_749B_4348_9D8C_477DE7391528__INCLUDED_)
#define AFX_TOOLANDRBUTTONPAGE_H__3E6BCC96_749B_4348_9D8C_477DE7391528__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolandRButtonPage.h : header file
//

#include "ToolandRButtonTabCtrl.h"
#include "ToolandRButtonBase_ListCtrl.h"
#include "DlgToolTip.h"							// for CDlgToolTip

/////////////////////////////////////////////////////////////////////////////
// CToolandRButtonPage dialog

interface IPropertyMap;
class CToolandRButtonPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CToolandRButtonPage)

// Construction
public:
	CToolandRButtonPage();
	~CToolandRButtonPage();

	//부모로부터 설정되어 있던 저장값을 받아서 Setting 한다.
	void SetProperty( IPropertyMap *p_pIPropertyMap);

// Dialog Data
	//{{AFX_DATA(CToolandRButtonPage)
	enum { IDD = IDD_PROPPAGE_TOOL_RBUTTOL_MENU };
	CToolandRButtonTabCtrl	m_ToolRButton_Tab;
	CToolandRButtonBase_ListCtrl	m_ToolRButtonItem_Ctrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CToolandRButtonPage)
	public:
	virtual BOOL OnApply();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CToolandRButtonPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	//Create tooltip object and Add text to StringArray 
	CDlgToolTip   m_toolTip;
	void CreateToolTip();
	
	//base 리스트 컨트롤 초기화.
	void InitBaseList(CListCtrl* pListCtrl);

// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
protected:
	IPropertyMap *	m_pIPropertyMap;

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
public:
	void	SetOcxHwnd( HWND p_hOcxWnd);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLANDRBUTTONPAGE_H__3E6BCC96_749B_4348_9D8C_477DE7391528__INCLUDED_)
