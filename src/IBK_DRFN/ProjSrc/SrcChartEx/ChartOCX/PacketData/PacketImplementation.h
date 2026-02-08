// PacketImplementation.h: interface for the CPacketImplementation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKETIMPLEMENTATION_H__E24ABB33_A837_49DE_AA47_48DF59595FFE__INCLUDED_)
#define AFX_PACKETIMPLEMENTATION_H__E24ABB33_A837_49DE_AA47_48DF59595FFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <afxtempl.h>
#include "BlockBaseData.h"
#include "PacketBaseData.h"		// CItemData
#include "PacketBase.h"

class CPacket;
class CPacketList;
class CPacketImplementation  
{
public:	
	CPacketImplementation(CPacket* pPacket,
			CPacketList * pParentList,
			CString strName, 
			CString strFieldName, 
			CString strType,
			CString szExpressionType,
			int nDataLength, 
			bool bUseReal,
			int nStartPeriod = 0,
			CString strRealFieldName = "");
	virtual ~CPacketImplementation();

	//////////Data Min/Max 관련 메소드////////////////////
	bool RestoreMax();
	bool RestoreMin();
	bool RestoreMinMax();

	bool SetMax(const double nMax);
	bool SetMin(const double nMin);
	bool SetMinMax(const double nMin, const double nMax);
	int SetMinMax(CPacketDataInfo::DATAINSERTTYPE nInsertType = CPacketDataInfo::NEW, CPoint ptDataRange = CPoint(0, 0));
	// (2009/1/17 - Seung-Won, Bae) Add without-Zero option.
	int GetMinMax(int nStart, int nEnd, double& nMin, double& nMax, BOOL p_bWithoutZero = FALSE);
//	double GetMin() const;
//	double GetMax() const;
	///////////////////////////////////////////////////////

	
	void SetBaseData(const CString& strType, const CString& strExpressionType, const int nStartPeriod, const bool bIsAutoMinMax = true);
	void SetYesOrNoBaseData(const bool bBaseData);
	void SetAcceptFromServer(const bool bAcceptFromServer);

	bool IsTRData() const;
	bool IsAcceptFromServer() const;
	bool IsZeroBasePacket() const;
	bool IsChangeRealData() const;
	//	Packet에 넣을 한 data의 길이
	int GetLength();
	//	data의 갯수 return
	int GetCount() const;
	int GetDataCount();
	//  전체 데이타중 가장 긴것의 길이
	int GetMaxCount();
	//	CPacket의 FieldName을 얻는다 (맵 디자인시 지정한 이름)
	CString GetFieldName();
	//	CPacket의 Name을 얻는다 (기존의 이름 or 사용자가 정의한 이름)
	CString GetName();
	//nami 9.30
	CPacketList* GetPacketList();
	CPacket* GetDatePacket();
	CPacketType::COMMODITY_BOND_NAME GetCommodityBondName() const;
	void SetCommodityBondName(const CPacketType::COMMODITY_BOND_NAME eCommodityBondName);
	// fail : -1  success : index of data
	// 분차트 근접시간 적용 - ojtaso (20080811)
	int GetIndexFromData(double nData, CString strType = _T(""));
	// return : true -> 범위 안인 경우
	//			false -> 범위를 벗어나거나 같은 값이 없는 경우
	bool GetCurOrPrevIndexFromData( const double dData, int& nCurOrPrevIndex, double& dCurOrPrevIndexData) const;
	bool GetData(const int nIndex, double& dData) const;
	// fail : ERRORVALUE   success : data of index
	double GetData(int nIndex);
	CString GetStrData(int nIndex);
	// 과거와 미래 시제의 시간까지도 return한다
	// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가 <<
	//int GetDataPastandFuture(int nIndex);
	double GetDataPastandFuture(int nIndex);
	bool GetDataPastandFuture(const int nIndex, double& dData);
	int GetTimeInterval();
	int GetDateUnitType();
	CString GetRealKeyName();
	CString GetRealFieldName();
	bool GetUseReal();
	bool IsUseStandardValue( const int p_nType) const;		// 0:Base,	1:Upper Base,	2:Lower Base
	double GetStandardValue( const int p_nType);			// 0:Base,	1:Upper Base,	2:Lower Base
	int GetStartPeriod();
	void SetRealDataGap( const double &p_dDataGap);
	bool GetRealDataGap( double& dDataGap) const;
	//nami 9.29
	CString GetType(const bool bIsFullType = true);
	double GetTypeFormatDouble(const bool bIsFullType = true);
	const CList<double, double>* GetnumericDataList() const;
	CList<double, double>* GetnumericDataList();
	CList<CString, CString>* GetstrDataList();
	

