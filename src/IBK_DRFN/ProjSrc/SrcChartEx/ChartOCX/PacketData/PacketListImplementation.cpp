// PacketListImplementation.cpp: implementation of the CPacketListImplementation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PacketListImplementation.h"

#include <assert.h>
#include <math.h>

#include "../Include_Chart/Dll_Load/IMetaTable.h"				// for _MTEXT()
#include "../Include_Chart/IChartCtrl.h"						// for IChartCtrl
#include "../../../SrcDrfn/inc/commonDefines.h" // 0024696: 파생상품 코드체계 변경 제도개선

#include "Conversion.h"
#include "DataMath.h"
#include "PacketBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//한투용일경우만 이부분을 풀어준다(사용한다).
//#define _HANTU_VERSION_
//#define _AITAS_VERSION_
#define _HANHA_VERSION_

#define _REMOVE_PACKET_COUNT	500		//50

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPacketListImplementation::CPacketListImplementation(CPacketList* pPacketList, const CPacketType::REALTYPE eRealType, IChartCtrl *p_pIChartCtrl) :
		m_pPacketList(pPacketList),
		m_realCenter( eRealType, p_pIChartCtrl, &m_PacketList)
{
	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = p_pIChartCtrl->GetSafeHwnd();

	m_nDataInsertType = CPacketDataInfo::NEW;
	m_nPriceType = CPacketType::ORIGINAL;

	m_dCurrentDate = 0.0;
	m_nHoldingPacketSize = 2048;
	m_nPacketLengthCount = 5;
	m_nMaxDataCount = 0;

	m_dVersion = 1.0;

	// 자신의 RQ : 복수종목 - ojtaso (20070117)
	m_lpszRQ = NULL;

	// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리
	m_nBaseTimeDiff = 0;
	m_nOrgBaseTimeDiff = 0;

	m_pDRDebuger = NULL;
}

CPacketListImplementation::CPacketListImplementation(CPacketList* pPacketList, 
			CString strPacketOption, const CPacketType::REALTYPE eRealType, IChartCtrl *p_pIChartCtrl):
		m_pPacketList(pPacketList),
		m_realCenter(eRealType, p_pIChartCtrl, &m_PacketList)
{
	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = p_pIChartCtrl->GetSafeHwnd();

	m_nDataInsertType = CPacketDataInfo::NEW ;
	m_nPriceType = CPacketType::ORIGINAL;

	m_dCurrentDate = 0.0;
	m_nHoldingPacketSize = 2048;
	m_nPacketLengthCount = 5;
	m_nMaxDataCount = 0;

	m_dVersion = 1.0;

	// (2008/2/4 - Seung-Won, Bae) for Other Meta of MultiLanguage
	if(!strPacketOption.IsEmpty()) m_strPacketOption = AddPacket(strPacketOption) ;

	// (2004.09.14, 배승원) 보조 Message Item을 범용적으로 관리하여 조회 이용할 수 있도록한다.
	m_mapHelpMessage.InitHashTable(50, TRUE);

	// 자신의 RQ : 복수종목 - ojtaso (20070117)
	m_lpszRQ = NULL;

	// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리
	m_nBaseTimeDiff = 0;
	m_nOrgBaseTimeDiff = 0;

	m_pDRDebuger = NULL;
}

CPacketListImplementation::~CPacketListImplementation()
{
	DeletePacketList(&m_PacketList);
	DeletePacketList(&m_IndicatorDataList);
	DeletePacketList(&m_AdditionList);
	if(m_lstRealKeyName.GetCount() > 0)
		m_lstRealKeyName.RemoveAll();

	if(m_pDRDebuger) delete m_pDRDebuger;	m_pDRDebuger = NULL;
}

int CPacketListImplementation::SetBaseData(long pData, int nCount, int nType)
{
	char szbozo[5];
	int nbozolen = 0 ;
	char *szCount = new char[m_nPacketLengthCount+1];
	memcpy(szCount, (void*)pData, m_nPacketLengthCount);
	szCount[m_nPacketLengthCount] = NULL;
	int nRowCount = atoi(szCount);
	delete szCount;

	//	보조 메세지 길이를 읽는다
	/*if(nType == -1)	{	// 보조메세지 길이 byte형 2자리 old version
		memcpy(szbozo, (void*)(pData+4), 1);
		szbozo[2] = NULL ;
		nbozolen = (int) ((unsigned char) szbozo[0]) * 256 ;
		memcpy(szbozo, (void*)(pData+5), 1);
		szbozo[2] = NULL ;
		nbozolen += (int) ((unsigned char) szbozo[0]);
		pData += 6 ;
	}*/
	if(nType == -1)	{	// 보조메세지 길이 byte형 2자리 old version
		memcpy(szbozo, (void*)(pData+m_nPacketLengthCount), 2) ;
		szbozo[2] = NULL ;
		nbozolen = atoi(szbozo) ;
		pData += m_nPacketLengthCount + 2;
	}
	else	{	// 보조메세지 길이 int형 4자리 new version
		memcpy(szbozo, (void*)(pData+m_nPacketLengthCount), 4) ;
		szbozo[4] = NULL ;
		nbozolen = atoi(szbozo) ;
		pData += m_nPacketLengthCount + 4;
	}

	//	보조메세지를 저장한다
	char * szBozomsg = new char[nbozolen + 1] ;
	memcpy(szBozomsg, (void*)(pData), nbozolen) ;
	szBozomsg[nbozolen] = NULL ;
	CString strbozo = szBozomsg ;
	delete[] szBozomsg ;

	//	보조메세지 세팅
	int retInfo = SetHelpMessage(strbozo, true);
	if(retInfo == -1){
		return -1;
	}
	else if(retInfo == 2){
		return SetBaseData(pData, nbozolen, nRowCount, false, m_AdditionList);
	}

	return SetBaseData(pData, nbozolen, nRowCount, true, m_PacketList);
}

int CPacketListImplementation::SetBaseData(long pData, const int nbozolen, const int nRowCount, const bool bIsBasePacket, CTypedPtrList<CObList, CPacket*>& packetList)
{
	Initialize_BaseDataBefore(bIsBasePacket);
	int nTotLength = SetBaseData(pData+nbozolen, nRowCount, &packetList);
	Initialize_BaseDataAfter(bIsBasePacket, nRowCount, &packetList);
	
	return (nTotLength + m_nPacketLengthCount + 4 + nbozolen);
}

int CPacketListImplementation::SetBaseData(CString szData, int nCount, int nType)
{
	CString strData = szData ;
	char * pData = new char[strData.GetLength()];
	memcpy(pData, strData, strData.GetLength());
	int nRet = SetBaseData((long)pData, nCount, nType) ;
	delete[] pData ;
	return nRet;
}

int CPacketListImplementation::GetPacketDataLength(int nIndex)
{
	return GetPacketDataLength(nIndex, &m_PacketList);
}

//sy 2005.08.05.
CPacket* CPacketListImplementation::GetBaseData(const CString& strPacketName, const bool bCheckCurrentPacket)
{
	CPacket* pPacket = GetBaseDataInCurrentInCompart(strPacketName, ",");
	if(pPacket != NULL || bCheckCurrentPacket)
		return pPacket;

	return GetBaseDataInNew(strPacketName);
}

bool CPacketListImplementation::GetPacketList(const CString& strPacketNames, const CString& strCompart, CList<CPacket*, CPacket*>& packetList) const
{
	bool bResult = false;
	CString strTemp = strPacketNames;
	if(strTemp.Right(strCompart.GetLength()) != strCompart)
		strTemp += strCompart;
	
	while(!strTemp.IsEmpty())
	{
		CString strPacketName = CDataConversion::GetStringData(strTemp, strCompart);
		CPacket* pPacket = GetBaseDataInCurrent(strPacketName);
		if(pPacket == NULL)
			continue;

		packetList.AddTail(pPacket);
		bResult = true;
	}

	return bResult;
}

bool CPacketListImplementation::GetPacketList(const CString& strPacketNames, const CString& strCompart, const bool bIsOnlyBaseTRData, CTypedPtrList<CObList, CPacket*>& packetList) const
{
	packetList.RemoveAll();
	CString strTemp = strPacketNames;
	if(strTemp.Right(strCompart.GetLength()) != strCompart)
		strTemp += strCompart;
	
	while(!strTemp.IsEmpty())
	{
		CString strPacketName = CDataConversion::GetStringData(strTemp, strCompart);
		CPacket* pPacket = GetPacketInTRData(strPacketName, bIsOnlyBaseTRData);
		if(pPacket == NULL)
			continue;

		packetList.AddTail(pPacket);
	}

	return (packetList.GetCount() > 0);
}

CPacket* CPacketListImplementation::GetBaseDataInCurrentInCompart(const CString& strPacketName, const CString& strCompart) const
{
	CString strTemp = strPacketName;
	CString strName = CDataConversion::GetStringData(strTemp, strCompart);
	if(strName.IsEmpty() && !strTemp.IsEmpty())
		strName = strTemp;

	return GetBaseDataInCurrent(strName);
}

CPacket* CPacketListImplementation::GetBaseDataInCurrent(const CString& strPacketName, const bool bIsOnlyTRData) const
{
	if(strPacketName.IsEmpty())
		return NULL;

	// TR Data
	CPacket* pPacket = GetPacket(strPacketName, m_PacketList);
	if(pPacket != NULL)
		return pPacket;
	//<< 20100324_JS.Kim 솔로몬.FX	시고저종 외의 패킷을 시고저종처럼 사용하기 위한 처리
	//						Redefine된 PacketName으로 Packet을 찾는다
	pPacket = GetRedefinedPacket(strPacketName, m_PacketList);
	if(pPacket != NULL)
		return pPacket;
	//>>


	// 지표 Data
	if(!bIsOnlyTRData)
	{
		pPacket = GetPacket(strPacketName, m_IndicatorDataList);
		if(pPacket != NULL)
			return pPacket;
	}

	// 추가 Data
	return GetPacket(strPacketName, m_AdditionList);
}

CPacket* CPacketListImplementation::GetPacketInTRData(const CString& strPacketName, const bool bIsOnlyBaseTRData) const
{
	if(strPacketName.IsEmpty())
		return NULL;

	// TR Data
	CPacket* pPacket = GetPacket(strPacketName, m_PacketList);
	if(pPacket != NULL)
		return pPacket;

	if(bIsOnlyBaseTRData)
		return NULL;

	// 추가 Data
	return GetPacket(strPacketName, m_AdditionList);
}

CPacket* CPacketListImplementation::GetPacket(const CString& strPacketName, const CTypedPtrList<CObList, CPacket*>& packetList) const
{
	if(strPacketName.IsEmpty())
		return NULL;

	POSITION pos = packetList.GetHeadPosition();
	while(pos != NULL)
	{
		CPacket* pPacket = (CPacket*) packetList.GetNext(pos);
		if(pPacket->GetName() == strPacketName)
			return pPacket;
		//<<20100323_JS.Kim 솔로몬.FX 맵차트콘트롤에서 매도/매수를 동시에 그리기 위해 종가 데이타가 매수종가/매도종가 2가지가 있다.
		if( CChartInfo::FX == m_realCenter.GetChartMode())
		{
			if(pPacket->GetSubName() == strPacketName)
				return pPacket;
		}
		//>>
	}

	return NULL;
}

CPacket* CPacketListImplementation::GetBaseDataInNew(const CString& strPacketName)
{
	CString strTemp = strPacketName;
	strTemp.TrimLeft();
	strTemp.TrimRight();
	CString strName = CDataConversion::GetStringData(strTemp, ",");
	if(strName.IsEmpty())
		strName = strTemp;
	if(strName.IsEmpty())
		return NULL;

	CPacket* pPacket = NULL;
	if(!strName.CompareNoCase( _MTEXT( C3_MARKET_PROFILE)))
	{
		pPacket = new CMPPacket(m_pPacketList, strName, "", "x 1", "x 1", 0, false, 0);
	}
	else
	{
		pPacket = new CPacket(m_pPacketList, strName, "", "x 1", "x 1", 0, false, 0);
		pPacket->SetYesOrNoBaseData(false);
	}

	m_IndicatorDataList.AddTail(pPacket);
	return pPacket;
}
//sy end

/*
CPacket * CPacketListImplementation::AddIndicatorData(CString strFuntion, CString strCondition)
{
	CPacket * pPacket = new CPacket(m_pPacketList, strFuntion, strFuntion, "99", "99", 0, false) ;
	CList <double, double> lsData ;
	// call Indicator function into Indication dll
	//	Indicatordll->GetIndicatorList(strFunction, strCondition, &lsData) ;
	pPacket->ChangeData(lsData) ;
	lsData.RemoveAll() ;

	m_IndicatorDataList.AddTail(pPacket) ;

	return pPacket ;
}
*/

//	생성해야 할 기본 데이타 Packet항목을 받아서 Packet List를 생성한다
CString CPacketListImplementation::AddPacket(CString strPacketOption)
{
	CString strNewPacketOption;
	CString strTemp = strPacketOption;
	while(!strTemp.IsEmpty())
	{
		CString strP = CDataConversion::GetStringData(strTemp, "\r\n") ;	
		if(strP.IsEmpty() && !strTemp.IsEmpty())
		{
			strP = strTemp;
			strTemp.Empty();
		}

		CString strU = CDataConversion::GetStringData(strP, ";") ;
		CString strName = _STOM0( CDataConversion::GetStringData(strP, ";"));
		CString strFieldName = CDataConversion::GetStringData(strP, ";") ;
		CString	strLength = CDataConversion::GetStringData(strP, ";");
		int nLength = atoi( strLength) ;
		CString strType = _MTOM6( CDataConversion::GetStringData(strP, ";")) ;
		strType.Replace( "×", "x");
		CString strReal = _MTOM6( CDataConversion::GetStringData(strP, ";")) ;
		bool bUseReal = ( strReal == _MTEXT( C6_NO)) ? false : true;

		if(strU == "1" && 
		   strName != "Packet" && strName != _MTEXT( C6_USER_DEFINE) && strName != _MTEXT( C6_INPUT_ITEM))	{
			CPacket * pPacket = new CPacket(m_pPacketList, strName, strFieldName, strType, strType, nLength, bUseReal) ;
			m_PacketList.AddTail(pPacket) ;
		}

		strNewPacketOption += strU + ";" + strName + ";" + strFieldName + ";" + strLength + ";" + strType + ";" + strReal + ";\r\n";
	}

	m_realCenter.SetRealBasePacket(GetBaseData( _MTEXT( C0_DATE_TIME)));
	return strNewPacketOption;
}

