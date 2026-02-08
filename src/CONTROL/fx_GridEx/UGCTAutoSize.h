/*************************************************************************
				Class Declaration : CUGCTAutoSize
**************************************************************************
	Source file : UGCTAutoSize.cpp
	Header file : UGCTAutoSize.h
	Copyright ? Dundas Software Ltd. 1994 - 2002, All Rights Reserved

	Purpose:
		The CUGCTAutoSize celltype extends the standard celltype providing
		funcitonality to size its cell to make sure that cell's data is
		visible in full.  It will not make columns or rows smaller,
		but it will enlarge them whenever needed.
	
	Details:
		This celltype does not send any notifications to the CUGCtrl derived
		classes.
*************************************************************************/
#pragma once


class UG_CLASS_DECL CUGCTAutoSize: public CUGCellType
{
protected:

public:
	CUGCTAutoSize();
	~CUGCTAutoSize();

	virtual void OnDraw(CDC *dc,RECT *rect,int col,long row,CUGCell *cell,int selected,int current);
	virtual void OnChangedCellWidth(int col, long row, int* width);
	virtual void OnChangedCellHeight(int col, long row, int* height);
};

