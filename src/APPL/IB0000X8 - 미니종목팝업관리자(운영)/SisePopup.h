#pragma once

#include "AxisExt.h"
#include "Common.h"
#include "FindEdit.h"

#define IDC_CODESEARCH		1000
#define IDC_CB_NEWS			2001

extern "C" WINUSERAPI BOOL WINAPI TrackMouseEvent(LPTRACKMOUSEEVENT lpEventTrack);

class CSisePopup : public CWnd, public CAxisExt
{
public:
	CSisePopup(CWnd *pParent);

public:
	void UpdateData(CMapStringToString *pd);
	void SetFields(CArray<struct FIELD_INFO*, struct FIELD_INFO*> *pa, VIEW_TYPE vtype);
	void SetCode(LPCSTR code);
	LPCSTR GetCode() { return (LPCSTR)m_code; }
	void HoverWindow();
	void LeaveWindow();
	void UpdateFlash(DWORD tick);
	void SetTransparent(DWORD tp);
	void SetFlash(BOOL bFlash);
	void SetOwner(CWnd *pOwner) { m_pOwner = pOwner; }
	void SetSearchData(CMap<CString,LPCSTR, CStringArray*, CStringArray*> *ps) { m_search = ps; }
	void EditCode();
	void SetBkColor(COLORREF rgb);
	void SetFlashColor(COLORREF rgb);
	void SetSimpleColor(BOOL bSimple) { m_bSimpleColor = bSimple; Invalidate(FALSE);}
	void UpdateNews(CString title,CString strValue);
	void SetViewType(SISE_TYPE type);

public:
	BOOL m_bTracking, m_bLBDown, m_bFlash, m_bSimpleColor;
	DWORD m_transparent;
	int		m_nType;
	int m_offsetY;
	CRect m_rcBody;
	COLORREF m_crBack, m_crFlash, m_crSimple;
	CBitmap m_bmpBack;
	CArray<struct FIELD_INFO*, struct FIELD_INFO*> *m_fields;
	CMap<CString,LPCSTR, CStringArray*, CStringArray*> *m_search;
	CArray<BOOL, BOOL> m_flashs;
	CArray<DWORD, DWORD> m_ticks;
	CArray<CRect, CRect> m_rects, m_frects;
	CStringArray m_hist, m_sCodes, m_sNames;
	CMapStringToString *m_data;
	CODE_TYPE m_type;
	CString m_name;
	CString m_code, m_oldSearch;
	void (CSisePopup::*DrawPrice)(CDC *pDC, CRect *pRc, LPCSTR key, CString &val);
	void (CSisePopup::*DrawDiff)(CDC *pDC, CRect *pRc, LPCSTR key, CString &val);
//	std::function<void(CSisePopup& pop, CDC* pDC, CRect* pRc, LPCSTR key, CString& val)> DrawPrice;
//	std::function<void(CSisePopup& pop, CDC* pDC, CRect* pRc, LPCSTR key, CString& val)> DrawDiff;



protected:
	CWnd *m_pOwner;
	std::unique_ptr<CFindEdit> m_pEdit;
	CString m_newsData;
	CString m_newsTiTle;
	bool	m_bNews;
	CRect m_rectNews;
	CComboBox m_cbNews;
	CMapStringToString m_mapNewsHis;

protected:
	void Draw(CDC *pDC);
	void MakeBackground();
	CString ApplyComma(CString &dat);
	void Search(CString text);

protected:
	void DrawSPrice(CDC *pDC, CRect *pRc, LPCSTR key, CString &val);
	void DrawFPrice(CDC *pDC, CRect *pRc, LPCSTR key, CString &val);
	void DrawJPrice(CDC *pDC, CRect *pRc, LPCSTR key, CString &val);
	void DrawSDiff(CDC *pDC, CRect *pRc, LPCSTR key, CString &val);
	void DrawFDiff(CDC *pDC, CRect *pRc, LPCSTR key, CString &val);
	void DrawJDiff(CDC *pDC, CRect *pRc, LPCSTR key, CString &val);

	void DrawRate(CDC *pDC, CRect *pRc, LPCSTR key, const CString &val);
	void DrawVolumn(CDC *pDC, CRect *pRc, LPCSTR key, const CString &val);
	void DrawName(CDC *pDC, CRect *pRc, LPCSTR key, const CString &val);
	void DrawNews(CDC* pDC, CRect rect);


public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSisePopup)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSisePopup();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSisePopup)
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnExitSizeMove(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEnterSizeMove(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEditKeyPress(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEditKillFocus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeCode(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelChangeNews();
};

