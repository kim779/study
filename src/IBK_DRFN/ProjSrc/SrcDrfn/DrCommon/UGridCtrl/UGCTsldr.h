
#define UGCT_SLIDER_MOVED 100

class CUGSliderType: public CUGCellType{

	CPen	m_blackPen;
	CPen	m_grayPen;
	CPen	m_darkGrayPen;
	CPen	m_whitePen;

	BOOL	m_updateSlider;

public:

	CUGSliderType();
	~CUGSliderType();
	virtual BOOL OnLClicked(int col,long row,int updn,RECT *rect,POINT *point);
	virtual BOOL OnMouseMove(int col,long row,POINT *point,UINT flags);

	virtual void OnDraw(CDC *dc,RECT *rect,int col,long row,
		CUGCell *cell,int selected,int current);
};
