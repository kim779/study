// BlocksPositionCalculator.h: interface for the CBlocksPositionCalculator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCKSPOSITIONCALCULATOR_H__C2233779_3A04_4B80_A18A_C9ECCBAF4E45__INCLUDED_)
#define AFX_BLOCKSPOSITIONCALCULATOR_H__C2233779_3A04_4B80_A18A_C9ECCBAF4E45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CBlocksPositionCalculator
{
public:
	// 전체영역이 모든 block들의 최소영역보다 큰 영역인지를 구별. (gap과 구분선 굵기 포함)
	// 전체영역 >= (block영역*block갯수 + (block갯수 -1)*(gap + 구분선 굵기))  
	static bool IsBiggerThanMinMainRegion(const int blockCount, const int allBlocksRegion, const int interBlockGap, const int compartLineThickness);
	// rect2가 또다른 rect1에 포함 되는지를 계산
	static bool IsRectInRect(const CRect& rect1, const CRect& rect2);

	// position 저장
	static void Add_EqualityPartition( CList< double, double>& positionList, const int blockCount, 
			const int allRegionLength, const int interBlockHalfGap, const int compartLineThickness);
	static void Add_DoubleSize( CList< double, double>& positionList, const int blockCount, 
			const int doubleSizeBlockNum, const int allRegionLength, const int interBlockHalfGap, const int compartLineThickness);

	// block들의 위치를 다시 정한다. -> positionList : m_lRow, m_lColumn
	static void SetBlocksPosition( CList< double, double>& positionList, const int maxExtent, const CString& positionData);

	// m_lRow, m_lColumn의 값을 string로 변환
	static CString GetSavedPosition( const CList< double, double>& positionList, const CString& prePosition);
	// 해당 위치(Gap을 포함하지 않은 순수 block) 가져오기 => 절대좌표
	static void GetBlockRegion(const int absoluteOrgPt, const int interBlockHalfGap, 
		const int compartLineThickness, const CList< double, double>& positionList, const int nIndex, double &x1, double &x2);
	// list에 입력할 position의 길이을 가져온다.
	static int GetPositionLength(const int startPosition, const int endPosition, 
		const int posNum, const int blockCount, const int interBlockHalfGap, const int compartLineThickness);
	// 구분선을 그리기위해서 해당 block의 위치를 가져온다.
	static CPoint GetAbsolutePoint( const CPoint& absoluteOrgPt, const CList< double, double>& rowList, const CList< double, double>& columnList, const int xIndex, const int yIndex);


	// 모든 block 위치를 균등으로 수정한다.
	static void InitializePositions(const CRect& newAllBlockRegion, 
		const int nDoubleSizeBlockRowNum, const int nDoubleSizeBlockColumnNum, 
		CList<int, int>& rowList, CList<int, int>& columnList, const int interBlockHalfGap, const int compartLineThickness);
	// 모든 block 기존위치에서 변경된 영역에 의해 수정한다.
	static void RecalculatePositions(const CRect& oldAllBlockRegion, const CRect& newAllBlockRegion, CList< double, double>& rowList, CList< double, double>& columnList);

	// point가 어느 구분선에 포함되는 지를 찾기.
	static bool FindCompartLine(const CPoint& absoluteOrgPt, const CList< double, double>& rowList, const CList< double, double>& columnList, 
		const CPoint& point, int& blockRowIndex, int& blockColumnIndex);
	// 선택된 block의 위치를 변경한다. --> 마우스에 의함.
	static bool MoveBlocksPosition( CList< double, double>& rowList, CList< double, double>& columnList, const CPoint& point, const int rowCompartLineNum, const int columnCompartLineNum);
	// block 위치 변경
	static bool MoveBlockPosition( CList< double, double>& positionList, const int sourceIndex, const int targetIndex);
	static void RecalculatePositions( CList< double, double>& positionList, const int oldAllRegionLength, const int newAllRegionLength);

private:
	static int m_nMouseClickThickness;  // 구분선 선택을 체크할 범위
	static int m_nMinBlockWidth;		// block의 최소 가로 세로 길이

	static int GetDoubleSizeBlockCount(const int blockCount, const int doubleSizeBlockNum);
	static bool IsBlockInDoubleSize(const int doubleSizeBlockNum, const int doubleSizeCount, const int curNum);

	static bool GetBlocksPosition( const CString& strPositon, CList< double, double>& positionList);
	static bool SetBlocksPosition( CList< double, double>& targetList, const CList< double, double>& sourceList);

	// Gap을 포함한 해당 위치(block) 가져오기 => 절대좌표
	static double GetAbsolutePosition( const double absoluteOrgPt, const CList< double, double>& positionList, const int nIndex);
	// 순수 block의 길이를 구한다.
	static int GetBlockLength(const int start, const int end, const int blockCount, const int gapCount, 
		const int compartLineCount, const int interBlockHalfGap, const int compartLineThickness);

	// 모든 block 위치를 균등으로 수정한다.
	static void InitializePositions(CList<int, int>& positionList, const int nDoubleSizeBlockNum, 
		const int allRegionLength, const int interBlockHalfGap, const int compartLineThickness);
	static int GetChangedPosition(const int oldAllRegionLength, const int newAllRegionLength, 
		const int oldPositionValue, const int posNum, const int blockCount);

	// point가 어느 구분선에 포함되는 지를 구한다.
	static bool AppropriateRowCompartLine(const CPoint& absoluteOrgPt, const CList< double, double>& rowList, const CList< double, double>& columnList, 
		const CPoint& point, int& rowCompartLineNum);
	static bool AppropriateColumnCompartLine(const CPoint& absoluteOrgPt, const CList< double, double>& rowList, const CList< double, double>& columnList, 
		const CPoint& point, int& columnCompartLineNum);
	static bool IsPtInRect(const CRect& rect, const CPoint& point);

	// 해당 block의 position에 값을 변경한다. --> 마우스로 인한 block 영역수정시 호출
	static bool MoveBlocksPosition( CList< double, double>& positionList, const int compartLineNum, const int modityData);
	static bool IsMoveBlockPosition(const int num, const int allCount);
	static bool GetChangedLengthInBlock( const CList< double, double>& positionList, const int sourceIndex, const int targetIndex, CList< double, double>& lengthInBlock);
	static bool GetChangedLengthInBlock(const int sourceIndex, const int targetIndex, CList< double, double>& lengthInBlock);
	static double GetData( const CList< double, double>& positionList, const int index);

};

#endif // !defined(AFX_BLOCKSPOSITIONCALCULATOR_H__C2233779_3A04_4B80_A18A_C9ECCBAF4E45__INCLUDED_)
