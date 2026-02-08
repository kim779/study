// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "cx_optionex.h"
#include "controlWnd.h"
#include "../../h/memdc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TM_AUTO	1001
/////////////////////////////////////////////////////////////////////////////
// CControlWnd

CControlWnd::CControlWnd(CWnd* pParent, _param* pParam, BOOL bMini)
	: m_pParent(pParent)
{
	EnableAutomation();

	SetParam(pParam);

	m_capture = false;

	for (int ii = OBJ_C; ii <= OBJ_O; ii++)
	{
		m_hBitmap[ii] = m_hBitmap_dn[ii] = m_hBitmap_hv[ii] = m_hBitmap_off[ii] = nullptr;
		m_bHover[ii] = FALSE;
		m_rcBtn[ii].SetRectEmpty();
	}

	if (m_bShowOFBtn)
	{
		for (int ii = OF_O; ii <= OF_OFF; ii++)
		{
			m_hOFBitmap[ii] = m_hOFBitmap_dn[ii] = m_hOFBitmap_hv[ii] = m_hOFBitmap_off[ii] = nullptr;
			m_bOFHover[ii] = FALSE;
			m_rcOFBtn[ii].SetRectEmpty();
		}

		m_nOFIndex = OF_O;
	}

	m_bTracking = FALSE;
	
	m_bMini = bMini;
	m_bDll = false;
	m_bPut = false;
	m_bOption = false;
	m_nIndex = OBJ_C;

	m_pTip = nullptr;
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
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_PROPERTY_EX(CControlWnd, "CurCode", GetCurCode, SetCurCode, VT_BSTR)
	DISP_PROPERTY_EX(CControlWnd, "MapHandle", GetMapHandle, SetMapHandle, VT_BSTR)
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
		break;
	case DLL_OUB:
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

	m_bShowOFBtn = false;

	//2013.10.22 KSJ 선물옵션전환버튼을 보여줄지여부(디폴트 안보여줌)
	if (m_Param.options.Find("/f") != -1)
		m_bShowOFBtn = atoi(m_Param.options.Mid(2)) == 0 ? false : true;
}

bool CControlWnd::Initialize(bool bDll)
{
// libctrl.cpp에 있는데요?
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	LOGFONT	lf{};
	bool	bReturn = true;

	m_bDll = bDll;
	m_sRoot = Variant(homeCC, "");
	m_backColor = GetIndexColor(m_Param.pRGB);
	
	if (m_sRoot.IsEmpty())
		bReturn = false;

	CString	ipath = m_sRoot + "\\image\\";
	m_sImagePath[OBJ_C] = ipath + "put_btn.bmp";
	m_sImagePath[OBJ_P] = ipath + "call_btn.bmp";
	m_sImagePath[OBJ_U] = ipath + "spin_up_btn.bmp";
	m_sImagePath[OBJ_D] = ipath + "spin_down_btn.bmp";
	m_sImagePath[OBJ_O] = ipath + "put_btn_off.bmp";

	m_sOFImagePath[OF_O] = ipath + "OPTION_BTN.bmp";
	m_sOFImagePath[OF_F] = ipath + "FUTURES_BTN.bmp";
	m_sOFImagePath[OF_OFF] = ipath + "OPTION_BTN_OFF.bmp";

	setFont(m_Param.fonts, m_Param.point, m_Param.style, &lf);
	
	m_font.CreatePointFontIndirect(&lf);
	SetButtonImages();	
	loadingOJcode();
	loadingMOJcode();
	loadingWOJcode();

	if (m_pTip == nullptr)
	{
		m_pTip = std::make_unique<CToolTipCtrl>();
		m_pTip->Create(this, TTS_ALWAYSTIP|TTS_NOPREFIX);
	}

	return bReturn;
}

void CControlWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
#ifdef _DEBUG
	Draw(&dc);
#else
	xxx::CMemDC	mdc(&dc);

	Draw(&mdc);
#endif
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

void CControlWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonUp(nFlags, point);

	if (m_capture)
	{
		bool	bUp = false;

		m_capture = false;
		ReleaseCapture();
		if (m_rcBtn[OBJ_C].PtInRect(point))
		{
			if (m_nIndex != OBJ_O)
				CPButtonAction();			//C/P Button
		}
		else if (m_rcOFBtn[OF_O].PtInRect(point))
		{
			OFButtonAction();
		}
		else
		{
			if (m_rcBtn[OBJ_U].PtInRect(point))	// 이전
			{
				bUp = true;
			}
			else if (m_rcBtn[OBJ_D].PtInRect(point))// 다음
			{
				bUp = false;
			}
			if (m_nIndex != OBJ_O)
				UpDownButtonAction(bUp);
		}

		Invalidate();
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CControlWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme{};

		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	else
	{
		m_bHover[OBJ_C] = m_bHover[OBJ_P] = m_bHover[OBJ_U] = m_bHover[OBJ_D] = m_bHover[OBJ_O] = FALSE;
		m_bOFHover[OF_O] = m_bOFHover[OF_F] = m_bOFHover[OF_OFF] = FALSE;
		int	ii = m_nIndex;
		int	jj = m_nOFIndex;

		if (m_rcBtn[ii].PtInRect(point))
		{
			//KSJ 툴팁
			MSG	msg{};

			msg.hwnd = this->GetSafeHwnd();
			msg.message = WM_MOUSEMOVE;
			m_pTip->Activate(TRUE);
			m_pTip->AddTool(this, "콜/풋옵션 전환");
			m_pTip->RelayEvent(&msg);
			//KSJ

			m_bHover[ii]=TRUE;
			Invalidate();
		}
	//	else if (m_rcOFBtn[jj].PtInRect(point))
		else if (m_bShowOFBtn)
		{
			//KSJ 툴팁			
			if (m_rcOFBtn[jj].PtInRect(point))
			{
				MSG	msg{};

				msg.hwnd = this->GetSafeHwnd();
				msg.message = WM_MOUSEMOVE;
				m_pTip->Activate(TRUE);
				m_pTip->AddTool(this, "근월선물/옵션 전환");
				m_pTip->RelayEvent(&msg);
				//KSJ
				
				m_bOFHover[jj]=TRUE;
				Invalidate();
			}	
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
	const	CPoint	point = CPoint(LOWORD(lParam), HIWORD(lParam));

	m_bHover[OBJ_C] = m_bHover[OBJ_P] = m_bHover[OBJ_U] = m_bHover[OBJ_D] = m_bHover[OBJ_O] = FALSE;
	m_bOFHover[OF_O] = m_bOFHover[OF_F] = m_bOFHover[OF_OFF] = FALSE;

	int	ii = m_nIndex;
	int	jj = m_nOFIndex;

	if (m_rcBtn[ii].PtInRect(point))
	{
		m_bHover[ii]=TRUE;
		Invalidate();
	}
//	else if (m_rcOFBtn[jj].PtInRect(point))  
	else if (m_bShowOFBtn)	
	{
		if (m_rcOFBtn[jj].PtInRect(point))
		{
			m_bOFHover[jj]=TRUE;
			Invalidate();
		}
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
	m_bHover[OBJ_C] = m_bHover[OBJ_P] = m_bHover[OBJ_U] = m_bHover[OBJ_D] = m_bHover[OBJ_O] = FALSE;
	m_bOFHover[OF_O] = m_bOFHover[OF_F] = m_bOFHover[OF_OFF] = FALSE;
	Invalidate();
	return 0;
}

BOOL CControlWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
	return CWnd::OnEraseBkgnd(pDC);
}

void CControlWnd::OnEnable(BOOL bEnable) 
{
	CWnd::OnEnable(bEnable);
	
	Invalidate();	
}

void CControlWnd::OnDestroy() 
{
	if (m_pTip)
		m_pTip.reset();

	CWnd::OnDestroy();	
}

COLORREF CControlWnd::GetIndexColor(int index)
{
	return m_pParent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

void CControlWnd::setFont(CString fName, int fsize, int style, LOGFONT* lf)
{
	lf->lfHeight = fsize * 10; 
	
	strcpy(lf->lfFaceName, fName);  
	
	lf->lfCharSet = DEFAULT_CHARSET;
	lf->lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf->lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf->lfQuality = DEFAULT_QUALITY;
	lf->lfPitchAndFamily = DEFAULT_PITCH;
	
	lf->lfWeight = FW_NORMAL;
	lf->lfItalic = FALSE;
	switch (style)
	{
	case 0: // none
	case 1: // none
		break;
	case 2: // italic
		lf->lfItalic = TRUE;
		break;
	case 3: // bold
		lf->lfWeight = FW_BOLD;
		break;
	case 4: // both
		lf->lfItalic = TRUE;
		lf->lfWeight = FW_BOLD;
		break;
	}
}

CString CControlWnd::Variant(int comm, CString data)
{
	CString retvalue;
	const	char*	dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)data.GetString());

	if ((long)dta > 1)
		retvalue = dta;

	return retvalue;
}

void CControlWnd::SetButtonImages()
{
	CString sImageDir;
	CFileFind finder;

	sImageDir.Format("%s\\image\\", m_sRoot);

	for (int ii = OBJ_C; ii <= OBJ_O; ii++)
	{
		m_sImagePath[ii].MakeLower();
		m_sImagePath[ii].Replace(".bmp", "");
		
		if (finder.FindFile(m_sImagePath[ii] + ".bmp"))
			m_hBitmap[ii] = getBitmap(m_sImagePath[ii] + ".bmp")->operator HBITMAP();
		
		if (finder.FindFile(m_sImagePath[ii] + "_dn.bmp"))
			m_hBitmap_dn[ii] = getBitmap(m_sImagePath[ii] + "_dn.bmp")->operator HBITMAP();
		
		if (finder.FindFile(m_sImagePath[ii] + "_en.bmp"))
			m_hBitmap_hv[ii] = getBitmap(m_sImagePath[ii] + "_en.bmp")->operator HBITMAP();

		if (finder.FindFile(m_sImagePath[ii] + "_OFF.bmp"))
			m_hBitmap_off[ii] = getBitmap(m_sImagePath[ii] + "_OFF.bmp")->operator HBITMAP();
	}

	if (m_bShowOFBtn)
	{
		for (int ii = OF_O; ii <= OF_OFF; ii++)
		{
			CFileFind finder;
			m_sOFImagePath[ii].MakeLower();
			m_sOFImagePath[ii].Replace(".bmp", "");
			
			if (finder.FindFile(m_sOFImagePath[ii] + ".bmp"))
				m_hOFBitmap[ii] = getBitmap(m_sOFImagePath[ii] + ".bmp")->operator HBITMAP();
			
			if (finder.FindFile(m_sOFImagePath[ii] + "_dn.bmp"))
				m_hOFBitmap_dn[ii] = getBitmap(m_sOFImagePath[ii] + "_DN.bmp")->operator HBITMAP();
			
			if (finder.FindFile(m_sOFImagePath[ii] + "_en.bmp"))
				m_hOFBitmap_hv[ii] = getBitmap(m_sOFImagePath[ii] + "_EN.bmp")->operator HBITMAP();
			
			if (finder.FindFile(m_sOFImagePath[ii] + "_OFF.bmp"))
				m_hOFBitmap_off[ii] = getBitmap(m_sOFImagePath[ii] + "_OFF.bmp")->operator HBITMAP();
		}
	}
}

void CControlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	CRect	rc;

	GetClientRect(&rc);
	m_rcAll = rc;

	if (m_bShowOFBtn)
	{
		m_rcOFBtn[OF_O] = rc;
		m_rcOFBtn[OF_O].right = rc.Height() -1;
		m_rcOFBtn[OF_OFF] = m_rcOFBtn[OF_F] = m_rcOFBtn[OF_O];
		
		m_rcBtn[OBJ_C] = rc;
		m_rcBtn[OBJ_C].left = m_rcOFBtn[OF_O].right;
		m_rcBtn[OBJ_C].right = m_rcOFBtn[OF_O].right + rc.Height() +1;
		m_rcBtn[OBJ_O] = m_rcBtn[OBJ_P] = m_rcBtn[OBJ_C];
	}
	else
	{
		m_rcBtn[OBJ_C] = rc;
		m_rcBtn[OBJ_C].right = rc.Height()-1;
		m_rcBtn[OBJ_O] = m_rcBtn[OBJ_P] = m_rcBtn[OBJ_C];
	}

	m_rcBtn[OBJ_U] = rc;
	m_rcBtn[OBJ_U].left = m_rcBtn[OBJ_C].right;
	m_rcBtn[OBJ_U].bottom = rc.CenterPoint().y;
	
	m_rcBtn[OBJ_D] = rc;
	m_rcBtn[OBJ_D].left = m_rcBtn[OBJ_C].right;
	m_rcBtn[OBJ_D].top = rc.CenterPoint().y;
}

