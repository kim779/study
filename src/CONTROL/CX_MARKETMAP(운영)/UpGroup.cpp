// UpGroup.cpp : implementation file
//

#include "stdafx.h"
#include "CX_MARKETMAP.h"
#include "UpGroup.h"
#include "../../h/memDC.h"


#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUpGroup
#define TM_INVALIDATE	1000

#define RGB_DEFAULT RGB(200, 200, 200)   //기관 외국인 개인 모두 값이 0 일때 

CUpGroup::CUpGroup()
{
	m_parent = nullptr;
	m_nBackColor = RGB_DEFAULT;

	m_nSumGG = 0.0f;
	m_nSumFg = 0.0f;
	m_nSumPs = 0.0f;
	m_nSumSgTt = 0.0f;
	m_nSumQnt = 0.0f;
	m_nSumAmt = 0.0f;
	m_nSumMny = 0.0f;
	m_nSumAmtFore = 0.0f;
	m_nSumAmtGigb = 0.0f;
	m_nSumMnFore = 0.0f;
	m_nSumMnGigb = 0.0f;

	m_bSelected = false;
	m_nSelected = -1;

	m_ToolTip = nullptr;
	m_mouseIN = false;

	m_nViewType = 0;
	m_nScale = 0;
	m_nStandard = 0;
}

CUpGroup::~CUpGroup()
{
}


BEGIN_MESSAGE_MAP(CUpGroup, CWnd)
	//{{AFX_MSG_MAP(CUpGroup)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

void CUpGroup::SetRect(CRect rect)
{
	m_rect = rect;
}

/////////////////////////////////////////////////////////////////////////////
// CUpGroup message handlers

void CUpGroup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	xxx::CMemDC memDC(&dc);
	CRect	rc, rect;
	std::shared_ptr<CUpGroup> item = nullptr;

	GetClientRect(&rc);
	draw_rect(&memDC, rc);

	if (m_nViewType == 0)
	{
		for (int ii = 0; ii < m_arrChild.GetSize();ii++)
		{
			item = m_arrChild.GetAt(ii);
			rect = item->GetRect();
			if (m_bSelected)
			{
				draw_rect(&memDC,rect);
				rect.OffsetRect(1,1);
			}
			draw_fillRect(&memDC,rect,item->GetBackColor(),item->GetName());
			if (!m_bSelected)
			{
				if (m_nSelected == ii)
					draw_selected(&memDC,item->GetRect());
			}
		}
	}
	else
	{
		rc.OffsetRect(1,1);
		if (m_bSelected)
		{
			for (int ii = 0; ii < m_arrChild.GetSize();ii++)
			{
				item = m_arrChild.GetAt(ii);
				rect = item->GetRect();

				draw_rect(&memDC,rect);
				rect.OffsetRect(1,1);
				draw_fillRect(&memDC,rect,item->GetBackColor(),item->GetName());
				if (!m_bSelected)
				{
					if (m_nSelected == ii)
						draw_selected(&memDC,item->GetRect());
				}
			}
		}
		else
		{
			draw_fillRect(&memDC,rc,m_nBackColor);
		}
	}

	if (!m_bSelected)
		draw_text(&memDC);
	
	// Do not call CWnd::OnPaint() for painting messages
}

