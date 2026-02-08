// STActor.cpp : implementation file
//
#include <afx.h>
#include "stdafx.h"
#include "../Include_ST/STActor.h"
#include "../Include_ST/STControllerLoader.h"
#include "../Common_ST/STConfigDef.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSTActor

extern CString Parser(LPCTSTR lpInputData, int *pNShifter );
CSTActor::CSTActor()
{
	m_fnSTMain = NULL;
	m_pSTControllerLoader = NULL;
	m_pSTController = NULL;
	m_hReceiver = NULL;
	m_lpFilePath = NULL;
	m_lKey = 0;
	m_lExpiredDate = 0;
	m_lSourdeType = -1;
	m_bUseUserID = FALSE;
	m_bUsePassword = FALSE;
	m_bUseExpiredDate = FALSE;
	m_nSTPos = -1;
}

CSTActor::~CSTActor()
{
	if(m_pSTControllerLoader)
	{
		delete m_pSTControllerLoader;
		m_pSTControllerLoader = NULL;
	}	
}

BEGIN_MESSAGE_MAP(CSTActor, CWnd)
	//{{AFX_MSG_MAP(CSTActor)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_NCDESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTActor message handlers

int CSTActor::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_pSTControllerLoader)
		m_pSTControllerLoader = new CSTControllerLoader;		

	if(!m_pSTController)
	{
		int nShifter = 0;
		int nPos = 0;
		CString strDataDivided;
		CString strTitle;
		CString strContents;

		CString strMode;
		while(1)
		{
			strDataDivided = Parser(m_strAdditionalOption, &nShifter);
			if(strDataDivided.IsEmpty())
				break;

			nPos = strDataDivided.Find('=');
			if(nPos!=-1)
			{
				strTitle = strDataDivided.Left(nPos);
				strContents = strDataDivided.Right(strDataDivided.GetLength() - nPos - 1);
			}
			else
				continue;

			if(strTitle == LOAD_MODE)
				strMode = strContents;
		}	
		
		if(strMode == MODE_ACTION)
			m_pSTController = m_pSTControllerLoader->GetSTController();
		else
			m_pSTController = m_pSTControllerLoader->GetSTControllerCfg();
	}	

	m_pSTController->SetUserID(m_strUserID);
	m_pSTController->SetExpiredDate(m_lExpiredDate);
	m_pSTController->SetPassword(m_strPassword);
	m_pSTController->SetSourceType(m_lSourdeType);
	m_pSTController->SetUseUserID(m_bUseUserID);
	m_pSTController->SetUsePassword(m_bUsePassword);
	m_pSTController->SetUseExpiredDate(m_bUseExpiredDate);
	m_pSTController->SetPath(m_lpFilePath);
	m_pSTController->SetSTName(m_strSTName, m_nSTPos);
	m_pSTController->SetReceiver(m_hReceiver,m_lKey);
	m_pSTController->SetAdditonalOption(m_strAdditionalOption);
	m_pSTController->SetpStringArrayInputs(&m_stringArrayInput);
	m_pSTController->SetChartItem(m_pChartItem);

	return m_pSTController->CreateApQurier(this);
}

void CSTActor::OnDestroy() 
{	
	if(m_pSTController)
	{
		m_pSTController->Destory();
		delete m_pSTController;
		m_pSTController = NULL;
	}
	CWnd::OnDestroy();
}

void CSTActor::SetReceiver(HWND hReceiver, long lKey)
{
	m_hReceiver = hReceiver;
	m_lKey = lKey;	
}

long CSTActor::Execute(LPCTSTR lpInputData) 
{
	return m_pSTController->Execute(lpInputData);
}

long CSTActor::Stop()
{
	return m_pSTController->Stop();
}

long CSTActor::Pause()
{
	return m_pSTController->Pause();
}

long CSTActor::Update(long lPos, BOOL bIsUpdateData)
{
	return m_pSTController->Update(lPos,bIsUpdateData);
}

