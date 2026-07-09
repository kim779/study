// login.cpp : implementation file
//

#include "stdafx.h"
#include "axislogin.h"
#include "login.h"
#include "sk_pc_identity.h"
#include "../../h/axis.h"
#include "../../h/axisfire.h"
#include "../../h/axisVar.h"

#include <TlHelp32.h>

void CSortStringArray::Sort()
{
    BOOL bNotDone = TRUE;
    
    while (bNotDone)
    {
        bNotDone = FALSE;
        for(int pos = 0;pos < GetUpperBound();pos++)
            bNotDone |= CompareAndSwap(pos);
    }
}

BOOL CSortStringArray::CompareAndSwap(int pos) {
    CString vc_b;
    int posFirst = pos;
    int posNext = pos + 1;
    
    if (GetAt(posFirst).CompareNoCase(GetAt(posNext)) > 0)
    {
        vc_b = GetAt(posFirst);
        SetAt(posFirst, GetAt(posNext));
        SetAt(posNext, vc_b);
        return TRUE;
    }
    return FALSE;
}

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  IPCCINOUTBOUND		1
#define  IPCCFAX		2
#define  IPCCEMAIL		3

#define WRAP_WHOI		1
#define WRAP_MANAGE		2
#define WRAP_ADMIN		3
#define WRAP_SUPPORT		4
#define WRAP_MANAGERSPON	5
#define WRAP_ADMINIRESPON	6
#define WRAP_INQUIR		7
#define USER_BlACKUSER		8
#define USER_BLACKUSETIME	9
#define USER_ORDER		10
#define USER_USID		11
#define USER_NAME		12
#define USER_DEPT		13
#define USER_TERM		14
#define USER_SVNO		15
#define USER_WHOI		16
#define USER_PSWD		17
#define USER_COMNO		18
#define USER_COMNAME		19
#define USER_DEPTNAME		20
#define USER_IDNO		21
#define USER_AUTH		22
#define USER_MGRAPPRIP		23
#define USER_MGRAPPRTERM	24
#define USER_DATE		25
#define USER_XDEF2		26
#define USER_COMGB		27
#define USER_INPUTGB		28
#define USER_CHK		29
#define USER_SVNM		30
#define USER_LCOMM		31
#define USER_LDATE		32
#define USER_LTIME		33
#define USER_LTODAY		34
#define USER_STORE		35
#define USER_DEPTTP		36
#define USER_BSNSTP		37
#define USER_MLOGIN		38
#define USER_DEPTTEl	39
#define USER_APPRYN		40
#define USER_CHANNEL	41
#define USER_INITGB		42
#define USER_CIF		43

#define L_ipcc			128

/////////////////////////////////////////////////////////////////////////////
// CLogin

IMPLEMENT_DYNCREATE(CLogin, CCmdTarget)

CLogin::CLogin()
{
	EnableAutomation();
	clear();

	m_strRegKey = "";
}

CLogin::~CLogin()
{
}


