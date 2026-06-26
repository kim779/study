#if !defined(AFX_SCROLLWINDOW_H__4B39F7F5_1AED_45A9_8399_AE6179D603DE__INCLUDED_)
#define AFX_SCROLLWINDOW_H__4B39F7F5_1AED_45A9_8399_AE6179D603DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScrollWindow.h : header file
//

#include <afxpriv.h>

/////////////////////////////////////////////////////////////////////////////
// CScrollWindow window

/**
 * A window that features scrolling. This window does not comply with the
 * document-view architecture. Therefore, it can be embedded in any CWnd
 * object. It also resolves a problem of CScrollView that the scrolling region
 * width and height are limited to 16 bits. Besides, it maintains backward
 * compatibility with Windows 95 and Windows 3.1.
 * @author Le Hong Tu
 */
class CScrollWindow : public CWnd
{
// Construction/Destruction
public:
    CScrollWindow();
    virtual ~CScrollWindow();

// Attributes
public:

// Operations
public:
    /**
     * Sets the sizes relating to the scrolling view port in device
     * coordinates.
     */
    void SetScrollSizes(SIZE sizeTotal,
                        const SIZE& sizePage = CSize(0, 0),
                        const SIZE& sizeLine = CSize(0, 0));

    /**
     * Returns the current location of the upper corner of the scrolling view
     * port in device coordinates.
     */
    CPoint GetScrollPosition() const;

    /**
     * Returns the total size in device coordinates of the scrolling region.
     */
    CSize GetTotalSize() const;

    /**
     * Returns the some dimensions of the scrolling region. All the values are
     * in device units.
     */
    void GetScrollSizes(SIZE& sizeTotal,
                        SIZE& sizePage, SIZE& sizeLine) const;

    /**
     * Moves the scrolling view port to a position in device coordinates.
     */
    void ScrollToPosition(POINT pt);
    
    /**
     * Fills the outside of the scrolling region with the specified brush.
     */
    void FillOutsideRect(CDC* pDC, CBrush* pBrush);
    
    /**
     * Resizes the parent window so that no scroll bar is needed.
     */
    void ResizeParentToFit(BOOL bShrinkOnly = TRUE);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CScrollWindow)
    //}}AFX_VIRTUAL

    /**
     * Scrolling implementation support for OLE.
     */
    virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);

    /**
     * Scrolling implementation support for OLE.
     */
    virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);

// Implementation
protected:
    enum
    {
        CX_BORDER = 1,
        CY_BORDER = 1
    };

    static BOOL m_bGotMouseScrollLines;

    CSize m_sizeTotal;           // Total size in device units
    CSize m_sizePage;            // Per page scroll size in device units
    CSize m_sizeLine;            // Per line scroll size in device units

    BOOL m_bInsideUpdate;        // Internal state for OnSize callback

    int m_cxVScroll;
    int m_cyHScroll;

protected:
    static UINT GetMouseScrollLines();

    /**
     * Adjust scrollbars and more.
     */
    void UpdateBars();

    /**
     * A helper method which is invoked by OnMouseWheel.
     */ 
    BOOL DoMouseWheel(UINT fFlags, short zDelta, CPoint point);

    /**
     * Gets the client size not taking into account the scroll bars.
     * @return TRUE if enough room to add scrollbars if needed.
     */
    BOOL GetTrueClientSize(CSize& size, CSize& sizeSb);
    void GetScrollBarSizes(CSize& sizeSb);

    /**
     * Helper to return the state of the scrollbars without actually changing
     * the state of the scrollbars
     */
    void GetScrollBarState(CSize sizeClient, CSize& needSb,
                           CSize& sizeRange, CPoint& ptMove, BOOL bInsideClient);
    
// Overridables
protected:
    /**
     * Called by the framework before the OnDraw member function is called for
     * screen display and before the OnPrint member function is called for each
     * page during printing or print preview.
     */
    virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

    /**
     * Called by the framework to render an image of the document.
     */
    virtual void OnDraw(CDC* pDC);

    // Generated message map functions
protected:
    //{{AFX_MSG(CScrollWindow)
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg BOOL OnMouseWheel(UINT fFlags, short zDelta, CPoint point);
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnPaint();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCROLLWINDOW_H__4B39F7F5_1AED_45A9_8399_AE6179D603DE__INCLUDED_)
