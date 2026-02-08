#include "stdafx.h"
#include "DispGrid.h"


/////////////////////////////////////////////////////////////////////////////
// CDispGrid
OLECHAR *CDispGrid::m_osInterfaceName[] = {
	// Variable
	L"Rows",
	L"Cols",
	L"ColFormatInfo",
	
	// Method
	L"GetBase",
	L"InsertRow",
	L"SetText",
	L"GetText",
	L"RefreshGrid",
	L"SetHeaderText",
	L"DeleteRow",
	L"SetApRowData",
	L"SetColumnWidth",
	L"GetNumberRows",
	L"SetForeColor",
	L"FindRow",
	L"GetCheckBoxState",
	L"SetBackColor",
	L"GetBackColor",
	L"SetCheckBoxState",
	L"SetData",
};


CDispGrid::CDispGrid()
{
	m_pdispatch = NULL;
}

void CDispGrid::InitInterfaceName(IDispatch *pDisp)
{
	int i, nCount = sizeof( m_osInterfaceName) / sizeof( OLECHAR *);
	m_pdID = new DISPID[ nCount];
	for( i = 0; i < nCount; i++)
		pDisp->GetIDsOfNames( IID_NULL, &m_osInterfaceName[ i], 1, LOCALE_USER_DEFAULT, m_pdID + i);
}


/////////////////////////////////////////////////////////////////////////////
// CDispGrid properties
long CDispGrid::GetRows()
{
	long result;
	GetProperty(m_pdID[IIx_Rows], VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetRows(long propVal)
{
	SetProperty(m_pdID[IIx_Rows], VT_I4, propVal);
}

short CDispGrid::GetCols()
{
	short result;
	GetProperty(m_pdID[IIx_Cols], VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetCols(short propVal)
{
	SetProperty(m_pdID[IIx_Cols], VT_I2, propVal);
}

CString CDispGrid::GetColFormatInfo()
{
	CString result;
	GetProperty(m_pdID[IIx_ColFormatInfo], VT_BSTR, (void*)&result);
	return result;
}

void CDispGrid::SetColFormatInfo(LPCTSTR propVal)
{
	SetProperty(m_pdID[IIx_ColFormatInfo], VT_BSTR, propVal);
}


/////////////////////////////////////////////////////////////////////////////
// CDispGrid Method
long CDispGrid::GetBase(LPCTSTR szKey)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_GetBase], DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		szKey);
	return result;
}

long CDispGrid::GetCtrlBasePacket()
{
	return GetBase("PACKOUT");
}

void CDispGrid::InsertRow()
{
	InvokeHelper(m_pdID[IIx_InsertRow], DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CDispGrid::SetText(short col, long row, LPCTSTR string)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4 VTS_BSTR;
	InvokeHelper(m_pdID[IIx_SetText], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 col, row, string);
}

CString CDispGrid::GetText(short nCol, long lRow)
{
	CString result;
	static BYTE parms[] =
		VTS_I2 VTS_I4;
	InvokeHelper(m_pdID[IIx_GetText], DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		nCol, lRow);
	return result;
}

void CDispGrid::RefreshGrid()
{
	InvokeHelper(m_pdID[IIx_RefreshGrid], DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CDispGrid::SetHeaderText(short col, short row, LPCTSTR caption)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_BSTR;
	InvokeHelper(m_pdID[IIx_SetHeaderText], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 col, row, caption);
}


short CDispGrid::DeleteRow(long row)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(m_pdID[IIx_DeleteRow], DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		row);
	return result;
}

void CDispGrid::SetApRowData(long nRowIndex, long nDataLength, LPCTSTR pData)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_BSTR;
	InvokeHelper(m_pdID[IIx_SetApRowData], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nRowIndex, nDataLength, pData);
}

void CDispGrid::SetColumnWidth(short nCol, short nWidth)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(m_pdID[IIx_SetColumnWidth], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nCol, nWidth);
}

long CDispGrid::GetNumberRows()
{
	long result;
	InvokeHelper(m_pdID[IIx_GetNumberRows], DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void CDispGrid::SetForeColor(short col, long row, unsigned long clrForeColor)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4 VTS_I4;
	InvokeHelper(m_pdID[IIx_SetForeColor], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 col, row, clrForeColor);
}

CString CDispGrid::FindRow(short col, LPCTSTR KeyValue)
{
	CString result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR;
	InvokeHelper(m_pdID[IIx_FindRow], DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		col, KeyValue);
	return result;
}

short CDispGrid::GetCheckBoxState(short col, long row)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I4;
	InvokeHelper(m_pdID[IIx_GetCheckBoxState], DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		col, row);
	return result;
}

