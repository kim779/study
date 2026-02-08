#include "stdafx.h"
#include "ExGridCtrl.h"

#include "MemberDraw.h"
#include "MemDC.h"

#include "../../inc/ExGridCtrlDefine.h"
#include "../../inc/ExGridCtrl.h"
#include "../../inc/ISkinColorMng.h"

// OLE stuff for clipboard operations
#include <afxadv.h>            // For CSharedFile
#include <afxconv.h>           // For LPTSTR -> LPSTR macros

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Spit out some messages as a sanity check for programmers
#ifdef GRIDCONTROL_NO_TITLETIPS
#pragma message(" -- CExGridCtrl: No titletips for cells with large data")
#endif
#ifdef GRIDCONTROL_NO_DRAGDROP
#pragma message(" -- CExGridCtrl: No OLE drag and drop")
#endif
#ifdef GRIDCONTROL_NO_CLIPBOARD
#pragma message(" -- CExGridCtrl: No clipboard support")
#endif
#ifdef GRIDCONTROL_NO_PRINTING
#pragma message(" -- CExGridCtrl: No printing support")
#endif

//
//	Note!
//
//		If pMember->m_pGrid DLL is dynamically linked against the MFC
//		DLLs, any functions exported from pMember->m_pGrid DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that pMember->m_pGrid macro appear in each
//		function, prior to any calls into MFC.  pMember->m_pGrid means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CExGridCtrlApp

BEGIN_MESSAGE_MAP(CExGridCtrlApp, CWinApp)
	//{{AFX_MSG_MAP(CExGridCtrlApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExGridCtrlApp construction

CExGridCtrlApp::CExGridCtrlApp()
{
	m_pCurMember = NULL;
}

long CExGridCtrlApp::ActionControl(LPVOID lpAction)
{
	LPST_ACTIONCONTROL lpActionControl = (LPST_ACTIONCONTROL)lpAction;
//	CExButton* pButton = (CExButton*)m_pCurMember->m_pControl;
//	m_pManager->CallNotifyControl((LPVOID)pButton->GetParent(), 0);

	return 0L;
}

void CExGridCtrlApp::ExitGridCtrl(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	delete m_pCurMember;
	m_pCurMember = NULL;
}

LPVOID CExGridCtrlApp::InitGridCtrl(LPVOID lpPointer, BOOL bUseDefSkin/* = FALSE*/)
{
	HWND hMainWnd;
	AfxGetHanaroMainWnd(hMainWnd);

	CMember* pMember = new CMemberDraw(lpPointer, (ISkinColorManager*)::SendMessage(hMainWnd, RMSG_INTERFACE, 6, 0), bUseDefSkin);
	return pMember;
}

UINT CExGridCtrlApp::GetMouseScrollLines()
{
    int nScrollLines = 3;            // reasonable default

#ifndef _WIN32_WCE
    // Do things the hard way in win95
    OSVERSIONINFO VersionInfo;
    VersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    if (!GetVersionEx(&VersionInfo) || 
        (VersionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS && VersionInfo.dwMinorVersion == 0))
    {
        HKEY hKey;
        if (RegOpenKeyEx(HKEY_CURRENT_USER,  _T("Control Panel\\Desktop"),
            0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
        {
            TCHAR szData[128];
            DWORD dwKeyDataType;
            DWORD dwDataBufSize = sizeof(szData);
            
            if (RegQueryValueEx(hKey, _T("WheelScrollLines"), NULL, &dwKeyDataType,
                (LPBYTE) &szData, &dwDataBufSize) == ERROR_SUCCESS)
            {
                nScrollLines = _tcstoul(szData, NULL, 10);
            }
            RegCloseKey(hKey);
        }
    }
    // win98 or greater
    else
           SystemParametersInfo (SPI_GETWHEELSCROLLLINES, 0, &nScrollLines, 0);
#endif

    return nScrollLines;
}

int CExGridCtrlApp::GetFixedColumnCount()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_nFixedCols;
}

void CExGridCtrlApp::SetGridBkColor(COLORREF clr)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_crGridBkColour = clr; 
}

int CExGridCtrlApp::GetRowCount()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_nRows; 
}

int CExGridCtrlApp::GetColumnCount()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_nCols; 
}

int CExGridCtrlApp::GetFixedRowCount()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_nFixedRows;
}

COLORREF CExGridCtrlApp::GetGridBkColor()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_crGridBkColour;
}

void CExGridCtrlApp::SetGridLineColor(COLORREF clr)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_crGridLineColour = clr;
}

COLORREF CExGridCtrlApp::GetGridLineColor()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_crGridLineColour;
}

CSize CExGridCtrlApp::GetCellTextExtent(int nRow, int nCol)
{
	return GetTextExtent(nRow, nCol, GetItemText(nRow,nCol));
}

void CExGridCtrlApp::SetTitleTipBackClr(COLORREF clr)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_crTTipBackClr = clr;
}

COLORREF CExGridCtrlApp::GetTitleTipBackClr()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_crTTipBackClr;
}

void CExGridCtrlApp::SetTitleTipTextClr(COLORREF clr)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_crTTipTextClr = clr;
}

COLORREF CExGridCtrlApp::GetTitleTipTextClr()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_crTTipTextClr;
}

void CExGridCtrlApp::SetFixedGridLineColor_Light(COLORREF clr)
{ 
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_crFixedGridLineColour_Light = clr;
}

void CExGridCtrlApp::SetFixedGridLineColor_Dark(COLORREF clr)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_crFixedGridLineColour_Dark = clr;
}

COLORREF CExGridCtrlApp::GetFixedGridLineColor_Light()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_crFixedGridLineColour_Light;
}

COLORREF CExGridCtrlApp::GetFixedGridLineColor_Dark()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_crFixedGridLineColour_Dark;
}

void CExGridCtrlApp::SetHighlightColor(COLORREF clr)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_crHighlight = clr;
}

void CExGridCtrlApp::SetHighlightTextColor(COLORREF clr)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_crHighlightText = clr;
}

COLORREF CExGridCtrlApp::GetHighlightColor()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_crHighlight;
}

COLORREF CExGridCtrlApp::GetHighlightTextColor()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_crHighlightText;
}

void CExGridCtrlApp::SetUseHighlightText(BOOL bUse)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bUseHighlightText = bUse;	// default : FALSE 로 설정했음.
} 

BOOL CExGridCtrlApp::GetUseHighlightText()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bUseHighlightText;
}

void CExGridCtrlApp::SetFixedRowSelectionHighlight(BOOL bUse)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bFixedRowSelectionHighlight = bUse;
}

BOOL CExGridCtrlApp::GetFixedRowSelectionHighlight()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bFixedRowSelectionHighlight;
}

void CExGridCtrlApp::SetTextColor(COLORREF clr)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_cellDefault.SetTextClr(clr);
}

COLORREF CExGridCtrlApp::GetTextColor()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_cellDefault.GetTextClr();
}

void CExGridCtrlApp::SetTextBkColor(COLORREF clr)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_cellDefault.SetBackClr(clr);
}

COLORREF CExGridCtrlApp::GetTextBkColor()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_cellDefault.GetBackClr();
}

void CExGridCtrlApp::SetFixedTextColor(COLORREF clr)
{ 
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	pMember->m_crFixedTextColour = clr;
    pMember->m_cellFixedColDef.SetTextClr(pMember->m_crFixedTextColour);
    pMember->m_cellFixedRowDef.SetTextClr(pMember->m_crFixedTextColour);
    pMember->m_cellFixedRowColDef.SetTextClr(pMember->m_crFixedTextColour);
}

COLORREF CExGridCtrlApp::GetFixedTextColor()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_cellFixedRowDef.GetTextClr();
}

void CExGridCtrlApp::SetFixedBkColor(COLORREF clr)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
 
	pMember->m_crFixedBkColour = clr;
    pMember->m_cellFixedColDef.SetBackClr(pMember->m_crFixedBkColour);
    pMember->m_cellFixedRowDef.SetBackClr(pMember->m_crFixedBkColour);
    pMember->m_cellFixedRowColDef.SetBackClr(pMember->m_crFixedBkColour);
}

void CExGridCtrlApp::SetGridColor(COLORREF clr)
{
	SetGridLineColor(clr);
}

COLORREF CExGridCtrlApp::GetGridColor()
{
	return GetGridLineColor();
}

void CExGridCtrlApp::SetBkColor(COLORREF clr)
{
	SetGridBkColor(clr);
}

COLORREF CExGridCtrlApp::GetBkColor()
{
	return GetGridBkColor();
}

void CExGridCtrlApp::SetDefCellMargin(int nMargin)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_cellDefault.SetMargin(nMargin); 
	pMember->m_cellFixedRowDef.SetMargin(nMargin); 
	pMember->m_cellFixedColDef.SetMargin(nMargin); 
	pMember->m_cellFixedRowColDef.SetMargin(nMargin);
}

int CExGridCtrlApp::GetDefCellMargin()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_cellDefault.GetMargin();
}

int CExGridCtrlApp::GetDefCellHeight()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_cellDefault.GetHeight();
}

void CExGridCtrlApp::SetDefCellHeight(int nHeight)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_cellDefault.SetHeight(nHeight); 
	pMember->m_cellFixedRowDef.SetHeight(nHeight); 
	pMember->m_cellFixedColDef.SetHeight(nHeight); 
	pMember->m_cellFixedRowColDef.SetHeight(nHeight);
}

int CExGridCtrlApp::GetDefCellWidth()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_cellDefault.GetWidth();
}

void CExGridCtrlApp::SetDefCellWidth(int nWidth)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_cellDefault.SetWidth(nWidth); 
	pMember->m_cellFixedRowDef.SetWidth(nWidth); 
	pMember->m_cellFixedColDef.SetWidth(nWidth); 
	pMember->m_cellFixedRowColDef.SetWidth(nWidth);
}

int CExGridCtrlApp::GetSelectedCount()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return (int)pMember->m_SelectedCellMap.GetCount();
}

CCellID CExGridCtrlApp::GetFocusCell()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_idCurrentCell;
}

BOOL CExGridCtrlApp::GetVirtualMode()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bVirtualMode;
}

void CExGridCtrlApp::SetCallbackFunc(GRIDCALLBACK pCallback, LPARAM lParam)
{ 
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_pfnCallback = pCallback;
	pMember->m_lParam = lParam;
}

GRIDCALLBACK CExGridCtrlApp::GetCallbackFunc()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_pfnCallback;
}

void CExGridCtrlApp::SetImageList(CImageList* pList)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_pImageList = pList;
}

CImageList* CExGridCtrlApp::GetImageList()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_pImageList;
}

int CExGridCtrlApp::GetGridLines()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_nGridLines;
}

void CExGridCtrlApp::SetEditable(BOOL bEditable)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bEditable = bEditable;
}

BOOL CExGridCtrlApp::IsEditable()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bEditable;
}

BOOL CExGridCtrlApp::GetListMode()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bListMode;
}

void CExGridCtrlApp::SetSingleRowSelection(BOOL bSing)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bSingleRowSelection = bSing;
}

BOOL CExGridCtrlApp::GetSingleRowSelection()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bSingleRowSelection & pMember->m_bListMode;
}

void CExGridCtrlApp::SetSingleColSelection(BOOL bSing)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bSingleColSelection = bSing;
}

BOOL CExGridCtrlApp::GetSingleColSelection()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bSingleColSelection;
}

void CExGridCtrlApp::EnableSelection(BOOL bEnable)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	ResetSelectedRange(); 
	pMember->m_bEnableSelection = bEnable; 
	ResetSelectedRange();
}

BOOL CExGridCtrlApp::IsSelectable()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bEnableSelection;
}

void CExGridCtrlApp::SetFixedColumnSelection(BOOL bSelect)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bFixedColumnSelection = bSelect;
}

BOOL CExGridCtrlApp::GetFixedColumnSelection()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bFixedColumnSelection;
}

void CExGridCtrlApp::SetFixedRowSelection(BOOL bSelect)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bFixedRowSelection = bSelect;
}

BOOL CExGridCtrlApp::GetFixedRowSelection()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bFixedRowSelection;
}

void CExGridCtrlApp::EnableDragAndDrop(BOOL bAllow)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bAllowDragAndDrop = bAllow;
}

BOOL CExGridCtrlApp::GetDragAndDrop()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bAllowDragAndDrop;
}

void CExGridCtrlApp::SetRowResize(BOOL bResize)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bAllowRowResize = bResize;
}

BOOL CExGridCtrlApp::GetRowResize()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bAllowRowResize;
}

void CExGridCtrlApp::SetColumnResize(BOOL bResize)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bAllowColumnResize = bResize;
}

BOOL CExGridCtrlApp::GetColumnResize()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bAllowColumnResize;
}

void CExGridCtrlApp::SetHeaderSort(BOOL bSortOnClick)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bSortOnClick = bSortOnClick;
}

BOOL CExGridCtrlApp::GetHeaderSort()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bSortOnClick;
}

void CExGridCtrlApp::SetHandleTabKey(BOOL bHandleTab)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bHandleTabKey = bHandleTab;
}

BOOL CExGridCtrlApp::GetHandleTabKey()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bHandleTabKey;
}

void CExGridCtrlApp::SetDoubleBuffering(BOOL bBuffer)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bDoubleBuffer = bBuffer;
}

BOOL CExGridCtrlApp::GetDoubleBuffering()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bDoubleBuffer;
}

void CExGridCtrlApp::EnableTitleTips(BOOL bEnable)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bTitleTips = bEnable;
}

BOOL CExGridCtrlApp::GetTitleTips()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bTitleTips;
}

int CExGridCtrlApp::GetSortColumn()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_nSortColumn;
}

void CExGridCtrlApp::SetSortAscending(BOOL bAscending)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bAscending = bAscending;
}

BOOL CExGridCtrlApp::GetSortAscending()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bAscending;
}

void CExGridCtrlApp::SetTrackFocusCell(BOOL bTrack)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bTrackFocusCell = bTrack;
}

BOOL CExGridCtrlApp::GetTrackFocusCell()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bTrackFocusCell;
}

void CExGridCtrlApp::SetFrameFocusCell(BOOL bFrame)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bFrameFocus = bFrame;
}

BOOL CExGridCtrlApp::GetFrameFocusCell()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bFrameFocus;
}

void CExGridCtrlApp::SetAutoSizeStyle(int nStyle)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_nAutoSizeColumnStyle = nStyle;
}

int CExGridCtrlApp::GetAutoSizeStyle()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_nAutoSizeColumnStyle;
}

void CExGridCtrlApp::EnableHiddenColUnhide(BOOL bEnable)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bHiddenColUnhide = bEnable;
}

BOOL CExGridCtrlApp::GetHiddenColUnhide()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bHiddenColUnhide;
}

void CExGridCtrlApp::EnableHiddenRowUnhide(BOOL bEnable)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bHiddenRowUnhide = bEnable;
}

BOOL CExGridCtrlApp::GetHiddenRowUnhide()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bHiddenRowUnhide;
}

void CExGridCtrlApp::EnableColumnHide(BOOL bEnable)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bAllowColHide = bEnable;
}

BOOL CExGridCtrlApp::GetColumnHide()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bAllowColHide;
}

void CExGridCtrlApp::EnableRowHide(BOOL bEnable)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bAllowRowHide = bEnable;
}

BOOL CExGridCtrlApp::GetRowHide()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bAllowRowHide;
}

void CExGridCtrlApp::SetDefaultReadOnly(BOOL bReadOnly)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bDefaultReadOnlyCell = bReadOnly;
}

BOOL CExGridCtrlApp::IsDefaultReadOnly()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bDefaultReadOnlyCell;
}

void CExGridCtrlApp::EnsureVisible(CCellID &cell)
{
	EnsureVisible(cell.row, cell.col);
}

int CExGridCtrlApp::SetMouseMode(int nMode)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	int nOldMode = pMember->m_MouseMode; 
	pMember->m_MouseMode = nMode; 
	
	return nOldMode;
}

int CExGridCtrlApp::GetMouseMode()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_MouseMode;
}

BOOL CExGridCtrlApp::IsVisibleVScroll()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return ( (pMember->m_nBarState & GVL_VERT) > 0);
} 

BOOL CExGridCtrlApp::IsVisibleHScroll()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return ( (pMember->m_nBarState & GVL_HORZ) > 0);
}

BOOL CExGridCtrlApp::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();
    //HINSTANCE hInst = AfxGetResourceHandle();

    if (!(::GetClassInfo(hInst, GRIDCTRL_CLASSNAME, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
#ifndef _WIN32_WCE_NO_CURSOR
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
#else
        wndcls.hCursor          = 0;
#endif
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = GRIDCTRL_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CExGridCtrlApp::Initialise()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    // Stop re-entry problems
    static BOOL bInProcedure = FALSE;
    if (bInProcedure)
        return FALSE;
    bInProcedure = TRUE;

#ifndef GRIDCONTROL_NO_TITLETIPS
    pMember->m_TitleTip.SetParentWnd(pMember->m_pGrid);
#endif

	// pMember->m_pGrid would be a good place to register the droptarget but
	// unfortunately pMember->m_pGrid causes problems if you are using the 
	// grid in a view. 
	// Moved from OnSize.
//#ifndef GRIDCONTROL_NO_DRAGDROP
//    pMember->m_DropTarget.Register(pMember->m_pGrid);
//#endif

	// ojtaso
//    if (::IsWindow(pMember->m_pGrid->m_hWnd))
//        pMember->m_pGrid->ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	// Kludge: Make sure the client edge shows
	// pMember->m_pGrid is so horrible it makes my eyes water.
	CRect rect;
	pMember->m_pGrid->GetWindowRect(rect);
	CWnd* pParent = pMember->m_pGrid->GetParent();
	if (pParent != NULL)
		pParent->ScreenToClient(rect);
	rect.InflateRect(1,1);	pMember->m_pGrid->MoveWindow(rect);
	rect.DeflateRect(1,1);  pMember->m_pGrid->MoveWindow(rect);
 
    bInProcedure = FALSE;
    return TRUE;
}

// creates the control - use like any other window create control
BOOL CExGridCtrlApp::Create(const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwStyle)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    ASSERT(pParentWnd->GetSafeHwnd());

 //   if (!pMember->m_pGrid->CWnd::Create(GRIDCTRL_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID))
 //       return FALSE;

    //Initialise(); - called in PreSubclassWnd

    // The number of rows and columns will only be non-zero if the constructor
    // was called with non-zero initialising parameters. If pMember->m_pGrid window was created
    // using a dialog template then the number of rows and columns will be 0 (which
    // means that the code below will not be needed - which is lucky 'cause it ain't
    // gonna get called in a dialog-template-type-situation.

    TRY
    {
        pMember->m_arRowHeights.SetSize(pMember->m_nRows);    // initialize row heights
        pMember->m_arColWidths.SetSize(pMember->m_nCols);     // initialize column widths
    }
    CATCH (CMemoryException, e)
    {
        e->ReportError();
        return FALSE;
    }
    END_CATCH

    int i;
    for (i = 0; i < pMember->m_nRows; i++)
        pMember->m_arRowHeights[i] = pMember->m_cellDefault.GetHeight();
    for (i = 0; i < pMember->m_nCols; i++)
        pMember->m_arColWidths[i] = pMember->m_cellDefault.GetWidth();

    return TRUE;
}

void CExGridCtrlApp::SetupDefaultCells()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    pMember->m_cellDefault.SetGrid((CExGridCtrl*)pMember->m_pGrid);            // Normal editable cell
    pMember->m_cellFixedColDef.SetGrid((CExGridCtrl*)pMember->m_pGrid);        // Cell for fixed columns
    pMember->m_cellFixedRowDef.SetGrid((CExGridCtrl*)pMember->m_pGrid);        // Cell for fixed rows
    pMember->m_cellFixedRowColDef.SetGrid((CExGridCtrl*)pMember->m_pGrid);     // Cell for area overlapped by fixed columns/rows

    pMember->m_cellDefault.SetTextClr(pMember->m_crWindowText);   
    pMember->m_cellDefault.SetBackClr(pMember->m_crWindowColour); 
    pMember->m_cellFixedColDef.SetTextClr(pMember->m_crFixedTextColour);
    pMember->m_cellFixedColDef.SetBackClr(pMember->m_crFixedBkColour);
    pMember->m_cellFixedRowDef.SetTextClr(pMember->m_crFixedTextColour);
    pMember->m_cellFixedRowDef.SetBackClr(0);
    pMember->m_cellFixedRowColDef.SetTextClr(pMember->m_crFixedTextColour);
    pMember->m_cellFixedRowColDef.SetBackClr(pMember->m_crFixedBkColour);
}

void CExGridCtrlApp::PreSubclassWindow()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    pMember->m_pGrid->CWnd::PreSubclassWindow();

    //HFONT hFont = ::CreateFontIndirect(pMember->m_cellDefault.GetFont());
    //OnSetFont((LPARAM)hFont, 0);
    //DeleteObject(hFont);

    Initialise();
}

// 추가 20050426 LYH : Grid 를 parent 로 삼는 컨트롤의 메세지를 받도록 설정한다.
LRESULT CExGridCtrlApp::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	if(message == WM_COMMAND)
	{
		CWnd *pOwner = pMember->m_pGrid->GetOwner();
		if (pOwner && IsWindow(pOwner->m_hWnd))
			return pOwner->SendMessage(message, wParam, lParam);
		else
			return 0;
	}

	return pMember->m_pGrid->CWnd::WindowProc(message, wParam, lParam);
}

// Sends a message to the parent in the form of a WM_NOTIFY message with
// a NM_GRIDVIEW structure attached
LRESULT CExGridCtrlApp::SendMessageToParent(int nRow, int nCol, int nMessage)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (!IsWindow(pMember->m_pGrid->m_hWnd))
        return 0;

    NM_GRIDVIEW nmgv;
    nmgv.iRow         = nRow;
    nmgv.iColumn      = nCol;
 	nmgv.iClickRow	  = pMember->m_LeftClickDownCell.row;
	nmgv.iClickCol	  = pMember->m_LeftClickDownCell.col;
	nmgv.hdr.hwndFrom = pMember->m_pGrid->m_hWnd;
    nmgv.hdr.idFrom   = pMember->m_pGrid->GetDlgCtrlID();
    nmgv.hdr.code     = nMessage;

    CWnd *pOwner = pMember->m_pGrid->GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
        return pOwner->SendMessage(WM_NOTIFY, nmgv.hdr.idFrom, (LPARAM)&nmgv);
    else
        return 0;
}

void CExGridCtrlApp::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    CWnd *pOwner = pMember->m_pGrid->GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
        pOwner->SendMessage(WM_CONTEXTMENU, (WPARAM)pWnd, MAKELPARAM(point.x, point.y));
}


// Send a request to the parent to return information on a given cell
LRESULT CExGridCtrlApp::SendDisplayRequestToParent(GV_DISPINFO* pDisplayInfo)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (!IsWindow(pMember->m_pGrid->m_hWnd))
        return 0;

    // Fix up the message headers
    pDisplayInfo->hdr.hwndFrom = pMember->m_pGrid->m_hWnd;
    pDisplayInfo->hdr.idFrom   = pMember->m_pGrid->GetDlgCtrlID();
    pDisplayInfo->hdr.code     = GVN_GETDISPINFO;

    // Send the message
    CWnd *pOwner = pMember->m_pGrid->GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
        return pOwner->SendMessage(WM_NOTIFY, pDisplayInfo->hdr.idFrom, (LPARAM)pDisplayInfo);
    else
        return 0;
}

// Send a hint to the parent about caching information
LRESULT CExGridCtrlApp::SendCacheHintToParent(const CCellRange& range)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (!IsWindow(pMember->m_pGrid->m_hWnd))
        return 0;

    GV_CACHEHINT CacheHint;

    // Fix up the message headers
    CacheHint.hdr.hwndFrom = pMember->m_pGrid->m_hWnd;
    CacheHint.hdr.idFrom   = pMember->m_pGrid->GetDlgCtrlID();
    CacheHint.hdr.code     = GVN_ODCACHEHINT;

    CacheHint.range = range;

    // Send the message
    CWnd *pOwner = pMember->m_pGrid->GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
        return pOwner->SendMessage(WM_NOTIFY, CacheHint.hdr.idFrom, (LPARAM)&CacheHint);
    else
        return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CExGridCtrl message handlers

void CExGridCtrlApp::OnPaint()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    CPaintDC dc(pMember->m_pGrid);      // device context for painting

    if (pMember->m_bDoubleBuffer)    // Use a memory DC to remove flicker
    {
        CMemDC MemDC(&dc);
        OnDraw(&MemDC);
    }
    else                    // Draw raw - pMember->m_pGrid helps in debugging vis problems.
        OnDraw(&dc);
}

BOOL CExGridCtrlApp::OnEraseBkgnd(CDC* /*pDC*/)
{
    return TRUE;    // Don't erase the background.
}

// Custom background erasure. pMember->m_pGrid gets called from within the OnDraw function,
// since we will (most likely) be using a memory DC to stop flicker. If we just
// erase the background normally through OnEraseBkgnd, and didn't fill the memDC's
// selected bitmap with colour, then all sorts of vis problems would occur
void CExGridCtrlApp::EraseBkgnd(CDC* pDC)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    CRect  VisRect, ClipRect, rect;
    CBrush FixedRowColBack(GetDefaultCell(TRUE, TRUE)->GetBackClr()),
           FixedRowBack(GetDefaultCell(TRUE, FALSE)->GetBackClr()),
           FixedColBack(GetDefaultCell(FALSE, TRUE)->GetBackClr()),
           TextBack(GetDefaultCell(FALSE, FALSE)->GetBackClr());
    CBrush Back(GetGridBkColor()); 
    //CBrush Back(GetTextBkColor());

    if (pDC->GetClipBox(ClipRect) == ERROR)
        return;
    GetVisibleNonFixedCellRange(VisRect);

    int nFixedColumnWidth = GetFixedColumnWidth();
    int nFixedRowHeight = GetFixedRowHeight();

    // Draw Fixed row/column background
    if (ClipRect.left < nFixedColumnWidth && ClipRect.top < nFixedRowHeight)
        pDC->FillRect(CRect(ClipRect.left, ClipRect.top, 
                      nFixedColumnWidth, nFixedRowHeight),
                      &FixedRowColBack);

    // Draw Fixed columns background
    if (ClipRect.left < nFixedColumnWidth && ClipRect.top < VisRect.bottom)
        pDC->FillRect(CRect(ClipRect.left, ClipRect.top, 
                      nFixedColumnWidth, VisRect.bottom),
                      &FixedColBack);
        
    // Draw Fixed rows background
    if (ClipRect.top < nFixedRowHeight && 
        ClipRect.right > nFixedColumnWidth && ClipRect.left < VisRect.right)
        pDC->FillRect(CRect(nFixedColumnWidth-1, ClipRect.top,
                      VisRect.right, nFixedRowHeight),
                      &FixedRowBack);

    // Draw non-fixed cell background
    if (rect.IntersectRect(VisRect, ClipRect)) 
    {
        CRect CellRect(max(nFixedColumnWidth, rect.left), 
                       max(nFixedRowHeight, rect.top),
                       rect.right, rect.bottom);
        pDC->FillRect(CellRect, &TextBack);
    }

    // Draw right hand side of window outside grid
    if (VisRect.right < ClipRect.right) 
        pDC->FillRect(CRect(VisRect.right, ClipRect.top, 
                      ClipRect.right, ClipRect.bottom),
                      &Back);

    // Draw bottom of window below grid
    if (VisRect.bottom < ClipRect.bottom && ClipRect.left < VisRect.right) 
        pDC->FillRect(CRect(ClipRect.left, VisRect.bottom,
                      VisRect.right, ClipRect.bottom),
                      &Back);
}

void CExGridCtrlApp::OnSize(UINT nType, int cx, int cy)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
  
    static BOOL bAlreadyInsideThisProcedure = FALSE;
    if (bAlreadyInsideThisProcedure)
        return;

    if (!::IsWindow(pMember->m_pGrid->m_hWnd))
        return;

	// pMember->m_pGrid is not the ideal place to register the droptarget
#ifndef GRIDCONTROL_NO_DRAGDROP
	pMember->m_DropTarget.Register((CExGridCtrl*)pMember->m_pControl);
#endif

    // Start re-entry blocking
    bAlreadyInsideThisProcedure = TRUE;

    EndEditing();        // destroy any InPlaceEdit's
    pMember->m_pGrid->CWnd::OnSize(nType, cx, cy);
    ResetScrollBars();

    // End re-entry blocking
    bAlreadyInsideThisProcedure = FALSE;
}

UINT CExGridCtrlApp::OnGetDlgCode()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    UINT nCode = DLGC_WANTARROWS | DLGC_WANTCHARS; // DLGC_WANTALLKEYS; //

    if (pMember->m_bHandleTabKey && !IsCTRLpressed())
        nCode |= DLGC_WANTTAB;

    return nCode;
}

#ifndef _WIN32_WCE
// If system colours change, then redo colours
void CExGridCtrlApp::OnSysColorChange()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    pMember->m_pGrid->CWnd::OnSysColorChange();

    if (GetDefaultCell(FALSE, FALSE)->GetTextClr() == pMember->m_crWindowText)                   // Still using system colours
        GetDefaultCell(FALSE, FALSE)->SetTextClr(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetDefaultCell(FALSE, FALSE)->GetBackClr() == pMember->m_crWindowColour)
        GetDefaultCell(FALSE, FALSE)->SetBackClr(::GetSysColor(COLOR_WINDOW));

    if (GetDefaultCell(TRUE, FALSE)->GetTextClr() == pMember->m_crWindowText)                   // Still using system colours
        GetDefaultCell(TRUE, FALSE)->SetTextClr(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetDefaultCell(TRUE, FALSE)->GetBackClr() == pMember->m_crWindowColour)
        GetDefaultCell(TRUE, FALSE)->SetBackClr(::GetSysColor(COLOR_WINDOW));

    if (GetDefaultCell(FALSE, TRUE)->GetTextClr() == pMember->m_crWindowText)                   // Still using system colours
        GetDefaultCell(FALSE, TRUE)->SetTextClr(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetDefaultCell(FALSE, TRUE)->GetBackClr() == pMember->m_crWindowColour)
        GetDefaultCell(FALSE, TRUE)->SetBackClr(::GetSysColor(COLOR_WINDOW));

    if (GetDefaultCell(TRUE, TRUE)->GetTextClr() == pMember->m_crWindowText)                   // Still using system colours
        GetDefaultCell(TRUE, TRUE)->SetTextClr(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetDefaultCell(TRUE, TRUE)->GetBackClr() == pMember->m_crWindowColour)
        GetDefaultCell(TRUE, TRUE)->SetBackClr(::GetSysColor(COLOR_WINDOW));

/*
    if (GetGridBkColor() == pMember->m_crShadow)
        SetGridBkColor(::GetSysColor(COLOR_3DSHADOW));

    pMember->m_crWindowText   = ::GetSysColor(COLOR_WINDOWTEXT);
    pMember->m_crWindowColour = ::GetSysColor(COLOR_WINDOW);
    pMember->m_cr3DFace       = ::GetSysColor(COLOR_3DFACE);
    pMember->m_crShadow       = ::GetSysColor(COLOR_3DSHADOW);
*/
}
#endif

#ifndef _WIN32_WCE_NO_CURSOR
// If we are drag-selecting cells, or drag and dropping, stop now
void CExGridCtrlApp::OnCaptureChanged(CWnd *pWnd)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (pWnd->GetSafeHwnd() == pMember->m_pGrid->GetSafeHwnd())
        return;

    // kill timer if active
    if (pMember->m_nTimerID != 0)
    {
        pMember->m_pGrid->KillTimer(pMember->m_nTimerID);
        pMember->m_nTimerID = 0;
    }

#ifndef GRIDCONTROL_NO_DRAGDROP
    // Kill drag and drop if active
    if (pMember->m_MouseMode == MOUSE_DRAGGING)
        pMember->m_MouseMode = MOUSE_NOTHING;
#endif
}
#endif

#if (_MFC_VER >= 0x0421) || (_WIN32_WCE >= 210)
// If system settings change, then redo colours
void CExGridCtrlApp::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    pMember->m_pGrid->CWnd::OnSettingChange(uFlags, lpszSection);

    if (GetDefaultCell(FALSE, FALSE)->GetTextClr() == pMember->m_crWindowText)                   // Still using system colours
        GetDefaultCell(FALSE, FALSE)->SetTextClr(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetDefaultCell(FALSE, FALSE)->GetBackClr() == pMember->m_crWindowColour)
        GetDefaultCell(FALSE, FALSE)->SetBackClr(::GetSysColor(COLOR_WINDOW));

    if (GetDefaultCell(TRUE, FALSE)->GetTextClr() == pMember->m_crWindowText)                   // Still using system colours
        GetDefaultCell(TRUE, FALSE)->SetTextClr(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetDefaultCell(TRUE, FALSE)->GetBackClr() == pMember->m_crWindowColour)
        GetDefaultCell(TRUE, FALSE)->SetBackClr(::GetSysColor(COLOR_WINDOW));

    if (GetDefaultCell(FALSE, TRUE)->GetTextClr() == pMember->m_crWindowText)                   // Still using system colours
        GetDefaultCell(FALSE, TRUE)->SetTextClr(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetDefaultCell(FALSE, TRUE)->GetBackClr() == pMember->m_crWindowColour)
        GetDefaultCell(FALSE, TRUE)->SetBackClr(::GetSysColor(COLOR_WINDOW));

    if (GetDefaultCell(TRUE, TRUE)->GetTextClr() == pMember->m_crWindowText)                   // Still using system colours
        GetDefaultCell(TRUE, TRUE)->SetTextClr(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetDefaultCell(TRUE, TRUE)->GetBackClr() == pMember->m_crWindowColour)
        GetDefaultCell(TRUE, TRUE)->SetBackClr(::GetSysColor(COLOR_WINDOW));

/*
    if (GetGridBkColor() == pMember->m_crShadow)
        SetGridBkColor(::GetSysColor(COLOR_3DSHADOW));

    pMember->m_crWindowText   = ::GetSysColor(COLOR_WINDOWTEXT);
    pMember->m_crWindowColour = ::GetSysColor(COLOR_WINDOW);
    pMember->m_cr3DFace       = ::GetSysColor(COLOR_3DFACE);
    pMember->m_crShadow       = ::GetSysColor(COLOR_3DSHADOW);
*/

    pMember->m_nRowsPerWheelNotch = GetMouseScrollLines(); // Get the number of lines
}
#endif

