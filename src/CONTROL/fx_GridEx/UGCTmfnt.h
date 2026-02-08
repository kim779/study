/*************************************************************************
				Class Declaration : CUGMultiFontType
**************************************************************************
	Source file : UGCTmfnt.cpp
	Header file : UGCTmfnt.h
	Copyright ? Dundas Software Ltd. 1994 - 2002, All Rights Reserved

	Purpose
		This celltype uses a simple inline syntax for specifying 
		fonts and colors within the text that is to be displayed
	Details
		The formating is performed inline unsing the following syntax:
			
			<:xxx>
		
		Where xxx is replaced with the formating criteria

		To change the font use the following:
			
			<:F,name,size,[weight]>

		Where name is the name of the font, size is the height of the font
		and weight is the boldness of the font. Weight is optional and defaults
		to 400. Valid wieght values are from 0 to 1000.

		To change the color of the font use the following:

			<:C,r,g,b>

		Where r,g,b make up the RGB value. Valid values are 0 to 255.
*************************************************************************/

#pragma once

//CUGMultiFontType Declaration
class UG_CLASS_DECL CUGMultiFontType: public CUGCellType
{
protected:
	std::unique_ptr<CFont> m_font;
	CFont* m_oldFont;

public:
	CUGMultiFontType();
	~CUGMultiFontType();

	//overloaded CUGCellType functions
	virtual LPCTSTR GetName();
	virtual LPCUGID GetUGID();
	virtual void OnDraw(CDC *dc,RECT *rect,int col,long row,CUGCell *cell,int selected,int current);
	virtual void GetBestSize(CDC *dc,CSize *size,CUGCell *cell);
	
	int ParseCommand(CDC *dc,CString *command);
};


