// Min.h: interface for the CMin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIN_H__F9B673AA_3AD6_4C07_B087_5E82A199D4AE__INCLUDED_)
#define AFX_MIN_H__F9B673AA_3AD6_4C07_B087_5E82A199D4AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CMin   : public CInnerFunction
{
public:
	CMin();
	virtual ~CMin();
	CString GetName(long lType);
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType,  const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName0;//TS
	CString m_strName1;//YES

};

#endif // !defined(AFX_MIN_H__F9B673AA_3AD6_4C07_B087_5E82A199D4AE__INCLUDED_)
