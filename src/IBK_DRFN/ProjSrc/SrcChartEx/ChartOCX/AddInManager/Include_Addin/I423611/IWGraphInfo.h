// IWGraphInfo.h: interface for the CIWGraphInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWGRAPHINFO_H__D3FCC53E_FF7F_4CF0_830D_0402C03D7E7E__INCLUDED_)
#define AFX_IWGRAPHINFO_H__D3FCC53E_FF7F_4CF0_830D_0402C03D7E7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../Include_Chart/Dll_Load/IndicatorInfo.h"
#include "../_IWUnknown.h"
#include "_IGraphInfo.h"
#include "_IChartOCX.h"										// for IChartOCX

class CIWGraphInfo : public CIWUnknown, public IGraphInfo 
{
// (2004.10.07, 배승원) Interface Wrapper에서 IUnknown의 Interface를 손쉽게 지원할 수 있도록 Macro를 정의한다.
protected:
	_IWRAPPER_IUNKNOWN_MANAGER( IGraphInfo, CIWGraphInfo, CIndicatorInfo, m_pIndicatorInfo)
	
protected:
	// 종찬(05/07/14) 지표 설정값을 제공한다.
	virtual CList<double, double>* GetCommonCalculateVarList();
	virtual CList<double, double>* GetCalculateVarList(const char* pcszSubGraphName);
	virtual CList<double, double>* GetCalculateVarList(const int nSubGraphIndex);

	// 종찬(05/07/15) 계산에 쓰이는 패킷명을 제공한다.
	virtual LPCTSTR GetCommonCalculatePacketNames();
	virtual LPCTSTR GetCalculatePacketNames(const int nSubGraphIndex);

	// 서브그래프의 인덱스를 리턴한다.
	virtual int GetSubGraphIndex(const char* pcszSubGraphName);	
	
	// 종찬(05/07/29) 서브그래프의 Show/Hide 여부를 리턴한다.
	virtual bool IsShowSubGraph(const char* pcszSubGraphName, bool& bSubGraphShow);
	virtual bool IsShowSubGraph(const int nSubGraphIndex, bool& bSubGraphShow);

};

#endif // !defined(AFX_IWGRAPHINFO_H__D3FCC53E_FF7F_4CF0_830D_0402C03D7E7E__INCLUDED_)
