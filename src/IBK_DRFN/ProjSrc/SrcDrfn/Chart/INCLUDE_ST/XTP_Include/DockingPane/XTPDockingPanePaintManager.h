// XTPDockingPanePaintManager.h : interface for the CXTPDockingPanePaintManager class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// ?004 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO 
// BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED 
// WRITTEN CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS 
// OUTLINED IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT.  CODEJOCK SOFTWARE 
// GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE 
// THIS SOFTWARE ON A SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

//:Ignore
#if !defined(__XTPDOCKINGPANEPAINTMANAGER_H__)
#define __XTPDOCKINGPANEPAINTMANAGER_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

#include "XTPDockingPaneDefines.h"
#include "XTPWinThemeWrapper.h"

//forwards

class CXTPDockingPaneTabsArray;
class CXTPDockingPaneTabbedContainer;
class CXTPDockingPaneAutoHidePanel;
class CXTPDockingPaneHiddenTabsArray;

struct XTP_DOCKINGPANE_TAB;

#define XTP_DB_HIDE		0x0002			// Pin button.
#define XTP_DB_CLOSE	0x0004			// Close button.
#define XTP_DB_PRESSED	0x1000			// Button is pressed.
#define XTP_DB_OVER		0x4000			// Cursor is over button

#define XTP_DB_ISPRESSED(Style)     (((Style) & XTP_DB_PRESSED) !=0 )			// Is button pressed.
#define XTP_DB_ISOVER(Style)        (((Style) & XTP_DB_OVER) == XTP_DB_OVER)	// Is cursor over the button.

//////////////////////////////////////////////////////////////////////
// Summary: CXTPDockingPaneCaptionButton is a stand alone internal class.
class _XTP_EXT_CLASS CXTPDockingPaneCaptionButton
{
public:
	// Summary: Constructs a CXTPDockingPaneCaptionButton object
	CXTPDockingPaneCaptionButton (DWORD wStyle) { m_wStyle = wStyle;m_pPane = 0;	}
	
	// Input:	pDC - Pointer to the device context in which to draw. 
	//			clrButton - Button color.
	//			pPane - Button owner.
	// Summary: Call this member function to draw a caption button.
	void Draw (CDC* pDC, COLORREF clrButton);

	// Input:	rc - Button's rectangle.
	// Summary: Call this member to set the button's rectangle.
	void SetRect (CRect rc);

	// Input:	pDC - Pointer to the device context in which to draw.
	//			pt - Center point.
	//			bPinned - TRUE if the button is pinned.
	// Summary: Call this member to draw a pin button.
	static void DrawPinnButton(CDC* pDC, CPoint pt, BOOL bPinned);

	// Input:	pWnd - Parent window of button.
	//			pt - Initial mouse position.
	// Summary: Call this method to click button.
	DWORD Click (CWnd* pWnd, CPoint pt);
	
	// Returns: A CRect object
	// Summary: Call this member to get the bounding rectangle 
	CRect GetRect() { return m_Rect;}
	
	// Input:	pt - Point to test.
	// Returns: TRUE if point belongs to the button; otherwise returns FALSE
	// Summary: Call this member to determine if the point belongs to button.
	BOOL PtInRect (POINT pt) const {return m_Rect.PtInRect (pt) != 0; }
	
	// Returns: Style of the button.
	// Summary: Call this member to get styles of the button.
	DWORD GetStyle() {return m_wStyle; }

	// Summary: Call this member to refresh styles of the button.
	void CheckForMouseOver (CPoint pt);

private:
	CRect m_Rect;
	DWORD m_wStyle;
	CXTPDockingPaneTabbedContainer* m_pPane;

private:
	friend class CXTPDockingPaneTabbedContainer;
};

//////////////////////////////////////////////////////////////////////
// Summary: CXTPDockingPanePaintManager is a stand alone class. 
//			Override some virtual functions to get a new visual theme.
class _XTP_EXT_CLASS CXTPDockingPanePaintManager
{

public:

