// ElderRayBull.h: interface for the CElderRayBull class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELDERRAYBULL_H__49C4DC41_604A_4C42_8E0E_5221BE970DC0__INCLUDED_)
#define AFX_ELDERRAYBULL_H__49C4DC41_604A_4C42_8E0E_5221BE970DC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_ST/InnerFunction.h"
class CElderRayBull  : public CInnerFunction
{
public:
	CElderRayBull();
	virtual ~CElderRayBull();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_ELDERRAYBULL_H__49C4DC41_604A_4C42_8E0E_5221BE970DC0__INCLUDED_)
