// BlocksPositionCalculator.cpp: implementation of the CBlocksPositionCalculator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlocksPositionCalculator.h"

#include "../Include_Chart/DataMath.h"
#include "../Include_Chart/Conversion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CBlocksPositionCalculator

int CBlocksPositionCalculator::m_nMouseClickThickness(2);
int CBlocksPositionCalculator::m_nMinBlockWidth(4);

// public =================================================================================
// 전체영역이 모든 block들의 최소영역보다 큰 영역인지를 구별. (gap과 구분선 굵기 포함)
// 전체영역 >= (block최소영역*block갯수 + (block갯수 -1)*(gap + 구분선 굵기))  
bool CBlocksPositionCalculator::IsBiggerThanMinMainRegion(const int blockCount, const int allBlocksRegion, const int interBlockGap, const int compartLineThickness)
{
	int minMainRegion = (m_nMinBlockWidth * blockCount) + ((blockCount -1)*(interBlockGap + compartLineThickness));
	return (allBlocksRegion > minMainRegion);
}

// rect2가 또다른 rect1에 포함 되는지를 계산
bool CBlocksPositionCalculator::IsRectInRect(const CRect& rect1, const CRect& rect2)
{
	if(!rect1.PtInRect(rect2.TopLeft()))
		return false;

	int bottom = rect2.bottom;
	if(rect2.bottom == rect1.bottom)
		bottom--;
	return rect1.PtInRect(CPoint(rect2.right, bottom)) ? true : false;
}

// ----------------------------------------------------------------------------------------
// list에 위치 저장.
void CBlocksPositionCalculator::Add_EqualityPartition( CList< double, double>& positionList, const int blockCount, 
			const int allRegionLength, const int interBlockHalfGap, const int compartLineThickness)
{
	int prePosition = 0;
	for(int i = 0; i <= blockCount; i++)
	{
		int length = GetPositionLength(prePosition, allRegionLength, i, blockCount, interBlockHalfGap, compartLineThickness);
		prePosition += length;
		positionList.AddTail(prePosition);
	}
}

void CBlocksPositionCalculator::Add_DoubleSize( CList< double, double>& positionList, const int blockCount, 
			const int doubleSizeBlockNum, const int allRegionLength, const int interBlockHalfGap, const int compartLineThickness)
{
	int doubleSizeCount = GetDoubleSizeBlockCount(blockCount, doubleSizeBlockNum);
	int realBlockCount = blockCount + doubleSizeCount;
	int prePosition = 0;

	for(int i = 0; i <= realBlockCount; i++){
		int length = GetPositionLength(prePosition, allRegionLength, 
					i, realBlockCount, interBlockHalfGap, compartLineThickness);
		prePosition += length;
		if(!IsBlockInDoubleSize(doubleSizeBlockNum, doubleSizeCount, i))
			positionList.AddTail(prePosition);
	}
}

// ----------------------------------------------------------------------------------------
// string로 넘어온 block들의 위치를 위치 list에 setting.
void CBlocksPositionCalculator::SetBlocksPosition( CList< double, double>& positionList, const int maxExtent, const CString& positionData)
{
	// position이 없을 경우.
	if(positionList.GetCount() <= 0){
		GetBlocksPosition(positionData, positionList);
		return;
	}

	CList< double, double> newPositionList;
	if(!GetBlocksPosition(positionData, newPositionList))
		return;

	if(newPositionList.GetTail() == maxExtent){
		SetBlocksPosition(positionList, newPositionList);
		return;
	}

	// 현재 저장된 size와 새로 변경할 size 가 틀린 경우.
	//	-> ocx의 size가 변경되는 경우. 
	//	-> 서로다른 Map에서 같은 user file을 참조할 경우.
	// string 로 들어오는 마지막 position이 틀릴 경우
	// string 로 된 position으로 변경 후 maxExtent으로 비율을 조절한다.
	int nOldAllRegionLength = ( int)newPositionList.GetTail();
	int nNewAllRegionLength = ( int)positionList.GetTail();
	if(SetBlocksPosition(positionList, newPositionList))
		RecalculatePositions(positionList, nOldAllRegionLength, nNewAllRegionLength);
}

