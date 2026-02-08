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

#include "../COMMONTR/KB_p0602.h"
#include "../COMMONTR/KB_p0603.h"
#include "../COMMONTR/KB_p0604.h"
#include "../COMMONTR/KB_d1013.h"
#include "../COMMONTR/KB_p0606.h"

class CChartItem;
class CChartItemData  
{
public:
	CShareInputData* m_pShareInputData;
	const char* GetNextKey() { return m_chkey;	}
	const char* GetNextKeyOld() { return m_chkeyOld;	}

	CString GetMarketQueryData_ChartItemData(BOOL bNext, UINT nRqstCnt);

	BOOL	m_bNextMarketData; //시장 지표 연속조회 여부 Flag
	void	SetBNextMarketData(BOOL bNext) { m_bNextMarketData = bNext; }
	CString GetCodeForRqstFromChartItemData() {return m_strCodeForRequest;}	
	BOOL	SetPacketDataInAdditionPacket(BOOL bReset); //시장지표 TR데이터를 차트에 주기 위해 

//	CString m_str10450PacketName;
//	CString m_str10451PacketName;
	

	//=================================================================================
	// 2005. 05. 18  add by sy, nam
	// 차트 중첩으로 인해 추가된 변수	
	int		m_nCntOverLap;
	BOOL	m_bOverLapping; 
	static CString CChartItemData::OverLapping_MakePacketOption(LPCTSTR lpPacketName,long lPacketSize,LPCTSTR lpPacketType, BOOL bUseInReal, int nCntOverLap, BOOL bChartFilled = TRUE, BOOL bOverLapping = FALSE);
    //=================================================================================

	//CChartItemData(CChartItem *pChartItem,long m_lTrCode);
	CChartItemData(CChartItem *pChartItem,CString m_strTRCode);
	void Init(CChartItem *pChartItem, CString strTRCode);
	virtual ~CChartItemData();
	void SetCodeForQuery(LPCTSTR lpCode, BOOL bInitKey = TRUE);
	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	CString GetQueryData(BOOL bNext, ST_SELCHART* lpSelChart = NULL);
	BOOL MakeChartData(void* szData, long nLength, ST_SELCHART* lpSelChart = NULL);
	CString GetTrCode();
	// 선택된 차트의 시장 세팅 : 복수종목 - ojtaso (20070215)
	void SetTRCode(LPCTSTR lpszTRCode);
	long GetLTrCode();
	CString GetCode() { return m_strCode;}
	long	GetCnt() { return m_lCnt;}
	BOOL AddPacketAndGraph(BOOL IsNoVolumeGraph = FALSE);
	BOOL AddPacketAndGraph2(BOOL bAddGraph = TRUE, BOOL bOverlapp = FALSE, BOOL bSacle=TRUE);		// 새로운 패킷,차트 추가 : 복수종목 - ojtaso (20061224)
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

	//05. 06. 22 by sy, nam ==============================
	//void SetUseRateValue(BOOL bUse);
	void SetChartNumericValue(int nValue);
	//============================================================

	//static CString MakePacketOption(LPCTSTR lpPacketName,long lPacketSize,LPCTSTR lpPacketType, BOOL bUseInReal);
	static CString MakePacketOption(LPCTSTR lpPacketName,long lPacketSize,LPCTSTR lpPacketType, BOOL bUseInReal, int nCntOverLap, BOOL bChartFilled = FALSE, BOOL bOverLapping = FALSE);
	static CString MakeGraphOption(LPCTSTR lpChartType,LPCTSTR lpChartName, CStringArray& rArrayPacketList, LPCTSTR lpChartTitle, long lChartStyle = 0,
									COLORREF clrGrp1 = RGB(218,46,46),COLORREF clrGrp2 = RGB(218,46,46),
									COLORREF clrGrp3 = RGB(46,46,218),COLORREF clrGrp4 = RGB(46,46,218),
									COLORREF clrGrp5 = RGB(218,46,46),COLORREF clrGrp6 = RGB(218,46,46),
									BOOL bUpFilled = 0, BOOL bDownFilled = 1,
									long lUpLineWidth = 1, long lDownLineWidth = 1, BOOL bHideGrp = 0);

	char m_chCompareChartType;
	void SetCompareChartType(char cType) { m_chCompareChartType = cType; }

private:
	CChartItem		*m_pChartItem;			// Parent Class;
	BOOL			m_bInitalChart;			// 초기화여부
	CString			m_strCodeForRequest;	// 코드 (데이터조회를 위한 코드..)
	CString			m_strCode;				// 실제 받은 데이터의 코드
	//long			m_lTrCode;				// TR-CODE

