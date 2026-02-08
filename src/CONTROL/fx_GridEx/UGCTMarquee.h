/*************************************************************************
				Class Declaration : CUGMarqueeType
**************************************************************************
	Source file : UGCTMarquee.cpp
	Header file : UGCTMarquee.h
	Copyright ? Dundas Software Ltd. 1994 - 2002, All Rights Reserved

    Purpose
		This class will automatically size the font of any cell using 
		this celltype so that all of the text will be visible.
*************************************************************************/
#pragma once



//CUGMarqueeType class declaration
class UG_CLASS_DECL CUGMarqueeType: public CUGCellType
{
protected:
	
	int m_timerID;
	long m_interval;

	void StartUp();

	static VOID CALLBACK EXPORT TimerProc(
			HWND hwnd,         // handle to window
			UINT uMsg,         // WM_TIMER message
			unsigned int idEvent,  // timer identifier
			DWORD dwTime       // current system time
			);

public:

	CUGMarqueeType();
	~CUGMarqueeType();

	int SetScrollInterval(long nMilliSec);

	//overloaded CUGCellType functions
	virtual LPCTSTR GetName();
	virtual LPCUGID GetUGID();
	virtual void OnDraw(CDC *dc,RECT *rect,int col,long row,
		CUGCell *cell,int selected,int current);

};

