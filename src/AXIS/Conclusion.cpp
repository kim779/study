// Conclusion.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "Conclusion.h"

#include "ContGrid.h"
#include "axMsg.hxx"

#include "grid/fxGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConclusion dialog

#define idGRID 9898
#define CNT_INIT_ROWS 8
#define CNT_COL	11
#define SIZE_CORRECT 17
#define GRIDH_RATIO	0.77

#define COL_0   0   //체결시각
#define COL_1   1   //주문#
#define COL_2   2   //원주문
#define COL_3   3   //거래소 구분
#define COL_4   4   //계좌번호
#define COL_5   5   //고객명
#define COL_6   6   //종목명 
#define COL_7   7   //구분 
#define COL_8   8   //수량 
#define COL_9   9   //가격
#define COL_10   10   //미체결

#define TM_RESTOREREC 9797
#define TM_SIEZDEF 9796

#define BTNGAP 3
#define ORIW 590  //590
#define ORIH 185

#define DF_STR_CHETIME  "체결시각"   // "체결시간" 
#define DF_STR_ORDERNUM  "주문번호"   // "주문#" 

CConclusion::CConclusion(CWnd* pParent /*=NULL*/)
	: CDialog(CConclusion::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConclusion)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_parent = pParent;
	m_grid = NULL;
	m_brush.CreateSolidBrush(RGB(238, 238, 238));

	m_pGrid = NULL;
	m_dHeightRatio = 0;
	m_binit = 0;
}

CConclusion::~CConclusion()
{
if (m_grid)
	{
		m_grid->DestroyWindow();
#ifndef DF_USE_CPLUS17
		delete m_grid;
#endif
	}
	m_brush.DeleteObject();

	if (m_pGrid)
	{
		m_pGrid->DestroyWindow();
#ifndef DF_USE_CPLUS17
		delete m_pGrid;
#endif
	}
}


void CConclusion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConclusion)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConclusion, CDialog)
	//{{AFX_MSG_MAP(CConclusion)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_OSETUP, OnOsetup)   
	ON_BN_CLICKED(IDC_ORIREC, OnOriRec)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConclusion message handlers

BOOL CConclusion::OnInitDialog() 
{
	CDialog::OnInitDialog();

CString slog;
slog.Format("[conclusion] oninitdialog  ------------------------------------------------");
//OutputDebugString(slog);
//	LoadCondition();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConclusion::LoadCondition()
{
	CString file, usnm = Axis::user;
	file.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, usnm, usnm); 
	
	char buffer[128] = {0,};
	GetPrivateProfileString("MODE", "CONCLUSION", "", buffer, 128, file);
	CString sdata, leftS, topS, rightS, bottomS;
	sdata.Format("%s", buffer);
	sdata.TrimRight();
	
	if(sdata.GetLength() == 0)
		SetTimer(TM_SIEZDEF, 1000, NULL);
	else
		SetTimer(TM_RESTOREREC, 1000, NULL);
}

