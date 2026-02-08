#if !defined(AFX_DTSGRID_H__BY_JUNOK_LEE__20051228_57ECF7257A__INCLUDED_)
#define AFX_DTSGRID_H__BY_JUNOK_LEE__20051228_57ECF7257A__INCLUDED_

// #include "../../_Include/DispGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CDispGrid wrapper class

class CDispGrid : public COleDispatchDriver
{
public:
	CDispGrid();
	
//{{ Dispid 자동처리
public:
	LPDISPATCH m_pdispatch;

	// 1. 변수선언. m_pdID, m_osInterfaceName[]
	DISPID*			m_pdID;
	static OLECHAR *	m_osInterfaceName[];

	// 2. enum 추가, 필요한 것만 리스트업.
	typedef enum {
		// Variable
		IIx_Rows=0,
		IIx_Cols,
		IIx_ColFormatInfo,

		// Method
		IIx_GetBase,
		IIx_InsertRow,
		IIx_SetText,
		IIx_GetText,
		IIx_RefreshGrid,
		IIx_SetHeaderText,
		IIx_DeleteRow,
		IIx_SetApRowData,
		IIx_SetColumnWidth,
		IIx_GetNumberRows,
		IIx_SetForeColor,
		IIx_FindRow,
		IIx_GetCheckBoxState,
		IIx_SetBackColor,
		IIx_GetBackColor,
		IIx_SetCheckBoxState,
		IIx_SetData,
	};

	// 3. void InitInterfaceName(IDispatch *pDisp);
	void InitInterfaceName(IDispatch *pDisp);

	// 4.
//	COleDispatchDriver m_dispDriver;

//}} Dispid자동처리
public:
// Attributes
	long GetRows();
	void SetRows(long);
	short GetCols();
	void SetCols(short);
	CString GetColFormatInfo();
	long GetNumberRows();
	void SetColFormatInfo(LPCTSTR);
	void SetColumnWidth(short nCol, short nWidth);

	void SetHeaderText(short col, short row, LPCTSTR caption);
	void SetForeColor(short col, long row, unsigned long clrForeColor);
	void SetBackColor(short col, long row, unsigned long clrBackColor);
	unsigned long GetBackColor();
	
	void SetData(short col, long row, LPCTSTR szData);

// Operations
	long GetBase(LPCTSTR szKey);
	void InsertRow();
	short DeleteRow(long row);
	void SetText(short col, long row, LPCTSTR string);
	CString GetText(short nCol, long lRow);

// Implementation
	long GetCtrlBasePacket();
	void RefreshGrid();
	void SetApRowData(long nRowIndex, long nDataLength, LPCTSTR pData);
	CString FindRow(short col, LPCTSTR KeyValue);
	short GetCheckBoxState(short col, long row);
	void SetCheckBoxState(short nCol, long lRow, double state);
};

#endif // !defined(AFX_DTSGRID_H__BY_JUNOK_LEE__20051228_57ECF7257A__INCLUDED_)

