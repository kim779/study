// SetItemOfColorListBox.cpp: implementation of the CSetItemOfColorListBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "SetItemOfColorListBox.h"

#include "../Include_Chart/DLL_Load/IMetaTable.h"		// for _MTEXT()

//dll의 enum값들 모임.
#include "BaseData.h"
#include "FindType.h"

#include "GraphData.h"
#include "ListBoxColorPickerST.h"

#include "Conversion.h"
#include "BlockBaseData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void CSetItemOfColorListBox::SetItem( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	ILPCSTR szIndicatorName = p_pIGraphInfo->GetIndicatorNameString();
	CBaseData::COLORLISTITEMTYPE texttype = CFindType::FindColorItemText( szIndicatorName);
	switch(texttype) {
		case CBaseData::PRICE://가격
			SetItem_PRICE(p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CBaseData::VOLUME://거래량
			SetItem_VOLUME(p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CBaseData::UP_DOWN:// 상승, 하락
			SetItem_UPDOWN( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CBaseData::DAEGIMEMUL:// 대기매물
			SetItem_DAEGIMEMUL( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CBaseData::COLOR:// 색상
			SetItem_COLOR( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CBaseData::MACD: //macd
			SetItem_MACD(p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CBaseData::PIVOT: //pivot
			SetItem_PIVOT(p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CBaseData::SUBGRAPHNAME://SubGraph이름
			SetItem_SUBGRAPHNAME(p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CBaseData::RAINBOW://시작색상, 종료색상
			SetItem_RAINBOW(p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CBaseData::SUB_UPFILL_DOWNFILL://SubGraph이름, 상승채움, 하락채움
			SetItem_SUB_UPFILL_DOWNFILL( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CBaseData::GLANCEBLANCE: //전환, 기준, 후행, 선행, 선행상승채움, 하락채움.
			SetItem_GLANCEBLANCE(p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CBaseData::INVERSELINE: //역시계곡선
			SetItem_INVERSELINE(p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CBaseData::NONE:
			SetItembyStyleandType(true, p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
	}
}

int CSetItemOfColorListBox::GetVolumeType(IGraphInfo* p_pIGraphInfo)
{
	//test -
	IDoubleList *pIConditionList = p_pIGraphInfo->GetDrawConditionList();
	if( !pIConditionList) return -1;
	double dType = -1;
	pIConditionList->GetHead( dType);
	pIConditionList->Release();
	return ( int)dType;
}

bool CSetItemOfColorListBox::IsLineChart( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex)
{
	if( p_nSubGraphIndex < 0) return false;
	if( !p_pIGraphInfo) return false;

	return ( p_pIGraphInfo->GetSubGraphStyle( p_nSubGraphIndex) == 2); //라인식
}

//가격차트
void CSetItemOfColorListBox::SetItem_PRICE(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	//test -
	if(IsLineChart( p_pIGraphInfo, p_nSubGraphIndex))
		SetItem_COLOR( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
	else { 
		SetItem_UPDOWN( p_pIGraphInfo, p_nSubGraphIndex, pListBox);

		IDoubleList *pIConditionList = p_pIGraphInfo->GetCommonCalculateConditionList();
		if( pIConditionList)
		{
			double dType = -1;
			pIConditionList->GetHead( dType);
			pIConditionList->Release();
			if( ( int)dType != 2) SetItem_EQUAL( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
		}
	}
}
//거래량
void CSetItemOfColorListBox::SetItem_VOLUME(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	//test -
	if( p_nSubGraphIndex < 0) return;
	if( p_pIGraphInfo == NULL) return;

// 2008.01.25 by LYH >>
	CString strColor;
	strColor.LoadString(IDS_STRING_COLOR);
// 2008.01.25 by LYH <<
	//일반
	if(GetVolumeType(p_pIGraphInfo) == 1)
		//sy 2004.05.11. -> 일반거래량 색은 1번째로 한다.
// 2008.01.25 by LYH >>
		//pListBox->AddString( _T("색상"), p_pIGraphInfo->GetSubGraphColor( 0, 1));
		pListBox->AddString( strColor, p_pIGraphInfo->GetSubGraphColor( 0, 1));
// 2008.01.25 by LYH <<
	else if(GetVolumeType(p_pIGraphInfo) == 2) //상승, 하락
		SetItem_UPDOWN(	p_pIGraphInfo, p_nSubGraphIndex, pListBox);
	else if(GetVolumeType(p_pIGraphInfo) == 3) { //캔들색과 같이
		SetItem_UPDOWN(	p_pIGraphInfo, p_nSubGraphIndex, pListBox);
		SetItem_EQUAL(	p_pIGraphInfo, p_nSubGraphIndex, pListBox);
	}
}

//상승, 하락 색 설정
void CSetItemOfColorListBox::SetItem_UPDOWN( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
// 2008.01.25 by LYH >>
	//pListBox->AddString(_T("상승"), p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 1));
	//pListBox->AddString(_T("하락"), p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 3));
	CString strUp, strDown;
	strUp.LoadString(IDS_STRING_UP);
	strUp.LoadString(IDS_STRING_DOWN);
	pListBox->AddString(strUp, p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 1));
	pListBox->AddString(strDown, p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 3));
// 2008.01.25 by LYH <<
}

//보합색 설정
void CSetItemOfColorListBox::SetItem_EQUAL( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
// 2008.01.25 by LYH >>
	//pListBox->AddString(_T("보합"), p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 5));
	CString strSteady;
	strSteady.LoadString(IDS_STRING_STEADY);
	pListBox->AddString(strSteady, p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 5));
// 2008.01.25 by LYH <<
}

//대기매물
void CSetItemOfColorListBox::SetItem_DAEGIMEMUL( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	if( p_nSubGraphIndex < 0 || p_pIGraphInfo->GetSubGraphCount() <= p_nSubGraphIndex) return;

	ILPCSTR szGraphName = p_pIGraphInfo->GetSubGraphNameString( p_nSubGraphIndex);
	if( !strcmp( szGraphName, _MTEXT( C3_OVERHANGING_SUPPLY))) 
	{
		//sy 2004.05.13. -> 대기매물의 색에 Text색 변경 가능하게 처리함.
		// 기존 : 기본테두리, 기본바, 최고거래 테두리, 현재가 테두리, 현재가 바
		//2008.01.31 by LYH 사용 안함
		pListBox->AddString("기본매물",		p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 1));
		pListBox->AddString("기본매물Text",	p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 2));
		pListBox->AddString("현재매물",		p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 3));
		pListBox->AddString("현재매물Text", p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 4));
		pListBox->AddString("최대매물",		p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 5));
		pListBox->AddString("최대매물Text", p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 6));
	}
	//sy 2004.05.12. : 기존엔 특수차트이 였으나 <가격차트>에 첨부되는 차트로 수정함.
	/*else if(p_nSubGraphIndex->GetName().CompareNoCase("가격") == 0 ||
		    p_nSubGraphIndex->GetName().CompareNoCase("체결가") == 0)  {
		int pSub = p_pIGraphInfo->GetAt(1);
		if(pSub == NULL)
			return;

		if(IsLineChart(p_nSubGraphIndex))
			pListBox->AddString(_T("색상"), pSub->GetColor().GetColor1());
		else {
			pListBox->AddString(_T("상승"), pSub->GetColor().GetColor1());
			pListBox->AddString(_T("하락"), pSub->GetColor().GetColor3());
		}
	}*/
}
//색상을 String으로 한 색 설정
void CSetItemOfColorListBox::SetItem_COLOR( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
// 2008.01.25 by LYH >>
	CString strColor;
	strColor.LoadString(IDS_STRING_COLOR);
	//pListBox->AddString("색상", p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 1));
	pListBox->AddString(strColor, p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 1));
// 2008.01.25 by LYH <<
}

//지표별로 색 설정
void CSetItemOfColorListBox::SetItem_SUBGRAPHNAME(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	ILPCSTR szGraphName = NULL;
	for(int i = 0 ; i< p_pIGraphInfo->GetSubGraphCount() ; i++)
	{
		szGraphName = p_pIGraphInfo->GetSubGraphNameString( i);
		pListBox->AddString( szGraphName, p_pIGraphInfo->GetSubGraphColor( i, 1));
	}
}

//레인보우
void CSetItemOfColorListBox::SetItem_RAINBOW( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	pListBox->AddString("시작색상", p_pIGraphInfo->GetSubGraphColor( 0, 1));
	pListBox->AddString("종료색상", p_pIGraphInfo->GetSubGraphColor( 0, 2));
}
//지표, 상승채움, 하락채움.
void CSetItemOfColorListBox::SetItem_SUB_UPFILL_DOWNFILL( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	ILPCSTR szGraphName = p_pIGraphInfo->GetSubGraphNameString( p_nSubGraphIndex);
	pListBox->AddString( szGraphName,	p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 5));
	pListBox->AddString( "상승채움",	p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 2));
	pListBox->AddString( "하락채움",	p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 4));
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetItem_SUB_UPFILL_DOWNFILL
 - Created at  :  2004-10-04   14:30
 - Author      :  최종찬
 - Description :  선형;채움식;용 함수 (서브그래프들과 UP/DOWN채움색상까지 지정한다.)
 -----------------------------------------------------------------------------------------*/
void CSetItemOfColorListBox::SetItem_SUB_UPFILL_DOWNFILL2( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
#ifdef _DEBUG
	AfxMessageBox( "What is this?");
#endif
/*
	for(int i = 0; i < p_pIGraphInfo->GetSubGraphCount(); i++)
	{
		// 함수파라미터로 들어온 SubGraphData와 같은 넘이면 서브그래프, 상승채움, 하락채움을
		// ListBox에 넣는다.
		if(pTmp_nSubGraphIndex->GetName() == p_nSubGraphIndex->GetName())
		{	
			pListBox->AddString(p_nSubGraphIndex->GetName(), p_nSubGraphIndex->GetColor().GetColor5());
			pListBox->AddString(_T("상승채움"), p_nSubGraphIndex->GetColor().GetColor2());
			pListBox->AddString(_T("하락채움"), p_nSubGraphIndex->GetColor().GetColor4());
		}
		else
		{
			pListBox->AddString((p_pIGraphInfo->GetSubGraphData(i))->GetName(), (p_pIGraphInfo->GetSubGraphData(i))->GetColor().GetColor1());
		}
	}
*/
}


//전환,기준, 후행, 선행, 상승구름띠, 하락구름띠.
void CSetItemOfColorListBox::SetItem_GLANCEBLANCE(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	ILPCSTR szSubGraphName = NULL;
	for( int i = 0 ; i <  p_pIGraphInfo->GetSubGraphCount() ; i++)
	{
		szSubGraphName = p_pIGraphInfo->GetSubGraphNameString( i);
		if( !strcmp( szSubGraphName, "선행스팬"))
		{
			pListBox->AddString( "상승구름띠", p_pIGraphInfo->GetSubGraphColor( i, 2));
			pListBox->AddString( "하락구름띠", p_pIGraphInfo->GetSubGraphColor( i, 4));
		}
		else pListBox->AddString( szSubGraphName, p_pIGraphInfo->GetSubGraphColor( i, 1));
	}
/*
		if(subName.CompareNoCase("전환선") == 0)
			pListBox->AddString("전환선", pSub->GetColor().GetColor1());
		else if(subName.CompareNoCase("기준선") == 0)
			pListBox->AddString("기준선", pSub->GetColor().GetColor1());
		else if(subName.CompareNoCase("후행스팬") == 0)
			pListBox->AddString("후행스팬", pSub->GetColor().GetColor1());

*/
/*
	int pSub = p_pIGraphInfo->GetAt(0);
	if(pSub != NULL)
		pListBox->AddString("전환선", pSub->GetColor().GetColor1());
	
	pSub = p_pIGraphInfo->GetAt(1);
	if(pSub != NULL) 
		pListBox->AddString("기준선", pSub->GetColor().GetColor1());

	pSub = p_pIGraphInfo->GetAt(2);
	if(pSub != NULL) 
		pListBox->AddString("후행스팬", pSub->GetColor().GetColor1());

	pSub = p_pIGraphInfo->GetAt(3);
	if(pSub != NULL) {
		pListBox->AddString("상승구름띠", pSub->GetColor().GetColor2());
		pListBox->AddString("하락구름띠", pSub->GetColor().GetColor4());
	}
*/
}

//InverseLine
void CSetItemOfColorListBox::SetItem_INVERSELINE(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	pListBox->AddString("역시계1", p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 1));
	pListBox->AddString("역시계2", p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 2));
	pListBox->AddString("역시계3", p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 3));
	pListBox->AddString("역시계4", p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 4));
	pListBox->AddString("역시계5", p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 5));
	pListBox->AddString("역시계6", p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 6));
}

