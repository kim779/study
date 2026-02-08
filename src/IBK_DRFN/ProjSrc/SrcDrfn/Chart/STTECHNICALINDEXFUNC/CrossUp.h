// CrossUp.h: interface for the CCrossUp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CROSSUP_H__261B7001_B8A6_480E_A42F_08D85E302CC7__INCLUDED_)
#define AFX_CROSSUP_H__261B7001_B8A6_480E_A42F_08D85E302CC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CCrossUp  : public CInnerFunction
{
public:
	CCrossUp();
	virtual ~CCrossUp();
	inline CString GetName(long lType);
	inline CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName0;
	CString m_strName1;

};

#endif // !defined(AFX_CROSSUP_H__261B7001_B8A6_480E_A42F_08D85E302CC7__INCLUDED_)
