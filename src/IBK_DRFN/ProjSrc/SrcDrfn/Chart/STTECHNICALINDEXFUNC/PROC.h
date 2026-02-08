// PROC.h: interface for the CPROC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROC_H__06781AC9_C5FE_478E_AAAD_E71B03090690__INCLUDED_)
#define AFX_PROC_H__06781AC9_C5FE_478E_AAAD_E71B03090690__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CPROC   : public CInnerFunction
{
public:
	CPROC();
	virtual ~CPROC();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTControllerD *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;


};

#endif // !defined(AFX_PROC_H__06781AC9_C5FE_478E_AAAD_E71B03090690__INCLUDED_)