void CControlWnd::loadingMOJcode()
{
	if (m_mojcode.GetSize() > 0)
		return;	
	
	CFile	fileH;
	CString	filePath;
	
	filePath.Format("%s/%s/%s", m_sRoot, TABDIR, "mocode.dat");
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary | CFile::shareDenyNone))
	{
		AfxMessageBox("옵션 파일을 읽을 수 없습니다.");
		return;
	}
	
	int	size = (int)fileH.GetLength();
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(size + 1);
	int	len = fileH.Read(pWb.get(), size);
	fileH.Close();

	if (len != size)
	{
		pWb.reset();
		return;
	}

	len = sizeof(struct ojcodh);
	size = (size - len) / sizeof(struct ojcode);
	struct  ojcode* ojcode = (struct ojcode*)&pWb[len];
	for (int ii = 0; ii < size; ii++, ojcode++)
	{
		m_mojcode.Add(*ojcode);
	}
	pWb.reset();
}

void CControlWnd::loadingOJcode()
{
	if (m_ojcode.GetSize() > 0)
		return;	
	
	CFile	fileH;
	CString	filePath;
	
	filePath.Format("%s/%s/%s", m_sRoot, TABDIR, OJCODE);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary | CFile::shareDenyNone))
	{
		AfxMessageBox("옵션 파일을 읽을 수 없습니다.");
		return;
	}

	int	size = (int)fileH.GetLength();
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(size + 1);
	int	len = fileH.Read(pWb.get(), size);
	fileH.Close();
	
	if (len != size)
	{
		pWb.reset();
		return;
	}

	len = sizeof(struct ojcodh);
	size = (size - len) / sizeof(struct ojcode);
	struct  ojcode*  ojcode = (struct ojcode *)&pWb[len];
	for (int ii = 0; ii < size; ii++, ojcode++)
	{
		m_ojcode.Add(*ojcode);
	}
	pWb.reset();
}

