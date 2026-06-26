// CDlg_Grid.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "afxdialogex.h"
#include "CDlg_Grid.h"

#include "CAxGrid.h"

#ifdef DF_ALL

#include "griditem.h"

#include "ctrl/resource.h"

#define GAP  5
#define USRDIR "user"
#define IMAGEDIR "image"
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

const int gap1 = 1;
const int gap2 = 2;
const int gap3 = 3;
const int gap5 = 5;
const int comboH = 20;
const int offsetGH = gap5 + comboH + gap3;

const int comboW = 102;
const int size18 = 20;
const int size44 = 36;


const int maxGROUP = 100, maxJONGMOK = 100;

const char* gSYMBOL = _T("$90303");
const char* gEXPECT = _T("90991");

const char	gnCHAR = 0x7f;
const char	tabCHAR = 0x09;
const char	lfCHAR = 0x0A;

const int	dispRow = 20 + 1;
const int	scrollW = ::GetSystemMetrics(SM_CYHSCROLL);

const int	colNAME = 0, colCURR = 1, colDIFF = 2;

#define	TM_ATTACH	1233

#define ID_GRID 9899

#endif


// CDlg_Grid 대화 상자

IMPLEMENT_DYNAMIC(CDlg_Grid, CDialogEx)

CDlg_Grid::CDlg_Grid(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{
#ifdef DF_ALL
	m_fontsize = 9;
#endif
}

CDlg_Grid::~CDlg_Grid()
{
}

void CDlg_Grid::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_Grid, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlg_Grid::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlg_Grid 메시지 처리기


void CDlg_Grid::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


BOOL CDlg_Grid::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
#ifdef DF_ALL

#ifdef DF_NOHTS
	m_root = _T("F:\\util\\HTS\\IBK투자증권 HTS");
	m_name = "3477995568";
#endif

	init();

#endif
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

#ifdef DF_ALL
void CDlg_Grid::init()
{

   //SetOwner(this);  //????  이거 하면 죽는다 자신말고 부모를 해야 한다
	//Variant(titleCC, idTOstring(IDS_TITLE));  //@@@

	m_bkBrush.DeleteObject();
	m_bkBrush.CreateSolidBrush(GetColor(64));
	m_blinkColor = GetColor(getFlashIndex());
   	m_bookmarkColor = GetColor(199);
	
	m_font = getAxFont(idTOstring(IDS_DEFAULTFONT), m_fontsize);	
	m_fontB = getAxFont(idTOstring(IDS_DEFAULTFONT), m_fontsize, 3);

	loadGridinfo();
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
	
	int  ii = 0;
	CRect recgrid{};
	for_each(_grid.begin(), _grid.end(), [&](auto& grid){
		clientRC.left = m_gridW * ii + (ii * 10);
		clientRC.right = clientRC.left + m_gridW;

		grid = std::make_unique<AxGrid>();
		grid->SetUserFont(m_font, m_fontB);
		recgrid = clientRC; 
		recgrid.DeflateRect(10, 0);
		grid->creategrid(clientRC, this, 20, 300, ID_GRID + ii++);
	});
	
	initialGrid(false); 

	
	repositionView();
	m_blink = IsBlink();
/*	SendMinMaxInfo();
 */
}

bool CDlg_Grid::IsBlink()
{

	char	readB[128];
	CString filePath;

	filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, "axisensetup.ini");
	GetPrivateProfileString("SCREEN", "UNFLESH", "0", readB, sizeof(readB), filePath);

	return atoi(readB) == 0;

}

void CDlg_Grid::repositionView()
{

	for (int vidx = viewTWO; vidx < maxVIEW; vidx++)
	{
		m_internm[vidx].ShowWindow(SW_SHOW);
#ifndef DF_NOHTS
		m_category[vidx]->ShowWindow(SW_SHOW);
#endif
		_grid[vidx]->ShowWindow(SW_SHOW);
	}
	for (int vidx = m_viewType; vidx < maxVIEW; vidx++)
	{
		m_internm[vidx].ShowWindow(SW_HIDE);
#ifndef DF_NOHTS
		m_category[vidx]->ShowWindow(SW_HIDE);
#endif
		_grid[vidx]->ShowWindow(SW_HIDE);
	}
}


