// ITempMemMng.h: interface for the CITempMemMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMPMEMMNG_H__C8E0D09B_3FCB_49BE_9040_803716BF4966__INCLUDED_)
#define AFX_ITEMPMEMMNG_H__C8E0D09B_3FCB_49BE_9040_803716BF4966__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define CITempMemMng CITempMemMng0001
class CITempMemMng0001
{
public:
	virtual ~CITempMemMng0001() {};
	virtual double *GetDblArray(int nSize) = 0;
	virtual CITempMemMng* GetSubTemp(int nIndex) = 0;
};

#endif // !defined(AFX_ITEMPMEMMNG_H__C8E0D09B_3FCB_49BE_9040_803716BF4966__INCLUDED_)
