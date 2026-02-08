// SterrBnLower.h: interface for the CSterrBnLower class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STERRBNLOWER_H__71827392_D2F9_474B_AB0E_231BFE72E831__INCLUDED_)
#define AFX_STERRBNLOWER_H__71827392_D2F9_474B_AB0E_231BFE72E831__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CSterrBnLower  : public CInnerFunction
{
public:
	CSterrBnLower();
	virtual ~CSterrBnLower();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_STERRBNLOWER_H__71827392_D2F9_474B_AB0E_231BFE72E831__INCLUDED_)