// ----------------------------------------------------------------------------------------
// block들의 위치(m_lRow, m_lColumn)를 CString로 변환.
CString CBlocksPositionCalculator::GetSavedPosition( const CList< double, double>& positionList, const CString& positionKindName)
{
	POSITION pos = positionList.GetHeadPosition();
	if(pos == NULL)
		return (positionKindName + "\r\n");

	CString blocksPosition = positionKindName;
	for(int i = 0; i < positionList.GetCount(); i++){
		int data = (int)positionList.GetNext(pos);
		CString temp;
		temp.Format("%d;", data);
		blocksPosition += temp;
	}
	return (blocksPosition + "\r\n");
}

// ----------------------------------------------------------------------------------------
// nIndex번째 block의 범위를 가져온다. (Gap이 포함안된 영역) => 절대좌표
void CBlocksPositionCalculator::GetBlockRegion(const int absoluteOrgPt, const int interBlockHalfGap,
		const int compartLineThickness, const CList< double, double>& positionList, const int nIndex, double &x1, double &x2)
{
	// (2006/10/8 - Seung-Won, Bae) Make a simple logic.
	// Get Start Position
	x1 = GetAbsolutePosition(absoluteOrgPt, positionList, nIndex);
	if( 0 < nIndex) x1 += interBlockHalfGap + compartLineThickness;

	// Get End Position
	x2 = GetAbsolutePosition(absoluteOrgPt, positionList, nIndex + 1);
	if( positionList.GetCount() - 1 != nIndex + 1) x2 -= interBlockHalfGap;
/*
	if(nIndex == 0 && positionList.GetCount() == 2){ // 2개의 블럭중에 첫번째 블럭 
		x1 = GetAbsolutePosition(absoluteOrgPt, positionList, nIndex);
		x2 = GetAbsolutePosition(absoluteOrgPt, positionList, nIndex +1);
	}
	else if(nIndex == 0){ // 2개의 블럭중에 마지막블럭
		x1 = GetAbsolutePosition(absoluteOrgPt, positionList, nIndex);
		x2 = GetAbsolutePosition(absoluteOrgPt, positionList, nIndex +1) - (interBlockHalfGap + compartLineThickness);
	}
	else if(nIndex == positionList.GetCount() -1){ // 마지막블럭
		x1 = GetAbsolutePosition(absoluteOrgPt, positionList, nIndex) + (interBlockHalfGap + compartLineThickness);
		x2 = GetAbsolutePosition(absoluteOrgPt, positionList, nIndex +1);
	}
	else{ // 중간블럭
		x1 = GetAbsolutePosition(absoluteOrgPt, positionList, nIndex) + (interBlockHalfGap + compartLineThickness);
		x2 = GetAbsolutePosition(absoluteOrgPt, positionList, nIndex +1) - (interBlockHalfGap + compartLineThickness);
	}
*/
}

// list의 위치에 따른 position의 길이를 구한다. 
// --> (startPosition, endPosition) 를 상대좌표를 변경하여 길이 구함.
int CBlocksPositionCalculator::GetPositionLength(const int startPosition, const int endPosition, 
		const int posNum, const int blockCount, const int interBlockHalfGap, const int compartLineThickness)
{
	// list count == 0 
	//		--> list 값 == 0
	// list count == blockCount : list의 마지막 위치
	//		--> list 값 == 전체 block 영역의 길이
	// list count == 1 :  처음 block에 해당되는 끝 영역(처음 영역 : 0)
	//		--> gap : 1개, 구분선 : 0개 를 이용해 계산
	// 나머지는 gap 
	//		--> 2개, 구분선 : 1개 를 이용해 계산

	if(posNum == 0)
		return 0;
	else if(posNum == blockCount)
		return (endPosition - startPosition);
	else if(posNum == 1)
		return (GetBlockLength(startPosition, endPosition, blockCount, (blockCount-posNum)*2, (blockCount-1), interBlockHalfGap, compartLineThickness) + interBlockHalfGap);

	return (GetBlockLength(startPosition, endPosition, blockCount-posNum+1, (blockCount-posNum)*2+1, blockCount-posNum+1, interBlockHalfGap, compartLineThickness) + interBlockHalfGap*2 +compartLineThickness);
}

