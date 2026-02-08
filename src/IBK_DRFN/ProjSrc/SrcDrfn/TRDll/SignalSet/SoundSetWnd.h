#if !defined(AFX_SOUNDSETWND_H__51821928_B6D1_4F72_AD9D_DBC39E2C2A2E__INCLUDED_)
#define AFX_SOUNDSETWND_H__51821928_B6D1_4F72_AD9D_DBC39E2C2A2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SoundSetWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSoundSetWnd window

#define IDC_COL_CNT				3
#define IDC_ROW_CNT				3

class CSoundSetWnd : public CWnd
{
// Construction
public:
	CSoundSetWnd();
	virtual ~CSoundSetWnd();

  CFont m_fontDefault, m_fontBold;
  CBrush  m_brushStatic;

public:
	CButton		m_btnNewsChk;
	CButton		m_btnConditionChk;
	CButton		m_btnFindChk;

	CStatic		m_ctrlTitle[IDC_COL_CNT];
	CComboBox	m_comboSound[IDC_COL_CNT][IDC_ROW_CNT];
	CButton		m_btnPlay[IDC_COL_CNT][IDC_ROW_CNT];
	CButton		m_btnFind[IDC_COL_CNT][IDC_ROW_CNT];


private:
	void SetEnable_Row(int nRow, BOOL bState);


public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSoundSetWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CSoundSetWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	//}}AFX_MSG

	afx_msg void OnBtnNewsChk();
	afx_msg void OnBtnConditionChk();
	afx_msg void OnBtnFindChk();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOUNDSETWND_H__51821928_B6D1_4F72_AD9D_DBC39E2C2A2E__INCLUDED_)
