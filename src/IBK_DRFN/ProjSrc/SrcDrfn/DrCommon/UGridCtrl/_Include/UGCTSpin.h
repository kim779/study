//class CUGSpinButtonType: public CUGCellType{
class AFX_EXT_CLASS CUGSpinButtonType: public CUGCellType{

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
	virtual int GetEditArea(RECT *rect);
	virtual BOOL OnDClicked(int col,long row,RECT *rect,POINT *point);
	virtual BOOL OnLClicked(int col,long row,int updn,RECT *rect,POINT *point);
	virtual BOOL OnMouseMove(int col,long row,POINT *point,UINT flags);

	virtual void OnDraw(CDC *dc,RECT *rect,int col,long row,
		CUGCell *cell,int selected,int current);
};
