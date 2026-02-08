#include "stdafx.h"
#include "axisGDlg.h"
#include "TotalCfg.h"
#include "ObjConfig.h"
#include "SaveFrameDlg.h"
#include "ScreenCfg.h"
#include "toolcfg.h"
#include "SetDrawtoolDlg.h"
#include "SetAidDlg.h"
#include "SetYAxisDlg.h"
#include "SetYShowDlg.h"


__declspec(dllexport) char* WINAPI axTotalConfig(CWnd* pMain, DWORD tkCOLOR, char *envinfo, char *ginfo, char *scrinfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAxisGDlgApp*	pApp = (CAxisGDlgApp*)AfxGetApp();
	pApp->InitChart();

	char*		ret = NULL;
	CTotalCfg	dlg(pMain, tkCOLOR, envinfo, ginfo, scrinfo);
	
	if (dlg.DoModal() == IDOK)
	{
		ret = dlg.GetData();
		pApp->ReleaseChart();
 
 
		return ret;
	}

	ret = dlg.GetData();
	if (ret)
		delete [] ret;
	pApp->ReleaseChart();
	return NULL;
}

__declspec(dllexport) bool WINAPI axIndcConfig(CWnd* pMain, DWORD tkCOLOR, char* envinfo, char* graphtotal, char *graph)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAxisGDlgApp*	pApp = (CAxisGDlgApp*)AfxGetApp();
	pApp->InitChart();

	CObjConfig dlg(pMain, tkCOLOR, envinfo, graphtotal, graph);
	
	if (dlg.DoModal() == IDOK)
	{
		pApp->ReleaseChart();
		return true;
	}

	pApp->ReleaseChart();
	return false;
}

// 1 - save, 2-load, 3-cancel
__declspec(dllexport) int WINAPI axSaveFrame(CWnd* pMain, char *pName, char *fList, char *Path)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CSaveFrameDlg	dlg(pMain, pName, fList, Path);
	
	if (dlg.DoModal() == IDOK)
	{
		return dlg.GetState();
	}

	return dlg.GetState();
}

__declspec(dllexport) char* WINAPI axScreenConfig(CWnd* pMain, DWORD tkCOLOR, char* envinfo, char *ginfo, char* scrinfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return NULL;
#if 0
	CAxisGDlgApp*	pApp = (CAxisGDlgApp*)AfxGetApp();
	pApp->InitChart();

	CScreenCfg	dlg(pMain, tkCOLOR, envinfo, ginfo, scrinfo);
	char*		ret = NULL;
	
	if (dlg.DoModal() == IDOK)
	{
		ret = dlg.GetData();
		pApp->ReleaseChart();
		return ret;
	}

	pApp->ReleaseChart();
	return ret;
#endif
}

__declspec(dllexport) BOOL WINAPI axToolConfig(CWnd* pMain, void* pToolInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CToolCfg dlg(pMain, pToolInfo);
	if (dlg.DoModal() == IDOK)
		return TRUE;

	return FALSE;
}

__declspec(dllexport) bool WINAPI axPnToolConfig(CWnd* pwndGView, CWnd* pwndView, char* pcInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//CAxisGDlgApp* pApp = (CAxisGDlgApp*)AfxGetApp();
	//CSetCustomDlg dlg(pApp->GetMyTool(wndView), pPnToolInfo, pwndGView);
	//CSetDrawtoolDlg dlg(pApp->GetMyTool(pwndView), pcInfo, pwndGView);
	CSetDrawtoolDlg dlg(pcInfo, pwndView, pwndGView);
	if (dlg.DoModal() == IDOK)
		return true;

	return false;
}

__declspec(dllexport) bool WINAPI axPnAidConfig(CWnd* pwndGView, CWnd* pwndView, char* pcInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//CAxisGDlgApp* pApp = (CAxisGDlgApp*)AfxGetApp();
	//CSetAidDlg dlg(pApp->GetMyTool(pwndView), pcInfo, pwndGView);
	CSetAidDlg dlg(pcInfo, pwndView, pwndGView);
	if (dlg.DoModal() == IDOK)
		return true;

	return false;
}

__declspec(dllexport) bool WINAPI axYAxisConfig(CWnd* pwndGView, CWnd* pwndView, char* pcInfo, char* pcTotalInfo, char* pcEnvInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CSetYAxisDlg dlg(pcInfo, pcTotalInfo, pcEnvInfo, pwndView, pwndGView);
	if (dlg.DoModal() == IDOK)
		return true;

	return false;
}

__declspec(dllexport) int WINAPI axYShowConfigByMove()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CSetYShowDlg dlg;
	dlg.DoModal();
	return dlg.m_iRet;
}


