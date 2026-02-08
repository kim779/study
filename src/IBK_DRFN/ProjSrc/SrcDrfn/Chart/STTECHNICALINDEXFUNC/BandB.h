// BandB.h: interface for the CBandB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BANDB_H__1D313483_2973_4A81_B389_AA8D22389A59__INCLUDED_)
#define AFX_BANDB_H__1D313483_2973_4A81_B389_AA8D22389A59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CBandB  : public CInnerFunction
{
public:
	CBandB();
	virtual ~CBandB();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_BANDB_H__1D313483_2973_4A81_B389_AA8D22389A59__INCLUDED_)
