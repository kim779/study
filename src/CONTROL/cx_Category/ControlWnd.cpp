// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "cx_category.h"
#include "controlWnd.h"
#include "dataio.h"

#include "../../h/axisvar.h"
#include "../../h/memDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// TR KEY
#define	TR_JRNK		1	// 등락종목
#define	TR_IGROUP	2	// 관심종목 그룹
#define	TR_IDATA	3	// 관심종목 그룹의 종목

#define	TM_AUTO		1001

/////////////////////////////////////////////////////////////////////////////
// CControlWnd

CControlWnd::CControlWnd(BOOL bMini)
{
	EnableAutomation();

	m_bShowPlay	= true;
	m_capture	= false;
	m_bRotate	= false;
	m_nLoads	= 0;

	m_pPopup	= nullptr;
	for (int ii = OBJ_R; ii <= OBJ_D; ii++)
	{
		m_hBitmap[ii] = m_hBitmap_dn[ii] = m_hBitmap_hv[ii] = nullptr;
		m_bHover[ii] = FALSE;
		m_rcBtn[ii].SetRectEmpty();
	}

	InitRotate();
	m_bTracking	= FALSE;
	
	memset(m_bCateChecked , 0x00, sizeof(bool) * CHECKER_COUNT);

	m_nAllCode = 0;
	m_pAllCode = nullptr;
	m_nSelectedCode = 0;
	m_pSelectedCode = nullptr;
	m_bMini = bMini;
}

CControlWnd::~CControlWnd()
{
}

void CControlWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_ERASEBKGND()
	ON_WM_ENABLE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_CATEGORY, OnCategory)
	ON_MESSAGE(WM_POPUPMENU, OnPopupMenu)
	ON_MESSAGE(WM_INITLOAD, OnInitLoad)
	ON_MESSAGE(WD_SIDE, OnCategoryUpDown)
	ON_MESSAGE(WD_SETVALUE, OnSetValue)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_PROPERTY_EX(CControlWnd, "JCodes", GetJCodes, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CControlWnd, "SelectedName", _GetSelectedName, _SetSelectedName, VT_BSTR)
	DISP_PROPERTY_EX(CControlWnd, "Data", GetData, SetData, VT_BSTR)
	DISP_FUNCTION(CControlWnd, "GetCurCode", GetCurCode, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "GetPreCode", GetPreCode, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "GetNextCode", GetNextCode, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "GetHandle", GetHandle, VT_I4, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {712BD4F1-954D-4EC4-ADB6-B12E785BDA69}
static const IID IID_IControlWnd =
{ 0x712bd4f1, 0x954d, 0x4ec4, { 0xad, 0xb6, 0xb1, 0x2e, 0x78, 0x5b, 0xda, 0x69 } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers

long CControlWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:
		return (long)m_rtnStr.GetString();
	case DLL_OUBx:
		struct	_extTHx* exth;
		exth = (struct _extTHx*)lParam;
		switch (exth->key)
		{
		case TR_JRNK:	// 등락종목
			if (exth->size > 4)
			{
				CString sCodes, str = exth->data;
				int	nRec = min(120, atoi(CString(str, 4)));

				str = str.Mid(4);
				for (int ii = 0; ii < nRec; ii++)
				{
					CString code = CString(str, 6);
					str = str.Mid(22);
					code.Trim();
					sCodes += code;
					sCodes += '\t';
				}
				m_sCodeList = sCodes;
	//			m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk)),
	//					(LPARAM)m_Param.name.GetString());
			}
			break;

		case TR_IGROUP:	// 관심종목 그룹
			m_pPopup->LoadGroup(exth->data, exth->size);
			break;

		default:
			if (exth->key >= TR_IDATA && exth->key <= TR_IDATA + procEVENT)		// 관심종목 그룹의 종목
				parseCode(exth->data, exth->size, exth->key - TR_IDATA);
			break;
		}
		break;

	case DLL_OUB:	// IN MAP : NOT USED. User DLL_OUBx Only!!
			// IN DLL : Available
		{
			int	key = HIBYTE(LOWORD(wParam));
			int	size = HIWORD(wParam);

			switch (key)
			{
			case TR_JRNK:	// 등락종목
				if (size > 4)
				{
					CString sCodes, str = (char*)lParam;
					int	nRec = min(120, atoi(CString(str, 4)));

					str = str.Mid(4);
					for (int ii = 0; ii < nRec; ii++)
					{
						CString code = CString(str, 6);
						str = str.Mid(22);
						code.Trim();
						sCodes += code;
						sCodes += '\t';
					}
					m_sCodeList = sCodes;
		//			m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk)),
		//					(LPARAM)m_Param.name.GetString());
				}
				break;

			case TR_IGROUP:	// 관심종목 그룹
				m_pPopup->LoadGroup((char*)lParam, size);
				break;

			default:
				if (key >= TR_IDATA && key <= TR_IDATA + procEVENT)		// 관심종목 그룹의 종목
					parseCode((char*)lParam, size, key - TR_IDATA);
				break;
			}
		}
		break;

	case DLL_ALERT:
		break;
	case DLL_TRIGGER:
		break;
	case DLL_DOMINO:
		break;
	case DLL_NOTICE:
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
	}
	return 0;
}
void CControlWnd::SetParam(struct _param *pParam)
{
	m_Param.key = pParam->key;
	m_Param.name = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	m_Param.tRGB = pParam->tRGB;
	m_Param.pRGB = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());

	CString tmp = m_Param.options;
	m_nKind = atoi(OptionParser(tmp, "/k"));
	m_sImagePath[OBJ_R] = OptionParser(tmp, "/i");
	m_sText = OptionParser(tmp, "/t");
	m_nUcmd = atoi(OptionParser(tmp, "/x"));	// updateX_20050330
	m_jCount = OptionParser(tmp, "/c");		// updateX_20050330
	m_sSymbol = OptionParser(tmp, "/r");

	m_type = 1;
	if (!m_sImagePath[OBJ_R].IsEmpty())
	{
		m_type++;
		if (!m_sText.IsEmpty())
			m_type++;
	}

	m_sUser = Variant(nameCC, "");
	m_sRoot = Variant(homeCC, "");
	m_backColor = GetIndexColor(m_Param.pRGB);

	const	int	offs = m_sImagePath[OBJ_S].ReverseFind('\\');
	if (offs == -1)
		m_sImagePath[OBJ_S] = m_sRoot + "\\image\\" + m_sImagePath[OBJ_R];
	else
		m_sImagePath[OBJ_S] = m_sRoot + "\\image\\" + m_sImagePath[OBJ_R].Mid(offs + 1);

	m_sImagePath[OBJ_R] = m_sImagePath[OBJ_S].Mid(0, m_sImagePath[OBJ_S].GetLength()-4) + "_정지.BMP";

	//m_sImagePath[OBJ_R] = m_sImagePath[OBJ_S] + "CODESTOP_EN";

	m_sImagePath[OBJ_U] = m_sRoot + "\\image\\" + "검색UP.BMP";
	m_sImagePath[OBJ_D] = m_sRoot + "\\image\\" + "검색DN.BMP";
}

