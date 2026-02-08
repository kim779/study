#include "stdafx.h"
#include "IBXXXX08.h"
#include "mapForm.h"
#include "libDefine.h"
#include "setupDlg.h"
#include "catchWnd.h"
#include "dlgHistory.h"
#include "../../h/axisfire.h"
#include "../../h/logio.h"
#include "imm.h"



struct	_uinfo {
	char	gubn[2];	// 업무구분
				// "MY" : 관심종목
				// "UL" : 사용자 데이터 목록 
				// "UD" : 사용자 데이터
	char	dirt[1];	// 전송방향 'U' : pc==>host, 'D' : host==>pc, 'X' : host==>pc (conversion), 'R' : 재조회
	char	cont[1];	// 연속구분 'F':First, 'M':Middle, 'L':First&Last, 'C':Cancel
	char	name[80];	// file name
	char	nblc[5];	// 블럭개수, 파일크기(max:64k)
	char	retc[1];	// return value 'O':정상 'E':에러
	char	emsg[40];	// error message
};

struct	_ginfo {
	char	gnox[2];	// group number
	char	gnam[20];	// group name
	char	jrec[4];	// _jinfo count
//	char	jptr[1];	// _jinfo pointer
};

struct	_jinfo {
	char	gubn[1];	// 종목구분	0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
	char	code[12];	// 종목코드
	char	xprc[10];	// 보유단가
	char	xnum[10];	// 보유수량
};

/*
struct	_bookmarkinfo {
	char	gubn[1];	// 종목구분	0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
	char	code[12];	// 종목코드
	int		bookmark;	// 북마크 여부
};
*/
struct	_updn {
	_uinfo	uinfo;
	_ginfo	ginfo;
};

const int sz_uinfo = sizeof(struct _uinfo);
const int sz_ginfo = sizeof(struct _ginfo);
const int sz_jinfo = sizeof(struct _jinfo);
const int sz_updn = sizeof(struct _updn);


#define	trUPDOWN	"pidomyst"	//2012.01.19 KSJ 내용설명 원래 pido이면 마스터로 가게된다 BP21 -> IS21, BP11 -> IS21
#define	trUPDOWN2	"pihomyst"	//2012.01.19 KSJ 내용설명 원래 piho이면 센터내에서 처리한다. BP21 -> IS21, BP11 -> IS11


#define TRKEY_GROUP		0x13	// 관심그룹...
#define TRKEY_INTER		0x14	// 관심리스트...


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// sise
struct _grInfo grInfoSise[] = {
	{ "종목코드",	60,	"1021",	GVFM_CENTER,	0,			"",	0},
	{ "종목명",	127,	"1022",	GVFM_LEFT,	0,			"",	0},
	{ "현재가",	66,	"2023",	GVFM_RIGHT,	GVAT_CONDITIONx,	"",	0},
	{ "전일대비",	66,	"2024",	GVFM_RIGHT,	GVAT_CONTRAST | GVAT_COMMA,		"",	0},
	{ "매도호가",	66,	"2025",	GVFM_RIGHT,	GVAT_CONDITIONx,	"",	0},
	{ "매수호가",	66,	"2026",	GVFM_RIGHT,	GVAT_CONDITIONx,	"",	0},
	{ "등락률",	60,	"2033",	GVFM_RIGHT,	GVAT_CONDITION,		"",	0},
	{ "거래량",	71,	"2027",	GVFM_RIGHT,	GVAT_CONDITIONx,	"",	0},
};
enum { ISCOL_CODE = 0, ISCOL_NAME, ISCOL_CURR, ISCOL_DIFF, ISCOL_MDGA, ISCOL_MSGA, ISCOL_RATE, ISCOL_GVOL };

// 선물 symbol
struct _grInfo grInfoFuture[] = {
	{ "종목코드",	60,	"30021",	GVFM_CENTER,	0,			"",	0},
	{ "종목명",	127,	"30022",	GVFM_LEFT,	0,			"",	0},
	{ "현재가",	66,	"31023",	GVFM_RIGHT,	GVAT_CONDITIONx,	"",	0},
	{ "전일대비",	66,	"31024",	GVFM_RIGHT,	GVAT_CONTRAST,		"",	0},
	{ "매도호가",	66,	"31025",	GVFM_RIGHT,	GVAT_CONDITIONx,	"",	0},
	{ "매수호가",	66,	"31026",	GVFM_RIGHT,	GVAT_CONDITIONx,	"",	0},
	{ "등락률",	60,	"31033",	GVFM_RIGHT,	GVAT_CONDITION,		"",	0},
	{ "거래량",	71,	"31027",	GVFM_RIGHT,	GVAT_CONDITIONx,	"",	0},
};

// 옵션 symbol
struct _grInfo grInfoOption[] = {
	{ "종목코드",	60,	"40021",	GVFM_CENTER,	0,			"",	0},
	{ "종목명",	127,	"40022",	GVFM_LEFT,	0,			"",	0},
	{ "현재가",	66,	"41023",	GVFM_RIGHT,	GVAT_CONDITIONx,	"",	0},
	{ "전일대비",	66,	"41024",	GVFM_RIGHT,	GVAT_CONTRAST,		"",	0},
	{ "매도호가",	66,	"41025",	GVFM_RIGHT,	GVAT_CONDITIONx,	"",	0},
	{ "매수호가",	66,	"41026",	GVFM_RIGHT,	GVAT_CONDITIONx,	"",	0},
	{ "등락률",	60,	"41033",	GVFM_RIGHT,	GVAT_CONDITION,		"",	0},
	{ "거래량",	71,	"41027",	GVFM_RIGHT,	GVAT_CONDITIONx,	"",	0},
};

// catch
struct _grInfo grInfoCatch[] = {
	{ "포착",	30,	"A001",	GVFM_CENTER,	0,		"",	0},
	{ "코드",	54,	"A002",	GVFM_LEFT,	0,		"",	0},
	{ "종목명",	106,	"A003",	GVFM_LEFT,	0,		"",	0},
#ifdef	_DEBUG	
	{ "확인창",	40,	"A005",	GVFM_CENTER,	0,		"",	0},
	{ "관심",	30,	"A006",	GVFM_CENTER,	0,		"",	0},
	{ "소리",	30,	"A007",	GVFM_CENTER,	0,		"",	0},
	{ "주문",	30,	"A004",	GVFM_CENTER,	0,		"",	0},

	{ "현재가",	60,	"A008",	GVFM_RIGHT,	GVAT_COMMA,	"",	0},
	{ "",		16,	"A009",	GVFM_CENTER,	0,		"",	0},
	{ "",		22,	"A010",	GVFM_CENTER,	0,		"",	0},
	{ "전일대비",	70,	"A011",	GVFM_RIGHT,	GVAT_COMMA,	"",	0},
	{ "",		16,	"A012",	GVFM_CENTER,	0,		"",	0},
	{ "",		22,	"A013",	GVFM_CENTER,	0,		"",	0},
	{ "매도호가",	70,	"A014",	GVFM_RIGHT,	GVAT_COMMA,	"",	0},
	{ "",		16,	"A015",	GVFM_CENTER,	0,		"",	0},
	{ "",		22,	"A016",	GVFM_CENTER,	0,		"",	0},
	{ "매수호가",	78,	"A017",	GVFM_RIGHT,	GVAT_COMMA,	"",	0},
	{ "",		16,	"A018",	GVFM_CENTER,	0,		"",	0},
	{ "",		22,	"A019",	GVFM_CENTER,	0,		"",	0},
	{ "등락률",	70,	"A020",	GVFM_RIGHT,	0,	"",	0},
	{ "",		16,	"A021",	GVFM_CENTER,	0,		"",	0},
	{ "",		22,	"A022",	GVFM_CENTER,	0,		"",	0},
	{ "거래량",	80,	"A023",	GVFM_RIGHT,	GVAT_COMMA,	"",	0},
	{ "",		16,	"A024",	GVFM_CENTER,	0,		"",	0},
#else	
	{ "확인창",	0,	"A005",	GVFM_CENTER,	GVAT_HIDDEN,	"",	0},
	{ "관심",	0,	"A006",	GVFM_CENTER,	GVAT_HIDDEN,	"",	0},
	{ "소리",	0,	"A007",	GVFM_CENTER,	GVAT_HIDDEN,	"",	0},
	{ "주문",	30,	"A004",	GVFM_CENTER,	0,		"",	0},

	{ "현재가",	60,	"A008",	GVFM_RIGHT,	GVAT_COMMA,	"",	0},
	{ "",		0,	"A009",	GVFM_CENTER,	GVAT_HIDDEN,	"",	0},
	{ "",		0,	"A010",	GVFM_CENTER,	GVAT_HIDDEN,	"",	0},
	{ "전일대비",	60,	"A011",	GVFM_RIGHT,	GVAT_COMMA,	"",	0},
	{ "",		0,	"A012",	GVFM_CENTER,	GVAT_HIDDEN,	"",	0},
	{ "",		0,	"A013",	GVFM_CENTER,	GVAT_HIDDEN,	"",	0},
	{ "매도호가",	60,	"A014",	GVFM_RIGHT,	GVAT_COMMA,	"",	0},
	{ "",		0,	"A015",	GVFM_CENTER,	GVAT_HIDDEN,	"",	0},
	{ "",		0,	"A016",	GVFM_CENTER,	GVAT_HIDDEN,	"",	0},
	{ "매수호가",	60,	"A017",	GVFM_RIGHT,	GVAT_COMMA,	"",	0},
	{ "",		0,	"A018",	GVFM_CENTER,	GVAT_HIDDEN,	"",	0},
	{ "",		0,	"A019",	GVFM_CENTER,	GVAT_HIDDEN,	"",	0},
	{ "등락률",	50,	"A020",	GVFM_RIGHT,	0,		"",	0},
	{ "",		0,	"A021",	GVFM_CENTER,	GVAT_HIDDEN,	"",	0},
	{ "",		0,	"A022",	GVFM_CENTER,	GVAT_HIDDEN,	"",	0},
	{ "거래량",	71,	"A023",	GVFM_RIGHT,	GVAT_COMMA,	"",	0},
	{ "",		0,	"A024",	GVFM_CENTER,	GVAT_HIDDEN,	"",	0},
#endif
};

enum { ICCOL_CATCH = 0, ICCOL_CODE, ICCOL_NAME,
	ICCOL_WINDOW, ICCOL_INTER, ICCOL_SOUND, ICCOL_JUMUN, 
	ICCOL_CURR, ICCOL_CURREX, ICCOL_CURREX2, 
	ICCOL_DIFF, ICCOL_DIFFEX, ICCOL_DIFFEX2, 
	ICCOL_MDGA, ICCOL_MDGAEX, ICCOL_MDGAEX2, 
	ICCOL_MSGA, ICCOL_MSGAEX, ICCOL_MSGAEX2, 
	ICCOL_RATE, ICCOL_RATEEX, ICCOL_RATEEX2, 
	ICCOL_GVOL, ICCOL_GVOLEX, 
 };

[[gsl::suppress(26409)]]
IMPLEMENT_DYNCREATE(CMapForm, CAxMapForm)

CMapForm::CMapForm()
	: CAxMapForm(CMapForm::IDD)
{
	//{{AFX_DATA_INIT(CMapForm)
	m_strSearch = _T("");
	//}}AFX_DATA_INIT
	initConstruct();
}

CMapForm::~CMapForm()
{
}

void CMapForm::initConstruct()
{
	m_sRootDir = _T("");	m_sTabDir = _T("");	m_sImgDir = _T("");	
	m_szFilename = _T("");	m_sUser = _T("");	m_sName = _T("");
	m_envPath = _T("");	
	m_gubn = ' ';
	m_bCustomer = false;

	m_rtsCode = _T("");
	m_sFontName    = "굴림체";	m_nFontPoint   = 9;	m_pFont = nullptr;	m_pBFont = nullptr;
	m_clrBk = RGB(255, 255, 255);
	m_clrBlink = RGB(255, 255, 255);

	m_pSise = nullptr;
	m_pCatch = nullptr;
	m_pSetupDlg = nullptr;
	m_arCode.RemoveAll();	m_arFCode.RemoveAll();	m_arOCode.RemoveAll();	m_arOHead.RemoveAll();
	m_mapCatch.RemoveAll();
	m_arCatch.RemoveAll();
	m_rtsCode = "";
	m_saveCode = "";

	m_bNum = true;
	m_pPopup = nullptr;
	m_IdCurKind = xALLCODE;
}