void CControlWnd::loadingWOJcode()
{
	if (m_wojcode.GetSize() > 0)
		return;	
	
	CFile	fileH;
	CString	filePath;
	
	filePath.Format("%s/%s/%s", m_sRoot, TABDIR, WOJCODE);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary | CFile::shareDenyNone))
	{
		AfxMessageBox("옵션 파일을 읽을 수 없습니다.");
		return;
	}
	
	int	size = (int)fileH.GetLength();
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(size + 1);
	int	len = fileH.Read(pWb.get(), size);
	fileH.Close();

	if (len != size)
	{
		pWb.reset();
		return;
	}

	len = sizeof(struct ojcodh);
	size = (size - len) / sizeof(struct ojcode);
	struct  ojcode* ojcode = (struct ojcode*)&pWb[len];
	for (int ii = 0; ii < size; ii++, ojcode++)
	{
		m_wojcode.Add(*ojcode);
	}
	pWb.reset();
}

CBitmap* CControlWnd::getBitmap(CString path)
{
	return (CBitmap*)m_pParent->SendMessage(WM_USER, getBITMAP, (LPARAM)path.GetString());
}

void CControlWnd::Draw(CDC *pDC)
{
	pDC->FillSolidRect(&m_rcAll, m_backColor);//GetSysColor(COLOR_BTNFACE));

	CBitmap	*bmp = nullptr;
	int	ii = m_nIndex;

	if (m_capture && m_bHover[ii] && m_hBitmap_dn[ii])
		bmp = CBitmap::FromHandle(m_hBitmap_dn[ii]);
	else if (m_bHover[ii] && m_hBitmap_hv[ii])
		bmp = CBitmap::FromHandle(m_hBitmap_hv[ii]);
	else if (m_bHover[ii] && m_hBitmap_off[ii])
		bmp = CBitmap::FromHandle(m_hBitmap_off[ii]);
	else if (m_hBitmap)
		bmp = CBitmap::FromHandle(m_hBitmap[ii]);

	if (bmp)
		DrawStretchByMask(pDC, bmp, m_rcBtn[ii], RGB(255, 0, 255));

	if (m_bShowOFBtn)
	{
		int	ii = m_nOFIndex;
		if (m_capture && m_bOFHover[ii] && m_hOFBitmap_dn[ii])
			bmp = CBitmap::FromHandle(m_hOFBitmap_dn[ii]);
		else if (m_bOFHover[ii] && m_hOFBitmap_hv[ii])
			bmp = CBitmap::FromHandle(m_hOFBitmap_hv[ii]);
		else if (m_bOFHover[ii] && m_hOFBitmap_off[ii])
			bmp = CBitmap::FromHandle(m_hOFBitmap_off[ii]);
		else if (m_hOFBitmap)
			bmp = CBitmap::FromHandle(m_hOFBitmap[ii]);
		else
			bmp = NULL;
		
		DrawStretchByMask(pDC, bmp, m_rcOFBtn[ii], RGB(255, 0, 255));
	}

	DrawButton(pDC, ii);

	if (!m_bMini)
	{
		for (ii = OBJ_U; ii <= OBJ_D; ii++)
		{
			if (m_capture && m_bHover[ii] && m_hBitmap_dn[ii])
				bmp = CBitmap::FromHandle(m_hBitmap_dn[ii]);
			else if (m_bHover[ii] && m_hBitmap_hv[ii])
				bmp = CBitmap::FromHandle(m_hBitmap_hv[ii]);
			else if (m_nIndex == OBJ_O)
				bmp = CBitmap::FromHandle(m_hBitmap_off[ii]);
			else if (m_hBitmap)
				bmp = CBitmap::FromHandle(m_hBitmap[ii]);
			else	
				bmp = NULL;

			DrawStretchByMask(pDC, bmp, m_rcBtn[ii], RGB(255, 0, 255));
		}
	}
}

