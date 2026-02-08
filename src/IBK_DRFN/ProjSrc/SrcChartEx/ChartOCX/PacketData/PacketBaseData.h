// PacketBaseData.h: interface for the CPacketBaseData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKETBASEDATA_H__E4CD4DBB_D4D4_4208_9E94_68873165E11F__INCLUDED_)
#define AFX_PACKETBASEDATA_H__E4CD4DBB_D4D4_4208_9E94_68873165E11F__INCLUDED_

#include <afxtempl.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CItem
{
public:
	CItem& operator=(const CItem& data);

public:
	CItem(const CString& strCode = "", const CString& strName = "");

	void Clear();
	void SetItem(const CString& strCode, const CString& strName);

	CString GetCode() const;
	CString GetName() const;

private:
	CString m_strCode; //코드번호
	CString m_strName; //종목명
};


// ============================================================================
class CDateData
{
public:
	CDateData( void);

// (2009/10/20 - Seung-Won, Bae) Comment. 오늘의 일자를 관리한다. (거래소 일자)
protected:
	CString		m_strPacketType;
	int			m_nDate;
	CString		m_strPacketTypedDate;
	int			m_nPacketTypedDate;
	int			m_nPacketTypeTimeFactor;				// Packet Type에 따라 일시를 구성시 곱해야 하는 수 (시간자리수)
public:
				//	PC의 오늘 일자로 초기화한다.
				//		Contructor에서 기본 구성시.
				//		보조메시지 설정전 초기화 처리시.
				//		다음 설정시 지정값이 없는 경우.
				//			보조메시지 DATE 값.
				//			보조메시지 DATATIME 값.
				//			보조메시지 DATATIME 값으로 받은 거래소 일자를 기준으로 재산정한 영업일.
				//				영업일은 지역시간 0시를 기준으로 바뀌며, 거래소 시간으로 일중에 변경된다.
				//				지역시간을 기준으로한 영업일은 일/주/월봉의 산출 기준이 된다.
	void		SetDate( void);
				// (2009/10/20 - Seung-Won, Bae) Comment. 지정된 설정값으로 일자로 초기화한다.
				//		보조메시지 DATE 값.
				//		보조메시지 DATATIME 값.
				//		보조메시지 DATATIME 값으로 받은 거래소 일자를 기준으로 재산정한 영업일.
				//				영업일은 지역시간 0시를 기준으로 바뀌며, 거래소 시간으로 일중에 변경된다.
				//				지역시간을 기준으로한 영업일은 일/주/월봉의 산출 기준이 된다.
	void		SetDate( const CString& strDate); //"YYYYMMDD" 형태여야 함!!
				// (2009/10/21 - Seung-Won, Bae) Comment. 자료일자 PacketType을 지정받아 해당 Format에 맞는 Date를 별도로 관리한다.
	void		SetPacketType( const CString& strType);
				// (2009/10/20 - Seung-Won, Bae) Comment. 자료일자 PacketType형의 오늘 일자를 구한다.
	CString		GetPacketTypedDateString( void) const		{	return m_strPacketTypedDate;	}
	int			GetPacketTypedDateInt( void) const			{	return m_nPacketTypedDate;		}
				// (2009/10/20 - Seung-Won, Bae) Comment. Full Type의 오늘 일자를 구한다.
	int			GetFullDate( void) const					{	return m_nDate;					}
				// Packet Type에 따라 일시를 구성시 곱해야 하는 수 (시간자리수)
				//	1		: No HH ~ YYYYMM or YYYYMMDD
				//	100		: "~HH"
				//	10000	: "~HHMM"
				//	1000000	: "~HHMMSS"
	int			GetPacketTypeTimeFactor( void) const		{	return m_nPacketTypeTimeFactor;	}
};


class CTimeData
{
public:
	CTimeData& operator=(const CTimeData& data);

public:
	CTimeData(const int nTime, const bool bTimeFromServer);

	void SetTime(const int nTime, const bool bTimeFromServer);
	
	bool IsTimeFromServer() const;
	int GetTimeFromServer() const;
	bool GetTimeFromServer(int& nTime) const;
	bool GetTimeFromServer(CString& strTime) const;

private:
	int m_nTime; // HHMMSS 형태의 시간이다.
	bool m_bTimeFromServer;
};


