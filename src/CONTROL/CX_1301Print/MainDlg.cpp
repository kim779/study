// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CX_1301Print.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FIRSTDATACOUNT 48	//2012.06.21 KSJ 첫번째 페이지에 나올 종목수
#define DATACOUNT 52	//2012.06.21 KSJ 두번째 페이지부터 한페이지에 나올 종목수

/////////////////////////////////////////////////////////////////////////////
// MainDlg dialog


MainDlg::MainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(MainDlg::IDD, pParent)
{
	EnableAutomation();

	m_tokenSize = 0;
	m_nTotalPages = 0;
	m_nNowPage  = 0;
}


void MainDlg::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void MainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MainDlg)
	DDX_Control(pDX, IDC_EDIT1, m_editStatus);
	DDX_Control(pDX, IDCANCEL, m_btnOK);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MainDlg, CDialog)
	//{{AFX_MSG_MAP(MainDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(MainDlg, CDialog)
	//{{AFX_DISPATCH_MAP(MainDlg)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMainDlg to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {B55F180D-37A0-4B94-B87D-14AEFA5A607D}
static const IID IID_IMainDlg =
{ 0xb55f180d, 0x37a0, 0x4b94, { 0xb8, 0x7d, 0x14, 0xae, 0xfa, 0x5a, 0x60, 0x7d } };

