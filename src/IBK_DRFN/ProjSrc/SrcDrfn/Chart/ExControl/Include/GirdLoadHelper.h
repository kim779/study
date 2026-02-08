#if !defined(AFX_LOADHELPERGRID_H__DF553F68_69A3_11D6_994C_004854800CB4__INCLUDED_)
#define AFX_LOADHELPERGRID_H__DF553F68_69A3_11D6_994C_004854800CB4__INCLUDED_

#include "BaseLoadHelper.h"

//Solomon-090829 사이트에 맞는 디렉토리 정보를 구하는 루틴
#include "../../../ForSite/FS_Env.h"

/////////////////////////////////////////////////////////////////////////////
//	CLoaderGridCtrl
/////////////////////////////////////////////////////////////////////////////
typedef long	(WINAPI* FpActionControl)(LPVOID, LPVOID);
typedef void	(WINAPI* FpExitGridCtrl)(LPVOID);
typedef LPVOID	(WINAPI* FpInitGridCtrl)(LPVOID);
typedef BOOL	(WINAPI* FpCreate)(LPVOID, RECT&, CWnd*, UINT, DWORD);
typedef int		(WINAPI* FpGetRowCount)(LPVOID);
typedef int		(WINAPI* FpGetColumnCount)(LPVOID);
typedef int		(WINAPI* FpGetFixedRowCount)(LPVOID);
typedef int		(WINAPI* FpGetFixedColumnCount)(LPVOID);
typedef BOOL	(WINAPI* FpSetRowCount)(LPVOID, int);
typedef BOOL	(WINAPI* FpSetColumnCount)(LPVOID, int);
typedef BOOL	(WINAPI* FpSetFixedRowCount)(LPVOID, int);
typedef BOOL	(WINAPI* FpSetFixedColumnCount)(LPVOID, int);
typedef int		(WINAPI* FpGetRowHeight)(LPVOID, int);
typedef BOOL	(WINAPI* FpSetRowHeight)(LPVOID, int, int);
typedef int		(WINAPI* FpGetColumnWidth)(LPVOID, int);
typedef BOOL	(WINAPI* FpSetColumnWidth)(LPVOID, int, int);
typedef BOOL	(WINAPI* FpGetCellOrigin)(LPVOID, int, int, LPPOINT);
typedef BOOL	(WINAPI* FpGetCellOrigin2)(LPVOID, CCellID&, LPPOINT);
typedef BOOL	(WINAPI* FpGetCellRect)(LPVOID, int, int, LPRECT);
typedef BOOL	(WINAPI* FpGetCellRect2)(LPVOID, CCellID&, LPRECT);
typedef BOOL	(WINAPI* FpGetTextRect)(LPVOID, CCellID&, LPRECT);
typedef BOOL	(WINAPI* FpGetTextRect2)(LPVOID, int, int, LPRECT);
typedef CCellID	(WINAPI* FpGetCellFromPt)(LPVOID, CPoint, BOOL);
typedef int		(WINAPI* FpGetFixedRowHeight)(LPVOID);
typedef int		(WINAPI* FpGetFixedColumnWidth)(LPVOID);
typedef long	(WINAPI* FpGetVirtualWidth)(LPVOID);
typedef long	(WINAPI* FpGetVirtualHeight)(LPVOID);
typedef CSize	(WINAPI* FpGetTextExtent)(LPVOID, int, int, LPCTSTR);
typedef CSize	(WINAPI* FpGetCellTextExtent)(LPVOID, int, int);
typedef void	(WINAPI* FpSetGridBkColor)(LPVOID, COLORREF);
typedef COLORREF	(WINAPI* FpGetGridBkColor)(LPVOID);
typedef void	(WINAPI* FpSetGridLineColor)(LPVOID, COLORREF);
typedef COLORREF	(WINAPI* FpGetGridLineColor)(LPVOID);
typedef void	(WINAPI* FpSetTitleTipBackClr)(LPVOID, COLORREF);
typedef COLORREF	(WINAPI* FpGetTitleTipBackClr)(LPVOID);
typedef void	(WINAPI* FpSetTitleTipTextClr)(LPVOID, COLORREF);
typedef COLORREF	(WINAPI* FpGetTitleTipTextClr)(LPVOID);
typedef void	(WINAPI* FpSetFixedGridLineColor_Light)(LPVOID, COLORREF);
typedef void	(WINAPI* FpSetFixedGridLineColor_Dark)(LPVOID, COLORREF);
typedef COLORREF	(WINAPI* FpGetFixedGridLineColor_Light)(LPVOID);
typedef COLORREF	(WINAPI* FpGetFixedGridLineColor_Dark)(LPVOID);
typedef void	(WINAPI* FpSetHighlightColor)(LPVOID, COLORREF);
typedef void	(WINAPI* FpSetHighlightTextColor)(LPVOID, COLORREF);
typedef COLORREF	(WINAPI* FpGetHighlightColor)(LPVOID);
typedef COLORREF	(WINAPI* FpGetHighlightTextColor)(LPVOID);
typedef void	(WINAPI* FpSetUseHighlightText)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpGetUseHighlightText)(LPVOID);
typedef void	(WINAPI* FpSetFixedRowSelectionHighlight)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpGetFixedRowSelectionHighlight)(LPVOID);
typedef void	(WINAPI* FpSetTextColor)(LPVOID, COLORREF);
typedef COLORREF	(WINAPI* FpGetTextColor)(LPVOID);
typedef void	(WINAPI* FpSetTextBkColor)(LPVOID, COLORREF);
typedef COLORREF	(WINAPI* FpGetTextBkColor)(LPVOID);
typedef void	(WINAPI* FpSetFixedTextColor)(LPVOID, COLORREF);
typedef COLORREF	(WINAPI* FpGetFixedTextColor)(LPVOID);
typedef void	(WINAPI* FpSetFixedBkColor)(LPVOID, COLORREF);
typedef COLORREF	(WINAPI* FpGetFixedBkColor)(LPVOID);
typedef void	(WINAPI* FpSetGridColor)(LPVOID, COLORREF);
typedef COLORREF	(WINAPI* FpGetGridColor)(LPVOID);
typedef void	(WINAPI* FpSetBkColor)(LPVOID, COLORREF);
typedef COLORREF	(WINAPI* FpGetBkColor)(LPVOID);
typedef void	(WINAPI* FpSetDefCellMargin)(LPVOID, int);
typedef int		(WINAPI* FpGetDefCellMargin)(LPVOID);
typedef int		(WINAPI* FpGetDefCellHeight)(LPVOID);
typedef void	(WINAPI* FpSetDefCellHeight)(LPVOID, int);
typedef int		(WINAPI* FpGetDefCellWidth)(LPVOID);
typedef void	(WINAPI* FpSetDefCellWidth)(LPVOID, int);
typedef int		(WINAPI* FpGetSelectedCount)(LPVOID);
typedef CCellID	(WINAPI* FpSetFocusCell1)(LPVOID, CCellID);
typedef CCellID	(WINAPI* FpSetFocusCell2)(LPVOID, int, int);
typedef CCellID	(WINAPI* FpGetFocusCell)(LPVOID);
typedef void	(WINAPI* FpSetVirtualMode)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpGetVirtualMode)(LPVOID);
typedef void	(WINAPI* FpSetCallbackFunc)(LPVOID, GRIDCALLBACK, LPARAM);
typedef GRIDCALLBACK	(WINAPI* FpGetCallbackFunc)(LPVOID);
typedef void	(WINAPI* FpSetImageList)(LPVOID, CImageList*);
typedef CImageList*	(WINAPI* FpGetImageList)(LPVOID);
typedef void	(WINAPI* FpSetGridLines)(LPVOID, int);
typedef int		(WINAPI* FpGetGridLines)(LPVOID);
typedef void	(WINAPI* FpSetEditable)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpIsEditable)(LPVOID);
typedef void	(WINAPI* FpSetListMode)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpGetListMode)(LPVOID);
typedef void	(WINAPI* FpSetSingleRowSelection)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpGetSingleRowSelection)(LPVOID);
typedef void	(WINAPI* FpSetSingleColSelection)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpGetSingleColSelection)(LPVOID);
typedef void	(WINAPI* FpEnableSelection)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpIsSelectable)(LPVOID);
typedef void	(WINAPI* FpSetFixedColumnSelection)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpGetFixedColumnSelection)(LPVOID);
typedef void	(WINAPI* FpSetFixedRowSelection)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpGetFixedRowSelection)(LPVOID);
typedef void	(WINAPI* FpEnableDragAndDrop)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpGetDragAndDrop)(LPVOID);
typedef void	(WINAPI* FpSetRowResize)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpGetRowResize)(LPVOID);
typedef void	(WINAPI* FpSetColumnResize)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpGetColumnResize)(LPVOID);
typedef void	(WINAPI* FpSetHeaderSort)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpGetHeaderSort)(LPVOID);
typedef void	(WINAPI* FpSetHandleTabKey)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpGetHandleTabKey)(LPVOID);
typedef void	(WINAPI* FpSetDoubleBuffering)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpGetDoubleBuffering)(LPVOID);
typedef void	(WINAPI* FpEnableTitleTips)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpGetTitleTips)(LPVOID);
typedef void	(WINAPI* FpSetSortColumn)(LPVOID, int);
typedef int		(WINAPI* FpGetSortColumn)(LPVOID);
typedef void	(WINAPI* FpSetSortAscending)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpGetSortAscending)(LPVOID);
typedef void	(WINAPI* FpSetTrackFocusCell)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpGetTrackFocusCell)(LPVOID);
typedef void	(WINAPI* FpSetFrameFocusCell)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpGetFrameFocusCell)(LPVOID);
typedef void	(WINAPI* FpSetAutoSizeStyle)(LPVOID, int);
typedef int		(WINAPI* FpGetAutoSizeStyle)(LPVOID);
typedef void	(WINAPI* FpEnableHiddenColUnhide)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpGetHiddenColUnhide)(LPVOID);
typedef void	(WINAPI* FpEnableHiddenRowUnhide)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpGetHiddenRowUnhide)(LPVOID);
typedef void	(WINAPI* FpEnableColumnHide)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpGetColumnHide)(LPVOID);
typedef void	(WINAPI* FpEnableRowHide)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpGetRowHide)(LPVOID);
typedef void	(WINAPI* FpSetDefaultReadOnly)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpIsDefaultReadOnly)(LPVOID);
typedef CGridCellBase*	(WINAPI* FpGetDefaultCell)(LPVOID, BOOL, BOOL);
typedef CGridCellBase*	(WINAPI* FpGetCell)(LPVOID, int, int);
typedef void	(WINAPI* FpSetModified)(LPVOID, BOOL, int, int);
typedef BOOL	(WINAPI* FpGetModified)(LPVOID, int, int);
typedef BOOL	(WINAPI* FpIsCellFixed)(LPVOID, int, int);
typedef BOOL	(WINAPI* FpSetItem)(LPVOID, GV_ITEM*);
typedef BOOL	(WINAPI* FpGetItem)(LPVOID, GV_ITEM*);
typedef BOOL	(WINAPI* FpSetItemText)(LPVOID, int, int, LPCTSTR);
typedef CString	(WINAPI* FpGetItemText)(LPVOID, int, int);
typedef BOOL	(WINAPI* FpSetItemTextFmt)(LPVOID, int, int, LPCTSTR, ...);
typedef BOOL	(WINAPI* FpSetItemTextFmtID)(LPVOID, int, int, UINT, ...);
typedef BOOL	(WINAPI* FpSetItemData)(LPVOID, int, int, LPARAM);
typedef LPARAM	(WINAPI* FpGetItemData)(LPVOID, int, int);
typedef BOOL	(WINAPI* FpSetItemImage)(LPVOID, int, int, int);
typedef int		(WINAPI* FpGetItemImage)(LPVOID, int, int);
typedef BOOL	(WINAPI* FpSetItemState)(LPVOID, int, int, UINT);
typedef UINT	(WINAPI* FpGetItemState)(LPVOID, int, int);
typedef BOOL	(WINAPI* FpSetItemFormat)(LPVOID, int, int, UINT);
typedef UINT	(WINAPI* FpGetItemFormat)(LPVOID, int, int);
typedef BOOL	(WINAPI* FpSetItemBkColour)(LPVOID, int, int, COLORREF);
typedef COLORREF	(WINAPI* FpGetItemBkColour)(LPVOID, int, int);
typedef BOOL	(WINAPI* FpSetItemFgColour)(LPVOID, int, int, COLORREF);
typedef COLORREF	(WINAPI* FpGetItemFgColour)(LPVOID, int, int);
typedef BOOL	(WINAPI* FpSetItemFont)(LPVOID, int, int, LOGFONT*);
typedef LOGFONT*	(WINAPI* FpGetItemFont)(LPVOID, int, int);
typedef BOOL	(WINAPI* FpIsItemEditing)(LPVOID, int, int);
typedef BOOL	(WINAPI* FpSetCellType)(LPVOID, int, int, CRuntimeClass*);
typedef int		(WINAPI* FpInsertColumn)(LPVOID, LPCTSTR, UINT, int);
typedef int		(WINAPI* FpInsertRow)(LPVOID, LPCTSTR, int);
typedef BOOL	(WINAPI* FpDeleteColumn)(LPVOID, int);
typedef BOOL	(WINAPI* FpDeleteRow)(LPVOID, int);
typedef BOOL	(WINAPI* FpDeleteNonFixedRows)(LPVOID);
typedef BOOL	(WINAPI* FpDeleteAllItems)(LPVOID);
typedef void	(WINAPI* FpClearCells)(LPVOID, CCellRange);
typedef BOOL	(WINAPI* FpAutoSizeRow)(LPVOID, int, BOOL);
typedef BOOL	(WINAPI* FpAutoSizeColumn)(LPVOID, int, UINT, BOOL);
typedef void	(WINAPI* FpAutoSizeRows)(LPVOID);
typedef void	(WINAPI* FpAutoSizeColumns)(LPVOID, UINT);
typedef void	(WINAPI* FpAutoSize)(LPVOID, UINT);
typedef void	(WINAPI* FpExpandColumnsToFit)(LPVOID, BOOL);
typedef void	(WINAPI* FpExpandLastColumn)(LPVOID);
typedef void	(WINAPI* FpExpandRowsToFit)(LPVOID, BOOL);
typedef void	(WINAPI* FpExpandToFit)(LPVOID, BOOL);
typedef void	(WINAPI* FpRefresh)(LPVOID);
typedef void	(WINAPI* FpAutoFill)(LPVOID);
typedef void	(WINAPI* FpEnsureVisible)(LPVOID, CCellID&);
typedef void	(WINAPI* FpEnsureVisible2)(LPVOID, int, int);
typedef BOOL	(WINAPI* FpIsCellVisible)(LPVOID, int, int);
typedef BOOL	(WINAPI* FpIsCellVisible2)(LPVOID, CCellID);
typedef BOOL	(WINAPI* FpIsCellEditable)(LPVOID, int, int);
typedef BOOL	(WINAPI* FpIsCellEditable2)(LPVOID, CCellID&);
typedef BOOL	(WINAPI* FpIsCellSelected)(LPVOID, int, int);
typedef BOOL	(WINAPI* FpIsCellSelected2)(LPVOID, CCellID&);
typedef void	(WINAPI* FpSetRedraw)(LPVOID, BOOL, BOOL);
typedef BOOL	(WINAPI* FpRedrawCell)(LPVOID, int, int, CDC*);
typedef BOOL	(WINAPI* FpRedrawCell2)(LPVOID, CCellID&, CDC*);
typedef BOOL	(WINAPI* FpRedrawRow)(LPVOID, int);
typedef BOOL	(WINAPI* FpRedrawColumn)(LPVOID, int);
typedef BOOL	(WINAPI* FpSave)(LPVOID, LPCTSTR, TCHAR);
typedef BOOL	(WINAPI* FpLoad)(LPVOID, LPCTSTR, TCHAR);
typedef CCellRange	(WINAPI* FpGetCellRange)(LPVOID);
typedef CCellRange	(WINAPI* FpGetSelectedCellRange)(LPVOID);
typedef void	(WINAPI* FpSetSelectedRange)(LPVOID, CCellRange&, BOOL, BOOL);
typedef void	(WINAPI* FpSetSelectedRange2)(LPVOID, int, int, int, int, BOOL, BOOL);
typedef BOOL	(WINAPI* FpIsValid)(LPVOID, int, int);
typedef BOOL	(WINAPI* FpIsValid2)(LPVOID, CCellID&);
typedef BOOL	(WINAPI* FpIsValid3)(LPVOID, CCellRange&);
typedef void	(WINAPI* FpCutSelectedText)(LPVOID);
typedef COleDataSource*	(WINAPI* FpCopyTextFromGrid)(LPVOID);
typedef BOOL	(WINAPI* FpPasteTextToGrid)(LPVOID, CCellID, COleDataObject*, BOOL);
typedef void	(WINAPI* FpOnBeginDrag)(LPVOID);
typedef DROPEFFECT	(WINAPI* FpOnDragEnter)(LPVOID, COleDataObject*, DWORD, CPoint);
typedef DROPEFFECT	(WINAPI* FpOnDragOver)(LPVOID, COleDataObject*, DWORD, CPoint);
typedef void	(WINAPI* FpOnDragLeave)(LPVOID);
typedef BOOL	(WINAPI* FpOnDrop)(LPVOID, COleDataObject*, DROPEFFECT, CPoint);
typedef void	(WINAPI* FpOnEditCut)(LPVOID);
typedef void	(WINAPI* FpOnEditCopy)(LPVOID);
typedef void	(WINAPI* FpOnEditPaste)(LPVOID);
typedef void	(WINAPI* FpOnEditSelectAll)(LPVOID);
typedef CCellID	(WINAPI* FpGetNextItem)(LPVOID, CCellID&, int);
typedef BOOL	(WINAPI* FpSortItems1)(LPVOID, int, BOOL, LPARAM);
typedef BOOL	(WINAPI* FpSortTextItems2)(LPVOID, int, BOOL, LPARAM);
typedef BOOL	(WINAPI* FpSortItems2)(LPVOID, PFNLVCOMPARE, int, BOOL, LPARAM);
typedef void	(WINAPI* FpSetCompareFunction)(LPVOID, PFNLVCOMPARE);
typedef BOOL	(WINAPI* FpRegisterWindowClass)(LPVOID);
typedef BOOL	(WINAPI* FpInitialise)(LPVOID);
typedef void	(WINAPI* FpSetupDefaultCells)(LPVOID);
typedef LRESULT	(WINAPI* FpSendMessageToParent)(LPVOID, int, int, int);
typedef LRESULT	(WINAPI* FpSendDisplayRequestToParent)(LPVOID, GV_DISPINFO*);
typedef LRESULT	(WINAPI* FpSendCacheHintToParent)(LPVOID, CCellRange&);
typedef BOOL	(WINAPI* FpInvalidateCellRect)(LPVOID, int, int);
typedef BOOL	(WINAPI* FpInvalidateCellRect2)(LPVOID, CCellID&);
typedef BOOL	(WINAPI* FpInvalidateCellRect3)(LPVOID, CCellRange&);
typedef void	(WINAPI* FpEraseBkgnd)(LPVOID, CDC*);
typedef BOOL	(WINAPI* FpGetCellRangeRect)(LPVOID, CCellRange&, LPRECT);
typedef BOOL	(WINAPI* FpSetCell)(LPVOID, int, int, CGridCellBase*);
typedef int		(WINAPI* FpSetMouseMode)(LPVOID, int);
typedef int		(WINAPI* FpGetMouseMode)(LPVOID);
typedef BOOL	(WINAPI* FpMouseOverRowResizeArea)(LPVOID, CPoint&);
typedef BOOL	(WINAPI* FpMouseOverColumnResizeArea)(LPVOID, CPoint&);
typedef CCellID	(WINAPI* FpGetTopleftNonFixedCell)(LPVOID, BOOL);
typedef CCellRange	(WINAPI* FpGetUnobstructedNonFixedCellRange)(LPVOID, BOOL);
typedef CCellRange	(WINAPI* FpGetVisibleNonFixedCellRange)(LPVOID, LPRECT, BOOL);
typedef BOOL	(WINAPI* FpIsVisibleVScroll)(LPVOID);
typedef BOOL	(WINAPI* FpIsVisibleHScroll)(LPVOID);
typedef void	(WINAPI* FpResetSelectedRange)(LPVOID);
typedef void	(WINAPI* FpResetScrollBars)(LPVOID);
typedef void	(WINAPI* FpEnableScrollBars)(LPVOID, int, BOOL);
typedef int		(WINAPI* FpGetScrollPos32)(LPVOID, int, BOOL);
typedef BOOL	(WINAPI* FpSetScrollPos32)(LPVOID, int, int, BOOL);
typedef BOOL	(WINAPI* FpSortTextItems3)(LPVOID, int, BOOL, int, int);
typedef BOOL	(WINAPI* FpSortItems3)(LPVOID, PFNLVCOMPARE, int, BOOL, LPARAM, int, int);
typedef CPoint	(WINAPI* FpGetPointClicked)(LPVOID, int, int, CPoint&);
typedef void	(WINAPI* FpValidateAndModifyCellContents)(LPVOID, int, int, LPCTSTR);
typedef void	(WINAPI* FpPreSubclassWindow)(LPVOID);
typedef LRESULT	(WINAPI* FpWindowProc)(LPVOID, UINT, WPARAM, LPARAM);
typedef CImageList*	(WINAPI* FpCreateDragImage)(LPVOID, CPoint*);
typedef void	(WINAPI* FpOnFixedColumnClick)(LPVOID, CCellID&);
typedef void	(WINAPI* FpOnFixedRowClick)(LPVOID, CCellID&);
typedef void	(WINAPI* FpOnEditCell)(LPVOID, int, int, CPoint, UINT);
typedef void	(WINAPI* FpOnEndEditCell)(LPVOID, int, int, CString);
typedef BOOL	(WINAPI* FpValidateEdit)(LPVOID, int, int, LPCTSTR);
typedef void	(WINAPI* FpEndEditing)(LPVOID);
typedef void	(WINAPI* FpOnDraw)(LPVOID, CDC*);
typedef CGridCellBase*	(WINAPI* FpCreateCell)(LPVOID, int, int);
typedef void	(WINAPI* FpDestroyCell)(LPVOID, int, int);
typedef void	(WINAPI* FpSelectAllCells)(LPVOID);
typedef void	(WINAPI* FpSelectColumns)(LPVOID, CCellID, BOOL, BOOL);
typedef void	(WINAPI* FpSelectRows)(LPVOID, CCellID, BOOL, BOOL);
typedef void	(WINAPI* FpSelectCells)(LPVOID, CCellID, BOOL, BOOL);
typedef void	(WINAPI* FpOnSelecting)(LPVOID, CCellID&);
typedef void	(WINAPI* FpOnPaint)(LPVOID);
typedef void	(WINAPI* FpOnHScroll)(LPVOID, UINT, UINT, CScrollBar*);
typedef void	(WINAPI* FpOnVScroll)(LPVOID, UINT, UINT, CScrollBar*);
typedef void	(WINAPI* FpOnSize)(LPVOID, UINT, int, int);
typedef void	(WINAPI* FpOnLButtonUp)(LPVOID, UINT, CPoint);
typedef void	(WINAPI* FpOnLButtonDown)(LPVOID, UINT, CPoint);
typedef void	(WINAPI* FpOnMouseMove)(LPVOID, UINT, CPoint);
typedef void	(WINAPI* FpOnTimer)(LPVOID, UINT);
typedef UINT	(WINAPI* FpOnGetDlgCode)(LPVOID);
typedef void	(WINAPI* FpOnKeyDown)(LPVOID, UINT, UINT, UINT);
typedef void	(WINAPI* FpOnKeyUp)(LPVOID, UINT, UINT, UINT);
typedef void	(WINAPI* FpOnChar)(LPVOID, UINT, UINT, UINT);
typedef void	(WINAPI* FpOnLButtonDblClk)(LPVOID, UINT, CPoint);
typedef BOOL	(WINAPI* FpOnEraseBkgnd)(LPVOID, CDC*);
typedef void	(WINAPI* FpOnSysKeyDown)(LPVOID, UINT, UINT, UINT);
typedef void	(WINAPI* FpOnContextMenu)(LPVOID, CWnd*, CPoint);
typedef void	(WINAPI* FpOnUpdateEditSelectAll)(LPVOID, CCmdUI*);
typedef BOOL	(WINAPI* FpOnSetCursor)(LPVOID, CWnd*, UINT, UINT);
typedef void	(WINAPI* FpOnRButtonDown)(LPVOID, UINT, CPoint);
typedef void	(WINAPI* FpOnRButtonUp)(LPVOID, UINT, CPoint);
typedef void	(WINAPI* FpOnSysColorChange)(LPVOID);
typedef void	(WINAPI* FpOnCaptureChanged)(LPVOID, CWnd*);
typedef void	(WINAPI* FpOnUpdateEditCopy)(LPVOID, CCmdUI*);
typedef void	(WINAPI* FpOnUpdateEditCut)(LPVOID, CCmdUI*);
typedef void	(WINAPI* FpOnUpdateEditPaste)(LPVOID, CCmdUI*);
typedef void	(WINAPI* FpOnSettingChange)(LPVOID, UINT, LPCTSTR);
typedef BOOL	(WINAPI* FpOnMouseWheel)(LPVOID, UINT, short, CPoint);
typedef LRESULT	(WINAPI* FpOnSetFont)(LPVOID, WPARAM, LPARAM);
typedef LRESULT	(WINAPI* FpOnGetFont)(LPVOID, WPARAM, LPARAM);
typedef LRESULT	(WINAPI* FpOnImeChar)(LPVOID, WPARAM, LPARAM);
typedef void	(WINAPI* FpOnEndInPlaceEdit)(LPVOID, NMHDR*, LRESULT*);
typedef UINT	(WINAPI* FpGetMouseScrollLines)(LPVOID);
typedef BOOL	(WINAPI* FpSetDefaultCellType)(LPVOID, CRuntimeClass*);
typedef void	(WINAPI* FpSetGridScrollBarVisible)(LPVOID, BOOL, BOOL);
typedef void	(WINAPI* FpSetUseFocusRect)(LPVOID, BOOL);
typedef BOOL	(WINAPI* FpGetUseFocusRect)(LPVOID);
typedef void	(WINAPI* FpSetDrawFixedLine)(LPVOID, BOOL);
class CLoaderGridCtrl : public CExLoadLib
{
public:
	CLoaderGridCtrl(LPVOID lpPointer, LPCSTR lpszRootPath = NULL) 
	{
// 		if(lpszRootPath)
// 			::sprintf(m_szDllName, "%s\\Bin\\%s", lpszRootPath, GRIDCTRL_DLL);
// 		else
// 		{
// 			//char szPath[MAX_PATH] = {0, };
// 			//GetCurrentWorkDirectory(szPath, sizeof(szPath));
// 			CString strPath;
// 			GET_MAIN_DIR(strPath);
// 
// 			if(strPath.GetLength() > 0)
// 				::sprintf(m_szDllName, "%s\\%s", strPath, GRIDCTRL_DLL);
// 			else
// 				::strcpy(m_szDllName, GRIDCTRL_DLL);
// 		}

		//-->Solomon-090829
		CString szDir;
		GetDir_DrfnBin(szDir);
		::sprintf(m_szDllName, "%s\\%s", szDir, GRIDCTRL_DLL);
		//<--Solomon-090829

		pActionControl = NULL;
		pExitGridCtrl = NULL;
		pInitGridCtrl = NULL;
		pCreate = NULL;
		pGetRowCount = NULL;
		pGetColumnCount = NULL;
		pGetFixedRowCount = NULL;
		pGetFixedColumnCount = NULL;
		pSetRowCount = NULL;
		pSetColumnCount = NULL;
		pSetFixedRowCount = NULL;
		pSetFixedColumnCount = NULL;
		pGetRowHeight = NULL;
		pSetRowHeight = NULL;
		pGetColumnWidth = NULL;
		pSetColumnWidth = NULL;
		pGetCellOrigin = NULL;
		pGetCellOrigin2 = NULL;
		pGetCellRect = NULL;
		pGetCellRect2 = NULL;
		pGetTextRect = NULL;
		pGetTextRect2 = NULL;
		pGetCellFromPt = NULL;
		pGetFixedRowHeight = NULL;
		pGetFixedColumnWidth = NULL;
		pGetVirtualWidth = NULL;
		pGetVirtualHeight = NULL;
		pGetTextExtent = NULL;
		pGetCellTextExtent = NULL;
		pSetGridBkColor = NULL;
		pGetGridBkColor = NULL;
		pSetGridLineColor = NULL;
		pGetGridLineColor = NULL;
		pSetTitleTipBackClr = NULL;
		pGetTitleTipBackClr = NULL;
		pSetTitleTipTextClr = NULL;
		pGetTitleTipTextClr = NULL;
		pSetFixedGridLineColor_Light = NULL;
		pSetFixedGridLineColor_Dark = NULL;
		pGetFixedGridLineColor_Light = NULL;
		pGetFixedGridLineColor_Dark = NULL;
		pSetHighlightColor = NULL;
		pSetHighlightTextColor = NULL;
		pGetHighlightColor = NULL;
		pGetHighlightTextColor = NULL;
		pSetUseHighlightText = NULL;
		pGetUseHighlightText = NULL;
		pSetFixedRowSelectionHighlight = NULL;
		pGetFixedRowSelectionHighlight = NULL;
		pSetTextColor = NULL;
		pGetTextColor = NULL;
		pSetTextBkColor = NULL;
		pGetTextBkColor = NULL;
		pSetFixedTextColor = NULL;
		pGetFixedTextColor = NULL;
		pSetFixedBkColor = NULL;
		pGetFixedBkColor = NULL;
		pSetGridColor = NULL;
		pGetGridColor = NULL;
		pSetBkColor = NULL;
		pGetBkColor = NULL;
		pSetDefCellMargin = NULL;
		pGetDefCellMargin = NULL;
		pGetDefCellHeight = NULL;
		pSetDefCellHeight = NULL;
		pGetDefCellWidth = NULL;
		pSetDefCellWidth = NULL;
		pGetSelectedCount = NULL;
		pSetFocusCell1 = NULL;
		pSetFocusCell2 = NULL;
		pGetFocusCell = NULL;
		pSetVirtualMode = NULL;
		pGetVirtualMode = NULL;
		pSetCallbackFunc = NULL;
		pGetCallbackFunc = NULL;
		pSetImageList = NULL;
		pGetImageList = NULL;
		pSetGridLines = NULL;
		pGetGridLines = NULL;
		pSetEditable = NULL;
		pIsEditable = NULL;
		pSetListMode = NULL;
		pGetListMode = NULL;
		pSetSingleRowSelection = NULL;
		pGetSingleRowSelection = NULL;
		pSetSingleColSelection = NULL;
		pGetSingleColSelection = NULL;
		pEnableSelection = NULL;
		pIsSelectable = NULL;
		pSetFixedColumnSelection = NULL;
		pGetFixedColumnSelection = NULL;
		pSetFixedRowSelection = NULL;
		pGetFixedRowSelection = NULL;
		pEnableDragAndDrop = NULL;
		pGetDragAndDrop = NULL;
		pSetRowResize = NULL;
		pGetRowResize = NULL;
		pSetColumnResize = NULL;
		pGetColumnResize = NULL;
		pSetHeaderSort = NULL;
		pGetHeaderSort = NULL;
		pSetHandleTabKey = NULL;
		pGetHandleTabKey = NULL;
		pSetDoubleBuffering = NULL;
		pGetDoubleBuffering = NULL;
		pEnableTitleTips = NULL;
		pGetTitleTips = NULL;
		pSetSortColumn = NULL;
		pGetSortColumn = NULL;
		pSetSortAscending = NULL;
		pGetSortAscending = NULL;
		pSetTrackFocusCell = NULL;
		pGetTrackFocusCell = NULL;
		pSetFrameFocusCell = NULL;
		pGetFrameFocusCell = NULL;
		pSetAutoSizeStyle = NULL;
		pGetAutoSizeStyle = NULL;
		pEnableHiddenColUnhide = NULL;
		pGetHiddenColUnhide = NULL;
		pEnableHiddenRowUnhide = NULL;
		pGetHiddenRowUnhide = NULL;
		pEnableColumnHide = NULL;
		pGetColumnHide = NULL;
		pEnableRowHide = NULL;
		pGetRowHide = NULL;
		pSetDefaultReadOnly = NULL;
		pIsDefaultReadOnly = NULL;
		pGetDefaultCell = NULL;
		pGetCell = NULL;
		pSetModified = NULL;
		pGetModified = NULL;
		pIsCellFixed = NULL;
		pSetItem = NULL;
		pGetItem = NULL;
		pSetItemText = NULL;
		pGetItemText = NULL;
		pSetItemTextFmt = NULL;
		pSetItemTextFmtID = NULL;
		pSetItemData = NULL;
		pGetItemData = NULL;
		pSetItemImage = NULL;
		pGetItemImage = NULL;
		pSetItemState = NULL;
		pGetItemState = NULL;
		pSetItemFormat = NULL;
		pGetItemFormat = NULL;
		pSetItemBkColour = NULL;
		pGetItemBkColour = NULL;
		pSetItemFgColour = NULL;
		pGetItemFgColour = NULL;
		pSetItemFont = NULL;
		pGetItemFont = NULL;
		pIsItemEditing = NULL;
		pSetCellType = NULL;
		pInsertColumn = NULL;
		pInsertRow = NULL;
		pDeleteColumn = NULL;
		pDeleteRow = NULL;
		pDeleteNonFixedRows = NULL;
		pDeleteAllItems = NULL;
		pClearCells = NULL;
		pAutoSizeRow = NULL;
		pAutoSizeColumn = NULL;
		pAutoSizeRows = NULL;
		pAutoSizeColumns = NULL;
		pAutoSize = NULL;
		pExpandColumnsToFit = NULL;
		pExpandLastColumn = NULL;
		pExpandRowsToFit = NULL;
		pExpandToFit = NULL;
		pRefresh = NULL;
		pAutoFill = NULL;
		pEnsureVisible = NULL;
		pEnsureVisible2 = NULL;
		pIsCellVisible = NULL;
		pIsCellVisible2 = NULL;
		pIsCellEditable = NULL;
		pIsCellEditable2 = NULL;
		pIsCellSelected = NULL;
		pIsCellSelected2 = NULL;
		pSetRedraw = NULL;
		pRedrawCell = NULL;
		pRedrawCell2 = NULL;
		pRedrawRow = NULL;
		pRedrawColumn = NULL;
		pSave = NULL;
		pLoad = NULL;
		pGetCellRange = NULL;
		pGetSelectedCellRange = NULL;
		pSetSelectedRange = NULL;
		pSetSelectedRange2 = NULL;
		pIsValid = NULL;
		pIsValid2 = NULL;
		pIsValid3 = NULL;
		pCutSelectedText = NULL;
		pCopyTextFromGrid = NULL;
		pPasteTextToGrid = NULL;
		pOnBeginDrag = NULL;
		pOnDragEnter = NULL;
		pOnDragOver = NULL;
		pOnDragLeave = NULL;
		pOnDrop = NULL;
		pOnEditCut = NULL;
		pOnEditCopy = NULL;
		pOnEditPaste = NULL;
		pOnEditSelectAll = NULL;
		pGetNextItem = NULL;
		pSortItems1 = NULL;
		pSortTextItems2 = NULL;
		pSortItems2 = NULL;
		pSetCompareFunction = NULL;
		pRegisterWindowClass = NULL;
		pInitialise = NULL;
		pSetupDefaultCells = NULL;
		pSendMessageToParent = NULL;
		pSendDisplayRequestToParent = NULL;
		pSendCacheHintToParent = NULL;
		pInvalidateCellRect = NULL;
		pInvalidateCellRect2 = NULL;
		pInvalidateCellRect3 = NULL;
		pEraseBkgnd = NULL;
		pGetCellRangeRect = NULL;
		pSetCell = NULL;
		pSetMouseMode = NULL;
		pGetMouseMode = NULL;
		pMouseOverRowResizeArea = NULL;
		pMouseOverColumnResizeArea = NULL;
		pGetTopleftNonFixedCell = NULL;
		pGetUnobstructedNonFixedCellRange = NULL;
		pGetVisibleNonFixedCellRange = NULL;
		pIsVisibleVScroll = NULL;
		pIsVisibleHScroll = NULL;
		pResetSelectedRange = NULL;
		pResetScrollBars = NULL;
		pEnableScrollBars = NULL;
		pGetScrollPos32 = NULL;
		pSetScrollPos32 = NULL;
		pSortTextItems3 = NULL;
		pSortItems3 = NULL;
		pGetPointClicked = NULL;
		pValidateAndModifyCellContents = NULL;
		pPreSubclassWindow = NULL;
		pWindowProc = NULL;
		pCreateDragImage = NULL;
		pOnFixedColumnClick = NULL;
		pOnFixedRowClick = NULL;
		pOnEditCell = NULL;
		pOnEndEditCell = NULL;
		pValidateEdit = NULL;
		pEndEditing = NULL;
		pOnDraw = NULL;
		pCreateCell = NULL;
		pDestroyCell = NULL;
		pSelectAllCells = NULL;
		pSelectColumns = NULL;
		pSelectRows = NULL;
		pSelectCells = NULL;
		pOnSelecting = NULL;
		pOnPaint = NULL;
		pOnHScroll = NULL;
		pOnVScroll = NULL;
		pOnSize = NULL;
		pOnLButtonUp = NULL;
		pOnLButtonDown = NULL;
		pOnMouseMove = NULL;
		pOnTimer = NULL;
		pOnGetDlgCode = NULL;
		pOnKeyDown = NULL;
		pOnKeyUp = NULL;
		pOnChar = NULL;
		pOnLButtonDblClk = NULL;
		pOnEraseBkgnd = NULL;
		pOnSysKeyDown = NULL;
		pOnContextMenu = NULL;
		pOnUpdateEditSelectAll = NULL;
		pOnSetCursor = NULL;
		pOnRButtonDown = NULL;
		pOnRButtonUp = NULL;
		pOnSysColorChange = NULL;
		pOnCaptureChanged = NULL;
		pOnUpdateEditCopy = NULL;
		pOnUpdateEditCut = NULL;
		pOnUpdateEditPaste = NULL;
		pOnSettingChange = NULL;
		pOnMouseWheel = NULL;
		pOnSetFont = NULL;
		pOnGetFont = NULL;
		pOnImeChar = NULL;
		pOnEndInPlaceEdit = NULL;
		pGetMouseScrollLines = NULL;
		pSetDefaultCellType = NULL;
		pSetGridScrollBarVisible = NULL;
		pSetUseFocusRect = NULL;
		pGetUseFocusRect = NULL;
		pSetDrawFixedLine = NULL;
		LoadLib();

		m_lpPointer = pInitGridCtrl(lpPointer);
	}

