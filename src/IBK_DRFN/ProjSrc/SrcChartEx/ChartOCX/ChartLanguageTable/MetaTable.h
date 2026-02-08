// MetaTable.h: interface for the CMetaTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METATABLE_H__E74B8439_C5AA_47D0_BE8B_BCB190322CA9__INCLUDED_)
#define AFX_METATABLE_H__E74B8439_C5AA_47D0_BE8B_BCB190322CA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <assert.h>									// for assert()
#include "../Include_Chart/Dll_Load/IMetaTable.h"	// for IMetaTable
#include "../Include_Chart/MapStringToWord.h"		// for CMapStringToWord

class CCodePage;
class CMetaTable : public CObject
{
public:
	CMetaTable();
	virtual ~CMetaTable();

protected:
	CString				m_strMetaFileStringData;
	CCodePage *			( m_pCodePage[ IMetaTable::CODE_PAGE_END]);

public:
	static const char *	m_szNullString;
	static const char *	( m_szMetaTexts[ IMetaTable::META_CODE_END + 1]);
	CMapStringToWord	m_mapIndicatorID;
	const char *		( m_szIndicatorNames[ IMetaTable::EDDC_INDICATOR_COUNT + 1]);

public:
	void			LoadMetaTable( const char *p_szMetaTableName, const char *p_szDataFolder);
	CCodePage *		GetCodePage( IMetaTable::E_CODEPAGE p_eCodePage)
					{	return ( IMetaTable::PACKET_NAME <= p_eCodePage && p_eCodePage < IMetaTable::CODE_PAGE_END) ? m_pCodePage[ p_eCodePage] : NULL;	}
};

#endif // !defined(AFX_METATABLE_H__E74B8439_C5AA_47D0_BE8B_BCB190322CA9__INCLUDED_)