/*
// Attributes
public:
	short GetScrollLockRows();
	void SetScrollLockRows(short);
	short GetScrollLockCols();
	void SetScrollLockCols(short);
	short GetTopHeaderRows();
	void SetTopHeaderRows(short);
	short GetSideHeaderCols();
	void SetSideHeaderCols(short);
	short GetRowHeightRatio();
	void SetRowHeightRatio(short);
	short GetHeaderRowHeightRatio();
	void SetHeaderRowHeightRatio(short);
	CString GetTopHdrInfo();
	void SetTopHdrInfo(LPCTSTR);
	CString GetColFormatInfo();
	void SetColFormatInfo(LPCTSTR);
	short GetMultiRows();
	void SetMultiRows(short);
	short GetBorderStyle();
	void SetBorderStyle(short);
	short GetAppearance();
	void SetAppearance(short);
	BOOL GetFlatHeader();
	void SetFlatHeader(BOOL);
	short GetUpdateMethod();
	void SetUpdateMethod(short);
	BOOL GetHeaderJoin();
	void SetHeaderJoin(BOOL);
	short GetSharedIndex();
	void SetSharedIndex(short);
	unsigned long GetBackColor();
	void SetBackColor(unsigned long);
	BOOL GetEnableResize();
	void SetEnableResize(BOOL);
	short GetLineInterval();
	void SetLineInterval(short);
	short GetToolTipMethod();
	void SetToolTipMethod(short);
	short GetTooltipTextColumn();
	void SetTooltipTextColumn(short);
	BOOL GetVScrollAlwaysShow();
	void SetVScrollAlwaysShow(BOOL);
	BOOL GetHScrollAlwaysShow();
	void SetHScrollAlwaysShow(BOOL);
	BOOL GetMoveScrollDataLine();
	void SetMoveScrollDataLine(BOOL);
	BOOL GetEventAtEndScroll();
	void SetEventAtEndScroll(BOOL);
	short GetTooltipShowColumn();
	void SetTooltipShowColumn(short);
	short GetInputDirection();
	void SetInputDirection(short);
	short GetRowLimit();
	void SetRowLimit(short);
	short GetSharedEventMethod();
	void SetSharedEventMethod(short);
	short GetSharedPrimaryCol();
	void SetSharedPrimaryCol(short);
	CString GetSharedOneName();
	void SetSharedOneName(LPCTSTR);
	CString GetSharedGetDataCols();
	void SetSharedGetDataCols(LPCTSTR);
	short GetSelectionMethod();
	void SetSelectionMethod(short);
	BOOL GetSelectClickTopHdr();
	void SetSelectClickTopHdr(BOOL);
	BOOL GetSelectClickSideHdr();
	void SetSelectClickSideHdr(BOOL);
	BOOL GetLTCornerColor();
	void SetLTCornerColor(BOOL);
	BOOL GetEnableBlink();
	void SetEnableBlink(BOOL);
	BOOL GetUseDblClickSend();
	void SetUseDblClickSend(BOOL);
	short GetKeyColumn();
	void SetKeyColumn(short);
	CString GetKeyControl();
	void SetKeyControl(LPCTSTR);
	unsigned long GetBullColor();
	void SetBullColor(unsigned long);
	unsigned long GetSteadyColor();
	void SetSteadyColor(unsigned long);
	unsigned long GetBearColor();
	void SetBearColor(unsigned long);
	BOOL GetEnableSwap();
	void SetEnableSwap(BOOL);
	CString GetSStr();
	void SetSStr(LPCTSTR);
	unsigned long GetRowColor();
	void SetRowColor(unsigned long);
	CString GetServerTag();
	void SetServerTag(LPCTSTR);
	BOOL GetHide();
	void SetHide(BOOL);
	CString GetDataName();
	void SetDataName(LPCTSTR);
	CString GetRealDLLName();
	void SetRealDLLName(LPCTSTR);
	BOOL GetUseRealData();
	void SetUseRealData(BOOL);
	CString GetHeaderMergeInfo();
	void SetHeaderMergeInfo(LPCTSTR);
	CString GetDataCellMergeInfo();
	void SetDataCellMergeInfo(LPCTSTR);
	short GetPriceColNo();
	void SetPriceColNo(short);
	short GetStdPriceColNo();
	void SetStdPriceColNo(short);
	short GetSignColNo();
	void SetSignColNo(short);
	short GetPriceChgColNo();
	void SetPriceChgColNo(short);
	CString GetRootPath();
	void SetRootPath(LPCTSTR);
	BOOL GetTableStyle();
	void SetTableStyle(BOOL);
	BOOL GetSearchAllCol();
	void SetSearchAllCol(BOOL);
	BOOL GetSearchAllRow();
	void SetSearchAllRow(BOOL);
	long GetMaxLimitRows();
	void SetMaxLimitRows(long);
	short GetScrollMethod();
	void SetScrollMethod(short);
	BOOL GetUseRealDraw();
	void SetUseRealDraw(BOOL);
	BOOL GetUserColorTable();
	void SetUserColorTable(BOOL);
	long GetTopColorTable();
	void SetTopColorTable(long);
	long GetSideColorTable();
	void SetSideColorTable(long);
	long GetBackColorTable();
	void SetBackColorTable(long);
	long GetLineColorTable();
	void SetLineColorTable(long);
	long GetSelectColorTable();
	void SetSelectColorTable(long);
	long GetRowColorTable();
	void SetRowColorTable(long);
	long GetBullColorTable();
	void SetBullColorTable(long);
	long GetSteadyColorTable();
	void SetSteadyColorTable(long);
	long GetBearColorTable();
	void SetBearColorTable(long);
	long GetBlinkColorTable();
	void SetBlinkColorTable(long);
	BOOL GetUseAdvDrawInfo();
	void SetUseAdvDrawInfo(BOOL);
	CString GetDllList();
	void SetDllList(LPCTSTR);
	BOOL GetHideLastRow();
	void SetHideLastRow(BOOL);
	short GetNoRecalculate();
	void SetNoRecalculate(short);
	BOOL GetUseBidColumn();
	void SetUseBidColumn(BOOL);
	CString GetBidColumnData();
	void SetBidColumnData(LPCTSTR);
	OLE_HANDLE GetHWnd();
	void SetHWnd(OLE_HANDLE);
	CString GetComment();
	void SetComment(LPCTSTR);
	CString GetControlID();
	void SetControlID(LPCTSTR);
	CString GetCustomFont();
	void SetCustomFont(LPCTSTR);
	long GetUseDragDrop();
	void SetUseDragDrop(long);
	long GetTabKeyStop();
	void SetTabKeyStop(long);
	unsigned long GetHeaderColor();
	void SetHeaderColor(unsigned long);
	unsigned long GetLineColor();
	void SetLineColor(unsigned long);
	unsigned long GetSideHeaderColor();
	void SetSideHeaderColor(unsigned long);
	unsigned long GetSelectColor();
	void SetSelectColor(unsigned long);
	short GetBlinkInterval();
	void SetBlinkInterval(short);
	CString GetLocalSharedList();
	void SetLocalSharedList(LPCTSTR);
	unsigned long GetBlinkColor();
	void SetBlinkColor(unsigned long);
	long GetUseDefaultFont();
	void SetUseDefaultFont(long);
	short GetRealTimeInterval();
	void SetRealTimeInterval(short);

// Operations
public:
	short SetNumberCols(short Cols, short redraw);
	short GetNumberCols();
	long SetNumberRows(long Rows, short redraw);
	long GetNumberRows();
	short LockColumns(short Cols);
	short LockRows(short Rows);
	void SetText(short col, long row, LPCTSTR string);
	CString GetText(short nCol, long lRow);
	void SetGridData(short nCount, long pData);
	BOOL SetAllProperties(long dwVersion, long dwDataClass);
	BOOL SetAllProperties2(long dwData);
	void SetData(short col, long row, LPCTSTR szData);
	long GetBase(LPCTSTR szKey);
	void SetBaseDesk(long lKey);
	void TestSetDataData(short nCount, LPCTSTR szData);
	void SetTextWithAttr(short col, long row, short nForeColorID, short nBackColorID, short nStateID, LPCTSTR szData);
	short GetCheckBoxState(short col, long row);
	short GetDropListState(short col, long row);
	CString GetDropListTextState(short col, long row);
	short OnDrdsAdvise();
	short OffDrdsAdvise();
	short DeleteRow(long row);
	void SetTextWithAttr2(short col, long row, unsigned long clrForeColor, unsigned long clrBackColor, short nStateID, LPCTSTR szData, long bUpdownSign);
	void ResetGrid();
	void SetForeColor(short col, long row, unsigned long clrForeColor);
	CString GetSelectedRows();
	void RefreshGrid();
	void SetColumnWidth(short nCol, short nWidth);
	long GetEndTextRow();
	void InsertRow();
	void OnDrdsAdviseOneItem(LPCTSTR szPacketName, LPCTSTR szFieldName, LPCTSTR szKeyName);
	CString FindRow(short col, LPCTSTR KeyValue);
	void SetCheckBoxState(short nCol, long lRow, double state);
	void SetHeaderText(short col, short row, LPCTSTR caption);
	void SetEndTextRow(long row);
	void SetColumnData(long startRow, short colNo, LPCTSTR szData, short step);
	long SetCellFocus(short col, long row);
	void SetLabelText(short col, long row, LPCTSTR szData);
	void SetEditable(short nCol, long lRow, short nState);
	void SetDllData(LPCTSTR szDllName, LPCTSTR szKey, LPCTSTR szData, long dwFlag);
	void SetTextNoMasking(short col, long row, LPCTSTR szData);
	void StartEdit(short col, long row);
	void MakeSaveShortKey(LPCTSTR szFileName, long nFlag);
	void SortColumn(short col, short nFlag);
	void SetBackColor(short col, long row, unsigned long clrBackColor);
	void SetCellBright(short col, short row);
	void SetCellUseColorTableFlag(short col, long row, short flag);
	void SetColWidth(short startcol, short endcol, LPCTSTR widthData);
	BOOL SetOneColumnFormat(long lData);
	void AppendRow();
	long InsertRowIndex(long row);
	void UpdateColorTable();
	void ChangeUpdateMethod(long nMethod);
	void OnGetCellIndex(short* nCol, short Xpoint, short Ypoint);
	long OnReceiveFetchData(long pData);
	void SetRscMng(long lKey);
	void SetColForeColor(short col, unsigned long color);
	void SetColBackColor(short col, unsigned long color);
	long GetAllProperties2(long lpData);
	long GetBasePropIf();
	void ModifyRect();
	long GetChangeGridHeight(long lOldRow, long lNewRow);
	void AddDrdsCondition(short nCol, double fData, short nCondition, short nType);
	short AddDrdsLogic(short nType, short nCol1, short nCol2);
	void RemoveDrdsCondition();
	void SetAccConclusion(short relationCol1, short relationCol2, short SaleCol, short BidCol);
	void SetVolumeReversion(short nBidPriceCol, short nOfferPriceCol, short nLotVolumeCol);
*/