	virtual ~CLoaderGridCtrl() 
	{
		if(m_hLib)
			pExitGridCtrl(m_lpPointer);
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
		{
			TRACE0("CLoaderGridCtrl GetProcAddress에서 Load에러 발생");
			return FALSE;
		}

		pActionControl	= (FpActionControl)::GetProcAddress(m_hLib, "Ex_ActionControl");
		pInitGridCtrl	= (FpInitGridCtrl)::GetProcAddress(m_hLib, "Ex_InitGridCtrl");
		pExitGridCtrl	= (FpExitGridCtrl)::GetProcAddress(m_hLib, "Ex_ExitGridCtrl");
		pCreate	= (FpCreate)::GetProcAddress(m_hLib, "Ex_Create");
		pGetRowCount	= (FpGetRowCount)::GetProcAddress(m_hLib, "Ex_GetRowCount");
		pGetColumnCount	= (FpGetColumnCount)::GetProcAddress(m_hLib, "Ex_GetColumnCount");
		pGetFixedRowCount	= (FpGetFixedRowCount)::GetProcAddress(m_hLib, "Ex_GetFixedRowCount");
		pGetFixedColumnCount	= (FpGetFixedColumnCount)::GetProcAddress(m_hLib, "Ex_GetFixedColumnCount");
		pSetRowCount	= (FpSetRowCount)::GetProcAddress(m_hLib, "Ex_SetRowCount");
		pSetColumnCount	= (FpSetColumnCount)::GetProcAddress(m_hLib, "Ex_SetColumnCount");
		pSetFixedRowCount	= (FpSetFixedRowCount)::GetProcAddress(m_hLib, "Ex_SetFixedRowCount");
		pSetFixedColumnCount	= (FpSetFixedColumnCount)::GetProcAddress(m_hLib, "Ex_SetFixedColumnCount");
		pGetRowHeight	= (FpGetRowHeight)::GetProcAddress(m_hLib, "Ex_GetRowHeight");
		pSetRowHeight	= (FpSetRowHeight)::GetProcAddress(m_hLib, "Ex_SetRowHeight");
		pGetColumnWidth	= (FpGetColumnWidth)::GetProcAddress(m_hLib, "Ex_GetColumnWidth");
		pSetColumnWidth	= (FpSetColumnWidth)::GetProcAddress(m_hLib, "Ex_SetColumnWidth");
		pGetCellOrigin	= (FpGetCellOrigin)::GetProcAddress(m_hLib, "Ex_GetCellOrigin");
		pGetCellOrigin2	= (FpGetCellOrigin2)::GetProcAddress(m_hLib, "Ex_GetCellOrigin2");
		pGetCellRect	= (FpGetCellRect)::GetProcAddress(m_hLib, "Ex_GetCellRect");
		pGetCellRect2	= (FpGetCellRect2)::GetProcAddress(m_hLib, "Ex_GetCellRect2");
		pGetTextRect	= (FpGetTextRect)::GetProcAddress(m_hLib, "Ex_GetTextRect");
		pGetTextRect2	= (FpGetTextRect2)::GetProcAddress(m_hLib, "Ex_GetTextRect2");
		pGetCellFromPt	= (FpGetCellFromPt)::GetProcAddress(m_hLib, "Ex_GetCellFromPt");
		pGetFixedRowHeight	= (FpGetFixedRowHeight)::GetProcAddress(m_hLib, "Ex_GetFixedRowHeight");
		pGetFixedColumnWidth	= (FpGetFixedColumnWidth)::GetProcAddress(m_hLib, "Ex_GetFixedColumnWidth");
		pGetVirtualWidth	= (FpGetVirtualWidth)::GetProcAddress(m_hLib, "Ex_GetVirtualWidth");
		pGetVirtualHeight	= (FpGetVirtualHeight)::GetProcAddress(m_hLib, "Ex_GetVirtualHeight");
		pGetTextExtent	= (FpGetTextExtent)::GetProcAddress(m_hLib, "Ex_GetTextExtent");
		pGetCellTextExtent	= (FpGetCellTextExtent)::GetProcAddress(m_hLib, "Ex_GetCellTextExtent");
		pSetGridBkColor	= (FpSetGridBkColor)::GetProcAddress(m_hLib, "Ex_SetGridBkColor");
		pGetGridBkColor	= (FpGetGridBkColor)::GetProcAddress(m_hLib, "Ex_GetGridBkColor");
		pSetGridLineColor	= (FpSetGridLineColor)::GetProcAddress(m_hLib, "Ex_SetGridLineColor");
		pGetGridLineColor	= (FpGetGridLineColor)::GetProcAddress(m_hLib, "Ex_GetGridLineColor");
		pSetTitleTipBackClr	= (FpSetTitleTipBackClr)::GetProcAddress(m_hLib, "Ex_SetTitleTipBackClr");
		pGetTitleTipBackClr	= (FpGetTitleTipBackClr)::GetProcAddress(m_hLib, "Ex_GetTitleTipBackClr");
		pSetTitleTipTextClr	= (FpSetTitleTipTextClr)::GetProcAddress(m_hLib, "Ex_SetTitleTipTextClr");
		pGetTitleTipTextClr	= (FpGetTitleTipTextClr)::GetProcAddress(m_hLib, "Ex_GetTitleTipTextClr");
		pSetFixedGridLineColor_Light	= (FpSetFixedGridLineColor_Light)::GetProcAddress(m_hLib, "Ex_SetFixedGridLineColor_Light");
		pSetFixedGridLineColor_Dark	= (FpSetFixedGridLineColor_Dark)::GetProcAddress(m_hLib, "Ex_SetFixedGridLineColor_Dark");
		pGetFixedGridLineColor_Light	= (FpGetFixedGridLineColor_Light)::GetProcAddress(m_hLib, "Ex_GetFixedGridLineColor_Light");
		pGetFixedGridLineColor_Dark	= (FpGetFixedGridLineColor_Dark)::GetProcAddress(m_hLib, "Ex_GetFixedGridLineColor_Dark");
		pSetHighlightColor	= (FpSetHighlightColor)::GetProcAddress(m_hLib, "Ex_SetHighlightColor");
		pSetHighlightTextColor	= (FpSetHighlightTextColor)::GetProcAddress(m_hLib, "Ex_SetHighlightTextColor");
		pGetHighlightColor	= (FpGetHighlightColor)::GetProcAddress(m_hLib, "Ex_GetHighlightColor");
		pGetHighlightTextColor	= (FpGetHighlightTextColor)::GetProcAddress(m_hLib, "Ex_GetHighlightTextColor");
		pSetUseHighlightText	= (FpSetUseHighlightText)::GetProcAddress(m_hLib, "Ex_SetUseHighlightText");
		pGetUseHighlightText	= (FpGetUseHighlightText)::GetProcAddress(m_hLib, "Ex_GetUseHighlightText");
		pSetFixedRowSelectionHighlight	= (FpSetFixedRowSelectionHighlight)::GetProcAddress(m_hLib, "Ex_SetFixedRowSelectionHighlight");
		pGetFixedRowSelectionHighlight	= (FpGetFixedRowSelectionHighlight)::GetProcAddress(m_hLib, "Ex_GetFixedRowSelectionHighlight");
		pSetTextColor	= (FpSetTextColor)::GetProcAddress(m_hLib, "Ex_SetTextColor");
		pGetTextColor	= (FpGetTextColor)::GetProcAddress(m_hLib, "Ex_GetTextColor");
		pSetTextBkColor	= (FpSetTextBkColor)::GetProcAddress(m_hLib, "Ex_SetTextBkColor");
		pGetTextBkColor	= (FpGetTextBkColor)::GetProcAddress(m_hLib, "Ex_GetTextBkColor");
		pSetFixedTextColor	= (FpSetFixedTextColor)::GetProcAddress(m_hLib, "Ex_SetFixedTextColor");
		pGetFixedTextColor	= (FpGetFixedTextColor)::GetProcAddress(m_hLib, "Ex_GetFixedTextColor");
		pSetFixedBkColor	= (FpSetFixedBkColor)::GetProcAddress(m_hLib, "Ex_SetFixedBkColor");
		pGetFixedBkColor	= (FpGetFixedBkColor)::GetProcAddress(m_hLib, "Ex_GetFixedBkColor");
		pSetGridColor	= (FpSetGridColor)::GetProcAddress(m_hLib, "Ex_SetGridColor");
		pGetGridColor	= (FpGetGridColor)::GetProcAddress(m_hLib, "Ex_GetGridColor");
		pSetBkColor	= (FpSetBkColor)::GetProcAddress(m_hLib, "Ex_SetBkColor");
		pGetBkColor	= (FpGetBkColor)::GetProcAddress(m_hLib, "Ex_GetBkColor");
		pSetDefCellMargin	= (FpSetDefCellMargin)::GetProcAddress(m_hLib, "Ex_SetDefCellMargin");
		pGetDefCellMargin	= (FpGetDefCellMargin)::GetProcAddress(m_hLib, "Ex_GetDefCellMargin");
		pGetDefCellHeight	= (FpGetDefCellHeight)::GetProcAddress(m_hLib, "Ex_GetDefCellHeight");
		pSetDefCellHeight	= (FpSetDefCellHeight)::GetProcAddress(m_hLib, "Ex_SetDefCellHeight");
		pGetDefCellWidth	= (FpGetDefCellWidth)::GetProcAddress(m_hLib, "Ex_GetDefCellWidth");
		pSetDefCellWidth	= (FpSetDefCellWidth)::GetProcAddress(m_hLib, "Ex_SetDefCellWidth");
		pGetSelectedCount	= (FpGetSelectedCount)::GetProcAddress(m_hLib, "Ex_GetSelectedCount");
		pSetFocusCell1	= (FpSetFocusCell1)::GetProcAddress(m_hLib, "Ex_SetFocusCell1");
		pSetFocusCell2	= (FpSetFocusCell2)::GetProcAddress(m_hLib, "Ex_SetFocusCell2");
		pGetFocusCell	= (FpGetFocusCell)::GetProcAddress(m_hLib, "Ex_GetFocusCell");
		pSetVirtualMode	= (FpSetVirtualMode)::GetProcAddress(m_hLib, "Ex_SetVirtualMode");
		pGetVirtualMode	= (FpGetVirtualMode)::GetProcAddress(m_hLib, "Ex_GetVirtualMode");
		pSetCallbackFunc	= (FpSetCallbackFunc)::GetProcAddress(m_hLib, "Ex_SetCallbackFunc");
		pGetCallbackFunc	= (FpGetCallbackFunc)::GetProcAddress(m_hLib, "Ex_GetCallbackFunc");
		pSetImageList	= (FpSetImageList)::GetProcAddress(m_hLib, "Ex_SetImageList");
		pGetImageList	= (FpGetImageList)::GetProcAddress(m_hLib, "Ex_GetImageList");
		pSetGridLines	= (FpSetGridLines)::GetProcAddress(m_hLib, "Ex_SetGridLines");
		pGetGridLines	= (FpGetGridLines)::GetProcAddress(m_hLib, "Ex_GetGridLines");
		pSetEditable	= (FpSetEditable)::GetProcAddress(m_hLib, "Ex_SetEditable");
		pIsEditable	= (FpIsEditable)::GetProcAddress(m_hLib, "Ex_IsEditable");
		pSetListMode	= (FpSetListMode)::GetProcAddress(m_hLib, "Ex_SetListMode");
		pGetListMode	= (FpGetListMode)::GetProcAddress(m_hLib, "Ex_GetListMode");
		pSetSingleRowSelection	= (FpSetSingleRowSelection)::GetProcAddress(m_hLib, "Ex_SetSingleRowSelection");
		pGetSingleRowSelection	= (FpGetSingleRowSelection)::GetProcAddress(m_hLib, "Ex_GetSingleRowSelection");
		pSetSingleColSelection	= (FpSetSingleColSelection)::GetProcAddress(m_hLib, "Ex_SetSingleColSelection");
		pGetSingleColSelection	= (FpGetSingleColSelection)::GetProcAddress(m_hLib, "Ex_GetSingleColSelection");
		pEnableSelection	= (FpEnableSelection)::GetProcAddress(m_hLib, "Ex_EnableSelection");
		pIsSelectable	= (FpIsSelectable)::GetProcAddress(m_hLib, "Ex_IsSelectable");
		pSetFixedColumnSelection	= (FpSetFixedColumnSelection)::GetProcAddress(m_hLib, "Ex_SetFixedColumnSelection");
		pGetFixedColumnSelection	= (FpGetFixedColumnSelection)::GetProcAddress(m_hLib, "Ex_GetFixedColumnSelection");
		pSetFixedRowSelection	= (FpSetFixedRowSelection)::GetProcAddress(m_hLib, "Ex_SetFixedRowSelection");
		pGetFixedRowSelection	= (FpGetFixedRowSelection)::GetProcAddress(m_hLib, "Ex_GetFixedRowSelection");
		pEnableDragAndDrop	= (FpEnableDragAndDrop)::GetProcAddress(m_hLib, "Ex_EnableDragAndDrop");
		pGetDragAndDrop	= (FpGetDragAndDrop)::GetProcAddress(m_hLib, "Ex_GetDragAndDrop");
		pSetRowResize	= (FpSetRowResize)::GetProcAddress(m_hLib, "Ex_SetRowResize");
		pGetRowResize	= (FpGetRowResize)::GetProcAddress(m_hLib, "Ex_GetRowResize");
		pSetColumnResize	= (FpSetColumnResize)::GetProcAddress(m_hLib, "Ex_SetColumnResize");
		pGetColumnResize	= (FpGetColumnResize)::GetProcAddress(m_hLib, "Ex_GetColumnResize");
		pSetHeaderSort	= (FpSetHeaderSort)::GetProcAddress(m_hLib, "Ex_SetHeaderSort");
		pGetHeaderSort	= (FpGetHeaderSort)::GetProcAddress(m_hLib, "Ex_GetHeaderSort");
		pSetHandleTabKey	= (FpSetHandleTabKey)::GetProcAddress(m_hLib, "Ex_SetHandleTabKey");
		pGetHandleTabKey	= (FpGetHandleTabKey)::GetProcAddress(m_hLib, "Ex_GetHandleTabKey");
		pSetDoubleBuffering	= (FpSetDoubleBuffering)::GetProcAddress(m_hLib, "Ex_SetDoubleBuffering");
		pGetDoubleBuffering	= (FpGetDoubleBuffering)::GetProcAddress(m_hLib, "Ex_GetDoubleBuffering");
		pEnableTitleTips	= (FpEnableTitleTips)::GetProcAddress(m_hLib, "Ex_EnableTitleTips");
		pGetTitleTips	= (FpGetTitleTips)::GetProcAddress(m_hLib, "Ex_GetTitleTips");
		pSetSortColumn	= (FpSetSortColumn)::GetProcAddress(m_hLib, "Ex_SetSortColumn");
		pGetSortColumn	= (FpGetSortColumn)::GetProcAddress(m_hLib, "Ex_GetSortColumn");
		pSetSortAscending	= (FpSetSortAscending)::GetProcAddress(m_hLib, "Ex_SetSortAscending");
		pGetSortAscending	= (FpGetSortAscending)::GetProcAddress(m_hLib, "Ex_GetSortAscending");
		pSetTrackFocusCell	= (FpSetTrackFocusCell)::GetProcAddress(m_hLib, "Ex_SetTrackFocusCell");
		pGetTrackFocusCell	= (FpGetTrackFocusCell)::GetProcAddress(m_hLib, "Ex_GetTrackFocusCell");
		pSetFrameFocusCell	= (FpSetFrameFocusCell)::GetProcAddress(m_hLib, "Ex_SetFrameFocusCell");
		pGetFrameFocusCell	= (FpGetFrameFocusCell)::GetProcAddress(m_hLib, "Ex_GetFrameFocusCell");
		pSetAutoSizeStyle	= (FpSetAutoSizeStyle)::GetProcAddress(m_hLib, "Ex_SetAutoSizeStyle");
		pGetAutoSizeStyle	= (FpGetAutoSizeStyle)::GetProcAddress(m_hLib, "Ex_GetAutoSizeStyle");
		pEnableHiddenColUnhide	= (FpEnableHiddenColUnhide)::GetProcAddress(m_hLib, "Ex_EnableHiddenColUnhide");
		pGetHiddenColUnhide	= (FpGetHiddenColUnhide)::GetProcAddress(m_hLib, "Ex_GetHiddenColUnhide");
		pEnableHiddenRowUnhide	= (FpEnableHiddenRowUnhide)::GetProcAddress(m_hLib, "Ex_EnableHiddenRowUnhide");
		pGetHiddenRowUnhide	= (FpGetHiddenRowUnhide)::GetProcAddress(m_hLib, "Ex_GetHiddenRowUnhide");
		pEnableColumnHide	= (FpEnableColumnHide)::GetProcAddress(m_hLib, "Ex_EnableColumnHide");
		pGetColumnHide	= (FpGetColumnHide)::GetProcAddress(m_hLib, "Ex_GetColumnHide");
		pEnableRowHide	= (FpEnableRowHide)::GetProcAddress(m_hLib, "Ex_EnableRowHide");
		pGetRowHide	= (FpGetRowHide)::GetProcAddress(m_hLib, "Ex_GetRowHide");
		pSetDefaultReadOnly	= (FpSetDefaultReadOnly)::GetProcAddress(m_hLib, "Ex_SetDefaultReadOnly");
		pIsDefaultReadOnly	= (FpIsDefaultReadOnly)::GetProcAddress(m_hLib, "Ex_IsDefaultReadOnly");
		pGetDefaultCell	= (FpGetDefaultCell)::GetProcAddress(m_hLib, "Ex_GetDefaultCell");
		pGetCell	= (FpGetCell)::GetProcAddress(m_hLib, "Ex_GetCell");
		pSetModified	= (FpSetModified)::GetProcAddress(m_hLib, "Ex_SetModified");
		pGetModified	= (FpGetModified)::GetProcAddress(m_hLib, "Ex_GetModified");
		pIsCellFixed	= (FpIsCellFixed)::GetProcAddress(m_hLib, "Ex_IsCellFixed");
		pSetItem	= (FpSetItem)::GetProcAddress(m_hLib, "Ex_SetItem");
		pGetItem	= (FpGetItem)::GetProcAddress(m_hLib, "Ex_GetItem");
		pSetItemText	= (FpSetItemText)::GetProcAddress(m_hLib, "Ex_SetItemText");
		pGetItemText	= (FpGetItemText)::GetProcAddress(m_hLib, "Ex_GetItemText");
		pSetItemTextFmt	= (FpSetItemTextFmt)::GetProcAddress(m_hLib, "Ex_SetItemTextFmt");
		pSetItemTextFmtID	= (FpSetItemTextFmtID)::GetProcAddress(m_hLib, "Ex_SetItemTextFmtID");
		pSetItemData	= (FpSetItemData)::GetProcAddress(m_hLib, "Ex_SetItemData");
		pGetItemData	= (FpGetItemData)::GetProcAddress(m_hLib, "Ex_GetItemData");
		pSetItemImage	= (FpSetItemImage)::GetProcAddress(m_hLib, "Ex_SetItemImage");
		pGetItemImage	= (FpGetItemImage)::GetProcAddress(m_hLib, "Ex_GetItemImage");
		pSetItemState	= (FpSetItemState)::GetProcAddress(m_hLib, "Ex_SetItemState");
		pGetItemState	= (FpGetItemState)::GetProcAddress(m_hLib, "Ex_GetItemState");
		pSetItemFormat	= (FpSetItemFormat)::GetProcAddress(m_hLib, "Ex_SetItemFormat");
		pGetItemFormat	= (FpGetItemFormat)::GetProcAddress(m_hLib, "Ex_GetItemFormat");
		pSetItemBkColour	= (FpSetItemBkColour)::GetProcAddress(m_hLib, "Ex_SetItemBkColour");
		pGetItemBkColour	= (FpGetItemBkColour)::GetProcAddress(m_hLib, "Ex_GetItemBkColour");
		pSetItemFgColour	= (FpSetItemFgColour)::GetProcAddress(m_hLib, "Ex_SetItemFgColour");
		pGetItemFgColour	= (FpGetItemFgColour)::GetProcAddress(m_hLib, "Ex_GetItemFgColour");
		pSetItemFont	= (FpSetItemFont)::GetProcAddress(m_hLib, "Ex_SetItemFont");
		pGetItemFont	= (FpGetItemFont)::GetProcAddress(m_hLib, "Ex_GetItemFont");
		pIsItemEditing	= (FpIsItemEditing)::GetProcAddress(m_hLib, "Ex_IsItemEditing");
		pSetCellType	= (FpSetCellType)::GetProcAddress(m_hLib, "Ex_SetCellType");
		pInsertColumn	= (FpInsertColumn)::GetProcAddress(m_hLib, "Ex_InsertColumn");
		pInsertRow	= (FpInsertRow)::GetProcAddress(m_hLib, "Ex_InsertRow");
		pDeleteColumn	= (FpDeleteColumn)::GetProcAddress(m_hLib, "Ex_DeleteColumn");
		pDeleteRow	= (FpDeleteRow)::GetProcAddress(m_hLib, "Ex_DeleteRow");
		pDeleteNonFixedRows	= (FpDeleteNonFixedRows)::GetProcAddress(m_hLib, "Ex_DeleteNonFixedRows");
		pDeleteAllItems	= (FpDeleteAllItems)::GetProcAddress(m_hLib, "Ex_DeleteAllItems");
		pClearCells	= (FpClearCells)::GetProcAddress(m_hLib, "Ex_ClearCells");
		pAutoSizeRow	= (FpAutoSizeRow)::GetProcAddress(m_hLib, "Ex_AutoSizeRow");
		pAutoSizeColumn	= (FpAutoSizeColumn)::GetProcAddress(m_hLib, "Ex_AutoSizeColumn");
		pAutoSizeRows	= (FpAutoSizeRows)::GetProcAddress(m_hLib, "Ex_AutoSizeRows");
		pAutoSizeColumns	= (FpAutoSizeColumns)::GetProcAddress(m_hLib, "Ex_AutoSizeColumns");
		pAutoSize	= (FpAutoSize)::GetProcAddress(m_hLib, "Ex_AutoSize");
		pExpandColumnsToFit	= (FpExpandColumnsToFit)::GetProcAddress(m_hLib, "Ex_ExpandColumnsToFit");
		pExpandLastColumn	= (FpExpandLastColumn)::GetProcAddress(m_hLib, "Ex_ExpandLastColumn");
		pExpandRowsToFit	= (FpExpandRowsToFit)::GetProcAddress(m_hLib, "Ex_ExpandRowsToFit");
		pExpandToFit	= (FpExpandToFit)::GetProcAddress(m_hLib, "Ex_ExpandToFit");
		pRefresh	= (FpRefresh)::GetProcAddress(m_hLib, "Ex_Refresh");
		pAutoFill	= (FpAutoFill)::GetProcAddress(m_hLib, "Ex_AutoFill");
		pEnsureVisible	= (FpEnsureVisible)::GetProcAddress(m_hLib, "Ex_EnsureVisible");
		pEnsureVisible2	= (FpEnsureVisible2)::GetProcAddress(m_hLib, "Ex_EnsureVisible2");
		pIsCellVisible	= (FpIsCellVisible)::GetProcAddress(m_hLib, "Ex_IsCellVisible");
		pIsCellVisible2	= (FpIsCellVisible2)::GetProcAddress(m_hLib, "Ex_IsCellVisible2");
		pIsCellEditable	= (FpIsCellEditable)::GetProcAddress(m_hLib, "Ex_IsCellEditable");
		pIsCellEditable2	= (FpIsCellEditable2)::GetProcAddress(m_hLib, "Ex_IsCellEditable2");
		pIsCellSelected	= (FpIsCellSelected)::GetProcAddress(m_hLib, "Ex_IsCellSelected");
		pIsCellSelected2	= (FpIsCellSelected2)::GetProcAddress(m_hLib, "Ex_IsCellSelected2");
		pSetRedraw	= (FpSetRedraw)::GetProcAddress(m_hLib, "Ex_SetRedraw");
		pRedrawCell	= (FpRedrawCell)::GetProcAddress(m_hLib, "Ex_RedrawCell");
		pRedrawCell2	= (FpRedrawCell2)::GetProcAddress(m_hLib, "Ex_RedrawCell2");
		pRedrawRow	= (FpRedrawRow)::GetProcAddress(m_hLib, "Ex_RedrawRow");
		pRedrawColumn	= (FpRedrawColumn)::GetProcAddress(m_hLib, "Ex_RedrawColumn");
		pSave	= (FpSave)::GetProcAddress(m_hLib, "Ex_Save");
		pLoad	= (FpLoad)::GetProcAddress(m_hLib, "Ex_Load");
		pGetCellRange	= (FpGetCellRange)::GetProcAddress(m_hLib, "Ex_GetCellRange");
		pGetSelectedCellRange	= (FpGetSelectedCellRange)::GetProcAddress(m_hLib, "Ex_GetSelectedCellRange");
		pSetSelectedRange	= (FpSetSelectedRange)::GetProcAddress(m_hLib, "Ex_SetSelectedRange");
		pSetSelectedRange2	= (FpSetSelectedRange2)::GetProcAddress(m_hLib, "Ex_SetSelectedRange2");
		pIsValid	= (FpIsValid)::GetProcAddress(m_hLib, "Ex_IsValid");
		pIsValid2	= (FpIsValid2)::GetProcAddress(m_hLib, "Ex_IsValid2");
		pIsValid3	= (FpIsValid3)::GetProcAddress(m_hLib, "Ex_IsValid3");
		pCutSelectedText	= (FpCutSelectedText)::GetProcAddress(m_hLib, "Ex_CutSelectedText");
		pCopyTextFromGrid	= (FpCopyTextFromGrid)::GetProcAddress(m_hLib, "Ex_CopyTextFromGrid");
		pPasteTextToGrid	= (FpPasteTextToGrid)::GetProcAddress(m_hLib, "Ex_PasteTextToGrid");
		pOnBeginDrag	= (FpOnBeginDrag)::GetProcAddress(m_hLib, "Ex_OnBeginDrag");
		pOnDragEnter	= (FpOnDragEnter)::GetProcAddress(m_hLib, "Ex_OnDragEnter");
		pOnDragOver	= (FpOnDragOver)::GetProcAddress(m_hLib, "Ex_OnDragOver");
		pOnDragLeave	= (FpOnDragLeave)::GetProcAddress(m_hLib, "Ex_OnDragLeave");
		pOnDrop	= (FpOnDrop)::GetProcAddress(m_hLib, "Ex_OnDrop");
		pOnEditCut	= (FpOnEditCut)::GetProcAddress(m_hLib, "Ex_OnEditCut");
		pOnEditCopy	= (FpOnEditCopy)::GetProcAddress(m_hLib, "Ex_OnEditCopy");
		pOnEditPaste	= (FpOnEditPaste)::GetProcAddress(m_hLib, "Ex_OnEditPaste");
		pOnEditSelectAll	= (FpOnEditSelectAll)::GetProcAddress(m_hLib, "Ex_OnEditSelectAll");
		pGetNextItem	= (FpGetNextItem)::GetProcAddress(m_hLib, "Ex_GetNextItem");
		pSortItems1	= (FpSortItems1)::GetProcAddress(m_hLib, "Ex_SortItems1");
		pSortTextItems2	= (FpSortTextItems2)::GetProcAddress(m_hLib, "Ex_SortTextItems2");
		pSortItems2	= (FpSortItems2)::GetProcAddress(m_hLib, "Ex_SortItems2");
		pSetCompareFunction	= (FpSetCompareFunction)::GetProcAddress(m_hLib, "Ex_SetCompareFunction");
		pRegisterWindowClass	= (FpRegisterWindowClass)::GetProcAddress(m_hLib, "Ex_RegisterWindowClass");
		pInitialise	= (FpInitialise)::GetProcAddress(m_hLib, "Ex_Initialise");
		pSetupDefaultCells	= (FpSetupDefaultCells)::GetProcAddress(m_hLib, "Ex_SetupDefaultCells");
		pSendMessageToParent	= (FpSendMessageToParent)::GetProcAddress(m_hLib, "Ex_SendMessageToParent");
		pSendDisplayRequestToParent	= (FpSendDisplayRequestToParent)::GetProcAddress(m_hLib, "Ex_SendDisplayRequestToParent");
		pSendCacheHintToParent	= (FpSendCacheHintToParent)::GetProcAddress(m_hLib, "Ex_SendCacheHintToParent");
		pInvalidateCellRect	= (FpInvalidateCellRect)::GetProcAddress(m_hLib, "Ex_InvalidateCellRect");
		pInvalidateCellRect2	= (FpInvalidateCellRect2)::GetProcAddress(m_hLib, "Ex_InvalidateCellRect2");
		pInvalidateCellRect3	= (FpInvalidateCellRect3)::GetProcAddress(m_hLib, "Ex_InvalidateCellRect3");
		pEraseBkgnd	= (FpEraseBkgnd)::GetProcAddress(m_hLib, "Ex_EraseBkgnd");
		pGetCellRangeRect	= (FpGetCellRangeRect)::GetProcAddress(m_hLib, "Ex_GetCellRangeRect");
		pSetCell	= (FpSetCell)::GetProcAddress(m_hLib, "Ex_SetCell");
		pSetMouseMode	= (FpSetMouseMode)::GetProcAddress(m_hLib, "Ex_SetMouseMode");
		pGetMouseMode	= (FpGetMouseMode)::GetProcAddress(m_hLib, "Ex_GetMouseMode");
		pMouseOverRowResizeArea	= (FpMouseOverRowResizeArea)::GetProcAddress(m_hLib, "Ex_MouseOverRowResizeArea");
		pMouseOverColumnResizeArea	= (FpMouseOverColumnResizeArea)::GetProcAddress(m_hLib, "Ex_MouseOverColumnResizeArea");
		pGetTopleftNonFixedCell	= (FpGetTopleftNonFixedCell)::GetProcAddress(m_hLib, "Ex_GetTopleftNonFixedCell");
		pGetUnobstructedNonFixedCellRange	= (FpGetUnobstructedNonFixedCellRange)::GetProcAddress(m_hLib, "Ex_GetUnobstructedNonFixedCellRange");
		pGetVisibleNonFixedCellRange	= (FpGetVisibleNonFixedCellRange)::GetProcAddress(m_hLib, "Ex_GetVisibleNonFixedCellRange");
		pIsVisibleVScroll	= (FpIsVisibleVScroll)::GetProcAddress(m_hLib, "Ex_IsVisibleVScroll");
		pIsVisibleHScroll	= (FpIsVisibleHScroll)::GetProcAddress(m_hLib, "Ex_IsVisibleHScroll");
		pResetSelectedRange	= (FpResetSelectedRange)::GetProcAddress(m_hLib, "Ex_ResetSelectedRange");
		pResetScrollBars	= (FpResetScrollBars)::GetProcAddress(m_hLib, "Ex_ResetScrollBars");
		pEnableScrollBars	= (FpEnableScrollBars)::GetProcAddress(m_hLib, "Ex_EnableScrollBars");
		pGetScrollPos32	= (FpGetScrollPos32)::GetProcAddress(m_hLib, "Ex_GetScrollPos32");
		pSetScrollPos32	= (FpSetScrollPos32)::GetProcAddress(m_hLib, "Ex_SetScrollPos32");
		pSortTextItems3	= (FpSortTextItems3)::GetProcAddress(m_hLib, "Ex_SortTextItems3");
		pSortItems3	= (FpSortItems3)::GetProcAddress(m_hLib, "Ex_SortItems3");
		pGetPointClicked	= (FpGetPointClicked)::GetProcAddress(m_hLib, "Ex_GetPointClicked");
		pValidateAndModifyCellContents	= (FpValidateAndModifyCellContents)::GetProcAddress(m_hLib, "Ex_ValidateAndModifyCellContents");
		pPreSubclassWindow	= (FpPreSubclassWindow)::GetProcAddress(m_hLib, "Ex_PreSubclassWindow");
		pWindowProc	= (FpWindowProc)::GetProcAddress(m_hLib, "Ex_WindowProc");
		pCreateDragImage	= (FpCreateDragImage)::GetProcAddress(m_hLib, "Ex_CreateDragImage");
		pOnFixedColumnClick	= (FpOnFixedColumnClick)::GetProcAddress(m_hLib, "Ex_OnFixedColumnClick");
		pOnFixedRowClick	= (FpOnFixedRowClick)::GetProcAddress(m_hLib, "Ex_OnFixedRowClick");
		pOnEditCell	= (FpOnEditCell)::GetProcAddress(m_hLib, "Ex_OnEditCell");
		pOnEndEditCell	= (FpOnEndEditCell)::GetProcAddress(m_hLib, "Ex_OnEndEditCell");
		pValidateEdit	= (FpValidateEdit)::GetProcAddress(m_hLib, "Ex_ValidateEdit");
		pEndEditing	= (FpEndEditing)::GetProcAddress(m_hLib, "Ex_EndEditing");
		pOnDraw	= (FpOnDraw)::GetProcAddress(m_hLib, "Ex_OnDraw");
		pCreateCell	= (FpCreateCell)::GetProcAddress(m_hLib, "Ex_CreateCell");
		pDestroyCell	= (FpDestroyCell)::GetProcAddress(m_hLib, "Ex_DestroyCell");
		pSelectAllCells	= (FpSelectAllCells)::GetProcAddress(m_hLib, "Ex_SelectAllCells");
		pSelectColumns	= (FpSelectColumns)::GetProcAddress(m_hLib, "Ex_SelectColumns");
		pSelectRows	= (FpSelectRows)::GetProcAddress(m_hLib, "Ex_SelectRows");
		pSelectCells	= (FpSelectCells)::GetProcAddress(m_hLib, "Ex_SelectCells");
		pOnSelecting	= (FpOnSelecting)::GetProcAddress(m_hLib, "Ex_OnSelecting");
		pOnPaint	= (FpOnPaint)::GetProcAddress(m_hLib, "Ex_OnPaint");
		pOnHScroll	= (FpOnHScroll)::GetProcAddress(m_hLib, "Ex_OnHScroll");
		pOnVScroll	= (FpOnVScroll)::GetProcAddress(m_hLib, "Ex_OnVScroll");
		pOnSize	= (FpOnSize)::GetProcAddress(m_hLib, "Ex_OnSize");
		pOnLButtonUp	= (FpOnLButtonUp)::GetProcAddress(m_hLib, "Ex_OnLButtonUp");
		pOnLButtonDown	= (FpOnLButtonDown)::GetProcAddress(m_hLib, "Ex_OnLButtonDown");
		pOnMouseMove	= (FpOnMouseMove)::GetProcAddress(m_hLib, "Ex_OnMouseMove");
		pOnTimer	= (FpOnTimer)::GetProcAddress(m_hLib, "Ex_OnTimer");
		pOnGetDlgCode	= (FpOnGetDlgCode)::GetProcAddress(m_hLib, "Ex_OnGetDlgCode");
		pOnKeyDown	= (FpOnKeyDown)::GetProcAddress(m_hLib, "Ex_OnKeyDown");
		pOnKeyUp	= (FpOnKeyUp)::GetProcAddress(m_hLib, "Ex_OnKeyUp");
		pOnChar	= (FpOnChar)::GetProcAddress(m_hLib, "Ex_OnChar");
		pOnLButtonDblClk	= (FpOnLButtonDblClk)::GetProcAddress(m_hLib, "Ex_OnLButtonDblClk");
		pOnEraseBkgnd	= (FpOnEraseBkgnd)::GetProcAddress(m_hLib, "Ex_OnEraseBkgnd");
		pOnSysKeyDown	= (FpOnSysKeyDown)::GetProcAddress(m_hLib, "Ex_OnSysKeyDown");
		pOnContextMenu	= (FpOnContextMenu)::GetProcAddress(m_hLib, "Ex_OnContextMenu");
		pOnUpdateEditSelectAll	= (FpOnUpdateEditSelectAll)::GetProcAddress(m_hLib, "Ex_OnUpdateEditSelectAll");
		pOnSetCursor	= (FpOnSetCursor)::GetProcAddress(m_hLib, "Ex_OnSetCursor");
		pOnRButtonDown	= (FpOnRButtonDown)::GetProcAddress(m_hLib, "Ex_OnRButtonDown");
		pOnRButtonUp	= (FpOnRButtonUp)::GetProcAddress(m_hLib, "Ex_OnRButtonUp");
		pOnSysColorChange	= (FpOnSysColorChange)::GetProcAddress(m_hLib, "Ex_OnSysColorChange");
		pOnCaptureChanged	= (FpOnCaptureChanged)::GetProcAddress(m_hLib, "Ex_OnCaptureChanged");
		pOnUpdateEditCopy	= (FpOnUpdateEditCopy)::GetProcAddress(m_hLib, "Ex_OnUpdateEditCopy");
		pOnUpdateEditCut	= (FpOnUpdateEditCut)::GetProcAddress(m_hLib, "Ex_OnUpdateEditCut");
		pOnUpdateEditPaste	= (FpOnUpdateEditPaste)::GetProcAddress(m_hLib, "Ex_OnUpdateEditPaste");
		pOnSettingChange	= (FpOnSettingChange)::GetProcAddress(m_hLib, "Ex_OnSettingChange");
		pOnMouseWheel	= (FpOnMouseWheel)::GetProcAddress(m_hLib, "Ex_OnMouseWheel");
		pOnSetFont	= (FpOnSetFont)::GetProcAddress(m_hLib, "Ex_OnSetFont");
		pOnGetFont	= (FpOnGetFont)::GetProcAddress(m_hLib, "Ex_OnGetFont");
		pOnImeChar	= (FpOnImeChar)::GetProcAddress(m_hLib, "Ex_OnImeChar");
		pOnEndInPlaceEdit	= (FpOnEndInPlaceEdit)::GetProcAddress(m_hLib, "Ex_OnEndInPlaceEdit");
		pGetMouseScrollLines	= (FpGetMouseScrollLines)::GetProcAddress(m_hLib, "Ex_GetMouseScrollLines");
		pSetDefaultCellType	= (FpSetDefaultCellType)::GetProcAddress(m_hLib, "Ex_SetDefaultCellType");
		pSetGridScrollBarVisible	= (FpSetGridScrollBarVisible)::GetProcAddress(m_hLib, "Ex_SetGridScrollBarVisible");
		pSetUseFocusRect	= (FpSetUseFocusRect)::GetProcAddress(m_hLib, "Ex_SetUseFocusRect");
		pGetUseFocusRect	= (FpGetUseFocusRect)::GetProcAddress(m_hLib, "Ex_GetUseFocusRect");
		pSetDrawFixedLine	= (FpSetDrawFixedLine)::GetProcAddress(m_hLib, "Ex_SetDrawFixedLine");

		return TRUE;
	}

public:
	// Implementation
	LPVOID				m_lpPointer;

