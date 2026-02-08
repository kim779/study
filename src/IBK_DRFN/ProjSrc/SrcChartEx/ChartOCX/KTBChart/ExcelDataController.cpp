// ExcelDataController.cpp: implementation of the CExcelDataController class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KTBChart.h"
#include "ExcelDataController.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "PacketList.h"
#include "PacketBase.h"
#include "Conversion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExcelDataController::CExcelDataController( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
{
	// (2008/11/27 - Seung-Won, Bae) for Normalized Minute Time Rule candle.
	m_nHeaderDataCount = 0;

	m_RowDataList.RemoveAll();

	// (2008/5/14 - Seung-Won, Bae) for Multi-Language of Special Date or Price Unit Text
	m_hOcxWnd = p_hOcxWnd;

	// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
	m_eChartMode = p_eChartMode;
}

CExcelDataController::~CExcelDataController()
{

}

CString CExcelDataController::GetStrValue(HKEY Hkey,CString strkey, CString strsubkey,CString strRValue)
{
   LONG err;
   HKEY openKey;
   BYTE Data[128];
   DWORD type;
   DWORD size=sizeof(Data);

   err=RegOpenKey(Hkey,strkey,&openKey);

   if(ERROR ==err)
	   err=RegQueryValueEx(openKey,strsubkey,0,&type,Data,&size);
	 
   if(err==ERROR_SUCCESS)
	{
		RegCloseKey(openKey);
		CString str(Data);
		return str;
	}
	
   RegCloseKey(openKey);
	return strRValue;

}

BOOL CExcelDataController::IsAllZeroData(CList<double, double>* datalist)
{
	POSITION pos = datalist->GetHeadPosition();
	while(pos) {
		double data = datalist->GetNext(pos);
		if(data) 
			return FALSE;
	}
	return TRUE;
}


CPoint CExcelDataController::GetRowandColumnCount(const CString& strTitles,
									const CString& strPacketNames,
									const CString& strHorzScale,
									CPacketList* pPacketList)
{
	int nColumnCount = CDataConversion::GetStringDataCount(strTitles, "|");
	int nRowCount = 0;

	if(!pPacketList)	return CPoint(0, nColumnCount);

	CPacket * pPacketheader = pPacketList->GetBaseData(strHorzScale);
	if(pPacketheader)	{
		nColumnCount ++;
		nRowCount = pPacketheader->GetCount();
	}
	else	{
		CString strT = strPacketNames;
		CString strPacketName = CDataConversion::GetStringData(strT, ",");
		CPacket * pPacket = pPacketList->GetBaseData(strPacketName);	
		if(pPacket)
			nRowCount = pPacket->GetCount();
	}

	return CPoint(nRowCount, nColumnCount);
}

//strTitles = 가격, 거래량, 5일, 10일~~
//strMyTitle = 일자,시가,고가,저가,종가,~~
void CExcelDataController::CallExcel(const CString& strTitles,
									const CString& strPacketNames,
									const CString& strPacketNamesPerBlock,
									const CString& strHorzScale,
									const CString& strFilePath,
									const BOOL bRun,
									CPacketList* pPacketList)
{
	CString strMyTitle = "";
	CString strTotalData = "";

	if(strPacketNames.IsEmpty())
		return ;
	CString szTitles = strTitles, szPacketNames = strPacketNames, szT = strPacketNamesPerBlock ;

	int ntitleCount = CDataConversion::GetStringDataCount(strTitles, ";");
	int nColumnCount = CDataConversion::GetStringDataCount(strPacketNamesPerBlock, ",") ;

	CPacket* pPacketheader = pPacketList->GetBaseData(strHorzScale);

	strMyTitle += GetHorzScaleTitleData(pPacketheader);
	strMyTitle += GetAllTitleData(ntitleCount, szTitles, strPacketNamesPerBlock);

	GetHorzScaleData(szTitles, strPacketNames, strHorzScale, pPacketheader, pPacketList);
	GetAllData(strTitles, strPacketNames, pPacketList);

	CPoint ptRowAndCol = GetRowandColumnCount(strMyTitle, strPacketNames, strHorzScale, pPacketList);
	WriteExcel( ptRowAndCol, strMyTitle, strFilePath, bRun);
}

