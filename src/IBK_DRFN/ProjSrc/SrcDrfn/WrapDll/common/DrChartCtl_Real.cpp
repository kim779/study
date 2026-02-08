#include "stdafx.h"
#include "DrChartCtl.h"

#include <math.h>

//--> [위닉스 리얼관련 설명]
//	주식:'D', ELW:'y', 시간외:'d', 선물옵션 : 'L',
//	해외선물  호가 :'2', 체결 : '1', FX : '3'
//	Flag 없으면	32번쨰, 34번째 값이 있으면 체결
//	시간외의 경우만 732,734 에 값이 있으면 체결
//<--

//#ifdef __MULTI_LANG__
#include "../../Chart/Include_Chart/Dll_Load/IMetaTable.h"	//@Solomon-MultiLang:091117
//#endif

long CDrChartCtl::DoRealData_NewReal(WPARAM wParam, LPARAM lParam)
{
	if(	!m_hWnd || IsWindow(m_hWnd)==FALSE)	return 0L;

	if (!m_pIRealReceiver) // 차트 리얼 수신 준비
	{
		m_pIRealReceiver = (IRealReceiver*)(m_pChart->GetAddInCustomInterface("ChartCommonAddIn.dll:DIRECT_REAL"));
		if (!m_pIRealReceiver)
			return 0L;
	}

	struct _alertR* alertR = (struct _alertR*)lParam;
	if(!alertR) return 0L;

	CString strCode;
	if (m_converter.m_bDirectSend)
	{
		strCode = alertR->code;
		if(strCode.GetLength()==0) return 0L;
		if (strCode.Compare("X1029")==0 ||
			strCode.Compare("XXXXX")==0 ) return 0L;

		if (strCode.GetAt(0) == 'A')		strCode = strCode.Mid(1);
		if (strCode.GetAt(0) == 'K')
		{
			CString strTemp;
			CWinixConverter::GetSiteUpKode(strCode.Mid(1), strTemp);
			strCode = strTemp;
		}
		
		if (strCode.IsEmpty())
			return -1;
		
		if (m_realMng.GetRealKey(strCode) < 0)
			return -1;
		//	-------------------------------------------
	}

	//DoWriteToFile(lParam);		//RealData To LogFile
	
	CString strRQ;
	BOOL bRQ = FALSE, bRet = TRUE;
	CChartProp info;
	CChartPropArr arrInfo;
	if (!strCode.IsEmpty())
	{
		m_chartMng.GetChartPropArr(strCode, arrInfo);
		int nSize = arrInfo.GetSize();
		DWORD* pArrData = NULL;
		//@by alzioyes:100406 최근Real이 0번째.
		//for (int i = 0; i < alertR->size; i++)
		for (int i=alertR->size-1; i>=0; i--)
		{
			pArrData = (DWORD*)alertR->ptr[i];
			for (int i=0; i<nSize; i++)
			{
				info = arrInfo.GetAt(i);
				strRQ = info.GetRQ();
				if (strRQ.Compare(_T("DEFAULT")) == 0)
					bRet = SetRealData_NewReal(&info, pArrData, TRUE);
				else
					bRet = SetRealDataRQ_NewReal(strRQ, &info, pArrData, TRUE);
			}
		}
	}
	else
	{
		DWORD* pArrData = NULL;

		//@by alzioyes:100406 최근Real이 0번째.
		//for (int i = 0; i < alertR->size; i++)
		for (int i=alertR->size-1; i>=0; i--)
		{
			pArrData = (DWORD*)alertR->ptr[i];
			bRet = SetRealData_NewReal(&m_converter.m_prop, pArrData, FALSE);
		}
	}

	return 1L;
}