//	여러 Packet에서 Minimun & Maximun Value를 Join한다
//	nStart : 시작데이타 위치
//	nEnd : 끝데이타 위치
//	strPacketNameList : Join할 Packet의 List(packet1,packet2,Packet3,...)
//	dMin : Minimum
//	dMax : Maximun
int CPacketListImplementation::GetMinMax(int nStart, int nEnd, CString strPacketNameList, double& nMin, double& nMax)
{
	if(strPacketNameList.IsEmpty())
		return -1 ;
	
	CString strTemp = strPacketNameList ;
	int nCount = CDataConversion::GetStringDataCount(strTemp, ",") ;
	if(strTemp.ReverseFind(',') != 0)
		nCount++;

	if(nCount <= 0)
		return -1 ;

	double nMi, nMa ;
	bool bres = true ;
	for(int i = 0 ; i < nCount ; i++)	{
		CString strName = CDataConversion::GetStringData(strTemp, ",") ;
		if(strName.IsEmpty() && strTemp.Find(",") < 0)
			strName = strTemp ;
		if(strName.IsEmpty())
			break ;
		CPacket * pPacket = GetBaseData(strName) ;
		double nMi1 = 0, nMa1 = 0 ;
		if(pPacket != NULL)	{
			int nres = pPacket->GetMinMax(nStart, nEnd, nMi1, nMa1) ;
			if(nres < 0)
				bres = false ;
			if(i == 0)	{
				nMi = nMi1 ;
				nMa = nMa1 ;
			}
			else	{
				nMi = nMi < nMi1 ? nMi : nMi1 ;
				nMa = nMa > nMa1 ? nMa : nMa1 ;
			}
		}
	}

	nMin = nMi ;
	nMax = nMa ;

	if(bres == false)
		return -1 ;
	else
		return 1 ;
}

bool CPacketListImplementation::GetAllPacketsMinMax(const int nStart, const int nEnd, double& dMin, double& dMax)
{
	// "자료일자"는 제외하고 구한다.
	bool bResult = false;
	if(GetAllPacketsMinMax(m_PacketList, nStart, nEnd, dMin, dMax))
		bResult = true;

	if(GetAllPacketsMinMax(m_IndicatorDataList, nStart, nEnd, dMin, dMax))
		bResult = true;

	if(GetAllPacketsMinMax(m_AdditionList, nStart, nEnd, dMin, dMax))
		bResult = true;

	return bResult;
}

//  bClear가 false면 지표데이터만 삭제
//  true면 기본데이터하고 지표데이터 모두 삭제
int CPacketListImplementation::ClearDataAll(const bool bClearIncludingTR)
{
	POSITION pos = m_PacketList.GetHeadPosition() ;
	while(pos)	{
		CPacket * pPacket = m_PacketList.GetNext(pos) ;
		if(pPacket)
			pPacket->ClearData(bClearIncludingTR) ;
	}

	pos = m_IndicatorDataList.GetHeadPosition() ;
	while(pos)	{
		CPacket * pPacket = m_IndicatorDataList.GetNext(pos) ;
		if(pPacket)
			pPacket->ClearData() ;
	}

	pos = m_AdditionList.GetHeadPosition();
	while(pos)	{
		CPacket * pPacket = m_AdditionList.GetNext(pos);
		if(pPacket)
			pPacket->ClearData(bClearIncludingTR);
	}

	//sy 2003.3.19. -> 종목코드와 종목명을 clear한다.
	if(bClearIncludingTR)
		m_itemData.Clear();

	return 1 ;
}

// bClearIncludingTR == true : TR data 포함
int CPacketListImplementation::ClearData(const CString& strPacketNames, const CString& strCompartName, const bool bClearIncludingTR)
{
	if(strPacketNames.IsEmpty())
		return -1;

	CString strT(strPacketNames), strPacketName;
	if(strT.Right(strCompartName.GetLength()) != strCompartName)
		strT += strCompartName;

	int nResult = 0;
	while(!strT.IsEmpty())	{
		strPacketName = CDataConversion::GetStringData(strT, strCompartName);
		CPacket * pPacket = GetBaseData(strPacketName);
		if(pPacket == NULL)
			continue;
		
		pPacket->ClearData(bClearIncludingTR);
		nResult = 1;
	}

	return nResult;
}

bool CPacketListImplementation::ClearAllIndicatorData(const bool bOnlyData)
{
	if(m_IndicatorDataList.GetCount() <= 0)
		return false;

	//sy 2003.8.5.
	if(!bOnlyData){
		return DeletePacketList(&m_IndicatorDataList);
	}

	POSITION pos = m_IndicatorDataList.GetHeadPosition();
	while(pos != NULL){
		CPacket* pPacket = m_IndicatorDataList.GetNext(pos);
		if(pPacket != NULL)
			pPacket->ClearData();
	}

	return true;
}

bool CPacketListImplementation::ClearIndicatorData(const CString &strName)
{
	if(strName.IsEmpty())	return false;

	if(strName == "ALL")	{
		POSITION pos = m_IndicatorDataList.GetHeadPosition() ;
		while(pos)	{
			CPacket * pPacket = m_IndicatorDataList.GetNext(pos) ;
			if(pPacket)
				pPacket->ClearData() ;
		}
		return true;
	}
	else	{
		POSITION pos = m_IndicatorDataList.GetHeadPosition();
		while(pos)	{
			CPacket * pPacket = m_IndicatorDataList.GetNext(pos);		
			if(pPacket && pPacket->GetName() == strName)	{
				pPacket->ClearData();
				return true;
			}
		}
		return false;
	}

	return false;
}

CList<double, double> * CPacketListImplementation::GetnumericDataList(CPacket *pPacket)
{
	if(pPacket == NULL)
		return NULL;

	return pPacket->GetnumericDataList() ;
}

CList<CString, CString> * CPacketListImplementation::GetstrDataList(CPacket *pPacket)
{
	if(pPacket == NULL)
		return NULL ;

	return pPacket->GetstrDataList() ;
}

// (2004.06.25, 배승원) Packet 명의 구분자를 전달받게 한다. (기본값은 이전에 고정되었던 ',')
int CPacketListImplementation::GetMaxCount(CString strPacketNameList, const CString& strCompartName)
{
	if(strPacketNameList.IsEmpty())	{
		POSITION pos = m_PacketList.GetHeadPosition() ;
		if(pos == NULL)
			return 0 ;
		else
			return m_PacketList.GetAt(pos)->GetCount() ;
	}
	
	CString strTemp = strPacketNameList ;
	int nCount = CDataConversion::GetStringDataCount(strTemp, strCompartName) ;
	if(strTemp.ReverseFind(',') > 0)
		nCount++;

	if(nCount <= 0)
		return 0 ;

	int nDataCount = 0 ;
	for(int i = 0 ; i < nCount ; i++)	{
		CString strName = CDataConversion::GetStringData(strTemp, strCompartName) ;
		if(strName == "error" && strTemp.Find(strCompartName) < 0)
			strName = strTemp ;
		if(strName == "error")
			break ;
		CPacket * pPacket = GetBaseData(strName) ;
		if(pPacket != NULL)
			nDataCount = nDataCount > pPacket->GetCount() ? nDataCount : pPacket->GetCount() ;
	}

	return nDataCount ;
}

CString CPacketListImplementation::GetPacketType(CPacket *pPacket, const bool bIsFullType)
{
	if(pPacket == NULL)
		return "" ;

	return pPacket->GetType(bIsFullType) ;
}

//sy 2005.08.23.
//보조메시지 (서버에서 받는 항목으로 각 항목별로 받을 수 있다)
int CPacketListImplementation::SetHelpMessage( const char *p_szHelpMessage, const bool bIsAllRemove)
{
	//보조메시지 초기화
	if(bIsAllRemove)
		SetHelpMessageInitialize( p_szHelpMessage);

	CString strMsgData = p_szHelpMessage;
	strMsgData.TrimRight();
	strMsgData.TrimLeft();
	if(strMsgData.IsEmpty())
		return 0;
	if(!strMsgData.IsEmpty() && strMsgData.Right(1) != "@")
		strMsgData += "@";

	//20081021 JS.Kim		일봉 TR의 날자는 영업일 기준이고, 현재날자는 거래소시간 기준이라 현재 날자를 영업일 날자로 변경해준다
	//return SetHelpMessage(strMsgData);
	int ret = SetHelpMessage(strMsgData);
	return ret;
}

void CPacketListImplementation::SetHelpMessageInitialize( const char *p_szHelpMessage)
{
	// "UM" type이 없을 경우는 "E"로 간주한다. (초기화 함)
	if( !strstr( p_szHelpMessage, "UM=")) SetDataInsertTypeInfo("E");
}

