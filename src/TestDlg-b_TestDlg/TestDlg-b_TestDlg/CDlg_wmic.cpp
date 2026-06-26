// CDlg_wmic.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CDlg_wmic.h"
#include "afxdialogex.h"


// CDlg_wmic 대화 상자

IMPLEMENT_DYNAMIC(CDlg_wmic, CDialogEx)

CDlg_wmic::CDlg_wmic(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_WMIC, pParent)
{

}

CDlg_wmic::~CDlg_wmic()
{
}

void CDlg_wmic::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbKey);
	DDX_Control(pDX, IDC_COMBO2, m_cbVal);
}


BEGIN_MESSAGE_MAP(CDlg_wmic, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_wmic::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDOK, &CDlg_wmic::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_DLGWMIC, &CDlg_wmic::OnBnClickedBtnDlgwmic)
	ON_BN_CLICKED(IDC_BTN_DLGWMIC2, &CDlg_wmic::OnBnClickedBtnDlgwmic2)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlg_wmic 메시지 처리기

//struct st_wmic
//{
//	CString key;
//	CString keyval;
//} _stwmic[] =
//{
//	{"baseboard", "Manufacturer"},
//	{"baseboard", "Model"},
//	{"baseboard", "Name"},
//	{"baseboard", "PartNumber"},
//	{"baseboard", "slotlayout"},
//	{"baseboard", "serialnumber"},
//	{"baseboard", "poweredon"},
//	{"bios", "name"},
//	{"bios", "version"},
//	{"bios", "serialnumber"},
//
//};
//
//enum en_wmic{ 
//	baseboard_Manufacturer = 0,
//	baseboard_Model,
//	baseboard_Name,
//	baseboard_PartNumber,
//	baseboard_slotlayout,
//	baseboard_serialnumber,
//	baseboard_poweredon
//	
//};

void CDlg_wmic::initcombo()
{
	CString stmp;
	m_cbKey.GetWindowText(stmp);
	m_cbVal.ResetContent();

	int index = m_cbKey.GetCurSel();
	//if (stmp == "baseboard")
	if (index == 0)  //baseboard
	{
		m_mapWmic.SetAt("baseboard", "메인보드");
		m_cbVal.AddString("Manufacturer");
		m_cbVal.AddString("Model");
		m_cbVal.AddString("Name");
		m_cbVal.AddString("PartNumber");
		m_cbVal.AddString("slotlayout");
		m_cbVal.AddString("serialnumber");
		m_cbVal.AddString("poweredon");
	}
	else if (index == 1)  //bios
	{
		m_mapWmic.SetAt("bios", "바이오스");
		m_cbVal.AddString("name");
		m_cbVal.AddString("version");
		m_cbVal.AddString("serialnumber");
	}
	else if (index == 2) //bootconfig
	{
		m_mapWmic.SetAt("bootconfig", "부트 옵션");
		m_cbVal.AddString("BootDirectory");
		m_cbVal.AddString("Caption");
		m_cbVal.AddString("TempDirectory");
		m_cbVal.AddString("Lastdrive");
	}
	else if (index == 3) //cdrom
	{
		m_mapWmic.SetAt("cdrom", "시디롬");
		m_cbVal.AddString("Name");
		m_cbVal.AddString("Drive");
		m_cbVal.AddString("Volumename");
	}
	else if (index == 4)  //computersystem
	{
		m_cbVal.AddString("Name");
		m_cbVal.AddString("domain");
		m_cbVal.AddString("Manufacturer");
		m_cbVal.AddString("Model");
		m_cbVal.AddString("NmberofProcessors");
		m_cbVal.AddString("PrimaryOwnerName");
		m_cbVal.AddString("Username");
		m_cbVal.AddString("Roles");
		m_cbVal.AddString("totalphysicalmemory /format:list");
	}
	else if (index == 5)  //cpu
	{
		m_cbVal.AddString("Name");
		m_cbVal.AddString("Caption");
		m_cbVal.AddString("MaxClockSpeed");
		m_cbVal.AddString("DeviceID");
		m_cbVal.AddString("status");
	}
	else if (index == 6)  //datafile
	{//wmic datafile where Name='C:\\Windows\\System32\\notepad.exe' get Name,Version
		m_cbVal.AddString("where Name= 'C:\\IBKS\\IBK투자증권 HTS\\tab\\hjcode.dat' get Name, Version");
		m_cbVal.AddString("FileSize");
		m_cbVal.AddString("FileType");
		m_cbVal.AddString("InstallDate");
		m_cbVal.AddString("Readable");
		m_cbVal.AddString("Writeable");
		m_cbVal.AddString("System");
		m_cbVal.AddString("Version");
	}
	else if (index == 7)  //dcomapp
	{
		m_mapWmic.SetAt("dcomapp", "DCOM - 분산 컴포넌트 객체 모델의  AppID(고유식별자)");
		m_cbVal.AddString("Name");
		m_cbVal.AddString("AppID /format:list");
	}
	else if (index == 8)  //desktop
	{
		m_mapWmic.SetAt("desktop", "바탕화면");
		m_cbVal.AddString("Name");
		m_cbVal.AddString("ScreenSaverExecutable");
		m_cbVal.AddString("ScreenSaverActive");
		m_cbVal.AddString("Wallpaper /format:list");
	}
	else if (index == 9)  //desktopmonitor
	{
		m_mapWmic.SetAt("desktopmonitor", "모니터 정보");
		m_cbVal.AddString("screenheight");
		m_cbVal.AddString("screenwidth");
	}
	else if (index == 10)  //diskdrive
	{
		m_mapWmic.SetAt("diskdrive", "");
		m_cbVal.AddString("Name");
		m_cbVal.AddString("Manufacturer");
		m_cbVal.AddString("Model");
		m_cbVal.AddString("InterfaceType");
		m_cbVal.AddString("MediaLoaded");
		m_cbVal.AddString("MediaType");
	}
	else if (index == 11)  //diskquota
	{
		m_mapWmic.SetAt("diskquota", "");
		m_cbVal.AddString("User");
		m_cbVal.AddString("Warninglimit");
		m_cbVal.AddString("DiskSpaceUsed");
		m_cbVal.AddString("QuotaVolume");
	}
	else if (index == 12)  //environment
	{
		m_mapWmic.SetAt("environment", "");
		m_cbVal.AddString("Description");
		m_cbVal.AddString("VariableValue");
	}
	else if (index == 13)  //fsdir
	{
		m_mapWmic.SetAt("fsdir", "where name='c:\windows' get Archice");
		m_cbVal.AddString("Description");
		m_cbVal.AddString("VariableValue");
	}
	else if (index == 14)  //group
	{
	m_mapWmic.SetAt("group", "");
	m_cbVal.AddString("Caption");
	m_cbVal.AddString("InstallDate");
	m_cbVal.AddString("LocalAccount");
	m_cbVal.AddString("LocalAccount");
	}
	else if (index == 37)  //product
	{
		m_mapWmic.SetAt("product", "");
		m_cbVal.AddString("Description");
		m_cbVal.AddString("InstallDate");
		m_cbVal.AddString("Name");
		m_cbVal.AddString("Vendor");
		m_cbVal.AddString("Version");
	}
	else if (index == 38)  //qfe
	{
		m_mapWmic.SetAt("qfe", "e");
		m_cbVal.AddString("description");
		m_cbVal.AddString("FixComments");
		m_cbVal.AddString("HotFixID");
		m_cbVal.AddString("InstalledBy");
		m_cbVal.AddString("InstalledOn");
		m_cbVal.AddString("ServicePackInEffect");
	}


	m_cbVal.Invalidate();
	m_cbVal.SetCurSel(0);
}

