// ControlWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "fx_AGdlg.h"
#include "ControlWnd.h"

#include "../../h/axisfire.h"

// CControlWnd

IMPLEMENT_DYNAMIC(CControlWnd, CWnd)

CControlWnd::CControlWnd()
{

	EnableAutomation();
}

CControlWnd::~CControlWnd()
{
}

void CControlWnd::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면
	// OnFinalRelease가 호출됩니다. 기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	DISP_FUNCTION_ID(CControlWnd, "SetType", dispidSetType, SetType, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CControlWnd, "SetBalListType", dispidSetBalListType, SetBalListType, VT_EMPTY, VTS_BSTR VTS_BSTR)
END_DISPATCH_MAP()

// 참고: IID_IControlWnd에 대한 지원을 추가하여
//  VBA에서 형식 안전 바인딩을 지원합니다. 
//  이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {717FD819-85BE-496F-A003-BFBCF50CF86D}
static const IID IID_IControlWnd =
{ 0x717FD819, 0x85BE, 0x496F, { 0xA0, 0x3, 0xBF, 0xBC, 0xF5, 0xC, 0xF8, 0x6D } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()


// CControlWnd 메시지 처리기입니다.



void CControlWnd::SetParam(_param *pParam)
{
	m_Param.key   = pParam->key;
	m_Param.name  = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect  = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	m_Param.tRGB  = pParam->tRGB;
	m_Param.pRGB  = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());

/*
	CString tmps, strdata;
	tmps = m_Param.options;
	strdata = Parser(tmps, _T("/g"));
	m_Unit = atoi(tmps);
	*/
}

void CControlWnd::SetType(LPCTSTR gubn, LPCTSTR type)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strhome, strname, path, tmps;
	strhome = (char*)m_pWizard ->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), NULL);
	strname = (char*)m_pWizard ->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC), NULL);
	
	path.Format(_T("%s\\user\\%s\\%s"),strhome,strname, _T("authcompany.ini"));

	CString strdata;
	strdata.Format(_T("%s"), gubn);
	strdata.TrimRight();
	WritePrivateProfileString("auth", "company", strdata, path);

	strdata.Format(_T("%s"), type);
	strdata.TrimRight();
	if(strdata.IsEmpty())
		strdata = _T("N");
	WritePrivateProfileString("auth", "chechYN", strdata, path);

	//m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnClick)), (LPARAM)(LPCTSTR)m_Param.name);
}


void CControlWnd::SetBalListType(LPCTSTR balcode, LPCTSTR type)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strhome, strname, path, tmps;
	strhome = (char*)m_pWizard ->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), NULL);
	strname = (char*)m_pWizard ->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC), NULL);
	
	path.Format(_T("%s\\user\\%s\\%s"),strhome,strname, _T("authcompany.ini"));

	CString strdata;
	strdata.Format(_T("%s"), balcode);
	strdata.TrimRight();
	WritePrivateProfileString("auth", "listcompany", strdata, path);

	strdata.Format(_T("%s"), type);
	strdata.TrimRight();
	if(strdata.IsEmpty())
		strdata = _T("N");
	WritePrivateProfileString("auth", "balchechYN", strdata, path);

	//m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk)), (LPARAM)(LPCTSTR)m_Param.name);
	// TODO: 여기에 디스패치 처리기를 추가합니다.
}

