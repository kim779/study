// AccDist.h: interface for the CAccDist class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCDIST_H__27ED2F3B_E44A_44DE_ABF6_1564310A1261__INCLUDED_)
#define AFX_ACCDIST_H__27ED2F3B_E44A_44DE_ABF6_1564310A1261__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CAccDist   : public CInnerFunction
{
public:
	CAccDist();
	virtual ~CAccDist();
	CString GetName(long lType);
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_ACCDIST_H__27ED2F3B_E44A_44DE_ABF6_1564310A1261__INCLUDED_)
