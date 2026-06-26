// CControlWnd.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CControlWnd.h"


// CControlWnd

#define IMG_HEIGHT 389
#define IMG_WIDTH 382

IMPLEMENT_DYNAMIC(CControlWnd, CWnd)
	    
void BackgroundThreadFunc(CWnd* pWnd) {
	// 백그라운드 스레드에서 처리할 그래픽 작업
   // 예: 렌더링, 그리기 등

   // 예시로서 여기서는 간단하게 메시지 박스를 띄워봅니다.
	//::MessageBox(NULL, _T("Background thread is working!"), _T("Background Thread"), MB_OK);
	CDC dc;
	if (dc.Attach(pWnd->GetDC()->m_hDC) != 0) {
		CRect rec;
		pWnd->GetClientRect(&rec);
		CRect rect(50, 50, 150, 150);
		dc.FillSolidRect(rect, RGB(255, 0, 255));
		dc.Detach();
		pWnd->Invalidate();
	}
}

UINT testThread(LPVOID lpvoid)
{
	CControlWnd* pwnd = (CControlWnd*)lpvoid;

	if (!pwnd->drawingThreadRunning)
		return 0;

	CDC dc;
	if (dc.Attach(pwnd->GetDC()->m_hDC) != 0)
	{
		 CMeMDC memdc(&dc);
		 while (true) {
			 std::lock_guard<std::mutex> lock(pwnd->threadMutex);
			 if (!pwnd->drawingThreadRunning) {
				 return 0;
				 break; // Exit the thread loop if requested
			 }

			 pwnd->TestDraw(&memdc);
			 Sleep(10);
			 if (!pwnd->drawingThreadRunning)
				 return 0;
		 }
	}
	return 0;
}

UINT RunDrawingThread(LPVOID lpvoid)
{
	//std::thread bgThread(BackgroundThreadFunc, (CWnd * )lpvoid);
	std::thread bgThread(testThread,lpvoid);


	// 스레드를 분리
	bgThread.detach();
	return 0;
}

void StartBackgroundThread(CWnd* pWnd) {
	// 백그라운드 스레드 생성
	std::thread bgThread(testThread, pWnd);


	// 스레드를 분리
	bgThread.detach();
}

/// ////////////////////////////////////////////////////////////

UINT _RunDrawingThread(LPVOID lpvoid) {
	CControlWnd* pwnd = (CControlWnd*)lpvoid;
	pwnd->SetDrawingThreadRunning(true);
	pwnd->drawingThread = std::thread(testThread, lpvoid);
	pwnd->drawingThread.detach();
	return 0;
}

/// ////////////////////////////////////////////////////////////
void CControlWnd::TestDraw(CDC* pDC)
{
	CRect clientRect;
	GetClientRect(&clientRect);
	if (clientRect.IsRectEmpty())
		return;

	srand(static_cast<unsigned int>(time(nullptr))); // 난수 발생기 초기화

	for (int i = 0; i < 1000; ++i) {
		int x = rand() % clientRect.Width();
		int y = rand() % clientRect.Height();
		COLORREF color = RGB(GetTickCount() % 256, GetTickCount() % 256, GetTickCount() % 256);

		CBrush brush;
		brush.CreateSolidBrush(color);

		pDC->SelectObject(&brush);
		pDC->Ellipse(x - 10, y - 10, x + 10, y + 10);
	}
}

int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 백그라운드 스레드 시작
	 AfxBeginThread(_RunDrawingThread, this, THREAD_PRIORITY_HIGHEST);

	return 0;
}

CControlWnd::CControlWnd()
{
	
}

CControlWnd::~CControlWnd()
{
	CloseDrawingThread();
}


BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CControlWnd 메시지 처리기




void CControlWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
					   // 
	//DrawInfo(&dc);


	//
	//CRect rec;
	//GetClientRect(rec);
	//dc.FillSolidRect(rec, RGB(255, 0, 0));
	//
	//
	//int iHeight = rec.Height() / 2;
	//rec.right -= 20;
	//rec.bottom -= iHeight;
	//dc.FillSolidRect(rec, RGB(255, 255, 0));

	//rec.top = rec.bottom;
	//rec.bottom = rec.top + iHeight - 20;
	//dc.FillSolidRect(rec, RGB(255, 0, 0));
	//




	//CPoint pt;
	//pt.x = 10;
	//pt.y = 10;

	//dc.SetPixel(pt, RGB(255, 0, 0));
	// 
	// 
	// 
	// 
	// 
	// 
	// 
	// 
	// 
	// 
