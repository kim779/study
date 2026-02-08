#pragma once

//#include <afxtempl.h>
#include "ItemInChart.h"


#define LENGTH_MSG_COUNT	5
#define LENGTH_MSG_SUBMSG_COUNT	4

typedef struct _tagPACKMAP
{
	char* szPacket;
	long lFID;
	BOOL bSub;
}PacketMap;

typedef struct _tagPACKSUBMAP
{
	char* szType;
	long lFID;
}PackSubMap;


static const char * pszTypeSubMsg[] =
{
  "UM"				// 0
, "UD"
, "UT"
, "RDATET"
, "PREVPRICE"
, "FUNDSTARTPOINT"
, "FUNDENDPOINT"
, "USEPACKET"
, "BOUND"
, "RESETUSEREAL"	
, "RESETPACKET"		//10
, "ERROR"
, "PRICETYPE"
, "NEWPACKET"
, "DRAWSTARTINDEX"
, "CODEANDNAME"
, "DOAPPENDBYREAL"
, "DATE"
, "MARKETTIME"
, "FRDA"
, "UTEC"			// 20
, "CB"
, "RDC"
, "BOUNDMARK"
};

class CMsgForChart
{
public:
	CMsgForChart();
	~CMsgForChart(void);
	UINT m_LengthOfSubMsg;	// 보조 메시지의 길이
	CString m_strLenOfSubMsg;	// 보조 메시지의 길이
	CString m_strData;	//데이터 (바디)
	CString m_strSubMsg;	// 보조 메시지 스트링
	// 보조 메시지 항목
// 	CString m_strUM;				// 1
// 	CString m_strUD;
// 	CString m_strUT;
// 	CString m_strRDATET;
// 	CString m_strPREVPRICE;
// 	CString m_strFUNDSTARTPOINT;
// 	CString m_strFUNDENDPOINT;
// 	CString m_strUSEPACKET;
// 	CString m_strBOUND;
// 	CString m_strRESETUSEREAL;		// 10
// 	CString m_strRESETPACKET;
// 	CString m_strERROR;
// 	CString m_strPRICETYPE;
// 	CString m_strNEWPACKET;
// 	CString m_strDRAWSTARTINDEX;
// 	CString m_strCODEANDNAME;
// 	CString m_strDOAPPENDBYREAL;
// 	CString m_strDATE;
// 	CString m_strMARKETTIME;
// 	CString m_strFRDA;				// 20
// 	CString m_strUTEC;
// 	CString m_strCB;
// 	CString m_strRDC;
// 	CString m_strBOUNDMARK;
	CString m_strExtMsg;

public:
	int m_nMultiGID;
	CString m_strGIDs;		// GID list, 구분자 ;
	CString m_strMsg;		// OCX에 전달할 전문: 데이터개수+보조메시지 길이+보조메시지+데이터
	CString m_strCount;		// 데이터 개수
	CString m_strPropChart;	// 차트의 프로퍼티 중 packet관련
	CObList* m_pItems;		// 해당 DSItem pointer

	//////////////////////////////////////////////////////////////////////////
	//	숫자를 스트링으로
	//	23 --> 00023 형태로 변경
	void NumToString(long lNum/*IN, 숫자*/, UINT ulLen/*IN, 자릿수*/, CString& strNum/*OUT, 스트링*/)
	{
		CString strFormat;
		strFormat.Format("%%0%dld",ulLen);
		strNum.Format(strFormat,lNum);
	}
	//////////////////////////////////////////////////////////////////////////
	long StringToNum(UINT uLen/*IN*/, CString strNum/*IN*/){return atol(strNum);}	// 스트링을 넘버로
	void ClearData(void){m_strData.Empty();	}							// 메시지 스트링을 초기화 ""
	void SetCount(long lNum){NumToString(lNum, LENGTH_MSG_COUNT, m_strCount);}	// 데이터 개수 set
	long GetCount(void){return StringToNum(LENGTH_MSG_COUNT, m_strCount);}		// 데이터 개수 get

	void AppendData(char* szFormat/*IN*/, ...);				// N/A
	void GenerateMsg(void);									// 전문(메시지) 생성
	void GenerateSubMsg(void);								// 보조 메시지 생성

	CString GenerateDataFormat();							// OCX의 packet 정보 구조체에 저장
	//CString GenerateDataFormat(CDSArray* pDSItems);
	bool IsValidData(CString strItem/*IN*/);				// packet이 유효한 지 판단 (OCX에서는 checkbox)
	CString GetItemName(CString strItem/*IN*/);				// packet 명 가져오기
	CString GetFieldName(CString strItem/*IN*/);			// 필드 명 가져오기
	long GetLengthOfItem(CString strItem/*IN*/);			// packet의 길이(자릿수)
	CString GetTypeOfItem(CString strItem/*IN*/);			// packet의 종류
	CString GetRealFlag(CString strItem/*IN*/);				// packet의 실시간 여부
	void RemoveAll(void);							 
	CString GenerateStringFormat(WORD wDataType/*IN, from DSItem*/, long lLen/*IN, from packet*/);
	void AddData(CString strData/*IN*/){m_strData += strData;}										//데이터 추가 
//	void mappingDSItem(CString strDSItems/*IN*/, CDSArray* pDSItems/*IN*/);							//packet과 DSItem연결
	CString TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN,OUT*/);	// 스트링 tokenizing

	// 보조 메시지 항목 SET
	void SetSubMsg(CString SubMsgItem, CString value);
// 	void SetUM(CString in);
// 	void SetUD(CString in);
// 	void SetUT(CString in);
// 	void SetRDATET(CString in);
// 	void SetPREVPRICE(CString in);
// 	void SetFUNDSTARTPOINT(CString in);
// 	void SetFUNDENDPOINT(CString in);
// 	void SetUSEPACKET(CString in);
// 	void SetBOUND(CString in);
// 	void SetRESETUSEREAL(CString in);
// 	void SetRESETPACKET(CString in);
// 	void SetERROR(CString in);
// 	void SetPRICETYPE(CString in);
// 	void SetNEWPACKET(CString in);
// 	void SetDRAWSTARTINDEX(CString in);
// 	void SetCODEANDNAME(CString in);
// 	void SetDOAPPENDBYREAL(CString in);
// 	void SetDATE(CString in);
// 	void SetMARKETTIME(CString in);
// 	void SetFRDA(CString in);
// 	void SetUTEC(CString in);
// 	void SetCB(CString in);
// 	void SetRDC(CString in);
// 	void SetBOUNDMARK(CString in);
// 	// 보조 메시지 GET
// 	CString GetUM(void);
// 	CString GetUD(void);
// 	CString GetUT(void);
// 	CString GetRDATET(void);
// 	CString GetPREVPRICE(void);
// 	CString GetFUNDSTARTPOINT(void);
// 	CString GetFUNDENDPOINT(void);
// 	CString GetUSEPACKET(void);
// 	CString GetBOUND(void);
// 	CString GetRESETUSEREAL(void);
// 	CString GetRESETPACKET(void);
// 	CString GetERROR(void);
// 	CString GetPRICETYPE(void);
// 	CString GetNEWPACKET(void);
// 	CString GetDRAWSTARTINDEX(void);
// 	CString GetCODEANDNAME(void);
// 	CString GetDOAPPENDBYREAL(void);
// 	CString SetDATE(void);
// 	CString GetMARKETTIME(void);
// 	CString GetFRDA(void);
// 	CString GetUTEC(void);
// 	CString GetCB(void);
// 	CString GetRDC(void);
// 	CString GetBOUNDMARK(void);
};

