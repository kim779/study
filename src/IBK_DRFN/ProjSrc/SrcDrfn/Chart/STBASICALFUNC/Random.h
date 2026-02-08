// Random.h: interface for the CRandom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RANDOM_H__54A9E664_CA37_443D_A4C0_CF89035CECB3__INCLUDED_)
#define AFX_RANDOM_H__54A9E664_CA37_443D_A4C0_CF89035CECB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CRandom   : public CInnerFunction
{
public:
	CRandom();
	virtual ~CRandom();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType,  const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;


};

#endif // !defined(AFX_RANDOM_H__54A9E664_CA37_443D_A4C0_CF89035CECB3__INCLUDED_)
