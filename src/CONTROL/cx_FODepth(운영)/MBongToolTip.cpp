#include "stdafx.h"
#include "MBongToolTip.h"
#include "CX_FODepth.h"
#include "MBongLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMBongToolTip::CMBongToolTip(CWnd* pView)
{
	WNDCLASS wndcls{};
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInst, TITLETIP_CLASSNAME, &wndcls)))
	{
		wndcls.style		= CS_SAVEBITS;
		wndcls.lpfnWndProc	= ::DefWindowProc;
		wndcls.cbClsExtra	= wndcls.cbWndExtra = 0;
		wndcls.hInstance	= hInst;
		wndcls.hIcon		= NULL;
		wndcls.hCursor		= LoadCursor(hInst, IDC_ARROW);
		wndcls.hbrBackground	= (HBRUSH)(COLOR_INFOBK + 1); 
		wndcls.lpszMenuName	= NULL;
		wndcls.lpszClassName	= TITLETIP_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))	AfxThrowResourceException();
	}

	m_nCellHeight = 0;
	m_strFontName = _T("±¼¸²");
	m_nFontSize = 9;
	CCX_FODepthApp*	pApp = (CCX_FODepthApp*)AfxGetApp();
	m_font = pApp->GetFont(pView, m_nFontSize, m_strFontName);
}

CMBongToolTip::~CMBongToolTip()
{
	m_rgn.DeleteObject();
}


BEGIN_MESSAGE_MAP(CMBongToolTip, CWnd)
	//{{AFX_MSG_MAP(CMBongToolTip)
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CMBongToolTip::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	DrawBitmap(&dc);
}

int CMBongToolTip::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_clrBack = GetSysColor(COLOR_INFOBK);
	return 0;
}


void CMBongToolTip::DrawBitmap(CDC* pDC)
{
	CRect	rect;
	CString	tmpstr;
	const	int	nCount = m_arTitle.GetSize();

	GetClientRect(rect);
	pDC->FillSolidRect(rect, m_clrBack);	
	CFont*	oldfont = pDC->SelectObject(m_font);
	rect.DeflateRect(1, 1);		

	for (int ii = 0 ; ii < nCount ; ii++)
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

CRect CMBongToolTip::SetData(const CStringArray& arString)
{	
	CString	tmpstr;
	CString	title, content;
	const	int	nCount = arString.GetSize();

	m_arTitle.RemoveAll();
	m_arContent.RemoveAll();

	for (int ii = 0 ; ii < nCount ; ii++)
	{
		tmpstr = arString.GetAt(ii);
		title = GetStrToken(0, tmpstr, ":");
		m_arTitle.Add(title);
		content = GetStrToken(1, tmpstr, ":");
		m_arContent.Add(content);
	}

	return MakeSizeWnd();	
}

LRESULT CMBongToolTip::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_ERASEBKGND)
		return TRUE;
	return CWnd::WindowProc(message, wParam, lParam);
}

CRect CMBongToolTip::MakeSizeWnd() 
{
	CRect	rcWnd;
	CRect	rcDraw;
	CString	tmpstr;
	int	nMaxWidth = 0;
	int	nHeight = 0;
	CSize	size = CSize(0, 0);
	const	int	nGab = 2;
	const	int	nCount = m_arTitle.GetSize(), ii = 0;

	GetWindowRect(rcWnd);
	rcDraw = rcWnd;
	rcDraw.DeflateRect(nGab, nGab, nGab * 2, nGab);
	
	for (int ii = 0 ; ii < nCount ; ii++)
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

CSize CMBongToolTip::CalcuWndSize(CString strExam)
{
	CSize	csSize;
	CDC*	pDC = GetDC();

	CFont*	oldfont = pDC->SelectObject(m_font);
	csSize = pDC->GetTextExtent(strExam);
	pDC->SelectObject(oldfont);
	ReleaseDC(pDC);		
	return csSize;
}

CString CMBongToolTip::GetStrToken(int nIndex, CString data, char* token)
{
	CString retval;
	char*	value = nullptr;
	const	int	nLen = data.GetLength();
	int	ii = 0;

	if (data.IsEmpty())
		return "";

	std::unique_ptr<char[]> dataptr = std::make_unique<char[]>(nLen + 1);
	CopyMemory(dataptr.get(), data, nLen);

	retval = value = strtok(dataptr.get(), token);
	if (ii == nIndex)
	{
		dataptr.reset();
		return retval;
	}

	while (value)
	{
		ii++;
		retval = value = strtok(NULL, token);
		
		if (ii == nIndex)
		{
			dataptr.reset();
			return retval;
		}
	}

	dataptr.reset();
	return "";	
}