bool CConclusion::Init()
{
CString slog;

	CRect rect, clientRC;
	GetWindowRect(&clientRC);
	ScreenToClient(clientRC);

slog.Format("[conclusion] Init start 전체DLG  [%d][%d][%d][%d]   [%d][%d]\n", clientRC.left, clientRC.top, clientRC.right, clientRC.bottom, clientRC.Width(), clientRC.Height());
//OutputDebugString(slog);


	clientRC.top = 0;
	clientRC.left = 0;
	clientRC.bottom = (int)((double)clientRC.Height() * (double)GRIDH_RATIO);
	clientRC.right = clientRC.Width();


	int	x{}, y{}, gapX{}, gapY{}, rowN{}, colN{};
	const int GAP = 10;
	CString	keys;

	CProfile profile(GetProfileFileName(pkSTInfo));   //mtbl

	const char* szConclusionList = "CONCLUSIONLIST";

	rowN = profile.GetInt(szConclusionList, "rowN", 4);
	colN = profile.GetInt(szConclusionList, "colN", 7);

#ifdef DF_USE_CPLUS17
	m_pGrid.reset();
#else
	if(m_pGrid != NULL)
		delete m_pGrid;
#endif

	LOGFONT lf; 
	memset(&lf, 0, sizeof(LOGFONT));
	_tcscpy_s(lf.lfFaceName, sizeof(lf.lfFaceName),_T("굴림체"));
	lf.lfHeight = 90;
	m_font.CreateFontIndirect(&lf);

#ifdef DF_USE_CPLUS17
	m_pGrid = std::make_unique<class CfxGrid>(&lf);
#else
	m_pGrid = new CfxGrid(&lf); 
#endif
	m_pGrid->Create(clientRC, this, idGRID, GVSC_VERT|GVSC_HORZ);
	m_pGrid->Initial(CNT_INIT_ROWS, CNT_COL, 1, 0);

	CString	sAttr, value;
	long lSumWidth = 0;
	const long lHeight = rowN * 18;

	CIColumn iCol;
	CDC *pDC = GetDC();
	CFont *pOldFont = pDC->SelectObject(&m_font);

	CString str_tmp;
	CString stmp;

	const int	width = 0;
	GVITEM	gvitem;
	
	for (int ii = 0; ii < colN; ii++)
	{
		keys.Format("title%02d", ii);
		str_tmp = profile.GetString(szConclusionList, keys, keys);
		if (str_tmp == "시간") str_tmp = DF_STR_CHETIME;
		if (str_tmp == "주문#") str_tmp = DF_STR_ORDERNUM;
//		if (str_tmp == "가격") str_tmp = " 가격 ";
//		if (str_tmp == "종목명") str_tmp = " 종목명 ";

		gvitem.row   = 0;
		gvitem.col   = ii;
		gvitem.state = 0;
		gvitem.mask  = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_MISC;
		gvitem.format= GVFM_HEADER;
		gvitem.fgcol = PALETTERGB(  255,   255,   255);
		gvitem.bkcol = PALETTERGB(0, 123, 206);
		
		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));	
		gvitem.text  = str_tmp;
		gvitem.attr  = GVAT_TEXT;
		m_pGrid->SetItem(&gvitem);
/*
		keys.Format("wide%02d", ii);
	
		CSize sz = pDC->GetTextExtent(profile.GetString(szConclusionList, keys, keys));
slog.Format("[conclusion] [%s] [%d] [%d] [%d]\n", profile.GetString(szConclusionList, keys, keys), sz.cx , (int)((double)sz.cx / (double)10), (int)(((double)sz.cx / (double)10) + 35));
//OutputDebugString(slog);
		width = sz.cx;
		if(width != 0)
			width = (int)((double)width / (double)10 + SIZE_CORRECT);
*/
	}	


	for (int ii = 0; ii < colN; ii++)
	{
		keys.Format("title%02d", ii);
		str_tmp = profile.GetString(szConclusionList, keys, keys);
		if (str_tmp == "시간") str_tmp = DF_STR_CHETIME;
		if (str_tmp == "주문#") str_tmp = DF_STR_ORDERNUM;
		iCol.m_sHeadCaption = str_tmp;
		
		keys.Format("wide%02d", ii);
		str_tmp = profile.GetString(szConclusionList, keys, keys);

	//	if(ii == COL_6 || ii == COL_0 || ii == COL_4)
		if ( ii == COL_0 )
			str_tmp += "  ";

		if (  ii == COL_4)
			str_tmp += "    ";

#ifndef DF_MK_CAPTION
		if (ii == COL_3)
		{
			str_tmp += "   ";
		}
#endif
		const CSize sz = pDC->GetTextExtent(str_tmp);

		gvitem.state = 0;
		gvitem.mask  = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR;

		keys.Format("attr%02d", ii);
		sAttr = profile.GetString(szConclusionList, keys, keys);

		iCol.m_dAttr = GetAttrByStr(sAttr);
		iCol.m_dAttr |= IGAT_USERCOLOR;

		iCol.m_lWidth = sz.cx;
		if(iCol.m_lWidth != 0)
			iCol.m_lWidth =  (int)((double)iCol.m_lWidth / (double)10 + SIZE_CORRECT);

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
		if(ii == COL_1 || ii == COL_8 || ii == COL_9 || ii == COL_10)
			gvitem.format = GVFM_RIGHT;
		else if(ii == COL_4 || ii == COL_5 || ii == COL_6 || ii == COL_9)
			gvitem.format = GVFM_LEFT;

		if(ii == COL_7 )
			gvitem.attr = GVAT_CONDITIONx;

		if(ii == COL_9 )
			gvitem.attr = gvitem.attr | GVAT_COMMA;

		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));
		gvitem.fgcol = GetColor(69);
		gvitem.bkcol = PALETTERGB(255, 255, 255);
		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));	
		m_pGrid->SetColumnItems(ii, &gvitem);

		m_pGrid->SetColumnWidth(ii, iCol.m_lWidth);

		lSumWidth += iCol.m_lWidth;	
	}
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