//
//	CDC mdc;
//	mdc.CreateCompatibleDC(&dc);
//
//	CBitmap bmp, * pOldBmp;  // 비트맵 객체를 선언한다
//
//	BITMAP bmpInfo;
//	bmp.LoadBitmap(IDB_BITMAP1);  // 비트맵 리소스를 읽어온다
//	bmp.GetBitmap(&bmpInfo);
//
//	//CBitmap* m_byt = new CBitmap;
////	m_byte = new CBitmap;
////	m_byte->Attach(bmp);
//	
//	pOldBmp = mdc.SelectObject(&bmp);
//
//	
//	dc.BitBlt(rec.left, rec.top, rec.Width(), rec.Height(), &mdc, 0, 0, SRCCOPY);
//	mdc.SelectObject(pOldBmp);
//	mdc.DeleteDC();

	////-------------------------------------------------------------------------------------------------------------------------
	//비트맵 마구 그리기
	//RECT stage_input_rect;
	//CRect stage_input_rect;
	//GetClientRect(stage_input_rect);


	//// 4. 마진을 추가한다. 

	//int input_rect_margin = 10;

	//stage_input_rect.bottom += input_rect_margin;

	//stage_input_rect.right += input_rect_margin;

	//stage_input_rect.top += input_rect_margin;

	//stage_input_rect.left += input_rect_margin;



	//// 5. stage의 위치를 저장한다. 이 위치에 실제로 그린다. 

	//inputStageTop = stage_input_rect.top;

	//inputStageBottom = stage_input_rect.bottom;

	//inputStageRight = stage_input_rect.right + 0;

	//inputStageLeft = stage_input_rect.left + 0;

	//inputStageWidth = inputStageRight - inputStageLeft;

	//inputStageHeight = inputStageBottom - inputStageTop;

	//inputStageXoffset = 0, inputStageYoffset = 0;

	//inputStageZoom = 1;

	//for (int i = 0; i < inputWidthPerRegion; i++)
	//{
	//	for (int j = 0; j < inputWidthPerRegion; j++)
	//	{
	//		for (int k = 0; k < 4; k++)
	//		{
	//			inputRGBbuffer[i * inputWidthPerRegion * 4 + j * 4 + k] = (int)(rand() % 255);	// 랜덤한 색
	//		}
	//	}
	//}



	//::SetStretchBltMode(dc.m_hDC, COLORONCOLOR);

	//::StretchDIBits(dc.m_hDC

	//	, inputStageLeft, inputStageTop, inputStageWidth, inputStageHeight

	//	, (int)(inputHalfWidthPerRegion * (1 - inputStageZoom) + inputStageXoffset + 0.5)

	//	, (int)(inputHalfWidthPerRegion * (1 - inputStageZoom) + inputStageYoffset + 0.5)

	//	, (int)((inputWidthPerRegion * inputStageZoom) + inputStageXoffset + 0.5)

	//	, (int)((inputWidthPerRegion * inputStageZoom) + inputStageYoffset + 0.5)

	//	, inputRGBbuffer, &inputBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

	////-------------------------------------------------------------------------------------------------------------------------

	
}

void CControlWnd::CheckWnd(CString strdata)
{
	

}

void CControlWnd::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
		case 9999:
		{
			Invalidate(TRUE);
		}
		break;
	}
	CWnd::OnTimer(nIDEvent);
}


long CControlWnd::GetScrollPos32(int nBar, BOOL bGetTrackPos)
{
	SCROLLINFO sinfo;
	sinfo.cbSize = sizeof(SCROLLINFO);

	if (bGetTrackPos)
	{
		if (GetScrollInfo(nBar, &sinfo, SIF_TRACKPOS))
			return sinfo.nTrackPos;
	}
	else
	{
		if (GetScrollInfo(nBar, &sinfo, SIF_POS))
			return sinfo.nPos;
	}

	return 0;
}

BOOL CControlWnd::SetScrollPos32(int nBar, int nPos, BOOL bRedraw)
{
	SCROLLINFO sinfo;

	sinfo.cbSize = sizeof(SCROLLINFO);
	sinfo.fMask = SIF_POS;
	sinfo.nPos = nPos;

	return SetScrollInfo(nBar, &sinfo, bRedraw);
}

long CControlWnd::GetScrollMax32(int nBar)
{
	SCROLLINFO sinfo;
	sinfo.cbSize = sizeof(SCROLLINFO);

	if (GetScrollInfo(nBar, &sinfo, SIF_RANGE))
	{
		m_slog.Format("\r\n GetScrollInfo max =[%d] ", sinfo.nMax);
		OutputDebugString(m_slog);
		return sinfo.nMax;
	}
	return 0;
}

void CControlWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nBar = 0;

	switch (nSBCode)
	{
	case SB_LINEUP:
		nBar = GetScrollPos(SB_VERT);
		SetScrollPos(SB_VERT, nBar - 10);
		break;
	case SB_LINEDOWN:
		nBar = GetScrollPos(SB_VERT);
		SetScrollPos(SB_VERT, nBar + 10);
		break;
	case SB_PAGEUP:
		nBar = GetScrollPos(SB_VERT);
	//	SetScrollPos(SB_VERT, nBar - m_rect.Height());
		break;
	case SB_PAGEDOWN:
		nBar = GetScrollPos(SB_VERT);
	//	SetScrollPos(SB_VERT, nBar + m_rect.Height());
		break;
	case SB_THUMBTRACK:
		SetScrollPos(SB_VERT, nPos);
		break;
	}
	//CClientDC dc(this); // 화면에 그릴 DC를 얻어옵니다.
	//CRect rect;
	//GetClientRect(&rect); // 클라이언트 영역 크기를 얻어옵니다.
	//int lineHeight = dc.GetTextExtent(_T("TEST")).cy; // 텍스트 높이를 얻어옵니다.
	//int nDelta{};

	//int	scrollPos{}, scrollMax{};
	//CRect clientRc;

	//GetClientRect(&clientRc);
	//
	//scrollPos = GetScrollPos32(SB_VERT);
	//scrollMax = GetScrollMax32(SB_VERT);

	//m_slog.Format("\r\n [fxigrid] nSBCode=[%d]  nPos=[%d]  scrollPos=[%d] scrollMax=[%d] Height=[%d]",
	//	nSBCode, nPos, scrollPos, scrollMax, clientRc.Height());

	//switch (nSBCode)
	//{
	//case SB_LINEUP:
	//	// 한 줄 위로 스크롤
	//	nDelta = -1;
	//	scrollPos = max(scrollPos - 1, 0);
	//	SetScrollPos32(SB_VERT, scrollPos);
	////	ScrollWindow(0, -lineHeight);
	//	break;
	//case SB_LINEDOWN:
	//	// 한 줄 아래로 스크롤
	//	 nDelta = 1;
	//	scrollPos = min(scrollPos + 1, scrollMax);
	//	SetScrollPos32(SB_VERT, scrollPos);
	////	ScrollWindow(0, lineHeight);
	//	break;
	//case SB_PAGEUP:
	//	// 한 페이지 위로 스크롤
	//	//ScrollWindow(0, -rect.Height());
	//	break;
	//case SB_PAGEDOWN:
	//	// 한 페이지 아래로 스크롤
	////	ScrollWindow(0, rect.Height());
	//	break;
	//case SB_THUMBPOSITION:
	//	// 스크롤바의 위치를 얻어와서 해당 위치로 스크롤
	////	nDelta = nPos - GetScrollPos(SB_VERT);
	////	ScrollWindow(0, -nDelta * lineHeight);
	//	break;
	//default:
	//	break;
	//}

	////m_nScrollPos += nDelta;
	////m_nScrollPos = max(0, min(100, m_nScrollPos)); // 스크롤바 위치를 최소값과 최대값 사이로 제한합니다.
	////m_vScrollBar.SetScrollPos(m_nScrollPos); // 스크롤바 위치를 업데이트합니다.


	//Invalidate();
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CControlWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	const int	wheel = 30;
	int	nBar = GetScrollPos(SB_VERT);
	OutputDebugString("\r\n ------------------------------------------------------");
	GetScrollMax32(SB_VERT);
	m_slog.Format("\r\n OnMouseWheel nBar=[%d]  ", nBar);
	OutputDebugString(m_slog);

	if (zDelta > 0)
		SetScrollPos(SB_VERT, nBar - wheel);
	else
		SetScrollPos(SB_VERT, nBar + wheel);

	nBar = GetScrollPos(SB_VERT);
	const double pos = ((double)(IMG_HEIGHT - m_rect.Height()) / (double)IMG_HEIGHT) * nBar;
	m_Scroll = (int)pos;

	if (nBar + wheel > IMG_HEIGHT){
		m_Scroll = IMG_HEIGHT - m_rect.Height();
	}

	Invalidate();

	CString sdata;
	sdata.Format("\r\n[OnMouseWheel] nBar=[%d] m_Scroll=[%f]", nBar, pos);
	OutputDebugString(sdata);


	/*if (GetScrollMax32(SB_VERT))
	{
		if (zDelta < 0)
		{
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), 0);
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), 0);
		}
		else
		{
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), 0);
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), 0);
		}
	}*/
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CControlWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	//adjustScroll(SB_VERT);
	/*
	GetClientRect(&m_rect);
	SCROLLINFO info;
	info.cbSize = 30;
	info.fMask = SIF_ALL;
	info.nMin = 0;
	info.nMax = IMG_HEIGHT;
	//info.nMax = 10;
	info.nPage = 1;   //한번 스크롤 할때 가는 거야 .. 이거 nBar에 더해주다가 nMAx가 넘어가려 하면 스크롤이 안되는거다
	//info.nPage = 1;
	info.nPos = 0;
	info.nTrackPos = 0;
	SetScrollInfo(SB_VERT, &info);
	*/
}