long CDrChartCtl::DoRealData_NewReal_Sub(CString& strCode, DWORD* pArrData)
{
	CMap<int, int, CString, LPCTSTR> mapRealData;

	CString strRQ;
	BOOL bRQ = FALSE, bRet = TRUE;
	CChartProp info;
	CChartPropArr arrInfo;
	if (!strCode.IsEmpty())
	{
		m_chartMng.GetChartPropArr(strCode, arrInfo);
		int nSize = arrInfo.GetSize();
		for (int i=0; i<nSize; i++)
		{
			info = arrInfo.GetAt(i);
			strRQ = info.GetRQ();
			if (strRQ.Compare(_T("DEFAULT")) == 0)
				bRet = SetRealData_NewReal(&info, pArrData, TRUE);
			else
				bRet = SetRealDataRQ_NewReal(strRQ, &info, pArrData, TRUE);
		}
	}
	else
	{
		bRet = SetRealData_NewReal(&m_converter.m_prop, pArrData, FALSE);
	}
	
	return 1L;
}

void _gGetMultiLangString(CString& szOrgName, CString &szInUseName)
{
	szInUseName = g_iMetaTable.GetMetaTextDirectly(IMetaTable::PACKET_NAME, IMetaTable::KOREAN, szOrgName);
}

BOOL CDrChartCtl::SetRealDataRQ(CString strRQ, CChartProp* pProp, CMap<int, int, CString, LPCTSTR> &map, BOOL bAdv)
{
	if (!pProp->m_bReal)
		return TRUE;

	double dValue;
	BOOL bAppend = FALSE, bSetReal = FALSE;
	CString strPacket, strValue, strDate;
	CString szInUseName;

	int nFID;
	ST_CHART_PACKET stPack;

	int nSize = pProp->GetPacketSize();
	for (int i=0; i<nSize; i++)
	{
		stPack = pProp->GetPacket(i);
		if (stPack.bReal)
		{
			nFID = atoi(stPack.szRealFID);
			strPacket = stPack.szPacketName;

			if ((nFID == m_converter.m_nFidTypeDate) || (nFID == m_converter.m_nFidTypeTime))
			continue;

			if (!map.Lookup(nFID, strValue))
				continue;
			
			if (!bSetReal && map.Lookup(34, strDate))
			{
				if (strDate.CompareNoCase(_T("장종료")) == 0)		strDate = _T("9999");
				else if (strDate.CompareNoCase(_T("장마감")) == 0)	strDate = _T("8888");

				dValue = atof(strDate);
				if (dValue <= 0)
					return FALSE;
				
				if (m_converter.m_strTimeName.GetLength())
				{
					_gGetMultiLangString(m_converter.m_strTimeName, szInUseName);
					m_pIRealReceiver->SetRealDataRQ(strRQ, szInUseName, dValue, bAppend);
					bSetReal = TRUE;
				}
				else if (m_converter.m_strDateName.GetLength())
				{
					_gGetMultiLangString(m_converter.m_strDateName, szInUseName);
					m_pIRealReceiver->SetRealDataRQ(strRQ, szInUseName, dValue, bAppend);
					bSetReal = TRUE;
				}
			}
			//<<20100310_JS.Kim	FX		FX 실시간 가격 앞에 1byte 는 등락구분 값이라 삭제해야한다.
			if (m_converter.GetChartType() == 2)
			{
				if( nFID == 25 || nFID == 26 || nFID == 129 || nFID == 130 || nFID == 131 || nFID == 132 || nFID == 133 || nFID == 134 )
					strValue = strValue.Mid(1);
			}
			//>>

			dValue = atof(strValue);
			if (m_converter.m_nRealAbs)
				dValue = fabs(dValue);
			else
			{
				if (bAdv && pProp->IsABS())
					dValue = fabs(dValue);
			}

			_gGetMultiLangString(strPacket, szInUseName);
			m_pIRealReceiver->SetRealDataRQ(strRQ, szInUseName, dValue, bAppend);
			bSetReal = TRUE;
		}
	}

	if (bSetReal)
		m_pIRealReceiver->SetRealPacketEndRQ(strRQ);

	return TRUE;
}

