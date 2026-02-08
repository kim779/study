// StyleFileBuilder.cpp: implementation of the CStyleFileBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "StyleFileBuilder.h"

#include <io.h>
#include "Conversion.h"
//#include "ChartEnvSetUpData.h"
#include "EnvironmentData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CFileBuilder

// file 을 open 하고 data를 읽는다.
bool CFileBuilder::FileRead(const CString& strFilePathAndName, CString& strFileData) const
{
	if(strFilePathAndName.IsEmpty())
		return false;

	FILE *stream = fopen(strFilePathAndName, "rb");
	if(stream == NULL)
		return false;
	
	fseek(stream, 0L, SEEK_END);// SEEK_SET);//tour2k
	//long nbytes1 = _filelength(stream->_file);
	long nbytes = ftell(stream);//tour2k 2022.7.27
	fseek(stream, 0L, SEEK_SET);

	char* buffer = new char[nbytes + 1];
	ZeroMemory(buffer, nbytes + 1);
	fread(buffer, sizeof(char), nbytes, stream);
	strFileData = buffer;

    fclose(stream);
	delete [] buffer;

	return (!strFileData.IsEmpty());
}

// file 을 생성하고 data를 쓴다. (이미 file이 존재하면 해당 file에 내용을 쓴다.)
bool CFileBuilder::FileCreatWrite(const CString& strFilePathAndName, const CString& strWriteData)
{
	// 해당 file 이 없으면 file 생성.
	if(!FileFind(strFilePathAndName)){
		if(!FileCreat(strFilePathAndName))
			return false;
	}

	// 해당 file에 data 쓰기.
	FileWrite(strFilePathAndName, strWriteData);

	return true;
}

// file 생성하기.
bool CFileBuilder::FileCreat(const CString& strFilePathAndName)
{
	FILE *stream = fopen(strFilePathAndName, "wb+");
	if(stream == NULL)
		return false;

    fclose(stream);
	return true;
}

// file 쓰기.
bool CFileBuilder::FileWrite(const CString& strFilePathAndName, const CString& strWriteData)
{
	// 해당 file에 data 쓰기.
	FILE *stream = fopen(strFilePathAndName, "wb+"); //"rb+"내용에 덥기.
	if(stream == NULL)
		return false;

	//fseek(stream, 0L, SEEK_END);
	fwrite(strWriteData, sizeof(char), strWriteData.GetLength(), stream);
	//fprintf(stream, "%s", writeData);
    fclose(stream);

	return true;
}

// file 찾기.
bool CFileBuilder::FileFind(const CString& strFillePathAndName) const
{
	if(strFillePathAndName.IsEmpty())
		return false;

	struct _finddata_t fileData;
	long hFile = _findfirst(strFillePathAndName, &fileData);
	if(hFile == -1L)
		return false;

	_findclose(hFile);
	return true;
}


///////////////////////////////////////////////////////////////////////////////
// class CStyleFileBuilder

CStyleFileBuilder::CStyleFileBuilder() :
	DEFFULT_FILE_NAME("DefaultStyle.cfg"), 
	USER_FILE_NAME("userStyle.cht"), 
	STYLENUM("nStyle="),
	PRICE_UP("priceup="),
	PRICE_DOUW("pricedown="),
	PRICE_EQUAL("priceequal="),
	UPFILL_PRICE("upfillprice="),
	DOWNFILL_PRICE("downfillprice="),
	TRADE_UP("tradeup="),
	TRADE_DOWN("tradedown="),
	TRADE_EQUAL("tradeequal="),
	UPFILL_TRADE("upfilltrade="),
	DOWNFILL_TRADE("downfilltrade="),
	GRAPH_BG("graphbg="),
	GRAPH_BORDER("graphborder="),
	GRAPH_GRID("graphgrid="),
	SCALE_BG("scalebg="),
	SCALE_BORDER("scaleborder="),
	SCALE_TEXT("scaletext="),
	SCALE_GRID("scalegrid="),
	GRID_WIDTH("gridwidth="),
	GRID_HEIGHT("gridheight="),
	DIVIDE_WIDTH("dividewidth="),
	DIVIDE_HEIGHT("divideheight="),
	ANALYSIS("analyze="),
	NUMERICAL("numerical=")
{
}

