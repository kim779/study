// Token.h: interface for the CToken class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOKEN_H__7DD65574_5A76_44A4_AF04_0E44C5F4827B__INCLUDED_)
#define AFX_TOKEN_H__7DD65574_5A76_44A4_AF04_0E44C5F4827B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CToken : public CString
{
// Constructors
public:
	CToken(const char* src, unsigned int type, int dispiid = -1)
		: CString(src), _type(type), _dispiid(dispiid) {
	}
	CToken(const CToken& src)
		: CString(src), _type(src._type), _dispiid(src._dispiid) {
	}

// Attributes
public:
	typedef enum tokenType {
		typeEndOfExpr = -1,
		typeUnknown = 0,
		typeOperator = 1,
		typeOperand = 2,
		typeLParenthesis = 3, /* left - ( */
		typeRParenthesis = 4, /* right - ) */
	} ttype;

	unsigned int _type;
	int _dispiid;

// Operations
public:

// Implementation
public:
	virtual ~CToken()
	{
	}
};

#endif // !defined(AFX_TOKEN_H__7DD65574_5A76_44A4_AF04_0E44C5F4827B__INCLUDED_)
