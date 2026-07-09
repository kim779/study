// DSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DSetupDlg.h"
//#include "afxdialogex.h"
#include "tmenu.h"
#include "../../h/axisvar.h"

// CDSetupDlg dialog

//IMPLEMENT_DYNAMIC(CDSetupDlg, CDialog)

const TCHAR *Header_Ddlg[] = {	_T("1"),			// 0
								_T("2"),			// 1
								_T("3"),		// 2	
								_T("4"),		// 2	
								NULL};	

const int colWidth_Ddlg[] = {	1,					// 0
								1,					// 1	
								1,					// 1
								0};					// 3


CDSetupDlg::CDSetupDlg(class CTMenu* menu, CString home, CString user, CWnd* pParent /*=NULL*/)
	: CSetupDialog(CDSetupDlg::IDD, pParent)
{
	m_caption = _T("기본설정");
	m_home = home;
	m_user = user;
	m_iselectlist = -1;
}

CDSetupDlg::CDSetupDlg(CWnd* pParent /*=NULL*/)
	: CSetupDialog(CDSetupDlg::IDD, pParent)
{

}

CDSetupDlg::~CDSetupDlg()
{
}

void CDSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DLIST_SOUND, m_list);
	DDX_Control(pDX, IDC_DCB_FSIZE, m_cbFsize);
	DDX_Control(pDX, IDC_DCB_MAX, m_cbMaxMap);
	DDX_Control(pDX, IDC_DCB_SOUNDFUNC, m_cbSoundFunc);
}


BEGIN_MESSAGE_MAP(CDSetupDlg, CDialog)
	ON_BN_CLICKED(IDC_DBTN_PLAY, &CDSetupDlg::OnBnClickedDbtnPlay)
	ON_WM_ACTIVATE()
//	ON_NOTIFY(HDN_ITEMCHANGED, 0, &CDSetupDlg::OnItemchangedDlistSound)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_DLIST_SOUND, &CDSetupDlg::OnItemchangedDlistSound)
ON_CBN_SELCHANGE(IDC_DCB_SOUNDFUNC, &CDSetupDlg::OnSelchangeDcbSoundfunc)
ON_NOTIFY(NM_DBLCLK, IDC_DLIST_SOUND, &CDSetupDlg::OnDblclkDlistSound)
END_MESSAGE_MAP()


// CDSetupDlg message handlers


