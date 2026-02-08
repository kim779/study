/***************************************************
Ultimate Grid 3.0 for MFC
Dundas Software Ltd. 1994-1996

	Multiple Font CellType
****************************************************/

#include "stdafx.h"

#include "../UGCtrl.h"
#include "../_Include/UGCTmfnt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/***************************************************
****************************************************/
CUGMultiFontType::CUGMultiFontType(){
	m_font = NULL;
	m_canOverLap = FALSE;
}

/***************************************************
****************************************************/
CUGMultiFontType::~CUGMultiFontType(){
	if(m_font != NULL)
		delete m_font;

}
/***************************************************
****************************************************/
void CUGMultiFontType::OnDraw(CDC *dc,RECT *rect,int col,long row,
							  CUGCell *cell,int selected,int current){


	SIZE size;
	COLORREF backcolor;
	int left,top;
	int len,x,y;
	CString param;
	int once = 1;
	int oldleft = rect->left;
	CString string;
	short alignment;

	long properties = cell->GetPropertyFlags();

	if(properties&UGCELL_TEXT_SET)
		string = cell->GetText();


	DrawBorder(dc,rect,rect,cell);


	//check the selected and current states
	if(selected || (current && m_ctrl->m_GI->m_currentCellMode&2)){
		dc->SetTextColor(cell->GetHTextColor());
		dc->SetBkColor(cell->GetHBackColor());
		backcolor = cell->GetHBackColor();
	}
	else{
		dc->SetTextColor(cell->GetTextColor());
		dc->SetBkColor(cell->GetBackColor());
		backcolor = cell->GetBackColor();
	}
	
	//check for bitmaps
	if(properties&UGCELL_BITMAP_SET){
		oldleft = rect->left;
		rect->left += DrawBitmap(dc,cell->GetBitmap(),rect,backcolor);
	}

	//text alignment
	if(properties&UGCELL_ALIGNMENT_SET)
		alignment = cell->GetAlignment();
	else
		alignment = 0;

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

	len = string.GetLength();

	for(x=0;x<len;x++){
		//check for the beginning of a format option
		if(string.GetAt(x) =='<' && string.GetAt(x+1) ==':'){
			//get the format option string
			y = x+2;
			while(string.GetAt(x) !='>' && x <len){
				x++;
			}
			ParseCommand(dc,&string.Mid(y,x-y));
			x++;
			if(x >= len)
				break;
		}
		//***draw the text*** 
		if(once){
			once = FALSE;
			dc->ExtTextOut(left,top,ETO_OPAQUE|ETO_CLIPPED,rect,string.Mid(x,1),1,NULL);
		}
		else
			dc->ExtTextOut(left,top,ETO_CLIPPED,rect,string.Mid(x,1),1,NULL);

		size = dc->GetTextExtent(string.Mid(x,1));
		left += size.cx;
		rect->left = left;
	}

	//reset the rect
	rect->left = oldleft;
}


/***************************************************
****************************************************/
int CUGMultiFontType::ParseCommand(CDC *dc,CString *command){


	int x;
	int pos[30];
	int len = command->GetLength();
	int count =0;
	CString fontName;
	CString string;
	int r,g,b,p;

	//make into upper case
	command->MakeUpper();

	
	//get all the position params
	for(x=0;x<len && count <29;x++){
		if(command->GetAt(x) ==_T(',')){
			pos[count] = x;
			count++;
		}
	}
	pos[count] = x;
	
	//see if it is a font command
	if(command->GetAt(0)==_T('F')){
		fontName = command->Mid(pos[0]+1,pos[1]-pos[0]-1);
		string = command->Mid(pos[1]+1,pos[2]-pos[1]-1);
		p = _ttoi(string);
		if(m_font != NULL)
			delete m_font;
		m_font = new CFont;
		m_font->CreateFont(p,0,0,0,500,0,0,0,0,0,0,0,0,fontName);
		dc->SelectObject(m_font);
	}
	else if(command->GetAt(0)==_T('C')){
		string = command->Mid(pos[0]+1,pos[1]-pos[0]-1);
		r = _ttoi(string);
		string = command->Mid(pos[1]+1,pos[2]-pos[1]-1);
		g = _ttoi(string);
		string = command->Mid(pos[2]+1,pos[3]-pos[2]-1);
		b = _ttoi(string);
		dc->SetTextColor(RGB(r,g,b));
	}

	return 1;
}
