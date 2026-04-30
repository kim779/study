// AccountCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Account.h"
#include "AccountCtrl.h"
#include "../../axis/axMsg.hxx"

#include "AccCrypto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_COMBO		0x80000001
#define IDC_EDIT		0x80000002

#define MAX_FONT_SIZE		24
#define DROPDOWNHEIGHT		300
#define MARGIN			2
#define CTRLGAP			0

#define APPEARANCE_FLAT		0
#define APPEARANCE_3D		1

#define BORDERSTYLE_NONE	0
#define BORDERSTYLE_FIXED	1

#define COLOR_BORDER		65
#define COLOR_FORE		69
#define COLOR_BACK		64
#define COLOR_EDITBACK		68

#define ST_GNORMAL		0
#define ST_GDOWN		1
#define ST_GOVER		2
#define ST_ARROWNORMAL		0
#define ST_ARROWDOWN		1
#define ST_ARROWOVER		2

#define MS_NORMAL		0
#define MS_DOWN			1
#define MS_OVER			2

#define ACC_DEPT		3
#define ACC_TYPE		2
#define ACC_NO			6
#define ACC_LENGTH		ACC_DEPT + ACC_TYPE + ACC_NO + 2

#define TABSPACE		" "

#define PP_SYM			"ANSM"
#define PL_SYM			"LAIF"

#define SEL_ST			0
#define SEL_FU			1
#define SEL_BC			2

#define SC_ACCLISTBYGROUP	102		// 그룹계좌조회

#define TT_INIT			1000
#define TT_QUERYACCNM		1001
#define TT_GETINIT		1002
#define TT_813REREAD		1003
#define TT_029REREAD		1004
#define TRKEY_GROUP		255
#define TRKEY_ACCNTNAME		254
#define TRKEY_ACINFO		253
#define TRKEY_SACMQ329		252		//2013.03.12 KSJ  자금대체대리인 조회
#define TRKEY_SACMQ748		251		//2013.10.29 MK 고객추가정보 조회
#define TRKEY_CEHCKACC		250
#define ONLY_SISE 		'9'		//객장 시세조회전용단말 아이디 앞자 "9" 로 시작

#define	LEN_USID		12 
#define	LEN_GRPD		4  
#define	LEN_ECOD		4
#define	LEN_EMSG		80
#define	LEN_NREC		4		//반복
#define	LEN_ACNO		11
#define	LEN_ACNM		40
#define	LEN_ACPW		8
#define	LEN_GREC		4		//이중반복
#define	LEN_GPID		4
#define	LEN_FLAG		1

#define LEN_GPNM		40

#define GID_CLIENT		"999"

typedef struct _IAccnm
{
	char	szIn[5];
	char	szAccount[20];
} IACCNM, *PIACCNM;

#define L_IAccnm	sizeof(_IAccnm)

typedef struct _OAccnm
{
	char	szOut[5];
	char	szAccount[20];
	char	szAccntNm[40];
} OACCNM, *POACCNM;

#define L_OAccnm	sizeof(_OAccnm)

//----------2013.10.29 MK SACMQ748----------
typedef struct _Isacmq748
{
	char	szIn[5];
	char	szAcntNo[20];
}ISACMQ748;

#define L_ISACMQ748	sizeof(_Isacmq748)

typedef struct _Osacmq748{
	char	szOut0[5];
	char	ClngrdCode[2];			//고객등급코드
	char	ClntGrdNm[40];			//고객등급명
	char	PoiNoteDocRegYn[1];		//투자목적기재서등록여부
	char	LastInclnGrdNm[100];		//최종고객성향등급명
	char	AcntNm[40];			
	char	BrthDt[8];	
	char	AcntNo[20];
	char	szOut1[5];
	char	PostNo[6];			//자택전화번호
	char	Addr[100];
	char	TelRgno[4];
	char	TelExno[4];
	char	TelSeqno[4];
	char	szOut2[5];
	char	PostNo1[6];			//회사전화번호
	char	Addr1[100];
	char	TelRgno1[4];
	char	TelExno1[4];
	char	TelSeqno1[4];
	char	WkplcNm[40];
	char	szOut3[5];
	char	EmailAddr[100];			//휴대전화번호
	char	MoveTelIdfyNo[4];
	char	MoveTelExno[4];
	char	MoveTelSeqno[4];
}OSACMQ748, *POSACMQ748;

#define L_OSACMQ748	sizeof(_Osacmq748)
//----------2013.10.29 MK SACMQ748----------

//2013.03.12 KSJ SACMQ329
typedef struct _Isacmq329{
	char	szIn[5];
	char	szUserID[16];
}ISACMQ329;

#define L_ISACMQ329	sizeof(_Isacmq329)

typedef struct _Osacmq329_grid{
	char	szAccount[20];
	char	szAccName[40];
	char	szItems[8];	//21주문대리인(일반계좌) 접근편의성을 위해서 한군데에서 관리
//	char	szItem2[1];	//22주문대리인(법인계좌)
//	char	szItem3[1];	//23주문대리인(법정대리)
//	char	szItem4[1];	//24주문대리인(투자일임업자)
//	char	szItem5[1];	//27금융상품매매대리(4989 펀드매매, 6113 인덱스펀드주문, 6202 RP매수)
//	char	szItem6[1];	//28종합계좌간자금대체대리인(8101 자금대체)
//	char	szItem7[1];	//temp
//	char	szItem8[1];	//temp
}OSACMQ329_GRID;

typedef struct _Osacmq329{
	char	szOut[5];
	OSACMQ329_GRID grid[1];
}OSACMQ329, *POSACMQ329;

#define L_OSACMQ329	sizeof(_Osacmq329)

#define AGENT_NOITEM		0	//세팅안됨
#define AGENT_ITEM1		1	//21주문대리인(일반계좌)
#define AGENT_ITEM2		2	//22주문대리인(법인계좌)
#define AGENT_ITEM3		3	//23주문대리인(법정대리)
#define AGENT_ITEM4		4	//24주문대리인(투자일임업자)
#define AGENT_ITEM5		5	//27금융상품매매대리(4989 펀드매매, 6111, 6112 6113 인덱스펀드주문)에서는 53으로 필터링함.
#define AGENT_ITEM6		6	//28종합계좌간자금대체대리인(8101 자금대체)
#define AGENT_ITEM7		7	//temp
#define AGENT_ITEM8		8	//temp

#define STR(field)	CString(field, sizeof(field))

typedef struct _Isacaq029
{
	char temp[5];
	char	szAcntNo[20];
}ISACAQ029;

typedef struct _Osacaq029 {
	char temp[5];
	char szAgntYN[1];			//대리인여부 : Y 대리인보유   N 대리인미보유
	char szInvstTp[1];            //투자자구분 : 0 전문금융소비자  1 일반금융소비자  9 등록안됨
	char szInfoPrvdTp[1];       //정보제공구분 : 0 미제공 1 제공
	char szInvstCnsvTp[1];      //투자권유구분 : 0 미제공 1 제공
	char szWeakInvastrYn[1];      //취약투자자여부 : 1 동의 9 동의안함 0 해당없음
	char szLastInclInGrdNm[100];   //최종고객성향등급명 : ex 안정형 안정추구형 위험중립형 적극투자형 공격투자형
	char szDueDt[8];                   //만기일
}OSACAQ029, * POSACAQ029;

#define L_ISACAQ029	sizeof(_Isacaq029)

//KJS
void CAccountCtrl::FileLog(CString strhome, LPCSTR log, ...)
{
	TRY
	{
		CString slog;
	slog.Format("[cx_account][WriteLog] [%s]\n", log);
	Output_DebugString(slog);

		FILE* fp;
		fopen_s(&fp, strhome + "\\exe\\axis.log", "a+");
		if (!fp) return;

		const CTime time = CTime::GetCurrentTime();
		fprintf(fp, (LPCSTR)time.Format("[%Y-%m-%d %H:%M:%S] "));

		va_list argptr;
		va_start(argptr, log);
		vfprintf(fp, log, argptr);
		va_end(argptr);
		fprintf(fp, "\n");

		fclose(fp);
	}
		CATCH(CMemoryException, e)
	{

	}
	END_CATCH
}

void WriteLog(CString strhome, LPCSTR log, ...)
{
	return;
#if 1
	TRY
	{
		CString slog;
	slog.Format("[cx_account][WriteLog] [%s]\n", log);
	Output_DebugString(slog);

		FILE* fp;
		fopen_s(&fp, strhome + "\\exe\\axis.log", "a+");
		if (!fp) return;

		const CTime time = CTime::GetCurrentTime();
		fprintf(fp, (LPCSTR)time.Format("[%Y-%m-%d %H:%M:%S] "));

		va_list argptr;
		va_start(argptr, log);
		vfprintf(fp, log, argptr);
		va_end(argptr);
		fprintf(fp, "\n");

		fclose(fp);
	}
		CATCH(CMemoryException, e)
	{

	}
	END_CATCH
#endif
}
/////////////////////////////////////////////////////////////////////////////
// CAccountCtrl

CAccountCtrl::CAccountCtrl(CWnd* pParent, _param* pParam)
	: m_bDLL(FALSE)
	, m_bEditMode(FALSE)
	, m_bFireEvent(FALSE)
	, m_bInit(FALSE)
	, m_bMultiClick(FALSE)
	, m_bOrderEnable(TRUE)
	, m_bTrackLeave(FALSE)
	, m_bUseAlias(FALSE)
	, m_nSelectGroup(0)
	, m_pCombo(nullptr)
	, m_pEdit(nullptr)
	, m_pFont(nullptr)
	, m_pGroupList(nullptr)
	, m_pParent(pParent)
	, m_strPassword(_T(""))
	, m_strRoot(_T(""))
	, m_strUser(_T(""))
	, m_strRet(_T(""))
	, m_hG(NULL)
	, m_hG_DN(NULL)
	, m_hG_EN(NULL)
	, m_hArrow(NULL)
	, m_hArrow_DN(NULL)
	, m_hArrow_EN(NULL)
	, m_nAppearance(APPEARANCE_FLAT)
	, m_nBorderStyle(BORDERSTYLE_FIXED)
	, m_nStateG(ST_GNORMAL)
	, m_nStateArrow(ST_ARROWNORMAL)
	, m_nType(SEL_ST)
	, m_pTipCtrl(nullptr)
	, m_strTempDomino(_T(""))
	, m_bAllowDelegation(TRUE)
	, m_bBankLinked(FALSE)
	, m_nTimerCnt(0)
	, m_nAgentType(AGENT_NOITEM)
	, m_strAgenAcctList(_T(""))
	, m_strAgenNameList(_T(""))
{
	EnableAutomation();

	SetParam(pParam);
	m_clrBorderColor = GetSysColor(COLOR_WINDOWFRAME);
	m_clrEditBackColor = RGB(255, 255, 255);

	m_bGroup = FALSE;

	m_mapName = (LPCTSTR)m_pParent->SendMessage(WM_USER, MAKEWPARAM(mapDLL, 0));

	char	readb[32]{};
	CString Path{};
	Path.Format("%s\\tab\\AXIS.INI", Variant(homeCC, ""));
	int readl = GetPrivateProfileString("AXIS", "reg", "", readb, sizeof(readb), Path);
	m_regkey = CString(readb, readl);

	LoadAccountType();
	Convert_V2();
	SetAccountType();

	m_bDestroy = FALSE;	//2012.02.08 KSJ
	m_bAddAll = true;	//2012.02.16 KSJ
	m_OldString.Empty() ;	//2012.02.28 KSJ
	/*shareddll.Load("cx_shared.dll");

	if (shareddll.IsLoaded())
	{
		if (!shareddll.Function<const bool WINAPI(int, CWnd**)>("axGetData")(1, &m_pAxisMainFrame))
		{
			m_pAxisMainFrame = GetTopLevelParent();

			m_slog.Format("				[cx_account]  GetTopLevelParent m_pAxisMainFrame=[%x]", m_pAxisMainFrame);
			OutputDebugString(m_slog);
		}
		else
		{
			m_slog.Format("				[cx_account] !!!!not GetTopLevelParent[%x] shared!!! m_pAxisMainFrame=[%x] ", GetTopLevelParent(), m_pAxisMainFrame);
			OutputDebugString(m_slog);
		}
	}*/
}

CAccountCtrl::~CAccountCtrl()
{
	if (m_pCombo)	m_pCombo.reset();
	if (m_pEdit)	m_pEdit.reset();

	SafeRemoveAccArray(&m_arrAllAcc);
	SafeRemoveAccArray(&m_arrHistoryAcc);
	for (int ii = 0; ii < m_arrGroupList.GetSize(); ii++)
	{
		std::shared_ptr<CGroup> pGroup = m_arrGroupList.GetAt(ii);
		if (pGroup)
		{
			SafeRemoveAccArray(&pGroup->m_arrAccount);
			pGroup.reset();
		}
	}
	m_arrGroupList.RemoveAll();
}

void CAccountCtrl::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.
	
	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CAccountCtrl, CWnd)
	//{{AFX_MSG_MAP(CAccountCtrl)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_CBN_SELCHANGE(IDC_COMBO, OnComboSelChange)
	ON_WM_TIMER()
	ON_WM_SETFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_DESTROY()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEWHEEL, OnMouseLeave)
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_EDITX, OnEditXMsg)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CAccountCtrl, CWnd)
//{{AFX_DISPATCH_MAP(CAccountCtrl)
	DISP_PROPERTY_EX(CAccountCtrl, "BackColor", _GetBackColor, _SetBackColor, VT_I4)
	DISP_PROPERTY_EX(CAccountCtrl, "Data", _GetData, _SetData, VT_BSTR)
	DISP_PROPERTY_EX(CAccountCtrl, "Dept", _GetDept, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CAccountCtrl, "ForeColor", _GetForeColor, _SetForeColor, VT_I4)
	DISP_PROPERTY_EX(CAccountCtrl, "Group", _GetGroup, _SetGroup, VT_BSTR)
	DISP_PROPERTY_EX(CAccountCtrl, "Name", _GetName, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CAccountCtrl, "Password", _GetPassword, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CAccountCtrl, "TabStop", _GetTabStop, _SetTabStop, VT_BOOL)
	DISP_PROPERTY_EX(CAccountCtrl, "Type", _GetType, SetNotSupported, VT_I2)
	DISP_PROPERTY_EX(CAccountCtrl, "Visible", _GetVisible, _SetVisible, VT_BOOL)
	DISP_PROPERTY_EX(CAccountCtrl, "OnlySise", GetOnlySise, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CAccountCtrl, "IsBankLinked", IsBankLinked, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX(CAccountCtrl, "AgentType", GetAgentType, SetAgentType, VT_I4)
	DISP_FUNCTION(CAccountCtrl, "Empty", _Empty, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CAccountCtrl, "SetFocus", _SetFocus, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CAccountCtrl, "IsAccAgent", IsAccAgent, VT_BOOL, VTS_BSTR VTS_I4)
	DISP_FUNCTION(CAccountCtrl, "GetAgentAccList", GetAgentAccList, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CAccountCtrl, "GetAgentNameList", GetAgentNameList, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CAccountCtrl, "ReHistroy", ReHistroy, VT_EMPTY, VTS_BSTR VTS_BSTR)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CAccountCtrl, "GetShowHideAcc", dispidGetShowHideAcc, GetShowHideAcc, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CAccountCtrl, "GetShowHideAccName", dispidGetShowHideAccName, GetShowHideAccName, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CAccountCtrl, "EnableCtrl", dispidEnableCtrl, EnableCtrl, VT_EMPTY, VTS_I2)
END_DISPATCH_MAP()

// Note: we add support for IID_IAccountCtrl to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {06ED8B27-3CFC-4A5E-B6D9-526FADC718A7}
static const IID IID_IAccountCtrl =
{ 0x6ed8b27, 0x3cfc, 0x4a5e, { 0xb6, 0xd9, 0x52, 0x6f, 0xad, 0xc7, 0x18, 0xa7 } };

BEGIN_INTERFACE_MAP(CAccountCtrl, CWnd)
	INTERFACE_PART(CAccountCtrl, IID_IAccountCtrl, Dispatch)
END_INTERFACE_MAP()

long CAccountCtrl::_GetBackColor() 
{
	return (long)m_clrBackColor;
}

void CAccountCtrl::_SetBackColor(long nNewValue) 
{
	m_clrBackColor = nNewValue;
	Invalidate();
}

BSTR CAccountCtrl::_GetData() 
{
	CString strResult(_T(""));

	if (m_pEdit)
		strResult = m_pEdit->GetInputData();

	return strResult.AllocSysString();
}

void CAccountCtrl::_SetData(LPCTSTR lpszNewValue) 
{
	if (m_pEdit)
	{
		m_pEdit->SetInputData(lpszNewValue);
	}
}

BSTR CAccountCtrl::_GetDept() 
{
	CString strResult(_T(""));

	if (m_pEdit)
		strResult = m_pEdit->GetInputData();

	if (strResult.GetLength() == ACC_DEPT + ACC_TYPE + ACC_NO)
		strResult = strResult.Left(ACC_DEPT);
	else
		strResult.Empty();

	return strResult.AllocSysString();
}

long CAccountCtrl::_GetForeColor() 
{
	return (long)m_clrForeColor;
}

void CAccountCtrl::_SetForeColor(long nNewValue) 
{
	m_clrForeColor = nNewValue;
	Invalidate();
}

BSTR CAccountCtrl::_GetGroup() 
{
	CString strResult(_T(""));

	if (m_nSelectGroup > 0)
		strResult = m_arrGroupList.GetAt(m_nSelectGroup)->m_strGrName;
	else
		strResult = _T("전체");

	return strResult.AllocSysString();
}

void CAccountCtrl::_SetGroup(LPCTSTR lpszNewValue) 
{
	std::shared_ptr<CGroup> pGroup = nullptr;
	const	int	nCount = m_arrGroupList.GetSize();

	for (int ii = 0; ii < nCount; ii++)
	{
		pGroup = m_arrGroupList.GetAt(ii);
		if (pGroup->m_strGrName == lpszNewValue)
		{
			m_nSelectGroup = ii + 1;
			return;
		}
	}
}

BSTR CAccountCtrl::_GetName() 
{
	CString strResult(_T(""));

	if (m_strDept == m_sAccnDept)
		m_strAccName = Find813Name(m_pEdit->GetInputData());
	if (!m_strAccName.IsEmpty())
		strResult = m_strAccName;
	else if (m_pEdit)
	{	
		CString strAcc = m_pEdit->GetInputData();

		strAcc.Trim();
		if (!strAcc.IsEmpty())
		{			
			std::shared_ptr<CAccount> pAcc = FindAccountInfo(strAcc);

			if (pAcc)
			{
				if (m_bUseAlias)
					strResult = pAcc->m_strAccntNick.IsEmpty() ? pAcc->m_strAccntName : pAcc->m_strAccntNick;//pAcc->m_strAccntName;
				else
					strResult = pAcc->m_strAccntName;
			}
		}
	}

	return strResult.AllocSysString();
}

BSTR CAccountCtrl::_GetPassword()
{
	CString strResult;

	if (m_pEdit)
	{	
		CString strAcc = m_pEdit->GetInputData();	
		strAcc.Trim();

		if (!strAcc.IsEmpty())
		{
			std::shared_ptr<CAccount> pAcc = FindAccountInfo(strAcc);
			if (pAcc)
			{
				if (pAcc->m_strPassword.IsEmpty())
				{
					strResult = GetPassword(strAcc);
					pAcc->m_strPassword = strResult;
				}
				else
					strResult = pAcc->m_strPassword;
			}
			
			if (strResult.IsEmpty() && m_strDept == m_sAccnDept)
			{
				strResult = Find813Pswd(strAcc);
			}

			if (m_bKeyIn && m_strDept == m_sAccnDept)
			{
				m_bKeyIn = false;
				strResult = "";
			}
		}
	}

	return strResult.AllocSysString();
}

BOOL CAccountCtrl::_GetTabStop() 
{
	const	LONG nStyle = GetWindowLong(this->GetSafeHwnd(), GWL_STYLE);

	return (nStyle & WS_TABSTOP);
}

void CAccountCtrl::_SetTabStop(BOOL bNewValue) 
{
	LONG dwStyle = GetWindowLong(GetSafeHwnd(), GWL_STYLE);

	if (bNewValue)
		dwStyle |= (LONG) WS_TABSTOP;
	else
		dwStyle &= ~((LONG) WS_TABSTOP);

	SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle);
}

short CAccountCtrl::_GetType() 
{
	return m_nType;
}

BOOL CAccountCtrl::_GetVisible() 
{
	return IsWindowVisible();
}

void CAccountCtrl::_SetVisible(BOOL bNewValue) 
{
	ShowWindow(bNewValue ? SW_SHOW : SW_HIDE);
}

void CAccountCtrl::_Empty() 
{
	if (m_pEdit)
		m_pEdit->EmptyData(TRUE);

	if (m_pParent && m_bInit)
	{
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange/*Change*/)), 
					(LPARAM)m_Param.name.GetString());
	}
}

