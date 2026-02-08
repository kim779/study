/***************************************************
Ultimate Grid 3.0 for MFC
Dundas Software Ltd. 1994-1996

	Cell Type Class
****************************************************/

#include "stdafx.h"

#include "../UGCtrl.h"
#include "../_Include/UGCTRado.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***************************************************
****************************************************/
CUGRadioType::CUGRadioType(){

	SetTextEditMode(FALSE);
}
/***************************************************
****************************************************/
CUGRadioType::~CUGRadioType(){
}
/***************************************************
****************************************************/
BOOL CUGRadioType::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point){
	
	if(updn){

		//get the height of the font
		int itemHeight;
		CDC * dc = m_ctrl->GetDC();
		
		m_ctrl->GetCell(col,row,&m_cell);
		if(m_cell.IsPropertySet(UGCELL_FONT_SET) == FALSE)
			if(m_ctrl->m_GI->m_defFont != NULL)
			dc->SelectObject(m_ctrl->m_GI->m_defFont);
		else
			dc->SelectObject(m_cell.GetFont());

		CSize size = dc->GetTextExtent(_T("X"),1);
		m_ctrl->ReleaseDC(dc);
		itemHeight = size.cy;

		//check the x -co-ord
		//int x = point->x - rect->left;
		//if(x <3 || x > itemHeight)
			//return FALSE;

		//find which item was selected
		int y = point->y - rect->top;
		int select = y / itemHeight;

		//get the old selection
		m_ctrl->GetCell(col,row,&m_cell);
		int oldSelect = (int)m_cell.GetNumber();

		//get the number of items
		CString label = m_cell.GetLabelText();
		int itemCount = 0;
		int t;
		int len = label.GetLength();
		for(t = 0;t<len;t++){
			if(label.GetAt(t) == _T('\n')){
				itemCount++;
			}
		}
		if(select >= itemCount)
			return FALSE;
		
		//check to see if they are the same
		if(select != oldSelect){
			m_cell.SetNumber(select);
			m_ctrl->SetCell(col,row,&m_cell);
			m_ctrl->RedrawCell(col,row);
		}
	
		m_ctrl->OnCellTypeNotify(m_ID, col, row, UGCT_RADIOBUTTON_SELECTED, select);
		
		return TRUE;
	}
	return FALSE;
}
/***************************************************
****************************************************/
BOOL CUGRadioType::OnKeyDown(int col,long row,UINT *vcKey){

	if(*vcKey == VK_SPACE){
		
		int select,col,itemHeight,itemCount;
		long row;
		CString label;
		CDC * dc;
				
		col = m_ctrl->GetCurrentCol();
		row= m_ctrl->GetCurrentRow();

		dc = m_ctrl->GetDC();
		
		m_ctrl->GetCell(col,row,&m_cell);
		if(m_cell.IsPropertySet(UGCELL_FONT_SET) == FALSE){
			if(m_ctrl->m_GI->m_defFont != NULL)
				dc->SelectObject(m_ctrl->m_GI->m_defFont);
		}
		else
			dc->SelectObject(m_cell.GetFont());

		CSize size = dc->GetTextExtent(_T("X"),1);
		m_ctrl->ReleaseDC(dc);
		itemHeight = size.cy;

		select = (int)m_cell.GetNumber();

		//get the number of items
		label = m_cell.GetLabelText();
		itemCount = 0;
		int len = label.GetLength();
		for(int t = 0; t < len ;t++){
			if(label.GetAt(t) == _T('\n')){
				itemCount++;
			}
		}
		if(select >= itemCount - 1)
			select = 0;
		else
			select++;
		
		//check to see if they are the same
		m_cell.SetNumber(select);
		m_ctrl->SetCell(col,row,&m_cell);
		m_ctrl->RedrawCell(col,row);
			
		m_ctrl->OnCellTypeNotify(m_ID, col, row, UGCT_RADIOBUTTON_SELECTED, select);
		
		return TRUE;
	}
	return FALSE;
}
/***************************************************

  current bit:	0-do nothing
				1-highlight
				
****************************************************/
void CUGRadioType::OnDraw(CDC *dc,RECT *rect,int col,long row,
						 CUGCell *cell,int selected,int current){
	
	DrawBorder(dc,rect,rect,cell);

	long properties = cell->GetPropertyFlags();

	int saveID = dc->SaveDC();
	
	CRgn rgn;
	rgn.CreateRectRgnIndirect(rect);
	dc->SelectClipRgn(&rgn);

	int alignment;
	if(properties&UGCELL_ALIGNMENT_SET)
		alignment = cell->GetAlignment();
	else
		alignment = 0;

	//select the font
	if(properties&UGCELL_FONT_SET )
		dc->SelectObject(cell->GetFont());

	//get the back color of the cell
	if(properties&UGCELL_BACKCOLOR_SET)
		dc->SetBkColor(cell->GetBackColor());
	else
		dc->SetBkColor(GetSysColor(COLOR_WINDOW));

	if(properties&UGCELL_TEXTCOLOR_SET)
		dc->SetTextColor(cell->GetTextColor());
	else
		dc->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	
	//clear the cell
	dc->ExtTextOut(rect->left,rect->top,ETO_OPAQUE,rect,_T(""),0,NULL);
	
	//get the height of the font
	int itemHeight;
	CSize size = dc->GetTextExtent(_T("X"),1);
	itemHeight = size.cy;

	//get the number of items
	CString label;
	if(properties&UGCELL_LABEL_SET)
		label = cell->GetLabelText();

	int itemCount = 0;
	int t;
	int len = label.GetLength();
	for(t = 0;t<len;t++){
		if(label.GetAt(t) == _T('\n')){
			itemCount++;
		}
	}
	//get the pointers to each
	int *positions = new int[itemCount+1];
	positions[0] = 0;
	int count =1;
	for(t = 0;t<len;t++){
		if(label.GetAt(t) == _T('\n')){
			positions[count] = t+1;
			count++;
		}
	}

	//get the selected item
	int selectNum = (int)cell->GetNumber();

	//draw the items
	RECT itemRect;
	CopyRect(&itemRect,rect);
	itemRect.left +=3;
	itemRect.top ++;
	for(t=0;t<itemCount;t++){
		//draw the circle
		itemRect.bottom = itemRect.top+itemHeight;
		itemRect.right = itemRect.left+itemHeight;
		itemRect.top+=2;
		itemRect.bottom-=2;
		itemRect.right-=3;
		//dc->SelectObject(GetStockObject(WHITE_BRUSH));
		dc->SelectObject((CBrush*)CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
		dc->Ellipse(&itemRect);
		
		//draw the selected button
		if(selectNum == t){
			itemRect.left+=2;
			itemRect.top+=2;
			itemRect.bottom-=2;
			itemRect.right-=2;
			//dc->SelectObject(GetStockObject(BLACK_BRUSH));
			dc->SelectObject((CBrush*)CBrush::FromHandle((HBRUSH)GetStockObject(BLACK_BRUSH)));
			dc->Ellipse(&itemRect);
			itemRect.left-=2;
    		itemRect.top-=2;
	    	itemRect.bottom+=2;
		    itemRect.right+=2;
		}

		//draw the label text
		itemRect.top-=2;
		itemRect.bottom+=2;
		itemRect.right+=3;
		itemRect.right = rect->right;
		itemRect.left += itemHeight;
	
		//text alignment
		CString label = cell->GetLabelText();
		CString string = label.Mid(positions[t],positions[t+1] - positions[t]-1);
		int left;
		if(alignment & UG_ALIGNCENTER){
			GetTextExtentPoint(dc->m_hDC,string,lstrlen(string),&size);
			left = itemRect.left + (itemRect.right - itemRect.left - size.cx) /2;
		}
		else if(alignment & UG_ALIGNRIGHT){
			GetTextExtentPoint(dc->m_hDC,string,lstrlen(string),&size);
			left = itemRect.right - size.cx - 3;
		}
		else{
			left = itemRect.left+3;
		}	
		//draw
		dc->ExtTextOut(left,itemRect.top,ETO_CLIPPED,
			&itemRect,
			label.Mid(positions[t],positions[t+1] - positions[t]-1) 
			,NULL);
		itemRect.left -= itemHeight;
		itemRect.top = itemRect.bottom;
	}
	
	delete positions;
	dc->RestoreDC(saveID);

}
