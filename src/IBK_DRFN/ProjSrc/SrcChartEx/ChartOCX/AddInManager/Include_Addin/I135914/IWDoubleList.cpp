// IWDoubleList.cpp: implementation of the CIWDoubleList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWDoubleList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// _IWRAPPER_IUNKNOWN_MANAGER_IMP
//////////////////////////////////////////////////////////////////////
_IWRAPPER_IUNKNOWN_MANAGER_IMP( CIWDoubleList)

//////////////////////////////////////////////////////////////////////
// (2006/7/12 - Seung-Won, Bae) Overrides
//////////////////////////////////////////////////////////////////////

// Attributes (head and tail)
// count of elements
int CIWDoubleList::GetCount() const
{
	if( !m_plDoubleList) return 0;
	return m_plDoubleList->GetCount();
}
BOOL CIWDoubleList::IsEmpty() const
{
	if( !m_plDoubleList) return TRUE;
	return m_plDoubleList->IsEmpty();
}

// peek at head or tail
BOOL CIWDoubleList::GetHead( double &p_dData) const
{
	if( !m_plDoubleList) return FALSE;
	p_dData = m_plDoubleList->GetHead();
	return TRUE;
}
BOOL CIWDoubleList::GetTail( double &p_dData) const
{
	if( !m_plDoubleList) return FALSE;
	p_dData = m_plDoubleList->GetTail();
	return TRUE;
}

// Operations
// get head or tail (and remove it) - don't call on empty list !
BOOL CIWDoubleList::RemoveHead( double &p_dData)
{
	if( !m_plDoubleList) return FALSE;
	p_dData = m_plDoubleList->RemoveHead();
	return TRUE;
}
BOOL CIWDoubleList::RemoveTail( double &p_dData)
{
	if( !m_plDoubleList) return FALSE;
	p_dData = m_plDoubleList->RemoveTail();
	return TRUE;
}

// add before head or after tail
POSITION CIWDoubleList::AddHead( double newElement)
{
	if( !m_plDoubleList) return NULL;
	return m_plDoubleList->AddHead( newElement);
}
POSITION CIWDoubleList::AddTail( double newElement)
{
	if( !m_plDoubleList) return NULL;
	return m_plDoubleList->AddTail( newElement);
}

// remove all elements
void CIWDoubleList::RemoveAll()
{
	if( m_plDoubleList) m_plDoubleList->RemoveAll();
}

// iteration
POSITION CIWDoubleList::GetHeadPosition() const
{
	if( !m_plDoubleList) return NULL;
	return m_plDoubleList->GetHeadPosition();
}
POSITION CIWDoubleList::GetTailPosition() const
{
	if( !m_plDoubleList) return NULL;
	return m_plDoubleList->GetTailPosition();
}
BOOL CIWDoubleList::GetNext( POSITION& rPosition, double &p_dData) const // return *Position++
{
	if( !m_plDoubleList) return FALSE;
	p_dData = m_plDoubleList->GetNext( rPosition);
	return TRUE;
}
BOOL CIWDoubleList::GetPrev( POSITION& rPosition, double &p_dData) const // return *Position--
{
	if( !m_plDoubleList) return FALSE;
	p_dData = m_plDoubleList->GetPrev( rPosition);
	return TRUE;
}

// getting/modifying an element at a given position
BOOL CIWDoubleList::GetAt( POSITION position, double &p_dData) const
{
	if( !m_plDoubleList) return FALSE;
	p_dData = m_plDoubleList->GetAt( position);
	return TRUE;
}
void CIWDoubleList::SetAt( POSITION position, double newElement)
{
	if( m_plDoubleList) m_plDoubleList->SetAt( position, newElement);
}
void CIWDoubleList::RemoveAt( POSITION position)
{
	if( m_plDoubleList) m_plDoubleList->RemoveAt( position);
}

// inserting before or after a given position
POSITION CIWDoubleList::InsertBefore( POSITION position, double newElement)
{
	if( !m_plDoubleList) return NULL;
	return m_plDoubleList->InsertBefore( position, newElement);
}
POSITION CIWDoubleList::InsertAfter( POSITION position, double newElement)
{
	if( !m_plDoubleList) return NULL;
	return m_plDoubleList->InsertAfter( position, newElement);
}

// helper functions (note: O(n) speed)
//	defaults to starting at the HEAD, return NULL if not found
POSITION CIWDoubleList::Find( double searchValue, POSITION startAfter) const
{
	if( !m_plDoubleList) return NULL;
	return m_plDoubleList->Find( searchValue, startAfter);
}
//	get the 'nIndex'th element (may return NULL)
POSITION CIWDoubleList::FindIndex( int nIndex) const
{
	if( !m_plDoubleList) return NULL;
	return m_plDoubleList->FindIndex( nIndex);
}
