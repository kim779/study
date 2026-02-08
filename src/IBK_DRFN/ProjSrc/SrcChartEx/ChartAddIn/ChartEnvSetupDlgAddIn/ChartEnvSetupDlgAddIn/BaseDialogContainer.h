// BaseDialogContainer.h: interface for the CBaseDialogContainer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEDIALOGCONTAINER_H__39D69BA2_9C75_4C1F_BEE0_52CA1256E7A1__INCLUDED_)
#define AFX_BASEDIALOGCONTAINER_H__39D69BA2_9C75_4C1F_BEE0_52CA1256E7A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <AFXTEMPL.h>


class CBaseDialog;


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Comments		: 
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
class CBaseDialogContainer  
{
public:
	CBaseDialogContainer();
	virtual ~CBaseDialogContainer();
	
	void AddBaseDialog( LPCTSTR pcszIndicatorName, CBaseDialog *pChildDialog);
	POSITION GetStartPosition() const;
	CBaseDialog *GetBaseDialog( LPCTSTR strIndicatorName);
	CBaseDialog *GetNextAssoc( POSITION &pos);
	void RemoveAll();

	
private:
	CTypedPtrMap < CMapStringToOb, CString, CBaseDialog *> m_mapBaseDialog;
};

#endif // !defined(AFX_BASEDIALOGCONTAINER_H__39D69BA2_9C75_4C1F_BEE0_52CA1256E7A1__INCLUDED_)
