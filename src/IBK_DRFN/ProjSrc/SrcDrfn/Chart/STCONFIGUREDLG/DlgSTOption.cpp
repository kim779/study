//-->090902-solomon : alzioyes ST에서의 @수정.
// 솔로몬의 경우 @가 버전처리되는 파일에 사용이 되면 안되기 때문에 
// '@'를 '^'으로 수정함.
//-->090902-solomon : alzioyes ST에서의 @수정.

// DlgSTOption.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSTOption.h"
#include "TabBasic.h"
#include "TabVariables.h"
#include "TabSignal.h"
#include "TabIndex.h"
#include "TabAuthority.h"
#include "DlgSTConfig.h"
#include "../Common_ST/STControllerDef.h"


const UINT RMSG_STCONFIGURENAME = ::RegisterWindowMessage("RMSG_STCONFIGURENAME");// when user clicks OK or Cancel, send it...
const UINT RMSG_VERIFYSCRIPTNAME = ::RegisterWindowMessage("RMSG_VERIFYSCRIPTNAME");// when user Kills focus at Name control, send it...
const UINT RMSG_STSAVETOUSERST = ::RegisterWindowMessage("RMSG_STSAVETOUSERST");


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSTOption dialog
#define CRBACKGROUND	RGB(211,223,241)


CDlgSTOption::CDlgSTOption(CWnd* pParent /*=NULL*/)
	: CRscDlg(CDlgSTOption::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSTOption)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSTControllerD = NULL;
	m_nType = 0;
	m_bUsePassword = FALSE;
	m_bUseUserID = FALSE;
	m_bUseExpiredDate = FALSE;
	m_hParentUsr = NULL;
	m_crBk = CRBACKGROUND;
	m_pTabBasic = NULL;
	m_pSTConfig = NULL;
}


void CDlgSTOption::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSTOption)
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_TAB1, m_tabCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSTOption, CRscDlg)
	//{{AFX_MSG_MAP(CDlgSTOption)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSTOption message handlers

