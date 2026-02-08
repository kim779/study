// KTBChartTool.cpp: implementation of the CKTBChartTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "../Include_Chart/DLL_Load/AddInManager.h"			// for CAddInManager
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../AddInManager/Include_AddIn/AddInCommand.h"		// for EAI_BAS_OCXELEMENTMENU
#include "../Include_Chart/Block.h"							// for CBlock
#include "OutsideCommonInterface.h"							// for _CHARTDLL_CHARTTYPE_PRICE
#include "KTBChart.h"										// for g_strOcxPath
#include "KTBChartCtl.h"									// for CKTBChartCtrl
#include "PacketList.h"										// for CPacketList
#include "MainBlock.h"										// for CMainBlock
#include "ToolBarManager.h"									// for IToolBarManager
#include "CfgFileVersionChecker.h"							// for CCfgFileVersionChecker
#include "MessageDefine.h"									// for UM_ANALYSIS_FUNCTION
#include "XScaleManager.h"

/*-----------------------------------------------------------------------------------------
 - Function    :  SetToolFuncGraphStyle
 - Created at  :  2004-11-03   13:55
 - Author      :  최종찬
 - Description :  그래프형태 설정
 -----------------------------------------------------------------------------------------*/
bool CKTBChartCtrl::RunToolFuncGraphStyle(const int nParam)
{
	long lResult = 0;
	if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( lResult, EAI_BAS_OCXELEMENTMENU, CMenuType::MENU_DRAWSTYLE, nParam);
	return lResult;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  RunToolFuncLineThickness
 - Created at  :  2004-11-03   13:55
 - Author      :  최종찬
 - Description :  선굵기 설정
 -----------------------------------------------------------------------------------------*/
bool CKTBChartCtrl::RunToolFuncLineThickness(const int nParam)
{
	long lResult = 0;
	if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( lResult, EAI_BAS_OCXELEMENTMENU, CMenuType::MENU_THICKNESS, nParam);
	return lResult;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  RunToolFuncGraphColor
 - Created at  :  2004-11-29   11:35
 - Author      :  최종찬
 - Description :  그래프 칼라 설정
 -----------------------------------------------------------------------------------------*/
bool CKTBChartCtrl::RunToolFuncGraphColor(COLORREF clrGraphColor)
{
	long lResult = 0;
	if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( lResult, EAI_BAS_OCXELEMENTMENU, CMenuType::MENU_COLOR, clrGraphColor);
	return lResult;
}

void CKTBChartCtrl::BehaviorBlock(long pBlockBehaviorData)
{
	CBlockBehaviorData* pBehaviorData = (CBlockBehaviorData*)pBlockBehaviorData;
	if( !pBehaviorData) return;
	if( m_pAddInManager)
	{
		CBlockIndex sourceindex = pBehaviorData->GetSourceBlockIndex();
		CBlockIndex targetindex = pBehaviorData->GetTargetBlockIndex();
		m_pAddInManager->OnBlockChanged( pBehaviorData->GetBlockBehavior(), sourceindex.GetRow(), sourceindex.GetColumn(), targetindex.GetRow(), targetindex.GetColumn());
	}
}

void CKTBChartCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// (2004.11.09, 배승원) KeyBoard Push를 AddIn Module에게 전달하여 처리토록 한다.
	if( m_pAddInManager) m_pAddInManager->OnKeyDown( nChar, nRepCnt, nFlags);

	COleControl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CKTBChartCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default

	// (2004.11.09, 배승원) KeyBoard Push를 AddIn Module에게 전달하여 처리토록 한다.
	if( m_pAddInManager) m_pAddInManager->OnKeyUp( nChar, nRepCnt, nFlags);
	
	COleControl::OnKeyUp(nChar, nRepCnt, nFlags);
}

// Analysis Function Message
// 모든 툴에 관한 외부 메세지 (메인에서 발생)
// (2006/1/31 - Seung-Won, Bae) Tool Command(Function) Message (Old Name : OnAnalysisFunction)
//		Bypass to RunToolFunction()
LRESULT CKTBChartCtrl::OnToolFunction( WPARAM wParam, LPARAM lParam)
{
	return RunToolFunction( ( CToolOptionInfo::TOOLOPTION)wParam, (int)lParam);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  RunToolFunction
 - Created at  :  2004-11-10   09:18
 - Author      :  최종찬
 - Parameters  :  nParam - 1: 수행, 0: 해제.
 - Return Value:  bool - true: 성공, false: 실패.
 - Description :  툴기능 수행 및 해제를 한다.
 -----------------------------------------------------------------------------------------*/
// (2006/1/31 - Seung-Won, Bae) Main Routine for Tool Command(Function).
BOOL CKTBChartCtrl::_RunToolFunction( short nToolCommand, short nOption)
{
	return RunToolFunction( ( const enum CToolOptionInfo::TOOLOPTION)nToolCommand, nOption);
}
bool CKTBChartCtrl::RunToolFunction( const CToolOptionInfo::TOOLOPTION p_eToolOption, const int p_nParam)
{
	CToolOptionInfo::TOOLOPTION enumToolOption = p_eToolOption;
	int nParam = p_nParam;

	//------------------------------------------------본격적으로 수행되는 부분
	switch(enumToolOption)
	{
		//sy 2004.05.12.
		// -> 특수차트와 일반지표의 추가 방식은 틀리다. 
		//    그런데 모든 방식이 <특수차트> 형식으로 하는 오류가 있었음.
		case CToolOptionInfo::T_CHART_PANDF:	// P&F
												return AddOrDeleteChart_Special( _MTEXT( C2_P_AND_F));

		case CToolOptionInfo::T_CHART_REVERSE:	// 역시계곡선
												return AddOrDeleteChart_Special( _MTEXT( C2_INVERSE_TIME_LINE));

		case CToolOptionInfo::T_CHART_THREELINEBREAK:	// 삼선전환도
														return AddOrDeleteChart_Special( _MTEXT( C2_THREE_LINE_BREAK));

		case CToolOptionInfo::T_CHART_BALANCE:	// 일목균형표
												return (AddChartBlock( _MTEXT( C2_OBVIOUSLY_BALANCE_CHART)) == TRUE ? true: false);

		case CToolOptionInfo::T_CHART_BARFORSALE:	// 대기매물
													return (AddChartBlock( _MTEXT( C2_OVERHANGING_SUPPLY)) == TRUE ? true: false);

		case CToolOptionInfo::T_CHART_RAINBOW:	// 그물차트
												return (AddChartBlock( _MTEXT( C2_RAINBOW_CHART)) == TRUE ? true: false);
		//sy end

		case CToolOptionInfo::T_RESTORE:	//sy 2004.05.12.
											//sy 2005.10.26.
											//nOption = 2 : 지표 초기화 되는 메시지 생략.
											//OnReturnOrg2(nParam == 2 ? false: true);
											//NeoRestore();
											OnReturnOrg(nParam == 2 ? false: true);
											//sy end
											break;

		case CToolOptionInfo::T_PREV:	if(nParam != 0)
										{
											if(GetXScaleManager()->GetReverse() == FALSE)
											{
												NeoFireDataUpandDown(0);
											}
											else
											{
												NeoFireDataUpandDown(1);
											}
										}
										break;

		case CToolOptionInfo::T_NEXT:	if(nParam != 0)
										{
											if(GetXScaleManager()->GetReverse() == FALSE)
											{
												NeoFireDataUpandDown(1);
											}
											else
											{
												NeoFireDataUpandDown(0);
											}
										}
										break;

		case CToolOptionInfo::T_DAILY:
		case CToolOptionInfo::T_WEEKLY:
		case CToolOptionInfo::T_MONTHLY:	NeoRequestBongData(enumToolOption, nParam);
											break;

		case CToolOptionInfo::T_WHOLEVIEW:	SetWholeView( nParam == 1);
											break;

		case CToolOptionInfo::T_DISPLAY_RESTORE:	// Display수 복원
													OnReturnOrgDisplay();
													break;

		case CToolOptionInfo::T_MINMAX_FULLDATA:	// 전체 최대최소 보기
													OnFullMinMax(nParam);
													break;

		case CToolOptionInfo::T_RADAR:				// 레이더 차트
													m_bRadar = !m_bRadar;
													break;

		case CToolOptionInfo::T_CHART_INVERT:	// 차트뒤집기
		{
												if(!m_pMainBlock) return false;
												int nR = 0, nC = 0, nV = 0, nG = 0;
												CString strGraphName;
												if( !m_pMainBlock->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, strGraphName, "DEFAULT")) return FALSE;
												CBlock *pBlock = m_pMainBlock->GetBlock( CBlockIndex( nR, nC));
												if( pBlock)
												{
													pBlock->SetVScaleInvert( nV, !pBlock->IsVScaleInvert( nV));
													InvalidateControl();
												}
												break;
		}
		// (2006/3/29 - Seung-Won, Bae) Mouse Drag Tools controled by Only m_nNumericalInquiryOption.
		case CToolOptionInfo::T_NIDLG_DRAG:					SetNumericalInquiryOption( p_nParam ? m_nNumericalInquiryOption | NUMERICALINQUIRYDLG					: m_nNumericalInquiryOption & ~NUMERICALINQUIRYDLG);
															return true;
		case CToolOptionInfo::T_CROSSHAIRS_DRAG:			SetNumericalInquiryOption( p_nParam ? m_nNumericalInquiryOption | CROSSLINEVIEW							: m_nNumericalInquiryOption & ~CROSSLINEVIEW);
															return true;

		case CToolOptionInfo::T_PRINT:	
		case CToolOptionInfo::T_PRINT_CHART:	OnPrint();
												return true;
	}
	//------------------------------------------------본격적으로 수행되는 부분 끝.

	// (2006/1/26 - Seung-Won, Bae) Do Analysis Tool Fucntion
	if( m_pAddInManager) m_pAddInManager->OnToolCommand( enumToolOption, nParam);

	return true;
}