BOOL CDlg_wmic::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_cbKey.AddString("baseboard");				//0
	m_cbKey.AddString("bios");							//1
	m_cbKey.AddString("bootconfig");				//2
	m_cbKey.AddString("cdrom");						//3
	m_cbKey.AddString("computersystem");		//4
	m_cbKey.AddString("cpu");							//5
	m_cbKey.AddString("datafile");					//6
	m_cbKey.AddString("dcomapp");				//7
	m_cbKey.AddString("desktop");					//8
	m_cbKey.AddString("desktopmonitor");		//9
	m_cbKey.AddString("diskdrive");					//10
	m_cbKey.AddString("diskquota");				//11
	m_cbKey.AddString("environment");			//12
	m_cbKey.AddString("fsdir");							//13
	m_cbKey.AddString("group");						//14
	m_cbKey.AddString("irq");							//15
	m_cbKey.AddString("job");							//16
	m_cbKey.AddString("loadorder");				//17
	m_cbKey.AddString("logicaldisk");				//18
	m_cbKey.AddString("memcache");				//19
	m_cbKey.AddString("memlogical");				//20
	m_cbKey.AddString("memphysical");			//21
	m_cbKey.AddString("netclient");					//22
	m_cbKey.AddString("netlogin");					//23
	m_cbKey.AddString("netprotocol");				//24
	m_cbKey.AddString("netuse");						//25
	m_cbKey.AddString("nic");							//26
	m_cbKey.AddString("nicconfig");					//27
	m_cbKey.AddString("ntdomain");				//28
	m_cbKey.AddString("ntevent");					//29
	m_cbKey.AddString("os");							//30	
	m_cbKey.AddString("pagefile");					//31
	m_cbKey.AddString("pagefileset");				//32
	m_cbKey.AddString("partition");					//33
	m_cbKey.AddString("printer");						//34
	m_cbKey.AddString("printjob");					//35
	m_cbKey.AddString("process");					//36
	m_cbKey.AddString("product");					//37
	m_cbKey.AddString("qfe");							//38
	m_cbKey.AddString("quotasetting");			//39
	m_cbKey.AddString("recoveros");				//40
	m_cbKey.AddString("Registry");					//41
	m_cbKey.AddString("scsicontroller");			//42
	m_cbKey.AddString("server");						//43
	m_cbKey.AddString("service");						//44
	m_cbKey.AddString("share");						//45
	m_cbKey.AddString("sounddev");				//46
	m_cbKey.AddString("startup");					//47
	m_cbKey.AddString("sysaccount");				//48
	m_cbKey.AddString("sysdriver");					//49
	m_cbKey.AddString("systemenclosure");		//50
	m_cbKey.AddString("systemslot");				//51
	m_cbKey.AddString("tapedrive");					//52
	m_cbKey.AddString("timezone");					//53
	m_cbKey.AddString("useraccount");			//54





	m_cbKey.SetCurSel(0);
	initcombo();


	CRect rec;
	((CWnd*)GetDlgItem(IDC_RECT_IMG))->ShowWindow(SW_HIDE);

	CString m_strimagepath;
	m_strimagepath = "F:\\util\\HTS\\IBK투자증권 HTS\\image\\event01.bmp";
	HBITMAP hBitmap;

	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_strimagepath,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)
		m_pClearBmp = CBitmap::FromHandle(hBitmap);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void CDlg_wmic::SearchWmic(CString skey, CString sval)
{
	CString command, stmp, stemp;
	command.Format(" /C wmic %s get %s", skey, sval);
	CString cmdLine = _T("cmd.exe") + command;

	FILE* pipe = _popen(cmdLine, "rt");

	if (pipe) {
		char buffer[128];
		//((CWnd*)GetDlgItem(IDC_EDIT_WMIC))->SetWindowText("");
		if (m_mapWmic.Lookup(skey, stmp))
		{
			stemp += stmp;
			stemp += "\r\n";
			((CWnd*)GetDlgItem(IDC_EDIT_WMIC))->SetWindowText(stemp);
		}
		while (!feof(pipe)) {
			if (fgets(buffer, 128, pipe) != NULL) {
				((CWnd*)GetDlgItem(IDC_EDIT_WMIC))->GetWindowText(stemp);
				stmp.Format("%s", buffer);
				stmp.Trim();
				stemp += "\r\n";
				stemp += stmp;
				((CWnd*)GetDlgItem(IDC_EDIT_WMIC))->SetWindowText(stemp);
				OutputDebugString(stmp);
			}
		}
	}
}