	FpActionControl		pActionControl;
	FpInitGridCtrl			pInitGridCtrl;
	FpExitGridCtrl			pExitGridCtrl;
	FpCreate			pCreate;
	FpGetRowCount		pGetRowCount;
	FpGetColumnCount		pGetColumnCount;
	FpGetFixedRowCount		pGetFixedRowCount;
	FpGetFixedColumnCount		pGetFixedColumnCount;
	FpSetRowCount		pSetRowCount;
	FpSetColumnCount		pSetColumnCount;
	FpSetFixedRowCount		pSetFixedRowCount;
	FpSetFixedColumnCount		pSetFixedColumnCount;
	FpGetRowHeight		pGetRowHeight;
	FpSetRowHeight		pSetRowHeight;
	FpGetColumnWidth		pGetColumnWidth;
	FpSetColumnWidth		pSetColumnWidth;
	FpGetCellOrigin		pGetCellOrigin;
	FpGetCellOrigin2		pGetCellOrigin2;
	FpGetCellRect		pGetCellRect;
	FpGetCellRect2		pGetCellRect2;
	FpGetTextRect		pGetTextRect;
	FpGetTextRect2		pGetTextRect2;
	FpGetCellFromPt		pGetCellFromPt;
	FpGetFixedRowHeight		pGetFixedRowHeight;
	FpGetFixedColumnWidth		pGetFixedColumnWidth;
	FpGetVirtualWidth		pGetVirtualWidth;
	FpGetVirtualHeight		pGetVirtualHeight;
	FpGetTextExtent		pGetTextExtent;
	FpGetCellTextExtent		pGetCellTextExtent;
	FpSetGridBkColor		pSetGridBkColor;
	FpGetGridBkColor		pGetGridBkColor;
	FpSetGridLineColor		pSetGridLineColor;
	FpGetGridLineColor		pGetGridLineColor;
	FpSetTitleTipBackClr		pSetTitleTipBackClr;
	FpGetTitleTipBackClr		pGetTitleTipBackClr;
	FpSetTitleTipTextClr		pSetTitleTipTextClr;
	FpGetTitleTipTextClr		pGetTitleTipTextClr;
	FpSetFixedGridLineColor_Light		pSetFixedGridLineColor_Light;
	FpSetFixedGridLineColor_Dark		pSetFixedGridLineColor_Dark;
	FpGetFixedGridLineColor_Light		pGetFixedGridLineColor_Light;
	FpGetFixedGridLineColor_Dark		pGetFixedGridLineColor_Dark;
	FpSetHighlightColor		pSetHighlightColor;
	FpSetHighlightTextColor		pSetHighlightTextColor;
	FpGetHighlightColor		pGetHighlightColor;
	FpGetHighlightTextColor		pGetHighlightTextColor;
	FpSetUseHighlightText		pSetUseHighlightText;
	FpGetUseHighlightText		pGetUseHighlightText;
	FpSetFixedRowSelectionHighlight		pSetFixedRowSelectionHighlight;
	FpGetFixedRowSelectionHighlight		pGetFixedRowSelectionHighlight;
	FpSetTextColor		pSetTextColor;
	FpGetTextColor		pGetTextColor;
	FpSetTextBkColor		pSetTextBkColor;
	FpGetTextBkColor		pGetTextBkColor;
	FpSetFixedTextColor		pSetFixedTextColor;
	FpGetFixedTextColor		pGetFixedTextColor;
	FpSetFixedBkColor		pSetFixedBkColor;
	FpGetFixedBkColor		pGetFixedBkColor;
	FpSetGridColor		pSetGridColor;
	FpGetGridColor		pGetGridColor;
	FpSetBkColor		pSetBkColor;
	FpGetBkColor		pGetBkColor;
	FpSetDefCellMargin		pSetDefCellMargin;
	FpGetDefCellMargin		pGetDefCellMargin;
	FpGetDefCellHeight		pGetDefCellHeight;
	FpSetDefCellHeight		pSetDefCellHeight;
	FpGetDefCellWidth		pGetDefCellWidth;
	FpSetDefCellWidth		pSetDefCellWidth;
	FpGetSelectedCount		pGetSelectedCount;
	FpSetFocusCell1		pSetFocusCell1;
	FpSetFocusCell2		pSetFocusCell2;
	FpGetFocusCell		pGetFocusCell;
	FpSetVirtualMode		pSetVirtualMode;
	FpGetVirtualMode		pGetVirtualMode;
	FpSetCallbackFunc		pSetCallbackFunc;
	FpGetCallbackFunc		pGetCallbackFunc;
	FpSetImageList		pSetImageList;
	FpGetImageList		pGetImageList;
	FpSetGridLines		pSetGridLines;
	FpGetGridLines		pGetGridLines;
	FpSetEditable		pSetEditable;
	FpIsEditable		pIsEditable;
	FpSetListMode		pSetListMode;
	FpGetListMode		pGetListMode;
	FpSetSingleRowSelection		pSetSingleRowSelection;
	FpGetSingleRowSelection		pGetSingleRowSelection;
	FpSetSingleColSelection		pSetSingleColSelection;
	FpGetSingleColSelection		pGetSingleColSelection;
	FpEnableSelection		pEnableSelection;
	FpIsSelectable		pIsSelectable;
	FpSetFixedColumnSelection		pSetFixedColumnSelection;
	FpGetFixedColumnSelection		pGetFixedColumnSelection;
	FpSetFixedRowSelection		pSetFixedRowSelection;
	FpGetFixedRowSelection		pGetFixedRowSelection;
	FpEnableDragAndDrop		pEnableDragAndDrop;
	FpGetDragAndDrop		pGetDragAndDrop;
	FpSetRowResize		pSetRowResize;
	FpGetRowResize		pGetRowResize;
	FpSetColumnResize		pSetColumnResize;
	FpGetColumnResize		pGetColumnResize;
	FpSetHeaderSort		pSetHeaderSort;
	FpGetHeaderSort		pGetHeaderSort;
	FpSetHandleTabKey		pSetHandleTabKey;
	FpGetHandleTabKey		pGetHandleTabKey;
	FpSetDoubleBuffering		pSetDoubleBuffering;
	FpGetDoubleBuffering		pGetDoubleBuffering;
	FpEnableTitleTips		pEnableTitleTips;
	FpGetTitleTips		pGetTitleTips;
	FpSetSortColumn		pSetSortColumn;
	FpGetSortColumn		pGetSortColumn;
	FpSetSortAscending		pSetSortAscending;
	FpGetSortAscending		pGetSortAscending;
	FpSetTrackFocusCell		pSetTrackFocusCell;
	FpGetTrackFocusCell		pGetTrackFocusCell;
	FpSetFrameFocusCell		pSetFrameFocusCell;
	FpGetFrameFocusCell		pGetFrameFocusCell;
	FpSetAutoSizeStyle		pSetAutoSizeStyle;
	FpGetAutoSizeStyle		pGetAutoSizeStyle;
	FpEnableHiddenColUnhide		pEnableHiddenColUnhide;
	FpGetHiddenColUnhide		pGetHiddenColUnhide;
	FpEnableHiddenRowUnhide		pEnableHiddenRowUnhide;
	FpGetHiddenRowUnhide		pGetHiddenRowUnhide;
	FpEnableColumnHide		pEnableColumnHide;
	FpGetColumnHide		pGetColumnHide;
	FpEnableRowHide		pEnableRowHide;
	FpGetRowHide		pGetRowHide;
	FpSetDefaultReadOnly		pSetDefaultReadOnly;
	FpIsDefaultReadOnly		pIsDefaultReadOnly;
	FpGetDefaultCell		pGetDefaultCell;
	FpGetCell		pGetCell;
	FpSetModified		pSetModified;
	FpGetModified		pGetModified;
	FpIsCellFixed		pIsCellFixed;
	FpSetItem		pSetItem;
	FpGetItem		pGetItem;
	FpSetItemText		pSetItemText;
	FpGetItemText		pGetItemText;
	FpSetItemTextFmt		pSetItemTextFmt;
	FpSetItemTextFmtID		pSetItemTextFmtID;
	FpSetItemData		pSetItemData;
	FpGetItemData		pGetItemData;
	FpSetItemImage		pSetItemImage;
	FpGetItemImage		pGetItemImage;
	FpSetItemState		pSetItemState;
	FpGetItemState		pGetItemState;
	FpSetItemFormat		pSetItemFormat;
	FpGetItemFormat		pGetItemFormat;
	FpSetItemBkColour		pSetItemBkColour;
	FpGetItemBkColour		pGetItemBkColour;
	FpSetItemFgColour		pSetItemFgColour;
	FpGetItemFgColour		pGetItemFgColour;
	FpSetItemFont		pSetItemFont;
	FpGetItemFont		pGetItemFont;
	FpIsItemEditing		pIsItemEditing;
	FpSetCellType		pSetCellType;
	FpInsertColumn		pInsertColumn;
	FpInsertRow		pInsertRow;
	FpDeleteColumn		pDeleteColumn;
	FpDeleteRow		pDeleteRow;
	FpDeleteNonFixedRows		pDeleteNonFixedRows;
	FpDeleteAllItems		pDeleteAllItems;
	FpClearCells		pClearCells;
	FpAutoSizeRow		pAutoSizeRow;
	FpAutoSizeColumn		pAutoSizeColumn;
	FpAutoSizeRows		pAutoSizeRows;
	FpAutoSizeColumns		pAutoSizeColumns;
	FpAutoSize		pAutoSize;
	FpExpandColumnsToFit		pExpandColumnsToFit;
	FpExpandLastColumn		pExpandLastColumn;
	FpExpandRowsToFit		pExpandRowsToFit;
	FpExpandToFit		pExpandToFit;
	FpRefresh		pRefresh;
	FpAutoFill		pAutoFill;
	FpEnsureVisible		pEnsureVisible;
	FpEnsureVisible2		pEnsureVisible2;
	FpIsCellVisible		pIsCellVisible;
	FpIsCellVisible2		pIsCellVisible2;
	FpIsCellEditable		pIsCellEditable;
	FpIsCellEditable2		pIsCellEditable2;
	FpIsCellSelected		pIsCellSelected;
	FpIsCellSelected2		pIsCellSelected2;
	FpSetRedraw		pSetRedraw;
	FpRedrawCell		pRedrawCell;
	FpRedrawCell2		pRedrawCell2;
	FpRedrawRow		pRedrawRow;
	FpRedrawColumn		pRedrawColumn;
	FpSave		pSave;
	FpLoad		pLoad;
	FpGetCellRange		pGetCellRange;
	FpGetSelectedCellRange		pGetSelectedCellRange;
	FpSetSelectedRange		pSetSelectedRange;
	FpSetSelectedRange2		pSetSelectedRange2;
	FpIsValid		pIsValid;
	FpIsValid2		pIsValid2;
	FpIsValid3		pIsValid3;
	FpCutSelectedText		pCutSelectedText;
	FpCopyTextFromGrid		pCopyTextFromGrid;
	FpPasteTextToGrid		pPasteTextToGrid;
	FpOnBeginDrag		pOnBeginDrag;
	FpOnDragEnter		pOnDragEnter;
	FpOnDragOver		pOnDragOver;
	FpOnDragLeave		pOnDragLeave;
	FpOnDrop		pOnDrop;
	FpOnEditCut		pOnEditCut;
	FpOnEditCopy		pOnEditCopy;
	FpOnEditPaste		pOnEditPaste;
	FpOnEditSelectAll		pOnEditSelectAll;
	FpGetNextItem		pGetNextItem;
	FpSortItems1		pSortItems1;
	FpSortTextItems2		pSortTextItems2;
	FpSortItems2		pSortItems2;
	FpSetCompareFunction		pSetCompareFunction;
	FpRegisterWindowClass		pRegisterWindowClass;
	FpInitialise		pInitialise;
	FpSetupDefaultCells		pSetupDefaultCells;
	FpSendMessageToParent		pSendMessageToParent;
	FpSendDisplayRequestToParent		pSendDisplayRequestToParent;
	FpSendCacheHintToParent		pSendCacheHintToParent;
	FpInvalidateCellRect		pInvalidateCellRect;
	FpInvalidateCellRect2		pInvalidateCellRect2;
	FpInvalidateCellRect3		pInvalidateCellRect3;
	FpEraseBkgnd		pEraseBkgnd;
	FpGetCellRangeRect		pGetCellRangeRect;
	FpSetCell		pSetCell;
	FpSetMouseMode		pSetMouseMode;
	FpGetMouseMode		pGetMouseMode;
	FpMouseOverRowResizeArea		pMouseOverRowResizeArea;
	FpMouseOverColumnResizeArea		pMouseOverColumnResizeArea;
	FpGetTopleftNonFixedCell		pGetTopleftNonFixedCell;
	FpGetUnobstructedNonFixedCellRange		pGetUnobstructedNonFixedCellRange;
	FpGetVisibleNonFixedCellRange		pGetVisibleNonFixedCellRange;
	FpIsVisibleVScroll		pIsVisibleVScroll;
	FpIsVisibleHScroll		pIsVisibleHScroll;
	FpResetSelectedRange		pResetSelectedRange;
	FpResetScrollBars		pResetScrollBars;
	FpEnableScrollBars		pEnableScrollBars;
	FpGetScrollPos32		pGetScrollPos32;
	FpSetScrollPos32		pSetScrollPos32;
	FpSortTextItems3		pSortTextItems3;
	FpSortItems3		pSortItems3;
	FpGetPointClicked		pGetPointClicked;
	FpValidateAndModifyCellContents		pValidateAndModifyCellContents;
	FpPreSubclassWindow		pPreSubclassWindow;
	FpWindowProc		pWindowProc;
	FpCreateDragImage		pCreateDragImage;
	FpOnFixedColumnClick		pOnFixedColumnClick;
	FpOnFixedRowClick		pOnFixedRowClick;
	FpOnEditCell		pOnEditCell;
	FpOnEndEditCell		pOnEndEditCell;
	FpValidateEdit		pValidateEdit;
	FpEndEditing		pEndEditing;
	FpOnDraw		pOnDraw;
	FpCreateCell		pCreateCell;
	FpDestroyCell		pDestroyCell;
	FpSelectAllCells		pSelectAllCells;
	FpSelectColumns		pSelectColumns;
	FpSelectRows		pSelectRows;
	FpSelectCells		pSelectCells;
	FpOnSelecting		pOnSelecting;
	FpOnPaint		pOnPaint;
	FpOnHScroll		pOnHScroll;
	FpOnVScroll		pOnVScroll;
	FpOnSize		pOnSize;
	FpOnLButtonUp		pOnLButtonUp;
	FpOnLButtonDown		pOnLButtonDown;
	FpOnMouseMove		pOnMouseMove;
	FpOnTimer		pOnTimer;
	FpOnGetDlgCode		pOnGetDlgCode;
	FpOnKeyDown		pOnKeyDown;
	FpOnKeyUp		pOnKeyUp;
	FpOnChar		pOnChar;
	FpOnLButtonDblClk		pOnLButtonDblClk;
	FpOnEraseBkgnd		pOnEraseBkgnd;
	FpOnSysKeyDown		pOnSysKeyDown;
	FpOnContextMenu		pOnContextMenu;
	FpOnUpdateEditSelectAll		pOnUpdateEditSelectAll;
	FpOnSetCursor		pOnSetCursor;
	FpOnRButtonDown		pOnRButtonDown;
	FpOnRButtonUp		pOnRButtonUp;
	FpOnSysColorChange		pOnSysColorChange;
	FpOnCaptureChanged		pOnCaptureChanged;
	FpOnUpdateEditCopy		pOnUpdateEditCopy;
	FpOnUpdateEditCut		pOnUpdateEditCut;
	FpOnUpdateEditPaste		pOnUpdateEditPaste;
	FpOnSettingChange		pOnSettingChange;
	FpOnMouseWheel		pOnMouseWheel;
	FpOnSetFont		pOnSetFont;
	FpOnGetFont		pOnGetFont;
	FpOnImeChar		pOnImeChar;
	FpOnEndInPlaceEdit		pOnEndInPlaceEdit;
	FpGetMouseScrollLines		pGetMouseScrollLines;
	FpSetDefaultCellType		pSetDefaultCellType;
	FpSetGridScrollBarVisible		pSetGridScrollBarVisible;
	FpSetUseFocusRect		pSetUseFocusRect;
	FpGetUseFocusRect		pGetUseFocusRect;
	FpSetDrawFixedLine		pSetDrawFixedLine;

public:
	// Function
	long ActionControl(LPVOID lpAction)
	{
		if(!m_hLib)
			return 0L;

		return pActionControl(m_lpPointer, lpAction);
	}