int CDlg_Grid::loadInterName(bool bcursel)
{
#ifndef DF_NOHTS
	Request_GroupList();
#endif
	return 0;
}

void CDlg_Grid::initControl()
{
	int	xpos = 0;
	CRect	rect, clientRC;
	GetClientRect(clientRC);

	CString	imgPath;
	imgPath.Format("%s\\%s\\", m_root, IMAGEDIR);

	m_bitmapH_nm = getAxBitmap(imgPath + "H버튼.bmp")->operator HBITMAP();
	m_bitmapH_dn = getAxBitmap(imgPath + "H버튼_dn.bmp")->operator HBITMAP();
	m_bitmapH_hv = getAxBitmap(imgPath + "H버튼_en.bmp")->operator HBITMAP();

	rect.top = clientRC.top + gap2;

	for (int vidx = 0; vidx < maxVIEW; vidx++)
	{
		rect.left = clientRC.left + (m_gridW * vidx);
		rect.right = rect.left + comboW;
		rect.bottom = rect.top + 200;

		if (vidx == 0) rect.left += gap2;

		m_internm[vidx].Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, rect, this, IDC_INTERNM0 + vidx);
		m_internm[vidx].SetFont(m_font, false);
		rect.bottom = rect.top + comboH;
		m_internm[vidx].MoveWindow(&rect);

		rect.left = rect.right + gap2;
		rect.right = rect.left + size18;
		rect.bottom = rect.top + size18;

#ifndef DF_NOHTS
		m_category[vidx] = std::make_unique<CCategory>(m_parent, categoryKEY + vidx);
		m_category[vidx]->CreateCategory(this, rect, m_root);
#endif
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
	
#ifdef DF_NOHTS
	m_expect.Create(
		"예상",   // 버튼 텍스트 (리소스 문자열 변환)
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // 버튼 스타일
		rect,    // 버튼의 위치 및 크기
		this,    // 부모 윈도우
		IDC_EXPECT // 컨트롤 ID
	);
#else
	m_expect.Create(idTOstring(IDS_EXPECT), rect, this, IDC_EXPECT, true);
	m_expect.SetImgBitmap(m_bitmap2_nm, m_bitmap2_dn, m_bitmap2_hv);
#endif
    m_expect.SetFont(m_font, false);

#ifdef DF_NOHTS
	m_config.Create(
		"설정",   // 버튼 텍스트 (리소스 문자열 변환)
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // 버튼 스타일
		rect,    // 버튼의 위치 및 크기
		this,    // 부모 윈도우
		IDC_CONFIG // 컨트롤 ID
	);
#else
	m_config.Create(idTOstring(IDS_CONFIG), rect, this, IDC_CONFIG);
	m_config.SetImgBitmap(m_bitmap2_nm, m_bitmap2_dn, m_bitmap2_hv);
#endif
	m_config.SetFont(m_font, false);
	
#ifdef DF_NOHTS
	m_minus.Create(
		"-",   // 버튼 텍스트 (리소스 문자열 변환)
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // 버튼 스타일
		rect,    // 버튼의 위치 및 크기
		this,    // 부모 윈도우
		IDC_MINUS // 컨트롤 ID
	);
#else
	m_minus.Create("", rect, this, IDC_MINUS);
	m_minus.SetImgBitmap(m_bitmapM_nm, m_bitmapM_dn, m_bitmapM_hv);
#endif
	m_minus.SetFont(m_font, false);

#ifdef DF_NOHTS
	m_plus.Create(
		"+",   // 버튼 텍스트 (리소스 문자열 변환)
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // 버튼 스타일
		rect,    // 버튼의 위치 및 크기
		this,    // 부모 윈도우
		IDC_PLUS // 컨트롤 ID
	);
#else
	m_plus.Create("", rect, this, IDC_PLUS);
	m_plus.SetImgBitmap(m_bitmapP_nm, m_bitmapP_dn, m_bitmapP_hv);
#endif
	m_plus.SetFont(m_font, false);
	

	rect.top = clientRC.top + gap2;
	rect.bottom = rect.top + size18;

	xpos = (m_gridW * m_viewType) - ((size18 + gap1 + size44 + gap1) * 2);

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

CBitmap* CDlg_Grid::getAxBitmap(CString path)
{
#ifdef DF_NOHTS
	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	if (hBitmap == NULL)
		return nullptr;

	// CBitmap 객체를 동적으로 생성하고 HBITMAP을 연결
	CBitmap* pBitmap = new CBitmap();
	pBitmap->Attach(hBitmap);
	return pBitmap;
#else
	return (CBitmap*)m_parent->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
#endif
}

void CDlg_Grid::restoreMF()
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

	string = CString(wbX, wbL);
	stringx = parseX(string, ":");

	const int width = atoi(stringx);
	const int height = atoi(string);

	if (width <= 0 || height <= 0)
		return;

#ifdef DF_NOHTS
	
#else
	m_parent->PostMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(width - 2, height - 2));