LRESULT CSTActor::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)	
	{
	case UMSG_ENDRECEIVINGFROMSERVER:
		{
			BOOL bRetValue = FALSE;
			long lCnt = m_pSTController->GetCntOfAllApData(); //KHD: AP를 사용하지 않는다.
#ifndef _DEBUG
			__try
			{
#endif
				ASSERT( 0 < lCnt);
				for( long lPos = 0; lPos < lCnt; lPos++)
				{
					bRetValue = m_pSTController->SetCurrentPos(lPos);
					if(bRetValue)
						m_fnSTMain(this);
				}
				m_pSTController->PrepareGraph();
				m_pSTController->DrawGraph(FALSE);
#ifndef _DEBUG
			}__except(EXCEPTION_EXECUTE_HANDLER){
				LPCTSTR lpError = m_pSTController->GetError();
				if(lpError)
					MessageBox(lpError);
				else
					MessageBox("사용중에 에러가 발생하였습니다(1).");
			}
#endif
		}
		break;

	case UMSG_ENDREALTIMEDATAFROMSERVER:
		{
			BOOL bRetValue = FALSE;
			bRetValue = m_pSTController->SetCurrentPos(wParam);
			m_pSTController->ResetLastSignal_ForReal(wParam);
#ifdef _DEBUG
			if(bRetValue)
				m_fnSTMain(this);
			
			m_pSTController->DrawGraph(TRUE);
#else
//			__try
//			{
				if(bRetValue)
					m_fnSTMain(this);

//			}__except(EXCEPTION_EXECUTE_HANDLER){				
//				LPCTSTR lpError = m_pSTController->GetError();
//				if(lpError)
//					MessageBox(lpError);
//				else
//					MessageBox("사용중에 에러가 발생하였습니다(2).");	
//				return 0L;
//			}
			m_pSTController->DrawGraph(TRUE);
#endif		
		}
		break;
	case UMSG_EXECUTEVIRTUALLY:
		{
			BOOL bRetValue = FALSE;
			bRetValue = m_pSTController->SetCurrentPos(wParam);
#ifdef _DEBUG
			if(bRetValue)
				m_fnSTMain(this);
#else
			__try
			{
				if(bRetValue)
					m_fnSTMain(this);
				
			}__except(EXCEPTION_EXECUTE_HANDLER){				
				LPCTSTR lpError = m_pSTController->GetError();
				if(lpError)
					MessageBox(lpError);
				else
					MessageBox("사용중에 에러가 발생하였습니다(3).");	
			}
#endif
		}
		break;
	default:
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

void CSTActor::SubIn(long lIndex) 
{
	m_pSTController->SubIn(lIndex);
}

void CSTActor::SubOut() 
{
	m_pSTController->SubOut();
}

void CSTActor::IfIn() 
{
	m_pSTController->IfIn();
}

void CSTActor::IfOut() 
{
	m_pSTController->IfOut();
}

void CSTActor::BracketIn() 
{
	m_pSTController->BracketIn();
}

void CSTActor::BracketOut() 
{
	m_pSTController->BracketOut();
}

void CSTActor::ForIn() 
{
	m_pSTController->ForIn();
}

void CSTActor::ForOut() 
{
	m_pSTController->ForOut();
}

BOOL CSTActor::SetInputs(long lIndex, LPCTSTR lpName, int nType, double dInitValue) 
{
	return m_pSTController->SetInputs(lIndex,lpName,nType,dInitValue);
}

BOOL CSTActor::SetVariables(long lHolderIndex, LPCTSTR lpName, int nType, double dInitValue) 
{
	return m_pSTController->SetVariables(lHolderIndex, lpName, nType, dInitValue);
}

BOOL CSTActor::SetArray(long lHolderIndex, LPCTSTR lpName, int nType, double dInitValue, long lArrayCnt)
{
	return m_pSTController->SetArray(lHolderIndex, lpName, nType, dInitValue, lArrayCnt);
}

BOOL CSTActor::SetSubInputsFrom(long lIndex, double dValue) 
{
	return m_pSTController->SetSubInputsFrom(lIndex, dValue);
}

BOOL CSTActor::IfCondition(BOOL bIfConditon) 
{
	return m_pSTController->IfCondition(bIfConditon);
}

double CSTActor::GetRawData(long lDataIndex, double dReverseIndex)
{
	return m_pSTController->GetRawData(lDataIndex, dReverseIndex);
}

CString CSTActor::GetRawSData(long lDataIndex, double dReverseIndex)
{
	return m_pSTController->GetRawSData(lDataIndex, dReverseIndex);
}

double CSTActor::InnerFunc(long lType, long lIndexFunc, long lIndex,
					  CComVariant varArg1, 
					  CComVariant varArg2, 
					  CComVariant varArg3, 
					  CComVariant varArg4, 
					  CComVariant varArg5, 
					  CComVariant varArg6, 
					  CComVariant varArg7, 
					  CComVariant varArg8, 
					  CComVariant varArg9, 
					  CComVariant varAgr10)
{
	return m_pSTController->InnerFunc(lType, lIndexFunc, lIndex,
					varArg1, varArg2, varArg3, varArg4, varArg5,
					varArg6, varArg7, varArg8, varArg9, varAgr10);
}

double CSTActor::GetVarHolderData(long lHolderIndex, double dIndex)
{
	return m_pSTController->GetVarHolderData(lHolderIndex, dIndex);
}

double CSTActor::GetInputHolderData(long lHolderIndex, double dIndex)
{
	return m_pSTController->GetInputHolderData(lHolderIndex, dIndex);
}

double CSTActor::GetFuncData(double dIndex)
{
	return m_pSTController->GetFuncData(dIndex);
}

void CSTActor::SetVarHolderData(long lHolderIndex, double dIndex, double dValue)
{
	m_pSTController->SetVarHolderData(lHolderIndex, dIndex, dValue);
}

void CSTActor::SetInputHolderData(long lHolderIndex, double dIndex, double dValue)
{
	m_pSTController->SetInputHolderData(lHolderIndex, dIndex, dValue);
}

void CSTActor::SetFuncData(double dIndex, double dValue)
{
	m_pSTController->SetFuncData(dIndex, dValue);
}

double CSTActor::SubFunc( long lSTActor, long lIndexFunc, long lIndex, 
						 BOOL bTemp0 /* = FALSE */,
						 BOOL bTemp1 /* = FALSE */,
						 BOOL bTemp2 /* = FALSE */,  
						 BOOL bTemp3 /* = FALSE */, 
						 BOOL bTemp4 /* = FALSE */,
						 BOOL bTemp5 /* = FALSE */, 
						 BOOL bTemp6 /* = FALSE */,  
						 BOOL bTemp7 /* = FALSE */, 
						 BOOL bTemp8 /* = FALSE */,  
						 BOOL bTemp9 /* = FALSE */)
{
	return m_pSTController->SubFunc( lSTActor, lIndexFunc, lIndex, 
							bTemp0, bTemp1, bTemp2, bTemp3, bTemp4,
							bTemp5, bTemp6, bTemp7, bTemp8,  bTemp9);
}

BOOL CSTActor::DeclareSubFunc(long lHolderIndex,LPCTSTR lpFuncName)
{
	return m_pSTController->DeclareSubFunc(lHolderIndex, lpFuncName);
}

BOOL CSTActor::DeclareInnerFunc(long lType, long lHolderIndex, LPCTSTR lpFuncName)
{
	return m_pSTController->DeclareInnerFunc(lType, lHolderIndex, lpFuncName);
}

void CSTActor::SetArrayHolderData(long lHolderIndex, double dIndex, double dValue)
{
	m_pSTController->SetArrayHolderData(lHolderIndex, dIndex, dValue);
}

double CSTActor::GetArrayHolderData(long lHolderIndex, double dIndex)
{
	return m_pSTController->GetArrayHolderData(lHolderIndex, dIndex);
}

long CSTActor::Command(LPCTSTR lpCommand, LPCTSTR lpOption)
{
	return m_pSTController->Command(lpCommand, lpOption);
}

void CSTActor::OnNcDestroy() 
{
	CWnd::OnNcDestroy();
}
