/***************************************************
Ultimate Grid 3.0 for MFC
Dundas Software Ltd. 1994-1996

	Cell Type Class
****************************************************/

#include "stdafx.h"

#include "../UGCtrl.h"
#include "../_Include/UGCTspin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define UG_SETDROPLIST 100
#define UG_CLEARDROPLIST 101

/***************************************************
****************************************************/
CUGSpinButtonType::CUGSpinButtonType(){
		
	//set up the variables
	btnWidth = GetSystemMetrics(SM_CXVSCROLL);

	btnTopDown = FALSE;
	btnBottomDown = FALSE;

	pen.CreatePen(PS_SOLID,1,GetSysColor(COLOR_BTNFACE));
	brush.CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
}

/***************************************************
****************************************************/
CUGSpinButtonType::~CUGSpinButtonType(){

}
/***************************************************
****************************************************/
int CUGSpinButtonType::GetEditArea(RECT *rect){
	
	rect->right -=(btnWidth+1);

	return TRUE;
}
/***************************************************
****************************************************/
BOOL CUGSpinButtonType::OnDClicked(int col,long row,RECT *rect,POINT *point){
	return OnLClicked(col,row,1,rect,point);
}
/***************************************************
****************************************************/
BOOL CUGSpinButtonType::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point){
	
	if(updn){
		if(point->x > (rect->right -btnWidth)){

			//copy the droplist button co-ords
			CopyRect(&btnRect,rect);
			btnRect.left = rect->right - btnWidth;
			
			btnCol = col;
			btnRow = row;

			//check to see which button
			if(point->y < (rect->top +(rect->bottom - rect->top)/2)){
				//Top button
				btnTopDown =	TRUE;
				btnBottomDown = FALSE;
				btnTopBottom = 1;
				btnRect.bottom = (rect->top +(rect->bottom - rect->top)/2);

				//notify the user of the list, so it can be modified if needed
				m_ctrl->OnCellTypeNotify(m_ID,btnCol,btnRow,UGCT_SPINBUTTONUP,0);
			}
			else{
				//bottom button
				btnBottomDown = TRUE;
				btnTopDown =	FALSE;
				btnTopBottom = 2;
				btnRect.top = (rect->top +(rect->bottom - rect->top)/2);

				//notify the user of the list, so it can be modified if needed
				m_ctrl->OnCellTypeNotify(m_ID,btnCol,btnRow,UGCT_SPINBUTTONDOWN,0);
			}
			m_ctrl->RedrawCell(btnCol,btnRow);
			return TRUE;
		}
	}

	else if(btnTopDown || btnBottomDown){		
		btnTopDown =	FALSE;
		btnBottomDown = FALSE;
		btnTopBottom =	0;
		m_ctrl->RedrawCell(col,row);
		return TRUE;
	}

	return FALSE;
}
/***************************************************
****************************************************/
BOOL CUGSpinButtonType::OnMouseMove(int col,long row,POINT *point,UINT flags){

	if((flags&MK_LBUTTON) == FALSE){
		btnTopDown =	FALSE;
		btnBottomDown = FALSE;
		return FALSE;
	}
	
	if(point->x >= btnRect.left && point->x <= btnRect.right){
		if(point->y >= btnRect.top && point->y <= btnRect.bottom){
			if(btnTopBottom==1 && !btnTopDown){
				btnTopDown = TRUE;
				m_ctrl->RedrawCell(btnCol,btnRow);
			}
			else if(btnTopBottom==2 && !btnBottomDown){
				btnBottomDown = TRUE;
				m_ctrl->RedrawCell(btnCol,btnRow);
			}
		}
		else if(btnTopDown || btnBottomDown){		
			btnTopDown = FALSE;
			btnBottomDown = FALSE;
			m_ctrl->RedrawCell(btnCol,btnRow);
		}
	}
	else if(btnTopDown || btnBottomDown){		
		btnTopDown = FALSE;
		btnBottomDown = FALSE;
		m_ctrl->RedrawCell(btnCol,btnRow);
	}

	return FALSE;
}

