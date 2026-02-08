// DlgToolbar.cpp : implementation file
//

#include "stdafx.h"
#include "multichart.h"
#include "DlgToolbar.h"

#include "./AnalTool/ChartToolBtn.h"
#include "./AnalTool/FileBitmap.h"
#include "./Control/PopupMenu.h"

#include "DlgSettingToolBar.h"

#include "define\ChartMsgDef.h"
#include "StdDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgToolbar dialog
extern HINSTANCE g_hInstance;
/////////////////////////////////////////////////////////////////////////////
// CDlgToolbar dialog
#define UDM_CHARTBUTTON_CLICK	WM_USER + 602
#define UDM_CHARTBUTTON_TOOLTIP	WM_USER + 603
#define WMU_ALGIN_CHART			WM_USER	+ 1043

#define TOOL_BUTTON_BASE_ID		2000
#define TOOL_BUTTON_END_ID		2031

#define TOOL_BUTTON_WIDTH		28
#define TOOL_BUTTON_HEIGHT		22
#define MOUSE_POINT_ID		10

#define Y_GAP		 4
#define X_OFFSET	2		// Left/Right margin from left/right dialg border to button's left/right face
#define Y_OFFSET	2		// Top margin from top dlg's border to button's top face
#define Y_OFFSET2	4		// Top margin from top dlg's border to button's top face

enum {WON_TICK, PERCENT_TICK, DOLLAR_TICK }; //원 단위, 백분율 단위, 달러 단위

BOOL g_bToggle_Bar[NUM_BUTTONS] = {
	FALSE,  FALSE, FALSE,  FALSE, TRUE, TRUE,  TRUE,  TRUE,  TRUE, TRUE,	//0~9
	TRUE,  TRUE, TRUE,  TRUE, TRUE, TRUE, TRUE,  TRUE,  TRUE, TRUE,			//10~19
	TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, TRUE,		//20~29
	TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,		//30~39
	TRUE, TRUE, FALSE, TRUE, FALSE, FALSE, TRUE, TRUE, TRUE, TRUE,			//40~
	TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,			//50~
	TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE,			//60~
	FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,			//70~
};

CDlgToolbar::CDlgToolbar(CWnd* pParent /*=NULL*/)
	: CRscDlg(CDlgToolbar::IDD, pParent)
{

	// (2008/9/13 - Seung-Won, Bae) for ClassWizard
	for(int i = 0; i < NUM_BUTTONS; i++)
	{
		m_pToolBtn[i] = NULL;
	}
	//{{AFX_DATA_INIT(CDlgToolbar)
		// NOTE: the ClassWizard will add member initialization here
	m_pMultiChartWnd = pParent;
	m_hWhiteBrush = ::CreateSolidBrush(RGB(255, 255, 255));
	m_pCurToggleBtn = NULL;
	m_bShowScale = FALSE;
	m_bShowInnerToolbar = FALSE;
	m_nYUnitType = WON_TICK;
	m_bCheckShowData = FALSE;
	m_nSelTool = 0;
	//}}AFX_DATA_INIT

	//20070104 by 정유진
	//윈도우 크기 변경시 툴바의 마지막 버튼이 화면보다 길게 배치된경우 상/하위 메뉴보기 버튼으로 변경
	//초기화
	m_drawinfo.nPreWindowPos = 0;
	m_drawinfo.nCurrWindowPos = 0;
	m_drawinfo.nRealMenuCount = 0;
	m_drawinfo.nCalcMenuCount = 0;
	m_drawinfo.nHighMenuCount = 0;
	m_drawinfo.nLowMenuCount = 0;
	m_drawinfo.nLastBtnPos = 0;
	m_drawinfo.nLowBojo = -1;
	m_drawinfo.nLowLastIndex = -1;
	m_drawinfo.nHighBojo = -1;
	m_drawinfo.nHighLastIndex = -1;
	m_drawinfo.nTotBojo = -1;
	m_nResizeFlag = 0;

	m_pBtnUp = NULL;
	m_pBtnDown = NULL;
	m_bInit = FALSE;
	//by 정유진 end

	m_pGateManager = NULL;
}

CDlgToolbar::~CDlgToolbar()
{
	// delete gdi
	if(m_hWhiteBrush)
		::DeleteObject(m_hWhiteBrush);

	// delete font
	if(m_BtnFont.GetSafeHandle())
		m_BtnFont.DeleteObject();

	// delete button
	for(int i = 0; i < NUM_BUTTONS; i++)
	{
		if(m_pToolBtn[i])
			delete m_pToolBtn[i];
	}
	//{{ delete UpDown button
	if (m_pBtnUp) 
	{
		m_pBtnUp->DestroyWindow();
		delete m_pBtnUp;
	}

	if (m_pBtnDown) 
	{
		m_pBtnDown->DestroyWindow();
		delete m_pBtnDown;
	}
	m_ImgList.DeleteImageList();
	//}}
}

void CDlgToolbar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgToolbar)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgToolbar, CDialog)
	//{{AFX_MSG_MAP(CDlgToolbar)
		// NOTE: the ClassWizard will add message map macros here
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgToolbar message handlers
BOOL CDlgToolbar::OnInitDialog() 
{
	CDialog::OnInitDialog();

	AfxGetIGateManager(m_pGateManager);

	if(m_pMultiChartWnd)
	{
		CStdDialog* pMainDlg = (CStdDialog*)m_pMultiChartWnd;
//		m_pChartMng = pMainDlg->m_ChartMng;

		m_pIL_W28H22 = &(pMainDlg->m_ImgList_W28H22); //폭 28에 높이 22인 버튼 이미지리스트
	}

	// TODO: Add extra initialization here
	ReadIniFile();
	if(!CreateToolButton())
		return FALSE;

	ResizeControl(25, 0);
	m_bInit = TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgToolbar::CreateToolButton()
{
	// create tooltip
// --> [Edit]  강지원 2008/11/18		( 독립실행시 툴팁이 가려지는 현상때문에 최상위 핸들을 준다. )
	if(!m_ToolTip.Create(this, TTS_ALWAYSTIP))
//	if(!m_ToolTip.Create(&CWnd::wndTopMost, TTS_ALWAYSTIP))
		return FALSE;
// <-- [Edit]  강지원 2008/11/18

	m_ToolTip.ModifyStyleEx( 0, WS_EX_TOPMOST );
	m_ToolTip.SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_HIDEWINDOW);
	m_ToolTip.Activate(TRUE);

	LOGFONT lf;
	::memset(&lf, 0x00, sizeof(LOGFONT));
	lstrcpy(lf.lfFaceName, _T("굴림체"));
	lf.lfHeight = 12;
	lf.lfWeight = FW_NORMAL;
	lf.lfCharSet = HANGUL_CHARSET;
	
	if(!m_BtnFont.CreateFontIndirect(&lf))
	{
		TRACE("%s[%d] : Tool Button용 font 생성 실패!", __FILE__, __LINE__);
		return FALSE;
	}

	m_nCursorIndex = 1;

	CStdDialog* pMainDlg = (CStdDialog*)m_pMultiChartWnd;
	CString strImageFolderPath = pMainDlg->m_szImageDir+"\\";
	CString strButtonName, strUpFile, strDownFile;
	int nKeyInx=0;

	m_pToolBtn[nKeyInx] = new CChartToolBtn;
	if(!m_pToolBtn[nKeyInx])
		return FALSE;

	if(!m_pToolBtn[nKeyInx]->CreateBtn(this, &m_BtnFont, TOOL_BUTTON_BASE_ID + nKeyInx))
		return FALSE;
	
	strButtonName = "툴선택";
	strButtonName.Replace(" ", "");

	m_pToolBtn[nKeyInx]->SetValueBtn(GetToolOption(TOOL_BUTTON_BASE_ID + nKeyInx), 
							   g_bToggle_Bar[nKeyInx], m_pIL_W28H22, nKeyInx*3, nKeyInx*3+1, nKeyInx*3+2);

	m_ToolTip.AddTool(m_pToolBtn[nKeyInx], strButtonName);

	nKeyInx=3;

	m_pToolBtn[nKeyInx] = new CChartToolBtn;
	if(!m_pToolBtn[nKeyInx])
		return FALSE;

	if(!m_pToolBtn[nKeyInx]->CreateBtn(this, &m_BtnFont, TOOL_BUTTON_BASE_ID + nKeyInx))
		return FALSE;
	
	strButtonName = "차트툴편집";
	strButtonName.Replace(" ", "");

	m_pToolBtn[nKeyInx]->SetValueBtn(GetToolOption(TOOL_BUTTON_BASE_ID + nKeyInx), 
							   g_bToggle_Bar[nKeyInx], m_pIL_W28H22, nKeyInx*3, nKeyInx*3+1, nKeyInx*3+2);
	m_ToolTip.AddTool(m_pToolBtn[nKeyInx], strButtonName);
	
	for(int i = 0; i < m_arrayTotalIdx.GetSize() ; i++)
	{
		nKeyInx = m_arrayTotalIdx.GetAt(i);

		m_pToolBtn[nKeyInx] = new CChartToolBtn;
		if(!m_pToolBtn[nKeyInx])
			return FALSE;

		if(!m_pToolBtn[nKeyInx]->CreateBtn(this, &m_BtnFont, TOOL_BUTTON_BASE_ID + nKeyInx))
			return FALSE;

		m_mapToolName.Lookup(nKeyInx, strButtonName);

		strButtonName.Replace(" ", "");

		m_pToolBtn[nKeyInx]->SetValueBtn(GetToolOption(TOOL_BUTTON_BASE_ID + nKeyInx), 
			g_bToggle_Bar[nKeyInx], m_pIL_W28H22, nKeyInx*3, nKeyInx*3+1, nKeyInx*3+2);

		m_ToolTip.AddTool(m_pToolBtn[nKeyInx], strButtonName);
	}

	//20070104 by 정유진
	//윈도우 크기 변경시 툴바의 마지막 버튼이 화면보다 길게 배치된경우 상/하위 메뉴보기 버튼으로 변경 
	//방향 이미지 읽기
	CBitmap			bitmap;
	CString			strImgPath;

	strImgPath.Format("%s\\toolbar_move.bmp",pMainDlg->m_szImageDir);
	m_ImgList.Create(28,12,ILC_MASK | ILC_COLORDDB, 1, 1);
	bitmap.Attach((HBITMAP)::LoadImage(NULL, (LPSTR)(LPCTSTR)strImgPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));	
	m_ImgList.Add(&bitmap, RGB(255,0,255));
	bitmap.DeleteObject();

	//상위버튼 추가
	if(m_pBtnUp == NULL)
	{
		m_pBtnUp = new CILButton;
		m_pBtnUp->Create("", WS_VISIBLE | WS_CHILD | WS_TABSTOP, CRect(0,0,0,0), this, TOOL_BUTTON_BASE_ID+200);
		m_pBtnUp->FP_vSetImage( &m_ImgList, 4, 6, FALSE, "", FALSE, 7);
		m_pBtnUp->FP_vSetToolTipText("상위버튼보기");
	}
	//하위버튼 추가
	if(m_pBtnDown == NULL)
	{
		m_pBtnDown = new CILButton();
		m_pBtnDown->Create("", WS_VISIBLE | WS_CHILD | WS_TABSTOP, CRect(0,0,0,0), this, TOOL_BUTTON_BASE_ID+201);
		m_pBtnDown->FP_vSetImage( &m_ImgList, 0, 2, FALSE, "", FALSE, 3);
		m_pBtnDown->FP_vSetToolTipText("하위버튼보기");
	}
	//by 정유진 end
	return TRUE;
}

