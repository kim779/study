#if !defined(AFX_QUICKMAKEDLG_H__F7992499_656C_49CC_A694_79EED52469FD__INCLUDED_)
#define AFX_QUICKMAKEDLG_H__F7992499_656C_49CC_A694_79EED52469FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QuickMakeDlg.h : header file
//
#include "resource.h"
#include "../../DrCommon/Dr_Control/DrBtnCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CQuickMakeDlg dialog
class CGrid_Comm;
class CQuickMakeDlg : public CDialog
{
// Construction
public:
	CQuickMakeDlg(CWnd* pParent = NULL);   // standard constructor

private:
  CFont m_fontDefault;
  CBrush				m_brushBk;

  CWnd *m_pwndParent;

public:
	CGrid_Comm		*m_pgridSource;
	CGrid_Comm		*m_pgridDest;
	
	CDrBtnCtrl		m_btnLeft;
	CDrBtnCtrl		m_btnLeftAll;
	CDrBtnCtrl		m_btnRight;
	CDrBtnCtrl		m_btnRightAll;

	CStatic			m_ctrlPosBar;
	CDrBtnCtrl		m_btnUp;
	CDrBtnCtrl		m_btnDown;
	CDrBtnCtrl		m_btnTop;
	CDrBtnCtrl		m_btnBottom;

	CDrBtnCtrl		m_btnOk;
	CDrBtnCtrl		m_btnCancel;

private:
	void OnSize();
	void OnSizeTopBar(CRect reClient);
	void OnSizeMoveBar(CRect reClient);
	void OnSizePosBar(CRect reClient);

	void SetAdd_Source(CList <XMLTreeInfoFolder, XMLTreeInfoFolder&> *plistInfoFolder);
	void SetAdd_Dest(CList <QuickList, QuickList> *plistQuickInfo);
	
	void GetInfoFileToQuick(XMLTreeInfoFile stXMLTreeInfoFile, QuickList &stQuickList);

	void SetDelete_Dest(int nSelectRow);
	void SetInsert_Dest(int nSelectRow, BOOL bErrView = TRUE);

	void SetUpdateInfoFolder(CList	<XMLTreeInfoFolder, XMLTreeInfoFolder&> *plistInfoFolder);

// Dialog Data
	//{{AFX_DATA(CQuickMakeDlg)
	enum { IDD = IDD_QUICKMAKE_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQuickMakeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CQuickMakeDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG

	afx_msg void OnBtnOk();
	afx_msg void OnBtnCancel();

	afx_msg void OnBtnLeft();
	afx_msg void OnBtnLeftAll();
	afx_msg void OnBtnRight();
	afx_msg void OnBtnRightAll();

	afx_msg void OnBtnUp();
	afx_msg void OnBtnTop();
	afx_msg void OnBtnDown();
	afx_msg void OnBtnBottom();

	//afx_msg void OnNotifyGrid(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotifyGrid(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUICKMAKEDLG_H__F7992499_656C_49CC_A694_79EED52469FD__INCLUDED_)
