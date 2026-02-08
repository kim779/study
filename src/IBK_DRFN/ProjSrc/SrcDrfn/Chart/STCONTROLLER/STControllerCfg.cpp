// STControllerCfg.cpp: implementation of the CSTControllerCfg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "STControllerCfg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "MemManager.h"
#include "../Common_ST/DataItem.h"
#include "../Common_ST/STControllerDef.h"
#include "ApQurier.h"

const UINT RMSG_STCONTROLLER_POINTER = ::RegisterWindowMessage("RMSG_STCONTROLLER_POINTER");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSTControllerCfg::CSTControllerCfg(CSTControllerApp *pTheApp)
:CSTController(pTheApp)
{
}

CSTControllerCfg::~CSTControllerCfg()
{
}


short CSTControllerCfg::Execute(LPCTSTR lpInputData) 
{
	InitMemManager();
	m_lCntOfAllApData = 1;
	m_pAPQurier->RequestDataToServer(NULL,this,&m_pAPDataItem);	
	m_pAPDataItem->SetDataItemSize(m_lCntOfAllApData);
	RegisterItemIntoLogicMaker();
	::SendMessage(m_hParent,UMSG_EXECUTEVIRTUALLY,0,0);
	if(GetError())	return 0;
	::SendMessage(m_hReceiver,RMSG_STCONTROLLER_POINTER,m_lKey,(LPARAM)(CISTControllerD *)this);	
	return 4;
}

void CSTControllerCfg::SetConfig()
{
//	m_STConfigureDlgLoader.ShowConfigureDlg(m_hReceiver,this,NULL,0);
}

void CSTControllerCfg::SetConfigOnWizard(CRect* pRect)
{
//	m_STConfigureDlgLoader.ShowConfigureDlgOnWizard(m_hReceiver,this,NULL,0, pRect);
}

void CSTControllerCfg::WriteConfig()
{
//	m_STConfigureDlgLoader.WriteConfigure(m_hReceiver,this,NULL,0,1);
}

void CSTControllerCfg::DeleteConfig()
{
//	m_STConfigureDlgLoader.WriteConfigure(m_hReceiver,this,NULL,0,0);
}

BOOL CSTControllerCfg::OnReceiveQuoteData()
{	
	 return FALSE;
}
