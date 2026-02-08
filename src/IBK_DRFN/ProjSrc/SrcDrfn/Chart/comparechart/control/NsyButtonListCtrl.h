#if !defined(AFX_NSYBUTTONLISTCTRL_H__CA9798D0_9469_46D8_BDC4_9E5A01AB17E2__INCLUDED_)
#define AFX_NSYBUTTONLISTCTRL_H__CA9798D0_9469_46D8_BDC4_9E5A01AB17E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyButtonListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNsyButtonListCtrl window
#include "xSkinButton.h"
#include "xListCtrl.h"
typedef struct BTNINFO
{
	int				nIndex;	
	CxSkinButton	xSkinButton;
}BTNINFO,*PBTNINFO;

class CStdDialog;
class CNsyButtonListCtrl : public CXListCtrl
{
// Construction
public:
	CFont m_FontOfFont;
	CNsyButtonListCtrl();
	CPtrArray m_ptrArrayBtnWnd;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNsyButtonListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:	
	void DeleteChartData(int nItem);
	void SendToChartRemoveGraph(int nItem);

	void ClearArrayBtnWnd();
	void Init_CompareChart();
	void DeleteOneItem();
	void DeleteSelectItem(int nItem);
	void RemoveOnlyList(int nItem);

	CString m_szImageDir;
	void Init();	
	void SetItemButton(int nItem, CString strJongName, int nJongCode);
	virtual ~CNsyButtonListCtrl();

	BOOL m_bShowBlank;
	void SetItemButtonVisible(int nItem, BOOL bVisible);

	// Generated message map functions
protected:
	BOOL m_bDirectRemove;

	//{{AFX_MSG(CNsyButtonListCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	//afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NSYBUTTONLISTCTRL_H__CA9798D0_9469_46D8_BDC4_9E5A01AB17E2__INCLUDED_)
