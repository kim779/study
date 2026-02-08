// Packetbase.h: interface for the CPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKETBASE_H__DDFE472D_2D8F_11D5_81A5_0050FC28B229__INCLUDED_)
#define AFX_PACKETBASE_H__DDFE472D_2D8F_11D5_81A5_0050FC28B229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "BlockBaseData.h"

class CRealUpdateMode
{
public:
	enum MODE {	APPEND,	UPDATE };
};

class CPacketList;
class CPacketImplementation;
class AFX_EXT_CLASS CPacket : public CObject  
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
	double GetMin()	const;
	double GetMax() const;
	void ResetMinMax();
	
	bool SetMax(const double nMax);
	bool SetMin(const double nMin);
	bool SetMinMax(const double nMin, const double nMax);
	void SetBaseData(const CString &strType, 
					 const CString &strExpressionType,
					 const int nStartPeriod, 
					 const bool bIsAutoMinMax = true);

	void SetYesOrNoBaseData(const bool bBaseData);
	bool IsTRData() const;
	//	Packet에 넣을 한 data의 길이
	int GetLength();
	//	data의 갯수 return
	int GetCount();
	//  실제 화면에 보이는 data 개수
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
	CRealUpdateMode::MODE GetRealUpdateMode(); 
	// fail : -1  success : index of data
	int GetIndexFromData(double nData);
	// return : true -> 범위 안인 경우
	//			false -> 범위를 벗어나거나 같은 값이 없는 경우
	bool GetCurOrPrevIndexFromData( const double dData, int& nCurOrPrevIndex, double& dCurOrPrevIndexData) const;
	// fail : 0   success : data of index
	double GetData(int nIndex);
	CString GetStrData(int nIndex);
	// 과거와 미래 시제의 시간까지도 return한다
	int GetDataPastandFuture(int nIndex);
	int GetTimeInterval();
	int GetDateUnitType();
	int GetFundTypeStartandEndPoint(CPoint& ptStart, CPoint& ptEnd);
	CString GetRealKeyName();
	CString GetRealFieldName();
	bool GetUseReal();
	double GetStandardValue();
	int GetStartPeriod();
	//nami 9.29
	CString GetType(const bool bIsFullType = true);
	double GetTypeFormatDouble(const bool bIsFullType = true);
	//	CPacket의 data list를 리턴한다
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
	int GetMinMax(int nStart, int nEnd, double& nMin, double& nMax);
	

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
	void SetRealUpdateMode(CRealUpdateMode::MODE mode);
	void SetBoundLimit(double nUpperBound, double nLowerBound);
	void SetStandardValue(double nValue);
	void SetStartPeriod(int nStartPeriod);
	void SetName(CString strName);
	void SetType(const CString& strType);
	void SetLength(int nLength);

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
	// (2004.12.29, 배승원) 누적 Data를 기반으로 하는 증가량 자동 산출 Packet의 경우에는,
	//			이전에 등록된 마지막 Data를 Numeric Data List에서 구하여 등록하지 않는다.
	//		이전에 등록된 마지막 Data는 누적이 아니라, 이미 증가량으로 산출된 Data로,
	//			이를 등록시키게 되면 이를 새로운 누적 Data로 오해하여 엉뚱한 증가량을 산출하여 등록케 된다.
	//		따라서, 허봉으로 등록되는 곳에서만 p_bAddPrev를 TRUE로 설정하여,
	//			별도로 관리되는 '실제 이전 누적 Data'(m_dRealDataGap)를 등록토록한다.
	//		이때 들어오는 dData는 이전에 산출된 증가량 Data이다. (혹은 전일 끝봉의 Data일수도 있다.)
	void AppendData(double nData, int nIndex = -1, BOOL p_bAddPrev = FALSE);
	//	List 형태로 들어온 data를 data list에 insert (head에 해당)
	void AppendData(CList<double, double>& lsnewData);	
	void AppendData(CList<CString, CString>& lsnewData);

	void UpdateData(const double nData);
	void UpdateData(const double nData, const int nIndex, const bool bIsCurrentData = true);
	void UpdateData(const CString& strData);
	void UpdateData(const CString& strData, const int nIndex, const bool bIsCurrentData = true);
	void UpdateData(CList<double, double>& lsnewData, CPoint ptUpdateRange);
	void UpdateData(CList<CString, CString>& lsnewData, CPoint ptUpdateRange);

	bool UseStandardValue();

	bool RemoveData(int nStart, int nEnd);
	int RemoveFictionData(const double dReallastDate, const bool bDate = false);
	// CPacket내의 data를 clear함
	virtual int ClearData(const bool bClear = false);	

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
};

class AFX_EXT_CLASS CBracket {
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

class AFX_EXT_CLASS CMPItem {
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

class AFX_EXT_CLASS CMProfile {
public :
	CMProfile();
	~CMProfile();

	int			GetDate() const					{	return m_nDate;	}
	CList<CMPItem*, CMPItem*>* GetMPItemList()	{	return &m_listMPItem; }
	CMPItem*	GetMPItem(const int nIndex);
	int			AddMPItem(CMPItem *item);
	int			GetTotalBracketCount();
	int			GetMaxVolumeIndex();
	int			GetCount();

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

class AFX_EXT_CLASS CMPPacket : public CPacket  
{
public :
	CMPPacket(CPacketList* pParentList, CString szName, CString szFieldName, 
			CString szType, CString szExpType, int nLength, bool bUseReal, 
			int nStartPeriod = 0, CString szRealFieldName = "");

	CMPPacket(CPacketList *pPacketList, CString szName);
	virtual ~CMPPacket();
	
	virtual int		GetCount();
	virtual int		ClearData(const bool bClear = false);

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
