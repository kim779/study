#include "StdAfx.h"
#include "DataBlockList.h"

CDataBlockListUNK::~CDataBlockListUNK(void)
{
	Clear();
}

void CDataBlockListUNK::Clear()
{
	m_nItemCount = 0;
	m_nFirstBlockStartIndex = 0;
	m_nLastBlockEndIndex = 0;

	m_nIndex = 0;
	m_nBlockIndex = 0;
	m_nIndexInBlock = 0;
	m_lpBlockIndex = NULL;

	vector<LPVOID>::iterator it = m_vtBlockPointer.begin();
	while(it != m_vtBlockPointer.end())
	{
		free(*it);
		it++;
	}

	m_vtBlockPointer.clear();
}

// (2008/12/27 - Seung-Won, Bae) Commnet
//	1. What is m_nItemCount?
int CDataBlockListUNK::GetItemCount()
{
	return m_nItemCount;
}

// (2008/12/27 - Seung-Won, Bae) Commnet.
//	1. Resize Block Vector.
BOOL CDataBlockListUNK::SetItemCount(int nItemCount)
{
	// 현재 사이즈와 같은 경우
	if( nItemCount == m_nItemCount) return TRUE;

	// 현재 사이즈 보다 줄어든 경우
	if( nItemCount < m_nItemCount)
	{
		int nDecCount_Total = m_nItemCount - nItemCount;

		while( nDecCount_Total != 0)
		{
			if( nDecCount_Total < m_nLastBlockEndIndex)
			{
				m_nLastBlockEndIndex -= nDecCount_Total;
				break;
			}
			else
			{
				nDecCount_Total -= m_nLastBlockEndIndex;
				m_nLastBlockEndIndex = m_nBlockSize;

				free(m_vtBlockPointer.back());
				m_vtBlockPointer.pop_back();
			}
		}
	}
	// 현재 사이즈 보다 늘어난 경우
	else
	{
		int nIncCount_Total = nItemCount - m_nItemCount;

		while( nIncCount_Total != 0)
		{
			if( nIncCount_Total < ( m_nBlockSize - m_nLastBlockEndIndex))
			{
				m_nLastBlockEndIndex += nIncCount_Total;
				break;
			}
			else
			{
				nIncCount_Total -= ( m_nBlockSize - m_nLastBlockEndIndex);
				m_nLastBlockEndIndex = 0;

				m_vtBlockPointer.push_back( malloc( m_nBlockSize * m_nItemSize));
			}
		}
	}

	m_nItemCount = nItemCount;
	return TRUE;
}

// (2008/12/27 - Seung-Won, Bae) Comment.
//	1. What is m_nItemSize?
int CDataBlockListUNK::GetItemSize()
{
	return m_nItemSize;
}

// (2008/12/27 - Seung-Won, Bae) Comment.
//	1. What is Block Size?
int CDataBlockListUNK::GetBlockSize()
{
	return m_nBlockSize;
}

// (2008/12/27 - Seung-Won, Bae) Comment.
//	1. Delete N items from front.
//	2. Why does not manage m_nItemCount? 
/*
void CDataBlockListUNK::DeleteFront(int nCountOfItem)
{
	while(nCountOfItem != 0)
	{
		if((m_nBlockSize - m_nFirstBlockStartIndex) > nCountOfItem)
		{
			m_nFirstBlockStartIndex += nCountOfItem;
			break;
		}
		else
		{
			nCountOfItem -= (m_nBlockSize - m_nFirstBlockStartIndex);
			m_nFirstBlockStartIndex = 0;
			
			vector<LPVOID>::iterator it = m_vtBlockPointer.begin();
			if(it == m_vtBlockPointer.end()) return;

			free(*it);
			m_vtBlockPointer.erase(it);
		}
	}
}
*/

// (2008/12/27 - Seung-Won, Bae) Comment.
//	1. Delete N blocks from front.
//	2. then what happen to m_nFirstBlockStartIndex?
//	3. so it is not used.
/*
void CDataBlockListUNK::DeleteFront_Block(int nCountOfBlock)
{
	int nCount;
	for(nCount = 0; nCount < nCountOfBlock; nCount++)
	{
		vector<LPVOID>::iterator it = m_vtBlockPointer.begin();
		if(it == m_vtBlockPointer.end()) return;

		free(*it);
		m_vtBlockPointer.erase(it);
	}
}
*/

