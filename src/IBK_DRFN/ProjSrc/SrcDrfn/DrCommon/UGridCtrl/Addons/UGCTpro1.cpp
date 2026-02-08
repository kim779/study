#include "stdafx.h"
#include "math.h"

#include "../UGCtrl.h"
#include "../_Include/UGCTpro1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***************************************************
****************************************************/
CUGAdvProgressType::CUGAdvProgressType(){

	SetTextEditMode(FALSE);
	m_lbuttondown  =	FALSE;
	m_numberSections =	0;
	m_gradlinePercent = 5;
	m_sectionPercent =	NULL;
	m_sectionColor =	NULL;
	m_canAdjust =		FALSE;
	m_direction =		UGCT_PROGRESS_LEFT;
	m_margin =			4;

	SetNumberSections(3);
	SetSectionPercent(0,50);
	SetSectionColor(0,RGB(0,128,0));
	SetSectionPercent(1,90);
	SetSectionColor(1,RGB(200,200,0));
	SetSectionPercent(2,100);
	SetSectionColor(2,RGB(255,0,0));
}

/***************************************************
****************************************************/
CUGAdvProgressType::CUGAdvProgressType(int adjust, int gridlines, int type){

	// call default constructor
	SetTextEditMode(FALSE);
	m_lbuttondown  =	FALSE;
	m_numberSections =	0;
	m_gradlinePercent = 5;
	m_sectionPercent =	NULL;
	m_sectionColor =	NULL;
	m_canAdjust =		FALSE;
	m_direction =		UGCT_PROGRESS_LEFT;
	m_margin =			4;

	int i;
	
	if (adjust)
		m_canAdjust = TRUE;

	SetGraduationLinePercent(gridlines);

	if (type == UGCT_PROGRESS_ALERT){
			SetNumberSections(3);
			SetSectionPercent(0,50);
			SetSectionColor(0,RGB(0,128,0));
			SetSectionPercent(1,90);
			SetSectionColor(1,RGB(200,200,0));
			SetSectionPercent(2,100);
			SetSectionColor(2,RGB(255,0,0));
	}

	if (type == UGCT_PROGRESS_NORMAL){
		SetNumberSections(1);
		SetSectionPercent(0,100);
		SetSectionColor(0,RGB(0,128,0));
	}

	if (type == UGCT_PROGRESS_RAINBOW){
		SetNumberSections(100);
		for (i=0;i<100;i++){
			SetSectionPercent(i,i+1);
			SetSectionColor(99-i,RGB(255-i, 200-i*2, 20+i));
		}
	}

}

