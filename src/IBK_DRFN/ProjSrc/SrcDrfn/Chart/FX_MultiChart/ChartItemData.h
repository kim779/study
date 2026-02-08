// ChartItemData.h: interface for the CChartItemData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTITEMDATA_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
#define AFX_CHARTITEMDATA_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "./define/ChartMsgDef.h"
#include "./define/TR10450_10451.h"

#include "../COMMONTR/EU_p0602.h"
#include "../COMMONTR/EU_p0603.h"
#include "../COMMONTR/EU_p0604.h"
#include "../COMMONTR/EU_p0605.h"
#include "../COMMONTR/EU_p0606.h"
#include "../COMMONTR/EU_p0607.h"

//<<20100308_JS.Kim
////<<< 20090902 by kmlee
//#include "../COMMONTR/EU_x56000.h"
#include "../COMMONTR/SM_fx112.h"
////>>>
////>>

class CChartItem;
class CChartItemData  
{
public:
	const char* GetNextKey() { return m_chkey;	}
	const char* GetNextKeyOld() { return m_chkeyOld;	}

	CString GetMarketQueryData_ChartItemData(BOOL bNext, UINT nRqstCnt);

	BOOL	m_bNextMarketData; //시장 지표 연속조회 여부 Flag
	void	SetBNextMarketData(BOOL bNext) { m_bNextMarketData = bNext; }
	CString GetCodeForRqstFromChartItemData() {return m_strCodeForRequest;}	
	BOOL	SetPacketDataInAdditionPacket(BOOL bReset); //시장지표 TR데이터를 차트에 주기 위해 

	CString	m_strOrderType;

	//=================================================================================
	// 2005. 05. 18  add by sy, nam
	// 차트 중첩으로 인해 추가된 변수	
	int		m_nCntOverLap;
	BOOL	m_bOverLapping; 
	static CString CChartItemData::OverLapping_MakePacketOption(LPCTSTR lpPacketName,long lPacketSize,LPCTSTR lpPacketType, BOOL bUseInReal, int nCntOverLap, BOOL bChartFilled = TRUE, BOOL bOverLapping = FALSE);
    //=================================================================================

	CChartItemData(CChartItem *pChartItem,CString m_strTRCode);
	void Init(CChartItem *pChartItem, CString strTRCode);
	virtual ~CChartItemData();
	void SetCodeForQuery(LPCTSTR lpCode, BOOL bInitKey = TRUE);
	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	stSend2Server* GetQueryData(BOOL bNext, ST_SELCHART* lpSelChart = NULL);
	BOOL MakeChartData(void* szData, long nLength, ST_SELCHART* lpSelChart = NULL);
	CString GetTrCode();
	// 선택된 차트의 시장 세팅 : 복수종목 - ojtaso (20070215)
	void SetTRCode(LPCTSTR lpszTRCode);
	long GetLTrCode();
	CString GetCode() { return m_strCode;}
	long	GetCnt() { return m_lCnt;}
	BOOL AddPacketAndGraph(BOOL IsNoVolumeGraph = FALSE);
	BOOL AddPacketAndGraph2(BOOL bAddGraph = TRUE, BOOL bOverlapp = FALSE);		// 새로운 패킷,차트 추가 : 복수종목 - ojtaso (20061224)
	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	BOOL AddLoadedPacket(LPCTSTR lpszRQ, ST_SELCHART* lpSelChart);
	
	CString GetPacketName();
	void RemoveRealItem();
	void AddRealItem();
	BOOL SetPacketData(BOOL bReset);
	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	BOOL SetLoadedPacketData(LPCTSTR lpszRQ, ST_SELCHART* lpSelChart);
	BOOL IsFilled();
	void ClearData();
	CString GetChartItemDataTitle(long nIndex = 0);
	long GetCntChartItemDataTitle();
	BOOL ClearChartItemData();
	long GetMaximumRequestCnt();
	BOOL ProcRequestCntAfterQuery(long lCntReceived);
	BOOL RemovePacketAndGraph();
	
	// 시장이 바뀌면 PacketName도 바꾼다 : 복수종목 - ojtaso (20070711)
	void ChangePacketNameArray(LPCTSTR lpszTRCode);
	void SetChartNumericValue(int nValue);

	void GetOHLValue(CString &strOpen, CString &strHigh, CString &strLow);