void CAccountCtrl::_SetFocus()
{
	SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// CAccountCtrl message handlers
//사용안함
void CAccountCtrl::Check2AgentAcc()
{
	CString slog;
	
#ifndef DF_TEST_MODE
	if (!m_staff)
	{
		m_bCheckAgetn = false;
		return;
	}
#endif
	//m_iAxisState = m_pAxisMainFrame->SendMessage(WM_USER, MMSG_GETMAIN_STATE, 0);
	//if (!m_bPOPTrdTed || m_iAxisState <= 5)
	//	return;

////주문대리인 조회 관련 화면 분류
//대표화면 내부의 오브젝트에 계좌 컨트롤이 있는경우 
	CString mapname0;
	mapname0 = (LPCSTR)m_pParent->SendMessage(WM_USER, MAKEWPARAM(mapDLL, 0), 0);
	slog.Format("\r\n[cx_account] -------------------------------------------[%s]----------------------------------------------------------", mapname0);
	WriteLog(Variant(homeCC), slog);

	CString	Path, strTemp;
	char	readb[10 * 1024]{};
	int	readl;                    
	Path.Format("%s\\tab\\AGENTCHECK.INI", Variant(homeCC, ""));
	readl = GetPrivateProfileString("SACAQ029", mapname0, "", readb, sizeof(readb), Path);
	strTemp.Format("%s", readb);
	strTemp.Trim();

	if (readl <= 0)  //AGENTCHECK 에 없는 화면은 안함
	{
		m_bCheckAgetn = false;
		return;
	}

	if (strTemp == "-1")  //IB101100= -1    이런화면도 안함
	{
		m_bCheckAgetn = true;
		slog.Format("\r\n [cx_account] !!!!! -1로 파일에  cx_account [%s]  map0=[%s]    m_bCheckAgetn=[%d] m_bDLL=[%d]  !!!!", m_Param.options, mapname0, m_bCheckAgetn, m_bDLL);
		WriteLog(Variant(homeCC), slog);
		return;
	}

	CStringArray arr;
	while (1)
	{
		arr.Add(Parser(strTemp, ";"));
		if (strTemp.IsEmpty())
			break;
	}

	//대표화면에 계좌컨트롤이 있는 경우
	BOOL bAccInMain = FALSE;
	if (arr.GetSize() == 1)
	{
		strTemp = arr.GetAt(0);
		if (mapname0 == Parser(strTemp, ","))
		{
			bAccInMain = TRUE;
		}
	}

	int idex = 1;
	CString strobjname, stmp, sresult;
	BOOL bFind = FALSE;
	m_slog.Format("[cx_account]---------------------------------------------------------------------------------------------------");
	Output_DebugString(m_slog);

	while (1)
	{
		stmp = (LPCSTR)m_pParent->SendMessage(WM_USER, MAKEWPARAM(mapDLL, idex), 0);
		sresult += stmp;
		sresult += " | ";

m_slog.Format("[cx_account][%s]<%d><%x>			mapDLL로 검색(stmp)  =[%s] idex =[%d]", __FUNCTION__, __LINE__, this, stmp, idex);
Output_DebugString(m_slog);

		for (int ii = 0; ii < arr.GetSize(); ii++ )
		{
			strTemp = (LPCSTR)m_pParent->SendMessage(WM_USER, MAKEWPARAM(mapDLL, idex + 1));

m_slog.Format("	[cx_account][%s]<%d>		strTemp  =[%s] mapname0(대표화면) =[%s] bAccInMain=[%d] m_bCheckAgetn=[%d] ",
	__FUNCTION__, __LINE__, strTemp, mapname0, bAccInMain, m_bCheckAgetn);
Output_DebugString(m_slog);

			if (arr.GetAt(ii).Find(stmp) >= 0 && (strTemp == mapname0))
			{

m_slog.Format("		[cx_account][%s]<%d>		arr에 있음 ->  stmp  =[%s] strTemp =[%s]", __FUNCTION__, __LINE__, stmp, strTemp);
Output_DebugString(m_slog);

				if (bAccInMain && idex > 1)
					continue;

				strobjname = stmp;
				bFind = TRUE;
				m_bCheckAgetn = true;

				Path.Format("%s\\tab\\AGENTCHECK.INI", Variant(homeCC, ""));
				readl = GetPrivateProfileString("SACAQ029", "hide", "", readb, sizeof(readb), Path);

				strTemp.Format("%s", readb);
				strTemp.Trim();



				if (strTemp.Find(m_mapName) >= 0)
					m_bCheckAgetn = false;

				slog.Format("\r\n			[cx_account]---  map0=[%s]  strobjname=<%s>[%s]  idex=[%d] ---  m_bCheckAgetn=[%d] m_bDLL=[%d]  sresult=[%s] ii=[%d] size=[%d] bAccInMain=[%d]",
					mapname0, strobjname, (LPCSTR)m_pParent->SendMessage(WM_USER, MAKEWPARAM(mapDLL, idex + 1), 0), idex, m_bCheckAgetn, m_bDLL, sresult, ii, arr.GetSize(), bAccInMain);
				Output_DebugString(slog);
				WriteLog(Variant(homeCC), slog);
				return;
			}
		}

		idex++;
		if (idex == 30)
		{
			strobjname = "못찾음";
			slog.Format("\r\n [cx_account] [%s] ---  map0=[%s]  strobjname=<%s>  idex=[%d] ---   sresult=[%s] ",
				m_Param.options, mapname0, strobjname, idex - 1, sresult);
			Output_DebugString(slog);
			WriteLog(Variant(homeCC), slog);
			bFind = FALSE;
			m_bCheckAgetn = false;
			return;;
		}
	}
}

BOOL CAccountCtrl::Initialize(BOOL bDLL)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_bKeyIn = false;
	m_bDLL = bDLL;
	
	m_staff = false;
	m_bBankLinked = FALSE;
	
	m_strRoot = Variant(homeCC);
	m_strUser = Variant(userCC);
	m_strName = Variant(nameCC);
	m_strDept = Variant(deptCC);
	m_sHome.Format("%s\\user\\%s\\%s.ini", m_strRoot, m_strName, m_strName);
//m_slog.Format("[cx_account][%s]<%d>  Variant(deptCC)  =[%s]", __FUNCTION__, __LINE__, m_strDept);
//Output_DebugString(m_slog);

	m_strDept.Trim();
	m_staff = IsNumber(m_strUser);
	//Check2AgentAcc();
	InitAllowDept();  //전 계좌 가능 지점부서 초기화 

	m_pCombo = std::make_unique<CAccCombo>(this);
	m_pEdit  = std::make_shared<CAccEdit>();

	CString	Path, strTemp;
	char	readb[10 * 1024]{};
	int	readl;
	
	Path.Format("%s\\tab\\ACCNTDEPT.INI", Variant(homeCC, ""));
	readl = GetPrivateProfileString("ACCNTDEPT", "DEPT", "811", readb, sizeof(readb), Path);

	strTemp = CString(readb, readl);
	strTemp.Trim();
	m_sAccnDept = strTemp;

//m_slog.Format("[cx_account][%s]<%d>m_sAccnDept=[%s]", __FUNCTION__, __LINE__, m_sAccnDept);
//Output_DebugString(m_slog);
	
	// 10 : ComboBox 안에 이미지를 넣기 위해서 CBS_OWNERDRAWFIXED 추가
	if (!m_pCombo->Create(CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_OWNERDRAWFIXED /*| CBS_AUTOHSCROLL*/,
						  CRect(0, -2, 0, DROPDOWNHEIGHT), this, IDC_COMBO))
	{
		TRACE("Create Failed AccCombo\n");
		return FALSE;
	}
	m_pCombo->SetFont(m_pFont);
	
	if (!m_pEdit->Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_EDIT))
	{
		TRACE("Create Failed AccEdit\n");
		return FALSE;
	}

	m_pEdit->SetFont(m_pFont);

	// 2012.02.02 KSJ 여기서 # digit로 세팅함. 그래서 숫자만 입력됨.
	m_pEdit->SetAccMask();
	m_pEdit->SetInsertMode(TRUE);

	LoadButtonImage();

	if (CreateFont())
		SetFont(m_pFont);

	// 주문대리인 2011.01.24, 로그인시 메인에서 LOGIN.Otpf 
	if (!m_staff)
	{
		CString strMapName;

		Path.Format("%s\\tab\\%s", m_strRoot, INI_DELIGATION);
		strMapName = (LPCSTR)m_pParent->SendMessage(WM_USER, MAKEWPARAM(mapDLL, 0), 0);
		if (strMapName.GetLength() == 8)
		{
			UINT	iVal;
			
			iVal = GetPrivateProfileInt("대리인", (LPCSTR)strMapName.Mid(2, 4), 1, Path);
			m_bAllowDelegation = (iVal==0) ? FALSE : TRUE;

			LPCSTR szOTPF = (LPCSTR)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, popCC), (LPARAM)"LOGIN.Otpf");
			if (szOTPF && !m_bAllowDelegation)
			{
				if (*szOTPF == '1')	// 투자일임업자임
				{
					iVal = GetPrivateProfileInt("투자일임업자", (LPCSTR)strMapName.Mid(2, 4), 0, Path);
					if (iVal==1)
						m_bAllowDelegation = TRUE;
				}
			}
		}
	}

	Path.Format("%s\\user\\%s\\%s.ini", m_strRoot, Variant(nameCC), Variant(nameCC));

	if (GetPrivateProfileInt(_T("Account"), _T("UseAlias"), 0, Path) == 1)
		m_bUseAlias = TRUE;
	
	if (!(long)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0L))
	{
		m_bEditMode = TRUE;
		
		if (!(long)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCCx), 0L))
		{
			m_bOrderEnable = FALSE;
		}
	}


	m_pEdit->SetEditMode(m_bEditMode);

	strTemp = m_Param.name;
	strTemp.Trim();
   
	m_onlysise = GetOnlySise();
	m_mapName = (LPCTSTR)m_pParent->SendMessage(WM_USER, MAKEWPARAM(mapDLL, 0));

	std::shared_ptr<CAccount> pAcc = nullptr;

	readl = GetPrivateProfileString(_T("AccountHistory"), strTemp, _T(""), readb, sizeof(readb), Path);

	//enc
	CString val = ReadAccountHistory(Path, strTemp);
	strncpy_s(readb, sizeof(readb), val, _TRUNCATE);
	readl = (int)strlen(readb);
	//enc


	strTemp = CString(readb, readl);
	strTemp.Trim();
	if (!strTemp.IsEmpty())
	{
		CString strLast;
		CString strLastAcc;
		CString strLastName;

		while (!strTemp.IsEmpty())
		{
			strLast = Parser(strTemp, _T("\t"));
			strLastAcc = Parser(strLast, _T("|"));
			strLastName = Parser(strLast, _T("|"));
		
			if (!m_mapName.Compare("IB140100") || !m_mapName.Compare("IB340100") || !m_mapName.Compare("IB611100"))
			{
			}
			else
			{
				if (m_bWrap)
				{
					// 종합계좌가 아니면 삭제한다.
					if (strLastAcc.Mid(3,2) != "00")
						continue;
					
					pAcc = FindAccountInfo(strLastAcc, TRUE);
					if (pAcc)
					{
						// 랩계좌 필터링
						if ((m_bWrap && !pAcc->m_bWrap) || (!m_bWrap && pAcc->m_bWrap))
							continue;
					}
				}
			}
			AppendAccHistory(strLastAcc, _T(""), strLastName);
		}
	}


	if (m_strDept != m_sAccnDept)
	{
		CString strData = Variant(accountCC);
		CString strName, strAccInfo;
		CMapStringToString tmpAcc;
		int	aflg = 0, nMapNo = atoi(m_mapName.Mid(2, 4));
		BOOL	bIgnore571 = FALSE;
		BOOL	bIgnore561 = FALSE;

		if (nMapNo==6101 || nMapNo==6301 || nMapNo==6501 || nMapNo==6601 || nMapNo==6701 || 
			nMapNo==6801 || nMapNo==6901 || nMapNo==6931 || nMapNo==8101 || nMapNo==8305 || 
			nMapNo==8306 || nMapNo==8308 || nMapNo==8313 || nMapNo==8402 || nMapNo==8502 || nMapNo==8612 ||
			(6201<=nMapNo && nMapNo<=6206) || (8501<=nMapNo && nMapNo<=8506) || (1501<=nMapNo && nMapNo<=1504))
			bIgnore571 = TRUE;

		if (nMapNo == 1032 || nMapNo == 1501 || nMapNo == 8501)
			bIgnore561 = TRUE;

		BOOL bISETF_SAVE_ACC = FALSE;
		if (nMapNo == 8401 || nMapNo == 1040 || nMapNo == 5000 || nMapNo == 2821 || nMapNo == 2831 || nMapNo == 2832 ||
			nMapNo == 1105 || nMapNo == 1402 || nMapNo == 1401 || nMapNo == 3600 ||
			nMapNo == 1304 || nMapNo == 1201 || nMapNo == 1202 ||
			nMapNo == 1204 || nMapNo == 1205 || nMapNo == 1206 || nMapNo == 1207 || nMapNo == 1208 || nMapNo == 1212 ||
			nMapNo == 1301 || nMapNo == 1306 || nMapNo == 1307 || nMapNo == 1308 || nMapNo == 1302 || nMapNo == 1303 ||
			nMapNo == 1304 || nMapNo == 1305 || nMapNo == 1707 || nMapNo == 1101 || nMapNo == 1102 || nMapNo == 1106 ||
			nMapNo == 1103 || nMapNo == 1107 || nMapNo == 1104 || nMapNo == 8220 || nMapNo == 1709 || nMapNo == 8215 || 
			nMapNo == 7141|| nMapNo == 1020 || nMapNo == 1403)  //modi 7141 매매일지차트
		{
			bISETF_SAVE_ACC = TRUE;
		}

		Path.Format("%s\\user\\%s\\%s", m_strRoot, Variant(nameCC), "userconf.ini");
		m_nSort = GetPrivateProfileInt(_T("IB0000A4"), _T("SORT"), 0, Path);

		while (!strData.IsEmpty())
		{
			strAccInfo = Parser(strData, _T("\n"));

			pAcc = std::make_shared<CAccount>();
			pAcc->m_strAccntNum = Parser(strAccInfo, _T("\t"));
			pAcc->m_strPassword = Parser(strAccInfo, _T("\t"));
			strName = Parser(strAccInfo, _T("\t"));
			pAcc->m_strAccntName = Parser(strName, _T("|"));
			pAcc->m_strAccntNick = Parser(strName, _T("|"));
			
			aflg = atoi((LPCTSTR)Parser(strName, _T("|")));
			pAcc->m_bDelegation = (aflg & 0x01) ? TRUE : FALSE;
			pAcc->m_bWrap = (aflg & 0x02) ? TRUE : FALSE;

//m_slog.Format("[cx_account][%s]<%d> [%s] [%s]  [%d]", __FUNCTION__,   __LINE__ ,   pAcc->m_strAccntNum, pAcc->m_strAccntName, aflg);
//Output_DebugString(m_slog);

			if (!(aflg & 0x01))
			{
				if ((m_sHideTradeAgent.Find("1") >= 0))  //주문 대리인 삭제
				{
				//	m_slog.Format("[cx_account][%s]<%d> 주문 대리인 삭제  [%s] [%s]  [%d]", __FUNCTION__, __LINE__, pAcc->m_strAccntNum, pAcc->m_strAccntName, aflg);
				//	Output_DebugString(m_slog);
					pAcc.reset();
					continue;
				}
			}
			
			pAcc->TrimData();
			if (!m_mapName.Compare("IB140100") || !m_mapName.Compare("IB340100") || !m_mapName.Compare("IB611100"))
			{
			}
			else
			{
				// 고객의 경우 랩계좌중 종합계좌만 처리한다.
				if (!m_staff && m_bWrap && pAcc->m_strAccntNum.Mid(3,2)!="00")
				{
					pAcc.reset();
					continue;
				}
				
				// 직원의 경우 랩계좌를 종합계좌로 치환처리한다.
				if (m_staff && m_bWrap)
				{
					LPCSTR	key;

					strTemp.Format("%.3s00%.6s", pAcc->m_strAccntNum.Mid(0,3), pAcc->m_strAccntNum.Mid(5,6));
					if (tmpAcc.LookupKey(strTemp, key)) 
					{
						pAcc.reset();
						continue;
					}
					pAcc->m_strAccntNum = strTemp;
					tmpAcc.SetAt(strTemp, "");
				}
				
				// 랩계좌 필터링
				if (!m_staff)
				{
					if ((m_bWrap && !pAcc->m_bWrap) || (!m_bWrap && pAcc->m_bWrap))
					{
						pAcc.reset();
						continue;
					}
				}
			}
			// 571 계좌 필터링
			if (bIgnore571 && pAcc->m_strAccntNum.Mid(0,3)=="571")
			{
				pAcc.reset();
				continue;
			}

			if (bIgnore561 && pAcc->m_strAccntNum.Mid(0,3) == "561")
			{
				pAcc.reset();
				continue;
			}

			/*if (pAcc->m_strAccntNum == "00115016701")
			{
				CString slog;
				slog.Format("[%d]   [%d]", bISETF_SAVE_ACC, IsFirstValidAcc(pAcc->m_strAccntNum));
				Variant(guideCC, slog);
			}*/


			if (!bISETF_SAVE_ACC && (pAcc->m_strAccntNum.Mid(3, 2) == "15"))
			{
				pAcc.reset();
				continue;
			}

			if (m_mapName == "IB170960" && (pAcc->m_strAccntNum.Mid(3, 2) == "12"))
			{
				pAcc.reset();
				continue;
			}

			if (!IsFirstValidAcc(pAcc->m_strAccntNum))
			{
				pAcc.reset();
				continue;
			}
			
			if (!m_bAllowDelegation && pAcc->m_bDelegation==0)
			{
				pAcc.reset();
				continue;
			}

			//2013.04.23 KSJ HTS 7700화면에서 은행연계 계좌가 있으면 WINE관련 배너를 노출하도록 하기 위해서.
			//은행계좌를 검출한다.
			strTemp = pAcc->m_strAccntNum.Mid(0,3);
			if (!m_bBankLinked && (strTemp=="501" || strTemp =="502" || strTemp =="503" || strTemp =="504" || strTemp =="551" ||
				strTemp =="552" || strTemp =="553" || strTemp =="554" || strTemp =="555" || strTemp =="571"))
				m_bBankLinked = TRUE;
			//2013.04.23 KSJ

			//2013.05.28 KSJ 계좌명정렬시
			if (m_nSort ==1)
			{
				bool	bSwap = false;
				int	ii = 0;

				for ( ; ii < m_arrAllAcc.GetSize(); ii++)
				{
					if (pAcc->m_strAccntName.Compare(m_arrAllAcc.GetAt(ii)->m_strAccntName) < 0)
					{
						bSwap = true;
						break;
					}
				}

				if (bSwap)
					m_arrAllAcc.InsertAt(ii, pAcc);
				else
					m_arrAllAcc.Add(pAcc);
			}
			else
				m_arrAllAcc.Add(pAcc);
		}
	}
	else  //파생상품 영업팀이라면
	{
		if (!ReadAcc813())
		{
			SetTimer(TT_813REREAD, 3000, NULL);
		}
	}

	//2013.05.28 KSJ 계좌명정렬시
	if (m_nSort == 1)
	{
		LoadHistoryAccName();
		LoadGroupAccName();	
	}
	else
	{
		LoadHistoryAcc();
		LoadGroupAcc();	
	}

	//=====고객시세단말 접속용 아이디일 경우
	if (((m_strUser[0] == ONLY_SISE) && (IsNumber(m_strUser))) || /*m_onlysise == 1 ||*/ m_strUser == "##ibk9")
	{
		if (m_strUser.Compare("991601") && m_strUser.Compare("991602"))
		{
			m_bEditMode = FALSE;
			m_bOrderEnable = FALSE;
			m_pEdit->SetEditMode(m_bEditMode);
			m_arrAllAcc.RemoveAll();
		}
	}

	SetTimer(TT_INIT, 10, NULL);

	return TRUE;
}

void CAccountCtrl::AddAcc(CString strAcc, CString strPassword, CString strAccName, CString strItems)	//2013.03.12 KSJ 자금대체, 금융매매대리 전용 계좌입력
{
	std::shared_ptr<CAccount> pAcc = nullptr;
	BOOL	bAdd = FALSE;
	BOOL	bFilter = FALSE; //TRUE 기존과 같이 필터링한다. TRUE 필터링 하지 않는다.

	switch (m_nAgentType)
	{
	case AGENT_ITEM5:
		bAdd = strItems.GetAt(AGENT_ITEM5 - 1) == '1' ? TRUE : FALSE;
		bFilter = TRUE;
		break;

	case AGENT_ITEM6:
		bAdd = strItems.GetAt(AGENT_ITEM6 - 1) == '1' ? TRUE : FALSE;
		break;

	default:
		break;
	}

	if (!bAdd)
		return;	//해당하는 타입이 아닐때 넣어줄 필요는 없음
	
	if (IsValidAcc(strAcc, FALSE, TRUE))
		return;

	if (bFilter)	//AGENT_ITEM5 False일때는 필터링을 해주어야 한다.
	{
		BOOL	bValid = FALSE;
		CString strType = strAcc.Mid(3, 2);
		const	int	nCount = m_arrAccType.GetSize();

		for (int ii = 0; ii < nCount; ii++)
		{
			if (strType == m_arrAccType.GetAt(ii))
			{
				bValid = TRUE;
				break;
			}
		}

		if (!bValid)
			return;
	}

	pAcc = std::make_shared<CAccount>();
	pAcc->m_strAccntNum = strAcc;
	pAcc->m_strPassword = strPassword;
	pAcc->m_strAccntName = strAccName;
	pAcc->m_bAgent5 = strItems.GetAt(AGENT_ITEM5 - 1) == '1' ? TRUE : FALSE;
	pAcc->m_bAgent6 = strItems.GetAt(AGENT_ITEM6 - 1) == '1' ? TRUE : FALSE;
	pAcc->TrimData();

	//m_slog.Format("[cx_account][%s]<%d>  [AGENT][ADD]  [%s] [%s]", __FUNCTION__, __LINE__, strAccName, strAcc);
	//Output_DebugString(m_slog);

	m_arrAllAcc.Add(pAcc);
	m_pCombo->AddAccString(strAcc + _T("\t") + strAccName);

	CString strGubn, strAccText;

	strAcc.Trim();
	strAccName.Trim();

	switch (_ttoi(strAcc.Mid(3, 2)))
	{
	case 10:	strGubn = _T("주식/채권");			break;
	case 11:	strGubn = _T("ELW전용 ");			break;
	case 12:	strGubn = _T("코넥스  ");			break;
	case 15:	strGubn = _T("연금  ");			break;
	case 20:	strGubn = _T("선물옵션");			break;
	case 21:	strGubn = _T("옵션전용");			break;
	case 30:	strGubn = _T("일반저축");			break;
	case 37:	strGubn = _T("ISA");			break;
	case 51:	strGubn = _T("CMA");			break;
	case 52:	strGubn = _T("RP전용  ");			break;
	case 53:	strGubn = _T("수익증권");			break;
	case 55:	strGubn = _T("연금저축");			break;
	case 57:	strGubn = _T("ISA펀드");			break;
	case 70:	strGubn = _T("신탁    ");			break;
	default:	strGubn = _T("        ");			break;
	}
	strAccText = strAcc;
	strAccText.Insert(ACC_DEPT, _T("-"));
	strAccText.Insert(ACC_DEPT + ACC_TYPE + 1, _T("-"));

	//8101같이 계좌컨트롤을 사용하지 않고 콤보박스를 사용할 경우 필요함.
	m_strAgenAcctList += _T("\t") + strAcc + strAccText + _T("   ") + strGubn + _T("   ") + strAccName;
	m_strAgenNameList += _T("\t") + strAccName;
}

void CAccountCtrl::DeleteAcc(int nDeleteIndex)
{
	CString str;
	CString strItem;
	int	nCount = 0;
	CAccountArr* pArr = nullptr;
	std::shared_ptr<CAccount> pAcc = nullptr;

	if (m_bGroup)
	{
		if (m_nSelectGroup == 0)
		{
			m_arrAllAcc.RemoveAt(nDeleteIndex);
			pArr = &m_arrAllAcc;
		}
		else
		{
			m_arrGroupAcc.RemoveAt(nDeleteIndex);
			pArr = &m_arrGroupAcc;
		}			
	}
	else
	{
		if (nDeleteIndex < m_arrHistoryAcc.GetSize())	//2012.03.30 KSJ 인덱스가 전체히스토리 사이즈보다 작을때만 실행됨.
		{
			if (nDeleteIndex != -1)
			{
				DeleteHistory(m_arrHistoryAcc[nDeleteIndex]->m_strAccntNum);
				m_arrHistoryAcc.RemoveAt(nDeleteIndex);
				
				//2012.03.30 KSJ 인덱스가 0일때는 메모리를 전체삭제해줌.
				//하지않으면 하나만 추가하면 다시 삭제하기전의 계좌가 보임.
				if (nDeleteIndex == 0 && m_arrHistoryAcc.GetSize() == 1)
					PushDeleteAllAccHistory();
				//KSJ
			}
			else
			{
				for (int n=0; n<m_arrHistoryAcc.GetSize(); ++n)
					DeleteHistory(m_arrHistoryAcc[n]->m_strAccntNum);
				SafeRemoveAccArray(&m_arrHistoryAcc);
				//SafeRemoveAccArray(&m_arrDelectAcc);
				m_arrHistoryAcc.RemoveAll();
				m_arrDelectAcc.RemoveAll();
				//PushDeleteAccHistory();
				PushDeleteAllAccHistory();
			}
		}
		pArr = &m_arrHistoryAcc;
	}
	if (m_pCombo)
		m_pCombo->DeleteAllAccArray();
	//m_pCombo->AddAccString("계좌정보");
	//if (m_staff)
	//	m_pCombo->AddAccString(m_acif1);

	nCount = pArr->GetSize();
	for (int ii = 0; ii < nCount; ii++)
	{
		pAcc = pArr->GetAt(ii);
		strItem = pAcc->m_strAccntNum;
		strItem.Insert(ACC_DEPT, _T("-"));
		strItem.Insert(ACC_DEPT + ACC_TYPE + 1, _T("-"));
		strItem += TABSPACE;
		if (m_bUseAlias)
			strItem += pAcc->m_strAccntNick.IsEmpty() ? pAcc->m_strAccntName : pAcc->m_strAccntNick;//pAcc->m_strAccntName;
		else
			strItem += GetAcntTypeName(pAcc->m_strAccntNum.Mid(3,2))+pAcc->m_strAccntName;
		
		m_pCombo->AddAccString(strItem);
	}
	
	if (nCount > 0 && m_bGroup == FALSE && !m_pCombo->GetSearchType())	//2012.02.27 KSJ 검색시에는 전체삭제 안보이게
		m_pCombo->AddAccString(STR_ALL_DELECT);
	SaveHistory();
};

CString CAccountCtrl::Variant(int nComm, CString strData /* = _T("") */)
{
	CString strRet;
	
	const	char*	pRet = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nComm), (LPARAM)strData.GetString());
	if ((long)pRet > 1)
		strRet = pRet;
	
	return strRet;
}

