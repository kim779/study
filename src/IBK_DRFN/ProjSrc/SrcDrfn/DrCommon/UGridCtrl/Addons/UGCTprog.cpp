/***************************************************
Ultimate Grid 3.0 for MFC
Dundas Software Ltd. 1994-1996

	Cell Type Class
****************************************************/

#include "stdafx.h"

#include "../UGCtrl.h"
#include "../_Include/UGCTprog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***************************************************
****************************************************/
CUGProgressType::CUGProgressType(){

}
/***************************************************
****************************************************/
CUGProgressType::~CUGProgressType(){
}
/***************************************************
****************************************************/
void CUGProgressType::OnDraw(CDC *dc,RECT *rect,int col,long row,
							CUGCell *cell,int selected,int current){
	

	DrawBorder(dc,rect,rect,cell);

	int  fillpos;
	RECT fillrect;
	int left,top;
	SIZE size;

	CString label;
	int		labelSet = FALSE;
		
	int alignment;
	long percent = 0;

	long		properties = cell->GetPropertyFlags();

	if(properties&UGCELL_STRING_SET)
		percent = (long)cell->GetNumber();


	//get the percent
	if(percent >100)
		percent = 100;
	else if(percent <0)
		percent = 0;

	if(properties&UGCELL_CELLTYPEEX_SET){
		if(cell->GetCellTypeEx() == 1){
			label = cell->GetLabelText();
			labelSet = TRUE;
		}
	}
	if(!labelSet){
		label.Format(_T("%% %ld"),percent);
	}

	//select the font
	if(properties&UGCELL_FONT_SET )
		dc->SelectObject(cell->GetFont());

	//text alignment
	if(properties&UGCELL_ALIGNMENT_SET)
		alignment = cell->GetAlignment();
	else
		alignment = 0;

	if(alignment & UG_ALIGNCENTER){
		GetTextExtentPoint(dc->m_hDC,label,label.GetLength(),&size);
		left = rect->left + (rect->right - rect->left - size.cx) /2;
	}
	else if(alignment & UG_ALIGNRIGHT){
		GetTextExtentPoint(dc->m_hDC,label,label.GetLength(),&size);
		left = rect->right - size.cx - 3;
	}
	else{
		left = rect->left+3;
	}	
	if(alignment & UG_ALIGNVCENTER){
		GetTextExtentPoint(dc->m_hDC,label,label.GetLength(),&size);
		top = rect->top + (rect->bottom - rect->top - size.cy) /2;
	}
	else if(alignment & UG_ALIGNBOTTOM){
		GetTextExtentPoint(dc->m_hDC,label,label.GetLength(),&size);
		top = rect->bottom - size.cy -1;
	}
	else{
		top = rect->top+1;
	}

	CopyRect(&fillrect,rect);
	
	dc->SetBkColor(cell->GetBackColor());
	dc->ExtTextOut(left,top,ETO_OPAQUE|ETO_CLIPPED,&fillrect,_T(""),0,NULL);

	fillrect.top++;
	fillrect.bottom --;

	//get the first rect
	fillpos = fillrect.left + (int)(((long)(fillrect.right - fillrect.left)*percent)/(long)100);
	fillrect.right = fillpos;
	dc->SetBkColor(cell->GetHBackColor());
	dc->SetTextColor(cell->GetHTextColor());
	dc->ExtTextOut(left,top,ETO_OPAQUE|ETO_CLIPPED,&fillrect,label,label.GetLength(),NULL);

	//draw the second half
	if(percent <100){
		fillrect.right = rect->right;
		fillrect.left = fillpos;
		dc->SetBkColor(cell->GetBackColor());
		dc->SetTextColor(cell->GetTextColor());
		dc->ExtTextOut(left,top,ETO_OPAQUE|ETO_CLIPPED,&fillrect,label,label.GetLength(),NULL);
	}


	return;
}