bool CStyleFileBuilder::ReadStyleFile_Default(const char *szFilePath, int& nStyleCount, CList<CStyleData*, CStyleData*>& styleDataList)
{
	CString strFileData;
	if(!CFileBuilder::FileRead(GetDefaultFilePathAndName( szFilePath), strFileData))
		return false;

	ReadStyleConfigFile( strFileData, styleDataList);
	nStyleCount = styleDataList.GetCount();

	return (nStyleCount > 0);
}

bool CStyleFileBuilder::ReadStyleFile_User( const char *szFilePath, CList<CStyleData*, CStyleData*>& styleDataList)
{
	CString strFileData;
	if( !CFileBuilder::FileRead(GetUserFilePathAndName( szFilePath), strFileData))
		return false;

	BOOL bWith4Default = CheckWith4Default( strFileData);
	ReadStyleConfigFile( strFileData, styleDataList, bWith4Default);

	return (styleDataList.GetCount() > 0);
}

bool CStyleFileBuilder::WriteUserStyleFile( const char *p_szFilePath, const int nDefaultStyleCount, CList<CStyleData*, CStyleData*>& styleDataList)
{
	if( !p_szFilePath) return false;
	if( !*p_szFilePath) return false;

	return CFileBuilder::FileCreatWrite( GetUserFilePathAndName( p_szFilePath), GetStyleDataForWrite( nDefaultStyleCount, styleDataList));
}

// ----------------------------------------------------------------------------
bool CStyleFileBuilder::GetDefaultStyleData(const CString& strFilePath, const int nStyleNumber, CStyleData& styleData)
{
	if(strFilePath.IsEmpty() || nStyleNumber < 0)
		return false;

	CString strFileData;
	if(!CFileBuilder::FileRead(GetDefaultFilePathAndName(strFilePath), strFileData))
		return false;

	CString strStyleData = CDataConversion::GetMidToTrimDataStartIndex(strFileData, "[", nStyleNumber, true, true);
	return GetStyleData(strStyleData, styleData);
}

bool CStyleFileBuilder::GetUserStyleData(const CString& strFilePath, const int nStyleNumber, CStyleData& styleData)
{
	if(strFilePath.IsEmpty() || nStyleNumber < 0)
		return false;

	CString strFileData;
	if(!CFileBuilder::FileRead(GetUserFilePathAndName(strFilePath), strFileData))
		return false;

	CString strStyleData = CDataConversion::GetMidToTrimDataStartIndex(strFileData, "[", nStyleNumber, true, true);
	return GetStyleData(strStyleData, styleData);
}


// private ====================================================================
CString CStyleFileBuilder::GetDefaultFilePathAndName(const CString& strFilePath)
{
	return (strFilePath + DEFFULT_FILE_NAME);
}

CString CStyleFileBuilder::GetUserFilePathAndName(const CString& strFilePath)
{
	return (strFilePath + USER_FILE_NAME);
}
//sy end

// ----------------------------------------------------------------------------
void CStyleFileBuilder::ReadStyleConfigFile(CString& strFileData, CList<CStyleData*, CStyleData*>& styleDataList, BOOL p_bWith4Default)
{
	int nCount = 0;
	CDataConversion::GetStringData(strFileData, "[", true);
	while(!strFileData.IsEmpty())
	{
		CString strStyleData = CDataConversion::GetStringData(strFileData, "[", true);
		if(strStyleData.IsEmpty() && !strFileData.IsEmpty())
		{
			strStyleData = strFileData;
			strFileData.Empty();
		}

		// (2008/4/7 - Seung-Won, Bae) Skip 4 Default.
		nCount++;
		if( p_bWith4Default && nCount <= 4) continue;

		CStyleData* pStyleData = new CStyleData();
		if(GetStyleData(strStyleData, *pStyleData))
		{
			styleDataList.AddTail(pStyleData);
		}
		else
		{
			delete pStyleData;
		}
	}
}

