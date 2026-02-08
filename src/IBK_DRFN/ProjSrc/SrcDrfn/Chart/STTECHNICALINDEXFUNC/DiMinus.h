// DiMinus.h: interface for the CDiMinus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIMINUS_H__BF9ED8A6_1145_487A_B737_BC7267749EB0__INCLUDED_)
#define AFX_DIMINUS_H__BF9ED8A6_1145_487A_B737_BC7267749EB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CDiMinus  : public CInnerFunction
{
public:
	CDiMinus();
	virtual ~CDiMinus();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_DIMINUS_H__BF9ED8A6_1145_487A_B737_BC7267749EB0__INCLUDED_)
