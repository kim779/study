// CLBparent.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CLBparent.h"


// CLBparent

IMPLEMENT_DYNAMIC(CLBparent, CWnd)
CString parser(CString& srcstr, CString substr)
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

CLBparent::CLBparent()
{

}

CLBparent::~CLBparent()
{
}


BEGIN_MESSAGE_MAP(CLBparent, CWnd)
    ON_WM_KILLFOCUS()
    ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


#define DF_MENU_STOCK   "Menu_S"        //주식 프레임 메뉴
#define DF_MENU_STRADE   "Trade_S"      
#define DF_MENU_SSISE   "SISE_S"
#define DF_MENU_SCHART   "CHART_S"
#define DF_MENU_SBALANCE   "BALANCE_S"
#define DF_MENU_SHISTORY  "HISTORY_S"
 
#define DF_MENU_FUTURE   "Menu_F"     //파생 프레임 메뉴
#define DF_MENU_FTRADE  "Trade_F"
#define DF_MENU_FSISE   "SISE_F"
#define DF_MENU_FCHART   "CHART_F"
#define DF_MENU_FBALANCE   "BALANCE_F"
#define DF_MENU_FHISTORY  "HISTORY_F"

#define DF_MENU_SUBX_TRD   0     //매매 관련 화면 리스트
#define DF_MENU_SUBX_SISE   1     //시세 관련 화면 리스트
#define DF_MENU_SUBX_CHART   2     //차트 관련 화면 리스트
#define DF_MENU_SUBX_BALANCE   3     //잔고 관련 화면 리스트

