#pragma once
// JumunConfirmDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJumunConfirmDlg dialog

class CJumunConfirmDlg : public CDialog
{
// Construction
public:
	CJumunConfirmDlg(int iType, CFont* pFont, COLORREF* colorlist, COLORREF txtcolor, COLORREF bgcolor, COLORREF* dangacolor, 
					bool bcrttype=false, CWnd* pParent = NULL);   // standard constructor

	void SetValue(CString sAccount, CString sAccnName, CString sCodeName, CString sGubn, CString sCrtText, CString sVol, CString sPrice);

// Dialog Data
	//{{AFX_DATA(CJumunConfirmDlg)
	enum { IDD = IDD_JUMUNCONFIRMDIALOG };
	CfxImgButton	m_OkBtn;
	CfxImgButton	m_CancelBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJumunConfirmDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd*		m_pParent;
	COLORREF	m_colorList[5], m_txtcolor, m_backcolor, m_dangaColor[2];
	CFont*		m_pFont, m_pBoldFont;
	CString		m_strValue[10];
	int			m_iType;
	bool		m_bCrtType;

	// Generated message map functions
	//{{AFX_MSG(CJumunConfirmDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnButtonok();
	afx_msg void OnButtoncancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

