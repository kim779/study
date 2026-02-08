// WC.h: interface for the CWC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WC_H__D9B4D6C2_C028_434A_AE97_1C603CDBC97A__INCLUDED_)
#define AFX_WC_H__D9B4D6C2_C028_434A_AE97_1C603CDBC97A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CWC    : public CInnerFunction
{
public:
	CWC();
	virtual ~CWC();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_WC_H__D9B4D6C2_C028_434A_AE97_1C603CDBC97A__INCLUDED_)