//-----------------------------------------------------------------------------
// Author		: LYH Date :2006.09.15
// Modifier		: 
// Comments		: 설정된 파일을 읽어온다
//-----------------------------------------------------------------------------
void CDlgToolbar::ReadIniFile(BOOL bReload /*=FALSE*/)
{
	//m_mapToolName.RemoveAll();
	m_arrayTotalIdx.RemoveAll();
	m_arrayAnalIdx.RemoveAll();
	m_arrayAdditionIdx.RemoveAll();
	m_arrayUserIdx.RemoveAll();

	CStdDialog* pMainDlg = (CStdDialog*)m_pMultiChartWnd;
	CString strImageFolderPath = pMainDlg->m_szImageDir+"\\";

	CFileFind FileFind;
	CString strPath, strDataPath, strUserPath, strConfigPath, strTemp;
	char szKey[30];

	IMainInfoManager* pMainInfoManager = (IMainInfoManager*)m_pGateManager->GetInterface(UUID_IMainInfoManager);

	//# Data Folder
	strPath = pMainInfoManager->GetDataDir();
	strDataPath.Format("%s\\%s", strPath, "ChartToolBarSetting.dat");
	//# User Folder
	strPath = pMainDlg->m_strUserDir;
	strUserPath.Format("%s\\%s", strPath, "ChartToolBarSetting.dat");	

	//# Version확인
	int nUserCnt, nDataCnt, nUserVer, nDataVer;

	// 분석툴 이름과 기능이 상이한거 수정 - ojtaso (20070827)
	BOOL bUserFile = FileFind.FindFile(strUserPath);
	if (!bUserFile)		//# Data 폴더
	{
		strTemp = strConfigPath = strDataPath;		
	}
	else										//# User 폴더
	{
		sprintf(szKey,"%s","TOTAL");
		//# Count
		nUserCnt	= ::GetPrivateProfileInt(szKey, "COUNT", -1, strDataPath);
		nDataCnt	= ::GetPrivateProfileInt(szKey, "COUNT", -1, strUserPath);
		//# Version
		nUserVer = ::GetPrivateProfileInt(szKey, "VERSION", -1, strDataPath);	
		nDataVer = ::GetPrivateProfileInt(szKey, "VERSION", -1, strUserPath);

		if(nUserVer != nDataVer)
			strConfigPath = strDataPath;
		else // 버전일치
			strConfigPath = strUserPath;
	}

	CString szIndex, szColNum, szColInfo, szFieldKey, szFieldName;
	int nColCount, nFieldKey;;

	//************************************************
	// 전체 리스트
	//************************************************
	sprintf(szKey,"%s","TOTAL LIST");
	nColCount = ::GetPrivateProfileInt(szKey, "COUNT", -1, strDataPath);

	for(int i=0;i < nColCount; i++)
	{
		szColNum.Format("%02d", i);
		::GetPrivateProfileString(szKey, szColNum, "", szColInfo.GetBuffer(MAX_PATH), MAX_PATH, strDataPath);	
		szColInfo.ReleaseBuffer();

		if (szColInfo.GetLength() == 0)
			continue;

		GetFieldData(szColInfo, nFieldKey, szFieldName);

		if(!bReload)
			m_mapToolName.SetAt(nFieldKey, szFieldName);
		m_arrayTotalIdx.Add(nFieldKey);
	}

	//************************************************
	// 분석도구 리스트
	//************************************************
	sprintf(szKey,"%s","ANAL LIST");
	nColCount = ::GetPrivateProfileInt(szKey, "COUNT", -1, strConfigPath);

	for(i=0;i < nColCount; i++)
	{
		szColNum.Format("%02d", i);
		::GetPrivateProfileString(szKey, szColNum, "", szColInfo.GetBuffer(MAX_PATH), MAX_PATH, strConfigPath);	
		szColInfo.ReleaseBuffer();
	
		if (szColInfo.GetLength() == 0)
			continue;

		GetFieldData(szColInfo, nFieldKey, szFieldName);

		if(!bReload)
			m_mapToolName.SetAt(nFieldKey, szFieldName);
		m_arrayAnalIdx.Add(nFieldKey);
	}
	
		
	sprintf(szKey,"%s","ADDITION LIST");
	nColCount = ::GetPrivateProfileInt(szKey, "COUNT", -1, strConfigPath);

	for(i=0;i < nColCount; i++)
	{
		szColNum.Format("%02d", i);
		::GetPrivateProfileString(szKey, szColNum, "", szColInfo.GetBuffer(MAX_PATH), MAX_PATH, strConfigPath);	
		szColInfo.ReleaseBuffer();
		
		if (szColInfo.GetLength() == 0)
			continue;

		GetFieldData(szColInfo, nFieldKey, szFieldName);

		if(!bReload)
			m_mapToolName.SetAt(nFieldKey, szFieldName);
		m_arrayAdditionIdx.Add(nFieldKey);
	}
	
	sprintf(szKey,"%s","USER LIST");
	nColCount = ::GetPrivateProfileInt(szKey, "COUNT", -1, strConfigPath);

	for(i=0;i < nColCount; i++)
	{
		szColNum.Format("%02d", i);
		::GetPrivateProfileString(szKey, szColNum, "", szColInfo.GetBuffer(MAX_PATH), MAX_PATH, strConfigPath);	
		szColInfo.ReleaseBuffer();
	
		if (szColInfo.GetLength() == 0)
			continue;

		GetFieldData(szColInfo, nFieldKey, szFieldName);

		m_arrayUserIdx.Add(nFieldKey);
	}

//	//20070105 by 정유진
//	//윈도우 크기 변경시 툴바의 마지막 버튼이 화면보다 길게 배치된경우 상/하위 메뉴보기 버튼으로 변경 
//	sprintf(szKey,"%s","TOOLHEIGHT OVER");
//	nColCount = ::GetPrivateProfileInt(szKey, "COUNT", -1, strConfigPath);
//
//	//해당 필드가 없는 경우 파일에 기록 후 정보를 읽어옴
//	if (nColCount <= 0)						
//	{
//		CString strFieldName, strValue,str;
//		int nCnt;
//
//		nCnt = 2;
//
//		sprintf(szKey,"%s","TOOLHEIGHT OVER");
//
//		strValue.Format("%d",nCnt);
//		::WritePrivateProfileString(szKey, "COUNT", strValue, strConfigPath);
//
//		for (i=0; i < nCnt ; i++)
//		{ 
//			if (i==0)
//			{
//				nFieldKey = 49;
//				strFieldName = "하위메뉴보기";
//			}
//			else 
//			{
//				nFieldKey = 50;
//				strFieldName = "상위메뉴보기";
//			}
//
//			strValue.Format("%d,%s", nFieldKey, strFieldName);
//
//			str.Format("%02d", i);
//			::WritePrivateProfileString(szKey, str, strValue, strConfigPath);
//		}	
//
//		//기록된 정보 read
//		for(i=0;i < nColCount; i++)
//		{
//			szColNum.Format("%02d", i);
//			::GetPrivateProfileString(szKey, szColNum, "", szColInfo.GetBuffer(MAX_PATH), MAX_PATH, strConfigPath);	
//			szColInfo.ReleaseBuffer();
//
//			if (szColInfo.GetLength() == 0)
//				continue;
//
//			GetFieldData(szColInfo, nFieldKey, szFieldName);
//
//			if(!bReload)
//				m_mapToolName.SetAt(nFieldKey, szFieldName);
//		}
//	}
//	else
//	{
//		for(i=0;i < nColCount; i++)
//		{
//			szColNum.Format("%02d", i);
//			::GetPrivateProfileString(szKey, szColNum, "", szColInfo.GetBuffer(MAX_PATH), MAX_PATH, strConfigPath);	
//			szColInfo.ReleaseBuffer();
//
//			if (szColInfo.GetLength() == 0)
//				continue;
//
//			GetFieldData(szColInfo, nFieldKey, szFieldName);
//
//			if(!bReload)
//				m_mapToolName.SetAt(nFieldKey, szFieldName);
//		}
//	}
//	//by 정유진 end
//
//	if(bUserFile)
//	{
//		if(m_mapToolName.Lookup(52, szFieldName))
//			m_mapToolName.SetAt(52, _T("가속저항호"));
//
//		if(m_mapToolName.Lookup(53, szFieldName))
//			m_mapToolName.SetAt(53, _T("가속저항팬"));
//	}
}

//-----------------------------------------------------------------------------
// Author		: Hui-Eun, Lee	Date :2006.9.11
// Modifier		: 
// Parameter	:	strFieldData	: 파일데이터
//					nFieldKey		: 리스트인덱스
//					strFieldName	: 리스트항목
// Comments		:  파일데이터를 읽어서 parsing
//-----------------------------------------------------------------------------
void CDlgToolbar::GetFieldData(CString strFieldData, int &nFieldKey, CString &strFieldName)
{
	CString strFieldKey;

	int nPos = strFieldData.Find(',');
	if(nPos==-1)	return;		
	strFieldKey = strFieldData.Left(nPos);			//필드키
	nFieldKey = atoi(strFieldKey);
	strFieldData.Delete(0,nPos+1);		

	strFieldName = strFieldData;			//필드명
}

