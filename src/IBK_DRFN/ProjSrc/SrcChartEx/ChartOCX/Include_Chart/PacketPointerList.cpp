// PacketPointerList.cpp: implementation of the CPacketPointerList class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PacketPointerList.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// CSubGraphPacketList

CSubGraphPacketList& CSubGraphPacketList::operator=(const CSubGraphPacketList& data)
{
	if(this == &data)
		return *this;

	SetData( data.GetSubGraphPacketList());
	return *this;
}

// public =====================================================================
void CSubGraphPacketList::RemoveAll()
{
	m_packetList.RemoveAll();
}

// ----------------------------------------------------------------------------
int CSubGraphPacketList::GetCount() const
{
	return m_packetList.GetCount();
}

POSITION CSubGraphPacketList::GetHeadPosition() const
{
	if(m_packetList.GetCount() <= 0)
		return NULL;
	return m_packetList.GetHeadPosition();
}

POSITION CSubGraphPacketList::GetHeadPosition()
{
	if(m_packetList.GetCount() <= 0)
		return NULL;
	return m_packetList.GetHeadPosition();
}

POSITION CSubGraphPacketList::GetTailPosition() const
{
	if(m_packetList.GetCount() <= 0)
		return NULL;
	return m_packetList.GetTailPosition();
}

POSITION CSubGraphPacketList::GetTailPosition()
{
	if(m_packetList.GetCount() <= 0)
		return NULL;
	return m_packetList.GetTailPosition();
}

// ----------------------------------------------------------------------------
CPacket* CSubGraphPacketList::GetHeadPacket() const
{
	if(m_packetList.GetCount() <= 0)
		return NULL;
	return m_packetList.GetHead();
}

CPacket* CSubGraphPacketList::GetHeadPacket()
{
	if(m_packetList.GetCount() <= 0)
		return NULL;
	return m_packetList.GetHead();
}

CPacket* CSubGraphPacketList::GetTailPacket() const
{
	if(m_packetList.GetCount() <= 0)
		return NULL;
	return m_packetList.GetTail();
}

CPacket* CSubGraphPacketList::GetTailPacket()
{
	if(m_packetList.GetCount() <= 0)
		return NULL;
	return m_packetList.GetTail();
}

CPacket* CSubGraphPacketList::GetPacket(const int nIndex)
{
	POSITION pos = m_packetList.FindIndex(nIndex);
	if(pos == NULL)
		return NULL;
	return m_packetList.GetAt(pos);
}

CPacket* CSubGraphPacketList::GetNext(POSITION& pos) const
{
	if(pos == NULL)
		return NULL;
	return m_packetList.GetNext(pos);
}

CPacket* CSubGraphPacketList::GetNext(POSITION& pos)
{
	if(pos == NULL)
		return NULL;
	return m_packetList.GetNext(pos);
}

// ----------------------------------------------------------------------------
// 같은 packet이 존재하는지 확인한다.
bool CSubGraphPacketList::IsExistenceSamePacket(CPacket* pPacket) const
{
	if(pPacket == NULL)
		return false;

	POSITION pos = m_packetList.GetHeadPosition();
	while(pos != NULL){
		CPacket* pTemp = m_packetList.GetNext(pos);
		if(pPacket == pTemp)
			return true;
	}
	return false;
}

const CList < CPacket *, CPacket *> &CSubGraphPacketList::GetSubGraphPacketList( void) const
{
	return m_packetList;
}

// private ====================================================================
void CSubGraphPacketList::SetData(const CList<CPacket*, CPacket*>& packetList)
{
	m_packetList.RemoveAll();
	POSITION pos = packetList.GetHeadPosition();
	while(pos != NULL){
		CPacket* pPacket = packetList.GetNext(pos);
		if(pPacket != NULL)
			m_packetList.AddTail(pPacket);
	}
}

// (2006/12/11 - Seung-Won, Bae) Support New Interface for Multi-Same Indicator.
//		Check. Is  Packet unique for another same indicator?
void CSubGraphPacketList::AddTail2( CPacket *pPacket)
{
	if(pPacket != NULL)
		m_packetList.AddTail(pPacket);
}

///////////////////////////////////////////////////////////////////////////////
// CGraphCalculateData

