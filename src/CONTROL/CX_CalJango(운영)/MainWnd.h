#pragma once
// MainWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window
#include <afxmt.h>
#include "Common.h"

class CParam
{
public:
	int	key=0;			// screen key
	CString	name;			// control symbol name
	CRect	rect;			// control rect
	CString	fonts;			// font name
	int	point=0;		// font point
	int	style=0;		// font style
	DWORD	tRGB=0;			// text color index
	DWORD	pRGB=0;			// paint color index
	CString	options;		// properties
};

class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd(CWnd* parent);
	virtual ~CMainWnd();

// Attributes
public:
	CWnd*	m_pParent;
	CParam	m_Param;

private:
#if 0
	CMapStringToPtr m_CodeMap;
#else
	CMap<CString, LPCTSTR, std::shared_ptr<Jango>, std::shared_ptr<Jango>> m_CodeMap;
#endif

	double	m_nMaip;
	double	m_nPprc;
	double	m_nSuik;
	double	m_dRate;

protected:
	CString m_acno;

// Operations
public:
	void	SetParam(struct _param *pParam);
	void	Calculate();

private:
	void	parsingAlertx(LPARAM lParam);
	CString getKey(bool bFuture, CString codeS, CString dateS, CString sygbS, CString sJggb, int trkey=-1);
	CString	Parser(CString &srcstr, CString substr);

	double	str2double(LPCSTR data, int dlen);
	double	str2double(const StringProxy& sp) { return str2double(sp.first, sp.second - sp.first); }
	int	str2int(LPCSTR data, int dlen);
	int	str2int(const StringProxy& sp) { return str2int(sp.first, sp.second - sp.first); }
	void	parseString(LPCSTR st, LPCSTR ed, CHAR del, vector<StringProxy>& retval);
	void	parseString(const StringProxy& sp, CHAR del, vector<StringProxy>& retval);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainWnd)
	CString m_dataList;
	afx_msg void OnDataListChanged();
	afx_msg void SetJango(LPCTSTR data);
	afx_msg void SetUpdate(LPCTSTR flag,LPCTSTR data);
	afx_msg void SetJango2(LPCTSTR sdata);
	afx_msg void SetUpdate2(LPCTSTR flag,LPCTSTR data);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};