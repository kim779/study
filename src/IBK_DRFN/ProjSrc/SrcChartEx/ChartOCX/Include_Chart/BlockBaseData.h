// BlockBaseData.h: interface for the CBlockBaseData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCKBASEDATA_H__63D0F4CD_D53A_40CE_8D29_2579A78A9F7D__INCLUDED_)
#define AFX_BLOCKBASEDATA_H__63D0F4CD_D53A_40CE_8D29_2579A78A9F7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// (2004.10.12, 배승원) BlockBaseEnum.h로 Enum Data와 같이 Define 상수 만을 별도의 Header로 분리한다.
//		(DLL 등 외부 Module 개발자와 공유하기 위함)
#include "BlockBaseEnum.h"

class CBlockIndex
{
public:
	CBlockIndex()										{	m_nRow = m_nColumn = -1;				}
	CBlockIndex( const int row, const int column)		{	m_nRow = row;	m_nColumn = column;		}
	
	void	SetRow( const int row)						{	m_nRow = row;							}
	void	SetColumn( const int column)				{	m_nColumn = column;						}
	void	SetIndex( const int row, const int column)	{	m_nRow = row;	m_nColumn = column;		}
	void	OffSet( const int rowOffSet, const int columnOffSet)
														{	m_nRow += rowOffSet;	m_nColumn += columnOffSet;	}

	int		GetRow( void) const							{	return m_nRow;							}
	int		GetColumn( void) const						{	return m_nColumn;						}
	bool	IsAllValueBiggerThanZero( void) const		{	return ( m_nRow >= 0 && m_nColumn >= 0);	}
	BOOL	IsSameIndex( const CBlockIndex &p_biTarget)	{	return ( m_nRow == p_biTarget.m_nRow && m_nColumn == p_biTarget.m_nColumn);	}

private:
	int m_nRow; // 행
	int m_nColumn; // 열
};

// graph style 저장
class CGraphStyle
{
public:
	CGraphStyle() { m_nStyle = -1; };
	CGraphStyle(const int style) { m_nStyle = style; };

	void SetGraphStyle(const int& style) { m_nStyle = style; };

	int GetGraphStyle() const { return m_nStyle; };
	CGraphBaseData::BONGSTYLE GetGraphBongStyle() const { return (CGraphBaseData::BONGSTYLE) m_nStyle; };
	CGraphBaseData::LINESTYLE GetGraphLineStyle() const { return (CGraphBaseData::LINESTYLE) m_nStyle; };
	CGraphBaseData::BARSTYLE GetGraphBarStyle() const { return (CGraphBaseData::BARSTYLE) m_nStyle; };
	CGraphBaseData::PIESTYLE GetGraphPieStyle() const { return (CGraphBaseData::PIESTYLE) m_nStyle; };
	CGraphBaseData::SPECIALSTYLE GetGraphSpecialStyle() const { return (CGraphBaseData::SPECIALSTYLE) m_nStyle; };
	CGraphBaseData::FUNDSTYLE GetGraphFundStyle() const { return (CGraphBaseData::FUNDSTYLE) m_nStyle; };
	CGraphBaseData::DATAVIEWSTYLE GetGraphDataViewStyle() const { return (CGraphBaseData::DATAVIEWSTYLE) m_nStyle; };
	CGraphBaseData::FUOPSTYLE GetGraphFuOpStyle() const { return (CGraphBaseData::FUOPSTYLE) m_nStyle; };

private:
	int m_nStyle;
};

// Graph Draw Style
class CGraphDrawStyle
{
public:
	CGraphDrawStyle() { m_nStyle = -1; };
	CGraphDrawStyle(const int style) { m_nStyle = style; };

	void SetDrawStyle(const int& style) { m_nStyle = style; };

