#if !defined(AFX_NTSTABLE_H__C1A5A5B3_BA57_4C00_9EBE_102BF3C487A4__INCLUDED_)
#define AFX_NTSTABLE_H__C1A5A5B3_BA57_4C00_9EBE_102BF3C487A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// Dispatch interfaces referenced by this interface
class COleFont;

/////////////////////////////////////////////////////////////////////////////
// CDispGrid wrapper class

class CDispTable : public COleDispatchDriver
{
//{{ Dispid 자동처리
public:
	CDispTable();
	LPDISPATCH m_pdispatch;

	// 1. 변수선언. m_pdID, m_osInterfaceName[]
	DISPID*			m_pdID;
	static OLECHAR *	m_osInterfaceName[];

	// 2. enum 추가, 필요한 것만 리스트업.
	typedef enum {
		
		IIx_GetCols=0,
		IIx_SetCols,
		IIx_GetRows,

		IIx_SetRows,
		IIx_GetText,
		IIx_SetText,
		IIx_ResetTable,
		IIx_GetBase,
	};

	// 3. void InitInterfaceName(IDispatch *pDisp);
	void InitInterfaceName(IDispatch *pDisp);

	// 4.
	//	COleDispatchDriver m_dispDriver;

public:
	short GetCols();
	void SetCols(short);
	short GetRows();
	void SetRows(short);
	CString GetText(short nCol, short nRow);
	void SetText(short nCol, short nRow, LPCTSTR szData);
	void ResetTable();

// Attributes
public:
	long GetCtrlBasePacket();
	long GetBase(LPCTSTR szKey);


};

class COleFont : public COleDispatchDriver
{
public:
	COleFont() {}		// Calls COleDispatchDriver default constructor
	COleFont(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	COleFont(const COleFont& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetName();
	void SetName(LPCTSTR);
	CY GetSize();
	void SetSize(const CY&);
	BOOL GetBold();
	void SetBold(BOOL);
	BOOL GetItalic();
	void SetItalic(BOOL);
	BOOL GetUnderline();
	void SetUnderline(BOOL);
	BOOL GetStrikethrough();
	void SetStrikethrough(BOOL);
	short GetWeight();
	void SetWeight(short);
	short GetCharset();
	void SetCharset(short);

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NTSTABLE_H__C1A5A5B3_BA57_4C00_9EBE_102BF3C487A4__INCLUDED_)


/*
public:
	OLE_COLOR GetBackColor();
	void SetBackColor(OLE_COLOR);
	unsigned long GetBearColor();
	void SetBearColor(unsigned long);
	unsigned long GetBlinkColor();
	void SetBlinkColor(unsigned long);
	short GetBlinkInterval();
	void SetBlinkInterval(short);
	short GetBorderThick();
	void SetBorderThick(short);
	unsigned long GetBullColor();
	void SetBullColor(unsigned long);
	unsigned long GetColorSelect();
	void SetColorSelect(unsigned long);
	short GetCols();
	void SetCols(short);
	unsigned long GetFixedSingleColor();
	void SetFixedSingleColor(unsigned long);
	OLE_COLOR GetForeColor();
	void SetForeColor(OLE_COLOR);
	CString GetLocalShareList();
	void SetLocalShareList(LPCTSTR);
	short GetPriceCellID();
	void SetPriceCellID(short);
	short GetPriceChgCellID();
	void SetPriceChgCellID(short);
	short GetRows();
	void SetRows(short);
	short GetSelectMethod();
	void SetSelectMethod(short);
	short GetShareEventMethod();
	void SetShareEventMethod(short);
	short GetSignCellID();
	void SetSignCellID(short);
	short GetStdPriceCellID();
	void SetStdPriceCellID(short);
	unsigned long GetSteadyColor();
	void SetSteadyColor(unsigned long);
	CString GetCellFormatInfo();
	void SetCellFormatInfo(LPCTSTR);
	CString GetRootPath();
	void SetRootPath(LPCTSTR);
	COleFont GetFont();
	void SetFont(LPDISPATCH);
	BOOL GetHide();
	void SetHide(BOOL);
	CString GetCellHSizes();
	void SetCellHSizes(LPCTSTR);
	CString GetCellVSizes();
	void SetCellVSizes(LPCTSTR);
	BOOL GetUseSkinColor();
	void SetUseSkinColor(BOOL);
	long GetSkinBackColor();
	void SetSkinBackColor(long);
	long GetSkinForeColor();
	void SetSkinForeColor(long);
	long GetSkinBullColor();
	void SetSkinBullColor(long);
	long GetSkinSteadyColor();
	void SetSkinSteadyColor(long);
	long GetSkinBearColor();
	void SetSkinBearColor(long);
	long GetSkinBlinkColor();
	void SetSkinBlinkColor(long);
	long GetSkinSelectColor();
	void SetSkinSelectColor(long);
	long GetSkinBorderColor();
	void SetSkinBorderColor(long);
	CString GetCustomFont();
	void SetCustomFont(LPCTSTR);
	CString GetComment();
	void SetComment(LPCTSTR);
	CString GetControlID();
	void SetControlID(LPCTSTR);
	BOOL GetNoUse();
	void SetNoUse(BOOL);
	long GetTabKeyStop();
	void SetTabKeyStop(long);
	short GetRealCellID();
	void SetRealCellID(short);
	BOOL GetEnableEventCellDataChanged();
	void SetEnableEventCellDataChanged(BOOL);
	short GetClickEventType();
	void SetClickEventType(short);
	CString GetCellFont();
	void SetCellFont(LPCTSTR);
	long GetRightShareEvent();
	void SetRightShareEvent(long);
	long GetUseDefaultFont();
	void SetUseDefaultFont(long);

// Operations
public:
	BOOL SetAllProperties2(long dwData);
	long GetBase(LPCTSTR szKey);
	void SetBaseDesk(long lKey);
	CString GetText(short nCol, short nRow);
	void SetText(short nCol, short nRow, LPCTSTR szData);
	void MakeSaveShortKey(LPCTSTR szFileName, long nFlag);
	void ResetTable();
	void OnGetCellIndex(short* nCol, short* nRow, short* pRectRight, short* nType, short Xpoint, short Ypoint);
	long OnReceiveFetchData(long pData);
	void SetCellBright(short nCol, short nRow);
	void SetCellForeClr(short col, long row, unsigned long clrCellFore);
	short OnDrds();
	short OffDrds();
	void SetRscMng(long lKey);
	void SetCellBackClr(short col, long row, unsigned long clrCellBack);
	void AboutBox();*/