BOOL CLBparent::MakePopMenu(CString sData, CPoint point)
{
    CString skey{}, stmp{}, sCode{}, sName{}, sFilepath{};
    CString sSMenu{}, sSHMenu{};
    CString sFMenu{}, sFHMenu{};

    //
    sFilepath = "F:\\util\\HTS\\IBK투자증권 HTS\\tab\\axPOPMenu.INI";  //여기선 탭이지만 개인 히스토리가 필요하므로   
    //

    CHMenu	 frameMenu, * menu{}, * subMenu{}, * popup{};
    sName = sData;
    sCode = parser(sName, "\t");

    DWORD rc;
    char* ptr{}, wb[1024]{};
    int iGubn = 0;
        
    //종목 시장 분류
    switch (iGubn) 
    {
        case 0:  //주식
        {
            //frame 메뉴 로드
            rc = GetPrivateProfileString(DF_MENU_STOCK, "menu", "", wb, 1024, sFilepath);
            sFMenu.Format("%s", wb);
            sFMenu.Trim();
       
            if (!rc)
            {
                sFMenu.Empty();
                return FALSE;
            }
        }
        break;
        case 1:  //파생
        {
            //frame 메뉴 로드
            rc = GetPrivateProfileString(DF_MENU_FUTURE, "menu", "", wb, 1024, sFilepath);
            sFMenu.Format("%s", wb);
            sFMenu.Trim();

            if (!rc)
            {
                sFMenu.Empty();
                return FALSE;
            }
        }
        break;
    }
    
    popup = new CHMenu();
    popup->CreatePopupMenu();

    CStringArray arrFMenu, arrSMenu, arrHMenu;
    m_pCodelist->StringSplit(sFMenu, arrFMenu, _T(';'));
   
    int	menuN{};
    int    nID{};
    CMapStringToString _map{};
 
    for (int ii = 0; ii < arrFMenu.GetSize(); ii++)
    {
        //서브 메뉴 로드  ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
        switch (ii)
        {
        case DF_MENU_SUBX_TRD:  //매매 관련 화면 리스트
        {

            rc = GetPrivateProfileString(iGubn == 0 ? DF_MENU_STRADE : DF_MENU_FTRADE, "menu", "", wb, 1024, sFilepath);
            sSMenu.Format("%s", wb);
            sSMenu.Trim();

            if (!rc)
            {
                sFMenu.Empty();
                return FALSE;
            }

            //히스토리 - 매매 메뉴 내역
            rc = GetPrivateProfileString(iGubn == 0 ? DF_MENU_STRADE : DF_MENU_FTRADE, "Trade_S_Hmenu", "", wb, 1024, sFilepath);
            sSHMenu.Format("%s", wb);
            sSHMenu.Trim();

        }
        break;
        case DF_MENU_SUBX_SISE: //시세 관련 화면 리스트
        {
            rc = GetPrivateProfileString(iGubn == 0 ? DF_MENU_SSISE : DF_MENU_FSISE, "menu", "", wb, 1024, sFilepath);
            sSMenu.Format("%s", wb);
            sSMenu.Trim();

            if (!rc)
            {
                sFMenu.Empty();
                return FALSE;
            }

            //히스토리 - 시세 메뉴 내역
            rc = GetPrivateProfileString(iGubn == 0 ? DF_MENU_STRADE : DF_MENU_FTRADE, "SISE_H_menu", "", wb, 1024, sFilepath);
            sSHMenu.Format("%s", wb);
            sSHMenu.Trim();

        }
        break;
        case DF_MENU_SUBX_CHART: //차트 관련 화면 리스트
        {
            rc = GetPrivateProfileString(iGubn == 0 ? DF_MENU_SCHART : DF_MENU_FCHART, "menu", "", wb, 1024, sFilepath);
            sSMenu.Format("%s", wb);
            sSMenu.Trim();

            if (!rc)
            {
                sFMenu.Empty();
                return FALSE;
            }

            //히스토리 - 차트  메뉴 내역
            rc = GetPrivateProfileString(iGubn == 0 ? DF_MENU_STRADE : DF_MENU_FTRADE, "CHART_S_menu", "", wb, 1024, sFilepath);
            sSHMenu.Format("%s", wb);
            sSHMenu.Trim();
        }
        break;
        case DF_MENU_SUBX_BALANCE: //잔고 관련 화면 리스트
        {
            rc = GetPrivateProfileString(iGubn == 0 ? DF_MENU_SBALANCE : DF_MENU_FBALANCE, "menu", "", wb, 1024, sFilepath);
            sSMenu.Format("%s", wb);
            sSMenu.Trim();

            if (!rc)
            {
                sFMenu.Empty();
                return FALSE;
            }

            //히스토리 - 잔고  메뉴 내역
            rc = GetPrivateProfileString(iGubn == 0 ? DF_MENU_STRADE : DF_MENU_FTRADE, "BALANCE_S_menu", "", wb, 1024, sFilepath);
            sSHMenu.Format("%s", wb);
            sSHMenu.Trim();
        }
        break;
        }
        //서브 메뉴 로드  ↑↑↑↑↑↑↑↑↑↑↑↑↑↑ 

        menu = new CHMenu();
        menu->CreatePopupMenu();

        stmp.Format("%s", arrFMenu.GetAt(ii));
        stmp.Replace("|", " ");
        popup->InsertMenuX(ii , MF_POPUP | MF_BYPOSITION | MF_STRING, (UINT)menu->m_hMenu, stmp);

        CHMenu* psub = (CHMenu*)popup->GetSubMenu(ii);
        int id = popup->GetMenuItemCount();
        
        m_pCodelist->StringSplit(sSHMenu, arrSMenu, _T(';'));
        for (int jj = 0; jj < arrSMenu.GetSize(); jj++)
        {
            stmp.Format("%s", arrSMenu.GetAt(jj));
            stmp.Replace("|", " ");
            int ret = psub->AppendMenuX(MF_BYPOSITION | MF_STRING, ++menuN, stmp, nullptr);   

            skey.Format("%d", menuN);
          //_map.SetAt(skey, stmp.Mid(stmp.Find(" ")));
            _map.SetAt(skey, stmp);

        }
        sSMenu.Empty();

        psub->AppendMenuX(MF_SEPARATOR);
        //히스토리 내역  추가
        m_pCodelist->StringSplit(sSMenu, arrHMenu, _T(';'));
        for (int jj = 0; jj < arrHMenu.GetSize(); jj++)
        {
            stmp.Format("%s", arrHMenu.GetAt(jj));
            stmp.Replace("|", " ");
            int ret = psub->AppendMenuX(MF_BYPOSITION | MF_STRING, ++menuN, stmp, nullptr);

            skey.Format("%d", menuN);
            _map.SetAt(skey, stmp.Mid(stmp.Find(" ")));
            _map.SetAt(skey, stmp);

        }
    }

   


    CPoint pt;
    GetCursorPos(&pt);


    stmp.Format("CLBparen cursor -- > [pt]  top=[%d]\t right=[%d]", pt.x, pt.y);
    OutputDebugString("\r\n" + stmp);

    stmp.Format("CLBparent[point]   point.x=[%d]\t point.y=[%d]", point.x, point.y);
    OutputDebugString("\r\n" + stmp);

    if (_bRightPop) //우측으로 띄울때
    {
        pt.x += point.x;
        pt.y -= point.y;

        pt.y -= 3;
        pt.x += 18;
    }
    else
    {

    }

    int index{};
    index = popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, this);

    switch(index)
    {
        default:
        {

        }
        break;
    }

    
    skey.Format("%d", index);
    if (_map.Lookup(skey, stmp))
    {
      //  stmp = skey + "   " + stmp;
        AfxMessageBox(m_sVal);
    }
}

