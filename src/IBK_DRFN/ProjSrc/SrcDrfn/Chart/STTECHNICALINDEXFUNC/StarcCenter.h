// StarcCenter.h: interface for the CStarcCenter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STARCCENTER_H__AC4A0117_A815_4D14_AC15_BC992D3D9EEA__INCLUDED_)
#define AFX_STARCCENTER_H__AC4A0117_A815_4D14_AC15_BC992D3D9EEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CStarcCenter  : public CInnerFunction
{
public:
	CStarcCenter();
	virtual ~CStarcCenter();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_STARCCENTER_H__AC4A0117_A815_4D14_AC15_BC992D3D9EEA__INCLUDED_)
