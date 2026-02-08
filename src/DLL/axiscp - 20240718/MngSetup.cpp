// MngSetup.cpp : implementation file
//

#include "stdafx.h"
#include "MngSetup.h"
#include "cpdefine.h"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define COLOR_INLINE	RGB(103,170,197)
#define COLOR_OUTLINE	RGB(255,255,255)
/////////////////////////////////////////////////////////////////////////////
// CMngSetup dialog


CMngSetup::CMngSetup(CWnd* pParent /*=NULL*/)
	: CSetupDialog(CMngSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMngSetup)
	//}}AFX_DATA_INIT

	m_caption = "장운영정보 설정";
	m_mapMng.RemoveAll();

	InitSignalControlMap();
}


void CMngSetup::DoDataExchange(CDataExchange* pDX)
{
	CSetupDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMngSetup)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMngSetup, CSetupDialog)
	//{{AFX_MSG_MAP(CMngSetup)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMngSetup message handlers

BOOL CMngSetup::OnInitDialog() 
{
	CSetupDialog::OnInitDialog();
	m_font.CreatePointFont(90, "굴림체");
	loadInfo();

	
	return TRUE;  
}

void CMngSetup::loadInfo()
{
	CString file;
	file.Format("%s\\%s\\%s\\mngsetup.ini", Axis::home, USRDIR, Axis::user);

	char	ssb[1024*4];
	const DWORD ssL = GetPrivateProfileSection("Manage", ssb, sizeof(ssb), file);
	
	DefaultSetup();

	CString subitem, keys, value, string = CString(ssb, ssL);
	for (; !string.IsEmpty(); )
	{
		int idx = string.Find('\0');
		if (idx == -1)	break;

		subitem  = string.Left(idx++);
		string = string.Mid(idx);

		idx = subitem.Find('=');
		if (idx == -1)	continue;

		keys = subitem.Left(idx++);
		value = subitem.Mid(idx);

		m_mapMng.SetAt(keys, value);
		setControlValue(keys, value);
	}

	int	nRes = -1;
	m_pos = GetPrivateProfileInt("Setup", "Pos", 2, file);
	
	switch (m_pos)
	{
	case 0: 
		nRes = RADIO_UPPERLEFT;
		break;
	case 2: 
		nRes = RADIO_UPPERRIGHT;
		break;
	case 3: 
		nRes = RADIO_BOTTOMLEFT;
		break;
	case 4: 
		nRes = RADIO_BOTTOMRIGHT;
		break;
	}
	if (nRes!=-1)
	{
		((CButton *)GetDlgItem(RADIO_UPPERLEFT))->SetCheck(FALSE);
		((CButton *)GetDlgItem(RADIO_UPPERRIGHT))->SetCheck(FALSE);
		((CButton *)GetDlgItem(RADIO_BOTTOMLEFT))->SetCheck(FALSE);
		((CButton *)GetDlgItem(RADIO_BOTTOMRIGHT))->SetCheck(FALSE);
		((CButton *)GetDlgItem(nRes))->SetCheck(TRUE);
	}

	((CButton *)GetDlgItem(IDC_POPUP))->SetCheck(m_popup = 
						GetPrivateProfileInt("Setup", "Popup", 0, file));
	((CButton *)GetDlgItem(IDC_SOUND))->SetCheck(m_sound = 
						GetPrivateProfileInt("Setup", "Sound", 0, file));
}

void CMngSetup::setControlValue(CString keys, CString value)
{
	UINT controlID{};
	int key = atoi(keys);

	if (m_signalMap.Lookup(key, controlID))
	{
		((CButton*)GetDlgItem(controlID))->SetCheck(atoi(value));
	}
}

