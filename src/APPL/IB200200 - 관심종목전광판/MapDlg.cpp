// MapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "IB200200.h"
#include "MapDlg.h"

#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "../../h/grid.h"
#include "../../axis/axMsg.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	CURR_COLOR	// 현재가 대비색상

#define	portFILE	"portfolio.ini"
#define	interFILE	"interField.ini"
#define	deliMITER	";"

#define	HCOD		"1301"
#define	FCOD		"30301"
#define	OCOD		"40301"
#define	PCOD		"40301"
#define	UCOD		"20301"
#define SFCOD		"50301"
#define	ECOD		"ECOD"

#define	kcurrMAP	"IB200100"		// kospi  현재가
#define	fcurrMAP	"IB400100"		// future 현재가
#define	ocurrMAP	"IB400200"		// option 현재가
#define	focurrMAP	"IB440100"		// 주식옵션현재가
#define	icurrMAP	"IB290300"		// 업종 현재가
#define	configMAP	"IB202201"		// config map
#define sfcurrMAP	"IB400300"		// 주식선물 현재가 @@Zeta 화면 아직 없음

const int idGRID1	= 2001;
const int idGRID2	= 2002;
const int idGRID3	= 2003;
const int idGRID4	= 2004;

const int rowH		= 20;

const int gap1		= 1;
const int gap2		= 2;
const int comboH	= 20;
const int offsetGH	= gap2 + comboH + gap2;

const int comboW	= 102;
const int size18	= 20;
const int size44	= 32;


const int maxGROUP = 100, maxJONGMOK = 80;

const char*	gSYMBOL	= _T("$90303");

const char	gnCHAR	= 0x7f;
const char	tabCHAR	= 0x09;
const char	lfCHAR	= 0x0A;

const int	dispRow = 20 +1;
const int	scrollW = ::GetSystemMetrics(SM_CYHSCROLL);

const int	colCODE = 1, colOPEN = 2, colHIGH = 3, colLOW = 4, colPCURR = 5, colPOPEN = 6, colPHIGH = 7, colPLOW = 8;
const int	colNAME = 9, colCURR = 10, colDIFF = 11;

/////////////////////////////////////////////////////////////////////////////
// CMapDlg dialog


CMapDlg::CMapDlg(CWnd* parent /*=NULL*/)
	: CDialog(CMapDlg::IDD, parent)
{
	//{{AFX_DATA_INIT(CMapDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_parent = parent;
	m_fontsize = 9;

	m_gridW = 0;
	m_rowC = maxJONGMOK + 1;
	m_colC = 5;

	for (int vidx = 0; vidx < maxVIEW; vidx++)
	{
		m_category[vidx] = NULL;

		m_grid[vidx] = NULL;
		m_inters[vidx].RemoveAll();
		m_waiting[vidx]   = false;
	}

	m_viewType = viewFOUR;
	m_blink = true;

	m_bThreadRTM = false;
	m_pThreadRTM = NULL;
	m_hEventRTM = NULL;
	m_bRemove	=	FALSE;	// 2011.12.29 KDK
	initRTM();

	m_nOldRow = -1;
}


void CMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMapDlg, CDialog)
	//{{AFX_MSG_MAP(CMapDlg)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_INTERNM0, OnSelchangeInterNM0)
	ON_CBN_SELCHANGE(IDC_INTERNM1, OnSelchangeInterNM1)
	ON_CBN_SELCHANGE(IDC_INTERNM2, OnSelchangeInterNM2)
	ON_CBN_SELCHANGE(IDC_INTERNM3, OnSelchangeInterNM3)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_TOPARENT, OnParent)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapDlg message handlers
BOOL CMapDlg::Create(CWnd* parent) 
{
	m_root = Variant(homeCC, "");
	m_name = Variant(nameCC, "");

	m_font = getAxFont();
	m_fontB= getAxFont(idTOstring(IDS_DEFAULTFONT), m_fontsize, 3);

	return CDialog::Create(IDD, parent);
}

BOOL CMapDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetOwner(m_parent);
	Variant(titleCC, idTOstring(IDS_TITLE));
	
	m_bkBrush.DeleteObject();
	m_bkBrush.CreateSolidBrush(GetColor(64));
	m_blinkColor = GetColor(getFlashIndex());

	m_font = getAxFont(idTOstring(IDS_DEFAULTFONT), m_fontsize);	

	restoreMF(); 
	loadFieldInfo(); 
	initControl(); 
	loadInterName();

	CRect	clientRC; 
	GetClientRect(&clientRC); 
	clientRC.top += offsetGH;

	LOGFONT lf; 
	m_font->GetLogFont(&lf); 
	lf.lfHeight = m_fontsize * 10;

	for (int vidx = 0; vidx < maxVIEW; vidx++)
	{
		clientRC.left = m_gridW * vidx; 
		clientRC.right = clientRC.left + m_gridW;

		m_grid[vidx] = new CfxGrid(&lf);
		m_grid[vidx]->Create(clientRC, this, idGRID1 + vidx, GVSC_BOTH);
	}

	initialGrid(false); 
	repositionView();

	for (vidx = 0; vidx < maxVIEW; vidx++)
	{
		int cursel = m_internm[vidx].GetCurSel();
		if (cursel != CB_ERR)
		{
			int gno = m_internm[vidx].GetItemData(cursel);
			loadInterest(vidx, gno); 
			sendTransaction(vidx);
		}
	}

	m_blink = IsBlink();

	SendMinMaxInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMapDlg::OnClose() 
{
	DestroyWindow(); 
	CDialog::OnClose();
}

void CMapDlg::OnDestroy() 
{
	m_sync.Lock();
	for (int vidx = 0; vidx < maxVIEW; vidx++)
	{
		clearInterest(vidx);

		if (m_category[vidx] != NULL)
		{
			m_category[vidx]->DestroyWindow();
			delete m_category[vidx]; m_category[vidx] = NULL;
		}
		if (m_grid[vidx] != NULL)
		{
			delete m_grid[vidx]; m_grid[vidx] = NULL;
		}
	}
	m_sync.Unlock();

	m_gridHdrX.RemoveAll(); 
	m_bkBrush.DeleteObject(); 
	saveMF();

	stopRTM();

	CDialog::OnDestroy();
}

BOOL CMapDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	bool	toggle;
	CString	string;

	switch (LOWORD(wParam))
	{
	case IDC_HAWKEYE:
		toggle = m_hawk.IsChecked();
		hawkeyeAction(toggle);
		break;

	case IDC_EXPECT:
		toggle = m_expect.IsChecked();
		toggleAction(toggle);
		break;

	case IDC_CONFIG:
		string.Format("%s /S /p5 /d %s\t1", configMAP, "registerCODE"); openView(typePOPUP, string);
		break;

	case IDC_PLUS:
	case IDC_MINUS:
		plusminusClick(LOWORD(wParam));
		SendMinMaxInfo();
		break;
	}
	
	return CDialog::OnCommand(wParam, lParam);
}

void CMapDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (m_grid[0] != NULL)
	{
 		CRect rect, grect, clientRC; GetClientRect(&clientRC);

		rect = grect = clientRC; 
		
		rect.top += gap2; grect.top += offsetGH;

		for (int vidx = 0; vidx < maxVIEW; vidx++)
		{
			rect.left = clientRC.left + (m_gridW*vidx); rect.right = rect.left + comboW; rect.bottom = rect.top + comboH;
			if (vidx == 0) rect.left += gap2;
			m_internm[vidx].MoveWindow(&rect);

			rect.left = rect.right + gap2; rect.right = rect.left + size18; rect.bottom = rect.top + size18;
			m_category[vidx]->MoveWindow(&rect);

			m_grid[vidx]->SetWindowPos(NULL, m_gridW*vidx, offsetGH, m_gridW, grect.Height(), SWP_NOZORDER);
			m_grid[vidx]->Adjust();
		}

		rect = clientRC; rect.top += gap2; rect.bottom = rect.top + size18;

		int xpos = (m_gridW*m_viewType) - ((size18 + gap1 + size44 + gap1) * 2);

		rect.left = xpos;              rect.right = rect.left + size44;		m_expect.MoveWindow(&rect);
		rect.left = rect.right + gap1; rect.right = rect.left + size44;		m_config.MoveWindow(&rect);
		rect.left = rect.right + gap1; rect.right = rect.left + size18;		m_minus.MoveWindow(&rect);
		rect.left = rect.right + gap1; rect.right = rect.left + size18;		m_plus.MoveWindow(&rect);
	}
}

