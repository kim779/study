// StandardError.h: interface for the CStandardError class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STANDARDERROR_H__94A0E0A5_DF8D_4056_BF0E_2A2432382A0E__INCLUDED_)
#define AFX_STANDARDERROR_H__94A0E0A5_DF8D_4056_BF0E_2A2432382A0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CStandardError  : public CInnerFunction
{
public:
	CStandardError();
	virtual ~CStandardError();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_STANDARDERROR_H__94A0E0A5_DF8D_4056_BF0E_2A2432382A0E__INCLUDED_)
