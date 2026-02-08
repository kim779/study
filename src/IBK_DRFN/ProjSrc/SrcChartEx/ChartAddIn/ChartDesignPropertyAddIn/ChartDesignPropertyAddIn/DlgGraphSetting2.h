#if !defined(AFX_DLGGRAPHSETTING2_H__5C73A2DE_C819_4EB9_937D_D6AC30C209DD__INCLUDED_)
#define AFX_DLGGRAPHSETTING2_H__5C73A2DE_C819_4EB9_937D_D6AC30C209DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGraphSetting2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSetting2 dialog

class CGraphSetting2Data;

class CDlgGraphSetting2 : public CDialog
{
// Construction
public:
	CDlgGraphSetting2(CGraphSetting2Data* pGraphSetting2Data = NULL, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGraphSetting2)
	enum { IDD = IDD_DIALOG_GRAPHSETTING2 };
	BOOL	m_bCurData;
	BOOL	m_bHighNow;
	BOOL	m_bLowHigh;
	BOOL	m_bLowNow;
	BOOL	m_bPrevNow;
	int		m_nCurDataType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGraphSetting2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGraphSetting2)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCurData();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetGraphSetting2Data(CGraphSetting2Data* pGraphSetting2Data);

private:
	bool SetDlgItemData();
	void SetDlgItemData_CurDataType(const int nCurDataType);
	bool SetGraphSetting2Data();

	void EnableWindow_CurDataType(const bool bEnable);
	void EnableWindow(const int nID, const bool bEnable);

private:
	CGraphSetting2Data* m_pGraphSetting2Data;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGRAPHSETTING2_H__5C73A2DE_C819_4EB9_937D_D6AC30C209DD__INCLUDED_)