//slog.Format("[conclusion] lSumWidth = [%d] \n", lSumWidth);
//OutputDebugString(slog);

	for (int ii = 0; ii < m_pGrid->GetRowCount(); ii++)
		m_pGrid->SetRowHeight(ii, 17);

	m_pGrid->SetBKSelColor(GetColor(78));
	m_pGrid->SetStepColor(1, GetColor(68), GetColor(77));
	m_pGrid->SetRowColResize(FALSE, TRUE);
	m_pGrid->SetGridFocusLine(FALSE, TRUE);
	m_pGrid->SetGridColor(PALETTERGB(192, 192, 192));
	m_pGrid->Adjust();

slog.Format("[conclusion] init lSumWidth=[%d] SM_CXVSCROLL=[%d]  ", lSumWidth, GetSystemMetrics(SM_CXVSCROLL) );
//OutputDebugString(slog);

	CRect	wRc, cRc, bRc;

	GetWindowRect(wRc);
	GetClientRect(cRc);
	gapX = wRc.Width() - cRc.Width() - GetSystemMetrics(SM_CXBORDER)*2;
	gapY = wRc.Height() - cRc.Height() - GetSystemMetrics(SM_CXBORDER)*2;

	m_pGrid->GetWindowRect(wRc);

	gapX += wRc.Width();
	gapY += wRc.Height();

	GetDlgItem(IDOK)->GetWindowRect(bRc);
	CRect tmprec;
	GetDlgItem(IDOK)->GetWindowRect(tmprec);
	ScreenToClient(tmprec);
	
	gapY += bRc.Height() + GAP;
	x = GetSystemMetrics(SM_CXSCREEN) - gapX;
	y = GetSystemMetrics(SM_CYSCREEN) - gapY;
	SetWindowPos(NULL, x, y, gapX, gapY, SWP_HIDEWINDOW);

m_recOri.SetRect(x,y,x+gapX, y+gapY);
slog.Format("[conclusion] SetWindowPos  [%d][%d][%d][%d]   W=[%d]H=[%d]\n", x, y, gapX + x , gapY + y,  m_recOri.Width(), m_recOri.Height());
//OutputDebugString(slog);

	GetClientRect(cRc);
	const CPoint pt= cRc.CenterPoint();
	
	x = pt.x - bRc.Width()*2 - GAP*2;
	y = wRc.Height() + GAP/2;
	GetDlgItem(IDC_OSETUP)->SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW);
	
	x = pt.x - bRc.Width() - GAP;
	GetDlgItem(IDC_CLEAR)->SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW);
	
	x = pt.x ;
	GetDlgItem(IDOK)->SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW);
	
	x = pt.x + bRc.Width() + GAP;
	GetDlgItem(IDC_ORIREC)->SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW);

	CRect btnrec, wrec;
	GetClientRect(wrec);
	GetDlgItem(IDC_OSETUP)->GetWindowRect(btnrec);
	ScreenToClient(btnrec);
	m_dBtn1RatioL = (double)btnrec.left / (double)wrec.Width();
	m_dBtn1RatioR = (double)btnrec.right / (double)wrec.Width();

	GetDlgItem(IDC_CLEAR)->GetWindowRect(btnrec);
	ScreenToClient(btnrec);
	m_dBtn2RatioL = (double)btnrec.left / (double)wrec.Width();
	m_dBtn2RatioR = (double)btnrec.right / (double)wrec.Width();

	GetDlgItem(IDOK)->GetWindowRect(btnrec);
	ScreenToClient(btnrec);
	m_dBtn3RatioL = (double)btnrec.left / (double)wrec.Width();
	m_dBtn3RatioR = (double)btnrec.right / (double)wrec.Width();

	GetDlgItem(IDC_ORIREC)->GetWindowRect(btnrec);
	ScreenToClient(btnrec);
	m_dBtn4RatioL = (double)btnrec.left / (double)wrec.Width();
	m_dBtn4RatioR = (double)btnrec.right / (double)wrec.Width();
	
	CRect rec;
	m_pGrid->GetWindowRect(rec);
	ScreenToClient(rec);

	double fratio = 0;
	GetWindowRect(clientRC);
	const int totalwidth = clientRC.Width() ;
	int sum = 0;
	for (int ii = 0; ii < m_pGrid->GetColumnCount(); ii++)
	{
		fratio = (double)(m_pGrid->GetColumnWidth(ii)/(double)(totalwidth - GetSystemMetrics(SM_CXVSCROLL))) ;
		stmp.Format("%.2f", fratio);

slog.Format("[conclusion] [%d] [%s]\n", m_pGrid->GetColumnWidth(ii), stmp);
//OutputDebugString(slog);
		sum += m_pGrid->GetColumnWidth(ii);
		m_arr.Add(stmp);
	}