// (2008/12/27 - Seung-Won, Bae) Comment.
//	Delete items at tail.
//	What is empty state?
void CDataBlockListUNK::DeleteBack(int nCountOfItem)
{
	while( nCountOfItem != 0 && m_nItemCount > 0)
	{
		if( nCountOfItem < m_nLastBlockEndIndex)
		{
			m_nLastBlockEndIndex -= nCountOfItem;
			m_nItemCount -= nCountOfItem;
			break;
		}
		else
		{
			nCountOfItem -= m_nLastBlockEndIndex;
			m_nItemCount -= m_nLastBlockEndIndex;

			// How do you dare to be sure ? m_nFirstBlockStartIndex can be Non-Zero.
			// Or m_nFirstBlockStartIndex can not be Zero.
			m_nLastBlockEndIndex = m_nBlockSize;

			free(m_vtBlockPointer.back());
			m_vtBlockPointer.pop_back();
		}
	}
}

// (2008/12/27 - Seung-Won, Bae) Comment.
//	when is it used?
/*
void CDataBlockListUNK::DeleteBack_Block(int nCountOfBlock)
{
	int nCount;
	for(nCount = 0; nCount < nCountOfBlock; nCount++)
	{
		vector<LPVOID>::iterator it = m_vtBlockPointer.end();
		if(m_vtBlockPointer.begin() == it) return;

		it--;
		free(*it);
		m_vtBlockPointer.erase(it);

		m_nItemCount -= m_nLastBlockEndIndex;

		if(m_vtBlockPointer.size() != 0) m_nLastBlockEndIndex = m_nBlockSize;
		else							 m_nLastBlockEndIndex = 0;
	}
}
*/

// (2008/12/27 - Seung-Won, Bae) Comment
//	It is zero-based index.
LPVOID CDataBlockListUNK::GetAtUNK( int nIndex)
{
	if( nIndex < 0 || m_nItemCount <= nIndex) return NULL;
	if( m_nItemCount < 0 || m_vtBlockPointer.size() * m_nBlockSize < m_nItemCount) return NULL;
	if( m_nBlockSize <= 0) return NULL;

	int nBlockIndex = nIndex / m_nBlockSize;
	int nIndexInBlock = nIndex % m_nBlockSize;

	if( m_vtBlockPointer.size() <= nBlockIndex) return NULL;

	LPVOID lpBlock = m_vtBlockPointer[ nBlockIndex];
	if( !lpBlock) return NULL;
	return ( PBYTE)lpBlock + ( nIndexInBlock * m_nItemSize);
}

// (2008/12/27 - Seung-Won, Bae) Comment
BOOL CDataBlockListUNK::SetAtUNK(int nIndex, LPVOID Item)
{
	if( m_nItemCount <= nIndex) return FALSE;
	
	if( nIndex < ( m_nBlockSize - m_nFirstBlockStartIndex))
	{
		memcpy( ( PBYTE)m_vtBlockPointer.front() + ( ( m_nFirstBlockStartIndex + nIndex) * m_nItemSize), Item, m_nItemSize);
		return TRUE;
	}

	nIndex -= ( m_nBlockSize - m_nFirstBlockStartIndex);
	int nBlockIndex = nIndex / m_nBlockSize;
	int nIndexInBlock = nIndex % m_nBlockSize;

	// (2009/8/13 - Seung-Won, Bae) Check range.
	if( m_vtBlockPointer.size() <= nBlockIndex + 1) return FALSE;
	LPVOID lpBlock = m_vtBlockPointer[ nBlockIndex + 1];

	memcpy( ( PBYTE)lpBlock + ( nIndexInBlock * m_nItemSize), Item, m_nItemSize);
	return TRUE;
}

// (2008/12/27 - Seung-Won, Bae) Comment
//	Set Iterator Index.
void CDataBlockListUNK::SetIndex(int nIndex)
{
	if( m_nItemCount <= nIndex) return;

	m_nIndex = nIndex;

	nIndex += m_nFirstBlockStartIndex;
	m_nBlockIndex = nIndex / m_nBlockSize;

	m_nIndexInBlock = nIndex % m_nBlockSize;

	if( ( m_nBlockSize - m_nFirstBlockStartIndex) > nIndex)
			m_lpBlockIndex = m_vtBlockPointer.front();
	else
	{
		// (2009/8/13 - Seung-Won, Bae) Check range.
		if( m_vtBlockPointer.size() <= m_nBlockIndex)
				m_lpBlockIndex = NULL;
		else	m_lpBlockIndex = m_vtBlockPointer[ m_nBlockIndex];
	}
}

int CDataBlockListUNK::GetIndex()
{
	return m_nIndex;
}