CToolOptionInfo::TOOLOPTION CDlgToolbar::GetToolOption(UINT nId)
{
	nId = nId - TOOL_BUTTON_BASE_ID;
	switch(nId)
	{
	case  1: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION;			// 추세선
	case  2: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION;			// 연장추세선
	case  3: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION;			// 십자선
	case  4: return CToolOptionInfo::TOOLOPTION::T_SYMBOL_BULET;			// 그리기 모음

	case  5: return CToolOptionInfo::TOOLOPTION::T_TOOL_LINE_CROSS;			// 십자선
	case  6: return CToolOptionInfo::TOOLOPTION::T_TOOL_LINE_HORZ;			// 수평선
	case  7: return CToolOptionInfo::TOOLOPTION::T_TOOL_LINE_VERT;			// 수직선
	case  8: return CToolOptionInfo::TOOLOPTION::T_TOOL_LINE;				// 추세선
	case  9: return CToolOptionInfo::TOOLOPTION::T_TOOL_PENCIL_LINE;			// 연필 추세선

	case 10: return CToolOptionInfo::TOOLOPTION::T_TOOL_ELLIOT_WAVE_LINE;	// 앨리어트파동선
	case 11: return CToolOptionInfo::TOOLOPTION::T_SYMBOL_SQUARE;			// 네모
	case 12: return CToolOptionInfo::TOOLOPTION::T_SYMBOL_CIRCLE;			// 원
	case 13: return CToolOptionInfo::TOOLOPTION::T_SYMBOL_TEXT;				// 텍스트
	case 14: return CToolOptionInfo::TOOLOPTION::T_TOOL_ANDREWS_PITCHFORK;	// 앤드류피치포크

	case 15: return CToolOptionInfo::TOOLOPTION::T_TOOL_FIBO_RETRACE;		// 존추세선
	case 16: return CToolOptionInfo::TOOLOPTION::T_TOOL_SPEED_FAN;			// 팬추세선
	case 17: return CToolOptionInfo::TOOLOPTION::T_TOOL_FIBONACCI_TIME;		// 피보나치수열
	case 18: return CToolOptionInfo::TOOLOPTION::T_TOOL_FIBONACCI_ARC;		// 활추세선
	case 19: return CToolOptionInfo::TOOLOPTION::T_TOOL_FIBONACCI_FAN;		// 피보나치팬

	case 20: return CToolOptionInfo::TOOLOPTION::T_TOOL_GANN_LINE45;		// 우상갠
	case 21: return CToolOptionInfo::TOOLOPTION::T_DELETE_INORDER;			// 단일 지우개
	case 22: return CToolOptionInfo::TOOLOPTION::T_DELETE_ALL;				// 전체 지우개

	case 23: return CToolOptionInfo::TOOLOPTION::T_RESTORE;					// 전체초기화
	//20080915 이문수 >>
	//case 24: return CToolOptionInfo::TOOLOPTION::T_ENVIRONMENT_CFG;			// 환경설정
	case 24: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION;			// 환경설정
	//20080915 이문수 <<
	case 25: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION;			// 이평셋
	case 26: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION;			// 공시
	case 27: return CToolOptionInfo::TOOLOPTION::T_PRICE_YSCALE_SETUP;		// Y축설정

	case 28: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION ;			// 수치조회(창)
	case 29: return CToolOptionInfo::TOOLOPTION::T_TOOL_TRACEING_TT;		// 전체데이터보기
	case 30: return CToolOptionInfo::TOOLOPTION::T_TOOL_PERIOD_SUMMARY ;	// 기간요약
//삼성
//	case 28: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION ;			// 수치조회
//	case 29: return CToolOptionInfo::TOOLOPTION::T_TOOL_TRACEING_TT;		// 전체데이터보기
//	case 30: return CToolOptionInfo::TOOLOPTION::T_TOOL_PERIOD_SUMMARY;		// 기간요약

	case 31: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION;			// 자동추세선
	case 32: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION;			// 추세패턴
	case 33: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION;			// 캔들패턴
	case 34: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION;			// 패턴통계 
	case 35: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION;			// 패턴설정

	case 36: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION;			// 저장
	case 37: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION;			// 불러오기
	case 38: return CToolOptionInfo::TOOLOPTION::T_PRINT_CHART;				// 화면출력
	case 39: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION;			// Excel변환

	case 40: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION;			// 눈금영역숨김
	case 41: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION;			// 스크롤숨기기
	case 42: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION;			// 일주월분
	case 43: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION;			// 수치조회하단
	case 44: return CToolOptionInfo::TOOLOPTION::T_DATALIST_REPORT;			// 텍스트윈도우
	case 45: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION;			// 기준선
	case 46: return CToolOptionInfo::TOOLOPTION::T_ORDER_LINE;				// 주문선
	case 47: return CToolOptionInfo::TOOLOPTION::T_TOOL_GANN_FAN45;			// 상향갠팬
	case 48: return CToolOptionInfo::TOOLOPTION::T_TOOL_GANN_FAN315;		// 하향갠팬
	case 49: return CToolOptionInfo::TOOLOPTION::T_TOOL_ANGLE_LINE;			// 각도추세선
	case 50: return CToolOptionInfo::TOOLOPTION::T_SYMBOL_TRIANGLE;			// 세모
		//case 51: return CToolOptionInfo::TOOLOPTION::T_TOOL_ANGLE_GRID;			// 갠앵글
	case 51: return CToolOptionInfo::TOOLOPTION::T_TOOL_GANN_GRID;			// 갠앵글
	case 52: return CToolOptionInfo::TOOLOPTION::T_TOOL_SPEED_ARC;			// 가속저항호
	case 53: return CToolOptionInfo::TOOLOPTION::T_TOOL_SPEED_LINE;			// 가속저항팬
	case 54: return CToolOptionInfo::TOOLOPTION::T_TOOL_LINE_TRISECT;		// 삼등분선
	case 55: return CToolOptionInfo::TOOLOPTION::T_TOOL_LINE_QUARTER;		// 사등분선
	case 56: return CToolOptionInfo::TOOLOPTION::T_TOOL_CYCLE_LINES;		// 사이클구간
	case 57: return CToolOptionInfo::TOOLOPTION::T_TOOL_ADJUST_RATIO;		// 조정백분율
	case 58: return CToolOptionInfo::TOOLOPTION::T_TOOL_LINEAR_REGRESSION;	// 직선회귀선
	case 59: return CToolOptionInfo::TOOLOPTION::T_TOOL_RAFF_REGRESSION;	// 직선회귀채널

	case 60: return CToolOptionInfo::TOOLOPTION::T_DRAG_Y_ZOOM;				// Y축확대
	case 61: return CToolOptionInfo::TOOLOPTION::T_PAN;						// 차트이동
//	case 62: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION;			// 부분확대
	case 63: return CToolOptionInfo::TOOLOPTION::T_TOOL_CANDLE_LINE;		// 봉중심선
	case 64: return CToolOptionInfo::TOOLOPTION::T_TOOL_CANDLE_BONG;		// 가상봉
	case 65: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION;			// 복사
	case 66: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION;			// 원클릭숨기기
	case 67: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION;			// 시간선
	case 68: return CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION;			// 거래량이평
	case 69: return CToolOptionInfo::TOOLOPTION::T_EXCEL_IMPORT;			// Excel불러오기
	case 70: return CToolOptionInfo::TOOLOPTION::T_UNDO_WORK;				// Undo
	case 71: return CToolOptionInfo::TOOLOPTION::T_REDO_WORK;				// Redo
	case 72: return CToolOptionInfo::TOOLOPTION::T_ORDER_LINE_MANAGER;		// 주문선관리
	case 73: return CToolOptionInfo::TOOLOPTION::T_MAGNIFYINGGLASS;			// 돋보기
	default: return CToolOptionInfo::TOOLOPTION(-1);
	}
	
	return CToolOptionInfo::TOOLOPTION(-1);
}

void CDlgToolbar::OnSize(UINT nType, int cx, int cy) 
{
	if(!m_bInit)
		return;

	if(cy > 0)
		ResizeToolbar(cy);

	CDialog::OnSize(nType, cx, cy);
}

void CDlgToolbar::ResizeControl(int cx, int cy)
{
	// ASSUME : index 0's handle is safe, remainant button's handles are also safe
	if(!m_pToolBtn[0]->GetSafeHwnd())
		return;

	int nBottom = 0;
	//int nGap = (cx - (5 * TOOL_BUTTON_WIDTH + 2 * X_OFFSET)) / 4;	// button gap
	int nGap = 0;
	RECT rect;

	rect.left = X_OFFSET;
	rect.top = Y_OFFSET;
	nBottom = rect.top + TOOL_BUTTON_HEIGHT;
	rect.right = rect.left + TOOL_BUTTON_WIDTH;
	rect.bottom = rect.top + TOOL_BUTTON_HEIGHT;
	m_pToolBtn[0]->MoveWindow(&rect);

	int nKeyIdx;
	rect.left = 0;
	rect.top = 0;
	rect.right = 0;
	rect.bottom = 0;
	for(int i = 3; i < m_arrayTotalIdx.GetSize()+5; i++)
	{
		if(m_pToolBtn[i])
			m_pToolBtn[i]->MoveWindow(&rect);
	}
	int nKeyInx;
	if(m_nSelTool == 0)
	{
		nKeyInx = m_nSelTool;
		m_pToolBtn[0]->SetValueBtn(GetToolOption(TOOL_BUTTON_BASE_ID + 0), 
							   g_bToggle_Bar[0], m_pIL_W28H22, nKeyInx*3, nKeyInx*3+1, nKeyInx*3+2);

		for(int i = 0; i < m_arrayUserIdx.GetSize(); i++)
		{
			rect.left = X_OFFSET;
			rect.top = Y_OFFSET2 + (i+1) * TOOL_BUTTON_HEIGHT + (i+1) * nGap;
			nBottom = rect.top + TOOL_BUTTON_HEIGHT;
			rect.right = rect.left + TOOL_BUTTON_WIDTH;
			rect.bottom = rect.top + TOOL_BUTTON_HEIGHT;

			nKeyIdx = m_arrayUserIdx.GetAt(i);
			if(m_pToolBtn[nKeyIdx])
				m_pToolBtn[nKeyIdx]->MoveWindow(&rect);
		}
	}
	else if(m_nSelTool == 1)
	{
		nKeyInx = m_nSelTool;
		m_pToolBtn[0]->SetValueBtn(GetToolOption(TOOL_BUTTON_BASE_ID + 0), 
							   g_bToggle_Bar[0], m_pIL_W28H22, nKeyInx*3, nKeyInx*3+1, nKeyInx*3+2);

		for(int i = 0; i < m_arrayAnalIdx.GetSize(); i++)
		{
			rect.left = X_OFFSET;
			rect.top = Y_OFFSET2 + (i+1) * TOOL_BUTTON_HEIGHT + (i+1) * nGap;
			nBottom = rect.top + TOOL_BUTTON_HEIGHT;
			rect.right = rect.left + TOOL_BUTTON_WIDTH;
			rect.bottom = rect.top + TOOL_BUTTON_HEIGHT;

			nKeyIdx = m_arrayAnalIdx.GetAt(i);
			if(m_pToolBtn[nKeyIdx])
				m_pToolBtn[nKeyIdx]->MoveWindow(&rect);
		}
	}
	else if(m_nSelTool == 2)
	{
		nKeyInx = m_nSelTool;
		m_pToolBtn[0]->SetValueBtn(GetToolOption(TOOL_BUTTON_BASE_ID + 0), 
							   g_bToggle_Bar[0], m_pIL_W28H22, nKeyInx*3, nKeyInx*3+1, nKeyInx*3+2);
		for(int i = 0; i < m_arrayAdditionIdx.GetSize(); i++)
		{
			rect.left = X_OFFSET;
			rect.top = Y_OFFSET2 + (i+1) * TOOL_BUTTON_HEIGHT + (i+1) * nGap;
			nBottom = rect.top + TOOL_BUTTON_HEIGHT;
			rect.right = rect.left + TOOL_BUTTON_WIDTH;
			rect.bottom = rect.top + TOOL_BUTTON_HEIGHT;

			nKeyIdx = m_arrayAdditionIdx.GetAt(i);
			if(m_pToolBtn[nKeyIdx])
				m_pToolBtn[nKeyIdx]->MoveWindow(&rect);
		}
	}

	//20070104 by 정유진
	//윈도우 크기 변경시 툴바의 마지막 버튼이 화면보다 길게 배치된경우 상/하위 메뉴보기 버튼으로 변경 
	//가장 마지막 버튼의 rect.bottom 저장
	m_drawinfo.nLastBtnPos = rect.bottom;
	//by 정유진 end
}

