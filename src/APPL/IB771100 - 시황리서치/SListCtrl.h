#pragma once
// SListCtrl.h : header file
//

struct NewsInfo
{
	CString sGisn; // CISAM RECORD NO.(GRID)
	CString sSisn; // CISAM RECORD NO.(본문조회)
	CString sKeyv; // 본문조회용 key  :  일자(8)+정보구분(2)+분류코드(4)+일련번호(10)
	CString sCode; // 종목코드
	CString sFlag; // HTML 여부 (TEXT: 0, HTML: 1)

	NewsInfo(CString sData1, CString sData2, CString sData3, CString sData4, CString sData5)
	{
		sGisn = sData1;
		sSisn = sData2;
		sKeyv = sData3;
		sCode = sData4;
		sFlag = sData5;
	};
	NewsInfo(){};
};
typedef CArray<NewsInfo, NewsInfo> CNewsInfoArray;

/////////////////////////////////////////////////////////////////////////////
// CSListCtrl window
#include "SHeaderCtrl.h"
#include "TipDlg.h"	


#define WM_REQUEST_NEXT WM_USER + 2007

class CSListCtrl : public CListCtrl
{
// Construction
public:
	CSListCtrl();
	
	int			m_nSelectedItem;
	COLORREF	m_clrBk1;
	COLORREF	m_clrBk2;
	COLORREF	m_clrText;
	COLORREF	m_clrSelectText;
	COLORREF	m_clrSelectBk;
	COLORREF	m_clrEdge;
	
	CSHeaderCtrl m_HdCtrl;
	int		m_nHideColIdx;
	CMapDlg		*m_pMapDlg;
	int			m_iNewsType, m_iTypeField;
	
	HBITMAP		m_hTypeBitmap[4];

	//스크랩 표시용
	LVHITTESTINFO ht{};
	std::unique_ptr<CTipDlg>	m_pTooltip;
	CNewsInfoArray	m_arNewsInfos;

// Attributes
public:
	void HideColumnOver(int nColIdx);
	inline void SetParentWnd(CMapDlg* mapdlg){
		m_pMapDlg = mapdlg;
		m_HdCtrl.SetParentWnd(mapdlg);
	}
	inline void SetBkColor(COLORREF bk,COLORREF bk1){
		m_clrBk1 = bk;m_clrBk2 = bk1;
	}
	inline void SetEdgeColor(COLORREF clr){
		m_clrEdge = clr;
	}
	inline void SetSelectColor(COLORREF clr){
		m_clrSelectBk = clr;
	}
	inline void SetTextColor(COLORREF clr){
		m_clrText = clr;
	}	
	void InvalidateHedarCtrl() ;

	void SetnnFont( CFont* m_pFont )
	{
		m_pnn = m_pFont;
	}
	
	int GetSelectedItem();
	void GetDrawColors(int nItem,
							   int nSubItem,
							   COLORREF& colorText,
							   COLORREF& colorBkgnd,
							   bool bSelect = true);

	void DrawText(int nItem,
						  int nSubItem,
						  CDC *pDC,
						  COLORREF crText,
						  COLORREF crBkgnd,
						  CRect& rect );
	void SetScroll(int index, bool bSel);
	void DrawColorText(CDC* pDC, CString sTxt, CRect& rc, COLORREF tcolor);

	void SetNewsType(int itype){m_iNewsType = itype;};
	void SetImageType(HBITMAP hbitmap1, HBITMAP hbitmap1_dn, HBITMAP hbitmap2, HBITMAP hbitmap2_dn);
	void DrawTypeImage(CDC *pDC, const CRect& rc);
	void SetNewsTypeColumn(int icol){m_iTypeField=icol;};
	void SetNewsInfo(int index, 	CString sGisn,	CString sSisn,	CString sKeyv,	CString sCode,	CString sFlag);
	bool GetNewsInfo(int index, NewsInfo& pnewsInfo);
	void DeleteAllNewsInfo(){m_arNewsInfos.RemoveAll();};

	BOOL IsScroll( void );

	void DrawNumberBackground( CRect* prc, CDC* paint );

private:	
	DWORD m_iInterval;

	CBitmap m_bmpNumberBack;
	CFont	m_fontNumber;	
	CFont*	m_pnn;			// normalfont


// Operations
public:
//	void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
//  Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSListCtrl)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSListCtrl)
	afx_msg void OnPaint();
	afx_msg void OnItemchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) ;	
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