COLORREF CControlWnd::getAxColor(int iColor)
{
	if (iColor & 0x02000000)
		return iColor;

	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)iColor);

}
void CControlWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	xxx::CMemDC	mdc(&dc);
	Draw(&mdc);
}

void CControlWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_capture)
		ReleaseCapture();
	
	SetCapture();
	m_capture = true;
	Invalidate();
	CWnd::OnLButtonDown(nFlags, point);
}

void CControlWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_capture)
		ReleaseCapture();
	SetCapture();
	m_capture = true;
	Invalidate();
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CControlWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonUp(nFlags, point);
	if (m_capture)
	{
		CRect rc;
		m_capture = false;
		ReleaseCapture();
		rc = m_rcBtn[OBJ_R];
		if (rc.PtInRect(point))
		{
			if (m_bRotate)	// 돌려보기면 멈춤
			{
				StopRotate();
			}
			else		// 멈춤이면 팝업메뉴
			{
				CWnd::OnLButtonUp(nFlags, point);
				Invalidate();

				PostMessage(WM_POPUPMENU, point.x, point.y);
				return;
			}
		}
		else
		{
			if (m_rcBtn[OBJ_U].PtInRect(point))	// 이전
			{
				//ProcRotate(false);
				m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnClick)),
					(LPARAM)m_Param.name.GetString());
			}
			else if (m_rcBtn[OBJ_D].PtInRect(point))// 다음
			{
				//ProcRotate(true);
				m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk)),
					(LPARAM)m_Param.name.GetString());
			}
		}

		Invalidate();
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CControlWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	else
	{
		m_bHover[OBJ_R] = m_bHover[OBJ_S] = m_bHover[OBJ_U] = m_bHover[OBJ_D] = FALSE;
		int	ii = (m_bRotate)? OBJ_R:OBJ_S;
		if (m_rcBtn[ii].PtInRect(point))
		{
			m_bHover[ii]=TRUE;
			Invalidate();
		}
		else
		{
			for (ii = OBJ_U; ii <= OBJ_D; ii++)
			{
				if (m_rcBtn[ii].PtInRect(point))
				{
					m_bHover[ii]=TRUE;
					Invalidate();
					break;
				}
			}
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

LRESULT CControlWnd::OnMouseHover(WPARAM wParam, LPARAM lParam) 
{
	const	int	xPos = LOWORD(lParam); 
	const	int	yPos = HIWORD(lParam); 
	const	CPoint	point = CPoint(xPos, yPos);

	m_bHover[OBJ_R] = m_bHover[OBJ_S] = m_bHover[OBJ_U] = m_bHover[OBJ_D] = FALSE;
	int	ii = (m_bRotate)? OBJ_R:OBJ_S;
	if (m_rcBtn[ii].PtInRect(point))
	{
		m_bHover[ii]=TRUE;
		Invalidate();
	}
	else
	{
		for (int ii = OBJ_U; ii <= OBJ_D; ii++)
		{
			if (m_rcBtn[ii].PtInRect(point))
			{
				m_bHover[ii]=TRUE;
				break;
			}
		}
	}

	Invalidate();
	return 0;
}


LRESULT CControlWnd::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bTracking = FALSE;
	m_bHover[OBJ_R] = m_bHover[OBJ_S] = m_bHover[OBJ_U] = m_bHover[OBJ_D] = FALSE;
	Invalidate();
	return 0;
}

BOOL CControlWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
	return CWnd::OnEraseBkgnd(pDC);
}


CSize CControlWnd::BitmapSize(HBITMAP hBitmap)
{
	if (!hBitmap)
		return CSize(0, 0);
	BITMAP BM;
	GetObject(hBitmap, sizeof(BITMAP), &BM);
	return CSize((int)BM.bmWidth, (int)BM.bmHeight);
}

void CControlWnd::OnEnable(BOOL bEnable) 
{
	CWnd::OnEnable(bEnable);
	
	Invalidate();	
}

void CControlWnd::OnDestroy() 
{
	for (int ii = 0; ii < m_hjCodes.GetSize(); ii++)
		m_hjCodes.GetAt(ii).reset();
	
	for (int ii = 0; ii < m_codeArr.GetSize(); ii++)
	{
		m_codeArr.GetAt(ii)->RemoveAll();
		m_codeArr.GetAt(ii).reset();
	}
	m_codeArr.RemoveAll();	

	if (m_pPopup)
		m_pPopup.reset();

	if (m_pAllCode)
		m_pAllCode.reset();

	if (m_pSelectedCode)
		m_pSelectedCode.reset();

	CString file, sKey;

	file.Format("%s\\user\\%s\\%s.ini", m_sRoot, m_sUser, m_sUser);
	sKey.Format("%d",(int)this);
	WritePrivateProfileString("CATEGORY", sKey, "0", file);

	CWnd::OnDestroy();	
}

COLORREF CControlWnd::GetIndexColor(int index)
{
	return m_pParent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

LOGFONT CControlWnd::setFont(CString fName, int fsize, int style)
{
	LOGFONT	lf{};

	lf.lfHeight = fsize * 10; 
	strcpy(lf.lfFaceName, fName);  
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = FALSE;

	switch (style)
	{
	case 0: // none
	case 1: // none
		break;
	case 2: // italic
		lf.lfItalic = TRUE;
		break;
	case 3: // bold
		lf.lfWeight = FW_BOLD;
		break;
	case 4: // both
		lf.lfItalic = TRUE;
		lf.lfWeight = FW_BOLD;
		break;
	}

	return lf;
}

CString CControlWnd::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}

	CString  temp = srcstr.Left(srcstr.Find(substr));
	srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
	return temp;
}

CString CControlWnd::Variant(int comm, CString data)
{
	CString retvalue;
	const	char* dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)data.GetString());

	if ((long)dta > 1)
		retvalue = dta;

	return retvalue;
}