void CControlWnd::DrawButton(CDC *pDC, int index)	//2013.10.23 bButton이 false면 콜풋버튼이고, true면 선옵버튼임
{
	const	COLORREF color = m_backColor;
	pDC->SetBkColor(color);
// 	m_nIndex = OBJ_C;

	switch (index)
	{
	case OBJ_U:
		pDC->DrawFrameControl(m_rcBtn[index], DFC_SCROLL, DFCS_SCROLLUP | (m_capture && m_bHover[index] ? DFCS_PUSHED:NULL));
		break;
	case OBJ_D:
		pDC->DrawFrameControl(m_rcBtn[index], DFC_SCROLL, DFCS_SCROLLDOWN | (m_capture && m_bHover[index] ? DFCS_PUSHED:NULL));
		break;
	case OBJ_O:	//Disable
		m_nIndex = OBJ_O;
		m_nOFIndex = OF_O;
			
		//Disabled 일때 글자색 회색으로 바꾸기
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

	BITMAP  bm{};
	pBitmap->GetBitmap(&bm);

	const	int bx = bm.bmWidth, by = bm.bmHeight;
	CRect	src;

	GetWindowRect(&src);
	CDC  buffDC, maskDC, memoryDC, copyDC;
	CBitmap  buffBitmap, maskBitmap, copyBitmap;
	
	buffDC.CreateCompatibleDC(pDC);
	buffBitmap.CreateCompatibleBitmap(pDC, bx, by);
	CBitmap* oldbuffBitmap = buffDC.SelectObject(&buffBitmap);
	
	maskDC.CreateCompatibleDC(pDC);
	maskBitmap.CreateBitmap(bx, by, 1, 1, NULL);
	CBitmap* oldmaskBitmap = maskDC.SelectObject(&maskBitmap);
	
	const	CRect  maskRc( 0, 0, bx, by);
	buffDC.FillSolidRect(&maskRc, maskcolor);
	
	memoryDC.CreateCompatibleDC(pDC);
	CBitmap* oldmemoryBitmap = memoryDC.SelectObject(pBitmap);
	buffDC.BitBlt( 0, 0, bx, by, &memoryDC, 0, 0, SRCCOPY);
	maskDC.BitBlt( 0, 0, bx, by, &buffDC, 0, 0, SRCCOPY);
	
	copyDC.CreateCompatibleDC(pDC);
	copyBitmap.CreateCompatibleBitmap(pDC, bx, by);
	CBitmap* oldcopyBitmap = copyDC.SelectObject(&copyBitmap);
	
	copyDC.BitBlt( 0, 0, bx, by, pDC, bRc.left, bRc.top, SRCCOPY);
	copyDC.BitBlt( 0, 0, bx, by, &maskDC, 0, 0, SRCAND);
	buffDC.BitBlt( 0, 0, bx, by, &maskDC, 0, 0, SRCMASK);
	copyDC.BitBlt( 0, 0, bx, by, &buffDC, 0, 0, SRCPAINT);
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

void CControlWnd::ChangeCode()
{
	// 20230125 파생상품 코드 개편  '1', 'A' : 선물
	//                              '2', 'B' : Call option
	//			        '3', 'C' : Put option
	//			        '4', 'D' : 스프레드
	char	ch = '\0';

	switch (m_strCurCode.GetAt(0))
	{
	case '2':	ch = '3';	break;	// Call
	case 'B':	ch = 'C';	break;	// Call
	case '3':	ch = '2';	break;	// Put
	case 'C':	ch = 'B';	break;	// Put
	default:	break;
	}

	if (ch != '\0')
		m_strCurCode.SetAt(0, ch);
}

void CControlWnd::SendTrigger()
{
	CString string;

	string.Format("%s\t%s", "ed_focod", m_strCurCode);	
	Variant(triggerCC, string);
	
	string.Format("%s\t%s", "40301", m_strCurCode);	
	Variant(triggerCC, string);

	string.Format("%s\t%s", "ed_cfocod", m_strCurCode);	
	Variant(triggerCC, string);
	
	if (m_bDll)	//Dll에서 호출 했을 경우 메세지를 보낸다.
	{
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange/*Change*/)), 
			(LPARAM)m_strCurCode.GetString());
	}
	else	//맵에 포함되어 있을때 이벤트를 발생시킨다.
	{
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange/*Change*/)), 
			(LPARAM)m_Param.name.GetString());
	}

	//2200에서는 맵핸들을 세팅함.
	if (!m_sMapHandle.IsEmpty())
	{
		string.Format("SetOPCode\t%s,%s", m_strCurCode, m_sMapHandle);
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
	}	
}