// For drag-selection. Scrolls hidden cells into view
// TODO: decrease timer interval over time to speed up selection over time
void CExGridCtrlApp::OnTimer(UINT nIDEvent)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    ASSERT(nIDEvent == WM_LBUTTONDOWN);
    if (nIDEvent != WM_LBUTTONDOWN)
        return;

    CPoint pt, origPt;

#ifdef _WIN32_WCE
    if (pMember->m_MouseMode == MOUSE_NOTHING)
        return;
    origPt = GetMessagePos();
#else
    if (!GetCursorPos(&origPt))
        return;
#endif

    pMember->m_pGrid->ScreenToClient(&origPt);

    CRect rect;
    pMember->m_pGrid->GetClientRect(rect);

    int nFixedRowHeight = GetFixedRowHeight();
    int nFixedColWidth = GetFixedColumnWidth();

    pt = origPt;
    if (pt.y > rect.bottom)
    {
        //SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
        pMember->m_pGrid->SendMessage(WM_KEYDOWN, VK_DOWN, 0);

        if (pt.x < rect.left)
            pt.x = rect.left;
        if (pt.x > rect.right)
            pt.x = rect.right;
        pt.y = rect.bottom;
        OnSelecting(GetCellFromPt(pt));
    }
    else if (pt.y < nFixedRowHeight)
    {
        //SendMessage(WM_VSCROLL, SB_LINEUP, 0);
        pMember->m_pGrid->SendMessage(WM_KEYDOWN, VK_UP, 0);

        if (pt.x < rect.left)
            pt.x = rect.left;
        if (pt.x > rect.right)
            pt.x = rect.right;
        pt.y = nFixedRowHeight + 1;
        OnSelecting(GetCellFromPt(pt));
    }

    pt = origPt;
    if (pt.x > rect.right)
    {
        // SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
        pMember->m_pGrid->SendMessage(WM_KEYDOWN, VK_RIGHT, 0);

        if (pt.y < rect.top)
            pt.y = rect.top;
        if (pt.y > rect.bottom)
            pt.y = rect.bottom;
        pt.x = rect.right;
        OnSelecting(GetCellFromPt(pt));
    }
    else if (pt.x < nFixedColWidth)
    {
        //SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
        pMember->m_pGrid->SendMessage(WM_KEYDOWN, VK_LEFT, 0);

        if (pt.y < rect.top)
            pt.y = rect.top;
        if (pt.y > rect.bottom)
            pt.y = rect.bottom;
        pt.x = nFixedColWidth + 1;
        OnSelecting(GetCellFromPt(pt));
    }
}

// move about with keyboard
void CExGridCtrlApp::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (!IsValid(pMember->m_idCurrentCell))
    {
        pMember->m_pGrid->CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
        return;
    }

    CCellID next = pMember->m_idCurrentCell;
    BOOL bChangeLine = FALSE;
	BOOL bHorzScrollAction = FALSE;
	BOOL bVertScrollAction = FALSE;

    if (IsCTRLpressed())
    {
        switch (nChar)
        {
        case 'A':
            OnEditSelectAll();
            break;
#ifndef GRIDCONTROL_NO_CLIPBOARD
        case 'X':
            OnEditCut();
            break;
        case VK_INSERT:
        case 'C':
            OnEditCopy();
            break;
        case 'V':
            OnEditPaste();
            break;
#endif
        }
    }

#ifndef GRIDCONTROL_NO_CLIPBOARD
    if (IsSHIFTpressed() &&(nChar == VK_INSERT))
        OnEditPaste();
#endif

    BOOL bFoundVisible;
    int iOrig;

    if (nChar == VK_DELETE)
    {
		CutSelectedText();
    }
    else if (nChar == VK_DOWN)
    {
        // don't let user go to a hidden row
        bFoundVisible = FALSE;
        iOrig = next.row;
        next.row++;
        while( next.row < GetRowCount())
        {
            if( GetRowHeight( next.row) > 0)
            {
                bFoundVisible = TRUE;
                break;
            }
            next.row++;
        }
        if( !bFoundVisible)
            next.row = iOrig;
    }
    else if (nChar == VK_UP)
    {
        // don't let user go to a hidden row
        bFoundVisible = FALSE;
        iOrig = next.row;
        next.row--;
        while( next.row >= pMember->m_nFixedRows)
        {
            if( GetRowHeight( next.row) > 0)
            {
                bFoundVisible = TRUE;
                break;
            }
            next.row--;
        }
        if( !bFoundVisible)
            next.row = iOrig;
    }
    else if (nChar == VK_RIGHT || (nChar == VK_TAB && !IsSHIFTpressed()) )
    {
        // don't let user go to a hidden column
        bFoundVisible = FALSE;
        iOrig = next.col;
        next.col++;

        if (nChar == VK_TAB)
        {
			// If we're at the end of a row, go down a row till we find a non-hidden row
            if (next.col == (GetColumnCount()) && next.row < (GetRowCount() - 1))
            {
				next.row++;
				while( next.row < GetRowCount())
				{
					if( GetRowHeight(next.row) > 0)
					{
						bFoundVisible = TRUE;
						break;
					}
					next.row++; 
				}

                next.col = pMember->m_nFixedCols;	// Place focus on first non-fixed column
                bChangeLine = TRUE;
            }
            else
                pMember->m_pGrid->CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
        }

		// We're on a non-hidden row, so look across for the next non-hidden column
        while( next.col < GetColumnCount())
        {
            if( GetColumnWidth( next.col) > 0)
            {
                bFoundVisible = TRUE;
                break;
            }
            next.col++;
        }

		// If nothing worked then don't bother
        if( !bFoundVisible)
            next.col = iOrig;
    }
    else if (nChar == VK_LEFT || (nChar == VK_TAB && IsSHIFTpressed()) )
    {
        // don't let user go to a hidden column
        bFoundVisible = FALSE;
        iOrig = next.col;
        next.col--;

        if (nChar == VK_TAB)
        {
            if (next.col == (GetFixedColumnCount()-1) && next.row > GetFixedRowCount())
            {
                next.row--;
				while( next.row > GetFixedRowCount())
				{
					if( GetRowHeight(next.row) > 0)
					{
						bFoundVisible = TRUE;
						break;
					}
					next.row--; 
				}

                next.col = GetColumnCount() - 1; 
                bChangeLine = TRUE;
            }
            else
                pMember->m_pGrid->CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
        }

        while( next.col >= pMember->m_nFixedCols)
        {
            if( GetColumnWidth( next.col) > 0)
            {
                bFoundVisible = TRUE;
                break;
            }
            next.col--;
        }
        if( !bFoundVisible)
            next.col = iOrig;
    }
    else if (nChar == VK_NEXT)
    {
        CCellID idOldTopLeft = GetTopleftNonFixedCell();
        pMember->m_pGrid->SendMessage(WM_VSCROLL, SB_PAGEDOWN, 0);
		bVertScrollAction = TRUE;
        CCellID idNewTopLeft = GetTopleftNonFixedCell();

        int increment = idNewTopLeft.row - idOldTopLeft.row;
        if (increment)
        {
            next.row += increment;
            if (next.row >(GetRowCount() - 1))
                next.row = GetRowCount() - 1;
        }
        else
            next.row = GetRowCount() - 1;
    }
    else if (nChar == VK_PRIOR)
    {
        CCellID idOldTopLeft = GetTopleftNonFixedCell();
        pMember->m_pGrid->SendMessage(WM_VSCROLL, SB_PAGEUP, 0);
		bVertScrollAction = TRUE;
        CCellID idNewTopLeft = GetTopleftNonFixedCell();
            
        int increment = idNewTopLeft.row - idOldTopLeft.row;
        if (increment) 
        {
            next.row += increment;
            if (next.row < pMember->m_nFixedRows) 
                next.row = pMember->m_nFixedRows;
        }
        else
            next.row = pMember->m_nFixedRows;
    }
    else if (nChar == VK_HOME)
    {
        // Home and Ctrl-Home work more like Excel
        //  and don't let user go to a hidden cell
        if (IsCTRLpressed())
        {
            pMember->m_pGrid->SendMessage(WM_VSCROLL, SB_TOP, 0);
            pMember->m_pGrid->SendMessage(WM_HSCROLL, SB_LEFT, 0);
			bVertScrollAction = TRUE;
			bHorzScrollAction = TRUE;
            next.row = pMember->m_nFixedRows;
            next.col = pMember->m_nFixedCols;
        }
        else
        {
            pMember->m_pGrid->SendMessage(WM_HSCROLL, SB_LEFT, 0);
 			bHorzScrollAction = TRUE;
            next.col = pMember->m_nFixedCols;
        }
        // adjust column to avoid hidden columns and rows
        while( next.col < GetColumnCount() - 1)
        {
            if( GetColumnWidth( next.col) > 0)
                break;
            next.col++;
        }
        while( next.row < GetRowCount() - 1)
        {
            if( GetRowHeight( next.row) > 0)
                break;
            next.row++;
        }
    }
    else if (nChar == VK_END)
    {
        // End and Ctrl-End work more like Excel
        //  and don't let user go to a hidden cell
        if (IsCTRLpressed())
        {
            pMember->m_pGrid->SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
            pMember->m_pGrid->SendMessage(WM_HSCROLL, SB_RIGHT, 0);
			bHorzScrollAction = TRUE;
			bVertScrollAction = TRUE;
            next.row = GetRowCount() - 1;
            next.col = GetColumnCount() - 1;
        }
        else
        {
            pMember->m_pGrid->SendMessage(WM_HSCROLL, SB_RIGHT, 0);
			bHorzScrollAction = TRUE;
            next.col = GetColumnCount() - 1;
        }
        // adjust column to avoid hidden columns and rows
        while( next.col > pMember->m_nFixedCols + 1)
        {
            if( GetColumnWidth( next.col) > 0)
                break;
            next.col--;
        }
        while( next.row > pMember->m_nFixedRows + 1)
        {
            if( GetRowHeight( next.row) > 0)
                break;
            next.row--;
        }
    }
    else if (nChar == VK_F2)
    {
        OnEditCell(pMember->m_idCurrentCell.row, pMember->m_idCurrentCell.col, CPoint( -1, -1), VK_LBUTTON);
    }
    else
    {
        pMember->m_pGrid->CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
        return;
    }

    if (next != pMember->m_idCurrentCell)
    {
        // While moving with the Cursorkeys the current ROW/CELL will get selected
        // OR Selection will get expanded when SHIFT is pressed
        // Cut n paste from OnLButtonDown - Franco Bez
        // Added check for NULL mouse mode - Chris Maunder.
        if (pMember->m_MouseMode == MOUSE_NOTHING)
        {
            pMember->m_PrevSelectedCellMap.RemoveAll();
            pMember->m_MouseMode = pMember->m_bListMode? MOUSE_SELECT_ROW : MOUSE_SELECT_CELLS;
            if (!IsSHIFTpressed() || nChar == VK_TAB)
                pMember->m_SelectionStartCell = next;

			// Notify parent that selection is changing - Arthur Westerman/Scot Brennecke 
            SendMessageToParent(next.row, next.col, GVN_SELCHANGING);
			OnSelecting(next);
			SendMessageToParent(next.row, next.col, GVN_SELCHANGED);

            pMember->m_MouseMode = MOUSE_NOTHING;
        }

        SetFocusCell(next);

        if (!IsCellVisible(next))
        {

            switch (nChar)
            {
            case VK_RIGHT:  
                pMember->m_pGrid->SendMessage(WM_HSCROLL, SB_LINERIGHT, 0); 
				bHorzScrollAction = TRUE;
                break;
                
            case VK_LEFT:   
                pMember->m_pGrid->SendMessage(WM_HSCROLL, SB_LINELEFT, 0);  
				bHorzScrollAction = TRUE;
                break;
                
            case VK_DOWN:   
                pMember->m_pGrid->SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);  
				bVertScrollAction = TRUE;
                break;
                
            case VK_UP:     
                pMember->m_pGrid->SendMessage(WM_VSCROLL, SB_LINEUP, 0);    
				bVertScrollAction = TRUE;
                break;                
                
            case VK_TAB:    
                if (IsSHIFTpressed())
                {
                    if (bChangeLine) 
                    {
                        pMember->m_pGrid->SendMessage(WM_VSCROLL, SB_LINEUP, 0);
						bVertScrollAction = TRUE;
                        SetScrollPos32(SB_HORZ, pMember->m_nHScrollMax);
                        break;
                    }
                    else 
					{
                        pMember->m_pGrid->SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
						bHorzScrollAction = TRUE;
					}
                }
                else
                {
                    if (bChangeLine) 
                    {
                        pMember->m_pGrid->SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
						bVertScrollAction = TRUE;
                        SetScrollPos32(SB_HORZ, 0);
                        break;
                    }
                    else 
					{
						pMember->m_pGrid->SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
						bHorzScrollAction = TRUE;
					}
                }
                break;
            }
            EnsureVisible(next); // Make sure cell is visible
            pMember->m_pGrid->Invalidate();
        }
        EnsureVisible(next); // Make sure cell is visible

		if (bHorzScrollAction)
			pMember->m_pGrid->SendMessage(WM_HSCROLL, SB_ENDSCROLL, 0);
		if (bVertScrollAction)
			pMember->m_pGrid->SendMessage(WM_VSCROLL, SB_ENDSCROLL, 0);
    }
}

void CExGridCtrlApp::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    pMember->m_pGrid->CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CExGridCtrlApp::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

#ifdef GRIDCONTROL_USE_TITLETIPS
    pMember->m_TitleTip.Hide();  // hide any titletips
#endif

    pMember->m_pGrid->CWnd::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

// Instant editing of cells when keys are pressed
void CExGridCtrlApp::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    // EFW - BUG FIX
    if (!IsCTRLpressed() && pMember->m_MouseMode == MOUSE_NOTHING && nChar != VK_ESCAPE)
    {
        if (!pMember->m_bHandleTabKey || (pMember->m_bHandleTabKey && nChar != VK_TAB))
            OnEditCell(pMember->m_idCurrentCell.row, pMember->m_idCurrentCell.col, CPoint( -1, -1), nChar);
    }

    pMember->m_pGrid->CWnd::OnChar(nChar, nRepCnt, nFlags);
}

// Added by KiteFly
LRESULT CExGridCtrlApp::OnImeChar(WPARAM wCharCode, LPARAM)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    // EFW - BUG FIX
    if (!IsCTRLpressed() && pMember->m_MouseMode == MOUSE_NOTHING && wCharCode != VK_ESCAPE) 
        OnEditCell(pMember->m_idCurrentCell.row, pMember->m_idCurrentCell.col, CPoint( -1, -1), wCharCode);
    return 0;
}

// Callback from any CInPlaceEdits that ended. pMember->m_pGrid just calls OnEndEditCell,
// refreshes the edited cell and moves onto next cell if the return character
// from the edit says we should.
void CExGridCtrlApp::OnEndInPlaceEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    GV_DISPINFO *pgvDispInfo = (GV_DISPINFO *)pNMHDR;
    GV_ITEM     *pgvItem = &pgvDispInfo->item;

    // In case OnEndInPlaceEdit called as window is being destroyed
    if (!IsWindow(pMember->m_pGrid->GetSafeHwnd()))
        return;

    OnEndEditCell(pgvItem->row, pgvItem->col, pgvItem->strText);
    //InvalidateCellRect(CCellID(pgvItem->row, pgvItem->col));

    switch (pgvItem->lParam)
    {
    case VK_TAB:
    case VK_DOWN:
    case VK_UP:
    case VK_RIGHT:
    case VK_LEFT:
    case VK_NEXT:
    case VK_PRIOR:
    case VK_HOME:
    case VK_END:
        OnKeyDown(pgvItem->lParam, 0, 0);
        OnEditCell(pMember->m_idCurrentCell.row, pMember->m_idCurrentCell.col, CPoint( -1, -1), pgvItem->lParam);
    }

    *pResult = 0;
}

// Handle horz scrollbar notifications
void CExGridCtrlApp::OnHScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar* /*pScrollBar*/)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    EndEditing();

#ifndef GRIDCONTROL_NO_TITLETIPS
    pMember->m_TitleTip.Hide();  // hide any titletips
#endif

    int scrollPos = GetScrollPos32(SB_HORZ);

    CCellID idTopLeft = GetTopleftNonFixedCell();

    CRect rect;
    pMember->m_pGrid->GetClientRect(rect);

    switch (nSBCode)
    {
    case SB_LINERIGHT:
        if (scrollPos < pMember->m_nHScrollMax)
        {
            // may have contiguous hidden columns.  Blow by them
            while (idTopLeft.col < (GetColumnCount()-1)
                    && GetColumnWidth( idTopLeft.col) < 1 )
            {
                idTopLeft.col++;
            }
            int xScroll = GetColumnWidth(idTopLeft.col);
            SetScrollPos32(SB_HORZ, scrollPos + xScroll);
            if (GetScrollPos32(SB_HORZ) == scrollPos)
                break;          // didn't work

            rect.left = GetFixedColumnWidth();
            //rect.left = GetFixedColumnWidth() + xScroll;
            //ScrollWindow(-xScroll, 0, rect);
            //rect.left = rect.right - xScroll;
            pMember->m_pGrid->InvalidateRect(rect);
			SendEndScrollNotify(pMember, idTopLeft, rect, SB_HORZ);
       }
        break;

    case SB_LINELEFT:
        if (scrollPos > 0 && idTopLeft.col > GetFixedColumnCount())
        {
            int iColToUse = idTopLeft.col-1;
            // may have contiguous hidden columns.  Blow by them
            while(  iColToUse > GetFixedColumnCount()
                    && GetColumnWidth( iColToUse) < 1 )
            {
                iColToUse--;
            }

            int xScroll = GetColumnWidth(iColToUse);
            SetScrollPos32(SB_HORZ, max(0, scrollPos - xScroll));
            rect.left = GetFixedColumnWidth();
            //ScrollWindow(xScroll, 0, rect);
            //rect.right = rect.left + xScroll;
            pMember->m_pGrid->InvalidateRect(rect);
        }
        break;

    case SB_PAGERIGHT:
        if (scrollPos < pMember->m_nHScrollMax)
        {
            rect.left = GetFixedColumnWidth();
            int offset = rect.Width();
            int pos = min(pMember->m_nHScrollMax, scrollPos + offset);
            SetScrollPos32(SB_HORZ, pos);
            rect.left = GetFixedColumnWidth();
            pMember->m_pGrid->InvalidateRect(rect);
			SendEndScrollNotify(pMember, idTopLeft, rect, SB_HORZ);
        }
      break;
        
    case SB_PAGELEFT:
        if (scrollPos > 0)
        {
            rect.left = GetFixedColumnWidth();
            int offset = -rect.Width();
            int pos = max(0, scrollPos + offset);
            SetScrollPos32(SB_HORZ, pos);
            rect.left = GetFixedColumnWidth();
            pMember->m_pGrid->InvalidateRect(rect);
        }
        break;
        
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
        {
            SetScrollPos32(SB_HORZ, GetScrollPos32(SB_HORZ, TRUE));
            pMember->m_idTopLeftCell.row = -1;
            CCellID idNewTopLeft = GetTopleftNonFixedCell();
            if (idNewTopLeft != idTopLeft)
            {
                rect.left = GetFixedColumnWidth();
                pMember->m_pGrid->InvalidateRect(rect);
				SendEndScrollNotify(pMember, idTopLeft, rect, SB_HORZ);
            }
        }
        break;
        
    case SB_LEFT:
        if (scrollPos > 0)
        {
            SetScrollPos32(SB_HORZ, 0);
            pMember->m_pGrid->Invalidate();
        }
        break;
        
    case SB_RIGHT:
        if (scrollPos < pMember->m_nHScrollMax)
        {
            SetScrollPos32(SB_HORZ, pMember->m_nHScrollMax);
            pMember->m_pGrid->Invalidate();
			SendEndScrollNotify(pMember, idTopLeft, rect, SB_HORZ);
		}
        break;
        
        
    default: 
        break;
    }
}

// Handle vert scrollbar notifications
void CExGridCtrlApp::OnVScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar* /*pScrollBar*/)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    EndEditing();

#ifndef GRIDCONTROL_NO_TITLETIPS
    pMember->m_TitleTip.Hide();  // hide any titletips
#endif

    // Get the scroll position ourselves to ensure we get a 32 bit value
    int scrollPos = GetScrollPos32(SB_VERT);

    CCellID idTopLeft = GetTopleftNonFixedCell();

    CRect rect;
    pMember->m_pGrid->GetClientRect(rect);

    switch (nSBCode)
    {
    case SB_LINEDOWN:
        if (scrollPos < pMember->m_nVScrollMax)
        {
            // may have contiguous hidden rows.  Blow by them
            while(  idTopLeft.row < (GetRowCount()-1)
				&& GetRowHeight( idTopLeft.row) < 1 )
            {
                idTopLeft.row++;
            }
			
            int yScroll = GetRowHeight(idTopLeft.row);
            SetScrollPos32(SB_VERT, scrollPos + yScroll);
			if (GetScrollPos32(SB_VERT) == scrollPos)
                break;          // didn't work
			
            rect.top = GetFixedRowHeight();
            //rect.top = GetFixedRowHeight() + yScroll;
            //ScrollWindow(0, -yScroll, rect);
            //rect.top = rect.bottom - yScroll;
            pMember->m_pGrid->InvalidateRect(rect);
			SendEndScrollNotify(pMember, idTopLeft, rect, SB_VERT);
        }
		break;
        
    case SB_LINEUP:
        if (scrollPos > 0 && idTopLeft.row > GetFixedRowCount())
        {
            int iRowToUse = idTopLeft.row-1;
            // may have contiguous hidden rows.  Blow by them
            while(  iRowToUse > GetFixedRowCount()
				&& GetRowHeight( iRowToUse) < 1 )
            {
                iRowToUse--;
            }
			
            int yScroll = GetRowHeight( iRowToUse);
            SetScrollPos32(SB_VERT, max(0, scrollPos - yScroll));
            rect.top = GetFixedRowHeight();
            //ScrollWindow(0, yScroll, rect);
            //rect.bottom = rect.top + yScroll;
            pMember->m_pGrid->InvalidateRect(rect);
        }
        break;
        
    case SB_PAGEDOWN:
        if (scrollPos < pMember->m_nVScrollMax)
        {
            rect.top = GetFixedRowHeight();
            scrollPos = min(pMember->m_nVScrollMax, scrollPos + rect.Height());
            SetScrollPos32(SB_VERT, scrollPos);
            rect.top = GetFixedRowHeight();
            pMember->m_pGrid->InvalidateRect(rect);
			SendEndScrollNotify(pMember, idTopLeft, rect, SB_VERT);
		}
		break;
        
    case SB_PAGEUP:
        if (scrollPos > 0)
        {
            rect.top = GetFixedRowHeight();
            int offset = -rect.Height();
            int pos = max(0, scrollPos + offset);
            SetScrollPos32(SB_VERT, pos);
            rect.top = GetFixedRowHeight();
            pMember->m_pGrid->InvalidateRect(rect);
        }
        break;
        
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
        {
            SetScrollPos32(SB_VERT, GetScrollPos32(SB_VERT, TRUE));
            pMember->m_idTopLeftCell.row = -1;
            CCellID idNewTopLeft = GetTopleftNonFixedCell();
            if (idNewTopLeft != idTopLeft)
            {
                rect.top = GetFixedRowHeight();
                pMember->m_pGrid->InvalidateRect(rect);
				SendEndScrollNotify(pMember, idTopLeft, rect, SB_VERT);
            }
			
        }
		break;
        
    case SB_TOP:
        if (scrollPos > 0)
        {
            SetScrollPos32(SB_VERT, 0);
            pMember->m_pGrid->Invalidate();
        }
        break;
        
    case SB_BOTTOM:
        if (scrollPos < pMember->m_nVScrollMax)
        {
            SetScrollPos32(SB_VERT, pMember->m_nVScrollMax);
            pMember->m_pGrid->Invalidate();
			SendEndScrollNotify(pMember, idTopLeft, rect, SB_VERT);
		}
		break;
  default: 
        break;
    }
}

void CExGridCtrlApp::SendEndScrollNotify(CMemberDraw* pMember, CCellID& idTopLeft, CRect& rect, UINT nScrollBar)
{
	if(nScrollBar == SB_VERT)
	{
		int nRowCount = GetRowCount();
		int bottom = GetFixedRowHeight();
        int i = 0;
		for ( i = idTopLeft.row; i < nRowCount; i++)
		{
			bottom += GetRowHeight(i);
			if (bottom >= rect.bottom)
			{
				break;
			}
		}
		
		int maxVisibleRow = min(i, nRowCount - 1);
		maxVisibleRow = maxVisibleRow - idTopLeft.row;

		if(idTopLeft.row > pMember->m_nPreVPos && idTopLeft.row + maxVisibleRow == nRowCount - 1)
			pMember->m_pGrid->GetParent()->PostMessage(RMSG_ENDSCROLLPOS, nScrollBar, 0);

		pMember->m_nPreVPos = idTopLeft.row;
	}
	else
	{
		int nColCount = GetColumnCount();
		int right = GetFixedColumnWidth();
        int i = 0;
		for ( i = idTopLeft.col; i < nColCount; i++)
		{
			right += GetColumnWidth(i);
			if (right >= rect.right)
			{
				right = rect.right;
				break;
			}
		}

		int maxVisibleCol = min(i, nColCount - 1);
		maxVisibleCol = maxVisibleCol - idTopLeft.col;

		if(idTopLeft.col > pMember->m_nPreHPos && idTopLeft.col + maxVisibleCol == nColCount - 1)
			pMember->m_pGrid->GetParent()->PostMessage(RMSG_ENDSCROLLPOS, nScrollBar, 0);

		pMember->m_nPreHPos = idTopLeft.col;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CExGridCtrl implementation functions

void CExGridCtrlApp::OnDraw(CDC* pDC)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (!pMember->m_bAllowDraw)
        return;

    CRect clipRect;
    if (pDC->GetClipBox(&clipRect) == ERROR)
        return;

    EraseBkgnd(pDC);            // OnEraseBkgnd does nothing, so erase bkgnd here.
    // pMember->m_pGrid necessary since we may be using a Memory DC.

#ifdef _DEBUG
	LARGE_INTEGER iStartCount;
	QueryPerformanceCounter(&iStartCount);
#endif

    CRect rect;
    int row, col;
    CGridCellBase* pCell;

    int nFixedRowHeight = GetFixedRowHeight();
    int nFixedColWidth  = GetFixedColumnWidth();

    CCellID idTopLeft = GetTopleftNonFixedCell();
    int minVisibleRow = idTopLeft.row,
        minVisibleCol = idTopLeft.col;

    CRect VisRect;
    CCellRange VisCellRange = GetVisibleNonFixedCellRange(VisRect);
    int maxVisibleRow = VisCellRange.GetMaxRow(),
        maxVisibleCol = VisCellRange.GetMaxCol();

    if (GetVirtualMode())
        SendCacheHintToParent(VisCellRange);

    // draw top-left cells 0..pMember->m_nFixedRows-1, 0..pMember->m_nFixedCols-1
    rect.bottom = -1;
    for (row = 0; row < pMember->m_nFixedRows; row++)
    {
        if (GetRowHeight(row) <= 0) continue;

        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(row)-1;
        rect.right = -1;

        for (col = 0; col < pMember->m_nFixedCols; col++)
        {
            if (GetColumnWidth(col) <= 0) continue;

            rect.left = rect.right+1;
            rect.right = rect.left + GetColumnWidth(col)-1;

            pCell = GetCell(row, col);
            if (pCell)
			{
				pCell->SetCoords(row,col);
                pCell->Draw(pDC, row, col, rect, FALSE);
			}
        }
    }

    CPen pen;
    pen.CreatePen(PS_SOLID, 0, pMember->m_crGridLineColour);
    pDC->SelectObject(&pen);

   // draw fixed column cells:  pMember->m_nFixedRows..n, 0..pMember->m_nFixedCols-1
    rect.bottom = nFixedRowHeight-1;
    for (row = minVisibleRow; row <= maxVisibleRow; row++)
    {
        if (GetRowHeight(row) <= 0) continue;

        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(row)-1;

        // rect.bottom = bottom pixel of previous row
        if (rect.top > clipRect.bottom)
            break;                // Gone past cliprect
        if (rect.bottom < clipRect.top)
            continue;             // Reached cliprect yet?

        rect.right = -1;
        for (col = 0; col < pMember->m_nFixedCols; col++)
        {
            if (GetColumnWidth(col) <= 0) continue;

            rect.left = rect.right+1;
            rect.right = rect.left + GetColumnWidth(col)-1;

            if (rect.left > clipRect.right)
                break;            // gone past cliprect
            if (rect.right < clipRect.left)
                continue;         // Reached cliprect yet?

            pCell = GetCell(row, col);
            if (pCell)
			{
				pCell->SetCoords(row,col);
                pCell->Draw(pDC, row, col, rect, FALSE);
			}
        }
    }

    // draw fixed row cells  0..pMember->m_nFixedRows, pMember->m_nFixedCols..n
    rect.bottom = -1;
    for (row = 0; row < pMember->m_nFixedRows; row++)
    {
        if (GetRowHeight(row) <= 0) continue;

        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(row)-1;

        // rect.bottom = bottom pixel of previous row
        if (rect.top > clipRect.bottom)
            break;                // Gone past cliprect
        if (rect.bottom < clipRect.top)
            continue;             // Reached cliprect yet?

        rect.right = nFixedColWidth-1;
        for (col = minVisibleCol; col <= maxVisibleCol; col++)
        {
            if (GetColumnWidth(col) <= 0) continue;

            rect.left = rect.right+1;
            rect.right = rect.left + GetColumnWidth(col)-1;

            if (rect.left > clipRect.right)
                break;        // gone past cliprect
            if (rect.right < clipRect.left)
                continue;     // Reached cliprect yet?

            pCell = GetCell(row, col);
            if (pCell)
			{
				pCell->SetCoords(row,col);
                pCell->Draw(pDC, row, col, rect, FALSE);
			}
			
			if(pMember->m_bDrawFixedLine)
			{
				pDC->MoveTo(rect.right, rect.top);
				pDC->LineTo(rect.right, rect.bottom);
				
				pDC->MoveTo(rect.left-1, rect.bottom);
				pDC->LineTo(rect.right, rect.bottom);
			}
       }
    }

    // draw rest of non-fixed cells
    rect.bottom = nFixedRowHeight-1;
    for (row = minVisibleRow; row <= maxVisibleRow; row++)
    {
        if (GetRowHeight(row) <= 0) continue;

        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(row)-1;

        // rect.bottom = bottom pixel of previous row
        if (rect.top > clipRect.bottom)
            break;                // Gone past cliprect
        if (rect.bottom < clipRect.top)
            continue;             // Reached cliprect yet?

        rect.right = nFixedColWidth-1;
        for (col = minVisibleCol; col <= maxVisibleCol; col++)
        {
            if (GetColumnWidth(col) <= 0) continue;

            rect.left = rect.right+1;
            rect.right = rect.left + GetColumnWidth(col)-1;

            if (rect.left > clipRect.right)
                break;        // gone past cliprect
            if (rect.right < clipRect.left)
                continue;     // Reached cliprect yet?

            pCell = GetCell(row, col);
            // TRACE(_T("Cell %d,%d type: %s\n"), row, col, pCell->GetRuntimeClass()->m_lpszClassName);
            if (pCell)
			{
				pCell->SetCoords(row,col);
                pCell->Draw(pDC, row, col, rect, FALSE);
			}
        }
    }


//    CPen pen;
//    pen.CreatePen(PS_SOLID, 0, pMember->m_crGridLineColour);
//    pDC->SelectObject(&pen);

    // draw vertical lines (drawn at ends of cells)
    if (pMember->m_nGridLines == GVL_BOTH || pMember->m_nGridLines == GVL_VERT)
    {
        int x = nFixedColWidth;
        for (col = minVisibleCol; col <= maxVisibleCol; col++)
        {
            if (GetColumnWidth(col) <= 0) continue;

            x += GetColumnWidth(col);
            pDC->MoveTo(x-1, nFixedRowHeight);
            pDC->LineTo(x-1, VisRect.bottom);
        }
    }

    // draw horizontal lines (drawn at bottom of each cell)
    if (pMember->m_nGridLines == GVL_BOTH || pMember->m_nGridLines == GVL_HORZ)
    {
        int y = nFixedRowHeight;
        for (row = minVisibleRow; row <= maxVisibleRow; row++)
        {
            if (GetRowHeight(row) <= 0) continue;
			
            y += GetRowHeight(row);
            pDC->MoveTo(nFixedColWidth, y-1);
            pDC->LineTo(VisRect.right,  y-1);
        }
    }

    pDC->SelectStockObject(NULL_PEN);

    // Let parent know it can discard it's data if it needs to.
    if (GetVirtualMode())
       SendCacheHintToParent(CCellRange(-1,-1,-1,-1));

#ifdef _DEBUG
	LARGE_INTEGER iEndCount;
	QueryPerformanceCounter(&iEndCount);
	TRACE1("Draw counter ticks: %d\n", iEndCount.LowPart-iStartCount.LowPart);
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
// CExGridCtrl Cell selection stuff

// Is a given cell designation valid (ie within the bounds of our number
// of columns/rows)?
BOOL CExGridCtrlApp::IsValid(int nRow, int nCol)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    return (nRow >= 0 && nRow < pMember->m_nRows && nCol >= 0 && nCol < pMember->m_nCols);
}

BOOL CExGridCtrlApp::IsValid(const CCellID& cell)
{
    return IsValid(cell.row, cell.col);
}

// Is a given cell range valid (ie within the bounds of our number
// of columns/rows)?
BOOL CExGridCtrlApp::IsValid(const CCellRange& range)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    return (range.GetMinRow() >= 0 && range.GetMinCol() >= 0 &&
        range.GetMaxRow() >= 0 && range.GetMaxCol() >= 0 &&
        range.GetMaxRow() < pMember->m_nRows && range.GetMaxCol() < pMember->m_nCols &&
        range.GetMinRow() <= range.GetMaxRow() && range.GetMinCol() <= range.GetMaxCol());
}

// Enables/Disables redraw for certain operations like columns auto-sizing etc,
// but not for user caused things such as selection changes.
void CExGridCtrlApp::SetRedraw(BOOL bAllowDraw, BOOL bResetScrollBars /* = FALSE */)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