void CUpGroup::draw_fillRect(CDC *pDC, CRect rc,COLORREF cl, CString str)
{
	pDC->FillSolidRect(rc, cl);

	if (m_bSelected)
	{
		CFont*	font = pDC->SelectObject(&m_font);
		const	COLORREF color = pDC->SetTextColor(RGB(0,0,0));
		
		pDC->DrawText(str, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		pDC->SetTextColor(color);
		pDC->SelectObject(font);
	}
}

void CUpGroup::draw_rect(CDC *pDC, CRect iRc)
{
	CPen	cPen(PS_SOLID, 1, RGB(128,128,128));
	CPen*	sPen = pDC->SelectObject(&cPen);

// 	pDC->MoveTo(iRc.left - 2, iRc.top);
// 	pDC->LineTo(iRc.left - 2 + m_buttonW - 4, iRc.top);
// 	pDC->MoveTo(iRc.left - 2  + m_buttonW - 4, iRc.top);
// 	pDC->LineTo(iRc.left - 2  + m_buttonW - 4, iRc.top + m_buttonH - 7);
// 	pDC->MoveTo(iRc.left - 2  + m_buttonW - 4, iRc.top + m_buttonH - 7);
// 	pDC->LineTo(iRc.left - 2 ,iRc.top + m_buttonH -7);
// 	pDC->MoveTo(iRc.left - 2 ,iRc.top + m_buttonH - 7);
// 	pDC->LineTo(iRc.left - 2 , iRc.top);
	POINT pt;
	pt.x = 0;
	pt.y = 0;

	pDC->RoundRect(iRc, pt);
	pDC->SelectObject(sPen);
	cPen.DeleteObject();
}

void CUpGroup::draw_selected(CDC *pDC, CRect rc)
{
	CPen	cPen(PS_SOLID, 2, RGB(237,28,36));
	CPen*	sPen = pDC->SelectObject(&cPen);
// 	pDC->MoveTo(iRc.left - 2, iRc.top);
// 	pDC->LineTo(iRc.left - 2 + m_buttonW - 4, iRc.top);
// 	pDC->MoveTo(iRc.left - 2  + m_buttonW - 4, iRc.top);
// 	pDC->LineTo(iRc.left - 2  + m_buttonW - 4, iRc.top + m_buttonH - 7);
// 	pDC->MoveTo(iRc.left - 2  + m_buttonW - 4, iRc.top + m_buttonH - 7);
// 	pDC->LineTo(iRc.left - 2 ,iRc.top + m_buttonH -7);
// 	pDC->MoveTo(iRc.left - 2 ,iRc.top + m_buttonH - 7);
// 	pDC->LineTo(iRc.left - 2 , iRc.top);
	POINT pt;
	pt.x = 0;
	pt.y = 0;
	
	pDC->DrawEdge(rc, EDGE_RAISED, BF_RECT);
	pDC->SelectObject(sPen);
	cPen.DeleteObject();
}

void CUpGroup::draw_text(CDC* pDC /*= NULL*/)
{
	CRect	rect;
	CClientDC	dc(this);

	if (!pDC)
		pDC = &dc;
	GetClientRect(rect);
	
	CFont*	font = pDC->SelectObject(&m_font);
	const	COLORREF color = pDC->SetTextColor(RGB(0,0,0));

	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(m_strUpName, rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(color);
	pDC->SelectObject(font);
}

int CUpGroup::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_font.CreatePointFont(90, "굴림");
	
	InitToolTip();

	return 0;
}

void CUpGroup::updateRectByRate()
{
	CRect	rect;
	
	//GetClientRect(rect);
	rect.top = 0;
	rect.left = 0;
	rect.bottom = rect.top + m_rect.Height();
	rect.right = rect.left + m_rect.Width();
	rect.OffsetRect(1,1);

	Quick_Sort(m_arrChild, m_arrChild.GetSize(), m_nStandard);  //0 시가총액 1 거래량 2 거래대금
	updateRectByRate(m_arrChild, rect);
}

void CUpGroup::updateRectByRate(CArray < std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>>& arrInfo,CRect rect)
{
	if (arrInfo.GetSize() < 1)
		return;
	
	if (arrInfo.GetSize() == 1)
	{
		arrInfo.GetAt(0)->SetRect(rect);
		return;
	}
	
	double	totalData = 0.0f;
	for (int ii = 0; ii < arrInfo.GetSize(); ii++)
	{
		totalData += arrInfo.GetAt(ii)->GetDetailData(m_nStandard);
	}

	std::shared_ptr<CUpGroup> info = nullptr;
	double upData = 0.0f;
	double downData = 0.0f;
	CArray < std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>>  arrInfoup;
	CArray < std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>>  arrInfodown;

	if (arrInfo.GetSize() == 2)
	{
		info = arrInfo.GetAt(0);
		upData += info->GetDetailData(m_nStandard);
		arrInfoup.Add(info);
		
		info = arrInfo.GetAt(1);
		downData += info->GetDetailData(m_nStandard);
		arrInfodown.Add(info);
	}
	else
	{
		for (int ii = 0; ii < arrInfo.GetSize();ii++)
		{
			info = arrInfo.GetAt(ii);
			if ((upData * 2) < totalData)
			{
				upData += info->GetDetailData(m_nStandard);
				arrInfoup.Add(info);
			}
			else
			{
				downData += info->GetDetailData(m_nStandard);
				arrInfodown.Add(info);
			}
		}
	}
	
	CRect	upRect,downRect;
	
	CopyRect(upRect,rect);
	CopyRect(downRect,rect);
	
	if (rect.Width() < rect.Height())
	{
		upRect.bottom = upRect.top + (int)(rect.Height() * (upData/totalData));
		downRect.top = upRect.bottom;
		downRect.bottom = rect.bottom;
	}
	else
	{
		upRect.right = upRect.left + (int)(rect.Width() * (upData/totalData));
		downRect.left = upRect.right;
		downRect.right = rect.right;
	}
	
	if (upData == 0)
	{
		updateRect(arrInfoup,upRect);
	}
	else
	{
		updateRectByRate(arrInfoup, upRect);
	}

	if (downData == 0)
	{
		updateRect(arrInfodown,downRect);
	}
	else
	{
		updateRectByRate(arrInfodown, downRect);
	}
}

void CUpGroup::updateRect(CArray < std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>>& arrInfo,CRect rect)
{
	if (arrInfo.GetSize() < 1)
		return;
	
	if (arrInfo.GetSize() == 1)
	{
		arrInfo.GetAt(0)->SetRect(rect);
		return;
	}
	
	const	double	totalCnt = arrInfo.GetSize();
	std::shared_ptr<CUpGroup> info = nullptr;
	double upData = 0.0f;
	double downData = 0.0f;
	CArray < std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>> arrInfoup;
	CArray < std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>> arrInfodown;
	
	if (arrInfo.GetSize() == 2)
	{
		info = arrInfo.GetAt(0);
		upData += 1;
		arrInfoup.Add(info);
		
		info = arrInfo.GetAt(1);
		downData += 1;
		arrInfodown.Add(info);
	}
	else
	{
		for (int ii = 0; ii < arrInfo.GetSize();ii++)
		{
			info = arrInfo.GetAt(ii);
			if ((upData*2) < totalCnt)
			{
				upData += 1;
				arrInfoup.Add(info);
			}
			else
			{
				downData += 1;
				arrInfodown.Add(info);
			}
		}
	}
	
	CRect	upRect,downRect;
	
	CopyRect(upRect,rect);
	CopyRect(downRect,rect);
	
	if (rect.Width() < rect.Height())
	{
		upRect.bottom = upRect.top + (int)(rect.Height() * (upData/totalCnt));
		
		downRect.top = upRect.bottom;
		downRect.bottom = rect.bottom;
	}
	else
	{
		upRect.right = upRect.left + (int)(rect.Width() * (upData/totalCnt));
		
		downRect.left = upRect.right;
		downRect.right = rect.right;
	}
	
	updateRect(arrInfoup, upRect);
	updateRect(arrInfodown, downRect);
}

void CUpGroup::updateColorByRate(int nUpColor, int nDownColor, int nEqualColor, double fMaxRate)
{
	double	fRate = GetRate();
	int	nColorR = 0;
	int	nColorG = 0;
	int	nColorB = 0;
	int	nOriginColor = 0;

	if (fRate == 0)
	{
		SetBackColor(nEqualColor);
	}
	else if (fRate >= 6)
	{
		SetBackColor(nUpColor);
	}
	else if (fRate <= -6)
	{
		SetBackColor(nDownColor);
	}
	else
	{
		if (fRate < 0)
		{
			fRate = fRate * -1.0f;
			nOriginColor = nUpColor;
		}
		else
		{
			nOriginColor = nDownColor;
		}

		nColorR = createColorR(nEqualColor,nOriginColor,fMaxRate,fRate);
		nColorG = createColorG(nEqualColor,nOriginColor,fMaxRate,fRate);
		nColorB = createColorB(nEqualColor,nOriginColor,fMaxRate,fRate);
 
		SetBackColor(RGB(nColorR,nColorG,nColorB));
	}

// 	for (int ii = 0; ii < m_arrChild.GetSize();ii++)
// 	{
// 		m_arrChild.GetAt(ii)->updateColorByRate(nUpColor,nDownColor,nEqualColor,fMaxRate);
// 	}
}

int CUpGroup::createColorB(int nEqualColor,int nOriginColor,double fMaxRate,double fRate)
{
	int	nColorB = 0;
	const	int	nEqualBlue = nEqualColor & 0xFF;
	const	int	nOriginBlue = nOriginColor & 0xFF;

	if (nEqualBlue < nOriginBlue)
	{
		nColorB = nOriginBlue;
	}
	else
	{
		nColorB = nEqualBlue;
	}

	nColorB = (int)(nColorB - (abs(nOriginBlue-nEqualBlue)*fRate/fMaxRate));

	return nColorB;
}

int CUpGroup::createColorG(int nEqualColor,int nOriginColor,double fMaxRate,double fRate)
{
	int	nColorG = 0;
	const	int	nEqualGreen = (nEqualColor>>8)&0xFF;
	const	int	nOriginGreen = (nOriginColor>>8)&0xFF;

	if (nEqualGreen < nOriginGreen)
	{
		nColorG = nOriginGreen;
	}
	else
	{
		nColorG = nEqualGreen;
	}

	nColorG = (int)(nColorG - (abs(nOriginGreen-nEqualGreen)*fRate/fMaxRate));

	return nColorG;
}

int CUpGroup::createColorR(int nEqualColor,int nOriginColor,double fMaxRate,double fRate)
{
	int	nColorR = 0;
	const	int	nEqualRed = (nEqualColor>>16)&0xFF;
	const	int	nOriginRed = (nOriginColor>>16)&0xFF;
	
	if (nEqualRed < nOriginRed)
	{
		nColorR = nOriginRed;
	}
	else
	{
		nColorR = nEqualRed;
	}
	
	nColorR = (int)(nColorR - (abs(nOriginRed-nEqualRed)*fRate/fMaxRate));
	
	return nColorR;
}

double CUpGroup::GetRate()
{
	double	dRateData = 0;
	CString strRate = m_strRate;

	if (!strRate.IsEmpty())
	{
		dRateData = atof(strRate);
	}

	for (int ii = 0 ; ii < m_arrChild.GetSize();ii++)
	{
		dRateData += m_arrChild.GetAt(ii)->GetRate();
	}

	return dRateData;
}

void CUpGroup::SetBackColor(COLORREF nColor)
{
	m_nBackColor = nColor;
}

void CUpGroup::updateRect()
{
	CRect	rect;

	//GetClientRect(rect);
	rect.top = 0;
	rect.left = 0;
	rect.bottom = rect.top + m_rect.Height();
	rect.right = rect.left + m_rect.Width();

	rect.OffsetRect(1,1);

	updateRect(m_arrChild,rect);
}

void CUpGroup::PutData(std::shared_ptr<CUpGroup> data)
{
	m_arrChild.Add(data);
}

void CUpGroup::updateChildColorByRate(int nUpColor, int nDownColor, int nEqualColor, double fMaxRate)
{
	for (int ii = 0; ii < m_arrChild.GetSize();ii++)
	{
		m_arrChild.GetAt(ii)->updateColorByRate(nUpColor,nDownColor,nEqualColor,fMaxRate);
	}
}

void CUpGroup::updateColorBySum(COLORREF ggColor, COLORREF fgColor, COLORREF psColor)
{
	COLORREF backCl = 0;
	int	nTmp = 0;
	const	int	nGG = atoi(m_strGign);   //기관 거래량
	const	int	nFg = atoi(m_strForg);	 //외국인거래량
	const	int	nPs = atoi(m_strPers);	//개인거래량

	if (nGG >= nFg)
	{
		backCl = ggColor;
		nTmp = nGG;
	}
	else
	{
		backCl = fgColor;
		nTmp = nFg;
	}

	if (nTmp < nPs)
	{
		backCl = psColor;
	}

	m_nBackColor = backCl;

	if (nGG <= 0 && nFg <= 0 && nPs <= 0)
		m_nBackColor = RGB_DEFAULT; 
}

void CUpGroup::updateChildColorBySum(COLORREF ggColor, COLORREF fgColor, COLORREF psColor)
{
	COLORREF backCl = 0;
	double	nTmp = 0;
	
	if (m_nSumGG >= m_nSumFg)
	{
		backCl = ggColor;
		nTmp = m_nSumGG;
	}
	else
	{
		backCl = fgColor;
		nTmp = m_nSumFg;
	}
	
	if (nTmp < m_nSumPs)
	{
		backCl = psColor;
	}
	
	m_nBackColor = backCl;

	if ((int)m_nSumFg <= 0 && (int)m_nSumGG <= 0 && (int)m_nSumPs <= 0)
		m_nBackColor = RGB_DEFAULT;  //15202246


	for (int ii = 0; ii < m_arrChild.GetSize();ii++)
	{
		m_arrChild.GetAt(ii)->updateColorBySum(ggColor,fgColor,psColor);
	}
}

void CUpGroup::Reset()
{
	for (int ii = 0; ii < m_arrChild.GetSize();ii++)
	{
		m_arrChild.GetAt(ii).reset();
	}

	m_arrChild.RemoveAll();
}


void CUpGroup::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default


	CWnd::OnLButtonUp(nFlags, point);
}