void CControlWnd::SetButtonImages()
{
	CString sImageDir = "";
	sImageDir.Format("%s\\image\\", m_sRoot);

	if (m_type == 1)
	{
		for (int ii = OBJ_R; ii <= OBJ_D; ii++)
		{
			m_hBitmap[ii] = m_hBitmap_dn[ii] = m_hBitmap_hv[ii] = NULL;
		}
	}
	else if (m_type == 2 || m_type == 3)
	{
		for (int ii = OBJ_R; ii <= OBJ_D; ii++)
		{
			CFileFind	finder;
			m_sImagePath[ii].MakeLower();
			m_sImagePath[ii].Replace(".bmp", "");

			if (finder.FindFile(m_sImagePath[ii] + ".bmp"))
				m_hBitmap[ii] = getBitmap(m_sImagePath[ii] + ".bmp")->operator HBITMAP();
		
			if (finder.FindFile(m_sImagePath[ii] + "_dn.bmp"))
				m_hBitmap_dn[ii] = getBitmap(m_sImagePath[ii] + "_dn.bmp")->operator HBITMAP();

			if (finder.FindFile(m_sImagePath[ii] + "_en.bmp"))
				m_hBitmap_hv[ii] = getBitmap(m_sImagePath[ii] + "_en.bmp")->operator HBITMAP();

		}
	}
}

int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	const	LOGFONT lf = setFont(m_Param.fonts, m_Param.point, m_Param.style);
	m_font.CreatePointFontIndirect(&lf);
	SetButtonImages();	

	PostMessage(WM_INITLOAD);

	CString temp = Variant(historyCC, "1301");

	while (!temp.IsEmpty()){
		
		CString temp1 = Parser(temp,"\t").Left(6);
		temp1.TrimRight();
		m_history.Add(temp1);
	}
	
	return 0;
}

void CControlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	CRect	rc;
	GetClientRect(&rc);
	m_rcAll = rc;

	m_rcBtn[OBJ_R] = rc;
	m_rcBtn[OBJ_R].right = rc.Height();
	m_rcBtn[OBJ_S] = m_rcBtn[OBJ_R];

	m_rcBtn[OBJ_U] = rc;
	m_rcBtn[OBJ_U].left = m_rcBtn[OBJ_R].right-1;
	m_rcBtn[OBJ_U].bottom = rc.CenterPoint().y;

	m_rcBtn[OBJ_D] = rc;
	m_rcBtn[OBJ_D].left = m_rcBtn[OBJ_R].right-1;
	m_rcBtn[OBJ_D].top = rc.CenterPoint().y;
}

int CompareHJ(const void* arg1, const void* arg2)
{
	const	struct hjcodex* pLVal = (struct hjcodex*)arg1;
	const	struct hjcodex* pRVal = (struct hjcodex*)arg2;

	return strcmp(pLVal->code, pRVal->code);
}

bool CControlWnd::LoadHjCode()
{
	if (m_hjCodes.GetSize() > 0)
		return false;

	CFile	file;
	CString path; 

	path.Format("%s\\%s\\%s", m_sRoot, TABDIR, HJCODEx);
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone))
		return false;

	int	size = (int)file.GetLength();
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(size + 1);
	struct	hjcodex* hj = (struct hjcodex*)pWb.get();
	std::shared_ptr<_mhjcode> mhj = nullptr;

	if (file.Read(pWb.get(), size) != size)
	{
		file.Close();
		pWb.reset();
		return false;
	}
	file.Close();

	size /= sizeof(struct hjcodex);
	qsort((void*)hj, size, sizeof(struct hjcodex), CompareHJ);

	for (int ii = 0; ii < size; ii++, hj++)
	{
		mhj = std::make_shared<_mhjcode>();

		mhj->code = CString(hj->code, HCodeLen).Trim();		mhj->code.Remove(0);
		mhj->hnam = CString(hj->hnam, HNameLen).Trim();		mhj->code.Remove(0);
		mhj->enam = CString(hj->enam, ENameLen).Trim();		mhj->code.Remove(0);
		mhj->symb = CString(hj->symb, HSymbolLen).Trim();	mhj->code.Remove(0);
		mhj->acdl = CString(hj->acdl, 6).Trim();
		mhj->acdm = CString(hj->acdm, 6).Trim();
		mhj->acds = CString(hj->acds, 6).Trim();

		mhj->ecng = hj->ecng;
		mhj->jsiz = hj->jsiz;
		mhj->size = hj->size;
		mhj->jjug = hj->jjug;
		mhj->kpgb = hj->kpgb;
		mhj->kosd = hj->kosd;
		mhj->ssgb = hj->ssgb;
		mhj->ucmd = hj->ucmd;
		mhj->itgb = hj->itgb;
		mhj->wsgb = hj->wsgb;
		mhj->jqty = hj->jqty;
		mhj->star = hj->star;
		mhj->unio = hj->unio;
		mhj->jchk = hj->jchk;

		m_hjCodes.Add(mhj);
		m_codeMap.SetAt(mhj->code, mhj->hnam);
		if ((mhj->jsiz >= 38 + 41 && mhj->jsiz <= 41 + 41))
		{
			TRACE("배당지수 %d 있음", mhj->jsiz);
		}
	}
	pWb.reset();

/*	속도 개선 전 logic
	전) array 생성 후 sort loop에서 다수 시간 소요
	후) structure qsort후 array 생성

	const	int	codeN = (int)file.GetLength() / sizeof(struct hjcodex);
	struct	hjcodex	hj {};
	std::shared_ptr<_mhjcode> mhj = nullptr;

	for (int ii = 0; ii < codeN; ii++)
	{
		if (file.Read(&hj, sizeof(hjcodex)) != sizeof(hjcodex))
			break;
		
		mhj = std::make_shared<_mhjcode>();

		mhj->code = CString(hj.code,HCodeLen);	mhj->code.Trim();	mhj->code.Remove(0);
		mhj->hnam = CString(hj.hnam,HNameLen);	mhj->hnam.Trim();	mhj->code.Remove(0);
		mhj->enam = CString(hj.enam,ENameLen);	mhj->enam.Trim();	mhj->code.Remove(0);
		mhj->symb = CString(hj.symb,HSymbolLen);mhj->symb.Trim();	mhj->code.Remove(0);
		mhj->ecng = hj.ecng;		
		mhj->jsiz = hj.jsiz;		
		mhj->size = hj.size;
		mhj->ucdm = hj.ucdm;
		mhj->ucds = hj.ucds;
		mhj->jjug = hj.jjug;
		mhj->kpgb = hj.kpgb;
		mhj->kosd = hj.kosd;
		mhj->ssgb = hj.ssgb;
		mhj->ucmd = hj.ucmd;
		mhj->itgb = hj.itgb;
		mhj->wsgb = hj.wsgb;
		mhj->jqty = hj.jqty;
		mhj->star = hj.star;
		mhj->unio = hj.unio;
		mhj->jchk = hj.jchk;

		m_hjCodes.Add(mhj);
		m_codeMap.SetAt(mhj->code, mhj->hnam);
		if ((mhj->jsiz >= 38 + 41 && mhj->jsiz <= 41 + 41))
		{
			TRACE("배당지수 %d 있음",mhj->jsiz);
		}
	 }
	file.Close();

	std::shared_ptr<_mhjcode> hja = nullptr, hjb = nullptr;
	for (int ii = 0 ; ii < m_hjCodes.GetSize() - 1 ; ii++)
	{
		for (int jj = ii+1 ; jj < m_hjCodes.GetSize() ; jj++)
		{
			hja = m_hjCodes.GetAt(ii);
			hjb = m_hjCodes.GetAt(jj);
			if (hjb->code.CompareNoCase(hja->code) < 0)
			{
				m_hjCodes.SetAt(ii, hjb);
				m_hjCodes.SetAt(jj, hja);
			}
		}
	}
*/

	return true;
}

