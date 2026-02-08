#pragma once
// ConfigDlg.h : header file
//
#include "HogaWindow.h"
/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog

class CConfigDlg : public CDialog
{
// Construction
public:
	CConfigDlg(int iType, CString sCrtType=_T(""), CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigDlg)
	enum { IDD = IDD_CONFIGDIALOG };
	//}}AFX_DATA
	int						m_iType;
	CHogaWindow*			m_pHogaWnd;
	CWnd*					m_pMain;

	CFont					m_pFont;
	COLORREF				m_lblbackcolor, m_databackcolor, m_diffcolor;
	CRect					m_siseRect[5];
	_config					m_stConfig;
	CString					m_strSise[10], m_strCode, m_strRealCode, m_strDifSign;


	void					CreateHogaWindow();
	void					SendHogaQuery(CString sCode);
	void					SetOubData(WPARAM wParam, LPARAM lParam);

	void					SetHogaData(char* szReceive) ;
	void					SetRealData(char* szReceive) ;
	void					PopupHoga();

	void					DrawPanel(CDC* pDC);
	void					DrawSisePanel(CDC* pDC, CRect rect);
	void					DrawLabelBox(CDC* pDC, CRect rect);

	void					ClearData();


	void					SetInitControl();
	void					SetConfigValue();
	void					SetSpinDeltaValue(NM_UPDOWN* pNMUD, CEdit* pEdit, int iunit);

	CString					GetDlgWindowText(UINT uiID);
	bool					SetDlgButtonCheck(UINT uiID, int icheck);
	int						GetDlgButtonCheck(UINT uiID);
	int						GetDlgComboGetCurSel(UINT uiID);


	void					SetCode(CString sCode, CString sName);
	CString					SetDiffValue(CString sDiff);

	void					SetJumunType(CString stype){m_strSise[7]=stype;};
	void					SetPossvol(CString svol){m_strSise[8]=svol;};
	









// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnButton1();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDeltaposSs2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSs3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSs4(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSs5(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSs6(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnSelchangePricecombo();
	afx_msg void OnClear();
	//}}AFX_MSG
	LONG OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHogaGubnCheck(UINT nID);

	DECLARE_MESSAGE_MAP()
};

