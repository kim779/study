// DrawingPacketFinder.h: interface for the CDrawingPacketFinder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWINGPACKETFINDER_H__F784AE77_D5B1_44C3_9C88_E1CDDCAD23E0__INCLUDED_)
#define AFX_DRAWINGPACKETFINDER_H__F784AE77_D5B1_44C3_9C88_E1CDDCAD23E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <afxtempl.h>

class CPacket;


class CDrawingPacketFinder  
{
public:
	CPacket* GetPracticalPacket(const CString& strIndicatorName, const CList<CPacket*, CPacket*>& packetList) const;
	CPacket* GetPacket(const CList<CPacket*, CPacket*>& packetList, const CString& strPacketName) const;
	CPacket* GetPacket(const CList<CPacket*, CPacket*>& packetList, const int nIndex) const;
	bool GetPacketData(const CPacket* pPacket, const int nIndex, double& dData) const;
};

#endif // !defined(AFX_DRAWINGPACKETFINDER_H__F784AE77_D5B1_44C3_9C88_E1CDDCAD23E0__INCLUDED_)
