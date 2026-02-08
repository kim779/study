// Momentum.h: interface for the CMomentum class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOMENTUM_H__0615B6FA_4E41_4314_A98A_823717BBC948__INCLUDED_)
#define AFX_MOMENTUM_H__0615B6FA_4E41_4314_A98A_823717BBC948__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CMomentum  : public CInnerFunction
{
public:
	CMomentum();
	virtual ~CMomentum();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

	
};

#endif // !defined(AFX_MOMENTUM_H__0615B6FA_4E41_4314_A98A_823717BBC948__INCLUDED_)