BOOL CDrChartCtl::SetRealDataRQ_NewReal(CString strRQ, CChartProp* pProp, DWORD* pArrData, BOOL bAdv)
{
	if (!pProp->m_bReal)
		return TRUE;

	CString szFlag = (char*)pArrData[0];
//	if(szFlag.GetLength()==0) return FALSE;
	//<<20100322_JS.Kim 맨앞에는 실시간 데이타 구분자임.
	// File상단의 [위닉스 리얼관련 설명]
// 	char chGB = szFlag.GetAt(0);
// 	if (m_converter.GetChartType() == 2 )
// 	{
// 		if( chGB != '3' )		return FALSE;
// 	}
// 	else 
// 	{
// 		if( chGB != 'D' && chGB != 'y' && chGB != 'L' )		return FALSE;
// 	}
	//>>

	double dValue;
	BOOL bAppend = FALSE, bSetReal = FALSE;
	CString strPacket, strValue, strDate;
	CString szInUseName;
	
	int nFID;
	ST_CHART_PACKET stPack;

	int nGB = 0, nPcnt;
	double dRate;
	char cType;
	CString strTextType, strGB;
	if (m_converter.GetChartType() == 1)
	{
		strGB = (char*)(char*)pArrData[309];	//	해외선물종목 시세표시구분
		cType = m_converter.GetDecimalRate(strGB, nPcnt, dRate, strTextType);
	}

	int nSize = pProp->GetPacketSize();
	for (int i=0; i<nSize; i++)
	{
		stPack = pProp->GetPacket(i);
		if (stPack.bReal)
		{
			nFID = atoi(stPack.szRealFID);
			strPacket = stPack.szPacketName;

			if ((nFID == m_converter.m_nFidTypeDate) || (nFID == m_converter.m_nFidTypeTime))
				continue;
			
			//if (!map.Lookup(nFID, strValue))	continue;
			strValue = (char*)pArrData[nFID];
			if(strValue.GetLength()==0) continue;

			//sashia, sini	작업할 부분.	------------->>
			if (m_converter.GetChartType() == 1)
			{
				strTextType = stPack.szTextType;
				if (strTextType.GetLength() > 2)
					strTextType = strTextType.Mid(2);
				
				if (atof(strTextType) == 0.000001)	// "0.000001"로 되어 있는 것은 해외인식하여 작업함.
				{
					if (!m_converter.TbondPrcLToF(cType, nPcnt, dRate, strValue))
						strValue.Format(_T("%lf"), m_converter.DataSeparation(strValue, nPcnt));
				}
			}
			//sashia	작업할 부분.	-------------<<

			strDate = (char*)pArrData[34];
			if (!bSetReal && strDate.IsEmpty()==FALSE)
			//if (!bSetReal && map.Lookup(34, strDate))
			{
				if (strDate.CompareNoCase(_T("장종료")) == 0)		strDate = _T("9999");
				else if (strDate.CompareNoCase(_T("장마감")) == 0)	strDate = _T("8888");
				
				dValue = atof(strDate);
				if (dValue <= 0)
					return FALSE;
				
				if (m_converter.m_strTimeName.GetLength())
				{
					_gGetMultiLangString(m_converter.m_strTimeName, szInUseName);
					m_pIRealReceiver->SetRealDataRQ(strRQ, szInUseName, dValue, bAppend);
					bSetReal = TRUE;
				}
				else if (m_converter.m_strDateName.GetLength())
				{
					_gGetMultiLangString(m_converter.m_strDateName, szInUseName);
					m_pIRealReceiver->SetRealDataRQ(strRQ, szInUseName, dValue, bAppend);
					bSetReal = TRUE;
				}
			}
			
			dValue = atof(strValue);
			if (m_converter.m_nRealAbs)
				dValue = fabs(dValue);
			else
			{
				if (bAdv && pProp->IsABS())
					dValue = fabs(dValue);
			}
			
			_gGetMultiLangString(strPacket, szInUseName);
			m_pIRealReceiver->SetRealDataRQ(strRQ, szInUseName, dValue, bAppend);
			bSetReal = TRUE;
		}
	}
	
	if (bSetReal)
		m_pIRealReceiver->SetRealPacketEndRQ(strRQ);
	
	return TRUE;
}

