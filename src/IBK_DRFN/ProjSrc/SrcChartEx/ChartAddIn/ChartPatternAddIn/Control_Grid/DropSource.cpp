#include "stdafx.h"  
#include "afxole.h"
#include "DropSource.h"

//#include "MessageDefine.h"

CDropSource::CDropSource()
{
}


CDropSource::~CDropSource()
{
}


DROPEFFECT CDropSource::StartDragging(DWORD Data, RECT * rClient, CPoint * MousePos)
{
     
	//Use the following if you are only using text
	//HGLOBAL hgData=GlobalAlloc(GPTR,10 );   
	HGLOBAL hgData=GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT ,10 );
    ASSERT(hgData!=NULL);
    
    
	LPCSTR  lpData=(LPCSTR)GlobalLock(hgData);
    ASSERT(lpData!=NULL);
	ltoa( Data, (char*)lpData, 10 );

	//Use the following if you are only using text
    CacheGlobalData(CF_TEXT, hgData);   
	//CacheGlobalData(CF_OWNERDISPLAY, hgData);   
  
    DROPEFFECT dropEffect=DoDragDrop(DROPEFFECT_COPY|DROPEFFECT_MOVE,(LPCRECT)rClient);  
	
    if((dropEffect&DROPEFFECT_MOVE)==DROPEFFECT_MOVE)    
	{
	    CompleteMove();    
		//SendMessage(GetActiveWindow(), RMSG_DROPGRIDFROMPOPUPLIST, (WPARAM) 0L, (LPARAM) Data);
	}

    LPARAM lparam;

    lparam=MousePos->y; 
    lparam=lparam<<16;
    lparam &= MousePos->x;                         
    
    SendMessage(GetActiveWindow(), WM_LBUTTONUP,0,lparam);        

	GlobalUnlock(hgData);
	
    Empty();
	return dropEffect;
}
