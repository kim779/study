// $Workfile:   PCTreeCtrl.h  $$Revision:   1.0  $$Date:   02 MAY 2002 15:33:28  $
/*
*************************************************************************************************************
*																										
*	Module Name:	PCTreeCtrl.h
*	
*	Func Group:		
*	
*	Description:	MFC Tree Control when, Tooltip is checked for the Tree Ctrl in 
*					rsrc editor, Tooltips are created only for those TreeItems that exceed
*					the Text size that does'nt fit in the visible client area of the Tree ctrl.
*					So I had to show the tooltips even when the window was shrunk (MFC Tree ctrl shall
*					display tooltips that were initially bigger to fit into the visible client area).Also
*					this Tooltip displays strings of any length.
*					
*					
*	History:		Programmer		Date		Comment
*					-----------------------------------------------
*					Megavarnan  	05/02/02	Created module.
*					
*
*	Copyright:		(C) 2002-03 by Megavarnan Selvaraj
*	 
**************************************************************************************************************
*/ 

// PCTreeCtrl.h : Declaration file
//

#ifndef _PCTREECTRL_H_
#define _PCTREECTRL_H_

#include "stdafx.h"

class CPCTreeCtrl: public CTreeCtrl
{
	
public:
	CPCTreeCtrl();
	virtual ~CPCTreeCtrl();

private:
	TCHAR* m_pchTip ;
	WCHAR* m_pwchTip;

protected:
	//{{AFX_MSG(CPCTreeCtrl)
	
public:
	virtual void PreSubclassWindow();
	virtual int OnToolHitTest(CPoint point, TOOLINFO * pTI) const;
	afx_msg BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
};
#endif // ifdef _PCTREECTRL_H_