// 보조메시지 순서 : UM->UD->UT->기타(기타는 순서 상관없음)
int CPacketListImplementation::SetHelpMessage(const CString& strInfo2)
{
	if(strInfo2.IsEmpty())
		return 0;

	CString strInfo( strInfo2);
	//>>[alzioyes:지우지말것,DRFN Platform 로그기능추가]
	if(!m_pDRDebuger) _GetDRDebuger();
	if(m_pDRDebuger) m_pDRDebuger->DRLog_Message(strInfo2);
	//<<

	// (2009/11/4 - Seung-Won, Bae) for HTS with WorldOn.
	if( strInfo.Find( "MARKETTIME") < 0)
	{
// 		if( CChartInfo::HTS == m_realCenter.GetChartMode())
// 			strInfo += "MARKETTIME=090000|153000@";
// 		// (2009/11/5 - Seung-Won, Bae) for Daily Candle with over night.
// 		if( CChartInfo::FX == m_realCenter.GetChartMode())
// 			strInfo += "MARKETTIME=070000|070000@";
// 			//strInfo += "MARKETTIME=" + m_strFXStartTime + "|" + m_strFXStartTime + "@";
// 		// (2009/12/2 - Seung-Won, Bae) for KRX_CME time
// 		if( CChartInfo::KRX_CME == m_realCenter.GetChartMode())
// 			strInfo += "MARKETTIME=180000|050000@";

		CString rValue;
		if(GetDefaultBozoValue(m_realCenter.GetChartMode(), "MARKETTIME", rValue))
			strInfo += rValue;
	}

	// (2009/11/16 - Seung-Won, Bae) Base Time of N-Minite
	if( strInfo.Find( "BASETIME") < 0)
	{
// 		if( CChartInfo::WORLD_ON == m_realCenter.GetChartMode())
// 			strInfo += "BASETIME=000000@";

		CString rValue;
		if(GetDefaultBozoValue(m_realCenter.GetChartMode(), "BASETIME", rValue))
			strInfo += rValue;
	}

#ifdef _DEBUG
	strInfo = strInfo + "MARKETDAYS=+20091121,-20091120@";
#endif

	//UM=이전/이후, UD=날짜시간타입, UT=실시간간격, RDATET=Reset Date Type
	//UD=0:notype 1:일 2:주 3:월 4:분 5:틱 6:Text 12:초
	//RDORT=0:none 1:date 2=time 3=date+time -> 안쓰기로 함
	//RDATET=YYMMDD 등 쓰일 데이타의 타입 
	//PREVPRICE=전일종가/전누적거래량등을 넣을때 씀		->  패킷명:전일기준가|패킷명:전일기준가@
	//PREVPRICE2=전일 시고저종값을 등록하는 용도로 사용 ->  ST PIVOT 전용
	//BOUND=패킷명:상한:하한선|패킷명:상한:하한선@을 지정한다
	//RESETUSEREAL=실시간사용유무 RESETUSEREAL=1@ or RESETUSEREAL=0@
	//RESETPACKET=패킷명:패킷길이:패킷타입|패킷명:패킷길이:패킷타입@  <- 02.19.2002 추가
	//ERROR=#Error:Error message@
	//PRICETYPE=수정주가 유무  0 = 주가, 1 = 수정주가, 2 = 락, 3 = 뉴스, 계좌
	//NEWPACKET=패킷명:패킷길이:패킷타입|패킷명:패킷길이:패킷타입@
	//DRAWSTARTINDEX=패킷명:데이터시작위치|패킷명:데이터시작위치@
	//CODEANDNAME=종목코드:종목명@
	//DATE=YYYYMMDD@ -> 현재 오늘 날짜를 받는다(복기에서 사용)
	//MARKETTIME=장시작시간|장마감시간@
	//FRDA=0 or 1@ -> ex) 1 : 장전 조회, 첫 리얼 데이터를 추가.거래일어났는지에 대한 유무(First Real Data Append)
	//UTEC=마지막 봉의 리얼갯수@ -> n틱일 경우만 필요함
	//CB=패킷명:장구분|패킷명:장구분@ -> 주식/업종/선물/옵션
	//ER=환율값:반올림자리:환율사용패킷명@  -> ex)환율때 사용 ER=10274:3:종가,시가,고가,저가,@
	//RDC=비율값:반올림자리:비율사용패킷명@  -> ex)선물/옵션일때 사용 RDC=0.01:3:종가,시가,고가,저가,@
	//HBPRICE=대기매물관련 1호가 가격
	//TDC=비율값:비율사용패킷명,|비율값:비율사용패킷명,@
	//EXCEPTIONVALUE=패킷명:값|패킷명:값@
	//CONTINUECODE=패킷명:값|패킷명:값@
	//HOGAUNIT=호가값@
	//MARKETTYPE=거래소코드@  -> 거래소구분 1:CME, 2:CBOT, 3 GX, 4:LIFFE, 5:EUREX, 6:HKFE, 7:NYBOT
	//TIMEDIFF=기준시와의 시차 (시간단위 값 "HH")=> 날자 데이타의 기준시와의 시차 값
	//MARKETDAYS=+YYYYMMDD,-YYYYMMDD@  -> 영업일자 변동사항 (기준은 주 5일)
	//REDEFPACKET=패킷명:New패킷명|패킷명:New패킷명@ -> 시고저종 패킷이 아닌 다른 패킷명으로 사용하는 경우 그 이름을 셋팅하여준다. //20100324_JS.Kim 솔로몬 FX
	char *pKey[] = {"UM", "UD", "UT", "RDATET", "PREVPRICE", 
					"BOUND", "RESETUSEREAL", "RESETPACKET", "ERROR", "PRICETYPE", 
					"NEWPACKET", "DRAWSTARTINDEX", "CODEANDNAME", "DATE", "MARKETTIME", 
					"UTEC", "CB", "ER", "RDC", "EXCEPTIONVALUE",
					"CONTINUECODE", "TDC", "REALTIMET", "BASETIME", "DATATIME", 
					"MARKETSTATUS", "TIMEDIFF", "MARKETDAYS", "REDEFPACKET"};

	CString strT = strInfo;
	bool usenewpacket = false;
	while(!strT.IsEmpty())
	{
		CString szT = CDataConversion::GetStringData(strT, "@") ;		
		CString szU = CDataConversion::GetStringData(szT, "=") ;
			
		if(szU == pKey[0]) // "UM"
		{
			SetDataInsertTypeInfo(szT) ;
		}
		else if(szU == pKey[1]) // "UD"
		{
			SetDateUnitType(szT);
		}
		else if(szU == pKey[2]) // "UT"
		{
			m_realCenter.SetTimeInterval(atoi(szT));
		}
		else if(szU == pKey[3]) // "RDATET"
		{
			ResetTimeType(szT);
		}
		else if(szU == pKey[4]) // "PREVPRICE"
		{
			SetPrevDatas(szT);
		}
		else if(szU == pKey[5]) // "BOUND"
		{
			SetBoundLimit(szT);
		}
		else if(szU == pKey[6]) // "RESETUSEREAL"
		{
			m_bUseRealData = (atoi(szT) > 0) ? true : false;
		}
		// 	02.19.2002  packet resetting
		else if(szU == pKey[7]) // "RESETPACKET"
		{
			ResetPacketInfo(szT);
			PacketClearPart();
		}
		// 02.27.2002  packet error message		
		else if(szU == pKey[8]) // "ERROR"
		{
			if(SetErrorMessage(szT))
				return -1;
		}
		// 03.05.2002  price type
		else if(szU == pKey[9]) // "PRICETYPE"
		{
			m_nPriceType = (CPacketType::PRICETYPE)atoi(szT);
		}
		// 03.08.2002 new packet
		else if(szU == pKey[10]) // "NEWPACKET"
		{
			if(SetNewPacket(szT))
				usenewpacket = true;
		}
		//sy 2003.3.17. 그리는 시작 index
		else if(szU == pKey[11]) // "DRAWSTARTINDEX"
		{
			SetDrawStartIndex(szT);
		}
		//sy 2003.3.19. 종목명과 종목명 받기
		else if(szU == pKey[12]) // "CODEANDNAME"
		{
			SetItemData(szT);
		}
		//sy 2005.11.22. 오늘 날짜 받기(PC 시간을 사용하지 않기 위함)
		else if(szU == pKey[13]) // "DATE"
		{
			m_realCenter.SetCurrentDate(szT);
		}
		//sy 2005.01.24. 장시작/장마감 시간 받기
		else if(szU == pKey[14]) // "MARKETTIME"
		{
			m_realCenter.SetMarketTime(szT);
		}
		//sy 2005.07.28. n틱일 경우 마지막 봉의 틱수.
		else if(szU == pKey[15]) // "UTEC"
		{
			m_realCenter.SetAddTimeInterval(atoi(szT));
		}
		//sy 2005.08.05. packet의 장구분(주식/업종/선물/옵션).
		else if(szU == pKey[16]) // "CB"
		{
			SetCommodityBondName(szT);
		}
		//sy 2005.08.26. 환율 비율 정보.
		else if(szU == pKey[17]) // "ER"
		{
			m_realCenter.SetRealDataModuleAndData( true, szT, true);
		}
		//sy 2005.08.26. 리얼 비율 정보.
		else if(szU == pKey[18]) // "RDC"
		{
			m_realCenter.SetRealDataModuleAndData( true, szT, false);
		}
		else if(szU == pKey[19]) // "EXCEPTIONVALUE"
		{
			SetExceptionValues(szT);
		}
		else if(szU == pKey[20]) // "CONTINUECODE"
		{
			SetContinueCodes(szT);
		}
		// (2008/6/12 - Seung-Won, Bae) Change packet name to meta text
		else if(szU == pKey[21]) // "TDC"
		{
			szT = _STOMS( szT, "-:,|.", IMetaTable::PACKET_NAME);
		}
		// 20080801 김진순 24시간 시세 => 실시간데이타에 날자도 포함시키기 위해 추가
		else if(szU == pKey[22]) // "REALTIMET"
		{
			m_realCenter.SetRealTimeType(szT);
		}
		// 20080801 김진순 24시간 시세 => 분봉처리시 봉의 시작시간으로 설정할 기준시간
		else if(szU == pKey[23]) // "BASETIME"
		{
			m_realCenter.SetNewRangeStartTime(szT);
		}
		// 20080804 김진순 24시간 시세 => TR로 조회한 시간 (일자+시간 ("YYYYMMDDHHMMSS"))
		else if(szU == pKey[24]) // "DATATIME"
		{
#ifdef _DEBUG
//			double dTime2 = atof( szT);
//			tm tmTime;
//			CDataConversion::ConvertDoubletoTM( dTime2, tmTime);
//		tmTime.tm_hour -= 11;
//		tmTime.tm_min -= 20;
//			mktime( &tmTime);
//			CDataConversion::ConvertTMtoDouble( tmTime, dTime2);
//			szT.Format( "%14.0f", dTime2);
#endif
			m_realCenter.SetCurrDateTime(szT);
//			double dTime = atof( szT);
//			CDataConversion::ConvertDoubletoTM( dTime, m_tmNMTRClock);
		}
		// 20080825_JS.Kim 허수봉 처리 => 장중인지 장마감인지 FLAG
		else if(szU == pKey[25]) // "MARKETSTATUS"
		{
// 이용되지 않음.
//			m_realCenter.SetMarketStatus(szT);
		}
		// 20080925_JS.Kim	서로 다른 시간대의 Scale 처리
		else if(szU == pKey[26]) // "TIMEDIFF"
		{
			SetOrgBaseTimeDifference(szT);
			// (2008/10/28 - Seung-Won, Bae) Trace for checking.
			TRACE( "\r\nTIMEDIFF : %s\r\n", szT);
		}
		//MARKETDAYS=+YYYYMMDD,-YYYYMMDD@  -> 영업일자 변동사항 (기준은 주 5일)
		else if(szU == pKey[27]) // "MARKETDAYS"
		{
			m_realCenter.SetMarketDayInfo(szT);
		}
		// 20100324_JS.Kim 솔로몬 FX. 매도/매수시고저종 패킷을 시고저종 패킷으로 정의하여 준다.
		else if(szU == pKey[28]) // "REDEFPACKET"
		{
			SetRedefinePacketName(szT);
		}

		// (2004.09.14, 배승원) 보조 Message Item을 범용적으로 관리하여 조회 이용할 수 있도록한다.
		m_mapHelpMessage.SetAt( szU, szT);
	}
	
	return (usenewpacket ? 2: 1);
}
//sy end

void CPacketListImplementation::SetDataInsertType(CPacketDataInfo::DATAINSERTTYPE nDataInsertType)
{
	m_nDataInsertType = nDataInsertType ;
}

CPacketDataInfo::DATAINSERTTYPE CPacketListImplementation::GetDataInsertType()
{
	return m_nDataInsertType ;
}

int CPacketListImplementation::GetMaxCount()
{
	if(m_PacketList.GetCount() <= 0)	return 0 ;

	POSITION pos = m_PacketList.GetHeadPosition() ;

	int nCount = 0;
	while(pos)	{
		CPacket * pPacket = m_PacketList.GetNext(pos);
		if(pPacket == NULL)
			break;

		int tmp = pPacket->GetCount();
		nCount = nCount > tmp ? nCount : tmp;
	}

	return nCount;
}

int CPacketListImplementation::SetDataInsertTypeInfo(const CString& strData/*, const bool bIsBasePacket*/)
{
	CPacketDataInfo::DATAINSERTTYPE ret = CPacketDataInfo::NEW ;

	//E:new A:Insert B:Append P:Part New
	if(strData == "E")
		ret = CPacketDataInfo::NEW ;
	else if(strData == "A")	//  이전	//Grid : Append  Chart : Insert
		ret = CPacketDataInfo::INSERT ;
	else if(strData == "B")	//	이후	//Grid : Insert	 Chart : Append
		ret = CPacketDataInfo::APPEND ;
	else if(strData == "P")	// 일부분 NEW
		ret = CPacketDataInfo::PART_NEW;
	else
		ret = CPacketDataInfo::NEW ;		//	UM이 없는 경우 무조건 New로 본다

	SetDataInsertType(ret);

	//sy 2005.11.18.
	Initialize_DataInsertType();
	return 1 ;
}

void CPacketListImplementation::SetDateUnitType(const CString& strData)
{
	CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType = (CScaleBaseData::HORZSCALEDRAWERTYPE)(atoi)(strData) ;
	m_realCenter.SetDateUnitType(eDateUnitType);
}

int CPacketListImplementation::GetTimeInterval()
{
	return m_realCenter.GetTimeInterval();
}
__int64 CPacketListImplementation::GetTimeIntervalWithSecond( void)
{
	return m_realCenter.GetTimeIntervalWithSecond();
}

// (2006/7/11 - Seung-Won, Bae) Retrived the Next First Time of Data.
// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
//int CPacketListImplementation::GetStartTimeInTimeRangeOfLastData( void)
__int64 CPacketListImplementation::GetStartTimeInTimeRangeOfLastData( void)
{
	//return ( int)m_realCenter.GetStartTimeInTimeRangeOfLastData();
	return m_realCenter.GetStartTimeInTimeRangeOfLastData();
}
// 2011.01.06 by SYS <<

//sy 2005.11.14.
int CPacketListImplementation::GetTotalTRIndex() const
{
	return m_trDataCountList.GetTotalTRIndex();
}

int CPacketListImplementation::GetTotalTRCount() const
{
	return m_trDataCountList.GetTotalTRCount();
}

int CPacketListImplementation::GetTRCount(const int nTRIndex) const
{
	return m_trDataCountList.GetTRCount(nTRIndex);
}

int CPacketListImplementation::GetRequestTRCount(const int nCurTRCount) const
{
	return m_trDataCountList.GetRequestTRCount(nCurTRCount);
}
//sy end

CScaleBaseData::HORZSCALEDRAWERTYPE CPacketListImplementation::GetDateUnitType()
{
	return m_realCenter.GetDateUnitType();
}

CPoint CPacketListImplementation::GetUpdateRange()
{
	return m_realCenter.GetUpdateRange();
}

void CPacketListImplementation::SetLength(CPacket *pPacket)
{
	if(pPacket == NULL)
		return ;
	double nMin = 0, nMax = 0 ;
	pPacket->GetMinMax(0, pPacket->GetnumericDataList()->GetCount(), nMin, nMax) ;
	pPacket->SetLength(CDataConversion::DoubleToString(nMax).GetLength()) ;
}

int CPacketListImplementation::ResetTimeType(CString strTimeType)
{
	if(strTimeType.IsEmpty())	return 0;

	if(m_PacketList.GetCount() == 0)
		return -1;

	CPacket *pPacket = GetBaseData( _MTEXT( C0_DATE_TIME)) ;

	if(pPacket)	{
		pPacket->SetType(strTimeType);
		return 1;
	}

	return -1;
}


int CPacketListImplementation::GetStartDate()
{
	if(m_PacketList.GetCount() <= 0)
		return -1;

	CPacket * pPacket = GetBaseData( _MTEXT( C0_DATE_TIME));
	if(pPacket && pPacket->GetCount() > 0)
		return (int)pPacket->GetnumericDataList()->GetHead();

	return -1;
}

int CPacketListImplementation::GetEndDate(CPacket* pDatePacket)
{
	if(m_PacketList.GetCount() <= 0)
		return -1;

	CPacket* pPacket = pDatePacket;
	if(pPacket == NULL)
		pPacket = GetBaseData( _MTEXT( C0_DATE_TIME));

	if(pPacket == NULL || pPacket->GetCount() <= 0)
		return -1;
	
	return (int)pPacket->GetnumericDataList()->GetTail();
}

//sy 2003.4.9. 보조메시지 중 전일 값 받는 부분
void CPacketListImplementation::SetPrevDatas(const CString& strData)
{
	CString strPrevDatas = strData;
	while(!strPrevDatas.IsEmpty()){
		CString strPrevData = CDataConversion::GetStringData(strPrevDatas, "|");
		if(strPrevData.IsEmpty() && !strPrevDatas.IsEmpty()){
			strPrevData = strPrevDatas;
			strPrevDatas.Empty();
		}
		SetPrevData(strPrevData);
	}
}

void CPacketListImplementation::SetPrevData(CString& strData)
{
	CString strPacketName = CDataConversion::GetStringData(strData, ":");
	strPacketName = _STOM0( strPacketName);
	CPacket* pPacket = GetBaseData(strPacketName);
	if(pPacket == NULL)
		return ;

	//sy 2003.4.9. 보조메시지로 data gap이 들어온다.
	if( pPacket->IsChangeRealData())	pPacket->SetRealDataGap(	atof(	strData));
	else								pPacket->SetStandardValue(			strData);
}

bool CPacketListImplementation::GetUseReal()
{
	POSITION pos = m_PacketList.GetHeadPosition();
	if(!pos)	return false;

	while(pos)
	{
		if(m_PacketList.GetNext(pos)->GetUseReal())
			return true;
	}

	return false;
}

void WriteDebug(CString szDebug)
{
	SYSTEMTIME sTime;
	GetSystemTime(&sTime);

	FILE *fp = fopen("C:\\Logs\\Chart_Packet_Log.txt", "a+t");
	if(fp != NULL) 
	{
		fprintf(fp, "[%s]\n", szDebug);
		fclose(fp);
	}
}

// ----------------------------------------------------------------------------
void CPacketListImplementation::SetBoundLimit(const CString& strData)
{
	//BOUND=패킷명:상한:하한선|패킷명:상한:하한선@
	CString strBLData = strData;
	strBLData.TrimRight();
	if(strBLData.IsEmpty())
		return;

	while(!strBLData.IsEmpty())
	{
		CString strOne = CDataConversion::GetStringData(strBLData, "|");
		if(strOne.IsEmpty() && !strBLData.IsEmpty())
		{
			strOne = strBLData;
			strBLData.Empty();
		}

		CString strName = CDataConversion::GetStringData(strOne, ":");
		strName = _STOM0( strName);
		CString strUpper = CDataConversion::GetStringData(strOne, ":");
		CString strLower = strOne;
		SetBoundLimit(atof(strUpper), atof(strLower), strName);
	}
}

void CPacketListImplementation::SetBoundLimit(double nUpperBound, double nLowerBound, const CString &strPacketName)
{
	CPacket * pPacket = GetBaseData(strPacketName);
	if(!pPacket)	return ;

	pPacket->SetBoundLimit(nUpperBound, nLowerBound);
}

//bIsFullType =true 면 데이타형
//false면 표현형
CString CPacketListImplementation::GetPacketType(CString strPacketName, const bool bIsFullType)
{
	if(strPacketName.IsEmpty())
		return "";

	CPacket * pPacket = GetBaseData(strPacketName);
	if(pPacket == NULL) 
		return "";

	return pPacket->GetType(bIsFullType);
}

