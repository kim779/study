// BlocksColumn.cpp: implementation of the CBlocksColumn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlocksColumn.h"

#include <assert.h>
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/Conversion.h"
#include "../Include_Chart/ViewData.h"
#include "../Include_Chart/ScaleDataList.h"

#include "MainBlockImp.h"
#include "BlockImp.h"
#include "VertScaleGroup.h"
#include "GraphSearchInfo.h"				// for CGraphSearchInfo
#include "DlgBlockInsert.h"					// for CDlgBlockInsert
#include "GraphImp.h"							// for CGraphImp

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ========================================================================================
CBlocksColumn::CBlocksColumn(CMainBlockImp* pMain):
	m_pMain(pMain)
{
	// (2008/1/22 - Seung-Won, Bae) for Multi-Language
	m_hOcxWnd = pMain->GetMainBlockBaseData()->GetParent_Handle();
}

CBlocksColumn::~CBlocksColumn()
{
	DeleteAll();
}

//////////////////////////////////////////////////////////////////////
// Base Setting
//////////////////////////////////////////////////////////////////////

CMainBlockBaseData* CBlocksColumn::GetMainBlockBaseData() const
{
	if(m_pMain == NULL)
		return NULL;

	return m_pMain->GetMainBlockBaseData();
}

// 모든 block이 두배 size 아님
void CBlocksColumn::InitializeNormalSizeBlockNumber( void)
{
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlockImp* pBlock = m_BlockList.GetNext(pos);
		assert(pBlock != NULL);
		pBlock->GetBlockData().SetDoubleSize(false);
	}
}

// (2006/11/12 - Seung-Won, Bae) Support VScale Min/Max Type Change for All VSCale
void CBlocksColumn::SetAllVScaleMinMaxType( CScaleBaseData::VERTMINMAXTYPE p_eVScaleMinMaxType)
{
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlockImp* pBlock = m_BlockList.GetNext(pos);
		if( !pBlock) return;
		pBlock->GetVertScaleGroupList().SetAllVScaleMinMaxType( p_eVScaleMinMaxType);
	}
}

// (2006/11/14 - Seung-Won, Bae) Support VScale Min/Max Type Release in TR
void CBlocksColumn::ReleaseAllVScaleSettingInFixedMinMax( void)
{
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlockImp* pBlock = m_BlockList.GetNext(pos);
		if( !pBlock) return;
		pBlock->GetVertScaleGroupList().ReleaseAllVScaleSettingInFixedMinMax();
	}
}

//////////////////////////////////////////////////////////////////////
// Create
//////////////////////////////////////////////////////////////////////

// 객체(blocksColumn)를 생성한다.
CBlocksColumn* CBlocksColumn::Make(CMainBlockImp* pMain)
{
	return new CBlocksColumn(pMain);
}

// 기존의 Block들을 삭제하고 원하는 갯수로 Block 만든다.
void CBlocksColumn::MakeBlocksColumn(const int nRowCount)
{
	DeleteAll();

	for(int nRow = 0; nRow < nRowCount; nRow++) m_BlockList.AddTail( CBlockImp::Make( GetMainBlockBaseData(), this));
}

// block의 graph 및 scale 을 생성한다.
// 그래프 생성후 계산 여부 : xScaleManager - ojtaso (20070701)
void CBlocksColumn::BuildGraphsAndScales( char *p_szBlockColumnData, BOOL& bCalc)
{
	if( !p_szBlockColumnData) return;
	if( !*p_szBlockColumnData) return;

	char *szBlock = strstr( p_szBlockColumnData, "Block;\r\n");
	if( !szBlock) return;
	szBlock += 8;												// "Block;\r\n"
	char *szNextBlock = strstr( szBlock, "BlockEnd;\r\n");
	if( szNextBlock) *szNextBlock = '\0';

	CBlockImp* pBlock = NULL;
	POSITION posBlock = m_BlockList.GetHeadPosition();
	while( posBlock && szBlock)
	{
		pBlock = m_BlockList.GetNext( posBlock);
		if( !pBlock) continue;

		// 그래프 생성후 계산 여부 : xScaleManager - ojtaso (20070701)
		pBlock->BuildGraphsAndScales( szBlock, bCalc);

		if( szNextBlock)
		{
			*szNextBlock = 'B';

			szBlock = strstr( szNextBlock, "Block;\r\n");
			if( szBlock)
			{
				szBlock += 8;		// "Block;\r\n"

				szNextBlock = strstr( szBlock, "BlockEnd;\r\n");
				if( szNextBlock) *szNextBlock = '\0';
			}
		}
		else szBlock = NULL;
	}
	//>> JS.Kim_20100929  solomon.. PAIR RQ는 저장하지 않도록 수정..
	//  Block 정보가 없는 Block은 삭제하자.
	if( posBlock && !szBlock )
	{
		while( posBlock )
		{
			pBlock = m_BlockList.GetNext( posBlock);
			if( !pBlock) continue;

			DeleteBlock(pBlock, FALSE);
		}
	}
	//}}
}

// RQ로 검색 : 복수종목 - ojtaso (20070118)
void CBlocksColumn::BuildGraphsAndScales(LPVOID pPacketRQ, char *p_szBlockColumnData)
{
	if( !p_szBlockColumnData) return;
	if( !*p_szBlockColumnData) return;

	char *szBlock = strstr( p_szBlockColumnData, "Block;\r\n");
	if( !szBlock) return;
	szBlock += 8;												// "Block;\r\n"
	char *szNextBlock = strstr( szBlock, "BlockEnd;\r\n");
	if( szNextBlock) *szNextBlock = '\0';

	CBlockImp* pBlock = NULL;
	POSITION posBlock = m_BlockList.GetHeadPosition();
	while( posBlock && szBlock)
	{
		pBlock = m_BlockList.GetNext( posBlock);
		if( !pBlock) continue;

		pBlock->BuildGraphsAndScales(pPacketRQ, szBlock);

		if( szNextBlock)
		{
			*szNextBlock = 'B';

			szBlock = strstr( szNextBlock, "Block;\r\n");
			if( szBlock)
			{
				szBlock += 8;		// "Block;\r\n"

				szNextBlock = strstr( szBlock, "BlockEnd;\r\n");
				if( szNextBlock) *szNextBlock = '\0';
			}
		}
		else szBlock = NULL;
	}
}

// blocksColumn의 모든 data
void CBlocksColumn::GetBlocksColumnAllData( CStringList &p_slBlockColumnData) const
{
	p_slBlockColumnData.AddTail( "BlockColumn;\r\n");

	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlockImp* pBlock = m_BlockList.GetNext(pos);
		assert(pBlock != NULL);
		pBlock->GetBlockAllData( p_slBlockColumnData);
	}

	p_slBlockColumnData.AddTail( "BlockColumnEnd;\r\n");
}