BEGIN_INTERFACE_MAP(MainDlg, CDialog)
	INTERFACE_PART(MainDlg, IID_IMainDlg, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MainDlg message handlers

void MainDlg::Run()
{
	LoadData();

	SetTimer(100,100,NULL);	
}

void MainDlg::SetProgressRange(int iMax, int iMin) 
{
	m_Progress.SetRange(iMin, iMax); 
	m_Progress.SetPos(iMin); 
}

void MainDlg::SetProgressPos(int iPos) 
{
	if (iPos < 0)
	{
		int ilower, iupper;
		m_Progress.GetRange(ilower, iupper);
		iPos = iupper;
	}
	else
		iPos += m_Progress.GetPos();

	m_Progress.SetPos(iPos); 
}

void MainDlg::Start()
{
	std::unique_ptr <CPrintDialog> pDlg = std::make_unique<CPrintDialog>(FALSE);

	if (pDlg->DoModal() == IDCANCEL)
	{
		pDlg.reset();
		OnCancel();
		return;
	}

	CPrintInfo pInfo;
	DOCINFO	docinfo{};

	docinfo.cbSize= sizeof(docinfo);
	docinfo.lpszDocName = _T("일별손익 출력");

	pInfo.SetMinPage(1);
	pInfo.SetMaxPage(m_nTotalPages);

	SetProgressRange(m_nTotalPages, 0);

	//pInfo.m_rectDraw.SetRect(0, 0, 500, 800); 
	CDC*	pDC = CDC::FromHandle(pDlg->GetPrinterDC());
	SetPrintAlign(pDC,pDlg->GetPrinterDC());
	if (pDC->StartDoc(&docinfo) < 0)
	{
		//SetPrinterStatus("인쇄를 시작할 수 없습니다.");
		return ;
	}

	for (int ii = 0; ii < m_nTotalPages; ii++)
	{
		SetProgressPos(ii+1);
		pDC->StartPage();

		m_nNowPage = ii + 1;
		DrawStuff(pDC);
		pDC->EndPage();
	}

	pDC->EndDoc();
	pDlg.reset();
	pDC->DeleteDC();

	OnCancel();
}

void MainDlg::SetPrintAlign(CDC *pDC, HDC hdcPrn)
{
	const	int	cxPage = ::GetDeviceCaps (hdcPrn, HORZRES) ;
	const	int	cyPage = ::GetDeviceCaps (hdcPrn, VERTRES) ;

	//pDC->SetMapMode (MM_ANISOTROPIC) ;
	SetMapMode(hdcPrn,MM_ANISOTROPIC);
	//pDC->SetWindowExt ( 2100, 2970) ;
	SetWindowExtEx(hdcPrn,2300,2970,NULL);
	//pDC->SetViewportExt (cxPage, cyPage) ;
	SetViewportExtEx(hdcPrn,cxPage,cyPage,NULL);

	const	int	nMarginX = (int)(50.0 / 2300.0 * (float)cxPage);
	const	int	nMarginY = (int)(150.0 / 2970.0 * (float)cyPage);

	//pDC->SetViewportOrg (nMarginX,  nMarginY) ;
	SetViewportOrgEx(hdcPrn,nMarginX,nMarginY,NULL);
	//pDC->SetTextAlign (TA_BASELINE | TA_CENTER) ;
}

//nAlign = 0 : LEFT,nAlign = 1 : RIGHT,nAlign = 2 : CENTER
void MainDlg::DrawBox(CDC* pDC,int x1,int y1,int x2,int y2,LPCTSTR text,BOOL MULTI,int nBold,BOOL BKMODE,int nAlign)
{
	CFont	font;
	int	nFontSize = 30;

	if (nBold == FW_BOLD)
	{
		nFontSize = 32;
	}

	VERIFY(font.CreateFont(	nFontSize,			// nHeight
				0,				// nWidth
				0,				// nEscapement
				0,				// nOrientation
				nBold,				// nWeight
				FALSE,				// bItalic
				FALSE,				// bUnderline
				0,				// cStrikeOut
				ANSI_CHARSET,			// nCharSet
				OUT_DEFAULT_PRECIS,		// nOutPrecision
				CLIP_DEFAULT_PRECIS,		// nClipPrecision
				DEFAULT_QUALITY,		// nQuality
				DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
				"굴림체"));			// lpszFacename

	pDC->Rectangle(x1,y1,x2,y2);

	CRect	rect(x1+4,y1+1,x2-4,y2-1);
	CFont	*pOldFont = (CFont *)pDC->SelectObject(&font);

	if (BKMODE == TRUE)
	{
		FillSolidBox(pDC,x1,y1,x2-x1,y2-y1,1);
	}

	if (MULTI == TRUE)
	{
		rect.top = rect.top + 2;
		pDC->DrawText(text,-1,rect,DT_CENTER | DT_VCENTER);
	}
	else
	{
		switch (nAlign)
		{
		case 0:	pDC->DrawText(text, -1, rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);	break;
		case 1:	pDC->DrawText(text, -1, rect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);	break;
		default:pDC->DrawText(text, -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);	break;
		}
	}

	pDC->SelectObject(pOldFont);
	DeleteObject(&font);
}

void MainDlg::DrawLine(CDC* pDC,POINT sPoint,POINT ePoint)
{
	pDC->MoveTo(sPoint);
	pDC->LineTo(ePoint);
}

void MainDlg::DrawTitle(CDC* pDC)
{
	CFont	font;

	VERIFY(font.CreateFont(	70,				// nHeight
				0,				// nWidth
				0,				// nEscapement
				0,				// nOrientation
				FW_BOLD,			// nWeight
				FALSE,				// bItalic
				FALSE,				// bUnderline
				0,				// cStrikeOut
				ANSI_CHARSET,			// nCharSet
				OUT_DEFAULT_PRECIS,		// nOutPrecision
				CLIP_DEFAULT_PRECIS,		// nClipPrecision
				DEFAULT_QUALITY,		// nQuality
				DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
				"굴림체"));			// lpszFacename

	CFont	*pOldFont = (CFont *)pDC->SelectObject(&font);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(BLACKTEXT);
	
	CRect rect(0, -50, 2230, 60);
		
	CString text = "[1301] 일별손익";
	
	pDC->DrawText(text,-1,rect,DT_CENTER | DT_TOP);
	
	pDC->SelectObject(pOldFont);
	DeleteObject(&font);
}

// 2012.04.23 KSJ 2차 수정본
void MainDlg::DrawTopBox(CDC* pDC)
{
	CString line;
	CStringArray ar;
	int	nTop = 0, nBottom = 0;
	CPen	pen(PS_SOLID, 2, RGB(0,0,0));
	CPen*	pOldPen = pDC->SelectObject(&pen);
	
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(BLACKTEXT);

	DrawTitle(pDC);

	line = m_arData.GetAt(0);
	SplitString(line,"\t",ar);

	//상단의 계좌정보
	nTop = 70;
	nBottom = 120;
	DrawBox(pDC,0,nTop,160,nBottom,ar.GetAt(0),FALSE,FW_NORMAL,TRUE,2);
// 	FillSolidBox(pDC,0,0,316,50,0);		// 계좌번호 

	ar[1].Insert(3,"-");
	ar[1].Insert(6,"-");
	DrawBox(pDC,160,nTop,400,nBottom,ar.GetAt(1),FALSE,FW_NORMAL,FALSE,1);
// 	FillSolidBox(pDC,316,0,316,50,0);		// 계좌번호 값

	DrawBox(pDC,400,nTop,550,nBottom,ar.GetAt(2),FALSE,FW_NORMAL,TRUE,2);
// 	FillSolidBox(pDC,632,0,316,50,0);	// 계좌명

	DrawBox(pDC,550,nTop,750,nBottom,ar.GetAt(3),FALSE,FW_NORMAL,FALSE,1);
	// 	FillSolidBox(pDC,632,0,316,50,0);	// 계좌명 값

	DrawBox(pDC,750,nTop,910,nBottom,ar.GetAt(4),FALSE,FW_NORMAL,TRUE,2);
	// 	FillSolidBox(pDC,948,0,316,50,0);	// 조회구분
	
	ar[5].Replace("당일", "");
	DrawBox(pDC,910,nTop,1400,nBottom,ar.GetAt(5),FALSE,FW_NORMAL,FALSE,1);
	// 	FillSolidBox(pDC,1264,0,316,50,0);	// 조회구분 값

	DrawBox(pDC,1400,nTop,1560,nBottom,ar.GetAt(6),FALSE,FW_NORMAL,TRUE,2);
	// 	FillSolidBox(pDC,948,0,316,50,0);	// 조회일자

	ar[7].Insert(4,"/");
	ar[7].Insert(7,"/");
	DrawBox(pDC,1560,nTop,1740,nBottom,ar.GetAt(7),FALSE,FW_NORMAL,FALSE,1);
	// 	FillSolidBox(pDC,1264,0,316,50,0);	// 조회일자 값

	DrawBox(pDC,1740,nTop,1900,nBottom,ar.GetAt(8),FALSE,FW_NORMAL,TRUE,2);
	// 	FillSolidBox(pDC,948,0,316,50,0);	// 출력일자
	
	ar[9].Replace("-", "/");
	DrawBox(pDC,1900,nTop,2230,nBottom,ar.GetAt(9),FALSE,FW_NORMAL,FALSE,1);
	// 	FillSolidBox(pDC,1264,0,316,50,0);	// 출력일자 값
	
	//상단의 계좌정보 끝

	//사이에 30만큼 띄움

	//상단 밑의 첫번째 줄
	ar.RemoveAll();
	line = m_arData.GetAt(1);
	SplitString(line,"\t",ar);

	nTop = 150;
	nBottom = 200;
	DrawBox(pDC, 0,    nTop, 250,  nBottom, ar.GetAt(0), FALSE, FW_NORMAL, TRUE,  2);	//매도
	DrawBox(pDC, 250,  nTop, 740,  nBottom, ar.GetAt(1), FALSE, FW_NORMAL, FALSE, 1);	//매도 값
	DrawBox(pDC, 740,  nTop, 990,  nBottom, ar.GetAt(2), FALSE, FW_NORMAL, TRUE,  2);	//거래비용
	DrawBox(pDC, 990,  nTop, 1480, nBottom, ar.GetAt(3), FALSE, FW_NORMAL, FALSE, 1);	//거래비용 값
	DrawBox(pDC, 1480, nTop, 1730, nBottom, ar.GetAt(4), FALSE, FW_NORMAL, TRUE,  2);	//손익금액
	DrawBox(pDC, 1730, nTop, 2230, nBottom, ar.GetAt(5), FALSE, FW_NORMAL, FALSE, 1);	//손익금액 값
	//상단 밑의 첫번째 줄 끝
	

	//상단 밑의 두번째 줄
	ar.RemoveAll();
	line = m_arData.GetAt(2);
	SplitString(line,"\t",ar);

	nTop = 200;
	nBottom = 250;
	DrawBox(pDC, 0,    nTop, 250,  nBottom, ar.GetAt(0), FALSE, FW_NORMAL, TRUE,  2);	//매수
	DrawBox(pDC, 250,  nTop, 740,  nBottom, ar.GetAt(1), FALSE, FW_NORMAL, FALSE, 1);	//매수 값
	DrawBox(pDC, 740,  nTop, 990,  nBottom, ar.GetAt(2), FALSE, FW_NORMAL, TRUE,  2);	//정산금액
	DrawBox(pDC, 990,  nTop, 1480, nBottom, ar.GetAt(3), FALSE, FW_NORMAL, FALSE, 1);	//정산금액 값
	DrawBox(pDC, 1480, nTop, 1730, nBottom, ar.GetAt(4), FALSE, FW_NORMAL, TRUE,  2);	//수익률
	DrawBox(pDC, 1730, nTop, 2230, nBottom, ar.GetAt(5), FALSE, FW_NORMAL, FALSE, 1);	//수익률 값
	//상단 밑의 두번째 줄 끝

	pDC->SelectObject(pOldPen);
	DeleteObject(&pen);
}
// KSJ

void MainDlg::DrawGrid(CDC* pDC)
{
	//CString topHeader[] = {"구분","종목번호","잔고수량","D+2일 잔고","매입평균가","매입금액","손익률","금일매수체결","금일매도체결","대출일","종목명","주문가능수량","신용금액","현재가","평가금액","매도손익금액","매수가격","매도가격"};
	if (m_nNowPage == 1)
	{
		DrawGridHeader(pDC);
		if (m_arData.GetSize() > 3)
			DrawGridData(pDC);
	}
	else
	{
		DrawGridHeader(pDC, false);	//2012.06.21 KSJ 두번째 페이지부터 헤더 출력하도록 추가함.
		DrawGridData(pDC);
	}
}

void MainDlg::DrawGridData(CDC* pDC)
{
	int	y = 0;
	CString tmp;
	CStringArray ar;

	if (m_arData.GetSize() < FIRSTDATACOUNT)
	{
		y = 380;
		for (int ii = 3; ii < m_arData.GetSize(); ii++)
		{
			tmp = m_arData.GetAt(ii);
			ar.RemoveAll();
			SplitString(tmp,":",ar);
			if (ar.GetSize() == 10)
				DrawGridLine(pDC,ar,y);

			y += 50;
		}
	}
	else
	{
		if (m_nNowPage == 1)
		{
			y = 380;

			for (int ii = 3; ii < FIRSTDATACOUNT; ii++)
			{
				tmp = m_arData.GetAt(ii);
				ar.RemoveAll();
				SplitString(tmp,":",ar);
				if (ar.GetSize() == 10)
					DrawGridLine(pDC,ar,y);
				
				y += 50;
			}
		}
		else
		{
			y = 100;

			const	int	nStart = FIRSTDATACOUNT + (m_nNowPage - 2) * DATACOUNT;

			for (int ii = nStart; ii < nStart + DATACOUNT; ii++)
			{
				if (ii > m_arData.GetSize() -1)
				{
					break;
				}

				tmp = m_arData.GetAt(ii);
				ar.RemoveAll();
				SplitString(tmp,":",ar);
				if (ar.GetSize() == 10)
					DrawGridLine(pDC,ar,y);
				
				y += 50;
			}
		}
	}

	if (m_nNowPage == m_nTotalPages)
	{
// 		if (m_arData.GetSize() > 30)
			y = y - 50;

		DrawBottomData(pDC,y);
	}

	DrawPageNumber(pDC);
}

void MainDlg::DrawPageNumber(CDC* pDC)
{
	CRect	rect;
	CFont	font;

	VERIFY(font.CreateFont(	30,				// nHeight
				0,				// nWidth
				0,				// nEscapement
				0,				// nOrientation
				FW_NORMAL,			// nWeight
				FALSE,				// bItalic
				FALSE,				// bUnderline
				0,				// cStrikeOut
				ANSI_CHARSET,			// nCharSet
				OUT_DEFAULT_PRECIS,		// nOutPrecision
				CLIP_DEFAULT_PRECIS,		// nClipPrecision
				DEFAULT_QUALITY,		// nQuality
				DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
				"굴림체"));			// lpszFacename
	
	CFont	*pOldFont = (CFont *)pDC->SelectObject(&font);
	
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(BLACKTEXT);
	
	rect.top = 2770;
	rect.left = 1800;
	rect.bottom = rect.top + 40;
	rect.right = 2100;

	CString strPage;

	strPage.Format("%d / %d",m_nNowPage,m_nTotalPages);
	
	pDC->DrawText(strPage,-1,rect,DT_RIGHT | DT_TOP);

	rect.left = 0;
	rect.right = 500;

	pDC->DrawText("IBK 투자증권 일별손익",-1,rect, DT_LEFT | DT_TOP);
	
	pDC->SelectObject(pOldFont);
	DeleteObject(&font);
}

void MainDlg::DrawBottomData(CDC* pDC,int y)
{
	CRect rect;
	CFont	font;

	y = y + 40;
	VERIFY(font.CreateFont(	30,				// nHeight
				0,				// nWidth
				0,				// nEscapement
				0,				// nOrientation
				FW_NORMAL,			// nWeight
				FALSE,				// bItalic
				FALSE,				// bUnderline
				0,				// cStrikeOut
				ANSI_CHARSET,			// nCharSet
				OUT_DEFAULT_PRECIS,		// nOutPrecision
				CLIP_DEFAULT_PRECIS,		// nClipPrecision
				DEFAULT_QUALITY,		// nQuality
				DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
				"굴림체"));			// lpszFacename

	CFont*	pOldFont = (CFont*)pDC->SelectObject(&font);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(BLACKTEXT);

	rect.top = y + 50;
	rect.left = 0;
	rect.bottom = rect.top + 50;
	rect.right = 2100;

	CString text = "※본 화면 내용은 참고자료로만 활용하시고 어떠한 경우에도 법적 책임소재에 대한 증빙자료로는 사용할 수 없음을 알려드립니다.";

	pDC->DrawText(text,-1,rect,DT_LEFT | DT_TOP);

	pDC->SelectObject(pOldFont);
	DeleteObject(&font);
}

//2012.06.21 KSJ 두번째 페이지부터 헤더출력하도록 수정
void MainDlg::DrawGridHeader(CDC* pDC , bool bFirst)
{
	int	nTop = 280;
	int	nBottom = 330;

	nTop = 280;
	nBottom = 330;

	//2012.06.21 KSJ 두번째 페이지부터 헤더출력하도록 수정
	if (!bFirst)
	{
		nTop = 0;
		nBottom = 50;
	}
	//KSJ

	//nAlign = 0 : LEFT,nAlign = 1 : RIGHT,nAlign = 2 : CENTER
	const	int	nPosition[] = {250, 420, 670, 840, 1010, 1260, 1430, 1730, 2030, 2230};
	int	nNextPos = 0, nPrePos = 0;

	DrawBox(pDC,0,nTop,nPosition[nNextPos++],nBottom + (nBottom - nTop),"종목명",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos],nTop,nPosition[3],nBottom,"금일매수",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop + (nBottom - nTop),nPosition[nNextPos++],nBottom + (nBottom - nTop),"매수수량",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop + (nBottom - nTop),nPosition[nNextPos++],nBottom + (nBottom - nTop),"매수금액",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop + (nBottom - nTop),nPosition[nNextPos++],nBottom + (nBottom - nTop),"평균가",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos],nTop,nPosition[6],nBottom,"금일매도",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop + (nBottom - nTop),nPosition[nNextPos++],nBottom + (nBottom - nTop),"매도수량",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop + (nBottom - nTop),nPosition[nNextPos++],nBottom + (nBottom - nTop),"매도금액",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop + (nBottom - nTop),nPosition[nNextPos++],nBottom + (nBottom - nTop),"평균가",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom + (nBottom - nTop),"수수료+제세금",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom + (nBottom - nTop),"손익금액",FALSE,FW_NORMAL,TRUE,2);
	DrawBox(pDC,nPosition[nPrePos++],nTop,nPosition[nNextPos++],nBottom + (nBottom - nTop),"수익률",FALSE,FW_NORMAL,TRUE,2);
}

