// PVT.h: interface for the CPVT class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PVT_H__201C1E8B_58FE_4A2A_B235_F4B8BDC3D938__INCLUDED_)
#define AFX_PVT_H__201C1E8B_58FE_4A2A_B235_F4B8BDC3D938__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CPVT  : public CInnerFunction
{
public:
	CPVT();
	virtual ~CPVT();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_PVT_H__201C1E8B_58FE_4A2A_B235_F4B8BDC3D938__INCLUDED_)
