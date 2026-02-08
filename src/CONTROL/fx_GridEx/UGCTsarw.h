/*************************************************************************
				Class Declaration : CUGSortArrowType
**************************************************************************
	Source file : UGCTsarw.cpp
	Header file : UGCTsarw.h
	Copyright ? Dundas Software Ltd. 1994 - 2002, All Rights Reserved


	Purpose
		This cell type is generally used to indicate the column that
		the grid is sorted by. 
	Details
		The sort arrow is drawn on the right hand side of the cell with
		text on the left. The up or down direction is determined with
		the extended cell type property:
			UGCT_SORTARROWUP or UGCT_SORTARROWDOWN (default)
*************************************************************************/


#pragma once


//celltype notifications
#define UGCT_SORTARROWUP	16
#define UGCT_SORTARROWDOWN	32

//CUGSortArrowType declaration
class UG_CLASS_DECL CUGSortArrowType: public CUGCellType
{
	int m_ArrowWidth;

public:

	CUGSortArrowType();
	~CUGSortArrowType();

	//overloaded CUGCellType functions
	virtual LPCTSTR GetName();
	virtual LPCUGID GetUGID();
	virtual void OnDraw(CDC *dc,RECT *rect,int col,long row,CUGCell *cell,int selected,int current);
	virtual void GetBestSize(CDC *dc,CSize *size,CUGCell *cell);
};