bool CBlocksColumn::InsertBlock_LoadDialog(CInsertingBlockHorzScaleData& insertingBlockHorzScaleData)
{
	CMainBlockBaseData* pMainBlockBaseData = GetMainBlockBaseData();
	// ( 2006 / 07 / 25 - Sang-Woo, Cho ) 
	// 원래 소스는 Block이 없거나 하나의 Block이 존재할 때에만 추가하는 지표의 Block을 바닥으로 보내게
	// 되어있었는데 Block의 개수가 하나인지을 확인하는 부분을 주석처리해서 항상 바닥으로 보내게 수정함.
	//if(pMainBlockBaseData == NULL || GetBlockCount() != 1)
	if( pMainBlockBaseData == NULL )
		return false;
	// End

	if( !pMainBlockBaseData->DoesMessageBoxShow())
	{
		// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
		//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
		m_pMain->SetHorzScalePosition( insertingBlockHorzScaleData.GetHorizontalScale(), CScaleBaseData::HORZ_BOTTOM);
		return false;
	}

	// (2007/6/27 - Seung-Won, Bae) Show UI Directly.
	//	CChartObjectWrapperHelper wrapperHelper(m_pMain->GetOCXPath());
	//	return wrapperHelper.InsertBlock(pMainBlockBaseData->GetParentWnd(), &insertingBlockHorzScaleData);
	// (2009/9/23 - Seung-Won, Bae) Use '{}' for ML_SET_LANGUAGE_RES()'s re-using.
	{
		ML_SET_LANGUAGE_RES();
		CString strMessage;
		strMessage.LoadString( IDS_X_SCALE_SETTING);
		CString strTitle;
		strTitle.LoadString( IDS_SELECT);
		if( IDYES != MessageBox( pMainBlockBaseData->GetParentWnd()->GetSafeHwnd(), strMessage, strTitle, MB_ICONQUESTION | MB_YESNO))
			return false;
	}

	ML_SET_LANGUAGE_RES();
	CDlgBlockInsert dlg( pMainBlockBaseData->GetParentWnd(), m_pMain, &insertingBlockHorzScaleData);
	BOOL bResult = (IDOK == dlg.DoModal());

	pMainBlockBaseData->SetMessageBoxUsed( bResult);
	return bResult ? true : false;
}

// insert 할 Block 구하기
// bIsAutoEmptyBlock == true : 현재의 column 에서 graph 가 없는 block이 insert block 가 된다.
CBlockImp* CBlocksColumn::InsertBlock_GetBlockForInsert(const int nInsertPosition, const bool bIsAutoEmptyBlock)
{
	// graph가 없는 빈block이 있는지 찾는다!
	if(bIsAutoEmptyBlock)
	{
		CGraphSearchInfo_AnyGraph gsi;
		if( !FindGraph( &gsi)) return gsi.m_pBlockImp;
	}

	// null graph 인 block 가 없을 경우 새 block을 만든다.
	SendMessageToParent_BlockInsert(nInsertPosition);
	return CBlockImp::Make(GetMainBlockBaseData(), this);
}

// bIsAutoOneBlockCount == true : block count가 1이하이면 
//                             삽입위치(nInsertPosition) 의 하단에 삽입한다.
// (2007/2/3 - Seung-Won, Bae) Do not control the Fixed X Scale Position.
//		Because. It is called with Building Chart, not Update.
POSITION CBlocksColumn::InsertBlock_WithPosition(CBlockImp* pInsertBlock, const int nInsertPosition, const bool bIsAutoOneBlockCount)
{
	int nBlockCount = m_BlockList.GetCount();

	// 새로운 block을 insert한다. -> 지표추가인 경우 사용
	if(bIsAutoOneBlockCount)
	{
		// block이 하나일경우는 무조건 뒤에 추가
		if(nBlockCount <= 1)
			return m_BlockList.InsertAfter(m_BlockList.FindIndex(nInsertPosition), pInsertBlock);

		if(nBlockCount <= nInsertPosition)
			return m_BlockList.InsertAfter(m_BlockList.GetTailPosition(), pInsertBlock);
			
		return m_BlockList.InsertBefore(m_BlockList.FindIndex(nInsertPosition), pInsertBlock);
	}

	// -> 지표외의 block관련 사용 (empty block 같은 경우)
	// 가장 앞에 추가
	if(nInsertPosition <= 0)
		return m_BlockList.InsertBefore(m_BlockList.GetHeadPosition(), pInsertBlock);
	
	// 가장 뒤에 추가
	else if(nBlockCount <= nInsertPosition)
		return m_BlockList.InsertAfter(m_BlockList.GetTailPosition(), pInsertBlock);

	return m_BlockList.InsertBefore(m_BlockList.FindIndex(nInsertPosition), pInsertBlock);

}

bool CBlocksColumn::InsertBlock( const int nInsertPosition, char *p_szBlockData)
{
	if( nInsertPosition < 0) return false;
	if( !p_szBlockData) return false;
	if( !*p_szBlockData) return false;

	char *szVertScaleGroupData = strstr( p_szBlockData, "VertScaleGroup;");
	if( !szVertScaleGroupData) return false;
	*szVertScaleGroupData = '\0';

	CHorizontalScale horzScale;
	horzScale.SetScaleData( p_szBlockData, m_pMain->GetParent_Handle());
	*szVertScaleGroupData = 'V';

	//가로 scale setting Dlg 띄우기
	CInsertingBlockHorzScaleData insertingBlockHorzScaleData( true, horzScale);
	
	// (2007/6/27 - Seung-Won, Bae) Support Manual Setting of X Scale for New Block.
	bool bResultDlg = InsertBlock_LoadDialog( insertingBlockHorzScaleData);

	//data setting
	CBlockImp* pInsertBlock = InsertBlock_GetBlockForInsert( nInsertPosition);
	if(pInsertBlock == NULL)
		return false;

	// 그래프 생성후 계산 여부 : xScaleManager - ojtaso (20070701)
	BOOL bCalc = FALSE;
	pInsertBlock->BuildGraphsAndScales( p_szBlockData, bCalc);
	if(bResultDlg && pInsertBlock)
	{
		if( insertingBlockHorzScaleData.IsOnlyBottomBlockShow()) SetAllHorzScalePosition( CScaleBaseData::HORZ_HIDE);
		pInsertBlock->GetBlockData().SetHorzScale(insertingBlockHorzScaleData.GetHorizontalScale());
	}

	//graph 생성된 후 insert 함
	//-> TR 요청을 날리는데 이때 외부에서 draw를 부르는 경우가 있음
	//	position이 추가되지 않아 draw 하면서 죽는 현상있으
	InsertBlock_WithPosition( pInsertBlock, nInsertPosition, true);
	return true;
}

