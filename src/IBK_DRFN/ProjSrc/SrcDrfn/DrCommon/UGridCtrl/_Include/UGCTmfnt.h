/**********************************************************
Ultimate Grid for MFC  - MultiFont Cell Type Add-on


CellTypeEx:
	none

***********************************************************/


class CUGMultiFontType: public CUGCellType{

public:

	CFont *m_font;

	CUGMultiFontType();
	~CUGMultiFontType();

	virtual void OnDraw(CDC *dc,RECT *rect,int col,long row,
		CUGCell *cell,int selected,int current);

	int ParseCommand(CDC *dc,CString *command);
	
};


