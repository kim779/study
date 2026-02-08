
#include <afxtempl.h>

#pragma once

class CItemData: public CObject
{
	public:
	CString code;
	CString name;
	CString date;
	CItemData();
	CItemData(const CItemData& ci);
	const CItemData& operator=(const CItemData& stringSrc);
	virtual ~CItemData();
};

