
#pragma once
// SiseWnd.h : header file
//

#include "HogaWnd.h"
//#include "SComboBox1.h"
//#include "IHCodeCtrl.h"
#include "../../control/fx_misc/fxCodeCtrl.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "../../h/jmcode.h"
#include "../../h/axisfire.h"

#include "sharemsg.h"

/////////////////////////////////////////////////////////////////////////////
// CSiseWnd window
enum sendtr_type {
	type_gongsi = 0,			// 공시
	type_hoga = 0,
	type_symbol
};

class CViewWnd;

#include "afxole.h"

class CSiseWnd;
/////////////////////////////////////////////////////////////////////////////
// COleEditDropTarget
class CDropTarget : public COleDropTarget
{
	// Construction
public:
	CDropTarget();

	CSiseWnd* m_pSiseWnd{};
	// Implementation
public:
	CString Parser(CString& srcstr, CString substr);
	void GetSiseWnd(CSiseWnd* cWnd);
	virtual ~CDropTarget();

	//
	// These members MUST be overridden for an OLE drop target
	// See DRAG and DROP section of OLE classes reference
	//
	DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD
		dwKeyState, CPoint point);
	DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD
		dwKeyState, CPoint point);
	void OnDragLeave(CWnd* pWnd);

	BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT
		dropEffect, CPoint point);
};

class CSiseWnd : public CWnd
{
	// Construction
public:
	CSiseWnd(CViewWnd* parent, CWnd* mainWnd);
	CSiseWnd(CViewWnd* parent, CWnd* mainWnd, int hogatype, int col, int row);

	std::unique_ptr<class CPrice> m_price;

	// Attributes
	CDropTarget	m_dropTarget;

	CViewWnd* m_pParent;
	CWnd* m_pMainWnd;
	class CContainerWnd* m_pMarketpicker{};

	int			m_iRowSiseWnd;
	int			m_iColSiseWnd;

	int			m_mapKey;
	std::unique_ptr<CHogaWnd> m_pHogaWnd;

	CFont* m_font;
	CString			m_strPal;
	CString			m_root;
	CString			m_name;
	CString			m_szCode;

	HINSTANCE		m_hiSymbol;				//종목명 심벌용
	CWnd* m_pwndSymbol;
	HINSTANCE		m_hiHoga;				//호가용
	CWnd* m_pwndHoga;
	HINSTANCE		m_hiCategory;				//호가용
	CWnd* m_pwndCategory;
	HINSTANCE		m_hiCodectrl;
	BOOL			m_bPercent;
	BOOL			m_bDragMode;


	std::unique_ptr<CfxCodeCtrl>	m_code;
//	CfxImgButton	m_btnSearch;
	std::unique_ptr <CfxImgButton> m_btnJM;
	std::unique_ptr <CfxImgButton> m_btnMaesu;
	std::unique_ptr <CfxImgButton> m_btnMaedo;
	CStringArray	m_SearchWordArray;		// 검색 데이타

	int				m_iHogaType;			//호가 타입
	HBITMAP			m_bitmap_nm, m_bitmap_dn, m_bitmap_hv;

	struct		_param	symbolparam;
	struct		_param	codeparam;
	struct		_param	cateparam;

	HBITMAP			m_map_nm, m_map_dn, m_map_hv;
	HBITMAP			m_map2_nm, m_map2_dn, m_map2_hv;

public:
	// Operations
	void		SetCode(CString code);
	CString		GetCode();
	inline int  GetMap10Key() { return m_mapKey; };

	void		getIndexSiseWnd(int col, int row);
	void		setIndexSiseWnd(int col, int row);

	CFont* getAxFont(CString fName, int point = 9, bool bBold = false, bool bItalic = false);
	COLORREF	GetColor(int color);
	CBitmap* getAxBitmap(CString path);
	LRESULT		SendAxisMessage(WPARAM wParam, LPARAM lPAram);
	CString		GetPalette();
	CString		GetAxCode(int cmd, LPARAM lParam);

	void		SendHogaWnd();
	void		SendChangeTab(int index);

	void		showHoga(int hogatype);

	void		init();
	void        InitSymbol(CRect rect, int Row, int Col);
	void		InitHoga(CRect rect);
	//	void		InitCategory(CRect rect);
	//	void		InitCodeCtrl(CRect rect);

	void		loadHoga(int hogatype, CHogaWnd* hogaWnd);
	void		closeMap();

	CString		Variant(int comm, CString data = "");

	void	    Draw(CDC* pDC, CRect rcClient);
	void		DrawBitmap(CDC* pDC, CRect rc, CString strImage);
	void		DrawRoundBitmap(CDC* pDC, CRect rc, int nKind);

	CString		Parser(CString& srcstr, CString substr);
	void		RequestJMInfo();
	void		sendSymbol(CString code);
	void		sendHoga(CString code);
	void		sendOOP(int key, char* datb, int datl);

	void		parsingSymbol(char* datb);
	void		parsingHoga(char* datb);
	void		SetRangePercent(int nHigh, int nLow);
	void		SetEnablePrice(bool flag);
	void		LoadPriceRange();
public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CSiseWnd)
public:
	virtual void OnFinalRelease();
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSiseWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSiseWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelendokComboSearch();
	afx_msg void OnButtonJongmok();
	afx_msg void OnButtonMaesu();
	afx_msg void OnButtonMaedo();
	afx_msg LRESULT OnRangePrice(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnMsgMKDLL(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHistoryCode();
	afx_msg LONG OnPriceRange(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CSiseWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	CString m_szMarket{};
	CString GetMarket();
	void SetMarket(CString sMarket);
	CString m_sROWCOL{};
	BOOL m_bReady{};
	BOOL m_bInit{};
};