//    TRACE(_T("%s: Setting redraw to %s\n"),
//             GetRuntimeClass()->m_lpszClassName, bAllowDraw? _T("TRUE") : _T("FALSE"));

    if (bAllowDraw && !pMember->m_bAllowDraw)
    {
        pMember->m_bAllowDraw = TRUE;
        Refresh();
    }

    pMember->m_bAllowDraw = bAllowDraw;
    if (bResetScrollBars)
        ResetScrollBars();
}

// Forces a redraw of a cell immediately (using a direct DC construction,
// or the supplied dc)
BOOL CExGridCtrlApp::RedrawCell(const CCellID& cell, CDC* pDC /* = NULL */)
{
    return RedrawCell(cell.row, cell.col, pDC);
}

BOOL CExGridCtrlApp::RedrawCell(int nRow, int nCol, CDC* pDC /* = NULL */)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    BOOL bResult = TRUE;
    BOOL bMustReleaseDC = FALSE;

    if (!pMember->m_bAllowDraw || !IsCellVisible(nRow, nCol))
        return FALSE;

    CRect rect;
    if (!GetCellRect(nRow, nCol, rect))
        return FALSE;

    if (!pDC)
    {
        pDC = pMember->m_pGrid->GetDC();
        if (pDC)
            bMustReleaseDC = TRUE;
    }

    if (pDC)
    {
        CPen pen;
        pen.CreatePen(PS_SOLID, 0, pMember->m_crGridLineColour);

        CPen* pOldPen = (CPen*) pDC->SelectObject(&pen);

        // Redraw cells directly
        if (nRow < pMember->m_nFixedRows || nCol < pMember->m_nFixedCols)
        {
            CGridCellBase* pCell = GetCell(nRow, nCol);
            if (pCell)
			{
                bResult = pCell->Draw(pDC, nRow, nCol, rect, TRUE);
				
				if(pMember->m_bDrawFixedLine)
				{
					pDC->MoveTo(rect.right, rect.top);
					pDC->LineTo(rect.right, rect.bottom);
					
					pDC->MoveTo(rect.left-1, rect.bottom);
					pDC->LineTo(rect.right, rect.bottom);
				}
			}
        }
        else
        {
            CGridCellBase* pCell = GetCell(nRow, nCol);
            if (pCell)
                bResult = pCell->Draw(pDC, nRow, nCol, rect, TRUE);

            // Since we have erased the background, we will need to redraw the gridlines
            if (pMember->m_nGridLines == GVL_BOTH || pMember->m_nGridLines == GVL_HORZ)
            {
                pDC->MoveTo(rect.left,    rect.bottom);
                pDC->LineTo(rect.right + 1, rect.bottom);
            }
            if (pMember->m_nGridLines == GVL_BOTH || pMember->m_nGridLines == GVL_VERT)
            {
                pDC->MoveTo(rect.right, rect.top);
                pDC->LineTo(rect.right, rect.bottom + 1);
            }
            pDC->SelectObject(pOldPen);
        }
    } else
        pMember->m_pGrid->InvalidateRect(rect, TRUE);     // Could not get a DC - pMember->m_pGrid->Invalidate it anyway
    // and hope that OnPaint manages to get one

    if (bMustReleaseDC)
        pMember->m_pGrid->ReleaseDC(pDC);

    return bResult;
}

// redraw a complete row
BOOL CExGridCtrlApp::RedrawRow(int row)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    BOOL bResult = TRUE;

    CDC* pDC = pMember->m_pGrid->GetDC();
    for (int col = 0; col < GetColumnCount(); col++)
        bResult = RedrawCell(row, col, pDC) && bResult;
    if (pDC)
        pMember->m_pGrid->ReleaseDC(pDC);

    return bResult;
}

// redraw a complete column
BOOL CExGridCtrlApp::RedrawColumn(int col)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    BOOL bResult = TRUE;

    CDC* pDC = pMember->m_pGrid->GetDC();
    for (int row = 0; row < GetRowCount(); row++)
        bResult = RedrawCell(row, col, pDC) && bResult;
    if (pDC)
        pMember->m_pGrid->ReleaseDC(pDC);

    return bResult;
}


// Sets the currently selected cell, returning the previous current cell
CCellID CExGridCtrlApp::SetFocusCell(int nRow, int nCol)
{
    return SetFocusCell(CCellID(nRow, nCol));
}

CCellID CExGridCtrlApp::SetFocusCell(CCellID cell)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (cell == pMember->m_idCurrentCell)
        return pMember->m_idCurrentCell;

    CCellID idPrev = pMember->m_idCurrentCell;

    // EFW - Bug Fix - Force focus to be in a non-fixed cell
    if (cell.row != -1 && cell.row < GetFixedRowCount())
        cell.row = GetFixedRowCount();
    if (cell.col != -1 && cell.col < GetFixedColumnCount())
        cell.col = GetFixedColumnCount();

    pMember->m_idCurrentCell = cell;

    if (IsValid(idPrev))
    {
        SetItemState(idPrev.row, idPrev.col,
            GetItemState(idPrev.row, idPrev.col) & ~GVIS_FOCUSED);
        RedrawCell(idPrev); // comment to reduce flicker

        if (GetTrackFocusCell() && idPrev.col != pMember->m_idCurrentCell.col)
            for (int row = 0; row < pMember->m_nFixedRows; row++)
                RedrawCell(row, idPrev.col);
        if (GetTrackFocusCell() && idPrev.row != pMember->m_idCurrentCell.row)
            for (int col = 0; col < pMember->m_nFixedCols; col++)
                RedrawCell(idPrev.row, col);
    }

    if (IsValid(pMember->m_idCurrentCell))
    {
        SetItemState(pMember->m_idCurrentCell.row, pMember->m_idCurrentCell.col,
            GetItemState(pMember->m_idCurrentCell.row, pMember->m_idCurrentCell.col) | GVIS_FOCUSED);

        RedrawCell(pMember->m_idCurrentCell); // comment to reduce flicker

        if (GetTrackFocusCell() && idPrev.col != pMember->m_idCurrentCell.col)
            for (int row = 0; row < pMember->m_nFixedRows; row++)
                RedrawCell(row, pMember->m_idCurrentCell.col);
        if (GetTrackFocusCell() && idPrev.row != pMember->m_idCurrentCell.row)
            for (int col = 0; col < pMember->m_nFixedCols; col++)
                RedrawCell(pMember->m_idCurrentCell.row, col);

        // EFW - New addition.  If in list mode, make sure the selected
        // row highlight follows the cursor.
        // Removed by C Maunder 27 May
        //if (pMember->m_bListMode)
        //{
        //    m_PrevSelectedCellMap.RemoveAll();
        //    pMember->m_MouseMode = MOUSE_SELECT_ROW;
        //    OnSelecting(pMember->m_idCurrentCell);

            // Leave pMember->m_pGrid off so that you can still drag the highlight around
            // without selecting rows.
            // pMember->m_MouseMode = MOUSE_NOTHING;
        //}

	}

    return idPrev;
}

// Sets the range of currently selected cells
void CExGridCtrlApp::SetSelectedRange(const CCellRange& Range,
                                 BOOL bForceRepaint /* = FALSE */, BOOL bSelectCells/*=TRUE*/)
{
    SetSelectedRange(Range.GetMinRow(), Range.GetMinCol(),
                     Range.GetMaxRow(), Range.GetMaxCol(),
                     bForceRepaint, bSelectCells);
}

void CExGridCtrlApp::SetSelectedRange(int nMinRow, int nMinCol, int nMaxRow, int nMaxCol,
                                 BOOL bForceRepaint /* = FALSE */, BOOL bSelectCells/*=TRUE*/)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (!pMember->m_bEnableSelection)
        return;

	CWaitCursor wait; // Thomas Haase 

    CDC* pDC = NULL;
    if (bForceRepaint)
        pDC = pMember->m_pGrid->GetDC();

	// Only redraw visible cells
    CCellRange VisCellRange;
	if (IsWindow(pMember->m_pGrid->GetSafeHwnd()))
		VisCellRange = GetVisibleNonFixedCellRange();
   
	if(GetFixedRowSelectionHighlight() == FALSE)
	{
		// EFW - Bug fix - Don't allow selection of fixed rows
		if(nMinRow >= 0 && nMinRow < GetFixedRowCount())
			nMinRow = GetFixedRowCount();
		if(nMaxRow >= 0 && nMaxRow < GetFixedRowCount())
			nMaxRow = GetFixedRowCount();
		if(nMinCol >= 0 && nMinCol < GetFixedColumnCount())
			nMinCol = GetFixedColumnCount();
		if(nMaxCol >= 0 && nMaxCol < GetFixedColumnCount())
			nMaxCol = GetFixedColumnCount();
	}

    // If we are selecting cells, then first clear out the list of currently selected cells, then
    if (bSelectCells)
    {
        POSITION pos;

        // Unselect all previously selected cells
        for (pos = pMember->m_SelectedCellMap.GetStartPosition(); pos != NULL; )
        {
            DWORD key;
            CCellID cell;
            pMember->m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);

            // Reset the selection flag on the cell
            if (IsValid(cell))
            {
                // pMember->m_pGrid will remove the cell from the pMember->m_SelectedCellMap map
                SetItemState(cell.row, cell.col,
                    GetItemState(cell.row, cell.col) & ~GVIS_SELECTED);

                // If pMember->m_pGrid is to be reselected, continue on past the redraw
                if (nMinRow <= cell.row && cell.row <= nMaxRow &&
                    nMinCol <= cell.col && cell.col <= nMaxCol)
                    continue;

				BOOL bValid = TRUE;
				if(GetFixedRowSelectionHighlight() == FALSE) 
				{
					bValid = (VisCellRange.IsValid() && VisCellRange.InRange( cell ))? TRUE : FALSE ; 
				}
				// if ( VisCellRange.IsValid() && VisCellRange.InRange( cell ) )
				if( bValid )
				{
					if (bForceRepaint && pDC)                    // Redraw NOW
						RedrawCell(cell.row, cell.col, pDC);
					else
						InvalidateCellRect(cell);                // Redraw at leisure
				}
            }
            else
            {
                pMember->m_SelectedCellMap.RemoveKey( key);  // if it's not valid, get rid of it!
            }
        }

        // if we are selecting cells, and there are previous selected cells to be retained 
        // (eg Ctrl is being held down) then copy them to the newly created list, and mark 
        // all these cells as selected
        // Note that if we are list mode, single row selection, then we won't be adding 
        // the previous cells. Only the current row of cells will be added (see below)
        if (!GetSingleRowSelection() &&
            nMinRow >= 0 && nMinCol >= 0 && nMaxRow >= 0 && nMaxCol >= 0)
        {
            for (pos = pMember->m_PrevSelectedCellMap.GetStartPosition(); pos != NULL; /* nothing */)
            {
                DWORD key;
                CCellID cell;
                pMember->m_PrevSelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);

                if (!IsValid(cell))
                    continue;

                int nState = GetItemState(cell.row, cell.col);

                // Set state as Selected. pMember->m_pGrid will add the cell to pMember->m_SelectedCellMap
                SetItemState(cell.row, cell.col, nState | GVIS_SELECTED);

				BOOL bValid = TRUE;
				if(GetFixedRowSelectionHighlight() == FALSE) 
				{
					bValid = (VisCellRange.IsValid() && VisCellRange.InRange( cell ))? TRUE : FALSE ; 
				}
				// if ( VisCellRange.IsValid() && VisCellRange.InRange( cell ) )
				if( bValid )
				{
					// Redraw (immediately or at leisure)
					if (bForceRepaint && pDC)
					    RedrawCell(cell.row, cell.col, pDC);
					else
					    InvalidateCellRect(cell);
				}
            }
        }
    }

    // Now select/deselect all cells in the cell range specified. If selecting, and the cell 
    // has already been marked as selected (above) then ignore it. If we are deselecting and
    // the cell isn't selected, then ignore
    if (nMinRow >= 0 && nMinCol >= 0 && nMaxRow >= 0 && nMaxCol >= 0 &&
        nMaxRow < pMember->m_nRows && nMaxCol < pMember->m_nCols &&
        nMinRow <= nMaxRow && nMinCol <= nMaxCol)
    {
        for (int row = nMinRow; row <= nMaxRow; row++)
            for (int col = nMinCol; col <= nMaxCol; col++)
            {
                BOOL bCellSelected = IsCellSelected(row, col);
                if (bSelectCells == bCellSelected)
                    continue;    // Already selected or deselected - ignore

                // Set the selected state. pMember->m_pGrid will add/remove the cell to pMember->m_SelectedCellMap
                if (bSelectCells)
                    SetItemState(row, col, GetItemState(row, col) | GVIS_SELECTED);
                else
                    SetItemState(row, col, GetItemState(row, col) & ~GVIS_SELECTED);

				BOOL bValid = TRUE;
				if(GetFixedRowSelectionHighlight() == FALSE) 
				{
					bValid = (VisCellRange.IsValid() && VisCellRange.InRange(row, col))? TRUE : FALSE ; 
				}
				// if ( VisCellRange.IsValid() && VisCellRange.InRange(row, col) )
				if( bValid )
				{
	                // Redraw (immediately or at leisure)
	                if (bForceRepaint && pDC)
	                    RedrawCell(row, col, pDC);
	                else
	                    InvalidateCellRect(row, col);
				}
            }
    }
    //    TRACE(_T("%d cells selected.\n"), pMember->m_SelectedCellMap.GetCount());

    if (pDC != NULL)
        pMember->m_pGrid->ReleaseDC(pDC);
}

// selects all cells
void CExGridCtrlApp::SelectAllCells()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (!pMember->m_bEnableSelection)
        return;

    SetSelectedRange(pMember->m_nFixedRows, pMember->m_nFixedCols, GetRowCount()-1, GetColumnCount()-1);
}

// selects columns
void CExGridCtrlApp::SelectColumns(CCellID currentCell, 
                              BOOL bForceRedraw /*=FALSE*/, BOOL bSelectCells /*=TRUE*/)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (!pMember->m_bEnableSelection)
        return;

    //if (currentCell.col == pMember->m_idCurrentCell.col) return;
    if (currentCell.col < pMember->m_nFixedCols)
        return;
    if (!IsValid(currentCell))
        return;

    if (GetSingleColSelection())
        SetSelectedRange(GetFixedRowCount(), currentCell.col,
                         GetRowCount()-1,    currentCell.col,
                         bForceRedraw, bSelectCells);
    else
        SetSelectedRange(GetFixedRowCount(),
                         min(pMember->m_SelectionStartCell.col, currentCell.col),
                         GetRowCount()-1,
                         max(pMember->m_SelectionStartCell.col, currentCell.col),
                         bForceRedraw, bSelectCells);
}

// selects rows
void CExGridCtrlApp::SelectRows(CCellID currentCell, 
                           BOOL bForceRedraw /*=FALSE*/, BOOL bSelectCells /*=TRUE*/)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (!pMember->m_bEnableSelection)
        return;

    //if (currentCell.row; == pMember->m_idCurrentCell.row) return;
    if (currentCell.row < pMember->m_nFixedRows)
        return;
    if (!IsValid(currentCell))
        return;

	int nMinCol = (GetFixedRowSelectionHighlight() == FALSE) ? GetFixedColumnCount() : 0;
    if (GetSingleRowSelection())
        SetSelectedRange(currentCell.row, nMinCol,
                         currentCell.row, GetColumnCount()-1, 
                         bForceRedraw, bSelectCells);
    else
        SetSelectedRange(min(pMember->m_SelectionStartCell.row, currentCell.row), nMinCol,
                         max(pMember->m_SelectionStartCell.row, currentCell.row), GetColumnCount()-1,
                         bForceRedraw, bSelectCells);
}

// selects cells
void CExGridCtrlApp::SelectCells(CCellID currentCell, 
                            BOOL bForceRedraw /*=FALSE*/, BOOL bSelectCells /*=TRUE*/)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (!pMember->m_bEnableSelection)
        return;

    int row = currentCell.row;
    int col = currentCell.col;
    if (row < pMember->m_nFixedRows || col < pMember->m_nFixedCols)
        return;
    if (!IsValid(currentCell))
        return;

    // Prevent unnecessary redraws
    //if (currentCell == pMember->m_LeftClickDownCell)  return;
    //else if (currentCell == pMember->m_idCurrentCell) return;

    SetSelectedRange(min(pMember->m_SelectionStartCell.row, row),
                     min(pMember->m_SelectionStartCell.col, col),
                     max(pMember->m_SelectionStartCell.row, row),
                     max(pMember->m_SelectionStartCell.col, col),
                     bForceRedraw, bSelectCells);
}

// Called when mouse/keyboard selection is a-happening.
void CExGridCtrlApp::OnSelecting(const CCellID& currentCell)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (!pMember->m_bEnableSelection)
        return;

    switch (pMember->m_MouseMode)
    {
    case MOUSE_SELECT_ALL:
        SelectAllCells();
        break;
    case MOUSE_SELECT_COL:
        SelectColumns(currentCell, FALSE);
        break;
    case MOUSE_SELECT_ROW:
        SelectRows(currentCell, FALSE);
        break;
    case MOUSE_SELECT_CELLS:
        SelectCells(currentCell, FALSE);
        break;
    }

    // EFW - Bug fix [REMOVED CJM: pMember->m_pGrid will cause infinite loop in list mode]
    // SetFocusCell(max(currentCell.row, pMember->m_nFixedRows), max(currentCell.col, pMember->m_nFixedCols));
}

void CExGridCtrlApp::ValidateAndModifyCellContents(int nRow, int nCol, LPCTSTR strText)
{
    if (!IsCellEditable(nRow, nCol))
        return;

    if (SendMessageToParent(nRow, nCol, GVN_BEGINLABELEDIT) >= 0)
    {
        CString strCurrentText = GetItemText(nRow, nCol);
        if (strCurrentText != strText)
        {
            SetItemText(nRow, nCol, strText);
            if (ValidateEdit(nRow, nCol, strText) && 
                SendMessageToParent(nRow, nCol, GVN_ENDLABELEDIT) >= 0)
            {
                SetModified(TRUE, nRow, nCol);
                RedrawCell(nRow, nCol);
            }
            else
            {
                SetItemText(nRow, nCol, strCurrentText);
            }
        }
    }
}

void CExGridCtrlApp::ClearCells(CCellRange Selection)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    for (int row = Selection.GetMinRow(); row <= Selection.GetMaxRow(); row++)
    {
        for (int col = Selection.GetMinCol(); col <= Selection.GetMaxCol(); col++)
        {
            // don't clear hidden cells
            if ( pMember->m_arRowHeights[row] > 0 && pMember->m_arColWidths[col] > 0 )
            {
                ValidateAndModifyCellContents(row, col, _T(""));
            }
		}
	}
    Refresh();
}

#ifndef GRIDCONTROL_NO_CLIPBOARD

////////////////////////////////////////////////////////////////////////////////////////
// Clipboard functions

// Deletes the contents from the selected cells
void CExGridCtrlApp::CutSelectedText()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (!IsEditable())
        return;

    for (POSITION pos = pMember->m_SelectedCellMap.GetStartPosition(); pos != NULL; )
    {
		DWORD key;
        CCellID cell;
        pMember->m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);
        ValidateAndModifyCellContents(cell.row, cell.col, _T(""));
    }
}

// Copies text from the selected cells to the clipboard
COleDataSource* CExGridCtrlApp::CopyTextFromGrid()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    USES_CONVERSION;

    CCellRange Selection = GetSelectedCellRange();
    if (!IsValid(Selection))
        return NULL;

    if (GetVirtualMode())
        SendCacheHintToParent(Selection);

    // Write to shared file (REMEBER: CF_TEXT is ANSI, not UNICODE, so we need to convert)
    CSharedFile sf(GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT);

    // Get a tab delimited string to copy to cache
    CString str;
    CGridCellBase *pCell;
    for (int row = Selection.GetMinRow(); row <= Selection.GetMaxRow(); row++)
    {
        // don't copy hidden cells
        if( pMember->m_arRowHeights[row] <= 0 )
            continue;

        str.Empty();
        for (int col = Selection.GetMinCol(); col <= Selection.GetMaxCol(); col++)
        {
            // don't copy hidden cells
            if( pMember->m_arColWidths[col] <= 0 )
                continue;

            pCell = GetCell(row, col);
            if (pCell &&(pCell->GetState() & GVIS_SELECTED))
            {
                // if (!pCell->GetText())
                //    str += _T(" ");
                // else 
                str += pCell->GetText();
            }
            if (col != Selection.GetMaxCol()) 
                str += _T("\t");
        }

        if (row != Selection.GetMaxRow()) 
            str += _T("\r\n");
        
        sf.Write(T2A(str.GetBuffer(1)), str.GetLength());
        str.ReleaseBuffer();
    }
    
    char c = '\0';
    sf.Write(&c, 1);

    if (GetVirtualMode())
        SendCacheHintToParent(CCellRange(-1,-1,-1,-1));

    DWORD dwLen = (DWORD) sf.GetLength();
    HGLOBAL hMem = sf.Detach();
    if (!hMem)
        return NULL;

    hMem = ::GlobalReAlloc(hMem, dwLen, GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT);
    if (!hMem)
        return NULL;

    // Cache data
    COleDataSource* pSource = new COleDataSource();
    pSource->CacheGlobalData(CF_TEXT, hMem);

    return pSource;
}

// Pastes text from the clipboard to the selected cells
BOOL CExGridCtrlApp::PasteTextToGrid(CCellID cell, COleDataObject* pDataObject, 
								BOOL bSelectPastedCells /*=TRUE*/)
{
    if (!IsValid(cell) || !IsCellEditable(cell) || !pDataObject->IsDataAvailable(CF_TEXT))
        return FALSE;

    // Get the text from the COleDataObject
    HGLOBAL hmem = pDataObject->GetGlobalData(CF_TEXT);
    CMemFile sf((BYTE*) ::GlobalLock(hmem), ::GlobalSize(hmem));

    // CF_TEXT is ANSI text, so we need to allocate a char* buffer
    // to hold pMember->m_pGrid.
    LPTSTR szBuffer = new TCHAR[::GlobalSize(hmem)];
    if (!szBuffer)
        return FALSE;

    sf.Read(szBuffer, ::GlobalSize(hmem));
    ::GlobalUnlock(hmem);

    // Now store in generic TCHAR form so we no longer have to deal with
    // ANSI/UNICODE problems
    CString strText = szBuffer;
    delete szBuffer;

    // Parse text data and set in cells...
    strText.LockBuffer();
    CString strLine = strText;
    int nLine = 0;

    // Find the end of the first line
	CCellRange PasteRange(cell.row, cell.col,-1,-1);
    int nIndex;
    do
    {
        int nColumn = 0;
        nIndex = strLine.Find(_T("\n"));

        // Store the remaining chars after the newline
        CString strNext = (nIndex < 0)? _T("")  : strLine.Mid(nIndex + 1);

        // Remove all chars after the newline
        if (nIndex >= 0)
            strLine = strLine.Left(nIndex);

        int nLineIndex = strLine.FindOneOf(_T("\t,"));
        CString strCellText = (nLineIndex >= 0)? strLine.Left(nLineIndex) : strLine;

        // skip hidden rows
        int iRowVis = cell.row + nLine;
        while( iRowVis < GetRowCount())
        {
            if( GetRowHeight( iRowVis) > 0)
                break;
            nLine++;
            iRowVis++;
        }

        while (!strLine.IsEmpty())
        {
            // skip hidden columns
            int iColVis = cell.col + nColumn;
            while( iColVis < GetColumnCount())
            {
                if( GetColumnWidth( iColVis) > 0)
                    break;
                nColumn++;
                iColVis++;
            }

            CCellID TargetCell(iRowVis, iColVis);
            if (IsValid(TargetCell))
            {
                strCellText.TrimLeft();
                strCellText.TrimRight();

                ValidateAndModifyCellContents(TargetCell.row, TargetCell.col, strCellText);

                // Make sure cell is not selected to avoid data loss
                SetItemState(TargetCell.row, TargetCell.col,
                    GetItemState(TargetCell.row, TargetCell.col) & ~GVIS_SELECTED);

				if (iRowVis > PasteRange.GetMaxRow()) PasteRange.SetMaxRow(iRowVis);
				if (iColVis > PasteRange.GetMaxCol()) PasteRange.SetMaxCol(iColVis);
            }

            strLine = (nLineIndex >= 0)? strLine.Mid(nLineIndex + 1) : _T("");
            nLineIndex = strLine.FindOneOf(_T("\t,"));
            strCellText = (nLineIndex >= 0)? strLine.Left(nLineIndex) : strLine;

            nColumn++;
        }

        strLine = strNext;
        nLine++;
    } while (nIndex >= 0);

    strText.UnlockBuffer();

	if (bSelectPastedCells)
		SetSelectedRange(PasteRange, TRUE);
	else
	{
		ResetSelectedRange();
		Refresh();
	}

    return TRUE;
}
#endif

#ifndef GRIDCONTROL_NO_DRAGDROP

// Start drag n drop
void CExGridCtrlApp::OnBeginDrag()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (!pMember->m_bAllowDragAndDrop)
        return;

    COleDataSource* pSource = CopyTextFromGrid();
    if (pSource)
    {
        SendMessageToParent(GetSelectedCellRange().GetTopLeft().row,
            GetSelectedCellRange().GetTopLeft().col,
            GVN_BEGINDRAG);

        pMember->m_MouseMode = MOUSE_DRAGGING;
        pMember->m_bLMouseButtonDown = FALSE;

        DROPEFFECT dropEffect = pSource->DoDragDrop(DROPEFFECT_COPY | DROPEFFECT_MOVE);

        if (dropEffect & DROPEFFECT_MOVE)
            CutSelectedText();

        if (pSource)
            delete pSource;    // Did not pass source to clipboard, so must delete
    }
}

// Handle drag over grid
DROPEFFECT CExGridCtrlApp::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState,
                                 CPoint point)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    // Any text data available for us?
    if (!pMember->m_bAllowDragAndDrop || !IsEditable() || !pDataObject->IsDataAvailable(CF_TEXT))
        return DROPEFFECT_NONE;

    // Find which cell we are over and drop-highlight it
    CCellID cell = GetCellFromPt(point, FALSE);

    // If not valid, set the previously drop-highlighted cell as no longer drop-highlighted
    if (!IsValid(cell))
    {
        OnDragLeave();
        pMember->m_LastDragOverCell = CCellID(-1,-1);
        return DROPEFFECT_NONE;
    }

    if (!IsCellEditable(cell))
        return DROPEFFECT_NONE;

    // Have we moved over a different cell than last time?
    if (cell != pMember->m_LastDragOverCell)
    {
        // Set the previously drop-highlighted cell as no longer drop-highlighted
        if (IsValid(pMember->m_LastDragOverCell))
        {
            UINT nState = GetItemState(pMember->m_LastDragOverCell.row, pMember->m_LastDragOverCell.col);
            SetItemState(pMember->m_LastDragOverCell.row, pMember->m_LastDragOverCell.col,
                nState & ~GVIS_DROPHILITED);
            RedrawCell(pMember->m_LastDragOverCell);
        }

        pMember->m_LastDragOverCell = cell;

        // Set the new cell as drop-highlighted
        if (IsValid(pMember->m_LastDragOverCell))
        {
            UINT nState = GetItemState(pMember->m_LastDragOverCell.row, pMember->m_LastDragOverCell.col);
            SetItemState(pMember->m_LastDragOverCell.row, pMember->m_LastDragOverCell.col,
                nState | GVIS_DROPHILITED);
            RedrawCell(pMember->m_LastDragOverCell);
        }
    }

    // Return an appropraite value of DROPEFFECT so mouse cursor is set properly
    if (dwKeyState & MK_CONTROL)
        return DROPEFFECT_COPY;
    else
        return DROPEFFECT_MOVE;
}

// Something has just been dragged onto the grid
DROPEFFECT CExGridCtrlApp::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState,
                                  CPoint point)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    // Any text data available for us?
    if (!pMember->m_bAllowDragAndDrop || !pDataObject->IsDataAvailable(CF_TEXT))
        return DROPEFFECT_NONE;

    // Find which cell we are over and drop-highlight it
    pMember->m_LastDragOverCell = GetCellFromPt(point, FALSE);
    if (!IsValid(pMember->m_LastDragOverCell))
        return DROPEFFECT_NONE;

    if (!IsCellEditable(pMember->m_LastDragOverCell))
        return DROPEFFECT_NONE;

    if (IsValid(pMember->m_LastDragOverCell))
    {
        UINT nState = GetItemState(pMember->m_LastDragOverCell.row, pMember->m_LastDragOverCell.col);
        SetItemState(pMember->m_LastDragOverCell.row, pMember->m_LastDragOverCell.col,
            nState | GVIS_DROPHILITED);
        RedrawCell(pMember->m_LastDragOverCell);
    }

    // Return an appropraite value of DROPEFFECT so mouse cursor is set properly
    if (dwKeyState & MK_CONTROL)
        return DROPEFFECT_COPY;
    else
        return DROPEFFECT_MOVE;
}

// Something has just been dragged away from the grid
void CExGridCtrlApp::OnDragLeave()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    // Set the previously drop-highlighted cell as no longer drop-highlighted
    if (IsValid(pMember->m_LastDragOverCell))
    {
        UINT nState = GetItemState(pMember->m_LastDragOverCell.row, pMember->m_LastDragOverCell.col);
        SetItemState(pMember->m_LastDragOverCell.row, pMember->m_LastDragOverCell.col,
            nState & ~GVIS_DROPHILITED);
        RedrawCell(pMember->m_LastDragOverCell);
    }
}

// Something has just been dropped onto the grid
BOOL CExGridCtrlApp::OnDrop(COleDataObject* pDataObject, DROPEFFECT /*dropEffect*/,
                       CPoint /* point */)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    pMember->m_MouseMode = MOUSE_NOTHING;
    
    if (!pMember->m_bAllowDragAndDrop || !IsCellEditable(pMember->m_LastDragOverCell))
        return FALSE;

    OnDragLeave();

    return PasteTextToGrid(pMember->m_LastDragOverCell, pDataObject, FALSE);
}
#endif

#ifndef GRIDCONTROL_NO_CLIPBOARD
void CExGridCtrlApp::OnEditCut()
{
    if (!IsEditable())
        return;

    COleDataSource* pSource = CopyTextFromGrid();
    if (!pSource)
        return;

    pSource->SetClipboard();
    CutSelectedText();
}

void CExGridCtrlApp::OnEditCopy()
{
    COleDataSource* pSource = CopyTextFromGrid();
    if (!pSource)
        return;

    pSource->SetClipboard();
}

void CExGridCtrlApp::OnEditPaste()
{
    if (!IsEditable())
        return;

	CCellRange cellRange = GetSelectedCellRange();

	// Get the top-left selected cell, or the Focus cell, or the topleft (non-fixed) cell
	CCellID cell;
	if (cellRange.IsValid())
	{
		cell.row = cellRange.GetMinRow();
		cell.col = cellRange.GetMinCol();
	}
	else
	{
		cell = GetFocusCell();
		if (!IsValid(cell))
			cell = GetTopleftNonFixedCell();
		if (!IsValid(cell))
			return;
	}

	// If a cell is being edited, then call it's edit window paste function.
    if ( IsItemEditing(cell.row, cell.col) )
    {
        CGridCellBase* pCell = GetCell(cell.row, cell.col);
        ASSERT(pCell);
        if (!pCell) return;

		CWnd* pEditWnd = pCell->GetEditWnd();
		if ( pEditWnd/* && pEditWnd->IsKindOf(RUNTIME_CLASS(CEdit))*/ )
		{
			((CEdit*)pEditWnd)->Paste();
			return;
		}
    }

    // Attach a COleDataObject to the clipboard and paste the data to the grid
    COleDataObject obj;
    if (obj.AttachClipboard())
        PasteTextToGrid(cell, &obj);
}
#endif

void CExGridCtrlApp::OnEditSelectAll()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    SendMessageToParent(pMember->m_LeftClickDownCell.row, pMember->m_LeftClickDownCell.col, GVN_SELCHANGING);
    SelectAllCells();
    SendMessageToParent(pMember->m_idCurrentCell.row, pMember->m_idCurrentCell.col, GVN_SELCHANGED);
}

#ifndef GRIDCONTROL_NO_CLIPBOARD
void CExGridCtrlApp::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetSelectedCount() > 0); // - Thomas Haase 
    //CCellRange Selection = GetSelectedCellRange();
    //pCmdUI->Enable(Selection.Count() && IsValid(Selection));
}

void CExGridCtrlApp::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsEditable() && GetSelectedCount() > 0); // - Thomas Haase 
    //CCellRange Selection = GetSelectedCellRange();
    //pCmdUI->Enable(IsEditable() && Selection.Count() && IsValid(Selection));
}

void CExGridCtrlApp::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
    CCellID cell = GetFocusCell();

    BOOL bCanPaste = IsValid(cell) && IsCellEditable(cell) &&
        ::IsClipboardFormatAvailable(CF_TEXT);

    pCmdUI->Enable(bCanPaste);
}
#endif

void CExGridCtrlApp::OnUpdateEditSelectAll(CCmdUI* pCmdUI)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    pCmdUI->Enable(pMember->m_bEnableSelection);
}

////////////////////////////////////////////////////////////////////////////////////////
// hittest-like functions

// TRUE if the mouse is over a row resize area
BOOL CExGridCtrlApp::MouseOverRowResizeArea(CPoint& point)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (point.x >= GetFixedColumnWidth())
        return FALSE;

    CCellID idCurrentCell = GetCellFromPt(point);
    CPoint start;
    if (!GetCellOrigin(idCurrentCell, &start))
        return FALSE;

    int endy = start.y + GetRowHeight(idCurrentCell.row);

    if ((point.y - start.y < pMember->m_nResizeCaptureRange && idCurrentCell.row != 0) ||
        endy - point.y < pMember->m_nResizeCaptureRange)
    {
        return TRUE;
    }
    else
        return FALSE;
}

// TRUE if the mouse is over a column resize area. point is in Client coords
BOOL CExGridCtrlApp::MouseOverColumnResizeArea(CPoint& point)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (point.y >= GetFixedRowHeight())
        return FALSE;

    CCellID idCurrentCell = GetCellFromPt(point);
    CPoint start;
    if (!GetCellOrigin(idCurrentCell, &start))
        return FALSE;

    int endx = start.x + GetColumnWidth(idCurrentCell.col);

    if ((point.x - start.x < pMember->m_nResizeCaptureRange && idCurrentCell.col != 0) ||
        endx - point.x < pMember->m_nResizeCaptureRange)
    {
        return TRUE;
    }
    else
        return FALSE;
}