bool CStyleFileBuilder::CheckWith4Default( const CString& strFileData) const
{
	//nStyle=6  -> default         nStyle=6,1  -> user
	CString strData = CDataConversion::GetMidToTrimDataIndex(strFileData, STYLENUM, "\r\n", 0, true);
	if( strData.IsEmpty()) return FALSE;
	return ( 0 < strData.Find( ","));
}

// ----------------------------------------------------------------------------
bool CStyleFileBuilder::GetStyleData(const CString& strStyleData, CStyleData& styleData) const
{
	if(strStyleData.IsEmpty())
		return false;

	CString strItemData = CDataConversion::GetLeftToTrimDataIndex(strStyleData, "]\r\n");
	strItemData.TrimLeft("[");
	if(strItemData.IsEmpty())
		return false;

	styleData.SetStyleName(strItemData);

	bool bResult = false;
	if(GetStyleData_Price(strStyleData, styleData))
		bResult = true;
	if(GetStyleData_Trade(strStyleData, styleData))
		bResult = true;
	if(GetStyleData_MA(strStyleData, styleData))
		bResult = true;
	if(GetStyleData_Graph(strStyleData, styleData))
		bResult = true;
	if(GetStyleData_Scale(strStyleData, styleData))
		bResult = true;
	if(GetStyleData_Etc(strStyleData, styleData))
		bResult = true;

	return bResult;
}

bool CStyleFileBuilder::GetStyleData_Price(const CString& strStyleData, CStyleData& styleData) const
{
	bool bResult = false;
	CString strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, PRICE_UP, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetPriceUpColor(CDataConversion::Str2RGB(strItemData));
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, PRICE_DOUW, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetPriceDownColor(CDataConversion::Str2RGB(strItemData));
		styleData.SetPriceEqualColor(styleData.GetPriceDownColor());
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, PRICE_EQUAL, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetPriceEqualColor(CDataConversion::Str2RGB(strItemData));
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, UPFILL_PRICE, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetPriceUpFill(atoi(strItemData) == 1);
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, DOWNFILL_PRICE, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetPriceDownFill(atoi(strItemData) == 1);
		bResult = true;
	}

	return bResult;
}

bool CStyleFileBuilder::GetStyleData_Trade(const CString& strStyleData, CStyleData& styleData) const
{
	bool bResult = false;
	CString strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, TRADE_UP, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetTradeUpColor(CDataConversion::Str2RGB(strItemData));
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, TRADE_DOWN, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetTradeDownColor(CDataConversion::Str2RGB(strItemData));
		styleData.SetTradeEqualColor(styleData.GetTradeDownColor());
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, TRADE_EQUAL, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetTradeEqualColor(CDataConversion::Str2RGB(strItemData));
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, UPFILL_TRADE, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetTradeUpFill(atoi(strItemData) == 1);
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, DOWNFILL_TRADE, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetTradeDownFill(atoi(strItemData) == 1);
		bResult = true;
	}

	return bResult;
}

bool CStyleFileBuilder::GetStyleData_MA(const CString& strStyleData, CStyleData& styleData) const
{
	bool bResult = false;
	if(GetStyleData_PriceMA(strStyleData, styleData))
		bResult = true;
	if(GetStyleData_TradeMA(strStyleData, styleData))
		bResult = true;
	return bResult;
}