BOOL CDrChartCtl::SetRealData(CChartProp* pProp, CMap<int, int, CString, LPCTSTR> &map, BOOL bAdv)
{
	if (!pProp->m_bReal)
		return TRUE;

	double dValue=0;
	BOOL bAppend = FALSE, bSetReal = FALSE;
	CString strPacket, strValue, strDate;
	CString szInUseName;

	int nFID;
	ST_CHART_PACKET stPack;

	CString strTextType, strGB;
	int nSize = pProp->GetPacketSize();
	for (int i=0; i<nSize; i++)
	{
		stPack = pProp->GetPacket(i);
		if (stPack.bReal)
		{
			nFID = atoi(stPack.szRealFID);
			strPacket = stPack.szPacketName;

			if ((nFID == m_converter.m_nFidTypeDate) || (nFID == m_converter.m_nFidTypeTime))
				continue;

			if (!map.Lookup(nFID, strValue))
				continue;

			//sashia, sini	작업할 부분.	------------->>
			if (m_converter.GetChartType() == 1)
			{
				strTextType = stPack.szTextType;
				if (strTextType.GetLength() > 2)
					strTextType = strTextType.Mid(2);
				
				if (atof(strTextType) == 0.000001)	// "0.000001"로 되어 있는 것은 해외인식하여 작업함.
				{
					if (!map.Lookup(309, strGB))	//	해외선물종목 시세표시구분
					{
//						m_converter.GetDecimalRate(strGB, nPcnt, dRate, strRes);
					}
				}
			}
			//sashia	작업할 부분.	-------------<<
			//<<20100310_JS.Kim	FX		FX 실시간 가격 앞에 1byte 는 등락구분 값이라 삭제해야한다.
			else if (m_converter.GetChartType() == 2)
			{
				if( nFID == 25 || nFID == 26 || nFID == 129 || nFID == 130 || nFID == 131 || nFID == 132 || nFID == 133 || nFID == 134 )
					strValue = strValue.Mid(1);
			}
			//>>

			if (!bSetReal && map.Lookup(34, strDate))
			{
				if (strDate.CompareNoCase(_T("장종료")) == 0)		strDate = _T("9999");
				else if (strDate.CompareNoCase(_T("장마감")) == 0)	strDate = _T("8888");

				dValue = atof(strDate);
				if (dValue <= 0)
					return FALSE;
				
				if (m_converter.m_strTimeName.GetLength())
				{
					_gGetMultiLangString(m_converter.m_strTimeName, szInUseName);
					m_pIRealReceiver->SetRealDataRQ(_T("DEFAULT"), szInUseName, dValue, bAppend);
					bSetReal = TRUE;
				}
				else if (m_converter.m_strDateName.GetLength())
				{
					_gGetMultiLangString(m_converter.m_strDateName, szInUseName);
					m_pIRealReceiver->SetRealDataRQ(_T("DEFAULT"), szInUseName, dValue, bAppend);
					bSetReal = TRUE;
				}
			}

			dValue = atof(strValue);
			if (m_converter.m_nRealAbs)
				dValue = fabs(dValue);
			else
			{
				if (bAdv && pProp->IsABS())
					dValue = fabs(dValue);
			}

			_gGetMultiLangString(strPacket, szInUseName);
			m_pIRealReceiver->SetRealDataRQ(_T("DEFAULT"), szInUseName, dValue, bAppend);
			bSetReal = TRUE;

		}
	}

	if (bSetReal)
		m_pIRealReceiver->SetRealPacketEndRQ(_T("DEFAULT"));

	return TRUE;
}

