// ToolBarManagerImplementation.cpp: implementation of the CToolBarManagerImplementation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToolBarManagerImplementation.h"

#include <fcntl.h>
#include <io.h>

#include "../Include_Chart/Dll_Load/PacketList.h"
#include "../Include_Chart/Dll_Load/MainBlock.h"
#include "../Include_Chart/Dll_Load/IndicatorList.h"
#include "../Include_Chart/Dll_Load/XScaleManager.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/IChartCtrl.h"					// for IChartCtrl
#include "Definition.h"
#include "ToolBarDlg.h"
#include "ToolOption.h"
#include "ObjTool.h"
#include "ButtonEx.h"
#include "ViewData.h"
#include "Conversion.h"
#include "Resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToolBarManagerImplementation::CToolBarManagerImplementation( IChartCtrl *p_pIChartCtrl)
{
	m_pIChartCtrl = p_pIChartCtrl;
	m_pToolBarDlg = NULL;
	m_pToolOption = NULL;
	m_nMaxRange = 0;
	m_nDataStartIndex = 0;

	// (2006/3/4 - Seung-Won, Bae) Initialize with -1 for Empty Data.
	m_nDataEndIndex = -1;
}

CToolBarManagerImplementation::~CToolBarManagerImplementation()
{
	// (2006/2/20 - Seung-Won, Bae) Manage Module State for Destroy Window
	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	if(m_pToolBarDlg != NULL)
	{
		delete m_pToolBarDlg;
		m_pToolBarDlg = NULL;
	}
	
	if(m_pToolOption != NULL)
	{
		delete m_pToolOption;
		m_pToolOption = NULL;
	}
}

/*-----------------------------------------------------------------------------------------
 - Function    :  CreateToolBar
 - Created at  :  2004-10-28   17:15
 - Author      :  최종찬
 - Parameters  :  strToolBarOption - 툴바에 등록된 버튼들 정보문자열
 -                strImageFile     - 버튼이미지파일명
 - Return Value:  bool - true:성공, false: 실패
 - Description :  툴바를 생성한다. 최우선적으로 툴바Dialog를 생성시킨후 각종 버튼들을 생성시킨다.
 -----------------------------------------------------------------------------------------*/
