// Sqrt.h: interface for the CSqrt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SQRT_H__914A5E20_F14F_498A_BFE1_660EB7ADA62B__INCLUDED_)
#define AFX_SQRT_H__914A5E20_F14F_498A_BFE1_660EB7ADA62B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CSqrt   : public CInnerFunction
{
public:
	CSqrt();
	virtual ~CSqrt();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController ,  long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_SQRT_H__914A5E20_F14F_498A_BFE1_660EB7ADA62B__INCLUDED_)
