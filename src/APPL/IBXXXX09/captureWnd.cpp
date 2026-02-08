#include "stdafx.h"
#include "IBXXXX09.h"
#include "captureWnd.h"
#include "../../h/interSt.h"
#include "../../control/fx_misc/misctype.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int gap2 = 2;
const int szWIDTH = 300, szHEIGHT = 198;
const int szTITLEH = 25;
const int idBUTTON = 1003;

const int idGRID = 1004;
const int rowH	 = 20;

const COLORREF	fgCOLOR = PALETTERGB(  0,   0,   0);
const COLORREF	bkCOLOR = PALETTERGB(249, 250, 253);
const COLORREF	tlCOLOR = PALETTERGB(200, 192, 241);
const COLORREF	bxCOLOR = PALETTERGB(192, 192, 192);
const COLORREF	bwCOLOR = PALETTERGB(255, 255, 255);
const COLORREF	cpCOLOR = PALETTERGB(255, 255, 196);
const COLORREF	bnCOLOR = PALETTERGB(200, 200, 200);
const COLORREF	gtCOLOR = PALETTERGB(237, 240, 248);

/////////////////////////////////////////////////////////////////////////////
// CCaptureWnd

CCaptureWnd::CCaptureWnd(CWnd* parent, CFont* font, CFont* bfont)
{
	m_parent = parent;
	m_font   = font;
	m_bfont = bfont;
}

CCaptureWnd::~CCaptureWnd()
{
}


BEGIN_MESSAGE_MAP(CCaptureWnd, CWnd)
	//{{AFX_MSG_MAP(CCaptureWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCaptureWnd message handlers
BOOL CCaptureWnd::CreatePopup(int key, const void* info, void* data, int wndpos)
{
	m_key = key;

	if (!CreateEx(0, AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(nullptr, IDC_ARROW)),
		_T("capture popup"), WS_POPUP|WS_BORDER, 0, 0, szWIDTH, szHEIGHT,
		m_parent->GetSafeHwnd(), (HMENU) 0, nullptr))
	{
//		TRACE("Error create capture popup...\n");
		return FALSE;
	}

	m_winRC.SetRect(0, 0, szWIDTH, szHEIGHT);
	m_titRC.SetRect(0, 0, szWIDTH, szTITLEH);

	m_boxBrush.CreateSolidBrush(bxCOLOR);
	m_boxRC.SetRect(gap2, szTITLEH + gap2, szWIDTH - (gap2*2), szHEIGHT - (gap2*2));

	positionWindow(); ShowWindow(SW_SHOW);  initialControl(); displayData(info, data);
	return true;
}

void CCaptureWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CMemDC	memDC(&dc);
	
	memDC.FillSolidRect(m_winRC, bkCOLOR);
	memDC.FillSolidRect(m_titRC, tlCOLOR);

	memDC.FrameRect(m_boxRC, &m_boxBrush);

	CFont* oldFont = memDC.SelectObject(m_font);

	memDC.SetBkMode(TRANSPARENT);
	memDC.DrawText(idTOstring(IDS_TXTCAPTURE), m_titRC, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	memDC.SelectObject(oldFont);
}

BOOL CCaptureWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	switch (LOWORD(wParam))
	{
	case idBUTTON:
		if (m_parent)	
			m_parent->PostMessage(WM_CAPTURE, wpCONFIRM, (LPARAM)m_key);
		break;
	default:
		break;
	}

	return CWnd::OnCommand(wParam, lParam);
}

void CCaptureWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_titRC.PtInRect(point))
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));	

	CWnd::OnLButtonDown(nFlags, point);
}