void CDlg_wmic::OnCbnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	initcombo();
}


void CDlg_wmic::OnCbnSelchangeCombo2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDlg_wmic::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CDlg_wmic::OnBnClickedBtnDlgwmic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strkey, strval;
	m_cbKey.GetWindowText(strkey);
	m_cbVal.GetWindowText(strval);
	SearchWmic(strkey, strval);
}


void CDlg_wmic::OnBnClickedBtnDlgwmic2()
{
	//command.Format(" /C wmic %s get %s", skey, sval);
	CString strCmd = _T("fsdir where name='c:\\windows' get CreationDate");
	CString command, stmp, stemp;
	command.Format(" /C wmic %s", strCmd);
	CString cmdLine = _T("cmd.exe") + command;

	FILE* pipe = _popen(cmdLine, "rt");

	if (pipe) {
		char buffer[128];
		((CWnd*)GetDlgItem(IDC_EDIT_WMIC))->SetWindowText("");
		while (!feof(pipe)) {
			if (fgets(buffer, 128, pipe) != NULL) {
				stmp.Format("%s", buffer);
				stmp.Trim();
				((CWnd*)GetDlgItem(IDC_EDIT_WMIC))->GetWindowText(stemp);
				stemp += "\r\n";
				stemp += stmp;
				((CWnd*)GetDlgItem(IDC_EDIT_WMIC))->SetWindowText(stemp);
				OutputDebugString(stmp);
			}
		}
	}
}

// wmic datafile where Name="C:\\Windows\\System32\\notepad.exe" get Name,Version  //cmd에서 성공
//wmic fsdir where name="c:\\windows" get Archice, CreationDate, LastModified, Readable, Writeable, System, Hidden, Status
//wmic get product Description


void CDlg_wmic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	CRect rec;
	((CWnd*)GetDlgItem(IDC_RECT_IMG))->GetClientRect(rec);
	
	if (m_pClearBmp == NULL)
		return;

	CDC memDC;
	CBitmap* pOldBmp = nullptr;
	BITMAP bmpInfo;
	memDC.CreateCompatibleDC(&dc);
	m_pClearBmp->GetBitmap(&bmpInfo);
	dc.SetStretchBltMode(HALFTONE);
	rec.right += 200;
	rec.bottom += 200;
	if (memDC.m_hDC != NULL)
	{
		pOldBmp = (CBitmap*)memDC.SelectObject(m_pClearBmp);
		dc.StretchBlt(rec.left, rec.top, rec.Width(), rec.Height(),
			&memDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
	}

	//dc.FillSolidRect(rec, RGB(255, 0, 0));

	if (pOldBmp)
		memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
}
