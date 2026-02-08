// OrderSetup.cpp : implementation file
//

#include "stdafx.h"
#include "OrderSetup.h"
#include "../../h/axisvar.h"

#include "mmsystem.h"

#define	SAVEFILE	"ordcfg.ini"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma	comment(lib, "Winmm.lib")

#define TOOLCOUNT_MIN	1
#define TOOLCOUNT_MAX	5
#define TOOLCOUNT_DEF	3
/////////////////////////////////////////////////////////////////////////////
// COrderSetup dialog
#define COLOR_INLINE	RGB(103,170,197)
#define COLOR_OUTLINE	RGB(255,255,255)

COrderSetup::COrderSetup(CWnd* pParent /*=NULL*/)
	: CSetupDialog(COrderSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(COrderSetup)
	m_ckcount = 0;
	m_autohide = 0;
	m_matchToolCount = 3;
	//}}AFX_DATA_INIT
	m_caption = "주문 / 체결 설정";
	section = "Common";

	InitButtonMap();
}

COrderSetup::~COrderSetup()
{
	FreeButtonMap();
}

void COrderSetup::DoDataExchange(CDataExchange* pDX)
{
	CSetupDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COrderSetup)
	DDX_Text(pDX, IDC_CHECOUNT, m_ckcount);
	DDV_MinMaxUInt(pDX, m_ckcount, 0, 99);
	DDX_Text(pDX, IDC_HIDECOUNT, m_autohide);
	DDV_MinMaxUInt(pDX, m_autohide, 0, 99);
	DDX_Text(pDX, IDC_MATCHTOOLCOUNT, m_matchToolCount);
	DDV_MinMaxInt(pDX, m_matchToolCount, 1, 5);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COrderSetup, CSetupDialog)
	//{{AFX_MSG_MAP(COrderSetup)
	ON_WM_PAINT()
	ON_NOTIFY(UDN_DELTAPOS, IDC_MATCHTOOLCOUNTSPIN, OnDeltaposMatchtoolcountspin)
	ON_EN_CHANGE(IDC_MATCHTOOLCOUNT, OnChangeMatchtoolcount)
	//}}AFX_MSG_MAP
	ON_NOTIFY(UDN_DELTAPOS, IDC_CHESPIN, OnCheSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_HIDESPIN, OnHideSpin)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrderSetup message handlers
void COrderSetup::OnCheSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	const NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	UpdateData(TRUE);
	if (pNMUpDown->iDelta > 0)
		m_ckcount -= 1;
	else	m_ckcount += 1;
	if (m_ckcount < 0) m_ckcount = 0;
	if (m_ckcount > 99) m_ckcount = 99;
	UpdateData(FALSE);
	*pResult = 0;
}

void COrderSetup::OnHideSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	const NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	UpdateData(TRUE);
	if (pNMUpDown->iDelta > 0)
	{
		if (m_autohide != 0)
			m_autohide -= 1;
	}
	else	m_autohide += 1;
	if (m_autohide < 0) m_autohide = 0;
	if (m_autohide > 99) m_autohide = 99;
	UpdateData(FALSE);
	*pResult = 0;
}

BOOL COrderSetup::OnInitDialog() 
{
	CSetupDialog::OnInitDialog();
	m_font.CreatePointFont(90, "굴림체");
	((CEdit *)GetDlgItem(IDC_CHECOUNT))->LimitText(2);
	((CEdit *)GetDlgItem(IDC_HIDECOUNT))->LimitText(2);

	loadInfo();

	const UINT ids[] = {IDC_ST_CONCLUSION, IDC_ST_SOUND};
	const int	 cnt   = sizeof(ids) / sizeof(UINT);

	for ( int ii = 0 ; ii < cnt ; ii++ )
	{
		GetDlgItem(ids[ii])->GetWindowRect(m_rect[ii]);
		m_rect[ii].DeflateRect(0, 6, 0, 2);
		ScreenToClient(m_rect[ii]);
	}
	
	return TRUE;
}