void CCaptureWnd::initialControl()
{
	const	int nHEIGHT = 22, nWIDTH = 80;
	//CRect	winRC(0, 0, szWIDTH, szHEIGHT), 
	
	CRect rect;

	rect.left = m_boxRC.left + gap2; rect.right  = rect.left + nWIDTH;
	rect.top  = m_boxRC.top + gap2;  rect.bottom = rect.top + nHEIGHT;
	m_code.Create("", WS_CHILD|WS_VISIBLE, rect, this, 1001);
	m_code.Init(ssTEXT|ssCOLOR|ssBORDER, 0, "", "", cpCOLOR, fgCOLOR, alLEFT); m_code.SetFont(m_bfont, false);

	rect.left  = rect.right+1; rect.right = rect.left + (nWIDTH *2);
	m_name.Create("", WS_CHILD|WS_VISIBLE, rect, this, 1002);
	m_name.Init(ssTEXT|ssCOLOR|ssBORDER, 0, "", "", cpCOLOR, fgCOLOR, alLEFT); m_name.SetFont(m_bfont, false);

	rect.left  = rect.right+(gap2*2); rect.right = rect.left + nWIDTH/2;
	m_ok.Create("", WS_CHILD|WS_VISIBLE, rect, this, idBUTTON);
	m_ok.Init(bsTEXT|bsCOLOR|bsPUSH, idTOstring(IDS_CONFIRM), "", bnCOLOR, fgCOLOR); m_ok.SetFont(m_bfont, false);

	LOGFONT lf; m_font->GetLogFont(&lf); lf.lfHeight = 90;
	rect = m_boxRC; rect.top += nHEIGHT + gap2; rect.DeflateRect(CSize(2, 2));
	m_grid = std::make_unique<CfxGrid>(&lf); m_grid->Create(rect, this, idGRID, GVSC_NONE);

	const int	rowC = 7, colC = 5;
	m_grid->Initial(rowC, colC, 1, 1);

	const struct	_gridHdr {
		UINT	stid;		// string table id
		UINT	width;		// fixed width

		UINT	format;		// nonfixed format
		UINT	attrx;		// nonfixed attr
	} gridHdr[] = {
		{0,			52,	GVFM_CENTER,	0},
		{IDS_GH_CAPTURE,	75,	GVFM_RIGHT,	GVAT_COMMA}, 
		{IDS_GH_CONDITION,	54,	GVFM_CENTER,	0},
		{IDS_GH_OPERATION,	34,	GVFM_CENTER,	0},
		{IDS_GH_CONFIGVAL,	75,	GVFM_RIGHT,	GVAT_COMMA},
	};

	const UINT	colHeader[] = { IDS_GH_CURR, IDS_GH_DIFF, IDS_GH_ASK1, IDS_GH_BID1, IDS_GH_RATE, IDS_GH_VOL };

	GVITEM	gvitem;

 	// column header setting
	for (int ii = 0; ii < colC; ii++)
	{
		gvitem.row   = 0;
		gvitem.col   = ii;
		gvitem.state = 0;
		gvitem.mask  = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_MISC;
		gvitem.format= GVFM_HEADER;
		gvitem.fgcol = fgCOLOR;
		gvitem.bkcol = gtCOLOR;

		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));

		gvitem.text  = (gridHdr[ii].stid != -1) ? idTOstring(gridHdr[ii].stid) : "";
		gvitem.attr  = 0;

		m_grid->SetItem(&gvitem);
		m_grid->SetColumnWidth(ii, gridHdr[ii].width);
	}

	for (int ii = 0; ii < colC; ii++)
	{
		gvitem.state = 0;
		gvitem.mask  = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR;

		gvitem.format= gridHdr[ii].format;
		gvitem.attr  = gridHdr[ii].attrx;
		gvitem.fgcol = fgCOLOR;
		gvitem.bkcol = (!ii) ? gtCOLOR : bwCOLOR;

		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));

		m_grid->SetColumnItems(ii, &gvitem);
	}

	for (int ii = 0; ii < m_grid->GetRowCount(); ii++)
	{
		m_grid->SetRowHeight(ii, rowH);
		if (ii) m_grid->SetItemText(ii, 0, idTOstring(colHeader[ii-1]));
	}

	m_grid->SetRowColResize(FALSE, FALSE);
	m_grid->SetGridFocusLine(FALSE, FALSE);
	m_grid->SetGridColor(bxCOLOR);
	m_grid->Adjust();
}

