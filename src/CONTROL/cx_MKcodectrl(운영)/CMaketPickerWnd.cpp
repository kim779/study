// CMaketPickerWnd.cpp: 구현 파일
//

#include "StdAfx.h"
#include "cx_MKcodectrl.h"
#include "CMaketPickerWnd.h"
#include "ControlWnd.h"
#include "../H/axisfire.h"

// CMaketPickerWnd

IMPLEMENT_DYNAMIC(CMaketPickerWnd, CWnd)

CMaketPickerWnd::CMaketPickerWnd()
{

}

CMaketPickerWnd::~CMaketPickerWnd()
{
}


BEGIN_MESSAGE_MAP(CMaketPickerWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CMaketPickerWnd 메시지 처리기


CString CMaketPickerWnd::Parser(CString& srcstr, CString substr)
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
	return "";
}

int CMaketPickerWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	//Init();
	return 0;
}


void CMaketPickerWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
	CRect	rc;
	CString txt;

	GetClientRect(&rc);
	dc.FillSolidRect(rc, RGB(255,255,255));

	dc.SetBkMode(TRANSPARENT);
	if (m_hIBitmap && m_hKBitmap && m_hNBitmap)
	{
		CDC	memDC;
		CBitmap* pOldBitmap = nullptr, * pBitmap = nullptr;
		if (m_markettype == TYPE_TOTAL)
			pBitmap = CBitmap::FromHandle(m_hIBitmap);
		else if (m_markettype == TYPE_KRX)
			pBitmap = CBitmap::FromHandle(m_hKBitmap);
		else
			pBitmap = CBitmap::FromHandle(m_hNBitmap);

		BITMAP stBitmapInfo;
		pBitmap->GetBitmap(&stBitmapInfo);

		memDC.CreateCompatibleDC(&dc);
		if (memDC.m_hDC != NULL)
			pOldBitmap = (CBitmap*)memDC.SelectObject(pBitmap);

		dc.StretchBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, BitmapSize(m_hIBitmap).cx, BitmapSize(m_hIBitmap).cy, SRCCOPY);
		if (pOldBitmap)
			memDC.SelectObject(pOldBitmap);
		memDC.DeleteDC();
	}
}

void CMaketPickerWnd::Init()
{
	m_sRoot ;
	m_markettype = TYPE_KRX;
	m_strMarket = "KRX";
	SetImages();
	Invalidate();
}

CBitmap* CMaketPickerWnd::getBitmap(CString path)
{
	return (CBitmap*)m_pWizard->SendMessage(WM_USER, getBITMAP, (LPARAM)path.GetString());
}

COLORREF CMaketPickerWnd::GetIndexColor(int index)
{
	return m_pWizard->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}


void CMaketPickerWnd::SetImages()
{
	CString sImageDir = "";
	sImageDir.Format("%s\\image\\", m_sRoot);

	//HBITMAP		m_hIBitmap, m_hKBitmap, m_hNBitmap;
	CFileFind	finder;
	m_sImagePath.MakeLower();

	if (m_sizetype == TYPE_NORMAL)
		m_sImagePath = sImageDir + "\\Market_Total.bmp";
	else
		m_sImagePath = sImageDir + "\\Market_S_Total.bmp";

	if (finder.FindFile(m_sImagePath))
		m_hIBitmap = getBitmap(m_sImagePath)->operator HBITMAP();
	else
		m_hIBitmap = NULL;

	if (m_sizetype == TYPE_NORMAL)
		m_sImagePath = sImageDir + "\\Market_KRX.bmp";
	else
		m_sImagePath = sImageDir + "\\Market_S_KRX.bmp";

	if (finder.FindFile(m_sImagePath))
		m_hKBitmap = getBitmap(m_sImagePath)->operator HBITMAP();
	else
		m_hKBitmap = m_hIBitmap;

	if (m_sizetype == TYPE_NORMAL)
		m_sImagePath = sImageDir + "\\Market_NXT.bmp";
	else
		m_sImagePath = sImageDir + "\\Market_S_NXT.bmp";

	if (finder.FindFile(m_sImagePath))
		m_hNBitmap = getBitmap(m_sImagePath)->operator HBITMAP();
	else
		m_hNBitmap = m_hIBitmap;
}

CSize CMaketPickerWnd::BitmapSize(HBITMAP hBitmap)
{
	if (!hBitmap)
		return CSize(0, 0);

	BITMAP BM;
	GetObject(hBitmap, sizeof(BITMAP), &BM);
	return CSize((int)BM.bmWidth, (int)BM.bmHeight);
}

void CMaketPickerWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_markettype == TYPE_TOTAL)
	{
		m_markettype = TYPE_KRX;
		m_strMarket = "KRX";
	}
	else if (m_markettype == TYPE_KRX)
	{
		m_markettype = TYPE_NXT;
		m_strMarket = "NXT";
	}
	else if (m_markettype == TYPE_NXT)
	{
		m_markettype = TYPE_TOTAL;
		m_strMarket = "통합";
	}
	((CControlWnd*)GetParent())->m_sMarketType = m_strMarket;
	((CControlWnd*)GetParent())->SendEventDLL(evOnDblClk);
	//m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(key, evOnClick)), (LPARAM)m_Param.name.GetString());
	//if (m_mapKey > 0)
	//{
	//	CString sVal;
	//	sVal.Format("%d\t%s", m_mapKey, m_strMarket);
	//	AfxGetMainWnd()->SendMessage(WM_USER, 0x14, (LPARAM)(LPCSTR)sVal);
	//}

    //CString strTrigger{};
	//strTrigger.Format("%s\t%s\t%s\t%s", m_Param.name, DF_TRIGGER_GUBN, m_sID, m_strMarket);

	/*m_slog.Format("       [MARKETPICKER][TRIGGER]      ");
	OutputDebugString(m_slog);
	m_slog.Format("[MARKETPICKER][TRIGGER][%s]<%d> strTrigger s= [%s] ", __FUNCTION__, __LINE__, strTrigger);
	OutputDebugString(m_slog);*/


	//m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, triggerCC), (LPARAM)strTrigger.operator LPCTSTR());


	//if (m_pContainerWnd && m_bInDLL)
	//	m_pContainerWnd->SendMessage(WM_USER + 1, MAKEWPARAM(100, 0), (LPARAM)strTrigger.GetString());

	//m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnClick)), (LPARAM)m_Param.name.GetString());

	Invalidate();
	CWnd::OnLButtonUp(nFlags, point);
}

void CMaketPickerWnd::SetMarketType(int itype)
{
	if (itype == TYPE_TOTAL)
	{
		m_markettype = TYPE_TOTAL;
		m_strMarket = "통합";
	}
	else if (itype == TYPE_KRX)
	{
		m_markettype = TYPE_KRX;
		m_strMarket = "KRX";
	}
	else if (itype == TYPE_NXT)
	{
		m_markettype = TYPE_NXT;
		m_strMarket = "NXT";
	}
	Invalidate();
}

void CMaketPickerWnd::SetMarketType(CString stype)
{
	if (stype == "KRX")
	{
		m_markettype = TYPE_KRX;
		m_strMarket = "KRX";
	}
	else if (stype == "NXT")
	{
		m_markettype = TYPE_NXT;
		m_strMarket = "NXT";
	}
	else if (stype == "통합")
	{
		m_markettype = TYPE_TOTAL;
		m_strMarket = "통합";
	}
	Invalidate();
}