// Get cell from point.
// point - client coordinates
// bAllowFixedCellCheck - if TRUE then fixed cells are checked
CCellID CExGridCtrlApp::GetCellFromPt(CPoint point, BOOL bAllowFixedCellCheck /*=TRUE*/)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    CCellID cellID; // return value

    CCellID idTopLeft = GetTopleftNonFixedCell();
	if (!bAllowFixedCellCheck && !IsValid(idTopLeft))
		return cellID;

    // calculate column index
    int fixedColWidth = GetFixedColumnWidth();

    if (point.x < 0 || (!bAllowFixedCellCheck && point.x < fixedColWidth)) // not in window
        cellID.col = -1;
    else if (point.x < fixedColWidth) // in fixed col
    {
        int xpos = 0;
        int col = 0;
        while (col < pMember->m_nFixedCols)
        {
            xpos += GetColumnWidth(col);
            if (xpos > point.x)
                break;
			col++;
        }
        cellID.col = col;
    }
    else    // in non-fixed col
    {
        int xpos = fixedColWidth;
		int col = idTopLeft.col; //pMember->m_nFixedCols;
        while ( col < GetColumnCount())
        {
            xpos += GetColumnWidth(col);
            if (xpos > point.x)
                break;
			col++;
        }

        if (col >= GetColumnCount())
            cellID.col = -1;
        else
            cellID.col = col;
    }

    // calculate row index
    int fixedRowHeight = GetFixedRowHeight();
    if (point.y < 0 || (!bAllowFixedCellCheck && point.y < fixedRowHeight)) // not in window
        cellID.row = -1;
    else if (point.y < fixedRowHeight) // in fixed col
    {
        int ypos = 0;
        int row = 0;
        while (row < pMember->m_nFixedRows) 
        {
            ypos += GetRowHeight(row);
            if (ypos > point.y)
                break;
			row++;
        }
        cellID.row = row;
    }
    else
    {
        int ypos = fixedRowHeight;
		int row = idTopLeft.row; //pMember->m_nFixedRows;
        while ( row < GetRowCount() )
        {
            ypos += GetRowHeight(row);
            if (ypos > point.y)
                break;
			row++;
        }

        if (row >= GetRowCount())
            cellID.row = -1;
        else
            cellID.row = row;
    }

    return cellID;
}

////////////////////////////////////////////////////////////////////////////////
// CExGridCtrl cellrange functions

// Gets the first non-fixed cell ID
CCellID CExGridCtrlApp::GetTopleftNonFixedCell(BOOL bForceRecalculation /*=FALSE*/)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    // Used cached value if possible
    if (pMember->m_idTopLeftCell.IsValid() && !bForceRecalculation)
        return pMember->m_idTopLeftCell;

    int nVertScroll = pMember->m_pGrid->GetScrollPos(SB_VERT), 
        nHorzScroll = pMember->m_pGrid->GetScrollPos(SB_HORZ);

    pMember->m_idTopLeftCell.col = pMember->m_nFixedCols;
    int nRight = 0;
    while (nRight < nHorzScroll && pMember->m_idTopLeftCell.col < (GetColumnCount()-1))
        nRight += GetColumnWidth(pMember->m_idTopLeftCell.col++);

    pMember->m_idTopLeftCell.row = pMember->m_nFixedRows;
    int nTop = 0;
    while (nTop < nVertScroll && pMember->m_idTopLeftCell.row < (GetRowCount()-1))
        nTop += GetRowHeight(pMember->m_idTopLeftCell.row++);

    //TRACE2("TopLeft cell is row %d, col %d\n",pMember->m_idTopLeftCell.row, pMember->m_idTopLeftCell.col);
    return pMember->m_idTopLeftCell;
}

// pMember->m_pGrid gets even partially visible cells
CCellRange CExGridCtrlApp::GetVisibleNonFixedCellRange(LPRECT pRect /*=NULL*/, 
                                                  BOOL bForceRecalculation /*=FALSE*/)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    CRect rect;
    pMember->m_pGrid->GetClientRect(rect);

    CCellID idTopLeft = GetTopleftNonFixedCell(bForceRecalculation);
    int i = 0;
    // calc bottom
    int bottom = GetFixedRowHeight();
    for ( i = idTopLeft.row; i < GetRowCount(); i++)
    {
        bottom += GetRowHeight(i);
        if (bottom >= rect.bottom)
        {
            bottom = rect.bottom;
            break;
        }
    }
    int maxVisibleRow = min(i, GetRowCount() - 1);

    // calc right
    int right = GetFixedColumnWidth();
    for (i = idTopLeft.col; i < GetColumnCount(); i++)
    {
        right += GetColumnWidth(i);
        if (right >= rect.right)
        {
            right = rect.right;
            break;
        }
    }
    int maxVisibleCol = min(i, GetColumnCount() - 1);
    if (pRect)
    {
        pRect->left = pRect->top = 0;
        pRect->right = right;
        pRect->bottom = bottom;
    }

    return CCellRange(idTopLeft.row, idTopLeft.col, maxVisibleRow, maxVisibleCol);
}

// used by ResetScrollBars() - pMember->m_pGrid gets only fully visible cells
CCellRange CExGridCtrlApp::GetUnobstructedNonFixedCellRange(BOOL bForceRecalculation /*=FALSE*/)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    CRect rect;
    pMember->m_pGrid->GetClientRect(rect);

    CCellID idTopLeft = GetTopleftNonFixedCell(bForceRecalculation);

    // calc bottom
    int bottom = GetFixedRowHeight();
    int i = 0;
    for ( i = idTopLeft.row; i < GetRowCount(); i++)
    {
        bottom += GetRowHeight(i);
        if (bottom >= rect.bottom)
            break;
    }
    int maxVisibleRow = min(i, GetRowCount() - 1);
    if (maxVisibleRow > 0 && bottom > rect.bottom)
        maxVisibleRow--;

    // calc right
    int right = GetFixedColumnWidth();
    for (i = idTopLeft.col; i < GetColumnCount(); i++)
    {
        right += GetColumnWidth(i);
        if (right >= rect.right)
            break;
    }
    int maxVisibleCol = min(i, GetColumnCount() - 1);
    if (maxVisibleCol > 0 && right > rect.right)
        maxVisibleCol--;


    return CCellRange(idTopLeft.row, idTopLeft.col, maxVisibleRow, maxVisibleCol);
}

// Returns the minimum bounding range of the current selection
// If no selection, then the returned CCellRange will be invalid
CCellRange CExGridCtrlApp::GetSelectedCellRange()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    CCellRange Selection(GetRowCount(), GetColumnCount(), -1,-1);
	
    for (POSITION pos = pMember->m_SelectedCellMap.GetStartPosition(); pos != NULL; )
    {
        DWORD key;
        CCellID cell;
        pMember->m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);

        Selection.SetMinRow( min(Selection.GetMinRow(), cell.row) );
        Selection.SetMinCol( min(Selection.GetMinCol(), cell.col) );
        Selection.SetMaxRow( max(Selection.GetMaxRow(), cell.row) );
        Selection.SetMaxCol( max(Selection.GetMaxCol(), cell.col) );

    }	

   return Selection;
}

// Returns ALL the cells in the grid
CCellRange CExGridCtrlApp::GetCellRange()
{
    return CCellRange(0, 0, GetRowCount() - 1, GetColumnCount() - 1);
}

// Resets the selected cell range to the empty set.
void CExGridCtrlApp::ResetSelectedRange()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    pMember->m_PrevSelectedCellMap.RemoveAll();
    SetSelectedRange(-1,-1,-1,-1);
    SetFocusCell(-1,-1);
}

// Get/Set scroll position using 32 bit functions
int CExGridCtrlApp::GetScrollPos32(int nBar, BOOL bGetTrackPos /* = FALSE */)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);

    if (bGetTrackPos)
    {
        if (pMember->m_pGrid->GetScrollInfo(nBar, &si, SIF_TRACKPOS))
            return si.nTrackPos;
    }
    else
    {
        if (pMember->m_pGrid->GetScrollInfo(nBar, &si, SIF_POS))
            return si.nPos;
    }

    return 0;
}

BOOL CExGridCtrlApp::SetScrollPos32(int nBar, int nPos, BOOL bRedraw /* = TRUE */)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    pMember->m_idTopLeftCell.row = -1;

    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask  = SIF_POS;
    si.nPos   = nPos;
    return pMember->m_pGrid->SetScrollInfo(nBar, &si, bRedraw);
}

void CExGridCtrlApp::EnableScrollBars(int nBar, BOOL bEnable /*=TRUE*/)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (bEnable)
    {
        if (!IsVisibleHScroll() && (nBar == SB_HORZ || nBar == SB_BOTH))
        {
            pMember->m_nBarState |= GVL_HORZ;
            pMember->m_pGrid->CWnd::EnableScrollBarCtrl(SB_HORZ, bEnable);
        }
        
        if (!IsVisibleVScroll() && (nBar == SB_VERT || nBar == SB_BOTH))
        {
            pMember->m_nBarState |= GVL_VERT;
            pMember->m_pGrid->CWnd::EnableScrollBarCtrl(SB_VERT, bEnable);
        }
    }
    else
    {
        if ( IsVisibleHScroll() && (nBar == SB_HORZ || nBar == SB_BOTH))
        {
            pMember->m_nBarState &= ~GVL_HORZ; 
            pMember->m_pGrid->CWnd::EnableScrollBarCtrl(SB_HORZ, bEnable);
        }
        
        if ( IsVisibleVScroll() && (nBar == SB_VERT || nBar == SB_BOTH))
        {
            pMember->m_nBarState &= ~GVL_VERT;
            pMember->m_pGrid->CWnd::EnableScrollBarCtrl(SB_VERT, bEnable);
        }
    }
}

// If resizing or cell counts/sizes change, call pMember->m_pGrid - it'll fix up the scroll bars
void CExGridCtrlApp::ResetScrollBars()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    // Force a refresh. 
    pMember->m_idTopLeftCell.row = -1;

    if (!pMember->m_bAllowDraw || !::IsWindow(pMember->m_pGrid->GetSafeHwnd())) 
        return;
    
    CRect rect;
    
    // pMember->m_pGrid would have caused OnSize event - Brian 
    //EnableScrollBars(SB_BOTH, FALSE); 
    
    pMember->m_pGrid->GetClientRect(rect);
    
    if (rect.left == rect.right || rect.top == rect.bottom)
        return;
    
    if (IsVisibleVScroll())
        rect.right += GetSystemMetrics(SM_CXVSCROLL) + GetSystemMetrics(SM_CXBORDER);
    
    if (IsVisibleHScroll())
        rect.bottom += GetSystemMetrics(SM_CYHSCROLL) + GetSystemMetrics(SM_CYBORDER);
    
    rect.left += GetFixedColumnWidth();
    rect.top += GetFixedRowHeight();
    
    
    if (rect.left >= rect.right || rect.top >= rect.bottom)
    {
        pMember->m_pGrid->EnableScrollBarCtrl(SB_BOTH, FALSE);
        return;
    }
    
    CRect VisibleRect(GetFixedColumnWidth(), GetFixedRowHeight(), 
		              rect.right, rect.bottom);
    CRect VirtualRect(GetFixedColumnWidth(), GetFixedRowHeight(),
		              GetVirtualWidth(), GetVirtualHeight());
    
    // Removed to fix single row scrollbar problem (Pontus Goffe)
    // CCellRange visibleCells = GetUnobstructedNonFixedCellRange();
    // if (!IsValid(visibleCells)) return;
        
    //TRACE(_T("Visible: %d x %d, Virtual %d x %d.  H %d, V %d\n"), 
    //      VisibleRect.Width(), VisibleRect.Height(),
    //      VirtualRect.Width(), VirtualRect.Height(),
    //      IsVisibleHScroll(), IsVisibleVScroll());

    // If vertical scroll bar, horizontal space is reduced
    if (VisibleRect.Height() < VirtualRect.Height())
        VisibleRect.right -= ::GetSystemMetrics(SM_CXVSCROLL);
    // If horz scroll bar, vert space is reduced
    if (VisibleRect.Width() < VirtualRect.Width())
        VisibleRect.bottom -= ::GetSystemMetrics(SM_CYHSCROLL);
    
    // Recheck vertical scroll bar
    //if (VisibleRect.Height() < VirtualRect.Height())
    // VisibleRect.right -= ::GetSystemMetrics(SM_CXVSCROLL);
    
    if (VisibleRect.Height() < VirtualRect.Height())
    {
        EnableScrollBars(SB_VERT, TRUE); 
        pMember->m_nVScrollMax = VirtualRect.Height() - 1;
    }
    else
    {
        EnableScrollBars(SB_VERT, FALSE); 
        pMember->m_nVScrollMax = 0;
    }

    if (VisibleRect.Width() < VirtualRect.Width())
    {
        EnableScrollBars(SB_HORZ, TRUE); 
        pMember->m_nHScrollMax = VirtualRect.Width() - 1;
    }
    else
    {
        EnableScrollBars(SB_HORZ, FALSE); 
        pMember->m_nHScrollMax = 0;
    }

    ASSERT(pMember->m_nVScrollMax < INT_MAX && pMember->m_nHScrollMax < INT_MAX); // pMember->m_pGrid should be fine

    /* Old code - CJM
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_PAGE;
    si.nPage = (pMember->m_nHScrollMax>0)? VisibleRect.Width() : 0;
    pMember->m_pGrid->SetScrollInfo(SB_HORZ, &si, FALSE); 
    si.nPage = (pMember->m_nVScrollMax>0)? VisibleRect.Height() : 0;
    pMember->m_pGrid->SetScrollInfo(SB_VERT, &si, FALSE);

    SetScrollRange(SB_VERT, 0, pMember->m_nVScrollMax, TRUE);
    SetScrollRange(SB_HORZ, 0, pMember->m_nHScrollMax, TRUE);
    */

    // New code - Paul Runstedler 
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_PAGE | SIF_RANGE;
    si.nPage = (pMember->m_nHScrollMax>0)? VisibleRect.Width() : 0;
    si.nMin = 0;
    si.nMax = pMember->m_nHScrollMax;
    pMember->m_pGrid->SetScrollInfo(SB_HORZ, &si, TRUE);

    si.fMask |= SIF_DISABLENOSCROLL;
    si.nPage = (pMember->m_nVScrollMax>0)? VisibleRect.Height() : 0;
    si.nMin = 0;
    si.nMax = pMember->m_nVScrollMax;
    pMember->m_pGrid->SetScrollInfo(SB_VERT, &si, TRUE);

	if(!pMember->m_bHideHScrollBar)
		pMember->m_pGrid->EnableScrollBarCtrl(SB_HORZ, FALSE);
	if(!pMember->m_bHideVScrollBar)
		pMember->m_pGrid->EnableScrollBarCtrl(SB_VERT, FALSE);
}

void CExGridCtrlApp::SetGridScrollBarVisible(BOOL bHScroll, BOOL bVScroll)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bHideHScrollBar = bHScroll;
	pMember->m_bHideVScrollBar = bVScroll;
}

////////////////////////////////////////////////////////////////////////////////////
// Row/Column position functions

// returns the top left point of the cell. Returns FALSE if cell not visible.
BOOL CExGridCtrlApp::GetCellOrigin(int nRow, int nCol, LPPOINT p)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    int i;

    if (!IsValid(nRow, nCol))
        return FALSE;

    CCellID idTopLeft;
    if (nCol >= pMember->m_nFixedCols || nRow >= pMember->m_nFixedRows)
        idTopLeft = GetTopleftNonFixedCell();

    if ((nRow >= pMember->m_nFixedRows && nRow < idTopLeft.row) ||
        (nCol>= pMember->m_nFixedCols && nCol < idTopLeft.col))
        return FALSE;

    p->x = 0;
    if (nCol < pMember->m_nFixedCols)                      // is a fixed column
        for (i = 0; i < nCol; i++)
            p->x += GetColumnWidth(i);
        else 
        {                                        // is a scrollable data column
            for (i = 0; i < pMember->m_nFixedCols; i++)
                p->x += GetColumnWidth(i);
            for (i = idTopLeft.col; i < nCol; i++)
                p->x += GetColumnWidth(i);
        }
        
        p->y = 0;
        if (nRow < pMember->m_nFixedRows)                      // is a fixed row
            for (i = 0; i < nRow; i++)
                p->y += GetRowHeight(i);
            else 
            {                                        // is a scrollable data row
                for (i = 0; i < pMember->m_nFixedRows; i++)
                    p->y += GetRowHeight(i);
                for (i = idTopLeft.row; i < nRow; i++)
                    p->y += GetRowHeight(i);
            }
            
            return TRUE;
}

BOOL CExGridCtrlApp::GetCellOrigin(const CCellID& cell, LPPOINT p)
{
    return GetCellOrigin(cell.row, cell.col, p);
}

// Returns the bounding box of the cell
BOOL CExGridCtrlApp::GetCellRect(const CCellID& cell, LPRECT pRect)
{
    return GetCellRect(cell.row, cell.col, pRect);
}

BOOL CExGridCtrlApp::GetCellRect(int nRow, int nCol, LPRECT pRect)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    CPoint CellOrigin;
    if (!GetCellOrigin(nRow, nCol, &CellOrigin))
        return FALSE;

    pRect->left   = CellOrigin.x;
    pRect->top    = CellOrigin.y;
    pRect->right  = CellOrigin.x + GetColumnWidth(nCol)-1;
    pRect->bottom = CellOrigin.y + GetRowHeight(nRow)-1;

    //TRACE("Row %d, col %d: L %d, T %d, W %d, H %d:  %d,%d - %d,%d\n",
    //      nRow,nCol, CellOrigin.x, CellOrigin.y, GetColumnWidth(nCol), GetRowHeight(nRow),
    //      pRect->left, pRect->top, pRect->right, pRect->bottom);

    return TRUE;
}

BOOL CExGridCtrlApp::GetTextRect(const CCellID& cell, LPRECT pRect)
{
    return GetTextRect(cell.row, cell.col, pRect);
}

BOOL CExGridCtrlApp::GetTextRect(int nRow, int nCol, LPRECT pRect)
{
    CGridCellBase* pCell = GetCell( nRow, nCol);
    if( pCell == NULL)
        return FALSE;
    
    if( !GetCellRect( nRow, nCol, pRect) )
        return FALSE;

    return pCell->GetTextRect( pRect);
}

// Returns the bounding box of a range of cells
BOOL CExGridCtrlApp::GetCellRangeRect(const CCellRange& cellRange, LPRECT lpRect)
{
    CPoint MinOrigin,MaxOrigin;

    if (!GetCellOrigin(cellRange.GetMinRow(), cellRange.GetMinCol(), &MinOrigin))
        return FALSE;
    if (!GetCellOrigin(cellRange.GetMaxRow(), cellRange.GetMaxCol(), &MaxOrigin))
        return FALSE;

    lpRect->left   = MinOrigin.x;
    lpRect->top    = MinOrigin.y;
    lpRect->right  = MaxOrigin.x + GetColumnWidth(cellRange.GetMaxCol()) - 1;
    lpRect->bottom = MaxOrigin.y + GetRowHeight(cellRange.GetMaxRow()) - 1;

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
// Grid attribute functions

LRESULT CExGridCtrlApp::OnSetFont(WPARAM hFont, LPARAM /*lParam */)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    LRESULT result = pMember->m_pGrid->Default();

    // Get the logical font
    LOGFONT lf;
    if (!GetObject((HFONT) hFont, sizeof(LOGFONT), &lf))
        return result;

    pMember->m_cellDefault.SetFont(&lf);
    pMember->m_cellFixedColDef.SetFont(&lf);
    pMember->m_cellFixedRowDef.SetFont(&lf);
    pMember->m_cellFixedRowColDef.SetFont(&lf);

    Refresh();

    return result;
}

LRESULT CExGridCtrlApp::OnGetFont(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    //LOGFONT    lf;
    //pMember->m_cellDefault.GetFontObject()->GetLogFont(&lf);

    return (LRESULT) pMember->m_cellDefault.GetFontObject()->GetSafeHandle();
}

#ifndef _WIN32_WCE_NO_CURSOR
BOOL CExGridCtrlApp::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (nHitTest == HTCLIENT)
    {
        switch (pMember->m_MouseMode)
        {
        case MOUSE_OVER_COL_DIVIDE:
            SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
            break;
        case MOUSE_OVER_ROW_DIVIDE:
            SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
            break;
#ifndef GRIDCONTROL_NO_DRAGDROP
        case MOUSE_DRAGGING:
            break;
#endif
        default:
            if (!GetVirtualMode())
            {
                CPoint pt(GetMessagePos());
                pMember->m_pGrid->ScreenToClient(&pt);
                CCellID cell = GetCellFromPt(pt);
                if (IsValid(cell))
                {
                    CGridCellBase* pCell = GetCell(cell.row, cell.col);
                    if (pCell)
                        return pCell->OnSetCursor();
                }
            }

            SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
        }
        return TRUE;
    }

    return pMember->m_pGrid->CWnd::OnSetCursor(pWnd, nHitTest, message);
}
#endif

////////////////////////////////////////////////////////////////////////////////////
// Row/Column count functions

BOOL CExGridCtrlApp::SetFixedRowCount(int nFixedRows)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (pMember->m_nFixedRows == nFixedRows)
        return TRUE;

    ASSERT(nFixedRows >= 0);

    ResetSelectedRange();

    // Force recalculation
    pMember->m_idTopLeftCell.col = -1;

    if (nFixedRows > GetRowCount())
        if (!SetRowCount(nFixedRows))
            return FALSE;
        
        if (pMember->m_idCurrentCell.row < nFixedRows)
            SetFocusCell(-1, - 1);
        
        if (!GetVirtualMode())
        {
            if (nFixedRows > pMember->m_nFixedRows)
            {
                for (int i = pMember->m_nFixedRows; i < nFixedRows; i++)
                    for (int j = 0; j < GetColumnCount(); j++)
                    {
                        SetItemState(i, j, GetItemState(i, j) | GVIS_FIXED | GVIS_FIXEDROW);
                        SetItemBkColour(i, j, pMember->m_crFixedBkColour );
                        SetItemFgColour(i, j, pMember->m_crFixedTextColour );
                    }
            }
            else
            {
                for (int i = nFixedRows; i < pMember->m_nFixedRows; i++)
                {
                    int j;
                    for (j = 0; j < GetFixedColumnCount(); j++)
                        SetItemState(i, j, GetItemState(i, j) & ~GVIS_FIXEDROW );

                    for (j = GetFixedColumnCount(); j < GetColumnCount(); j++)
                    {
                        SetItemState(i, j, GetItemState(i, j) & ~(GVIS_FIXED | GVIS_FIXEDROW) );
                        SetItemBkColour(i, j, pMember->m_crFixedBkColour );
                        SetItemFgColour(i, j, pMember->m_crFixedTextColour );
                    }
                }
            }
        }

        pMember->m_nFixedRows = nFixedRows;
        
        Refresh();
        
        return TRUE;
}

BOOL CExGridCtrlApp::SetFixedColumnCount(int nFixedCols)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (pMember->m_nFixedCols == nFixedCols)
        return TRUE;

    ASSERT(nFixedCols >= 0);

    if (nFixedCols > GetColumnCount())
        if (!SetColumnCount(nFixedCols))
            return FALSE;

    if (pMember->m_idCurrentCell.col < nFixedCols)
        SetFocusCell(-1, - 1);

    ResetSelectedRange();

    // Force recalculation
    pMember->m_idTopLeftCell.col = -1;

    if (!GetVirtualMode())
    {
        if (nFixedCols > pMember->m_nFixedCols)
        {
            for (int i = 0; i < GetRowCount(); i++)
                for (int j = pMember->m_nFixedCols; j < nFixedCols; j++)
                {
                    SetItemState(i, j, GetItemState(i, j) | GVIS_FIXED | GVIS_FIXEDCOL);
                    SetItemBkColour(i, j, pMember->m_crFixedBkColour );
                    SetItemFgColour(i, j, pMember->m_crFixedTextColour );
                }
        }
        else
        {
			{ // Scope limit i,j
	            for (int i = 0; i < GetFixedRowCount(); i++)
		            for (int j = nFixedCols; j < pMember->m_nFixedCols; j++)
			            SetItemState(i, j, GetItemState(i, j) & ~GVIS_FIXEDCOL );
			}
			{// Scope limit i,j
	            for (int i = GetFixedRowCount(); i < GetRowCount(); i++)
		            for (int j = nFixedCols; j < pMember->m_nFixedCols; j++)
			        {
				        SetItemState(i, j, GetItemState(i, j) & ~(GVIS_FIXED | GVIS_FIXEDCOL) );
					    SetItemBkColour(i, j, pMember->m_crFixedBkColour );
						SetItemFgColour(i, j, pMember->m_crFixedTextColour );
	                }
			}
        }
    }
        
    pMember->m_nFixedCols = nFixedCols;
        
    Refresh();
        
    return TRUE;
}

BOOL CExGridCtrlApp::SetRowCount(int nRows)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    BOOL bResult = TRUE;

    ASSERT(nRows >= 0);
    if (nRows == GetRowCount())
        return bResult;

    // Force recalculation
    pMember->m_idTopLeftCell.col = -1;

    if (nRows < pMember->m_nFixedRows)
        pMember->m_nFixedRows = nRows;

    if (pMember->m_idCurrentCell.row >= nRows)
        SetFocusCell(-1, - 1);

    int addedRows = nRows - GetRowCount();

    // If we are about to lose rows, then we need to delete the GridCell objects
    // in each column within each row
    if (addedRows < 0)
    {
        if (!GetVirtualMode())
        {
            for (int row = nRows; row < pMember->m_nRows; row++)
            {
                // Delete cells
                for (int col = 0; col < pMember->m_nCols; col++)
                    DestroyCell(row, col);
            
                // Delete rows
                GRID_ROW* pRow = pMember->m_RowData[row];
                if (pRow)
                    delete pRow;
            }
        }
        pMember->m_nRows = nRows;
    }
    
    TRY
    {
        pMember->m_arRowHeights.SetSize(nRows);

        if (GetVirtualMode())
        {
            pMember->m_nRows = nRows;
            if (addedRows > 0)
            {
                int startRow = nRows - addedRows;
                for (int row = startRow; row < nRows; row++)
                    pMember->m_arRowHeights[row] = pMember->m_cellDefault.GetHeight();
            }
        }
        else
        {
            // Change the number of rows.
            pMember->m_RowData.SetSize(nRows);

            // If we have just added rows, we need to construct new elements for each cell
            // and set the default row height
            if (addedRows > 0)
            {
                // initialize row heights and data
                int startRow = nRows - addedRows;
                for (int row = startRow; row < nRows; row++)
                {
                    pMember->m_arRowHeights[row] = pMember->m_cellDefault.GetHeight();

                    pMember->m_RowData[row] = new GRID_ROW;
                    pMember->m_RowData[row]->SetSize(pMember->m_nCols);
                    for (int col = 0; col < pMember->m_nCols; col++)
                    {
                        GRID_ROW* pRow = pMember->m_RowData[row];
                        if (pRow && !GetVirtualMode())
                            pRow->SetAt(col, CreateCell(row, col));
                    }
                    pMember->m_nRows++;
                }
            }
        }
    }
    CATCH (CMemoryException, e)
    {
        e->ReportError();
        bResult = FALSE;
    }
    END_CATCH

    SetModified();
    ResetScrollBars();
    Refresh();

    return bResult;
}

BOOL CExGridCtrlApp::SetColumnCount(int nCols)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    BOOL bResult = TRUE;

    ASSERT(nCols >= 0);

    if (nCols == GetColumnCount())
        return bResult;

    // Force recalculation
    pMember->m_idTopLeftCell.col = -1;

    if (nCols < pMember->m_nFixedCols)
        pMember->m_nFixedCols = nCols;

    if (pMember->m_idCurrentCell.col >= nCols)
        SetFocusCell(-1, - 1);

    int addedCols = nCols - GetColumnCount();

    // If we are about to lose columns, then we need to delete the GridCell objects
    // within each column
    if (addedCols < 0 && !GetVirtualMode())
    {
        for (int row = 0; row < pMember->m_nRows; row++)
            for (int col = nCols; col < GetColumnCount(); col++)
                DestroyCell(row, col);
    }

    TRY 
    {
        // Change the number of columns.
        pMember->m_arColWidths.SetSize(nCols);
    
        // Change the number of columns in each row.
        if (!GetVirtualMode())
            for (int i = 0; i < pMember->m_nRows; i++)
                if (pMember->m_RowData[i])
                    pMember->m_RowData[i]->SetSize(nCols);
        
        // If we have just added columns, we need to construct new elements for each cell
        // and set the default column width
        if (addedCols > 0)
        {
            // initialized column widths
            int startCol = nCols - addedCols;
            for (int col = startCol; col < nCols; col++)
                pMember->m_arColWidths[col] = pMember->m_cellFixedColDef.GetWidth();
        
            // initialise column data
            if (!GetVirtualMode())
            {
                for (int row = 0; row < pMember->m_nRows; row++)
                    for (int col = startCol; col < nCols; col++)
                    {
                        GRID_ROW* pRow = pMember->m_RowData[row];
                        if (pRow)
                            pRow->SetAt(col, CreateCell(row, col));
                    }
            }
        }
        // else    // check for selected cell ranges
        //    ResetSelectedRange();
    }
    CATCH (CMemoryException, e)
    {
        e->ReportError();
        bResult = FALSE;
    }
    END_CATCH

    pMember->m_nCols = nCols;

    SetModified();
    ResetScrollBars();
    Refresh();

    return bResult;
}

// Insert a column at a given position, or add to end of columns (if nColumn = -1)
int CExGridCtrlApp::InsertColumn(LPCTSTR strHeading,
                            UINT nFormat /* = DT_CENTER|DT_VCENTER|DT_SINGLELINE */,
                            int nColumn  /* = -1 */)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (nColumn >= 0 && nColumn < pMember->m_nFixedCols)
    {

        // TODO: Fix it so column insertion works for in the fixed column area
        ASSERT(FALSE);
        return -1;
    }

    // If the insertion is for a specific column, check it's within range.
    if (nColumn >= 0 && nColumn > GetColumnCount())
        return -1;

    // Force recalculation
    pMember->m_idTopLeftCell.col = -1;

    ResetSelectedRange();

    // Gotta be able to at least _see_ some of the column.
    if (pMember->m_nRows < 1)
        SetRowCount(1);

    // Allow the user to insert after the last of the columns, but process it as a
    // "-1" column, meaning it gets flaged as being the last column, and not a regular
    // "insert" routine.
    if (nColumn == GetColumnCount())
        nColumn = -1;

    TRY
    {
        if (nColumn < 0)
        {
            nColumn = pMember->m_nCols;
            pMember->m_arColWidths.Add(0);
            if (!GetVirtualMode())
            {
                for (int row = 0; row < pMember->m_nRows; row++)
                {
                    GRID_ROW* pRow = pMember->m_RowData[row];
                    if (!pRow)
                        return -1;
                    pRow->Add(CreateCell(row, nColumn));
                }
            }
        } 
        else
        {
            pMember->m_arColWidths.InsertAt(nColumn, (UINT)0);
            if (!GetVirtualMode())
            {
                for (int row = 0; row < pMember->m_nRows; row++) 
                {
                    GRID_ROW* pRow = pMember->m_RowData[row];
                    if (!pRow)
                        return -1;
                    pRow->InsertAt(nColumn, CreateCell(row, nColumn));
                }
            }
        }
    }
    CATCH (CMemoryException, e)
    {
        e->ReportError();
        return -1;
    }
    END_CATCH

    pMember->m_nCols++;
    
    // Initialise column data
    SetItemText(0, nColumn, strHeading);
    for (int row = 0; row < pMember->m_nRows; row++) 
        SetItemFormat(row, nColumn, nFormat);
    
    // initialized column width
    pMember->m_arColWidths[nColumn] = GetTextExtent(0, nColumn, strHeading).cx;
    
    if (pMember->m_idCurrentCell.col != -1 && nColumn < pMember->m_idCurrentCell.col)
        pMember->m_idCurrentCell.col++;
    
    ResetScrollBars();

    SetModified();
    
    return nColumn;
}

// Insert a row at a given position, or add to end of rows (if nRow = -1)
int CExGridCtrlApp::InsertRow(LPCTSTR strHeading, int nRow /* = -1 */)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (nRow >= 0 && nRow < pMember->m_nFixedRows)
    {
        // TODO: Fix it so column insertion works for in the fixed row area
        ASSERT(FALSE);
        return -1;
    }

    // If the insertion is for a specific row, check it's within range.
    if (nRow >= 0 && nRow >= GetRowCount())
        return -1;

    // Force recalculation
    pMember->m_idTopLeftCell.col = -1;

    ResetSelectedRange();

    // Gotta be able to at least _see_ some of the row.
    if (pMember->m_nCols < 1)
        SetColumnCount(1);

    TRY
    {
        // Adding a row to the bottom
        if (nRow < 0)
        {
            nRow = pMember->m_nRows;
            pMember->m_arRowHeights.Add(0);
            if (!GetVirtualMode())
                pMember->m_RowData.Add(new GRID_ROW);
        }
        else
        {
            pMember->m_arRowHeights.InsertAt(nRow, (UINT)0);
            if (!GetVirtualMode())
                pMember->m_RowData.InsertAt(nRow, new GRID_ROW);
        }

        if (!GetVirtualMode())
            pMember->m_RowData[nRow]->SetSize(pMember->m_nCols);
    }
    CATCH (CMemoryException, e)
    {
        e->ReportError();
        return -1;
    }
    END_CATCH

    pMember->m_nRows++;

    // Initialise cell data
    if (!GetVirtualMode())
    {
        for (int col = 0; col < pMember->m_nCols; col++)
        {
            GRID_ROW* pRow = pMember->m_RowData[nRow];
            if (!pRow)
                return -1;
            pRow->SetAt(col, CreateCell(nRow, col));
        }
    }

    // Set row title
    SetItemText(nRow, 0, strHeading);

    // initialized row height
    if (strHeading && strHeading[0])
        pMember->m_arRowHeights[nRow] = GetTextExtent(nRow, 0, strHeading).cy;
    else
        pMember->m_arRowHeights[nRow] = pMember->m_cellFixedRowDef.GetHeight();

    if (pMember->m_idCurrentCell.row != -1 && nRow < pMember->m_idCurrentCell.row)
        pMember->m_idCurrentCell.row++;

    ResetScrollBars();

    SetModified();

    return nRow;
}

///////////////////////////////////////////////////////////////////////////////
// Cell creation stuff

BOOL CExGridCtrlApp::SetCellType(int nRow, int nCol, CRuntimeClass* pRuntimeClass)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (GetVirtualMode())
        return FALSE;

    ASSERT(IsValid(nRow, nCol));
    if (!IsValid(nRow, nCol))
        return FALSE;