bool CStyleFileBuilder::GetStyleData_PriceMA(const CString& strStyleData, CStyleData& styleData) const
{
	bool bResult = false;

	int nIndex = 1;
	CString strKey;
	strKey.Format( "price%d=", nIndex);
	CString strItemData = CDataConversion::GetMidToTrimDataIndex( strStyleData, strKey, "\r\n");
	while( !strItemData.IsEmpty())
	{
		styleData.SetPriceMAColor( nIndex - 1, CDataConversion::Str2RGB(strItemData));
		bResult = true;

		nIndex++;
		strKey.Format( "price%d=", nIndex);
		strItemData = CDataConversion::GetMidToTrimDataIndex( strStyleData, strKey, "\r\n");
	}

	return bResult;
}

bool CStyleFileBuilder::GetStyleData_TradeMA(const CString& strStyleData, CStyleData& styleData) const
{
	bool bResult = false;

	int nIndex = 1;
	CString strKey;
	strKey.Format( "trade%d=", nIndex);
	CString strItemData = CDataConversion::GetMidToTrimDataIndex( strStyleData, strKey, "\r\n");
	while( !strItemData.IsEmpty())
	{
		styleData.SetTradeMAColor( nIndex - 1, CDataConversion::Str2RGB(strItemData));
		bResult = true;

		nIndex++;
		strKey.Format( "trade%d=", nIndex);
		strItemData = CDataConversion::GetMidToTrimDataIndex( strStyleData, strKey, "\r\n");
	}

	return bResult;
}

bool CStyleFileBuilder::GetStyleData_Graph(const CString& strStyleData, CStyleData& styleData) const
{
	bool bResult = false;
	CString strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, GRAPH_BG, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetGraphBGColor(CDataConversion::Str2RGB(strItemData));
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, GRAPH_BORDER, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetGraphBorderColor(CDataConversion::Str2RGB(strItemData));
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, GRAPH_GRID, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetGraphGridColor(CDataConversion::Str2RGB(strItemData));
		bResult = true;
	}

	return bResult;
}

bool CStyleFileBuilder::GetStyleData_Scale(const CString& strStyleData, CStyleData& styleData) const
{
	bool bResult = false;
	CString strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, SCALE_BG, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetScaleBGColor(CDataConversion::Str2RGB(strItemData));
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, SCALE_BORDER, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetScaleBorderColor(CDataConversion::Str2RGB(strItemData));
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, SCALE_TEXT, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetScaleTextColor(CDataConversion::Str2RGB(strItemData));
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, SCALE_GRID, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetScaleGridColor(CDataConversion::Str2RGB(strItemData));
		bResult = true;
	}

	return bResult;
}

bool CStyleFileBuilder::GetStyleData_Etc(const CString& strStyleData, CStyleData& styleData) const
{
	bool bResult = false;
	CString strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, GRID_WIDTH, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetGridWidth(atoi(strItemData));
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, GRID_HEIGHT, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetGridHeight(atoi(strItemData));
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, DIVIDE_WIDTH, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetDivideWidth(atoi(strItemData));
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, DIVIDE_HEIGHT, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetDivideHeight(atoi(strItemData));
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, ANALYSIS, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetAnalyzeColor(CDataConversion::Str2RGB(strItemData));
		bResult = true;
	}
	
	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, NUMERICAL, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetNumericalColor(CDataConversion::Str2RGB(strItemData));
		bResult = true;
	}

	return bResult;
}

//-----------------------------------------------------------------------------
CString CStyleFileBuilder::GetStyleDataForWrite(const int nDefaultStyleCount, CList<CStyleData*, CStyleData*>& styleDataList) const
{
	int nCount = 0;
	CString strStyleDataList;
	POSITION pos = styleDataList.GetHeadPosition();
	while(pos != NULL)
	{
		CStyleData* pStyleData = styleDataList.GetNext(pos);

// (2009/9/21 - Seung-Won, Bae) Save with Default.
//		nCount++;
//		if( nCount <= nDefaultStyleCount) continue;

		CString strStyleData = GetStyleData(pStyleData);
		if(strStyleData.IsEmpty())
			continue;

		strStyleDataList += strStyleData;
	}

	return strStyleDataList;
}