// RQ로 검색 : 복수종목 - ojtaso (20070118)
bool CBlocksColumn::InsertBlock(LPVOID pPacketRQ, const int nInsertPosition, char *p_szBlockData)
{
	if( nInsertPosition < 0) return false;
	if( !p_szBlockData) return false;
	if( !*p_szBlockData) return false;

	char *szVertScaleGroupData = strstr( p_szBlockData, "VertScaleGroup;");
	if( !szVertScaleGroupData) return false;
	*szVertScaleGroupData = '\0';

	CHorizontalScale horzScale;
	horzScale.SetScaleData( p_szBlockData, m_pMain->GetParent_Handle());
	*szVertScaleGroupData = 'V';

	//가로 scale setting Dlg 띄우기
	CInsertingBlockHorzScaleData insertingBlockHorzScaleData( true, horzScale);
	
	// ( 2006 / 07 / 31 - Sang-Woo, Cho ) - 지표추가시 Scale설정 Dialog가 보이지 않게 수정.
	bool bResultDlg = InsertBlock_LoadDialog( insertingBlockHorzScaleData);
	//bool bResultDlg = true;
	//insertingBlockHorzScaleData.SetOnlyBottomBlockShow(true);
	insertingBlockHorzScaleData.SetOnlyBottomBlockShow(false);

	// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
	//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
	m_pMain->SetHorzScalePosition( insertingBlockHorzScaleData.GetHorizontalScale(), ( CScaleBaseData::HORZSCALEPOSITION)1);

	//insertingBlockHorzScaleData.GetHorizontalScale().SetGridType(3);//(CScaleBaseData::SCALEGRIDTYPE)SLNONE
	// End

	//data setting
	CBlockImp* pInsertBlock = InsertBlock_GetBlockForInsert( nInsertPosition);
	if(pInsertBlock == NULL)
		return false;

	pInsertBlock->BuildGraphsAndScales(pPacketRQ, p_szBlockData);
	if(bResultDlg && pInsertBlock)
	{
		if( insertingBlockHorzScaleData.IsOnlyBottomBlockShow()) SetAllHorzScalePosition( CScaleBaseData::HORZ_HIDE);
		pInsertBlock->GetBlockData().SetHorzScale(insertingBlockHorzScaleData.GetHorizontalScale());
	}

	//graph 생성된 후 insert 함
	//-> TR 요청을 날리는데 이때 외부에서 draw를 부르는 경우가 있음
	//	position이 추가되지 않아 draw 하면서 죽는 현상있으
	InsertBlock_WithPosition( pInsertBlock, nInsertPosition, true);
	return true;
}
// ==> 끝에서 두번째에 있는 위치에 추가
bool CBlocksColumn::InsertBlocks(const int nInsertPosition, const CString& strGraphData)
{
	if( strGraphData.IsEmpty()) return false;
	CString strBlockData;
	char *szBlockData = strBlockData.GetBufferSetLength( strGraphData.GetLength());
	strcpy( szBlockData, strGraphData);

	bool bResult = false;
	int nInsertCount = 0;
	szBlockData = strstr( szBlockData, "Block;\r\n");
	while( szBlockData)
	{
		szBlockData += 8;												// "Block;\r\n";

		char *szBlockDataEnd = strstr( szBlockData, "BlockEnd;\r\n");
		if( szBlockDataEnd) *szBlockDataEnd = '\0';

		bool bResult2 = InsertBlock( nInsertPosition + nInsertCount, szBlockData);
		if( szBlockDataEnd) *szBlockDataEnd = 'B';
		nInsertCount++;

		bResult |= bResult2;
		if( szBlockDataEnd) szBlockData = strstr( szBlockDataEnd, "Block;\r\n");
		else szBlockData = NULL;
	}

	return bResult;
}

// RQ로 검색 : 복수종목 - ojtaso (20070118)
bool CBlocksColumn::InsertBlocks(LPVOID pPacketRQ, const int nInsertPosition, const CString& strGraphData)
{
	if( strGraphData.IsEmpty()) return false;
	CString strBlockData;
	char *szBlockData = strBlockData.GetBufferSetLength( strGraphData.GetLength());
	strcpy( szBlockData, strGraphData);

	bool bResult = false;
	int nInsertCount = 0;
	szBlockData = strstr( szBlockData, "Block;\r\n");
	while( szBlockData)
	{
		szBlockData += 8;												// "Block;\r\n";

		char *szBlockDataEnd = strstr( szBlockData, "BlockEnd;\r\n");
		if( szBlockDataEnd) *szBlockDataEnd = '\0';

		bool bResult2 = InsertBlock(pPacketRQ, nInsertPosition + nInsertCount, szBlockData);
		if( szBlockDataEnd) *szBlockDataEnd = 'B';
		nInsertCount++;

		bResult |= bResult2;
		if( szBlockDataEnd) szBlockData = strstr( szBlockDataEnd, "Block;\r\n");
		else szBlockData = NULL;
	}

	return bResult;
}

// ==> 끝에서 두번째에 있는 위치에 추가
bool CBlocksColumn::InsertBlocks( const CString& strGraphData)
{
	if( !strGraphData.IsEmpty()) return false;

	SendMessageToParent_BlockInsert(0);
	MakeBlocksColumn(CDataConversion::GetStringDataCount(strGraphData, "Block;\r\n"));

	CString strBlockColumnData;
	char *szBlockColumnData = strBlockColumnData.GetBufferSetLength( strGraphData.GetLength());
	strcpy( szBlockColumnData, strGraphData);
	// 그래프 생성후 계산 여부 : xScaleManager - ojtaso (20070701)
	BOOL bCalc = FALSE;
	BuildGraphsAndScales( szBlockColumnData, bCalc);
	return true;
}

// RQ로 검색 : 복수종목 - ojtaso (20070118)
bool CBlocksColumn::InsertBlocks(LPVOID pPacketRQ, const CString& strGraphData)
{
	if( !strGraphData.IsEmpty()) return false;

	SendMessageToParent_BlockInsert(0);
	MakeBlocksColumn(CDataConversion::GetStringDataCount(strGraphData, "Block;\r\n"));

	CString strBlockColumnData;
	char *szBlockColumnData = strBlockColumnData.GetBufferSetLength( strGraphData.GetLength());
	strcpy( szBlockColumnData, strGraphData);
	BuildGraphsAndScales(pPacketRQ, szBlockColumnData);
	return true;
}

// block을 추가 한다.
bool CBlocksColumn::InsertBlock(const int nInsertPosition, const bool bDoesMaintainOldScale)
{
	if(nInsertPosition < 0 || nInsertPosition > m_BlockList.GetCount())
		return false;

	CBlockImp* pInsertBlock = InsertBlock_GetBlockForInsert( nInsertPosition, false);
	if(pInsertBlock == NULL) return false;
	InsertBlock_WithPosition( pInsertBlock, nInsertPosition, false);

	// 가로/세로 scale를 셋팅.
	if(bDoesMaintainOldScale) SetMaintainOldScale( pInsertBlock);

	return (pInsertBlock != NULL);
}

//////////////////////////////////////////////////////////////////////
// Delete
//////////////////////////////////////////////////////////////////////

// 객체를 삭제한다. Make에서 new를 했으므로..
void CBlocksColumn::Delete(CBlocksColumn* pBlocksColumn)
{
	delete pBlocksColumn;
}

// 리스트의 모든 내용(block 들) 삭제
void CBlocksColumn::DeleteAll( void)
{
	// (2007/2/16 - Seung-Won, Bae) Remove in list after Deleting the Object.
	//		In Destructor, can be access the List.
	while( !m_BlockList.IsEmpty())
	{
		CBlockImp::Delete( m_BlockList.GetHead());
		m_BlockList.RemoveHead();
	}
}