BOOL CDlgSTOption::OnInitDialog() 
{
	CRscDlg::OnInitDialog();

	// TODO: Add extra initialization here
	m_brBkColor.CreateSolidBrush(m_crBk);
	m_btOK.SetSkin(IDB_BITMAP_OKNOR,IDB_BITMAP_OKSEL,IDB_BITMAP_OKNOR,IDB_BITMAP_OKDIS);
	m_btCancel.SetSkin(IDB_BITMAP_CANCELNOR,IDB_BITMAP_CANCELSEL,IDB_BITMAP_CANCELNOR,IDB_BITMAP_CANCELDIS);
	if(m_nType==2||m_nType==4)
	{
		SetWindowText("사용자함수옵션");
	}
	else
	{
		SetWindowText("단위전략옵션");
	}

	if(m_nOpenType == 0) {
		ModifyStyle(WS_CHILD, WS_POPUP|WS_CAPTION|WS_SYSMENU);

		CRect rcDlg;
		GetWindowRect(rcDlg);

		rcDlg.bottom += GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER);
		MoveWindow(rcDlg);

		m_tabCtrl.GetWindowRect(rcDlg);
		ScreenToClient(rcDlg);

		rcDlg.bottom += GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER);
		m_tabCtrl.MoveWindow(rcDlg);

		m_tabCtrl.SetTopLeftCorner(CPoint(10,28));//14,32
		m_tabCtrl.SetLeftOffset(0);	//9
		m_tabCtrl.SetUpperOffset(0);//9
		m_tabCtrl.SetSelectedColor(RGB(0, 0, 0),CRBACKGROUND);
		m_tabCtrl.SetNormalColor(RGB(0,0,0), RGB(255,255,255) , RGB(226,225,233));
		m_tabCtrl.SetTabBkColor(RGB(234,235,233));

		m_pTabBasic = new CTabBasic(this);
		m_pTabBasic->Create(CTabBasic::IDD,this);
		m_pTabBasic->SetColor(m_crBk);
		m_pTabBasic->SetBrush(&m_brBkColor);
		m_arrayTab.Add(m_pTabBasic);
		m_tabCtrl.AddTab(m_pTabBasic,"기본");
		m_pTabBasic->ShowWindow(SW_SHOW);


		if(m_nType==0||m_nType==1||m_nType==3)
		{
			CTabVariables *pTabVariables = new CTabVariables(this);
			pTabVariables->Create(CTabVariables::IDD,this);
			pTabVariables->SetColor(m_crBk);
			pTabVariables->SetBrush(&m_brBkColor);
			m_arrayTab.Add(pTabVariables);
			m_tabCtrl.AddTab(pTabVariables,"변수");

			CTabSignal *pTabSignal = new CTabSignal(this);
			pTabSignal->Create(CTabSignal::IDD,this);
			pTabSignal->SetColor(m_crBk);
			pTabSignal->SetBrush(&m_brBkColor);
			m_arrayTab.Add(pTabSignal);
			m_tabCtrl.AddTab(pTabSignal,"신호표시");

			CTabIndex *pTabIndex = new CTabIndex(this);
			pTabIndex->Create(CTabIndex::IDD,this);
			pTabIndex->SetColor(m_crBk);
			pTabIndex->SetBrush(&m_brBkColor);
			m_arrayTab.Add(pTabIndex);
			m_tabCtrl.AddTab(pTabIndex,"지표표시");

		}
		CTabAuthority *pTabAuthority = new CTabAuthority(this);
		pTabAuthority->Create(CTabAuthority::IDD,this);
		pTabAuthority->SetColor(m_crBk);
		pTabAuthority->SetBrush(&m_brBkColor);
		m_arrayTab.Add(pTabAuthority);
		m_tabCtrl.AddTab(pTabAuthority,"보안");	
	}
	else if(m_nOpenType == 1) {
		ModifyStyle(WS_POPUP|WS_CAPTION|WS_SYSMENU, WS_CHILD);

		m_tabCtrl.ShowWindow(SW_HIDE);
		m_pSTConfig = new CDlgSTConfig(this);
		m_pSTConfig->Create(CDlgSTConfig::IDD,this);
		m_pSTConfig->SetColor(m_crBk);
		m_pSTConfig->SetBrush(&m_brBkColor);
		m_pSTConfig->ShowWindow(SW_SHOW);

		CRect rect;
		GetClientRect(rect);
		if(m_pSTConfig->GetSafeHwnd())
			m_pSTConfig->SetWindowPos(NULL, rect.left , rect.top, rect.Width(), rect.Height(), SWP_NOACTIVATE);	
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSTOption::ChangeConfigureInfo()
{
	if(m_nOpenType == 0) {
		CRscDlg *pTabDlg = NULL;
		int nSize = m_arrayTab.GetSize();
		for(int nPos=0;nPos<nSize;nPos++)
		{
			pTabDlg = m_arrayTab.GetAt(nPos);
			pTabDlg->LoadInfo();
		}
	}
	else {
		if(m_pSTConfig)
			m_pSTConfig->ReloadInfo();
	}
}

void CDlgSTOption::OnDestroy() 
{
	CRscDlg::OnDestroy();
	
	// TODO: Add your message handler code here
	m_brBkColor.DeleteObject();
	CRscDlg *pTabDlg = NULL;
	int nSize = m_arrayTab.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pTabDlg = m_arrayTab.GetAt(nPos);
		pTabDlg->DestroyWindow();
		delete pTabDlg;
	}
	m_arrayTab.RemoveAll();	

	if(m_pSTConfig) {
		m_pSTConfig->DestroyWindow();
		delete m_pSTConfig;
		m_pSTConfig = NULL;
	}

}

CString CDlgSTOption::GetSfgPath()
{
	if(m_pSTControllerD)
	{
		return m_pSTControllerD->GetSfgPath();
	}
	return m_strSfgName;
}

CString CDlgSTOption::GetSdPath()
{
	CString strPath;
	if(m_pSTControllerD)
	{
		strPath = m_pSTControllerD->GetPath();
	}
	else
	{
		strPath = GetSfgPath();
	}
	int nPos = strPath.ReverseFind('\\');
	strPath.Delete(0,nPos+1);
	//nPos = strPath.Find('@');
	nPos = strPath.Find('^');	//090902-solomon @->^
	if(nPos > 0)
		strPath = strPath.Mid(0, nPos);
	else
		strPath = strPath.Left(strPath.GetLength() - 3);
	return strPath;
}

CString CDlgSTOption::GetPassword()
{
	if(m_pSTControllerD)
	{
		return m_pSTControllerD->GetPassword();
	}
	return m_strPassword;
}

