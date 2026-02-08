// DEMA2.h: interface for the CDEMA2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMA2_H__F19752F6_A126_403C_9A80_077BA8A5D55C__INCLUDED_)
#define AFX_DEMA2_H__F19752F6_A126_403C_9A80_077BA8A5D55C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_ST/InnerFunction.h"
class CDEMA2 : public CInnerFunction 
{
public:
	CDEMA2();
	virtual ~CDEMA2();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_DEMA2_H__F19752F6_A126_403C_9A80_077BA8A5D55C__INCLUDED_)
