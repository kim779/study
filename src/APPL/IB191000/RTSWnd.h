#pragma once
// RTSWnd.h : header file
//

#include "BaseWnd.h"
#include "RTSGrid.h"
#include "setup.h"
#include "../../h/axisfire.h"
#include <afxmt.h>

/////////////////////////////////////////////////////////////////////////////
// CRTSWnd window

class CRTSWnd : public CBaseWnd
{
// Construction
public:
	CRTSWnd(CWnd* pMainWnd);

// Attributes
public:
	void SetSelectItem(CString strTitle, UINT nKind);

// Operations
protected:
	void LoadConfig();
	void SaveConfig();
	void SetUp();
	void Dispatch(int nKey, char* pDataB, int nDataL);
	void Alert(CString strData);
	BOOL IsValidCode(CString strCode);

public:
	void SendTR(BOOL bClear = FALSE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRTSWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	void FormatThousand(CString& strVal);
	virtual ~CRTSWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRTSWnd)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	void DrawTitle(CDC* pDC, CRect rcClient);
	void OperDraw(CDC* pDC);
	void OperResize(int cx, int cy);
	void OperInit();

private:
	CString m_strTitle;
	UINT m_nKind;

	CRTSGrid* m_pGrid;
	Cconfig* m_pConfig;

	CCriticalSection m_csSync;
	BOOL m_bOn;
	int m_nRow;
};

