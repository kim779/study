// CrossDown.h: interface for the CCrossDown class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CROSSDOWN_H__6E9047BC_C093_498D_AD73_1A88C95B1152__INCLUDED_)
#define AFX_CROSSDOWN_H__6E9047BC_C093_498D_AD73_1A88C95B1152__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CCrossDown  : public CInnerFunction
{
public:
	CCrossDown();
	virtual ~CCrossDown();
	inline CString GetName(long lType);
	inline CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName0;
	CString m_strName1;

};

#endif // !defined(AFX_CROSSDOWN_H__6E9047BC_C093_498D_AD73_1A88C95B1152__INCLUDED_)
