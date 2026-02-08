// Packetbase.h: interface for the CPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKETBASE_H__DDFE472D_2D8F_11D5_81A5_0050FC28B229__INCLUDED_)
#define AFX_PACKETBASE_H__DDFE472D_2D8F_11D5_81A5_0050FC28B229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "../BlockBaseData.h"

// (2006/9/5 - Seung-Won, Bae) for Import in Extension DLL
#ifndef AFX_EXT_CLASS_PACKETDATA
	#ifdef _PACKETDATA_EXPORT								// in PacketList.dll
		#define AFX_EXT_CLASS_PACKETDATA	AFX_CLASS_EXPORT
//	#elif defined _DD_NOT_IMPORT							// in Lib or Regular used in PacketList.
//		#define AFX_EXT_CLASS_DD			
	#else													// in Other, To Link
		#define AFX_EXT_CLASS_PACKETDATA	AFX_CLASS_IMPORT
	#endif
#endif

class CPacketList;
class CPacketImplementation;
class AFX_EXT_CLASS_PACKETDATA CPacket : public CObject  
{
public:	
	CPacket(CPacketList * pParentList,
			CString strName, 
			CString strFieldName, 
			CString strType,
			CString szExpressionType,
			int nDataLength, 
			bool bUseReal,
			int nStartPeriod = 0,
			CString strRealFieldName = "");

	virtual ~CPacket();

	//nami 9.5
	bool RestoreMax();
	bool RestoreMin();
	bool RestoreMinMax();
//	double GetMin()	const;
//	double GetMax() const;
	
	bool SetMax(const double nMax);
	bool SetMin(const double nMin);
	bool SetMinMax(const double nMin, const double nMax);
	void SetBaseData(const CString &strType, 
					 const CString &strExpressionType,
					 const int nStartPeriod, 
					 const bool bIsAutoMinMax = true);

	void SetYesOrNoBaseData(const bool bBaseData);
	void SetAcceptFromServer(const bool bAcceptFromServer);
	bool IsTRData() const;
	bool IsAcceptFromServer() const;
	// data 기본이 "0" 인 packet
	bool IsZeroBasePacket() const;
	bool IsChangeRealData() const;
	//	Packet에 넣을 한 data의 길이
	int GetLength();
	//	data의 갯수 return
	virtual int GetCount() const;
	//  실제 화면에 보이는 data 개수
	int GetDataCount();
	//  전체 데이타중 가장 긴것의 길이
	int GetMaxCount();
	//	CPacket의 FieldName을 얻는다 (맵 디자인시 지정한 이름)
	CString GetFieldName();
	//	CPacket의 Name을 얻는다 (기존의 이름 or 사용자가 정의한 이름)
	CString GetName();
	// 20100324_JS.Kim 솔로몬.FX	시고저종 외의 패킷을 시고저종처럼 사용하기 위한 처리
	CString GetSubName();
	void	SetSubName(CString strSubName);
	//nami 9.30
	CPacketList* GetPacketList();
	CPacket* GetDatePacket();
	CPacketType::COMMODITY_BOND_NAME GetCommodityBondName() const;
	void SetCommodityBondName(const CPacketType::COMMODITY_BOND_NAME eCommodityBondName);
	// fail : -1  success : index of data
	int GetIndexFromData(double nData);
	// 분차트 근접시간 적용 - ojtaso (20080811)
	int GetIndexFromData(double nData, CString strType);
	// return : true -> 범위 안인 경우
	//			false -> 범위를 벗어나거나 같은 값이 없는 경우
	bool GetCurOrPrevIndexFromData( const double dData, int& nCurOrPrevIndex, double& dCurOrPrevIndexData) const;
	bool GetData(const int nIndex, double& dData) const;
	// fail : ERRORVALUE   success : data of index
	double GetData(int nIndex);
	CString GetStrData(int nIndex);
	// 과거와 미래 시제의 시간까지도 return한다
	// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
	//int GetDataPastandFuture(int nIndex);
	double GetDataPastandFuture(int nIndex);
	// 2011.01.06 by SYS <<
	bool GetDataPastandFuture(const int nIndex, double& dData);
	int GetTimeInterval();
	int GetDateUnitType();
	CString GetRealKeyName();
	CString GetRealFieldName();
	bool GetUseReal();
	bool IsUseStandardValue( const int p_nType = 0) const;		// 0:Base,	1:Upper Base,	2:Lower Base
	double GetStandardValue( const int p_nType = 0);			// 0:Base,	1:Upper Base,	2:Lower Base
	int GetStartPeriod();
	void SetRealDataGap( const double &p_dDataGap);
	bool GetRealDataGap(double& dDataGap) const;
	//nami 9.29
	CString GetType(const bool bIsFullType = true);
	double GetTypeFormatDouble(const bool bIsFullType = true);
	//	CPacket의 data list를 리턴한다
	const CList<double, double>* GetnumericDataList() const;
	CList<double, double>* GetnumericDataList();
	CList<CString, CString>* GetstrDataList();
	//	data의 Minimum & Maximum값을 구해줌
	//	nStart : data의 시작 위치
	//	nEnd   : data의 끝 위치
	//	dMin   : 리턴할 Minimum 값 
	//	dMax   : 리턴할 Maximum 값
	//	return value : success = 0 or 1
	//	-2 : type error
	//	-1 : error
	//	 0 : nEnd - nStart = Data Count -> success
	//	 1 : success
	// (2009/1/17 - Seung-Won, Bae) Add without-Zero option.
	int GetMinMax(int nStart, int nEnd, double& nMin, double& nMax, BOOL p_bWithoutZero = FALSE);