void COrderSetup::defaultStat()
{
	Load(Format("%s\\%s\\%s", Axis::home, MTBLDIR, SAVEFILE), FALSE);
	((CButton *) GetDlgItem(IDC_ORDERWINDOW))->SetCheck(BST_CHECKED);
}

const int maxPosButtons = 6;
const UINT posButtons[maxPosButtons] = 
{
	IDC_POSLT, IDC_POSCENTER, IDC_POSRT, IDC_POSLB, IDC_POSMOUSE, IDC_POSRB
};

void COrderSetup::Load(const char* fileName, BOOL bSound)
{
	CProfile profile(fileName);
	CAttribute* attr{};

	for (auto& item : _buttons)
	{
		const BOOL bflag = profile.GetInt(section, item->tagName, item->defValue);
		((CButton*)GetDlgItem(item->resourceID))->SetCheck(bflag);
	}

	const int value = profile.GetInt(section, "CK_RTF", CON_POPUP | CON_SOUND);

	((CButton *) GetDlgItem(IDC_POPUPWINDOW))->SetCheck(value & CON_POPUP);
	EnableConclusion(value & CON_POPUP);

	((CButton *) GetDlgItem(IDC_MSNWINDOW))->SetCheck(value & CON_MSN);

	((CButton *) GetDlgItem(IDC_APPLY_SOUND))->SetCheck(value & CON_SOUND);
	EnableSound(value & CON_SOUND);

	m_ckcount = profile.GetInt(section, "CK_HISTORY", 20);
	m_autohide = profile.GetInt(section, "CK_AUTOHIDE");
	m_matchToolCount = profile.GetInt(section, "MatchToolCount", TOOLCOUNT_DEF);

	int matchPos = profile.GetInt(section, "CK_POS", 5);

	for (int i = 0; i < maxPosButtons; i++)
		GetDlgItem(posButtons[i])->SendMessage(BM_SETCHECK, FALSE);

	if (matchPos < 0 || matchPos >= maxPosButtons)
		matchPos = 5;

	GetDlgItem(posButtons[matchPos])->SendMessage(BM_SETCHECK, TRUE);

	CheckEnableToolCount();

	DefaultSoundFile();

	if (bSound)
	{
		m_waveF[IDX_MDORDER] = profile.GetString(section, "SOUNDFILE_MDORDER", m_waveF[IDX_MDORDER]);
		m_waveF[IDX_MSORDER] = profile.GetString(section, "SOUNDFILE_MSORDER", m_waveF[IDX_MSORDER]);
		m_waveF[IDX_JJORDER] = profile.GetString(section, "SOUNDFILE_JJORDER", m_waveF[IDX_JJORDER]);
		m_waveF[IDX_CSORDER] = profile.GetString(section, "SOUNDFILE_CSORDER", m_waveF[IDX_CSORDER]);
		m_waveF[IDX_MDCON]   = profile.GetString(section, "SOUNDFILE_MDCON",   m_waveF[IDX_MDCON]);
		m_waveF[IDX_MSCON]   = profile.GetString(section, "SOUNDFILE_MSCON",   m_waveF[IDX_MSCON]);
		m_waveF[IDX_REFUSAL] = profile.GetString(section, "SOUNDFILE_REFUSAL", m_waveF[IDX_REFUSAL]);
		/*
		//초기 설정값 변경
		for(int i=0; i<7;i++)
		{
			if (m_waveF[i].Find("C:\Program Files\IBKS\HTS"))
			{
				if (Axis::home != "C:\Program Files\IBKS\HTS")
				{
					m_waveF[i] = Axis::home +"\\"+ m_waveF[i].Mid(26, m_waveF[i].GetLength()-25);
				}
			}
		}
		*/
		


	}
	GetDlgItem(IDC_MDORDER_FILENAME)->SetWindowText(m_waveF[IDX_MDORDER]);
	GetDlgItem(IDC_MSORDER_FILENAME)->SetWindowText(m_waveF[IDX_MSORDER]);
	GetDlgItem(IDC_JJORDER_FILENAME)->SetWindowText(m_waveF[IDX_JJORDER]);
	GetDlgItem(IDC_CSORDER_FILENAME)->SetWindowText(m_waveF[IDX_CSORDER]);
	GetDlgItem(IDC_MDCON_FILENAME)->SetWindowText(m_waveF[IDX_MDCON]);
	GetDlgItem(IDC_MSCON_FILENAME)->SetWindowText(m_waveF[IDX_MSCON]);
	GetDlgItem(IDC_REFU_FILENAME)->SetWindowText(m_waveF[IDX_REFUSAL]);

	UpdateData(FALSE);
}

