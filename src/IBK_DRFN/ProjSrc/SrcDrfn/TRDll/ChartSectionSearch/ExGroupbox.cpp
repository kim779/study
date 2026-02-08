// ExGroupbox.cpp : implementation file
//

#include "stdafx.h"
#include "ExGroupbox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define COLOR_MAIN_CANVAS		RGB(185,205,230) 

IMPLEMENT_DYNAMIC(CExGroupbox,CButton);

/////////////////////////////////////////////////////////////////////////////
// CExGroupbox

CExGroupbox::CExGroupbox()
{
	m_strTitle = _T("");
	
	m_clrBorder = ::GetSysColor(COLOR_3DSHADOW);
	m_clrClientBackground = ::GetSysColor(COLOR_BTNFACE);

	m_clrTitleText = ::GetSysColor(COLOR_WINDOWTEXT);
	m_clrTitleBackground = ::GetSysColor(COLOR_BTNFACE);

	m_dwAlignment = SS_LEFT;
}

CExGroupbox::~CExGroupbox()
{
	m_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CExGroupbox, CButton)
	//{{AFX_MSG_MAP(CExGroupbox)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExGroupbox message handlers

BOOL CExGroupbox::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= BS_ICON;
	return CButton::PreCreateWindow(cs);
}

void CExGroupbox::PreSubclassWindow() 
{
	CButton::PreSubclassWindow();
	//modified the style to avoid text overlap when press tab 
	ModifyStyle(0, BS_ICON);
	// Get Defalut Font 
	CFont* cf = GetFont();
	if(cf !=NULL)
	{
		cf->GetObject(sizeof(m_lf), &m_lf);
	}
	else
	{	
		GetObject(GetStockObject(SYSTEM_FONT), sizeof(m_lf), &m_lf);
	}
	ReconstructFont();
}

void CExGroupbox::ReconstructFont()
{
	m_font.DeleteObject();
	BOOL bCreated = m_font.CreateFontIndirect(&m_lf);
	ASSERT(bCreated);
}



CExGroupbox& CExGroupbox::SetAlignment(DWORD dwType)
{
	switch(dwType)
	{
	default	:	ASSERT(false);
	case SS_LEFT:	
		m_dwAlignment =  SS_LEFT;
		break;
	case	SS_CENTER:	
		m_dwAlignment = SS_CENTER;
		break;
	case	SS_RIGHT	:
		m_dwAlignment = SS_RIGHT;
		break;
	}
	UpdateSurface();	
	return *this;
}

CExGroupbox& CExGroupbox::SetBackgroundColor(COLORREF clrBKTilte,  COLORREF clrBKClient)
{
	m_clrTitleBackground = clrBKTilte;
	m_clrClientBackground = clrBKClient;
	UpdateSurface();
	return *this;
}//(캡션영역배경색, 그룹내부 배경색)

CExGroupbox& CExGroupbox::SetBackgroundColor(COLORREF clrBKClient) 
{
	m_clrTitleBackground = clrBKClient;
	m_clrClientBackground = clrBKClient;
	UpdateSurface();
	return *this;
}//배경색 일치

CExGroupbox& CExGroupbox::SetBorderColor(COLORREF clrBorder)
{
	m_clrBorder = clrBorder;
	UpdateSurface();
	return *this;
}
 
CExGroupbox& CExGroupbox::SetCatptionTextColor(COLORREF clrText ) 
{
	m_clrTitleText = clrText;
	UpdateSurface();
	return *this;
}

CExGroupbox& CExGroupbox::SetFont(LOGFONT lf)
{
	CopyMemory(&m_lf, &lf, sizeof(m_lf));
	ReconstructFont();
	UpdateSurface();
	return *this;
}

CExGroupbox& CExGroupbox::SetFontBold(BOOL bBold)
{
	m_lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
	ReconstructFont();
	UpdateSurface();
	return *this;
}

CExGroupbox& CExGroupbox::SetFontItalic(BOOL bSet)
{
	m_lf.lfItalic = bSet;
	ReconstructFont();
	UpdateSurface();
	return *this;	
}

CExGroupbox& CExGroupbox::SetFontName(const CString& strFont, BYTE byCharSet)
{
	m_lf.lfCharSet = byCharSet;

	_tcscpy(m_lf.lfFaceName,strFont);
	ReconstructFont();
	UpdateSurface();

	return *this;
}

CExGroupbox& CExGroupbox::SetFontSize(int nSize)
{
	CFont cf;
	LOGFONT lf;

	cf.CreatePointFont(nSize * 10, m_lf.lfFaceName);
	cf.GetLogFont(&lf);

	m_lf.lfHeight = lf.lfHeight;
	m_lf.lfWidth  = lf.lfWidth;

	ReconstructFont();
	UpdateSurface();
	return *this;
}

CExGroupbox& CExGroupbox::SetFontUnderline(BOOL bSet)
{
	m_lf.lfUnderline = bSet;
	ReconstructFont();
	UpdateSurface();
	return *this;
}

