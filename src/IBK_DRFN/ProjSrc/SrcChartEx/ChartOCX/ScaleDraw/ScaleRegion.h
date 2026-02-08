// ScaleRegion.h: interface for the CScaleDrawingRegion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALEREGION_H__70A989E2_BBED_11D5_9371_0050FC28B229__INCLUDED_)
#define AFX_SCALEREGION_H__70A989E2_BBED_11D5_9371_0050FC28B229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseData.h"

class CScaleDrawingRegion  
{
public:
	CScaleDrawingRegion();
	CScaleDrawingRegion(const CRect& scaleTextRegion, const CRect& scaleGridRegion);
	
	CRect GetScaleTextRegion() const;
	CRect GetScaleGridRegion() const;

private:
	CRect m_scaleTextRegion;
	CRect m_scaleGridRegion;
};

#endif // !defined(AFX_SCALEREGION_H__70A989E2_BBED_11D5_9371_0050FC28B229__INCLUDED_)