void COrderSetup::loadInfo()
{
	Load(Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, SAVEFILE),  TRUE);

	m_status = AfxGetApp()->GetProfileInt(ENVIRONMENT, STATUS, 0);
	if (m_status & stConfirm)
		((CButton *) GetDlgItem(IDC_ORDERWINDOW))->SetCheck(BST_CHECKED);
	
	UpdateData(FALSE);
}

void COrderSetup::saveInfo()
{
	UpdateData(TRUE);
	Save(Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, SAVEFILE));

	if (((CButton *) GetDlgItem(IDC_ORDERWINDOW))->GetCheck() == BST_CHECKED)
		m_status |= stConfirm;
	else	
		m_status &= ~stConfirm;

	AfxGetApp()->WriteProfileInt(ENVIRONMENT, STATUS, m_status);
}

void COrderSetup::Save(const char* fileName)
{
	CProfile profile(fileName);
	
	for (auto item : _buttons)
	{
		profile.Write(section, item->tagName, ((CButton*)GetDlgItem(item->resourceID))->GetCheck());
	}

	DWORD	rtfK = 0;
	if (((CButton *) GetDlgItem(IDC_POPUPWINDOW))->GetCheck() == BST_CHECKED)	rtfK = CON_POPUP;
	if (((CButton *) GetDlgItem(IDC_MSNWINDOW))->GetCheck() == BST_CHECKED)		rtfK |= CON_MSN;
	if (((CButton *) GetDlgItem(IDC_APPLY_SOUND))->GetCheck() == BST_CHECKED)	rtfK |= CON_SOUND;

	profile.Write(section, "CK_RTF", (int)rtfK);

	profile.Write(section, "CK_HISTORY", (int)m_ckcount);
	profile.Write(section, "CK_AUTOHIDE", (int)m_autohide);
	profile.Write(section, "MatchToolCount", (int)m_matchToolCount);

	int index = 5;
	for (int i = 0; i < maxPosButtons; i++)
	{
		if (GetDlgItem(posButtons[i])->SendMessage(BM_GETCHECK))
		{
			index = i;
			break;
		}
	}
	profile.Write(section, "CK_POS", index);

	profile.Write(section, "SOUNDFILE_MDORDER", m_waveF[IDX_MDORDER]);
	profile.Write(section, "SOUNDFILE_MSORDER", m_waveF[IDX_MSORDER]);
	profile.Write(section, "SOUNDFILE_JJORDER", m_waveF[IDX_JJORDER]);
	profile.Write(section, "SOUNDFILE_CSORDER", m_waveF[IDX_CSORDER]);
	profile.Write(section, "SOUNDFILE_MDCON",	m_waveF[IDX_MDCON]);
	profile.Write(section, "SOUNDFILE_MSCON",	m_waveF[IDX_MSCON]);
	profile.Write(section, "SOUNDFILE_REFUSAL", m_waveF[IDX_REFUSAL]);
}

