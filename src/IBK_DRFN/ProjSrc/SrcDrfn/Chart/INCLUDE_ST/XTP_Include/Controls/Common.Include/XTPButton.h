// XTPButton.h : interface for the CXTPButton class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
// (c)1998-2009 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPBUTTON_H__)
#define __XTPBUTTON_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPImageManager;
class CXTPImageManagerIcon;
class CXTPMarkupContext;
class CXTPMarkupUIElement;
class CXTPButtonTheme;
class CXTPImageManagerIconHandle;

#ifndef CXTPButtonBase
#define CXTPButtonBase CButton
#endif

// -------------------------------------------------------------------
// Summary:
//     Image and text relationship settings for a pushbutton control.
// Remarks:
//     The CXTPButton::SetTextImageRelation method uses flags from the XTPButtonTextImageRelation
//     enumeration to set the text and image relationship inside the button control.
// -------------------------------------------------------------------
enum XTPButtonTextImageRelation
{
	xtpButtonOverlay, // Image is displayed over top of the text.
	xtpButtonImageAboveText, // Image is displayed above the text.
	xtpButtonTextAboveImage, // Text is displayed above the image.
	xtpButtonImageBeforeText, // Image is displayed before\to the left of the text.
	xtpButtonTextBeforeImage // Text is displayed before\to the left of the image.
};

// -------------------------------------------------------------------
// Summary:
//     PushButton styles.
// Remarks:
//     The CXTPButton::SetPushButtonStyle method uses flags from the XTPPushButtonStyle
//     enumeration to set the style of the button.
// -------------------------------------------------------------------
enum XTPPushButtonStyle
{
	xtpButtonNormal, // Normal push button.
	xtpButtonDropDown, // Dropdown button.  Button will have an arrow indication it has a dropdown.  The dropdown will appear under the button.
	xtpButtonDropDownRight, // Dropdown button.  Button will have an arrow indication it has a dropdown.  The dropdown will appear to the right of the button.
	xtpButtonSplitDropDown // Split button.  Button will be split into a "button" part and "arrow" part.  The "arrow" part will have an arrow indication it has a dropdown.  The dropdown will appear under the button when t he "arrow" part is clicked.
};

// -------------------------------------------------------------------
// Summary:
//     Paint styles of a control.
// Remarks:
//     The CXTPButton::SetTheme method uses flags from the XTPButtonTheme
//     enumeration to set the appearance of the button.
// -------------------------------------------------------------------
enum XTPButtonTheme
{
	xtpButtonThemeStandard, // Standard appearance style.
	xtpButtonThemeFlat, // Flat appearance style.
	xtpButtonThemeUltraFlat, // Ultra flat appearance style.
	xtpButtonThemeOffice2000, // Office 2000 appearance style.
	xtpButtonThemeOfficeXP, // Office XP appearance style.
	xtpButtonThemeOffice2003, // Office 2003 appearance style.
	xtpButtonThemeOffice2007 // Office 2007 appearance style.
};

// -------------------------------------------------------------------
// Summary:
//     Gets or sets the border style of the control.
// Remarks:
//     There are 3 different border styles for the control which are
//     single line with caption, complete border with caption and no
//     border or caption.  CXTPButton::SetBorderStyle method stores a
//     value from the XTPGroupBoxBorderStyle enumeration to specify
//     the border style of the control.
// -------------------------------------------------------------------
enum XTPGroupBoxBorderStyle
{
	xtpGroupBoxBorder, // Complete frame with caption.
	xtpGroupBoxSingleLine, // Single line with caption.
	xtpGroupBoxNone // No frame or caption.
};

//===========================================================================
// Summary:
//     Push Button control.
//===========================================================================
class _XTP_EXT_CLASS CXTPButton : public CXTPButtonBase
{
	DECLARE_DYNAMIC(CXTPButton)

public:
	// ----------------------------------------
	// Summary:
	//     Constructs a CXTPButton object
	// ----------------------------------------
	CXTPButton();

	// -----------------------------------------------------------
	// Summary:
	//     Destroys a CXTPButton object, handles cleanup and
	//     deallocation
	// -----------------------------------------------------------
	virtual ~CXTPButton();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the button styles for this object. This function returns only the BS_ style values, not any of the other window styles.
	// Returns: Returns BS_ style for this button.
	//-----------------------------------------------------------------------
	BYTE GetButtonStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the the button is currently being pushed.
	// Returns:
	//     TRUE if the button is currently being pushed, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsPushed();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the the button is currently being highlighted (mouse over).
	// Returns:
	//     TRUE if the button is currently being highlighted (mouse over), FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsHighlighted();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the checked state has
	//     been defined for the button control.
	// Returns:
	//     TRUE if the checked state is defined for the button control,
	//     otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL GetChecked();