#endif
	return;
}

CString CDlg_Grid::parseX(CString& srcstr, CString substr)
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

void CDlg_Grid::loadGridinfo()
{
	CString filePath;
	filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, interFILE);
	int	readL = 0;
	char	readB[1024];
	CString	string, stringx;

	readL = GetPrivateProfileString("INTERFIELD", "BOOKMARKCOLOR", "", readB, sizeof(readB), filePath);
	if (readL > 0) m_bookmarkColor = _httoi(readB);
}

int CDlg_Grid::_httoi(const TCHAR* value)
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
	std::unique_ptr < TCHAR[] > mstr = std::make_unique<TCHAR[]>(strlen(value) * 2 + 1);

	TCHAR* temp = _tcsdup(value);

	if (temp) {
		_tcsupr_s(temp, _tcslen(temp) + 1);  // 안전한 대문자 변환
		memcpy(mstr.get(), temp, _tcslen(temp));
	}
	//memcpy(mstr.get(), _tcsupr(_tcsdup(value)), strlen(value));

	TCHAR* ss = mstr.get();
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

	return result;
}

COLORREF CDlg_Grid::GetColor(int color)
{
#ifdef DF_NOHTS
	TCHAR section[] = _T("Palette");
	TCHAR key[16];
	_stprintf_s(key, _T("RGB%03d"), color); // RGB000, RGB001 형태의 키 생성

	TCHAR buffer[32];

	CString	filePath;
	filePath.Format("%s\\%s\\%s", m_root, "tab", "PALETTE.INI");
	GetPrivateProfileString(section, key, _T("0,0,0"), buffer, sizeof(buffer) / sizeof(TCHAR), filePath);

	int r, g, b;
	_stscanf_s(buffer, _T("%d, %d, %d"), &r, &g, &b); // 문자열을 정수로 변환

	return RGB(r, g, b); // COLORREF 값 반환
#else
	//if (color & 0x02000000)
	//	return color;

	//return m_parent->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)color);

#endif
}

int CDlg_Grid::getFlashIndex()
{
	int	readL{};
	char	readB[128];
	//CString	filePath; filePath.Format("%s/%s/%s", m_root, TABDIR, PALETTE);

	//readL = GetPrivateProfileString(GENERALSN, FLASHSN, "8", readB, sizeof(readB), filePath);
	return atoi(CString(readB, readL));
}

void CDlg_Grid::loadFieldInfo()
{
	const struct _gridHdr gridHdr[] = {
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
		{IDS_GH_CURR,	60,	GVAT_SORTABS,		"2023",	GVFM_RIGHT,	GVAT_COMMA | GVAT_CONDITIONx,	4},	// 10
		{IDS_GH_DIFF,	60,	GVAT_SORTVAL,		"2024",	GVFM_RIGHT,	GVAT_COMMA | GVAT_CONTRAST,	4},	// 11
		{IDS_GH_RATE,	46,	GVAT_SORTVAL,		"2033",	GVFM_RIGHT,	GVAT_COMMA | GVAT_CONDITION,	4},	// 12
		{IDS_GH_VOL,	75,	GVAT_SORTABS,		"2027",	GVFM_RIGHT,	GVAT_COMMA | GVAT_CONDITIONx,	3},	// 13
		{IDS_GH_ASK,	60,	GVAT_SORTABS,		"2025",	GVFM_RIGHT,	GVAT_COMMA | GVAT_CONDITIONx,	2},	// 14
		{IDS_GH_BID,	60,	GVAT_SORTABS,		"2026",	GVFM_RIGHT,	GVAT_COMMA | GVAT_CONDITIONx,	2},	// 15
	};

	_vgridHdr.clear();
	const int gridHdrC = sizeof(gridHdr) / sizeof(_gridHdr);

	m_gridW = 0;
	for (int ii = 0; ii < gridHdrC; ii++)
	{
		if (gridHdr[ii].fidx >= (UINT)m_viewType)
		{
			_vgridHdr.push_back(gridHdr[ii]);
			if (!(gridHdr[ii].attr & GVAT_HIDDEN))
				m_gridW += gridHdr[ii].width;
		}
	}

	if (m_gridW > 0) m_gridW += scrollW;
}

