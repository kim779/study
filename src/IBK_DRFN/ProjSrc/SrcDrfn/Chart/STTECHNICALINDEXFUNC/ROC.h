// ROC.h: interface for the CROC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROC_H__14E5B856_F554_448E_8C8D_33FEEBBD2D18__INCLUDED_)
#define AFX_ROC_H__14E5B856_F554_448E_8C8D_33FEEBBD2D18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CROC  : public CInnerFunction
{
public:
	CROC();
	virtual ~CROC();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_ROC_H__14E5B856_F554_448E_8C8D_33FEEBBD2D18__INCLUDED_)
