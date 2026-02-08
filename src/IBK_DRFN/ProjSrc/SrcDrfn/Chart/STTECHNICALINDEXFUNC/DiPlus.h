// DiPlus.h: interface for the CDiPlus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIPLUS_H__CDBF300C_C8E8_406C_BBE9_7D91ADDF3D26__INCLUDED_)
#define AFX_DIPLUS_H__CDBF300C_C8E8_406C_BBE9_7D91ADDF3D26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CDiPlus   : public CInnerFunction
{
public:
	CDiPlus();
	virtual ~CDiPlus();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_DIPLUS_H__CDBF300C_C8E8_406C_BBE9_7D91ADDF3D26__INCLUDED_)