CString CExcelDataController::GetHorzScaleTitleData(CPacket* pPacketheader)
{
	CString strTitle = "";
	if(pPacketheader->GetName() == _MTEXT( C0_DATE_TIME))	
	{
		// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
		CString strType = pPacketheader->GetType();
		if (strType == _T("YYYYMMDDHHMMSS"))
		{
			CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = (CScaleBaseData::HORZSCALEDRAWERTYPE)pPacketheader->GetDateUnitType();
			int nHourType = 0;
			if( CScaleBaseData::HORZ_TIME_ONE	== eDateUnit || 
				CScaleBaseData::HORZ_TIME_TWO	== eDateUnit ||
				CScaleBaseData::HORZ_TICK		== eDateUnit ||
				CScaleBaseData::HORZ_TIME_SECOND== eDateUnit)
			{
				if (strType.Find("HH") >= 0)
					nHourType = 1;
			}
			else
			{
				nHourType = 0;
			}

			if (nHourType == 1) 
			{
				strTitle += _LTEXT6( C6_TIME);
				strTitle += "|";
			}
			else {
				strTitle += _LTEXT6( C6_DATE);
				strTitle += "|";
			}
		}
		else
		// 2011.01.27 by SYS <<
		{
			if(strType.Find("HH") >= 0) 
			{
				strTitle += _LTEXT6( C6_TIME);
				strTitle += "|";
			}
			else {
				strTitle += _LTEXT6( C6_DATE);
				strTitle += "|";
			}
		}
	}
	else {
		strTitle += (LPSTR)(LPCTSTR)pPacketheader->GetName();
		strTitle += "|";
	}
	return strTitle;
}

CString CExcelDataController::GetAllTitleData(const int nTitleCount,
											  const CString& szTitles,
											  const CString& strPacketNamesPerBlock)
{
	CString strTitle = "";
	CString strTitles = szTitles;
	CString szT = strPacketNamesPerBlock;

	for(int i = 1 ; i < nTitleCount + 1 ; i++)
	{
		CString maintitle = CDataConversion::GetStringData(strTitles, ";");
		maintitle.TrimLeft(); maintitle.TrimRight();		
		CString strP = CDataConversion::GetStringData(szT, ";");
		int nOneRecordCount = CDataConversion::GetStringDataCount(strP, ",") ;
		if(nOneRecordCount == 1 && !maintitle.IsEmpty()) {	
			strTitle += _MTOL1( (LPSTR)(LPCTSTR)maintitle);
			strTitle += "|";
		}
		else
		{
			for(int p = 0 ; p < nOneRecordCount ; p++)
			{
				CString strName = CDataConversion::GetStringData(strP, ",");
				// (2008/3/21 - Seung-Won, Bae) Check Dynamic Packet Name
				// 1. Rainbow Chart
				int nPrefixLen = 0;
				if( !strncmp( strName, _MTEXT( C0_RAINBOW_PACKET), nPrefixLen = strlen( _MTEXT( C0_RAINBOW_PACKET))))
					strName = _LTEXT0( C0_RAINBOW_PACKET) + strName.Mid( nPrefixLen);
				else
				// 2. Box Chart
				if( !strncmp( strName, _MTEXT( C2_BOX_CHART), nPrefixLen = strlen( _MTEXT( C2_BOX_CHART))))
						strName = CString( _LTEXT2( C2_BOX_CHART)) + "_" + _MTOL0( strName.Mid( nPrefixLen));
				else
				// 3. Others
					strName = _MTOL0( strName);

				strTitle += strName;
				strTitle += "|";
			}
		}
	}
	return strTitle;
}