long CControlWnd::OnCategory(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 100)
	{
		CString sSelectedName = (char*)lParam;
		m_sSelectedName = sSelectedName;
		return 0;
	}

	switch (m_nKind)
	{
		//맵 
	case 0:
		switch (wParam)
		{
		case 1: // 분류별 Load (계열사)
		case 2:	// 분류별 Load (테마리스트)
			{
				m_rtnStr = m_sSymbol + "\tE\n";
				const	CStringArray *pStrArr = (CStringArray*)lParam;

				for (int ii = 0; ii < pStrArr->GetSize(); ii++)
				{
					m_rtnStr += pStrArr->GetAt(ii);
					m_rtnStr += '\n';
				}
				m_pParent->PostMessage(WM_USER, MAKEWPARAM(formDLL, m_Param.key), (LPARAM)m_rtnStr.GetString());
			}
			break;
		case 6:	// 업종 load
			{
				if (lParam / 100 == upKOSPI || lParam / 100 == upKOSPI200 || lParam / 100 == upKOSPIx)
					m_rtnStr.Format("%s\tK%03d\n", m_sSymbol, lParam);
				else
					m_rtnStr.Format("%s\tQ%03d\n", m_sSymbol, lParam);
				m_pParent->PostMessage(WM_USER, MAKEWPARAM(formDLL, m_Param.key), (LPARAM)m_rtnStr.GetString());
			}
			break;
		case 7:	// 관심등록
			RequestCode((int)lParam, procFORM);
			/*
			m_rtnStr = m_sSymbol + "\tE\n";
			m_rtnStr += LoadCodeList(lParam);
			m_pParent->PostMessage(WM_USER, MAKEWPARAM(formDLL, m_Param.key), (LPARAM)m_rtnStr.GetString());
			*/
			break;
		case 8:	// History
			m_rtnStr = m_sSymbol + "\tH\n";
			m_pParent->PostMessage(WM_USER, MAKEWPARAM(formDLL, m_Param.key), (LPARAM)m_rtnStr.GetString());
			break;
		case 9:	// 전종목
			m_rtnStr = m_sSymbol + "\tA\n";
			m_pParent->PostMessage(WM_USER, MAKEWPARAM(formDLL, m_Param.key), (LPARAM)m_rtnStr.GetString());
			break;
		case 10: // 코스피 전종목
			break;
		case 11: // 코스닥 전종목
			break;
		case 12: // 코스피200
			break;
		case 13: // 코스닥50
			break;
		case 14: // KRX100
			break;
		case 15: // 프리보드(제3시장)
			break;
		case 16: // ETF
			break;
		case 17: // ELW
			break;
		default:
			break;
		}
		break;
	case 1:	//dll 화면
		switch (wParam)
		{
		case 1: // 분류별 Load (계열사)
		case 2:	// 분류별 Load (테마리스트)
			m_sCodeList.Empty();
			{
				const	CStringArray *pStrArr = (CStringArray*)lParam;

				for (int ii = 0; ii < pStrArr->GetSize(); ii++)
				{
					m_sCodeList += pStrArr->GetAt(ii);
					m_sCodeList += '\t';
				}
			}
//			m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk)),
//					(LPARAM)m_Param.name.GetString());
			break;
		case 4: // 등락종목
			{
				m_rtnStr.Format("%03d", lParam);
				SendTR("PIBOJRNK", (char *)m_rtnStr.GetString(), m_rtnStr.GetLength(), TR_JRNK);
			}
			break;
		case 6:	// 업종 load
			{
				CString sPath;
				sPath.Format("%s\\%s\\%s", m_sRoot, TABDIR, HJCODEx);
				m_sCodeList = loadJCode(sPath, lParam);
//				m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk)),
//					(LPARAM)m_Param.name.GetString());
			}
			break;
		case 7:	// 관심등록
			RequestCode((int)lParam, procEVENT);
			/*
			m_sCodeList = LoadCodeList(lParam);
			m_sCodeList.Replace("\n", "\t");
			*/
//			m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk)),
//					(LPARAM)m_Param.name.GetString());
			break;
		default:
			m_rtnStr.Empty();
			break;
		}
		break;
	}
	return 0;
}

long CControlWnd::OnPopupMenu(WPARAM wParam, LPARAM lParam)
{
	CMenu	cMenu;
	CPoint	point = CPoint(wParam, lParam);

	ClientToScreen(&point);
	cMenu.CreatePopupMenu();
	cMenu.AppendMenu(MFT_STRING, 1, "돌려보기 설정");
	cMenu.AppendMenu(MFT_STRING, 2, "시작");

	const	int	ret = cMenu.TrackPopupMenu(TPM_RIGHTBUTTON|TPM_RETURNCMD, point.x, point.y, this, NULL);
	if (ret == 1)
	{
		CRect	rc = m_rcBtn[OBJ_R];
		ClientToScreen(&rc);
		ShowPopup(rc);
	}
	else if (ret == 2)
	{
		StartRotate();
	}

	return 0;
}

int CompareAll(const void *arg1, const void *arg2)
{
	const	struct _AllCode* pLVal= (struct _AllCode*)arg1;
	const	struct _AllCode* pRVal= (struct _AllCode*)arg2;
   
	return strcmp(pLVal->code, pRVal->code);
}