	BOOL Create(RECT& rect, CWnd* parent, UINT nID, DWORD dwStyle)
	{
		if(!m_hLib)
			return FALSE;

		return pCreate(m_lpPointer, rect, parent, nID, dwStyle);
	}

	int GetRowCount()
	{
		if(!m_hLib)
			return -1;

		return pGetRowCount(m_lpPointer);
	}

	int GetColumnCount()
	{
		if(!m_hLib)
			return -1;

		return pGetColumnCount(m_lpPointer);
	}

	int GetFixedRowCount()
	{
		if(!m_hLib)
			return -1;

		return pGetFixedRowCount(m_lpPointer);
	}

	int GetFixedColumnCount()
	{
		if(!m_hLib)
			return -1;

		return pGetFixedColumnCount(m_lpPointer);
	}

	BOOL SetRowCount(int nRows)
	{
		if(!m_hLib)
			return FALSE;

		return pSetRowCount(m_lpPointer, nRows);
	}

	BOOL SetColumnCount(int nCols)
	{
		if(!m_hLib)
			return FALSE;

		return pSetColumnCount(m_lpPointer, nCols);
	}

	BOOL SetFixedRowCount(int nFixedRows)
	{
		if(!m_hLib)
			return FALSE;

		return pSetFixedRowCount(m_lpPointer, nFixedRows);
	}