	//	CPacket이 실시간 받는 Packet일 경우 실시간 data Field Name을 세팅한다
	void SetRealFieldName(CString strRealFieldName, CString strRealKeyName);
	void SetBoundLimit(double nUpperBound, double nLowerBound);
	void SetStandardValue( const char *p_szValue);
	void SetStartPeriod(int nStartPeriod);
	void SetName(CString strName);
	void SetType(const CString& strType);
	void SetType(const CString& strFullType, const CString& strExpressionType);
	void SetLength(int nLength);

	//	CPacket내의 data를 주어진 data로 바꾼다 list의 data값은 모두 copy된다
	void ChangeData(CList<double, double>& lsnewData);
	void ChangeData(CList<CString, CString>& lsnewData);

	//	String 형태로 들어온 data를 data list에 insert (head에 해당)
	void InsertData(CString strData);	
	//	주소로 들어온 data를  읽어서 double형으로 data list에 insert (head에 해당)
	void InsertData(long pData, int nDataLength);
	//	주소로 들어온 data를  읽어서 double형으로 data list에 append (tail에 해당)
	void AppendData(long pData, int nDataLength);
	//	String 형태로 들어온 data를 double형으로 바꾸어서 data list에 append (tail에 해당)
	void AppendData(const CString& strData);
	void AppendData(double nData, int nIndex = -1);
	//	List 형태로 들어온 data를 data list에 insert (head에 해당)
	void AppendData(CList<double, double>& lsnewData);	
	void AppendData(CList<CString, CString>& lsnewData);

	void UpdateData(const double nData);
	void UpdateData(const double nData, const int nIndex, const bool bIsCurrentData = true);
	void UpdateData(const CString& strData);
	void UpdateData(const CString& strData, const int nIndex, const bool bIsCurrentData = true);
//	void UpdateData(CList<double, double>& lsnewData, CPoint ptUpdateRange);
	void UpdateData(CList<CString, CString>& lsnewData, CPoint ptUpdateRange);


	bool RemoveData(int nStart, int nEnd);
	int	 RemoveBaseData(const double nCount);
	int RemoveFictionData(const double dReallastDate, const bool bDate = false);
	// CPacket내의 data를 clear함
	int ClearData(const bool bClearIncludingTR = false);
	void Initialize(const bool bClearIncludingTR);

public:
	//sy 2005.07.11. -> 임시 값을 넣을수 있는 list
	void AddTailTempList(const double& dData);
	void RemoveAllTempList();

	bool SetTempListData(const int nIndex, const double& dData, const bool bIsAutoTail = true);
	bool GetTempListData(const int nIndex, double& dData) const;
	bool GetHeadTempListData(double& dData) const;
	bool GetTailTempListData(double& dData) const;
	CList<double, double>* GetTempList() const;

private:
	CPacket* m_pPacket;

	//////////Data Min/Max 관련//////////
	double m_dMaxBound;
	double m_dMinBound;	
	bool m_bUseBound;
	//////////////////////////////////////

	bool m_bBaseData;
	bool m_bAcceptFromServer; //sy 2005.11.08.
	int m_nStartPeriod;
	int m_nLength;

	// (2007/1/14 - Seung-Won, Bae) Support Upper and Lower Base Data.
	CItemData m_standardValue;
	CItemData m_upperBaseValue;
	CItemData m_lowerBaseValue;

	bool m_bUseReal;
	//속성페이지에서 설정한 타입 "*0.01"외.
	CString m_Type;
	//scale, title type
	//nami 8.29
	CString m_ExpressionType;
	//sy 2003.3.19. 
	// -> 누적data의 실시간 적용을 위한 이전 데이터("기본거래량", "누적거래량")
	CItemData m_realDataGap;
	//sy 2005.08.05. 장구분(주식/업종/선물/옵션)
	CPacketType::COMMODITY_BOND_NAME m_eCommodityBondName;

	CString m_FieldName;
	CString m_Name;
	CString m_RealFieldName;
	CString m_RealKeyName;
	CList <double, double> m_lnData;
	CList <CString, CString> m_lstrData;
	CPacketList * m_pParentList;

	//sy 2005.07.11. -> 임시 값을 넣을수 있는 list
	CList<double, double> m_dTempDataList;

	// 20080908_JS.Kim	8진법/32진법 표시 적용
	int		m_nBase;
	int		m_nDecLen;		// 채권가격표시에서 ' 다음 숫자의 길이
	int		m_nDecLen2;		// 채권가격표시에서 . 다음 숫자의 길이

private:
	// data의 Minimun & Maximum 값을 구해서 저장함
	// SetMinMax에서 datatiype이 add와 insert인 경우에 호출된다
//	int ResetMinMax(int nStart, int nEnd, double& nMi, double& nMa);
//	void ResetMinMax(const double &dValue);