long CControlWnd::OnInitLoad(WPARAM wParam, LPARAM lParam)
{
	CFile	file;
	CString path, code; 
	
	path.Format("%s\\%s\\%s", m_sRoot, TABDIR, HJCODEx);
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone))
		return 0;
	
	CStringArray	arCode;
	int	size = (int)file.GetLength();
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(size + 1);
	struct	hjcodex* hj = (struct hjcodex*)pWb.get();

	if (file.Read(pWb.get(), size) != size)
	{
		file.Close();
		pWb.reset();
		return 0;
	}
	file.Close();

	size /= sizeof(struct hjcodex);

	arCode.RemoveAll();
	for (int ii = 0; ii < size; ii++, hj++)
	{
		switch ((char)hj->kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
			switch (hj->ssgb)
			{
			case jmDEF:
				break;
			default:
				continue;
			}
			break;
		default:
			continue;
		}
	
		code = CString(hj->code,HCodeLen);
		code.Trim();
		code = code.Mid(1);
		arCode.Add(code);
	}
	pWb.reset();

	if (arCode.GetSize() <= 0)
		return 0;

	size = arCode.GetSize();
	m_pAllCode = std::make_unique<_AllCode[]>(size);
	for (int ii = 0; ii < size; ii++)
	{
		CopyMemory(m_pAllCode[ii].code, arCode.GetAt(ii).GetString(), L_ACODE);
	}

	qsort((void*)m_pAllCode.get(), size, sz_AllCode, CompareAll);
	m_nAllCode = size;

	return 1;
}

void CControlWnd::StartRotate()
{
	if (m_TimerGap >= 2 && m_rotateArr.GetSize() > 1)
	{
		m_bRotate = true;
		m_TimerID = TM_AUTO;
		SetTimer(m_TimerID, m_TimerGap*1000, NULL);
		Invalidate();
	}
}

void CControlWnd::StopRotate()
{
	if (m_TimerGap >= 2 && m_rotateArr.GetSize() > 1)
	{
		m_bRotate = false;
		KillTimer(m_TimerID);
	}
}

void CControlWnd::InitRotate(int Gap)
{
	m_rotateArr.RemoveAll();
	m_rotatePos = 0;
	m_TimerID = 0;
	m_TimerGap = Gap;
	m_rtnStr = "";
}

bool CControlWnd::ProcRotate(bool bNext)
{
	const	int	size = m_rotateArr.GetSize();
	if (size < 2)
		return false;
	
	if (bNext)
	{
		m_rotatePos = ++m_rotatePos % size;
	}
	else
	{
		m_rotatePos = (--m_rotatePos < 0)? size-1:m_rotatePos;
	}
	m_rtnStr = m_rotateArr.GetAt(m_rotatePos);

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange)),
				(LPARAM)m_Param.name.GetString());
	return true;
}

CString CControlWnd::GetCodeFromAll(LPCTSTR pCode, bool bNext)
{
	CString	str;

	if (m_nAllCode < 1)
		return str;

	int	left = 0;
	int	right = m_nAllCode-1;
	int	mid = 0, res = 0;
	int	sIndex = -1;

	while (left <= right)
	{
		mid = (left + right)/2;
		res = strcmp(m_pAllCode[mid].code, pCode);
		if (res == 0)
		{
			sIndex = mid;
			break;
		}
		else if (res > 0)
		{
			right = mid - 1;
		}
		else
		{
			left = mid + 1;
		}
	}

	if (sIndex >= 0)
	{
		if (bNext)
		{
			sIndex++;
			sIndex = sIndex % m_nAllCode;
		}
		else
		{
			sIndex--;
			if (sIndex < 0)
				sIndex = m_nAllCode-1;
		}

		str = m_pAllCode[sIndex].code;
	}

	return str;
}

CString CControlWnd::GetCodeFromSelectedItem(LPCTSTR pCode, bool bNext)
{
	CString	str = "";
	if (m_nSelectedCode < 1)
		return str;

	int	left = 0;
	int	right = m_nSelectedCode-1;
	int	mid = 0, res = 0;
	int	sIndex = -1;

	while (left <= right)
	{
		mid = (left + right)/2;
		res = strcmp(m_pSelectedCode[mid].code, pCode);
		if (res == 0)
		{
			sIndex = mid;
			break;
		}
		else if (res > 0)
		{
			right = mid - 1;
		}
		else
		{
			left = mid + 1;
		}
	}

	if (sIndex >= 0)
	{
		if (bNext)
		{
			sIndex++;
			sIndex = sIndex%m_nSelectedCode;
		}
		else
		{
			sIndex--;
			if (sIndex < 0)
				sIndex = m_nSelectedCode-1;
		}

		str = m_pSelectedCode[sIndex].code;
	}

	if (str == "")
	{
		str = m_pSelectedCode[0].code;
	}

	return str;
}

void CControlWnd::ShowPopup(CRect rc)
{
	if (m_pPopup)
		m_pPopup.reset();

	CString sClassName  = AfxRegisterWndClass(0);
	m_pPopup = std::make_unique<CCategoryPopup>(this, m_TimerGap);
	m_pPopup->setPathInfo(m_sRoot, m_sUser);
	m_pPopup->setParent(this);
	m_pPopup->m_nKind = m_nKind;

	//if (m_nKind == 0)
	//	rc.SetRect(rc.left  , rc.bottom  , rc.left + 370 , rc.bottom + 350);
	//else
	if (m_bShowPlay)
		rc.SetRect(rc.left  , rc.bottom  , rc.left + PT_CX, rc.bottom + PT_CY);
	else
		rc.SetRect(rc.left  , rc.bottom  , rc.left + PT_CX, rc.bottom + PT_CX-30);
	m_pPopup->CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, sClassName, NULL,
		WS_POPUP/*|WS_BORDER|WS_EX_CLIENTEDGE|WS_THICKFRAME*/, rc, NULL, NULL, NULL);

	m_pPopup->SetFont(&m_font);
	m_pPopup->ShowWindow(SW_SHOW);
	m_pPopup->SetFocus();
}


