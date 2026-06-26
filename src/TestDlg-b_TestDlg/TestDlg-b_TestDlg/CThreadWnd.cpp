// CThreadWnd.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CThreadWnd.h"

#include "CProducer_TD.h"

IMPLEMENT_DYNAMIC(CThreadWnd, CWnd)

CThreadWnd::CThreadWnd()
{
	
}

CThreadWnd::~CThreadWnd()
{
    if (m_bThreadUSE)
    {
        CString str;
        str.Format("\r\n[test][%d][%s]CThreadWnd 소멸자 WaitForSingleObject 로 기다린다 ", m_id, __FUNCTION__);
        OutputDebugString(str);

        m_tdProducer->endThread();
        
       switch (WaitForSingleObject(m_tdProducer->m_Event_end, INFINITE))
        {
            case WAIT_OBJECT_0:
            {
                str.Format("\r\n[test][%d][%s]CThreadWnd 소멸자 CThreadWnd의   WAIT_OBJECT_0", m_id,__FUNCTION__ );
                OutputDebugString(str);
            }
            break;
            case WAIT_FAILED:
            case WAIT_ABANDONED:
            {
                str.Format("\r\n[test][%d][%s]CThreadWnd의  소멸자 WAIT_FAILED    WAIT_ABANDONED error=[%d]", m_id, __FUNCTION__,  GetLastError());
                OutputDebugString(str);
            }
            break;
            case WAIT_TIMEOUT:
            {
                str.Format("\r\n[test][%d][%s] CThreadWnd의 소멸자 WAIT_TIMEOUT", m_id,__FUNCTION__ );
                OutputDebugString(str);
            }
            break;
            default:
            {
                str.Format("\r\n[test][%d][%s] CThreadWnd 의  default", m_id, __FUNCTION__);
                OutputDebugString(str);
            }
            break;
        }
    }
 
}

