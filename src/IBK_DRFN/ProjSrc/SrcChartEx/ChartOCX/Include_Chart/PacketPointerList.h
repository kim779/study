// PacketPointerList.h: interface for the CPacketPointerList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKETPOINTERLIST_H__639CE188_9C3B_4D23_AA88_3D88682998AD__INCLUDED_)
#define AFX_PACKETPOINTERLIST_H__639CE188_9C3B_4D23_AA88_3D88682998AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CPacket;

class CSubGraphPacketList  
{
public:
	CSubGraphPacketList& operator=(const CSubGraphPacketList& data);

public:
	void RemoveAll();
	int GetCount() const;

	POSITION GetHeadPosition() const;
	POSITION GetHeadPosition();
	POSITION GetTailPosition() const;
	POSITION GetTailPosition();
	CPacket* GetHeadPacket() const;
	CPacket* GetHeadPacket();
	CPacket* GetTailPacket() const;
	CPacket* GetTailPacket();
	CPacket* GetPacket(const int nIndex);
	CPacket* GetNext(POSITION& pos) const;
	CPacket* GetNext(POSITION& pos);
	const CList < CPacket *, CPacket *> &GetSubGraphPacketList( void) const;

	// 같은 packet이 존재하는지 확인한다.
	bool IsExistenceSamePacket(CPacket* pPacket) const;

private:
	CList<CPacket*, CPacket*> m_packetList;
	void SetData(const CList<CPacket*, CPacket*>& packetList);

// (2006/12/11 - Seung-Won, Bae) Support New Interface for Multi-Same Indicator.
//		Check. Is  Packet unique for another same indicator?
public:
	void									AddTail2( CPacket *pPacket);
};

class CIndicator;
class CGraphCalculateData
{
public:
	CGraphCalculateData& operator=(const CGraphCalculateData& data);

public:
	CGraphCalculateData(CPacket*& pSignalPacket, CIndicator *&p_pIndicator);
	void RemoveAll();
	void AddTail(CSubGraphPacketList* pSubGraphPacketList);

	const CList<CSubGraphPacketList*, CSubGraphPacketList*>& GetSubGraphPacketList() const;
	CList<CSubGraphPacketList*, CSubGraphPacketList*>* GetSubGraphPacketList();
	CPacket* GetSignalPacket() const;

	int GetCount() const;

	POSITION GetHeadPosition() const;
	POSITION GetHeadPosition();
	CSubGraphPacketList* GetHead() const;
	CSubGraphPacketList* GetHead();
	CSubGraphPacketList* GetTail() const;
	CSubGraphPacketList* GetTail();
	CSubGraphPacketList* GetAt(const int nIndex);
	CSubGraphPacketList* GetNext(POSITION& pos) const;
	CSubGraphPacketList* GetNext(POSITION& pos);

	void SetSignalPacket(CPacket* pSignalPacket);

private:
	void SetData(const CList<CSubGraphPacketList*, CSubGraphPacketList*>& subGraphPacketList);

private:
	CList<CSubGraphPacketList*, CSubGraphPacketList*> m_subGraphPacketList;
	CPacket*& m_pSignalPacket;				// 신호 계산 packet

// (2006/9/8 - Seung-Won, Bae) the Temp Variables for Calculating 
protected:
	CIndicator *&m_pIndicator;
public:
	CIndicator *&GetIndicator( void) const	{	return m_pIndicator;	}
};


/*//추후에 공식 수정시 사용하기
class CSubGraphCalculateData
{
public:
	CSubGraphCalculateData& operator=(const CSubGraphCalculateData& data);

public:
	CSubGraphCalculateData(const bool bIsHiding = false);
	void RemoveAll();
	void AddTail(CSubGraphPacketList* pSubGraphPacketList);

	void SetHiding(const bool bIsHiding);

	bool IsHiding() const;
	const CList<CSubGraphPacketList*, CSubGraphPacketList*>& GetSubGraphPacketList() const;
	CList<CSubGraphPacketList*, CSubGraphPacketList*>* GetSubGraphPacketList();

	int GetCount() const;

	POSITION GetHeadPosition() const;
	POSITION GetHeadPosition();
	CSubGraphPacketList* GetHead() const;
	CSubGraphPacketList* GetHead();
	CSubGraphPacketList* GetTail() const;
	CSubGraphPacketList* GetTail();
	CSubGraphPacketList* GetAt(const int nIndex);
	CSubGraphPacketList* GetNext(POSITION& pos) const;
	CSubGraphPacketList* GetNext(POSITION& pos);

private:
	bool m_bIsHiding;
	CList<CSubGraphPacketList*, CSubGraphPacketList*> m_subGraphPacketList;

	void SetData(const CList<CSubGraphPacketList*, CSubGraphPacketList*>& subGraphPacketList);
};

class CGraphCalculateData
{
public:
	CGraphCalculateData& operator=(const CGraphCalculateData& data);

public:
	CGraphCalculateData();
	void RemoveAll();
	void AddTail(CSubGraphCalculateData* pSubGraphCalData);

	const CList<CSubGraphCalculateData*, CSubGraphCalculateData*>& GetSubGraphCalDataList() const;
	CList<CSubGraphCalculateData*, CSubGraphCalculateData*>* GetSubGraphCalDataList();

	int GetCount() const;

	POSITION GetHeadPosition() const;
	POSITION GetHeadPosition();
	CSubGraphCalculateData* GetHead() const;
	CSubGraphCalculateData* GetHead();
	CSubGraphCalculateData* GetTail() const;
	CSubGraphCalculateData* GetTail();
	CSubGraphCalculateData* GetAt(const int nIndex);
	CSubGraphCalculateData* GetNext(POSITION& pos) const;
	CSubGraphCalculateData* GetNext(POSITION& pos);

private:
	CList<CSubGraphCalculateData*, CSubGraphCalculateData*> m_subGraphCalDataList;

	void SetData(const CList<CSubGraphCalculateData*, CSubGraphCalculateData*>& subGraphCalDataList);
};*/

#endif // !defined(AFX_PACKETPOINTERLIST_H__639CE188_9C3B_4D23_AA88_3D88682998AD__INCLUDED_)
