// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__83C30872_8276_4049_BC46_029C0660CB67__INCLUDED_)
#define AFX_CHILDVIEW_H__83C30872_8276_4049_BC46_029C0660CB67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>
#include <afxcoll.h>

#define K_MAX		5
#define K_NORMAL	0
#define K_SNDRCV	1
#define K_RTMS		2
#define K_SYSTRACE	3
#define K_REPORT	4

class CQue : public CObject
{
public:
	CQue()		{ m_pBytes = NULL; }
	virtual ~CQue()	{ if (m_pBytes)	delete [] m_pBytes; }

public:
	DWORD	m_flag;
	int	m_nBytes;
	char	*m_pBytes;
};

struct _options {
	BOOL	send;		// displays send data	(default : TRUE)
	BOOL	receive;	// displays receive data (default : TRUE)
	BOOL	header;		// displays data header (default : TRUE)
	BOOL	data;		// displays data (default : TRUE)
	BOOL	rts;		// displays rts data 0:all code, 1:filtering code (default : FALSE)
};

/////////////////////////////////////////////////////////////////////////////
// CChildView window
#include "RichLib/RichEditCtrlEx.h"
class CChildView : public CWnd
{
// Construction
public:
	CChildView();
	virtual ~CChildView();
	
// Attributes
protected:
	CFont		m_font;
	CRichEditCtrlEx	m_trace;

	BOOL	m_bBINARY;
	BOOL	m_bSNDRCV;
	BOOL	m_bRTM;
	BOOL	m_bSYSTRACE;
	BOOL	m_bREPORT;
	BOOL	m_bNOSCROLL;
	BOOL	m_bSTAYONTOP;

	BOOL	m_bBold[K_MAX];
	BOOL	m_bItalic[K_MAX];
	BOOL	m_bUline[K_MAX];
	BOOL	m_bStrike[K_MAX];

	int	m_matchN;
	bool	m_bMatchCase;
	bool	m_bMatchWholeWord;

	BOOL	m_bSameColor;
	BOOL	m_bSameFont;

	COLORREF	m_bColor;
	COLORREF	m_tColor[K_MAX];
	CString		m_fName[K_MAX];
	int		m_fSize[K_MAX];
	int		m_rtsL;

	CString		m_findS;
	CFindReplaceDialog*	m_findDlg;
	static const UINT	m_findMSG;

	CObArray		m_que;
	CCriticalSection	m_sync;

	CMapStringToString	m_mapCode;
	struct _options		m_options;

	CString			m_filename;

	// 키워드 필터
	CEdit		m_editKeyword;
	CButton		m_chkFilter;
	CString		m_strKeyword;
	BOOL		m_bFilterOn;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void Test();

	void	Init();
	void	CopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) ;

	void	addTrace(CString dat, int kind = K_NORMAL);
	CString	parse(CString &dat, CString separate);
	void	parseData(CString dat, CMapStringToString& ary, CStringArray& aryS);
	void	ReportParse(CString dat, CMapStringToString& ary, CStringArray& aryS);
	void	setTrace(bool setting = true);
	void	OnRCVData(WPARAM wParam, LPARAM lParam);
	void	WriteFile(char* pBytes, int nBytes);
	BOOL	SearchData(CString FindName, bool bMatchCase, bool bMatchWholeWord, bool bSearchDown);
	BOOL	SearchUP(CString FindName, bool bMatchCase, bool bMatchWholeWord);
	void	MovePos(CHARRANGE cr);
	void	loadOptions();
	// Generated message map functions
protected:
	//{{AFX_MSG(CChildView)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnModeBinary();
	afx_msg void OnUpdateModeBinary(CCmdUI* pCmdUI);
	afx_msg void OnModeText();
	afx_msg void OnUpdateModeText(CCmdUI* pCmdUI);
	afx_msg void OnTrBase();
	afx_msg void OnUpdateTrBase(CCmdUI* pCmdUI);
	afx_msg void OnRtm();
	afx_msg void OnUpdateRtm(CCmdUI* pCmdUI);
	afx_msg void OnSystemtrace();
	afx_msg void OnUpdateSystemtrace(CCmdUI* pCmdUI);
	afx_msg void OnSetbkcolor();
	afx_msg void OnSettextcolor();
	afx_msg void OnSetfont();
	afx_msg void OnClear();
	afx_msg void OnDestroy();
	afx_msg void OnNoscroll();
	afx_msg void OnUpdateNoscroll(CCmdUI* pCmdUI);
	afx_msg void OnStayontop();
	afx_msg void OnUpdateStayontop(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnNextFind();
	afx_msg void OnPrevFind();
	afx_msg void OnStringFind();
	afx_msg void OnReportformat();
	afx_msg void OnUpdateReportformat(CCmdUI* pCmdUI);
	afx_msg void OnOptions();
	afx_msg void OnSave();
	afx_msg void OnSaveas();
	afx_msg void OnChangeEditKeyword();
	afx_msg void OnClickedChkFilter();
	//}}AFX_MSG
	afx_msg LRESULT OnFindDialogMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnReceive(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__83C30872_8276_4049_BC46_029C0660CB67__INCLUDED_)
