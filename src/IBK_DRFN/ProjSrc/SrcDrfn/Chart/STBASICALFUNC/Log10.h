// Log10.h: interface for the CLog10 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOG10_H__67B6E7D4_A4F0_4C66_89E1_D2309CD07D84__INCLUDED_)
#define AFX_LOG10_H__67B6E7D4_A4F0_4C66_89E1_D2309CD07D84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CLog10   : public CInnerFunction
{
public:
	CLog10();
	virtual ~CLog10();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType,  const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_LOG10_H__67B6E7D4_A4F0_4C66_89E1_D2309CD07D84__INCLUDED_)
