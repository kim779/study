#include "StdAfx.h"
#include ".\msgforchart.h"


CMsgForChart::CMsgForChart()
: m_LengthOfSubMsg(0)
, m_strData(_T(""))
, m_strMsg(_T(""))
, m_strPropChart(_T(""))
, m_strSubMsg(_T(""))
, m_pItems(NULL)
, m_strExtMsg(_T(""))
, m_nMultiGID(0)
, m_strGIDs(_T(""))
{
	m_pItems = NULL;
	m_pItems = new CObList;
	
}

CMsgForChart::~CMsgForChart(void)
{
	if(m_pItems)
		delete m_pItems;
}

////////////////////////////////////////////////
// OCX 에 잔달할 데이터 메시지 생성
void CMsgForChart::GenerateMsg(void)
{	
	// Header 생성: 데이터 개수 + 보조메시지 길이 + 보조 메시지
	m_strMsg = m_strCount + m_strLenOfSubMsg + m_strSubMsg;
	// Data body add
	m_strMsg += m_strData;
}

/////////////////////////////////////////////
// 보조 메시지 만들기
void CMsgForChart::GenerateSubMsg(void)
{
	//////////////////////////////////////////////////////////////////////////
	// 2006년 6월 25일
	// 보조 메시지 생성
	int iToKenStart = 0;
	int iStart = m_strSubMsg.Find(TokenizeEx(m_strExtMsg,"=",iToKenStart), 0);
	int iEnd;
	if( iStart == -1)
		m_strSubMsg += m_strExtMsg;
	else
	{
		iEnd = m_strSubMsg.Find("@",iStart);
		CString strOld = m_strSubMsg.Mid(iStart, iEnd-iStart+1);
		m_strSubMsg.Replace(strOld, m_strExtMsg);
	}
// 	m_strSubMsg = m_strUM + m_strUD + m_strUT + m_strRDATET + m_strPREVPRICE + m_strFUNDSTARTPOINT
// 				+ m_strFUNDENDPOINT + m_strUSEPACKET + m_strBOUND + m_strRESETUSEREAL + m_strRESETPACKET
// 				+ m_strERROR + m_strPRICETYPE + m_strNEWPACKET + m_strDRAWSTARTINDEX + m_strCODEANDNAME
// 				+ m_strDOAPPENDBYREAL + m_strDATE + m_strMARKETTIME + m_strFRDA + m_strUTEC + m_strCB 
// 				+ m_strRDC + m_strBOUNDMARK + m_strExtMsg;


	// 보조 메시지 길이를 구하고 스트링으로 변환
	NumToString(m_strSubMsg.GetLength(), LENGTH_MSG_SUBMSG_COUNT, m_strLenOfSubMsg);
}

void CMsgForChart::AppendData(char* szFormat/*IN*/, ...)
{
	va_list vl;
	va_start( vl, szFormat );
//	FormatData();

//	CString tmp;
//	tmp.Format(szFormat,char*);
	//m_strData. += strAddingData;
}