void CUpGroup::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CString sTrg;

	sTrg.Format("1301\t%s",m_strcode);
	m_parent->SendMessage(WD_TRIGGERCC, 0, (LPARAM)sTrg.GetString());

	CWnd::OnLButtonDown(nFlags, point);
}

void CUpGroup::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bSelected && nFlags == 0)
	{  //업종지도 상태에서 더블클릭
		
		m_bSelected = true;
		MoveWindow(m_parentRect,TRUE);

		CopyRect(m_rect,m_parentRect);

		if (m_nScale == 0)
			updateRect();
		else
			updateRectByRate();

		m_parent->SendMessage(WD_UPDBCLICK,1,(LPARAM)(LPCSTR)m_strUp);
		
	}
	else
	{   //종목 지도 상태에서 더블클릭
		CRect	bRc;
		std::shared_ptr<CUpGroup> upGroup = nullptr;

		for (int ii = 0; ii < m_arrChild.GetSize();ii++)
		{
			upGroup = m_arrChild.GetAt(ii);
			bRc = upGroup->GetRect();
				
		//	ClientToScreen(bRc);
				
			if (bRc.PtInRect(point))
			{
				CString strCode;
				strCode.Format("%s",upGroup->GetCode());
				m_parent->SendMessage(WD_UPDBCLICK,2,(LPARAM)(LPCSTR)strCode);
				m_parent->PostMessage(WD_UPDBCLICK,0,0);
			}
		}
	}
	CWnd::OnLButtonDblClk(nFlags, point);
}

