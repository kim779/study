#pragma once
// MapWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window
#include "../../control/fx_misc/fxImgButton.h"
#include "sharemsg.h"
#include "GridWnd.h"
#include <afxmt.h>
class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd(CWnd* pWnd);

// Attributes
public:

// Operations
public:
	BOOL GetRTSVisible() { return m_bRTS; };
	void SetRTSVisible(BOOL bRTS) { m_bRTS = bRTS; };
	void SetRTSTreeData(CGridWnd* pGrid);
	COLORREF getAxColor(int color);
	CFont*	 GetAxFont(CString fName = _T("굴림체"), int point = 9, bool bBold = false, bool bItalic = false);
	void DestroyWnd();

	void testSaveFile(CString dt);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
// MEMBERS
// window members
	CWnd*	m_pWnd;
	class CTreeWnd*		m_pTreeWnd;
	class CBaseWnd*		m_pToolWnd; 
	class CBaseWnd*		m_pBasketWnd;
	class CBaseWnd*		m_pGroupWnd;
	class CBaseWnd*		m_pRTSWnd;
	bool			m_bProc;
	int			m_nOption;
	
	
// resource memebers
	CSize		m_size;
	int			m_nTree;	// 트리 사이즈
	int			m_nBasket;	// 바구니 싸이즈
	int			m_nGroup;	// 그룹윈도우 싸이즈
// for resizing	
	int			m_nResize;
	CPoint			m_xPos;
	CPoint			m_xPosMove;
	CRect			m_rcDIVIDE;
// data members
	CString			m_user;
	CString			m_id;
	CString			m_home;
	BOOL			m_bTree;
	BOOL			m_bRTS;
	BOOL			m_bBasket;
	CCriticalSection	m_sync;
	CRect			m_rcDivide[3];
	CRect			m_rcPanel[3];//for rounding

	CfxImgButton		m_btOption;
	class COptDlg*		m_pOptDlg;
	CFont*			m_pFont;
	bool			m_bDestroy;
	CBrush*			m_pBrush;
	int	m_nOptHeight;
	CString			m_szRET;
	CString			m_strWaveFile;
	BOOL			m_bWaveApply;

	COLORREF	m_clrKospi;
	COLORREF	m_clrKosdaq;
	BOOL		m_bMKClrApply;
	BOOL		m_bPopupDlg;
	BOOL		m_bDispCode;

// etc...
	void DrawBitmap(CDC *pDC, CRect rc, CString strImage);
	void DrawRoundBitmap(CDC *pDC, CRect rc, int nKind);
	
// OPERATION
// windows operation
	CBrush*	GetAxBrush(COLORREF clr);
	void	DrawMove(CRect rect);
	void	SendOper(class CSendData* sdata);
	void	SelectGroup(int kind);
	void	CreateChild();
	void	DestroyChild();
	void	ResizeOper(int cx, int cy);
	LONG	OperTreeVisible(int opt);
	LONG	OperRTSVisible(int opt);
	LONG	OperBasketVisible(int opt);
	void	OperSize(int opt, CSize size);
	void	OpenScreen(int type, CString data);
	LONG	OperDLLOUB(WPARAM wParam, LPARAM lParam);
	void	parsingTrigger(CString datB);
	HBITMAP getBitmap(CString path);
	DWORD	SearchOption(WORD kind, DWORD option);
	
	void	parsingDomino(CString datB);
// resource operation
	void	DrawDivide(CDC* pDC);
	void	SetPallette();
	void	InvalidateDivide();
	
// data operation
	void	init();
	void	loadinfo();
	void	saveinfo();
	void	sendTR(CString trCode, char* datB, int datL, int key);
	CString Variant(int comm, CString data = "");
	void SendMode(bool bSend = true);
	void Enable(int kind, bool bEnable = true);
// etc...
	void GuideMessage(CString msg);

public:		// RTM
	void SendMinMaxInfo();
	BOOL IsValidCode(CString strCode);
	CString GetDataTitle(int nKind);
	CArray<char *, char *>	m_arBytesRTM;
	CCriticalSection	m_syncRTM;
	CWinThread*		m_pThreadRTM;
	bool			m_bThreadRTM;

	void	doRTM(char *pBytes);
	bool	WaitRTM();
private:	// RTM
	HANDLE			m_hEventRTM;
	
	void	initRTM();
	void	stopRTM();
	void	removeRTM();
private:
	BOOL	m_bRTMReady;
	MINMAXINFO m_MMI;

	CString m_strPal;
	CString GetPalette();
};