//////////////////////////////////////////////////////////////////////////////
// map으로부터 유효한 데이터 형식 get & 관리를 위하여 CList인 m_pItems에 등록
// 
CString CMsgForChart::GenerateDataFormat()
{

	CString strFormat, ss;
	int cur=0, prev=0;
	CItemInChartEu* pItem;
	while(1)
	{
		//////////////////////////////////////////////////////////////////////////
		// 한 줄 식 분리, 속성 항목별로 분리 작업
		cur = m_strPropChart.Find(0x0d, prev);
		if(cur == -1)
			break;
		ss = m_strPropChart.Mid(prev, cur-prev);
		prev = cur+2;
		//////////////////////////////////////////////////////////////////////////

		if(!IsValidData(ss)) continue;	// 유효한 것이 아니면 skip

		// OCX packet에서 정보 분리
		pItem = new CItemInChartEu;
		pItem->m_bValid = true;
		pItem->m_strName = GetItemName(ss);
		pItem->m_strField = GetFieldName(ss);
		pItem->m_strType = GetTypeOfItem(ss);
		pItem->m_lLength = GetLengthOfItem(ss);
		pItem->m_lLengthBackup = pItem->m_lLength;
		pItem->m_bReal = (GetRealFlag(ss)== "유")?TRUE:FALSE;

		// 삼성 : TR인 경우 아이템명과 FID를 분리한다. 경우) 아이템명, 아이템명(GID:FID), 레코드명:아이템명(GID:FID)
		//@유진방식: 괄호'()'로 분리되는 부분을 추출한다. 조회용 FID와 리얼처리용으로 분리한다.
		int iStart = 0;
		{
			// 기존의 삼성은 GID 도 가능했지만 유진은 특정 TR별로 셋팅하므로 GID는 없다고 봐야 할것으로 생각됨.
			// () 가 있을 경우에는 리얼팻킷으로 분리한다.
			CString strField = TokenizeEx(pItem->m_strField, "(", iStart);
			m_nMultiGID = 1;
			if(strField == "")	// 리얼세팅이 없는 경우
			{
				pItem->m_nFID = atoi(pItem->m_strField);
			}
			else
			{
				pItem->m_nFID = atoi(strField);
				strField= pItem->m_strField.Mid(iStart);
				strField.TrimRight(")");
				//pItem->m_nRealID = atoi(strField);

				//@Champ-union090827-alzioyes.
				if(strField[0]>='0' && strField[0]<='9')
					pItem->m_nRealID = atoi(strField);
				else
					pItem->m_szRealID = strField;
			}
		}
		
		m_pItems->AddTail(pItem);
	}

	return _T("");
}

// 기존의 루틴 GenerateDataFormat
/*
CString CMsgForChart::GenerateDataFormat()
{

	CString strFormat, ss;
	int cur=0, prev=0;
	CItemInChartEu* pItem;
	while(1)
	{
		//////////////////////////////////////////////////////////////////////////
		// 한 줄 식 분리, 속성 항목별로 분리 작업
		cur = m_strPropChart.Find(0x0d, prev);
		if(cur == -1)
			break;
		ss = m_strPropChart.Mid(prev, cur-prev);
		prev = cur+2;
		//////////////////////////////////////////////////////////////////////////

		if(!IsValidData(ss)) continue;	// 유효한 것이 아니면 skip

		// OCX packet에서 정보 분리
		pItem = new CItemInChartEu;
		pItem->m_bValid = true;
		pItem->m_strName = GetItemName(ss);
		pItem->m_strField = GetFieldName(ss);
		pItem->m_strType = GetTypeOfItem(ss);
		pItem->m_lLength = GetLengthOfItem(ss);
		pItem->m_lLengthBackup = pItem->m_lLength;
		pItem->m_bReal = (GetRealFlag(ss)== "유")?TRUE:FALSE;


		// 삼성 : TR인 경우 아이템명과 FID를 분리한다. 경우) 아이템명, 아이템명(GID:FID), 레코드명:아이템명(GID:FID)
		//@유진방식: 괄호'()'로 분리되는 부분을 추출한다. 조회용 FID와 리얼처리용으로 분리한다.
		int iStart = 0;
		if(pItem->m_strField.Find("(",0) != -1 || atol(pItem->m_strField) == 0)	// TR 방식
		{
			CString strField = pItem->m_strField;
			pItem->m_strField = TokenizeEx(strField, "(",iStart);
			if(pItem->m_strField == "")
			{
				pItem->m_strField = strField;
			}
			else
			{
				// 아이템명에서 레코드명 분리
				int iStart2 = 0;
				CString strRecord = TokenizeEx(pItem->m_strField,":",iStart2);
				if(strRecord != "") // record name이 있다면...
				{
					pItem->m_strRecord = strRecord;
					pItem->m_strField = pItem->m_strField.Right(pItem->m_strField.GetLength() - iStart2);
				}
				// GIF, FID 분리
				CString strGID = TokenizeEx(strField, ":", iStart);
				if(strGID == "")	// GID 가 없다면...
					pItem->m_nFID = atoi(TokenizeEx(strField, ")", iStart));
				else
				{
					pItem->m_nGID = atoi(strGID);
					pItem->m_nFID = atoi(TokenizeEx(strField, ")", iStart));
				}
			}
		}
		else	//FID 방식, 경우) FID, GID:FID, GID:FID:INDEX
		{
			CString strGID = TokenizeEx(pItem->m_strField, ":", iStart);
			if(strGID == "")	// GID가 없는 경우
			{
				m_nMultiGID = 1;
				pItem->m_nFID = atoi(pItem->m_strField);
			}
			else				// GID가 있는 경우
			{

				if(m_strGIDs.Find(strGID) == -1)	// 존재하지 않으면(new인 경우)....추가
				{
					m_strGIDs += strGID + ";";
					m_nMultiGID++;
				}
				pItem->m_nGID = atoi(strGID);
				CString strFID = TokenizeEx(pItem->m_strField, ":", iStart);
				if(strFID == "")	// INDEX가 없는 경우
				{
					pItem->m_nFID = atoi(pItem->m_strField.Right(pItem->m_strField.GetLength()-iStart));
				}
				else				// INDEX가 있는 경우
				{
					pItem->m_nFID = atoi(strFID);
					pItem->m_nIndex = atoi(pItem->m_strField.Right(pItem->m_strField.GetLength()-iStart));
				}
			}
		}
		
		m_pItems->AddTail(pItem);
	}

	return _T("");
}
*/