void CControlWnd::CPButtonAction()
{
	m_bPut = !m_bPut;
	ChangeCode();
	SendTrigger();
}

//2013.10.23 KSJ 최근월선물과 최근월옵션 전환
void CControlWnd::OFButtonAction()
{
	m_bOption = !m_bOption;
	GetOFCode();
	SendTrigger();
}

void CControlWnd::GetOFCode()
{
	CString strCode;

	if (m_bOption)
	{
		if (m_bPut)
			strCode = (LPCSTR)m_pParent->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(getCODEcall, 2)), 0);
		else
			strCode = (LPCSTR)m_pParent->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(getCODEput, 2)), 0);
	}
	else
		strCode = (LPCSTR)m_pParent->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(getCODEfuture, 2)), 0);

	m_strCurCode = strCode;
}

void CControlWnd::UpDownButtonAction_MOpCode(bool bUp)
{
	struct ojcode	ojcode;
	CString code, name;
	
	int	nRow = 0, nCol = 0;
	bool	bSearch = false;
	
	//위치 검색
	for (nRow = 0; nRow < m_mojcode.GetSize(); nRow++)
	{
		//행사가격별
		ojcode = m_mojcode.GetAt(nRow);
		
	//	for (nCol = 0; nCol < 4; nCol++)
	//	{
		//월별	
		if (m_bPut)  //콜
		{
			for (nCol = 7; nCol < 11; nCol++)
			{
				if (ojcode.call[nCol].yorn == '1')
				{
					code = CString(ojcode.call[nCol].cod2, OCodeLen);	
					if (!m_strCurCode.Compare(code))
					{
						bSearch = true;
						break;				
					}
				}
			}
		
		}
		else   //풋
		{
			for (nCol = 0; nCol < 4; nCol++)
			{
				if (ojcode.put[nCol].yorn == '1')
				{
					code = CString(ojcode.put[nCol].cod2, OCodeLen);	
					if (!m_strCurCode.Compare(code))
					{
						bSearch = true;
						break;				
					}
				}
				
			
			}
			
		}
		
		if (bSearch)
			break;
/*
		if (!m_strCurCode.Compare(code))
		{
			bSearch = true;
			break;
		}
		*/
	}
	
//	if (bSearch)
//		break;
//	}
	
	//위쪽이면 하나더해주고, 아래쪽이면 하나빼준다.
	if (bUp)
		nRow++;
	else
		nRow--;
	
	if (nRow <= m_mojcode.GetSize() && nRow >= 0)
	{
		ojcode = m_mojcode.GetAt(nRow);
		
		if (m_bPut)
		{
			if (ojcode.call[nCol].yorn != '1')
				code = m_strCurCode;
			else
				code = CString(ojcode.call[nCol].cod2, OCodeLen);				
		}
		else
		{
			if (ojcode.put[nCol].yorn != '1')
				code = m_strCurCode;
			else
				code = CString(ojcode.put[nCol].cod2, OCodeLen);				
		}
	}
	
	if (bSearch)
		m_strCurCode = code;
	
	SendTrigger();
}