void CMngSetup::saveInfo()
{
	CString file, keys, value;
	file.Format("%s\\%s\\%s\\mngsetup.ini", Axis::home, USRDIR, Axis::user);

	for (POSITION pos = m_mapMng.GetStartPosition(); pos; )
	{
		m_mapMng.GetNextAssoc(pos, keys, value);
		
		WritePrivateProfileString("Manage", keys, value, file);
	}
	
	WritePrivateProfileString("Setup", "Pos", Format("%d", m_pos), file);
	WritePrivateProfileString("Setup", "Popup", Format("%d", m_popup), file);
	WritePrivateProfileString("Setup", "Sound", Format("%d", m_sound), file);
}
/*
{ 'S', {{ 47,  "GUBN"}, { 600, "BELLCODE"    }, { 601, "KEYVALUE"    },
{ 23,  "메시지" },
047	1	600	000	601	2110600	023	선물옵션 장개시10 분전"}
XJANG	047	1	600	000	601	110300	023	주식 장개시5 분전"}
KEYVALUE : XJANG 고정

GUBN : 0:장운영정보, 1:알림
BELLCODE : wave 파일

KEYVALUE :								*/
/* janinfo.keyv = XYZnnnn						*/
/* X									*/
/*	0:주식시장      1:KOSDAQ시장    2:선물시장      3:옵션시장      */
/*      4:하이일드펀드  5:프리보드      6:채권          7:주식옵션      */
/* Y									*/
/*	if z == 0  (장운영정보)						*/
/*	1:C.B 발동	2:C.B 해제					*/
/*	if z == 1  (알림메시지)						*/
/*	1:장개시전      2:장마감        3:시간외종가    4:시간외단일가  */
/*      5:시간외단일가 매매종료         6:장전동시호가  7:장후동시호가  */
/* Z    0:cont[40]      1:xmsg[60]                                      */
/* nnnn seconds (장개시전과 장마감전 인경우에만 사용)                   */

BOOL CMngSetup::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	const UINT	nRes = LOWORD(wParam);
	CString value;
	int	chk = 0;

	int signal{};
	UINT controlID{};
	value.Empty();

	switch (nRes)
	{
	case RADIO_UPPERLEFT:
		m_pos = 0;	
		break;
	case RADIO_UPPERRIGHT:
		m_pos = 2;
		break;
	case RADIO_BOTTOMLEFT:
		m_pos = 3;	
		break;
	case RADIO_BOTTOMRIGHT:
		m_pos = 4;
		break;
	case IDC_POPUP:
		m_popup = ((CButton *)GetDlgItem(nRes))->GetCheck();
		break;
	case IDC_SOUND:
		m_sound = ((CButton *)GetDlgItem(nRes))->GetCheck();
		break;
	default:
		for (POSITION pos = m_signalMap.GetStartPosition(); pos; )
		{
			m_signalMap.GetNextAssoc(pos, signal, controlID);
		
			if (nRes == controlID)
			{
				chk = ((CButton*)GetDlgItem(nRes))->GetCheck();
				value.Format("%07d", signal);
				break;
			}
		}
		break;
	}

	if (!value.IsEmpty())
	{
		if (chk)	m_mapMng.SetAt(value, "1");
		else		m_mapMng.SetAt(value, "0");
	}
	
	return CSetupDialog::OnCommand(wParam, lParam);
}

void CMngSetup::ApplySetup()
{
	saveInfo();
}

