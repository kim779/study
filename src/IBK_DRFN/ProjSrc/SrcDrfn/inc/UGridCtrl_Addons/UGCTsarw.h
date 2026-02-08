/**********************************************************
Ultimate Grid for MFC  - Sort Arrow Cell Type Add-on


CellTypeEx:
	none

Notifications (OnCellTypeNotify)
	UGCT_SORTARROWUP - sent when the up arrow is pressed
	UGCT_SORTARROWSOWN - sent when the down arrow is pressed

***********************************************************/

#ifndef UGCT_SORTARROW
#define UGCT_SORTARROW

//celltype notifications
#define UGCT_SORTARROWUP	16
#define UGCT_SORTARROWDOWN	32


//class CUGSortArrowType: public CUGCellType{
class AFX_EXT_CLASS CUGSortArrowType: public CUGCellType{

	int m_ArrowWidth;

public:

	CUGSortArrowType();
	~CUGSortArrowType();
	virtual void OnDraw(CDC *dc,RECT *rect,int col,long row,CUGCell *cell,int selected,int current);
};

#endif
//
