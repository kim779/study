// StochasticsD.h: interface for the CStochasticsD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STOCHASTICSD_H__FBD96C0A_E36E_45A0_AEAC_7B6114541458__INCLUDED_)
#define AFX_STOCHASTICSD_H__FBD96C0A_E36E_45A0_AEAC_7B6114541458__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
/* StochasticsD -> StoSlowD */
class CStochasticsD  : public CInnerFunction
{
public:
	CStochasticsD();
	virtual ~CStochasticsD();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_STOCHASTICSD_H__FBD96C0A_E36E_45A0_AEAC_7B6114541458__INCLUDED_)