BOOL COrderSetup::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int	idx = -1;
	CString	string;
	switch (LOWORD(wParam))
	{
	case IDC_REFU_SEARCH:idx++;
	case IDC_MSCON_SEARCH:	idx++;
	case IDC_MDCON_SEARCH:	idx++;
	case IDC_CSORDER_SEARCH:idx++;
	case IDC_JJORDER_SEARCH:idx++;
	case IDC_MSORDER_SEARCH:idx++;
	case IDC_MDORDER_SEARCH:idx++;
		SearchFile(idx);
		break;
	case IDC_REFU_PLAY:	idx++;
	case IDC_MSCON_PLAY:	idx++;
	case IDC_MDCON_PLAY:	idx++;
	case IDC_CSORDER_PLAY:	idx++;
	case IDC_JJORDER_PLAY:	idx++;
	case IDC_MSORDER_PLAY:	idx++;
	case IDC_MDORDER_PLAY:	idx++;
		Soundplay(idx);
		break;
	case IDC_APPLY_SOUND:
		EnableSound(((CButton*)GetDlgItem(IDC_APPLY_SOUND))->GetCheck());
		break;

	case IDC_POPUPWINDOW:
		EnableConclusion(((CButton*)GetDlgItem(IDC_POPUPWINDOW))->GetCheck());
		break;
	case IDC_MATCHTOOLBAR:
		CheckEnableToolCount();
		break;
	case IDOK:	
		saveInfo();	
		break;
	case IDC_DEFAULTORDER:
		defaultStat();
		break;
	default:
		break;
	}
	return CSetupDialog::OnCommand(wParam, lParam);
}

void COrderSetup::SearchFile(int idx)
{
	//CString tmp; tmp.Format("%d",idx);
	//AfxMessageBox(tmp);
	if (idx < 0 || idx >= MAXSOUND)
		return;
	//AfxMessageBox(m_waveF[idx]);
	if (!IsFileExist(m_waveF[idx])) m_waveF[idx] = "";
	
	CFileDialog	dlg(TRUE, "*.wav", m_waveF[idx], 
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_EXPLORER,
			"Sound file(*.wav) |*.wav||", NULL );
	if (dlg.DoModal() == IDOK)
	{
		m_waveF[idx] = dlg.GetPathName();
		UINT nRes{};
		switch (idx)
		{
		case 0:	nRes = IDC_MDORDER_FILENAME;	break;
		case 1:	nRes = IDC_MSORDER_FILENAME;	break;
		case 2:	nRes = IDC_JJORDER_FILENAME;	break;
		case 3:	nRes = IDC_CSORDER_FILENAME;	break;
		case 4:	nRes = IDC_MDCON_FILENAME;	break;
		case 5:	nRes = IDC_MSCON_FILENAME;	break;
		case 6:	nRes = IDC_REFU_FILENAME;	break;
		default:	return;
		}
		GetDlgItem(nRes)->SetWindowText(m_waveF[idx]);
	}
}

void COrderSetup::Soundplay(int idx)
{
	if (m_waveF[idx].IsEmpty())
		MessageBeep(MB_OK);
	else
	{
		sndPlaySound(NULL, SND_NODEFAULT|SND_ASYNC);
		sndPlaySound(m_waveF[idx], SND_NODEFAULT|SND_ASYNC);
	}
}


