// BW.h: interface for the CBW class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BW_H__FE59530F_EB8A_43F4_BB00_FF2DC10F462F__INCLUDED_)
#define AFX_BW_H__FE59530F_EB8A_43F4_BB00_FF2DC10F462F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CBW  : public CInnerFunction
{
public:
	CBW();
	virtual ~CBW();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_BW_H__FE59530F_EB8A_43F4_BB00_FF2DC10F462F__INCLUDED_)
