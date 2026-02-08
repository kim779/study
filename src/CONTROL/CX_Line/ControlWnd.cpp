// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Line.h"
#include "ControlWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlWnd

CControlWnd::CControlWnd()
{
	m_iLineColorIdx = 0;
	m_iLineColor = 0xFF0000;
	EnableAutomation();
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

	CStatic::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlWnd, CStatic)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CControlWnd, CStatic)
	//{{AFX_DISPATCH_MAP(CControlWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CControlWnd, "ShowLine", dispidShowLine, ShowLine, VT_EMPTY, VTS_I2)
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {540240F4-55B8-4AE2-A30C-7D13F4A6A062}
static const IID IID_IControlWnd =
{ 0x540240f4, 0x55b8, 0x4ae2, { 0xa3, 0xc, 0x7d, 0x13, 0xf4, 0xa6, 0xa0, 0x62 } };

BEGIN_INTERFACE_MAP(CControlWnd, CStatic)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers


void CControlWnd::SetParam(struct	_param* param, CWnd *parent)
{
	m_pMainWindow = parent;
	CString sopt = param->options;
	m_iLineType = m_iControlType = 0;

	if(sopt != "")
	{
		m_iControlType = atoi( GetFindOptionString(sopt, "ctype", "/").Trim() );
		m_iLineType = atoi( GetFindOptionString(sopt, "dtype", "/").Trim() );
		

		const CString sTmp = GetFindOptionString(sopt, "lcolor", "/").Trim();
		if(!sTmp.IsEmpty())
		{
			m_iLineColorIdx = atoi(sTmp);
			m_iLineColor = getAxColor(m_iLineColorIdx);
		}
		//-------/ctype0/dtype0/lcolor43/w13
	}

//	m_iLineType
	/*

[CX_LINE]
TYPE|3|0-LINE;1-PANEL;|LINE|ctype
LINETYPE|3|0-VERT;1-HORZ;|VERT|dtype
LINECOLOR|2|00001002|43|lcolor
WIDTH|1|||w1

	m_pParam     = (struct _param *)param;
	m_strParamName = CString(m_pParam->name, m_pParam->name.GetLength());
	m_key	  = m_pParam->key;
	CString sopt = m_pParam->options;

	m_iOptionColor[0] = 64;
	if(sopt != "")
	{
		[CX_INTEREST]
		APP_NAME|1||DH200000|ax1
		필드선택콤보|3|0-hide;1-show;|1|t1
		종목명필드수정|3|0-false;1-true;|1|t2
		그리드칼라타입|3|1-한줄칼라;2-두줄칼라;|1|t3
		확대/축소그룹|3|1-show;0-hide;|1|t4
		툴바영역|3|1-show;0-hide;|1|t5
		종목연동|3|1-click;2-DBclick;|1|t6
		확장모드|3|1-true;0-false;|0|t7

		기본필드심볼|1||2023;2024;2033;2027;|tx1
		상세필드심볼|1||2025;2026;2204;|tx2
		배경색|2|00001002|6|cb
		그리드헤더배경|2|00001002|74|c1
		그리드헤더텍스트|2|00001002|76|c2
		그리드칼라1|2|00001002|78|r1
		그리드칼라2|2|00001002|192|r2
		그리드선택칼라|2|00001002|78|sr
		그리드리얼칼라|2|00001002|78|f1
		
		m_iOptionColor[0] = CInt( GetFindOptionString(sopt, "cb", "/") );
		m_iOptionColor[1] = CInt( GetFindOptionString(sopt, "c1", "/") );
		m_iOptionColor[2] = CInt( GetFindOptionString(sopt, "c2", "/") );
		m_iOptionColor[3] = CInt( GetFindOptionString(sopt, "r1", "/") );
		m_iOptionColor[4] = CInt( GetFindOptionString(sopt, "r2", "/") );
		m_iOptionColor[5] = CInt( GetFindOptionString(sopt, "sr", "/") );//select row
		m_iOptionColor[6] = CInt( GetFindOptionString(sopt, "f1", "/") );//real color


		m_iOptionType[0] = CInt( GetFindOptionString(sopt, "t1", "/") );
		m_iOptionType[1] = CInt( GetFindOptionString(sopt, "t2", "/") );
		m_iOptionType[2] = CInt( GetFindOptionString(sopt, "t3", "/") );
		m_iOptionType[3] = CInt( GetFindOptionString(sopt, "t4", "/") );
		m_iOptionType[4] = CInt( GetFindOptionString(sopt, "t5", "/") );
		m_iOptionType[5] = CInt( GetFindOptionString(sopt, "t6", "/") );
		m_iOptionType[6] = CInt( GetFindOptionString(sopt, "t7", "/") );

		CString strtmp =GetFindOptionString(sopt, "t8", "/");
		if(strtmp  == "") m_iOptionType[7] = 1;
		else m_iOptionType[7] = 2;
		
		if(!m_iOptionType[5]) m_iOptionType[5] = 1;

		m_strAppName = GetFindOptionString(sopt, "ax1", "/");
		m_strDefaultFields = GetFindOptionString(sopt, "tx1", "/");
		m_strDetailFields = GetFindOptionString(sopt, "tx2", "/");
	}

	m_clrBack = getAxColor(64);
//	m_clrRealColor = getAxColor(m_iOptionColor[6]);
	m_clrRealColor = getAxColor(78);
	*/
}

