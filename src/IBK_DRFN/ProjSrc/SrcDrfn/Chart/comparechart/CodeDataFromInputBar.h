// CodeDataFromInputBar.h: interface for the CCodeDataFromInputBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CODEDATAFROMINPUTBAR_H__1943BC6A_7285_42F7_928A_8B99C5A99F71__INCLUDED_)
#define AFX_CODEDATAFROMINPUTBAR_H__1943BC6A_7285_42F7_928A_8B99C5A99F71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Chart_Common/DlgCompData.h"

class CCodeDataFromInputBar  
{
public:
	CCodeDataFromInputBar();
	virtual ~CCodeDataFromInputBar();
	CString	m_strCode;
	long	m_lEndTime;
	char	m_chType;		//1분,2일,3주,4월
	long	m_lTypeNum;		//?봉.
	// gm0604 added
	long	m_lDataCnt;
	long	m_lPtrST;
	CString m_strSTName;
	// gm0604 added (end)

	// Display 카운트
	long	m_lDispCnt;

	// 조회 요청할 카운트 수
	long	m_lRequestCnt;

	//가격추이 분할비교(A)/가격추이 중첩비교(B)/누적등락률 추이 중첩비교(C)/기간별 수익률 비교(D)/총기간 누적등락 비교(E)
	char	m_cChartShape;
	
	int		m_nUnitType;	//WON_TYPE,PERCENT_TYPE,DOLLAR_TYPE

	// '0' : 기본,원
	// '1' : 기준일 종가기준 
	// '2' : $
	// '3' : 당일시가기준
	// '4' : 현재가기준
	// '5' : 전일종가기준.
	int		m_cDataGubun;

	// 0선, 1:봉
	int		m_nLineType;	

	// 당일조회여부
	BOOL	m_bTodayRequest;

};

// 비교차트는 데이터를 공유하므로 한 군데에서 관리하기 위해 만듬.
// CShareInputData를 하나만 생성해서 공유해서 처리하도록 수정필요.
class CShareInputData
{
public:
	CShareInputData();

	CCodeDataFromInputBar	m_DataInputBar;
	int		m_nScreenType;	// 0:비교차트, 1:당일등락률차트
	int		m_nBase;		// 당일등락률차트일때 등락기준 0:전일종가, 1:시가, 2:현재가
	BOOL	m_bCanDel;

	void	ReInit();
};

class CDlgCompDataHandle : public IDlgCompDataHandle
{
public:
	STDMETHOD_(void, LoadData)(long pDataKey);
	STDMETHOD_(void, SaveData)(long pDataKey);
	
	//기타 개별저장이 필요한 저장.
	STDMETHOD_(void, SaveSelGroup)(long pDataKey);
	STDMETHOD_(void, SaveShowGroupInfo)(long pDataKey);
	STDMETHOD_(void, SaveSelCodeInfo)(long pDataKey);
	STDMETHOD_(void, SaveTimeUse)(long pDataKey);

	STDMETHOD_(void, ClearAll)(long pDataKey);
	STDMETHOD_(void, ClearGroupList)(long pGroupList);
	STDMETHOD_(int,  CopyGroupList)(long pOrg, long pTarget);

	STDMETHOD_(void, DoCheckFirst)();
};

#endif // !defined(AFX_CODEDATAFROMINPUTBAR_H__1943BC6A_7285_42F7_928A_8B99C5A99F71__INCLUDED_)
