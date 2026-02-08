// BaseDialogContainer.cpp: implementation of the CBaseDialogContainer class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "BaseDialogContainer.h"
#include "BaseDialog.h"


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Modifier		: 
// Comments		: Construction
//-----------------------------------------------------------------------------
CBaseDialogContainer::CBaseDialogContainer()
{

}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Modifier		: 
// Comments		: Destruction
//-----------------------------------------------------------------------------
CBaseDialogContainer::~CBaseDialogContainer()
{
	RemoveAll();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Return void : 
// Param  LPCTSTR pcszIndicatorName : Key로 사용할 지표명
// Comments		: 지표설정 다이얼로그를 관리할 Map에 추가한다.
//-----------------------------------------------------------------------------
void CBaseDialogContainer::AddBaseDialog( LPCTSTR pcszIndicatorName, CBaseDialog *pChildDialog)
{
	m_mapBaseDialog.SetAt( pcszIndicatorName, pChildDialog);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Param  LPCTSTR pcszIndicatorName : 가져올 지표명(Key값) 
// Comments		: 지표설정 다이얼로그를 찾아 리턴한다.
//-----------------------------------------------------------------------------
CBaseDialog *CBaseDialogContainer::GetBaseDialog( LPCTSTR pcszIndicatorName )
{
	if(pcszIndicatorName == "")
		return NULL;

	CBaseDialog *pBaseDialog = NULL;
	m_mapBaseDialog.Lookup( pcszIndicatorName, pBaseDialog);
	return pBaseDialog;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Return void : 
// Comments		: 지표설정Dlg관리 Map의 내용을 전부 삭제한다. 
//-----------------------------------------------------------------------------
void CBaseDialogContainer::RemoveAll()
{
	POSITION pos = m_mapBaseDialog.GetStartPosition();

	while( pos )
	{
		CBaseDialog * pBaseDlg = NULL;
		CString strIndicatorName;
		m_mapBaseDialog.GetNextAssoc( pos, strIndicatorName, pBaseDlg);
		if( pBaseDlg) delete pBaseDlg;
	}

	m_mapBaseDialog.RemoveAll();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Return POSITION : 
// Comments		: Map의 Start Position을 가져온다.
//-----------------------------------------------------------------------------
POSITION CBaseDialogContainer::GetStartPosition() const
{
	return m_mapBaseDialog.GetStartPosition();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Param  POSITION &pos : 
// Comments		: Map의 Item들을 POSITION별로 가져온다.
//-----------------------------------------------------------------------------
CBaseDialog *CBaseDialogContainer::GetNextAssoc( POSITION &pos )
{
	CString strKey = _T("");
	CBaseDialog *pBaseDialog = NULL;
	
	m_mapBaseDialog.GetNextAssoc( pos, strKey, pBaseDialog);

	return pBaseDialog;
}