/***************************************************
****************************************************/
CUGAdvProgressType::~CUGAdvProgressType(){
	if (m_sectionPercent != NULL){
		delete[] m_sectionPercent;
		delete[] m_sectionColor;
	}
}
/***************************************************
****************************************************/
void CUGAdvProgressType::OnDraw(CDC *dc,RECT *rect,int col,long row,
								CUGCell *cell, int selected, int current){


	long percent = 0; 
	int i;
	int lineHeight;
	int  fillpos;
	int  anchor;
	int  moving;
	RECT fillrect, marginrect;
	int left,top;
 	CString string;
	SIZE size;
	short alignment = 0;
	long cellTypeEx = 0;

	long properties = cell->GetPropertyFlags();

	if(properties&UGCELL_TEXT_SET)
		percent = _ttol(cell->GetText());
	if(properties&UGCELL_ALIGNMENT_SET)
		alignment = cell->GetAlignment();
	
	if(properties&UGCELL_CELLTYPEEX_SET)
		cellTypeEx = cell->GetCellTypeEx();

	
	DrawBorder(dc,rect,rect,cell);

	
	//get the percent
	if(percent >100)
		percent = 100;
	else if(percent <0)
		percent = 0;

	if(cellTypeEx&1){
 		string = cell->GetLabelText();
	} 
	else {
		string.Format(_T("%% %ld"),percent);
	}

	//text alignment
	if(alignment & UG_ALIGNCENTER){
		GetTextExtentPoint(dc->m_hDC,string,string.GetLength(),&size);
		left = rect->left + (rect->right - rect->left - size.cx) /2;
	}
	else if(alignment & UG_ALIGNRIGHT){
		GetTextExtentPoint(dc->m_hDC,string,string.GetLength(),&size);
		left = rect->right - size.cx - 3;
	}
	else{
		left = rect->left+3;
	}	
	if(alignment & UG_ALIGNVCENTER){
		GetTextExtentPoint(dc->m_hDC,string,string.GetLength(),&size);
		top = rect->top + (rect->bottom - rect->top - size.cy) /2;
	}
	else if(alignment & UG_ALIGNBOTTOM){
		GetTextExtentPoint(dc->m_hDC,string,string.GetLength(),&size);
		top = rect->bottom - size.cy -1;
	}
	else{
		top = rect->top+1;
	}

	//get the first rect
	CopyRect(&fillrect,rect);
	
	// get anchor positions for future calculations

	switch(m_direction){
		case UGCT_PROGRESS_LEFT:
			anchor = fillrect.left;
			moving = fillrect.right;
			fillrect.top += m_margin;
			fillrect.bottom -= m_margin;
			break;
		case UGCT_PROGRESS_TOP:
			anchor = fillrect.top;
			moving = fillrect.bottom;
			fillrect.left += m_margin;
			fillrect.right -= m_margin;
			break;
		case UGCT_PROGRESS_RIGHT:
			anchor = fillrect.right;
			moving = fillrect.left;
			fillrect.top += m_margin;
			fillrect.bottom -= m_margin;
			break;
		case UGCT_PROGRESS_BOTTOM:
			anchor = fillrect.bottom;
			moving = fillrect.top;
			fillrect.left += m_margin;
			fillrect.right -= m_margin;
			break;
	}

	for (i=0;i<m_numberSections;i++){

		fillpos = anchor + ((long)(moving - anchor) * min(m_sectionPercent[i],percent))/100;

		switch(m_direction){
			case UGCT_PROGRESS_LEFT:
				fillrect.right = fillpos;
				break;
			case UGCT_PROGRESS_TOP:
				fillrect.bottom = fillpos;
				break;
			case UGCT_PROGRESS_RIGHT:
				fillrect.left = fillpos;
				break;
			case UGCT_PROGRESS_BOTTOM:
				fillrect.top = fillpos;
				break;
		}

		dc->SetBkColor(m_sectionColor[i]);
		dc->SetTextColor(cell->GetHTextColor());
		dc->ExtTextOut(left,top,ETO_OPAQUE|ETO_CLIPPED,&fillrect,string,NULL);

		switch(m_direction){
			case UGCT_PROGRESS_LEFT:
				fillrect.left = fillrect.right;
				break;
			case UGCT_PROGRESS_TOP:
				fillrect.top = fillrect.bottom;
				break;
			case UGCT_PROGRESS_RIGHT:
				fillrect.right = fillrect.left;
				break;
			case UGCT_PROGRESS_BOTTOM:
				fillrect.bottom = fillrect.top;
				break;
		}
	}
			
	//draw the second half
	if(percent <100){

		switch(m_direction){
			case UGCT_PROGRESS_LEFT:
				fillrect.right = rect->right;
				fillrect.left = fillpos;
				break;
			case UGCT_PROGRESS_TOP:
				fillrect.bottom = rect->bottom;
				fillrect.top = fillpos;
				break;
			case UGCT_PROGRESS_RIGHT:
				fillrect.left = rect->left;
				fillrect.right = fillpos;
				break;
			case UGCT_PROGRESS_BOTTOM:
				fillrect.top = rect->top;
				fillrect.bottom = fillpos;
				break;
		}

			
		dc->SetBkColor(cell->GetBackColor());
		dc->SetTextColor(cell->GetTextColor());
		dc->ExtTextOut(left,top,ETO_OPAQUE|ETO_CLIPPED,&fillrect,string,NULL);
	}
	if(m_margin > 0){
		dc->SetBkColor(cell->GetBackColor());
		dc->SetTextColor(cell->GetTextColor());


		switch(m_direction){
			case UGCT_PROGRESS_LEFT:
			case UGCT_PROGRESS_RIGHT:
				CopyRect(&marginrect,rect);
				marginrect.bottom = marginrect.top+m_margin;
				dc->ExtTextOut(left,top,ETO_OPAQUE|ETO_CLIPPED,&marginrect,"",NULL);

				CopyRect(&marginrect,rect);
				marginrect.top = marginrect.bottom-m_margin;
				dc->ExtTextOut(left,top,ETO_OPAQUE|ETO_CLIPPED,&marginrect,"",NULL);
				break;
			case UGCT_PROGRESS_TOP:
			case UGCT_PROGRESS_BOTTOM:
				CopyRect(&marginrect,rect);
				marginrect.right = marginrect.left+m_margin;
				dc->ExtTextOut(left,top,ETO_OPAQUE|ETO_CLIPPED,&marginrect,"",NULL);

				CopyRect(&marginrect,rect);
				marginrect.left = marginrect.right-m_margin;
				dc->ExtTextOut(left,top,ETO_OPAQUE|ETO_CLIPPED,&marginrect,"",NULL);
				break;
		}

	}

	if (m_gradlinePercent>0 && (m_direction == UGCT_PROGRESS_LEFT || m_direction == UGCT_PROGRESS_RIGHT)){
		for (i=0; i<100; i+=m_gradlinePercent){
			if (m_gradlinePercent == 5){
				fillpos = anchor + ((long)(moving - anchor)*i/100);
				dc->MoveTo(fillpos, fillrect.bottom);
				
				if ((i % 25) ==0){
					lineHeight = (fillrect.bottom-fillrect.top)/3;
					dc->MoveTo(fillpos, fillrect.bottom);
					dc->LineTo(fillpos, fillrect.bottom -lineHeight);
					dc->MoveTo(fillpos+1, fillrect.bottom);
					dc->LineTo(fillpos+1, fillrect.bottom -lineHeight);
		
				}
				else if ((i % 50) ==0){
					lineHeight = (fillrect.bottom-fillrect.top)/2;
					dc->MoveTo(fillpos-1, fillrect.bottom);
					dc->LineTo(fillpos-1, fillrect.bottom -lineHeight);
					dc->MoveTo(fillpos, fillrect.bottom);
					dc->LineTo(fillpos, fillrect.bottom -lineHeight);
					dc->MoveTo(fillpos+1, fillrect.bottom);
					dc->LineTo(fillpos+1, fillrect.bottom -lineHeight);
				}
				else{
					lineHeight = (fillrect.bottom-fillrect.top)/4;
					dc->MoveTo(fillpos, fillrect.bottom);
					dc->LineTo(fillpos, fillrect.bottom -lineHeight);
				}
			} 
			else {
				lineHeight = (fillrect.bottom-fillrect.top)/4;
				dc->MoveTo(fillpos, fillrect.bottom);
				dc->LineTo(fillpos, fillrect.bottom -lineHeight);
			}
		}
	}
}