	int GetIndexOfFuture(const double dData);
	int GetIndexOfPast(const double dData);
	int GetIndexInCaseTypeIsnotBun(const double dData);
	int GetIndexInCaseTypeIsnotBun_CurData(const double dData);
	// 분차트 근접시간 적용 - ojtaso (20080811)
	int GetIndexInCaseTypeIsBun(double dData, CString strType = _T(""));
	// 분차트 근접시간 적용 - ojtaso (20080811)
	int GetIndexInCaseTypeIsBun_CurData(const double dData, CString strType = _T(""));
	void GetComparisionDatas(const double dData, int& HeadNum, int& tailNum, int& dataNum);

	bool GetCurOrPrevIndexFromData_Date(const double& dData, double& dCurOrPrevIndexData, int& nCurOrPrevIndex) const;

	// Find Index
	POSITION FindPosition(const int nIndex) const;
	POSITION FindStrPosition(const int nIndex) const;

	int RemoveDateData(const double dReallastDate);

	bool GetRealData_UpdateData(const double& dData, double& dRealData);
	double GetRealData_AppendData(const double& dData);

	bool IsDataInPacketType(const double& dData) const;
	bool IsDataInPacketType_Date(const double& dData) const;
	bool IsDataInPacketType_Time(const double& dData) const;
	int GetTypePartData(const double& dOrgData, const CString& strType) const;

// (2006/12/20 - Seung-Won, Bae) Support Binary TR Data Receiving
public:
	void	AppendTrData( const char *p_szData);
	void	AppendTrData( const double dData);
	void	InsertTrData( const char *p_szData);
	void	InsertTrData( const double dData);

// (2007/2/7 - Seung-Won, Bae) for Exception Value
protected:
	double	m_dExceptionValue;
public:
	void SetExceptionValue( const double &p_dExceptionValue);
	double GetExceptionValue( void) const	{	return m_dExceptionValue;	}

// (2007/2/22 - Seung-Won, Bae) for Continue Code
protected:
	double m_dContinueCode;
public:
	void SetContinueCode( const double &p_dContinueCode);
	double GetContinueCode( void) const	{	return m_dContinueCode;	}

// (2007/2/21 - Seung-Won, Bae) Support Not-Auto Scale Type.
protected:
	BOOL	m_bAutoScaleType;
	CString	m_strScaleType;
public:
	void	SetAutoScaleType( BOOL p_bAutoScaleType)	{	m_bAutoScaleType = p_bAutoScaleType;	}
	BOOL	IsAutoScaleType( void)						{	return m_bAutoScaleType;				}
	void	SetScaleType( const CString &p_strScaleType)	{	m_strScaleType = p_strScaleType;	}
	CString	GetScaleType( void)								{	return m_strScaleType;				}
	double	GetScaleTypeFormatDouble( void);

	// 현재가 등락률 - ojtaso (20080602)
	void	OffsetPacketData(double dData);

	// 20080908 김진순	8진법/32진법 표시 적용
	void	SetBaseNumber(int nBase)	{	m_nBase = nBase;	}
	int		GetBaseNumber()				{	return m_nBase;		}
	void	SetDecLen(int nDecLen, int nDecLen2)	{	m_nDecLen = nDecLen;	m_nDecLen2 = nDecLen2;	}
	int		GetDecLen(int &nDecLen, int &nDecLen2)	{	nDecLen = m_nDecLen;	nDecLen2 = m_nDecLen2;	return m_nDecLen;	}

// (2009/1/17 - Seung-Won, Bae) Support Min/Max cache. Do not make Min/Max every time of updating data.
//		Do not support Full Range Min/Max for collision with Pre-Min/Max.
protected:
	BOOL	m_bWithoutZero_Pre;
	double	m_dContinueCode_Pre;
	double	m_dExceptionValue_Pre;
	int		m_nStartPeriod_Pre;
	int		m_nStartIndex_Pre;
	int		m_nEndIndex_Pre;
	double	m_dPreMaxValue;
	double	m_dPreMinValue;
	double	m_dPreMaxValueWithoutEnd;
	double	m_dPreMinValueWithoutEnd;
public:
	void	ClearMinMax( void);
	void	UpdateMinMax( double p_dData, int p_nIndex);

// (2009/5/21 - Seung-Won, Bae) Shift the data of packet to future.
public:
	BOOL	ShiftData( long p_szShiftCount);

// 20081007_JS.Kim	사용자시간대 선택
public:
	void	CalcPacketTimeDiff(int nTimeDiff);
	double	CalcTimeDiff(double dData, int nTimeDiff);
	double	CalcTimeDiff(double dData);
	double	CalcBackTimeDiff(double dData);		// 원래 시간대로 되돌리는 함수

// 20100324_JS.Kim 솔로몬.FX	시고저종 외의 패킷을 시고저종처럼 사용하기 위한 처리
public:
	CString	m_strSubName;

	CString GetSubName()						{	return m_strSubName;				};
	void	SetSubName(CString strSubName)		{	m_strSubName = strSubName;	};
};

#endif // !defined(AFX_PACKETIMPLEMENTATION_H__E24ABB33_A837_49DE_AA47_48DF59595FFE__INCLUDED_)