/*
    if (!pRuntimeClass->IsDerivedFrom(RUNTIME_CLASS(CGridCellBase)))
    {
        ASSERT( FALSE);
        return FALSE;
    }
*/

    CGridCellBase* pNewCell = (CGridCellBase*) pRuntimeClass->CreateObject();

    CGridCellBase* pCurrCell = GetCell(nRow, nCol);
    if (pCurrCell)
        *pNewCell = *pCurrCell;

    SetCell(nRow, nCol, pNewCell);
    delete pCurrCell;

    return TRUE;
}

BOOL CExGridCtrlApp::SetDefaultCellType( CRuntimeClass* pRuntimeClass)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    ASSERT( pRuntimeClass != NULL );
/*
    if (!pRuntimeClass->IsDerivedFrom(RUNTIME_CLASS(CGridCellBase)))
    {
        ASSERT( FALSE);
        return FALSE;
    }
*/
    pMember->m_pRtcDefault = pRuntimeClass;
    return TRUE;
}

// Creates a new grid cell and performs any necessary initialisation
/*virtual*/ CGridCellBase* CExGridCtrlApp::CreateCell(int nRow, int nCol)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    ASSERT(!GetVirtualMode());

    if (!pMember->m_pRtcDefault/* || !pMember->m_pRtcDefault->IsDerivedFrom(RUNTIME_CLASS(CGridCellBase))*/)
    {
        ASSERT( FALSE);
        return NULL;
    }
    CGridCellBase* pCell = (CGridCellBase*) pMember->m_pRtcDefault->CreateObject();
    if (!pCell)
        return NULL;

    pCell->SetGrid((CExGridCtrl*)pMember->m_pGrid);
    pCell->SetCoords(nRow, nCol); 

    if (nCol < pMember->m_nFixedCols)
        pCell->SetState(pCell->GetState() | GVIS_FIXED | GVIS_FIXEDCOL);
    if (nRow < pMember->m_nFixedRows)
        pCell->SetState(pCell->GetState() | GVIS_FIXED | GVIS_FIXEDROW);
    
	// cell 생성할때 default 속성에 readonly 를 넣을지 여부 LYH 20050418
	// editable 을 설정하면 inplaceedit 가 기본으로 실행되는데, checkbox 등을 위해서 막는다.
	if(IsDefaultReadOnly() != FALSE)
		pCell->SetState(pCell->GetState() | GVIS_READONLY);

    pCell->SetFormat(pCell->GetDefaultCell()->GetFormat());

    return pCell;
}

// Performs any cell cleanup necessary to maintain grid integrity
/*virtual*/ void CExGridCtrlApp::DestroyCell(int nRow, int nCol)
{
    // Should NEVER get here in virtual mode.
    ASSERT(!GetVirtualMode());

    // Set the cells state to 0. If the cell is selected, pMember->m_pGrid
    // will remove the cell from the selected list.
    SetItemState(nRow, nCol, 0);

    delete GetCell(nRow, nCol);
}

BOOL CExGridCtrlApp::DeleteColumn(int nColumn)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (nColumn < 0 || nColumn >= GetColumnCount())
        return FALSE;

    ResetSelectedRange();

    if (!GetVirtualMode())
    {
        for (int row = 0; row < GetRowCount(); row++)
        {
            GRID_ROW* pRow = pMember->m_RowData[row];
            if (!pRow)
                return FALSE;

            DestroyCell(row, nColumn);
        
            pRow->RemoveAt(nColumn);
        }
    }
    pMember->m_arColWidths.RemoveAt(nColumn);
    pMember->m_nCols--;
    if (nColumn < pMember->m_nFixedCols)
        pMember->m_nFixedCols--;
    
    if (nColumn == pMember->m_idCurrentCell.col)
        pMember->m_idCurrentCell.row = pMember->m_idCurrentCell.col = -1;
    else if (nColumn < pMember->m_idCurrentCell.col)
        pMember->m_idCurrentCell.col--;
    
    ResetScrollBars();

    SetModified();

    return TRUE;
}

BOOL CExGridCtrlApp::DeleteRow(int nRow)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (nRow < 0 || nRow >= GetRowCount())
        return FALSE;

    ResetSelectedRange();

    if (!GetVirtualMode())
    {
        GRID_ROW* pRow = pMember->m_RowData[nRow];
        if (!pRow)
            return FALSE;

        for (int col = 0; col < GetColumnCount(); col++)
            DestroyCell(nRow, col);

        delete pRow;
        pMember->m_RowData.RemoveAt(nRow);
    }

    pMember->m_arRowHeights.RemoveAt(nRow);

    pMember->m_nRows--;
    if (nRow < pMember->m_nFixedRows)
        pMember->m_nFixedRows--;
    
    if (nRow == pMember->m_idCurrentCell.row)
        pMember->m_idCurrentCell.row = pMember->m_idCurrentCell.col = -1;
    else if (nRow < pMember->m_idCurrentCell.row)
        pMember->m_idCurrentCell.row--;
    
    ResetScrollBars();

    SetModified();
    
    return TRUE;
}

// Handy function that removes all non-fixed rows
BOOL CExGridCtrlApp::DeleteNonFixedRows()
{
    ResetSelectedRange();

    int nFixed = GetFixedRowCount();
    int nCount = GetRowCount();

    // Delete all data rows
    for (int nRow = nCount; nRow >= nFixed; nRow--)
        DeleteRow(nRow);

    return TRUE;
}

// Removes all rows, columns and data from the grid.
BOOL CExGridCtrlApp::DeleteAllItems()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    ResetSelectedRange();

    pMember->m_arColWidths.RemoveAll();
    pMember->m_arRowHeights.RemoveAll();

    // Delete all cells in the grid
    if (!GetVirtualMode())
    {
        for (int row = 0; row < pMember->m_nRows; row++)
        {
            for (int col = 0; col < pMember->m_nCols; col++)
                DestroyCell(row, col);

            GRID_ROW* pRow = pMember->m_RowData[row];
            delete pRow;
        }

        // Remove all rows
        pMember->m_RowData.RemoveAll();
    }

    pMember->m_idCurrentCell.row = pMember->m_idCurrentCell.col = -1;
    pMember->m_nRows = pMember->m_nFixedRows = pMember->m_nCols = pMember->m_nFixedCols = 0;

    ResetScrollBars();

    SetModified();

    return TRUE;
}

void CExGridCtrlApp::AutoFill()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (!::IsWindow(pMember->m_pGrid->m_hWnd))
        return;

    CRect rect;
    pMember->m_pGrid->GetClientRect(rect);

    SetColumnCount(rect.Width() / pMember->m_cellDefault.GetWidth() + 1);
    SetRowCount(rect.Height() / pMember->m_cellDefault.GetHeight() + 1);
    SetFixedRowCount(1);
    SetFixedColumnCount(1);
    ExpandToFit();
}

/////////////////////////////////////////////////////////////////////////////
// CExGridCtrl data functions

// Set CListCtrl::GetNextItem for details
CCellID CExGridCtrlApp::GetNextItem(CCellID& cell, int nFlags)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if ((nFlags & GVNI_ALL) == GVNI_ALL)
    {    // GVNI_ALL Search whole Grid beginning from cell
        //          First row (cell.row) -- ONLY Columns to the right of cell
        //          following rows       -- ALL  Columns
        int row = cell.row , col = cell.col + 1;
        if (row <= 0)
            row = GetFixedRowCount();
        for (; row < GetRowCount(); row++)
        {
            if (col <= 0)
                col = GetFixedColumnCount();
            for (; col < GetColumnCount(); col++)
            {
                int nState = GetItemState(row, col);
                if ((nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) || 
                    (nFlags & GVNI_FOCUSED     && nState & GVIS_FOCUSED)     ||
                    (nFlags & GVNI_SELECTED    && nState & GVIS_SELECTED)    ||
                    (nFlags & GVNI_READONLY    && nState & GVIS_READONLY)    ||
                    (nFlags & GVNI_FIXED       && nState & GVIS_FIXED)       ||
                    (nFlags & GVNI_MODIFIED    && nState & GVIS_MODIFIED))
                    return CCellID(row, col);
            }
            // go to First Column
            col = GetFixedColumnCount();
        }
    }
    else if ((nFlags & GVNI_BELOW) == GVNI_BELOW && 
             (nFlags & GVNI_TORIGHT) == GVNI_TORIGHT)
    {   // GVNI_AREA Search Grid beginning from cell to Lower-Right of Grid
        //           Only rows starting with  cell.row and below
        //           All rows   -- ONLY Columns to the right of cell
        int row = cell.row;
        if (row <= 0)
            row = GetFixedRowCount();
        for (; row < GetRowCount(); row++)
        {
            int col = cell.col + 1;
            if (col <= 0)
                col = GetFixedColumnCount();
            for (; col < GetColumnCount(); col++) 
            {
                int nState = GetItemState(row, col);
                if ((nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) || 
                    (nFlags & GVNI_FOCUSED     && nState & GVIS_FOCUSED)     ||
                    (nFlags & GVNI_SELECTED    && nState & GVIS_SELECTED)    ||
                    (nFlags & GVNI_READONLY    && nState & GVIS_READONLY)    ||
                    (nFlags & GVNI_FIXED       && nState & GVIS_FIXED)       ||
                    (nFlags & GVNI_MODIFIED    && nState & GVIS_MODIFIED))
                    return CCellID(row, col);
            }
        }
    }
    else if ((nFlags & GVNI_ABOVE) == GVNI_ABOVE) 
    {
        for (int row = cell.row - 1; row >= GetFixedRowCount(); row--) 
        {
            int nState = GetItemState(row, cell.col);
            if ((nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) || 
                (nFlags & GVNI_FOCUSED     && nState & GVIS_FOCUSED)     ||
                (nFlags & GVNI_SELECTED    && nState & GVIS_SELECTED)    ||
                (nFlags & GVNI_READONLY    && nState & GVIS_READONLY)    ||
                (nFlags & GVNI_FIXED       && nState & GVIS_FIXED)       ||
                (nFlags & GVNI_MODIFIED    && nState & GVIS_MODIFIED))
                return CCellID(row, cell.col);
        }
    }
    else if ((nFlags & GVNI_BELOW) == GVNI_BELOW)
    {
        for (int row = cell.row + 1; row < GetRowCount(); row++) 
        {
            int nState = GetItemState(row, cell.col);
            if ((nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) || 
                (nFlags & GVNI_FOCUSED     && nState & GVIS_FOCUSED)     ||
                (nFlags & GVNI_SELECTED    && nState & GVIS_SELECTED)    ||
                (nFlags & GVNI_READONLY    && nState & GVIS_READONLY)    ||
                (nFlags & GVNI_FIXED       && nState & GVIS_FIXED)       ||
                (nFlags & GVNI_MODIFIED    && nState & GVIS_MODIFIED))
                return CCellID(row, cell.col);
        }
    } 
    else if ((nFlags & GVNI_TOLEFT) == GVNI_TOLEFT)
    {
        for (int col = cell.col - 1; col >= GetFixedColumnCount(); col--) 
        {
            int nState = GetItemState(cell.row, col);
            if ((nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) || 
                (nFlags & GVNI_FOCUSED     && nState & GVIS_FOCUSED)     ||
                (nFlags & GVNI_SELECTED    && nState & GVIS_SELECTED)    ||
                (nFlags & GVNI_READONLY    && nState & GVIS_READONLY)    ||
                (nFlags & GVNI_FIXED       && nState & GVIS_FIXED)       ||
                (nFlags & GVNI_MODIFIED    && nState & GVIS_MODIFIED))
                return CCellID(cell.row, col);
        }
    }
    else if ((nFlags & GVNI_TORIGHT) == GVNI_TORIGHT)
    {
        for (int col = cell.col + 1; col < GetColumnCount(); col++) 
        {
            int nState = GetItemState(cell.row, col);
            if ((nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) || 
                (nFlags & GVNI_FOCUSED     && nState & GVIS_FOCUSED)     ||
                (nFlags & GVNI_SELECTED    && nState & GVIS_SELECTED)    ||
                (nFlags & GVNI_READONLY    && nState & GVIS_READONLY)    ||
                (nFlags & GVNI_FIXED       && nState & GVIS_FIXED)       ||
                (nFlags & GVNI_MODIFIED    && nState & GVIS_MODIFIED))
                return CCellID(cell.row, col);
        }
    }
    
    return CCellID(-1, -1);
}

// Sorts on a given column using the cell text
BOOL CExGridCtrlApp::SortTextItems(int nCol, BOOL bAscending, LPARAM data /* = 0 */)
{
    return SortItems(pfnCellTextCompare, nCol, bAscending, data);
}

void CExGridCtrlApp::SetCompareFunction(PFNLVCOMPARE pfnCompare)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	pMember->m_pfnCompare = pfnCompare;
}

// Sorts on a given column using the cell text and using the specified comparison
// function
BOOL CExGridCtrlApp::SortItems(int nCol, BOOL bAscending, LPARAM data /* = 0 */)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    SetSortColumn(nCol);
    SetSortAscending(bAscending);
    ResetSelectedRange();
    SetFocusCell(-1, - 1);

	if (pMember->m_pfnCompare == NULL)
		return SortItems(pfnCellTextCompare, nCol, bAscending, data);
	else
	    return SortItems(pMember->m_pfnCompare, nCol, bAscending, data);
}

// Sorts on a given column using the supplied compare function (see CListCtrl::SortItems)
BOOL CExGridCtrlApp::SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending,
                          LPARAM data /* = 0 */)
{
    SetSortColumn(nCol);
    SetSortAscending(bAscending);
    ResetSelectedRange();
    SetFocusCell(-1, -1);
    return SortItems(pfnCompare, nCol, bAscending, data, GetFixedRowCount(), -1);
}

int CALLBACK CExGridCtrlApp::pfnCellTextCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	UNUSED_ALWAYS(lParamSort);

	CGridCellBase* pCell1 = (CGridCellBase*) lParam1;
	CGridCellBase* pCell2 = (CGridCellBase*) lParam2;
	if (!pCell1 || !pCell2) return 0;

	return _tcscmp(pCell1->GetText(), pCell2->GetText());
}

int CALLBACK CExGridCtrlApp::pfnCellNumericCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	UNUSED_ALWAYS(lParamSort);

	CGridCellBase* pCell1 = (CGridCellBase*) lParam1;
	CGridCellBase* pCell2 = (CGridCellBase*) lParam2;
	if (!pCell1 || !pCell2) return 0;

	int nValue1 = _ttol(pCell1->GetText());
	int nValue2 = _ttol(pCell2->GetText());

	if (nValue1 < nValue2)
		return -1;
	else if (nValue1 == nValue2)
		return 0;
	else
		return 1;
}

// private recursive sort implementation
BOOL CExGridCtrlApp::SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data,
                          int low, int high)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (nCol >= GetColumnCount())
        return FALSE;

    if (high == -1)
        high = GetRowCount() - 1;

    int lo = low;
    int hi = high;
    
    if (hi <= lo)
        return FALSE;
    
    //LPARAM midItem = GetItemData((lo + hi)/2, nCol);
	LPARAM pMidCell = (LPARAM) GetCell((lo + hi)/2, nCol);
    
    // loop through the list until indices cross
    while (lo <= hi)
    {
        // Find the first element that is greater than or equal to the partition 
        // element starting from the left Index.
        if (bAscending)
            while (lo < high  && pfnCompare( (LPARAM)GetCell(lo, nCol), (LPARAM) pMidCell, data) < 0)
                ++lo;
        else
            while (lo < high && pfnCompare((LPARAM)GetCell(lo, nCol), pMidCell, data) > 0)
				++lo;
                
		// Find an element that is smaller than or equal to  the partition 
		// element starting from the right Index.
		if (bAscending)
			while (hi > low && pfnCompare((LPARAM)GetCell(hi, nCol), pMidCell, data) > 0)
				--hi;
		else
			while (hi > low && pfnCompare((LPARAM)GetCell(hi, nCol), pMidCell, data) < 0)
				--hi;
                        
        // If the indexes have not crossed, swap if the items are not equal
        if (lo <= hi)
        {
			// swap only if the items are not equal
            if (pfnCompare((LPARAM)GetCell(lo, nCol), (LPARAM)GetCell(hi, nCol), data) != 0)
            {
				for (int col = 0; col < GetColumnCount(); col++)
                {
					CGridCellBase *pCell = GetCell(lo, col);
                    SetCell(lo, col, GetCell(hi, col));
                    SetCell(hi, col, pCell);
                }
                UINT nRowHeight = pMember->m_arRowHeights[lo];
                pMember->m_arRowHeights[lo] = pMember->m_arRowHeights[hi];
                pMember->m_arRowHeights[hi] = nRowHeight;
            }
                            
            ++lo;
            --hi;
         }
    }
    
    // If the right index has not reached the left side of array
    // must now sort the left partition.
    if (low < hi)
        SortItems(pfnCompare, nCol, bAscending, data, low, hi);
    
    // If the left index has not reached the right side of array
    // must now sort the right partition.
    if (lo < high)
        SortItems(pfnCompare, nCol, bAscending, data, lo, high);
    
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CExGridCtrl data functions

BOOL CExGridCtrlApp::SetItem(const GV_ITEM* pItem)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (!pItem || GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(pItem->row, pItem->col);
    if (!pCell)
        return FALSE;

    SetModified(TRUE, pItem->row, pItem->col);

    if (pItem->mask & GVIF_TEXT)
        pCell->SetText(pItem->strText);
    if (pItem->mask & GVIF_PARAM)
        pCell->SetData(pItem->lParam);
    if (pItem->mask & GVIF_IMAGE)
        pCell->SetImage(pItem->iImage);
    if (pItem->mask & GVIF_STATE)
        SetItemState(pItem->row, pItem->col, pItem->nState);
    if (pItem->mask & GVIF_FORMAT)
        pCell->SetFormat(pItem->nFormat);
    if (pItem->mask & GVIF_BKCLR)
        pCell->SetBackClr(pItem->crBkClr);
    if (pItem->mask & GVIF_FGCLR)
        pCell->SetTextClr(pItem->crFgClr);
    if (pItem->mask & GVIF_FONT)
        pCell->SetFont(&(pItem->lfFont));
    if( pItem->mask & GVIF_MARGIN)
        pCell->SetMargin( pItem->nMargin);
    
    return TRUE;
}

BOOL CExGridCtrlApp::GetItem(GV_ITEM* pItem)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (!pItem)
        return FALSE;
    CGridCellBase* pCell = GetCell(pItem->row, pItem->col);
    if (!pCell)
        return FALSE;

    if (pItem->mask & GVIF_TEXT)
        pItem->strText = GetItemText(pItem->row, pItem->col);
    if (pItem->mask & GVIF_PARAM)
        pItem->lParam  = pCell->GetData();;
    if (pItem->mask & GVIF_IMAGE)
        pItem->iImage  = pCell->GetImage();
    if (pItem->mask & GVIF_STATE)
        pItem->nState  = pCell->GetState();
    if (pItem->mask & GVIF_FORMAT)
        pItem->nFormat = pCell->GetFormat();
    if (pItem->mask & GVIF_BKCLR)
        pItem->crBkClr = pCell->GetBackClr();
    if (pItem->mask & GVIF_FGCLR)
        pItem->crFgClr = pCell->GetTextClr();
    if (pItem->mask & GVIF_FONT)
        memcpy(&(pItem->lfFont), pCell->GetFont(), sizeof(LOGFONT));
    if( pItem->mask & GVIF_MARGIN)
        pItem->nMargin = pCell->GetMargin();

    return TRUE;
}

BOOL CExGridCtrlApp::SetItemText(int nRow, int nCol, LPCTSTR str)
{
    if (GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    if (!pCell)
        return FALSE;

    pCell->SetText(str);

    SetModified(TRUE, nRow, nCol);
    return TRUE;
}

#if !defined(_WIN32_WCE) || (_WIN32_WCE >= 210)
// EFW - 06/13/99 - Added to support printf-style formatting codes
BOOL CExGridCtrlApp::SetItemTextFmt(int nRow, int nCol, LPCTSTR szFmt, ...)
{
    if (GetVirtualMode())
        return FALSE;

    CString strText;

    va_list argptr;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    if (!pCell)
        return FALSE;

    // Format the message text
    va_start(argptr, szFmt);
    strText.FormatV(szFmt, argptr);
    va_end(argptr);

    pCell->SetText(strText);

    SetModified(TRUE, nRow, nCol);
    return TRUE;
}

// EFW - 06/13/99 - Added to support string resource ID.  Supports
// a variable argument list too.
BOOL CExGridCtrlApp::SetItemTextFmtID(int nRow, int nCol, UINT nID, ...)
{
    if (GetVirtualMode())
        return FALSE;

    CString strFmt, strText;
    va_list argptr;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    if (!pCell)
        return FALSE;

    // Format the message text
    va_start(argptr, nID);
    VERIFY(strFmt.LoadString(nID));
    strText.FormatV(strFmt, argptr);
    va_end(argptr);

    pCell->SetText(strText);

    SetModified(TRUE, nRow, nCol);
    return TRUE;
}
#endif

BOOL CExGridCtrlApp::SetItemData(int nRow, int nCol, LPARAM lParam)
{
    if (GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    if (!pCell)
        return FALSE;

    pCell->SetData(lParam);
    SetModified(TRUE, nRow, nCol);
    return TRUE;
}

LPARAM CExGridCtrlApp::GetItemData(int nRow, int nCol)
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    if (!pCell)
        return (LPARAM) 0;

    return pCell->GetData();
}

BOOL CExGridCtrlApp::SetItemImage(int nRow, int nCol, int iImage)
{
    if (GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    if (!pCell)
        return FALSE;

    pCell->SetImage(iImage);
    SetModified(TRUE, nRow, nCol);
    return TRUE;
}

int CExGridCtrlApp::GetItemImage(int nRow, int nCol)
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return -1;

    return pCell->GetImage();
}

BOOL CExGridCtrlApp::SetItemState(int nRow, int nCol, UINT state)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    BOOL bSelected = IsCellSelected(nRow, nCol);

    // If the cell is being unselected, remove it from the selected list
    if (bSelected && !(state & GVIS_SELECTED))
    {
        CCellID cell;
        DWORD key = MAKELONG(nRow, nCol);

        if (pMember->m_SelectedCellMap.Lookup(key, (CCellID&)cell))
            pMember->m_SelectedCellMap.RemoveKey(key);
    }

    // If cell is being selected, add it to the list of selected cells
    else if (!bSelected && (state & GVIS_SELECTED))
    {
        CCellID cell(nRow, nCol);
        pMember->m_SelectedCellMap.SetAt(MAKELONG(nRow, nCol), cell);
    }

    if (GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return FALSE;

    // Set the cell's state
    pCell->SetState(state);

    return TRUE;
}

UINT CExGridCtrlApp::GetItemState(int nRow, int nCol)
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return 0;

    return pCell->GetState();
}

BOOL CExGridCtrlApp::SetItemFormat(int nRow, int nCol, UINT nFormat)
{
    if (GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return FALSE;

    pCell->SetFormat(nFormat);
    return TRUE;
}

UINT CExGridCtrlApp::GetItemFormat(int nRow, int nCol)
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return 0;

    return pCell->GetFormat();
}

BOOL CExGridCtrlApp::SetItemBkColour(int nRow, int nCol, COLORREF cr /* = CLR_DEFAULT */)
{
    if (GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return FALSE;

    pCell->SetBackClr(cr);
    return TRUE;
}

COLORREF CExGridCtrlApp::GetItemBkColour(int nRow, int nCol)
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return 0;

    return pCell->GetBackClr();
}

BOOL CExGridCtrlApp::SetItemFgColour(int nRow, int nCol, COLORREF cr /* = CLR_DEFAULT */)
{
    if (GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return FALSE;
    
    pCell->SetTextClr(cr);
    return TRUE;
}

COLORREF CExGridCtrlApp::GetItemFgColour(int nRow, int nCol)
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return 0;
    
    return pCell->GetTextClr();
}

BOOL CExGridCtrlApp::SetItemFont(int nRow, int nCol, const LOGFONT* plf)
{
    if (GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return FALSE;
    
    pCell->SetFont(plf);
    
    return TRUE;
}

LOGFONT* CExGridCtrlApp::GetItemFont(int nRow, int nCol)
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell) 
        return GetDefaultCell(nRow < GetFixedRowCount(), nCol < GetFixedColumnCount())->GetFont();
    
    return pCell->GetFont();
}

BOOL CExGridCtrlApp::IsItemEditing(int nRow, int nCol)
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return FALSE;

    return pCell->IsEditing();
}

////////////////////////////////////////////////////////////////////////////////////
// Row/Column size functions

long CExGridCtrlApp::GetVirtualWidth()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    long lVirtualWidth = 0;
    int iColCount = GetColumnCount();
    for (int i = 0; i < iColCount; i++)
        lVirtualWidth += pMember->m_arColWidths[i];

    return lVirtualWidth;
}

long CExGridCtrlApp::GetVirtualHeight()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    long lVirtualHeight = 0;
    int iRowCount = GetRowCount();
    for (int i = 0; i < iRowCount; i++)
        lVirtualHeight += pMember->m_arRowHeights[i];

    return lVirtualHeight;
}

int CExGridCtrlApp::GetRowHeight(int nRow)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    ASSERT(nRow >= 0 && nRow < pMember->m_nRows);
    if (nRow < 0 || nRow >= pMember->m_nRows)
        return -1;

    return pMember->m_arRowHeights[nRow];
}

int CExGridCtrlApp::GetColumnWidth(int nCol)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    ASSERT(nCol >= 0 && nCol < pMember->m_nCols);
    if (nCol < 0 || nCol >= pMember->m_nCols)
        return -1;

    return pMember->m_arColWidths[nCol];
}

BOOL CExGridCtrlApp::SetRowHeight(int nRow, int height)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    ASSERT(nRow >= 0 && nRow < pMember->m_nRows && height >= 0);
    if (nRow < 0 || nRow >= pMember->m_nRows || height < 0)
        return FALSE;

    pMember->m_arRowHeights[nRow] = height;
    ResetScrollBars();

    return TRUE;
}

BOOL CExGridCtrlApp::SetColumnWidth(int nCol, int width)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    ASSERT(nCol >= 0 && nCol < pMember->m_nCols && width >= 0);
    if (nCol < 0 || nCol >= pMember->m_nCols || width < 0)
        return FALSE;

    pMember->m_arColWidths[nCol] = width;
    ResetScrollBars();

    return TRUE;
}

int CExGridCtrlApp::GetFixedRowHeight()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    int nHeight = 0;
    for (int i = 0; i < pMember->m_nFixedRows; i++)
        nHeight += GetRowHeight(i);

    return nHeight;
}

int CExGridCtrlApp::GetFixedColumnWidth()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    int nWidth = 0;
    for (int i = 0; i < pMember->m_nFixedCols; i++)
        nWidth += GetColumnWidth(i);

    return nWidth;
}

BOOL CExGridCtrlApp::AutoSizeColumn(int nCol, UINT nAutoSizeStyle /*=GVS_DEFAULT*/, 
                               BOOL bResetScroll /*=TRUE*/)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    ASSERT(nCol >= 0 && nCol < pMember->m_nCols);
    if (nCol < 0 || nCol >= pMember->m_nCols)
        return FALSE;

    //  Skip hidden columns when autosizing
    if( GetColumnWidth( nCol) <=0 )
        return FALSE;

    CSize size;
    CDC* pDC = pMember->m_pGrid->GetDC();
    if (!pDC)
        return FALSE;

    int nWidth = 0;

    ASSERT(GVS_DEFAULT <= nAutoSizeStyle && nAutoSizeStyle <= GVS_BOTH);
    if (nAutoSizeStyle == GVS_DEFAULT)
        nAutoSizeStyle = GetAutoSizeStyle();

    int nStartRow = (nAutoSizeStyle & GVS_HEADER)? 0 : GetFixedRowCount();
    int nEndRow   = (nAutoSizeStyle & GVS_DATA)? GetRowCount()-1 : GetFixedRowCount()-1;

    if (GetVirtualMode())
        SendCacheHintToParent(CCellRange(nStartRow, nCol, nEndRow, nCol));

    for (int nRow = nStartRow; nRow <= nEndRow; nRow++)
    {
        CGridCellBase* pCell = GetCell(nRow, nCol);
        if (pCell)
            size = pCell->GetCellExtent(pDC);
        if (size.cx > nWidth)
            nWidth = size.cx;
    }

    if (GetVirtualMode())
        SendCacheHintToParent(CCellRange(-1,-1,-1,-1));

    pMember->m_arColWidths[nCol] = nWidth;

    pMember->m_pGrid->ReleaseDC(pDC);
    if (bResetScroll)
        ResetScrollBars();

    return TRUE;
}

BOOL CExGridCtrlApp::AutoSizeRow(int nRow, BOOL bResetScroll /*=TRUE*/)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    ASSERT(nRow >= 0 && nRow < pMember->m_nRows);
    if (nRow < 0 || nRow >= pMember->m_nRows)
        return FALSE;

    //  Skip hidden rows when autosizing
    if( GetRowHeight( nRow) <=0 )
        return FALSE;

    CSize size;
    CDC* pDC = pMember->m_pGrid->GetDC();
    if (!pDC)
        return FALSE;

    int nHeight = 0;
    int nNumColumns = GetColumnCount();

    if (GetVirtualMode())
        SendCacheHintToParent(CCellRange(nRow, 0, nRow, nNumColumns));

    for (int nCol = 0; nCol < nNumColumns; nCol++)
    {
        CGridCellBase* pCell = GetCell(nRow, nCol);
        if (pCell)
            size = pCell->GetCellExtent(pDC);
        if (size.cy > nHeight)
            nHeight = size.cy;
    }
    pMember->m_arRowHeights[nRow] = nHeight;

    if (GetVirtualMode())
        SendCacheHintToParent(CCellRange(-1,-1,-1,-1));

    pMember->m_pGrid->ReleaseDC(pDC);
    if (bResetScroll)
        ResetScrollBars();

    return TRUE;
}

void CExGridCtrlApp::AutoSizeColumns(UINT nAutoSizeStyle /*=GVS_DEFAULT*/)
{
    int nNumColumns = GetColumnCount();
    for (int nCol = 0; nCol < nNumColumns; nCol++)
    {
        //  Skip hidden columns when autosizing
        if( GetColumnWidth( nCol) > 0 )
            AutoSizeColumn(nCol, nAutoSizeStyle, FALSE);
    }
    ResetScrollBars();
}

void CExGridCtrlApp::AutoSizeRows()
{
    int nNumRows = GetRowCount();
    for (int nRow = 0; nRow < nNumRows; nRow++)
    {
        //  Skip hidden rows when autosizing
        if( GetRowHeight( nRow) > 0 )
            AutoSizeRow(nRow, FALSE);
    }
    ResetScrollBars();
}

// sizes all rows and columns
// faster than calling both AutoSizeColumns() and AutoSizeRows()
void CExGridCtrlApp::AutoSize(UINT nAutoSizeStyle /*=GVS_DEFAULT*/)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    CDC* pDC = pMember->m_pGrid->GetDC();
    if (!pDC)
        return;

    int nNumColumns = GetColumnCount();

    int nCol, nRow;

    ASSERT(GVS_DEFAULT <= nAutoSizeStyle && nAutoSizeStyle <= GVS_BOTH);
    if (nAutoSizeStyle == GVS_DEFAULT)
        nAutoSizeStyle = GetAutoSizeStyle();

    int nStartRow = (nAutoSizeStyle & GVS_HEADER)? 0 : GetFixedRowCount();
    int nEndRow   = (nAutoSizeStyle & GVS_DATA)? GetRowCount()-1 : GetFixedRowCount()-1;

    if (GetVirtualMode())
        SendCacheHintToParent(CCellRange(nStartRow, 0, nEndRow, nNumColumns));

    // Row initialisation - only work on rows whose height is > 0
    for (nRow = nStartRow; nRow <= nEndRow; nRow++)
    {
        if( GetRowHeight( nRow) > 0 )
            pMember->m_arRowHeights[nRow] = 1;
    }

    CSize size;
    for (nCol = 0; nCol < nNumColumns; nCol++)
    {
        //  Don't size hidden columns or rows
        if( GetColumnWidth( nCol) > 0 )
        {
            // Skip columns that are hidden, but now initialize
            pMember->m_arColWidths[nCol] = 0;
            for (nRow = nStartRow; nRow <= nEndRow; nRow++)
            {
                if( GetRowHeight( nRow) > 0 )
                {
                    CGridCellBase* pCell = GetCell(nRow, nCol);
                    if (pCell)
                        size = pCell->GetCellExtent(pDC);
                    if (size.cx >(int) pMember->m_arColWidths[nCol])
                        pMember->m_arColWidths[nCol] = size.cx;
                    if (size.cy >(int) pMember->m_arRowHeights[nRow])
                        pMember->m_arRowHeights[nRow] = size.cy;
                }
            }
        }
    }

    if (GetVirtualMode())
        SendCacheHintToParent(CCellRange(-1,-1,-1,-1));

    pMember->m_pGrid->ReleaseDC(pDC);

    ResetScrollBars();
    Refresh();
}

// Expands the columns to fit the screen space. If bExpandFixed is FALSE then fixed 
// columns will not be affected
void CExGridCtrlApp::ExpandColumnsToFit(BOOL bExpandFixed /*=TRUE*/)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (bExpandFixed)
	{
		if (GetColumnCount() <= 0) return;
	} 
	else
	{
		if (GetColumnCount() <= GetFixedColumnCount()) return;
	}

    EnableScrollBars(SB_HORZ, FALSE);

    CRect rect;
    pMember->m_pGrid->GetClientRect(rect);

    int nFirstColumn = (bExpandFixed)? 0 : GetFixedColumnCount();

    int nNumColumnsAffected = 0;
    int col = 0;
    for ( col = nFirstColumn; col < GetColumnCount(); col++)
    {
        if (pMember->m_arColWidths[col] > 0)
            nNumColumnsAffected++;
    }

    if (nNumColumnsAffected <= 0)
        return;

    long virtualWidth = GetVirtualWidth();
    int nDifference = rect.Width() -(int) virtualWidth;
    int nColumnAdjustment = nDifference / nNumColumnsAffected;

    for (col = nFirstColumn; col < GetColumnCount(); col++)
    {
        if (pMember->m_arColWidths[col] > 0)
            pMember->m_arColWidths[col] += nColumnAdjustment;    
    }

    if (nDifference > 0)
    {
        int leftOver = nDifference % nNumColumnsAffected;
        for (int nCount = 0, col = nFirstColumn; 
             (col < GetColumnCount()) && (nCount < leftOver); col++, nCount++)
        {
            if (pMember->m_arColWidths[col] > 0)
                pMember->m_arColWidths[col] += 1;
        }
    }
    else 
    {
        int leftOver = (-nDifference) % nNumColumnsAffected;
        for (int nCount = 0, col = nFirstColumn; 
             (col < GetColumnCount()) && (nCount < leftOver); col++, nCount++)
        {
            if (pMember->m_arColWidths[col] > 0)
                pMember->m_arColWidths[col] -= 1;
        }
    }

    Refresh();

    ResetScrollBars();
}