void CMapForm::destroy()
{
	deleteAllMapCatch();
	m_arCode.RemoveAll();	m_arFCode.RemoveAll();	m_arOCode.RemoveAll();	m_arOHead.RemoveAll();
}


void CMapForm::DoDataExchange(CDataExchange* pDX)
{
	CAxMapForm::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapForm)
	DDX_Control(pDX, IDC_BTNHISTORY, m_btnHistory);
	DDX_Control(pDX, IDC_BTNCATCHDEL, m_btnCatchDel);
	DDX_Control(pDX, IDC_BTNINTER, m_btnInter);
	DDX_Control(pDX, IDC_BTNAPPLY, m_btnApply);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_SEARCH, m_editSearch);
	DDX_Control(pDX, IDC_TREE, m_tree);
	DDX_Control(pDX, IDC_BTNUP, m_btnUp);
	DDX_Control(pDX, IDC_BTNOK, m_btnOk);
	DDX_Control(pDX, IDC_BTNDEL, m_btnDel);
	DDX_Control(pDX, IDC_BTNCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTNADEL, m_btnADel);
	DDX_Control(pDX, IDC_BTNADD, m_btnAdd);
	DDX_Control(pDX, IDC_BTNSISE, m_btnSise);
	DDX_Text(pDX, IDC_SEARCH, m_strSearch);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMapForm, CAxMapForm)
	//{{AFX_MSG_MAP(CMapForm)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTNADD, OnBtnadd)
	ON_BN_CLICKED(IDC_BTNADEL, OnBtnadel)
	ON_BN_CLICKED(IDC_BTNCANCEL, OnBtncancel)
	ON_BN_CLICKED(IDC_BTNDEL, OnBtndel)
	ON_BN_CLICKED(IDC_BTNOK, OnBtnok)
	ON_BN_CLICKED(IDC_BTNUP, OnBtnup)
	ON_BN_CLICKED(IDC_RDSAVE, OnRdsave)
	ON_BN_CLICKED(IDC_RDDEL, OnRddel)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnSelchangedTree)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_BN_CLICKED(IDC_BTNSISE, OnBtnsise)
	ON_EN_SETFOCUS(IDC_SEARCH, OnSetfocusSearch)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_BN_CLICKED(IDC_BTNAPPLY, OnBtnapply)
	ON_BN_CLICKED(IDC_BTNINTER, OnBtninter)
	ON_BN_CLICKED(IDC_BTNCATCHDEL, OnBtncatchdel)
	ON_BN_CLICKED(IDC_BTNHISTORY, OnBtnhistory)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_KEYMSG, OnKeyEvent)
	ON_MESSAGE(WM_LOCAL, OnLocal)
	ON_NOTIFY(GVNM_LMOUSEUP, IDC_GRCATCH, OnGrCatchLMouseup)
	ON_NOTIFY(GVNM_DBLCLICKED, IDC_GRCATCH, OnGrCatchDblClicked)	
	ON_NOTIFY(GVNM_RMOUSEDOWN, IDC_GRCATCH, OnGrCatchRMouseDown)
END_MESSAGE_MAP()
//ON_EN_CHANGE(IDC_SEARCH, OnChangeSearch)

#ifdef _DEBUG
void CMapForm::AssertValid() const
{
	CAxMapForm::AssertValid();
}

void CMapForm::Dump(CDumpContext& dc) const
{
	CAxMapForm::Dump(dc);
}
#endif //_DEBUG


LRESULT CMapForm::OnMessage(WPARAM wParam, LPARAM lParam)
{
	int len = 0;
	switch(LOBYTE(LOWORD(wParam))) 
	{
	case DLL_OUB:
		m_section.Lock();
		len = HIWORD(wParam);
		if (HIWORD(wParam))
		{
			switch(HIBYTE(LOWORD(wParam)))
			{
			case SC_SISE:		// 현재가등 시세조회
				oubSise((char *)lParam, len);		break;
			case SC_SISEDBCLICK:	// 현재가등 시세조회
				oubSise((char *)lParam, len);		OnBtnadd();	break;
				break;
			case SC_HISTORY:	
				oubHistory((char *)lParam, len);	break;	
			default:
				receiveOub( HIBYTE(LOWORD(wParam)), CString((char*)lParam, len));
				break;
			}	
			

		}
		m_section.Unlock();
		break;
	case DLL_ALERT:
//		m_section.Lock();
///		alert((char *)lParam, (int)wParam);
//		m_section.Unlock();
		break;
	case DLL_ALERTx:
		{
			struct _alertR* alert = (struct _alertR*)lParam;
			if (m_rtsCode.CompareNoCase(alert->code) != 0)
				return 0;
			alertx(gsl::span<DWORD>((DWORD*)alert->ptr[0], 1000));
		}
		break;
	case DLL_TRIGGER:
		trigger((char *)lParam);
		break;
	case DLL_SETPAL:
		ChangeColors();
		break;
	case DLL_DOMINO:
		domino((char *)lParam);
		break;
	}
	return FALSE;
}


LRESULT CMapForm::OnLocal(WPARAM wParam, LPARAM lParam)
{
	switch ((int)wParam)
	{
	case EVT_CHAR:
		{
			POSITION pos = m_list.GetFirstSelectedItemPosition();
			if (!pos)	return FALSE;
			int index = m_list.GetNextSelectedItem(pos);
			CString tmpS;
			switch ((int)lParam)
			{
			case VK_UP:
				index--;
				if (index < 0)	index = 0;
				m_list.SetScroll(index);				
				m_editSearch.GetWindowText(tmpS);
				m_editSearch.SetSel(tmpS.GetLength(), -1);
				break;
			case VK_DOWN:
				index++;
				if (index > m_list.GetItemCount())	index = m_list.GetItemCount();
				m_list.SetScroll(index);
				m_editSearch.GetWindowText(tmpS);
				m_editSearch.SetSel(tmpS.GetLength(), -1);
				break;
			case VK_RETURN:
				{
					UpdateData(TRUE);
					selectList(SC_SISEDBCLICK);
					UpdateData(FALSE);
					m_editSearch.SetWindowText("");
					m_editSearch.SetSel(0, -1);
				}
				break;
			}
		}
		break;

	case EVT_TEXT:	// onchange
		{
			CString str = (char *)lParam;
			char* CmpStr = (LPSTR)str.operator LPCTSTR();
			const char cName = str.GetAt(str.GetLength()-1);

			if ((cName >= '0' && cName <= '9')
				&& (str.Left(1) >= '0' && str.Left(1) <= '9'))
			{
				if (!m_bNum)
				{
					m_bNum = true;
					m_list.ListSort(0);
				}
			}
			else
			{
				if (m_bNum)
				{
					m_bNum = false;
					m_list.ListSort(1);
				}
			}

			if (!m_bNum)
			{
				char *strCmp = (LPSTR)str.operator LPCTSTR();
				const int index = findIndex(strCmp, str.GetLength());
//TRACE("index[%d] [%s]\n", index, str);
				if (index < 0)
					return TRUE;
				m_list.SetScroll(index);
				
			}
			else 
			{
				char *strCmp = (LPSTR)str.operator LPCTSTR();
				const int index = findIndex(strCmp, str.GetLength(), true);
				if (index < 0)
					return TRUE;
				m_list.SetScroll(index);

				if (str.GetLength() == 6)
				{
					selectList();
					m_editSearch.SetFocus();
					m_editSearch.SetSel(0, -1);
					m_editSearch.SetWindowText("");
				}
			}
		}

		break;
	}
	return TRUE;
}

// VK_RETURN값 처리
LRESULT CMapForm::OnKeyEvent(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_RETURN:
		break;
	}
	return 0;
}

void CMapForm::OnGrCatchLMouseup(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GVITEM *pgvitem = (NM_GVITEM*)pNMHDR;
	if (pgvitem->row <= 0)
		return;
	CString codeS = m_pCatch->GetItemText(pgvitem->row, ICCOL_CODE);
	querySise(codeS);
}

void CMapForm::OnGrCatchDblClicked(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GVITEM *pgvitem = (NM_GVITEM*)pNMHDR;
	CString codeS = m_pCatch->GetItemText(pgvitem->row, ICCOL_CODE);
	popupSetup(false, codeS, pgvitem->row);
}

void CMapForm::OnGrCatchRMouseDown(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GVITEM *pgvitem = (NM_GVITEM*)pNMHDR;
	if (pgvitem->row <= 0 || !m_pPopup)
		return;

	_codeinfo *codeinfo = nullptr;
	CString codeS = m_pCatch->GetItemText(pgvitem->row, ICCOL_CODE);
	if (!m_mapCatch.Lookup(codeS, codeinfo))
		return;

	CRect rect;
	m_pCatch->GetItemRect(pgvitem->row, pgvitem->col, rect);
	m_pCatch->ClientToScreen(rect);
	m_pPopup->SetData(codeinfo);
	m_pPopup->Show(CPoint(rect.left, rect.bottom));
}
//=================================================================================================
void CMapForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	Variant(titleCC, "[시세포착설정]");
	Request_GroupList();
	m_sRootDir = Variant(homeCC, "");
	m_sTabDir.Format("%s\\%s\\", m_sRootDir, TABDIR);
	m_sImgDir.Format("%s\\image\\", m_sRootDir);

	CString	gubn = Variant(systemCC, "");
	if (gubn.GetLength() > 0) 
		m_gubn = gubn.GetAt(0);

	m_sUser = Variant(userCC, "");
	m_sName = Variant(nameCC, "");
	m_bCustomer = false;
	if (m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0))
		m_bCustomer = true;
	m_szFilename.Format("%s/%s/%s/codeinfo.dat", m_sRootDir, USRDIR, m_sName);

	m_envPath.Format("%s\\%s\\%s\\%s", m_sRootDir, USRDIR, m_sName, CATCHPATH);

	m_clrBk = GetIndexColor(CLRBG);
	m_clrBlink = GetIndexColor(CLRBLINK);
	m_clrRoundColor = GetIndexColor(clIndexPanel);
	m_strPal		= GetPalette();
	
	m_br.CreateSolidBrush(m_clrRoundColor);
	
	m_iGuideHeight = GUIDE_HEIGHT;
	m_iGridHeadrHeight = GRIDHEADER_HEIGHT;

	m_hGuideBitmap = getAxBitmap("알림_ic.bmp")->operator HBITMAP();
	m_hRoundBitmap[0] = getAxBitmap("axpanel1_lt.bmp")->operator HBITMAP();
	m_hRoundBitmap[1] = getAxBitmap("axpanel1_rt.bmp")->operator HBITMAP();
	m_hRoundBitmap[2] = getAxBitmap("axpanel1_lb.bmp")->operator HBITMAP();
	m_hRoundBitmap[3] = getAxBitmap("axpanel1_lb.bmp")->operator HBITMAP();

	m_guidecolor = m_pParent->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)GUIDCOLOR_INDEX);
	m_guideTxtcolor = m_pParent->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)GUIDTXTCOLOR_INDEX);

	loadJCode();

	loadFCode();
	loadOCode();

	loadEnvironment();

	initControl();
	loadCatch();

	InitMoveCtrl();
}

void CMapForm::OnDestroy() 
{
	destroy();
	if (m_pPopup)
		m_pPopup->DestroyWindow();
	CAxMapForm::OnDestroy();
}