BOOL CDrChartCtl::SetRealData_NewReal(CChartProp* pProp, DWORD* pArrData, BOOL bAdv)
{
	if (!pProp->m_bReal)
		return TRUE;
	int nChartType = m_converter.GetChartType(); //0:기본차트 1:해외차트 2:FX차트 3:CME차트
	CString szFlag = (char*)pArrData[0];
//	if(szFlag.GetLength()==0) return FALSE;
	//<<20100322_JS.Kim 맨앞에는 실시간 데이타 구분자임.
	// File상단의 [위닉스 리얼관련 설명]
// 	if( szFlag.GetLength() <= 0 )	return FALSE;
// 	char chGB = szFlag.GetAt(0);
// 	if (nChartType == 2 )
// 	{
// 		if( chGB != '3' )		return FALSE;
// 	}
// 	else 
// 	{
// 		if( chGB != 'D' && chGB != 'y' && chGB != 'L' )		return FALSE;
// 	}
 	if (nChartType == 1 )
 	{
 		if(szFlag.IsEmpty() || szFlag.GetAt(0) != '1' )
			return TRUE;
 	}

	//>>

	double dValue=0;
	BOOL bAppend = FALSE, bSetReal = FALSE;
	CString strPacket, strValue, strDate;
	CString szInUseName;
	
	int nFID;
	ST_CHART_PACKET stPack;

	int nGB = 0, nPcnt;
	double dRate;
	char cType;
	CString strTextType, strGB;
	if (nChartType == 1)
	{
		strGB = (char*)(char*)pArrData[309];	//	해외선물종목 시세표시구분
		cType = m_converter.GetDecimalRate(strGB, nPcnt, dRate, strTextType);
	}

	int nSize = pProp->GetPacketSize();
	for (int i=0; i<nSize; i++)
	{
		stPack = pProp->GetPacket(i);
		if (stPack.bReal)
		{
			nFID = atoi(stPack.szRealFID);
			strPacket = stPack.szPacketName;

			if ((nFID == m_converter.m_nFidTypeDate) || (nFID == m_converter.m_nFidTypeTime))
				continue;
			
			//if (!map.Lookup(nFID, strValue))	continue;
			strValue = (char*)pArrData[nFID];
			if(strValue.GetLength()==0) continue;

			//sashia, sini	작업할 부분.	------------->>
			if (nChartType == 1 || nChartType == 3)
			{
				//<<20100310_JS.Kim			 실시간 가격 앞에 1byte 는 등락구분 값이라 삭제해야한다.
				strValue.Remove('-');
				strValue.Remove('+');
				//>>

				strTextType = stPack.szTextType;
				if (strTextType.GetLength() > 2)
					strTextType = strTextType.Mid(2);
				
				if (atof(strTextType) == 0.000001)	// "0.000001"로 되어 있는 것은 해외인식하여 작업함.
				{
					if (!m_converter.TbondPrcLToF(cType, nPcnt, dRate, strValue))
						strValue.Format(_T("%lf"), m_converter.DataSeparation(strValue, nPcnt));
				}
			}
			//sashia	작업할 부분.	-------------<<

			// 종목코드로 시장정보를 얻어와도 스프레드선물이 선물로 취급되어 케이스별 예외처리.(위닉스플랫폼...)
			// 스프레드 선물일 경우 앞자리의 부호는 색상을 나타냄 (+, -, 0)상승/하락/보합
			// 스프레드 선물 현재가는 음수를 갖을 수 있음. 뒷자리 부호는 실제 현재가격의 부호.
			// 상승/하락 색상은 등락률 부호를 사용하므로 따로 처리 안함.
			CString strCode;
			strCode = m_converter.m_strCurCode;
			if (strCode.Left(1) == "A" || strCode.Left(1) == "Q")
				strCode = strCode.Mid(1);

			int nMarket = m_converter.GetMarketTypeFromCode(strCode);

			if (strCode != pProp->GetCode())
				return FALSE;

			strDate = (char*)pArrData[m_converter.m_nFidTypeTime];
			if (!bSetReal && strDate.IsEmpty()==FALSE)
			{
				if (strDate.CompareNoCase(_T("장종료")) == 0)		strDate = _T("9999");
				else if (strDate.CompareNoCase(_T("장마감")) == 0)	strDate = _T("8888");
				dValue = atof(strDate);

				if (dValue <= 0)
					return FALSE;
				
				if (m_converter.m_strTimeName.GetLength())
				{
					_gGetMultiLangString(m_converter.m_strTimeName, szInUseName);
					m_pIRealReceiver->SetRealDataRQ(_T("DEFAULT"), szInUseName, dValue, bAppend);
					bSetReal = TRUE;
				}
				else if (m_converter.m_strDateName.GetLength())
				{
					_gGetMultiLangString(m_converter.m_strDateName, szInUseName);
					m_pIRealReceiver->SetRealDataRQ(_T("DEFAULT"), szInUseName, dValue, bAppend);
					bSetReal = TRUE;
				}
			}
			//<<20100310_JS.Kim	FX		FX 실시간 가격 앞에 1byte 는 등락구분 값이라 삭제해야한다.
			if (nChartType == 2)
			{
				if( nFID == 25 || nFID == 26 || nFID == 129 || nFID == 130 || nFID == 131 || nFID == 132 || nFID == 133 || nFID == 134 )
					strValue = strValue.Mid(1);
			}
			//>>					

			if( nFID == 23 ) //선물종가
			{
//				strValue = strValue.Mid( 1 );
				if (nMarket == CMasterTypeInfo::FUTOPT_CHART || nMarket == CMasterTypeInfo::STOCKFUTOPT_CHART || nMarket == CMasterTypeInfo::UPJONG_CHART)
					dValue = atof(strValue) * 100;
				else
					dValue = atof(strValue);

				dValue = fabs(dValue);
			}
			else
				dValue = atof(strValue);

			if (m_converter.m_nRealAbs)
				dValue = fabs(dValue);
			else
			{
				if (bAdv && pProp->IsABS())
					dValue = fabs(dValue);
			}

			_gGetMultiLangString(strPacket, szInUseName);
			m_pIRealReceiver->SetRealDataRQ(_T("DEFAULT"), szInUseName, dValue, bAppend);
			bSetReal = TRUE;
		}
	}
	
	if (bSetReal)
		m_pIRealReceiver->SetRealPacketEndRQ(_T("DEFAULT"));
	
	return TRUE;
}