void MainDlg::DrawGridLine(CDC* pDC, const CStringArray& ar,int y1)
{
	const	int	nBottom = y1 + 50;
	const	int	nPosition[] = {250, 420, 670, 840, 1010, 1260, 1430, 1730, 2030, 2230};
	int	nNextPos = 0, nPrePos = 0,  nIndex = 0;

	//nAlign = 0 : LEFT,nAlign = 1 : RIGHT,nAlign = 2 : CENTER
	//2012.06.08 KSJ nNextPos-1이 되지 않아 따로 nPrePos증가시켜줌.. 당연히 되어야하는데 왜 안되는지 모르겠음..
	DrawBox(pDC, 0,                    y1, nPosition[nNextPos++], nBottom, ar.GetAt(nIndex++), FALSE, FW_NORMAL, FALSE, 0);
	DrawBox(pDC, nPosition[nPrePos++], y1, nPosition[nNextPos++], nBottom, ar.GetAt(nIndex++), FALSE, FW_NORMAL, FALSE, 1);
	DrawBox(pDC, nPosition[nPrePos++], y1, nPosition[nNextPos++], nBottom, ar.GetAt(nIndex++), FALSE, FW_NORMAL, FALSE, 1);
	DrawBox(pDC, nPosition[nPrePos++], y1, nPosition[nNextPos++], nBottom, ar.GetAt(nIndex++), FALSE, FW_NORMAL, FALSE, 1);
	DrawBox(pDC, nPosition[nPrePos++], y1, nPosition[nNextPos++], nBottom, ar.GetAt(nIndex++), FALSE, FW_NORMAL, FALSE, 1);
	DrawBox(pDC, nPosition[nPrePos++], y1, nPosition[nNextPos++], nBottom, ar.GetAt(nIndex++), FALSE, FW_NORMAL, FALSE, 1);
	DrawBox(pDC, nPosition[nPrePos++], y1, nPosition[nNextPos++], nBottom, ar.GetAt(nIndex++), FALSE, FW_NORMAL, FALSE, 1);
	DrawBox(pDC, nPosition[nPrePos++], y1, nPosition[nNextPos++], nBottom, ar.GetAt(nIndex++), FALSE, FW_NORMAL, FALSE, 1);
	DrawBox(pDC, nPosition[nPrePos++], y1, nPosition[nNextPos++], nBottom, ar.GetAt(nIndex++), FALSE, FW_NORMAL, FALSE, 1);
	DrawBox(pDC, nPosition[nPrePos++], y1, nPosition[nNextPos++], nBottom, ar.GetAt(nIndex++), FALSE, FW_NORMAL, FALSE, 1);
}