/***************************************************

  current bit:	0-do nothing
				1-highlight
				
****************************************************/
void CUGSpinButtonType::OnDraw(CDC *dc,RECT *rect,int col,long row,
							  CUGCell *cell,int selected,int current){
	
	int style = 0;
	if( cell->IsPropertySet( UGCELL_CELLTYPEEX_SET ) )
		style = cell->GetCellTypeEx();

	//if the cell is not current and hide button is on
	//then dont draw the button
	if( style & UGCT_SPINBUTTONHIDEBUTTON && !current)
	{
		CUGCellType::OnDraw(dc,rect,col,row,cell,selected,current);
		return;
	}

	
	
	DrawBorder(dc,rect,rect,cell);

	int t,x,y;
	RECT rectout;
	CPen * oldpen;
	int left = rect->left;
	int height = rect->bottom - rect->top;

	rect->left = rect->right - btnWidth;
	
	//draw the top button
	t = rect->bottom;
	rect->bottom = 	(rect->top +(rect->bottom - rect->top)/2);
	if(btnTopDown && current){
		cell->SetBorder(UG_BDR_RECESSED);
		DrawBorder(dc,rect,&rectout,cell);
	}
	else{
		cell->SetBorder(UG_BDR_RAISED);
		DrawBorder(dc,rect,&rectout,cell);
	}
	//fill the border in
	dc->SetBkColor(GetSysColor(COLOR_BTNFACE));
	dc->FillRect(&rectout,&brush);
	//draw the down arrow
	if(height >9){
		x= ((btnWidth-5)/2) + rect->left;
		y = ((rect->bottom - rect->top -3)/2) + rect->top;
		dc->SelectObject((CPen*)CPen::FromHandle((HPEN)GetStockObject(BLACK_PEN)));
		dc->MoveTo(x,y+2);
		dc->LineTo(x+5,y+2);
		dc->MoveTo(x+1,y+1);
		dc->LineTo(x+4,y+1);
		dc->MoveTo(x+2,y);
		dc->LineTo(x+2,y+1);
	}
	//reset the rect
	rect->bottom = t;

	//draw the bottom button
	t = rect->top;
	rect->top = 	(rect->top +(rect->bottom - rect->top)/2);
	if(btnBottomDown && current){
		cell->SetBorder(UG_BDR_RECESSED);
		DrawBorder(dc,rect,&rectout,cell);
	}
	else{
		cell->SetBorder(UG_BDR_RAISED);
		DrawBorder(dc,rect,&rectout,cell);
	}
	//fill the border in
	dc->SetBkColor(GetSysColor(COLOR_BTNFACE));
	dc->FillRect(&rectout,&brush);
	//draw the down arrow
	if(height >9){
		x= ((btnWidth-5)/2) + rect->left;
		y = ((rect->bottom - rect->top -3)/2) + rect->top +1;
		dc->SelectObject((CPen*)CPen::FromHandle((HPEN)GetStockObject(BLACK_PEN)));
		dc->MoveTo(x,y);
		dc->LineTo(x+5,y);
		dc->MoveTo(x+1,y+1);
		dc->LineTo(x+4,y+1);
		dc->MoveTo(x+2,y+2);
		dc->LineTo(x+2,y+1);
	}
	//reset the rect
	rect->top = t;

	//make a line to separate the border from the rest ofthe cell
	oldpen = (CPen *)dc->SelectObject((CPen *)&pen);
	dc->MoveTo(rect->left-1,rect->top);
	dc->LineTo(rect->left-1,rect->bottom);
	dc->SelectObject(oldpen);
	
	//draw the text in using the default drawing routine
	rect->left =left;
	rect->right -=(btnWidth+1);

	DrawText(dc,rect,0,col,row,cell,selected,current);

	return;
}