BOOL CMapDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NM_GVITEM* nmgv = (NM_GVITEM*) lParam;
	if (wParam < idGRID1 || wParam > idGRID4)
		return CDialog::OnNotify(wParam, lParam, pResult);

	int vidx = int(wParam) - idGRID1;
	
	switch (nmgv->hdr.code)
	{
	case GVNM_LMOUSEDOWN:
		m_nOldRow = nmgv->row;
		break;
	case GVNM_LMOUSEUP:
		if (nmgv->row == m_nOldRow && nmgv->row > -1)
		{
			CString code = m_grid[vidx]->GetItemText(nmgv->row, colCODE);
			code.TrimLeft();
			code.TrimRight();

			CString strTrigger(_T(""));

			if(code.GetLength() != 6)
				break;
			
			strTrigger.Format("1301\t%s", code);
			Variant(triggerCC, strTrigger);
		}

		m_nOldRow = -1;
		break;

	case GVNM_COLUMNMOVE:
		{
			if (nmgv->row >= m_gridHdrX.GetSize()
					|| nmgv->col >= m_gridHdrX.GetSize())
				break;

			_gridHdr gridHdr;

			gridHdr = m_gridHdrX.GetAt(nmgv->col);
			m_gridHdrX.InsertAt(nmgv->row, gridHdr);

			if (nmgv->col > nmgv->row)
				nmgv->col += 1;

			m_gridHdrX.RemoveAt(nmgv->col);
		}
		break;
	case GVNM_DBLCLICKED:
		{
			CString code, string; int kind = 0;
			if (nmgv->row >= 1 && nmgv->col >= colNAME)
//			if (nmgv->col == colNAME || nmgv->col == colCURR)
			{
				code = m_grid[vidx]->GetItemText(nmgv->row, colCODE);
				code.TrimLeft(); code.TrimRight();
				if (code.IsEmpty()) break;

				switch (getCodeType(code))
				{
				case kospiCODE:
					string.Format("%s /S/t0/d%s\t%s", kcurrMAP, HCOD, code);
					openView(typeVIEW, string); break;
				case futureCODE:
					string.Format("%s /S/t0/d%s\t%s", fcurrMAP, FCOD, code);
					openView(typeVIEW, string); break;
				case optionCODE:
					string.Format("%s /S/t0/d%s\t%s", ocurrMAP, OCOD, code);
					openView(typeVIEW, string); break;
				case foptionCODE:
					string.Format("%s /S/t0/d%s\t%s", focurrMAP,PCOD, code);
					openView(typeVIEW, string); break;
				case indexCODE:
					string.Format("%s /S/t0/d%s\t%s", icurrMAP, UCOD, code);
					openView(typeVIEW, string); break;
				case sfCODE:
					string.Format("%s /S/t0/d%s\t%s", sfcurrMAP, SFCOD, code);
					openView(typeVIEW, string); break;
				}
			}
		}
		break;
	case GVNM_RMOUSEDOWN:
		{
/*			static CString mouseData;

			CPoint	point; GetCursorPos(&point);
			mouseData.Format("IB200200\t\n%d\t%d\n", point.x, point.y);

			if (nmgv->row >= 1)
			{
				m_grid[vidx]->SetSelectRow(nmgv->row);

				CString	code, string = _T("");
				code = m_grid[vidx]->GetItemText(nmgv->row, colCODE);
				code.TrimLeft(); code.TrimRight();

				if (!code.IsEmpty())
				{
					switch (getCodeType(code))
					{
					case kospiCODE:		string.Format("%s\t%s", HCOD, code);	break;
					case futureCODE:	string.Format("%s\t%s", FCOD, code);	break;
					case optionCODE:	string.Format("%s\t%s", OCOD, code);	break;
					case foptionCODE:	string.Format("%s\t%s", PCOD, code);	break;
					case indexCODE:		string.Format("%s\t%s", UCOD, code);	break;
					case sfCODE:		string.Format("%s\t%s", SFCOD, code);	break;
					}

					if (!string.IsEmpty()) mouseData += string;
				}
			}

			m_parent->PostMessage(WM_USER, MAKEWPARAM(mouseDLL, 0), (long)mouseData.operator LPCTSTR());
*/
		}
		break;
	}

	return CDialog::OnNotify(wParam, lParam, pResult);
}

HBRUSH CMapDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
		return m_bkBrush;
	default:
		break;
	}
	return hbr;
}

void CMapDlg::OnSelchangeInterNM0()
{
	int index, vidx = 0, gno;
	index = m_internm[vidx].GetCurSel();
	if (index != CB_ERR)
	{
		gno  = m_internm[vidx].GetItemData(index);
		loadInterest(vidx, gno); sendTransaction(vidx); m_grid[vidx]->SetFocus();
	}
}

void CMapDlg::OnSelchangeInterNM1()
{
	int index, vidx = 1, gno;
	index = m_internm[vidx].GetCurSel();
	if (index != CB_ERR)
	{
		gno  = m_internm[vidx].GetItemData(index);
		loadInterest(vidx, gno); sendTransaction(vidx); m_grid[vidx]->SetFocus();
	}
}

void CMapDlg::OnSelchangeInterNM2()
{
	int index, vidx = 2, gno;
	index = m_internm[vidx].GetCurSel();
	if (index != CB_ERR)
	{
		gno  = m_internm[vidx].GetItemData(index);
		loadInterest(vidx, gno); sendTransaction(vidx); m_grid[vidx]->SetFocus();
	}
}

void CMapDlg::OnSelchangeInterNM3()
{
	int index, vidx = 3, gno;
	index = m_internm[vidx].GetCurSel();
	if (index != CB_ERR)
	{
		gno  = m_internm[vidx].GetItemData(index);
		loadInterest(vidx, gno); sendTransaction(vidx); m_grid[vidx]->SetFocus();
	}
}

void CMapDlg::plusminusClick(UINT id)
{
	if (id == IDC_PLUS)
	{
		switch (m_viewType)
		{
		case viewTWO:	m_viewType = viewTHREE; break;
		case viewTHREE:	m_viewType = viewFOUR;	break;
		case viewFOUR:	return;
		}
	}
	else if (id == IDC_MINUS)
	{
		switch (m_viewType)
		{
		case viewTWO:	return;
		case viewTHREE:	m_viewType = viewTWO;	break;
		case viewFOUR:	m_viewType = viewTHREE;	break;
		}
	}

	loadFieldInfo(); initialGrid(); repositionView();
	bool toggle = m_expect.IsChecked();
	if (toggle) toggleAction(toggle);

	for (int vidx = 0; vidx < (int) m_viewType; vidx++)
	{
		int cursel = m_internm[vidx].GetCurSel();
		if (cursel != CB_ERR)
		{
			int gno = m_internm[vidx].GetItemData(cursel);
			loadInterest(vidx, gno); sendTransaction(vidx);
		}
	}
}