void CExcelDataController::GetHorzScaleData(const CString& strTitles,
									const CString& strPacketNames,
									const CString& strHorzScale,
									CPacket* pPacketheader,
									CPacketList* pPakcetList)
{
	int nPacketCount = CDataConversion::GetStringDataCount(strPacketNames, ",");
	int nrowcount = pPakcetList->GetMaxCount();
	// (2008/11/27 - Seung-Won, Bae) for Normalized Minute Time Rule candle.
	m_nHeaderDataCount = 0;
	// row
	// 1st column data
	if(pPacketheader)	
	{
		CList<double, double>* datalist = pPacketheader->GetnumericDataList();
		CList<CString, CString>* datalist2 = pPacketheader->GetstrDataList();
		if(datalist != NULL)	
		{
			POSITION pos = datalist->GetTailPosition();
			// (2008/11/27 - Seung-Won, Bae) for Normalized Minute Time Rule candle.
			m_nHeaderDataCount = datalist->GetCount();
			for(int j = 0 ; j < m_nHeaderDataCount; j++)	
			{
				//nami 9.2
				CString strv;
				if(strHorzScale.Compare( _MTEXT( C0_DATE_TIME)) == 0) 
				{
					strv = CDataConversion::DoubleToString(datalist->GetPrev(pos));

					// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
					CString strType = pPacketheader->GetType();
					if (strType == _T("YYYYMMDDHHMMSS"))
					{
						// nDataUnit (0:nontype, 1:일, 2:주, 3:월, 4:분, 5:틱, 6:Text)
						CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = (CScaleBaseData::HORZSCALEDRAWERTYPE)pPacketheader->GetDateUnitType();
						
						CString strFormat = _T("YYYYMMDD");	// 디폴트포맷
						switch(eDateUnit)
						{
						case CScaleBaseData::HORZ_DAILY:		// 일
						case CScaleBaseData::HORZ_WEEKLEY:		// 주 
							strFormat = _T("YYYYMMDD");		
							break;
						case CScaleBaseData::HORZ_MONTHLY:		// 월 
							strFormat = _T("YYYYMM");		
							break;
						case CScaleBaseData::HORZ_TIME_ONE:		// 분
						case CScaleBaseData::HORZ_TIME_TWO:
							strFormat = _T("MMDDHHMM");
							break;
						case CScaleBaseData::HORZ_TICK:			// 틱 
						case CScaleBaseData::HORZ_TIME_SECOND:
							strFormat = _T("DDHHMMSS");
							break;
						}
						strv = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(strv, strFormat, "");
					}
					else
					// 2011.01.27 by SYS <<
					{
						strv = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(strv, strType, "");
					}
				}
				else
					strv = CDataConversion::DoubleToString(datalist->GetPrev(pos));

				CString strValue;
				strValue += (LPSTR)(LPCTSTR)strv;
				strValue += _T("\t");
				m_RowDataList.AddTail(strValue);
			}
		}
		else if(datalist2 != NULL)	{
			POSITION pos = datalist2->GetTailPosition();
			// (2008/11/27 - Seung-Won, Bae) for Normalized Minute Time Rule candle.
			m_nHeaderDataCount = datalist2->GetCount();
			for(int j = 0 ; j < m_nHeaderDataCount; j++)	{
				CString strv = datalist2->GetPrev(pos);

				CString strValue;
				strValue += (LPSTR)(LPCTSTR)strv;
				strValue += _T("\t");
				m_RowDataList.AddTail(strValue);
			}
		}
	}
}

void CExcelDataController::GetAllData(const CString& strTitles,
										 const CString& strPacketNames,
										 CPacketList* pPakcetList)
{
	// insert data value
	CString strPT = strPacketNames;
	int nPacketCount = CDataConversion::GetStringDataCount(strPacketNames, ",");
	for(int i = 1 ; i < nPacketCount + 1 ; i++)	{	
		CPacket * pPacket = pPakcetList->GetBaseData(CDataConversion::GetStringData(strPT, ","));
		if(!pPacket)	continue;
		CList<double, double>* datalist = pPacket->GetnumericDataList();
		CList<CString, CString>* datalist2 = pPacket->GetstrDataList();
		if(datalist != NULL)
		{
		   //nami 8.23
			// (2007/5/30 - Seung-Won, Bae) Support Empty Packet for Added Title.
			if( IsAllZeroData(datalist))
			{
				POSITION rowDataListPos = m_RowDataList.GetHeadPosition();
				POSITION prevDataListPos = rowDataListPos;
				while( rowDataListPos)
				{
					CString strValue = m_RowDataList.GetNext( rowDataListPos);
					strValue += "0";
					strValue += _T("\t");;
					m_RowDataList.SetAt( prevDataListPos, strValue);
					prevDataListPos = rowDataListPos;
				}
			}
			else
			{
				POSITION pos = datalist->GetTailPosition();
				// (2008/11/27 - Seung-Won, Bae) for Normalized Minute Time Rule candle.
				int nColumnDataCount = pPacket->GetCount();
				// column
				for(int j = 0 ; j < nColumnDataCount; j++)	{
					CString strv = CDataConversion::DoubleToString(datalist->GetPrev(pos));
					if(pPacket->GetType().Find("MM") > 0)
						strv = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(strv, pPacket->GetType(), "");
					else
						strv = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(strv, pPacket->GetType());

					//sy 2002.10.4
					// (2008/11/27 - Seung-Won, Bae) for Normalized Minute Time Rule candle.
					POSITION rowDataListPos = FindPosition( j + ( 0 < m_nHeaderDataCount ? m_nHeaderDataCount - nColumnDataCount : 0));
					if(rowDataListPos == NULL)
						break;
					CString strValue = m_RowDataList.GetAt(rowDataListPos);
					strValue += (LPSTR)(LPCTSTR)strv;
					strValue += _T("\t");;
					m_RowDataList.SetAt(rowDataListPos, strValue);
				}		
			}
		}
		else if(datalist2 != NULL)	{
			POSITION pos = datalist2->GetTailPosition();
			// column
			for(int j = 0 ; j < pPacket->GetCount() ; j++)	{
				CString strv = datalist2->GetPrev(pos);
				//sy 2002.10.4
				POSITION rowDataListPos = FindPosition(j);
				if(rowDataListPos == NULL)
					break;
				CString strValue = m_RowDataList.GetAt(rowDataListPos);
				strValue += (LPSTR)(LPCTSTR)strv;
				strValue += _T("\t");;
				m_RowDataList.SetAt(rowDataListPos, strValue);
			}		
		}
	}
}