HBRUSH CMapForm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	const int nID = pWnd->GetDlgCtrlID();
	pDC->SetBkMode(TRANSPARENT);
	
	switch (nID)
	{
	case IDC_STATIC1:	
		return (HBRUSH)getAxBrush(GetAxColor(181))->operator HBRUSH();
	case IDC_RDSAVE:
	case IDC_RDDEL:
		return m_br; 
	case IDC_TREE:
	case IDC_SEARCH:
	case IDC_RESULT:
		return hbr;
	}
	return m_br;
}

void CMapForm::ChangeColors() 
{
	m_clrBk = GetAxColor(BACKINDEXCOLOR);
	m_clrRoundColor = GetAxColor(TOOLINDEXCOLOR);


	m_br.CreateSolidBrush(m_clrRoundColor);
	m_strPal = GetPalette();

	GetDlgItem(IDC_STATIC1)->Invalidate();
	GetDlgItem(IDC_RDSAVE)->Invalidate();
	GetDlgItem(IDC_RDDEL)->Invalidate();

	Invalidate();
}

void CMapForm::OnPaint() 
{
#ifdef _DEBUG
	CPaintDC memdc(this); 

//	xxx::CMemDC memdc(&dc);
#else
	CPaintDC dc(this);

	xxx::CMemDC memdc(&dc);
#endif 
	CRect rcCtrl;
	CRect rcRound;
	CRect rcToolBar;
	memdc.SetBkMode(TRANSPARENT);
	m_ClientRect.InflateRect(50, 50);
	memdc.FillSolidRect(m_ClientRect, m_clrBk);

	DrawGuideBar(&memdc, m_ClientRect);

	// 1번째 Round
	m_tree.GetWindowRect(rcRound);
	ScreenToClient(rcRound);
	rcRound.InflateRect(ROUND_GAP, ROUND_GAP);
	memdc.FillSolidRect(rcRound, GetAxColor(181)); 
	DrawRoundBitmap(&memdc, rcRound, ROUND_PANEL);

	// 2번째 Round
	m_list.GetWindowRect(rcCtrl);
	ScreenToClient(rcCtrl);
	rcRound.left = rcCtrl.left;
	rcRound.top = rcCtrl.top;
	m_editSearch.GetWindowRect(rcCtrl);
	ScreenToClient(rcCtrl);
	rcRound.right = rcCtrl.right;
	rcRound.bottom = rcCtrl.bottom;
	rcRound.InflateRect(ROUND_GAP, ROUND_GAP);
	memdc.FillSolidRect(rcRound, GetAxColor(181)); 
	DrawRoundBitmap(&memdc, rcRound, ROUND_PANEL);

	// 3번째 Round
	GetDlgItem(IDC_POSCODEINFO)->GetWindowRect(rcCtrl);
	ScreenToClient(rcCtrl);
	rcRound.left = rcCtrl.left - 2;
	rcRound.top = rcCtrl.top;
	m_btnApply.GetWindowRect(rcCtrl);
	ScreenToClient(rcCtrl);
	rcRound.right = rcCtrl.right;
	rcRound.bottom = rcCtrl.bottom;
	rcRound.bottom += ROUND_GAP;
	rcRound.InflateRect(ROUND_GAP, ROUND_GAP);
	memdc.FillSolidRect(rcRound, GetAxColor(181)); 
	DrawRoundBitmap(&memdc, rcRound, ROUND_PANEL);

	rcToolBar = rcRound;
	rcToolBar.top = rcCtrl.top - 7;
	rcToolBar.bottom = rcCtrl.bottom + 5;
	rcToolBar.left += 5;
	rcToolBar.right -= 5;
	GetDlgItem(IDC_POSCATCH)->GetWindowRect(rcCtrl);
	ScreenToClient(rcCtrl);
	memdc.FillSolidRect(rcToolBar, m_clrRoundColor); 

	draw_groupBox(&memdc, IDC_HTS_STATIC);
}

void CMapForm::DrawGuideBar(CDC* pDC, CRect rc)
{
	CRect rcGuidBar;

	CBrush brush(m_guidecolor);
	CBrush* pOldBrush = pDC->SelectObject(&brush); 
	CPen pen(PS_SOLID, 1, m_panelcolor[0] );
	CPen* pOldPen = pDC->SelectObject(&pen); 

	CFont* pOldFont; 
	pOldFont = pDC->SelectObject(m_pFont); 
	
	GetDlgItem(IDC_RESULT)->GetWindowRect(rcGuidBar);
	GetDlgItem(IDC_RESULT)->ShowWindow(SW_HIDE);
	ScreenToClient(rcGuidBar);
	pDC->SetTextColor( m_guideTxtcolor);
	pDC->FillSolidRect(rcGuidBar, m_guidecolor); 
	DrawRoundBitmap(pDC, rcGuidBar, ROUND_PANEL);
	
	CRect alarmr = rcGuidBar;
	alarmr.left  = rcGuidBar.left + 5;
	alarmr.top  = rcGuidBar.top + 4;
	alarmr.right = alarmr.left + 14;
	alarmr.bottom = alarmr.top + 14;
	
	rcGuidBar.left = alarmr.right ;
	rcGuidBar.DeflateRect(3, 1, 1, 0);
	pDC->DrawText(m_strGuideMessage, rcGuidBar, DT_SINGLELINE|DT_LEFT|DT_VCENTER); 
	DrawBitmap(pDC, alarmr, "알림_IC.BMP" );
	
	pDC->SelectObject(pOldFont); 	
	pDC->SelectObject(pOldPen); 
	pDC->SelectObject(pOldBrush); 
	
	DeleteObject(pOldPen);
	DeleteObject(pOldBrush);
}

void CMapForm::DrawCornerRoundBitmap(CDC* pDC, CRect rc)
{
	const int iPoint = 5;
	
	DrawBitmap(pDC, CRect(rc.left, rc.top, rc.left+iPoint, rc.top +iPoint), "axpanel1_lt.bmp");
	DrawBitmap(pDC, CRect(rc.right-iPoint, rc.top, rc.right, rc.top +iPoint), "axpanel1_rt.bmp");
	DrawBitmap(pDC, CRect(rc.left, rc.bottom-iPoint, rc.left+iPoint, rc.bottom), "axpanel1_lb.bmp");
	DrawBitmap(pDC, CRect(rc.right-iPoint, rc.bottom-iPoint, rc.right, rc.bottom), "axpanel1_rb.bmp");
}

void CMapForm::OnSize(UINT nType, int cx, int cy) 
{
	CAxMapForm::OnSize(nType, cx, cy);

	GetClientRect(&m_ClientRect);
}

CString CMapForm::GetPalette()
{
	char buff[10];
	CString strPal;
	CString file;
	file.Format("%s\\%s\\%s", m_sRootDir, "tab", "palette.ini");
	
	GetPrivateProfileString("General", "Palette", "", buff, sizeof(buff), file);
	strPal = buff;
	
	return strPal;
}

void CMapForm::DrawRoundBitmap(CDC *pDC, CRect rc, int nKind)
{
	CRect bitRc;
	
	switch(nKind)
	{
	case ROUND_PANEL:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rt.bmp");
		
		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lb.bmp");
		
		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rb.bmp");
		break;
	case ROUND_CONTENTS:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rt.bmp");
		
		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lb.bmp");
		
		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rb.bmp");
		break;
	}
}

void CMapForm::DrawBitmap(CDC *pDC, CRect rc, CString strImage)
{
	CString strPath = Variant(homeCC, "");
	CString sImageDir = strPath + "\\image\\" + strImage;
	
	HBITMAP BitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), sImageDir, IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	CDC		memDC;
	
	memDC.CreateCompatibleDC(pDC);
	
	if (memDC.m_hDC != nullptr) 
		memDC.SelectObject(BitMap);
	
	if (memDC.m_hDC)
		::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);
	
	DeleteObject(BitMap);
	memDC.DeleteDC();
}

CString CMapForm::Variant(int comm, CString data)
{
	CString retvalue;
	char* dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);
	
	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";
	
	return retvalue;
}

void CMapForm::OnBtnsise() 
{
	querySise(m_saveCode, SC_SISE, false);
}

void CMapForm::OnBtnok() 
{
	saveEnvironment();
	saveCatch();
	CString string; string = "OnCatchChange\tok";
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR) string);

	m_pParent->PostMessage(WM_USER, closeDLL);
}

void CMapForm::OnBtncancel() 
{
	m_pParent->PostMessage(WM_USER, closeDLL);
}

void CMapForm::OnBtnapply() 
{
	saveEnvironment();
	saveCatch();
	CString string; string = "OnCatchChange\tok";
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR) string);
}

void CMapForm::OnBtnadd() 
{
	CString codeS = m_pSise->GetItemText(SISEROW, ISCOL_CODE);
	if (!codeS.IsEmpty())
		popupSetup(true, codeS);
}

void CMapForm::OnBtnadel() 
{
	if (!m_pCatch)	return;
	
	if (MessageBox("포착조건을 모두 삭제하시겠습니까?", "시세포착", MB_OKCANCEL|MB_ICONSTOP|MB_SYSTEMMODAL) == IDCANCEL)
		return;

	_vInfo.clear();
	m_mapCatch.RemoveAll();
	m_pCatch->Clear();
	m_pCatch->SetRowCount(1);

	setMessage("포착조건을 모두 삭제하였습니다.");
}

void CMapForm::OnBtndel() 
{
	if (!m_pCatch)	return;
	bool bIsDel = false;
	_codeinfo *codeinfo = nullptr;
	int minRow, maxRow, minCol, maxCol;
	m_pCatch->GetSelectedCellRange(minRow, maxRow, minCol, maxCol);
	for (int row = minRow; row <= maxRow; row++)
	{
		CString codeS = m_pCatch->GetItemText(row, ICCOL_CODE);
		if (m_mapCatch.Lookup(codeS, codeinfo))
		{
			m_mapCatch.RemoveKey(codeS);
			m_pCatch->DeleteRow(row);
			bIsDel = true;
			int gubn;
			setMessage("[%s : %s] 시세포착조건을 삭제하였습니다.", codeS, getCodeName(codeS, gubn));
			break;
		}
	}
	if (!bIsDel)
	{
		setMessage("삭제할 시세포착조건이 없습니다.");
	}
}

void CMapForm::OnBtnup() 
{
	if (!m_pCatch)	return;
	int minRow{}, maxRow{}, minCol{}, maxCol{};

	bool bIsUp = false;
	m_pCatch->GetSelectedCellRange(minRow, maxRow, minCol, maxCol);

	for (int row = minRow; row <= maxRow; row++)
	{
		CString codeS = m_pCatch->GetItemText(row, ICCOL_CODE);
		if (!codeS.IsEmpty())
		{
			popupSetup(false, codeS, row);	
			bIsUp = true;
			break;
		}
	}
	if (!bIsUp)
	{
		setMessageBox("수정할 시세포착조건을 선택하세요");
	}
}

void CMapForm::OnRdsave() 
{
}

void CMapForm::OnRddel() 
{
}


void CMapForm::OnBtncatchdel() 
{
	if (!m_pCatch)	return;
	CString codeS;
	_codeinfo *codeinfo = nullptr;
	for (int row = 1; row < m_pCatch->GetRowCount(); row++)
	{
		codeS = m_pCatch->GetItemText(row, ICCOL_CODE);
		if (!m_mapCatch.Lookup(codeS, codeinfo))
			continue;

//		if (codeinfo->capture[0] == '1')
			codeinfo->capture[0] = '0';

//		if (m_pCatch->GetItemText(row, ICCOL_CATCH) == "◎")
			m_pCatch->SetItemText(row, ICCOL_CATCH, "");
	
//		if (codeinfo->capture[0] == '1')
//			m_pCatch->SetItemText(row, ICCOL_JUMUN, "호출"); // 포착된  항목
//		else
			m_pCatch->SetItemText(row, ICCOL_JUMUN, "");
	}
}

void CMapForm::OnBtnhistory() 
{
	queryHistory();
}