// 03.11.2002  새 패킷을 새로 만든다(추가 패킷이므로 m_AdditionList에 추가된다)
bool CPacketListImplementation::SetNewPacket(const CString& strPacketInfo)
{
	if(strPacketInfo.IsEmpty())
		return false;

	// 새로 데이타를 받을 패킷을 생성하거나 이미 있으면 그 길이를 맞춰준다
	CString szInfo = strPacketInfo;
	while(! szInfo.IsEmpty())
	{
		CString szone = CDataConversion::GetStringData(szInfo, "|");
		if(szone.IsEmpty() && ! szInfo.IsEmpty())
		{
			szone = szInfo;
			szInfo.Empty();
		}
		CString name = CDataConversion::GetStringData(szone, ":");
		name = _STOM0( name);
		CString length = CDataConversion::GetStringData(szone, ":");
		CString type = szone;

		if(! name.IsEmpty())
		{
			CPacket* pPacket = GetPacketInTRData(name, false);
			if(pPacket != NULL)
			{
				pPacket->SetLength(atoi(length));
				//sy 2005.10.27. -> insert 일때 들어올수 있음.
				//pPacket->ClearData(true);
			}
			else
			{
				m_AdditionList.AddTail(new CPacket(m_pPacketList, name, name, type, type, atoi(length), false, 0));
			}
		}		
	}
	return true;
}

// 02.19.2002  전체 패킷 정보를 재 세팅한다 
bool CPacketListImplementation::ResetPacketInfo(const CString& strPacketInfo)
{
	if(strPacketInfo.IsEmpty())
		return false;

	CString szInfo = strPacketInfo;
	while(! szInfo.IsEmpty())	{
		CString szone = CDataConversion::GetStringData(szInfo, "|");
		if(szone.IsEmpty() && ! szInfo.IsEmpty())	{
			szone = szInfo;
			szInfo.Empty();
		}
		SetPacketInfo(szone);
	}
	return true;
}

// 02.19.2002  패킷 정보를 재설정한다   
// 패킷명에 따른 패킷길이, 패킷타입 재설정
bool CPacketListImplementation::SetPacketInfo(const CString& strPacketInfo)
{
	CString strName, strType;
	int nLength = 0;
	if(!GetPacketTypeInInfo(strPacketInfo, strName, strType, nLength))
		return false;

	strName = _STOM0( strName);
	strType = _STOM6( strType);
	CPacket * pPacket = GetBaseData(strName);
	if(pPacket == NULL)
		return false;

	pPacket->SetLength(nLength);
	if(!strType.IsEmpty()){
		pPacket->SetType(strType);
	}

	return true;
}

//sy 2004.2.3.
// strInfo -> 패킷명:패킷길이:패킷타입|  or  패킷명:패킷길이|
bool CPacketListImplementation::GetPacketTypeInInfo(const CString& strInfo, CString& strName, CString& strType, int& nLength) const
{
	if(strInfo.IsEmpty())
		return false;

	CString strTemp = strInfo;
	strTemp.TrimRight('|');
	if(strTemp.Right(1) != ':')
		strTemp += ':';

	strName = CDataConversion::GetStringData(strTemp, ":");
	if(strName.IsEmpty())
		return false;

	nLength = atoi(CDataConversion::GetStringData(strTemp, ":"));
	strType = CDataConversion::GetStringData(strTemp, ":");
	if(strType.IsEmpty() && !strTemp.IsEmpty())
		strType = strTemp;
	return true;
}
//sy end

// 02.19.2002  일부분 패킷만 새 데이타가 내려오는 경우 
// 모든 데이타의 패킷이 clear되는 것을 막고자 따로 처리한다
bool CPacketListImplementation::PacketClearPart()
{
	if(m_nDataInsertType != CPacketDataInfo::PART_NEW)
		return false;

	POSITION pos = m_PacketList.GetHeadPosition();
	while(pos)	{
		CPacket * pPacket = m_PacketList.GetNext(pos);
		//sy workint
		CString strName = pPacket->GetName();
		if(pPacket->GetLength() > 0)
			pPacket->ClearData(true);
	}

	//sy 2005.11.25. -> 현재 돌려주면 안되고 TR set 이 다된다음 돌려준다!!
	// 패킷의 상태를 New로 돌려준다
	// (단 packet이 모두 clear되는 SetDataInsertTypeInfo()는 부르지 않는다)
	//SetDataInsertType(CPacketDataInfo::NEW);
	return true;
}

void CPacketListImplementation::DeleteData(CTypedPtrList<CObList, CPacket*>* lstPacket)
{
	POSITION pos = lstPacket->GetHeadPosition() ;
	while(pos)	{
		CPacket * pPacket = lstPacket->GetNext(pos) ;
		if(pPacket)
			pPacket->ClearData() ;
	}
}

// packet List에 있는 각 Packet들을 delete 한다 
bool CPacketListImplementation::DeletePacketList(CTypedPtrList<CObList, CPacket*>* lstPacket)
{
	if(!lstPacket)	return false;

	POSITION pos = lstPacket->GetHeadPosition();
	while(pos)	{
		CPacket * pPacket = lstPacket->GetNext(pos) ;
		delete pPacket ;
		pPacket = NULL ;		
	}
	lstPacket->RemoveAll() ;
	return true;
}

// 03.22.2002  특정 패킷에 있는 특정 index의 데이타를 return한다
double CPacketListImplementation::GetDataFromIndex(CString strFunction, int nIndex)
{
	CPacket * pPacket = GetBaseData(strFunction);
	if(pPacket)
		return pPacket->GetData(nIndex);
	else
		return 0;
}

// 03.22.2002  특정 패킷에 있는 특정 index의 데이타를 return한다
CString CPacketListImplementation::GetStrDataFromIndex(CString strFunction, int nIndex)
{
	CPacket * pPacket = GetBaseData(strFunction);
	if(pPacket == NULL)
		return "";

	return pPacket->GetStrData(nIndex);
}

bool CPacketListImplementation::FindKey(CString strKey)
{
	if(m_lstRealKeyName.GetCount() <= 0)
		return false;

	POSITION pos = m_lstRealKeyName.GetHeadPosition();
	while(pos)	{
		if(m_lstRealKeyName.GetNext(pos) == strKey)
			return true;
	}
	return false;
}

// (2006/6/5 - Seung-Won, Bae) return Deleted Data Count
int CPacketListImplementation::RemovePacketList(int nStart, int nEnd, int nType)
{
	//삭제없이 무한정 받는다.
	if(m_nHoldingPacketSize == 0)
		return 0;

	if(nType == 0 && GetMaxCount() < m_nHoldingPacketSize)
		return 0;

	//sy 2004.07.16. -> 올바른 return 값을 주기
	int nDeletedDataCount = 0;
	// TR data
	POSITION pos = m_PacketList.GetHeadPosition();
	while(pos)
	{
		CPacket * pPacket = m_PacketList.GetNext(pos);
		if( pPacket->RemoveData(nStart, nEnd)) nDeletedDataCount = nEnd - nStart;
	}
	// 지표 data
	pos = m_IndicatorDataList.GetHeadPosition();
	while(pos)
	{
		CPacket * pPacket = m_IndicatorDataList.GetNext(pos);
		if( pPacket->RemoveData(nStart, nEnd)) nDeletedDataCount = nEnd - nStart;
	}

	//0005977: [7111] 종합차트 - 틱주기 차트 조회 시, HTS 기능이 타사 보다 0.2초정도 느려지는 것처럼 보이는 현상
	//if(m_nHoldingPacketSize>0)
	//{
	//	CString szDebug;
	//	szDebug.Format("\n[DRCS]Ln(%d) CPacketListImplementation::SetHoldingPacketSize m_nHoldingPacketSize(%d)\n", __LINE__, m_nHoldingPacketSize);
	//	OutputDebugString(szDebug);
	//}
	return nDeletedDataCount;
}

int CPacketListImplementation::GetCount(const CString strPacketName)
{
	CPacket * pPacket = GetBaseData(strPacketName);
	if(pPacket == NULL)
		return -1;

	return pPacket->GetCount();
}

CTypedPtrList<CObList, CPacket*>& CPacketListImplementation::GetTRPacketList()
{
	return m_PacketList;
}

/*
CTypedPtrList<CObList, CPacket*>& CPacketListImplementation::GetIndicatorPacketList()
{
	return m_IndicatorDataList;
}
*/

CString CPacketListImplementation::GetTRPacketNames(const CString& strCompart) const
{
	CString strAllPacketNames;
	POSITION pos = m_PacketList.GetHeadPosition();
	while(pos != NULL)
	{
		CPacket* pPacket = m_PacketList.GetNext(pos);
		if(pPacket == NULL)
			continue;
		strAllPacketNames += (pPacket->GetName() + strCompart);
	}

	return strAllPacketNames;
}

void CPacketListImplementation::SetCurrentDate()
{
	CPacket* pPacket = GetBaseData( _MTEXT( C0_DATE_TIME));
	if(pPacket == NULL || pPacket->GetCount() <= 0)
		return;

	m_dCurrentDate = pPacket->GetData(pPacket->GetCount()-1);
}

double CPacketListImplementation::GetCurrentDate()
{
	return m_dCurrentDate;
}

//주가예측에서 입력된 거짓데이터 삭제
bool CPacketListImplementation::RemoveFictionData()
{
	CPacket* pPacket = GetBaseData( _MTEXT( C0_DATE_TIME));
	if(pPacket == NULL || pPacket->GetCount() <= 0)
		return false;

	int nDeletePacketCount = pPacket->RemoveFictionData(m_dCurrentDate, true);
	if(nDeletePacketCount > 0) {
		pPacket = GetBaseData( _MTEXT( C0_OPEN));
		if(pPacket == NULL)
			return false;
		pPacket->RemoveFictionData(nDeletePacketCount);

		pPacket = GetBaseData( _MTEXT( C0_HIGH));
		if(pPacket == NULL)
			return false;
		pPacket->RemoveFictionData(nDeletePacketCount);

		pPacket = GetBaseData( _MTEXT( C0_LOW));
		if(pPacket == NULL)
			return false;
		pPacket->RemoveFictionData(nDeletePacketCount);

		pPacket = GetBaseData( _MTEXT( C0_CLOSE));
		if(pPacket == NULL)
			return false;
		pPacket->RemoveFictionData(nDeletePacketCount);

		pPacket = GetBaseData( _MTEXT( C0_VOLUME));
		if(pPacket == NULL)
			return false;
		pPacket->RemoveFictionData(nDeletePacketCount);
		
		return true;
	}
	return false;
}

void CPacketListImplementation::SetHoldingPacketSize(const int nSize)
{
	m_nHoldingPacketSize = nSize;

	//0005977: [7111] 종합차트 - 틱주기 차트 조회 시, HTS 기능이 타사 보다 0.2초정도 느려지는 것처럼 보이는 현상
	//if(m_nHoldingPacketSize>0)
	//{
	//	CString szDebug;
	//	szDebug.Format("\n[DRCS]Ln(%d) CPacketListImplementation::SetHoldingPacketSize m_nHoldingPacketSize(%d)\n", __LINE__, m_nHoldingPacketSize);
	//	OutputDebugString(szDebug);
	//}
}

void CPacketListImplementation::SetBasePacketName(const CString &szPacketName)
{
	m_realCenter.SetRealBasePacket(GetBaseData(szPacketName));
}

//startPeriod는 index (index >= 0)
void CPacketListImplementation::SetDrawStartIndex(const CString &strInfo)
{
	CString szT = strInfo;
	while(!szT.IsEmpty()){
		CString one = CDataConversion::GetStringData(szT, "|");
		if(one.IsEmpty()){
			one = szT;
			szT.Empty();
		}

		CString szName = CDataConversion::GetStringData(one, ":");
		one.TrimLeft(" ");
		one.TrimRight(" ");
		if(szName.IsEmpty() || one.IsEmpty())
			continue;

		szName = _STOM0( szName);
		CPacket* pPacket = GetBaseData(szName);
		if(pPacket != NULL)
			pPacket->SetStartPeriod(atoi(one));
	}
}

//sy 2003.3.19. 종목명과 종목명 받기
void CPacketListImplementation::SetItemData(const CString &strData)
{
	if(strData.IsEmpty())
		return;

	CString strName = strData;
	CString strCode = CDataConversion::GetStringData(strName, ":");
	strCode.TrimLeft(" ");
	strCode.TrimRight(" ");
	m_itemData.SetItem(strCode, strName);
}

/*///////////////////////////////////////////////////////////////
기능 :	입력된 패킷에서 설정된 데이터길이를 구하는 메소드
입력 :	lstPacket == 검색할 패킷
		nIndex == -1(패킷전체에서 필요한 길이합산),
				   n(해당패킷의 길이 리턴)
///////////////////////////////////////////////////////////////*/
int CPacketListImplementation::GetPacketDataLength(int nIndex, CTypedPtrList<CObList, CPacket*>* lstPacket)
{
	if(!lstPacket)
		return 0;

	int nRet = 0 ;
	if(nIndex < 0)	{
		POSITION pos = lstPacket->GetHeadPosition();
		while(pos)	{
			CPacket * pPacket = lstPacket->GetNext(pos);
			nRet += pPacket->GetLength();
		}
		return nRet;
	}

	if(nIndex > lstPacket->GetCount())
		return 0;
	
	POSITION pos = lstPacket->FindIndex(nIndex) ;
	CPacket * pPacket = lstPacket->GetAt(pos) ;
	nRet = pPacket->GetLength() ;
	return nRet ;
}

int CPacketListImplementation::GetPacketDataLength(const CString &szPacketName)
{
	int nRet = 0;
	CPacket *packet = NULL;
	CString packetName = szPacketName;
	CString onePacketName = CDataConversion::GetStringData(packetName, ";");
	while(onePacketName != "") {
		packet = GetBaseData(onePacketName);
		if(packet == NULL)
			return -1;
		
		nRet += packet->GetLength();
		onePacketName = CDataConversion::GetStringData(packetName, ";");
	}

	return nRet;

	
}

CPacket * CPacketListImplementation::GetRealPacket(const CString& strRealFieldName, const CString& strRealKeyName)
{
	if(strRealFieldName.IsEmpty())
		return NULL;

	POSITION pos = m_PacketList.GetHeadPosition();

	CPacket * pRet = NULL;
	while(pos)	{
		CPacket * pPacket = m_PacketList.GetNext(pos);
		CString strKey = pPacket->GetRealKeyName();
		//종목코드가 5자리는 채워서 2자리는 빈공간으로 들어오는경우
		strKey.TrimRight();
		if(pPacket->GetUseReal() == true 
			&& pPacket->GetRealFieldName() == strRealFieldName
			&& strKey == strRealKeyName)	{
				return pPacket;	
		}
	}
	return NULL;
}