// 해당 point의 block을 삭제
bool CBlocksColumn::DeleteBlock(CBlockImp* pBlock, const bool bAutoHorzScaleDelete)
{
	assert(pBlock != NULL);
	POSITION pos = m_BlockList.Find(pBlock);
	if(pos == NULL)
		return false;

	if( !bAutoHorzScaleDelete)
	{
		// HORZ_TOP, HORZ_BOTTOM, HORZ_BOTH 일 때만 Dlg 보임
		CScaleBaseData::HORZSCALEPOSITION horzPosition = pBlock->GetBlockData().GetHorzScale().GetHorzScalePosition();
		if( horzPosition == CScaleBaseData::HORZ_TOP || horzPosition == CScaleBaseData::HORZ_BOTTOM || horzPosition == CScaleBaseData::HORZ_BOTH)
		{
			CMainBlockBaseData* pMainBlockBaseData = GetMainBlockBaseData();
			int nAnswer = IDNO;
			if( pMainBlockBaseData->DoesMessageBoxShow())
			{
				ML_SET_LANGUAGE_RES();
				CString strMessage;
				strMessage.LoadString( IDS_USE_OLD_X_SCALE);
				CString strTitle;
				strTitle.LoadString( IDS_SELECT);
				if( !pMainBlockBaseData)	nAnswer = AfxMessageBox(strMessage, MB_ICONQUESTION | MB_YESNO);
				else						nAnswer = MessageBox(pMainBlockBaseData->GetParent_Handle(), strMessage, strTitle, MB_ICONQUESTION | MB_YESNO);
			}
			else nAnswer = IDYES;

			if( nAnswer == IDYES)
			{
				pMainBlockBaseData->GetScaleAttributes().SetHorzScaleMessageBoxShow(false);

				POSITION pos = m_BlockList.Find(pBlock);
				if( pos)
				{
					if( pos == m_BlockList.GetTailPosition())	m_BlockList.GetPrev(pos);
					else										m_BlockList.GetNext(pos);

					CBlockImp* pTargetBlock = m_BlockList.GetAt( pos);
					if( pTargetBlock)
					{
						// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
						//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
						m_pMain->SetHorzScalePosition( pTargetBlock->GetBlockData().GetHorzScale(), pBlock->GetBlockData().GetHorzScale().GetHorzScalePosition());
					}
				}
			}
		}
	}

	if( m_pMain)
	{
		CBlockBehaviorData blockBehaviorData(CBlockBaseData::BLOCK_DELETE, m_pMain->FindBlockNumber(pBlock));
		GetMainBlockBaseData()->SendMessageToParent(blockBehaviorData);
	}

	// (2007/2/16 - Seung-Won, Bae) Remove in list after Deleting the Object.
	//		In Destructor, can be access the List.
	CBlockImp::Delete( pBlock);
	m_BlockList.RemoveAt(pos);
	return true;
}

BOOL CBlocksColumn::DeleteAllIndicator( const char *p_szIndicatorName)
{
	if( !p_szIndicatorName)		return FALSE;
	if( !*p_szIndicatorName)	return FALSE;

	BOOL bResult = FALSE;
	CBlockImp *pBlock = NULL;
	POSITION posBlock = m_BlockList.GetHeadPosition();
	while( posBlock)
	{
		pBlock = m_BlockList.GetNext( posBlock);
		if( !pBlock) continue;
		if( pBlock->GetVertScaleGroupList().DeleteAllIndicator( p_szIndicatorName)) bResult = TRUE;
	}
	return bResult;
}

//////////////////////////////////////////////////////////////////////
// Search Object
//////////////////////////////////////////////////////////////////////

// Block
CBlockImp *CBlocksColumn::GetBlock( int p_nRowIndex) const
{
	if( p_nRowIndex < 0 || GetBlockCount() <= p_nRowIndex) return NULL;

	POSITION posBlock = m_BlockList.FindIndex( p_nRowIndex);
	if( !posBlock) return NULL;

	return m_BlockList.GetAt( posBlock);
}

// 해당 block의 세로 scale 값
CVertScaleGroup *CBlocksColumn::GetVertScaleGroup( const int nBlockRowIndex, const int nVertScaleGroupIndex) const
{
	CBlockImp* pBlock = GetBlock( nBlockRowIndex);
	if( !pBlock) return NULL;

	return pBlock->GetVertScaleGroupList().GetVertScaleGroup( nVertScaleGroupIndex);
}

// (2007/2/22 - Seung-Won, Bae) Support Next Search.
BOOL CBlocksColumn::FindGraph( CGraphSearchInfo *p_pGraphSearchInfo) const
{
	if( !p_pGraphSearchInfo)	return FALSE;

	int nRowBlockIndex	= p_pGraphSearchInfo->m_nRowBlockIndex;
	POSITION posBlock	= ( p_pGraphSearchInfo->m_posRowBlock ? p_pGraphSearchInfo->m_posRowBlock : m_BlockList.FindIndex( p_pGraphSearchInfo->m_nRowBlockIndex));

	CBlockImp *pBlock = NULL;
	POSITION posCurrentBlock = NULL;
	while( posBlock)
	{
		posCurrentBlock = posBlock;
		pBlock = m_BlockList.GetNext( posBlock);
		if( !pBlock) continue;
		if( p_pGraphSearchInfo->FindGraph( pBlock))
		{
			p_pGraphSearchInfo->m_posRowBlock		= posCurrentBlock;
			p_pGraphSearchInfo->m_nRowBlockIndex	= nRowBlockIndex;
			p_pGraphSearchInfo->m_pBlockImp			= pBlock;
			return TRUE;
		}
		nRowBlockIndex++;
	}

	// (2007/4/11 - Seung-Won, Bae) Reset for Next BlockColumn Search
	p_pGraphSearchInfo->m_posRowBlock		= NULL;
	p_pGraphSearchInfo->m_nRowBlockIndex	= 0;
	return FALSE;
}