	// Input:	dc - Reference to the device context in which to draw.
	//			pPane - Pointer to the tabbed container.
	//			rc - Client rectangle of the tabbed container.
	// Summary: Override this member function to draw a tabbed container.
	virtual void DrawPane(CDC& dc, CXTPDockingPaneTabbedContainer* pPane, CRect rc);

	// Input:	dc - Reference to the device context in which to draw.
	//			lst - Tabs Collection.
	//			rc - Client rectangle of the tabbed container.
	// Summary: Override this member function to draw a tab bar.
	virtual void DrawTabs(CDC& dc, const CXTPDockingPaneTabsArray* lst, CRect rc) = 0;

	// Input:	dc - Reference to the device context in which to draw.
	//			pPane - Pointer to the tabbed container.
	//			rc - Client rectangle of the tabbed container.
	// Summary: Override this member function to draw a caption.
	virtual void DrawCaption(CDC& dc, CXTPDockingPaneTabbedContainer* pPane, CRect rc) = 0;

	// Summary: This method is called to refresh the visual metrics of manager.
	virtual void RefreshMetrics();

	// Input:	pPane - Pointer to the tabbed container. 
	//			rect - Client rectangle to be changed.
	// Summary: Override this member to change the client rectangle of the child docking pane.
	virtual void AdjustClientRect(CXTPDockingPaneTabbedContainer* pPane, CRect& rect);

	// Input:	pPane - Pointer to the tabbed container. 
	//			rect - Client rectangle of the tabbed container.
	// Summary: Override this member to change the default position of the tabs. 
	virtual void AdjustTabs(CXTPDockingPaneTabbedContainer* pPane, CRect rect);

	// Input:	dc - Reference to the device context in which to draw.
	//			pPanel - Pointer to the auto-hide container.
	//			rc - Client rectangle of the auto-hide container.
	//			bIndent - TRUE if an indent is needed.
	// Summary: Override this member to draw an auto-hide panel. 
	virtual void DrawAutoHidePanel(CDC& dc, CXTPDockingPaneAutoHidePanel* pPanel, CRect rc, BOOL bIndent) = 0;

	// Input:	dc - Reference to the device context in which to draw.
	//			pContainer - Pointer to the tabbed container.
	//			rc - Drawing Rectangle.
	//			direction - Direction of the auto-hide container.
	//			tabs - Tabs collection.
	// Summary: Override this member to draw a group of tabs in an auto-hide panel. 
	virtual void DrawHiddenTabs(CDC& dc, CXTPDockingPaneTabbedContainer* pContainer, CRect& rc, XTPDockingPaneDirection direction, CXTPDockingPaneHiddenTabsArray& tabs) = 0;

public:

	// Input:	pDC - Pointer to a valid device context
	//			x0 - Specifies the logical x-coordinate of the start position. 
	//			y0 - Specifies the logical y-coordinate of the start position. 
	//			x1 - Specifies the logical x-coordinate of the endpoint for the line. 
	//			y1 - Specifies the logical y-coordinate of the endpoint for the line. 
	//			nPen - Specifies the color used to paint the line.
	// Summary: Draws a line.
	static void Line(CDC* pDC, int x0, int y0, int x1, int y1, int nPen);
	
	// Input:	pDC - Pointer to a valid device context
	//			x - Specifies the logical x-coordinate of the point to be set. 
	//			y - Specifies the logical y-coordinate of the point to be set. 
	//			nPen -  Specifies the color used to paint the point
	// Summary: Sets the pixel at the point.
	static void Pixel(CDC* pDC, int x, int y, int nPen);
	
	// Input:	pDC - Pointer to a valid device context
	//			rc - Specifies the rectangle in logical units.
	//			nPenEntry - Specifies the color used to paint the rectangle.
	//			nPenBorder - Specifies the color used to fill the rectangle.
	// Summary: Draws a rectangle.
	static void Rectangle(CDC* pDC, CRect rc, int nPenEntry, int nPenBorder);

    // Input:	bUseOfficeFont - TRUE to use office font.
	// Summary: Call this member to use office (Tahoma) font.
	void UseOfficeFont(bool bUseOfficeFont);
    