LONG CMapDlg::OnMessage(WPARAM wParam, LPARAM lParam)
{
	int	vidx = 0;

	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		vidx = HIBYTE(LOWORD(wParam));
		if (vidx >= categoryKEY && vidx <= categoryKEY+maxVIEW)
		{
			vidx -= categoryKEY;
			if (m_category[vidx] != NULL)
				m_category[vidx]->SendMessage(WM_USER+99, wParam, lParam);
			return 0;
		}

		parsingOubs((char *) lParam, HIWORD(wParam), vidx);
		m_waiting[vidx] = false;
		break;
/*
	case DLL_ALERT:
		{
			if (!m_bThreadRTM)
				break;

			char *pBuf = (char*)lParam;
			int len = strlen(pBuf);

			char* pBytes = new char[len+1];
			memset(pBytes, 0x00, len+1);
			memcpy(pBytes, pBuf, len);

			m_syncRTM.Lock();
			m_arBytesRTM.Add(pBytes);
			m_syncRTM.Unlock();
			SetEvent(m_hEventRTM);
		}

		break;
*/
//2012.03.16 KSJ alertX 추가
	case DLL_ALERTx:
		{
			if (!m_bThreadRTM)
				break;
									
			doRTMx(lParam);
		}
		
		break;
// KSJ
	case DLL_SETPAL:
		reloadscreen();
		break;
	case DLL_SETFONT:
		{
			int	fontsize = HIWORD(wParam);
			CString fname = CString((char*)lParam);
			reloadscreen(fname, fontsize);
		}
		break;
	case DLL_TRIGGER:
		parsingTrigger((char *)lParam);
		break;
	case DLL_DOMINO:
		break;
	}

	return 0;
}

LONG CMapDlg::OnParent(WPARAM wParam, LPARAM lParam)
{
	int	vidx;
	switch (LOWORD(wParam))
	{
	case cpCATEGROY:
		vidx = HIWORD(wParam) - categoryKEY;
		if (loadInterest(vidx, m_category[vidx]->category_GetJCodes()) <= 0)
			break;
		sendTransaction(vidx); 
//		m_grid[vidx]->SetFocus();
		break;
	default:
		break;
	}

	return 0;
}

void CMapDlg::sendTransaction(int vidx)
{
	int	sendL = 0;
	char	sendB[2048];
	char	tempB[64];

	if (m_waiting[vidx])
	{
		Variant(guideCC, idTOstring(IDS_GUIDE1));
		return;
	}

	m_waiting[vidx] = true;

	FillMemory(sendB, sizeof(sendB), ' ');

	sprintf(tempB, "%s%c", gSYMBOL, gnCHAR);
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	struct	_gridHi* gridHi;

	gridHi = (struct _gridHi *) &sendB[sendL];
	sendL += sizeof(struct _gridHi);

	CopyMemory(gridHi->visible, "99", sizeof(gridHi->visible));
	sprintf(tempB, "%04d", m_inters[vidx].GetSize());
	CopyMemory(gridHi->rows, tempB, sizeof(gridHi->rows));

	gridHi->type = '0';
	gridHi->dir  = '1';
	gridHi->sort = '0';

	struct	_gridHdr gridHdr;
	for (int ii = 0; ii < m_gridHdrX.GetSize(); ii++)
	{
		gridHdr = m_gridHdrX.GetAt(ii);

		sprintf(tempB, "%s%c", gridHdr.symbol, lfCHAR);
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	}

	sprintf(tempB, "%s%c", "1301", gnCHAR);		// 종목코드
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	struct _inters* pinters;
	CString temp_code;

	for (ii = 0; ii < m_inters[vidx].GetSize(); ii++)
	{
		pinters = m_inters[vidx].GetAt(ii);
		
		//code에서 종목명 떼어내기
		temp_code = pinters->code;
		strcpy(pinters->code,temp_code.Left(6));

		sprintf(tempB, "%s%c", pinters->code, gnCHAR);
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	}
	sendB[sendL] = lfCHAR;  sendL += 1;
	sendB[sendL] = tabCHAR; sendL += 1;
	sendB[sendL] = 0x00; sendTR("pooppoop", sendB, sendL, vidx);
}

void CMapDlg::parsingOubs(char* datB, int datL, int vidx)
{
	int gridHoL = sizeof(struct _gridHo);
	if (datL < gridHoL) return;
	
	m_grid[vidx]->Clear(); m_grid[vidx]->MessageToGrid(WM_VSCROLL, SB_TOP);

	CString	string, stringx, entry;

	string = CString(&datB[gridHoL], datL - gridHoL);
	for (int ii = 0; ii < m_inters[vidx].GetSize(); ii++)
	{
		if (string.IsEmpty())
			break;

		stringx = parseX(string, lfCHAR);
		for (int jj = 0; jj < m_gridHdrX.GetSize(); jj++)
		{
			if (stringx.IsEmpty())
				break;

			entry = parseX(stringx, tabCHAR); entry.TrimRight();
			if (jj == colCURR && entry.GetLength() > 0)
			{
#ifdef	CURR_COLOR
				//entry.Remove('+'); entry.Remove('-');
#else
				entry.Remove('+'); entry.Remove('-');
#endif
			}

			m_grid[vidx]->SetItemText(ii+1, jj, entry);
		}
	}

	if (m_hawk.IsChecked()) hawkeyeAction(vidx);
}

void CMapDlg::parsingAlert(CString datB)
{
	int	idx;
	char	xrow[4];
	CMapStringToString fms;
	CString	code, symbol, entry;

	if ((idx = datB.Find('\t')) == -1)
		return;

	code = datB.Left(idx++);
	datB = datB.Mid(idx);

	if (checkRealTimeCode(code, xrow) <= 0)
		return;

	while (!datB.IsEmpty())
	{
		idx = datB.Find('\t');
		if (idx == -1) break;

		symbol = datB.Left(idx++);
		datB = datB.Mid(idx);

		idx = datB.Find('\t');
		if (idx == -1)
		{
			entry = datB;	datB = _T("");
		}
		else
		{
			entry = datB.Left(idx++); datB = datB.Mid(idx);
		}
		fms.SetAt(symbol, entry);
	}

	const int countX = m_gridHdrX.GetSize(); _gridHdr gridHdr;
	for (int ii = 3; ii < countX; ii++)
	{
		gridHdr = m_gridHdrX.GetAt(ii);
		symbol = CString(gridHdr.symbol, strlen(gridHdr.symbol));
		if (symbol.GetLength() >= 3) symbol = symbol.Right(3);

		if (!fms.Lookup(symbol, entry))
			continue;

		for (int vidx = 0; vidx < (int)m_viewType; vidx++)
		{
			if (xrow[vidx] == -1) continue;

			if (ii == colCURR && entry.GetLength() > 0)
			{
#ifdef	CURR_COLOR
				// entry.Remove('+'); entry.Remove('-');
#else
				entry.Remove('+'); entry.Remove('-');
#endif
			}

			// change TOfabs -> TOf
			if (TOf(entry) == TOf(m_grid[vidx]->GetItemText(xrow[vidx], ii)))
				continue;

			m_grid[vidx]->SetItemText(xrow[vidx], ii, entry);
			if ((!m_hawk.IsChecked() || ii > colCURR) && m_blink)
				m_grid[vidx]->Blink(xrow[vidx], ii, m_blinkColor);

			if (m_hawk.IsChecked())	hawkEyeColor(vidx, ii);
		}
	}
}

//2012.03.16 KSJ alertX 추가

void CMapWnd::parsingAlertx(LPARAM lParam)
{
	int	idx;
	char	xrow[4];
	CMapStringToString fms;
	CString	code, symbol, entry, total_str, dataExceptCode;
	int count = 0;
	
	DWORD* data;
	struct _alertR* alertR;
	alertR = (struct _alertR*)lParam;
	
	code = alertR->code;
	
	for(int ii = 0; ii < alertR->size; ii++)
	{
		data = (DWORD*)alertR->ptr[ii];
		
		count = checkRealTimeCode(code, xrow);
		if (count <= 0)
			return;
		
		//그룹 종목 중복 허용일 경우, m_iRowCode로 사용한다
		for(int rowPosition = 0 ; rowPosition<count ; rowPosition++)
		{
			for (int vidx = 0 ; vidx < maxVIEW ; vidx++)
			{
				xrow[vidx] = m_irowCode[vidx][rowPosition]; 
			}
			
			entry = _T("");
			
			const int countX = m_gridHdrX.GetSize(); 
			_gridHdr gridHdr;
			
			for (int ii = 3; ii < countX; ii++)
			{
				gridHdr = m_gridHdrX.GetAt(ii);
				symbol = CString(gridHdr.symbol, strlen(gridHdr.symbol));
				
				if (symbol.GetLength() >= 3) 
					symbol = symbol.Right(3);
				
				if (data[atoi(symbol)])
				{
					entry = (char*)data[atoi(symbol)];
					
					for (int vidx = 0; vidx < (int)m_viewType; vidx++)
					{
						//				if (xrow[vidx] == -1) continue;
						
						if (ii == colCURR && entry.GetLength() > 0)
						{
#ifdef	CURR_COLOR
							// entry.Remove('+'); entry.Remove('-');
#else
							entry.Remove('+'); 
							entry.Remove('-');
#endif
						}
						
						// change TOfabs -> TOf
						if (TOf(entry) == TOf(m_grid[vidx]->GetItemText(xrow[vidx], ii)))
							continue;
						
						m_grid[vidx]->SetItemText(xrow[vidx], ii, entry);
						
						if ((!m_hawk.IsChecked() || ii > colCURR) && m_blink)
							m_grid[vidx]->Blink(xrow[vidx], ii, m_blinkColor);
						
						if (m_hawk.IsChecked())	hawkEyeColor(vidx, ii);
					}
				}
			}
		}	// for rowPosition		
	}	// while datB
}

