// MinutesToTime.h: interface for the CMinutesToTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINUTESTOTIME_H__9475CA87_AABA_443B_B399_C2BD384987E1__INCLUDED_)
#define AFX_MINUTESTOTIME_H__9475CA87_AABA_443B_B399_C2BD384987E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CMinutesToTime   : public CInnerFunction
{
public:
	CMinutesToTime();
	virtual ~CMinutesToTime();
	CString GetName(long lType);
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType,  const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_MINUTESTOTIME_H__9475CA87_AABA_443B_B399_C2BD384987E1__INCLUDED_)