void CSetItemOfColorListBox::OnApply(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex)
{
	//sy 2004.05.12.
	if( p_nSubGraphIndex < 0) return;
	if(pListBox == NULL || p_pIGraphInfo == NULL) return;

	ILPCSTR szIndicatorName = p_pIGraphInfo->GetIndicatorNameString();
	CBaseData::COLORLISTITEMTYPE texttype = CFindType::FindColorItemText( szIndicatorName);
	switch(texttype) {
		case CBaseData::PRICE: //가격
			OnApply_PRICE(pListBox, p_pIGraphInfo, p_nSubGraphIndex);
			break;
		case CBaseData::VOLUME:  //거래량
			OnApply_VOLUME(pListBox, p_pIGraphInfo, p_nSubGraphIndex);
			break;
		case CBaseData::UP_DOWN: // 상승, 하락
			OnApply_UPDOWN( p_pIGraphInfo, pListBox, p_nSubGraphIndex);
			break;
		case CBaseData::SUBGRAPHNAME://SubGraph이름
		case CBaseData::PIVOT: //pivot
			OnApply_SUBGRAPHNAME(pListBox, p_pIGraphInfo, p_nSubGraphIndex);
			break;
		case CBaseData::MACD: //macd
			OnApply_MACD(pListBox, p_pIGraphInfo, p_nSubGraphIndex);
			break;
		case CBaseData::COLOR: // 색상
			OnApply_COLOR( p_pIGraphInfo, pListBox, p_nSubGraphIndex);
			break;
		case CBaseData::DAEGIMEMUL: // 대기매물
			OnApply_DAEGIMEMUL(pListBox, p_pIGraphInfo, p_nSubGraphIndex);
			break;
		case CBaseData::RAINBOW: //시작색상, 종료색상
			OnApply_RAINBOW(pListBox, p_pIGraphInfo, p_nSubGraphIndex);
			break;
		case CBaseData::SUB_UPFILL_DOWNFILL: //SubGraph이름, 상승채움, 하락채움
			OnApply_SUB_UPFILL_DOWNFILL( p_pIGraphInfo, pListBox, p_nSubGraphIndex);
			break;
		case CBaseData::GLANCEBLANCE: //전환, 기준, 후행, 선행, 선행상승채움, 하락채움.
			OnApply_GLANCEBLANCE(pListBox, p_pIGraphInfo, p_nSubGraphIndex);
			break;
		case CBaseData::INVERSELINE: //역시계곡선
			OnApply_INVERSELINE(pListBox, p_pIGraphInfo, p_nSubGraphIndex);
			break;
		case CBaseData::NONE:
			SetItembyStyleandType(false, p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
	}
}

void CSetItemOfColorListBox::OnApply_PRICE(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex)
{
	OnApply_upcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(0));
	if(!IsLineChart( p_pIGraphInfo, p_nSubGraphIndex)) { 
		OnApply_downcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(1));

		IDoubleList *pIConditionList = p_pIGraphInfo->GetDrawConditionList();
		if( pIConditionList)
		{
			double dType = -1;
			pIConditionList->GetHead( dType);
			pIConditionList->Release();
			if( ( int)dType == 1) OnApply_equalcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor( 2));
		}
	}
}

