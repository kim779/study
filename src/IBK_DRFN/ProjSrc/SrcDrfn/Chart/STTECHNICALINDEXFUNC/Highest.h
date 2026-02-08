// Highest.h: interface for the CHighest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HIGHEST_H__E68C8ECC_E774_4FAE_888C_B816BB419204__INCLUDED_)
#define AFX_HIGHEST_H__E68C8ECC_E774_4FAE_888C_B816BB419204__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CHighest    : public CInnerFunction
{
public:
	CHighest();
	virtual ~CHighest();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;


};

#endif // !defined(AFX_HIGHEST_H__E68C8ECC_E774_4FAE_888C_B816BB419204__INCLUDED_)
