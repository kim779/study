// Price.cpp : implementation file
//

#include "stdafx.h"
#include "SiseWnd.h"
#include "Price.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrice

CPrice::CPrice(CWnd *wizard)
: CAxisExt(wizard)
{
	m_hPriceDll = nullptr;
	m_pPrice = nullptr;
}

CPrice::~CPrice()
{
	//if (m_hPriceDll)
	//	FreeLibrary(m_hPriceDll);
}


BEGIN_MESSAGE_MAP(CPrice, CWnd)
	//{{AFX_MSG_MAP(CPrice)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPrice message handlers

void CPrice::OnDestroy() 
{
	CWnd::OnDestroy();
}

long CPrice::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case formDLL:
		{
			CString str = (char*)lParam;
			//AfxMessageBox(str);
			char chr[100];
			strcpy(chr, str);

			return GetParent()->SendMessage(WM_USER+21, (WPARAM) &chr, lParam);
		}
		break;
	}
	return m_pParent->SendMessage(WM_USER, wParam, lParam);
}

void CPrice::createPrice(int Col,int Row)
{
	CString dir;
	dir.Format("%s\\dev\\cx_Price.dll", m_sRoot);

	CString indexInfo;
	int iInfo;
	indexInfo.Format("%d%d", Col, Row);
	iInfo = atoi(indexInfo);

	struct	_param	param;
	GetClientRect(&param.rect);
	param.name = "c_pric";
	param.fonts = "±¼¸²";
	param.point = 10;
	param.style = 0;
	param.tRGB = 63;
	param.pRGB = 62;
	param.options = "/xpibohoga/rHoGa/d0/i°¡°Ý.bmp";
	param.key = iInfo;

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

void CPrice::SetValues(CString str)
{
	m_rtnStr = str;
	if (m_pPrice)
		m_pPrice->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(DLL_OUB, 0), str.GetLength()), (long)str.operator LPCTSTR());
}

void CPrice::BtnClick()
{
//	AfxMessageBox("BTNCLICK");
	if (m_pPrice)
	{
		m_pPrice->SendMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKELONG(0, 0));
		m_pPrice->SendMessage(WM_LBUTTONUP, MK_LBUTTON, MAKELONG(0, 0));
	}
}

CString CPrice::GetCtrlProperty( LPCSTR prop_name )
{
	//USES_CONVERSION;
	if (!m_pPrice) return "";
	if (!m_pPrice->GetSafeHwnd()) return "";
	
	IDispatch *pDisp = m_pPrice->GetIDispatch(FALSE);
	if (!pDisp) return "";
	
	_variant_t var;
	CComDispatchDriver driver(pDisp);
	driver.GetPropertyByName(_bstr_t(prop_name), &var);
	
	return (LPCSTR)(_bstr_t)var;
}

void CPrice::SetCtrlProperty( _bstr_t prop_name, _variant_t &var )
{
	if (!m_pPrice) return;
	if (!m_pPrice->GetSafeHwnd()) return;
	
	IDispatch *pDisp = m_pPrice->GetIDispatch(FALSE);
	if (!pDisp) return;
	
	CComDispatchDriver driver(pDisp);
	driver.PutPropertyByName(prop_name, &var);
}

void CPrice::OnLButtonDown( UINT nFlags, CPoint point )
{
	BtnClick();
	CWnd::OnLButtonDown(nFlags, point);
}