void CExcelDataController::WriteExcel(const CPoint& ptRowAndCol,
									  const CString& strTitle,
									  const CString& strFilePath,
									  const BOOL bRun)
{
	SYSTEMTIME sTime;
	GetSystemTime(&sTime);

	CString  szFilePath ;
	szFilePath.Format("%sChartData_%04d%02d%02d_%02d%02d%02d.nc2", strFilePath, 
				sTime.wYear, sTime.wMonth, sTime.wDay, sTime.wHour+9, sTime.wMinute, sTime.wSecond);

	CString szExcel = "NO" ;
	CString strKey = "Excel.csv\\shell\\open\\command" ;
	int Ver = 60 ;
	HKEY hkey = HKEY_CLASSES_ROOT ;
	szExcel = GetStrValue(hkey, strKey, "", "NO") ;
	szExcel.MakeLower() ;
	int start = szExcel.Find("excel.exe") ;
	szExcel = szExcel.Left(start + 9) ;
	if(szExcel.Left(1).Compare("\"") == 0)
		szExcel = szExcel.Right(szExcel.GetLength() - 1) ;

	CreateDirectory("C:\\Temp", NULL) ; 
	CFile file;
	CFileException e;
	if( !file.Open( szFilePath, CFile::modeCreate | CFile::modeWrite, &e ) )
	{
		#ifdef _DEBUG
			afxDump << "File could not be opened " << e.m_cause << "\n";
		#endif
    }

	CString szData, szTmp;
	
	WriteColumnHeader(strTitle, &file);
	WriteAllData(ptRowAndCol, &file);

	file.Close();

	// 04.17.2002 파일명 앞뒤에 따옴표(" ")가 있어야 한다
	szExcel = szExcel + " \"" + szFilePath + "\"";	

	ML_SET_LANGUAGE_RES();
	CString strFormat;
	strFormat.LoadString( IDS_FORMAT_EXCEL_FILE_SAVED);
	if(bRun)
	{
		if(::WinExec((LPCTSTR)szExcel, SW_SHOW) > 31)
			return;
		else	
		{
			CString szMsg;
			szMsg.Format( strFormat,szFilePath);
			AfxMessageBox(szMsg); //,"nfxChart") ;
			return;
		}
		return;
	}
	else
	{
		CString szMsg;
		szMsg.Format( strFormat, szFilePath);
		AfxMessageBox(szMsg); //,"nfxChart") ;
		return;
	}
}

void CExcelDataController::WriteColumnHeader(const CString& strTitle,
											 CFile* file)
{
	// header column
	CString title = strTitle;

	CString szData = "";
	int nCount = CDataConversion::GetStringDataCount(title, "|"); 
	for(int col = 0 ; col < nCount ; col++) 
	{
		CString szTmp = CDataConversion::GetStringData(title, "|");
		szData += szTmp;
		szData += _T("\t");
	}
	szData += _T("\r\n");
	file->Write(szData, szData.GetLength());
}

void CExcelDataController::WriteAllData(const CPoint& ptRowAndCol,
										CFile* file)
{
	CString szRowData = "";
	for(int row = 0  ; row < ptRowAndCol.x ; row++) 
	{
		szRowData = m_RowDataList.GetAt(FindPosition(row));
		szRowData +=_T("\r\n");
		file->Write(szRowData, szRowData.GetLength());
	}
}

POSITION CExcelDataController::FindPosition(const int nIndex) const
{	
	int nTailIndex = m_RowDataList.GetCount() -1;

	if(nIndex < 0 || nIndex > nTailIndex)
		return NULL;

	if(nIndex > nTailIndex/2){
		int nCount = nTailIndex;
		POSITION pos = m_RowDataList.GetTailPosition();
		while(pos != NULL){
			if(nCount-- == nIndex)
				return pos;
			m_RowDataList.GetPrev(pos);
		}
	}
	else{
		int nCount = 0;
		POSITION pos = m_RowDataList.GetHeadPosition();
		while(pos != NULL){
			if(nCount++ == nIndex)
				return pos;
			m_RowDataList.GetNext(pos);
		}
	}

	return NULL;
}
