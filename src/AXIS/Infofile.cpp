// Infofile.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "Infofile.h"
#include <io.h>

#define	UPMAXSIZE	1024*15
#define	TM_WAIT		1234
#define	TMI_WAIT	1000*5

#define WM_USERINFO	WM_USER+1000
#define K_QUERYFILELIST	0
#define K_CHECKLIST	1
#define K_UPLOAD	2
#define K_DOWNLOAD	3

#define K_GUIDEMSG	9
#define CHART_OLD	"gex.zip"
#define _COMPRESS_

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfofile dialog


CInfofile::CInfofile(CWnd* wizard, bool upload /*= true*/, CWnd* parent /*=NULL*/)
	: CDialog(CInfofile::IDD, parent)
{
	//{{AFX_DATA_INIT(CInfofile)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_wizard = wizard;
	m_wait = false;
	m_upload = upload;
	m_brush.CreateSolidBrush(RGB(240, 240, 240));
	m_chartlist.RemoveAll();
	m_applyBits = APPLY_NONE;
	m_clearchart = false;
	m_bULFlag = false;
	m_strUL = "";
}

CInfofile::~CInfofile()
{
	m_errfile.RemoveAll();
	m_brush.DeleteObject();
}

void CInfofile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInfofile)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInfofile, CDialog)
	//{{AFX_MSG_MAP(CInfofile)
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_ALL, OnAll)
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USERINFO, OnMsg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfofile message handlers
LONG CInfofile::OnMsg(WPARAM wParam, LPARAM lParam)
{
	const int	kind = (int) lParam;
	CString	text, title;
	switch (kind)
	{
	case K_QUERYFILELIST:
		QueryFilelist();
		break;
	case K_CHECKLIST:
		if (m_filelist.GetSize() <= 0)
		{
			title.LoadString(ST_USEINFO_MSGTITLE);
			text.LoadString(ST_USEINFO_TEXT_NOFILELIST);
			//**::MessageBox(this->m_hWnd, text, title, MB_ICONINFORMATION|MB_OK);
			Axis::MessageBox(this, text, MB_ICONINFORMATION | MB_OK);
			PostMessage(WM_CLOSE);
		}
		break;
	case K_UPLOAD:
		Uploadfile();
		break;
	case K_DOWNLOAD:
		Downloadfile();
		break;
	case K_GUIDEMSG:
		title.LoadString(ST_USEINFO_MSGTITLE);
		//**::MessageBox(this->m_hWnd, m_msg, title, MB_ICONINFORMATION|MB_OK);
		Axis::MessageBox(this, m_msg, MB_ICONINFORMATION | MB_OK);
		if (m_upload)
			PostMessage(WM_USERINFO, 0, K_UPLOAD);
		else	PostMessage(WM_USERINFO, 0, K_DOWNLOAD);
		break;
	default:
		break;
	}
	return 0;
}

BOOL CInfofile::OnInitDialog() 
{
	CDialog::OnInitDialog();

#ifdef DF_USE_CPLUS17
	m_changer = std::make_unique<CControlChanger>(this);
#else
	m_changer = new CControlChanger(this);
#endif

	Init();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CInfofile::PreTranslateMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			if (!m_wait)	OnAction();
			return TRUE;
		case VK_ESCAPE:
			if (m_wait)	return TRUE;
			break;
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		if (m_wait)	return TRUE;
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

CString CInfofile::Parser(CString &srcstr, CString substr)
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
	return "";
}

void CInfofile::OnAction()
{
	m_errfile.RemoveAll();
	
	if (!Ready())
	{
		CString	text, title, strFILE, strMSG;
		title.LoadString(ST_USEINFO_MSGTITLE);
		if (m_errfile.GetSize())
		{

			text.LoadString(ST_USEINFO_TEXT_ERRORMSG);
			for (int ii = 0; ii < m_errfile.GetSize(); ii++)
			{
				strMSG = m_errfile.GetAt(ii);
				strFILE = Parser(strMSG, "\t");
				text += "\n[" + strFILE + "] " + strMSG;
			}
		}
		else	
			text.LoadString(ST_USEINFO_TEXT_NOCHECKFILE);

		Axis::MessageBox(this, text, MB_ICONINFORMATION | MB_OK);
		return;
	}

	
	m_wait = true;
	CString	guide;
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	if (m_upload)
	{
		guide.LoadString(ST_USEINFO_TEXT_UPLOADDING);
		GetWnd(IDC_GUIDEMSG)->SetWindowText(guide);
		if (!m_clearchart)
			Uploadfile();
		else
			ClearChart();
	}
	else
	{
		guide.LoadString(ST_USEINFO_TEXT_DOWNLOADDING);
		GetWnd(IDC_GUIDEMSG)->SetWindowText(guide);
		Downloadfile();
	}
}

void CInfofile::OnAll() 
{
	const CButton* button = (CButton *) GetWnd(IDC_ALL);
	if (button->GetCheck() == BST_CHECKED)
	{
		GetWnd(IDC_ENSETUPINFO)->EnableWindow(FALSE);
		GetWnd(IDC_TOOLEDITINFO)->EnableWindow(FALSE);
		GetWnd(IDC_SETTICKERINFO)->EnableWindow(FALSE);
		GetWnd(IDC_WORKSPACEINFO)->EnableWindow(FALSE);
		GetWnd(IDC_CHARTINFO)->EnableWindow(FALSE);
		GetWnd(IDC_CODEMEMO)->EnableWindow(FALSE);
	}
	else
	{
		GetWnd(IDC_ENSETUPINFO)->EnableWindow();
		GetWnd(IDC_TOOLEDITINFO)->EnableWindow();
		GetWnd(IDC_SETTICKERINFO)->EnableWindow();
		GetWnd(IDC_WORKSPACEINFO)->EnableWindow();
		GetWnd(IDC_CHARTINFO)->EnableWindow();
		GetWnd(IDC_CODEMEMO)->EnableWindow();
	}
}

