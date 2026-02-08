// CDlgServerOrder.cpp: 구현 파일
//

#include "stdafx.h"
#include "axis.h"
#include "CDlgServerOrder.h"
#include "axMsg.hxx"
//#include "afxdialogex.h"


#include "ContGrid.h"

#include "grid/fxGrid.h"

// CDlgServerOrder 대화 상자

//IMPLEMENT_DYNAMIC(CDlgServerOrder, CDialog)

#define idGRID 9898
#define CNT_INIT_ROWS 8
#define CNT_COL	3
#define SIZE_CORRECT 17
#define GRIDH_RATIO	0.77

#define BTNGAP 3
#define ORIW 750 
#define ORIH 185

#define COL_0   0   //알림 발송시간
#define COL_1   1   //알림내용
#define COL_2   2   //세부내용


#define TM_SIEZDEF 9988
#define TM_RESTOREREC 9987
#define SERVERORDER_MSG_MAPPOP 20
#define DF_ONEBTN

CDlgServerOrder::CDlgServerOrder(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DLG_SERVERORDER, pParent)
{
	m_parent = pParent;
	m_font = std::make_unique<CFont>();
	m_font->CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("굴림체"));
}

CDlgServerOrder::~CDlgServerOrder()
{
}

void CDlgServerOrder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgServerOrder, CDialog)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_SSETUP, &CDlgServerOrder::OnBnClickedSsetup)
	ON_BN_CLICKED(IDC_ORIRECT, &CDlgServerOrder::OnBnClickedOrirect)
	ON_BN_CLICKED(IDC_SCLEAR, &CDlgServerOrder::OnBnClickedSclear)
	ON_BN_CLICKED(IDOK, &CDlgServerOrder::OnBnClickedOk)
	ON_WM_DESTROY()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgServerOrder 메시지 처리기


