// DrawCalculator.h: interface for the CDrawCalculator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWCALCULATOR_H__063F6F64_A9B0_4A6A_8C47_9974CA942D7C__INCLUDED_)
#define AFX_DRAWCALCULATOR_H__063F6F64_A9B0_4A6A_8C47_9974CA942D7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPacket;

class CDrawCalculator  
{
public:
	CDrawCalculator();
	virtual ~CDrawCalculator();

	int GetDataDrawCount(CPacket* dataPacket, int dataCount);
	bool GetInitDataUpCheck(CPacket* dataPacket, int nUnitSize, POSITION startPos);
	int PFdataDrawingCount(int dataCount, CPacket* dataPacket, POSITION startPos, bool UPCheck, int nUnitSize, int checkData, int checkCount, int checkConvert);
};

#endif // !defined(AFX_DRAWCALCULATOR_H__063F6F64_A9B0_4A6A_8C47_9974CA942D7C__INCLUDED_)