void CMapForm::OnSetfocusSearch() 
{
	m_editSearch.SetSel(0, -1);
}

void CMapForm::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	selectList();
	*pResult = 0;
}

void CMapForm::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	selectList(SC_SISEDBCLICK);
 	*pResult = 0;
}

void CMapForm::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM item = m_tree.GetSelectedItem();
	if (!item)	return;

	const DWORD data = m_tree.GetItemData(item);
	if (data <= 0)	return;

	switch (data)
	{
	case xALLCODE:
		updateListCode(-1);
		break;
	case xKOSPI:
		updateListCode(jmKOSPI);
		break;
	case xKOSDAQ:
		updateListCode(jmKOSDAQ);
		break;
	case xETF:
		updateListCode(-1, jmETF);
		break;
	case xELW:
		updateListCode(-1, jmELW);
		m_list.ListSort(0);
		break;
	case xFUTURE:
		updateListFCode();
		break;
	case xCALL:
		updateListOCode(xCALL);
		break;
	case xPUT:
		updateListOCode(xPUT);
		break;
	case xINTEREST:
		break;
	case xTHEMA:
		break;
	case xGROUP:
		break;
	default:
		switch (data/1000)
		{
		case xINTEREST:
			updateListOther(data/1000, data%1000);
			break;
		case xTHEMA:
			updateListOther(data/1000, data%1000);
			break;
		case xGROUP:
			updateListOther(data/1000, data%1000);
			break;

		}
	}
	m_list.ListSort(0);
	*pResult = 0;
}
	

void CMapForm::alertx(gsl::span<DWORD> data)
{
	CMapStringToString	rtmStore;

	if (data[23])
	{
		rtmStore.SetAt("023", CString((char*)data[23]).Trim());
		rtmStore.SetAt("024", CString((char*)data[24]).Trim());
		rtmStore.SetAt("025", CString((char*)data[25]).Trim());
		rtmStore.SetAt("026", CString((char*)data[26]).Trim());
		rtmStore.SetAt("027", CString((char*)data[27]).Trim());
		rtmStore.SetAt("033", CString((char*)data[33]).Trim());

		CString tmp;
		if (!rtmStore.Lookup("024", tmp)) return;

		alertSise(rtmStore, "023", ISCOL_CURR);	// 현재가
		alertSise(rtmStore, "024", ISCOL_DIFF);	// 대비
		alertSise(rtmStore, "025", ISCOL_MDGA);	// 매도호가
		alertSise(rtmStore, "026", ISCOL_MSGA);	// 매수호가
		alertSise(rtmStore, "027", ISCOL_GVOL);	// 거래량
		alertSise(rtmStore, "033", ISCOL_RATE);	// 등락률

		if (m_pSetupDlg)	m_pSetupDlg->Alert(rtmStore);
		rtmStore.RemoveAll();
	}
}


//=================================================================================================
void CMapForm::alert(char* buf, int len)
{
	CString dataS = CString(buf);
	//TRACE("dataS: "+dataS+"\n");
	dataS.TrimRight(); dataS.TrimLeft();
	CString sCode = Parser(dataS, "\t"), sSym, sData;

	if (m_rtsCode != sCode)
		return;
	
	CMapStringToString	rtmStore;
	int	idx = 0;
	
	rtmStore.RemoveAll();
	for (; !dataS.IsEmpty(); )
	{
		idx = dataS.Find('\t');
		if (idx < 0)
			break;
		sSym = dataS.Left(idx);
		
		dataS = dataS.Mid(idx+1);
		idx = dataS.Find('\t');
		if (idx < 0)
			idx = dataS.GetLength();
		sData = dataS.Left(idx);
		rtmStore.SetAt(sSym, sData);

		if (dataS.GetLength() <= idx)
			break;
		if (sSym == "024") break;
		dataS = dataS.Mid(idx+1);
	}
	CString tmp; //asdfasdf
	if (!rtmStore.Lookup("024",tmp)) return;
	alertSise(rtmStore, "023", ISCOL_CURR);	// 현재가
	alertSise(rtmStore, "024", ISCOL_DIFF);	// 대비
	alertSise(rtmStore, "025", ISCOL_MDGA);	// 매도호가
	alertSise(rtmStore, "026", ISCOL_MSGA);	// 매수호가
	alertSise(rtmStore, "027", ISCOL_GVOL);	// 거래량
	alertSise(rtmStore, "033", ISCOL_RATE);	// 등락률
	
	if (m_pSetupDlg)	m_pSetupDlg->Alert(rtmStore);
	rtmStore.RemoveAll();
}

void CMapForm::alertSise(const CMapStringToString &rtmStore, CString symS, int col)
{
	CString dataS;
	int fkind = 0;
	if (rtmStore.Lookup(symS, dataS) && m_pSise->GetItemText(SISEROW, col) != dataS)
	{
		fkind = getFKind(col);
		m_pSise->SetItemText(SISEROW, col, _GetFormatData(fkind, dataS));
		m_pSise->Blink(SISEROW, col, m_clrBlink);
	}
}

void CMapForm::domino(CString tmpS)
{
	if (tmpS.IsEmpty())
		return;
	CString symS = Parser(tmpS, "\t");
	tmpS.TrimLeft();
	if (symS.Find("InterView") != -1)
	{
		if (atoi(tmpS) == 0)
		{
			m_btnInter.ShowWindow(SW_HIDE);
			m_btnInter.EnableWindow(FALSE);
		}
	}
}

void CMapForm::trigger(CString tmpS)
{
	if (tmpS.IsEmpty())
		return;
	CString symS = Parser(tmpS, "\t");
	tmpS.TrimLeft();
	if (symS.Find("OnPortfolio") != -1 && tmpS == "ok")
	{
		initTree();
		loadCatch();
		return;
	}
	if (symS.Find("AlarmSetup") != -1)
	{
		if (!m_pCatch)	return;
		CString codeS = tmpS;
		if (!codeS.IsEmpty() && (codeS.GetAt(0) == 'A' || codeS.GetAt(0) == 'J'))
			codeS = codeS.Mid(1);
		for (int row = 1; row < m_pCatch->GetRowCount(); row++)
		{
			if (codeS == m_pCatch->GetItemText(row, ICCOL_CODE))
				m_pCatch->SetItemText(row, ICCOL_CATCH, "◎");
		}
	}
	
}