BOOL CDlgServerOrder::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgServerOrder::InitGrid()
{
	CString slog;
	CRect rect, clientRC;
	GetWindowRect(&clientRC);
	ScreenToClient(clientRC);

	clientRC.top = 0;
	clientRC.left = 0;
	clientRC.bottom = (int)((double)clientRC.Height() * (double)GRIDH_RATIO);
	clientRC.right = clientRC.Width();

	int	x{}, y{}, gapX{}, gapY{}, rowN{}, colN{};
	const int GAP = 10;
	CString	keys;

	CProfile profile(GetProfileFileName(pkSTInfo));
	const char* szConclusionList = "SERVERORDERLIST";

	rowN = profile.GetInt(szConclusionList, "rowN", 7);
	colN = profile.GetInt(szConclusionList, "colN", 3);

	m_pGrid.reset();

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	_tcscpy_s(lf.lfFaceName, sizeof(lf.lfFaceName), _T("굴림체"));
	lf.lfHeight = 90;
	m_gridfont.CreateFontIndirect(&lf);

	m_pGrid = std::make_unique<class CfxGrid>(&lf);
	m_pGrid->Create(clientRC, this, idGRID, GVSC_VERT | GVSC_HORZ);
	m_pGrid->Initial(CNT_INIT_ROWS, CNT_COL, 1, 0);

	CString	sAttr, value;
	CIColumn iCol;
	CDC* pDC = GetDC();
	CFont* pOldFont = pDC->SelectObject(&m_gridfont);

	CString str_tmp;
	CString stmp;

	const int	width = 0;
	GVITEM	gvitem;

	//gird header 설정
	for (int ii = 0; ii < colN; ii++)
	{
		keys.Format("title%02d", ii);
		str_tmp = profile.GetString(szConclusionList, keys, keys);


		gvitem.row = 0;
		gvitem.col = ii;
		gvitem.state = 0;
		gvitem.mask = GVMK_TEXT | GVMK_FORMAT | GVMK_FONT | GVMK_ATTR | GVMK_FGCOLOR | GVMK_BKCOLOR | GVMK_MISC;
		gvitem.format = GVFM_HEADER;
		gvitem.fgcol = PALETTERGB(255, 255, 255);
		gvitem.bkcol = PALETTERGB(0, 123, 206);

		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));
		gvitem.text = str_tmp;
		gvitem.attr = GVAT_TEXT;
		m_pGrid->SetItem(&gvitem);

	}

	//gird colmun 설정
	for (int ii = 0; ii < colN; ii++)
	{
		keys.Format("wide%02d", ii);
		str_tmp = profile.GetString(szConclusionList, keys, keys);

		const CSize sz = pDC->GetTextExtent(str_tmp);

		gvitem.state = 0;
		gvitem.mask = GVMK_FORMAT | GVMK_FONT | GVMK_ATTR | GVMK_FGCOLOR | GVMK_BKCOLOR;

		keys.Format("attr%02d", ii);
		sAttr = profile.GetString(szConclusionList, keys, keys);

		iCol.m_dAttr = GetAttrByStr(sAttr);
		iCol.m_dAttr |= IGAT_USERCOLOR;

		iCol.m_lWidth = sz.cx;
		if (iCol.m_lWidth != 0)
			iCol.m_lWidth = (int)((double)iCol.m_lWidth / (double)colN);

		if ((iCol.m_dAttr & IGAT_HIDDEN) != IGAT_HIDDEN)
		{
			iCol.m_lWidth += 5;
			gvitem.attr = GVAT_HIDDEN;
		}

		if (iCol.m_dAttr & IGAT_EDITFORMAT)
		{
			keys.Format("format%02d", ii);
			iCol.m_sEditFormat = profile.GetString(szConclusionList, keys, keys);
			gvitem.attr = GVAT_TEXT;
		}
		else
			iCol.m_sEditFormat.Empty();

		gvitem.format = GVFM_CENTER;
		if(ii == COL_1)
			gvitem.format = GVFM_LEFT;
		else if(ii == COL_2)
			gvitem.format = GVAT_HIDDEN;

		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));
		gvitem.fgcol = GetColor(69);
		if(colN == 0)
			gvitem.bkcol = PALETTERGB(255, 0, 0);
		else
			gvitem.bkcol = PALETTERGB(0, 255, 0);
		//gvitem.bkcol = PALETTERGB(255, 255, 255);
		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));
		m_pGrid->SetColumnItems(ii, &gvitem);

		m_pGrid->SetColumnWidth(ii, iCol.m_lWidth);

		m_iSumGridCol += iCol.m_lWidth;
	}
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	//grid row 설정
	CRect rec;
	m_pGrid->GetWindowRect(rec);
	ScreenToClient(rec);

	for (int ii = 0; ii < m_pGrid->GetRowCount(); ii++)
		m_pGrid->SetRowHeight(ii, 17);

	m_pGrid->SetBKSelColor(GetColor(78));
	m_pGrid->SetStepColor(1, GetColor(68), GetColor(77));
	m_pGrid->SetRowColResize(FALSE, TRUE);
	m_pGrid->SetGridFocusLine(FALSE, TRUE);
	m_pGrid->SetGridColor(PALETTERGB(192, 192, 192));
	m_pGrid->Adjust();
}

