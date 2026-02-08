// MA.h: interface for the CMA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MA_H__85E365F1_D4F3_4DFD_938F_2FE802E6BE3C__INCLUDED_)
#define AFX_MA_H__85E365F1_D4F3_4DFD_938F_2FE802E6BE3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CMA  : public CInnerFunction
{
public:
	CMA();
	virtual ~CMA();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_MA_H__85E365F1_D4F3_4DFD_938F_2FE802E6BE3C__INCLUDED_)
