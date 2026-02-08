/**********************************************************
Ultimate Grid for MFC  - Radio Button Cell Type Add-on


CellTypeEx:
	none

Notifications (OnCellTypeNotify)
	none

***********************************************************/

#define UGCT_RADIOBUTTON_SELECTED 100


class CUGRadioType: public CUGCellType{

	CUGCell m_cell;
	CPen m_darkPen;
	CPen m_lightPen;


public:

	CUGRadioType();
	~CUGRadioType();
	virtual BOOL OnLClicked(int col,long row,int updn,RECT *rect,POINT *point);
	virtual BOOL OnKeyDown(int col,long row,UINT *vcKey);
	virtual void OnDraw(CDC *dc,RECT *rect,int col,long row,
		CUGCell *cell,int selected,int current);

};