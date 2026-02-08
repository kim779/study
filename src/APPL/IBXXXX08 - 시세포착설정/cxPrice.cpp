#include "stdafx.h"
#include "IBXXXX08.h"
#include "cxPrice.h"
#include "libDefine.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCxPrice::CCxPrice()
{
}

CCxPrice::~CCxPrice()
{
	if (m_pPrice)
	{
		m_pPrice->DestroyWindow();
		LIB_DELETE(m_pPrice);
	}
	if (m_hPriceDll)
		FreeLibrary(m_hPriceDll);
}


BEGIN_MESSAGE_MAP(CCxPrice, CWnd)
	//{{AFX_MSG_MAP(CCxPrice)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()

long CCxPrice::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case formDLL:
		{
			CString str = (char*)lParam;
			char chr[100];
			strcpy(chr, str);
			return GetParent()->SendMessage(WM_PRICE, (WPARAM) &chr, lParam);
		}
		break;
	}
	return m_pParent->SendMessage(WM_USER, wParam, lParam);
}

void CCxPrice::OnDestroy() 
{
	CWnd::OnDestroy();
}

void CCxPrice::CreatePrice()
{
	CString dir;
	dir.Format("%s\\dev\\cx_Price.dll", m_sRoot);

	struct	_param	param;
	GetClientRect(&param.rect);
	param.name = "17405";
	param.fonts = "±¼¸²";
	param.point = 10;
	param.style = 0;
	param.tRGB = 63;
	param.pRGB = 62;
	param.options = "/rHoGa/i1BTN.BMP";

	///k1/i1BTN.BMP/t¿ø/red_jprc/xpibohoga

	m_hPriceDll = LoadLibrary(dir);
	if (!m_hPriceDll)
	{
		TRACE("cx_Price.dll load error !!\n");
		return;
	}

	CWnd*	(APIENTRY *axCreate)(CWnd*, void*);
	axCreate = (CWnd* (APIENTRY *)(CWnd*, void*)) GetProcAddress(m_hPriceDll, "axCreate");
	if (axCreate == nullptr)
	{
		TRACE("cx_Price.dll axCtrl Create error !!\n");
		return;
	}
	m_pPrice = axCreate(this, (void*)&param);
}

void CCxPrice::SetValues(CString str)
{
	if (!m_pPrice)	return;
	m_rtnStr = str;
	m_pPrice->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(DLL_OUB, 0), str.GetLength()), (long)str.operator LPCTSTR());
}

void CCxPrice::BtnClick()
{
	if (!m_pPrice)	return;
	m_pPrice->SendMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKELONG(0, 0));
	m_pPrice->SendMessage(WM_LBUTTONUP, MK_LBUTTON, MAKELONG(0, 0));
}