BOOL CDlgServerOrder::Init()
{
	CString slog, stmp;
	CRect rect;

	int	x{}, y{}, gapX{}, gapY{};
	const int GAP = 10;

	InitGrid();

	CRect	wRc, cRc, bRc;
	GetWindowRect(wRc);
	GetClientRect(cRc);
	gapX = wRc.Width() - cRc.Width() - GetSystemMetrics(SM_CXBORDER) * 2;
	gapY = wRc.Height() - cRc.Height() - GetSystemMetrics(SM_CXBORDER) * 2;
	m_pGrid->GetWindowRect(wRc);
	gapX += wRc.Width();
	gapY += wRc.Height();

	GetDlgItem(IDOK)->GetWindowRect(bRc);
	gapY += bRc.Height() + GAP;
	x = GetSystemMetrics(SM_CXSCREEN) - gapX;
	y = GetSystemMetrics(SM_CYSCREEN) - gapY;
	SetWindowPos(NULL, x, y, gapX, gapY, SWP_HIDEWINDOW);

	m_recOri.SetRect(x, y, x + gapX, y + gapY);
slog.Format("[conclusion] SetWindowPos  [%d][%d][%d][%d]   W=[%d]H=[%d]\n", x, y, gapX + x, gapY + y, m_recOri.Width(), m_recOri.Height());
//OutputDebugString(slog);



	GetClientRect(cRc);
	const CPoint pt = cRc.CenterPoint();

	x = pt.x - bRc.Width() * 2 - GAP * 2;
	y = wRc.Height() + GAP / 2;
	GetDlgItem(IDC_SSETUP)->SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);

	x = pt.x - bRc.Width() - GAP;
	GetDlgItem(IDC_SCLEAR)->SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);

	x = pt.x;
	GetDlgItem(IDOK)->SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);

	x = pt.x + bRc.Width() + GAP;
	GetDlgItem(IDC_ORIRECT)->SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);

	CRect btnrec, wrec;
	GetClientRect(wrec);
	GetDlgItem(IDC_SSETUP)->GetWindowRect(btnrec);
	ScreenToClient(btnrec);
	m_dBtn1RatioL = (double)btnrec.left / (double)wrec.Width();
	m_dBtn1RatioR = (double)btnrec.right / (double)wrec.Width();

	GetDlgItem(IDC_SCLEAR)->GetWindowRect(btnrec);
	ScreenToClient(btnrec);
	m_dBtn2RatioL = (double)btnrec.left / (double)wrec.Width();
	m_dBtn2RatioR = (double)btnrec.right / (double)wrec.Width();

	GetDlgItem(IDOK)->GetWindowRect(btnrec);
	ScreenToClient(btnrec);
	m_dBtn3RatioL = (double)btnrec.left / (double)wrec.Width();
	m_dBtn3RatioR = (double)btnrec.right / (double)wrec.Width();

	GetDlgItem(IDC_ORIRECT)->GetWindowRect(btnrec);
	ScreenToClient(btnrec);
	m_dBtn4RatioL = (double)btnrec.left / (double)wrec.Width();
	m_dBtn4RatioR = (double)btnrec.right / (double)wrec.Width();

	double fratio = 0;
	int sum = 0;

	for (int ii = 0; ii < m_pGrid->GetColumnCount(); ii++)
	{
		fratio = (double)(m_pGrid->GetColumnWidth(ii) / (double)(m_iSumGridCol));
		stmp.Format("%.2f", fratio);

		slog.Format("[conclusion] [%d] [%s]\n", m_pGrid->GetColumnWidth(ii), stmp);
		//OutputDebugString(slog);
		sum += m_pGrid->GetColumnWidth(ii);
		m_arr.Add(stmp);
	}
	
	GetClientRect(wrec);
	SendMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(wrec.right, wrec.bottom));  //test
	Invalidate();

	OnBnClickedOrirect();
	ShowWindow(SW_HIDE);
	LoadCondition();
	return true;
}