// 구분선을 그리기위해서 해당 block의 위치를 가져온다. => 절대좌표
CPoint CBlocksPositionCalculator::GetAbsolutePoint(const CPoint& absoluteOrgPt, const CList< double, double>& rowList, const CList< double, double>& columnList, const int xIndex, const int yIndex)
{
	return CPoint( ( int)GetAbsolutePosition(absoluteOrgPt.x, columnList, xIndex), ( int)GetAbsolutePosition(absoluteOrgPt.y, rowList, yIndex));
}

// ----------------------------------------------------------------------------------------
// 이미 생성된 모든 block의 위치를 초기화한다. => 모든 block 위치를 균등으로 수정
void CBlocksPositionCalculator::InitializePositions(const CRect& newAllBlockRegion, 
		const int nDoubleSizeBlockRowNum, const int nDoubleSizeBlockColumnNum, 
		CList<int, int>& rowList, CList<int, int>& columnList, const int interBlockHalfGap, const int compartLineThickness)
{
	InitializePositions(rowList, nDoubleSizeBlockRowNum, newAllBlockRegion.bottom - newAllBlockRegion.top, interBlockHalfGap, compartLineThickness);
	InitializePositions(columnList, nDoubleSizeBlockColumnNum, newAllBlockRegion.right - newAllBlockRegion.left, interBlockHalfGap, compartLineThickness);
}

// 모든 block 기존위치에서 변경된 영역에 의해 수정한다. => 기존 block의 비율을 생각하여 수정
void CBlocksPositionCalculator::RecalculatePositions(const CRect& oldAllBlockRegion, const CRect& newAllBlockRegion, CList< double, double>& rowList, CList< double, double>& columnList)
{
	if(oldAllBlockRegion.Height() != newAllBlockRegion.Height())
		RecalculatePositions(rowList, (oldAllBlockRegion.bottom - oldAllBlockRegion.top), (newAllBlockRegion.bottom - newAllBlockRegion.top));
	if(oldAllBlockRegion.Width() != newAllBlockRegion.Width())
		RecalculatePositions(columnList, (oldAllBlockRegion.right - oldAllBlockRegion.left), (newAllBlockRegion.right - newAllBlockRegion.left));
}

// ----------------------------------------------------------------------------------------
// 마우스 이벤트에 의해 호출되는 함수들..
// point가 row, column에 해당되는 영역에 적합한 지를 찾는다.
bool CBlocksPositionCalculator::FindCompartLine(const CPoint& absoluteOrgPt, const CList< double, double>& rowList, const CList< double, double>& columnList, 
		const CPoint& point, int& blockRowIndex, int& blockColumnIndex)
{
	bool ret = false;
	// 가로(세로) 구분선 체크
	if(AppropriateRowCompartLine(absoluteOrgPt, rowList, columnList, point, blockRowIndex))
		ret = true;
	if(AppropriateColumnCompartLine(absoluteOrgPt, rowList, columnList, point, blockColumnIndex))
		ret = true;

	return ret;
}

// 구분선의 선택된 경우 point 위치로 position 변경
bool CBlocksPositionCalculator::MoveBlocksPosition( CList< double, double>& rowList, CList< double, double>& columnList, const CPoint& point, const int rowCompartLineNum, const int columnCompartLineNum)
{
	bool ret = false;
	if(MoveBlocksPosition(columnList, columnCompartLineNum, point.x))
		ret = true;
	if(MoveBlocksPosition(rowList, rowCompartLineNum, point.y))
		ret = true;

	return ret;
}