// (2009/10/20 - Seung-Won, Bae) Comment. 장시작 및 마감시간을 관리한다.
class CMarketTimeData
{
public:
	CMarketTimeData();

private:
	CTimeData m_startTime;	// 장시작시간
	CTimeData m_endTime;	// 장마감시간

public:
			// (2009/10/20 - Seung-Won, Bae) 장시작시간을 지정한다.
	void	SetMarketStartTime( const int nMarketStartTime, const bool bTimeFromServer = true);
			// (2009/10/20 - Seung-Won, Bae) 장마감시간을 지정한다.
	void	SetMarketEndTime( const int nMarketEndTime, const bool bTimeFromServer = true);
			// 장시작/마감 시간을 가져오기.
			// return : true 인 경우는 서버로 time를 받은 경우에 해당된다.
	bool	GetMarketTime( int& nMarketStartTime, int& nMarketEndTime) const;
			// 장시작 시간을 구한다.
	bool	GetMarketStartTime( CString& strMarketStartTime) const;
			// (2009/11/3 - Seung-Won, Bae) Make Market Date to Real Date of WorldOn Real Data.
	bool	IsReverseTime( void) const		{	return m_endTime.GetTimeFromServer() <= m_startTime.GetTimeFromServer();		}
};


// ============================================================================
class CRealRateData
{
public:
	CRealRateData& operator=(const CRealRateData& data);

public:
	CRealRateData( HWND p_hOcxWnd, const CString& strData = "");

	void SetRealRateData(const CString& strData, const CString& strCompart = ':');
	void SetRealRateData_PartChange(const CString& strData, const CString& strCompart = ':');
	void SetRealRate(const double dRealRate);
	void SetPacketNames(const CString& strPacketNames, const CString& strCompart = ',');

	bool DoesUsingRealRate() const;
	bool DoesPacketName(const CString& strPacketName) const;
	double GetRealRate() const;
	int GetRoundIndex() const;
	CList<CString, CString>* GetPacketNameList() const;

	bool GetApplyDataInRealRate(const CString& strPacketName, const double dOrgData, double& dNewData) const;
	// 2008.10.24 JS.Kim	Packet의 데이타를 원 데이타로 복구
	bool GetOrgDataInRealRate(const CString& strPacketName, const double dOrgData, double& dNewData) const;

private:
	void Initialize();

	bool AddTail_PacketName(const CString& strPacketName);

	void SetRealRate(CString& strData, const CString& strCompart);
	void SetRountIndex(CString& strData, const CString& strCompart);
	void SetPacketNameList(CString& strData, const CString& strCompart);
	void SetPacketNameList(const CList<CString, CString>* pPacketNameList);

	CString GetData(CString& strData, const CString& strCompart) const;

	double GetApplyDataInRealRate(const double dOrgData) const;
	CString GetApplyDataInRealRate(const CString& strOrgData) const;
	CString GetApplyDataInRealRate(const CString& strPacketName, const CString& strOrgData) const;
	// 2008.10.24 JS.Kim	Packet의 데이타를 원 데이타로 복구
	double GetOrgDataInRealRate(const double dRealRateData) const;

	bool DoesBringPacketNameList(const CString& strPacketName) const;

private:
	double m_dRealRate;	// 비율
	int m_nRoundIndex;	// 반올림자리
	CList<CString, CString> m_packetNameList;	//비율을 적용할 패킷명

// (2008/1/2 - Seung-Won, Bae) Support MultiLanguage Version.
protected:
	HWND		m_hOcxWnd;
};


// ============================================================================
// TR data 가 들어오는 갯수를 list로 저장 (0 보다 클 경우만 저장)
class CTRDataCountList
{
public:
	void Initialize();

	bool AddTRData(const int nCount);

	int GetTotalTRIndex() const;
	int GetTotalTRCount() const;
	int GetTRCount(const int nIndex) const;
	int GetTRCount_Tail() const;
	
	int GetRequestTRCount(const int nCurTRCount) const;

private:
	int GetTRIndex(const int nCurTRCount) const;
	int GetTotalTRCount(const int nEndIndex) const;

private:
	int m_nTotalCount;
	CList<int, int> m_dataCountList;
};

// ============================================================================
class CItemData
{
public:
	CItemData& operator=(const CItemData& data);

public:
	CItemData(const bool bUsing, const double& dData);

	void Set(const bool bUsing, const double& dData);
	void SetUsing(const bool bUsing);
	void SetData(const double& dData);

	bool IsUsing() const;
	double GetData() const;

private:
	bool m_bUsing;
	double m_dData;
};

#endif // !defined(AFX_PACKETBASEDATA_H__E4CD4DBB_D4D4_4208_9E94_68873165E11F__INCLUDED_)
