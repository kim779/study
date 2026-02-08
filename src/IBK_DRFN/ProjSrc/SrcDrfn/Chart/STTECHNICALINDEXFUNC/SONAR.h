// SONAR.h: interface for the CSONAR class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SONAR_H__8C7BCA09_0CB9_4773_82A1_A44A0D9C6B95__INCLUDED_)
#define AFX_SONAR_H__8C7BCA09_0CB9_4773_82A1_A44A0D9C6B95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CSONAR  : public CInnerFunction
{
public:
	CSONAR();
	virtual ~CSONAR();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;


};

#endif // !defined(AFX_SONAR_H__8C7BCA09_0CB9_4773_82A1_A44A0D9C6B95__INCLUDED_)
