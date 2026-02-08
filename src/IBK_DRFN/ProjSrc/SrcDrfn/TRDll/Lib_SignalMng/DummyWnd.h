#if !defined(AFX_DUMMYWND_H__0DBD14F4_C3F2_49DF_BE96_01F34D4B732B__INCLUDED_)
#define AFX_DUMMYWND_H__0DBD14F4_C3F2_49DF_BE96_01F34D4B732B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DummyWnd.h : header file
//

//# ªÔº∫¡ı±«
//#include "../../../Controls/CommApi/Commexport.h"	-hidden-
//#include "../../../Common/CommHeaderDef.h"

#include "Signal_MngDef.h"
/////////////////////////////////////////////////////////////////////////////
// CDummyWnd window

class CLib_SignalMngApp;
class CDummyWnd : public CWnd
{
// Construction
public:
	CDummyWnd();

// Attributes
public:
	CLib_SignalMngApp *m_ptheApp;

	int Modual_SendTr(CString TrNo, int nTrLen, BYTE *TrData);
	BOOL SetSearch_NewsAll(Signal_NewsInfo stNews);
	
	int Modual_SendTr2(CString TrNo, int nTrLen, BYTE *TrData, LPCSTR strScreenCode);
	BOOL SetSearch_NewsAll2(Signal_NewsInfo stNews, LPCSTR strScreenCode);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDummyWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDummyWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDummyWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	afx_msg long OnGetData(WPARAM wParam, LPARAM lParam);
	afx_msg long OnGetBroad(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DUMMYWND_H__0DBD14F4_C3F2_49DF_BE96_01F34D4B732B__INCLUDED_)
