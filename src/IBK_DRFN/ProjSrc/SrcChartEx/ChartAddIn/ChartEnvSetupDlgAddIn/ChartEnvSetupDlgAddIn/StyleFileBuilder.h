// StyleFileBuilder.h: interface for the CStyleFileBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STYLEFILEBUILDER_H__3509997E_F0EC_4975_9AEE_FE84D0A1E23B__INCLUDED_)
#define AFX_STYLEFILEBUILDER_H__3509997E_F0EC_4975_9AEE_FE84D0A1E23B__INCLUDED_

#include <afxtempl.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStyleData;

class CFileBuilder  
{
protected:
	// file 을 open 하고 data를 읽는다.
	bool FileRead(const CString& strFilePathAndName, CString& strFileData) const;
	// file 을 생성하고 data를 쓴다.
	bool FileCreatWrite(const CString& strFilePathAndName, const CString& strWriteData);
	bool FileCreat(const CString& strFilePathAndName);
	bool FileWrite(const CString& strFilePathAndName, const CString& strWriteData);
	// file 찾기.
	bool FileFind(const CString& strFillePathAndName) const;
};


class CStyleFileBuilder : public CFileBuilder
{
public:
	CStyleFileBuilder();

	bool ReadStyleFile_Default(const char *szFilePath, int& nStyleCount, CList<CStyleData*, CStyleData*>& styleDataList);
	bool ReadStyleFile_User( const char *szFilePath, CList<CStyleData*, CStyleData*>& styleDataList);
	bool WriteUserStyleFile( const char *p_pszFilePath, const int nDefaultStyleCount, CList<CStyleData*, CStyleData*>& styleDataList);

	bool GetDefaultStyleData(const CString& strFilePath, const int nStyleNumber, CStyleData& styleData);
	bool GetUserStyleData(const CString& strFilePath, const int nStyleNumber, CStyleData& styleData);

private:
	CString GetDefaultFilePathAndName(const CString& strFilePath);
	CString GetUserFilePathAndName(const CString& strFilePath);

	void ReadStyleConfigFile(CString& strFileData, CList<CStyleData*, CStyleData*>& styleDataList, BOOL p_bWith4Default = FALSE);
	bool CheckWith4Default(const CString& strFileData) const;

	bool GetStyleData(const CString& strStyleData, CStyleData& styleData) const;
	bool GetStyleData_Price(const CString& strStyleData, CStyleData& styleData) const;
	bool GetStyleData_Trade(const CString& strStyleData, CStyleData& styleData) const;
	bool GetStyleData_MA(const CString& strStyleData, CStyleData& styleData) const; 
	bool GetStyleData_PriceMA(const CString& strStyleData, CStyleData& styleData) const;
	bool GetStyleData_TradeMA(const CString& strStyleData, CStyleData& styleData) const;
	bool GetStyleData_Graph(const CString& strStyleData, CStyleData& styleData) const;
	bool GetStyleData_Scale(const CString& strStyleData, CStyleData& styleData) const;
	bool GetStyleData_Etc(const CString& strStyleData, CStyleData& styleData) const;

	CString GetStyleDataForWrite(const int nDefaultStyleCount, CList<CStyleData*, CStyleData*>& styleDataList) const;
	CString GetStyleData(const CStyleData* pStyleData) const;
	CString GetStyleData_Price(const CStyleData* pStyleData) const;
	CString GetStyleData_PriceMA(const CStyleData* pStyleData) const;
	CString GetStyleData_Trade(const CStyleData* pStyleData) const;
	CString GetStyleData_TradeMA(const CStyleData* pStyleData) const;
	CString GetStyleData_Graph(const CStyleData* pStyleData) const;
	CString GetStyleData_Scale(const CStyleData* pStyleData) const;
	CString GetStyleData_Etc(const CStyleData* pStyleData) const;

private:
	CString	DEFFULT_FILE_NAME;
	CString	USER_FILE_NAME;

	CString STYLENUM;
	CString PRICE_UP;
	CString PRICE_DOUW;
	CString PRICE_EQUAL;
	CString UPFILL_PRICE;
	CString DOWNFILL_PRICE;
	CString TRADE_UP;
	CString TRADE_DOWN;
	CString TRADE_EQUAL;
	CString UPFILL_TRADE;
	CString DOWNFILL_TRADE;
	CString GRAPH_BG;
	CString GRAPH_BORDER;
	CString GRAPH_GRID;
	CString SCALE_BG;
	CString SCALE_BORDER;
	CString SCALE_TEXT;
	CString SCALE_GRID;
	CString GRID_WIDTH;
	CString GRID_HEIGHT;
	CString DIVIDE_WIDTH;
	CString DIVIDE_HEIGHT;
	CString ANALYSIS;
	CString NUMERICAL;
};
/*
class CStyleFileBuilder_OldVer
{
public:
	CStyleFileBuilder_OldVer();

	bool IsVersion(const CString& strStyleData) const;

	bool GetStyleData_PriceMA(const CString& strStyleData, CStyleData& styleData) const;
	bool GetStyleData_TradeMA(const CString& strStyleData, CStyleData& styleData) const;

private:
	CString PRICE_5;
	CString PRICE_10;
	CString PRICE_20;
	CString PRICE_60;
	CString PRICE_120;
	CString PRICE_240;
	CString TRADE_5;
	CString TRADE_20;
	CString TRADE_60;
	CString TRADE_120;
};
*/

#endif // !defined(AFX_STYLEFILEBUILDER_H__3509997E_F0EC_4975_9AEE_FE84D0A1E23B__INCLUDED_)
