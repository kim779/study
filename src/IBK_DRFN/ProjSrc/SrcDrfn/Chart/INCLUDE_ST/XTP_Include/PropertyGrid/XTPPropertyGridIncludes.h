// XTPPropertyGridIncludes.h
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
#if _MSC_VER > 1000
#pragma once
#endif 
//:End Ignore

#include "XTPPropertyGridInplaceEdit.h"
#include "XTPPropertyGridInplaceButton.h"
#include "XTPPropertyGridInplaceList.h"
#include "XTPPropertyGridItem.h"
#include "XTPPropertyGrid.h"
#include "XTPPropertyGridItemBool.h"
#include "XTPPropertyGridItemColor.h" 
#include "XTPPropertyGridItemFont.h"
#include "XTPPropertyGridItemNumber.h"
#include "XTPPropertyGridItemSize.h"

// BULLETED LIST:

// Input:   nGridAction -  Value of wParam specifies an value that indicates the user's 
//			request. This parameter can be one of the following values:
//			[ul]
//			[li]<b>XTP_PGN_SORTORDER_CHANGED</b> The sort order has changed in the property grid.[/li]
//			[li]<b>XTP_PGN_SELECTION_CHANGED</b> The selection has changed in the property grid.[/li]
//			[li]<b>XTP_PGN_ITEMVALUE_CHANGED</b> The value has changed for pItem in the property grid.[/li]
//			[li]<b>XTP_PGN_EDIT_CHANGED</b> The edit value has changed in the property grid.[/li]
//			[/ul]
//			pItem - The value of lParam points to a CXTPPropertyGridItem object that contains information for the 
//			specified item. This pointer should <b>never</b> be NULL.
// Returns: If the application is to process this message, the return value should be TRUE, otherwise the 
//			return value is FALSE.  
//
// Example: Here is an example of how an application would process the XTWM_PROPERTYGRID_NOTIFY 
//			message. 
//
//			<pre>int nGridAction = (int)wParam;</pre>
//			 
//			<pre>// Cast the lParam to an CXTPPropertyGridItem* object pointer.
//			CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;
//			ASSERT(pItem);</pre>
//
//			<pre>switch (nGridAction)
//			{
//			    case XTP_PGN_SORTORDER_CHANGED:
//					  {
//			             m_nSort = m_wndXTPPropertyGrid.GetPropertySort();
//			             UpdateData(FALSE);
//					  }
//			        break;
//			    case XTP_PGN_ITEMVALUE_CHANGED:
//					  {
//			             TRACE(_T("Value Changed. Caption = %s, ID = %i, Value = %s\n"),
//			                 pItem->GetCaption(), pItem->GetID(), pItem->GetValue());
//					  }
//			        break;
//			    case XTP_PGN_SELECTION_CHANGED:
//					  {
//			             TRACE(_T("Selection Changed. Item = %s\n"), pItem->GetCaption());
//					  }
//			        break;
//			}
//			return FALSE;</pre>
// Summary: The XTWM_PROPERTYGRID_NOTIFY message is sent to the CXTPPropertyGrid owner window 
//			whenever an action occurs within the CXTPPropertyGrid
//   
//			<pre>XTWM_PROPERTYGRID_NOTIFY 
//			nGridAction = (int) wParam;              // Property grid action 
//			pItem = (CXTPPropertyGridItem*) lParam;  // pointer to a CXTPPropertyGridItem object</pre>
//
//			When the user performs an action in the property grid, the XTWM_PROPERTYGRID_NOTIFY message is 
//			sent to the property grid's owner window.
const UINT XTPWM_PROPERTYGRID_NOTIFY = (WM_APP + 2533);
const UINT XTP_PGN_SORTORDER_CHANGED  = 1;	// The sort order has changed in the property grid.
const UINT XTP_PGN_SELECTION_CHANGED  = 2;	// The selection has changed in the property grid.
const UINT XTP_PGN_ITEMVALUE_CHANGED  = 3;	// The value has changed for pItem in the property grid.
const UINT XTP_PGN_EDIT_CHANGED      = 4;	// The edit value has changed in the property grid.
const UINT XTP_PGN_INPLACEBUTTONDOWN = 5;	// Item's inplace button (combo or expand) down.
const UINT XTP_PGN_DRAWITEM = 6;			// Item's draw event.
const UINT XTP_PGS_OWNERDRAW = 0x0010L;		// Apply this style to the property grid to use the owner draw feature.

//:Ignore
#ifdef _XTP_DOTNET
const UINT XTP_PGN_ITEMEXPANDCHANGED = 7;	// Item's inplace button (combo or expand) down.
#endif
//:End Ignore

const UINT XTP_PGN_DBLCLICK = 8;            // Double click event.