bool CUpGroup::IsNumber(CString str)
{
	bool	bRet = TRUE;
	const	int	nLen = str.GetLength();
	for (int ii =0; ii < nLen ; ii++)
	{
		if (isdigit(str.GetAt(ii)) == FALSE)
		{
			bRet = FALSE;
			break;
		}
	}
	return bRet;
}

void CUpGroup::SetSumQnt(CString sGign,CString sForg,CString sPers)
{
	m_nSumGG += atof(sGign);
	m_nSumFg += atof(sForg);
	m_nSumPs += atof(sPers);
}

void CUpGroup::OnMouseMove(UINT nFlags, CPoint point) 
{
	TRACKMOUSEEVENT	csTME{};
	CWnd*	wndUnderMouse = nullptr;
	CWnd*	wndActive = this;

	ClientToScreen(&point);
	wndUnderMouse = WindowFromPoint(point);
	if (wndUnderMouse && wndUnderMouse->m_hWnd == m_hWnd && wndActive)
	{
		std::shared_ptr<CUpGroup> upGroup = nullptr;
		CRect	bRc;
		CString str, strtmp; //등락 업종 지도 상태

		if (!m_mouseIN)
		{
			m_mouseIN = true;
			
			csTME.cbSize = sizeof(csTME);
			csTME.dwFlags = TME_LEAVE;
			csTME.hwndTrack = m_hWnd;
			::_TrackMouseEvent(&csTME);
		}

		if (m_nViewType == 0)   //등락
		{
			for (int ii = 0; ii < m_arrChild.GetSize();ii++)
			{
				upGroup = m_arrChild.GetAt(ii);
				bRc = upGroup->GetRect();
				ClientToScreen(bRc);

				if (bRc.PtInRect(point))
				{
					m_strcode = upGroup->GetCode();

					if (m_nSelected != ii)
					{
						m_nSelected = ii;

						if (!m_bSelected)
							SetTimer(TM_INVALIDATE,200,NULL);
					}

					m_ToolTip->SetToolRect(this, 1, upGroup->GetRect());

					CString str, strtmp; //등락 업종 지도 상태
					if (upGroup->GetDiff().Find("+") >= 0)
						strtmp = "▲  " + upGroup->GetDiff().Mid(1);
					else if (upGroup->GetDiff().Find("-") >= 0)
						strtmp = "▼  " + upGroup->GetDiff().Mid(1);
					else
						strtmp = upGroup->GetDiff();

					str.Format(" %s(%s) %s\n 현재가 : %s \n 전일대비 : %s \n 등락율 : %s%% \n 거래량 : %s",
						upGroup->GetName(),upGroup->GetCode(),upGroup->GetUpName(),
						upGroup->GetCurr(), strtmp, upGroup->GetCurRate(), upGroup->GetAmount());
					SetToolTipText(str);
				}
			}
		}
		else		//수급
		{
			if (m_bSelected)
			{
				for (int ii = 0; ii < m_arrChild.GetSize();ii++)
				{
					upGroup = m_arrChild.GetAt(ii);
					bRc = upGroup->GetRect();
					ClientToScreen(bRc);
					
					if (bRc.PtInRect(point))
					{						
						if (m_nSelected != ii)
						{
							m_nSelected = ii;

							if (m_bSelected)
								SetTimer(TM_INVALIDATE,200,NULL);
						}
						
						m_ToolTip->SetToolRect(this, 1, upGroup->GetRect());  //test
							
						//종목명 종목코드 업종 \n 현재가 \n 전일대비 \n 등락률 \n 거래량
						//등락 업종 지도 상태
						if (upGroup->GetDiff().Find("+") >= 0)
							strtmp = "▲  " + upGroup->GetDiff().Mid(1);
						else if (upGroup->GetDiff().Find("-") >= 0)
							strtmp = "▼  " + upGroup->GetDiff().Mid(1);
						else
							strtmp = upGroup->GetDiff();

						str.Format(" %s(%s) %s\n 현재가 : %s \n 전일대비 : %s \n 등락율 : %s%% \n 거래량 : %s",
							upGroup->GetName(),upGroup->GetCode(),upGroup->GetUpName(),
							upGroup->GetCurr(), strtmp, upGroup->GetCurRate() + " %", upGroup->GetAmount());
						SetToolTipText(str);
					}
				}
			}
			else
			{
				/*
				CRect	rc;
				GetClientRect(&rc);

				m_ToolTip->SetToolRect(this, 1, rc);

				CString strSg;

				if (m_nBackColor == RGB(198,247,231))
				{
					strSg = "기관";
				}
				else if (m_nBackColor == RGB(255,206,214))
				{
					strSg = "외국인";
				}
				else
				{
					strSg = "개인"; 
				}
				
				CString str;
				str.Format("%s [기 : %d 외 : %d 개 : %d]",strSg,(int)m_nSumGG,(int)m_nSumFg,(int)m_nSumPs);
				SetToolTipText(str);
				*/
			}
		}
	}
	else
	{
		m_mouseIN = false;
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CUpGroup::SetToolTipText(CString text, BOOL bActivate)
{
	if (text.IsEmpty())
		return;
	
	InitToolTip();
	if (m_ToolTip->GetToolCount() == 0)
		m_ToolTip->AddTool(this, text, CRect(0,0,0,0) , 1);
	
	m_ToolTip->UpdateTipText(text, this, 1);
	m_ToolTip->Activate(bActivate);
}

void CUpGroup::InitToolTip()
{
	if (!m_ToolTip)
	{
		m_ToolTip = std::make_unique<CToolTipCtrl>();
		m_ToolTip->Create(this, TTS_ALWAYSTIP);
		m_ToolTip->SetMaxTipWidth(250);
		m_ToolTip->Activate(TRUE);
	}
}

void CUpGroup::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_ToolTip)
	{
		if (m_ToolTip->m_hWnd)
		{
			MSG	msg{};

			m_ToolTip->RelayEvent(&msg);
			m_ToolTip->Activate(FALSE);
			m_ToolTip->DelTool(this);
		}
		m_ToolTip.reset();
	}
}