	static CString MakePacketOption(LPCTSTR lpPacketName,long lPacketSize,LPCTSTR lpPacketType, BOOL bUseInReal, int nCntOverLap, BOOL bChartFilled = FALSE, BOOL bOverLapping = FALSE);
	static CString MakeGraphOption(LPCTSTR lpChartType,LPCTSTR lpChartName, CStringArray& rArrayPacketList, LPCTSTR lpChartTitle, long lChartStyle = 0,
									COLORREF clrGrp1 = RGB(218,46,46),COLORREF clrGrp2 = RGB(218,46,46),
									COLORREF clrGrp3 = RGB(46,46,218),COLORREF clrGrp4 = RGB(46,46,218),
									COLORREF clrGrp5 = RGB(218,46,46),COLORREF clrGrp6 = RGB(218,46,46),
									BOOL bUpFilled = 0, BOOL bDownFilled = 1,
									long lUpLineWidth = 1, long lDownLineWidth = 1, BOOL bHideGrp = 0);

private:
	CChartItem		*m_pChartItem;			// Parent Class;
	BOOL			m_bInitalChart;			// 초기화여부
	CString			m_strCodeForRequest;	// 코드 (데이터조회를 위한 코드..)
	CString			m_strCode;				// 실제 받은 데이터의 코드

	// TR-CODE - 05. 05. 06 추가 - 기존 지존의 TR코드는 숫자형이지만 
	// 한화의 TR 코드는 "HWS + 숫자" 형태이기 때문에 CString 형으로 쓸수 있도록 추가한다.
	CString			m_strTRCode;			
	
	CString			m_strName;
	char			m_chkey[ChartKeySize+1];//[14];		// 다음 / 이전 Fetch Key	
	char			m_chkeyOld[ChartKeySize+1];//[14];		// 다음 / 이전 Fetch Key		
	long			m_lCnt;				// 받은 전체 데이터 수
	long			m_lCntTemp;			// 받은 전체 데이터 수(임시)..
	long			m_lStartDate;
	long			m_lEndDate;
	CString			m_strOpen;			//	시가
	CString			m_strHigh;			//	고가
	CString			m_strLow;			//	저가
	CString			m_strPrice;			//	현재가
	CString			m_strSign;			//	부호
	CString			m_strChange;		//	대비
	CString			m_strChrate;		//	대비율
	CString			m_strVolume;		//	거래량
	CString			m_strPreprice;		//	기준가
	CString			m_strKey;			//	이전키

	CString			m_strPosition;		//	소수점 자리 단위

	long			m_lDataSize;
	CStringArray	m_strArrayRealItemRegistered;

public:
	SM_fx112_CHART*	m_pfx112_ChartData;		// 챠트데이타 - FX

	CStringArray	m_strArrayPacketName;
	CStringArray	m_strArrayGraphTitle;
	BOOL			m_bIsFilled;
	long			m_lCntRequired;
	long			m_lCntRequiredByPrim;
	long			GetDataSize() { return m_lDataSize;}
	long			GetData();
	CString*		GetPstrPacketName(long lPosByOutput, long lPosByInput, CString *pstrPacketName,CString *pstrPacketName1);
	long			GetCntForQuery(BOOL bNext);
	BOOL			RemoveData();

	BOOL			MaketOutputData(void* szData, long nLength, ST_SELCHART* lpSelChart = NULL);
	BOOL			MakeChartData_EU_p0602(void* szData, long nLength, ST_SELCHART* lpSelChart = NULL);

	//<<< 20090902 by kmlee
	stSend2Server*	GetQueryData_EU_x56000 (BOOL bNext, ST_SELCHART* lpSelChart = NULL);	//QUERY_STRJONGMOK
	//>>>
	
	CString			GetQueryDataMarket(BOOL bNext, int nRqstCnt);

	CString			GetReciveValue(short nType);
	CString			GetPipLowest();

	void AddGraphPriceChart(void);	// JSR 20060122 ADD For Making Price Chart
	void AddGraphPriceChart2NewItem(int& nRow, int& nCol, BOOL bAddGraph);	// 새로운 차트 추가 : 복수종목 - ojtaso (20061224)
	void AddGraphPriceChart_Overlapp(int& nRow, int& nCol);
	//2006.08.28 - KHD
public:
	BOOL AddPacketAndGraphAnal();
	// 복수종목 불러오기 : 복수종목 - ojtaso (20070528)
	BOOL m_bResetChartCfgWith;

	void OnSelectChartItem(ST_SELCHART* lpSelChart = NULL);

};

#endif // !defined(AFX_CHARTITEMDATA_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