void CMapForm::initControl()
{
	m_pFont = getAxFont(m_sFontName, m_nFontPoint);
	if (m_pFont)	m_pFont->GetLogFont(&m_lf);
	m_lf.lfHeight = m_nFontPoint * 10;

	m_pBFont = getAxFont(m_sFontName, m_nFontPoint, false, FW_BOLD);
	if (m_pBFont)	m_pBFont->GetLogFont(&m_blf);
	m_blf.lfHeight = m_nFontPoint * 10;
	
	// image button
	setImgBtn(&m_btnUp, 4, TRUE);
	setImgBtn(&m_btnDel, 4, TRUE);
	setImgBtn(&m_btnADel, 4, TRUE);
	setImgBtn(&m_btnAdd, 4, TRUE);
	setImgBtn(&m_btnInter, 4, TRUE);
	setImgBtn(&m_btnHistory, 4, TRUE);
	setImgBtn(&m_btnCatchDel, 7, TRUE);
	
	setImgBtn(&m_btnOk, 4, TRUE);
	setImgBtn(&m_btnApply, 4, TRUE);
	setImgBtn(&m_btnCancel, 4, TRUE);
	setImgBtn(&m_btnSise, 4, TRUE);
#ifndef	_DEBUG
	m_btnApply.ShowWindow(SW_HIDE);
	m_btnApply.EnableWindow(FALSE);

	//m_btnHistory.ShowWindow(SW_HIDE);
	//m_btnHistory.EnableWindow(FALSE);
#endif

//	m_result.setColor(GetIndexColor(78), GetIndexColor(69), GetIndexColor(77));

	initList();
	initTree();
	initGrid();

	if (!m_pPopup)
	{
		m_pPopup = std::make_unique<CCatchWnd>(this, m_pParent);
		m_pPopup->SetFont(m_pFont, m_lf);
		m_pPopup->SetBFont(m_pBFont, m_blf);
		m_pPopup->CreatePopup();
	}

	DWORD	dwConversion{}, dwSentence{};
	HIMC hImc = ImmGetContext(this->m_hWnd);
	if (ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
	{
		dwConversion |= IME_CMODE_HANGEUL; 
		ImmSetConversionStatus(hImc, dwConversion, dwSentence);
	}

	m_editSearch.SetParent(this);
	//((CButton *)GetDlgItem(IDC_RDSAVE))->SetCheck(TRUE);

}

void CMapForm::InitMoveCtrl()
{
	MoveCtrl(IDC_TREE,	  ROUND_GAP+1, ROUND_GAP, 0);
	MoveCtrl(IDC_LIST,	  ROUND_GAP*3, ROUND_GAP, 0);
	MoveCtrl(IDC_STATIC1, ROUND_GAP*3, ROUND_GAP, 0);
	MoveCtrl(IDC_SEARCH,  ROUND_GAP*3, ROUND_GAP, 0);

	MoveCtrl(IDC_BTNADD,	0, ROUND_GAP, 0);
	MoveCtrl(IDC_BTNUP,		0, ROUND_GAP, 0);
	MoveCtrl(IDC_BTNDEL,	0, ROUND_GAP, 0);
	MoveCtrl(IDC_BTNADEL,	0, ROUND_GAP, 0);
	MoveCtrl(IDC_BTNCATCHDEL, 0, ROUND_GAP, 0);
	MoveCtrl(IDC_BTNHISTORY, 0, ROUND_GAP, 0);
	MoveCtrl(IDC_BTNINTER,	0, ROUND_GAP, 0);
	MoveCtrl(IDC_STATIC,	0, ROUND_GAP, 0);
	MoveCtrl(IDC_RDSAVE,	0, ROUND_GAP, 0);
	MoveCtrl(IDC_RDDEL,		0, ROUND_GAP, 0);
	MoveCtrl(IDC_BTNOK,		0, ROUND_GAP, 0);
	MoveCtrl(IDC_BTNCANCEL, 0, ROUND_GAP, 0);
	MoveCtrl(IDC_BTNAPPLY,	0, ROUND_GAP, 0);
}

void CMapForm::initGrid()
{
	// 시세grid
	CRect rc;
	GetDlgItem(IDC_POSCODEINFO)->GetWindowRect(rc);
	ScreenToClient(rc);
	m_pSise = std::make_unique<CExGrid>();
	m_pSise->Create(rc, this, IDC_GRSISE, GVSC_NONE, 0, WS_CHILD|WS_VISIBLE);
	initGrid(m_pSise.get(), IDC_GRSISE);

	// catch grid
	GetDlgItem(IDC_POSCATCH)->GetWindowRect(rc);
	ScreenToClient(rc);
	m_pCatch = std::make_unique <CExGrid>();
	m_pCatch->Create(rc, this, IDC_GRCATCH, GVSC_BOTH, 0, WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL);
	initGrid(m_pCatch.get(), IDC_GRCATCH);
}

void CMapForm::initGrid(CExGrid* pGrid, UINT idc)
{
	_GVITEM gvitem;
	COLORREF gridHead{};
	CFont* pFont = nullptr;
	int ii = 0;
	int colCnt = 0;
	
	gridHead = GetIndexColor(CLRGRHB);

	gvitem.row = 0;
	gvitem.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_FGCOLOR|GVMK_BKCOLOR;
	gvitem.mask |= GVMK_ATTR|GVMK_MISC|GVMK_PARAM;
	
	gvitem.font = m_lf;
	gvitem.format = GVFM_HEADER;
	gvitem.fgcol = GetIndexColor(CLRGRHF);
	gvitem.bkcol = gridHead;

	switch (idc)
	{
	case IDC_GRSISE:
		pGrid->SetGridFocusLine(FALSE, FALSE);
		colCnt = sizeof(grInfoSise)/sizeof(_grInfo);
		pGrid->Initial(1, colCnt, 1, 0);
		for (ii = 0; ii < colCnt; ii++)
		{
			gvitem.symbol = grInfoSise[ii].symbols;
			gvitem.text = grInfoSise[ii].heads;
			gvitem.attr = 0;
			gvitem.col = ii;
			pGrid->SetItem(&gvitem);
			pGrid->SetColumnWidth(ii, grInfoSise[ii].width);
		}
		
		insertGrid(pGrid, idc, SISEROW);
		break;

	case IDC_GRCATCH:
		pGrid->SetGridFocusLine(TRUE, TRUE);
		colCnt = sizeof(grInfoCatch)/sizeof(_grInfo);
		pGrid->Initial(1, colCnt, 1, 0);
		for (ii = 0; ii < colCnt; ii++)
		{
			gvitem.symbol = grInfoCatch[ii].symbols;
			gvitem.text = grInfoCatch[ii].heads;
			gvitem.attr = grInfoCatch[ii].attr;
			gvitem.attr &= ~GVAT_COMMA;
			gvitem.col = ii;
			pGrid->SetItem(&gvitem);
			pGrid->SetColumnWidth(ii, grInfoCatch[ii].width);
		}
		break;
	}
	
	pGrid->SetRowHeight(0, GRIDROW_HEIGHT);
	pGrid->SetRowColResize(FALSE, FALSE);
	pGrid->SetBKSelColor(GetIndexColor(CLRGRSEL));
	pGrid->SetGridColor(GetIndexColor(CLRGRLINE));
	pGrid->SetBkColor(GetIndexColor(CLRGRROWB1));
	pGrid->Adjust();
}

int CMapForm::insertGrid(CExGrid* pGrid, UINT idc, int row)
{
	_GVITEM gvitem;
	CString	 tmpS, statS;

	gvitem.font   = m_lf;
	gvitem.mask   = GVMK_FONT|GVMK_TEXT|GVMK_FORMAT|GVMK_PARAM|GVMK_ATTR|GVMK_BKCOLOR|GVMK_FGCOLOR;
	gvitem.bkcol = GetIndexColor(CLRGRROWB1);
	gvitem.fgcol  = GetIndexColor(CLRGRROWF);

	row = pGrid->InsertRow(" ", row);
	pGrid->SetRowHeight(row, GRIDROW_HEIGHT);
	gvitem.row    = row;

	int col = 0;
	switch (idc)
	{
	case IDC_GRSISE:
		for (col = 0; col < sizeof(grInfoSise)/sizeof(_grInfo); col++)
		{
			gvitem.text = "";
			gvitem.col = col;
			gvitem.format = grInfoSise[col].format;
			gvitem.attr = grInfoSise[col].attr;
			pGrid->SetItem(&gvitem);
		}
		break;
	case IDC_GRCATCH:
		for (col = 0; col < sizeof(grInfoCatch)/sizeof(_grInfo); col++)
		{
			gvitem.text = "";
			gvitem.col = col;
			gvitem.format = grInfoCatch[col].format;
			gvitem.attr = grInfoCatch[col].attr;
			gvitem.attr |= GVAT_ONLYROW;
			pGrid->SetItem(&gvitem);
		}
		break;
	}
	return row;
}


void CMapForm::initList()
{
	CRect rc;
	m_list.GetClientRect(rc);
	addColumn(&m_list, "코드", 0, 56);
	addColumn(&m_list, "이름", 1, rc.Width() - 55 -19);
	m_list.SetGridLines();
	m_list.SetFullRowSelect();
	
}

BOOL CMapForm::addColumn(CListCtrl *ctrl, LPCTSTR string, int item, int width)
{
	LV_COLUMN lvc;

	lvc.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt     = LVCFMT_LEFT;
	lvc.pszText = (LPTSTR) string;
	lvc.cx      = width;	//ctrl->GetStringWidth(lvc.pszText) + 

	return ctrl->InsertColumn(item, &lvc);
}

int CMapForm::addItem(CListCtrl *ctrl, LPCSTR string, int nItem, int nSubItem)
{
	if (nSubItem == 0) // code
		return ctrl->InsertItem(nItem, string);
	else		  // name
		return ctrl->SetItemText(nItem, nSubItem, string);
}

void CMapForm::initTree()
{
	m_tree.DeleteAllItems();
	
	bool bIsInter = false;
	HTREEITEM hRoot;

	m_tree.SetBkColor(GetAxColor(181));

	// 관심종목그룹 Add
	[[gsl::suppress(6387)]]
	hRoot = m_tree.InsertItem(TVIF_TEXT, _T("관심그룹"), 0, 0, 0, 0, 0, nullptr, nullptr);
	_hInter = hRoot;
	//setSubTree(hRoot, xINTEREST);

	if (m_tree.ItemHasChildren(hRoot))
	{
		bIsInter = true;
		m_tree.Expand(hRoot, TVE_EXPAND);
		HTREEITEM item = m_tree.GetChildItem(hRoot);
		m_tree.SelectItem(item);
	}

	hRoot = m_tree.InsertItem(TVIF_TEXT, _T("전종목"), 0, 0, 0, 0, 0, nullptr, nullptr);
	m_tree.SetItemData(hRoot, xALLCODE);
	if (!bIsInter)
	{
		m_tree.SelectItem(hRoot);
	}
	m_tree.SetItemData(m_tree.InsertItem(TVIF_TEXT, _T("거래소"), 0, 0, 0, 0, 0, nullptr, nullptr), xKOSPI);
	m_tree.SetItemData(m_tree.InsertItem(TVIF_TEXT, _T("코스닥"), 0, 0, 0, 0, 0, nullptr, nullptr), xKOSDAQ);
	m_tree.SetItemData(m_tree.InsertItem(TVIF_TEXT, _T("ETF"), 0, 0, 0, 0, 0, nullptr, nullptr), xETF);
	m_tree.SetItemData(m_tree.InsertItem(TVIF_TEXT, _T("ELW"), 0, 0, 0, 0, 0, nullptr, nullptr), xELW);
	
	hRoot = m_tree.InsertItem(TVIF_TEXT, _T("섹터종목"), 0, 0, 0, 0, 0, nullptr, nullptr);
	setSubTree(hRoot, xTHEMA);

	// 그룹사별
	hRoot = m_tree.InsertItem(TVIF_TEXT, _T("그룹사별"), 0, 0, 0, 0, 0, nullptr, nullptr);
	setSubTree(hRoot, xGROUP);

	m_tree.SetItemData(m_tree.InsertItem(TVIF_TEXT, _T("선물"), 0, 0, 0, 0, 0, nullptr, nullptr), xFUTURE);
	m_tree.SetItemData(m_tree.InsertItem(TVIF_TEXT, _T("콜옵션"), 0, 0, 0, 0, 0, nullptr, nullptr), xCALL);
	m_tree.SetItemData(m_tree.InsertItem(TVIF_TEXT, _T("풋옵션"), 0, 0, 0, 0, 0, nullptr, nullptr), xPUT);	
}

void CMapForm::setSubTree(HTREEITEM hItem, int gubn)
{
	int readL = 0; char readB[256]{};
	CString	filePath, string;
	CString	stringx, data, skey;

	switch (gubn)
	{
	case xINTEREST:
		{
			//CString	gno, gname;
			//filePath.Format("%s\\%s\\%s\\%s", m_sRootDir, USRDIR, m_sName, FILE_SAVE);

			//readL = GetPrivateProfileString(_T("GROUPORDER"), "00", "", readB, sizeof(readB), filePath);
			//if (readL <= 0)	return;

			//string = CString(readB, readL);

			//while (!string.IsEmpty())
			//{
			//	gno = Parser(string, ";");
			//	if (gno.IsEmpty() || gno.GetLength() != 2)
			//		break;

			//	readL = GetPrivateProfileString(_T("GROUPNAME"), gno, "", readB, sizeof(readB), filePath);
			//	gname = CString(readB, readL); 
			//	data.Format("%02d%03d", xINTEREST, atoi(gno));
			//	if (isExistFile(atoi(gno)))
			//		m_tree.SetItemData(m_tree.InsertItem(gname, hItem), atoi(data));
			//}
		}
		break;
	case xTHEMA:
		{
			filePath.Format("%s/%s/%s", m_sRootDir, TABDIR, "themcd.ini");
			for (int ii = 0; ii < 200; ii++)
			{
				skey.Format("%02d", ii);
				readL = GetPrivateProfileString(_T("FOLDER"), skey, "", readB, sizeof(readB), filePath);
				if (readL <= 0) break;

				string = CString(readB, readL); 
				stringx = Parser(string, ";"); 
				stringx.TrimRight();
				data.Format("%02d%03d", xTHEMA, atoi(string));
				m_tree.SetItemData(m_tree.InsertItem(stringx, hItem), atoi(data));
			}
		}
		break;

	case xGROUP:
		{
			filePath.Format("%s/%s/%s", m_sRootDir, TABDIR, "gropcd.ini");
			for (int ii = 0; ii < 200; ii++)
			{
				skey.Format("%02d", ii);
				readL = GetPrivateProfileString(_T("FOLDER"), skey, "", readB, sizeof(readB), filePath);
				if (readL <= 0) break;

				string = CString(readB, readL);	
				stringx = Parser(string, ";"); 
				stringx.TrimRight();
				data.Format("%02d%03d", xGROUP, atoi(string));
				m_tree.SetItemData(m_tree.InsertItem(stringx, hItem), atoi(data));
			}
		}
		break;
	}
}

bool CMapForm::isExistFile(int gno)
{
	CString	filePath;
	filePath.Format("%s/%s/%s/portfolio.i%02d",  m_sRootDir, USRDIR, m_sName, gno);

	CFileFind finder;
	if (!finder.FindFile(filePath))
		return false;

	return true;
}

void CMapForm::setImgBtn(CImgBtn *pButton, int nLength, BOOL bImageFit)
{
	CString sImgDir = "";
	sImgDir.Format("%s%dbtn", m_sImgDir, nLength);

	pButton->m_hBitmap    = getAxBitmap(sImgDir + ".bmp")->operator HBITMAP();
	pButton->m_hBitmap_dn = getAxBitmap(sImgDir + "_dn.bmp")->operator HBITMAP();
	pButton->m_hBitmap_hv = getAxBitmap(sImgDir + "_en.bmp")->operator HBITMAP();
	if (bImageFit)
		pButton->FitImageSize();
}

void CMapForm::loadEnvironment()
{
	const BOOL bSave = (BOOL)GetPrivateProfileInt(SECNAME, SAVEOPTION, VALOPTION, m_envPath);	// 주문확인
	if (bSave == 0)		// 저장
	{
		((CButton *)GetDlgItem(IDC_RDSAVE))->SetCheck(TRUE);
		((CButton *)GetDlgItem(IDC_RDDEL))->SetCheck(FALSE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_RDSAVE))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RDDEL))->SetCheck(TRUE);
	}
	
}
void CMapForm::saveEnvironment()
{
	// 수량값 설정
	CString tmpS;
	if (((CButton *)GetDlgItem(IDC_RDSAVE))->GetCheck())
		tmpS = "0";
	else
		tmpS = "1";

	WritePrivateProfileString(SECNAME, SAVEOPTION, tmpS, m_envPath);

}

bool CMapForm::loadJCode()
{
	struct	_code	code;
	DllInvoker dll("AxisCode");
	if (!dll.IsLoaded())
		return false;
	auto map = (std::map<CString, struct hjcodex*>*)dll.Function<char* WINAPI(int)>("getArray")(1);
	if (map->size() <= 0)
		return false;

	for_each(map->begin(), map->end(), [&](const auto item) {
		code.codeS = CString(item.second->code, HCodeLen).Mid(1).Trim();
		code.nameS = CString(item.second->hnam, HNameLen).Mid(1).Trim();
		code.kosd = item.second->kosd;
		code.ssgb = item.second->ssgb;
		m_arCode.Add(code);
	});
	return true;
}

