#include "stdafx.h"
#include "AxToolTipWnd.h"
#include "C_MBong.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAxToolTipWnd::CAxToolTipWnd(CWnd* pView)
{
	WNDCLASS wndcls{};
	HINSTANCE hInst = AfxGetInstanceHandle();
	if(!(::GetClassInfo(hInst, TITLETIP_CLASSNAME, &wndcls)))
	{
		wndcls.style		= CS_SAVEBITS;
		wndcls.lpfnWndProc	= ::DefWindowProc;
		wndcls.cbClsExtra	= wndcls.cbWndExtra = 0;
		wndcls.hInstance	= hInst;
		wndcls.hIcon		= nullptr;
		wndcls.hCursor		= LoadCursor(hInst, IDC_ARROW);
		wndcls.hbrBackground	= (HBRUSH)(COLOR_INFOBK + 1); 
		wndcls.lpszMenuName	= nullptr;
		wndcls.lpszClassName	= TITLETIP_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))	AfxThrowResourceException();
	}

	m_nCellHeight = 0;
	m_strFontName = _T("±¼¸²");
	m_nFontSize = 9;
	CC_MBongApp* pApp = (CC_MBongApp*)AfxGetApp();
	m_font = pApp->GetFont(pView, m_nFontSize, m_strFontName);
}

CAxToolTipWnd::~CAxToolTipWnd()
{
	m_rgn.DeleteObject();
}


BEGIN_MESSAGE_MAP(CAxToolTipWnd, CWnd)
	//{{AFX_MSG_MAP(CAxToolTipWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CAxToolTipWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	DrawBitmap(&dc);
}

int CAxToolTipWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_clrBack = GetSysColor(COLOR_INFOBK);
	return 0;
}


void CAxToolTipWnd::DrawBitmap(CDC* pDC)
{
	CDC		memDC;
	CRect		rect;
	CString		tmpstr;
	int		ii = 0;
	const int		nCount = m_arTitle.GetSize();
	CFont*		oldfont = nullptr;

	GetClientRect(rect);
	pDC->FillSolidRect(rect, m_clrBack);	
	oldfont = pDC->SelectObject(m_font);
	rect.DeflateRect(1, 1);		
//	rect = m_rcDraw;

//	int	nCellHeight = rect.Height() / nCount;

	
	for ( ii = 0 ; ii < nCount ; ii++ )
	{
		rect.bottom = rect.top + m_nCellHeight;
		tmpstr = m_arTitle.GetAt(ii);
		pDC->DrawText(tmpstr, rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
		tmpstr = m_arContent.GetAt(ii);
		pDC->DrawText(tmpstr, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
		rect.top = rect.bottom;
	}
	
	pDC->SelectObject(oldfont);
}

CRect CAxToolTipWnd::SetData(const CStringArray& arString)
{	
	const int nCount = arString.GetSize();
	int	ii = 0;
	CString	tmpstr;
	CString	title, content;

	m_arTitle.RemoveAll();
	m_arContent.RemoveAll();

	for ( ii = 0 ; ii < nCount ; ii++ )
	{
		tmpstr = arString.GetAt(ii);
		title = LIB_GetStrToken(0, tmpstr, ":");
		m_arTitle.Add(title);
		content = LIB_GetStrToken(1, tmpstr, ":");
		m_arContent.Add(content);
	}

	return MakeSizeWnd();	
}

LRESULT CAxToolTipWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_ERASEBKGND)
		return TRUE;
	return CWnd::WindowProc(message, wParam, lParam);
}

CRect CAxToolTipWnd::MakeSizeWnd() 
{
	CRect		rcWnd;
	CRect		rcDraw;
	CString		tmpstr;
	const int	nCount = m_arTitle.GetSize();
	int		nMaxWidth = 0;
	int		nHeight = 0;
	const int	nGab = 2;
	CSize		size = CSize(0, 0);
	
	GetWindowRect(rcWnd);
	rcDraw = rcWnd;
	rcDraw.DeflateRect(nGab, nGab, nGab * 2, nGab);
	
	for (int  ii = 0 ; ii < nCount ; ii++ )
	{
		tmpstr.Format("%s  %s", m_arTitle.GetAt(ii), m_arContent.GetAt(ii));
		size = CalcuWndSize(tmpstr);
	
		if (size.cx > nMaxWidth)
			nMaxWidth = size.cx;
	}

	m_nCellHeight = nHeight = size.cy + 2;
	rcDraw.right = rcDraw.left + nMaxWidth;
	rcDraw.bottom = rcDraw.top + (nHeight * nCount);

	rcWnd = rcDraw;
	m_rcDraw = rcDraw;
	ScreenToClient(m_rcDraw);	
	m_rcBase = rcWnd;	
	return rcWnd;
}

CSize CAxToolTipWnd::CalcuWndSize(CString strExam)
{
	CSize	csSize;
	CFont	*oldfont = nullptr;
	CDC*	pDC = GetDC();

	oldfont = pDC->SelectObject(m_font);
	csSize = pDC->GetTextExtent(strExam);
	pDC->SelectObject(oldfont);
	ReleaseDC(pDC);		
	return csSize;
}
