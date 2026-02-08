// Data.h: interface for the CData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_H__2B78372A_DBFA_45D3_8AF2_15D6966C106F__INCLUDED_)
#define AFX_DATA_H__2B78372A_DBFA_45D3_8AF2_15D6966C106F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Addin_134221/I134221/_IStringList.h"	// for IStringList
#include "../Include_Addin_134221/I134221/_IDoubleList.h"	// for IDoubleList

class CData : public CObject  
{
public:
	long m_lDataCount;
	CString m_strPacketName;
	CString m_strPacketType;
	CData();
//	CData(const CData& ci);
//	const CData& operator=(const CData& stringSrc);
	virtual ~CData();
//	CStringList m_RowDataList;
	IStringList *m_pistrDataList;
	IDoubleList *m_pidDataList;
//	CList< CString, CString> m_strDataList;
//	CList<double, double> m_dDataList;
};

#endif // !defined(AFX_DATA_H__2B78372A_DBFA_45D3_8AF2_15D6966C106F__INCLUDED_)