#pragma warning (disable : 4244)
void CDlgServerOrder::ResizeGrid()
{
	CString slog;
	double dratio{}, dwidth{};
	int ii = 0, isum = 0;;

	CRect clientRC, gridrec;
	GetWindowRect(&clientRC);

	ScreenToClient(clientRC);
	clientRC.top = 0;
	clientRC.left = 0;
	clientRC.bottom = clientRC.bottom - 42;
	clientRC.right = clientRC.Width() - 7;
	m_pGrid->MoveWindow(clientRC);
	m_pGrid->Adjust();

	gridrec = clientRC;
	for (ii = 0; ii < m_pGrid->GetColumnCount(); ii++)
	{
		dratio = atof(m_arr.GetAt(ii));
		dwidth = gridrec.Width();
		double width = dwidth * dratio;

		isum += (int)width;
		if (ii == m_pGrid->GetColumnCount() - 2 && (width > 0))
			width -= GetSystemMetrics(SM_CXVSCROLL);
		m_pGrid->SetColumnWidth(ii, (int)width);
	}

	m_pGrid->Adjust();

	CRect rec, wrec;
	GetClientRect(wrec);
	GetDlgItem(IDC_SSETUP)->GetWindowRect(rec);
	ScreenToClient(rec);
	rec.left = wrec.Width() * m_dBtn1RatioL;
	rec.right = wrec.Width() * m_dBtn1RatioR;

	rec.top = wrec.bottom - BTNGAP - rec.Height();
	rec.bottom = wrec.bottom - BTNGAP;
	GetDlgItem(IDC_SSETUP)->MoveWindow(rec);
	GetDlgItem(IDC_SSETUP)->Invalidate();

	GetDlgItem(IDC_SCLEAR)->GetWindowRect(rec);
	ScreenToClient(rec);
	rec.left = wrec.Width() * m_dBtn2RatioL;
	rec.right = wrec.Width() * m_dBtn2RatioR;
	rec.top = wrec.bottom - BTNGAP - rec.Height();
	rec.bottom = wrec.bottom - BTNGAP;

	GetDlgItem(IDC_SCLEAR)->MoveWindow(rec);
	GetDlgItem(IDC_SCLEAR)->Invalidate();

	GetDlgItem(IDOK)->GetWindowRect(rec);
	ScreenToClient(rec);
	rec.left = wrec.Width() * m_dBtn3RatioL;
	rec.right = wrec.Width() * m_dBtn3RatioR;
	rec.top = wrec.bottom - BTNGAP - rec.Height();
	rec.bottom = wrec.bottom - BTNGAP;

	GetDlgItem(IDOK)->MoveWindow(rec);
	GetDlgItem(IDOK)->Invalidate();

	GetDlgItem(IDC_ORIRECT)->GetWindowRect(rec);
	ScreenToClient(rec);
	rec.left = wrec.Width() * m_dBtn4RatioL;
	rec.right = wrec.Width() * m_dBtn4RatioR;
	rec.top = wrec.bottom - BTNGAP - rec.Height();
	rec.bottom = wrec.bottom - BTNGAP;

	GetDlgItem(IDC_ORIRECT)->MoveWindow(rec);
	GetDlgItem(IDC_ORIRECT)->Invalidate();


#ifdef DF_ONEBTN
	GetDlgItem(IDC_SSETUP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SCLEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ORIRECT)->ShowWindow(SW_HIDE);

	m_pGrid->GetWindowRect(gridrec);
	GetDlgItem(IDOK)->GetWindowRect(rec);

	int ioffsetx = gridrec.right - rec.right;
	GetDlgItem(IDOK)->GetWindowRect(rec);
	ScreenToClient(rec);
	rec.OffsetRect(ioffsetx - GetSystemMetrics(SM_CXVSCROLL), 0);
	GetDlgItem(IDOK)->MoveWindow(rec);
#endif
}
#pragma warning (default : 4244)

void CDlgServerOrder::LoadCondition()
{
	CString file, usnm = Axis::user;
	file.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, usnm, usnm);

	char buffer[128] = { 0, };
	GetPrivateProfileString("MODE", "SERVERORDER", "", buffer, 128, file);
	CString sdata, leftS, topS, rightS, bottomS;
	sdata.Format("%s", buffer);
	sdata.TrimRight();

	if (sdata.GetLength() == 0)
		SetTimer(TM_SIEZDEF, 1000, NULL);
	else
		SetTimer(TM_RESTOREREC, 1000, NULL);
}

void CDlgServerOrder::ChangePalette()
{
	//if (m_grid)	m_grid->ChangePalette();
	/*
	m_grid->m_headBgColor  = GetColor(74);	// 헤더색
	m_grid->m_headFgColor  = GetColor(76);  //RGB(90, 90, 90);
	m_grid->m_dataBgColor1 = GetColor(68);	// 1st row color
	m_grid->m_dataBgColor2 = GetColor(77);	// 2st row color
	m_grid->m_lineColor    = GetColor(75);
	m_grid->m_dataFgColor  = GetColor(69);
	m_grid->m_selBgColor   = GetColor(78);	// select row color
	m_grid->Invalidate();
	*/

	CString slog;
	slog.Format("[conclusion] changepallet \n");
	//	OutputDebugString(slog);
	/*
		GVITEM	item;
		item.row = 1;

		for (int ii = 0; ii < m_pGrid->GetColumnCount(); ii++)
		{
			item.col    = ii;
			m_pGrid->GetItem(&item);
			item.bkcol =GetColor(74);
			m_pGrid->SetItem(&item);
		}

		m_pGrid->Adjust();
		*/

}