BEGIN_MESSAGE_MAP(CThreadWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_TIMER()
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CThreadWnd 메시지 처리기

//#define DF_1THREAD
#define DF_2THREAD
//#define DF_TIMER

void CThreadWnd::initGrid()
{
    CString stmp, strtmp;
    CRect recGrid;
    GetClientRect(&recGrid);
    recGrid.DeflateRect(10, 10);

    m_pGrid = new CfxIGrid(this);

    const int ret = m_pGrid->Create(nullptr, _T("igrid"), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, recGrid, this, 100);
    m_pGrid->ResetContent();

    m_pGrid->m_b2RowColor = true;

    m_pGrid->m_lineColor = RGB(0, 0, 0);
    m_pGrid->m_headBgColor = RGB(255, 255, 255);
    m_pGrid->m_headFgColor = RGB(0, 0, 0);
    m_pGrid->m_dataBgColor = RGB(255, 255, 255);
    m_pGrid->m_dataBgColor1 = RGB(255, 255, 255);
    m_pGrid->m_dataBgColor2 = RGB(255, 255, 255);
    m_pGrid->m_gridFgColor = RGB(0, 0, 0);
    m_pGrid->m_selFgColor = RGB(0, 0, 0);
    m_pGrid->m_selBgColor = RGB(255, 0, 255);
    m_pGrid->m_upColor = RGB(255, 0, 0);
    m_pGrid->m_dnColor = RGB(0, 0, 255);
    m_pGrid->m_eqColor = RGB(0, 0, 0);

  
    CFont* pFont = new CFont();
    pFont->CreateFont(12,                        // nHeight
        0,                         // nWidth
        0,                         // nEscapement
        0,                         // nOrientation
        FW_BOLD,	           // nWeight
        FALSE,                     // bItalic
        FALSE,                     // bUnderline
        0,                         // cStrikeOut
        DEFAULT_CHARSET,           // nCharSet
        OUT_DEFAULT_PRECIS,        // nOutPrecision
        CLIP_DEFAULT_PRECIS,       // nClipPrecision
        DEFAULT_QUALITY,           // nQuality
        DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
        "굴림");                   // lpszFacename
    m_pGrid->m_pFont = pFont;

    CStringArray arrCol;
    arrCol.Add("시간");
    arrCol.Add("체결가");
    arrCol.Add("전일비");
    arrCol.Add("체결량");
    arrCol.Add("거래량");

    for (int ii = 0; ii < arrCol.GetSize(); ii++)
    {
        CIColumn column;
        GetCIColumnData(arrCol.GetAt(ii), column);
        column.m_lWidth = 60;
        m_pGrid->AddQColumn(column);
    }

    m_pGrid->m_lOriDataHeight = 20;
    m_pGrid->ReSizeGrid();

    m_pGrid->m_holdCOUNT = 10;

    int icnt = recGrid.Height() / (int)m_pGrid->m_lOriDataHeight;
    stmp.Format("\t\t\t\t\t");
    m_slog.Format("icnt=[%d]", icnt);
    LOG_OUTP(3, "cx_grid", __FUNCTION__, m_slog);
    for (int ii = 0; ii < icnt; ii++)
        m_pGrid->AddRow(stmp);
}

BOOL CThreadWnd::GetCIColumnData(CString colname, CIColumn& column)
{
    const int iCnt = sizeof(gridHdr) / sizeof(struct _gridHdr);

    for (int ii = 0; ii < iCnt; ii++)
    {
        if (gridHdr[ii].m_sHeadCaption == colname)
        {
            column.m_sHeadCaption = colname;
            column.m_dAttr = gridHdr[ii].m_dAttr;
            column.m_sEditFormat = gridHdr[ii].editFormat;
            return TRUE;
        }
    }

    return FALSE;
}


#include <afxwin.h> // MFC 헤더 파일
#include <windows.h> // Windows API 헤더 파일
#pragma comment(lib, "Kernel32.lib")
int CThreadWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
  
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
    if (m_bThreadUSE)
    {
        m_tdProducer = (CProducer_TD*)AfxBeginThread(RUNTIME_CLASS(CProducer_TD), THREAD_PRIORITY_NORMAL, 0, 0);
        //0x01 -> 0001
        //0x02 -> 0010
        //0x03 -> 0011
        //0x04 -> 0100
        //0x05 -> 0101
        //0x06 -> 0110
        //0x07 -> 0111
        //0x08 -> 1000
        
        ////DWORD_PTR dwProcessAffinityMask = 0x01 | 0x02 | 0x03 | 0x04 | 0x05 | 0x06 | 0x07 | 0x08 | 0x09 | 0x0a;
        //DWORD_PTR dwProcessAffinityMask =  0x03;
        //if (SetThreadAffinityMask(m_tdProducer->m_hThread, dwProcessAffinityMask) == 0) {
        //    // Handle the error
        //    int ierror = GetLastError();
        //    CString slog;
        //    slog.Format(" 스레드 친화도 실패[%d]", ierror);
        //    AfxMessageBox(slog);
        //    return FALSE;
        //}


        m_tdProducer->m_id = m_id;
        m_tdProducer->Startup(this, "");

       /* HANDLE hProcess = GetCurrentProcess();
        DWORD_PTR dwThreadAffinityMask, dwProcessAffinityMask, dwSystemAffinityMask;
        if (GetProcessAffinityMask(hProcess, &dwProcessAffinityMask, &dwSystemAffinityMask))
        {
            if (GetThreadAffinityMask(m_tdProducer, &dwThreadAffinityMask))
            {

            }
        }*/
        m_slog.Format("\r\n[test][%d][%s]  CThreadWnd::onCreate  m_tdProducer=[%x] m_tdProducer m_id=[%d]", m_id, __FUNCTION__, m_tdProducer, m_tdProducer->m_id);
       // OutputDebugString(m_slog);
        /*
        HANDLE hProcess = GetCurrentProcess();

	// 스레드가 현재 어떤 CPU 코어에서 실행되고 있는지 가져옵니다.
	DWORD_PTR dwAffinityMask;
	DWORD_PTR dwSystemAffinityMask;
	DWORD_PTR dwProcessAffinityMask;
	GetThreadContext
	if (GetProcessAffinityMask(hProcess, &dwProcessAffinityMask, &dwSystemAffinityMask))
	{
		DWORD_PTR dwThreadAffinityMask;
		if (GetThreadAffinityMask(hThread, &dwThreadAffinityMask, &dwThreadAffinityMask))
		{
			// 스레드의 현재 CPU 코어를 출력합니다.
			std::cout << "Thread is running on CPU core: " << dwThreadAffinityMask << std::endl;
		}
		else
		{
			std::cerr << "Failed to get thread affinity mask." << std::endl;
		}
	}
	else
	{
		std::cerr << "Failed to get process affinity mask." << std::endl;
	}
        */
    }
    else
    {
        m_slog.Format("\r\n[test][%d][%s]  CThreadWnd::onCreate  m_bThreadUSE=[%d]", m_id,__FUNCTION__ , m_bThreadUSE);
        OutputDebugString(m_slog);
    }

    initGrid();
	return 0;
}

void CThreadWnd::Dispatch(char*& pdata, int ilen)
{
    if (m_bThreadUSE)
    {
        if (m_tdProducer->m_alive)
        {
m_slog.Format("\r\n[test][%d][%s]   pdata = [%x]", m_id, __FUNCTION__, pdata);
OutputDebugString(m_slog);
            m_tdProducer->Dispatch(0, pdata, ilen);
        }
    }
}

void CThreadWnd::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: 여기에 메시지 처리기 코드를 추가합니다.
                       // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
    CRect clientRect;
    dc.GetClipBox(&clientRect);
    dc.FillSolidRect(clientRect, m_col);

   // dc.DrawText(m_strText, CRect(0, 0, 100, 20), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    return;
}

void CThreadWnd::MsgFromThread(CString str)
{
    if (m_bThreadUSE)
    {
        CString strResult;
            if (m_strText == str)
                return;
            m_strText = str;
            m_slog.Format("\r\n[test][%d][%s]     [%s]", m_id, __FUNCTION__ , m_strText);
            m_icnt = 0;
           
            strResult.Format("%s\t%s\t%s\t%s\t%s", str, str, str, str, str);
            m_pGrid->AddRowHold(strResult);
            m_pGrid->SetScrollTop();
    }
}

void CThreadWnd::OnDestroy()
{
    CWnd::OnDestroy();


}


void CThreadWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    CRect rec(0, 0, 100, 100);
    InvalidateRect(rec, true);
#ifdef DF_1THREAD
  
#elif  defined(DF_2THREAD)
   // SetEvent(m_hEventRTM);
#elif defined( DF_TIMER)

#endif
    CWnd::OnLButtonUp(nFlags, point);
}


void CThreadWnd::endThread()
{
    if (m_bThreadUSE)
        m_tdProducer->m_alive = false;
}