BOOL CInfofile::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_wait)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		return TRUE;	
	}
	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CInfofile::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TM_WAIT)
	{
		m_wait = false;
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		KillTimer(nIDEvent);
		Axis::MessageBox("Time out............");
	}
	CDialog::OnTimer(nIDEvent);
}


void CInfofile::Init()
{
	CString	title, guide;
	if (m_upload)
	{
		title.LoadString(ST_USEINFO_UPTITLE);
		guide.LoadString(ST_USEINFO_STARTUPGUIDE);
		GetWnd(IDC_CODEMEMO)->ShowWindow(SW_SHOW);
	}
	else
	{
		title.LoadString(ST_USEINFO_DOWNTITLE);
		GetWnd(IDC_CODEMEMO)->ShowWindow(SW_HIDE);
		guide.LoadString(ST_USEINFO_STARTDOWNGUIDE);
		PostMessage(WM_USERINFO, 0, K_QUERYFILELIST);
	}

	SetWindowText(title);
	GetWnd(IDC_GUIDEMSG)->SetWindowText(guide);
}

void CInfofile::QueryFilelist()
{
	
	CString	string;
	m_wait = true;
	SetTimer(TM_WAIT, TMI_WAIT, NULL);
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));

	char		sndB[256]{};
	_userTH*	userTH = (_userTH *) sndB;
	usrinfo*	info = (usrinfo *) &sndB[L_userTH];

	memset(sndB, ' ', sizeof(sndB));
	sprintf(userTH->trc, "PMHOUPDN");
	userTH->key = 'L';
	userTH->stat = US_PASS;

	sprintf(info->gubn, "UL");
	info->dirt[0] = 'D';	// host -> pc
	sndB[L_userTH + L_usr] = '\0';

	BOOL	rc = FALSE;	

	m_bULFlag = false;
	m_strUL = "";

	m_wizard->InvokeHelper(DI_TRX, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_I4), (long)sndB, L_usr);
}

void CInfofile::ClearChart()
{
	if (!m_upload)
		return;

	m_clearchart = false;

	CString	string;
	m_wait = true;
	SetTimer(TM_WAIT, TMI_WAIT, NULL);
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));

	//2012.08.18 김덕기 - 연속처리 위한 플래그
	m_bULFlag = false;

	char		sndB[256]{};
	_userTH*	userTH = (_userTH *) sndB;
	usrinfo*	info = (usrinfo *) &sndB[L_userTH];

	memset(sndB, ' ', sizeof(sndB));
	sprintf(userTH->trc, "PMHOUPDN");
	userTH->key = 'L';
	userTH->stat = US_PASS;

	sprintf(info->gubn, "UD");
	info->dirt[0] = 'U';	// host -> pc
	info->cont[0] = 'X';
	sndB[L_userTH + L_usr] = '\0';

	BOOL	rc = FALSE;	
	m_wizard->InvokeHelper(DI_TRX, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_I4), (long)sndB, L_usr);
}