CString CStyleFileBuilder::GetStyleData(const CStyleData* pStyleData) const
{
	if(pStyleData == NULL)
		return "";

	CString strStyleData;
	strStyleData.Format("[%s]\r\n%s%s%s%s%s%s%s\r\n", 
		pStyleData->GetStyleName(), 
		GetStyleData_Price(pStyleData), GetStyleData_PriceMA(pStyleData),
		GetStyleData_Trade(pStyleData), GetStyleData_TradeMA(pStyleData), 
		GetStyleData_Graph(pStyleData), GetStyleData_Scale(pStyleData), GetStyleData_Etc(pStyleData));

	return strStyleData;
}

CString CStyleFileBuilder::GetStyleData_Price(const CStyleData* pStyleData) const
{
	CString strStyleData;
	strStyleData.Format("%s%s\r\n%s%s\r\n%s%s\r\n%s%d\r\n%s%d\r\n", 
		PRICE_UP, CDataConversion::RGB2Str(pStyleData->GetPriceUpColor()), 
		PRICE_DOUW, CDataConversion::RGB2Str(pStyleData->GetPriceDownColor()), 
		PRICE_EQUAL, CDataConversion::RGB2Str(pStyleData->GetPriceEqualColor()), 
		UPFILL_PRICE, pStyleData->GetPriceUpFill(), 
		DOWNFILL_PRICE, pStyleData->GetPriceDownFill());

	return strStyleData;
}

CString CStyleFileBuilder::GetStyleData_PriceMA(const CStyleData* pStyleData) const
{
	CString strStyleData;
	CString strColor;
	
	int nMACount = pStyleData->GetPriceMACount();
	for( int i = 0; i < nMACount; i++)
	{
		strColor.Format( "price%d=%s\r\n", i + 1, CDataConversion::RGB2Str( pStyleData->GetPriceMAColor( i)));
		strStyleData += strColor;
	}

	return strStyleData;
}

CString CStyleFileBuilder::GetStyleData_Trade(const CStyleData* pStyleData) const
{
	CString strStyleData;
	strStyleData.Format("%s%s\r\n%s%s\r\n%s%s\r\n%s%d\r\n%s%d\r\n",
		TRADE_UP, CDataConversion::RGB2Str(pStyleData->GetTradeUpColor()), 
		TRADE_DOWN, CDataConversion::RGB2Str(pStyleData->GetTradeDownColor()), 
		TRADE_EQUAL, CDataConversion::RGB2Str(pStyleData->GetTradeEqualColor()), 
		UPFILL_TRADE, pStyleData->GetTradeUpFill(), 
		DOWNFILL_TRADE, pStyleData->GetTradeDownFill());

	return strStyleData;
}

CString CStyleFileBuilder::GetStyleData_TradeMA(const CStyleData* pStyleData) const
{
	CString strStyleData;
	CString strColor;
	
	int nMACount = pStyleData->GetTradeMACount();
	for( int i = 0; i < nMACount; i++)
	{
		strColor.Format( "trade%d=%s\r\n", i + 1, CDataConversion::RGB2Str( pStyleData->GetTradeMAColor( i)));
		strStyleData += strColor;
	}

	return strStyleData;
}

CString CStyleFileBuilder::GetStyleData_Graph(const CStyleData* pStyleData) const
{
	CString strStyleData;
	strStyleData.Format("%s%s\r\n%s%s\r\n%s%s\r\n", 
		GRAPH_BG, CDataConversion::RGB2Str(pStyleData->GetGraphBGColor()), 
		GRAPH_BORDER, CDataConversion::RGB2Str(pStyleData->GetGraphBorderColor()), 
		GRAPH_GRID, CDataConversion::RGB2Str(pStyleData->GetGraphGridColor()));

	return strStyleData;
}

