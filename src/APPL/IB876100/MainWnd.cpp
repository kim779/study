// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB876100.h"
#include "MainWnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainWnd


struct	MAP_NAME
{
	int		indx;
	char	name[10];
	char	desc[40];
} map_name[] = {
	{1, "IB876130", "맵일차트"},
	{2, "IB876130", "기관매수"},
	{3, "IB876130", "해외지수"},
//	{4, "IB876150", "관심종목"},
	{-1, "",		""		  }
};


#define HEIGHT_PLUS_MAP1  30
		
int	ii = 1;

CMainWnd::CMainWnd()
{
	EnableAutomation();
}

CMainWnd::CMainWnd(CWnd *parent)
{
	m_pWizard = parent;

	m_mapKey = -1;
	m_timer = 0;
	

}

CMainWnd::~CMainWnd()
{
}

void CMainWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {E9112B2E-CB3C-4596-9813-656D854A8860}
static const IID IID_IMainWnd =
{ 0xe9112b2e, 0xcb3c, 0x4596, { 0x98, 0x13, 0x65, 0x6d, 0x85, 0x4a, 0x88, 0x60 } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers

void CMainWnd::init_map()
{
	CRect	rc2, rc3;
	CString mapName;

	
	
	m_pMapWnd3->GetClientRect( rc3 ); 	
	m_pMapWnd3->ShowWindow(SW_SHOW);
	rc2.InflateRect(0,1);
	m_pMapWnd3->SetWindowPos( NULL, 0, rc3.Height()/2+30, rc3.Width(), rc3.Height(), SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
	rc2.DeflateRect(0,1);
	m_pMapWnd3->SetWindowPos( NULL, 0, rc3.Height()/2+30, rc3.Width(), rc3.Height(), SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
	m_pMapWnd3->MoveWindow(0, rc3.Height()/2+30, rc3.Width(), rc3.Height()/2);
	

}

CString CMainWnd::Variant(int nComm, CString strData)
{
	CString strRet(_T(""));
	
	char* pData = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nComm), (LPARAM)(LPCTSTR)strData);
	
	if ((long)pData > 1)
		strRet = pData;
	
	return strRet;
}

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	CRect rc, rc1;
	CTime timer2;     
	timer2 = CTime::GetCurrentTime(); 

	Variant(titleCC, "[8761] 전광판");


	CString mapName;
	GetClientRect(&rc);
	
	mapName.Format("IB876170");
	
	m_pMapWnd7 = std::make_unique<CMapWnd>(m_pWizard);
	m_pMapWnd7->Create(NULL, NULL, WS_CHILD, rc, this, IDC_MAPVIEW7);
	m_pMapWnd7->ChangeMap(mapName, "");
	
	m_pMapWnd7->GetClientRect( rc1 ); 	
	m_pMapWnd7->ShowWindow(SW_SHOW);
	rc1.InflateRect(0,1);
	m_pMapWnd7->SetWindowPos( NULL, 0, 0, rc1.Width()/7, rc1.Height()/2-30, SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
	rc1.DeflateRect(0,1);
	m_pMapWnd7->SetWindowPos( NULL, 0, 0, rc1.Width()/7, rc1.Height()/2-30, SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );

	if (1)
	{
		mapName.Format("IB876112");

		m_pMapWnd8 = std::make_unique<CMapWnd>(m_pWizard);
		m_pMapWnd8->Create(NULL, NULL, WS_CHILD, rc, this, IDC_MAPVIEW8);
		m_pMapWnd8->ChangeMap(mapName, "");
		m_pMapWnd8->ShowWindow(SW_HIDE);
	}


	mapName.Format("IB876110");
	
	m_pMapWnd1 = std::make_unique<CMapWnd>(m_pWizard);
	m_pMapWnd1->Create(NULL, NULL, WS_CHILD, rc, this, IDC_MAPVIEW1);
	m_pMapWnd1->ChangeMap(mapName, "");
	m_pMapWnd1->ShowWindow(SW_HIDE);

	mapName.Format("IB876111");
	
	m_pMapWnd5 = std::make_unique<CMapWnd>(m_pWizard);
	m_pMapWnd5->Create(NULL, NULL, WS_CHILD, rc, this, IDC_MAPVIEW1);
	m_pMapWnd5->ChangeMap(mapName, "");
	m_pMapWnd5->ShowWindow(SW_HIDE);

	mapName.Format("IB876120");
	m_pMapWnd2 = std::make_unique<CMapWnd>(m_pWizard);
	m_pMapWnd2->Create(NULL, NULL, WS_CHILD, rc, this, IDC_MAPVIEW2);
	m_pMapWnd2->ChangeMap(mapName, "");
	m_pMapWnd2->ShowWindow(SW_HIDE);
	
 	mapName.Format("IB876160");
 	m_pMapWnd4 = std::make_unique<CMapWnd>(m_pWizard);
 	m_pMapWnd4->Create(NULL, NULL, WS_CHILD, rc, this, IDC_MAPVIEW4);
 	m_pMapWnd4->ChangeMap(mapName, "");
 	m_pMapWnd4->ShowWindow(SW_HIDE);

	mapName.Format("IB876161");
	m_pMapWnd6 = std::make_unique<CMapWnd>(m_pWizard);
	m_pMapWnd6->Create(NULL, NULL, WS_CHILD, rc, this, IDC_MAPVIEW6);
	m_pMapWnd6->ChangeMap(mapName, "");
	m_pMapWnd6->ShowWindow(SW_HIDE);
	
	m_label = std::make_unique<CLabel>();
	m_label->Create("test", WS_CHILD|WS_VISIBLE|SS_CENTER,  rc, this);
	m_label->SetFontName("휴먼둥근헤드라인");
	m_label->SetFontSize(30);
	m_label->SetText("코스피최근6개월동향");
	m_label->SetBkColor(RGB(255,255,255));
//	m_label->MoveWindow(25, 665+30, 35, 330);  //test
	m_label->MoveWindow(25, 481, 35, 330);
	m_label->ShowWindow(SW_HIDE);

// 	m_label2 = new CLabel();
// 	m_label2->Create("", WS_CHILD|WS_VISIBLE|ES_CENTER,  rc, this);
// 	m_label2->SetFontName("휴먼둥근헤드라인");
// 	m_label2->SetFontSize(50);
// 	m_clock.Format("%04d-%02d-%02d %02d:%02d:%02d", timer2.GetYear(), timer2.GetMonth(), timer2.GetDay(),
// 		timer2.GetHour(),timer2.GetMinute(),timer2.GetSecond()); 
// 	m_label2->SetBkColor(RGB(255,255,255));
// 	
// 	m_label2->MoveWindow(-10, 0, 460, 230);
// 	m_label2->ShowWindow(SW_HIDE);
//	get_xml();

//	BOOL iResult = ::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 0, 0, MAP_WIDTH, MAP_HEIGHT, SWP_NOSIZE | SWP_NOMOVE);

	return 0;
}


void CMainWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rect;

	GetClientRect(&rect);


	dc.FillSolidRect(rect, RGB(0,0,0));

	
}

void CMainWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	if (m_pMapWnd1)
	{
		if (m_pMapWnd1->m_hWnd && ::IsWindow(m_pMapWnd1->m_hWnd))
			m_pMapWnd1->DestroyWindow();
	}
	
	if (m_pMapWnd2)
	{
		if (m_pMapWnd2->m_hWnd && ::IsWindow(m_pMapWnd2->m_hWnd))
			m_pMapWnd2->DestroyWindow();
	}
	
	if (m_pMapWnd6)
	{
		m_pMapWnd6->closeMap();
		
		if (m_pMapWnd6->m_hWnd && ::IsWindow(m_pMapWnd6->m_hWnd))
			m_pMapWnd6->DestroyWindow();
	}

	if (m_pMapWnd7)
	{
		m_pMapWnd7->closeMap();
		
		if (m_pMapWnd7->m_hWnd && ::IsWindow(m_pMapWnd7->m_hWnd))
			m_pMapWnd7->DestroyWindow();
	}

	if (m_pMapWnd8)
	{
		if (m_pMapWnd8->m_hWnd && ::IsWindow(m_pMapWnd8->m_hWnd))
			m_pMapWnd8->DestroyWindow();
	}
//	delete m_label;
	
	KillTimer(1);
//	KillTimer(2);
//	KillTimer(3);

	
}


LONG CMainWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	char	*temp=nullptr;
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:		// Wizard request TR Header
		// return char*
		break;
	case DLL_OUB:		// Received Data
		// HIBYTE(LOWORD(WPARAM)) : key, HIWORD(WPARAM) : DATA size
		// HIBYTE(LOWORD(WPARAM)) : DATA size for MSB 8bits (control FORM OUB)
		break;
	case DLL_ALERT:	// RealTime data 
		// HIWORD(WPARAM) : stat
		// update : if control DLL,  DATAs
		// otherwise  CODE \t DATAs				
		break;
	case DLL_TRIGGER:	// Trigger data
		// HIBYTE(LOWORD(WPARAM)) : key
		// HIWORD(WPARAM) : trigger stat
		// trigger stat(true)  -> text : PROC \t PARAM
		//           (false) -> text : SYM \t DATA		
		temp = (char *)lParam;
		set_timer(temp);
		
		break;
	case DLL_DOMINO:	// Domino data
		// LPARAM : text, SYM \t DATA (\n ... )
		// HIWORD(WPARAM) : domino stat			
		break;
	
	}
	return TRUE;

}

