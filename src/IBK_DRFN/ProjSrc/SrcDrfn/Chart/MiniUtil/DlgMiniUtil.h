#if !defined(AFX_DLGMINIUTIL_H__B7C854BD_1BBC_43B5_B2DD_BE4EC959D957__INCLUDED_)
#define AFX_DLGMINIUTIL_H__B7C854BD_1BBC_43B5_B2DD_BE4EC959D957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMiniUtil.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMiniUtil dialog
#include "./DefineFiles/MiniSetupInfo.h"
#include "Separator.h"

class CDlgMiniUtil : public CDialog
{
// Construction
public:
//	CDlgMiniUtil(long lArMiniSetupInfo, CWnd* pParent = NULL);   // standard constructor
	CDlgMiniUtil(CString strPropertyInfo, CWnd* pParent = NULL);   // standard constructor


	CString m_strDivision;   //주기버튼 텍스트
	BOOL m_bLoad;
	CBrush m_brushStatic;	

// 멤버변수
public:
	CString m_strPropertyInfo;
	CString m_strPropertyInfoOut;

// 멤버함수
public:
	void SetPropertyInfo();
	CString GetPropertyInfo();
	CString TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN, OUT*/);


	void VerifyCircleButton(UINT nID, int nComboNum);  // 주기버튼 문자 조합 #nID = 컨트롤 ID, 
													   //nComboNum #0 = 구분 , #1 = 기간
	void ShowControl(int nEditCircleID, int nSpinID, int nThedayID);
	void ShowControl(int nID);
	void HideControl(int nID);
	void HideControl(int nEditCircleID, int nSpinID, int nThedayID);
	void OnEditCircleChange(UINT nID);

	BOOL GetInfo();
	void SetInfo();

	void SetInit();
	void SetDefault();

	void DrawLine(CDC* pDC);
	void ModifyLineStyle();
	
	void RemoveAllArray();

	CMiniSetupInfoArray	 m_arrMiniSetupInfo;
//	 CMiniSetupInfoArray	*m_pArMiniSetupInfo;


// Dialog Data
	//{{AFX_DATA(CDlgMiniUtil)
	enum { IDD = IDD_DLG_MINIUTIL };
	CSeparator	m_Line01;
	CSeparator	m_Line02;
	CSeparator	m_Line03;
	CSeparator	m_Line04;
	CSeparator	m_Line05;
	CSeparator	m_Line06;
	CSeparator	m_Line07;
	CSeparator	m_Line08;
	CSeparator	m_Line09;
	CSeparator	m_Line10;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMiniUtil)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	afx_msg void OnSelChangeComboDivision(UINT nID);  

	// Generated message map functions
	//{{AFX_MSG(CDlgMiniUtil)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBtnApply();
	afx_msg void OnBtnCancel();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMINIUTIL_H__B7C854BD_1BBC_43B5_B2DD_BE4EC959D957__INCLUDED_)