BOOL CDSetupDlg::OnInitDialog()
{
	CSetupDialog::OnInitDialog();
	Init();
	return TRUE; 
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDSetupDlg::Init()
{
	char		buffer[128];
	memset(buffer,0, 128);
	CString		file, tmpS, dat, section, key, cpfile;
	file.Format(_T("%s\\%s\\%s\\%s"),m_home, USRDIR,m_user, SETUPFILE);

	CFileFind find;
	if(!find.FindFile(file))
	{
		cpfile.Format("%s\\%s\\axisensetup.ini", m_home, MTBLDIR);
		CopyFile(cpfile, file, TRUE);
	}

	InitList(m_list, Header_Ddlg);
	//윈도우 시작시 프로그램 실행
	GetPrivateProfileString(_T("SCREEN"), _T("WINSTART") , _T("0"), buffer, sizeof(buffer), file);
	tmpS.Format(_T("%s"), buffer);
	if(tmpS == _T("1"))
		((CButton*)GetDlgItem(IDC_DCK_WINSTART))->SetCheck(true);
	else
		((CButton*)GetDlgItem(IDC_DCK_WINSTART))->SetCheck(false);

	//프로그램 실행시 자동 로그인
	CWinApp* app = AfxGetApp();
	bool bauto  = app->GetProfileInt(WORKSTATION, "AutoLogin", 0) ? TRUE : FALSE;

//
	//GetPrivateProfileString(_T("SCREEN"), _T("AUTOLOGIN") , _T("0"), buffer, sizeof(buffer), file);
	//tmpS.Format(_T("%s"), buffer);
	if(bauto)
		((CButton*)GetDlgItem(IDC_DCK_AUTOLOGIN))->SetCheck(true);
	else
		((CButton*)GetDlgItem(IDC_DCK_AUTOLOGIN))->SetCheck(false);

	//폰트크기
	GetPrivateProfileString(_T("SCREEN"), _T("FONTSIZE") , _T("1"), buffer, sizeof(buffer), file);
	m_cbFsize.AddString(_T("작게"));
	m_cbFsize.AddString(_T("보통"));
	m_cbFsize.AddString(_T("크게"));
	tmpS.Format(_T("%s"), buffer);
	m_cbFsize.SetCurSel(_ttoi(tmpS));

	//최대화면 
	GetPrivateProfileString(_T("SCREEN"), _T("MAXMAP") , _T("1"), buffer, sizeof(buffer), file);
	m_cbMaxMap.AddString(_T("8"));
	m_cbMaxMap.AddString(_T("16"));
	m_cbMaxMap.AddString(_T("24"));
	m_cbMaxMap.AddString(_T("32"));
	tmpS.Format(_T("%s"), buffer);

	switch(atoi(tmpS))
	{
		case 8:
			m_cbMaxMap.SetCurSel(0);
			break;
		case 16:
			m_cbMaxMap.SetCurSel(1);
			break;
		case 24:
			m_cbMaxMap.SetCurSel(2);
			break;
		case 32:
			m_cbMaxMap.SetCurSel(3);
			break;
		default:
			m_cbMaxMap.SetCurSel(2);
			break;
	}

	//최대화면 초과시 
	GetPrivateProfileString(_T("SCREEN"), _T("mapop") , _T("0"), buffer, sizeof(buffer), file);
	tmpS.Format(_T("%s"), buffer);
	if(tmpS == _T("1"))
		((CButton*)GetDlgItem(IDC_DRA_WARN))->SetCheck(true);
	else
		((CButton*)GetDlgItem(IDC_DRA_FIRSTCLOSE))->SetCheck(true);

	//소리선택
	m_cbSoundFunc.AddString(_T("소리없음"));
	m_cbSoundFunc.AddString(_T("기본설정"));
	m_cbSoundFunc.AddString(_T("사용자설정"));

	//음소거 설정  IDC_DCK_SOUNDALL
	GetPrivateProfileString(_T("SCREEN"), _T("soundoff") , _T("0"), buffer, sizeof(buffer), file);
	tmpS.Format(_T("%s"), buffer);
	if(tmpS == _T("1"))
		((CButton*)GetDlgItem(IDC_DCK_SOUNDALL))->SetCheck(true);
	else
		((CButton*)GetDlgItem(IDC_DCK_SOUNDALL))->SetCheck(false);

	SetListItem();
}

CString CDSetupDlg::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return _T("");
}

void CDSetupDlg::SetListItem()
{
	int igubn = -1;
	char		buffer[128];
	memset(buffer,0, 128);
	CString		file, tmpS, dat, temp, data, wavepath;
	file.Format(_T("%s\\%s\\%s\\%s"),m_home, USRDIR,m_user, SETUPFILE);

	GetPrivateProfileString(_T("SOUND"), _T("count") , _T("0"), buffer, sizeof(buffer), file);
	tmpS.Format(_T("%s"), buffer);

	if(_ttoi(tmpS) == 0)
	{
		file.Format(_T("%s\\%s\\%s"),m_home, MTBLDIR,SETUPFILE);
		GetPrivateProfileString(_T("SOUND"), _T("count") , _T("0"), buffer, sizeof(buffer), file);
		tmpS.Format(_T("%s"), buffer);
	}

	for(int ii = 0 ; ii < _ttoi(tmpS) ; ii++)
	{
		dat.Format(_T("%.2d"), ii);
		GetPrivateProfileString(_T("SOUND"), dat , _T("00"), buffer, sizeof(buffer), file);
		data.Format(_T("%s"), buffer);
		temp = Parser(data, _T("\t"));
		dat =  Parser(data, _T("\t"));
		wavepath = Parser(data, _T("\t"));
		
		for(int jj = 0 ; jj < 4; jj++)
		{
			if(jj == 0)
			{
				m_list.InsertItem(ii, _T(""));
			//	if(_ttoi(data) > 1)
			//		igubn =  _ttoi(data) - 2;
			//	else
				igubn =  _ttoi(dat);

				if(!igubn)
					m_list.SetItem(ii, 0, LVIF_IMAGE, NULL, 0, 0, 0, 0);
				else
					m_list.SetItem(ii, 0, LVIF_IMAGE, NULL, 1, 0, 0, 0);
			}
			else if(jj == 1)
				m_list.SetItemText(ii,jj, temp);
			else  if(jj == 2)
				m_list.SetItemText(ii,jj, dat);	
			else  if(jj == 3)
				m_list.SetItemText(ii,jj, wavepath);	
		}
	}
}

