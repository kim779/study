#pragma once
// JangoWnd.h : header file
//
#include "MapWnd.h"
#include "ControlWnd.h"
#include "AxScrollBar.h"
#include <afxtempl.h>

//#define MAX_ROW		12
#define MAX_ROW		7
#define MIN_ROW		2
/////////////////////////////////////////////////////////////////////////////
// JangoWnd window
struct _code
{
	CString code;
	CString name;
};

class CJangoWnd : public CWnd
{
// Construction
public:
	CJangoWnd(CWnd* pParent, CMapWnd* pMapWnd, CWnd* pView);

protected:
	//CRect		m_tRc1;			// 선물
	CRect		m_tRc2;			// 옵션
	CRect		m_rcCombo;		
	
	CString		m_text1;
	CString		m_text2;

	CRect		m_oRc1;			// 선물 1
	CRect		m_oRc2;			// 선물 2
	CRect		m_oRc3;			// 선물 3
	CRect		m_oRc4;			// 선물 4
	CRect		m_oRc5;			// 선물 5
	CRect		m_oRc6;			// 선물 6
	CRect		m_oRc7;			// 선물 7

	CRect		m_rcCall[MAX_ROW + 1];
	CRect		m_rcHsga[MAX_ROW + 1];
	CRect		m_rcPut[MAX_ROW + 1];

	CComboBox	m_cbOption;
	int			m_iRowCnt{};

	CWnd*		m_pParent{};
	CMapWnd*	m_pMapWnd{};
	CWnd*		m_pView{};
	CAxScrollBar	m_vbar;

	CString		m_home;
	CString		m_user;
	CString		m_mouseDLL;

	std::unique_ptr<class CfxImgButton>	m_pBtnFuture[11]{};

	bool		m_bLBDown;

	int			m_row;
	CArray <struct _code, struct _code>	m_arCode;

	void		setButtonBitmaps();

	bool		OCodeLoad(CString tabPath);
	bool		FCodeLoad(CString tabPath);

	void		setScroll();

	COLORREF	GetIndexColor(int index);
	CBitmap*	GetAxBitmap(CString path);
	CFont*		GetAxFont(CString fName = "굴림", int point = 9, bool bItalic = false, int nBold = 0);
	CBrush*		GetAxBrush(COLORREF clr);
	CPen*		GetAxPen(COLORREF clr, int width, int style);
	CString		SetComma(CString src);
	CString		Parser(CString &srcstr, CString substr);
	void		DrawSiseText(CDC* pDC, CString data, CRect rc, UINT nFormat = 0, UINT type = 0);
public:
	void		invalidateRC();
protected:
	CFont* m_pFont{}, * m_pBoldFont{};
	int			m_iRowH{};

private:
	HBITMAP		m_hBitmap{}, m_hBitmap_dn{}, m_hBitmap_hv{};

	struct _Code
	{
		CString Code;
		CString Name;
		CString Jango;
		CString Pgga;
		int Micheg=0;
	};

	struct _OCode
	{
		CString Hsga;
		struct _Code Call;
		struct _Code Put;
		bool	atm=false;
	};

	struct OCode
	{
		CString	Price;
		char	Atm{};
		struct CALL
		{
			char	Exist{};
			CString Name;
			CString	Code;
		} Call[11];
		struct PUT
		{
			char	Exist{};
			CString	Name;
			CString	Code;
		} Put[11];
	};
	struct	OCode	oCode;

	CArray <_Code *, _Code *> m_pFCode;		// code list
	CArray <_OCode *, _OCode *> m_pOCode[11];	// code list
	int	m_atmIdx[11]{};

// Operations
public:
	void		Init(CString home, CString user);
//	void		SetJango(WPARAM wParam, LPARAM lParam);
	void		SetJango(CString code, CString jgvl, CString pgdg);
	void		ClearJango();
	void		ClearMicheg();
	void		SetSize(bool bExpand);
	void		SetMicheg(CString code, int jmcnt);
	void		ChangeFont(double* lRate, CFont* pfont, CFont* boldFont);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJangoWnd)
	//}}AFX_VIRTUAL

// Implementationq
public:
	virtual ~CJangoWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CJangoWnd)zn
	afx_msg void OnButtonFuture1();
	afx_msg void OnButtonFuture2();
	afx_msg void OnButtonFuture3();
	afx_msg void OnButtonFuture4();
	afx_msg void OnButtonFuture5();
	afx_msg void OnButtonFuture6();
	afx_msg void OnButtonFuture7();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnOptSelEndOk();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

