#if !defined(AFX_ANALBARBASEDLG_H__722F9766_0E3C_4363_8945_ABD1A32B9A7E__INCLUDED_)
#define AFX_ANALBARBASEDLG_H__722F9766_0E3C_4363_8945_ABD1A32B9A7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnalBarBaseDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAnalBarBaseDlg dialog
#include "UserColorTable.h"

class CAnalBarBaseDlg : public CDialog
{
// Construction
public:
	CAnalBarBaseDlg(UINT nID, CWnd* pParent = NULL);   // standard constructor
	virtual ~CAnalBarBaseDlg();

	virtual void EnableDlgControls(bool bEnable);
	virtual void ResizeControl(int cx, int cy);
	virtual void ChangeColor() = 0;
	void setColorTable(CArray<STUSERCOLORTABLE, STUSERCOLORTABLE>* pArrColor) { m_pArrColorTable = pArrColor; };
	LRESULT SendMessageToChartViaMain(UINT nMsgId, LPVOID lpCode, DWORD dwParam);
	void	SetChartHandle(HWND hWnd) { ASSERT(hWnd != NULL); m_hChart = hWnd; };
	void	SetChartHandle(CWnd* pWnd) { ASSERT_VALID(pWnd); m_hChart = pWnd->GetSafeHwnd(); };

	CArray<STUSERCOLORTABLE, STUSERCOLORTABLE>* m_pArrColorTable;

// Dialog Data
	//{{AFX_DATA(CAnalBarBaseDlg)
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnalBarBaseDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HWND	m_hChart;
//	HBRUSH  m_hBrush;
	// Generated message map functions
	//{{AFX_MSG(CAnalBarBaseDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANALBARBASEDLG_H__722F9766_0E3C_4363_8945_ABD1A32B9A7E__INCLUDED_)
