// SystemTradingDoc.cpp : implementation of the CSystemTradingDoc class
//

#include "stdafx.h"
#include "SystemTrading.h"
#include "MainFrm.h"

#include "SystemTradingDoc.h"
#include "SystemTradingView.h"
#include "../Include_ST/DeclareDefine.h"
#include "../include_ST/StructDefine.h"
#include "../include_ST/MessageDefine.h"

#include "DlgPassword.h"
#include "LoadHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingDoc

IMPLEMENT_DYNCREATE(CSystemTradingDoc, CDocument)

BEGIN_MESSAGE_MAP(CSystemTradingDoc, CDocument)
	//{{AFX_MSG_MAP(CSystemTradingDoc)
	ON_COMMAND(ID_BUILD_COMPILE, OnBuildCompile)
	ON_COMMAND(ID_BUILD_SINGLE_COMPILE, OnBuildSingleCompile)
	ON_UPDATE_COMMAND_UI(ID_BUILD_COMPILE, OnUpdateBuildCompile)
	ON_COMMAND(ID_VIEW_PROPERTY, OnViewProperty)
	ON_COMMAND(ID_BUILD_COMPILE_DLL, OnBuildCompileDll)
	ON_UPDATE_COMMAND_UI(ID_BUILD_COMPILE_DLL, OnUpdateBuildCompileDll)
	ON_COMMAND(ID_BUILD_SINGLE_COMPILE_DLL, OnBuildSingleCompileDll)
	ON_UPDATE_COMMAND_UI(ID_BUILD_SINGLE_COMPILE, OnUpdateBuildSingleCompile)
	ON_UPDATE_COMMAND_UI(ID_BUILD_SINGLE_COMPILE_DLL, OnUpdateBuildSingleCompileDll)
	ON_COMMAND(ID_BUILD_STOP, OnBuildStop)
	ON_UPDATE_COMMAND_UI(ID_BUILD_STOP, OnUpdateBuildStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingDoc construction/destruction

#pragma warning(disable:4355)
CSystemTradingDoc::CSystemTradingDoc() : m_xTextBuffer(this),
m_pScriptSec(NULL)
{
	m_nFileType = 0;
	m_nScriptType = 0;

	::memset(m_szScriptName, 0x00, sizeof(m_szScriptName));
}

CSystemTradingDoc::~CSystemTradingDoc()
{
	char szBackupFile[MAX_PATH] = {NULL, };
	::wsprintf(szBackupFile, _T("%s.bak"), m_strPathName);

	::SetFileAttributes(m_strPathName, FILE_ATTRIBUTE_NORMAL);
	if(::CopyFile(szBackupFile, m_strPathName, FALSE))
		::DeleteFile(szBackupFile);

	if(m_pScriptSec)
	{
		delete m_pScriptSec;
		m_pScriptSec = NULL;
	}
}

BOOL CSystemTradingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSystemTradingDoc serialization

void CSystemTradingDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingDoc diagnostics

#ifdef _DEBUG
void CSystemTradingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSystemTradingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingDoc commands

void CSystemTradingDoc::SetScriptType(BOOL bRedraw)
{
	char szConfig[MAX_PATH] = {NULL, };
	::GetSTFile(szConfig, m_szScriptName, theApp.GetRootPath());

	m_nScriptType = ::GetPrivateProfileInt(_T("Basic"), _T("LangType"), m_nScriptType, szConfig);

	SetSystemTradingFontLog();

	POSITION pos = GetFirstViewPosition();
	CSystemTradingView* pView = (CSystemTradingView*)GetNextView(pos);

	pView->ChangeScriptType(m_nScriptType, bRedraw);
}

BOOL CSystemTradingDoc::SetScriptProperty(BOOL bRedraw, BOOL bUsePassword, CString strPassword, BOOL bUseUserID, CString strUserID, BOOL bUseExpires, CString strExpires)
{
	SetScriptType(bRedraw);

	BOOL bModified = m_bModified;
	if(m_pScriptSec)
	{
		if(!bModified)
			bModified = m_pScriptSec->GetUsePassword() != bUsePassword;
		m_pScriptSec->SetUsePassword(bUsePassword);

		if(!bModified)
			bModified = m_pScriptSec->GetPassword().Compare(strPassword);
		m_pScriptSec->SetPassword(strPassword);
		
		if(!bModified)
			bModified = m_pScriptSec->GetUseUserID() != bUseUserID;
		m_pScriptSec->SetUseUserID(bUseUserID);

		if(!bModified)
			bModified = m_pScriptSec->GetUserID().Compare(strUserID);
		m_pScriptSec->SetUserID(strUserID);

		if(!bModified)
			bModified = m_pScriptSec->GetUseExpires() != bUseExpires;
		m_pScriptSec->SetUseExpires(bUseExpires);

		if(!bModified)
			bModified = m_pScriptSec->GetExpires().Compare(strExpires);
		m_pScriptSec->SetExpires(strExpires);

		char szSaveFile[MAX_PATH] = {NULL, };
		::lstrcpy(szSaveFile, GetParseFileName());
		
		ProcessSaveFile(szSaveFile);
	}

	if(bModified)
	{
		((CMainFrame*)theApp.m_pMainWnd)->ChangeProperty(FALSE, m_szScriptName, m_nFileType);
		((CMainFrame*)theApp.m_pMainWnd)->ChangeProperty(TRUE, m_szScriptName, m_nFileType);
	}

	return bModified;
}

void CSystemTradingDoc::SetSystemTradingFontLog()
{
	char szSection[20];

	if(m_nScriptType == ST_YESTRADER)
		::lstrcpy(szSection, _T("YESTRADER"));
	else
		::lstrcpy(szSection, _T("TRADESTATION"));

	::memset(&m_lf, 0x00, sizeof(m_lf));
	m_lf.lfHeight = ::GetProfileInt(szSection, _T("TextHeight"), 14);
	m_lf.lfWeight = FW_NORMAL;
	m_lf.lfCharSet = ANSI_CHARSET;
	m_lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_lf.lfQuality = DEFAULT_QUALITY;
	m_lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	::GetProfileString(szSection, _T("SavedFont"), _T("굴림체"), m_lf.lfFaceName, sizeof(m_lf.lfFaceName));
}

void CSystemTradingDoc::ApplyClientConfig()
{
	SetSystemTradingFontLog();

	POSITION posView = GetFirstViewPosition();
	while(posView)
	{
		CSystemTradingView* pView = (CSystemTradingView*)GetNextView(posView);
		pView->SetSystemTradingColor();
		pView->Invalidate(FALSE);
	}
}

BOOL CSystemTradingDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
//	if (!CDocument::OnOpenDocument(lpszPathName))
//		return FALSE;
	
	CString strFile(lpszPathName);
	CString strExt(strFile.Right(3));

	if(!strExt.CompareNoCase(_T("uft")))
		m_nFileType = FT_USERFUNCTION;
	else if(!strExt.CompareNoCase(_T("sts")))
		m_nFileType = FT_STRATEGY;
	else 
		return FALSE;

	strExt.Format(_T("%s.bak"), lpszPathName);
	::CopyFile(lpszPathName, strExt, FALSE);

	// 1,password,1,userid,0,expires
	char szLine[MAX_PATH + 1] = {NULL, };
	((CMainFrame*)theApp.m_pMainWnd)->DecodingFile(strExt, lpszPathName, szLine);
	::SetFileAttributes(lpszPathName, FILE_ATTRIBUTE_HIDDEN);

	SetScriptName(lpszPathName);
	m_pScriptSec = new CScriptSecurity(m_nFileType, m_szScriptName, szLine);

	// 만료일 검사
	if(m_pScriptSec->GetUseExpires())
	{
		CTime tmCurrent(CTime::GetCurrentTime());
		int nCurrent = (tmCurrent.GetYear() * 10000) + (tmCurrent.GetMonth() * 100) + tmCurrent.GetDay();
		int nExpires = atoi(m_pScriptSec->GetExpires());

		if(nCurrent > nExpires)
		{
			AfxMessageBox("만료일이 지난 스크립트이므로 사용할 수 없습니다.");

			::SetFileAttributes(lpszPathName, FILE_ATTRIBUTE_NORMAL);
			::CopyFile(strExt, lpszPathName, FALSE);
			::DeleteFile(strExt);

			return FALSE;
		}
	}

	// 비밀번호 검사
	if(m_pScriptSec->GetUsePassword())
	{
		CDlgPassword dlg;
		if(dlg.DoModal() == IDOK)
		{
			if(dlg.m_strPassword.Compare(m_pScriptSec->GetPassword()))
			{
				AfxMessageBox(_T("올바른 비밀번호가 아닙니다."));

				::SetFileAttributes(lpszPathName, FILE_ATTRIBUTE_NORMAL);
				::CopyFile(strExt, lpszPathName, FALSE);
				::DeleteFile(strExt);

				return FALSE;
			}
		}
		else
		{
			::SetFileAttributes(lpszPathName, FILE_ATTRIBUTE_NORMAL);
			::CopyFile(strExt, lpszPathName, FALSE);
			::DeleteFile(strExt);
	
			return FALSE;
		}
	}

	// ID검사
	if(m_pScriptSec->GetUseUserID())
	{
		CString strIDs(m_pScriptSec->GetUserID());
		CString strID(_T(""));
		int i = 0;

		while(AfxExtractSubString(strID, strIDs, i, ';'))
		{
			if(strID.IsEmpty())
				break;

			// 여기서 ID검사

			i++;
		}
	}

	BOOL bLoad = m_xTextBuffer.LoadFromFile(lpszPathName);
	if(bLoad)
	{
		SetScriptType(FALSE);

		((CMainFrame*)theApp.m_pMainWnd)->ShowFunctionInStrategy(GetScriptName(), m_nFileType, m_nScriptType);
	}

	return bLoad;
}

