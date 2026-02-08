// CalculateData.h: interface for the CCalculateData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCULATEDATA_H__C0CEA852_EBFC_4BFE_A3BA_1FC372980117__INCLUDED_)
#define AFX_CALCULATEDATA_H__C0CEA852_EBFC_4BFE_A3BA_1FC372980117__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IndicatorInfo.h"

class CCalculateData  
{
public:
	CCalculateData(const CString& m_PacketNameForCal = "", CIndicatorInfo* pIndicatorInfo = NULL);
	virtual ~CCalculateData();

	CPoint GetPoint() const;
	CIndicatorInfo* GetIndicatorInfo();
	CString GetPacketNamesForSave() const;

	void SetPoint(const CPoint& point);
	void SetPacketNamesForSave(const CString& strData);
	void SetIndicatorInfo(CIndicatorInfo* pIndicatorInfo);

private:
	CPoint m_point;
	CString m_PacketNamesForSave;
	CIndicatorInfo* m_pIndicatorInfo;
};

#endif // !defined(AFX_CALCULATEDATA_H__C0CEA852_EBFC_4BFE_A3BA_1FC372980117__INCLUDED_)