/***************************************************
****************************************************/
BOOL CUGAdvProgressType::OnMouseMove(int col,long row,POINT *point,UINT flags){

	if((flags&MK_LBUTTON) == FALSE)
		return FALSE;

	if (!m_canAdjust)
		return FALSE;

	int ref, span;
	float ftemp;
	CUGCell mycell;

	if (point->x>m_rect.right) 
		point->x = m_rect.right;
	if (point->x<m_rect.left)  
		point->x = m_rect.left;
	if (point->y>m_rect.bottom) 
		point->y = m_rect.bottom;
	if (point->y<m_rect.top) 
		point->y = m_rect.top;

	m_ctrl->GetCell(m_col, m_row, &mycell);


	switch(m_direction){

		case UGCT_PROGRESS_LEFT:
			ref = point->x - m_rect.left;
			span = m_rect.right - m_rect.left; 
			break;
		case UGCT_PROGRESS_TOP:
			ref = point->y - m_rect.top;
			span = m_rect.bottom - m_rect.top; 
			break;
		case UGCT_PROGRESS_RIGHT:
			ref = point->x - m_rect.right;
			span = m_rect.left - m_rect.right; 
			break;
		case UGCT_PROGRESS_BOTTOM:
			ref = point->y - m_rect.bottom;
			span = m_rect.top - m_rect.bottom; 
			break;
	}
	
	ftemp = ((float)ref / (float)span) * 100;
	

	if((flags&MK_LBUTTON)){
		// we are resizing the progress bar
		// find the new mouse position, and change the percent value in the
		// cell to reflect the mouse position
	
		mycell.SetNumber(ftemp);
		m_ctrl->SetCell(m_col, m_row, &mycell);
		m_ctrl->RedrawCell(m_col, m_row);

	} else {
		// we just moving over the resize area, not changing the progress bar
		// if the mouse pointer moves within 5 pixels of the current progress bar setting
		// then we should change the mouse pointer to double resize arrows.

		if (abs((int)ftemp - (int)mycell.GetNumber()) < 5){
			if (m_direction == UGCT_PROGRESS_TOP || m_direction == UGCT_PROGRESS_BOTTOM){
				// change the mouse pointer to double arrows left and right
			} else {
				// change the mouse pointer to double up and down arrows
			}
		}else{
		}
			// change mouse pointer to normal


	}
	return TRUE;
}