	// TR-CODE - 05. 05. 06 추가 - 기존 지존의 TR코드는 숫자형이지만 
	// 한화의 TR 코드는 "HWS + 숫자" 형태이기 때문에 CString 형으로 쓸수 있도록 추가한다.
	CString			m_strTRCode;			
	
	CString			m_strName;
	char			m_chkey[512];		// 다음 / 이전 Fetch Key	
	char			m_chkeyOld[512];	// 다음 / 이전 Fetch Key		
	long			m_lCnt;				// 받은 전체 데이터 수
	long			m_lCntTemp;			// 받은 전체 데이터 수(임시)..
	long			m_lStartDate;
	long			m_lEndDate;
//@유진삭제	S33100_CHART*	m_pSt33100Data;		//JONGMOK
//@유진삭제	S35100_CHART*	m_pSt35100Data;		//UPJONG
//@유진삭제	S36100_CHART*	m_pSt36100Data;		//FUTOPT
/*
	EU_p0602_CHART_COMP*	m_pEu0602_ChartData;		// 주식
	EU_p0603_CHART_COMP*	m_pEu0603_ChartData;		// 업종
	EU_p0604_CHART_COMP*	m_pEu0604_ChartData;		// 선옵
	EU_p0605_CHART_COMP*	m_pEu0605_ChartData;		// 주식선옵
*/

	KB_p0602_CHART_COMP*	m_pKB0602_ChartData;		// 주식
	KB_p0603_CHART_COMP*	m_pKB0603_ChartData;		// 업종
	KB_p0604_CHART_COMP*	m_pKB0604_ChartData;		// 선옵
	KB_d1013_CHART_COMP*	m_pKB1013_ChartData;		// 주식선옵
	KB_p0606_CHART_COMP*	m_pKB0606_ChartData;		// 해외

	char*			m_pSt10450Data;		
	char*			m_pSt10451Data;
	char*			m_pSt10452Data;

	S03040_CHART*	m_pSt03040Data;		//FOREIGN
	S01007_CHART*	m_pSt01007Data;		//ELW

	long			m_lDataSize;
	CStringArray	m_strArrayRealItemRegistered;
	public:
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

	BOOL			MakeChartData_KB_p0602(void* szData, long nLength, ST_SELCHART* lpSelChart = NULL);
	BOOL			MakeChartData_KB_p0603(void* szData, long nLength, ST_SELCHART* lpSelChart = NULL);
	BOOL			MakeChartData_KB_p0604(void* szData, long nLength, ST_SELCHART* lpSelChart = NULL);
	BOOL			MakeChartData_KB_d1013(void* szData, long nLength, ST_SELCHART* lpSelChart = NULL);
	BOOL			MakeChartData_KB_p0606(void* szData, long nLength, ST_SELCHART* lpSelChart = NULL); //해외종목
	BOOL			MakeChartDataELW(void* szData, long nLength, ST_SELCHART* lpSelChart = NULL); //ELW		01007

	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	CString			GetQueryData_KB_p0602(BOOL bNext, ST_SELCHART* lpSelChart = NULL);	//QUERY_STRJONGMOK
	CString			GetQueryData_KB_p0603(BOOL bNext, ST_SELCHART* lpSelChart = NULL);	//QUERY_STRUPJONG
	CString			GetQueryData_KB_p0604(BOOL bNext, ST_SELCHART* lpSelChart = NULL);	//QUERY_STRFUTOPT
	CString			GetQueryData_KB_d1013(BOOL bNext, ST_SELCHART* lpSelChart = NULL);	//QUERY_STRSTOCKFUTOPT
	CString			GetQueryData_KB_p0606(BOOL bNext, ST_SELCHART* lpSelChart = NULL);	//QUERY_STRFOREIGN

	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
//	CString			GetQueryDataELW(BOOL bNext, ST_SELCHART* lpSelChart = NULL);			//ELW		01007

	CString			GetQueryDataMarket(BOOL bNext, int nRqstCnt);

	void AddGraphPriceChart(void);	// JSR 20060122 ADD For Making Price Chart
	void AddGraphPriceChart2NewItem(int& nRow, int& nCol, BOOL bAddGraph);	// 새로운 차트 추가 : 복수종목 - ojtaso (20061224)
	//void AddGraphPriceChart_Overlapp(int& nRow, int& nCol);
	void AddGraphPriceChart_Overlapp(int& nRow, int& nCol, BOOL bSacle=TRUE);
	//2006.08.28 - KHD
public:
	BOOL AddPacketAndGraphAnal();
	// 복수종목 불러오기 : 복수종목 - ojtaso (20070528)
	BOOL m_bResetChartCfgWith;

	void OnSelectChartItem(ST_SELCHART* lpSelChart = NULL);

};

#endif // !defined(AFX_CHARTITEMDATA_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
