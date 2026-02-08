// STOperatorApp.cpp : implementation file
//

#include "stdafx.h"
#include "../Include_ST/STOperatorApp.h"
#include "../Include_ST/STActor.h"
#include "../Common_ST/STControllerDef.h"
#include "../Common_ST/DllSTLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTOperatorApp

CSTOperatorApp::CSTOperatorApp()
{
}

long CSTOperatorApp::LoadST(void (*fnSTMain)(CSTActor* pSTActor)
								, HWND hParent  //KHD: 성과분석에서 전략메니져 윈도우 핸들
								, HWND hSocket
								, CDBMgr* pChartItem
								, long lKey
								, LPCTSTR lpSTName
								, int nSTPos
								, LPCTSTR lpAdditionalOption
								, char *szInputs[]
								, LPCTSTR lpUserID
								, LPCTSTR lpPassword
								, long lExpiredDate
								, long lSourceType
								, BOOL bUseUserID	
								, BOOL bUsePassword	
								, BOOL bUseExpiredDate	)
{
	//전략을 초기화 한다.
	
	CSTActor* pSTActor = new CSTActor;//Controller 생성 
	CString strFilePath = this->m_pszHelpFilePath;
	int nLength = strFilePath.GetLength();
	int nPos = strFilePath.ReverseFind('.');
	strFilePath.Delete(nPos,nLength-nPos);
	strFilePath += ".SD";	
	pSTActor->m_lpFilePath = strFilePath;
	//pSTActor->SetReceiver(hParent,hSocket, pDBMgr,lKey);
	pSTActor->SetReceiver(hParent,lKey);
	pSTActor->m_fnSTMain = fnSTMain;
	pSTActor->m_strSTName = lpSTName;
	pSTActor->m_nSTPos = nSTPos;
	pSTActor->m_strUserID = lpUserID;
	pSTActor->m_strPassword = lpPassword;
	pSTActor->m_lExpiredDate = lExpiredDate;
	pSTActor->m_lSourdeType = lSourceType;
	pSTActor->m_bUseUserID = bUseUserID;
	pSTActor->m_bUsePassword = bUsePassword;
	pSTActor->m_bUseExpiredDate = bUseExpiredDate;
	pSTActor->m_strAdditionalOption = lpAdditionalOption;
	pSTActor->m_pChartItem = (long)pChartItem;//KHD :2006.08.

	pSTActor->m_stringArrayInput.RemoveAll();
	CString strTemp;
	int nPosStr = -1;
	for(nPos=0;nPos<MAXCNTOFINPUTHOLDER;nPos++)
	{
		strTemp = szInputs[nPos];
		if(strTemp=="#")	break;
		nPosStr = strTemp.Find('(',0);
		if(nPosStr>=0)
		{
			strTemp.Delete(0,nPosStr+1);
			strTemp.TrimRight(')');
		}
		pSTActor->m_stringArrayInput.Add(strTemp);
	}
	//OnCreate 호출
	BOOL bRetValue = pSTActor->Create(NULL,"STActor",WS_CHILD,CRect(0,0,0,0),CWnd::FromHandle(hParent),0x2000);
	if(!bRetValue)
	{
		delete pSTActor;
		return NULL;
	}
	return (long)pSTActor;
}

long CSTOperatorApp::ExecuteST(long lSTActor,LPCTSTR lpInputData)
{
	if(lSTActor)
	{
		return ((CSTActor*)lSTActor)->Execute(lpInputData);		
	}
	return 0;
}

long CSTOperatorApp::UnloadST(long lSTActor)
{
	CSTActor* pSTActor = (CSTActor*)lSTActor;
	pSTActor->DestroyWindow();
	delete pSTActor;	
	pSTActor = NULL;
	return NULL;
}

long CSTOperatorApp::StopST(long lSTActor)
{
	if(lSTActor)
	{
		return ((CSTActor*)lSTActor)->Stop();	
	}
	return 0;
}

long CSTOperatorApp::PauseST(long lSTActor)
{
	if(lSTActor)
	{
		return ((CSTActor*)lSTActor)->Pause();	
	}
	return 0;
}

long CSTOperatorApp::UpdateST(long lSTActor,long lPos, BOOL bIsUpdateData)
{
	if(lSTActor)
	{
		return ((CSTActor*)lSTActor)->Update(lPos,bIsUpdateData);	
	}
	return 0;
}

long CSTOperatorApp::CommandST(long lSTActor, LPCTSTR lpCommand, LPCTSTR lpOption)
{
	if(lSTActor)
	{
		return ((CSTActor*)lSTActor)->Command(lpCommand,lpOption);	
	}
	return 0;
}

BEGIN_MESSAGE_MAP(CSTOperatorApp, CWinApp)
	//{{AFX_MSG_MAP(CSTOperatorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSTOperatorApp message handlers




int CSTOperatorApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWinApp::ExitInstance();
}

BOOL CSTOperatorApp::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	return CWinApp::InitInstance();
}