void CDlg_Grid::initialGrid(int size)
{
	LOGFONT lf{}, lfB{};
	m_font->GetLogFont(&lf); lf.lfHeight = m_fontsize * 10;
	m_fontB->GetLogFont(&lfB); lfB.lfHeight = m_fontsize * 10;

	for (int vidx = 0; vidx < maxVIEW; vidx++)
	{
		while (_grid[vidx]->DeleteColumn(0)) {};
		int col = 0;
		auto* harr = _grid[vidx]->getHeaderArr();
		harr->clear();
		for_each(_vgridHdr.begin(), _vgridHdr.end(), [&](auto& item) {
			if (item.attr == GVAT_HIDDEN)
				return;

			harr->emplace_back(idTOstring(item.stid), item.width, item.attr, item.format, item.attrx, true, CString(item.symbol, 8).Trim());
			_grid[vidx]->InsertColumn(col++, idTOstring(item.stid), DT_CENTER | DT_VCENTER, item.width);
			});
		_grid[vidx]->SetBKSelColor(GetColor(78));
		_grid[vidx]->SetConditionColor(GetColor(94), GetColor(95));
		_grid[vidx]->SetStepColor(1, GetColor(68), GetColor(77));
		_grid[vidx]->SetHeaderColor(GetColor(76), GetColor(74));
	}

	if (size)
	{
		int colW{}, rowH{};
		CRect rc;
		_grid[0]->GetClientRect(rc);
		colW = m_gridW;
		rowH = rc.Height();

		colW = colW * m_viewType;

#ifndef DF_NOHTS
		m_parent->PostMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(colW - m_viewType, rowH + offsetGH + 1));
		writeVtCode(colW, rowH + offsetGH + 1);
#endif
	}
}

CFont* CDlg_Grid::getAxFont(CString fName, int point, int style)
{
	struct _fontR fontR {};
	
	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point;
	fontR.italic = false;
	fontR.bold = 0;

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

#ifdef DF_NOHTS
	LOGFONT lf = { 0 };

	// 글꼴 크기 설정 (포인트 단위를 픽셀로 변환)
	CClientDC dc(NULL);  // 화면 DC 생성
	int nLogPixelsY = dc.GetDeviceCaps(LOGPIXELSY);  // DPI 가져오기
	lf.lfHeight = -MulDiv(fontR.point, GetDeviceCaps(dc, LOGPIXELSY), 72);

	// 글꼴 이름 복사
	lstrcpy(lf.lfFaceName, fontR.name);

	// 기울기 및 볼드 설정
	lf.lfItalic = fontR.italic;
	lf.lfWeight = fontR.bold;

	// CFont 객체 생성 및 초기화
	CFont* pFont = new CFont();
	if (!pFont->CreateFontIndirect(&lf)) {
		delete pFont;
		return nullptr;
	}

	return pFont;
#else
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
#endif
}

double CDlg_Grid::TOfabs(CString string)
{
	double dval = 0;
	if (string.IsEmpty()) return dval;

	dval = fabs(atof(string));
	return dval;
}

double CDlg_Grid::TOf(CString string)
{
	double dval = 0;
	if (string.IsEmpty()) return dval;

	dval = atof(string);
	return dval;
}

CString CDlg_Grid::idTOstring(UINT id)
{
	CString	msg;
	if (!msg.LoadString(id))
		msg = _T("");
	return msg;
}

CString	CDlg_Grid::charTOstring(char* datB, int datL)
{
	CString	string = CString(datB, datL);
	string.TrimLeft(); string.TrimRight();
	return string;
}
#endif