bool CToolBarManagerImplementation::CreateToolBar(const CString& strToolBarOption, 
												  const CString& strImageFile)
{
	// (2006/2/17 - Seung-Won, Bae) Set Module State for Dialog Creation
	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	if(m_pToolOption == NULL)
		m_pToolOption = new CToolOption(strToolBarOption);
	else
		m_pToolOption->Reset(strToolBarOption);

	int nScrollBarWidth = 0;	//스크롤바의 싸이즈를 계산한다.
	bool bRadarChart = false;	// 레이다차트(펼침형) 사용여부
	bool bStatusBar = false;	// 수치조회데이타보기(StatusBar)사용여부.
	
	m_rectToolBarDlg = CalculateToolBarRect(nScrollBarWidth, bRadarChart, bStatusBar);

	CreateToolBarDialog();	// 툴바Dialog 생성
	CreateToolBarControl(strImageFile, nScrollBarWidth, bRadarChart, bStatusBar);	// 툴바Item생성
	return true;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  CreateToolBarDialog
 - Created at  :  2004-10-21   10:16
 - Author      :  최종찬
 - Parameters  :  rectToolBarDlg - 계산된 툴바Dialog의 Rect
 - Return Value:  bool - true성공, false실패
 - Description :  툴바 Dialog를 생성한다.
 -----------------------------------------------------------------------------------------*/
bool CToolBarManagerImplementation::CreateToolBarDialog()
{
	ML_SET_DEFAULT_RES();

	// (2006/2/8 - Seung-Won, Bae) Do not use GetChartWindowPointer()'s result. That does not exist in CWnd Object Map of this Module.
	if(m_pToolBarDlg == NULL) m_pToolBarDlg = new CToolBarDlg( CWnd::FromHandle( m_pIChartCtrl ? m_pIChartCtrl->GetSafeHwnd() : NULL), this, m_pIChartCtrl);
	else m_pToolBarDlg->DestroyWindow();

	m_pToolBarDlg->SetControlData(m_pPacketList, m_pPacketListManager, m_pxScaleManager);

	// (2006/2/8 - Seung-Won, Bae) Do not use GetChartWindowPointer()'s result. That does not exist in CWnd Object Map of this Module.
	if(!m_pToolBarDlg->Create(IDD_TOOLBAR, CWnd::FromHandle( m_pIChartCtrl->GetChartWindowPointer()->GetSafeHwnd())))
		return false;

	// 최초 생성시에만 MoveWindow를 호출한다. --> 수치조회창 생성전에 Rect를 계산하므로 가능!
	m_pToolBarDlg->MoveWindow(m_rectToolBarDlg);
	m_pToolBarDlg->ShowWindow(SW_SHOW);
	return true;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  CreateToolBarControl
 - Created at  :  2004-10-25   14:22
 - Author      :  최종찬
 - Parameters  :  strImageFile - 버튼이미지 파일
				  nScrollBarWidth - 스크롤바의 너비
				  bRadarChart - 레이다차트 사용여부
				  bStatusBar - 수치조회데이타보기 사용여부
 - Return Value:  bool - true 성공,  false 실패.
 - Description :  툴바Dialog에 등록된 툴버튼을 생성하여 올린다.
 -----------------------------------------------------------------------------------------*/
bool CToolBarManagerImplementation::CreateToolBarControl(const CString& strImageFile, const int nScrollBarWidth, const bool bRadarChart, const bool bStatusBar)
{
	if(m_pToolBarDlg == NULL)
		return false;

	return m_pToolBarDlg->CreateCtrl(m_pToolOption, m_pIChartCtrl->GetRscFont(), strImageFile, nScrollBarWidth, bRadarChart, bStatusBar);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  ToolBarPosChanged
 - Created at  :  2004-10-20   15:50
 - Author      :  최종찬
 - Parameters  :  rectToolBarPos - 새로운 툴바의 영역
 - Return Value:  None
 - Description :  툴바의 위치를 지정한다.
 -----------------------------------------------------------------------------------------*/
void CToolBarManagerImplementation::ToolBarPosChanged(const CRect rectToolBarPos)
{
	if(m_pToolBarDlg == NULL)
		return;

	// 1. 툴바 이동. (툴바에 종속된 컨트롤 모두 이동된다.)
	m_rectToolBarDlg = rectToolBarPos;
	if(m_pToolBarDlg) m_pToolBarDlg->MoveToolBarDlg();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetToolBarRect
 - Created at  :  2004-10-21   09:25
 - Author      :  최종찬
 - Description :  ToolBar의 크기를 리턴한다. 툴바크기를 뺀 차트OCX의 크기를 다시 계산하기 위함.
 -----------------------------------------------------------------------------------------*/
const CRect CToolBarManagerImplementation::GetToolBarRect()
{
	return m_rectToolBarDlg;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  CalculateToolBarRect
 - Created at  :  2004-10-25   09:20
 - Author      :  최종찬
 - Parameters  :  [out]nScrollBarWidth - 스크롤바의 너비.
 -				  [out]bRadarChart - 레이다차트(펼침형) 사용여부
 -				  [out]bStatusBar - 수치조회데이타보기(Status Bar) 사용여부
 - Return Value:  CRect - 다이얼로그의 Rect를 리턴한다.
 - Description :  가로사이즈는 (OCX에 의해) 정해져있으나 세로사이즈는 여기서 계산을 함.
				  스크롤바의 크기도 여기서 계산이 됨.
				  (NOTE: 버튼들이 툴바의 가로싸이즈보다 많을경우는 어찌하는가? - 생각해볼것)
 -----------------------------------------------------------------------------------------*/
const CRect CToolBarManagerImplementation::CalculateToolBarRect(int& nScrollBarWidth, bool& bRadarChart, bool& bStatusBar)
{
	if(m_pToolOption == NULL)
		return CRect(0, 0, 0, 0);

	CTypedPtrList<CObList, CObjTool*>* pToolList = m_pToolOption->GetToolOptionList(CToolOptionInfo::INNER);
	if(pToolList == NULL || pToolList->GetCount() <= 0)
		return CRect(0, 0, 0, 0);
	
	
	// 1. ScrollBar넓이 계산: 일반버튼들 갯수(버튼너비)와 Slider Bar(버튼너비X3), 색설정(버튼너비X3), 
	//					그래프스타일설정(버튼너비X3)을 총합을 뺀 나머지를 ScrollBar넓이로 계산해둔다.
	// 2. 레이다차트영역계산: 레이다 차트의 첫번째 옵션값이 '펼침형'이면 m_nHeight값만큼을 추가한다.
	//						(툴바 아랫쪽에 영역 생성.)
	// 3. 수치조회데이타보기영역계산: (버튼이 생성되지는 않는다) 첫번째 옵션이 'Status Bar'이면 
	//						Height값을 높이에 추가한다.(툴바 윗쪽에 생성)
	

	int nTmpWidth = 0, nTmpHeight = 0;
	int nCount = pToolList->GetCount();
	// NOTE: 종찬(04/10/25) 툴바의 최초 높이값 설정. 좀더 살펴볼것.
	if(nCount >= 1)
		nTmpHeight = TB_BUTTON_HEIGHT;

	POSITION pos = pToolList->GetHeadPosition();
	for(int i = 0; i < nCount; i++)
	{
		CObjTool* pObjTool = pToolList->GetNext(pos);
		if(pObjTool == NULL)
			continue;

		CString strOption1, strOption2;	// 툴버튼의 첫째/두번째 옵션
		switch(pObjTool->GetType()) 
		{
		case CToolOptionInfo::T_INFLATE:			// 확대/축소 (버튼 두개)
			nTmpWidth += (TB_BUTTON_WIDTH * 2);
			break;
		case CToolOptionInfo::T_GRAPH_STYLE:		// 그래프스타일 (버튼너비X3)
		case CToolOptionInfo::T_GRAPH_THICKNESS:	// 선 굵기 (버튼너비X3)
		case CToolOptionInfo::T_INDICATOR_SERIES:	// 지표연속조회 (버튼너비X3)
		case CToolOptionInfo::T_AUTOSCROLL:			// 자동스크롤 (버튼 세개 == 버튼너비X3)
			nTmpWidth += (TB_BUTTON_WIDTH * 3);
			break;
		case CToolOptionInfo::T_DISPLAY_SLIDERBAR:	// 슬라이더바 (너비 == 79 Fixed)
			nTmpWidth += TB_BUTTON_SLIDERBAR;
			break;
		case CToolOptionInfo::T_RADAR:				// 레이더차트 (버튼형:버튼한개, 펼침형:Height추가)
			pObjTool->GetOption(strOption1, strOption2);

			if(strOption1 == _MTEXT( C4_BUTTON_TYPE))
			{
				bRadarChart = false;
				nTmpWidth += TB_BUTTON_WIDTH;
			}
			else	// 펼침형
			{
				bRadarChart = true;
				nTmpHeight += TB_RADAR_HEIGHT;
			}
			break;
		// (2005/9/28 - Seung-Won, Bae) Status Bar is not created in Dialog. It is created in the other Dialog.
		//								So. Do not increase Dialog's Height and Width.
		case CToolOptionInfo::T_INQUIRY_STATUS:
		case CToolOptionInfo::T_SCROLL:				// 스크롤바는 일단 여기서 계산하지 않는다.
			break;
		default:									// 그외의 것들은 버튼너비만 추가.
			nTmpWidth += TB_BUTTON_WIDTH;
			break;
		}//end of switch			
	}//end of for(int...
	
	// ScrollBar의 너비를 계산한다.
	CRect rectToolBar;
	if( m_pIChartCtrl) rectToolBar = m_pIChartCtrl->GetToolBarRegion();
	nScrollBarWidth = rectToolBar.Width() - nTmpWidth;

	return CRect(rectToolBar.left, rectToolBar.bottom - nTmpHeight, rectToolBar.right, rectToolBar.bottom);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetSliderMin
 - Created at  :  2004-10-29   15:26
 - Author      :  최종찬
 - Description :  Slider의 최소값을 세팅한다.
 -----------------------------------------------------------------------------------------*/
void CToolBarManagerImplementation::SetSliderMin(const int nSliderMin)
{
	if(m_pToolBarDlg == NULL)
		return;

	m_pToolBarDlg->SetSliderMin(nSliderMin);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetGraphType
 - Created at  :  2004-11-01   16:37
 - Author      :  최종찬
 - Description :  그래프타입에 따라 그래프스타일을 콤보에 세팅한다.
 -----------------------------------------------------------------------------------------*/
void CToolBarManagerImplementation::SetGraphType(CGraphBaseData::GRAPHTYPE graphType, int nIndex)
{
	if(m_pToolBarDlg == NULL || m_pToolOption == NULL)
		return;
	
	if(!m_pToolOption->GetTool(CToolOptionInfo::T_GRAPH_STYLE))
		return;
	
	m_pToolBarDlg->SetGraphType(graphType, nIndex);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetLineThickness
 - Created at  :  2004-11-03   11:26
 - Author      :  최종찬
 - Description :  OCX에서 선택한 라인두께설정값에 따라 콤보에 해당 스타일 설정값으로 바꾼다.
 -----------------------------------------------------------------------------------------*/
void CToolBarManagerImplementation::SetLineThickness(int nThickness)
{
	if(m_pToolBarDlg == NULL || m_pToolOption == NULL)
		return;
	
	if(!m_pToolOption->GetTool(CToolOptionInfo::T_GRAPH_THICKNESS))
		return;
	
	m_pToolBarDlg->SetLineThickness(nThickness);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetGraphColor
 - Created at  :  2004-11-29   17:04
 - Author      :  최종찬
 - Description :  OCX에서 선택한 그래프의 색상을 '색설정'버튼에 넘겨준다.
 -----------------------------------------------------------------------------------------*/
void CToolBarManagerImplementation::SetGraphColor(COLORREF clrGraphColor)
{
	if(m_pToolBarDlg == NULL || m_pToolOption == NULL)
		return;
	
	// 색설정 버튼이 없으면 칼라값을 보내줄 필요 없다.
	if(!m_pToolOption->GetTool(CToolOptionInfo::T_GRAPH_COLOR))
		return;

	m_pToolBarDlg->SetGraphColor(clrGraphColor);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetOneChart
 - Created at  :  2004-11-05   11:08
 - Author      :  최종찬
 - Description :  단일지표차트인지를 툴바에 세팅한다.
 -----------------------------------------------------------------------------------------*/
void CToolBarManagerImplementation::SetOneChart( bool bOneChart, bool p_bIsNoZoomChart)
{
	if(m_pToolBarDlg)
		m_pToolBarDlg->SetOneChart(bOneChart, p_bIsNoZoomChart);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetControlData
 - Created at  :  2004-11-01   09:51
 - Author      :  최종찬
 - Description :  데이타클래스를 세팅한다. 
 -----------------------------------------------------------------------------------------*/
void CToolBarManagerImplementation::SetControlData(CPacketList* pPacketList, CPacketListManager* pPacketListManager, CXScaleManager* pxScaleManager, CMainBlock* pMainBlock, CIndicatorList* pIndicatorList)
{
	m_pPacketList = pPacketList;
	m_pPacketListManager = pPacketListManager;
	m_pxScaleManager = pxScaleManager;
	m_pMainBlock = pMainBlock;
	m_pIndicatorList = pIndicatorList;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetStartEndIndex
 - Created at  :  2004-11-01   10:54
 - Author      :  최종찬
 - Description :  시작인덱스와 끝인덱스를 가져온다.
 - UpDate	   :  스크롤바가 없을 경우에 대비하여 본클래스의 멤버변수에서 리턴한다.
 -----------------------------------------------------------------------------------------*/
bool CToolBarManagerImplementation::GetStartEndIndex(int& nStartIndex, int& nEndIndex)
{
	nStartIndex = m_nDataStartIndex;
	nEndIndex = m_nDataEndIndex;
	return true;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetMaxRange
 - Created at  :  2004-11-17   14:49
 - Author      :  최종찬
 - Description :  MaxRange값을 리턴한다.
 -----------------------------------------------------------------------------------------*/
int CToolBarManagerImplementation::GetMaxRange() const
{
	if(m_pToolBarDlg && m_pToolBarDlg->IsScrollBar())
		return m_pToolBarDlg->GetMaxRange();
	else
		return m_nMaxRange;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  IsToolScrollBar
 - Created at  :  2005-02-28   10:31
 - Author      :  최종찬
 - Description :  스크롤바 사용유무를 리턴한다.
 -----------------------------------------------------------------------------------------*/
bool CToolBarManagerImplementation::IsToolScrollBar()
{
	if(m_pToolBarDlg)
		return m_pToolBarDlg->IsScrollBar();
	return false;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetToolBarHwnd
 - Created at  :  2005-09-10   
 - Author      :  최종찬
 - Description :  툴바Dialog의 윈도우 핸들을 리턴한다.
 -----------------------------------------------------------------------------------------*/
HWND CToolBarManagerImplementation::GetToolBarHwnd()
{
	if(m_pToolBarDlg)
		return m_pToolBarDlg->GetSafeHwnd();
	else
		return NULL;
}

/*-------------------------------------------------------------------------------
 - Function    :  SetStartEndIndexToOcx
 - Created at  :  2005-04-19   08:50
 - Author      :  최종찬
 - Description :  Start와 End Index를 OCX에 세팅한다.
 -------------------------------------------------------------------------------*/
// (2007/5/23 - Seung-Won, Bae) Case Analysis.
//	It is called from ...
/*
	S (Scroll to)
		CToolBarDlg::OnHScroll() ~ ScrollBar

		CToolOptionInfo::T_INFLATE (Zoom In/Out)				~ CToolBarDlg::SetToolFunction()

		CToolBarDlg::OnHScroll() ~ Zoom SlideBar (Zoom In/Out)

		CRadar::OnLButtonUp()

//			CSubGraph::SendMessage_SubGraph()					: from ChartObject DLL, In GlanceBalance Chart's PreSpan SubGraph's Adding or Removing.
//			~ChartCtrl::OnGraphAdded()							: **************************************************** GlanceBlance (After Calculation)
//			~ChartCtrl::OnGraphDeleted()						: **************************************************** GlanceBlance (After Calculation)
//			~ChartCtrl::OnGraphDeleted(RQ)						: **************************************************** GlanceBlance (After Calculation)
//			CGraphImp::~CGraphImp()								: On Removed Some RQ.
*/
void CToolBarManagerImplementation::SetStartEndIndexToOcx(const int nStart, const int nEnd)
{
	m_nDataStartIndex = nStart;
	m_nDataEndIndex = nEnd;

	if(m_pIChartCtrl)
		m_pIChartCtrl->SetStartEndIndex(nStart, nEnd);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetPageCountNextStepToOcx
 - Created at  :  
 - Author      :  yiilyoul@magicn.com
 - Description :
 -----------------------------------------------------------------------------------------*/
int CToolBarManagerImplementation::GetPageCountNextStepToOcx(const int nSliderPos_Prev, const int nSliderPos_New, const UINT nSBCode, int nMin, int nMax)
{
	if(m_pIChartCtrl)
	{
		return m_pIChartCtrl->GetPageCountNextStep(nSliderPos_Prev, nSliderPos_New, nSBCode, nMin, nMax);
	} else return nSliderPos_New;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetReverse
 - Created at  :  
 - Author      :  yiilyoul@magicn.com
 - Description :
 -----------------------------------------------------------------------------------------*/
BOOL CToolBarManagerImplementation::GetReverseToOcx()
{
	if(m_pxScaleManager)
	{
		return m_pxScaleManager->GetReverse();
	}

	return FALSE;
}

/*-------------------------------------------------------------------------------
 - Function    :  ReleaseOtherButtonUI
 - Created at  :  2005-03-02   14:46
 - Author      :  최종찬   
 - Description :  툴버튼이 눌러져 있다면  해제시킨다.
 -------------------------------------------------------------------------------*/
void CToolBarManagerImplementation::ReleaseOtherButtonUI(CToolOptionInfo::TOOLOPTION enumToolOption)
{
	if(m_pToolBarDlg)
		m_pToolBarDlg->ReleaseOtherButtonUI(enumToolOption);
}



/*-------------------------------------------------------------------------------
 - Function    :  UseToolOption
 - Created at  :  2005-03-03   09:23
 - Author      :  최종찬   
 - Description :  툴바에 해당 툴아이템을 사용하는지 확인한다.(함수명만으로 설명추가함)
 -------------------------------------------------------------------------------*/
BOOL CToolBarManagerImplementation::UseToolOption(CToolOptionInfo::TOOLOPTION enumToolOption)
{
	if(!m_pToolOption)
		return FALSE;
	return m_pToolOption->UseTool(CToolOptionInfo::INNER, enumToolOption);
}


/*-------------------------------------------------------------------------------
 - Function    :  GetToolOption
 - Created at  :  2005-03-03   09:46
 - Author      :  최종찬   
 - Description :  툴바에 해당 툴아이템이 있는지 확인한다.(함수명만으로 설명추가함)
 -------------------------------------------------------------------------------*/
BOOL CToolBarManagerImplementation::GetToolOption(CToolOptionInfo::TOOLOPTION enumToolOption)
{
	if(!m_pToolOption)
		return FALSE;

	CObjTool* pObjTool = m_pToolOption->GetTool(CToolOptionInfo::INNER, enumToolOption);
	if(pObjTool)
		return TRUE;
	else
		return FALSE;
}


//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/3/4
// Modifier		: 
// Comments		: Notify Cursor Mode Change for Drag Cross Line & Numercial Inquiry DLG
//-----------------------------------------------------------------------------
void CToolBarManagerImplementation::OnNumericalInquiryOptionChanged( const long p_lNumericalInquiryOption)
{
	// (2006/3/4 - Seung-Won, Bae) Push or Pop the Button of Numerical Inquiry Dialog or Drag Cross Line
	if(m_pToolBarDlg)
	{
		CButtonEx* pButton = m_pToolBarDlg->GetButtonEx( CToolOptionInfo::T_NIDLG_DRAG);
		if( pButton) pButton->SetState( ( p_lNumericalInquiryOption & NUMERICALINQUIRYDLG) ? true : false);

		pButton = m_pToolBarDlg->GetButtonEx( CToolOptionInfo::T_CROSSHAIRS_DRAG);
		if( pButton) pButton->SetState( ( p_lNumericalInquiryOption & CROSSLINEVIEW) ? true : false);
	}
}


/*-------------------------------------------------------------------------------
 - Function    :  GetRealPacketNames
 - Created at  :  2005-03-24   14:08
 - Author      :  최종찬
 - Parameters  :  strTitle       - "가격"이고..
 -                strPacketNames - "시가,고가,저가,종가"인 경우...
 - Return Value:  CString - 새롭게 바꾼 패킷명.
 - Description :  1틱일경우는 검사하여 새로운 패킷명을 리턴해준다.
 -------------------------------------------------------------------------------*/
CString CToolBarManagerImplementation::GetRealPacketNames(const CString& strTitle, const CString& strPacketNames) const
{
	CString strRealPacketNames = strPacketNames;
	if(strRealPacketNames.Right(1) != ",")
		strRealPacketNames += ",";

	// title = "가격", packetName = "시가,고가,저가,종가", "1틱"일 경우 
	// -> packetName : "종가"
	if(strTitle == _MTEXT( C3_PRICE) && strPacketNames.Find( _MTEXT( C0_CLOSE)) > 0)
	{
		if(m_pPacketList->GetDateUnitType() == CScaleBaseData::HORZ_TICK && m_pPacketList->GetTimeInterval() == 1)
		{
			strRealPacketNames = _MTEXT( C0_CLOSE);
			strRealPacketNames += ",";
		}
	}

	return strRealPacketNames;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  FilterSimpleInquiryData
 - Created at  :  2005-04-06   18:47
 - Author      :  최종찬
 - Parameters  :  strPacketNames - 패킷명.
 - Return Value:  bool - true: 조회 데이타에 추가, false: 조회 데이타에서 제외
 - Description :  Single Line이나 Simple Window형태의 수치조회창에서 필요로하는 데이타만을 추출해내기
				  위한 bool변수를 리턴해준다.
 -----------------------------------------------------------------------------------------*/
bool CToolBarManagerImplementation::FilterSimpleInquiryData(const CString& strPacketNames)
{
	// Simple형태의 조회데이타에 꼭 들어가야하는 패킷들...
	if(strPacketNames.Find( _MTEXT( C0_OPEN)) != -1)
		return true;
	if(strPacketNames.Find( _MTEXT( C0_HIGH)) != -1)
		return true;
	if(strPacketNames.Find( _MTEXT( C0_LOW)) != -1)
		return true;
	if(strPacketNames.Find( _MTEXT( C0_CLOSE)) != -1)
		return true;
	if(strPacketNames.Find( _MTEXT( C0_VOLUME)) != -1)
		return true;
	if(strPacketNames.Find( _MTEXT( C3_PRICE_MA_1)) != -1)
		return true;
	if(strPacketNames.Find( _MTEXT( C3_PRICE_MA_2)) != -1)
		return true;
	if(strPacketNames.Find( _MTEXT( C3_PRICE_MA_3)) != -1)
		return true;
	if(strPacketNames.Find( _MTEXT( C3_PRICE_MA_4)) != -1)
		return true;
	if(strPacketNames.Find( _MTEXT( C3_PRICE_MA_5)) != -1)
		return true;
	if(strPacketNames.Find( _MTEXT( C3_PRICE_MA_6)) != -1)
		return true;

	return false;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  IsSpecialIndicator
 - Created at  :  2005-05-27   16:49
 - Author      :  최종찬
 - Return Value:  bool - TRUE: 특수지표, FALSE: 일반지표
 - Description :  특수지표일경우 항상 전체보기상태이어야한다. 그런데 '확대'버튼을 누를때
				  전체보기상태가 해제되는데 특수지표를 보고 있을 경우는 전체보기상태를 해제
				  시키지 않도록하기 위해 지금 보고 있는 지표가 특수지표인지 ToolBarDlg에게 알려줘야한다.
 -----------------------------------------------------------------------------------------*/
bool CToolBarManagerImplementation::IsSpecialIndicator()
{
	return m_pIChartCtrl->IsRunningOneChart() ? true : false;
}

// (2009/11/6 - Seung-Won, Bae) Do not disable zoom and scroll on OneChart
bool CToolBarManagerImplementation::IsRunningNoZoomChart()
{
	return m_pIChartCtrl->IsRunningNoZoomChart() ? true : false;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  ReDraw
 - Created at  :  2005-08-16   19:01
 - Author      :  최종찬
 - Description :  툴바및 수치조회창을 그린다. OCX의 OnDraw에서 강제적으로 호출.
 -----------------------------------------------------------------------------------------*/
void CToolBarManagerImplementation::ReDraw()
{
	if(m_pToolBarDlg)
		m_pToolBarDlg->ReDraw();
}

// (2006/11/15 - Seung-Won, Bae) Tool On/Off State Changed.
BOOL CToolBarManagerImplementation::OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn)
{
	if( !m_pToolBarDlg) return FALSE;

	CButtonEx* pButton = m_pToolBarDlg->GetButtonEx( p_eToolID);
	if( pButton && pButton->GetSafeHwnd()) pButton->SetState( p_bTurnOn ? true : false);
	return TRUE;
}

// (2009/1/5 - Seung-Won, Bae) Update ScrollBar and Zoom SliderBar from XScaleManager by Real.
void CToolBarManagerImplementation::UpdateZoomAndScrollInfo( int p_nFullRange, int p_nStartPos, int p_nViewCount, BOOL p_bWholeView)
{
	if( !m_pToolBarDlg) return;
	m_nDataStartIndex = p_nStartPos;
	m_nDataEndIndex = p_nStartPos + p_nViewCount - 1;
	m_pToolBarDlg->UpdateZoomAndScrollInfo( p_nFullRange, p_nStartPos, p_nViewCount, p_bWholeView);
}
