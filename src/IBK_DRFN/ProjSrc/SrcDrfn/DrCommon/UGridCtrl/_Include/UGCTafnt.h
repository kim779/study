class CUGAutoFontType: public CUGCellType{


	HFONT CreateAutoSizeFont(HDC hdc, RECT *rect,LPCTSTR string);

public:

	CUGAutoFontType();
	~CUGAutoFontType();
	virtual void OnDraw(CDC *dc,RECT *rect,int col,long row,
		CUGCell *cell,int selected,int current);

};