// KSJ

void CMapDlg::parsingTrigger(CString datB)
{
	CString	symbol;

	symbol = parseX(datB, "\t");

	if (symbol == _T("OnPortfolio") && datB == _T("ok"))
	{
		int	idxCBO[maxVIEW];
		int	gnoCBO[maxVIEW];
		char	namCBO[maxVIEW][128];

		CString	filePath; filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, portFILE);

		// save combo index
		for (int vidx = 0; vidx < maxVIEW; vidx++)
		{
			idxCBO[vidx]  = m_internm[vidx].GetCurSel();
			gnoCBO[vidx]  = m_internm[vidx].GetItemData(idxCBO[vidx]);
			m_internm[vidx].GetLBText(idxCBO[vidx], namCBO[vidx]);
		}
	
		int count = loadInterName(false);

		if (count > 0)
		{
			int gno; CString text;


			for (vidx = 0; vidx < maxVIEW; vidx++)
			{
				if (m_internm[vidx].GetCurSel() == -1)
				{
					if (m_internm[vidx].GetCount() > 0)
						m_internm[vidx].SetCurSel(0);
					else
						m_grid[vidx]->Clear();
				}

				if (idxCBO[vidx] < count)
				{
					gno = m_internm[vidx].GetItemData(idxCBO[vidx]);
//					m_internm[vidx].GetLBText(idxCBO[vidx], text);
					m_internm[vidx].GetWindowText(text);

					if (text.IsEmpty())
						continue;

					if (gno == gnoCBO[vidx] && !text.Compare(namCBO[vidx]))
					{
						m_internm[vidx].SetCurSel(idxCBO[vidx]);
						continue;
					}
				}

				if (vidx < count)
					m_internm[vidx].SetCurSel(vidx);
				else if (count >= 1)
					m_internm[vidx].SetCurSel(0);
			}

			for (vidx = 0; vidx < maxVIEW; vidx++)
			{
				int cursel = m_internm[vidx].GetCurSel();
				if (cursel != CB_ERR)
				{
					gno = m_internm[vidx].GetItemData(cursel);
					loadInterest(vidx, gno); sendTransaction(vidx);
				}
			}
		}
		else
		{
			for (vidx = 0; vidx < maxVIEW; vidx++)
				m_grid[vidx]->Clear();
		}
	}
}

void CMapDlg::initControl()
{
	int	xpos;
	CRect	rect, clientRC; 
	GetClientRect(clientRC);

	CString	imgPath; 
	imgPath.Format("%s/%s/", m_root, IMAGEDIR);

	m_bitmapH_nm = getAxBitmap(imgPath + "H버튼.bmp")->operator HBITMAP();
	m_bitmapH_dn = getAxBitmap(imgPath + "H버튼_dn.bmp")->operator HBITMAP();
	m_bitmapH_hv = getAxBitmap(imgPath + "H버튼_en.bmp")->operator HBITMAP();

	rect.top = clientRC.top + gap2;

	for (int vidx = 0; vidx < maxVIEW; vidx++)
	{
		rect.left = clientRC.left + (m_gridW*vidx); 
		rect.right = rect.left + comboW; 
		rect.bottom = rect.top + 200;

		if (vidx == 0) rect.left += gap2;

		m_internm[vidx].Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST, rect, this, IDC_INTERNM0 + vidx);
		m_internm[vidx].SetFont(m_font, false); 
		rect.bottom = rect.top + comboH; 
		m_internm[vidx].MoveWindow(&rect);

		rect.left = rect.right + gap2; 
		rect.right = rect.left + size18; 
		rect.bottom = rect.top + size18;

		m_category[vidx] = new CCategory(m_parent, categoryKEY+vidx);
		m_category[vidx]->CreateCategory(this, rect, m_root);

/*		if (vidx == 0)
		{
			rect.left = rect.right + gap2; 
			rect.right = rect.left + size18;
			m_hawk.Create("", rect, this, IDC_HAWKEYE, true);
			m_hawk.SetFont(m_font, false);  
			m_hawk.SetImgBitmap(m_bitmapH_nm, m_bitmapH_dn, m_bitmapH_hv);
		}*/
	}

	m_bitmap2_nm = getAxBitmap(imgPath + "2btn.bmp")->operator HBITMAP();
	m_bitmap2_dn = getAxBitmap(imgPath + "2btn_dn.bmp")->operator HBITMAP();
	m_bitmap2_hv = getAxBitmap(imgPath + "2btn_en.bmp")->operator HBITMAP();

	m_bitmapM_nm = getAxBitmap(imgPath + "minusBTN.bmp")->operator HBITMAP();
	m_bitmapM_dn = getAxBitmap(imgPath + "minusBTN_dn.bmp")->operator HBITMAP();
	m_bitmapM_hv = getAxBitmap(imgPath + "minusBTN_en.bmp")->operator HBITMAP();

	m_bitmapP_nm = getAxBitmap(imgPath + "plusBTN.bmp")->operator HBITMAP();
	m_bitmapP_dn = getAxBitmap(imgPath + "plusBTN_dn.bmp")->operator HBITMAP();
	m_bitmapP_hv = getAxBitmap(imgPath + "plusBTN_en.bmp")->operator HBITMAP();


	rect.SetRect(0, 0, 0, 0);
	m_expect.Create(idTOstring(IDS_EXPECT), rect, this, IDC_EXPECT, true);
	m_expect.SetFont(m_font, false);  
	m_expect.SetImgBitmap(m_bitmap2_nm, m_bitmap2_dn, m_bitmap2_hv);

	m_config.Create(idTOstring(IDS_CONFIG), rect, this, IDC_CONFIG);
	m_config.SetFont(m_font, false); 
	m_config.SetImgBitmap(m_bitmap2_nm,m_bitmap2_dn, m_bitmap2_hv);

	m_minus.Create("", rect, this, IDC_MINUS);
	m_minus.SetFont(m_font, false);  
	m_minus.SetImgBitmap(m_bitmapM_nm, m_bitmapM_dn, m_bitmapM_hv);
	
	m_plus.Create("", rect, this, IDC_PLUS);
	m_plus.SetFont(m_font, false);  
	m_plus.SetImgBitmap(m_bitmapP_nm, m_bitmapP_dn, m_bitmapP_hv);

	rect.top = clientRC.top + gap2;
	rect.bottom = rect.top + size18;

	xpos = (m_gridW*m_viewType) - ((size18 + gap1 + size44 + gap1) * 2);

	rect.left = xpos;              
	rect.right = rect.left + size44;		
	m_expect.MoveWindow(&rect);

	rect.left = rect.right + gap1; 
	rect.right = rect.left + size44;		
	m_config.MoveWindow(&rect);

	rect.left = rect.right + gap1; 
	rect.right = rect.left + size18;		
	m_minus.MoveWindow(&rect);

	rect.left = rect.right + gap1; 
	rect.right = rect.left + size18;		
	m_plus.MoveWindow(&rect);
}