CString CControlWnd::LoadCodeList(int nGroup)
{
// updateX_20051224
	struct	_inters	inter{};

	CString dir, sRtn;

	dir.Format("%s\\user\\%s\\portfolio.i%02d", m_sRoot, m_sUser, nGroup);
	CFile	file;
	
	if (file.Open(dir, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
	{
		UINT	nBytesRead = 0;
		do
		{
			nBytesRead = file.Read(&inter, sizeof(_inters));
			if (nBytesRead == sizeof(inter))
			{
				CString sCode = getMidString(inter.code, 0, 12);
				sCode.TrimLeft();
				if (!sCode.IsEmpty())
				{
					sRtn += sCode;
					sRtn += '\t';
				}
			}
		} while ((int)nBytesRead);
		file.Close();
	}

	return sRtn;
}

CString CControlWnd::OptionParser(CString sOption, CString sKey)
{
	CString tmp = sOption;

	Parser(tmp, sKey);
	tmp = Parser(tmp, "/");
	return tmp;
}

CBitmap* CControlWnd::getBitmap(CString path)
{
	return (CBitmap*)m_pParent->SendMessage(WM_USER, getBITMAP, (LPARAM)path.GetString());
}

CFont* CControlWnd::getAxFont(CString fName, int point, int style)
{
	struct	_fontR	fontR {};

	fontR.name = (LPSTR)fName.GetString();
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

void CControlWnd::SendTR(CString trc, char* datb, int datl, int key)
{
	struct _userTH	udat {};
	int	index = 0;
	std::unique_ptr<TCHAR[]> buff = std::make_unique<TCHAR[]>(L_userTH + datl + 128);

	// key & control name
	buff[index++] = key;
	CopyMemory(&buff[index], m_Param.name, m_Param.name.GetLength());
	index += m_Param.name.GetLength();
	buff[index++] = '\t';

	// userTH
	CopyMemory(udat.trc, trc, trc.GetLength());
	udat.stat = US_KEY;
	udat.key  = m_Param.key;
	CopyMemory(&buff[index], &udat, L_userTH);
	index += L_userTH;

	// data
	CopyMemory(&buff[index], datb, datl);

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datl), (LPARAM)buff.get());
	buff.reset();
}

CString CControlWnd::loadJCode(CString path, int nKind)
{
	CFile	fileH;

	if (!fileH.Open(path, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
	{
		AfxMessageBox("종목 화일을 읽을 수 없습니다.");
		return "";
	}
 
	CString	sCodes;
	int	nCnt = 0, xval = 0, yval = 0;
	const	int	maxCnt = max(atoi(m_jCount), 120);		// updateX_20050330
	bool	bReg = false;
	int	size = (int)fileH.GetLength();
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(size + 1);
	struct	hjcodex* hj = (struct hjcodex*)pWb.get();

	if (fileH.Read(pWb.get(), size) != size)
	{
		fileH.Close();
		pWb.reset();
		return "";
	}
	fileH.Close();

	xval = nKind / 100;
	yval = nKind % 100;
	size /= sizeof(struct hjcodex);
	for (int ii = 0; ii < size; ii++, hj++)
	{
		if (hj->ssgb == jmSINJU || hj->ssgb == jmSINJS || hj->ssgb == jmHYFND || hj->ssgb == jmELW)
			continue;

		bReg = false;
		if (xval == upKOSPI || xval == upKOSPI200 || xval == upKOSPIx)
		{
			if (hj->kosd != jmKOSPI)
				continue;

			if (xval == upKOSPI)
			{
				if (yval == 1)
				{
					switch (m_nUcmd)			// updateX_20050330
					{
					case 0:		// 전체
						bReg = true; break;
					case 1:		// 보통주
						if (hj->ucmd == stockCom)
							bReg = true;
						break;
					case 2:		// 우선주
						if (hj->ucmd == stockPre)
							bReg = true;
						break;
					}
				}
				else if (yval < 5)
				{
					if (hj->size == yval)
						bReg = true;
				}
				else if (yval < 27)
				{
					CString	ucdm = acd_KOSPI[yval];
					if (!ucdm.CompareNoCase(hj->acdm) || !ucdm.CompareNoCase(hj->acds))
						bReg = true;
				}
				else if (yval == 27)
				{
					if (hj->jjug == yval)
						bReg = true;
				}
				else if (yval < 41)
				{
					if (hj->jsiz == yval)
						bReg = true;
				}
				else if (yval == 41)
				{
					if (hj->jsiz >= yval + 38 && hj->jsiz <= yval + 41)
						bReg = true;
				}
				else if (yval == 42)
				{
					if (hj->wsgb == 1)
						bReg = true;
				}
			}
			else if (xval == upKOSPI200)
			{
				if (hj->kpgb == 1 || hj->kpgb == 2 || hj->kpgb == 3)
					bReg = true;
			}
			else if (xval == upKOSPIx)
			{
				if (yval == 1)
				{
					if (hj->kpgb == 2 || hj->kpgb == 3)
						bReg = true;
				}
				else if (yval == 2)
				{
					if (hj->kpgb == 3)
						bReg = true;
				}
				else if (yval == 3)
				{
					if (hj->itgb == 1)
						bReg = true;
				}
			}
		}
		else if (xval == upKOSDAQ || xval == upKOSDAQ50)
		{
			if (hj->kosd != jmKOSDAQ)
				continue;

			if (xval == upKOSDAQ)
			{
				if (yval == 1)
				{
					switch (m_nUcmd)			// updateX_20050330
					{
					case 0:		// 전체
						bReg = true; break;
					case 1:		// 보통주
						if (hj->ucmd == stockCom)
							bReg = true;
						break;
					case 2:		// 우선주
						if (hj->ucmd == stockPre)
							bReg = true;
						break;
					}
				}
				else if (yval == 2)
				{
					if (hj->jjug == yval)
						bReg = true;
				}
				else if (yval < 5)
				{
					if (hj->size == yval)
						bReg = true;
				}
				else if (yval == 5)
				{
					if (hj->size == 4 && hj->jjug == 2)
						bReg = true;
				}
				else if (yval < 38 || yval == 41)
				{
					CString	ucdm = acd_KOSDAQ[yval];
					if (!ucdm.CompareNoCase(hj->acdm) || !ucdm.CompareNoCase(hj->acds))
						bReg = true;
				}
				else if (yval < 41)
				{
					if (hj->jsiz == yval - 37)
						bReg = true;
				}
				else if (yval > 42 && yval <= 45)
				{
					CString ucdm = acd_KOSDAQ[yval];
					if (!ucdm.CompareNoCase(hj->acdl))
						bReg = true;
				}
			}
		}
		
		if (bReg && (hj->ssgb != jmSINJU && hj->ssgb != jmSINJS && hj->ssgb != jmHYFND && hj->ssgb != jmELW))
		{
			CString sCode = CString(hj->code, HCodeLen);
			sCode = sCode.Mid(1);
			sCode.TrimRight();

			sCodes += CString(sCode, strlen(sCode));
			sCodes += '\t';
			nCnt++;

			if (nCnt >= maxCnt)			// updateX_20050330
				break;
		}
	}
	pWb.reset();
	return sCodes;
}

CString CControlWnd::getMidString(char *pStr, int nFirst, int nLen)
{
	if ((int)strlen(pStr + nFirst) < nLen)
		nLen = strlen(pStr + nFirst);
	return CString(pStr + nFirst, nLen);
}

void CControlWnd::SendOK(CArray <std::shared_ptr<_mhjcode>, std::shared_ptr<_mhjcode>>& arr, int Gap, int idx)
{
	InitRotate(Gap);

	if (m_pSelectedCode)
		m_pSelectedCode.reset();
	m_nSelectedCode = 0;

	CString file, sKey;
	const	int	nCode = arr.GetSize();

	m_pSelectedCode = std::make_unique<_AllCode[]>(nCode);
	file.Format("%s\\user\\%s\\%s.ini", m_sRoot, m_sUser, m_sUser);
	sKey.Format("%d",(int)this);

	if (nCode < 1)
	{
		WritePrivateProfileString("CATEGORY", sKey, "0", file);
	}
	else
	{
		WritePrivateProfileString("CATEGORY", sKey, "1", file);
	}

	if (idx == 0)	//종목
	{
		for (int ii = 0 ; ii < nCode ; ii++)
		{
			sKey = arr.GetAt(ii)->code.Mid(1);
			m_rotateArr.Add(sKey);
			CopyMemory(m_pSelectedCode[ii].code, sKey.GetString(), min(L_ACODE, sKey.GetLength()));
		}
		qsort((void*)m_pSelectedCode.get(), nCode, sizeof(_AllCode), CompareAll);
		m_nSelectedCode = nCode;
	}
	else if (idx == 1) //History
	{
		CString	tmp;

		tmp = Variant(historyCC, "1301");
		while (!tmp.IsEmpty())
		{		
			CString tmp1 = Parser(tmp,"\t").Left(6);
			tmp1.TrimRight();
			m_rotateArr.Add(tmp1);
		}
	}

	
	if (m_nChkPlay == 1)
	{
		StartRotate();
	}
}

void CControlWnd::Draw(CDC *pDC)
{
	pDC->FillSolidRect(&m_rcAll, m_backColor);//GetSysColor(COLOR_BTNFACE));

	CBitmap	*bmp = nullptr;
	int	ii = (m_bRotate)? OBJ_R:OBJ_S;

	if (m_capture && m_bHover[ii] && m_hBitmap_dn[ii])
		bmp = CBitmap::FromHandle(m_hBitmap_dn[ii]);
	else if (m_bHover[ii] && m_hBitmap_hv[ii])
		bmp = CBitmap::FromHandle(m_hBitmap_hv[ii]);
	else if (m_hBitmap)
		bmp = CBitmap::FromHandle(m_hBitmap[ii]);
	else
		bmp = NULL;

	if (bmp)
		DrawStretchByMask(pDC, bmp, m_rcBtn[ii], RGB(255, 0, 255));
	else
		DrawButton(pDC, ii);

	if (!m_bMini)
	{
		for (ii = OBJ_U; ii <= OBJ_D; ii++)
		{
			if (m_capture && m_bHover[ii] && m_hBitmap_dn[ii])
				bmp = CBitmap::FromHandle(m_hBitmap_dn[ii]);
			else if (m_bHover[ii] && m_hBitmap_hv[ii])
				bmp = CBitmap::FromHandle(m_hBitmap_hv[ii]);
			else if (m_hBitmap)
				bmp = CBitmap::FromHandle(m_hBitmap[ii]);
			else	
				bmp = NULL;

			if (bmp)
				DrawStretchByMask(pDC, bmp, m_rcBtn[ii], RGB(255, 0, 255));
			else
				DrawButton(pDC, ii);
		}
	}

}

void CControlWnd::DrawButton(CDC *pDC, int index)
{
	const	COLORREF	color = m_backColor;//GetSysColor(COLOR_BTNFACE);

	pDC->SetBkColor(color);
	switch (index)
	{
	case OBJ_R:
		pDC->DrawFrameControl(m_rcBtn[index], DFC_BUTTON, DFCS_BUTTONPUSH | (m_capture && m_bHover[index] ? DFCS_PUSHED:NULL));
		pDC->DrawText(_T("▶"), &m_rcBtn[index], DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		break;
	case OBJ_S:
		pDC->DrawFrameControl(m_rcBtn[index], DFC_BUTTON, DFCS_BUTTONPUSH | (m_capture && m_bHover[index] ? DFCS_PUSHED:NULL));
		pDC->DrawText(_T("■"), &m_rcBtn[index], DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		break;
	case OBJ_U:
		pDC->DrawFrameControl(m_rcBtn[index], DFC_SCROLL, DFCS_SCROLLUP | (m_capture && m_bHover[index] ? DFCS_PUSHED:NULL));
		break;
	case OBJ_D:
		pDC->DrawFrameControl(m_rcBtn[index], DFC_SCROLL, DFCS_SCROLLDOWN | (m_capture && m_bHover[index] ? DFCS_PUSHED:NULL));
		break;
	}
}

#define SRCMASK   0x00220326    // mask
void CControlWnd::DrawStretchByMask(CDC *pDC, CBitmap *pBitmap, CRect bRc, COLORREF maskcolor)
{
	if (!pBitmap || !pBitmap->GetSafeHandle())
		return;
/*
	CDC memDC;
	CBitmap* pOldBitmap;
	BITMAP  bm;
	pBitmap->GetBitmap(&bm);
		
	memDC.CreateCompatibleDC(pDC);	
	
	if (memDC.m_hDC != NULL) 
		pOldBitmap = (CBitmap*)memDC.SelectObject(pBitmap);
	  
	pDC->StretchBlt(0, 0, bRc.Width(), bRc.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
*/

	BITMAP	bm{};

	pBitmap->GetBitmap(&bm);
	const	int	bx = bm.bmWidth;
	const	int	by = bm.bmHeight;
	CDC	 buffDC, maskDC, memoryDC, copyDC;
	CBitmap  buffBitmap, maskBitmap, copyBitmap;
	
	buffDC.CreateCompatibleDC(pDC);
	buffBitmap.CreateCompatibleBitmap(pDC, bx, by);
	CBitmap* oldbuffBitmap = buffDC.SelectObject(&buffBitmap);
	
	maskDC.CreateCompatibleDC(pDC);
	maskBitmap.CreateBitmap(bx, by, 1, 1, NULL);
	CBitmap* oldmaskBitmap = maskDC.SelectObject(&maskBitmap);
	
	const	CRect  maskRc(0, 0, bx, by);
	buffDC.FillSolidRect(&maskRc, maskcolor);
	
	memoryDC.CreateCompatibleDC(pDC);
	CBitmap* oldmemoryBitmap = memoryDC.SelectObject(pBitmap);
	buffDC.BitBlt(0, 0, bx, by, &memoryDC, 0, 0, SRCCOPY);
	maskDC.BitBlt(0, 0, bx, by, &buffDC, 0, 0, SRCCOPY);
	
	copyDC.CreateCompatibleDC(pDC);
	copyBitmap.CreateCompatibleBitmap(pDC, bx, by);
	CBitmap* oldcopyBitmap = copyDC.SelectObject(&copyBitmap);
	
	copyDC.BitBlt(0, 0, bx, by, pDC, bRc.left, bRc.top, SRCCOPY);
	copyDC.BitBlt(0, 0, bx, by, &maskDC, 0, 0, SRCAND);
	buffDC.BitBlt(0, 0, bx, by, &maskDC, 0, 0, SRCMASK);
	copyDC.BitBlt(0, 0, bx, by, &buffDC, 0, 0, SRCPAINT);
	pDC->StretchBlt(bRc.left, bRc.top, bRc.Width(), bRc.Height(), &copyDC, 0, 0, bx, by, SRCCOPY);
	
	copyDC.SelectObject(oldcopyBitmap);
	memoryDC.SelectObject(oldmemoryBitmap);
	maskDC.SelectObject(oldmaskBitmap);
	buffDC.SelectObject(oldbuffBitmap);
	
	buffDC.DeleteDC();
	maskDC.DeleteDC();
	memoryDC.DeleteDC();
	copyDC.DeleteDC();
} 

HCURSOR CControlWnd::GetCursor(int kind)
{
	_ASSERT(m_pParent);
	return (HCURSOR)m_pParent->SendMessage(WM_USER, MAKEWPARAM(getCURSOR, 0), (LPARAM)kind);
}

BOOL CControlWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	HCURSOR	hCursor = GetCursor(10);
	if (hCursor)
	{
		SetCursor(hCursor);
		return TRUE;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CControlWnd::OnTimer(UINT nIDEvent) 
{
	if (!ProcRotate(true))
		StopRotate();
	
	CWnd::OnTimer(nIDEvent);
}

BSTR CControlWnd::GetCurCode() 
{
	
	CString strResult = m_rtnStr;

	return strResult.AllocSysString();
}

BSTR CControlWnd::GetPreCode(LPCTSTR code) 
{
	//CString strResult = GetCodeFromAll(code, false);
	CString strResult;

	if (m_nSelectedCode > 0)
	{
		strResult = GetCodeFromSelectedItem(code,false);
	}
	else
	{
		strResult = GetCodeFromAll(code,false);
	}

	return strResult.AllocSysString();
}

BSTR CControlWnd::GetNextCode(LPCTSTR code) 
{
	//CString strResult = GetCodeFromAll(code, true);
	CString strResult;

	if (m_nSelectedCode > 0)
	{
		strResult = GetCodeFromSelectedItem(code,true);
	}
	else
	{
		strResult = GetCodeFromAll(code,true);
	}

	return strResult.AllocSysString();
}


BSTR CControlWnd::GetJCodes() 
{
	CString strResult;
	strResult = m_sCodeList;

	return strResult.AllocSysString();
}

BSTR CControlWnd::_GetSelectedName() 
{
	return m_sSelectedName.AllocSysString();
}

void CControlWnd::_SetSelectedName(LPCTSTR lpszNewValue) 
{
	m_sSelectedName = lpszNewValue;
}

BSTR CControlWnd::GetData() 
{
	CString strResult;
	

	return strResult.AllocSysString();
}

void CControlWnd::SetData(LPCTSTR lpszNewValue) 
{
	// TODO: Add your property handler here
}

void CControlWnd::SetProperties(LPCTSTR str) 
{
	// TODO: Add your dispatch handler code here

}

BSTR CControlWnd::GetProperties() 
{
	CString strResult;
	// TODO: Add your dispatch handler code here

	return strResult.AllocSysString();
}

long CControlWnd::GetHandle() 
{
	// TODO: Add your dispatch handler code here
	return (long)this;
}

LRESULT CControlWnd::OnCategoryUpDown(WPARAM wParam, LPARAM lParam)
{

	if ((int)lParam == 0)	// 이전
	{
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnClick)),
			(LPARAM)m_Param.name.GetString());
	}
	else if ((int)lParam == 1)// 다음
	{
		//ProcRotate(true);
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk)),
			(LPARAM)m_Param.name.GetString());
	}
	return 0;
}