BOOL CSystemTradingDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	CString strFile(lpszPathName);
	CString strExt = strFile.Right(3);

	if(!strExt.CompareNoCase(_T("uft")))
	{
		m_nFileType = FT_USERFUNCTION;
		UINT nCompiled = CMainFrame::IMG_FUNC;
		if(m_pScriptSec->GetUsePassword())
			nCompiled++;

		if(m_bModified)
			ChangeCompiledOption(nCompiled);
	}
	else if(!strExt.CompareNoCase(_T("sts")))
	{
		m_nFileType = FT_STRATEGY;
		UINT nCompiled = CMainFrame::IMG_STS;
		if(m_pScriptSec->GetUsePassword())
			nCompiled++;

		if(m_bModified)
			ChangeCompiledOption(nCompiled);
	}
	else 
		return FALSE;

	((CMainFrame*)theApp.m_pMainWnd)->ReloadScriptInfoToExtLib();

	ProcessSaveFile(lpszPathName);
	SetScriptName(lpszPathName);
	
	((CMainFrame*)theApp.m_pMainWnd)->ScanScriptFile(m_szScriptName, m_nFileType, m_nScriptType);
	((CMainFrame*)theApp.m_pMainWnd)->ChangeProperty(FALSE, m_szScriptName, m_nFileType);
	((CMainFrame*)theApp.m_pMainWnd)->ChangeProperty(TRUE, m_szScriptName, m_nFileType);

	if(m_nFileType == FT_STRATEGY)
		((CMainFrame*)theApp.m_pMainWnd)->MakeSTFile(m_szScriptName);

	return TRUE;	//	Note - we didn't call inherited member!
}

