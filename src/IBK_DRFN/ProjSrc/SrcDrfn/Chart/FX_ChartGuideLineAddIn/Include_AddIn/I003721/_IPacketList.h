// IPacketList.h: interface for the IPacketList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPACKETLIST_H__14E92AC8_5337_4E3E_AF37_69F0544719FE__INCLUDED_)
#define AFX_IPACKETLIST_H__14E92AC8_5337_4E3E_AF37_69F0544719FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "_AddInVer.h"			// for AddIn Version Define

interface IPacketList : public IUnknown
{
protected:
	virtual ~IPacketList()	{}

public:
// Attributes (head and tail)
	// count of elements
	virtual int		GetCount() const	= 0;
	virtual BOOL	IsEmpty() const		= 0;

	// peek at head or tail
	virtual IPacket *	GetHead() const	= 0;
	virtual IPacket *	GetTail() const	= 0;

// Operations
	// get head or tail (and remove it) - don't call on empty list !
	virtual IPacket *	RemoveHead()	= 0;
	virtual IPacket *	RemoveTail()	= 0;

	// add before head or after tail
	virtual POSITION	AddHead(IPacket *newElement)	= 0;
	virtual POSITION	AddTail(IPacket *newElement)	= 0;

	// add another list of elements before head or after tail
	virtual void	AddHead(IPacketList* pNewList)	= 0;
	virtual void	AddTail(IPacketList* pNewList)	= 0;

	// remove all elements
	virtual void	RemoveAll()	= 0;

	// iteration
	virtual POSITION	GetHeadPosition() const				= 0;
	virtual POSITION	GetTailPosition() const				= 0;
	virtual IPacket *	GetNext(POSITION& rPosition) const	= 0; // return *Position++
	virtual IPacket *	GetPrev(POSITION& rPosition) const	= 0; // return *Position--

	// getting/modifying an element at a given position
	virtual IPacket *	GetAt(POSITION position) const				= 0;
	virtual void		SetAt(POSITION pos, IPacket *newElement)	= 0;
	virtual void		RemoveAt(POSITION position)					= 0;

	// inserting before or after a given position
	virtual POSITION	InsertBefore(POSITION position, IPacket *newElement)	= 0;
	virtual POSITION	InsertAfter(POSITION position, IPacket *newElement)		= 0;

	// helper functions (note: O(n) speed)
	virtual POSITION	Find(IPacket *searchValue, POSITION startAfter = NULL) const	= 0;
		// defaults to starting at the HEAD, return NULL if not found
	virtual POSITION	FindIndex(int nIndex) const										= 0;
		// get the 'nIndex'th element (may return NULL)
};

typedef CList <IPacket *, IPacket *> CIPacketListBase;
class CIPacketList : public CIPacketListBase, public IPacketList
{
public:
	~CIPacketList()	{}

// (2004.10.07, 배승원) Interface Wrapper의 기본 Interface (IUnknown용)를 제공한다.
protected:
	virtual ULONG	STDMETHODCALLTYPE AddRef(	void)	{	return 1;	}
	virtual ULONG	STDMETHODCALLTYPE Release(	void)	{	return 1;	}
	virtual HRESULT	STDMETHODCALLTYPE QueryInterface( REFIID iid, void ** ppvObject)	{	return E_NOINTERFACE;	}

public:
// Attributes (head and tail)
	// count of elements
	virtual int		GetCount() const	{	return CIPacketListBase::GetCount();	}
	virtual BOOL	IsEmpty() const		{	return CIPacketListBase::IsEmpty();	}

	// peek at head or tail
	virtual IPacket *	GetHead() const	{	return CIPacketListBase::GetHead();	}
	virtual IPacket *	GetTail() const	{	return CIPacketListBase::GetTail();	}

// Operations
	// get head or tail (and remove it) - don't call on empty list !
	virtual IPacket *	RemoveHead()	{	return CIPacketListBase::RemoveHead();	}
	virtual IPacket *	RemoveTail()	{	return CIPacketListBase::RemoveTail();	}

	// add before head or after tail
	virtual POSITION	AddHead(IPacket *newElement)	{	return CIPacketListBase::AddHead( newElement);	}
	virtual POSITION	AddTail(IPacket *newElement)	{	return CIPacketListBase::AddTail( newElement);	}

	// add another list of elements before head or after tail
	virtual void	AddHead(IPacketList* pNewList)	{	CIPacketListBase::AddHead( ( CIPacketListBase *)pNewList);	}
	virtual void	AddTail(IPacketList* pNewList)	{	CIPacketListBase::AddTail( ( CIPacketListBase *)pNewList);	}

	// remove all elements
	virtual void	RemoveAll()	{	CIPacketListBase::RemoveAll();	}

	// iteration
	virtual POSITION	GetHeadPosition() const				{	return CIPacketListBase::GetHeadPosition();	}
	virtual POSITION	GetTailPosition() const				{	return CIPacketListBase::GetTailPosition();	}
	virtual IPacket *	GetNext(POSITION& rPosition) const	{	return CIPacketListBase::GetNext( rPosition);	} // return *Position++
	virtual IPacket *	GetPrev(POSITION& rPosition) const	{	return CIPacketListBase::GetPrev( rPosition);	} // return *Position--

	// getting/modifying an element at a given position
	virtual IPacket *	GetAt(POSITION position) const				{	return CIPacketListBase::GetAt( position);	}
	virtual void		SetAt(POSITION pos, IPacket *newElement)	{	CIPacketListBase::SetAt( pos, newElement);	}
	virtual void		RemoveAt(POSITION position)					{	CIPacketListBase::RemoveAt( position);	}

	// inserting before or after a given position
	virtual POSITION	InsertBefore(POSITION position, IPacket *newElement)	{	return CIPacketListBase::InsertBefore( position, newElement);	}
	virtual POSITION	InsertAfter(POSITION position, IPacket *newElement)		{	return CIPacketListBase::InsertAfter( position, newElement);	}

	// helper functions (note: O(n) speed)
	virtual POSITION	Find(IPacket *searchValue, POSITION startAfter = NULL) const	{	return CIPacketListBase::Find( searchValue, startAfter);	}
		// defaults to starting at the HEAD, return NULL if not found
	virtual POSITION	FindIndex(int nIndex) const										{	return CIPacketListBase::FindIndex( nIndex);	}
		// get the 'nIndex'th element (may return NULL)
};

#endif // !defined(AFX_IPACKETLIST_H__14E92AC8_5337_4E3E_AF37_69F0544719FE__INCLUDED_)
