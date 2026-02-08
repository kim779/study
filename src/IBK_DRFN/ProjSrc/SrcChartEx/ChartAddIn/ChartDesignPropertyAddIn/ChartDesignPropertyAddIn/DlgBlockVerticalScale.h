#if !defined(AFX_DLGBLOCKVERTICALSCALE_H__A89BF627_5C80_40EE_BC74_9C49A80F0371__INCLUDED_)
#define AFX_DLGBLOCKVERTICALSCALE_H__A89BF627_5C80_40EE_BC74_9C49A80F0371__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBlockVerticalScale.h : header file
//
#include "CheckComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgBlockVerticalScale dialog
class CChartLink;
class CDlgBlockVerticalScale : public CDialog
{
// Construction
public:
	CDlgBlockVerticalScale( CChartLink *pItem, HWND p_hOcxWnd, CWnd* pParent = NULL);   // standard constructor

	void SetAllItemInfoOfPacketPage(const CString &strAllItemInfoOfPacketPage);
// Dialog Data
	//{{AFX_DATA(CDlgBlockVerticalScale)
	enum { IDD = IDD_DLG_VERTSCALE };
	CCheckComboBox	m_ScaleDataComboBox;
	int		m_nScalePosition;
	CString	m_strScaleData;
	int		m_nScaleCompartType;
	int		m_nScaleExpressionType;
	int		m_nMinMaxType;
	double	m_dMax;
	double	m_dMin;
	BOOL	m_bMinMaxLock;
	int		m_nDownSpace;
	int		m_nUpSpace;
	double	m_dHogaNumber;
	double	m_dUserNumber;
	int		m_nUserHogaType;
	BOOL	m_bIsInvert;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBlockVerticalScale)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBlockVerticalScale)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboSaclestyle();
	//}}AFX_MSG
	void OnRdoMinmaxType( UINT nID);
	void OnRdoUserHogaType( UINT nID);
	DECLARE_MESSAGE_MAP()
private:
	CChartLink* m_pItem;
	CString m_strAllItemInfoOfPacketPage;

	void AddStringToScaleDataComboBox();
	void CheckingItems();

// (2007/7/16 - Seung-Won, Bae) Check Invalid Value.
protected:
	void CheckValue( int p_nPreSpace, int p_nNewSpace, int p_nID);

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBLOCKVERTICALSCALE_H__A89BF627_5C80_40EE_BC74_9C49A80F0371__INCLUDED_)