slog.Format("[conclusion] totalwidth=[%d] sum=[%d]\n", totalwidth, sum);
//OutputDebugString(slog);

	m_dHeightRatio = (double)18 / (double)rec.Height();
	m_dHeightRatio = 1;

	m_pGrid->GetWindowRect(rect);
	GetWindowRect(clientRC);
	ScreenToClient(rect);
	m_dGridLRadio = (double)rect.left / (double)clientRC.Width();
	m_dGridTRadio = (double)rect.top / (double)clientRC.Height();
	m_dGridRRadio = (double)rect.right / (double)clientRC.Width();
	m_dGridBRadio = (double)rect.bottom / (double)clientRC.Height();

	GetClientRect(wrec);
	SendMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(wrec.right, wrec.bottom));  //test
	Invalidate();

slog.Format("[conclusion] m_dHeightRatio=[%f]\n", m_dHeightRatio);
//OutputDebugString(slog);
	ChangePalette();
	
	OnOriRec();
	ShowWindow(SW_HIDE);
	LoadCondition();
	return true;
}

void CConclusion::ChangePalette()
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

BOOL CConclusion::AddConclusion(CString dat, int max, bool bConclusion)
{
CString slog;
slog.Format("[conclusion] [%s][%d]\n", dat, m_pGrid->GetRowCount());
//OutputDebugString(slog);
CString tmpS, tmp;
	tmpS = dat;

	if (dat.IsEmpty()) return FALSE;
	if (m_pGrid)
	//	return m_grid->AddData(dat, max, bConclusion);
	{
		CString sTmp = dat, sOrdNo = "", stime = "", sjum = "", sacc = "", smkgb = "";
		stime = parseX(dat, "\t"); stime.TrimRight();

		if(!stime.IsEmpty())
			stime.Format("%s:%s:%s", stime.Left(2), stime.Mid(2,2), stime.Mid(4,2));

		sjum = parseX(dat, "\t");
		sOrdNo = parseX(dat, "\t");
		smkgb = parseX(dat, "\t");
		sacc = parseX(dat, "\t");
		sacc.Format("%s-%s-%s", sacc.Left(3), sacc.Mid(3,2), sacc.Right(6));
		sOrdNo = stime + " " + sjum;
slog.Format("[conclusion] sOrdNo = [%s]\n", sOrdNo);
//OutputDebugString(slog);	
	/*	
		if (!sOrdNo.IsEmpty())
		{
			int nRow = FindColData(sOrdNo);	// 체결시각(0), 주문번호(1)
			if (nRow >= 0)
				m_pGrid->DeleteRow(nRow);
		}
	*/
		const int iheight = m_pGrid->GetRowHeight(1);
		m_pGrid->InsertRow("",1);
		m_pGrid->SetRowHeight(1, iheight);

		/*
		int irow = 0;
		for(int ii = 1 ; ii <= m_pGrid->GetRowCount() ; ii++)
		{
			tmpS = m_pGrid->GetItemText(ii, COL_3);
			tmpS.TrimRight();
			if(tmpS.IsEmpty())
			{
				irow = ii;
				slog.Format("[conclusion] data = [%s] irow=[%d] cnt=[%d]\n", m_pGrid->GetItemText(ii - 1, COL_3), irow, m_pGrid->GetRowCount());
				OutputDebugString(slog);	
				break;
			}
		}

		if(irow < CNT_INIT_ROWS )
		{
slog.Format("[conclusion] delete irow=[%d] [%d]\n", irow, m_pGrid->GetRowCount() - 1);
OutputDebugString(slog);
			m_pGrid->DeleteRow(m_pGrid->GetRowCount() - 1);
		}
		*/
		
		/*
		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));
		int igridheigth = m_pGrid->GetRowHeight(1);
		lf.lfHeight = m_pGrid->GetRowHeight(1) * 6;
		_tcscpy(lf.lfFaceName, _T("굴림체"));
		CFont font;
		font.CreateFontIndirect(&lf);
		
		for(int j = 0; j < m_pGrid->GetColumnCount(); j++)
		{	
			GVITEM		gvitem;
			CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));
			m_pGrid->SetItemFont(1, j, &lf);	
		}
		*/

		CString stmp;
		for(int ii = 0 ; ii < m_pGrid->GetColumnCount(); ii++)
		{
			if(ii == COL_0)
				m_pGrid->SetItemText(1, ii, stime);
			else if(ii == COL_1)
				m_pGrid->SetItemText(1, ii, sjum);
			else if(ii == COL_2)
				m_pGrid->SetItemText(1, ii, "");
			else if (ii == COL_3)
				m_pGrid->SetItemText(1, ii, "");  //smkgb  
			else if(ii == COL_4)
				m_pGrid->SetItemText(1, ii, sacc);
			else
			{
				stmp = parseX(dat, "\t");
				m_pGrid->SetItemText(1, ii, stmp);
			}
		}

		int nRowCnt = m_pGrid->GetRowCount();
	
		while (max > 0 && nRowCnt > max)
			m_pGrid->DeleteRow(--nRowCnt);

		m_pGrid->Adjust();

		return TRUE;
	}
	return FALSE;
}