void CMapDlg::initialGrid(int size)
{
	CRect	clientRC;

	LOGFONT lf, lfB;
	m_font->GetLogFont(&lf); lf.lfHeight = m_fontsize * 10;
	m_fontB->GetLogFont(&lfB); lfB.lfHeight = m_fontsize * 10;

	for (int vidx = 0; vidx < maxVIEW; vidx++)
	{
		m_grid[vidx]->Initial(m_rowC, m_colC, 1, 0);

		int	width = 0;
		GVITEM	gvitem; _gridHdr gridHdr;

 		// column header setting
		for (int ii = 0; ii < m_colC; ii++)
		{
			gridHdr = m_gridHdrX.GetAt(ii);

			gvitem.row   = 0;
			gvitem.col   = ii;
			gvitem.state = 0;
			gvitem.mask  = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR;
			gvitem.format= GVFM_HEADER;
			gvitem.fgcol = GetColor(76);
			gvitem.bkcol = GetColor(74);

			CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));

			gvitem.text  = idTOstring(gridHdr.stid);
			gvitem.attr  = gridHdr.attr;

			m_grid[vidx]->SetItem(&gvitem);
			m_grid[vidx]->SetColumnWidth(ii, gridHdr.width);
		}

		for (ii = 0; ii < m_colC; ii++)
		{
			gridHdr = m_gridHdrX.GetAt(ii);

			gvitem.state = 0;
			gvitem.mask  = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_SYMBOL;

			gvitem.format= gridHdr.format;
			gvitem.symbol= gridHdr.symbol; 
			gvitem.attr  = gridHdr.attrx;
			gvitem.fgcol = GetColor(69);
			gvitem.bkcol = GetColor(68);

			CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));
			if (ii == colCURR) CopyMemory(&gvitem.font, &lfB, sizeof(LOGFONT));

			m_grid[vidx]->SetColumnItems(ii, &gvitem);
		}

		m_grid[vidx]->SetBKSelColor(GetColor(78));
		m_grid[vidx]->SetRowColResize(FALSE, TRUE);
		m_grid[vidx]->SetKeepingColumn(colNAME);		// keep column count
		m_grid[vidx]->SetGridFocusLine(FALSE, TRUE);
		m_grid[vidx]->SetGridColor(GetColor(65));
		m_grid[vidx]->SetConditionColor(GetColor(94), GetColor(95), GetColor(69));
//		m_grid[vidx]->SetStepColor(1, GetColor(68), GetColor(77));
		m_grid[vidx]->Adjust();
	}

	if (size)
	{
		int colW, rowH; m_grid[0]->GetCellSize(0, dispRow, colW, rowH); colW = (colW+scrollW) * m_viewType;
		m_parent->PostMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(colW-m_viewType, rowH+offsetGH+1));
		writeVtCode(colW, rowH+offsetGH+1);
	}
}

int CMapDlg::loadInterName(bool bcursel)
{
	for (int vidx = 0; vidx < maxVIEW; vidx++)
		m_internm[vidx].ResetContent();

	CString	filePath; filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, portFILE);

	int readL = 0; char readB[1024];
	CString	string, gno, gname;

	readL = GetPrivateProfileString(_T("GROUPORDER"), "00", "", readB, sizeof(readB), filePath);
	string = CString(readB, readL);

	for (int ii = 0; !string.IsEmpty(); ii++)
	{
		gno   = parseX(string, ";");
		readL = GetPrivateProfileString(_T("GROUPNAME"), gno, "", readB, sizeof(readB), filePath);
		gname = CString(readB, readL);

		if (gname.IsEmpty()) continue;

		for (vidx = 0; vidx < maxVIEW; vidx++)
		{
			int index = m_internm[vidx].AddString(gname);
			m_internm[vidx].SetItemData(index, atoi(gno));
		}
	}

	int count = m_internm[0].GetCount();

	if (bcursel)
	{
		for (vidx = 0; vidx < maxVIEW; vidx++)
		{
			if (count > vidx)
				m_internm[vidx].SetCurSel(vidx);
			else if (count >= 1)
				m_internm[vidx].SetCurSel(0);
		}
	}

	return count;
}

void CMapDlg::loadFieldInfo()
{
	struct	_gridHdr gridHdr[] = {
		{IDS_GH_RCOD,	0,	GVAT_HIDDEN,		"1021",	GVFM_LEFT,	0,				9},	// 0
		{IDS_GH_CODE,	0,	GVAT_HIDDEN,		"1301",	GVFM_LEFT,	0,				9},	// 1
		{IDS_GH_OPEN,	0,	GVAT_HIDDEN,		"2029",	GVFM_RIGHT,	0,				9},	// 2
		{IDS_GH_HIGH,	0,	GVAT_HIDDEN,		"2030",	GVFM_RIGHT,	0,				9},	// 3
		{IDS_GH_LOW,	0,	GVAT_HIDDEN,		"2031",	GVFM_RIGHT,	0,				9},	// 4
		{IDS_GH_PRECURR,0,	GVAT_HIDDEN,		"2934",	GVFM_RIGHT,	0,				9},	// 5
		{IDS_GH_PREOPEN,0,	GVAT_HIDDEN,		"2931",	GVFM_RIGHT,	0,				9},	// 6
		{IDS_GH_PREHIGH,0,	GVAT_HIDDEN,		"2932",	GVFM_RIGHT,	0,				9},	// 7
		{IDS_GH_PRELOW,	0,	GVAT_HIDDEN,		"2933",	GVFM_RIGHT,	0,				9},	// 8
		{IDS_GH_NAME,	70,	GVAT_SORTSTR,		"1022",	GVFM_LEFT,	0,				9},	// 9
#ifdef	CURR_COLOR
		{IDS_GH_CURR,	60,	GVAT_SORTABS,		"2023",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx|GVAT_ZEROSUP,	4},	// 10
#else
		{IDS_GH_CURR,	60,	GVAT_SORTABS,		"2023",	GVFM_RIGHT,	GVAT_COMMA|GVAT_ZEROSUP,			4},	// 10
#endif
		{IDS_GH_DIFF,	60,	GVAT_SORTVAL,		"2024",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONTRAST|GVAT_ZEROSUP,	4},	// 11
		{IDS_GH_RATE,	60,	GVAT_SORTVAL,		"2033",	GVFM_CENTER,	GVAT_CONDITION|GVAT_ZEROSUP,			4},	// 12

		{IDS_GH_VOL,	75,	GVAT_SORTABS,		"2027",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx|GVAT_ZEROSUP,	3},	// 13

		{IDS_GH_ASK,	60,	GVAT_SORTABS,		"2025",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	2},	// 14
		{IDS_GH_BID,	60,	GVAT_SORTABS,		"2026",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	2},	// 15
	};

	m_gridHdrX.RemoveAll();
	const int gridHdrC = sizeof(gridHdr) / sizeof(_gridHdr);

	m_gridW = 0;
	for (int ii = 0; ii < gridHdrC; ii++)
	{
		if (gridHdr[ii].fidx >= (UINT)m_viewType)
		{
			m_gridHdrX.Add(gridHdr[ii]);

			if (!(gridHdr[ii].attr & GVAT_HIDDEN))
				m_gridW += gridHdr[ii].width;
		}
	}
	if (m_gridW > 0) m_gridW += scrollW;

	m_colC = m_gridHdrX.GetSize();
}