void COrderSetup::EnableConclusion(BOOL bEnable)
{
	CWnd*	pWnd = NULL;
	
	// 체결 그룹 박스
	//GetDlgItem(IDC_ST_CONCLUSION)->EnableWindow(bEnable);
	const UINT ids[] = {
			IDC_ST_POSITION,
			IDC_POSLT,
			IDC_POSCENTER,
			IDC_POSRT,
			IDC_POSLB,
			IDC_POSMOUSE,
			IDC_POSRB,
			IDC_ST_HISTORY,
			IDC_CHECOUNT,
			IDC_CHESPIN,
			IDC_ST_HISTORY2,
			IDC_ST_AUTOHIDE,
			IDC_HIDECOUNT,
			IDC_HIDESPIN,
			IDC_ST_AUTOHIDE2,
			//IDC_ST_SOUND
		};
	
	const int	ncnt = sizeof(ids)/sizeof(UINT);

	for ( int  ii = 0 ; ii < ncnt ; ii++ )
	{
		pWnd = GetDlgItem(ids[ii]);
		pWnd->EnableWindow(bEnable);
	}
/*
	if (bEnable)
	{
		if (((CButton*)GetDlgItem(IDC_APPLY_SOUND))->GetCheck() == BST_CHECKED)
			EnableSound(TRUE);
	}
	else
		EnableSound(FALSE);	*/
}

void COrderSetup::EnableSound(BOOL bEnable)
{
	CWnd*	pWnd = NULL;
	
	// 체결 그룹 박스
	//GetDlgItem(IDC_ST_CONCLUSION)->EnableWindow(bEnable);
	const UINT ids[] = {
			IDC_MDORDER,
			IDC_MDORDER_FILENAME,
			IDC_MDORDER_SEARCH,
			IDC_MDORDER_PLAY,
			IDC_MSORDER,
			IDC_MSORDER_FILENAME,
			IDC_MSORDER_SEARCH,
			IDC_MSORDER_PLAY,
			IDC_JJORDER,
			IDC_JJORDER_FILENAME,
			IDC_JJORDER_SEARCH,
			IDC_JJORDER_PLAY,
			IDC_CSORDER,
			IDC_CSORDER_FILENAME,
			IDC_CSORDER_SEARCH,
			IDC_CSORDER_PLAY,
			IDC_MDCON,
			IDC_MDCON_FILENAME,
			IDC_MDCON_SEARCH,
			IDC_MDCON_PLAY,
			IDC_MSCON,
			IDC_MSCON_FILENAME,
			IDC_MSCON_SEARCH,
			IDC_MSCON_PLAY,
			IDC_REFUSAL,
			IDC_REFU_FILENAME,
			IDC_REFU_SEARCH,
			IDC_REFU_PLAY
		};
	
	const int	ncnt = sizeof(ids)/sizeof(UINT);

	for ( int  ii = 0 ; ii < ncnt ; ii++ )
	{
		pWnd = GetDlgItem(ids[ii]);
		pWnd->EnableWindow(bEnable);
	}
}

void COrderSetup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	draw_groupBox(&dc, IDC_STATIC_14);
	draw_groupBox(&dc, IDC_STATIC_15);
	draw_groupBox(&dc, IDC_ST_CONCLUSION);
	draw_groupBox(&dc, IDC_ST_POSITION);
	draw_groupBox(&dc, IDC_ST_SOUND);
/*
	CRect	rect;

	for ( int ii = 0 ; ii < 2 ; ii++ )
	{
		DrawBox(&dc, m_rect[ii], RGB(128, 128, 128), BOX_TOP|BOX_LEFT);
		
		rect = m_rect[ii];
		rect.OffsetRect(1, 1);
		DrawBox(&dc, rect, RGB(255, 255, 255), BOX_TOP|BOX_LEFT);
		DrawBox(&dc, m_rect[ii], RGB(128, 128, 128), BOX_BOTTOM|BOX_RIGHT);
		DrawBox(&dc, rect, RGB(255, 255, 255), BOX_BOTTOM|BOX_RIGHT);
	
	}

*/
	// Do not call CSetupDialog::OnPaint() for painting messages
}

