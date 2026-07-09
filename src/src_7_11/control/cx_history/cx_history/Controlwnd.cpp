// Controlwnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "cx_history.h"
#include "Controlwnd.h"

#include "../../../h/axisfire.h"

// CControlwnd

IMPLEMENT_DYNAMIC(CControlwnd, CWnd)

CControlwnd::CControlwnd()
{

	EnableAutomation();
	m_pWizard = NULL;
}

CControlwnd::~CControlwnd()
{
}

void CControlwnd::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면
	// OnFinalRelease가 호출됩니다. 기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlwnd, CWnd)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlwnd, CWnd)
	DISP_FUNCTION_ID(CControlwnd, "Gethistory", dispidGethistory, Gethistory, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CControlwnd, "GethistoryCode", dispidGethistoryCode, GethistoryCode, VT_BSTR, VTS_NONE)
END_DISPATCH_MAP()

// 참고: IID_IControlwnd에 대한 지원을 추가하여
//  VBA에서 형식 안전 바인딩을 지원합니다. 
//  이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {284956DB-5AE0-4542-8683-F622CC39BC9E}
static const IID IID_IControlwnd =
{ 0x284956DB, 0x5AE0, 0x4542, { 0x86, 0x83, 0xF6, 0x22, 0xCC, 0x39, 0xBC, 0x9E } };

BEGIN_INTERFACE_MAP(CControlwnd, CWnd)
	INTERFACE_PART(CControlwnd, IID_IControlwnd, Dispatch)
END_INTERFACE_MAP()


// CControlwnd 메시지 처리기입니다.


CString CControlwnd::Parser(CString &srcstr, CString substr)
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
	return _T("");
}


BSTR CControlwnd::Gethistory(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 여기에 디스패치 처리기를 추가합니다.
	CString tmpS, temp;
	CStringArray arr;
	CString	strHistory = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, historyCC), (long)("1301"));
	while(1)
	{
		tmpS = Parser(strHistory, _T("\t"));
		temp = Parser(tmpS, _T(" "));
		arr.Add(tmpS.TrimRight());
		if(arr.GetSize() == 6)
			break;
	}
	strResult.Empty();
	for(int ii = 0 ; ii < arr.GetSize() ; ii++)
	{
		strResult += arr.GetAt(ii);

		if(ii != arr.GetSize() - 1)
			strResult += _T("\t");
	}

	return strResult.AllocSysString();
}


BSTR CControlwnd::GethistoryCode(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strResult;
	CString tmpS, temp;
	CStringArray arr;
	CString	strHistory = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, historyCC), (long)("1301"));
	while(1)
	{
		tmpS = Parser(strHistory, _T("\t"));
		temp = Parser(tmpS, _T(" "));
		arr.Add(temp.TrimRight());
		if(arr.GetSize() == 6)
			break;
	}
	strResult.Empty();
	for(int ii = 0 ; ii < arr.GetSize() ; ii++)
	{
		strResult += arr.GetAt(ii);

		if(ii != arr.GetSize() - 1)
			strResult += _T("\t");
	}

	return strResult.AllocSysString();
}
