class CUGProgressType: public CUGCellType{

public:

	CUGProgressType();
	~CUGProgressType();
	virtual void OnDraw(CDC *dc,RECT *rect,int col,long row,
		CUGCell *cell,int selected,int current);

};