//Type:0=INFO LINE Type:1=Data Line
void MainDlg::FillSolidBox(CDC* pDC,int x,int y,int w,int h,int type)
{
	if (type == 0)
	{
		pDC->FillSolidRect(x+1,y+1,w-2,h-2,RGB(190,199,205));
	}
	else
	{
		pDC->FillSolidRect(x+1,y+1,w-2,h-2,HEADERBKCL);
	}
}

void MainDlg::DrawStuff(CDC* pDC)
{
	if (m_nNowPage == 1)
	{
		DrawTopBox(pDC);
		DrawGrid(pDC);
	}
	else
	{
		DrawGrid(pDC);
	}
}

void MainDlg::GetTokenSplit(CString sText, TCHAR sSplit) 
{
	 CString sResult;

	 if (sText != "")
	 {
		 // afxwin.h을 이용하여 AfxExtractSubString함수를 이용한다.
		 while (AfxExtractSubString(sResult, sText, m_tokenSize, sSplit))
		 {
			if (sResult.GetLength() > 0)
			{
				sResult.Trim();
				if (sResult.IsEmpty())
				{
					sResult = " ";
				}
	
			}

			m_tokenSize += 1;
		 }
	 }
}

BOOL MainDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_dataFile = "1301data.dat";
	m_editStatus.SetWindowText("인쇄중.......");
	m_nTotalPages = 0;
	Run();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void MainDlg::SplitString(CString &strData, CString strToken,CStringArray &ar) //
{
	CString sResult = strData;
	if (strToken == "")
		return;

	int	iFind = strData.Find(strToken); 

	if (iFind < 0)
	{
		ar.Add(sResult);
		return;
	}
	
	while (iFind > -1)
	{
		sResult = strData.Left(iFind);
		strData = strData.Right(strData.GetLength() - iFind - strToken.GetLength() );  

		ar.Add(sResult);

		iFind = strData.Find(strToken);
	}

	sResult = strData;
	ar.Add(sResult);
}

void MainDlg::LoadData()
{
	CStdioFile file;

	if (!file.Open(m_dataFile,CFile::modeRead | CFile::typeText | CFile::shareDenyNone))
	{
		m_editStatus.SetWindowText("인쇄할 데이타가 없습니다.");
		return;
	}
	
	CString strLine;

	while (file.ReadString(strLine))
	{
		//2012.01.05 KSJ 마지막 데이터가 빈값이면 스페이스 들어옴..
		//strLine.TrimRight();
		//KSJ

		if (strLine != "")
			m_arData.Add(strLine);
	}

	file.Close();

	if (m_arData.GetSize() > 30)
	{
		m_nTotalPages = (m_arData.GetSize() - 30) / DATACOUNT + 1;
		
		if ((m_arData.GetSize() - 30) % DATACOUNT > 0)
		{
			m_nTotalPages += 1;
		}
	}
	else
	{
		m_nTotalPages = 1;
	}
}

void MainDlg::OnTimer(UINT nIDEvent) 
{
	KillTimer(100);

	SetProgressPos(1);
	Start();
	
	CDialog::OnTimer(nIDEvent);
}