void CDSetupDlg::InitList(CListCtrl &list, const TCHAR**pszTitle)
{

	CRect rc;
	list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	list.GetClientRect(&rc);

	list.InsertColumn(0,("icon"), LVCFMT_CENTER, 20);
	list.InsertColumn(1, _T("sound"), LVCFMT_LEFT, (int)(rc.Width() - 20));
	list.InsertColumn(2, _T("onoff"), LVCFMT_LEFT, 0);
	list.InsertColumn(3, _T("path"), LVCFMT_LEFT, 0);

	m_pImageList = new CImageList();		
	if(!m_pImageList->Create( 16, 16, ILC_COLOR32, 1, 1))
	{
		delete m_pImageList;
		m_pImageList = NULL;
	}
	else 
	{	
		CBitmap bitmapOn, bitmapOff
			;
		bitmapOff.LoadBitmap(IDB_SOUNDOFF);
		m_pImageList->Add(&bitmapOff, RGB(255, 0, 255));

		bitmapOn.LoadBitmap(IDB_SOUNDON);	
		m_pImageList->Add(&bitmapOn, RGB(255, 0, 255));
	
	}

	list.SetImageList(m_pImageList, LVSIL_SMALL);	
}

void CDSetupDlg::ResizeListColumn(CXListCtrl &list, const int *arrWidth)
{
#ifdef TESTDEBUG
	SCROLLINFO si;
	CRect rc;
	list.GetWindowRect(&rc);
	int nMargin=0;

	// 스크롤 유무에 따른 마진 추가
	list.GetScrollInfo(SB_VERT, &si);	
	if( si.nPage && si.nPage <= (UINT)si.nMax ) 
	{
		nMargin = ::GetSystemMetrics(SM_CXVSCROLL);
	}

	// 보더 유무에 따른 마진 추가
	DWORD dwExStyle = list.GetExStyle();
	if( dwExStyle&WS_EX_STATICEDGE || 
		dwExStyle&WS_EX_CLIENTEDGE )
	{
		nMargin += ::GetSystemMetrics(SM_CXEDGE)*2;
	}

	int nWidth = 0;
	int nLastWidth = rc.Width()-nMargin;
	for( int i=0; ; i++ )
	{
		// 마지막 컬럼일 경우
		if( arrWidth[i] == 0 )
		{
			list.SetColumnWidth(i, nLastWidth);
			break;
		}

		nWidth = (rc.Width()*arrWidth[i])/100;
		if( i == 0)
			list.SetColumnWidth(i, 0);
		else
			list.SetColumnWidth(i, nWidth);
		nLastWidth -= nWidth;
	}
	list.RedrawWindow();
#endif
}

void CDSetupDlg::ApplySetup()
{
	save_info();
}

void CDSetupDlg::save_info()
{
	CString		file, value, tmp;
	file.Format(_T("%s\\%s\\%s\\%s"),m_home, USRDIR, m_user, SETUPFILE);
	CWinApp* app = AfxGetApp();

	//윈도우 실행시 자동 시작
	if(((CButton *)GetDlgItem(IDC_DCK_WINSTART))->GetCheck())
	{
		WritePrivateProfileString(_T("SCREEN"), _T("WINSTART"), _T("1"), file);
		app->WriteProfileInt(WORKSTATION, "WINSTART", 1);
	}
	else
	{
		WritePrivateProfileString(_T("SCREEN"), _T("WINSTART"), _T("0"), file);
		app->WriteProfileInt(WORKSTATION, "WINSTART", 0);
	}

	//HTS 실행시 자동로그인 
	
	if(((CButton *)GetDlgItem(IDC_DCK_AUTOLOGIN))->GetCheck())
	{
		WritePrivateProfileString(_T("SCREEN"), _T("AUTOLOGIN"), _T("1"), file);
		app->WriteProfileInt(WORKSTATION, "AutoLogin", 1);
	}
	else
	{
		WritePrivateProfileString(_T("SCREEN"), _T("AUTOLOGIN"), _T("0"), file);
		app->WriteProfileInt(WORKSTATION, "AutoLogin", 0);
	}
		
	//폰트 사이즈
	value.Format(_T("%d"), m_cbFsize.GetCurSel());
	WritePrivateProfileString(_T("SCREEN"), _T("FONTSIZE"), value, file);

	//최대화면 갯수
	value.Format(_T("%d"), m_cbMaxMap.GetCurSel());
	switch(atoi(value))
	{
		case 0:
			value = _T("8");
			break;
		case 1:
			value = _T("16");
			break;
		case 2:
			value = _T("24");
			break;
		case 3:
			value = _T("32");
			break;
		default:
			value = _T("24");
			break;
	}
	WritePrivateProfileString(_T("SCREEN"), _T("MAXMAP"), value, file);

	//최대화면 초과시
	//최초 열린화면 닫기 or 경고창
	if(((CButton*)GetDlgItem(IDC_DRA_FIRSTCLOSE))->GetCheck())
		WritePrivateProfileString(_T("SCREEN"), _T("mapop"), _T("0"), file);
	else
		WritePrivateProfileString(_T("SCREEN"), _T("mapop"), _T("1"), file);

	//IDC_DCB_SOUNDFUNC
	//소리선택
	value.Format(_T("%d"), m_list.GetItemCount());
	WritePrivateProfileString(_T("SOUND"), _T("count"), value, file);
	for(int ii = 0 ; ii < m_list.GetItemCount() ; ii++)
	{
		tmp.Format(_T("%.2d"), ii);
		value.Format(_T("%s\t%s\t%s"), m_list.GetItemText(ii,1), m_list.GetItemText(ii,2), m_list.GetItemText(ii,3));
		WritePrivateProfileString(_T("SOUND"), tmp, value, file);
	}

	//음소거설정  
	if(((CButton*)GetDlgItem(IDC_DCK_SOUNDALL))->GetCheck())
		WritePrivateProfileString(_T("SCREEN"), _T("soundoff"), _T("1"), file);
	else
		WritePrivateProfileString(_T("SCREEN"), _T("soundoff"), _T("0"), file);
}