LRESULT CControlWnd::OnSetValue(WPARAM wParam, LPARAM lParam)
{
	char* cVal = (char*)lParam;

	if (m_rotateArr.GetSize() > 0)
	{
		cVal[0] = '1';
	}
	else
	{
		cVal[0] = '0';
	}

	return 0;
}

void CControlWnd::RequestGroup()
{
	struct	_updn	updn {};

	FillMemory(&updn, sz_updn, ' ');
	CopyMemory(updn.uinfo.gubn, "MY", sizeof(updn.uinfo.gubn));
	updn.uinfo.dirt[0] = 'U';
	updn.uinfo.cont[0] = 'g';
	CopyMemory(updn.uinfo.nblc, "00001", sizeof(updn.uinfo.nblc));
	updn.uinfo.retc[0] = 'U';

	SendTR("PIDOMYST", (char *)&updn, sz_updn, TR_IGROUP);
}

void CControlWnd::RequestCode(int igroup, int iprocess)
{
	struct	_updn	updn {};
	CString tmps;

	FillMemory(&updn, sz_updn, ' ');
	CopyMemory(updn.uinfo.gubn, "MY", sizeof(updn.uinfo.gubn));
	updn.uinfo.dirt[0] = 'U';
	updn.uinfo.cont[0] = 'j';
	CopyMemory(updn.uinfo.nblc, "00001", sizeof(updn.uinfo.nblc));
	updn.uinfo.retc[0] = 'U';

	tmps.Format("%02d", igroup);
	CopyMemory(updn.ginfo.gnox, tmps, sizeof(updn.ginfo.gnox));

	SendTR("PIDOMYST", (char *)&updn, sz_updn, TR_IDATA+iprocess);
}