/***************************************************
****************************************************/
BOOL CUGAdvProgressType::OnLClicked(int col, long row, int updn, RECT *rect, POINT *point){
	if (!m_canAdjust)
		return UG_SUCCESS;

	int ref, span;
	CUGCell mycell;
	float ftemp;
	if (updn==1){
		m_ctrl->GetCell(col, row, &mycell);

		switch(m_direction){
		case UGCT_PROGRESS_LEFT:
			ref = point->x - rect->left;
			span = rect->right - rect->left; 
			break;
		case UGCT_PROGRESS_TOP:
			ref = point->y - rect->top;
			span = rect->bottom - rect->top; 
			break;
		case UGCT_PROGRESS_RIGHT:
			ref = point->x - rect->right;
			span = rect->left - rect->right; 
			break;
		case UGCT_PROGRESS_BOTTOM:
			ref = point->y - rect->bottom;
			span = rect->top - rect->bottom; 
			break;
		}
		ftemp = ((float)ref / (float)span) * 100;
		if (ftemp>98)
				ftemp=(float)100;
		if (ftemp<2)
				ftemp=(float)0;

		mycell.SetNumber(ftemp);
		m_ctrl->SetCell(col, row, &mycell);

		m_lbuttondown = TRUE;
		m_row = row;
		m_col = col;
		CopyRect(&m_rect,rect);
		m_ctrl->RedrawCell(col, row);

		return TRUE;
	} else {
		// the mouse button was released
		m_lbuttondown = FALSE;

	}

	return FALSE;
}
/***************************************************
****************************************************/
BOOL CUGAdvProgressType::OnDClicked(int col,long row,RECT *rect,POINT *point){
	return OnLClicked(col, row, TRUE, rect, point);
}


/***************************************************
****************************************************/
int CUGAdvProgressType::CalcPointFromValue (int percent, POINT *point){
	return UG_SUCCESS;
}		


