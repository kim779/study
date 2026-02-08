#pragma once

// ControlWnd.h : header file
//
#include "CategoryPopup.h"
/////////////////////////////////////////////////////////////////////////////
// CControlWnd window
#define WD_SIDE		(WM_USER + 5)
#define WD_SETVALUE	(WM_USER + 6)

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

#define	L_ACODE		7
struct _AllCode
{
	char	code[L_ACODE]{};
};
#define	sz_AllCode	sizeof(_AllCode)

class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd(BOOL bMini = FALSE);
	virtual	~CControlWnd();

	enum { OBJ_R = 0, OBJ_S, OBJ_U, OBJ_D};

// Attributes
public:
	CWnd*		m_pParent;
	CParam		m_Param;
	CString		m_rtnStr, m_sCodeList;
	CString		m_sSelectedName;
	bool		m_bCateChecked[CHECKER_COUNT];
	CArray <std::shared_ptr<_mhjcode>, std::shared_ptr<_mhjcode>>		m_hjCodes;
	CMapStringToString	m_codeMap;
	CArray <std::shared_ptr<CStringArray>, std::shared_ptr<CStringArray>>	m_codeArr;
	bool		m_bShowPlay;
	int		m_nLoads;
	CStringArray	m_history;

	DWORD		m_dwState;
	bool		m_capture, m_bRotate;
	BOOL		m_bTracking;
	BOOL		m_bHover[4];
	CRect		m_rcAll, m_rcBtn[4];
	HBITMAP		m_hBitmap[4], m_hBitmap_dn[4], m_hBitmap_hv[4];
	
	CString		m_sUser, m_sRoot;

	COLORREF	m_HoverColor;
	COLORREF	m_backColor;

	CString		m_sImagePath[4];
	CString		m_sText, m_sSymbol;

	int		m_nUcmd;		// updateX_20050330
	CString		m_jCount;		// updateX_20050330

	int		m_type, m_nKind;
	std::unique_ptr<CCategoryPopup>	m_pPopup;
	CFont		m_font;

	int		m_nAllCode;
	int		m_nSelectedCode;
	std::unique_ptr<_AllCode[]>	m_pAllCode;
	std::unique_ptr<_AllCode[]>	m_pSelectedCode;

	int		m_nChkPlay;

private:
	// 자체 돌려보기
	CStringArray	m_rotateArr;
	int		m_rotatePos;
	int		m_TimerID;
	int		m_TimerGap;

	void		StartRotate();
	void		StopRotate();
	void		InitRotate(int Gap = 2);
	bool		ProcRotate(bool bNext);
	CString		GetCodeFromAll(LPCTSTR pCode, bool bNext);
	CString		GetCodeFromSelectedItem(LPCTSTR pCode, bool bNext);


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
	BOOL		m_bMini;

	void		Draw(CDC *pDC);
	void		DrawButton(CDC *pDC, int index);
	void		DrawStretchByMask(CDC *pDC, CBitmap *pBitmap, CRect bRc, COLORREF maskcolor);
	bool		LoadHjCode();
	void		SendOK(CArray <std::shared_ptr<_mhjcode>, std::shared_ptr<_mhjcode>>& arr, int Gap, int idx =0);
	CString		getMidString(char *pStr, int nFirst, int nLen);
	CString		loadJCode(CString path, int nKind);
	void		SendTR(CString trc, char* datb, int datl, int key);
	CFont*		getAxFont(CString fName, int point, int style);
	CBitmap*	getBitmap(CString path);
	COLORREF	getAxColor(int nIndex);
	CString		OptionParser(CString sOption, CString sKey);
	CString		LoadCodeList(int nGroup);
	void		ShowPopup(CRect rc);
	void		SetButtonImages();
	CString		Variant(int comm, CString data);
	CString		Parser(CString &srcstr, CString substr);
	CSize		BitmapSize(HBITMAP hBitmap);
	void		SetParam(struct _param *pParam);
	LOGFONT		setFont(CString fName, int fsize, int style);
	COLORREF	GetIndexColor(int index);
	HCURSOR		GetCursor(int kind);

	void		RequestGroup();
	void		RequestCode(int igroup, int iprocess = 0);

protected:
	void		parseCode(char* datb, int datl, int iprocess);
	enum	{ procNONE = 0, procFORM, procEVENT };

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg	void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg long OnCategory(WPARAM wParam, LPARAM lParam);
	afx_msg long OnPopupMenu(WPARAM wParam, LPARAM lParam);
	afx_msg long OnInitLoad(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCategoryUpDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetValue(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	afx_msg BSTR GetJCodes();
	afx_msg BSTR _GetSelectedName();
	afx_msg void _SetSelectedName(LPCTSTR lpszNewValue);
	afx_msg BSTR GetData();
	afx_msg void SetData(LPCTSTR lpszNewValue);
	afx_msg BSTR GetCurCode();
	afx_msg BSTR GetPreCode(LPCTSTR code);
	afx_msg BSTR GetNextCode(LPCTSTR code);
	afx_msg void SetProperties(LPCTSTR str);
	afx_msg BSTR GetProperties();
	afx_msg long GetHandle();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};