// block 위치 변경
bool CBlocksPositionCalculator::MoveBlockPosition( CList< double, double>& positionList, const int sourceIndex, const int targetIndex)
{
	int allCount = positionList.GetCount();
	if(!IsMoveBlockPosition(sourceIndex, allCount) || !IsMoveBlockPosition(targetIndex, allCount))
		return false;

	// position 간의 길이
	CList< double, double> lengthInBlock;
	if(!GetChangedLengthInBlock(positionList, sourceIndex, targetIndex, lengthInBlock))
		return false;

	// 변경된 길이를 block position list에 변경
	double dSetData = 0;
	for(int i = 1; i < allCount; i++){
		POSITION pos = positionList.FindIndex(i);
		if(pos != NULL){
			dSetData += GetData(lengthInBlock, i -1);
			positionList.SetAt(pos, dSetData);
		}
	}
	return true;
}


// private ===============================================================================
int CBlocksPositionCalculator::GetDoubleSizeBlockCount(const int blockCount, const int doubleSizeBlockNum)
{
	if(doubleSizeBlockNum < 0 || doubleSizeBlockNum >= blockCount)
		return 0;

	return (blockCount > 2 ? 2: 1);
}

bool CBlocksPositionCalculator::IsBlockInDoubleSize(const int doubleSizeBlockNum, const int doubleSizeCount, const int curNum)
{
	return (curNum > doubleSizeBlockNum && curNum <= (doubleSizeBlockNum + doubleSizeCount));
}

// ---------------------------------------------------------------------------------------
bool CBlocksPositionCalculator::GetBlocksPosition(const CString& strPositon, CList<double, double>& positionList)
{
	positionList.RemoveAll();

	int nCount = CDataConversion::GetStringDataCount(strPositon, ";");
	for(int i = 0; i < nCount ; i++){
		CString strData = CDataConversion::GetMidToTrimDataIndex(strPositon, ";", i);
		if(strData.IsEmpty())
			return false;
		int nData = atoi(strData);
		positionList.AddTail(nData);
	}

	return (positionList.GetCount() > 0);
}

bool CBlocksPositionCalculator::SetBlocksPosition(CList< double, double>& targetList, const CList< double, double>& sourceList)
{
	if(sourceList.GetCount() <= 0 || targetList.GetCount() != sourceList.GetCount())
		return false;

	POSITION targetPos = targetList.GetHeadPosition();
	POSITION sourcePos = sourceList.GetHeadPosition();
	while(targetPos != NULL && sourcePos != NULL){
		double dData = sourceList.GetAt(sourcePos);
		targetList.SetAt(targetPos, dData);

		targetList.GetNext(targetPos);
		sourceList.GetNext(sourcePos);
	}

	return true;
}

// =--------------------------------------------------------------------------------------
// nIndex번째 위치 (Gap이 포함된 영역)
// RelativeToAbsolute
double CBlocksPositionCalculator::GetAbsolutePosition( const double absoluteOrgPt, const CList< double, double>& positionList, const int nIndex)
{
	POSITION pos = positionList.FindIndex(nIndex);
	if(pos != NULL)
		return (positionList.GetAt(pos) + absoluteOrgPt);
	return absoluteOrgPt;
}


// block의 길이를 구한다.
int CBlocksPositionCalculator::GetBlockLength(const int start, const int end, const int blockCount, const int gapCount, 
		const int compartLineCount, const int interBlockHalfGap, const int compartLineThickness)
{
	return CMath::Round((double)((end - start) - interBlockHalfGap*gapCount - compartLineCount*compartLineThickness) / (double)blockCount);
}

