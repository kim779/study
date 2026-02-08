#pragma once
// ConfirmDlg.h : header file
//

#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_misc/fxStatic.h"

#include "ConfirmGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CConfirmDlg dialog

class CMapWnd;

class CConfirmDlg : public CDialog
{
// Construction
public:
	void SetOrderType(int nType);
	void SetOrderData(CStringArray* pArr);
	CConfirmDlg(CWnd* pParent = NULL, CMapWnd* pMapWnd = NULL, int nType = 1);   // standard constructor
	virtual ~CConfirmDlg();
// Dialog Data
	//{{AFX_DATA(CConfirmDlg)
	enum { IDD = IDD_CONFIRM };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfirmDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfirmDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CString Parser(CString &srcstr, CString substr);

	std::unique_ptr<CfxImgButton> m_pOk;
	std::unique_ptr<CfxImgButton> m_pCancel;
	std::unique_ptr<CfxStatic> m_pMsg;
	std::unique_ptr<CConfirmGrid> m_pConfirmGrid;
	CStringArray* m_pArrOrder;
	CMapWnd* m_pMapWnd;
	int m_nOrderType;

private:
	void FillOrderData();
};