// RQ로 검색 : 복수종목 - ojtaso (20070118)
BOOL CBlocksColumn::FindGraph(LPVOID pPacketRQ, CGraphSearchInfo *p_pGraphSearchInfo) const
{
	if( !p_pGraphSearchInfo)	return FALSE;

	int nRowBlockIndex = 0;
	CBlockImp *pBlock = NULL;
	POSITION posBlock = m_BlockList.GetHeadPosition();
	while( posBlock)
	{
		pBlock = m_BlockList.GetNext( posBlock);
		if( !pBlock) continue;
		if( p_pGraphSearchInfo->FindGraph(pPacketRQ, pBlock))
		{
			p_pGraphSearchInfo->m_nRowBlockIndex = nRowBlockIndex;
			p_pGraphSearchInfo->m_pBlockImp = pBlock;
			return TRUE;
		}
		nRowBlockIndex++;
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// Move
//////////////////////////////////////////////////////////////////////

// 해당 block은 모든 vertScaleGroup를 block로 만든다.
// bDoesMaintainOldScale = true : 가장 하단에 있는 block 만 Y축 scale 이 보이고 나머지 block는 숨김.
bool CBlocksColumn::MoveAllVertScaleGroupsInPartition(const int nBlockRowIndex, const bool bDoesMaintainOldScale)
{
	CBlockImp* pTargetBlock = GetBlock(nBlockRowIndex);
	if(pTargetBlock == NULL)
		return false;

	bool bResult = false;
	CVertScaleGroupList& targetVertScaleGroupList = pTargetBlock->GetVertScaleGroupList();
	int nVertScaleGroupCount = targetVertScaleGroupList.GetCount();
	for(int nIndex = 1; nIndex < nVertScaleGroupCount; nIndex++)
	{
		CVertScaleGroup* pSourceVertScaleGroup = targetVertScaleGroupList.GetVertScaleGroup(1);
		if( !pSourceVertScaleGroup) continue;

		CBlockImp* pNewBlock = CBlockImp::Make(GetMainBlockBaseData(), this);
		if( !pNewBlock) continue;

		if( !pNewBlock->GetVertScaleGroupList().AddTailInNoEqual( pSourceVertScaleGroup))
		{
			CBlockImp::Delete(pNewBlock);
			continue;
		}

		m_BlockList.AddTail( pNewBlock);
		
		pNewBlock->GetBlockData() = pTargetBlock->GetBlockData();
		// 가로/세로 scale를 셋팅.
		if(bDoesMaintainOldScale) SetMaintainOldScale(pNewBlock);

		if( targetVertScaleGroupList.RemoveAt( pSourceVertScaleGroup, false)) bResult = true;
	}

	return bResult;
}

// block move
bool CBlocksColumn::MoveBlock(CBlocksPosition* pBlocksPosition, const int nSourceBlockIndex, const int nTargetBlockIndex)
{
	if(nSourceBlockIndex >= m_BlockList.GetCount() || nTargetBlockIndex >= m_BlockList.GetCount())
		return false;

	// block 위치 및 값 변경
	if(!pBlocksPosition->MoveBlockPosition(nSourceBlockIndex, nTargetBlockIndex, CBlockBaseData::ROW))
		return false;

	if( m_pMain)
	{
		CGraphSearchInfo_BlocksColumn gsi;
		gsi.m_pBlocksColumn = this;
		if( m_pMain->FindGraph( &gsi))
		{
			CBlockBehaviorData blockBehaviorData(CBlockBaseData::BLOCK_MOVE, CBlockIndex(nSourceBlockIndex, gsi.m_nColumnBlockIndex), CBlockIndex(nTargetBlockIndex, gsi.m_nColumnBlockIndex));
			GetMainBlockBaseData()->SendMessageToParent(blockBehaviorData);
		}
	}

	POSITION sourcePos = m_BlockList.FindIndex( nSourceBlockIndex);
	POSITION targetPos = m_BlockList.FindIndex( nTargetBlockIndex);
	bool bInsertBefore = ( nSourceBlockIndex > nTargetBlockIndex);

	POSITION psnNext = sourcePos;
	if( bInsertBefore)	m_BlockList.GetPrev( psnNext);
	else				m_BlockList.GetNext( psnNext);

	CBlockImp *pBlock = m_BlockList.GetAt( sourcePos);
	if( !pBlock) return false;
	CScaleBaseData::HORZSCALEPOSITION eXScalePosition = pBlock->GetBlockData().GetHorzScale().GetHorzScalePosition();
	m_BlockList.RemoveAt( sourcePos);
	if(bInsertBefore)	sourcePos = m_BlockList.InsertBefore(	targetPos, pBlock);
	else				sourcePos = m_BlockList.InsertAfter(	targetPos, pBlock);

	// (2007/2/3 - Seung-Won, Bae) Control the Fixed X Scale Position.
	CScaleBaseData::HORZSCALEPOSITION eXScalePositionNext = CScaleBaseData::HORZ_NONE;
	POSITION psnUpdate = NULL;
	while( psnUpdate != sourcePos)
	{
		psnUpdate = psnNext;
		if( bInsertBefore)	pBlock = m_BlockList.GetPrev( psnNext);
		else				pBlock = m_BlockList.GetNext( psnNext);
		if( !pBlock) continue;
		eXScalePositionNext = pBlock->GetBlockData().GetHorzScale().GetHorzScalePosition();

		// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
		//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
		m_pMain->SetHorzScalePosition( pBlock->GetBlockData().GetHorzScale(), eXScalePosition);

		eXScalePosition = eXScalePositionNext;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// Query & Update
//////////////////////////////////////////////////////////////////////

// -> BlockBackground 만을 그린다.
void CBlocksColumn::DrawAllBlocksBackground(CDC* pDC, const int nBlockColumnIndex, const CBlocksPosition& blocksPosition)
{
	//sy test
	POSITION pos = m_BlockList.GetHeadPosition();
	// block을 그린다.
	int nBlockRowIndex = -1;
	while(pos != NULL)
	{
		nBlockRowIndex++;
		CBlockImp* pBlock = (CBlockImp*) m_BlockList.GetNext(pos);
		assert(pBlock != NULL);
		pBlock->DrawBlockBackground(pDC, blocksPosition.GetBlockRegion(CBlockIndex(nBlockRowIndex, nBlockColumnIndex)), 
			blocksPosition.GetInterBlockHalfGap());
	}
}

// block HorzScale, VertScale을 그린다.
void CBlocksColumn::DrawAllHorzAndVertScales(CDC* pDC, const int nBlockColumnIndex, const CBlocksPosition& blocksPosition)
{
	//sy test
	POSITION pos = m_BlockList.GetHeadPosition();
	// block을 그린다.
	int nBlockRowIndex = -1;
	while(pos != NULL)
	{
		nBlockRowIndex++;
		CBlockImp* pBlock = (CBlockImp*) m_BlockList.GetNext(pos);
		assert(pBlock != NULL);
		pBlock->DrawHorzAndVertScale(pDC, blocksPosition.GetBlockRegion(CBlockIndex(nBlockRowIndex, nBlockColumnIndex)));
	}
}

// block HorzScale, VertScale을 그린다.
void CBlocksColumn::DrawAllGraphs(CDC* pDC, const int nBlockColumnIndex, const CBlocksPosition& blocksPosition)
{
	//sy test
	POSITION pos = m_BlockList.GetHeadPosition();
	// block을 그린다.
	int nBlockRowIndex = -1;
	while(pos != NULL)
	{
		nBlockRowIndex++;
		CBlockImp* pBlock = (CBlockImp*) m_BlockList.GetNext(pos);
		assert(pBlock != NULL);
		pBlock->DrawAllGraphs(pDC, blocksPosition.GetBlockRegion(CBlockIndex(nBlockRowIndex, nBlockColumnIndex)));
	}
}

// vert scale Min/Max 를 계산한다.
void CBlocksColumn::CalculateDisplayDataMinMaxAllVertScaleGroup( void)
{
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlockImp* pBlock = m_BlockList.GetNext(pos);
		assert(pBlock != NULL);
		pBlock->GetVertScaleGroupList().CalculateDisplayDataMinMaxAllVertScaleGroup();
	}
}
// drawing region 을 계산한다.
void CBlocksColumn::CalculateDrawingRegion(CDC* pDC, const int nBlockColumnIndex, const CBlocksPosition& blocksPosition)
{
	// 세로 scale의 가로 길이 구한다. -> 모든 block의 왼쪽 scale의 길이가 같다.
	int leftWidth = 0;		// block에 있는 왼쪽 scale의 가로 길이를 구한다.
	int rightWidth = 0;		// block에 있는 오른쪽 scale의 가로 길이를 구한다.
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlockImp* pBlock = (CBlockImp*) m_BlockList.GetNext(pos);
		assert(pBlock != NULL);
		pBlock->GetVertScaleGroupList().GetVertScaleRegionWidth( pDC, leftWidth, rightWidth);
	}

	// block의 세로 scale 폭을 구한다. -> 가로 scale 높이는 block에서 구한다.
//@Solomon-100130 test by alzioyes //rightWidth = 48;
	m_vertScaleRegionWidth = CVertScaleRegionWidth( leftWidth, rightWidth);

	pos = m_BlockList.GetHeadPosition();
	// block을 그린다.
	int nBlockRowIndex = -1;
	while(pos != NULL)
	{
		nBlockRowIndex++;
		CBlockImp* pBlock = (CBlockImp*) m_BlockList.GetNext(pos);
		assert(pBlock != NULL);
		CRect blockRegion = blocksPosition.GetBlockRegion(CBlockIndex(nBlockRowIndex, nBlockColumnIndex));
		pBlock->CalculateDrawingRegion( pDC, blockRegion, m_vertScaleRegionWidth);
	}
}

CScaleBaseData::SCALEGRIDTYPE CBlocksColumn::GetFirstBlockVGridLineType( void)
{
	CGraphSearchInfo_ActiveVertScaleAndGridLine gsi;
	if( FindGraph( &gsi)) if( gsi.m_pVertScaleGroup) return gsi.m_pVertScaleGroup->GetVertScale().GetGridType();

	CGraphSearchInfo_VisibleSubGraph gsi2;
	if( FindGraph( &gsi2)) if( gsi2.m_pVertScaleGroup) return gsi2.m_pVertScaleGroup->GetVertScale().GetGridType();

	return CScaleBaseData::SHORTDOTTEDLINE;
}

// 해당 RQ만 작업 : 복수종목 - ojtaso (20070420)
void CBlocksColumn::GetColumnViewData(LPCTSTR lpszRQ, CColumnViewData& columnViewData) const
{
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlockImp* pBlock = m_BlockList.GetNext(pos);
		assert(pBlock != NULL);
		CRowViewData rowViewData;
		rowViewData.SetHorzScaleName(pBlock->GetBlockData().GetHorzScale().GetScaleData());
		pBlock->GetVertScaleGroupList().GetRowViewData(lpszRQ, rowViewData);

		if(rowViewData.GetSubGraphViewDataCount())
			columnViewData.AddTail(rowViewData);
	}
}

int CBlocksColumn::GetBlockCount( void) const
{
	return m_BlockList.GetCount();
}

// scale 관련부분 변경
void CBlocksColumn::SetAllScaleGridType(const CScaleBaseData::SCALEGRIDTYPE eGridType)
{
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlockImp* pBlock = (CBlockImp*) m_BlockList.GetNext(pos);
		assert(pBlock != NULL);
		pBlock->SetAllScaleGridType(eGridType);
	}
}