CString CDlgSTOption::GetExpiredDate() 
{ 
	if(m_pSTControllerD)
	{
		CString strExpiredDate;
		strExpiredDate.Format("%d",m_pSTControllerD->GetExpiredDate());
		return strExpiredDate;
	}
	return m_strExpiredDate;
}

CString CDlgSTOption::GetUserID() 
{ 
	if(m_pSTControllerD)
	{
		return m_pSTControllerD->GetUserID();
	}
	return m_strUserID;
}

BOOL CDlgSTOption::GetUsePassword() 
{ 
	if(m_pSTControllerD)
	{
		return m_pSTControllerD->GetUsePassword();
	}
	return m_bUsePassword;
}

BOOL CDlgSTOption::GetUseUserID() 
{ 
	if(m_pSTControllerD)
	{
		return m_pSTControllerD->GetUseUserID();
	}
	return m_bUseUserID;
}

BOOL CDlgSTOption::GetUseExpiredDate() 
{ 
	if(m_pSTControllerD)
	{
		return m_pSTControllerD->GetUseExpiredDate();
	}
	return m_bUseExpiredDate;
}

void CDlgSTOption::ChangeSfgPath(LPCTSTR lpSfgName)
{
	CString strSfgName = GetSfgPath();
	int nPos = strSfgName.ReverseFind('\\');
	strSfgName.Delete(nPos,strSfgName.GetLength()-nPos);
	strSfgName += "\\";
	strSfgName += lpSfgName;
	//strSfgName += "@*.ST";
	strSfgName += "^*.ST";	//090902-solomon @->^
	GetSTFile(strSfgName);
//	m_strSfgName = strSfgName;
}

void CDlgSTOption::GetSTFile(LPCTSTR lpszScriptFile)
{
	CFileFind ff;
	BOOL bFind = ff.FindFile(lpszScriptFile);

	if(bFind)
	{
		ff.FindNextFile();		// A일치하는 결과는 하나이므로...
		m_strSfgName = ff.GetFilePath();
	}
	else
	{
		m_strSfgName = lpszScriptFile;
		m_strSfgName.Replace(_T("*"), _T("0"));
	}

}
void CDlgSTOption::OnCancel() 
{
	// TODO: Add extra cleanup here
	::SendMessage(m_hParentUsr,RMSG_STCONFIGURENAME,(WPARAM)(LPCTSTR)GetSfgPath(),0);
	CRscDlg::OnCancel();
}

void CDlgSTOption::SaveConfig()
{
	if(m_nOpenType == 0) {
		CRscDlg *pTabDlg = NULL;
		int nSize = m_arrayTab.GetSize();
		BOOL bRetValue = FALSE;
		for(int nPos=0;nPos<nSize;nPos++)
		{
			pTabDlg = m_arrayTab.GetAt(nPos);
			bRetValue = pTabDlg->SendMessage(UMSG_ST_SAVE_ENV);
			if(!bRetValue)
			{
				m_tabCtrl.SelectTab(nPos);
				MessageBox(m_strErrorMessage);
				return;
			}
		}
	}
	else {
		if(m_pSTConfig)
			m_pSTConfig->SendMessage(UMSG_ST_SAVE_ENV);
	}
}

void CDlgSTOption::DeleteConfig()
{
	if(m_nOpenType == 0) {
		CRscDlg *pTabDlg = NULL;
		int nSize = m_arrayTab.GetSize();
		BOOL bRetValue = FALSE;
		for(int nPos=0;nPos<nSize;nPos++)
		{
			pTabDlg = m_arrayTab.GetAt(nPos);
			pTabDlg->SendMessage(UMSG_ST_REMOVE_ENV);
		}
	}
}

void CDlgSTOption::OnOK() 
{
	// TODO: Add extra validation here
	if(m_nType==1||m_nType==2)
	{
		CString strName;
		if(m_pTabBasic)
		{
			m_pTabBasic->m_editName.GetWindowText(strName);
		}	
		BOOL bRetValue = ::SendMessage(m_hParentUsr,RMSG_VERIFYSCRIPTNAME,m_nType,(LPARAM)(LPCTSTR)strName);
		if(bRetValue) 
		{
			SaveConfig();
			CString strSfgPath = GetSfgPath();
			strSfgPath.MakeUpper();
			::SendMessage(m_hParentUsr,RMSG_STCONFIGURENAME,(WPARAM)(LPCTSTR)strSfgPath,1);
			CRscDlg::OnOK();
		}
	}
	else
	{
		SaveConfig();
		CString strSfgPath = GetSfgPath();
		strSfgPath.MakeUpper();
		::SendMessage(m_hParentUsr,RMSG_STCONFIGURENAME,(WPARAM)(LPCTSTR)strSfgPath,1);
		CRscDlg::OnOK();
	}
}

