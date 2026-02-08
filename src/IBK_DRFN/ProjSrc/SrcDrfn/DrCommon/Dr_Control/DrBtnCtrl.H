#if !defined(AFX_DRBTNCTRL_H__AD4C16A1_B86C_11D3_A635_00105A7C2F91__INCLUDED_)
#define AFX_DRBTNCTRL_H__AD4C16A1_B86C_11D3_A635_00105A7C2F91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ButtonST.h : header file
//

/************************************************/
/*																		          */
/*							DR Botton Control							  */
/*                                              */
/*																		          */
/*							작성일 : 2006. 07 ~							*/
/*						작성자 : 우동우 (Doori co.)		    */
/*																		          */
/************************************************/

#define STYLE_BASE        1
#define STYLE_SET         2

//버튼 MouseOverEvent Color
#define OPT_NONE  0
#define OPT_GREEN 1
#define OPT_WHITE 2
#define OPT_IMG   3

#define BTN_SELL 1
#define BTN_BUY  2

/////////////////////////////////////////////////////////////////////////////
// CDrBtnCtrl window
class CDrBtnCtrl : public CBitmapButton
{
	// Construction
public:
	CDrBtnCtrl();
	~CDrBtnCtrl();
	enum {ST_ALIGN_HORIZ, ST_ALIGN_VERT, ST_ALIGN_HORIZ_RIGHT};

public:
	BOOL m_bToggle;
	BOOL m_bIsToggleBtn;
	int	 m_nType;

	COLORREF  m_clrBgToggleNormal;
	COLORREF  m_clrBgToggleSelect;
	COLORREF  m_clrTextToggleNormal;
	COLORREF  m_clrTextToggleSelect;

	typedef struct tagBtnClrInfo
	{
		COLORREF  clrFont;
		COLORREF  clrBorder;
		COLORREF  clrBtnBkRnd;
		COLORREF  clr1DgrLTop;
		COLORREF  clr2DgrLTop;
		COLORREF  clr3DgrLTop;
		COLORREF  clr1DgrRBttm;
		COLORREF  clr2DgrRBttm;
		COLORREF  clr3DgrRBttm;
	} BtnClrInfo;
	BtnClrInfo  m_stBtnClr[4];
  
	CFont	m_fontType;
	void SetBtnColor(int nType);
	void SetBtnType(int nType);
	int	 GetBtnType() {return m_nType;};

	void OnDrawBtn(CDC* pDC, CRect& rc, /*int nBtnStyle,*/ int nStatus);
	void OnDrawText(CDC* pDC, CRect& rc, int nStatus);

	void GXPatB(CDC* pDC, int x, int y, int dx, int dy, COLORREF rgb);
	void GXPatB(CDC* pDC,const CRect& rc, COLORREF rgb);

	BOOL m_bSetImgBtn;
	CImageList m_imglistTree;
	BOOL m_bMouseover;

	void SetLoadImage(int nImgID, int nImgWidth = 0);
	void DrawItem_DrewBtn(LPDRAWITEMSTRUCT lpDIS);
	void DrawItem_FlatBtn(LPDRAWITEMSTRUCT lpDIS);
	void DrawBaseBtn(CDC* pDC, CRect& rc, int nBtnStyle, int nStatus, int nToggleState);
	void DrawGradientRect(CDC* pDC, CRect& rc, COLORREF colorStart, COLORREF colorEnd);
	void DrawGrayBtn(CDC* pDC, CRect& rc, int nBtnStyle, int nStatus, int nToggleState);

public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrBtnCtrl)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void PreSubclassWindow();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	void DrawTransparent(BOOL bRepaint = FALSE);
	
	BOOL GetDefault();
	
	void SetTooltipText(int nId, BOOL bActivate = TRUE);
	void SetTooltipText(CString& sText, BOOL bActivate = TRUE);
	void ActivateTooltip(BOOL bEnable = TRUE);
	
	BOOL SetBtnCursor(int nCursorId = -1);
	
	void SetFlatFocus(BOOL bDrawFlatFocus, BOOL bRepaint = FALSE);
	BOOL GetFlatFocus();
	
	void SetDefaultActiveFgColor(BOOL bRepaint = FALSE);
	void SetActiveFgColor(COLORREF crNew, BOOL bRepaint = FALSE);
	const COLORREF GetActiveFgColor();
	
	void SetDefaultActiveBgColor(BOOL bRepaint = FALSE);
	void SetActiveBgColor(COLORREF crNew, BOOL bRepaint = FALSE);
	const COLORREF GetActiveBgColor();
	
	void SetDefaultInactiveFgColor(BOOL bRepaint = FALSE);
	void SetInactiveFgColor(COLORREF crNew, BOOL bRepaint = FALSE);
	const COLORREF GetInactiveFgColor();
	
	void SetDefaultInactiveBgColor(BOOL bRepaint = FALSE);
	void SetInactiveBgColor(COLORREF crNew, BOOL bRepaint = FALSE);
	const COLORREF GetInactiveBgColor();
	
	void SetShowText(BOOL bShow = TRUE);
	BOOL GetShowText();
	
	void SetAlign(int nAlign);
	int GetAlign();
	
	void SetFlat(BOOL bState = TRUE);
	BOOL GetFlat();
	
	void DrawBorder(BOOL bEnable = TRUE);
	void SetIcon(int nIconInId, int nIconOutId = NULL);
	void SetIcon(HICON hIconIn, HICON hIconOut = NULL);
	void SetIcon(CString strIconInId, CString strIconOutId = "");

	BOOL SetLoadBitmaps(LPCTSTR lpszBitmapResource, LPCTSTR lpszBitmapResourceSel = NULL, LPCTSTR lpszBitmapResourceDisabled = NULL);
  	
	static const short GetVersionI();
	static const char* GetVersionC();

	void SetColor(COLORREF clrBtnBg);
	void SetColor(COLORREF clrInActiveBg, COLORREF clrInActiveFg,
		COLORREF clrActiveBg, COLORREF clrActiveFg);

	void SetToggleBtnColor(COLORREF clrBgToggleNormal, COLORREF clrBgToggleSelect,
		COLORREF clrTextToggleNormal, COLORREF clrTextToggleSelect);

	void SetMOverEventOpt(int bFlag = 0);
  
	BOOL	GetBitmapSelected(CDC* pDC, int nIndex, CBitmap* pSrc);

