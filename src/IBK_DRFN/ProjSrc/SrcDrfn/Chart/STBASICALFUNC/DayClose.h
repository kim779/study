// DayClose.h: interface for the CDayClose class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DAYCLOSE_H__F084144D_807E_4E5D_A9F6_79E345FBD1F9__INCLUDED_)
#define AFX_DAYCLOSE_H__F084144D_807E_4E5D_A9F6_79E345FBD1F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CDayClose  : public CInnerFunction
{
public:
	CDayClose();
	virtual ~CDayClose();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_DAYCLOSE_H__F084144D_807E_4E5D_A9F6_79E345FBD1F9__INCLUDED_)
