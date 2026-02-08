// ButtonProperty.cpp: implementation of the CButtonProperty class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ButtonProperty.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "ButtonEx.h"
#include "Definition.h"
#include "ToolBarDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CButtonProperty::CButtonProperty(CWnd* pParent, CButtonList* pButtonList)
{
	m_pButtonList = pButtonList;
	m_pParent = (CToolBarDlg*) pParent;
}

CButtonProperty::~CButtonProperty()
{
}


/*-----------------------------------------------------------------------------------------
 - Function    :  ReleaseOtherButtonUI
 - Created at  :  2004-11-19   15:50
 - Author      :  최종찬
 - Parameters  :  enumToolOption - 새롭게 눌려진 툴옵션.(이 툴옵션을 제외한 나머지것들에서 Release를 시켜야한다.)
 - Return Value:  int - 1: 성공, else: 실패
 - Description :  새로 눌려진 버튼 이외의 버튼을 속성(?)에 맞게 Release시킨다. (이 클래스의 시작점)
 -----------------------------------------------------------------------------------------*/
int CButtonProperty::ReleaseOtherButtonUI(CToolOptionInfo::TOOLOPTION enumToolOption, const CString& strParam /*= _T("")*/)
{
	switch(enumToolOption) 
	{
		// (2006/3/4 - Seung-Won, Bae) Do not release Numerical Inquiry DLG and Cross Line.
		//		Cross Line of Inner Toolbar control NumericalInquiryOption Value.
		case CToolOptionInfo::T_NIDLG_DRAG:
		case CToolOptionInfo::T_CROSSHAIRS_DRAG:
		// (2006/6/29 - Seung-Won, Bae) Over Tools
		case CToolOptionInfo::T_NIDLG_OVER:
		case CToolOptionInfo::T_CROSSHAIRS_OVER:
													break;

		case CToolOptionInfo::T_INFLATE:				// 확대/축소
			// 모달리스Dlg, 레이더, 전체최대최소보기, 외부툴바 제외시킴
			// (2006/6/23 - Seung-Won, Bae) Except the T_CROSSHAIRS_DRAG, T_NIDLG_DRAG
			ReleaseCaseZoom(enumToolOption, strParam);
			break;
		case CToolOptionInfo::T_CHART_PANDF:			// P&F
		case CToolOptionInfo::T_CHART_REVERSE:			// 역시계곡선
		case CToolOptionInfo::T_CHART_THREELINEBREAK:	// 삼선전환도
		case CToolOptionInfo::T_CHART_BARFORSALE:		// 대기매물
			// 외부툴바 제외시킴
			ReleaseCaseSpecialChart(enumToolOption);
			break;
		default:
			// 전체보기, 레이더차트, 전체최대최소보기, 외부툴바 제외시킴.
			ReleaseCaseDefault(enumToolOption);
			break;
	}

	return 1;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  ReleaseCaseDefault
 - Created at  :  2004-11-22   16:21
 - Author      :  최종찬
 - Description :  기본적인 버튼해제 방식. 전체보기, 레이더차트, 전체최대최소보기, 외부툴바를 제외시킴.
 - UpDaate	   :  (04/11/30) 일목균형표도 추가적으로 제외
 -----------------------------------------------------------------------------------------*/
void CButtonProperty::ReleaseCaseDefault(CToolOptionInfo::TOOLOPTION enumToolOption)
{
	CToolOptionInfo::TOOLOPTION enumButtonToolOption = (CToolOptionInfo::TOOLOPTION)-1;

	if(m_pButtonList == NULL)
		return;
	
	POSITION pos = m_pButtonList->GetHeadPosition();
	while(pos)
	{
		CButtonEx* pButton = m_pButtonList->GetNext(pos);
		if( pButton == NULL) continue;
		// 버튼이 눌려져있고
		if( !pButton->GetState()) continue;

		enumButtonToolOption = pButton->GetToolOptionInfo();
		// 자기자신은 제외하고
		if( enumButtonToolOption == enumToolOption) continue;

		switch( enumButtonToolOption)
		{
			case CToolOptionInfo::T_SHOW_OUTERTOOLBAR:	// 외부툴바 제외
			case CToolOptionInfo::T_WHOLEVIEW:			// 전체보기 제외
			case CToolOptionInfo::T_RADAR:				// 레이더차트 제외
			case CToolOptionInfo::T_CHART_BALANCE:		// 일목균형표 제외
			case CToolOptionInfo::T_MINMAX_FULLDATA:	// 전체최대최소보기 제외한 툴옵션 해제.
			// (2006/3/4 - Seung-Won, Bae) Do not release Numerical Inquiry DLG and Cross Line.
			//		Cross Line of Inner Toolbar control NumericalInquiryOption Value.
			case CToolOptionInfo::T_CROSSHAIRS_DRAG:
			case CToolOptionInfo::T_NIDLG_DRAG:
			// (2006/6/29 - Seung-Won, Bae) Over Tools
			case CToolOptionInfo::T_NIDLG_OVER:
			case CToolOptionInfo::T_CROSSHAIRS_OVER:
														break;
			default:	ReleaseToolOption( enumButtonToolOption);
						pButton->SetState(false);				// 버튼상태 다시 Invalidate를 해주고...
						pButton->Invalidate();
						break;
		}
	}//end of while(
}


/*-----------------------------------------------------------------------------------------
 - Function    :  ReleaseCaseSpecialChart
 - Created at  :  2004-11-22   16:15
 - Author      :  최종찬
 - Description :  특수차트버튼을 클릭했을 경우. 외부툴바를 제외한 모든 툴옵션를 해제한다.
 -----------------------------------------------------------------------------------------*/
void CButtonProperty::ReleaseCaseSpecialChart(CToolOptionInfo::TOOLOPTION enumToolOption)
{
	CButtonEx* pButton = NULL;
	CToolOptionInfo::TOOLOPTION enumButtonToolOption = (CToolOptionInfo::TOOLOPTION)-1;

	if(m_pButtonList == NULL)
		return;

	POSITION pos = m_pButtonList->GetHeadPosition();
	while(pos)
	{
		pButton = m_pButtonList->GetNext(pos);

		enumButtonToolOption = pButton->GetToolOptionInfo();

		if(pButton->GetState() 
			&& enumButtonToolOption != enumToolOption 
			&& enumButtonToolOption != CToolOptionInfo::T_SHOW_OUTERTOOLBAR)
		{
			ReleaseToolOption(enumButtonToolOption);
			pButton->SetState(false);				// 버튼상태 다시 Invalidate를 해주고...
			pButton->Invalidate();
		}//end of if(..
	}//end of while(
}


/*-----------------------------------------------------------------------------------------
 - Function    :  ReleaseCaseZoom
 - Created at  :  2004-11-22   15:24
 - Author      :  최종찬
 - Description :  확대/축소버튼이 눌렸을때 버튼풀기. 
				  확대- 기호, 모의매매, 주가예측, 공식추가, 패턴분석, 돋보기, 레이더차트
						전체최대최소보기, 외부툴바를 제외한 모든 툴옵션풀기.
				  축소- 위와 상동 + 전체보기까지 제외한 모든 툴옵션 풀기.
 - UpDaate	   :  (04/11/30) 일목균형표도 추가적으로 제외
-----------------------------------------------------------------------------------------*/
void CButtonProperty::ReleaseCaseZoom(CToolOptionInfo::TOOLOPTION enumToolOption, const CString& strParam)
{
	CButtonEx* pButton = NULL;
	CToolOptionInfo::TOOLOPTION enumButtonToolOption = (CToolOptionInfo::TOOLOPTION)-1;

	if(m_pButtonList == NULL)
		return;

	POSITION pos = m_pButtonList->GetHeadPosition();
	while(pos)
	{
		pButton = m_pButtonList->GetNext(pos);
		
		enumButtonToolOption = pButton->GetToolOptionInfo();

		if( pButton->GetState()							// 눌려져있는 버튼에서	
			&& !IsModalessDialog( enumButtonToolOption) // 모달리스Dlg 제외하고
			&& enumButtonToolOption != enumToolOption)	// 자기자신 제외,
			switch( enumButtonToolOption)
		{
			case CToolOptionInfo::T_RADAR:						// 레이더차트 제외,
			case CToolOptionInfo::T_MINMAX_FULLDATA:			// 전체최대최소보기 제외,
			case CToolOptionInfo::T_SHOW_OUTERTOOLBAR:			// 외부툴바 제외,
			case CToolOptionInfo::T_CHART_BALANCE:				// 일목균형표 제외
			// (2006/6/23 - Seung-Won, Bae) Except the T_CROSSHAIRS_DRAG, T_NIDLG_DRAG
			case CToolOptionInfo::T_CROSSHAIRS_DRAG:
			case CToolOptionInfo::T_NIDLG_DRAG:
			// (2006/6/29 - Seung-Won, Bae) Over Tools
			case CToolOptionInfo::T_NIDLG_OVER:
			case CToolOptionInfo::T_CROSSHAIRS_OVER:
														break;

			default:	if(strParam == _MTEXT( C4_ZOOM_OUT))	// 축소버튼이면
						{
							if(enumButtonToolOption != CToolOptionInfo::T_WHOLEVIEW)	// 전체보기도 제외한 
								ReleaseToolOption(enumButtonToolOption);				// 놈들의 버튼을 푼다.
						}
						else									// 만약 확대버튼일때는
						{
							if(enumButtonToolOption != CToolOptionInfo::T_WHOLEVIEW)
								ReleaseToolOption(enumButtonToolOption);
							else								// 전체보기의 경우는 상태만 변경시킨다.
								m_pParent->SetStateWholeView(0);
						}
						pButton->SetState(false);				// 버튼상태 다시 Invalidate를 해주고...
						pButton->Invalidate();
		}//end of if(.......) swtich
	}//end of while(
}


/*-----------------------------------------------------------------------------------------
 - Function    :  IsModalessDialog
 - Created at  :  2004-11-22   15:34
 - Author      :  최종찬
 - Description :  기호Dlg, 모의매매, 주가예측, 공식추가, 패턴분석, 돋보기, 레이더차트(여기선 제외),
				  목표치분선 등 모달리스 다이얼로그인지 확인한다.
 -----------------------------------------------------------------------------------------*/
bool CButtonProperty::IsModalessDialog(CToolOptionInfo::TOOLOPTION enumToolOption)
{
	switch(enumToolOption)
	{
	case CToolOptionInfo::T_SYMBOL_BULET:			// 기호Dlg
	case CToolOptionInfo::T_TRADER_REPORT:			// 모의매매
	case CToolOptionInfo::T_PRICE_FORCAST:			// 주가예측
	case CToolOptionInfo::T_MAKE_FORMULA:			// 공식추가
	case CToolOptionInfo::T_PATTERN:				// 봉패턴분석
	case CToolOptionInfo::T_MAGNIFYINGGLASS:		// 돋보기
	case CToolOptionInfo::T_RADAR:					// 레이더차트
	case CToolOptionInfo::T_TARGET_ANALYSIS:		// 목표치분석
		return true;
	default:
		return false;
	}
}



/*-----------------------------------------------------------------------------------------
 - Function    :  ReleaseToolOption
 - Created at  :  2004-11-22   15:45
 - Author      :  최종찬
 - Description :  실행중인 ToolOption을 헤제한다.
 -----------------------------------------------------------------------------------------*/
void CButtonProperty::ReleaseToolOption(CToolOptionInfo::TOOLOPTION enumToolOption)
{
	m_pParent->ReleaseToolFunction(enumToolOption);
}