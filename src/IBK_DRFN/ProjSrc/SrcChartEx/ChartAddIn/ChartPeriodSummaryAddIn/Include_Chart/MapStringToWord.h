// MapStringToWord.h: interface for the CMapStringToWord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPSTRINGTOWORD_H__E06CB198_8340_4F54_86E2_F18D86F8A54C__INCLUDED_)
#define AFX_MAPSTRINGTOWORD_H__E06CB198_8340_4F54_86E2_F18D86F8A54C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CMapStringToWord : public CMapStringToPtr
{
public:
	void	SetAt( LPCTSTR key, WORD p_wWord)
			{	CMapStringToPtr::SetAt( key, ( void *)p_wWord);	}
	BOOL	Lookup( LPCTSTR key, WORD &rValue) const
			{
				void *pValue;
				BOOL bResult = CMapStringToPtr::Lookup( key, pValue);
				rValue = ( WORD)pValue;
				return bResult;
			}
};

#endif // !defined(AFX_MAPSTRINGTOWORD_H__E06CB198_8340_4F54_86E2_F18D86F8A54C__INCLUDED_)
