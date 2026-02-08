// WMA.h: interface for the CWMA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WMA_H__4FC1AE4A_ED55_4666_9FBF_B8AC95F18CF9__INCLUDED_)
#define AFX_WMA_H__4FC1AE4A_ED55_4666_9FBF_B8AC95F18CF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CWMA  : public CInnerFunction
{
public:
	CWMA();
	virtual ~CWMA();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;


};

#endif // !defined(AFX_WMA_H__4FC1AE4A_ED55_4666_9FBF_B8AC95F18CF9__INCLUDED_)