LRESULT CUpGroup::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		if (m_ToolTip && m_ToolTip->m_hWnd)
		{
			MSG	msg{};
			POINT	point;
			
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);
			
			msg.hwnd = m_hWnd;
			msg.message = message;
			msg.wParam = wParam;
			msg.lParam = lParam;
			msg.pt = point;
			
			m_ToolTip->RelayEvent(&msg);
		}
		break;
	default:
		break;
	}
	
	return CWnd::WindowProc(message, wParam, lParam);
}

LRESULT CUpGroup::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_mouseIN = false;
	m_nSelected = -1;

	Invalidate();
	return 0;
}

void CUpGroup::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case TM_INVALIDATE:
		KillTimer(TM_INVALIDATE);
		Invalidate();
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

void CUpGroup::SetSumAmt(CString sAmt)
{
	if (IsNumber(sAmt))
		m_nSumAmt += atof(sAmt);
}

void CUpGroup::SetSumSgTotal(CString sSgTt)
{
	if (IsNumber(sSgTt))
		m_nSumSgTt += atof(sSgTt);
}

void CUpGroup::Quick_Sort(CArray < std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>>& arrInfo, int count, int type)
{
	Quick_Recursive(arrInfo, 0, count-1, type);
}

void CUpGroup::Quick_Recursive(CArray < std::shared_ptr<CUpGroup>, std::shared_ptr<CUpGroup>>& arrInfo, int left, int right , int type)
{
	const	int	left_hold = left;
	const	int	right_hold = right;
	std::shared_ptr<CUpGroup> upGroup = arrInfo.GetAt(left);
	double	pivot = upGroup->GetDetailData(type);
	
	while (left < right)
	{
		while ((arrInfo.GetAt(right)->GetDetailData(type) <= pivot) && (left < right))
			right--;
		
		if (left != right)
		{
			arrInfo.SetAt(left,arrInfo.GetAt(right));
		}
		
		while ((arrInfo.GetAt(left)->GetDetailData(type) >= pivot) && (left < right))
			left++;
		
		if (left != right)
		{
			arrInfo.SetAt(right,arrInfo.GetAt(left));
			right--;
		}
	}
	
	arrInfo.SetAt(left,upGroup);
	pivot = left;
	left  = left_hold;
	right = right_hold;
	
	if (left < pivot)	Quick_Recursive(arrInfo,left,(int)pivot-1,type);
	if (right > pivot)	Quick_Recursive(arrInfo,(int)pivot+1,right,type);
}