	int GetDrawStyle() const { return m_nStyle; };
	CGraphBaseData::BONGDRAWSTYLE GetGraphBongDrawStyle() const { return (CGraphBaseData::BONGDRAWSTYLE) m_nStyle; };
	CGraphBaseData::LINEDRAWSTYLE GetGraphLineDrawStyle() const { return (CGraphBaseData::LINEDRAWSTYLE) m_nStyle; };
	CGraphBaseData::BARDRAWSTYLE GetGraphBarDrawStyle() const { return (CGraphBaseData::BARDRAWSTYLE) m_nStyle; };
	CGraphBaseData::PIEDRAWSTYLE GetGraphPieDrawStyle() const { return (CGraphBaseData::PIEDRAWSTYLE) m_nStyle; };
	CGraphBaseData::DATABARDRAWSTYLE GetGraphDataBarDrawStyle() const { return (CGraphBaseData::DATABARDRAWSTYLE) m_nStyle; };
	CGraphBaseData::DATASIGNALDRAWSTYLE GetGraphDataSignalDrawStyle() const { return (CGraphBaseData::DATASIGNALDRAWSTYLE) m_nStyle; };
	CGraphBaseData::COMPARTTEXTDRAWSTYLE GetGraphCompartTextDrawStyle() const { return (CGraphBaseData::COMPARTTEXTDRAWSTYLE) m_nStyle; };

private:
	int m_nStyle;
};

// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
class CGraphPaintStyle
{
public:
	CGraphPaintStyle() { m_nStyle = 1; };
	CGraphPaintStyle(const int style) { m_nStyle = style; };

	void SetPaintStyle(const int& style) { m_nStyle = style; };

	int GetPaintStyle() const { return m_nStyle; };
	CGraphBaseData::BARPAINTSTYLE GetGraphBarPaintStyle() const { return (CGraphBaseData::BARPAINTSTYLE) m_nStyle; };

private:
	int m_nStyle;
};

// 기준선 두께, 모양 설정 - ojtaso (20071213)
class CGraphBaseLineStyle
{
public:
	CGraphBaseLineStyle() { m_nStyle = 0; }
	CGraphBaseLineStyle(const int style) { m_nStyle = style; }
	
	void SetBaseLineStyle(const int& style) { m_nStyle = style; }

	int GetBaseLineStyle() const { return m_nStyle; }
	CGraphBaseData::LINEDRAWSTYLE GetBaseLineDrawStyle() const { return (CGraphBaseData::LINEDRAWSTYLE) m_nStyle; };

private:
	int m_nStyle;
};

// ----------------------------------------------------------------------------
// block에서 ocx에 메시지 줄때 data
class CBlockBehaviorData
{
public:
	CBlockBehaviorData(const CBlockBaseData::BLOCKBEHAVIOR eBlockBehavior, const CBlockIndex& sourceBlockIndex)
	{
		m_eBlockBehavior = eBlockBehavior;
		m_sourceBlockIndex = sourceBlockIndex;
	};

	CBlockBehaviorData(const CBlockBaseData::BLOCKBEHAVIOR eBlockBehavior, const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex)
	{
		m_eBlockBehavior = eBlockBehavior;
		m_sourceBlockIndex = sourceBlockIndex;
		m_targetBlockIndex = targetBlockIndex;
	};

	void SetSourceBlockIndex(const CBlockIndex& blockIndex) { m_sourceBlockIndex = blockIndex; };
	void SetTargetBlockIndex(const CBlockIndex& blockIndex) { m_targetBlockIndex = blockIndex; };

	CBlockBaseData::BLOCKBEHAVIOR GetBlockBehavior() const { return m_eBlockBehavior; };
	CBlockIndex GetSourceBlockIndex() const { return m_sourceBlockIndex; };
	CBlockIndex GetTargetBlockIndex() const { return m_targetBlockIndex; };

private:
	CBlockBaseData::BLOCKBEHAVIOR m_eBlockBehavior;
	CBlockIndex m_sourceBlockIndex;
	CBlockIndex m_targetBlockIndex;
};

#endif // !defined(AFX_BLOCKBASEDATA_H__63D0F4CD_D53A_40CE_8D29_2579A78A9F7D__INCLUDED_)
