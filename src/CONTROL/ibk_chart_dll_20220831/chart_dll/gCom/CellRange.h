#if !defined(AFX_CELLRANGE_H__F86EF761_725A_11D1_ABBA_00A0243D1382__INCLUDED_)
#define AFX_CELLRANGE_H__F86EF761_725A_11D1_ABBA_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif

class CCellID
{    
public:
	int row, col;
public:
	CCellID(int nRow = -1, int nCol = -1) : row(nRow), col(nCol) {}
	int IsValid() const { return (row >= 0 && col >= 0); }
	int operator==(const CCellID& rhs)    { return (row == rhs.row && col == rhs.col); }
	int operator!=(const CCellID& rhs)    { return !operator==(rhs); }
};

class CCellRange
{ 
public:
	CCellRange(int nMinRow = -1, int nMinCol = -1, int nMaxRow = -1, int nMaxCol = -1)
	{
	Set(nMinRow, nMinCol, nMaxRow, nMaxCol);
	}

	void Set(int nMinRow = -1, int nMinCol = -1, int nMaxRow = -1, int nMaxCol = -1);

	int  IsValid() const;
	int  InRange(int row, int col) const;
	int  InRange(const CCellID& cellID) const;
	int  Count() { return (m_nMaxRow - m_nMinRow + 1) * (m_nMaxCol - m_nMinCol + 1); }

	CCellID  GetTopLeft() const;
	CCellRange  Intersect(const CCellRange& rhs) const;

	int GetMinRow() const {return m_nMinRow;}
	void SetMinRow(int minRow) {m_nMinRow = minRow;}

	int GetMinCol() const {return m_nMinCol;}
	void SetMinCol(int minCol) {m_nMinCol = minCol;}

	int GetMaxRow() const {return m_nMaxRow;}
	void SetMaxRow(int maxRow) {m_nMaxRow = maxRow;}

	int GetMaxCol() const {return m_nMaxCol;}
	void SetMaxCol(int maxCol) {m_nMaxCol = maxCol;}

	int GetRowSpan() const {return m_nMaxRow - m_nMinRow + 1;}
	int GetColSpan() const {return m_nMaxCol - m_nMinCol + 1;}

	int  operator==(const CCellRange& rhs);
	int  operator!=(const CCellRange& rhs);

protected:
	int m_nMinRow;
	int m_nMinCol;
	int m_nMaxRow;
	int m_nMaxCol;
};

#endif // !defined(AFX_CELLRANGE_H__F86EF761_725A_11D1_ABBA_00A0243D1382__INCLUDED_)
