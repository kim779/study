// MainChartFormulate.h: interface for the CMainChartFormulate class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_MAINCHARTFORMULATE_H__40507629_BC7A_441B_9E2D_C576F9CC32B1__INCLUDED_)
#define AFX_MAINCHARTFORMULATE_H__40507629_BC7A_441B_9E2D_C576F9CC32B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

// (2006/9/5 - Seung-Won, Bae) for Import in Extension DLL
#ifndef AFX_EXT_CLASS_FORMULATE
	#ifdef _FORMULATE_EXPORT									// in IndicatorCondition
		#define AFX_EXT_CLASS_FORMULATE	AFX_CLASS_EXPORT
//	#elif defined _DD_NOT_IMPORT							// in Lib or Regular used in IndicatorCondition.
//		#define AFX_EXT_CLASS_DD			
	#else													// in Other, To Link
		#define AFX_EXT_CLASS_FORMULATE	AFX_CLASS_IMPORT
	#endif
#endif

class CIndicatorList;
class CPacketList;
class CMainChartFormulateImplementation;
class CChartFormulateData;
class CGraphCalculateData;
class CIndicatorInfo;

class CPacket;
class AFX_EXT_CLASS_FORMULATE CMainChartFormulate  
{
public:
	CMainChartFormulate();
	virtual ~CMainChartFormulate();

	//외부에 노출된 함수에서 호출.df
	//계산된 Data를 Packet에 저장하고, 저장한 packetName을 얻는다.
	bool CalculateIndicator(const CChartFormulateData& chartFormulateData,
							CGraphCalculateData* graphCalculateData, 
							CIndicatorInfo*& pIndicatorInfo);
private:
	CMainChartFormulateImplementation* m_pMainImplementation;
};

#endif // !defined(AFX_MAINCHARTFORMULATE_H__40507629_BC7A_441B_9E2D_C576F9CC32B1__INCLUDED_)