BOOL CControlWnd::LoadImage()
{
	CString	filename;
	filename.Format("%s", "전략상태.bmp");

	HBITMAP	hBmp;
	hBmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
		filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	m_pBitmap = std::make_unique<CBitmap>();
	m_pBitmap->Attach(hBmp);

	if (m_pBitmap)
	{
		BITMAP bmpInfo;
		m_pBitmap.get()->GetBitmap(&bmpInfo);
		SetScrollPos(SB_VERT, 0);
		m_Scroll = 0;
		Invalidate();
		return TRUE;
	}
	else
	{
		Invalidate();
		return FALSE;
	}
}

void CControlWnd::DrawInfo(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(rc, RGB(255, 0, 0));

	if (m_pBitmap)
	{
		CDC dcMem;
		dcMem.CreateCompatibleDC(pDC);

		CBitmap* pbitmapOld = dcMem.SelectObject(m_pBitmap.get());
		pDC->BitBlt(0, 0 - m_Scroll, IMG_WIDTH, IMG_HEIGHT, &dcMem, 0, 0, SRCCOPY);

		CString sdata;
		sdata.Format("\r\n  m_Scroll=[%d]", m_Scroll);
		OutputDebugString(sdata);

		dcMem.SelectObject(pbitmapOld);
		dcMem.DeleteDC();
	}
	else
	{
		
	}
}

void CControlWnd::adjustScroll(int wBar)
{
	CRect clientRc;
	GetClientRect(&clientRc);
/*	if (wBar == SB_HORZ || wBar == SB_BOTH)
	{
		if (m_lSumColumnWidth > clientRc.Width())  //컬럼이 있으면 들어간다.
		{
			SCROLLINFO info;
			info.fMask = SIF_RANGE | SIF_PAGE | SIF_TRACKPOS;
			GetScrollInfo(SB_HORZ, &info);
			info.fMask  |= (m_wDisableBar & SB_HORZ? SIF_DISABLENOSCROLL:0);
			info.cbSize = sizeof(SCROLLINFO);
			info.nMin = 0; info.nMax = GetLastColCnt();
			info.nPage     = 1;
			info.nPos      = m_StartPnt.x;
			info.nTrackPos = m_StartPnt.x;

			SetScrollInfo(SB_HORZ, &info);
			ShowScrollBar(SB_HORZ);
		}
		else
		{
			if (m_wDisableBar & SB_HORZ)
			{
				SCROLLINFO info;
				info.fMask = SIF_RANGE | SIF_PAGE | SIF_TRACKPOS | SIF_DISABLENOSCROLL;
				SetScrollInfo(SB_HORZ, &info);
			}
			else
				ShowScrollBar(SB_HORZ, FALSE);
		}
	}
*/

	std::vector<int> _vArrData;
	if (wBar == SB_VERT || wBar == SB_BOTH)
	{
		//데이터행들과 헤더의 길이가  > 실제 화면 보다 큰경우  ->  이경우 스크롤이 생겨야 한다
		if (m_lHeadHeight + m_lDataHeight * gsl::narrow_cast<int>(_vArrData.size()) > clientRc.Height())
		{
			SCROLLINFO info;
			info.fMask = SIF_RANGE | SIF_PAGE | SIF_TRACKPOS;
			GetScrollInfo(SB_VERT, &info);
			info.fMask |= (m_wDisableBar & SB_VERT?SIF_DISABLENOSCROLL:0);
			info.cbSize = sizeof(SCROLLINFO);
			info.nMin = 0; 
			info.nMax = _vArrData.size() - (clientRc.Height() - m_lHeadHeight) / m_lDataHeight;  //이상
			info.nPage     = 1;
			info.nPos      = m_StartPnt.y;
			info.nTrackPos = m_StartPnt.y;

			SetScrollInfo(SB_VERT, &info);
			ShowScrollBar(SB_VERT);
		}
		
		else
		{
			if (m_wDisableBar & SB_VERT)
			{
				SCROLLINFO info;
				info.fMask = SIF_RANGE | SIF_PAGE | SIF_TRACKPOS | SIF_DISABLENOSCROLL;
				SetScrollInfo(SB_VERT, &info);
			}
			else
				ShowScrollBar(SB_VERT, FALSE);
		}
	}
}



void CControlWnd::OnClose()
{

	CWnd::OnClose();
}


void CControlWnd::OnDestroy()
{
	CWnd::OnDestroy();
	CloseDrawingThread();
}
