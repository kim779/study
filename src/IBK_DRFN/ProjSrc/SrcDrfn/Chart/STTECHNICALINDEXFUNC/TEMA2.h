// TEMA2.h: interface for the CTEMA2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEMA2_H__8F18814B_813D_477B_A21B_3D1100CE604B__INCLUDED_)
#define AFX_TEMA2_H__8F18814B_813D_477B_A21B_3D1100CE604B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_ST/InnerFunction.h"
class CTEMA2 : public CInnerFunction 
{
public:
	CTEMA2();
	virtual ~CTEMA2();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_TEMA2_H__8F18814B_813D_477B_A21B_3D1100CE604B__INCLUDED_)
