#if !defined(AFX_TOOLCFG_H__AE9E90C1_8A92_4B80_ADD5_E0FE67A4A561__INCLUDED_)
#define AFX_TOOLCFG_H__AE9E90C1_8A92_4B80_ADD5_E0FE67A4A561__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CToolCfg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolCfg dialog

class CToolCfg : public CDialog
{
// Construction
public:
	CToolCfg(CWnd* pMain, void* pvToolInfo, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CToolCfg)
	enum { IDD = IDD_TOOL_CONFIG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolCfg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolCfg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CWnd		*m_pMain;

	struct _toolInfo*	m_pToolInfo;

	CColorCtrl	m_colorCB;
	CKindCtrl	m_styleCB;
	CWidthCtrl	m_widthCB;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLCFG_H__AE9E90C1_8A92_4B80_ADD5_E0FE67A4A561__INCLUDED_)
