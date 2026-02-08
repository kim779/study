#ifndef __DROPTARGET
#define __DROPTARGET
#include "afxole.h"   

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// COleDropWndTarget window

class CDropTarget : protected COleDropTarget
{
// Construction
public:
    CDropTarget();


// Implementation
public:
	BOOL Initialize(CWnd* wnd);
    virtual ~CDropTarget();   
    

   virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD 
                                                dwKeyState, CPoint point );
   virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD 
                                               dwKeyState, CPoint point );
   virtual void OnDragLeave(CWnd* pWnd);
   
   virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT 
                                          dropEffect, CPoint point );
};
#endif