CExGroupbox& CExGroupbox::SetText(LPCTSTR lpszText)
{
	if(IsWindow(this->GetSafeHwnd())) 
	{
		m_strTitle = lpszText;
		m_strTitle = _T(" ") + m_strTitle + _T(" ");
		UpdateSurface();
	}	
	return *this;
}

CExGroupbox& CExGroupbox::SetUserFont(CFont* fontdata)
{
	fontdata->GetLogFont(&m_lf);

	ReconstructFont();
	UpdateSurface();
	return *this;
}
/*
logFont.lfHeight = nHeight;                  \
	logFont.lfWidth = 0;                         \
	logFont.lfEscapement = 0;                    \
	logFont.lfOrientation = 0;                     \
	logFont.lfWeight = nWeight;                     \
	logFont.lfItalic = 0;                             \
	logFont.lfUnderline	=	0;                 \
	logFont.lfStrikeOut	=	0;             \
	logFont.lfCharSet	=	129;             \
	logFont.lfOutPrecision	=	3;              \
	logFont.lfClipPrecision	=	2;               \
	logFont.lfQuality	=	1;                \
	logFont.lfPitchAndFamily=	49;              \
	strcpy(logFont.lfFaceName, (const char*)_T("굴림체"));  \
  fontData.CreateFontIndirect(&logFont);
  */
void CExGroupbox::UpdateSurface()
{
	CRect (rc);
	GetWindowRect(rc);
	RedrawWindow();

	GetParent()->ScreenToClient(rc);
	GetParent()->InvalidateRect(rc,TRUE);
	GetParent()->UpdateWindow();
}


BOOL CExGroupbox::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	dwStyle |= BS_ICON;
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

//그룹 박스 캡션, 내부 프레임, 보더 그리기
void CExGroupbox::OnPaint() 
{
    CPaintDC dc(this); 
	
	CRect reClient;
	GetClientRect(reClient);

	CRect reText, reFrame;	
	// 텍스트 영역
	CSize sizeText;
	
	CFont *pOldFont = dc.SelectObject(&m_font);
  		
	// 타이틀 텍스트 얻어오기
	if ( m_strTitle.IsEmpty() )
	{ 
        GetWindowText(m_strTitle);
		if ( ! m_strTitle.IsEmpty() )
			m_strTitle = _T(" ") + m_strTitle + _T(" ");
	}
	
	if ( ! m_strTitle.IsEmpty() )
	{
		sizeText = dc.GetTextExtent(m_strTitle);
	}
	else
	{
		sizeText.cx = 0;
		sizeText.cy = 0;
	}

	//캡션 정렬 영역 계산
	switch(m_dwAlignment)
	{
	
		case SS_LEFT:	
			reText.top = reClient.top;
			reText.left = reClient.left + 7;
			
			reText.bottom = reText.top + sizeText.cy;
			reText.right = reText.left + sizeText.cx;
			break;
		case	SS_CENTER:	
			reText.top = reClient.top;
			reText.left = reClient.left + (reClient.Width() - sizeText.cx) / 2 ;
			
			reText.bottom = reText.top + sizeText.cy;
			reText.right = reText.left + sizeText.cx;
			break;
		case	SS_RIGHT	:
			reText.top = reClient.top;
			reText.right = reClient.right -10 ;
			
			reText.bottom = reText.top + sizeText.cy;
			reText.left = reText.right - sizeText.cx;
			break;
	}
	reFrame.left = reClient.left;
	reFrame.top = reClient.top + sizeText.cy/2;
	
	reFrame.right = reClient.right;
	reFrame.bottom = reFrame.top + reClient.Height() - sizeText.cy/2; 
	
	// 보더 그리기
	CPen penFrame;
	CBrush brushBKFrame(m_clrTitleBackground);
	
	penFrame.CreatePen(PS_SOLID, 1, m_clrBorder);
	
	CPen* pOldPen = dc.SelectObject(&penFrame);
	CBrush* pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);

	dc.Rectangle(reFrame);
	
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush); 
	
	dc.IntersectClipRect(reText);
	dc.FillSolidRect(reText, m_clrTitleBackground);	
	
	COLORREF clrOldText = dc.SetTextColor(m_clrTitleText);
	UINT nMode = dc.SetBkMode(TRANSPARENT);
	
	dc.DrawText(m_strTitle, &reText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP ); 
	
	// restore DC
	dc.SetBkMode(nMode);
    dc.SetTextColor(clrOldText);
    dc.SelectObject(pOldFont);
	// Do not call CButton::OnPaint() for painting messages
}

//그룹 박스 내부 잔상 제거
BOOL CExGroupbox::OnEraseBkgnd(CDC* pDC) 
{
	CRect rect;
   
	CBrush brushBKFrame(m_clrClientBackground);
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&brushBKFrame);

    CPen penFrame;	
	penFrame.CreatePen(PS_SOLID, 1, m_clrClientBackground);	
	CPen* pOldPen = pDC->SelectObject(&penFrame);

    GetClientRect(&rect);
    pDC->Rectangle(rect.left, rect.top, rect.right, rect.bottom);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush); 

	penFrame.DeleteObject();
	brushBKFrame.DeleteObject();
    
	InvalidateRect(rect, FALSE);
	
	return TRUE;
}