	// ------------------------------------------------------------------------
	// Summary:
	//     Sets the highlight state for the button control.
	// Parameters:
	//     bChecked -  Specifies whether the button is to be checked. TRUE will
	//                 check the button. FALSE will uncheck it.
	// Remarks:
	//     This member function sets the highlighting state of the button
	//     control. This is the same as its CButton counterpart, except the
	//     m_bChecked flag is set. This is done because MFC does not recognize
	//     the button as being checked unless it is a radio or check box.
	// ------------------------------------------------------------------------
	void SetChecked(BOOL bChecked);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the flat style appearance of the button control.
	// Remarks:
	//     The control will appear flat until the mouse pointer moves over it,
	//     at which point it appears three-dimensional.
	// Returns:
	//     TRUE if the flat style is used, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL GetFlatStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the flat style appearance of the button control.
	// Parameters:
	//     bFlatStyle - TRUE to use the flat style, FALSE otherwise.
	// Remarks:
	//     The control will appear flat until the mouse pointer moves over it,
	//     at which point it appears three-dimensional.
	//-----------------------------------------------------------------------
	void SetFlatStyle(BOOL bFlatStyle = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the control is drawn using visual styles\SkinFramework, if supported.
	// Remarks:
	//     This method enables Windows XP visual styles for the control. This
	//     Control will draw with visual styles if the control and the operating
	//     system supports it.
	//     Visual styles and SkinFramwork skins are specifications for the
	//     appearance of controls.  GetUseVisualStyle determines whether to
	//     use the currently set Windows XP visual style.  If using SkinFramwork,
	//     it determines whether to use the currently set style of the SkinFramework.
	// Returns:
	//     TRUE if the currently set Windows XP visual style will be used to
	//     theme the control.  If using SkinFramwork, the currently set style
	//     of the SkinFramework will be used to skin the control.
	//
	//     FALSE if the currently set appearance\style in the OS will be used.
	//-----------------------------------------------------------------------
	BOOL GetUseVisualStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables Windows XP visual styles for the control.
	// Remarks:
	//     This method enables Windows XP visual styles for the control. This
	//     Control will draw with visual styles if the control and the operating
	//     system supports it.
	//     Visual styles and SkinFramwork skins are specifications for the
	//     appearance of controls.  GetUseVisualStyle determines whether to
	//     use the currently set Windows XP visual style.  If using SkinFramwork,
	//     it determines whether to use the currently set style of the SkinFramework.
	// Parameters:
	//     bUseVisualStyle - TRUE if the currently set Windows XP visual style will be used to
	//                       theme the control.  If using SkinFramwork, the currently set style
	//                       of the SkinFramework will be used to skin the control.
	//
	//                       FALSE if the currently set appearance\style in the OS will be used.
	//-----------------------------------------------------------------------
	void SetUseVisualStyle(BOOL bUseVisualStyle = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine the gap between the
	//     button image and the button's edge.
	// Returns:
	//     The amount in pixels between the button image and border.
	//-----------------------------------------------------------------------
	long GetBorderGap() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the gap between the button's edge
	//     and the image.
	// Parameters:
	//     nBorderGap - Amount in pixels of the gap between the button edge and
	//            the image.
	// Returns:
	//     The previous border gap value.
	//-----------------------------------------------------------------------
	void SetBorderGap(int nBorderGap);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function returns a flag from the XTPPushButtonStyle enumeration
	//     the show the current style of the button.
	// Returns:
	//     The button styles for this CXTPButton object.
	//-----------------------------------------------------------------------
	XTPPushButtonStyle GetPushButtonStyle() const;

	// ----------------------------------------------------------------------------
	// Summary:
	//     This member function will set the display style for the button.
	// Parameters:
	//     nPushButtonStyle -  Specifies the button style. The button style can be defined
	//                 as one or more of the styles listed in the Remarks section.
	// Remarks:
	//     The Button can be set to and value from the XTPPushButtonStyle enumeration.
	// See Also:
	//     GetPushButtonStyle
	// ----------------------------------------------------------------------------
	void SetPushButtonStyle(XTPPushButtonStyle nPushButtonStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the alignment of the text on the button control.
	// Returns:
	//     Alignment of the text on the button control.
	//-----------------------------------------------------------------------
	virtual int GetTextAlignment() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the alignment of the text on the button control.
	// Parameters:
	//     dwAlignment - Alignment of the text on the button control.
	//-----------------------------------------------------------------------
	void SetTextAlignment(DWORD dwAlignment);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the alignment of the image on the button control.
	// Returns:
	//     Alignment of the image on the button control.
	//-----------------------------------------------------------------------
	virtual int GetImageAlignment() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the alignment of the image on the button control.
	// Parameters:
	//     dwAlignment - Alignment of the image on the button control.
	//-----------------------------------------------------------------------
	void SetImageAlignment(DWORD dwAlignment);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the text and image relation in the button.
	// Returns:
	//     Flag from the XTPButtonTextImageRelation enumeration that
	//     specifies the text and image relation in the button
	//-----------------------------------------------------------------------
	XTPButtonTextImageRelation GetTextImageRelation() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the text and image relation in the button.
	// Parameters:
	//     realtion - Flag from the XTPButtonTextImageRelation enumeration that
	//                specifies the text and image relation in the button
	//-----------------------------------------------------------------------
	void SetTextImageRelation(XTPButtonTextImageRelation realtion);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine the gap between the
	//     button text and the image.
	// Returns:
	//     The amount in pixels between the button text and image displayed.
	//-----------------------------------------------------------------------
	long GetImageGap() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the gap between the button's text
	//     and the image.
	// Parameters:
	//     nImageGap - Amount in pixels of the gap between the button text and
	//            the image.
	// Returns:
	//     The previous border gap value.
	//-----------------------------------------------------------------------
	void SetImageGap(int nImageGap);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the size of the image
	//     displayed for the button control.
	// Returns:
	//     A CSize object.
	//-----------------------------------------------------------------------
	virtual CSize GetImageSize();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return a pointer to the image
	//     manager for the button control.
	// Returns:
	//     A pointer to a CXTPImageManagerIcon object.
	//-----------------------------------------------------------------------
	CXTPImageManagerIcon* GetIcon() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the image of the control.
	// Parameters:
	//     pDC           - Pointer to a valid device context.
	//     rc            - Size of the image.
	//-----------------------------------------------------------------------
	virtual void DrawImage(CDC* pDC, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tborder style of the control.
	// Returns:
	//     Flag from the XTPGroupBoxBorderStyle enumeration that
	//     specifies the border style of the control.
	//-----------------------------------------------------------------------
	XTPGroupBoxBorderStyle GetBorderStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tborder style of the control.
	// Parameters:
	//     style - Flag from the XTPGroupBoxBorderStyle enumeration that
	//             specifies the border style of the control.
	//-----------------------------------------------------------------------
	void SetBorderStyle(XTPGroupBoxBorderStyle style);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determins whether a focus rectangle is drawn around the contents of a
	//     button when the button has focus.
	// Returns:
	//     TRUE to show the focus rectangle, FALSE to hide it.
	//-----------------------------------------------------------------------
	BOOL GetShowFocus() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies whether a focus rectangle is drawn around the contents of a
	//     button when the button has focus.
	// Parameters:
	//     bShowFocus   - TRUE to show the focus rectangle, FALSE to hide it.
	//-----------------------------------------------------------------------
	void SetShowFocus(BOOL bShowFocus);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to switch the visual theme of the control.
	// Parameters:
	//     nStyle - New visual theme. Can be any of the values listed in the Remarks section.
	//     pPaintManager  - A CXTPButtonTheme pointer to the theme object.
	//              If pPaintManager is NULL - themes are disabled and the paint
	//              manager is used.
	// Remarks:
	//     nStyle can be one of the following:
	//     * <b>xtpButtonThemeStandard</b> Standard appearance style.
	//     * <b>xtpButtonThemeFlat</b> Flat appearance style.
	//     * <b>xtpButtonThemeUltraFlat</b> Ultra flat appearance style.
	//     * <b>xtpButtonThemeOffice2000</b> Office 2000 appearance style.
	//     * <b>xtpButtonThemeOfficeXP</b> Office XP appearance style.
	//     * <b>xtpButtonThemeOffice2003</b> Office 2003 appearance style.
	//     * <b>xtpButtonThemeOffice2007</b> Office 2007 appearance style.
	//-----------------------------------------------------------------------
	void SetTheme(XTPButtonTheme nStyle);
	void SetTheme(CXTPButtonTheme* pPaintManager); // <COMBINE CXTPButton::SetTheme@XTPButtonTheme>

	//-----------------------------------------------------------------------
	// Summary:
	//     Redraws the button.
	// Parameters:
	//     bUpdateWindow - TRUE to notify the parent window.
	//-----------------------------------------------------------------------
	void RedrawButton(BOOL bUpdateWindow = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to determine if the buttons has a drop
	//     down arrown and is drop down style.
	// Returns:
	//     TRUE if the button uses the Drop Down Style, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsDropDownStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to determine the size and location
	//     of the button.
	// Returns:
	//     The size and location of the button or an empty rect
	//     if no button is defined.
	//-----------------------------------------------------------------------
	virtual CRect GetButtonRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the control used as the buddy control.
	// Parameters:
	//     pWndBuddy - Handle to the control to use as a buddy.
	//-----------------------------------------------------------------------
	void SetBuddy(CWnd* pWndBuddy);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the normal and hot state icons for
	//     the push button.
	// Parameters:
	//     size           - CSize object that represents the size of the icon.
	//     hIcon          - Handle to the normal icon.
	//     pIcon          - Handle to a CXTPImageManagerIcon icon.
	//     hIconHandle    - Handle to the normal icon.
	//     hIconHot       - Handle to the hot icon.
	//     hIconHotHandle - Handle to the hot icon.
	//     nID            - Resource ID for the normal icon.
	//     nHotID         - Resource ID for the pressed icon.
	//     lpszID         - Resource string ID for the normal icon.
	//     lpszHotID      - Resource string ID for the hot icon.
	//     bRedraw        - Specifies whether the button is to be redrawn. A
	//                      nonzero value redraws the button. A zero value does
	//                      not redraw the button. The button is redrawn by default.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL SetIcon(CSize size, HICON hIcon, HICON hIconHot = NULL, BOOL bRedraw = TRUE);
	virtual BOOL SetIcon(CSize size, UINT nID, UINT nHotID = 0, BOOL bRedraw = TRUE);                //<COMBINE CXTButton::SetIcon@CSize@HICON@HICON@BOOL>
	virtual BOOL SetIcon(CSize size, LPCTSTR lpszID, LPCTSTR lpszHotID = NULL, BOOL bRedraw = TRUE); //<COMBINE CXTButton::SetIcon@CSize@HICON@HICON@BOOL>
	BOOL SetIcon(CSize size, CXTPImageManagerIcon* pIcon, BOOL bRedraw = TRUE); //<COMBINE CXTButton::SetIcon@CSize@HICON@HICON@BOOL>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the normal and hot state icons for
	//     the push button.
	// Parameters:
	//     size           - CSize object that represents the size of the icon.
	//     hIconHandle    - Handle to the normal icon.
	//     hIconHotHandle - Handle to the hot icon.
	//     bRedraw        - Specifies whether the button is to be redrawn. A
	//                      nonzero value redraws the button. A zero value does
	//                      not redraw the button. The button is redrawn by default.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL SetIcon(CSize size, const CXTPImageManagerIconHandle& hIconHandle, const CXTPImageManagerIconHandle& hIconHotHandle, BOOL bRedraw = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the bitmap associated with the
	//     button.
	// Parameters:
	//     size    - CSize object that represents the size of the bitmap.
	//     nID     - Resource ID for the bitmap.
	//     bRedraw - Specifies whether the button is to be redrawn. A nonzero
	//               value redraws the button. A zero value does not redraw
	//               the button. The button is redrawn by default.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL SetBitmap(CSize size, UINT nID, BOOL bRedraw = TRUE);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the button's text minus the '&'.
	// Parameters:
	//     bRemoveAmpersand - TRUE to remove the '&' used for mnemonics.
	// Returns:
	//     A CString object that represents the button's text.
	//-----------------------------------------------------------------------
	virtual CString GetButtonText();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the size of the check box glyph.
	// Parameters:
	//     bCheckBox - TRUE if the Checkbox is checked, FALSE otherwise.
	// Returns:
	//     The size of the check box glyph.
	//-----------------------------------------------------------------------
	CSize GetGlyphSize(BOOL bCheckBox);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the button is a push button or drop down button.
	// Returns:
	//     TRUE if the buttonis a push button, FALSE if it is a drop down button.
	//-----------------------------------------------------------------------
	BOOL IsPushButton() const;

	//-----------------------------------------------------------------------
	// Summary: Allows the use of a XAML snippet to be used for the Text of
	//          panes in the control.
	// Input:   bEnableMarkup - TRUE to enable markup, FALSE to disable markup.
	//-----------------------------------------------------------------------
	void EnableMarkup(BOOL bEnableMarkup = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the button's text color.
	// Returns:
	//     The button's text color.
	//-----------------------------------------------------------------------
	virtual COLORREF GetButtonTextColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the button's back color.
	// Returns:
	//     The button's back color.
	//-----------------------------------------------------------------------
	virtual COLORREF GetButtonBackColor();

	//-----------------------------------------------------------------------
	// Summary: Returns markup context.
	// Returns: Returns markup context.
	//-----------------------------------------------------------------------
	CXTPMarkupContext* GetMarkupContext() const;

	//-----------------------------------------------------------------------
	// Summary: Call this method to get Markup element that renders caption of the control.
	// Returns: Pointer to CXTPMarkupUIElement element.
	//-----------------------------------------------------------------------
	CXTPMarkupUIElement* GetMarkupUIElement();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Occurs when the drop down button is dropped down.
	//-----------------------------------------------------------------------
	virtual void OnDropDown();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function performs all of the drawing logic of the
	//     calendar control window.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//-----------------------------------------------------------------------
	virtual void OnDraw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called when the text has changed.
	//-----------------------------------------------------------------------
	virtual void InternalTextChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     Manually drops down a drop down button.
	//-----------------------------------------------------------------------
	void DoDropDown();

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPButton)
	virtual void DoDraw(CDC* pDC);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	void PreSubclassWindow();
	BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPButton)
	afx_msg LRESULT OnSetStyle(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnPaint();
	afx_msg LRESULT OnSetState(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMouseLeave();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnInvalidate();
	afx_msg LRESULT OnDefaultAndInvalidate(WPARAM, LPARAM);
	afx_msg LRESULT OnUpdateUIState(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSysColorChange();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnGetDlgCode(WPARAM wParam, LPARAM lParam);

	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	BOOL m_bPushed;         // TRUE if button was pressed
	BOOL m_bHot;            // TRUE if mouse under button
	CXTPButtonTheme* m_pButtonTheme;  // Pointer to a button theme.
	BOOL m_bChecked;  // TRUE if checked.

	BOOL m_bFlatStyle;  // TRUE to se the flat style.
	BOOL m_bUseVisualStyle;  // TRUE to use visual styles.

	long m_nImageAlignment;  // Image Alignment.
	long m_nBorderGap; // Gap between the button edge and the image.

	long m_nPushButtonStyle; // The style of the button,

	BOOL m_bEnableMarkup;  // TRUE to enable markup.

	long m_nImageGap; // Gap between the button text and the image.
	long m_nTextImageRelation;  // Image and text orientation.
	CXTPImageManagerIcon* m_pIcon; // Multiple state icon manager converts icon to different states.
	long m_nBorderStyle; // Border style used.
	BOOL m_bShowFocus;  // TRUE to show the focus rectangle.

	HWND m_hWndBuddy;  // Handle to the buddy control.

protected:
	CXTPMarkupContext* m_pMarkupContext;  // Markup context.
	CXTPMarkupUIElement* m_pUIElement;    // Markup element pointer.
};

#include "../../XTPWinThemeWrapper.h"
#include "../../XTPColorManager.h"


//===========================================================================
// Summary:
//     CXTPButtonTheme is a class used to perform Button Theme
//     drawing tasks.  This is the base class for all button themes.
//===========================================================================
class _XTP_EXT_CLASS CXTPButtonTheme : public CXTPCmdTarget
{
public:

	// ----------------------------------------
	// Summary:
	//     Constructs a CXTPButtonTheme object
	// ----------------------------------------
	CXTPButtonTheme();

	// -----------------------------------------------------------
	// Summary:
	//     Destroys a CXTPButtonTheme object, handles cleanup and
	//     deallocation
	// -----------------------------------------------------------
	virtual ~CXTPButtonTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the background for the
	//     button.
	// Parameters:
	//     pDC     - A CDC pointer that represents the current device
	//               context.
	//     pButton - Points to a CXTPButton object.
	// Returns:
	//     TRUE if the background was drawn successfully, otherwise returns
	//     FALSE.
	//-----------------------------------------------------------------------
	virtual void DrawButtonBackground(CDC* pDC, CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the focus rectangle
	//     for the button.
	// Parameters:
	//     pDC     - A CDC pointer that represents the current device
	//               context.
	//     pButton - Points to a CXTPButton object.
	//-----------------------------------------------------------------------
	void DrawFocusRect(CDC* pDC, CXTPButton* pButton);

	virtual void DrawButton(CDC* pDC, CXTPButton* pButton);

	void DrawButtonVisualStyleBackground(CDC* pDC, CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the theme manager to refresh
	//     the visual styles used by each components theme.
	// Parameters:
	//     pButton - Points to a CXTPButton object.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics(CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the text for the button.
	// Parameters:
	//     pDC     - A CDC pointer that represents the current device
	//               context.
	//     pButton - Points to a CXTPButton object.
	//-----------------------------------------------------------------------
	void DrawPushButtonText(CDC* pDC, CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function draws the icon for the button, if one has
	//     been defined.
	// Parameters:
	//     pDC     - A CDC pointer that represents the current device
	//               context.
	//     pButton - Points to a CXTPButton object.
	//-----------------------------------------------------------------------
	void DrawPushButtonIcon(CDC* pDC, CXTPButton* pButton);
	void DrawPushButtonDropDown(CDC* pDC, CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the color used to draw
	//     the button text.
	// Parameters:
	//     pButton - Points to a CXTPButton object.
	// Returns:
	//     An RGB value that represents the button text color.
	//-----------------------------------------------------------------------
	COLORREF GetTextColor(CXTPButton* pButton);

	virtual void DrawCheckBoxMark(CDC* pDC, CXTPButton* pButton);
	virtual void DrawRadioButtonMark(CDC* pDC, CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the text for the button.
	// Parameters:
	//     pDC     - A CDC pointer that represents the current device
	//               context.
	//     pButton - Points to a CXTPButton object.
	//-----------------------------------------------------------------------
	void DrawButtonText(CDC* pDC, CXTPButton* pButton);

	virtual void DrawGroupBox(CDC* pDC, CXTPButton* pButton);

	void CalcRects(CDC* pDC, CXTPButton* pButton, CRect* pButtonText, UINT* pDrawFlags, CRect* pButtonIcon);
	void CalcRect(CDC* pDC, CXTPButton* pButton, LPRECT lprc, int code);
	BOOL IsVisualThemeUsed(CXTPButton* pButton);
	void AlphaEllipse(CDC* pDC, CRect rc, COLORREF clrBorder, COLORREF clrFace);

	CXTPPaintManagerColor m_crTextDisabled;     // RGB value for disabled text color.
	CXTPPaintManagerColor m_crBorderHilite;     // RGB value for border highlight color.
	CXTPPaintManagerColor m_crBorderShadow;     // RGB value for border shadow color.
	CXTPPaintManagerColor m_crBorder3DHilite;   // RGB value for 3D border highlight color.
	CXTPPaintManagerColor m_crBorder3DShadow;   // RGB value for 3D border shadow color.
	CXTPPaintManagerColor m_crBack;
	CXTPPaintManagerColor m_crText;

	CXTPWinThemeWrapper m_themeButton;

	BOOL m_bOffsetHiliteText;
	int m_nBorderWidth;
	BOOL m_bFlatGlyphs;

	int m_cxBorder;
	int m_cyBorder;
	int m_cyEdge;
	int m_cxEdge;
};

//===========================================================================
// Summary:
//     CXTPButtonFlatTheme is a class used to perform Flat Theme
//     drawing tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPButtonFlatTheme : public CXTPButtonTheme
{
public:

	// ----------------------------------------
	// Summary:
	//     Constructs a CXTPButtonFlatTheme object
	// ----------------------------------------
	CXTPButtonFlatTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the background for the
	//     button.
	// Parameters:
	//     pDC     - A CDC pointer that represents the current device
	//               context.
	//     pButton - Points to a CXTPButton object.
	// Returns:
	//     TRUE if the background was drawn successfully, otherwise returns
	//     FALSE.
	//-----------------------------------------------------------------------
	void DrawButtonBackground(CDC* pDC, CXTPButton* pButton);
};

//===========================================================================
// Summary:
//     CXTPButtonUltraFlatTheme is a class used to perform Ultra Flat Theme
//     drawing tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPButtonUltraFlatTheme : public CXTPButtonTheme
{
public:

	// ----------------------------------------
	// Summary:
	//     Constructs a CXTPButtonUltraFlatTheme object
	// ----------------------------------------
	CXTPButtonUltraFlatTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the theme manager to refresh
	//     the visual styles used by each components theme.
	// Parameters:
	//     pButton - Points to a CXTPButton object.
	//-----------------------------------------------------------------------
	void RefreshMetrics(CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the background for the
	//     button.
	// Parameters:
	//     pDC     - A CDC pointer that represents the current device
	//               context.
	//     pButton - Points to a CXTPButton object.
	// Returns:
	//     TRUE if the background was drawn successfully, otherwise returns
	//     FALSE.
	//-----------------------------------------------------------------------
	void DrawButtonBackground(CDC* pDC, CXTPButton* pButton);
	virtual void DrawGroupBox(CDC* pDC, CXTPButton* pButton);
	virtual void DrawCheckBoxMark(CDC* pDC, CXTPButton* pButton);
	virtual void DrawRadioButtonMark(CDC* pDC, CXTPButton* pButton);

	CXTPPaintManagerColor m_crBackPushed; // RGB value for pushed background color.
	CXTPPaintManagerColor m_crBackHilite; // RGB value for highlighted background color.
	CXTPPaintManagerColor m_crTextPushed; // RGB value for highlighted text color.
	CXTPPaintManagerColor m_crTextHilite; // RGB value for pushed text color.
	CXTPPaintManagerColor m_crBackChecked;// RGB value for when the control is checked.

	BOOL m_bHiglightButtons;

};

//===========================================================================
// Summary:
//     CXTPButtonOffice2000Theme is a class used to perform Office 2000 Theme
//     drawing tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPButtonOffice2000Theme : public CXTPButtonUltraFlatTheme
{
public:

	// ----------------------------------------
	// Summary:
	//     Constructs a CXTPButtonOffice2000Theme object
	// ----------------------------------------
	CXTPButtonOffice2000Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the background for the
	//     button.
	// Parameters:
	//     pDC     - A CDC pointer that represents the current device
	//               context.
	//     pButton - Points to a CXTPButton object.
	// Returns:
	//     TRUE if the background was drawn successfully, otherwise returns
	//     FALSE.
	//-----------------------------------------------------------------------
	void DrawButtonBackground(CDC* pDC, CXTPButton* pButton);
};

//===========================================================================
// Summary:
//     CXTPButtonOffice2007Theme is a class used to perform Office 2007 Theme
//     drawing tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPButtonOffice2007Theme : public CXTPButtonUltraFlatTheme
{
public:

	// ----------------------------------------
	// Summary:
	//     Constructs a CXTPButtonOffice2007Theme object
	// ----------------------------------------
	CXTPButtonOffice2007Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the background for the
	//     button.
	// Parameters:
	//     pDC     - A CDC pointer that represents the current device
	//               context.
	//     pButton - Points to a CXTPButton object.
	// Returns:
	//     TRUE if the background was drawn successfully, otherwise returns
	//     FALSE.
	//-----------------------------------------------------------------------
	void DrawButtonBackground(CDC* pDC, CXTPButton* pButton);
	void DrawCheckBoxMark(CDC* pDC, CXTPButton* pButton);
	void DrawRadioButtonMark(CDC* pDC, CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the theme manager to refresh
	//     the visual styles used by each components theme.
	// Parameters:
	//     pButton - Points to a CXTPButton object.
	//-----------------------------------------------------------------------
	void RefreshMetrics(CXTPButton* pButton);

};

//===========================================================================
// Summary:
//     CXTPButtonOfficeXPTheme is a class used to perform Office XP Theme
//     drawing tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPButtonOfficeXPTheme : public CXTPButtonUltraFlatTheme
{
public:

	// ----------------------------------------
	// Summary:
	//     Constructs a CXTPButtonOfficeXPTheme object
	// ----------------------------------------
	CXTPButtonOfficeXPTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the background for the
	//     button.
	// Parameters:
	//     pDC     - A CDC pointer that represents the current device
	//               context.
	//     pButton - Points to a CXTPButton object.
	// Returns:
	//     TRUE if the background was drawn successfully, otherwise returns
	//     FALSE.
	//-----------------------------------------------------------------------
	void DrawButtonBackground(CDC* pDC, CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the theme manager to refresh
	//     the visual styles used by each components theme.
	// Parameters:
	//     pButton - Points to a CXTPButton object.
	//-----------------------------------------------------------------------
	void RefreshMetrics(CXTPButton* pButton);

};

//===========================================================================
// Summary:
//     CXTPButtonOffice2003Theme is a class used to perform Office 2003 Theme
//     drawing tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPButtonOffice2003Theme : public CXTPButtonOfficeXPTheme
{
public:

	// ----------------------------------------
	// Summary:
	//     Constructs a CXTPButtonOffice2003Theme object
	// ----------------------------------------
	CXTPButtonOffice2003Theme();
	BOOL m_bLunaTheme;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the background for the
	//     button.
	// Parameters:
	//     pDC     - A CDC pointer that represents the current device
	//               context.
	//     pButton - Points to a CXTPButton object.
	// Returns:
	//     TRUE if the background was drawn successfully, otherwise returns
	//     FALSE.
	//-----------------------------------------------------------------------
	void DrawButtonBackground(CDC* pDC, CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the theme manager to refresh
	//     the visual styles used by each components theme.
	// Parameters:
	//     pButton - Points to a CXTPButton object.
	//-----------------------------------------------------------------------
	void RefreshMetrics(CXTPButton* pButton);

};



AFX_INLINE BOOL CXTPButton::IsPushed() {
	return m_bPushed;
}
AFX_INLINE BOOL CXTPButton::IsHighlighted() {
	return m_bHot || (::GetCapture() == m_hWnd);
}
AFX_INLINE BOOL CXTPButton::GetFlatStyle() const{
	return m_bFlatStyle;
}
AFX_INLINE void CXTPButton::SetFlatStyle(BOOL bFlatStyle/* = TRUE*/) {
	m_bFlatStyle = bFlatStyle;
	RedrawButton();
}
AFX_INLINE BOOL CXTPButton::GetUseVisualStyle() const {
	return m_bUseVisualStyle;
}
AFX_INLINE long CXTPButton::GetBorderGap() const {
	return m_nBorderGap;
}
AFX_INLINE void CXTPButton::SetBorderGap(int nBorderGap) {
	m_nBorderGap = nBorderGap;
	RedrawButton();
}
AFX_INLINE XTPPushButtonStyle CXTPButton::GetPushButtonStyle() const {
	return (XTPPushButtonStyle)m_nPushButtonStyle;
}
AFX_INLINE void CXTPButton::SetPushButtonStyle(XTPPushButtonStyle nPushButtonStyle) {
	m_nPushButtonStyle = nPushButtonStyle;
	RedrawButton();
}
AFX_INLINE void CXTPButton::SetTextAlignment(DWORD dwAlignment) {
	ModifyStyle(BS_LEFT | BS_CENTER | BS_RIGHT | BS_TOP | BS_BOTTOM | BS_VCENTER, dwAlignment);
	RedrawButton();
}
AFX_INLINE int CXTPButton::GetImageAlignment() const {
	return m_nImageAlignment;
}
AFX_INLINE void CXTPButton::SetImageAlignment(DWORD dwAlignment) {
	m_nImageAlignment = dwAlignment;
	RedrawButton();
}
AFX_INLINE XTPButtonTextImageRelation CXTPButton::GetTextImageRelation() const {
	return (XTPButtonTextImageRelation)m_nTextImageRelation;
}
AFX_INLINE void CXTPButton::SetTextImageRelation(XTPButtonTextImageRelation realtion) {
	m_nTextImageRelation = realtion;
	RedrawButton();
}
AFX_INLINE long CXTPButton::GetImageGap() const {
	return m_nImageGap;
}
AFX_INLINE void CXTPButton::SetImageGap(int nImageGap) {
	m_nImageGap = nImageGap;
}
AFX_INLINE CXTPImageManagerIcon* CXTPButton::GetIcon() const {
	return m_pIcon;
}
AFX_INLINE XTPGroupBoxBorderStyle CXTPButton::GetBorderStyle() const {
	return (XTPGroupBoxBorderStyle)m_nBorderStyle;
}
AFX_INLINE void CXTPButton::SetBorderStyle(XTPGroupBoxBorderStyle style) {
	m_nBorderStyle = style;
}
AFX_INLINE BOOL CXTPButton::GetShowFocus() const {
	return m_bShowFocus;
}
AFX_INLINE void CXTPButton::SetShowFocus(BOOL bShowFocus) {
	m_bShowFocus = bShowFocus;
}
AFX_INLINE void CXTPButton::RedrawButton(BOOL bUpdateWindow) {
	if (m_hWnd) Invalidate(FALSE);
	if (m_hWnd && bUpdateWindow) UpdateWindow();
}
AFX_INLINE CXTPMarkupContext* CXTPButton::GetMarkupContext() const {
	return m_pMarkupContext;
}

AFX_INLINE CXTPMarkupUIElement* CXTPButton::GetMarkupUIElement() {
	return m_pUIElement;
}
AFX_INLINE BOOL CXTPButton::IsDropDownStyle() const {
	return m_nPushButtonStyle == xtpButtonDropDown || m_nPushButtonStyle == xtpButtonDropDownRight || m_nPushButtonStyle == xtpButtonSplitDropDown;
}


#endif // #if !defined(__XTPBUTTON_H__)