DWORD CDlgServerOrder::GetAttrByStr(CString sAttr)
{
	sAttr.Replace("IGAT_COMMA", "1");
	sAttr.Replace("IGAT_CONTRAST", "2");
	sAttr.Replace("IGAT_CONDITION", "4");
	sAttr.Replace("IGAT_CONDITIONx", "8");
	sAttr.Replace("IGAT_ZEROSUP", "10");
	sAttr.Replace("IGAT_ZEROSUPx", "20");
	sAttr.Replace("IGAT_EDITFORMAT", "40");
	sAttr.Replace("IGAT_MOUSELINE", "1000");
	sAttr.Replace("IGAT_MOUSEHAND", "2000");
	sAttr.Replace("IGAT_USERIMAGE", "4000000");
	sAttr.Replace("IGAT_USERCOLOR", "8000000");
	sAttr.Replace("IGAT_LEFT", "10000000");
	sAttr.Replace("IGAT_CENTER", "20000000");
	sAttr.Replace("IGAT_RIGHT", "40000000");
	sAttr.Replace("IGAT_HIDDEN", "80000000");

	DWORD dwAttr = 0;
	while (!sAttr.IsEmpty())
	{
		CString sVal = parseX(sAttr, "|");
		dwAttr |= _httoi(sVal);
	}
	return dwAttr;
}

COLORREF CDlgServerOrder::GetColor(int index)
{
	int		pos{};
	CString		file, tmpS, dat, section, key;
	CStringArray	ary;

	CProfile profile(GetProfileFileName(pkPalette));
	section = profile.GetString(GENERALSN, "Palette", "Palette");

	key.Format("RGB%03d", index);
	dat = profile.GetString(section, key);
	if (dat.IsEmpty())
	{
		if (!section.Compare("Palette"))
			return 0;
		section = "Palette";
		dat = profile.GetString(section, key);
		if (dat.IsEmpty())
			return 0;
	}
	for (; !dat.IsEmpty();)
	{
		pos = dat.Find(',');
		if (pos == -1)
		{
			ary.Add(dat);
			break;
		}
		tmpS = dat.Left(pos++);
		dat = dat.Mid(pos);
		ary.Add(tmpS);
	}

	if (ary.GetSize() < 3)
		return RGB(215, 215, 215);

	return RGB(atoi(ary.GetAt(0)), atoi(ary.GetAt(1)), atoi(ary.GetAt(2)));
}

CString CDlgServerOrder::parseX(CString& srcstr, CString substr)
{
	const int nFind = srcstr.Find(substr);
	if (nFind == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(nFind);
		srcstr = srcstr.Mid(nFind + substr.GetLength());
		return temp;
	}
}

#pragma warning(disable : 26408)
int CDlgServerOrder::_httoi(const TCHAR* value)
{
	struct CHexMap
	{
		TCHAR chr;
		int value;
	};
	const int HexMapL = 16;
	const CHexMap HexMap[HexMapL] =
	{
		{'0', 0}, {'1', 1},
		{'2', 2}, {'3', 3},
		{'4', 4}, {'5', 5},
		{'6', 6}, {'7', 7},
		{'8', 8}, {'9', 9},
		{'A', 10}, {'B', 11},
		{'C', 12}, {'D', 13},
		{'E', 14}, {'F', 15}
	};
	TCHAR* mstr = _tcsupr(_tcsdup(value));
	TCHAR* ss = mstr;
	int result = 0;
	if (*ss == '0' && *(ss + 1) == 'X') ss += 2;
	bool firsttime = true;
	while (*ss != '\0')
	{
		bool found = false;
		for (int i = 0; i < HexMapL; i++)
		{
			if (*ss == HexMap[i].chr)
			{
				if (!firsttime) result <<= 4;
				result |= HexMap[i].value;
				found = true;
				break;
			}
		}
		if (!found) break;
		ss++;
		firsttime = false;
	}
	free(mstr);
	return result;
}
#pragma warning(default : 26408)

