#if !defined(AFX_EXLISTCTRL_H__C9DCFCE8_E6BB_4CD4_8EAE_DACBC0D070DA__INCLUDED_)
#define AFX_EXLISTCTRL_H__C9DCFCE8_E6BB_4CD4_8EAE_DACBC0D070DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExListCtrl.h : header file
//

#include "../DefineFiles/client_typedef.h"

/////////////////////////////////////////////////////////////////////////////
// CExListCtrl window
class CExListCtrl : public CListCtrl
{
// Construction
public:
	CExListCtrl();

public:
	BOOL m_bFullSelect;				/*item 선택시 row전체를 select상태로 setting
									TRUE - 전체 선택, FALSE - 특정 column만 선택*/
								

// Operations
public:
	BOOL SetAddColumn(int nItem, int nSubItem, LPCSTR strText);

	BOOL SetAddItem(int nItem, int nSubItem, LPCSTR strText,
										BOOL bCheckStyle = FALSE, BOOL bAdd = FALSE, int nImageIndex = -1);
	void SetDataItem(int nItem, QuickList	stQuickList, BOOL bAdd = TRUE);
	void SetDataItem(int nItem, CString strType, XMLTreeInfoFile	stXMLTreeInfoFile);


	int GetSelItem();
	void GetDataItem(int nItem, QuickList &stQuickList);
	void GetDataItem(int nItem, CString &strType, XMLTreeInfoFile &stXMLTreeInfoFile);



	void AutoSize();	
	void CalculateColSize();
	
	void SetCheckStyle();
	BOOL SetFullSelect();



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExListCtrl)
	afx_msg void OnItemClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnReceiveItemCheck(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXLISTCTRL_H__C9DCFCE8_E6BB_4CD4_8EAE_DACBC0D070DA__INCLUDED_)
