/*************************************************************************
				Class Declaration : CUGLabeledType
**************************************************************************
	Source file : UGCTLabeled.cpp
	Header file : UGCTLabeled.h
	Copyright ? Dundas Software Ltd. 1994 - 2002, All Rights Reserved

    Purpose
		This class will automatically size the font of any cell using 
		this celltype so that all of the text will be visible.
*************************************************************************/
#pragma once



//CUGLabeledType class declaration
class UG_CLASS_DECL CUGLabeledType: public CUGCellType
{
protected:

	int	  m_fontHeight;
	CFont m_labelFont;
	COLORREF m_labelColor;

public:

	CUGLabeledType();
	~CUGLabeledType();

	//overloaded CUGCellType functions
	virtual LPCTSTR GetName();
	virtual LPCUGID GetUGID();
	virtual int GetEditArea(RECT *rect);
	virtual void OnDraw(CDC *dc,RECT *rect,int col,long row,CUGCell *cell,int selected,int current);
	virtual void GetBestSize(CDC *dc,CSize *size,CUGCell *cell);

	int SetLabelFont(int height,int weight,LPCTSTR name,COLORREF color);
};

