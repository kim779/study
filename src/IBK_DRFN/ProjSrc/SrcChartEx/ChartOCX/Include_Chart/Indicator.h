// Indicator.h: interface for the CIndicator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDICATOR_H__EDA4FFFD_EAA4_4AD5_BBAD_B18D2B4D4D74__INCLUDED_)
#define AFX_INDICATOR_H__EDA4FFFD_EAA4_4AD5_BBAD_B18D2B4D4D74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>							// for CList

#include "./Dll_Load/IMetaTable.h"				// for IMetaTable
#include "../Include_Chart/BlockBaseEnum.h"		// for CCalculateBaseData

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

#define TYPE_INTERGER "x1"
#define TYPE_INTERGER_PERCENT "x1%"
#define TYPE_TWODECIMAL "x0.01"
#define TYPE_TWODECIMAL_PERCENT "x0.01%"

class CPacket;
class CPacketList;
class CCalculateData;
class CIndicatorInfo;
class CChartFormulateData;
class CGraphCalculateData;
class CSubGraphPacketList;
class AFX_EXT_CLASS_FORMULATE CIndicator  
{
// (2006/9/9 - Seung-Won, Bae) for Indicator Object of each Graph.
protected:
	IMetaTable::EDDC_INDICATOR_ID m_eID;
public:
	CIndicator( IMetaTable::EDDC_INDICATOR_ID p_eID);
	virtual ~CIndicator();
	IMetaTable::EDDC_INDICATOR_ID GetID( void)			{	return m_eID;	}

public:
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData) = 0;

	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									CGraphCalculateData* pGraphCalculateData,
									CIndicatorInfo* pIndicatorInfo);

public:
	CPacket* GetPacket(CPacketList* pPacketList, const CString& strPacketNames, 
		const CCalculateBaseData::CALCULATETYPE eCalType, const CPoint& point);

protected:
	void GetSignalData(const CPoint& point,
					   const int nDataCount,
					   const double dSourceCondition,
					   const double dCondition,
					   const CList<double, double>& dSourceList,
					   CPacket* pSignalPacket);

	void GetSignalData(const CPoint& point,
					   const int nDataCount,
					   const double dSourceCondition,
					   const double dCondition,
					   CPacket* pSourceData,
					   CPacket* pSignalData);

	CList<CString, CString>* GetstrDataList(CPacket *pPacket);

	bool GetSignalPacket(CPacketList* pPacketList, CString& strFuncName, CPacket*& pSignalPacket);

	CPacket* GetPacket(const CPoint& point, 
					   const CChartFormulateData& chartFormulateData);

	void MakeZeroMemory(const int nCount, double* doubleArray);
	POSITION FindPosition(const int nIndex, const CList<int, int>& pSourceList) const;
	POSITION FindPosition(const int nIndex, const CList<double, double>& pSourceList) const;
	POSITION FindPosition(const int nIndex, const CList<CString, CString>& pSourceList) const;

	bool IsValidPacket(CPacket* pPacket);
	bool IsValidPacket(const CList<double, double>* pSourceList);

	double GetAt(const CList<double, double>* pSourceList, int nIndex);
	double GetAt(const CList<double, double>* pSourceList, POSITION& pos);
	CString GetAt(const CList<CString, CString>* pSourceList, int nIndex);
	CString GetAt(const CList<CString, CString>* pSourceList, POSITION& pos);
	double GetNext(const CList<double, double>* pSourceList, POSITION& pos);
	CString GetNext(const CList<CString, CString>* pSourceList, POSITION& pos);
	double GetKindMA(const CChartFormulateData& chartFormulateData, CList<double, double>* pConditionList);
	
	double GetCondition(const int& nIndex, CList<double, double>* pConditionList) const;
	int GetPreEndPrice(CList<double, double>* pDateList, 
							   CScaleBaseData::HORZSCALEDRAWERTYPE nType,
							   int startindex);

	
	bool SeachCalForPacketName(const CString& calpacketname, const CString& packetname);
	//CPacket* MakePacket(const CPoint& point, const CChartFormulateData& chartFormulateData);
	CPacket* MakePacket_HighLowClose(CPacketList* pPacketList, const CCalculateBaseData::CALCULATETYPE eCalType, const CPoint& point);
	CPacket* MakePacket_HighLow(CPacketList* pPacketList, const CCalculateBaseData::CALCULATETYPE eCalType, const CPoint& point);

	bool ClearPacketData(CPacket* pPacket);

	void SetData(const CPoint& point, const int nUpdateIndex, const double dData, CList<double, double>* pData);
	void SetData(const CPoint &point, const int nUpdateIndex, const double dData, CPacket* pPacket);
	void SetData(const CPoint &point, const int nUpdateIndex, const CString& strData, CPacket* pPacket);

	bool SetGraphPacketList(CPacketList* pPacketList,
							CGraphCalculateData* pGraphCalculateData,
							CIndicatorInfo*& pIndicatorInfo,
							CPacket*& pPacket);
	bool SetGraphPacketList(CPacketList* pPacketList,
							CGraphCalculateData* pGraphCalculateData,
							CIndicatorInfo*& pIndicatorInfo,
							CPacket*& pPacket, CPacket*& pPacket1);