void CInfofile::ProcessFMX(WPARAM wParam, LPARAM lParam)
{
// 원래 소스 부분
// 	int	key = LOWORD(wParam);
// 	int	len = HIWORD(wParam);
// 
// 	usrinfo* info;
// 	CString kind;
// 
// 	info = (usrinfo *) (char *) lParam;
// 	lParam += L_usr;
// 	kind = CString(info->gubn, sizeof(info->gubn));
// 	
// 	if (kind == "UL")
// 	{
// 		m_wait = false;
// 		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
// 		KillTimer(TM_WAIT);
// 
// 		m_filelist.RemoveAll();
// 		m_chartlist.RemoveAll();
// 		int	count = atoi(CString(info->nblc, sizeof(info->nblc)));
// 
// 		CString s;
// 		
// 		d_list* list = (d_list*) (char *) lParam;
// 		for (int ii = 0; ii < count; ii++)
// 		{
// 			CString	name = CString(list[ii].name, sizeof(list[ii].name));
// 			name.TrimRight();
// 			m_filelist.Add(name);
// 			/**
// 			if (name.Find(".c") >= 0)
// 				m_chartlist.Add(name);
// 			**/
// 			CString nameUpper(name);
// 			nameUpper.MakeUpper();
// 
// // 			s.Format("DN FMX : %s\n",nameUpper);
// // 			OutputDebugString(s);
// 			if (nameUpper.Find(".INI") < 0 && nameUpper.Find(".MMO") < 0)
// 			{
// 				if (nameUpper.Find("IB7") >= 0)
// 					m_chartlist.Add(name);
// 				else if(nameUpper.Find("CHART.ZIP") >= 0)
// 					m_chartlist.Add(name);
// // 				else if(nameUpper.Find("DAT") >= 0 || nameUpper.Find("CHT") >= 0)
// // 					m_chartlist.Add(name);
// 			}
// 
// 		}
// 		PostMessage(WM_USERINFO, 0, K_CHECKLIST);
// 	}

	//2012.08.18 김덕기 - 연속처리 위한 플래그
	const int	key = LOWORD(wParam);
	const int	len = HIWORD(wParam);

	usrinfo* info{};
	CString kind;

	CString s;

	if(m_bULFlag == false)
	{
		m_strUL = (char*)lParam;
		m_strUL = m_strUL.Mid(0,len);

//  		s.Format("UL DATA : [%s]\n",m_strUL);
//  		OutputDebugString(s);

		info = (usrinfo *) (char *) lParam;
		lParam += L_usr;
		kind = CString(info->gubn, sizeof(info->gubn));
	}
	else
	{
		CString strTmp;
		strTmp = (char*)lParam;
		strTmp = strTmp.Mid(0,len);

// 		s.Format("UL DATA : [%s]\n",strTmp);
//  		OutputDebugString(s);
		//\n 제거를 위해
		//m_strUL = m_strUL.Mid(0,strlen(m_strUL));

		m_strUL += strTmp;
		
		lParam = (LPARAM)m_strUL.operator LPCTSTR();

		info = (usrinfo *) (char *) lParam;
		lParam += L_usr;
		kind = CString(info->gubn, sizeof(info->gubn));
	}
	
	if (kind == "UL")
	{
		m_wait = false;
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		KillTimer(TM_WAIT);

		m_filelist.RemoveAll();
		m_chartlist.RemoveAll();
		int	count = atoi(CString(info->nblc, sizeof(info->nblc)));
		
		const d_list* list = (d_list*) (char *) lParam;

		int nListCnt = 0;

		CString tmp = (char*)lParam;

		nListCnt = strlen(tmp)/sizeof(d_list);

// 		s.Format("UL COUNT : [%d]	LIST CNT : [%d] TMP [%s] \n",count,nListCnt,tmp);
// 		OutputDebugString(s);

		if(count > nListCnt)
		{
			m_bULFlag = true;
			return;
		}

		m_bULFlag = false;

// 		s.Format("UL DATA : [%s]\n",m_strUL);
// 		OutputDebugString(s);

		for (int ii = 0; ii < count; ii++)
		{
			CString	name = CString(list[ii].name, sizeof(list[ii].name));
			name.TrimRight();
			m_filelist.Add(name);
			/**
			if (name.Find(".c") >= 0)
				m_chartlist.Add(name);
			**/
			CString nameUpper(name);
			nameUpper.MakeUpper();

// 			s.Format("DN FMX : %s\n",nameUpper);
// 			OutputDebugString(s);
			if (nameUpper.Find(".INI") < 0 && nameUpper.Find(".MMO") < 0)
			{
				if (nameUpper.Find("IB7") >= 0)
					m_chartlist.Add(name);
				else if(nameUpper.Find("CHART.ZIP") >= 0)
					m_chartlist.Add(name);
// 				else if(nameUpper.Find("DAT") >= 0 || nameUpper.Find("CHT") >= 0)
// 					m_chartlist.Add(name);
			}

		}
		PostMessage(WM_USERINFO, 0, K_CHECKLIST);
	}
	else if (kind == "UD")
	{
		m_wait = false;
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		KillTimer(TM_WAIT);

		CString		file, fileN, str, szDir;
		CFile		hFile;
		UINT		nOpenFlags{};
		if (info->cont[0] == 'X')
		{
			if (m_upload && info->retc[0] == '0')
				PostMessage(WM_USERINFO, 0, K_UPLOAD);				
			else
				AfxMessageBox("서버파일 삭제오류");
			
			return;
		}

		if (info->retc[0] == 'E')
		{
			m_msg = CString(info->emsg, sizeof(info->emsg));
			PostMessage(WM_USERINFO, 0,  K_GUIDEMSG);
			return;
		}

		if (m_upload)
		{
			PostMessage(WM_USERINFO, 0, K_UPLOAD);
			return;
		}

		fileN = CString(info->name, sizeof(info->name));
		fileN.TrimRight();
		nOpenFlags = CFile::modeCreate|CFile::modeWrite|CFile::typeBinary;

		if (fileN.Find(".ini") >= 0 || fileN.Find(".mmo") >= 0)
			file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, fileN);
		else 
		{
			if (fileN.Find("IB7") >= 0 || fileN.Find("ib7") >= 0)
				file.Format("%s\\gex\\%s\\%s", Axis::home, Axis::user, fileN);
			else if(fileN.Find("Chart.zip") >= 0)
			{
				/*2012.08.07 김덕기 - 폴더 생성되게 수정*/
// 				CString chartFolder;
// 				chartFolder.Format("%s\\user\\%s\\Chart", Axis::home, Axis::user);
// 				CreateDirectory(chartFolder,NULL);
				
				file.Format("%s\\user\\%s\\%s", Axis::home, Axis::user, fileN);
			}
// 			else if(fileN.Find("dat") >= 0 || fileN.Find("cht") >= 0)
// 			{
// 				/*2012.08.07 김덕기 - 폴더 생성되게 수정*/
// 				CString chartFolder;
// 				chartFolder.Format("%s\\user\\%s\\Chart", Axis::home, Axis::user);
// 				CreateDirectory(chartFolder,NULL);
// 
// 				file.Format("%s\\user\\%s\\Chart\\%s", Axis::home, Axis::user, fileN);
// 			}
		}

		CString strTmp;
// 		strTmp.Format("OPEN DN FILE : [%s]\n",file);
// 		OutputDebugString(strTmp);

		file.TrimLeft();
		file.TrimRight();

		if(file == "")
		{
			PostMessage(WM_USERINFO, 0, K_DOWNLOAD);
			return;
		}

		if (!hFile.Open(file, nOpenFlags))
		{
			Axis::MessageBox(this, "파일 생성오류입니다.", MB_ICONINFORMATION);
			PostMessage(WM_USERINFO, 0, K_DOWNLOAD);
			return;
		}

		hFile.Write((char*) lParam, atoi(CString(info->nblc, sizeof(info->nblc))));
		hFile.Close();
		
		PostMessage(WM_USERINFO, 0, K_DOWNLOAD);
	}

}

