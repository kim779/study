#if !defined(AFX_COMCTRL_H__INCLUDED_)
#define AFX_COMCTRL_H__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "griditem.h"
//
// defined for CodeCtrl
//
#define	GU_CODE		0
#define	GU_FUTURE	1
#define	GU_OPTION	2
#define	GU_INDEX	3
#define	GU_FOCODE	4
#define	GU_POPTION	5

class CComCtrl:public CWnd
{
public:
	BOOL create(CWnd *pParent)
	{
		return CreateControl("AXCOMCTL.AxComCtl.1", NULL, NULL,
						CRect(0, 0, 0, 0), pParent, 0);
	}

	CWnd*	axComCreate(CString sKind, CString strVal, DWORD dwStyle, CRect rect, CWnd* parentWnd, UINT nID, int flag = 0)
	{
		RECT *pRect = rect;
		long rtn;
		
		InvokeHelper(1, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4,
			sKind, strVal,  dwStyle, (long)pRect, (long)parentWnd, nID, flag);
		return (CWnd*)rtn;
	}

	bool	axComDestroy(CWnd* key)
	{
		BOOL rtn;
		InvokeHelper(2, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*)VTS_I4, (long)key);
		return (rtn?true:false);
	}

	// IMGBtn
	void	setImgBitmaps(CWnd* key, HBITMAP hBitmap, HBITMAP hBitmap_dn, HBITMAP hBitmap_hv)
	{
		InvokeHelper(3, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_I4 VTS_I4 VTS_I4, 
			(long)key, (long)hBitmap, (long)hBitmap_dn, (long)hBitmap_hv);
	}

	// CodeCtrl
	void	setCodeKind(CWnd *key, int nKind)
	{
		InvokeHelper(4, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_I4,
			(long)key, (long)nKind);
	}

	void	setCodeHistory(CWnd *key, CString sHistory)
	{
		InvokeHelper(5, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_BSTR,
			(long)key, sHistory);
	}
	// FxGrid
	BOOL	Initial(CWnd *key, int nRow, int nCol, int fixRow, int fixCol)
	{
		BOOL rtn;
		InvokeHelper(6, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4,
			(long)key, nRow, nCol, fixRow, fixCol);
		return rtn;
	}

	void	Refresh(CWnd *key, BOOL NOselect = TRUE)
	{
		InvokeHelper(7, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_BOOL, 
			(long)key, NOselect);
	}

	void	Adjust(CWnd *key)
	{
		InvokeHelper(8, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4,
			(long)key);
	}

	void	Clear(CWnd *key, int sidxRow = -1, int sidxCol = -1)
	{
		InvokeHelper(9, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_I4 VTS_I4,
			(long)key, sidxRow, sidxCol);
	}

	BOOL	Sort(CWnd *key, int nCol, BOOL ascending)
	{
		BOOL rtn;
		InvokeHelper(10, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_BOOL,
			(long)key, nCol, ascending);
		return rtn;
	}

	void	Blink(CWnd *key, int nRow, int nCol, COLORREF color, int interval = 1000)
	{
		InvokeHelper(11, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4,
			(long)key, nRow, nCol, color, interval);
	}

	BOOL	GetCellSize(CWnd *key, int fromRow, int endRow, int& colW, int& rowH)
	{
		BOOL rtn;
		InvokeHelper(12, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_PI4 VTS_PI4,
			(long)key, fromRow, endRow, &colW, &rowH);
		return rtn;
	}

	void	GetCellFromPoint(CWnd *key, CPoint pt, int& nRow, int& nCol)
	{
		InvokeHelper(13, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_I4 VTS_I4 VTS_PI4 VTS_PI4,
			(long)key, pt.x, pt.y, &nRow, &nCol);
	}

	void	GetTopLeftNonFixedCell(CWnd *key, int& nRow, int& nCol)
	{
		InvokeHelper(14, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_PI4 VTS_PI4,
			(long)key, &nRow, &nCol);
	}

	void	GetSelectedCellRange(CWnd *key, int& minRow, int& maxRow, int& minCol, int& maxCol)
	{
		InvokeHelper(15, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4,
			(long)key, &minRow, &maxRow, &minCol, &maxCol);
	}

	void	ClearAllRTM(CWnd *key)
	{
		InvokeHelper(16, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4,
			(long)key);
	}

	void	ClearMerge(CWnd *key)
	{
		InvokeHelper(17, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4,
			(long)key);
	}
	
	LONG	MessageToGrid(CWnd *key, UINT msg, int scode)
	{
		long rtn;
		InvokeHelper(18, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4,
			(long)key, msg, scode);
		return rtn;
	}

	BOOL	SubclassWindow(CWnd *key, HWND hWnd)
	{
		BOOL rtn;
		InvokeHelper(19, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4,
			(long)key, (long)hWnd);
		return rtn;
	}

	BOOL	MakeExcelFile(CWnd *key, CString path, BOOL visible = TRUE)
	{
		BOOL rtn;
		InvokeHelper(20, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_BSTR VTS_BOOL,
			(long)key, path, visible);
		return rtn;
	}

	int	GetRowCount(CWnd *key)
	{
		int rtn;
		InvokeHelper(21, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4,
			(long)key);
		return rtn;
	}

	BOOL	SetRowCount(CWnd *key, int nRows = 10)
	{
		BOOL rtn;
		InvokeHelper(22, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4,
			(long)key, nRows);
		return rtn;
	}

	int	GetColumnCount(CWnd *key)
	{
		int rtn;
		InvokeHelper(23, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4,
			(long)key);
		return rtn;
	}

	BOOL	SetColumnCount(CWnd *key, int nCols = 10)
	{
		BOOL rtn;
		InvokeHelper(24, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4,
			(long)key, nCols);
		return rtn;
	}

	int	GetFixedRowCount(CWnd *key)
	{
		int rtn;
		InvokeHelper(25, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4,
			(long)key);
		return rtn;
	}

	BOOL	SetFixedRowCount(CWnd *key, int nFixedRows = 1)
	{
		BOOL rtn;
		InvokeHelper(26, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4,
			(long)key, nFixedRows);
		return rtn;
	}

	int	GetFixedColumnCount(CWnd *key)
	{
		int rtn;
		InvokeHelper(27, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4,
			(long)key);
		return rtn;
	}

	BOOL	SetFixedColumnCount(CWnd *key, int nFixedCols = 1)
	{
		BOOL rtn;
		InvokeHelper(28, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4,
			(long)key, nFixedCols);
		return rtn;
	}

	int	GetRowHeight(CWnd *key, int nRow)
	{
		int rtn;
		InvokeHelper(29, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4,
			(long)key, nRow);
		return rtn;
	}

	BOOL	SetRowHeight(CWnd *key, int row, int height)
	{
		BOOL rtn;
		InvokeHelper(30, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4,
			(long)key, row, height);
		return rtn;
	}

	int	GetColumnWidth(CWnd *key, int nCol)
	{
		int rtn;
		InvokeHelper(31, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4,
			(long)key, nCol);
		return rtn;
	}

	BOOL	SetColumnWidth(CWnd *key, int col, int width)
	{
		BOOL rtn;
		InvokeHelper(32, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4,
			(long)key, col, width);
		return rtn;
	}

	int	GetFixedRowHeight(CWnd *key)
	{
		int rtn;
		InvokeHelper(33, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4,
			(long)key);
		return rtn;
	}

	int	GetFixedColumnWidth(CWnd *key)
	{
		int rtn;
		InvokeHelper(34, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4,
			(long)key);
		return rtn;
	}

	long	GetVirtualWidth(CWnd *key)
	{
		long rtn;
		InvokeHelper(35, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4,
			(long)key);
		return rtn;
	}

	long	GetVirtualHeight(CWnd *key)
	{
		long rtn;
		InvokeHelper(36, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4,
			(long)key);
		return rtn;
	}

	COLORREF GetGridColor(CWnd *key)
	{
		long rtn;
		InvokeHelper(37, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4,
			(long)key);
		return rtn;
	}

	void	SetGridColor(CWnd *key, COLORREF color)
	{
		InvokeHelper(38, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_I4,
			(long)key, color);
	}

	void	SetConditionColor(CWnd *key, COLORREF upCOL, COLORREF dnCOL, COLORREF eqCOL)
	{
		InvokeHelper(39, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_I4 VTS_I4 VTS_I4,
			(long)key, upCOL, dnCOL, eqCOL);
	}

	COLORREF GetUpColor(CWnd *key)
	{
		long rtn;
		InvokeHelper(40, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4,
			(long)key);
		return rtn;
	}

	void	SetUpColor(CWnd *key, COLORREF color)
	{
		InvokeHelper(41, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_I4,
			(long)key, color);
	}

	COLORREF GetEqColor(CWnd *key)
	{
		long rtn;
		InvokeHelper(42, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4,
			(long)key);
		return rtn;
	}

	void	SetEqColor(CWnd *key, COLORREF color)
	{
		InvokeHelper(43, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_I4,
			(long)key, color);
	}

	COLORREF GetDnColor(CWnd *key)
	{
		long rtn;
		InvokeHelper(44, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4,
			(long)key);
		return rtn;
	}

	void	SetDnColor(CWnd *key, COLORREF color)
	{
		InvokeHelper(45, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_I4,
			(long)key, color);
	}

	int	GetGridLine(CWnd *key)
	{
		long rtn;
		InvokeHelper(46, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4,
			(long)key);
		return rtn;
	}

	void	SetGridLine(CWnd *key, int nWhichLine)
	{
		InvokeHelper(47, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_I4,
			(long)key, nWhichLine);
	}

	int	GetGridLineType(CWnd *key)
	{
		long rtn;
		InvokeHelper(48, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4,
			(long)key);
		return rtn;
	}

	void	SetGridLineType(CWnd *key, int LineType)
	{
		InvokeHelper(49, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_I4,
			(long)key, LineType);
	}

	int	GetGridScroll(CWnd *key)
	{
		long rtn;
		InvokeHelper(50, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4,
			(long)key);
		return rtn;
	}

	void	SetGridScroll(CWnd *key, int scroll)
	{
		InvokeHelper(51, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_I4,
			(long)key, scroll);
	}

	void	SetGridDIScroll(CWnd *key, BOOL bDisable)
	{
		InvokeHelper(52, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_BOOL,
			(long)key, bDisable);
	}

	void	SetGridFocusLine(CWnd *key, BOOL focus, BOOL select)
	{
		InvokeHelper(53, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_BOOL VTS_BOOL,
			(long)key, focus, select);
	}

	void	SetRowColResize(CWnd *key, BOOL enRow, BOOL enCol)
	{
		InvokeHelper(54, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_BOOL VTS_BOOL,
			(long)key, enRow, enCol);
	}

	void	SetHandleTabKey(CWnd *key, BOOL bHandleTab = TRUE)
	{
		InvokeHelper(55, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_BOOL,
			(long)key, bHandleTab);
	}

	void	SetUseOLE(CWnd *key, BOOL bUseOLE = FALSE)
	{
		InvokeHelper(56, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_BOOL,
			(long)key, bUseOLE);
	}

	void	SetTitleTips(CWnd *key, BOOL bEnable = TRUE)
	{
		InvokeHelper(57, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_BOOL,
			(long)key, bEnable);
	}

	void	SetImageList(CWnd *key, CImageList* pList)
	{
		InvokeHelper(58, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_I4,
			(long)key, (long)pList);
	}

	long GetFocusCellRow(CWnd *key)
	{
		long rtn;
		InvokeHelper(59, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4,
			(long)key);
		return rtn;
	}

	void	SetSelectRow(CWnd *key, int row)
	{
		InvokeHelper(60, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_I4,
			(long)key, row);
	}

	void	SetFocusCellEdit(CWnd *key, int row, int col, BOOL edit = TRUE)
	{
		InvokeHelper(61, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_I4 VTS_I4 VTS_BOOL,
			(long)key, row, col, edit);
	}

	void	SetFocusCellSensitive(CWnd *key, BOOL sensitive)
	{
		InvokeHelper(62, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_BOOL,
			(long)key, sensitive);
	}

	BOOL	SetImageListIndex(CWnd *key, int nRow, int nCol, int index)
	{
		BOOL rtn;
		InvokeHelper(63, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4 VTS_I4,
			(long)key, nRow, nCol, index);
		return rtn;
	}

	BOOL	GetItemRect(CWnd *key, int nRow, int nCol, LPRECT pRect)
	{
		BOOL rtn;
		InvokeHelper(64, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4 VTS_I4,
			(long)key, nRow, nCol, (long)pRect);
		return rtn;
	}

	BOOL	SetItem(CWnd *key, const GVITEM* gvitem)
	{
		BOOL rtn;
		InvokeHelper(65, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4,
			(long)key, (long)gvitem);
		return rtn;
	}

	BOOL	GetItem(CWnd *key, GVITEM* gvitem)
	{
		BOOL rtn;
		InvokeHelper(66, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4,
			(long)key, (long)gvitem);
		return rtn;
	}

	BOOL	SetColumnItems(CWnd *key, int nCol, GVITEM* gvitem)
	{
		BOOL rtn;
		InvokeHelper(67, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4,
			(long)key, nCol, (long)gvitem);
		return rtn;
	}

	BOOL	 SetItemText   (CWnd *key, int nRow, int nCol, LPCTSTR str, BOOL bRedraw = TRUE)
	{
		BOOL rtn;
		InvokeHelper(68, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4 VTS_BSTR VTS_BOOL,
			(long)key, nRow, nCol, str, bRedraw);
		return rtn;
	}

	CString  GetItemText   (CWnd *key, int nRow, int nCol)
	{
		CString txt;
		long rtn;
		InvokeHelper(69, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4,
			(long)key, nRow, nCol);
		txt = (char*)rtn;
		return txt;
	}

	BOOL	 SetItemData   (CWnd *key, int nRow, int nCol, LPARAM lParam, BOOL bRedraw = TRUE)
	{
		BOOL rtn;
		InvokeHelper(70, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BOOL,
			(long)key, nRow, nCol, lParam, bRedraw);
		return rtn;
	}

	LPARAM	 GetItemData   (CWnd *key, int nRow, int nCol)
	{
		long rtn;
		InvokeHelper(71, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4,
			(long)key, nRow, nCol);
		return rtn;
	}

	BOOL	 SetItemImage  (CWnd *key, int nRow, int nCol, int image, BOOL bRedraw = TRUE)
	{
		BOOL rtn;
		InvokeHelper(72, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BOOL,
			(long)key, nRow, nCol, image, bRedraw);
		return rtn;
	}

	int	 GetItemImage  (CWnd *key, int nRow, int nCol)
	{
		int rtn;
		InvokeHelper(73, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4,
			(long)key, nRow, nCol);
		return rtn;
	}

	BOOL	 SetItemState  (CWnd *key, int nRow, int nCol, UINT state)
	{
		BOOL rtn;
		InvokeHelper(74, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4 VTS_I4,
			(long)key, nRow, nCol, state);
		return rtn;
	}

	UINT	 GetItemState  (CWnd *key, int nRow, int nCol)
	{
		long rtn;
		InvokeHelper(75, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4,
			(long)key, nRow, nCol);
		return rtn;
	}

	BOOL	 SetItemAttr   (CWnd *key, int nRow, int nCol, UINT attr)
	{
		BOOL rtn;
		InvokeHelper(76, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4 VTS_I4,
			(long)key, nRow, nCol, attr);
		return rtn;
	}

	UINT	 GetItemAttr   (CWnd *key, int nRow, int nCol)
	{
		long rtn;
		InvokeHelper(77, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4,
			(long)key, nRow, nCol);
		return rtn;
	}

	BOOL	 SetItemFormat (CWnd *key, int nRow, int nCol, UINT format)
	{
		BOOL rtn;
		InvokeHelper(78, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4 VTS_I4,
			(long)key, nRow, nCol, format);
		return rtn;
	}

	UINT	 GetItemFormat (CWnd *key, int nRow, int nCol)
	{
		long rtn;
		InvokeHelper(79, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4,
			(long)key, nRow, nCol);
		return rtn;
	}

	BOOL	 SetItemBkColor(CWnd *key, int nRow, int nCol, COLORREF col = CLR_DEFAULT, BOOL force = FALSE, BOOL bRedraw = TRUE)
	{
		BOOL rtn;
		InvokeHelper(80, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BOOL VTS_BOOL,
			(long)key, nRow, nCol, col, force, bRedraw);
		return rtn;
	}

	COLORREF GetItemBkColor(CWnd *key, int nRow, int nCol)
	{
		long rtn;
		InvokeHelper(81, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4,
			(long)key, nRow, nCol);
		return rtn;
	}

	BOOL	 SetItemFgColor(CWnd *key, int nRow, int nCol, COLORREF col = CLR_DEFAULT, BOOL force = FALSE, BOOL bRedraw = TRUE)
	{
		BOOL rtn;
		InvokeHelper(82, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BOOL VTS_BOOL,
			(long)key, nRow, nCol, col, force, bRedraw);
		return rtn;
	}

	COLORREF GetItemFgColor(CWnd *key, int nRow, int nCol)
	{
		long rtn;
		InvokeHelper(83, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4,
			(long)key, nRow, nCol);
		return rtn;
	}

	BOOL	 SetItemFont   (CWnd *key, int nRow, int nCol, LOGFONT* logfont, BOOL bRedraw = TRUE)
	{
		BOOL rtn;
		InvokeHelper(84, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BOOL,
			(long)key, nRow, nCol, (long)logfont, bRedraw);
		return rtn;
	}

	LOGFONT* GetItemFont   (CWnd *key, int nRow, int nCol)
	{
		long rtn;
		InvokeHelper(85, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4,
			(long)key, nRow, nCol);
		return (LOGFONT*)rtn;
	}

	BOOL	 SetItemParam  (CWnd *key, int nRow, int nCol, LPARAM param, BOOL bRedraw = TRUE)
	{
		BOOL rtn;
		InvokeHelper(86, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BOOL,
			(long)key, nRow, nCol, param, bRedraw);
		return rtn;
	}

	LPARAM	 GetItemParam  (CWnd *key, int nRow, int nCol)
	{
		long rtn;
		InvokeHelper(87, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4,
			(long)key, nRow, nCol);
		return rtn;
	}

	BOOL	SetItemSymbol  (CWnd *key, int nRow, int nCol, LPCTSTR str)
	{
		BOOL rtn;
		InvokeHelper(88, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4 VTS_BSTR,
			(long)key, nRow, nCol, str);
		return rtn;
	}

	CString	GetItemSymbol  (CWnd *key, int nRow, int nCol)
	{
		CString txt;
		long rtn;
		InvokeHelper(89, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4,
			(long)key, nRow, nCol);
		txt = (char*)rtn;
		return txt;
	}

	BOOL	SetItemMisc    (CWnd *key, int nRow, int nCol, LPCTSTR str, BOOL bRedraw = TRUE)
	{
		BOOL rtn;
		InvokeHelper(90, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4 VTS_BSTR VTS_BOOL,
			(long)key, nRow, nCol, str, bRedraw);
		return rtn;
	}

	CString	GetItemMisc    (CWnd *key, int nRow, int nCol)
	{
		CString txt;
		long rtn;
		InvokeHelper(91, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_I4,
			(long)key, nRow, nCol);
		txt = (char*)rtn;
		return txt;
	}

	int	InsertColumn(CWnd *key, LPCTSTR strHeading, UINT nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE, int nColumn = -1)
	{
		long rtn;
		InvokeHelper(92, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4 VTS_BSTR VTS_I4 VTS_I4,
			(long)key, strHeading, nFormat, nColumn);
		return rtn;
	}

	int	InsertRow(CWnd *key, LPCTSTR strHeading, int nRow = -1, BOOL noRefresh = FALSE)
	{
		long rtn;
		InvokeHelper(93, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL,
			(long)key, strHeading, nRow, noRefresh);
		return rtn;
	}

	BOOL	InsertRow(CWnd *key, int drag, int& drop)
	{
		BOOL rtn;
		InvokeHelper(94, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_PI4,
			(long)key, drag, &drop);
		return rtn;
	}

	BOOL	DeleteColumn(CWnd *key, int nColumn)
	{
		BOOL rtn;
		InvokeHelper(95, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4,
			(long)key, nColumn);
		return rtn;
	}

	BOOL	DeleteRow(CWnd *key, int nRow, BOOL noRefresh = FALSE)
	{
		BOOL rtn;
		InvokeHelper(96, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4 VTS_BOOL,
			(long)key, nRow, noRefresh);
		return rtn;
	}

	BOOL	DeleteNonFixedRows(CWnd *key)
	{
		BOOL rtn;
		InvokeHelper(97, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4,
			(long)key);
		return rtn;
	}

	BOOL	DeleteAllItems(CWnd *key)
	{
		BOOL rtn;
		InvokeHelper(98, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4,
			(long)key);
		return rtn;
	}

	BOOL	AutoSizeRow(CWnd *key, int nRow)
	{
		BOOL rtn;
		InvokeHelper(99, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4,
			(long)key, nRow);
		return rtn;
	}

	BOOL	AutoSizeColumn(CWnd *key, int nCol)
	{
		BOOL rtn;
		InvokeHelper(100, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4 VTS_I4,
			(long)key, nCol);
		return rtn;
	}

	void	AutoSizeRows(CWnd *key)
	{
		InvokeHelper(101, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4,
			(long)key);
	}

	void	AutoSizeColumns(CWnd *key)
	{
		InvokeHelper(102, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4,
			(long)key);
	}

	void	AutoSize(CWnd *key)
	{
		InvokeHelper(103, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4,
			(long)key);
	}

	void	SetFGSelColor(CWnd *key, COLORREF color)
	{
		InvokeHelper(104, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_I4,
			(long)key, color);
	}

	void	SetBKSelColor(CWnd *key, COLORREF color)
	{
		InvokeHelper(105, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_I4,
			(long)key, color);
	}

	COLORREF GetFGSelColor(CWnd *key)
	{
		int rtn;
		InvokeHelper(106, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4,
			(long)key);
		return rtn;
	}

	COLORREF GetBKSelColor(CWnd *key)
	{
		int rtn;
		InvokeHelper(107, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4,
			(long)key);
		return rtn;
	}

	COLORREF GetGridBKColor(CWnd *key)
	{
		int rtn;
		InvokeHelper(108, DISPATCH_METHOD, VT_BOOL, (void*)&rtn, (BYTE*) VTS_I4,
			(long)key);
		return rtn;
	}

	void	SetGridBKColor(CWnd *key, COLORREF color)
	{
		InvokeHelper(109, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_I4,
			(long)key, color);
	}

	long GetFocusCellCol(CWnd *key)
	{
		long rtn;
		InvokeHelper(110, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4,
			(long)key);
		return rtn;
	}

	void SetTabColor(CWnd *key, COLORREF fColor, COLORREF bColor, COLORREF fSelColor, COLORREF bSelColor, COLORREF bgColor)
	{
		InvokeHelper(111, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4,
			(long)key, fColor, bColor, fSelColor, bSelColor, bgColor);
	}

	void SetStaticColor(CWnd *key, COLORREF bkClr, COLORREF frClr, COLORREF bxClr)
	{
		InvokeHelper(112, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_I4 VTS_I4 VTS_I4,
			(long)key, bkClr, frClr, bxClr);
	}

	void SetStaticColor(CWnd *key, COLORREF bkClr, COLORREF frClr)
	{
		InvokeHelper(112, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, (BYTE*) VTS_I4 VTS_I4 VTS_I4 VTS_I4,
			(long)key, bkClr, frClr, bkClr);
	}

	int GetSelectRow(CWnd *key)
	{
		long rtn;
		InvokeHelper(113, DISPATCH_METHOD, VT_I4, (void*)&rtn, (BYTE*) VTS_I4,
			(long)key);
		return rtn;
	}
};

#endif // !defined(AFX_COMCTRL_H__INCLUDED_)