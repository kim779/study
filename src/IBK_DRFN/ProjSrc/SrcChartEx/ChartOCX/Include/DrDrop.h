//DrDrop.h

#ifndef __DrDrop_H__
#define __DrDrop_H__

#include "message.h"

typedef struct
{
	char	OutputString[64];
	char	SourceName[3];
	char	DataName[3];
	char	FieldName[16];
} FSItem;

class CDrDrop : public COleDropTarget
{
public:
    CDrDrop();

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