void CExGridCtrlApp::ExpandLastColumn()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (GetColumnCount() <= 0)
        return;

    // Search for last non-hidden column
    int nLastColumn = GetColumnCount()-1;
    while (pMember->m_arColWidths[nLastColumn] <= 0)
        nLastColumn--;

    if (nLastColumn <= 0)
        return;

    EnableScrollBars(SB_HORZ, FALSE);

    CRect rect;
    pMember->m_pGrid->GetClientRect(rect);

    long virtualWidth = GetVirtualWidth();
    int nDifference = rect.Width() -(int) virtualWidth;

    if (nDifference > 0)
    {
        //if (GetVirtualHeight() > rect.Height())
        //    nDifference -= GetSystemMetrics(SM_CXVSCROLL);

        pMember->m_arColWidths[ nLastColumn ] += nDifference;
        Refresh();
    }

    ResetScrollBars();
}

// Expands the rows to fit the screen space. If bExpandFixed is FALSE then fixed 
// rows will not be affected
void CExGridCtrlApp::ExpandRowsToFit(BOOL bExpandFixed /*=TRUE*/)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (bExpandFixed)
	{
		if (GetRowCount() <= 0) return;
	} 
	else
	{
		if (GetRowCount() <= GetFixedRowCount()) return;
	}

    EnableScrollBars(SB_VERT, FALSE); 

    CRect rect;
    pMember->m_pGrid->GetClientRect(rect);
    
    int nFirstRow = (bExpandFixed)? 0 : GetFixedRowCount();
    int row = 0;
    int nNumRowsAffected = 0;
    for ( row = nFirstRow; row < GetRowCount(); row++)
    {
        if (pMember->m_arRowHeights[row] > 0)
            nNumRowsAffected++;
    }

    if (nNumRowsAffected <= 0)
        return;

    long virtualHeight = GetVirtualHeight();
    int nDifference = rect.Height() -(int) virtualHeight;
    int nRowAdjustment = nDifference / nNumRowsAffected;
    
    for (row = nFirstRow; row < GetRowCount(); row++)
    {
        if (pMember->m_arRowHeights[row] > 0)
            pMember->m_arRowHeights[row] += nRowAdjustment;    
    }
    
    if (nDifference > 0)
    {
        int leftOver = nDifference % nNumRowsAffected;
        for (int nCount = 0, row = nFirstRow; 
             (row < GetRowCount()) && (nCount < leftOver); row++, nCount++)
        {
            if (pMember->m_arRowHeights[row] > 0)
                pMember->m_arRowHeights[row] += 1;
        }
    } 
    else 
    {
        int leftOver = (-nDifference) % nNumRowsAffected;
        for (int nCount = 0, row = nFirstRow; 
             (row < GetRowCount()) && (nCount < leftOver); row++, nCount++)
        {
            if (pMember->m_arRowHeights[row] > 0)
                pMember->m_arRowHeights[row] -= 1;
        }
    }

    Refresh();

    ResetScrollBars();
}

// Expands the cells to fit the screen space. If bExpandFixed is FALSE then fixed 
// cells  will not be affected
void CExGridCtrlApp::ExpandToFit(BOOL bExpandFixed /*=TRUE*/)
{
    ExpandColumnsToFit(bExpandFixed);   // pMember->m_pGrid will remove any existing horz scrollbar
    ExpandRowsToFit(bExpandFixed);      // pMember->m_pGrid will remove any existing vert scrollbar
    ExpandColumnsToFit(bExpandFixed);   // Just in case the first adjustment was with a vert
                                        // scrollbar in place
    Refresh();
}

/////////////////////////////////////////////////////////////////////////////////////
// Attributes

void CExGridCtrlApp::SetVirtualMode(BOOL bVirtual)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    DeleteAllItems();
    pMember->m_bVirtualMode = bVirtual;

    // Force some defaults here.
    if (pMember->m_bVirtualMode)
    {
        SetEditable(FALSE);
        SetHeaderSort(FALSE);
        SetAutoSizeStyle(GVS_HEADER);
        SetFixedColumnSelection(FALSE);
        SetFixedRowSelection(FALSE);
    }
}

void CExGridCtrlApp::SetGridLines(int nWhichLines /*=GVL_BOTH*/) 
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    pMember->m_nGridLines = nWhichLines;
    Refresh();
}

void CExGridCtrlApp::SetListMode(BOOL bEnableListMode /*=TRUE*/)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    ResetSelectedRange();
    SetSortColumn(-1);
    pMember->m_bListMode = bEnableListMode;
    SetFixedRowSelection(FALSE);
    Refresh();
}

void CExGridCtrlApp::SetSortColumn(int nCol)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (pMember->m_nSortColumn >= 0)
        InvalidateCellRect(0, pMember->m_nSortColumn);
    pMember->m_nSortColumn = nCol;
    if (nCol >= 0)
        InvalidateCellRect(0, nCol);
}

BOOL CExGridCtrlApp::IsCellFixed(int nRow, int nCol)
{
    return (nRow < GetFixedRowCount() || nCol < GetFixedColumnCount());
}

void CExGridCtrlApp::SetModified(BOOL bModified /*=TRUE*/, int nRow /*=-1*/, int nCol /*=-1*/)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    // Cannot guarantee sorting anymore...
    if (nCol < 0 || nCol == GetSortColumn())
        SetSortColumn(-1);

    if (nRow >= 0 && nCol >= 0)
    {
        if (bModified)
        {
            SetItemState(nRow, nCol, GetItemState(nRow, nCol) | GVIS_MODIFIED);
            pMember->m_bModified = TRUE;
        }
        else
            SetItemState(nRow, nCol, GetItemState(nRow, nCol) & ~GVIS_MODIFIED);
    }
    else
        pMember->m_bModified = bModified;

    if (!pMember->m_bModified)
    {
        for (int row = 0; row < GetRowCount(); row++)
            for (int col = 0; col < GetColumnCount(); col++)
                SetItemState(row, col, GetItemState(row, col) & ~GVIS_MODIFIED);
    }
}

BOOL CExGridCtrlApp::GetModified(int nRow /*=-1*/, int nCol /*=-1*/)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (nRow >= 0 && nCol >= 0)
        return ( (GetItemState(nRow, nCol) & GVIS_MODIFIED) == GVIS_MODIFIED );
    else
        return pMember->m_bModified;
}

/////////////////////////////////////////////////////////////////////////////////////
// GridCtrl cell visibility tests and invalidation/redraw functions

void CExGridCtrlApp::Refresh()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (pMember->m_pGrid->GetSafeHwnd() && pMember->m_bAllowDraw)
        pMember->m_pGrid->Invalidate();
}

// EnsureVisible supplied by Roelf Werkman
void CExGridCtrlApp::EnsureVisible(int nRow, int nCol)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (!pMember->m_bAllowDraw)
        return;

    CRect rectWindow;
    /*
    // set the scroll to the approximate position of row (Nigel Page-Jones)
    int nPos = (int)((float)nRow / GetRowCount() * 1000);
    float fPos = (float)nPos / 1000;
    SCROLLINFO scrollInfo;
    pMember->m_pGrid->GetScrollInfo(SB_VERT, &scrollInfo);
    scrollInfo.nPos = (int)(scrollInfo.nMax * fPos);
    pMember->m_pGrid->SetScrollInfo(SB_VERT, &scrollInfo, FALSE);
    
    pMember->m_pGrid->GetClientRect(rectWindow);

    // redraw cells    if necessary (Nigel Page-Jones)
    CCellID idTopLeft = GetTopleftNonFixedCell(FALSE);
    CCellID idNewTopLeft = GetTopleftNonFixedCell(TRUE);
    if (idNewTopLeft != idTopLeft)
    {
        rectWindow.top = GetFixedRowHeight();
        pMember->m_pGrid->InvalidateRect(rectWindow);
    }
    */

    // We are going to send some scroll messages, which will steal the focus 
    // from it's rightful owner. Squirrel it away ourselves so we can give
    // it back. (Damir)
    CWnd* pFocusWnd = pMember->m_pGrid->GetFocus();

    CCellRange VisibleCells = GetVisibleNonFixedCellRange();

    int right = nCol - VisibleCells.GetMaxCol();
    int left  = VisibleCells.GetMinCol() - nCol;
    int down  = nRow - VisibleCells.GetMaxRow();
    int up    = VisibleCells.GetMinRow() - nRow;

    int iColumnStart;
    int iRowStart;

    iColumnStart = VisibleCells.GetMaxCol() + 1;
    while( right > 0 )
    {
        if( GetColumnWidth( iColumnStart ) > 0 )
            pMember->m_pGrid->SendMessage( WM_HSCROLL, SB_LINERIGHT, 0 );

        right--;
        iColumnStart++;
    }

    iColumnStart = VisibleCells.GetMinCol() - 1;
    while( left > 0 )
    {
        if( GetColumnWidth( iColumnStart ) > 0 )
            pMember->m_pGrid->SendMessage( WM_HSCROLL, SB_LINELEFT, 0 );
        left--;
        iColumnStart--;
    }

    iRowStart = VisibleCells.GetMaxRow() + 1;
    while( down > 0 )
    {
        if( GetRowHeight( iRowStart ) > 0 )
            pMember->m_pGrid->SendMessage( WM_VSCROLL, SB_LINEDOWN, 0 );
        down--;
        iRowStart++;
    }

    iRowStart = VisibleCells.GetMinRow() - 1;
    while( up > 0 )
    {
        if( GetRowHeight( iRowStart ) > 0 )
            pMember->m_pGrid->SendMessage( WM_VSCROLL, SB_LINEUP, 0 );
        up--;
        iRowStart--;
    }

    // Move one more if we only see a snall bit of the cell
    CRect rectCell;
    if (!GetCellRect(nRow, nCol, rectCell))
    {
		if (pFocusWnd && ::IsWindow(pFocusWnd->GetSafeHwnd()))
			pFocusWnd->SetFocus(); 
        return;
    }

    pMember->m_pGrid->GetClientRect(rectWindow);

    // The previous fix was fixed properly by Martin Richter 
    while (rectCell.right > rectWindow.right
        && rectCell.left > GetFixedColumnWidth()
        && IsVisibleHScroll() // Junlin Xu: added to prevent infinite loop
        )
    {
        pMember->m_pGrid->SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
        if (!GetCellRect(nRow, nCol, rectCell))
        {
            pFocusWnd->SetFocus(); 
            return;
        }
    }

    while (rectCell.bottom > rectWindow.bottom
        && rectCell.top > GetFixedRowHeight()
        && IsVisibleVScroll() // Junlin Xu: added to prevent infinite loop
        )
    {
        pMember->m_pGrid->SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
        if (!GetCellRect(nRow, nCol, rectCell))
        {
            pFocusWnd->SetFocus(); 
            return;
        }
    }

    // restore focus to whoever owned it
	if (pFocusWnd && ::IsWindow(pFocusWnd->GetSafeHwnd()))
	    pFocusWnd->SetFocus(); 
}

BOOL CExGridCtrlApp::IsCellEditable(CCellID &cell)
{
    return IsCellEditable(cell.row, cell.col);
}

BOOL CExGridCtrlApp::IsCellEditable(int nRow, int nCol)
{
	UINT nState = GetItemState(nRow, nCol);
    return IsEditable() && ((nState & GVIS_READONLY) != GVIS_READONLY);
}

BOOL CExGridCtrlApp::IsCellSelected(CCellID &cell)
{
    return IsCellSelected(cell.row, cell.col);
}

BOOL CExGridCtrlApp::IsCellSelected(int nRow, int nCol)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (GetVirtualMode())
    {   
        if (!IsSelectable())
            return FALSE;

        CCellID cell;
        DWORD key = MAKELONG(nRow, nCol);

        return (pMember->m_SelectedCellMap.Lookup(key, (CCellID&)cell));       
    }
    else
        return IsSelectable() && ((GetItemState(nRow, nCol) & GVIS_SELECTED) == GVIS_SELECTED);
}

BOOL CExGridCtrlApp::IsCellVisible(CCellID cell) 
{
    return IsCellVisible(cell.row, cell.col);
}

BOOL CExGridCtrlApp::IsCellVisible(int nRow, int nCol)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (!IsWindow(pMember->m_pGrid->m_hWnd))
        return FALSE;

    int x, y;

    CCellID TopLeft;
    if (nCol >= GetFixedColumnCount() || nRow >= GetFixedRowCount())
    {
        TopLeft = GetTopleftNonFixedCell();
        if (nCol >= GetFixedColumnCount() && nCol < TopLeft.col)
            return FALSE;
        if (nRow >= GetFixedRowCount() && nRow < TopLeft.row)
            return FALSE;
    }
    
    CRect rect;
    pMember->m_pGrid->GetClientRect(rect);
    if (nCol < GetFixedColumnCount())
    {
        x = 0;
        for (int i = 0; i <= nCol; i++) 
        {
            if (x >= rect.right)
                return FALSE;
            x += GetColumnWidth(i);    
        }
    } 
    else 
    {
        x = GetFixedColumnWidth();
        for (int i = TopLeft.col; i <= nCol; i++) 
        {
            if (x >= rect.right)
                return FALSE;
            x += GetColumnWidth(i);    
        }
    }
    
    if (nRow < GetFixedRowCount())
    {
        y = 0;
        for (int i = 0; i <= nRow; i++) 
        {
            if (y >= rect.bottom)
                return FALSE;
            y += GetRowHeight(i);    
        }
    } 
    else 
    {
        if (nRow < TopLeft.row)
            return FALSE;
        y = GetFixedRowHeight();
        for (int i = TopLeft.row; i <= nRow; i++) 
        {
            if (y >= rect.bottom)
                return FALSE;
            y += GetRowHeight(i);    
        }
    }
    
    return TRUE;
}

BOOL CExGridCtrlApp::InvalidateCellRect(const CCellID& cell)
{
    return InvalidateCellRect(cell.row, cell.col);
}

BOOL CExGridCtrlApp::InvalidateCellRect(const int row, const int col)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (!::IsWindow(pMember->m_pGrid->GetSafeHwnd()) || !pMember->m_bAllowDraw)
        return FALSE;

    if (!IsValid(row, col))
        return FALSE;

    if (!IsCellVisible(row, col))
        return FALSE;

    CRect rect;
    if (!GetCellRect(row, col, rect))
        return FALSE;
    rect.right++;
    rect.bottom++;
    pMember->m_pGrid->InvalidateRect(rect, TRUE);

    return TRUE;
}

BOOL CExGridCtrlApp::InvalidateCellRect(const CCellRange& cellRange)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    ASSERT(IsValid(cellRange));
    if (!::IsWindow(pMember->m_pGrid->GetSafeHwnd()) || !pMember->m_bAllowDraw)
        return FALSE;

    CCellRange visibleCellRange = GetVisibleNonFixedCellRange().Intersect(cellRange);

    CRect rect;
    if (!GetCellRangeRect(visibleCellRange, rect))
        return FALSE;

    rect.right++;
    rect.bottom++;
    pMember->m_pGrid->InvalidateRect(rect, TRUE);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CExGridCtrl Mouse stuff

// Handles mouse wheel notifications
// Note - if pMember->m_pGrid doesn't work for win95 then use OnRegisteredMouseWheel instead
#if !defined(_WIN32_WCE) && (_MFC_VER >= 0x0421)
BOOL CExGridCtrlApp::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    // A pMember->m_nRowsPerWheelNotch value less than 0 indicates that the mouse
    // wheel scrolls whole pages, not just lines.
    if (pMember->m_nRowsPerWheelNotch == -1)
    {
        int nPagesScrolled = zDelta / 120;

        if (nPagesScrolled > 0)
		{
            for (int i = 0; i < nPagesScrolled; i++)
			{
                pMember->m_pGrid->PostMessage(WM_VSCROLL, SB_PAGEUP, 0);
			}
		}
        else
		{
            for (int i = 0; i > nPagesScrolled; i--)
			{
                pMember->m_pGrid->PostMessage(WM_VSCROLL, SB_PAGEDOWN, 0);
			}
		}
    }
    else
    {
        int nRowsScrolled = pMember->m_nRowsPerWheelNotch * zDelta / 120;

        if (nRowsScrolled > 0)
		{
            for (int i = 0; i < nRowsScrolled; i++)
			{
                pMember->m_pGrid->PostMessage(WM_VSCROLL, SB_LINEUP, 0);
			}
		}
        else
		{
            for (int i = 0; i > nRowsScrolled; i--)
			{
                pMember->m_pGrid->PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);
			}
		}
    }

    return pMember->m_pGrid->CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
#endif // !defined(_WIN32_WCE) && (_MFC_VER >= 0x0421)

void CExGridCtrlApp::OnMouseMove(UINT /*nFlags*/, CPoint point)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    CRect rect;
    pMember->m_pGrid->GetClientRect(rect);

#ifndef GRIDCONTROL_NO_DRAGDROP
    // If outside client area, return (unless we are drag n dropping)
    if (pMember->m_MouseMode != MOUSE_DRAGGING && !rect.PtInRect(point))
        return;
#endif

    // Sometimes a MOUSEMOVE message can come after the left buttons
    // has been let go, but before the BUTTONUP message hs been processed.
    // We'll keep track of mouse buttons manually to avoid pMember->m_pGrid.
    // All bMouseButtonDown's have been replaced with the member pMember->m_bLMouseButtonDown
    // BOOL bMouseButtonDown = ((nFlags & MK_LBUTTON) == MK_LBUTTON);

    // If the left mouse button is up, then test to see if row/column sizing is imminent
    if (!pMember->m_bLMouseButtonDown ||
        (pMember->m_bLMouseButtonDown && pMember->m_MouseMode == MOUSE_NOTHING))
    {
        if (pMember->m_bAllowColumnResize && MouseOverColumnResizeArea(point))
        {
            if (pMember->m_MouseMode != MOUSE_OVER_COL_DIVIDE)
            {
#ifndef _WIN32_WCE_NO_CURSOR
                SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
#endif
                pMember->m_MouseMode = MOUSE_OVER_COL_DIVIDE;
            }
        }
        else if (pMember->m_bAllowRowResize && MouseOverRowResizeArea(point))
        {
            if (pMember->m_MouseMode != MOUSE_OVER_ROW_DIVIDE)
            {
#ifndef _WIN32_WCE_NO_CURSOR
                SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
#endif
                pMember->m_MouseMode = MOUSE_OVER_ROW_DIVIDE;
            }
        }
        else if (pMember->m_MouseMode != MOUSE_NOTHING)
        {
#ifndef _WIN32_WCE_NO_CURSOR
            SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
#endif
            pMember->m_MouseMode = MOUSE_NOTHING;
        }

        if (pMember->m_MouseMode == MOUSE_NOTHING)
        {
            CGridCellBase* pCell = NULL;
            CCellID idCurrentCell;
            if (!GetVirtualMode() || pMember->m_bTitleTips)
            {
                // Let the cell know that a big fat cursor is currently hovering
                // over it.
                idCurrentCell = GetCellFromPt(point);
                pCell = GetCell(idCurrentCell.row, idCurrentCell.col);
                if (pCell)
                    pCell->OnMouseOver();
            }

#ifndef GRIDCONTROL_NO_TITLETIPS
            // Titletips anyone? anyone?
            if (pMember->m_bTitleTips)
            {
                CRect TextRect, CellRect;
                if (pCell)
                {
                    LPCTSTR szTipText = pCell->GetTipText();
                    if (!pMember->m_bRMouseButtonDown
						&& szTipText && szTipText[0]
                        && !pCell->IsEditing()
                        && GetCellRect( idCurrentCell.row, idCurrentCell.col, &TextRect)
                        && pCell->GetTipTextRect( &TextRect)
                        && GetCellRect(idCurrentCell.row, idCurrentCell.col, CellRect) )
                    {
						TRACE0("Showing TitleTip\n");
						pMember->m_TitleTip.Show(TextRect, pCell->GetTipText(),  0, CellRect,
                                        pCell->GetFont(),  GetTitleTipTextClr(), GetTitleTipBackClr());
                    }
                }
            }
#endif
        }

        pMember->m_LastMousePoint = point;
        return;
    }

    if (!IsValid(pMember->m_LeftClickDownCell))
    {
        pMember->m_LastMousePoint = point;
        return;
    }

    // If the left mouse button is down, then process appropriately
    if (pMember->m_bLMouseButtonDown)
    {
        switch (pMember->m_MouseMode)
        {
        case MOUSE_SELECT_ALL:
            break;

        case MOUSE_SELECT_COL:
        case MOUSE_SELECT_ROW:
        case MOUSE_SELECT_CELLS:
            {
                CCellID idCurrentCell = GetCellFromPt(point);
                if (!IsValid(idCurrentCell))
                    return;

                if (idCurrentCell != GetFocusCell())
                {
                    OnSelecting(idCurrentCell);

                    // EFW - BUG FIX - Keep the appropriate cell row and/or
                    // column focused.  A fix in SetFocusCell() will place
                    // the cursor in a non-fixed cell as needed.
                    if((idCurrentCell.row >= pMember->m_nFixedRows &&
                      idCurrentCell.col >= pMember->m_nFixedCols) ||
                      pMember->m_MouseMode==MOUSE_SELECT_COL ||
                      pMember->m_MouseMode==MOUSE_SELECT_ROW)
                    {
                        SetFocusCell(idCurrentCell);
                    }
                }
                break;
            }

        case MOUSE_SIZING_COL:
            {
                CDC* pDC = pMember->m_pGrid->GetDC();
                if (!pDC)
                    break;

                CRect oldInvertedRect(pMember->m_LastMousePoint.x, rect.top,
                    pMember->m_LastMousePoint.x + 2, rect.bottom);
                pDC->InvertRect(&oldInvertedRect);
                CRect newInvertedRect(point.x, rect.top, 
                    point.x + 2, rect.bottom);
                pDC->InvertRect(&newInvertedRect);
                pMember->m_pGrid->ReleaseDC(pDC);
            }
            break;
            
        case MOUSE_SIZING_ROW:        
            {
                CDC* pDC = pMember->m_pGrid->GetDC();
                if (!pDC)
                    break;
                
                CRect oldInvertedRect(rect.left, pMember->m_LastMousePoint.y,
                    rect.right, pMember->m_LastMousePoint.y + 2);
                pDC->InvertRect(&oldInvertedRect);
                CRect newInvertedRect(rect.left, point.y, 
                    rect.right, point.y + 2);
                pDC->InvertRect(&newInvertedRect);
                pMember->m_pGrid->ReleaseDC(pDC);
            }
            break;
            
#ifndef GRIDCONTROL_NO_DRAGDROP
        case MOUSE_PREPARE_EDIT:
        case MOUSE_PREPARE_DRAG:
            pMember->m_MouseMode = MOUSE_PREPARE_DRAG;
            OnBeginDrag();    
            break;
#endif
        }    
    }

    pMember->m_LastMousePoint = point;
}

// Returns the point inside the cell that was clicked (coords relative to cell top left)
CPoint CExGridCtrlApp::GetPointClicked(int nRow, int nCol, const CPoint& point)
{
    CPoint PointCellOrigin;
    if( !GetCellOrigin( nRow, nCol, &PointCellOrigin)  )
        return CPoint( 0, 0);

    CPoint PointClickedCellRelative( point);
    PointClickedCellRelative -= PointCellOrigin;
    return PointClickedCellRelative;
}

void CExGridCtrlApp::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    TRACE0("CExGridCtrlApp::OnLButtonDblClk\n");

    CCellID cell = GetCellFromPt(point);
    if( !IsValid( cell) )
    {
        //ASSERT(FALSE);
        return;
    }

#ifdef _WIN32_WCE
    if (MouseOverColumnResizeArea(point))
#else
    if (pMember->m_MouseMode == MOUSE_OVER_COL_DIVIDE)
#endif
    {
        CPoint start;
        if (!GetCellOrigin(0, cell.col, &start))
            return;

        if (point.x - start.x < pMember->m_nResizeCaptureRange)     // Clicked right of border
            cell.col--;

        //  ignore columns that are hidden and look left towards first visible column
        BOOL bFoundVisible = FALSE;
        while( cell.col >= 0)
        {
            if( GetColumnWidth( cell.col) > 0)
            {
                bFoundVisible = TRUE;
                break;
            }
            cell.col--;
        }
        if( !bFoundVisible)
            return;

        AutoSizeColumn(cell.col, GetAutoSizeStyle());
        pMember->m_pGrid->Invalidate();
    }
#ifdef _WIN32_WCE
    else if (MouseOverRowResizeArea(point))
#else
    else if (pMember->m_MouseMode == MOUSE_OVER_ROW_DIVIDE)
#endif
    {
        CPoint start;
        if (!GetCellOrigin(0, cell.col, &start))
            return;

        if (point.y - start.y < pMember->m_nResizeCaptureRange)     // Clicked below border
            cell.row--;

        //  ignore rows that are hidden and look up towards first visible row
        BOOL bFoundVisible = FALSE;
        while( cell.row >= 0)
        {
            if( GetRowHeight( cell.row) > 0)
            {
                bFoundVisible = TRUE;
                break;
            }
            cell.row--;
        }
        if( !bFoundVisible)
            return;

        AutoSizeRow(cell.row);
        pMember->m_pGrid->Invalidate();
    }
    else if (pMember->m_MouseMode == MOUSE_NOTHING)
    {
        CPoint pointClickedRel;
        pointClickedRel = GetPointClicked( cell.row, cell.col, point);

        CGridCellBase* pCell = NULL;
        if (IsValid(cell))
            pCell = GetCell(cell.row, cell.col);

        // Clicked in the text area? Only then will cell selection work
        BOOL bInTextArea = FALSE;
        if (pCell)
        {
            CRect rectCell;
            if (GetCellRect(cell.row, cell.col, rectCell) && pCell->GetTextRect(rectCell))
                bInTextArea = rectCell.PtInRect(point);
        }

        if (cell.row >= pMember->m_nFixedRows && IsValid(pMember->m_LeftClickDownCell) && 
            cell.col >= pMember->m_nFixedCols && bInTextArea)
        {
            OnEditCell(cell.row, cell.col, pointClickedRel, VK_LBUTTON);
        }
        else if (pMember->m_bListMode)
        {
            if (!IsValid(cell))
                return;
            if (cell.row >= pMember->m_nFixedRows && cell.col >= pMember->m_nFixedCols && bInTextArea)
                OnEditCell(cell.row, cell.col, pointClickedRel, VK_LBUTTON);
        }

        if (IsValid(cell))
        {
            CGridCellBase* pCell = GetCell(cell.row, cell.col);
            if (pCell)
                pCell->OnDblClick(pointClickedRel);
            SendMessageToParent(cell.row, cell.col, NM_DBLCLK);
        }
    }

    pMember->m_pGrid->CWnd::OnLButtonDblClk(nFlags, point);
}

void CExGridCtrlApp::OnLButtonDown(UINT nFlags, CPoint point)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

#ifdef GRIDCONTROL_USE_TITLETIPS
    // EFW - Bug Fix
    pMember->m_TitleTip.Hide();  // hide any titletips
#endif

    // TRACE0("CExGridCtrlApp::OnLButtonDown\n");
    // CWnd::OnLButtonDown(nFlags, point);

    pMember->m_pGrid->SetFocus();

    pMember->m_bLMouseButtonDown   = TRUE;
    pMember->m_LeftClickDownPoint = point;
    pMember->m_LeftClickDownCell  = GetCellFromPt(point);
    if (!IsValid(pMember->m_LeftClickDownCell))
        return;

    // If the SHIFT key is not down, then the start of the selection area should be the 
    // cell just clicked. Otherwise, keep the previous selection-start-cell so the user
    // can add to their previous cell selections in an intuitive way. If no selection-
    // start-cell has been specified, then set it's value here and now.
    if ((nFlags & MK_SHIFT) != MK_SHIFT)
        pMember->m_SelectionStartCell = pMember->m_LeftClickDownCell;
    else
    {
        if (!IsValid(pMember->m_SelectionStartCell))
            pMember->m_SelectionStartCell = pMember->m_idCurrentCell;
    }

    EndEditing();

    // tell the cell about it 
    CGridCellBase* pCell = GetCell(pMember->m_LeftClickDownCell.row, pMember->m_LeftClickDownCell.col);
    if (pCell)
        pCell->OnClickDown(GetPointClicked( pMember->m_LeftClickDownCell.row, pMember->m_LeftClickDownCell.col, point));

    // Clicked in the text area? Only then will cell selection work
    BOOL bInTextArea = FALSE;
    if (pCell)
    {
        CRect rectCell;
        if (GetCellRect(pMember->m_LeftClickDownCell.row, pMember->m_LeftClickDownCell.col, rectCell) &&
            pCell->GetTextRect(rectCell))
        {
            bInTextArea = rectCell.PtInRect(point);
        }
    }

    // If the user clicks on the current cell, then prepare to edit it.
    // (If the user moves the mouse, then dragging occurs)
    if (pMember->m_LeftClickDownCell == pMember->m_idCurrentCell && 
        !(nFlags & MK_CONTROL) && bInTextArea &&
        IsCellEditable(pMember->m_LeftClickDownCell))
    {
        pMember->m_MouseMode = MOUSE_PREPARE_EDIT;
        return;
    }
    // If the user clicks on a selected cell, then prepare to drag it.
    // (If the user moves the mouse, then dragging occurs)
    else if (IsCellSelected(pMember->m_LeftClickDownCell))
    {
        SetFocusCell(pMember->m_LeftClickDownCell.row, pMember->m_LeftClickDownCell.col);

        // If control is pressed then unselect the cell or row (depending on the list mode)
        if (nFlags & MK_CONTROL)
        {
            SetFocusCell(pMember->m_LeftClickDownCell);
            if (GetListMode())
                SelectRows(pMember->m_LeftClickDownCell, TRUE, FALSE);
            else
                SelectCells(pMember->m_LeftClickDownCell, TRUE, FALSE);
            return;
        }
#ifndef GRIDCONTROL_NO_DRAGDROP
        else if (pMember->m_bAllowDragAndDrop)
            pMember->m_MouseMode = MOUSE_PREPARE_DRAG;
#endif
    }
    else if (pMember->m_MouseMode != MOUSE_OVER_COL_DIVIDE &&
             pMember->m_MouseMode != MOUSE_OVER_ROW_DIVIDE)        
    {
		if (pMember->m_LeftClickDownCell.row >= GetFixedRowCount() && 
			pMember->m_LeftClickDownCell.col >= GetFixedColumnCount())
		{
            SetFocusCell(pMember->m_LeftClickDownCell.row, pMember->m_LeftClickDownCell.col);
		}
		else
			SetFocusCell(-1, -1);

		//Don't set focus on any cell if the user clicked on a fixed cell - David Pritchard
        //if (GetRowCount() > GetFixedRowCount() && 
        //    GetColumnCount() > GetFixedColumnCount())
        //    SetFocusCell(max(pMember->m_LeftClickDownCell.row, pMember->m_nFixedRows),
        //                 max(pMember->m_LeftClickDownCell.col, pMember->m_nFixedCols));
    }
    
    pMember->m_pGrid->SetCapture();
    
    if (pMember->m_MouseMode == MOUSE_NOTHING)
    {
        if (pMember->m_bAllowColumnResize && MouseOverColumnResizeArea(point))
        {
            if (pMember->m_MouseMode != MOUSE_OVER_COL_DIVIDE)
            {
#ifndef _WIN32_WCE_NO_CURSOR
                SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
#endif
                pMember->m_MouseMode = MOUSE_OVER_COL_DIVIDE;
            }
        }
        else if (pMember->m_bAllowRowResize && MouseOverRowResizeArea(point))
        {
            if (pMember->m_MouseMode != MOUSE_OVER_ROW_DIVIDE)
            {
#ifndef _WIN32_WCE_NO_CURSOR
                SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
#endif
                pMember->m_MouseMode = MOUSE_OVER_ROW_DIVIDE;
            }
        }
        // else if (pMember->m_MouseMode != MOUSE_NOTHING)
        //{
        //    SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
        //    pMember->m_MouseMode = MOUSE_NOTHING;
        //}
    }
    
    if (pMember->m_MouseMode == MOUSE_OVER_COL_DIVIDE) // sizing column
    {
        pMember->m_MouseMode = MOUSE_SIZING_COL;

        // Kludge for if we are over the last column...
        if (GetColumnWidth(GetColumnCount()-1) < pMember->m_nResizeCaptureRange)
        {
            CRect VisRect;
            GetVisibleNonFixedCellRange(VisRect);
            if (abs(point.x - VisRect.right) < pMember->m_nResizeCaptureRange)
                pMember->m_LeftClickDownCell.col = GetColumnCount()-1;
        }

        CPoint start;
        if (!GetCellOrigin(0, pMember->m_LeftClickDownCell.col, &start))
            return;

        if( !pMember->m_bHiddenColUnhide)
        {
            //  ignore columns that are hidden and look left towards first visible column
            BOOL bLookForVisible = TRUE;
            BOOL bIsCellRightBorder = point.x - start.x >= pMember->m_nResizeCaptureRange;

            if( bIsCellRightBorder
                && pMember->m_LeftClickDownCell.col + 1 >= GetColumnCount() )
            {
                // clicked on last column's right border

                // if last column is visible, don't do anything
                if( pMember->m_LeftClickDownCell.col >= 0)
                    bLookForVisible = FALSE;
            }

            if( bLookForVisible)
            {
                // clicked on column divider other than last right border
                BOOL bFoundVisible = FALSE;
                int iOffset = 1;

                if( bIsCellRightBorder)
                    iOffset = 0;

                while( pMember->m_LeftClickDownCell.col - iOffset >= 0)
                {
                    if( GetColumnWidth( pMember->m_LeftClickDownCell.col - iOffset) > 0)
                    {
                        bFoundVisible = TRUE;
                        break;
                    }
                    pMember->m_LeftClickDownCell.col--;
                }
                if( !bFoundVisible)
                    return;
            }
        }


        CRect rect;
        pMember->m_pGrid->GetClientRect(rect);
        CRect invertedRect(point.x, rect.top, point.x + 2, rect.bottom);

        CDC* pDC = pMember->m_pGrid->GetDC();
        if (pDC)
        {
            pDC->InvertRect(&invertedRect);
            pMember->m_pGrid->ReleaseDC(pDC);
        }

        // If we clicked to the right of the colimn divide, then reset the click-down cell
        // as the cell to the left of the column divide - UNLESS we clicked on the last column
        // and the last column is teensy (kludge fix)
        if (point.x - start.x < pMember->m_nResizeCaptureRange)
        {
            if (pMember->m_LeftClickDownCell.col < GetColumnCount()-1 || 
                GetColumnWidth(GetColumnCount()-1) >= pMember->m_nResizeCaptureRange)
            {
                if (!GetCellOrigin(0, --pMember->m_LeftClickDownCell.col, &start))
                    return;
            }
        }

		// Allow a cell resize width no greater than that which can be viewed within
		// the grid itself
		int nMaxCellWidth = rect.Width()-GetFixedColumnWidth();
        rect.left  = start.x + 1;
		rect.right = rect.left + nMaxCellWidth;

        pMember->m_pGrid->ClientToScreen(rect);
#ifndef _WIN32_WCE_NO_CURSOR
        ClipCursor(rect);
#endif
    }
    else if (pMember->m_MouseMode == MOUSE_OVER_ROW_DIVIDE) // sizing row
    {
        pMember->m_MouseMode = MOUSE_SIZING_ROW;

        // Kludge for if we are over the last column...
        if (GetRowHeight(GetRowCount()-1) < pMember->m_nResizeCaptureRange)
        {
            CRect VisRect;
            GetVisibleNonFixedCellRange(VisRect);
            if (abs(point.y - VisRect.bottom) < pMember->m_nResizeCaptureRange)
                pMember->m_LeftClickDownCell.row = GetRowCount()-1;
        }

        CPoint start;
        if (!GetCellOrigin(pMember->m_LeftClickDownCell, &start))
            return;

        if( !pMember->m_bHiddenRowUnhide)
        {
            //  ignore rows that are hidden and look up towards first visible row
            BOOL bLookForVisible = TRUE;
            BOOL bIsCellBottomBorder = point.y - start.y >= pMember->m_nResizeCaptureRange;

            if( bIsCellBottomBorder
                && pMember->m_LeftClickDownCell.row + 1 >= GetRowCount() )
            {
                // clicked on last row's bottom border

                // if last row is visible, don't do anything
                if( pMember->m_LeftClickDownCell.row >= 0)
                    bLookForVisible = FALSE;
            }

            if( bLookForVisible)
            {
                // clicked on row divider other than last bottom border
                BOOL bFoundVisible = FALSE;
                int iOffset = 1;

                if( bIsCellBottomBorder)
                    iOffset = 0;

                while( pMember->m_LeftClickDownCell.row - iOffset >= 0)
                {
                    if( GetRowHeight( pMember->m_LeftClickDownCell.row - iOffset) > 0)
                    {
                        bFoundVisible = TRUE;
                        break;
                    }
                    pMember->m_LeftClickDownCell.row--;
                }
                if( !bFoundVisible)
                    return;
            }
        }

        CRect rect;
        pMember->m_pGrid->GetClientRect(rect);
        CRect invertedRect(rect.left, point.y, rect.right, point.y + 2);

        CDC* pDC = pMember->m_pGrid->GetDC();
        if (pDC)
        {
            pDC->InvertRect(&invertedRect);
            pMember->m_pGrid->ReleaseDC(pDC);
        }

        // If we clicked below the row divide, then reset the click-down cell
        // as the cell above the row divide - UNLESS we clicked on the last row
        // and the last row is teensy (kludge fix)
        if (point.y - start.y < pMember->m_nResizeCaptureRange)            // clicked below border
        {
            if (pMember->m_LeftClickDownCell.row < GetRowCount()-1 || 
                GetRowHeight(GetRowCount()-1) >= pMember->m_nResizeCaptureRange)
            {
                if (!GetCellOrigin(--pMember->m_LeftClickDownCell.row, 0, &start))
                    return;
            }
        }

		int nMaxCellHeight = rect.Height()-GetFixedRowHeight();
        rect.top = start.y + 1;
		rect.bottom = rect.top + nMaxCellHeight;

        pMember->m_pGrid->ClientToScreen(rect);

#ifndef _WIN32_WCE_NO_CURSOR
        ClipCursor(rect);
#endif
    }
    else
