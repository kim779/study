// ElementSaverBuilder.cpp: implementation of the CElementSaverBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "symbol.h"
#include "ElementSaverBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "Conversion.h"

#include "DrawingSymbol.h"
#include "DrawingSymbolRow.h"
#include "DrawingSYmbolFactory.h"

#include "OrderAndElement.h"
#include "OrderSet.h"
#include "SymbolData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CString CElementSaverBuilder::GetSavedSymbolsData(const COrder &order, CList<CDrawingSymbol*, CDrawingSymbol*>* pSymbolList)
{
	CString strValue = "";
	POSITION pos = pSymbolList->GetHeadPosition();
	while(pos) {
		CDrawingSymbol* pSymbol = pSymbolList->GetNext(pos);

		CString strIndex = GetIndexString(pSymbol, order);
		if(strIndex.IsEmpty())
			return "";

		strValue += strIndex;
		strValue += CDataConversion::IntToString(pSymbol->GetSymbolType()) + ";"; 
		strValue += CDataConversion::IntToString(pSymbol->GetSizeType()) + ";"; 
		strValue += CDataConversion::DoubleToString((pSymbol->GetStartPoint()).GetX()) + ";"; 
		strValue += CDataConversion::DoubleToString((pSymbol->GetStartPoint()).GetY()) + ";"; 
		strValue += CDataConversion::DoubleToString((pSymbol->GetEndPoint()).GetX()) + ";"; 
		strValue += CDataConversion::DoubleToString((pSymbol->GetEndPoint()).GetY()) + ";"; 
		strValue += CDataConversion::IntToString((pSymbol->GetSymbolSize()).cx) + ";"; 
		strValue += CDataConversion::IntToString((pSymbol->GetSymbolSize()).cy) + ";"; 		
		strValue += pSymbol->GetText() + ";"; 		
		strValue += CDataConversion::RGB2Str(pSymbol->GetColor()) + ";"; 		
		strValue += CDataConversion::IntToString(pSymbol->GetWeight()) + ";";
		//by jwp 2006/12/11
		strValue += CDataConversion::IntToString(pSymbol->GetFillState()) + ";"; 		
		strValue += MakeStringFromLOGFONT(pSymbol->GetLogFont()) + ";";	
		//sy 2005.04.06
		strValue += CDataConversion::RGB2Str(pSymbol->GetTextColor()) + ";"; 
		strValue += CDataConversion::Bool2Str(pSymbol->DidDrawFromTop()) + ";";
		// 20081007 JS.Kim	사용자시간대 선택.
		strValue += CDataConversion::IntToString(pSymbol->GetStartPoint().GetTimeDiff()) + ";\r\n";
		strValue += "SymbolEnd;\r\n";
	}
	return strValue;
}

CString CElementSaverBuilder::GetSavedSymbolData_ForUndoRedo(const COrder &order, CDrawingSymbol* pSymbol)
{
	CString strIndex = GetIndexString(pSymbol, order);
	if(strIndex.IsEmpty())
		return "";

	CString strSymbolData;
	strSymbolData += strIndex;
	strSymbolData += CDataConversion::IntToString(pSymbol->GetSymbolType()) + ";"; 
	strSymbolData += CDataConversion::IntToString(pSymbol->GetSizeType()) + ";"; 
	strSymbolData += CDataConversion::DoubleToString((pSymbol->GetStartPoint()).GetX()) + ";"; 
	strSymbolData += CDataConversion::DoubleToString((pSymbol->GetStartPoint()).GetY()) + ";"; 
	strSymbolData += CDataConversion::DoubleToString((pSymbol->GetEndPoint()).GetX()) + ";"; 
	strSymbolData += CDataConversion::DoubleToString((pSymbol->GetEndPoint()).GetY()) + ";"; 
	strSymbolData += CDataConversion::IntToString((pSymbol->GetSymbolSize()).cx) + ";"; 
	strSymbolData += CDataConversion::IntToString((pSymbol->GetSymbolSize()).cy) + ";"; 		
	strSymbolData += pSymbol->GetText() + ";"; 		
	strSymbolData += CDataConversion::RGB2Str(pSymbol->GetColor()) + ";"; 		
	strSymbolData += CDataConversion::IntToString(pSymbol->GetWeight()) + ";";
	//by jwp 2006/12/11
	strSymbolData += CDataConversion::IntToString(pSymbol->GetFillState()) + ";"; 		
	strSymbolData += MakeStringFromLOGFONT(pSymbol->GetLogFont()) + ";";	
	//sy 2005.04.06
	strSymbolData += CDataConversion::RGB2Str(pSymbol->GetTextColor()) + ";"; 
	strSymbolData += CDataConversion::Bool2Str(pSymbol->DidDrawFromTop()) + ";";
	// 20081007 JS.Kim	사용자시간대 선택.
	strSymbolData += CDataConversion::IntToString(pSymbol->GetStartPoint().GetTimeDiff()) + ";\r\n";
	strSymbolData += "SymbolEnd;\r\n";

	return strSymbolData;
}