LRESULT CLBparent::OnMessage(WPARAM wParam, LPARAM lParam)
{
    switch (LOWORD(wParam))
    {
        case MSG_POP_MENU:
        {
            CString stmp, scode, sval;
            stmp.Format("%s", (char*)lParam);
            scode = parser(stmp, "\n");
            sval = parser(stmp, "\t");
            CPoint point;
            point.x = atoi(sval);
            point.y = atoi(stmp);
            MakePopMenu(scode, point);
        }
        break;
    }
    return TRUE;
}

// CLBparent 메시지 처리기
BOOL CLBparent::CreatePopUpWindow(CWnd* pParent, CRect rec)
{
    //test
    m_sVal = "testtest";
    CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
        ::LoadCursor(NULL, IDC_ARROW),
        (HBRUSH)(COLOR_WINDOW + 1),
        NULL);

        // 윈도우 생성
    return CreateEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW,  // 확장 스타일: 최상위 및 도구 창
        className,                        // 클래스 이름
        _T("My Popup Window"),            // 윈도우 이름
        WS_POPUP | WS_VISIBLE | WS_BORDER, // 윈도우 스타일
        rec,                             // 위치와 크기
        pParent,                       // 부모 윈도우
        0);                               // 컨트롤 ID 없음
}

BOOL CLBparent::CreateListBox(CString items)
{
    if (1)
    {
        CRect rec;
        GetClientRect(rec);
        m_pCodelist = new CCodeListCtrl(nullptr, items);
        m_pCodelist->Create(LVS_REPORT| LVS_NOCOLUMNHEADER, rec, this, 9898);
        m_pCodelist->InsertColumn(0, "");
        m_pCodelist->InsertColumn(1, "종목명");
        m_pCodelist->InsertColumn(2, "코드");
        m_pCodelist->InsertColumn(3, "");
        m_pCodelist->InsertColumn(4, "");
        m_pCodelist->SetCheckboxeStyle(RC_CHKBOX_NORMAL);	// Enable checkboxes

        CBitmap bmp;
        m_imageList.Create(20, 20, ILC_COLORDDB | ILC_MASK, 11, 1);

        ASSERT(m_imageList.m_hImageList);

        bmp.LoadBitmap(IDB_TOOL_IMGLIST);
        m_imageList.Add(&bmp, RGB(255,0,255));
        m_pCodelist->SetImageList(&m_imageList);
        bmp.DeleteObject();





        CStringArray arr;;
        m_pCodelist->StringSplit(items, arr, _T('\t'));
        CString text{}, item{}, scode{}, sname{};
        
        for (int idx = 0 ; idx < arr.GetCount(); idx++)
        {
            sname = arr.GetAt(idx);
            scode = parser(sname, " ");
            m_pCodelist->InsertItem(idx, "");
            m_pCodelist->SetItemText(idx, 1, scode);
            m_pCodelist->SetItemText(idx, 2, sname);
            m_pCodelist->SetItemImage(idx, 3, 1);
            m_pCodelist->SetItemImage(idx, 4, 0);
            m_pCodelist->SetItemData(idx, idx);
        }





        m_pCodelist->SetColumnWidth(0, 20);
        m_pCodelist->SetColumnWidth(1, 60);
        m_pCodelist->SetColumnWidth(2, 100);
        m_pCodelist->SetColumnWidth(3, 20);
        m_pCodelist->SetColumnWidth(4, 20);
        m_pCodelist->SetFocus();
    }
    else
    {
        m_child = new CHistory(nullptr, items);
        CRect rect;
        GetClientRect(rect);
        if (1)
        {
            if (!((CHistory*)m_child)->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rect, this, 223)) //test codelist
            {
                delete m_child;
            }
        }
    }
 
    return TRUE;
}

void CLBparent::OnKillFocus(CWnd* pNewWnd)
{
    CWnd::OnKillFocus(pNewWnd);
    OutputDebugString("\t\n -------------------------------kill focus");
    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CLBparent::InitMenu()
{

}

