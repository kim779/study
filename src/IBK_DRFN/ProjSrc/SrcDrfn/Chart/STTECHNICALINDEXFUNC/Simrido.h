// Simrido.h: interface for the CSimrido class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMRIDO_H__0FA85F42_9DD4_4134_91B7_6CA454A04DAF__INCLUDED_)
#define AFX_SIMRIDO_H__0FA85F42_9DD4_4134_91B7_6CA454A04DAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CSimrido  : public CInnerFunction
{
public:
	CSimrido();
	virtual ~CSimrido();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_SIMRIDO_H__0FA85F42_9DD4_4134_91B7_6CA454A04DAF__INCLUDED_)
