// ToolBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ToolBarDlg.h"

#include "../Include_Chart/Dll_Load/PacketList.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/IChartCtrl.h"					// for IChartCtrl
#include "../Include_Chart/IChartCtrl.h"					// for IChartCtrl

#include "Definition.h"
#include "CtrlFactory.h"
#include "ToolBarManagerImplementation.h"
#include "ToolOption.h"
#include "ObjTool.h"
#include "ButtonProperty.h"
// 툴바 등록에 필요한 Ctrl 헤더들.
#include "ButtonEx.h"
#include "RadarControl.h"
#include "GraphStyleCombo.h"
#include "GraphLineThicknessCombo.h"
#include "ScrollBarEx.h"
#include "SliderCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBitmap CToolBarDlg::m_bitmapButton;

extern bool IsToggleButton(CToolOptionInfo::TOOLOPTION enumToolOption);
/////////////////////////////////////////////////////////////////////////////
// CToolBarDlg dialog


CToolBarDlg::CToolBarDlg(CWnd* pParent, CToolBarManagerImplementation* pToolBarImpl, IChartCtrl *p_pIChartCtrl)
	: CDialog(CToolBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolBarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pScrollBar = NULL;
	m_pSliderCtrl = NULL;
	m_pGraphStyleCombo = NULL;
	m_pGraphLineCombo = NULL;
	m_pRadarControl = NULL;
	m_pButtonProperty = NULL;
	m_clrSelection = RGB(0,0,0);
	m_nSliderMin = 5;
	m_nSliderMax = 5;
	m_nRemainCount = 0;
	m_nGlanceBalanceMargin = 0;
	m_nScrollWidth = 0;
	m_bCheckGlanceBalance = false;
	m_bRadarChart = false;
	m_bStatusBar = false;
	m_bOneChart = false;
	m_bIsNoZoomChart = false;						// (2009/11/6 - Seung-Won, Bae) Do not disable zoom and scroll on OneChart
	m_bUseRadarChart = FALSE;
	m_pToolBarManagerImple = pToolBarImpl;
	m_nScrollingDataCount = 1;

	// (2006/11/18 - Seung-Won, Bae) Support Chart OCX Ctrl Interface
	m_pIChartCtrl = p_pIChartCtrl;

	// (2007/2/8 - Seung-Won, Bae) Disable WholeView Btn in Special Chart.
	m_pWholeViewBtn = NULL;
	// (2007/2/8 - Seung-Won, Bae) Disable Zoom Btn in Special Chart.
	m_pZoomInBtn = NULL;
	m_pZoomOutBtn = NULL;
}

void CToolBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolBarDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolBarDlg, CDialog)
	//{{AFX_MSG_MAP(CToolBarDlg)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBarDlg message handlers

/*-----------------------------------------------------------------------------------------
 - Function    :  OnCancel, OnOK
 - Created at  :  2004-10-18   15:02
 - Author      :  최종찬
 - Description :  Enter키와 Esc키에 의한 Dlg Close를 막음.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::OnCancel() 
{
//	CDialog::OnCancel();
}
void CToolBarDlg::OnOK() 
{
//	CDialog::OnOK();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  ~CToolBarDlg
 - Created at  :  2004-11-25   09:38
 - Author      :  최종찬
 - Description :  소멸자
 -----------------------------------------------------------------------------------------*/
CToolBarDlg::~CToolBarDlg()
{
	DeleteCtrl();
	
	if(m_pButtonProperty)
	{
		delete m_pButtonProperty;
		m_pButtonProperty = NULL;
	}
}


/*-----------------------------------------------------------------------------------------
 - Function    :  DeleteCtrl
 - Created at  :  2004-11-30   15:35
 - Author      :  최종찬
 - Description :  리스트에 담겨져 있는 컨트롤들을 삭제한다. 단순 제거(Remove)가 아님.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::DeleteCtrl()
{
	POSITION pos = m_listToolBarCtrl.GetHeadPosition();
	
	CWnd* pWnd = NULL;
	while(pos)
	{
		pWnd = m_listToolBarCtrl.GetNext(pos);
		if(pWnd)
		{
			if( pWnd->GetSafeHwnd()) pWnd->DestroyWindow();
			delete pWnd;
		}
		pWnd = NULL;
	}
	m_listToolBarCtrl.RemoveAll();
	m_listToolButton.RemoveAll();	// 버튼 오브젝트들은 m_listToolBarCtrl에서 이미 delete된다.

	if(m_pRadarControl)
	{
		delete m_pRadarControl;
		m_pRadarControl = NULL;
	}

	// (2008/2/5 - Seung-Won, Bae) Reset Pointers
	m_pSliderCtrl = NULL;
	m_pScrollBar = NULL;
	m_pGraphStyleCombo = NULL;
	m_pGraphLineCombo = NULL;
	m_pWholeViewBtn = NULL;
	m_pZoomInBtn = NULL;
	m_pZoomOutBtn = NULL;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  CreateCtrl
 - Created at  :  2004-10-26   13:21
 - Author      :  최종찬
 - Parameters  :  pToolOption  - Tool아이템 정보리스트 (WILL: CTypedPtrList<CObList, CObjTool*>*을 인자로 보낼것)
 -				  pFont - 리소스 폰트
 -				  strImageFile - 버튼 이미지 파일명.
 -                nScrollWidth - 스크롤바의 길이.
 -                bRadarChart  - RadarChart(펼침형) 사용여부
 -                bStatusBar   - 수치조회데이타보기(Status Bar) 사용여부
 - Return Value:  bool - true:성공, false:실패.
 - Description :  툴바Dialog에 툴버튼을 생성한다. RadarChart와 Status Bar도 같이 생성함.(내부툴바에
				  등록되어 있을 경우.)
 - Update	   :  종찬(05/04/14) 블랙데이임에도 짱깨를 못먹었다. 아~ 글루미한 내인생. 자동스크롤버튼을 추가한다.
 -----------------------------------------------------------------------------------------*/
