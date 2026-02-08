#pragma once
// BaseWnd.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CBaseWnd window
#include "sharemsg.h"

class CBaseWnd : public CWnd
{
// Construction
public:
	CBaseWnd(CWnd* pMainWnd);

// Attributes
public:

// Operations
public:
	virtual void OperDraw(CDC* pDC);
	virtual void OperInit();
	virtual void OperDestory();
	virtual void OperResize(int cx, int cy);
	virtual void OperPallette();
	virtual CString Variant(int comm, CString data = "");
	virtual void DrawBitmap(CDC *pDC, CRect rc, CString strImage);
	virtual void DrawRoundBitmap(CDC *pDC, CRect rc, int nKind , int nUp = 0);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseWnd)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

private:
	
// Implementation
public:
	CString m_strPal;
	virtual ~CBaseWnd();

	CWnd* GetMainWnd() { return m_pMainWnd; };
	// Generated message map functions
protected:
	//{{AFX_MSG(CBaseWnd)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()	

	CFont*	 GetAxFont(CString fName = _T("±¼¸²Ã¼"), int point = 9, bool bBold = false, bool bItalic = false);
	CFont*	 GetAxFont(struct _fontR* font);
	CBrush*  GetAxBrush(COLORREF clr);	
	CPen*	GetAxPen(COLORREF clr, int width = 1, int style = PS_SOLID);
	CPen*	GetAxPen(struct _penR* pen);
	COLORREF GetAxColor(int color);
	BOOL	IsInit() { return m_bInit; }
	void ShowGuideMessage(CString strMessage);
	CString GetPalette();


// window members
	CWnd*		m_pMainWnd;	
	CWnd*		m_pViewWnd;
	
// drawing memebers
	CRect		m_rect;
	COLORREF	m_clrPallete;
	CFont*		m_pFont;
	BOOL		m_bInit;
	
// data members
	CString		m_root;
	CString		m_user;
	CString		m_fileCFG;
	bool		m_bBitmap;
};