void CMngSetup::DefaultSetup()
{
	CString file;
	file.Format("%s\\%s\\mngsetup.ini", Axis::home, MTBLDIR);

	char	ssb[1024*4];
	const DWORD ssL = GetPrivateProfileSection("Manage", ssb, sizeof(ssb), file);
	if (ssL <= 0)		return;

	m_mapMng.RemoveAll();

	CString subitem, keys, value, string = CString(ssb, ssL);
	for (; !string.IsEmpty(); )
	{
		int idx = string.Find('\0');
		if (idx == -1)	break;

		subitem  = string.Left(idx++);
		string = string.Mid(idx);

		idx = subitem.Find('=');
		if (idx == -1)	continue;

		keys = subitem.Left(idx++);
		value = subitem.Mid(idx);

		m_mapMng.SetAt(keys, value);
		setControlValue(keys, value);
	}

	m_pos = GetPrivateProfileInt("Setup", "Pos", 2, file);
	((CButton *)GetDlgItem(RADIO_UPPERLEFT))->SetCheck(0);
	((CButton *)GetDlgItem(RADIO_UPPERRIGHT))->SetCheck(0);
	((CButton *)GetDlgItem(RADIO_BOTTOMLEFT))->SetCheck(0);
	((CButton *)GetDlgItem(RADIO_BOTTOMRIGHT))->SetCheck(0);

	int	nRes{};
	if (m_pos == 0)	    	{ nRes = RADIO_UPPERLEFT;   m_pos = 0; }
	else if (m_pos == 1)	{ nRes = RADIO_UPPERRIGHT;  m_pos = 2; }
	else if (m_pos == 2)	{ nRes = RADIO_BOTTOMLEFT;  m_pos = 3; }
	else if (m_pos == 3)	{ nRes = RADIO_BOTTOMRIGHT; m_pos = 4; }
	((CButton *)GetDlgItem(nRes))->SetCheck(1);

	m_popup = GetPrivateProfileInt("Setup", "Popup", 0, file);
	((CButton *)GetDlgItem(IDC_POPUP))->SetCheck(m_popup);

	m_sound = GetPrivateProfileInt("Setup", "Sound", 0, file);
	((CButton *)GetDlgItem(IDC_SOUND))->SetCheck(m_sound);
}

BOOL CMngSetup::PreTranslateMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			pMsg->wParam = VK_TAB;
		break;
	}

	return CSetupDialog::PreTranslateMessage(pMsg);
}

void CMngSetup::InitSignalControlMap()
{
	int signals[] = {
		610000, 110000, 110600, 110300, 110060, 110010, 710000, 210000, 
		210300, 210060, 210010, 100000, 200000, 310000, 410000, 510000, 
		2610000, 2110000, 2110600, 2110300, 2110060, 2110010, 2710000, 
		2210000, 2100000, 2200000, 601196, 0
	};
	const UINT const controls[] = {
		KOSPI_BEFORE_HOGA, 	KOSPI_OPEN, KOSPI_OPEN_10M, KOSPI_OPEN_5M, KOSPI_OPEN_1M, 
		KOSPI_OPEN_10S, KOSPI_AFTER_HOGA, KOSPI_CLOSE, KOSPI_CLOSE_5M, KOSPI_CLOSE_1M, 
		KOSPI_CLOSE_10S, KOSPI_OPEN_CIRCUIT, KOSPI_CLOSE_CIRCUIT, KOSPI_OPEN_TIME, 
		KOSPI_CLOSE_TIME, KOSPI_CLOSE_TIME_ONE, FUTURE_BEFORE_HOGA, FUTURE_OPEN, 
		FUTURE_OPEN_10M, FUTURE_OPEN_5M, FUTURE_OPEN_1M, FUTURE_OPEN_10S, FUTURE_AFTER_HOGA, 
		FUTURE_CLOSE, FUTURE_OPEN_CIRCUIT, FUTURE_CLOSE_CIRCUIT, KOBA_ELW_CLOSE, 0
	};

	for (int i = 0; signals[i]; i++)
	{
		m_signalMap.SetAt(signals[i], controls[i]);
	}
}

void CMngSetup::draw_groupBox(CPaintDC *pdc, int ids)
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
	pdc->TextOut(rc.left+7, rc.top-5, data);
}

void CMngSetup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	draw_groupBox(&dc, IDC_STATIC_10);
	draw_groupBox(&dc, IDC_STATIC_11);
	draw_groupBox(&dc, IDC_STATIC_12);
	draw_groupBox(&dc, IDC_STATIC_13);
	// Do not call CSetupDialog::OnPaint() for painting messages
}
