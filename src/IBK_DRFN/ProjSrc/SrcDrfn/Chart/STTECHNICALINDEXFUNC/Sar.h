// Sar.h: interface for the CSar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAR_H__AB15BA3E_5EAE_417C_8B9B_32506C0EE3B4__INCLUDED_)
#define AFX_SAR_H__AB15BA3E_5EAE_417C_8B9B_32506C0EE3B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CSar   : public CInnerFunction
{
public:
	CSar();
	virtual ~CSar();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_SAR_H__AB15BA3E_5EAE_417C_8B9B_32506C0EE3B4__INCLUDED_)