void CSetItemOfColorListBox::OnApply_VOLUME(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex)
{
	//일반거래량
	if(GetVolumeType(p_pIGraphInfo) == 1) { 
		//sy 2004.05.11. -> 일반거래량 색은 1번째로 한다.
		OnApply_upcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(0));
		//p_nSubGraphIndex->GetColor().SetColor5(pListBox->GetColor(0));
		//p_nSubGraphIndex->GetColor().SetColor6(pListBox->GetColor(0));
		//sy end
	}
	else if(GetVolumeType(p_pIGraphInfo) == 2) { //상승, 하락
		OnApply_upcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(0));
		OnApply_downcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(1));
	}
	else if(GetVolumeType(p_pIGraphInfo) == 3) { //캔들색과 같이
		OnApply_upcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(0));
		OnApply_downcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(1));
		OnApply_equalcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(2));
	}
}

void CSetItemOfColorListBox::OnApply_UPDOWN( IGraphInfo* p_pIGraphInfo, CListBoxColorPickerST *pListBox, int p_nSubGraphIndex)
{
	OnApply_upcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(0));
	OnApply_downcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(1));
}

void CSetItemOfColorListBox::OnApply_SUBGRAPHNAME(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex)
{
	for(int i = 0 ; i< p_pIGraphInfo->GetSubGraphCount(); i++) OnApply_upcolor( p_pIGraphInfo, i, pListBox->GetColor(i));
}