void CMapDlg::toggleAction(bool toggle)
{
	struct	_symbol {
		UINT	stid1;
		char*	symb1;
		UINT	stid2;
		char*	symb2;
	} chksym [] = {
		{ IDS_GH_CURR,	"2023",	IDS_GH_ANTIPRC,	"2111" },
// 		{ IDS_GH_DIFF,	"2024",	IDS_GH_DIFF,	"2113" },
// 		{ IDS_GH_RATE,	"2033",	IDS_GH_RATE,	"2114" },
		{ IDS_GH_DIFF,	"2024",	IDS_GH_DIFF,	"2115" },
		{ IDS_GH_RATE,	"2033",	IDS_GH_RATE,	"2116" },
		{ IDS_GH_VOL,	"2027",	IDS_GH_ANTIVOL,	"2112" },
	};
	const	int chksymC = sizeof(chksym) / sizeof(_symbol);


	/*
	if (toggle)		// 예상체결가 형태로 변경
	{
		_gridHdr gridHdr;
		for (int ii = 0; ii < chksymC; ii++)
		{
			for (int jj = 0; jj < m_gridHdrX.GetSize(); jj++)
			{
				gridHdr = m_gridHdrX.GetAt(jj);
				if (atoi(gridHdr.symbol) != atoi(chksym[ii].symb1))
					continue;
			
				gridHdr.stid = chksym[ii].stid2;
				strcpy(gridHdr.symbol, chksym[ii].symb2);

				m_gridHdrX.SetAt(jj, gridHdr);
				break;
			}
		}
	}
	else
	{
		_gridHdr gridHdr;
		for (int ii = 0; ii < chksymC; ii++)
		{
			for (int jj = 0; jj < m_gridHdrX.GetSize(); jj++)
			{
				gridHdr = m_gridHdrX.GetAt(jj);
				if (atoi(gridHdr.symbol) != atoi(chksym[ii].symb2))
					continue;
			
				gridHdr.stid = chksym[ii].stid1;
				strcpy(gridHdr.symbol, chksym[ii].symb1);

				m_gridHdrX.SetAt(jj, gridHdr);
				break;
			}
		}
	}
*/
	//2011.12.27 KSJ 수정본 업종일때는 심볼바꾸지 안도록 수정하기 위해
	CString strCode;
	
	if (toggle)		// 각 Row마다 코스피, 코스닥이면 헤더를 변경하고 심볼을 변경한다.
	{
		_gridHdr gridHdr;
		
		for(int nRow = 1; nRow < m_grid->GetRowCount(); nRow++)
		{
			strCode.Empty();
			strCode = m_grid->GetItemText(nRow, colCODE);
			
			if(strCode.GetLength() == 6)	//코스피, 코스닥
			{
				for (int ii = 0; ii < chksymC; ii++)
				{
					for (int jj = colCURR; jj < m_gridHdrX.GetSize(); jj++)
					{
						gridHdr = m_gridHdrX.GetAt(jj);
						
						if (atoi(m_grid->GetItemSymbol(nRow, jj)) != atoi(chksym[ii].symb1))
							continue;
						
						m_grid->SetItemSymbol(nRow, jj, chksym[ii].symb2);
						gridHdr.stid = chksym[ii].stid2;
						
						m_gridHdrX.SetAt(jj, gridHdr);
						break;
					}
				}
			}
		}
	}
	else		// false일때는 무조건 현재가로 바꿔준다.
	{
		_gridHdr gridHdr;
		
		for(int nRow = 1; nRow < m_grid->GetRowCount(); nRow++)
		{
			strCode.Empty();
			strCode = m_grid->GetItemText(nRow, colCODE);
			
			if(strCode.GetLength() == 6)	//코스피, 코스닥
			{
				for (int ii = 0; ii < chksymC; ii++)
				{
					for (int jj = colCURR; jj < m_gridHdrX.GetSize(); jj++)
					{
						gridHdr = m_gridHdrX.GetAt(jj);
						
						if (atoi(m_grid->GetItemSymbol(nRow, jj)) != atoi(chksym[ii].symb1))
							continue;
						
						m_grid->SetItemSymbol(nRow, jj, chksym[ii].symb1);
						gridHdr.stid = chksym[ii].stid1;
						strcpy(gridHdr.symbol, chksym[ii].symb1);
						
						m_gridHdrX.SetAt(jj, gridHdr);
						break;
					}
				}
			}
		}
	}
	
	//sendTransaction();
	// KSJ

	m_colC = m_gridHdrX.GetSize(); initialGrid(false);

	for (int vidx = 0; vidx < (int)m_viewType; vidx++)
		sendTransaction(vidx);
}

int CMapDlg::loadInterest(int vidx, int gno)
{
	clearInterest(vidx);

	CString	filePath; filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_name, gno);
	if (!existFile(gno))
		return 0;

	UINT	readL;
	struct	_inters* pinters;

	CFile	fileH(filePath, CFile::modeRead);

	for (int ii = 0; ii < maxJONGMOK; ii++)
	{
		pinters = (_inters *) new char[sz_inters];
		FillMemory(pinters, sz_inters, ' ');

		readL = fileH.Read(pinters, sz_inters);
		if (readL < sz_inters)
		{
			delete pinters;
			break;
		}
		m_inters[vidx].Add(pinters);
	}
	fileH.Close();

	return m_inters[vidx].GetSize();
}

int CMapDlg::loadInterest(int vidx, CString codes)
{
	if (codes.IsEmpty()) return 0;

	clearInterest(vidx);

	CString	code, name;
	struct	_inters* pinters;

	for (int ii = 0; ii < maxJONGMOK; ii++)
	{
		code = parseX(codes, tabCHAR);
		if (code.IsEmpty()) continue;

		if (code.GetLength() == 6 || code.GetLength() == 8)
		{
			char wB[64]; strcpy(wB, code);
			char* xxP = (char *)m_parent->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)wB);
			name = xxP; name.TrimLeft(); name.TrimRight();
		}

		if (name.IsEmpty()) continue;

		pinters = (_inters *) new char[sz_inters];
		FillMemory(pinters, sz_inters, ' ');

		pinters->gubn[0] = '0';
		strcpy(pinters->code, code);
		strcpy(pinters->name, name);

		m_inters[vidx].Add(pinters);
	}

	return m_inters[vidx].GetSize();
}

void CMapDlg::saveInterest(int vidx, int gno)
{
	CString	filePath;
	filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_name, gno);

	struct	_inters* pinters;

	CFile	fileH(filePath, CFile::modeWrite|CFile::modeCreate);

	for (int ii = 0; ii < m_inters[vidx].GetSize(); ii++)
	{
		pinters = m_inters[vidx].GetAt(ii);
		fileH.Write(pinters, sz_inters);
	}
	fileH.Close();
}

void CMapDlg::clearInterest(int vidx)
{
	if (m_hawk.IsChecked()) hawkeyeClear(vidx);

	for (int ii = 0; ii < m_inters[vidx].GetSize(); ii++)
		delete m_inters[vidx].GetAt(ii);
	m_inters[vidx].RemoveAll();
}

bool CMapDlg::existFile(int gno)
{
	CString	filePath;
	filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_name, gno);

	DWORD	fLen = 0;
	CFileFind finder;
	if (!finder.FindFile(filePath))
		return false;

	CFile file(filePath, CFile::modeRead);
	fLen = file.GetLength(); file.Close();

	if (fLen <= 0)
		return false;

	return true;
}

int CMapDlg::getCodeType(CString code)
{
	int codeL = code.GetLength();

	if (codeL == 6)					// kospi code
	{
		return kospiCODE;
	}
	else if (codeL <= 3)				// upjong code
	{
		return indexCODE;
	}
	else if (code[0] == '1' || code[0] == '4')	// future code
	{
		if (code[1] == '0')
			return futureCODE;
		else
			return sfCODE;
	}
	else if (code[0] == '2' || code[0] == '3')
	{
		if (code[1] == '0')			// future option code
		{
			return optionCODE;
		}
		else					// 
		{
			return foptionCODE;
		}
	}

	return 0;
}

void CMapDlg::writeVtCode(int cx, int cy)
{
	CString	filePath; filePath.Format("%s/%s/%s", m_root, TABDIR, VTCODE);
	CString	keyName;  keyName = _T("IB200200");
	CString string;   string.Format(" %d %d Normal\t;관심종목전광판", cx, cy);

	WritePrivateProfileString("DLL Controls", keyName, string, filePath);
}

int CMapDlg::checkRealTimeCode(CString code, char* xrow)
{
	CString string;
	int	count = 0;
	const int realtimeCol = 0;

	for (int vidx = 0; vidx < maxVIEW; vidx++)
		xrow[vidx] = -1;

	CString strText(_T(""));

	for (vidx = 0; vidx < (int)m_viewType; vidx++)
	{
		for (int ii = 1; ii < m_rowC; ii++)
		{
			strText = m_grid[vidx]->GetItemText(ii, realtimeCol);

//			if (m_grid[vidx]->GetItemText(ii, realtimeCol) == code)
			if (!strText.IsEmpty() && strText == code)
			{
				xrow[vidx] = ii; count++;
				break;
			}
		}
	}

	return count;
}

