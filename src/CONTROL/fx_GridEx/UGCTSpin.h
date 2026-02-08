/*************************************************************************
				Class Declaration : CUGSpinButtonType
**************************************************************************
	Source file : UGCTSpin.cpp
	Header file : UGCTSpin.h
	Copyright ? Dundas Software Ltd. 1994 - 2002, All Rights Reserved

  Details
		Cell type extended style
			UGCT_SPINBUTTONHIDEBUTTON - when set the spin button will not
				be shown unless the cell has focus

		Notifications (OnCellTypeNotify)
			UGCT_SPINBUTTONUP - sent when the up arrow is pressed
			UGCT_SPINBUTTONDOWN - sent when the down arrow is pressed

*************************************************************************/

#pragma once

//CUGSpinButtonType declaration
class UG_CLASS_DECL CUGSpinButtonType: public CUGCellType
{
protected:

	CPen	pen;
	CBrush	brush;

	int		btnBottomDown;
	int		btnTopDown;
	int		btnTopBottom;	//1-top button in progress 2-bottom in progress

	int		btnWidth;
	RECT	btnRect;
	int		btnCol;
	long	btnRow;

public:

	CUGSpinButtonType();
	~CUGSpinButtonType();

	//overloaded CUGCellType functions
	virtual LPCTSTR GetName();
	virtual LPCUGID GetUGID();
	virtual int GetEditArea(RECT *rect);
	virtual BOOL OnDClicked(int col,long row,RECT *rect,POINT *point);
	virtual BOOL OnLClicked(int col,long row,int updn,RECT *rect,POINT *point);
	virtual BOOL OnMouseMove(int col,long row,POINT *point,UINT flags);
	virtual void OnDraw(CDC *dc,RECT *rect,int col,long row,CUGCell *cell,int selected,int current);
	virtual void GetBestSize(CDC *dc,CSize *size,CUGCell *cell);
};