void CDlgServerOrder::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case TM_RESTOREREC:
	{
		KillTimer(nIDEvent);

		CString file, usnm = Axis::user;
		file.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, usnm, usnm);
		char buffer[128] = { 0, };
		GetPrivateProfileString("MODE", "SERVERORDER", "", buffer, 128, file);
		CString sdata, leftS, topS, rightS, bottomS;
		sdata.Format("%s", buffer);
		sdata.TrimRight();

		leftS = Parser(sdata, "|");
		topS = Parser(sdata, "|");
		rightS = Parser(sdata, "|");
		bottomS = Parser(sdata, "|");
		m_recDef.left = m_recDef.right - (atoi(rightS) - atoi(leftS));
		m_recDef.top = m_recDef.bottom - (atoi(bottomS) - atoi(topS));

		CString slog;
		slog.Format("[conclusion] ontimer [%d][%d][%d][%d]   [%d][%d]\n", m_recDef.left, m_recDef.top, m_recDef.right, m_recDef.bottom, m_recDef.Width(), m_recDef.Height());
		//OutputDebugString(slog);

		MoveWindow(m_recDef);
		ResizeGrid();
		ShowWindow(SW_HIDE);
		m_parent->SendMessage(WM_USER, MMSG_RESTORESERVERORD, SERVERORDER_MSG_MAPPOP);   //ref 서버주문 리스트
		ShowWindow(SW_HIDE);

	}
	break;
	case TM_SIEZDEF:
	{
		KillTimer(nIDEvent);
		m_recDef.left = 0;
		m_recDef.right = ORIW;
		m_recDef.top = 0;
		m_recDef.bottom = ORIH;

		CString slog;
		slog.Format("[conclusion] ontimer init [%d][%d][%d][%d]   [%d][%d]\n", m_recDef.left, m_recDef.top, m_recDef.right, m_recDef.bottom, m_recDef.Width(), m_recDef.Height());
		//OutputDebugString(slog);

		MoveWindow(m_recDef);
		ResizeGrid();
		ShowWindow(SW_HIDE);
		m_parent->SendMessage(WM_USER, MMSG_RESTORESERVERORD, SERVERORDER_MSG_MAPPOP);   //ref 서버주문 리스트
		ShowWindow(SW_HIDE);
	}
	break;
	}
	CDialog::OnTimer(nIDEvent);
}

CString CDlgServerOrder::Parser(CString& srcstr, CString substr)
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

void CDlgServerOrder::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_pGrid != NULL && m_arr.GetSize() > 0)
	{
		ResizeGrid();
		m_pGrid->Adjust();
		Invalidate();
	}
}


void CDlgServerOrder::OnBnClickedSsetup()
{
	//화면 띄우기
}


void CDlgServerOrder::OnBnClickedOrirect()
{
	SetWindowPos(NULL, m_recOri.left, m_recOri.top, ORIW, ORIH, NULL);
	ResizeGrid();
	ShowWindow(SW_HIDE);
	m_parent->SendMessage(WM_USER, MMSG_RESTORESERVERORD, SERVERORDER_MSG_MAPPOP);
}


void CDlgServerOrder::OnBnClickedSclear()
{
	m_pGrid->Clear();

	int nRowCnt = m_pGrid->GetRowCount();
	while (nRowCnt > CNT_INIT_ROWS)
		m_pGrid->DeleteRow(--nRowCnt);

	m_pGrid->Adjust();
}


void CDlgServerOrder::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnOK();
}


void CDlgServerOrder::OnDestroy()
{
	CDialog::OnDestroy();

	CString  sdata;
	CString file, usnm = Axis::user;
	CRect rec;

	GetWindowRect(rec);
	sdata.Format("%d|%d|%d|%d", rec.left, rec.top, rec.right, rec.bottom);

	file.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, usnm, usnm);
	WritePrivateProfileString("MODE", "SERVERORDER", sdata, file);
}