bool CInfofile::Ready()
{
	CString		file, str, zipfile;

	m_sfilelist.RemoveAll();
	const CButton* button = (CButton *) GetWnd(IDC_ALL);
	if (button->GetCheck() == BST_CHECKED)
	{
		if (m_upload)
			m_clearchart= true;

		zipfile = Compress("axisensetup.ini");		// 사용자 환경설정
		if (!zipfile.IsEmpty())
		{
			m_sfilelist.Add(zipfile);
			str.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, zipfile);
			m_removelist.Add(str);
		}

		zipfile = Compress("usertool.ini");		// 사용자 툴바
		if (!zipfile.IsEmpty())
		{
			m_sfilelist.Add(zipfile);
			str.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, zipfile);
			m_removelist.Add(str);
		}

		zipfile = Compress("axisticker.ini");		// 티커정보
		if (!zipfile.IsEmpty())
		{
			m_sfilelist.Add(zipfile);
			str.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, zipfile);
			m_removelist.Add(str);
		}

		str.Format("%s.ini", Axis::user);	// workspace info file
		zipfile = Compress(str);
		if (!zipfile.IsEmpty())
		{
			m_sfilelist.Add(zipfile);
			str.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, zipfile);
			m_removelist.Add(str);
		}

		zipfile = Compress("memo.mmo");		// memo file
		if (!zipfile.IsEmpty())
		{
			m_sfilelist.Add(zipfile);
			str.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, zipfile);
			m_removelist.Add(str);
		}

// 		str.Format("%s\\gex\\%s", Axis::home, Axis::user);	// chart infomation
// 		CompressChart(str);		
		
		str.Format("%s\\user\\%s\\Chart", Axis::home, Axis::user);
		CompressDRFNChart(str);
	}
	else
	{
		if (((CButton *)GetWnd(IDC_ENSETUPINFO))->GetCheck() == BST_CHECKED)
		{
			zipfile = Compress("axisensetup.ini");
			if (!zipfile.IsEmpty())
			{
				m_sfilelist.Add(zipfile);
				str.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, zipfile);
				m_removelist.Add(str);
			}		
		}

		if (((CButton *)GetWnd(IDC_TOOLEDITINFO))->GetCheck() == BST_CHECKED)
		{
			zipfile = Compress("usertool.ini");
			if (!zipfile.IsEmpty())
			{
				m_sfilelist.Add(zipfile);
				str.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, zipfile);
				m_removelist.Add(str);
			}
		}

		if (((CButton *)GetWnd(IDC_SETTICKERINFO))->GetCheck() == BST_CHECKED)
		{
			zipfile = Compress("axisticker.ini");
			if (!zipfile.IsEmpty())
			{
				m_sfilelist.Add(zipfile);
				str.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, zipfile);
				m_removelist.Add(str);
			}
		}

		if (((CButton *)GetWnd(IDC_WORKSPACEINFO))->GetCheck() == BST_CHECKED)
		{
			str.Format("%s.ini", Axis::user);
			zipfile = Compress(str);
			if (!zipfile.IsEmpty())
			{
				m_sfilelist.Add(zipfile);
				str.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, zipfile);
				m_removelist.Add(str);
			}

			str = "saveInfo.ini";
			zipfile = Compress(str);
			if (!zipfile.IsEmpty())
			{
				m_sfilelist.Add(zipfile);
				str.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, zipfile);
				m_removelist.Add(str);
			}

			str = "saveCode.ini";
			zipfile = Compress(str);
			if (!zipfile.IsEmpty())
			{
				m_sfilelist.Add(zipfile);
				str.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, zipfile);
				m_removelist.Add(str);
			}
		}

		if (((CButton *)GetWnd(IDC_CODEMEMO))->GetCheck() == BST_CHECKED)
		{
			zipfile = Compress("memo.mmo");
			if (!zipfile.IsEmpty())
			{
				m_sfilelist.Add(zipfile);
				str.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, zipfile);
				m_removelist.Add(str);
			}
		}

		if (((CButton *)GetWnd(IDC_CHARTINFO))->GetCheck() == BST_CHECKED)
		{
// 			str.Format("%s\\gex\\%s", Axis::home, Axis::user);	// chart infomation
// 			CompressChart(str);			
			
			str.Format("%s\\user\\%s\\Chart", Axis::home, Axis::user);
			CompressDRFNChart(str);

			if (m_upload)
				m_clearchart = true;
		}
	}

	if (m_sfilelist.GetSize() <= 0)
		return false;
	return true;
}