CString CElementSaverBuilder::MakeStringFromLOGFONT(const LOGFONT &logfont)
{
	CString strValue = "";
	strValue += CDataConversion::IntToString(logfont.lfWidth) + ",";
	strValue += CDataConversion::IntToString(logfont.lfHeight) + ",";
	strValue += CDataConversion::IntToString(logfont.lfWeight) + ",";
	CString text = logfont.lfFaceName;
	strValue += text + ",";
	return strValue;
}

LOGFONT CElementSaverBuilder::MakeLOGFONTFromString(const CString &data)
{
	CString logdata = data;

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
	lf.lfWidth = atoi(CDataConversion::GetStringData(logdata, ","));
	lf.lfHeight = atoi(CDataConversion::GetStringData(logdata, ","));
	lf.lfWeight = atoi(CDataConversion::GetStringData(logdata, ","));
	strcpy(lf.lfFaceName, CDataConversion::GetStringData(logdata, ","));        // request a face name "Arial"

	lf.lfItalic = FALSE;
	lf.lfUnderline = FALSE;
	lf.lfStrikeOut = FALSE;
	lf.lfPitchAndFamily = FIXED_PITCH;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfCharSet = HANGUL_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	return lf;
}

CString CElementSaverBuilder::GetIndexString(const CDrawingSymbol* pSymbol, const COrder &order) const
{
	int nIndex = order.GetIndex(pSymbol);
	if(nIndex < 0)
		return "";

	return (CDataConversion::IntToString(nIndex)+ "IndexEnd;\r\n");
}

// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
//void CElementSaverBuilder::BuildSymbolsFromString(const CString &data, CTree& tree, COrder &order)
void CElementSaverBuilder::BuildSymbolsFromString(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType, const CString &data, CTree& tree, COrder &order)
// 2011.02.07 by SYS <<
{
	COrderSet orderset;

	CString alldata = data;
	int nCount = CDataConversion::GetStringDataCount(alldata, "BlockColumnEnd;\r\n");

	for(int nColumn = 0; nColumn < nCount ; nColumn++)
	{
		// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
		//BuildSymbolsFromString_Column(CDataConversion::GetStringData(alldata, "BlockColumnEnd;\r\n"),
		//	nColumn, tree, orderset);
		BuildSymbolsFromString_Column(eScaleType, CDataConversion::GetStringData(alldata, "BlockColumnEnd;\r\n"),
			nColumn, tree, orderset);
		// 2011.02.07 by SYS <<
	}

	AddElement(orderset, order);	
}

long CElementSaverBuilder::BuildSymbolsFromString_ForUndoRedo(CSymbolData& symbolData, const CBlockIndex& blockIndex, CString& strSymbolData, 
		CTree& tree, COrder& order)
{
	if(strSymbolData.IsEmpty())		return 0;

	COrderSet orderset;
	
	// Get Index
	int nIndex = atoi(CDataConversion::GetStringData(strSymbolData, "IndexEnd;\r\n"));

	// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//CDrawingSymbol* pSymbol = MakeElement(strSymbolData);
	CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType = symbolData.GetXScaleDrawType();
	CDrawingSymbol* pSymbol = MakeElement(eScaleType, strSymbolData);
	// 2011.02.07 by SYS <<

	// Tree에 Element Add
	orderset.Add(nIndex, pSymbol);
	tree.Add(blockIndex.GetColumn(), blockIndex.GetRow(), pSymbol);

	// Order에 Element Add
	AddElement_ForUndoRedo(orderset, order);

	symbolData.RegisterElementToOrderManager((long)pSymbol);

	return (long)pSymbol;
}

// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
//void CElementSaverBuilder::BuildSymbolsFromString_Column(const CString &data, const int nColumn, CTree& tree, COrderSet& orderset)
void CElementSaverBuilder::BuildSymbolsFromString_Column(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType, const CString &data, const int nColumn, CTree& tree, COrderSet& orderset)
// 2011.02.07 by SYS <<
{
	CString columndata = data;
	int nCount = CDataConversion::GetStringDataCount(columndata, "BlockEnd;\r\n");
	for(int nRow = 0; nRow < nCount ; nRow++)
	{
		// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
		//BuildSymbolsFromString_Row(CDataConversion::GetStringData(columndata, "BlockEnd;\r\n"),
		//	nColumn, nRow, tree, orderset);
		BuildSymbolsFromString_Row(eScaleType, CDataConversion::GetStringData(columndata, "BlockEnd;\r\n"),
			nColumn, nRow, tree, orderset);
		// 2011.02.07 by SYS <<
	}
}

// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
//void CElementSaverBuilder::BuildSymbolsFromString_Row(const CString &data, const int nColumn, const int nRow, CTree& tree, COrderSet& orderset)
void CElementSaverBuilder::BuildSymbolsFromString_Row(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType, const CString &data, const int nColumn, const int nRow, CTree& tree, COrderSet& orderset)
// 2011.02.07 by SYS <<
{
	CString rowdata = data;
	int nCount = CDataConversion::GetStringDataCount(rowdata, "SymbolEnd;\r\n");
	for(int nIndex = 0 ; nIndex< nCount; nIndex++) 
	{
		// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
		//BuildSymbolsFromString_Symbol(CDataConversion::GetStringData(rowdata, "SymbolEnd;\r\n"),
		//	nColumn, nRow, tree, orderset);
		BuildSymbolsFromString_Symbol(eScaleType, CDataConversion::GetStringData(rowdata, "SymbolEnd;\r\n"),
			nColumn, nRow, tree, orderset);
		// 2011.02.07 by SYS <<
	}
}

// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
//void CElementSaverBuilder::BuildSymbolsFromString_Symbol(const CString &data, const int nColumn, const int nRow, CTree& tree, COrderSet& orderset)
void CElementSaverBuilder::BuildSymbolsFromString_Symbol(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType, const CString &data, const int nColumn, const int nRow, CTree& tree, COrderSet& orderset)
// 2011.02.07 by SYS <<
{
	CString symboldata = data;
	int nIndex = atoi(CDataConversion::GetStringData(symboldata, "IndexEnd;\r\n"));

	// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//CDrawingSymbol* pSymbol = MakeElement(symboldata);
	CDrawingSymbol* pSymbol = MakeElement(eScaleType, symboldata);
	// 2011.02.07 by SYS <<

	orderset.Add(nIndex, pSymbol);
	tree.Add(nColumn, nRow, pSymbol);
}

// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
//CDrawingSymbol* CElementSaverBuilder::MakeElement(const CString& strSymboldata)
CDrawingSymbol* CElementSaverBuilder::MakeElement(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType, const CString& strSymboldata)
// 2011.02.07 by SYS <<
{
	CString data = strSymboldata;
	int symboltype= atoi(CDataConversion::GetStringData(data, ";"));
	int sizetype= atoi(CDataConversion::GetStringData(data, ";"));
	
	// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//double start_x = atof(CDataConversion::GetStringData(data, ";"));
	CString strX;
	__int64 lMakeTime = 0;
	strX			= CDataConversion::GetStringData(data, ";");
	lMakeTime		= MakeDateTime(eScaleType, strX);
	double start_x	= (double)lMakeTime;
	// 2011.02.07 by SYS <<
	
	double start_y = atof(CDataConversion::GetStringData(data, ";"));

	
	// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//double end_x = atof(CDataConversion::GetStringData(data, ";"));
	strX			= CDataConversion::GetStringData(data, ";");
	lMakeTime		= MakeDateTime(eScaleType, strX);
	double end_x	= (double)lMakeTime;
	// 2011.02.07 by SYS <<


	double end_y = atof(CDataConversion::GetStringData(data, ";"));
	int size_cx = atoi(CDataConversion::GetStringData(data, ";"));
	int size_cy = atoi(CDataConversion::GetStringData(data, ";"));
	CString strText = CDataConversion::GetStringData(data, ";");
	COLORREF color = CDataConversion::Str2RGB(CDataConversion::GetStringData(data, ";"));
	int weight = atoi(CDataConversion::GetStringData(data, ";"));
	bool bFill = atoi(CDataConversion::GetStringData(data, ";")) == 1;
	LOGFONT logfont = MakeLOGFONTFromString(CDataConversion::GetStringData(data, ";"));
	//sy 2005.04.06
	COLORREF textColor = CDataConversion::Str2RGB(CDataConversion::GetStringData(data, ";"));
	bool bDrawFromTop = atoi(CDataConversion::GetStringData(data, ";")) ? true : false;
	// 20081007 JS.Kim	사용자시간대 선택. 
	int nTimeDiff = atoi(CDataConversion::GetStringData(data, ";"));

	if(end_x == -1 && end_y == -1)
	{
		//ont_point객체
		// 20081007 JS.Kim	사용자시간대 선택.
		CDrawingSymbol* pSymbol = CDrawingSymbolFactory::Make(CSymbolBaseData::TOOLTYPE(symboltype), 
			CSymbolBaseData::SIZETYPE(sizetype), CCoordinate(start_x, start_y, nTimeDiff), 
			CSize(size_cx, size_cy), color, logfont, textColor);
		pSymbol->SetText(strText);
		return pSymbol;
	}
	else //two_point객체
	{
		// 20081007 JS.Kim	사용자시간대 선택.
		return CDrawingSymbolFactory::Make(CSymbolBaseData::TOOLTYPE(symboltype),CCoordinate(start_x, start_y, nTimeDiff),
										   CCoordinate(end_x, end_y, nTimeDiff), color,PS_SOLID,weight,bFill, logfont, bDrawFromTop);
	}
}

// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
__int64 CElementSaverBuilder::MakeDateTime(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType, CString strX)
{
	__int64 lMakeTime = 0;

	CString strMakeTime = strX;

	// - 날짜시간 패킷이 14Byte 형식이 아닌 경우 비교 처리
	// 1. YYYYMMDD 형식 : 시분초를 0값으로 초기화
	// 2. YYYYMM   형식 : 일시분초를 1일 0시 0분 0초 로 초기화
	// 3. MMDDHHMM 형식 : 로컬PC 년도 및 00 초로 초기화
	// 	  단, 분석툴에 저장된 날짜정보가 로컬PC의 년월일보다 이후 날짜면... 
	// 	  로컬PC의 년정보를 -1 해준다.
	// 4. DDHHMMSS 형식 : 로컬PC 년/월로 초기화
	// 	  단, 분석툴에 저장된 날짜정보가 로컬PC의 년월일보다 이후 날짜면... 
	// 	  로컬PC의 월정보을 -1 해준다.
	if (strX != "-1" && strX.GetLength() != 14)
	{
		// 날짜시간 정보가 14Byte(년월일시분초) 형태가 아니라면 기존 분석툴 파일 형식이라고 본다.
		// 기존 저장파일과의 호환을 위해 수정 필요.

		CTime timeCur			= CTime::GetCurrentTime();
		CString strCurYear		= timeCur.Format("%Y");
		CString strCurMon		= timeCur.Format("%m");
		CString strCurYearMon	= timeCur.Format("%Y%m");
		CString strCurDate		= timeCur.Format("%Y%m%d");
		CString strCurFull		= timeCur.Format("%Y%m%d%H%M%S");

		//
		CString strFormat = _T("YYYYMMDD");	// 디폴트포맷
		
		switch(eScaleType)
		{
		case CScaleBaseData::HORZ_DAILY:		// 일
		case CScaleBaseData::HORZ_WEEKLEY:		// 주 
			{
				// 일간 또는 주간인 경우 : 0시 0분 0초를 덧붙인다.
				strMakeTime = strX + _T("000000");
			}
			break;
		case CScaleBaseData::HORZ_MONTHLY:		// 월 
			{
				// 월간인 경우 : 1일 0시 0분 0초를 덧붙인다.
				strMakeTime = strX + _T("01000000");
			}
			break;
		case CScaleBaseData::HORZ_TIME_ONE:		// 분
		case CScaleBaseData::HORZ_TIME_TWO:
			{
				// 분간인 경우 : 로컬PC년도 + 월일시분 + 0초

				CString strMon;
				CString strDay;
				
				if (strX.GetLength() == 7)
				{
					// 월Data가 %02d 형태가 아니고 한자리만 저장된 경우 (예, 5월이면 05가 아닌 5가 저장된 경우임)
					strMon.Format("%02d", atoi(strX.Left(1)));
					strDay.Format("%02d", atoi(strX.Mid(1,2)));
				}
				else
				{
					strMon.Format("%02d", atoi(strX.Left(2)));
					strDay.Format("%02d", atoi(strX.Mid(2,2)));
				}
				
				CString strTemp1	= strCurYear + strMon + strDay;
				CString strYear		= strCurYear;
				
				// 분석툴에 저장된 월일정보가 로컬PC의 년월일보다 이후 날짜면... 로컬PC년도를 -1 해준다.
				if (_atoi64(strTemp1) > _atoi64(strCurDate))
				{
					strYear.Format("%04d", atoi(strCurYear) -1);
				}
				
				strMakeTime = strYear + strMon + strDay + strX.Right(4) + _T("00");
			}
			break;
		case CScaleBaseData::HORZ_TICK:			// 틱 
		case CScaleBaseData::HORZ_TIME_SECOND:
			{
				// 틱/초인 경우 : 로컬PC년월 + 일시분초
				
				CString strDay;
				
				if (strX.GetLength() == 7)
					strDay.Format("%02d", atoi(strX.Left(1)));	// 일data가 %02d 형태가 아니고 한자리만 저장된 경우 (예, 5일이면 05가 아닌 5가 저장된 경우임)
				else
					strDay.Format("%02d", atoi(strX.Left(2)));
				
				CString strTemp1	= strCurYearMon + strDay;
				CString strYearMon	= strCurYearMon;
				
				// 분석툴에 저장된 일정보가 로컬PC의 년월일보다 이후 날짜면... 로컬PC 월정보를 -1 해준다.
				if (_atoi64(strTemp1) > _atoi64(strCurDate))
				{
					int nYear	= atoi(strCurYear);
					int nMon	= atoi(strCurMon);
					if (nMon == 1)
					{
						// 로컬pc날짜가 1월인 경우...  년도를 -1 하고 월은 12월로 처리...
						nMon = 12;
						nYear--;
					}
					else
					{
						nMon--;
					}
					strYearMon.Format("%02d%02d", nYear, nMon);
				}
				
				strMakeTime = strYearMon + strDay + strX.Right(6);
			}
			break;
		default:
			{
				strMakeTime = strCurFull;
			}
			break;
		}
	}

	lMakeTime = _atoi64(strMakeTime);
	return lMakeTime;
}
// 2011.02.07 by SYS <<


void CElementSaverBuilder::AddElement(const COrderSet& orderset, COrder &order)
{
	for(int nIndex = 0; nIndex < orderset.GetCount(); nIndex++) {
		CDrawingSymbol* pSymbol = orderset.GetElement(nIndex);
		if(pSymbol != NULL)
			order.Add(pSymbol);
	}
}

void CElementSaverBuilder::AddElement_ForUndoRedo(const COrderSet& orderset, COrder &order)
{
	POSITION pos = orderset.GetHeadPosition();
	while(pos)
	{
		COrderAndElement* pOrderAndElement = orderset.GetNext(pos);
		CDrawingSymbol* pSymbol = NULL;
		if(pOrderAndElement != NULL && 
			(pSymbol = pOrderAndElement->GetElement()) != NULL)
			order.Add(pSymbol);
	}
}