double CUpGroup::floordata(double dData, int igubn)
{
	double	dResult = 0;

	switch (igubn)
	{
	case 1:    //소수점 첫째 자리 반올림 
		dResult = floor(dData * 10.f + 0.5) / 10.f;
		break;
	case 2:    // 소수점 둘째 자리 반올림 
		dResult = floor(dData * 100.f + 0.5) / 100.f;
		break;
	}
	return dResult;
}

//0:시가총액 1:거래량 2:거래대금 3:외국인매수수량 4:외국인매수대금
double CUpGroup::GetSortData(int nType)
{
	switch (nType)
	{
	case 0:
		return floordata(m_nSumSgTt);
	case 1:
		return floordata(m_nSumAmt);
	case 2:
		return floordata(m_nSumMny);
	case 3:
		return floordata(m_nSumAmt);
	case 4:
		return floordata(m_nSumAmt);
	default:
		break;
	}
	return 0;
}

//0:시가총액 1:거래량 2:거래대금 3:외국인매수수량 4:외국인매수대금
double CUpGroup::GetDetailData(int nType)
{
	switch (nType)
	{
	case 0:
		return floordata(m_nSumSgTt);
	case 1:
		return floordata(m_nSumAmt);
	case 2:
		return floordata(m_nSumMny);
	case 3:
		return floordata(m_nSumMny);  
	case 4:
		return floordata(m_nSumMny);  
	default:
		break;
	}
	return 0;
}

