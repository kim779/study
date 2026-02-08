// NthLowest.h: interface for the CNthLowest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NTHLOWEST_H__5DBCFF8A_510B_4F36_AFDC_9DA07521C5EE__INCLUDED_)
#define AFX_NTHLOWEST_H__5DBCFF8A_510B_4F36_AFDC_9DA07521C5EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CNthLowest   : public CInnerFunction
{
public:
	CNthLowest();
	virtual ~CNthLowest();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_NTHLOWEST_H__5DBCFF8A_510B_4F36_AFDC_9DA07521C5EE__INCLUDED_)