void CAccountCtrl::LoadButtonImage() 
{
	CString strImgName;
	CString strImgDir(m_strRoot);
	strImgDir += _T("\\image");
	
	strImgName.Format("%s\\AXCOMBO.bmp", strImgDir);
	m_hAccArrow = (HBITMAP)::LoadImage(AfxGetApp()->m_hInstance, strImgName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	strImgName.Format("%s\\AXCOMBO_DN.bmp", strImgDir);
	m_hAccArrow_DN = (HBITMAP)::LoadImage(AfxGetApp()->m_hInstance, strImgName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	strImgName.Format("%s\\AXCOMBO_EN.bmp", strImgDir);
	m_hAccArrow_EN = (HBITMAP)::LoadImage(AfxGetApp()->m_hInstance, strImgName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	strImgName.Format("%s\\G.bmp", strImgDir);
	m_hG = (HBITMAP)::LoadImage(AfxGetApp()->m_hInstance, strImgName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	strImgName.Format("%s\\G_DN.bmp", strImgDir);
	m_hG_DN = (HBITMAP)::LoadImage(AfxGetApp()->m_hInstance, strImgName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	strImgName.Format("%s\\G_EN.bmp", strImgDir);
	m_hG_EN = (HBITMAP)::LoadImage(AfxGetApp()->m_hInstance, strImgName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	strImgName.Format("%s\\G_S.bmp", strImgDir);
	m_hG_S = (HBITMAP)::LoadImage(AfxGetApp()->m_hInstance, strImgName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	strImgName.Format("%s\\G_S_DN.bmp", strImgDir);
	m_hG_S_DN = (HBITMAP)::LoadImage(AfxGetApp()->m_hInstance, strImgName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	strImgName.Format("%s\\G_S_EN.bmp", strImgDir);
	m_hG_S_EN = (HBITMAP)::LoadImage(AfxGetApp()->m_hInstance, strImgName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	strImgName.Format("%s\\Arrow.bmp", strImgDir);
	m_hArrow = (HBITMAP)::LoadImage(AfxGetApp()->m_hInstance, strImgName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	strImgName.Format("%s\\Arrow_DN.bmp", strImgDir);
	m_hArrow_DN = (HBITMAP)::LoadImage(AfxGetApp()->m_hInstance, strImgName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	strImgName.Format("%s\\Arrow_EN.bmp", strImgDir);
	m_hArrow_EN = (HBITMAP)::LoadImage(AfxGetApp()->m_hInstance, strImgName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	BITMAP bmpG;
	BITMAP bmpArrow;
	
	::GetObject(m_hG, sizeof(BITMAP), &bmpG);
	::GetObject(m_hArrow, sizeof(BITMAP), &bmpArrow);
	
	m_szG.cx = bmpG.bmWidth;
	m_szG.cy = bmpG.bmHeight;
	m_szArrow.cx = bmpArrow.bmWidth;
	m_szArrow.cy = bmpArrow.bmHeight;
}

void CAccountCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	ResizeCtrl(cx, cy);	

}

void CAccountCtrl::ResizeCtrl(int cx, int cy)
{
	if (!m_pCombo)
		return;
	
	CFont* pFont = m_pCombo->GetFont();


	int nHeight = 0;
	int nEditWidth = 0;
	
	if (pFont)
	{
		LOGFONT lf;
		pFont->GetLogFont(&lf);
		nHeight = abs(lf.lfHeight)+11;
	}
	else
		nHeight = cy;
	

	m_pCombo->MoveWindow(0, 0 - (nHeight - cy), 0, nHeight);
	if (!IsNumber(m_strUser))
		m_pCombo->SetDroppedWidth(cx+72); // 계좌 리스트박스의 너비 설정
	else
		m_pCombo->SetDroppedWidth(cx+72+120); // 계좌 리스트박스의 너비 설정	2012.02.28 KSJ 120 -> 72로 수정// 2012.03.05 72 -> 120으로 다시 수정함.. 법인팀은 이름이 길어서 작게안됨..
	
	
	CRect	rcClient;
	
	GetClientRect(&rcClient);
	const	int	bWidth = rcClient.Width()*19/140+34;
	
	nEditWidth = rcClient.Width()-bWidth;//rcButton.left-bWidth -3;//cx - (m_szG.cx + m_szArrow.cx) - CTRLGAP - (cy - MARGIN);
	
	ResizeEdit(nEditWidth+2, cy);
}

void CAccountCtrl::ResizeEdit(int cx, int cy)
{
	if (!m_pEdit)
		return;
	
	CRect	rcEdit;
	int	nFtHeight = 0;
	const	int	sz = (int)((cy/2)-1)<(cx/8 - MARGIN)?((cy/2)-1):(cx/8 - MARGIN) ;

	m_Param.point = sz;
	if (m_Param.point>MAX_FONT_SIZE) // 화면 최대화 했을 때 계좌 전체가 보이지 않는 부분 수정(09/07/21)
		m_Param.point = MAX_FONT_SIZE;

	CFont* pFont = getAxFont(m_Param.fonts, m_Param.point, m_Param.style);
	m_pEdit->SetFont(pFont);
		
	if (pFont)
	{
		LOGFONT lf;
		pFont->GetLogFont(&lf);
		nFtHeight = abs(lf.lfHeight);
	}
	else
		nFtHeight = cy;

	rcEdit.SetRect(MARGIN, cy / 2 - nFtHeight / 2, cx - MARGIN, cy / 2 + nFtHeight / 2);
	
	m_pEdit->MoveWindow(rcEdit);
}

void CAccountCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC dcMem;
	CRect rcClient;
	CRect rcCombo;
	CRect rcButton;
	CSize sz;

	if (m_staff)
		sz = m_szG + m_szArrow;
	else
		sz = 0;
	HBITMAP hG = NULL, hArrow = NULL;
	
	GetClientRect(&rcClient);
	dcMem.CreateCompatibleDC(&dc);
	dc.FillSolidRect(rcClient, m_clrBackColor);
	dcMem.FillSolidRect(rcClient, m_clrBackColor);
	
	rcCombo = rcButton = rcClient;
	rcCombo.right = rcCombo.right - sz.cx - CTRLGAP;
	rcButton.left = rcCombo.right + CTRLGAP;
	
	// DrawBorder
	if (m_nBorderStyle == BORDERSTYLE_FIXED)
	{
		if (m_nAppearance == APPEARANCE_FLAT)
		{
			CPen	pPen(PS_SOLID, 1, m_clrBorderColor);
			CPen*	pOldPen = dc.SelectObject(&pPen);
			CBrush	pBrush(m_clrEditBackColor);
			CBrush* pOldBrush = dc.SelectObject(&pBrush);
			
			dc.Rectangle(rcCombo);
			dc.SelectObject(pOldPen);
			dc.SelectObject(pOldBrush);
			
			pPen.DeleteObject();
			pBrush.DeleteObject();
		}
		else
			dc.DrawEdge(rcCombo, EDGE_SUNKEN, BF_RECT);
	}

	// DrawDownArrow
	const	COLORREF clrBack = RGB(128, 128, 128);
	const	COLORREF clrArrow = RGB(224, 224, 224);
	
//	CPoint pt[3];
	CRect rcArrowBack, rcArrow;
	
	rcArrowBack = rcCombo;
//	rcArrowBack.DeflateRect(0, 3, 3, 0);
	rcArrowBack.left = rcArrowBack.right - rcArrowBack.Height() + 2;
	
	rcArrow = rcArrowBack;
	rcArrow.DeflateRect(3, rcArrow.Height() * 2 / 5);
	rcArrow.OffsetRect(1, 0);
	rcArrow.right = rcArrow.left + (rcArrow.Width() / 2) * 2 - 1; 
	rcArrow.bottom = rcArrow.top + rcArrow.Width() / 2;

	CPen penBtn(PS_SOLID, 1, clrBack);
	CPen* pOldBrush = dc.SelectObject(&penBtn);
/*
	dc.MoveTo(rcArrowBack.left -1, rcArrowBack.top + 1);
	dc.LineTo(rcArrowBack.left -1, rcArrowBack.bottom-1);
*/
	const	int	bWidth = rcClient.Width()*19/140-1;
	dc.MoveTo(rcButton.left-bWidth -1, rcArrowBack.top + 1);
	dc.LineTo(rcButton.left-bWidth -1, rcArrowBack.bottom-1);
	
	dc.SelectObject(&pOldBrush);

	//  비트맵 처리로 바뀌면서 주석 처리됨(2008.06.22:kbkim)
	// 	CPen penBtn(PS_SOLID, 1, clrBack);
	// 	CPen penArrow(PS_SOLID, 1, clrArrow);
	// 	CBrush brushBtn(clrBack);
	// 	CBrush brushArrow(clrArrow);
	// 	
	// 	CPen* pOldPen = dc.SelectObject(&penBtn);
	// 	CBrush* pOldBrush = dc.SelectObject(&brushBtn);
	
	// 	dc.FillRect(&rcArrowBack, &brushBtn);
	// 	dc.MoveTo(rcArrowBack.left, rcArrowBack.top + 1);
	// 	dc.LineTo(rcArrowBack.left, rcArrowBack.bottom-1);
	// 	dc.MoveTo(rcArrowBack.right-1, rcArrowBack.top+1);
	// 	dc.LineTo(rcArrowBack.right-1, rcArrowBack.bottom-1);
	// 	
	// 	dc.SelectObject(&penArrow);
	// 	dc.SelectObject(&brushArrow);
	// 	
	// 	pt[0].y = rcArrow.bottom;
	// 	pt[1].y = rcArrow.top;
	// 	pt[2].y = rcArrow.top;
	// 	pt[0].x = rcArrow.left + (rcArrow.Width() / 2);
	// 	pt[1].x = rcArrow.right - 1;
	// 	pt[2].x = rcArrow.left;
	// 	dc.Polygon(pt, 3);
	// 	
	// 	dc.SelectObject(pOldPen);
	// 	dc.SelectObject(pOldBrush);
	
	if (m_hAccArrow == NULL || m_hAccArrow_DN == NULL ||m_hAccArrow_EN == NULL || 
		m_hG == NULL || m_hG_DN == NULL || m_hG_EN == NULL || 
		m_hArrow == NULL || m_hArrow_DN == NULL || m_hArrow_EN == NULL)
		return;
	
	switch (m_nStateG)
	{
	case ST_GNORMAL:	hG = m_nSelectGroup == 0 ? m_hG : m_hG_S;	break;
	case ST_GDOWN:		hG = m_nSelectGroup == 0 ? m_hG_DN : m_hG_S_DN;	break;
	case ST_GOVER:
	default:		hG = m_nSelectGroup == 0 ? m_hG_EN : m_hG_S_EN;	break;
	}
	
	switch (m_nStateArrow)
	{
	case ST_ARROWNORMAL:	hArrow = m_hArrow;	break;
	case ST_ARROWDOWN:	hArrow = m_hArrow_DN;	break;
	case ST_ARROWOVER:
	default:		hArrow = m_hArrow_EN;	break;
	}
	

	CBitmap* pOldBitmap = dcMem.SelectObject(CBitmap::FromHandle(m_hAccArrow));
	dc.StretchBlt(/*rcArrowBack.left+1*/rcButton.left-bWidth+1, 0, bWidth/*rcButton.left - rcArrowBack.left -2*/, rcArrowBack.Height(), &dcMem, 0, 0, 17, 20, SRCCOPY);

	CString str;
	str.Format("[rcArrowBack.left:%d,rcClient.Width():%d][m_szArrow.cy:%d]\n", rcArrowBack.left, rcClient.Width(),m_szArrow.cy);
// 	TRACE(str);
	
	pOldBitmap = dcMem.SelectObject(CBitmap::FromHandle(hG));
	dc.StretchBlt(rcButton.left, 0, m_szG.cx, rcButton.Height(), &dcMem, 0, 0, m_szG.cx, m_szG.cy, SRCCOPY);

	dcMem.SelectObject(CBitmap::FromHandle(hArrow));
	dc.StretchBlt(rcButton.left + m_szG.cx, 0, m_szArrow.cx, rcButton.Height(), &dcMem, 0, 0, m_szArrow.cx, m_szArrow.cy, SRCCOPY);
	
	dcMem.SelectObject(pOldBitmap);
}

void CAccountCtrl::SetButtonState(CPoint point, int nMouseState)
{
	if (nMouseState == MS_NORMAL)
	{
		m_nStateG = m_nStateArrow = nMouseState;
		Invalidate();
		return;
	}
	
	UINT	nStateG = 0, nStateArrow = 0;
	CRect	rc;

	GetClientRect(&rc);
	rc.left = rc.right - (m_szG.cx + m_szArrow.cx);
	rc.right = rc.left + m_szG.cx;
	
	if (rc.PtInRect(point))
	{
		nStateG = nMouseState;
		nStateArrow = ST_ARROWNORMAL;

		if (m_nStateG != nStateG || m_nStateArrow != nStateArrow)
		{
			m_nStateG = nStateG;
			m_nStateArrow = nStateArrow;
			Invalidate();
		}

		m_bMultiClick = FALSE;
	}
	else
	{
		rc.left = rc.right;
		rc.right = rc.left + m_szArrow.cx;
		
		if (rc.PtInRect(point))
		{
			nStateG = ST_GNORMAL;
			nStateArrow = nMouseState;
			
			if (m_nStateG != nStateG || m_nStateArrow != nStateArrow)
			{
				m_nStateG = nStateG;
				m_nStateArrow = nStateArrow;
				Invalidate();
			}
			
			if (nMouseState == MS_DOWN)
				m_bMultiClick = TRUE;
		}
		else
			m_bMultiClick = FALSE;
	}
}

void CAccountCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect	rcClient, rcArrow, rcList, rcGroup;

	GetClientRect(&rcClient);
	rcArrow = rcClient;
	rcArrow.right = rcArrow.right - (m_szG.cx + m_szArrow.cx) - CTRLGAP;
	/*
	if (m_staff)
		rcArrow.right = rcArrow.right - (m_szG.cx + m_szArrow.cx) - CTRLGAP;
	else
		rcArrow.right = rcArrow.right - CTRLGAP;
	*/	
//	rcArrow.DeflateRect(0, 3, 2, 3);
	rcArrow.left = rcArrow.right - rcArrow.Height();
	if (!m_staff)
	{
		ShowAccountList(true);
	}
	else if (rcArrow.PtInRect(point))
		ShowAccountList(FALSE);
	else
	{
		SetButtonState(point, MS_DOWN);

		rcList = rcClient;
		rcList.left = rcList.right - m_szArrow.cx;

		rcGroup = rcList;
		rcGroup.right = rcList.left;
		rcGroup.left = rcGroup.right - m_szG.cx;

		if (rcList.PtInRect(point))
			ShowGroupList();
		else if (rcGroup.PtInRect(point))
			ShowAccountList(TRUE);
	}
	Invalidate();
	CWnd::OnLButtonDown(nFlags, point);
}

void CAccountCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect	rcButton;

	GetClientRect(&rcButton);
	rcButton.left = rcButton.right - (m_szG.cx + m_szArrow.cx);
	CONST	BOOL	bIn = rcButton.PtInRect(point);

	if (!m_bTrackLeave && bIn) 
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		_TrackMouseEvent(&tme);
		m_bTrackLeave = TRUE;
		SetButtonState(point, MS_OVER);
	}
	else if (m_bTrackLeave && bIn)
	{
		CRect	rc = rcButton;

		if (m_nStateG == ST_GOVER)
			rc.left = rc.right - m_szArrow.cx;
		else if (m_nStateArrow == ST_ARROWOVER)
			rc.right = rc.left + m_szG.cx;

		if (rc.PtInRect(point) && (m_nStateG == ST_GOVER || m_nStateArrow == ST_ARROWOVER))
			SetButtonState(point, MS_OVER);
	}
	else if (m_bTrackLeave && !bIn)
	{
		m_bTrackLeave = FALSE;
		SetButtonState(CPoint(0, 0), MS_NORMAL);
	}

	CWnd::OnMouseMove(nFlags, point);
}

LPARAM CAccountCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bTrackLeave = FALSE;
	SetButtonState(CPoint(0, 0), MS_NORMAL);
	
	return 0;
}

void CAccountCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CRect rcButton;
	GetClientRect(&rcButton);
	rcButton.left = rcButton.right - (m_szG.cx + m_szArrow.cx);

	if (m_bTrackLeave && rcButton.PtInRect(point))
	{
		SetButtonState(point, MS_OVER);
	}

	CWnd::OnLButtonUp(nFlags, point);
}

CString CAccountCtrl::Parser(CString &strSrc, CString strSub)
{
	CString strTemp(_T(""));

	if (strSrc.Find(strSub) == -1)
	{
		strTemp = strSrc;
		strSrc.Empty();
		return strTemp;
	}
	strTemp = strSrc.Left(strSrc.Find(strSub));
	strSrc = strSrc.Mid(strSrc.Find(strSub) + strSub.GetLength());
	return strTemp;
}

void CAccountCtrl::SetFont(CFont* pFont, BOOL bRedraw /* = TRUE */)
{
	CWnd::SetFont(pFont, bRedraw);

	CRect	rcClient;

	m_pEdit->SetFont(pFont, bRedraw);
	m_pCombo->SetFont(pFont, bRedraw);

	GetClientRect(&rcClient);
	ResizeCtrl(rcClient.Width(), rcClient.Height());
}

BOOL CAccountCtrl::CreateFont()
{
	struct	_fontR	fontR{};

	fontR.name = (LPSTR)(LPCTSTR)m_Param.fonts;
	fontR.point = m_Param.point;
	fontR.italic = false;
	fontR.bold = FW_NORMAL;

	m_pFont = (CFont*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(getFONT, 0), (LPARAM)&fontR);

	return (m_pFont == nullptr) ? FALSE : TRUE;
}

void CAccountCtrl::ShowAccountList(BOOL bGroup, BOOL bSearch)
{
	if (!m_bEnableCtrl)
		return;

	Invalidate();

	//=====고객시세단말 접속용 아이디일 경우
	if (((m_strUser[0] == ONLY_SISE) &&(IsNumber(m_strUser))) || /*m_onlysise == 1 ||*/ m_strUser == "##ibk9")
		return;

	CAccountArr	*pArr = nullptr;
	std::shared_ptr<CAccount> pAcc = nullptr;
	int	nCount = 0;
	CString strItem;
	BOOL	bAddAccount = FALSE;
	
	m_bGroup = bGroup;
	m_pCombo->SetSearchType(FALSE);
	if (bGroup)
	{
		if (m_nSelectGroup == 0)
		{
			pArr = &m_arrAllAcc;
		}
		else
		{
			if (m_pCombo->GetGroupKey() != m_nSelectGroup)
			{
				//2013.05.28 KSJ 계좌명정렬시
				if (m_nSort ==1)
				{
					LoadGroupAccName();	
				}
				else
				{
					LoadGroupAcc();	
				}
			}

			pArr = &m_arrGroupAcc;
		}

		m_pCombo->SetGroupKey(m_nSelectGroup);
	}
	else if (bSearch)
	{
		m_pCombo->SetSearchType(TRUE);
		pArr = &m_arrSearchAcc; 
	}
	else
	{
		pArr = &m_arrHistoryAcc; 
	}

	m_pCombo->DeleteAllAccArray();
	//m_pCombo->AddAccString("계좌정보");
	//if (m_staff)
	//	m_pCombo->AddAccString(m_acif1);
	
	nCount = pArr->GetSize();
	for (int ii = 0; ii < nCount; ii++)
	{
		pAcc = pArr->GetAt(ii);
		strItem = pAcc->m_strAccntNum;
		strItem.Insert(ACC_DEPT, _T("-"));
		strItem.Insert(ACC_DEPT + ACC_TYPE + 1, _T("-"));
		strItem += TABSPACE;
		if (m_bUseAlias)
			strItem += pAcc->m_strAccntNick.IsEmpty() ? pAcc->m_strAccntName : pAcc->m_strAccntNick;//pAcc->m_strAccntName;
		else
			strItem += GetAcntTypeName(pAcc->m_strAccntNum.Mid(3,2))+pAcc->m_strAccntName;

		if ((pArr==&m_arrAllAcc) && m_staff)
		{
			if ((pAcc->m_bWrap && !m_bWrap) || (!pAcc->m_bWrap && m_bWrap))
			{
				continue;
			}
		}

		m_pCombo->AddAccString(strItem);
		bAddAccount = TRUE;
	}

	if (bAddAccount && m_bGroup == FALSE && !bSearch)	//2012.02.27 KSJ 검색시에는 전체삭제 안보이게
		m_pCombo->AddAccString(STR_ALL_DELECT);

	m_pCombo->SetGroupList(bGroup);

	if (m_pCombo->GetCount() == 0)
		m_pCombo->AddAccString(_T(""));

	CRect	rc;

	GetClientRect(&rc);
	m_pCombo->SendMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(rc.Width(), rc.Height()));
	m_pCombo->Invalidate(FALSE);
	m_pCombo->SetGroup(bGroup);
	m_pCombo->ShowDropDown();

	if (!bSearch)
	{
		m_pCombo->SetFocus();
	}
	Invalidate();
}

void CAccountCtrl::ShowGroupList()
{
	if (!m_bEnableCtrl)
		return;

	Invalidate();

	if (((m_strUser[0] == ONLY_SISE) &&(IsNumber(m_strUser))) || /*m_onlysise == 1 ||*/ m_strUser == "##ibk9")
		return;

	CRect  rcGroupList;
	int	nCount = 0;
	std::shared_ptr<CGroup> pGroup = nullptr;

	if (m_pGroupList == nullptr)
	{
		m_pGroupList = std::make_unique<CMenu>();
		m_pGroupList->CreatePopupMenu();
		m_pGroupList->AppendMenu(MF_STRING, 0, "전체");
		
		nCount = m_arrGroupList.GetSize();
		if (nCount > 0)
			m_pGroupList->AppendMenu(MF_SEPARATOR);

		for (int ii = 0; ii < nCount; ii++)
		{
			pGroup = m_arrGroupList.GetAt(ii);
			m_pGroupList->AppendMenu(MF_STRING, ii + 1, (LPCTSTR)pGroup->m_strGrName);
		}
		if (nCount > 0)
			m_pGroupList->AppendMenu(MF_SEPARATOR);
		m_pGroupList->AppendMenu(MF_STRING, (nCount+1), "계좌정보");
	}

	GetWindowRect (&rcGroupList);
	rcGroupList.left = rcGroupList.right - m_szArrow.cx;
	int	itmp = -1;
	
	itmp = m_pGroupList->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_RETURNCMD,
								rcGroupList.left, rcGroupList.bottom, this);
	nCount = m_arrGroupList.GetSize();
	CString strAcc2, strDept;
	if (itmp == nCount+1)
	{
		//QueryAccntInfo("");
		strAcc2 = m_pEdit->GetInputData();
		BOOL bList = FALSE;
		// 전계좌 조회 부서 혹은 지점 체크
		if (m_bEditMode) //직원이라면 지점번호받아옴
		{
			strDept = Variant(deptCC);
			strDept.Trim();

			if (GetAllowAllAcc(strDept))  //전체 허용 지점 리스트 체크
			{
				bList = TRUE;
			}
			else
			{
				std::shared_ptr<CAccount> pAcc = nullptr;
				
				nCount = m_arrAllAcc.GetSize();
				for (int ii = 0; ii < nCount; ii++)
				{
					pAcc = m_arrAllAcc.GetAt(ii);
					if (strAcc2 == pAcc->m_strAccntNum)
					{
						bList = TRUE;
						break;
					}
				}
			}
		}
		if (bList)
		{
			QuerySACMQ748();
		}
		else
		{
			Variant(guideCC, "관리계좌만 조회가능합니다.");
			return;
		}
	}
	else
	{
		m_nSelectGroup = itmp;
	}
	Invalidate();
}

void CAccountCtrl::OnComboSelChange()
{
	//2012.02.14 KSJ 콤보박스를 클릭할때마다 마스크를 바꿔준다.
	m_pEdit->SetAccMask();

	CString strText(_T(""));
	const	int	nIndex = m_pCombo->GetCurSel();
	
	//if ((nIndex == 0) && m_staff){
		/*
		if (m_pEdit)
		{
			m_pEdit->SetInputData("");
			QueryAccntInfo(_T(""));
		}
		*/
	//	return;
	//}
	if (nIndex == -1)
	{
		if (m_pEdit)
		{
			m_pEdit->SetInputData("");
			//QueryAccntInfo(_T(""));
		}
		return;
	}

	strText = m_pCombo->GetListCtrlText(nIndex);
	
	if (strText == STR_ALL_DELECT)
	{
		m_pCombo->DeleteAllAcc();
		m_pCombo->DeleteAllAccArray();
		SafeRemoveAccArray(&m_arrHistoryAcc);

		DeleteAcc(-1);
		SaveHistory();
		PushDeleteAllAccHistory();
		return;
	}

	if (strText.IsEmpty())
		return;

	std::shared_ptr<CAccount> pAcc = nullptr;
	CAccountArr* pArr = nullptr;
	if (m_pCombo->GetGroup())
	{
		if (m_nSelectGroup == 0)
			pArr = &m_arrAllAcc;
		else
			pArr = &m_arrGroupAcc;
	}
	else if (m_pCombo->GetSearchType())	//2012.02.27 KSJ 검색시에는 전체계좌에서 검색
	{
		pArr = &m_arrAllAcc;
	}
	else
	{
		pArr = &m_arrHistoryAcc;
	}

	CString strAcntNum = strText;
	strAcntNum.Replace("-", "");
	strAcntNum = strAcntNum.Left(11);
	for (int ii = 0; ii < pArr->GetSize(); ii++)
	{
		if (pArr->GetAt(ii)->m_strAccntNum == strAcntNum)
		{
			pAcc = pArr->GetAt(ii);
			break;
		}
	}
	CString slog;
	if (pAcc)
	{
		if (m_bUseAlias)
			m_strAccName = pAcc->m_strAccntNick.IsEmpty() ? pAcc->m_strAccntName : pAcc->m_strAccntNick;
		else
			m_strAccName = pAcc->m_strAccntName;
		
		//slog.Format("[cx_account] [%d] m_strAccName=[%s]\r\n", m_pCombo->GetSearchType(), m_strAccName);
		//Output_DebugString(slog);
		PushAccHistory(pAcc->m_strAccntNum, pAcc->m_strPassword, m_strAccName);
	}
	else
	{
		//slog.Format("[cx_account] [%d] no pAcc\r\n");
		//Output_DebugString(slog);
		//Output_DebugString(slog);
	}
	
	if (!m_pEdit)
		return;

	if (!strText.IsEmpty())
	{
		m_pEdit->SetInputData(strText);
		//QueryAccntInfo(strText);
	}


	CString strAccount = m_pEdit->GetInputData();

	if (strAccount.GetLength() == 11)
		m_strPassword = GetPassword(strAccount);

	SetFocus();
	DeleteDeleteAccList(strAccount);

	if (m_pParent && m_bInit)
	{
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange/*Change*/)), 
						(LPARAM)m_Param.name.GetString());
	}
	SaveHistory();

	//test checkacc
	QueryAccntCheck(strAccount);  //OnComboSelChange

	Invalidate();
}

void CAccountCtrl::LoadHistoryAcc()
{
	if (m_strDept == m_sAccnDept)
		return;
	
	SafeRemoveAccArray(&m_arrHistoryAcc);

	CString strData, strTemp, strPop = PopAccHistory();
	std::shared_ptr<CAccount> pAcc = nullptr;

	while (!strPop.IsEmpty())
	{
		strData = Parser(strPop, _T("|"));
		strTemp = Parser(strData, _T("\t"));
		strTemp.TrimRight();
		if (!IsValidAcc(strTemp, FALSE))
			continue;

		pAcc = std::make_shared<CAccount>();
		pAcc->m_strAccntNum = strTemp;
		pAcc->m_strPassword = Parser(strData, _T("\t"));
		pAcc->m_strAccntName = Parser(strData, _T("\t"));
		pAcc->TrimData();

		m_arrHistoryAcc.Add(pAcc);
	}
}

//2013.05.28 KSJ 계좌명정렬
void CAccountCtrl::LoadHistoryAccName()
{
	if (m_strDept == m_sAccnDept)
		return;
	
	SafeRemoveAccArray(&m_arrHistoryAcc);

	int	index = 0;
	bool	bSwap = false;
	CString strData, strTemp, strPop = PopAccHistory();
	std::shared_ptr<CAccount> pAcc = nullptr;
	
	while (!strPop.IsEmpty())
	{
		strData = Parser(strPop, _T("|"));
		strTemp = Parser(strData, _T("\t"));
		strTemp.TrimRight();
		if (!IsValidAcc(strTemp, FALSE))
			continue;

		pAcc = std::make_shared<CAccount>();
		pAcc->m_strAccntNum = strTemp;
		pAcc->m_strPassword = Parser(strData, _T("\t"));
		pAcc->m_strAccntName = Parser(strData, _T("\t"));
		pAcc->TrimData();

		bSwap = false;
		index = 0;
		for ( ; index < m_arrHistoryAcc.GetSize(); index++)
		{
			if (pAcc->m_strAccntName.Compare(m_arrHistoryAcc.GetAt(index)->m_strAccntName) < 0)
			{
				bSwap = true;
				break;
			}
		}
		
		if (bSwap)
			m_arrHistoryAcc.InsertAt(index, pAcc);
		else
			m_arrHistoryAcc.Add(pAcc);
	}
}

void CAccountCtrl::PushAccHistory(CString strAcc, CString strPassword, CString strAccName)
{
	CString strData;
	CString	strPush, strPushKey;
	CString strPop = PopAccHistory();

	strPushKey.Format("%s%d", PP_SYM, 0);
	strData.Format("%s\t%s\t%s\t%s|", strPushKey, strAcc, strPassword, strAccName);

	if (strPop.IsEmpty())
	{
		strPush = strData;
	}
	else
	{
		const	int	nStartPos = strPop.Find((LPCTSTR)strAcc);
		int	nEndPos = -1;

		if (nStartPos != -1)
		{
			nEndPos = strPop.Find(_T("|"), nStartPos);

			if (nEndPos != -1)
				strPop.Delete(nStartPos, nEndPos - nStartPos + 1);
		}

		strPush = strData + strPop;
	}
	
	Variant(pushCC, strPush);
}

////////////////////////// 
// 함수명	    : CAccountCtrl::PushDeleteAccHistory
// 설 명	    : 사용자가 삭제한 계좌 정보의 내용을 메모리에서 삭제하는 기능
// 작성일시	    : 2009/06/02
// 작성자       : kbkim
// 파라미터	    : 
// 리턴 값 		: 
// 관련요건	    : 000010(조원형:2009/05/12)
////////////////////////// 
void CAccountCtrl::PushDeleteAccHistory()
{
	CString strData;
	CString strPush, strPushKey;
	CString strPop = PopAccHistory();
	int	nDeleteAccIndex = 0;
	std::shared_ptr<CAccount> pAcc = nullptr;

	strPushKey.Format("%s%d", PP_SYM, 0);
	if (strPop.IsEmpty())
	{
		strPush = strPop;
	}
	else
	{
		strPush = strPop;
		nDeleteAccIndex = m_arrDelectAcc.GetSize();
		for (int nIndex = 0; nIndex < nDeleteAccIndex; nIndex++)
		{
			pAcc = m_arrDelectAcc.GetAt(nIndex);
			strData.Format("%s\t%s\t%s|", pAcc->m_strAccntNum, "", pAcc->m_strAccntName);
			strPush.Replace(strData, "");
		}	
	}

	Variant(pushCC, strPushKey + "\t" + strPush);
}

CString CAccountCtrl::PopAccHistory()
{
	CString strPopKey;

	strPopKey.Format("%s%d", PP_SYM, 0);
	
	return Variant(popCC, strPopKey);
}

void CAccountCtrl::QueryGroupList()
{
	char	datb[L_gmid + 128]{};
	struct	_gmid* gmid = (struct _gmid*)datb;

	gmid->xflg = '1';
	CopyMemory(gmid->usrid, (char *)m_strUser.GetString(), sizeof(gmid->usrid));
	CopyMemory(gmid->grec, "0000", sizeof(gmid->grec));

	switch (m_nType)
	{
	case SEL_ST:
		gmid->sflag = '1';				// 현물 조회
		break;
	case SEL_FU:
		gmid->sflag = '2';				// 선물옵션 조회
		break;
	default:						// 현재 두종류 외에는 그룹계좌 지원안함
		return;
	}

	CString strData;

	//2013.01.15 KSJ	PIDOSETA가 어디서 문제가 생기는지 보기 위해서. 구조체 마지막에 맵이름과, UserID를 넣는다.
	CString strMapName1 = (LPCSTR)m_pParent->SendMessage(WM_USER, MAKEWPARAM(mapDLL, 0), 0);
	CString strUserName = Variant(userCC);

	strData.Format("[%s][%s]", strUserName, strMapName1);
	CopyMemory(&datb[L_gmid], (char*)strData.GetString(), strData.GetLength());
	SendTR(_T("PIDOSETa"), NULL, datb, L_gmid + strData.GetLength(), TRKEY_GROUP);
	//KSJ
}

void CAccountCtrl::LoadGroupAcc()
{
	if (m_nSelectGroup == 0)
		return;

	m_arrGroupAcc.RemoveAll();

	std::shared_ptr<CAccount> pAcc = nullptr;
	std::shared_ptr<CGroup> pGroup = m_arrGroupList.GetAt(m_nSelectGroup - 1);
	const	int	nCount = pGroup->m_arrAccount.GetSize();

	for (int ii = 0; ii < nCount; ii++)
	{
		pAcc = pGroup->m_arrAccount.GetAt(ii);
		if (!pAcc->m_strAccntNum.IsEmpty() && IsValidAcc(pAcc->m_strAccntNum))
			m_arrGroupAcc.Add(pAcc);
	}
}

//2013.05.28 KSJ 계좌명 정렬
void CAccountCtrl::LoadGroupAccName()
{
	if (m_nSelectGroup == 0)
		return;
	
	m_arrGroupAcc.RemoveAll();
	
	std::shared_ptr<CAccount> pAcc = nullptr;
	std::shared_ptr<CGroup> pGroup = m_arrGroupList.GetAt(m_nSelectGroup - 1);
	const	int	nCount = pGroup->m_arrAccount.GetSize();
	
	for (int ii = 0; ii < nCount; ii++)
	{
		pAcc = pGroup->m_arrAccount.GetAt(ii);
		if (!pAcc->m_strAccntNum.IsEmpty() && IsValidAcc(pAcc->m_strAccntNum))
		{
			bool	bSwap = false;
			int	nAcc = 0;

			for (nAcc = 0; nAcc < m_arrGroupAcc.GetSize(); nAcc++)
			{
				if (pAcc->m_strAccntName.Compare(m_arrGroupAcc.GetAt(nAcc)->m_strAccntName) < 0)
				{
					bSwap = true;
					break;
				}
			}

			if (bSwap)
				m_arrGroupAcc.InsertAt(nAcc, pAcc);
			else
				m_arrGroupAcc.Add(pAcc);
		}
	}
}