void CCaptureWnd::displayData(const void* info, void* data)
{
	if (m_grid == nullptr)
		return;
	CString	string, stringx = (char*) data;
	struct	_codeinfo* cinfo = (_codeinfo *) info;

	string.Format("%s", chTOstring(cinfo->code, sizeof(cinfo->code))); m_code.SetText(string);
	string.Format("%s", chTOstring(cinfo->name, sizeof(cinfo->name))); 
	if (!string.IsEmpty() && string.GetAt(0) == '&')
		string = string.Mid(1);
	m_name.SetText(string);

	CString valS;
	for (int ii = 0; ii < m_grid->GetRowCount(); ii++)
	{
		string = parseX(stringx, "\t");
		if (cinfo->alarm[ii].kind[0] >= '1')
		{
			valS = chTOstring(cinfo->alarm[ii].aval, sizeof(cinfo->alarm[ii].aval));
			m_grid->SetItemText(ii+1, 1, string);
			m_grid->SetItemText(ii+1, 2, cinfo->alarm[ii].cond[0] == '0' ? ">=(이상)" : "<=(이하)");
			if (ii != 0)	m_grid->SetItemText(ii+1, 3, cinfo->alarm[ii].oper[0] == '0' ? "AND" : "OR");
			m_grid->SetItemText(ii+1, 4, valS);
		}
	}
}

#include "../../H/axisfire.h"
CString CCaptureWnd::Variant(int comm, CString data)
{
	CString retvalue;
	char* dta = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";

	return retvalue;
}


void CCaptureWnd::positionWindow()
{
	const int captionH = ::GetSystemMetrics(SM_CYCAPTION);
	int	xPosition = 0, yPosition = 0;
	CRect	parentRC; 
	//const gsl::not_null<CWnd*> parentW = AfxGetMainWnd();
	CWnd* parentW = AfxGetMainWnd();

	if (parentW == nullptr)
	{
		OutputDebugString("\r\n[IBXXXX09] ------------  nullptr\r\n");

		CString strdata, name;
		name = Variant(nameCC, "");

		int readL = 0;
		char readB[1024];
		CString userD;
		userD.Format("%s\\%s\\%s\\%s.ini", Variant(homeCC, ""), "user", name, name);
		readL = GetPrivateProfileString("MODE", "main", "", readB, sizeof(readB), userD);

		if (readL <= 0)
			return;

		CString sWnd;
		sWnd.Format("%s", readB);
		int ddata = atoi(sWnd);
		HWND hWnd = (HWND)ddata;
		CWnd* m_pMainFrame = CWnd::FromHandle(hWnd);

		if (m_pMainFrame && m_pMainFrame->GetSafeHwnd())
		{
				m_pMainFrame->ShowWindow(SW_RESTORE);
				m_pMainFrame->SetForegroundWindow();
		}

		CenterWindow(m_pMainFrame);
		ShowWindow(SW_SHOW);

		return;
	}
	parentW->GetWindowRect(&parentRC); 
	parentRC.top += captionH;

	switch (m_wndpos)
	{
	case wpLT:		// left_top
		xPosition = parentRC.left;
		yPosition = parentRC.top;
		break;

	case wpRT:		// right_top
		xPosition = parentRC.right - m_winRC.Width();
		yPosition = parentRC.top;
		break;

	case wpLB:	// left_bottom
		xPosition = parentRC.left;
		yPosition = parentRC.bottom - m_winRC.Height();
		break;

	case wpRB:	// right_bottom
		xPosition = parentRC.right - m_winRC.Width();
		yPosition = parentRC.bottom - m_winRC.Height();
		break;
	default:
		CenterWindow(parentW); 
		return;
	}

	SetWindowPos(nullptr, xPosition, yPosition, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_SHOWWINDOW);
}

CString CCaptureWnd::idTOstring(UINT id)
{
	CString	msg;
	if (!msg.LoadString(id))
		msg = _T("");
	return msg;
}

CString CCaptureWnd::chTOstring(char* chB, int chL)
{
	CString string = CString(chB, chL);
	string.TrimRight(); return string;
}

CString CCaptureWnd::parseX(CString &srcstr, CString substr)
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


void CCaptureWnd::OnDestroy() 
{
	CWnd::OnDestroy();
}
