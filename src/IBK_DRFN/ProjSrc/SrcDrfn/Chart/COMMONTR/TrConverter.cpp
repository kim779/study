/* 
if(m_strTRCode == QUERY_STRSTOCK_COMP)		
		return GetQueryData03090(bNext);
	else if(m_strTRCode == QUERY_STRUPJONG_COMP)
		return GetQueryData03092(bNext);	
	else if(m_strTRCode == QUERY_STRMARKET)
		return GetQueryData90001(bNext);		
	else if(m_strTRCode == QUERY_STRFUTOPT_COMP)
		return GetQueryData03091(bNext);
	else if(m_strTRCode == QUERY_STRFOREIGN)
*/
#include "stdafx.h"
#include "./TrConverter.h"
#include "../comparechart/define/ChartMsgDef.h"
#include "../CommonTR/EU_p0602.h"	//주식
#include "../CommonTR/EU_p0603.h"	//업종
#include "../CommonTR/EU_p0604.h"	//선옵
#include "../../inc/IDataUtilMng.h"
// --------------------------------------------------------------------------------------
//			주식종목 조회
//	QUERY_STRSTOCK_COMP
//	S03090_I
//	CString CChartItemData::GetQueryData03090(BOOL bNext)
// --------------------------------------------------------------------------------------
int CTrConverter::GetSize_S03090_P0602()
{
	int nSize = 80 + // 80Byte
				sizeof(EU_p0602_InRec1) + // TR데이타
				27 + 1 + //레코드 1 Size
				15 + 1;	//레코드 2 Size
	return nSize;
}

int CTrConverter::MakeTrDataIn_S03090_P0602(BYTE *pTrData, char* szSaveBuf)
{
	// S03090_I와 EU_p0602_InRec1의 구조가 동일하므로 동일하게 처리한다.
	//S03090_I* pOrgData = (S03090_I*)pTrData;
	//EU_p0602_InRec1* pInBlock = (EU_p0602_InRec1*)szSaveBuf;
	//memset(pInBlock, 0x20, sizeof(EU_p0602_InRec1));

	EU_p0602_InRec1* pInBlock = (EU_p0602_InRec1*)pTrData;
	memcpy(szSaveBuf, pInBlock, sizeof(EU_p0602_InRec1));

	return sizeof(EU_p0602_InRec1);
}

void CTrConverter::MakeFidDataIn_S03090_P0602(BYTE *pTrData, char* szSaveBuf, int &nPos, int nLineType)
{
	//1. 연속패킷, DRFN 연속키값 쓸필요있나???, 그련 space(80)
	memset(&szSaveBuf[nPos], 0x20, 80);
	nPos += 80;

	//2. TR값 문자열로 만든다.
	int nTrDataSize = CTrConverter::MakeTrDataIn_S03090_P0602((BYTE*)pTrData, &szSaveBuf[nPos]);
	nPos += nTrDataSize;

	//3. 레코드 번호 1부터 차례로 
	CString szOutRecord;

	// 1st Record
	szOutRecord.Format("%-16d", 1);
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	IDataUtilManager2* pDataUtilMng = (IDataUtilManager2*)AfxGetPctrInterface(UUID_IDataUtilManager);
	pDataUtilMng->GetFillString(szOutRecord, 0, 10, 3);
	szOutRecord += "012013";
	//szOutRecord = "000002003004005009010012013";
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	memcpy(&szSaveBuf[nPos], "|", 1);	//구분자.
	nPos++;

	// 2st Record
	szOutRecord.Format("%-16d", 2);
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	if(nLineType==0)	szOutRecord = "000001";
	else				szOutRecord = "000001002003004";
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	memcpy(&szSaveBuf[nPos], ";", 1);	//구분자.
	nPos++;
}

// --------------------------------------------------------------------------------------
//			업종 조회
//	QUERY_STRUPJONG_COMP, p0603
//	D03092_I
//	CString CChartItemData::GetQueryData03092(BOOL bNext)
// --------------------------------------------------------------------------------------
int CTrConverter::GetSize_D03092_P0603()
{
	int nSize = 80 + // 80Byte
				sizeof(EU_p0603_InRec1) + // TR데이타
				27 + 1 + //레코드 1 Size
				15 + 1;	//레코드 2 Size
	return nSize;
}

int CTrConverter::MakeTrDataIn_D03092_P0603(BYTE *pTrData, char* szSaveBuf)
{
	// D03092_I와 EU_p0602_InRec1의 구조가 동일하므로 동일하게 처리한다.

	EU_p0603_InRec1* pInBlock = (EU_p0603_InRec1*)pTrData;
	memcpy(szSaveBuf, pInBlock, sizeof(EU_p0603_InRec1));

	return sizeof(EU_p0603_InRec1);
}