void CControlWnd::UpDownButtonAction_WOpCode(bool bUp)
{
	struct ojcode	ojcode;
	CString code = _T(""), name = _T("");
	
	int	nRow = 0, nCol = 0;
	bool	bSearch = false;
	
	//위치 검색
	for (nRow = 0; nRow < m_wojcode.GetSize(); nRow++)
	{
		//행사가격별
		ojcode = m_wojcode.GetAt(nRow);
		//월별
		if (m_bPut)
		{
			for (nCol = 9; nCol < 11; nCol++)
			{
				if (ojcode.call[nCol].yorn == '1')
				{
					code = CString(ojcode.call[nCol].cod2, OCodeLen);	
					if (!m_strCurCode.Compare(code))
					{
						bSearch = true;
						break;
					}
				}
			}
						
		}
		else
		{
			for (nCol = 0; nCol < 2; nCol++)
			{
				if (ojcode.put[nCol].yorn == '1')
				{
					//	continue;
					code = CString(ojcode.put[nCol].cod2, OCodeLen);
					if (!m_strCurCode.Compare(code))
					{
						bSearch = true;
						break;
					}
				}
			}
		}
			
		if (bSearch)
			break;
	}
	
	//위쪽이면 하나더해주고, 아래쪽이면 하나빼준다.
	if (bUp)
		nRow++;
	else
		nRow--;
	
	if (nCol < 11 && nRow <= m_wojcode.GetSize() && nRow >= 0 && nRow < m_wojcode.GetSize())
	{
		ojcode = m_wojcode.GetAt(nRow);
		
		if (m_bPut)  //콜
		{
			if (ojcode.call[nCol].yorn != '1')
				code = m_strCurCode;
			else
				code = CString(ojcode.call[nCol].cod2, OCodeLen);				
		}
		else         //풋
		{
			if (ojcode.put[nCol].yorn != '1')
				code = m_strCurCode;
			else
				code = CString(ojcode.put[nCol].cod2, OCodeLen);				
		}
	}
	
	if (bSearch)
		m_strCurCode = code;
	
	SendTrigger();
}