BOOL CConclusion::AddFail(CString dat, int max)
{
CString slog;
slog.Format("[conclusion] AddFail [%s] [%d]\n", dat, max);
//OutputDebugString(slog);

	if (dat.IsEmpty())	return FALSE;
	if (m_pGrid)
		//return m_grid->AddFail(dat, max);
	{
		CString sTmp = dat, sOrdNo = "", stime = "", sjum = "", sacc = "", smkgb = "";
		stime = parseX(dat, "\t");
		stime.TrimRight();

		if(!stime.IsEmpty())
			stime.Format("%s:%s:%s", stime.Left(2), stime.Mid(2,2), stime.Mid(4,2));
		
		sjum = parseX(dat, "\t");
		sOrdNo = parseX(dat, "\t");
		smkgb = parseX(dat, "\t");
		sacc = parseX(dat, "\t");
		sacc.Format("%s-%s-%s", sacc.Left(3), sacc.Mid(3,2), sacc.Right(6));
	
		sOrdNo = stime + " " + sjum;
		/*
		if (!sOrdNo.IsEmpty())
		{	
			int nRow = FindColData(sOrdNo);	// 체결시각(0), 주문번호(1)
			if (nRow >= 0)
				m_pGrid->DeleteRow(nRow);
		}
		*/
		const int iheight = m_pGrid->GetRowHeight(1);
		m_pGrid->InsertRow("", 1);
		m_pGrid->SetRowHeight(1, iheight);
		/*
		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));
		int igridheigth = m_pGrid->GetRowHeight(1);
		lf.lfHeight = m_pGrid->GetRowHeight(1) * 6;
		_tcscpy(lf.lfFaceName, _T("굴림체"));
		CFont font;
		font.CreateFontIndirect(&lf);
		
		for(int j = 0; j < m_pGrid->GetColumnCount(); j++)
		{	
			GVITEM		gvitem;
			CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));
			m_pGrid->SetItemFont(1, j, &lf);	
		}
		*/
		CString stmp;
		for(int ii = 0 ; ii < m_pGrid->GetColumnCount(); ii++)
		{
			if(ii == COL_0)
				m_pGrid->SetItemText(1, ii, stime);
			else if(ii == COL_1)
				m_pGrid->SetItemText(1, ii, sjum);
			else if(ii == COL_2)
				m_pGrid->SetItemText(1, ii, "");
			else if (ii == COL_3)
				m_pGrid->SetItemText(1, ii, smkgb);
			else if(ii == COL_4)
				m_pGrid->SetItemText(1, ii, sacc);
			else if(ii == COL_7)
			{	
				stmp = parseX(dat, "\t");	
				stmp.TrimLeft();
				stmp.TrimRight();
				if(stmp.Find("매도") >=0)
					stmp = "-" + stmp;
				else if(stmp.Find("매수") >= 0)
					stmp = "+" + stmp;

				m_pGrid->SetItemText(1, ii, stmp);
			}
			else
			{
				stmp = parseX(dat, "\t");
				stmp.TrimLeft();
				stmp.TrimRight();
				m_pGrid->SetItemText(1, ii, stmp);
			}
		}
		
		int nRowCnt = m_pGrid->GetRowCount();
	
		while (max > 0 && nRowCnt > max)
			m_pGrid->DeleteRow(--nRowCnt);
		
		m_pGrid->Adjust();

		return TRUE;
	}
	return FALSE;
}


