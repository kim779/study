// IPacketQueryer.h: interface for the CIPacketQueryer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPACKETQUERYER_H__843AE6B5_A2CB_470B_A9D4_28EE17F338B8__INCLUDED_)
#define AFX_IPACKETQUERYER_H__843AE6B5_A2CB_470B_A9D4_28EE17F338B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2007/3/28
// Comments		: Packet Queryer Interface of PACKET_QUERYER AddIn
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

//********************************************************************
// Caution! Do not update or delete or add in old interface
//	This header was released already to OCX Wrapper etc.
//********************************************************************

interface IDblList;
interface IPacketQueryer : public IUnknown
{
protected:
	virtual ~IPacketQueryer()	{}

// (2007/3/28 - Seung-Won, Bae) Suppoert Double Value List of Packet
public:
	virtual IDblList *	GetDoubleList( const char *p_szChartName, const char *p_szSubGraphName, const int p_nPacketIndex) = 0;
};

#endif // !defined(AFX_IPACKETQUERYER_H__843AE6B5_A2CB_470B_A9D4_28EE17F338B8__INCLUDED_)