CGraphCalculateData& CGraphCalculateData::operator=(const CGraphCalculateData& data)
{
	if(this == &data)
		return *this;

	SetData(data.GetSubGraphPacketList());
	m_pSignalPacket		= data.GetSignalPacket();
	m_pIndicator		= data.GetIndicator();	// (2006/9/8 - Seung-Won, Bae) the Temp Variables for Calculating 
	return *this;
}

CGraphCalculateData:: CGraphCalculateData( CPacket*& pSignalPacket, CIndicator *&p_pIndicator) :
	m_pSignalPacket(pSignalPacket),
	m_pIndicator( p_pIndicator)	// (2006/9/8 - Seung-Won, Bae) the Temp Variables for Calculating 
{
}

// public =====================================================================
void CGraphCalculateData::RemoveAll()
{
	m_subGraphPacketList.RemoveAll();
}

void CGraphCalculateData::AddTail(CSubGraphPacketList* pSubGraphPacketList)
{
	if(pSubGraphPacketList != NULL)
		m_subGraphPacketList.AddTail(pSubGraphPacketList);
}

// ----------------------------------------------------------------------------
const CList<CSubGraphPacketList*, CSubGraphPacketList*>& CGraphCalculateData::GetSubGraphPacketList() const
{
	return m_subGraphPacketList;
}

CList<CSubGraphPacketList*, CSubGraphPacketList*>* CGraphCalculateData::GetSubGraphPacketList()
{
	return &m_subGraphPacketList;
}

CPacket* CGraphCalculateData::GetSignalPacket() const
{
	return m_pSignalPacket;
}

// ----------------------------------------------------------------------------
int CGraphCalculateData::GetCount() const
{
	return m_subGraphPacketList.GetCount();
}

POSITION CGraphCalculateData::GetHeadPosition() const
{
	if(m_subGraphPacketList.GetCount() <= 0)
		return NULL;
	return m_subGraphPacketList.GetHeadPosition();
}

POSITION CGraphCalculateData::GetHeadPosition()
{
	if(m_subGraphPacketList.GetCount() <= 0)
		return NULL;
	return m_subGraphPacketList.GetHeadPosition();
}

// ----------------------------------------------------------------------------
CSubGraphPacketList* CGraphCalculateData::GetHead() const
{
	if(m_subGraphPacketList.GetCount() <= 0)
		return NULL;
	return m_subGraphPacketList.GetHead();
}

CSubGraphPacketList* CGraphCalculateData::GetHead()
{
	if(m_subGraphPacketList.GetCount() <= 0)
		return NULL;
	return m_subGraphPacketList.GetHead();
}

CSubGraphPacketList* CGraphCalculateData::GetTail() const
{
	if(m_subGraphPacketList.GetCount() <= 0)
		return NULL;
	return m_subGraphPacketList.GetTail();
}

CSubGraphPacketList* CGraphCalculateData::GetTail()
{
	if(m_subGraphPacketList.GetCount() <= 0)
		return NULL;
	return m_subGraphPacketList.GetTail();
}

CSubGraphPacketList* CGraphCalculateData::GetAt(const int nIndex)
{
	POSITION pos = m_subGraphPacketList.FindIndex(nIndex);
	if(pos == NULL)
		return NULL;
	return m_subGraphPacketList.GetAt(pos);
}

CSubGraphPacketList* CGraphCalculateData::GetNext(POSITION& pos) const
{
	if(pos == NULL)
		return NULL;
	return m_subGraphPacketList.GetNext(pos);
}

CSubGraphPacketList* CGraphCalculateData::GetNext(POSITION& pos)
{
	if(pos == NULL)
		return NULL;
	return m_subGraphPacketList.GetNext(pos);
}

// ----------------------------------------------------------------------------
void CGraphCalculateData::SetSignalPacket(CPacket* pSignalPacket)
{
	m_pSignalPacket = pSignalPacket;
}