// QUERY_STRSTOCK_COMP
void CTrConverter::MakeFidDataIn_D03092_P0603(BYTE *pTrData, char* szSaveBuf, int &nPos, int nLineType)
{
	//1. 연속패킷, DRFN 연속키값 쓸필요있나???, 그련 space(80)
	memset(&szSaveBuf[nPos], 0x20, 80);
	nPos += 80;

	//2. TR값 문자열로 만든다.
	int nTrDataSize = CTrConverter::MakeTrDataIn_D03092_P0603((BYTE*)pTrData, &szSaveBuf[nPos]);
	nPos += nTrDataSize;

	//3. 레코드 번호 1부터 차례로 
	CString szOutRecord;

	// 1st Record
	szOutRecord.Format("%-16d", 1);
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	IDataUtilManager2* pDataUtilMng = (IDataUtilManager2*)AfxGetPctrInterface(UUID_IDataUtilManager);
	pDataUtilMng->GetFillString(szOutRecord, 0, 10, 3);
	szOutRecord += "012013";
	//szOutRecord = "000002003004005009010012013";
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	memcpy(&szSaveBuf[nPos], "|", 1);	//구분자.
	nPos++;

	// 2st Record
	szOutRecord.Format("%-16d", 2);
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	if(nLineType==0)	szOutRecord = "000001";
	else				szOutRecord = "000001002003004";
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	memcpy(&szSaveBuf[nPos], ";", 1);	//구분자.
	nPos++;
}

// --------------------------------------------------------------------------------------
//			선옵 조회
//	QUERY_STRFUTOPT_COMP, p0604
//	D03091_I
//	CChartItemData::GetQueryData03091
// --------------------------------------------------------------------------------------
int CTrConverter::GetSize_D90001_P0604()
{
	int nSize = 80 + // 80Byte
				sizeof(EU_p0604_InRec1) + // TR데이타
				27 + 1 + //레코드 1 Size
				15 + 1;	//레코드 2 Size
	return nSize;
}

int CTrConverter::MakeTrDataIn_D90001_P0604(BYTE *pTrData, char* szSaveBuf)
{
	// D03091_I과 EU_p0604_InRec1의 구조가 동일하므로 동일하게 처리한다.

	EU_p0604_InRec1* pInBlock = (EU_p0604_InRec1*)pTrData;
	memcpy(szSaveBuf, pInBlock, sizeof(EU_p0604_InRec1));

	return sizeof(EU_p0604_InRec1);
}

// QUERY_STRSTOCK_COMP
void CTrConverter::MakeFidDataIn_D90001_P0604(BYTE *pTrData, char* szSaveBuf, int &nPos, int nLineType)
{
	//1. 연속패킷, DRFN 연속키값 쓸필요있나???, 그련 space(80)
	memset(&szSaveBuf[nPos], 0x20, 80);
	nPos += 80;

	//2. TR값 문자열로 만든다.
	int nTrDataSize = CTrConverter::MakeTrDataIn_D90001_P0604((BYTE*)pTrData, &szSaveBuf[nPos]);
	nPos += nTrDataSize;

	//3. 레코드 번호 1부터 차례로 
	CString szOutRecord;

	// 1st Record
	szOutRecord.Format("%-16d", 1);
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	IDataUtilManager2* pDataUtilMng = (IDataUtilManager2*)AfxGetPctrInterface(UUID_IDataUtilManager);
	pDataUtilMng->GetFillString(szOutRecord, 0, 10, 3);
	szOutRecord += "012013";
	//szOutRecord = "000002003004005009010012013";
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	memcpy(&szSaveBuf[nPos], "|", 1);	//구분자.
	nPos++;

	// 2st Record
	szOutRecord.Format("%-16d", 2);
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	if(nLineType==0)	szOutRecord = "000001";
	else				szOutRecord = "000001002003004";
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	memcpy(&szSaveBuf[nPos], ";", 1);	//구분자.
	nPos++;
}

#include "../../inc/IAUTrCommMng.h"
CString CTrConverter::GetCodeName(LPVOID	aTRData)
{
	STDR_FID* pStdrFid_Rec1 = (STDR_FID*)aTRData;
	STDR_FID* pStdrFid_Rec2 = pStdrFid_Rec1 + 1;
	
	CString strOut;
	// 종목명
	strOut += pStdrFid_Rec1->vvRpValueList[1][0];
	return strOut;
}
