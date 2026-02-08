// EOM.h: interface for the CEOM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EOM_H__E0091A23_8062_43C8_8EB6_ACB177E43441__INCLUDED_)
#define AFX_EOM_H__E0091A23_8062_43C8_8EB6_ACB177E43441__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CEOM    : public CInnerFunction
{
public:
	CEOM();
	virtual ~CEOM();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_EOM_H__E0091A23_8062_43C8_8EB6_ACB177E43441__INCLUDED_)