void CConclusion::OnClear()
{
	//if (m_grid)	m_grid->AddData();
	//if (m_grid)	m_grid->ResetContent();
//	if(m_pGrid) m_pGrid->DeleteAllItems();
	m_pGrid->Clear();

	int nRowCnt = m_pGrid->GetRowCount();
	while ( nRowCnt > CNT_INIT_ROWS)
		m_pGrid->DeleteRow(--nRowCnt);

	m_pGrid->Adjust();
}

void CConclusion::OnOsetup() 
{
	/*
	CTime now = CTime::GetCurrentTime();
	CString yymmdd;
	yymmdd.Format("%d%d%d", now.GetHour(), now.GetMinute(), now.GetSecond());
	yymmdd += "\t25\t0\t00110011601\t테스트계좌\t한창\t-거부\t1\t1090\t0";
	AddConclusion(yymmdd, 300, true);
	*/
//	AddConclusion("111230\t150\t140\t222345-11\t이원재\t테스트\t+매수\t30\t3000", 100, true);
	m_parent->SendMessage(WM_COMMAND, MAKEWPARAM(ID_APP_ORDERSETUP, 0), 0);
	SetFocus();
}

HBRUSH CConclusion::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	hbr = m_brush;	
	return hbr;
}

void CConclusion::OnTimer(UINT nIDEvent) 
{
	//AddConclusion("11:12:30\t150\t140\t222345-11\t이원재\t테스트\t+매수\t30\t3000", 100, true);

	switch(nIDEvent)
	{
	case TM_RESTOREREC:  
		{
			KillTimer(nIDEvent);
		
			CString file, usnm = Axis::user;
			file.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, usnm, usnm); 
			char buffer[128] = {0,};
			GetPrivateProfileString("MODE", "CONCLUSION", "", buffer, 128, file);
			CString sdata, leftS, topS, rightS, bottomS;
			sdata.Format("%s", buffer);
			sdata.TrimRight();

			leftS = Parser(sdata, "|");
			topS =  Parser(sdata, "|");
			rightS =  Parser(sdata, "|");
			bottomS =  Parser(sdata, "|");
			m_recDef.left = m_recDef.right - (atoi(rightS) - atoi(leftS));
			m_recDef.top = m_recDef.bottom - (atoi(bottomS) - atoi(topS));

CString slog;
slog.Format("[conclusion] ontimer [%d][%d][%d][%d]   [%d][%d]\n", m_recDef.left, m_recDef.top, m_recDef.right, m_recDef.bottom, m_recDef.Width(), m_recDef.Height());
//OutputDebugString(slog);

			MoveWindow(m_recDef);
			ResizeGrid();
			ShowWindow(SW_HIDE);
			m_parent->SendMessage(WM_USER, MMSG_RESTORECONDLG, 0);
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
			m_parent->SendMessage(WM_USER, MMSG_RESTORECONDLG, 0);
			ShowWindow(SW_HIDE);
		}
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

COLORREF CConclusion::GetColor(int index)
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


DWORD CConclusion::GetAttrByStr(CString sAttr)
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
#pragma warning(disable : 26408)
int CConclusion::_httoi(const TCHAR *value)
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
	TCHAR *mstr = _tcsupr(_tcsdup(value));
	TCHAR *ss = mstr;
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

CString CConclusion::parseX(CString &srcstr, CString substr)
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

void CConclusion::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	if(m_pGrid != NULL  && m_arr.GetSize() > 0)
	{
		ResizeGrid();
		m_pGrid->Adjust();
	}
	
}

