// MemberDraw.cpp: implementation of the CMemberDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExGridCtrl.h"
#include "MemberDraw.h"
#include "../../inc/ISkinColorMng.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemberDraw::CMemberDraw(LPVOID lpControl, ISkinColorManager* pISkinColorManager, BOOL bUseDefSkin) : CMember(lpControl, bUseDefSkin)
{
	m_pGrid = (CGridCtrl_Friendly*)lpControl;
	SetSkinColorManager(pISkinColorManager);

#if !defined(GRIDCONTROL_NO_DRAGDROP) || !defined(GRIDCONTROL_NO_CLIPBOARD)
    _AFX_THREAD_STATE* pState = AfxGetThreadState();
    if (!pState->m_bNeedTerm && !AfxOleInit())
        AfxMessageBox(_T("OLE initialization failed. Make sure that the OLE libraries are the correct version"));
#endif

    // Store the system colours in case they change. The gridctrl uses
    // these colours, and in OnSysColorChange we can check to see if
    // the gridctrl colours have been changed from the system colours.
    // If they have, then leave them, otherwise change them to reflect
    // the new system colours.
//    m_crWindowText        = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(3) : m_pISkinColorManager->GetColor(3);
//    m_crWindowColour      = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(23) : m_pISkinColorManager->GetColor(23);
//    m_cr3DFace            = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(20) : m_pISkinColorManager->GetColor(20);
//    m_crShadow            = ::GetSysColor(COLOR_3DSHADOW);
//    m_crGridLineColour    = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(256) : m_pISkinColorManager->GetColor(256);
//
//	m_crFixedBkColour = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(20) : m_pISkinColorManager->GetColor(20);
//	m_crFixedTextColour = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(3) : m_pISkinColorManager->GetColor(3);
//	m_crGridBkColour = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(23) : m_pISkinColorManager->GetColor(23);
//
//	m_crFixedGridLineColour_Light = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(20) : m_pISkinColorManager->GetColor(20);
//	m_crFixedGridLineColour_Dark = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(22) : m_pISkinColorManager->GetColor(22);
//	m_crHighlight = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(29) : m_pISkinColorManager->GetColor(29);
//	m_crHighlightText = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(3) : m_pISkinColorManager->GetColor(3);
	m_bUseHighlightText = FALSE;
	m_bFixedRowSelectionHighlight = FALSE; // 추가 LYH 20050422 : FixedRow만 지원하고 FixedColumn은 지원하지 않는다.

    m_nRows               = 0;
    m_nCols               = 0;
    m_nFixedRows          = 0;
    m_nFixedCols          = 0;

    m_bVirtualMode        = FALSE;
    m_pfnCallback         = NULL;

    m_nVScrollMax         = 0;          // Scroll position
    m_nHScrollMax         = 0;
    m_nRowsPerWheelNotch  = theApp.GetMouseScrollLines(); // Get the number of lines
                                                   // per mouse wheel notch to scroll
    m_nBarState           = GVL_NONE;
    m_MouseMode           = theApp.MOUSE_NOTHING;
    m_nGridLines          = GVL_BOTH;
    m_bEditable           = TRUE;
    m_bListMode           = FALSE;
    m_bSingleRowSelection = FALSE;
    m_bSingleColSelection = FALSE;
    m_bLMouseButtonDown   = FALSE;
    m_bRMouseButtonDown   = FALSE;
    m_bAllowDraw          = TRUE;       // allow draw updates
    m_bEnableSelection    = TRUE;
    m_bFixedColumnSelection = TRUE;
    m_bFixedRowSelection  = TRUE;
    m_bAllowRowResize     = TRUE;
    m_bAllowColumnResize  = TRUE;
    m_bSortOnClick        = FALSE;      // Sort on header row click
    m_bHandleTabKey       = TRUE;
#ifdef _WIN32_WCE
    m_bDoubleBuffer       = FALSE;      // Use double buffering to avoid flicker?
#else
    m_bDoubleBuffer       = TRUE;       // Use double buffering to avoid flicker?
#endif
    m_bTitleTips          = TRUE;       // show cell title tips

    m_bWysiwygPrinting    = FALSE;      // use size-to-width printing

    m_bHiddenColUnhide    = TRUE;       // 0-width columns can be expanded via mouse
    m_bHiddenRowUnhide    = TRUE;       // 0-Height rows can be expanded via mouse

    m_bAllowColHide       = TRUE;       // Columns can be contracted to 0-width via mouse
    m_bAllowRowHide       = TRUE;       // Rows can be contracted to 0-height via mouse

    m_bAscending          = TRUE;       // sorting stuff
    m_nSortColumn         = -1;
	m_pfnCompare		  = NULL;

    m_nAutoSizeColumnStyle = GVS_BOTH;  // Autosize grid using header and data info

	m_bDefaultReadOnlyCell = FALSE;		// cell 생성할때 default 속성에 readonly 를 넣을지 여부 LYH 20050418

    m_nTimerID            = 0;          // For drag-selection
    m_nTimerInterval      = 25;         // (in milliseconds)
    m_nResizeCaptureRange = 3;          // When resizing columns/row, the cursor has to be
                                        // within +/-3 pixels of the dividing line for
                                        // resizing to be possible
    m_pImageList          = NULL;       // Images in the grid
    m_bAllowDragAndDrop   = FALSE;      // for drag and drop - EFW - off by default
    m_bTrackFocusCell     = TRUE;       // Track Focus cell?
    m_bFrameFocus         = TRUE;       // Frame the selected cell?

    m_pRtcDefault = RUNTIME_CLASS(CGridCell);

//    SetupDefaultCells();
//    SetGridBkColor(m_crShadow);
//
//    // Set up the initial grid size
//    SetRowCount(nRows);
//    SetColumnCount(nCols);
//    SetFixedRowCount(nFixedRows);
//    SetFixedColumnCount(nFixedCols);
//
//    SetTitleTipTextClr(CLR_DEFAULT);  //FNA
//    SetTitleTipBackClr(CLR_DEFAULT); 

    // set initial selection range (ie. none)
    m_SelectedCellMap.RemoveAll();
    m_PrevSelectedCellMap.RemoveAll();

	m_bHideHScrollBar = m_bHideVScrollBar = TRUE;
	m_nPreVPos = m_nPreHPos = -1;
	
	m_bUseFocusRect = TRUE;
	m_bDrawFixedLine = FALSE;
}

CMemberDraw::~CMemberDraw()
{
#ifndef GRIDCONTROL_NO_TITLETIPS
    if (m_bTitleTips && ::IsWindow(m_TitleTip.GetSafeHwnd())) 
        m_TitleTip.DestroyWindow();
#endif
}