//////////////////////////////////////////////////
// 데이터의 유/무 get
bool CMsgForChart::IsValidData(CString strItem)
{
	CString res;
	int cur = 0;
	res = TokenizeEx(strItem, ";", cur);	
	return (res == "1") ? true : false;
}

///////////////////////////////////////////////////
// 차트에서 표현하고자 하는 데이터의 이름 get
CString CMsgForChart::GetItemName(CString strItem)
{
	CString res;
	int cur = 0;
	TokenizeEx(strItem, ";", cur);
	res = TokenizeEx(strItem, ";", cur);

	return res;
}

//////////////////////////////////////////////////
// 차트에서 표현하고자 하는 데이터의 필드명 get
CString CMsgForChart::GetFieldName(CString strItem)
{
	CString res;
	int cur = 0;
	TokenizeEx(strItem, ";", cur);
	TokenizeEx(strItem, ";", cur);
	res = TokenizeEx(strItem, ";", cur);

	return res;
}

///////////////////////////////////////////
// 차트에서 표현하고자 하는 데이터의 길이 get
long CMsgForChart::GetLengthOfItem(CString strItem)
{
	CString res;
	int cur = 0;
	TokenizeEx(strItem, ";", cur);	
	TokenizeEx(strItem, ";", cur);
	TokenizeEx(strItem, ";", cur);
	res = TokenizeEx(strItem, ";", cur);	

	return atol((LPSTR)(LPCTSTR)res);
}

//////////////////////////////////////////////////
// 차트에서 표현하고자 하는 데이터의 형식 get
CString CMsgForChart::GetTypeOfItem(CString strItem)
{
	CString res;
	int cur = 0;
	TokenizeEx(strItem, ";", cur);	
	TokenizeEx(strItem, ";", cur);	
	TokenizeEx(strItem, ";", cur);
	TokenizeEx(strItem, ";", cur);
	res = TokenizeEx(strItem, ";", cur);
	return res;
}

CString CMsgForChart::GetRealFlag(CString strItem)
{
	CString res;
	int cur = 0;
	TokenizeEx(strItem, ";", cur);	
	TokenizeEx(strItem, ";", cur);	
	TokenizeEx(strItem, ";", cur);
	TokenizeEx(strItem, ";", cur);
	TokenizeEx(strItem, ";", cur);
	res = TokenizeEx(strItem, ";", cur);
	return res;
}