#ifndef GRIDCONTROL_NO_DRAGDROP
    if (pMember->m_MouseMode != MOUSE_PREPARE_DRAG) // not sizing or editing -- selecting
#endif
    {
	    SendMessageToParent(pMember->m_LeftClickDownCell.row, pMember->m_LeftClickDownCell.col, GVN_SELCHANGING);

        // If Ctrl pressed, save the current cell selection. pMember->m_pGrid will get added
        // to the new cell selection at the end of the cell selection process
        pMember->m_PrevSelectedCellMap.RemoveAll();
        if (nFlags & MK_CONTROL)
        {
            for (POSITION pos = pMember->m_SelectedCellMap.GetStartPosition(); pos != NULL; )
            {
                DWORD key;
                CCellID cell;
                pMember->m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);
                pMember->m_PrevSelectedCellMap.SetAt(key, cell);
            }
        }
        
        if (pMember->m_LeftClickDownCell.row < GetFixedRowCount())
            OnFixedRowClick(pMember->m_LeftClickDownCell);
        else if (pMember->m_LeftClickDownCell.col < GetFixedColumnCount())
            OnFixedColumnClick(pMember->m_LeftClickDownCell);
        else
        {
            pMember->m_MouseMode = pMember->m_bListMode? MOUSE_SELECT_ROW : MOUSE_SELECT_CELLS;
            OnSelecting(pMember->m_LeftClickDownCell);

            pMember->m_nTimerID = pMember->m_pGrid->SetTimer(WM_LBUTTONDOWN, pMember->m_nTimerInterval, 0);
        }
    }   
    pMember->m_LastMousePoint = point;
}

void CExGridCtrlApp::OnLButtonUp(UINT nFlags, CPoint point)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    // TRACE0("CExGridCtrlApp::OnLButtonUp\n");

    pMember->m_pGrid->CWnd::OnLButtonUp(nFlags, point);

    pMember->m_bLMouseButtonDown = FALSE;

#ifndef _WIN32_WCE_NO_CURSOR
    ClipCursor(NULL);
#endif

    if (pMember->m_pGrid->GetCapture()->GetSafeHwnd() == pMember->m_pGrid->GetSafeHwnd())
    {
        ReleaseCapture();
        pMember->m_pGrid->KillTimer(pMember->m_nTimerID);
        pMember->m_nTimerID = 0;
    }

    CPoint pointClickedRel;
    pointClickedRel = GetPointClicked( pMember->m_idCurrentCell.row, pMember->m_idCurrentCell.col, point);

    // pMember->m_MouseMode == MOUSE_PREPARE_EDIT only if user clicked down on current cell
    // and then didn't move mouse before clicking up (releasing button)
    if (pMember->m_MouseMode == MOUSE_PREPARE_EDIT)
    {
        OnEditCell(pMember->m_idCurrentCell.row, pMember->m_idCurrentCell.col, pointClickedRel, VK_LBUTTON);
    }
#ifndef GRIDCONTROL_NO_DRAGDROP
    // pMember->m_MouseMode == MOUSE_PREPARE_DRAG only if user clicked down on a selected cell
    // and then didn't move mouse before clicking up (releasing button)
    else if (pMember->m_MouseMode == MOUSE_PREPARE_DRAG) 
    {
        CGridCellBase* pCell = GetCell(pMember->m_idCurrentCell.row, pMember->m_idCurrentCell.col);
        if (pCell)
            pCell->OnClick( GetPointClicked( pMember->m_idCurrentCell.row, pMember->m_idCurrentCell.col, point) );
        SendMessageToParent(pMember->m_LeftClickDownCell.row, pMember->m_LeftClickDownCell.col, NM_CLICK);
	    SendMessageToParent(pMember->m_LeftClickDownCell.row, pMember->m_LeftClickDownCell.col, GVN_SELCHANGING);
        ResetSelectedRange();
	    SendMessageToParent(pMember->m_idCurrentCell.row, pMember->m_idCurrentCell.col, GVN_SELCHANGED);
    }
#endif
    else if (pMember->m_MouseMode == MOUSE_SIZING_COL)
    {
        CRect rect;
        pMember->m_pGrid->GetClientRect(rect);
        CRect invertedRect(pMember->m_LastMousePoint.x, rect.top, pMember->m_LastMousePoint.x + 2, rect.bottom);
        
        CDC* pDC = pMember->m_pGrid->GetDC();
        if (pDC)
        {
            pDC->InvertRect(&invertedRect);
            pMember->m_pGrid->ReleaseDC(pDC);
        }
        
        if (pMember->m_LeftClickDownPoint != point && (point.x != 0 || point.y != 0)) // 0 pt fix by email1@bierling.net
        {   
            CPoint start;
            if (!GetCellOrigin(pMember->m_LeftClickDownCell, &start))
                return;

            int nColumnWidth = max(point.x - start.x, pMember->m_bAllowColHide? 0 : 1);

            SetColumnWidth(pMember->m_LeftClickDownCell.col, nColumnWidth);
            ResetScrollBars();
            pMember->m_pGrid->Invalidate();
        }
    }
    else if (pMember->m_MouseMode == MOUSE_SIZING_ROW)
    {
        CRect rect;
        pMember->m_pGrid->GetClientRect(rect);
        CRect invertedRect(rect.left, pMember->m_LastMousePoint.y, rect.right, pMember->m_LastMousePoint.y + 2);

        CDC* pDC = pMember->m_pGrid->GetDC();
        if (pDC)
        {
            pDC->InvertRect(&invertedRect);
            pMember->m_pGrid->ReleaseDC(pDC);
        }
        
        if (pMember->m_LeftClickDownPoint != point  && (point.x != 0 || point.y != 0)) // 0 pt fix by email1@bierling.net
        {
            CPoint start;
            if (!GetCellOrigin(pMember->m_LeftClickDownCell, &start))
                return;
            
            int nRowHeight = max(point.y - start.y, pMember->m_bAllowRowHide? 0 : 1);

            SetRowHeight(pMember->m_LeftClickDownCell.row, nRowHeight);
            ResetScrollBars();
            pMember->m_pGrid->Invalidate();
        }
    }
    else
    {
	    SendMessageToParent(pMember->m_idCurrentCell.row, pMember->m_idCurrentCell.col, GVN_SELCHANGED);

        CGridCellBase* pCell = GetCell(pMember->m_idCurrentCell.row, pMember->m_idCurrentCell.col);
        if (pCell)
            pCell->OnClick( GetPointClicked( pMember->m_idCurrentCell.row, pMember->m_idCurrentCell.col, point) );
        SendMessageToParent(pMember->m_LeftClickDownCell.row, pMember->m_LeftClickDownCell.col, NM_CLICK);
    }
    
    pMember->m_MouseMode = MOUSE_NOTHING;
    
#ifndef _WIN32_WCE_NO_CURSOR
    SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
#endif
    
    if (!IsValid(pMember->m_LeftClickDownCell))
        return;
    
    CWnd *pOwner = pMember->m_pGrid->GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
        pOwner->PostMessage(WM_COMMAND, MAKELONG(pMember->m_pGrid->GetDlgCtrlID(), BN_CLICKED),
        (LPARAM) pMember->m_pGrid->GetSafeHwnd());
}

#ifndef _WIN32_WCE
void CExGridCtrlApp::OnRButtonDown(UINT nFlags, CPoint point)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    pMember->m_pGrid->CWnd::OnRButtonDown(nFlags, point);
	pMember->m_bRMouseButtonDown = TRUE;

#ifdef GRIDCONTROL_USE_TITLETIPS
	TRACE0("Hiding TitleTip\n");
    pMember->m_TitleTip.Hide();  // hide any titletips
#endif
}

// EFW - Added to forward right click to parent so that a context
// menu can be shown without deriving a new grid class.
void CExGridCtrlApp::OnRButtonUp(UINT nFlags, CPoint point)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    pMember->m_pGrid->CWnd::OnRButtonUp(nFlags, point);

	pMember->m_bRMouseButtonDown = FALSE;

    CCellID FocusCell;
    FocusCell = GetCellFromPt(point);

    EndEditing();        // Auto-destroy any InPlaceEdit's

    // If not a valid cell, pass -1 for row and column
    if(!IsValid(FocusCell))
        SendMessageToParent(-1, -1, NM_RCLICK);
    else
    {
        SetFocusCell(-1,-1);
        SetFocusCell(max(FocusCell.row, pMember->m_nFixedRows),
            max(FocusCell.col, pMember->m_nFixedCols));

        // tell the cell about it
        CGridCellBase* pCell = GetCell(FocusCell.row, FocusCell.col);
        if (pCell)
            pCell->OnRClick( GetPointClicked( FocusCell.row, FocusCell.col, point) );

        SendMessageToParent(FocusCell.row, FocusCell.col, NM_RCLICK);
    }
}
#endif

#ifndef _WIN32_WCE
/////////////////////////////////////////////////////////////////////////////
// CExGridCtrl persistance

BOOL CExGridCtrlApp::Save(LPCTSTR filename, TCHAR chSeparator/*=_T(',')*/)
{
    CStdioFile File;
    CFileException ex;
	CString strSeparator(chSeparator);

    if (!File.Open(filename, CFile::modeWrite | CFile::modeCreate| CFile::typeText, &ex))
    {
        ex.ReportError();
        return FALSE;
    }

    TRY
    {
        int nNumColumns = GetColumnCount();
        for (int i = 0; i < GetRowCount(); i++)
        {
            for (int j = 0; j < nNumColumns; j++)
            {
                File.WriteString(GetItemText(i,j));
                File.WriteString((j==(nNumColumns-1))? _T("\n"): strSeparator);
            }
        }

        File.Close();
    }

    CATCH (CFileException, e)
    {
        AfxMessageBox(_T("Unable to save grid list"));
        return FALSE;
    }
    END_CATCH

    return TRUE;
}

BOOL CExGridCtrlApp::Load(LPCTSTR filename, TCHAR chSeparator/*=_T(',')*/)
{
    if (GetVirtualMode())
        return FALSE;

    TCHAR *token, *end;
    TCHAR buffer[1024];
    CStdioFile File;
    CFileException ex;

    if (!File.Open(filename, CFile::modeRead | CFile::typeText))
    {
        ex.ReportError();
        return FALSE;
    }

    DeleteAllItems();

    TRY
    {
        // Read Header off file
        File.ReadString(buffer, 1024);

        // Get first token
        for (token=buffer, end=buffer;
             *end && (*end != chSeparator) && (*end != _T('\n')); 
             end++)
            ;

        if ((*end == _T('\0')) && (token == end))
            token = NULL;

        *end = _T('\0');

        while (token)
        {
            InsertColumn(token);

            // Get next token
            for (token=++end; *end && (*end != chSeparator) && (*end != _T('\n'));
               end++)
                ;

            if ((*end == _T('\0')) && (token == end))
                token = NULL;

            *end = _T('\0');
        }

        // Read in rest of data
        int nItem = 1;
        while (File.ReadString(buffer, 1024))
        {
            // Get first token
            for (token=buffer, end=buffer;
              *end && (*end != chSeparator) && (*end != _T('\n')); end++)
                ;

            if ((*end == _T('\0')) && (token == end))
                token = NULL;

            *end = _T('\0');

            int nSubItem = 0;
            while (token)
            {
                if (!nSubItem)
                    InsertRow(token);
                else
                    SetItemText(nItem, nSubItem, token);

                // Get next token
                for (token=++end; *end && (*end != chSeparator) && (*end != _T('\n'));
                  end++)
                    ;

                if ((*end == _T('\0')) && (token == end))
                    token = NULL;

                *end = _T('\0');

                nSubItem++;
            }
            nItem++;
        }

        AutoSizeColumns(GetAutoSizeStyle());
        File.Close();
    }

    CATCH (CFileException, e)
    {
        AfxMessageBox(_T("Unable to load grid data"));
        return FALSE;
    }
    END_CATCH

    return TRUE;
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CExGridCtrl overrideables

#ifndef GRIDCONTROL_NO_DRAGDROP
// pMember->m_pGrid is no longer needed since I've changed to OLE drag and drop - but it's
// still cool code. :)
CImageList* CExGridCtrlApp::CreateDragImage(CPoint *pHotSpot)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    CDC* pDC = pMember->m_pGrid->GetDC();
    if (!pDC)
        return NULL;

    CRect rect;
    CCellID cell = GetFocusCell();
    if (!GetCellRect(cell.row, cell.col, rect))
        return NULL;
    
    // Translate coordinate system
    rect.BottomRight() = CPoint(rect.Width(), rect.Height());
    rect.TopLeft()     = CPoint(0, 0);
    *pHotSpot = rect.BottomRight(); 
    
    // Create a new imagelist (the caller of pMember->m_pGrid function has responsibility
    // for deleting pMember->m_pGrid list)
    CImageList* pList = new CImageList;
    if (!pList || !pList->Create(rect.Width(), rect.Height(), ILC_MASK, 1, 1))
    {    
        if (pList)
            delete pList;
        return NULL;
    }
    
    // Create mem DC and bitmap
    CDC MemDC;
    CBitmap bm;
    MemDC.CreateCompatibleDC(pDC);
    bm.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
    CBitmap* pOldBitmap = MemDC.SelectObject(&bm);
    MemDC.SetWindowOrg(0, 0);
    
    // Draw cell onto bitmap in memDC
    CGridCellBase* pCell = GetCell(cell.row, cell.col);
    if (pCell)
        pCell->Draw(&MemDC, cell.row, cell.col, rect, FALSE);
    
    // Clean up
    MemDC.SelectObject(pOldBitmap);
    pMember->m_pGrid->ReleaseDC(pDC);
    
    // Add the bitmap we just drew to the image list.
    pList->Add(&bm, GetDefaultCell(FALSE, FALSE)->GetBackClr());
    bm.DeleteObject();

    return pList;
}
#endif

void CExGridCtrlApp::OnFixedRowClick(CCellID& cell)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (!IsValid(cell))
        return;

    if (GetHeaderSort())
    {
        CWaitCursor waiter;
        if (cell.col == GetSortColumn())
            SortItems(cell.col, !GetSortAscending());
        else
            SortItems(cell.col, TRUE);
        pMember->m_pGrid->Invalidate();
    }

	// Did the user click on a fixed column cell (so the cell was within the overlap of
	// fixed row and column cells) - (fix by David Pritchard)
    if (GetFixedColumnSelection())
    {
        if (cell.col < GetFixedColumnCount())
        {
            pMember->m_MouseMode = MOUSE_SELECT_ALL;
            OnSelecting(cell);
        }
        else 
        {
            pMember->m_MouseMode = MOUSE_SELECT_COL;
            OnSelecting(cell);
        }
    }
}

void CExGridCtrlApp::OnFixedColumnClick(CCellID& cell)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (!IsValid(cell))
        return;

//    if (pMember->m_bListMode && (GetItemState(cell.row, pMember->m_nFixedCols) & GVNI_SELECTED))
//    {
//        OnEditCell(cell.row, cell.col, VK_LBUTTON);
//        return;
//    }

	// Did the user click on a fixed row cell (so the cell was within the overlap of
	// fixed row and column cells) - (fix by David Pritchard)
    if (GetFixedRowSelection())
    {
        if (cell.row < GetFixedRowCount())
        {
            pMember->m_MouseMode = MOUSE_SELECT_ALL;
            OnSelecting(cell);
        }
        else
        {
            pMember->m_MouseMode = MOUSE_SELECT_ROW;
            OnSelecting(cell);
        }
    }
}

// Gets the extent of the text pointed to by str (no CDC needed)
// By default pMember->m_pGrid uses the selected font (which is a bigger font)
CSize CExGridCtrlApp::GetTextExtent(int nRow, int nCol, LPCTSTR str)
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    if (!pCell)
        return CSize(0, 0);
    else
        return pCell->GetTextExtent(str);
}

// virtual
void CExGridCtrlApp::OnEditCell(int nRow, int nCol, CPoint point, UINT nChar)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

#ifndef GRIDCONTROL_NO_TITLETIPS
    pMember->m_TitleTip.Hide();  // hide any titletips
#endif

    // Can we do it?
    CCellID cell(nRow, nCol);
    if (!IsValid(cell) || !IsCellEditable(nRow, nCol))
        return;

    // Can we see what we are doing?
    EnsureVisible(nRow, nCol);
    if (!IsCellVisible(nRow, nCol))
        return;

    // Where, exactly, are we gonna do it??
    CRect rect;
    if (!GetCellRect(cell, rect))
        return;

    // Check we can edit...
    if (SendMessageToParent(nRow, nCol, GVN_BEGINLABELEDIT) >= 0)
    {
        // Let's do it...
        CGridCellBase* pCell = GetCell(nRow, nCol);
        if (pCell)
            pCell->Edit(nRow, nCol, rect, point, IDC_INPLACE_CONTROL, nChar);
    }
}

// virtual
void CExGridCtrlApp::EndEditing()
{
    CCellID cell = GetFocusCell();
    if (!IsValid(cell)) return;
    CGridCellBase *pCell = GetCell(cell.row, cell.col);
    if (pCell)
        pCell->EndEdit();
}

// virtual
void CExGridCtrlApp::OnEndEditCell(int nRow, int nCol, CString str)
{
    CString strCurrentText = GetItemText(nRow, nCol);
    if (strCurrentText != str)
    {
        SetItemText(nRow, nCol, str);
        if (ValidateEdit(nRow, nCol, str) && 
            SendMessageToParent(nRow, nCol, GVN_ENDLABELEDIT) >= 0)
        {
            SetModified(TRUE, nRow, nCol);
            RedrawCell(nRow, nCol);
        }
        else
        {
            SetItemText(nRow, nCol, strCurrentText);
        }
    }

    CGridCellBase* pCell = GetCell(nRow, nCol);
    if (pCell)
        pCell->OnEndEdit();
}

// If pMember->m_pGrid returns FALSE then the editing isn't allowed
// virtual
BOOL CExGridCtrlApp::ValidateEdit(int nRow, int nCol, LPCTSTR str)
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return TRUE;

    return pCell->ValidateEdit(str);
}

// virtual
CString CExGridCtrlApp::GetItemText(int nRow, int nCol)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (nRow < 0 || nRow >= pMember->m_nRows || nCol < 0 || nCol >= pMember->m_nCols)
        return _T("");

    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return _T("");

    return pCell->GetText();
}

// Returns the default cell implementation for the given grid region
CGridCellBase* CExGridCtrlApp::GetDefaultCell(BOOL bFixedRow, BOOL bFixedCol)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
 
    if (bFixedRow && bFixedCol) return (CGridCellBase*) &pMember->m_cellFixedRowColDef;
    if (bFixedRow)              return (CGridCellBase*) &pMember->m_cellFixedRowDef;
    if (bFixedCol)              return (CGridCellBase*) &pMember->m_cellFixedColDef;
    return (CGridCellBase*) &pMember->m_cellDefault;
}

CGridCellBase* CExGridCtrlApp::GetCell(int nRow, int nCol)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
 
    if (nRow < 0 || nRow >= pMember->m_nRows || nCol < 0 || nCol >= pMember->m_nCols) 
        return NULL;

    if (GetVirtualMode())
    {
        CGridCellBase* pCell = GetDefaultCell(nRow < pMember->m_nFixedRows, nCol < pMember->m_nFixedCols);
        static GV_DISPINFO gvdi;
        gvdi.item.row     = nRow;
        gvdi.item.col     = nCol;
        gvdi.item.mask    = 0xFFFFFFFF;
        gvdi.item.nState  = 0;
        gvdi.item.nFormat = pCell->GetFormat();
        gvdi.item.iImage  = pCell->GetImage();
        gvdi.item.crBkClr = pCell->GetBackClr();
        gvdi.item.crFgClr = pCell->GetTextClr();
        gvdi.item.lParam  = pCell->GetData();
        memcpy(&gvdi.item.lfFont, pCell->GetFont(), sizeof(LOGFONT));
        gvdi.item.nMargin = pCell->GetMargin();
        gvdi.item.strText.Empty();

        // Fix the state bits
        if (IsCellSelected(nRow, nCol))   gvdi.item.nState |= GVIS_SELECTED;
        if (nRow < GetFixedRowCount())    gvdi.item.nState |= (GVIS_FIXED | GVIS_FIXEDROW);
        if (nCol < GetFixedColumnCount()) gvdi.item.nState |= (GVIS_FIXED | GVIS_FIXEDCOL);
        if (GetFocusCell() == CCellID(nRow, nCol)) gvdi.item.nState |= GVIS_FOCUSED;
        
        if (pMember->m_pfnCallback)
            pMember->m_pfnCallback(&gvdi, pMember->m_lParam);
        else
            SendDisplayRequestToParent(&gvdi);

        static CGridCell cell;
        cell.SetState(gvdi.item.nState);
        cell.SetFormat(gvdi.item.nFormat);
        cell.SetImage(gvdi.item.iImage);
        cell.SetBackClr(gvdi.item.crBkClr);
        cell.SetTextClr(gvdi.item.crFgClr);
        cell.SetData(gvdi.item.lParam);
        cell.SetFont(&(gvdi.item.lfFont));
        cell.SetMargin(gvdi.item.nMargin);
        cell.SetText(gvdi.item.strText);
        cell.SetGrid((CExGridCtrl*)pMember->m_pGrid);

        return (CGridCellBase*) &cell;
    }

    GRID_ROW* pRow = pMember->m_RowData[nRow];
    if (!pRow) return NULL;
    return pRow->GetAt(nCol);
}

BOOL CExGridCtrlApp::SetCell(int nRow, int nCol, CGridCellBase* pCell)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	
    if (GetVirtualMode())
        return FALSE;

    if (nRow < 0 || nRow >= pMember->m_nRows || nCol < 0 || nCol >= pMember->m_nCols) 
        return FALSE;

    GRID_ROW* pRow = pMember->m_RowData[nRow];
    if (!pRow) return FALSE;

    pCell->SetCoords( nRow, nCol); 
    pRow->SetAt(nCol, pCell);

    return TRUE;
}

COLORREF CExGridCtrlApp::GetFixedBkColor()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_cellFixedRowDef.GetBackClr();
}

// recursive sort implementation
BOOL CExGridCtrlApp::SortTextItems(int nCol, BOOL bAscending, int low, int high)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

    if (nCol >= GetColumnCount())
        return FALSE;

    if (high == -1)
        high = GetRowCount() - 1;

    int lo = low;
    int hi = high;

    if (hi <= lo)
        return FALSE;
    
    CString midItem = GetItemText((lo + hi)/2, nCol);
    
    // loop through the list until indices cross
    while (lo <= hi)
    {
        // Find the first element that is greater than or equal to the partition 
        // element starting from the left Index.
        if (bAscending)
            while (lo < high  && GetItemText(lo, nCol) < midItem)
                ++lo;
            else
                while (lo < high && GetItemText(lo, nCol) > midItem)
                    ++lo;
                
                // Find an element that is smaller than or equal to  the partition 
                // element starting from the right Index.
                if (bAscending)
                    while (hi > low && GetItemText(hi, nCol) > midItem)
                        --hi;
                    else
                        while (hi > low && GetItemText(hi, nCol) < midItem)
                            --hi;
                        
                        // If the indexes have not crossed, swap if the items are not equal
                        if (lo <= hi)
                        {
                            // swap only if the items are not equal
                            if (GetItemText(lo, nCol) != GetItemText(hi, nCol))
                            {
                                for (int col = 0; col < GetColumnCount(); col++)
                                {
                                    CGridCellBase *pCell = GetCell(lo, col);
                                    SetCell(lo, col, GetCell(hi, col));
                                    SetCell(hi, col, pCell);
                                }
                                UINT nRowHeight = pMember->m_arRowHeights[lo];
                                pMember->m_arRowHeights[lo] = pMember->m_arRowHeights[hi];
                                pMember->m_arRowHeights[hi] = nRowHeight;
                            }
                            
                            ++lo;
                            --hi;
                        }
    }
    
    // If the right index has not reached the left side of array
    // must now sort the left partition.
    if (low < hi)
        SortTextItems(nCol, bAscending, low, hi);
    
    // If the left index has not reached the right side of array
    // must now sort the right partition.
    if (lo < high)
        SortTextItems(nCol, bAscending, lo, high);
    
    return TRUE;
}

void CExGridCtrlApp::SetUseFocusRect(BOOL bUse)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bUseFocusRect = bUse;
}

BOOL CExGridCtrlApp::GetUseFocusRect()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->m_bUseFocusRect;
}

void CExGridCtrlApp::SetDrawFixedLine(BOOL bDraw)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bDrawFixedLine = bDraw;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CExGridCtrlApp object

CExGridCtrlApp theApp;

LPVOID WINAPI Ex_InitGridCtrl(LPVOID lpPointer)
{
	return theApp.InitGridCtrl(lpPointer);
}

LPVOID WINAPI Ex_InitGridCtrl2(LPVOID lpPointer, BOOL bUseDefSkin)
{
	return theApp.InitGridCtrl(lpPointer, bUseDefSkin);
}

void WINAPI Ex_ExitGridCtrl(LPVOID lpPointer)
{
	theApp.ExitGridCtrl(lpPointer);
}

long WINAPI Ex_ActionControl(LPVOID lpPointer, LPVOID lpAction)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.ActionControl(lpAction);
}

BOOL WINAPI Ex_Create(LPVOID lpPointer, const RECT& rect, CWnd* parent, UINT nID, DWORD dwStyle)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	BOOL bReturn = theApp.Create(rect, parent, nID, dwStyle);
	return bReturn;

}

int WINAPI Ex_GetRowCount(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetRowCount();
}

int WINAPI Ex_GetColumnCount(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetColumnCount();
}

int WINAPI Ex_GetFixedRowCount(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetFixedRowCount();
}

int WINAPI Ex_GetFixedColumnCount(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetFixedColumnCount();
}

BOOL WINAPI Ex_SetRowCount(LPVOID lpPointer, int nRows)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SetRowCount(nRows);
}

BOOL WINAPI Ex_SetColumnCount(LPVOID lpPointer, int nCols)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SetColumnCount(nCols);
}

BOOL WINAPI Ex_SetFixedRowCount(LPVOID lpPointer, int nFixedRows)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SetFixedRowCount(nFixedRows);
}

BOOL WINAPI Ex_SetFixedColumnCount(LPVOID lpPointer, int nFixedCols)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SetFixedColumnCount(nFixedCols);
}

int WINAPI Ex_GetRowHeight(LPVOID lpPointer, int nRow)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetRowHeight(nRow);
}

BOOL WINAPI Ex_SetRowHeight(LPVOID lpPointer, int row, int height)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SetRowHeight(row, height);
}

int WINAPI Ex_GetColumnWidth(LPVOID lpPointer, int nCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetColumnWidth(nCol);
}

BOOL WINAPI Ex_SetColumnWidth(LPVOID lpPointer, int col, int width)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SetColumnWidth(col, width);
}

BOOL WINAPI Ex_GetCellOrigin(LPVOID lpPointer, int nRow, int nCol, LPPOINT p)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetCellOrigin(nRow, nCol, p);
}

BOOL WINAPI Ex_GetCellOrigin2(LPVOID lpPointer, const CCellID& cell, LPPOINT p)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetCellOrigin(cell, p);
}

BOOL WINAPI Ex_GetCellRect(LPVOID lpPointer, int nRow, int nCol, LPRECT pRect)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetCellRect(nRow, nCol, pRect);
}

BOOL WINAPI Ex_GetCellRect2(LPVOID lpPointer, const CCellID& cell, LPRECT pRect)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetCellRect(cell, pRect);
}

BOOL WINAPI Ex_GetTextRect(LPVOID lpPointer, const CCellID& cell, LPRECT pRect)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetTextRect(cell, pRect);
}

BOOL WINAPI Ex_GetTextRect2(LPVOID lpPointer, int nRow, int nCol, LPRECT pRect)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetTextRect(nRow, nCol, pRect);
}

CCellID WINAPI Ex_GetCellFromPt(LPVOID lpPointer, CPoint point, BOOL bAllowFixedCellCheck)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetCellFromPt(point, bAllowFixedCellCheck);
}

int WINAPI Ex_GetFixedRowHeight(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetFixedRowHeight();
}

int WINAPI Ex_GetFixedColumnWidth(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetFixedColumnWidth();
}

long WINAPI Ex_GetVirtualWidth(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetVirtualWidth();
}

long WINAPI Ex_GetVirtualHeight(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetVirtualHeight();
}

CSize WINAPI Ex_GetTextExtent(LPVOID lpPointer, int nRow, int nCol, LPCTSTR str)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetTextExtent(nRow, nCol, str);
}

CSize WINAPI Ex_GetCellTextExtent(LPVOID lpPointer, int nRow, int nCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetCellTextExtent(nRow, nCol);
}

void WINAPI Ex_SetGridBkColor(LPVOID lpPointer, COLORREF clr)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetGridBkColor(clr);
}

COLORREF WINAPI Ex_GetGridBkColor(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetGridBkColor();
}

void WINAPI Ex_SetGridLineColor(LPVOID lpPointer, COLORREF clr)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetGridLineColor(clr);
}

COLORREF WINAPI Ex_GetGridLineColor(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetGridLineColor();
}

void WINAPI Ex_SetTitleTipBackClr(LPVOID lpPointer, COLORREF clr)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetTitleTipBackClr(clr);
}

COLORREF WINAPI Ex_GetTitleTipBackClr(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetTitleTipBackClr();
}

void WINAPI Ex_SetTitleTipTextClr(LPVOID lpPointer, COLORREF clr)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetTitleTipTextClr(clr);
}

COLORREF WINAPI Ex_GetTitleTipTextClr(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetTitleTipTextClr();
}

void WINAPI Ex_SetFixedGridLineColor_Light(LPVOID lpPointer, COLORREF clr)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetFixedGridLineColor_Light(clr);
}

void WINAPI Ex_SetFixedGridLineColor_Dark(LPVOID lpPointer, COLORREF clr)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetFixedGridLineColor_Dark(clr);
}

COLORREF WINAPI Ex_GetFixedGridLineColor_Light(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetFixedGridLineColor_Light();
}