bool CMapForm::loadFCode()
{
	if (m_arFCode.GetSize() > 0) return false;

	CFile	fileH;
	CString	filePath;
	struct  fjcode  fjcode;
	struct	_fcode	fcode;
	
	filePath.Format("%s\\%s", m_sTabDir, FJCODE);
  	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox("선물종목코드가 없습니다.");
		return false;
	}

	const int cnt = gsl::narrow_cast<int>(fileH.GetLength() / sizeof(struct fjcode));
	for (int ii = 0; ii < cnt; ii++)
	{
		fileH.Read(&fjcode, sizeof(struct fjcode));

		fcode.codeS = _GetString(fjcode.cod2, FCodeLen);
		if (fcode.codeS.GetLength() == 8)
		{
			if (fcode.codeS.GetAt(0) != '1' && fcode.codeS.GetAt(0) != 'A') //파생상품 코드개편
				continue;
			if (fcode.codeS.GetAt(2) != '1')
				continue;
		}
		fcode.nameS = _GetString(fjcode.hnam, FNameLen);
		m_arFCode.Add(fcode);
      	}
	fileH.Close();
	return true;
}

bool CMapForm::loadOCode()
{
	if (m_arOCode.GetSize() > 0) return false;

	CFile	fileH;
	CString	filePath;
	struct  ojcodh  OJCodh;
	struct  ojcode  OJCode;
	
	filePath.Format("%s\\%s", m_sTabDir, OJCODE);
  	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox("옵션종목코드가 없습니다.");
		return false;
	}

	const int len = fileH.Read(&OJCodh, sizeof(struct ojcodh));
	for (int ii = 0; ii < 4; ii++)
	{
		m_arOHead.Add(_GetString(OJCodh.pjym[ii], sizeof(OJCodh.pjym[ii])));
	}
	const int codeN = gsl::narrow_cast<int>((fileH.GetLength() - len) / sizeof(struct ojcode));
	for (int ii = 0; ii < codeN; ii++)
	{
		fileH.Read(&OJCode, sizeof(struct ojcode));
		m_arOCode.Add(OJCode);
	}
	fileH.Close();
	return true;
}


bool CMapForm::loadCatch()
{
	if (!m_pCatch)	return false;

	m_pCatch->Clear();
	m_pCatch->SetRowCount(1);
	
	deleteAllMapCatch();
	
	CFileFind finder;
	if (!finder.FindFile(m_szFilename))
		return false;

	UINT	readL{};
	CString	codeS;
	CFile	fileH(m_szFilename, CFile::modeRead);
	for (int ii = 0; ii < maxJONGMOK; ii++)
	{
		auto &codeinfo = _vInfo.emplace_back(std::make_unique<_codeinfo>());
		readL = fileH.Read(codeinfo.get(), sz_codeinfo);
		if (readL < sz_codeinfo)
		{
			_vInfo.erase((_vInfo.rbegin()+ 1).base());
			break;
		}
		
		codeS = getString(codeinfo->code, sizeof(codeinfo->code));
		m_mapCatch.SetAt(codeS, codeinfo.get());
		m_arCatch.Add(codeS);
	}
	fileH.Close();

	updateCatch();
	return true;
}

void CMapForm::saveCatch()
{
	CFile	fileH(m_szFilename, CFile::modeWrite|CFile::modeCreate);
	if (fileH.m_hFile == CFile::hFileNull) return;

	_codeinfo *codeinfo = nullptr;
	CString codeS;
	for (int row = 1; row < m_pCatch->GetRowCount(); row++)
	{
		codeS = m_pCatch->GetItemText(row, ICCOL_CODE);
		if (!m_mapCatch.Lookup(codeS, codeinfo))
			continue;
		fileH.Write(codeinfo, sz_codeinfo);
	}
	fileH.Close();
}

void CMapForm::updateListFCode()
{
	CWaitCursor wait;
	m_list.DeleteAllItems();

	int row = 0;
	_fcode   fcode; 
	for (int ii = 0; ii < m_arFCode.GetSize(); ii++)
	{
		fcode = m_arFCode.GetAt(ii);
		addItem(&m_list, fcode.codeS, row, 0);
		addItem(&m_list, fcode.nameS, row, 1);
		row++;	
	}
	m_bNum = true;	
	AfxGetApp()->RestoreWaitCursor();
}

void CMapForm::updateListOCode(int type)
{
	CWaitCursor wait;
	m_list.DeleteAllItems();

	int row = 0;
	int ii = 0, jj = 0;
	struct  ojcode  OJCode;
	if (type == xCALL)
	{
		for (jj = 3; jj >= 0; jj--)
		{
			for (ii = 0; ii < m_arOCode.GetSize(); ii++)
			{
				OJCode = m_arOCode.GetAt(ii);
				if (OJCode.call[jj].yorn != '1')	continue;
				addItem(&m_list, _GetString(OJCode.call[jj].cod2, sizeof(OJCode.call[jj].cod2)), row, 0);
				addItem(&m_list, _GetString(OJCode.call[jj].hnam, sizeof(OJCode.call[jj].hnam)), row, 1);
				row++;	
			}
		}
	}
	else
	{
		for (jj = 0; jj < 4; jj++)
		{
			for (ii = 0; ii < m_arOCode.GetSize(); ii++)
			{
				OJCode = m_arOCode.GetAt(ii);
			
				if (OJCode.put[jj].yorn != '1')	continue;
				addItem(&m_list, _GetString(OJCode.put[jj].cod2, sizeof(OJCode.put[jj].cod2)), row, 0);
				addItem(&m_list, _GetString(OJCode.put[jj].hnam, sizeof(OJCode.put[jj].hnam)), row, 1);
				row++;	
			}
		}
	}

	m_bNum = true;	
	AfxGetApp()->RestoreWaitCursor();
}

void CMapForm::updateListCode(int kosd, int ssgb /*= -1*/)
{
	CWaitCursor wait;
	m_list.DeleteAllItems();
	struct	_code	code;

	for (int ii = 0, row = 0; ii < m_arCode.GetSize(); ii++)
	{
		code = m_arCode.GetAt(ii);

		switch (kosd)
		{
		case -1:	// 전체
			switch (ssgb)
			{
			case jmETF:
			case jmELW:
				if ((char)ssgb != code.ssgb)
					continue;
				break;
			default:
				if (code.ssgb == jmELW)	// 전종목에 ELW 보이지 않도록ㄹ
					continue;
				break;
			}
			break;
		case jmKOSPI:
		case jmKOSDAQ:
			if ((char)kosd != code.kosd)
				continue;
			else	//2016.12.16 KSJ 코스피 코스닥 종목중에 ETF, ELW 나오지 않도록
			{
				switch (code.ssgb)
				{
				case jmETF:
				case jmELW:
						continue;
					break;
				default:
					break;
				}
			}
			break;
		}
	
		addItem(&m_list, code.codeS, row, 0);
		addItem(&m_list, code.nameS, row, 1);
		row++;
	}
	m_bNum = true;
	AfxGetApp()->RestoreWaitCursor();
}


void CMapForm::updateListOther(int gubn, int sub)
{
	CWaitCursor wait;
	m_list.DeleteAllItems();

	CString	filePath, string = _T(""), codeS, nameS;
	int row = 0;
	int codeGubn{};
	switch (gubn)
	{
	case xINTEREST:
		{
			Request_GroupCode(sub);
			//if (!isExistFile(sub))
			//	return;
			//filePath.Format("%s\\%s\\%s\\portfolio.i%02d", m_sRootDir, USRDIR, m_sName, sub);

			//UINT	readL{};
			//struct	_inters inters;

			//CFile	fileH(filePath, CFile::modeRead);
			//if (fileH.m_hFile == CFile::hFileNull)
			//	return;

			//for (int ii = 0; ii < maxJONGMOK; ii++)
			//{
			//	readL = fileH.Read(&inters, sz_inters);
			//	if (readL < sz_inters)
			//		break;
			//	codeS = getString(inters.code, sizeof(inters.code));
			//	nameS = getCodeName(codeS, codeGubn);
			//	if (nameS.IsEmpty())
			//		continue;
			//	addItem(&m_list, codeS, row, 0);
			//	addItem(&m_list, nameS, row, 1);
			//	row++;
			//	
			//}
			//fileH.Close();
		}
		break;
	case xTHEMA:
		{
			char	readB[512];
			UINT	readL;
			CString	skey;
			skey.Format("%03d", sub);
			filePath.Format("%s/%s/%s", m_sRootDir, TABDIR, "themcd.ini");
			readL = GetPrivateProfileString(_T("TMCODE"), skey, "", readB, sizeof(readB), filePath);
			if (readL > 0) 	string = CString(readB, readL);

			while (!string.IsEmpty())
			{
				codeS = Parser(string, ";");
				nameS = getCodeName(codeS, codeGubn);
				if (nameS.IsEmpty())
					continue;
				addItem(&m_list, codeS, row, 0);
				addItem(&m_list, nameS, row, 1);
				row++;
			}
		}
		break;
	case xGROUP:
		{
			char	readB[512];
			UINT	readL;
			CString	skey;
			skey.Format("%03d", sub);
			filePath.Format("%s/%s/%s", m_sRootDir, TABDIR, "gropcd.ini");
			readL = GetPrivateProfileString(_T("GRCODE"), skey, "", readB, sizeof(readB), filePath);
			if (readL > 0) 	string = CString(readB, readL);

			while (!string.IsEmpty())
			{
				codeS = Parser(string, ";");
				nameS = getCodeName(codeS, codeGubn);
				if (nameS.IsEmpty())
					continue;
				addItem(&m_list, codeS, row, 0);
				addItem(&m_list, nameS, row, 1);
				row++;
			}
		}
		break;
	}
	m_bNum = true;
	AfxGetApp()->RestoreWaitCursor();

}

COLORREF CMapForm::GetAxColor(int iColor)
{
	if (iColor & 0x02000000)
		return iColor;
	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)iColor);
}

CString CMapForm::getCodeName(CString codeS, int& gubn)
{
	gubn = CG_NOT;
	const int codeL = codeS.GetLength();
	int ii = 0, jj = 0;
	if (codeL == 6)					// kospi code
	{
		_code   code; 
		for (ii = 0; ii < m_arCode.GetSize(); ii++)
		{
			code = m_arCode.GetAt(ii);
			if (codeS != code.codeS)
				continue;
			if (code.kosd == jmKOSPI)
				gubn = CG_KOSPI;
			else if (code.kosd == jmKOSDAQ)
				gubn = CG_KOSDAQ;
			return code.nameS;
		}
	}
	if (codeL == 8)					// 선물,옵션
	{
		switch (codeS.GetAt(0))
		{
		case '1':		// 선물
		case 'A':       //파생상품 코드개편
			{
				_fcode   fcode; 
				for (ii = 0; ii < m_arFCode.GetSize(); ii++)
				{
					fcode = m_arFCode.GetAt(ii);
					if (fcode.codeS == codeS)
					{
						gubn = CG_FUTURE;
						return fcode.nameS;
					}
				}
			}
			break;
		case '2':		// 콜옵션
		case 'B':
			{
				struct  ojcode  OJCode;
				for (ii = 0; ii < m_arOCode.GetSize(); ii++)
				{
					OJCode = m_arOCode.GetAt(ii);
					for (jj = 0; jj < 4; jj++)
					{
						if (OJCode.call[jj].yorn != '1')	continue;
						if (_GetString(OJCode.call[jj].cod2, sizeof(OJCode.call[jj].cod2)) == codeS)
						{
							gubn = CG_CALL;
							return _GetString(OJCode.call[jj].hnam, sizeof(OJCode.call[jj].hnam));
						}
					}
				}
			}
			break;
		case '3':		// 풋옵션
		case 'C':
			{
				struct  ojcode  OJCode;
				for (ii = 0; ii < m_arOCode.GetSize(); ii++)
				{
					OJCode = m_arOCode.GetAt(ii);
					for (jj = 0; jj < 4; jj++)
					{
						if (OJCode.put[jj].yorn != '1')	continue;
						if (_GetString(OJCode.put[jj].cod2, sizeof(OJCode.put[jj].cod2)) == codeS)
						{
							gubn = CG_PUT;
							return _GetString(OJCode.put[jj].hnam, sizeof(OJCode.put[jj].hnam));
						}
					}
				}
			}
			break;
		}
	}
	
	return _T("");
}