void CBlocksColumn::SetAllHorzScaleGridType(const CScaleBaseData::SCALEGRIDTYPE eGridType)
{
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlockImp* pBlock = (CBlockImp*) m_BlockList.GetNext(pos);
		assert(pBlock != NULL);
		pBlock->GetBlockData().GetHorzScale().SetGridType(eGridType);
	}
}

void CBlocksColumn::SetAllHorzScalePosition(const CScaleBaseData::HORZSCALEPOSITION ePosition)
{
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlockImp* pBlock = (CBlockImp*) m_BlockList.GetNext(pos);
		assert(pBlock != NULL);

		// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
		//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
		m_pMain->SetHorzScalePosition( pBlock->GetBlockData().GetHorzScale(), ePosition);
	}
}

void CBlocksColumn::GetHorzScalePtrDataList(const CBlockImp* pCurrentBlock, CHorizontalScalePtrDataList& horzScalePtrDataList) const
{
	if(m_BlockList.GetCount() >= 0)
		horzScalePtrDataList.SetFirstIndex(0);

	ML_SET_LANGUAGE_RES();
	CString strFormat;
	strFormat.LoadString( IDS_FORMAT_BLOCK_SCALE);

	int nIndex = 0;
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlockImp* pBlock = m_BlockList.GetNext(pos);
		assert(pBlock != NULL);
		
		CString strTitle;
		strTitle.Format( strFormat, nIndex +1);
		horzScalePtrDataList.AddTail(CHorizontalScalePtrData(&pBlock->GetBlockData().GetHorzScale(), strTitle));
		if(pBlock == pCurrentBlock)
			horzScalePtrDataList.SetFirstIndex(nIndex);
		nIndex++;
	}
}

void CBlocksColumn::SetAllVertScaleGridType(const CScaleBaseData::SCALEGRIDTYPE eGridType)
{
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlockImp* pBlock = (CBlockImp*) m_BlockList.GetNext(pos);
		assert(pBlock != NULL);
		pBlock->GetVertScaleGroupList().SetScaleGridType(eGridType);
	}
}

void CBlocksColumn::SetAllVertScalePosition(const CScaleBaseData::VERTSCALEPOSITION ePosition)
{
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlockImp* pBlock = (CBlockImp*) m_BlockList.GetNext(pos);
		assert(pBlock != NULL);
		pBlock->GetVertScaleGroupList().SetAllVertScalePosition(ePosition);
	}
}

bool CBlocksColumn::SetAllVertScaleLogType(const bool bIsLogType)
{
	if(m_BlockList.GetCount() <= 0)
		return false;

	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlockImp* pBlock = m_BlockList.GetNext(pos);
		assert(pBlock != NULL);
		pBlock->GetBlockData().SetVertScaleLogType(bIsLogType);
	}
	return true;
}

