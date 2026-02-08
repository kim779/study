// ToolOrderManagerAddInImp.cpp: implementation of the CToolOrderManagerAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "ToolOrderManagerAddInImp.h"

#include "../Include_Chart/BlockBaseEnum.h"
#include "../Include_AddIn/AddInCommand.h"					// for EAI_DRDS_PACKET_END
#include "../Include_AddIn/I000000/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn/I000000/_IChartOCX.h"			// for IChartOCX
#include "../Include_AddIn/I000000/_IAddInToolMgr.h"		// for IAddInToolMgr

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToolOrderManagerAddInImp::CToolOrderManagerAddInImp(IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	if( m_lstToolMessageInfo.GetCount() > 0)
	{
		POSITION pos = m_lstToolMessageInfo.GetHeadPosition();
	
		TOOL_MESSAGE_INFO *pToolMessageInfo;
		while(pos)
		{
			pToolMessageInfo = NULL;
			pToolMessageInfo = m_lstToolMessageInfo.GetAt(pos);

			if( pToolMessageInfo == NULL )	break;
			delete pToolMessageInfo;
			m_lstToolMessageInfo.GetNext(pos);
		}

		m_lstToolMessageInfo.RemoveAll();
	}
}


CToolOrderManagerAddInImp::~CToolOrderManagerAddInImp()
{
	if( m_lstToolMessageInfo.GetCount() > 0)
	{
		POSITION pos = m_lstToolMessageInfo.GetHeadPosition();
	
		TOOL_MESSAGE_INFO *pToolMessageInfo;
		while(pos)
		{
			pToolMessageInfo = NULL;
			pToolMessageInfo = m_lstToolMessageInfo.GetAt(pos);

			if( pToolMessageInfo == NULL )	break;
			delete pToolMessageInfo;
			m_lstToolMessageInfo.GetNext(pos);
		}

		m_lstToolMessageInfo.RemoveAll();
	}
}


// ( 2006 / 11 / 17 - Sang-Woo, Cho ) - Multiple Item in DLL
CString CToolOrderManagerAddInImp::m_strAddInItemName = "TOOL_ORDER_MANAGER";


//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CToolOrderManagerAddInImp)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_char_long)
	ONADDINEVENT( OnResetMainBlock )
	ONADDINEVENT( OnToolCommand )
END_ADDIN_EVENT_MAP()