void CLogin::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CLogin, CCmdTarget)
	//{{AFX_MSG_MAP(CLogin)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CLogin, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CLogin)
	
	DISP_PROPERTY_EX(CLogin, "Usid", _getUsid, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Name", _getUsnm, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "bExpert", _getbExpert, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "bBondEx", _getbBondEx, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "bPassChange", _getbPassChange, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "bSameConnect", _getbSameConnect, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "btmpuse", _getbtmpuse, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "bUserst", _getbUserst, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "deptNm", _getdeptNm, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "bnotice", _getbnotice, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "buuidrec", _getbuuidrec, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "reserve", _getreserve, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "m_errc", _getm_errc, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "ldate", _getldate, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "ltime", _getltime, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "comGb", _GetcomGb, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "upGb", _GetupGb, SetNotSupported, VT_BSTR)
	//DISP_PROPERTY_EX_ID(CLogin, "auth", dispidauth, Getauth, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "auth",Getauth, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "compNm",GetcompNm, SetNotSupported, VT_BSTR)


	DISP_PROPERTY_EX(CLogin, "Order", _getOrder, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX(CLogin, "Term", _getTerm, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "XDef2", _getDef2, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Chk", _GetChk, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Svnm", _GetSvnm, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "lComm", _getlComm, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "lDate", _getllDate, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Today", _getToday, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "W_Manage", _getManage, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "W_Admin", _getAdmin, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "W_Support", _getSupport, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "W_Managerespon", _getManagerespon, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "W_Adminrespon", _getAdminrespon, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "W_Inquir", _getInquir, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "W_WrapWhoi", _WrapWhoi, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "BlackUserTime", _getBlackUserTime, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "BlackUser", _getBlackUser, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "CallLevel", _getCallLevel, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "IPCCInOut", _getIPCCInOut, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "IPCCFax", _getIPCCFax, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "IPCCSms", _getIPCCSms, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "IPCCEMail", _getIPCCEmail, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Item", _getItem, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "StoreGb", _getStoreGb, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "MacAddr", _getMacAddr, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "ConnArea", GetConnArea, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "DeptTP", GetDeptTP, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "BsnsTP", GetBsnsTP, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "DeptTel", GetDeptTel, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "ApprYN", GetApprYN, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Channel", GetChannel, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLogin, "Cif", GetCif, SetNotSupported, VT_BSTR)
	DISP_FUNCTION(CLogin, "ChangeInfo", _ChangeInfo, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CLogin, "WriteReg", _WriteReg, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
	
END_DISPATCH_MAP()

// Note: we add support for IID_ILogin to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {DEE758DA-505A-48FF-87CD-4E4CD82266A8}
static const IID IID_ILogin =
{ 0xdee758da, 0x505a, 0x48ff, { 0x87, 0xcd, 0x4e, 0x4c, 0xd8, 0x22, 0x66, 0xa8 } };

BEGIN_INTERFACE_MAP(CLogin, CCmdTarget)
	INTERFACE_PART(CLogin, IID_ILogin, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogin message handlersd

UINT Thread_KISA(LPVOID pParam)
{
	CLogin *pLogin = (CLogin*)pParam;
	
	pLogin->OnOpenUrl(pLogin->m_term);
	return 0;
}

ULONG CLogin::ProcIDFromWnd(HWND hwnd)
{
	ULONG idProc;
	::GetWindowThreadProcessId(hwnd, &idProc);
	return idProc;
}

HWND CLogin::GetWinHandle(ULONG pid)
{
	HWND tempHwnd = ::FindWindowA(NULL, NULL); //최상위 윈도우 핸들 찾기
	while(tempHwnd != NULL)
	{
		if(::GetParent(tempHwnd) == NULL)
		{
			if(pid == ProcIDFromWnd(tempHwnd))
			{
				return tempHwnd;
			}
		}
		tempHwnd = ::GetWindow(tempHwnd, GW_HWNDNEXT);
	}
	return 0;
}


void CLogin::SendDataToMain(char* pdata)
{
	//class CMainFrame *pWnd = (class CMainFrame*)AfxGetMainWnd();
	//(CMainFrame*)pWnd->SetLogindata(pdata);
	//pWnd->SendMessage( WM_USER + 500, 9892 ,(LPARAM)pdata);.
	//CWnd *pWnd = AfxGetMainWnd();
	 //CAxisApp* papp =  AfxGetMainWnd()->Af;
	// CopyMemory(papp->m_pSignR, pdata, L_signR);
	return;
}

CString CLogin::SetLogin(char* data)
{
	int pos = 0;
	CString	string, text, info, tmpS, userdata;
	struct	_signR*	signR = (struct _signR *)data;

	
	tmpS.Format(_T("@@axislogin SetLogin mapN[8]=[%.8s]\n sign[30]=[%.30s]\n name[30]=[%.30s]\n menu[12]=[%.12s]\n trx[3]=[%.3s]\n usage[3]=[%.3s]\n idle[3]=[%.3s]\n info[64]=[%.64s]\n infox[192]=[%.192s]\n"), 
																   signR->mapN, signR->sign, signR->name, 
																   signR->menu, signR->trx, signR->usage, signR->idle, signR->info, signR->infox);
	OutputDebugString(tmpS);

	CAxisloginApp* app = (CAxisloginApp *)AfxGetApp();

	text = CString(signR->sign, sizeof(signR->sign));
	pos  = text.Find('\0');
	if (pos != -1)
		text = text.Left(pos);
	text.TrimRight();
	m_user = text;

	if(m_user.IsEmpty())
	{

	}

	text = CString(signR->name, sizeof(signR->name));
	pos  = text.Find('\0');
	if (pos != -1)
		text = text.Left(pos);
	text.TrimRight();
	if (!text.IsEmpty())
		m_name = text;
	else
		m_name = _T("Anonymous");

	app->m_user	= m_user;
	app->m_name	= m_name;


tmpS.Format(_T("axislogin signR_ID=%s, signR_name=%s  m_user=%s m_name=%s \n"), 
	CString(signR->sign, sizeof(signR->sign)), CString(signR->name, sizeof(signR->name)), m_user, m_name);
OutputDebugString(tmpS);

	TCHAR szPath[MAX_PATH];
	memset(szPath,0x00, sizeof(szPath));
	GetModuleFileName(NULL, szPath, MAX_PATH);

	CString strRegKey("");
	strRegKey.Format("%s",szPath);
	strRegKey = strRegKey.Mid(0,strRegKey.ReverseFind('\\'));
	strRegKey = strRegKey.Mid(0,strRegKey.ReverseFind('\\'));
	strRegKey = strRegKey.Mid(strRegKey.ReverseFind('\\')+1);

#ifdef _DEBUG
	strRegKey = _T("TBOARD_WORK");
#else
	strRegKey = _T("KBond");
#endif
	//HKEY	hTKey;
//	CString	SubKey;
	//SubKey.Format("Software\\%s\\%s", strRegKey,AXISNAME);

	/*
	if ( RegOpenKeyEx( HKEY_CURRENT_USER, SubKey, 0, KEY_READ, &hTKey ) != ERROR_SUCCESS ) {
		CWnd *pWnd = AfxGetMainWnd();
		if (pWnd != NULL)
			pWnd->GetWindowText(strRegKey);
		if (!strRegKey.IsEmpty()){
			strRegKey.TrimRight();
			strRegKey.TrimLeft();
		}
		if (strRegKey == "싸이칸 Plus")
			strRegKey = "cyKhanPlus";
		else if (strRegKey == "싸이칸 Plus TEST")
			strRegKey = "cyKhanPlusTest";
		else if (strRegKey == "CallPro")
			strRegKey = "cyKhanCallPro";
		else if (strRegKey == "CallPro TEST")
			strRegKey = "cyKhanCallProTest";
		else if (strRegKey == "HiNuri")
			strRegKey = "HiNuri";
		else if (strRegKey == "HiNuriTest")
			strRegKey = "HiNuriTest";
		else
			strRegKey = "";
	}
	*/

	if (strRegKey.IsEmpty())
		strRegKey = AXISKEY;

	if (data == NULL || strlen(data) <= 0)
	{
		clear();
		return _T("");
	}
	OutputDebugString("@@" + strRegKey);
	m_strRegKey = strRegKey;

	clear();
	//m_order = (signR->flag & flagACN) ? FALSE : TRUE;

	text = CString(signR->sign, sizeof(signR->sign));
	pos  = text.Find('\0');
	if (pos != -1)
		text = text.Left(pos);
	text.TrimRight();
	m_user = text;
	m_user.TrimLeft();
	m_user.TrimRight();

	text = CString(signR->name, sizeof(signR->name));
	pos  = text.Find('\0');
	if (pos != -1)
		text = text.Left(pos);
	text.TrimRight();
	if (!text.IsEmpty())
		m_name = text;
	else
		m_name = _T("Anonymous");
	m_name.TrimLeft();
	m_name.TrimRight();

	info = CString(signR->info, sizeof(signR->info));
	text = CString(signR->infox, sizeof(signR->infox));
	text.TrimRight();

	//text = text.Left(60);  //아이와 사용자명까지 다 infox로 내려온경우  (emsg (200)은 제외)


	struct loginmod //사용한 화면
	{
		char userid[30];
		char name[30];
		char auth[7];
		char comp[6];
		char upjong[2];
		char expivt[1];
		char dept[6];
		char bonddealer[1];
		char pwch[1];
		char dupc[1];
		char tmpuse[1];
		char userst[1];
		char emailfrm[1];
		char notice[1];
		char uudierec[1];
		char reserve[20];
		char errc[1];
		//char emsg[200];
		char ldat[8];
		char ltim[6];
		char compNm[50];
	};

	struct loginmod* plogout;
	plogout = (struct loginmod*)text.operator LPCSTR();
	m_auth = CString(plogout->auth, 7).TrimRight();
	m_comGb = CString(plogout->comp, 6).TrimRight();
	m_upGb = CString(plogout->upjong, 2).TrimRight();
	m_bExpert = CString(plogout->expivt, 1).TrimRight();
	m_deptNm = CString(plogout->dept, 6).TrimRight();
	m_bBondEx = CString(plogout->bonddealer, 1).TrimRight();
	m_bPassChange = CString(plogout->pwch, 1).TrimRight();
	m_bSameConnect = CString(plogout->dupc, 1).TrimRight();
	m_btmpuse = CString(plogout->tmpuse, 1).TrimRight();
	m_bUserst = CString(plogout->userst, 1).TrimRight();
	m_bemailfrm = CString(plogout->emailfrm, 1).TrimRight();
	m_bnotice = CString(plogout->notice, 1).TrimRight();
	m_buuidrec = CString(plogout->uudierec, 1).TrimRight();
	m_reserve = CString(plogout->reserve, 20).TrimRight();
	m_errc = CString(plogout->errc, 1).TrimRight();
	m_ldate = CString(plogout->ldat, 8).TrimRight();
	m_ltime = CString(plogout->ltim, 6).TrimRight();
	m_strcompNm = CString(plogout->compNm, 50).TrimRight();

	CString strinfo;
	strinfo.Format(_T("아이디=[%s]\n이름=[%s]\n권한=[%s]\n회사코드=[%s]\n업종=[%s]\n전문투자자여부=[%s]\n부서=[%s]\n채권전문딜러여부=[%s]\n비멀번호변경=[%s]\n동시접속여부=[%s]\n임시사용여부=[%s]\n이용제한요청여부=[%s]\n이메일=[%s]\n공지팝업유무=[%s]\n사용자고유키여부=[%s]\n예비용=[%s]\n에러코드=[%s]\n에러메시지=[%s]\n날짜=[%s]\n시간=[%s]\n"),
				m_user, m_name, m_auth, m_comGb, m_upGb, m_bExpert, m_deptNm, m_bBondEx, m_bPassChange, m_bSameConnect,
				m_btmpuse, m_bUserst, m_bemailfrm, m_bnotice,m_buuidrec, m_reserve, m_errc, m_errmsg, m_ldate, m_ltime);
	OutputDebugString("@@" + strinfo);
	/*
	TRACE(_T("\n\n===================================\n"));
	CString strresult;
	for (int ii = 0; !text.IsEmpty(); ii++)
	{
		pos = text.Find('\t');
		if (pos != -1)
		{
			string = text.Left(pos);
			text   = text.Mid(pos+1);
		}
		else
		{
			string = text;
			text   = _T("");
		}

		switch (ii)
		{
		case 0:
			m_auth =  string;	 break;    //사용자권한 7
		case 1:
			m_comGb = string;	 break;    //회사코드 6
		case 2:
			m_upGb = string;	 break;    //업종 2
		case 3:
			m_bExpert = string;	break;     //전문투자자 여부 1
		case 4:
			{
				m_deptNm  = string;        //부서명 6
			}
			break;
		case 5:
			m_bBondEx = string;	break;    //채권전문딜러 여부 1
		case 6:
			m_bPassChange = string;	break;    //비밀번호변경 1 (1 : 변경필요)
		case 7:
			m_bSameConnect = string;	break;   //동시접속 여부 1
		case 8:
			m_btmpuse = string;	break;    //임시사용여부 1
		case 9:
			m_bUserst   = string;	break;  //이용제한 요청여부 1
		case 10:
			m_bemailfrm  = string;	break;  //email  activate 여부 1
		case 11:
			m_bnotice   = string;	break;  //공지팝업 존재 유무 1
		case 12:
			m_buuidrec    = string;	break;  //사용자 고유키 재생성 여부 1
		case 13:
			m_reserve   = string;	break;  //예비용 20
		case 14:
			m_errc = string;	break;   //에러코드 1
		case 15:
			m_ldate    = string;	break;  //날짜 8
		case 16:
			m_ltime   = string;   break;   //시간 8
		default:
			break;
		}
		userdata += string;
		userdata += _T("\t");
		tmpS.Format(_T("%s = [%s] \n"), arr.GetAt(ii), string);
		TRACE(tmpS);
		strresult += string;
		strresult += _T("\n");
	}
	*/
	TRACE(_T("========================================\n\n"));  
	
	SendDataToMain((char*)signR);
	
#ifdef not_onlyfits
	m_whoiNinfo = m_whoi;
	m_whoiNinfo.Insert(999, "\t" + usrAuth);

	bool	bIpcc = false;
	//CString	isIPCC;
	HKEY	hKey;
	LONG	res;
	CString sSubKey;
	
	sSubKey.Format("Software\\%s\\%s\\%s", strRegKey, AXISNAME, (LPCTSTR)"IPCC");
	if (RegOpenKeyEx(HKEY_CURRENT_USER, sSubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		unsigned char szVal[100];
		DWORD dwType = REG_DWORD; 
		memset(szVal, 0x20, sizeof(szVal));
		DWORD dwSize = sizeof(szVal);

		res = RegQueryValueEx(hKey, _T("IPCC"), NULL, &dwType, (LPBYTE)szVal, &dwSize);

		if ((res == ERROR_SUCCESS) && (dwSize > 0))
		{
			bIpcc = true;		
		}
	}

	RegCloseKey(hKey);
//////////////////////////////////////////////////////////////////////////
	CString sIPCC = _T("");
	
	m_whoiNinfo.Insert(999, "\t" + sIPCC);
//////////////////////////////////////////////////////////////////////////
	app->m_user	= m_user;
	app->m_name	= m_name;
	app->m_dept	= m_dept;
	app->m_term	= m_term;
	app->m_whoi	= m_whoiNinfo;
	app->m_wrapwhoi = m_wrapwhoi;

	sSubKey.Format("Software\\%s\\%s\\%s", strRegKey, AXISNAME, INFORMATION);
TRACE("[LOGIN_TiTlestrRegKey]%s\t%s\n", strRegKey, sSubKey);
	WriteReg(strRegKey, sSubKey);

//	if (RegOpenKeyEx(HKEY_CURRENT_USER, sSubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
//	{
// 		unsigned char szVal[100];
// 
// 		lstrcpy((char *)szVal, LPCTSTR(m_deptNm)); 
// 		RegSetValueEx(hKey, (LPCTSTR)"DEPTNM", 0, REG_SZ, szVal, m_deptNm.GetLength());
// 
// 		lstrcpy((char *)szVal, LPCTSTR(m_dept));
// 		RegSetValueEx(hKey, (LPCTSTR)"DEPTID", 0, REG_SZ, szVal, m_dept.GetLength());
// 
// 		lstrcpy((char *)szVal, LPCTSTR(m_svNm));
// 
// 		RegSetValueEx(hKey, (LPCTSTR)"SVRNM", 0, REG_SZ, szVal, m_svNm.GetLength());
// 
// 		lstrcpy((char *)szVal, LPCTSTR(m_multiLogin)); 
// 		RegSetValueEx(hKey, (LPCTSTR)"MultiLoginOut", 0, REG_SZ, szVal, m_multiLogin.GetLength());
// 
// 		lstrcpy((char *)szVal, LPCTSTR(m_ApprYN)); 
// 		RegSetValueEx(hKey, (LPCTSTR)"ApprYN", 0, REG_SZ, szVal, m_ApprYN.GetLength());
// 
// 		if (strRegKey == "HiNuri" || strRegKey == "HiNuriTest")
// 		{
// 			m_Channel = "HN";
// 			lstrcpy((char *)szVal, LPCTSTR(m_Channel));
// 			RegSetValueEx(hKey, (LPCTSTR)"Channel", 0, REG_SZ, szVal, m_Channel.GetLength());
// 		}
//
//		RegCloseKey(hKey);
//	} 
//	else
//		  TRACE("CLogin::SetLogin() RegOpenKeyEx  Failed");
	
	CString path, home;
	DWORD	v_type;
	char	twb[256];

	DWORD	len = sizeof(twb);
	sSubKey.Format("Software\\%s\\%s\\%s", strRegKey, AXISNAME, ENVIRONMENT);
	if (RegOpenKeyEx(HKEY_CURRENT_USER, sSubKey, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return m_whoiNinfo;
	}
	
	if ((RegQueryValueEx(hKey, "RootDir", NULL, &v_type, (LPBYTE)twb, &len) != ERROR_SUCCESS))
	{
		RegCloseKey(hKey);
		return m_whoiNinfo;
	}
	
	path.Format("%s\\%s\\AXIS.ini", twb, TABDIR);
	
	if (!m_item.CompareNoCase("Y"))
		WritePrivateProfileString("ACCN", "ITEM", "1", path);
	else
		WritePrivateProfileString("ACCN", "ITEM", "0", path);
	
	if (atoi(m_whoi) == 0)
		WritePrivateProfileString("WHOIGLB", "STAF", m_mgrIp, path);
	else
		WritePrivateProfileString("WHOIGLB", "USER", m_whoi, path);

	RegCloseKey(hKey);
#endif
	return _T("");
}

void CLogin::WriteReg(CString strRegKey, CString sSubKey)
{
	//HKEY	hKey;
	/*
	if (RegOpenKeyEx(HKEY_CURRENT_USER, sSubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		unsigned char szVal[100];

		lstrcpy((char *)szVal, LPCTSTR(m_deptNm)); 
		RegSetValueEx(hKey, (LPCTSTR)"DEPTNM", 0, REG_SZ, szVal, m_deptNm.GetLength());

		lstrcpy((char *)szVal, LPCTSTR(m_dept));
		RegSetValueEx(hKey, (LPCTSTR)"DEPTID", 0, REG_SZ, szVal, m_dept.GetLength());

		lstrcpy((char *)szVal, LPCTSTR(m_svNm));

		RegSetValueEx(hKey, (LPCTSTR)"SVRNM", 0, REG_SZ, szVal, m_svNm.GetLength());

		lstrcpy((char *)szVal, LPCTSTR(m_multiLogin)); 
		RegSetValueEx(hKey, (LPCTSTR)"MultiLoginOut", 0, REG_SZ, szVal, m_multiLogin.GetLength());

		lstrcpy((char *)szVal, LPCTSTR(m_ApprYN)); 
		RegSetValueEx(hKey, (LPCTSTR)"ApprYN", 0, REG_SZ, szVal, m_ApprYN.GetLength());

		if (strRegKey == "HiNuri" || strRegKey == "HiNuriTest")
		{
			m_Channel = "HN";
			lstrcpy((char *)szVal, LPCTSTR(m_Channel));
			RegSetValueEx(hKey, (LPCTSTR)"Channel", 0, REG_SZ, szVal, m_Channel.GetLength());
		}
	
		lstrcpy((char *)szVal, LPCTSTR(m_introTP)); 
		RegSetValueEx(hKey, (LPCTSTR)"introTP", 0, REG_SZ, szVal, m_introTP.GetLength());

		RegCloseKey(hKey);
	} else
		  TRACE("CLogin::SetLogin() RegOpenKeyEx  Failed");
		  */
}

// #include <Iphlpapi.h>
// #pragma comment (lib, "IpHlpApi.lib")
void CLogin::GetMacAddr(CString& data)
{
	int ret;
	CString sMac;
	PC_INFO_CONTEXT	pContext;
	
	memset(&pContext, 0x00, sizeof(PC_INFO_CONTEXT));
	
	ret = GetPCIdentity(&pContext, 13);
	
	if (ret == P_PARAMETER_IS_NULL)
		data = "";
	
	if (ret == P_OK)
	{
		sMac = pContext.szPhysicalMAC;
		data = sMac;
	}

// 	CString checkIP("");
// 	CString strMac("");
// 	CSortStringArray strArr;
// 	CString MacName;
// 	BYTE outBuf[256]= {0};
// 	DWORD BytesReturned;
// 	HANDLE hDev;
// 	CArrayAdpInfo aryAdpInfo;
// 	int OID_802_3_PERMANENT_ADDRESS = 0x01010101;
// 	DWORD IOCTL_NDIS_QUERY_GLOBAL_STATS  = 0x00170002;
// 	int iEthernet = 0;
// 	int iWireless = 0;
// 	IP_ADAPTER_INFO AdapterInfo[16];
// 	DWORD dwBufLen = sizeof(AdapterInfo);
// 	DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
// 	if (dwStatus == ERROR_SUCCESS)
// 	{
// 		for (int ii = 0; ii < 16; ii++)
// 		{
// 			checkIP = (((AdapterInfo[ii]).IpAddressList).IpAddress).String;
// 			if(checkIP == ""){
// 				continue;
// 			}
// 			MacName.Format("\\\\.\\%s",(AdapterInfo[ii]).AdapterName);
// 			hDev = CreateFile(MacName,
// 				GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
// 				NULL, OPEN_EXISTING, 0, 0);
// 			
// 			if (hDev == INVALID_HANDLE_VALUE){
// 				continue;
// 			}
// 			memset(outBuf,0,256);
// 			if(DeviceIoControl(hDev, IOCTL_NDIS_QUERY_GLOBAL_STATS, (LPVOID)&OID_802_3_PERMANENT_ADDRESS,4,outBuf,256,&BytesReturned,NULL))
// 			{
// 				struct	_AdapterInfo AdpInfo;
// 				sprintf(AdpInfo.sMac,"%.2X%.2X%.2X%.2X%.2X%.2X",outBuf[0],outBuf[1],outBuf[2],outBuf[3],outBuf[4],outBuf[5]);
// 				AdpInfo.iType = (AdapterInfo[ii]).Type;
// 				aryAdpInfo.Add(AdpInfo);
// 				if(AdpInfo.iType == 6) {  // 유선
// 					iEthernet++;
// 				}
// 				else if(AdpInfo.iType == 71){ //무선
// 					iWireless++;
// 				}
// 			}
// 			else
// 			{
// 				TRACE("ERROR information: %d\n",GetLastError());
// 			}
// 			CloseHandle(hDev);
// 		}
// 	}
// 	else
// 	{
// 		TRACE("NOT FOUND   ");
// 	}
// 	
// 	if (iEthernet != 0){
// 		for(int jj=0; jj < aryAdpInfo.GetSize(); jj++)
// 		{
// 			if (aryAdpInfo.GetAt(jj).iType == 6){
// 				strMac.Format("%s",aryAdpInfo.GetAt(jj).sMac);
// 				strArr.Add(strMac);
// 			}
// 		}
// 	}
// 	else if (iWireless != 0){
// 		for(int jj=0; jj < aryAdpInfo.GetSize(); jj++)
// 		{
// 			if (aryAdpInfo.GetAt(jj).iType == 71){
// 				strMac.Format("%s",aryAdpInfo.GetAt(jj).sMac);
// 				strArr.Add(strMac);
// 			}
// 		}
// 	}
// 	else{
// 		for(int jj=0; jj < aryAdpInfo.GetSize(); jj++)
// 		{
// 			strMac.Format("%s",aryAdpInfo.GetAt(jj).sMac);
// 			strArr.Add(strMac);
// 		}
// 	}
// 	
// 	if( strArr.GetSize() != 0)
// 	{
// 		strArr.Sort();
// 		data.Format("%s",strArr.GetAt(0));
// 	}
// 	else
// 	{
// 		data.Format("000000000000");
// 	}
}

CString CLogin::IpccEtcdata(int nkind, CString sfind, CString svalue)
{
	int pos = -1;
	CString	stmp, stmps, rttmp = _T("");

	pos = svalue.Find(sfind);

	if (pos > 0)
	{
		stmp = svalue.Mid(pos);	
		pos = -1;
		pos = stmp.Find(",");
		if (pos > 0)
		{
			stmps = stmp.Left(pos);
			pos = -1;
			pos = stmps.Find("=");
			if (pos > 0)
				rttmp = stmps.Mid(pos + 1);
		}
	}

	return rttmp;
}

CString CLogin::GetLogin(int id)
{
	/*
	if (id & 0xff00)
	{
		id = id & 0x00ff;
		switch (id)
		{
		case WRAP_WHOI		:	m_returns = m_wrapwhoi;		break;
		case WRAP_MANAGE	:	m_returns = m_manage;		break;
		case WRAP_ADMIN		:	m_returns = m_admin;		break;
		case WRAP_SUPPORT	:	m_returns = m_support;		break;
		case WRAP_MANAGERSPON	:	m_returns = m_managerespon;	break;
		case WRAP_ADMINIRESPON	:	m_returns = m_adminrespon;	break;
		case WRAP_INQUIR	:	m_returns = m_inquir;		break;
		case USER_BlACKUSER	:	m_returns = m_blackuser;	break;
		case USER_BLACKUSETIME	:	m_returns = _T("");		break;
		case USER_ORDER		:	
			if (m_order)
				m_returns = "Y";
			else
				m_returns = "N";
			break;
		case USER_USID		:	m_returns = m_user;		break;
		case USER_NAME		:	m_returns = m_name;		break;
		case USER_DEPT		:	m_returns = m_dept;		break;
		case USER_TERM		:	m_returns = m_term;		break;
		case USER_SVNO		:	m_returns = m_svno;		break;
		case USER_WHOI		:	m_returns = m_whoi;		break;
		case USER_PSWD		:	m_returns = m_pswd;		break;
		case USER_COMNO		:	m_returns = m_comNo;		break;
		case USER_COMNAME	:	m_returns = m_comNm;		break;
		case USER_DEPTNAME	:	m_returns = m_deptNm;		break;
		case USER_IDNO		:	m_returns = m_idNo;		break;
		case USER_AUTH		:	m_returns = m_auth;		break;
		case USER_MGRAPPRIP	:	m_returns = m_mgrIp;		break;
		case USER_MGRAPPRTERM	:	m_returns = m_mgrTerm;		break;
		case USER_DATE		:	m_returns = m_date;		break;
		case USER_XDEF2		:	m_returns = m_def2;		break;
		case USER_COMGB		:	m_returns = _T("");		break;
		case USER_INPUTGB	:	m_returns = _T("");		break;
		case USER_CHK		:	m_returns = m_chk;		break;
		case USER_SVNM		:	m_returns = m_svNm;		break;
		case USER_LCOMM		:	m_returns = m_lcomm;		break;
		case USER_LDATE		:	m_returns = m_ldate;		break;
		case USER_LTIME		:	m_returns = m_ltime;		break;
		case USER_LTODAY	:	m_returns = m_Today;		break;
		case USER_STORE		:	m_returns = m_storeGb;		break;
		case USER_DEPTTP	:	m_returns = m_DeptTP;		break;
		case USER_BSNSTP	:	m_returns = m_BsnsTP;		break;
		case USER_MLOGIN	:	m_returns = m_multiLogin;	break;
		case USER_DEPTTEl	:	m_returns = m_DeptTel;		break;
		case USER_APPRYN	:	m_returns = m_ApprYN;		break;
		case USER_CHANNEL	:	m_returns = m_Channel;		break;
		case USER_CIF		:	m_returns = m_Cif;			break;
		case 0:
		default:
			m_returns = _T("");
			break;
		}
	}
	else
	{
		switch (id)
		{
		case 0:		// postCode
			m_returns = m_dept;	break;
		case 1:		// date
			m_returns = m_date;	break;
		case 2:		// can input account
		default:
			m_returns = _T("");	break;
		}
	}
	return m_returns;
	*/
	return _T("");
}

void CLogin::clear()
{
	m_returns = _T("");
	m_term		= _T("");

	m_user		= _T("");
	m_name		= _T("");
	m_bExpert	= _T("");
	m_bBondEx	= _T("");
	m_bemailfrm = _T("");
	m_bPassChange = _T("");
	m_bSameConnect = _T("");
	m_btmpuse	= _T("");
	m_bUserst		= _T("");
	m_deptNm	= _T("");
	m_bnotice		= _T("");
	m_buuidrec		= _T("");
	m_reserve		= _T("");
	m_errc	= _T("");
	m_ldate		= _T("");
	m_ltime		= _T("");
	
}

void CLogin::OnOpenUrl(CString strIP)
{
	/*
    CInternetSession session;
	
	
    try {
		CString strUrl;
		strUrl.Format("http://www.hi-ib.com/inc/common/UserIPCheck.jsp?userip=%s",strIP);
        CInternetFile *p_html = (CInternetFile *)session.OpenURL(strUrl);

        if(p_html != NULL){
            CString str, html_text;
			
            p_html->SetReadBufferSize(512);
            while(1){
                if(!p_html->ReadString(str)) break;
                html_text += str;
            }
            p_html->Close();
            delete p_html;
            session.Close();
			m_ConnectArea.Format("%s",html_text);
        }
		
    } catch(CInternetException *){
    }
	*/
}

BSTR CLogin::_getUsid() 
{
	return m_user.AllocSysString();
}

BSTR CLogin::_getUsnm() 
{
	return m_name.AllocSysString();
}

BSTR CLogin::_getbExpert() 
{
	return m_bExpert.AllocSysString();
}

BSTR CLogin::_getbBondEx() 
{
	return m_bBondEx.AllocSysString();
}

BSTR CLogin::_getbPassChange() 
{
	return m_bPassChange.AllocSysString();
}

BSTR CLogin::_getbSameConnect() 
{
	return m_bSameConnect.AllocSysString();
}

BSTR CLogin::_getbtmpuse() 
{
	return m_btmpuse.AllocSysString();
}

BSTR CLogin::_getbUserst() 
{
	return m_bUserst.AllocSysString();
}

BSTR CLogin::_getdeptNm() 
{
	return m_deptNm.AllocSysString();
}

BSTR CLogin::_getbnotice() 
{
	return m_bnotice.AllocSysString();
}

BSTR CLogin::_getbuuidrec() 
{
	return m_buuidrec.AllocSysString();
}

BSTR CLogin::_getreserve() 
{
	return m_reserve.AllocSysString();
}

BSTR CLogin::_getm_errc() 
{
	return m_errc.AllocSysString();
}

BSTR CLogin::_getldate() 
{
	return m_ldate.AllocSysString();
}

BSTR CLogin::_GetcomGb()     //회사코드 6
{
	return m_comGb.AllocSysString();
}

BSTR CLogin::_GetupGb()    //업종 6
{
	return m_upGb.AllocSysString();
}

BSTR CLogin::_getltime() 
{
	return m_ltime.AllocSysString();
}

BSTR CLogin::Getauth(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 여기에 디스패치 처리기를 추가합니다.
	strResult.Format(_T("%s"), m_auth);
	strResult.TrimRight();
	return strResult.AllocSysString();
}



BSTR CLogin::GetcompNm(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 여기에 디스패치 처리기를 추가합니다.
	strResult.Format(_T("%s"), m_strcompNm);
	strResult.TrimRight();
	return strResult.AllocSysString();
}























BSTR CLogin::_getTerm() 
{
	return m_term.AllocSysString();
}

BSTR CLogin::_getllDate() 
{
	return m_ldate.AllocSysString();
}

BOOL CLogin::_getOrder() 
{
	return TRUE;
}

BSTR CLogin::_getDef2() 
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::_GetChk() 
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::_GetSvnm() 
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::_getlComm() 
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::_getManage() 
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::_getAdmin() 
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::_getSupport() 
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::_getManagerespon() 
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::_getAdminrespon() 
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::_getInquir() 
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::_WrapWhoi() 
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::_getBlackUserTime() 
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::_getBlackUser() 
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::_getToday()
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::_getCallLevel()
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::_getIPCCInOut()
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::_getIPCCFax()
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::_getIPCCSms()
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::_getIPCCEmail()
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::_getItem()
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::_getStoreGb()
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::_getMacAddr() 
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::GetConnArea() 
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::GetDeptTP() 
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::GetBsnsTP() 
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::GetDeptTel() 
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::GetApprYN() 
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::GetChannel() 
{
	CString strdata;
	return strdata.AllocSysString();
}

BSTR CLogin::GetCif() 
{	
	CString strdata;
	return strdata.AllocSysString();
}

void CLogin::_ChangeInfo(LPCTSTR strMap, LPCTSTR strKind, LPCTSTR strValue) 
{
	/*
	CString tmp, mapN;
	CAxisloginApp* app = (CAxisloginApp *)AfxGetApp();

	mapN = strMap;

	if (mapN.Find("CK9856") < 0)
		return;

	tmp = strKind;

	if (!tmp.CompareNoCase("Dept"))
	{
		m_dept = strValue;
		app->m_dept = strValue;
	}
	if (!tmp.CompareNoCase("DeptName"))
		m_deptNm = strValue;
	if (!tmp.CompareNoCase("DeptTP"))
		m_DeptTP = strValue;
	if (!tmp.CompareNoCase("BsnsTP"))
		m_BsnsTP = strValue;
	if (!tmp.CompareNoCase("DeptTel"))
		m_DeptTel = strValue;
	*/
}

void CLogin::_WriteReg() 
{
	CString sSubKey;

	sSubKey.Format("Software\\%s\\%s\\%s", m_strRegKey, AXISNAME, INFORMATION);
	TRACE("[CHANGE_TiTlestrRegKey]%s\t%s\n", m_strRegKey, sSubKey);
	WriteReg(m_strRegKey, sSubKey);
}