void CMapForm::querySise(CString codeS, int key /*= SC_SISE */, bool bCheck /*= true*/)
{
	if (codeS.IsEmpty())	return;

	if (bCheck && m_saveCode == codeS && key == SC_SISE)	return;
	m_saveCode = codeS;

	CString sendS, searchSymS, tmpS, sRtsCode = "";
	
	// 시세조회
	int gubun;
	getCodeName(codeS, gubun);
	
	switch (gubun)
	{
	case CG_KOSPI :				// Stock
	case CG_KOSDAQ :		
		{
			sRtsCode = "1301";
			for (int ii = 0; ii < sizeof(grInfoSise)/sizeof(_grInfo); ii++)
			{
				tmpS = CString(grInfoSise[ii].symbols);
				searchSymS += tmpS + "\t";
			}
			m_IdCurKind = xALLCODE;
		}
		break;
	case CG_FUTURE :				// Future
		{
			sRtsCode = "30301";	
			for (int ii = 0; ii < sizeof(grInfoFuture)/sizeof(_grInfo); ii++)
			{
				tmpS = CString(grInfoFuture[ii].symbols);
				searchSymS += tmpS + "\t";
			}
			m_IdCurKind = xFUTURE;
		}
		break;
	case CG_CALL :
	case CG_PUT :				// Option
		{
			sRtsCode = "40301";
			for (int ii = 0; ii < sizeof(grInfoOption)/sizeof(_grInfo); ii++)
			{
				tmpS = CString(grInfoOption[ii].symbols);
				searchSymS += tmpS + "\t";
			}
			m_IdCurKind = xCALL;
		}
		break;
	}
	

	m_rtsCode = "";
	sendS.Format("%s%c%s\t%s", sRtsCode, 0x7f, codeS, searchSymS);
	SendTR("POOPPOOP", US_OOP, sendS, key);

	TRACE("send[%s]\n", sendS);
}

void CMapForm::queryHistory()
{
	_lsmid lsmid;
	memset(&lsmid, ' ', sz_lsmid);

	lsmid.func[0] = '2';	// 조회
	memcpy(&lsmid.mapx, CATCHMAP, sizeof(lsmid.mapx));	// main map name
	lsmid.kind[0] = lkHis;					// history 조회


	_gridHi *gridHi = (_gridHi *)&lsmid.gridHi;
	memcpy(gridHi->visible, "99", sizeof(gridHi->visible));
	memcpy(gridHi->rows, "9999", sizeof(gridHi->rows));
	gridHi->type = '1';
	gridHi->dir = '1';
	gridHi->sort = '0';
	gridHi->key = '0';
	memcpy(&gridHi->page, "0000", 4);


	sendTR(TR_LOGIO, (char *)&lsmid, sz_lsmid, SC_HISTORY, 0);
}


void CMapForm::oubSise(char* buf, int len)
{
	if (!m_pSise)	return;

	int pos = 0;
	CString	tmpS;

	const int maxSiseCol = sizeof(grInfoSise)/sizeof(_grInfo);

	// clear
	for (int ii = 0; ii < maxSiseCol; ii++)
		m_pSise->SetItemText(1, ii, "");

	int fkind = FK_NOT;
	CString datS = CString(buf, len);
	while (!datS.IsEmpty())
	{
		tmpS = Parser(datS, "\t");
		tmpS.TrimRight();	tmpS.TrimLeft();	
		
		switch (pos)
		{
		case ISCOL_CODE :
			m_rtsCode = tmpS;
			if (!tmpS.IsEmpty() && (tmpS.GetAt(0) == 'A' || tmpS.GetAt(0) == 'J'))
				tmpS = tmpS.Mid(1);
			break;
		case ISCOL_DIFF :
			if (atof(tmpS) == 0.0)
				tmpS = "0";
		}
		fkind = getFKind(pos);
		m_pSise->SetItemText(1, pos, _GetFormatData(fkind, tmpS));
		pos++;
	}
}

void CMapForm::oubHistory(char* buf, int len)
{
	_lsmod *lsmod = (struct _lsmod *)buf;
	int nrec = atoi(_GetString(lsmod->nrec, sizeof(lsmod->nrec)));
	CDlgHistory dlg(this, m_pParent);
	dlg.SetEnv(m_sRootDir, m_sName, m_lf);
	buf += sizeof(lsmod->nrec);
	dlg.SetData(nrec, buf);
	dlg.DoModal();
}

int CMapForm::getFKind(int siseCol)
{
	int fkind = FK_NOT;
	switch (siseCol)
	{
	case ISCOL_CODE:	case ISCOL_NAME:
		fkind = FK_NOT;
		break;
	case ISCOL_CURR:	case ISCOL_MDGA:	case ISCOL_MSGA:
		switch (m_IdCurKind)
		{
		case xALLCODE : fkind = FK_COMMA;	break;
		case xFUTURE : 
		case xCALL :
		case xPUT :	fkind = FK_FLOATx;		break;
		}		
		break;
	case ISCOL_GVOL:
		fkind = FK_COMMA;
		break;
	case ISCOL_RATE:
		fkind = FK_FLOATx;
		break;
	}
	return fkind;
}

void CMapForm::deleteAllMapCatch()
{
	_vInfo.clear();
	m_mapCatch.RemoveAll();
	m_arCatch.RemoveAll();
}


void CMapForm::updateCatch()
{
	if (!m_pCatch)	return;
	
	CString	codeS;
	struct	_codeinfo* codeinfo = nullptr;
	int row = 1;
	CString tmpS, markS;

	for (int ii = 0; ii < m_arCatch.GetSize(); ii++)
	{
		codeS = m_arCatch.GetAt(ii);
		if (!m_mapCatch.Lookup(codeS, codeinfo))
			continue;
		row = m_pCatch->GetRowCount();
		insertGrid(m_pCatch.get(), IDC_GRCATCH, row);
		updateCatch(row, codeinfo, false);
	}
}

void CMapForm::updateCatch(int row, _codeinfo *codeinfo, bool bInit)
{
	if (bInit)
	{
		for (int col = 0; col <= sizeof(grInfoCatch)/sizeof(_grInfo); col++)
			m_pCatch->SetItemText(row, col, "");
	}

	COLORREF clrCond = GetIndexColor(CLRGRROWB1);
	const COLORREF clrUp = GetIndexColor(CLRUP);
	const COLORREF clrDn = GetIndexColor(CLRDN);

	CString tmpS;
	if (codeinfo->capture[0] == '1')	// 포착여부
		m_pCatch->SetItemText(row, ICCOL_CATCH, "◎");

	tmpS = getString(codeinfo->code, sizeof(codeinfo->code));	// 종목코드
	m_pCatch->SetItemText(row, ICCOL_CODE, tmpS);

	tmpS = getString(codeinfo->name, sizeof(codeinfo->name));	// 종목명
	m_pCatch->SetItemText(row, ICCOL_NAME, tmpS);
	
	if (codeinfo->o_run[0] == '1')		// 주문창
	{
// 		if (codeinfo->o_kind[0] == '1')
// 		{
// 			m_pCatch->SetItemText(row, ICCOL_JUMUN, "매수");
// 			m_pCatch->SetItemFgColor(row, ICCOL_JUMUN, GetIndexColor(CLRMSF));
// 		}
// 		else
// 		{
// 			m_pCatch->SetItemText(row, ICCOL_JUMUN, "매도");
// 			m_pCatch->SetItemFgColor(row, ICCOL_JUMUN, GetIndexColor(CLRMDF));
// 		}
		//+ 요건 번호 : 0014 [20090513:kbkim]
		if (codeinfo->o_run[0] == '1')
		{
			if (codeinfo->capture[0] == '1')
				m_pCatch->SetItemText(row, ICCOL_JUMUN, "호출"); // 포착된  항목
			else
				m_pCatch->SetItemText(row, ICCOL_JUMUN, "예약");

			if (codeinfo->o_kind[0]=='0')
				m_pCatch->SetItemFgColor(row, ICCOL_JUMUN, GetIndexColor(CLRMDF));
			else
				m_pCatch->SetItemFgColor(row, ICCOL_JUMUN, GetIndexColor(CLRMSF));
		}
		//- 
	}
	if (codeinfo->a_kind[0] & 0x01)		// 확인창
		m_pCatch->SetItemText(row, ICCOL_WINDOW, "Y");
	if (codeinfo->a_kind[0] & 0x02)		// 관심종목신호적용
		m_pCatch->SetItemText(row, ICCOL_INTER, "Y");
	if (codeinfo->a_kind[0] & 0x04)		// 소리적용
		m_pCatch->SetItemText(row, ICCOL_SOUND, "Y");

	// 감시항목	0:none, 1:현재가, 2:전일대비, 3:매도호가, 4:매수호가, 5:등락률, 6:거래량
	for (int ii = 0, col = ICCOL_CURR; ii < 6; ii++)
	{
		if (codeinfo->alarm[ii].kind[0] > '0')
		{
			tmpS = getString(codeinfo->alarm[ii].aval, sizeof(codeinfo->alarm[ii].aval));
			m_pCatch->SetItemText(row, col, tmpS);
			clrCond = GetIndexColor(CLRGRROWB1);
			if (!tmpS.IsEmpty())
			{
				// 감시조건 '0':이상, '1':이하
				if (codeinfo->alarm[ii].cond[0] == '1')	
				{
					tmpS = "<=";
					clrCond = clrDn;
				}
				else
				{
					tmpS = ">=";
					clrCond = clrUp;
				}
				m_pCatch->SetItemText(row, col+1, tmpS);

				if (ii != 0)
				{
					// 감시연산 '0':AND,  '1':OR
					if (codeinfo->alarm[ii].oper[0] == '1')
					{
						tmpS = "OR";
					}
					else
					{
						tmpS = "AND";
					}
					m_pCatch->SetItemText(row, col-1, tmpS);
				}
			}
			m_pCatch->SetItemBkColor(row, col, clrCond);
		}
		col += 3;
	}
}
	
CString	CMapForm::getString(char* datB, int datL)
{
	for (int ii = 0; ii < datL; ii++)
	{
		if (datB[ii] == 0x00)
			datB[ii] = ' ';
	}
	
	CString	string = CString(datB, datL);
	string.TrimLeft(); string.TrimRight();
	return string;
}
	
