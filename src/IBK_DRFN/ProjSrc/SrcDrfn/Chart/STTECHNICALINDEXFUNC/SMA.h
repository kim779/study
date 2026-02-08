// SMA.h: interface for the CSMA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMA_H__ABFBA950_AE31_45E1_93F0_3924CF5CB906__INCLUDED_)
#define AFX_SMA_H__ABFBA950_AE31_45E1_93F0_3924CF5CB906__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CSMA    : public CInnerFunction
{
public:
	CSMA();
	virtual ~CSMA();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_SMA_H__ABFBA950_AE31_45E1_93F0_3924CF5CB906__INCLUDED_)
