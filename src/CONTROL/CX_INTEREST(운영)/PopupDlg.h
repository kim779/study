#pragma once
// PopupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPopupDlg dialog

class CPopupDlg : public CDialog
{
// Construction
public:
	CPopupDlg(CWnd* pParent = nullptr);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPopupDlg)
	enum { IDD = IDD_POPUP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attributes
private:
	CStringArray m_arrMenuString;
	CUIntArray   m_arrMenuID;
	CUIntArray   m_arrMenuCheck;
	CPtrArray    m_arrBtn;
	std::vector<std::unique_ptr<CButton>> _vBtn;
	int          m_nCurSel;

public:
	int m_nX;
	int m_nY;
	int m_nResult;

// Operators
public:
	void AddMenu(int nID, CString strMenu, int nCheck = 0) { m_arrMenuID.Add(nID); m_arrMenuString.Add(strMenu); m_arrMenuCheck.Add(nCheck); }
	HBITMAP getBitmap(CString path);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPopupDlg)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_MSG
	afx_msg void OnCommandBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};