void CControlWnd::UpDownButtonAction(bool bUp)
{
	if (m_bOption)
	{
		switch (m_strCurCode.GetAt(2))
		{
		case '9':
			UpDownButtonAction_WOpCode(bUp);	
			return;
		case '5':
			UpDownButtonAction_MOpCode(bUp);	
			return;
		default:break;
		}
	}
	
	struct ojcode	ojcode;
	CString code, name;
	int	nRow = 0, nCol = 0;
	bool	bSearch = false;
	
	//위치 검색
	for (nRow = 0; nRow < m_ojcode.GetSize(); nRow++)
	{
		//행사가격별
		ojcode = m_ojcode.GetAt(nRow);
		
		//월별	
		if (m_bPut)  //콜
		{
			for (nCol = 7; nCol < 11; nCol++)
			{
				if (ojcode.call[nCol].yorn == '1')
				{
					code = CString(ojcode.call[nCol].cod2, OCodeLen);	
					if (!m_strCurCode.Compare(code))
					{
						bSearch = true;
						break;				
					}
				}
			}
		}
		else   //풋
		{
			for (nCol = 0; nCol < 4; nCol++)
			{
				if (ojcode.put[nCol].yorn == '1')
				{
					code = CString(ojcode.put[nCol].cod2, OCodeLen);	
					if (!m_strCurCode.Compare(code))
					{
						bSearch = true;
						break;				
					}
				}
			}
		}
		
		if (bSearch)
			break;
	}
	
	//위쪽이면 하나더해주고, 아래쪽이면 하나빼준다.
	if (bUp)
		nRow++;
	else
		nRow--;
	
	if (nRow <= m_ojcode.GetSize() && nRow >= 0)
	{
		ojcode = m_ojcode.GetAt(nRow);
		if (m_bPut)
		{
			if (ojcode.call[nCol].yorn != '1')
				code = m_strCurCode;
			else
				code = CString(ojcode.call[nCol].cod2, OCodeLen);				
		}
		else
		{
			if (ojcode.put[nCol].yorn != '1')
				code = m_strCurCode;
			else
				code = CString(ojcode.put[nCol].cod2, OCodeLen);				
		}
	}
	
	if (bSearch)
		m_strCurCode = code;
	
	SendTrigger();
}

//////////////////////////////////////////////////////////////////////////
void CControlWnd::SetEnable(bool bEnable)
{
	this->EnableWindow(bEnable);
}

BSTR CControlWnd::GetCurCode() 
{
	CString strResult = m_strCurCode;
	// TODO: Add your property handler here

	return strResult.AllocSysString();
}

void CControlWnd::SetCurCode(LPCTSTR lpszNewValue) 
{
	// TODO: Add your dispatch handler code here

	m_strCurCode = lpszNewValue;
	const	bool	bEnable = true;

	// 20230125 파생상품 코드 개편  '1', 'A' : 선물
	//                              '2', 'B' : Call option
	//			        '3', 'C' : Put option
	//			        '4', 'D' : 스프레드
	if (m_strCurCode.GetAt(0) == '2' || m_strCurCode.GetAt(0) == 'B')	// Call
	{
		m_nIndex = OBJ_C;
		m_nOFIndex = OF_F;
		m_bPut = true;
		m_bOption = true;
	}
	else if (m_strCurCode.GetAt(0) == '3' || m_strCurCode.GetAt(0) == 'C')	// Put
	{
		m_nIndex = OBJ_P;
		m_nOFIndex = OF_F;
		m_bPut = false;
		m_bOption = true;
	}
	else
	{
		m_nIndex = OBJ_O;
		m_nOFIndex = OF_O;
		m_bOption = false;
// 		bEnable = false;
	}

	CDC * pDC = GetDC();
	DrawButton(pDC, m_nIndex);

	Invalidate(FALSE);

	SetEnable(bEnable);
}

BSTR CControlWnd::GetMapHandle() 
{
	CString strResult = m_sMapHandle;
	// TODO: Add your property handler here

	return strResult.AllocSysString();
}

void CControlWnd::SetMapHandle(LPCTSTR lpszNewValue) 
{
	// TODO: Add your property handler here
	m_sMapHandle = lpszNewValue;
}