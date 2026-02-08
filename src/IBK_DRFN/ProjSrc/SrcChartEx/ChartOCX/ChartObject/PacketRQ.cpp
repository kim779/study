// PacketRQ.cpp: imp;lementation of the CPacketRQ class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PacketRQ.h"

#include "../Include_Chart/Dll_Load/PacketList.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/OutsideCommonInterface.h"

#include "algorithm"
#include "GraphImp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define LEN_INDEX 5		// Unique한 RQ를 생성할때 사용되는 수의 문자열 길이(순수 숫자의 문자열 길이는 총 길이의 -2)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// 선택된 차트 정보는 외부dll에서 관리토록 수정 : 복수종목 - ojtaso (20070504)
CPacketRQ::CPacketRQ() :
	m_pPacketList(NULL),
	m_lpszItemCode(NULL),
	m_lpszItemName(NULL)
{
	m_lpszRQ = new char[8];
	::memset(m_lpszRQ, 0x00, 8);
	::lstrcpy(m_lpszRQ, _T("DEFAULT"));

	m_lpszPacketName = ::strdup( _MTEXT( C0_DATE_TIME));
}

// RQ를 만들기 위한 종목코드를 시장/종목/주기/틱수로 변경 : 복수종목 - ojtaso (20070106)
// 틱 카운트 추가 : 복수종목 - ojtaso (20070329)
// 차트의 원, %, $의 구분관련 : 복수종목 - ojtaso (20070411)
// 선택된 차트 정보는 외부dll에서 관리토록 수정 : 복수종목 - ojtaso (20070504)
CPacketRQ::CPacketRQ(LPCTSTR lpszItemCode, LPCTSTR lpszItemName, const UINT& nIndex) :
	m_pPacketList(NULL)
{
	m_lpszItemCode = ::strdup(lpszItemCode);
	m_lpszItemName = ::strdup(lpszItemName);
	m_lpszPacketName = ::strdup( _MTEXT( C0_DATE_TIME));

	m_lpszRQ = new char[::lstrlen(lpszItemCode) + LEN_INDEX];
	::sprintf(m_lpszRQ, _T("%s_%d"), lpszItemCode, nIndex);

}

// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
CPacketRQ::CPacketRQ(LPCTSTR lpszRQ, LPCTSTR lpszItemCode, LPCTSTR lpszItemName) :
	m_pPacketList(NULL)
{
	m_lpszItemCode = ::strdup(lpszItemCode);
	m_lpszItemName = ::strdup(lpszItemName);
	m_lpszPacketName = ::strdup( _MTEXT( C0_DATE_TIME));

	m_lpszRQ = new char[::lstrlen(lpszRQ) + 1];
	::memset(m_lpszRQ, 0x00, ::lstrlen(lpszRQ) + 1);
	::lstrcpy(m_lpszRQ, lpszRQ);
}

CPacketRQ::~CPacketRQ()
{
	// 선택된 차트 정보는 외부dll에서 관리토록 수정 : 복수종목 - ojtaso (20070504)
	if(m_lpszItemCode)
		::free(m_lpszItemCode);
	if(m_lpszItemName)
		::free(m_lpszItemName);
	if(m_lpszPacketName)
		::free(m_lpszPacketName);

	if(m_lpszRQ)
		delete [] m_lpszRQ;

	m_lpszItemCode = m_lpszItemName = m_lpszRQ = m_lpszPacketName = NULL;

	if(m_pPacketList)
	{
		delete m_pPacketList;
		m_pPacketList = NULL;
	}
}

BOOL CPacketRQ::IsSameItemCode(LPCTSTR lpszItemCode)
{
	return !::strcmp(m_lpszItemCode, lpszItemCode);
}

void CPacketRQ::CreatePacketList(CString strPacketOption, CPacketType::REALTYPE eRealType, int nLength, bool bUseRealData, const int nSize, const CString& szPacketName, double dVersion, IChartCtrl *p_pIChartCtrl)
{
	m_pPacketList = new CPacketList(strPacketOption, eRealType, p_pIChartCtrl);

	m_pPacketList->SetPacketLengthCount(nLength);
	m_pPacketList->SetUseRealData(bUseRealData == FALSE ? FALSE : TRUE);
	m_pPacketList->SetHoldingPacketSize(nSize);
	m_pPacketList->SetBasePacketName(szPacketName);		
	// ADD: 최종찬(04/02/18) Packet 버젼을 설정한다.
	m_pPacketList->SetVersion(dVersion);
	m_pPacketList->SetRQ(m_lpszRQ);
}

