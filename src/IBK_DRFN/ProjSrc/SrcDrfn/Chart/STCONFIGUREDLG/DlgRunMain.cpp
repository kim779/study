// DlgSTOption.h 와 DlgSTOption.cpp를 참고로 해서 만듬.
// 
// DlgRunMain.cpp : implementation file
//

#include "stdafx.h"
#include "DlgRunMain.h"
#include "DlgSTConfig.h"
#include "../Common_ST/STControllerDef.h"
#include ".\dlgrunmain.h"
#include <io.h>				// for _chmod() function supports
#include <sys/stat.h>		// for _chmod() function supports


const UINT RMSG_STCONFIGURENAME = ::RegisterWindowMessage("RMSG_STCONFIGURENAME");// when user clicks OK or Cancel, send it...
const UINT RMSG_VERIFYSCRIPTNAME = ::RegisterWindowMessage("RMSG_VERIFYSCRIPTNAME");// when user Kills focus at Name control, send it...


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRunMain dialog
//#define CRBACKGROUND	RGB(211,223,241)
#define CRBACKGROUND	RGB(239,242,247)
#define DEFAULT_BKGRND_COLOR		RGB(247,248,252)

CDlgRunMain::CDlgRunMain(LPCSTR szSTFile, LPCSTR szSTName, CWnd* pParent /*=NULL*/)
	: CRscDlg(CDlgRunMain::IDD, pParent)
{
	m_szSTFile = szSTFile;
	m_szSTName = szSTName;
//	m_crBk = CRBACKGROUND;
	m_crBk = DEFAULT_BKGRND_COLOR;
	m_hParentUsr = NULL;
	m_hViewHandle = NULL;

	//{{AFX_DATA_INIT(CDlgRunMain)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgRunMain::CDlgRunMain(HWND hViewHandle, LPCSTR szSTFile, LPCSTR szSTName, CWnd* pParent /*=NULL*/)
	: CRscDlg(CDlgRunMain::IDD, pParent)
{
	m_szSTFile = szSTFile;
	m_szSTName = szSTName;
//	m_crBk = CRBACKGROUND;
	m_crBk = DEFAULT_BKGRND_COLOR;
	m_hParentUsr = NULL;
	m_hViewHandle = hViewHandle;

	//{{AFX_DATA_INIT(CDlgRunMain)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CDlgRunMain::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRunMain)
	DDX_Control(pDX, IDC_ST_TITLE, m_editTitle);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_SETDEFAULT, m_btDefault);
	DDX_Control(pDX, IDC_TAB1, m_tabCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRunMain, CRscDlg)
	//{{AFX_MSG_MAP(CDlgRunMain)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SETDEFAULT, OnSetdefault)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRunMain message handlers

BOOL CDlgRunMain::OnInitDialog() 
{
	CRscDlg::OnInitDialog();

	InitControl();
	// TODO: Add extra initialization here
	m_brBkColor.CreateSolidBrush(m_crBk);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgRunMain::OnDestroy() 
{
	CRscDlg::OnDestroy();	

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
}

HBRUSH CDlgRunMain::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CDlgRunMain::InitControl()
{
	m_editTitle.SetWindowText(m_szSTName);
//	SetDlgItemText(IDC_ST_TITLE, m_szSTName);
	CString szDefCFGFile;
	GetDefaultFile(szDefCFGFile);

//	m_btOK.SetSkin(IDB_BITMAP_OKNOR,IDB_BITMAP_OKSEL,IDB_BITMAP_OKNOR,IDB_BITMAP_OKDIS);
//	m_btCancel.SetSkin(IDB_BITMAP_CANCELNOR,IDB_BITMAP_CANCELSEL,IDB_BITMAP_CANCELNOR,IDB_BITMAP_CANCELDIS);

	m_tabCtrl.SetTopLeftCorner(CPoint(10,62));
	m_tabCtrl.SetLeftOffset(0);	//9
	m_tabCtrl.SetUpperOffset(0);//9
	m_tabCtrl.SetSelectedColor(RGB(0, 0, 0),CRBACKGROUND);
	m_tabCtrl.SetNormalColor(RGB(0,0,0), RGB(255,255,255) , RGB(226,225,233));
	m_tabCtrl.SetTabBkColor(DEFAULT_BKGRND_COLOR);

	CTabRunSignal*	pTabRunSignal	= new CTabRunSignal;	// 신호설정
	pTabRunSignal->SetSTInfo(m_szSTFile);
	pTabRunSignal->SetSTDefInfo(szDefCFGFile);
	pTabRunSignal->Create(CTabRunSignal::IDD,this);
	pTabRunSignal->SetColor(m_crBk);
	pTabRunSignal->SetBrush(&m_brBkColor);
	m_arrayTab.Add(pTabRunSignal);
	m_tabCtrl.AddTab(pTabRunSignal,"신호설정");

	CTabRunJumun*	pTabRunJumun	= new CTabRunJumun;		// 주문설정
	pTabRunJumun->SetSTInfo(m_szSTFile);
	pTabRunJumun->SetSTDefInfo(szDefCFGFile);
//	pTabRunJumun->SetViewHandle(m_hViewHandle);
	pTabRunJumun->Create(CTabRunJumun::IDD,this);
	pTabRunJumun->SetColor(m_crBk);
	pTabRunJumun->SetBrush(&m_brBkColor);
	m_arrayTab.Add(pTabRunJumun);
	m_tabCtrl.AddTab(pTabRunJumun,"주문설정");

	// 강제청산과 운영설정 이 바뀌었다고 함.
	CTabRunSetup*	pTabRunSetup	= new CTabRunSetup;		// 강제청산
	pTabRunSetup->SetSTInfo(m_szSTFile);
	pTabRunSetup->SetSTDefInfo(szDefCFGFile);
	pTabRunSetup->Create(CTabRunSetup::IDD,this);
	pTabRunSetup->SetColor(m_crBk);
	pTabRunSetup->SetBrush(&m_brBkColor);
	m_arrayTab.Add(pTabRunSetup);
	m_tabCtrl.AddTab(pTabRunSetup,"강제청산");

	// 수정할 예정. 06.09.29
	CTabRunClear*	pTabRunClear	= new CTabRunClear;		// 운용설정
	pTabRunClear->SetSTInfo(m_szSTFile);
	pTabRunClear->SetSTDefInfo(szDefCFGFile);
	pTabRunClear->Create(CTabRunClear::IDD,this);
	pTabRunClear->SetColor(m_crBk);
	pTabRunClear->SetBrush(&m_brBkColor);
	m_arrayTab.Add(pTabRunClear);
	m_tabCtrl.AddTab(pTabRunClear,"운용설정");


}

BOOL CDlgRunMain::SaveConfig()
{
	CRscDlg *pTabDlg = NULL;
	int nSize = m_arrayTab.GetSize();
	BOOL bRetValue = FALSE;

	pTabDlg = m_arrayTab.GetAt(1);		// 주문설정탭에서의 인증확인을 먼저한다.


	bRetValue = pTabDlg->SaveInfo();
	if(!bRetValue) 
	{
//		AfxMessageBox(_T("계좌 비밀번호가 입력되지 않았습니다."));
		m_tabCtrl.SelectTab(1);
//		pTabDlg->GetDlgItem(IDC_ET_PW)->SetFocus();
		return FALSE;
	}

	for(int nPos=0;nPos<nSize;nPos++)
	{
		pTabDlg = m_arrayTab.GetAt(nPos);
		//bRetValue = pTabDlg->SendMessage(UMSG_ST_SAVE_ENV);
		bRetValue = pTabDlg->SaveInfo();
		if(!bRetValue)
		{
			m_tabCtrl.SelectTab(nPos);
			// MessageBox(m_strErrorMessage);
			return FALSE;
		}
	}
	return TRUE;
}

void CDlgRunMain::OnOK()
{
	::chmod(m_szSTFile, _S_IWRITE);
	if(!SaveConfig()) return;
	if(m_hParentUsr)	::SendMessage(m_hParentUsr,RMSG_STCONFIGURENAME,(WPARAM)(LPCTSTR)m_szSTFile,1);
	CRscDlg::OnOK();
}

void CDlgRunMain::OnCancel()
{
	if(m_hParentUsr)	::SendMessage(m_hParentUsr,RMSG_STCONFIGURENAME,(WPARAM)(LPCTSTR)m_szSTFile,0);
	CRscDlg::OnCancel();
}

#include "../../inc/IMainInfoMng.h"
void CDlgRunMain::OnSetdefault()
{
	/*CRscDlg *pTabDlg = NULL;
	int nSize = m_arrayTab.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pTabDlg = m_arrayTab.GetAt(nPos);
		pTabDlg->SetDefault();
	}*/
	CString szDefaultFile;
	if(!GetDefaultFile(szDefaultFile))	return;

	CRscDlg *pTabDlg = NULL;
	int nSize = m_arrayTab.GetSize();
	BOOL bRetValue = FALSE;
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pTabDlg = m_arrayTab.GetAt(nPos);
		pTabDlg->SetSTInfo(szDefaultFile);
		bRetValue = pTabDlg->SaveInfo();
		pTabDlg->SetSTInfo(m_szSTFile);
	}
}


BOOL CDlgRunMain::GetDefaultFile(CString &szDefaultFile)
{
	IMainInfoManager* pMng = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
	if(pMng)
	{
		szDefaultFile = pMng->GetEncUserDir();
		szDefaultFile += "\\Chart\\STData\\ConfigDef.dat";//KHD : 기본값으로 저장 완료
		return TRUE;
	}
	return FALSE;
}

BOOL CDlgRunMain::PreTranslateMessage(MSG* pMsg) 
{
	// 엔터가 쳤을때 죽는 현상 방지		JSJ_Modify_040421
	if(pMsg->message == WM_KEYDOWN)   
	{
		if(pMsg->wParam == VK_F4 && (::GetKeyState(VK_CONTROL) & 0x8000))
			pMsg->wParam = VK_ESCAPE;

		if(pMsg->wParam == VK_ESCAPE)
			return TRUE;
		else
			return FALSE;
	}
	else if(pMsg->message == WM_SYSKEYDOWN)
	{
		if( ::GetKeyState(VK_MENU) & 0x8000)
			return FALSE;
	}
	
	return CRscDlg::PreTranslateMessage(pMsg);
}