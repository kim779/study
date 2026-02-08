// Parser.h: interface for the Parser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARSER_H__60CC984C_819C_11D5_9711_00C026A220A8__INCLUDED_)
#define AFX_PARSER_H__60CC984C_819C_11D5_9711_00C026A220A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// #include "../common/parser.h"

class CParser  
{
public:
	CParser();
	virtual ~CParser();

// Operations
	static CString GetString(CString& szTmp, TCHAR deli);
};

#endif // !defined(AFX_PARSER_H__60CC984C_819C_11D5_9711_00C026A220A8__INCLUDED_)