BOOL CAccountCtrl::IsValidAcc(CString strAccount, BOOL bOrderCheck, BOOL bAgent)
{
	if (!m_bOrderEnable && bOrderCheck)
		return FALSE;

	strAccount.Remove(_T('-'));
	strAccount.Trim();

	if (strAccount.IsEmpty() || strAccount.GetLength() != 11)
		return FALSE;

	BOOL	bValid = FALSE;
	CString strType = strAccount.Mid(3, 2);
	int	nCount = 0;
	std::shared_ptr<CAccount> pAcc = nullptr;

	nCount = m_arrAccType.GetSize();
	for (int ii = 0; ii < nCount; ii++)
	{
		if (strType == m_arrAccType.GetAt(ii))
		{
			bValid = TRUE;
			break;
		}
	}

	for (int ii = 0; ii < m_arrAllAcc.GetSize(); ii++)
	{
		pAcc = m_arrAllAcc.GetAt(ii);
		if (strAccount == pAcc->m_strAccntNum)
		{
			if ((m_bWrap && !pAcc->m_bWrap) || (!m_bWrap && pAcc->m_bWrap))
			{
				bValid = FALSE;
			}
			break;
		}
	}
	
	//2013.03.12 KSJ Agent를 추가하기 위해서 연결한 것이라면
	if (!bValid && !bAgent)
		return bValid;

	bValid = TRUE;

	// 사용자의 의해 삭제된 계좌는 리스트로 나타내지 않는 부분
	
	if (!m_bGroup)  // 그룹을 선택했을 때는 삭제된 계좌도 나올 수 있도록 처리
	{
		nCount = m_arrDelectAcc.GetSize();
		for (int ii = 0; ii < nCount; ii++)
		{
			pAcc = m_arrDelectAcc.GetAt(ii);
			if (strAccount == pAcc->m_strAccntNum)
				return FALSE;
		}
	}
	

	//- 
	BOOL bList = FALSE;
	nCount = m_arrAllAcc.GetSize();
	for (int ii = 0; ii < nCount; ii++)
	{
		pAcc = m_arrAllAcc.GetAt(ii);
		if (strAccount == pAcc->m_strAccntNum)
		{
			bList = TRUE;
			break;
		}
	}

	m_dept = Variant(deptCC);  //트리거 데이터를 날릴때 지점 체크를 하기 위해 하나더 추가한것임
	m_dept.Trim();
	
	// 전계좌 조회 부서 혹은 지점 체크
	CString strDept;
	if (m_bEditMode) //직원이라면 지점번호받아옴
		strDept = m_dept;

	if (GetAllowAllAcc(strDept))  //전체 허용 지점 리스트 체크
	{
		if (strAccount.GetLength() >= 3 && strDept.GetLength() >= 3)
			bList = TRUE;
	}
	else if (m_bEditMode && m_bOrderEnable && !bList)	// 직원이면서 관리 계좌가 아닌 경우
	{
		// 직원과 입력한 계좌명의 부서코드가 동일하면 관리 계좌가 아니더라도 True
		if (strAccount.GetLength() >= 3 && strDept.GetLength() >= 3)
		{
			//지점코드 확인하는 로직 폐쇄
			//if (strAccount.Left(3) == strDept.Left(3))
			bList = TRUE;
		}
	}

	if (bValid && bList)
		return TRUE;

	return FALSE;
}

void CAccountCtrl::SafeRemoveAccArray(CAccountArr* pArr)
{
	std::shared_ptr<CAccount> pAcc = nullptr;

	for (int ii = 0; ii < pArr->GetSize(); ii++)
	{
		pAcc = pArr->GetAt(ii);
		if (pAcc)
			pAcc.reset();
	}

	pArr->RemoveAll();
}

CString CAccountCtrl::GetPassword(CString strAccount)
{
	CString strPassword(_T(""));
	
	if (strAccount.GetLength() == 11)
		strPassword = Variant(passCC, strAccount);

	return strPassword;
}

//#define DF_SEARCHMODE  //중복계좌가 있는경우 조회모드
//#define DF_JUSTSEARCH  1     //그냥 조회모드
void CAccountCtrl::AccountInputComplete(CString strAccount)
{
	CString slog;
	//slog.Format("[cx_account] strAccount=[%s]\r\n", strAccount);
	//Output_DebugString(slog);

	if (strAccount.IsEmpty())
	{
		if (m_Param.name.Find(_T("AN3A")) >= 0 && !m_bEditMode)
		{
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange/*Change*/)),     //해외
						(LPARAM)m_Param.name.GetString());
		}
		return;
	}
	m_bKeyIn = true;
	if (strAccount.GetLength() == 11)
		m_strPassword = GetPassword(strAccount);
	else
		m_strPassword.Empty();
	
	if (m_pParent && m_bInit && m_bEditMode)
	{
		m_bFireEvent = TRUE;
		DeleteDeleteAccList(strAccount);

#ifdef DF_SEARCHMODE
		int ifind = 0;
		std::shared_ptr<CAccount> pAcc = AccFindFromArr(strAccount, TRUE, ifind);
		if (pAcc == nullptr)
			pAcc = AccFindFromArr(strAccount, FALSE, ifind);

		if (pAcc == nullptr || pAcc->m_strAccntName.IsEmpty() || ifind > 1)
		{
			QueryAccntName(strAccount);
			SetTimer(TT_QUERYACCNM, 10000, NULL);

			return;
		}
#elif   DF_JUSTSEARCH == 1
		slog.Format("[cx_account]DF_JUSTSEARCH \r\n");
		Output_DebugString(slog);

		std::shared_ptr<CAccount> pAcc;
		QueryAccntName(strAccount);
		SetTimer(TT_QUERYACCNM, 10000, NULL);
		return;
#else
		std::shared_ptr<CAccount> pAcc = FindAccountInfo(strAccount, TRUE);
//m_slog.Format("AccountInputComplete1  pAcc= [%x]", pAcc);
//FileLog(m_strRoot, m_slog);
		if (pAcc == nullptr)
			pAcc = FindAccountInfo(strAccount, FALSE);
//m_slog.Format("AccountInputComplete2  pAcc= [%x]", pAcc);
//FileLog(m_strRoot, m_slog);
		if (pAcc == nullptr || pAcc->m_strAccntName.IsEmpty())
		{
			QueryAccntName(strAccount);
			SetTimer(TT_QUERYACCNM, 10000, NULL);

			return;
		}
#endif
		if (strAccount == pAcc->m_strAccntNum)
		{
			// 2011.06.13 랩계좌 관련화면에서 입력시, 코스콤에서 검증하도록 변경
#if 0
			if (m_bWrap && !pAcc->m_bWrap)
			{
				MessageBox("랩약정 계좌를 입력하십시오.", "IBK투자증권", MB_OK);
				m_pEdit->SetWindowText("");
				m_pEdit->SetFocus();
				m_strAccName = "";
				m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange/*Change*/)), 
							(LPARAM)m_Param.name.GetString());
				return;
			}
#endif
			if (!m_bWrap && pAcc->m_bWrap)
			{
				MessageBox("랩약정 계좌를 입력하실 수 없습니다.", "IBK투자증권", MB_OK);
				m_pEdit->SetWindowText("");
				m_pEdit->SetFocus();
				m_strAccName = "";
				m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange/*Change*/)), 
							(LPARAM)m_Param.name.GetString());
				return;
			}
		}

		QueryAccntCheck(m_pEdit->GetInputData());  //test AccountInputComplete(...)

		m_strAccName = pAcc->m_strAccntName;
		PushAccHistory(strAccount, _T(""), m_strAccName);
	}

	if (m_pParent)
	{
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange/*Change*/)),   //해외 일반고객
				(LPARAM)m_Param.name.GetString());
	}

	SaveHistory();
}
std::shared_ptr<CAccount> CAccountCtrl::AccFindFromArr(CString strAccount, BOOL bAll, int& ifind)
{
	int cnt = 0;
	int index = 0;
	std::shared_ptr<CAccount> pAcc = nullptr;
	CArray<std::shared_ptr<CAccount>, std::shared_ptr<CAccount>>* pArrAcc = bAll ? &m_arrAllAcc : &m_arrHistoryAcc;
	const	int	nCount = pArrAcc->GetSize();

	for (int ii = 0; ii < nCount; ii++)
	{
		pAcc = pArrAcc->GetAt(ii);
		if (pAcc->m_strAccntNum == strAccount)
		{
			index = ii;
			cnt++;
			ifind++;
		}
	}
	if (cnt > 0)
		return pArrAcc->GetAt(index);
	else
		return nullptr;
}

std::shared_ptr<CAccount> CAccountCtrl::FindAccountInfo(CString strAccount, BOOL bAll)
{
	std::shared_ptr<CAccount> pAcc = nullptr;
	CArray<std::shared_ptr<CAccount>, std::shared_ptr<CAccount>>* pArrAcc = bAll ? &m_arrAllAcc : & m_arrHistoryAcc;
	const	int	nCount = pArrAcc->GetSize();
		
	for (int ii = 0; ii < nCount; ii++)
	{
		pAcc = pArrAcc->GetAt(ii);
		if (pAcc->m_strAccntNum == strAccount)
			return pAcc;
	}

	return nullptr;
}

LRESULT CAccountCtrl::OnMessage(WPARAM wParam, LPARAM lParam)
{
	CString strlParam, strNameData;

	//TRACE("OnMessage: [%d]\n", LOBYTE(LOWORD(wParam)));
	switch (LOBYTE(LOWORD(wParam))) 
	{
	case DLL_INIT:
		/*
		if (!Initialize(FALSE))
		{
			TRACE("AccountCtrl Initialize Failed\n");
			return 0;
		}
		*/
		//SetTimer(TT_GETINIT, 500, NULL);
		break;
	case DLL_INB:
		if (m_pEdit)
		{
			m_strRet = m_pEdit->GetInputData();
			return (long)m_strRet.GetString();
		}
		break;

	case DLL_OUBx:
		struct	_extTHx* exth;
		exth = (struct _extTHx*)lParam;
		if (exth->size == 0)
			break;

		switch ((BYTE)exth->key)
		{
		case TRKEY_CEHCKACC:
		{
			char* pBuf = (char*)exth->data;
			CString strAccount, strAccName, strItems;

			strItems = CString(((struct _ledgerH*)exth->data)->ecod, 4);
			strItems = CString(((struct _ledgerH*)exth->data)->emsg, 99).Trim();

			if (strItems.Left(1) != "0")
				return 0;

			pBuf = pBuf + L_ledgerH + L_ISACAQ029;
			const	OSACAQ029* osacmq329 = (OSACAQ029*)pBuf;

		
			m_sbAgn.Format("%.1s", osacmq329->szAgntYN);				//대리인여부  Y 대리인보유   N 대리인미보유
			m_sInvst.Format("%.1s", osacmq329->szInvstTp);					//투자자구분   0 전문금융소비자  1 일반금융소비자  9 등록안됨

			m_sInfogubn.Format("%.1s", osacmq329->szInfoPrvdTp);    //정보제공구분  0 미제공 1 제공
			m_sInfogubn.TrimRight();
			if (m_sInfogubn.IsEmpty()){
				//m_slog = "[cx_account][dll_oubx] 정보제공구분 데이터 안옴";
				//m_pAxisMainFrame->SendMessage(WM_USER, MMSG_SHARED_GUIDEMESSAGE, (LPARAM)(LPSTR)(LPCTSTR)m_slog);
				//Output_DebugString(m_slog);
				//m_sInfogubn = "0";
			}

			m_sInvsCnvs.Format("%.1s", osacmq329->szInvstCnsvTp);    //투자권유구분  0 미제공 1 제공
			m_sInvsCnvs.TrimRight();
			if (m_sInvsCnvs.IsEmpty()) {
				//m_slog = "[cx_account][dll_oubx] 투자권유구분 데이터 안옴";
				//m_pAxisMainFrame->SendMessage(WM_USER, MMSG_SHARED_GUIDEMESSAGE, (LPARAM)(LPSTR)(LPCTSTR)m_slog);
				//Output_DebugString(m_slog);
				//m_sInvsCnvs = "0";
			}

			m_sWeakInv.Format("%.1s", osacmq329->szWeakInvastrYn);    //취약투자자여부 1 동의 9 동의안함 0 해당없음
			m_sWeakInv.TrimRight();
			if (m_sWeakInv.IsEmpty()) {
				//m_slog = "[cx_account][dll_oubx] 취약투자자여부 데이터 안옴";
				//m_pAxisMainFrame->SendMessage(WM_USER, MMSG_SHARED_GUIDEMESSAGE, (LPARAM)(LPSTR)(LPCTSTR)m_slog);
				//Output_DebugString(m_slog);
				//m_sWeakInv = "0";
			}

			m_sCusGrdNm.Format("%.100s", osacmq329->szLastInclInGrdNm);    //최종고객성향등급명  ex 안정형 안정추구형 위험중립형 적극투자형 공격투자형
			m_sCusGrdNm.TrimRight();
			m_sDueDt.Format("%.8s", osacmq329->szDueDt);					 //만기일 

			m_slog.Format("[cx_account] @@@ dll_oubx 대리인여부=[%s],  투자자구분=[%s][%s], 정보제공구분=[%s][%s]", 
				m_sbAgn, 
				m_sInvst,m_sInvst=="1"?"일반금융소비자": m_sInvst == "0"?"전문투자자":"등록안됨",
				m_sInfogubn,m_sInfogubn=="1"?"정보제공":"정보미제공");
			Output_DebugString(m_slog);
		
			CString stmp;
			stmp.Format(", 투자권유구분=[%s][%s],  취약투자자여부=[%s][%s],  최종고객성향등급명=[%s], 만기일=[%s]",
				m_sInvsCnvs,m_sInvsCnvs == "1" ? "투자권유구분제공" : "투자권유구분미제공",
				m_sWeakInv,m_sWeakInv == "1" ? "취약투자자여부동의" : m_sWeakInv == "9"?"동의안함":"해당없음",
				m_sCusGrdNm , m_sDueDt);

			//m_slog += stmp;
			//if(m_pAxisMainFrame)
			//	m_pAxisMainFrame->SendMessage(WM_USER, MMSG_SHARED_GUIDEMESSAGE, (LPARAM)(LPSTR)(LPCTSTR)m_slog);
			
			m_slog.Format("[cx_account] @@@ dll_oubx 투자권유구분=[%s][%s],  취약투자자여부=[%s][%s], 최종고객성향등급명=[%s],  만기일=[%s]",
				m_sInvsCnvs, m_sInvsCnvs == "1" ? "투자권유구분제공" : "투자권유구분미제공",
				m_sWeakInv, m_sWeakInv == "1" ? "취약투자자여부동의" : m_sWeakInv == "9" ? "동의안함" : "해당없음",
				m_sCusGrdNm, m_sDueDt);
			Output_DebugString(m_slog);
		

			//WriteLog(Variant(homeCC), m_slog);
			//AfxMessageBox(sRet);
			SACAQ0239Ret_Check();
			m_bSearching = FALSE;
		}
		break;
		case TRKEY_ACINFO:
			{
				CString strAcc;
				const	struct _acif_out* acif_out = (_acif_out*)exth->data;

				m_acif2 = "";
				if (m_pEdit)
					strAcc = m_pEdit->GetInputData();
				/*
				if (strAcc.GetLength()==11)
				{
					strAcc.Insert(3,"-");
					strAcc.Insert(6,"-");
				}
				*/
				m_acif1 = CString(acif_out->lgnm, sizeof(acif_out->lgnm)); m_acif1.Trim();
				m_acif2 = CString(acif_out->prnm, sizeof(acif_out->prnm)); m_acif2.Trim();
				m_acif2.Remove(NULL); m_acif2.Remove(NULL);
				m_acif2.Remove(' ');	//전화번호 정보이기때문에 공백 제거해도 상관없음

				const	int	sLen = m_acif2.GetLength();

				if (sLen == 0)
				{
					m_acif1 = "[" + m_acif1 + "]";
				}
				else
				{
					if (sLen == 10)
					{
						m_acif2.Insert(3, '-'); m_acif2.Insert(7, '-');
					}
					else if (sLen == 11)
					{
						m_acif2.Insert(3, '-'); m_acif2.Insert(8, '-');
					}
					if (!m_acif2.IsEmpty())
						m_acif1 = ("[☎ " + m_acif2 + "] [" + m_acif1 + "]");
					else
						m_acif1 = ("[" + m_acif1 + "]");
				}
				//system.push
				CString str_conv = strAcc;
				str_conv.TrimRight();
				if (str_conv.GetLength() == 11)
				{
					str_conv.Insert(3, "-");
					str_conv.Insert(6, "-");
				}
				MessageBox("계좌: " + str_conv + "\n\n" + m_acif1, "IBK투자증권");
				PushAcntInfo(strAcc, m_acif1);
			}
			break;

		case TRKEY_GROUP:
			{
				const	struct _gmod* gmod = (_gmod*)exth->data;

				if (gmod->okgb == _T('Y'))
					ParseGroupList(exth->data);
			}
			break;
		case TRKEY_SACMQ748:
			ParseSACMQ748(exth->data);
			break;
		case TRKEY_SACMQ329:
			if (m_nAgentType != AGENT_NOITEM)
				ParseSACMQ329(exth->data);
			break;
		case TRKEY_ACCNTNAME:
			ParseAccntName(exth->data);
			break;
		default:
			if (exth->size == 11)
			{
				if (m_bInit)
				{
					CString acnt = (char*)exth->data;
					if (IsValidAcc(acnt) || m_strDept == m_sAccnDept)
					{
						if (IsValidAcc(acnt))
						{
							m_pEdit->SetInputData(acnt);
							m_strAccName = GetAcntName(acnt);
							if (m_pParent)
							{
								m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange/*Change*/)),
									(LPARAM)m_Param.name.GetString());
							}
						}
					}
				}
				else
					m_strTempDomino = CString(exth->data);
			}
			break;
		}
		break;

	case DLL_OUB:	// NOT USED. Use DLL_OUBx!!
		//key = HIBYTE(LOWORD(wParam));
			
		//tmp.Format("%d", key);
		//AfxMessageBox(tmp);
			
		if (HIWORD(wParam))
		{	
			strlParam   = (char *)lParam;
			strNameData = (char *)(lParam +L_ledgerH + L_IAccnm + 5);	//ledger빼고, 입력빼고, 출력레코드갯수빼고 남은값이 60의 배수이면 계좌명조회
			const	int	nLength = strNameData.GetLength();		//계좌명 조회값이라면 60의 배수이다.
			const	int	key = HIBYTE(LOWORD(wParam));

			if (key == TRKEY_CEHCKACC)
			{
				char* pBuf = (char*)lParam;
				CString strAccount, strAccName, strItems;

				const	struct _ledgerH* ledger = (_ledgerH*)lParam;
				
				CString strecod(STR(ledger->ecod));
				if (strecod.Left(1) != "0")
					return 0;

				pBuf = pBuf + L_ledgerH + L_ISACAQ029;
				const	OSACAQ029* osacmq329 = (OSACAQ029*)pBuf;
				if (0)
				{
					m_sbAgn.Format("%.1s", osacmq329->szAgntYN);				//대리인여부  Y 대리인보유   N 대리인미보유
					m_sInvst.Format("%.1s", osacmq329->szInvstTp);					//투자자구분   0 전문금융소비자  1 일반금융소비자  9 등록안됨

					m_sInfogubn.Format("%.1s", osacmq329->szInfoPrvdTp);    //정보제공구분  0 미제공 1 제공
					m_sInfogubn.TrimRight();
					if (m_sInfogubn.IsEmpty()) {
						m_slog = "[cx_account][dll_oubx] 정보제공구분 데이터 안옴";
						//m_pAxisMainFrame->SendMessage(WM_USER, MMSG_SHARED_GUIDEMESSAGE, (LPARAM)(LPSTR)(LPCTSTR)m_slog);
						Output_DebugString(m_slog);
					}

					m_sInvsCnvs.Format("%.1s", osacmq329->szInvstCnsvTp);    //투자권유구분  0 미제공 1 제공
					m_sInvsCnvs.TrimRight();
					if (m_sInvsCnvs.IsEmpty()) {
						m_slog = "[cx_account][dll_oubx] 투자권유구분 데이터 안옴";
						//m_pAxisMainFrame->SendMessage(WM_USER, MMSG_SHARED_GUIDEMESSAGE, (LPARAM)(LPSTR)(LPCTSTR)m_slog);
						Output_DebugString(m_slog);
					}

					m_sWeakInv.Format("%.1s", osacmq329->szWeakInvastrYn);    //취약투자자여부 1 동의 9 동의안함 0 해당없음
					m_sWeakInv.TrimRight();
					if (m_sWeakInv.IsEmpty()) {
						m_slog = "[cx_account][dll_oubx] 취약투자자여부 데이터 안옴";
						//m_pAxisMainFrame->SendMessage(WM_USER, MMSG_SHARED_GUIDEMESSAGE, (LPARAM)(LPSTR)(LPCTSTR)m_slog);
						Output_DebugString(m_slog);
					}

					m_sCusGrdNm.Format("%.100s", osacmq329->szLastInclInGrdNm);    //최종고객성향등급명  ex 안정형 안정추구형 위험중립형 적극투자형 공격투자형
					m_sCusGrdNm.TrimRight();
					m_sDueDt.Format("%.8s", osacmq329->szDueDt);					 //만기일 

					m_slog.Format("[cx_account] @@@ dll_oubx 대리인여부=[%s],  투자자구분=[%s][%s], 정보제공구분=[%s][%s]",
						m_sbAgn,
						m_sInvst, m_sInvst == "1" ? "일반금융소비자" : m_sInvst == "0" ? "전문투자자" : "등록안됨",
						m_sInfogubn, m_sInfogubn == "1" ? "정보제공" : "정보미제공");
					Output_DebugString(m_slog);

					CString stmp;
					stmp.Format(", 투자권유구분=[%s][%s],  취약투자자여부=[%s][%s],  최종고객성향등급명=[%s], 만기일=[%s]",
						m_sInvsCnvs, m_sInvsCnvs == "1" ? "투자권유구분제공" : "투자권유구분미제공",
						m_sWeakInv, m_sWeakInv == "1" ? "취약투자자여부동의" : m_sWeakInv == "9" ? "동의안함" : "해당없음",
						m_sCusGrdNm, m_sDueDt);

					m_slog += stmp;
					if (m_pAxisMainFrame)
						m_pAxisMainFrame->SendMessage(WM_USER, MMSG_SHARED_GUIDEMESSAGE, (LPARAM)(LPSTR)(LPCTSTR)m_slog);

					m_slog.Format("[cx_account] @@@ dll_oubx 투자권유구분=[%s][%s],  취약투자자여부=[%s][%s], 최종고객성향등급명=[%s],  만기일=[%s]",
						m_sInvsCnvs, m_sInvsCnvs == "1" ? "투자권유구분제공" : "투자권유구분미제공",
						m_sWeakInv, m_sWeakInv == "1" ? "취약투자자여부동의" : m_sWeakInv == "9" ? "동의안함" : "해당없음",
						m_sCusGrdNm, m_sDueDt);
					Output_DebugString(m_slog);
				}
				else
				{
					m_sbAgn.Format("%.1s", osacmq329->szAgntYN);				//대리인여부  Y 대리인보유   N 대리인미보유
					m_sInvst.Format("%.1s", osacmq329->szInvstTp);					//투자자구분   0 전문금융소비자  1 일반금융소비자  9 등록안됨
					m_sInfogubn.Format("%.1s", osacmq329->szInfoPrvdTp);    //정보제공구분  0 미제공 1 제공
					m_sInvsCnvs.Format("%.1s", osacmq329->szInvstCnsvTp);    //투자권유구분  0 미제공 1 제공
					m_sWeakInv.Format("%.1s", osacmq329->szWeakInvastrYn);    //취약투자자여부 1 동의 9 동의안함 0 해당없음
					m_sCusGrdNm.Format("%.100s", osacmq329->szLastInclInGrdNm);    //최종고객성향등급명  ex 안정형 안정추구형 위험중립형 적극투자형 공격투자형
					m_sCusGrdNm.TrimRight();
					m_sDueDt.Format("%.8s", osacmq329->szDueDt);					 //만기일 

				/*	m_slog.Format("[cx_account] @@@ dll_oubx 대리인여부=[%s]  투자자구분=[%s] 정보제공구분=[%s]", m_sbAgn, m_sInvst, m_sInfogubn);
					Output_DebugString(m_slog);
					m_slog.Format("[cx_account] @@@ dll_oubx 투자권유구분=[%s]  취약투자자여부=[%s] 최종고객성향등급명=[%s] 만기일=[%s]",
						m_sInvsCnvs, m_sWeakInv, m_sCusGrdNm, m_sDueDt);
					Output_DebugString(m_slog);*/
				}
				
				//WriteLog(Variant(homeCC), m_slog);
				//AfxMessageBox(sRet);
				SACAQ0239Ret_Check();
				m_bSearching = FALSE;
				return 0;
			}
				
			if (HIWORD(wParam) == L_acif_out)	//경우: 계좌정보 key 값을 가져오지 못해서 불가피하게 데이터 사이즈로 체크
			{
				CString strAcc;
				const	struct _acif_out* acif_out = (_acif_out*)lParam;

				m_acif2 = "";
				if (m_pEdit)
					strAcc = m_pEdit->GetInputData();
				/*
				if (strAcc.GetLength()==11)
				{
					strAcc.Insert(3,"-");
					strAcc.Insert(6,"-");
				}
				*/
				m_acif1 = CString(acif_out->lgnm, sizeof(acif_out->lgnm)); m_acif1.Trim();
				m_acif2 = CString(acif_out->prnm, sizeof(acif_out->prnm)); m_acif2.Trim();
				m_acif2.Remove(NULL); m_acif2.Remove(NULL);	
				m_acif2.Remove(' ');	//전화번호 정보이기때문에 공백 제거해도 상관없음

				const	int	sLen = m_acif2.GetLength();

				if (sLen == 0)
				{
					m_acif1 = "["+m_acif1+"]";
				}
				else
				{
					if (sLen == 10)
					{
						m_acif2.Insert(3, '-'); m_acif2.Insert(7, '-');
					}else if (sLen == 11)
					{
						m_acif2.Insert(3, '-'); m_acif2.Insert(8, '-');
					}
					if (!m_acif2.IsEmpty())
					m_acif1 = ("[☎ "+m_acif2+"] ["+m_acif1+"]");
					else
						m_acif1 = ("["+m_acif1+"]");
				}
				//system.push
				CString str_conv = strAcc;
				str_conv.TrimRight();
				if (str_conv.GetLength()==11)
				{
					str_conv.Insert(3, "-");
					str_conv.Insert(6, "-");
				}
				MessageBox("계좌: "+str_conv+"\n\n"+m_acif1,"IBK투자증권");
				PushAcntInfo(strAcc,m_acif1);

			}
			//2013.03.12 KSJ lParam의 길이가 L_gmod 이거나 strNameData의 길이가 60의 배수일때 계좌명 조회로 인식한다.
			else //if (strlen((char *)lParam) == 11 || strlParam.GetLength() == L_gmod || nLength%60 == 0) //빠지는 경우가 생겨서 주석처리함.
			{
				const	struct _gmod* gmod = (_gmod*)lParam;
					
				if (gmod->okgb == _T('Y'))
					ParseGroupList((char *)lParam);
				else if (gmod->okgb != _T('N'))
				{
					if (strlen((char *)lParam) == 11)
					{
						if (m_bInit)
						{
							CString acnt = (char *)lParam;
							if (IsValidAcc(acnt) || m_strDept == m_sAccnDept)
							{
								if (IsValidAcc((char *)lParam)) 
								{
									m_pEdit->SetInputData((char *)lParam);
									m_strAccName = GetAcntName((LPCSTR)lParam);
									if (m_pParent)
									{
										m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange/*Change*/)), 
														(LPARAM)m_Param.name.GetString());
									}
								}
							}
						}
						else
							m_strTempDomino = CString((char*)lParam);
					}
					else
					{
						//2013.04.19 KSJ 서비스 이름을 보고 판단함.
						const	struct _ledgerH * ledger = (_ledgerH*)lParam;
						CString strSvcd(STR(ledger->svcd));

						strSvcd.Trim();

						if (!strSvcd.Compare("SACMQ329"))
						{
							if (m_nAgentType != AGENT_NOITEM)
								ParseSACMQ329((char *)lParam);
						}
						else if (!strSvcd.Compare("SACMQ748"))
						{
							ParseSACMQ748((char *)lParam);
						}
						else
							ParseAccntName((char *)lParam);
					}
				}
			}
		}
		break;
	case DLL_ALERT:
		break;
	case DLL_TRIGGER:
		{
			CString strTrigger((char*)lParam);
		}
		break;
	case DLL_DOMINO:
		{
			CString strDomino((char*)lParam);

		}
		break;
	case DLL_SETPAL:
		break;
	case DLL_GUIDE:
		break;
	case DLL_SETFONT:
		{
			m_Param.point = HIWORD(wParam);
			CString tmp = CString((char*)lParam);
			if (!tmp.IsEmpty())
				m_Param.fonts = tmp;
			Invalidate();
		}
		break;
	default:break;
	}
	return FALSE;
}

