// LogicElement.h: interface for the CLogicElement class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGICELEMENT_H__C93B8C2F_0668_4C94_9F1D_C33652FD4088__INCLUDED_)
#define AFX_LOGICELEMENT_H__C93B8C2F_0668_4C94_9F1D_C33652FD4088__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Common_ST/IDataItem.h"
#include "LogicItem.h"

class CLogicElement  : public CLogicItem
{
public:
	CLogicElement();
	virtual ~CLogicElement();
	virtual double GetValue();
	virtual void SetIndex(long lIndex) { m_lIndex = lIndex;}

	void SetItem(CIDataItem* pIDataItem) { m_pIDataItem = pIDataItem;}
private:
	CIDataItem* m_pIDataItem;
	long m_lIndex;
};

#endif // !defined(AFX_LOGICELEMENT_H__C93B8C2F_0668_4C94_9F1D_C33652FD4088__INCLUDED_)
