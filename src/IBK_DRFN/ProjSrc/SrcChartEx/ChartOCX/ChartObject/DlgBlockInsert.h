#if !defined(AFX_DLGBLOCKINSERT_H__1C3D9526_E683_4C23_8819_FF253EB0AC81__INCLUDED_)
#define AFX_DLGBLOCKINSERT_H__1C3D9526_E683_4C23_8819_FF253EB0AC81__INCLUDED_

#include "resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBlockInsert.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgBlockInsert dialog

class CMainBlockImp;
class CInsertingBlockHorzScaleData;

class CDlgBlockInsert : public CDialog
{
// Construction
public:
	CDlgBlockInsert( CWnd* pParent, CMainBlockImp *p_pMainBlockImp, CInsertingBlockHorzScaleData* pInsertingHorzScaleData);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgBlockInsert)
	enum { IDD = IDD_BLOCK_INSERT };
	BOOL	m_bOnlyBottomShow;
	int		m_nScalePosition;
	int		m_nScaleGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBlockInsert)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBlockInsert)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnOnlyBottomShow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


private:
	CWnd* m_pParent;
	CInsertingBlockHorzScaleData* m_pInsertingBlockHorzScaleData;
	
	void InitializeData();
	void SetHorzScaleData();

	void EnableWindowOnOnlyEndScale(const bool bEnable);
	void EnableWindow(CWnd* pItem, const bool bEnable);

// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
public:
	CMainBlockImp *		m_pMainBlockImp;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBLOCKINSERT_H__1C3D9526_E683_4C23_8819_FF253EB0AC81__INCLUDED_)
