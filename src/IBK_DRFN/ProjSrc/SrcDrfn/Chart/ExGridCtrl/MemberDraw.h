// MemberDraw.h: interface for the CMemberDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMBERDRAW_H__DDBDC4C4_50D2_4B78_A560_1206DFCF013D__INCLUDED_)
#define AFX_MEMBERDRAW_H__DDBDC4C4_50D2_4B78_A560_1206DFCF013D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../inc/_COMMON/Member.h"

class CMemberDraw : public CMember  
{
public:
	CMemberDraw(LPVOID lpControl, ISkinColorManager* pISkinColorManager, BOOL bUseDefSkin);
	virtual ~CMemberDraw();

protected:
    class CGridCtrl_Friendly : public CWnd
    {
		friend class CExGridCtrlApp; 
    };
	
public:
	CGridCtrl_Friendly*		m_pGrid;

    // General attributes
    COLORREF    m_crFixedTextColour, m_crFixedBkColour /* 20 */;
    COLORREF    m_crGridBkColour /* 23 */, m_crGridLineColour;
    COLORREF    m_crWindowText, m_crWindowColour, m_cr3DFace,     // System colours
                m_crShadow;
    COLORREF    m_crTTipBackClr, m_crTTipTextClr;                 // Titletip colours - FNA
    
	COLORREF	m_crFixedGridLineColour_Light, m_crFixedGridLineColour_Dark; // 추가 LYH
	COLORREF	m_crHighlight, m_crHighlightText; // 추가 LYH
	BOOL		m_bUseHighlightText; // 추가 LYH
    BOOL        m_bFixedRowSelectionHighlight; // 추가 LYH 20050422 : FixedRow만 지원하고 FixedColumn은 지원하지 않는다.
    
	BOOL        m_bVirtualMode;
    LPARAM      m_lParam;                                           // lParam for callback
    GRIDCALLBACK m_pfnCallback;                                     // The callback function

    int         m_nGridLines;
    BOOL        m_bEditable;
    BOOL        m_bModified;
    BOOL        m_bAllowDragAndDrop;
    BOOL        m_bListMode;
    BOOL        m_bSingleRowSelection;
    BOOL        m_bSingleColSelection;
    BOOL        m_bAllowDraw;
    BOOL        m_bEnableSelection;
    BOOL        m_bFixedRowSelection, m_bFixedColumnSelection;
    BOOL        m_bSortOnClick;
    BOOL        m_bHandleTabKey;
    BOOL        m_bDoubleBuffer;
    BOOL        m_bTitleTips;
    int         m_nBarState;
    BOOL        m_bWysiwygPrinting;
    BOOL        m_bHiddenColUnhide, m_bHiddenRowUnhide;
    BOOL        m_bAllowColHide, m_bAllowRowHide;
    BOOL        m_bAutoSizeSkipColHdr;
    BOOL        m_bTrackFocusCell;
    BOOL        m_bFrameFocus;
    UINT        m_nAutoSizeColumnStyle;
	BOOL		m_bDefaultReadOnlyCell;	// cell 생성할때 default 속성에 readonly 를 넣을지 여부 LYH 20050418
	BOOL		m_bUseFocusRect;		// 포커스를 그릴것인지 ojtaso

    // Cell size details
    int         m_nRows, m_nFixedRows, m_nCols, m_nFixedCols;
    CUIntArray  m_arRowHeights, m_arColWidths;
    int         m_nVScrollMax, m_nHScrollMax;
	int			m_nPreVPos, m_nPreHPos;

    // Fonts and images
    CRuntimeClass*   m_pRtcDefault; // determines kind of Grid Cell created by default
    CGridDefaultCell m_cellDefault;  // "default" cell. Contains default colours, font etc.
    CGridDefaultCell m_cellFixedColDef, m_cellFixedRowDef, m_cellFixedRowColDef;
    CFont       m_PrinterFont;  // for the printer
    CImageList* m_pImageList;

    // Cell data
    CTypedPtrArray<CObArray, GRID_ROW*> m_RowData;

    // Mouse operations such as cell selection
    int         m_MouseMode;
    BOOL        m_bLMouseButtonDown, m_bRMouseButtonDown;
    CPoint      m_LeftClickDownPoint, m_LastMousePoint;
    CCellID     m_LeftClickDownCell, m_SelectionStartCell;
    CCellID     m_idCurrentCell, m_idTopLeftCell;
    int         m_nTimerID;
    int         m_nTimerInterval;
    int         m_nResizeCaptureRange;
    BOOL        m_bAllowRowResize, m_bAllowColumnResize;
    int         m_nRowsPerWheelNotch;
    CMap<DWORD,DWORD, CCellID, CCellID&> m_SelectedCellMap, m_PrevSelectedCellMap;

#ifndef GRIDCONTROL_NO_TITLETIPS
    CTitleTip   m_TitleTip;             // Title tips for cells
#endif

    // Drag and drop
    CCellID     m_LastDragOverCell;
#ifndef GRIDCONTROL_NO_DRAGDROP
    CGridDropTarget m_DropTarget;       // OLE Drop target for the grid
#endif

    // Printing information
    CSize       m_CharSize;
    int         m_nPageHeight;
    CSize       m_LogicalPageSize,      // Page size in gridctrl units.
                m_PaperSize;            // Page size in device units.
    // additional properties to support Wysiwyg printing
    int         m_nPageWidth;
    int         m_nPrintColumn;
    int         m_nCurrPrintRow;
    int         m_nNumPages;
    int         m_nPageMultiplier;

    // sorting
    int          m_bAscending;
    int          m_nSortColumn;
	PFNLVCOMPARE m_pfnCompare;

    // EFW - Added to support shaded/unshaded printout.  If true, colored
    // cells will print as-is.  If false, all text prints as black on white.
    BOOL        m_bShadedPrintOut;

    // EFW - Added support for user-definable margins.  Top and bottom are in 
    // lines.  Left, right, and gap are in characters (avg width is used).
    int         m_nHeaderHeight, m_nFooterHeight, m_nLeftMargin,
                m_nRightMargin, m_nTopMargin, m_nBottomMargin, m_nGap;

	BOOL		m_bHideHScrollBar;
	BOOL		m_bHideVScrollBar;
	BOOL		m_bDrawFixedLine;
};

#endif // !defined(AFX_MEMBERDRAW_H__DDBDC4C4_50D2_4B78_A560_1206DFCF013D__INCLUDED_)
