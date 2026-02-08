// Accum.h: interface for the CAccum class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCUM_H__C2B41F0E_9162_4C55_AA6E_4C3124DAEC16__INCLUDED_)
#define AFX_ACCUM_H__C2B41F0E_9162_4C55_AA6E_4C3124DAEC16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CAccum  : public CInnerFunction
{
public:
	CAccum();
	virtual ~CAccum();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_ACCUM_H__C2B41F0E_9162_4C55_AA6E_4C3124DAEC16__INCLUDED_)