COLORREF WINAPI Ex_GetFixedGridLineColor_Dark(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetFixedGridLineColor_Dark();
}

void WINAPI Ex_SetHighlightColor(LPVOID lpPointer, COLORREF clr)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetHighlightColor(clr);
}

void WINAPI Ex_SetHighlightTextColor(LPVOID lpPointer, COLORREF clr)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetHighlightTextColor(clr);
}

COLORREF WINAPI Ex_GetHighlightColor(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetHighlightColor();
}

COLORREF WINAPI Ex_GetHighlightTextColor(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetHighlightTextColor();
}

void WINAPI Ex_SetUseHighlightText(LPVOID lpPointer, BOOL bUse)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetUseHighlightText(bUse);
}

BOOL WINAPI Ex_GetUseHighlightText(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetUseHighlightText();
}

void WINAPI Ex_SetFixedRowSelectionHighlight(LPVOID lpPointer, BOOL bUse)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetFixedRowSelectionHighlight(bUse);
}

BOOL WINAPI Ex_GetFixedRowSelectionHighlight(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetFixedRowSelectionHighlight();
}

void WINAPI Ex_SetTextColor(LPVOID lpPointer, COLORREF clr)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetTextColor(clr);
}

COLORREF WINAPI Ex_GetTextColor(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetTextColor();
}

void WINAPI Ex_SetTextBkColor(LPVOID lpPointer, COLORREF clr)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetTextBkColor(clr);
}

COLORREF WINAPI Ex_GetTextBkColor(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetTextBkColor();
}

void WINAPI Ex_SetFixedTextColor(LPVOID lpPointer, COLORREF clr)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetFixedTextColor(clr);
}

COLORREF WINAPI Ex_GetFixedTextColor(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetFixedTextColor();
}

void WINAPI Ex_SetFixedBkColor(LPVOID lpPointer, COLORREF clr)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetFixedBkColor(clr);
}

COLORREF WINAPI Ex_GetFixedBkColor(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetFixedBkColor();
}

void WINAPI Ex_SetGridColor(LPVOID lpPointer, COLORREF clr)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetGridColor(clr);
}

COLORREF WINAPI Ex_GetGridColor(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetGridColor();
}

void WINAPI Ex_SetBkColor(LPVOID lpPointer, COLORREF clr)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetBkColor(clr);
}

COLORREF WINAPI Ex_GetBkColor(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetBkColor();
}

void WINAPI Ex_SetDefCellMargin(LPVOID lpPointer, int nMargin)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetDefCellMargin(nMargin);
}

int WINAPI Ex_GetDefCellMargin(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetDefCellMargin();
}

int WINAPI Ex_GetDefCellHeight(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetDefCellHeight();
}

void WINAPI Ex_SetDefCellHeight(LPVOID lpPointer, int nHeight)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetDefCellHeight(nHeight);
}

int WINAPI Ex_GetDefCellWidth(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetDefCellWidth();
}

void WINAPI Ex_SetDefCellWidth(LPVOID lpPointer, int nWidth)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetDefCellWidth(nWidth);
}

int WINAPI Ex_GetSelectedCount(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetSelectedCount();
}

CCellID WINAPI Ex_SetFocusCell1(LPVOID lpPointer, CCellID cell)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SetFocusCell(cell);
}

CCellID WINAPI Ex_SetFocusCell2(LPVOID lpPointer, int nRow, int nCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SetFocusCell(nRow, nCol);
}

CCellID WINAPI Ex_GetFocusCell(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetFocusCell();
}

void WINAPI Ex_SetVirtualMode(LPVOID lpPointer, BOOL bVirtual)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetVirtualMode(bVirtual);
}

BOOL WINAPI Ex_GetVirtualMode(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetVirtualMode();
}

void WINAPI Ex_SetCallbackFunc(LPVOID lpPointer, GRIDCALLBACK pCallback, LPARAM lParam)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetCallbackFunc(pCallback, lParam);
}

GRIDCALLBACK WINAPI Ex_GetCallbackFunc(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetCallbackFunc();
}

void WINAPI Ex_SetImageList(LPVOID lpPointer, CImageList* pList)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetImageList(pList);
}

CImageList* WINAPI Ex_GetImageList(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetImageList();
}

void WINAPI Ex_SetGridLines(LPVOID lpPointer, int nWhichLines)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetGridLines(nWhichLines);
}

int WINAPI Ex_GetGridLines(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetGridLines();
}

void WINAPI Ex_SetEditable(LPVOID lpPointer, BOOL bEditable)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetEditable(bEditable);
}

BOOL WINAPI Ex_IsEditable(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.IsEditable();
}

void WINAPI Ex_SetListMode(LPVOID lpPointer, BOOL bEnableListMode)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetListMode(bEnableListMode);
}

BOOL WINAPI Ex_GetListMode(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetListMode();
}

void WINAPI Ex_SetSingleRowSelection(LPVOID lpPointer, BOOL bSing)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetSingleRowSelection(bSing);
}

BOOL WINAPI Ex_GetSingleRowSelection(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetSingleRowSelection();
}

void WINAPI Ex_SetSingleColSelection(LPVOID lpPointer, BOOL bSing)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetSingleColSelection(bSing);
}

BOOL WINAPI Ex_GetSingleColSelection(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetSingleColSelection();
}

void WINAPI Ex_EnableSelection(LPVOID lpPointer, BOOL bEnable)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.EnableSelection(bEnable);
}

BOOL WINAPI Ex_IsSelectable(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.IsSelectable();
}

void WINAPI Ex_SetFixedColumnSelection(LPVOID lpPointer, BOOL bSelect)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetFixedColumnSelection(bSelect);
}

BOOL WINAPI Ex_GetFixedColumnSelection(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetFixedColumnSelection();
}

void WINAPI Ex_SetFixedRowSelection(LPVOID lpPointer, BOOL bSelect)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetFixedRowSelection(bSelect);
}

BOOL WINAPI Ex_GetFixedRowSelection(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetFixedRowSelection();
}

void WINAPI Ex_EnableDragAndDrop(LPVOID lpPointer, BOOL bAllow)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.EnableDragAndDrop(bAllow);
}

BOOL WINAPI Ex_GetDragAndDrop(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetDragAndDrop();
}

void WINAPI Ex_SetRowResize(LPVOID lpPointer, BOOL bResize)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetRowResize(bResize);
}

BOOL WINAPI Ex_GetRowResize(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetRowResize();
}

void WINAPI Ex_SetColumnResize(LPVOID lpPointer, BOOL bResize)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetColumnResize(bResize);
}

BOOL WINAPI Ex_GetColumnResize(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetColumnResize();
}

void WINAPI Ex_SetHeaderSort(LPVOID lpPointer, BOOL bSortOnClick)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetHeaderSort(bSortOnClick);
}

BOOL WINAPI Ex_GetHeaderSort(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetHeaderSort();
}

void WINAPI Ex_SetHandleTabKey(LPVOID lpPointer, BOOL bHandleTab)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetHandleTabKey(bHandleTab);
}

BOOL WINAPI Ex_GetHandleTabKey(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetHandleTabKey();
}

void WINAPI Ex_SetDoubleBuffering(LPVOID lpPointer, BOOL bBuffer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetDoubleBuffering(bBuffer);
}

BOOL WINAPI Ex_GetDoubleBuffering(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetDoubleBuffering();
}

void WINAPI Ex_EnableTitleTips(LPVOID lpPointer, BOOL bEnable)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.EnableTitleTips(bEnable);
}

BOOL WINAPI Ex_GetTitleTips(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetTitleTips();
}

void WINAPI Ex_SetSortColumn(LPVOID lpPointer, int nCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetSortColumn(nCol);
}

int WINAPI Ex_GetSortColumn(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetSortColumn();
}

void WINAPI Ex_SetSortAscending(LPVOID lpPointer, BOOL bAscending)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetSortAscending(bAscending);
}

BOOL WINAPI Ex_GetSortAscending(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetSortAscending();
}

void WINAPI Ex_SetTrackFocusCell(LPVOID lpPointer, BOOL bTrack)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetTrackFocusCell(bTrack);
}

BOOL WINAPI Ex_GetTrackFocusCell(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetTrackFocusCell();
}

void WINAPI Ex_SetFrameFocusCell(LPVOID lpPointer, BOOL bFrame)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetFrameFocusCell(bFrame);
}

BOOL WINAPI Ex_GetFrameFocusCell(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetFrameFocusCell();
}

void WINAPI Ex_SetAutoSizeStyle(LPVOID lpPointer, int nStyle)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetAutoSizeStyle(nStyle);
}

int WINAPI Ex_GetAutoSizeStyle(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetAutoSizeStyle();
}

void WINAPI Ex_EnableHiddenColUnhide(LPVOID lpPointer, BOOL bEnable)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.EnableHiddenColUnhide(bEnable);
}

BOOL WINAPI Ex_GetHiddenColUnhide(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetHiddenColUnhide();
}

void WINAPI Ex_EnableHiddenRowUnhide(LPVOID lpPointer, BOOL bEnable)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.EnableHiddenRowUnhide(bEnable);
}

BOOL WINAPI Ex_GetHiddenRowUnhide(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetHiddenRowUnhide();
}

void WINAPI Ex_EnableColumnHide(LPVOID lpPointer, BOOL bEnable)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.EnableColumnHide(bEnable);
}

BOOL WINAPI Ex_GetColumnHide(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetColumnHide();
}

void WINAPI Ex_EnableRowHide(LPVOID lpPointer, BOOL bEnable)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.EnableRowHide(bEnable);
}

BOOL WINAPI Ex_GetRowHide(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetRowHide();
}

void WINAPI Ex_SetDefaultReadOnly(LPVOID lpPointer, BOOL bReadOnly)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetDefaultReadOnly(bReadOnly);
}

BOOL WINAPI Ex_IsDefaultReadOnly(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.IsDefaultReadOnly();
}

CGridCellBase* WINAPI Ex_GetDefaultCell(LPVOID lpPointer, BOOL bFixedRow, BOOL bFixedCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetDefaultCell(bFixedRow, bFixedCol);
}

CGridCellBase* WINAPI Ex_GetCell(LPVOID lpPointer, int nRow, int nCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetCell(nRow, nCol);
}

void WINAPI Ex_SetModified(LPVOID lpPointer, BOOL bModified, int nRow, int nCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetModified(bModified, nRow, nCol);
}

BOOL WINAPI Ex_GetModified(LPVOID lpPointer, int nRow, int nCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetModified(nRow, nCol);
}

BOOL WINAPI Ex_IsCellFixed(LPVOID lpPointer, int nRow, int nCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.IsCellFixed(nRow, nCol);
}

BOOL WINAPI Ex_SetItem(LPVOID lpPointer, const GV_ITEM* pItem)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SetItem(pItem);
}

BOOL WINAPI Ex_GetItem(LPVOID lpPointer, GV_ITEM* pItem)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetItem(pItem);
}

BOOL WINAPI Ex_SetItemText(LPVOID lpPointer, int nRow, int nCol, LPCTSTR str)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SetItemText(nRow, nCol, str);
}

CString WINAPI Ex_GetItemText(LPVOID lpPointer, int nRow, int nCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetItemText(nRow, nCol);
}

BOOL WINAPI Ex_SetItemTextFmt(LPVOID lpPointer, int nRow, int nCol, LPCTSTR szFmt, ...)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

//	return theApp.SetItemTextFmt(nRow, nCol, szFmt, ...);
	return FALSE;
}

BOOL WINAPI Ex_SetItemTextFmtID(LPVOID lpPointer, int nRow, int nCol, UINT nID, ...)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

//	return theApp.SetItemTextFmtID(nRow, nCol, nID, ...);
	return FALSE;
}

BOOL WINAPI Ex_SetItemData(LPVOID lpPointer, int nRow, int nCol, LPARAM lParam)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SetItemData(nRow, nCol, lParam);
}

LPARAM WINAPI Ex_GetItemData(LPVOID lpPointer, int nRow, int nCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetItemData(nRow, nCol);
}

BOOL WINAPI Ex_SetItemImage(LPVOID lpPointer, int nRow, int nCol, int iImage)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SetItemImage(nRow, nCol, iImage);
}

int WINAPI Ex_GetItemImage(LPVOID lpPointer, int nRow, int nCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetItemImage(nRow, nCol);
}

BOOL WINAPI Ex_SetItemState(LPVOID lpPointer, int nRow, int nCol, UINT state)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SetItemState(nRow, nCol, state);
}

UINT WINAPI Ex_GetItemState(LPVOID lpPointer, int nRow, int nCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetItemState(nRow, nCol);
}

BOOL WINAPI Ex_SetItemFormat(LPVOID lpPointer, int nRow, int nCol, UINT nFormat)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SetItemFormat(nRow, nCol, nFormat);
}

UINT WINAPI Ex_GetItemFormat(LPVOID lpPointer, int nRow, int nCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetItemFormat(nRow, nCol);
}

BOOL WINAPI Ex_SetItemBkColour(LPVOID lpPointer, int nRow, int nCol, COLORREF cr)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SetItemBkColour(nRow, nCol, cr);
}

COLORREF WINAPI Ex_GetItemBkColour(LPVOID lpPointer, int nRow, int nCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetItemBkColour(nRow, nCol);
}

BOOL WINAPI Ex_SetItemFgColour(LPVOID lpPointer, int nRow, int nCol, COLORREF cr)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SetItemFgColour(nRow, nCol, cr);
}

COLORREF WINAPI Ex_GetItemFgColour(LPVOID lpPointer, int nRow, int nCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetItemFgColour(nRow, nCol);
}

BOOL WINAPI Ex_SetItemFont(LPVOID lpPointer, int nRow, int nCol, const LOGFONT* lf)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SetItemFont(nRow, nCol, lf);
}

LOGFONT* WINAPI Ex_GetItemFont(LPVOID lpPointer, int nRow, int nCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetItemFont(nRow, nCol);
}

BOOL WINAPI Ex_IsItemEditing(LPVOID lpPointer, int nRow, int nCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.IsItemEditing(nRow, nCol);
}

BOOL WINAPI Ex_SetCellType(LPVOID lpPointer, int nRow, int nCol, CRuntimeClass* pRuntimeClass)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SetCellType(nRow, nCol, pRuntimeClass);
}

int WINAPI Ex_InsertColumn(LPVOID lpPointer, LPCTSTR strHeading, UINT nFormat, int nColumn)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.InsertColumn(strHeading, nFormat, nColumn);
}

int WINAPI Ex_InsertRow(LPVOID lpPointer, LPCTSTR strHeading, int nRow)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.InsertRow(strHeading, nRow);
}

BOOL WINAPI Ex_DeleteColumn(LPVOID lpPointer, int nColumn)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.DeleteColumn(nColumn);
}

BOOL WINAPI Ex_DeleteRow(LPVOID lpPointer, int nRow)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.DeleteRow(nRow);
}

BOOL WINAPI Ex_DeleteNonFixedRows(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.DeleteNonFixedRows();
}

BOOL WINAPI Ex_DeleteAllItems(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.DeleteAllItems();
}

void WINAPI Ex_ClearCells(LPVOID lpPointer, CCellRange Selection)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.ClearCells(Selection);
}

BOOL WINAPI Ex_AutoSizeRow(LPVOID lpPointer, int nRow, BOOL bResetScroll)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.AutoSizeRow(nRow, bResetScroll);
}

BOOL WINAPI Ex_AutoSizeColumn(LPVOID lpPointer, int nCol, UINT nAutoSizeStyle, BOOL bResetScroll)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.AutoSizeColumn(nCol, nAutoSizeStyle, bResetScroll);
}

void WINAPI Ex_AutoSizeRows(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.AutoSizeRows();
}

void WINAPI Ex_AutoSizeColumns(LPVOID lpPointer, UINT nAutoSizeStyle)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.AutoSizeColumns(nAutoSizeStyle);
}

void WINAPI Ex_AutoSize(LPVOID lpPointer, UINT nAutoSizeStyle)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.AutoSize(nAutoSizeStyle);
}

void WINAPI Ex_ExpandColumnsToFit(LPVOID lpPointer, BOOL bExpandFixed)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.ExpandColumnsToFit(bExpandFixed);
}

void WINAPI Ex_ExpandLastColumn(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.ExpandLastColumn();
}

void WINAPI Ex_ExpandRowsToFit(LPVOID lpPointer, BOOL bExpandFixed)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.ExpandRowsToFit(bExpandFixed);
}

void WINAPI Ex_ExpandToFit(LPVOID lpPointer, BOOL bExpandFixed)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.ExpandToFit(bExpandFixed);
}

void WINAPI Ex_Refresh(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.Refresh();
}

void WINAPI Ex_AutoFill(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.AutoFill();
}

void WINAPI Ex_EnsureVisible(LPVOID lpPointer, CCellID& cell)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.EnsureVisible(cell);
}

void WINAPI Ex_EnsureVisible2(LPVOID lpPointer, int nRow, int nCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.EnsureVisible(nRow, nCol);
}

BOOL WINAPI Ex_IsCellVisible(LPVOID lpPointer, int nRow, int nCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.IsCellVisible(nRow, nCol);
}

BOOL WINAPI Ex_IsCellVisible2(LPVOID lpPointer, CCellID cell)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.IsCellVisible(cell);
}

BOOL WINAPI Ex_IsCellEditable(LPVOID lpPointer, int nRow, int nCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.IsCellEditable(nRow, nCol);
}

BOOL WINAPI Ex_IsCellEditable2(LPVOID lpPointer, CCellID& cell)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.IsCellEditable(cell);
}

BOOL WINAPI Ex_IsCellSelected(LPVOID lpPointer, int nRow, int nCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.IsCellSelected(nRow, nCol);
}

BOOL WINAPI Ex_IsCellSelected2(LPVOID lpPointer, CCellID& cell)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.IsCellSelected(cell);
}

void WINAPI Ex_SetRedraw(LPVOID lpPointer, BOOL bAllowDraw, BOOL bResetScrollBars)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetRedraw(bAllowDraw, bResetScrollBars);
}

BOOL WINAPI Ex_RedrawCell(LPVOID lpPointer, int nRow, int nCol, CDC* pDC)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.RedrawCell(nRow, nCol, pDC);
}

BOOL WINAPI Ex_RedrawCell2(LPVOID lpPointer, const CCellID& cell, CDC* pDC)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.RedrawCell(cell, pDC);
}

BOOL WINAPI Ex_RedrawRow(LPVOID lpPointer, int row)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.RedrawRow(row);
}

BOOL WINAPI Ex_RedrawColumn(LPVOID lpPointer, int col)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.RedrawColumn(col);
}

BOOL WINAPI Ex_Save(LPVOID lpPointer, LPCTSTR filename, TCHAR chSeparator)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.Save(filename, chSeparator);
}

BOOL WINAPI Ex_Load(LPVOID lpPointer, LPCTSTR filename, TCHAR chSeparator)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.Load(filename, chSeparator);
}

CCellRange WINAPI Ex_GetCellRange(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetCellRange();
}

CCellRange WINAPI Ex_GetSelectedCellRange(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetSelectedCellRange();
}

void WINAPI Ex_SetSelectedRange(LPVOID lpPointer, const CCellRange& Range, BOOL bForceRepaint, BOOL bSelectCells)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetSelectedRange(Range, bForceRepaint, bSelectCells);
}

void WINAPI Ex_SetSelectedRange2(LPVOID lpPointer, int nMinRow, int nMinCol, int nMaxRow, int nMaxCol, BOOL bForceRepaint, BOOL bSelectCells)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetSelectedRange(nMinRow, nMinCol, nMaxRow, nMaxCol, bForceRepaint, bSelectCells);
}

BOOL WINAPI Ex_IsValid(LPVOID lpPointer, int nRow, int nCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.IsValid(nRow, nCol);
}

BOOL WINAPI Ex_IsValid2(LPVOID lpPointer, const CCellID& cell)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.IsValid(cell);
}

BOOL WINAPI Ex_IsValid3(LPVOID lpPointer, const CCellRange& range)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.IsValid(range);
}

void WINAPI Ex_CutSelectedText(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.CutSelectedText();
}

COleDataSource* WINAPI Ex_CopyTextFromGrid(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.CopyTextFromGrid();
}

BOOL WINAPI Ex_PasteTextToGrid(LPVOID lpPointer, CCellID cell, COleDataObject* pDataObject, BOOL bSelectPastedCells)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.PasteTextToGrid(cell, pDataObject, bSelectPastedCells);
}

void WINAPI Ex_OnBeginDrag(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnBeginDrag();
}

DROPEFFECT WINAPI Ex_OnDragEnter(LPVOID lpPointer, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.OnDragEnter(pDataObject, dwKeyState, point);
}

DROPEFFECT WINAPI Ex_OnDragOver(LPVOID lpPointer, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.OnDragOver(pDataObject, dwKeyState, point);
}

void WINAPI Ex_OnDragLeave(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnDragLeave();
}

BOOL WINAPI Ex_OnDrop(LPVOID lpPointer, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.OnDrop(pDataObject, dwKeyState, point);
}

void WINAPI Ex_OnEditCut(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnEditCut();
}

void WINAPI Ex_OnEditCopy(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnEditCopy();
}

void WINAPI Ex_OnEditPaste(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnEditPaste();
}

void WINAPI Ex_OnEditSelectAll(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnEditSelectAll();
}

CCellID WINAPI Ex_GetNextItem(LPVOID lpPointer, CCellID& cell, int nFlags)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetNextItem(cell, nFlags);
}

BOOL WINAPI Ex_SortItems1(LPVOID lpPointer, int nCol, BOOL bAscending, LPARAM data)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SortItems(nCol, bAscending, data);
}

BOOL WINAPI Ex_SortTextItems2(LPVOID lpPointer, int nCol, BOOL bAscending, LPARAM data)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SortTextItems(nCol, bAscending, data);
}

BOOL WINAPI Ex_SortItems2(LPVOID lpPointer, PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SortItems(pfnCompare, nCol, bAscending, data);
}

void WINAPI Ex_SetCompareFunction(LPVOID lpPointer, PFNLVCOMPARE pfnCompare)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetCompareFunction(pfnCompare);
}

BOOL WINAPI Ex_RegisterWindowClass(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.RegisterWindowClass();
}

BOOL WINAPI Ex_Initialise(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.Initialise();
}

void WINAPI Ex_SetupDefaultCells(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetupDefaultCells();
}

LRESULT WINAPI Ex_SendMessageToParent(LPVOID lpPointer, int nRow, int nCol, int nMessage)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SendMessageToParent(nRow, nCol, nMessage);
}

LRESULT WINAPI Ex_SendDisplayRequestToParent(LPVOID lpPointer, GV_DISPINFO* pDisplayInfo)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SendDisplayRequestToParent(pDisplayInfo);
}

LRESULT WINAPI Ex_SendCacheHintToParent(LPVOID lpPointer, const CCellRange& range)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SendCacheHintToParent(range);
}

BOOL WINAPI Ex_InvalidateCellRect(LPVOID lpPointer, const int row, const int col)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.InvalidateCellRect(row, col);
}

BOOL WINAPI Ex_InvalidateCellRect2(LPVOID lpPointer, const CCellID& cell)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.InvalidateCellRect(cell);
}

BOOL WINAPI Ex_InvalidateCellRect3(LPVOID lpPointer, const CCellRange& cellRange)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.InvalidateCellRect(cellRange);
}

void WINAPI Ex_EraseBkgnd(LPVOID lpPointer, CDC* pDC)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.EraseBkgnd(pDC);
}

BOOL WINAPI Ex_GetCellRangeRect(LPVOID lpPointer, const CCellRange& cellRange, LPRECT lpRect)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetCellRangeRect(cellRange, lpRect);
}

BOOL WINAPI Ex_SetCell(LPVOID lpPointer, int nRow, int nCol, CGridCellBase* pCell)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SetCell(nRow, nCol, pCell);
}

int WINAPI Ex_SetMouseMode(LPVOID lpPointer, int nMode)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SetMouseMode(nMode);
}

int WINAPI Ex_GetMouseMode(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetMouseMode();
}

BOOL WINAPI Ex_MouseOverRowResizeArea(LPVOID lpPointer, CPoint& point)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.MouseOverRowResizeArea(point);
}

BOOL WINAPI Ex_MouseOverColumnResizeArea(LPVOID lpPointer, CPoint& point)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.MouseOverColumnResizeArea(point);
}

CCellID WINAPI Ex_GetTopleftNonFixedCell(LPVOID lpPointer, BOOL bForceRecalculation)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetTopleftNonFixedCell(bForceRecalculation);
}

CCellRange WINAPI Ex_GetUnobstructedNonFixedCellRange(LPVOID lpPointer, BOOL bForceRecalculation)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetUnobstructedNonFixedCellRange(bForceRecalculation);
}

CCellRange WINAPI Ex_GetVisibleNonFixedCellRange(LPVOID lpPointer, LPRECT pRect, BOOL bForceRecalculation)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetVisibleNonFixedCellRange(pRect, bForceRecalculation);
}

BOOL WINAPI Ex_IsVisibleVScroll(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.IsVisibleVScroll();
}

BOOL WINAPI Ex_IsVisibleHScroll(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.IsVisibleHScroll();
}

void WINAPI Ex_ResetSelectedRange(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.ResetSelectedRange();
}

void WINAPI Ex_ResetScrollBars(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.ResetScrollBars();
}

void WINAPI Ex_EnableScrollBars(LPVOID lpPointer, int nBar, BOOL bEnable)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.EnableScrollBars(nBar, bEnable);
}

int WINAPI Ex_GetScrollPos32(LPVOID lpPointer, int nBar, BOOL bGetTrackPos)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetScrollPos32(nBar, bGetTrackPos);
}

BOOL WINAPI Ex_SetScrollPos32(LPVOID lpPointer, int nBar, int nPos, BOOL bRedraw)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SetScrollPos32(nBar, nPos, bRedraw);
}

BOOL WINAPI Ex_SortTextItems3(LPVOID lpPointer, int nCol, BOOL bAscending, int low, int high)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SortTextItems(nCol, bAscending, low, high);
}

BOOL WINAPI Ex_SortItems3(LPVOID lpPointer, PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data, int low, int high)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SortItems(pfnCompare, nCol, bAscending, data, low, high);
}

CPoint WINAPI Ex_GetPointClicked(LPVOID lpPointer, int nRow, int nCol, const CPoint& point)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetPointClicked(nRow, nCol, point);
}

void WINAPI Ex_ValidateAndModifyCellContents(LPVOID lpPointer, int nRow, int nCol, LPCTSTR strText)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.ValidateAndModifyCellContents(nRow, nCol, strText);
}

void WINAPI Ex_PreSubclassWindow(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.PreSubclassWindow();
}

LRESULT WINAPI Ex_WindowProc(LPVOID lpPointer, UINT message, WPARAM wParam, LPARAM lParam)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.WindowProc(message, wParam, lParam);
}

CImageList* WINAPI Ex_CreateDragImage(LPVOID lpPointer, CPoint* pHotSpot)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.CreateDragImage(pHotSpot);
}

void WINAPI Ex_OnFixedColumnClick(LPVOID lpPointer, CCellID& cell)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnFixedColumnClick(cell);
}

void WINAPI Ex_OnFixedRowClick(LPVOID lpPointer, CCellID& cell)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnFixedRowClick(cell);
}

void WINAPI Ex_OnEditCell(LPVOID lpPointer, int nRow, int nCol, CPoint point, UINT nChar)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnEditCell(nRow, nCol, point, nChar);
}

void WINAPI Ex_OnEndEditCell(LPVOID lpPointer, int nRow, int nCol, CString str)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnEndEditCell(nRow, nCol, str);
}

BOOL WINAPI Ex_ValidateEdit(LPVOID lpPointer, int nRow, int nCol, LPCTSTR str)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.ValidateEdit(nRow, nCol, str);
}

void WINAPI Ex_EndEditing(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.EndEditing();
}

void WINAPI Ex_OnDraw(LPVOID lpPointer, CDC* pDC)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnDraw(pDC);
}

CGridCellBase* WINAPI Ex_CreateCell(LPVOID lpPointer, int nRow, int nCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.CreateCell(nRow, nCol);
}

void WINAPI Ex_DestroyCell(LPVOID lpPointer, int nRow, int nCol)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.DestroyCell(nRow, nCol);
}

void WINAPI Ex_SelectAllCells(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SelectAllCells();
}

void WINAPI Ex_SelectColumns(LPVOID lpPointer, CCellID currentCell, BOOL bForceRedraw, BOOL bSelectCells)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SelectColumns(currentCell, bForceRedraw, bSelectCells);
}

void WINAPI Ex_SelectRows(LPVOID lpPointer, CCellID currentCell, BOOL bForceRedraw, BOOL bSelectCells)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SelectRows(currentCell, bForceRedraw, bSelectCells);
}

void WINAPI Ex_SelectCells(LPVOID lpPointer, CCellID currentCell, BOOL bForceRedraw, BOOL bSelectCells)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SelectCells(currentCell, bForceRedraw, bSelectCells);
}

void WINAPI Ex_OnSelecting(LPVOID lpPointer, const CCellID& currentCell)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnSelecting(currentCell);
}

void WINAPI Ex_OnPaint(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnPaint();
}

void WINAPI Ex_OnHScroll(LPVOID lpPointer, UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnHScroll(nSBCode, nPos, pScrollBar);
}

void WINAPI Ex_OnVScroll(LPVOID lpPointer, UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnVScroll(nSBCode, nPos, pScrollBar);
}

void WINAPI Ex_OnSize(LPVOID lpPointer, UINT nType, int cx, int cy)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnSize(nType, cx, cy);
}

void WINAPI Ex_OnLButtonUp(LPVOID lpPointer, UINT nFlags, CPoint point)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnLButtonUp(nFlags, point);
}

void WINAPI Ex_OnLButtonDown(LPVOID lpPointer, UINT nFlags, CPoint point)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnLButtonDown(nFlags, point);
}

void WINAPI Ex_OnMouseMove(LPVOID lpPointer, UINT nFlags, CPoint point)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnMouseMove(nFlags, point);
}

void WINAPI Ex_OnTimer(LPVOID lpPointer, UINT nIDEvent)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnTimer(nIDEvent);
}

UINT WINAPI Ex_OnGetDlgCode(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.OnGetDlgCode();
}

void WINAPI Ex_OnKeyDown(LPVOID lpPointer, UINT nChar, UINT nRepCnt, UINT nFlags)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnKeyDown(nChar, nRepCnt, nFlags);
}

void WINAPI Ex_OnKeyUp(LPVOID lpPointer, UINT nChar, UINT nRepCnt, UINT nFlags)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnKeyUp(nChar, nRepCnt, nFlags);
}

void WINAPI Ex_OnChar(LPVOID lpPointer, UINT nChar, UINT nRepCnt, UINT nFlags)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnChar(nChar, nRepCnt, nFlags);
}

void WINAPI Ex_OnLButtonDblClk(LPVOID lpPointer, UINT nFlags, CPoint point)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnLButtonDblClk(nFlags, point);
}

BOOL WINAPI Ex_OnEraseBkgnd(LPVOID lpPointer, CDC* pDC)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.OnEraseBkgnd(pDC);
}

void WINAPI Ex_OnSysKeyDown(LPVOID lpPointer, UINT nChar, UINT nRepCnt, UINT nFlags)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void WINAPI Ex_OnContextMenu(LPVOID lpPointer, CWnd* pWnd, CPoint point)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnContextMenu(pWnd, point);
}

void WINAPI Ex_OnUpdateEditSelectAll(LPVOID lpPointer, CCmdUI* pCmdUI)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnUpdateEditSelectAll(pCmdUI);
}

BOOL WINAPI Ex_OnSetCursor(LPVOID lpPointer, CWnd* pWnd, UINT nHitTest, UINT message)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.OnSetCursor(pWnd, nHitTest, message);
}

void WINAPI Ex_OnRButtonDown(LPVOID lpPointer, UINT nFlags, CPoint point)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnRButtonDown(nFlags, point);
}

void WINAPI Ex_OnRButtonUp(LPVOID lpPointer, UINT nFlags, CPoint point)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnRButtonUp(nFlags, point);
}

void WINAPI Ex_OnSysColorChange(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnSysColorChange();
}

void WINAPI Ex_OnCaptureChanged(LPVOID lpPointer, CWnd* pWnd)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnCaptureChanged(pWnd);
}

void WINAPI Ex_OnUpdateEditCopy(LPVOID lpPointer, CCmdUI* pCmdUI)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnUpdateEditCopy(pCmdUI);
}

void WINAPI Ex_OnUpdateEditCut(LPVOID lpPointer, CCmdUI* pCmdUI)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnUpdateEditCut(pCmdUI);
}

void WINAPI Ex_OnUpdateEditPaste(LPVOID lpPointer, CCmdUI* pCmdUI)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnUpdateEditPaste(pCmdUI);
}

void WINAPI Ex_OnSettingChange(LPVOID lpPointer, UINT uFlags, LPCTSTR lpszSection)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnSettingChange(uFlags, lpszSection);
}

BOOL WINAPI Ex_OnMouseWheel(LPVOID lpPointer, UINT nFlags, short zDelta, CPoint pt)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.OnMouseWheel(nFlags, zDelta, pt);
}

LRESULT WINAPI Ex_OnSetFont(LPVOID lpPointer, WPARAM hFont, LPARAM lParam)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.OnSetFont(hFont, lParam);
}

LRESULT WINAPI Ex_OnImeChar(LPVOID lpPointer, WPARAM wCharCode, LPARAM lParam)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.OnImeChar(wCharCode, lParam);
}

void WINAPI Ex_OnEndInPlaceEdit(LPVOID lpPointer, NMHDR* pNMHDR, LRESULT* pResult)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.OnEndInPlaceEdit(pNMHDR, pResult);
}

UINT WINAPI Ex_GetMouseScrollLines(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetMouseScrollLines();
}

UINT WINAPI Ex_SetDefaultCellType(LPVOID lpPointer, CRuntimeClass* pRuntimeClass)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.SetDefaultCellType(pRuntimeClass);
}

void WINAPI Ex_SetGridScrollBarVisible(LPVOID lpPointer, BOOL bHScroll, BOOL bVScroll)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetGridScrollBarVisible(bHScroll, bVScroll);
}

void WINAPI Ex_SetUseFocusRect(LPVOID lpPointer, BOOL bUse)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	theApp.SetUseFocusRect(bUse);
}

BOOL WINAPI Ex_GetUseFocusRect(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	return theApp.GetUseFocusRect();
}

void WINAPI Ex_SetDrawFixedLine(LPVOID lpPointer, BOOL bDraw)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.SetDrawFixedLine(bDraw);
}