BOOL CDlgToolbar::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_MOUSEMOVE)   
	{
		if ( m_ToolTip.GetSafeHwnd() )
		{
			m_ToolTip.SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOOWNERZORDER);
			m_ToolTip.RelayEvent(pMsg);
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CDlgToolbar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	//<<20100326_JS.Kim 솔로몬. 툴바버튼 툴팁처리.. Pretranslate Message가 들어오지 않는다...
	if(message == UDM_CHARTBUTTON_TOOLTIP)
	{
		m_ToolTip.RelayEvent((MSG *)lParam);	
	}
	//>>
	else if(message == UDM_CHARTBUTTON_CLICK)
	{
		if(wParam == 3)	//Not Toggle Button Up
		{
			CWnd* wndButton = (CWnd *)lParam;
			int nIdx = wndButton->GetDlgCtrlID() - TOOL_BUTTON_BASE_ID;
			if(nIdx == 0)
			{
				m_nSelTool += 1;
				if(m_nSelTool>2)
					m_nSelTool = 0;
				//ResizeControl(25, 0);
				//20070105 by 정유진
				//윈도우 크기 변경시 툴바의 마지막 버튼이 화면보다 길게 배치된경우 상/하위 메뉴보기 버튼으로 변경 
				if (m_nResizeFlag == 1)
				{
					m_nResizeFlag = 0;
				}

				m_pBtnUp->ShowWindow(FALSE);
				m_pBtnDown->ShowWindow(FALSE);
				//by 정유진 end

				ResizeControl(25, 0);

				//20070105 by 정유진
				//윈도우 크기 변경시 툴바의 마지막 버튼이 화면보다 길게 배치된경우 상/하위 메뉴보기 버튼으로 변경 
				CRect rt;
				GetClientRect( &rt );

				ResizeToolbar(rt.bottom);
				////by 정유진 end
			}
			else if(nIdx == 3)
			{
				AFX_MANAGE_STATE(AfxGetStaticModuleState());
				//HINSTANCE hInst = AfxGetResourceHandle();
				//AfxSetResourceHandle(g_hInstance);	

				CDlgSettingToolBar dlg;
				int nRet = dlg.DoModal();

				if(nRet==IDOK)
				{
					ReadIniFile(TRUE);
					//20070105 by 정유진
					//윈도우 크기 변경시 툴바의 마지막 버튼이 화면보다 길게 배치된경우 상/하위 메뉴보기 버튼으로 변경 
					if (m_nResizeFlag == 1)
					{
						m_nResizeFlag = 0;
					}

					m_pBtnUp->ShowWindow(FALSE);
					m_pBtnDown->ShowWindow(FALSE);
					ResizeControl(25, 0);
					//20070105 by 정유진
					//윈도우 크기 변경시 툴바의 마지막 버튼이 화면보다 길게 배치된경우 상/하위 메뉴보기 버튼으로 변경 
					CRect rt;
					GetClientRect( &rt );

					ResizeToolbar(rt.bottom);
					////by 정유진 end
				}
				//AfxSetResourceHandle(hInst);
			}
			if(nIdx > 3)
			{
				CString strToolName = "";
				m_mapToolName.Lookup(nIdx, strToolName);

				if(strToolName == "저장")
				{
				//	CWnd* pWnd = GetDlgItem((int)lParam);
					CRect rect;
					CPoint pt;
					wndButton->GetWindowRect(&rect); 
					pt.x = rect.left;
					pt.y = rect.bottom - 250;

					if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
						m_pChartMng->SendMessage(UMSG_SAVECHART,0,(LPARAM)&pt);	
					SelectToolSelection();
				}
				else if(strToolName == "불러오기")
				{
				//	CWnd* pWnd = GetDlgItem((int)lParam);
					CRect rect;
					CPoint pt;
					wndButton->GetWindowRect(&rect); 

					pt.x = rect.left;
					pt.y = rect.bottom;

					if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
						m_pChartMng->SendMessage(UMSG_LOADCHART,0,(LPARAM)&pt);	
					SelectToolSelection();
				}
				else if(strToolName == "인쇄")
				{
					//::SendMessage(((CStdDialog*)m_pMultiChartWnd)->m_hMainFrame,WM_COMMAND, (WPARAM)ID_FILE_PRINT, 0);
					if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
						m_pChartMng->SendMessage(UMSG_CHART_PRINT,0,0);	
					SelectToolSelection();
				}
				else if(strToolName == "Excel변환")
				{
					if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
						m_pChartMng->SendMessage(UMSG_CHART_EXCEL,0,0);	
					SelectToolSelection();
				}
				else if(strToolName == "자동추세선")
				{
					if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
						m_pChartMng->SendMessage(UMSG_ANALYSISPATTERN, 4);	
					//SelectToolSelection();
				}
				else if(strToolName == "추세패턴")
				{
					if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
						m_pChartMng->SendMessage(UMSG_ANALYSISPATTERN, 5);	
					//SelectToolSelection();
				}
				else if(strToolName == "캔들패턴")
				{
					if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
						m_pChartMng->SendMessage(UMSG_ANALYSISPATTERN, 6);	
					//SelectToolSelection();
				}
				else if(strToolName == "패턴통계")
				{
					if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
						m_pChartMng->SendMessage(UMSG_ANALYSISPATTERN, 7);	
					SelectToolSelection();
				}
				else if(strToolName == "패턴설정")
				{
					if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
						m_pChartMng->SendMessage(UMSG_ANALYSISPATTERN, 8);	
					SelectToolSelection();
				}

				//다음 처리인 RMS_ANALYSIS_FUNC메세지 핸들러에서 처리되고 있음
				else if(strToolName == "환경설정")
				{
					if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
						m_pChartMng->SendMessage(UMSG_SHOWENVIRDLG);	
					//SelectToolSelection();
				}

				else if(strToolName == "가격이평" || strToolName == "이평Set")
				{
					if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
						m_pChartMng->SendMessage(UMSG_SHOWINDICATORCONFIGDLG, 1);	
					//SelectToolSelection();
				}
				else if(strToolName == "거래량이평")
				{
					if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
						m_pChartMng->SendMessage(UMSG_SHOWINDICATORCONFIGDLG, 2);	
					//SelectToolSelection();
				}
				//{{ JS.Kim_20100901 solomon_74	일주월분 순서를 분일주월로 변경
				//else if(strToolName == "일주월분")
				else if(strToolName == "일주월분" || strToolName == "분일주월")
				//}}
				{
					if(m_pChartMng->GetSafeHwnd())
						m_pChartMng->SendMessage(UMSG_DWMM_CHART, NULL, TRUE);
				}
				else if(strToolName == "기준선") //기준선
				{
					if(m_pChartMng->GetSafeHwnd())
						m_pChartMng->SendMessage(UMSG_CHARTGUIDELINE, NULL, FALSE);
				}
				else if(strToolName == "수치조회하단")
				{
					m_bCheckShowData = !m_bCheckShowData;
					((CStdDialog*)m_pMultiChartWnd)->SendMessage(USMG_SHOW_DATAWND, 0, (LPARAM)m_bCheckShowData);
					if(!m_bCheckShowData)
						SelectToolSelection();
				}
				//test
//				else if(strToolName == "기간요약") //기간요약
//				{
//					if(m_pChartMng->GetSafeHwnd())
//						m_pChartMng->SendMessage(UMSG_PERIOD_SUMMARY, NULL, FALSE);
//				}
				else if(strToolName == "원클릭숨기기") //원클릭숨기기
				{
					((CStdDialog*)m_pMultiChartWnd)->SendMessage(UMSG_ONECLICK_HIDE, NULL, FALSE);
				}
				else if(strToolName == "눈금영역숨김")
				{
					m_bShowScale = !m_bShowScale;
					m_pChartMng->SendMessage(UMSG_SHOWSCALE,m_bShowScale);
					SelectToolSelection();
				}
				else if(strToolName == "스크롤숨기기")
				{
					m_bShowInnerToolbar = !m_bShowInnerToolbar;
					m_pChartMng->PostMessage(UMSG_SHOWINNERTOOLBAR,m_bShowInnerToolbar);
					SelectToolSelection();
				}
				//삼성
				//else if(strToolName == "공시")
				//{
				//	if(m_pChartMng->GetSafeHwnd())
				//		m_pChartMng->SendMessage(UMSG_NEWS, NULL, FALSE);
				//	//SelectToolSelection();
				//}
				else if(strToolName == "수치조회")
				{
					((CStdDialog*)m_pMultiChartWnd)->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_NIDLG_OVER, FALSE);
// --> [Edit]  강지원 2008/11/04	( 수치조회창 Popup 다이얼로그로 변경 )
					m_pChartMng->PostMessage(UMSG_CHART_INQUIRY_SHOW, SW_HIDE);
// <-- [Edit]  강지원 2008/11/04
				}
				else if(strToolName == "돋보기")
				{
					// 돋보기 구현 - ojtaso (20080519)
					((CStdDialog*)m_pMultiChartWnd)->OnMagnifyingglass(FALSE);
//					((CStdDialog*)m_pMultiChartWnd)->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_MAGNIFYINGGLASS, FALSE);
				}
			}
		}
		if(wParam == 1)		// button up
		{
			CWnd* pCurWnd = (CWnd *)lParam;

			int nSel = pCurWnd->GetDlgCtrlID() - TOOL_BUTTON_BASE_ID;

			CString strToolName = "";
			m_mapToolName.Lookup(nSel, strToolName);
			if(strToolName == "자동추세선")
			{
				if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
					m_pChartMng->SendMessage(UMSG_ANALYSISPATTERN, 1);	
				//SelectToolSelection();
			}
			else if(strToolName == "추세패턴")
			{
				if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
					m_pChartMng->SendMessage(UMSG_ANALYSISPATTERN, 2);	
				//SelectToolSelection();
			}
			else if(strToolName == "캔들패턴")
			{
				if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
					m_pChartMng->SendMessage(UMSG_ANALYSISPATTERN, 3);	
				//SelectToolSelection();
			}
//			else if(strToolName == "일주월분")
//			{
//				if(m_pChartMng->GetSafeHwnd())
//					m_pChartMng->SendMessage(UMSG_DWMM_CHART, NULL, TRUE);
//			}
			//test
//			else if(strToolName == "기간요약") //"기간요약"
//			{
//				if(m_pChartMng->GetSafeHwnd())
//					m_pChartMng->SendMessage(UMSG_PERIOD_SUMMARY, NULL, TRUE);
//			}
			else if(strToolName == "원클릭숨기기") //"원클릭숨기기"
			{
				((CStdDialog*)m_pMultiChartWnd)->SendMessage(UMSG_ONECLICK_HIDE, NULL, TRUE);
			}
			else if(strToolName == "눈금영역숨김")
			{
				m_bShowScale = !m_bShowScale;
				m_pChartMng->SendMessage(UMSG_SHOWSCALE,m_bShowScale);
				SelectToolSelection();
			}
			else if(strToolName == "스크롤숨기기")
			{
				m_bShowInnerToolbar = !m_bShowInnerToolbar;
				m_pChartMng->PostMessage(UMSG_SHOWINNERTOOLBAR,m_bShowInnerToolbar);
				SelectToolSelection();
			}
			else if(strToolName == "수치조회하단")
			{
				m_bCheckShowData = !m_bCheckShowData;
				((CStdDialog*)m_pMultiChartWnd)->SendMessage(USMG_SHOW_DATAWND, 0, (LPARAM)m_bCheckShowData);
				if(!m_bCheckShowData)
					SelectToolSelection();
			}
			//삼성
			//else if(strToolName == "공시")
			//{
			//	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
			//		m_pChartMng->SendMessage(UMSG_NEWS, NULL, TRUE);
			//	//SelectToolSelection();
			//}
			else if(strToolName == "수치조회")
			{
				((CStdDialog*)m_pMultiChartWnd)->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_NIDLG_OVER, TRUE);
// --> [Edit]  강지원 2008/11/04	( 수치조회창 Popup 다이얼로그로 변경 )
				m_pChartMng->PostMessage(UMSG_CHART_INQUIRY_SHOW, SW_SHOW);
// <-- [Edit]  강지원 2008/11/04
			}
			else if(strToolName == "돋보기")
			{
				// 돋보기 구현 - ojtaso (20080519)
				((CStdDialog*)m_pMultiChartWnd)->OnMagnifyingglass(TRUE);
//				((CStdDialog*)m_pMultiChartWnd)->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_MAGNIFYINGGLASS, TRUE);
			}
			else
			{
				m_pCurToggleBtn = pCurWnd;	
				((CStdDialog*)m_pMultiChartWnd)->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
			}
		}
		else if(wParam == 2)	// button down
		{	
			
			CChartToolBtn* pBtn = (CChartToolBtn *)m_pCurToggleBtn;

			int nID = (int)lParam;
			int nSel = nID - TOOL_BUTTON_BASE_ID;

			if(!m_pCurToggleBtn)
				return 0L;
			CString strToolName = "";
			m_mapToolName.Lookup(nSel, strToolName);

			if(m_pCurToggleBtn->GetDlgCtrlID() != nID)
			{

				if(pBtn && IsWindow(pBtn->m_hWnd))
				{
					if(GetToolOption(nID) != -1 && GetToolOption(nID) != CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION)
					{
						pBtn->ReleaseBtn();
					}
					// 2003. 2. 3 ReleaseBtn()의 중복실행을 방지..
					if(strToolName == _T("도형") ||strToolName == _T("기호") || strToolName == _T("라인종류 지정") || strToolName == _T("라인색 지정"))
						m_pCurToggleBtn = NULL;
				}		
			}
			strToolName.Replace(" ", "");
			if(strToolName == "Y축단위전환")
			{
				if( m_nYUnitType == WON_TICK)
				{
					m_nYUnitType = PERCENT_TICK;
					//m_btYUnit.SetWindowText("%");
				}
				else if(m_nYUnitType == DOLLAR_TICK)
				{
					m_nYUnitType = WON_TICK;
					//m_btYUnit.SetWindowText("\\");
				}
				else if(m_nYUnitType == PERCENT_TICK)
				{
					m_nYUnitType = DOLLAR_TICK;
					//m_btYUnit.SetWindowText("$");
				}

				if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
					m_pChartMng->SendMessage(UMSG_RATEVALUE, m_nYUnitType);	
			}
		}	
		
		return 1L;
	}
	else if(message == RMS_ANALYSIS_FUNC)
	{
		NMHDR* nmhdr = (NMHDR*)lParam;

		int nID = (int)wParam;
		if(nID < 0)
			return CDialog::WindowProc(message, wParam, lParam);

		//툴버튼선택
		if(nmhdr->code == 1)
		{
			if(wParam != CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION)
				((CStdDialog*)m_pMultiChartWnd)->SendMessage(RMS_ANALYSIS_FUNC, wParam, TRUE);
		}
		//툴버튼해제
		else
		{
			//SelectToolSelection();
			if(wParam != CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION)
			{
				((CStdDialog*)m_pMultiChartWnd)->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
			}
		}
	}

	
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CDlgToolbar::DestroyWindow()
{
	((CStdDialog*)m_pMultiChartWnd)->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, 0);
	return CDialog::DestroyWindow();
}