int CDrChartCtl::RealDataParsing(char* pszBuff, CMap<int, int, CString, LPCTSTR> &map)
{
	int nIndex=0, nDataIdx=0, nDataCount=0;
	CString strData;
	int nFID = 0;
	char szData[128];

	memset(szData, 0x00, sizeof(szData));

	while (pszBuff[nIndex] != 0x00)
	{
		if (pszBuff[nIndex] == '\t')
		{
			strData.Format(_T("%s"), szData);
			memset(szData, 0x00, sizeof(szData));

			if (nDataCount % 2 == 1)
				map.SetAt(nFID, strData);
			else
				nFID = atoi(strData);

			nDataCount++;
			nIndex++;
			nDataIdx=0;
		}
		else
			szData[nDataIdx++] = pszBuff[nIndex++];

		ASSERT(nDataIdx < sizeof(szData));
	}

	return 1;
}

int CDrChartCtl::RealDataParsing_NewReal(DWORD* pArrData, CMap<int, int, CString, LPCTSTR> &map)
{
	char* pItemData;
	CString strData;
	for(int nFID=0; nFID<1000; nFID++)
	{
		pItemData = (char*)pArrData[nFID];
		if(!pArrData[nFID])
		{
			strData = (char*)pArrData[nFID];
			map.SetAt(nFID, strData);
		}
	}

	return 1;
}

CString CDrChartCtl::ConvertToValidData( CString strData )
{
	CString strResult;
	return strResult;
}