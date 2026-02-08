// StoFastK.h: interface for the CStoFastK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STOFASTK_H__56CF522D_F90F_4BE6_9F76_17851762E85F__INCLUDED_)
#define AFX_STOFASTK_H__56CF522D_F90F_4BE6_9F76_17851762E85F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CStoFastK  : public CInnerFunction
{
public:
	CStoFastK();
	virtual ~CStoFastK();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_STOFASTK_H__56CF522D_F90F_4BE6_9F76_17851762E85F__INCLUDED_)