CString CStyleFileBuilder::GetStyleData_Scale(const CStyleData* pStyleData) const
{
	CString strStyleData;
	strStyleData.Format("%s%s\r\n%s%s\r\n%s%s\r\n%s%s\r\n", 
		SCALE_BG, CDataConversion::RGB2Str(pStyleData->GetScaleBGColor()), 
		SCALE_BORDER, CDataConversion::RGB2Str(pStyleData->GetScaleBorderColor()), 
		SCALE_TEXT, CDataConversion::RGB2Str(pStyleData->GetScaleTextColor()), 
		SCALE_GRID, CDataConversion::RGB2Str(pStyleData->GetScaleGridColor()));

	return strStyleData;
}

CString CStyleFileBuilder::GetStyleData_Etc(const CStyleData* pStyleData) const
{
	CString strStyleData;
	strStyleData.Format("%s%d\r\n%s%d\r\n%s%d\r\n%s%d\r\n%s%s\r\n%s%s\r\n", 
		GRID_WIDTH, pStyleData->GetGridWidth(), 
		GRID_HEIGHT, pStyleData->GetGridHeight(),
		DIVIDE_WIDTH, pStyleData->GetDivideWidth(), 
		DIVIDE_HEIGHT, pStyleData->GetDivideHeight(), 
		ANALYSIS, CDataConversion::RGB2Str(pStyleData->GetAnalyzeColor()), 
		NUMERICAL, CDataConversion::RGB2Str(pStyleData->GetNumericalColor()));

	return strStyleData;
}

/*

///////////////////////////////////////////////////////////////////////////////
// class CStyleFileBuilder_OldVer

CStyleFileBuilder_OldVer::CStyleFileBuilder_OldVer() :
	PRICE_5("price5="),
	PRICE_10("price10="),
	PRICE_20("price20="),
	PRICE_60("price60="),
	PRICE_120("price120="),
	PRICE_240("price240="),
	TRADE_5("trade5="),
	TRADE_20("trade20="),
	TRADE_60("trade60="),
	TRADE_120("trade120=")
{
}

// public =====================================================================
bool CStyleFileBuilder_OldVer::IsVersion(const CString& strStyleData) const
{
	if(strStyleData.IsEmpty())
		return false;

	return (strStyleData.Find(PRICE_240) >= 0);
}

// ----------------------------------------------------------------------------
bool CStyleFileBuilder_OldVer::GetStyleData_PriceMA(const CString& strStyleData, CStyleData& styleData) const
{
	bool bResult = false;
	CString strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, PRICE_5, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetPriceColor1(CDataConversion::Str2RGB(strItemData));
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, PRICE_10, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetPriceColor2(CDataConversion::Str2RGB(strItemData));
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, PRICE_20, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetPriceColor3(CDataConversion::Str2RGB(strItemData));
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, PRICE_60, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetPriceColor4(CDataConversion::Str2RGB(strItemData));
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, PRICE_120, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetPriceColor5(CDataConversion::Str2RGB(strItemData));
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, PRICE_240, "\r\n");
	if(!strItemData.IsEmpty())
	{
		COLORREF color = CDataConversion::Str2RGB(strItemData);
		styleData.SetPriceColor6(color);
		styleData.SetPriceColor7(color);
		styleData.SetPriceColor8(color);
		styleData.SetPriceColor9(color);
		styleData.SetPriceColor10(color);
		bResult = true;
	}

	return bResult;
}

bool CStyleFileBuilder_OldVer::GetStyleData_TradeMA(const CString& strStyleData, CStyleData& styleData) const
{
	bool bResult = false;
	CString strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, TRADE_5, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetTradeColor1(CDataConversion::Str2RGB(strItemData));
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, TRADE_20, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetTradeColor2(CDataConversion::Str2RGB(strItemData));
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, TRADE_60, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetTradeColor3(CDataConversion::Str2RGB(strItemData));
		bResult = true;
	}

	strItemData = CDataConversion::GetMidToTrimDataIndex(strStyleData, TRADE_120, "\r\n");
	if(!strItemData.IsEmpty())
	{
		styleData.SetTradeColor4(CDataConversion::Str2RGB(strItemData));
		bResult = true;
	}

	return bResult;
}

*/