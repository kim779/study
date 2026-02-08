#if !defined(AFX_FIELDSET_H__3CAD2346_4C6D_4DD4_92E7_EEFFE0A18ADD__INCLUDED_)
#define AFX_FIELDSET_H__3CAD2346_4C6D_4DD4_92E7_EEFFE0A18ADD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FieldSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFieldSet dialog
class CCodeCtrl;
class CFieldSet : public CDialog
{
// Construction
public:
	//CFieldSet(CWnd* pParent = NULL);   // standard constructor
	CFieldSet(CWnd* pParent , StFieldSet *pstMainFieldSet);
	StFieldSet	m_stFieldSet;
	StFieldSet	*m_pMainData;
	//StFieldSet	m_stFieldSetPast;
	BOOL m_bChange;
	//char* m_lpPast;

	CfxImgButton m_btnOk;
	CfxImgButton m_btnCancel;

	CString m_strScreenNo;
	CString m_strFieldPath;
// Dialog Data
	//{{AFX_DATA(CFieldSet)
	enum { IDD = IDD_DLG_FIELDSET };
	CButton	m_chkConvolPower;
	//CButton	m_btnCancel;
	CEdit	m_editColNum;
	CSpinButtonCtrl	m_spinCol;
	CButton	m_chkAccumDealVol;
	CButton	m_chkChange;
	CButton	m_chkChgRate;
	CButton	m_chkCodeItem;
	CButton	m_chkConVol;
	CButton	m_chkConVolRate;
	CButton	m_chkDealVolRate;
	CButton	m_chkNameItem;
	CButton	m_chkPriceNow;
	CButton	m_chkTime;
	CButton m_chkCount;
	int		m_nColNum;
	//}}AFX_DATA
private:
	CCodeCtrl*		m_pctrlJMCombo;
	CWnd*			m_pctrlCodeInput;
	CWnd*			m_pWndMainFrm;
	
public:
	void SetInit();
	void SetCheckField();
	void GetCheckField();
	BOOL CheckChange();
	void OnSize();

	static BOOL GetLoad_FieldInfo(CString strFullPath, StFieldSet	*pstFieldSet);
	static BOOL SetSave_FieldInfo(CString strFullPath, StFieldSet	*pstFieldSet);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFieldSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFieldSet)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnOk();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCount();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIELDSET_H__3CAD2346_4C6D_4DD4_92E7_EEFFE0A18ADD__INCLUDED_)