void CControlWnd::parseCode(char* datb, int datl, int iprocess)
{
	if (datl < sz_ginfo)
		return;

	struct	_ginfo* group = (struct _ginfo*)datb;
	const	int	cnt = atoi(CString(group->jrec, sizeof(group->jrec)));
	if (cnt <= 0)
		return;

	CString	tmps, rtns;
	CStringArray codes;
	struct	_jinfo* jinfo = (struct _jinfo*)&datb[sz_ginfo];

	for (int ii = 0; ii < cnt; ii++, jinfo++)
	{
		tmps = CString(jinfo->code, sizeof(jinfo->code));
		tmps.Trim();
		if (iprocess == procNONE)
			codes.Add(tmps);
		else
		{
			rtns += tmps;
			rtns += '\t';
		}
	}

	switch (iprocess)
	{
	case procFORM:
		m_rtnStr = m_sSymbol + "\tE\n";
		m_rtnStr += rtns;
		m_pParent->PostMessage(WM_USER, MAKEWPARAM(formDLL, m_Param.key), (LPARAM)m_rtnStr.GetString());
		break;

	case procEVENT:
		m_sCodeList = rtns;
// 원래 주석이었음. 사용하게 되어도 Post로 할 것!!!
//		m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk)), (LPARAM)m_Param.name.GetString());
		break;

	case procNONE:
	default:
		m_pPopup->LoadInterestCode(codes);
		break;
	}
}
