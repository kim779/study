// SterrBnUpper.h: interface for the CSterrBnUpper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STERRBNUPPER_H__3F72BDEC_A6BF_459D_99B1_8216414B42C9__INCLUDED_)
#define AFX_STERRBNUPPER_H__3F72BDEC_A6BF_459D_99B1_8216414B42C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CSterrBnUpper  : public CInnerFunction
{
public:
	CSterrBnUpper();
	virtual ~CSterrBnUpper();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_STERRBNUPPER_H__3F72BDEC_A6BF_459D_99B1_8216414B42C9__INCLUDED_)