void CSystemTradingDoc::DeleteContents() 
{
	CDocument::DeleteContents();
	m_xTextBuffer.FreeAll();
}

void CSystemTradingDoc::SetTitle(LPCTSTR lpszTitle) 
{
	if(m_nFileType == FT_STRATEGY)
	{
		CString strTitle(_T(""));
		CString strName(lpszTitle);
		strTitle.Format(_T("[%s] Strategy"), strName.Left(strName.Find(_T("."))));
	
		CDocument::SetTitle(strTitle);
	}
	else if(m_nFileType == FT_USERFUNCTION)
	{
		CString strTitle(_T(""));
		CString strName(lpszTitle);
		strTitle.Format(_T("[%s] Function"), strName.Left(strName.Find(_T("."))));
	
		CDocument::SetTitle(strTitle);
	}
	else
		CDocument::SetTitle(lpszTitle);
}

BOOL CSystemTradingDoc::SaveModified() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDocument::SaveModified();
}

void CSystemTradingDoc::OnBuildCompile() 
{	
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if(m_bModified)
	{
		char szSaveFile[MAX_PATH] = {NULL, };
		::lstrcpy(szSaveFile, GetParseFileName());

		ProcessSaveFile(szSaveFile);
	}

	ClearAllExecution();
	
	theApp.m_pMainWnd->SendMessage(RMSG_COMPILEDLL, FALSE, (LPARAM)GetScriptName());
}

