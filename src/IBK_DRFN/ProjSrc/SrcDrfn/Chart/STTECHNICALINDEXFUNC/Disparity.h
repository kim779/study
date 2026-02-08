// Disparity.h: interface for the CDisparity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPARITY_H__2E9D3FA2_A0EF_4E90_98B8_C24F5F4717E1__INCLUDED_)
#define AFX_DISPARITY_H__2E9D3FA2_A0EF_4E90_98B8_C24F5F4717E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Disparity_SMA.h"
class CDisparity   : public CDisparity_SMA
{
public:
	CDisparity();
	virtual ~CDisparity();
};

#endif // !defined(AFX_DISPARITY_H__2E9D3FA2_A0EF_4E90_98B8_C24F5F4717E1__INCLUDED_)
