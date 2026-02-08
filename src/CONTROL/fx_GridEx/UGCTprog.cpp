/*************************************************************************
				Class Implementation : CUGProgressType
**************************************************************************
	Source file : UGCTprog.cpp
	Copyright ?Dundas Software Ltd. 1994 - 2002, All Rights Reserved
*************************************************************************/

#include "stdafx.h"
#include "UGCtrl.h"
#include "UGCTprog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***************************************************
CUGProgressType - Constructor
	Intialize all member variables here
****************************************************/
CUGProgressType::CUGProgressType(){

}
/***************************************************
~CUGProgressType - Destructor
	Free all allocated resources here
****************************************************/
CUGProgressType::~CUGProgressType(){
}

/***************************************************
GetName  - overloaded CUGCellType::GetName
	Returns a readable name for the cell type.
	Returned value is used to help end-users
	to see what cell type are available.

    **See CUGCellType::GetName for more details
	about this function

Params:
	<none>
Return:
	cell type name
****************************************************/
LPCTSTR CUGProgressType::GetName()
{
	return _T("Progress Bar");
}

/***************************************************
GetUGID  - overloaded CUGCellType::GetUGID
	Returns a GUID for the cell type, this number
	is unique for each cell type and never changes.
	This number can be used to find the cell types
	added to this instance of the Ultimate Grid.

    **See CUGCellType::GetUGID for more details
	about this function

Params:
	<none>
Returns:
	UGID (which is actually a GUID)
****************************************************/
LPCUGID CUGProgressType::GetUGID()
{
	static const UGID ugid = 
	{ 0x4585888d, 0xd9eb, 0x11d5, { 0x9b, 0x37, 0x0, 0x50, 0xba, 0xd4, 0x4b, 0xcd } };

	return &ugid;
}

/***************************************************
OnDraw - overloaded CUGCellType::OnDraw
	Draws the progress bar, using the cells text field
	as a percentage, and the cells label field as a
	label

    **See CUGCellType::OnDraw for more details
	about this function
****************************************************/
void CUGProgressType::OnDraw(CDC *dc,RECT *rect,int col,long row,
							CUGCell *cell,int selected,int current){

	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(selected);
	UNREFERENCED_PARAMETER(current);

	DrawBorder(dc,rect,rect,cell);

	int  fillpos{};
	RECT fillrect{};
	int left{}, top{};
	SIZE size{};

	CString label;
	int labelSet = FALSE;
		
	int alignment{};
	long percent = 0;

	const long properties = cell->GetPropertyFlags();

	if(properties&UGCELL_STRING_SET)
		percent = (long)cell->GetNumber();


	//get the percent
	if(percent >100)
		percent = 100;
	else if(percent <0)
		percent = 0;

	if(properties&UGCELL_CELLTYPEEX_SET){
		if(cell->GetCellTypeEx() == UGCT_PROGRESSUSELABEL){
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
	DrawBackground( dc, &fillrect, cell->GetBackColor());

	fillrect.top++;
	fillrect.bottom --;

	//get the first rect
	fillpos = fillrect.left + (int)(((long)(fillrect.right - fillrect.left)*percent)/(long)100);
	fillrect.right = fillpos;
	dc->SetBkColor(cell->GetHBackColor());
	DrawBackground( dc, &fillrect, cell->GetHBackColor());
	dc->SetTextColor(cell->GetHTextColor());
	dc->ExtTextOut(left,top,ETO_CLIPPED,&fillrect,label,label.GetLength(),nullptr);

	//draw the second half
	if(percent <100){
		fillrect.right = rect->right;
		fillrect.left = fillpos;
		DrawBackground( dc, &fillrect, cell->GetBackColor());
		dc->SetTextColor(cell->GetTextColor());
		dc->ExtTextOut(left,top,ETO_CLIPPED,&fillrect,label,label.GetLength(),nullptr);
	}

	return;
}

/****************************************************
GetBestSize
	Returns the best (nominal) size for a cell using
	this cell type, with the given cell properties.
Params:
	dc		- device context to use to calc the size on	
	size	- return the best size in this param
	cell	- pointer to a cell object to use for the calc.
Return:
	<none>
*****************************************************/
void CUGProgressType::GetBestSize(CDC *dc,CSize *size,CUGCell *cell)
{
	long percent{};
	CString string;

	// create a string as user sees it in the cell
	if( cell->GetPropertyFlags()&UGCELL_STRING_SET )
		percent = (long)cell->GetNumber();

	//get the percent
	if( percent > 100 )
		percent = 100;
	else if( percent < 0 )
		percent = 0;

	if(cell->GetCellTypeEx() == UGCT_PROGRESSUSELABEL)
		string = cell->GetLabelText();
	else
		string.Format(_T("%% %ld"),percent);

	cell->SetText( string );
	// call the parent class implementation
	CUGCellType::GetBestSize( dc, size, cell );
}