int CMapDlg::getFlashIndex()
{
	int	readL;
	char	readB[128];
	CString	filePath; filePath.Format("%s/%s/%s", m_root, TABDIR, PALETTE);

	readL = GetPrivateProfileString(GENERALSN, FLASHSN, "8", readB, sizeof(readB), filePath);
	return atoi(CString(readB, readL));
}

void CMapDlg::repositionView()
{
	for (int vidx = viewTWO; vidx < maxVIEW; vidx++)
	{
		m_internm[vidx].ShowWindow(SW_SHOW);
		m_category[vidx]->ShowWindow(SW_SHOW);
		m_grid[vidx]->ShowWindow(SW_SHOW);
	}
	for (vidx = m_viewType; vidx < maxVIEW; vidx++)
	{
		m_internm[vidx].ShowWindow(SW_HIDE);
		m_category[vidx]->ShowWindow(SW_HIDE);
		m_grid[vidx]->ShowWindow(SW_HIDE);
	}
}

void CMapDlg::reloadscreen(CString fname, int fsize)
{
	m_bkBrush.DeleteObject();
	m_bkBrush.CreateSolidBrush(GetColor(64));
	m_blinkColor = GetColor(getFlashIndex());

	if (!fname.IsEmpty() || fsize != 0)
	{
		if (fname.IsEmpty()) fname = idTOstring(IDS_DEFAULTFONT);
		m_font = getAxFont(fname, fsize);
	}

	initialGrid(false); Invalidate();
	if (m_hawk.IsChecked()) hawkeyeAction(true);
}

void CMapDlg::saveMF()
{
	CString	filePath; filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, portFILE);

	CRect	rect;	GetClientRect(&rect);
	CString	string; string.Format("%04d:%04d", rect.Width()+2, rect.Height()+2);
	WritePrivateProfileString("IB200200", "size", string, filePath);

	string.Format("%01d", (int)m_viewType);
	WritePrivateProfileString("IB200200", "viewtype", string, filePath);
}

void CMapDlg::restoreMF()
{
	DWORD	wbL = 0; char	wbX[64];

	CString	filePath; filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, portFILE);

	wbL = GetPrivateProfileString("IB200200", "viewtype", "", wbX, sizeof(wbX), filePath);
	if (wbL > 0)
	{
		if (atoi(CString(wbX, wbL)) >= 2 && atoi(CString(wbX, wbL)) <= 4)
		{
			switch (atoi(CString(wbX, wbL)))
			{
			case 2: m_viewType = viewTWO;	break;
			case 3: m_viewType = viewTHREE;	break;
			case 4: m_viewType = viewFOUR;	break;
			}
		}
	}

	wbL = GetPrivateProfileString("IB200200", "size", "", wbX, sizeof(wbX), filePath);
	if (wbL <= 0) return;

	CString	string, stringx;
	
	string  = CString(wbX, wbL);
	stringx = parseX(string, ":");

	int	width  = atoi(stringx);
	int	height = atoi(string);

	if (width <= 0 || height <= 0)
		return;

	m_parent->PostMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(width-2, height-2));
	return;
}

void CMapDlg::hawkeyeAction(bool toggle)
{
	CString	code;

	if (toggle)	// hawkeye color
	{
		for (int vidx = 0; vidx < maxVIEW; vidx++)
		{
			for (int ii = 1; ii < m_rowC; ii++)
			{
				code = m_grid[vidx]->GetItemText(ii, colCODE);
				code.TrimLeft(); code.TrimRight();
				if (!code.IsEmpty()) hawkEyeColor(vidx, ii);
			}
		}
	}
	else
	{
		for (int vidx = 0; vidx < maxVIEW; vidx++)
		{
			for (int ii = 1; ii < m_rowC; ii++)
			{
				code = m_grid[vidx]->GetItemText(ii, colCODE);
				code.TrimLeft(); code.TrimRight();
				if (!code.IsEmpty())
				{
					m_grid[vidx]->SetItemFgColor(ii, colNAME, GetColor(69));
					m_grid[vidx]->SetItemBkColor(ii, colNAME, GetColor(68));
					m_grid[vidx]->SetItemBkColor(ii, colCURR, GetColor(68));
				}
			}
		}
	}
}

void CMapDlg::hawkeyeAction(int vidx)
{
	CString	code;
	for (int ii = 1; ii < m_rowC; ii++)
	{
		code = m_grid[vidx]->GetItemText(ii, colCODE);
		code.TrimLeft(); code.TrimRight();
		if (!code.IsEmpty()) hawkEyeColor(vidx, ii);
	}
}

void CMapDlg::hawkeyeClear(int vidx)
{
	CString	code;
	for (int ii = 1; ii < m_rowC; ii++)
	{
		code = m_grid[vidx]->GetItemText(ii, colCODE);
		code.TrimLeft(); code.TrimRight();
		if (!code.IsEmpty())
		{
			m_grid[vidx]->SetItemFgColor(ii, colNAME, GetColor(69));
			m_grid[vidx]->SetItemBkColor(ii, colNAME, GetColor(68));
			m_grid[vidx]->SetItemBkColor(ii, colCURR, GetColor(68));
		}
	}
}

const int colorKKRED  = PALETTERGB(255,   0,   0);
const int colorDKRED  = PALETTERGB(255, 137, 155);
const int colorLTRED  = PALETTERGB(255, 201, 206);

const int colorKKBLUE = PALETTERGB(  0,   0, 255);
const int colorDKBLUE = PALETTERGB(126, 176, 255);
const int colorLTBLUE = PALETTERGB(200, 222, 255);

void CMapDlg::hawkEyeColor(int vidx, int row)
{
	double	curr, open, high, low;
	double	pcurr, popen, phigh, plow, dval;

	curr  = TOfabs(m_grid[vidx]->GetItemText(row, colCURR));
	open  = TOfabs(m_grid[vidx]->GetItemText(row, colOPEN));
	high  = TOfabs(m_grid[vidx]->GetItemText(row, colHIGH));
	low   = TOfabs(m_grid[vidx]->GetItemText(row, colLOW));

	pcurr = TOfabs(m_grid[vidx]->GetItemText(row, colPCURR));
	popen = TOfabs(m_grid[vidx]->GetItemText(row, colPOPEN));
	phigh = TOfabs(m_grid[vidx]->GetItemText(row, colPHIGH));
	plow  = TOfabs(m_grid[vidx]->GetItemText(row, colPLOW));
	
	if (curr <= 0) curr = pcurr;
	if (curr <= 0) return;

	// colNAME foreground
	if (open >= phigh)	// 상승갭
	{
		m_grid[vidx]->SetItemFgColor(row, colNAME, colorKKRED);
	}
	else if (open <= plow)	// 하락갭
	{
		m_grid[vidx]->SetItemFgColor(row, colNAME, colorKKBLUE);
	}
	else
	{
		m_grid[vidx]->SetItemFgColor(row, colNAME, GetColor(69));
	}

	// colNAME background
	if (curr >= high)
	{
		m_grid[vidx]->SetItemBkColor(row, colNAME, colorLTRED);
	}
	else if (curr <= low)
	{
		m_grid[vidx]->SetItemBkColor(row, colNAME, colorLTBLUE);
	}
	else
	{
		m_grid[vidx]->SetItemBkColor(row, colNAME, GetColor(68));
	}

	dval = (phigh - plow) / 3.;

	// colCURR background
	if (curr >= phigh)
	{
		m_grid[vidx]->SetItemBkColor(row, colCURR, colorDKRED);
	}
	else if (curr >= (plow + dval*2))
	{
		m_grid[vidx]->SetItemBkColor(row, colCURR, colorLTRED);
	}
	else if (curr >= (plow + dval))
	{
		m_grid[vidx]->SetItemBkColor(row, colCURR, GetColor(68));
	}
	else if (curr >= plow)
	{
		m_grid[vidx]->SetItemBkColor(row, colCURR, colorLTBLUE);
	}
	else if (curr < plow)
	{
		m_grid[vidx]->SetItemBkColor(row, colCURR, colorDKBLUE);
	}
	else // default
	{
		TRACE("check  error....\n");
	}
}