void CInfofile::Uploadfile()
{
	CString		file, fileN, str;
	CFile		hFile;
	char		sndB[UPMAXSIZE]{};
	m_bUploadMemo = false; 
	for (;;)
	{
		if (m_sfilelist.GetSize() <= 0)
		{
			m_wait = false;
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			str.LoadString(ST_USEINFO_TEXT_ENDWORK);
			GetWnd(IDC_GUIDEMSG)->SetWindowText(str);
			removeCompressfile();
			return;
		}
		fileN = m_sfilelist.GetAt(0);
		m_sfilelist.RemoveAt(0);

		if (fileN.Find("memo") >= 0)
		{
			m_bUploadMemo = true; 
			continue;
		}

		CString s;
// 		s.Format("FILE NAME : %s\n",fileN);
// 		OutputDebugString(s);
		
		if (fileN.Find(".ini") >= 0 || fileN.Find(".mmo") >= 0)
			file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, fileN);
		else if(fileN.Find("Chart.zip") >= 0)
			file.Format("%s\\user\\%s\\%s", Axis::home, Axis::user, fileN);
		else
			file.Format("%s\\gex\\%s\\%s", Axis::home, Axis::user, fileN);
// 		else if(fileN.Find(".dat") >= 0 || fileN.Find(".cht") >= 0 || fileN.Find(".cfg") >= 0)
// 			file.Format("%s\\user\\%s\\Chart\\%s", Axis::home, Axis::user, fileN);

// 		s.Format("UPLOAD FILE : [%s]\n",file);
// 		OutputDebugString(s);

		if (!hFile.Open(file, CFile::modeRead | CFile::typeBinary))
			continue;
		
		const int fileL = (int)hFile.GetLength();
		const int fSize = L_usr+fileL;

// 		s.Format("OPEN UPLOAD FILE : [%s]	SIZE : [%d]\n",file,fSize);
// 		OutputDebugString(s);

		if (fileL > UPMAXSIZE)
		{
			hFile.Close();
			str.Format("%s이 전송 사이즈를 초과 하였습니다.", fileN);
			Axis::MessageBox(this, str, MB_ICONSTOP);
			continue;
		}
		//2012.08.18 김덕기 - 연속처리 위한 플래그
		m_bULFlag = false;
		
		_userTH*	userTH = (_userTH *) sndB;
		usrinfo*	info = (usrinfo *) &sndB[L_userTH];

		memset(sndB, ' ', fSize);
		hFile.Read(&sndB[L_userTH + L_usr], fileL);
		hFile.Close();

		sprintf(userTH->trc, "PMHOUPDN");
		userTH->key = 'L';
		userTH->stat = US_PASS;

		sprintf(info->gubn, "UD");
		info->dirt[0] = 'U';	// pc -> host
		CopyMemory(info->name, fileN, fileN.GetLength());
		sprintf(info->nblc, "%05d", fileL);
		info->retc[0] = '0';

		BOOL	rc = FALSE;
		m_wizard->InvokeHelper(DI_TRX, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_I4), (long)sndB, fSize);
		break;
	}
}

void CInfofile::Downloadfile()
{
	CString		file, fileN, str;

	for (;;)
	{
		if (m_sfilelist.GetSize() <= 0)
		{
			m_wait = false;
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			str.LoadString(ST_USEINFO_TEXT_ENDWORK);
			GetWnd(IDC_GUIDEMSG)->SetWindowText(str);
			Decompressfile();
			return;
		}

		bool	match  = false;
		fileN = m_sfilelist.GetAt(0);
		m_sfilelist.RemoveAt(0);

		if (fileN.Find("memo") >= 0) 
			continue;

		CString s;
// 		s.Format("DN FILE NAME : %s\n",fileN);
// 		OutputDebugString(s);

		for (int ii = 0; !match && ii < m_filelist.GetSize(); ii++)
		{
			str = m_filelist.GetAt(ii);
			if (fileN.CompareNoCase(str))
				continue;
			match  = true;
		}

		if (!match)	continue;

		//2012.08.18 김덕기 - 연속처리 위한 플래그
		m_bULFlag = false;

		char		sndB[1024]{};
		_userTH*	userTH = (_userTH *) sndB;
		usrinfo*	info = (usrinfo *) &sndB[L_userTH];
		
		memset(sndB, ' ', sizeof(sndB));
		sprintf(userTH->trc, "PMHOUPDN");
		userTH->key = 'L';
		userTH->stat = US_PASS;

		sprintf(info->gubn, "UD");
		info->dirt[0] = 'D';	// host -> pc
		const int	len = fileN.GetLength();
		if (sizeof(info->name) < len)
			continue;
		CopyMemory(info->name, fileN, len);

		BOOL	rc = FALSE;
		m_wizard->InvokeHelper(DI_TRX, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_I4), (long)sndB, L_usr);
		break;
	}
}

void CInfofile::Decompressfile()
{
	CString	file;

	for (int ii = 0; ii < m_removelist.GetSize(); ii++)
	{
		file = m_removelist.GetAt(ii);
// 		CString s;
// 		s.Format("DECOMP FILE LIST : [%s]\n",file);
// 		OutputDebugString(s);
		if (_taccess(file, 04))	continue;
		Decompress(file);
	}
	m_removelist.RemoveAll();
}

void CInfofile::removeCompressfile()
{
	CString	file;

	for (int ii = 0; ii < m_removelist.GetSize(); ii++)
		::DeleteFile(m_removelist.GetAt(ii));
	m_removelist.RemoveAll();
}

CString CInfofile::Compress(CString target, CString path)
{
	if (!m_upload)	return target + ".zip";

	CString		zipFile, srcFile;

	if (path.IsEmpty())
		srcFile.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, target);
	else
		srcFile.Format("%s\\%s", path, target);

	CString s;