// (2008/12/27 - Seung-Won, Bae) Comment
//	Get Current UNK and Move index to next.
LPVOID CDataBlockListUNK::GetNextUNK()
{
	// ? How do you believe m_lpBlockIndex is not NULL?
	if( !m_lpBlockIndex || m_nIndexInBlock < 0 || m_nBlockSize <= m_nIndexInBlock) return NULL;
	if( m_nItemCount <= m_nIndex || m_nIndex < 0) return NULL;
	LPVOID lpResult = ( ( PBYTE)m_lpBlockIndex + ( m_nIndexInBlock * m_nItemSize));

	m_nIndex++;
	m_nIndexInBlock++;
	if( m_nBlockSize <= m_nIndexInBlock)
	{
		m_nBlockIndex++;
		m_nIndexInBlock = 0;
		
		// (2009/8/13 - Seung-Won, Bae) Check range.
		if( m_vtBlockPointer.size() <= m_nBlockIndex)
				m_lpBlockIndex = NULL;
		else	m_lpBlockIndex = m_vtBlockPointer[ m_nBlockIndex];
	}

	return lpResult;
}

// (2008/12/27 - Seung-Won, Bae) Comment
//	Get Current UNK and Move index to prev.
LPVOID CDataBlockListUNK::GetPrevUNK()
{
	// ? How do you believe m_lpBlockIndex is not NULL?
	if( !m_lpBlockIndex || m_nIndexInBlock < 0 || m_nBlockSize <= m_nIndexInBlock) return NULL;
	if( m_nItemCount <= m_nIndex || m_nIndex < 0) return NULL;
	LPVOID lpResult = ( ( PBYTE)m_lpBlockIndex + ( m_nIndexInBlock * m_nItemSize));

	m_nIndex--;
	m_nIndexInBlock--;
	if( m_nIndexInBlock < 0)
	{
		m_nIndexInBlock = m_nBlockSize - 1;
		m_nBlockIndex--;
		if( m_nBlockIndex < 0)	m_lpBlockIndex = NULL;
		else
		{
			// (2009/8/13 - Seung-Won, Bae) Check range.
			if( m_vtBlockPointer.size() <= m_nBlockIndex)
					m_lpBlockIndex = NULL;
			else	m_lpBlockIndex = m_vtBlockPointer[ m_nBlockIndex];
		}
	}

	return lpResult;
}

// (2009/1/10 - Seung-Won, Bae) Support external cursor.
CDataBlockListUNK::DblCursor::DblCursor()
{
	m_nItemCount	= 0;
	m_nIndex		= -1;
	m_nBlockIndex	= -1;
	m_lpBlockIndex	= NULL;
	m_nIndexInBlock	= -1;
}
void CDataBlockListUNK::GetDblCursorUNK( int nIndex, int nZeroDTI, DblCursor &p_dblCursor)
{
	nIndex -= nZeroDTI;

	if( m_nItemCount <= nIndex || nIndex < 0) return;

	p_dblCursor.m_nItemCount = m_nItemCount;
	p_dblCursor.m_nIndex = nIndex;

	nIndex += m_nFirstBlockStartIndex;
	p_dblCursor.m_nBlockIndex = nIndex / m_nBlockSize;

	p_dblCursor.m_nIndexInBlock = nIndex % m_nBlockSize;

	if( ( m_nBlockSize - m_nFirstBlockStartIndex) > nIndex)
			p_dblCursor.m_lpBlockIndex = m_vtBlockPointer.front();
	else
	{
		// (2009/8/13 - Seung-Won, Bae) Check range.
		if( m_vtBlockPointer.size() <= p_dblCursor.m_nBlockIndex)
				p_dblCursor.m_lpBlockIndex = NULL;
		else	p_dblCursor.m_lpBlockIndex = m_vtBlockPointer[ p_dblCursor.m_nBlockIndex];
	}
}
LPVOID CDataBlockListUNK::GetAtUNK( DblCursor &p_dblCursor)
{
	if( 0 == p_dblCursor.m_nItemCount || p_dblCursor.m_nItemCount <= p_dblCursor.m_nIndex || p_dblCursor.m_nIndex < 0) return NULL;
	if( !p_dblCursor.m_lpBlockIndex || p_dblCursor.m_nIndexInBlock < 0 || m_nBlockSize <= p_dblCursor.m_nIndexInBlock) return NULL;
	return ( ( PBYTE)p_dblCursor.m_lpBlockIndex + ( p_dblCursor.m_nIndexInBlock * m_nItemSize));
}
