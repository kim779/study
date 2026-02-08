
#ifndef UGCT_ADVANCEDPROGRESS
#define UGCT_ADVANCEDPROGRESS

#define UGCT_PROGRESS_NORMAL	0
#define UGCT_PROGRESS_ALERT		1
#define UGCT_PROGRESS_RAINBOW	255

// defines for starting postion
#define UGCT_PROGRESS_TOP		1		// paint from top to bottom
#define UGCT_PROGRESS_LEFT		2		// paint from left to right (default)
#define UGCT_PROGRESS_RIGHT		3		// paint from right to left
#define UGCT_PROGRESS_BOTTOM	4		// paint from bottom to top

class CUGAdvProgressType: public CUGCellType{

	int m_lbuttondown;
	int m_canAdjust;
	int m_row;
	int m_col;
	RECT m_rect;
	int m_numberSections;
	int *m_sectionPercent;
	COLORREF *m_sectionColor;
	int m_gradlinePercent;
	int m_direction;
	int m_margin;
	

public:

	CUGAdvProgressType();
	CUGAdvProgressType(int adjust, int gridlines, int type);
	~CUGAdvProgressType();
	int SetCanAdjust(int adjust);
	int CalcPointFromValue(int percent, POINT *point);
	int SetNumberSections(int sections);
	int SetSectionPercent(int section, int percent);
	int SetSectionColor(int section, COLORREF color);
	int SetGraduationLinePercent(int percent);
	int SetDirection(int direction);
	int SetMargin(int margin);
	

	virtual void OnDraw(CDC *dc,RECT *rect,int col,long row,
		CUGCell *cell,int selected,int current);
	virtual BOOL OnLClicked(int col,long row,int updn,RECT *rect,POINT *point);
	virtual BOOL OnMouseMove(int col,long row,POINT *point, UINT flags);
	virtual BOOL OnDClicked(int col,long row,RECT *rect,POINT *point);
	


};

#endif
//
