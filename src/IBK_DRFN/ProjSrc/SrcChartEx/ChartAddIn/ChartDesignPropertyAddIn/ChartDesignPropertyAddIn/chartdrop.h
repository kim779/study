//ChartDrop.h

#ifndef __ChartDrop_H__
#define __ChartDrop_H__

#include "message.h"

typedef struct
{
	HTREEITEM	hItem;
	char		ChartName[64];
} ChartItem;

//Drag&Drop
class CChartDrop : public COleDropTarget
{
public:
    CChartDrop();

public:
    virtual DROPEFFECT OnDragEnter(CWnd* pWnd,COleDataObject* pObj,
		DWORD dwKeyState, CPoint point);
    
	virtual DROPEFFECT OnDragOver(CWnd* pWnd,COleDataObject* pObj,
		DWORD dwKeyState,CPoint point);

    virtual BOOL OnDrop( CWnd* pWnd,COleDataObject* pObj,
		DROPEFFECT de,CPoint point );

public:
    void SetParent( CWnd* pWnd );
	void SetName(LPSTR Name);

private:
	CString		m_Name;
    CWnd*		m_pWnd;
    BOOL		m_bIsDragging;
};
#endif