void CSetItemOfColorListBox::OnApply_COLOR( IGraphInfo* p_pIGraphInfo, CListBoxColorPickerST *pListBox, int p_nSubGraphIndex)
{
	OnApply_upcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(0));
}

void CSetItemOfColorListBox::OnApply_RAINBOW(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex)
{
	p_pIGraphInfo->SetSubGraphColor( p_nSubGraphIndex, 1, pListBox->GetColor( 0));
	p_pIGraphInfo->SetSubGraphColor( p_nSubGraphIndex, 2, pListBox->GetColor( 1));
}

void CSetItemOfColorListBox::OnApply_SUB_UPFILL_DOWNFILL( IGraphInfo* p_pIGraphInfo, CListBoxColorPickerST *pListBox, int p_nSubGraphIndex)
{
	p_pIGraphInfo->SetSubGraphColor( p_nSubGraphIndex, 5, pListBox->GetColor( 0));
	OnApply_upcolor(	p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor( 1));
	OnApply_downcolor(	p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor( 2));
}

/*-----------------------------------------------------------------------------------------
 - Function    :  OnApply_SUB_UPFILL_DOWNFILL
 - Created at  :  2004-10-04   14:42
 - Author      :  최종찬
 - Description :  선형;채움식; 일때의 지표설정을 적용한다.
 -----------------------------------------------------------------------------------------*/