	// Summary: This member is called to reallocate fonts.
	void FreeSysFont();


protected:

	// Summary: Constructs a CXTPDockingPanePaintManager object
	CXTPDockingPanePaintManager(void);
	
	// Summary: Destroys a CXTPDockingPanePaintManager object, handles cleanup and de-allocation
	virtual ~CXTPDockingPanePaintManager(void);

    // Summary: Log font helper.
	struct CXTPLogFont : public LOGFONT {
		// Summary: Constructs a CXTPLogFont object
        CXTPLogFont(bool bIconTitle=false);
    };

    // Summary: Client Metrics helper.
	struct CXTPNonClientMetrics : public NONCLIENTMETRICS {
		// Summary: Constructs a CXTPNonClientMetrics object
        CXTPNonClientMetrics();
    };

	// Input:	strFaceName - Font need to test.
	// Returns: True if font exists; otherwise returns false
    // Summary: Call this member to check if the font exists in the system.
	bool FontExists(CString& strFaceName);
	
	// Input:	dc - Reference to device context to draw
	//			rcItem - Bounding rectangle.
	//			strTitle - Text of the tab.
	//			hIcon - Icon handle.
	// Summary: Call this member to draw single tab.
	void DrawTabIconText(CDC& dc, CRect rcItem, CString strTitle, HICON hIcon);
	
	// Input:	dc - Reference to device context to draw
	//			direction - Direction of the tab.
	//			bSelected - TRUE if selected.
	//			rcTab - Bounding rectangle.
	//			strTitle - Text of the tab.
	//			hIcon - Icon handle.
	// Summary: Call this member to draw single hidden tab.
	void DrawHiddenTabIconText(CDC& dc, XTPDockingPaneDirection direction, BOOL bSelected, CRect rcTab, CString strTitle, HICON hIcon);

public:
	int   m_nTitleHeight;    // Title's height.
	int	  m_nTabsHeight;     // Tab bar's height.
	int   m_nPanelsHeight;   // Auto-hide panel's height.
	CFont m_fntTitle;        // Title font.
	CFont m_fntIcon;         // Tabs font.
	CFont m_fntIconVertical; // Vertical tabs font.
    bool  m_bUseOfficeFont;  // TRUE to use Tahoma font.

protected:
	XTPDockingPanePaintTheme m_themeCurrent;	// Current theme.
    CString m_strOfficeFont;					// Office font name.


private:
	friend CXTPDockingPanePaintManager* XTPDockingPanePaintManager();	
	friend class CXTPDockingPaneManager;								
};


//:Ignore
namespace XTPDockingPanePaintThemes
{
	class _XTP_EXT_CLASS CXTPDockingPaneDefaultTheme : public CXTPDockingPanePaintManager
	{
	public:
		virtual void DrawTabs(CDC& dc, const CXTPDockingPaneTabsArray* lst, CRect rc);
		virtual void DrawCaption(CDC& dc,CXTPDockingPaneTabbedContainer* pPane, CRect rc);
		virtual void DrawAutoHidePanel(CDC& dc, CXTPDockingPaneAutoHidePanel* pPanel, CRect rc, BOOL bIndent);	
		virtual void DrawHiddenTabs(CDC& dc, CXTPDockingPaneTabbedContainer* pContainer, CRect& rc, XTPDockingPaneDirection direction, CXTPDockingPaneHiddenTabsArray& tabs);
	
	protected:
		void _DrawPinnButton(CDC& dc, CRect rc, BOOL bPushed, BOOL bPinned);
	};

	class _XTP_EXT_CLASS CXTPDockingPaneGripperedTheme : public CXTPDockingPaneDefaultTheme
	{
	public:
		virtual void DrawCaption(CDC& dc,CXTPDockingPaneTabbedContainer* pPane, CRect rc);
	};