	BOOL SetFixedColumnCount(int nFixedCols)
	{
		if(!m_hLib)
			return FALSE;

		return pSetFixedColumnCount(m_lpPointer, nFixedCols);
	}

	int GetRowHeight(int nRow)
	{
		if(!m_hLib)
			return -1;

		return pGetRowHeight(m_lpPointer, nRow);
	}

	BOOL SetRowHeight(int row, int height)
	{
		if(!m_hLib)
			return FALSE;

		return pSetRowHeight(m_lpPointer, row, height);
	}

	int GetColumnWidth(int nCol)
	{
		if(!m_hLib)
			return -1;

		return pGetColumnWidth(m_lpPointer, nCol);
	}

	BOOL SetColumnWidth(int col, int width)
	{
		if(!m_hLib)
			return FALSE;

		return pSetColumnWidth(m_lpPointer, col, width);
	}

	BOOL GetCellOrigin(int nRow, int nCol, LPPOINT p)
	{
		if(!m_hLib)
			return FALSE;

		return pGetCellOrigin(m_lpPointer, nRow, nCol, p);
	}

	BOOL GetCellOrigin(CCellID& cell, LPPOINT p)
	{
		if(!m_hLib)
			return FALSE;

		return pGetCellOrigin2(m_lpPointer, cell, p);
	}