void CSystemTradingDoc::OnBuildSingleCompile() 
{
	if(m_bModified)
	{
		char szSaveFile[MAX_PATH] = {NULL, };
		::lstrcpy(szSaveFile, GetParseFileName());

		CString strFile(szSaveFile);
		CString strExt = strFile.Right(3);

		if(!strExt.CompareNoCase(_T("uft")))
			ChangeCompiledOption(CMainFrame::IMG_FUNC);
		else if(!strExt.CompareNoCase(_T("sts")))
			ChangeCompiledOption(CMainFrame::IMG_STS);

		ProcessSaveFile(szSaveFile);
	}
	
	ClearAllExecution();

	if(m_nFileType == FT_STRATEGY)
	{
		UINT nMessage = m_nScriptType == ST_TRADESTATION ? RMSG_SINGLIECOMPILEDLLMAIN : RMSG_SINGLIECOMPILEDLLMAIN_YES;
		theApp.m_pMainWnd->SendMessage(nMessage, FALSE, (LPARAM)GetScriptName());
	}
	else if(m_nFileType == FT_USERFUNCTION)
	{
		UINT nMessage = m_nScriptType == ST_TRADESTATION ? RMSG_SINGLIECOMPILEDLLFUNCTION : RMSG_SINGLIECOMPILEDLLFUNCTION_YES;
		theApp.m_pMainWnd->SendMessage(nMessage, FALSE, (LPARAM)GetScriptName());
	}
}

void CSystemTradingDoc::OnUpdateBuildCompile(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(FALSE);
//	return;

	if(m_nFileType == FT_STRATEGY)
	{
		CMainFrame* pMainFrame = (CMainFrame*)theApp.m_pMainWnd;
		pCmdUI->Enable(!pMainFrame->m_bCompiling);
	}
	else
		pCmdUI->Enable(FALSE);
}

void CSystemTradingDoc::OnViewProperty() 
{
	if(m_bModified)
	{
		char szSaveFile[MAX_PATH] = {NULL, };
		::lstrcpy(szSaveFile, GetParseFileName());

		ProcessSaveFile(szSaveFile);

		CMainFrame* pMainFrame = (CMainFrame*)theApp.m_pMainWnd;
		pMainFrame->ScanScriptFile(m_szScriptName, m_nFileType, m_nScriptType);

		pMainFrame->ChangeProperty(FALSE, m_szScriptName, m_nFileType);
		pMainFrame->ChangeProperty(TRUE, m_szScriptName, m_nFileType);
	}
	
	theApp.m_pMainWnd->SendMessage(RMSG_VIEWPROPERTY, 0, (LPARAM)m_pScriptSec);
}

void CSystemTradingDoc::ChangeCompiledOption(UINT nCompiled)
{
	CMainFrame* pMainFrame = (CMainFrame*)theApp.m_pMainWnd;

	pMainFrame->ChangeCompiledOption(GetScriptName(), m_nFileType, nCompiled);
}

void CSystemTradingDoc::SetScriptName(LPCTSTR lpszPathName)
{
	LPCSTR lpszName = ::strrchr(lpszPathName, '\\') + 1;
	int nEnd = ::lstrlen(lpszName) - ::lstrlen(::strrchr(lpszName, '.'));

	::strncpy(m_szScriptName, lpszName, nEnd);
}

void CSystemTradingDoc::OnBuildCompileDll() 
{
	if(m_bModified)
	{
		char szSaveFile[MAX_PATH] = {NULL, };
		::lstrcpy(szSaveFile, GetParseFileName());

		CString strFile(szSaveFile);
		CString strExt = strFile.Right(3);

		if(!strExt.CompareNoCase(_T("uft")))
			ChangeCompiledOption(CMainFrame::IMG_FUNC);
		else if(!strExt.CompareNoCase(_T("sts")))
			ChangeCompiledOption(CMainFrame::IMG_STS);

		ProcessSaveFile(szSaveFile);
	}

	ClearAllExecution();
	
	theApp.m_pMainWnd->SendMessage(RMSG_COMPILEDLL, TRUE, (LPARAM)GetScriptName());
}

void CSystemTradingDoc::OnUpdateBuildCompileDll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
	return;

	if(m_nFileType == FT_STRATEGY)
	{
		CMainFrame* pMainFrame = (CMainFrame*)theApp.m_pMainWnd;
		pCmdUI->Enable(!pMainFrame->m_bCompiling);
	}
	else
		pCmdUI->Enable(FALSE);
}

