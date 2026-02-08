// DayOfWeek.h: interface for the CDayOfWeek class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DAYOFWEEK_H__F53ADA5C_73E5_4DE4_95B0_239DFA223165__INCLUDED_)
#define AFX_DAYOFWEEK_H__F53ADA5C_73E5_4DE4_95B0_239DFA223165__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CDayOfWeek  : public CInnerFunction
{
public:
	CDayOfWeek();
	virtual ~CDayOfWeek();
	CString GetName(long lType);
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType,  const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_DAYOFWEEK_H__F53ADA5C_73E5_4DE4_95B0_239DFA223165__INCLUDED_)