	// data의 Minimun & Maximum 값을 구해서 저장함
	// parameter 
	//   nInsertType : NEW, INSERT, APPEND
	//	 ptDataRange : 추가된 데이타의 처음위치, 끝위치 (default -1, -1));
	// return
	//	-2 : type error
	//	-1 : error
	//	 none zero : success
	int SetMinMax(CPacketDataInfo::DATAINSERTTYPE nInsertType = CPacketDataInfo::NEW, CPoint ptDataRange = CPoint(0, 0));
	//	CPacket이 실시간 받는 Packet일 경우 실시간 data Field Name을 세팅한다
	void SetRealFieldName(CString strRealFieldName, CString strRealKeyName);
	void SetBoundLimit(double nUpperBound, double nLowerBound);
	void SetStandardValue( const char *p_szValue);
	void SetStartPeriod(int nStartPeriod);
	void SetName(CString strName);
	void SetType(const CString& strType);
	void SetType(const CString& strFullType, const CString& strExpressionType);
	void SetLength(int nLength);

	// (2007/2/7 - Seung-Won, Bae) for Exception Value
	void	SetExceptionValue( const double &p_dExceptionValue);
	double	GetExceptionValue( void);

	// (2007/2/22 - Seung-Won, Bae) for Continue Code
	void SetContinueCode( const double &p_dContinueCode);
	double GetContinueCode( void);

	//	CPacket내의 data를 주어진 data로 바꾼다 list의 data값은 모두 copy된다
	void ChangeData(CList<double, double>& lsnewData);
	void ChangeData(CList<CString, CString>& lsnewData);

