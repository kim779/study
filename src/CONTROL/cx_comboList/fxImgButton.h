
#pragma once
// fxImgButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CfxImgButton window


class CfxImgButton : public CWnd
{
public:
    CfxImgButton(CFont* pFont = NULL);
    virtual ~CfxImgButton();

public:
    bool    m_capture, m_bChecked, m_bCheck, m_bStretch, m_bFocus, m_bDrawFocus;
    BOOL    m_bHover, m_bTracking;

    // 기존 비트맵 (그대로 유지)
    CBitmap* m_Bitmap;
    CBitmap* m_Bitmapdn;
    CBitmap* m_Bitmaphv;

    BOOL    m_bLClick;

protected:
    CFont* m_pFont;
    std::unique_ptr<CToolTipCtrl> m_pTipCtrl;
    HCURSOR m_hCursor;

private:
    BOOL        m_bText;
    COLORREF    m_clText;
    CPoint      m_posLClick;

    // PNG 추가
    CImage      m_imgNormal;
    CImage      m_imgDown;
    CImage      m_imgHover;
    BOOL        m_bUsePng = FALSE;

public:
    BOOL    Create(LPCTSTR caption, const RECT& rect, CWnd* parent, UINT nID, bool bCheck = FALSE, bool bStretch = FALSE);
    void    SetCursor(HCURSOR hcursor) { m_hCursor = hcursor; }
    void    SetImgBitmap(CBitmap* pBitmap, CBitmap* pBitmap_dn, CBitmap* pBitmap_hv);
    void    SetFont(CFont* pFont, BOOL bRedraw = true);
    void    SetCheck(bool bCheck = true);
    void    SetDrawFocus(bool bDraw = true);
    bool    IsChecked();

    // PNG 로드 추가
    BOOL    LoadPng(CString sNormal, CString sDown = "", CString sHover = "");

protected:
    CString Parser(CString& srcstr, CString substr);
    CSize   BitmapSize(HBITMAP hBitmap);

    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    virtual void PreSubclassWindow();

public:
    void    SetText(LPCTSTR caption, LPCTSTR tooltip);
    BOOL    IsShowText();
    void    ShowText(BOOL bShowText);
    void    CreateTooltip();
    void    DrawStretchByMask(CDC* pDC, CBitmap* pBitmap, CRect bRc, COLORREF maskcolor);
    void    DrawPng(CDC* pDC, CImage& img, CRect rc); // PNG 그리기 추가

protected:
    afx_msg void OnPaint();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnEnable(BOOL bEnable);
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
    afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    DECLARE_MESSAGE_MAP()
};