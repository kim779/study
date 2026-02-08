// AxWnd.cpp : implementation file
//

#include "stdafx.h"
#include "AxWnd.h"
#include "controlwnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define cTAB "\t"
#define cDEL 0x7f

// 컨트롤 화면...
__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent, void* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CControlWnd *m_pControlWnd = new CControlWnd(parent);
	//m_pControlWnd->m_pParent = parent;

	m_pControlWnd->SetParam((struct _param*)pParam);
	m_pControlWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, ((struct _param*)pParam)->rect, parent, 1000);
	return m_pControlWnd;
}

/////////////////////////////////////////////////////////////////////////////
// CAxWnd

CAxWnd::CAxWnd(CWnd *pWnd)
{
	m_pWizard = pWnd;
	m_pFont = nullptr;
}

CAxWnd::~CAxWnd()
{
}

BEGIN_MESSAGE_MAP(CAxWnd, CWnd)
	//{{AFX_MSG_MAP(CAxWnd)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, AxMessage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAxWnd message handlers

// Function name	: CAxWnd::AxMessage
// Description		: 플랫폼에 데이터를 받아 처리 하는 함수 
// Return type		: long	
// Argument		: WPARAM wParam
// Argument		: LPARAM lParam
long CAxWnd::AxMessage(WPARAM wParam, LPARAM lParam)	
{
	char*	pcBuffer = NULL;
	UINT	uiLength = 0;
	const	int	key = HIBYTE(LOWORD(wParam));
	
	switch (LOBYTE(LOWORD(wParam)))
	{
	// 플랫폼에 값을 반환  ===============================================================================================
	case DLL_INB:
		// Control 사용시 platforme에 값을 반환...
		return (long)m_rtnStr.GetString();

	// TR 조회시 받는 초기 데이터	======================================================================================
	case DLL_OUBx:
		struct	_extTHx* exth;
		exth = (struct _extTHx*)lParam;
		Oub(exth->data, exth->size, exth->key);
		break;
	case DLL_OUB:	// NOT USED. User DLL_OUBx Only !!!
		pcBuffer = (char*)lParam;
		uiLength = (UINT)HIWORD(wParam);
		Oub(pcBuffer, uiLength, key);
		break;

	// RealData	======================================================================================================
	case DLL_ALERT:
		if (HIWORD(wParam) & alert_DEIN)
		{
			// 체결 데이터가 아니면 거름
			return 0;
		}
		pcBuffer = (char*)lParam;
		Alert(pcBuffer);
		break;
	// Trigger	===========================================================================================================
	case DLL_TRIGGER:
		pcBuffer = (char*)lParam;
		Trigger(pcBuffer);
		break;
	// Domino	=======================================================================================================
	case DLL_DOMINO:
		pcBuffer = (char*)lParam;
		Domino(pcBuffer);
//		ParsingTrigger(pcBuffer);
		break;
	// 계좌Data Receive	=======================================================================================================
	case DLL_NOTICE:
		Notice(pcBuffer);
		break;
//	case DLL_INVOKE:
//		switch (HIWORD(wParam))
//		{
//		case setMapMenu:
//			break;
//		case getMapInfo:
//			static char acBuffer[1024];
//			GetMapInfo(acBuffer, (char*)lParam);
//			return (long)acBuffer;
//			break;
//		case setMapInfo:
//			SetMapInfo((char*)lParam);
//			break;
//		}
		break;
	// color adjust	========================================================================================================
	case DLL_SETPAL:
		break;
	// Font adjust	=====================================================================================================
	case DLL_SETFONT:
		break;
	case DLL_SETFCB:
		break;
	// Drop	=============================================================================================================
	case DLL_DROP:
//		DoDrop(wParam, lParam);
	default:
		break;
	}
	return 0;
}

int CAxWnd::OpenView(CString mapN, int iType)
{
//	#define	typeVIEW		0x01
//	#define	typeMODAL		0x02
//	#define	typePOPUP		0x03
//	#define	typePOPUPX		0x04
	return m_pWizard->SendMessage(WM_USER, MAKEWPARAM(viewDLL, iType), (LPARAM)mapN.GetString());
}

CString CAxWnd::Variant(int iCommand, CString strData)
{
	CString strRetvalue;
	const	char* pcData = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, iCommand), 
						     (LPARAM)strData.GetString());
	if ((long)pcData > 1)
		strRetvalue = pcData;

	return strRetvalue;
}

// 컨트롤 용...
void CAxWnd::SendTRC(CString name, CString data, char stat)
{
	return;

	CString trData = ""; 
	CString tempdata = "";
	CString pass;
	
	struct _userTH udat;
	
	strcpy(udat.trc, name);
	udat.key = m_Param.key;
	udat.stat = stat;

	trData.Empty();
	trData = m_Param.name;
	trData += '\t';
	trData += CString((char *)&udat, L_userTH);
	trData += data;

	if (m_pWizard->GetSafeHwnd())
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, trData.GetLength() - L_userTH - m_Param.name.GetLength() - 1), (LPARAM)trData.GetString());
}

