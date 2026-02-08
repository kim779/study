// SwingHigh.h: interface for the CSwingHigh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SWINGHIGH_H__A3306C49_7B11_465F_979D_CC7FC109B50A__INCLUDED_)
#define AFX_SWINGHIGH_H__A3306C49_7B11_465F_979D_CC7FC109B50A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CSwingHigh  : public CInnerFunction
{
public:
	CSwingHigh();
	virtual ~CSwingHigh();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_SWINGHIGH_H__A3306C49_7B11_465F_979D_CC7FC109B50A__INCLUDED_)