BOOL CDlgServerOrder::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (wParam == idGRID)
	{
		CString stemp;
		CString slog;

		NM_GVITEM* nmgv = (NM_GVITEM*)lParam;

		const int imessae = nmgv->hdr.code;

		if (imessae != 5)
			return 0;


		const int nRow = nmgv->row;
		const int nCol = nmgv->col;

		if (nCol != COL_1)
			return 0;
		/*
1 = 주식자동주문 서비스 신청  	; 신청 시점
2 = 주식자동주문 서비스 폐지	; 해지 시점	
3 = 자동주문 조건 만족 시 (매도)	; 조건만족 시점
4 = 자동주문 조건 만족 시 (매수)	; 조건만족 시점
5 = 자동주문 조건 만족 시 (신규편입매도)	; 조건만족 시점
6 = 조건만료 시(기간만료, 권리락 등으로 인한 조건만료)	; 감시조건 일 배치 시점(만요일 익일 08:00)
7 = 감시 중인 내역이 1건 이상	; 주 1회 (월요일 오전 08:20)

1 = 주식자동주문 서비스가 신청되었습니다
2 = 주식자동주문 서비스가 해지되었습니다.
3 = 종목명 주식자동주문(매도) 감시조건을 충족했습니다. 주문내역을 확인해주세요 [확인]/주문내역보기
4 = 종목명 주식자동주문(매수) 감시조건을 충족했습니다. 주문내역을 확인해주세요 [확인]/주문내역보기
5 = 종목명 주식자동주문(신규편입매도) 감시조건을 충족했습니다. 주문내역을 확인해주세요 [확인]/주문내역보기
6 = 주식자동주문 감시 만료가 발생했습니다(N건) HTS/MTS에서 감시내역을 확인해 주세요 [확인]/감시내역보기
7 = 주식자동주문 감시 현황 입니다 (매수감시 N건 /매도감시 N건 / 신규잔고편입매도 N건)
		*/
		CString sval;
		sval = m_pGrid->GetItemText(nRow, nCol);

		int igubn = atoi(m_pGrid->GetItemText(nRow, COL_2));
		switch(igubn)
		{
			case 1:   //서비스 신청
			{

			}
			break;
			case 2:  //서비스 해지
			{

			}
			break;
			case 3:  //(매도) 감시조건을 충족  /주문내역보기
			{
				m_parent->SendMessage(WM_USER, MMSG_RESTORESERVERORD, igubn);
			}
			break;
			case 4: //(매수) 감시조건을 충족  /주문내역보기
			{
				m_parent->SendMessage(WM_USER, MMSG_RESTORESERVERORD, igubn);
			}
			break;
			case 5: //(신규편입매도) 감시조건을 충족  /감시내역보기
			{
				m_parent->SendMessage(WM_USER, MMSG_RESTORESERVERORD, igubn);
			}
			break;
			case 6: //만료가 발생했습니다
			{
				m_parent->SendMessage(WM_USER, MMSG_RESTORESERVERORD, igubn);
			}
			break;
			case 7:  //주식자동주문 감시 현황 입니다
			{

			}
			break;
			case 8:  //착오주문
			{
				m_parent->SendMessage(WM_USER, MMSG_RESTORESERVERORD, igubn);
			}
			break;
		}
	}

	return CDialog::OnNotify(wParam, lParam, pResult);
}

void	CDlgServerOrder::AddServerOrd(CString dat, int max)
{
	CString slog;
	slog.Format("[conclusion] [%s][%d]\n", dat, m_pGrid->GetRowCount());
	//OutputDebugString(slog);
	CString tmpS, tmp;
	tmpS = dat;

	if (dat.IsEmpty())	return ;
	if (m_pGrid)
	{
		CString sTmp = dat, sOrdNo = "", stime = "", sdata = "", sdetail = "";
		stime = parseX(dat, "\t"); stime.TrimRight();

		if (!stime.IsEmpty())
			stime.Format("%s.%s.%s %s:%s:%s", stime.Left(4), stime.Mid(4, 2), stime.Mid(6,  2), stime.Mid(8, 2), stime.Mid(10, 2), stime.Mid(12, 2));

		sdata = parseX(dat, "\t");
		sdata.Replace("||", "");
		sdetail = parseX(dat, "\t");
	
		const int iheight = m_pGrid->GetRowHeight(1);
		m_pGrid->InsertRow("", 1);
		m_pGrid->SetRowHeight(1, iheight);

		CString stmp;
		for (int ii = 0; ii < m_pGrid->GetColumnCount(); ii++)
		{
			if (ii == COL_0)
				m_pGrid->SetItemText(1, ii, stime);
			else if (ii == COL_1)
				m_pGrid->SetItemText(1, ii, sdata);
			else if (ii == COL_2)
				m_pGrid->SetItemText(1, ii, sdetail);
		}

		int nRowCnt = m_pGrid->GetRowCount();

		while (max > 0 && nRowCnt > max)
			m_pGrid->DeleteRow(--nRowCnt);

		m_pGrid->Adjust();
	}
}

void CDlgServerOrder::OnPaint()
{
	CPaintDC dc(this); 

	/*if (m_pGrid)
	{
		CRect rec;
		m_pGrid->GetWindowRect(rec);
		ScreenToClient(rec);

		rec.top = rec.bottom + 5;
		rec.bottom = rec.top + 15;
		dc.SetBkMode(TRANSPARENT);
		CFont* pOldFont = dc.SelectObject(m_font.get());
		dc.DrawText(_T(" ※ 최근 30일간 알림 수신 내역의 최대 100개까지 조회되요."), -1, rec,  DT_LEFT);
		dc.SelectObject(pOldFont);
	}*/
}
