// MAO.h: interface for the CMAO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAO_H__B738DB6B_605F_430f_8BA9_1D794748CC7F__INCLUDED_)
#define AFX_MAO_H__B738DB6B_605F_430f_8BA9_1D794748CC7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CMAO : public CInnerFunction 
{
public:
	CMAO();
	virtual ~CMAO();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_MAO_H__B738DB6B_605F_430f_8BA9_1D794748CC7F__INCLUDED_)
