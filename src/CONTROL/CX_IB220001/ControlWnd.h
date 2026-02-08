#pragma once

#include <afxtempl.h>
#include "AxisExt.h"
#include "dataio.h"
#include "grid/GridCtrl.h"
#include "../../h/axisfire.h"

#define WMX_LAZYDRAW		WM_USER+1212
#define IDC_GRID		1000
#define DRAWQUEUE_CNT		50
#define DRAWQUEUE_TIMER		100


class CControlWnd : public CWnd, public CAxisExt
{
public:
	CControlWnd(CWnd* pParent, _param* pParam);

protected:
	CString	m_sMapHandle;
	CString m_code;
	CString m_hsga;
	int	m_SelRow, m_HsgaRow, m_DrawQueue;
	int	m_nPredict;
	int	m_nType;
	CRect	m_rcOrg;
	CRect	m_rect;
	std::unique_ptr<CGridCtrl> m_pGrid;

	CMap < CString, LPCTSTR, std::shared_ptr<OPTION_DATA>, std::shared_ptr<OPTION_DATA> > m_data;
	CMap < CString, LPCTSTR, std::shared_ptr<OPTION_DATA>, std::shared_ptr<OPTION_DATA> > m_qdata;

protected:
	void	ClearData();
	void	ParseOUB(LPCSTR dat, int len);
	void	ParseAlert(struct _alertR* alert);
	void	DrawCode(std::shared_ptr<OPTION_DATA> od, CDC *pdc = nullptr);
	OPTION_DATA* GetData(int row, int col);
	void	ChangeTheme();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
public:
	virtual void OnFinalRelease();
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

public:
	virtual ~CControlWnd();

protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	afx_msg void OnHsgaChanged();
	afx_msg void SetMapHandle(LPCTSTR sHandle);
	afx_msg BSTR GetMapHandle();
	afx_msg void SetPredict(short nIndex);
	afx_msg void SetType(short nType);
	//}}AFX_DISPATCH
	afx_msg LRESULT OnUser(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnLazyDraw(WPARAM wParam, LPARAM lParam);
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};