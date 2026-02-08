/***************************************************
Ultimate Grid 3.0 for MFC
Dundas Software Ltd. 1994-1996

	Cell Type Class
****************************************************/

#include "stdafx.h"

#include "../UGCtrl.h"
#include "../_Include/UGCTsldr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/***************************************************
****************************************************/
CUGSliderType::CUGSliderType(){
	SetTextEditMode(FALSE);

	m_grayPen.CreatePen(PS_SOLID,1,RGB(200,200,200));
	m_darkGrayPen.CreatePen(PS_SOLID,1,RGB(100,100,100));

	m_updateSlider = FALSE;
}

/***************************************************
****************************************************/
CUGSliderType::~CUGSliderType(){
}

/***************************************************
****************************************************/
BOOL CUGSliderType::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point){

	if(updn){
		m_updateSlider = FALSE;
	}

	CUGCell cell;
	m_ctrl->GetCell(col,row,&cell);

	//get the slider percentage
	int percent = 0;
	if(cell.IsPropertySet(UGCELL_STRING_SET))
		percent = _ttol(cell.GetText());
	//get the percent
	if(percent >100)
		percent = 100;
	else if(percent <0)
		percent = 0;

	//find the slider x position
	long width = rect->right - rect->left - 6;
	int xPos = rect->left + 3 + ((width * percent) / 100);

	int mouseX = point->x;

	if(mouseX > xPos-4 && mouseX < xPos+4){
		m_updateSlider = TRUE;
		return TRUE;
	}

	return FALSE;
}

/***************************************************
****************************************************/
BOOL CUGSliderType::OnMouseMove(int col,long row,POINT *point,UINT flags){

	if((flags& MK_LBUTTON)== FALSE){
		return FALSE;
	}

	if(m_updateSlider){

		RECT rect;
		m_ctrl->GetCellRect(col,row,&rect);

		long mouseX = point->x - rect.left - 3;

		if(mouseX < 0)
			mouseX = 0;

		long width = rect.right - rect.left - 6;

		long percent = (mouseX*100)/width;

		if(percent < 0)
			percent = 0;
		if(percent > 100)
			percent = 100;

		CUGCell cell;
		m_ctrl->GetCell(col,row,&cell);
		if(percent != (long)cell.GetNumber()){
			cell.SetNumber(percent);
			m_ctrl->SetCell(col,row,&cell);
			m_ctrl->RedrawCell(col,row);
		}
		return TRUE;
	}
	return FALSE;
}

/***************************************************
****************************************************/
void CUGSliderType::OnDraw(CDC *dc,RECT *rect,int col,long row,
						   CUGCell *cell,int selected,int current){

	DrawBorder(dc,rect,rect,cell);

	long properties = cell->GetPropertyFlags();

	//get the slider percentage
	int percent = 0;
	if(properties&UGCELL_STRING_SET)
		percent = _ttol(cell->GetText());
	//get the percent
	if(percent >100)
		percent = 100;
	else if(percent <0)
		percent = 0;

	//fill in the background
	dc->SetBkColor(cell->GetBackColor());
	dc->ExtTextOut(rect->left,rect->top,ETO_OPAQUE|ETO_CLIPPED,rect,_T(""),0,NULL);

	//make sure the window is wide enough
	int width = rect->right - rect->left - 6;
	if(width <= 6){
		return;
	}

	//find the slider x position
	int xPos = rect->left + ((width * percent) / 100) +3;

	int yPosMid = rect->top + (rect->bottom - rect->top)/2;
	
	//draw the slider track
	dc->SelectObject((CPen*)CPen::FromHandle((HPEN)GetStockObject(BLACK_PEN)));
	dc->MoveTo(rect->left+1,yPosMid-1);
	dc->LineTo(rect->right,yPosMid-1);
	dc->MoveTo(rect->left+1,yPosMid+1);
	dc->LineTo(rect->right,yPosMid+1);
	dc->SelectObject((CPen *)&m_darkGrayPen);
	dc->MoveTo(rect->left+1,yPosMid);
	dc->LineTo(rect->right,yPosMid);

	//draw the slider
	dc->MoveTo(xPos+3,rect->top+2);
	dc->LineTo(xPos-2,rect->top+2);
	dc->LineTo(xPos-2,rect->bottom -2);
	dc->LineTo(xPos+3,rect->bottom -2);
	dc->LineTo(xPos+3,rect->top+2);
	dc->SelectObject((CPen*)CPen::FromHandle((HPEN)GetStockObject(WHITE_PEN)));
	dc->MoveTo(xPos+2,rect->top+3);
	dc->LineTo(xPos-1,rect->top+3);
	dc->LineTo(xPos-1,rect->bottom -3);
	dc->SelectObject((CPen *)&m_darkGrayPen);
	dc->LineTo(xPos+2,rect->bottom -3);
	dc->LineTo(xPos+2,rect->top+3);	
	dc->SelectObject((CPen *)&m_grayPen);
	dc->MoveTo(xPos+1,rect->top+4);
	dc->LineTo(xPos,rect->top+4);
	dc->LineTo(xPos,rect->bottom-4);
	dc->LineTo(xPos+1,rect->bottom-4);
	dc->LineTo(xPos+1,rect->top+4);
}
