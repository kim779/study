// MainChartFormulate.h: interface for the CMainChartFormulate class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_MAINCHARTFORMULATE_H__40507629_BC7A_441B_9E2D_C576F9CC32B1__INCLUDED_)
#define AFX_MAINCHARTFORMULATE_H__40507629_BC7A_441B_9E2D_C576F9CC32B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CIndicatorList;
class CPacketList;
class CMainChartFormulateImplementation;
class CChartFormulateData;
class CGraphCalculateData;
class CIndicatorInfo;

class CPacket;
class AFX_EXT_CLASS CMainChartFormulate  
{
public:
	CMainChartFormulate();
	virtual ~CMainChartFormulate();

	//외부에 노출된 함수에서 호출.df
	//계산된 Data를 Packet에 저장하고, 저장한 packetName을 얻는다.
	bool CalculateIndicator(const CChartFormulateData& chartFormulateData,
							CGraphCalculateData* graphCalculateData, 
							CIndicatorInfo*& pIndicatorInfo);
	//저장 안함.
	void GetGraphData(const CString& strIndicatorName,
					  CList<double, double>& dConditionList,
					  CList<CString, CString>& dPacketNameList,
					  CPacketList* pPacketList,
					  CIndicatorList* pIndicatorList,
					  CList<CPacket*, CPacket*>* pPacketsList);

	//저장 안함.
	void GetIndicatorData(const CString& strFuncName, const CString& strSubIndex, 
					  CList<double, double>& dConditionList, 
					  CList<CString, CString>& PacketNameList, 
					  CPacketList* pPacketList, 
					  CIndicatorList* pIndicatorList, 
					  CList<CPacket*, CPacket*>* pPacketsList);

	//Signal을 얻는다.
	void GetSignalData(CPacketList* pPacketList, const CString& strFunctionName, CList<double, double>*& pSignalData);

	// packet 주소들을 초기화(NULL)
	void Initialize_AllPackets();
	void Initialize_Packet(const CString& strIndicatorName);

	void DeleteDataInPacket(CPacketList* pPacketList, const CString& strIndicatorName);

	void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
					CGraphCalculateData* graphCalculateData, 
					CIndicatorInfo* pIndicatorInfo);

	void GetAllGraphPackets(const CChartFormulateData& chartFormulateData,
											CGraphCalculateData* graphCalculateData,
											CIndicatorInfo*& pIndicatorInfo);

private:
	CMainChartFormulateImplementation* m_pMainImplementation;
};

#endif // !defined(AFX_MAINCHARTFORMULATE_H__40507629_BC7A_441B_9E2D_C576F9CC32B1__INCLUDED_)