// 2012.02.22 KSJ 에디트컨트롤 한글로 바꾸기
#include "imm.h"
#pragma comment(lib, "imm32")
// KSJ

//2012.02.15 KSJ
/////////////////////////////////////////////////////////////////////////////
// CDlg message handlers
long CAccountCtrl::OnEditXMsg(WPARAM wParam, LPARAM lParam)					// 검색시 에디터 박스에 키가 입력 되었을때
{
	CString	string;

	switch ((int) wParam)
	{
	case wpBLANK:
		searchAction(1);	
		break;
	case wpENTER:		
		m_pEdit->GetWindowText(string);

		if (!string.IsEmpty())
		{
			searchAction(1);

			if (m_arrSearchAcc.GetSize() == 0)
			{
				MessageBox("입력하신 관리고객이 없습니다. 고객명 확인 후\n다시 입력해주시기 바랍니다.", "관리고객명 확인", MB_OK);
				m_pEdit->SetWindowText("");
				m_pEdit->SetFocus();

				// 2012.02.22 KSJ 에디트컨트롤 한글로 바꾸기
				HIMC himc=ImmGetContext(this->GetSafeHwnd());
				ImmSetConversionStatus(himc, IME_CMODE_NATIVE,IME_SMODE_CONVERSATION);
				ImmReleaseContext(this->GetSafeHwnd(),himc);
				// KSJ
			}
			else
			{
				OnComboSelChange();
				m_pCombo->ShowDropDown(FALSE);
			}
			m_arrSearchAcc.Copy(m_arrAllAcc);
		}
		break;
// 	case wpSEARCH1:
// 		searchAction(0);	// number search by kwon
// 		break;
	case wpSEARCH2:
		searchAction(1);	// char search by kwon`
		break;
	case wpSETACCOUNT:
		m_pEdit->SetInputData((char*)lParam);
		break;
	}
	
	return 0;
}

void CAccountCtrl::searchAction(int column)
{
	CString	string; 
	m_pEdit->GetWindowText(string);
	if (column == 0)	// number search
	{
	}
	else			// name search
	{
	//	m_bAddAll = SearchData(string, m_bAddAll);  
		m_bAddAll = Search_MidData(string, m_bAddAll);
				
		if (string.IsEmpty() && m_OldString.IsEmpty())	//2012.02.28 KSJ 빈값일때는 한번만 조회하도록
			return;

		m_OldString = string;

		m_pCombo->UpdateData(TRUE);
		if (m_bAddAll || m_arrSearchAcc.GetSize() != 0)	//2012.02.27 KSJ 검색된 것이 없으면 리스트를 표시하지 않음.
		{
			ShowAccountList(FALSE, TRUE);
			m_pCombo->SetCurSel(0);
		}
	}
}

#define	HANGULMSK	0x80
#define	ChosungS	0xA4A1	// ㄱ
#define	ChosungE	0xA4BE	// ㅎ

bool CAccountCtrl::FindWordFromACCList(CString strSearch, CAccountArr* pArr)
{
	CString	strName;
	std::shared_ptr<CAccount> pAcc = nullptr;

	m_arrSearchAcc.RemoveAll();
	if (pArr == nullptr)
		return false;

	for (int ii = 0; ii < pArr->GetSize(); ii++)
	{
		pAcc = pArr->GetAt(ii);
		strName = pAcc->m_strAccntName;
		if (strName.Find(strSearch) >= 0)
		{
			m_arrSearchAcc.Add(pAcc);
		}
	}
	
	if (m_arrSearchAcc.GetSize() > 0)
		return true;

	return false;
}

bool CAccountCtrl::Search_MidData(CString sName, bool bAddAll)
{	
	CString	strName;
	bool	breturn = true;
	CAccountArr* pArr = nullptr;
	std::shared_ptr<CAccount> pAcc = nullptr;

	if (m_bGroup)
	{
		if (m_nSelectGroup == 0)
			pArr = &m_arrAllAcc;
		else
			pArr = &m_arrGroupAcc;
	}
	else
	//	pArr = &m_arrAllAcc;
		pArr = &m_arrHistoryAcc;     //여기 확인 필요 

	m_arrSearchAcc.RemoveAll();
	if (sName.IsEmpty())
	{
		m_arrSearchAcc.Copy(*pArr);
		return true;
	}

	char	cName = 0;
	const	int	sLen = sName.GetLength();

	if (sLen == 1 || sLen > 2)  //영어 한글자 혹은 한글(초성만아님) 혹은 영어 두글자 이상
	{	// 일반검색적용(길이만큼 맞는것만 적용)  //맨처음 입력받으면 이거다
		FindWordFromACCList(sName, pArr);
	}
	else if (sLen == 2)
	{
		CAccountArr	arSearch;
		const	int	ii = 0;
		WORD	wHangul = 0, wHangul2 = 0, wStart = 0, wEnd = 0;

		arSearch.RemoveAll();
		m_arrSearchAcc.RemoveAll();
		cName = sName.GetAt(0);
		if (cName & HANGULMSK)	// 한글일땐 2Byte
		{
			wHangul = MakeHangul(sName.GetAt(ii), sName.GetAt(ii+1));
			if (IsChosung(wHangul, wStart, wEnd))
			{	// 초성 비교
				for (int jj = 0; jj < pArr->GetSize(); jj++)
				{
					pAcc = pArr->GetAt(jj);
					strName = pAcc->m_strAccntName;
					strName.TrimLeft();
					if (strName.GetLength() < 2)	
						continue;
					
					wHangul2 = MakeHangul(strName.GetAt(ii), strName.GetAt(ii+1));
					if (wStart <= wHangul2 && wHangul2 <= wEnd)	
					{
						m_arrSearchAcc.Add(pAcc);
						breturn = true;
					}
					else
					{
						/*
						if (HANGULMSK & strName.GetAt(ii + 1))
						{
							CString str;
							str = strName.Mid(ii + 2, ii + 3);
							
							const char* pdat = GetInitialSound(LPSTR((LPCTSTR)str));
							
							if ((LPSTR)(LPCTSTR)sName == pdat)
								arSearch.Add(pAcc);
						}
						*/
						
					} //else
				
				}  //for
			}
			else  //초성이 아니고 완성된 한글
			{
				FindWordFromACCList(sName, pArr);
				breturn = true;
			}
		}
		else
			FindWordFromACCList(sName, pArr);
	}

	return breturn;
}

bool CAccountCtrl::SearchData(CString sName, bool bAddAll)
{
	CString	strName;
	int	position = 0;
	bool	breturn = false;
	std::shared_ptr<CAccount> pAcc = nullptr;

	m_arrSearchAcc.RemoveAll();
	if (sName.IsEmpty())
	{
		m_arrSearchAcc.Copy(m_arrAllAcc);
		return true;
	}

	char	cName = 0;
	const	int	sLen = sName.GetLength();

	if (sLen == 1)
	{	// 일반검색적용(길이만큼 맞는것만 적용)
		m_arrSearchAcc.RemoveAll();
		for (int ii = 0; ii < m_arrAllAcc.GetSize(); ii++)
		{
			pAcc = m_arrAllAcc.GetAt(ii);
			strName = pAcc->m_strAccntName;
			if (strncmp(sName, strName, sLen) != 0)
				continue;
					
			m_arrSearchAcc.Add(pAcc);
			position += 1;
		}
	}
	else
	{	// 한글 검색
		CAccountArr	arSearch;
		int	ii = 0;
		WORD	wHangul = 0, wHangul2 = 0, wStart = 0, wEnd = 0;

		arSearch.RemoveAll();
		cName = sName.GetAt(ii);
		if (cName & HANGULMSK)	// 한글일땐 2Byte
		{
			wHangul = MakeHangul(sName.GetAt(ii), sName.GetAt(ii+1));
			if (IsChosung(wHangul, wStart, wEnd))
			{	// 초성 비교
				for (int jj = 0; jj < m_arrAllAcc.GetSize(); jj++)
				{
					pAcc = m_arrAllAcc.GetAt(jj);
					strName = pAcc->m_strAccntName;
					strName.TrimLeft();
					if (ii < strName.GetLength())	//2012.02.27 KSJ 실제 계좌명보다 작아야함.
					{
						if (strName.GetLength() < 2)
							continue;
						wHangul2 = MakeHangul(strName.GetAt(ii), strName.GetAt(ii+1));
						if (wStart <= wHangul2 && wHangul2 <= wEnd)
							arSearch.Add(pAcc);
					}
				}
			}
			else
			{	// 한글 비교
				for (int jj = 0; jj < m_arrAllAcc.GetSize(); jj++)
				{
					pAcc = m_arrAllAcc.GetAt(jj);
					strName = pAcc->m_strAccntName;
					strName.TrimLeft();

					if (ii < strName.GetLength())	//2012.02.27 KSJ 실제 계좌명보다 작아야함.
					{
						if (strName.GetLength() < 2)
							continue;
						wHangul2 = MakeHangul(strName.GetAt(ii), strName.GetAt(ii+1));
						if (wHangul == wHangul2)
							arSearch.Add(pAcc);
					}
				}
			}
			ii += 2;

			breturn = true;
		}
		else			// 그외는 1Byte
		{
			for (int jj = 0; jj < m_arrAllAcc.GetSize(); jj++)
			{
				pAcc = m_arrAllAcc.GetAt(jj);
				strName = pAcc->m_strAccntName;
				if (strName.GetLength() < 1)
					continue;
				if (cName == strName.GetAt(ii))
					arSearch.Add(pAcc);
			}
			ii += 1;
		}

		for (; ii < sLen && arSearch.GetSize();)
		{
			cName = sName.GetAt(ii);
			if (cName & HANGULMSK)	// 한글일땐 2Byte
			{
				wHangul = MakeHangul(sName.GetAt(ii), sName.GetAt(ii+1));
				if (IsChosung(wHangul, wStart, wEnd))
				{	// 초성 비교
					for (int jj = arSearch.GetUpperBound(); jj >= 0; jj--)
					{
						pAcc = arSearch.GetAt(jj);
						strName = pAcc->m_strAccntName;
						strName.TrimLeft();
						if (ii < strName.GetLength())	//2012.02.27 KSJ 실제 계좌명보다 작아야함.
						{
							if (strName.GetLength() < ii+2)
								arSearch.RemoveAt(jj);;
							wHangul2 = MakeHangul(strName.GetAt(ii), strName.GetAt(ii+1));
							if (wStart > wHangul2 || wHangul2 > wEnd)
								arSearch.RemoveAt(jj);
						}
					}
				}
				else
				{	// 한글 비교

					for (int jj = arSearch.GetUpperBound(); jj >= 0; jj--)
					{
						pAcc = arSearch.GetAt(jj);
						strName = pAcc->m_strAccntName;
						strName.TrimLeft();
						if (strName.GetLength() < ii+2)	
							arSearch.RemoveAt(jj);

						if (ii < strName.GetLength())	//2012.02.27 KSJ 실제 계좌명보다 작아야함.
						{
							wHangul2 = MakeHangul(strName.GetAt(ii), strName.GetAt(ii+1));
							if (wHangul != wHangul2)
								arSearch.RemoveAt(jj);
						}
					}
				}
				ii += 2;

				breturn = true;
			}
			else			// 그외는 1Byte
			{
				for (int jj = arSearch.GetUpperBound(); jj >= 0; jj--)
				{
					pAcc = arSearch.GetAt(jj);
					strName = pAcc->m_strAccntName;

					if (strName.GetLength() < ii+1)
						arSearch.RemoveAt(jj);
					if (cName != strName.GetAt(ii))
						arSearch.RemoveAt(jj);
				}
				ii += 1;
			}
		}

		m_arrSearchAcc.RemoveAll();

		for (ii = 0; ii < arSearch.GetSize(); ii++)
		{
			pAcc = arSearch.GetAt(ii);

			m_arrSearchAcc.Add(pAcc);
			position += 1;
		}
	}

	return breturn;
}

WORD CAccountCtrl::MakeHangul(char cChar1, char cChar2)
{	// 2Byte문자로부터 한글을 맹글어주라~~
	return ((cChar1<<8) | (0x00FF & cChar2));
}

bool CAccountCtrl::IsChosung(WORD wHangul, WORD &wStart, WORD &wEnd)
{	// 초성만 가진 한글인지~~
	if (ChosungS > wHangul || wHangul > ChosungE)
		return false;
	
	const	WORD	pChosung[19] = 
	{
		0xA4A1, 0xA4A2, 0xA4A4, 0xA4A7, 0xA4A8, 0xA4A9, 0xA4B1,	// ㄱ,ㄲ,ㄴ,ㄷ,ㄸ,ㄹ,ㅁ
			0xA4B2, 0xA4B3, 0xA4B5, 0xA4B6, 0xA4B7, 0xA4B8, 0xA4B9,	// ㅂ,ㅃ,ㅅ,ㅆ,ㅇ,ㅈ,ㅉ
			0xA4BA, 0xA4BB, 0xA4BC, 0xA4BD, 0xA4BE			// ㅊ,ㅋ,ㅌ,ㅍ,ㅎ
	};
	
	const	WORD	pHangulStart[19] = 
	{
		0xB0A1, 0xB1EE, 0xB3AA, 0xB4D9, 0xB5FB, 0xB6F3, 0xB8B6,	// 가,까,나,다,따,라,마
			0xB9D9, 0xBAFC, 0xBBE7, 0xBDCE, 0xBEC6, 0xC0DA, 0xC2A5,	// 바,빠,사,싸,아,자,짜
			0xC2F7, 0xC4AB, 0xC5B8, 0xC6C4, 0xC7CF			// 차,카,타,파,하
	};
	const	WORD	pHangulEnd[19] = 
	{
		0xB1ED, 0xB3A9, 0xB4D8, 0xB5FA, 0xB6F2, 0xB8B5,		
			0xB9D8, 0xBAFB, 0xBBE6, 0xBDCD, 0xBEC5, 0xC0D9, 0xC2A4,	
			0xC2F6, 0xC4AA, 0xC5B7, 0xC6C3, 0xC7CE, 0xC8FE			
	};
	int	ii = 0;
	
	for ( ; ii < 19; ii++)
	{
		if (wHangul == pChosung[ii])
		{
			wStart = pHangulStart[ii];
			wEnd = pHangulEnd[ii];
			return true;
		}			
	}
	
	wStart = pHangulStart[ii] - 1;
	wEnd = pHangulEnd[ii] + 1;
	
	return true;
}

//KSJ

void CAccountCtrl::ParseGroupList(char* pBuf)
{
	const	struct _gmod* gmod = (_gmod*)pBuf;
	
	if (gmod->okgb != _T('Y'))
		return;
	
	bool	bFind_GID_CLIENT = false;
	int	nGrec = atoi(CString(gmod->grec, sizeof(gmod->grec)));

	if (nGrec == 0)
		return;

	CString strTemp, strAccountNum;
	int	nAccCount = 0;
	struct	_group*	group = nullptr;
	struct	_accn* accn   = nullptr;
	std::shared_ptr<CAccount> pAcc = nullptr;
	std::shared_ptr<CAccount> acSave = nullptr;
	std::shared_ptr<CGroup> pGp = nullptr;

	pBuf += L_gmod;
	for (int ii = 0; ii < nGrec; ii++)
	{
		group = (_group *)pBuf;
		
		pGp = std::make_shared<CGroup>();
		pGp->m_strGroupID = CString(group->gpid, sizeof(group->gpid));
		pGp->m_strGrSortNum = CString(group->seqn, sizeof(group->seqn));
		pGp->m_strGrName = CString(group->gnam, sizeof(group->gnam));
		pGp->m_strAccntCnt = CString(group->nrec, sizeof(group->nrec));
		nAccCount = atoi(pGp->m_strAccntCnt);
		if (pGp->m_strGroupID == GID_CLIENT)
		{
			bFind_GID_CLIENT = true;
		}	
		pBuf += L_group;

		for (int jj = 0; jj < nAccCount; jj++)
		{
			accn = (_accn *)pBuf;

			strTemp = CString(accn->accn, sizeof(accn->accn));
			strAccountNum = strTemp.Left(3) + "00" + strTemp.Right(6);
			pAcc = FindAccountInfo(strAccountNum, TRUE);
			if (pAcc && m_staff)
			{
				if ((m_bWrap && !pAcc->m_bWrap) || (!m_bWrap && pAcc->m_bWrap))
				{
					pBuf += L_accn;
					continue;
				}
			}

			acSave = std::make_shared<CAccount>();

			acSave->m_strSortNum = CString(accn->seqn, sizeof(accn->seqn));
			acSave->m_strAccntNum = CString(accn->accn, sizeof(accn->accn));
			acSave->m_strAccntName = CString(accn->acnm, sizeof(accn->acnm));
			strTemp = CString(accn->rate, sizeof(accn->rate));
			strTemp.TrimRight();
			acSave->m_strAllocRate.Format("%.f", atof(strTemp));
			acSave->m_strAccntNick = CString(accn->alis, sizeof(accn->alis));

			if (!m_bOrderEnable)
				acSave->m_strPassword = CString(accn->pass, sizeof(accn->pass));
			else
				acSave->m_strPassword = Variant(passCC, acSave->m_strAccntNum);

			acSave->m_strPriAcc = accn->prea;
			acSave->m_strAllocMulti =  CString(accn->multi, sizeof(accn->multi));
			acSave->TrimData();

			pGp->m_arrAccount.Add(acSave);
			pBuf += L_accn;
		}

		pGp->TrimData();
		m_arrGroupList.Add(pGp);
	}
	if (bFind_GID_CLIENT && (!m_staff))
	{		
		_SetGroup(GID_CLIENT);
	}	
}

BOOL CAccountCtrl::SendTR(CString strName, BYTE type, char* pBytes, int nBytes, int key)
{
	struct _userTH	udat {};
	int	index = 0;
	std::unique_ptr<TCHAR[]> buff = std::make_unique<TCHAR[]>(L_userTH + nBytes + 128);

	CopyMemory(udat.trc, strName, strName.GetLength());
	udat.stat = type | US_KEY;
	if (m_bDLL)
	{
		udat.key = key;			//TRKEY_GROUP;
	}
	else
	{
		udat.key = m_Param.key;

		buff[index++] = key;		//TRKEY_GROUP;
		CopyMemory(&buff[index], (char*)m_Param.name.GetString(), m_Param.name.GetLength());
		index += m_Param.name.GetLength();
		buff[index++] = '\t';
	}

	CopyMemory(&buff[index], &udat, L_userTH);
	index += L_userTH;
	CopyMemory(&buff[index], pBytes, nBytes);

	index = m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, nBytes), (LPARAM)buff.get());
	buff.reset();
	return index;
}

void CAccountCtrl::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TT_INIT)
	{
		KillTimer(TT_INIT);
		m_bInit = TRUE;
		
		//2013.01.16 KSJ TT_INIT 타이머 실행 횟수를 최대 2번으로 제한한다.
		//KillTimer가 동작되지 않고 넘어가서 100번씩 될때가 있다......
		if (m_nTimerCnt++ > 2)
			return;
		//KSJ
		
		if (m_bOrderEnable)
			QueryGroupList();
		else 
		{
			if (m_Param.name.Find(_T("AN3A")) >= 0)
			{
				m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange/*Change*/)),     //해외
							(LPARAM)m_Param.name.GetString());
			}
		}

		CString Path, strHistory;
		char	readb[10 * 1024]{};
		CString strKey = m_Param.name;
		std::shared_ptr<CAccount> pAcc = nullptr;

		strKey.Trim();
		Path.Format("%s\\user\\%s\\%s.ini", m_strRoot, Variant(nameCC), Variant(nameCC));

		//if (strKey.Find("AN2") == -1) strKey = "AN1A";

		GetPrivateProfileString(_T("AccountHistory"), strKey, _T(""), readb, sizeof(readb), Path);

		//enc
		CString val = ReadAccountHistory(Path, strKey);
		strncpy_s(readb, sizeof(readb), val, _TRUNCATE);
		int readl = (int)strlen(readb);
		//enc


		strHistory = readb;
		strHistory.Trim();
		if (!strHistory.IsEmpty())
		{
			CString strLast;
			CString strLastAcc;
			CString strLastName;

			while (!strHistory.IsEmpty())
			{
				strLast = Parser(strHistory, _T("\t"));
				strLastAcc = Parser(strLast, _T("|"));
				strLastName = Parser(strLast, _T("|"));
				
				AppendAccHistory(strLastAcc, _T(""), strLastName);
			}
		}

		//2013.05.28 KSJ 계좌명정렬시
		if (m_nSort ==1)
		{
			LoadHistoryAccName();
		}
		else
		{
			LoadHistoryAcc();
		}
	
		CString strAccount(_T(""));
		if (m_pEdit && m_strDept != m_sAccnDept)
		{
			if (m_strTempDomino.IsEmpty())	// 도미노 데이터가 있는지 확인
			{
				for (int ii = 0; pAcc == nullptr && ii < 200; ii++)
				{
					if (m_arrHistoryAcc.GetSize() > ii)
						pAcc = m_arrHistoryAcc.GetAt(ii);
					else if (m_bOrderEnable && m_arrAllAcc.GetSize() > ii)
						pAcc = m_arrAllAcc.GetAt(ii);

					// 랩계좌타입이면 종합계좌가 아닐경우 Skip
					if (pAcc)
					{
						if (m_bWrap && pAcc->m_strAccntNum.Mid(3,2) != "00") 
						{
							pAcc = nullptr;
							continue;
						}
						break;
					}
				}
				
				if (pAcc)
					strAccount = pAcc->m_strAccntNum;
			}
			else
			{
				strAccount = m_strTempDomino;
			}
			
			m_pEdit->SetInputData(strAccount);
			AccountInputComplete(strAccount);
			QueryAccntCheck(m_pEdit->GetInputData());  //test checkacc , ontimer
		}

		//2013.04.23 KSJ 계좌리스트가 전부 들어가면 이벤트를 보낸다.
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnClick/*Click*/)), 
				(LPARAM)m_Param.name.GetString());
		//2013.04.23 KSJ 


		//계좌 History 관련 화살표 Bmp 적용에 따른 테두리 색상 변경(2009.06.22:kbkim)
		m_clrBorderColor = RGB(140, 140, 140);
		m_clrEditBackColor = GetIndexColor(COLOR_EDITBACK);

		CDC*	pDC = m_pEdit->GetDC();
		pDC->SetTextColor(m_clrForeColor);
		ReleaseDC(pDC);
		Invalidate();
	}
	else if (nIDEvent == TT_QUERYACCNM)
	{
		KillTimer(TT_QUERYACCNM);

		if (m_strAccName.IsEmpty())
		{
			Variant(guideCC, "계좌명 조회 실패.  관리자에게 문의바랍니다.");
			m_pEdit->SetInputData(_T(""));
			//QueryAccntInfo(_T(""));
			m_pEdit->SetSel(0, -1);
			m_pEdit->SetFocus();
		}
		else if (m_strAccName.Compare("NONAME!"))
		{
			m_strAccName = "";
			m_pEdit->SetInputData(_T(""));
			//QueryAccntInfo(_T(""));
			m_pEdit->SetSel(0, -1);
			m_pEdit->SetFocus();
		}
		else if (m_bFireEvent)
		{
			if (m_pEdit)
			{
				CString strAccount = m_pEdit->GetInputData();

				if (!strAccount.IsEmpty() && !m_strAccName.IsEmpty())
				{
					PushAccHistory(m_pEdit->GetInputData(), _T(""), m_strAccName);
					m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange/*Change*/)), 
								(LPARAM)m_Param.name.GetString());
					SaveHistory();
				}
			}
		}
	}
	else if (nIDEvent == TT_813REREAD)
	{
		KillTimer(nIDEvent);

		if (!ReadAcc813())
		{
			SetTimer(TT_813REREAD, 3000, NULL);
		}
		else
		{
			SetTimer(TT_INIT, 10, NULL);
		}
	}
	else if (nIDEvent == TT_029REREAD)
	{
		KillTimer(TT_029REREAD);
		m_bSearching = FALSE;
	}

	CWnd::OnTimer(nIDEvent);
}

void CAccountCtrl::SetParam(_param *pParam)
{
	m_Param.key = pParam->key;
	m_Param.name = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	m_clrForeColor = m_Param.tRGB = pParam->tRGB;
	m_clrBackColor = m_Param.pRGB = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());
	
	m_sHideTradeAgent.Empty();

	m_sOption.Empty();

	int	idx = 0, pos = 0;
	CString	keys, text, tmps, strtemp;
	CString sOption{};
	sOption = m_Param.options;

	tmps = _T("/ ");
	keys = _T("wt");

	for (int ii = 0; ii < keys.GetLength(); ii++)
	{
		tmps.SetAt(1, keys[ii]);
		idx = sOption.Find(tmps);
		if (idx < 0)
			continue;

		idx += 2;
		pos = sOption.Find('/', idx);

		text = (pos < 0) ? sOption.Mid(idx) : sOption.Mid(idx, pos - idx);
		text.TrimLeft();

		switch (keys[ii])
		{
		case 'w':
		{
			if (OptionParser(sOption, "/w") == 3)
				m_sHideTradeAgent = "1";
		}
		break;
		case 't':
		{
#ifndef DF_TEST_MODE
			if (!(long)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0L))
#endif
			{
				m_sOption = OptionParser(sOption, "/t");
				
				//CString sArr[10]{ "", };
				//CString stmp;
				//int idx = 0;
				//while (AfxExtractSubString(stmp, m_sOption, idx, '\n'))
				//{
				//	sArr[idx] = stmp;
				//	++idx; // 성공했을 때만 증가
				//}

				if (m_sOption.Find("투자성향정보팝업") >= 0)
					m_bPOPTrdTed = TRUE;
			}
		}
		break;
		}
	}

	m_bWrap = (m_Param.name.Left(2)=="AM") ? TRUE : FALSE;
}