#pragma warning (disable : 4244)
void CConclusion::ResizeGrid()
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
	clientRC.right = clientRC.Width() - 7 ;
	m_pGrid->MoveWindow(clientRC);
	m_pGrid->Adjust();

	gridrec = clientRC;
	for (ii = 0; ii < m_pGrid->GetColumnCount(); ii++)
	{
		dratio = atof(m_arr.GetAt(ii));
		dwidth = gridrec.Width();
		double width = dwidth * dratio;

		isum += (int)width;
		if(ii == m_pGrid->GetColumnCount() - 1)
			width += clientRC.Width() - isum - GetSystemMetrics(SM_CXVSCROLL);
		m_pGrid->SetColumnWidth(ii, (int)width);
	}	

	m_pGrid->Adjust();
	
	CRect rec, wrec;
	GetClientRect(wrec);
	GetDlgItem(IDC_OSETUP)->GetWindowRect(rec);
	ScreenToClient(rec);
	rec.left = wrec.Width() * m_dBtn1RatioL;
	rec.right = wrec.Width() * m_dBtn1RatioR;

	rec.top = wrec.bottom - BTNGAP - rec.Height();
	rec.bottom = wrec.bottom - BTNGAP;
	GetDlgItem(IDC_OSETUP)->MoveWindow(rec);
	GetDlgItem(IDC_OSETUP)->Invalidate();
	
	GetDlgItem(IDC_CLEAR)->GetWindowRect(rec);
	ScreenToClient(rec);
	rec.left = wrec.Width() *m_dBtn2RatioL;
	rec.right = wrec.Width() * m_dBtn2RatioR;
	rec.top = wrec.bottom - BTNGAP - rec.Height();
	rec.bottom = wrec.bottom - BTNGAP;

	GetDlgItem(IDC_CLEAR)->MoveWindow(rec);
	GetDlgItem(IDC_CLEAR)->Invalidate();
	
	GetDlgItem(IDOK)->GetWindowRect(rec);
	ScreenToClient(rec);
	rec.left = wrec.Width() * m_dBtn3RatioL;
	rec.right = wrec.Width() * m_dBtn3RatioR;
	rec.top = wrec.bottom - BTNGAP - rec.Height();
	rec.bottom = wrec.bottom - BTNGAP;

	GetDlgItem(IDOK)->MoveWindow(rec);
	GetDlgItem(IDOK)->Invalidate();

	GetDlgItem(IDC_ORIREC)->GetWindowRect(rec);
	ScreenToClient(rec);
	rec.left = wrec.Width() * m_dBtn4RatioL;
	rec.right = wrec.Width() * m_dBtn4RatioR;
	rec.top = wrec.bottom - BTNGAP - rec.Height();
	rec.bottom = wrec.bottom - BTNGAP;

	GetDlgItem(IDC_ORIREC)->MoveWindow(rec);
	GetDlgItem(IDC_ORIREC)->Invalidate();
}
#pragma warning (default : 4244)

int CConclusion::FindColData(CString strdata)
{
	CString slog;
	const int iret = -1;
	CString sdata;
	for(int ii = 0 ; ii < m_pGrid->GetRowCount(); ii++)
	{
		sdata = m_pGrid->GetItemText(ii, COL_0) + " " + m_pGrid->GetItemText(ii, COL_1);
		if(strdata == sdata)
			return ii;
	}
	return iret;
}

void CConclusion::OnOriRec()
{
	SetWindowPos(NULL, m_recOri.left, m_recOri.top, ORIW, ORIH, NULL);
	ResizeGrid();
	ShowWindow(SW_HIDE);
	m_parent->SendMessage(WM_USER, MMSG_RESTORECONDLG, 0);
}

void CConclusion::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	CString  sdata;
	CString file, usnm = Axis::user;
	CRect rec;

	GetWindowRect(rec);
	sdata.Format("%d|%d|%d|%d", rec.left, rec.top, rec.right, rec.bottom);	
	
	file.Format("%s\\%s\\%s\\%s.ini", Axis::home, USRDIR, usnm, usnm); 
	WritePrivateProfileString("MODE", "CONCLUSION",sdata, file);

CString slog;
slog.Format("[conclusion] ondestory [%d][%d][%d][%d]   [%d][%d]\n", rec.left, rec.top, rec.right, rec.bottom, rec.Width(), rec.Height());
//OutputDebugString(slog);
}		

CString CConclusion::Parser(CString &srcstr, CString substr)
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
