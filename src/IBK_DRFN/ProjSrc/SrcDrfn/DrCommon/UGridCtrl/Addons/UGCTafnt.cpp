/***************************************************
Ultimate Grid 3.0 for MFC
Dundas Software Ltd. 1994-1996

	Cell Type Class
****************************************************/

#include "stdafx.h"

#include "../UGCtrl.h"
#include "../_Include/UGCTAfnt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***************************************************
****************************************************/
CUGAutoFontType::CUGAutoFontType(){
	m_canOverLap = FALSE;
}
/***************************************************
****************************************************/
CUGAutoFontType::~CUGAutoFontType(){
}
/***************************************************
****************************************************/
void CUGAutoFontType::OnDraw(CDC *dc,RECT *rect,int col,long row,
							 CUGCell *cell,int selected,int current){


	DrawBorder(dc,rect,rect,cell);

	//create the font needed
	if(cell->IsPropertySet(UGCELL_FONT_SET))
		dc->SelectObject(cell->GetFont());

	HFONT newFont = NULL;

	if(cell->IsPropertySet(UGCELL_TEXT_SET)){
		int oldLeft = rect->left;
		rect->left += m_ctrl->m_GI->m_margin;
		newFont = CreateAutoSizeFont(dc->m_hDC,rect,cell->GetText());
		dc->SelectObject(newFont);
		cell->ClearPropertySetFlag(UGCELL_FONT_SET);
		rect->left = oldLeft;
	}

	//draw the text in using the default drawing routine
	DrawText(dc,rect,0,col,row,cell,selected,current);

	if(newFont != NULL)
		DeleteObject(newFont);
}

/***************************************************
****************************************************/
HFONT CUGAutoFontType::CreateAutoSizeFont(HDC hdc, RECT *rect,LPCTSTR string)
{
	HFONT	font;
	int		Height = 1;
	int		Width = rect->right - rect->left;

	SIZE	size;
	float   ratio;
	TEXTMETRIC tm;
	TCHAR	fontFace[60];
	int		rt;
	int saveID = SaveDC(hdc);
	
	GetTextFace(hdc,60,fontFace);
	GetTextMetrics(hdc,&tm);

	Height = Width/_tcslen(string);
	Height = Height + Height/2;

	font =  CreateFont(Height, 0, 0, 0, 600, 0,0,0,0,0,0,0,0,fontFace);
	SelectObject(hdc,font);

	GetTextExtentPoint(hdc,string,_tcslen(string),&size);
	ratio = (float)size.cy/(float)size.cx;
	Height = (int)((float)Width*ratio);
	rt = RestoreDC(hdc,saveID);
	rt = DeleteObject(font);
	font = CreateFont(Height, 0, 0, 0, 600, 0,0,0,0,0,0,0,0,fontFace);
	SelectObject(hdc,font);
	GetTextExtentPoint(hdc,string,_tcslen(string),&size);

	if (Width >size.cx)
	{
		while (Width > size.cx && Height < 500)
		{
			rt = RestoreDC(hdc,saveID);
			rt = DeleteObject(font);
			Height++;
			font = CreateFont(Height, 0, 0, 0, 600, 0,0,0,0,0,0,0,0,fontFace);
			SelectObject(hdc,font);
			GetTextExtentPoint(hdc,string,_tcslen(string),&size);

		}
		rt = RestoreDC(hdc,saveID);
		rt = DeleteObject(font);
		Height--;
		font = CreateFont(Height, 0, 0, 0, 600, 0,0,0,0,0,0,0,0,fontFace);

	}
	else if (Width < size.cx)
	{
		while (Width < size.cx  && Height > 1)
		{
			rt = RestoreDC(hdc,saveID);
			rt = DeleteObject(font);
			Height--;
			font = CreateFont(Height, 0, 0, 0, 600, 0,0,0,0,0,0,0,0,fontFace);
			SelectObject(hdc,font);
			GetTextExtentPoint(hdc,string,_tcslen(string),&size);
			if(Height<1)
				break;
		}

		rt = RestoreDC(hdc,saveID);
		rt = DeleteObject(font);
		Height--;
		font = CreateFont(Height, 0, 0, 0, 600, 0,0,0,0,0,0,0,0,fontFace);
	}
	if (Height >(rect->bottom - rect->top))
	{
		Height = rect->bottom - rect->top;
		if(Height <2)
			Height = 2;
		rt = RestoreDC(hdc,saveID);
		rt = DeleteObject(font);
		font = CreateFont(Height, 0, 0, 0, 600, 0,0,0,0,0,0,0,0,fontFace);
	}
	
	rt = RestoreDC(hdc,saveID);

	return font;
}
