#include "stdafx.h"
#include "LineObjList.h"

// CLineList
void CLineList::DeleteAll()
{
	CPeriodObj* tmpLine;
	POSITION pos = GetHeadPosition();

	while(pos != NULL)
	{
		tmpLine = GetNextLine2(pos);
		delete tmpLine;
	}
}

int CLineList::GetLineCount()
{
	return GetCount();
}


// CLineList
void CLineList::DeleteAll2()
{
	CPeriodObj* tmpLine;
	POSITION pos = GetHeadPosition();

	while(pos != NULL)
	{
		tmpLine = GetNextLine2(pos);
		delete tmpLine;
	}
}

int CLineList::GetLineCount2()
{
	return GetCount();
}

