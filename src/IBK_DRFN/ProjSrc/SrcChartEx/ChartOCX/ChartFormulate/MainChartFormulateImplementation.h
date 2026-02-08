// MainChartFormulateImplementation.h: interface for the CMainChartFormulateImplementation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINCHARTFORMULATEIMPLEMENTATION_H__163DD804_C95D_475D_B52A_8B32138F4642__INCLUDED_)
#define AFX_MAINCHARTFORMULATEIMPLEMENTATION_H__163DD804_C95D_475D_B52A_8B32138F4642__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "CalculateData.h"
#include "IndicatorFactory.h"

class CIndicator;
class CPacket;
class CPacketList;
class CIndicatorList;
class CChartFormulateData;
class CGraphAllPacket;

class CMainChartFormulateImplementation  
{
public:
	CMainChartFormulateImplementation();
	virtual ~CMainChartFormulateImplementation();

	//CMainChartFormulate에서 호출. DrawAllEnvironment에게 SubGraph를 return
	bool CalculateIndicator(const CChartFormulateData& chartFormulateData,
			CGraphCalculateData* graphCalculateData, CIndicatorInfo*& pIndicatorInfo);

/*
	void GetGraphData(const CString& strGraphName, CList<double, double>& dConditionList,
			CList<CString, CString>& dPacketNameList, CPacketList* pPacketList,
			CIndicatorList* pIndicatorList,	CList<CPacket*, CPacket*>* pPacketsList);

	void GetIndicatorData(const CString& strGraphName, const CString& strSubIndex,
			CList<double, double>& dConditionList, CList<CString, CString>& dPacketNameList,
			CPacketList* pPacketList, CIndicatorList* pIndicatorList, CList<CPacket*, CPacket*>* pPacketsList);
*/

	// packet 주소들을 초기화(NULL)
	void Initialize_AllPackets();

//	void Initialize_Packet(const CString& strGraphName);
//	void DeleteDataInPacket(CPacketList* pPacketList, const CString& strGraphName);

//	void DeletePacket(const CString& strGraphName, const CString& strSubGraphName,
//			CPacketList* pPacketList, CIndicatorList* pIndicatorList);

//	void GetAllGraphPackets(const CChartFormulateData& chartFormulateData,
//					CGraphCalculateData* graphCalculateData, CIndicatorInfo*& pIndicatorInfo);

private:
	CCalculateData m_CalculateData;
	CIndicatorSingleton m_indiSingleton;
	CGraphAllPacket* m_pGraphAllPacket;
	
	//속성페이지에서 선택한 지표외에는 UpdateRange가 -1로 넘어와도 Packet에 계산된 Data가 없을때
	//PacketData의 Count를 계산할 영역으로 CPoint를 얻음.
	CPoint GetDataPoint(const CChartFormulateData& chartFormulateData, CGraphCalculateData* pGraphCalculateData);

	void GetDataPointWhenREALAPPEND(CPacketList* pPacketList,
			CGraphCalculateData* pGraphCalculateData, CPoint& point);

	bool Calculate(CIndicator* pIndicator, const CChartFormulateData& chartFormulateData,
			CGraphCalculateData* graphCalculateData, CIndicatorInfo* pIndicatorInfo);
	bool Calculate_Graph(CIndicator* pIndicator, const CChartFormulateData& chartFormulateData,
			CGraphCalculateData* graphCalculateData, CIndicatorInfo* pIndicatorInfo);
	bool Calculate_SubGraph(CIndicator* pIndicator, const CChartFormulateData& chartFormulateData,
			CGraphCalculateData* graphCalculateData, CIndicatorInfo* pIndicatorInfo);

	bool IsThisDaegiMemul(const CString& strGraphName, const CCalculateBaseData::CALCULATETYPE calculatetype);
	bool IsCalculateSubGraph(const CString& strIndicatorName) const;

//	void GetSystemTradeforPacketList(CGraphCalculateData* pGraphCalculateData, 
//			CList<CPacket*, CPacket*>* pGraphList);
//	void DeleteSubGraphPacketList(CGraphCalculateData *pGraphCalculateData);
};

#endif // !defined(AFX_MAINCHARTFORMULATEIMPLEMENTATION_H__163DD804_C95D_475D_B52A_8B32138F4642__INCLUDED_)