// 가로/세로 scale을 셋팅한다.
void CBlocksColumn::SetMaintainOldScale(CBlockImp* pBlock)
{
	if(pBlock == NULL) return;

	// 기본이 되는 Block를 가져오기.
	CBlockImp* pBaseBlock = NULL;
	CGraphSearchInfo_OtherBlock gsi;
	gsi.m_pBlockImp = pBlock;
	if( FindGraph( &gsi)) pBaseBlock = gsi.m_pBlockImp;

	// 가로 scale 값을 수정.
	CHorizontalScale horzScale;
	if(pBaseBlock != NULL) horzScale = pBaseBlock->GetBlockData().GetHorzScale();

	// scale data의 기본 : <자료일자>
	if(horzScale.GetScaleData().IsEmpty()) horzScale.SetData( _MTEXT( C0_DATE_TIME));

	// scale position -> block 1개      : <HORZ_BOTTOM>
	//                -> block 2개 이상 : <HORZ_HIDE>
	if(m_BlockList.GetCount() <= 1)
	{
		// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
		//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
		m_pMain->SetHorzScalePosition( horzScale, CScaleBaseData::HORZ_BOTTOM);
	}
	else
	{
		// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
		//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
		m_pMain->SetHorzScalePosition( horzScale, CScaleBaseData::HORZ_HIDE);
	}

	if( !m_BlockList.IsEmpty())
	{
		pBlock->GetBlockData().SetHorzScale( horzScale);
		
		if( 2 <= m_BlockList.GetCount() && m_BlockList.GetTail() == pBlock)
		{
			// Block 의 갯수가 2개 이상일 경우 가로 Scale의 위치를 수정한다.
			CBlockImp* pPrevEndBlock = GetBlock(m_BlockList.GetCount() - 2);
			if( pPrevEndBlock)
			{
				// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
				//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
				m_pMain->SetHorzScalePosition( pPrevEndBlock->GetBlockData().GetHorzScale(), CScaleBaseData::HORZ_HIDE);
				m_pMain->SetHorzScalePosition( pBlock->GetBlockData().GetHorzScale(), CScaleBaseData::HORZ_BOTTOM);
			}
		}
	}


	// 세로 scale 값을 수정.
	CVerticalScale vertScale;
	if( pBaseBlock)
	{
		CVertScaleGroup *pVertScaleGroup = pBaseBlock->GetVertScaleGroupList().GetVertScaleGroup( 0);
		if( pVertScaleGroup) vertScale = pVertScaleGroup->GetVertScale();
	}

	// scale data의 기본 : <지표 Data>
	if(vertScale.GetScaleData().IsEmpty())	vertScale.SetData( _MTEXT( C6_INDICATOR_DATA));

	// scale position -> <VERT_RIGHT>
	if(m_BlockList.GetCount() <= 1)			vertScale.SetVertScalePosition(CScaleBaseData::VERT_RIGHT);
	
	// (2007/7/4 - Seung-Won, Bae) Clear Y Scale Min/Max Bound Setting
	vertScale.SetVertScaleMinMaxType( CScaleBaseData::VERT_MINMAX_OF_VIEW);
	vertScale.SetVertScaleMinMaxLock( FALSE);

	if( pBlock->GetVertScaleGroupList().GetCount() <= 0) pBlock->GetVertScaleGroupList().MakeVertScaleGroups_Empty();
	CVertScaleGroup* pVertScaleGroup = pBlock->GetVertScaleGroupList().GetVertScaleGroup( 0);
	if( pVertScaleGroup) pVertScaleGroup->SetVerticalScale(vertScale);
}

bool CBlocksColumn::GetAllIndicatorName(CList<CString, CString>& strIndicatorNameList) const
{
	bool bResult = false;
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlockImp* pBlock = m_BlockList.GetNext(pos);
		assert(pBlock != NULL);
		if(pBlock->GetVertScaleGroupList().GetAllIndicatorName(strIndicatorNameList))
			bResult = true;
	}
	return bResult;
}

bool CBlocksColumn::GetAllGraphName(CList<CString, CString>& strGraphNameList) const
{
	bool bResult = false;
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlockImp* pBlock = m_BlockList.GetNext(pos);
		assert(pBlock != NULL);
		if(pBlock->GetVertScaleGroupList().GetAllGraphName(strGraphNameList))
			bResult = true;
	}
	return bResult;
}

// strPacketName 가 포함된 모든 subGraph의 packet들을 초기화
bool CBlocksColumn::Initialize_Packet(const CString& strPacketName, const bool bIsCalculate)
{
	bool bResult = false;
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlockImp* pBlock = m_BlockList.GetNext(pos);
		assert(pBlock != NULL);
		if(pBlock->GetVertScaleGroupList().Initialize_Packet(strPacketName, bIsCalculate))
			bResult = true;
	}
	return bResult;
}

// graph의 indicatorinfo를 초기화(NULL)
void CBlocksColumn::Initialize_IndicatorInfo(const bool bIsNULL)
{
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlockImp* pBlock = m_BlockList.GetNext(pos);
		assert(pBlock != NULL);
		pBlock->GetVertScaleGroupList().Initialize_IndicatorInfo(bIsNULL);
	}
}

// Change Input Packet Names and Calculate again.
BOOL CBlocksColumn::ChangeIndicatorPacketNames( const char *p_szIndicatorName, const int p_nSubGraphIndex, const char *p_szPacketNames)
{
	if( !p_szIndicatorName || !p_szPacketNames) return FALSE;
	if( !*p_szIndicatorName || !*p_szPacketNames) return FALSE;

	BOOL bResult = FALSE, bResult2 = FALSE;
	CBlockImp *pBlock = NULL;
	POSITION pos = m_BlockList.GetHeadPosition();
	while( pos)
	{
		pBlock = m_BlockList.GetNext(pos);
		if( !pBlock) continue;
		bResult2 = pBlock->GetVertScaleGroupList().ChangeIndicatorPacketNames( p_szIndicatorName, p_nSubGraphIndex, p_szPacketNames);
		bResult = bResult || bResult2;
	}
	return bResult;
}

// (2007/2/20 - Seung-Won, Bae) Support Indicator Hiding
//		It can not be replaced with AddChartBlock(), because of Condition Reset.
void CBlocksColumn::ShowIndicator( const char *p_szIndicatorName, BOOL p_bShow, BOOL p_bRecalculate)
{
	if( !p_szIndicatorName) return;
	if( !*p_szIndicatorName) return;

	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlockImp* pBlock = m_BlockList.GetNext(pos);
		assert(pBlock != NULL);
		pBlock->GetVertScaleGroupList().ShowIndicator( p_szIndicatorName, p_bShow, p_bRecalculate);
	}
}

// -> IndicatorInfo를 gaphData(subGraph포함)로 변경.
bool CBlocksColumn::ChangeAllIndicatorInfoFromGraphData()
{
	bool bResult = false;
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlockImp* pBlock = m_BlockList.GetNext(pos);
		assert(pBlock != NULL);
		if(pBlock->GetVertScaleGroupList().ChangeAllIndicatorInfoFromGraphData())
			bResult = true;
	}
	return bResult;
}

BOOL CBlocksColumn::ChangeAllGraphDataFromIndicatorInfo( BOOL p_bWithCalculate)
{
	BOOL bResult = FALSE;
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlockImp* pBlock = m_BlockList.GetNext(pos);
		assert(pBlock != NULL);
		if(pBlock->GetVertScaleGroupList().ChangeAllGraphDataFromIndicatorInfo( p_bWithCalculate))
			bResult = TRUE;
	}
	return bResult;
}

// -> 해당 graph를 Map Default IndicatorInfo로 변경
bool CBlocksColumn::ChangeAllMapDefaultIndicatorInfo(CIndicatorList* pIndicatorList)
{
	bool bResult = false;
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlockImp* pBlock = m_BlockList.GetNext(pos);
		assert(pBlock != NULL);
		if(pBlock->GetVertScaleGroupList().ChangeAllMapDefaultIndicatorInfo(pIndicatorList))
			bResult = true;
	}
	return bResult;
}

//////////////////////////////////////////////////////////////////////
// Notify
//////////////////////////////////////////////////////////////////////