unsigned long CDispGrid::GetBackColor()
{
	unsigned long result;
	GetProperty(m_pdID[IIx_GetBackColor], VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetBackColor(short col, long row, unsigned long clrBackColor)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4 VTS_I4;
	InvokeHelper(0x8d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 col, row, clrBackColor);
}

void CDispGrid::SetCheckBoxState(short nCol, long lRow, double state)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4 VTS_R8;
	InvokeHelper(m_pdID[IIx_SetCheckBoxState], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nCol, lRow, state);
}

void CDispGrid::SetData(short col, long row, LPCTSTR szData)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4 VTS_BSTR;
	InvokeHelper(m_pdID[IIx_SetData], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 col, row, szData);
}

/*
long CDispGrid::GetRows()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetRows(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

short CDispGrid::GetCols()
{
	short result;
	GetProperty(0x2, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetCols(short propVal)
{
	SetProperty(0x2, VT_I2, propVal);
}

short CDispGrid::GetScrollLockRows()
{
	short result;
	GetProperty(0x3, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetScrollLockRows(short propVal)
{
	SetProperty(0x3, VT_I2, propVal);
}

short CDispGrid::GetScrollLockCols()
{
	short result;
	GetProperty(0x4, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetScrollLockCols(short propVal)
{
	SetProperty(0x4, VT_I2, propVal);
}

short CDispGrid::GetTopHeaderRows()
{
	short result;
	GetProperty(0x5, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetTopHeaderRows(short propVal)
{
	SetProperty(0x5, VT_I2, propVal);
}

short CDispGrid::GetSideHeaderCols()
{
	short result;
	GetProperty(0x6, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetSideHeaderCols(short propVal)
{
	SetProperty(0x6, VT_I2, propVal);
}

short CDispGrid::GetRowHeightRatio()
{
	short result;
	GetProperty(0x7, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetRowHeightRatio(short propVal)
{
	SetProperty(0x7, VT_I2, propVal);
}

short CDispGrid::GetHeaderRowHeightRatio()
{
	short result;
	GetProperty(0x8, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetHeaderRowHeightRatio(short propVal)
{
	SetProperty(0x8, VT_I2, propVal);
}

CString CDispGrid::GetTopHdrInfo()
{
	CString result;
	GetProperty(0x9, VT_BSTR, (void*)&result);
	return result;
}

void CDispGrid::SetTopHdrInfo(LPCTSTR propVal)
{
	SetProperty(0x9, VT_BSTR, propVal);
}

CString CDispGrid::GetColFormatInfo()
{
	CString result;
	GetProperty(0xa, VT_BSTR, (void*)&result);
	return result;
}

void CDispGrid::SetColFormatInfo(LPCTSTR propVal)
{
	SetProperty(0xa, VT_BSTR, propVal);
}

short CDispGrid::GetMultiRows()
{
	short result;
	GetProperty(0xb, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetMultiRows(short propVal)
{
	SetProperty(0xb, VT_I2, propVal);
}

short CDispGrid::GetBorderStyle()
{
	short result;
	GetProperty(0xc, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetBorderStyle(short propVal)
{
	SetProperty(0xc, VT_I2, propVal);
}

short CDispGrid::GetAppearance()
{
	short result;
	GetProperty(0xd, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetAppearance(short propVal)
{
	SetProperty(0xd, VT_I2, propVal);
}

BOOL CDispGrid::GetFlatHeader()
{
	BOOL result;
	GetProperty(0xe, VT_BOOL, (void*)&result);
	return result;
}

void CDispGrid::SetFlatHeader(BOOL propVal)
{
	SetProperty(0xe, VT_BOOL, propVal);
}

short CDispGrid::GetUpdateMethod()
{
	short result;
	GetProperty(0xf, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetUpdateMethod(short propVal)
{
	SetProperty(0xf, VT_I2, propVal);
}

BOOL CDispGrid::GetHeaderJoin()
{
	BOOL result;
	GetProperty(0x10, VT_BOOL, (void*)&result);
	return result;
}

void CDispGrid::SetHeaderJoin(BOOL propVal)
{
	SetProperty(0x10, VT_BOOL, propVal);
}

short CDispGrid::GetSharedIndex()
{
	short result;
	GetProperty(0x11, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetSharedIndex(short propVal)
{
	SetProperty(0x11, VT_I2, propVal);
}

unsigned long CDispGrid::GetBackColor()
{
	unsigned long result;
	GetProperty(0x12, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetBackColor(unsigned long propVal)
{
	SetProperty(0x12, VT_I4, propVal);
}

BOOL CDispGrid::GetEnableResize()
{
	BOOL result;
	GetProperty(0x13, VT_BOOL, (void*)&result);
	return result;
}

void CDispGrid::SetEnableResize(BOOL propVal)
{
	SetProperty(0x13, VT_BOOL, propVal);
}

short CDispGrid::GetLineInterval()
{
	short result;
	GetProperty(0x14, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetLineInterval(short propVal)
{
	SetProperty(0x14, VT_I2, propVal);
}

short CDispGrid::GetToolTipMethod()
{
	short result;
	GetProperty(0x15, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetToolTipMethod(short propVal)
{
	SetProperty(0x15, VT_I2, propVal);
}

short CDispGrid::GetTooltipTextColumn()
{
	short result;
	GetProperty(0x16, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetTooltipTextColumn(short propVal)
{
	SetProperty(0x16, VT_I2, propVal);
}

BOOL CDispGrid::GetVScrollAlwaysShow()
{
	BOOL result;
	GetProperty(0x17, VT_BOOL, (void*)&result);
	return result;
}

void CDispGrid::SetVScrollAlwaysShow(BOOL propVal)
{
	SetProperty(0x17, VT_BOOL, propVal);
}

BOOL CDispGrid::GetHScrollAlwaysShow()
{
	BOOL result;
	GetProperty(0x18, VT_BOOL, (void*)&result);
	return result;
}

void CDispGrid::SetHScrollAlwaysShow(BOOL propVal)
{
	SetProperty(0x18, VT_BOOL, propVal);
}

BOOL CDispGrid::GetMoveScrollDataLine()
{
	BOOL result;
	GetProperty(0x19, VT_BOOL, (void*)&result);
	return result;
}

void CDispGrid::SetMoveScrollDataLine(BOOL propVal)
{
	SetProperty(0x19, VT_BOOL, propVal);
}

BOOL CDispGrid::GetEventAtEndScroll()
{
	BOOL result;
	GetProperty(0x1a, VT_BOOL, (void*)&result);
	return result;
}

void CDispGrid::SetEventAtEndScroll(BOOL propVal)
{
	SetProperty(0x1a, VT_BOOL, propVal);
}

short CDispGrid::GetTooltipShowColumn()
{
	short result;
	GetProperty(0x1b, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetTooltipShowColumn(short propVal)
{
	SetProperty(0x1b, VT_I2, propVal);
}

short CDispGrid::GetInputDirection()
{
	short result;
	GetProperty(0x1c, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetInputDirection(short propVal)
{
	SetProperty(0x1c, VT_I2, propVal);
}

short CDispGrid::GetRowLimit()
{
	short result;
	GetProperty(0x1d, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetRowLimit(short propVal)
{
	SetProperty(0x1d, VT_I2, propVal);
}

short CDispGrid::GetSharedEventMethod()
{
	short result;
	GetProperty(0x1e, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetSharedEventMethod(short propVal)
{
	SetProperty(0x1e, VT_I2, propVal);
}

short CDispGrid::GetSharedPrimaryCol()
{
	short result;
	GetProperty(0x1f, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetSharedPrimaryCol(short propVal)
{
	SetProperty(0x1f, VT_I2, propVal);
}

CString CDispGrid::GetSharedOneName()
{
	CString result;
	GetProperty(0x20, VT_BSTR, (void*)&result);
	return result;
}

void CDispGrid::SetSharedOneName(LPCTSTR propVal)
{
	SetProperty(0x20, VT_BSTR, propVal);
}

CString CDispGrid::GetSharedGetDataCols()
{
	CString result;
	GetProperty(0x21, VT_BSTR, (void*)&result);
	return result;
}

void CDispGrid::SetSharedGetDataCols(LPCTSTR propVal)
{
	SetProperty(0x21, VT_BSTR, propVal);
}

short CDispGrid::GetSelectionMethod()
{
	short result;
	GetProperty(0x22, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetSelectionMethod(short propVal)
{
	SetProperty(0x22, VT_I2, propVal);
}

BOOL CDispGrid::GetSelectClickTopHdr()
{
	BOOL result;
	GetProperty(0x23, VT_BOOL, (void*)&result);
	return result;
}

void CDispGrid::SetSelectClickTopHdr(BOOL propVal)
{
	SetProperty(0x23, VT_BOOL, propVal);
}

BOOL CDispGrid::GetSelectClickSideHdr()
{
	BOOL result;
	GetProperty(0x24, VT_BOOL, (void*)&result);
	return result;
}

void CDispGrid::SetSelectClickSideHdr(BOOL propVal)
{
	SetProperty(0x24, VT_BOOL, propVal);
}

BOOL CDispGrid::GetLTCornerColor()
{
	BOOL result;
	GetProperty(0x25, VT_BOOL, (void*)&result);
	return result;
}

void CDispGrid::SetLTCornerColor(BOOL propVal)
{
	SetProperty(0x25, VT_BOOL, propVal);
}

BOOL CDispGrid::GetEnableBlink()
{
	BOOL result;
	GetProperty(0x26, VT_BOOL, (void*)&result);
	return result;
}

void CDispGrid::SetEnableBlink(BOOL propVal)
{
	SetProperty(0x26, VT_BOOL, propVal);
}

BOOL CDispGrid::GetUseDblClickSend()
{
	BOOL result;
	GetProperty(0x27, VT_BOOL, (void*)&result);
	return result;
}

void CDispGrid::SetUseDblClickSend(BOOL propVal)
{
	SetProperty(0x27, VT_BOOL, propVal);
}

short CDispGrid::GetKeyColumn()
{
	short result;
	GetProperty(0x28, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetKeyColumn(short propVal)
{
	SetProperty(0x28, VT_I2, propVal);
}

CString CDispGrid::GetKeyControl()
{
	CString result;
	GetProperty(0x29, VT_BSTR, (void*)&result);
	return result;
}

void CDispGrid::SetKeyControl(LPCTSTR propVal)
{
	SetProperty(0x29, VT_BSTR, propVal);
}

unsigned long CDispGrid::GetBullColor()
{
	unsigned long result;
	GetProperty(0x2a, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetBullColor(unsigned long propVal)
{
	SetProperty(0x2a, VT_I4, propVal);
}

unsigned long CDispGrid::GetSteadyColor()
{
	unsigned long result;
	GetProperty(0x2b, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetSteadyColor(unsigned long propVal)
{
	SetProperty(0x2b, VT_I4, propVal);
}

unsigned long CDispGrid::GetBearColor()
{
	unsigned long result;
	GetProperty(0x2c, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetBearColor(unsigned long propVal)
{
	SetProperty(0x2c, VT_I4, propVal);
}

BOOL CDispGrid::GetEnableSwap()
{
	BOOL result;
	GetProperty(0x2d, VT_BOOL, (void*)&result);
	return result;
}

void CDispGrid::SetEnableSwap(BOOL propVal)
{
	SetProperty(0x2d, VT_BOOL, propVal);
}

CString CDispGrid::GetSStr()
{
	CString result;
	GetProperty(0x2e, VT_BSTR, (void*)&result);
	return result;
}

void CDispGrid::SetSStr(LPCTSTR propVal)
{
	SetProperty(0x2e, VT_BSTR, propVal);
}

unsigned long CDispGrid::GetRowColor()
{
	unsigned long result;
	GetProperty(0x2f, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetRowColor(unsigned long propVal)
{
	SetProperty(0x2f, VT_I4, propVal);
}

CString CDispGrid::GetServerTag()
{
	CString result;
	GetProperty(0x30, VT_BSTR, (void*)&result);
	return result;
}

void CDispGrid::SetServerTag(LPCTSTR propVal)
{
	SetProperty(0x30, VT_BSTR, propVal);
}

BOOL CDispGrid::GetHide()
{
	BOOL result;
	GetProperty(0x31, VT_BOOL, (void*)&result);
	return result;
}

void CDispGrid::SetHide(BOOL propVal)
{
	SetProperty(0x31, VT_BOOL, propVal);
}

CString CDispGrid::GetDataName()
{
	CString result;
	GetProperty(0x32, VT_BSTR, (void*)&result);
	return result;
}

void CDispGrid::SetDataName(LPCTSTR propVal)
{
	SetProperty(0x32, VT_BSTR, propVal);
}

CString CDispGrid::GetRealDLLName()
{
	CString result;
	GetProperty(0x33, VT_BSTR, (void*)&result);
	return result;
}

void CDispGrid::SetRealDLLName(LPCTSTR propVal)
{
	SetProperty(0x33, VT_BSTR, propVal);
}

BOOL CDispGrid::GetUseRealData()
{
	BOOL result;
	GetProperty(0x34, VT_BOOL, (void*)&result);
	return result;
}

void CDispGrid::SetUseRealData(BOOL propVal)
{
	SetProperty(0x34, VT_BOOL, propVal);
}

CString CDispGrid::GetHeaderMergeInfo()
{
	CString result;
	GetProperty(0x35, VT_BSTR, (void*)&result);
	return result;
}

void CDispGrid::SetHeaderMergeInfo(LPCTSTR propVal)
{
	SetProperty(0x35, VT_BSTR, propVal);
}

CString CDispGrid::GetDataCellMergeInfo()
{
	CString result;
	GetProperty(0x36, VT_BSTR, (void*)&result);
	return result;
}

void CDispGrid::SetDataCellMergeInfo(LPCTSTR propVal)
{
	SetProperty(0x36, VT_BSTR, propVal);
}

short CDispGrid::GetPriceColNo()
{
	short result;
	GetProperty(0x37, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetPriceColNo(short propVal)
{
	SetProperty(0x37, VT_I2, propVal);
}

short CDispGrid::GetStdPriceColNo()
{
	short result;
	GetProperty(0x38, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetStdPriceColNo(short propVal)
{
	SetProperty(0x38, VT_I2, propVal);
}

short CDispGrid::GetSignColNo()
{
	short result;
	GetProperty(0x39, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetSignColNo(short propVal)
{
	SetProperty(0x39, VT_I2, propVal);
}

short CDispGrid::GetPriceChgColNo()
{
	short result;
	GetProperty(0x3a, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetPriceChgColNo(short propVal)
{
	SetProperty(0x3a, VT_I2, propVal);
}

CString CDispGrid::GetRootPath()
{
	CString result;
	GetProperty(0x3b, VT_BSTR, (void*)&result);
	return result;
}

void CDispGrid::SetRootPath(LPCTSTR propVal)
{
	SetProperty(0x3b, VT_BSTR, propVal);
}

BOOL CDispGrid::GetTableStyle()
{
	BOOL result;
	GetProperty(0x3c, VT_BOOL, (void*)&result);
	return result;
}

void CDispGrid::SetTableStyle(BOOL propVal)
{
	SetProperty(0x3c, VT_BOOL, propVal);
}

BOOL CDispGrid::GetSearchAllCol()
{
	BOOL result;
	GetProperty(0x3d, VT_BOOL, (void*)&result);
	return result;
}

void CDispGrid::SetSearchAllCol(BOOL propVal)
{
	SetProperty(0x3d, VT_BOOL, propVal);
}

BOOL CDispGrid::GetSearchAllRow()
{
	BOOL result;
	GetProperty(0x3e, VT_BOOL, (void*)&result);
	return result;
}

void CDispGrid::SetSearchAllRow(BOOL propVal)
{
	SetProperty(0x3e, VT_BOOL, propVal);
}

long CDispGrid::GetMaxLimitRows()
{
	long result;
	GetProperty(0x3f, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetMaxLimitRows(long propVal)
{
	SetProperty(0x3f, VT_I4, propVal);
}

short CDispGrid::GetScrollMethod()
{
	short result;
	GetProperty(0x40, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetScrollMethod(short propVal)
{
	SetProperty(0x40, VT_I2, propVal);
}

BOOL CDispGrid::GetUseRealDraw()
{
	BOOL result;
	GetProperty(0x41, VT_BOOL, (void*)&result);
	return result;
}

void CDispGrid::SetUseRealDraw(BOOL propVal)
{
	SetProperty(0x41, VT_BOOL, propVal);
}

BOOL CDispGrid::GetUserColorTable()
{
	BOOL result;
	GetProperty(0x42, VT_BOOL, (void*)&result);
	return result;
}

void CDispGrid::SetUserColorTable(BOOL propVal)
{
	SetProperty(0x42, VT_BOOL, propVal);
}

long CDispGrid::GetTopColorTable()
{
	long result;
	GetProperty(0x43, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetTopColorTable(long propVal)
{
	SetProperty(0x43, VT_I4, propVal);
}

long CDispGrid::GetSideColorTable()
{
	long result;
	GetProperty(0x44, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetSideColorTable(long propVal)
{
	SetProperty(0x44, VT_I4, propVal);
}

long CDispGrid::GetBackColorTable()
{
	long result;
	GetProperty(0x45, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetBackColorTable(long propVal)
{
	SetProperty(0x45, VT_I4, propVal);
}

long CDispGrid::GetLineColorTable()
{
	long result;
	GetProperty(0x46, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetLineColorTable(long propVal)
{
	SetProperty(0x46, VT_I4, propVal);
}

long CDispGrid::GetSelectColorTable()
{
	long result;
	GetProperty(0x47, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetSelectColorTable(long propVal)
{
	SetProperty(0x47, VT_I4, propVal);
}

long CDispGrid::GetRowColorTable()
{
	long result;
	GetProperty(0x48, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetRowColorTable(long propVal)
{
	SetProperty(0x48, VT_I4, propVal);
}

long CDispGrid::GetBullColorTable()
{
	long result;
	GetProperty(0x49, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetBullColorTable(long propVal)
{
	SetProperty(0x49, VT_I4, propVal);
}

long CDispGrid::GetSteadyColorTable()
{
	long result;
	GetProperty(0x4a, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetSteadyColorTable(long propVal)
{
	SetProperty(0x4a, VT_I4, propVal);
}

long CDispGrid::GetBearColorTable()
{
	long result;
	GetProperty(0x4b, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetBearColorTable(long propVal)
{
	SetProperty(0x4b, VT_I4, propVal);
}

long CDispGrid::GetBlinkColorTable()
{
	long result;
	GetProperty(0x4c, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetBlinkColorTable(long propVal)
{
	SetProperty(0x4c, VT_I4, propVal);
}

BOOL CDispGrid::GetUseAdvDrawInfo()
{
	BOOL result;
	GetProperty(0x4d, VT_BOOL, (void*)&result);
	return result;
}

void CDispGrid::SetUseAdvDrawInfo(BOOL propVal)
{
	SetProperty(0x4d, VT_BOOL, propVal);
}

CString CDispGrid::GetDllList()
{
	CString result;
	GetProperty(0x4e, VT_BSTR, (void*)&result);
	return result;
}

void CDispGrid::SetDllList(LPCTSTR propVal)
{
	SetProperty(0x4e, VT_BSTR, propVal);
}

BOOL CDispGrid::GetHideLastRow()
{
	BOOL result;
	GetProperty(0x4f, VT_BOOL, (void*)&result);
	return result;
}

void CDispGrid::SetHideLastRow(BOOL propVal)
{
	SetProperty(0x4f, VT_BOOL, propVal);
}

short CDispGrid::GetNoRecalculate()
{
	short result;
	GetProperty(0x50, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetNoRecalculate(short propVal)
{
	SetProperty(0x50, VT_I2, propVal);
}

BOOL CDispGrid::GetUseBidColumn()
{
	BOOL result;
	GetProperty(0x51, VT_BOOL, (void*)&result);
	return result;
}

void CDispGrid::SetUseBidColumn(BOOL propVal)
{
	SetProperty(0x51, VT_BOOL, propVal);
}

CString CDispGrid::GetBidColumnData()
{
	CString result;
	GetProperty(0x52, VT_BSTR, (void*)&result);
	return result;
}

void CDispGrid::SetBidColumnData(LPCTSTR propVal)
{
	SetProperty(0x52, VT_BSTR, propVal);
}

OLE_HANDLE CDispGrid::GetHWnd()
{
	OLE_HANDLE result;
	GetProperty(DISPID_HWND, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetHWnd(OLE_HANDLE propVal)
{
	SetProperty(DISPID_HWND, VT_I4, propVal);
}

CString CDispGrid::GetComment()
{
	CString result;
	GetProperty(0x53, VT_BSTR, (void*)&result);
	return result;
}

void CDispGrid::SetComment(LPCTSTR propVal)
{
	SetProperty(0x53, VT_BSTR, propVal);
}

CString CDispGrid::GetControlID()
{
	CString result;
	GetProperty(0x54, VT_BSTR, (void*)&result);
	return result;
}

void CDispGrid::SetControlID(LPCTSTR propVal)
{
	SetProperty(0x54, VT_BSTR, propVal);
}

CString CDispGrid::GetCustomFont()
{
	CString result;
	GetProperty(0x5c, VT_BSTR, (void*)&result);
	return result;
}

void CDispGrid::SetCustomFont(LPCTSTR propVal)
{
	SetProperty(0x5c, VT_BSTR, propVal);
}

long CDispGrid::GetUseDragDrop()
{
	long result;
	GetProperty(0x5d, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetUseDragDrop(long propVal)
{
	SetProperty(0x5d, VT_I4, propVal);
}

long CDispGrid::GetTabKeyStop()
{
	long result;
	GetProperty(0x5e, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetTabKeyStop(long propVal)
{
	SetProperty(0x5e, VT_I4, propVal);
}

unsigned long CDispGrid::GetHeaderColor()
{
	unsigned long result;
	GetProperty(0x55, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetHeaderColor(unsigned long propVal)
{
	SetProperty(0x55, VT_I4, propVal);
}

unsigned long CDispGrid::GetLineColor()
{
	unsigned long result;
	GetProperty(0x56, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetLineColor(unsigned long propVal)
{
	SetProperty(0x56, VT_I4, propVal);
}

unsigned long CDispGrid::GetSideHeaderColor()
{
	unsigned long result;
	GetProperty(0x57, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetSideHeaderColor(unsigned long propVal)
{
	SetProperty(0x57, VT_I4, propVal);
}

unsigned long CDispGrid::GetSelectColor()
{
	unsigned long result;
	GetProperty(0x58, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetSelectColor(unsigned long propVal)
{
	SetProperty(0x58, VT_I4, propVal);
}

short CDispGrid::GetBlinkInterval()
{
	short result;
	GetProperty(0x59, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetBlinkInterval(short propVal)
{
	SetProperty(0x59, VT_I2, propVal);
}

CString CDispGrid::GetLocalSharedList()
{
	CString result;
	GetProperty(0x5a, VT_BSTR, (void*)&result);
	return result;
}

void CDispGrid::SetLocalSharedList(LPCTSTR propVal)
{
	SetProperty(0x5a, VT_BSTR, propVal);
}

unsigned long CDispGrid::GetBlinkColor()
{
	unsigned long result;
	GetProperty(0x5b, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetBlinkColor(unsigned long propVal)
{
	SetProperty(0x5b, VT_I4, propVal);
}

long CDispGrid::GetUseDefaultFont()
{
	long result;
	GetProperty(0x5f, VT_I4, (void*)&result);
	return result;
}

void CDispGrid::SetUseDefaultFont(long propVal)
{
	SetProperty(0x5f, VT_I4, propVal);
}

short CDispGrid::GetRealTimeInterval()
{
	short result;
	GetProperty(0x60, VT_I2, (void*)&result);
	return result;
}

void CDispGrid::SetRealTimeInterval(short propVal)
{
	SetProperty(0x60, VT_I2, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CDispGrid operations

short CDispGrid::SetNumberCols(short Cols, short redraw)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x61, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Cols, redraw);
	return result;
}

short CDispGrid::GetNumberCols()
{
	short result;
	InvokeHelper(0x62, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

long CDispGrid::SetNumberRows(long Rows, short redraw)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I2;
	InvokeHelper(0x63, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Rows, redraw);
	return result;
}

long CDispGrid::GetNumberRows()
{
	long result;
	InvokeHelper(0x64, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

short CDispGrid::LockColumns(short Cols)
{
	short result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x65, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Cols);
	return result;
}

short CDispGrid::LockRows(short Rows)
{
	short result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x66, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Rows);
	return result;
}

void CDispGrid::SetText(short col, long row, LPCTSTR string)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4 VTS_BSTR;
	InvokeHelper(0x67, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 col, row, string);
}

CString CDispGrid::GetText(short nCol, long lRow)
{
	CString result;
	static BYTE parms[] =
		VTS_I2 VTS_I4;
	InvokeHelper(0x68, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		nCol, lRow);
	return result;
}

void CDispGrid::SetGridData(short nCount, long pData)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4;
	InvokeHelper(0x69, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nCount, pData);
}

BOOL CDispGrid::SetAllProperties(long dwVersion, long dwDataClass)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x6a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		dwVersion, dwDataClass);
	return result;
}

BOOL CDispGrid::SetAllProperties2(long dwData)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		dwData);
	return result;
}

void CDispGrid::SetData(short col, long row, LPCTSTR szData)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4 VTS_BSTR;
	InvokeHelper(0x6c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 col, row, szData);
}


void CDispGrid::SetBaseDesk(long lKey)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lKey);
}

void CDispGrid::TestSetDataData(short nCount, LPCTSTR szData)
{
	static BYTE parms[] =
		VTS_I2 VTS_BSTR;
	InvokeHelper(0x6f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nCount, szData);
}

void CDispGrid::SetTextWithAttr(short col, long row, short nForeColorID, short nBackColorID, short nStateID, LPCTSTR szData)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4 VTS_I2 VTS_I2 VTS_I2 VTS_BSTR;
	InvokeHelper(0x70, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 col, row, nForeColorID, nBackColorID, nStateID, szData);
}

short CDispGrid::GetCheckBoxState(short col, long row)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I4;
	InvokeHelper(0x71, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		col, row);
	return result;
}

short CDispGrid::GetDropListState(short col, long row)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I4;
	InvokeHelper(0x72, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		col, row);
	return result;
}

CString CDispGrid::GetDropListTextState(short col, long row)
{
	CString result;
	static BYTE parms[] =
		VTS_I2 VTS_I4;
	InvokeHelper(0x73, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		col, row);
	return result;
}

short CDispGrid::OnDrdsAdvise()
{
	short result;
	InvokeHelper(0x74, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short CDispGrid::OffDrdsAdvise()
{
	short result;
	InvokeHelper(0x75, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short CDispGrid::DeleteRow(long row)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x76, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		row);
	return result;
}

void CDispGrid::SetTextWithAttr2(short col, long row, unsigned long clrForeColor, unsigned long clrBackColor, short nStateID, LPCTSTR szData, long bUpdownSign)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_I2 VTS_BSTR VTS_I4;
	InvokeHelper(0x77, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 col, row, clrForeColor, clrBackColor, nStateID, szData, bUpdownSign);
}

void CDispGrid::ResetGrid()
{
	InvokeHelper(0x78, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CDispGrid::SetForeColor(short col, long row, unsigned long clrForeColor)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4 VTS_I4;
	InvokeHelper(0x79, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 col, row, clrForeColor);
}

CString CDispGrid::GetSelectedRows()
{
	CString result;
	InvokeHelper(0x7a, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CDispGrid::RefreshGrid()
{
	InvokeHelper(0x7b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CDispGrid::SetColumnWidth(short nCol, short nWidth)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x7c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nCol, nWidth);
}

long CDispGrid::GetEndTextRow()
{
	long result;
	InvokeHelper(0x7d, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void CDispGrid::InsertRow()
{
	InvokeHelper(0x7e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CDispGrid::OnDrdsAdviseOneItem(LPCTSTR szPacketName, LPCTSTR szFieldName, LPCTSTR szKeyName)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x7f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szPacketName, szFieldName, szKeyName);
}

CString CDispGrid::FindRow(short col, LPCTSTR KeyValue)
{
	CString result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR;
	InvokeHelper(0x80, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		col, KeyValue);
	return result;
}

void CDispGrid::SetCheckBoxState(short nCol, long lRow, double state)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4 VTS_R8;
	InvokeHelper(0x81, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nCol, lRow, state);
}

void CDispGrid::SetHeaderText(short col, short row, LPCTSTR caption)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_BSTR;
	InvokeHelper(0x82, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 col, row, caption);
}

void CDispGrid::SetEndTextRow(long row)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x83, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 row);
}

void CDispGrid::SetColumnData(long startRow, short colNo, LPCTSTR szData, short step)
{
	static BYTE parms[] =
		VTS_I4 VTS_I2 VTS_BSTR VTS_I2;
	InvokeHelper(0x84, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 startRow, colNo, szData, step);
}

long CDispGrid::SetCellFocus(short col, long row)
{
	long result;
	static BYTE parms[] =
		VTS_I2 VTS_I4;
	InvokeHelper(0x85, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		col, row);
	return result;
}

void CDispGrid::SetLabelText(short col, long row, LPCTSTR szData)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4 VTS_BSTR;
	InvokeHelper(0x86, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 col, row, szData);
}

void CDispGrid::SetEditable(short nCol, long lRow, short nState)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4 VTS_I2;
	InvokeHelper(0x87, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nCol, lRow, nState);
}

void CDispGrid::SetDllData(LPCTSTR szDllName, LPCTSTR szKey, LPCTSTR szData, long dwFlag)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4;
	InvokeHelper(0x88, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szDllName, szKey, szData, dwFlag);
}

void CDispGrid::SetTextNoMasking(short col, long row, LPCTSTR szData)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4 VTS_BSTR;
	InvokeHelper(0x89, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 col, row, szData);
}

void CDispGrid::StartEdit(short col, long row)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4;
	InvokeHelper(0x8a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 col, row);
}

void CDispGrid::MakeSaveShortKey(LPCTSTR szFileName, long nFlag)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0x8b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szFileName, nFlag);
}

void CDispGrid::SortColumn(short col, short nFlag)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x8c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 col, nFlag);
}

void CDispGrid::SetBackColor(short col, long row, unsigned long clrBackColor)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4 VTS_I4;
	InvokeHelper(0x8d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 col, row, clrBackColor);
}

void CDispGrid::SetCellBright(short col, short row)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x8e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 col, row);
}

void CDispGrid::SetCellUseColorTableFlag(short col, long row, short flag)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4 VTS_I2;
	InvokeHelper(0x8f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 col, row, flag);
}

void CDispGrid::SetColWidth(short startcol, short endcol, LPCTSTR widthData)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_BSTR;
	InvokeHelper(0x90, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 startcol, endcol, widthData);
}

BOOL CDispGrid::SetOneColumnFormat(long lData)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x91, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		lData);
	return result;
}

void CDispGrid::AppendRow()
{
	InvokeHelper(0x92, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long CDispGrid::InsertRowIndex(long row)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x93, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		row);
	return result;
}

void CDispGrid::UpdateColorTable()
{
	InvokeHelper(0x94, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CDispGrid::ChangeUpdateMethod(long nMethod)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x95, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nMethod);
}

void CDispGrid::OnGetCellIndex(short* nCol, short Xpoint, short Ypoint)
{
	static BYTE parms[] =
		VTS_PI2 VTS_I2 VTS_I2;
	InvokeHelper(0x96, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nCol, Xpoint, Ypoint);
}

long CDispGrid::OnReceiveFetchData(long pData)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x97, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		pData);
	return result;
}

void CDispGrid::SetRscMng(long lKey)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x98, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lKey);
}

void CDispGrid::SetColForeColor(short col, unsigned long color)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4;
	InvokeHelper(0x99, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 col, color);
}

void CDispGrid::SetColBackColor(short col, unsigned long color)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4;
	InvokeHelper(0x9a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 col, color);
}

long CDispGrid::GetAllProperties2(long lpData)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x9b, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lpData);
	return result;
}

long CDispGrid::GetBasePropIf()
{
	long result;
	InvokeHelper(0x9c, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void CDispGrid::ModifyRect()
{
	InvokeHelper(0x9d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long CDispGrid::GetChangeGridHeight(long lOldRow, long lNewRow)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x9e, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lOldRow, lNewRow);
	return result;
}

void CDispGrid::AddDrdsCondition(short nCol, double fData, short nCondition, short nType)
{
	static BYTE parms[] =
		VTS_I2 VTS_R8 VTS_I2 VTS_I2;
	InvokeHelper(0x9f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nCol, fData, nCondition, nType);
}

short CDispGrid::AddDrdsLogic(short nType, short nCol1, short nCol2)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(0xa0, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		nType, nCol1, nCol2);
	return result;
}

void CDispGrid::RemoveDrdsCondition()
{
	InvokeHelper(0xa1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CDispGrid::SetAccConclusion(short relationCol1, short relationCol2, short SaleCol, short BidCol)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(0xa2, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 relationCol1, relationCol2, SaleCol, BidCol);
}

void CDispGrid::SetVolumeReversion(short nBidPriceCol, short nOfferPriceCol, short nLotVolumeCol)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(0xa3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nBidPriceCol, nOfferPriceCol, nLotVolumeCol);
}
*/