HBRUSH CUpGroup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CUpGroup::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CString sTrg;

	if (!m_bSelected)
	{
		sTrg.Format("20301\t%03s",m_strUp);
		m_parent->SendMessage(WD_TRIGGERCC, 0, (LPARAM)sTrg.GetString());
	}
	else
	{
		std::shared_ptr<CUpGroup> upGroup = nullptr;
		CRect	bRc;

		for (int ii = 0; ii < m_arrChild.GetSize();ii++)
		{
			upGroup = m_arrChild.GetAt(ii);
			bRc = upGroup->GetRect();
			
			//ClientToScreen(bRc);
			if (bRc.PtInRect(point))
			{	
				sTrg.Format("1301\t%s",upGroup->GetCode());
				m_parent->SendMessage(WD_TRIGGERCC, 1, (LPARAM)sTrg.GetString());
			}
		}
	}
	CWnd::OnRButtonUp(nFlags, point);
}

void CUpGroup::SetSumMoney(CString sMny)
{
	if (IsNumber(sMny))
		m_nSumMny += (int)(atof(sMny));
}

void CUpGroup::SetSumAmtFore(CString sFore)  //외국인 순매수량 합산
{
	m_nSumAmtFore += atof(sFore);
}

void CUpGroup::SetSumAmtGign(CString sGign)  //기관 순매수량 합산 
{
	m_nSumAmtGigb += atof(sGign);
}

