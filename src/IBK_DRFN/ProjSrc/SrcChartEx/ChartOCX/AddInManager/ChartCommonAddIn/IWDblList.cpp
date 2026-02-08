// IWDblList.cpp: implementation of the CIWDblList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWDblList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// _IWRAPPER_IUNKNOWN_MANAGER_IMP
//////////////////////////////////////////////////////////////////////
_IWRAPPER_IUNKNOWN_MANAGER_IMP( CIWDblList)

//////////////////////////////////////////////////////////////////////
// (2006/7/12 - Seung-Won, Bae) Overrides
//////////////////////////////////////////////////////////////////////

// Attributes (head and tail)
// count of elements
int CIWDblList::GetCount() const
{
	if( !m_plDblList) return 0;
	return m_plDblList->GetCount();
}
BOOL CIWDblList::IsEmpty() const
{
	if( !m_plDblList) return TRUE;
	return m_plDblList->IsEmpty();
}

// peek at head or tail
BOOL CIWDblList::GetHead( double &p_dData) const
{
	if( !m_plDblList) return FALSE;
	p_dData = m_plDblList->GetHead();
	return TRUE;
}
BOOL CIWDblList::GetTail( double &p_dData) const
{
	if( !m_plDblList) return FALSE;
	p_dData = m_plDblList->GetTail();
	return TRUE;
}

// Operations
// get head or tail (and remove it) - don't call on empty list !
BOOL CIWDblList::RemoveHead( double &p_dData)
{
	if( !m_plDblList) return FALSE;
	p_dData = m_plDblList->RemoveHead();
	return TRUE;
}
BOOL CIWDblList::RemoveTail( double &p_dData)
{
	if( !m_plDblList) return FALSE;
	p_dData = m_plDblList->RemoveTail();
	return TRUE;
}

// add before head or after tail
POSITION CIWDblList::AddHead( double newElement)
{
	if( !m_plDblList) return NULL;
	return m_plDblList->AddHead( newElement);
}
POSITION CIWDblList::AddTail( double newElement)
{
	if( !m_plDblList) return NULL;
	return m_plDblList->AddTail( newElement);
}

// remove all elements
void CIWDblList::RemoveAll()
{
	if( m_plDblList) m_plDblList->RemoveAll();
}

// iteration
POSITION CIWDblList::GetHeadPosition() const
{
	if( !m_plDblList) return NULL;
	return m_plDblList->GetHeadPosition();
}
POSITION CIWDblList::GetTailPosition() const
{
	if( !m_plDblList) return NULL;
	return m_plDblList->GetTailPosition();
}
BOOL CIWDblList::GetNext( POSITION& rPosition, double &p_dData) const // return *Position++
{
	if( !m_plDblList) return FALSE;
	p_dData = m_plDblList->GetNext( rPosition);
	return TRUE;
}
BOOL CIWDblList::GetPrev( POSITION& rPosition, double &p_dData) const // return *Position--
{
	if( !m_plDblList) return FALSE;
	p_dData = m_plDblList->GetPrev( rPosition);
	return TRUE;
}

// getting/modifying an element at a given position
BOOL CIWDblList::GetAt( POSITION position, double &p_dData) const
{
	if( !m_plDblList) return FALSE;
	p_dData = m_plDblList->GetAt( position);
	return TRUE;
}
void CIWDblList::SetAt( POSITION position, double newElement)
{
	if( m_plDblList) m_plDblList->SetAt( position, newElement);
}
void CIWDblList::RemoveAt( POSITION position)
{
	if( m_plDblList) m_plDblList->RemoveAt( position);
}

// inserting before or after a given position
POSITION CIWDblList::InsertBefore( POSITION position, double newElement)
{
	if( !m_plDblList) return NULL;
	return m_plDblList->InsertBefore( position, newElement);
}
POSITION CIWDblList::InsertAfter( POSITION position, double newElement)
{
	if( !m_plDblList) return NULL;
	return m_plDblList->InsertAfter( position, newElement);
}

// helper functions (note: O(n) speed)
//	defaults to starting at the HEAD, return NULL if not found
POSITION CIWDblList::Find( double searchValue, POSITION startAfter) const
{
	if( !m_plDblList) return NULL;
	return m_plDblList->Find( searchValue, startAfter);
}
//	get the 'nIndex'th element (may return NULL)
POSITION CIWDblList::FindIndex( int nIndex) const
{
	if( !m_plDblList) return NULL;
	return m_plDblList->FindIndex( nIndex);
}