int CPacketListImplementation::SetBaseData(long pData, int nCount, CTypedPtrList<CObList, CPacket*>* lstPacket)
{
	if(nCount <= 0)
		return 0;

	int nAllPacketLength = GetPacketDataLength(-1, lstPacket) ;
	int nTotLength = nAllPacketLength * nCount;
	char *szData = new char[nTotLength + 1];
	memset(szData, 0x00, nTotLength);
	memcpy(szData, (void*)(pData), nTotLength) ;
	szData[nTotLength] = NULL ;

	switch(m_nDataInsertType)
	{
		case CPacketDataInfo::NEW :
		case CPacketDataInfo::APPEND :
		{
			//sy 2005.07.14.
			POSITION pos = lstPacket->GetHeadPosition();
			int nDataTotalIndex = 0;
			while(pos != NULL)
			{
				AppendAllDataInPacket(lstPacket->GetNext(pos), szData, false, nAllPacketLength, nCount, nDataTotalIndex);
			}
			break;
		}
		case CPacketDataInfo::INSERT :
		{
			//sy 2005.07.14.
			POSITION pos = lstPacket->GetHeadPosition();
			int nDataTotalIndex = 0;
			while(pos != NULL)
			{
				InsertAllDataInPacket(lstPacket->GetNext(pos), szData, nAllPacketLength, nCount, nDataTotalIndex);
			}
			break;
		}
		case CPacketDataInfo::PART_NEW:
		{
			//sy 2005.07.14.
			POSITION pos = lstPacket->GetHeadPosition();
			int nDataTotalIndex = 0;
			while(pos != NULL)
			{
				CPacket* pPacket = lstPacket->GetNext(pos);
				if(pPacket == NULL)
					break;

				//sy 2005.11.28.
				// length > 0 일 경우만 기존에 있는 packet은 삭제 한다.
				//pPacket->ClearData(true);
				AppendAllDataInPacket(pPacket, szData, true, nAllPacketLength, nCount, nDataTotalIndex);
			}
			break;
		}
		default:
			break;
	}

	delete szData;
	return nTotLength;
}

void CPacketListImplementation::Initialize_BaseDataBefore(const bool bIsBasePacket)
{
	// 보조메시지 이후 TR Data 이전.
	if(bIsBasePacket)
	{
		Initialize_PacketData();
	}
}

void CPacketListImplementation::Initialize_BaseDataAfter(const bool bIsBasePacket, const int nRowCount, CTypedPtrList<CObList, CPacket*>* pPacketList)
{
	// 보조메시지 이후 TR Data 이후.
	SetCurrentDate();
	SetPecketBaseData(pPacketList);

	if( bIsBasePacket)
	{
		AddTRDataCount( nRowCount);
		// (2007/1/3 - Seung-Won, Bae) Real Receiving State will be resetted in Only New TR.
		if( m_nDataInsertType != CPacketDataInfo::INSERT)
		{
			CPacket *pOpenPacket = NULL;
			CPacket *pHighPacket = NULL;
			CPacket *pLowPacket = NULL;
			CPacket *pClosePacket = NULL;
			CPacket *pVolumePacket = NULL;
			CPacket *pPacket = NULL;
			CString strPacketName;
			POSITION psnPacket = m_PacketList.GetHeadPosition();
			while( psnPacket && !( pOpenPacket && pHighPacket && pLowPacket && pClosePacket && pVolumePacket))
			{
				pPacket = m_PacketList.GetNext( psnPacket);
				if( !pPacket) continue;
				strPacketName = pPacket->GetName();
				if( !pOpenPacket)	if( strPacketName == _MTEXT( C0_OPEN))		pOpenPacket = pPacket;
				if( !pHighPacket)	if( strPacketName == _MTEXT( C0_HIGH))		pHighPacket = pPacket;
				if( !pLowPacket)	if( strPacketName == _MTEXT( C0_LOW))		pLowPacket = pPacket;
				if( !pClosePacket)	if( strPacketName == _MTEXT( C0_CLOSE))		pClosePacket = pPacket;
				if( !pVolumePacket)	if( strPacketName == _MTEXT( C0_VOLUME))	pVolumePacket = pPacket;
			}
			//<< 20100324_JS.Kim 솔로몬.FX	시고저종 외의 패킷을 시고저종처럼 사용하기 위한 처리
			if( CChartInfo::FX == m_realCenter.GetChartMode() )
			{
				if( !pOpenPacket || !pHighPacket || !pLowPacket || !pClosePacket )
				{
					psnPacket = m_PacketList.GetHeadPosition();
					while( psnPacket && !( pOpenPacket && pHighPacket && pLowPacket && pClosePacket && pVolumePacket))
					{
						pPacket = m_PacketList.GetNext( psnPacket);
						if( !pPacket) continue;
						strPacketName = pPacket->GetSubName();
						if( !strPacketName.IsEmpty() )
						{
							if( !pOpenPacket)	if( strPacketName == _MTEXT( C0_OPEN))		pOpenPacket = pPacket;
							if( !pHighPacket)	if( strPacketName == _MTEXT( C0_HIGH))		pHighPacket = pPacket;
							if( !pLowPacket)	if( strPacketName == _MTEXT( C0_LOW))		pLowPacket = pPacket;
							if( !pClosePacket)	if( strPacketName == _MTEXT( C0_CLOSE))		pClosePacket = pPacket;
							if( !pVolumePacket)	if( strPacketName == _MTEXT( C0_VOLUME))	pVolumePacket = pPacket;
						}
					}
				}
			}
			//>>
			m_realCenter.InitializeTRPart( pOpenPacket, pHighPacket, pLowPacket, pClosePacket, pVolumePacket);
		}
	}

	//sy 2005.11.28. 기본으로 packet을 다 추가한뒤엔 NEW 모드로 전환
	if(m_nDataInsertType == CPacketDataInfo::PART_NEW)
		SetDataInsertType(CPacketDataInfo::NEW);
}

void CPacketListImplementation::Initialize_PacketData()
{
	// packet clear에 관련된 부분으로 보조메시지가 들어온뒤 data 넣기전에 처리하는 부분으로 
	// 보조메시의 내용을 클리어 하지 않기 위해 순수하게 data만 클리어 한다.

	// 신규 data인 경우에는 기존의 모든 데이타를 지우고 다시 저장한다
	// 저장공간은 그대로 두고 데이타만 지운다
	if(m_nDataInsertType == CPacketDataInfo::NEW)
	{
		ClearDataAll(true);
		//sy 2005.10.25. -> 기본 TR 이 아닌 보조 TR로 들어온 data
		//DeletePacketList(&m_AdditionList);
	}
	else if(m_nDataInsertType == CPacketDataInfo::INSERT)
	{
		DeleteData(&m_IndicatorDataList);
	}
	else if(m_nDataInsertType == CPacketDataInfo::APPEND)
	{
	}
	else if(m_nDataInsertType == CPacketDataInfo::PART_NEW)
	{
	}
}

// 각 packet 을 초기화. 
// bClearData = true : 기본 TR도 포함하여 data를 clear.
void CPacketListImplementation::Initialize_Packet(const bool bClearIncludingTR)
{
	POSITION pos = m_PacketList.GetHeadPosition() ;
	while(pos)
	{
		CPacket * pPacket = m_PacketList.GetNext(pos) ;
		if(pPacket)
			pPacket->Initialize(bClearIncludingTR);
	}

	pos = m_IndicatorDataList.GetHeadPosition() ;
	while(pos)
	{
		CPacket * pPacket = m_IndicatorDataList.GetNext(pos) ;
		if(pPacket)
			pPacket->Initialize(bClearIncludingTR);
	}

	pos = m_AdditionList.GetHeadPosition();
	while(pos)
	{
		CPacket * pPacket = m_AdditionList.GetNext(pos);
		if(pPacket)
			pPacket->Initialize(bClearIncludingTR);
	}
}

void CPacketListImplementation::Initialize_DataInsertType()
{
	//	신규 data인 경우에는 기존의 모든 데이타를 지우고 다시 저장한다
	//	저장공간은 그대로 두고 데이타만 지운다
	if(m_nDataInsertType == CPacketDataInfo::NEW)
	{
		Initialize_Packet();
		Initialize_HelpMessage();
	}
	else if(m_nDataInsertType == CPacketDataInfo::INSERT)
	{
		//sy 2005.12.05
		//지표data를 삭제하면 안된다
		//-> 기본packet 후 보조packet(시장지표)가 들어오는 경우가 있음 : 기본packet에 대한 지표data 삭제되는 현상 발생됨
		//DeleteData(&m_IndicatorDataList);
	}
	else if(m_nDataInsertType == CPacketDataInfo::APPEND)
	{
		int nOldIndex = m_PacketList.GetAt((m_PacketList.GetHeadPosition()))->GetCount();
		m_realCenter.SetUpdateRangeOffset(nOldIndex, nOldIndex);
	}
	else if(m_nDataInsertType == CPacketDataInfo::PART_NEW)
	{
	}
}

void CPacketListImplementation::Initialize_HelpMessage()
{
	m_nPriceType = CPacketType::ORIGINAL;
	m_realCenter.InitializeHelpMessage();
	m_trDataCountList.Initialize();
	m_itemData.Clear();

	// (2004.09.14, 배승원) 보조 Message Item을 범용적으로 관리하여 조회 이용할 수 있도록한다.
	m_mapHelpMessage.RemoveAll();
}

void CPacketListImplementation::SetPecketBaseData(CTypedPtrList<CObList, CPacket*>* pPacketList)
{
	if(pPacketList == NULL)
		return;

	POSITION pos = pPacketList->GetHeadPosition() ;
	while(pos)
	{
		CPacket * pPacket = pPacketList->GetNext(pos);
		pPacket->SetYesOrNoBaseData(true);
		pPacket->SetAcceptFromServer(true);
	}
}
//sy end

int CPacketListImplementation::SetBaseDataNewPacket(long pData, int nCount)
{
	if(nCount <= 0)
		return 0;
	return nCount;
}