void CSystemTradingDoc::OnBuildSingleCompileDll() 
{
	if(m_bModified)
	{
		char szSaveFile[MAX_PATH] = {NULL, };
		::lstrcpy(szSaveFile, GetParseFileName());

		ProcessSaveFile(szSaveFile);
	}
	
	ClearAllExecution();

	if(m_nFileType == FT_STRATEGY)
	{
		UINT nMessage = m_nScriptType == ST_TRADESTATION ? RMSG_SINGLIECOMPILEDLLMAIN : RMSG_SINGLIECOMPILEDLLMAIN_YES;
		theApp.m_pMainWnd->SendMessage(nMessage, TRUE, (LPARAM)GetScriptName());
	}
	else if(m_nFileType == FT_USERFUNCTION)
	{
		UINT nMessage = m_nScriptType == ST_TRADESTATION ? RMSG_SINGLIECOMPILEDLLFUNCTION : RMSG_SINGLIECOMPILEDLLFUNCTION_YES;
		theApp.m_pMainWnd->SendMessage(nMessage, TRUE, (LPARAM)GetScriptName());
	}
}

void CSystemTradingDoc::SelectLine(UINT nLineNumber)
{
	POSITION pos = GetFirstViewPosition();
	
	CSystemTradingView* pView = (CSystemTradingView*)GetNextView(pos);
	if(pView)
		pView->SetLineSelect(nLineNumber);
}

void CSystemTradingDoc::SelectLine(UINT nLineNumber, UINT nColumn)
{
	POSITION pos = GetFirstViewPosition();
	
	CSystemTradingView* pView = (CSystemTradingView*)GetNextView(pos);
	if(pView)
		pView->SetLineSelect(nLineNumber, nColumn);
}

void CSystemTradingDoc::ClearAllExecution()
{
	POSITION pos = GetFirstViewPosition();
	
	CSystemTradingView* pView = (CSystemTradingView*)GetNextView(pos);
	if(pView)
		pView->ClearAllExecution();
}

void CSystemTradingDoc::OnUpdateBuildSingleCompile(CCmdUI* pCmdUI) 
{
	CMainFrame* pMainFrame = (CMainFrame*)theApp.m_pMainWnd;
	pCmdUI->Enable(!pMainFrame->m_bCompiling);
}

void CSystemTradingDoc::OnUpdateBuildSingleCompileDll(CCmdUI* pCmdUI) 
{
	CMainFrame* pMainFrame = (CMainFrame*)theApp.m_pMainWnd;
	pCmdUI->Enable(!pMainFrame->m_bCompiling);
}

void CSystemTradingDoc::OnBuildStop() 
{
	CMainFrame* pMainFrame = (CMainFrame*)theApp.m_pMainWnd;
	pMainFrame->StopCompiling(m_szScriptName);
}

void CSystemTradingDoc::OnUpdateBuildStop(CCmdUI* pCmdUI) 
{
	CMainFrame* pMainFrame = (CMainFrame*)theApp.m_pMainWnd;
	pCmdUI->Enable(pMainFrame->m_bCompiling);
}

void CSystemTradingDoc::ProcessSaveFile(LPCSTR lpszFile)
{
	m_xTextBuffer.SaveToFile(lpszFile);

	char szBackupFile[MAX_PATH] = {NULL, };
	::wsprintf(szBackupFile, _T("%s.bak"), lpszFile);
	
	// 1,password,1,userid,0,expires
	char szLine[MAX_PATH + 1] = {NULL, };
	if(m_pScriptSec)
	{
		::wsprintf(szLine, _T("%d,%s,%d,%s,%d,%s"), 
			m_pScriptSec->GetUsePassword(), m_pScriptSec->GetPassword(), 
			m_pScriptSec->GetUseUserID(), m_pScriptSec->GetUserID(), 
			m_pScriptSec->GetUseExpires(), m_pScriptSec->GetExpires());
	}
	
	((CMainFrame*)theApp.m_pMainWnd)->EncodingFile(lpszFile, szBackupFile, szLine);
	::SetFileAttributes(lpszFile, FILE_ATTRIBUTE_HIDDEN);
}

void CSystemTradingDoc::SaveFile()
{
	char szBackupFile[MAX_PATH] = {NULL, };
	::wsprintf(szBackupFile, _T("%s.bak"), m_strPathName);
	
	::SetFileAttributes(m_strPathName, FILE_ATTRIBUTE_NORMAL);
	if(::CopyFile(szBackupFile, m_strPathName, FALSE))
		::DeleteFile(szBackupFile);
}