//	bool SetGraphPacketList(CPacketList* pPacketList,
//							CGraphCalculateData* pGraphCalculateData,
//							CIndicatorInfo*& pIndicatorInfo,
//							CPacket*& pPacket, CPacket*& pPacket1, CPacket*& pPacket2);
//	bool SetGraphPacketList(CPacketList* pPacketList,
//							CGraphCalculateData* pGraphCalculateData,
//							CIndicatorInfo*& pIndicatorInfo,
//							CPacket*& pPacket, CPacket*& pPacket1, CPacket*& pPacket2, 
//							CPacket*& pPacket3);
//	bool SetGraphPacketList(CPacketList* pPacketList,
//							CGraphCalculateData* pGraphCalculateData,
//							CIndicatorInfo*& pIndicatorInfo,
//							CPacket*& pPacket, CPacket*& pPacket1, CPacket*& pPacket2, 
//							CPacket*& pPacket3, CPacket*& pPacket4);
	void SetPacketTypes(CPacket* pPacket, const CString& strUnit, const CString& strType, 
						const int nStartPeriod);

	bool InCaseNoPacketPointer(CPacketList* pPacketList, CIndicatorInfo*& pIndicatorInfo,
					           CPacket*& pPacket, CSubGraphPacketList* pSubGraphList);
	bool InCaseNoPacketPointer(CPacketList* pPacketList, CIndicatorInfo*& pIndicatorInfo,
					           CPacket*& pPacket, CPacket*& pPacket1, 
							   CGraphCalculateData* pGraphCalculateData);
//	bool InCaseNoPacketPointer(CPacketList* pPacketList, CIndicatorInfo*& pIndicatorInfo,
//					           CPacket*& pPacket, CPacket*& pPacket1, CPacket*& pPacket2, 
//							   CGraphCalculateData* pGraphCalculateData);
//	bool InCaseNoPacketPointer(CPacketList* pPacketList, CIndicatorInfo*& pIndicatorInfo,
//					           CPacket*& pPacket, CPacket*& pPacket1, CPacket*& pPacket2, 
//							   CPacket*& pPacket3, CGraphCalculateData* pGraphCalculateData);
//	bool InCaseNoPacketPointer(CPacketList* pPacketList, CIndicatorInfo*& pIndicatorInfo,
//					           CPacket*& pPacket, CPacket*& pPacket1, CPacket*& pPacket2, 
//							   CPacket*& pPacket3, CPacket*& pPacket4, 
//							   CGraphCalculateData* pGraphCalculateData);

	bool IsIndicatorCalculated(const CCalculateBaseData::CALCULATETYPE calculatetype, CPacket* pPacket);
	CList<double, double>* GetnumericDataList(CPacket* pPacket);

	CString GetPacketType(CPacketList* pPacketList) const;

protected:
	void GetSimpleMA(const CPoint& point, const double& dCond, CList<double, double>* pSourceList, CPacket* pMAPacket, BOOL p_bContinueZero = FALSE);
};

#endif // !defined(AFX_INDICATOR_H__EDA4FFFD_EAA4_4AD5_BBAD_B18D2B4D4D74__INCLUDED_)