void COrderSetup::DrawBox(CDC* pDC, CRect rect, COLORREF clrLine, int opt, COLORREF clrInner, bool bFill)
{
	CPen	pen(PS_SOLID, 1, clrLine);
	CPen*	oldpen = pDC->SelectObject(&pen);
	
	if (bFill)
		pDC->FillSolidRect(rect, clrInner);
	
	if (opt & BOX_LEFT)
	{
		pDC->MoveTo(rect.TopLeft());
		pDC->LineTo(CPoint(rect.left, rect.bottom));
	}

	if (opt & BOX_RIGHT)
	{
		pDC->MoveTo(CPoint(rect.right - 1, rect.top));
		pDC->LineTo(CPoint(rect.right - 1, rect.bottom));
	}

	if (opt & BOX_TOP)
	{
		pDC->MoveTo(rect.TopLeft());
		pDC->LineTo(CPoint(rect.right, rect.top));
	}

	if (opt & BOX_BOTTOM)
	{
		pDC->MoveTo(rect.left, rect.bottom);
		pDC->LineTo(CPoint(rect.right, rect.bottom));
	}
	
	pDC->SelectObject(oldpen);
	pen.DeleteObject();
}


void COrderSetup::ApplySetup()
{
	saveInfo();
}

void COrderSetup::DefaultSetup()
{
	defaultStat();
}

void COrderSetup::InitButtonMap()
{
	struct button {
		UINT buttonID;
		LPCTSTR tagName;
		int defaultValue;
	} const buttons[] = {
		{ IDC_ACCOUNT,  "TabAccount", 1},
		{ IDC_ACCPASS,  "TabPassword", 1},
		{ IDC_JCODE,	"TabCode", 1},
		{ IDC_MMTYPE,	"TabMMGB", 1},
		{ IDC_MSVOL,	"TabQty", 1},
		{ IDC_MSPRICE,	"TabPrc", 1},
		{ IDC_INCLUDEBUTTON, "TabButton", 1},
		{ IDC_MMCONDITION, "TabCondition", 1},
		{ IDC_SAVEPRICE,	"KeepPrc", 1},
		{ IDC_SAVEVOL,		"KeepQty", 1},
		{ IDC_ORDERWINDOW,  "ShowMsg", 1},
		{ IDC_SHOWMSG,		"ShowMsg2", 1},
		{ IDC_MATCHTOOLBAR,	"MatchToolBar", 0},
		{ IDC_SENDENTER,	"EnterOrder", 1},
		{ IDC_ACCLINK,		"AccLink", 1},		
		{ IDC_NOFEE,		"NoFee", 1},
		{ IDC_MDORDER,		"SOUNDON_MDORDER", 1},
		{ IDC_MSORDER,		"SOUNDON_MSORDER", 1},
		{ IDC_JJORDER,		"SOUNDON_JJORDER", 1},
		{ IDC_CSORDER,		"SOUNDON_CSORDER", 1},
		{ IDC_MDCON,		"SOUNDON_MDCON", 1},
		{ IDC_MSCON,		"SOUNDON_MSCON", 1},
		{ IDC_REFUSAL,		"SOUNDON_REFUSAL", 1},
		{ IDC_POSLT,		"PosLT", 0},
		{ IDC_POSCENTER,	"PosCenter", 0},
		{ IDC_POSRT,		"PosRT", 0},
		{ IDC_POSLB,		"PosLB", 0},
		{ IDC_POSMOUSE,		"PosMouse", 0},
		{ IDC_POSRB,		"PosRB", 1},
		{ NULL, NULL }
	};

	for (int i = 0; buttons[i].buttonID; i++)
	{
		_buttons.emplace_back(std::make_shared< CAttribute>(buttons[i].buttonID, buttons[i].tagName, buttons[i].defaultValue, buttons[i].defaultValue));
	}
}

void COrderSetup::FreeButtonMap()
{
	/*while (m_buttons.GetCount())     //vc2019 test
		delete m_buttons.RemoveHead();*/
	_buttons.clear();
}