	class _XTP_EXT_CLASS CXTPDockingPaneOfficeTheme : public CXTPDockingPanePaintManager
	{
	public:
		virtual void DrawTabs(CDC& dc, const CXTPDockingPaneTabsArray* lst, CRect rc);
		virtual void DrawCaption(CDC& dc, CXTPDockingPaneTabbedContainer* pPane, CRect rc);
		virtual void DrawAutoHidePanel(CDC& dc, CXTPDockingPaneAutoHidePanel* pPanel, CRect rc, BOOL bIndent);
		virtual void DrawHiddenTabs(CDC& dc, CXTPDockingPaneTabbedContainer* pContainer, CRect& rc, XTPDockingPaneDirection direction, CXTPDockingPaneHiddenTabsArray& tabs);

	};

	class _XTP_EXT_CLASS CXTPDockingPaneNativeXPTheme : public CXTPDockingPaneOfficeTheme
	{
	public:
		CXTPDockingPaneNativeXPTheme();
		virtual void RefreshMetrics();
		virtual void DrawCaption(CDC& dc, CXTPDockingPaneTabbedContainer* pPane, CRect rc);

	protected:
		CXTPWinThemeWrapper m_themeWindow;
	};	

	class _XTP_EXT_CLASS CXTPDockingPaneVisioTheme : public CXTPDockingPaneOfficeTheme
	{
	public:
		CXTPDockingPaneVisioTheme();
		void RefreshMetrics();
		void DrawPane(CDC& dc, CXTPDockingPaneTabbedContainer* pPane, CRect rc);
		void AdjustClientRect(CXTPDockingPaneTabbedContainer* pPane, CRect& rect);
		void AdjustTabs(CXTPDockingPaneTabbedContainer* pPane, CRect rect);
		void DrawTabs(CDC& dc, const CXTPDockingPaneTabsArray* lst, CRect rc);
		void DrawCaption(CDC& dc, CXTPDockingPaneTabbedContainer* pPane, CRect rc);
	
	public:
		int _DrawButtonFrame(CDC& dc, CRect rc, DWORD dwStyle);
		BOOL m_bDrawTabsIcons;
	protected:
		CBitmap m_bmpGripper;
	};
	
	class _XTP_EXT_CLASS CXTPDockingPaneOffice2003Theme : public CXTPDockingPaneOfficeTheme  
	{
	public:
		CXTPDockingPaneOffice2003Theme();
		virtual ~CXTPDockingPaneOffice2003Theme();

		virtual void RefreshMetrics();
		virtual void DrawCaption(CDC& dc, CXTPDockingPaneTabbedContainer* pPane, CRect rc);		
		virtual void DrawTabs(CDC& dc, const CXTPDockingPaneTabsArray* lst, CRect rc);
		virtual void DrawHiddenTabs(CDC& dc, CXTPDockingPaneTabbedContainer* pContainer, CRect& rc, XTPDockingPaneDirection direction, CXTPDockingPaneHiddenTabsArray& tabs);
		virtual void DrawAutoHidePanel(CDC& dc, CXTPDockingPaneAutoHidePanel* pPanel, CRect rc, BOOL bIndent);
		
		COLORREF GetColor(XTP_DOCKINGPANE_TAB* pTab);
		COLORREF DrawButton(CDC* pDC, CRect rc, COLORREF clrDark, BOOL bFocused, COLORREF clrBorder, COLORREF clrLeftHighlight, COLORREF clrTopHighlight);

		int _DrawButtonFrame(CDC& dc, CRect rc, DWORD dwStyle);

	protected:
		COLORREF m_clrCommandBarLight;
		COLORREF m_clrCommandBarDark;

		COLORREF m_clrBackgroundLight;
		COLORREF m_clrBackgroundDark;
		COLORREF m_clrBorderSelected;
		COLORREF m_clrBorder;
		COLORREF m_clrBorderLeftHighlight;
		COLORREF m_clrBorderTopHighlight;
		COLORREF m_clrHighlight;
		
		COLORREF m_clrPanelTab;
		COLORREF m_clrPanelTabSelected;

		COLORREF m_clrDockBarDark;
		COLORREF m_clrDockBarLight;

		BOOL m_bLunaTheme;
	};

};
using namespace XTPDockingPanePaintThemes;
//:End Ignore

#endif // #if !defined(__XTPDOCKINGPANEPAINTMANAGER_H__)