void CMainWnd::set_timer(char *timer)
{
	int		cTime=0;
	CRect	rc, rc3, tempRect, tmpRect;
	CString mapName;
	CString stmp;

	stmp.Format("%s", timer);

	if(stmp.Find("MaxCntTrigger") >= 0)
		return;

	cTime = atoi(timer);
	m_timer = cTime * 1000;
	
	
	m_pMapWnd7->ShowWindow(SW_HIDE);
//	m_label2->ShowWindow(SW_SHOW);
	GetClientRect(&rc);

	const int iHeight = rc.Height() / 14;
	

	//무사고
	int idelta = 155;
	m_pMapWnd8->ShowWindow(SW_SHOW);
	tempRect = CRect(0, 0, rc.Width(), idelta);
	m_pMapWnd8->MoveWindow(tempRect);

	//코스피
	m_pMapWnd1->ShowWindow(SW_SHOW);

	if (1)
	{
		tempRect = CRect(0, idelta, rc.Width(), idelta * 2);
		//tempRect.OffsetRect(0, iHeight);  //test
		m_pMapWnd1->MoveWindow(tempRect);
		//m_pMapWnd1->ShowWindow(SW_HIDE);
	}
	else
	{
		tempRect = CRect(0, 0, rc.Width(), iHeight * 2);
		tmpRect = tempRect;
		tmpRect.DeflateRect(0, 2);  //test
		m_pMapWnd1->MoveWindow(tmpRect);
	}
	

	//코스닥
	m_pMapWnd5->ShowWindow(SW_SHOW);

	if (1)
	{
		tempRect.OffsetRect(0, idelta);  //test
		m_pMapWnd5->MoveWindow(tempRect);
		//m_pMapWnd5->ShowWindow(SW_HIDE);
	}
	else
	{
		tempRect.OffsetRect(0, iHeight * 3); 
		tmpRect = tempRect;
		tmpRect.DeflateRect(0, 2);  
		m_pMapWnd5->MoveWindow(tmpRect);
	}


	if (1)  //시계
	{
		m_pMapWnd4->ShowWindow(SW_SHOW);
		tempRect.OffsetRect(0, iHeight * 2);
		m_pMapWnd4->MoveWindow(tempRect);
	}
	else {
		//	m_pMapWnd4->GetClientRect( rc2 ); 	// 시계
		m_pMapWnd4->ShowWindow(SW_SHOW);
		tempRect.OffsetRect(0, iHeight * 3);
		tempRect.bottom -= iHeight;
		tmpRect = tempRect;
		tmpRect.InflateRect(0, 3);
		m_pMapWnd4->MoveWindow(tmpRect);
	}


	/*
	rc2.InflateRect(0,1);
	m_pMapWnd4->SetWindowPos( NULL, 0, 0, 590, 230, SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
	rc2.DeflateRect(0,1);
	m_pMapWnd4->SetWindowPos( NULL, 0, 0, 590, 230, SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
	m_pMapWnd4->MoveWindow(0, 50, 720, 125);
	*/

	//하단 차트
//	m_pMapWnd2->GetClientRect( rc3 ); 	
	m_pMapWnd2->ShowWindow(SW_SHOW);
	/*
	rc3.InflateRect(0,1);
	m_pMapWnd2->SetWindowPos( NULL, 0, rc3.Height()/2+30, rc3.Width(), rc3.Height()/2, SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
	rc3.DeflateRect(0,1);
	m_pMapWnd2->SetWindowPos( NULL, 0, rc3.Height()/2+30, rc3.Width(), rc3.Height()/2, SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
	m_pMapWnd2->MoveWindow(0, 481, rc3.Width(), 430);  //test
	*/
	tempRect.OffsetRect(0, iHeight * 2);
	tempRect.bottom = rc.bottom;
	tempRect.bottom += HEIGHT_PLUS_MAP1;   
	tmpRect = tempRect;
	tmpRect.DeflateRect(0, 2);
	m_rectchart_1 = tmpRect;				 //로테이션 되는 하단 맵중 첫번째는 약간 크게 그려서 스크롤을 안보이게 한다. 
	m_pMapWnd2->MoveWindow(tmpRect);

	tmpRect.bottom -= HEIGHT_PLUS_MAP1;		 	
	m_rectchart_2 = tmpRect;

	tmpRect.bottom -= 15;
	m_rectchart_3 = tmpRect;
	


	m_label->ShowWindow(SW_SHOW);
	m_label->GetClientRect( rc3 ); 
	ClientToScreen(rc3);
	rc3.OffsetRect(0, tempRect.top - rc3.top);
	m_label->MoveWindow(rc3);


// 	m_pMapWnd4->GetClientRect( rc3 ); 	
// 	rc2.InflateRect(0,1);
// 	m_pMapWnd4->SetWindowPos( NULL, 0, rc3.Height()/2+30, rc3.Width(), rc3.Height()/2, SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
// 	rc2.DeflateRect(0,1);
// 	m_pMapWnd4->SetWindowPos( NULL, 0, rc3.Height()/2+30, rc3.Width(), rc3.Height()/2, SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
// 	m_pMapWnd4->MoveWindow(0, 670, rc3.Width(), 430);
// 	m_pMapWnd4->ShowWindow(SW_HIDE);

	
	

//	SetTimer(1, m_timer, NULL);
	SetTimer(1, 1, NULL);
//	SetTimer(2, 1000, NULL);	
//	SetTimer(3, 1000 * 3600, NULL);
}

void CMainWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default


	CWnd::OnTimer(nIDEvent);
	CRect rc, rc2;
	CTime timer;     
	CString	mapName;
	CString slog;
	timer = CTime::GetCurrentTime(); 

	GetClientRect(&rc);
KillTimer(nIDEvent);
	if(nIDEvent == 1)
	{
// 		if(ii == 3)
// 		{
// 			m_pMapWnd2->ShowWindow(SW_HIDE);
// //			m_pMapWnd4->ShowWindow(SW_SHOW);
// 			m_label->ShowWindow(SW_HIDE);
// 			ii++;
// 		}
	KillTimer(nIDEvent);
		if(ii == 0)
		{	
			m_pMapWnd2->ChangeMap(map_name[ii].name);	
			
			GetClientRect( rc ); 	
			
			rc.InflateRect(0,1);
			m_pMapWnd2->SetWindowPos( NULL, 0, m_rectchart_1.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
			
			m_pMapWnd2->MoveWindow(m_rectchart_1);
			m_label->ShowWindow(SW_SHOW);
			m_pMapWnd2->ShowWindow(SW_SHOW);
			ii++;
		}
		else
		{
			CRect rec;
			m_label->ShowWindow(SW_HIDE);
			
			m_pMapWnd2->ChangeMap(map_name[ii].name);

			if(ii == 1)
				rec = m_rectchart_2;
			else if(ii == 2)
				rec = m_rectchart_3;

			m_pMapWnd2->SetWindowPos( NULL, 0, rec.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
			m_pMapWnd2->MoveWindow(rec);
	
			ii++;
		}
		
		if(ii >= 3)
			ii = 0;
	}
	else if(nIDEvent == 3)
	{
		mapName.Format("IB876161");
		m_pMapWnd6 = std::make_unique<CMapWnd>(m_pWizard);
		m_pMapWnd6->Create(NULL, NULL, WS_CHILD, rc, this, IDC_MAPVIEW6);
		m_pMapWnd6->ChangeMap(mapName, "");
		m_pMapWnd6->ShowWindow(SW_HIDE);

		m_pMapWnd6->GetClientRect( rc2 ); 	
		m_pMapWnd6->ShowWindow(SW_SHOW);
		rc2.InflateRect(0,1);
		m_pMapWnd6->SetWindowPos( NULL, 0, rc2.Height()/2-30, rc2.Width(), rc2.Height()/2+30, SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
		rc2.DeflateRect(0,1);
		m_pMapWnd6->SetWindowPos( NULL, 0, rc2.Height()/2-30, rc2.Width(), rc2.Height()/2+30, SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
		m_pMapWnd6->MoveWindow(450, 0, rc2.Width(), 230);
	}
}

bool CMainWnd::get_xml()
{
	CInternetSession session;
    CInternetFile* file = NULL;
    try 
    {
        // try to connect to the URL
        file = (CInternetFile*) session.OpenURL(URL); 
    }
    catch (CInternetException* m_pException)
    {
        // set file to NULL if there's an error
        file = NULL; 
        m_pException->Delete();
    }

	CStdioFile dataStore;
	CStdioFile subStore;
    
    if (file)
    {
        CString somecode;
		
		const BOOL bIsOk = subStore.Open(_T("../tab/subfile.txt"),
            CFile::modeCreate 
            | CFile::modeWrite 
            | CFile::shareDenyWrite 
            | CFile::typeText);
        
        if (!bIsOk)
            return FALSE;
        
		CString str;
//		BOOL start=FALSE;
        // continue fetching code until there is no more
        while (file->ReadString(somecode) != NULL) 
        {
			somecode += "\n";
			subStore.WriteString(somecode);		
		}
        
        file->Close();
		subStore.Close();
        delete file;
    }
    else
    {
        dataStore.WriteString(_T("Could not establish a connection with the server..."));    
    }

	return true;
}
