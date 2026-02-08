/***************************************************
Ultimate Grid 3.0 for MFC
Dundas Software Ltd. 1994-1996

	Pie Chart CellType
****************************************************/

#include "stdafx.h"

#include "../UGCtrl.h"
#include "../_Include/UGCTpie.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/***************************************************
****************************************************/
CUGPieChartType::CUGPieChartType(){
	m_Separation = 0;
	m_AspectRatio = TRUE;
	m_btnWidth = GetSystemMetrics(SM_CXVSCROLL);

}

/***************************************************
****************************************************/
CUGPieChartType::~CUGPieChartType(){

}
/***************************************************
****************************************************/
int CUGPieChartType::SetSeparation(int separation){
	if(separation <0 || separation > 64)
		return 1;
	m_Separation = separation;
	return UG_SUCCESS;
}
/***************************************************
****************************************************/
int CUGPieChartType::GetEditArea(RECT *rect){
	rect->right -= m_btnWidth;
	return TRUE;
}

/***************************************************
****************************************************/
void CUGPieChartType::OnDraw(CDC *dc,RECT *rect,int col,long row,
							CUGCell *cell,int selected,int current){
	
	DrawBorder(dc,rect,rect,cell);

	long properties = cell->GetPropertyFlags();
	long cellTypeEx = 0;

	//get the extended style
	if(properties&UGCELL_CELLTYPEEX_SET)
		cellTypeEx = cell->GetCellTypeEx();
	
	if(cellTypeEx&UGCT_PIECHARTDATA){
		OnDrawChartData(dc,rect,cell,selected,current);
		return;
	}
	

	int separation = m_Separation;
	if(cellTypeEx&UGCT_PIECHARTSEPARATE && separation == 0)
		separation = 2;

	//check the selected and current states
	if(selected || (current && m_ctrl->m_GI->m_currentCellMode&2)){
		CBrush brush(cell->GetHBackColor());
		dc->FillRect(rect,&brush);
	}
	else{
		CBrush brush(cell->GetBackColor());
		dc->FillRect(rect,&brush);
	}

	//adjust the rect
	RECT origRect;
	CopyRect(&origRect,rect);
	rect->left +=2+separation;
	rect->top +=2+separation;
	rect->right -=2+separation;
	rect->bottom -=2+separation;

	if(m_AspectRatio == TRUE){
		int width = rect->right - rect->left;
		int height = rect->bottom - rect->top;
		if(width != height){
			if(width >height){
				int dif = (width - height)/2;
				rect->left += dif;
				rect->right -= dif;
			}
			else{
				int dif = (height - width)/2;
				rect->top += dif;
				rect->bottom -= dif;
			}
		}
	}

	//get the pie data range
	long range[4];
	CString pString = cell->GetText();
	int len = pString.GetLength();
	int spos =0;
	int index = 0;
	for(int t=0;t<len;t++){
		if(pString.GetAt(t)==_T('\n')){
			range[index] = _ttol(pString.Mid(spos,t-spos));
			index++;
			spos = t+1;
		}
	}
	if(range[0] > range[2])
		return;
	if(range[1] > range[3])
		return;
	
	//get the data values total
	CUGCell		c;	
	double		totalValue =0;
	double		*Values = new double[(range[2]-range[0]+1) * (range[3]-range[1]+1)];
	COLORREF	*Colors = new COLORREF[(range[2]-range[0]+1) * (range[3]-range[1]+1)];
	long		totalSlices =0;
	for(int x= range[0];x<=range[2];x++){
		for(int y= range[1];y<=range[3];y++){
			m_ctrl->GetCellIndirect(x,y,&c);
			Values[totalSlices] = c.GetNumber();
			totalValue +=Values[totalSlices];
			Colors[totalSlices] = c.GetHBackColor();
			totalSlices ++;
		}
	}

	//draw the slices
	double scale = 360 / totalValue;
	int startAngle =0;
	int endAngle;
	for(int t= 0;t< totalSlices ;t++){
		endAngle = startAngle + (int)(Values[t] *scale); 
		if(endAngle == startAngle)
			continue;
		if(t == totalSlices-1)
			endAngle = 360;
		DrawPieSlice(dc,rect,startAngle,endAngle,separation,Colors[t]);
		startAngle = endAngle;
	}

	//reset the rect
	CopyRect(rect,&origRect);

	delete[] Values;
	delete[] Colors;

}
/***************************************************
****************************************************/
int CUGPieChartType::DrawPieSlice(CDC *pDC,RECT *rect,int startAngle,int endAngle,int separation,COLORREF color){
	
	
	POINT start;
	POINT end;
	RECT  tRect;
	CBrush brush(color);
	CBrush * oldbrush = pDC->SelectObject(&brush);
	
	CopyRect(&tRect,rect);
		
	double sa = (6.28318 / 360 ) * (double)startAngle*-1;	//start angle
	double ea = (6.28318 / 360 ) * (double)endAngle*-1;		//end angle
	double ma = (6.28318 / 360 ) * 
		(double)(startAngle+(endAngle-startAngle)/2) *-1;//middle angle

	//separation
	if(separation >0){
		separation +=1;
		POINT sep;
		double d= cos(ma) * (double)separation;
		sep.x = (int)(cos(ma) * (double)separation);
		sep.y = (int)(sin(ma) * (double)separation);
		
		//adjust the rect
		tRect.left +=sep.x;
		tRect.right +=sep.x;
		tRect.top +=sep.y;
		tRect.bottom +=sep.y;
	}

	int width	= tRect.right - tRect.left;
	int height	= tRect.bottom - tRect.top;
	int yCenter = tRect.top + (height/2)  ;
	int xCenter = tRect.left + (width/2) ;

	start.x = (int)(cos(sa) * width) + xCenter;
	start.y = (int)(sin(sa) * height) + yCenter;

	end.x = (int)(cos(ea) * width) + xCenter;
	end.y = (int)(sin(ea) * height) + yCenter;

	pDC->Pie(&tRect,start,end);

	pDC->SelectObject(oldbrush);

	return TRUE;
}

/***************************************************
****************************************************/
void CUGPieChartType::OnDrawChartData(CDC *dc,RECT *rect,CUGCell *cell,int selected,int current){
	

	RECT origRect;
	CopyRect(&origRect,rect);

	//draw the colored rect
	int bwidth = m_btnWidth;
	if(bwidth >= (rect->right - rect->left))
		bwidth = (rect->right - rect->left);

	rect->left = rect->right - bwidth;
	int offset = (rect->bottom - rect->top - bwidth)/2 +1;
	if(offset <0)
		offset = 0;

	CBrush brush(cell->GetBackColor());
	dc->FillRect(rect,&brush);

	rect->top +=offset;
	rect->bottom -=offset;
	rect->left+=2;
	rect->right -=2;
	CBrush brush2(cell->GetHBackColor());
	dc->FillRect(rect,&brush2);

	//draw the text in using the default drawing routine
	CopyRect(rect,&origRect);
	rect->right -= bwidth;
	DrawText(dc,rect,0,0,0,cell,selected,current);
}
