// LRL.h: interface for the CLRL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LRL_H__A9D8DC38_8629_4F0B_8C67_56119A3800BA__INCLUDED_)
#define AFX_LRL_H__A9D8DC38_8629_4F0B_8C67_56119A3800BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CLRL  : public CInnerFunction
{
public:
	CLRL();
	virtual ~CLRL();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_LRL_H__A9D8DC38_8629_4F0B_8C67_56119A3800BA__INCLUDED_)
