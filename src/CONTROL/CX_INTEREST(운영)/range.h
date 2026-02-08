#if !defined(AFX_RANGE_H__INCLUDED_)
#define AFX_RANGE_H__INCLUDED_

#pragma once

class CIdCell
{
// Attributes
public:
	int row, col;

// Operations
public:
	CIdCell(int nRow = -1, int nCol = -1) : row(nRow), col(nCol) {}
	void SetItem(int nRow = -1, int nCol = -1)
	{
		row = nRow;	col = nCol;	
	}
	int IsValid() const { return (row >= 0 && col >= 0); }
	int operator==(const CIdCell& rhs)    { return (row == rhs.row && col == rhs.col); }
	int operator!=(const CIdCell& rhs)    { return !operator==(rhs); }
};

class CRangeCell
{
// Attribute
protected:
	int	m_nMinRow;
	int	m_nMinCol;
	int	m_nMaxRow;
	int	m_nMaxCol;

// Operations
public:
	CRangeCell(int nMinRow = -1, int nMinCol = -1, int nMaxRow = -1, int nMaxCol = -1)
	{
		Set(nMinRow, nMinCol, nMaxRow, nMaxCol);
	}

	void	Set(int nMinRow = -1, int nMinCol = -1, int nMaxRow = -1, int nMaxCol = -1);

	int	IsValid() const;
	int	InRange(int row, int col) const;
	int	InRange(const CIdCell& idCell) const;
	int	Count()
	{ 
		return (m_nMaxRow - m_nMinRow +1) * (m_nMaxCol - m_nMinCol +1);
	}

	CIdCell	   GetTopLeft() const;
	CRangeCell Intersect(const CRangeCell& rhs) const;

	int	GetMinRow() const { return m_nMinRow; }
	void	SetMinRow(int minRow) { m_nMinRow = minRow; }

	int	GetMinCol() const { return m_nMinCol; }
	void	SetMinCol(int minCol) { m_nMinCol = minCol; }

	int	GetMaxRow() const { return m_nMaxRow; }
	void	SetMaxRow(int maxRow) { m_nMaxRow = maxRow; }

	int	GetMaxCol() const { return m_nMaxCol; }
	void	SetMaxCol(int maxCol) { m_nMaxCol = maxCol; }

	int  operator==(const CRangeCell& rhs);
	int  operator!=(const CRangeCell& rhs);

};

inline void CRangeCell::Set(int minRow, int minCol, int maxRow, int maxCol)
{
	m_nMinRow = minRow;
	m_nMinCol = minCol;
	m_nMaxRow = maxRow;
	m_nMaxCol = maxCol;
}

inline int CRangeCell::IsValid() const
{
	return (m_nMinRow >= 0 && m_nMinCol >= 0 && m_nMaxRow >= 0 && m_nMaxCol >= 0 &&
		m_nMinRow <= m_nMaxRow && m_nMinCol <= m_nMaxCol);
}

inline int CRangeCell::InRange(int row, int col) const
{
	return (row >= m_nMinRow && row <= m_nMaxRow && col >= m_nMinCol && col <= m_nMaxCol);
}

inline int CRangeCell::InRange(const CIdCell& idCell) const
{
	return InRange(idCell.row, idCell.col);
}

inline CIdCell CRangeCell::GetTopLeft() const
{
	return CIdCell(m_nMinRow, m_nMinCol);
}

inline CRangeCell CRangeCell::Intersect(const CRangeCell& rhs) const
{
	return CRangeCell(max(m_nMinRow,rhs.m_nMinRow), max(m_nMinCol,rhs.m_nMinCol),
			min(m_nMaxRow,rhs.m_nMaxRow), min(m_nMaxCol,rhs.m_nMaxCol));
}

inline int CRangeCell::operator==(const CRangeCell& rhs)
{
	return ((m_nMinRow == rhs.m_nMinRow) && (m_nMinCol == rhs.m_nMinCol)
		&& (m_nMaxRow == rhs.m_nMaxRow) && (m_nMaxCol == rhs.m_nMaxCol));
}

inline int CRangeCell::operator!=(const CRangeCell& rhs)
{
	return !operator==(rhs);
}

#endif // !defined(AFX_RANGE_H__INCLUDED_)