CString CAccountCtrl::GetLedger(CString strTR, char chType, CString strSvcn, CString strLastKey /* = _T("") */)
{
	struct _ledgerH ledger;

	FillMemory(&ledger, L_ledgerH, ' ');
	switch (chType)
	{
	case 'I':
		chType = '2';	break;
	case 'U':
		chType = '3';	break;
	case 'D':
		chType = '4';	break;
	case 'Q':
	default:
		chType = '1';
	}

	m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);

	if (!strLastKey.IsEmpty())
		CopyMemory(&ledger.next, strLastKey, strLastKey.GetLength());

	return CString((char*)&ledger, L_ledgerH);
}

//----------2013.10.29 MK SACMQ748(고객추가정보 조회)----------
void CAccountCtrl::QuerySACMQ748()
{
	CString strData;
	char	datb[L_userTH + L_ledgerH + L_ISACMQ748+128]{};
	struct	_userTH	udat {};
	struct	_ledgerH ledger {};
	ISACMQ748 isacamq748{};
	int	index = 0;

	FillMemory(&ledger, L_ledgerH, ' ');
	m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	
	CopyMemory(ledger.svcd, _T("SACMQ748"), sizeof(ledger.svcd));
	CopyMemory(ledger.usid, (LPCTSTR)m_strUser, m_strUser.GetLength());
	CopyMemory(ledger.brno, "000", ACC_DEPT);
	CopyMemory(ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));
	
	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '3';
	ledger.odrf[0] = '2';
	
	strData = m_pEdit->GetInputData();
	FillMemory(&isacamq748, L_ISACMQ748, ' ');
	CopyMemory(isacamq748.szIn, _T("00001"), sizeof(isacamq748.szIn));
	CopyMemory(isacamq748.szAcntNo, (LPCTSTR)strData, 11);
	
	CopyMemory(udat.trc, "pibopbxq", sizeof(udat.trc));
	udat.stat = US_ENC | US_KEY;
	
	if (m_bDLL)
		udat.key = (BYTE)TRKEY_SACMQ748;
	else
	{
		udat.key = m_Param.key;

		datb[index++] = (BYTE)TRKEY_SACMQ748;
		CopyMemory(&datb[index], (char *)m_Param.name.GetString(), m_Param.name.GetLength());
		index += m_Param.name.GetLength();
		datb[index++] = '\t';
	}

	CopyMemory(&datb[index], (char*)&udat, L_userTH);	index += L_userTH;
	CopyMemory(&datb[index], (char*)&ledger, L_ledgerH);	index += L_ledgerH;
	CopyMemory(&datb[index], (char*)&isacamq748, L_ISACMQ748);
	
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, L_ISACMQ748 + L_ledgerH), (LPARAM)datb);
}

void CAccountCtrl::ParseSACMQ748(char* pBuf)
{
	CString strData, strdata1, str_conv; 
	CString LastInclnGrdNm, MoveTelIdfyNo, MoveTelExno, MoveTelSeqno;

	pBuf = pBuf + L_ledgerH + L_ISACMQ748;
	strData = pBuf;

	const	OSACMQ748* osacmq748 = (OSACMQ748*)pBuf;
	LastInclnGrdNm = STR(osacmq748->LastInclnGrdNm);
	MoveTelIdfyNo = STR(osacmq748->MoveTelIdfyNo);
	MoveTelExno = STR(osacmq748->MoveTelExno);
	MoveTelSeqno = STR(osacmq748->MoveTelSeqno);
	if (!MoveTelIdfyNo.IsEmpty())
	{
		strdata1 = "[☎"+MoveTelIdfyNo+"-"+MoveTelExno+"-"+MoveTelSeqno+"]";
	}
	
	str_conv = m_pEdit->GetInputData();
	str_conv.TrimRight();
	if (str_conv.GetLength()==11)
	{
		str_conv.Insert(3, "-");
		str_conv.Insert(6, "-");
	}
	LastInclnGrdNm.TrimRight();
	MessageBox("계좌: "+str_conv+"\n\n"+strdata1+"["+LastInclnGrdNm+"]","IBK투자증권");
}

//----------2013.10.29 MK SACMQ748(고객추가정보 조회)----------


//2013.03.12 KSJ 자금대체대리인 조회
void CAccountCtrl::QuerySACMQ329()
{
	struct	_userTH udat {};
	struct	_ledgerH ledger {};
	ISACMQ329 isacamq329{};
	char	datb[L_userTH + L_ledgerH + L_ISACMQ329 + 128]{};
	int	index = 0;

	FillMemory(&ledger, L_ledgerH, ' ');
	m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	
	CopyMemory(ledger.svcd, _T("SACMQ329"), sizeof(ledger.svcd));
	CopyMemory(ledger.usid, (LPCTSTR)m_strUser, m_strUser.GetLength());
	CopyMemory(ledger.brno, "000", ACC_DEPT);
	CopyMemory(ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));
	
	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '3';
	ledger.odrf[0] = '1';
	
	FillMemory(&isacamq329, L_ISACMQ329, ' ');
	CopyMemory(isacamq329.szIn, _T("00001"), sizeof(isacamq329.szIn));
	CopyMemory(isacamq329.szUserID, (LPCTSTR)m_strUser, m_strUser.GetLength());
	
	CopyMemory(udat.trc, "pibopbxq", sizeof(udat.trc));
	udat.stat = US_ENC | US_KEY;
	
	if (m_bDLL)
		udat.key = (BYTE)TRKEY_SACMQ329;
	else
	{
		udat.key = m_Param.key;

		datb[index++] = (BYTE)TRKEY_SACMQ329;
		CopyMemory(&datb[index], (char *)m_Param.name.GetString(), m_Param.name.GetLength());
		index += m_Param.name.GetLength();
		datb[index++] = '\t';
	}
	
	CopyMemory(&datb[index], (char*)&udat, L_userTH);	index += L_userTH;
	CopyMemory(&datb[index], (char*)&ledger, L_ledgerH);	index += L_ledgerH;
	CopyMemory(&datb[index], (char*)&isacamq329, L_ISACMQ329);

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, L_ISACMQ329 + L_ledgerH), (LPARAM)datb);
}

void CAccountCtrl::ParseSACMQ329(char* pBuf)
{
	//m_nAgentType가 NoItems이 아니고 범위안에 세팅되어 있을때만 넘어가도록
	if (m_nAgentType < 0 && m_nAgentType > AGENT_ITEM8)
		return;

	CString strAccount, strAccName, strItems;

	//20200116 에러코드 확인
	strItems = CString(((struct _ledgerH* )pBuf)->ecod, 4);
	strItems.Trim();
	
	if (strItems.Left(1) != "0")
		return;

	pBuf = pBuf + L_ledgerH + L_ISACMQ329;
	const	OSACMQ329* osacmq329 = (OSACMQ329*)pBuf;
	for (int ii = 0; ii < _ttoi(STR(osacmq329->szOut)); ii++)
	{
		strAccount = STR(osacmq329->grid[ii].szAccount);
		strAccName = STR(osacmq329->grid[ii].szAccName);
		strItems = STR(osacmq329->grid[ii].szItems);
		
		AddAcc(strAccount, "", strAccName, strItems);
	}
}

//KSJ

void CAccountCtrl::QueryAccntName(CString strAccount)
{
	struct	_userTH	udat {};
	struct	_ledgerH ledger {};
	IACCNM	iaccnm{};
	char	datb[L_userTH + L_ledgerH + L_IAccnm + 128]{};
	int	index = 0;

	FillMemory(&ledger, L_ledgerH, ' ');
	m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	
	CopyMemory(ledger.svcd, _T("SACMQ902"), sizeof(ledger.svcd));
	CopyMemory(ledger.usid, (LPCTSTR)m_strUser, m_strUser.GetLength());
	CopyMemory(ledger.brno, strAccount.Left(ACC_DEPT), ACC_DEPT);
	CopyMemory(ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));
	
	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '3';
	ledger.odrf[0] = '1';
	
	FillMemory(&iaccnm, L_IAccnm, ' ');
	CopyMemory(iaccnm.szIn, _T("00001"), sizeof(iaccnm.szIn));
	CopyMemory(iaccnm.szAccount, (LPCTSTR)strAccount, strAccount.GetLength());

	CopyMemory(udat.trc, "pibopbxq", sizeof(udat.trc));
	udat.stat = US_ENC | US_KEY;

	if (m_bDLL)
		udat.key = (BYTE)TRKEY_ACCNTNAME;
	else
	{
		udat.key = m_Param.key;

		datb[index++] = (BYTE)TRKEY_ACCNTNAME;
		CopyMemory(&datb[index], (char*)m_Param.name.GetString(), m_Param.name.GetLength());
		index += m_Param.name.GetLength();
		datb[index++] = '\t';
	}

	CopyMemory(&datb[index], (char*)&udat, L_userTH);	index += L_userTH;
	CopyMemory(&datb[index], (char*)&ledger, L_ledgerH);	index += L_ledgerH;
	CopyMemory(&datb[index], (char*)&iaccnm, L_IAccnm);

	m_strAccName.Empty();

	static int nCount = 0;
	nCount++;

	CString slog;
	slog.Format("[cx_account]QueryAccntName \r\n");
	Output_DebugString(slog);

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, L_IAccnm + L_ledgerH), (LPARAM)datb);
}

void CAccountCtrl::ParseAccntName(char *pBuf)
{
	CString strData = pBuf;
	CString dept_Num;
	BOOL bSucc = TRUE;

	pBuf = pBuf + L_ledgerH + L_IAccnm;

	const	OACCNM* oaccnm = (OACCNM*)pBuf;
	m_strAccName = CString(oaccnm->szAccntNm, sizeof(oaccnm->szAccntNm));
	m_strAccName.Trim();
	
	if (m_strAccName == "")
	{
		if (m_Param.name.Find(_T("AN3A")) >= 0)
		{
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange/*Change*/)),     //해외
						(LPARAM)m_Param.name.GetString());
		}
		return;
	}
	
	dept_Num = m_strAccName.Mid(0,3);
	QueryAccntCheck(m_pEdit->GetInputData());  //test checkacc  , parseaccntname(char*)

	if (!IsValidAccComplete(CString(oaccnm->szAccount, sizeof(oaccnm->szAccount)), dept_Num, FALSE)&&(m_dept!="812"))
	{
		KillTimer(TT_QUERYACCNM);
		Variant(guideCC, "유효하지 않은 계좌입니다. 다시 입력하십시오.");
		m_pEdit->SetInputData(_T(""));
		//QueryAccntInfo(_T(""));
		m_pEdit->SetSel(0, -1);
		m_pEdit->SetFocus();
		m_strAccName.Empty();
		Invalidate();
		return;
	}
      	

	m_strAccName = m_strAccName.Mid(3);
	if (m_strAccName.IsEmpty())
	{
		m_pEdit->SetInputData(_T(""));
		//QueryAccntInfo(_T(""));
		m_pEdit->SetSel(0, -1);
		bSucc = FALSE;
	}

	if (m_bFireEvent)
	{
		KillTimer(TT_QUERYACCNM);

		if (m_pEdit)
			PushAccHistory(m_pEdit->GetInputData(), _T(""), m_strAccName);
		//CString strPop = PopAccHistory();
		//AfxMessageBox(strPop);
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange/*Change*/)), 
							(LPARAM)m_Param.name.GetString());
	}

	SaveHistory();

}

COLORREF CAccountCtrl::GetIndexColor(int nColor)
{
	if (nColor & 0x02000000)
		return nColor;

	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)nColor);
}

void CAccountCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	Invalidate();
	CWnd::OnSetFocus(pOldWnd);
	
	if (pOldWnd != m_pEdit.get())
		m_pEdit->SetFocus();
}

void CAccountCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CAccountCtrl::PushTab()
{
	SetFocus();
	m_pParent->PostMessage(WM_KEYDOWN, VK_TAB, 0);
}

// 2012.02.28 KSJ 화살키보드 누를때 정의
void CAccountCtrl::PushDownKey()
{
	m_pCombo->SetFocus();
	m_pCombo->SetCurSel(m_pCombo->GetCurSel()+1);
}

void CAccountCtrl::PushUpKey()
{
	m_pCombo->SetFocus();
	if (m_pCombo->GetCurSel() != 0)
		m_pCombo->SetCurSel(m_pCombo->GetCurSel()-1);
	else
		m_pCombo->SetCurSel(0);
}

void CAccountCtrl::PushLeftKey()
{
	m_pCombo->SetFocus();
	m_pCombo->SetCurSel(m_pCombo->GetCurSel());
	OnComboSelChange();
	m_pCombo->ShowDropDown(FALSE);
}

void CAccountCtrl::PushRightKey()
{
	m_pCombo->SetFocus();
	m_pCombo->SetCurSel(m_pCombo->GetCurSel());
	OnComboSelChange();
	m_pCombo->ShowDropDown(FALSE);
}
// KSJ

void CAccountCtrl::SetAccountType()
{
	m_arrAccType.RemoveAll();
	IStrVectorMap p = m_AccTypeMap.find(m_Param.name);
	if (p==m_AccTypeMap.end())
		return;

	IStrVector pos = p->second.begin();
	for (; pos!=p->second.end(); ++pos)
		m_arrAccType.Add(*pos);
}

void CAccountCtrl::OnDestroy() 
{
	CString strUserPath;
	CString strSaveData, strAccInfo;
	CString strKey = m_Param.name;
	std::shared_ptr<CAccount> pAcc = nullptr;

	strKey.Trim();
	strUserPath.Format("%s\\user\\%s\\%s.ini", m_strRoot, Variant(nameCC), Variant(nameCC));
	const	int	nCount = m_arrHistoryAcc.GetSize();

	if (nCount == 0)
	{
		if (m_pEdit)
		{
			CString strAccount = m_pEdit->GetInputData();
			pAcc = FindAccountInfo(strAccount);

			if (pAcc)
				strSaveData.Format("%s|%s|%s", pAcc->m_strAccntNum, pAcc->m_strAccntName, pAcc->m_strAccntNick);
			else
				strSaveData.Format("%s|%s|", strAccount, m_strAccName);
		}
	}
	else
	{
		for (int ii = 0; ii < nCount; ii++)
		{
			pAcc = m_arrHistoryAcc.GetAt(ii);

			if (pAcc)
				strAccInfo.Format("%s|%s|%s", pAcc->m_strAccntNum, pAcc->m_strAccntName, pAcc->m_strAccntNick);

			if (ii < nCount - 1)
				strAccInfo += _T("\t");

			strSaveData += strAccInfo;
		}
	}

	if (m_pTipCtrl)
		m_pTipCtrl.reset();

	//if (strKey.Find("AN2") == -1) strKey = "AN1A";
	WritePrivateProfileString(_T("AccountHistory"), (LPCTSTR)strKey, (LPCTSTR)strSaveData, (LPCTSTR)strUserPath);
	//enc
	WriteAccountHistory(strUserPath, strKey, strSaveData);
	//enc
	PushDeleteAccHistory();

	m_bDestroy = TRUE;	//2012.02.08 KSJ
	SaveHistory();

	//2011.12.07 KSJ
	if (m_hAccArrow != NULL)	DeleteObject(m_hAccArrow);
	if (m_hAccArrow_DN != NULL)	DeleteObject(m_hAccArrow_DN);
	if (m_hAccArrow_EN != NULL)	DeleteObject(m_hAccArrow_EN);
	if (m_hG != NULL)		DeleteObject(m_hG);
	if (m_hG_DN != NULL)		DeleteObject(m_hG_DN);
	if (m_hG_EN != NULL)		DeleteObject(m_hG_EN);
	if (m_hG_S != NULL)		DeleteObject(m_hG_S);
	if (m_hG_S_DN != NULL)		DeleteObject(m_hG_S_DN);
	if (m_hG_S_EN != NULL)		DeleteObject(m_hG_S_EN);
	if (m_hArrow != NULL)		DeleteObject(m_hArrow);
	if (m_hArrow_DN != NULL)	DeleteObject(m_hArrow_DN);
	if (m_hArrow_EN != NULL)	DeleteObject(m_hArrow_EN);
	//KSJ

	for (auto pWnd : m_arrOubWnd)
	{
		if (pWnd && ::IsWindow(pWnd->GetSafeHwnd()))
			pWnd->DestroyWindow();
		delete pWnd;
	}

	CWnd::OnDestroy();
}

void CAccountCtrl::SaveHistory()
{
	if (m_strDept == m_sAccnDept)
		return;

	CString strSaveData;
	CString strAccInfo;
	CString tmp;
	CString strUserPath;
	CString strKey = m_Param.name;
	std::shared_ptr<CAccount> pAcc = nullptr;

	strKey.Trim();
	strUserPath.Format("%s\\user\\%s\\%s.ini", m_strRoot, Variant(nameCC), Variant(nameCC));

	//2013.05.28 KSJ 계좌명정렬시
	if (m_nSort ==1)
	{
		LoadHistoryAccName();
	}
	else
	{
		LoadHistoryAcc();
	}

	const	int	nCount = m_arrHistoryAcc.GetSize();
	if (nCount == 0)
	{
		if (m_pEdit)
		{
			//2012.02.08 KSJ 종료시에는 입력 데이터를 지워준다.
			if (m_bDestroy)
				m_pEdit->SetInputData("");
			// KSJ

			CString strAccount = m_pEdit->GetInputData();
			strAccount.Trim();
			pAcc = FindAccountInfo(strAccount);
			if (pAcc)
			{
				if (strAccount.GetLength() == 11)
					strSaveData.Format("%s|%s|%s", pAcc->m_strAccntNum, pAcc->m_strAccntName, pAcc->m_strAccntNick);
			}
			else
			{
				if (strAccount.GetLength() == 11)
					strSaveData.Format("%s|%s|", strAccount, m_strAccName);
			}
		}
	}
	else
	{
		for (int ii = 0; ii < nCount; ii++)
		{
			pAcc = m_arrHistoryAcc.GetAt(ii);
			if (pAcc)
			{
				tmp = pAcc->m_strAccntNum;
				tmp.Trim();
				if (tmp.GetLength() != 11) continue;

				strAccInfo.Format("%s|%s|%s", pAcc->m_strAccntNum, pAcc->m_strAccntName, pAcc->m_strAccntNick);
				if (ii < nCount - 1)
					strAccInfo += _T("\t");
				
				strSaveData += strAccInfo;
			}
		}
	}
	
	WritePrivateProfileString(_T("AccountHistory"), (LPCTSTR)strKey, (LPCTSTR)strSaveData, (LPCTSTR)strUserPath);
	//enc
	WriteAccountHistory(strUserPath, strKey, strSaveData);
	//enc
	SortHistory();
}

//전계좌 조회 가능 지점여부 체크   2008.07.22 추가분
BOOL CAccountCtrl::GetAllowAllAcc(CString dep)
{
	CString tmp;
	const	int	nCount = m_allow_dept.GetSize();

	for (int ii = 0; ii < nCount; ii++)
	{
		tmp = m_allow_dept.GetAt(ii);
		if (tmp == dep)
			return TRUE;
	}

	return FALSE;
}

//전 계좌 가능 지점부서 초기화   2008.07.22 추가분
void CAccountCtrl::InitAllowDept()
{
	m_allow_dept.RemoveAll();
	m_allow_dept.Add("903");		//전산실
	m_allow_dept.Add("908");		//영업관리팀
	m_allow_dept.Add("911");		//마케팅전략팀
	m_allow_dept.Add("914");		//준법감시팀
	m_allow_dept.Add("915");		//감사팀
	m_allow_dept.Add("916");		//콜센터
	m_allow_dept.Add("901");		//콜센터

	m_allow_dept.Add("978");

}

//초기화시 유효계좌 체크   2008.07.22 추가분
BOOL CAccountCtrl::IsFirstValidAcc(CString strAccount, BOOL bOrderCheck)
{

	if (!m_bOrderEnable && bOrderCheck)
		return FALSE;

	strAccount.Remove(_T('-'));
	strAccount.Trim();

	if (strAccount.IsEmpty() || strAccount.GetLength() != 11)
		return FALSE;

	BOOL bValid = FALSE;
	CString strType = strAccount.Mid(3, 2);
	int nCount = 0;

	nCount = m_arrAccType.GetSize();

	for (int ii = 0; ii < nCount; ii++)
	{
		if (strType == m_arrAccType.GetAt(ii))
		{
			bValid = TRUE;
			break;
		}
	}

	return bValid;

}

long CAccountCtrl::GetOnlySise()
{
/*	
	int vals = AfxGetApp()->GetProfileInt(WORKSTATION, "OnlySise", 4);
	CString str; str.Format("%d",vals);
	
	CWnd* m_main = AfxGetMainWnd();
	if (m_main == NULL) MessageBox("NULL");
*/
	
	HKEY	hKey = NULL;
	
	//char strKeyPath[255];
	//unsigned char  Value;
	DWORD Value;
	Value = 0;
	/*
	#ifdef _DEBUG
		const char* defaultRegKey = "SOFTWARE\\IBK\\AXIS Workstation V04.00\\Workstation";
	#else  // _RELEASE
		const char* defaultRegKey = "SOFTWARE\\IBK투자증권MAC\\AXIS Workstation V04.00\\Workstation";
	#endif // _DEBUG
	*/
	//const char* defaultRegKey = "SOFTWARE\\IBK투자증권MAC\\AXIS Workstation V04.00\\Workstation";
	CWnd* m_main = AfxGetApp()->GetMainWnd();
	CString str_title;
	if (m_main)
	{
		m_main->GetWindowText(str_title);
	}
	else
	{
		// 메인Frame이 focus를 잃은 경우 위에서 AfxGetApp()->GetMainWnd()는 NULL을 리턴한다.
		// 이렇게 되면, Wizard인 m_pParent를 거슬러 올라가 Title을 추적하도록 한다.          2010.08.05 by lkm
		m_main = m_pParent->GetParent();
		for (int n=0; m_main!=NULL; ++n)
		{
			m_main->GetWindowText(str_title);
			m_main = m_main->GetParent();
		}
	}

	CString file;
	
	file.Format("%s\\%s\\%s\\%s", m_strRoot, "user", m_strName, "axisensetup.ini");
	
	const	int	titleChange = GetPrivateProfileInt("SCREEN", "TITLECHANGE", 0, file);
	
	CString tmp;

	if (titleChange)
	{
		CString userTitle,titleType1,titleType2,titleType3;
		char	buff[1024]{};

		GetPrivateProfileString("SCREEN","USERTITLE","",buff,sizeof(buff),file);
		userTitle = buff;
		
		titleType1.Format("%s(직원용)",userTitle);
		titleType2.Format("%s - 개발용",userTitle);
		titleType3.Format("%s(직원용) - 개발용",userTitle);

		if (str_title == titleType2)
		{
			//MessageBox("Dev");
			tmp = "SOFTWARE\\IBK\\AXIS Workstation V04.00\\Workstation";
		}
		else if (str_title == userTitle)
		{
			//MessageBox("Real");
			tmp = "SOFTWARE\\IBK투자증권MAC\\AXIS Workstation V04.00\\Workstation";
		}
		else if (str_title == titleType1)
		{
			//MessageBox("Real");
			tmp = "SOFTWARE\\IBK투자증권MAC_STAFF\\AXIS Workstation V04.00\\Workstation";
		}	
		else if (str_title == titleType3)
		{
			//MessageBox("Real");
			tmp = "SOFTWARE\\IBK_STAFF\\AXIS Workstation V04.00\\Workstation";
		}
	}
	else
	{
		if (str_title == "IBK개발")
		{
			//MessageBox("Dev");
			tmp = "SOFTWARE\\IBK\\AXIS Workstation V04.00\\Workstation";
		}
		else if (str_title == "IBK hot Trading")
		{
			//MessageBox("Real");
			tmp = "SOFTWARE\\IBK투자증권MAC\\AXIS Workstation V04.00\\Workstation";
		}
		else if (str_title == "[직원]IBK hot Trading")
		{
			//MessageBox("Real");
			tmp = "SOFTWARE\\IBK투자증권MAC_STAFF\\AXIS Workstation V04.00\\Workstation";
		}	
		else if (str_title == "[직원]IBK개발")
		{
			//MessageBox("Real");
			tmp = "SOFTWARE\\IBK_STAFF\\AXIS Workstation V04.00\\Workstation";
		}
	}
	
	//m_slog.Format("titleChange=[%d] tmp=[%s] ", titleChange, tmp);
	//FileLog(m_strRoot, m_slog);

	//const char* defaultRegKey = "SOFTWARE\\IBK\\AXIS Workstation V04.00\\Workstation";
	
	//const char* defaultRegKey = "SOFTWARE\\IBK\\AXIS Workstation V04.00\\Workstation";
    //sprintf(strKeyPath, defaultRegKey);
	
	DWORD dwType = REG_DWORD;
	DWORD dwSize = sizeof(DWORD);
	if (RegOpenKeyEx(HKEY_CURRENT_USER, (CString)tmp, 0, KEY_READ,&hKey) == ERROR_SUCCESS)
	{
		
		if (RegQueryValueEx(hKey, "OnlySise",0, &dwType, (LPBYTE)&Value, &dwSize) == ERROR_SUCCESS) 
		{
			return Value;
		}
		else
		{
			return 0;
		}
	}
	else
		return 0;
	
	RegCloseKey(hKey);
    //return Value;
}

CFont* CAccountCtrl::getAxFont(CString fName, int point, int style)
{
	struct	_fontR	fontR {};

	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point;
	fontR.italic = false;
	fontR.bold = 0;
	switch (style)
	{
	case 0: // none
	case 1: // none
		break;
	case 2: // italic
		fontR.italic = true;
		break;
	case 3: // bold
		fontR.bold = FW_BOLD;
		break;
	case 4: // both
		fontR.italic = true;
		fontR.bold = FW_BOLD;
		break;
	}
	return (CFont*)m_pParent->SendMessage(WM_USER, getFONT, (LPARAM)&fontR);
}

void CAccountCtrl::AppendAccHistory(CString strAcc, CString strPassword, CString strAccName)
{
	CString strData(_T(""));
	CString strPush(_T(""));
	CString strPop = PopAccHistory();
	CString strPushKey(_T(""));

	strPushKey.Format("%s%d", PP_SYM, 0);

	strData.Format("%s\t%s\t%s|", strAcc, strPassword, strAccName);
	
	if (strPop.IsEmpty())
	{
		strPush = strData;
	}
	else
	{
		const	int	nStartPos = strPop.Find((LPCTSTR)strAcc);
		int	nEndPos = -1;

		if (nStartPos != -1)
		{
			nEndPos = strPop.Find(_T("|"), nStartPos);
			if (nEndPos != -1)
				strPop.Delete(nStartPos, nEndPos - nStartPos + 1);
		}

		strPush = strPop+strData;
	}

	strPush = strPushKey + "\t" + strPush;
	Variant(pushCC, strPush);
}

void CAccountCtrl::AddDeleteAccList(CString strAcc)
{
	std::shared_ptr<CAccount> pAccount = nullptr;

	strAcc.Replace("-", "");
	pAccount = FindAccountInfo(strAcc, FALSE);
	
	if (pAccount)
		m_arrDelectAcc.Add(pAccount);
}

////////////////////////// 
// 함수명	    : CAccountCtrl::PushDeleteAccHistory
// 설 명	    : 삭제된 계좌 정보의 내용을 메모리에서 삭제하는 기능
// 작성일시	    : 2009/06/02
// 작성자       : kbkim
// 파라미터	    : CString strAcc : 삭제 지정을 해제하고자 하는 계좌번호
// 리턴 값 		: 
// 관련요건	    : 000010(조원형:2009/05/12)
////////////////////////// 
void CAccountCtrl::DeleteDeleteAccList(CString strAcc)
{
	std::shared_ptr<CAccount> pAccount = nullptr;

	for (int nIndex = m_arrDelectAcc.GetUpperBound() ; nIndex >= 0; nIndex--)
	{
		pAccount = m_arrDelectAcc.GetAt(nIndex);
		if (strAcc == pAccount->m_strAccntNum)
			m_arrDelectAcc.RemoveAt(nIndex);
	}
}