//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 17
// Return BOOL	: 
// Param  	const EAICommandType p_eCommandID : Tool의 동작을 나타내는 값
// Param  const char *p_szParam1 : Tool AddIn 에서 보내준 구분 문자값
// Param  const long p_lParam	 : Tool AddIn 내부에서의 구분값
// Comments		: Tool AddIn에서 Tool을 생성시키거나 소멸시킬때 순서를 기억하기
//				: 위해 각각의 Tool AddIn에서 보내주는 값을 List로 가지고 있는다.
//-----------------------------------------------------------------------------
BOOL CToolOrderManagerAddInImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)
{
#ifdef _DEBUG
	TRACE("ChartCommonAddIn.dll:ToolOrderManager %s\r\n", p_szParam );
#endif

	TOOL_MESSAGE_INFO *pToolMessageInfo = NULL;

	// 1. Tool AddIn에서 새로운 Tool을 만들었을때 List의 마지막에 등록해준다.
	if( EAI_TOOLORDERMGR_ADD_TOOL_TO_ORDERMANAGER == p_eCommandID )
	{
		pToolMessageInfo = new TOOL_MESSAGE_INFO;
		
		pToolMessageInfo->strToolName = p_szParam;
		pToolMessageInfo->lToolValue = p_lParam;
		
		m_lstToolMessageInfo.AddTail(pToolMessageInfo);
		
		return TRUE;
	}

	// 2. Tool AddIn에서 기존의 Tool을 삭제할때 List에서 해당되는 항목을 삭제한다.
	else if( EAI_TOOLORDERMGR_DELETE_TOOL_FROM_ORDERMANAGER == p_eCommandID )
	{
		if( m_lstToolMessageInfo.GetCount() <= 0 )
			return FALSE;

		POSITION pos = m_lstToolMessageInfo.GetHeadPosition();
	
		while(pos)
		{
			pToolMessageInfo = NULL;
			pToolMessageInfo = m_lstToolMessageInfo.GetAt(pos);

			if( pToolMessageInfo == NULL )	break;

			if( pToolMessageInfo->strToolName == p_szParam && pToolMessageInfo->lToolValue == p_lParam )
			{
				delete pToolMessageInfo;
				m_lstToolMessageInfo.RemoveAt(pos);
				break;
			}

			m_lstToolMessageInfo.GetNext(pos);
		}

		return TRUE;
	}
	
	return FALSE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 17
// Return void	: 
// Param  void	: 
// Comments		: 화면을 초기화(T_DELETE_ALL) 하는 Message가 들어오면 Tool들의 
//				: 생성 정보를 저장하고 있던 list를 초기화한다.
//-----------------------------------------------------------------------------
void CToolOrderManagerAddInImp::OnResetMainBlock( void )
{
	if( m_lstToolMessageInfo.GetCount() > 0 )
	{
		POSITION pos = m_lstToolMessageInfo.GetHeadPosition();

		TOOL_MESSAGE_INFO *pToolMessageInfo;
		while(pos)
		{
			pToolMessageInfo = NULL;
			pToolMessageInfo = m_lstToolMessageInfo.GetAt(pos);

			if( pToolMessageInfo == NULL )	break;
			delete pToolMessageInfo;
			m_lstToolMessageInfo.GetNext(pos);
		}

		m_lstToolMessageInfo.RemoveAll();
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 17
// Return void	: 
// Param  const int p_nToolType : 
// Param  const int p_nCmdOption : 
// Comments		: 
//-----------------------------------------------------------------------------
void CToolOrderManagerAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption )
{
	CToolOptionInfo::TOOLOPTION eToolOption = (CToolOptionInfo::TOOLOPTION)p_nToolType;

	// 1. 모든 Tool을 삭제하라는 Message를 받으면 List를 초기화한다.
	if( eToolOption == CToolOptionInfo::T_DELETE_ALL )
	{
		if( m_lstToolMessageInfo.GetCount() > 0 )
		{
			POSITION pos = m_lstToolMessageInfo.GetHeadPosition();

			TOOL_MESSAGE_INFO *pToolMessageInfo;
			while(pos)
			{
				pToolMessageInfo = NULL;
				pToolMessageInfo = m_lstToolMessageInfo.GetAt(pos);

				if( pToolMessageInfo == NULL )	break;
				delete pToolMessageInfo;
				m_lstToolMessageInfo.GetNext(pos);
			}

			m_lstToolMessageInfo.RemoveAll();
		}
	}

	// 2. 이전 Tool을 삭제하라는 Message를 받으면 List에서 마지막 항목을 삭제하고 m_pIAddInToolMgr에게
	// 삭제할 항목의 정보를 전송한다.
	else if( eToolOption == CToolOptionInfo::T_DELETE_INORDER )
	{
		if( m_lstToolMessageInfo.GetCount() > 0 )
		{
			TOOL_MESSAGE_INFO *pToolMessageInfo = NULL;
			pToolMessageInfo = m_lstToolMessageInfo.GetTail();

			if( m_pIAddInToolMgr && pToolMessageInfo != NULL )
			{
				m_pIAddInToolMgr->OnAddInToolCommand( EAI_TOOLORDERMGR_DELETE_THIS_TOOL,	\
													  pToolMessageInfo->strToolName,		\
													  pToolMessageInfo->lToolValue );
			
			
				delete pToolMessageInfo;
				m_lstToolMessageInfo.RemoveTail();
			}
		}
	}
}