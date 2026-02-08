// WILLA.h: interface for the CWILLA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WILLA_H__AC8EF07D_B2FA_4BA6_A695_C6D40222D562__INCLUDED_)
#define AFX_WILLA_H__AC8EF07D_B2FA_4BA6_A695_C6D40222D562__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CWILLA  : public CInnerFunction
{
public:
	CWILLA();
	virtual ~CWILLA();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_WILLA_H__AC8EF07D_B2FA_4BA6_A695_C6D40222D562__INCLUDED_)