void CSetItemOfColorListBox::OnApply_SUB_UPFILL_DOWNFILL2( IGraphInfo* p_pIGraphInfo, CListBoxColorPickerST *pListBox, int p_nSubGraphIndex)
{
#ifdef _DEBUG
	AfxMessageBox( "What is it?");
#endif
/*
	for(int i = 0; i < p_pIGraphInfo->GetSubGraphCount(); i++)
	{
		int pTmp_nSubGraphIndex = p_pIGraphInfo->GetSubGraphData(i);

		if(pTmp_nSubGraphIndex->GetName() == p_nSubGraphIndex->GetName())
		{
			p_nSubGraphIndex->GetColor().SetColor5(pListBox->GetColor(0));
			OnApply_upcolor(p_nSubGraphIndex, pListBox->GetColor(1));
			OnApply_downcolor(p_nSubGraphIndex, pListBox->GetColor(2));
		}
		else
			OnApply_upcolor(p_pIGraphInfo->GetSubGraphData(i), pListBox->GetColor(i+2));
	}
*/
}

void CSetItemOfColorListBox::OnApply_GLANCEBLANCE(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex)
{
	ILPCSTR szSubGraphName = NULL;
	for(int i = 0 ; i< p_pIGraphInfo->GetSubGraphCount() ; i++)
	{
		szSubGraphName = p_pIGraphInfo->GetSubGraphNameString( i);
		OnApply_upcolor( p_pIGraphInfo, i, pListBox->GetColor(i));
		if( !strcmp( szSubGraphName, "선행스팬")) OnApply_downcolor( p_pIGraphInfo, i, pListBox->GetColor(i+1));
	}
/*
	int pSub = p_pIGraphInfo->GetAt(0);
	if(pSub != NULL)
		OnApply_upcolor(pSub, pListBox->GetColor(0));

	pSub = p_pIGraphInfo->GetAt(1);
	if(pSub != NULL)
		OnApply_upcolor(pSub, pListBox->GetColor(1));

	pSub = p_pIGraphInfo->GetAt(2);
	if(pSub != NULL)
		OnApply_upcolor(pSub, pListBox->GetColor(2));

	pSub = p_pIGraphInfo->GetAt(3);
	if(pSub != NULL) {
		OnApply_upcolor(pSub, pListBox->GetColor(3));
		OnApply_downcolor(pSub, pListBox->GetColor(4));
	}
*/
}

void CSetItemOfColorListBox::OnApply_MACD( CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex)
{
	ILPCSTR szSubGraphName = NULL;
	for(int i = 0 ; i< p_pIGraphInfo->GetSubGraphCount(); i++)
	{
		szSubGraphName = p_pIGraphInfo->GetSubGraphNameString( i);
		if( !strcmp( szSubGraphName, "Oscillator"))
		{
			p_pIGraphInfo->SetSubGraphColor( p_nSubGraphIndex, 5, pListBox->GetColor( i));
			OnApply_upcolor(	p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(i + 1));
			OnApply_downcolor(	p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(i + 2));
		}
		else OnApply_upcolor(	p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(i));
	}
}

void CSetItemOfColorListBox::OnApply_upcolor( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, const COLORREF& color)
{
	p_pIGraphInfo->SetSubGraphColor( p_nSubGraphIndex, 1, color);
	p_pIGraphInfo->SetSubGraphColor( p_nSubGraphIndex, 2, color);
}