// block 추가/이동시 ocx에 SendMessage.
void CBlocksColumn::SendMessageToParent_BlockInsert(const int nSourceBlockIndex)
{
	if(m_pMain == NULL) return;

	CGraphSearchInfo_BlocksColumn gsi;
	gsi.m_pBlocksColumn = this;
	if( !m_pMain->FindGraph( &gsi)) return;

	CBlockBehaviorData blockBehaviorData( CBlockBaseData::BLOCK_INSERT, CBlockIndex(nSourceBlockIndex, gsi.m_nColumnBlockIndex));
	GetMainBlockBaseData()->SendMessageToParent(blockBehaviorData);
}

//////////////////////////////////////////////////////////////////////
// Calculate Indicator
//////////////////////////////////////////////////////////////////////

// 지표를 계산한다.
void CBlocksColumn::CalculateAllGraphs(const CCalculateBaseData::CALCULATETYPE eCalType)
{
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlockImp* pBlock = m_BlockList.GetNext(pos);
		pBlock->GetVertScaleGroupList().CalculateAllGraphs(eCalType);
	}
}

bool CBlocksColumn::CalculateAllGraphs(const CList<CString, CString>& packetNameList, const CCalculateBaseData::CALCULATETYPE eCalType, const bool bIsHeadGraph)
{
	bool bResult = false;
	POSITION pos = m_BlockList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlockImp* pBlock = m_BlockList.GetNext(pos);
		if(!pBlock->GetVertScaleGroupList().CalculateAllGraphs(packetNameList, eCalType, bIsHeadGraph))
			continue;

		bResult = true;
		if(bIsHeadGraph)
			return true;
	}
	return bResult;
}


// (2009/2/13 - Seung-Won, Bae) for Multi-Column.
int CBlocksColumn::GetColumnIndex( void)
{
	if( !m_pMain) return -1;

	CGraphSearchInfo_BlocksColumn gsi;
	gsi.m_pBlocksColumn = this;
	if( !m_pMain->FindGraph( &gsi)) return -1;

	return gsi.m_nColumnBlockIndex;
}


// (2009/6/25 - Seung-Won, Bae) for Balanced Row Arrange without scale.
void CBlocksColumn::GetSumOfHorzScaleHeightForEachBlock( CList< int, int> &p_lnSumOfHScaleHeight)
{
	POSITION posScaleHeight = p_lnSumOfHScaleHeight.GetHeadPosition();
	POSITION posBlock = m_BlockList.GetHeadPosition();
	while( posBlock)
	{
		CBlockImp* pBlock = m_BlockList.GetNext( posBlock);
		if( !pBlock) continue;
		CHorizontalScale &horizontalScale = pBlock->GetBlockData().GetHorzScale();
		int nScaleCount = 0;
		switch( horizontalScale.GetHorzScalePosition())
		{
			case CScaleBaseData::HORZ_TOP:
			case CScaleBaseData::HORZ_BOTTOM:	nScaleCount = 1;
												break;
			case CScaleBaseData::HORZ_BOTH:		nScaleCount = 2;
												break;
		}

		int nNewScaleHeight = horizontalScale.GetScaleHeight();
		if( nNewScaleHeight < 0) nNewScaleHeight = GetMainBlockBaseData()->GetScaleAttributes().GetMinHorzScaleRegionHeight();
		nNewScaleHeight = nScaleCount * nNewScaleHeight;
		if( posScaleHeight)
		{
			if( p_lnSumOfHScaleHeight.GetAt( posScaleHeight) < nNewScaleHeight)
				p_lnSumOfHScaleHeight.SetAt( posScaleHeight, nNewScaleHeight);
			p_lnSumOfHScaleHeight.GetNext( posScaleHeight);
		}
		else p_lnSumOfHScaleHeight.AddTail( nNewScaleHeight);
	}
}

// (2009/10/2 - Seung-Won, Bae) Get Horizontal Scale RQ for X Scale of each block with different time scale.
CString	CBlocksColumn::GetHorzScaleDataRQ( CBlockImp *pBlock)
{
	if( !pBlock) return "DEFAULT";

	POSITION psnBlock = m_BlockList.Find( pBlock);
	if( !psnBlock) return "DEFAULT";

	CStringList slRQs;

	// 1. check current block price chart.
	CGraphSearchInfo_IndicatorName gsi;
	gsi.m_strIndicatorName = _MTEXT( C2_PRICE_CHART);
	while( pBlock->GetVertScaleGroupList().FindGraph( &gsi))
	{
		if( gsi.m_pGraphImp) slRQs.AddTail( gsi.m_pGraphImp->GetGraphRQ());
		gsi.SetNextStartGraph();
	}
	CScaleBaseData::HORZSCALEPOSITION eHorzScalePos = pBlock->GetBlockData().GetHorzScale().GetHorzScalePosition();

	// 2. check upper block price chart.
	POSITION psnUpBlock = psnBlock;
	m_BlockList.GetPrev( psnUpBlock);
	CScaleBaseData::HORZSCALEPOSITION eUpHorzScalePos = eHorzScalePos;
	while( psnUpBlock && CScaleBaseData::HORZ_TOP != eUpHorzScalePos)
	{
		CBlockImp *pUpBlock = m_BlockList.GetPrev( psnUpBlock);
		if( !pUpBlock) continue;
		eUpHorzScalePos = pUpBlock->GetBlockData().GetHorzScale().GetHorzScalePosition();
		if( CScaleBaseData::HORZ_BOTTOM == eUpHorzScalePos
			|| CScaleBaseData::HORZ_BOTH == eUpHorzScalePos)
			break;
		
		gsi.ResetInfo();
		while( pUpBlock->GetVertScaleGroupList().FindGraph( &gsi))
		{
			if( gsi.m_pGraphImp) slRQs.AddTail( gsi.m_pGraphImp->GetGraphRQ());
			gsi.SetNextStartGraph();
		}
	}

	// 3. check down block price chart.
	POSITION psnDownBlock = psnBlock;
	m_BlockList.GetNext( psnDownBlock);
	CScaleBaseData::HORZSCALEPOSITION eDownHorzScalePos = eHorzScalePos;
	while( psnDownBlock && CScaleBaseData::HORZ_BOTTOM != eDownHorzScalePos)
	{
		CBlockImp *pDownBlock = m_BlockList.GetNext( psnDownBlock);
		if( !pDownBlock) continue;
		eDownHorzScalePos = pDownBlock->GetBlockData().GetHorzScale().GetHorzScalePosition();
		if( CScaleBaseData::HORZ_TOP == eDownHorzScalePos
			|| CScaleBaseData::HORZ_BOTH == eDownHorzScalePos)
			break;
		
		gsi.ResetInfo();
		while( pDownBlock->GetVertScaleGroupList().FindGraph( &gsi))
		{
			if( gsi.m_pGraphImp) slRQs.AddTail( gsi.m_pGraphImp->GetGraphRQ());
			gsi.SetNextStartGraph();
		}
	}

	CStringList &slSelectedRQs = m_pMain->GetRQHistory();
	POSITION psnRQs = slSelectedRQs.GetHeadPosition();
	while( psnRQs)
	{
		CString strRQ = slSelectedRQs.GetNext( psnRQs);
		if( slRQs.Find( strRQ)) return strRQ;
	}

	return "DEFAULT";
}