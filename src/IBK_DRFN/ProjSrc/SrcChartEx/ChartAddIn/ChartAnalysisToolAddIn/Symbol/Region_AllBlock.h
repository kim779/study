// Region_AllBlock.h: interface for the CRegion_AllBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGION_ALLBLOCK_H__A8ED42F4_7D4B_4080_BB36_56292E9EC8C5__INCLUDED_)
#define AFX_REGION_ALLBLOCK_H__A8ED42F4_7D4B_4080_BB36_56292E9EC8C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "../Include_Analysis/Include_AddIn.h"				// for Ixxxxxx.h

#include "BlockBaseData.h"

class CCoordinateCalculatorData;
class CRegion_Column;
class CRegion_AllBlock  
{
public:
	CRegion_AllBlock();
	virtual ~CRegion_AllBlock();

	void SetDrawingRegions(IChartManager* pMainBlock);
	void SetAllBlockRegion(IChartManager* pMainBlock);
	void Initialize();

	//Block영역을 저장하고 얻는다.
	void SetBlockRegion(const int nColumnIndex, const int nRowIndex, const CRect& blockregion);
	CRect GetBlockRegion(const int nColumnIndex, const int nRowIndex) const;

	//Block영역을 관리해야하는 Column리스트를 만든다.
	void MakeColumnList(const int nColumnCount, const int nRowCount);

	bool InsertBlockIndex(const CBlockIndex &blockIndex);
	void DeleteColumnBlockList();

	//Block의 현 BlockIndex에 해당하는 Data의 정보를 얻는다.
//	void GetCoordinateCalculatorData(CBlockIndex blockIndex, CCoordinateCalculatorData& coordinateData);

private:
	//Block영역을 관리하는 Column리스트.
	CList<CRegion_Column*, CRegion_Column* > m_ColumnBlockRegionList; 

};

#endif // !defined(AFX_REGION_ALLBLOCK_H__A8ED42F4_7D4B_4080_BB36_56292E9EC8C5__INCLUDED_)
