/*************************************************************************
				Class Declaration : CUGEllipsisType
**************************************************************************
	Source file : UGCTelps.cpp
	Header file : UGCTelps.h
	Copyright ? Dundas Software Ltd. 1994 - 2002, All Rights Reserved

	Purpose
		Draws the cells value as text. Plus adds a ellipsis button on the
		right hand side of the cell. This button when clicked will fire a 
		notifiction (UGCT_ELLIPSISBUTTONCLICK).

	Details
		Cell type extended style
			UGCT_ELLIPSISHIDEBUTTON - when set the ellipsis button will not
				be shown unless the cell has focus

		Notifications (OnCellTypeNotify)
			UGCT_ELLIPSISBUTTONCLICK - sent when the ellipsis button is clicked

*************************************************************************/



//celltype notification
#define UGCT_ELLIPSISBUTTONCLICK	100

#define UGCT_ELLIPSISHIDEBUTTON		512

//CUGEllipsisType declaration
class UG_CLASS_DECL CUGEllipsisType: public CUGCellType
{
protected:

	int		m_btnWidth{};
	int		m_btnDown{};
	long		m_btnRow{};
	int		m_btnCol{};
	RECT		m_btnRect{};

	CPen		m_pen;
	CBrush		m_brush;

public:

	CUGEllipsisType();
	~CUGEllipsisType();
	
	//overloaded CUGCellType functions
	virtual LPCTSTR GetName();
	virtual LPCUGID GetUGID();
	virtual int GetEditArea(RECT *rect);
	virtual BOOL OnLClicked(int col,long row,int updn,RECT *rect,POINT *point);
	virtual BOOL OnDClicked(int col,long row,RECT *rect,POINT *point);
	virtual BOOL OnMouseMove(int col,long row,POINT *point,UINT flags);
	virtual void OnDraw(CDC *dc,RECT *rect,int col,long row,CUGCell *cell,int selected,int current);
	virtual void GetBestSize(CDC *dc,CSize *size,CUGCell *cell);
};

