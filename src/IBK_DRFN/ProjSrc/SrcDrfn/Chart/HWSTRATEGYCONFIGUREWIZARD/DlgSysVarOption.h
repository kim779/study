#if !defined(AFX_DLGSYSVAROPTION_H__42A893FD_9439_409E_B459_5F580EFA78C5__INCLUDED_)
#define AFX_DLGSYSVAROPTION_H__42A893FD_9439_409E_B459_5F580EFA78C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSysVarOption.h : header file
//

#include "resource.h"
#include "Control/ColorButton.h"
#include "../STConfigureDlg/Control/XListCtrl.h"
#include "../../inc/ExEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSysVarOption dialog

class CDlgSysVarOption : public CDialog
{
// Construction
public:
	CDlgSysVarOption(CWnd* pParent = NULL);   // standard constructor

	CStringList*	m_pInputList;
	CString			m_szSTPath, m_szSFGPath;

	void InitColumn();
	void SetParam(CStringList* p_dwInput, LONG dwOutput);
	void DoApplyInputParam();
	void DoLoadVarList(LPCSTR szDataFile);
	void DoBtnClr(CColorButton &rColorButton);		// 버튼의 색상을 변경할 수 있도록 한다.

// Dialog Data
	//{{AFX_DATA(CDlgSysVarOption)
	enum { IDD = IDD_DLGSYSVAROPTION };
	CColorButton	m_btnClrBull;
	CColorButton	m_btnClrBear;
	CXListCtrl		m_InputList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSysVarOption)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSysVarOption)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtClrBull();
	afx_msg void OnBtClrBear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSYSVAROPTION_H__42A893FD_9439_409E_B459_5F580EFA78C5__INCLUDED_)