void CDlgToolbar::SelectToolSelection()
{
	if (m_nCursorIndex < 0 || m_nCursorIndex >= NUM_BUTTONS)
		return;

	if(m_pToolBtn[m_nCursorIndex])
	{
		m_pToolBtn[m_nCursorIndex]->m_nBtDown = 3;//BT_DEF_DOWN
		m_pToolBtn[m_nCursorIndex]->m_bDowned = TRUE;
		m_pToolBtn[m_nCursorIndex]->m_nBeState = m_pToolBtn[m_nCursorIndex]->m_nBtDown;
		m_pToolBtn[m_nCursorIndex]->InvalidateRect(NULL, TRUE);
		if (m_pToolBtn[m_nCursorIndex]->m_nCurrToggle == 0 && m_pCurToggleBtn != m_pToolBtn[m_nCursorIndex])
		{
			SendMessage(UDM_CHARTBUTTON_CLICK, 2, m_pToolBtn[m_nCursorIndex]->GetDlgCtrlID());
			m_pCurToggleBtn = m_pToolBtn[m_nCursorIndex];
		}
	}
}

void CDlgToolbar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	CRect rcClient;
//		  rcDraw;
	GetClientRect( &rcClient );

	CDC memDC;
	CBitmap bitmap;
	memDC.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	//Font 추가
	CFont *pFtDefault = (CFont *)memDC.SelectObject(GetParent()->GetFont());

	memDC.FillSolidRect ( rcClient, (COLORREF)GetParent()->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0));