bool CPacketListImplementation::SetErrorMessage(const CString& strMessage)
{
	if(strMessage.IsEmpty())	return false;
	CString strM = strMessage;
	CString error_no = CDataConversion::GetStringData(strM, ":");
	
	if(error_no == "P0")	{
		AfxMessageBox(strM, MB_ICONERROR);
//		MessageBox(NULL, strM, "Error", MB_ICONERROR);
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
//sy 2003.3.19. -> 종목코드/종목이름
void CPacketListImplementation::SetItemData(const CString& strCode, const CString& strName)
{
	m_itemData.SetItem(strCode, strName);
}

void CPacketListImplementation::GetItemData(CString& strCode, CString& strName) const
{
	strCode = m_itemData.GetCode();
	strName = m_itemData.GetName();
}

CString CPacketListImplementation::GetItemCode() const
{
	return m_itemData.GetCode();
}

CString CPacketListImplementation::GetItemName() const
{
	return m_itemData.GetName();
}

//종가 packet의 장구분 (주식/업종/선물/옵션).
CPacketType::COMMODITY_BOND_NAME CPacketListImplementation::GetCommodityBondNameInBase() const
{
	CPacket* pClosePacket = GetBaseDataInCurrent( _MTEXT( C0_CLOSE));
	if(pClosePacket != NULL)
		return pClosePacket->GetCommodityBondName();
		
	return GetCommodityBondNameInCode();
}

//code로 장구분.
CPacketType::COMMODITY_BOND_NAME CPacketListImplementation::GetCommodityBondNameInCode() const
{
	if(m_lstRealKeyName.GetCount() <= 0)
		return CPacketType::NONAME;

	CString strKeyName = m_lstRealKeyName.GetHead();
	int nLength = strKeyName.GetLength();

	switch(nLength){
	case 3:
		return CPacketType::INDUSTRY;	//업종
	case 6:
		return CPacketType::STOCK;		//주식
	case 8:
		{
			CString strLeft = strKeyName.Left(1);
			if(IS_FUT_CODE_1(strLeft)) // 0024696: 파생상품 코드체계 변경 제도개선
				return CPacketType::FUTURES; //선물
			else if(IS_CALL_CODE_1(strLeft))
				return CPacketType::OPTION;	 //옵션
		}
		return CPacketType::NONAME;
	}

	return CPacketType::NONAME;
}

int CPacketListImplementation::GetLastestRealIndex()
{
	return m_realCenter.GetUpdateOrAppendIndex();
}

/*
void CPacketListImplementation::AddPacket(const CString szPacketOption)
{
	SetOption(szPacketOption);
}
*/

bool CPacketListImplementation::RemovePacket(const CString szPacketName)
{
	POSITION pos = m_PacketList.GetHeadPosition();
	CPacket *packet;
	while(pos)
	{
		packet = m_PacketList.GetAt(pos);

		if(packet != NULL && !packet->GetName().CompareNoCase(szPacketName))
		{
			delete packet;
			packet = NULL;
			m_PacketList.RemoveAt(pos);
			return true;
		}
		m_PacketList.GetNext(pos);
	}
	return false;
}

//sy 2003.8.13.
// bIsDelete = true : packet 삭제한다. (false : packet data 삭제)
// bIsOnlyNoTRData = true : TR Data 가 아닌경우만 수행한다.
bool CPacketListImplementation::DeleteOrClearPacket(const CString& strPacketName, const bool bIsDelete, const bool bIsOnlyNoTRData)
{
	return DeleteOrClearPacket(GetBaseData(strPacketName), bIsDelete, bIsOnlyNoTRData);
}

// bIsDelete = true : packet 삭제한다. (false : packet data 삭제)
// bIsOnlyNoTRData = true : TR Data 가 아닌경우만 수행한다.
bool CPacketListImplementation::DeleteOrClearPacket(CPacket* pPacket, const bool bIsDelete, const bool bIsOnlyNoTRData)
{
	if(pPacket == NULL)
		return false;

	CTypedPtrList<CObList, CPacket*>* pPacketList = NULL;
	POSITION pos = NULL;
	if(!FindPosition(pPacket, pPacketList, pos))
		return false;

	// TR Data 인 경우 실행하지 않는다.
	if(bIsOnlyNoTRData && pPacket->IsTRData())
		return false;

	if(bIsDelete){
		pPacketList->RemoveAt(pos);
		delete pPacket;
	}
	else{
		pPacket->ClearData(true);
	}

	return true;
}

//sy 2005.10.21.
void CPacketListImplementation::SetPacketData(const CString& szPacketNames, long pData, long lCount)
{
	//>>[alzioyes:지우지말것,DRFN Platform 로그기능추가]
	{
		CString szMessage;
		szMessage.Format("CPacketListImplementation::SetPacketData[%s]", szPacketNames);
		if(!m_pDRDebuger) _GetDRDebuger();
		if(m_pDRDebuger) 
		{
			m_pDRDebuger->DRLog_Message(szMessage);		
			m_pDRDebuger->DRLog_ResponseData("PacketList", (LPBYTE)pData, lCount);	//[alzioyes:지우지말것,DRFN Platform 로그기능추가]
		}
	}
	//<<

	// 보조메시지 넣기.
	bool bIsBasePacket = true;
	int nDataCount = 0;
	CTypedPtrList<CObList, CPacket*> packetList;
	if(!SetPacketData_HelpMsg(szPacketNames, pData, lCount, nDataCount, bIsBasePacket, packetList))
	{
		Initialize_BaseDataBefore( bIsBasePacket);
		Initialize_BaseDataAfter( bIsBasePacket, 0, &packetList);
		return;
	}

	SetPacketData(pData, lCount, nDataCount, bIsBasePacket, packetList);
}

void CPacketListImplementation::SetPacketData(long pData, long lCount, const int nDataCount, const bool bIsBasePacket, CTypedPtrList<CObList, CPacket*>& packetList)
{
	Initialize_BaseDataBefore(bIsBasePacket);

	// packet 에 data 넣기.
	char *szData = new char[lCount - nDataCount + 1];
	if( (lCount-nDataCount)>0)
	{
		memset(szData, 0x00, lCount - nDataCount);
		memcpy(szData, (void*)(pData + nDataCount), lCount - nDataCount) ;
	}
	szData[lCount - nDataCount] = NULL;

	int nRowCount = GetRowCount(pData);

// (2009/3/10 - Seung-Won, Bae) Special Chart X Scale Error TEST
//#ifdef _DEBUG
//	nRowCount = 0;
//#endif

	SetPacketData(packetList, szData, nRowCount);
	delete [] szData;

	Initialize_BaseDataAfter(bIsBasePacket, nRowCount, &packetList);
}

bool CPacketListImplementation::SetPacketData_HelpMsg(const CString& szPacketName, long pData, long lCount, 
			int& nDataCount, bool& bIsBasePacket, CTypedPtrList<CObList, CPacket*>& packetList)
{
	if(pData <= 0)
		return false;

	// 보조메시지 넣기.
	CString strHelpMsgData;
	GetHelpMessageData(pData, nDataCount, strHelpMsgData);
	if(SetHelpMessage(strHelpMsgData, true) == 2)
	{
		GetPacketList(szPacketName, ';', false, packetList);
		bIsBasePacket = false;
	}
	else
	{
		GetPacketList(szPacketName, ';', true, packetList);
		bIsBasePacket = true;
	}

	return (packetList.GetCount() > 0);
}

int CPacketListImplementation::GetRowCount(long pData) const
{
	if(pData <= 0)
		return 0;

	char *szCount = new char[m_nPacketLengthCount+1];
	memcpy(szCount, (void*)pData, m_nPacketLengthCount);
	szCount[m_nPacketLengthCount] = NULL;
	int nRowCount = atoi(szCount);
	delete[] szCount;

	return nRowCount;
}

void CPacketListImplementation::GetHelpMessageData(long pData, int& nDataCount, CString& strHelpMsgData) const
{
	nDataCount = 0;
	if(pData <= 0)
		return;

	char szbozo[5];
	memcpy(szbozo, (void*)(pData+m_nPacketLengthCount), 4) ;
	szbozo[4] = NULL;
	int nHelpMsgLength = atoi(szbozo);
	
	nDataCount = m_nPacketLengthCount + 4 + nHelpMsgLength;

	char * szBozomsg = new char[nHelpMsgLength + 1] ;
	memcpy(szBozomsg, (void*)(pData+m_nPacketLengthCount+4), nHelpMsgLength) ;
	szBozomsg[nHelpMsgLength] = NULL ;
	strHelpMsgData = szBozomsg ;
	delete[] szBozomsg ;
}

bool CPacketListImplementation::SetPacketData(CTypedPtrList<CObList, CPacket*>& packetList, char* szData, const int nRowCount)
{
	if(packetList.GetCount() <= 0)
		return false;

	bool bResult = false;
	int nAllPacketLength = GetPacketDataLength(-1, &packetList);
	switch(m_nDataInsertType)
	{
		case CPacketDataInfo::NEW :
		case CPacketDataInfo::APPEND :
		{

// (2009/1/29 - Seung-Won, Bae) for 92000 TEST
//if( m_realCenter.GetDateUnitType() != CScaleBaseData::HORZ_TICK)
//{
//	m_nI = 0;

			//sy 2005.09.30.
			int nDataTotalIndex = 0;
			POSITION pos = packetList.GetHeadPosition();
			while(pos != NULL)
			{
				AppendAllDataInPacket(packetList.GetNext(pos), szData, false, nAllPacketLength, nRowCount, nDataTotalIndex);
			}
// (2009/1/29 - Seung-Won, Bae) for 92000 TEST
//}
//else
//{
//	for( m_nI = 22; 0 <= m_nI; m_nI--)
//	{
//		//sy 2005.09.30.
//		int nDataTotalIndex = 0;
//		POSITION pos = packetList.GetHeadPosition();
//		while(pos != NULL)
//		{
//			AppendAllDataInPacket(packetList.GetNext(pos), szData, false, nAllPacketLength, nRowCount, nDataTotalIndex);
//		}
//	}
// (2009/1/29 - Seung-Won, Bae) for 92000 TEST
//}

			bResult = true;
			break;
		}
		// (2004.05.19, 배승원) DLL을 통한 Packet 등록시에도 Insert가 가능하도록 한다.
		case CPacketDataInfo::INSERT :
		{
			//sy 2005.09.30.
			int nDataTotalIndex = 0;
			POSITION pos = packetList.GetHeadPosition();
			while(pos != NULL)
			{
				InsertAllDataInPacket(packetList.GetNext(pos), szData, nAllPacketLength, nRowCount, nDataTotalIndex);
			}
			bResult = true;
			break;
		}
		case CPacketDataInfo::PART_NEW:
		{
			//sy 2005.09.30.
			int nDataTotalIndex = 0;
			POSITION pos = packetList.GetHeadPosition();
			while(pos != NULL)
			{
				CPacket* pPacket = packetList.GetNext(pos);
				if(pPacket == NULL)
					break;

				//sy 2005.11.28.
				// length > 0 일 경우만 기존에 있는 packet은 삭제 한다.
				//pPacket->ClearData(true);
				AppendAllDataInPacket(pPacket, szData, true, nAllPacketLength, nRowCount, nDataTotalIndex);
			}
			bResult = true;
			break;
		}
		default:
			break;
	}
	//SetMaxDataCount();

	return bResult;
}
//sy end

//sy 2005.07.14.
void CPacketListImplementation::AppendAllDataInPacket(CPacket* pPacket, char* szData, const bool bClearData, 
			const int nAllPacketLength, const int nRowCount, int& nDataTotalIndex)
{
	if(pPacket == NULL)
		return;

	int nItemLength = pPacket->GetLength();
	if(nItemLength <= 0)
		return;

	//sy 2005.11.28.
	//-> 여러 packet이 length만 바꾸어서 들어올경우가 있기 때문에 length > 0 경우만 data clear!
	if(bClearData)
	{
		//sy 2006.03.28.
		//pPacket->Initialize(true);
		pPacket->ClearData(true);
	}

	// (2006/11/19 - Seung-Won, Bae) Retrieve TDC Info
	double dFactor = 1.0;
	CString strTDC;
	if( m_mapHelpMessage.Lookup( "TDC", strTDC))
	{
		int nIndex = strTDC.Find( pPacket->GetName() + ",");
		if( 0 < nIndex)
		{
			char cSeperator = strTDC.GetAt( nIndex - 1);
			if( ',' == cSeperator || ':' == cSeperator)
			{
				strTDC = strTDC.Left( nIndex);
				int nStart = strTDC.ReverseFind( '|') + 1;
				int nEnd = strTDC.ReverseFind( ':');
				if( nStart < nEnd) dFactor = atof( strTDC.Mid( nStart, nEnd - nStart));
			}
		}
	}

	// (2007/1/19 - Seung-Won, Bae) Check Double-Byte Character Truncation.
	BOOL bText = ( pPacket->GetType() == _MTEXT( C6_CHARACTER));

	CString strDataBuffer;
	char *szDataBuffer = strDataBuffer.GetBufferSetLength( nItemLength);
	memset( szDataBuffer, 0, nItemLength);
	if( 1.0 != dFactor)
	{
		for(int nId = 0 ; nId < nRowCount ; nId++)
		{
			memcpy(szDataBuffer, szData + nAllPacketLength*nId+nDataTotalIndex, nItemLength);
			szDataBuffer[ nItemLength] = NULL;
			pPacket->AppendTrData( atof( szDataBuffer) * dFactor);
		}
	}
	else if( pPacket->GetName() == _MTEXT( C0_DATE_TIME))
	{
		CString strPacketType = pPacket->GetType();
		double dDateTime = 0.;
		int nTimeDiff = 0;
		if( 0 <= strPacketType.Find( "HH")) nTimeDiff = m_nBaseTimeDiff - m_nOrgBaseTimeDiff;
		for(int nId = 0 ; nId < nRowCount ; nId++)
		{
			memcpy(szDataBuffer, szData + nAllPacketLength*nId+nDataTotalIndex, nItemLength);
			szDataBuffer[ nItemLength] = NULL;
			dDateTime = atof( szDataBuffer);
			if( 0 != nTimeDiff) dDateTime = CDataConversion::CalcTimeDiff( dDateTime, nTimeDiff, m_nBaseTimeDiff, strPacketType);
			pPacket->AppendTrData( dDateTime);
		}
	}
	else for( int nId = 0 ; nId < nRowCount ; nId++)
	{
		memcpy( szDataBuffer, szData + nAllPacketLength*nId+nDataTotalIndex, nItemLength);
		szDataBuffer[ nItemLength] = NULL;

		if( bText)
		{
			char *szLeadByte = szDataBuffer;
			while( *szLeadByte)
			{
				if( *szLeadByte & 0x80)
				{
					if( *(szLeadByte + 1) & 0x80) szLeadByte += 2;
					else *szLeadByte = '\0';
				}
				else szLeadByte++;
			}
		}

// (2009/1/29 - Seung-Won, Bae) for 92000 TEST
//if( nDataTotalIndex == 0)
//{
//	int nDay = ( szDataBuffer[ 0] - '0') * 10 + ( szDataBuffer[ 1] - '0');
//	nDay -= m_nI;
//	if( nDay < 1) nDay += 31;
//	szDataBuffer[ 0] = '0' + nDay / 10;
//	szDataBuffer[ 1] = '0' + nDay % 10;
//}

		pPacket->AppendData( ( long)szDataBuffer, nItemLength);
	}
	nDataTotalIndex += nItemLength;
}

void CPacketListImplementation::InsertAllDataInPacket(CPacket* pPacket, char* szData, const int nAllPacketLength, 
			const int nRowCount, int& nDataTotalIndex)
{
	if(pPacket == NULL)
		return;

	int nItemLength = pPacket->GetLength();
	if(nItemLength <= 0)
		return;

	// (2006/11/19 - Seung-Won, Bae) Retrieve TDC Info
	double dFactor = 1.0;
	CString strTDC;
	if( m_mapHelpMessage.Lookup( "TDC", strTDC))
	{
		int nIndex = strTDC.Find( pPacket->GetName() + ",");
		if( 0 < nIndex)
		{
			char cSeperator = strTDC.GetAt( nIndex - 1);
			if( ',' == cSeperator || ':' == cSeperator)
			{
				strTDC = strTDC.Left( nIndex);
				int nStart = strTDC.ReverseFind( '|') + 1;
				int nEnd = strTDC.ReverseFind( ':');
				if( nStart < nEnd) dFactor = atof( strTDC.Mid( nStart, nEnd - nStart));
			}
		}
	}

	// (2007/1/19 - Seung-Won, Bae) Check Double-Byte Character Truncation.
	BOOL bText = ( pPacket->GetType() == _MTEXT( C6_CHARACTER));

	CString strDataBuffer;
	char *szDataBuffer = strDataBuffer.GetBufferSetLength( nItemLength);
	memset( szDataBuffer, 0, nItemLength);
	if( 1.0 != dFactor)
	{
		for(int nId = nRowCount - 1; nId >= 0; nId--)
		{
			memcpy(szDataBuffer, szData + nAllPacketLength*nId+nDataTotalIndex, nItemLength);
			szDataBuffer[nItemLength] = NULL;
			pPacket->InsertTrData( atof( szDataBuffer) * dFactor);
		}
	}
	else if( pPacket->GetName() == _MTEXT( C0_DATE_TIME))
	{
		CString strPacketType = pPacket->GetType();
		double dDateTime = 0.;
		int nTimeDiff = 0;
		if( 0 <= strPacketType.Find( "HH")) nTimeDiff = m_nBaseTimeDiff - m_nOrgBaseTimeDiff;
		for(int nId = nRowCount - 1; nId >= 0; nId--)
		{
			memcpy(szDataBuffer, szData + nAllPacketLength*nId+nDataTotalIndex, nItemLength);
			szDataBuffer[nItemLength] = NULL;
			dDateTime = atof( szDataBuffer);
			if( 0 != nTimeDiff) dDateTime = CDataConversion::CalcTimeDiff( dDateTime, nTimeDiff, m_nBaseTimeDiff, strPacketType);
			pPacket->InsertTrData( dDateTime);
		}
	}
	else for(int nId = nRowCount - 1; nId >= 0; nId--)
	{
		memcpy( szDataBuffer, szData + nAllPacketLength*nId+nDataTotalIndex, nItemLength);
		szDataBuffer[ nItemLength] = NULL;

		if( bText)
		{
			char *szLeadByte = szDataBuffer;
			while( *szLeadByte)
			{
				if( *szLeadByte & 0x80)
				{
					if( *(szLeadByte + 1) & 0x80) szLeadByte += 2;
					else *szLeadByte = '\0';
				}
				else szLeadByte++;
			}
		}
		pPacket->InsertData( ( long)szDataBuffer, nItemLength);
	}
	nDataTotalIndex += nItemLength;
}
//sy end

//sy 2004.2.3.
// 해당 packet의 data setting.
// strPacketType : 현재는 보조메시지의 "RESETPACKET"만 해당됨"
// ex)	CString strPacketNames = "선물매수";
//		CString strPacketTypes = "선물매수:8|@";
//		CString strPacketData = "   10.00   30.00";
//		SetPacketData(strPacketNames, strPacketTypes, 2, strPacketData)
bool CPacketListImplementation::SetPacketData(const CString& strPacketName, const CString& strPacketType, 
		const char* pPacketData, const int nCount)
{
	CPacket* pPacket = GetBaseData(strPacketName);
	if(pPacket == NULL)
		return false;

	// 보조 data
	ResetPacketInfo(strPacketType);

	// 실 data
	return SetPacketData(pPacket, pPacketData, nCount);
}

// 실 data setting
bool CPacketListImplementation::SetPacketData(CPacket* pPacket, const char* pPacketData, const int nCount)
{
	if(nCount < 0 || pPacket == NULL)
		return false;

	// 기본data라도 삭제한다.!
	pPacket->ClearData(true);

	int nLength = pPacket->GetLength();
	char* buffer = new char[nLength + 1];

	for(int nIndex = 0; nIndex < nCount; nIndex++)
	{
		ZeroMemory(buffer, nLength + 1);
		memcpy(buffer, pPacketData + (nLength * nIndex), nLength);
		pPacket->AppendData(buffer);
	}

	delete [] buffer;

	//SetMaxDataCount();
	return true;
}
//sy end

bool CPacketListImplementation::SetData(const CString szPacketName, int nIndex, double dValue, bool IsByReal)
{
	CPacket *packet = GetBaseData(szPacketName);
	if(packet == NULL)
		return false;

	double oldValue = packet->GetData(nIndex);

	if(oldValue == dValue)
		return false;

	packet->UpdateData(dValue, nIndex);

	if(IsByReal)
		m_realCenter.SetUpdateOrAppendIndex(nIndex);

	return true;
}

void CPacketListImplementation::SetPacketLengthCount(int nLength)
{
	if(nLength != 4 && nLength != 5) {
		AfxMessageBox("Error!! PacketLengthSize Setting");
		m_nPacketLengthCount = 5;
		return;
	}

	m_nPacketLengthCount = nLength;
}

//sy 2003.7.25.
bool CPacketListImplementation::GetAllPacketsMinMax(CTypedPtrList<CObList, CPacket*>& packetList, const int nStart, const int nEnd, double& dMin, double& dMax)
{
	bool bStart = true;
	POSITION pos = packetList.GetHeadPosition();
	while(pos != NULL){
		CPacket* pPacket = packetList.GetNext(pos);
		//"자료일자"는 구하지 않는다.
		if(pPacket->GetName().Find( _MTEXT( C0_DATE_TIME)) != -1)
			continue;

		double dTempMin = 0, dTempMax = 0;
		if(pPacket->GetMinMax(nStart, nEnd, dTempMin, dTempMax) < 0)
			continue;

		if(bStart || dMin > dTempMin)
			dMin = dTempMin;
		if(bStart || dMax < dTempMax)
			dMax = dTempMax;

		bStart = false;
	}
	return !bStart;
}

//sy 2003.8.14.
bool CPacketListImplementation::FindPosition(CPacket* pPacket, CTypedPtrList<CObList, CPacket*>*& pPacketList, POSITION& pos)
{
	if(pPacket == NULL)
		return false;

	pos = m_PacketList.Find(pPacket);
	if(pos != NULL){
		pPacketList = &m_PacketList;
		return true;
	}

	pos = m_IndicatorDataList.Find(pPacket);
	if(pos != NULL){
		pPacketList = &m_IndicatorDataList;
		return true;
	}

	return false;
}

void CPacketListImplementation::SetVersion(double dVersion)
{
	m_dVersion = dVersion;
}

CPacketType::REALTYPE CPacketListImplementation::GetRealType() const
{
	return m_realCenter.GetRealType();
}

int CPacketListImplementation::GetRemovePacketCountInRealData() const
{
	return _REMOVE_PACKET_COUNT;
}

// (2004.09.14, 배승원) 보조 Message Item을 범용적으로 관리하여 조회 이용할 수 있도록한다.
BOOL CPacketListImplementation::GetHelpMessageItem( const char *p_szMsgKey, CString &p_strMsgValue)
{
	return m_mapHelpMessage.Lookup( p_szMsgKey, p_strMsgValue);
}

//sy 2005.08.05. 장구분 -> packetName:장구분|packetName:장구분|@
void CPacketListImplementation::SetCommodityBondName(const CString& strData)
{
	CString strAllCommodityData = strData;
	if(strAllCommodityData.Find("|") < 0)
		strAllCommodityData += "|";

	while(!strAllCommodityData.IsEmpty())
	{
		CString strCommodityData = CDataConversion::GetStringData(strAllCommodityData, "|");
		if(strCommodityData.IsEmpty() && !strAllCommodityData.IsEmpty())
		{
			strCommodityData = strAllCommodityData;
			strAllCommodityData.Empty();
		}

		CString strPacketName = CDataConversion::GetStringData(strCommodityData, ":");
		CPacket* pPacket = GetBaseDataInCurrent(strPacketName);
		if(pPacket == NULL)
			continue;

		pPacket->SetCommodityBondName(CPacketType::COMMODITY_BOND_NAME(atoi(strCommodityData)));
	}
}
//sy end

bool CPacketListImplementation::SetHelpMessageRealDataRate(const CString& strHelpMessage, const bool bIsRemove)
{
	return m_realCenter.SetRealDataModuleAndData( bIsRemove, strHelpMessage, false);
}
//sy end

//sy 2005.11.01.
void CPacketListImplementation::SetMaxDataCount()
{
	m_nMaxDataCount = 0;
	if(m_PacketList.GetCount() <= 0)
		return;

	POSITION pos = m_PacketList.GetHeadPosition();
	while(pos != NULL)
	{
		CPacket * pPacket = m_PacketList.GetNext(pos);
		if(pPacket == NULL)
			continue;

		int nCount = pPacket->GetCount();
		if(m_nMaxDataCount < nCount)
			m_nMaxDataCount = nCount;
	}
}
//sy end

//sy 2005.11.14.
void CPacketListImplementation::AddTRDataCount(const int nCount)
{
	switch(m_nDataInsertType)
	{
	case CPacketDataInfo::NEW:
		m_trDataCountList.Initialize();
		m_trDataCountList.AddTRData(nCount);
		break;
	case CPacketDataInfo::INSERT:
		m_trDataCountList.AddTRData(nCount);
		break;
	case CPacketDataInfo::APPEND:
	default:
		break;
	}
}
//sy end



// (2006/12/20 - Seung-Won, Bae) Support Binary TR Data Receiving
BOOL CPacketListImplementation::SetBinTrHelpMsg( const char *p_szHelpMessage)
{
	m_lBinTrRowCount	= 0;
	m_pBinTrPacketList	= NULL;

	// (2009/6/5 - Seung-Won, Bae) Initialize before Help Message.
	Initialize_BaseDataBefore( !strstr( p_szHelpMessage, "NEWPACKET="));

	//	보조메세지 세팅
	switch( SetHelpMessage( p_szHelpMessage, true))
	{
		case 1:	m_pBinTrPacketList		= &m_PacketList;
				m_bBinTrIsBasePacket	= TRUE;
				break;
		case 2: m_pBinTrPacketList		= &m_AdditionList;
				m_bBinTrIsBasePacket	= FALSE;
				break;
		case 0:
		default: return FALSE;
	}

	return TRUE;
}
BOOL CPacketListImplementation::SetBinTrDoubleData( const char *p_szPacketName, long p_nCount, double FAR* p_pdData)
{
	if( !m_pBinTrPacketList) return FALSE;
	if( m_lBinTrRowCount < p_nCount) m_lBinTrRowCount = p_nCount;

	if( p_nCount <= 0) return TRUE;

	CPacket *pPacket = NULL;
	POSITION psnPacket = m_pBinTrPacketList->GetHeadPosition();
	while( psnPacket)
	{
		pPacket = m_pBinTrPacketList->GetNext( psnPacket);
		if( !pPacket) continue;
		if( pPacket->GetName() == p_szPacketName) break;
		pPacket = NULL;
	}
	if( !pPacket) return FALSE;
	if( pPacket->GetLength() <= 0) return FALSE;
	if( pPacket->GetType() == _MTEXT( C6_CHARACTER)) return FALSE;

	// (2006/11/19 - Seung-Won, Bae) Retrieve TDC Info
	double dFactor = 1.0;
	CString strTDC;
	if( m_mapHelpMessage.Lookup( "TDC", strTDC))
	{
		int nIndex = strTDC.Find( pPacket->GetName() + ",");
		if( 0 < nIndex)
		{
			char cSeperator = strTDC.GetAt( nIndex - 1);
			if( ',' == cSeperator || ':' == cSeperator)
			{
				strTDC = strTDC.Left( nIndex);
				int nStart = strTDC.ReverseFind( '|') + 1;
				int nEnd = strTDC.ReverseFind( ':');
				if( nStart < nEnd) dFactor = atof( strTDC.Mid( nStart, nEnd - nStart));
			}
		}
	}

	switch(m_nDataInsertType)
	{
		case CPacketDataInfo::NEW:
		case CPacketDataInfo::APPEND:
		case CPacketDataInfo::PART_NEW:
		{
			//sy 2005.11.28.
			//-> 여러 packet이 length만 바꾸어서 들어올경우가 있기 때문에 length > 0 경우만 data clear!
			if( CPacketDataInfo::PART_NEW == m_nDataInsertType)
			{
				//sy 2006.03.28.
				//pPacket->Initialize(true);
				pPacket->ClearData( true);
			}

			// (2009/12/2 - Seung-Won, Bae) for KRX_CME time
			if( CChartInfo::KRX_CME == m_realCenter.GetChartMode()
				&& 0 <= pPacket->GetType().Find( "HH"))
			{
				CTimeTypeConvert ttcTrPacket;
				ttcTrPacket.SetPacketType( pPacket->GetType());
				for(int nId = 0 ; nId < p_nCount ; nId++)
				{
					__int64 nDateTime = ttcTrPacket.GetFullTypeTime( ( _int64)p_pdData[ nId]);
					nDateTime = CDataConversion::CalcTimeSum( nDateTime, 0);
					nDateTime = ttcTrPacket.GetPacketTypeTime( nDateTime);
					pPacket->AppendTrData( ( double)nDateTime);
				}
			}
			else if( 1.0 == dFactor) for(int nId = 0 ; nId < p_nCount ; nId++)
				pPacket->AppendTrData( p_pdData[ nId] * dFactor);
			else if( pPacket->GetName() == _MTEXT( C0_DATE_TIME))
			{
				CString strPacketType = pPacket->GetType();
				double dDateTime = 0.;
				int nTimeDiff = 0;
				if( 0 <= strPacketType.Find( "HH")) nTimeDiff = m_nBaseTimeDiff - m_nOrgBaseTimeDiff;
				for(int nId = 0 ; nId < p_nCount ; nId++)
				{
					dDateTime = p_pdData[ nId];
					if( 0 != nTimeDiff) dDateTime = CDataConversion::CalcTimeDiff( dDateTime, nTimeDiff, m_nBaseTimeDiff, strPacketType);
					pPacket->AppendTrData( dDateTime);
				}
			}
			else for(int nId = 0 ; nId < p_nCount ; nId++)
				pPacket->AppendTrData( p_pdData[ nId]);
			break;
		}

		case CPacketDataInfo::INSERT:
		{
			// (2009/12/2 - Seung-Won, Bae) for KRX_CME time
			if( CChartInfo::KRX_CME == m_realCenter.GetChartMode()
				&& 0 <= pPacket->GetType().Find( "HH"))
			{
				CTimeTypeConvert ttcTrPacket;
				ttcTrPacket.SetPacketType( pPacket->GetType());
				for(int nId = 0 ; nId < p_nCount ; nId++)
				{
					__int64 nDateTime = ttcTrPacket.GetFullTypeTime( ( _int64)p_pdData[ nId]);
					nDateTime = CDataConversion::CalcTimeSum( nDateTime, 0);
					nDateTime = ttcTrPacket.GetPacketTypeTime( nDateTime);
					pPacket->AppendTrData( ( double)nDateTime);
				}
			}
			else if( 1.0 == dFactor) for(int nId = p_nCount - 1; 0 <= p_nCount ; nId--)
				pPacket->InsertTrData( p_pdData[ nId] * dFactor);
			else if( pPacket->GetName() == _MTEXT( C0_DATE_TIME))
			{
				CString strPacketType = pPacket->GetType();
				double dDateTime = 0.;
				int nTimeDiff = 0;
				if( 0 <= strPacketType.Find( "HH")) nTimeDiff = m_nBaseTimeDiff - m_nOrgBaseTimeDiff;
				for(int nId = p_nCount - 1; 0 <= p_nCount ; nId--)
				{
					dDateTime = p_pdData[ nId];
					if( 0 != nTimeDiff) dDateTime = CDataConversion::CalcTimeDiff( dDateTime, nTimeDiff, m_nBaseTimeDiff, strPacketType);
					pPacket->InsertTrData( p_pdData[ nId]);
				}
			}
			else for(int nId = p_nCount - 1; 0 <= p_nCount ; nId--)
				pPacket->InsertTrData( p_pdData[ nId]);
			break;
		}

		default:
			break;
	}
	return TRUE;
}
BOOL CPacketListImplementation::SetBinTrStringData( const char *p_szPacketName, long p_nCount, const char **p_pszData)
{
	if( !m_pBinTrPacketList) return FALSE;
	if( m_lBinTrRowCount < p_nCount) m_lBinTrRowCount = p_nCount;

	if( p_nCount <= 0) return TRUE;

	CPacket *pPacket = NULL;
	POSITION psnPacket = m_pBinTrPacketList->GetHeadPosition();
	while( psnPacket)
	{
		pPacket = m_pBinTrPacketList->GetNext( psnPacket);
		if( !pPacket) continue;
		if( pPacket->GetName() == p_szPacketName) break;
		pPacket = NULL;
	}
	if( !pPacket) return FALSE;
	if( pPacket->GetLength() <= 0) return FALSE;
	if( pPacket->GetType() != _MTEXT( C6_CHARACTER)) return FALSE;

	switch(m_nDataInsertType)
	{
		case CPacketDataInfo::NEW:
		case CPacketDataInfo::APPEND:
		case CPacketDataInfo::PART_NEW:
		{
			//sy 2005.11.28.
			//-> 여러 packet이 length만 바꾸어서 들어올경우가 있기 때문에 length > 0 경우만 data clear!
			if( CPacketDataInfo::PART_NEW == m_nDataInsertType)
			{
				//sy 2006.03.28.
				//pPacket->Initialize(true);
				pPacket->ClearData( true);
			}

			for(int nId = 0 ; nId < p_nCount ; nId++) pPacket->AppendTrData( p_pszData[ nId]);
			break;
		}

		case CPacketDataInfo::INSERT:
		{
			for(int nId = p_nCount - 1; 0 <= p_nCount ; nId--) pPacket->InsertTrData( p_pszData[ nId]);
			break;
		}

		default:
			break;
	}
	return TRUE;
}

BOOL CPacketListImplementation::SetBinTrEnd( void)
{
	if( !m_pBinTrPacketList) return FALSE;

	Initialize_BaseDataAfter( m_bBinTrIsBasePacket ? true : false, m_lBinTrRowCount, m_pBinTrPacketList);
	return TRUE;
}

// 자신의 RQ : 복수종목 - ojtaso (20070117)
void CPacketListImplementation::SetRQ(LPCTSTR lpszRQ)
{
	m_lpszRQ = lpszRQ;
}

// 자신의 RQ : 복수종목 - ojtaso (20070117)
LPCTSTR CPacketListImplementation::GetRQ()
{
	return m_lpszRQ;
}

// (2007/2/7 - Seung-Won, Bae) for Exception Value
void CPacketListImplementation::SetExceptionValues( const CString &strData)
{
	CString strExceptionValues = strData;
	while(!strExceptionValues.IsEmpty())
	{
		CString strExceptionValue = CDataConversion::GetStringData(strExceptionValues, "|");
		if(strExceptionValue.IsEmpty() && !strExceptionValues.IsEmpty())
		{
			strExceptionValue = strExceptionValues;
			strExceptionValues.Empty();
		}
		CString strPacketName = CDataConversion::GetStringData( strExceptionValue, ":");
		strPacketName = _STOM0( strPacketName);
		CPacket* pPacket = GetBaseData( strPacketName);
		if( pPacket == NULL) continue;
		pPacket->SetExceptionValue(	atof( strExceptionValue));
	}
}

// (2007/2/22 - Seung-Won, Bae) for Continue Code
void CPacketListImplementation::SetContinueCodes( const CString &strData)
{
	CString strContinueCodes = strData;
	while(!strContinueCodes.IsEmpty())
	{
		CString strContinueCode = CDataConversion::GetStringData(strContinueCodes, "|");
		if(strContinueCode.IsEmpty() && !strContinueCodes.IsEmpty())
		{
			strContinueCode = strContinueCodes;
			strContinueCodes.Empty();
		}
		CString strPacketName = CDataConversion::GetStringData( strContinueCode, ":");
		strPacketName = _STOM0( strPacketName);
		CPacket* pPacket = GetBaseData( strPacketName);
		if( pPacket == NULL) continue;
		pPacket->SetContinueCode( atof( strContinueCode));
	}
}

// (2009/5/21 - Seung-Won, Bae) Shift the data of packet to future.
BOOL CPacketListImplementation::ShiftPacket( LPCTSTR p_szPacketNames, long p_szShiftCount)
{
	CString szPacketName( p_szPacketNames);
	CTypedPtrList<CObList, CPacket*> packetList;
	GetPacketList( szPacketName, ';', false, packetList);

	if( packetList.IsEmpty()) return FALSE;

	POSITION psnPacket = packetList.GetHeadPosition();
	while( psnPacket)
	{
		CPacket *pPacket = packetList.GetNext( psnPacket);
		if( !pPacket) return FALSE;
		if( !pPacket->ShiftData( p_szShiftCount)) return FALSE;
	}
	return TRUE;
}

// 현재가 등락률 - ojtaso (20080602)
void CPacketListImplementation::OffsetPacketData(double dData, CString strPacketName/* = _T("")*/)
{
	POSITION pos = m_PacketList.GetHeadPosition();
	while(pos)
	{
		CPacket * pPacket = m_PacketList.GetNext(pos);
		CString strName = pPacket->GetName();
		if(pPacket == NULL || strName != strPacketName)
			continue;

		pPacket->OffsetPacketData(dData);
	}
}

// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리
void CPacketListImplementation::SetOrgBaseTimeDifference(const CString& strData)
{
	m_nOrgBaseTimeDiff = atoi(strData);
	if( GetBaseTimeName().IsEmpty() )		// 현재 셋팅된 사용자 시간이 없는 경우만 Packet 시간대로 셋팅
		SetBaseTimeDifference(m_nOrgBaseTimeDiff);
	m_realCenter.SetTimeDiffInfo( m_nOrgBaseTimeDiff, m_nBaseTimeDiff);
}

int CPacketListImplementation::GetOrgBaseTimeDifference()
{
	return m_nOrgBaseTimeDiff;
}

// 20081007 JS.Kim	사용자시간대 선택
void CPacketListImplementation::SetBaseTimeName(CString strBaseTimeName)
{
	m_strBaseTimeName = strBaseTimeName;
}

CString CPacketListImplementation::GetBaseTimeName()
{
	return m_strBaseTimeName;
}

void CPacketListImplementation::SetBaseTimeDifference(const int nTimeDiff)
{
	m_nBaseTimeDiff = nTimeDiff;
	m_realCenter.SetTimeDiffInfo( m_nOrgBaseTimeDiff, m_nBaseTimeDiff);
}

// 사용자가 선택한 현재 시간 구하기
int CPacketListImplementation::GetBaseTimeDifference()
{
	return m_nBaseTimeDiff;
}

// Packet에 Data 넣을 때 사용할 사용자시간-원래시간 계산한 값 구하기
int	CPacketListImplementation::GetCalcDataTimeDifference()
{
	return m_nBaseTimeDiff - m_nOrgBaseTimeDiff;
}

__int64 CPacketListImplementation::GetCurrDateTime() const
{
	return m_realCenter.GetCurrDateTime();
}

bool CPacketListImplementation::GetMarketTime(int& nMarketStartTime, int& nMarketEndTime) const
{
	return m_realCenter.GetMarketTime(nMarketStartTime, nMarketEndTime);
}
	
// 20081007 JS.Kim	사용자시간대 선택
void CPacketListImplementation::CalcPacketTimeDiff(int nBaseTimeDiff)
{
	CPacket* pPacket = GetBaseData( _MTEXT( C0_DATE_TIME));
	if( pPacket )
	{
		pPacket->CalcPacketTimeDiff(nBaseTimeDiff);
	}
}

//	CPacket*	GetDatePacket();

//20081021 JS.Kim		일봉 TR의 날자는 영업일 기준이고, 현재날자는 거래소시간 기준이라 현재 날자를 영업일 날자로 변경해준다

//20081021 JS.Kim		
// 장시작시간(영업시작시간)이 종목의 거래소시간이 아닌 CME시간(시카고(GMT-06:00))으로 오므로 종목의 거래소시간으로 맞춰준다
#define		CME_TIMEDIFF		-6
//void CPacketListImplementation::CalcMarketTime( )

	// 20081024 JS.Kim	허수봉을 ocx 및 AddIn 으로 주기위해 허수봉의 시간을 실봉시간으로 환산한다.
//	bool		CalcNMTimeToRealTime(CPacket *pPacket, const double& dTime, double& dRealTime, double& dCloseData);

// 20081029 JS.Kim	영업일 처리 기준을 구한다.
CPacketType::MARKETDAYTYPE CPacketListImplementation::GetMarketDayType()
{
	return m_realCenter.GetMarketDayType();
}
//>>

// (2009/10/19 - Seung-Won, Bae) Real Data Processing Interface.
bool CPacketListImplementation::SetRealData( const char *p_szPacketName, const double dData, int &p_nRemovePacketDataCount, const int nOption)
{
	//>>[DRFN Platform 로그기능추가/로그루틴 지우지 말것] 필요시 여기에서도 확인.
// 	{
// 		CString szMessage;
// 		szMessage.Format("CPacketListImplementation::SetRealData[%s]", p_szPacketName);
// 		if(!m_pDRDebuger) _GetDRDebuger();
//		if(m_pDRDebuger) m_pDRDebuger->DRLog_Message(szMessage);
// 	}
	//<<
	p_nRemovePacketDataCount = 0;
	CPacket* pPacket = GetBaseData( p_szPacketName);
	if( !pPacket) return false;

	//<< 20100324_JS.Kim 솔로몬.FX	시고저종 외의 패킷을 시고저종처럼 사용하기 위한 처리
	if( CChartInfo::FX == m_realCenter.GetChartMode() )
	{
		if( strcmp(p_szPacketName, "_SellClose_") == 0 )
		{
			m_realCenter.m_pOpenPacket	= GetPacket("_SellOpen_", m_PacketList);
			m_realCenter.m_pHighPacket	= GetPacket("_SellHigh_", m_PacketList);
			m_realCenter.m_pLowPacket	= GetPacket("_SellLow_", m_PacketList);
		}
		else if( strcmp(p_szPacketName, "_BuyClose_") == 0 )
		{
			m_realCenter.m_pOpenPacket	= GetPacket("_BuyOpen_", m_PacketList);
			m_realCenter.m_pHighPacket	= GetPacket("_BuyHigh_", m_PacketList);
			m_realCenter.m_pLowPacket	= GetPacket("_BuyLow_", m_PacketList);
		}
		
	}
	//>>

	p_nRemovePacketDataCount = RemovePacketList( 0, GetRemovePacketCountInRealData(), 0);
	return m_realCenter.SetRealData(pPacket, dData, nOption, p_nRemovePacketDataCount);
}
bool CPacketListImplementation::GetRealData_CloseTime( const __int64 &p_nClock, __int64 &p_nRealDataTime)
{
	CPacket* pPacket = GetBaseData( _MTEXT( C0_DATE_TIME));
	if( !pPacket) return false;
	return m_realCenter.GetRealData_CloseTime( pPacket, p_nClock, p_nRealDataTime);
}
void CPacketListImplementation::PushNMTRState( const __int64 &p_nRealDataTime)
{
	m_realCenter.PushNMTRState( p_nRealDataTime);
}
void CPacketListImplementation::PopNMTRState( void)
{
	m_realCenter.PopNMTRState();
}
// (2009/11/12 - Seung-Won, Bae) Get NMTR Info
BOOL CPacketListImplementation::GetLastClockAndRealTime( __int64 &p_nLastClockTime, __int64 &p_nLastRealTime)
{
	return m_realCenter.GetLastClockAndRealTime( p_nLastClockTime, p_nLastRealTime);
}


// (2009/10/21 - Seung-Won, Bae) Comment. 실시간 Data의 Packet Type을 관리한다.
CString CPacketListImplementation::GetRealTimeType( void)
{
	return m_realCenter.GetRealTimeType();
}


// (2009/11/1 - Seung-Won, Bae) for Normalized Candle data.
double CPacketListImplementation::GetOrgDataBeforeRate( const char *p_szPacketName, const double p_dRatedData)
{
	return m_realCenter.GetOrgDataBeforeRate( p_szPacketName, p_dRatedData);
}


// (2009/11/3 - Seung-Won, Bae) Make Market Date to Real Date of WorldOn Real Data.
void CPacketListImplementation::GetRealData_RealDateTime( double &p_dData)
{
	m_realCenter.GetRealData_RealDateTime( p_dData);
}

// (2009/12/2 - Seung-Won, Bae) Notify to other modules.
void CPacketListImplementation::ChangeChartMode( int p_nChartMode)
{
	m_realCenter.ChangeChartMode( p_nChartMode);
}

//<< 20100324_JS.Kim 솔로몬.FX	시고저종 외의 패킷을 시고저종처럼 사용하기 위한 처리
// Redefine Packet Name	"REDEFPACKET"
void CPacketListImplementation::SetRedefinePacketName(const CString& strData)
{
	if( strData.IsEmpty() )		return;

	CString strRedefines = strData;
	while(!strRedefines.IsEmpty())
	{
		CString strRedefine = CDataConversion::GetStringData(strRedefines, "|");
		if(strRedefine.IsEmpty() && !strRedefines.IsEmpty())
		{
			strRedefine = strRedefines;
			strRedefines.Empty();
		}
		CString strPacketName = CDataConversion::GetStringData( strRedefine, ":");
		strPacketName = _STOM0( strPacketName);
		strRedefine = _STOM0( strRedefine);

		CPacket* pPacket = GetBaseData( strRedefine);
		if( pPacket == NULL) continue;
		pPacket->SetSubName( strPacketName );
	}
}

// Redefine된 PacketName으로 Packet을 찾는다
CPacket* CPacketListImplementation::GetRedefinedPacket(const CString& strPacketName, const CTypedPtrList<CObList, CPacket*>& packetList) const
{
	if(strPacketName.IsEmpty())
		return NULL;

	POSITION pos = packetList.GetHeadPosition();
	while(pos != NULL)
	{
		CPacket* pPacket = (CPacket*) packetList.GetNext(pos);
		if(pPacket->GetSubName() == strPacketName)
			return pPacket;
	}

	return NULL;
}
//>>

// (2011/12/09 - Junok, Lee) add function to get default chart bozo message value.
BOOL CPacketListImplementation::GetDefaultBozoValue(int p_nChartMode, LPCSTR p_szKey, CString &p_rValue)
{
	if(p_nChartMode==CChartInfo::HTS)
	{
		if(strcmp(p_szKey, "MARKETTIME")==0)
		{
			p_rValue = "MARKETTIME=090000|153000@";
			return TRUE;
		}
		return FALSE;
	}

	if(p_nChartMode==CChartInfo::KRX_CME)
	{
		if(strcmp(p_szKey, "MARKETTIME")==0)
		{
			p_rValue = "MARKETTIME=180000|050000@";
			return TRUE;
		}
		return FALSE;
	}

	if(p_nChartMode==CChartInfo::WORLD_ON)
	{
		if(strcmp(p_szKey, "BASETIME")==0)
		{
			p_rValue = "BASETIME=000000@";
			return TRUE;
		}

		return FALSE;
	}

	if(p_nChartMode==CChartInfo::FX)
	{
		if(strcmp(p_szKey, "MARKETTIME")==0)
		{
			p_rValue = "MARKETTIME=060000|060000@";
			return TRUE;
		}

		if(strcmp(p_szKey, "BASETIME")==0)
		{
			p_rValue = "BASETIME=060000@";
			return TRUE;
		}

		return FALSE;
	}
	return FALSE;
}

BOOL CPacketListImplementation::_GetDRDebuger()
{
	if(m_pDRDebuger)
		return TRUE;

	m_pDRDebuger = new CDRDebugerLoader();
	if(m_pDRDebuger->IsLoaded()==FALSE)
	{
		delete m_pDRDebuger;
		m_pDRDebuger = NULL;
		return FALSE;
	}
	return TRUE;;
}