protected:
	//{{AFX_MSG(CDrBtnCtrl)
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();

	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	
	DECLARE_MESSAGE_MAP()
private:
	void DrawTheIcon(CDC* pDC, CString* title, RECT* rcItem, CRect* captionRect, BOOL IsPressed, BOOL IsDisabled);
	void InitToolTip();
	void PaintBk(CDC* pDC);

	int m_nMOverOpt;
	
	int m_nAlign;
	BOOL m_bShowText;
	BOOL m_bDrawBorder;
	BOOL m_bIsFlat;
	BOOL m_MouseOnButton;
	BOOL m_bDrawFlatFocus;
	
	HCURSOR m_hCursor;
	CToolTipCtrl m_ToolTip;
	
	HICON m_hIconIn;
	HICON m_hIconOut;
	BYTE m_cyIcon;
	BYTE m_cxIcon;
	
	CDC m_dcBk;
	CBitmap m_bmpBk;
	CBitmap* m_pbmpOldBk;
	BOOL m_bDrawTransparent;
	
	BOOL m_bIsDefault;
	
	COLORREF  m_crInactiveBg;
	COLORREF  m_crInactiveFg;
	COLORREF  m_crActiveBg;
	COLORREF  m_crActiveFg;

	UINT		m_nBitmapID;
};


#ifdef ST_USE_MEMDC
//////////////////////////////////////////////////
// CMemDC - memory DC
//
// Author: Keith Rule
// Email:  keithr@europa.com
// Copyright 1996-1997, Keith Rule
//
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
// History - 10/3/97 Fixed scrolling bug.
//                   Added print support.
//           25 feb 98 - fixed minor assertion bug
//
// This class implements a memory Device Context

class CMemDC : public CDC
{
public:
	
	// constructor sets up the memory DC
	CMemDC(CDC* pDC) : CDC()
	{
		ASSERT(pDC != NULL);
		
		m_pDC = pDC;
		m_pOldBitmap = NULL;
		m_bMemDC = !pDC->IsPrinting();
		
		if (m_bMemDC)    // Create a Memory DC
		{
			pDC->GetClipBox(&m_rect);
			CreateCompatibleDC(pDC);
			m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
			m_pOldBitmap = SelectObject(&m_bitmap);
			SetWindowOrg(m_rect.left, m_rect.top);
		}
		else        // Make a copy of the relevent parts of the current DC for printing
		{
			m_bPrinting = pDC->m_bPrinting;
			m_hDC       = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}
	}
	
	// Destructor copies the contents of the mem DC to the original DC
	~CMemDC()
	{
		if (m_bMemDC) 
		{    
			// Copy the offscreen bitmap onto the screen.
			m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
				this, m_rect.left, m_rect.top, SRCCOPY);
			
			//Swap back the original bitmap.
			SelectObject(m_pOldBitmap);
		} else {
			// All we need to do is replace the DC with an illegal value,
			// this keeps us from accidently deleting the handles associated with
			// the CDC that was passed to the constructor.
			m_hDC = m_hAttribDC = NULL;
		}
	}
	
	// Allow usage as a pointer
	CMemDC* operator->() {return this;}
	
	// Allow usage as a pointer
	operator CMemDC*() {return this;}
	
private:
	CBitmap  m_bitmap;      // Offscreen bitmap
	CBitmap* m_pOldBitmap;  // bitmap originally found in CMemDC
	CDC*     m_pDC;         // Saves CDC passed in constructor
	CRect    m_rect;        // Rectangle of drawing area.
	BOOL     m_bMemDC;      // TRUE if CDC really is a Memory DC.
};

#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRBTNCTRL_H__AD4C16A1_B86C_11D3_A635_00105A7C2F91__INCLUDED_)