// private ====================================================================
void CGraphCalculateData::SetData(const CList<CSubGraphPacketList*, CSubGraphPacketList*>& subGraphPacketList)
{
	m_subGraphPacketList.RemoveAll();
	POSITION pos = subGraphPacketList.GetHeadPosition();
	while(pos != NULL){
		CSubGraphPacketList* pSubGraphPacketList = subGraphPacketList.GetNext(pos);
		if(pSubGraphPacketList != NULL)
			m_subGraphPacketList.AddTail(pSubGraphPacketList);
	}
}


/*///////////////////////////////////////////////////////////////////////////////
// CSubGraphCalculateData

CSubGraphCalculateData& CSubGraphCalculateData::operator=(const CSubGraphCalculateData& data)
{
	if(this == &data)
		return *this;

	m_bIsHiding = data.IsHiding();
	SetData(data.GetSubGraphPacketList());
	return *this;
}

CSubGraphCalculateData::CSubGraphCalculateData(const bool bIsHiding):
	m_bIsHiding(bIsHiding)
{
}

// public =====================================================================
void CSubGraphCalculateData::RemoveAll()
{
	m_subGraphPacketList.RemoveAll();
}

void CSubGraphCalculateData::AddTail(CSubGraphPacketList* pSubGraphPacketList)
{
	if(pSubGraphPacketList != NULL)
		m_subGraphPacketList.AddTail(pSubGraphPacketList);
}

void CSubGraphCalculateData::SetHiding(const bool bIsHiding)
{
	m_bIsHiding = bIsHiding;
}

// ----------------------------------------------------------------------------
bool CSubGraphCalculateData::IsHiding() const
{
	return m_bIsHiding;
}

const CList<CSubGraphPacketList*, CSubGraphPacketList*>& CSubGraphCalculateData::GetSubGraphPacketList() const
{
	return m_subGraphPacketList;
}

CList<CSubGraphPacketList*, CSubGraphPacketList*>* CSubGraphCalculateData::GetSubGraphPacketList()
{
	return &m_subGraphPacketList;
}

// ----------------------------------------------------------------------------
int CSubGraphCalculateData::GetCount() const
{
	return m_subGraphPacketList.GetCount();
}

POSITION CSubGraphCalculateData::GetHeadPosition() const
{
	if(m_subGraphPacketList.GetCount() <= 0)
		return NULL;
	return m_subGraphPacketList.GetHeadPosition();
}

POSITION CSubGraphCalculateData::GetHeadPosition()
{
	if(m_subGraphPacketList.GetCount() <= 0)
		return NULL;
	return m_subGraphPacketList.GetHeadPosition();
}

// ----------------------------------------------------------------------------
CSubGraphPacketList* CSubGraphCalculateData::GetHead() const
{
	if(m_subGraphPacketList.GetCount() <= 0)
		return NULL;
	return m_subGraphPacketList.GetHead();
}

CSubGraphPacketList* CSubGraphCalculateData::GetHead()
{
	if(m_subGraphPacketList.GetCount() <= 0)
		return NULL;
	return m_subGraphPacketList.GetHead();
}

CSubGraphPacketList* CSubGraphCalculateData::GetTail() const
{
	if(m_subGraphPacketList.GetCount() <= 0)
		return NULL;
	return m_subGraphPacketList.GetTail();
}

CSubGraphPacketList* CSubGraphCalculateData::GetTail()
{
	if(m_subGraphPacketList.GetCount() <= 0)
		return NULL;
	return m_subGraphPacketList.GetTail();
}

CSubGraphPacketList* CSubGraphCalculateData::GetAt(const int nIndex)
{
	POSITION pos = m_subGraphPacketList.FindIndex(nIndex);
	if(pos == NULL)
		return NULL;
	return m_subGraphPacketList.GetAt(pos);
}

CSubGraphPacketList* CSubGraphCalculateData::GetNext(POSITION& pos) const
{
	if(pos == NULL)
		return NULL;
	return m_subGraphPacketList.GetNext(pos);
}

CSubGraphPacketList* CSubGraphCalculateData::GetNext(POSITION& pos)
{
	if(pos == NULL)
		return NULL;
	return m_subGraphPacketList.GetNext(pos);
}


// private ====================================================================
void CSubGraphCalculateData::SetData(const CList<CSubGraphPacketList*, CSubGraphPacketList*>& subGraphPacketList)
{
	m_subGraphPacketList.RemoveAll();
	POSITION pos = subGraphPacketList.GetHeadPosition();
	while(pos != NULL){
		CSubGraphPacketList* pSubGraphPacketList = subGraphPacketList.GetNext(pos);
		if(pSubGraphPacketList != NULL)
			m_subGraphPacketList.AddTail(pSubGraphPacketList);
	}
}


///////////////////////////////////////////////////////////////////////////////
// CGraphCalculateData

CGraphCalculateData& CGraphCalculateData::operator=(const CGraphCalculateData& data)
{
	if(this == &data)
		return *this;

	SetData(data.GetSubGraphCalDataList());
	return *this;
}

CGraphCalculateData::CGraphCalculateData()
{
}

// public =====================================================================
void CGraphCalculateData::RemoveAll()
{
	m_subGraphCalDataList.RemoveAll();
}

void CGraphCalculateData::AddTail(CSubGraphCalculateData* pSubGraphCalData)
{
	if(pSubGraphCalData != NULL)
		m_subGraphCalDataList.AddTail(pSubGraphCalData);
}

// ----------------------------------------------------------------------------
const CList<CSubGraphCalculateData*, CSubGraphCalculateData*>& CGraphCalculateData::GetSubGraphCalDataList() const
{
	return m_subGraphCalDataList;
}

CList<CSubGraphCalculateData*, CSubGraphCalculateData*>* CGraphCalculateData::GetSubGraphCalDataList()
{
	return &m_subGraphCalDataList;
}

// ----------------------------------------------------------------------------
int CGraphCalculateData::GetCount() const
{
	return m_subGraphCalDataList.GetCount();
}

POSITION CGraphCalculateData::GetHeadPosition() const
{
	if(m_subGraphCalDataList.GetCount() <= 0)
		return NULL;
	return m_subGraphCalDataList.GetHeadPosition();
}

POSITION CGraphCalculateData::GetHeadPosition()
{
	if(m_subGraphCalDataList.GetCount() <= 0)
		return NULL;
	return m_subGraphCalDataList.GetHeadPosition();
}

// ----------------------------------------------------------------------------
CSubGraphCalculateData* CGraphCalculateData::GetHead() const
{
	if(m_subGraphCalDataList.GetCount() <= 0)
		return NULL;
	return m_subGraphCalDataList.GetHead();
}

CSubGraphCalculateData* CGraphCalculateData::GetHead()
{
	if(m_subGraphCalDataList.GetCount() <= 0)
		return NULL;
	return m_subGraphCalDataList.GetHead();
}

CSubGraphCalculateData* CGraphCalculateData::GetTail() const
{
	if(m_subGraphCalDataList.GetCount() <= 0)
		return NULL;
	return m_subGraphCalDataList.GetTail();
}

CSubGraphCalculateData* CGraphCalculateData::GetTail()
{
	if(m_subGraphCalDataList.GetCount() <= 0)
		return NULL;
	return m_subGraphCalDataList.GetTail();
}

CSubGraphCalculateData* CGraphCalculateData::GetAt(const int nIndex)
{
	POSITION pos = m_subGraphCalDataList.FindIndex(nIndex);
	if(pos == NULL)
		return NULL;
	return m_subGraphCalDataList.GetAt(pos);
}

CSubGraphCalculateData* CGraphCalculateData::GetNext(POSITION& pos) const
{
	if(pos == NULL)
		return NULL;
	return m_subGraphCalDataList.GetNext(pos);
}

CSubGraphCalculateData* CGraphCalculateData::GetNext(POSITION& pos)
{
	if(pos == NULL)
		return NULL;
	return m_subGraphCalDataList.GetNext(pos);
}


// private ====================================================================
void CGraphCalculateData::SetData(const CList<CSubGraphCalculateData*, CSubGraphCalculateData*>& subGraphCalDataList)
{
	m_subGraphCalDataList.RemoveAll();
	POSITION pos = subGraphCalDataList.GetHeadPosition();
	while(pos != NULL){
		CSubGraphCalculateData* pSubGraphCalData = subGraphCalDataList.GetNext(pos);
		if(pSubGraphCalData != NULL)
			m_subGraphCalDataList.AddTail(pSubGraphCalData);
	}
}
*/