// RQ를 만들기 위한 종목코드를 시장/종목/주기/틱수로 변경 : 복수종목 - ojtaso (20070106)
// 틱 카운트 추가 : 복수종목 - ojtaso (20070329)
// 차트의 원, %, $의 구분관련 : 복수종목 - ojtaso (20070411)
// 선택된 차트 정보는 외부dll에서 관리토록 수정 : 복수종목 - ojtaso (20070504)
void CPacketRQ::SetItemCode(LPCTSTR lpszItemCode, LPCTSTR lpszItemName)
{
	// 타이틀에 종목명 표시 : 복수종목 - ojtaso (20070322)
	LIST_RQ_GRAPH::iterator it = m_listGraph.begin();
	while(it != m_listGraph.end())
	{
		(*it)->ChangeSubGraphTitle(m_lpszItemName, lpszItemName);
		it++;
	}

	if(m_lpszItemCode)
		::free(m_lpszItemCode);
	if(m_lpszItemName)
		::free(m_lpszItemName);

	m_lpszItemCode = ::strdup(lpszItemCode);
	m_lpszItemName = ::strdup(lpszItemName);
}

void CPacketRQ::AddRQGraph(CGraphImp* pGraph)
{
	LIST_RQ_GRAPH::iterator it = std::find(m_listGraph.begin(), m_listGraph.end(), pGraph);

	if(it == m_listGraph.end())
		m_listGraph.push_back(pGraph);
}

void CPacketRQ::RemoveRQGraph(CGraphImp* pGraph)
{
	LIST_RQ_GRAPH::iterator it = std::find(m_listGraph.begin(), m_listGraph.end(), pGraph);

	if(it != m_listGraph.end())
		m_listGraph.erase(it);
}

BOOL CPacketRQ::IsEmptyRQGraph()
{
	return m_listGraph.empty();
}

// 선택된 차트 정보는 외부dll에서 관리토록 수정 : 복수종목 - ojtaso (20070504)
void CPacketRQ::GetItemCode(LPCTSTR& lpszItemCode, LPCTSTR& lpszItemName)
{
	lpszItemCode = m_lpszItemCode;
	lpszItemName = m_lpszItemName;
}

void CPacketRQ::SetItemRQ(LPCSTR lpszRQ)
{
	if(m_lpszRQ)
		::free(m_lpszRQ);

	m_lpszRQ = ::strdup(lpszRQ);
}

void CPacketRQ::SetItemCode(LPCTSTR lpszItemCode)
{
	if(m_lpszItemCode)
		::free(m_lpszItemCode);

	m_lpszItemCode = ::strdup(lpszItemCode);
}

void CPacketRQ::SetItemName(LPCTSTR lpszItemName)
{
	if(m_lpszItemName)
		::free(m_lpszItemName);

	m_lpszItemName = ::strdup(lpszItemName);
}

void CPacketRQ::SetPacketName(LPCTSTR lpszPacketName)
{
	if(m_lpszPacketName)
		::free(m_lpszPacketName);

	m_lpszPacketName = ::strdup(lpszPacketName);
}

void CPacketRQ::SetRQ(LPCTSTR lpszRQ)
{
	if(m_lpszRQ)
		::free(m_lpszRQ);

	m_lpszRQ = ::strdup(lpszRQ);
}

void CPacketRQ::Change(CPacketRQ& packetRQ)
{
	if(this == &packetRQ)
		return;

	LPSTR lpszItemCode = ::strdup(packetRQ.GetItemCode());
	LPSTR lpszItemName = ::strdup(packetRQ.GetItemName());

	packetRQ.SetItemCode(m_lpszItemCode, m_lpszItemName);
	SetItemCode(lpszItemCode, lpszItemName);

	LPSTR lpszPacketName = ::strdup(packetRQ.GetPacketName());

	packetRQ.SetPacketName(m_lpszPacketName);
	SetPacketName(lpszPacketName);

	CPacketList* lpPacketList = packetRQ.GetPacketList();
	packetRQ.SetPacketList(m_pPacketList);

	m_pPacketList = lpPacketList;

	::free(lpszItemCode);
	::free(lpszItemName);
	::free(lpszPacketName);

	LIST_RQ_GRAPH listGraph = packetRQ.GetGraphList();
	packetRQ.SetGraphList(m_listGraph);

	m_listGraph = listGraph;
}