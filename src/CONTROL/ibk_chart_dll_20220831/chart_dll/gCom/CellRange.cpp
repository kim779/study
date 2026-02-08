#include "stdafx.h"
#include "cellRange.h"

void CCellRange::Set(int minRow, int minCol, int maxRow, int maxCol)
{
     m_nMinRow = minRow;
     m_nMinCol = minCol;
     m_nMaxRow = maxRow;
     m_nMaxCol = maxCol;
}

int CCellRange::operator==(const CCellRange& rhs)
{
     return ((m_nMinRow == rhs.m_nMinRow) && (m_nMinCol == rhs.m_nMinCol) &&
             (m_nMaxRow == rhs.m_nMaxRow) && (m_nMaxCol == rhs.m_nMaxCol));
}

int CCellRange::operator!=(const CCellRange& rhs)
{
     return !operator==(rhs);
}

int CCellRange::IsValid() const
{
     return (m_nMinRow >= 0 && m_nMinCol >= 0 && m_nMaxRow >= 0 && m_nMaxCol >= 0 &&
             m_nMinRow <= m_nMaxRow && m_nMinCol <= m_nMaxCol);
}

int CCellRange::InRange(int row, int col) const
{
     return (row >= m_nMinRow && row <= m_nMaxRow && col >= m_nMinCol && col <= m_nMaxCol);
}

int CCellRange::InRange(const CCellID& cellID) const
{
     return InRange(cellID.row, cellID.col);
}

CCellID CCellRange::GetTopLeft() const
{
     return CCellID(m_nMinRow, m_nMinCol);
}

CCellRange CCellRange::Intersect(const CCellRange& rhs) const
{
     return CCellRange(max(m_nMinRow,rhs.m_nMinRow), max(m_nMinCol,rhs.m_nMinCol),
                       min(m_nMaxRow,rhs.m_nMaxRow), min(m_nMaxCol,rhs.m_nMaxCol));
}