	BOOL GetCellRect(int nRow, int nCol, LPRECT pRect)
	{
		if(!m_hLib)
			return FALSE;

		return pGetCellRect(m_lpPointer, nRow, nCol, pRect);
	}

	BOOL GetCellRect(CCellID& cell, LPRECT pRect)
	{
		if(!m_hLib)
			return FALSE;

		return pGetCellRect2(m_lpPointer, cell, pRect);
	}

	BOOL GetTextRect(CCellID& cell, LPRECT pRect)
	{
		if(!m_hLib)
			return FALSE;

		return pGetTextRect(m_lpPointer, cell, pRect);
	}

	BOOL GetTextRect(int nRow, int nCol, LPRECT pRect)
	{
		if(!m_hLib)
			return FALSE;

		return pGetTextRect2(m_lpPointer, nRow, nCol, pRect);
	}

	CCellID	GetCellFromPt(CPoint point, BOOL bAllowFixedCellCheck)
	{
		if(!m_hLib)
			return CCellID();
//			return NULL;

		return pGetCellFromPt(m_lpPointer, point, bAllowFixedCellCheck);
	}

	int GetFixedRowHeight()
	{
		if(!m_hLib)
			return -1;

		return pGetFixedRowHeight(m_lpPointer);
	}

	int GetFixedColumnWidth()
	{
		if(!m_hLib)
			return -1;

		return pGetFixedColumnWidth(m_lpPointer);
	}

	long GetVirtualWidth()
	{
		if(!m_hLib)
			return 0L;

		return pGetVirtualWidth(m_lpPointer);
	}

	long GetVirtualHeight()
	{
		if(!m_hLib)
			return 0L;

		return pGetVirtualHeight(m_lpPointer);
	}

	CSize GetTextExtent(int nRow, int nCol, LPCTSTR str)
	{
		if(!m_hLib)
			return CSize(0);

		return pGetTextExtent(m_lpPointer, nRow, nCol, str);
	}

	CSize GetCellTextExtent(int nRow, int nCol)
	{
		if(!m_hLib)
			return CSize(0);

		return pGetCellTextExtent(m_lpPointer, nRow, nCol);
	}

	void SetGridBkColor(COLORREF clr)
	{
		if(!m_hLib)
			return;

		pSetGridBkColor(m_lpPointer, clr);
	}

	COLORREF GetGridBkColor()
	{
		if(!m_hLib)
			return 0x000000;

		return pGetGridBkColor(m_lpPointer);
	}

	void SetGridLineColor(COLORREF clr)
	{
		if(!m_hLib)
			return;

		pSetGridLineColor(m_lpPointer, clr);
	}

	COLORREF GetGridLineColor()
	{
		if(!m_hLib)
			return 0x000000;

		return pGetGridLineColor(m_lpPointer);
	}

	void SetTitleTipBackClr(COLORREF clr)
	{
		if(!m_hLib)
			return;

		pSetTitleTipBackClr(m_lpPointer, clr);
	}

	COLORREF GetTitleTipBackClr()
	{
		if(!m_hLib)
			return 0x000000;
		
		return pGetTitleTipBackClr(m_lpPointer);
	}

	void SetTitleTipTextClr(COLORREF clr)
	{
		if(!m_hLib)
			return;

		pSetTitleTipTextClr(m_lpPointer, clr);
	}

	COLORREF GetTitleTipTextClr()
	{
		if(!m_hLib)
			return 0x000000;
		
		return pGetTitleTipTextClr(m_lpPointer);
	}

	void SetFixedGridLineColor_Light(COLORREF clr)
	{
		if(!m_hLib)
			return;

		pSetFixedGridLineColor_Light(m_lpPointer, clr);
	}

	void SetFixedGridLineColor_Dark(COLORREF clr)
	{
		if(!m_hLib)
			return;

		pSetFixedGridLineColor_Dark(m_lpPointer, clr);
	}

	COLORREF GetFixedGridLineColor_Light()
	{
		if(!m_hLib)
			return 0x000000;
		
		return pGetFixedGridLineColor_Light(m_lpPointer);
	}

	COLORREF GetFixedGridLineColor_Dark()
	{
		if(!m_hLib)
			return 0x000000;
		
		return pGetFixedGridLineColor_Dark(m_lpPointer);
	}

	void SetHighlightColor(COLORREF clr)
	{
		if(!m_hLib)
			return;

		pSetHighlightColor(m_lpPointer, clr);
	}

	void SetHighlightTextColor(COLORREF clr)
	{
		if(!m_hLib)
			return;

		pSetHighlightTextColor(m_lpPointer, clr);
	}

	COLORREF GetHighlightColor()
	{
		if(!m_hLib)
			return 0x000000;
		
		return pGetHighlightColor(m_lpPointer);
	}

	COLORREF GetHighlightTextColor()
	{
		if(!m_hLib)
			return 0x000000;
		
		return pGetHighlightTextColor(m_lpPointer);
	}

	void SetUseHighlightText(BOOL bUse)
	{
		if(!m_hLib)
			return;

		pSetUseHighlightText(m_lpPointer, bUse);
	}

	BOOL GetUseHighlightText()
	{
		if(!m_hLib)
			return FALSE;

		return pGetUseHighlightText(m_lpPointer);
	}

	void SetFixedRowSelectionHighlight(BOOL bUse)
	{
		if(!m_hLib)
			return;

		pSetFixedRowSelectionHighlight(m_lpPointer, bUse);
	}

	BOOL GetFixedRowSelectionHighlight()
	{
		if(!m_hLib)
			return FALSE;

		return pGetFixedRowSelectionHighlight(m_lpPointer);
	}

	void SetTextColor(COLORREF clr)
	{
		if(!m_hLib)
			return;

		pSetTextColor(m_lpPointer, clr);
	}

	COLORREF GetTextColor()
	{
		if(!m_hLib)
			return 0x000000;
		
		return pGetTextColor(m_lpPointer);
	}

	void SetTextBkColor(COLORREF clr)
	{
		if(!m_hLib)
			return;

		pSetTextBkColor(m_lpPointer, clr);
	}

	COLORREF GetTextBkColor()
	{
		if(!m_hLib)
			return 0x000000;
		
		return pGetTextBkColor(m_lpPointer);
	}

	void SetFixedTextColor(COLORREF clr)
	{
		if(!m_hLib)
			return;

		pSetFixedTextColor(m_lpPointer, clr);
	}

	COLORREF GetFixedTextColor()
	{
		if(!m_hLib)
			return 0x000000;
		
		return pGetFixedTextColor(m_lpPointer);
	}

	void SetFixedBkColor(COLORREF clr)
	{
		if(!m_hLib)
			return;

		pSetFixedBkColor(m_lpPointer, clr);
	}

	COLORREF GetFixedBkColor()
	{
		if(!m_hLib)
			return 0x000000;
		
		return pGetFixedBkColor(m_lpPointer);
	}

	void SetGridColor(COLORREF clr)
	{
		if(!m_hLib)
			return;

		pSetGridColor(m_lpPointer, clr);
	}

	COLORREF GetGridColor()
	{
		if(!m_hLib)
			return 0x000000;
		
		return pGetGridColor(m_lpPointer);
	}

	void SetBkColor(COLORREF clr)
	{
		if(!m_hLib)
			return;

		pSetBkColor(m_lpPointer, clr);
	}

	COLORREF GetBkColor()
	{
		if(!m_hLib)
			return 0x000000;
		
		return pGetBkColor(m_lpPointer);
	}

	void SetDefCellMargin(int nMargin)
	{
		if(!m_hLib)
			return;

		pSetDefCellMargin(m_lpPointer, nMargin);
	}

	int GetDefCellMargin()
	{
		if(!m_hLib)
			return 0;

		return pGetDefCellMargin(m_lpPointer);
	}

	int GetDefCellHeight()
	{
		if(!m_hLib)
			return 0;

		return pGetDefCellHeight(m_lpPointer);
	}

	void SetDefCellHeight(int nHeight)
	{
		if(!m_hLib)
			return;

		pSetDefCellHeight(m_lpPointer, nHeight);
	}

	int GetDefCellWidth()
	{
		if(!m_hLib)
			return 0;

		return pGetDefCellWidth(m_lpPointer);
	}

	void SetDefCellWidth(int nWidth)
	{
		if(!m_hLib)
			return;

		pSetDefCellWidth(m_lpPointer, nWidth);
	}

	int GetSelectedCount()
	{
		if(!m_hLib)
			return 0;

		return pGetSelectedCount(m_lpPointer);
	}

	CCellID SetFocusCell(CCellID cell)
	{
		if(!m_hLib)
		{
			CCellID rCell;
			return rCell;
		}

		return pSetFocusCell1(m_lpPointer, cell);
	}

	CCellID SetFocusCell(int nRow, int nCol)
	{
		if(!m_hLib)
		{
			CCellID rCell;
			return rCell;
		}

		return pSetFocusCell2(m_lpPointer, nRow, nCol);
	}

	CCellID GetFocusCell()
	{
		if(!m_hLib)
		{
			CCellID rCell;
			return rCell;
		}

		return pGetFocusCell(m_lpPointer);
	}

	void SetVirtualMode(BOOL bVirtual)
	{
		if(!m_hLib)
			return;

		pSetVirtualMode(m_lpPointer, bVirtual);
	}

	BOOL GetVirtualMode()
	{
		if(!m_hLib)
			return FALSE;

		return pGetVirtualMode(m_lpPointer);
	}

	void SetCallbackFunc(GRIDCALLBACK pCallback, LPARAM lParam)
	{
		if(!m_hLib)
			return;

		pSetCallbackFunc(m_lpPointer, pCallback, lParam);
	}

	GRIDCALLBACK GetCallbackFunc()
	{
		if(!m_hLib)
			return NULL;

		return pGetCallbackFunc(m_lpPointer);
	}

	void SetImageList(CImageList* pList)
	{
		if(!m_hLib)
			return;

		pSetImageList(m_lpPointer, pList);
	}

	CImageList*	GetImageList()
	{
		if(!m_hLib)
			return NULL;

		return pGetImageList(m_lpPointer);
	}

	void SetGridLines(int nWhichLines)
	{
		if(!m_hLib)
			return;

		pSetGridLines(m_lpPointer, nWhichLines);
	}

	int GetGridLines()
	{
		if(!m_hLib)
			return 0;

		return pGetGridLines(m_lpPointer);
	}

	void SetEditable(BOOL bEditable)
	{
		if(!m_hLib)
			return;

		pSetEditable(m_lpPointer, bEditable);
	}

	BOOL IsEditable()
	{
		if(!m_hLib)
			return FALSE;

		return pIsEditable(m_lpPointer);
	}

	void SetListMode(BOOL bEnableListMode)
	{
		if(!m_hLib)
			return;

		pSetListMode(m_lpPointer, bEnableListMode);
	}

	BOOL GetListMode()
	{
		if(!m_hLib)
			return FALSE;

		return pGetListMode(m_lpPointer);
	}

	void SetSingleRowSelection(BOOL bSing)
	{
		if(!m_hLib)
			return;

		pSetSingleRowSelection(m_lpPointer, bSing);
	}

	BOOL GetSingleRowSelection()
	{
		if(!m_hLib)
			return TRUE;

		return pGetSingleRowSelection(m_lpPointer);
	}

	void SetSingleColSelection(BOOL bSing)
	{
		if(!m_hLib)
			return;

		pSetSingleColSelection(m_lpPointer, bSing);
	}

	BOOL GetSingleColSelection()
	{
		if(!m_hLib)
			return TRUE;

		return pGetSingleColSelection(m_lpPointer);
	}

	void EnableSelection(BOOL bEnable)
	{
		if(!m_hLib)
			return;

		pEnableSelection(m_lpPointer, bEnable);
	}

	BOOL IsSelectable()
	{
		if(!m_hLib)
			return FALSE;

		return pIsSelectable(m_lpPointer);
	}

	void SetFixedColumnSelection(BOOL bSelect)
	{
		if(!m_hLib)
			return;

		pSetFixedColumnSelection(m_lpPointer, bSelect);
	}

	BOOL GetFixedColumnSelection()
	{
		if(!m_hLib)
			return FALSE;

		return pGetFixedColumnSelection(m_lpPointer);
	}