/*****************************************************

This functions sets the number of divisions within the 
progress bar area.  Progress bars can be all one color,
or may be divided into different colored regions to 
handle different situations.  

You may wish to use a progress bar with 3 areas, green
for the safe zone, yellow for the warning zone, and red
for the danger zone.

When you call SetNumberSections, it will automatically
assign default colors and percentage markers to the
progress bar.  If you wish to change the defaults 
(which you probably will), then you will need to call
SetSectionPercent(), and SetSectionColor();


*******************************************************/
int CUGAdvProgressType::SetNumberSections(int sections){

	if (sections<0 || sections>100)
		return 1;

	int i;

	m_numberSections = sections;

	// if the lists already exists, delete
	if (m_sectionPercent != NULL){
		delete[] m_sectionPercent;
		delete[] m_sectionColor;
	}

	// create new lists for section data
	m_sectionPercent = new int[sections];
	m_sectionColor   = new COLORREF[sections];

	// assign default values
	for (i=0;i<=sections;i++){

		SetSectionColor(i,RGB(255 - (200/sections) * i,0,0));
		SetSectionPercent(i, (100/sections * i));
	}
	return UG_SUCCESS;
}
		 

/*****************************************************

This function is called after SetNumberSections()

This function allows you to set the percentage at which
the progress bar color will change.  The region that
will be colored will defined by the number you pass this
function, up to the value assigned to the next highest
section.

For example if you set section 1 to 50, and section 2
to 80, you would have a progress bar with 3 regions,
0 to 50, 50 to 80, and 80 to 100.  You cannot set
the percent for section 0, as it is always 0 which is
the beginning position of the progress bar.

*******************************************************/

int CUGAdvProgressType::SetSectionPercent(int section, int percent){
	if (percent<0 || percent >100)
		return 1;

	if (section < 0 || section >= m_numberSections)
		return 1;
	if (section>0)
		if (m_sectionPercent[section-1]+1 > percent)
			return 1;

	m_sectionPercent[section] = percent;
	// assign the percent here
	return UG_SUCCESS;
}


/*****************************************************

This function is called after SetNumberSections()

This function allows you to set the color of a given 
Section. 

You may wish to color the first section green, the
second section yellow, and the final section red,
for example to illustrate safety, warning and danger
levels.

*******************************************************/

int CUGAdvProgressType::SetSectionColor(int section, COLORREF color){

	if (section < 0 || section >= m_numberSections)
		return 1;

	// assign section color to appropriate section
	m_sectionColor[section] = color;
	return UG_SUCCESS;
}

/*****************************************************

This function allows you to change the interval of the
graduation lines in the progress bar.  Graduation lines
are the "ruler" type markings that can optionally appear
in a progress bar.

A good standard default is 5%.  If 5% is chosen, markings 
at 25% and 50% will be emphasized automatically, if you
choose another percentage, no emphasis will appear.

You can disable the graduation lines entirely by passing
a value of 0;

*******************************************************/

int CUGAdvProgressType::SetGraduationLinePercent(int percent){
	if (percent<0 || percent >100)
		return 1;

	m_gradlinePercent = percent;
	return UG_SUCCESS;

}


/***************************************************
****************************************************/
int CUGAdvProgressType::SetCanAdjust(int adjust){
	if (adjust)
		m_canAdjust = TRUE;
	return UG_SUCCESS;
}

/***************************************************
****************************************************/
int CUGAdvProgressType::SetDirection(int direction){

	// bounds checking
	if (direction != UGCT_PROGRESS_TOP)
		if (direction != UGCT_PROGRESS_LEFT)
			if (direction != UGCT_PROGRESS_RIGHT)
				if (direction != UGCT_PROGRESS_BOTTOM)
					return 1;

	m_direction = direction;
	return UG_SUCCESS;
}

/***************************************************
****************************************************/
int CUGAdvProgressType::SetMargin(int margin){
	m_margin = margin;
	return UG_SUCCESS;
}

