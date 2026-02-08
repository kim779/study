// PsyhologicalLine.h: interface for the CPsyhologicalLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PSYHOLOGICALLINE_H__DEE79441_5BAE_424F_9E7B_6B1CA291792F__INCLUDED_)
#define AFX_PSYHOLOGICALLINE_H__DEE79441_5BAE_424F_9E7B_6B1CA291792F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CPsyhologicalLine  : public CInnerFunction
{
public:
	CPsyhologicalLine();
	virtual ~CPsyhologicalLine();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_PSYHOLOGICALLINE_H__DEE79441_5BAE_424F_9E7B_6B1CA291792F__INCLUDED_)