LRESULT CAccountCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	MSG	msg;

	msg.hwnd = m_hWnd;
	msg.message = message;
	msg.wParam = wParam;
	msg.lParam = lParam;
	if (message >= WM_MOUSEFIRST && message <= WM_MOUSELAST && m_staff)
	{
		CRect	rc;
		CString strTip;
		const	int	x = (unsigned int)lParam & 0xFFFF;

		GetClientRect(&rc);
		if (m_pTipCtrl == nullptr)
		{
			m_pTipCtrl = std::make_unique<CToolTipCtrl>();
			m_pTipCtrl->Create(this);
		}
		
		if ((x > 127))
		{
			strTip.Format("그룹 계좌 리스트");
			m_pTipCtrl->AddTool(this, (LPCTSTR)strTip, &rc, GetDlgCtrlID());
			m_pTipCtrl->Activate(TRUE);
			m_pTipCtrl->RelayEvent(&msg);
		}
		else if ((x < 127) && (x >=125))
		{
			m_pTipCtrl->DelTool(this);
			m_pTipCtrl->Activate(FALSE);
		}
		else if ((x >= 105) && (x < 120))
		{
			strTip.Format("전체 계좌 리스트");
			m_pTipCtrl->AddTool(this, (LPCTSTR)strTip, &rc, GetDlgCtrlID());
			m_pTipCtrl->Activate(TRUE);
			m_pTipCtrl->RelayEvent(&msg);
		}
		else if ((x < 105) && (x >=103))
		{
			m_pTipCtrl->DelTool(this);
			m_pTipCtrl->Activate(FALSE);
		}
		else if ((x < 103))
		{
			strTip.Format("선택한 계좌 히스토리");
			m_pTipCtrl->AddTool(this, (LPCTSTR)strTip, &rc, GetDlgCtrlID());
			m_pTipCtrl->Activate(TRUE);
			m_pTipCtrl->RelayEvent(&msg);
		}
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

BOOL CAccountCtrl::IsValidAccComplete(CString strAccount, CString dept, BOOL bOrderCheck)
{
#ifdef DF_TEST_MODE
	return TRUE;
#endif
	if (!m_bOrderEnable && bOrderCheck)
		return FALSE;

	strAccount.Remove(_T('-'));
	strAccount.Trim();	
	if (strAccount.IsEmpty() || strAccount.GetLength() != 11)
		return FALSE;
	
	BOOL	bValid = FALSE;
	CString strType = strAccount.Mid(3, 2);
	int	nCount = 0;
	
	nCount = m_arrAccType.GetSize();
	for (int ii = 0; ii < nCount; ii++)
	{
		if (strType == m_arrAccType.GetAt(ii))
		{
			bValid = TRUE;
			break;
		}
	}
	
	if (!bValid)
		return bValid;
	
	BOOL	bList = FALSE;
	std::shared_ptr<CAccount> pAcc = nullptr;

	nCount = m_arrAllAcc.GetSize();
	for (int ii = 0; ii < nCount; ii++)
	{
		pAcc = m_arrAllAcc.GetAt(ii);
		if (strAccount == pAcc->m_strAccntNum)
		{
			bList = TRUE;
			break;
		}
	}

	// 전계좌 조회 부서 혹은 지점 체크
	CString strDept;
	if (m_bEditMode) //직원이라면 지점번호받아옴
	{
		strDept = Variant(deptCC);
		strDept.Trim();
	}
	if (GetAllowAllAcc(strDept))  //전체 허용 지점 리스트 체크
	{
		if (strAccount.GetLength() >= 3 && strDept.GetLength() >= 3)
		bList = TRUE;
	}
	else if (m_bEditMode && m_bOrderEnable && !bList)	// 직원이면서 관리 계좌가 아닌 경우
	{
		//직원과 입력한 계좌명의 부서코드가 동일하면 관리 계좌가 아니더라도 True
		
 		if (strAccount.GetLength() >= 3 && strDept.GetLength() >= 3)
 		{
 			//if (strAccount.Left(3) == strDept.Left(3))
 				bList = TRUE;
 		}
	}

	//TRACE(strDept+" <==> "+dept);
	if ((bValid && bList) || (strDept == dept))
		return TRUE;

	return FALSE;
}

BOOL CAccountCtrl::IsNumber(CString str)
{
	BOOL	bRet = TRUE;
	const	int	nLen = str.GetLength();

	for (int ii = 0; ii < nLen; ii++)
	{
		if (isdigit(str.GetAt(ii)) == FALSE)
		{
			bRet = FALSE;
			break;
		}
	}
	return bRet;
}

CString CAccountCtrl::GetAcntTypeName(CString tp)
{
	CString ret;

	tp.Trim();
	if (tp.GetLength() != 2 || tp.IsEmpty())
		return "";

	if (IsNumber(tp))
	{
		switch (atoi(tp))
		{
		case 0:	ret = "종합";		break;
		case 10:ret = "주식/채권";		break;
		case 11:ret = "ELW전용";	break;
		case 12:ret = "코넥스";		break;
		case 15:ret = "연금";		break;
		case 55:ret = "연금저축";		break;
		case 20:ret = "선물옵션";	break;
		case 21:ret = "옵션전용";	break;
		case 30:ret = "일반저축";	break;
		case 31:ret = "생계저축";	break;
		case 32:ret = "세우저축";	break;
		case 37:ret = "ISA";		break;
		case 57:ret = "ISA펀드";		break;
		case 50:ret = "자산관리";	break;
		case 51:ret = "CMA";	break;
		case 52:ret = "RP전용";		break;
		case 53:ret = "수익증권";	break;
		case 60:ret = "WRAP";		break;
		case 70:ret = "신탁";		break;
		case 91:ret = "상품저축";
		default:ret = "";		break;
		}
	}

	if (ret.IsEmpty())
		return "";

	CString tmp;
	tmp.Format("%-10s", ret);

	return tmp;
}

void CAccountCtrl::PushDeleteAllAccHistory()
{
 	CString strPush(_T(""));
 	CString strPushKey(_T(""));

	strPushKey.Format("%s%d", PP_SYM, 0);
		
	Variant(pushCC, strPushKey + "\t" + strPush);
}

void CAccountCtrl::LoadAllHistoryAcc()
{
	SafeRemoveAccArray(&m_arrHistoryAcc);

	bool	isWrap = false;
	CString strData, strTemp;
	CString	strPop = PopAccHistory();
	std::shared_ptr<CAccount> pAcc = nullptr;
	std::shared_ptr<CAccount> pAcc2 = nullptr;

	while (!strPop.IsEmpty())
	{
		strData = Parser(strPop, _T("|"));
		strTemp = Parser(strData, _T("\t"));

		isWrap = false;
		for (int ii = 0; ii < m_arrAllAcc.GetSize(); ii++)
		{
			pAcc2 = m_arrAllAcc.GetAt(ii);
			if (pAcc->m_strAccntNum == pAcc2->m_strAccntNum)
			{
				if ((m_bWrap && !pAcc2->m_bWrap) || (!m_bWrap && pAcc2->m_bWrap))
				{
					isWrap = true;
					break;
				}
			}
		}
		if (isWrap)
			continue;

		pAcc = std::make_shared<CAccount>();
		pAcc->m_strAccntNum = strTemp;
		pAcc->m_strPassword = Parser(strData, _T("\t"));
		pAcc->m_strAccntName = Parser(strData, _T("\t"));
		pAcc->TrimData();

		m_arrHistoryAcc.Add(pAcc);
	}
}

//2013.05.28 KSJ 계좌명정렬
void CAccountCtrl::LoadAllHistoryAccName()
{
	SafeRemoveAccArray(&m_arrHistoryAcc);
	
	CString strData, strTemp;
	CString strPop = PopAccHistory();
	std::shared_ptr<CAccount> pAcc  = nullptr;
	std::shared_ptr<CAccount> pAcc2 = nullptr;
	bool	isWrap = false;
	bool	isSwap = false;
	int	ii = 0;

	while (!strPop.IsEmpty())
	{
		strData = Parser(strPop, _T("|"));
		strTemp = Parser(strData, _T("\t"));

		isWrap = false;
		for (ii = 0; ii < m_arrAllAcc.GetSize(); ii++)
		{
			pAcc2 = m_arrAllAcc.GetAt(ii);
			if (pAcc->m_strAccntNum == pAcc2->m_strAccntNum)
			{
				if ((m_bWrap && !pAcc2->m_bWrap) || (!m_bWrap && pAcc2->m_bWrap))
				{
					isWrap = true;
					break;
				}
			}
		}
		if (isWrap)
			continue;

		pAcc = std::make_shared<CAccount>();
		pAcc->m_strAccntNum = strTemp;
		pAcc->m_strPassword = Parser(strData, _T("\t"));
		pAcc->m_strAccntName = Parser(strData, _T("\t"));
		pAcc->TrimData();

		ii = 0;
		isSwap = false;
		for ( ; ii < m_arrHistoryAcc.GetSize(); ii++)
		{
			if (pAcc->m_strAccntName.Compare(m_arrHistoryAcc.GetAt(ii)->m_strAccntName) < 0)
			{
				isSwap = true;
				break;
			}
		}

		if (isSwap)
			m_arrHistoryAcc.InsertAt(ii, pAcc);
		else
			m_arrHistoryAcc.Add(pAcc);
	}
}

CString CAccountCtrl::Find813Pswd(CString acnt)
{
	CString	Path;
	int	grec = 0;
	int	pos = 0;
	CString ecod, emsg, acno, acpw;
	bool	find = false;
	CFile	file;

	acnt.TrimRight();
	Path.Format("%s\\%s\\%s\\AccList.dat", m_strRoot, USRDIR, m_strName);
	if (file.Open(Path, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
	{
		const	int	len = (int)file.GetLength();

		if (len > 0)	
		{
			std::unique_ptr<char[]> pBuf = std::make_unique<char[]>(len);
			file.Read(pBuf.get(), len);
						
			find = false;
			while (pos < len)
			{
				acno = CString(&pBuf[pos], LEN_ACNO);		pos += LEN_ACNO;
				pos += LEN_ACNM;		// acnm
				acpw = CString(&pBuf[pos], LEN_ACPW);		pos += LEN_ACPW;
				grec = atoi(CString(&pBuf[pos], LEN_GREC));	pos += LEN_GREC;
				pos += (grec * LEN_GPID);	// gpid

				acno.TrimRight();
				if (acno == acnt)
				{
					find = true;
					break;
				}
				//pos+=1;			//서비스에서 리드해올땐 필요없음
			}
			pBuf.reset();
		}
		file.Close();
	}
	
	return (find ? acpw : "");
}

CString CAccountCtrl::Find813Name(CString acnt)
{
	CString	Path;
	int	grec = 0;
	int	pos  = 0;
	CString ecod, emsg, acno, acnm;
	bool	find = false;
	CFile	file;

	acnt.TrimRight();
	Path.Format("%s\\%s\\%s\\AccList.dat", m_strRoot, USRDIR, m_strName);
	if (file.Open(Path, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
	{
		const	int	len = (int)file.GetLength();
		if (len > 0)	
		{
			std::unique_ptr<char[]> pBuf = std::make_unique<char[]>(len);
			file.Read(pBuf.get(), len);
						
			find = false;
			while (pos < len)
			{
				acno = CString(&pBuf[pos], LEN_ACNO);		pos += LEN_ACNO;
				acnm = CString(&pBuf[pos], LEN_ACNM);		pos += LEN_ACNM;
				pos += LEN_ACPW;		// acpw
				grec = atoi(CString(&pBuf[pos], LEN_GREC));	pos += LEN_GREC;
				pos += (grec * LEN_GPID);	// gpid

				acno.TrimRight();
				if (acno == acnt)
				{
					find = true;
					break;
				}
				//pos+=1;			//서비스에서 리드해올땐 필요없음
			}
			pBuf.reset();
		}
		file.Close();
	}
	
	return (find ? acnm : "");
}

BOOL CAccountCtrl::ReadAcc813()
{
	CString	Path;
	int	grec = 0;
	int	pos  = 0;
	CString ecod, emsg, acno, acnm, acpw;
	std::shared_ptr<CAccount> pAcc = nullptr;
	CFile	file;

	Path.Format("%s\\%s\\%s\\AccList.dat", m_strRoot, USRDIR, m_strName);
	if (file.Open(Path, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
	{
		const	int	len = (int)file.GetLength();
		if (len > 0)	
		{
			std::unique_ptr<char[]> pBuf = std::make_unique<char[]>(len);
			file.Read(pBuf.get(), len);
							
			while (pos < len)
			{
				acno = CString(&pBuf[pos], LEN_ACNO);		pos += LEN_ACNO;
				acnm = CString(&pBuf[pos], LEN_ACNM);		pos += LEN_ACNM;
				acpw = CString(&pBuf[pos], LEN_ACPW);		pos += LEN_ACPW;
				grec = atoi(CString(&pBuf[pos], LEN_GREC));	pos += LEN_GREC;
				pos += (grec * LEN_GPID);

				if (IsVaidAcc813(acno))
				{
					pAcc = std::make_shared<CAccount>();
					pAcc->m_strAccntNum = acno;
					pAcc->m_strPassword = acpw;
					pAcc->m_strAccntName = acnm;
					pAcc->m_strAccntNick = "";
					pAcc->m_bDelegation	 = 0;
					pAcc->TrimData();
						
					m_arrAllAcc.Add(pAcc);
				}
				//pos+=1;			//서비스에서 리드해올땐 필요없음
			}
			pBuf.reset();
		}
		file.Close();
		return true;
	}

	return false;
}

BOOL CAccountCtrl::IsVaidAcc813(CString strAccount)
{
	if (!m_bOrderEnable)
		return FALSE;

	strAccount.Remove(_T('-'));
	strAccount.Trim();

	if (strAccount.IsEmpty() || strAccount.GetLength() != 11)
		return FALSE;

	CString strType = strAccount.Mid(3, 2);
	const	int	nCount = m_arrAccType.GetSize();

	for (int ii = 0; ii < nCount; ii++)
	{
		if (strType == m_arrAccType.GetAt(ii))
			return TRUE;
	}

	return FALSE;
}

void CAccountCtrl::QueryAccntInfo(CString strAcc)
{
	strAcc = m_pEdit->GetInputData();
	strAcc.TrimRight();
	if (strAcc.IsEmpty())
		return;

	struct	_acif	acif{};

	strAcc.Remove('-');
	CopyMemory(acif.acno, (char*)strAcc.Left(11).GetString(), 11);
	CopyMemory(acif.gubn, "09", 2);

	SendTR(_T("PIBFUSIF"), NULL, (char *)&acif, L_acif, (BYTE)TRKEY_ACINFO);	//2012.08.14 KSJ PIBOUSIF ==> PIBFUSIF
}

void CAccountCtrl::PushAcntInfo(CString strAcc, CString strLastAcif)
{
	CString strPush;
	strPush.Format("%s\t%s\t%s", PL_SYM, strAcc, strLastAcif);
	Variant(pushCC, strPush);
}

BOOL CAccountCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if (zDelta > 0)	
		PostMessage(WM_VSCROLL, SB_LINEUP, 0);		
	else
		PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CAccountCtrl::LoadAccountType()
{
	const	struct acc_type_info {
		char *name;
		char *type[100];
	} info[] = {
		{ "AN10", { "10", "11", "12", "37", NULL } },
		{ "AN30", { "30", "31", "32", NULL } },
		{ "AN11", { "50", NULL } },
		{ "AN12", { "51", NULL } },
		{ "AN13", { "52", NULL } },
		{ "AN14", { "60", NULL } },
		{ "AN15", { "70", NULL } },
		{ "AN24", { "51", "52", NULL } },
		{ "AN20", { "20", "21", NULL } },
		{ "AN50", { "53", "57", NULL } },
		{ "AN55", { "53", "55", "57", NULL } },
		{ "AN99", { "00", NULL } },
		{ "AN01", { "10", "11", "12", "30", "31", "32", "37", "50", "51", "52", "60", "70", NULL } },
		{ "AN16", { "50", "51", "52", NULL } },
		{ "AN17", { "60", "70", NULL } },
		{ "AN21", { "20", NULL } },
		{ "AN22", { "21", NULL } },
		{ "AN19", { "10", "37", NULL } },
		{ "AN18", { "10", "11", "12", "30", "37", NULL } },
		{ "AN31", { "30", NULL } },
		{ "AN32", { "31", NULL } },
		{ "AN33", { "32", NULL } },
		{ "AN34", { "10", "37", "51", "52", NULL } },
		{ "AN1A", { "10", "11", "12", "15", "30", "31", "32", "37", "91", NULL } },
		{ "AN1B", { "10", "30", "31", "32", "37", NULL } },
		{ "AN1E", { "15", "55", NULL } },
		{ "AN1D", { "55", NULL } },
		{ "AN00", { "10", "11", "12", "30", "31", "32", "37", "50", "51", "52", "60", "70", "20", "21", "53", "57", "00", NULL } },	
		{ "AN96", { "10", "11", "12", "30", "31", "32", "37", "50", "51", "52", "60", "20", "21", NULL } },	//2013.08.12 KSJ 문서번호(E-Biz지원-13-0342) 1401에서만 사용
		{ "AN95", { "10", "11", "12", "15", "30", "31", "32", "37", "50", "51", "52", "60", "20", "21", NULL } },
		{ "AN97", { "10", "11", "12", "30", "31", "32", "37", "50", "51", "52", "60", "70", "20", "21", NULL } },	
		{ "AN98", { "10", "11", "12", "30", "31", "32", "37", "50", "51", "52", "60", "70", "20", "21", "53", "55", "57", NULL } },
		{ "AN51", { "10", "37", "52", NULL } },
		{ "AN1C", { "10", "37", "30", NULL } },
		{ "AN2A", { "10", "37", "30", "31", "32", "51", "52", NULL } },
		{ "AM99", { "00", NULL } },
		{ "AN3A", { "10", NULL } },
		{ "AN1F", { "10", "11", "15", "30", "31", "32", "37", "91", NULL } },
		{ "AN4A", { "10", "11", "12", "15", "30", "31", "32", "37", "53","55","57","91", NULL } },
		{ "AN5A", { "10", "12", "15", "30", "31", "32", "37", "91", NULL } },
		{ "AN6A", { "10", "12", "15", "30", "37", NULL } }, //20240920 유상예약청약
		{ "AN7A", { "10","15", "37", NULL } },
		{ NULL, { NULL } },
	};

	char	buffini[1024 * 128]{};
	CString pathini;
	CString stemp;
	BOOL	except = false;
	StrVector src;

	pathini.Format("%s\\tab\\ACCOUNTEXCEPT.ini", Variant(homeCC));
	GetPrivateProfileString(_T("ACCOUNTEXCEPT"), _T("MAPNAME"), _T(""), buffini, sizeof(buffini)-1, pathini);

	Split(buffini, '/', &src);
	IStrVector st = src.begin(), ed = src.end();

	for (; st != ed; ++st)
	{
		if (st->Left(8) == m_mapName)
		{
			except = TRUE;
		}
	}
	
	for (int ii = 0; info[ii].name != NULL; ii++)
	{
		StrVector v;
		for (int jj = 0; info[ii].type[jj] != NULL; jj++)
		{
			if (except)
			{
				CString cmp;

				cmp = CString(info[ii].type[jj]);
				if (m_mapName == "IB850500" && (cmp == "37" || cmp == "57"))     //수익증권담보대출   AN50
					continue;
				else if (m_mapName == "IB850400" && (cmp == "37" || cmp == "57"))  //매도대금담보대출  AN10
					continue;
				else if (m_mapName == "IB850600" && (cmp == "37" || cmp == "57"))  //수익증권환매대금담보대출 AN50
					continue;
				else if (m_mapName == "IB611600" && (cmp == "37" || cmp == "57"))  //펀드이동 접수/신청 AN50
					continue;
				else if (m_mapName == "IB100800" && (cmp == "37" || cmp == "57"))  //권리공매도주문  AN1A
					continue; 
				else if (m_mapName == "IB650100" && cmp == "57")  //장내채권주문 AN1A
					continue;
				else if (m_mapName == "IB630100" && cmp == "57")  //소액채권주문 AN1A
					continue;
				else if (m_mapName == "IB670100" && (cmp == "37" || cmp == "12"))  //소액채권주문 AN1A
					continue;
				else if (m_mapName == "IB662000" && (cmp == "37" || cmp == "12"))    
					continue;
			//	else if (m_mapName == "IB821500" && m_Param.name == "AN98" && (cmp == "37" || cmp == "57"))  //MY계좌정보계별계좌현황 AN98
			//		continue;
				else if ((cmp != "12"))  //12계좌를 빼준다
				{
					v.push_back(info[ii].type[jj]);
				}
				/*
				if ((cmp != "12"))
				{
					v.push_back(info[ii].type[jj]);
				}
				*/
			}
			else
			{
				v.push_back(info[ii].type[jj]);
			}
		}

		stemp.Format("%s", info[ii].name);
		stemp.TrimRight();
		if (m_mapName == "IB821500" && stemp == "AN98")  //연금저축계좌 My계좌정보에만 추가
			v.push_back("15");

		m_AccTypeMap[info[ii].name] = v;
	}
}

void CAccountCtrl::Convert_V2()
{
	char	buff[1024 * 10]{};
	CString iniPath;

	iniPath.Format("%s\\user\\%s\\%s.ini", Variant(homeCC), Variant(nameCC), Variant(nameCC));
	const	int	ver = GetPrivateProfileInt("AccountHistory", "VERSION", 0, iniPath);

	if (ver == 0)
	{
		// AN1A 에 모든 히스토리저장되어 있다. 이걸 각자 타입별로 찢어야 한다.
		GetPrivateProfileString("AccountHistory", "AN1A", "", buff, sizeof(buff)-1, iniPath);

		//enc
		CString val = ReadAccountHistory(iniPath, "AN1A");
		strncpy_s(buff, sizeof(buff), val, _TRUNCATE);
		int readl = (int)strlen(buff);
		//enc
		
		StrVector vAcnt;
		IStrVector p;
		IStrVectorMap p2;
		StrVectorMap tmpData;

		Split(buff, '\t', &vAcnt);
		for (p = vAcnt.begin(); p < vAcnt.end(); p++)
		{
			StrVector vField;

			Split(*p, '|', &vField);		// 0:AcntNumber, 1:AcntName, 2:AcntAlias
			if (vField.size() < 2)
				continue;

			CString type = vField[0].Mid(3, 2);
			for (p2 = m_AccTypeMap.begin(); p2!=m_AccTypeMap.end(); ++p2)
			{
				if (std::find(p2->second.begin(), p2->second.end(), type) != p2->second.end())
				{
					if (tmpData.find(p2->first)==tmpData.end())
						tmpData[p2->first] = StrVector();

					tmpData[p2->first].push_back(*p);
				}
			}
		}
		
		for (p2=tmpData.begin(); p2!=tmpData.end(); ++p2)
		{
			if (p2->second.empty())
				continue;

			CString data;
			Join(&p2->second, '\t', &data);
			WritePrivateProfileString("AccountHistory", p2->first, data, iniPath);
			//enc
			WriteAccountHistory(iniPath, p2->first, data);
			//enc
			WritePrivateProfileString("AccountHistory", "VERSION", "2", iniPath);
		}
	}
}

void CAccountCtrl::Split(const char *text, const char del, StrVector *pResult)
{
	pResult->clear();
	LPCSTR st = text, ed = text + strlen(text);
	while (st<ed)
	{
		LPCSTR pos = std::find(st, ed, del);
		pResult->push_back(CString(st, pos-st));
		st = pos + 1;
	}
}

void CAccountCtrl::Join(StrVector *pSrc, const char del, CString *pResult)
{
	pResult->Empty();
	size_t	n = 0;
	const	size_t	size = pSrc->size();
	if (size == 0)
		return;

	for (n = 0; n < size-1; n++)
	{
		(*pResult) += (*pSrc)[n];
		(*pResult) += del;
	}
	(*pResult) += (*pSrc)[n];
}

void CAccountCtrl::SortHistory()
{
	DWORD	size;
	char	buff[1024*64];
	CString iniPath;

	iniPath.Format("%s\\user\\%s\\%s.ini", Variant(homeCC), Variant(nameCC), Variant(nameCC));
	size = GetPrivateProfileString("AccountHistory", m_Param.name, "", buff, sizeof(buff)-1, iniPath);

	//enc
	CString val = ReadAccountHistory(iniPath, m_Param.name);
	strncpy_s(buff, sizeof(buff), val, _TRUNCATE);
	int readl = (int)strlen(buff);
	size = readl;
	//enc

	if (size == 0)
		return;

	StrVector dst;
	Split(buff, '\t', &dst);
	if (dst.empty())
		return;
	
	if (dst[0].GetLength()<11)
		return;

	CString type = dst[0].Mid(3,2);
	IStrVectorMap pos = m_AccTypeMap.begin();

	for (; pos!=m_AccTypeMap.end(); ++pos)
	{
		if (m_Param.name == pos->first) continue;		// 같은 타입은 당연히 할필요 없지!
		if (std::find(pos->second.begin(), pos->second.end(), type) == pos->second.end()) continue; // 해야할놈만 하자!
		
		size = GetPrivateProfileString("AccountHistory", pos->first, "", buff, sizeof(buff)-1, iniPath);

		//enc
		CString val = ReadAccountHistory(iniPath, pos->first);
		strncpy_s(buff, sizeof(buff), val, _TRUNCATE);
		int readl = (int)strlen(buff);
		size = readl;
		//enc

		
		StrVector src;
		Split(buff, '\t', &src);
		
		IStrVector spos = std::find(src.begin(), src.end(), dst[0]);
		if (spos==src.end())
		{
			src.insert(src.begin(), dst[0]);
		}
		else
		{
			src.erase(spos);
			src.insert(src.begin(), dst[0]);
		}

		CString data;
		Join(&src, '\t', &data);

		WritePrivateProfileString("AccountHistory", pos->first, data, iniPath);
		//enc
		WriteAccountHistory(iniPath, pos->first, data);
		//enc
	}
}

CString CAccountCtrl::GetAcntName(LPCSTR acno)
{
	if (strlen(acno) == 11)
	{			
		std::shared_ptr<CAccount> pAcc = FindAccountInfo(acno);
		if (pAcc)
		{
			if (m_bUseAlias)
				return pAcc->m_strAccntNick.IsEmpty() ? pAcc->m_strAccntName : pAcc->m_strAccntNick;

			return pAcc->m_strAccntName;
		}
	}
	return "";
}

void CAccountCtrl::DeleteHistory(LPCSTR acno)
{
	if (acno == NULL || strlen(acno) != 11)
		return;

	DWORD	size = 0;
	char	buff[1024 * 128]{};
	CString iniPath;
	CString data;

	iniPath.Format("%s\\user\\%s\\%s.ini", Variant(homeCC), Variant(nameCC), Variant(nameCC));
	IStrVectorMap pos = m_AccTypeMap.begin();	
	for ( ; pos != m_AccTypeMap.end(); ++pos)
	{
		if (m_Param.name == pos->first)
			continue;		// 같은 타입은 당연히 할필요 없지!
		
		size = GetPrivateProfileString("AccountHistory", pos->first, "", buff, sizeof(buff)-1, iniPath);

		//enc
		CString val = ReadAccountHistory(iniPath, pos->first);
		strncpy_s(buff, sizeof(buff), val, _TRUNCATE);
		int readl = (int)strlen(buff);
		size = readl;
		//enc

		if (size == 0)
			continue;

		StrVector src;
		Split(buff, '\t', &src);
		
		IStrVector st = src.begin(), ed = src.end();
		for ( ; st != ed; ++st)
		{
			if (st->Left(11) == acno)
				st = src.erase(st);
		}
		
		Join(&src, '\t', &data);
		
		WritePrivateProfileString("AccountHistory", pos->first, data, iniPath);
		//enc
		WriteAccountHistory(iniPath, pos->first, data);
		//enc
	}
}

BOOL CAccountCtrl::IsBankLinked() 
{
	return m_bBankLinked;
}

//2013.03.12 KSJ 대리인 추가
long CAccountCtrl::GetAgentType() 
{
	return m_nAgentType;
}

void CAccountCtrl::SetAgentType(long nNewValue) 
{
	m_nAgentType = AGENT_NOITEM;

	//설정된 값 체크
	if (nNewValue > AGENT_NOITEM && nNewValue < AGENT_ITEM8 + 1)
	{
		m_nAgentType = nNewValue;
		QuerySACMQ329();
	}
}

//금융상품대리인이나, 종합계좌간자금대체대리인이 설정된 계좌인지 체크
BOOL CAccountCtrl::IsAccAgent(LPCTSTR strAcc, long nAgentType) 
{
	BOOL	bResult = FALSE;
	CString strAccntNum = strAcc;
	std::shared_ptr<CAccount> pAcc = nullptr;

	if (strAccntNum.IsEmpty())
		strAccntNum = m_pEdit->GetInputData();

	for (int ii = 0; ii < m_arrAllAcc.GetSize(); ++ii)
	{
		pAcc = m_arrAllAcc.GetAt(ii);
		if (strAccntNum == pAcc->m_strAccntNum)
		{
			switch (nAgentType)
			{
			case AGENT_ITEM5:
				bResult = pAcc->m_bAgent5;
				break;

			case AGENT_ITEM6:
				bResult = pAcc->m_bAgent6;
				break;

			default:
				break;
			}
		}
	}

	return bResult;
}
//KSJ

BSTR CAccountCtrl::GetAgentAccList() 
{
	CString strResult;

	strResult = m_strAgenAcctList;
	return strResult.AllocSysString();
}

BSTR CAccountCtrl::GetAgentNameList() 
{
	CString strResult;

	strResult = m_strAgenNameList;
	return strResult.AllocSysString();
}

void CAccountCtrl::ReHistroy(LPCTSTR accdata, LPCTSTR accnmdata) 
{
	CString stracc, straccnm;

	stracc.Format("%s", accdata);
	straccnm.Format("%s", accnmdata);

// 	PushAccHistory(accdata, _T(""), accnmdata);
	PushAccHistory(CString(accdata), _T(""), straccnm);
}


// 입력받은 한글의 초성을 반환
const char* CAccountCtrl::GetInitialSound(char *han_src)
{
	/* index 사용에 전반적인 오류 있음.
	const char *isarr[19] =
			{"ㄱ","ㄲ","ㄴ","ㄷ","ㄸ","ㄹ","ㅁ","ㅂ","ㅃ","ㅅ","ㅆ",
			"ㅇ","ㅈ","ㅉ","ㅊ","ㅋ","ㅌ","ㅍ","ㅎ"};
	const char *isarr2[19] =
			{"가","까","나","다","따","라","마","바","빠","사","싸","아","자","짜","차","카","타","파","하"    };
	
	int	index = 0; 
	for (index = 0;index < 19; index++)
	{
		if ((strcmp(isarr2[index],han_src) <= 0) && (strcmp(isarr2[index+1],han_src) > 0))
			return isarr[index - 1];
	}
	
	if ((strcmp(isarr2[index],han_src) <=0) && (strcmp("힛",han_src) >=0))
		return isarr[index];
	*/
	return NULL;
}

void CAccountCtrl::OnAccNameChanged() 
{
	// TODO: Add notification handler code

}


BSTR CAccountCtrl::GetShowHideAcc(BSTR strShow, BSTR strHide)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	CStringArray arrShow, arrHide;
	CString strtmp{};

	//보여줘야할 계좌 ------------------------------------------------------
	strtmp.Format("%s", strShow);
	int startPos = 0;
	int endPos = 0;
	while ((endPos = strtmp.Find(',', startPos)) != -1) {
		CString subStr = strtmp.Mid(startPos, endPos - startPos);
		arrShow.Add(subStr);
		startPos = endPos + 1;
	}
	// 마지막 구분자 이후의 문자열 추가
	CString lastSubStr = strtmp.Mid(startPos);
	if (!lastSubStr.IsEmpty())
		arrShow.Add(lastSubStr);


	//보여주지 않아야 할 계좌------------------------------------------------
	strtmp.Format("%s", strHide);
	startPos = 0;
	endPos = 0;
	while ((endPos = strtmp.Find(',', startPos)) != -1) {
		CString subStr = strtmp.Mid(startPos, endPos - startPos);
		arrHide.Add(subStr);
		startPos = endPos + 1;
	}
	// 마지막 구분자 이후의 문자열 추가
	lastSubStr = strtmp.Mid(startPos);
	if (!lastSubStr.IsEmpty())
		arrHide.Add(lastSubStr);

	CString strData = Variant(accountCC);
	CString strName, strAccInfo, strAccnum, strpass, strGubn, strAccText, strType;
	CMapStringToString tmpAcc;

	while (!strData.IsEmpty())
	{
		strAccInfo = Parser(strData, _T("\n"));
		strAccnum = Parser(strAccInfo, _T("\t"));
		strpass = Parser(strAccInfo, _T("\t"));
		strName = Parser(strAccInfo, _T("|"));

		strAccnum.Trim();
		strName.Trim();

		strType = strAccnum.Mid(3, 2);
		switch (_ttoi(strType))
		{
		case 10:	strGubn = _T("주식/채권");			break;
		case 11:	strGubn = _T("ELW전용");			break;
		case 12:	strGubn = _T("코넥스");			break;
		case 15:	strGubn = _T("연금");			break;
		case 20:	strGubn = _T("선물옵션");			break;
		case 21:	strGubn = _T("옵션전용");			break;
		case 30:	strGubn = _T("일반저축");			break;
		case 51:	strGubn = _T("CMA");			break;
		case 52:	strGubn = _T("RP전용");			break;
		case 53:	strGubn = _T("수익증권");			break;
		case 55:	strGubn = _T("연금저축");			break;
		case 57:	strGubn = _T("ISA펀드");			break;
		case 70:	strGubn = _T("신탁");			break;
		default:  	strGubn = _T("              ");			break;
		}
		strAccText = strAccnum;
		strAccText.Insert(ACC_DEPT, _T("-"));
		strAccText.Insert(ACC_DEPT + ACC_TYPE + 1, _T("-"));

		BOOL bAddfind = FALSE;
		BOOL bRemovefind = FALSE;
		for (int ii = 0; ii < arrShow.GetSize(); ii++)
		{
			strtmp = arrShow.GetAt(ii);
			if (strtmp == strType)
				bAddfind = TRUE;
		}

		for (int ii = 0; ii < arrHide.GetSize(); ii++)
		{
			strtmp = arrHide.GetAt(ii);
			if (strtmp == strType)
				bRemovefind = TRUE;
		}

		if (arrShow.GetSize() == 0)																 //파라미터가 공백으로 올경우 전부 추가
			strResult += _T("\t") + strAccnum + strAccText + _T("   ") + strGubn + _T("   ") + strName;
		else if (arrShow.GetSize() == 0 && arrHide.GetSize() > 0)              //안보여주는 계좌만 들어온 경우
		{
			if(!bRemovefind)
				strResult += _T("\t") + strAccnum + strAccText + _T("   ") + strGubn + _T("   ") + strName;
		}
		else if (arrHide.GetSize() == 0 && arrShow.GetSize() > 0)             //보여주는 계좌만 들어온 경우
		{
			if (bAddfind)
				strResult += _T("\t") + strAccnum + strAccText + _T("   ") + strGubn + _T("   ") + strName;
		}
	}

	return strResult.AllocSysString();
}


BSTR CAccountCtrl::GetShowHideAccName(BSTR strShow, BSTR strHide)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CStringArray arrShow, arrHide;
	CString strtmp{}, strResult{};

	//보여줘야할 계좌 ------------------------------------------------------
	strtmp.Format("%s", strShow);
	int startPos = 0;
	int endPos = 0;
	while ((endPos = strtmp.Find(',', startPos)) != -1) {
		CString subStr = strtmp.Mid(startPos, endPos - startPos);
		arrShow.Add(subStr);
		startPos = endPos + 1;
	}
	// 마지막 구분자 이후의 문자열 추가
	CString lastSubStr = strtmp.Mid(startPos);
	if (!lastSubStr.IsEmpty())
		arrShow.Add(lastSubStr);


	//보여주지 않아야 할 계좌------------------------------------------------
	strtmp.Format("%s", strHide);
	startPos = 0;
	endPos = 0;
	while ((endPos = strtmp.Find(',', startPos)) != -1) {
		CString subStr = strtmp.Mid(startPos, endPos - startPos);
		arrHide.Add(subStr);
		startPos = endPos + 1;
	}
	// 마지막 구분자 이후의 문자열 추가
	lastSubStr = strtmp.Mid(startPos);
	if (!lastSubStr.IsEmpty())
		arrHide.Add(lastSubStr);

	CString strData = Variant(accountCC);
	CString strName, strAccInfo, strAccnum, strpass, strGubn, strAccText, strType;
	CMapStringToString tmpAcc;

	while (!strData.IsEmpty())
	{
		strAccInfo = Parser(strData, _T("\n"));
		strAccnum = Parser(strAccInfo, _T("\t"));
		strpass = Parser(strAccInfo, _T("\t"));
		strName = Parser(strAccInfo, _T("|"));

		strAccnum.Trim();
		strName.Trim();

		strType = strAccnum.Mid(3, 2);

		BOOL bAddfind = FALSE;
		BOOL bRemovefind = FALSE;
		for (int ii = 0; ii < arrShow.GetSize(); ii++)
		{
			strtmp = arrShow.GetAt(ii);
			if (strtmp == strType)
				bAddfind = TRUE;
		}

		for (int ii = 0; ii < arrHide.GetSize(); ii++)
		{
			strtmp = arrHide.GetAt(ii);
			if (strtmp == strType)
				bRemovefind = TRUE;
		}

		if (arrShow.GetSize() == 0)  //파라미터가 공백으로 올경우 전부 추가
			strResult += _T("\t") + strName;
		else if (arrShow.GetSize() == 0 && arrHide.GetSize() > 0)              //안보여주는 계좌만 들어온 경우
		{
			if (!bRemovefind)
				strResult += _T("\t") + strName;
		}
		else if (arrHide.GetSize() == 0 && arrShow.GetSize() > 0)             //보여주는 계좌만 들어온 경우
		{
			if (bAddfind)
				strResult += _T("\t") + strName;
		}
	}

	return strResult.AllocSysString();
}

void CAccountCtrl::QueryAccntCheck(CString strAccount)
{
	//m_slog.Format("				[cx_account]  QueryAccntCheck  m_bCheckAgetn=[%d]  m_staff=[%d] m_iAxisState=[%d] m_bPOPTrdTed=[%d] m_bSearching=[%d]",
	//	m_bCheckAgetn, m_staff, m_iAxisState, m_bPOPTrdTed, m_bSearching);
	//OutputDebugString(m_slog);
	//m_pAxisMainFrame->SendMessage(WM_USER, MMSG_SHARED_GUIDEMESSAGE, (LPARAM)(LPSTR)(LPCTSTR)m_slog);
#ifndef DF_TEST_MODE
	if (!m_staff)
		return;

#endif
	
	if (!m_bPOPTrdTed)
		return;

	if (m_bSearching)
	{
		m_slog = "조회중";
		//m_pAxisMainFrame->SendMessage(WM_USER, MMSG_SHARED_GUIDEMESSAGE, (LPARAM)(LPSTR)(LPCTSTR)m_slog);
		return;
	}

	if (m_pAxisMainFrame == nullptr)
	{
		shareddll.Load("cx_shared.dll");

		if (shareddll.IsLoaded())
		{
			if (!shareddll.Function<const bool WINAPI(int, CWnd**)>("axGetData")(1, &m_pAxisMainFrame))
			{
				m_pAxisMainFrame = GetTopLevelParent();

				//m_slog.Format("				[cx_account]  GetTopLevelParent m_pAxisMainFrame=[%x]", m_pAxisMainFrame);
				//OutputDebugString(m_slog);
			}
			else
			{
				//m_slog.Format("				[cx_account] !!!!not GetTopLevelParent[%x] shared!!! m_pAxisMainFrame=[%x] ", GetTopLevelParent(), m_pAxisMainFrame);
				//OutputDebugString(m_slog);
			}
		}
	}

	m_iAxisState = m_pAxisMainFrame->SendMessage(WM_USER, MMSG_GETMAIN_STATE, 0);  //real
	if (m_iAxisState <= 5)
		return;

	CString strData;
	char	datb[L_userTH + L_ledgerH + L_ISACAQ029 + 128]{};
	struct	_userTH	udat {};
	struct	_ledgerH ledger {};
	ISACAQ029 isacaq029{};
	int	index = 0;

	FillMemory(&ledger, L_ledgerH, ' ');
	m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);

	CopyMemory(ledger.svcd, _T("SACAQ029"), sizeof(ledger.svcd));
	CopyMemory(ledger.usid, (LPCTSTR)m_strUser, m_strUser.GetLength());
	CopyMemory(ledger.brno, "000", ACC_DEPT);
	CopyMemory(ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '3';
	ledger.odrf[0] = '2';

	strData = m_pEdit->GetInputData();

	strData.Trim();
	if (strData.IsEmpty())
		return;

	FillMemory(&isacaq029, L_ISACAQ029, ' ');
	CopyMemory(isacaq029.temp, "00001", 5);
	CopyMemory(isacaq029.szAcntNo, (LPCTSTR)strData, 11);

	CopyMemory(udat.trc, "pibopbxq", sizeof(udat.trc));
	udat.stat = US_ENC | US_KEY;

	if (m_bDLL)
		udat.key = (BYTE)TRKEY_CEHCKACC;
	else
	{
		udat.key = m_Param.key;

		datb[index++] = (BYTE)TRKEY_CEHCKACC;
		CopyMemory(&datb[index], (char*)m_Param.name.GetString(), m_Param.name.GetLength());
		index += m_Param.name.GetLength();
		datb[index++] = '\t';
	}

	CopyMemory(&datb[index], (char*)&udat, L_userTH);	index += L_userTH;
	CopyMemory(&datb[index], (char*)&ledger, L_ledgerH);	index += L_ledgerH;
	CopyMemory(&datb[index], (char*)&isacaq029, L_ISACAQ029);

	int iret = m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, L_ISACAQ029 + L_ledgerH), (LPARAM)datb);
	m_bSearching = TRUE;
	SetTimer(TT_029REREAD, 2000, nullptr);

	CString slog;
	slog.Format("\r\n [cx_account] iret=[%d]", iret);
	Output_DebugString(slog);
}

void CAccountCtrl::ShowAcntCheckResult()
{
	CRect rc;
	GetClientRect(&rc);
	ClientToScreen(rc);

	CString sClassName = AfxRegisterWndClass(0);

	m_pOubWnd = new COubWnd;
	m_pOubWnd->m_pWizard = m_pParent;
	m_pOubWnd->m_pParent = this;
	int cx = 230, cy = 100;

	CString slog;
	slog.Format("[cx_account]osacmq329  \r\n");
	Output_DebugString(slog);

	if (!m_pOubWnd->CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_DLGMODALFRAME, sClassName, NULL,
		WS_POPUP | WS_BORDER, CRect(rc.left, rc.bottom, rc.left + cx, rc.bottom + cy),
		NULL, NULL, NULL))
	{
		m_pOubWnd = NULL;
		slog.Format("[cx_account]CreateEx FALSE  \r\n");
		Output_DebugString(slog);
	}
	else
		m_pOubWnd->ShowWindow(SW_SHOWNORMAL);
}

void CAccountCtrl::EnableCtrl(SHORT bEnable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (bEnable)
		m_bEnableCtrl = TRUE;
	else
		m_bEnableCtrl = FALSE;

	m_pCombo->EnableWindow(m_bEnableCtrl);
	m_pEdit->EnableWindow(m_bEnableCtrl);
	
	m_pCombo->Invalidate();
	m_pEdit->Invalidate();
}

CString CAccountCtrl::GetEditData()
{
	if (m_pEdit)
		return m_pEdit->GetInputData();
	else
		return _T("");
}

void CAccountCtrl::GetFontInfo(CString& sFontName, int& iSize, int& itype) const
{
	sFontName = m_Param.fonts;
	iSize = m_Param.point;
	itype = m_Param.style;
}

bool CAccountCtrl::SACAQ0239Ret_Check()
{
	int year = _ttoi(m_sDueDt.Mid(0, 4));
	int month = _ttoi(m_sDueDt.Mid(4, 2));
	int day = _ttoi(m_sDueDt.Mid(6, 2));
	
	COleDateTime inputDate(year, month, day, 0, 0, 0);
	COleDateTime now = COleDateTime::GetCurrentTime();
	COleDateTime today(now.GetYear(), now.GetMonth(), now.GetDay(), 0, 0, 0);

	//투자자구분(m_sInvst   0 전문금융소비자  1 일반금융소비자  9 등록안됨
	//투정미등록 혹은 만기일(m_sDueDt)이 당일 이전 
	//m_sInvst = "9"; //test
	if(m_sInvst == "9" || (inputDate < today)) 
		CreateOubWnd("투자자정보확인서만료(미등록)계좌\n\n입니다");

	//투자자구분(m_sInvst)   0 전문금융소비자  1 일반금융소비자  9 등록안됨
	//정보제공구분(m_sInfogubn)     0 미제공 1 제공 
	// 투자자구분이 일반금융소비자 이고 정보제공구분이 0미제공
	//m_sInvst = "1"; //test
	if (m_sInvst == "1" && m_sInfogubn == "0") 
		CreateOubWnd("투자자정보미제공 고객입니다");
	
	
	// 투자권유구분이 일반금융소비자 이고 투자권유구분  0미제공
	//안정형 0  안정추구형 1 위험중립형 2 적극투자형 3 공격투자형 4
	int igrade{};
	if (m_sCusGrdNm.Find("안정형") >= 0)
		igrade = 0;
	else if (m_sCusGrdNm.Find("안정추구형") >= 0)
		igrade = 1;
	else if (m_sCusGrdNm.Find("위험중립형") >= 0)
		igrade = 2;
	else if (m_sCusGrdNm.Find("적극투자형") >= 0)
		igrade = 3;
	else if (m_sCusGrdNm.Find("공격투자형") >= 0)
		igrade = 4;

	// 투자자구분(m_sInvst   0 전문금융소비자  1 일반금융소비자  9 등록안됨
	// 투자권유구분(m_sInvsCnvs)      //투자권유구분  0 미제공 1 제공
	//정보제공구분(m_sInfogubn)     0 미제공 1 제공 
	//m_sInvsCnvs = "0"; //test
	if ((m_sInvst == "1" && m_sInvsCnvs == "0") || (m_sInvst == "1" && m_sInfogubn == "1" && igrade < 3))
	{
			CString smsg;
			smsg = "고객투자성향을 확인해 주세요!";
			smsg += "\n -고객투자성향 : " + m_sCusGrdNm;

			if (m_sWeakInv == "1")
				smsg += "\n -취약 동의";
			else 	if (m_sWeakInv == "9")
				smsg += "\n -취약 동의안함";
			else 	if (m_sWeakInv == "0")
				smsg += "\n -취약 미해당";

			if (m_sInfogubn == "0")
				smsg += "- 정보 미제공";
			else if (m_sInfogubn == "1")
				smsg += "- 정보 제공";

			if (m_sInvsCnvs == "0")
				smsg += "- 투자권유 미희망";
			else if (m_sInvsCnvs == "1")
				smsg += "- 투자권유 희망";

			smsg += "\n -투자자정보확인서만료 : " + m_sDueDt;

			CreateOubWnd(smsg);
	}

	//test
#ifdef DF_TEST_MODE
	m_sbAgn = "Y";
#endif
	if (m_sbAgn == "Y")
	{
		CString strData;
		m_pEdit->GetWindowText(strData);
		strData.Replace("-", "");
		CString sIniPath = m_sHome ;
#ifdef DF_ACC_HASH
		CString sKey = HashAccount(strData);	// strData = 계좌번호
#else
		CString sKey = EncryptAccount(strData);
#endif

		// 7일 억제 중인지 확인
		TCHAR szDate[16] = {};
		GetPrivateProfileString(_T("AgentPopup"), sKey,
			_T(""), szDate, 16, sIniPath);

		bool bSkip = false;
		if (_tcslen(szDate) == 8)
		{
			CString s(szDate);
			COleDateTime saved(
				_ttoi(s.Mid(0, 4)), _ttoi(s.Mid(4, 2)), _ttoi(s.Mid(6, 2)), 0, 0, 0);
			COleDateTime now = COleDateTime::GetCurrentTime();
			bSkip = ((now - saved).GetTotalDays() < 7.0);
		}

		if (!bSkip)
			CreateOubWnd(" 주문대리인이 약정된 계좌입니다.  \n\n 자세한 사항은 화면번호(8782)에서\n\n 확인하여 주시기 바랍니다.", TRUE);
	}

	return true;
}


void CAccountCtrl::CreateOubWnd(CString sMsg, BOOL bIsAgent)
{
	CString strData;
	strData = m_pEdit->GetInputData();
	strData.Trim();
	strData.Replace("-", "");
	if (strData.IsEmpty())
		return;

	CRect rcParent;
	GetClientRect(&rcParent);
	ClientToScreen(&rcParent);

	int cx = 260, cy = 100;

	// 대리인이면 체크박스 영역만큼 높이 추가
	if (bIsAgent)
		cy = 185;

	COubWnd* pWnd = new COubWnd;
	pWnd->m_pWizard = m_pParent;
	pWnd->m_pParent = this;
	pWnd->m_sMsg = sMsg;
	pWnd->m_bIsAgent = bIsAgent;
	pWnd->m_bDontShow = FALSE;
	pWnd->m_sAccNo = strData;								// 계좌번호 세팅
	pWnd->m_sIniPath = m_sHome ;	// ini 경로 세팅

	CString sClassName = AfxRegisterWndClass(0);

	//-----------------------------
	// 모니터 워크 영역
	//-----------------------------
	HMONITOR hMon = MonitorFromRect(&rcParent, MONITOR_DEFAULTTONEAREST);
	MONITORINFO mi;
	mi.cbSize = sizeof(mi);
	GetMonitorInfo(hMon, &mi);

	CRect rcWork = mi.rcWork;

	int left = rcParent.left;
	int top = 0;

	//-----------------------------
	// 팝업이 1개 이상이면 최근 팝업 아래에 배치
	//-----------------------------
	if (!m_arrOubWnd.empty())
	{
		COubWnd* pLast = m_arrOubWnd.back();
		CRect rcLast;
		pLast->GetWindowRect(&rcLast);
		top = rcLast.bottom + 2;
	}
	else
	{
		// 첫 번째 팝업이면 부모 아래
		top = rcParent.bottom;
	}

	int bottom = top + cy;

	//-----------------------------
	// 모니터 아래로 넘어가면 → 첫 번째 팝업 위로 보낸다
	//-----------------------------
	if (!m_arrOubWnd.empty() && bottom > rcWork.bottom)
	{
		COubWnd* pFirst = m_arrOubWnd.front();
		CRect rcFirst;
		pFirst->GetWindowRect(&rcFirst);

		top = rcFirst.top - cy - 2;
		bottom = top + cy;

		if (top < rcWork.top)
			top = rcWork.top;
	}

	//-----------------------------
	// 팝업 생성
	//-----------------------------
	if (!pWnd->CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_DLGMODALFRAME, sClassName, NULL,
		WS_POPUP | WS_BORDER, CRect(left, top, left + cx, top + cy),
		NULL, NULL, NULL))
	{
		delete pWnd;
		return;
	}

	pWnd->ShowWindow(SW_SHOWNORMAL);

	//-----------------------------
	// 배열에 추가
	//-----------------------------
	m_arrOubWnd.push_back(pWnd);
}

// AccountCtrl.cpp 에 추가 (COubWnd 것과 동일 로직)
CString CAccountCtrl::HashAccount(const CString& sAccNo)
{
	DWORD hash = 2166136261u;
	for (int i = 0; i < sAccNo.GetLength(); i++)
	{
		hash ^= (BYTE)sAccNo[i];
		hash *= 16777619u;
	}
	CString sHash;
	sHash.Format(_T("%08X"), hash);
	return sHash;
}

void CAccountCtrl::RemoveOubWnd(COubWnd* pwnd)
{
	auto it = std::find(m_arrOubWnd.begin(), m_arrOubWnd.end(), pwnd);
	if (it != m_arrOubWnd.end())
		m_arrOubWnd.erase(it);
}

CString CAccountCtrl::OptionParser(CString sOption, CString sKey)
{
	CString tmp = sOption;
	Parser(tmp, sKey);
	tmp = Parser(tmp, "/");
	return tmp;
}



CString CAccountCtrl::EncryptAccount(const CString& sAccNo)
{
	CByteArray arr;
	for (int i = 0; i < sAccNo.GetLength(); i++)
	{
		BYTE b = (BYTE)sAccNo[i];
		b ^= s_xorKey[i % s_keyLen];		// XOR
		b += (BYTE)(i * 7);				// 위치별 오프셋 추가
		arr.Add(b);
	}
	return BytesToHex(arr);
}

CString CAccountCtrl::DecryptAccount(const CString& sEncrypted)
{
	CByteArray arr;
	HexToBytes(sEncrypted, arr);

	CString sResult;
	for (int i = 0; i < arr.GetSize(); i++)
	{
		BYTE b = arr[i];
		b -= (BYTE)(i * 7);				// 위치별 오프셋 제거
		b ^= s_xorKey[i % s_keyLen];		// XOR 복원
		sResult += (TCHAR)b;
	}
	return sResult;
}

CString CAccountCtrl::ReadAccountHistory(const CString& strUserPath, const CString& strKey)
{
	char readb[1024 * 64]{};
	GetPrivateProfileStringA("AccountHistory", strKey, "", readb, sizeof(readb), strUserPath);

	CString val(readb);
	if (val.IsEmpty())
		return val;

	if (val.Find('|') >= 0)
		return val;

	// 암호화 여부 확인
	char status[8] = {};
	GetPrivateProfileStringA("ENCRYPT", "STATUS", "0", status, _countof(status), strUserPath);
	if (strcmp(status, "1") == 0)
	{
		auto key = DeriveKeyFromRegkey(m_regkey);
		val = AesDecrypt(val, key);
		SecureZeroMemory(key.data(), key.size());
	}
	return val;
}

// ── 쓰기 공통 헬퍼 (새로 추가) ──────────────────────────────────
void CAccountCtrl::WriteAccountHistory(const CString& strUserPath, const CString& strKey, const CString& strData)
{
	CString valToWrite = strData;

	// 암호화 여부 확인
	char status[8] = {};
	GetPrivateProfileStringA("ENCRYPT", "STATUS", "0", status, _countof(status), strUserPath);
	if (strcmp(status, "1") == 0 && !strData.IsEmpty())
	{
		auto key = DeriveKeyFromRegkey(m_regkey);
		valToWrite = AesEncrypt(strData, key);
		SecureZeroMemory(key.data(), key.size());
	}

	WritePrivateProfileString(_T("AccountHistory"), (LPCTSTR)strKey, (LPCTSTR)valToWrite, (LPCTSTR)strUserPath);
}