bool CToolBarDlg::CreateCtrl(CToolOption* pToolOption, const CFont* pFont, const CString& strImageFile, const int nScrollWidth, const bool bRadarChart, const bool bStatusBar)
{
	DeleteCtrl();	// 툴바DLG를 맵젠에서 재생성하는 프로세싱을 위함.

	m_bStatusBar = bStatusBar;
	m_bRadarChart = bRadarChart;
	m_nScrollWidth = nScrollWidth;

	bool bBitmap = false;
	if(!m_bitmapButton.GetSafeHandle())
		bBitmap = MakeBitMap(strImageFile);	// 비트맵만들기
	else
		bBitmap = true;					//비트맵이미지가 이미 있다.

	CRect rectToolBarDlg;
	GetClientRect(rectToolBarDlg);

	// 컨트롤생성 팩토리
	CCtrlFactory factory( m_pIChartCtrl, this, rectToolBarDlg, bStatusBar, bRadarChart, nScrollWidth, bBitmap);
	factory.Initialize();
	
	CTypedPtrList<CObList, CObjTool*>* pToolList = pToolOption->GetToolOptionList(CToolOptionInfo::INNER);
	POSITION pos = pToolList->GetHeadPosition();
	while(pos)
	{
		CObjTool* pObjTool = pToolList->GetNext(pos);
		switch(pObjTool->GetType())
		{
		case CToolOptionInfo::T_DISPLAY_SLIDERBAR:		// ------------슬라이더바
			{
				m_pSliderCtrl = factory.CreateSliderCtrl();
				m_listToolBarCtrl.AddTail(m_pSliderCtrl);
				break;
			}
		case CToolOptionInfo::T_INQUIRY_STATUS:			// ------------수치조회데이타보기
			{
				CString strOpt1, strOpt2;
				pObjTool->GetOption(strOpt1, strOpt2);
				if(strOpt1 != _T("Status Bar"))	// Status Bar 옵션이 아닐경우에만 버튼 생성.
				{
					CButtonEx* pButton = factory.CreateButton(pObjTool);
					m_listToolBarCtrl.AddTail(pButton);
					m_listToolButton.AddTail(pButton);
				}

				// (2006/6/27 - Seung-Won, Bae) Not Supported Type 2
				// else m_pToolBarManagerImple->CreateNumericalInquiryDlg(2);

				break;
			}
		case CToolOptionInfo::T_SCROLL:					// ------------스크롤
			{
				m_pScrollBar = factory.CreateScrollBar();
				m_listToolBarCtrl.AddTail(m_pScrollBar);
				break;
			}
		case CToolOptionInfo::T_GRAPH_STYLE:			// ------------그래프 형태
			{
				m_pGraphStyleCombo = factory.CreateGraphStyleCombo(pObjTool->GetName());
				m_listToolBarCtrl.AddTail(m_pGraphStyleCombo);
				break;
			}
		case CToolOptionInfo::T_GRAPH_THICKNESS:		// ------------선굵기
			{
				m_pGraphLineCombo = factory.CreatreGraphLineThicknessCombo(pObjTool->GetName());
				m_listToolBarCtrl.AddTail(m_pGraphLineCombo);
				break;
			}
		case CToolOptionInfo::T_RADAR:					// ------------레이더차트
			{
				if(!m_bRadarChart)
				{	// 버튼을 생성
					CButtonEx* pButton = factory.CreateButton(pObjTool);
					m_listToolBarCtrl.AddTail(pButton);
					m_listToolButton.AddTail(pButton);				
				}
				else
				{	// 레이더차트를 생성
					// 이넘은 Position Changed할때 따로 계산하므로 List추가하지 않는다.
					m_pRadarControl = factory.CreateRadarControl();				
					m_pRadarControl->SetIChartCtrl( m_pIChartCtrl);
				}
				break;
			}
		case CToolOptionInfo::T_INFLATE:				// ------------확대/축소
			{
				// (2007/1/5 - Seung-Won, Bae) Change Position for samsung.
				// 축소 버튼 생성
				CButtonEx* pButtonZoomOut = factory.CreateButton(pObjTool, _MTEXT( C4_ZOOM_OUT));
				m_listToolBarCtrl.AddTail(pButtonZoomOut);
				m_listToolButton.AddTail(pButtonZoomOut);		
				// 확대 버튼 생성
				CButtonEx* pButtonZoomIn = factory.CreateButton(pObjTool, _MTEXT( C4_ZOOM_IN));
				m_listToolBarCtrl.AddTail(pButtonZoomIn);
				m_listToolButton.AddTail(pButtonZoomIn);
				// (2007/2/8 - Seung-Won, Bae) Disable Zoom Btn in Special Chart.
				m_pZoomInBtn = pButtonZoomIn;
				m_pZoomOutBtn = pButtonZoomOut;
				break;
			}
		case CToolOptionInfo::T_AUTOSCROLL:				// ------------자동스크롤.
			{
				// 왼쪽스크롤 버튼 생성
				CButtonEx* pButtonLeftScroll = factory.CreateButton(pObjTool, _MTEXT( C4_LEFT_AUTO_SCROLL));
				m_listToolBarCtrl.AddTail(pButtonLeftScroll);
				m_listToolButton.AddTail(pButtonLeftScroll);
				// 자동스크롤 정지 버튼 생성
				CButtonEx* pButtonStop = factory.CreateButton(pObjTool, _MTEXT( C4_STOP_AUTO_SCROLL));
				m_listToolBarCtrl.AddTail(pButtonStop);
				m_listToolButton.AddTail(pButtonStop);
				//오른쪽스크롤 버튼 생성
				CButtonEx* pButtonRightScroll = factory.CreateButton(pObjTool, _MTEXT( C4_RIGHT_AUTO_SCROLL));
				m_listToolBarCtrl.AddTail(pButtonRightScroll);
				m_listToolButton.AddTail(pButtonRightScroll);
				break;
			}
		default:										// ------------위에 명시한 case이외의 것들은 모두 버튼이다. 버튼아닌 넘들은 나가있어! ^^;
			{
				CButtonEx* pButton = factory.CreateButton(pObjTool);
				m_listToolBarCtrl.AddTail(pButton);
				m_listToolButton.AddTail(pButton);
				// (2007/2/8 - Seung-Won, Bae) Disable WholeView Btn in Special Chart.
				if( CToolOptionInfo::T_WHOLEVIEW == pObjTool->GetType()) m_pWholeViewBtn = pButton;
				break;
			}
		}
	}

	// 등록된 버튼이 있으면 ButtonRelation클래스오브젝트 생성.
	if(m_listToolButton.GetCount() > 0)
	{
		if(m_pButtonProperty)
		{
			delete m_pButtonProperty;
			m_pButtonProperty = NULL;
		}
		m_pButtonProperty = new CButtonProperty(this, &m_listToolButton);
	}

	return true;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  MakeBitMap
 - Created at  :  2004-10-21   15:22
 - Author      :  최종찬
 - Parameters  :  strFileName - 버튼이미지 파일.
 - Return Value:  bool - true성공, false실패
 - Description :  버튼이미지 파일로 부터 Bitmap을 만든다.
 -----------------------------------------------------------------------------------------*/
bool CToolBarDlg::MakeBitMap(const CString& strFileName)
{
	if(strFileName.IsEmpty())
		return false;

	HBITMAP hBitmap = NULL;
	hBitmap = (HBITMAP)LoadImage(NULL, strFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);

	return (m_bitmapButton.Attach(hBitmap) ? true : false);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  MoveToolBarDlg
 - Created at  :  2004-10-29   13:25
 - Author      :  최종찬
 - Parameters  :  rectNew - 새로운 툴바영역
 - Return Value:  None
 - Description :  차트창의 Resizing이나 이동시 호출.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::MoveToolBarDlg( void)
{
	// 움직인 가로길이의 차이를 구한다.
	CRect rectOld;
	GetClientRect(rectOld);
	CRect rectNew = m_pToolBarManagerImple->GetToolBarRect();
	int nSubWidth = rectNew.Width() - rectOld.Width();
	// 새로운 Dialog의 높이를 세팅한다.
	rectNew.top = rectNew.bottom - rectOld.Height();

	// 툴버튼이 없을경우는 ToolBar Dlg영역도 없다!
	if(m_listToolBarCtrl.GetCount() == 0)
		rectNew.top = rectNew.bottom;

	// 1. ToolBar Dialog를 이동시킨다.
	MoveWindow( rectNew);
	m_pToolBarManagerImple->SetToolBarRect( rectNew);

	// 2. ToolBar Dialog에 등록된 Ctrl들을 이동시킨다.
	CRect rectCtrl(0, 0, 0, TB_BUTTON_HEIGHT);

	// 수치조회데이타보기(StatusBar)가 있으면 높이 추가
	if(m_bStatusBar) 
	{
		rectCtrl.top += TB_BUTTON_HEIGHT;
		rectCtrl.bottom += TB_BUTTON_HEIGHT;
	}
	
	POSITION pos = m_listToolBarCtrl.GetHeadPosition();
	while(pos)
	{
		CWnd* pWnd = m_listToolBarCtrl.GetNext(pos);

		if(pWnd == NULL)
			continue;
		switch(pWnd->GetDlgCtrlID())
		{
		case IDC_SCROLL:
			rectCtrl.right = rectCtrl.left + m_nScrollWidth + nSubWidth;
			m_nScrollWidth = rectCtrl.Width();	// 스크롤바 넓이 재 세팅.
			((CScrollBarEx*)pWnd)->SetScrollBarWidth(m_nScrollWidth);
			pWnd->MoveWindow(rectCtrl);
			break;
		case IDC_SLIDER:
			rectCtrl.right = rectCtrl.left + TB_BUTTON_SLIDERBAR;
			pWnd->MoveWindow(rectCtrl);
			break;
		case IDC_GRAPHSTYLE_COMBO:		// 그래프스타일
		case IDC_LINETHICKNESS_COMBO:	// 선굵기
			rectCtrl.bottom += 100;
			rectCtrl.right = rectCtrl.left + (TB_BUTTON_WIDTH * 3);
			pWnd->MoveWindow(rectCtrl);
			rectCtrl.bottom -= 100;
			break;
		case IDC_RADAR:
			if(!m_bRadarChart)	// 버튼형.
				rectCtrl.right = rectCtrl.left + TB_BUTTON_WIDTH;
			pWnd->MoveWindow(rectCtrl);
			break;
		default:
			rectCtrl.right = rectCtrl.left + TB_BUTTON_WIDTH;
			pWnd->MoveWindow(rectCtrl);
			break;
		}//end of switch

		rectCtrl.left = rectCtrl.right;
	}//end of while

	if(m_pRadarControl)
		m_pRadarControl->OnToolBarPositionChanged(rectNew);
	
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetScrollData_TR
 - Created at  :  2004-10-27   13:52
 - Author      :  최종찬
 - Parameters  :  nTotalDataCount      - 총데이타 갯수
 -                nPageDataCount       - 한페이지에 보여줄 데이타 갯수
 -                bFullPage            - 전체보기인가?
 -                dataType             - 
 -                nGlanceBalanceMargin - 일목균형표에서의 추가되는 데이타갯수(SliderCtrl의 Max Range용)
 - Return Value:  None
 - Description :  초기TR을 받았을때 ScrollBar 세팅. SliderCtrl에 Min/Max값과 Position 세팅.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetScrollData_TR(const int nTotalDataCount, 
								   const int nPageDataCount, 
								   const bool bFullPage, 
								   CPacketDataInfo::DATAINSERTTYPE dataType, 
								   const int nGlanceBalanceMargin)
{
	// 0. Process Scale with None-Data TR as with nPageDataCount Data TR
	if(nTotalDataCount == 0)
	{
		//sy 2003.7.15.
		//전체 data보기가 아닌 경우 한화면에 보이는 data 갯수로 설정한다.
		//-> TR이 없고 처음 실시간이 들어오는 경우에 한화면에 꽉차게 되는 현상 수정
		if(!bFullPage && nPageDataCount > 0)
		{
			SetScrollData_TR(nPageDataCount, nPageDataCount, bFullPage, dataType, nGlanceBalanceMargin);
			return;
		}
	}

	// 1. Check Minimum Limitation of One-Page-Data-Count.
	// (2003.11.12, 배승원) Map에서 설정한 '한 화면 최소 Data 수'를 SlideBar에 적용한다.
	int nPageDataCountTmp = nPageDataCount;
	if( nPageDataCountTmp < m_nSliderMin) 
		nPageDataCountTmp = m_nSliderMin;

	// 2. with GlanceBalance
	// NOTE: 종찬(04/11/17) MaxRange 구하는 부분. 이부분 이렇게 복잡해질 필요가 없을듯하기도 하다.
	//		나중에 개선해 볼것!
	if(nGlanceBalanceMargin != 0)
	{// 일목균형표 체크시
		if(nPageDataCountTmp > nTotalDataCount)
			m_nSliderMax = nPageDataCountTmp;
		else
			m_nSliderMax = nTotalDataCount;

		m_nGlanceBalanceMargin = nGlanceBalanceMargin;
	}
	else if(m_nGlanceBalanceMargin != 0 && nGlanceBalanceMargin == 0)
	{// 일목균형표 언체크시
		if(m_nRemainCount <= 0)
		{
			m_nSliderMax = nTotalDataCount;

			if(nPageDataCountTmp > nTotalDataCount)
				nPageDataCountTmp = nTotalDataCount;
		}
		else if(m_nRemainCount > m_nGlanceBalanceMargin)
			m_nSliderMax = nPageDataCountTmp;
		else if( m_nRemainCount > 0 && m_nRemainCount < m_nGlanceBalanceMargin)
		{
			m_nSliderMax = nTotalDataCount + m_nRemainCount;
			nPageDataCountTmp = m_nSliderMax;
		}

		if(m_bCheckGlanceBalance)
		{
			m_nGlanceBalanceMargin = 0;
			m_bCheckGlanceBalance = FALSE;
		}
		else
			m_bCheckGlanceBalance = TRUE;			
	}
	else if(nGlanceBalanceMargin == 0)
	{
		// MODIFY: 종찬(04/07/28) '전체보기'선택시 SliderMaxRange변경
		if(bFullPage)
			m_nSliderMax = nTotalDataCount;
		else if(nPageDataCountTmp > nTotalDataCount)
			m_nSliderMax = nPageDataCountTmp;
		else
			m_nSliderMax = nTotalDataCount;
		// MODIFY: 04/07/28

		// MODIFY: 종찬(04/07/21) 여백의 갯수를 구함. 순전히 일목균형표때문이얏!!! T_T
		m_nRemainCount = m_nSliderMax - nTotalDataCount;
	}//else if(nGla...

	// (2007/2/8 - Seung-Won, Bae) Disable WholeView Btn in Special Chart.
	if( m_pWholeViewBtn) m_pWholeViewBtn->EnableWindow( !m_bIsNoZoomChart);
	// (2007/2/8 - Seung-Won, Bae) Disable Zoom Btn in Special Chart.
	if( m_pZoomInBtn) m_pZoomInBtn->EnableWindow( !m_bIsNoZoomChart);
	if( m_pZoomOutBtn) m_pZoomOutBtn->EnableWindow( !m_bIsNoZoomChart);

	// 3. with SlideBar
	if(m_pSliderCtrl)
	{
		m_pSliderCtrl->SetRange(m_nSliderMin, m_nSliderMax, true);

		m_pSliderCtrl->EnableWindow( !m_bIsNoZoomChart);

		if( m_bOneChart) m_pSliderCtrl->SetPos( nTotalDataCount);
		else
		{
			if( bFullPage)	m_pSliderCtrl->SetPos( nTotalDataCount);
			else			m_pSliderCtrl->SetPos( nPageDataCountTmp);
		}

	}//if(m_pSliderCtrl...

	// 4. with ScrollBar
	if(!m_pScrollBar) return;
	m_pScrollBar->SetScrollData_TR(nTotalDataCount, nPageDataCountTmp, bFullPage, dataType, m_nGlanceBalanceMargin);
	
	// (2007/2/8 - Seung-Won, Bae) Disable ScrollBar in Special Chart.
	m_pScrollBar->EnableWindow( !m_bIsNoZoomChart);

	// 5. with Radar Control
	if(m_pRadarControl)
	{
		// 종찬(04/09/01) 초기 차트윈도우 생성시 아래 레이더차트용 startIndex를 제대로 주기 위해서...
		int nStartIndex = 0;
		int nEndIndex = 0;
		m_pScrollBar->GetDataStartEnd(nStartIndex, nEndIndex);
		// 종찬(04/11/29) 일목균형표에 의한 추가Data분을 제거해주어야 한다.
		m_pRadarControl->SetDataViewInfo(nTotalDataCount - nGlanceBalanceMargin, nStartIndex, nEndIndex - nStartIndex + 1, m_bOneChart);
	}
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetStartEndIndexToOcx
 - Created at  :  2004-10-27   18:01
 - Author      :  최종찬
 - Description :  스크롤바에서 계산한 Start/End Index를 OCX에 보낸다.
 - Update	   :  (11/05) Radar차트에도 스크롤바에서 계산된 Index를 세팅한다.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetStartEndIndexToOcx(const int nStartIndex, const int nEndIndex)
{
	if(m_pToolBarManagerImple)
		m_pToolBarManagerImple->SetStartEndIndexToOcx(nStartIndex, nEndIndex);
	
	// Radar차트가 있을경우. Radar Chart에도 해당 인덱스로 세팅한다. 웅햐햐햐~~~
	if(m_pRadarControl)
		m_pRadarControl->SetDataViewInfo(m_pPacketList->GetMaxCount(), nStartIndex, nEndIndex - nStartIndex + 1, m_bOneChart);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetPageCountNextStepToOcx
 - Created at  :  
 - Author      :  yiilyoul@magicn.com
 - Description :  
 - Update	   :  
 -----------------------------------------------------------------------------------------*/
int CToolBarDlg::GetPageCountNextStepToOcx(const int nSliderPos_Prev, const int nSliderPos_New, const UINT nSBCode, const int nMin, const int nMax)
{
	if(m_pToolBarManagerImple)
	{
		return m_pToolBarManagerImple->GetPageCountNextStepToOcx(nSliderPos_Prev, nSliderPos_New, nSBCode, nMin, nMax);
	} else return nSliderPos_New;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetReverseToOcx
 - Created at  :  
 - Author      :  yiilyoul@magicn.com
 - Description :  
 - Update	   :  
 -----------------------------------------------------------------------------------------*/
BOOL CToolBarDlg::GetReverseToOcx()
{
	if(m_pToolBarManagerImple)
	{
		return m_pToolBarManagerImple->GetReverseToOcx();
	}
	
	return FALSE;
}

/*-------------------------------------------------------------------------------
 - Function    :  ExecuteZoomInZoomOut
 - Created at  :  2005-04-15   14:04
 - Author      :  최종찬
 - Description :  Zoom In/Zoom Out 수행
 -------------------------------------------------------------------------------*/
void CToolBarDlg::ExecuteZoomInZoomOut(CButtonEx* pButton)
{
	if(pButton == NULL)
		return;

	int nZoomCount = 0;
	bool bCount = true;

	CString strOption2 = pButton->GetOption2();
	bCount = ( pButton->GetOption1() == _MTEXT( C4_COUNT));
	CString strPrefix = strOption2.SpanExcluding( "0123456789");
	nZoomCount = atoi( strOption2.Mid( strPrefix.GetLength()).SpanIncluding( "0123456789"));

	if( !bCount) nZoomCount = ( m_nSliderMax * nZoomCount) / 100;
	if(pButton->GetParam() == _MTEXT( C4_ZOOM_IN))	// 확대
			m_pIChartCtrl->ZoomTo( -nZoomCount, TRUE);
	else	m_pIChartCtrl->ZoomTo( nZoomCount, TRUE);
}


/*-------------------------------------------------------------------------------
 - Function    :  ExecuteRadar
 - Created at  :  2005-04-15   14:07
 - Author      :  최종찬
 - Description :  Radar Chart를 실행한다.
 -------------------------------------------------------------------------------*/
void CToolBarDlg::ExecuteRadar()
{
	// 이미 레이다차트가 있으면 아무것도 하지 않으면 된다.
	if(m_pRadarControl)
		return;

	// 툴바에 Radar Chart 추가한 Rect값을 다시 계산한다.
	m_bUseRadarChart = TRUE;
	
	CRect rectToolBarDlg( m_pToolBarManagerImple->GetToolBarRect());
	rectToolBarDlg.top -= TB_RADAR_HEIGHT;
	MoveWindow( rectToolBarDlg);
	m_pToolBarManagerImple->SetToolBarRect( rectToolBarDlg);

	m_pRadarControl = new CRadarControl();
	m_pRadarControl->Create(this, IDC_RADAR);
	m_pRadarControl->SetIChartCtrl( m_pIChartCtrl);

	int nStartIndex = 0;
	int nEndIndex = 0;
	if(m_pScrollBar)
		m_pScrollBar->GetDataStartEnd(nStartIndex, nEndIndex);
	else
	{
		nStartIndex = 0;
		nEndIndex = m_pPacketList->GetMaxCount() - 1;
	}

	m_pRadarControl->SetDataViewInfo(m_pPacketList->GetMaxCount(), nStartIndex, nEndIndex - nStartIndex + 1, m_bOneChart);
	if( m_pIChartCtrl) m_pIChartCtrl->InvalidateOleControl(true);
}


/*-------------------------------------------------------------------------------
 - Function    :  ExecuteGraphColorSetting
 - Created at  :  2005-04-15   14:11
 - Author      :  최종찬
 - Description :  그래프색상설정창을 실행한다.
 -------------------------------------------------------------------------------*/
void CToolBarDlg::ExecuteGraphColorSetting()
{
	CColorDialog dlgColor(m_clrSelection);
	if(dlgColor.DoModal() != IDOK)
		return;
	m_clrSelection = dlgColor.GetColor();
	
	if( m_pIChartCtrl) m_pIChartCtrl->RunToolFuncGraphColor(m_clrSelection);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetToolFunction
 - Created at  :  2004-11-09   15:34
 - Author      :  최종찬
 - Description :  버튼Ctrl의 툴기능을 수행한다. (OCX에 기능구현된 함수들을 호출한다.)
				  참고로 툴기능 수행에 필요한 최소 데이타는 nParam이나 pButton클래스자체내에 있다.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetToolFunction(CToolOptionInfo::TOOLOPTION enumToolOption, CButtonEx* pButton /* = NULL*/, long nParam /* = 0*/)
{
	switch(enumToolOption) 
	{
		case CToolOptionInfo::T_INFLATE:	// 확대, 축소
			ExecuteZoomInZoomOut(pButton);
			break;
		case CToolOptionInfo::T_AUTOSCROLL:	// 자동 스크롤
			// (2006/11/25 - Seung-Won, Bae) Support New Scroll ID with Each Button.
			if( !pButton || !m_pIChartCtrl) break;
			if(			pButton->GetParam() == _MTEXT( C4_LEFT_AUTO_SCROLL))	m_pIChartCtrl->RunToolFunction( CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_LEFT,	1);
			else if(	pButton->GetParam() == _MTEXT( C4_STOP_AUTO_SCROLL))			m_pIChartCtrl->RunToolFunction( CToolOptionInfo::T_ADDIN_AUTOSCROLL_STOP,		1);
			else if(	pButton->GetParam() == _MTEXT( C4_RIGHT_AUTO_SCROLL))	m_pIChartCtrl->RunToolFunction( CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_RIGHT,	1);
			break;
		case CToolOptionInfo::T_GRAPH_STYLE:				// 그래프스타일
			if( m_pIChartCtrl) m_pIChartCtrl->RunToolFuncGraphStyle(nParam);
			break;
		case CToolOptionInfo::T_GRAPH_THICKNESS:			// 선굵기
			if( m_pIChartCtrl) m_pIChartCtrl->RunToolFuncLineThickness(nParam);
			break;
		case CToolOptionInfo::T_RADAR:						// 레이다차트 생성
			ExecuteRadar();
			break;
		case CToolOptionInfo::T_GRAPH_COLOR:				// 색설정
			ExecuteGraphColorSetting();
			break;

		default:
			if( m_pIChartCtrl) m_pIChartCtrl->RunToolFunction(enumToolOption, 1);
			break;
	}//end of switch
}


/*-----------------------------------------------------------------------------------------
 - Function    :  ReleaseToolFunction
 - Created at  :  2004-11-09   15:34
 - Author      :  최종찬
 - Description :  버튼Ctrl의 툴기능 수행을 해제한다.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::ReleaseToolFunction(CToolOptionInfo::TOOLOPTION enumToolOption, CButtonEx* pButton /* = NULL*/, long nParam /* = 0*/)
{
	switch(enumToolOption)
	{
		// 레이다차트
		case CToolOptionInfo::T_RADAR:	if(m_pRadarControl)
										{
											// 툴바에서 Radar Chart 지운 Rect를 다시 계산한다.
											m_bUseRadarChart = FALSE;

											CRect rectToolBarDlg( m_pToolBarManagerImple->GetToolBarRect());
											rectToolBarDlg.top += TB_RADAR_HEIGHT;
											MoveWindow( rectToolBarDlg);
											m_pToolBarManagerImple->SetToolBarRect( rectToolBarDlg);

											delete m_pRadarControl;
											m_pRadarControl = NULL;

											if( m_pIChartCtrl) m_pIChartCtrl->InvalidateOleControl(true);
										}
										break;

		case CToolOptionInfo::T_AUTOSCROLL:	// 자동 스크롤
			// (2006/11/25 - Seung-Won, Bae) Support New Scroll ID with Each Button.
			if( !pButton || !m_pIChartCtrl) break;
			if(			pButton->GetParam() == _MTEXT( C4_LEFT_AUTO_SCROLL))	m_pIChartCtrl->RunToolFunction( CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_LEFT,	0);
			else if(	pButton->GetParam() == _MTEXT( C4_RIGHT_AUTO_SCROLL))	m_pIChartCtrl->RunToolFunction( CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_RIGHT,	0);
			break;

		default:	if( m_pIChartCtrl) m_pIChartCtrl->RunToolFunction( enumToolOption, 0);
					break;
	}
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetOneChart
 - Created at  :  2004-11-05   11:03
 - Author      :  최종찬
 - Description :  단일지표차트일경우를 세팅한다. true:단일지표차트.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetOneChart( bool bOneChart, bool p_bIsNoZoomChart)
{
	m_bOneChart = bOneChart;
	// (2009/11/6 - Seung-Won, Bae) Do not disable zoom and scroll on OneChart
	m_bIsNoZoomChart = p_bIsNoZoomChart;

	// (2007/2/8 - Seung-Won, Bae) Disable WholeView Btn in Special Chart.
	if( m_pWholeViewBtn) m_pWholeViewBtn->EnableWindow( !m_bIsNoZoomChart);
	// (2007/2/8 - Seung-Won, Bae) Disable Zoom Btn in Special Chart.
	if( m_pZoomInBtn) m_pZoomInBtn->EnableWindow( !m_bIsNoZoomChart);
	if( m_pZoomOutBtn) m_pZoomOutBtn->EnableWindow( !m_bIsNoZoomChart);

	// 3. with SlideBar
	if(m_pSliderCtrl) m_pSliderCtrl->EnableWindow( !m_bIsNoZoomChart);

	// 4. with ScrollBar
	if( m_pScrollBar) m_pScrollBar->EnableWindow( !m_bIsNoZoomChart);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetOneChart
 - Created at  :  2004-11-05   11:05
 - Author      :  최종찬
 - Description :  현재 단일지표차트상태인지 리턴한다. true:단일지표차트.
 -----------------------------------------------------------------------------------------*/
bool CToolBarDlg::GetOneChart()
{
	return m_bOneChart;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetCountMinPerPage
 - Created at  :  2004-11-04   10:26
 - Author      :  최종찬
 - Description :  한화면의 최소 데이타 수를 가져온다. (For Radar Chart)
 -----------------------------------------------------------------------------------------*/
int CToolBarDlg::GetCountMinPerPage()
{
	if( m_pIChartCtrl) return m_pIChartCtrl->GetMinCountPerPage();
	return -1;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetDataCount
 - Created at  :  2004-10-28   14:20
 - Author      :  최종찬
 - Description :  실제데이타갯수(여백제외)를 구해온다.
 -----------------------------------------------------------------------------------------*/
int CToolBarDlg::GetDataCount()
{
	if( m_pIChartCtrl) return m_pIChartCtrl->GetDataCountForToolBar();
	return -1;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetControlData
 - Created at  :  2004-11-04   11:30
 - Author      :  최종찬
 - Parameters  :  pPacketList - 패킷리스트
 - Return Value:  None
 - Description :  OCX로부터 가져온 컨트롤데이타를 세팅한다.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetControlData(CPacketList* pPacketList, CPacketListManager* pPacketListManager, CXScaleManager* pxScaleManager)
{
	m_pPacketList = pPacketList;
	m_pPacketListManager = pPacketListManager;
	m_pxScaleManager = pxScaleManager;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetScrollData_Real
 - Created at  :  2004-10-28   15:04
 - Author      :  최종찬
 - Parameters  :  nTotalDataCount - 총데이타갯수
 -                nPageDataCount  - 한페이지에 보일 데이타 갯수
 -                bFullPage       - 전체보기 유무
 - Return Value:  None
 - Description :  RealData가 들어왔을때 SliderCtrl과 ScrollBar를 조정한다.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetScrollData_Real(const int nTotalDataCount, const int nPageDataCount, const bool bFullPage)
{
	// 1. Process Scroll with ScrollBar
	if(m_pScrollBar)	// ScrollBar가 없으면 이 함수 자체를 안타지만 그래도 안전장치.
		m_pScrollBar->SetScrollData_Real(nTotalDataCount);

	// 2. Backup m_nSliderMax
	// MODIFY: 종찬(04/07/28) '전체보기'에서는 TotalDataCount가 SliderMaxRange이다.
	if(bFullPage)
		m_nSliderMax = nTotalDataCount;
	else if( m_nSliderMax < nPageDataCount)
		m_nSliderMax = nPageDataCount;
	else if(m_nSliderMax < nTotalDataCount)
		m_nSliderMax = nTotalDataCount;

	// 3. Update Slider Control
	if( m_pSliderCtrl)
	{
		m_pSliderCtrl->SetRange(m_nSliderMin, m_nSliderMax, true);
		if(bFullPage) 
			m_pSliderCtrl->SetPos(nTotalDataCount);
	}
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetScrollData_RemovePacket
 - Created at  :  2004-10-28   15:11
 - Author      :  최종찬
 - Parameters  :  nTotalDataCount  - 총데이타갯수
 -                bFullPage        - 전체보기 유무
 -                nRemoveDataCount - 지워진 데이타 갯수
 - Return Value:  None
 - Description :  PacketData가 많아서 처리하기가 귀찮아질때 이전 패킷data를 지우게 되는데 이때
				  호출되어 ScrollBar와 SliderCtrl을 조정한다.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetScrollData_RemovePacket(const int nTotalDataCount, const bool bFullPage, const int nRemoveDataCount)
{
	// 0. Update SlideBar
	m_nSliderMax -= nRemoveDataCount;
	if(m_pSliderCtrl)
	{
		m_pSliderCtrl->SetRange(m_nSliderMin, m_nSliderMax, true);
		if(bFullPage)
			m_pSliderCtrl->SetPos(nTotalDataCount);
	}

	// 1. Scroll with ScrollBar
	if(m_pScrollBar) m_pScrollBar->SetScrollData_RemovePacket(nTotalDataCount, nRemoveDataCount);	
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetSliderPos
 - Created at  :  2004-10-28   15:49
 - Author      :  최종찬
 - Description :  SliderCtrl에 PageDataCount를 세팅하고 OCX에 알려준다.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetSliderPos(int nPageDataCount)
{
	//sy working -> "전체보기" 일 경우 : 토글
	ToggleBT(CToolOptionInfo::T_WHOLEVIEW, IsWholeView());

	if(!m_pSliderCtrl || !m_pSliderCtrl->GetSafeHwnd())
		return;

	m_pSliderCtrl->SetPos(nPageDataCount);
	m_pSliderCtrl->Invalidate();
}

//sy 2005.12.09.
void CToolBarDlg::ToggleBT(const CToolOptionInfo::TOOLOPTION eToolOption, const bool bState)
{
	CButtonEx* pButtonEx = GetButtonEx(eToolOption);
	if(pButtonEx == NULL)
		return;

	if(pButtonEx->GetState() == bState)
		return;

	pButtonEx->SetState(bState);
	pButtonEx->Invalidate();
}
//sy end

/*-----------------------------------------------------------------------------------------
 - Function    :  SetSliderMin
 - Created at  :  2004-10-29   15:25
 - Author      :  최종찬
 - Description :  Slider의 최소값을 세팅한다.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetSliderMin(const int nSliderMin)
{
	m_nSliderMin = nSliderMin;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnHScroll
 - Created at  :  2004-11-01   14:34
 - Author      :  최종찬
 - Description :  Scroll과 SliderBar의 HScroll작업을 처리한다.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if( !pScrollBar) return;

	if( pScrollBar->GetDlgCtrlID() == IDC_SCROLL)
	{
		m_pScrollBar->OnHScroll(nSBCode, nPos, pScrollBar);
		return;
	}

    if( pScrollBar->GetDlgCtrlID() == IDC_SLIDER && nSBCode != SB_ENDSCROLL)
    {
    	CSliderCtrl *pSlider = ( CSliderCtrl *)pScrollBar;
    	int nCount = m_pScrollBar->GetPageDataCount();
    	int nSliderPos = m_pSliderCtrl->GetPos();
    	
   		if( m_pToolBarManagerImple && (nSBCode == SB_PAGELEFT || nSBCode == SB_PAGERIGHT || nSBCode == SB_THUMBTRACK))
  		{
  			int nSliderPos_Prev;
  			int nSliderPageSize = m_pSliderCtrl->GetPageSize();
  			int nSliderMin, nSliderMax;
  			m_pSliderCtrl->GetRange(nSliderMin, nSliderMax);

  			switch(nSBCode)
  			{
  			case SB_PAGELEFT:	
  				{
  					nSliderPos_Prev = nSliderPos + nSliderPageSize;
  					if(nSliderPos_Prev > nSliderMax) nSliderPos_Prev = nSliderMax;
  				}
  				break;

  			case SB_PAGERIGHT:
  				{
  					nSliderPos_Prev = nSliderPos - nSliderPageSize;
  					if(nSliderPos_Prev < nSliderMin) nSliderPos_Prev = nSliderMin;
  				}
  				break;
  			}

  			nSliderPos = GetPageCountNextStepToOcx(nSliderPos_Prev, nSliderPos, nSBCode);
  		}

		m_pIChartCtrl->ZoomTo( nSliderPos - nCount, TRUE);
    }
}



/*-----------------------------------------------------------------------------------------
 - Function    :  GetStartEndIndex
 - Created at  :  2004-11-01   14:35
 - Author      :  최종찬
 - Parameters  :  nStartIndex - 데이타의 시작인덱스
 -                nEndIndex   - 데이타의 끝인덱스
 - Return Value:  bool : true성공, false실패.
 - Description :  ScrollBar가 가지고 있는 Start/End인덱스를 가져온다.
 -----------------------------------------------------------------------------------------*/
bool CToolBarDlg::GetStartEndIndex(int& nStartIndex, int& nEndIndex)
{
	if(m_pScrollBar)
	{
		m_pScrollBar->GetDataStartEnd(nStartIndex, nEndIndex);
		return true;
	}
	else
		return false;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetGraphType
 - Created at  :  2004-11-01   16:32
 - Author      :  최종찬
 - Parameters  :  graphType - 현재 선택된 그래프타입.
 -                nIndex    - 인덱스
 - Return Value:  None
 - Description :  OCX에서 선택한 그래프스타일에 따라 콤보에 해당스타일형으로 세팅한다.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetGraphType(CGraphBaseData::GRAPHTYPE graphType, int nIndex)
{
	if(m_pGraphStyleCombo)
		m_pGraphStyleCombo->SetGraphType(graphType, nIndex);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetLineThickness
 - Created at  :  2004-11-03   11:25
 - Author      :  최종찬
 - Parameters  :  nThickness - 선두께 갯수.
 - Description :  OCX에서 선택한 라인두께설정값에 따라 콤보에 해당 스타일 설정값으로 바꾼다.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetLineThickness(int nThickness)
{
	if(m_pGraphLineCombo)
		m_pGraphLineCombo->SetLineThickness(nThickness);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetGraphColor
 - Created at  :  2004-11-29   17:07
 - Author      :  최종찬
 - Description :  OCX에서 선택한 그래프의 색상을 멤버변수에 세팅한다.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetGraphColor(COLORREF clrGraphColor)
{
	m_clrSelection = clrGraphColor;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetPacketList
 - Created at  :  2004-11-04   11:32
 - Author      :  최종찬
 - Description :  패킷리스트를 리턴한다.(Radar Chart에서 사용)
 -----------------------------------------------------------------------------------------*/
CPacketList* CToolBarDlg::GetPacketList()
{
	return m_pPacketList;
}

CPacketListManager* CToolBarDlg::GetPacketListManager()
{
	return m_pPacketListManager;
}

CXScaleManager* CToolBarDlg::GetXScaleManager()
{
	return m_pxScaleManager;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetResourceFont
 - Created at  :  2004-11-04   11:39
 - Author      :  최종찬
 - Description :  리소스 폰트를 가져온다.
 -----------------------------------------------------------------------------------------*/
CFont* CToolBarDlg::GetResourceFont()
{
	if( m_pIChartCtrl) return m_pIChartCtrl->GetRscFont();
	return NULL;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  IsWholeView
 - Created at  :  2004-11-04   13:48
 - Author      :  최종찬
 - Description :  OCX가 현재 전체보기인지를 리턴한다.
 -----------------------------------------------------------------------------------------*/
bool CToolBarDlg::IsWholeView()
{
	if( m_pIChartCtrl) return m_pIChartCtrl->IsWholeView();
	return false;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetStateWholeView
 - Created at  :  2004-11-22   15:58
 - Author      :  최종찬
 - Description :  전체보기 상태를 OCX에 세팅한다.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetStateWholeView(const int nState)
{
	if( m_pIChartCtrl) m_pIChartCtrl->SetStateWholeView(nState);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  ReleaseOtherButtonUI
 - Created at  :  2004-11-11   11:08
 - Author      :  최종찬
 - Parameters  :  enumToolOption - 릴리즈 시킬 Tool버튼의 Enum값
 -                strParam - 하나의 툴옵션을 공유하는 버튼을 구분하기 위한 값. (ex: 확대/축소)
 - Description :  해당버튼을 릴리즈 시킨다.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::ReleaseOtherButtonUI(CToolOptionInfo::TOOLOPTION enumToolOption, const CString& strParam /*= _T("")*/)
{
	if(m_pButtonProperty)
		m_pButtonProperty->ReleaseOtherButtonUI(enumToolOption, strParam);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  IsScrollBar
 - Created at  :  2004-11-17   13:29
 - Author      :  최종찬
 - Description :  스크롤바를 사용하는지를 리턴한다.
 -----------------------------------------------------------------------------------------*/
bool CToolBarDlg::IsScrollBar()
{
	if(m_pScrollBar == NULL)
		return false;
	return m_pScrollBar->GetSafeHwnd() ? true : false;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetMaxRange
 - Created at  :  2004-11-17   14:59
 - Author      :  최종찬
 - Description :  MaxRange를 리턴한다.
 -----------------------------------------------------------------------------------------*/
int CToolBarDlg::GetMaxRange()
{
	return m_nSliderMax;
}



/*-----------------------------------------------------------------------------------------
 - Function    :  GetButtonEx
 - Created at  :  2004-12-02   15:03
 - Author      :  최종찬
 - Description :  해당 툴옵션이 지정되어 있는 버튼 포인터를 얻어온다.
 -----------------------------------------------------------------------------------------*/
CButtonEx* CToolBarDlg::GetButtonEx(CToolOptionInfo::TOOLOPTION enumToolOption)
{
	// (2006/11/25 - Seung-Won, Bae) Support Auto Scroll Tool Button
	CToolOptionInfo::TOOLOPTION eToolOption2 = enumToolOption;
	if( enumToolOption == CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_LEFT || enumToolOption == CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_RIGHT)
		enumToolOption = CToolOptionInfo::T_AUTOSCROLL;

	POSITION pos = m_listToolButton.GetHeadPosition();
	CButtonEx* pButton = NULL;
	while(pos)
	{
		pButton = m_listToolButton.GetNext(pos);
		if( enumToolOption == pButton->GetToolOptionInfo())
		{
			// (2006/11/25 - Seung-Won, Bae) Support Auto Scroll Tool Button
			if( enumToolOption != CToolOptionInfo::T_AUTOSCROLL) return pButton;
			if( eToolOption2 == CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_LEFT && pButton->GetParam() == _MTEXT( C4_LEFT_AUTO_SCROLL)) return pButton;
			if( eToolOption2 == CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_RIGHT && pButton->GetParam() == _MTEXT( C4_RIGHT_AUTO_SCROLL)) return pButton;
		}
	}
	return NULL;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  ReDraw
 - Created at  :  2005-08-16   19:02
 - Author      :  최종찬
 - Description :  툴바 Invalidate.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::ReDraw()
{
	POSITION pos = m_listToolBarCtrl.GetHeadPosition();
	while(pos)
	{
		CWnd* pWnd = m_listToolBarCtrl.GetNext(pos);

		if(pWnd == NULL)
			continue;
		pWnd->Invalidate();
	}
}

// (2009/1/5 - Seung-Won, Bae) Update ScrollBar and Zoom SliderBar from XScaleManager by Real.
void CToolBarDlg::UpdateZoomAndScrollInfo( int p_nFullRange, int p_nStartPos, int p_nViewCount, BOOL p_bWholeView)
{
	CButtonEx* pButton = m_pWholeViewBtn;
	if( pButton && pButton->GetSafeHwnd()) pButton->SetState( p_bWholeView ? true : false);

	if( m_pScrollBar) if( m_pScrollBar->GetSafeHwnd())
		m_pScrollBar->UpdateZoomAndScrollInfo( p_nFullRange, p_nStartPos, p_nViewCount);
	if( m_pSliderCtrl) if( m_pSliderCtrl->GetSafeHwnd())
	{
		m_nSliderMax = p_nFullRange;
		m_pSliderCtrl->SetRange( m_nSliderMin, m_nSliderMax, true);
		m_pSliderCtrl->SetPos( p_nViewCount);
	}
}
