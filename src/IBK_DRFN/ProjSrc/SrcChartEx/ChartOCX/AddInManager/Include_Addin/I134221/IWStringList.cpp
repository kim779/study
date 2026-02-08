// IWStringList.cpp: implementation of the CIWStringList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWStringList.h"

#include "IWString.h"		// for CIWString

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// _IWRAPPER_IUNKNOWN_MANAGER_IMP
//////////////////////////////////////////////////////////////////////
_IWRAPPER_IUNKNOWN_MANAGER_IMP( CIWStringList)

//////////////////////////////////////////////////////////////////////
// (2006/7/12 - Seung-Won, Bae) Overrides
//////////////////////////////////////////////////////////////////////

// Attributes (head and tail)
// count of elements
int CIWStringList::GetCount() const
{
	if( !m_plStringList) return 0;
	return m_plStringList->GetCount();
}
BOOL CIWStringList::IsEmpty() const
{
	if( !m_plStringList) return TRUE;
	return m_plStringList->IsEmpty();
}

// peek at head or tail
BOOL CIWStringList::GetHead( ILPCSTR &p_pszData) const
{
	p_pszData = NULL;
	if( !m_plStringList) return FALSE;
	p_pszData = CIWString::CreateObject( m_plStringList->GetHead());
	return TRUE;
}
BOOL CIWStringList::GetTail( ILPCSTR &p_pszData) const
{
	p_pszData = NULL;
	if( !m_plStringList) return FALSE;
	p_pszData = CIWString::CreateObject( m_plStringList->GetTail());
	return TRUE;
}

// Operations
// get head or tail (and remove it) - don't call on empty list !
BOOL CIWStringList::RemoveHead( ILPCSTR &p_pszData)
{
	p_pszData = NULL;
	if( !m_plStringList) return FALSE;
	p_pszData = CIWString::CreateObject( m_plStringList->RemoveHead());
	return TRUE;
}
BOOL CIWStringList::RemoveTail( ILPCSTR &p_pszData)
{
	p_pszData = NULL;
	if( !m_plStringList) return FALSE;
	p_pszData = CIWString::CreateObject( m_plStringList->RemoveTail());
	return TRUE;
}

// add before head or after tail
POSITION CIWStringList::AddHead( const char *newElement)
{
	if( !m_plStringList) return NULL;
	return m_plStringList->AddHead( newElement);
}
POSITION CIWStringList::AddTail( const char *newElement)
{
	if( !m_plStringList) return NULL;
	return m_plStringList->AddTail( newElement);
}

// remove all elements
void CIWStringList::RemoveAll()
{
	if( m_plStringList) m_plStringList->RemoveAll();
}

// iteration
POSITION CIWStringList::GetHeadPosition() const
{
	if( !m_plStringList) return NULL;
	return m_plStringList->GetHeadPosition();
}
POSITION CIWStringList::GetTailPosition() const
{
	if( !m_plStringList) return NULL;
	return m_plStringList->GetTailPosition();
}
BOOL CIWStringList::GetNext( POSITION& rPosition, ILPCSTR &p_pszData) const // return *Position++
{
	p_pszData = NULL;
	if( !m_plStringList) return FALSE;
	p_pszData = CIWString::CreateObject( m_plStringList->GetNext( rPosition));
	return TRUE;
}
BOOL CIWStringList::GetPrev( POSITION& rPosition, ILPCSTR &p_pszData) const // return *Position--
{
	p_pszData = NULL;
	if( !m_plStringList) return FALSE;
	p_pszData = CIWString::CreateObject( m_plStringList->GetPrev( rPosition));
	return TRUE;
}

// getting/modifying an element at a given position
BOOL CIWStringList::GetAt( POSITION position, ILPCSTR &p_pszData) const
{
	p_pszData = NULL;
	if( !m_plStringList) return FALSE;
	p_pszData = CIWString::CreateObject( m_plStringList->GetAt( position));
	return TRUE;
}
void CIWStringList::SetAt( POSITION position, const char *newElement)
{
	if( m_plStringList) m_plStringList->SetAt( position, newElement);
}
void CIWStringList::RemoveAt( POSITION position)
{
	if( m_plStringList) m_plStringList->RemoveAt( position);
}

// inserting before or after a given position
POSITION CIWStringList::InsertBefore( POSITION position, const char *newElement)
{
	if( !m_plStringList) return NULL;
	return m_plStringList->InsertBefore( position, newElement);
}
POSITION CIWStringList::InsertAfter( POSITION position, const char *newElement)
{
	if( !m_plStringList) return NULL;
	return m_plStringList->InsertAfter( position, newElement);
}

// helper functions (note: O(n) speed)
//	defaults to starting at the HEAD, return NULL if not found
POSITION CIWStringList::Find( const char *searchValue, POSITION startAfter) const
{
	if( !m_plStringList) return NULL;
	return m_plStringList->Find( searchValue, startAfter);
}
//	get the 'nIndex'th element (may return NULL)
POSITION CIWStringList::FindIndex( int nIndex) const
{
	if( !m_plStringList) return NULL;
	return m_plStringList->FindIndex( nIndex);
}