void CUpGroup::SetSumMnFore(CString sMnFore)   //외국인 순매수대금 합산 
{
	m_nSumMnFore += atof(sMnFore);
}

void CUpGroup::SetSumMnGign(CString sMnGign)    //기관 순매수대금 합산 
{
	m_nSumMnGigb += atof(sMnGign);
}

void CUpGroup::SetPers(CString data) 
{ 
	m_strPers = data;  	//개인 순매수량
}	

CString CUpGroup::GetPers() 
{ 
	return m_strPers;
}

void CUpGroup::SetForg(CString data) 
{ 
	m_strForg = data;  //외국인 순매수량
}		

CString CUpGroup::GetForg() 
{ 
	return m_strForg; 
}

void CUpGroup::SetGign(CString data) 
{ 
	m_strGign = data;   	//기관 순매수량
}	

CString CUpGroup::GetGign() 
{ 
	return m_strGign; 
}

void CUpGroup::SetMnPers(CString data) 
{ 
	m_strMnPers = data;  //개인 거래대금
}		

CString CUpGroup::GetMnPers() 
{ 
	return m_strMnPers;  	//개인 거래대금
}

void CUpGroup::SetMnForg(CString data) 
{ 
	m_strMnForg = data; 
}		

CString CUpGroup::GetMnForg() 
{ 
	return m_strMnForg; 
}

void CUpGroup::SetMnGign(CString data) 
{ 
	m_strMnGign = data; 
}

CString CUpGroup::GetMnGign() 
{ 
	return m_strMnGign; 	//기관 거래대금
}