void CDSetupDlg::DefaultSetup()
{
	/*
	CString file;
	file.Format("%s\\%s\\usertool.ini", m_home, MTBLDIR);
	
	LoadInfo(file);	
	*/
}

void CDSetupDlg::OnBnClickedDbtnPlay()
{
	// TODO: Add your control notification handler code here
}


void CDSetupDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CSetupDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CDSetupDlg::OnItemchangedDlistSound(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!(pNMLV->uChanged == LVIF_STATE && pNMLV->uNewState == (LVIS_SELECTED | LVIS_FOCUSED)))
		return;

	m_iselectlist = pNMLV->iItem;

	int igubn = _ttoi(m_list.GetItemText(pNMLV->iItem, 2));
	m_cbSoundFunc.SetCurSel(igubn);

	if(m_cbSoundFunc.GetCurSel() == 0)
		((CWnd*)GetDlgItem(IDC_DBTN_PLAY))->EnableWindow(false);
	else
		((CWnd*)GetDlgItem(IDC_DBTN_PLAY))->EnableWindow(true);

	*pResult = 0;
}


void CDSetupDlg::OnSelchangeDcbSoundfunc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_cbSoundFunc.GetCurSel() == 2)
	{
		CFileDialog	dlg(TRUE, "*.wav", NULL, 
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			"수신음(*.wav) |*.wav||", NULL );
		if (dlg.DoModal() == IDOK)  //m_iselectlist
		{
			
			//m_list.SetItem(m_iselectlist, 0, LVIF_IMAGE, NULL, 1, 0, 0, 0);				
			m_list.SetItemText(m_iselectlist,3, dlg.GetPathName());
			//m_list.SetItemText(m_iselectlist,2, _T("3"));	
			//m_cbSoundFunc.SetCurSel(1);
		}
		else
			m_cbSoundFunc.SetCurSel(1);
	}
	else
	{
		if(m_iselectlist > -1)
		{
			CString tmp;
			m_list.SetItem(m_iselectlist, 0, LVIF_IMAGE, NULL, m_cbSoundFunc.GetCurSel(), 0, 0, 0);	
			tmp.Format(_T("%d"), m_cbSoundFunc.GetCurSel());
			m_list.SetItemText(m_iselectlist, 2, tmp);
		}
	}
}


void CDSetupDlg::OnDblclkDlistSound(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (MessageBox("해당 음원등록을 삭제하시겠습니까?", "알람 등록 삭제", MB_YESNO) == IDYES)
		m_list.DeleteItem(pNMItemActivate->iItem);

	*pResult = 0;
}


BOOL CDSetupDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
			case VK_ESCAPE:
				{
					::TranslateMessage(pMsg);
					::DispatchMessage(pMsg);
				}
				return TRUE;
		}
	}
	return CSetupDialog::PreTranslateMessage(pMsg);
}
