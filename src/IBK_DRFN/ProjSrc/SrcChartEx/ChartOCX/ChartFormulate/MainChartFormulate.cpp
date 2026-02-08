// MainChartFormulate.cpp: implementation of the CMainChartFormulate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainChartFormulate.h"
#include "PacketList.h"

#include "ChartFormulateData.h"
#include "MainChartFormulateImplementation.h"
#include "Signal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMainChartFormulate::CMainChartFormulate():
	m_pMainImplementation(NULL)
{
	m_pMainImplementation = new CMainChartFormulateImplementation();
}

CMainChartFormulate::~CMainChartFormulate()
{
	delete m_pMainImplementation;
}

//main에서 호출.
bool CMainChartFormulate::CalculateIndicator(const CChartFormulateData& chartFormulateData,
											 CGraphCalculateData* graphCalculateData, 
											 CIndicatorInfo*& pIndicatorInfo)
{
	return m_pMainImplementation->CalculateIndicator(chartFormulateData, graphCalculateData, pIndicatorInfo);
}

/*
void CMainChartFormulate::GetSignalData(CPacketList* pPacketList, const CString& strGraphName, CList<double, double>*& pSignalData)
{
	CSignal().GetSignalDatas(strGraphName, pPacketList, pSignalData);	
}


//SystemTrader(double)SK\CalculateSystemIndicator.cpp(48):	pMainChartFormulate->GetGraphData()
//시스템 트레이딩에서 호출..
void CMainChartFormulate::GetGraphData(const CString& strGraphName,
									   CList<double, double>& dConditionList,
									   CList<CString, CString>& dPacketNameList,
									   CPacketList* pPacketList,
									   CIndicatorList* pIndicatorList,
									   CList<CPacket*, CPacket*>* pPacketsList)
{
	m_pMainImplementation->GetGraphData(strGraphName, dConditionList, dPacketNameList, pPacketList,
										pIndicatorList, pPacketsList);
}

//교보에서 신호사용시 호출..
void CMainChartFormulate::GetIndicatorData(const CString& strFuncName,
										   const CString& strSubIndex,
									       CList<double, double>& dConditionList,
									       CList<CString, CString>& PacketNameList,
									       CPacketList* pPacketList,
									       CIndicatorList* pIndicatorList,
									       CList<CPacket*, CPacket*>* pPacketsList)
{
	m_pMainImplementation->GetIndicatorData(strFuncName, strSubIndex, dConditionList, PacketNameList, 
		                                    pPacketList, pIndicatorList, pPacketsList);
}

void CMainChartFormulate::Initialize_Packet(const CString& strGraphName)
{
	m_pMainImplementation->Initialize_Packet(strGraphName);
}



void CMainChartFormulate::DeleteDataInPacket(CPacketList* pPacketList, const CString& strGraphName)
{
	m_pMainImplementation->DeleteDataInPacket(pPacketList, strGraphName);
}

void CMainChartFormulate::GetAllGraphPackets(const CChartFormulateData& chartFormulateData,
											CGraphCalculateData* graphCalculateData,
											CIndicatorInfo*& pIndicatorInfo)
{
	m_pMainImplementation->GetAllGraphPackets(chartFormulateData, graphCalculateData, pIndicatorInfo);
}
*/
