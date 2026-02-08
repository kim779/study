// IWHelpMessageMap.cpp: implementation of the CIWHelpMessageMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWHelpMessageMap.h"

#include "IWString.h"				// for CIWString

//////////////////////////////////////////////////////////////////////
// _IWRAPPER_IUNKNOWN_MANAGER_IMP
//////////////////////////////////////////////////////////////////////
_IWRAPPER_IUNKNOWN_MANAGER_IMP( CIWHelpMessageMap)

//////////////////////////////////////////////////////////////////////
// Attributes
//////////////////////////////////////////////////////////////////////
// number of elements
int CIWHelpMessageMap::GetCount() const
{
	if( !m_pHelpMessageMap) return 0;
	return m_pHelpMessageMap->GetCount();
}
BOOL CIWHelpMessageMap::IsEmpty() const
{
	if( !m_pHelpMessageMap) return TRUE;
	return m_pHelpMessageMap->IsEmpty();
}
// Lookup
BOOL CIWHelpMessageMap::Lookup(	LPCTSTR key, ILPCSTR &p_pszValue) const
{
	if( !m_pHelpMessageMap) return FALSE;

	CString strValue;
	BOOL bResult = m_pHelpMessageMap->Lookup( key, strValue);
	p_pszValue = CIWString::CreateObject( strValue);
	return bResult;
}
BOOL CIWHelpMessageMap::LookupKey( LPCTSTR key, ILPCSTR &p_pszKey) const
{
	if( !m_pHelpMessageMap) return FALSE;

	const char *szKey = NULL;
	BOOL bResult = m_pHelpMessageMap->LookupKey( key, szKey);
	p_pszKey = CIWString::CreateObject( szKey);
	return bResult;
}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////
// iterating all (key, value) pairs
POSITION CIWHelpMessageMap::GetStartPosition() const
{
	if( !m_pHelpMessageMap) return NULL;
	return m_pHelpMessageMap->GetStartPosition();
}
void CIWHelpMessageMap::GetNextAssoc( POSITION& rNextPosition, ILPCSTR &p_pszKey, ILPCSTR &p_pszValue) const
{
	p_pszKey	= NULL;
	p_pszValue	= NULL;
	if( !m_pHelpMessageMap) return;

	CString strKey, strValue;
	m_pHelpMessageMap->GetNextAssoc( rNextPosition, strKey, strValue);
	p_pszKey	= CIWString::CreateObject( strKey);
	p_pszValue	= CIWString::CreateObject( strValue);
}
// advanced features for derived classes
UINT CIWHelpMessageMap::GetHashTableSize() const
{
	if( !m_pHelpMessageMap) return 0;
	return m_pHelpMessageMap->GetHashTableSize();
}