//	rcDraw = rcClient;
//	rcDraw.left = X_OFFSET;
//	rcDraw.right = X_OFFSET + TOOL_BUTTON_WIDTH;
//	rcDraw.bottom = Y_OFFSET - 7;

	CRect rcBtns;
	rcBtns = rcClient;
	rcBtns.left = X_OFFSET;
	rcBtns.right = X_OFFSET + TOOL_BUTTON_WIDTH;
	rcBtns.top = Y_OFFSET + TOOL_BUTTON_HEIGHT + 1;

	CPen penLine1(PS_SOLID, 1, RGB(128,128,128));
	CPen *pOldPen = memDC.SelectObject(&penLine1);
	if(m_nSelTool == 0)
	{
		memDC.MoveTo(rcBtns.left, rcBtns.top);
		memDC.LineTo(rcBtns.right, rcBtns.top);
		for(int i = 0; i < m_arrayUserIdx.GetSize(); i++)
		{
			rcBtns.left = X_OFFSET;
			rcBtns.top = Y_OFFSET2 + (i+1) * TOOL_BUTTON_HEIGHT;
			rcBtns.right = rcBtns.left + TOOL_BUTTON_WIDTH;
			rcBtns.bottom = rcBtns.top + TOOL_BUTTON_HEIGHT;
		}
		memDC.MoveTo(rcBtns.left, rcBtns.bottom);
		memDC.LineTo(rcBtns.right, rcBtns.bottom);
	}
	else if(m_nSelTool == 1)
	{
		memDC.MoveTo(rcBtns.left, rcBtns.top);
		memDC.LineTo(rcBtns.right, rcBtns.top);
		for(int i = 0; i < m_arrayAnalIdx.GetSize(); i++)
		{
			rcBtns.left = X_OFFSET;
			rcBtns.top = Y_OFFSET2 + (i+1) * TOOL_BUTTON_HEIGHT;
			rcBtns.right = rcBtns.left + TOOL_BUTTON_WIDTH;
			rcBtns.bottom = rcBtns.top + TOOL_BUTTON_HEIGHT;
		}
		memDC.MoveTo(rcBtns.left, rcBtns.bottom);
		memDC.LineTo(rcBtns.right, rcBtns.bottom);
	}
	else if(m_nSelTool == 2)
	{
		memDC.MoveTo(rcBtns.left, rcBtns.top);
		memDC.LineTo(rcBtns.right, rcBtns.top);
		for(int i = 0; i < m_arrayAdditionIdx.GetSize(); i++)
		{
			rcBtns.left = X_OFFSET;
			rcBtns.top = Y_OFFSET2 + (i+1) * TOOL_BUTTON_HEIGHT;
			rcBtns.right = rcBtns.left + TOOL_BUTTON_WIDTH;
			rcBtns.bottom = rcBtns.top + TOOL_BUTTON_HEIGHT;
		}
		memDC.MoveTo(rcBtns.left, rcBtns.bottom);
		memDC.LineTo(rcBtns.right, rcBtns.bottom);
	}
	
	rcBtns.bottom = rcClient.bottom - Y_OFFSET*2;
	if(m_pBtnDown->IsWindowVisible())
	{
		memDC.MoveTo(rcBtns.left, rcBtns.bottom);
		memDC.LineTo(rcBtns.right, rcBtns.bottom);
	}
	memDC.SelectObject(pOldPen);
	//CPen penLine1(PS_SOLID, 1, RGB(252,252,252));
	//CPen *pOldPen = memDC.SelectObject(&penLine1);

	//memDC.MoveTo(rcDraw.left, rcDraw.bottom);
	//memDC.LineTo(rcDraw.right, rcDraw.bottom);

	//CPen penLine2(PS_SOLID, 1, RGB(108,108,108));
	//memDC.SelectObject(&penLine2);

	//memDC.MoveTo(rcDraw.left, rcDraw.bottom+1);
	//memDC.LineTo(rcDraw.right, rcDraw.bottom+1);

	//rcDraw.bottom = Y_OFFSET2 + TOOL_BUTTON_HEIGHT - 5;
	//CPen penLine3(PS_SOLID, 1, RGB(161,161,161));
	//memDC.SelectObject(&penLine3);

	//memDC.MoveTo(rcDraw.left, rcDraw.bottom);
	//memDC.LineTo(rcDraw.right, rcDraw.bottom);

	//CPen penLine4(PS_SOLID, 1, RGB(236,236,236));
	//memDC.SelectObject(&penLine4);

	//memDC.MoveTo(rcDraw.left, rcDraw.bottom+1);
	//memDC.LineTo(rcDraw.right, rcDraw.bottom+1);

	//memDC.SelectObject(pOldPen);

	// copy the memory device context back into the original DC via BitBlt().
	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &memDC, 0,0, SRCCOPY);

	// cleanup resources.
	memDC.SelectObject(pOldBitmap);
	//Font 추가
	memDC.SelectObject(pFtDefault);
	memDC.DeleteDC();
	bitmap.DeleteObject();
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CDlgToolbar::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CDlgToolbar::ResetAllButton()
{
	for(int i = 0; i < NUM_BUTTONS; i++)
	{
		if(m_pToolBtn[i])
		{
			if (m_pToolBtn[i]->m_nCurrToggle == 1)
			{
				m_pToolBtn[i]->m_nCurrToggle = 0;
				m_pToolBtn[i]->InvalidateRect(NULL, TRUE);
			}
		}
	}
	m_nCursorIndex = 1;
	m_bShowScale = FALSE;
	m_bShowInnerToolbar = FALSE;
	m_nYUnitType = WON_TICK;
	m_bCheckShowData = FALSE;
}

void CDlgToolbar::GetDrawInfo(int nY, int nSelTool)
{	
	int		nWinPos, nLow, ncnt;

	//실제 배치된 버튼수
	if (nSelTool == 0)						//사용자정의
		m_drawinfo.nRealMenuCount = m_arrayUserIdx.GetSize();
	else if (nSelTool == 1)					//분석도구 리스트
		m_drawinfo.nRealMenuCount = m_arrayAnalIdx.GetSize();
	else if (nSelTool == 2)					//부가기능 리스트
		m_drawinfo.nRealMenuCount = m_arrayAdditionIdx.GetSize();

	//현재 변경된 윈도우 크기
	nWinPos = nY - (Y_OFFSET); 
	m_drawinfo.nCurrWindowPos = nWinPos;

	//변경된 윈도우에 배치될 수 있는 버튼수
	// -1은 방향 버튼의 수를 뺀것
	ncnt = nWinPos / TOOL_BUTTON_HEIGHT;
	m_drawinfo.nCalcMenuCount = ncnt - 1;

	//상위에 배치 가능한 버튼수
	m_drawinfo.nHighMenuCount = m_drawinfo.nCalcMenuCount;

	//하위에 배치 가능한 버튼수
	nLow = m_drawinfo.nRealMenuCount - m_drawinfo.nHighMenuCount + 2;
	if (nLow <= 0)
		m_drawinfo.nLowMenuCount = 0;
	else
		m_drawinfo.nLowMenuCount = nLow;

	/*CString st;
	st.Format("\n실제 = %d / 계산 = %d / 상 = %d / 하 = %d / lt = %d\n curr = %d / npos = %d / ncnt = %d\n", m_drawinfo.nRealMenuCount, m_drawinfo.nCalcMenuCount, 
	m_drawinfo.nHighMenuCount, m_drawinfo.nLowMenuCount, m_drawinfo.nLastBtnPos, m_drawinfo.nCurrWindowPos,nWinPos);
	OutputDebugString(st);*/

	return;
}

void CDlgToolbar::ResizeToolbar(int nY)
{
	BOOL bRet = FALSE;
	//버튼이 배치될 수 있는 정보를 얻는다.
	GetDrawInfo(nY, m_nSelTool);

	//초기화
	m_drawinfo.nLowBojo = -1;
	m_drawinfo.nLowLastIndex = -1;
	m_drawinfo.nHighBojo = -1;
	m_drawinfo.nLowLastIndex = -1;
	m_drawinfo.nTotBojo = -1;

	//윈도우 크기 변경으로 툴바의 버튼 이미지가 잘린경우만 툴바 다시 그림
	if ( ((m_drawinfo.nPreWindowPos == 0) || (m_drawinfo.nPreWindowPos == m_drawinfo.nCurrWindowPos)) && (m_nResizeFlag == 0) ) 
	{
		if ( (m_drawinfo.nCurrWindowPos > 0) && (m_drawinfo.nCurrWindowPos < m_drawinfo.nLastBtnPos))
			bRet = DrawHighMenu(m_drawinfo.nCalcMenuCount, m_nSelTool);
		else
		{
			m_drawinfo.nPreWindowPos = 0;
			m_nResizeFlag = 0;
			return;
		}
	}
	else if ((m_drawinfo.nPreWindowPos > m_drawinfo.nCurrWindowPos) && (m_nResizeFlag == 1))
	{
		bRet = DrawHighMenu(m_drawinfo.nHighMenuCount, m_nSelTool);
	}
	else if ((m_drawinfo.nPreWindowPos < m_drawinfo.nCurrWindowPos) && (m_nResizeFlag == 1))
	{
		if (m_drawinfo.nCalcMenuCount < m_drawinfo.nRealMenuCount)
		{
			bRet = DrawHighMenu(m_drawinfo.nHighMenuCount, m_nSelTool);
		}
		else if (m_drawinfo.nCalcMenuCount >= m_drawinfo.nRealMenuCount)
		{
			m_pBtnUp->ShowWindow(FALSE);
			m_pBtnDown->ShowWindow(FALSE);
			ResizeControl(25, 0);

			m_drawinfo.nPreWindowPos = 0;
			m_nResizeFlag = 0;

			return;
		}
	}
	else
	{
		m_drawinfo.nPreWindowPos = 0;
		m_nResizeFlag = 0;

		return;
	}

	if (bRet)
	{
		m_drawinfo.nPreWindowPos = m_drawinfo.nCurrWindowPos;

		m_nResizeFlag = 1;
	}

}