	void SetFixedRowSelection(BOOL bSelect)
	{
		if(!m_hLib)
			return;

		pSetFixedRowSelection(m_lpPointer, bSelect);
	}

	BOOL GetFixedRowSelection()
	{
		if(!m_hLib)
			return FALSE;

		return pGetFixedRowSelection(m_lpPointer);
	}

	void EnableDragAndDrop(BOOL bAllow)
	{
		if(!m_hLib)
			return;

		pEnableDragAndDrop(m_lpPointer, bAllow);
	}

	BOOL GetDragAndDrop()
	{
		if(!m_hLib)
			return FALSE;

		return pGetDragAndDrop(m_lpPointer);
	}

	void SetRowResize(BOOL bResize)
	{
		if(!m_hLib)
			return;

		pSetRowResize(m_lpPointer, bResize);
	}

	BOOL GetRowResize()
	{
		if(!m_hLib)
			return FALSE;

		return pGetRowResize(m_lpPointer);
	}

	void SetColumnResize(BOOL bResize)
	{
		if(!m_hLib)
			return;

		pSetColumnResize(m_lpPointer, bResize);
	}

	BOOL GetColumnResize()
	{
		if(!m_hLib)
			return FALSE;

		return pGetColumnResize(m_lpPointer);
	}

	void SetHeaderSort(BOOL bSortOnClick)
	{
		if(!m_hLib)
			return;

		pSetHeaderSort(m_lpPointer, bSortOnClick);
	}

	BOOL GetHeaderSort()
	{
		if(!m_hLib)
			return FALSE;

		return pGetHeaderSort(m_lpPointer);
	}

	void SetHandleTabKey(BOOL bHandleTab)
	{
		if(!m_hLib)
			return;

		pSetHandleTabKey(m_lpPointer, bHandleTab);
	}

	BOOL GetHandleTabKey()
	{
		if(!m_hLib)
			return FALSE;

		return pGetHandleTabKey(m_lpPointer);
	}

	void SetDoubleBuffering(BOOL bBuffer)
	{
		if(!m_hLib)
			return;

		pSetDoubleBuffering(m_lpPointer, bBuffer);
	}

	BOOL GetDoubleBuffering()
	{
		if(!m_hLib)
			return FALSE;

		return pGetDoubleBuffering(m_lpPointer);
	}

	void EnableTitleTips(BOOL bEnable)
	{
		if(!m_hLib)
			return;

		pEnableTitleTips(m_lpPointer, bEnable);
	}

	BOOL GetTitleTips()
	{
		if(!m_hLib)
			return FALSE;

		return pGetTitleTips(m_lpPointer);
	}

	void SetSortColumn(int nCol)
	{
		if(!m_hLib)
			return;

		pSetSortColumn(m_lpPointer, nCol);
	}

	int GetSortColumn()
	{
		if(!m_hLib)
			return -1;

		return pGetSortColumn(m_lpPointer);
	}

	void SetSortAscending(BOOL bAscending)
	{
		if(!m_hLib)
			return;

		pSetSortAscending(m_lpPointer, bAscending);
	}

	BOOL GetSortAscending()
	{
		if(!m_hLib)
			return TRUE;

		return pGetSortAscending(m_lpPointer);
	}

	void SetTrackFocusCell(BOOL bTrack)
	{
		if(!m_hLib)
			return;

		pSetTrackFocusCell(m_lpPointer, bTrack);
	}

	BOOL GetTrackFocusCell()
	{
		if(!m_hLib)
			return FALSE;

		return pGetTrackFocusCell(m_lpPointer);
	}

	void SetFrameFocusCell(BOOL bFrame)
	{
		if(!m_hLib)
			return;

		pSetFrameFocusCell(m_lpPointer, bFrame);
	}

	BOOL GetFrameFocusCell()
	{
		if(!m_hLib)
			return FALSE;

		return pGetFrameFocusCell(m_lpPointer);
	}

	void SetAutoSizeStyle(int nStyle)
	{
		if(!m_hLib)
			return;

		pSetAutoSizeStyle(m_lpPointer, nStyle);
	}

	int GetAutoSizeStyle()
	{
		if(!m_hLib)
			return 0;

		return pGetAutoSizeStyle(m_lpPointer);
	}

	void EnableHiddenColUnhide(BOOL bEnable)
	{
		if(!m_hLib)
			return;

		pEnableHiddenColUnhide(m_lpPointer, bEnable);
	}

	BOOL GetHiddenColUnhide()
	{
		if(!m_hLib)
			return FALSE;

		return pGetHiddenColUnhide(m_lpPointer);
	}

	void EnableHiddenRowUnhide(BOOL bEnable)
	{
		if(!m_hLib)
			return;

		pEnableHiddenRowUnhide(m_lpPointer, bEnable);
	}

	BOOL GetHiddenRowUnhide()
	{
		if(!m_hLib)
			return FALSE;

		return pGetHiddenRowUnhide(m_lpPointer);
	}

	void EnableColumnHide(BOOL bEnable)
	{
		if(!m_hLib)
			return;

		pEnableColumnHide(m_lpPointer, bEnable);
	}

	BOOL GetColumnHide()
	{
		if(!m_hLib)
			return FALSE;

		return pGetColumnHide(m_lpPointer);
	}

	void EnableRowHide(BOOL bEnable)
	{
		if(!m_hLib)
			return;

		pEnableRowHide(m_lpPointer, bEnable);
	}

	BOOL GetRowHide()
	{
		if(!m_hLib)
			return FALSE;

		return pGetRowHide(m_lpPointer);
	}

	void SetDefaultReadOnly(BOOL bReadOnly)
	{
		if(!m_hLib)
			return;

		pSetDefaultReadOnly(m_lpPointer, bReadOnly);
	}

	BOOL IsDefaultReadOnly()
	{
		if(!m_hLib)
			return FALSE;

		return pIsDefaultReadOnly(m_lpPointer);
	}

	CGridCellBase* GetDefaultCell(BOOL bFixedRow, BOOL bFixedCol)
	{
		if(!m_hLib)
			return NULL;

		return pGetDefaultCell(m_lpPointer, bFixedRow, bFixedCol);
	}

	CGridCellBase* GetCell(int nRow, int nCol)
	{
		if(!m_hLib)
			return NULL;

		return pGetCell(m_lpPointer, nRow, nCol);
	}

	void SetModified(BOOL bModified, int nRow, int nCol)
	{
		if(!m_hLib)
			return;

		pSetModified(m_lpPointer, bModified, nRow, nCol);
	}

	BOOL GetModified(int nRow, int nCol)
	{
		if(!m_hLib)
			return FALSE;

		return pGetModified(m_lpPointer, nRow, nCol);
	}

	BOOL IsCellFixed(int nRow, int nCol)
	{
		if(!m_hLib)
			return FALSE;

		return pIsCellFixed(m_lpPointer, nRow, nCol);
	}

	BOOL SetItem(GV_ITEM* pItem)
	{
		if(!m_hLib)
			return FALSE;

		return pSetItem(m_lpPointer, pItem);
	}

	BOOL GetItem(GV_ITEM* pItem)
	{
		if(!m_hLib)
			return FALSE;

		return pGetItem(m_lpPointer, pItem);
	}

	BOOL SetItemText(int nRow, int nCol, LPCTSTR str)
	{
		if(!m_hLib)
			return FALSE;

		return pSetItemText(m_lpPointer, nRow, nCol, str);
	}

	CString	GetItemText(int nRow, int nCol)
	{
		if(!m_hLib)
			return _T("");

		return pGetItemText(m_lpPointer, nRow, nCol);
	}

/*
	BOOL SetItemTextFmt(int nRow, int nCol, LPCTSTR szFmt, ...)
	{
		if(!m_hLib)
			return FALSE;

		return pSetItemTextFmt(m_lpPointer, nRow, nCol, szFmt, ...);
	}
*/

/*
	BOOL SetItemTextFmtID(int nRow, int nCol, UINT nID, ...)
	{
		if(!m_hLib)
			return FALSE;

		return pSetItemTextFmtID(m_lpPointer, nRow, nCol, nID, ...);
	}
*/

	BOOL SetItemData(int nRow, int nCol, LPARAM lParam)
	{
		if(!m_hLib)
			return FALSE;

		return pSetItemData(m_lpPointer, nRow, nCol, lParam);
	}

	LPARAM GetItemData(int nRow, int nCol)
	{
		if(!m_hLib)
			return 0L;

		return pGetItemData(m_lpPointer, nRow, nCol);
	}

	BOOL SetItemImage(int nRow, int nCol, int iImage)
	{
		if(!m_hLib)
			return FALSE;

		return pSetItemImage(m_lpPointer, nRow, nCol, iImage);
	}

	int GetItemImage(int nRow, int nCol)
	{
		if(!m_hLib)
			return -1;

		return pGetItemImage(m_lpPointer, nRow, nCol);
	}

	BOOL SetItemState(int nRow, int nCol, UINT state)
	{
		if(!m_hLib)
			return FALSE;

		return pSetItemState(m_lpPointer, nRow, nCol, state);
	}

	UINT GetItemState(int nRow, int nCol)
	{
		if(!m_hLib)
			return 0;

		return pGetItemState(m_lpPointer, nRow, nCol);
	}

	BOOL SetItemFormat(int nRow, int nCol, UINT nFormat)
	{
		if(!m_hLib)
			return FALSE;

		return pSetItemFormat(m_lpPointer, nRow, nCol, nFormat);
	}

	UINT GetItemFormat(int nRow, int nCol)
	{
		if(!m_hLib)
			return 0;

		return pGetItemFormat(m_lpPointer, nRow, nCol);
	}

	BOOL SetItemBkColour(int nRow, int nCol, COLORREF cr)
	{
		if(!m_hLib)
			return FALSE;

		return pSetItemBkColour(m_lpPointer, nRow, nCol, cr);
	}

	COLORREF GetItemBkColour(int nRow, int nCol)
	{
		if(!m_hLib)
			return 0x000000;

		return pGetItemBkColour(m_lpPointer, nRow, nCol);
	}

	BOOL SetItemFgColour(int nRow, int nCol, COLORREF cr)
	{
		if(!m_hLib)
			return FALSE;

		return pSetItemFgColour(m_lpPointer, nRow, nCol, cr);
	}

	COLORREF GetItemFgColour(int nRow, int nCol)
	{
		if(!m_hLib)
			return 0x000000;

		return pGetItemFgColour(m_lpPointer, nRow, nCol);
	}

	BOOL SetItemFont(int nRow, int nCol, LOGFONT* lf)
	{
		if(!m_hLib)
			return FALSE;

		return pSetItemFont(m_lpPointer, nRow, nCol, lf);
	}

	LOGFONT* GetItemFont(int nRow, int nCol)
	{
		if(!m_hLib)
			return NULL;

		return pGetItemFont(m_lpPointer, nRow, nCol);
	}

	BOOL IsItemEditing(int nRow, int nCol)
	{
		if(!m_hLib)
			return FALSE;

		return pIsItemEditing(m_lpPointer, nRow, nCol);
	}

	BOOL SetCellType(int nRow, int nCol, CRuntimeClass* pRuntimeClass)
	{
		if(!m_hLib)
			return FALSE;

		return pSetCellType(m_lpPointer, nRow, nCol, pRuntimeClass);
	}

	int InsertColumn(LPCTSTR strHeading, UINT nFormat, int nColumn)
	{
		if(!m_hLib)
			return -1;

		return pInsertColumn(m_lpPointer, strHeading, nFormat, nColumn);
	}

	int InsertRow(LPCTSTR strHeading, int nRow)
	{
		if(!m_hLib)
			return -1;

		return pInsertRow(m_lpPointer, strHeading, nRow);
	}

	BOOL DeleteColumn(int nColumn)
	{
		if(!m_hLib)
			return FALSE;

		return pDeleteColumn(m_lpPointer, nColumn);
	}

	BOOL DeleteRow(int nRow)
	{
		if(!m_hLib)
			return FALSE;

		return pDeleteRow(m_lpPointer, nRow);
	}

	BOOL DeleteNonFixedRows()
	{
		if(!m_hLib)
			return FALSE;

		return pDeleteNonFixedRows(m_lpPointer);
	}

	BOOL DeleteAllItems()
	{
		if(!m_hLib)
			return FALSE;

		return pDeleteAllItems(m_lpPointer);
	}

	void ClearCells(CCellRange Selection)
	{
		if(!m_hLib)
			return;

		pClearCells(m_lpPointer, Selection);
	}

	BOOL AutoSizeRow(int nRow, BOOL bResetScroll)
	{
		if(!m_hLib)
			return FALSE;

		return pAutoSizeRow(m_lpPointer, nRow, bResetScroll);
	}

	BOOL AutoSizeColumn(int nCol, UINT nAutoSizeStyle, BOOL bResetScroll)
	{
		if(!m_hLib)
			return FALSE;

		return pAutoSizeColumn(m_lpPointer, nCol, nAutoSizeStyle, bResetScroll);
	}

	void AutoSizeRows()
	{
		if(!m_hLib)
			return;

		pAutoSizeRows(m_lpPointer);
	}

	void AutoSizeColumns(UINT nAutoSizeStyle)
	{
		if(!m_hLib)
			return;

		pAutoSizeColumns(m_lpPointer, nAutoSizeStyle);
	}

	void AutoSize(UINT nAutoSizeStyle)
	{
		if(!m_hLib)
			return;

		pAutoSize(m_lpPointer, nAutoSizeStyle);
	}

	void ExpandColumnsToFit(BOOL bExpandFixed)
	{
		if(!m_hLib)
			return;

		pExpandColumnsToFit(m_lpPointer, bExpandFixed);
	}

	void ExpandLastColumn()
	{
		if(!m_hLib)
			return;

		pExpandLastColumn(m_lpPointer);
	}

	void ExpandRowsToFit(BOOL bExpandFixed)
	{
		if(!m_hLib)
			return;

		pExpandRowsToFit(m_lpPointer, bExpandFixed);
	}

	void ExpandToFit(BOOL bExpandFixed)
	{
		if(!m_hLib)
			return;

		pExpandToFit(m_lpPointer, bExpandFixed);
	}

	void Refresh()
	{
		if(!m_hLib)
			return;

		pRefresh(m_lpPointer);
	}

	void AutoFill()
	{
		if(!m_hLib)
			return;

		pAutoFill(m_lpPointer);
	}

	void EnsureVisible(CCellID& cell)
	{
		if(!m_hLib)
			return;

		pEnsureVisible(m_lpPointer, cell);
	}

	void EnsureVisible(int nRow, int nCol)
	{
		if(!m_hLib)
			return;

		pEnsureVisible2(m_lpPointer, nRow, nCol);
	}

	BOOL IsCellVisible(int nRow, int nCol)
	{
		if(!m_hLib)
			return FALSE;

		return pIsCellVisible(m_lpPointer, nRow, nCol);
	}

	BOOL IsCellVisible(CCellID cell)
	{
		if(!m_hLib)
			return FALSE;

		return pIsCellVisible2(m_lpPointer, cell);
	}

	BOOL IsCellEditable(int nRow, int nCol)
	{
		if(!m_hLib)
			return FALSE;

		return pIsCellEditable(m_lpPointer, nRow, nCol);
	}

	BOOL IsCellEditable(CCellID& cell)
	{
		if(!m_hLib)
			return FALSE;

		return pIsCellEditable2(m_lpPointer, cell);
	}

	BOOL IsCellSelected(int nRow, int nCol)
	{
		if(!m_hLib)
			return FALSE;

		return pIsCellSelected(m_lpPointer, nRow, nCol);
	}

	BOOL IsCellSelected(CCellID& cell)
	{
		if(!m_hLib)
			return FALSE;

		return pIsCellSelected2(m_lpPointer, cell);
	}

	void SetRedraw(BOOL bAllowDraw, BOOL bResetScrollBars)
	{
		if(!m_hLib)
			return;

		pSetRedraw(m_lpPointer, bAllowDraw, bResetScrollBars);
	}

	BOOL RedrawCell(int nRow, int nCol, CDC* pDC)
	{
		if(!m_hLib)
			return FALSE;

		return pRedrawCell(m_lpPointer, nRow, nCol, pDC);
	}

	BOOL RedrawCell(CCellID& cell, CDC* pDC)
	{
		if(!m_hLib)
			return FALSE;

		return pRedrawCell2(m_lpPointer, cell, pDC);
	}

	BOOL RedrawRow(int row)
	{
		if(!m_hLib)
			return FALSE;

		return pRedrawRow(m_lpPointer, row);
	}

	BOOL RedrawColumn(int col)
	{
		if(!m_hLib)
			return FALSE;

		return pRedrawColumn(m_lpPointer, col);
	}

	BOOL Save(LPCTSTR filename, TCHAR chSeparator)
	{
		if(!m_hLib)
			return FALSE;

		return pSave(m_lpPointer, filename, chSeparator);
	}

	BOOL Load(LPCTSTR filename, TCHAR chSeparator)
	{
		if(!m_hLib)
			return FALSE;

		return pLoad(m_lpPointer, filename, chSeparator);
	}

	CCellRange GetCellRange()
	{
		if(!m_hLib)
			return CCellRange();

		return pGetCellRange(m_lpPointer);
	}

	CCellRange GetSelectedCellRange()
	{
		if(!m_hLib)
			return CCellRange();

		return pGetSelectedCellRange(m_lpPointer);
	}

	void SetSelectedRange(CCellRange& Range, BOOL bForceRepaint, BOOL bSelectCells)
	{
		if(!m_hLib)
			return;

		pSetSelectedRange(m_lpPointer, Range, bForceRepaint, bSelectCells);
	}

	void SetSelectedRange(int nMinRow, int nMinCol, int nMaxRow, int nMaxCol, BOOL bForceRepaint, BOOL bSelectCells)
	{
		if(!m_hLib)
			return;

		pSetSelectedRange2(m_lpPointer, nMinRow, nMinCol, nMaxRow, nMaxCol, bForceRepaint, bSelectCells);
	}

	BOOL IsValid(int nRow, int nCol)
	{
		if(!m_hLib)
			return FALSE;

		 return pIsValid(m_lpPointer, nRow, nCol);
	}

