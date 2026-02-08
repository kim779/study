/*************************************************************************
				Class Declaration : CUGNoteType
**************************************************************************
	Source file : UGCTNote.cpp
	Header file : UGCTNote.h
	Copyright ? Dundas Software Ltd. 1994 - 2002, All Rights Reserved

    Purpose
		
*************************************************************************/
#pragma once

#include "UGCTNoteWnd.h"

#define ID_NOTEWND 300
#define OFSET_VERT 15
#define OFSET_HORIZ 5

static CRect m_noteCellRect;

//CUGNoteType class declaration
class UG_CLASS_DECL CUGNoteType: public CUGCellType
{
protected:
	
	int m_timerID;

	BOOL m_bInit;
	CUGNoteTypeEWnd m_noteWnd;

	int m_nNoteWidth,m_nNoteHeight;
	
	static VOID EXPORT CALLBACK TimerProc(
			HWND hwnd,         // handle to window
			UINT uMsg,         // WM_TIMER message
			unsigned int idEvent,  // timer identifier
			DWORD dwTime       // current system time
			);

public:

	CUGNoteType();
	~CUGNoteType();

	//overloaded CUGCellType functions
	virtual LPCTSTR GetName();
	virtual LPCUGID GetUGID();
	virtual void OnDraw(CDC *dc,RECT *rect,int col,long row,
		CUGCell *cell,int selected,int current);
	virtual void OnSetFocus(int col,long row,CUGCell *cell);
	virtual void OnKillFocus(int col,long row,CUGCell *cell);
	virtual BOOL OnLClicked(int col,long row,int updn,RECT *rect,POINT *point);
	virtual BOOL OnMouseMove(int col,long row,POINT *point,UINT nFlags);
	virtual void OnScrolled(int col,long row,CUGCell *cell);

	int SetNoteSize(int width, int height);

};