// 	s.Format("SRC FILE : %s\n",srcFile);
// 	OutputDebugString(s);
	// does zip source file exist?
	if (_taccess(srcFile, 04))	// can read?
	{
		CString	str;
		str.Format("%s\t%s", srcFile, "파일을 찾을 수 없습니다.");
		//OutputDebugString(str);
		m_errfile.Add(str);
		return _T("");
	}
	
	// use only the file name for zip file entry
	const TCHAR * cp = _tcsrchr(srcFile, _T('\\'));  //vc2019  TCHAR -> const TCHAR
	if (cp == NULL)
		cp = (TCHAR *)(const char *)srcFile;
	else
		cp++;

	zipFile.Format("%s.zip", srcFile);
	HZIP hz = CreateZip((void *)(const char *) zipFile, 0, ZIP_FILENAME);
	if (hz)
	{
		const ZRESULT zr = ZipAdd(hz, cp, (void *)(const char *)srcFile, 0, ZIP_FILENAME);
		CloseZip(hz);

		// did add work?
		if (zr != ZR_OK)
			zipFile.Empty();
		else	zipFile.Format("%s.zip", target);
	}

	return zipFile;
}

CString CInfofile::CompressFolder(CString target, CString path)
{
	if (!m_upload)	return target + ".zip";
	
	CString		zipFile, srcFile, findFile;
	
// 	if (path.IsEmpty())
// 		srcFile.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, target);
// 	else
// 		srcFile.Format("%s\\%s", path, target);
	srcFile = target;
	
	CString s;
// 	s.Format("SRC FILE : %s\n",srcFile);
// 	OutputDebugString(s);
	// does zip source file exist?
	if (_taccess(srcFile, 0))	// can read?
	{
		CString	str;
		str.Format("%s\t%s", srcFile, "폴더를 찾을 수 없습니다.");
		OutputDebugString(str);
		m_errfile.Add(str);
		return _T("");
	}
	
	// use only the file name for zip file entry
	TCHAR * cp = (TCHAR * )_tcsrchr(srcFile, _T('\\'));   //vc2019 
	if (cp == NULL)
		cp = (TCHAR *)(const char *)srcFile;
	else
		cp++;
	
	zipFile.Format("%s.zip", srcFile);
	//HZIP hz = CreateZip((void *)(const char *) zipFile, 0, ZIP_FILENAME);

// 	s.Format("ZIP FOLDER : [%s]\n",zipFile);
// 	OutputDebugString(s);
// 
// 	s.Format("ZIP DATA : [%s]\n",cp);
// 	OutputDebugString(s);
	WIN32_FIND_DATA FindFileData = { 0 };
//	HANDLE		hFind;

// 	if (hz)
// 	{
// 		findFile.Format("%s\\*",srcFile);
// 
// // 		ZRESULT zr = ZipAdd(hz,cp, 0,0,ZIP_FOLDER);
// 
// 		//hFind = FindFirstFile(findFile, &FindFileData);
// 
// 		FindAllFiles(findFile,FindFileData,NULL,cp);
// // 
// // 		ZRESULT zr1 = ZipAdd(hz,"Chart\\saved",0,0,ZIP_FOLDER);
// 
// //		CloseZip(hz);
// 		
// 		// did add work?
// 		if (zr != ZR_OK)
// 			zipFile.Empty();
// 		else	zipFile.Format("%s.zip", cp);
// 	}
	findFile.Format("%s\\*",srcFile);
	
	FindAllFiles(findFile,FindFileData, cp);
	
	return zipFile;
}
#pragma warning (disable : 6067)
void CInfofile::FindAllFiles(CString path,WIN32_FIND_DATA FindFileData,TCHAR* cp)
{
	//OutputDebugString("BEGIN FIND ALL FILES\n");
	HANDLE hFind = FindFirstFile(path,&FindFileData);

	CString pPath;
	CString strTmp;
	CString cur_Tmp;
	CString strLog;
	CString filename;

	UpdateData(FALSE);

	if(hFind == INVALID_HANDLE_VALUE)
	{
		strLog.Format("COMPRESS FOLDER ERROR : [INVALID_HANDLE] [%s]\n",FindFileData.cFileName);
		UpdateData(FALSE);
		return;
	}

	filename.Format("%s",FindFileData.cFileName);

	while(1)
	{
		if(filename != "." && filename != "..")
		{
			pPath = path.Left(path.GetLength() - 1);
			pPath += FindFileData.cFileName;

			CString strCP;

			if( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 )
			{
				pPath += "\\*";
				
				strCP.Format("%s\\%s",cp,FindFileData.cFileName);
				FindAllFiles(pPath,FindFileData,(TCHAR*)(LPCTSTR)strCP);
			}
			else
			{
				const TCHAR * cptmp = _tcsrchr(pPath, _T('\\'));   //vc2019  TCHAR -> const TCHAR
				if (cptmp == NULL)
					cptmp = (TCHAR *)(const char *)pPath;
				else
					cptmp++;

				strCP.Format("%s\\%s",cp,cptmp);

				CString zipFile;
				zipFile.Format("%s\\user\\%s\\%s.Chart.zip", Axis::home, Axis::user, FindFileData.cFileName);

				HZIP hz = CreateZip((void *)(const char *) zipFile, 0, ZIP_FILENAME);

				const ZRESULT zrF = ZipAdd(hz,cp,0,0,ZIP_FOLDER);
				
				const ZRESULT zr = ZipAdd(hz, (TCHAR*)(LPCTSTR)strCP, (void *)(const char *)pPath, 0, ZIP_FILENAME);

				CloseZip(hz);

				if (zr == ZR_OK)
				{
					strTmp.Format("%s.Chart.zip",FindFileData.cFileName);

					m_sfilelist.Add(strTmp);				
					m_removelist.Add(zipFile);	
				}
			}
		}

		if(FindNextFile(hFind,&FindFileData) == FALSE)
		{
			const DWORD dwErr = GetLastError();

			if(dwErr != ERROR_NO_MORE_FILES)
			{
				strLog.Format("COMPRESS FOLDER ERROR : [ERROR_CODE] [%s]\n",dwErr);
			}

			break;
		}

		filename.Format("%s",FindFileData.cFileName);
	}
}