bool CMapDlg::IsBlink()
{
	char	readB[128];
	CString filePath;

	filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, "axisensetup.ini");
	GetPrivateProfileString("SCREEN", "UNFLESH", "0", readB, sizeof(readB), filePath);

	return atoi(readB) == 0;
}

///////////////////////////////////////////////////////////////////////////
//
CString CMapDlg::parseX(CString &srcstr, CString substr)
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

CString CMapDlg::Variant(int comm, CString data)
{
	CString retvalue;
	char* dta = (char*)m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";

	return retvalue;
}

int CMapDlg::openView(int type, CString data)
{
	return m_parent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)(LPCTSTR)data);
}

void CMapDlg::sendTR(CString trCode, char* datB, int datL, int key)
{
	char* sendB = new char [L_userTH + datL + 1];

	struct	_userTH* uTH;
	uTH = (struct _userTH *) sendB;

	strcpy(uTH->trc, trCode);
	uTH->key = key;
	uTH->stat = US_PASS;

	CopyMemory(&sendB[L_userTH], datB, datL);
	m_parent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datL), (LPARAM) sendB);

	delete [] sendB;
}

CFont* CMapDlg::getAxFont(CString fName, int point, int style)
{
	struct _fontR fontR;

	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point; fontR.italic = false; fontR.bold = 0;

	switch (style)
	{
	case 0: case 1: // none
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
	return (CFont*)m_parent->SendMessage(WM_USER, getFONT, (long)&fontR);
}

CBitmap* CMapDlg::getAxBitmap(CString path)
{
	return (CBitmap*)m_parent->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}

COLORREF CMapDlg::GetColor(int color)
{
	if (color & 0x02000000)
		return color;

	return m_parent->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

double CMapDlg::TOfabs(CString string)
{
	double dval = 0;
	if (string.IsEmpty()) return dval;

	dval = fabs(atof(string));
	return dval;
}

double CMapDlg::TOf(CString string)
{
	double dval = 0;
	if (string.IsEmpty()) return dval;

	dval = atof(string);
	return dval;
}

CString CMapDlg::idTOstring(UINT id)
{
	CString	msg;
	if (!msg.LoadString(id))
		msg = _T("");
	return msg;
}

CString	CMapDlg::charTOstring(char* datB, int datL)
{
	CString	string = CString(datB, datL);
	string.TrimLeft(); string.TrimRight();
	return string;
}

UINT ParsingRTM(LPVOID lpvoid)
{
	CMapDlg* pMain = (CMapDlg *)lpvoid;
	if (pMain == NULL)
		return FALSE;

	char* pBytes = NULL;
	while (pMain->m_bThreadRTM)
	{
		if (pMain->m_arBytesRTM.GetUpperBound() < 0)
		{
			pMain->WaitRTM();
			continue;
		}
		pMain->m_syncRTM.Lock();
		pBytes = pMain->m_arBytesRTM.GetAt(0);
		pMain->m_arBytesRTM.RemoveAt(0);
		pMain->m_syncRTM.Unlock();

		pMain->doRTM(pBytes);
		delete pBytes;
		pBytes = NULL;
		
	}
	return 0;
}

//2012.03.16 KSJ AlertX 추가
UINT ParsingRTMx(LPVOID lpvoid)
{
	CMapWnd* pMain = (CMapWnd *)lpvoid;
	if (pMain == NULL)
		return FALSE;
	
	LPARAM lParam = NULL;
	
	while (pMain->m_bThreadRTM)
	{
		if (pMain->m_bRemove == TRUE || pMain->m_arBytesRTMx.GetUpperBound() < 0) // 2011.12.29 KDK
		{
			pMain->WaitRTM();
			continue;
		}
		pMain->m_syncRTM.Lock();		// 2011.12.29 KDK
		lParam = pMain->m_arBytesRTMx.GetAt(0);
		pMain->m_arBytesRTMx.RemoveAt(0);
		pMain->m_syncRTM.Unlock();		// 2011.12.29 KDK
		
		pMain->doRTMx(lParam);
	}
	return 0;
}
// KSJ

bool CMapDlg::WaitRTM()
{
	if (!m_pThreadRTM)
		return false;
	DWORD res = WaitForSingleObject(m_hEventRTM, 3000);
	switch (res)
	{
	case WAIT_OBJECT_0:
		return true;
	case WAIT_FAILED:
	case WAIT_ABANDONED:
	case WAIT_TIMEOUT:
		return false;
	}
	return true;
}

void CMapDlg::doRTM(char *pBytes)
{
	m_sync.Lock();
	if (m_grid[0] != NULL) parsingAlert(pBytes);
	m_sync.Unlock();
}

void CMapDlg::doRTMx(LPARAM lParam)
{
	//2012.02.13 KSJ 이벤트로 보내면 데이터가 깨져서 직접 메소드 호출함.
	if (m_grid[0] != NULL) parsingAlertx(lParam);
}

void CMapDlg::initRTM()
{
	if (m_bThreadRTM)
		return;

	m_bThreadRTM = true;
	// 	m_pThreadRTM = AfxBeginThread((AFX_THREADPROC) ParsingRTM, (LPVOID) this, THREAD_PRIORITY_NORMAL);
	m_pThreadRTM = AfxBeginThread((AFX_THREADPROC) ParsingRTMx, (LPVOID) this, THREAD_PRIORITY_NORMAL);
	m_hEventRTM = ::CreateEvent(NULL, FALSE, FALSE, NULL);	
}

void CMapDlg::stopRTM()
{
	if (!m_bThreadRTM)
		return;
	
	m_bThreadRTM = false;
	SetEvent(m_hEventRTM);
	m_pThreadRTM->PostThreadMessage(WM_QUIT, 0, 0);
	switch (WaitForSingleObject(m_pThreadRTM->m_hThread, 1000))
	{
	case WAIT_OBJECT_0:
	case WAIT_FAILED:
	case WAIT_ABANDONED:
		break;
	case WAIT_TIMEOUT:
		TerminateThread(m_pThreadRTM->m_hThread, 0);
		break;
	}
	removeRTM();
}

void CMapDlg::removeRTM()
{
	m_syncRTM.Lock();
	char* pBytes = NULL;
	for (int ii = m_arBytesRTM.GetUpperBound(); ii >= 0; ii--)
	{
		pBytes = m_arBytesRTM.GetAt(ii);
		delete pBytes;	pBytes = NULL;
	}
	m_arBytesRTM.RemoveAll();
	m_syncRTM.Unlock();
}

//2012.03.16 KSJ alertX 추가
void CMapDlg::removeRTMx()
{
	m_bRemove = TRUE;		// 2011.12.29 KDK
	
	m_syncRTM.Lock();
	LPARAM lParam = NULL;
	for (int ii = m_arBytesRTMx.GetUpperBound(); ii >= 0; ii--)
	{
		lParam = m_arBytesRTMx.GetAt(ii);
	}
	m_arBytesRTMx.RemoveAll();
	m_syncRTM.Unlock();
	
	m_bRemove = FALSE;		// 2011.12.29 KDK
}
// KSJ

void CMapDlg::SendMinMaxInfo()
{
	int nKey = m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), NULL);

	if (nKey == 0)
		return;

	int nWidth = 0;
	int colW, rowH; 
	m_grid[0]->GetCellSize(0, dispRow, colW, rowH); 
	nWidth = (colW+scrollW) * m_viewType;

	m_MMI.ptReserved.x = 13;
	m_MMI.ptReserved.y = 0;
	m_MMI.ptMinTrackSize.x = nWidth + 10 - m_viewType;
	m_MMI.ptMinTrackSize.x = nWidth + 10 - m_viewType;
	m_MMI.ptMaxSize.x = nWidth;

	AfxGetMainWnd()->SendMessage(WM_AXIS, MAKEWPARAM(axMinMaxInfo, nKey), (LPARAM)&m_MMI);
}
