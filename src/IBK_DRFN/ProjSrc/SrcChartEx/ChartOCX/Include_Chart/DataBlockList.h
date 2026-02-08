#pragma once
#pragma warning(disable: 4786)

#include <vector>
using namespace std;

#define COUNT_BLOCK_POINTER_TABLE_DEFAULT	10
#define SIZE_BLOCK_DEFAULT					512

// (2008/12/27 - Seung-Won, Bae) Comment.
//	1. CDataBlockListUNK is Data Block Manager or Data Block Object ?
class CDataBlockListUNK
{
public:
	CDataBlockListUNK()
	{
		m_nItemSize = sizeof(double);
		m_nBlockSize = SIZE_BLOCK_DEFAULT;
//		m_vtBlockPointer.reserve(COUNT_BLOCK_POINTER_TABLE_DEFAULT);
	}

	CDataBlockListUNK(int nItemSize, int nBlockSize)
	{
		m_nItemSize = nItemSize;
		m_nBlockSize = nBlockSize;

		m_nItemCount = 0;
		m_nFirstBlockStartIndex = 0;
		m_nLastBlockEndIndex = 0;

		m_nIndex = 0;
		m_nBlockIndex = 0;
		m_nIndexInBlock = 0;
		m_lpBlockIndex = NULL;

//		m_vtBlockPointer.reserve(COUNT_BLOCK_POINTER_TABLE_DEFAULT);
	}

public:
	virtual ~CDataBlockListUNK(void);

// 내부변수
protected:
													// [Block Vector Structure Info]
	int m_nItemSize;								// Item Size
	int m_nBlockSize;								// Item Max Count in Block

													// [Current Item Info]
	int m_nItemCount;								// Item Count
	int m_nFirstBlockStartIndex;					// the Index of the first item in first block. (Zero Based)
													//		and It can not be Non-Zero. It is always zero.
	int m_nLastBlockEndIndex;						// the End Index for new item in Last Block.
													//		or item count in Last Block.
													
													// [Iterator Index]
	int m_nIndex;									// Full Index
	int m_nBlockIndex;								// Block Index for m_nIndex
	LPVOID m_lpBlockIndex;							// Block Pointer of Block Index-th
	int m_nIndexInBlock;							// Partial Index in m_nBlockIndex

	vector<LPVOID>	m_vtBlockPointer;				// the Vector of Data Array Block

// 제공변수
public:

// 내부함수
protected:

// 제공함수
public:
	void	Clear();									// 전체 데이터를 초기화 합니다.

	BOOL	SetItemCount(int nSize);					// 데이터 갯수를 설정 합니다.
	int		GetItemCount();								// 데이터 갯수를 읽어 옵니다.

	int		GetItemSize();								// 아이템 사이즈를 읽어옵니다.
	int		GetBlockSize();								// 블록 사이즈를 읽어옵니다.	- 메모리 블록 사이즈는 Item Size * Block Size 입니다.

//	void	DeleteFront(int nCountOfItem = 1);			// 앞쪽에서 아이템을 제거합니다.
//	void	DeleteFront_Block(int nCountOfItem = 1);	// 앞쪽에서 블록이 제거됩니다.	- 블록은 반드시 블록 사이즈 만큼 제거되는 것이 아니라 메모리 구조상의 블록이 제거됩니다.

	void	DeleteBack(int nCount = 1);					// 뒤쪽에서 아이템을 제거합니다.
//	void	DeleteBack_Block(int nCount = 1);			// 뒤쪽에서 블록이 제거됩니다. - 블록은 반드시 블록 사이즈 만큼 제거되는 것이 아니라 메모리 구조상의 블록이 제거됩니다.

	inline void	PushBackUNK(LPVOID Item = NULL)			// 뒤쪽에 아이템을 추가합니다.
				{
					if( m_nBlockSize < m_nLastBlockEndIndex) return;

					LPVOID lpLastBlock = NULL;
					if(m_nItemCount == 0 || m_nLastBlockEndIndex == m_nBlockSize)
					{
						m_nLastBlockEndIndex = 0;

						lpLastBlock = malloc(m_nBlockSize * m_nItemSize);
						m_vtBlockPointer.push_back(lpLastBlock);
					}
					else lpLastBlock =  m_vtBlockPointer.back();

					if( Item && lpLastBlock) 
						memcpy( ( ( PBYTE)lpLastBlock + ( m_nLastBlockEndIndex * m_nItemSize)), Item, m_nItemSize);

					m_nLastBlockEndIndex++;
					m_nItemCount++;
				}

	BOOL	SetAtUNK(int nIndex, LPVOID Item);			// 특정 위치에 아이템을 설정합니다.
	LPVOID	GetAtUNK(int nIndex);						// 특정 위치에 아이템을 읽어옵니다.