	//	String 형태로 들어온 data를 data list에 insert (head에 해당)
	void InsertData(CString strData);	
	//	주소로 들어온 data를  읽어서 int형으로 data list에 insert (head에 해당)
	void InsertData(long pData, int nDataLength);
	//	주소로 들어온 data를  읽어서 int형으로 data list에 append (tail에 해당)
	void AppendData(long pData, int nDataLength);
	//	String 형태로 들어온 data를 int형으로 바꾸어서 data list에 append (tail에 해당)
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
	bool RemoveBaseData( int p_nCount);
	int RemoveFictionData(const double dReallastDate, const bool bDate = false);
	// CPacket내의 data를 clear함
	virtual int ClearData(const bool bClearIncludingTR = false);	
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
	CPacketImplementation* m_pPacketImplementation;

// (2006/12/20 - Seung-Won, Bae) Support Binary TR Data Receiving
public:
	void	AppendTrData( const char *p_szData);
	void	AppendTrData( const double dData);
	void	InsertTrData( const char *p_szData);
	void	InsertTrData( const double dData);

// (2007/2/21 - Seung-Won, Bae) Support Not-Auto Scale Type.
public:
	void	SetAutoScaleType( BOOL p_bAutoScaleType);
	BOOL	IsAutoScaleType( void);
	void	SetScaleType( const CString &p_strScaleType);
	CString	GetScaleType( void);
	double	GetScaleTypeFormatDouble( void);
	// 현재가 등락률 - ojtaso (20080602)
	void OffsetPacketData(double dData);

// 20080908 김진순	8진법/32진법 표시 적용
public:
	void	SetBaseNumber(int nBase);
	int		GetBaseNumber();
	void	SetDecLen(int nDecLen, int nDecLen2);
	int		GetDecLen(int &nDecLen, int &nDecLen2);

// (2009/1/17 - Seung-Won, Bae) Support Min/Max cache. Do not make Min/Max every time of updating data.
//		Do not support Full Range Min/Max for collision with Pre-Min/Max.
public:
	void	ClearMinMax( void);

// (2009/5/21 - Seung-Won, Bae) Shift the data of packet to future.
public:
	BOOL	ShiftData( long p_szShiftCount);

// 20081007 JS.Kim	사용자시간대 선택
public:
	void	CalcPacketTimeDiff(int nTimeDiff);
	double	CalcTimeDiff(double dData);
	double	CalcTimeDiff(double dData, int nTimeDiff);
	double	CalcBackTimeDiff(double dData);		// 원래 시간대로 되돌리는 함수
};

class AFX_EXT_CLASS_PACKETDATA CBracket {
public :
	CBracket( CString szItem, int nIndex, int nDate);
	~CBracket();

	int				GetIndex()	const	{	return m_nIndex;	}
	int				GetDate()	const	{	return m_nDate;		}
	const char *	GetItem()	const	{	return m_szItem;	}

private :
	int		m_nIndex;
	int		m_nDate;
	CString	m_szItem;
};

class AFX_EXT_CLASS_PACKETDATA CMPItem {
public :
	CMPItem();
	~CMPItem();

	int		GetPriceX100()	const { return m_nPriceX100;	}
	DWORD	GetVolume()		const { return m_dwVolume;		}
	CList<CBracket*, CBracket*>* GetBracketList()	{ return &m_listBracket; }

	void	SetPrice(	int		nPriceX100)	{	m_nPriceX100	=	nPriceX100;		}
	void	AddVolume(	DWORD	dwVolume)	{	m_dwVolume		+=	dwVolume;		}

// (2004.01.19, 배승원, 분석) 새로운 CBracket Object를 생성 및 Data를 설정한다.
public:
	void	AddBracket( CString& strAlpa, int nIndex, int nDate);

// (2004.01.20, 배승원, 분석) Price는 비교 대상이 되므로 Int로 관리한다.
// (2004.01.26, 배승원) Volume도 Max Volume 등과 비교하는 경우가 있어 double을 이용하지 않는다.
private :
	int		m_nPriceX100;
	DWORD	m_dwVolume;
	CList< CBracket *, CBracket *> m_listBracket;
};

class AFX_EXT_CLASS_PACKETDATA CMProfile {
public :
	CMProfile();
	~CMProfile();

	int			GetDate() const					{	return m_nDate;	}
	CList<CMPItem*, CMPItem*>* GetMPItemList()	{	return &m_listMPItem; }
	CMPItem*	GetMPItem(const int nIndex);
	int			AddMPItem(CMPItem *item);
	int			GetTotalBracketCount();
	int			GetMaxVolumeIndex();
	virtual int	GetCount() const;