//////////////////////////////////////////////////////////////////////////
// CString의 tokenize를 대체
// CString의 tokenize를 사용하지 않은 이유
// 필드명을 입력하지 않았을 경우 ""를 인식하지 옷하기 때문
// 자료일자;D1입회일;8;YYYYMMDD;유; <-- OK
// 자료일자;;8;YYYYMMDD;유;			<-- not OK, 이러한 경우를 대비하기 위해서
CString CMsgForChart::TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN, OUT*/)
{
	CString strRes = _T("");
	int iCur = iStart;
	iCur = strSrc.Find(pszTokens, iStart);
	if(iCur != -1)	// tokenize 할 문자가 없을 경우에는 "" 리턴
	{
		strRes = strSrc.Mid(iStart, iCur-iStart);
		iCur++;
		iStart = iCur;
	}
	return strRes;
}
//////////////////////////////////////////////////////////////////////////
// OCX property에서 설정한 packet과 DSItem을 연결하기 위함
// 차트 컨트롤의 property 중 "DSItem연결" 항목과 관련됨
// void CMsgForChart::mappingDSItem(CString strDSItems, CDSArray* pDSItems)
// {
// 	CString strRes;
// 	int cur = 0;
// 	POSITION pos = m_pItems->GetHeadPosition();
// 	while(pos)
// 	{
// 		CItemInChartEu* pItem = (CItemInChartEu*)m_pItems->GetNext(pos);
// 		cur = strDSItems.Find(pItem->m_strName);
// 		if(cur == -1) continue;
// 		strDSItems.Tokenize(":", cur);
// 		strRes = strDSItems.Tokenize(";", cur);
// 		//pItem->m_strField = strRes;
// 		pItem->m_pDSItem = pDSItems->GetDSItem(strRes);
// 		if(!pItem->m_pDSItem) continue;
// 		pItem->m_strFormat = GenerateStringFormat(pItem->m_pDSItem->m_wDataType, pItem->m_lLength); // 입력 데이터 포맷
// 		pItem->m_wType = pItem->m_pDSItem->m_wDataType;		
// 	}
// }
////////////////////////////////////////////////
// ocx map 에서 추출한 유효한 아이템 삭제
void CMsgForChart::RemoveAll(void)
{
	POSITION pos = m_pItems->GetHeadPosition();
	while(pos)
	{
		CItemInChartEu * pItem = (CItemInChartEu*)m_pItems->GetNext(pos);
		delete pItem;
		pItem = NULL;
	}
	m_pItems->RemoveAll();
}

/////////////////////////////////////////////////////////////////////
// OCX에 전달할 전문의 형식을 맞추기 위해 
// data 의 형식은 DSItem에 서 가져오고
// data의 길이는 ocx내에서 설정한 map에서 가져온다.
// 반환값: 형식 스트링 예) %10ld
//
CString CMsgForChart::GenerateStringFormat(WORD wDataType, long lLen)
{
	CString ret;

	static const char * pszTypeString[] =
	{"", "s", "c", "hd", "hu", "ld", "lu", "f", "lf", "I64d"	};

	ret.Format("%%%ld%s",lLen, pszTypeString[wDataType]);

	return ret;
}

// 보조 메시지 항목 SET - 입력 value
void CMsgForChart::SetSubMsg(CString SubMsgItem, CString value)
{
	if(SubMsgItem == _T("RESETPACKET") || SubMsgItem == _T("NEWPACKET"))
	{
		CItemInChartEu* pItem = NULL;
		int iStart = 0;
		//value += "|";
		CString strPacketName = TokenizeEx(value,":", iStart);	// get packet name 
		POSITION pos = m_pItems->GetHeadPosition();
		while (pos)
		{
			pItem = (CItemInChartEu*)m_pItems->GetNext(pos);
			//if(pItem->m_strName == strPacketName)
			{
				//pItem->m_lLengthBackup = pItem->m_lLength;
				pItem->m_lLength = atol(TokenizeEx(value,":", iStart));	
				pItem->m_strType = TokenizeEx(value,"|", iStart);

			//	break;
			}
			strPacketName = TokenizeEx(value,":", iStart);	// get packet name 

		}
		//value.Delete(value.GetLength()-1, 1);
	}
	else if(SubMsgItem == _T("RDATET"))
	{
		CItemInChartEu* pItem;
		POSITION pos = m_pItems->GetHeadPosition();
		while (pos)
		{
			pItem = (CItemInChartEu*)m_pItems->GetNext(pos);
			if(pItem->m_strName == "자료일자")
				break;
		}

		pItem->m_strType = value;
		pItem->m_lLength = value.GetLength();
	}
	
	m_strExtMsg = SubMsgItem + "=" + value + "@";	GenerateSubMsg();
}