BOOL CDlgToolbar::DrawLowMenu(int nLowMenuCount, int nHighMenuCount, int nSelTool)
{
	if (m_drawinfo.nLowBojo == 0)
	{
		m_pBtnDown->EnableWindow(FALSE);
		AfxMessageBox("표시할 하위메뉴가 없습니다.");
		return FALSE;

	}
	else if ((m_drawinfo.nCalcMenuCount <= 1) && (m_nResizeFlag == 1))
	{
#ifdef _DEBUG
		AfxMessageBox("ToolBar에 메뉴를 표시할 수 없습니다.");
#endif
		m_nResizeFlag = 0;
		return FALSE;
	}

	if(!m_pToolBtn[0]->GetSafeHwnd())
		return FALSE;

	m_pBtnUp->EnableWindow(TRUE);
	m_pBtnDown->EnableWindow(TRUE);

	int nBottom = 0;
	RECT rect;

	rect.left = X_OFFSET;
	rect.top = Y_OFFSET;
	rect.right = rect.left + TOOL_BUTTON_WIDTH;
	rect.bottom = rect.top + TOOL_BUTTON_HEIGHT;
	m_pToolBtn[0]->MoveWindow(&rect);

	int nKeyIdx;
	rect.left = 0;
	rect.top = 0;
	rect.right = 0;
	rect.bottom = 0;
	for(int i = 3; i < m_arrayTotalIdx.GetSize()+5; i++)
	{
		if(m_pToolBtn[i])
			m_pToolBtn[i]->MoveWindow(&rect);
	}
	int nKeyInx;
	int nPos = 0;

	nKeyInx = nSelTool;
	m_pToolBtn[0]->SetValueBtn(GetToolOption(TOOL_BUTTON_BASE_ID + 0), g_bToggle_Bar[0], m_pIL_W28H22, nKeyInx*3, nKeyInx*3+1, nKeyInx*3+2);

	int nTmp, nMod, nLoop;
	if ( nLowMenuCount >= m_drawinfo.nCalcMenuCount)
	{
		nMod = (nLowMenuCount - 1) % (m_drawinfo.nCalcMenuCount-1);

		if (m_drawinfo.nLowBojo == -1)
		{
			if (nMod > 0)
				nTmp = ((nLowMenuCount - 1) / (m_drawinfo.nCalcMenuCount -1)) + 1;
			else if (nMod == 0)
				nTmp = ((nLowMenuCount - 1) / (m_drawinfo.nCalcMenuCount - 1));
			else
				nTmp = 0;

			m_drawinfo.nLowBojo = nTmp;
			m_drawinfo.nTotBojo = nTmp;

			//상위메뉴
			if (nTmp > 0)
				m_drawinfo.nHighBojo = 0;
		}		
	}

	if (m_drawinfo.nLowBojo > 1) 
		nLoop = m_drawinfo.nCalcMenuCount;
	else if (m_drawinfo.nLowBojo == 1 )
	{
		if (nMod > 0)
			nLoop = nMod + 1;
		else
			nLoop = m_drawinfo.nCalcMenuCount;
	}
	else if (m_drawinfo.nLowBojo < 0)
		nLoop = nLowMenuCount;

	for(i = 0; i < nLoop ; i++)
	{
		if (m_drawinfo.nLowLastIndex < 0)
			nPos = (nHighMenuCount - 1)  + i;
		else
			nPos = m_drawinfo.nLowLastIndex  + i;

		if ( i < (nLoop - 1))
		{
			rect.left = X_OFFSET;
			rect.top = Y_OFFSET2 + (i+1) * TOOL_BUTTON_HEIGHT;
			rect.right = rect.left + TOOL_BUTTON_WIDTH;
			rect.bottom = rect.top + TOOL_BUTTON_HEIGHT;

			if (nSelTool == 0)
				nKeyIdx = m_arrayUserIdx.GetAt(nPos);
			else if(nSelTool == 1)
				nKeyIdx = m_arrayAnalIdx.GetAt(nPos);
			else if(nSelTool == 2)
				nKeyIdx = m_arrayAdditionIdx.GetAt(nPos);

			if(m_pToolBtn[nKeyIdx])
				m_pToolBtn[nKeyIdx]->MoveWindow(&rect);	

		}
		else if ( i < nLoop)
		{
			//RECT btnrect;
			CRect toolrect;
			GetClientRect(&toolrect);

			rect.left = X_OFFSET;
			rect.top = toolrect.bottom - TOOL_BUTTON_HEIGHT - Y_OFFSET*2;
			rect.right = rect.left + TOOL_BUTTON_WIDTH;
			rect.bottom = toolrect.bottom - (TOOL_BUTTON_HEIGHT / 2) - Y_OFFSET*2;

			m_pBtnUp->MoveWindow(&rect);

			rect.left = X_OFFSET;
			rect.top = toolrect.bottom - (TOOL_BUTTON_HEIGHT / 2) - Y_OFFSET*2;
			rect.right = rect.left + TOOL_BUTTON_WIDTH;
			rect.bottom = toolrect.bottom - Y_OFFSET*2;

			m_pBtnDown->MoveWindow(&rect);
		}
	}

	if (m_drawinfo.nLowBojo > 1)
	{
		m_drawinfo.nLowBojo -= 1;
		m_drawinfo.nLowLastIndex = nPos;
		m_drawinfo.nHighBojo += 1;
		m_drawinfo.nHighLastIndex = nPos - (2 * (m_drawinfo.nCalcMenuCount - 1));
	}
	else if (m_drawinfo.nLowBojo == 1)
	{
		if(m_drawinfo.nLowLastIndex<=0)
			m_drawinfo.nHighLastIndex = 0;
		else
			m_drawinfo.nHighLastIndex = m_drawinfo.nLowLastIndex - (m_drawinfo.nCalcMenuCount - 1);
		m_drawinfo.nLowBojo -= 1;
		m_drawinfo.nLowLastIndex = nPos;
		m_drawinfo.nHighBojo = m_drawinfo.nTotBojo;
		m_pBtnDown->EnableWindow(FALSE);
	}
	else if (m_drawinfo.nLowBojo == -1)
		m_pBtnDown->EnableWindow(FALSE);

	return TRUE;
}

BOOL CDlgToolbar::DrawHighMenu(int nHighMenuCount, int nSelTool)
{
	if (m_drawinfo.nHighBojo == 0)
	{
		AfxMessageBox("표시할 상위메뉴가 없습니다.");
		return FALSE;
	}
	else if ((m_drawinfo.nCalcMenuCount < 1) && (m_nResizeFlag == 1))
	{
#ifdef _DEBUG
		AfxMessageBox("ToolBar에 메뉴를 표시할 수 없습니다.");
#endif
		m_drawinfo.nPreWindowPos = m_drawinfo.nCurrWindowPos;
		m_nResizeFlag = 0;
		return FALSE;
	}

	if(!m_pToolBtn[0]->GetSafeHwnd())
		return FALSE;

	m_pBtnUp->EnableWindow(TRUE);
	m_pBtnDown->EnableWindow(TRUE);
	int nBottom = 0;
	RECT rect;

	rect.left = X_OFFSET;
	rect.top = Y_OFFSET;
	rect.right = rect.left + TOOL_BUTTON_WIDTH;
	rect.bottom = rect.top + TOOL_BUTTON_HEIGHT;
	m_pToolBtn[0]->MoveWindow(&rect);

	int nKeyIdx;
	rect.left = 0;
	rect.top = 0;
	rect.right = 0;
	rect.bottom = 0;
	for(int i = 3; i < m_arrayTotalIdx.GetSize()+5; i++)
	{
		if(m_pToolBtn[i])
			m_pToolBtn[i]->MoveWindow(&rect);
	}
	int nKeyInx;

	nKeyInx = m_nSelTool;
	m_pToolBtn[0]->SetValueBtn(GetToolOption(TOOL_BUTTON_BASE_ID + 0), g_bToggle_Bar[0], m_pIL_W28H22, nKeyInx*3, nKeyInx*3+1, nKeyInx*3+2);

	int nLoop, nPos;

	if (m_drawinfo.nHighBojo < 0)
		nLoop = nHighMenuCount;
	else
	{
		nLoop = m_drawinfo.nCalcMenuCount;
		//m_drawinfo.nLowBojo = 0;
	}

	for(i = 0; i < nLoop ; i++)
	{
		if (m_drawinfo.nHighBojo < 0)
			nPos = i;
		else
			nPos = m_drawinfo.nHighLastIndex + i;

		if ( i < (nLoop - 1))
		{
			rect.left = X_OFFSET;
			rect.top = Y_OFFSET2 + (i + 1) * TOOL_BUTTON_HEIGHT ;
			rect.right = rect.left + TOOL_BUTTON_WIDTH;
			rect.bottom = rect.top + TOOL_BUTTON_HEIGHT;

			if (nSelTool == 0)
				nKeyIdx = m_arrayUserIdx.GetAt(nPos);
			else if (nSelTool == 1)
				nKeyIdx = m_arrayAnalIdx.GetAt(nPos);
			else if (nSelTool == 2)
				nKeyIdx = m_arrayAdditionIdx.GetAt(nPos);

			if(m_pToolBtn[nKeyIdx])
				m_pToolBtn[nKeyIdx]->MoveWindow(&rect);	

		}
		else if ( i < nLoop)
		{
			CRect toolrect;
			GetClientRect(&toolrect);

			rect.left = X_OFFSET;
			rect.top = toolrect.bottom - TOOL_BUTTON_HEIGHT - Y_OFFSET*2;
			rect.right = rect.left + TOOL_BUTTON_WIDTH;
			rect.bottom = toolrect.bottom - (TOOL_BUTTON_HEIGHT / 2) - Y_OFFSET*2;

			m_pBtnUp->MoveWindow(&rect);

			rect.left = X_OFFSET;
			rect.top = toolrect.bottom - (TOOL_BUTTON_HEIGHT / 2) - Y_OFFSET*2;
			rect.right = rect.left + TOOL_BUTTON_WIDTH;
			rect.bottom = toolrect.bottom - Y_OFFSET*2;
			
			m_pBtnDown->MoveWindow(&rect);
			m_pBtnUp->ShowWindow(TRUE);
			m_pBtnDown->ShowWindow(TRUE);
		}
	}

	if (m_drawinfo.nHighBojo > 1)
	{
		m_drawinfo.nHighBojo -= 1;
		//이전의 그리기 시작 인덱스
		m_drawinfo.nHighLastIndex = nPos - (2 * (m_drawinfo.nCalcMenuCount - 1));
		m_drawinfo.nLowBojo += 1;
		m_drawinfo.nLowLastIndex = nPos;
	}
	else if (m_drawinfo.nHighBojo == 1)
	{
		m_drawinfo.nHighBojo -= 1;
		m_drawinfo.nHighLastIndex = nPos;
		m_drawinfo.nLowBojo = m_drawinfo.nTotBojo;
		m_drawinfo.nLowLastIndex = (2 * (m_drawinfo.nCalcMenuCount - 1)) - nPos;

		m_pBtnUp->EnableWindow(FALSE);
	}
	else if (m_drawinfo.nHighBojo == -1)
		m_pBtnUp->EnableWindow(FALSE);

	return TRUE;
}

BOOL CDlgToolbar::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(nCode ==CN_COMMAND)
	{
		long nCmdID = nID - TOOL_BUTTON_BASE_ID;

		switch(nCmdID)
		{
			case 200:	//Up
				if (m_drawinfo.nCalcMenuCount < 1)
				{
					AfxMessageBox("ToolBar에 상위메뉴를 표시할 수 없습니다.");
				}
				DrawHighMenu(m_drawinfo.nHighMenuCount, m_nSelTool);
					break;
			case 201:	//
				if (m_drawinfo.nCalcMenuCount < 1)
				{
					AfxMessageBox("ToolBar에 하위메뉴를 표시할 수 없습니다.");
				}
				DrawLowMenu(m_drawinfo.nLowMenuCount,m_drawinfo.nHighMenuCount, m_nSelTool);
				break;
		}
	}
	return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

