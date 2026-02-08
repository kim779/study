#if !defined(AFX_DLGCONFIRM_H__D827B161_66DB_42C1_BCA1_C17A9017FEDF__INCLUDED_)
#define AFX_DLGCONFIRM_H__D827B161_66DB_42C1_BCA1_C17A9017FEDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgConfirm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgConfirm dialog

class CDlgConfirm : public CDialog
{
// Construction
public:
	CDlgConfirm(CWnd* pParent = NULL);   // standard constructor
	
	void SetData(CString strName, int nUpDown, int nSellBuy);
// Dialog Data
	//{{AFX_DATA(CDlgConfirm)
	enum { IDD = IDD_DLG_CONFIRM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgConfirm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	CString m_strName;
	int m_nUpDown;
	int m_nSellBuy;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgConfirm)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include <afxtempl.h>
typedef struct 
{
	char aTime[8+1];
	CString strName;
	int nUpDown;
	int nSellBuy;
} stCDlgConfirmList_Item;
typedef CList<stCDlgConfirmList_Item*, stCDlgConfirmList_Item*> List_stCDlgConfirmList_Item;
class CDlgConfirmList : public CDialog
{
	// Construction
public:
	CDlgConfirmList(CWnd* pParent = NULL);   // standard constructor
	~CDlgConfirmList();
	
	void SetData(CString strName, int nUpDown, int nSellBuy);
	void ExSetData(CString strName, int nUpDown, int nSellBuy);
	// Dialog Data
	//{{AFX_DATA(CDlgConfirmList)
	enum { IDD = IDD_DLG_CONFIRMLIST };
	CListBox	m_ctlListbox;
	//}}AFX_DATA	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgConfirmList)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	CString m_strName;
	int m_nUpDown;
	int m_nSellBuy;

	CString m_szUpDown[2], m_szSellBuy[2];
	List_stCDlgConfirmList_Item m_ItemList;
	BOOL _DoShowIndex(int nIndex);

	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CDlgConfirmList)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeList1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCONFIRM_H__D827B161_66DB_42C1_BCA1_C17A9017FEDF__INCLUDED_)
