// XTDefines.h interface for the CXTAuxData struct.
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
#if !defined(__XTDEFINES_H__)
#define __XTDEFINES_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTNotify.h"

// ----------------------------------------------------------------------
// Summary:
//     Enumeration to determine mouse state.
// Remarks:
//     XTMouseState type defines the constants used to determine the
//     state and position the mouse cursor is with relation to a
//     control's window. This is used to determine how the control should
//     be rendered.
// See Also:
//     CXTFlatEdit::DrawEdit, CXTFlatComboBox::DrawCombo,
//     CXTOutBarCtrl::DrawFolder
//
// <KEYWORDS xtMouseNormal, xtMouseHover, xtMouseSelect>
// ----------------------------------------------------------------------
enum XTMouseState
{
	xtMouseNormal = 1, // The mouse cursor is not hovering.
	xtMouseHover  = 2, // The mouse cursor is hovering the control.
	xtMouseSelect = 3  // The mouse cursor is hovering the control and the left button is pressed.
};

#ifndef BTNS_WHOLEDROPDOWN
#define BTNS_WHOLEDROPDOWN              0x80  /* draw dropdown arrow, but without split arrow section */
#endif
// Summary: The I_IMAGENONE constant is used when defining text-only toolbar buttons with no
//          space allocated to the button icon
//          The constant is normally defined in version 5.81 and greater SDK headers
#ifndef I_IMAGENONE
#define I_IMAGENONE (-2)
#endif//I_IMAGENONE

#ifndef IDC_HAND
#define IDC_HAND            MAKEINTRESOURCE(32649)
#endif

// -------------------------------------------------------------------
// Summary:
//     Paint styles of a control.
// Remarks:
//     XTPControlTheme is used by many of the controls to set the theme.
// -------------------------------------------------------------------
enum XTPControlTheme
{
	xtpControlThemeStandard, // Standard appearance style.
	xtpControlThemeFlat, // Flat appearance style.
	xtpControlThemeUltraFlat, // Ultra flat appearance style.
	xtpControlThemeOffice2000, // Office 2000 appearance style.
	xtpControlThemeOfficeXP, // Office XP appearance style.
	xtpControlThemeOffice2003, // Office 2003 appearance style.
	xtpControlThemeOffice2007 // Office 2007 appearance style.
};


//////////////////////////////////////////////////////////////////////

#endif // #if !defined(__XTDEFINES_H__)