// ---------------------------------------------------------------------------------------
// 이미 생성된 모든 block의 위치를 초기화한다. => 모든 block 위치를 균등으로 수정
void CBlocksPositionCalculator::InitializePositions(CList<int, int>& positionList, const int nDoubleSizeBlockNum, 
		const int allRegionLength, const int interBlockHalfGap, const int compartLineThickness)
{
	POSITION pos = positionList.GetHeadPosition();
	if(pos == NULL)
		return;

	int nBlockCount = positionList.GetCount() -1;
	int doubleSizeCount = GetDoubleSizeBlockCount(nBlockCount, nDoubleSizeBlockNum);
	int realBlockCount = nBlockCount + doubleSizeCount;
	int prePosition = 0, posNum = 0;
	while(pos != NULL){
		int length = GetPositionLength(prePosition, allRegionLength, posNum, realBlockCount, interBlockHalfGap, compartLineThickness);
		prePosition += length;
		if(IsBlockInDoubleSize(nDoubleSizeBlockNum, doubleSizeCount, posNum++))
			continue;
		positionList.SetAt(pos, prePosition);
		positionList.GetNext(pos);
	}
}

// 모든 block 기존위치에서 변경된 영역에 의해 수정한다.
void CBlocksPositionCalculator::RecalculatePositions( CList< double, double>& positionList, const int oldAllRegionLength, const int newAllRegionLength)
{
	POSITION pos = positionList.GetHeadPosition();
	if(pos == NULL)
		return;

	int posNum = 0;
	while(pos != NULL){
		int length = GetChangedPosition(oldAllRegionLength, newAllRegionLength, 
			(int)positionList.GetAt(pos), posNum++, (positionList.GetCount()-1));
		positionList.SetAt(pos, length);
		positionList.GetNext(pos);
	}
}

// 새 영역에 의해 기존의 영역의 비율로 변경된 값을 가져온다.
int CBlocksPositionCalculator::GetChangedPosition(const int oldAllRegionLength, const int newAllRegionLength, 
			const int oldPositionValue, const int posNum, const int blockCount)
{
	// list count == 0 
	//		--> 원래의 값
	// 나머지 --> 비율로 계산

	if(posNum == 0)
		return oldPositionValue;
	else if(posNum == blockCount)
		return newAllRegionLength;

	if(oldAllRegionLength == 0)
		return oldPositionValue;

	return CMath::Round(((double)newAllRegionLength*(double)oldPositionValue)/(double)oldAllRegionLength);
}

// ---------------------------------------------------------------------------------------
// 해당 point가 가로 구분선에 해당되는지를 구한다.
bool CBlocksPositionCalculator::AppropriateRowCompartLine(const CPoint& absoluteOrgPt, const CList< double, double>& rowList, const CList< double, double>& columnList, 
		const CPoint& point, int& rowCompartLineNum)
{
	for(int row = 1; row < rowList.GetCount() -1; row++){
		CPoint start( ( int)GetAbsolutePosition(absoluteOrgPt.x, columnList, 0), 
				( int)GetAbsolutePosition(absoluteOrgPt.y, rowList, row) - m_nMouseClickThickness);
		CPoint end( ( int)GetAbsolutePosition(absoluteOrgPt.x, columnList, columnList.GetCount()-1), 
				( int)GetAbsolutePosition(absoluteOrgPt.y, rowList, row) +m_nMouseClickThickness);
		if(IsPtInRect(CRect(start, end), point)){
			rowCompartLineNum = row;
			return true;
		}
	}
	return false;
}

// 해당 point가 세로 구분선에 해당되는지를 구한다.
bool CBlocksPositionCalculator::AppropriateColumnCompartLine(const CPoint& absoluteOrgPt, const CList< double, double>& rowList, const CList< double, double>& columnList, 
		const CPoint& point, int& columnCompartLineNum)
{
	for(int column = 1; column < columnList.GetCount() -1; column++){
		CPoint start( ( int)GetAbsolutePosition(absoluteOrgPt.x, columnList, column) - m_nMouseClickThickness, 
				( int)GetAbsolutePosition(absoluteOrgPt.y, rowList, 0));
		CPoint end( ( int)GetAbsolutePosition(absoluteOrgPt.x, columnList, column) + m_nMouseClickThickness, 
				( int)GetAbsolutePosition(absoluteOrgPt.y, rowList, rowList.GetCount()-1));
		if(IsPtInRect(CRect(start, end), point)){
			columnCompartLineNum = column;
			return true;
		}
	}
	return false;
}