//----------------------------------------------------------------------------------------------------------------
// data Parsing

void CAxWnd::Oub(CString szData, int iDataLength, int key)
{	
}

void CAxWnd::Alert(CString szData)
{
}

void CAxWnd::Domino(CString szData)
{
}

void CAxWnd::Trigger(CString szData)
{
}

void CAxWnd::Notice(CString szData)
{	
}

CString CAxWnd::Comma(CString str)
{
	CString szTemp, stripData = str;
	char dChar = 0x00;

	stripData.TrimLeft(); 
	if (stripData.GetLength() <= 3)
		return stripData;

	switch (stripData[0])
	{
	case '+':
		dChar = '+'; 
		stripData.Delete(0);
		break;
	case '-':
		dChar = '-'; 
		stripData.Delete(0);
		break;
	}

	AfxExtractSubString(szTemp, stripData, 1, '.');
	AfxExtractSubString(stripData, stripData, 0, '.');

	stripData.MakeReverse();
	str = _T("");

	for (int ii = 0; ii < stripData.GetLength(); ii++)
	{
		if ((ii != 0) && ((ii % 3) == 0))
			str += ',';

		str += stripData[ii];
	}

	str.MakeReverse();

	if (!szTemp.IsEmpty())
		str = str + "." + szTemp;
	
	if (dChar != 0x00) 
		str.Insert(0, dChar);

	return str;
}

CString CAxWnd::Parser(CString &srcstr, CString substr)
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

COLORREF CAxWnd::GetIndexColor(int color)
{
	if (!m_pWizard || color & 0x02000000)
		return color;

	return m_pWizard->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

CPen* CAxWnd::GetAxPen(COLORREF clr, int width, int style)
{
	struct	_penR	penR {};

	penR.clr = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)m_pWizard->SendMessage(WM_USER, getPEN, (LPARAM)&penR);
}

CBrush* CAxWnd::GetAxBrush(COLORREF clr)
{
	return (CBrush*)m_pWizard->SendMessage(WM_USER, getBRUSH, (LPARAM)clr);
}

CFont* CAxWnd::GetAxFont(CString fName, int point, int style)
{
	struct _fontR fontR {};

	fontR.name = (LPSTR)fName.GetString();
	fontR.point = point;
	fontR.italic = false;
	fontR.bold = 0;

	switch (style)
	{
	case 0:
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
	return (CFont*)m_pWizard->SendMessage(WM_USER, getFONT, (LPARAM)&fontR);
}

#if 0
HBITMAP CAxWnd::GetBitmap(CString bmps)
{
	return ((CBitmap *)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(getBITMAP, 0), (LPARAM)bmps.GetString()))->operator HBITMAP();
}
#else
CBitmap* CAxWnd::GetBitmap(CString bmps)
{
	return (CBitmap*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(getBITMAP, 0), (LPARAM)bmps.GetString());
}
#endif


// Function name	: CAxWnd::SetParam
// Description		: virtual함수 컨트롤에서만 사용... 
// Return type		: void 
// Argument		: _param *pParam
void CAxWnd::SetParam(_param *pParam)
{
	m_Param.key     = pParam->key;
	m_Param.name    = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect.CopyRect(pParam->rect);
	m_Param.fonts   = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point   = pParam->point;
	m_Param.style   = pParam->style;
	m_Param.tRGB    = pParam->tRGB;
	m_Param.pRGB    = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());

	m_pFont		= GetAxFont(m_Param.name, m_Param.point);
}

// Function name	: CAxWnd::RemoveHistory
// Description		: 플랫폼의 히스토리 코드를 삭제...
// Return type		: bool 
// Argument		: CString str - SYM \t CODE, CODE is empty, remove all
bool CAxWnd::RemoveHistory(CString str)
{
	Variant(codeCCx, str);
	return true;
}

// Function name	: CAxWnd::GetCursor
// Description		: Cursor 모양 바꾸기
// Return type		: void 
// Argument		: int kind
HCURSOR CAxWnd::GetCursor(int kind)
{
	if (m_pWizard)
		return (HCURSOR)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(getCURSOR, 0), (LPARAM)kind);
	return 0;
}

// Function name	: CAxWnd::SetAxFocus
// Description		: 컨트롤에서 포커스가 있는 윈도우를 플랫폼에 알려주는 함수	 
// Return type		: BOOL	포커스를 가질지 안가질지 여부 
BOOL CAxWnd::SetAxFocus(bool bflag)
{
	return TRUE;

	if (m_pWizard)
		return m_pWizard->SendMessage(WM_USER, MAKEWPARAM(focusDLL, MAKEWORD(m_Param.key, bflag)), (LPARAM)this);

	return FALSE;
}