	BOOL IsValid(CCellID& cell)
	{
		if(!m_hLib)
			return FALSE;

		return pIsValid2(m_lpPointer, cell);
	}

	BOOL IsValid(CCellRange& range)
	{
		if(!m_hLib)
			return FALSE;

		return pIsValid3(m_lpPointer, range);
	}

	void CutSelectedText()
	{
		if(!m_hLib)
			return;

		pCutSelectedText(m_lpPointer);
	}

	COleDataSource* CopyTextFromGrid()
	{
		if(!m_hLib)
			return NULL;

		return pCopyTextFromGrid(m_lpPointer);
	}

	BOOL PasteTextToGrid(CCellID cell, COleDataObject* pDataObject, BOOL bSelectPastedCells)
	{
		if(!m_hLib)
			return FALSE;

		return pPasteTextToGrid(m_lpPointer, cell, pDataObject, bSelectPastedCells);
	}

	void OnBeginDrag()
	{
		if(!m_hLib)
			return;

		pOnBeginDrag(m_lpPointer);
	}

	DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
	{
		if(!m_hLib)
			return 0;

		return pOnDragEnter(m_lpPointer, pDataObject, dwKeyState, point);
	}

	DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
	{
		if(!m_hLib)
			return 0;

		return pOnDragOver(m_lpPointer, pDataObject, dwKeyState, point);
	}

	void OnDragLeave()
	{
		if(!m_hLib)
			return;

		pOnDragLeave(m_lpPointer);
	}

	BOOL OnDrop(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
	{
		if(!m_hLib)
			return FALSE;

		return pOnDrop(m_lpPointer, pDataObject, dwKeyState, point);
	}

	void OnEditCut()
	{
		if(!m_hLib)
			return;

		pOnEditCut(m_lpPointer);
	}

	void OnEditCopy()
	{
		if(!m_hLib)
			return;

		pOnEditCopy(m_lpPointer);
	}

	void OnEditPaste()
	{
		if(!m_hLib)
			return;

		pOnEditPaste(m_lpPointer);
	}

	void OnEditSelectAll()
	{
		if(!m_hLib)
			return;

		pOnEditSelectAll(m_lpPointer);
	}

	CCellID	GetNextItem(CCellID& cell, int nFlags)
	{
		if(!m_hLib)
			return CCellID();

		return pGetNextItem(m_lpPointer, cell, nFlags);
	}

	BOOL SortItems(int nCol, BOOL bAscending, LPARAM data)
	{
		if(!m_hLib)
			return FALSE;

		return pSortItems1(m_lpPointer, nCol, bAscending, data);
	}

	BOOL SortTextItems(int nCol, BOOL bAscending, LPARAM data)
	{
		if(!m_hLib)
			return FALSE;

		return pSortTextItems2(m_lpPointer, nCol, bAscending, data);
	}

	BOOL SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data)
	{
		if(!m_hLib)
			return FALSE;

		return pSortItems2(m_lpPointer, pfnCompare, nCol, bAscending, data);
	}

	void SetCompareFunction(PFNLVCOMPARE pfnCompare)
	{
		if(!m_hLib)
			return;

		pSetCompareFunction(m_lpPointer, pfnCompare);
	}

	BOOL RegisterWindowClass()
	{
		if(!m_hLib)
			return FALSE;

		return pRegisterWindowClass(m_lpPointer);
	}

	BOOL Initialise()
	{
		if(!m_hLib)
			return FALSE;

		return pInitialise(m_lpPointer);
	}

	void SetupDefaultCells()
	{
		if(!m_hLib)
			return;

		pSetupDefaultCells(m_lpPointer);
	}

	LRESULT	SendMessageToParent(int nRow, int nCol, int nMessage)
	{
		if(!m_hLib)
			return 0L;

		return pSendMessageToParent(m_lpPointer, nRow, nCol, nMessage);
	}

	LRESULT	SendDisplayRequestToParent(GV_DISPINFO* pDisplayInfo)
	{
		if(!m_hLib)
			return 0L;

		return pSendDisplayRequestToParent(m_lpPointer, pDisplayInfo);
	}

	LRESULT	SendCacheHintToParent(CCellRange& range)
	{
		if(!m_hLib)
			return 0L;

		return pSendCacheHintToParent(m_lpPointer, range);
	}

	BOOL InvalidateCellRect(int row, int col)
	{
		if(!m_hLib)
			return FALSE;

		return pInvalidateCellRect(m_lpPointer, row, col);
	}

	BOOL InvalidateCellRect(CCellID& cell)
	{
		if(!m_hLib)
			return FALSE;

		return pInvalidateCellRect2(m_lpPointer, cell);
	}

	BOOL InvalidateCellRect(CCellRange& cellRange)
	{
		if(!m_hLib)
			return FALSE;

		return pInvalidateCellRect3(m_lpPointer, cellRange);
	}

	void EraseBkgnd(CDC* pDC)
	{
		if(!m_hLib)
			return;

		pEraseBkgnd(m_lpPointer, pDC);
	}

	BOOL GetCellRangeRect(CCellRange& cellRange, LPRECT lpRect)
	{
		if(!m_hLib)
			return FALSE;

		return pGetCellRangeRect(m_lpPointer, cellRange, lpRect);
	}

	BOOL SetCell(int nRow, int nCol, CGridCellBase* pCell)
	{
		if(!m_hLib)
			return FALSE;

		return pSetCell(m_lpPointer, nRow, nCol, pCell);
	}

	int SetMouseMode(int nMode)
	{
		if(!m_hLib)
			return 0;

		return pSetMouseMode(m_lpPointer, nMode);
	}

	int GetMouseMode()
	{
		if(!m_hLib)
			return 0;

		return pGetMouseMode(m_lpPointer);
	}

	BOOL MouseOverRowResizeArea(CPoint& point)
	{
		if(!m_hLib)
			return FALSE;

		return pMouseOverRowResizeArea(m_lpPointer, point);
	}

	BOOL MouseOverColumnResizeArea(CPoint& point)
	{
		if(!m_hLib)
			return FALSE;

		return pMouseOverColumnResizeArea(m_lpPointer, point);
	}

	CCellID	GetTopleftNonFixedCell(BOOL bForceRecalculation)
	{
		if(!m_hLib)
			return CCellID();
//			return NULL;

		return pGetTopleftNonFixedCell(m_lpPointer, bForceRecalculation);
	}

	CCellRange GetUnobstructedNonFixedCellRange(BOOL bForceRecalculation)
	{
		if(!m_hLib)
			return CCellRange();

		return pGetUnobstructedNonFixedCellRange(m_lpPointer, bForceRecalculation);
	}

	CCellRange GetVisibleNonFixedCellRange(LPRECT pRect, BOOL bForceRecalculation)
	{
		if(!m_hLib)
			return CCellRange();

		return pGetVisibleNonFixedCellRange(m_lpPointer, pRect, bForceRecalculation);
	}

	BOOL IsVisibleVScroll()
	{
		if(!m_hLib)
			return FALSE;

		return pIsVisibleVScroll(m_lpPointer);
	}

	BOOL IsVisibleHScroll()
	{
		if(!m_hLib)
			return FALSE;

		return pIsVisibleHScroll(m_lpPointer);
	}

	void ResetSelectedRange()
	{
		if(!m_hLib)
			return;

		pResetSelectedRange(m_lpPointer);
	}

	void ResetScrollBars()
	{
		if(!m_hLib)
			return;

		pResetScrollBars(m_lpPointer);
	}

	void EnableScrollBars(int nBar, BOOL bEnable)
	{
		if(!m_hLib)
			return;

		pEnableScrollBars(m_lpPointer, nBar, bEnable);
	}

	int GetScrollPos32(int nBar, BOOL bGetTrackPos)
	{
		if(!m_hLib)
			return 0;

		return pGetScrollPos32(m_lpPointer, nBar, bGetTrackPos);
	}

	BOOL SetScrollPos32(int nBar, int nPos, BOOL bRedraw)
	{
		if(!m_hLib)
			return FALSE;

		return pSetScrollPos32(m_lpPointer, nBar, nPos, bRedraw);
	}

	BOOL SortTextItems(int nCol, BOOL bAscending, int low, int high)
	{
		if(!m_hLib)
			return FALSE;

		return pSortTextItems3(m_lpPointer, nCol, bAscending, low, high);
	}

	BOOL SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data, int low, int high)
	{
		if(!m_hLib)
			return FALSE;

		return pSortItems3(m_lpPointer, pfnCompare, nCol, bAscending, data, low, high);
	}

	CPoint	GetPointClicked(int nRow, int nCol, CPoint& point)
	{
		if(!m_hLib)
			return CPoint(0);

		return pGetPointClicked(m_lpPointer, nRow, nCol, point);
	}

	void ValidateAndModifyCellContents(int nRow, int nCol, LPCTSTR strText)
	{
		if(!m_hLib)
			return;

		pValidateAndModifyCellContents(m_lpPointer, nRow, nCol, strText);
	}

	void PreSubclassWindow()
	{
		if(!m_hLib)
			return;

		pPreSubclassWindow(m_lpPointer);
	}

	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if(!m_hLib)
			return 0L;

		return pWindowProc(m_lpPointer, message, wParam, lParam);
	}

	CImageList* CreateDragImage(CPoint* pHotSpot)
	{
		if(!m_hLib)
			return NULL;

		return pCreateDragImage(m_lpPointer, pHotSpot);
	}

	void OnFixedColumnClick(CCellID& cell)
	{
		if(!m_hLib)
			return;

		pOnFixedColumnClick(m_lpPointer, cell);
	}

	void OnFixedRowClick(CCellID& cell)
	{
		if(!m_hLib)
			return;

		pOnFixedRowClick(m_lpPointer, cell);
	}

	void OnEditCell(int nRow, int nCol, CPoint point, UINT nChar)
	{
		if(!m_hLib)
			return;

		pOnEditCell(m_lpPointer, nRow, nCol, point, nChar);
	}

	void OnEndEditCell(int nRow, int nCol, CString str)
	{
		if(!m_hLib)
			return;

		pOnEndEditCell(m_lpPointer, nRow, nCol, str);
	}

	BOOL ValidateEdit(int nRow, int nCol, LPCTSTR str)
	{
		if(!m_hLib)
			return FALSE;

		return pValidateEdit(m_lpPointer, nRow, nCol, str);
	}

	void EndEditing()
	{
		if(!m_hLib)
			return;

		pEndEditing(m_lpPointer);
	}

	void OnDraw(CDC* pDC)
	{
		if(!m_hLib)
			return;

		pOnDraw(m_lpPointer, pDC);
	}

	CGridCellBase* CreateCell(int nRow, int nCol)
	{
		if(!m_hLib)
			return NULL;

		return pCreateCell(m_lpPointer, nRow, nCol);
	}

	void DestroyCell(int nRow, int nCol)
	{
		if(!m_hLib)
			return;

		pDestroyCell(m_lpPointer, nRow, nCol);
	}

	void SelectAllCells()
	{
		if(!m_hLib)
			return;

		pSelectAllCells(m_lpPointer);
	}

	void SelectColumns(CCellID currentCell, BOOL bForceRedraw, BOOL bSelectCells)
	{
		if(!m_hLib)
			return;

		pSelectColumns(m_lpPointer, currentCell, bForceRedraw, bSelectCells);
	}

	void SelectRows(CCellID currentCell, BOOL bForceRedraw, BOOL bSelectCells)
	{
		if(!m_hLib)
			return;

		pSelectRows(m_lpPointer, currentCell, bForceRedraw, bSelectCells);
	}

	void SelectCells(CCellID currentCell, BOOL bForceRedraw, BOOL bSelectCells)
	{
		if(!m_hLib)
			return;

		pSelectCells(m_lpPointer, currentCell, bForceRedraw, bSelectCells);
	}

	void OnSelecting(CCellID& currentCell)
	{
		if(!m_hLib)
			return;

		pOnSelecting(m_lpPointer, currentCell);
	}

	void OnPaint()
	{
		if(!m_hLib)
			return;

		pOnPaint(m_lpPointer);
	}

	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
	{
		if(!m_hLib)
			return;

		pOnHScroll(m_lpPointer, nSBCode, nPos, pScrollBar);
	}

	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
	{
		if(!m_hLib)
			return;

		pOnVScroll(m_lpPointer, nSBCode, nPos, pScrollBar);
	}

	void OnSize(UINT nType, int cx, int cy)
	{
		if(!m_hLib)
			return;

		pOnSize(m_lpPointer, nType, cx, cy);
	}

	void OnLButtonUp(UINT nFlags, CPoint point)
	{
		if(!m_hLib)
			return;

		pOnLButtonUp(m_lpPointer, nFlags, point);
	}

	void OnLButtonDown(UINT nFlags, CPoint point)
	{
		if(!m_hLib)
			return;

		pOnLButtonDown(m_lpPointer, nFlags, point);
	}

	void OnMouseMove(UINT nFlags, CPoint point)
	{
		if(!m_hLib)
			return;

		pOnMouseMove(m_lpPointer, nFlags, point);
	}

	void OnTimer(UINT nIDEvent)
	{
		if(!m_hLib)
			return;

		pOnTimer(m_lpPointer, nIDEvent);
	}

	UINT OnGetDlgCode()
	{
		if(!m_hLib)
			return 0;

		return pOnGetDlgCode(m_lpPointer);
	}

	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if(!m_hLib)
			return;

		pOnKeyDown(m_lpPointer, nChar, nRepCnt, nFlags);
	}

	void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if(!m_hLib)
			return;

		pOnKeyUp(m_lpPointer, nChar, nRepCnt, nFlags);
	}

	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if(!m_hLib)
			return;

		pOnChar(m_lpPointer, nChar, nRepCnt, nFlags);
	}

	void OnLButtonDblClk(UINT nFlags, CPoint point)
	{
		if(!m_hLib)
			return;

		pOnLButtonDblClk(m_lpPointer, nFlags, point);
	}

	BOOL OnEraseBkgnd(CDC* pDC)
	{
		if(!m_hLib)
			return FALSE;

		return pOnEraseBkgnd(m_lpPointer, pDC);
	}

	void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if(!m_hLib)
			return;

		pOnSysKeyDown(m_lpPointer, nChar, nRepCnt, nFlags);
	}

	void OnContextMenu(CWnd* pWnd, CPoint point)
	{
		if(!m_hLib)
			return;

		pOnContextMenu(m_lpPointer, pWnd, point);
	}

	void OnUpdateEditSelectAll(CCmdUI* pCmdUI)
	{
		if(!m_hLib)
			return;

		pOnUpdateEditSelectAll(m_lpPointer, pCmdUI);
	}

	BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
	{
		if(!m_hLib)
			return FALSE;

		return pOnSetCursor(m_lpPointer, pWnd, nHitTest, message);
	}

	void OnRButtonDown(UINT nFlags, CPoint point)
	{
		if(!m_hLib)
			return;

		pOnRButtonDown(m_lpPointer, nFlags, point);
	}

	void OnRButtonUp(UINT nFlags, CPoint point)
	{
		if(!m_hLib)
			return;

		pOnRButtonUp(m_lpPointer, nFlags, point);
	}

	void OnSysColorChange()
	{
		if(!m_hLib)
			return;

		pOnSysColorChange(m_lpPointer);
	}

	void OnCaptureChanged(CWnd* pWnd)
	{
		if(!m_hLib)
			return;

		pOnCaptureChanged(m_lpPointer, pWnd);
	}

	void OnUpdateEditCopy(CCmdUI* pCmdUI)
	{
		if(!m_hLib)
			return;

		pOnUpdateEditCopy(m_lpPointer, pCmdUI);
	}

	void OnUpdateEditCut(CCmdUI* pCmdUI)
	{
		if(!m_hLib)
			return;

		pOnUpdateEditCut(m_lpPointer, pCmdUI);
	}

	void OnUpdateEditPaste(CCmdUI* pCmdUI)
	{
		if(!m_hLib)
			return;

		pOnUpdateEditPaste(m_lpPointer, pCmdUI);
	}

	void OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
	{
		if(!m_hLib)
			return;

		pOnSettingChange(m_lpPointer, uFlags, lpszSection);
	}

	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		if(!m_hLib)
			return FALSE;

		return pOnMouseWheel(m_lpPointer, nFlags, zDelta, pt);
	}

	LRESULT OnSetFont(WPARAM hFont, LPARAM lParam)
	{
		if(!m_hLib)
			return 0L;

		return pOnSetFont(m_lpPointer, hFont, lParam);
	}

	LRESULT OnGetFont(WPARAM hFont, LPARAM lParam)
	{
		if(!m_hLib)
			return 0L;

		return pOnGetFont(m_lpPointer, hFont, lParam);
	}

	LRESULT OnImeChar(WPARAM wCharCode, LPARAM lParam)
	{
		if(!m_hLib)
			return 0L;

		return pOnImeChar(m_lpPointer, wCharCode, lParam);
	}

	void OnEndInPlaceEdit(NMHDR* pNMHDR, LRESULT* pResult)
	{
		if(!m_hLib)
			return;

		pOnEndInPlaceEdit(m_lpPointer, pNMHDR, pResult);
	}

	UINT GetMouseScrollLines()
	{
		if(!m_hLib)
			return 0;

		return pGetMouseScrollLines(m_lpPointer);
	}

	BOOL SetDefaultCellType(CRuntimeClass* pRuntimeClass)
	{
		if(!m_hLib)
			return FALSE;

		return pSetDefaultCellType(m_lpPointer, pRuntimeClass);
	}

	void SetGridScrollBarVisible(BOOL bHScroll, BOOL bVScroll)
	{
		if(!m_hLib)
			return;

		pSetGridScrollBarVisible(m_lpPointer, bHScroll, bVScroll);
	}

	void SetUseFocusRect(BOOL bUse)
	{
		if(!m_hLib)
			return;

		pSetUseFocusRect(m_lpPointer, bUse);
	}

	BOOL GetUseFocusRect()
	{
		if(!m_hLib)
			return FALSE;

		return pGetUseFocusRect(m_lpPointer);
	}

	void SetDrawFixedLine(BOOL bDraw)
	{
		if(!m_hLib)
			return;

		pSetDrawFixedLine(m_lpPointer, bDraw);
	}
};

#endif // !defined(AFX_LOADHELPERGRID_H__DF553F68_69A3_11D6_994C_004854800CB4__INCLUDED_)