// point가 rect안에 들어있는지를 체크
bool CBlocksPositionCalculator::IsPtInRect(const CRect& rect, const CPoint& point)
{
	return (rect.PtInRect(point) != 0) ? true: false;
}

// ---------------------------------------------------------------------------------------
// 마우스로 선택한 위치(block의 위치가 들어있는 리스트)에 값(위치)을 변경
/*bool CBlocksPositionCalculator::MoveBlocksPosition(CList<int, int>& positionList, const int compartLineNum, const int modityData)
{	
	// 0 위치는 시작 위치기 때문에 구분선이 없다!!
	if(compartLineNum <= 0 || compartLineNum >= positionList.GetCount() -1)
		return false;

	POSITION pos = positionList.FindIndex(compartLineNum);
	if(pos == NULL)
		return false;

	int data = positionList.GetAt(pos) +modityData;
	if(data > positionList.GetAt(positionList.FindIndex(compartLineNum -1)) + m_nMinBlockWidth &&
		data < positionList.GetAt(positionList.FindIndex(compartLineNum +1)) - m_nMinBlockWidth){
		positionList.SetAt(pos, data);
		return true;
	}

	return false;
}*/

bool CBlocksPositionCalculator::MoveBlocksPosition( CList< double, double>& positionList, const int compartLineNum, const int modityData)
{	
	// 0 위치는 시작 위치기 때문에 구분선이 없다!!
	if(compartLineNum <= 0 || compartLineNum >= positionList.GetCount() -1)
		return false;

	POSITION pos = positionList.FindIndex(compartLineNum -1);
	if(pos == NULL)
		return false;

	double dPreData = positionList.GetNext( pos) + m_nMinBlockWidth;
	double dCurData = positionList.GetNext( pos) + modityData;
	double dNextData = positionList.GetNext( pos) - m_nMinBlockWidth;

	if( dCurData > dPreData && dCurData < dNextData){
		POSITION curPos = positionList.FindIndex(compartLineNum);
		positionList.SetAt(curPos, dCurData);
		return true;
	}

	return false;
}

// ----------------------------------------------------------------------------------------
bool CBlocksPositionCalculator::IsMoveBlockPosition(const int num, const int allCount)
{
	if(allCount <= 2)
		return false;

	return (num > -1 && num < allCount);
}

// position 간의 길이
bool CBlocksPositionCalculator::GetChangedLengthInBlock( const CList< double, double>& positionList, const int sourceIndex, const int targetIndex, CList< double, double>& lengthInBlock)
{
	// 기존의 position 간의 간격.
	for(int i = 1; i < positionList.GetCount(); i++){
		double data = GetData( positionList, i) - GetData( positionList, i -1);
		lengthInBlock.AddTail(data);
	}

	// 해당 position 변경
	return GetChangedLengthInBlock(sourceIndex, targetIndex, lengthInBlock);
}

bool CBlocksPositionCalculator::GetChangedLengthInBlock(const int sourceIndex, const int targetIndex, CList< double, double>& lengthInBlock)
{
	if(sourceIndex < 0 || targetIndex < 0 || lengthInBlock.GetCount() <= 0)
		return false;
	
	POSITION sourcePos = lengthInBlock.FindIndex(sourceIndex);
	POSITION targetPos = lengthInBlock.FindIndex(targetIndex);
	if(sourcePos == NULL || targetPos == NULL)
		return false;
	
	double data = lengthInBlock.GetAt(sourcePos);
	if(sourceIndex > targetIndex)
		lengthInBlock.InsertBefore(targetPos, data);
	else
		lengthInBlock.InsertAfter(targetPos, data);

	lengthInBlock.RemoveAt(sourcePos);
	return true;
}

// 해당 index에 값
double CBlocksPositionCalculator::GetData(const CList< double, double>& positionList, const int index)
{
	POSITION pos = positionList.FindIndex(index);
	if(pos == NULL)
		return 0;
	return positionList.GetAt(pos);
}

// ----------------------------------------------------------------------------------------
