#if !defined(AFX_REGKEYWORDDLG_H__C2CB57E8_7C27_4069_B4AC_7AEC4FD71BBA__INCLUDED_)
#define AFX_REGKEYWORDDLG_H__C2CB57E8_7C27_4069_B4AC_7AEC4FD71BBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CRegKeywordDlg.h : header file
//

#include "resource.h"
#include "../../DrCommon/Dr_Control/DrBtnCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CRegKeywordDlg dialog

class CRegKeywordDlg : public CDialog
{
private :
  CFont m_fontDefault;
  CBrush  m_brushStatic;

	void SetInit();

public:
	CRegKeywordDlg(CWnd* pParent, CList <KeywordData, KeywordData&>	*plistUserData);   // standard constructor
	CList <KeywordData, KeywordData&>	*m_plistUserData;

	CStringArray	m_saSetData;

	CButton		m_btnGroup;
	CStatic		m_ctrlS;
	CEdit		m_editS;
	CStatic		m_ctrlV;
	CEdit		m_editV;
	CStatic		m_ctrlPoint;
	CComboBox	m_comboPoint;

	CDrBtnCtrl		m_btnOk;
	CDrBtnCtrl		m_btnCancel;

// Dialog Data
	//{{AFX_DATA(CRegKeywordDlg)
	enum { IDD = IDD_DLG_RESKEYWORD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegKeywordDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegKeywordDlg)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
  afx_msg void OnDestroy();
	//}}AFX_MSG

	afx_msg void OnBtnOk();
	afx_msg void OnBtnCancel();

	afx_msg void OnChangeRegPoint();

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGKEYWORDDLG_H__C2CB57E8_7C27_4069_B4AC_7AEC4FD71BBA__INCLUDED_)