void CMapForm::popupSetup(bool bAdd, CString codeS, int updateRow /*=-1*/)
{
	if (!m_pSise)	return;
	if (m_mapCatch.GetCount() > maxJONGMOK)
	{
		setMessageBox("최대 [%d]개의 시세포착조건을 설정하실 수 있습니다.", maxJONGMOK);
		return;
	}
	m_pSetupDlg.reset();

	int gubn{};
	CString nameS = getCodeName(codeS, gubn);
	gubn = m_pParent->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)codeS);

	m_pSetupDlg = std::make_unique<CSetupDlg>(m_pParent, m_pParent, codeS, nameS);
	_codeinfo *codeinfo = nullptr;
	if (bAdd)
	{
		if (!m_mapCatch.Lookup(codeS, codeinfo))
		{
			auto &info = _vInfo.emplace_back(std::make_unique< _codeinfo>());
			memset(info.get(), ' ', sz_codeinfo);
			memcpy(&info->code, codeS, codeS.GetLength());
			memcpy(&info->name, nameS, nameS.GetLength());
			m_mapCatch.SetAt(codeS, info.get());

			codeinfo = info.get();
		}
		else
		{
			setMessageBox("[%s : %s] 시세포착조건이 있습니다.", codeS, nameS);
			return;
		}
	}
	else 
	{
		if (!m_mapCatch.Lookup(codeS, codeinfo))
			return;
	}

	CString siseS = "";
	CString colS;
	for (int col = ISCOL_CODE; col <= ISCOL_GVOL; col++)
	{
		colS = m_pSise->GetItemText(SISEROW, col);
		colS = _GetFormatData(FK_REMOVE, colS);
		siseS +=  colS + "\t";
	}
	
	m_pSetupDlg->SetCodeInfo(bAdd, siseS, codeinfo, gubn);
	m_pSetupDlg->SetFont(m_lf, m_pFont);
	m_pSetupDlg->SetBFont(m_blf, m_pBFont);
	m_pSetupDlg->SetRoot(m_sRootDir);

	if (m_pSetupDlg->DoModal() == IDCANCEL)
	{
		if (bAdd)
		{
			m_mapCatch.RemoveKey(codeS);
			setMessage("[%s : %s] 시세포착조건 설정을 취소하셨습니다.", codeS, nameS);
		}
	}
	else
	{
		m_pSetupDlg->GetCodeInfo(codeinfo);

		if (bAdd)
		{
			const int row = m_pCatch->GetRowCount();
			insertGrid(m_pCatch.get(), IDC_GRCATCH, row);
			updateCatch(row, codeinfo, false);

			setMessage("[%s : %s] 시세포착조건 설정을 추가하셨습니다.", codeS, nameS);
		}
		else
		{
			updateCatch(updateRow, codeinfo, true);
			setMessage("[%s : %s] 시세포착조건 설정을 수정하셨습니다.", codeS, nameS);
		}
	}
	Invalidate();
}

void CMapForm::setMessage(char* format, ...)
{
	CString tmpS;
	va_list valist;
	va_start(valist, format);
	tmpS.FormatV(format, valist);
	va_end(valist);

	m_strGuideMessage = tmpS;
}

void CMapForm::setMessageBox(char* format, ...)
{
	CString tmpS;
	va_list valist;
	va_start(valist, format);
	tmpS.FormatV(format, valist);
	va_end(valist);

	MessageBox(tmpS, "시세포착", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
}

int CMapForm::findIndex(char* str, int len)
{
	const int rlen = delSpace(str, len);

	char* buf{};
	CString	tmp = _T("");

	for (int ii = 0; ii < m_list.GetItemCount(); ii++)
	{
		tmp = m_list.GetItemText(ii, 1);
		buf = (LPSTR)(tmp.operator LPCTSTR());
		delSpace(buf, HNameLen);
		if (memcmp(buf, str, rlen) >= 0)
			return ii;
	}
	return -1;
}

int CMapForm::findIndex(char* str, int len, bool bnum)
{
	const int rlen = delSpace(str, len);

	char* buf{};
	CString	tmp = _T("");

	for (int ii = 0; ii < m_list.GetItemCount(); ii++)
	{
		tmp = m_list.GetItemText(ii, 0);
		buf = (LPSTR)(tmp.operator LPCTSTR());
		if (memcmp(buf, str, rlen) >= 0)
			return ii;
	}
	return -1;
}

int CMapForm::delSpace(char* str, int len)
{
	char	temp[128]{};
	int jj = 0;
	for (int ii = 0; ii < len; ii++)
	{
		if (str[ii] != 0x20)
			temp[jj++] = str[ii];
	}
	CopyMemory(str, temp, jj);
	str[jj] = 0x00;
	return jj;
}

void CMapForm::selectList(int key /* = SC_SISE */)
{
	const int index = m_list.GetNextItem(-1, LVNI_SELECTED);
	if (index < 0)
		return;
	CString codeS, nameS;
	_codeinfo *codeinfo = nullptr;

	if (key == SC_SISEDBCLICK && m_mapCatch.Lookup(codeS, codeinfo))
	{
		setMessageBox("[%s : %s] 시세포착조건이 있습니다.", codeS, nameS);
		return;
	}

	codeS = m_list.GetItemText(index, 0);

	querySise(codeS, key);
}

void CMapForm::OnBtninter() 
{
	_codeinfo *codeinfo = nullptr;
	CString codeS, sendS, tmpS;
	int cnt = 0;
	
	sendS.Format("%s%c", _T("내관심종목"), gnCHAR);

	for (int row = 1; row < m_pCatch->GetRowCount(); row++)
	{
		codeS = m_pCatch->GetItemText(row, ICCOL_CODE);
		if (!m_mapCatch.Lookup(codeS, codeinfo))
			continue;
		
		tmpS.Format("%s%c", codeS, 0x7f);
		sendS += tmpS;

		cnt++;
	}
	if (cnt == 0)	return;

	tmpS.Format("%s /S/d%s%c%s%c", configMAP, "appendGROUP", tabCHAR, sendS, lfCHAR);

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)tmpS);
}

////////////////////////// 
// 함수명	    : CMapForm::MoveCtrl
// 설 명	    : 다이얼로그에 컨트롤을 움직이는 기능
// 작성일시	    : 2009/06/02
// 작성자       : kbkim
// 파라미터	    : UINT nCtrlID : 움직이고자 하는  컨트롤 ID
//				  int nCx : 움직이고자 하는 X 크기(음수:왼쪽으로)
//				  int nCy : 움직이고자 하는 Y 크기(음수:위쪽으로)
// 리턴 값 		: 
// 관련요건	    : 
////////////////////////// 
int CMapForm::MoveCtrl(UINT nCtrlID, int nCx, int nCy, int nCtrlWidthGap)
{
	CRect rCtrl;
	
	if(!GetDlgItem(nCtrlID)->GetSafeHwnd())
		return 0;

	GetDlgItem(nCtrlID)->GetWindowRect(rCtrl);
	ScreenToClient(&rCtrl);
	GetDlgItem(nCtrlID)->SetWindowPos(nullptr, rCtrl.left + nCx, rCtrl.top + nCy, 0, 0, SWP_NOSIZE);
	
	return int(rCtrl.right) + nCtrlWidthGap;
}


int CMapForm::MoveCtrl(CWnd* pWnd, int nCx, int nCy, int nCtrlWidthGap)
{
	CRect rCtrl;
	
	pWnd->SetWindowPos(nullptr, nCx, nCy, 0, 0, SWP_NOSIZE);
	pWnd->GetWindowRect(rCtrl);
	ScreenToClient(&rCtrl);
	
	return int(rCtrl.right) + nCtrlWidthGap;
}

void CMapForm::sendTR(CString trC, char* pBytes, int nBytes, int key, BYTE stat)
{
	if (!m_pParent)	
		return;

	const int len = L_userTH + nBytes;
	auto sndB = std::make_unique<char[]>(len+1);
	struct _userTH* udat = (struct _userTH*) sndB.get();

	
	memcpy(udat->trc, trC, trC.GetLength());
	udat->key = key;	
	udat->stat = stat;

	CopyMemory(&sndB[L_userTH], pBytes, nBytes);
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, nBytes), (LPARAM)sndB.get());
}
#ifdef _DEBUG
void CMapForm::draw_groupBox(CDC *pdc, int ids)
#else
void CMapForm::draw_groupBox(xxx::CMemDC* pdc, int ids)
#endif
{
	pdc->SelectObject(m_pFont);
	CRect rc, out;
	GetDlgItem(ids)->GetClientRect(rc);
	GetDlgItem(ids)->GetWindowRect(out);
	ScreenToClient(out);
	
	rc.top+=6;
	CPen pn_in, pn_out;
	pn_in.CreatePen(PS_SOLID, 1, GetAxColor(38));
	pn_out.CreatePen(PS_SOLID, 1, GetAxColor(76));
	
	rc.OffsetRect(out.left, out.top); 
	rc.OffsetRect(1,1);
	
	pdc->SelectObject(pn_out);
	pdc->MoveTo(rc.left, rc.top); pdc->LineTo(rc.right, rc.top); pdc->LineTo(rc.right, rc.bottom);
	pdc->LineTo(rc.left, rc.bottom); pdc->LineTo(rc.left, rc.top);
	rc.OffsetRect(-1,-1);
	pdc->SelectObject(pn_in);
	pdc->MoveTo(rc.left, rc.top); pdc->LineTo(rc.right, rc.top); pdc->LineTo(rc.right, rc.bottom);
	pdc->LineTo(rc.left, rc.bottom); pdc->LineTo(rc.left, rc.top);
	
	CString data;
	GetDlgItem(ids)->GetWindowText(data);
	pdc->TextOut(rc.left+7, rc.top-5, data);
} 


void CMapForm::sendTRx(CString trCode, char* datB, int datL, int key)
{
	std::string sendB;
	sendB.resize(L_userTH + datL + 1);
	struct _userTH* uTH{};
	uTH = (struct _userTH*)sendB.c_str();
	strcpy(uTH->trc, trCode);
	uTH->key = key;
	uTH->stat = US_PASS;

	CopyMemory(&sendB[L_userTH], datB, datL);
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datL), (LPARAM)sendB.c_str());
	sendB.clear();
} 

void CMapForm::Request_GroupList()
{
	int	sendL = 0;
	std::string sendB;
	sendB.resize(16 * 1024);
	const int isz_updn = sz_uinfo;
	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += isz_updn;

	FillMemory((char*)updn, isz_updn, ' ');
	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'g';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'O';
	sendTRx(trUPDOWN, sendB.data(), sendL, TRKEY_GROUP);
}

void CMapForm::Request_GroupCode(int iseq)
{
	const int index = iseq;
	int sendL = 0;
	CString stmp;
	std::string sendB;
	sendB.resize(16 * 1024);

	const int isz_updn = sz_uinfo + 2;
	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += isz_updn;
	FillMemory((char*)updn, isz_updn, ' ');
	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'j';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'O';
	stmp.Format("%d", index);
	memcpy((char*)&sendB[sz_uinfo], stmp, stmp.GetLength());

	sendTRx(trUPDOWN, sendB.data(), sendL, TRKEY_INTER + iseq);
}

void CMapForm::receiveOub(int key, CString data)
{
	struct  _glistex {
		char    ngrs[2];           /* New Group Seqn       */
		char    ogrs[2];           /* Old Group Seqn       */
		char	gnam[30];
	};

	if (key == TRKEY_GROUP)
	{
		const int cnt = atoi(data.Left(4));
		data = data.Mid(4);
		data.Trim();
		if (!data.IsEmpty() && cnt > 0)
		{
			//std::vector<std::pair<CString, CString>> vGroupName;
			const gsl::span<_glistex> groupList((_glistex*)data.GetString(), cnt);

			std::sort(groupList.begin(), groupList.end(), [](auto item, auto item1) {
				return CString(item.ngrs, 2).Trim().Compare((CString(item1.ngrs, 2).Trim())) < 0;
			});

			for_each(groupList.begin(), groupList.end(), [&](auto item) {
				const CString groupKey = CString(item.ngrs, 2).Trim();
				const CString groupName = CString(item.gnam, 30).Trim();

				data.Format("%02d%03d", xINTEREST, atoi(groupKey));
				m_tree.SetItemData(m_tree.InsertItem(groupName, _hInter), atoi(data));
			});	
		}
	}
	else
	{
		receiveOubCode(data, key);
	}
}


void CMapForm::receiveOubCode(CString& data, int key)
{
	const CString skey = data.Left(2);
	const CString sGroupName = data.Mid(2, 20);
	const CString sCount = data.Mid(22, 4);
	data = data.Mid(26);

	m_list.DeleteAllItems();
	const gsl::span<struct _jinfo> codelist((struct _jinfo*)data.GetString(), atoi(sCount));

	CString codeS, nameS;
	int row = 0;
	for_each(codelist.begin(), codelist.end(), [&](auto item) {	
		codeS = CString(item.code, 12).Trim();
		int codeGubn = 0;
		nameS = getCodeName(codeS, codeGubn);
		addItem(&m_list, codeS, row, 0);
		addItem(&m_list, nameS, row++, 1);
	});
}