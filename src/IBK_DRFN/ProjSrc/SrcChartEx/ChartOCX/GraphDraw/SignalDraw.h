// SignalDraw.h: interface for the CSignalDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIGNALDRAW_H__AD5FA349_DFA4_4B06_AD74_99EFAF0ADF04__INCLUDED_)
#define AFX_SIGNALDRAW_H__AD5FA349_DFA4_4B06_AD74_99EFAF0ADF04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Draw.h"
#include "conversion.h"

class CPacket;
class CDisplayAttributes;
class CDisplayDataMinMax;
class CGraphBaseData;
class CDrawingGraphData;
class CSignalData;

class CSignalDraw : public CDraw   
{
public:
	// xScaleManager/복수종목 적용 - xScaleManager - ojtaso (20070404)
	bool DrawSignal(CDC* pDC, 
			const CDrawingGraphData& drawingGraphData,
			const CDisplayAttributes& displayAttributes, 
			CPacket* pPacket, 
			const CList<double, double>* pSignalData,
			CSignalData &p_signalData, 
			const CString& strRQ,
			CXScaleManager* pxScaleManager, int p_nColumn);
	
private:
};

#endif // !defined(AFX_SIGNALDRAW_H__AD5FA349_DFA4_4B06_AD74_99EFAF0ADF04__INCLUDED_)
