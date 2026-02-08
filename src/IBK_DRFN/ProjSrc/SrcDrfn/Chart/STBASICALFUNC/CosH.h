// CosH.h: interface for the CCosH class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COSH_H__5F895FB3_90DA_400A_A7E9_B5E48F985B6E__INCLUDED_)
#define AFX_COSH_H__5F895FB3_90DA_400A_A7E9_B5E48F985B6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CCosH   : public CInnerFunction
{
public:
	CCosH();
	virtual ~CCosH();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType,  const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_COSH_H__5F895FB3_90DA_400A_A7E9_B5E48F985B6E__INCLUDED_)
