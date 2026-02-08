/*
	Ultimate Grid Pie Chart Cell Type Add-on
	  
	Usage:
		- Add the UGCT_PIECHART cell type to the grid
		- Create one to many cells of the UGCT_PIECHARTDATA type with
		  the UGCT_PIECHARTDATA cell typeEx style
		  numbers in them
		- Add the following text to the UGCT_PIECHART type
		  "startcol\nstartrow\nendcol\nendrow\n"
		  where startcol,startrow start of the block of UGCT_PIECHARTDATA cells
		  and endrow,endcol end of the block of UGCT_PIECHARTDATA cells
		 
		- the highlight back color of the UGCT_PIECHARTDATA cells is used as the
		  pie slice color, pluse it is used to draw a rect on the right side of
		  the data


*/

#ifndef UGCT_PIECHART
#define UGCT_PIECHART

//extended types
#define UGCT_PIECHARTSEPARATE	16
#define UGCT_PIECHARTPERCENT	32
#define UGCT_PIECHARTDATA		64


class CUGPieChartType: public CUGCellType{

	int m_btnWidth;

	int m_Separation;
	BOOL m_AspectRatio;

public:

	CUGPieChartType();
	~CUGPieChartType();

	virtual void OnDraw(CDC *dc,RECT *rect,int col,long row,
		CUGCell *cell,int selected,int current);
	virtual int GetEditArea(RECT *rect);

	int DrawPieSlice(CDC *pDC,RECT *rect,int startAngle,int endAngle,int separation,COLORREF color);
	int SetSeparation(int separation);
	
	void OnDrawChartData(CDC *dc,RECT *rect,CUGCell *cell,int selected,int current);
};

#endif