void COrderSetup::DefaultSoundFile()
{
	m_waveF[IDX_MDORDER].Format("%s\\image\\매도주문접수.wav", Axis::home);
	m_waveF[IDX_MSORDER].Format("%s\\image\\매수주문접수.wav", Axis::home);
	m_waveF[IDX_JJORDER].Format("%s\\image\\정정주문접수.wav", Axis::home);
	m_waveF[IDX_CSORDER].Format("%s\\image\\취소주문접수.wav", Axis::home);
	m_waveF[IDX_MDCON].Format("%s\\image\\매도주문체결.wav", Axis::home);
	m_waveF[IDX_MSCON].Format("%s\\image\\매수주문체결.wav", Axis::home);
	m_waveF[IDX_REFUSAL].Format("%s\\image\\주문거부.wav", Axis::home);
}

void COrderSetup::OnDeltaposMatchtoolcountspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	const NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	UpdateData();
	if (pNMUpDown->iDelta > 0)
	{
		m_matchToolCount--;
	}
	else {
		m_matchToolCount++;
	}

	ValidToolCount();

	UpdateData(FALSE);
	
	*pResult = 0;
}

void COrderSetup::OnChangeMatchtoolcount() 
{
	UpdateData();
	ValidToolCount();
	UpdateData(FALSE);
}

void COrderSetup::ValidToolCount()
{
	if (m_matchToolCount < TOOLCOUNT_MIN)
		m_matchToolCount = TOOLCOUNT_MIN;
	if (m_matchToolCount > TOOLCOUNT_MAX)
		m_matchToolCount = TOOLCOUNT_MAX;
}

void COrderSetup::CheckEnableToolCount()
{
	const BOOL enable = GetDlgItem(IDC_MATCHTOOLBAR)->SendMessage(BM_GETCHECK);

	const UINT ids[] = {
		IDC_MATCHTOOLCOUNT, IDC_MATCHTOOLCOUNTSPIN, IDC_LABEL_MATCHTOOL, NULL
	};

	for (int i = 0; ids[i]; i++)
	{
		GetDlgItem(ids[i])->EnableWindow(enable);
	}
}

void COrderSetup::draw_groupBox(CPaintDC *pdc, int ids)
{
	pdc->SelectObject(m_font);
	CRect rc, out, pout;
	GetDlgItem(ids)->GetClientRect(rc);
	GetDlgItem(ids)->GetWindowRect(out);
	GetWindowRect(pout);
	out.InflateRect(pout);
	/*
	CString tmp;
	tmp.Format("[%d]",out.left);
	AfxMessageBox(tmp);
	*/
	rc.top+=6;
	//rc.right -=20;
	CPen pn_in, pn_out;
	pn_in.CreatePen(PS_SOLID, 1, Axis::GetColor(38));
	pn_out.CreatePen(PS_SOLID, 1, Axis::GetColor(76));
	
	rc.OffsetRect(out.left, out.top);	
	rc.OffsetRect(1,1);

	pdc->SelectObject(pn_out);
	pdc->MoveTo(rc.left, rc.top); pdc->LineTo(rc.right, rc.top); pdc->LineTo(rc.right, rc.bottom);
	pdc->LineTo(rc.left, rc.bottom); pdc->LineTo(rc.left, rc.top);
	rc.OffsetRect(-1,-1);
	pdc->SelectObject(pn_in);
	pdc->MoveTo(rc.left, rc.top); pdc->LineTo(rc.right, rc.top); pdc->LineTo(rc.right, rc.bottom);
	pdc->LineTo(rc.left, rc.bottom); pdc->LineTo(rc.left, rc.top);
	//pdc->SelectObject(pdc->GetCurrentBrush());
	//Axis::dialogBgColor
	//pdc->SelectObject(&Axis::dialogBgBrush);
	pdc->SetBkColor(Axis::dialogBgColor);
	CString data;
	GetDlgItem(ids)->GetWindowText(data);
	data.Replace("&&", "&");
	pdc->TextOut(rc.left+7, rc.top-5, data);
}