#pragma warning (default : 6067)

void CInfofile::CompressChart(CString path)
{
	if (!m_upload)
	{
		const int	ncnt = m_chartlist.GetSize();
		CString	filename, temp;

		if (!ncnt)
		{
			m_sfilelist.Add(CHART_OLD);
			filename.Format("%s\\%s", path, CHART_OLD);
			m_removelist.Add(filename);
		}
		else
		{
			for ( int ii = 0 ; ii < ncnt ; ii++ )
			{
				temp = m_chartlist.GetAt(ii);
				m_sfilelist.Add(temp);
				filename.Format("%s\\%s", path, temp);
				m_removelist.Add(filename);				
			}
		}
		return;
	}

	CString		zipfile;
	WIN32_FIND_DATA FindFileData;
	HANDLE		hFind;
	
	CString		allfile, filename;

	allfile.Format("%s\\*.zip", path);
	hFind = FindFirstFile(allfile, &FindFileData);
	
	while (hFind != INVALID_HANDLE_VALUE)
	{
		filename.Format("%s\\%s", path, FindFileData.cFileName);
		::DeleteFile(filename);
		if (!FindNextFile(hFind, &FindFileData))	break;
	}

	//**allfile.Format("%s\\*.c*", path);
	allfile.Format("%s\\IB7*.*", path);
	hFind = FindFirstFile(allfile, &FindFileData);
	
	while (hFind != INVALID_HANDLE_VALUE)
	{
		filename.Format("%s\\%s", path, FindFileData.cFileName);
		if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			const TCHAR * cp = _tcsrchr(filename, _T('\\'));  //vc2019  TCHAR -> const TCHAR
			
			if (cp == NULL)
				cp = (TCHAR *)(const char*)filename;
			else
				cp++;

			zipfile = Compress(cp, path);		// 사용자 환경설정
			
			if (!zipfile.IsEmpty())
			{
				m_sfilelist.Add(zipfile);				
				m_removelist.Add(filename + ".zip");	
			}	
		}

		if (!FindNextFile(hFind, &FindFileData))	break;
	}	
}

// void CInfofile::CompressDRFNChart(CString path)
// {
// 	if (!m_upload)
// 	{
// 		int	ncnt = m_chartlist.GetSize();
// 		CString	filename, temp;
// 
// 		if (!ncnt)
// 		{
// 			m_sfilelist.Add(CHART_OLD);
// 			filename.Format("%s\\%s", path, CHART_OLD);
// 			m_removelist.Add(filename);
// 		}
// 		else
// 		{
// 			for ( int ii = 0 ; ii < ncnt ; ii++ )
// 			{
// 				temp = m_chartlist.GetAt(ii);
// 				m_sfilelist.Add(temp);
// 				filename.Format("%s\\%s", path, temp);
// 				m_removelist.Add(filename);				
// 			}
// 		}
// 		return;
// 	}
// 
// 	CString		zipfile;
// 	WIN32_FIND_DATA FindFileData;
// 	HANDLE		hFind;
// 	
// 	CString		allfile, filename;
// 
// 	allfile.Format("%s\\*.zip", path);
// 	hFind = FindFirstFile(allfile, &FindFileData);
// 	
// 	while (hFind != INVALID_HANDLE_VALUE)
// 	{
// 		filename.Format("%s\\%s", path, FindFileData.cFileName);
// 		::DeleteFile(filename);
// 		if (!FindNextFile(hFind, &FindFileData))	break;
// 	}
// 
// 	//**allfile.Format("%s\\*.c*", path);
// 	allfile.Format("%s\\*.*", path);
// 	hFind = FindFirstFile(allfile, &FindFileData);
// 
// 	CString s;
// // 	s.Format("FIND FILE : %s\n",allfile);
// // 	OutputDebugString(s);
// 	
// 	while (hFind != INVALID_HANDLE_VALUE)
// 	{
// 		filename.Format("%s\\%s", path, FindFileData.cFileName);
// 
// // 		s.Format("FIND FILE : %s\n",filename);
// // 		OutputDebugString(s);
// 		if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
// 		{
// 			TCHAR * cp = _tcsrchr(filename, _T('\\'));
// 			
// 			if (cp == NULL)
// 				cp = (TCHAR *)(const char*)filename;
// 			else
// 				cp++;
// 
// 			zipfile = Compress(cp, path);		// 사용자 환경설정
// 			
// 			if (!zipfile.IsEmpty())
// 			{
// 				m_sfilelist.Add(zipfile);				
// 				m_removelist.Add(filename + ".zip");	
// 			}	
// 		}
// 
// 		if (!FindNextFile(hFind, &FindFileData))	break;
// 	}
// }
void CInfofile::CompressDRFNChart(CString path)
{
	if (!m_upload)
	{
		const int	ncnt = m_chartlist.GetSize();
		CString	filename, temp;

		if (!ncnt)
		{
			m_sfilelist.Add(CHART_OLD);
			filename.Format("%s\\%s", path, CHART_OLD);
			m_removelist.Add(filename);
		}
		else
		{
			for ( int ii = 0 ; ii < ncnt ; ii++ )
			{
				temp = m_chartlist.GetAt(ii);
				CString tmp;
// 				tmp.Format("CHART LIST : [%s] [%s]\n",path,temp);
// 				OutputDebugString(tmp);
				m_sfilelist.Add(temp);
				filename.Format("%s\\user\\%s\\%s", Axis::home, Axis::user, temp);

				m_removelist.Add(filename);				
			}
		}
		return;
	}

	CString		zipfile;
	WIN32_FIND_DATA FindFileData;
	HANDLE		hFind;
	
	CString		allfile, filename;

	allfile.Format("%s.zip", path);
	hFind = FindFirstFile(allfile, &FindFileData);
	
	if (hFind != INVALID_HANDLE_VALUE)
	{
		filename.Format("%s\\%s", path, FindFileData.cFileName);
		::DeleteFile(filename);
		//if (!FindNextFile(hFind, &FindFileData))	break;
	}

	//**allfile.Format("%s\\*.c*", path);
// 	allfile.Format("%s\\*.*", path);
// 	hFind = FindFirstFile(allfile, &FindFileData);

	CString s;
// 	s.Format("FIND FOLDER : %s\n",path);
// 	OutputDebugString(s);
	
	filename.Format("%s", path);
	
	// 		s.Format("FIND FILE : %s\n",filename);
	// 		OutputDebugString(s);
	
	zipfile = CompressFolder(path);		// 사용자 환경설정
	
// 	if (!zipfile.IsEmpty())
// 	{
// 		m_sfilelist.Add(zipfile);				
// 		m_removelist.Add(filename + ".zip");	
// 	}
}

