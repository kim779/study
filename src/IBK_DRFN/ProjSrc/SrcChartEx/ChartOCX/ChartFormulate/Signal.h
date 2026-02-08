// Signal.h: interface for the CSignal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIGNAL_H__43DE1F04_C959_4E93_BD35_979670DF8235__INCLUDED_)
#define AFX_SIGNAL_H__43DE1F04_C959_4E93_BD35_979670DF8235__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "BlockBaseData.h"

class CIndicator;
class CPacket;
class CPacketList;
class CIndicatorList;
class CIndicatorInfo;

class CSignal  
{
// vntsorl:[0003431] [7111]차트화면에서의 조회속도가 구차트에 비해 늦어 개선 요구
// (2010/3/20 - Seung-Won, Bae) Do not use ST MA for Map
typedef CList<double, double> CDoublePacketData;

public:
	CSignal();
	virtual ~CSignal();

	bool GetSignalDataPacket(const CString& strGraphName, CPacketList* pPacketList, CPacket*& pSignalPacket) const;
	void GetSignalDatas(const CString& strFuncName, CPacketList* pPacketList, CDoublePacketData *&pSignalData);
//	void CalculateSignal(const CString& strFuncName, const CString& strSubGraphNames, CPacketList* pPacketList, CIndicatorList* pIndicatorList, CPacket* pSignalPacket);
	void GetSignalTypesbyUsingGraphDatas(const CPoint& point, CDoublePacketData *pGraphData2, CDoublePacketData *pGraphData1, CPacket* pSignalPacket);

private:
//	bool DoReCalculation(const CString& strGraphName);
	void MakeRealPacket(const CString& strGraphName, CDoublePacketData *pDataList, CDoublePacketData &dRealDataList);
	CGraphBaseData::SignalTYPE GetSignalType(const double upBaseline, const double downBaseline, const double oldData, const double nowData);
	CGraphBaseData::SignalTYPE GetSignalType(const double baseline, const double oldData, const double nowData);
	CGraphBaseData::SignalTYPE GetSignalType3(const double oldBaseData, const double nowBaseData, const double oldData, const double nowData);

	double GetCurDataFromList( POSITION& pos, CDoublePacketData *pGraphData);
	int GetFirstNonZeroValueIndex( CDoublePacketData *pGraphData1);
	//기준선값 얻기. Signal계산을 위해서.
//	CList<double, double>* GetBaseLineList(const CString& strFuncName, CIndicatorList* pIndicatorList);

	void GetSignalTypesbyUsingGraphDataandTwoBaseLine(const CPoint& point, CDoublePacketData *pBaseLineList, CDoublePacketData *pGraphData, CPacket* pSignalPacket);
	void GetSignalTypesbyUsingGraphDataandOneBaseLine(const CPoint& point, CDoublePacketData *pBaseLineList, CDoublePacketData *pGraphData, CPacket* pSignalPacket);
	void GetSignalTypesbyUsingGraphDataandMinMaxBaseLine(const CPoint& point, CDoublePacketData *pBaseLineList, CDoublePacketData *pGraphData, CPacket* pSignalPacket);

	void GetSignalDatabyUsingGraphDataandTwoBaseLine(const CPoint& point, const double nUpBaseline, const double nDownBaseline, CDoublePacketData *pGraphData, CPacket* pSignalPacket);
	void GetSignalDatabyUsingGraphDataandOneBaseLine(const CPoint& point, const double nBaseline, CDoublePacketData *pGraphData, CPacket* pSignalPacket);

	void GetPacketDataList(const CString& strGraphName, const CString &strSubGraphName, CPacketList *pPacketList, CDoublePacketData &dRealDataList);
	CPoint GetPoint(CPacketList* pPacketList, CPacket* pSignalPacket);
	void SetData(const CPoint &point, const int nUpdateIndex, const double dData, CPacket* pPacket);
};

#endif // !defined(AFX_SIGNAL_H__43DE1F04_C959_4E93_BD35_979670DF8235__INCLUDED_)
