// ControlWnd.cpp: 구현 파일

#include "pch.h"
#include "CX_ANITICKER.h"
#include "ControlWnd.h"

IMPLEMENT_DYNCREATE(ControlWnd, CWnd)

ControlWnd::ControlWnd()
{
	EnableAutomation();
}

ControlWnd::~ControlWnd()
{
}

void ControlWnd::OnFinalRelease()
{
	CWnd::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(ControlWnd, CWnd)
	//{{AFX_MSG_MAP(ControlWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(ControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(ControlWnd)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(ControlWnd, "SetData", dispidSetData, SetData, VT_I2, VTS_I4 VTS_BSTR)
END_DISPATCH_MAP()

// {F461206F-C87B-452F-81FE-095D6C8A3900}
static const IID IID_IControlWnd =
{ 0xf461206f, 0xc87b, 0x452f, { 0x81, 0xfe, 0x09, 0x5d, 0x6c, 0x8a, 0x39, 0x00 } };

BEGIN_INTERFACE_MAP(ControlWnd, CWnd)
	INTERFACE_PART(ControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()


SHORT ControlWnd::SetData(int gubn, BSTR sVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.

	return 0;
}