void CDlgSTOption::ApplyConfig()
{
	CString szPath;
	if(m_nType==1||m_nType==2)
	{
		CString strName;
		if(m_pTabBasic)
		{
			m_pTabBasic->m_editName.GetWindowText(strName);
		}	
		BOOL bRetValue = ::SendMessage(m_hParentUsr,RMSG_VERIFYSCRIPTNAME,m_nType,(LPARAM)(LPCTSTR)strName);
		if(bRetValue) 
		{
			SaveConfig();
			szPath = GetSfgPath();
			::SendMessage(m_hParentUsr,RMSG_STCONFIGURENAME,(WPARAM)(LPCTSTR)szPath,1);
		}
	}
	else
	{
		CString szSTPath;
		szSTPath.GetBuffer(MAX_PATH);
		if(::SendMessage(m_hParentUsr,RMSG_STSAVETOUSERST,(WPARAM)0L,(LPARAM)&szSTPath) == 1L) {
			szSTPath.ReleaseBuffer();
			m_szSTPath = szSTPath;
			SaveConfig();
			szPath = GetSfgPath();
			::SendMessage(m_hParentUsr,RMSG_STCONFIGURENAME,(WPARAM)(LPCTSTR)szPath,1);
			//::SendMessage(m_hParentUsr,RMSG_STCONFIGURENAME,(WPARAM)(LPCTSTR)m_szSTPath,1);
		}
	}
}

// 1:OnlySfg, 2: ST,if possible 
DWORD CDlgSTOption::GetPrivateProfileStringInST(LPCTSTR lpAppName,LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize, int nUseType)
{
	DWORD dwSizeRead = 0;
	if(m_pSTControllerD&&nUseType!=1)
	{
		CString strSTName = m_pSTControllerD->GetSTNamePath();
		int nPos = m_pSTControllerD->GetSTPos();
		if(strSTName.GetLength()&&nPos>=0)
		{
			CString strAppNameSTName;
			strAppNameSTName.Format("%s%d",lpAppName,nPos);
			dwSizeRead = ::GetPrivateProfileString(strAppNameSTName,lpKeyName,lpDefault,lpReturnedString,nSize,strSTName);
			if(dwSizeRead)
			{
				return dwSizeRead;
			}
		}
	}

	CString szPath = GetSfgPath();
	dwSizeRead = ::GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpReturnedString,nSize,szPath);
	if(!dwSizeRead)
	{
		lpReturnedString = (LPTSTR)lpDefault;
	}
	return dwSizeRead;
}


// 1:OnlySfg, 2: ST 3:Both
BOOL CDlgSTOption::WritePrivateProfileStringInST(LPCTSTR lpAppName,LPCTSTR lpKeyName,LPCTSTR lpString, int nUseType)
{
	BOOL bRetValue = 0;
	if(m_pSTControllerD&&nUseType!=1)
	{
		CString strSTName = m_pSTControllerD->GetSTNamePath();
		int nPos = m_pSTControllerD->GetSTPos();
		if(strSTName.GetLength()&&nPos>=0)
		{
			CString strAppNameSTName;
			strAppNameSTName.Format("%s%d",lpAppName,nPos);
//			bRetValue = ::WritePrivateProfileString(strAppNameSTName,lpKeyName,lpString,strSTName);
			bRetValue = ::WritePrivateProfileString(strAppNameSTName,lpKeyName,lpString,m_szSTPath);
		}
		if(nUseType==2)
		{
			return bRetValue;
		}
	}
	bRetValue = ::WritePrivateProfileString(lpAppName,lpKeyName,lpString,GetSfgPath());
	return bRetValue;

}

HBRUSH CDlgSTOption::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CRscDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return hbr;
		break;
	default:
		pDC->SetBkColor(m_crBk);
		return m_brBkColor;		
		break;
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}