void CSetItemOfColorListBox::OnApply_downcolor( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, const COLORREF& color)
{
	p_pIGraphInfo->SetSubGraphColor( p_nSubGraphIndex, 3, color);
	p_pIGraphInfo->SetSubGraphColor( p_nSubGraphIndex, 4, color);
}

void CSetItemOfColorListBox::OnApply_equalcolor( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, const COLORREF& color)
{
	p_pIGraphInfo->SetSubGraphColor( p_nSubGraphIndex, 5, color);
	p_pIGraphInfo->SetSubGraphColor( p_nSubGraphIndex, 6, color);
}

void CSetItemOfColorListBox::OnApply_DAEGIMEMUL(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex)
{
	ILPCSTR szSubGraphName = p_pIGraphInfo->GetSubGraphNameString( p_nSubGraphIndex);
	if( !strcmp( szSubGraphName, _MTEXT( C3_OVERHANGING_SUPPLY)))
		p_pIGraphInfo->SetSubGraphColor( p_nSubGraphIndex, pListBox->GetColor(0), pListBox->GetColor(1), pListBox->GetColor(2), pListBox->GetColor(3),
			pListBox->GetColor(4), pListBox->GetColor(5));
}

void CSetItemOfColorListBox::OnApply_INVERSELINE(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex)
{
	if( p_nSubGraphIndex < 0) return;
	if( pListBox == NULL) return;

	p_pIGraphInfo->SetSubGraphColor( p_nSubGraphIndex, pListBox->GetColor(0), pListBox->GetColor(1), pListBox->GetColor(2), pListBox->GetColor(3), 
		pListBox->GetColor(4), pListBox->GetColor(5));
}

void CSetItemOfColorListBox::SetItem_PIVOT(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST *pListBox)
{
	ILPCSTR szSubGraphName = NULL;
	for(int i = 0 ; i< p_pIGraphInfo->GetSubGraphCount() ; i++)
	{
		szSubGraphName = p_pIGraphInfo->GetSubGraphNameString( i);
		if( !strcmp( szSubGraphName, "R1"))			pListBox->AddString("1차 지지선",	p_pIGraphInfo->GetSubGraphColor( i, 1));
		else if( !strcmp( szSubGraphName, "R2"))	pListBox->AddString("2차 지지선",	p_pIGraphInfo->GetSubGraphColor( i, 1));
		else if( !strcmp( szSubGraphName, "S1"))		pListBox->AddString("1차 저항선",	p_pIGraphInfo->GetSubGraphColor( i, 1));
		else if( !strcmp( szSubGraphName, "S2"))	pListBox->AddString("2차 저항선",	p_pIGraphInfo->GetSubGraphColor( i, 1));
		else if( !strcmp( szSubGraphName, "PIVOT"))	pListBox->AddString("피봇",			p_pIGraphInfo->GetSubGraphColor( i, 1));
	}
}

void CSetItemOfColorListBox::SetItem_MACD(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST *pListBox)
{
	ILPCSTR szSubGraphName = NULL;
	for(int i = 0 ; i< p_pIGraphInfo->GetSubGraphCount(); i++)
	{
		szSubGraphName = p_pIGraphInfo->GetSubGraphNameString( i);
		if( strcmp( szSubGraphName, "Oscillator")) pListBox->AddString( szSubGraphName, p_pIGraphInfo->GetSubGraphColor( i, 1));
		else SetItem_SUB_UPFILL_DOWNFILL( p_pIGraphInfo, i, pListBox);
	}
}

