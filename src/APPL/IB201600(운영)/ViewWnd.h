#pragma once
// ViewWnd.h : header file
//

#include "SiseWnd.h"
#include "sharemsg.h"

/////////////////////////////////////////////////////////////////////////////
// CViewWnd window
class CMainWnd;

class CViewWnd : public CWnd
{
// Construction
public:
	CViewWnd(CMainWnd* parent, CWnd* mainWnd);

// Attributes
	CMainWnd* m_pParent{};
	CWnd* m_pMainWnd{};

	CSiseWnd* m_pSiseWnd[MATRIX_MAXCOL][MATRIX_MAXROW]{};
	std::vector<std::unique_ptr<CSiseWnd>> _vWnd;
	CArray<CRect, CRect> m_arRect;

	int m_bSiseWndOpen[MATRIX_MAXCOL][MATRIX_MAXROW]{};

//	CBrush			m_bkBrush;
	CString			m_root;
	CString			m_name;
	CString			m_user;
	CString			m_id;
	CString			m_home;
	CFont*			m_font{};
	CString			m_strPal;
	COLORREF		m_clrBgColor{};

	int				m_iRows{};
	int				m_iCols{};
	int				m_iIndex{};
	UINT				uSiseMapID{};
	int				m_nBtnIndex{};
public:

// Operations
	COLORREF		GetColor(int color);
	CString			GetPalette();
	CString			Variant(int comm, CString data = "");
	CFont*			getAxFont(CString fName, int point=9, bool bBold=false, bool bItalic=false);
	LRESULT			SendAxisMessage(WPARAM wParam, LPARAM lPAram);

	void			Draw(CDC *pDC, CRect rcClient);
	void			DrawBitmap(CDC *pDC, CRect rc, CString strImage);
	void			DrawRoundBitmap(CDC *pDC, CRect rc, int nKind);

	void			makeSiseWnd(int hoga);
	void			initSettingSiseWnd(int row, int col, int hoga=10);
	void			SettingSiseWnd(int row, int col, int hoga=10);
	void			changeSiseWnd(int row, int col,int hoga);
	void			SetPos();
	void			deleteSiseWnd();

	void			RecvData(int kind, char* data);
	
	void			saveCodeInfo();
	void			saveCodeInfo(int index,int col, int row);
	void			loadCodeInfo();
	void			chanegeTab(int index);
	void			GoanSimSettingSiseWnd(int row, int col, int hoga, int lastLineWndCount,CString* codes,int nCount);
	void			saveCodeInfo(int index);
	void			saveMarketInfo(int indexbtn);
	void			loadCodeInfo(int index);
	void			CallSnapShot();
	void			CallMarketSnapShot();

	CString         parseX(CString &srcstr, CString substr);
	inline CMainWnd*  GetParent() { return m_pParent; };
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CViewWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CViewWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

