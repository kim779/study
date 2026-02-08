#pragma once
// MainWnd.h : header file
//
#include "../../control/fx_misc/fxImgButton.h"
#include "ViewWnd.h"
#include "ToolWnd.h"
#include "sharemsg.h"
#include "MatrixWnd.h"
#include <afxmt.h>

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window

struct SISEWND_INFO{
	int rows{};
	int cols{};
	int count{};
	int maxcols{};
	CString* codes{};
	int mode{};
};

class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd(CWnd* pParent);

// Attributes
	CWnd*			m_pMainWindow{};
	CSize			m_size;
	std::unique_ptr<CToolWnd>		m_pToolWnd{};
	std::unique_ptr <CViewWnd>		m_pViewWnd{};		//	CArray <class CViewWnd*, class CViewWnd*> m_pViewWnd;
	std::unique_ptr<CMatrixWnd>		m_pMatrixWnd{};

public:
	

	CString			m_root;
	CString			m_name;
	CString			m_user;
	CString			m_id;
	CString			m_home;
	CBrush			m_bkBrush;
	
	COLORREF		m_clrPallete{};
	char			m_key{};
	CString			m_strParamName, m_strRealCode;
	CFont*			m_font{}, m_fontB{};
	int			m_fontsize{};
	COLORREF		m_clrLineColor{}, m_clrBgColor{}, m_clrDataFgColor{}, m_clrLblFgColor{};
	COLORREF		m_clrColor[10]{};
	int			m_clrColorIdx[10]{};

	CfxImgButton		m_btnMatrix;
	HBITMAP			m_bitmap_nm{}, m_bitmap_dn{}, m_bitmap_hv{};

	int				m_iRows{};
	int				m_iCols{};
	int				m_iHoga{};
	int				m_iCombo{};

	CCriticalSection	m_sync;
	BOOL			m_bIsOpen{};			//처음 열었는지 확인하는 변수
	bool			m_bDestroy{};

	//RTM
	HANDLE			m_hEventRTM{};
	bool			m_bThreadRTM{};
	CArray<char*, char*>	m_arBytesRTM{};
	CCriticalSection	m_syncRTM{};
	CWinThread*		m_pThreadRTM{};
	BOOL			m_bRTMReady{};
	int			m_nBtnIndex{};
// Operations

	BOOL			OnInit();
	void			loadScreenInfo(int index);
	void			saveScreenInfo(int index);
	
	BOOL			create(CWnd* parent);
	BOOL			Create();
	void			CreateChild();
	void			SetIsOpen(bool data);				
	BOOL			GetIsOpen();			
	
	void			Procedure(CString strProc, int mapkey);
	int				GetMajor();

	void			init();
	int				GetRows();
	int				GetCols();
	int				GetHoga();
	void			SetRows(int num);
	void			SetCols(int num);
	COLORREF		GetColor(int color);
	CFont*			getAxFont(CString fName, int point=9, bool bBold=false, bool bItalic=false);
	CBitmap*		getAxBitmap(CString path);
	void			SetParam(struct	_param* param, CWnd *parent);
	LRESULT			SendAxisMessage(WPARAM wParam, LPARAM lPAram);

	void			OperSize(CSize size);
	void			OperSize(int row, int col);

	void			parsingDomino(CString datB);
	CString			parseX(CString &srcstr, CString substr);
	int				GetMonitorStatus();
	CString*		GetSplit(CString sText, TCHAR sSplit);
	void			LoadRange();
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	public:
	virtual void OnFinalRelease();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	CString Variant(int comm, CString data = "");
	virtual ~CMainWnd();

private:
	CWnd* m_pWnd{};

	CString		m_token[100];

	std::unique_ptr<SISEWND_INFO> m_sInfo{};

	// Generated message map functions
	CBrush*	GetAxBrush(COLORREF clr);
	void    Draw(CDC *pDC, CRect rcClient);
	void	DrawBitmap(CDC *pDC, CRect rc, CString strImage);
	void	DrawRoundBitmap(CDC *pDC, CRect rc, int nKind);

	CString m_strPal;
	CString GetPalette();
	void	SetPallette();
protected:
	//{{AFX_MSG(CMainWnd)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LPARAM OnNcHitTest(CPoint point);
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

