#pragma once


#define UGCT_CODECOMBOUSELABEL BIT1

//CUGCodeComboType Declaration
class UG_CLASS_DECL CUGCodeComboType: public CUGCellType
{
public:

	CUGCodeComboType();
	~CUGCodeComboType();

	//overloaded CUGCellType functions
	virtual LPCTSTR GetName();
	virtual LPCUGID GetUGID();
	virtual void OnDraw(CDC *dc,RECT *rect,int col,long row,CUGCell *cell,int selected,int current);
	virtual void GetBestSize(CDC *dc,CSize *size,CUGCell *cell);
};