	void	SetIndex(int nIndex);						// 현재 위치를 설정 합니다.
	int		GetIndex();									// 현재 위치를 읽어 옵니다.

	LPVOID	GetNextUNK();								// 현재 위치에서 다음번 위치를 읽습니다.
	LPVOID	GetPrevUNK();								// 현재 위치에서 앞쪽 위치를 읽습니다.

// (2009/1/10 - Seung-Won, Bae) Support external cursor.
public:
	class DblCursor
	{
	friend class CDataBlockListUNK;
	protected:
		int		m_nItemCount;
	public:
		int		m_nIndex;									// Full Index
		int		m_nBlockIndex;								// Block Index for m_nIndex
		LPVOID	m_lpBlockIndex;								// Block Pointer of Block Index-th
		int		m_nIndexInBlock;							// Partial Index in m_nBlockIndex
	public:
				DblCursor();
		BOOL	IsEnd( void)	{	return	m_nIndex < m_nItemCount;	}
	};
	void			GetDblCursorUNK( int nIndex, int nZeroDTI, DblCursor &p_dblCursor);
	LPVOID			GetAtUNK( DblCursor &p_dblCursor);
	inline LPVOID	GetNextUNK( DblCursor &p_dblCursor)
					{
						if( 0 == p_dblCursor.m_nItemCount || p_dblCursor.m_nItemCount <= p_dblCursor.m_nIndex || p_dblCursor.m_nIndex < 0) return NULL;

						// ? How do you believe m_lpBlockIndex is not NULL?
						if( !p_dblCursor.m_lpBlockIndex || p_dblCursor.m_nIndexInBlock < 0 || m_nBlockSize <= p_dblCursor.m_nIndexInBlock) return NULL;
						LPVOID lpResult = ( ( PBYTE)p_dblCursor.m_lpBlockIndex + ( p_dblCursor.m_nIndexInBlock * m_nItemSize));

						p_dblCursor.m_nIndex++;
						p_dblCursor.m_nIndexInBlock++;
						if( m_nBlockSize <= p_dblCursor.m_nIndexInBlock)
						{
							p_dblCursor.m_nBlockIndex++;
							p_dblCursor.m_nIndexInBlock = 0;
							p_dblCursor.m_lpBlockIndex = m_vtBlockPointer[ p_dblCursor.m_nBlockIndex];
						}

						return lpResult;
					}
};

template <typename T, int nBlockSize = SIZE_BLOCK_DEFAULT> class CDataBlockList : public CDataBlockListUNK
{
public:
	CDataBlockList(void) :CDataBlockListUNK(sizeof(T), nBlockSize)
	{
	}

public:
	virtual ~CDataBlockList(void)
	{
	}

	LPVOID PushBack(T* pItem)
	{
		PushBackUNK(pItem);
	}

	inline void PushBack(T Item)
	{
		PushBackUNK(&Item);
	}

	BOOL SetAt(int nIndex, T *pItem)
	{
		return SetAtUNK(nIndex, pItem);
	}

	BOOL SetAt(int nIndex, T Item)
	{
		return SetAtUNK(nIndex, &Item);
	}

	T& operator[](int nIndex)
	{
		T* pItem = (T*)GetAtUNK(nIndex);

		return *pItem;
	}

// (2008/12/28 - Seung-Won, Bae) Do not use '&'.
//		How do you know the end of list?
/*
	T& GetPrev()
	{
		T* pItem = (T*)GetPrevUNK();

		return *pItem;
	}
*/
	T* GetPrev()	{	return (T*)GetPrevUNK();	}

// (2008/12/28 - Seung-Won, Bae) Do not use '&'.
//		How do you know the validity of index ?
/*
	T& GetAt(int nIndex)
	{
		T* pItem = (T*)GetAtUNK(nIndex);

		return *pItem;
	}
*/
	T*	GetAt( int nIndex)	{	return ( T*)GetAtUNK( nIndex);	}

// (2008/12/28 - Seung-Won, Bae) Do not use '&'.
//		How do you know the end of list?
/*
	T& GetNext()
	{
		T* pItem = (T*)GetNextUNK();

		return *pItem;
	}
*/
	T* GetNext()	{	return ( T*)GetNextUNK();	}

	// (2009/1/10 - Seung-Won, Bae) Support external cursor.
	void			GetCursor( int nIndex, int nZeroDTI, DblCursor &p_dblCursor)	{	GetDblCursorUNK( nIndex, nZeroDTI, p_dblCursor);	}
	T *				GetAt( DblCursor &p_dblCursor)									{	return ( T *)GetAtUNK( p_dblCursor);				}
	inline T *		GetNext( DblCursor &p_dblCursor)								{	return ( T *)GetNextUNK( p_dblCursor);				}
};