//2007.01.17 add by LYH  툴바버튼 동기화
void CDlgToolbar::SetToggleButton(int nSelToolNo, BOOL bSelect)
{
	int nID = -1;
	if(nSelToolNo > 50000)
	{
		nID = nSelToolNo - 50000;
		if(nID == 40)
		{
			m_bShowScale = bSelect;
		}
		else if(nID == 41)
		{
			m_bShowInnerToolbar = bSelect;
		}
	}
	else
	{
		switch(nSelToolNo)
		{
		case CToolOptionInfo::TOOLOPTION::T_SYMBOL_BULET			:nID= 4; break;	// 그리기 모음
		case CToolOptionInfo::TOOLOPTION::T_TOOL_LINE_CROSS			:nID= 5; break;	// 십자선                                               
		case CToolOptionInfo::TOOLOPTION::T_TOOL_LINE_HORZ			:nID= 6; break;	// 수평선                                                 
		case CToolOptionInfo::TOOLOPTION::T_TOOL_LINE_VERT			:nID= 7; break;	// 수직선                                                 
		case CToolOptionInfo::TOOLOPTION::T_TOOL_LINE				:nID= 8; break;// 추세선                                                 
		case CToolOptionInfo::TOOLOPTION::T_TOOL_PENCIL_LINE		:nID= 9; break;	// 연필 추세선                                          
		case CToolOptionInfo::TOOLOPTION::T_TOOL_ELLIOT_WAVE_LINE	:nID=10; break;// 앨리어트파동선                                           
		case CToolOptionInfo::TOOLOPTION::T_SYMBOL_SQUARE			:nID=11; break;// 네모                                                 
		case CToolOptionInfo::TOOLOPTION::T_SYMBOL_CIRCLE			:nID=12; break;// 원                                                 
		case CToolOptionInfo::TOOLOPTION::T_SYMBOL_TEXT				:nID=13; break;	// 텍스트                                               
		case CToolOptionInfo::TOOLOPTION::T_TOOL_ANDREWS_PITCHFORK	:nID=14; break;	// 앤드류피치포크                                           
		case CToolOptionInfo::TOOLOPTION::T_TOOL_FIBO_RETRACE		:nID=15; break;// 존추세선                                                 
		case CToolOptionInfo::TOOLOPTION::T_TOOL_SPEED_FAN			:nID=16; break;	// 팬추세선                                                 
		case CToolOptionInfo::TOOLOPTION::T_TOOL_FIBONACCI_TIME		:nID=17; break;	// 피보나치수열                                         
		case CToolOptionInfo::TOOLOPTION::T_TOOL_FIBONACCI_ARC		:nID=18; break;	// 활추세선                                                 
		case CToolOptionInfo::TOOLOPTION::T_TOOL_FIBONACCI_FAN		:nID=19; break;	// 피보나치팬                                               
		case CToolOptionInfo::TOOLOPTION::T_TOOL_GANN_LINE45		:nID=20; break;// 우상갠                                                 
		case CToolOptionInfo::TOOLOPTION::T_DELETE_INORDER			:nID=21; break;// 단일 지우개                                              
		case CToolOptionInfo::TOOLOPTION::T_DELETE_ALL				:nID=22; break;	// 전체 지우개
		case CToolOptionInfo::TOOLOPTION::T_RESTORE					:nID=23; break;	// 전체 초기화
			//	case CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION			:nID=24; break;	// 환경설정                                         
			//	case CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION			:nID=25; break;	// 이평셋초기화                                           
			//	case CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION			:nID=26; break;	// 공시                                                 
		case CToolOptionInfo::TOOLOPTION::T_PRICE_YSCALE_SETUP		:nID=27; break;	// Y축설정                                                 
		case CToolOptionInfo::TOOLOPTION::T_NIDLG_OVER				:nID=28; break;	// 수치                                                 
		case CToolOptionInfo::TOOLOPTION::T_TOOL_TRACEING_TT		:nID=29; break;// 요약데이                                                 
		case CToolOptionInfo::TOOLOPTION::T_TOOL_PERIOD_SUMMARY		:nID=30; break;	// 기간조회                                             
			//	case CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION			:nID=31; break;	// 자동추세
			//	case CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION			:nID=32; break;	// 추세패턴
			//	case CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION			:nID=33; break;	// 캔들패턴
			//	case CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION			:nID=34; break;	// 패턴통계 
			//	case CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION			:nID=35; break;	// 패턴설정
			//	case CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION			:nID=36; break;	// 저장
			//	case CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION			:nID=37; break;	// 불러오기
		case CToolOptionInfo::TOOLOPTION::T_PRINT_CHART				:nID=38; break;	// 화면출력
			//	case CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION			:nID=39; break;	// Excel변환
			//	case CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION			:nID=40; break;	// 눈금영역숨김
			//	case CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION			:nID=41; break;	// 스크롤숨기기
			//	case CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION			:nID=42; break;	// 일주월분
			//	case CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION			:nID=43; break;	// 수치조회하단
		case CToolOptionInfo::TOOLOPTION::T_DATALIST_REPORT			:nID=44; break;	// 텍스트윈도우
			//	case CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION			:nID=45; break;	// 기준선
		case CToolOptionInfo::TOOLOPTION::T_ORDER_LINE				:nID=46; break;	// 주문선
		case CToolOptionInfo::TOOLOPTION::T_TOOL_GANN_FAN45			:nID=47; break;	// 상향갠팬
		case CToolOptionInfo::TOOLOPTION::T_TOOL_GANN_FAN315		:nID=48; break;// 하향갠팬
		case CToolOptionInfo::TOOLOPTION::T_TOOL_ANGLE_LINE			:nID=49; break;	// 각도추세선
		case CToolOptionInfo::TOOLOPTION::T_SYMBOL_TRIANGLE			:nID=50; break;	// 세모
		case CToolOptionInfo::TOOLOPTION::T_TOOL_GANN_GRID			:nID=51; break;	// 갠앵글
		case CToolOptionInfo::TOOLOPTION::T_TOOL_SPEED_ARC			:nID=52; break;	// 가속저항호
		case CToolOptionInfo::TOOLOPTION::T_TOOL_SPEED_LINE			:nID=53; break;	// 가속저항팬
		case CToolOptionInfo::TOOLOPTION::T_TOOL_LINE_TRISECT		:nID=54; break;// 삼등분선
		case CToolOptionInfo::TOOLOPTION::T_TOOL_LINE_QUARTER		:nID=55; break;// 사등분선
		case CToolOptionInfo::TOOLOPTION::T_TOOL_CYCLE_LINES		:nID=56; break;// 사이클구간
		case CToolOptionInfo::TOOLOPTION::T_TOOL_ADJUST_RATIO		:nID=57; break;// 조정백분율
		case CToolOptionInfo::TOOLOPTION::T_TOOL_LINEAR_REGRESSION	:nID=58; break;	// 직선회귀선
		case CToolOptionInfo::TOOLOPTION::T_TOOL_RAFF_REGRESSION	:nID=59; break;// 직선회귀채널
		case CToolOptionInfo::TOOLOPTION::T_DRAG_Y_ZOOM				:nID=60; break;	// Y축확대
		case CToolOptionInfo::TOOLOPTION::T_PAN						:nID=61; break;	// 차트이동
			//	case CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION			:nID=62; break;	// 부분확대
		case CToolOptionInfo::TOOLOPTION::T_TOOL_CANDLE_LINE		:nID=63; break;// 봉중심선
		case CToolOptionInfo::TOOLOPTION::T_TOOL_CANDLE_BONG		:nID=64; break;// 가상봉
			//	case CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION			:nID=65; break;	// 복사
			//	case CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION			:nID=66; break;	// 원클릭숨기기
			//	case CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION			:nID=67; break;	// 시간선
			//	case CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION			:nID=68; break;	// 거래량이평
			//	case CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION			:nID=69; break;	// Excel불러오기
		case CToolOptionInfo::TOOLOPTION::T_UNDO_WORK				:nID=70; break;// Undo
		case CToolOptionInfo::TOOLOPTION::T_REDO_WORK				:nID=71; break;// Redo
		case CToolOptionInfo::TOOLOPTION::T_ORDER_LINE_MANAGER		:nID=72; break;// 주문선관리
		case CToolOptionInfo::TOOLOPTION::T_MAGNIFYINGGLASS			:nID=73; break;// 돋보기
		}
	}
	if(nID<0)
		return;
//	m_pToolBtn[nID]->SetToggleBtnFromOutside();
	if(m_pToolBtn[nID])
	{
		if (m_pToolBtn[nID]->m_nCurrToggle == 1 && bSelect == 0)
		{
			m_pToolBtn[nID]->m_nCurrToggle = 0;
			m_pToolBtn[nID]->InvalidateRect(NULL, TRUE);
		}
		if (m_pToolBtn[nID]->m_nCurrToggle == 0 && bSelect == 1)
		{
			m_pToolBtn[nID]->m_nCurrToggle = 1;
			m_pToolBtn[nID]->InvalidateRect(NULL, TRUE);
		}
	}
}
//end

BOOL CDlgToolbar::GetIndexArray(int nSelTool, CArray<int, int>& arIndx, CArray<int, int>& arCheck)
{
	int nKeyIdx;
	if(nSelTool == 0)
	{
		for(int i = 0; i < m_arrayUserIdx.GetSize(); i++)
		{
			nKeyIdx = m_arrayUserIdx.GetAt(i);
			arIndx.Add(nKeyIdx);
			arCheck.Add(m_pToolBtn[nKeyIdx]->GetToggle());
		}
		return TRUE;
	}
	else if(nSelTool == 1)
	{
		for(int i = 0; i < m_arrayAnalIdx.GetSize(); i++)
		{
			nKeyIdx = m_arrayAnalIdx.GetAt(i);
			arIndx.Add(nKeyIdx);
			arCheck.Add(m_pToolBtn[nKeyIdx]->GetToggle());
		}
		return TRUE;
	}
	else if(nSelTool == 2)
	{
		for(int i = 0; i < m_arrayAdditionIdx.GetSize(); i++)
		{
			nKeyIdx = m_arrayAdditionIdx.GetAt(i);
			arIndx.Add(nKeyIdx);
			arCheck.Add(m_pToolBtn[nKeyIdx]->GetToggle());
		}
		return TRUE;
	}

	return FALSE;
}

BOOL CDlgToolbar::GetToolNameMap(CMap<int, int, CString, CString>& mapToolName)
{
	int nKeyIdx;
	CString strButtonName;
	for(int i = 0; i < m_arrayTotalIdx.GetSize() ; i++)
	{
		nKeyIdx = m_arrayTotalIdx.GetAt(i);

		m_mapToolName.Lookup(nKeyIdx, strButtonName);
		mapToolName.SetAt(nKeyIdx, strButtonName);
	}

	return TRUE;
}

void CDlgToolbar::SetToggleBtnFromOutside(UINT nID)
{
	nID = nID - TOOL_BUTTON_BASE_ID;
	m_pToolBtn[nID]->SetToggleBtnFromOutside();
}