	CMPItem*	AddHead();
	CMPItem*	AddTail();
	CMPItem*	InsertBefore(int nIndex);
	CMPItem*	InsertAfter(int nIndex);
	void		SetDate(const int nDate)	{	m_nDate = nDate;	}

private:
	int m_nDate;
	CList<CMPItem*, CMPItem*> m_listMPItem;

// (2004.01.26, 배승원) Max Volume과 Max Bracket Count인 CMPItem이 1개 이상일 경우를 감안하여
//		Max인 Index값을 제공하지 않고, 그 값을 제공토록한다.
public:
	int			GetMaxBracketCount( void);
	DWORD		GetMaxVolume( void);

// (2004.01.19, 배승원) 단위 MP Chart의 가격 값의 크기를 구한다. (구간 대표값의 범위이다.)
//		(이곳에서도 GetPriceScaleX100()을 제공하고 싶으나 PriceUnitX100을 알지 못하는 관계로 제공하지 않는다.)
public:
	BOOL	GetPriceMinMaxX100( int &nMinX100, int &nMaxX100);

// (2004.01.27, 배승원) 등록된 CMPItem이 없는지 조회하는 Interface를 제공한다.
public:
	BOOL	IsEmpty( void)	{	return m_listMPItem.IsEmpty();	}

// (2004.02.03, 배승원) 최종 종가에 Last Bracket Mark가 출력되도록 한다.
//		MP Chart에서 출력시 이용할 최종 종가를 관리한다.
protected:
	int		m_nLastPriceX100;
public:
	int		GetLastPriceX100( void) const		{	return m_nLastPriceX100;				}
	void	SetLastPriceX100( int p_nLastPriceX100)	{	m_nLastPriceX100 = p_nLastPriceX100;	}
};

class AFX_EXT_CLASS_PACKETDATA CMPPacket : public CPacket  
{
public :
	CMPPacket(CPacketList* pParentList, CString szName, CString szFieldName, 
			CString szType, CString szExpType, int nLength, bool bUseReal, 
			int nStartPeriod = 0, CString szRealFieldName = "");

	CMPPacket(CPacketList *pPacketList, CString szName);
	virtual ~CMPPacket();
	
	virtual int	GetCount() const;
	virtual int	ClearData(const bool bClearIncludingTR = false);

	int			GetMaxHitting();
	int			GetMaxMPItemCount();
	int			GetTotalMPItemCount();
	int			AddMarketProfile(CMProfile *mp);
	CMProfile*	AddTail();
	CMProfile*	GetMarketProfile(const int nIndex);
	CList< CMProfile*, CMProfile*>* GetMarketProfileList() { return &m_listMarketProfile; }

private :
	CList<CMProfile*, CMProfile*> m_listMarketProfile;

	void		RemoveAll();

// (2004.01.26, 배승원) MProfile의 GetMaxVolume()을 이용하여 처리한다.
public:
	DWORD 		GetMaxVolume( void);

// (2004.01.19, 배승원) Drawing 시에 참조할 수 있도록 가격 폭을 관리한다.
protected:
	int		m_nPriceUnitX100;
public:
	void	SetPriceUnitX100( int p_nPriceUnitX100)	{	m_nPriceUnitX100 = p_nPriceUnitX100;	}
	int		GetPriceUnitX100( void) const			{	return m_nPriceUnitX100;	}

// (2004.01.19, 배승원) Drawing 시에 Y Scale인 가격축에 점유하는 영역을 제공한다.
//		(각 가격 값은 구간의 하한값으로 Scale에서는 Max에 구간폭이 가해진 수치가 반환된다.)
public:
	BOOL	GetPriceScaleX100( int &nMinX100, int &nMaxX100);

// (2004.01.20, 배승원) Drawing시에 X Scale인 Bracket 수를 조회하는 Interface를 제공한다.
public:
	int		GetMaxBracketCount( void) const;
};

#endif // !defined(AFX_PACKETBASE_H__DDFE472D_2D8F_11D5_81A5_0050FC28B229__INCLUDED_)
