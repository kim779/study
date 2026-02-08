#pragma once
// ControlWnd.h : header file
//
#include "AxBrowser.h"
#include <afxmt.h>
/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

class CParam
{
public:
	int		key=0;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point=0;		// font point
	int		style=0;		// font style
	DWORD		tRGB=0;			// text color index
	DWORD		pRGB=0;			// paint color index
	CString		options;		// properties
};

struct  _mid 
{
        char    gubn[2];	// 큰 분류구분
	char	date[8];	// 날짜입력
};

struct  _mod
{
        char    nrec[4];	// # nrow count
        char    *data;
};

class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd();
	virtual ~CControlWnd();

// Attributes
public:
	CString		m_sRoot, m_sUser;
	CParam		m_Param;
	CWnd		*m_pParent;

protected:
	CAxBrowser	*m_pHtml;
//	CCriticalSection	m_cs;

private:
	CString		m_szFileName;
        CString		m_gubn;                // 큰 분류구분       
	CString		m_date;                // 날짜      

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	//void ResizeWeb(char *pcBuffer);
	void	EmptyFile();
	void	ParsingWeb(char *pcBuffer, int size);
	void	SendWeb(CString str);
	void	SendTR(CString name, char* data, BYTE type, int nLen = 0);
	CString	OptionParser(CString sOption, CString sKey);
	CString	Parser(CString &srcstr, CString substr);
	CString	Variant(int comm, CString data);
	void	SetParam(struct _param *pParam);

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMessage1(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	afx_msg void SetGubn(LPCTSTR lpszNewValue);
	afx_msg void SetDate(LPCTSTR lpszNewValue);
	afx_msg void Start();
	afx_msg BSTR GetProperties();
	afx_msg void SetProperties(LPCTSTR sParam);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};