void CInfofile::Decompress(CString target)
{
#ifdef _UNICODE
	ZIPENTRYW ze;
#else
	ZIPENTRY ze{};
#endif
	CString		zipFile, dir, srcfile;

	const int	pos = target.ReverseFind('\\');
	if (pos == -1)	return;
	dir = target.Left(pos);
	CString s;
// 	s.Format("DECOMPRESS : %s\n",target);
// 	OutputDebugString(s);
	// does zip source file exist?
	if (_taccess(target, 04))	return ;

	HZIP hz = OpenZip((char*)(const char*)target, 0, ZIP_FILENAME);

	if (hz)
	{
		ZRESULT zr = GetZipItem(hz, -1, &ze); 
		if (zr == ZR_OK)
		{
			const int itemCnt = ze.index;
			for (int ii = 0; ii < itemCnt; ii++)
			{
				GetZipItem(hz, ii, &ze);
				srcfile.Format("%s\\%s", dir, ze.name);

				srcfile.Replace('/','\\');

// 				s.Format("DECOMPRESS PATH : [%s]\n",srcfile);
// 				OutputDebugString(s);
				
				if(itemCnt > 1)
				{
					if(ii == 0)
					{
						CreateFolder(srcfile);
					}
					else
					{
						zr = UnzipItem(hz, ii, (char *)(const char*)srcfile, 0, ZIP_FILENAME);
					}
				}
				else
				{
					zr = UnzipItem(hz, ii, (char *)(const char*)srcfile, 0, ZIP_FILENAME);
				}
			}
			CloseZip(hz);
			::DeleteFile(target);
			SetApplyBits(ze.name);
		}
	}
	else
	{
		zipFile.Format("%s 압축해제 오류", target.Mid(pos + 1));
		//**AfxMessageBox(zipFile);
		Axis::MessageBox(zipFile);
	}
}

void CInfofile::CreateFolder(CString strTargetPath)
{
	if(_taccess(strTargetPath,0) != 0)
	{
		BOOL l_bExtractSucc = TRUE;

		int j = 0;

		CString strDirPath = _T("");
		CString strNowPath = _T("");

		while(l_bExtractSucc)
		{
			l_bExtractSucc = AfxExtractSubString(strNowPath,strTargetPath,j,'\\');

			strDirPath += strNowPath + _T("\\");

			if(l_bExtractSucc && _taccess(strDirPath,0) != 0)
			{
				CreateDirectory(strDirPath,NULL);
			}

			j++;
		}
	}
}

void CInfofile::SetApplyBits(CString fName)
{
	if (fName.Find(".ini") == -1 && fName.Find(".mmo") == -1)	 return;

	if (fName.Find("axisensetup.ini") != -1)
		m_applyBits |= APPLY_ENSETUP;
	else if (fName.Find("usertool.ini") != -1)
		m_applyBits |= APPLY_TOOLBAR;
	else if (fName.Find("axisticker.ini") != -1)
		m_applyBits |= APPLY_TICKER;
	else if (fName.Find("memo.mmo") != -1)
		m_applyBits |= APPLY_CODEMEMO;
	else
		m_applyBits |= APPLY_WORKSPACE;
}

BOOL CInfofile::OnEraseBkgnd(CDC* pDC) 
{
	if (Axis::dialogBgColor > 0)
	{
		CRect rc;
		GetClientRect(&rc);
		
		CBrush* oldBrush = pDC->SelectObject(&Axis::dialogBgBrush);

		pDC->PatBlt(0, 0, rc.Width(), rc.Height(), PATCOPY);
		pDC->SelectObject(oldBrush);

		return TRUE;
	}
	else
	{
		return CDialog::OnEraseBkgnd(pDC);
	}
}

HBRUSH CInfofile::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = Axis::GetCtlBrush(pWnd);

	pDC->SetBkMode(TRANSPARENT);
	if (hbr)
	{
		return hbr;
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CInfofile::OnDestroy() 
{
	CDialog::OnDestroy(); 
#ifndef DF_USE_CPLUS17
	delete m_changer;	
#endif
}

void CInfofile::OnOK() 
{
	OnAction();
}

