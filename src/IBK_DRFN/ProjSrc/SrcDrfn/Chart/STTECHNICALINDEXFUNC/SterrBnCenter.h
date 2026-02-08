// SterrBnCenter.h: interface for the CSterrBnCenter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STERRBNCENTER_H__4CBD1DC0_57F5_4576_9FB5_D42A41F2D23F__INCLUDED_)
#define AFX_STERRBNCENTER_H__4CBD1DC0_57F5_4576_9FB5_D42A41F2D23F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CSterrBnCenter  : public CInnerFunction
{
public:
	CSterrBnCenter();
	virtual ~CSterrBnCenter();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_STERRBNCENTER_H__4CBD1DC0_57F5_4576_9FB5_D42A41F2D23F__INCLUDED_)