CString CControlWnd::GetFindOptionString(CString sData, CString sKey, CString sEndChar)
{
	int iFind = sData.Find(sKey);
	if (iFind < 0) return _T("");

	int iendFind = sData.Find(sEndChar, iFind);
	if (iendFind < 0)
	{
		iendFind = sData.GetLength();
		//		return _T("");
	}


	iFind = iFind + sKey.GetLength();
	CString sResult = sData;
	sResult = sResult.Mid(iFind, iendFind - iFind);
	return sResult;
}

COLORREF CControlWnd::getAxColor(int color)
{
	if (color & 0x02000000)
		return color;
	return SendAxisMessage( MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

HCURSOR CControlWnd::getAxCursor(int ikind)
{
	return (HCURSOR)SendAxisMessage( getCURSOR, (LPARAM)ikind);
}

CFont* CControlWnd::getAxFont(CString fName, int point, bool bBold, bool bItalic)
{
	struct _fontR fontR;

	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point;
	fontR.italic = bItalic; 
	fontR.bold = bBold ? FW_BOLD : FW_NORMAL;
	return (CFont*)SendAxisMessage( getFONT, (long)&fontR);
}


CFont* CControlWnd::getAxFont(struct _fontR* font)
{
	return getAxFont(font->name, font->point, (font->bold == FW_BOLD) ? true : false, font->italic);
}

HBITMAP CControlWnd::getBitmap(CString path, int idir)
{
//	if(idir != 0) path = m_strImagePath + path;

	path = m_strImagePath + path;
	CBitmap*	pBitmap = (CBitmap*)SendAxisMessage( getBITMAP, (LPARAM)path.operator LPCTSTR());
	return pBitmap->operator HBITMAP();
}

CPen* CControlWnd::getAxPen(COLORREF clr, int width, int style)
{
	struct	_penR penR;
	penR.clr   = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*) SendAxisMessage( getPEN, (long)&penR);
}

CString CControlWnd::Variant(int comm, CString data)
{
	CString retvalue = _T("");
	char* dta = (char*)SendAxisMessage(MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue.Format("%s", dta);
	
	return retvalue;
}

LRESULT CControlWnd::SendAxisMessage(WPARAM wParam, LPARAM lPAram)
{	return m_pMainWindow->SendMessage(WM_USER, wParam, lPAram);}


int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CControlWnd::OnDestroy() 
{
	CStatic::OnDestroy();
	
	// TODO: Add your message handler code here
}

void CControlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	if(!m_iControlType)
	{
		//case line
		if(m_iLineType) //horz
		{
			if(cy  > 1)
				SetWindowPos(&CWnd::wndBottom, 0, 0, cx, 1, SWP_NOMOVE   );
		}
		else//vert
		{
			if(cx  > 1)
				SetWindowPos(&CWnd::wndBottom, 0, 0, 1, cy, SWP_NOMOVE   );
		}
	}
}

void CControlWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(&rc);

	if(m_iControlType)
	{
	//	dc.MoveTo(rc.left, rc.top);
	//	dc.LineTo(rc.right, rc.top );  
	}
	else
		dc.FillSolidRect(rc,  m_iLineColor); 
}

void CControlWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CStatic::OnTimer(nIDEvent);
}

void CControlWnd::OnMove(int x, int y) 
{
	CStatic::OnMove(x, y);
	
	// TODO: Add your message handler code here
}

LONG CControlWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	const LONG	ret = 0;
	switch(LOBYTE(LOWORD(wParam)))
	{
		case DLL_OUB:
			break;
		case DLL_ALERT:
			break;
		case DLL_SETPAL:
			if(m_iLineColorIdx > 0)
			{
				m_iLineColor = getAxColor(m_iLineColorIdx);
				Invalidate(FALSE);
			}
			break;

		case DLL_SETFONT:
		case DLL_SETFONTx:
 			break;

		case DLL_TRIGGER: //설정확인도 여기서
			break;
		case DLL_DOMINO:
			break;
		case DLL_GUIDE:
			return 1;
			break;
	}

	return ret;
}

void CControlWnd::ShowLine(SHORT bShow)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (bShow == 1)
		ShowWindow(SW_SHOW);
	else
		ShowWindow(SW_HIDE);
}
