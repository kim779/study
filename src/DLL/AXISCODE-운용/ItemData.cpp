#include "stdafx.h"
#include "ItemData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemData::CItemData()
{
}

CItemData::CItemData(const CItemData& ci)
{
	code = ci.code;
	name = ci.name;
	date = ci.date;
}

CItemData::~CItemData()
{
}

const CItemData& CItemData::operator=(const CItemData &ci)
{
	code = ci.code;
	name = ci.name;
	date = ci.date;

	return *this;
}