//subgraph가 하나일 경우만 가능.
//여러개일 경우는 화면에..모든 subgraph의 color가 나와야 하기때문에
//이렇게는 불가능.
void CSetItemOfColorListBox::SetItembyStyleandType(const bool bSetUp, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	if( p_nSubGraphIndex < 0) return;
	
	switch( p_pIGraphInfo->GetSubGraphType( p_nSubGraphIndex))
	{
		case CGraphBaseData::Bong_Type:
			InCaseOfStylsIsBong( p_pIGraphInfo, bSetUp, p_nSubGraphIndex, pListBox);
			break;
		case CGraphBaseData::Line_Type:
			// 종찬(04/10/04) 선형;채움형; 수정
			InCaseOfStylsIsLine(bSetUp, p_nSubGraphIndex, pListBox, p_pIGraphInfo);
			break;
		case CGraphBaseData::Bar_Type:
			InCaseOfStylsIsBar(bSetUp, p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		default:
			InCaseOfStylsIsLine(bSetUp, p_nSubGraphIndex, pListBox, p_pIGraphInfo);
			break;
	}
}

//봉형;일본식 
void CSetItemOfColorListBox::InCaseOfStylsIsBong( IGraphInfo* p_pIGraphInfo, const bool bSetUp, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	if( p_nSubGraphIndex < 0) return;

	switch( p_pIGraphInfo->GetSubGraphStyle( p_nSubGraphIndex))
	{
		case CGraphBaseData::CandlestickBong:
// (2006.10.30 안승환) 바차트(고'저'종)추가		case CGraphBaseData::HighLowCloseBong:
		case CGraphBaseData::OpenHighLowCloseBong:
			InCaseOfTypeIsCandlestickandHighLowCloseBong( p_pIGraphInfo, bSetUp, p_nSubGraphIndex, pListBox);
			break;
		case CGraphBaseData::LineBong:
			InCaseOfTypeIsLineBong( p_pIGraphInfo, bSetUp, p_nSubGraphIndex, pListBox);
			break;
	}
}

void CSetItemOfColorListBox::InCaseOfTypeIsCandlestickandHighLowCloseBong( IGraphInfo* p_pIGraphInfo, const bool bSetUp, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	if( bSetUp) SetItem_UPDOWN( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
	else
	{
		OnApply_upcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(0));
		OnApply_downcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(1));
	}
}

void CSetItemOfColorListBox::InCaseOfTypeIsLineBong( IGraphInfo* p_pIGraphInfo, const bool bSetUp, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	if(bSetUp)
		SetItem_COLOR( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
	else
		OnApply_upcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(0));
}

//선형;일반식 선형;레인보우식 선형;0선비연속식 선형;선형구름식
//선형;계단식 선형;지그제그식 선형;채움식 선형;지그제그식
void CSetItemOfColorListBox::InCaseOfStylsIsLine(const bool bSetUp, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox, IGraphInfo* p_pIGraphInfo)
{
	if(bSetUp)
		SetItemInCaseOfStylsIsLine(p_nSubGraphIndex, pListBox, p_pIGraphInfo);
	else 
		OnApplyInCaseOfStylsIsLine(p_nSubGraphIndex, pListBox, p_pIGraphInfo);
//		case CGraphBaseData::HorizontalLine:// 수평식
//		case CGraphBaseData::StairLine:// 계단식
//		case CGraphBaseData::ComparisonLine:	// 대비식
//		case CGraphBaseData::CloudLine:// 구름식
//		case CGraphBaseData::DataDotSimpleLine:	// 좌표점일반
//      case CGraphBaseData::RainbowLine:	// 레인보우식
}


void CSetItemOfColorListBox::OnApplyInCaseOfStylsIsLine(int p_nSubGraphIndex, CListBoxColorPickerST* pListBox, IGraphInfo* p_pIGraphInfo)
{
	if( p_nSubGraphIndex < 0) return;

	switch( p_pIGraphInfo->GetSubGraphStyle( p_nSubGraphIndex))
	{
		case CGraphBaseData::SimpleLine:// 일반식
		case CGraphBaseData::ZigZagLine:	// 지그재그식
		case CGraphBaseData::NonZeroLine:// 0선비연속식 (값이 "0"일 경우는 그리지 않음)
			OnApply_COLOR( p_pIGraphInfo, pListBox, p_nSubGraphIndex);
			break;
		case CGraphBaseData::FillLine:// 채움식
			// 종찬(04/10/04) 선형;채움식일경우는 서브그래프가 더 있을수 있다.
			OnApply_SUB_UPFILL_DOWNFILL2( p_pIGraphInfo, pListBox, p_nSubGraphIndex);
			break;
		case CGraphBaseData::LineCloudLine:	//선형 구름식
		case CGraphBaseData::CloudLine:		//구름식
			OnApply_UPDOWN( p_pIGraphInfo, pListBox, p_nSubGraphIndex);
			break;
		default:
			OnApply_COLOR( p_pIGraphInfo, pListBox, p_nSubGraphIndex);
			break;
	}
}

