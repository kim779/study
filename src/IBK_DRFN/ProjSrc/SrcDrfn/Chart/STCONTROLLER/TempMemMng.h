// TempMemMng.h: interface for the CTempMemMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEMPMEMMNG_H__607E371E_21C9_487F_88B9_13FF13D2F753__INCLUDED_)
#define AFX_TEMPMEMMNG_H__607E371E_21C9_487F_88B9_13FF13D2F753__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Common_ST/ITempMemMng.h"
#define MAX_INDEX_TEMPMEM	10
class CTempMemMng  : public CITempMemMng0001
{
public:
	CTempMemMng();
	virtual ~CTempMemMng();
	virtual double *GetDblArray(int nSize);
	virtual CITempMemMng* GetSubTemp(int nIndex);
private:
	CTempMemMng *m_pTempMemMng[MAX_INDEX_TEMPMEM];
	double *m_pDbArray; 
};

#endif // !defined(AFX_TEMPMEMMNG_H__607E371E_21C9_487F_88B9_13FF13D2F753__INCLUDED_)