void CSetItemOfColorListBox::SetItemInCaseOfStylsIsLine(int p_nSubGraphIndex, CListBoxColorPickerST* pListBox, IGraphInfo* p_pIGraphInfo)
{
	if( p_nSubGraphIndex < 0) return;

	switch( p_pIGraphInfo->GetSubGraphStyle( p_nSubGraphIndex))
	{
		case CGraphBaseData::SimpleLine:// 일반식
		case CGraphBaseData::ZigZagLine:	// 지그재그식
		case CGraphBaseData::NonZeroLine:// 0선비연속식 (값이 "0"일 경우는 그리지 않음)
			SetItem_COLOR( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CGraphBaseData::FillLine:// 채움식
			// 종찬(04/10/04) 선형;채움식일경우는 서브그래프가 더 있을수 있다.
			SetItem_SUB_UPFILL_DOWNFILL2( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CGraphBaseData::LineCloudLine:	// 선형 구름식
		case CGraphBaseData::CloudLine:		//구름식
			SetItem_UPDOWN( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		default:
			SetItem_COLOR( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
	}
}


//바형;박스식 바형;도트식 
//바형;상하등락식   
//제대로 안들어오는데..
void CSetItemOfColorListBox::InCaseOfStylsIsBar(const bool bSetUp, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	if(bSetUp)
		SetItemInCaseOfStylsIsBar(p_pIGraphInfo, p_nSubGraphIndex, pListBox);
	else 
		OnApplyInCaseOfStylsIsBar(p_pIGraphInfo, p_nSubGraphIndex, pListBox);
//      case CGraphBaseData::HorizontalBar:	// 수평식
//      case CGraphBaseData::UpDownBar:	// 상하식
//      case CGraphBaseData::CombBar:	// 빗형식
//      case CGraphBaseData::LeftRightBar:	// 좌우식
//      case CGraphBaseData::LeftRightCombBar:	// 좌우빗형식
//      case CGraphBaseData::GroupBar:	// 그룹바식
//      case CGraphBaseData::RatioBar:	// 비율식 (ex- 매수/매도 거래량 표현시)
//      case CGraphBaseData::TextHorizontalBar:	// Text가 있는 수평식
//      case CGraphBaseData::TextGroupBar:	// Text가 있는 그룹 바식
//      case CGraphBaseData::FixedRateLeftRightBar:	// 고정비율좌우식
}

void CSetItemOfColorListBox::OnApplyInCaseOfStylsIsBar(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	if( p_nSubGraphIndex < 0) return;

	switch( p_pIGraphInfo->GetSubGraphStyle( p_nSubGraphIndex))
	{
		case CGraphBaseData::VerticalBar:// 수직식
			{
				BOOL bApplyed = FALSE;
				IDoubleList *pIConditionList = p_pIGraphInfo->GetCommonCalculateConditionList();
				if( pIConditionList)
				{
					if( pIConditionList->GetCount())
					{
						OnApply_UPDOWN( p_pIGraphInfo, pListBox, p_nSubGraphIndex);
						bApplyed = TRUE;
					}
					pIConditionList->Release();
				}
				if( !bApplyed) OnApply_COLOR( p_pIGraphInfo, pListBox, p_nSubGraphIndex);
			}
			break;
		case CGraphBaseData::BoxBar:	// 박스식
		case CGraphBaseData::DotBar:	// 도트식
			OnApply_UPDOWN( p_pIGraphInfo, pListBox, p_nSubGraphIndex);
			break;
		case CGraphBaseData::UpDownHighLowBar:	// 상하등락식
			OnApply_SUB_UPFILL_DOWNFILL( p_pIGraphInfo, pListBox, p_nSubGraphIndex);
			break;
		default:
			OnApply_COLOR( p_pIGraphInfo, pListBox, p_nSubGraphIndex);
			break;
	}

}

void CSetItemOfColorListBox::SetItemInCaseOfStylsIsBar(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	if( p_nSubGraphIndex < 0) return;

	switch( p_pIGraphInfo->GetSubGraphStyle( p_nSubGraphIndex))
	{
		case CGraphBaseData::VerticalBar:// 수직식
		{
			//상승하락 표현
			BOOL bSetted = FALSE;
			IDoubleList *pIConditionList = p_pIGraphInfo->GetCommonCalculateConditionList();
			if( pIConditionList)
			{
				if( pIConditionList->GetCount())
				{
					SetItem_UPDOWN( p_pIGraphInfo, p_nSubGraphIndex, pListBox);						
					bSetted = TRUE;
				}
				pIConditionList->Release();
			}
			if( !bSetted) SetItem_COLOR( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		}
		case CGraphBaseData::BoxBar:	// 박스식
		case CGraphBaseData::DotBar:	// 도트식
			SetItem_UPDOWN( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CGraphBaseData::UpDownHighLowBar:	// 상하등락식
			SetItem_SUB_UPFILL_DOWNFILL( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		default:
			SetItem_COLOR( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
	}
}


